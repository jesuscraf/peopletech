
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4unicode.hpp   
// Project:             Meta4 PeopleNet    
// Author:              Meta Software M.S. , S.A
// Date:                13-11-2006
// Language:            C++
// Operating System:    ALL
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la declaración de als funciones de unicode
//
//
//==============================================================================

#include "m4unicode_dll.hpp"

#include <stdio.h>
#include <m4stl.hpp>

#ifdef _WINDOWS
#include <windows.h>
#endif



#ifndef __M4UNICODE_HPP__
#define __M4UNICODE_HPP__


//=================================================================================
// Defines
//=================================================================================
#define	M4UNI_FACTOR							1.2

#define	M4UNI_UTF8_BOM_SIZE						3
#define	M4UNI_UTF16_BOM_SIZE					2


//=================================================================================
// Símbolos exportados
//=================================================================================

extern char	M4_DECL_M4UNICODE	acUTF8BOM     [ M4UNI_UTF8_BOM_SIZE ] ;
extern char	M4_DECL_M4UNICODE	acUTF16WinBOM [ M4UNI_UTF16_BOM_SIZE ] ;
extern char	M4_DECL_M4UNICODE	acUTF16UnixBOM[ M4UNI_UTF16_BOM_SIZE ] ;
extern char	M4_DECL_M4UNICODE	acUTF16BOM    [ M4UNI_UTF16_BOM_SIZE ] ;



//=================================================================================
// Tipos definidos
//=================================================================================

typedef unsigned short		M4XMLCh ;


// Modo de apertura de ficheros
typedef	enum eUniFileOpenMode_tag
{
	M4UniRead,
	M4UniReadBinary,
	M4UniWrite,
	M4UniWriteBinary,
	M4UniAppend,
	M4UniAppendBinary,
	M4UniReadWrite,
	M4UniReadWriteBinary

} eUniFileOpenMode_t;

// Tipos de encodings de ficheros
typedef	enum eUniFileEncodingType_tag
{
	M4UniNative,
	M4UniANSI,
	M4UniUtf8,
	M4UniUtf16

} eUniFileEncodingType_t;





//=================================================================================
// Funciones del API
// La explicación está en el cpp
//=================================================================================

//=================================================================================
// Funciones generales
//=================================================================================
M4_DECL_M4UNICODE	size_t				M4StrLen			( const char* ai_pccString, int ai_iLength ) ;
M4_DECL_M4UNICODE	int					M4CharPosition		( const char* ai_pccString, int ai_iLength, size_t ai_iCharNumber, size_t& ao_riSize ) ;
M4_DECL_M4UNICODE	int					M4IsAscii			( const char* ai_pccString, int ai_iLength ) ;


//=================================================================================
// Funciones de ficheros
//=================================================================================
M4_DECL_M4UNICODE	int					M4WriteBOM			( FILE* ai_pfFile ) ;
M4_DECL_M4UNICODE	FILE*				M4Fopen				( const char* ai_pccFilename, const eUniFileOpenMode_t ai_eMode, eUniFileEncodingType_t& aio_reEncoding ) ;
M4_DECL_M4UNICODE	void				M4Open				( ofstream &ai_rfFile, const char* ai_pccFilename, eUniFileOpenMode_t ai_eMode, eUniFileEncodingType_t& aio_reEncoding ) ;
M4_DECL_M4UNICODE	void				M4Open				( ifstream &ai_rfFile, const char* ai_pccFilename, eUniFileOpenMode_t ai_eMode, eUniFileEncodingType_t& ao_reEncoding ) ;
M4_DECL_M4UNICODE	char*				M4ReadFile			( ifstream &ai_rfFile, size_t ai_iCharNumber,  eUniFileEncodingType_t ai_eEncoding, int& ao_riLength ) ;
M4_DECL_M4UNICODE	char*				M4ReadFile			( ifstream &ai_rfFile, const char* ai_pccChar, eUniFileEncodingType_t ai_eEncoding, int& ao_riLength ) ;
M4_DECL_M4UNICODE	int					M4RenameFile		( const char* ai_pccOldFilename, const char* ai_pccNewFilename, char** ao_ppError = NULL ) ;
M4_DECL_M4UNICODE	int					M4RemoveFile		( const char* ai_pccFilename, char** ao_ppError = NULL ) ;
M4_DECL_M4UNICODE	int					M4MakeDirectory		( const char* ai_pccDirectory, char** ao_ppError = NULL ) ;
M4_DECL_M4UNICODE	int					M4RemoveDirectory	( const char* ai_pccDirectory, char** ao_ppError = NULL ) ;
M4_DECL_M4UNICODE	int					M4FileStatus		( const char* ai_pccFilename, struct stat& ao_rstStatus, char** ao_ppError = NULL ) ;
M4_DECL_M4UNICODE	char*				M4FileToCpp			( const char* ai_pccString, int& aio_riLength, int ai_iIsUtf8 ) ;


