//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           loaddll.cpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           4 feb 99
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    To call functions using Loadll.
////
//==============================================================================

#include <stdio.h> 
#include "m4types.hpp" 
#include "m4logsys.hpp"




/*************************************************************************

NAME: FormatErrorCode_Dll

  Use this functions with Loadll.

  Prototype:

  extern "C" M4_DECL_M4LOGSYS m4return_t FormatErrorCode_Dll (
	m4uint32_t ai_ulCode,
	m4pchar_t ai_pszParams,
	m4uchar_t ai_byIdParamPos,
	m4pchar_t ao_pszOutString,
	m4uint32_t ai_uBufferSize,
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang);



*************************************************************************/

extern "C" M4_DECL_M4LOGSYS m4return_t FormatErrorCode_Dll (
	m4uint32_t ai_ulCode,
	m4pchar_t ai_pszParams,
	m4uchar_t ai_byIdParamPos,
	m4pchar_t ao_pszOutString,
	m4uint32_t ai_uBufferSize,
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang)
{
	return FormatErrorCode (
		ai_ulCode,
		ai_pszParams,
		ai_byIdParamPos,
		ao_pszOutString,
		ai_uBufferSize,
		ao_uBufferFilled,
		ai_uiLang
	);
}

extern "C" M4_DECL_M4LOGSYS void LogsysSetWorkingDir_Dll(m4pcchar_t ai_pszWorkDir) 
{
	LogsysSetWorkingDir(ai_pszWorkDir);
}

