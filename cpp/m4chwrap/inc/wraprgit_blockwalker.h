// WrapRgIt_BlockWalker.h : Declaration of the CWrapRgIt_BlockWalker

#ifndef __WRAPRGIT_BLOCKWALKER_H_
#define __WRAPRGIT_BLOCKWALKER_H_

#include "resource.h"       // main symbols

#include "iwraprgit.h"

/////////////////////////////////////////////////////////////////////////////
// CWrapRgIt_BlockWalker
class ATL_NO_VTABLE CWrapRgIt_BlockWalker : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapRgIt_BlockWalker, &CLSID_WrapRgIt_BlockWalker>,
	public IDispatchImpl<IWrapRgIt_BlockWalker, &IID_IWrapRgIt_BlockWalker, &LIBID_CHANNELWRAPPERLib>,

    public CIWrapRgIt
{
public:
	CWrapRgIt_BlockWalker();
	~CWrapRgIt_BlockWalker();

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPRGIT_BLOCKWALKER)

BEGIN_COM_MAP(CWrapRgIt_BlockWalker)
    COM_INTERFACE_ENTRY(IWrapRgIt_BlockWalker)
    COM_INTERFACE_ENTRY(IWrapRgIt)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
    COM_INTERFACE_ENTRY(IWrapRgItFilter)    

	COM_INTERFACE_ENTRY2(IDispatch, IWrapRgIt_BlockWalker)

    //Use this macro to disambiguate two branches of inheritance. 
    //For example, if you derive your class object from two dual interfaces, 
    //you expose IDispatch using COM_INTERFACE_ENTRY2 since IDispatch 
    //can be obtained from either one of the interfaces.


    //COM_INTERFACE_ENTRY2(IDispatch, IWrapRgIt)
	//COM_INTERFACE_ENTRY2(IDispatch, IEnumVARIANT)
    //COM_INTERFACE_ENTRY2(IDispatch, IWrapRgItFilter)    
END_COM_MAP()

// IWrapRgIt_BlockWalker
public:
    STDMETHOD(Attach)(/*[in]*/ IWrapNode *ai_Node, /*[out, retval]*/ short* ao_RetVal);
    STDMETHOD(get_RegisterIterator)(/*[out, retval]*/ IWrapRgIt* *pVal);
};

#endif //__WRAPRGIT_BLOCKWALKER_H_
