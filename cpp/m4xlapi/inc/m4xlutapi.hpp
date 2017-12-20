//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlutapi.hpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:			 21/10/2015
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines Unit Testing API and utilities.
//
//==============================================================================

#ifndef _M4XLUTAPI_H_
#define _M4XLUTAPI_H_

#include "m4xlapi_dll.hpp"
#include <m4types.hpp>
#include <m4var.hpp>

// -----------------------------------------------------------------------------
// Unit testing public API.
// -----------------------------------------------------------------------------

// Compares two Excel workbook structs.
M4_DECL_M4XLAPI  m4return_t CmpWbStruct( m4pchar_t ai_pcWB1, m4pchar_t ai_pcWB2, m4pvoid_t ai_pvContext, m4bool_t& ao_bEqual );

// Compares two Excel worksheets.
M4_DECL_M4XLAPI  m4return_t CmpWsContent( m4pchar_t ai_pcWB1, m4pchar_t ai_pcWB2, int ai_iWS1, int ai_iWS2, int ai_iFR, int ai_iFC, int ai_iLR, int ai_iLC, m4pchar_t ai_pcIgnoreList, m4pvoid_t ai_pvContext, m4bool_t& ao_bEqual );

#endif
