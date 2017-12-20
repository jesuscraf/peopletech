//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4java
// File:             m4javaapi.hpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:			 12/01/2009
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

#ifndef _M4JAVAAPI_H_
#define _M4JAVAAPI_H_

#include "m4javaapi_dll.hpp"
#include "m4types.hpp"
#include "m4var.hpp"


extern "C" M4_DECL_M4JAVAAPI m4return_t M4CreateObject4J   ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext ) ;
extern "C" M4_DECL_M4JAVAAPI m4return_t M4DestroyObject4J  ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext ) ;
extern "C" M4_DECL_M4JAVAAPI m4return_t M4InvokeMethod4J   ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext ) ;
extern "C" M4_DECL_M4JAVAAPI m4return_t M4ReleaseContexts4J( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext ) ;

#endif
