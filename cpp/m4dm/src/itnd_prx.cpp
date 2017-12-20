#include "dmres.hpp"

//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             itnd_prx.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             08/09/97
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

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "item_prx.hpp"
#include "node_knl.hpp"
#include "rec_tran.hpp"
#include "chan_knl.hpp"
#include "dsitem.hpp"
#include "chlog.hpp"
#include "itemdef.hpp"
#include "v_contxt.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"

///////////////////////////////////////////////////////////////////////////////
//
// ClItemProxy_Node
//

//## end module.includes preserve=yes
//ClItemProxy_Node::ClItemProxy_Node ()
//{
//}

//-----------------------------------------------------------------

m4return_t ClItemProxy_Node::Validation (const StKernelContext &ai_context, m4uint32_t ai_Offset)
{
    if (ai_context.GetpRWT ())
        return M4_SUCCESS;
    else
    {
        ClCompiledMCR * pmcr   = ai_context.pItemDef_Interface->GetpCMCR ();
        ClHandle ItemHandle = ai_context.pItemDef_Interface->GetHandle();

        DUMP_CHLOG_DEBUGF(M4_CH_DM_NULL_RWT + ai_Offset, M4ObjNodeItemI(pmcr, ItemHandle));

        return M4_ERROR;
    }

    return M4_SUCCESS;
}

m4return_t ClItemProxy_Node::GetDescription (const StKernelContext &ai_context, const void *&oi_Item) const
{
    return ai_context.GetpRWT ()->GetItemBlobFileDescription (M.iPos, 0, oi_Item);
}

m4return_t ClItemProxy_Node::GetAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices) const
{
    return ai_context.GetpRWT ()->GetItemDataAddressByPos_AndHasSlices (M.iPos, 0, *ao_address, ao_hasslices);
}

m4return_t ClItemProxy_Node::GetSize (const StKernelContext &ai_context, m4uint32_t &ao_size) const
{
    return ai_context.GetpRWT ()->GetItemSizeByPos (M.iPos, 0, ao_size);
}

//-----------------------------------------------------------------

m4return_t ClItemProxy_Node::SetValueByOffset (const StKernelContext &ai_context, const void* oi_Item, const m4uint32_t ai_Size, m4uint32_t ai_offset, m4uint32_t * ao_pRetSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) const
{
    return ai_context.GetpRWT ()->SetItemValueByPosAndOffset (M.iPos, (void*)oi_Item, 0, ai_offset, ai_Size, ao_pRetSize, ai_bChangeStatus, ai_bIsUser);
}
m4return_t ClItemProxy_Node::SetDescription (const StKernelContext &ai_context, const void* oi_Item) const
{
    return ai_context.GetpRWT ()->SetItemBlobFileDescription (M.iPos, (void*)oi_Item, 0);
}

m4return_t ClItemProxy_Node::SetSize (const StKernelContext &ai_context, m4uint32_t ai_newsize) const
{
    return ai_context.GetpRWT ()->SetItemSizeByPos (M.iPos, 0, ai_newsize);
}

//-----------------------------------------------------------------

m4return_t ClItemProxy_Node::GetOldAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices) const
{
    m4return_t res = ai_context.GetpRWT ()->GetLastItemDataAddressByPos_AndHasSlices (M.iPos, 0, *ao_address, ao_hasslices);
    if (res != M4_SUCCESS) {
        *ao_address = 0;
        ao_hasslices = M4_FALSE;
    }
    return M4_SUCCESS;
}

m4return_t ClItemProxy_Node::GetOldSize (const StKernelContext &ai_context, m4uint32_t &ao_size) const
{
    m4return_t res = ai_context.GetpRWT ()->GetLastItemSizeByPos (M.iPos, 0, ao_size);
    if (res != M4_SUCCESS)
        ao_size = 0;
    return M4_SUCCESS;
}

//-----------------------------------------------------------------

m4return_t ClItemProxy_Node::IsNull (const StKernelContext &ai_context, m4bool_t &ao_IsNull)
{
    return ai_context.GetpRWT ()->GetItemNullByPos (M.iPos, 0, ao_IsNull);
}

m4return_t ClItemProxy_Node::SetNull (const StKernelContext &ai_context, m4bool_t ai_bChangeStatus)
{
    return ai_context.GetpRWT ()->SetItemNullByPos (M.iPos, 0, M4_TRUE, ai_bChangeStatus);
}

m4return_t ClItemProxy_Node::OldIsNull (const StKernelContext &ai_context, m4bool_t &ao_IsNull)
{
    m4return_t res =  ai_context.GetpRWT ()->GetLastItemNullByPos (M.iPos, 0, ao_IsNull);
    if (res != M4_SUCCESS)
        ao_IsNull = M4_TRUE;
    return M4_SUCCESS;
}
 
m4return_t ClItemProxy_Node::GetHasSlices (const StKernelContext &ai_context, m4bool_t &ao_hasslices)
{
    return ai_context.GetpRWT ()->ItemHasSlices (M.iPos, 0, ao_hasslices);
}

m4return_t ClItemProxy_Node::SetHasSlices (const StKernelContext &ai_context, m4bool_t ai_hasslices)
{
    return ai_context.GetpRWT ()->SetItemHasSlices (M.iPos, 0, ai_hasslices);
}

m4return_t ClItemProxy_Node::OldHasSlices (const StKernelContext &ai_context, m4bool_t &ao_hasslices)
{
    m4return_t res = ai_context.GetpRWT ()->LastItemHasSlices (M.iPos, 0, ao_hasslices);
    if (res != M4_SUCCESS)
        ao_hasslices = M4_FALSE;
    return M4_SUCCESS;
}

m4return_t ClItemProxy_Node::GetPriority (const StKernelContext &ai_context, m4char_t &ao_priority)
{
    return ai_context.GetpRWT ()->GetItemPriority (M.iPos, 0, ao_priority);
}

m4return_t ClItemProxy_Node::SetPriority (const StKernelContext &ai_context, m4char_t ai_priority)
{
    return ai_context.GetpRWT ()->SetItemPriority (M.iPos, 0, ai_priority);
}

m4return_t ClItemProxy_Node::OldPriority (const StKernelContext &ai_context, m4char_t &ao_priority)
{
    m4return_t res = ai_context.GetpRWT ()->LastItemPriority (M.iPos, 0, ao_priority);
    if (res != M4_SUCCESS)
        ao_priority = 3;
    return M4_SUCCESS;
}

m4return_t ClItemProxy_Node::GetIsUser (const StKernelContext &ai_context, m4bool_t &ao_bIsUser)
{
    return ai_context.GetpRWT ()->GetItemIsUser (M.iPos, 0, ao_bIsUser);
}

m4return_t ClItemProxy_Node::CreateTempFile(const StKernelContext &ai_context, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension)
{
    ClDsItemBlobFile* IBF = ai_context.GetpRWT ()->GetItemBlobFile(M.iPos, 0);
    if (IBF)
	{
        return IBF->CreateTempFile(ai_pDirectory, ai_pExtension);
	}
    else
	{
        return M4_ERROR;
	}
}

m4return_t ClItemProxy_Node::ChangedMask(const StKernelContext &ai_context, m4uint32_t & ao_oChangedMask)
{
    ClComparisonResultMask CRM;

    ClDsItemBlobFile* IBF = ai_context.GetpRWT ()->GetItemBlobFile(M.iPos, 0);
    if (IBF)
    {
        IBF->ChangedMask(CRM);
        ao_oChangedMask = (m4uint32_t) CRM;
        return M4_SUCCESS;
    }
    else
        return M4_ERROR;
}
