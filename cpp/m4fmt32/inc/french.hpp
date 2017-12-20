//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             FRENCH.HPP
// Project:          M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:             21/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
// 
// Definition:
//
//    This module defines the french format for the M4fmt32.dll
//
//
//==============================================================================

#ifndef __FRENCH__H__
#define __FRENCH__H__

#include "plugin.hpp"
#include "m4types.hpp"

//////////////////////////////////////////////////////////////////////////////////

m4int16_t FRA_NumberToText(ClCadena* a_pCad,ClCadena* a_pNum, m4bool_t bGender, m4bool_t bIsDate);
m4int16_t FRA_DecimalToText(ClCadena* a_pCad,ClCadena* a_pNum, m4uint32_t ai_iDecGrup, m4bool_t bGender);
m4int16_t FRA_YearToText(ClCadena* a_pCad,ClCadena* a_pNum, m4bool_t bGender);

const m4char_t* FRA_GetMonth(m4uint16_t);
const m4char_t* FRA_GetDay(m4uint16_t);
	 
void AttachFRALanguage(const StPlugIn** ai_stInfo, m4char_t* ai_pcLanguage);

////////////////////////////////////////////////////////////////////////////////////

m4uint32_t FRA_CenDecUni(ClCadena* a_pCad, m4char_t* a_cendecuni, m4bool_t* pbHay, m4uint32_t tamgrup=3);

#endif

