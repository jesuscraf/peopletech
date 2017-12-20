//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapslice.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapSlice
//
//==============================================================================
#include "resource.h"       // main symbols
#include "index.hpp"

#ifndef __WRAPSLICE_H_
#define __WRAPSLICE_H_

class ClValue;

/////////////////////////////////////////////////////////////////////////////
// CWrapSlice
class ATL_NO_VTABLE CWrapSlice : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapSlice, &CLSID_WrapSlice>,
	public IDispatchImpl<IWrapSlice, &IID_IWrapSlice, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapSlice();
	~CWrapSlice();

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPSLICE)

BEGIN_COM_MAP(CWrapSlice)
	COM_INTERFACE_ENTRY(IWrapSlice)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWrapSlice
public:
	STDMETHOD(Init)(/*[in]*/ VARIANT ai_varRef);

	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);

	STDMETHOD(Delete)(/*[out, retval]*/ short* ao_sRetVal);

	STDMETHOD(get_StartDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_EndDate)(/*[in]*/ DATE ai_date);

	STDMETHOD(get_EndDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_StartDate)(/*[in]*/ DATE ai_date);

private:
	ClValue* m_poValue;
	ClSliceIndex m_oIndex;
};

#endif //__WRAPSLICE_H_
