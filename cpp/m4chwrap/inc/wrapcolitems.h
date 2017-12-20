
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapcolitems.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapAccess
//
//==============================================================================
#include "resource.h"       // main symbols

#ifndef __WRAPCOLITEMS_H_
#define __WRAPCOLITEMS_H_

class ClItem;

/////////////////////////////////////////////////////////////////////////////
// CWrapColItems
class ATL_NO_VTABLE CWrapColItems : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapColItems, &CLSID_WrapColItems>,
	public IDispatchImpl<IWrapColItems, &IID_IWrapColItems, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapColItems();
	~CWrapColItems();

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPCOLITEMS)

BEGIN_COM_MAP(CWrapColItems)
	COM_INTERFACE_ENTRY(IWrapColItems)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWrapColItems
public:
	STDMETHOD(Init)(/*[in]*/ VARIANT ai_VarRef);
	STDMETHOD(GetItemByPos)(/*[in]*/ int ai_pos, /*[out, retval]*/ IWrapItem** ao_item);
	STDMETHOD(GetItemByName)(/*[in]*/ BSTR ai_name, /*[out, retval]*/ IWrapItem** ao_item);
	STDMETHOD(Count)(/*[out, retval]*/ int* ao_count);

private:
	ClItem* m_poItem;
};

#endif //__WRAPCOLITEMS_H_
