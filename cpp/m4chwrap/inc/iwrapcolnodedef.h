// WrapColNodeDef.h : Declaration of the CIWrapColNodeDef

#ifndef __WRAPCOLNODEDEF_H_
#define __WRAPCOLNODEDEF_H_

#include "resource.h"       // main symbols

#include "m4types.hpp"

class ClNodeDirectory_Data;

/////////////////////////////////////////////////////////////////////////////
// CIWrapColNodeDef
class ATL_NO_VTABLE CIWrapColNodeDef : 
	//public CComObjectRootEx<CComSingleThreadModel>,
	//public CComCoClass<CIWrapColNodeDef, &CLSID_WrapColNodeDef>,
	public IDispatchImpl<IWrapColNodeDef, &IID_IWrapColNodeDef, &LIBID_CHANNELWRAPPERLib>,

//interface de colección:
    public IDispatchImpl<IEnumVARIANT, &IID_IEnumVARIANT, &LIBID_CHANNELWRAPPERLib>

{
public:
	CIWrapColNodeDef()
	{
        m_pnodedir = 0;
        m_index = 0;
	}

/*
DECLARE_REGISTRY_RESOURCEID(IDR_WRAPCOLNODEDEF)

BEGIN_COM_MAP(CIWrapColNodeDef)
	COM_INTERFACE_ENTRY(IWrapColNodeDef)
	COM_INTERFACE_ENTRY(IDispatch)
	//COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
*/

// IWrapColNodeDef
public:
  	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
    STDMETHOD(Item)(/*[in]*/ int ai_iIndex, /*[out, retval]*/ IWrapNodeDef** ao_poRegister);
    STDMETHOD(_NewEnum)(/*[out, retval]*/ IUnknown** ao_unk);
    STDMETHOD(Clone)(/*[out]*/ IEnumVARIANT** ppenum);
    STDMETHOD(Reset)();
    STDMETHOD(Skip)(/*[in]*/ ULONG celt);
    STDMETHOD(Next)(/*[in]*/ ULONG celt, /*[in]*/ VARIANT* rgvar, /*[out]*/ ULONG* pceltFetched);

    void Init (ClNodeDirectory_Data *ai_pndc);

private:
//interface de colección de nodos:
    ClNodeDirectory_Data *m_pnodedir;
    m4uint32_t m_index;
};

#endif //__WRAPCOLNODEDEF_H_
