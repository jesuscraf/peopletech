//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             M4FMT.CPP
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98 - 12/06/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "m4win.hpp" 

#include "fmt.h"
#define _INCLUDING_FROM_M4FMT_
#include "fmtdf.cpp"

#include "clm4fmt.hpp"
#include "m4fmt.hpp"

#include "m4fmt.hmg"

#include "cadena.h"
#include <stdio.h>


m4bool_t WINAPI M4FMTAttachM4Fmt(m4uint32_t* ao_TaskId)
{
	m4bool_t SwOk;
	m4uint32_t OkCount;
	MIT_Id_t NewTask ;

	SPAWN_IN(NewTask,g_FMT_pstPlugIn) ;

	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	OkCount = Insp->m_poTrace->GetOkCount();

	//Inicializacion de variables globales por Tarea
	TaskEnv->g_pClM4Fmt = NULL ;
	TaskEnv->g_fTraceFile = NULL ;
	TaskEnv->g_AccessDesign = NULL ;
	TaskEnv->ClM4Fmt_g_pFormatoWINDOWS = NULL;
	TaskEnv->ClM4Fmt_g_pFormatoISO = NULL;
	TaskEnv->ClM4Fmt_g_pDD = NULL;
	TaskEnv->Container_g_pContainer = NULL ;
	TaskEnv->Cadena_g_pCad = NULL;
	TaskEnv->Cadena_g_pMask = NULL;
	TaskEnv->Cadena_g_pNum = NULL;

	//Creacion de la Base de Datos de Fuentes
	M4_NEW_ITEM( Insp->m_poMemProf, FMT_MGI_PLUG_IN, TaskEnv->m_poPlugIn, FMT_ClPlugInManager ) ;
	TaskEnv->m_poPlugIn->Init();

	M4_NEW_ITEM( Insp->m_poMemProf, FMT_MGI_PLUG_IN, TaskEnv->g_pClM4Fmt, CLM4FMT );

	*ao_TaskId = NewTask;
	*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4FMT_MSG_10 ] ) << SEND_MSG ;
	*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4FMT_MSG_30 ] ) << *ao_TaskId << SEND_MSG ;
	*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4FMT_MSG_40 ] ) << SEND_MSG ;
	Insp->m_poTrace->Indent();

	SwOk = (OkCount!=Insp->m_poTrace->GetOkCount())? M4_FALSE : M4_TRUE;

	SPAWN_OUT() ;

	return SwOk;
}

m4bool_t WINAPI M4FMTSetDesignAccess(m4uint32_t ai_TaskId, ClAccess* ai_poDesignAccess)
{
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	TaskEnv->g_AccessDesign = ai_poDesignAccess;
	
	SWAP_OUT() ;
	
	return M4_TRUE;
}

m4bool_t WINAPI M4FMTDettachM4Fmt(m4uint32_t ai_TaskId)
{
	m4bool_t SwOk;
	m4uint32_t OkCount;
	KILL_IN(ai_TaskId) ;

	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	OkCount = Insp->m_poTrace->GetOkCount();

	M4_DELETE_ITEM( Insp->m_poMemProf, FMT_MGI_PLUG_IN, TaskEnv->m_poPlugIn, FMT_ClPlugInManager ) ;

	M4_DELETE_ITEM( Insp->m_poMemProf, FMT_MGI_PLUG_IN, TaskEnv->g_pClM4Fmt, CLM4FMT ) ;

	Insp->m_poTrace->Unindent();
	*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4FMT_MSG_40 ] ) << SEND_MSG ;
	*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4FMT_MSG_20 ] ) << SEND_MSG ;
	*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4FMT_MSG_30 ] ) << ai_TaskId << SEND_MSG ;
	*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4FMT_MSG_40 ] ) << SEND_MSG ;

	SwOk = (OkCount!=Insp->m_poTrace->GetOkCount())? M4_FALSE : M4_TRUE;

	KILL_OUT(ai_TaskId) ;

	return SwOk;
}

