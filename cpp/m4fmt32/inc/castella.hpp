//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             CASTELLA.HPP
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98 - 12/06/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines the format for the spanish language
//
//
//==============================================================================

#ifndef __CASTELLA_H
#define __CASTELLA_H
    
#include "plugin.hpp"
#include "m4types.hpp"


/////////////////////////////////////////////////////////////////////////////////

m4int16_t ESP_NumberToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender, m4bool_t bIsDate);
m4int16_t ESP_DecimalToText(ClCadena *a_pCad, ClCadena *a_pNum, m4uint32_t ai_iDecGrup, m4bool_t bGender);      
m4int16_t ESP_YearToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender) ;

const m4char_t* ESP_GetMonth(m4uint16_t);
const m4char_t* ESP_GetDay(m4uint16_t);

void AttachESPLanguage(const StPlugIn** ai_stInfo, m4char_t* ai_pcLanguage);

///////////////////////////////////////////////////////////////////////////////

m4uint32_t ESP_CenDecUni(ClCadena *a_pCad, m4char_t* a_cendecuni, m4bool_t* pbHay,
						 m4bool_t* pbTio, m4bool_t bIsDate, m4uint32_t tamgrup=3);


#endif



