
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ct_templ.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/21/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "eng.h"
#include "engdf.inl"

#include "designdt.h"
#include "totalize.h"
#include "sess_rd.h"

#include "ct_templ.h"

#define GROUP_ID    ENG_TGI_LOADER


//=======================================================================ClENG_CT_T (template)

ClENG_CT_T::ClENG_CT_T()
{
	m_poInsp = GET_INSP() ;
	m_poCT=NULL ;
	m_poCell=NULL ;
	m_poLevelHeight=NULL ;
	m_puiIdParamSec=NULL ;
	m_iLevelCount=0 ;	//Necesario porque en constructor copia Init hace de flag
}

ClENG_CT_T::ClENG_CT_T(const ClENG_CT_T & ai_oSource)  
{
	m_poInsp = GET_INSP() ;
	m_poCT=NULL ;
	m_poCell=NULL ;
	m_poLevelHeight=NULL ;
	m_puiIdParamSec=NULL ;

	Init(&ai_oSource) ;
}

void ClENG_CT_T::Init(const ClENG_CT_T *ai_poSource) 
{
	m4int16_t B, B2 ;
	StENG_DT_CTCell * C1, *C2 ;

	ClENG_CT_T::End() ;

	m_poCT			= ai_poSource->m_poCT ;
	m_iLevelCount	= ai_poSource->m_iLevelCount ;

	if ( m_iLevelCount ) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poCell, StENG_DT_CTCell, 
			m_iLevelCount*ENG_CT_TYPE_COUNT ) ;
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poLevelHeight, DvcUnit_t, 
			m_iLevelCount ) ;
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_puiIdParamSec, m4uint32_t, 
			m_iLevelCount ) ;
		for (B=0;B<m_iLevelCount;++B) {
			m_poLevelHeight[B]	= ai_poSource->m_poLevelHeight[B] ;
			m_puiIdParamSec[B]	= ai_poSource->m_puiIdParamSec[B] ;
			for (B2=0;B2<ENG_CT_TYPE_COUNT;++B2) {
				C1=GetCell(B,(ENG_CT_CELL_TYPE)B2) ;
				C2=ai_poSource->GetCell(B,(ENG_CT_CELL_TYPE)B2) ;
				*C1 = *C2 ;
			} ;
		} ;

	} ;
}


void ClENG_CT_T::End()
{
	m_poCT=NULL ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poCell, StENG_DT_CTCell, 
		m_iLevelCount*ENG_CT_TYPE_COUNT ) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poLevelHeight, DvcUnit_t, 
		m_iLevelCount ) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_puiIdParamSec, m4uint32_t, 
		m_iLevelCount ) ;
}


m4bool_t ClENG_CT_T::Init( 
	StENG_DT_CTData *ai_poCT_Owner, StENG_DD_CTData *ai_poCT_DD, ClENG_DesignTree *ai_poDT, 
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t BLevel, BType ;
	StENG_DD_CTData::CellList_t::iterator CellIt ;
	StENG_DT_CTCell CellAux ;
	StENG_DT_CTCell *Cell ;
	ENG_IdSectionList_t::iterator ItIdParam ;

	ClENG_CT_T::End();

	m_poCT = ai_poCT_Owner ;

	m_iLevelCount = m_poCT->m_iLevelCount ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poCell, StENG_DT_CTCell, 
		m_iLevelCount*ENG_CT_TYPE_COUNT ) ;
	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poLevelHeight, DvcUnit_t, 
		m_iLevelCount ) ;
	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_puiIdParamSec, m4uint32_t, 
			m_iLevelCount ) ;

	//****Carga de m_puiIdParamSec
	
	for ( BLevel=0, ItIdParam=m_poCT->m_oParamSec.begin() ; 
		BLevel<m_iLevelCount; ++BLevel, ++ItIdParam ) 
	{
		m_puiIdParamSec[BLevel] = *ItIdParam ;
	} ;

	//****Carga de template

	for ( CellIt=ai_poCT_DD->m_oCellList.begin(); CellIt!=ai_poCT_DD->m_oCellList.end() ; ++CellIt) {
		//Llamada especial, solo Level, etc
		SwOk = M4_BOOL( SwOk && CellAux.Init( &(*CellIt), NULL ) ) ;	
		if (!SwOk) goto exit ;

		if ( ! M4_IN_RANGE( CellAux.m_iLevel, 0, m_iLevelCount ) ) {
			/*##Poner num msg*/
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Level de celda %0:s fuera de rango: valido del 0 al %1:s ", ClMIT_Msg::MIXED_OUT) 
				<< CellAux.m_iLevel << m_iLevelCount << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		Cell = GetCell(CellAux.m_iLevel,CellAux.m_Type) ;

		if ( Cell->m_bLoaded ) {
			/*##Poner num msg*/
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Celda %0:s %1:s ya cargada", ClMIT_Msg::MIXED_OUT) 
				<< CellAux.m_iLevel << (m4int16_t)CellAux.m_Type << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		SwOk = M4_BOOL( SwOk && Cell->Init( &(*CellIt), ai_poDT ) ) ;

	} ;

	//****Calculo de m_poLevelHeight[x]

	for ( BLevel=0; BLevel<m_iLevelCount; ++BLevel ) {
		m_poLevelHeight[BLevel] = 0 ;
		for (BType=0; BType<ENG_CT_TYPE_COUNT; ++BType ) {
			Cell = GetCell(BLevel,(ENG_CT_CELL_TYPE)BType) ;
			if ( Cell->m_bLoaded ) {
				if ( Cell->m_Height > m_poLevelHeight[BLevel] ) {
					m_poLevelHeight[BLevel] = Cell->m_Height ;
				} ;
			} ;
		} ;
	} ;

exit:
	if (SwOk) Trace() ;

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;		
}

StENG_DT_CTCell * ClENG_CT_T::GetCell( m4int16_t ai_Level, ENG_CT_CELL_TYPE ai_CntType ) const
{ 
	if ( ! M4_IN_RANGE( ai_Level, 0, m_iLevelCount-1 ) ) {
		/*##Poner num msg*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Level de celda %0:s fuera de rango: valido del 0 al %1:s ", ClMIT_Msg::MIXED_OUT) 
			<< ai_Level << m_iLevelCount << SEND_MSG ;
		return NULL ;
	} else {
		return &m_poCell[(ai_Level)*ENG_CT_TYPE_COUNT+ai_CntType] ;
	} ;
} ;

DvcUnit_t ClENG_CT_T::GetAccLevelHeight( m4int16_t ai_Level ) 
{
	m4int16_t B ;
	DvcUnit_t R ;

	for ( B=ai_Level, R=0; B<m_iLevelCount; ++B ) R += m_poLevelHeight[B] ;
	return R ;
}

StENG_DT_FieldData	* ClENG_CT_T::FindIdCTField( ENG_FIND_ACTION ai_Action, m4char_t *ai_pcIdCtField )
{
	m4int16_t B, B2 ;
	StENG_DT_FieldData	*R=NULL ;
	StENG_DT_CTCell * Cell ;

	for (B=0, R=NULL; B<m_iLevelCount; ++B) {
		for (B2=0;B2<ENG_CT_TYPE_COUNT;++B2) {
			Cell = & m_poCell[(B)*ENG_CT_TYPE_COUNT+B2] ;
			if ( Cell->m_bLoaded ) {
				R = Cell->FindIdCTField(ENG_TRY_FIND,ai_pcIdCtField) ;
				if (R) break ;
			} ;
		} ;
	} ;
	switch(ai_Action) {
	case ENG_EXIST_REQUIRED:
		if (!R) {
			/*##Num error*/
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
				"Field with IdCT %0:s not found", ClMIT_Msg::MIXED_OUT) 
				<< ai_pcIdCtField << SEND_MSG ;
		} ;
		break ;

	case ENG_NOT_EXIST_REQUIRED:
		if (R) {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Fiel with IdCT %0:s exist", ClMIT_Msg::MIXED_OUT) 
				<< ai_pcIdCtField << SEND_MSG ;
		} ;
		break ;
	} ;
	return R ;
}


void ClENG_CT_T::Trace() 
{
	m4int16_t B, B2 ;
	StENG_DT_CTCell * C ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_CT_T::Trace()", ClMIT_Msg::ENTRY_FORMAT )
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

	for (B=0;B<m_iLevelCount;++B) {
		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "LEVEL %0:s ", ClMIT_Msg::ENTRY_FORMAT )
				<< B << CRAZY_MSG << SEND_MSG   ;
		} ;
		#endif
		for (B2=0;B2<ENG_CT_TYPE_COUNT;++B2) {
			C=GetCell(B,(ENG_CT_CELL_TYPE)B2) ;
			if (C->m_bLoaded) {
				#ifdef RPT_ENGINE_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Template CELL %0:s") 
						<< ClENG_DesignTree::GetCellTypeTraceStr( (ENG_CT_CELL_TYPE)B2 )
						<< CRAZY_MSG << SEND_MSG   ;
					m_poInsp->m_poTrace->Indent() ;
					*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
						"IdParamSec %0:s CT_Level %1:s CT_Cell %2:s Width %3:s milim Height %4:s milim "
						"RegionColour %5:s RegionBorder %6:s FieldCount %7:s" ) 
						<< C->m_uiIdParamSec << C->m_iLevel 
						<< ClENG_DesignTree::GetCellTypeTraceStr( C->m_Type )
						<< ClMetric::DvcToMilim(C->m_Width) << ClMetric::DvcToMilim(C->m_Height)
						<< C->m_uiRegionColour << C->m_oRegionBorder.InChar(0)
						<< (m4int16_t)C->m_oFieldList.size()
						<< CRAZY_MSG << SEND_MSG  ;
					m_poInsp->m_poTrace->Unindent() ;
				} ;
				#endif
			} ;
		} ;
		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "LEVEL %0:s ", ClMIT_Msg::EXIT_FORMAT )
				<< B << CRAZY_MSG << SEND_MSG   ;
		} ;
		#endif
	} ;
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_CT_T::Trace()", ClMIT_Msg::EXIT_FORMAT )
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif
}

void ClENG_CT_T::IncrCountFontFormatColourInstances( 
		m4uint32_t ai_uiIdFont, m4uint32_t ai_uiIdFormat, m4uint32_t ai_uiIdColour, 
		m4uint32_t *aoi_puiFontCount, m4uint32_t *aoi_puiFormatCount,
		m4uint32_t *aoi_puiColourCount ) 
{
	StENG_DT_CTCell *Cell ;
	m4int16_t Level, Type ;

	for (Level=0;Level<m_iLevelCount;++Level) {
		for (Type=0;Type<ENG_CT_TYPE_COUNT;++Type) {
			Cell = GetCell( Level, (ENG_CT_CELL_TYPE)Type )  ;
			if (!Cell) continue ;
			if ( !Cell->m_bLoaded ) continue ;
			Cell->IncrCountFontFormatColourInstances( 
				ai_uiIdFont, ai_uiIdFormat, ai_uiIdColour, aoi_puiFontCount, aoi_puiFormatCount, aoi_puiColourCount ) ;
		} ;
	} ;
}

