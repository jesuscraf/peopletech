//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             enumitem.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of Cenumitem
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "enumitem.h"
#include "variantref.h"
#include "newinterface.h"
#include "item.hpp"
#include "m4types.hpp"
#include "wrapdef.h"


/////////////////////////////////////////////////////////////////////////////
// Cenumitem

STDMETHODIMP Cenumitem::Init(VARIANT ai_var)
{
	ClVariantRef oVarRef(ai_var);

	m_poItem = (ClItem*)(void*)oVarRef;

	Reset();

	return S_OK;
}

STDMETHODIMP Cenumitem::Next(ULONG celt, VARIANT* rgvar, ULONG* pceltFetched)
{
	ENTER_METHOD(Cenumitem::Next);

	m4uint32_t l,l1,l2;
	m4uint32_t Count = m_poItem->Count();

	if (pceltFetched != NULL)
	{
		*pceltFetched = 0;
	}

	for (l=0; l< celt; l++)
	{
		VariantInit(&rgvar[l]);
	}

	// Retrieve the next cElements elements.
	for (l1 = m_lCurrent, l2 = 0; 
		l1 < Count && l2 < celt; 
		l1++, l2++)	
	{
		m_poItem->MoveTo((ClItemIndex)l1);
//???
		rgvar[l2].vt = VT_UNKNOWN;// | VT_BYREF;
//		rgvar[l2].vt = VT_DISPATCH;// | VT_BYREF;
//		rgvar[l2].vt = VT_I4;		

		NewInterface(CLSID_WrapItem,
			IID_IWrapItem,
			&(rgvar[l2].punkVal)); //ppunkVal

//		rgvar[l2].lVal = l1; 		

		ClVariantRef oVarRef((void*) m_poItem);

		((IWrapItem*)(rgvar[l2].punkVal))->Init(oVarRef);

  }
	
	// Set count of elements retrieved.
	if (pceltFetched != NULL)
	{
		*pceltFetched = l2;	
	}

	m_lCurrent = l1;

	LEAVE_METHOD(Cenumitem::Next, (l2 < celt) ? S_FALSE : S_OK);
}

STDMETHODIMP Cenumitem::Skip(ULONG celt)
{
	ENTER_METHOD(Cenumitem::Skip);

	m4uint32_t Count = m_poItem->Count();

	m_lCurrent += celt;

	if (m_lCurrent > Count)
	{
		m_lCurrent = Count;

		LEAVE_METHOD(Cenumitem::Skip, S_FALSE);
	}

	LEAVE_METHOD(Cenumitem::Skip, S_OK);
}

STDMETHODIMP Cenumitem::Reset()
{
	ENTER_METHOD(Cenumitem::Reset);

	m_lCurrent = 0;

	LEAVE_METHOD(Cenumitem::Reset, S_OK);
}

STDMETHODIMP Cenumitem::Clone(IEnumVARIANT** ppenum)
{
	ENTER_METHOD(Cenumitem::Clone);

	NewInterface(CLSID_enumitem, IID_IEnumVARIANT, (IUnknown**)ppenum);

	ClVariantRef oVarRef((void*) ppenum);

	((Ienumitem*)(*ppenum))->Init(oVarRef);
	(*ppenum)->Skip(m_lCurrent);

	LEAVE_METHOD(Cenumitem::Clone, S_OK);
}
