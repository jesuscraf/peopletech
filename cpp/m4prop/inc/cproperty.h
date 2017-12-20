// CProperty.h : Declaration of the CCProperty

#ifndef __CPROPERTY_H_
#define __CPROPERTY_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCProperty
class ATL_NO_VTABLE CCProperty : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCProperty, &CLSID_CProperty>,
	public IDispatchImpl<ICProperty, &IID_ICProperty, &LIBID_M4PROPLib>
{
public:
	CCProperty()
	{
		m_strName = ::SysAllocString(L"");
		m_strGroup = ::SysAllocString(L"");
		VariantInit(&m_vValue);
		VariantInit(&m_vDefValue);
		m_iType = FE_NOTDEFINED;
		m_bEditable = FALSE;
		ATLTRACE("Creación de CProperty\n");
	}
	~CCProperty()
	{
		::SysFreeString(m_strName);
		::SysFreeString(m_strGroup);
		VariantClear(&m_vValue);
		ATLTRACE("Destrucción de CProperty\n");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CPROPERTY)

BEGIN_COM_MAP(CCProperty)
	COM_INTERFACE_ENTRY(ICProperty)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICProperty
public:
	STDMETHOD(get_DefValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_DefValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Group)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Group)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Editable)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Editable)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ M4PropertyType *pVal);
	STDMETHOD(put_Type)(/*[in]*/ M4PropertyType newVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
protected:
	BSTR	m_strName;
	BSTR    m_strGroup;
	VARIANT m_vValue;
	M4PropertyType	m_iType;
	BOOL    m_bEditable;
	VARIANT m_vDefValue;
};

#endif //__CPROPERTY_H_
