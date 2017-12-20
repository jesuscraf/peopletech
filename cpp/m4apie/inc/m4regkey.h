// M4RegKey.h : Declaration of the CM4RegKey

#ifndef __M4REGKEY_H_
#define __M4REGKEY_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CM4RegKey
//##ModelId=396B3299010A
class ATL_NO_VTABLE CM4RegKey : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4RegKey, &CLSID_M4RegKey>,
	public IDispatchImpl<IM4RegKey, &IID_IM4RegKey, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B3299029B
	CM4RegKey()
	{
		m_bstrMainKey = SysAllocString(OLESTR("NOT_DEFINED"));
		m_bstrSubKey = SysAllocString(OLESTR("NOT_DEFINED"));
	}
	
	//##ModelId=396B3299029A
	~CM4RegKey()
	{
		SysFreeString(m_bstrMainKey);
		SysFreeString(m_bstrSubKey);
	}


DECLARE_REGISTRY_RESOURCEID(IDR_M4REGKEY)

BEGIN_COM_MAP(CM4RegKey)
	COM_INTERFACE_ENTRY(IM4RegKey)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4RegKey
public:
	//##ModelId=396B32990288
	STDMETHOD(OpenKeyEx)(/*[in]*/ M4RegKeyMode openMode);
	//##ModelId=396B3299027D
	STDMETHOD(DeleteKey)(BSTR KeyName);
	//##ModelId=396B32990274
	STDMETHOD(DeleteValue)(BSTR ValueName);
	//##ModelId=396B32990272
	STDMETHOD(OpenKey)();
	//##ModelId=396B3299025E
	STDMETHOD(get_ValueName)(long nIndex, /*[out, retval]*/ BSTR *pVal);
	//##ModelId=396B3299024A
	STDMETHOD(get_Value)(BSTR bstrValueName, /*[out, retval]*/ VARIANT *pVal);
	//##ModelId=396B32990236
	STDMETHOD(put_Value)(BSTR bstrValueName, /*[in]*/ VARIANT newVal);
	//##ModelId=396B32990223
	STDMETHOD(get_ValidPosition)(/*[out, retval]*/ BOOL *pVal);
	//##ModelId=396B32990218
	STDMETHOD(get_SubKey)(/*[out, retval]*/ BSTR *pVal);
	//##ModelId=396B3299020E
	STDMETHOD(put_SubKey)(/*[in]*/ BSTR newVal);
	//##ModelId=396B32990204
	STDMETHOD(get_MainKey)(/*[out, retval]*/ BSTR *pVal);
	//##ModelId=396B329901F1
	STDMETHOD(put_MainKey)(/*[in]*/ BSTR newVal);
	//##ModelId=396B329901D3
	STDMETHOD(get_KeyName)(long nIndex, /*[out, retval]*/ BSTR *pVal);
	//##ModelId=396B329901C8
	STDMETHOD(get_GetNumberOfValues)(/*[out, retval]*/ long *pVal);
	//##ModelId=396B329901BF
	STDMETHOD(get_GetNumberOfKeys)(/*[out, retval]*/ long *pVal);
	//##ModelId=396B329901B4
	STDMETHOD(CloseKey)();
	//##ModelId=396B32990196
	STDMETHOD(CreateKey)();

// IMPLEMENTATION       JFM

protected:

	//##ModelId=396B3299018E
	BSTR m_bstrSubKey;
	//##ModelId=396B3299017A
	BSTR m_bstrMainKey;
	//##ModelId=396B32990164
	BOOL m_bValid;
	//##ModelId=396B3299015C
	CMyRegKey m_rgKey;

};

#endif //__M4REGKEY_H_
