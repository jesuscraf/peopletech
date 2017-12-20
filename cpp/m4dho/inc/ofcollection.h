// OFCollection.h : Declaration of the COFCollection

#ifndef __OFCOLLECTION_H_
#define __OFCOLLECTION_H_

#include "resource.h"       // main symbols
#include "cgocollection.h"

/////////////////////////////////////////////////////////////////////////////
// COFCollection
class ATL_NO_VTABLE COFCollection : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<COFCollection, &CLSID_OFCollection>,
	public CGoCollection< ICSingleProp, IDispatchImpl<IOFCollection, &IID_IOFCollection, &LIBID_M4DHOLib> >
{
public:
	COFCollection()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_OFCOLLECTION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(COFCollection)
	COM_INTERFACE_ENTRY(IOFCollection)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IOFCollection
public:
};

#endif //__OFCOLLECTION_H_
