// M4General.h : Declaration of the CM4General

#ifndef __M4GENERAL_H_
#define __M4GENERAL_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CM4General
//##ModelId=396B329603B8
class ATL_NO_VTABLE CM4General : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4General, &CLSID_M4General>,
	public IDispatchImpl<IM4General, &IID_IM4General, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B32970019
	CM4General()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_M4GENERAL)

BEGIN_COM_MAP(CM4General)
	COM_INTERFACE_ENTRY(IM4General)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4General
public:
	//##ModelId=396B32970010
	STDMETHOD(GetIDsBitmapOfResource)(/*[in]*/BSTR nameLibrary, /*[out, retval]*/ ICPropBag * * nameBitmap);
};

#endif //__M4GENERAL_H_
