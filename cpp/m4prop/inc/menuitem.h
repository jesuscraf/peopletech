// MenuItem.h : Declaration of the CMenuItem

#ifndef __MENUITEM_H_
#define __MENUITEM_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMenuItem
class ATL_NO_VTABLE CMenuItem : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMenuItem, &CLSID_MenuItem>,
	public IDispatchImpl<IMenuItem, &IID_IMenuItem, &LIBID_M4PROPLib>
{
public:
	CMenuItem()
	{
		m_strKey = NULL;
		m_strCaption = NULL;
		m_bChecked = 0;
		m_bEnabled = -1;
		m_bBold = 0;
		m_bVisible = -1;
	}
	~CMenuItem()
	{
		RELEASE_BSTR(m_strKey);
		RELEASE_BSTR(m_strCaption);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MENUITEM)

BEGIN_COM_MAP(CMenuItem)
	COM_INTERFACE_ENTRY(IMenuItem)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

private:
	BSTR m_strKey;
	BSTR m_strCaption;
	VARIANT_BOOL m_bChecked;
	VARIANT_BOOL m_bEnabled;
	VARIANT_BOOL m_bBold;
	VARIANT_BOOL m_bVisible;

// IMenuItem
public:
	STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Bold)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Bold)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Enabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Enabled)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Checked)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Checked)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Caption)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Caption)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Key)(/*[in]*/ BSTR newVal);
};

#endif //__MENUITEM_H_
