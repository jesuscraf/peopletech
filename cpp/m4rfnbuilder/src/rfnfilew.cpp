//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             drvAPI.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:27/01/98
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

#include <windows.h>

#include "rfn.h"
#include "rfndf.inl"
#include "clargs.h"
#include "sysinfo.h"
#include "rfnfilew.h"
#include "pscwidthsextractor.h"
#include "fontm.h"
#include "rfnstrt.h"


#define GROUP_ID RFN_TGI_OTHERS
#define RFN ClFONT_M_RFNStruct		//Para abreviar!


//==============================================================================ClRFN_RFN_W

ClRFN_RFN_W::ClRFN_RFN_W() 
{
	m_poInsp=GET_INSP();
}

void ClRFN_RFN_W::End() 
{	
	m4int16_t B ;

	m_oFile.Close() ;

	m_oPCL_Driver.End() ; m_oESCP_Driver.End() ; m_oRVW_Driver.End() ; m_oPSC_Driver.End() ; 

	for (B=0; B < RFN::SELECTION_COUNT; ++B) {
		m_oSelectionItem[B].m_PCL_DvcCharSet.End() ;
		m_oSelectionItem[B].m_RVW_DvcFontId.End() ;
		m_oSelectionItem[B].m_RVW_DvcTrans.End() ;
	} ;

	m_oSoftFonts.clear() ;
}


void ClRFN_RFN_W::Init( )
{
	ClRFN_RFN_W::End() ;
}


void ClRFN_RFN_W::StSelectionItem::Reset() 
{
	m4int16_t B ;

	for (B=0; B < RFN::DVC_COUNT ;++B) m_bLoaded[B]=M4_FALSE ;
	m_PCL_DvcCharSet.StrCpy("") ;
	m_RVW_DvcFontId.StrCpy("") ;
	m_RVW_DvcTrans.StrCpy("") ;
	m_PSC_DvcFontId.StrCpy( "" ) ;
	m_PCL_DvcSoftFontP.StrCpy("") ;
	m_PCL_DvcSoftFontF.StrCpy("") ;
	m_PCL_DvcTrans.StrCpy("") ;
	m_PSC_DvcSoftFontP.StrCpy("") ;
	m_PSC_DvcSoftFontF.StrCpy("") ;
	m_PSC_DvcTrans.StrCpy("") ;
}

m4bool_t ClRFN_RFN_W::StSelectionItem::NothingLoaded() 
{
	m4bool_t R=M4_TRUE ;
	m4int16_t B ;

	for (B=0; B < RFN::DVC_COUNT; ++B ) {
		if ( m_bLoaded[B] ) {
			R=M4_FALSE ;
			break ;
		} ;
	} ;
	return R ;
}

void ClRFN_RFN_W::WInt(m4int16_t ai_Int) 
{
	m_oFile.W(&ai_Int,sizeof(m4int16_t)) ;
}

void ClRFN_RFN_W::WLong(m4int32_t ai_Long) 
{
	m_oFile.W(&ai_Long,sizeof(m4int32_t)) ;
}

void ClRFN_RFN_W::WChar(m4uint8_t ai_Char) 
{
	m_oFile.W(&ai_Char,sizeof(m4uint8_t)) ;
}

void ClRFN_RFN_W::WStr(m4char_t *ai_pcStr, m4int32_t ai_Size) 
{
	size_t L ;

	WLong(ai_Size) ;
	L = strlen(ai_pcStr) ;
	if ( L>=ai_Size ) {

		m_oFile.W( ai_pcStr, sizeof(m4char_t)*ai_Size ) ;

	} else {

		ClMIT_Str Str ;

		Str.StrCpy(ai_pcStr) ;
		Str.StrInsertChar(ClMIT_Str::STR_CAT,ai_Size-L,' ') ;
		m_oFile.W( Str.InChar(0), sizeof(m4char_t)*ai_Size ) ;
	} ;
}

m4bool_t ClRFN_RFN_W::SelectItemEq( 
	ClRFN_RFN_W::StSelectionItem *ai_OpeA, ClRFN_RFN_W::StSelectionItem *ai_OpeB) 
{
	m4bool_t SwEq=M4_FALSE ;
	m4int16_t B ;

	//Datos PCL

	if ( m_oHeaderData.m_bSwOn[RFN::PCL] )  {
		
		if ( ai_OpeA->m_bLoaded[RFN::PCL] != ai_OpeB->m_bLoaded[RFN::PCL] ) goto exit ;
		if ( !ai_OpeA->m_bLoaded[RFN::PCL] && !ai_OpeB->m_bLoaded[RFN::PCL] ) {
			SwEq=M4_TRUE;
			goto exit ;
		} ;
		if ( ai_OpeA->m_PCL_Height != ai_OpeB->m_PCL_Height ) goto exit ;
		if ( ai_OpeA->m_PCL_MaxWidth != ai_OpeB->m_PCL_MaxWidth ) goto exit ;
		if ( ai_OpeA->m_PCL_SelectionHeight != ai_OpeB->m_PCL_SelectionHeight ) goto exit ;
		if ( ai_OpeA->m_PCL_DvcFontId != ai_OpeB->m_PCL_DvcFontId ) goto exit ;
		if ( ClMIT_Str::StrICmpTrim( ai_OpeA->m_PCL_DvcCharSet, ai_OpeB->m_PCL_DvcCharSet )!=0 ) goto exit ;
		if ( ClMIT_Str::StrICmpTrim( ai_OpeA->m_PCL_DvcSoftFontF, ai_OpeB->m_PCL_DvcSoftFontF )!=0 ) goto exit ;

		if ( ! m_oHeaderData.m_bSwFixed  ) {
			for (B=0;B<RFN::METRIC_ARRAY_SIZE;++B) {
				if ( ai_OpeA->m_Metric[RFN::PCL][B] != ai_OpeB->m_Metric[RFN::PCL][B] ) goto exit ;
			} ;
		} ;

	} ;

	//Datos PSC

	if ( m_oHeaderData.m_bSwOn[RFN::PSC] )  {
		
		if ( ai_OpeA->m_bLoaded[RFN::PSC] != ai_OpeB->m_bLoaded[RFN::PSC] ) goto exit ;
		if ( !ai_OpeA->m_bLoaded[RFN::PSC] && !ai_OpeB->m_bLoaded[RFN::PSC] ) {
			SwEq=M4_TRUE;
			goto exit ;
		} ;
		if ( ai_OpeA->m_PSC_Height != ai_OpeB->m_PSC_Height ) goto exit ;
		if ( ai_OpeA->m_PSC_MaxWidth != ai_OpeB->m_PSC_MaxWidth ) goto exit ;
		if ( ai_OpeA->m_PSC_SelectionHeight != ai_OpeB->m_PSC_SelectionHeight ) goto exit ;
		if ( ClMIT_Str::StrICmpTrim( ai_OpeA->m_PSC_DvcFontId, ai_OpeB->m_PSC_DvcFontId )!=0 ) goto exit ;
		if ( ClMIT_Str::StrICmpTrim( ai_OpeA->m_PSC_DvcSoftFontF, ai_OpeB->m_PSC_DvcSoftFontF )!=0 ) goto exit ;

		if ( ! m_oHeaderData.m_bSwFixed  ) {
			for (B=0;B<RFN::METRIC_ARRAY_SIZE;++B) {
				if ( ai_OpeA->m_Metric[RFN::PSC][B] != ai_OpeB->m_Metric[RFN::PSC][B] ) goto exit ;
			} ;
		} ;

	} ;
	
	
	//Datos ESCP

	if ( m_oHeaderData.m_bSwOn[RFN::ESCP] )  {
		
		if ( ai_OpeA->m_bLoaded[RFN::ESCP] != ai_OpeB->m_bLoaded[RFN::ESCP] ) goto exit ;
		if ( !ai_OpeA->m_bLoaded[RFN::ESCP] && !ai_OpeB->m_bLoaded[RFN::ESCP] ) {
			SwEq=M4_TRUE;
			goto exit ;
		} ;
		if ( ai_OpeA->m_ESCP_Height != ai_OpeB->m_ESCP_Height ) goto exit ;
		if ( ai_OpeA->m_ESCP_MaxWidth != ai_OpeB->m_ESCP_MaxWidth ) goto exit ;
		if ( ai_OpeA->m_ESCP_SelectionHeight != ai_OpeB->m_ESCP_SelectionHeight ) goto exit ;
		if ( ai_OpeA->m_ESCP_DvcFontId != ai_OpeB->m_ESCP_DvcFontId ) goto exit ;
		if ( ai_OpeA->m_ESCP_Sw2Y != ai_OpeB->m_ESCP_Sw2Y ) goto exit ;
		if ( ai_OpeA->m_ESCP_Sw2x != ai_OpeB->m_ESCP_Sw2x ) goto exit ;
		if ( ai_OpeA->m_ESCP_SwCompr != ai_OpeB->m_ESCP_SwCompr ) goto exit ;

		if ( ! m_oHeaderData.m_bSwFixed  ) {
			for (B=0;B<RFN::METRIC_ARRAY_SIZE;++B) {
				if ( ai_OpeA->m_Metric[RFN::ESCP][B] != ai_OpeB->m_Metric[RFN::ESCP][B] ) goto exit ;
			} ;
		} ;

	} ;

	//Datos RVW

	if ( m_oHeaderData.m_bSwOn[RFN::RVW] )  {
		
		if ( ai_OpeA->m_bLoaded[RFN::RVW] != ai_OpeB->m_bLoaded[RFN::RVW] ) goto exit ;
		if ( !ai_OpeA->m_bLoaded[RFN::RVW] && !ai_OpeB->m_bLoaded[RFN::RVW] ) {
			SwEq=M4_TRUE;
			goto exit ;
		} ;
		if ( ai_OpeA->m_RVW_Height != ai_OpeB->m_RVW_Height ) goto exit ;
		if ( ai_OpeA->m_RVW_MaxWidth != ai_OpeB->m_RVW_MaxWidth ) goto exit ;
		if ( ai_OpeA->m_RVW_SelectionHeight != ai_OpeB->m_RVW_SelectionHeight ) goto exit ;
		if ( ClMIT_Str::StrICmpTrim( ai_OpeA->m_RVW_DvcFontId, ai_OpeB->m_RVW_DvcFontId )!=0 ) goto exit ;

		if ( ! m_oHeaderData.m_bSwFixed  ) {
			for (B=0;B<RFN::METRIC_ARRAY_SIZE;++B) {
				if ( ai_OpeA->m_Metric[RFN::RVW][B] != ai_OpeB->m_Metric[RFN::RVW][B] ) goto exit ;
			} ;
		} ;

	} ;

	SwEq=M4_TRUE ;

exit:
	return SwEq ;
}



