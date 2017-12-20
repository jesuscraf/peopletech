// EOCollection.h : Declaration of the CEOCollection

#ifndef __EOCOLLECTION_H_
#define __EOCOLLECTION_H_

#include "cexeccollection.h"
#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEOCollection
class ATL_NO_VTABLE CEOCollection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEOCollection, &CLSID_EOCollection>,
	public CExecCollection< IDispatch , IDispatchImpl<IEOCollection, &IID_IEOCollection, &LIBID_M4PROPLib> >
{
public:
	CEOCollection()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EOCOLLECTION)

BEGIN_COM_MAP(CEOCollection)
	COM_INTERFACE_ENTRY(IEOCollection)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IEOCollection
public:
};

#endif //__EOCOLLECTION_H_
