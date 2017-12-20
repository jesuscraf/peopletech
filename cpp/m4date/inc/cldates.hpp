//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cldates.hpp
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


#ifndef _CLCDATES_HPP_
#define _CLCDATES_HPP_
//## begin module.includes preserve=yes

#include "fechas.h"


#include "m4stl.hpp"


#include <time.h>
#include "cldatesbase.hpp"
//## end module.includes preserve=yes
//## begin module.epilog preserve=yes


// Convierte una cadena en formato TS a una fecha numérica
m4date_t M4_DECL_M4DATE ClStringToDate( m4pcchar_t ai_pszString ) ;

// Convierte una cadena en formato TS a una fecha numérica dando la longitud de la cadena
void M4_DECL_M4DATE ClStringToDate( m4pcchar_t ai_pszString, size_t ai_szLength, m4date_t &ao_rdDate ) ;

// Convierte dos longs con los dias y segundos a una fecha numérica
void M4_DECL_M4DATE ClLongsToDate( m4uint32_t ai_iDays, m4uint32_t ai_iSeconds, m4date_t &ao_rdDate ) ;

// Convierte dos longs con los dias y segundos a una fecha numérica
void M4_DECL_M4DATE ClDateToLongs( m4date_t ai_dDate, m4uint32_t &ao_riDays, m4uint32_t &ao_riSeconds ) ;

// Devuelve la fecha actual en dates
void M4_DECL_M4DATE ClActualDate( m4date_t &ao_rdDate, m4uint8_t ai_Mode = M4CL_TIME_GM_TIME ) ;

// Devuelve la fecha actual en dates
m4date_t M4_DECL_M4DATE ClActualDate( m4uint8_t ai_Mode = M4CL_TIME_GM_TIME ) ;

// Devuelve la fecha actual en longs
void M4_DECL_M4DATE ClActualDate( m4uint32_t &ao_riDays, m4uint32_t &ao_riSeconds, m4uint8_t ai_Mode = M4CL_TIME_GM_TIME ) ;

// Devuelve la fecha actual en cadena
void M4_DECL_M4DATE ClActualDate( m4pchar_t ao_pszString, size_t ai_szLength, m4uint8_t ai_Mode = M4CL_TIME_GM_TIME ) ;

// Devuelve fecha redondeada a dias
m4date_t M4_DECL_M4DATE ClDateRoundToDays( m4date_t ai_dDate ) ;

// Devuelve fecha redondeada a horas
m4date_t M4_DECL_M4DATE ClDateRoundToHours( m4date_t ai_dDate ) ;

// Devuelve fecha redondeada a minutos
m4date_t M4_DECL_M4DATE ClDateRoundToMinutes( m4date_t ai_dDate ) ;

// Devuelve fecha redondeada a segundos
m4date_t M4_DECL_M4DATE ClDateRoundToSeconds( m4date_t ai_dDate ) ;

// Devuelve fecha -INF
m4date_t M4_DECL_M4DATE ClMinusInfiniteDate ( void );

// Devuelve fecha +INF
m4date_t M4_DECL_M4DATE ClPlusInfiniteDate ( void );

// Pasa de time_t a m4date_t
m4date_t M4_DECL_M4DATE ClTime_tToDate( time_t ai_lTime, m4uint8_t ai_Mode );


//## end module.epilog
#endif // _CLCDATES_HPP_
 
