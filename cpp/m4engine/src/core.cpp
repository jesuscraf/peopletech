//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             core.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "eng.h"
#include "engdf.inl"

#include "mitdates.h"
#include "analex.h"
#include "clreparg.h"

#include "core.hmg"
#include "core.h"

//============================================================================ClENG_Container

ClENG_Container::ClENG_Container()
{
	m_poInsp=GET_INSP();
	m_poReportEnv=NULL ;
	m_State=NOT_INITIALIZED ;
}

void ClENG_Container::End()
{
	FieldList_t::iterator It;

	for ( It=m_oField.begin() ; It != m_oField.end(); ++It) {
		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, *It ) ;
	} ;
	m_oField.clear() ;

	m_oRegion.End() ;

	m_State=NOT_INITIALIZED ;
}

m4bool_t ClENG_Container::PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_Analex Analex ;
	FieldList_t::iterator It;

	for ( It=m_oField.begin() ; It != m_oField.end(); ++It) {
		ai_pstEnv->m_iLayoutPage= m_iPage ;
		if ( ! (*It)->PreAnalex(ai_pstEnv) ) SwOk=M4_FALSE ;
	} ;

	SwOk = M4_BOOL(SwOk && Analex.Init( ai_pstEnv, & m_oCancel, ClMIT_Trace::PROPAGATE_ERROR ));

	m_State=WAITING_FOR_PRELOAD ;

	return SwOk ;
}

void ClENG_Container::InitFixed( 	
	StENG_ReportEnv *ai_poReportEnv,
	m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
	DvcUnit_t ai_Height, DvcUnit_t ai_Width,
	const m4char_t * ai_pcCancel) 
{
	InitNotFixed(ai_poReportEnv,ai_iPage, ai_X, ai_Height, ai_Width,
		ai_Y, 0, M4_TRUE, ai_pcCancel) ;
	m_oFlag.Switch( X_Y_FIXED, M4_TRUE);

	m_State=INITIALIZED ;
}

void ClENG_Container::InitNotFixed( 	
		StENG_ReportEnv *ai_poReportEnv,
		m4int16_t ai_iPage, DvcUnit_t ai_X, 
		DvcUnit_t ai_Height, DvcUnit_t ai_Width,
		DvcUnit_t ai_Y, DvcUnit_t ai_SizeYLimit,
		m4bool_t ai_SwTogether, const m4char_t * ai_pcCancel ) 
{
	m4uint16_t uiActualPage;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc ;

	ClENG_Container::End();

	m_poReportEnv = ai_poReportEnv ;

	Dvc=m_poReportEnv->m_poDvc ;
	uiActualPage = ai_iPage + 1;

	if ( !M4_IN_RANGE(ai_iPage,0,Dvc->GetNumLayoutPages()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_10 ] ) 
			<< uiActualPage << (m4int16_t)(Dvc->GetNumLayoutPages()) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	m_oFlag.Switch( TOGETHER, ai_SwTogether );
	m_oFlag.Switch( X_Y_FIXED, M4_FALSE);
	m_iPage=ai_iPage; m_X=ai_X; m_Y=ai_Y; m_Width=ai_Width; 
	m_HeightBackup=m_Height=ai_Height;
	m_SizeYLimit=ai_SizeYLimit;
	m_oCancel.Init( ai_pcCancel, "CancelContainer" ) ;
	m_HeightPrinted = 0 ;
	m_iAttemp=0;

	//No se pone Y: lo mas normal es que varie por impresion, asi que lo pondremos cada vez
	//Sin embargo, X no varia

	m_oRegion.Init( Dvc, 0, m_X, m_Width, m_Height ) ;

	m_State=INITIALIZED ;
}

#define ENG_ROUND_DVC_MARGIN 10

void ClENG_Container::InitField( ClENG_Field *ai_poField ) 
{
	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ CORE_MSG_250 ] ) 
		<< ai_poField->GetIdDesc() << END_LOC ;

//	if ( ai_poField->GetTop()+ai_poField->GetHeight()-1 > m_Height-1 ||
//		ai_poField->GetLeft()+ai_poField->GetWidth()-1 > m_Width-1	
//		) {
	//Se dá un margen de 10 Dvc para dar error, debido a que hay calculos de 0.2, etc, que el
	//compiler? copro? redondea mal, y provoca warnings falsos.
	if ( ai_poField->GetTop()+ai_poField->GetHeight()-1 > m_Height-1 + ENG_ROUND_DVC_MARGIN ||
		ai_poField->GetLeft()+ai_poField->GetWidth()-1 > m_Width-1+ ENG_ROUND_DVC_MARGIN	
		) {
	
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_40 ] ) << SEND_MSG ;
		//Efecto local
	} ;
	m_oField.push_back(ai_poField) ;

	*m_poInsp->m_poTrace << POP_LOC ;

	m_State=LOADING_FIELDS ;
}

void ClENG_Container::PrintPreload( m4bool_t ai_bOptimize ) 
{
	ClENG_Field *F ;
	FieldList_t::iterator It,Next;

	for ( It=m_oField.begin() ; It != m_oField.end(); ) {
		F=*It ;
		if ( F->IsStatic() && ( ai_bOptimize ? F->IsOptimize() : (m4bool_t)! F->IsOptimize() ) ) {
			F->NewPrint() ;
			F->Print(m_iPage,m_X,m_Y,m_Height,0) ;
			M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_FIELD, F ) ;
			Next=It;  ++Next ;
			m_oField.erase(It) ; 
			It=Next ;	//erase inhabilita un ++It posterior
		} else ++It ;
	} ;

	m_State=PRINTED ;
}

void ClENG_Container::NewPrint( ) 
{
	FieldList_t::iterator It ;
	ClENG_Field *F ;
	DvcUnit_t AuxVarHeight ;

	//Reseteos

	m_HeightPrinted = 0 ;
	m_iAttemp = 0 ;

	//Test cancelacion 

	if ( ClENG_VR::ExecuteFilter(&m_oCancel) ) {
		m_State = PRINTED ;
		return  ;
	} ;

	//Pasa a NewPrint sus Fields.

	for ( It=m_oField.begin() ; It != m_oField.end(); ++It ) {
		F=*It ;
		F->NewPrint() ;
	} ;

	//Si el container no es fijo, testea posible crecimiento de altura

	if ( ! IsXYFixed() ) {
 
		for ( It=m_oField.begin(), m_Height = m_HeightBackup ; It != m_oField.end(); ++It ) {
			F=*It ;
			if ( ! F->IsVariableHeight() ) continue ;
			AuxVarHeight = F->GetTop() + F->GetHeight() ;
			m_Height = M4_MAX( AuxVarHeight, m_Height ) ;
		} ;
		if ( m_oRegion.GetHeight() != m_Height ) m_oRegion.SetHeight(m_Height) ;

	} ;

	// Se realiza una nueva iteración para ajustar, si procede, el tamaño de los fields (p.ej. lineas)
	// que deben ajustarse al contenedor
	for ( It = m_oField.begin(); It != m_oField.end(); ++It) {
		F=*It ;
		if ( F->IsAdjustedToContainer())
		{
			F->EvaluateContainerHeight( m_HeightBackup, m_Height ) ;
		}
	};

	m_State=START_PRINTING ;
}

