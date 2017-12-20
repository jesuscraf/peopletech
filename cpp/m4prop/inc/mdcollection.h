// MDCollection.h : Declaration of the CMDCollection

#ifndef __MDCOLLECTION_H_
#define __MDCOLLECTION_H_

#include "cmycollection.h"
#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMDCollection
class ATL_NO_VTABLE CMDCollection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMDCollection, &CLSID_MDCollection>,
	public CMyCollection< IMDObject, IDispatchImpl<IMDCollection, &IID_IMDCollection, &LIBID_M4PROPLib> >
{
public:
	CMDCollection()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MDCOLLECTION)

BEGIN_COM_MAP(CMDCollection)
	COM_INTERFACE_ENTRY(IMDCollection)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IMDCollection
public:
	STDMETHOD(IsPresent)(/*[in]*/ VARIANT vIndex, /*[out, retval]*/ VARIANT_BOOL *pretVal);
};

#endif //__MDCOLLECTION_H_