void ClENG_CT_T::GenerateNewCTIdField( ClMIT_Str *ao_poStr ) 
{
	StENG_DT_CTCell *Cell ;
	m4int16_t Level, Type ;
	m4bool_t SwFound ;
	ENG_DT_FieldList_t::iterator ItF ;
	m4uint32_t Key, Count ;

	Key=-1 ;
	do {

		//Generar clave

		ao_poStr->Accommodate( 60 ) ;
		ao_poStr->StrCpy("CT_Field_") ;
		sprintf( ao_poStr->InChar(ao_poStr->StrLen()), "%lu",  Key==-1 ? 0: Key ) ;
		ao_poStr->OverStringValid() ;

		//Comprobar unicidad

		for (SwFound=M4_FALSE, Level=0, Count=0 ; !SwFound && Level<m_iLevelCount ; ++Level) {
			for (Type=0; !SwFound && Type<ENG_CT_TYPE_COUNT; ++Type) {
				Cell = GetCell( Level, (ENG_CT_CELL_TYPE)Type )  ;
				if (!Cell) continue ;
				if ( !Cell->m_bLoaded ) continue ;
				for ( ItF = Cell->m_oFieldList.begin(); ItF != Cell->m_oFieldList.end(); ++ItF ) {
					++Count ;
					if ( ClMIT_Str::StrICmpTrim( (*ItF).m_oIdCtField, ao_poStr->InChar(0) )==0 ) {
						SwFound=M4_TRUE ;
						break ;
					} ;
				} ;
			}
		} ;

		//Next Key

		if ( Key==-1 ) Key=Count ;
		else ++Key ;

	} while  ( SwFound ) ;
}


//=======================================================================ClENG_DisplayCell

ClENG_DisplayCell::ClENG_DisplayCell() 
{ 
	m_poInsp = GET_INSP() ;
	m_poSon=NULL ;
	m_poOwner=NULL ;
	m_oCTIndex.StrCpy("");
	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poSon, StENG_CellList ) ;
}

ClENG_DisplayCell::ClENG_DisplayCell(const ClENG_DisplayCell& ai_oSource) 
{ 
	m_poInsp=GET_INSP(); 
	m_poSon=NULL ;
	m_oCTIndex.StrCpy("");
	Init(&ai_oSource) ; 
} 

ClENG_DisplayCell & ClENG_DisplayCell::operator = (const ClENG_DisplayCell &ai_oOpe) 
{ 
	m_poInsp=ai_oOpe.m_poInsp ; 
	m_poSon=NULL ;
	m_poOwner=NULL ;
	Init(&ai_oOpe) ; 
	return *this ;
} 

void ClENG_DisplayCell::End() 
{
	//En ultima instancia, se llama a los destructores de los nodos, destruyendo la rama

	M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poSon, StENG_CellList ) ;

	m_oCTIndex.StrCpy("");
}

void ClENG_DisplayCell::Init(ClENG_CntDisplay *ai_poOwner) 
{
	ClENG_DisplayCell::End() ;

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poSon, StENG_CellList ) ;

	m_poOwner = ai_poOwner ;
}


void ClENG_DisplayCell::Init(const ClENG_DisplayCell *ai_poSource) 
{
	Init(ai_poSource->m_poOwner) ;

	m_FieldsType	= ai_poSource->m_FieldsType ;	
	m_iLevel		= ai_poSource->m_iLevel ;	
	m_Cell			= ai_poSource->m_Cell ;
	m_bDummy		= ai_poSource->m_bDummy ;
	m_Top			= ai_poSource->m_Top ;
	m_Left			= ai_poSource->m_Left ;
	m_Height		= ai_poSource->m_Height ;
	m_Width			= ai_poSource->m_Width ;		
	m_FieldYOffset	= ai_poSource->m_FieldYOffset ;
	m_FieldXOffset	= ai_poSource->m_FieldXOffset ;
	m_oCTIndex.StrCpy( ai_poSource->m_oCTIndex );

	m_poSon->m_oList = ai_poSource->m_poSon->m_oList ;	

}

void ClENG_DisplayCell::Trace( )  
{
	ENG_CellList_t::iterator It ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"TRACE CELL", ClMIT_Msg::ENTRY_FORMAT) 
			<< CRAZY_MSG << SEND_MSG   ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"FieldsType %0:s CT_Level %1:s CT_Cell %2:s CTIndex %3:s "
			"SwDummy %4:s Top %5:s milim "
			"Left %6:s milim Width %7:s milim Height %8:s milim "
			"FieldsYOffset %9:s milim FieldsXOffset %10:s milim CTIndex %11:s" ) 
			<< ClENG_DesignTree::GetCellFieldTypeTraceStr(m_FieldsType) << m_iLevel 
			<< ( m_FieldsType == ENG_STATIC_FIELDS ? "Static" :
				ClENG_DesignTree::GetCellTypeTraceStr( m_Cell ) )
			<< m_oCTIndex.InChar(0)
			<< m_bDummy 
			<< ClMetric::DvcToMilim(m_Top) << ClMetric::DvcToMilim(m_Left)
			<< ClMetric::DvcToMilim(m_Width) << ClMetric::DvcToMilim(m_Height)
			<< ClMetric::DvcToMilim(m_FieldYOffset) << ClMetric::DvcToMilim(m_FieldXOffset)
			<< m_oCTIndex.InChar(0)
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

	for ( It=m_poSon->m_oList.begin(); It!=m_poSon->m_oList.end(); ++It ) {
		(*It).Trace() ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"TRACE CELL", ClMIT_Msg::EXIT_FORMAT) 
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif

} ;


void ClENG_DisplayCell::Serialize( ClENG_VB_Serialize *ai_poSerial ) 
{
	ENG_CellList_t::iterator It ;

	//***CellEOF
	ai_poSerial->SerializeBool(M4_FALSE) ;

	//***FieldsType
	ai_poSerial->SerializeInt( m_FieldsType ) ;

	//***CT_Level
	ai_poSerial->SerializeInt( m_iLevel ) ;

	//***CT_Cell
	ai_poSerial->SerializeInt( m_Cell ) ;

	//***SwDummy
	ai_poSerial->SerializeBool( m_bDummy ) ;

	//***Top
	ai_poSerial->SerializeLong( m_Top ) ;

	//***Left
	ai_poSerial->SerializeLong( m_Left ) ;

	//***Width
	ai_poSerial->SerializeLong( m_Width ) ;

	//***Height
	ai_poSerial->SerializeLong( m_Height ) ;

	//***FieldYOffset
	ai_poSerial->SerializeLong( m_FieldYOffset ) ;

	//***FieldXOffset
	ai_poSerial->SerializeLong( m_FieldXOffset ) ;
	
	//***Recorre hijos

	for ( It=m_poSon->m_oList.begin(); It!=m_poSon->m_oList.end(); ++It ) {
		(*It).Serialize(ai_poSerial) ;
	} ;

}


