#ifndef __ENUMITEM_H_
#define __ENUMITEM_H_
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             enumitem.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//   Declaration of the Cenumitem
//
//==============================================================================

#include "resource.h"       // main symbols
#include "m4types.hpp"

class ClItem;

/////////////////////////////////////////////////////////////////////////////
// Cenumitem
class ATL_NO_VTABLE Cenumitem : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Cenumitem, &CLSID_enumitem>,
	public IDispatchImpl<Ienumitem, &IID_Ienumitem, &LIBID_CHANNELWRAPPERLib>,
	public IDispatchImpl<IEnumVARIANT, &IID_IEnumVARIANT, &LIBID_CHANNELWRAPPERLib>

{
public:
	Cenumitem() :
	  m_poItem(0), m_lCurrent(0)
	{
	}

	~Cenumitem()
	{
		m_poItem = 0;
		m_lCurrent = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ENUMITEM)

BEGIN_COM_MAP(Cenumitem)
	COM_INTERFACE_ENTRY(Ienumitem)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
	COM_INTERFACE_ENTRY2(IDispatch, Ienumitem)
	COM_INTERFACE_ENTRY2(IUnknown, IEnumVARIANT)
END_COM_MAP()

// Ienumitem
public:
	STDMETHOD(Init)(/*[in]*/ VARIANT ai_var);

// IEnumVARIANT
public:
	STDMETHOD(Clone)(/*[out]*/ IEnumVARIANT** ppenum);
	STDMETHOD(Reset)();
	STDMETHOD(Skip)(/*[in]*/ ULONG celt);
	STDMETHOD(Next)(/*[in]*/ ULONG celt, /*[in]*/ VARIANT* rgvar, /*[out]*/ ULONG* pceltFetched);

private:
	ClItem* m_poItem;
	m4uint32_t m_lCurrent;
};

#endif //__ENUMITEM_H_