void ClENG_Container::Print() 
{
	DvcUnit_t DeviceY, DeviceInterval, CntInterval ;
	m4bool_t SwParcial ;
	ClENG_Field * Field ;
	FieldList_t::iterator It;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc  ;

	//Permite entrar solo en estados permitidos

	if ( !( GetState()==START_PRINTING || GetState()==PARTIAL_PRINTED ) ) return ;
	
	//Si no hay fields, no hace nada y sale.
	if ( IsEmpty() ) {
		m_State = PRINTED ;
		return  ;
	} ;

	//Haya maxima ventana de device imprimible, la deja en DeviceY, DeviceInterval
	//Sale si no hay sitio para imprimir, testea error por limite de intentos.

	if ( IsXYFixed() ) {
		DeviceY			= m_Y ;
		DeviceInterval	= m_Height ;
	} else {

		DvcUnit_t YBottom=m_Y+m_SizeYLimit-1, Aux ;

		Aux= Dvc->GetBaseLineY(m_iPage) ;
		DeviceY = m_Y >= Aux ? m_Y : Aux ;
		Dvc->SetBaseLineY(m_iPage,DeviceY) ;
		if ( DeviceY > YBottom ) {
			++m_iAttemp ;
			if (m_iAttemp > PRINT_ATTEMP ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_50 ] ) << m_iAttemp << SEND_MSG ;
				//Efecto local
				m_State = PRINTED ;
				return ;
			} ;
			return  ;	//Impresion vacia
		} ;
		DeviceInterval=YBottom-DeviceY+1 ;
	} ;

	//Teniendo en cuenta toda la causistica, deja:
	//	-Se imprime todo el container en una sola vez:
	//		-SwParcial=FALSE
	//		-CntInterval=Intervalo a imprimir de este container (m_Height)
	//	-Se va a imprimir un trozo de container, sea el primero u otro:
	//		-SwParcial=TRUE
	//		-CntInterval=Intervalo a imprimir de este container

	if ( GetState()==START_PRINTING ) {

		if ( m_Height <= DeviceInterval ) {

			SwParcial=M4_FALSE ;
			CntInterval = m_Height ;

		} else {
			if ( IsTogether() ) {
				++m_iAttemp ;
				if (m_iAttemp > PRINT_ATTEMP ) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_50 ] ) << m_iAttemp << SEND_MSG ;
					//Efecto local
					m_State = PRINTED ;
					return  ;
				} ;
				return  ;
			} else {
				SwParcial=M4_TRUE ;
				CntInterval=DeviceInterval ;
			} ;
		} ;

	} else {

		SwParcial = M4_TRUE ;
		CntInterval = M4_MIN( DeviceInterval, m_Height-m_HeightPrinted )  ;
	} ;

	if ( !SwParcial ) {

		//IMPRESION COMPLETA SIN PARTIR CONTAINER

		m_oRegion.Show(m_iPage, 0, DeviceY ) ; 

		// Se realiza la impresión de todos los Fields que no se deben ajustar al contenedor
		for ( It=m_oField.begin() ; It != m_oField.end(); ++It) {
			(*It)->Print(m_iPage, m_X, DeviceY, CntInterval, 0 ) ;
		} ;

		m_HeightPrinted += CntInterval ;
		
		if ( !IsXYFixed() ) Dvc->IncrBaseLineY( m_iPage, CntInterval) ;
		
		m_State = PRINTED ;

		return  ;

	} else {

		//IMPRESION DE UNA PARTE DEL CONTAINER

		m4bool_t SwAllPrinted;
		//Ajuste de intervalo por fields que no pueden partirse
		CntInterval = AdjustCntInterval(CntInterval)  ;
		if (CntInterval<=0) {
			++m_iAttemp ;
			if (m_iAttemp > PRINT_ATTEMP ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_50 ] ) << m_iAttemp << SEND_MSG ;
				//Efecto local
				m_State = PRINTED ;
				return  ;
			} ;
		} ;

		//Impresion de region y campos

		m_oRegion.Show(m_iPage,0, DeviceY, m_HeightPrinted, CntInterval  ) ; 

		for ( 
			It=m_oField.begin(), SwAllPrinted=M4_TRUE ; 
			It != m_oField.end(); 
			++It) 
		{
			Field=*It ;
			Field->Print(m_iPage, m_X, DeviceY, CntInterval, m_HeightPrinted ) ;
			if (Field->GetState() != ClENG_Field::PRINTED ) SwAllPrinted=M4_FALSE ;
		} ;

		m_HeightPrinted += CntInterval ;
		if ( ! IsXYFixed() ) Dvc->IncrBaseLineY(m_iPage, CntInterval ) ;

		if ( ! SwAllPrinted ) {

			if ( m_HeightPrinted >= m_Height ) {
				//Si el container se ha impreso y queda algún field, pasa algo raro !
				++m_iAttemp ;
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_50 ] ) 
					<< m_iAttemp << SEND_MSG ;
				//Efecto local
				m_State = PRINTED ;
				return ;
			} ;

			m_State = PARTIAL_PRINTED ;

			return  ;
			
		} else {

			m_State = PRINTED ;

			return ;
			
		} ;
	} ;

}

DvcUnit_t ClENG_Container::AdjustCntInterval(DvcUnit_t ai_CntInterval)  
{

	FieldList_t::iterator It;
	ClENG_Field * Field ;
	m4bool_t swChanged ;
	DvcUnit_t firstTop, lastTop ;
	DvcUnit_t interval, newinterval ;
	DvcUnit_t partialHeight;
	DvcUnit_t maxParcialHeight;
	DvcUnit_t maxTotalHeight;

	maxTotalHeight = maxParcialHeight = ai_CntInterval;
	
	//Reduce si es necesario CntInterval de forma que no abarque de forma parcial campos 
	//	que no tienen la capacidad de partirse. 

	for  ( swChanged=M4_TRUE, interval=ai_CntInterval; swChanged && interval >0 ; ) 
		{

		swChanged=M4_FALSE ;
		firstTop	= m_HeightPrinted ;
		lastTop		= m_HeightPrinted+interval-1 ;
		maxParcialHeight = 0;
		maxTotalHeight = 0;

		for ( It=m_oField.begin(); It != m_oField.end(); ++It) {
			Field=*It ;
			if ( Field->GetState() != ClENG_Field::START_PRINTING ) continue ;
			
			// Se comprueba si el Field comienza en el área imprimible
			if ( M4_IN_RANGE( Field->GetTop(), firstTop, lastTop ) ) {
				if (Field->IsPartialPrintAllowed() ) {
					// Si es divisible se ajusta la mayor altura que puede imprimir dentro del área imprimible
					partialHeight = Field->AdjustCntInterval(interval - (Field->GetTop() - m_HeightPrinted));
					if (partialHeight == 0) {
						// No cabe un field divisible
						swChanged=M4_TRUE ;
						interval =  Field->GetTop() - m_HeightPrinted ;
						break ;
					}
					maxParcialHeight = M4_MAX(maxParcialHeight, Field->GetTop() + partialHeight);
				} else {
					// Si no es divisible hay que imprimirlo completo
					partialHeight = Field->GetHeight() ;
					maxTotalHeight = M4_MAX(maxTotalHeight, Field->GetTop() + partialHeight);
				}
				//Top empieza en trozo a imprimir ...
				if ( ! M4_IN_RANGE( Field->GetTop()+partialHeight-1, firstTop, lastTop  ) ) 
				{
					//... y sin embargo no puede imprimirse completamente !
					swChanged=M4_TRUE ;
					interval =  Field->GetTop() - m_HeightPrinted ;
					break ;
				} ;
			} ;
		} ;
	} ;
	/*
	if (maxTotalHeight < maxParcialHeight)
	{
		interval = M4_MIN(maxParcialHeight, interval);
	}
	*/

	return interval ;
}


//============================================================================ClENG_CntGroup

ClENG_CntGroup::ClENG_CntGroup()
{
	m_poInsp=GET_INSP();
	m_poReportEnv=NULL ;

	m_State = NOT_INITIALIZED ;
}

void ClENG_CntGroup::End() 
{
	CntList_t::iterator It ;

	for (It=m_oList.begin(); It != m_oList.end(); ++It) {
		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, (*It) ) ;
	} ;
	m_oList.clear() ;
	m_oLocation.StrCpy("");

	m_State = NOT_INITIALIZED ;
}

void ClENG_CntGroup::Init( StENG_ReportEnv *ai_poReportEnv, m4char_t *ai_pcLocationStr) 
{
	ClENG_CntGroup::End() ;

	m_poReportEnv = ai_poReportEnv ;

	m_oLocation.StrCpy(ai_pcLocationStr) ;

	m_State = INITIALIZED ;
}

void ClENG_CntGroup::AddCnt( ClENG_Container *ai_poCnt ) 
{
	m_oList.push_back(ai_poCnt) ;

	m_State = LOADING_CONTAINERS ;
} 

