//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cryptwrap
// File:                
// Project:             peoplenet
// Author:              Meta4 Spain S.A
// Date:                08-03-2006
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#ifndef	_M4CRYPTWRAP_HPP_
#define _M4CRYPTWRAP_HPP_

#include <m4types.hpp>
#include <m4win.hpp>
#include <m4cryptwrap_dll.hpp>

extern "C" long M4_DECL_M4CRYPTWRAP WINAPI SynCryptSecondKeyWrap( m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t ao_szCrypt, m4int_t ai_iLenBufferOut, m4int_t & ao_iLenOut ) ;
extern "C" long M4_DECL_M4CRYPTWRAP WINAPI SynDeCryptSecondKeyWrap( m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t ao_szCrypt, m4int_t ai_iLenBufferOut, m4int_t & ao_iLenOut ) ;
extern "C" long M4_DECL_M4CRYPTWRAP WINAPI SynCryptWrap( m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t ao_szCrypt, m4int_t ai_iLenBufferOut, m4int_t & ao_iLenOut ) ;
extern "C" long M4_DECL_M4CRYPTWRAP WINAPI SynDeCryptWrap( m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t ao_szCrypt, m4int_t ai_iLenBufferOut, m4int_t & ao_iLenOut ) ;

#endif




