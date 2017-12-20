//	==============================================================================
//
//	 (c) Copyright  1991-2014 Meta4 Spain S.A
//	 All rights reserved.
//
//	 Module:            m4ln4unit		
//	 File:              m4unit_file.hpp
//	 Project:           m4ln4unit
//	 Author:            Meta4 Spain S.A
//	 Date:				160211
//	 Language:          C++
//	 Operating System:  WINDOWS
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================

#ifndef _M4UNIT_FILE_HPP_
#define _M4UNIT_FILE_HPP_

#include "m4ln4unit_dll.hpp"
#include "m4types.hpp"
#include "m4var.hpp"

// -------------
// API
// -------------
// PDF binary comparation
extern "C" M4_DECL_M4LN4UNIT m4return_t M4ComparePDF(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
// File binary comparation
extern "C" M4_DECL_M4LN4UNIT m4return_t M4CompareFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
// -------------

#endif