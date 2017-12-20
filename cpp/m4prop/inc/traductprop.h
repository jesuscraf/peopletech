// TraductProp.h : Declaration of the CTraductProp

#ifndef __TRADUCTPROP_H_
#define __TRADUCTPROP_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTraductProp
class ATL_NO_VTABLE CTraductProp : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTraductProp, &CLSID_TraductProp>,
	public IDispatchImpl<ITraductProp, &IID_ITraductProp, &LIBID_M4PROPLib>
{
public:
	CTraductProp()
	{
		m_pBSTRData = NULL;
		m_nPropNum = 0;
		m_bstrProperty = NULL;
		m_bstrIdentifier = NULL;
	}
	~CTraductProp()
	{
		FreeData();
		RELEASE_BSTR(m_bstrProperty);
		RELEASE_BSTR(m_bstrIdentifier);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRADUCTPROP)

BEGIN_COM_MAP(CTraductProp)
	COM_INTERFACE_ENTRY(ITraductProp)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ITraductProp
public:
	STDMETHOD(Clone)(/*[out, retval]*/ ITraductProp * * pRetProp);
	STDMETHOD(TestEqual)(/*[out, retval]*/ VARIANT_BOOL * bRet);
	STDMETHOD(get_PropertyNumber)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Property)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Property)(/*[in]*/ BSTR newVal);
	STDMETHOD(AllValues)(/*[in]*/ BSTR newVal);
	STDMETHOD(Init)(/*[in]*/ long PropertyNumber, /*[in]*/ _Collection * pCollection);
	STDMETHOD(InitFromBSTRArray)(/*[in]*/ VARIANT newValue);
	STDMETHOD(get_ID)(/*[in]*/ long nIndex, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ValueByLng)(/*[in]*/ VARIANT vLangID, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ValueByLng)(/*[in]*/ VARIANT vLangID, /*[in]*/ BSTR newVal);
	HRESULT PutValue(int i, BSTR * newValue);
	HRESULT GetValue(int i, BSTR * pVal, bool & ao_bNullValue);

	BSTR * m_pBSTRData;
	BSTR * m_pBSTRIds;
private:
	BSTR m_bstrIdentifier;
	BSTR m_bstrProperty;
	long m_nPropNum;

	HRESULT _get_ValueByLng(VARIANT vLangID, BSTR *pVal, bool & ao_bNullValue);

public:
	STDMETHOD(ResetIdentifier)();
	STDMETHOD(CloneClean)(/*[out, retval]*/ ITraductProp ** pRet);
	STDMETHOD(get_Identifier)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Identifier)(/*[in]*/ BSTR newVal);
	void InitData(long nPropNumber);
	void FreeData();

	STDMETHOD(get_ValueByLng)(/*[in]*/ VARIANT vLangID, /*[out, retval]*/ BSTR *pVal, /*[out, retval]*/ bool & ao_bNullValue);

	HRESULT FinalConstruct()
	{
		USES_CONVERSION;

		char * pNew = NULL;
		ResetIdentifier();
		
		return S_OK;
	}

/*// ICDefProp
	STDMETHOD(get_Name)(BSTR * pVal)
	{
		if (pVal == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(put_Name)(BSTR pVal)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(get_Value)(VARIANT * pVal)
	{
		if (pVal == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(put_Value)(VARIANT pVal)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(get_DefValue)(VARIANT * pVal)
	{
		if (pVal == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(put_DefValue)(VARIANT pVal)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(get_Type)(M4PropertyType * pVal)
	{
		if (pVal == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(put_Type)(M4PropertyType pVal)
	{
		return E_NOTIMPL;
	}*/
};

#endif //__TRADUCTPROP_H_