m4bool_t ClRFN_RFN_W::PrepareFile( 
	m4char_t * ai_pcPCL_Driver, m4char_t * ai_pcESCP_Driver, m4char_t *ai_pcRVW_Driver, 
	m4char_t * ai_pcPSC_Driver,
	m4char_t *ai_pcFileName, m4char_t *ai_pcFontName, m4char_t *ai_pcFilePARName,
	m4int16_t ai_HeightVarCount, m4bool_t ai_bSwFixed, 
	m4bool_t ai_bSwPCL, m4bool_t ai_bSwPSC, m4bool_t ai_bSwRVW, m4bool_t ai_bSwESCP ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str StrAux, StrFile ;
	m4int16_t B ;


	// UNICODE REPORTS.
	m4bool_t bAddBOM = M4_FALSE;
	if ( ! m_oFile.Open( ai_pcFileName, ClMIT_File::OVERWRITE, ClMIT_File::WRITE, ClMIT_File::TRY_OPEN , bAddBOM ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_CountIndexWritten=0 ;
	m_CountMetricWritten=0 ;

	m_oPCL_Driver.StrCpy(ai_pcPCL_Driver) ; m_oESCP_Driver.StrCpy(ai_pcESCP_Driver) ; 
	m_oRVW_Driver.StrCpy(ai_pcRVW_Driver) ; m_oPSC_Driver.StrCpy(ai_pcPSC_Driver) ;

	m_oHeaderData.m_HeightVarCount	= ai_HeightVarCount ;
	m_oHeaderData.m_bSwFixed		= ai_bSwFixed;
	m_oHeaderData.m_bSwOn[RFN::PCL]	= ai_bSwPCL;
	m_oHeaderData.m_bSwOn[RFN::RVW]	= ai_bSwRVW;
	m_oHeaderData.m_bSwOn[RFN::PSC]	= ai_bSwPSC;
	m_oHeaderData.m_bSwOn[RFN::ESCP]	= ai_bSwESCP;


	//Escribo cabecera

	ClMIT_SysInfo::ExtractFromPath( ai_pcFilePARName, &StrAux, &StrFile );

	WChar( m_poInsp->m_poSysInfo->GetEndian() ) ;
	WStr( StrFile, RFN::HEADER_FILE_PAR) ;
	WStr( (m4char_t *)RFN::RFN_FILE_ID, RFN::HEADER_FILE_ID ) ;
	WInt( RFN::CURRENT_VERSION ) ;
	WStr( ai_pcFontName, RFN::HEADER_TYPE_FACE ) ;
	WInt( ai_HeightVarCount ) ;
	WChar( ai_bSwFixed ? 1 : 0 ) ;
	WChar( RFN::PackDvcFlags( ai_bSwPCL, ai_bSwPSC, ai_bSwRVW, ai_bSwESCP ) ) ;

	//Y ahora n index block basura

	StrAux.Accommodate( RFN::INDEX_SIZE ) ;

	for (B=0;B<ai_HeightVarCount;++B) {
		m_oFile.W(StrAux.InChar(0),sizeof(m4char_t)*RFN::INDEX_SIZE) ;
	} ;

	//Actualizo Offsets

	m_IndexZoneOff  = RFN::HEADER_SIZE  ; 
	m_MetricZoneOff = RFN::HEADER_SIZE *  ai_HeightVarCount * RFN::INDEX_SIZE ;

exit:

	return SwOk ;
}

void ClRFN_RFN_W::EndFile() 
{
	SaveSoftFontsToFile() ;
	
	m_oFile.Close() ;

	if ( m_CountIndexWritten != m_oHeaderData.m_HeightVarCount ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Height variations %0:s, written %1:s" ) 
			<< m_oHeaderData.m_HeightVarCount << m_CountIndexWritten << SEND_MSG ;
	}
}


void  ClRFN_RFN_W::BeginHeightBlock(m4int32_t ai_DesignHeight, m4int32_t ai_DesignWidth) 
{
	m4int16_t B ;

	m_oHeightVarData.m_DesignHeight = ai_DesignHeight ;
	m_oHeightVarData.m_DesignWidth  = ai_DesignWidth ;

	for (B=0;B<RFN::SELECTION_COUNT;++B) m_oSelectionItem[B].Reset() ;
}


m4bool_t ClRFN_RFN_W::EndHeightBlock() 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t Redirect[RFN::SELECTION_COUNT] ;	//-1 sin redireccion, n indice al que se redirige
	m4int32_t OffAux[RFN::SELECTION_COUNT] ;
	m4int16_t B, B2 ;
	DvcUnit_t MaxWidthAux ;
	m4int16_t Trans[ENG_TRANS_ARRAY_SIZE] ;

	if ( m_CountIndexWritten >= m_oHeaderData.m_HeightVarCount ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Full Height Variations written (%0:s)" ) 
			<< m_oHeaderData.m_HeightVarCount << SEND_MSG ;
		SwOk=M4_FALSE;
		goto exit ;
	} ;

	//Redirecciones

	for (B=0;B<RFN::SELECTION_COUNT;++B) {
		Redirect[B]=-1;
		//Mira si hay anteriores iguales para redirigir
		for (B2=0;B2<B;++B2) {
			if ( SelectItemEq(&m_oSelectionItem[B],&m_oSelectionItem[B2]) ) {
				Redirect[B]=B2 ;
				break ;
			} ;
		} ;

	} ;
	

	//Y ahora, a grabar los Metric Blocks pertinentes. De paso calcula MaxWidthAux

	for (B=0, MaxWidthAux=0;B<RFN::SELECTION_COUNT;++B) {

		if ( m_oSelectionItem[B].NothingLoaded() )  {
			OffAux[B]=-1;
			continue ;
		} ;

		if ( Redirect[B] != -1  )  {
			OffAux[B] = OffAux[Redirect[B]] ; 
			continue ;
		} ;

		// Calculo MaxWidthAux

		if ( m_oHeaderData.m_bSwOn[RFN::PCL] ) {
			MaxWidthAux = M4_MAX( MaxWidthAux, m_oSelectionItem[B].m_PCL_MaxWidth ) ;
		} ;
		if ( m_oHeaderData.m_bSwOn[RFN::PSC] ) {
			MaxWidthAux = M4_MAX( MaxWidthAux, m_oSelectionItem[B].m_PSC_MaxWidth ) ;
		} ;
		if ( m_oHeaderData.m_bSwOn[RFN::RVW] ) {
			MaxWidthAux = M4_MAX( MaxWidthAux, m_oSelectionItem[B].m_RVW_MaxWidth ) ;
		} ;
		if ( m_oHeaderData.m_bSwOn[RFN::ESCP] ) {
			MaxWidthAux = M4_MAX( MaxWidthAux, m_oSelectionItem[B].m_ESCP_MaxWidth ) ;
		} ;

		// A escribir un metric Block. Grabar siempre para que el bloque sea del tamaño correcto

		OffAux[B]=m_MetricZoneOff ;

		m_oFile.SetPos(m_MetricZoneOff) ;

		WInt(m_oSelectionItem[B].m_PCL_Height) ;
		WInt(m_oSelectionItem[B].m_PCL_MaxWidth) ;
		WInt(m_oSelectionItem[B].m_PCL_SelectionHeight) ;
		WInt(m_oSelectionItem[B].m_PCL_DvcFontId) ;
		WStr(m_oSelectionItem[B].m_PCL_DvcCharSet,RFN::PCL_SYMBOL_SET) ;

		//Tratamiento SoftFontOff
		if ( m_oSelectionItem[B].m_PCL_DvcSoftFontF.StrLenTrim()==0 ) {
			WLong(-1) ;	
		} else {
			if ( ! AddSoftFont( m_oSelectionItem[B].m_PCL_DvcSoftFontP, m_oFile.GetPos(), 
				m_oSelectionItem[B].m_PCL_DvcTrans , M4_FALSE )  ) 
			{
				SwOk=M4_FALSE ;
				goto exit ;
			};
			WLong(-1) ;	
		} ;

		WInt(m_oSelectionItem[B].m_PSC_Height) ;
		WInt(m_oSelectionItem[B].m_PSC_MaxWidth) ;
		WInt(m_oSelectionItem[B].m_PSC_SelectionHeight) ;
		WStr(m_oSelectionItem[B].m_PSC_DvcFontId,RFN::PSC_FONT_ID) ;

		//Tratamiento SoftFontOff
		if ( m_oSelectionItem[B].m_PSC_DvcSoftFontF.StrLenTrim()==0 ) {
			WLong(-1) ;	
		} else {
			if ( ! AddSoftFont( m_oSelectionItem[B].m_PSC_DvcSoftFontP, m_oFile.GetPos(), 
				m_oSelectionItem[B].m_PSC_DvcTrans, M4_TRUE ) )
			{
				SwOk=M4_FALSE ;
				goto exit ;
			};
			WLong(-1) ;	
		} ;
		
		WInt(m_oSelectionItem[B].m_ESCP_Height) ;
		WInt(m_oSelectionItem[B].m_ESCP_MaxWidth) ;
		WInt(m_oSelectionItem[B].m_ESCP_SelectionHeight) ;
		WChar( 
			RFN::PackESCP_Flags( m_oSelectionItem[B].m_ESCP_Sw2Y, 
				m_oSelectionItem[B].m_ESCP_Sw2x, m_oSelectionItem[B].m_ESCP_SwCompr)
		) ;
		WInt(m_oSelectionItem[B].m_ESCP_DvcFontId) ;

		WStr(m_oSelectionItem[B].m_RVW_DvcFontId, RFN::RVW_FACE_NAME) ;
		WInt(m_oSelectionItem[B].m_RVW_Height) ;
		WInt(m_oSelectionItem[B].m_RVW_MaxWidth) ;
		WInt(m_oSelectionItem[B].m_RVW_SelectionHeight) ;

		SwOk = SwOk && ClRFN_RFN_W::TranslationStrToArray( m_oSelectionItem[B].m_RVW_DvcTrans, Trans ) ;
		if ( !SwOk ) goto exit ;
		m_oFile.W( Trans,sizeof(m4int16_t)*ENG_TRANS_ARRAY_SIZE) ;


		if ( !m_oHeaderData.m_bSwFixed ) {
			m_oFile.W( &m_oSelectionItem[B].m_Metric[RFN::PCL],sizeof(m4int32_t)*RFN::METRIC_ARRAY_SIZE) ;	
			m_oFile.W( &m_oSelectionItem[B].m_Metric[RFN::PSC],sizeof(m4int32_t)*RFN::METRIC_ARRAY_SIZE) ;	
			m_oFile.W( &m_oSelectionItem[B].m_Metric[RFN::ESCP],sizeof(m4int32_t)*RFN::METRIC_ARRAY_SIZE) ;	
			m_oFile.W( &m_oSelectionItem[B].m_Metric[RFN::RVW],sizeof(m4int32_t)*RFN::METRIC_ARRAY_SIZE) ;	
		} ;

		m_MetricZoneOff = m_oFile.GetPos() ;

	} ;

	//Y a grabar el Index Block

	m_oFile.SetPos(m_IndexZoneOff+m_CountIndexWritten*RFN::INDEX_SIZE) ;
	WLong( ClMetric::PointToDvc(m_oHeightVarData.m_DesignHeight)/100 ) ;

	//A calcular MaxWidth si este es 0!

	if ( m_oHeightVarData.m_DesignWidth == 0 ) {

		m_oHeightVarData.m_DesignWidth = MaxWidthAux  ;	
		WLong( m_oHeightVarData.m_DesignWidth+2 ) ;//+2 margen redondeos
	
	} else {

		//WLong( ClMetric::CPIToDvcWidthChar(m_oHeightVarData.m_DesignWidth/100)+2 ) ;//+2 margen redondeos
		//Formula CPIToDvc a pelo para simular como se generaban los RFN viejos
		{
			CPI_t X1 =  m_oHeightVarData.m_DesignWidth ;
			//DvcUnit_t X2 ;

			X1 = X1 / 100.0 ;
			WLong( ClMetric::InchToDvc(1/X1)+2 ) ;//+2 margen redondeos
		} ;
	} ;

	for (B=0;B<RFN::SELECTION_COUNT;++B) WLong( OffAux[B] ) ;

	//Incrementa m_CountIndexWritten

	++m_CountIndexWritten ;

exit:
	return SwOk ;
}


m4bool_t ClRFN_RFN_W::MapFont( 
		m4char_t *ai_pcDevice, m4char_t *ai_pcFaceName, Point_t ai_MapHeight,
		m4bool_t ai_bBold, m4bool_t ai_bItalic, m4bool_t ai_bUnderline,
		m4bool_t ai_bSwFixed, 
		DvcUnit_t *ao_iHeight, DvcUnit_t *ao_iMaxWidth, DvcUnit_t *ao_piWidth,
		m4char_t *ai_pcDriver) 
{
	m4bool_t SwOk=M4_TRUE ;
	HDC WinDC=NULL;
	HFONT WinF=NULL;
	//CFont Fuente;
	TEXTMETRIC tm;

	m4char_t Str2[2];
	m4uint32_t Aux,Aux2;
	m4int16_t B ;
	SIZE Sz;
	
	WinDC=CreateDC(ai_pcDriver,ai_pcDevice,NULL,NULL);
	if (!WinDC)
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, "Error al crear el dispositivo de contexto de Windows") 
			<< GENERAL_MSG << SEND_MSG   ;
		SwOk=M4_FALSE ;
		goto exit ;
	}

	if ( SetMapMode( WinDC, MM_TWIPS )==0 )
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, "Error al fijar el modo de mapeo para el dispositivo") 
			<< GENERAL_MSG << SEND_MSG   ;
		SwOk=M4_FALSE ;
		goto exit ;
	}
	
	Aux=FF_DONTCARE;
	Aux2=ANSI_CHARSET;

	WinF=CreateFont(
			- ClMetric::PointToTwip(ai_MapHeight),0,0,0,
			ai_bBold ? FW_BOLD : FW_NORMAL,
			ai_bItalic,ai_bUnderline,M4_FALSE,Aux2,
			OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
			(ai_bSwFixed ? FIXED_PITCH : VARIABLE_PITCH) | Aux,
			ai_pcFaceName
	) ;

	if (!WinF)
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, "Error al crear la fuente") 
			<< GENERAL_MSG << SEND_MSG   ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( !SelectObject(WinDC,WinF) )
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, "Error al seleccionar la fuente en el dispositivo de contexto") 
			<< GENERAL_MSG << SEND_MSG   ;
		SwOk=M4_FALSE ;
		goto exit ;
	};

	GetTextMetrics(WinDC,&tm);

	strcpy( Str2, "a" ) ;
	GetTextExtentPoint32( WinDC, Str2, 1, &Sz );

	*ao_iHeight = ClMetric::TwipToDvc(ClMetric::RoundValue(ClMetric::PointToTwip(ai_MapHeight)/10.0)*10.0) ; // (Sz.cy/10.0)
	
	*ao_iMaxWidth = 0 ;
	
	for (B=0; B<RFN::METRIC_ARRAY_SIZE; ++B)
	{
		Str2[0]=B ;
		GetTextExtentPoint32( WinDC, Str2, 1, &Sz );
		ao_piWidth[B]=ClMetric::TwipToDvc(ClMetric::RoundValue(Sz.cx/10.0)*10.0) ; //( Sz.cx/10.0 ) ;	
		*ao_iMaxWidth = M4_MAX( *ao_iMaxWidth, ao_piWidth[B] ) ;
	} ;

