// PresentationCol.h : Declaration of the CPresentationCol

#ifndef __PRESENTATIONCOL_H_
#define __PRESENTATIONCOL_H_

#include "resource.h"       // main symbols
#include "cgocollection.h"

/////////////////////////////////////////////////////////////////////////////
// CPresentationCol
class ATL_NO_VTABLE CPresentationCol : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPresentationCol, &CLSID_PresentationCol>,
	public CGoCollection< IPresentationItem,  IDispatchImpl<IPresentationCol, &IID_IPresentationCol, &LIBID_M4DHOLib> >
	//public IDispatchImpl<IPresentationCol, &IID_IPresentationCol, &LIBID_M4DHOLib>
{
public:
	CPresentationCol()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PRESENTATIONCOL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPresentationCol)
	COM_INTERFACE_ENTRY(IPresentationCol)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IPresentationCol
public:
	HRESULT FinalConstruct()
	{
		return S_OK;
	}
};

#endif //__PRESENTATIONCOL_H_
