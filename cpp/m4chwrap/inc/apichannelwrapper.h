//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             channelwrapper.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:
//
// Definition:
//     ChannelWrapper.h : Definitions of DLL Exports.
//
//==============================================================================


#ifndef _APICHANNELWRAPPER_H_
#define _APICHANNELWRAPPER_H_

#include "stdafx.h"
#include "m4types.hpp"

// M4GetErrorString - Get Error String through ChDll (using Error-Channel) or M4LogSys (using m4logmsg.ini)
#ifdef M4_USING_DLL
	#ifdef M4_IMPL_CHWRAP
		#define M4_DECL_CHWRAP __declspec(dllexport) _stdcall
	#else
		#define M4_DECL_CHWRAP __declspec(dllimport) _stdcall
	#endif 
#else
	#define M4_DECL_CHWRAP
#endif

/* Bug 0079117
El identificador del error debe ser unsigned
*/

m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorString(m4int32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer);
m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByParams(m4int32_t ai_uiCodErr, SAFEARRAY **ai_pParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer);
m4int16_t _M4ChannelWrapperGetErrorString(m4uint32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, m4language_t ai_uiLang);
m4int16_t _M4ChannelWrapperGetErrorStringByParams(m4uint32_t ai_uiCodErr, SAFEARRAY **ai_pParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, m4language_t ai_uiLang);
m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByLang(m4int32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, short ai_uiLang);
m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByParamsByLang(m4int32_t ai_uiCodErr, SAFEARRAY **ai_pParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, short ai_uiLang);
m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByLangType(m4int32_t ai_uiCodErr, m4pchar_t ai_pszParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, short ai_uiLangType);
m4int16_t M4_DECL_CHWRAP M4ChannelWrapperGetErrorStringByParamsByLangType(m4int32_t ai_uiCodErr, SAFEARRAY **ai_pParams, m4uchar_t ai_byIdParamPos, void* ai_piCVM, BSTR *ao_pcBuffer, short ai_uiLangType);

void M4_DECL_CHWRAP M4ChannelWrapperErrorLookup( void* ai_piCVM, SAFEARRAY **ai_ppCodErrs );

#endif