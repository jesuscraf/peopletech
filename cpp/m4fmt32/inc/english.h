//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             English.h
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
//    This module defines...
//
//
//==============================================================================

#ifndef __ENGLISH__H__
#define __ENGLISH__H__
               
#include "plugin.hpp"
#include "m4types.hpp"


/////////////////////////////////////////////////////////////////////////////////

m4int16_t ENG_NumberToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender, m4bool_t bIsDate);
m4int16_t ENG_DecimalToText(ClCadena  *a_pCad, ClCadena  *a_pNum, m4uint32_t ai_iDecGrup, m4bool_t bGender);
m4int16_t ENG_YearToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender) ;

const m4char_t* ENG_GetMonth(m4uint16_t);   
const m4char_t* ENG_GetDay(m4uint16_t);

void AttachENGLanguage(const StPlugIn** ai_stInfo, m4char_t* ai_pcLanguage);

////////////////////////////////////////////////////////////////////////////////////

m4uint32_t ENG_CenDecUni(ClCadena *a_pCad, m4char_t*  a_cendecuni, m4bool_t* pbHay, m4uint32_t tamgrup= 3); //Valor por defecto


#endif

