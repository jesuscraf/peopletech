// MenuPopPup.h : Declaration of the CMenuPopPup

#ifndef __MENUPOPPUP_H_
#define __MENUPOPPUP_H_

#include "resource.h"       // main symbols
#include "cpm4prop.h"
#include "cmycollection.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuPopPup
class ATL_NO_VTABLE CMenuPopPup : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMenuPopPup, &CLSID_MenuPopPup>,
	public IProvideClassInfo2Impl<&CLSID_MenuPopPup, &DIID__MenuPopPupEvents, &LIBID_M4PROPLib >,
	public CMyCollection<IMenuItem, IDispatchImpl<IMenuPopPup, &IID_IMenuPopPup, &LIBID_M4PROPLib> >,
	public CProxy_MenuPopPupEvents<CMenuPopPup>,
	public IConnectionPointContainerImpl<CMenuPopPup>
{
public:
	CMenuPopPup()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MENUPOPPUP)

BEGIN_COM_MAP(CMenuPopPup)
	COM_INTERFACE_ENTRY(IMenuPopPup)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMenuPopPup)
	CONNECTION_POINT_ENTRY(DIID__MenuPopPupEvents)
END_CONNECTION_POINT_MAP()

// IMenuPopPup
public:
	STDMETHOD(Reset)();
	STDMETHOD(MenuEventClick)(/*[in, out]*/ BSTR * strKey);
	STDMETHOD(AddNewMenuItem)(BSTR* strCaption,BSTR* strKey,VARIANT_BOOL * bEnabled,VARIANT_BOOL * bChecked,VARIANT_BOOL * bVisible,VARIANT_BOOL * bBold,VARIANT_BOOL * bRet);
};

#endif //__MENUPOPPUP_H_
