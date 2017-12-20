//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             WrapIMap.h  
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             04/12/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Declaration of the CWrapIMap
//
//==============================================================================

#include "resource.h"       // main symbols

#ifndef __WRAPIMAP_H_
#define __WRAPIMAP_H_

class ClLevel2;

/////////////////////////////////////////////////////////////////////////////
// CWrapIMap
class ATL_NO_VTABLE CWrapIMap : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapIMap, &CLSID_WrapIMap>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapIMap, &IID_IWrapIMap, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapIMap();
	void Init(ClLevel2* ai_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPIMAP)

BEGIN_COM_MAP(CWrapIMap)
	COM_INTERFACE_ENTRY(IWrapIMap)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IWrapIMap
public:
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
	STDMETHOD(GetAccess)(/*[in]*/ long ai_pos, /*[out, retval]*/ IWrapAccess** ao_access);
	STDMETHOD(GetChannelName)(/*[in]*/ long ai_pos, /*[out, retval]*/ BSTR* ao_pst);
	STDMETHOD(GetInstance)(/*[in]*/ long ai_pos, /*[out, retval]*/ BSTR* ao_pst);
	STDMETHOD(InsertL2Channel)(/*[in]*/ BSTR ai_pstInstance, /*[in]*/ IWrapChannel* ai_pChannel, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(InsertL2Access)(/*[in]*/ BSTR ai_pstInstance, /*[in]*/ IWrapAccess* ai_pAccess, /*[in]*/ VARIANT_BOOL ai_bIsOwner, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(EraseL2Instance)(/*[in]*/ BSTR ai_pstInstance, /*[in]*/ VARIANT_BOOL ai_bDeleteChannel, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(GetChannelByInstance)(/*[in]*/ BSTR ai_pstInstance, /*[out]*/IWrapChannel** pVal, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(EraseAllL2Instances)(/*[out, retval]*/ short* ao_RetVal);
	// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	ClLevel2* m_poLevel2;
};

#endif //__WRAPIMAP_H_
