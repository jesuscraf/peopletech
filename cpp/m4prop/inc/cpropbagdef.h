// CPropBagDef.h : Declaration of the CCPropBagDef

#ifndef __CPROPBAGDEF_H_
#define __CPROPBAGDEF_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCPropBagDef
class ATL_NO_VTABLE CCPropBagDef : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCPropBagDef, &CLSID_CPropBagDef>,
	public ISupportErrorInfo,
	public IDispatchImpl<ICPropBagDef, &IID_ICPropBagDef, &LIBID_M4PROPLib>
{
public:
	CCPropBagDef()
	{
		CComObject<CCPropColDef> * pColDef = NULL;
		CComObject<CCPropColDef>::CreateInstance(&pColDef);
		m_PropCol = pColDef;
		m_PropCol->AddRef();
		
		CComObject<CCChildColDef> * pChildDef = NULL;
		CComObject<CCChildColDef>::CreateInstance(&pChildDef);
		pChildDef->QueryInterface(IID_ICChildColDef, (void**)&m_ChildCol);
		m_strClassName = NULL;
		m_strAlias = NULL;
		m_strKey = NULL;
		m_nClassNameLength = 0;
		m_nAliasLength = 0;
		GetUniqueKey(&m_strKey);
	}

	~CCPropBagDef()
	{
		RELEASE(m_PropCol);
		RELEASE(m_ChildCol);
		free(m_strAlias);
		m_strAlias = NULL;
		free(m_strClassName);
		m_strClassName = NULL;
		free(m_strKey);
		m_strKey = NULL;
	}

	DefProp * CCPropBagDef::GetDefProp(long nId);

DECLARE_REGISTRY_RESOURCEID(IDR_CPROPBAGDEF)

BEGIN_COM_MAP(CCPropBagDef)
	COM_INTERFACE_ENTRY(ICPropBagDef)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

protected:
	long m_nLength;

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ICPropBagDef
protected:
public:
	STDMETHOD(SolveTranslation)();
	STDMETHOD(ReplaceChild)(/*[in]*/ ICPropBagDef * pOld, /*[in]*/ ICPropBagDef * pNew);
	STDMETHOD(Reset)();
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	void ChangeAlias(char * strAlias);
	void ChangeClassName(char * strClassName);
	char * m_strClassName;
	char * m_strAlias;
	char * m_strKey;
	short m_nClassNameLength, m_nAliasLength;
	CCPropColDef  * m_PropCol;
	ICChildColDef * m_ChildCol;
	DefProp * GetDefProp(const char * key);
	STDMETHOD(get_ChildPBDefCountRec)(/*[out, retval]*/ long *pVal);
	STDMETHOD(Property)(/*[in, out]*/ BSTR PropertyName, /*[out, retval]*/ VARIANT * vReturn);
	STDMETHOD(get_ChildColDef)(/*[out, retval]*/ ICChildColDef **pVal);
	STDMETHOD(put_ChildColDef)(/*[out, retval]*/ ICChildColDef *pVal);
	STDMETHOD(get_PropColDef)(/*[out, retval]*/ ICPropColDef **pVal);
	STDMETHOD(AddProperty)(/*[in]*/ BSTR bstrName, /*[in]*/ VARIANT vValue, /*[in]*/ VARIANT vDefValue, /*[in, optional, defaultvalue(0)]*/ M4PropertyType sType);
	STDMETHOD(AddChild)(/*[in]*/ ICPropBagDef ** pbdChild, /*[in]*/ VARIANT vIndex);
	STDMETHOD(get_ClassName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ClassName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Alias)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Alias)(/*[in]*/ BSTR newVal);
	STDMETHOD(putref_ChildColDef)(/*[out, retval]*/ ICChildColDef *pVal);
	
};




#endif //__CPROPBAGDEF_H_