m4bool_t ClENG_DisplayCell::AddFields( 
	ClENG_DesignTree *ai_poDT, StENG_DT_CntData *ao_poCntData, m4bool_t ai_bSwAddFields,
	m4bool_t ai_bSwTestCell ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ENG_CellList_t::iterator It ;
	StENG_DT_FieldData Field ;
	StENG_DT_CTCell *CellAux=NULL ;
	ENG_DT_FieldList_t::iterator ItF, ItF2 ;
	ENG_DT_FieldList_t *FieldList, *FieldList2 ;
	ClMIT_Str StrAux ;

	if ( ! ai_bSwAddFields  ) {

		//****Modo Añadir fields simulacion celda. Añade uno solo, que simula la region. 

		Field.InitFieldStrDefault(ai_poDT) ;

		Field.m_Top		= m_Top ;
		Field.m_Left	= m_Left ;
		Field.m_Width	= m_Width ;
		Field.m_Height	= m_Height ;

		switch ( m_FieldsType ) {
		case ENG_TEMPLATE_H_FIELDS:
		case ENG_TEMPLATE_R_FIELDS:
			CellAux = m_poOwner->GetTemplate()->GetCell( m_iLevel, m_Cell ) ;
			if (!CellAux) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
			Field.m_oRegionBorder.StrCpy( CellAux->m_oRegionBorder ) ;
			Field.m_uiRegionColour	=  CellAux->m_uiRegionColour ;
			break ;
		case ENG_STATIC_FIELDS:
		default:
			Field.m_oRegionBorder.StrCpy(m_poOwner->GetCntData()->m_oRegionBorder) ;
			Field.m_uiRegionColour	= m_poOwner->GetCntData()->m_uiRegionColour ;
			break ;
		} ;

		if ( ai_bSwTestCell ) {
			Field.m_oRegionBorder.StrCpy("010000100000010000100000010000100000010000100000") ;
		} ;

		ao_poCntData->m_oFieldList.push_back( Field ) ;

	} else if ( ! m_bDummy ) {

		//****Modo Añadir fields ( filtro Dummy ya pasado )

		if ( m_FieldsType==ENG_TEMPLATE_H_FIELDS || m_FieldsType==ENG_STATIC_FIELDS )  {
			
			//En modo Header y Static, es un simple recorrido

			if (m_FieldsType==ENG_TEMPLATE_H_FIELDS) {
				CellAux = m_poOwner->GetTemplate()->GetCell( m_iLevel, m_Cell ) ;
				if (!CellAux) {
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
				FieldList = &CellAux->m_oFieldList ;
			} else {
				FieldList = &m_poOwner->GetCntData()->m_oFieldList ;
			} ;

			for ( ItF=FieldList->begin() ; ItF!=FieldList->end(); ++ItF )	 {
				if (m_FieldsType==ENG_STATIC_FIELDS) {
					//Solo los NO fusion
					if ( (*ItF).m_oIdCtField.StrLenTrim()>0 ) continue ;
				} ;

				Field = (*ItF) ;
				Field.m_Top		+= m_Top +  m_FieldYOffset ;
				Field.m_Left	+= m_Left + m_FieldXOffset ;

				ClENG_Analex::PrepareForAnalexCT( Field.m_oExpr, m_oCTIndex, &StrAux ) ;
				Field.m_oExpr.StrCpy(StrAux) ;

				Field.m_oIdCtField.StrCpy("") ;	//Para que luego filtros no lo ignoren

				//Multiplexacion en diseño de Id de PI de CT

				if (m_FieldsType==ENG_TEMPLATE_H_FIELDS) {
					if (Field.m_oIdPi.StrLenTrim()>0) {
						Field.m_oIdPi.StrCat("_") ;
						Field.m_oIdPi.StrCat(m_oCTIndex) ;
					} ;
				} ;

				ao_poCntData->m_oFieldList.push_back( Field ) ;
			} ;


		} else {

			//En modo row es mas complicado: matching Template-Cnt por IdCTField.

			CellAux = m_poOwner->GetTemplate()->GetCell( m_iLevel, m_Cell ) ;
			if (!CellAux) {
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
			FieldList  = & CellAux->m_oFieldList ;
			FieldList2 = & m_poOwner->GetCntData()->m_oFieldList ;

			for ( ItF=FieldList->begin() ; ItF!=FieldList->end(); ++ItF )	 {
				for ( ItF2=FieldList2->begin() ; ItF2!=FieldList2->end(); ++ItF2 ) {
					if (  ClMIT_Str::StrICmpTrim( 
						(*ItF).m_oIdCtField, (*ItF2).m_oIdCtField )==0 ) 
					{
						break ;
					} ;
				} ;
				if ( ItF2==FieldList2->end() ) continue ;
				
				Field = (*ItF2) ;
				Field.m_Top		+= m_Top + m_FieldYOffset ;
				Field.m_Left	= (*ItF).m_Left + m_Left + m_FieldXOffset ;
				Field.m_Width	= (*ItF).m_Width ;

				ClENG_Analex::PrepareForAnalexCT( Field.m_oExpr, m_oCTIndex, &StrAux ) ;
				Field.m_oExpr.StrCpy(StrAux) ;

				Field.m_oIdCtField.StrCpy("") ;	//Para que luego filtros no lo ignoren

				//Multiplexacion en diseño de Id de PI de CT

				if (Field.m_oIdPi.StrLenTrim()>0) {
					Field.m_oIdPi.StrCat("_") ;
					Field.m_oIdPi.StrCat(m_oCTIndex) ;
				} ;

				ao_poCntData->m_oFieldList.push_back( Field ) ;
			} ;
		} ;
	} ;


	//****Recorre hijos

	for ( It=m_poSon->m_oList.begin(); It!=m_poSon->m_oList.end(); ++It ) {
		(*It).AddFields(ai_poDT,ao_poCntData,ai_bSwAddFields,ai_bSwTestCell) ;
	} ;

exit:
	return SwOk ;
}

//=======================================================================ClENG_CntDisplay

void ClENG_CntDisplay::StPage::End() 
{
	m_oLevel0.clear() ;
}

void ClENG_CntDisplay::StPage::Init(const ClENG_CntDisplay::StPage *ai_poSource) 
{
	End() ;

	m_Top		= ai_poSource->m_Top ;
	m_Left		= ai_poSource->m_Left ;
	m_Height	= ai_poSource->m_Height ;
	m_Width		= ai_poSource->m_Width ;
	m_oLevel0	= ai_poSource->m_oLevel0 ;
	m_iPage		= ai_poSource->m_iPage ;
}


void ClENG_CntDisplay::Init(const ClENG_CntDisplay *ai_poSource) 
{
	m4int16_t B, BC ;

	ClENG_CntDisplay::End() ;

	m_uiIdSection	= ai_poSource->m_uiIdSection ; 
	m_TypeCnt		= ai_poSource->m_TypeCnt ;
	m_poCnt			= ai_poSource->m_poCnt ; 
	m_poCT_T		= ai_poSource->m_poCT_T ;	
	m_oPage			= ai_poSource->m_oPage ;
}


ClENG_CntDisplay::ClENG_CntDisplay()
{
	m_poInsp = GET_INSP() ;
	m_poCnt=NULL ;
	m_poCT_T=NULL ;
}

ClENG_CntDisplay::ClENG_CntDisplay(const ClENG_CntDisplay & ai_oSource) 
{ 
	m_poInsp=ai_oSource.m_poInsp ; 
	m_poCnt=NULL ;
	m_poCT_T=NULL ;
	Init(&ai_oSource) ; 
} 

ClENG_CntDisplay & ClENG_CntDisplay::operator = (const ClENG_CntDisplay &ai_oOpe) 
{ 
	m_poInsp=ai_oOpe.m_poInsp ; 
	m_poCnt=NULL ;
	m_poCT_T=NULL ;
	Init(&ai_oOpe) ; 
	return *this ;
} 


void ClENG_CntDisplay::End()
{
	m_poCnt=NULL ;
	m_poCT_T=NULL ;
	m_oPage.clear() ;
}


m4bool_t ClENG_CntDisplay::Init( 
	m4uint32_t ai_uiIdSection, ENG_TYPE_CONTAINER ai_TypeCnt, StENG_DT_CntData *ai_poCnt, 
	ClENG_CT_T *ai_poCT_T, ClENG_KeyTot *ai_poDataTree, m4int16_t ai_iTogLevel ) 
{
	m4bool_t SwOk=M4_TRUE ;

	ClENG_CntDisplay::End() ;

	m_uiIdSection	= ai_uiIdSection ;
	m_TypeCnt		= ai_TypeCnt ;
	m_poCnt			= ai_poCnt ;
	if ( m_poCnt->m_MergeMode == ENG_CT_NO_MERGE ) {

		StPage Cnt ;
		ClENG_DisplayCell Cell ;

		Cell.Init(this) ;

		//**** Tratamiento NO CT

		ai_poCT_T = NULL ;
		ai_poDataTree = NULL ;
		m_poCT_T = ai_poCT_T ;

		//Construcción plantilla: immediata, simplemente enlazamos con el Cnt de fusion

		Cell.m_FieldsType		= ENG_STATIC_FIELDS ;
		Cell.m_Top				= 0 ;
		Cell.m_Left				= 0 ;
		Cell.m_Width			= m_poCnt->m_Width  ;
		Cell.m_Height			= m_poCnt->m_Height ;
		Cell.m_FieldYOffset		= 0 ;
		Cell.m_FieldXOffset		= 0 ;
		Cnt.m_oLevel0.push_back(Cell) ;
		Cnt.m_Top				= m_poCnt->m_Top ;
		Cnt.m_Left				= m_poCnt->m_Left ;
		Cnt.m_Width				= m_poCnt->m_Width ;
		Cnt.m_Height			= m_poCnt->m_Height ;
		Cnt.m_iPage				= m_poCnt->m_iPage  ;
		m_oPage.push_back(Cnt) ;

	} else {
		ClENG_CntDisplayBuilder cntBuilder ;
		DvcUnit_t LevelWidth ;
		m4bool_t  LevelPrinted ;

		//**** Tratamiento CT

		m_poCT_T = ai_poCT_T ;
		if ( ! m_poCT_T || ! ai_poDataTree ) {
			/*##Poner num msg*/
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Se esta intentando fusionar un container y no existe la template asociada "
				"o falta el arbol de claves", ClMIT_Msg::MIXED_OUT) 
				<< SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( m_poCT_T->GetLevelCount() != ai_poDataTree->GetLevelCount() || 
			m_poCT_T->GetLevelCount()==0 ) 
		{
			/*##Poner num msg*/
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Se esta intentando fusionar un container y la cuenta de niveles de la "
				"template (%0:s) no coincide con la del arbol de claves (%1:s), o ambas son 0"
				, ClMIT_Msg::MIXED_OUT) 
				<< m_poCT_T->GetLevelCount() << ai_poDataTree->GetLevelCount() << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		//****Llamar al Builder, genera toda la información de breaks y distribución 
		//	de cells

		if ( !  cntBuilder.Init(this, ai_poDataTree, ai_iTogLevel ) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;


		//****Convierte si procede a modo TO_ROW

		if ( ai_poCnt->m_MergeMode==ENG_CT_ROW_MERGE ) {
			ToRowMerge() ;
		} ;


		
	} ;

exit:

	return SwOk ;
}

void ClENG_CntDisplay::Trace() 
{
	PageList_t::iterator It ;
	ENG_CellList_t::iterator ItCell ;
	ClMIT_Str CtIdAux ;
	m4int16_t B ;

	for ( It=m_oPage.begin(), B=0 ; It!=m_oPage.end(); ++It, ++B ) {

		if ( m_poCT_T ) {
			CtIdAux.StrCpy(m_poCT_T->GetCT()->m_oCTId) ;
		} else {
			CtIdAux.StrCpy("") ;
		} ;

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"CONTAINER DISPLAY TRACE PAGE %0:s", ClMIT_Msg::ENTRY_FORMAT) 
				<< B << CRAZY_MSG << SEND_MSG   ;
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"CTId %0:s IdSection %1:s IdContainer %2:s Height %3:s milim "
				"Width %4:s milim Page %5:s"  ) 
				<< CtIdAux.InChar(0) << m_uiIdSection << (m4int16_t)m_TypeCnt
				<< ClMetric::DvcToMilim((*It).m_Height)
				<< ClMetric::DvcToMilim((*It).m_Width)
				<< (*It).m_iPage
				<< CRAZY_MSG << SEND_MSG   ;
		};
		#endif

		for ( ItCell=(*It).m_oLevel0.begin() ; ItCell!=(*It).m_oLevel0.end(); ++ItCell ) {
			(*ItCell).Trace() ;
		} ;

		#ifdef RPT_ENGINE_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"CONTAINER DISPLAY TRACE", ClMIT_Msg::EXIT_FORMAT) 
				<< CRAZY_MSG << SEND_MSG   ;
		};
		#endif
		;
	} ;
}

void ClENG_CntDisplay::Serialize( ClENG_VB_Serialize *ai_poSerial ) 
{
	PageList_t::iterator It ;
	ENG_CellList_t::iterator ItCell ;
	ClMIT_Str CtIdAux ;

	for ( It=m_oPage.begin() ; It!=m_oPage.end(); ++It ) {

		//***CntEOF
		ai_poSerial->SerializeBool(M4_FALSE) ;	
		
		//***CTId
		
		if ( m_poCT_T ) {
			CtIdAux.StrCpy(m_poCT_T->GetCT()->m_oCTId) ;
		} else {
			CtIdAux.StrCpy("") ;
		} ;
		ai_poSerial->SerializeStr(CtIdAux);
		
		//***IdSection
		ai_poSerial->SerializeULong( m_uiIdSection ) ;	

		//***IdContainer
		ai_poSerial->SerializeInt( m_TypeCnt ) ;	

		//***Height
		ai_poSerial->SerializeLong( (*It).m_Height ) ;	

		//***Width
		ai_poSerial->SerializeLong( (*It).m_Width ) ;	

		//***Page
		ai_poSerial->SerializeInt( (*It).m_iPage ) ;	

		//***Celdas

		for ( ItCell=(*It).m_oLevel0.begin() ; ItCell!=(*It).m_oLevel0.end(); ++ItCell ) {
			(*ItCell).Serialize(ai_poSerial) ;
		} ;

		//***CellEOF
		ai_poSerial->SerializeBool( M4_TRUE ) ;	
	} ;
}

