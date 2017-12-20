//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             virtual_value_R.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             03/12/97
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

#include "virt_val.hpp"
#include "chan_knl.hpp"
#include "node_knl.hpp"
#include "chlog.hpp"
#include "v_contxt.hpp"
#include "itemdef.hpp"

#include "m4mdrt.hpp"	// M4MDrt


void _ClVirtual_Value_R::DumpError(const StKernelContext &ai_context, m4uint32_t ai_ErrorCode)
{
    m4uint32_t ItemHandle  = ai_context.pItemDef_Interface->GetHandle();
    ClCompiledMCR * pmcr   = ai_context.pItemDef_Interface->GetpCMCR ();
    DUMP_CHLOG_WARNINGF (ai_ErrorCode, M4ObjNodeItemI(pmcr, ItemHandle));
}