m4bool_t ClENG_CntGroup::PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) 
{
	CntList_t::iterator It ;
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX,"ClENG_CntGroup::PreAnalex() container  %0:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< (char*)m_oLocation << SEND_MSG   ;
	};
	#endif

	//Mensaje genérico, excepción para el container de pagina, metido a huevo en el LogMsg.ini
	if(!strcmp(m_oLocation.InChar(0),"Page")){
		*m_poInsp->m_poTrace << ClMIT_PushLoc( 2162979, "Compiling page field expression" ) << END_LOC ;
	} else {
		*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ CORE_MSG_5015 ] ) 
			<< m_oLocation.InChar(0) << END_LOC ;
	};

	for (It=m_oList.begin(); It != m_oList.end(); ++It) {
		SwOk = M4_BOOL(SwOk && (*It)->PreAnalex(ai_pstEnv));
	} ;

	*m_poInsp->m_poTrace << POP_LOC ;

	m_State = WAITING_FOR_PRELOAD ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX,"ClENG_CntGroup::PreAnalex() container  %0:s, returning %1:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< (char*)m_oLocation << SwOk << SEND_MSG   ;
	};
	#endif

	
	return SwOk ;
}

void ClENG_CntGroup::PrintPreload( m4bool_t ai_bOptimize ) 
{
	CntList_t::iterator It ;

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ CORE_MSG_5000 ] ) 
		<< m_oLocation.InChar(0) << END_LOC ;

	for (It=m_oList.begin(); It != m_oList.end(); ++It) {
		(*It)->PrintPreload(ai_bOptimize) ;
	} ;

	*m_poInsp->m_poTrace << POP_LOC ;

	m_State = START_PRINTING ;
}


m4bool_t ClENG_CntGroup::AreAllXYFixed() 
{
	CntList_t::iterator It ;
	m4bool_t SwOk=M4_TRUE ;

	for (It=m_oList.begin(); It != m_oList.end(); ++It) {
		SwOk = M4_BOOL(SwOk && (*It)->IsXYFixed());


	} ;
	return SwOk ;
}

m4bool_t ClENG_CntGroup::AreAllNotXYFixed() 
{
	CntList_t::iterator It ;
	m4bool_t SwOk=M4_TRUE ;

	for (It=m_oList.begin(); It != m_oList.end(); ++It) {
		SwOk = M4_BOOL(SwOk && !(*It)->IsXYFixed());
	} ;
	return SwOk ;
}


ClENG_Container * ClENG_CntGroup::NextCnt( CntList_t::iterator &ao_It, NEXT_OPERATION ai_Ope) 
{
	if (ai_Ope==GO_FIRST) ao_It=m_oList.begin() ;
	else ++ao_It ;

	return ao_It==m_oList.end()? NULL : (*ao_It) ;
}

ClENG_Container *ClENG_CntGroup::GetFirst() 
{
	CntList_t::iterator It ;
	It=m_oList.begin() ;
	return It==m_oList.end()? NULL : (*It) ;
}

ClENG_Container *ClENG_CntGroup::GetLast() 
{
	if (m_oList.size()!=0) {
		return (m_oList.back()) ;
	} else return NULL ;
}

m4bool_t ClENG_CntGroup::Print( ) 
{
	CntList_t::iterator It ;
	ClENG_Container *Cnt ;
	m4bool_t SwAllPrinted;

	//Mensaje genérico, excepción para el container de pagina, metido a huevo en el LogMsg.ini
	if(!strcmp(m_oLocation.InChar(0),"Page")){
		*m_poInsp->m_poTrace << ClMIT_PushLoc( 2162980, "Printing page field expression" ) << END_LOC ;
	} else {
		*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ CORE_MSG_5020 ] ) 
		<< m_oLocation.InChar(0) << END_LOC ;
	};


	//Puede no disparase el PrintPreload, en este caso pasar a START_PRINTING
	if ( GetState()==WAITING_FOR_PRELOAD ) m_State = START_PRINTING ;

	if ( GetState()==START_PRINTING ) {
		for (It=m_oList.begin(); It != m_oList.end(); ++It) {
			(*It)->NewPrint() ;
		} ;
	} ;

	for (It=m_oList.begin(), SwAllPrinted=M4_TRUE; It != m_oList.end(); ++It) {
		Cnt=(*It) ;
		Cnt->Print() ;
		if (Cnt->GetState()!=ClENG_Container::PRINTED) SwAllPrinted=M4_FALSE ;

	} ;

	*m_poInsp->m_poTrace << POP_LOC ;

	if ( SwAllPrinted ) {
		m_State=START_PRINTING ;
		return M4_TRUE ;
	} else {
		m_State=NOT_ALL_PRINTED ;
		return M4_FALSE ;
	} ;
}

//============================================================================ClENG_Section

ClENG_Section::ClENG_Section() 
{
	m_poInsp=GET_INSP();
	m_poReportEnv=NULL ;
	m_pstSon=NULL;
	m_BLPrivate=NULL;
	m_BLSave=NULL;
	m_BLMax=NULL;
	m_poReport=NULL ;
	
	m_poBeforeCnt=m_poHeaderCnt=
	m_poPreVRCnt=m_poPostVRCnt=m_poVRCnt=
	m_poFooterCnt=m_poAfterCnt=NULL ;
}

void ClENG_Section::End() 
{
	m4int16_t B;

	if (m_pstSon) {
		for ( B=0; B<m_iNSons; ++B ) {
			M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_pstSon[B].m_poSon ) ;
		} ;
	} ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_pstSon, StSon, m_iNSons ) ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_BLPrivate, DvcUnit_t, m_iCountBLItems ) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_BLSave, DvcUnit_t, m_iCountBLItems ) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_BLMax, DvcUnit_t, m_iCountBLItems ) ;

	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poBeforeCnt) ;
	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poHeaderCnt) ;
	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poPreVRCnt) ;
	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poVRCnt) ;
	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poPostVRCnt) ;
	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poFooterCnt) ;
	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_poAfterCnt) ;

	m_oResetBeforeTotal.End() ;
	m_oSetPostVRTotal.End() ; 

	m_oVR.End() ;

	m_oLocation.Init(0,"");
}

