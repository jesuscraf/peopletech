// M4Collection.h : Declaration of the CM4Collection

#ifndef __M4COLLECTION_H_
#define __M4COLLECTION_H_

#include "resource.h"       // main symbols
#include "cgocollection.h"

/////////////////////////////////////////////////////////////////////////////
// CM4Collection
class ATL_NO_VTABLE CM4Collection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4Collection, &CLSID_M4Collection>,
	//public IDispatchImpl<IM4Collection, &IID_IM4Collection, &LIBID_M4COLLib>
	public CGoCollection< IDispatch, IDispatchImpl<IM4Collection, &IID_IM4Collection, &LIBID_M4COLLib> >
{
public:
	CM4Collection()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_M4COLLECTION)

BEGIN_COM_MAP(CM4Collection)
	COM_INTERFACE_ENTRY(IM4Collection)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4Collection
public:
};

#endif //__M4COLLECTION_H_
