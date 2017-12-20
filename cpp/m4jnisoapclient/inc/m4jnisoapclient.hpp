//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4jnisoapclient
// File:                m4jnisoapclient.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                12/01/2009
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines ...
//
//
//==============================================================================
#ifndef _M4JNISOAPCLIENT_HPP_
#define _M4JNISOAPCLIEN_HPP_

#include "m4var.hpp"
#include "m4types.hpp"

#include "m4jnisoapclient_dll.hpp"


extern "C" M4_DECL_M4JNISOAPCLIENT m4return_t M4GenerateSoapClient(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

#endif