exit:

	if (WinF) DeleteObject(WinF) ;
	if (WinDC) DeleteDC(WinDC) ;
	return SwOk ;
}



m4bool_t ClRFN_RFN_W::SelectionDataPCL( 

	m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline,
	m4char_t *ai_pcMapName, Point_t ai_MapHeight,  CPI_t ai_Pitch,
	Point_t ai_DvcHeight, Point_t ai_SelectionHeight,

	m4int16_t ai_PCL_DvcFontId,	m4char_t * ai_PCL_DvcCharSet,
	m4char_t * ai_PCL_SoftFont, m4char_t * ai_PCL_DvcTrans ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StSelectionItem *S ;
	DvcUnit_t HeightMapped, MaxWidthMapped, WidthArrayMapped[RFN::METRIC_ARRAY_SIZE] ;
	m4int16_t B ;
	ClMIT_Str StrPath ;

	if (!m_oHeaderData.m_bSwOn[RFN::PCL]) goto exit ;

	S = & m_oSelectionItem[ RFN::PackSIndex(ai_bSwBold,ai_bSwItalic,ai_bSwUnderline) ] ;

	if ( S->m_bLoaded[RFN::PCL] ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Selection map duplicated" ) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	S->m_bLoaded[RFN::PCL]=M4_TRUE ;

	if ( !  MapFont( 
		m_oPCL_Driver, ai_pcMapName, ai_MapHeight,
		ai_bSwBold, ai_bSwItalic, ai_bSwUnderline,
		m_oHeaderData.m_bSwFixed, 
		&HeightMapped, &MaxWidthMapped, WidthArrayMapped,
		"WINSPOOL") ) 
	{
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	
	S->m_PCL_Height		=  ai_DvcHeight== 0 ? ClMetric::DvcToPoint(HeightMapped)*100 : 
							ai_DvcHeight*100 ; 
	S->m_PCL_SelectionHeight	= ai_SelectionHeight == 0 ?  S->m_PCL_Height : 
							ai_SelectionHeight  *100 ;
	S->m_PCL_MaxWidth	=  ai_Pitch == 0 ? MaxWidthMapped : 
								ClMetric::CPIToDvcWidthChar(ai_Pitch) ;
	S->m_PCL_DvcFontId = ai_PCL_DvcFontId ;
	S->m_PCL_DvcCharSet.StrCpy(ai_PCL_DvcCharSet) ;

	//Importante, guardar nombre sin path para que funciones de comparacion funcionen OK

	S->m_PCL_DvcSoftFontP.StrCpy(ai_PCL_SoftFont) ; ;
	ClMIT_SysInfo::ExtractFromPath(ai_PCL_SoftFont,&StrPath, &S->m_PCL_DvcSoftFontF) ;
	S->m_PCL_DvcTrans.StrCpy( ai_PCL_DvcTrans ) ;


	for (B=0;B<RFN::METRIC_ARRAY_SIZE;++B) S->m_Metric[RFN::PCL][B] = WidthArrayMapped[B] ;
exit:
	return SwOk;
}

m4bool_t ClRFN_RFN_W::SelectionDataPSC( 

		m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline,
		m4char_t *ai_pcMapName, Point_t ai_MapHeight, CPI_t ai_Pitch,
		Point_t ai_DvcHeight, Point_t ai_SelectionHeight,

		m4char_t * ai_PSC_DvcFontId,	
		m4char_t * ai_PSC_SoftFont, m4char_t * ai_PSC_DvcTrans ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StSelectionItem *S ;
	DvcUnit_t HeightMapped, MaxWidthMapped, WidthArrayMapped[RFN::METRIC_ARRAY_SIZE] ;
	m4int16_t B ;
	ClMIT_Str StrPath ;

	if (!m_oHeaderData.m_bSwOn[RFN::PSC]) goto exit ;

	S = & m_oSelectionItem[ RFN::PackSIndex(ai_bSwBold,ai_bSwItalic,ai_bSwUnderline) ] ;

	if ( S->m_bLoaded[RFN::PSC] ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Selection map duplicated" ) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	S->m_bLoaded[RFN::PSC]=M4_TRUE ;

	if ( !  MapFont( 
		m_oPSC_Driver, ai_pcMapName, ai_MapHeight,
		ai_bSwBold, ai_bSwItalic, ai_bSwUnderline,
		m_oHeaderData.m_bSwFixed, 
		&HeightMapped, &MaxWidthMapped, WidthArrayMapped,
		"WINSPOOL") ) 
	{
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	S->m_PSC_Height		=  ai_DvcHeight== 0 ? ClMetric::DvcToPoint(HeightMapped)*100 : 
							ai_DvcHeight*100 ; 
	S->m_PSC_SelectionHeight	= ai_SelectionHeight == 0 ?  S->m_PSC_Height : 
							ai_SelectionHeight  *100 ;
	S->m_PSC_MaxWidth	= ai_Pitch == 0 ? MaxWidthMapped : ClMetric::CPIToDvcWidthChar(ai_Pitch) ;
	S->m_PSC_DvcFontId.StrCpy(ai_PSC_DvcFontId) ;

	//Importante, guardar nombre sin path para que funciones de comparacion funcionen OK

	S->m_PSC_DvcSoftFontP.StrCpy(ai_PSC_SoftFont) ; ;
	ClMIT_SysInfo::ExtractFromPath(ai_PSC_SoftFont,&StrPath, &S->m_PSC_DvcSoftFontF) ;
	S->m_PSC_DvcTrans.StrCpy( ai_PSC_DvcTrans ) ;

	for (B=0;B<RFN::METRIC_ARRAY_SIZE;++B) S->m_Metric[RFN::PSC][B] = WidthArrayMapped[B] ;
exit:
	return SwOk;
}



m4bool_t ClRFN_RFN_W::SelectionDataRVW( 

		m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline,
		m4char_t *ai_pcMapName, Point_t ai_MapHeight, CPI_t ai_Pitch, 
		Point_t ai_DvcHeight, Point_t ai_SelectionHeight,
		m4char_t * ai_RVW_DvcTrans ) 
{
	m4bool_t SwOk=M4_TRUE ;
	StSelectionItem *S ;
	DvcUnit_t HeightMapped, MaxWidthMapped, WidthArrayMapped[RFN::METRIC_ARRAY_SIZE] ;
	m4int16_t B ;

	if (!m_oHeaderData.m_bSwOn[RFN::RVW]) goto exit ;

	S = & m_oSelectionItem[ RFN::PackSIndex(ai_bSwBold,ai_bSwItalic,ai_bSwUnderline) ] ;

	if ( S->m_bLoaded[RFN::RVW] ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Selection map duplicated" ) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	S->m_bLoaded[RFN::RVW]=M4_TRUE ;

	if ( !  MapFont( 
		m_oRVW_Driver, ai_pcMapName, ai_MapHeight,
		ai_bSwBold, ai_bSwItalic, ai_bSwUnderline,
		m_oHeaderData.m_bSwFixed, 
		&HeightMapped, &MaxWidthMapped, WidthArrayMapped,
		"WINSPOOL") ) 
	{
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	
	S->m_RVW_Height		=  ai_DvcHeight== 0 ? ClMetric::DvcToPoint(HeightMapped)*100 : 
							ai_DvcHeight*100 ; 
	S->m_RVW_SelectionHeight	= ai_SelectionHeight == 0 ?  S->m_RVW_Height : 
							ai_SelectionHeight  *100 ;
	S->m_RVW_MaxWidth	=  ai_Pitch == 0 ? MaxWidthMapped : 
							ClMetric::CPIToDvcWidthChar(ai_Pitch) ;
	S->m_RVW_DvcFontId.StrCpy(ai_pcMapName) ;
	S->m_RVW_DvcTrans.StrCpy(ai_RVW_DvcTrans) ;

	for (B=0;B<RFN::METRIC_ARRAY_SIZE;++B) S->m_Metric[RFN::RVW][B] = WidthArrayMapped[B] ;
exit:
	return SwOk;

}

m4bool_t ClRFN_RFN_W::SelectionDataESCP( 

		m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline,
		m4char_t *ai_pcMapName, Point_t ai_MapHeight, CPI_t ai_Pitch,
		Point_t ai_DvcHeight, Point_t ai_SelectionHeight,

		m4int16_t ai_ESCP_DvcFontId, m4bool_t ai_ESCP_Sw2Y, m4bool_t ai_ESCP_Sw2x, 
		m4bool_t ai_ESCP_SwCompr )
{
	m4bool_t SwOk=M4_TRUE ;
	StSelectionItem *S ;
	DvcUnit_t HeightMapped, MaxWidthMapped, WidthArrayMapped[RFN::METRIC_ARRAY_SIZE] ;
	m4int16_t B ;

	if (!m_oHeaderData.m_bSwOn[RFN::ESCP]) goto exit ;

	S = & m_oSelectionItem[ RFN::PackSIndex(ai_bSwBold,ai_bSwItalic,ai_bSwUnderline) ] ;

	if ( S->m_bLoaded[RFN::ESCP] ) {
		*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Selection map duplicated" ) 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	S->m_bLoaded[RFN::ESCP]=M4_TRUE ;

	if ( !  MapFont( 
		m_oESCP_Driver, ai_pcMapName, ai_MapHeight,
		ai_bSwBold, ai_bSwItalic, ai_bSwUnderline,
		m_oHeaderData.m_bSwFixed, 
		&HeightMapped, &MaxWidthMapped, WidthArrayMapped,
		"WINSPOOL") ) 
	{
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	
	S->m_ESCP_Height		=  ai_DvcHeight== 0 ? ClMetric::DvcToPoint(HeightMapped)*100 : 
							ai_DvcHeight*100 ; 
	S->m_ESCP_SelectionHeight	= ai_SelectionHeight == 0 ?  S->m_ESCP_Height : 
							ai_SelectionHeight  *100 ;
	S->m_ESCP_MaxWidth		= ai_Pitch == 0 ? MaxWidthMapped : ClMetric::CPIToDvcWidthChar(ai_Pitch) ;
	S->m_ESCP_DvcFontId		= ai_ESCP_DvcFontId ;
	S->m_ESCP_Sw2Y			= ai_ESCP_Sw2Y ;
	S->m_ESCP_Sw2x			= ai_ESCP_Sw2x ;
	S->m_ESCP_SwCompr		= ai_ESCP_SwCompr ;

	for (B=0;B<RFN::METRIC_ARRAY_SIZE;++B) S->m_Metric[RFN::ESCP][B] = WidthArrayMapped[B] ;
exit:
	return SwOk;

}

m4bool_t ClRFN_RFN_W::AddSoftFont( 
	m4char_t *ai_pcFullFontName, m4int32_t ai_FileOff, 
	m4char_t *ai_pcTranslation, m4bool_t ai_bPSC_AFM ) 
{
	ClMIT_Str StrPath, StrFile, StrFileNoExt, StrExt, StrAFM ;
	SoftFontList_t::iterator It ;
	StSoftFontData Data ;
	m4int16_t B ;
	RFN_EncodingWidths_t encW ;
	RFN_GlyphWidths_t glyphW ;
	m4bool_t swOk=M4_TRUE ;

	ClMIT_File::SplitPath( ai_pcFullFontName, &StrPath, &StrFile );
	ClMIT_File::SplitPath( ai_pcFullFontName, &StrPath, &StrFileNoExt, &StrExt );

	It = m_oSoftFonts.find(StrFile) ; 
	if ( It==m_oSoftFonts.end() ) {
		//Nuevo
		Data.m_oFileName.StrCpy(ai_pcFullFontName) ;
		Data.m_oOffsets.clear() ;
		Data.m_oOffsets.push_back(ai_FileOff) ;
		Data.m_oTrans.StrCpy(ai_pcTranslation) ;

		for (B=0;B<ENG_PSC_AFM_WIDTHS_ARRAY_SIZE;++B) {
			Data.m_iPSC_AFM_Widths[B]=0 ;
		} ;

		if (ai_bPSC_AFM) {

			if ( stricmp( StrExt, ".pfa")!=0 ) {
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"SoftFont file %0:s must be a PFA file" ) 
					<< ai_pcFullFontName << SEND_MSG ;
				swOk=M4_FALSE ;
				goto exit ;

			} ;
			StrAFM.StrCpy(StrPath) ;
			StrAFM.StrCat(StrFileNoExt) ;
			StrAFM.StrCat(".afm") ;

			if ( ! WidthsExtractor( ai_pcFullFontName, StrAFM, &encW, &glyphW ) ) {
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Error extracting info from softfont files %0:s and %1:s" ) 
					<< ai_pcFullFontName << StrAFM.InChar(0) <<  SEND_MSG ;
				swOk=M4_FALSE ;
				goto exit ;
			} ;

			if ( encW.size()>ENG_PSC_AFM_WIDTHS_ARRAY_SIZE) {
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Info extracted from softfont files %0:s and %1:s exceed %2:s characters" ) 
					<< ai_pcFullFontName << StrAFM.InChar(0) 
					<< (m4int16_t)ENG_PSC_AFM_WIDTHS_ARRAY_SIZE << SEND_MSG ;
				swOk=M4_FALSE ;
				goto exit ;
			} ;

			for (B=0;B<encW.size();++B) {
				Data.m_iPSC_AFM_Widths[B]=encW.at(B)  ;
			} ;

		} ;
		m_oSoftFonts.insert( SoftFontList_t::value_type(StrFile,Data) ) ;

	} else {
		//Ya existente, meter Offset
		(*It).second.m_oOffsets.push_back(ai_FileOff) ;
	} ;

exit:
	return swOk ;
}


m4bool_t ClRFN_RFN_W::TranslationStrToArray( m4char_t *ai_pcTrans, 
		m4int16_t ao_Array[ENG_TRANS_ARRAY_SIZE]  ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	IntVector_t vectorAux ;
	m4char_t typeInfo ;

	
	for (B=0;B<ENG_TRANS_ARRAY_SIZE;++B) ao_Array[B] = 0 ;
	
	if ( ClMIT_Str::StrLenTrim(ai_pcTrans)==0 ) {
			ao_Array[ENG_TRANS_TYPE_POS] = ENG_SOFT_TRANS_TYPE_EMPTY  ;
	} else {


		if ( !TranslationVector( ai_pcTrans, vectorAux, &typeInfo ) ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		//Crea un vector de enteros en base a la entrada

		if ( vectorAux.size() == 0 ) {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Translation string %0:s in SoftFont doesn't have type info" ) 
				<< ai_pcTrans << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		if ( typeInfo == '-' ) {

			ao_Array[ENG_TRANS_TYPE_POS] = ENG_SOFT_TRANS_TYPE_EMPTY ;

		} else if ( typeInfo == 'I' ) {

			ao_Array[ENG_TRANS_TYPE_POS] = ENG_SOFT_TRANS_TYPE_I_2_5 ;
			if ( vectorAux.size() != ENG_TRANS_I_2_5_COUNT )
			{
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Translation string %0:s in SoftFont,  missing codes (there are %1:s)" ) 
					<< ai_pcTrans 
					<<  (m4int16_t)(vectorAux.size())  << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} else {
				ao_Array[ENG_TRANS_I_2_5_START] = vectorAux[0] ;
				ao_Array[ENG_TRANS_I_2_5_STOP]  = vectorAux[1] ;
				for (B=0;B<ENG_TRANS_I_2_5_DIGIT_COUNT;++B) {
					ao_Array[ENG_TRANS_I_2_5_ZERO+B]  = vectorAux[2+B] ;
				} ;
			} ;
			
		} else if ( typeInfo == '8' ) {

			ao_Array[ENG_TRANS_TYPE_POS] = ENG_SOFT_TRANS_TYPE_128 ;
			if ( vectorAux.size() != ENG_TRANS_128_COUNT )
			{
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Translation string %0:s in SoftFont,  missing codes (there are %1:s)" ) 
					<< ai_pcTrans 
					<<  (m4int16_t)(vectorAux.size())  << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} else {
				m4int16_t idx ;

				idx=0 ; 

				for (B=0;B<ENG_TRANS_128_C_DIGIT_COUNT;++B) {
					ao_Array[ENG_TRANS_128_C_ZERO_POS+B]  = vectorAux[idx] ;
					++idx ;
				} ;

				ao_Array[ENG_TRANS_128_FNC1] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_START_A] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_START_B] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_START_C] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_STOP] = vectorAux[idx] ; ++idx ;

				ao_Array[ENG_TRANS_128_A_FNC2] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_FNC2] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_FNC3] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_FNC3] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_FNC4] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_FNC4] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_SHIFT] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_SHIFT] = vectorAux[idx] ; ++idx ;

				ao_Array[ENG_TRANS_128_B_TO_A] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_TO_C] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_TO_B] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_TO_C] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_C_TO_A] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_C_TO_B] = vectorAux[idx] ; ++idx ;

				ao_Array[ENG_TRANS_128_A_CTRL] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_BLANK] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_BLANK] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_DIGIT] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_DIGIT] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_PUNCT] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_PUNCT] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_MAY] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_MAY] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_A_PUNCT2] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_PUNCT2] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_ASC96] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_MIN] = vectorAux[idx] ; ++idx ;
				ao_Array[ENG_TRANS_128_B_123] = vectorAux[idx] ; ++idx ;

			} ;
	
		} else if ( typeInfo == 'P' ) {

			ao_Array[ENG_TRANS_TYPE_POS] = ENG_SOFT_TRANS_TYPE_POSTNET ;
			if ( vectorAux.size() != ENG_TRANS_POSTNET_COUNT )
			{
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Translation string %0:s in SoftFont,  missing codes (there are %1:s)" ) 
					<< ai_pcTrans 
					<<  (m4int16_t)(vectorAux.size())  << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} else {
				ao_Array[ENG_TRANS_POSTNET_START] = vectorAux[0] ;
				ao_Array[ENG_TRANS_POSTNET_STOP]  = vectorAux[1] ;
				for (B=0;B<ENG_TRANS_POSTNET_DIGIT_COUNT;++B) {
					ao_Array[ENG_TRANS_POSTNET_ZERO+B]  = vectorAux[2+B] ;
				} ;
			} ;

		} else if ( typeInfo == 'M' ) {

			ao_Array[ENG_TRANS_TYPE_POS] = ENG_SOFT_TRANS_TYPE_PSC_MIXED ;
			if ( vectorAux.size() != ENG_TRANS_PSC_MIXED_COUNT )
			{
				*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
					"Translation string %0:s in SoftFont,  missing codes (there are %1:s)" ) 
					<< ai_pcTrans 
					<<  (m4int16_t)(vectorAux.size())  << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} else {
				ao_Array[ENG_TRANS_PSC_MIXED_SHIFT_IN] = vectorAux[0] ;
				ao_Array[ENG_TRANS_PSC_MIXED_SHIFT_OUT]  = vectorAux[1] ;
				ao_Array[ENG_TRANS_PSC_MIXED_EURO] = vectorAux[2] ;
			} ;

		
		} else {
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Translation string %0:s in SoftFont has a unknow type info" ) 
				<< ai_pcTrans << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} ;

