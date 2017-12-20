// WrapColArgument.h : Declaration of the CIWrapColArgument

#ifndef __WRAPCOLARGUMENT_H_
#define __WRAPCOLARGUMENT_H_

#include "resource.h"       // main symbols

#include "iwrapargument.h"

class ClArgumentList;
class _ClArgumentList_Base;

/////////////////////////////////////////////////////////////////////////////
// CIWrapColArgument
class ATL_NO_VTABLE CIWrapColArgument : 
	//public CComObjectRootEx<CComSingleThreadModel>,
	//public CComCoClass<CIWrapColArgument, &CLSID_WrapColArgument>,
	public IDispatchImpl<IWrapColArgument, &IID_IWrapColArgument, &LIBID_CHANNELWRAPPERLib>,

    public CIWrapArgument
{
public:
	CIWrapColArgument();

/*
DECLARE_REGISTRY_RESOURCEID(IDR_WRAPCOLARGUMENT)

BEGIN_COM_MAP(CIWrapColArgument)
	COM_INTERFACE_ENTRY(IWrapColArgument)
	COM_INTERFACE_ENTRY(IDispatch)
	//COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
*/

// IWrapColArgument
public:
    STDMETHODIMP get_Count (long* ao_iCount);
    STDMETHODIMP Item(VARIANT ai_var, IWrapArgument** ao_poArg);

    void Init (_ClArgumentList_Base *ai_pargs);

private:
    ClArgumentList *m_poArgumentList;
};

#endif //__WRAPCOLARGUMENT_H_
