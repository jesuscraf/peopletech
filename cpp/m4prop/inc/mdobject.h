// MDObject.h : Declaration of the CMDObject

#ifndef __MDOBJECT_H_
#define __MDOBJECT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMDObject
class ATL_NO_VTABLE CMDObject : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMDObject, &CLSID_MDObject>,
	public IDispatchImpl<IMDObject, &IID_IMDObject, &LIBID_M4PROPLib>
{
public:
	CMDObject()
	{
		m_bstrKey = ::SysAllocString(L"");
		m_pObject = NULL;
		m_PropBag = NULL;
	}

	~CMDObject()
	{
		::SysFreeString(m_bstrKey);
		RELEASE(m_pObject);
		RELEASE(m_PropBag);
	}



DECLARE_REGISTRY_RESOURCEID(IDR_MDOBJECT)

BEGIN_COM_MAP(CMDObject)
	COM_INTERFACE_ENTRY(IMDObject)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IMDObject
public:
	STDMETHOD(get_PropBag)(/*[out, retval]*/ ICPropBag * *pVal);
	STDMETHOD(putref_PropBag)(/*[in]*/ ICPropBag * *newVal);
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Key)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Obj)(/*[out, retval]*/ IDispatch * *pVal);
	STDMETHOD(putref_Obj)(/*[in]*/ IDispatch * *newVal);

protected:
	IDispatch	*m_pObject;
	BSTR		m_bstrKey;
	ICPropBag	*m_PropBag;

};

#endif //__MDOBJECT_H_
