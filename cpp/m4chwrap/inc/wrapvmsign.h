//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapvmsign.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             26/06/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of CWrapVMSign
//
//==============================================================================

#ifndef __WRAPVMSIGN_H_
#define __WRAPVMSIGN_H_

#include "resource.h"       // main symbols

class ClDMContext;

enum m4VMTypeId {
	M4CL_VMTYPE_FAT,
	M4CL_VMTYPE_SERVER,
	M4CL_VMTYPE_CLIENT,
	M4CL_VMTYPE_EMULATOR
};

/////////////////////////////////////////////////////////////////////////////
// CWrapVMSign
class ATL_NO_VTABLE CWrapVMSign : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapVMSign, &CLSID_WrapVMSign>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapVMSign, &IID_IWrapVMSign, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapVMSign() 
		: m_poDMContext(0)
	{
	}

	~CWrapVMSign()
	{
		m_poDMContext = 0;
	}

	void Init(ClDMContext* ai_poDMContext)
	{
		m_poDMContext = ai_poDMContext;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPVMSIGN)

BEGIN_COM_MAP(CWrapVMSign)
	COM_INTERFACE_ENTRY(IWrapVMSign)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IWrapVMSign
public:
	STDMETHOD(GetIsLocalServiceByID)(/*[in]*/ long ai_pos, /*[out]*/ VARIANT_BOOL* ao_fIsLocal, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(GetServiceNameByID)(/*[in]*/ long ai_id, /*[out]*/ BSTR* ao_SvcNam, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(GetServiceIDByPos)(/*[in]*/ long ai_pos, /*[out]*/ long* ao_SvcId, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_NumberOfServices)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_CSType)(/*[out, retval]*/ CSTYPE *pVal);
	STDMETHOD(get_VMCapacityName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_VMCapacityID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_VMTypeID)(/*[out, retval]*/ VMTYPE *pVal);
	STDMETHOD(get_System)(/*[out, retval]*/ long *pVal);

private:
	ClDMContext* m_poDMContext;
};

#endif //__WRAPVMSIGN_H_
