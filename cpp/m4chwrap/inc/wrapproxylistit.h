
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             WrapProxyListIt.h
// Project:          ProxyListIt Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapProxyListIt
//
//==============================================================================
#include "resource.h"       // main symbols

#ifndef __WRAPPROXYLISTIT_H_
#define __WRAPPROXYLISTIT_H_

class ClProxyList;

/////////////////////////////////////////////////////////////////////////////
// CWrapProxyListIt
class ATL_NO_VTABLE CWrapProxyListIt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapProxyListIt, &CLSID_WrapProxyListIt>,
	public IDispatchImpl<IWrapProxyListIt, &IID_IWrapProxyListIt, &LIBID_CHANNELWRAPPERLib>,
	public ISupportErrorInfo
{
public:
	CWrapProxyListIt();
	~CWrapProxyListIt();
	
	void Init(ClProxyList* ai_poProxyList);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPPROXYLISTIT)

BEGIN_COM_MAP(CWrapProxyListIt)
	COM_INTERFACE_ENTRY(IWrapProxyListIt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

public:

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = { &IID_IWrapProxyListIt };

		for (int i=0; i < sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i],riid))
			{
				return S_OK;
			}
		}
		return S_FALSE;
	}

	// IWrapProxyListIt
	STDMETHOD(Item)(/* [in] */ long ai_iIndex, /* [retval][out] */ IWrapProxyListIt** ao_item);
    STDMETHOD(get_Count)(/* [retval][out] */ long* pVal);
    STDMETHOD(get_Child)(/* [retval][out] */ IWrapProxyListIt** pVal);
	STDMETHOD(IsLeaf)(/* [retval][out] */ VARIANT_BOOL* pVal);
    STDMETHOD(get_Id)(/* [retval][out] */ BSTR* pVal);
    STDMETHOD(get_LocalUniqueId)(/* [retval][out] */ BSTR* pVal);
    STDMETHOD(get_RemoteUniqueId)(/* [retval][out] */ BSTR* pVal);

private:
	ClProxyList* m_poProxyList;
};

#endif //__WRAPProxyListIt_H_
