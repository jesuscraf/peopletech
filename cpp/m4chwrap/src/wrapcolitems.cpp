//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapcolitems.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapColItems
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapcolitems.h"
#include "newinterface.h"
#include "m4types.hpp"
#include "variantref.h"
#include "wrapdef.h"
#include "gstring.h"
#include "node.hpp"

/////////////////////////////////////////////////////////////////////////////
// CWrapColItems
CWrapColItems::CWrapColItems()
	: m_poItem(0)
{
}

CWrapColItems::~CWrapColItems()
{
	m_poItem = 0;
}

STDMETHODIMP CWrapColItems::Count(int* ao_count)
{
	ENTER_METHOD(CWrapColItems::Count);

	*ao_count = m_poItem->Count();

	TRACE2("Setting return value to %d", *ao_count);

	LEAVE_METHOD(CWrapColItems::Count, S_OK);
}

STDMETHODIMP CWrapColItems::GetItemByName(BSTR ai_name, IWrapItem** ao_item)
{
	ENTER_METHOD(CWrapColItems::GetItemByName);

	if (NewInterface(CLSID_WrapItem, IID_IWrapItem, (IUnknown**)ao_item) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapColItems::GetItemByName, E_UNEXPECTED);
	}
	else
	{
		gstring tmpGstring(ai_name);

		if (tmpGstring)
		{
            //Edu:
			m4bool_t noteof = m_poItem->MoveTo(tmpGstring);

			if (noteof)
			{
				ClVariantRef oVarRef((void*) m_poItem);

				(*ao_item)->Init(oVarRef);

				LEAVE_METHOD(CWrapColItems::GetItemByName, S_OK);
			}
		}

		LEAVE_METHOD(CWrapColItems::GetItemByName, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapColItems::GetItemByPos(int ai_pos, IWrapItem** ao_item)
{
	ENTER_METHOD(CWrapColItems::GetItemByPos);

    //Edu:
	m4bool_t noteof = m_poItem->MoveTo((ClItemIndex)ai_pos);

	if (noteof)
	{
		if (NewInterface(CLSID_WrapItem, IID_IWrapItem, (IUnknown**)ao_item) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapColItems::GetItemByPos, E_UNEXPECTED);
		}

		ClVariantRef oVarRef((void*) m_poItem);

		(*ao_item)->Init(oVarRef);
		
		LEAVE_METHOD(CWrapColItems::GetItemByPos, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapColItems::GetItemByPos, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapColItems::Init(VARIANT ai_VarRef)
{
	ClVariantRef oVarRef(ai_VarRef);

	m_poItem = (ClItem*)(void*)oVarRef;

	TRACE2("Address of item is %p", m_poItem);
	return S_OK;
}