m4bool_t ClENG_Section::Init( 

		ClENG_Report *ai_Report, 
		StENG_ReportEnv *ai_poReportEnv,
		ClMIT_IdNumDesc *ai_poIdDesc, ClENG_Section *ai_poFather, m4int16_t ai_iNSons,  
		ClENG_VR::NULL_IF_EMPTY ai_NoPrintEmpty, 

		ClENG_CntGroup * ai_poBeforeCnt, BREAK_MODE ai_BeforePageBreak, 
		ClENG_CntGroup * ai_poAfterCnt,  BREAK_MODE ai_AfterPageBreak, 
		ClENG_CntGroup * ai_poHeaderCnt, HEADER_MODE ai_HeaderMode, 
		ClENG_CntGroup * ai_poFooterCnt, FOOTER_MODE ai_FooterMode, 

		ClENG_CntGroup * ai_poPreVRCnt,  ClENG_CntGroup * ai_poVRCnt, 
		ClENG_CntGroup * ai_poPostVRCnt,

		const m4char_t * ai_pcPrepareRecordset,
		const m4char_t * ai_pcCancelSection,
		const m4char_t * ai_pcResetHeaderTotals,
		const m4char_t * ai_pcCancelVR,
		const m4char_t * ai_pcResetBeforeTotal, 
		const m4char_t * ai_pcSetPostVRTotal, 

		ClENG_VR::GO_THROUGH_MODE ai_GoT, ClENG_VR::RANGE_MODE ai_RangeMode,
		ClMIT_ChnNode * ai_poRecordNode, ClMIT_ChnItem * ai_poGuideItem,
		ClENG_VR::StGroup::GroupKey_t * ai_poGroupKey, 

		m4bool_t ai_bSwSynchronized, m4bool_t ai_bBreakVR, 
		m4bool_t ai_bNullOnSons,     ClMIT_Flag::Value_t ai_MaskTogether,

		ClENG_VR::OneOneList_t *ai_poOneOne,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	m4int16_t B ;
	m4bool_t Return=M4_TRUE ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc; 

	ClENG_CntGroup::CntList_t::iterator CntIt ;
	ClENG_Container *BCnt ;

	m4bool_t SwAux ;
	ClMIT_Flag Flag ; 
	struct {
		ClMIT_Flag::Value_t Mask ;
		ClENG_CntGroup **Cnt ;
		DvcUnit_t *Acu ; 
	} BMask[] = {
		{ TOG_CNT_BEFORE, &m_poBeforeCnt, &m_TogBeforeHeight }, 
		{ TOG_CNT_PRE_VR, &m_poPreVRCnt, &m_TogCntHeight }, 
		{ TOG_CNT_VR, &m_poVRCnt, &m_TogCntHeight}, 
		{ TOG_CNT_POST_VR, &m_poPostVRCnt, &m_TogCntHeight }, 
		{ TOG_CNT_AFTER, &m_poAfterCnt, &m_TogAfterHeight }, 
		{ ClMIT_Flag::NONE_BITS, NULL } 
	} ;

	ClMIT_Str strAux ;
	
	ClENG_Section::End() ;
	
	m_poReport = ai_Report  ;
	m_poReportEnv = ai_poReportEnv ;
	Dvc=m_poReportEnv->m_poDvc ;

	m_oDesc.StrCpy(ai_poIdDesc->GetDesc()) ;
	m_oLocation.Init( ai_poIdDesc->GetIdNum(), m_oDesc ) ;

	if (ai_iNSons!=0) ai_MaskTogether=ClMIT_Flag::NONE_BITS ;
	
	m_iIdSec=ai_poIdDesc->GetIdNum() ;
	m_poFather=ai_poFather;
	m_iNSons=ai_iNSons;
	m_bSynchronized=ai_bSwSynchronized ; 
	ai_bNullOnSons=   m_iNSons==0 ? M4_FALSE : ai_bNullOnSons ;
	m_iCountBLItems = Dvc->GetNumLayoutPages();
	m_bBreakVR = ai_bBreakVR ;

	if (!m_bSynchronized) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_BLPrivate, DvcUnit_t, m_iCountBLItems ) ;
	} ;
	if (!m_bSynchronized || m_iNSons>0 ) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_BLSave, DvcUnit_t, m_iCountBLItems ) ;
	} ;

	if (m_iNSons>0) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_pstSon, StSon, m_iNSons ) ;
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CORE, m_BLMax, DvcUnit_t, m_iCountBLItems ) ;
		for ( B=0; B<m_iNSons; ++B ) m_pstSon[B].m_poSon=NULL ;
	} ;

	//Copiar los containers antes de cualquier testeo de error, para que se hereden los 
	//	punteros y End borre los containers.

	if (ai_BeforePageBreak==BEFORE_CONTAINER_BREAK && !ai_poBeforeCnt ) 
		ai_BeforePageBreak=AFTER_CONTAINER_BREAK ;
	if (ai_AfterPageBreak==BEFORE_CONTAINER_BREAK && !ai_poAfterCnt ) 
		ai_AfterPageBreak=AFTER_CONTAINER_BREAK ;
	m_poBeforeCnt=ai_poBeforeCnt;	m_BeforePageBreak=ai_BeforePageBreak;
	m_poAfterCnt=ai_poAfterCnt;		m_AfterPageBreak=ai_AfterPageBreak;

	m_poHeaderCnt=ai_poHeaderCnt;	m_HeaderMode=ai_HeaderMode;
	m_poFooterCnt=ai_poFooterCnt;	m_FooterMode=ai_FooterMode;

	m_poPreVRCnt=ai_poPreVRCnt; m_poPostVRCnt=ai_poPostVRCnt; m_poVRCnt=ai_poVRCnt ;
	
	strAux.StrCpy("PrepareRecordset") ;
	strAux.StrCat(m_oDesc) ;
	m_oPrepareRecordset.Init( ai_pcPrepareRecordset, strAux ) ;
	strAux.StrCpy("ResetHeaderTotals") ;
	strAux.StrCat(m_oDesc) ;
	m_oResetHeaderTotals.Init( ai_pcResetHeaderTotals, strAux ) ;
	strAux.StrCpy("ResetBeforeTotal") ;
	strAux.StrCat(m_oDesc) ;
	m_oResetBeforeTotal.Init( ai_pcResetBeforeTotal, strAux ) ;
	strAux.StrCpy("SetPostVRTotal") ;
	strAux.StrCat(m_oDesc) ;
	m_oSetPostVRTotal.Init( ai_pcSetPostVRTotal, strAux ) ; 
	

	//----------Inicializacion VR

	if (!m_oVR.Init(
		this, ai_poFather ? &ai_poFather->m_oVR : NULL,  
		&m_oItemMod, ai_GoT, ai_RangeMode, ai_poRecordNode, ai_poGuideItem,
		ai_poGroupKey, ai_poOneOne,
		ai_pcPrepareRecordset, ai_pcCancelSection, ai_pcCancelVR,
		ai_NoPrintEmpty, ai_bNullOnSons, ClMIT_Trace::PROPAGATE_ERROR) ) 
	{
		Return=M4_FALSE ;
		goto exit ;
	} ;

	//----------Inicializaciones containers

	if (m_poHeaderCnt) {
		if ( !m_poHeaderCnt->AreAllXYFixed() ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_60 ] ) << SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} ;
	} ;

	if (m_poFooterCnt) {
		if ( !m_poFooterCnt->AreAllXYFixed()) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_70 ] ) << SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} ;
	} ;

	if ( IsEndSection() ) {
		if ( m_poVRCnt ) {
			if ( !m_poVRCnt->AreAllNotXYFixed() ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_610 ] ) << SEND_MSG ;
				Return=M4_FALSE ;
				goto exit ;
			} ;
		} ;
	} ;

	//-------------Inicializacion valores Together

	Flag.Reset(ai_MaskTogether) ;	SwAux=M4_FALSE ; 
	m_TogCntHeight=m_TogBeforeHeight= m_TogAfterHeight= 0 ;
	for (B=0; BMask[B].Cnt;++B) {
		if ( *BMask[B].Cnt && Flag.True(BMask[B].Mask) ) {
			BCnt = (*BMask[B].Cnt)->NextCnt( CntIt, ClENG_CntGroup::GO_FIRST) ;
			while (BCnt) {
				if ( ! BCnt->IsXYFixed() ) {
					if (SwAux) {
						if ( m_iTogPage != BCnt->GetNumPage() || 
							m_TogTop != BCnt->GetY() ||
							m_TogBottom != BCnt->GetY()+BCnt->GetSizeYLimit()-1 ) 
						{
							*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2162982, 
								"Together activated and there are containers with different limits or in different pages.", ClMIT_Msg::MIXED_OUT) 
								<< SEND_MSG ;
							Return=M4_FALSE ;
							goto exit ;
						} ;
					} else {
						SwAux=M4_TRUE ;
						m_iTogPage= BCnt->GetNumPage() ;
						m_TogTop = BCnt->GetY() ;
						m_TogBottom = BCnt->GetY()+BCnt->GetSizeYLimit()-1 ;
					} ;
					(*BMask[B].Acu) +=  BCnt->GetHeight() ;
				} ;
				BCnt = (*BMask[B].Cnt)->NextCnt( CntIt, ClENG_CntGroup::NEXT) ;
			} ;

		} ;
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

void ClENG_Section::InitLinks( m4int16_t ai_iNSon, 
		ClENG_Section * ai_poSon, m4int16_t ai_iParallel ) 
{
	if ( IsEndSection() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_90 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	if ( !M4_IN_RANGE(ai_iNSon, 0, m_iNSons-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_100 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	if ( m_pstSon[ai_iNSon].m_poSon ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_MSG_110 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	m_pstSon[ai_iNSon].m_poSon = ai_poSon ;
	m_pstSon[ai_iNSon].m_iParallel = ai_iParallel ;
}

m4bool_t ClENG_Section::PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	ClENG_Analex Analex ;

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ CORE_MSG_5010 ] ) 
		<< m_oLocation << END_LOC ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX,"ClENG_Section::PreAnalex() Section %0:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< m_oLocation << SEND_MSG   ;
	};
	#endif

	// Preanaliza este 

	ai_pstEnv->m_poDefaultNode=m_oVR.GetRecordNode() ;
	ai_pstEnv->m_LoadMode = ClMIT_ChnItem::ITEM_LOAD ; 
	ai_pstEnv->m_poItemExternProp= &m_oItemMod ;
	ai_pstEnv->m_poFieldExternProp= &m_oFieldMod ;
	ai_pstEnv->m_poSect=this ;

	if ( m_poBeforeCnt )	SwOk= M4_BOOL(SwOk && m_poBeforeCnt->PreAnalex(ai_pstEnv));
	if ( m_poAfterCnt )		SwOk= M4_BOOL(SwOk && m_poAfterCnt->PreAnalex(ai_pstEnv));
	if ( m_poHeaderCnt )	SwOk= M4_BOOL(SwOk && m_poHeaderCnt->PreAnalex(ai_pstEnv));
	if ( m_poFooterCnt )	SwOk= M4_BOOL(SwOk && m_poFooterCnt->PreAnalex(ai_pstEnv));
	if ( m_poPreVRCnt )		SwOk= M4_BOOL(SwOk && m_poPreVRCnt->PreAnalex(ai_pstEnv));
	if ( m_poPostVRCnt )	SwOk= M4_BOOL(SwOk && m_poPostVRCnt->PreAnalex(ai_pstEnv));
	if ( m_poVRCnt )		SwOk= M4_BOOL(SwOk && m_poVRCnt->PreAnalex(ai_pstEnv));

	SwOk = M4_BOOL(SwOk && Analex.Init( ai_pstEnv, & m_oResetHeaderTotals, 
				   ClMIT_Trace::PROPAGATE_ERROR ));
	SwOk = M4_BOOL(SwOk && Analex.Init( ai_pstEnv, & m_oResetBeforeTotal, 
				   ClMIT_Trace::PROPAGATE_ERROR ));
	SwOk = M4_BOOL(SwOk && Analex.Init( ai_pstEnv, & m_oSetPostVRTotal, 
				   ClMIT_Trace::PROPAGATE_ERROR ));

	SwOk = M4_BOOL(SwOk && m_oVR.PreAnalex(ai_pstEnv));

	//Y va a por los hijos

	for (B=0; B<m_iNSons; ++B) {
		SwOk= M4_BOOL(SwOk && m_pstSon[B].m_poSon->PreAnalex(ai_pstEnv));
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX,"ClENG_Section::PreAnalex() Section %0:s returning %1:s", 
			ClMIT_Msg::EXIT_FORMAT) 
			<< m_oLocation << SwOk << SEND_MSG   ;
	};
	#endif

	*m_poInsp->m_poTrace << POP_LOC ;

	return SwOk ;
}

