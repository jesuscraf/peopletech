//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clrepreq.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:21/10/97
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

#include "clrepreq.h"

void ClENG_ReportRequest::Init () 
{ 
	ClENG_ReportRequest::End() ; 

	m_LinkManager=BUILD_MANAGER;
	m_LinkDesign=ENG_BUILD_DESIGN;
	m_LinkData=BUILD_DATA;
	m_oReportId.StrCpy("NONE") ;
	m_iExecutionType=0 ;
	m_iMetadataLang=0;
	m_iDataLang=0;
	m_iCopies=1;
	m_Dvc=DVC_PCL ;
	m_Duplex=StENG_DvcParams::NO_DUPLEX ;
	m_Tumble=StENG_DvcParams::NO_TUMBLE ;
	m_bTableOfContents=M4_TRUE;
	m_bSwPublish = M4_FALSE ;
	m_HTMLNavigation=ClENG_DvcHTML::NAVIGATION_ACTIVE;
	m_HTMLMenu=ClENG_DvcHTML::BOTTOM;
	m_bHTMLSearch=M4_TRUE;
	m_bHTMLZoom=M4_TRUE;
	m_bRePrinter=M4_FALSE ;	
	m_Preserve=ClENG_ReportOuputRepository::NO_PRESERVE ;
	m_bCreateDir=M4_TRUE;
	m_Resolution=ClENG_DvcPCL::P_600_DPI ;
	m_PSCResolution=ClENG_DvcPSC::P_600_DPI ;
	m_iXFactor=60 ;
	m_iYFactor=180 ;
	m_JPageSize.StrCpy("") ;
	m_BinDefault=ClENG_PagePrinter::PRINTER_DEFAULT_BIN ;
	m_BinPcl=-1;
	m_uiIdFont=-1 ;
	m_bLoadBreakDesign=M4_TRUE ;
	m_Break=ClENG_Report::NO_BREAK ;
	m_bSwEOPSep ;
	m_bHTMLSwPageFill=M4_FALSE;
	m_bTXTSwPageFill=M4_TRUE;
	m_bPackDesign=m_bPackData=M4_FALSE ;
	m_DesignAutoload = m_DataAutoload = ClMIT_Chn::AUTOLOAD_OFF ;
	m_bSwDateTraceMove = M4_FALSE ;

	m_bTestField =  m_bTestCnt = m_bTestCell =M4_FALSE ;
	m_bTestOnlyDesign=M4_FALSE ;

	m_oDestCurrency.StrCpy("");
	m_dExchangeDate = 0;
	m_oExchangeType.StrCpy("");

	m_oToPrinter.StrCpy("");

	m_bDelete = M4_FALSE ;

	m_cFillChar = ' ' ;
	m_oEOLSeq.StrCpy("\r\n") ;
	m_oEOPSeq.StrCpy("\f") ;
	m_bClipColumn = M4_TRUE ;
	m_bTXTSwPageFill = M4_TRUE ;

	m_FileParams.Init() ;

	m_MoveTraceMode = ClMIT_Trace::NEVER_MOVE ;
	m_oMoveTraceFile.StrCpy("");

	m_bSwTimeExp=M4_FALSE ;

	m_oImagePath.StrCpy("") ;

	m_bUpdateDependencies=M4_FALSE ;
	m_bRebuildDependencies=M4_FALSE ;

	m_bExtractStrings=M4_FALSE;

	m_bSwColour=M4_FALSE ;

	m_DeclareExtFile=NOT_DECLARED ;

	m_oIDChannel.StrCpy("");

	m_oHTML_StartTemplate.StrCpy("m4_html_data_start.html") ;
	m_oHTML_EndTemplate.StrCpy("m4_html_data_end.html") ; 
	m_oHTML_BreakTemplate.StrCpy("m4_main.html") ; 

	// Tratamiento /FORCE_UNICODE_OUTPUT
	m_bForceUnicodeOutput = M4_FALSE ;

	// Preprocesamiento de los campos estándar para Excel (TXT_UNICODE).
	m_bPreprocessForExcel = M4_FALSE;

	// Caracter delimitador en los ficheros TXT.
	m_cFieldDelimiter = -1;

	// Emulación impresión duplex.
	m_bDuplexEmulation = M4_FALSE;

}

ClENG_ReportRequest::ClENG_ReportRequest() 
{
	m_poInsp=GET_INSP();
}

void ClENG_ReportRequest::End() 
{
}

