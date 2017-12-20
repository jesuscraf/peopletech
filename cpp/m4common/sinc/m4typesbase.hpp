//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              All
// File:                M4typesbase.hpp   
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

#ifndef __M4TYPESBASE_HPP__
#define __M4TYPESBASE_HPP__

#if !defined(_M4SOLX86_) && !defined(_M4SOLSPARC_) && !defined (_M4HPUX1020_) && !defined (_M4HPUX1100_) && !defined(_M4AIX_) && !defined(_M4DECALPHA_)
     #define _M4WINX86_
     // error no se ha definido la plataforma
#endif
//_____________________________________________________________
//
//    MACROS
//_______________________________________________________________

#define  M4_TRUE 			1
#define  M4_FALSE			0


//______________________________________________________________
//
//    TIPOS PROPIOS
//______________________________________________________________

//typedef	enum{ M4_ERROR = -1, M4_SUCCESS = 0, M4_WARNING = 1 }  m4return_t;	

#ifdef _AIX
const char M4_ERROR =  -1;
const char M4_WARNING = 1;
const char M4_SUCCESS = 0;
#else
#define M4_ERROR   -1
#define M4_WARNING  1
#define M4_SUCCESS  0
#endif
typedef char m4return_t;
// codigos de error

//_______________________________________________________________
//
//  TIPOS BASICOS
//_______________________________________________________________

//// OJO intentar usar tipos basicos, y hacer siempre castings explicitos

//Globales a todos los sistemas
typedef void*					    m4pvoid_t;
typedef	char					m4char_t;	// CHAR 8bits (-128,127)
typedef	char	*				m4pchar_t;
typedef const char *			m4pcchar_t;
typedef long                    m4int32_t;      //INT 32bits (-2147483647, 2147483647)
typedef unsigned long           m4uint32_t;     //UNSIGNED int 32 bits (0, 4294967295)

//Globales a todos los sistemas
typedef float                   m4float_t;
typedef double                  m4double_t;     //DOUBLE 1.7E +/- 308 (15 digits)


//________________________________________________________________
//
//   TIPOS NO BASICOS
//________________________________________________________________

typedef double       m4date_t;

// Definicion de bool (segun sistema)
#ifdef _M4WINX86_
// Win32-Intel
typedef bool               m4bool_t;
#endif

//UNIX o WIN-NoIntel
#ifndef _M4WINX86_

// En algunos Compiladores de UNIX puede no existir 'bool'
// ya que es una incorporación reciente al ANSI C++ 3.0
// que a fecha de hoy 29/1/98 ningún fabricante implementa al 100%

// Esta solucion antigua puede no funcionar:
// typedef char		 	m4bool_t;   // BOOL ocupa 8bits
// typedef char*		m4pbool_t;
// typedef char			BOOL;   /// es un char como m4bool_t

// La solucion más simple seria algo así:
// pero en esta solución no debemos confiar en el casting
// automático de "int" a "m4bool_t", aunque en el fondo
// es un "int". Habria mucho que hablar sobre este asunto...
// Otra solución seria hacer una 'class m4bool_t' y sobrecargar
// todas las posibles situaciones... parece demasiada "carga"

//Redefinimos M4_TRUE y M4_FALSE
#ifdef M4_FALSE
#undef M4_FALSE
#endif
#ifdef M4_TRUE
#undef M4_TRUE
#endif


// Para aquellos compiladores Unix que no implementen 'bool'
// definiremos _M4_NO_BOOL, para usar un 'enum'
#ifdef _M4_NO_BOOL

// Unix con bool No definido
typedef enum _m4_bool { m4_false=0, m4_true=1 } m4bool_t;
#define M4_FALSE			m4_false
#define M4_TRUE 			m4_true

#else	// _M4_NO_BOOL

// Unix con bool definido
typedef bool				m4bool_t;
#define M4_FALSE			false
#define M4_TRUE 			true

#endif	// _M4_NO_BOOL

typedef m4bool_t *		m4pbool_t;


#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif



#endif ///  _M4WINX86_

#endif