m4int16_t WINAPI M4FMTCreateNewFormat (m4uint32_t ai_TaskId, m4char_t* a_pcNombre, m4int16_t * a_piH)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTCreateNewFormat (a_pcNombre, a_piH);
	
exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTSetProperty (m4uint32_t ai_TaskId, m4int16_t a_iHandle, m4char_t* a_pcNombre, m4char_t* a_pcValor)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTSetProperty (a_iHandle, a_pcNombre, a_pcValor);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTDestroyFormat (m4uint32_t ai_TaskId, m4int16_t a_iHandle)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTDestroyFormat(a_iHandle);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTCreateAddFormat (m4uint32_t ai_TaskId, m4int16_t a_iHandleIn, m4char_t* a_pcNombre, m4int16_t * a_piHandleOut)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTCreateAddFormat (a_iHandleIn, a_pcNombre, a_piHandleOut);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTConvertFmtToFmt (m4uint32_t ai_TaskId, m4int16_t a_iHandleIn, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4int16_t a_iHandleOut, m4char_t* a_pcOut,  m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTConvertFmtToFmt (a_iHandleIn, a_pcIn, a_iTipo, a_iIsSQLM4, a_iHandleOut, a_pcOut, a_iLonOut, a_piLonEscrita);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTApplyFormat (m4uint32_t ai_TaskId, m4int16_t a_iHandle, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTApplyFormat (a_iHandle, a_pcIn, a_iTipo, a_iIsSQLM4, a_pcOut, a_iLonOut, a_piLonEscrita);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTConvertIsoToWin (m4uint32_t ai_TaskId, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTConvertIsoToWin (a_pcIn, a_iTipo, a_iIsSQLM4, a_pcOut, a_iLonOut, a_piLonEscrita);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTConvertWinToIso (m4uint32_t ai_TaskId, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTConvertWinToIso (a_pcIn, a_iTipo, a_iIsSQLM4, a_pcOut, a_iLonOut, a_piLonEscrita);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTConvertFmtToIso (m4uint32_t ai_TaskId, m4int16_t a_iHandle, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTConvertFmtToIso (a_iHandle, a_pcIn, a_iTipo, a_iIsSQLM4, a_pcOut, a_iLonOut, a_piLonEscrita);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTConvertUserToFmt (m4uint32_t ai_TaskId, m4int16_t a_iHandleIn, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4int16_t a_iHandleOut, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTConvertUserToFmt (a_iHandleIn, a_pcIn, a_iTipo, a_iIsSQLM4, a_iHandleOut, a_pcOut, a_iLonOut, a_piLonEscrita);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTConvertSQLToM4 (m4uint32_t ai_TaskId, m4int16_t a_iTipoSql, m4int16_t  *a_piTipoM4)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTConvertSQLToM4 (a_iTipoSql, a_piTipoM4);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTGetFormatName (m4uint32_t ai_TaskId, m4int16_t a_iHandle, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t  *a_piLonEscrita)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTGetFormatName (a_iHandle, a_pcOut, a_iLonOut, a_piLonEscrita);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTGetFormatHandle (m4uint32_t ai_TaskId, m4int16_t  *a_piHandle, m4char_t* a_pcIn)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTGetFormatHandle (a_piHandle, a_pcIn);

exit:
	;
	SWAP_OUT() ;

	return iError;
}

m4int16_t WINAPI M4FMTGetErrorName (m4uint32_t ai_TaskId, m4int16_t a_iError, m4char_t* a_pcErrorName, m4int16_t a_iLonErrorName, m4int16_t  *a_iErrorNamel)
{
	m4int16_t iError;
	SWAP_IN(ai_TaskId) ;
	StFMT_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	iError = TaskEnv->g_pClM4Fmt->_M4FMTGetErrorName (a_iError, a_pcErrorName, a_iLonErrorName, a_iErrorNamel);

exit:
	;
	SWAP_OUT() ;

	return iError;
}