m4bool_t ClENG_CntDisplay::CreateCntData( m4int16_t ai_iPage, 
	ClENG_DesignTree *ai_poDT, StENG_DT_CntData *ao_poCntData, m4bool_t ai_bSwTestCell ) 
{
	m4bool_t SwOk=M4_TRUE ;
	PageList_t::iterator It ;
	ENG_CellList_t::iterator ItCell ;
	m4int16_t C ;

	//Busca StPage 

	for ( It=m_oPage.begin(), C=ai_iPage; It!=m_oPage.end(), C ; ++It, --C ) ;
	if ( It==m_oPage.end() ) {
		/*##Num error*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"CreatenCntData failed, page %0:s not found", ClMIT_Msg::MIXED_OUT) 
			<< ai_iPage << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	//Cnt: Datos de partida, los del container de fusion, con lista Fields vacia.

	ao_poCntData->Init( m_poCnt ) ;
	ao_poCntData->m_oFieldList.clear() ;

	//Cnt: Top, Left, ect, los calculados

	ao_poCntData->m_Top		= (*It).m_Top ;
	ao_poCntData->m_Left	= (*It).m_Left ;
	ao_poCntData->m_Height	= (*It).m_Height ;
	ao_poCntData->m_Width	= (*It).m_Width ;
	ao_poCntData->m_iPage	= (*It).m_iPage ;

	//Añadir fields y frames. 
	//	Primera pasada añade Fields que simulan celdas. Deben ir todos al principio
	//	Segunda pasada añade los Fields en si.
	
	for ( ItCell=(*It).m_oLevel0.begin(); ItCell!=(*It).m_oLevel0.end(); ++ItCell ) {
		SwOk = M4_BOOL( SwOk && (*ItCell).AddFields( ai_poDT, ao_poCntData, M4_FALSE, ai_bSwTestCell ) ) ;
		if (!SwOk) goto exit ;
	} ;
	for ( ItCell=(*It).m_oLevel0.begin(); ItCell!=(*It).m_oLevel0.end(); ++ItCell ) {
		SwOk = M4_BOOL( SwOk && (*ItCell).AddFields( ai_poDT, ao_poCntData, M4_TRUE, ai_bSwTestCell ) ) ;
		if (!SwOk) goto exit ;
	} ;

exit:
	return SwOk ;
}

void ClENG_CntDisplay::ToRowMerge() 
{
	PageList_t::iterator ItPage ;
	ENG_CellList_t::iterator ItCell, ItNext, ItInsert ;
	ENG_CellList_t *Level0 ;
	
	for ( ItPage=m_oPage.begin() ; ItPage!=m_oPage.end(); ++ItPage ) {
		Level0 = & (*ItPage).m_oLevel0 ;
		for ( ItCell=Level0->begin() ; ItCell!=Level0->end(); ItCell=ItNext ) {

			//Se guarda ItNext, porque ItCell puede ser borrada y seria ilegal in ++ItCell.
			ItNext    = ItCell ; 
			++ItNext ;

			if ( (*ItCell).m_poSon->m_oList.size() != 0 ) {

				//SOLO se pasa sobre celdas de nivel raiz que NO son nodos
				//Se inserta siempre en Level0, despues del ItCell no pagina.

				ItInsert = ItCell ;
				AddPageNode( &(*ItCell), Level0, ItInsert )  ;

				//Ahora se borra el ItCell actual, pues ha sido sustiyuido por los hijos.

				Level0->erase( ItCell ) ;

			} ;
		} ;

		//Ahora se retocan los datos necesarios sobre la lista plana (un solo nivel).

		(*ItPage).m_Height = m_poCnt->m_Height  ;

		for ( ItCell=Level0->begin() ; ItCell!=Level0->end(); ++ItCell ) {
			(*ItCell).m_Top		= 0 ;
			(*ItCell).m_Height	= m_poCnt->m_Height ;
		} ;
	} ;
}

void ClENG_CntDisplay::AddPageNode( ClENG_DisplayCell *ai_poNode, 
		ENG_CellList_t *ai_poInsertList, ENG_CellList_t::iterator &ai_InsertFrom  ) 
{
	ENG_CellList_t *SonList = &ai_poNode->m_poSon->m_oList ;
	ENG_CellList_t::iterator ItCell ;

	if ( SonList->size()==0 ) {

		//Ademas de insertar, actualiza ai_InsertFrom para que sucesivas inserciones vayan
		//por orden de aparicion.

		ai_InsertFrom = ai_poInsertList->insert( ai_InsertFrom, 
				ENG_CellList_t::value_type(*ai_poNode) ) ;

	} else {
	
		for ( ItCell=SonList->begin() ; ItCell!=SonList->end(); ++ItCell ) {
			AddPageNode( &(*ItCell), ai_poInsertList, ai_InsertFrom )  ;
		} ;

	} ;
}


//===================================================Auxiliares  ClENG_CntDisplayBuilder


StENG_BuildingCell::StENG_BuildingCell(const StENG_BuildingCell& ai_oSource) 
{
	m_poSon = NULL ;
	m_poInsp = GET_INSP() ;

	Init(&ai_oSource) ; 
} 

StENG_BuildingCell & StENG_BuildingCell::operator = (const StENG_BuildingCell &ai_oOpe) 
{ 
	m_poSon = NULL ;
	m_poInsp = GET_INSP() ;

	Init(&ai_oOpe) ; 
	return *this ;
} 
	
StENG_BuildingCell::StENG_BuildingCell() 
{
	m_poSon = NULL ;
	m_poInsp = GET_INSP() ;
}

void StENG_BuildingCell::End() 
{
	m_Keys.clear() ;
	M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poSon, StENG_BuildingCellList ) ;
}


void StENG_BuildingCell::MarkAsDummy(DvcUnit_t ai_SubstractMinWidth) 
{
/*
	if ( m_MinWidth - ai_SubstractMinWidth > 0) {
		m_MinWidth -= ai_SubstractMinWidth ;
	} ;
*/
	m_MinWidth = M4_MAX(0, m_MinWidth - ai_SubstractMinWidth) ;
	m_bDummy = M4_TRUE ;
}

void StENG_BuildingCell::Trace(StENG_BuildingCellList *ai_poTree)
{
	#ifndef RPT_ENGINE_TRACE
	return ;
	#endif
	StMIT_Inspector* insp = GET_INSP();	

	if(insp->m_poTrace->IsTraceCodeRipped()) {
		return ;
	} ;


	*insp->m_poTrace << ClMIT_Msg( GROUP_ID, "StENG_BuildingCell::Trace()", 
		ClMIT_Msg::ENTRY_FORMAT )  << CRAZY_MSG << SEND_MSG   ;

	TraceLevel(ai_poTree) ;

	*insp->m_poTrace << ClMIT_Msg( GROUP_ID, "StENG_BuildingCell::Trace()",
		ClMIT_Msg::EXIT_FORMAT ) << CRAZY_MSG << SEND_MSG  ;
} ;

void StENG_BuildingCell::TraceLevel(StENG_BuildingCellList *ai_poList) 
{
	StMIT_Inspector* insp = GET_INSP();	

	ENG_BuildingCellList_t::iterator it ;
	StENG_BuildingCell *cell ;
	ClMIT_Str strAux ;

	for ( it=ai_poList->m_oList.begin() ; it!=ai_poList->m_oList.end(); ++it ) {
		cell = &(*it) ;
		cell->GetKeyString(&strAux) ;
		*insp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			" CellType %0:s Level %1:s Index %2:s SwDummy %3:s MinWidth %4:s StrKey %5:s" )
			<< ClENG_DesignTree::GetCellTypeTraceStr( cell->GetCellType() ) 
			<< cell->GetLevel() << cell->GetIndex() 
			<< cell->IsDummy() << cell->GetMinWidth() << strAux.InChar(0)
			<< CRAZY_MSG << SEND_MSG   ;
		if ( ! cell->IsLeaf() ) {
			*insp->m_poTrace << ClMIT_Msg( GROUP_ID, "", ClMIT_Msg::ENTRY_FORMAT ) 
				<< CRAZY_MSG << SEND_MSG ;
			TraceLevel(cell->GetSonList()) ;
			*insp->m_poTrace << ClMIT_Msg( GROUP_ID, "", ClMIT_Msg::EXIT_FORMAT ) 
				<< CRAZY_MSG << SEND_MSG ;
		} ;
	} ;

}

StENG_BuildingCell * StENG_BuildingCell::Init(
	StENG_BuildingCellList *ai_poTree,
	StENG_BuildingCell *ai_poFather, m4int16_t ai_iLevel, 
	ENG_CT_CELL_TYPE ai_CellType, m4int16_t ai_iIndex, 	
	m4bool_t ai_bDummy, DvcUnit_t ai_MinWidth
	) 
{
	StENG_BuildingCell::End() ;

	StKey keyAux;

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poSon, StENG_BuildingCellList ) ;

	keyAux.m_iLevel = ai_iLevel ;
	keyAux.m_CellType = ai_CellType ;
	keyAux.m_iIndex =  ai_iIndex ;
	m_bDummy = ai_bDummy ;
	m_MinWidth = ai_MinWidth ;

	if (ai_poFather) {

		m_Keys = ai_poFather->m_Keys ;
		m_Keys.push_back(keyAux) ;
		ai_poFather->m_poSon->m_oList.push_back(*this) ;

		return  & ai_poFather->m_poSon->m_oList.back() ;

	} else {
	
		m_Keys.push_back(keyAux) ;
		ai_poTree->m_oList.push_back(*this) ;
		
		return  & ai_poTree->m_oList.back() ;
	} ;
}


void StENG_BuildingCell::Init(const StENG_BuildingCell *ai_poSource) 
{
	StENG_BuildingCell::End() ;

	StKey keyAux;

	M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_LOADER, m_poSon, StENG_BuildingCellList ) ;

	m_bDummy  = ai_poSource->m_bDummy ;		
	m_MinWidth  = ai_poSource->m_MinWidth ;

	*m_poSon =  *ai_poSource->m_poSon ; 
	m_Keys = ai_poSource->m_Keys ;
}

m4bool_t StENG_BuildingCell::IsLeaf() 
{ 
	return m_poSon->m_oList.size()==0 ; 
}

m4bool_t StENG_BuildingCell::EqualsTo(StKey *ai_poKey) 
{
	StKey *keyAuxA = &m_Keys.back() ;
	return 
		keyAuxA->m_iLevel==ai_poKey->m_iLevel && 
		keyAuxA->m_CellType== ai_poKey->m_CellType &&	
		keyAuxA->m_iIndex==ai_poKey->m_iIndex ;
}

m4bool_t StENG_BuildingCell::EqualsTo(StENG_BuildingCell *ai_poCell) 
{
	return EqualsTo(&ai_poCell->m_Keys.back()) ;
}

m4bool_t StENG_BuildingCell::CopyToAnotherTree(StENG_BuildingCellList *ai_poTree) 
{
	m4bool_t SwOk=M4_TRUE, SwFound ;
	m4int16_t level ;
	StENG_BuildingCellList *insertInto ;
	StENG_BuildingCell *cellFound ;

	//Localiza la lista en el origen donde insertar

	if (m_Keys.size()==1) {
		//Nivel raiz
		insertInto=ai_poTree ;
	} else {
		cellFound = GetCell(ai_poTree, &m_Keys, m_Keys.size()-2) ;
		if (cellFound) {
			insertInto = cellFound->m_poSon ;
		} else {
			insertInto=NULL ;
		} ;

	} ;

	if (insertInto) {
		insertInto->m_oList.push_back(*this) ;
		//OJO, borrar hijos !
		insertInto->m_oList.back().m_poSon->m_oList.clear() ;
	} ;

	return insertInto!=NULL ;
}


StENG_BuildingCell *StENG_BuildingCell::GetCell(StENG_BuildingCellList *ai_poTree,
		KeyVector_t *ai_Keys, m4int16_t ai_Level)
{
	m4int16_t level, topLevel= 
		M4_IN_RANGE(ai_Level, 0, ai_Keys->size()-1) ? ai_Level : ai_Keys->size()-1 ;
	StENG_BuildingCellList *listInto ;
	ENG_BuildingCellList_t::iterator it ;
	StENG_BuildingCell *cellFound ;

	//Localiza la lista donde buscar
	
	for (  
		listInto=ai_poTree, cellFound=NULL, level=0 ; 
		level <= topLevel; 
		++level ) 
	{
		//Busca en insert Into donde insertar
		for (
			it=listInto->m_oList.begin(), cellFound=NULL; 
			it!=listInto->m_oList.end(); 
			++it ) 
		{
			if ( (*it).EqualsTo( & (*ai_Keys)[level] ) ) {
				cellFound=&(*it);
				listInto = (*it).m_poSon  ;
				break ;
			} ;
		} ;
		if (!cellFound) break ;
	} ;
	return cellFound ;
}

StENG_BuildingCell *StENG_BuildingCell::GetCell(StENG_BuildingCellList *ai_poTree, m4int16_t ai_Level) 
{
	return GetCell(ai_poTree, &m_Keys, ai_Level) ;
}

StENG_BuildingCell *StENG_BuildingCell::GetNextBrother(
	StENG_BuildingCellList *ai_poTree, m4int16_t ai_Deep ) 
{
	StENG_BuildingCellList *father ;
	StENG_BuildingCell *cellAux ;
	ENG_BuildingCellList_t::iterator it, cellFound ;
	
	if (ai_Deep<0) {
		ai_Deep = GetLevel() ;
	} ;
	if ( ai_Deep==0) {
		father = ai_poTree ;
	} else {
		cellAux = GetCell(ai_poTree, ai_Deep-1) ;
		if (cellAux) {
			father = cellAux->GetSonList() ;
		} else {
			father = NULL ;
		} ;
	} ;

	if (father) {

		for (
			it=father->m_oList.begin(), cellFound=father->m_oList.end(); 
			it!=father->m_oList.end(); 
			++it ) 
		{
			if ( (*it).EqualsTo( & m_Keys[ai_Deep] ) ) {
				cellFound=it;
				break ;
			} ;
		} ;
		
		if ( cellFound != father->m_oList.end() ) {
			++cellFound ;
		}

		if ( cellFound != father->m_oList.end() ) {
			return  &(*cellFound) ;
		} else {
			return NULL ;
		} ;

	} else {
	
		return NULL ;
	
	} ;


}

