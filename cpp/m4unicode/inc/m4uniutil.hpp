
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4uniutil.hpp   
// Project:             Meta4 PeopleNet    
// Author:              Meta Software M.S. , S.A
// Date:                03-01-2007
// Language:            C++
// Operating System:    ALL
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la declaración de las funciones de utilidad de unicode
//
//
//==============================================================================


#include <stdio.h>

#ifdef _WINDOWS
#include <windows.h>
#endif




#ifndef __M4UNIUTIL_HPP__
#define __M4UNIUTIL_HPP__





//=================================================================================
// Defines
// Ver notas _Utf16Chars
//
//    Char. number range  |        UTF-8 octet sequence
//       (hexadecimal)    |              (binary)
//    --------------------+---------------------------------------------
//    0000 0000-0000 007F | 0xxxxxxx
//    0000 0080-0000 07FF | 110xxxxx 10xxxxxx
//    0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
//    0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//
//=================================================================================

// Indica si un byte utf8 es ascci. Comienza por 1 bit 0 (0xxx xxxx)
#define	M4UNI_IS_ASCII( c )				( ( c >> 7 ) == 0x00 )

// Indica si un byte utf8 es leading de un caracter ansi. Comienza por 1100 00 (1100 00xx)
#define	M4UNI_IS_ANSI( c )				( ( c >> 2 ) == 0x30 )

// Indica si un byte utf8 no es trailing. No comienza por 10 (10xx xxxx)
#define	M4UNI_IS_NOT_TRAILING( c )		( ( b >> 6 ) != 0x02 )

// Calcula la longitud de un caracter utf-8 a partir de su leading byte
#define	M4UNI_UTF8_LENGTH( c )			( c <= 0x7F ? 1 : c <= 0xDF ? 2 : c <= 0xEF ? 3 : 4 )

// Convierte 2 bytes utf8 que representan un caracter ansi en su correspondiente byte ansi.
// 2 primeros bits los 2 últimos de c, 6 últimos bits los 6 últimos de d
#define	M4UNI_UTF8_TO_ANSI( c, d )		( ( c << 6 ) | ( d & 0x3F ) )




//=================================================================================
// Funciones del utilidad
// La explicación está en el cpp
//=================================================================================

int				_IsUnicode		( void ) ;
void			_SetUnicode		( int ai_iUnicode ) ;

size_t			_Utf16Chars		( const char* ai_pccString, int ai_iLength ) ;
size_t			_Utf8Bytes		( const M4XMLCh* ai_pcwcString, int ai_iLength ) ;
size_t			_Utf8Chars		( const char* ai_pccString, int ai_iLength ) ;
int				_Utf8Position	( const char* ai_pccString, int ai_iLength, size_t ai_iCharNumber, size_t& ao_riSize ) ;

char*			_Utf16ToUtf8	( const M4XMLCh* ai_pcwcString, int& aio_riLength ) ;
M4XMLCh*		_Utf8ToUtf16	( const char* ai_pccString, int& aio_riLength ) ;

char*			_Utf16ToANSI	( const M4XMLCh* ai_pcwcString, int& aio_riLength ) ;
M4XMLCh*		_ANSIToUtf16	( const char* ai_pccString, int& aio_riLength ) ;

char*			_Utf16ToCpp		( const M4XMLCh* ai_pcwcString, int& aio_riLength ) ;
M4XMLCh*		_CppToUtf16		( const char* ai_pccString, int& aio_riLength ) ;


#ifdef _WINDOWS
BSTR		_MultiByteToWideChar( const char* ai_pccString, UINT ai_uiPage, int ai_iMbLength ) ;
char*		_WideCharToMultiByte( const BSTR& ai_oString, UINT ai_uiPage, int ai_iWcLength ) ;
#endif



#endif



