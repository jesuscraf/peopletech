//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             USA.HPP
// Project:          M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:             22/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines the American-English format for the M4fmt32.dll
//
//
//==============================================================================

#ifndef __USA__HPP__
#define __USA__HPP__
               
#include "plugin.hpp"
#include "m4types.hpp"


//////////////////////////////////////////////////////////////////////////////////
      
m4int16_t USA_NumberToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender, m4bool_t bIsDate);
m4int16_t USA_DecimalToText(ClCadena  *a_pCad, ClCadena  *a_pNum, m4uint32_t ai_iDecGrup, m4bool_t bGender);
m4int16_t USA_YearToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender) ;

const m4char_t* USA_GetMonth(m4uint16_t);   
const m4char_t* USA_GetDay(m4uint16_t);
   
void AttachUSALanguage(const StPlugIn** ai_stInfo, m4char_t* ai_pcLanguage);

///////////////////////////////////////////////////////////////////////////////////

m4uint32_t USA_CenDecUni(ClCadena *a_pCad, m4char_t*  a_cendecuni, m4bool_t* pbHay, m4uint32_t tamgrup= 3); //Valor por defecto

#endif