void ClENG_Section::GetTraceStr( ClENG_Section::RETURN_SECTION ai_Enum, ClMIT_Str *ao_poStr ) 
{
	switch (ai_Enum) {
	case ClENG_Section::NOT_END_SEC_EOP:
		ao_poStr->StrCpy("Section not ended, forcing EOP request") ;
		break ;
	case ClENG_Section::NOT_END_SEC_EOP_DUPLEX:
		ao_poStr->StrCpy("Section not ended, forcing EOP request, Duplex Emulation") ;
		break ;
	case ClENG_Section::END_SEC_EOP:
		ao_poStr->StrCpy("Section ended, EOP request") ;
		break ;
	case ClENG_Section::END_SEC_NOT_EOP:
		ao_poStr->StrCpy("Section ended, EOP not required") ;
		break ;
	case ClENG_Section::END_SEC_NOT_EOP_DUPLEX:
		ao_poStr->StrCpy("Section ended, EOP not required, Duplex Emulation") ;
		break ;
	default:
		ao_poStr->StrCpy("Enum Error!!!") ;
		break ;
	} ;
}


void ClENG_Section::PrepareBreakSection() 
{
	m_bBreakVR=M4_TRUE ;
	if (m_AfterPageBreak==NONE_BREAK) m_AfterPageBreak=AFTER_CONTAINER_BREAK ;
	ClearBreakFlag() ;
}

void ClENG_Section::PrintHeader() {

	// La ejecucion es independiente de si hay o no container, o si este se imprime o no.

	ClENG_VR::ExecuteAction(&m_oResetHeaderTotals) ;

	if ( m_poHeaderCnt ) {
		if ( m_HeaderMode==H_ALL_PAGES || 
			( m_HeaderMode==ONLY_FIRST_PAGE   && GetNumPageSec(0)==1 ) ||
			( m_HeaderMode==EXCEPT_FIRST_PAGE && GetNumPageSec(0)!=1 ) )
		{
			PrintCntGroup( m_poHeaderCnt ) ;
		} ;
	} ;
}



void ClENG_Section::PrintFooter(ClENG_VR::StSave *ai_Pos) 
{
	ClENG_VR::StSave CurrentPos ;

	m_oVR.Save(&CurrentPos) ;
	m_oVR.Restore(ai_Pos) ;
	PrintFooter() ;
	m_oVR.Restore(&CurrentPos) ;
} 


void ClENG_Section::PrintFooter() {
	if ( m_poFooterCnt  ) {
		if (  m_FooterMode==F_ALL_PAGES || 
			( m_FooterMode==ONLY_LAST_PAGE   && m_oVR.IsLast() ) || 
			( m_FooterMode==EXCEPT_LAST_PAGE && !m_oVR.IsLast() )  )
		{
			PrintCntGroup( m_poFooterCnt ) ;
		} ;
	} ;
}

m4bool_t ClENG_Section::IsEOPTogether(m4bool_t ai_bSwBeforeHanging) 
{
	m4bool_t	R = M4_FALSE;
	m4int32_t	iNRecords = 0;
	m4int32_t	iBaseLine = 0;
	m4int32_t	iTogBeforeHeight = 0;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc  ;

	if (IsTogether() && !(m_bTogBeforeDone && !ai_bSwBeforeHanging)) 
	{
		if (ai_bSwBeforeHanging) 
		{
			m_bTogBeforeDone = M4_FALSE;
		}
		
		if (Dvc->GetBaseLineY(m_iTogPage) > m_TogTop) 
		{
			// Bug 0157506. Si el número de registros es negativo, es el caso en el que noç
			// caebn más registros en la página.
			iBaseLine = Dvc->GetBaseLineY(m_iTogPage);
			iTogBeforeHeight = (ai_bSwBeforeHanging ? m_TogBeforeHeight : 0 );
			iNRecords = (m_TogBottom - iBaseLine - iTogBeforeHeight - m_TogAfterHeight) / m_TogCntHeight;

			if (iNRecords < 0)
			{
				iNRecords = 0;
			}

			// Si se ha impreso algo en el area ...
			if (m_oVR.TestAlmostNRecords(iNRecords))
			{
				if (ai_bSwBeforeHanging)
				{
					m_bTogBeforeDone = M4_TRUE;
				}

				R = M4_TRUE;
			} ;
		} ;
	} ;
	return R ;
}

m4uint32_t ClENG_Section::GetNumPageSec(m4int16_t ai_iPage) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	return (m_poReportEnv->m_poDvc->GetNumPageDvc(0)-m_uiPageSec)+ai_iPage+1 ;
}


