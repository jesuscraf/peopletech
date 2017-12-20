//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              All
// File:                M4types.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module set the typedefs for all the proyect
//
//
//==============================================================================

#ifndef __M4TYPES_HPP__
#define __M4TYPES_HPP__


#if defined(_aCC) || defined (_AIX) || defined (_WINDOWS)
	#define	M4_TYPE_ITERATOR	typename
#else
	#define	M4_TYPE_ITERATOR
#endif


#include "m4define.hpp"
#include "m4typesbase.hpp"


//_______________________________________________________________
//
//  TIPOS BASICOS
//_______________________________________________________________

//// OJO intentar usar tipos basicos, y hacer siempre castings explicitos

//Globales a todos los sistemas
typedef void					m4void_t;

typedef const void*			    m4pcvoid_t;



typedef unsigned char			m4uchar_t;  // UNSIGNED CHAR 8bits (0, 255)
typedef unsigned char	*		m4puchar_t;

typedef int 					m4int_t;	//INT segun la maquihna
typedef int 	*				m4pint_t;
typedef unsigned int 			m4uint_t;	//UINT segun la maquihna
typedef unsigned int	*		m4puint_t;

typedef char                    m4int8_t ;      //INT 8bits (-128,127)
typedef char    *               m4pint8_t ;
typedef unsigned char           m4uint8_t;      //UNSIGNED INT 8bits (0, 255)
typedef unsigned char   *       m4puint8_t;
typedef short                   m4int16_t;      //INT 16 bits (-32768, 32767)
typedef short   *               m4pint16_t;
typedef unsigned short          m4uint16_t;     //UNSIGNED INT 16 bits (0, 65535)
typedef unsigned short  *       m4puint16_t;

typedef long    *               m4pint32_t;

typedef unsigned long   *       m4puint32_t;
typedef const m4int32_t			m4cint32_t;		//INT 32bits (-2147483647, 2147483647)
typedef const char				m4cint8_t;		//INT 8bits (-128,127)

// Definicion de Enteros de 64 bits y bool (segun sistema)
#ifdef _M4WINX86_
// Win32-Intel
typedef __int64 		m4int64_t;      //INT 64 bits (-9.223.372.036.854.775.807, 9.223.372.036.854.775.807)
typedef __int64   *		m4pint64_t;
typedef unsigned __int64	m4uint64_t; 	//UNSIGNED INT 64 bits
typedef unsigned __int64	* m4puint64_t;

typedef bool *             m4pbool_t;

#elif defined(_M4DECALPHA_)
// DEC-Alpha
typedef int              m4int64_t;
typedef int       *      m4pint64_t;
typedef unsigned  int    m4uint64_t;
typedef unsigned  int  * m4int64_t ;  
#elif defined(_M4SOLSPARC_) || defined(_M4SOLX86_) || defined(_M4HPUX1020_) || defined(_M4HPUX1100_) || defined(_AIX)
// Solaris (Sparc o Intel)
typedef long long			m4int64_t;
typedef long long *			m4pint64_t;
typedef unsigned long long	m4uint64_t;
typedef unsigned long long * m4puint64_t;
#endif


//Globales a todos los sistemas
typedef float                   m4float_t;      //FLOAT 3.4E +/- 38 (7 digits)
typedef float    *               m4pfloat_t;

typedef double   *               m4pdouble_t;
typedef long double				m4ldouble_t;    //LONG DOUBLE 1.2E +/- 4932 (19 digits)
typedef long double		*		m4pldouble_t;


//________________________________________________________________
//
//   TIPOS NO BASICOS
//________________________________________________________________
#ifndef _M4WINX86_
	#include <stdio.h>
#endif

typedef size_t		m4size_t;



#ifdef _UNIX
typedef	m4int32_t		m4handle_t;
#else
typedef	intptr_t		m4handle_t;
#endif

typedef m4uint32_t	m4language_t; // Para el identificador de idioma
typedef m4uint32_t	m4LCID_t; // Para el LCID de Windows

//________________________________________________________________
//
//   TIPOS DE WINDOWS
//________________________________________________________________
typedef m4int8_t*				m4lpint8_t;		//LPSTR
typedef const m4int8_t*			m4lpcint8_t;    //LPCSTR
typedef m4lpint8_t              m4lpstr_t;      //LPSTR
typedef m4lpcint8_t             m4lpcstr_t;     //LPCSTR


// Output Format Specifier Macros (printf, fprintf, sprintf...)
#ifdef _WINDOWS
	#define M4_PRIdPTR	"%Id"
	#define M4_PRIuPTR	"%Iu"
#else
	#define M4_PRIdPTR	"%ld"
	#define M4_PRIuPTR	"%lu"
#endif

#ifdef _UNIX
	#include <sys/types.h>
	typedef	pid_t		m4process_t;
#else
	typedef	intptr_t	m4process_t;
#endif


//UNIX o WIN-NoIntel
#ifndef _M4WINX86_
	#include "sqlunx.h"
#endif

// Extensiones en prueba
#include "m4typesext.hpp"

#ifdef _SOLARIS
	#define M4_XERCES_LITERAL(x)	(const XMLCh*)U##x
#else
	#define M4_XERCES_LITERAL(x)	(const XMLCh*)u##x
#endif

#endif   // M4TYPES
