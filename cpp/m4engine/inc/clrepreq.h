//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clrepreq.h
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
//    Coleccion de parametros de report
//
//
//==============================================================================
#ifndef __CLREPREQ__H__
#define __CLREPREQ__H__

#include "execontx.hpp"

#include "clstr.h"

#include "pageprn.h"
#include "cldvcpcl.h"
#include "cldvcfi.h"
#include "cldvcpsc.h"
#include "cldvcpdf.h"
#include "cldvchtml.h"
#include "clreport.h"
#include "clmetric.h"
#include "designin.h"
#include "xapichn.h"

class ClENG_ReportRequest {
MIT_PRIVATE:
	StMIT_Inspector* m_poInsp;	//Simple link
MIT_PUBLIC:

	enum DVC_TYPE { DVC_PCL, DVC_ESCP, DVC_TXT, DVC_RVW, DVC_HTML, DVC_PSC, DVC_PDF, DVC_TXT_UNICODE, DVC_PDF_UNICODE} ;

	enum LINK_MANAGER_MODE {
		BUILD_MANAGER,	//Defecto, hace un ClMIT_Chn::StartUp(). El manager sera destruido, lo
						//	que significa que cualquier canal que posea tambien se 
						//	destruira.
		LINK_MANAGER	//Hace un ClMIT_Chn::StartUp(m_oManagerContext). 	
	} m_LinkManager ;
	ClVMRunContext m_oManagerContext;	

	ENG_LINK_DESIGN_MODE m_LinkDesign ;
	ClVMRunContext m_oDesignContext	;

	enum LINK_DATA_MODE {
		BUILD_DATA,		//Defecto, igual que antes. El nombre del canal se coje del canal de
						//	diseño. Reprinter discrimina que nombre usar. 
		LINK_LOAD_DATA,	//Como antes.
		LINK_DATA,		//Como antes.
		LINK_LOAD_UNLOAD_DATA,	//Como antes.
		LINK_UNLOAD_DATA		//Como antes.
	} m_LinkData ;
	ClVMRunContext m_oDataContext ;
	
	ClMIT_Str m_oReportId /*def "NONE"*/ ;
	m4int16_t m_iExecutionType /*def 0*/, m_iCopies /*def 1*/;
	DVC_TYPE   m_Dvc ;		//def DVC_PCL

	//Procesos de ejecución tecnológicos
	m4uint16_t m_iMetadataLang; //DEF: 1
	m4uint16_t m_iDataLang;		//DEF: 0

	m4bool_t   m_bRePrinter ;	//def false
	ClMIT_Chn::AUTOLOAD_MODE m_DesignAutoload ;	//def AUTOLOAD_OFF
	ClMIT_Chn::AUTOLOAD_MODE m_DataAutoload ;		//def AUTOLOAD_OFF
	ClENG_PagePrinter::PAPER_SOURCE m_BinDefault ;	//def PRINTER_DEFAULT_BIN
	m4int32_t m_BinPcl;	// default -1.
	m4bool_t m_bPackDesign, m_bPackData ;	//def FALSE

	StENG_DvcParams::DUPLEX_MODE   m_Duplex ;	//def NO_DUPLEX
	StENG_DvcParams::TUMBLE_MODE   m_Tumble ;	//def NO_TUMBLE

	DECLARE_EXTERNAL_FILE m_DeclareExtFile ; //def NOT_DECLARED

	ClMIT_Str m_oToPrinter ;	//Def ""

	m4bool_t m_bSwTimeExp ;	//Def false. Si true, saca la traza de time expensis.

	m4bool_t m_bTableOfContents;  //DEF: TRUE. Genera una tabla de contenidos indexado.

	m4bool_t m_bSwPublish ; //DEF:FALSE, activar publicacion

	//---->Para PCL

	ClENG_DvcPCL::PAGE_RESOLUTION m_Resolution ;	//def P_600_DPI
	m4bool_t m_bSwColour ;	//def FALSE

	//---->Fin de PCL

	//---->Para PSC

	ClENG_DvcPSC::PAGE_RESOLUTION m_PSCResolution ;	//def P_600_DPI

	//---->Fin de PSC


	//--->Para TXT

	//Si -1, usar la primera font. Ojo, comparar con ==-1, no un<0, pues es unsigned.
	m4uint32_t m_uiIdFont ;		//def -1

	m4char_t m_cFillChar ;	//Def ' '
	ClMIT_Str  m_oEOLSeq ;	//Def CR/LF
	ClMIT_Str  m_oEOPSeq ;	//Def FF
	m4bool_t m_bClipColumn ;	//Def TRUE
	m4bool_t m_bTXTSwPageFill ;	//Para TXT def TRUE. 

	//--->Fin de TXT

	//--->Para TXT UNICODE
	m4bool_t m_bPreprocessForExcel; // Preprocesamiento de campos estandar para Excel.
	m4char_t m_cFieldDelimiter; // Delimitador de los campos de ficheros TXT_UNICODE.
	//--->Fin de TXT UNICODE

