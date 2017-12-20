//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4dlg.dll
// File:             m4dlg.hpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             08-January-1999
// Language:         C++
// Operating System: Windows 32 ONLY!
// Design Document:  
//
//
//
// Definition:
//
//		This module defines...
//
//
//==============================================================================


#include "m4doc_dll.hpp"
#include "m4types.hpp"
#include "m4typesext.hpp"
#include "m4define.hpp"
#include "m4var.hpp"	



#ifndef __M4DLGCAB_HPP__
#define __M4DLGCAB_HPP__



extern "C" M4_DECL_M4DOC m4return_t SelectFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t SaveFileAs(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);


#endif





