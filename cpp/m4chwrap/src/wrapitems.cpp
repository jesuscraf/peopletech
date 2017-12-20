//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapitems.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapItems
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapitems.h"
#include "m4types.hpp"
#include "newinterface.h"
#include "variantref.h"
#include "item.hpp"
#include "gstring.h"
#include "wrapdef.h"


/////////////////////////////////////////////////////////////////////////////
// CWrapItems

CWrapItems::CWrapItems()
	: m_poItem(0)
{
}

CWrapItems::~CWrapItems()
{
	m_poItem = 0;
}

void CWrapItems::GetItemByName(BSTR ai_name, IWrapItem** ao_item)
{
	NewInterface(CLSID_WrapItem, IID_IWrapItem, (IUnknown**)ao_item);

	gstring tmpGstring(ai_name);

    //Edu:
	m4bool_t noteof = m_poItem->MoveTo(tmpGstring);

	if (noteof)
	{
		ClVariantRef oVarRef((void*) m_poItem);
		(*ao_item)->Init(oVarRef);
	}
}

void CWrapItems::GetItemByPos(int ai_pos, IWrapItem** ao_item)
{	
    //Edu:
	m4bool_t noteof = m_poItem->MoveTo((ClItemIndex)ai_pos);

	if (noteof)
	{
		NewInterface(CLSID_WrapItem, IID_IWrapItem, (IUnknown**)ao_item);

		ClVariantRef oVarRef((void*) m_poItem);
		(*ao_item)->Init(oVarRef);		
	}
}

STDMETHODIMP CWrapItems::get_Count(long * pVal)
{
	ENTER_METHOD(CWrapItems::get_Count);

	*pVal = m_poItem->Count();

	LEAVE_METHOD(CWrapItems::get_Count, S_OK);
}

STDMETHODIMP CWrapItems::Item(VARIANT ai_var, IWrapItem** ao_item)
{
	ENTER_METHOD(CWrapItems::Item);

	switch (ai_var.vt)
	{
	case VT_BSTR:
		GetItemByName(ai_var.bstrVal, ao_item);
		break;

	case VT_UI1:
		GetItemByPos(ai_var.bVal, ao_item);
		break;

	case VT_I2:
		GetItemByPos(ai_var.iVal, ao_item);
		break;

	case VT_I4:
		GetItemByPos(ai_var.lVal, ao_item);
		break;

	default:
		LEAVE_METHOD(CWrapItems::Item, E_UNEXPECTED);
	}

	LEAVE_METHOD(CWrapItems::Item, S_OK);
}

STDMETHODIMP CWrapItems::_NewEnum(IUnknown** ao_unk)
{
	ENTER_METHOD(CWrapItems::_NewEnum);

	NewInterface(CLSID_enumitem, IID_IUnknown, (IUnknown**)ao_unk);

	ClVariantRef oVarRef((void*) m_poItem);
	((Ienumitem*)(*ao_unk))->Init(oVarRef);
	
	LEAVE_METHOD(CWrapItems::_NewEnum, S_OK);
}

STDMETHODIMP CWrapItems::Init(VARIANT ai_varref)
{
	ClVariantRef oVarRef(ai_varref);

	m_poItem = (ClItem*)(void*)oVarRef;
	return S_OK;
}