void StENG_BuildingCell::GetKeyString(ClMIT_Str *ao_poKeyStr) 
{
	m4int16_t B ;
	m4char_t StrNumber[20] ; 

	ao_poKeyStr->StrCpy("") ;
	for (B=0; B<m_Keys.size(); ++B) {
		sprintf(StrNumber, (B>0 ? "_%i" : "%i" ), (int)m_Keys[B].m_iIndex ) ;
		ClMIT_Str::OverStringValid(StrNumber,20) ;
		ao_poKeyStr->StrCat(StrNumber) ;
	} ;
}



m4bool_t StENG_BuildingCell::TogetherBreakNeeded( 
	StENG_BuildingCellList *ai_Tree,  
	m4int16_t ai_TogLevel, m4int16_t *ao_ExcludeLevel,	m4int16_t *ao_ExcludeIndex  )
{
	StENG_BuildingCell *togNextCell, *togThisCell ;
	m4int16_t level, togLevel ;

	//***Si no hay together, nunca se intenta

	if (ai_TogLevel<=0 ) {
		*ao_ExcludeLevel = *ao_ExcludeIndex = -1 ;
		return M4_FALSE ;
	}  else {
		togLevel = ai_TogLevel-1 ;
	} ;
	

	togThisCell = GetCell(ai_Tree, togLevel) ;
	if ( togThisCell->GetIndex()<0 ) {
		
		//***Celda a nivel together en pre/post total, no estamos en mitad de una 
		//	seleccion, seguno que NO hay candidato.
		
		*ao_ExcludeLevel = *ao_ExcludeIndex = -1 ;
		return M4_FALSE ;

	} else {

		//***Hay indice, puede haber break

		//***Primera causa; hijos no seleccionados

		for (level=togLevel+1 ; level<= GetLevel() ; ++level) {
			togNextCell = GetNextBrother( ai_Tree, level ) ;
			if (togNextCell) {
				togThisCell = GetCell(ai_Tree, togLevel) ;
				*ao_ExcludeLevel = togLevel ;
				*ao_ExcludeIndex = togThisCell->GetIndex() ;
				return M4_TRUE ;
			} ;
		} ;

		//***Segunda causa, aunque todos hijos estan selecc, hay celda hermana de 
		//	together con mismo el indice (si no es mismo indice o bien es otro bloque
		//	together o bien un pre/post total.

		togThisCell = GetCell( ai_Tree, togLevel ) ;
		togNextCell = GetNextBrother( ai_Tree, togLevel ) ;
		if ( togNextCell ) {
			if ( togThisCell->GetIndex() == togNextCell->GetIndex() ) {
				*ao_ExcludeLevel = togLevel ;
				*ao_ExcludeIndex = togThisCell->GetIndex() ;
				return M4_TRUE ;
			} ;
		} ;

	
	} ;

	*ao_ExcludeLevel = *ao_ExcludeIndex = -1 ;
	return M4_FALSE ;
} ;


//===============================================================ClENG_CntDisplayBuilder

/*

-------------------------------------------------------------------------
																		+
									Página								| 1
	 																	+	
		.----------------------------------------------------------------- ....
		|													+			+
		|							Container				| 2			|
		|													+			|
		|			.-------------------------------------------. +		|
		|			|								 		+	| |		|
		|			|				Celda display			| 3	| |		|
		|			|								 		+	| |		|
		|			|	.-----------------------------------. +	| |		|
		|			|	|									| |	| |		|
		|	B		| C	|		Celda parameterización		| |4| |		|
		|<--------->|<->|									| |	| | 5	| 6
		|			|	.-----------------------------------. +	| |		|
		|			|					D						| |		|
		|			|	<----------------------------------->	| |		|
		|			|											| |		|
		|			.-------------------------------------------. +		|
		|									E							|
	A	|			<------------------------------------------->		|
<------>|																|
		|																+
		.------------------------------------------------------------------	.....
											F
		<------------------------------------------------------------------ ...

A) StPage::m_Left
B) ClENG_DisplayCell::m_Left
C) ClENG_DisplayCell::m_FieldXOffset
D) (m_poCnt ó m_poCt->m_iLevel->m_Cell) m_Width
E) ClENG_DisplayCell::m_Width
F) StPage::m_Width 

1) StPage::m_Top
2) ClENG_DisplayCell::m_Top
3) ClENG_DisplayCell::m_FieldYOffset
4) (m_poCnt ó m_poCt->m_iLevel->m_Cell) m_Height
5) ClENG_DisplayCell::m_Height
6) StPage::m_Height

*/

ClENG_CntDisplayBuilder::ClENG_CntDisplayBuilder()
{
	m_poInsp = GET_INSP() ;
	m_poDisplayCnt=NULL ;
	m_poParamCnt = NULL ;
	m_poTemplate = NULL ;
	m_poCT = NULL ;
	m_piLevelHeight=NULL ;
}

void ClENG_CntDisplayBuilder::End()
{
	m_poDisplayCnt=NULL ;
	m_poParamCnt = NULL ;
	m_poTemplate = NULL ;
	m_poCT = NULL ;
	m_oPage.clear() ;
	if (m_piLevelHeight) {
		M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, 
			m_piLevelHeight, DvcUnit_t, m_iLevelCountAux ) ;
		m_iLevelCountAux=0 ;
	} ;
}

m4bool_t ClENG_CntDisplayBuilder::Init(
	ClENG_CntDisplay *ai_poDisplayCnt,
	ClENG_KeyTot *ai_poDataTree, m4int16_t ai_iTogLevel
	)
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_CntDisplayBuilder::End() ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_CntDisplayBuilder::Init()",
			ClMIT_Msg::ENTRY_FORMAT )
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

	m_poDisplayCnt		= ai_poDisplayCnt ;
	m_poTemplate		= m_poDisplayCnt->GetTemplate() ;
	m_poCT				= m_poTemplate->GetCT() ;
	m_poParamCnt		= m_poDisplayCnt->GetCntData() ;
	m_poDataTree		= ai_poDataTree ;
	m_MergePage			= m_poCT->m_MergePage ;	
	m_XColumnLeft		= m_poCT->m_XColumnLeft ;		
	m_XColumnRight		= m_poCT->m_XColumnRight ;	
	m_iTogLevel			= ai_iTogLevel != 
					ClENG_CntDisplay::TOG_LOAD_FROM_DESIGN ? ai_iTogLevel :
						m_poCT->m_iTogLevel ; 
	m_iLevelCount		=  m_poTemplate->GetLevelCount();
	m_MergeYMode        =  m_poParamCnt->m_MergeYMode;
	m_MergeXMode        =  m_poParamCnt->m_MergeXMode;

	//***********Primer paso, creamos las BuildingCells, todas en la primera página
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Firts step : Building cells",
			ClMIT_Msg::ENTRY_FORMAT ) << CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

	StENG_BuildingCellList listAux, *plistAux ;

	m_oPage.push_back( listAux ) ;
	plistAux = & m_oPage.back() ;
	BuildCellsForLevel(plistAux, 0, NULL) ;

	#ifdef RPT_ENGINE_TRACE

	StENG_BuildingCell::Trace(plistAux);

	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Firts step : Building cells",
			ClMIT_Msg::EXIT_FORMAT ) << CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

	
	//***********Segundo paso, las distribuimos por paginas si no caben en la actual

	if ( ! ApplyBreaks() ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//***********Tercer paso, creamos en las display cells como un reflejo de las
	//	building cells ya construidas, no hay calculos de saltos de pagina ni nada 
	//	parecido, todo está hecho. Los únicos calculos son de offsets y similares.

	BuildDisplayCnt() ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_CntDisplayBuilder::Init()"
			, ClMIT_Msg::EXIT_FORMAT )
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

	return SwOk ;
} ;


void ClENG_CntDisplayBuilder::BuildCellsForLevel(
	StENG_BuildingCellList *ai_poTree, 
	m4int16_t ai_iLevel, StENG_BuildingCell *ai_poFather) 
{
	ClENG_KeyTot::StInfo totInfo ;
	m4int16_t BK, BKtop ;
	StENG_BuildingCell cell, *pCell ;
	StENG_DT_CTCell * templCell ;

	//***PRE TOTAL. al principio, ni itera ni tiene hijos

	templCell = m_poTemplate->GetCell( ai_iLevel, ENG_CT_PRE_T ) ;
	if ( templCell->m_bLoaded ) {
		cell.Init(ai_poTree, ai_poFather, ai_iLevel, ENG_CT_PRE_T, -1, M4_FALSE, templCell->m_Width ) ;
	} ;

	//***ZONA de hijos que iteran por clave

	//Llamadas niveles padre ya han posicionado nodos clave padre !!!
	m_poDataTree->GetInfoByIndex(ClENG_KeyTot::FIND_REQ,ai_iLevel,0,0,&totInfo) ;
	BKtop = totInfo.m_oSonCount ;
	for (BK=0; BK<BKtop; ++BK) {

		//***PRE SUBTOTAL, itera pero no tiene hijos

		templCell = m_poTemplate->GetCell( ai_iLevel, ENG_CT_PRE_S) ;
		if ( templCell->m_bLoaded ) {
			cell.Init(ai_poTree, ai_poFather, ai_iLevel, ENG_CT_PRE_S, BK, M4_FALSE, templCell->m_Width ) ;
		} ;

		//***SUB, ademas de iterar tiene hijos

		templCell = m_poTemplate->GetCell( ai_iLevel, ENG_CT_SUB) ;
		if ( templCell->m_bLoaded ) {
			pCell = cell.Init(ai_poTree, ai_poFather, ai_iLevel, ENG_CT_SUB, BK, M4_FALSE, templCell->m_Width ) ;

			m_poDataTree->SetIndexKey(ai_iLevel,BK);

			if ( ai_iLevel < (m_poTemplate->GetLevelCount()-1) ) {
				BuildCellsForLevel(ai_poTree, ai_iLevel+1, pCell)  ;
			} ;
		} ;

		//***POST SUBTOTAL, itera pero no tiene hijos

		templCell = m_poTemplate->GetCell( ai_iLevel, ENG_CT_POST_S) ;
		if ( templCell->m_bLoaded ) {
			cell.Init(ai_poTree, ai_poFather, ai_iLevel, ENG_CT_POST_S, BK, M4_FALSE, templCell->m_Width ) ;
		} ;

	} ;

	//***POST TOTAL. al final, ni itera ni tiene hijos

	templCell = m_poTemplate->GetCell( ai_iLevel, ENG_CT_POST_T ) ;
	if ( templCell->m_bLoaded ) {
		cell.Init(ai_poTree, ai_poFather, ai_iLevel, ENG_CT_POST_T, -1, M4_FALSE, templCell->m_Width ) ;
	} ;
}