	//---->Para PDF(UNICODE)
	m4bool_t m_bForceUnicodeOutput; // Tratamiento /FORCE_UNICODE_OUTPUT
	//---->Fin de PDF(UNICODE)

	//---->Para HTML

	m4bool_t m_bSwEOPSep;		//Para HTML, def FALSE
	m4bool_t m_bHTMLSwPageFill ; //Para HTML def FALSE.
	ClENG_DvcHTML::HTML_NAVIGATION  m_HTMLNavigation; //def NAVIGATION_ACTIVE
	ClENG_DvcHTML::HTML_MENU m_HTMLMenu ; //def BOTTOM
	m4bool_t m_bHTMLZoom;	//Para HTML, def TRUE
	m4bool_t m_bHTMLSearch; //Para HTML, def TRUE
	ClMIT_Str m_oHTML_StartTemplate ;	//Para HTML, def m4_html_data_start.html 
	ClMIT_Str m_oHTML_EndTemplate ;		//Para HTML, def m4_html_data_end.html 
	ClMIT_Str m_oHTML_BreakTemplate ;	//Para HTML, def m4_main.html 

	//---->Fin de HTML

	//---->Para ESC
	m4int16_t m_iXFactor, m_iYFactor ;			//def 60 y 180
	ClMIT_Str m_JPageSize ;						//def ""

	//---->Fin de ESC

	//Pone los defectos normales.

	StFileParams m_FileParams ;

	ClMIT_Trace::MOVE_TRACE_MODES m_MoveTraceMode ;	//def NEVER_MOVE
	m4bool_t m_bSwDateTraceMove ;	
	ClMIT_Str m_oMoveTraceFile ;	//def "" 

	ClENG_ReportOuputRepository::PRESERVE_MODE m_Preserve ;			//def NO_PRESERVE
	m4bool_t m_bDelete ;		//def FALSE. Si true, al salir, borra los ficheros output.

	m4bool_t m_bCreateDir;     //def TRUE, indica si los directorios deben crearse o no.

	// Bug 0296814: Emulación de impresión duplex.
	m4bool_t m_bDuplexEmulation; 

	// Breaks de report. Ver ClENG_Report.

	m4bool_t m_bLoadBreakDesign ;	//def TRUE: carga m_Break y m_iBreakSec del canal.
	ClENG_Report::BREAK_MODE  m_Break ;	//def NO_BREAK
	m4int16_t  m_iBreakSec ;

	//----------------Modo Test

	m4bool_t m_bTestField, m_bTestCnt, m_bTestCell ;	//def FALSE 
	m4bool_t m_bTestOnlyDesign ;	//def FALSE 

	ClMIT_Str m_oImagePath ;	//def "" (no tiene parametro se ClMIT_Args)

	m4bool_t m_bImageTestOk ;	//Resultado del test ( lo pone ClENG_OutDvc::End() ).

	//----------------Exchange Multimoneda

	ClMIT_Str     m_oDestCurrency;  //def ""
	m4date_t  m_dExchangeDate;  //def 0
	ClMIT_Str	  m_oExchangeType;  //def ""

	//----------------Dependencias

	m4bool_t  m_bUpdateDependencies;  //def FALSE
	m4bool_t  m_bRebuildDependencies; //def FALSE

	//----------------Regularizacion de expresiones

	m4bool_t m_bExtractStrings;  //def FALSE

	//----------------Canal de datos
	// Para ejecutar informes sobre canales de datos heredados, no sobre
	// el que esta diseñado. Con este ID se sobreescribe el nombre del
	// canal de datos en la capa DDLF_Chn
	ClMIT_Str m_oIDChannel;		//def ""

	

MIT_PUBLIC:

	ClENG_ReportRequest() ;
	virtual ~ClENG_ReportRequest() { ClENG_ReportRequest::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_ReportRequest) ; }
	virtual void End() ;

	// La clase derivada, en el Init, debe de rellenar los miembros de esta clase que desee.
	// Este Init reseta todos los valores a unos defectos.

	void Init() ;

	void Init(ClENG_ReportRequest *ai_poFrom) ;

	// Da el valor de Bin de una pagina, que sera uno especifico o el defecto.
	// VIRTUAL PURA 

	virtual ClENG_PagePrinter::PAPER_SOURCE GetBin( m4int16_t ai_iPage )=0 ;
	
	//Devuelve el valor de m_bRePrinter
	m4bool_t GetReprinter(void) { return m_bRePrinter; };

	static m4char_t *LManagerToTraceStr( LINK_MANAGER_MODE ai_Mode ) ;
	static m4char_t *LDesignToTraceStr(  ENG_LINK_DESIGN_MODE ai_Mode ) ;
	static m4char_t *LDataToTraceStr(    LINK_DATA_MODE ai_Mode ) ;
	m4char_t *LManagerToTraceStr() { return LManagerToTraceStr(m_LinkManager) ;}
	m4char_t *LDesignToTraceStr( ) { return LDesignToTraceStr(m_LinkDesign) ;}
	m4char_t *LDataToTraceStr() { return LDataToTraceStr(m_LinkData); }
} ;

#endif

