//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clreparg.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:08/08/1997
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernández && 24/04/98
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "eng.h"
#include "engdf.inl"
#include "sysinfo.h"
#include "trace.h"
#include "cldvctxt.h"
#include "mitdates.h"
#include "sesschn.h"

#include "clreparg.hmg"
#include "clreparg.h"

#include "cldmcontext.hpp"

#include "file_misc.hpp"

#include "m4objreg.hpp"

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumDvc[] = 
{
	{ "PCL", ClENG_ReportRequest::DVC_PCL},
	{ "PSC", ClENG_ReportRequest::DVC_PSC},
	{ "ESCP",ClENG_ReportRequest::DVC_ESCP },
	{ "TXT", ClENG_ReportRequest::DVC_TXT },
	{ "RVW", ClENG_ReportRequest::DVC_RVW },
	{ "HTML", ClENG_ReportRequest::DVC_HTML },
	{ "PDF", ClENG_ReportRequest::DVC_PDF },
	{ "TXT_UNICODE", ClENG_ReportRequest::DVC_TXT_UNICODE },
	{ "PDF_UNICODE", ClENG_ReportRequest::DVC_PDF_UNICODE },
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumBin[] = 
{
	{ "MANUAL",ClENG_PagePrinter::MANUAL},
	{ "UPPER",ClENG_PagePrinter::UPPER_BIN},
	{ "LOWER",ClENG_PagePrinter::LOWER_BIN},
	{ "PRINTER_DEFAULT", ClENG_PagePrinter::PRINTER_DEFAULT_BIN},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumDPI[] =
{
	{ "300", ClENG_DvcPCL::P_300_DPI},
	{ "600", ClENG_DvcPCL::P_600_DPI},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumDetail[] =
{
	{ "IGNORE",    ClMIT_Msg::IGNORE_ALL},
	{ "GENERAL",   ClMIT_Msg::GENERAL},
	{ "DETAILED",  ClMIT_Msg::DETAILED},
	{ "CRAZY",     ClMIT_Msg::CRAZY},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumGDebug[] =
{
	{ "APICHN",		ENG_TGI_APICHN },
	{ "LOADERCN",	ENG_TGI_LOADERCN } ,
	{ "LOADER ",	ENG_TGI_LOADER } ,
	{ "CORE",		ENG_TGI_CORE }, 
	{ "COREVR",		ENG_TGI_CORE_VR }, 
	{ "REPARG",		ENG_TGI_CLREPARG },
	{ "ANALEX",		ENG_TGI_ANALEX },
	{ "WRAPVB",		ENG_TGI_WRAPVB },
	{ "ALL",		ENG_TGI_ALL_GROUP},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumBreak[] =
{
	{ "NONE",		ClENG_Report::NO_BREAK },
	{ "RECORD",		ClENG_Report::RECORD_BREAK } ,
	{ "SLICE",		ClENG_Report::RECORD_SLICE_BREAK },
	{ "DESIGN",		-1 },	//No es un enum, valor interno
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumPack[] =
{
	{ "DESIGN", ClENG_ReportArgs::PACK_DESIGN},
	{ "DATA",   ClENG_ReportArgs::PACK_DATA},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumAutoload[] = 
{
	{ "OFF", ClMIT_Chn::AUTOLOAD_OFF},
	{ "BLOCK", ClMIT_Chn::AUTOLOAD_BLOCK},
	{ "PRG", ClMIT_Chn::AUTOLOAD_PRG},
	{ "NODESAYS", ClMIT_Chn::AUTOLOAD_NODESAYS},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumDuplex[] = 
{
	{ "OFF",		 StENG_DvcParams::NO_DUPLEX},
	{ "EMULATED",	 StENG_DvcParams::EMULATED_DUPLEX},
	{ "REAL",		 StENG_DvcParams::REAL_DUPLEX},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumTumble[] = 
{
	{ "OFF",		StENG_DvcParams::NO_TUMBLE},
	{ "FALSE",		StENG_DvcParams::TUMBLE_FALSE},
	{ "TRUE",		StENG_DvcParams::TUMBLE_TRUE},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumHTMLNavigation[] = 
{
	{ "OFF",		 ClENG_DvcHTML::NAVIGATION_OFF},
	{ "ACTIVE",		 ClENG_DvcHTML::NAVIGATION_ACTIVE},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumHTMLMenu[] = 
{
	{ "BOTTOM",		 ClENG_DvcHTML::BOTTOM},
	{ "TOP",		 ClENG_DvcHTML::TOP},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumMoveTraceMode[] = 
{
	{ "NEVER",		ClMIT_Trace::NEVER_MOVE},
	{ "ALWAYS",		ClMIT_Trace::MOVE_ALWAYS},
	{ "ON_SUCCESS",	ClMIT_Trace::MOVE_ON_SUCCESS},
	{ "ON_PROBLEM",	ClMIT_Trace::MOVE_ON_PROBLEM},
	{ "ON_ERROR",	ClMIT_Trace::MOVE_ON_ERROR},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumIdDesc[] = 
{
	{ "ID",		 0},
	{ "DESC",	 1},
	{ "ID_DESC", 2},
	{  NULL, 0 }
} ;

ClMIT_Args::EnumTrans_t const ClENG_ReportArgs::m_EnumDeclareExtFile[] =
{
	{ "TEMPORARY",			TEMPORARY},
	{ "REFERENCE",			REFERENCE},
	{ "COPY",				COPY},
	{ "REFERENCE_AND_COPY",	REFERENCE_AND_COPY},
	{  NULL,				0 }
};


m4char_t const ClENG_ReportArgs::Separator=':' ;
m4char_t const * const ClENG_ReportArgs::ArgSeparator=" \t" ;

ClENG_ReportArgs::ClENG_ReportArgs() 
{
	m_poInsp=GET_INSP();
}

void ClENG_ReportArgs::End() { 
	ClMIT_Args::End() ; 
	ClENG_ReportRequest::End(); 
}

m4bool_t ClENG_ReportArgs::Init( const m4char_t **ai_pcArgV, m4int16_t ai_iArgC, 
						ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4bool_t  swOk ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::Init() from argument array", 
			ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	} ;
	#endif

	ClENG_ReportArgs::End() ;

	ClMIT_Args::Init(ai_pcArgV,ai_iArgC, Separator) ;
	ClENG_ReportRequest::Init() ;

	swOk=Load(ai_ErrorEffect) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::Init() from argument array return %0:s", 
			ClMIT_Msg::EXIT_FORMAT) << swOk << SEND_MSG ;
	} ;
	#endif
	return swOk ;
}

m4bool_t ClENG_ReportArgs::Init( const m4char_t *ai_pcCommand, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4bool_t  swOk ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::Init() from command string", 
			ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	} ;
	#endif

	ClENG_ReportArgs::End() ;

	ClMIT_Args::Init(ai_pcCommand,Separator,ArgSeparator) ;
	ClENG_ReportRequest::Init() ;
	swOk=Load(ai_ErrorEffect) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::Init() from command string return %0:s", 
			ClMIT_Msg::EXIT_FORMAT) << swOk << SEND_MSG ;
	} ;
	#endif
	return swOk ;
}

m4bool_t ClENG_ReportArgs::Init( ClENG_ReportArgs *ai_poFrom, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	ClMIT_Str Fixed, Vble ;
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::Init() from copy", 
			ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	} ;
	#endif

	ClENG_ReportArgs::End() ;

	ai_poFrom->GetInitString(&Fixed,&Vble) ;
	if ( Vble.StrLenTrim()>0 ) {
		Fixed.StrInsertChar(ClMIT_Str::STR_CAT,1, ArgSeparator[0]) ;
		Fixed.StrCat(Vble) ;
	} ;

	ClMIT_Args::Init(Fixed,Separator,ArgSeparator) ;

	ClENG_ReportRequest::Init() ;

	SwOk = M4_BOOL( SwOk && Load(ai_ErrorEffect) ) ;

	ClENG_ReportRequest::Init(ai_poFrom) ;	//Hace falta para resto de params (LinkMode ...)
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::Init() from copy return %0:s", 
			ClMIT_Msg::EXIT_FORMAT) << SwOk << SEND_MSG ;
	} ;
	#endif

	return SwOk ;
}

m4bool_t ClENG_ReportArgs::ReLoad( const m4char_t *ai_pcCommand,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::ReLoad() adding command string %0:s", 
			ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcCommand << SEND_MSG ;
	} ;
	#endif

	ClMIT_Str FixedPar, VblePar ;
	ClMIT_Str ArgSep ;

	GetArgSep(&ArgSep) ;

	GetInitString(&FixedPar,&VblePar) ;

	FixedPar.StrInsertChar(ClMIT_Str::STR_CAT,1,*ArgSep.InChar(0));
	FixedPar.StrInsertChar(ClMIT_Str::STR_CAT,1,*ArgSep.InChar(0));
	FixedPar.StrCat(ai_pcCommand) ;
	FixedPar.StrInsertChar(ClMIT_Str::STR_CAT,1,*ArgSep.InChar(0));
	FixedPar.StrInsertChar(ClMIT_Str::STR_CAT,1,*ArgSep.InChar(0));
	FixedPar.StrCat(VblePar) ;
	ClMIT_Args::Init(FixedPar,Separator,ArgSeparator) ;

	SwOk=Load(ai_ErrorEffect) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::ReLoad() return %0:s", 
			ClMIT_Msg::EXIT_FORMAT) << SwOk << SEND_MSG ;
	} ;
	#endif

	return SwOk ;
}

void ClENG_ReportArgs::GetInitString(ClMIT_Str *ai_poFixedPar, ClMIT_Str *ai_poVblePar) 
{
	m4int16_t B, UIndex, FIndex ;
	ClMIT_Str *Str, ArgSep ;

	GetArgSep(&ArgSep) ;
	
	Str=ai_poFixedPar ; FIndex=0; UIndex=OPTIONAL_ARGS_BEG-1 ;

	Str->StrCpy("") ;
	for (B=FIndex;B<=UIndex;++B) {
		Str->StrInsertChar(ClMIT_Str::STR_CAT,1,ClMIT_Args::GetDefaultOpenChar()) ;
		Str->StrInsertChar(ClMIT_Str::STR_CAT,1,ClMIT_Args::GetDefaultOpenChar()) ;
		Str->StrCat(GetArg(B)) ;
		Str->StrInsertChar(ClMIT_Str::STR_CAT,1,ClMIT_Args::GetDefaultCloseChar()) ;
		Str->StrInsertChar(ClMIT_Str::STR_CAT,1,ClMIT_Args::GetDefaultCloseChar()) ;
		if (B!=UIndex) {
			Str->StrInsertChar(ClMIT_Str::STR_CAT,1,*ArgSep.InChar(0)) ;
		} ;
	} ;

	Str=ai_poVblePar ; FIndex=OPTIONAL_ARGS_BEG ; UIndex=GetArgCount()-1 ;

	Str->StrCpy("") ;
	for (B=FIndex;B<=UIndex;++B) {
		Str->StrInsertChar(ClMIT_Str::STR_CAT,1,ClMIT_Args::GetDefaultOpenChar()) ;
		Str->StrInsertChar(ClMIT_Str::STR_CAT,1,ClMIT_Args::GetDefaultOpenChar()) ;
		Str->StrCat(GetArg(B)) ;
		Str->StrInsertChar(ClMIT_Str::STR_CAT,1,ClMIT_Args::GetDefaultCloseChar()) ;
		Str->StrInsertChar(ClMIT_Str::STR_CAT,1,ClMIT_Args::GetDefaultCloseChar()) ;
		if (B!=UIndex) {
			Str->StrInsertChar(ClMIT_Str::STR_CAT,1,*ArgSep.InChar(0)) ;
		} ;
	} ;
} 

// Lo alocado lo desalocan los destructores . Es una extension de los Init, no puede 
//	reusarse varias veces sin haber hecho un Init-End.

m4bool_t ClENG_ReportArgs::Load(ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	const m4char_t *		Beg ;
	size_t					Len ;
	m4int16_t				Index;
	ClMIT_Msg::MsgGroup_t	GroupId ;
	PACK_CHANNELS			PackChannel ;
	m4bool_t				SwOK=M4_TRUE ;
	m4bool_t				MessageShown=M4_FALSE;
	ClMIT_Msg::MSG_LEVEL	Detail, ThisDetail ;	
	ClMIT_Str				StrAux ;
	ClMIT_Str				StrAux2 ;
	StENG_TaskEnv *			TaskEnv = NULL;
	ClChannelManager *		poChannelManager = NULL;
	ClVMBaseEnv *			poEnvironment = NULL;
	m4pcchar_t				pcJavaHome = NULL;
	m4bool_t				bCheckOutputDir = M4_FALSE;
	ClMIT_Str				oOutputDir;
	
	#ifdef RPT_ENGINE_TRACE

	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
	
		m4int16_t B ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::Load() Call with this parameters", 
			ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;

		for (B=0; B < GetArgCount(); ++B ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, GetArg(B) ) << SEND_MSG ;
		} ;

		// Se salva para luego realizar la salida en el mismo contexto que la entrada, y dejar 
		//	las indentaciones consistentes. Esto se hace porque esta funcion puede cambiar el 
		//	detalle de su propio grupo.

		ThisDetail=m_poInsp->m_poTrace->GetGroupDetail(ENG_TGI_CLREPARG) ;
	};

	#endif

	if ( IsValidSubArg(ID_REPORT,0,"ID_REPORT") ) {
		GetString(ID_REPORT,0,&m_oReportId) ;
	} else 	{
		SwOK=M4_FALSE ;	goto exit ;
	} ;

	if ( IsValidSubArg(EXECUTION_TYPE,0,"EXECUTION_TYPE") ) {
		m_iExecutionType= (m4int16_t) GetLong(EXECUTION_TYPE,0) ;
	} else {
		SwOK=M4_FALSE ;	goto exit ;
	};

	if ( IsValidSubArg(DVC_TYPE,0,"DVC_TYPE") ) {
		m_Dvc = (ClENG_ReportRequest::DVC_TYPE)GetEnum(DVC_TYPE,0,m_EnumDvc,&SwOK) ;
		if ( !SwOK ) goto exit ;
	} else {
		SwOK=M4_FALSE ;	goto exit ;
	} ;

	// Unificacion de PDF y PDF Avanzado
	if (m_Dvc == ClENG_ReportRequest::DVC_PDF || m_Dvc == ClENG_ReportRequest::DVC_PDF_UNICODE)
	{
		TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
		if (TaskEnv != NULL && TaskEnv->m_APIChn.m_poMVC != NULL)
		{
			poChannelManager = TaskEnv->m_APIChn.m_poMVC->GetChannelManager();

			if (poChannelManager != NULL )
			{
				poEnvironment = poChannelManager->GetEnvironment();

				if (poEnvironment != NULL )
				{
					pcJavaHome = poEnvironment -> GetJavaHome();
				}
			}
		}

		// Si hay Java se ejecuta PDF_UNICODE. En otro caso se mantiene el PDF normal
		if (pcJavaHome != NULL && strlen(pcJavaHome) > 0)
		{
			m_Dvc = ClENG_ReportRequest::DVC_PDF_UNICODE;
		}
		else
		{
			m_Dvc = ClENG_ReportRequest::DVC_PDF;
		}
	}

	if ( GetArgCount() > OPTIONAL_ARGS_BEG )  {
		
		for ( Index=OPTIONAL_ARGS_BEG; Index < GetArgCount(); ++Index ) {
			
			GetString(Index,0,&Beg, &Len) ; 
			
			if ( strnicmp( Beg,"/PATH",Len)==0 && strlen("/PATH") == Len ) {
				
				// Tratamiento /PATH
				
				if ( !IsValidSubArg(Index,1,"/PATH:Path") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;
				
				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP 
				&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) 
				{
				m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
				"ClENG_ReportArgs::Load()", NULL, Beg ) ;
				SwOK=M4_FALSE ;
				goto exit ;
				} ;
				*/
				
				GetString(Index,1,&Beg, &Len) ;
					
				//NOTA: copy sin contar Len para que un c:\path trague como un argumento
				m_FileParams.m_oPath.StrCpy(Beg) ;

				ClMIT_File::ConvertToThisPlatform( &m_FileParams.m_oPath ) ;
				m_FileParams.m_bIsFile = M4_FALSE ;

				m_FileParams.m_bPathLoaded = m_FileParams.m_bIsFileLoaded = M4_TRUE ;
				
			} else if ( strnicmp( Beg,"/FILE",Len)==0 && strlen("/FILE") == Len ) {

				// Tratamiento /FILE

				if ( !IsValidSubArg(Index,1,"/FILE:Path") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP 
					&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) 
				{
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/
	
				GetString(Index,1,&Beg, &Len) ;
				//NOTA: hara un copy sin contar Len para que un c:\path trague como un argumento
				m_FileParams.m_oPath.StrCpy(Beg) ;
				ClMIT_File::ConvertToThisPlatform( &m_FileParams.m_oPath ) ;
				m_FileParams.m_bIsFile = M4_TRUE ;

				m_FileParams.m_bPathLoaded = m_FileParams.m_bIsFileLoaded = M4_TRUE ;

			} else if ( strnicmp( Beg,"/WEB",Len)==0 && strlen("/WEB") == Len ) {

				// Tratamiento /WEB

				if ( !IsValidSubArg(Index,1,"/WEB:Path") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				
				GetString(Index,1,&Beg, &Len) ;
				
				m_FileParams.m_oWebPath.StrCpy(Beg);
				ClMIT_File::ConvertToThisPlatform( &m_FileParams.m_oWebPath ) ;
				m_FileParams.m_bWebPathLoaded = M4_TRUE ;

			} else if ( strnicmp( Beg,"/HTML_MENU_FILE",Len)==0 && strlen("/HTML_MENU_FILE") == Len ) {

				// Tratamiento /HTML_MENU_FILE

				if ( !IsValidSubArg(Index,1,"/HTML_MENU_FILE:Path") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				
				GetString(Index,1,&Beg, &Len) ;
				
				m_FileParams.m_oHTMLMenuFile.StrCpy(Beg);
				ClMIT_File::ConvertToThisPlatform( &m_FileParams.m_oHTMLMenuFile ) ;
				m_FileParams.m_bHTMLMenuFileLoaded = M4_TRUE ;

			} else if ( strnicmp( Beg,"/NDELBREAK",Len)==0 && strlen("/NDELBREAK") == Len ) {

				// Tratamiento /NDELBREAK

				m_FileParams.m_bUseDelBreak=M4_FALSE ; m_FileParams.m_bUseDelBreakLoaded=M4_TRUE ;


			} else if ( 
				(strnicmp( Beg,"/TOC",Len)==0 && strlen("/TOC") == Len) ||
				(strnicmp( Beg,"/NTOC",Len)==0 && strlen("/NTOC") == Len)) 
			{

				//Tratamiento /TOC

				m_bTableOfContents=strnicmp( Beg,"/TOC",Len)==0 ;

			} else if ( 
				(strnicmp( Beg,"/ZOOM",Len)==0 && strlen("/ZOOM") == Len) ||
				(strnicmp( Beg,"/NZOOM",Len)==0 && strlen("/NZOOM") == Len)) 
			{

				//Tratamiento /ZOOM

				m_bHTMLZoom=strnicmp( Beg,"/ZOOM",Len)==0 ;

			} else if ( 
				(strnicmp( Beg,"/SEARCH",Len)==0 && strlen("/SEARCH") == Len) ||
				(strnicmp( Beg,"/NSEARCH",Len)==0 && strlen("/NSEARCH") == Len)) 
			{

				//Tratamiento /SEARCH

				m_bHTMLSearch=strnicmp( Beg,"/SEARCH",Len)==0 ;

			} else if ( strnicmp( Beg,"/DELBREAK",Len)==0 && strlen("/DELBREAK") == Len ) {

				// Tratamiento /DELBREAK

				m_FileParams.m_bUseDelBreak = M4_TRUE ; m_FileParams.m_bUseDelBreakLoaded=M4_TRUE ;

				switch ( GetSubArgCount(Index) ) {

				case 1:
					break;

				case 2:

					m_FileParams.m_bDefaultDelBreak = M4_BOOL( GetLong(Index,1) != 0 ); 
					m_FileParams.m_bDefaultDelBreakLoaded=M4_TRUE ;
					break;

				case 3:

					m_FileParams.m_bDefaultDelBreak = M4_BOOL( GetLong(Index,1) !=0 ); 
					m_FileParams.m_bDefaultDelBreakLoaded=M4_TRUE ;

					m_FileParams.m_bConsecutiveBreaks = M4_BOOL( GetLong(Index,2) != 0 ); 
					m_FileParams.m_bConsecutiveBreaksLoaded=M4_TRUE ;

					break;

				default:

					IsValidSubArg(Index,GetSubArgCount(Index)+2,"/DELBREAK:DefaultBreakFlag:ConsecutiveFlag") ;
					SwOK=M4_FALSE ;	
					goto exit ;
					break ;

				} ;

			
			} else if ( strnicmp( Beg,"/EXT",Len)==0 && strlen("/EXT") == Len ) {

				// Tratamiento /EXT

				if ( !IsValidSubArg(Index,1,"/EXT:FileExt") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP 
				&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) 
				{
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				GetString(Index,1,&Beg, &Len) ;
				m_FileParams.m_oExtension.StrCpy(Beg) ;					

				m_FileParams.m_bExtensionLoaded=M4_TRUE ;

			} else if ( strnicmp( Beg,"/SEP",Len)==0 && strlen("/SEP") == Len ) {

				// Tratamiento /SEP

				if ( !IsValidSubArg(Index,1,"/SEP:SepChar") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP  
				&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_FileParams.m_cSeparator = (m4int16_t)GetLong(Index,1) ;					
				m_FileParams.m_bSeparatorLoaded=M4_TRUE ;


			} else if ( strnicmp( Beg,"/NUMSECCHARS",Len)==0 && strlen("/NUMSECCHARS") == Len ) {

				// Tratamiento /NUMSECCHARS

				if ( !IsValidSubArg(Index,1,"/NUMSECCHARS:NSecChars") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP  
					&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) 
				{
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_FileParams.m_iNumOfSecChars=(m4int16_t)GetLong(Index,1) ;
				m_FileParams.m_bNumOfSecCharsLoaded=M4_TRUE ;

			} else if ( strnicmp( Beg,"/NUMPAGECHARS",Len)==0 && strlen("/NUMPAGECHARS") == Len ) {

				// Tratamiento /NUMPAGECHARS

				if ( !IsValidSubArg(Index,1,"/NUMPAGECHARS:NPageChars") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP  
				&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_FileParams.m_iNumOfPageChars=(m4int16_t)GetLong(Index,1) ;
				m_FileParams.m_bNumOfPageCharsLoaded=M4_TRUE ;
			
			} else if ( strnicmp( Beg,"/FILEOFF",Len)==0 && strlen("/FILEOFF") == Len ) {

				// Tratamiento /FILEOFF

				if ( !IsValidSubArg(Index,1,"/FILEOFF:FileOffset") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP  
				&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_FileParams.m_iOffSet=(m4int16_t)GetLong(Index,1) ;
				m_FileParams.m_bOffSetLoaded=M4_TRUE ;


			} else if ( strnicmp( Beg,"/TOPRINTER",Len)==0 && strlen("/TOPRINTER") == Len ) {

				// Tratamiento /TOPRINTER

				if ( !IsValidSubArg(Index,1,"/TOPRINTER:PrinterName") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP 
					&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) 
				{
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/
	
				GetString(Index,1,&Beg, &Len) ;
				m_oToPrinter.StrCpy(Beg) ;

				// Bug 0142169. No se cambian las barras de las impresoras
				// porque si tenemos una impresora instalada mediante una URL
				// no funciona al cambiar ""\\http://" por "\\http:\\".
				// ClMIT_File::ConvertToThisPlatform( &m_oToPrinter ) ;

			} else if ( strnicmp( Beg,"/BIN",Len)==0 && strlen("/BIN") == Len ) {

				// Tratamiento /BIN

				if ( !IsValidSubArg(Index,1,"/BIN:Bin") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				GetString(Index,1,&Beg, &Len) ;
				if ( (*Beg=='D'|| *Beg=='d' ) && Len==1 ) {
					m_BinDefault=(ClENG_PagePrinter::PAPER_SOURCE)
						GetEnum(Index,2,m_EnumBin, &SwOK) ;
					if (!SwOK) goto exit ;
				} ;
			}
			else if (strnicmp(Beg, "/BINPCL", Len) ==0 && strlen("/BINPCL") == Len)
			{
				// Tratamiento /BINPCL
				if (!IsValidSubArg(Index, 1, "/BINPCL:BinPcl")) 
				{
					SwOK = M4_FALSE;
					goto exit;
				}

				GetString(Index, 1, &Beg, &Len);
				if ((*Beg == 'D'|| *Beg == 'd') && Len == 1)
				{
					m_BinPcl = GetLong(Index, 2);
				}
			} else if ( strnicmp( Beg,"/COPIES",Len)==0 && strlen("/COPIES") == Len ) {

				// Tratamiento /COPIES

				if ( !IsValidSubArg(Index,1,"/COPIES:NCopies") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_iCopies=(m4int16_t)GetLong(Index,1) ;

			} else if ( strnicmp( Beg,"/FILLCHAR",Len)==0 && strlen("/FILLCHAR") == Len ) {

				// Tratamiento /FILLCHAR

				if ( GetSubArgCount(Index)==1 ) {

					m_cFillChar=ClENG_DvcTxt::NO_FILL_CHAR ;

				} else {
					if ( !IsValidSubArg(Index,1,"/FILLCHAR:ASCIICode") ) {
						SwOK=M4_FALSE ;	goto exit ;
					} ;
					m_cFillChar=(m4uint8_t)GetLong(Index,1) ;
				} ;


			} else if ( 
				(strnicmp( Beg,"/EOLSEQ",Len)==0 && strlen("/EOLSEQ") == Len) ||
				(strnicmp( Beg,"/EOPSEQ",Len)==0 && strlen("/EOPSEQ") == Len) ) 
			{

				// Tratamiento /EOLSEQ y /EOPSEQ

				ClMIT_Str *Str ;
				m4char_t *PChar ;
				m4int16_t BAux ;

				Str = strnicmp( Beg,"/EOLSEQ",Len)==0 ? &m_oEOLSeq : &m_oEOPSeq ; 
				Str->StrCpy("") ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_TXT ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/
				
				Str->Accommodate(GetSubArgCount(Index)+1) ;	//Sobra algun char, pero, que mas da?
				PChar=Str->InChar(0) ;
				for (BAux=1; BAux<GetSubArgCount(Index);++BAux, ++PChar) {
					*PChar= (m4uint8_t)GetLong(Index,BAux) ;
				} ;
				*PChar= M4_END_STR ;

			} else if ( strnicmp( Beg,"/DUPLEX",Len)==0 && strlen("/DUPLEX") == Len ) {

				// Tratamiento /DUPLEX

				if ( !IsValidSubArg(Index,1,"/DUPLEX:OFF/EMULATED/REAL") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				m_Duplex=(StENG_DvcParams::DUPLEX_MODE)GetEnum(Index,1,m_EnumDuplex, &SwOK) ;
				if (!SwOK) goto exit ;

			} else if ( strnicmp( Beg,"/TUMBLE",Len)==0 && strlen("/TUMBLE") == Len ) {

				// Tratamiento /DUPLEX

				if ( !IsValidSubArg(Index,1,"/TUMBLE:OFF/FALSE/TRUE") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				
				m_Tumble=(StENG_DvcParams::TUMBLE_MODE)GetEnum(Index,1,m_EnumTumble, &SwOK) ;
				if (!SwOK) goto exit ;

			} else if ( strnicmp( Beg,"/HTML_NAVIGATION",Len)==0 && strlen("/HTML_NAVIGATION") == Len ) {

				// Tratamiento /HTML_NAVIGATION

				if ( !IsValidSubArg(Index,1,"/HTML_NAVIGATION:OFF/ACTIVE") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				m_HTMLNavigation=(ClENG_DvcHTML::HTML_NAVIGATION)GetEnum(Index,1,m_EnumHTMLNavigation, &SwOK) ;
				if (!SwOK) goto exit ;

			
			} else if ( strnicmp( Beg,"/HTML_MENU",Len)==0 && strlen("/HTML_MENU") == Len ) {

				// Tratamiento /HTML_MENU

				if ( !IsValidSubArg(Index,1,"/HTML_MENU:BOTTOM/TOP") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				m_HTMLMenu=(ClENG_DvcHTML::HTML_MENU)GetEnum(Index,1,m_EnumHTMLMenu, &SwOK) ;
				if (!SwOK) goto exit ;

			
			} else if ( 
				(strnicmp( Beg,"/CREATE_DIR",Len)==0 && strlen("/CREATE_DIR") == Len) ||
				(strnicmp( Beg,"/NCREATE_DIR",Len)==0 && strlen("/NCREATE_DIR") == Len)) 
			{

				//Tratamiento /CREATE_DIR

				m_bCreateDir=strnicmp( Beg,"/CREATE_DIR",Len)==0 ;

			} else if (	strnicmp( Beg,"/PRESERVE",Len)==0 && strlen("/PRESERVE") == Len ) { 

				m_Preserve = ClENG_ReportOuputRepository::PRESERVE_FIX_NAME ;

			} else if (	strnicmp( Beg,"/NPRESERVE",Len)==0 && strlen("/NPRESERVE") == Len ) { 

				m_Preserve = ClENG_ReportOuputRepository::NO_PRESERVE ;

			} else if (	strnicmp( Beg,"/PRESERVE_AUTO",Len)==0 && strlen("/PRESERVE_AUTO") == Len ) { 

				m_Preserve = ClENG_ReportOuputRepository::PRESERVE_MOVE_NAME ;

			} else if (	strnicmp( Beg,"/PRESERVE_DIR",Len)==0 && strlen("/PRESERVE_DIR") == Len ) { 

				m_Preserve = ClENG_ReportOuputRepository::PRESERVE_DIR ;

			} else if ( 
				(strnicmp( Beg,"/DELETE",Len)==0 && strlen("/DELETE") == Len) ||
				(strnicmp( Beg,"/NDELETE",Len)==0 && strlen("/NDELETE") == Len)) 
			{

				// Tratamiento /DELETE

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP 
					&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) 
				{
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_bDelete=M4_BOOL(strnicmp( Beg,"/DELETE",Len)==0);

			} else if ( 
				(strnicmp( Beg,"/CLIPCOLUMN",Len)==0 && strlen("/CLIPCOLUMN") == Len) ||
				(strnicmp( Beg,"/NCLIPCOLUMN",Len)==0 && strlen("/NCLIPCOLUMN") == Len)) 
			{

				// Tratamiento /CLIPCOLUMN

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL && m_Dvc != ClENG_ReportRequest::DVC_ESCP 
					&& m_Dvc != ClENG_ReportRequest::DVC_TXT && m_Dvc != ClENG_ReportRequest::DVC_RVW ) 
				{
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_bClipColumn=M4_BOOL(strnicmp( Beg,"/CLIPCOLUMN",Len)==0);

			} else if ( strnicmp( Beg,"/DPI",Len)==0 && strlen("/DPI") == Len ) {

				// Tratamiento /DPI

				if ( !IsValidSubArg(Index,1,"/DPI:Resolution") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_PCL ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/
				m_Resolution=(ClENG_DvcPCL::PAGE_RESOLUTION)
						GetEnum(Index,1,m_EnumDPI, &SwOK) ;
				m_PSCResolution=(ClENG_DvcPSC::PAGE_RESOLUTION)
						GetEnum(Index,1,m_EnumDPI, &SwOK) ;
				if (!SwOK) goto exit ;

			} else if ( strnicmp( Beg,"/XFACTOR",Len)==0 && strlen("/XFACTOR") == Len ) {

				// Tratamiento /XFACTOR

				if ( !IsValidSubArg(Index,1,"/XFACTOR:XFactor") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_ESCP ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_iXFactor=(m4int16_t)GetLong(Index,1) ;

			} else if ( strnicmp( Beg,"/YFACTOR",Len)==0 && strlen("/YFACTOR") == Len ) {

				// Tratamiento /YFACTOR

				if ( !IsValidSubArg(Index,1,"/YFACTOR:YFactor") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_ESCP ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_iYFactor=(m4int16_t)GetLong(Index,1) ;

			} else if ( strnicmp( Beg,"/JPAGESIZE",Len)==0 && strlen("/JPAGESIZE") == Len ) {

				if ( !IsValidSubArg(Index,1,"/JPAGESIZE:PageSize:PageSize:...") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				GetString(Index,1,&Beg, &Len) ;
				
				m_JPageSize.StrCpy(Beg);
		
			
			} else if ( strnicmp( Beg,"/FONT",Len)==0 && strlen("/FONT") == Len ) {

				// Tratamiento /FONT

				if ( !IsValidSubArg(Index,1,"/FONT:IdFont") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_TXT ) {
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_uiIdFont=(m4int16_t)GetLong(Index,1) ;

			} else if ( 
				(strnicmp( Beg,"/REPRINTER",Len)==0 && strlen("/REPRINTER") == Len) ||
				(strnicmp( Beg,"/NREPRINTER",Len)==0 && strlen("/NREPRINTER") == Len) ) 
			{

				// Tratamiento /REPRINTER

				m_bRePrinter=M4_BOOL(strnicmp( Beg,"/REPRINTER",Len)==0);

			} else if ( strnicmp( Beg,"/DEBUG",Len)==0 && strlen("/DEBUG") == Len ) {

				// Tratamiento /DEBUG

				if ( !IsValidSubArg(Index,2,"/DEBUG:Group:DetailLevel") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				GroupId = (ClMIT_Msg::MsgGroup_t)GetEnum(Index,1,m_EnumGDebug, &SwOK) ;
				if (!SwOK) goto exit ;

				Detail= (ClMIT_Msg::MSG_LEVEL)GetEnum(Index,2,m_EnumDetail, &SwOK);
				if (!SwOK) goto exit ;

				if ( GroupId != ENG_TGI_ALL_GROUP ) {
					m_poInsp->m_poTrace->SetGroupDetail( GroupId,Detail) ;
				} else {
					m_poInsp->m_poTrace->SetGroupDetail( 0, ClMIT_Trace::GROUP_COUNT-1, Detail ) ;
				} ;

			} else if ( strnicmp( Beg,"/BREAK",Len)==0 && strlen("/BREAK") == Len ) {

				// Tratamiento /BREAK

				m_Break = (ClENG_Report::BREAK_MODE)GetEnum(Index,1,m_EnumBreak, &SwOK);

				if (!SwOK) goto exit ;

				if ( (m4int16_t)m_Break==-1 ) {
					m_bLoadBreakDesign=M4_TRUE ;
				} else {
					m_bLoadBreakDesign=M4_FALSE ;
					if ( IsValidSubArg(Index,2,"/BREAK:Mode:Section") ) {
						m_iBreakSec=(m4int16_t)GetLong(Index,2) ;
					} else {
						SwOK=M4_FALSE ;	goto exit ;
					} ;
				} ;

			} else if ( 
				(strnicmp( Beg,"/FLUSH_SBS",Len)==0 && strlen("/FLUSH_SBS") == Len) ||
				(strnicmp( Beg,"/NFLUSH_SBS",Len)==0 && strlen("/NFLUSH_SBS") == Len) ) 
			{

				m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].Switch( ClMIT_Trace::FLUSH_SBS, 
					M4_BOOL(strnicmp( Beg,"/FLUSH_SBS",Len)==0) ) ;
				m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::FE_OUT].Switch( ClMIT_Trace::FLUSH_SBS, 
					M4_BOOL(strnicmp( Beg,"/FLUSH_SBS",Len)==0) ) ;

			} else if ( 
				(strnicmp( Beg,"/EOPSEP",Len)==0 && strlen("/EOPSEP") == Len) ||
				(strnicmp( Beg,"/NEOPSEP",Len)==0 && strlen("/NEOPSEP") == Len) ) 
			{

				// Tratamiento /EOPSEP

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_HTML ) 
				{
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_bSwEOPSep=M4_BOOL(strnicmp( Beg,"/EOPSEP",Len)==0);

			} else if ( 
				(strnicmp( Beg,"/TESTMODE",Len)==0 && strlen("/TESTMODE") == Len ) ||
				(strnicmp( Beg,"/NTESTMODE",Len)==0 && strlen("/NTESTMODE") == Len ) ) 
			{

				// Tratamiento /TESTMODE

				if ( strnicmp( Beg,"/NTESTMODE",Len)==0 ) {

					m_bTestField = m_bTestCnt = m_bTestCell = M4_FALSE ;
				
				} else {
				
					if ( !IsValidSubArg(Index,1) ) {
	
						m_bTestField = m_bTestCnt = M4_TRUE ;
						m_bTestCell = M4_FALSE ;
						
					} else {

						m_bTestField = m_bTestCnt = m_bTestCell = M4_FALSE ;

						GetString(Index,1,&Beg, &Len);

						if ( strchr( Beg, 'C' ) || strchr( Beg, 'c' ) ) {
							m_bTestCnt = M4_TRUE ;
						} ;

						if ( strchr( Beg, 'F' ) || strchr( Beg, 'f' ) ) {
							m_bTestField = M4_TRUE ;
						} ;

						if ( strchr( Beg, 'L' ) || strchr( Beg, 'l' ) ) {
							m_bTestCell = M4_TRUE ;
						} ;

					} ;

				} ;

			} else if ( 
				(strnicmp( Beg,"/ONLYDESIGN",Len)==0 && strlen("/ONLYDESIGN") == Len ) ||
				(strnicmp( Beg,"/NONLYDESIGN",Len)==0 && strlen("/NONLYDESIGN") == Len ) ) 
			{

				// Tratamiento /ONLYDESIGN

				m_bTestOnlyDesign=M4_BOOL(strnicmp( Beg,"/ONLYDESIGN",Len)==0);

			} else if ( 
				(strnicmp( Beg,"/PAGEFILL",Len)==0 && strlen("/PAGEFILL") == Len) ||
				(strnicmp( Beg,"/NPAGEFILL",Len)==0 && strlen("/NPAGEFILL") == Len)) 
			{

				// Tratamiento /PAGEFILL

				/*
				if ( m_Dvc != ClENG_ReportRequest::DVC_HTML && m_Dvc != ClENG_ReportRequest::DVC_TXT ) 
				{
					m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ CLREPARG_MSG_10 ], 
						"ClENG_ReportArgs::Load()", NULL, Beg ) ;
					SwOK=M4_FALSE ;
					goto exit ;
				} ;
				*/

				m_bHTMLSwPageFill=M4_BOOL(strnicmp( Beg,"/PAGEFILL",Len)==0);
				m_bTXTSwPageFill=m_bHTMLSwPageFill ;

			} else if ( strnicmp( Beg,"/PACK",Len)==0 && strlen("/PACK") == Len ) {

				// Tratamiento /PACK

				if ( !IsValidSubArg(Index,1,"/PACK:DATA or DESIGN") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				PackChannel = (PACK_CHANNELS)GetEnum(Index,1,m_EnumPack, &SwOK) ;
				if (!SwOK) goto exit ;

				if ( PackChannel != PACK_DESIGN ) {
					m_bPackDesign=M4_TRUE ;
				} else if ( PackChannel != PACK_DATA ) {
					m_bPackData=M4_TRUE ;
				} ;

			} else if ( strnicmp( Beg,"/AUTOLOAD",Len)==0 && strlen("/AUTOLOAD") == Len ) {

				if ( !IsValidSubArg(Index,2,"/AUTOLOAD:DATA or DESIGN:0 or 1") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				PackChannel = (PACK_CHANNELS)GetEnum(Index,1,m_EnumPack, &SwOK) ;
				if (!SwOK) goto exit ;

				if ( PackChannel == PACK_DESIGN ) {
					m_DesignAutoload=(ClMIT_Chn::AUTOLOAD_MODE)GetEnum(Index,2,m_EnumAutoload, &SwOK) ;
				} else if ( PackChannel == PACK_DATA ) {
					m_DataAutoload=(ClMIT_Chn::AUTOLOAD_MODE)GetEnum(Index,2,m_EnumAutoload, &SwOK) ;
				} ;
				if (!SwOK) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

			} else if ( strnicmp( Beg,"/DESTCUR",Len)==0 && strlen("/DESTCUR") == Len ) {

				// Tratamiento /DESTCUR

				if ( !IsValidSubArg(Index,1,"/DESTCUR:IdCurrency") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				GetString(Index,1,&Beg, &Len);
				m_oDestCurrency.StrCpy(Beg);

			} else if ( strnicmp( Beg,"/EXCHDATE",Len)==0 && strlen("/EXCHDATE") == Len ) {

				// Tratamiento /EXCHDATE

				if ( !IsValidSubArg(Index,1,"/EXCHDATE:Date") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				GetString(Index,1,&Beg,&Len) ;
				m_dExchangeDate= ClMIT_Dates::StringToDate(Beg) ;

			} else if ( strnicmp( Beg,"/EXCHTYPE",Len)==0 && strlen("/EXCHTYPE") == Len ) {

				// Tratamiento /DESTCUR

				if ( !IsValidSubArg(Index,1,"/EXCHTYPE:Type") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				GetString(Index,1,&Beg, &Len);
				m_oExchangeType.StrCpy(Beg);
			
			} else if ( strnicmp( Beg,"/TWRAP",Len)==0 && strlen("/TWRAP") == Len ) {

				// Tratamiento //TWRAP

				if ( !IsValidSubArg(Index,1,"/TWRAP:NumColumns (0 desactivate)") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;
				m_poInsp->m_poTrace->SetWrap((m4int16_t)GetLong(Index,1)) ;

			} else if ( 
				(strnicmp( Beg,"/INDENT",Len)==0 && strlen("/INDENT") == Len )|| 
				(strnicmp( Beg,"/NINDENT",Len)==0 && strlen("/NINDENT") == Len ) ) 
			{

				// Tratamiento /INDENT

				m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].Switch( ClMIT_Trace::INDENT, 
					M4_BOOL(strnicmp( Beg,"/INDENT",Len)==0 ));
				m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::FE_OUT].Switch( ClMIT_Trace::INDENT, 
					M4_BOOL(strnicmp( Beg,"/INDENT",Len)==0 ));

			} else if ( strnicmp( Beg,"/MOVETRACE",Len)==0 && strlen("/MOVETRACE") == Len ) {

				// Tratamiento /MOVETRACE

				if ( !IsValidSubArg(Index,3,"/MOVETRACE:ALWAYS/ON_SUCCESS/ON_PROBLEM/ON_ERROR:DATE/NDATE:Path") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				m_MoveTraceMode = (ClMIT_Trace::MOVE_TRACE_MODES)GetEnum(Index,1,m_EnumMoveTraceMode, &SwOK) ;
				if (!SwOK) goto exit ;

				GetString(Index,2,&Beg, &Len); m_oMoveTraceFile.ToASCIZ(Beg,Len);
				if ( stricmp( m_oMoveTraceFile,"DATE")==0 ) {
					m_bSwDateTraceMove=M4_TRUE ;
				} else if ( stricmp( m_oMoveTraceFile,"NDATE")==0 ) {
					m_bSwDateTraceMove=M4_FALSE ;
				} else {
					SwOK=M4_FALSE ;	goto exit ;
				} 

				//NOTA: copy sin contar Len para que un c:\path trague como un argumento
				GetString(Index,3,&Beg, &Len); m_oMoveTraceFile.StrCpy(Beg);

			} else if ( 
				(strnicmp( Beg,"/TIMEEXP",Len)==0 && strlen("/TIMEEXP") == Len) ||
				(strnicmp( Beg,"/NTIMEEXP",Len)==0 && strlen("/NTIMEEXP") == Len)) 
			{

				//Tratamiento /TIMEEXP

				m_bSwTimeExp=M4_BOOL(strnicmp( Beg,"/TIMEEXP",Len)==0);

			} else if ( 
				(strnicmp( Beg,"/MEMEXP",Len)==0 && strlen("/MEMEXP") == Len) ||
				(strnicmp( Beg,"/NMEMEXP",Len)==0 && strlen("/NMEMEXP") == Len)) 
			{

				//Tratamiento MEMEXP
				
				m_poInsp->m_poMemProf->SetCheckPointAtEnd(M4_BOOL(strnicmp( Beg,"/MEMEXP",Len)==0));

			} else if ( strnicmp( Beg,"/ID_DESC",Len)==0 && strlen("/ID_DESC") == Len ) {

				// Tratamiento /ID_DESC

				m4int16_t IdDesc ;

				if ( !IsValidSubArg(Index,1,"/ID_DESC:ID/DESC/ID_DESC") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				IdDesc = GetEnum(Index,1,m_EnumIdDesc, &SwOK) ;
				if (!SwOK) goto exit ;

				switch (IdDesc) {
				case 0:
					m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].On(ClMIT_Trace::ID_OUT) ;
					m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].Off(ClMIT_Trace::DESC_OUT) ;
					break;
				case 1:
					m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].Off(ClMIT_Trace::ID_OUT) ;
					m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].On(ClMIT_Trace::DESC_OUT) ;
					break;
				case 2:
					m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].On(
						ClMIT_Trace::ID_OUT|ClMIT_Trace::DESC_OUT) ;
					break;
				} ;

			
			} else if ( 
				(strnicmp( Beg,"/UPDATE_DEPENDENCES",Len)==0 && strlen("/UPDATE_DEPENDENCES") == Len) ||
				(strnicmp( Beg,"/NUPDATE_DEPENDENCES",Len)==0 && strlen("/NUPDATE_DEPENDENCES") == Len)) 
			{

				//Tratamiento /UPDATE_DEPENDENCES

				m_bUpdateDependencies=M4_BOOL(strnicmp( Beg,"/UPDATE_DEPENDENCES",Len)==0);

			} else if ( 
				(strnicmp( Beg,"/REBUILD_DEPENDENCES",Len)==0 && strlen("/REBUILD_DEPENDENCES") == Len) ||
				(strnicmp( Beg,"/NREBUILD_DEPENDENCES",Len)==0 && strlen("/NREBUILD_DEPENDENCES") == Len)) 
			{

				//Tratamiento /REBUILD_DEPENDENCES

				m_bRebuildDependencies=M4_BOOL(strnicmp( Beg,"/REBUILD_DEPENDENCES",Len)==0);

			} else if ( 
				(strnicmp( Beg,"/EXTRACT_STRINGS",Len)==0 && strlen("/EXTRACT_STRINGS") == Len) ||
				(strnicmp( Beg,"/NEXTRACT_STRINGS",Len)==0 && strlen("/NEXTRACT_STRINGS") == Len)) 
			{

				//Tratamiento /EXTRACT_STRINGS

				m_bExtractStrings=M4_BOOL(strnicmp( Beg,"/EXTRACT_STRINGS",Len)==0);

			} else if ( 
				(strnicmp( Beg,"/SILENT_CANCEL",Len)==0 && strlen("/SILENT_CANCEL") == Len) ||
				(strnicmp( Beg,"/NSILENT_CANCEL",Len)==0 && strlen("/NSILENT_CANCEL") == Len) ) 
			{

				m_poInsp->m_poTrace->SetSilentCancel( M4_BOOL(strnicmp( Beg,"/SILENT_CANCEL",Len)==0) ) ;

			} else if ( strnicmp( Beg,"/DEBUGMODE",Len)==0 && strlen("/DEBUGMODE") == Len ) {

				m_poInsp->m_poTrace->SetGroupDetail( 0, ClMIT_Trace::GROUP_COUNT-1, ClMIT_Msg::CRAZY  ) ;

				m_poInsp->m_poTrace->SetSilentCancel( M4_FALSE ) ;

				m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].
					Switch( ClMIT_Trace::FLUSH_SBS,M4_TRUE) ; 
				m_poInsp->m_poTrace->m_oOutputFlags[ClMIT_Msg::FE_OUT].
					Switch( ClMIT_Trace::FLUSH_SBS,M4_TRUE) ; 

				m_bSwTimeExp=M4_TRUE ;

				m_poInsp->m_poMemProf->SetCheckPointAtEnd(M4_TRUE) ;

			} else if ( 
				(strnicmp( Beg,"/COLOR",Len)==0 && strlen("/COLOR") == Len )|| 
				(strnicmp( Beg,"/NCOLOR",Len)==0 && strlen("/NCOLOR") == Len ) ) 
			{

				m_bSwColour= M4_BOOL(strnicmp( Beg,"/COLOR",Len)==0);

			} else if ( strnicmp( Beg,"/DECLARE_EXTERNAL_FILE",Len)==0 && strlen("/DECLARE_EXTERNAL_FILE") == Len ) {

				// Tratamiento de la declaracion de interés de los ficheros

				if ( !IsValidSubArg(Index,1,"/DECLARE_EXTERNAL_FILE:/TEMPORARY/REFERENCE/COPY/REFERENCE_AND_COPY") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				m_DeclareExtFile=(DECLARE_EXTERNAL_FILE)GetEnum(Index,1,m_EnumDeclareExtFile, &SwOK) ;
				if (!SwOK) goto exit ;

			} else if ( strnicmp( Beg,"/METADATA_LANGUAGE",Len)==0 && strlen("/METADATA_LANGUAGE") == Len ) {

				// Tratamiento /METADATA_LANGUAGE

				if ( !IsValidSubArg(Index,1,"/METADATA_LANGUAGE:Lang") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				m_iMetadataLang=(m4int16_t)GetLong(Index,1) ;

			} else if ( strnicmp( Beg,"/DATA_LANGUAGE",Len)==0 && strlen("/DATA_LANGUAGE") == Len ) {

				// Tratamiento /DATA_LANGUAGE

				if ( !IsValidSubArg(Index,1,"/DATA_LANGUAGE:Lang") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				m_iDataLang=(m4int16_t)GetLong(Index,1) ;

			} else if ( strnicmp( Beg,"/ID_CHANNEL",Len)==0 && strlen("/ID_CHANNEL") == Len ) {

				// Tratamiento /ID_CHANNEL

				if ( !IsValidSubArg(Index,1,"/ID_CHANNEL:ID_Channel") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				GetString(Index,1,&Beg, &Len) ;
				m_oIDChannel.StrCpy(Beg) ;

			} else if ( strnicmp( Beg,"/HTML_TEMPLATE_DATA",Len)==0 && strlen("/HTML_TEMPLATE_DATA") == Len ) {

				// Tratamiento /HTML_TEMPLATE_DATA

				if ( !IsValidSubArg(Index,2,"/HTML_TEMPLATE_DATA:IdStartTemplate:IdEndTemplate") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				GetString(Index,1,&Beg, &Len) ;
				m_oHTML_StartTemplate.ToASCIZ(Beg,Len) ;
				
				GetString(Index,2,&Beg, &Len) ;
				m_oHTML_EndTemplate.ToASCIZ(Beg,Len) ;

			} else if ( strnicmp( Beg,"/HTML_TEMPLATE_MAIN",Len)==0 && strlen("/HTML_TEMPLATE_MAIN") == Len ) {

				// Tratamiento /HTML_TEMPLATE_MAIN

				if ( !IsValidSubArg(Index,1,"/HTML_TEMPLATE_MAIN:IdMainBreakTemplate") ) {
					SwOK=M4_FALSE ;	goto exit ;
				} ;

				GetString(Index,1,&Beg, &Len) ;
				m_oHTML_BreakTemplate.ToASCIZ(Beg,Len) ;
			
			} else if ( 
				(strnicmp( Beg,"/PUBLISH",Len)==0 && strlen("/PUBLISH") == Len) ||
				(strnicmp( Beg,"/NPUBLISH",Len)==0 && strlen("/NPUBLISH") == Len)) 
			{

				//Tratamiento /PUBLISH

				m_bSwPublish = strnicmp( Beg,"/PUBLISH",Len)==0 ;

			} else if (
				(strnicmp (Beg,"/FORCE_UNICODE_OUTPUT", Len ) == 0 && strlen("/FORCE_UNICODE_OUTPUT") ==Len) ||
				(strnicmp (Beg,"/NFORCE_UNICODE_OUTPUT", Len ) == 0 && strlen("/NFORCE_UNICODE_OUTPUT") ==Len))
			{
				
				// Tratamiento /FORCE_UNICODE_OUTPUT
				m_bForceUnicodeOutput = M4_BOOL(strnicmp( Beg,"/FORCE_UNICODE_OUTPUT",Len)==0) ;

			} else if (strnicmp (Beg,"/PREPROCESS_FOR_EXCEL", Len) == 0 && strlen("/PREPROCESS_FOR_EXCEL") == Len) 
			{
				// Tratamiento para preprocesar los campos estándar para Excel (TXT_UNICODE).
				m_bPreprocessForExcel = M4_BOOL(strnicmp( Beg,"/PREPROCESS_FOR_EXCEL", Len)==0) ;
			}
			else if (strnicmp(Beg,"/FIELD_DELIMITER", Len) == 0 && strlen("/FIELD_DELIMITER") == Len)
			{
				// Tratamiento /FIELD_DELIMITER. Separador de los campos generados en un fichero
				// txt. Aplica solo a los TXT_UNICODE.
				if (GetSubArgCount(Index) == 1) 
				{
					 m_cFieldDelimiter = ClENG_DvcTxt::NO_FIELD_DELIMITER_CHAR;
				}
				else
				{
					if (!IsValidSubArg(Index,1,"/FIELD_DELIMITER:ASCIICode"))
					{
						SwOK = M4_FALSE;
						goto exit;
					} 

					m_cFieldDelimiter = (m4uint8_t) GetLong(Index, 1);
				}
			}
			else if ( strnicmp( Beg,"/M4THROW",Len)==0 && strlen("/M4THROW") == Len ) 
			{
				;	//Uso de la M4THROW, simplemente lo absorbemos para que no de errores

			}
			else if (strnicmp (Beg, "/DUPLEX_EMULATION", Len) == 0 && strlen("/DUPLEX_EMULATION") == Len) 
			{
				m_bDuplexEmulation = M4_BOOL(strnicmp(Beg, "/DUPLEX_EMULATION", Len) == 0);
			}
			else if (
				(strnicmp (Beg,"/IGNORE_FILE_PATH_DEF", Len ) == 0 && strlen("/IGNORE_FILE_PATH_DEF") ==Len) ||
				(strnicmp (Beg,"/NIGNORE_FILE_PATH_DEF", Len ) == 0 && strlen("/NIGNORE_FILE_PATH_DEF") ==Len))
			{
				
				// Tratamiento IGNORE_FILE_PATH_DEF
				m_FileParams.m_bIgnoreFilePathPref = M4_BOOL(strnicmp( Beg,"/IGNORE_FILE_PATH_DEF",Len)==0) ;

			}
			else 
			{
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLREPARG_MSG_20 ] ) 
					<< Beg << SEND_MSG ;
				SwOK=M4_FALSE ;
				MessageShown=M4_TRUE;
				goto exit ;
			} ;
		} ;
	} ;

	// Bug 0297428: evitar rutas fuera de temporal de los reports se ejecuta en servidor.
	// Bug 0299670. Esta verificación solo se hace si no se ejecuta en web (cuando no viene el parámetro /WEB)
	if (m_FileParams.m_bPathLoaded == M4_TRUE && m_FileParams.m_bIsFileLoaded == M4_TRUE && m_FileParams.m_bWebPathLoaded == M4_FALSE)
	{
		bCheckOutputDir = CheckOutputDir(m_FileParams.m_oPath, m_FileParams.m_bIsFile, &oOutputDir);
		if (bCheckOutputDir == M4_FALSE)
		{	
			// Bug 0298667. Especificar sólo directorio de salida en el mensaje de error.
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY_EX[ CLREPARG_MSG_50 ] ) << oOutputDir.InChar(0) << SEND_MSG ;
			
			SwOK = M4_FALSE ;
			MessageShown = M4_TRUE;
			goto exit ;
		}
	}

	if ( m_FileParams.m_oPath.StrLen()==0 ) {

		StrAux.Accommodate(40) ;

		switch (m_Dvc) {

		case ClENG_ReportRequest::DVC_PCL:
			StrAux.StrCpy( "PCL_" ) ;
			break;

		case ClENG_ReportRequest::DVC_PSC:
			StrAux.StrCpy( "PSC_" ) ;
			break;

		case ClENG_ReportRequest::DVC_ESCP:
			StrAux.StrCpy("ESCP_" ) ;
			break;

		case ClENG_ReportRequest::DVC_TXT:
		case ClENG_ReportRequest::DVC_TXT_UNICODE:
			StrAux.StrCpy("TXT_" ) ;
			break;

		case ClENG_ReportRequest::DVC_RVW:
			StrAux.StrCpy("RVW_" ) ;
			break;

		case ClENG_ReportRequest::DVC_HTML:
			StrAux.StrCpy("HTML_" ) ;
			break;

		case ClENG_ReportRequest::DVC_PDF:
		case ClENG_ReportRequest::DVC_PDF_UNICODE:
			StrAux.StrCpy("PDF_" ) ;
			break;
		} ;

		ClMIT_Dates::DateToFileString( &StrAux2, ClMIT_Dates::ActualTimeStamp() ) ;
		sprintf(StrAux.EndChar(), "%s_%02u", StrAux2.InChar(0), (unsigned int)(rand() % 100));
		StrAux.OverStringValid() ;
		if ( m_oMoveTraceFile.StrLenTrim()==0 )
		{
			// Aseguramos que en el fichero de traza no se va a concatener dos veces el timestamp
			m_bSwDateTraceMove = M4_FALSE;
		}

		m_FileParams.m_oPath.StrCpy(StrAux) ;
		m_FileParams.m_bPathLoaded=M4_TRUE ;

	} ;

	if ( m_FileParams.m_oExtension.StrLen()==0 ) {

		switch (m_Dvc) {

		case ClENG_ReportRequest::DVC_PCL:
			m_FileParams.m_oExtension.StrCpy("pcl") ;
			break;

		case ClENG_ReportRequest::DVC_PSC:
			m_FileParams.m_oExtension.StrCpy("ps") ;
			break;

		case ClENG_ReportRequest::DVC_ESCP:
			m_FileParams.m_oExtension.StrCpy("esc") ;
			break;

		case ClENG_ReportRequest::DVC_TXT:
		case ClENG_ReportRequest::DVC_TXT_UNICODE:
			m_FileParams.m_oExtension.StrCpy("txt") ;
			break;

		case ClENG_ReportRequest::DVC_RVW:
			m_FileParams.m_oExtension.StrCpy("rvw") ;
			break;

		case ClENG_ReportRequest::DVC_HTML:
			m_FileParams.m_oExtension.StrCpy("html") ;
			break;

		case ClENG_ReportRequest::DVC_PDF:
		case ClENG_ReportRequest::DVC_PDF_UNICODE:
			m_FileParams.m_oExtension.StrCpy("pdf") ;
			break;
		} ;

		m_FileParams.m_bExtensionLoaded=M4_TRUE ;
		
	} ;

	//----Tratamiento trace move

	if ( m_oMoveTraceFile.StrLenTrim()==0 ) {
		//No se paso el fichero, sera lo que hay en m_oPath
		m_oMoveTraceFile.StrCpy( m_FileParams.m_oPath ) ;
	} else {
		//Si se pasa, el path es relativo al temporal de traza
		ClMIT_Str StrAux;

		StrAux.StrCpy(m_oMoveTraceFile) ;
		SetOutputPath(StrAux, &m_oMoveTraceFile) ;
	} ;

	//Si debemos, concatenamos la cadena de time-stamp
	if ( m_bSwDateTraceMove ) {

		ClMIT_Str StrAux;

		ClMIT_Dates::DateToFileString(&StrAux,ClMIT_Dates::ActualDate()) ;
		m_oMoveTraceFile.StrCat("_");
		m_oMoveTraceFile.StrCat(StrAux);
	} ;

	//Ahora le concatenamos la extension
	m_oMoveTraceFile.StrCat(".log");

	//Y ponemos el modo

	m_poInsp->m_poTrace->SetMove(m_MoveTraceMode,m_oMoveTraceFile) ;

	//----Fin tratamiento trace move

exit:

	#ifdef RPT_ENGINE_TRACE

	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		Detail=m_poInsp->m_poTrace->GetGroupDetail(ENG_TGI_CLREPARG) ;
		m_poInsp->m_poTrace->SetGroupDetail(ENG_TGI_CLREPARG, ThisDetail) ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_ReportArgs::Load()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG ;
		m_poInsp->m_poTrace->SetGroupDetail(ENG_TGI_CLREPARG,Detail) ;
	};

	#endif

	m_poInsp->m_poTrace->TestEffect( SwOK, ai_ErrorEffect ) ;
	
	if (!SwOK && !MessageShown) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLREPARG_MSG_40 ] ) << SEND_MSG ;
	} ;
	
	return SwOK ;
}


ClENG_PagePrinter::PAPER_SOURCE ClENG_ReportArgs::GetBin( m4int16_t ai_iPage ) 
{
	m4char_t Str[40] ;
	m4int16_t Index ;
	m4bool_t  SwOK ;
	ClENG_PagePrinter::PAPER_SOURCE Source ;

	sprintf(Str,"/BIN:%i",(int)ai_iPage-1) ;
	ClMIT_Str::OverStringValid(Str,40) ;
	if ( ( Index=Find(0,Str,0,M4_FALSE) ) < 0 ) {
		return m_BinDefault ;
	} else {
		Source =(ClENG_PagePrinter::PAPER_SOURCE)GetEnum(Index,2,m_EnumBin, &SwOK) ;
		if (SwOK) {
			return Source ;
		} else {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLREPARG_MSG_30 ] ) 
				<< ai_iPage << SEND_MSG ;
			//Efecto local
			return m_BinDefault ;
		} ;
	};
}

m4int32_t ClENG_ReportArgs::GetBinPcl(m4int16_t ai_iPage) 
{
	m4char_t pszStr[40] ;
	m4int16_t iIndex ;
	m4bool_t  SwOK ;
	m4int32_t iPaperSource = -1;

	sprintf(pszStr, "/BINPCL:%i", (m4int32_t) (ai_iPage - 1));
	ClMIT_Str::OverStringValid(pszStr, 40);
	if ((iIndex = Find(0, pszStr, 0, M4_FALSE)) < 0)
	{
		iPaperSource = m_BinPcl;
	}
	else
	{
		iPaperSource = GetLong(iIndex, 2);
	}
	return iPaperSource;
}

void ClENG_ReportArgs::SetOutputPath(const m4char_t *ai_pcFile, ClMIT_Str *ao_poStr) 
{
	m_poInsp->m_poSysInfo->GetValue( ENG_RK_WORKDIR, ao_poStr ) ;
	ClMIT_SysInfo::AppendToPath(ao_poStr, ai_pcFile) ;
	
}

m4bool_t ClENG_ReportArgs::CheckOutputDir(const m4char_t *ai_pcFile, m4bool_t ai_bRelativeDir, ClMIT_Str * ao_oOutputDir)
{

#ifdef _WINDOWS

	StENG_TaskEnv *		TaskEnv = NULL;
	ClMIT_SessionChn *	SesChn = NULL;
	ClMIT_Str			sTempDir;
	ClMIT_Str			sFilePath;
	ClChannelManager *	poChannelManager = NULL;
	ClFileName			oDirName;
	m4char_t			szFileName[M4_MAX_PATH + 1];
	m4uint32_t			iTempDirLen = 0;
	m4uint32_t			iDirLen = 0;
	m4bool_t			bCheckOutputDir = M4_TRUE;
				
	TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	if (TaskEnv == NULL)
	{
		return M4_TRUE;
	}
	
	SesChn = TaskEnv->m_APIChn.m_poMVC->GetSessChn();
	if (SesChn == NULL)
	{
		return M4_TRUE;
	}

	if (SesChn->GetRestrictOutputDir() == M4_FALSE)
	{
		return M4_TRUE;
	}
	
	poChannelManager = TaskEnv->m_APIChn.m_poMVC->GetChannelManager();
	if (poChannelManager != NULL)
	{
		if (poChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE &&
			poChannelManager->GetDMContext()->IsSM() == M4_FALSE)
		{
			// Directorio temporal de los informes.
			m_poInsp->m_poSysInfo->GetValue(ENG_RK_WORKDIR, &sTempDir);
			
			if (ai_bRelativeDir == M4_TRUE)
			{
				sFilePath.StrCat(sTempDir);
				sFilePath.StrCat("\\");
				sFilePath.StrCat(ai_pcFile);
			}
			else
			{
				sFilePath.StrCpy(ai_pcFile);
			}

			// Obtener la ruta completa del directorio de salida.
			if (ClFileName(sFilePath.InChar(0)).GetDir(oDirName) != M4_SUCCESS)
			{
				return M4_TRUE;
			}
			
			// Obtener el nombre del fichero expandido.
			oDirName.GetFileName(szFileName);
			ao_oOutputDir->StrCpy(szFileName);

			// Comparar que el directorio empieza por el directorio temporal de los informes
			// y que lo que viene después es un subdirectorio.
			iTempDirLen = sTempDir.StrLenTrim();
			iDirLen = strlen(szFileName);
			if (strnicmp(szFileName, sTempDir.InChar(0), iTempDirLen) == 0)
			{
				if (iDirLen > iTempDirLen)
				{
					if (szFileName[iTempDirLen] != '\\')
					{
						bCheckOutputDir = M4_FALSE;
					}
				}
			}
			else
			{
				bCheckOutputDir = M4_FALSE;
			}
		}
	}

	return bCheckOutputDir;

#else
	return M4_TRUE;
#endif
}

