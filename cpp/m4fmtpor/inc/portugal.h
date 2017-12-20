//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             Portugal.h
// Project:			 M4fmtpor.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 09/07/2002
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//==============================================================================

#ifndef __PORTUGAL__H__
#define __PORTUGAL__H__
               

#include "cadena.h"
#include "m4types.hpp"
#include "m4fmtpor_dll.hpp"

#include "plugin.hpp"

/////////////////////////////////////////////////////////////////////////////////

short POR_NumberToText(ClCadena *a_pCad, ClCadena *a_pNum, bool bGender, bool bIsDate);
short POR_DecimalToText(ClCadena  *a_pCad, ClCadena  *a_pNum, unsigned long ai_iDecGrup, bool bGender);
short POR_YearToText(ClCadena *a_pCad, ClCadena *a_pNum, bool bGender) ;

const char* POR_GetMonth(unsigned short);   
const char* POR_GetDay(unsigned short);



typedef void (*AttachLanguage_t)(const StPlugIn** ai_stInfo, char* ai_pcLanguage);

extern "C" M4_DECL_M4FMTPOR void AttachNewLanguage(const StPlugIn** ai_stInfo, char* ai_pcLanguage);

////////////////////////////////////////////////////////////////////////////////////

unsigned long POR_CenDecUni(ClCadena *a_pCad, char*  a_cendecuni, bool* pbHay, bool bGender, unsigned long tamgrup= 3); //Valor por defecto


#endif

