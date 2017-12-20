// LineCol.h : Declaration of the CLineCol

#ifndef __LINECOL_H_
#define __LINECOL_H_

#include "resource.h"       // main symbols
#include "cgocollection.h"

/////////////////////////////////////////////////////////////////////////////
// CLineCol
//##ModelId=396DF30A013F
class ATL_NO_VTABLE CLineCol : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLineCol, &CLSID_LineCol>,
	public CGoCollection< IDesignLine, IDispatchImpl<ILineCol, &IID_ILineCol, &LIBID_M4DESIGNLib> >
	//public IDispatchImpl<ILineCol, &IID_ILineCol, &LIBID_M4DESIGNLib>
{
public:
	//##ModelId=396DF30A017D
	CLineCol()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LINECOL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLineCol)
	COM_INTERFACE_ENTRY(ILineCol)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ILineCol
public:
};

#endif //__LINECOL_H_
