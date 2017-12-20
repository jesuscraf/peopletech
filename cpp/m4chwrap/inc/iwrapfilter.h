// WrapFilter.h : Declaration of the CIWrapFilter

#ifndef __WRAPFILTER_H_
#define __WRAPFILTER_H_

#include "resource.h"       // main symbols

#include "iwrapcolargument.h"

class _ClRegisterFilter;

/////////////////////////////////////////////////////////////////////////////
// CIWrapFilter
class ATL_NO_VTABLE CIWrapFilter : 
	//public CComObjectRootEx<CComSingleThreadModel>,
	//public CComCoClass<CIWrapFilter, &CLSID_WrapFilter>,
	public IDispatchImpl<IWrapFilter, &IID_IWrapFilter, &LIBID_CHANNELWRAPPERLib>,

    public CIWrapColArgument
{
public:
	CIWrapFilter();

/*
DECLARE_REGISTRY_RESOURCEID(IDR_WRAPFILTER)

BEGIN_COM_MAP(CIWrapFilter)
	COM_INTERFACE_ENTRY(IWrapFilter)
	COM_INTERFACE_ENTRY(IDispatch)
	//COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
*/

// IWrapFilter
public:
	STDMETHOD(get_coArgument)(/*[out, retval]*/ IWrapColArgument** pVal);
    STDMETHOD(Refresh)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(Delete)(/*[out, retval]*/ short* ao_RetVal);

    void Init (_ClRegisterFilter *ai_pfilter);

private:
    _ClRegisterFilter *m_pfilter;
};

#endif //__WRAPFILTER_H_
