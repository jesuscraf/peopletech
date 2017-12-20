
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapitems.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapItems
//
//==============================================================================

#include "resource.h"       // main symbols

#ifndef __WRAPITEMS_H_
#define __WRAPITEMS_H_

class ClItem;

/////////////////////////////////////////////////////////////////////////////
// CWrapItems
class ATL_NO_VTABLE CWrapItems : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapItems, &CLSID_WrapItems>,
	public IDispatchImpl<IWrapItems, &IID_IWrapItems, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapItems();
	~CWrapItems();

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPITEMS)

BEGIN_COM_MAP(CWrapItems)
	COM_INTERFACE_ENTRY(IWrapItems)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWrapItems
public:
	STDMETHOD(Init)(/*[in]*/ VARIANT ai_varref);
	STDMETHOD(_NewEnum)(/*[out, retval]*/ IUnknown** ao_unk);
	STDMETHOD(Item)(/*[in]*/ VARIANT ai_var, /*[out,retval]*/ IWrapItem** ao_item);
	STDMETHOD(get_Count)(/*[out, retval]*/ long* pVal);

private:
	void GetItemByPos(int ai_pos, IWrapItem** ao_item);
	void GetItemByName(BSTR ai_name, IWrapItem** ao_item);

	ClItem* m_poItem;
};

#endif //__WRAPITEMS_H_