ClENG_Section::RETURN_SECTION ClENG_Section::PrintSection() 
{
	ClENG_Section::RETURN_SECTION SectionEvent, Return, SectionEventAux  ;
	m4int16_t B;
	m4bool_t SwAllPrinted ;
	m4bool_t SwPrintPreviousVRInFooter=M4_FALSE ;
	ClENG_VR::StSave PrevVR ;
	ClENG_CntGroup * poCntGroup = NULL;	
		
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc ;

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ CORE_MSG_5030 ] ) 
		<< m_oLocation << END_LOC ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Printing section %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< m_oLocation << SEND_MSG   ;
	};
	#endif

	BLSaveOnEntry() ;

	if (m_Reentry != NONE_REENTRY) {
		if ( TaskEnv->m_APIChn.m_poMVC->UpdateCancel() ) {
			m_Reentry=END_REENTRY;
			Return = END_SEC_NOT_EOP ;
			goto exit ;
		} ;
		m_oVR.ReLocate() ;	//Por si otra seccion que comparte nodo altera la posicion
		PrintHeader() ;	//Impresion header en hojas que no son la primera de la seccion
	} ;
	
	// OJO, los breaks ignorados y el orden de los case son INTENCIONADOS. No cambiarlos.
	switch ( m_Reentry ) {


	case NONE_REENTRY:

		// ----------------> Primera vez que entra

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"NONE_ENTRY zone ... Section %0:s") 
				<< m_oLocation << CRAZY_MSG << SEND_MSG   ;
		};
		#endif

		// Se posiciona en el primer registro, se procesa PrepareRecordset, CancelSection, 
		//	etc.

		m_bResetBeforeTotalsDone=M4_FALSE ;

		if ( ! m_oVR.EntryInRecordSet() ) {
			Return = END_SEC_NOT_EOP ;
			m_Reentry = END_REENTRY ;
			goto exit ;
		} ;
		m_oVR.Rewind() ;

		//Switch de impresion

		m_bSwPreVR = m_bSwVR = m_bSwPostVR = M4_TRUE ;

		//Resetea la hebra de proceso a la primera.

		m_iSonReentry=0 ;

		//Todos los hijos entraran inicialmente como NONE_REENTRY 

		for ( B=0; B<m_iNSons; ++B ) {
			m_pstSon[B].m_poSon->m_Reentry=NONE_REENTRY ;
		} ;			

		//Reseteo de fields, cada vez que se cambia de recordset.
		//Debe ser puesto a FALSE tras imprimir un registro.

		m_oFieldMod.SetResetDuplicate(M4_TRUE) ;

		// Para calculo de NumPageSec.

		m_uiPageSec = Dvc->GetNumPageDvc(0) ;

		//Se imprime header de la primera pagina de la seccion. 
		//Despues de Rewind, por lo que ya se esta posicionado en el primer VR.
		PrintHeader() ;	

		// ----------------> Seccion Before

		//Control de BreakBefore, antes de su container 
		//Fijarse que, por la reentrada, no puden producirse dos EOPs seguidos debidos a 
		//	este break y a uno de Together.

		if ( m_BeforePageBreak==BEFORE_CONTAINER_BREAK ) {
			m_Reentry=BEFORE_REENTRY;
			Return = NOT_END_SEC_EOP ;
			goto exit ;
		} ;

		if ( IsEOPTogether(M4_TRUE) ) {
			m_Reentry=BEFORE_REENTRY;
			Return = NOT_END_SEC_EOP ;
			goto exit ;
		} ;

	case BEFORE_REENTRY:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"BEFORE_REENTRY zone ... Section %0:s In record %1:s") 
				<< m_oLocation << m_oVR.GetRecordNode()->GetNumRecord() << CRAZY_MSG << SEND_MSG   ;
		};
		#endif

		//Control de impresion Before

		if ( m_poBeforeCnt ) {

			//Control de EOPs provocados por Before
			
			if ( ! PrintCntGroup(m_poBeforeCnt) ) {
				m_Reentry=BEFORE_REENTRY;
				Return = NOT_END_SEC_EOP ;
				goto exit ;
			} ;
		} ;

		// Siempre tras impresion Before, independiente de ella. Al estar despues SOLO se
		//	diapara una vez por VR. Si estuviese antes, podria reentrase n veces.
		//De todas formas, como no cuesta nada, pongo el if.

		if (!m_bResetBeforeTotalsDone) {
			ClENG_VR::ExecuteAction(&m_oResetBeforeTotal) ;
			m_bResetBeforeTotalsDone=M4_TRUE ;
		} ;

		//Control de BreakBefore, despues de su container 

		if ( m_BeforePageBreak==AFTER_CONTAINER_BREAK ) {
			m_Reentry=DETAIL_REENTRY;
			Return = NOT_END_SEC_EOP ;
			//Caso especial, si en EOF y no hay Cnt de after, fin de seccion
			if ( !m_poAfterCnt && m_oVR.IsEOF() ) {
				PrintFooter() ;
				m_Reentry=END_REENTRY;
				Return = END_SEC_EOP ;
			} ;
			goto exit ;
		} ;


	case DETAIL_REENTRY:
		
		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"DETAIL_REENTRY zone ... Section %0:s In record %1:s") 
				<< m_oLocation << m_oVR.GetRecordNode()->GetNumRecord() << CRAZY_MSG << SEND_MSG   ;
		};
		#endif

		//Control together 

		if ( IsEOPTogether(M4_FALSE) ) {
			m_Reentry=DETAIL_REENTRY ;
			Return = NOT_END_SEC_EOP ;
			goto exit ;
		} ;

		// ----------------> Seccion detalle

		//Si se reentra, o es que es la primera vez que se entra, o es que se reentra desde 
		//	un retorno de procesar un EOP.
		//Cuando se reentra m_iSonReentry siempre esta actualizado, y el registro selccionado
		//	es el que toca procesar (o esta en proceso).

		
		//Ejecuta todas las hebras que quedan tantas veces como registros queden en el 
		//	padre.
		while ( !m_oVR.IsEOF() ) {

			//Comienzo a procesar las hebras pendientes
				
			while (m_iSonReentry < m_iNSons || IsEndSection() ) {

				// Ejecuta la hebra actual con el registro padre actual.
	
				//PreRecord

				if (m_bSwPreVR)
				{
					//Control NewPublish 
					
					if ( GetReport()->IsPublish() ) {
						if ( GetReport()->GetBreakMode() != ClENG_Report::NO_BREAK ) {
							//Con rupturas, disparar en la seccion que rompe
							if ( GetReport()->GetBreakSection()==this ) {
								//Solo tiene efecto la primera llamada
								Dvc->PreparePublishGroup( m_poReportEnv->m_poChnData, 
									GetReport()->GetReportId() ) ;
							} ;
						} else {
							//Sin rupturas, disparar en la primera seccion
							//Como el metodo solo hace caso la primera vez, simplemente 
							//	llamarlo.
							Dvc->PreparePublishGroup( m_poReportEnv->m_poChnData, 
									GetReport()->GetReportId() ) ;
						} ;
					} ;
					
					
					if ( m_poPreVRCnt )
					{
						poCntGroup = m_poPreVRCnt;
				
						//Control de EOPs provocados por Prerecord
						if ( ! PrintCntGroup( m_poPreVRCnt ) ) {
							m_Reentry=DETAIL_REENTRY;
							Return = NOT_END_SEC_EOP ;
							goto exit ;
						} else {
							SwPrintPreviousVRInFooter=M4_FALSE ;
						} ;
					} ;
					m_bSwPreVR=M4_FALSE ;
				} ;

				//Record

				if (m_bSwVR)
				{
					if ( m_poVRCnt )
					{
						poCntGroup = m_poVRCnt;
					
						//Control de EOPs provocados por Prerecord

						if ( ! PrintCntGroup( m_poVRCnt ) )
						{
							m_Reentry=DETAIL_REENTRY;
							Return = NOT_END_SEC_EOP ;
							goto exit ;
						} else {
							SwPrintPreviousVRInFooter=M4_FALSE ;
						} ;

					} ;

					m_bSwVR=M4_FALSE ;
				} ;

				// Bug 98264
				// Antes de imprimir las secciones hijas, actualizaremos las lineas base.
				// En el caso de los CTs, la sección padre sólo tiene una página de layout,
				// y las secciones hijas (los CTs) pueden tener más de una, dependiendo
				// de si las columnas del CT caben en una página, con lo que se queda la 
				// línea base desactualizada.
				if (poCntGroup)
				{
					UpdateBaseLineY(poCntGroup);
				}

				if ( ! IsEndSection() ) {
	
					//Ejecucion de secciones hijas
	
					SectionEvent=END_SEC_NOT_EOP ;

					BLPreLoopSon() ;

					for ( B=m_iSonReentry ; B<m_iNSons; ++B ) {

						if ( m_pstSon[B].m_iParallel != m_pstSon[m_iSonReentry].m_iParallel ) 
							break ;

						// Ejecuta una seccion de una hebra 
	
						BLPrePrintSon() ;
						SectionEventAux = m_pstSon[B].m_poSon->PrintSection() ;

						switch (SectionEventAux) {
						case NOT_END_SEC_EOP:
							//Cualquier seccion inacabada provoca un NOT_END_SEC_EOP incondicional.
							SectionEvent=NOT_END_SEC_EOP ;
							break;
						case NOT_END_SEC_EOP_DUPLEX:
							//Cualquier seccion inacabada provoca un NOT_END_SEC_EOP_DUPLEX incondicional.
							SectionEvent = NOT_END_SEC_EOP_DUPLEX;
							break;
						case END_SEC_EOP:
							//Si alguna seccion da END_SEC_EOP y ninguna da NOT_END_SEC_EOP,
							//	SectionEvent sera END_SEC_EOP. Luego hay que testear este caso.
							if (SectionEvent!=NOT_END_SEC_EOP && SectionEvent!=NOT_END_SEC_EOP_DUPLEX) SectionEvent=END_SEC_EOP ;
							break;
						case END_SEC_NOT_EOP:
							//Nada, SectionEvent=END_SEC_NOT_EOP si todas hijas dan esto.
							break;
						case END_SEC_NOT_EOP_DUPLEX:
							SectionEvent = END_SEC_NOT_EOP_DUPLEX;
							break;
						} ;

						BLPostPrintSon() ;
					} ;			

					//Reposiciona el registro por si una seccion hija lo ha movido
					m_oVR.ReLocate() ;

					BLPostLoopSon() ;

					switch (SectionEvent) {
					case NOT_END_SEC_EOP:
					case NOT_END_SEC_EOP_DUPLEX:
						
						//Hay alguna seccion inacabada, EOP incondicional
						m_Reentry=DETAIL_REENTRY ;
						
						if (SectionEvent == NOT_END_SEC_EOP_DUPLEX)
						{
							Return = NOT_END_SEC_EOP_DUPLEX;
						}
						else
						{
							Return = NOT_END_SEC_EOP;
						}
						
						goto exit ;
						break;
					case END_SEC_EOP:
						//Todas acaban y alguna pide EOP, caso especial, podemos salir con EOP 
						//o acabar
						if ( !m_poPostVRCnt && !m_poAfterCnt && m_oVR.IsLast() )  {
							ClENG_VR::ExecuteAction(&m_oSetPostVRTotal) ;
							PrintFooter() ;
							m_Reentry=END_REENTRY;
							Return = END_SEC_EOP ;
						} else {
							m_Reentry=DETAIL_REENTRY ;
							Return = NOT_END_SEC_EOP ;
							goto exit ;
						} ;
						break;
					case END_SEC_NOT_EOP_DUPLEX:
						Return = END_SEC_NOT_EOP_DUPLEX;
						break;
					case END_SEC_NOT_EOP:
						//Todas acaban y ninguna pide EOP, no hago nada y sigo
						break;
					} ;

					// Solo llega aqui si acaba completamente de ejecutar un thread
	
					m_iSonReentry=B ;

					// Fin ejecucion de secciones hijas

				} else {

					break ;	//Si es EndSection, llega aqui solo tras procesar 
				} ;			//PreVR y VR; sale incondicionalmente del bucle.

			} ;	//Acabe de ejecutar todas las hebras para el actual registro padre


		case DETAIL_REENTRY_2:

			#ifdef RPT_ENGINE_TRACE
			if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
				*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"DETAIL_REENTRY_2 zone (procesing PostRecord) ... Section %0:s In record %1:s") 
					<< m_oLocation << m_oVR.GetRecordNode()->GetNumRecord() << CRAZY_MSG << SEND_MSG   ;
			};
			#endif

			//PostRecord

			if (m_bSwPostVR) {
	
				if ( m_poPostVRCnt ) {

					//Control de EOPs provocados por Postrecord

					if ( ! PrintCntGroup( m_poPostVRCnt ) )
					{
						m_Reentry=DETAIL_REENTRY_2;
						Return = NOT_END_SEC_EOP ;
						goto exit ;
					} else {
						SwPrintPreviousVRInFooter=M4_FALSE ;
					} ;
				} ;

				// Siempre tras impresiuon PostVR, por los mismos motivos que en PreVR.

				ClENG_VR::ExecuteAction(&m_oSetPostVRTotal) ;

				m_bSwPostVR = M4_FALSE ;

			} ;

			// Desactivacion ResetDuplicate

			m_oFieldMod.SetResetDuplicate(M4_FALSE) ;

			SetBreakFlag() ;

			//Esta linea se sale del bucle si el siguiente es EOF, pero dejando la posicion 
			//en el ultimo VR (el actual), para que el After y Footer se impriman en esta 
			//posicion.
			if (m_oVR.IsLast())
			{
				break ;
			}

			//Salva enterior por si hace falta para proceso footer
			m_oVR.Save(&PrevVR) ;
			//Imprimirá footer con anterior. Esto se desactivará si en la actual sección se
			//	consigue imprimir algo via container PreVR, VR ó PostVR
			SwPrintPreviousVRInFooter=M4_TRUE ;

			m_oVR.Next() ;
			m_bSwPreVR = m_bSwVR = m_bSwPostVR = M4_TRUE ;

			// Pone modo de reentrada NONE_REENTRY en secciones hijas
			m_iSonReentry=0 ;
			for ( B=0; B<m_iNSons; ++B ) {
				m_pstSon[B].m_poSon->m_Reentry=NONE_REENTRY ;
			} ;			

			if ( TaskEnv->m_APIChn.m_poMVC->UpdateCancel() ) {
				m_Reentry=END_REENTRY;
				Return = END_SEC_NOT_EOP ;
				goto exit ;
			} ;

			if ( IsBreakVR() && !m_oVR.IsEOF() )
			{
				// Procesa BreakRecord
				
				//Debe hacerlo despues del Next para evitar paginas en blanco e impresiones de 
				//	Header con datos de un registro ya impreso.
					
				m_Reentry=DETAIL_REENTRY_3 ;
				Return = NOT_END_SEC_EOP ;

				if (m_poReportEnv->m_poExeParams->m_bDuplexEmulation == M4_TRUE)
				{
					Return = NOT_END_SEC_EOP_DUPLEX;
				}

				SwPrintPreviousVRInFooter=M4_TRUE ;

				goto exit ;
			} ;

		case DETAIL_REENTRY_3:

			#ifdef RPT_ENGINE_TRACE
			if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
				*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"DETAIL_REENTRY_3 zone (next record in thread) ... Section %0:s In record %1:s") 
					<< m_oLocation << m_oVR.GetRecordNode()->GetNumRecord() << CRAZY_MSG << SEND_MSG   ;
			};
			#endif
			;

		} ;		//Fin !IsEOF(), Fin proceso detalle
				
		// ----------------> Seccion After

		//Control de BreakAfter, antes de su container 

		if ( m_AfterPageBreak==BEFORE_CONTAINER_BREAK ) {
			m_Reentry=AFTER_REENTRY;
			Return = NOT_END_SEC_EOP ;
			goto exit ;
		} ;

	case AFTER_REENTRY:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"AFTER_REENTRY zone ... Section %0:s In record %1:s") 
				<< m_oLocation << m_oVR.GetRecordNode()->GetNumRecord() << CRAZY_MSG << SEND_MSG   ;
		};
		#endif

		//Control de impresion After

		if ( m_poAfterCnt ) {
			
			SwAllPrinted= PrintCntGroup( m_poAfterCnt )  ; 	

			//Control de EOPs provocados por After

			if ( !SwAllPrinted ) {
				m_Reentry=AFTER_REENTRY;
				Return = NOT_END_SEC_EOP ;
				goto exit ;
			} ;
		} ;

		//Control de BreakAfter, despues de su container 

		if ( m_AfterPageBreak==AFTER_CONTAINER_BREAK ) {
			m_Reentry=END_REENTRY;
			Return = END_SEC_EOP ;
			goto exit ;
		} ;

		//Aunque al acabar de imprimir la seccion no se salte pagina, si que se imprime el
		//	footer. Si pasa por aqui, la entrada END_REENTRY y el Return=END_SEC_NOT_EOP 
		//	evitara que el footer se vuelva a imprimir a la salida.

		PrintFooter() ;

	case END_REENTRY:

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"END_REENTRY zone ... Section %0:s In record %1:s") 
				<< m_oLocation << m_oVR.GetRecordNode()->GetNumRecord() << CRAZY_MSG << SEND_MSG   ;
		};
		#endif

		m_Reentry=END_REENTRY ;

		if (IsBreakVR() && m_poReportEnv->m_poExeParams->m_bDuplexEmulation == M4_TRUE)
		{
			Return = END_SEC_NOT_EOP_DUPLEX;
		}
		else
		{
			Return = END_SEC_NOT_EOP ;
		}
		goto exit ;
	} ;

