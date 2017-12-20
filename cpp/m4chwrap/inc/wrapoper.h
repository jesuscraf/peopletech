//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapoper.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapOper
//
//==============================================================================
#include "resource.h"       // main symbols

#ifndef __WRAPOPER_H_
#define __WRAPOPER_H_

class _ClOperation_Base;

/////////////////////////////////////////////////////////////////////////////
// CWrapOper
class ATL_NO_VTABLE CWrapOper : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapOper, &CLSID_WrapOper>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapOper, &IID_IWrapOper, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapOper();
	~CWrapOper();

	void Init(_ClOperation_Base* ai_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPOPER)

BEGIN_COM_MAP(CWrapOper)
	COM_INTERFACE_ENTRY(IWrapOper)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IWrapOper
public:
	STDMETHOD(get_IsModified)(/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(get_IsUpdate)(/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(get_IsInsert)(/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(get_IsDelete)(/*[out, retval]*/ VARIANT_BOOL* pVal);

	STDMETHOD(get_IsModified_DB)(/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(get_IsUpdate_DB)(/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(get_IsInsert_DB)(/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(get_IsDelete_DB)(/*[out, retval]*/ VARIANT_BOOL* pVal);

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	_ClOperation_Base* m_pOperation;
};

#endif //__WRAPOPER_H_
