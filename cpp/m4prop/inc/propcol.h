// PropCol.h : Declaration of the CPropCol

#ifndef __PROPCOL_H_
#define __PROPCOL_H_

#include "resource.h"       // main symbols
#include "cmycollection.h"

/////////////////////////////////////////////////////////////////////////////
// CPropCol
class ATL_NO_VTABLE CPropCol : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPropCol, &CLSID_PropCol>,
	public CMyCollection<ICProperty, IDispatchImpl<IPropCol, &IID_IPropCol, &LIBID_M4PROPLib> >

{
public:
	CPropCol()
	{
		ATLTRACE("Creación de CPropCol()\n");
	}

	~CPropCol()
	{
		ATLTRACE("Destrucción de CPropCol()\n");	
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROPCOL)

BEGIN_COM_MAP(CPropCol)
	COM_INTERFACE_ENTRY(IPropCol)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IPropCol
public:
	STDMETHOD(get_Property)(/*[in]*/ VARIANT nIndex, /*[in]*/ VARIANT vDefValue, /*[out, retval]*/ ICProperty **pVal);
//	STDMETHOD(ConditionalAddProperty)(/*[in]*/ BSTR Name, /*[in]*/ VARIANT vValue, /*[in]*/ VARIANT vDefault);
	STDMETHOD(AddProperty)(/*[in]*/  BSTR strName, /*[in]*/ VARIANT vValue, VARIANT_BOOL bEditable, M4PropertyType nType, VARIANT vDefValue);
};

#endif //__PROPCOL_H_