void ClENG_CntDisplayBuilder::SelectCells(
	StENG_BuildingCellList *ai_Page, DvcUnit_t ai_DvcMaxWidth, 
	m4int16_t &ao_LeafCount, 
	ENG_BuildingCellList_t::iterator &ao_LastLeaf, 
	m4bool_t &ao_SwAllSelected,
	StENG_BuildingCellList *ai_NewPage,
	m4int16_t ai_ExcludeLevel, m4int16_t ai_ExcludeIndex ) 
{
	DvcUnit_t left ;

	if (ai_NewPage) {
		
		//Si hago el move, muevo todo a la nueva pagina.
		//Invierto los punteros ai_Page y ai_NewPage para que el resto del proceso siga
		//	igual; ai_Page es el destino desde el que seleccionar y ai_NewPage al que 
		//	mover.

		StENG_BuildingCellList *paux ;
		
		ai_NewPage->m_oList = ai_Page->m_oList;
		ai_Page->m_oList.clear() ;
		paux = ai_Page ;
		ai_Page = ai_NewPage ;
		ai_NewPage = paux ;
	} ;

	//Reset valores de salida y proceso

	ao_LeafCount = 0 ;
	ao_LastLeaf  = ai_Page->m_oList.end() ;
	ao_SwAllSelected = M4_TRUE ;
	left = ai_DvcMaxWidth ;

	//Proceso recursivo

	SelectCellsAux( ai_Page, left, ao_LeafCount, 
		ao_LastLeaf, 
		ao_SwAllSelected, ai_NewPage, ai_ExcludeLevel, ai_ExcludeIndex  ) ;
}

void ClENG_CntDisplayBuilder::SelectCellsAux(
	StENG_BuildingCellList *ai_Cells, DvcUnit_t &ao_DvcLeft, 
	m4int16_t &ao_LeafCount, 
	ENG_BuildingCellList_t::iterator &ao_LastLeaf, 
	m4bool_t &ao_SwAllSelected,
	StENG_BuildingCellList *ai_NewPage,
	m4int16_t ai_ExcludeLevel, m4int16_t ai_ExcludeIndex ) 
{
	ENG_BuildingCellList_t::iterator it, nextIt ;
	StENG_BuildingCell *cell ;
	DvcUnit_t sonWidth ;

	for (it=ai_Cells->m_oList.begin(); it !=ai_Cells->m_oList.end(); it=nextIt) {
		nextIt=it ;	++nextIt ; //Por si borramos el actual iterador
		cell = &(*it) ;

		//Si el minimo de la celda no cabe (tenga o no hijos), no cabrá nada más.
		if (cell->GetMinWidth() > ao_DvcLeft) {
			ao_SwAllSelected=M4_FALSE ;
			break ;
		} ;

		//Si hay control ExcludeIndex y se cumple, no cabrá nada más.
		if (ai_ExcludeIndex>=0) {
			if (cell->GetLevel()==ai_ExcludeLevel && cell->GetIndex()==ai_ExcludeIndex ) {
				ao_SwAllSelected=M4_FALSE ;
				break ;
			} ;
		} ;

		//Si hay hijos, controlar que llega a seleccionarse algún nodo hoja
		//Si no se selecciona ninguno, no cabrá mas.
		//Ojo, por si no se selecciona, usar valores dummy pra LastLeaf, etc, y NO
		//	hacer ninguna operación de movimiento.
		if ( ! cell->IsLeaf() ) {
			DvcUnit_t leftDummy = ao_DvcLeft ;
			m4int16_t leafCountDummy = 0 ;
			ENG_BuildingCellList_t::iterator lastLeafDummy ;
			m4bool_t allSelected=M4_FALSE ;

			SelectCellsAux(cell->GetSonList(), leftDummy, leafCountDummy, 
				lastLeafDummy, allSelected, NULL, -1, -1 ) ;
			if (leafCountDummy==0) {
				ao_SwAllSelected=M4_FALSE ;
				break ;
			} ;
		} ;

		//Esta celda se selecciona seguro. 
		// Para Left: Primero  proceder con hijos para saber que ocupan y ajustar ancho 
		//	del padre.
		// Para mover: Primero insertar padre, para que luego los hijos puedan 
		//	inserstarse en él.

		//Inserto cell en destino si debo
		
		if (ai_NewPage) {
			cell->CopyToAnotherTree(ai_NewPage) ;
		} ;

		//Calculos / inserciones / borrados  en hijos

		sonWidth = ao_DvcLeft ;
		SelectCellsAux(cell->GetSonList(), ao_DvcLeft, ao_LeafCount, 
			ao_LastLeaf, ao_SwAllSelected, ai_NewPage, ai_ExcludeLevel, ai_ExcludeIndex) ;
		sonWidth -= ao_DvcLeft ;

		//Ajuste Left de la celda padre

		if ( cell->GetMinWidth() > sonWidth ) {
			ao_DvcLeft -=  (cell->GetMinWidth()-sonWidth) ;
		} ;

		//Si es nodo hoja, actualiza contadores. Da igual orden (si tiene hijos no es leaf)
		if ( cell->IsLeaf() ) {
			++ao_LeafCount ;
			ao_LastLeaf = it ;
		} ;
		
		//Borro o marco cell padre en origen si debo
		
		if (ai_NewPage) {
			if ( ao_SwAllSelected) {
				
				//La unica forma de que ao_SwAllSelected=TRUE es que los hijos
				//	y el padre se hayan seleccionado.
				
				ai_Cells->m_oList.erase(it) ;

			} else {
				
				//Y la unica forma de que ao_SwAllSelected=FALSE es que algun hijo no
				//	se haya seleccionado.
				
				cell->MarkAsDummy( sonWidth ) ;

			} ;
		} ;

	} ;

}

DvcUnit_t ClENG_CntDisplayBuilder::GetMaxPageWidth(
	m4int16_t ai_IndexPage, 
	m4bool_t *ao_bSwStatic, m4bool_t *ao_bSwDummyStatic ) 
{
	DvcUnit_t levelX ; //Pos de comienzo ABSOLUTA a la izquierda
	m4bool_t swStatic, swDummyStatic ;

	
	if ( m_MergePage == ENG_CT_NONE_PAGE ) {
	
		swStatic=M4_FALSE ;
	
	} else {

		switch ( m_MergeYMode ) {
		
		case ENG_CT_DOWN_STC:
		case ENG_CT_OVER_STC:
			swStatic=M4_TRUE ;
			swDummyStatic = m_MergePage == ENG_CT_FIRST_PAGE ?
				(ai_IndexPage>0) : M4_FALSE ;
			break;
		
		case ENG_CT_ALIGN_STC:
		default:
			if ( m_MergePage == ENG_CT_FIRST_PAGE ) {
				swStatic = ai_IndexPage==0 ;
				swDummyStatic = M4_FALSE ;
			} else {
				swStatic = M4_TRUE ;
				swDummyStatic = M4_FALSE ;
			} ;
			break;
		} ;
	} ;

	levelX = m_poParamCnt->m_OffsetLeft ;

	if (swStatic) {
		if (m_MergeYMode==ENG_CT_ALIGN_STC) {
			levelX += m_poParamCnt->m_Width ;
		} ;
	} ;
	
	
	if (ao_bSwStatic) {
		*ao_bSwStatic = swStatic ;
	} ;
	if (ao_bSwDummyStatic) {
		*ao_bSwDummyStatic = swDummyStatic ;
	} ;

	return m_XColumnRight - levelX - m_poParamCnt->m_Left +1 ;
}



m4bool_t ClENG_CntDisplayBuilder::ApplyBreaks()
{
	m4bool_t SwOk=M4_TRUE  ;
	DvcUnit_t maxPageWidth ;
	m4int16_t leafCount ;
	ENG_BuildingCellList_t::iterator lastLeaf ;
	m4bool_t SwAllSelected ;
	StENG_BuildingCellList *page, *newPage, pageAux ;
	m4int16_t excludeLevel, excludeIndex ;
	m4int16_t indexPage ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Second step : apply breaks()",
			ClMIT_Msg::ENTRY_FORMAT )
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

	//***Si el ancho de hoja es infinito, no hay que hacer nada
	
	if (m_iTogLevel==ClENG_CntDisplay::TOG_NO_LIMIT) {
		goto exit ;
	} ;

	for ( page = & m_oPage.back(), indexPage=0 ; M4_TRUE ; ) {

		//****Si en la pagina actual cabe todo, ya hemos acabado.

		maxPageWidth = GetMaxPageWidth(indexPage) ;
		
		SelectCells(page, maxPageWidth, leafCount, lastLeaf, SwAllSelected ) ;
		if (SwAllSelected) break ;

		//****Si no cabe nada, control especial
		if ( leafCount==0 ) {
			
			if ( ! (indexPage==0 && m_MergePage==ENG_CT_FIRST_PAGE  
				&& m_MergeYMode ==ENG_CT_ALIGN_STC ) ) 
			{
				
				// Hay que evitar recursiones infinitas.
				// Solo puede no seleccionarse nada si si estamos en la primera hoja, 
				//  el modo del container estatico es ALIGN_Y y FIRST_PAGE. Otros casos
				//	son erroneos.

				SwOk=M4_FALSE ;
				goto exit ;

			} else {
			
				//No seleccionar nada; simplemente crear otra pagina nueva y mover todo 
				//	ahi.
				
				pageAux.m_oList = page->m_oList ;
				page->m_oList.clear() ;
				m_oPage.push_back(pageAux) ;
				page = & m_oPage.back() ;
				++indexPage ;

				continue ;
			} ;
		} ;

		//***A partir de aqui, todo es un proceso 'cabe algo, pero no todo'

		//***Vemos si podemos ajustar las rupturas por Together
		//Solo tenemos que controlar este caso si hay Together, y si estamos dejando una 
		//	impresion a medias en el nivel indicado.

		if ( (*lastLeaf).TogetherBreakNeeded(
			page, m_iTogLevel, &excludeLevel, &excludeIndex ) ) 
		{
			//BREAK por Together. Compruebo que se va a seleccionar algo con 
			//	las nuevas limitaciones
				
			SelectCells(page, maxPageWidth, leafCount, lastLeaf, 						
				SwAllSelected, NULL, excludeLevel, excludeIndex ) ;
			if ( leafCount<0 ) {
				//No se selecciona nada, no hay break, no cabria nada: excepcion, si se
				//	va a selecc un cnt estatico solo en esta pagina y en modo ALIGN, si
				//	dejamos que no se selecc nada; la sioguiente pagina tiene mas sitio.
				if ( ! (indexPage==0 && m_MergePage==ENG_CT_FIRST_PAGE  
					&& m_MergeYMode ==ENG_CT_ALIGN_STC ) )  
				{
					excludeLevel = -1;
					excludeIndex = -1; 
				} ;
			} ;
		} ;

		//***Efectuo el break de página 

		pageAux.m_oList.clear() ;
		m_oPage.push_back(pageAux) ;
		newPage = & m_oPage.back() ;
		SelectCells(page, maxPageWidth, leafCount, lastLeaf, 
			SwAllSelected, newPage, excludeLevel, excludeIndex ) ;
		page = newPage ;
		++indexPage ;

	} ;
exit:
	if ( !SwOk ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"No se puede generar container de crosstab. Pagina %0:s"
			, ClMIT_Msg::MIXED_OUT) 
			<< (m4uint32_t)(indexPage+1) << SEND_MSG ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		PageList_t::iterator itAux ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "There are %0:s Pages after cell breaks") 
			<< (m4uint32_t)m_oPage.size() << CRAZY_MSG << SEND_MSG ;
		for ( itAux=m_oPage.begin(), indexPage=0; itAux!=m_oPage.end(); ++itAux, ++indexPage ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Page %0:s") << 
				indexPage << CRAZY_MSG << SEND_MSG ;
			page = &(*itAux) ;
			StENG_BuildingCell::Trace(page) ;
		} ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Second step : apply breaks()",
			ClMIT_Msg::EXIT_FORMAT )
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif


	return SwOk ;
}