exit:
	return SwOk ;
}


m4bool_t ClRFN_RFN_W::SaveSoftFontsToFile() 
{
	SoftFontList_t::iterator It ;
	OffList_t::iterator  OffIt ;
	m4bool_t SwOk=M4_TRUE ;
	size_t LongAux ;
	m4int16_t Trans[ENG_TRANS_ARRAY_SIZE], B ;
	m4int16_t PSC_AFM_Widths[ENG_PSC_AFM_WIDTHS_ARRAY_SIZE] ;
	
	m4int32_t PosOff, PosAux, PosSize ;

	m_oFile.SetPos( m_oFile.GetSize() ) ;
	
	for ( It=m_oSoftFonts.begin() ; It!=m_oSoftFonts.end(); ++It ) {

		//Traduccion de cadena Trans a valores en array Trans

		SwOk = SwOk && ClRFN_RFN_W::TranslationStrToArray( (*It).second.m_oTrans, Trans ) ;
		if ( !SwOk ) goto exit ;

		//A grabar el SoftFont Block. 

		PosOff = m_oFile.GetPos()  ;

		WStr(((*It).first).InChar(0), RFN::SOFT_FONT_NAME ) ;
		m_oFile.W(Trans, sizeof(m4int16_t)*ENG_TRANS_ARRAY_SIZE) ;
		m_oFile.W( (*It).second.m_iPSC_AFM_Widths, 
			sizeof(m4int16_t)*ENG_PSC_AFM_WIDTHS_ARRAY_SIZE)  ;
		PosSize = m_oFile.GetPos()  ; WLong( 0 ) ;
		SwOk = SwOk && m_oFile.WFile( (*It).second.m_oFileName, &LongAux ) ;
		if (!SwOk) goto exit ;
		
		PosAux = m_oFile.GetPos()  ;

		m_oFile.SetPos(PosSize) ;
		WLong( LongAux ) ;

		//Actualizar Offsets

		for ( OffIt=(*It).second.m_oOffsets.begin() ; OffIt!=(*It).second.m_oOffsets.end(); ++OffIt ) {
			m_oFile.SetPos( *OffIt ) ;
			WLong( PosOff );
		} ;

		m_oFile.SetPos(PosAux) ;

	} ;
exit:
	return SwOk ;
}

