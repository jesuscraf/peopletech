
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapcolprop.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapColProp
//
//==============================================================================
#include "resource.h"       // main symbols

#ifndef __WRAPCOLPROP_H_
#define __WRAPCOLPROP_H_

class ClColProp;

/////////////////////////////////////////////////////////////////////////////
// CWrapColProp
class ATL_NO_VTABLE CWrapColProp : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapColProp, &CLSID_WrapColProp>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapColProp, &IID_IWrapColProp, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapColProp();
	~CWrapColProp();

	void Init(ClColProp* ai_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPCOLPROP)

BEGIN_COM_MAP(CWrapColProp)
	COM_INTERFACE_ENTRY(IWrapColProp)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IWrapColProp
public:
	STDMETHOD(SetValue)(/*[in]*/ IWrapChannel* ai_poChannel, /*[in]*/ BSTR ai_stName, /*[in]*/ VARIANT ai_oVariant, /*[out]*/ VARIANT* ao_poVariant, /*[out]*/ ULONGLONG* ao_plOldTime, /*[out]*/ ULONGLONG* ao_plNewTime, /*[out, retval]*/ short* ao_piReturn);
	STDMETHOD(SetPropValue)(/*[in]*/ BSTR ai_stName, /*[in]*/ VARIANT ai_oVariant);
	STDMETHOD(GetPropValueById)(/*[in]*/ int ai_iId, /*[out,retval]*/ VARIANT* ao_poVariant);
	STDMETHOD(GetPropValue)(/*[in]*/ BSTR ai_stName, /*[out, retval]*/ VARIANT* ao_poVariant);
	STDMETHOD(GetPropName)(/*[in]*/ int ai_iId, /*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(Count)(/*[out, retval]*/ int* ao_iCount);

// ISupportsErrorInfo
public:
	STDMETHOD(get_GetPropFlagsById)(int ai_iId, short ai_iFlag, /*[out, retval]*/ short *pVal);
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	ClColProp* m_poColProp;
};

#endif //__WRAPCOLPROP_H_