void ClENG_CntDisplayBuilder::BuildDisplayCnt()
{
	//Varibales que empiezan por dp : de display !
	PageList_t::iterator itPage ;
	m4int16_t indexPage, B ;
	
	ClENG_CntDisplay::StPage dpPage, *dpCnt ;
	ENG_CellList_t *dpCellList ;
	DvcUnit_t dpMaxPageWidth ;
	m4bool_t dpSwStatic, dpSwDummyStatic ;
	ClENG_DisplayCell *dpStaticCell, staticCellAux ;
	DvcUnit_t TopOfLeftCell ; //<0, no hay celda de relleno por offset left
	DvcUnit_t levelX ; //Offset de comienzo de celdas no static
	//true si falta de ajustar el ancho de la celda estatica a la del cnt de display
	m4bool_t swAdjustStaticCellWidth ; 
	DvcUnit_t heightIncr ; //Incr de altura del ultimo level debido a ajustes
	DvcUnit_t levelWidth  ;


	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Third step, building the Display Container",
			ClMIT_Msg::ENTRY_FORMAT )
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

	//***Bucle de paginas

	for ( 
		itPage=m_oPage.begin(), indexPage=0; 
		itPage!=m_oPage.end(); 
		++itPage, ++indexPage ) 
	{

		//Construye pagina contenedora de celdas de display
		
		dpPage.End() ;
		m_poDisplayCnt->m_oPage.push_back(dpPage) ;
		dpCnt = & m_poDisplayCnt->m_oPage.back() ;
		dpCnt->m_iPage =  indexPage;

		dpCellList = & dpCnt->m_oLevel0 ;
		dpMaxPageWidth = GetMaxPageWidth(indexPage, &dpSwStatic, &dpSwDummyStatic) ;

		//Reset valores de ajuste

		levelX						= m_poParamCnt->m_OffsetLeft ;
		swAdjustStaticCellWidth		= M4_FALSE ;
		heightIncr					= 0 ;

		if (dpSwStatic) {
			
			//*****Hay celda estatica

			//Construye dpStaticCell

			staticCellAux.Init(m_poDisplayCnt) ;
			dpCellList->push_back(staticCellAux) ;
			dpStaticCell = & dpCellList->back() ;
		
			//Datos StaticCell y ao_poDisplayCnt relacionados, varian por modo de fusion.
			//No aparece ENG_CT_RIGHT_STC porque por ahora es el unico modo permitido.

			dpStaticCell->m_FieldsType = ENG_STATIC_FIELDS ;
			dpStaticCell->m_bDummy = dpSwDummyStatic ;
			dpStaticCell->m_iLevel = 0 ;
			dpStaticCell->m_Cell  = (ENG_CT_CELL_TYPE)0 ;
			dpStaticCell->m_FieldYOffset = 0 ;
			dpStaticCell->m_FieldXOffset = 0 ;
			dpStaticCell->m_oCTIndex.StrCpy("") ;

			dpCnt->m_Top	= m_poParamCnt->m_Top ;
			dpCnt->m_Left   = m_poParamCnt->m_Left ;

			switch ( m_MergeYMode ) {
			case ENG_CT_OVER_STC :
				
				//Celda estatica abajo, resto arriba.

				dpStaticCell->m_Top		= m_poTemplate->GetAccLevelHeight(0) ;
				dpStaticCell->m_Left	= 0 ;
				//Ajustarlo luego a ao_poDisplayCnt->m_Width
				//dpStaticCell->m_Width=?? ;	
				m_OffTopYMode			= 0 ;
				dpStaticCell->m_Height	= m_poParamCnt->m_Height ;

				dpCnt->m_Height			= m_poTemplate->GetAccLevelHeight(0) + 
					dpStaticCell->m_Height ;	
				swAdjustStaticCellWidth=M4_TRUE ;
				//Ajustarlo luego += (*ao_poLevelWidth)
				dpCnt->m_Width			= m_poParamCnt->m_OffsetLeft  ;	

				TopOfLeftCell			= 0 ;

				break ;

			case ENG_CT_DOWN_STC:

				//Celda estatica arriba, resto abajo.
	
				dpStaticCell->m_Top		= 0 ;
				dpStaticCell->m_Left	= 0 ;
				//Ajustarlo luego a ao_poDisplayCnt->m_Width
				//dpStaticCell->m_Width=?? ;	
				m_OffTopYMode			= m_poParamCnt->m_Height ;
				dpStaticCell->m_Height	= m_poParamCnt->m_Height ;

				dpCnt->m_Height			= m_poTemplate->GetAccLevelHeight(0) + 
					dpStaticCell->m_Height ;	
				swAdjustStaticCellWidth=M4_TRUE ;
				//Ajustarlo luego += (*ao_poLevelWidth) ;
				dpCnt->m_Width			= m_poParamCnt->m_OffsetLeft  ;	

				TopOfLeftCell			= dpStaticCell->m_Height ;

				break ;

			case ENG_CT_ALIGN_STC:

				//Celda estatica alineada a derecha, resto izquierda.

				dpStaticCell->m_Top		= 0 ;
				dpStaticCell->m_Left	= 0 ;
				dpStaticCell->m_Width	= m_poParamCnt->m_Width  + 
					m_poParamCnt->m_OffsetLeft ;	
				dpStaticCell->m_Height	= 
					M4_MAX( m_poParamCnt->m_Height, m_poTemplate->GetAccLevelHeight(0)  ) ;
				m_OffTopYMode			= 0 ;

				//La altura tanto de la celda como el Cnt será igual, y será la más alta.
				//Tener en cuenta este posible incremento de altura extra en las celdas de CT.
				dpCnt->m_Height			= dpStaticCell->m_Height ; 
					M4_MAX( dpStaticCell->m_Height, m_poTemplate->GetAccLevelHeight(0)  ) ;
				dpCnt->m_Width			= m_poParamCnt->m_Width + 
					m_poParamCnt->m_OffsetLeft ;	
				heightIncr = dpCnt->m_Height - m_poTemplate->GetAccLevelHeight(0) ;

				levelX += m_poParamCnt->m_Width  ;

				TopOfLeftCell = -1 ;

				break ;
			} ;

			if (m_poParamCnt->m_OffsetLeft>0 && TopOfLeftCell>=0 ) {
				ClENG_DisplayCell FillLeftCell ;
				FillLeftCell.Init(m_poDisplayCnt) ;
				FillLeftCell.m_FieldsType = ENG_STATIC_FIELDS ;
				FillLeftCell.m_bDummy = M4_TRUE ;
				FillLeftCell.m_iLevel = 0 ;
				FillLeftCell.m_Cell  = (ENG_CT_CELL_TYPE)0 ;
				FillLeftCell.m_FieldYOffset = 0 ;
				FillLeftCell.m_FieldXOffset = 0 ;
				FillLeftCell.m_oCTIndex.StrCpy("") ;
				FillLeftCell.m_Top=TopOfLeftCell ; FillLeftCell.m_Left=0 ;
				FillLeftCell.m_Width = m_poParamCnt->m_OffsetLeft ;
				FillLeftCell.m_Height= m_poTemplate->GetAccLevelHeight(0) ;
				dpCellList->push_back(FillLeftCell) ;
			} ;

		} else {
		
			//*****No hay celda estatica

			dpStaticCell = NULL ;

			dpCnt->m_Top	= m_poParamCnt->m_Top ;
			dpCnt->m_Left	= m_XColumnLeft ;
			dpCnt->m_Width	= 0 ;
			m_OffTopYMode	= 0 ;
			if (indexPage>0) {
				//Si no hay en 2º instancia StaticCell, pero la hubiese antes, hay
				//	que preservar el alto.
				dpCnt->m_Height = m_poDisplayCnt->m_oPage.front().m_Height ;
			} else {
				dpCnt->m_Height = m_poTemplate->GetAccLevelHeight(0) ;
			} ;
		} ;

		//Calculo de LevelHeight, la altura de nivel siempre se cojerá de aqui

		if (m_piLevelHeight) {
			M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, 
				m_piLevelHeight, DvcUnit_t, m_iLevelCountAux ) ;
		} ;
		m_iLevelCountAux = m_poTemplate->GetLevelCount() ;
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_LOADER, 
			m_piLevelHeight, DvcUnit_t, m_iLevelCountAux ) ;
		for (B=0;B<m_iLevelCountAux;++B) {
			m_piLevelHeight[B] = m_poTemplate->GetLevelHeight(B) ;
		} ;
		for (B=m_iLevelCountAux-1; B >= 0; --B) {
			if ( m_piLevelHeight[B]>0 ) {
				m_piLevelHeight[B] += heightIncr ;
				break ;
			} ;
		} ;

		//Construccion de celdas de display de la pagina

		BuildDisplayLevel( 
			&(*itPage), dpCnt, &dpCnt->m_oLevel0, 0, levelX, &levelWidth)  ; 

		//Procesos de ajuste final del container 

		dpCnt->m_Width += levelWidth ;
		if ( swAdjustStaticCellWidth ) {
			dpStaticCell->m_Width = dpCnt->m_Width ;
		} ;

	} ;	//Fin bucle pagina


	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Third step, building the Display Container",
			ClMIT_Msg::EXIT_FORMAT )
			<< CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif

}

