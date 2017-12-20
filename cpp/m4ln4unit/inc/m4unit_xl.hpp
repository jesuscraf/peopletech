//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ln4unit
// File:             m4unit_xl.hpp
// Project:			 m4ln4unit
// Author:           Meta Software M.S. , S.A
// Date:			 04/03/2016
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

#ifndef _M4UNIT_XL_HPP_
#define _M4UNIT_XL_HPP_

#include "m4ln4unit_dll.hpp"
#include "m4types.hpp"
#include "m4var.hpp"


// -----------------------------------------------------------------------------
// Unit testing public API.
// -----------------------------------------------------------------------------

// Compares two Excel workbook structs.
extern "C" M4_DECL_M4LN4UNIT m4return_t M4CmpXlsWb( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );

// Compares two Excel worksheets.
extern "C" M4_DECL_M4LN4UNIT m4return_t M4CmpXlsWs( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );



#endif
