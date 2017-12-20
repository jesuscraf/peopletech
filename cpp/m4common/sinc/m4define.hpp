//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cldefine.hpp   
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
//    This module set the defines for all the proyect
//
//
//==============================================================================

#ifndef __M4DEFINE_HPP__
#define __M4DEFINE_HPP__

#if defined _WINDOWS && _MSC_VER >= 1900
	#pragma warning(disable: 4251)		// Avoid warning C4251: non dll-interface class
	#pragma warning(disable: 4275)		// Avoid warning C4275: non dll-interface class
	#define __DOMDocument_FWD_DEFINED__	// Para evitar la redefinición del DOMDocument de msxml.h de windows
#endif


#include <assert.h>
#include "m4definebase.hpp"
#include "m4stl.hpp"

#ifdef _DEBUG
#define M4_ASSERT(x)	assert(x)
#else
#define M4_ASSERT(x)
#endif

#if defined ( _DEBUG)
#   define M4_MSG_ASSERT(x, msg)	if (!(x)) {cerr << msg << endl; cerr.flush();} assert(x)
#else
#   define M4_MSG_ASSERT(x, msg)
#endif

// The next two defines could be used to compile the functions as
// inline or normally, depending on the state of _M4_DEBUG CRG
#ifdef _DEBUG
#undef _M4_USING_INLINE
#else
#define _M4_USING_INLINE
#endif

#ifdef _M4_USING_INLINE
#define M4_INLINE inline
#else
#define M4_INLINE
#endif


#ifndef M4_PRINT_ERROR
	#define M4_PRINT_ERROR( type, number )		cerr << "Tipo:" << type << " Número:" << number << endl;
#endif


// Funciones de alineamiento

#define	M4_ROUND_TO_2( x )		( ( ( ( x - 1 ) >> 0x1 ) + 1 ) << 0x1 )
#define	M4_ROUND_TO_4( x )		( ( ( ( x - 1 ) >> 0x2 ) + 1 ) << 0x2 )
#define	M4_ROUND_TO_8( x )		( ( ( ( x - 1 ) >> 0x3 ) + 1 ) << 0x3 )
#define	M4_ROUND_TO_16( x )		( ( ( ( x - 1 ) >> 0x4 ) + 1 ) << 0x4 )

// Funciones de longitud de path

#define	M4_MAX_PATH 512

// Arquitecturas soportadas

// Arquitectura no definida
#define M4_UNDEFINED_ARCH		-1
#define M4_UNDEFINED_STR_ARCH	"UNDEFINED!!!"

// Arquitectura de INTEL
#define M4_ARCH_INTEL			0
#define M4_ARCH_STR_INTEL		"INTEL"

// Arquitectura de SUN_SPARC
#define M4_ARCH_SPARC			1
#define M4_ARCH_STR_SPARC		"SPARC"

// Arquitectura de HP
#define M4_ARCH_PA_RISC			2
#define M4_ARCH_STR_RISC		"PA_RISC"

// Arquitectura de DEC DIGITAL (ALPHA)
#define M4_ARCH_DIGITAL			3	
#define M4_ARCH_STR_DIGITAL		"DIGITAL"

// Arquitectura de AIX IBM
#define M4_ARCH_AIX				4			
#define M4_ARCH_STR_AIX			"AIX"

// Calculemos la arquitectura activa, que desde ahora en adelante sera llamada:
#ifdef _M4SOLSPARC_ 
	#define M4_ARCH				M4_ARCH_SPARC
#endif
#ifdef _WINDOWS
	#define M4_ARCH				M4_ARCH_INTEL 
#endif
#ifdef _M4SOLX86_
	#define M4_ARCH				M4_ARCH_INTEL 
#endif
#ifdef _M4AIX_
	#define M4_ARCH				M4_ARCH_AIX
#endif

#ifndef M4_ARCH
#define M4_ARCH					M4_UNDEFINED_ARCH
#endif


#endif