void ClENG_ReportRequest::Init(ClENG_ReportRequest *ai_poFrom) 
{
	ClENG_ReportRequest::End() ;

	m_LinkManager		= ai_poFrom->m_LinkManager ;

	m_oManagerContext	= ai_poFrom->m_oManagerContext ;	
	m_oDesignContext	= ai_poFrom->m_oDesignContext ;
	m_oDataContext		= ai_poFrom->m_oDataContext ;

	m_LinkDesign		= ai_poFrom->m_LinkDesign ;
	m_LinkData			= ai_poFrom->m_LinkData ;
	m_oReportId.StrCpy( ai_poFrom->m_oReportId ) ;
	m_iMetadataLang		= ai_poFrom->m_iMetadataLang ;
	m_iDataLang			= ai_poFrom->m_iDataLang ;
	m_iExecutionType	= ai_poFrom->m_iExecutionType ;
	m_iCopies			= ai_poFrom->m_iCopies ;
	m_Dvc				= ai_poFrom->m_Dvc ;
	m_bRePrinter		= ai_poFrom->m_bRePrinter ;
	m_DesignAutoload	= ai_poFrom->m_DesignAutoload ;
	m_DataAutoload		= ai_poFrom->m_DataAutoload ;
	m_BinDefault		= ai_poFrom->m_BinDefault ;
	m_bPackDesign		= ai_poFrom->m_bPackDesign ;
	m_bPackData			= ai_poFrom->m_bPackData ;
	m_Duplex			= ai_poFrom->m_Duplex ;
	m_Tumble			= ai_poFrom->m_Tumble ;
	m_bTableOfContents  = ai_poFrom->m_bTableOfContents ;
	m_bSwPublish		= ai_poFrom->m_bSwPublish ;
	m_HTMLNavigation	= ai_poFrom->m_HTMLNavigation ;
	m_HTMLMenu			= ai_poFrom->m_HTMLMenu ;
	m_bHTMLSearch		= ai_poFrom->m_bHTMLSearch ;
	m_bHTMLZoom			= ai_poFrom->m_bHTMLZoom ;
	m_DeclareExtFile	= ai_poFrom->m_DeclareExtFile ;
	m_oToPrinter.StrCpy( ai_poFrom->m_oToPrinter) ;
	m_bSwTimeExp		= ai_poFrom->m_bSwTimeExp ;
	m_Resolution		= ai_poFrom->m_Resolution ;
	m_bSwColour			= ai_poFrom->m_bSwColour ;
	m_PSCResolution		= ai_poFrom->m_PSCResolution ;
	m_uiIdFont			= ai_poFrom->m_uiIdFont ;
	m_cFillChar			= ai_poFrom->m_cFillChar ;
	m_oEOLSeq.StrCpy( ai_poFrom->m_oEOLSeq ) ;
	m_oEOPSeq.StrCpy( ai_poFrom->m_oEOPSeq ) ;
	m_bClipColumn		= ai_poFrom->m_bClipColumn ;
	m_bTXTSwPageFill	= ai_poFrom->m_bTXTSwPageFill ;
	m_bSwEOPSep			= ai_poFrom->m_bSwEOPSep ;
	m_bHTMLSwPageFill	= ai_poFrom->m_bHTMLSwPageFill ;
	m_iXFactor			= ai_poFrom->m_iXFactor ;
	m_iYFactor			= ai_poFrom->m_iYFactor ;
	m_JPageSize.StrCpy(ai_poFrom->m_JPageSize) ;
	m_FileParams		= ai_poFrom->m_FileParams ;
	m_MoveTraceMode		= ai_poFrom->m_MoveTraceMode ;
	m_bSwDateTraceMove	= ai_poFrom->m_bSwDateTraceMove ;
	m_oMoveTraceFile.StrCpy( ai_poFrom->m_oMoveTraceFile ) ;
	m_Preserve			= ai_poFrom->m_Preserve ;
	m_bCreateDir		= ai_poFrom->m_bCreateDir ;
	m_bDelete			= ai_poFrom->m_bDelete ;
	m_bLoadBreakDesign	= ai_poFrom->m_bLoadBreakDesign ;
	m_Break				= ai_poFrom->m_Break ;
	m_iBreakSec			= ai_poFrom->m_iBreakSec ;
	m_bTestField			= ai_poFrom->m_bTestField ;
	m_bTestCnt			= ai_poFrom->m_bTestCnt ;
	m_bTestCell			= ai_poFrom->m_bTestCell ;
	m_bTestOnlyDesign	= ai_poFrom->m_bTestOnlyDesign ;
	m_oImagePath.StrCpy( ai_poFrom->m_oImagePath ) ;
	m_bImageTestOk		= ai_poFrom->m_bImageTestOk ;
	m_oDestCurrency.StrCpy( ai_poFrom->m_oDestCurrency ) ;
	m_dExchangeDate		= ai_poFrom->m_dExchangeDate ;
	m_oExchangeType.StrCpy( ai_poFrom->m_oExchangeType ) ;
	m_bUpdateDependencies	= ai_poFrom->m_bUpdateDependencies ;
	m_bRebuildDependencies	= ai_poFrom->m_bRebuildDependencies ;
	m_bExtractStrings		= ai_poFrom->m_bExtractStrings ;
	m_oIDChannel.StrCpy( ai_poFrom->m_oIDChannel );

	m_oHTML_StartTemplate.StrCpy(	ai_poFrom->m_oHTML_StartTemplate ) ;
	m_oHTML_EndTemplate.StrCpy(		ai_poFrom->m_oHTML_EndTemplate ) ; 
	m_oHTML_BreakTemplate.StrCpy(	ai_poFrom->m_oHTML_BreakTemplate ) ; 

	m_bForceUnicodeOutput = ai_poFrom->m_bForceUnicodeOutput ;
	m_bPreprocessForExcel = ai_poFrom->m_bPreprocessForExcel ;
	m_cFieldDelimiter = ai_poFrom->m_cFieldDelimiter ;

	m_bDuplexEmulation = ai_poFrom->m_bDuplexEmulation ;

}


