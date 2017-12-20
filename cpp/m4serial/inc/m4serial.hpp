//-- {CycleCode: 61} file [0..2804]
//-- {AddDecl: 62} module.includes preserve=yes [0..725]
//## begin module.includes preserve=yes

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4serial.dll
// File:                m4serial.hpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene el interfaz de la serialización
//
//
//==============================================================================



#include "m4serial_dll.hpp"
#include "clsrlzbl.hpp"
#include "clprsstb.hpp"

#include "m4types.hpp"
//## end module.includes preserve=yes
//-- {InsertRegion: 66} module.vulnerableDeclarations [726..727]
//-- {StartSubRegion: 63} module.ifndef [728..752]
#ifndef __M4SERIAL_HPP__
//-- {StartSubRegion: 64} module.define [753..777]
#define __M4SERIAL_HPP__
//-- {InsertRegion: 67} module.vulnerableDeclarations [778..2794]
//## begin module.epilog preserve=yes


//=============================================================================
// Defines de las Distintas Arquitecturas
//=============================================================================

#define M4_ARCHITECTURE_INTEL		0	// Arquitectura INTEL
#define M4_ARCHITECTURE_SPARC		1	// Arquitectura SPARC - SUN
#define M4_ARCHITECTURE_PA_RISC		2	// Arquitectura PA-RISC - HP
#define M4_ARCHITECTURE_ALPHA		3	// Arquitectura Alpha - DIGITAL
#define M4_ARCHITECTURE_POWERPC		4	// Arquitectura PowerPC - IBM AIX


// Definicion del Orden de los Bytes
#define M4_BYTEORDER_NORMAL		0		// LITTLE-ENDIAN	(Intel)
#define M4_BYTEORDER_INVERTED	1		// BIG-ENDIAN


// Definicion de la Arquitectura en la que nos encontramos
// Se define a partir de la maquina en que se compila
#if   defined(_M4WINX86_)
	#define M4_ARCHITECTURE		M4_ARCHITECTURE_INTEL

#elif defined(_M4SOLX86_)
	#define M4_ARCHITECTURE		M4_ARCHITECTURE_INTEL

#elif defined(_M4SOLSPARC_)
	#define M4_ARCHITECTURE		M4_ARCHITECTURE_SPARC

#elif defined(_M4HPUX1020_)
	#define M4_ARCHITECTURE		M4_ARCHITECTURE_PA_RISC

#elif defined(_M4HPUX1100_)
	#define M4_ARCHITECTURE		M4_ARCHITECTURE_PA_RISC

#elif defined(_M4DECALPHA_)
	#define M4_ARCHITECTURE		M4_ARCHITECTURE_ALPHA

#elif defined(_M4AIX_)
	#define M4_ARCHITECTURE		M4_ARCHITECTURE_POWERPC

#endif


//=================================================================================
// Funciones de tipo de orden
//=================================================================================

inline	m4int8_t	M4_DECL_M4SERIAL	M4GetByteOrder( m4uint16_t ai_iArchitecture )
{
	switch( ai_iArchitecture )
	{
		case M4_ARCHITECTURE_INTEL :
			return M4_BYTEORDER_NORMAL ;

		case M4_ARCHITECTURE_SPARC :
			return M4_BYTEORDER_INVERTED ;

		case M4_ARCHITECTURE_PA_RISC :
			return M4_BYTEORDER_INVERTED ;

		case M4_ARCHITECTURE_ALPHA :
			return M4_BYTEORDER_INVERTED ;

		case M4_ARCHITECTURE_POWERPC :
			return M4_BYTEORDER_INVERTED ;

		default:
			return M4_BYTEORDER_NORMAL ;
	}
}



//## end module.epilog
//-- {StartSubRegion: 65} module.endif [2795..2804]
#endif


