// WrapBookMark.h : Declaration of the CWrapBookMark

#ifndef __WRAPBOOKMARK_H_
#define __WRAPBOOKMARK_H_

#include "resource.h"       // main symbols

#include "bookmark.hpp"

/////////////////////////////////////////////////////////////////////////////
// CWrapBookMark
class ATL_NO_VTABLE CWrapBookMark : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapBookMark, &CLSID_WrapBookMark>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapBookMark, &IID_IWrapBookMark, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapBookMark()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPBOOKMARK)

BEGIN_COM_MAP(CWrapBookMark)
	COM_INTERFACE_ENTRY(IWrapBookMark)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IWrapBookMark
public:
	STDMETHOD(ApplyTo)(IWrapAccess* ai_piAccess, short* ao_sRetVal);
	STDMETHOD(ApplyToBranch)(IWrapAccess* ai_piAccess, BSTR ai_sCutNode, BSTR * ai_sLast, short* ao_sRetVal);
	STDMETHOD(GetFrom)(IWrapAccess* ai_piAccess, short* ao_sRetVal);
	STDMETHOD(GetFrom_PK)(IWrapAccess* ai_piAccess, short* ao_sRetVal);
	STDMETHOD(SerializeBranch)  (/*[in]*/IWrapChannel* ai_piChannel, /*[in]*/IWrapAccess* ai_piAccess, /*[in]*/BSTR ai_sNodeId, /*[out]*/BSTR *ao_sSerialized, /*[out, retval]*/short* ao_sRetVal);
	STDMETHOD(DeserializeBranch)(/*[in]*/IWrapChannel* ai_piChannel, /*[in]*/BSTR ai_sSerialized, /*[out, retval]*/short* ao_sRetVal);

	void GetFrom (ClAccess &ai_poAccess);
	void GetFrom_PK (ClAccess &ai_poAccess);

private:
    ClAccessBookMark m_BookMark;
};

#endif //__WRAPBOOKMARK_H_
