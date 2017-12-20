//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             WrapIMap.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             04/12/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapIMap
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapimap.h"
#include "cldefine.hpp"
#include "wrapdef.h"
#include "gstring.h"
#include "instmap.hpp"
#include "wrapaccess.h"
#include "newinterface.h"
#include "rerror.h"
#include "access.hpp"
#include "channel.hpp"
#include "wrapchannel.h"


/////////////////////////////////////////////////////////////////////////////
// CWrapIMap

STDMETHODIMP CWrapIMap::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapCVM };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

CWrapIMap::CWrapIMap()
	: m_poLevel2(0)
{
}

void CWrapIMap::Init(ClLevel2* ai_VarRef)
{
	m_poLevel2 = ai_VarRef;
}

STDMETHODIMP CWrapIMap::GetChannelName(long ai_pos, BSTR* ao_pst)
{
	ENTER_METHOD(CWrapIMap::GetChannelName);

	gstring tmpGstring(m_poLevel2->m_oInstanceMap.GetChannelName(ai_pos));

	if (tmpGstring)
	{
		*ao_pst = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapIMap::GetChannelName, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapIMap::GetChannelName, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapIMap::GetInstance(long ai_pos, BSTR* ao_pst)
{
	ENTER_METHOD(CWrapIMap::GetInstance);
	
	m4return_t ret;
	m4pcchar_t pIdInstance = 0;
	ClAccess * pAccess = 0;

	ret = m_poLevel2->m_oInstanceMap.GetInstanceAndAccess(ai_pos, pIdInstance, pAccess);

	if (M4_SUCCESS == ret)
	{
		gstring tmpGstring(pIdInstance);

		if (tmpGstring)
		{
			*ao_pst = (BSTR)tmpGstring;

			LEAVE_METHOD(CWrapIMap::GetInstance, S_OK);
		}
	}

	LEAVE_METHOD(CWrapIMap::GetInstance, E_UNEXPECTED);
}

STDMETHODIMP CWrapIMap::GetAccess(long ai_pos, IWrapAccess** ao_access)
{
	ENTER_METHOD(CWrapIMap::GetAccess);

	//Nos llega un puntero a Access, creamos uno nuevo para devolverlo.
	ClAccess* poAccessOrigin = m_poLevel2->m_oInstanceMap.GetAccess(ai_pos);
	ClAccess* poAccess = poAccessOrigin->GetpChannel()->CreateAccess();

	if (poAccess)
	{
		if (NewInterface(CLSID_WrapAccess, IID_IWrapAccess, (IUnknown**)ao_access) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapChannel::GetAccess, E_UNEXPECTED);
		}
		else
		{
			((CWrapAccess*)(*ao_access))->Init(poAccess);
			LEAVE_METHOD(CWrapIMap::GetAccess, S_OK);
		}
	}
	else
	{
		// Cannot create access
		FormatError(M4_CH_WO_CANT_CREATE_ACCESS, "Unable to create new access object");
		LEAVE_METHOD(CWrapIMap::GetAccess, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapIMap::get_Count(long* pVal)
{
	ENTER_METHOD(CWrapIMap::get_Count);

	*pVal = m_poLevel2->m_oInstanceMap.Count();

	TRACE2("Returning %d", *pVal);

	LEAVE_METHOD(CWrapIMap::get_Count, S_OK);
}

STDMETHODIMP CWrapIMap::InsertL2Channel(BSTR ai_pstInstance, IWrapChannel* ai_pChannel, short* ao_RetVal)
{
	ENTER_METHOD(CWrapIMap::InsertL2Channel);

	gstring tmpGstring(ai_pstInstance);

	if (tmpGstring)
	{
		ClChannel* pChannel = 0;

		((CWrapChannel*)ai_pChannel)->Unwrap(&pChannel);

		if (pChannel)
		{
			*ao_RetVal = m_poLevel2->InsertL2Channel(tmpGstring, pChannel->GetHandle());
				
			LEAVE_METHOD(CWrapIMap::InsertL2Channel, S_OK);
		}
	}

	*ao_RetVal = M4_ERROR;

	LEAVE_METHOD(CWrapIMap::InsertL2Channel, S_OK);
}

STDMETHODIMP CWrapIMap::InsertL2Access(BSTR ai_pstInstance, IWrapAccess* ai_pAccess, VARIANT_BOOL ai_bIsOwner, short* ao_RetVal)
{
	ENTER_METHOD(CWrapIMap::InsertL2Access);

	m4bool_t bIsOwner;
	gstring tmpGstring(ai_pstInstance);

	if (tmpGstring)
	{
		bIsOwner = (ai_bIsOwner == VARIANT_TRUE) ? M4_TRUE : M4_FALSE;

		ClAccess* pAccess = 0;

		((CWrapAccess*)ai_pAccess)->Unwrap(&pAccess);

		if (pAccess)
		{
			*ao_RetVal = m_poLevel2->InsertL2Access(tmpGstring, pAccess, bIsOwner);

			LEAVE_METHOD(CWrapIMap::InsertL2Access, S_OK);
		}
	}

	*ao_RetVal = M4_ERROR;

	LEAVE_METHOD(CWrapIMap::InsertL2Access, S_OK);
}

STDMETHODIMP CWrapIMap::EraseL2Instance(BSTR ai_pstInstance, VARIANT_BOOL ai_bDeleteChannel, short* ao_RetVal)
{
	ENTER_METHOD(CWrapIMap::EraseL2Instance);

	m4bool_t bDeleteChannel;
	gstring tmpGstring(ai_pstInstance);

	if (tmpGstring)
	{
		bDeleteChannel = (ai_bDeleteChannel == VARIANT_TRUE) ? M4_TRUE : M4_FALSE;

		*ao_RetVal = m_poLevel2->EraseL2Instance(tmpGstring, bDeleteChannel);
				
		LEAVE_METHOD(CWrapIMap::EraseL2Instance, S_OK);
	}

	*ao_RetVal = M4_ERROR;

	LEAVE_METHOD(CWrapIMap::EraseL2Instance, S_OK);
}

STDMETHODIMP CWrapIMap::GetChannelByInstance(BSTR ai_pstInstance, IWrapChannel** pVal, short* ao_RetVal)
{
	ENTER_METHOD(CWrapIMap::GetChannelByInstance);

	ClChannel * pChannel = 0;
	ClAccess * pAccess = 0;
	gstring tmpGstring(ai_pstInstance);

	if (tmpGstring)
	{
		*ao_RetVal = m_poLevel2->m_oInstanceMap.Find(tmpGstring, &pAccess,&pChannel);

		if (M4_SUCCESS == *ao_RetVal)
		{
			if (NewInterface(CLSID_WrapChannel, IID_IWrapChannel, (IUnknown**)pVal) != M4_SUCCESS)
			{
				*ao_RetVal = M4_ERROR;
				LEAVE_METHOD(CWrapExecutor::GetChannelByInstance, S_OK);
			}

			((CWrapChannel*)(*pVal))->Init(pChannel);
		}
	}

	LEAVE_METHOD(CWrapIMap::GetChannelByInstance, S_OK);
}

STDMETHODIMP CWrapIMap::EraseAllL2Instances(short* ao_RetVal)
{
	ENTER_METHOD(CWrapIMap::EraseAllL2Instances);
	
	*ao_RetVal = m_poLevel2->EraseAllL2Instances(M4_TRUE, M4_TRUE, NULL);

	LEAVE_METHOD(CWrapIMap::EraseAllL2Instances, S_OK);
}