//=================================================================================
// Funciones de XML
//=================================================================================
M4_DECL_M4UNICODE	const char*			M4CharSet			( void ) ;
M4_DECL_M4UNICODE	const char*			M4HTMLHeader		( void ) ;
M4_DECL_M4UNICODE	const char*			M4ANSIHeader		( void ) ;
M4_DECL_M4UNICODE	const char*			M4ISOHeader			( void ) ;
M4_DECL_M4UNICODE	const char*			M4Utf8Header		( void ) ;
M4_DECL_M4UNICODE	const char*			M4XMLHeader			( void ) ;
M4_DECL_M4UNICODE	const char*			M4HTMLScapeChar		( const char ai_ccChar ) ;
M4_DECL_M4UNICODE	const char*			M4XMLScapeChar		( const char ai_ccChar ) ;
M4_DECL_M4UNICODE	char*				M4XMLToCpp			( const M4XMLCh* ai_pcwcString ) ;
M4_DECL_M4UNICODE	M4XMLCh*			M4CppToXML			( const char* ai_pccString ) ;
M4_DECL_M4UNICODE	void				M4XMLToSTL			( const M4XMLCh* ai_pcwcString, string& ao_rsString ) ;
M4_DECL_M4UNICODE	char*				M4XMLToUtf8			( const M4XMLCh* ai_pcwcString ) ;


//=================================================================================
// Funciones de ANSI
//=================================================================================
M4_DECL_M4UNICODE	char*				M4CppToANSI			( const char* ai_pccString, int& aio_riLength ) ;
M4_DECL_M4UNICODE	char*				M4ANSIToCpp			( const char* ai_pccString, int& aio_riLength ) ;


//=================================================================================
// Funciones de UTF8
//=================================================================================
M4_DECL_M4UNICODE	char*				M4CppToUtf8			( const char* ai_pccString, int& aio_riLength ) ;
M4_DECL_M4UNICODE	char*				M4Utf8ToCpp			( const char* ai_pccString, int& aio_riLength ) ;


//=================================================================================
// Funciones de reports
//=================================================================================
M4_DECL_M4UNICODE	char*				M4ReportsToCpp		( const M4XMLCh* ai_pcwcString, int& aio_riLength ) ;
M4_DECL_M4UNICODE	M4XMLCh*			M4CppToReports		( const char* ai_pccString, int& aio_riLength ) ;


//=================================================================================
// Funciones de LN4
//=================================================================================
M4_DECL_M4UNICODE	void				M4SwapUtf16			( const M4XMLCh* ai_pcwcString, int ai_iLength ) ;
M4_DECL_M4UNICODE	char*				M4Utf16ToCpp		( const M4XMLCh* ai_pcwcString, int& aio_riLength ) ;
M4_DECL_M4UNICODE	M4XMLCh*			M4CppToUtf16		( const char* ai_pccString, int& aio_riLength ) ;


//=================================================================================
// Funciones de DB
//=================================================================================
M4_DECL_M4UNICODE	char*				M4DBToCpp			( const void* ai_pcvcString, int& aio_riLength ) ;
M4_DECL_M4UNICODE	void*				M4CppToDB			( const char* ai_pccString, int& aio_riLength ) ;


//=================================================================================
// Funciones de VB
//=================================================================================
#ifdef _WINDOWS
M4_DECL_M4UNICODE	BSTR				M4CppToVB			( const char* ai_pccString ) ;
M4_DECL_M4UNICODE	BSTR				M4CppToVBBinary		( const char* ai_pccString, size_t ai_iMbLength ) ;
M4_DECL_M4UNICODE	char*				M4VBToCpp			( const BSTR& ai_oString ) ;
M4_DECL_M4UNICODE	char*				M4VBToCppBinary		( const BSTR& ai_oString, size_t ai_iWcLength ) ;
#endif


//=================================================================================
// Funciones de locale
//=================================================================================

M4_DECL_M4UNICODE	char*				M4ChrUpr			( char* ai_pcString, size_t& ao_riBytesEaten ) ;
M4_DECL_M4UNICODE	char*				M4StrUpr			( char* ai_pcString ) ;
M4_DECL_M4UNICODE	char*				M4StrLwr			( char* ai_pcString ) ;

M4_DECL_M4UNICODE	int					M4StrCmp			( char* ai_pccString1, char* ai_pccString2 ) ;
M4_DECL_M4UNICODE	int					M4StrCmpi			( char* ai_pccString1, char* ai_pccString2 ) ;


#endif

