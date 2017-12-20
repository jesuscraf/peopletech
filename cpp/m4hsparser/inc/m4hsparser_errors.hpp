//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                m4hasparser_errors.hpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Date:                22/2/01
// Language:            C++
// Operating System:    UNIX, WIN32
// Design Document:     
//
//
// Definition:
//
//    Errores definidos en M4HSPARSER
//
//
//==============================================================================


#ifndef __M4HSPARSER_ERRORS_HPP__
#define __M4HSPARSER_ERRORS_HPP__


#include "m4res.hpp"

// Definido en m4res.hpp
// #define M4_HSPARSER_ERROR_BASE				0x001D0000		1900544

//==============================================================================
//  Errores de la m4hsparser
//==============================================================================


#define M4HSPARSER_E_SINTAX			M4_HSPARSER_ERROR_BASE + 0
// "Error en la sentencia analizada.\n\nSe ha encontrado un error sintáctico en la sentencia analizada.\n\nDebe revisar la sentencia."

#define M4HSPARSER_E_RESOLVEJOIN	M4_HSPARSER_ERROR_BASE + 1
// "Error en la sentencia analizada.\n\nAl intentar resolver una estructura de tipo join no se ha encontrado ninguna tabla cuyo alias sea %0:s.\n\nDebe revisar la sentencia."

#define M4HSPARSER_E_ITEMSINTAX		M4_HSPARSER_ERROR_BASE + 2
// "Error en la sentencia analizada.\n\nEn la parte select hay una estructura no válida que no cumple el formato @ITEM = .... .\n\nDebe revisar la sentencia."

#define M4HSPARSER_E_NOBASE			M4_HSPARSER_ERROR_BASE + 3
// "Error en la sentencia analizada.\n\nEn la sentencia analizada no se ha encontrado ninguna tabla base.\n\nDebe revisar la sentencia."

#endif