exit:
	if ( Return==END_SEC_EOP || Return==NOT_END_SEC_EOP || Return==NOT_END_SEC_EOP_DUPLEX ) {
		if ( SwPrintPreviousVRInFooter ) {
			PrintFooter(&PrevVR) ;
		} else {
			PrintFooter() ;
		} ;
	} ;

	if (!m_bResetBeforeTotalsDone && (Return==END_SEC_EOP || Return==END_SEC_NOT_EOP || Return== END_SEC_NOT_EOP_DUPLEX ) ) {
		ClENG_VR::ExecuteAction(&m_oResetBeforeTotal) ;
		m_bResetBeforeTotalsDone=M4_TRUE ;
	} ;

	BLRestoreOnExit() ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		ClMIT_Str ReturnTxt ;
		ClENG_Section::GetTraceStr( Return,&ReturnTxt) ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"Printing section %0:s returning %1:s in record %2:s", ClMIT_Msg::EXIT_FORMAT) 
			<< m_oLocation << ReturnTxt.InChar(0) << m_oVR.GetRecordNode()->GetNumRecord() <<SEND_MSG   ;
	} ;
	#endif

	*m_poInsp->m_poTrace << POP_LOC ;

	return Return ;
}


void ClENG_Section::UpdateBaseLineY(ClENG_CntGroup * ai_CntGroup)
{
	ClENG_Section * poSonSection = NULL;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM());
	ClENG_OutDevice * poDvc = m_poReportEnv->m_poDvc ;
	
	m4bool_t bReplicatePages = poDvc->GetReplicatePages();
	if (bReplicatePages == M4_TRUE)
	{
		// Páginas de layout del container group actual. Hay un container por página.
		// Se considera que el número de páginas es el número de containers en el 
		// container group.
		for (m4uint32_t i = 0; i < m_iNSons; i++)
		{
			// BaseLineY de la sección actual, de la primera página.
			// Esta es la única actualizada porque es la única página
			// que existe y en la que se ha impreso algo anteriormente.
			DvcUnit_t baseLineY = poDvc->GetBaseLineY(0);
			
			poSonSection = m_pstSon[i].m_poSon;
			ClENG_OutDevice * poSonDvc = poSonSection->m_poReportEnv->m_poDvc;
			m4uint32_t iNumSonLayout = poSonDvc->GetNumLayoutPages();
			m4uint32_t iNumCont = ai_CntGroup->GetCount();
			if (iNumCont < iNumSonLayout)
			{
				for (m4uint32_t j = iNumCont - 1; j < iNumSonLayout; j++)
				{
					DvcUnit_t aux = poSonDvc->GetBaseLineY(j);
					poSonDvc->SetBaseLineY(j, baseLineY);
				}
			}
		}
	}	
}

