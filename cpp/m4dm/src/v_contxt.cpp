//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             v_contxt.cpp
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

#include "v_contxt.hpp"

#include "aregister.hpp"
#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "access.hpp"
//#include "defmain.hpp"

ClRecordWithTransactions *StKernelContext::GetpRWT (void) const
{
    return pNode_Data->GetpRWT ();
}


ClMMIndex StGMContext_Register::GetMMIndex (void) const
{
    M4_ASSERT (reg_union.m_pRegister);

    return reg_union.m_pRegister->GetMMIndex ();
}

ClRegisterIndex StGMContext_Register::GetRegisterIndex (void) const
{
    if (reg_union.m_pRegister) {
        return reg_union.m_pRegister->Index;
    }
    else {
        return M4DM_EOF_INDEX;
    }
}

void StKernelContext::Init1 (_ClRegister_Base *ai_pr)
{
    M4_ASSERT (sizeof (StGMContext_Index) == sizeof (StGMContext_Register));

    if (ai_pr)
        m_piMMIndex = &ai_pr->MMIndex;

    _AStGMContext *p = new (&m_xxx) StGMContext_Register (ai_pr);
    m_poAccess = ai_pr->GetpAccess();
}

void StKernelContext::GetStartDate (m4double_t &ao_dDate) const
{
    m4VariantType vDate;
    if (m_poAccess)
    {
        //m_poAccess->GetpChannel()->Data_StartDate.Get(vDate);
		m_poAccess->ExecutionStartDate.Get(vDate);
        ao_dDate = vDate.Data.DoubleData;
    }
    else
        ao_dDate = M4_MINUS_INF_DATE;
}

void StKernelContext::GetEndDate (m4double_t &ao_dDate) const
{
    m4VariantType vDate;
    if (m_poAccess)
    {
        //m_poAccess->GetpChannel()->Data_EndDate.Get(vDate);
		m_poAccess->ExecutionEndDate.Get(vDate);
        ao_dDate = vDate.Data.DoubleData;
    }
    else
        ao_dDate = M4_PLUS_INF_DATE;
}


void StKernelContext::GetUnitDate (m4double_t &ao_dDate) const
{
    m4VariantType vDate;
    if (m_poAccess)
    {
        m_poAccess->GetpChannel()->UnitDate.Get(vDate);
        ao_dDate = vDate.Data.DoubleData;
    }
    else
        ao_dDate = M4_UNIT_ONE_DAY;
}


m4return_t 
StKernelContext::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(StKernelContext);
		ao_size-=sizeof(BlockReference);
		ao_size-=sizeof(m_xxx);

    if (BlockReference.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
    if (m_xxx.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t 
_AStGMContext::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_AStGMContext);

	return M4_SUCCESS;
}
