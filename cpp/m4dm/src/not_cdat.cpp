//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             not_cdat.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             24/06/98
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


#include "not_cdat.hpp"
#include "sizer.hpp"


void ClNotifier_Channel_Data::DestroyRegister_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->DestroyRegister_Before (ai_pnode, ai_br, ai_index);
    }
}

void ClNotifier_Channel_Data::DestroyRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->DestroyRegister_After (ai_pnode, ai_br, ai_index);
    }
}

void ClNotifier_Channel_Data::AddRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->AddRegister_After (ai_pnode, ai_br, ai_index);
    }
}

void ClNotifier_Channel_Data::InsertRegister_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, ClMMIndex ai_index)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->InsertRegister_After (ai_pnode, ai_br, ai_index);
    }
}

void ClNotifier_Channel_Data::DestroyAllRegisters_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->DestroyAllRegisters_After (ai_pnode, ai_br);
    }

}

void ClNotifier_Channel_Data::DestroyBlock_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->DestroyBlock_After (ai_pnode, ai_br);
    }

}

void ClNotifier_Channel_Data::Arrange_After (ClNode_Data *ai_pnode, const ClBlockReference &ai_br, m4uint32_t *ai_pindex_qtc)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->Arrange_After (ai_pnode, ai_br, ai_pindex_qtc);
    }
}

void ClNotifier_Channel_Data::ChannelDataChanged_Before (ClChannel_Data *ai_pClChannel_Data, ClNotifier_Channel_Data *ai_pnewnotifier, ClChannel * ai_poCh)
{
    //esta es un poco más especial, pero tampoco mucho más

    //atención, sólo sirve si somos distintos, sino lio:
    M4_ASSERT (this != ai_pnewnotifier);
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
        {
            if (ai_poCh == m_pClientVector[i]->GetpChannel())
                m_pClientVector[i]->ChannelDataChanged_Before (ai_pClChannel_Data, ai_pnewnotifier, ai_poCh);
        }
    }
}

void ClNotifier_Channel_Data::DestroyProxyBlock_Before (ClNode_Data *ai_pnode, const ClBlockReference &ai_br)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->DestroyProxyBlock_Before (ai_pnode, ai_br);
    }
}

void ClNotifier_Channel_Data::ReleaseAll_Before (void)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->ReleaseAll_Before ();
    }
}

void ClNotifier_Channel_Data::DestroyChannelData_Before (void)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->DestroyChannelData_Before ();
    }
}

void ClNotifier_Channel_Data::DestroyChannel_Before (ClChannel * ai_poCh)
{
    m4uint32_t i;
    for (i = 0; i <= m_iLast; i++)
    {
        if (m_pClientVector[i])
            m_pClientVector[i]->DestroyChannel_Before (ai_poCh);
    }
}

m4return_t 
ClNotifier_Channel_Data::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClNotifier_Channel_Data);

		ao_size-=sizeof(_AClClient_Notifier);
	if (_AClClient_Notifier::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
		ao_size-=sizeof(_AClNotifier);
	if (_AClNotifier::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}


