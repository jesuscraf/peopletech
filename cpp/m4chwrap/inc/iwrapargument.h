// WrapArgument.h : Declaration of the CIWrapArgument

#ifndef __WRAPARGUMENT_H_
#define __WRAPARGUMENT_H_

#include "resource.h"       // main symbols

class _AClArgument;

/////////////////////////////////////////////////////////////////////////////
// CIWrapArgument
class ATL_NO_VTABLE CIWrapArgument : 
	//public CComObjectRootEx<CComSingleThreadModel>,
	//public CComCoClass<CIWrapArgument, &CLSID_WrapArgument>,
	public IDispatchImpl<IWrapArgument, &IID_IWrapArgument, &LIBID_CHANNELWRAPPERLib>
{
public:
	CIWrapArgument();

/*
DECLARE_REGISTRY_RESOURCEID(IDR_WRAPARGUMENT)

BEGIN_COM_MAP(CIWrapArgument)
	COM_INTERFACE_ENTRY(IWrapArgument)
	COM_INTERFACE_ENTRY(IDispatch)
	//COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
*/

// IWrapArgument
public:
	STDMETHOD(get_ArgumentName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);

    void Init (_AClArgument *m_parg);

private:    
    _AClArgument *m_poArg;
};

#endif //__WRAPARGUMENT_H_
