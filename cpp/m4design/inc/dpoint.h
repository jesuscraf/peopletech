// DPoint.h : Declaration of the CDPoint

#ifndef __DPOINT_H_
#define __DPOINT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDPoint
//##ModelId=396DF30C002A
class ATL_NO_VTABLE CDPoint : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDPoint, &CLSID_DPoint>,
	public IDispatchImpl<IDPoint, &IID_IDPoint, &LIBID_M4DESIGNLib>
{
public:
	//##ModelId=396DF30C00B7
	CDPoint()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DPOINT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDPoint)
	COM_INTERFACE_ENTRY(IDPoint)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IDPoint
public:
	//##ModelId=396DF30C0086
	CLine *		m_oLine;
	//##ModelId=396DF30C0072
	CM4Handle *	m_oHandle;
	//##ModelId=396DF30C00AD
	STDMETHOD(get_y)(/*[out, retval]*/ long *pVal);
	//##ModelId=396DF30C00A2
	STDMETHOD(put_y)(/*[in]*/ long newVal);
	//##ModelId=396DF30C0098
	STDMETHOD(get_x)(/*[out, retval]*/ long *pVal);
	//##ModelId=396DF30C008E
	STDMETHOD(put_x)(/*[in]*/ long newVal);
};

#endif //__DPOINT_H_