void ClENG_CntDisplayBuilder::BuildDisplayLevel( 
	StENG_BuildingCellList *ai_Cells,
	ClENG_CntDisplay::StPage *ai_dpCnt, ENG_CellList_t *ai_dpCellList,
	m4int16_t ai_iLevel, DvcUnit_t ai_LevelX, 
	DvcUnit_t *ao_poLevelWidth) 
{
	ENG_BuildingCellList_t::iterator cellIt ;
	StENG_BuildingCell *cell ;
	StENG_DT_CTCell *TemplCell ;
	m4bool_t SwOk=M4_TRUE ;
	ClENG_DisplayCell dpCellAux, *dpCell ;
	DvcUnit_t sonWidth ;

	*ao_poLevelWidth = 0 ;

	//Se sale sin consumir nada si level vacio

	if ( m_poTemplate->GetLevelHeight(ai_iLevel)==0 ) {
		*ao_poLevelWidth=0 ;
		goto exit ;
	} ;

	//Bucle tipos de celda

	for (cellIt=ai_Cells->m_oList.begin(); cellIt !=ai_Cells->m_oList.end(); ++cellIt) {

		cell = &(*cellIt) ;

		//Obtiene Cell Template a procesar, debe existir
		TemplCell = m_poTemplate->GetCell( ai_iLevel, cell->GetCellType() ) ;
		if ( ! TemplCell->m_bLoaded ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		//Limpia celda auxiliar
		dpCellAux.Init(m_poDisplayCnt) ;
		ai_dpCellList->push_back(dpCellAux) ;
		dpCell =  & ai_dpCellList->back() ;

		//Datos Cell calculados de igual forma en todos casos.

		dpCell->m_FieldsType		= m_poParamCnt->m_MergeMode == ENG_CT_HEADER_MERGE ? 
			ENG_TEMPLATE_H_FIELDS : ENG_TEMPLATE_R_FIELDS ;
		dpCell->m_iLevel			= cell->GetLevel() ;
		dpCell->m_Cell		= (ENG_CT_CELL_TYPE)cell->GetCellType(); 
		dpCell->m_Top		= GetTopLevelHeight( ai_iLevel ) + m_OffTopYMode ;
		dpCell->m_Left		= ai_LevelX ;
		dpCell->m_bDummy	=  cell->IsDummy() ;	 
		cell->GetKeyString( &dpCell->m_oCTIndex ) ;
		//Cojer ancho de building cell, para dummy cells puede ser menor que la de
		//	template !
		dpCell->m_Width		= cell->GetMinWidth() ;

		if ( cell->IsLeaf() ) {
		
			//****Celda sin hijos

			dpCell->m_Height	= GetAccLevelHeight(ai_iLevel) ;

		} else {

			//****Celda con hijos, ancho variable 

			dpCell->m_Height = GetLevelHeight(ai_iLevel) ; 

			//Imprimir hijas para saber ancho real DisplayCell.m_Width

			BuildDisplayLevel( cell->GetSonList(), ai_dpCnt, & dpCell->m_poSon->m_oList,
				ai_iLevel+1, ai_LevelX, &sonWidth)  ;

			if ( sonWidth > dpCell->m_Width) {
				//Si los hijos ocupan mas que esta celda, ajustar el ancho de esta celda
				
				dpCell->m_Width = sonWidth ;
			
			} else if (sonWidth < dpCell->m_Width) {

				//Si los hijos ocupan menos, hay que repartir el espacio sobrante entre los hijos

				DistributeWidthIncr( & dpCell->m_poSon->m_oList, 
					dpCell->m_Width - sonWidth, 0  ) ;

			} ;
		} ;

		//****Centrados CellParametrización en CellDisplay

		if ( m_poParamCnt->m_MergeMode == ENG_CT_HEADER_MERGE  ) 
		{
			if ( dpCell->m_Width != cell->GetMinWidth() ) {
				switch ( TemplCell->m_AlignParamCellX ) {
				case ENG_CT_CENTER_X_DISPLAY:
					dpCell->m_FieldXOffset = (dpCell->m_Width - cell->GetMinWidth())/2 ;
					break ;
				case ENG_CT_RIGHT_DISPLAY:
					dpCell->m_FieldXOffset = dpCell->m_Width - cell->GetMinWidth() ;
					break ;
				case ENG_CT_LEFT_DISPLAY:
				default:
					dpCell->m_FieldXOffset = 0 ;
					break ;
				} ;
			} else {
				dpCell->m_FieldXOffset = 0 ;
			} ;

			if ( dpCell->m_Height != TemplCell->m_Height ) {
				switch ( TemplCell->m_AlignParamCellY ) {
				case ENG_CT_CENTER_Y_DISPLAY:
					dpCell->m_FieldYOffset = (dpCell->m_Height - TemplCell->m_Height)/2 ;
					break ;
				case ENG_CT_BOTTOM_DISPLAY:
					dpCell->m_FieldYOffset = dpCell->m_Height - TemplCell->m_Height ;
					break ;
				case ENG_CT_TOP_DISPLAY:
				default:
					dpCell->m_FieldYOffset = 0 ;
					break ;
				} ;

			} else {
				dpCell->m_FieldYOffset = 0 ;
			} ;

		} else {
			
			dpCell->m_FieldXOffset = 0 ;
			dpCell->m_FieldYOffset = 0 ;
		} ;

		//Ajustar valores

		*ao_poLevelWidth += dpCell->m_Width ; 
		ai_LevelX		 += dpCell->m_Width ;

	} ;	

exit:

	if (!SwOk) {
		//Poner num msg
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"No se puede generar container de crosstab. Pagina %0:s, Nivel %1:s ",
			ClMIT_Msg::MIXED_OUT) 
			<< (m4uint32_t)m_oPage.size() << ai_iLevel << SEND_MSG ;
	} ;
}


DvcUnit_t ClENG_CntDisplayBuilder::GetAccLevelHeight( m4int16_t ai_Level ) 
{
	m4int16_t B ;
	DvcUnit_t R ;

	for ( B=ai_Level, R=0; B<m_iLevelCount; ++B ) R += m_piLevelHeight[B] ;
	return R ;
}

DvcUnit_t ClENG_CntDisplayBuilder::GetTopLevelHeight( m4int16_t ai_Level ) 
{
	m4int16_t B ;
	DvcUnit_t R ;

	for ( B=0, R=0; B<ai_Level; ++B ) R += m_piLevelHeight[B] ;
	return R ;
}

DvcUnit_t ClENG_CntDisplayBuilder::GetLevelHeight( m4int16_t ai_Level ) 
{
	return m_piLevelHeight[ai_Level] ;
}


void ClENG_CntDisplayBuilder::DistributeWidthIncr( ENG_CellList_t *ai_poList, DvcUnit_t ai_Incr, DvcUnit_t ai_LeftIncr ) 
{
	ENG_CellList_t::iterator It ;
	//Incrementos de celda, distribuye por igual. Para evitar redondeos, resto a al primero.
	DvcUnit_t FirstCellIncr, CellIncr, *Incr ;		
	StENG_DT_CTCell *TemplCell ;

	if (ai_poList->size()==0) return ;

	CellIncr		= ai_Incr / ai_poList->size() ;
	FirstCellIncr	= CellIncr + ai_Incr % ai_poList->size()  ;

	for ( It = ai_poList->begin(), Incr=&FirstCellIncr ; It != ai_poList->end(); 
		++It, Incr= &CellIncr ) 
	{
		TemplCell = m_poTemplate->GetCell( (*It).m_iLevel, (*It).m_Cell ) ;

		(*It).m_Left  += ai_LeftIncr ;
		(*It).m_Width += *Incr ;

		if ( m_poParamCnt->m_MergeMode == ENG_CT_HEADER_MERGE  ) 
		{
			switch ( TemplCell->m_AlignParamCellX ) {
			case ENG_CT_CENTER_X_DISPLAY:
				(*It).m_FieldXOffset += *Incr/2 ;
				break ;
			case ENG_CT_RIGHT_DISPLAY:
				(*It).m_FieldXOffset += *Incr ;
				break ;
			case ENG_CT_LEFT_DISPLAY:
			default:
				(*It).m_FieldXOffset = 0 ;
				break ;
			} ;

		} else {
			(*It).m_FieldXOffset = 0 ;
		} ;

		DistributeWidthIncr( & (*It).m_poSon->m_oList, *Incr, ai_LeftIncr ) ;

		ai_LeftIncr += *Incr ;
	} ;
}

//=======================================================================ClENG_CntSample


ClENG_CntSample::ClENG_CntSample()
{
	m_poInsp = GET_INSP() ;
}

void ClENG_CntSample::End()
{
	m_oCnt.clear() ;
}


m4bool_t ClENG_CntSample::Init( 
		ClENG_DesignTree *ai_poDT, m4char_t *ai_pcCT, ClENG_KeyTot *ai_poKeyTot,
		const m4char_t * ai_pcLevelInfo, 
		m4bool_t ai_bSwIncr, m4int16_t ai_iTogLevel, m4bool_t ai_bSwAllCnt,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StENG_DT_CTData * CT ;
	ClENG_KeyTot KeyTestAux ;
	StENG_DT_SectionData *Sect ;

	ClENG_CntSample::End() ;

	CT = ai_poDT->FindCT(ENG_EXIST_REQUIRED,ai_pcCT) ;
	if ( ! CT ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if (!ai_poKeyTot) {
		SwOk = M4_BOOL ( SwOk && 
			KeyTestAux.InitTest( CT->m_iLevelCount, CT->m_oTreeList, CT->m_oTotList,
			ai_pcLevelInfo, ai_bSwIncr, ClMIT_Trace::PROPAGATE_ERROR) ) ;
		if (!SwOk) goto exit ;
		ai_poKeyTot = &KeyTestAux ;
	} ;

	Sect = ai_poDT->FindSection( ENG_EXIST_REQUIRED,CT->m_iFirstCrossRow) ;
	if (!Sect) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	SwOk = M4_BOOL( SwOk && CT_AddCnt( Sect, CT, ai_poKeyTot, ai_iTogLevel, ai_bSwAllCnt ) ) ;
	if (!SwOk) goto exit ;

exit:

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;		
}


m4bool_t ClENG_CntSample::CT_AddCnt( 
	StENG_DT_SectionData *ai_poSection, 
	StENG_DT_CTData *ai_poCT, ClENG_KeyTot *ai_poKeyTot, m4int16_t ai_iTogLevel, m4bool_t ai_bSwAllCnt )
{
	StENG_DT_SonSectionData *SonSect ;
	StENG_DT_CntData *Cnt ;
	m4int16_t B, C, BTop ;
	m4bool_t SwOk=M4_TRUE ;
	ENG_TYPE_CONTAINER PreOrder[4] = { 
		ENG_CNT_BEFORE, ENG_CNT_HEADER, ENG_CNT_PRERECORD, ENG_CNT_RECORD  }  ;
	ENG_TYPE_CONTAINER PostOrder[3] = {
		ENG_CNT_POSTRECORD, ENG_CNT_FOOTER, ENG_CNT_AFTER
	}  ;
	ENG_TYPE_CONTAINER *AuxCntType ;
	ClENG_CntDisplay CntDisplay ;

	for ( C=0; C<=2; ++C ) {
		
		if (C==1) {
			for ( B=0; B < ai_poSection->m_iSonSectionCount; ++ B ) {
				SonSect = &ai_poSection->m_oSonSection[B] ;

				SwOk = M4_BOOL( SwOk &&
					CT_AddCnt( &SonSect->m_oSection, ai_poCT, ai_poKeyTot, ai_iTogLevel, ai_bSwAllCnt ) 
					) ;
				if (!SwOk) goto exit ;
			} ;
		} else {
			if (C==0) {
				AuxCntType=PreOrder ;
				BTop=4 ;
			} else {
				AuxCntType=PostOrder ;
				BTop=3 ;
			} ;

			for (B=0; B<BTop; ++B) {

				Cnt = ai_poSection->FindCnt( ENG_TRY_FIND,AuxCntType[B] )  ;
				if ( ! Cnt ) continue ;

				if (!ai_bSwAllCnt) {
					if ( ClMIT_Str::StrICmpTrim( 
						ai_poCT->m_oCTId,  Cnt->m_oCTId ) !=0 ) continue ;
				} ;

				SwOk = M4_BOOL( SwOk && 
					CntDisplay.Init( 
						ai_poSection->m_iIdSection, AuxCntType[B], Cnt, 
						&ai_poCT->m_oTemplate, ai_poKeyTot, ai_iTogLevel ) 
				) ;
				if (!SwOk) goto exit ;

				m_oCnt.push_back(CntDisplay) ;
			} ;
		} ;
	} ;

exit:
	return SwOk ;		
}

void ClENG_CntSample::Serialize( ClENG_VB_Serialize *ai_poSerial ) 
{
	ENG_CntList_t::iterator It ;

	ai_poSerial->Init() ;

	for ( It=m_oCnt.begin(); It != m_oCnt.end(); ++It ) {
		(*It).Serialize(ai_poSerial) ;
	} ;

	ai_poSerial->SerializeBool(M4_TRUE) ;	//CntEOF
}

void ClENG_CntSample::Trace(  ) 
{
	ENG_CntList_t::iterator It ;

	for ( It=m_oCnt.begin(); It != m_oCnt.end(); ++It ) {
		(*It).Trace() ;
	} ;
}

m4int16_t ClENG_CntSample::GetLayoutPageCount() 
{
	ENG_CntList_t::iterator It ;
	m4int16_t R=0 ;

	for ( It=m_oCnt.begin(); It != m_oCnt.end(); ++It ) {
		R = M4_MAX( R, (*It).GetPageCount() ) ;
	} ;
	return R ;
}

