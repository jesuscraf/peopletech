// LocalID.h: Definition of the CLocalID class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALID_H__E03EF4F0_1771_11D3_BDB1_00C04FA49636__INCLUDED_)
#define AFX_LOCALID_H__E03EF4F0_1771_11D3_BDB1_00C04FA49636__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLocalID

class CLocalID : 
	public CComDualImpl<ILocalID, &IID_ILocalID, &LIBID_M4LCIDLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CLocalID,&CLSID_LocalID>
{
public:
	CLocalID();
BEGIN_COM_MAP(CLocalID)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ILocalID)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CLocalID) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_LocalID)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	bool IsWinNT();
	int FindPosition (char *strRes, int index, unsigned char **FinalRes);
	HMODULE	instance;
// ILocalID
public:
	STDMETHOD(GetLocalInfo)(/*[in]*/ M4LocalType emlt, /*[out, retval]*/ BSTR * bstrOut);
	STDMETHOD(SetLanguage)(/*[in]*/long LCID);
	STDMETHOD(GetLanguage)(/*[out, retval]*/long *LCID);
	STDMETHOD(LoadResourceString)(/*[in]*/long IdRes, /*[in, optional]*/ long hInst,/*[out, retval]*/BSTR *resource);
	STDMETHOD(get_hInstance)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_hInstance)(/*[in]*/ long newVal);
	STDMETHOD(GetCurrentDateFormat)(/*[out, retval]*/ BSTR *dateFormat);
	STDMETHOD(SetCurrentDateFormat)(/*[in]*/ BSTR dateFormat);
	STDMETHOD(GetCurrentDate)(/*[out, retval]*/ BSTR *date);
	STDMETHOD(SetCurrentDate)(/*[in]*/ BSTR date);
};

#endif // !defined(AFX_LOCALID_H__E03EF4F0_1771_11D3_BDB1_00C04FA49636__INCLUDED_)