m4bool_t ClENG_Section::PrintCntGroup( ClENG_CntGroup *ai_CntGroup ) 
{
	m4bool_t R ;

	if ( m_oVR.IsEOF() && m_oVR.GetRangeMode()==ClENG_VR::SUBGROUP_VR )  {

		m_poFather->m_oVR.ReLocate() ;

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped() 
			&& m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL )
		{
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
				"Printing container %0:s of section %1:s, record %2:s of node %3:s",
				ClMIT_Msg::ENTRY_FORMAT)
				<< ai_CntGroup->GetLocation()->InChar(0)
				<< m_oLocation 
				<< m_oVR.GetRecordNode()->GetNumRecord()
				<< m_oVR.GetRecordNode()->GetIdDesc().GetId()
				<< DETAILED_MSG << SEND_MSG   ;
		};
		#endif

		R=ai_CntGroup->Print() ;
		m_oVR.ReLocate() ;

	} else {
		
		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped() 
			&& m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL )
		{
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
				"Printing container %0:s of section %1:s, record %2:s of node %3:s",
				ClMIT_Msg::ENTRY_FORMAT )
				<< ai_CntGroup->GetLocation()->InChar(0)
				<< m_oLocation 
				<< m_oVR.GetRecordNode()->GetNumRecord()
				<< m_oVR.GetRecordNode()->GetIdDesc().GetId()
				<< DETAILED_MSG << SEND_MSG   ;
		};
		#endif

		R = ai_CntGroup->Print() ;

	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() 
		&& m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::GENERAL )
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"Printing container %0:s of section %1:s", ClMIT_Msg::EXIT_FORMAT  )
			<< ai_CntGroup->GetLocation()->InChar(0)
			<< m_oLocation 
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	
	return R ;
}

void ClENG_Section::BLSaveOnEntry() 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc  ;

	if (!IsSynchronized() ) {

		m4int16_t B, Top ;
		
		Top=Dvc->GetNumLayoutPages() ;
		for (B=0; B<Top; ++B ) {
			m_BLSave[B]=Dvc->GetBaseLineY(B) ;
			Dvc->SetBaseLineY( B, m_BLPrivate[B] ) ;
		} ;
	} ;
}

void ClENG_Section::BLRestoreOnExit() 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc  ;

	if (!IsSynchronized() ) {

		m4int16_t B, Top ;
		
		Top=Dvc->GetNumLayoutPages() ;
		for (B=0; B<Top; ++B ) {
			m_BLPrivate[B] = Dvc->GetBaseLineY( B ) ;
			Dvc->SetBaseLineY(B, m_BLSave[B]) ;
		} ;
	} ;
}

void ClENG_Section::BLPreLoopSon() 
{
	m4int16_t B, Top ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc  ;
	
	Top=Dvc->GetNumLayoutPages() ;

	if (!IsSynchronized() ) {

		for (B=0; B<Top; ++B ) {
			m_BLPrivate[B] = Dvc->GetBaseLineY( B ) ;
		} ;

	} else {

		for (B=0; B<Top; ++B )
		{
			m_BLSave[B]=Dvc->GetBaseLineY(B) ;
		} ;

	} ;

	for (B=0; B<Top; ++B ) {
		m_BLMax[B] = 0 ;
	} ;
}

void ClENG_Section::BLPrePrintSon() 
{
	m4int16_t B, Top ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc  ;
		
	Top=Dvc->GetNumLayoutPages() ;

	for (B=0; B<Top; ++B )
	{
		Dvc->SetBaseLineY( B, m_BLSave[B] ) ;
	} ;
}

void ClENG_Section::BLPostPrintSon() 
{
	m4int16_t B, Top ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc  ;
		
	Top=Dvc->GetNumLayoutPages() ;

	for (B=0; B<Top; ++B ) {
		m_BLMax[B] = M4_MAX( m_BLMax[B], Dvc->GetBaseLineY( B ) ) ;
	} ;
}

void ClENG_Section::BLPostLoopSon() 
{
	m4int16_t B, Top ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClENG_OutDevice *Dvc=m_poReportEnv->m_poDvc  ;
		
	Top=Dvc->GetNumLayoutPages() ;

	if (!IsSynchronized() ) {

		for (B=0; B<Top; ++B ) {
			m_BLSave[B] = m_BLMax[B] ;
			Dvc->SetBaseLineY( B, m_BLPrivate[B] ) ;
		} ;

	} else {

		for (B=0; B<Top; ++B ) {
			Dvc->SetBaseLineY( B, m_BLMax[B] ) ;
		} ;

	} ;
}

m4bool_t ClENG_Section::CheckRecordSetPosition( m4bool_t ai_bForceReentry )
{
	m4bool_t bForceChildReentry = M4_FALSE;

	if ( m_Reentry == NONE_REENTRY || ai_bForceReentry )
	{
		// Primera ejecución en el nodo. Se posiciona en el primer registro
		// para la sección actual
		if (!m_oVR.EntryInRecordSet())
		{
			// Se ha cancelado el posicionamiento en el recordset (o no hay registro)
			return M4_FALSE;
		}
		m_oVR.Rewind() ;
		if (m_oVR.IsEOF())
		{
			// Segunda verificación de EOF (no se ha posicionado en registro)
			return M4_FALSE;
		}
		// y se actualiza la marca paa que se haga también recursivamente
		bForceChildReentry = M4_TRUE;
	}
	else if ( m_iNSons > 0 )
	{
		// Se analiza si todas las secciones hijas están en END_REENTRY
		// (han terminado de ejecutarse)
		m4bool_t bAllEndReentry = M4_TRUE;
		for ( m4int16_t B=0; B<m_iNSons && bAllEndReentry; ++B ) {
			if ( m_pstSon[B].m_poSon->m_Reentry != END_REENTRY ) bAllEndReentry = M4_FALSE;
		} ;	
		if ( bAllEndReentry == M4_TRUE )
		{
			// Nos movemos al siguiente registro
			m_oVR.FindNext();
			if (m_oVR.IsEOF())
			{
				// Verificación de EOF (no hay más registros)
				return M4_FALSE;
			}
			// y se actualiza la marca paa que se se posicione en el primer
			// registros recursivamente
			bForceChildReentry = M4_TRUE;
		}
	}
	if ( bForceChildReentry == M4_TRUE )
	{
		for ( m4int16_t B=0; B<m_iNSons; ++B ) {
			// En este punto no influye la situación de EOF de algún hijo
			m_pstSon[B].m_poSon->CheckRecordSetPosition( M4_TRUE ) ;
		} ;	
	}

	return M4_TRUE;
}

