// PersistObject.h : Declaration of the CPersistObject

#ifndef __PERSISTOBJECT_H_
#define __PERSISTOBJECT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPersistObject
class ATL_NO_VTABLE CPersistObject : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPersistObject, &CLSID_PersistObject>,
	public IDispatchImpl<IPersistObject, &IID_IPersistObject, &LIBID_M4PROPLib>
{
public:
	CPersistObject()
	{
		m_strMain = NULL;
		HRESULT hr = CoCreateInstance(CLSID_TraductProp, NULL, CLSCTX_INPROC_SERVER, IID_ITraductProp, (void**)&m_pTraduct);
		if (hr != S_OK)
		{
			m_pTraduct = NULL;
		}
	}

	~CPersistObject()
	{
		RELEASE_BSTR(m_strMain);
		RELEASE(m_pTraduct);
	}


DECLARE_REGISTRY_RESOURCEID(IDR_PERSISTOBJECT)

BEGIN_COM_MAP(CPersistObject)
	COM_INTERFACE_ENTRY(IPersistObject)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IPersistObject
public:
	STDMETHOD(get_PersistMain)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_PersistMain)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_PersistTraductProp)(/*[out, retval]*/ ITraductProp * *pVal);
	STDMETHOD(putref_PersistTraductProp)(/*[in]*/ ITraductProp *newVal);

public:
	ITraductProp * m_pTraduct;
	BSTR m_strMain;
};

#endif //__PERSISTOBJECT_H_
