//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             clfuvar.cpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             5/05/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class FilterUtilityVariableArguments
//
//==============================================================================

#include "smarthan.hpp"
#include "channel.hpp"
#include "clcodejit.hpp"
#include "clbsearch.hpp"


ClSmartHandle::ClSmartHandle(m4int32_t ai_iHandle, ClChannel * ai_pClChannel)
{
    m_pClChannel = ai_pClChannel;
    m_vHandle.Type = M4CL_CPP_TYPE_NUMBER;
    m_vHandle.Data.DoubleData = (m4double_t) ai_iHandle;
    _ClSmartPointer::IncReference ();
    m_bOwner = M4_FALSE;
}


ClSmartHandle::ClSmartHandle(ClChannel * ai_pClChannel)
{
    m_pClChannel = ai_pClChannel;
    m_vHandle.Data.DoubleData = 0;
    m_vHandle.Type = M4CL_CPP_TYPE_NUMBER;
    _ClSmartPointer::IncReference ();
    m_bOwner = M4_TRUE;
}

ClSmartHandle * ClSmartHandle::Attach(void)
{
    _ClSmartPointer::IncReference ();
    return this;
}

m4return_t ClSmartHandle::Detach(void)
{
    m4int32_t iCount = _ClSmartPointer::GetReferenceCount ();
    if (iCount == 1)
    {
        if ((m_vHandle.Data.DoubleData)&&(m_bOwner))
        {
            m_pClChannel->GetpCodeJIT ()->RemoveCodebyHandle ((m4int32_t)m_vHandle.Data.DoubleData);
        }
        _ClSmartPointer::DecReference ();
    }
    else
        _ClSmartPointer::DecReference ();

    return M4_SUCCESS;
}

m4return_t ClSmartHandle::SetHandle(m4VariantType ai_vNewHandle)
{
/*    if (m_pvHandle)
        delete m_pvHandle;
    m_pvHandle = new m4VariantType(ai_vNewHandle);
*/
    M4_ASSERT(ai_vNewHandle.Type == M4CL_CPP_TYPE_NUMBER);

    m_vHandle.Data.DoubleData = ai_vNewHandle.Data.DoubleData;
    m_bOwner = M4_TRUE;

    return M4_SUCCESS;
}

m4return_t ClSmartHandle::GetHandle(m4VariantType & ao_vNewHandle)
{
    ao_vNewHandle = m_vHandle;

    return M4_SUCCESS;
}

m4return_t 
ClSmartHandle::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClSmartHandle);
		ao_size-=sizeof(_ClSmartPointer);
		ao_size-=sizeof(m_vHandle);

	if (_ClSmartPointer::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

    if (m_vHandle.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

//*****************************************************************
// SMARTSEARCHINEX
//*****************************************************************


ClSmartSearchIndex::ClSmartSearchIndex(ClBSearchIndex * ai_pBSearchIndex)
{
    m_pBSearchIndex = ai_pBSearchIndex;
    _ClSmartPointer::IncReference ();
    m_bOwner = M4_FALSE;
}


ClSmartSearchIndex::ClSmartSearchIndex(void)
{
    m_pBSearchIndex = 0;
    _ClSmartPointer::IncReference ();
    m_bOwner = M4_TRUE;
}

ClSmartSearchIndex * ClSmartSearchIndex::Attach(void)
{
    _ClSmartPointer::IncReference ();
    return this;
}

m4return_t ClSmartSearchIndex::Detach(void)
{
    m4int32_t iCount = _ClSmartPointer::GetReferenceCount ();
    if (iCount == 1)
    {
        if ((m_pBSearchIndex)&&(m_bOwner))
            delete m_pBSearchIndex;

        _ClSmartPointer::DecReference ();
    }
    else
        _ClSmartPointer::DecReference ();

    return M4_SUCCESS;
}

m4return_t ClSmartSearchIndex::SetIndex(ClBSearchIndex * ai_pBSearchIndex)
{
    m_pBSearchIndex = ai_pBSearchIndex;
    m_bOwner = M4_TRUE;

    return M4_SUCCESS;
}

ClBSearchIndex * ClSmartSearchIndex::GetIndex(void)
{
    return m_pBSearchIndex;
}

m4return_t 
ClSmartSearchIndex::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClSmartSearchIndex);
		ao_size-=sizeof(_ClSmartPointer);

	if (_ClSmartPointer::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

    if ((m_bOwner)&&(m_pBSearchIndex))
    {
        if (m_pBSearchIndex->GetSize(ao_size)!=M4_SUCCESS)
        {
		    return M4_ERROR;
	    }
    }

	return M4_SUCCESS;
}
