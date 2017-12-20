
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapregister.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapRegister
//
//==============================================================================
#include "resource.h"       // main symbols
#include "index.hpp"

#ifndef __WRAPREGISTER_H_
#define __WRAPREGISTER_H_

class ClRegister;

/////////////////////////////////////////////////////////////////////////////
// CWrapRegister
class ATL_NO_VTABLE CWrapRegister : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapRegister, &CLSID_WrapRegister>,
	public IDispatchImpl<IWrapRegister, &IID_IWrapRegister, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapRegister();
	~CWrapRegister();

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPREGISTER)

BEGIN_COM_MAP(CWrapRegister)
	COM_INTERFACE_ENTRY(IWrapRegister)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWrapRegister
public:
	STDMETHOD(get_ItemIterator)(/*[out, retval]*/ IWrapItIt* *pVal);
	STDMETHOD(get_WrapItems)(/*[out, retval]*/ IWrapItems* *pVal);
	STDMETHOD(Free)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_Index)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Operation)(/*[out, retval]*/ IWrapOper* *pVal);
	STDMETHOD(get_IsEof)(/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(Delete)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_CoItems)(/*[out, retval]*/ IWrapColItems* *pVal);
	STDMETHOD(Unwrap)(/*[out, retval]*/ VARIANT* ao_VarRef);
	STDMETHOD(Init)(/*[in]*/ VARIANT ai_VarRef);
    STDMETHOD(SendRegisterMark)(/*[in]*/VARIANT_BOOL* pVal);
private:
	ClRegister* m_poRegister;
	ClRegisterIndex m_oIndex;
	IWrapItIt* m_poItemIterator;
};

#endif //__WRAPREGISTER_H_
