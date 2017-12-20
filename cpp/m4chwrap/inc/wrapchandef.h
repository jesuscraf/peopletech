//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapchandef.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapChanDef
//
//==============================================================================
#include "resource.h"       // main symbols

#include "iwrapcolnodedef.h"

#ifndef __WRAPCHANDEF_H_
#define __WRAPCHANDEF_H_

class ClChannelDef;

/////////////////////////////////////////////////////////////////////////////
// CWrapChanDef
class ATL_NO_VTABLE CWrapChanDef : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapChanDef, &CLSID_WrapChanDef>,
	public IDispatchImpl<IWrapChanDef, &IID_IWrapChanDef, &LIBID_CHANNELWRAPPERLib>,

    public CIWrapColNodeDef
{
public:
	CWrapChanDef();
	~CWrapChanDef();
	void Init(ClChannelDef* ai_poChanDef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPCHANDEF)

BEGIN_COM_MAP(CWrapChanDef)
	COM_INTERFACE_ENTRY(IWrapChanDef)
    COM_INTERFACE_ENTRY2(IDispatch, IWrapChanDef)

//interface de colección de nodos:
    COM_INTERFACE_ENTRY(IEnumVARIANT)
    COM_INTERFACE_ENTRY(IWrapColNodeDef)
    COM_INTERFACE_ENTRY2(IUnknown, IEnumVARIANT)
    COM_INTERFACE_ENTRY2(IDispatch, IWrapColNodeDef)
END_COM_MAP()

// IWrapChanDef
public:
	STDMETHOD(get_CreationType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_InheritLevel)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_OrgType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_InstanceOrgTypeByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_InstanceOrganizationByPosition)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_CheckConcurrency)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_OwnerFlag)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_IsMDCacheable)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_InstanceRSMByPosition)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ParameterItem)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ParameterNode)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfParameters)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_CacheMaxPeriod)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_IsCacheable)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ServiceAltId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ServiceId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_EnvironmentVars)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_HasSecurity)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_InstanceCsAccessByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_InstanceOpenModeByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_InstanceScopeByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecExecute)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecUpdate)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecDelete)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecWrite)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecRead)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_VMCapacityReq)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_CsExeType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_IsSeparable)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_CsType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Build)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_TimeUnit)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_RootIdByPosition)(/*[in]*/ short ai_pos, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfRoots)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_GetInstanceT3ByPosition)(/*[in]*/ short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetInstanceIdByPosition)(/*[in]*/ short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfInstances)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Id)(/*[out, retval]*/ BSTR *pVal);

	STDMETHOD(get_IsExternal)(/* [retval][out] */ BYTE *pVal);
	STDMETHOD(get_OleClassGuid)(/* [retval][out] */ BSTR *pVal);

//interface de colección de nodos:
    STDMETHODIMP get_ColNodeDef(IWrapColNodeDef** pVal);
    

private:
	ClChannelDef* m_poChanDef;
};

#endif //__WRAPCHANDEF_H_
