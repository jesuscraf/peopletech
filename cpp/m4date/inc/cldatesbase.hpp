//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cldatesbase.hpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                19-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the prototypes for date manipulating functions
//
//
//==============================================================================

#ifndef __CLDATESBASE_HPP__
#define __CLDATESBASE_HPP__

#include <m4date_dll.hpp>

#define	M4CL_MINUS_INFINITE_DATE	                    "1800-01-01"
#define	M4CL_PLUS_INFINITE_DATE	                        "4000-01-01"
#define M4CL_PLUS_INFINITE_YEAR							4000
#define M4CL_MINUS_INFINITE_YEAR						1800
#define M4CL_PLUS_INFINITE_IN_JULIAN					1460594.0 //4000-01-01 in iso
#define M4CL_MINUS_INFINITE_IN_JULIAN					657061.0  //1800-01-01 in iso


//
// Convierte una serie de números que representan una fecha a esa fecha numérica
//

void M4_DECL_M4DATE ClNumbersToDate( int ai_iYear, int ai_iMonth, int ai_iDay, m4int32_t ai_iHour, m4int32_t ai_iMinute, m4int32_t ai_iSecond, m4date_t &ao_rdDate ) ;


//
// Convierte una fecha numérica a una serie de números que representan esa fecha
//

void M4_DECL_M4DATE ClDateToNumbers( m4date_t ai_dDate, int &ao_riYear, int &ao_riMonth, int &ao_riDay, m4int32_t &ao_riHour, m4int32_t &ao_riMinute, m4int32_t &ao_riSecond ) ;


//
// Convierte una cadena en formato TS a una fecha numérica
//

void M4_DECL_M4DATE ClStringToDate( m4pcchar_t ai_pszString, m4date_t &ao_rdDate ) ;

//
// Convierte una fecha numérica en una cadena en formato TS
// Si la cadena es más pequeña del formato, se trunca
//

void M4_DECL_M4DATE ClDateToString( m4date_t ai_dDate, m4pchar_t ao_pszString, size_t ai_szLength ) ;

#endif