m4char_t *ClENG_ReportRequest::LManagerToTraceStr( LINK_MANAGER_MODE ai_Mode ) 
{
	m4char_t *Str ;

	switch (ai_Mode) {
	case BUILD_MANAGER:
		Str="BUILD_MANAGER (Manager created and destroyed, and  local channels also)" ;
		break ;
	case LINK_MANAGER:
		Str="LINK_MANAGER (Manager used only)" ;
		break ;
	} ;
	return Str ;
}

m4char_t *ClENG_ReportRequest::LDesignToTraceStr(  ENG_LINK_DESIGN_MODE ai_Mode ) 
{
	m4char_t *Str ;

	switch (ai_Mode) {
	case ENG_BUILD_DESIGN:
		Str="BUILD_DESIGN (Channel create Y, parameters load Y, set filter Y, load Y, unload Y, destroy Y)" ;
		break ;
	case ENG_LINK_LOAD_DESIGN:
		Str="LINK_LOAD_DESIGN (Channel create N, parameters load N, set filter Y, load Y, unload N, destroy N)" ;
		break ;
	case ENG_LINK_DESIGN:
		Str="LINK_DESIGN (Channel create N, parameters load N, set filter N, load N, unload N, destroy N)" ;
		break ;
	case ENG_LINK_LOAD_UNLOAD_DESIGN:
		Str="LINK_LOAD_UNLOAD_DESIGN (Channel create N, parameters load N, set filter Y, load Y, unload Y, destroy N)" ;
		break ;
	case ENG_LINK_UNLOAD_DESIGN:
		Str="LINK_UNLOAD_DESIGN (Channel create N, parameters load N, set filter N, load N, unload Y, destroy N)" ;
		break ;
	} ;
	return Str ;
}

m4char_t *ClENG_ReportRequest::LDataToTraceStr(    LINK_DATA_MODE ai_Mode ) 
{
	m4char_t *Str ;

	switch (ai_Mode) {
	case BUILD_DATA:
		Str="BUILD_DATA (Channel create Y, parameters load N, set filter N, load Y, unload Y, destroy Y)" ;
		break ;
	case LINK_LOAD_DATA:
		Str="LINK_LOAD_DATA (Channel create N, parameters load N, set filter N, load Y, unload N, destroy N)" ;
		break ;
	case LINK_DATA:
		Str="LINK_DATA (Channel create N, parameters load N, set filter N, load N, unload N, destroy N)" ;
		break ;
	case LINK_LOAD_UNLOAD_DATA:
		Str="LINK_LOAD_UNLOAD_DATA (Channel create N, parameters load N, set filter N, load Y, unload Y, destroy N)" ;
		break ;
	case LINK_UNLOAD_DATA:
		Str="LINK_UNLOAD_DATA (Channel create N, parameters load N, set filter N, load N, unload Y, destroy N)" ;
		break ;
	} ;
	return Str ;
}
