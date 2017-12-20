// m4xmlregistry.h : Declaration of the CM4XmlRegistry

#ifndef __M4XMLREGISTRY_H_
#define __M4XMLREGISTRY_H_

#include "resource.h"       // main symbols

#define UNDEFINDED_REGISTRY_HANDLE -1

/////////////////////////////////////////////////////////////////////////////
// CM4XmlRegistry
class ATL_NO_VTABLE CM4XmlRegistry : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4XmlRegistry, &CLSID_M4XmlRegistry>,
	public IDispatchImpl<IM4XmlRegistry, &IID_IM4XmlRegistry, &LIBID_M4APIELib>
{
public:
	CM4XmlRegistry()
	{
		m_bUseGlobalInstance = TRUE;
		m_iHandle = UNDEFINDED_REGISTRY_HANDLE;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_M4XMLREGISTRY)

BEGIN_COM_MAP(CM4XmlRegistry)
	COM_INTERFACE_ENTRY(IM4XmlRegistry)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4XmlRegistry
public:
	STDMETHOD(get_Handle)(/*[out, retval]*/ short * retVal);
	STDMETHOD(put_Handle)(/*[in]*/ short newVal);
	STDMETHOD(get_UseGlobalInstance)(/*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(put_UseGlobalInstance)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(LoadMachineRegistry)(/*[out, retval]*/ short * retVal);
	STDMETHOD(Load)(/*[in]*/ BSTR ai_sFile, /*[in]*/ VARIANT_BOOL ai_bIsXml, /*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(GetValue)(/*[in]*/ BSTR ai_sKey, /*[in]*/ BSTR ai_sValue, /*[out]*/ BSTR *ao_sData, /*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(SetValue)(/*[in]*/ BSTR ai_sKey, /*[in]*/ BSTR ai_sValue, /*[in]*/ BSTR ai_sData, /*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(GetValueNames)(/*[in]*/ BSTR ai_sKey, /*[out]*/ SAFEARRAY **ao_psaValues, /*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(GetSubKeyNames)(/*[in]*/ BSTR ai_sKey, /*[out]*/ SAFEARRAY **ao_psaKeys, /*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(DeleteValue)(/*[in]*/ BSTR ai_sKey, /*[in]*/ BSTR ai_sValue, /*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(DeleteKey)(/*[in]*/ BSTR ai_sKey, /*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(Merge)(/*[in]*/ short ai_iDestinationHandle, /*[out, retval]*/ VARIANT_BOOL * retVal);
	STDMETHOD(Save)(/*[in]*/ BSTR ai_sFile, /*[out, retval]*/ short * VARIANT_BOOL);
	STDMETHOD(UnLoad)(/*[out, retval]*/ short * VARIANT_BOOL);

protected:

	BOOL m_bUseGlobalInstance;
	short m_iHandle;
	HRESULT _ElementNames(BOOL ai_ReadValues, BSTR ai_sKey, SAFEARRAY ** ao_psaElements, VARIANT_BOOL * retVal);
};

#endif //__M4XMLREGISTRY_H_
