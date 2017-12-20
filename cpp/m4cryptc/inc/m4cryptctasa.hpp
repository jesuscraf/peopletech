//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4criptc.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:15/01/1999
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
#ifndef __M4CRYPTC__HPP__
#define __M4CRYPTC__HPP__

#define NO_MD2
#define NO_SHA
#define NO_SHA1
#define NO_RC4
#define NO_RC2
#define NO_BLOWFISH
#define NO_IDEA
#define NO_MDC2
#define NO_RSA
#define NO_DSA
#define NO_DH


#include <m4types.hpp>
#include <m4win.hpp>


m4return_t EncryptOneWay ( m4pcchar_t ai_szsrc , m4int_t ai_ilen , m4pchar_t ao_szdst , m4int_t ai_iLenBufferOut , m4int_t &ao_idlen ) ;
m4return_t SynCrypt ( m4pcchar_t ai_szText , m4int_t ai_iLenText , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szCrypt , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut , m4bool_t ai_bApplyInternalKey = M4_TRUE) ;
m4return_t SynDeCrypt ( m4pcchar_t ai_szCrypt , m4int_t ai_iLenCrypt , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szText , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut , m4bool_t ai_bApplyInternalKey = M4_TRUE) ;
m4return_t SymCryptASCII ( m4pcchar_t ai_szText , m4int_t ai_iLenText , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szCrypt , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) ;
m4return_t SymDeCryptASCII ( m4pcchar_t ai_szCrypt , m4int_t ai_iLenCrypt , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szText , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) ;


#endif

