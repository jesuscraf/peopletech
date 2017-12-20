// ContainedObjects.h : Declaration of the CContainedObjects

#ifndef __CONTAINEDOBJECTS_H_
#define __CONTAINEDOBJECTS_H_

#include "cexeccollection.h"
#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CContainedObjects
class ATL_NO_VTABLE CContainedObjects : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CContainedObjects, &CLSID_ContainedObjects>,
	public CExecCollection< IDispatch , IDispatchImpl<IContainedObjects, &IID_IContainedObjects, &LIBID_M4PROPLib> >
	//public IDispatchImpl<IContainedObjects, &IID_IContainedObjects, &LIBID_M4PROPLib>
{
public:
	CContainedObjects()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONTAINEDOBJECTS)

BEGIN_COM_MAP(CContainedObjects)
	COM_INTERFACE_ENTRY(IContainedObjects)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IContainedObjects
public:
};

#endif //__CONTAINEDOBJECTS_H_