m4bool_t ClRFN_RFN_W::ExtractFromPRN_To_USP(m4char_t *ai_pcPRNFile, m4char_t *ai_pcUSPFile, 
		m4int16_t ai_iCharSetOffset, m4int16_t *ao_poFontsCreated ) 
{
	m4bool_t SwOk=M4_TRUE, SwHeaderToProcess, Sw1, Sw2 ;
	ClMIT_File F ;
	size_t PRNSize, USPCount, CharCount, MinChar, MaxChar ;
	StMIT_Inspector * Insp = GET_INSP() ;
	m4char_t *PRNPos, *BegHeader, *BegHeaderData ;
	size_t SizeHeaderData ;
	ClMIT_Str PRN, StrAux ;
	m4int32_t CharCode, SizeDef ;
	m4char_t *BegDef, *BegDefSeq ;

	*ao_poFontsCreated=0 ;

	//------------>Cargar PRN en RAM
	// UNICODE REPORTS.
	m4bool_t bAddBOM = M4_FALSE;
	if ( ! F.Open( ai_pcPRNFile, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN , bAddBOM )  ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	PRNSize=F.GetSize() ;
	if ( PRNSize > PRN_MAX_SIZE || PRNSize==0 ) {
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"PRN size is %0:s, and the range is 0 and %1:s" ) 
			<< (m4uint32_t)PRNSize << (m4uint32_t)PRN_MAX_SIZE << SEND_MSG ;
		F.Close() ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	PRN.Accommodate(PRNSize+2) ;	//+2 por si acaso
	*PRN.InChar(PRNSize)=M4_END_STR ;	//Para garantizar el fin de cadena.
	F.R( PRN.InChar(0), PRNSize ) ;
	F.Close() ;

	//------------>Bucle Principal de lectura de PRN. 

	for ( PRNPos=PRN.InChar(0), SwHeaderToProcess=M4_FALSE, USPCount=0 ; ;  ) {

		//---->Localiza la cabecera de una SoftFont
		
		if ( ! SwHeaderToProcess ) {
			if ( ! FindSoftFontHeader(PRNPos, &BegHeader, &BegHeaderData, &SizeHeaderData ) ) {
				goto exit ;
			} ;
		} else SwHeaderToProcess=M4_FALSE ;
		
		//---->Cabecera localizada, a copiarla y saltarsela !

		StrAux.Accommodate( strlen(ai_pcUSPFile)+20 ) ;
		sprintf( StrAux.InChar(0), "%s_%i.usp", ai_pcUSPFile, (int)USPCount ) ;
		StrAux.OverStringValid() ;	

		if ( F.IsOpen() ) F.Close() ;
	
		// UNICODE REPORTS.
		m4bool_t bAddBOM = M4_FALSE;
		if ( ! F.Open( StrAux, ClMIT_File::OVERWRITE, ClMIT_File::WRITE, ClMIT_File::TRY_OPEN , bAddBOM )  ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		
		++ (*ao_poFontsCreated) ;
		++ USPCount ;

		F.W( BegHeader, (BegHeaderData-BegHeader) ) ;
		F.W( BegHeaderData, SizeHeaderData ) ;
		
		PRNPos +=  (BegHeaderData-PRNPos)+SizeHeaderData ;

		//---->Ahora a copiar cualquier sucesion de definicion de caracteres forma parte de la 
		//	SoftFont. Pero hay que verificar que si entre definiciones hay una cabecera de 
		//	SoftFont, debemos acabar con la actual y procesar la otra.

		MinChar = 255 ; MaxChar= 0 ;
		CharCount=0 ;
		while ( M4_TRUE ) {

			Sw1 = FindSoftFontHeader(PRNPos, &BegHeader, &BegHeaderData, &SizeHeaderData ) ;
			Sw2 = FindSoftFontCharDef(PRNPos, &CharCode, &SizeDef, &BegDefSeq, &BegDef ) ;
			if ( ( Sw1 && !Sw2) || ( Sw1 && Sw2 && BegHeader < BegDefSeq ) ) {
				SwHeaderToProcess = M4_TRUE ;
				break ;
			} ;

			if (!Sw2) break ;
			
			++CharCount ;
			MinChar = M4_MIN( MinChar, CharCode+ai_iCharSetOffset) ;
			MaxChar = M4_MAX( MaxChar, CharCode+ai_iCharSetOffset) ;

			StrAux.Accommodate( 80 ) ;
			sprintf( StrAux.InChar(0), "\033*c%iE\033(s%luW", 
				(int)(CharCode+ai_iCharSetOffset),(unsigned long)SizeDef ) ;
			StrAux.OverStringValid() ;	
			F.WStr(StrAux) ;			
			F.W(BegDef,SizeDef) ;			
			PRNPos += (BegDef-PRNPos)+SizeDef ;
		} ;

		*GET_INSP()->m_poTrace << ClMIT_Msg(GROUP_ID, 
			"USP Font created, %0:s characters, range from %1:s to %2:s" )
			<< (m4uint32_t)CharCount << (m4uint32_t)MinChar << (m4uint32_t)MaxChar
			<< SEND_MSG ;
	} ;

	//------------>Fin
exit:
	
	return SwOk ;
}

m4bool_t ClRFN_RFN_W::FindSoftFontHeader(m4char_t *ai_pcFrom, m4char_t **ao_pcBegHeader, 
		m4char_t **ao_pcBegHeaderData, size_t *ao_SizeData ) 
{
	m4bool_t SwFound= M4_FALSE ;
	ClMIT_Str StrNum ;

	while (M4_TRUE) {

		*ao_pcBegHeader = ai_pcFrom = strstr(ai_pcFrom,"\033)s") ;	//Localiza posible comienzo ....
		if (!ai_pcFrom) break ;				//... y si no lo hay, fuera.
		ai_pcFrom += strlen("\033)s") ;		//Se salta lo ya leido
											//Haya numero actualizando Pos ....
		ai_pcFrom += StrNum.GetString(ClMIT_Str::WHILE_CHAR, ai_pcFrom, "0123456789" ) ;
		if ( StrNum.StrLen() == 0 )  continue ;	//Si no leyó numero, a leer otra cabecera
		if ( *ai_pcFrom != 'W' ) continue ;	//Y si lo siguiente no es una W, a leer otra cabecera
		++ai_pcFrom ;						//Se salta la W

		SwFound=M4_TRUE ;

		*ao_pcBegHeaderData = ai_pcFrom ;
		*ao_SizeData = atol( StrNum ) ;

		break ;
	} ;

	return SwFound ;
}


m4bool_t ClRFN_RFN_W::FindSoftFontCharDef(
	m4char_t *ai_pcFrom, m4int32_t *ao_CharCode, m4int32_t *ao_SizeDef, 
	m4char_t **ao_pcBegDefSeq, m4char_t **ao_pcBegDef ) 
{
	m4bool_t SwFound= M4_FALSE ;
	ClMIT_Str StrNum1, StrNum2 ;

	while (M4_TRUE) {

		*ao_pcBegDefSeq = ai_pcFrom = strstr(ai_pcFrom,"\033*c") ;	//Localiza posible comienzo ....
		if (!ai_pcFrom) break ;				//... y si no lo hay, fuera.
		ai_pcFrom += strlen("\033*c") ;		//Se salta lo ya leido
											//Haya numero actualizando Pos ....
		ai_pcFrom += StrNum1.GetString(ClMIT_Str::WHILE_CHAR, ai_pcFrom, "0123456789" ) ;
		if ( StrNum1.StrLen() == 0 )  continue ;	//Si no leyó numero, a leer otra def
		if ( strncmp(ai_pcFrom,"E\033(s", strlen("E\033(s") )==0 ) {	//Lo siguiente debe ser "E<Esc>(s"
			ai_pcFrom += strlen("E\033(s") ;		//Incrementa lo leido
		} else {
			continue ;								//Si no leyó OK, a leer otra def
		} ;
											//Haya numero actualizando Pos ....
		ai_pcFrom += StrNum2.GetString(ClMIT_Str::WHILE_CHAR, ai_pcFrom, "0123456789" ) ;
		if ( StrNum2.StrLen() == 0 )  continue ;	//Si no leyó numero, a leer otra def
		if ( *ai_pcFrom != 'W' ) continue ;	//Y si lo siguiente no es una W, a leer otra def
		++ai_pcFrom ;						//Se salta la W

		SwFound=M4_TRUE ;

		*ao_CharCode =  atol( StrNum1 ) ;
		*ao_SizeDef =   atol( StrNum2 ) ;
		*ao_pcBegDef = ai_pcFrom ;

		break ;

	} ;

	return SwFound ;

}


m4bool_t ClRFN_RFN_W::ExtractFromPRN_To_PFA(m4char_t *ai_pcPRNFile, m4char_t *ai_pcPFAFile, 
		m4int16_t *ao_poFontsCreated ) 
{
	m4bool_t SwOk=M4_TRUE  ;
	ClMIT_File F ;
	size_t PRNSize, PFACount ;
	StMIT_Inspector * Insp = GET_INSP() ;
	m4char_t *PRNPos, *BegSoftFont, *EndSoftFont  ;
	ClMIT_Str PRN, StrAux ;

	*ao_poFontsCreated=0 ;

	//------------>Cargar PRN en RAM
	// UNICODE REPORTS.
	m4bool_t bAddBOM = M4_FALSE;	
	if ( ! F.Open( ai_pcPRNFile, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN, bAddBOM )  ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	PRNSize=F.GetSize() ;
	if ( PRNSize > PRN_MAX_SIZE || PRNSize==0 ) {
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"PRN size is %0:s, and the range is 0 and %1:s" ) 
			<< (m4uint32_t)PRNSize << (m4uint32_t)PRN_MAX_SIZE << SEND_MSG ;
		F.Close() ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	PRN.Accommodate(PRNSize+2) ;	//+2 por si acaso
	*PRN.InChar(PRNSize)=M4_END_STR ;	//Para garantizar el fin de cadena.
	F.R( PRN.InChar(0), PRNSize ) ;
	F.Close() ;

	//------------>Bucle Principal de lectura de PRN. 

	for ( PRNPos=PRN.InChar(0), PFACount=0 ; ;  ) {

		//---->Localiza el comentario de comienzo de SoftFont
		
		PRNPos = strstr(PRNPos,"%%BeginFont") ;
		if (!PRNPos) break ;	//Si no hay más, fuera!
		PRNPos += strcspn( PRNPos, "\n\r") ;	//Se salta la linea
		PRNPos += strspn( PRNPos, "\n\r") ;
		BegSoftFont = PRNPos ;

		//---->Y ahora el de fin 

		PRNPos = strstr(PRNPos,"%%EndFont") ;
		if (!PRNPos) break ;	//Si no hay más, fuera!
		EndSoftFont = PRNPos-1 ;

		//---->A copiar la font y saltarsela !

		StrAux.Accommodate( strlen(ai_pcPFAFile)+20 ) ;
		sprintf( StrAux.InChar(0), "%s_%i.pfa", ai_pcPFAFile, (int)PFACount ) ;
		StrAux.OverStringValid() ;	

		if ( F.IsOpen() ) F.Close() ;

		// UNICODE REPORTS.
		m4bool_t bAddBOM = M4_FALSE;
		if ( ! F.Open( StrAux, ClMIT_File::OVERWRITE, ClMIT_File::WRITE, ClMIT_File::TRY_OPEN , bAddBOM )  ) {
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		++ (*ao_poFontsCreated) ;
		++ PFACount ;

		F.W( BegSoftFont, (EndSoftFont-BegSoftFont+1) ) ;

	} ;

	//------------>Fin
exit:
	
	return SwOk ;

}

m4bool_t ClRFN_RFN_W::TranslationVector( m4char_t *ai_pcTrans, IntVector_t &ao_Vector, m4char_t *ao_FirstEntry ) 
{
	ClMIT_Args ArgAux ;
	m4bool_t SwOk=M4_TRUE ;
	m4uint32_t B, toB, C, toC ;
	m4int16_t aux, base ;
	ClMIT_Str strAux ;

	ArgAux.Init( ai_pcTrans, '*', " ," ) ;
	if ( ArgAux.GetArgCount()==0 ) {
		SwOk=M4_FALSE ;
		goto exit ;

	} ;
	for ( 
		B=0, toB = ArgAux.GetArgCount(), ao_Vector.clear() ;
		B<toB; 
		++B ) 
	{
		aux = ArgAux.GetSubArgCount(B) ;
		if (B==0) {

			strAux.StrCpy(ArgAux.GetArg(B)) ;
			strAux.FullTrim() ;
			*ao_FirstEntry=*strAux.InChar(0) ;
		
		} else if (aux==1) {
			
			//Formato normal un valor
			
			ao_Vector.push_back( ArgAux.GetLong(B,0) ) ;
		
		} else if (aux==2) {

			//Formato base * repeticiones

			for (
				C=0, toC=ArgAux.GetLong(B,1), base=ArgAux.GetLong(B,0) ; 
				C<toC; 
				++C, ++base ) 
			{
				ao_Vector.push_back( base ) ;
			} ;
		
		} else {
			*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID, 
				"Error de formato en la informacion SoftTrans, argumento %0:s") 
				<< ArgAux.GetArg(B)
				<< GENERAL_MSG << SEND_MSG   ;
			SwOk=M4_FALSE ;
			goto exit ;

		} ;
	} ;

exit:
	
	return SwOk;
}


