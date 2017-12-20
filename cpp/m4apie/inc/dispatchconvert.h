// DispatchConvert.h : Declaration of the CDispatchConvert

#ifndef __DISPATCHCONVERT_H_
#define __DISPATCHCONVERT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDispatchConvert
//##ModelId=396B3295003C
class ATL_NO_VTABLE CDispatchConvert : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDispatchConvert, &CLSID_DispatchConvert>,
	public IDispatchImpl<IDispatchConvert, &IID_IDispatchConvert, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B32950125
	CDispatchConvert()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DISPATCHCONVERT)

BEGIN_COM_MAP(CDispatchConvert)
	COM_INTERFACE_ENTRY(IDispatchConvert)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IDispatchConvert
public:
	//##ModelId=396B3295011B
	STDMETHOD(Long2Object)(/*[in]*/ long *Value, /*[out,retval]*/ IDispatch **Object);
	//##ModelId=396B3295010E
	STDMETHOD(Object2Long)(/*[in]*/IDispatch *Object,/*[out,retval]*/ long *Value);
};

#endif //__DISPATCHCONVERT_H_
