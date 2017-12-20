// WrapStRgMemInfo.h : Declaration of the CWrapStRgMemInfo

#ifndef __WRAPSTRGMEMINFO_H_
#define __WRAPSTRGMEMINFO_H_

#include "resource.h"       // main symbols
#include "logcomobject.h"
#include "meminfo.hpp"

/////////////////////////////////////////////////////////////////////////////
// CWrapStRgMemInfo
class ATL_NO_VTABLE CWrapStRgMemInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapStRgMemInfo, &CLSID_WrapStRgMemInfo>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapStRgMemInfo, &IID_IWrapStRgMemInfo, &LIBID_CHANNELWRAPPERLib>
{
	DECLARE_LOG_AGGREGATABLE(CWrapStRgMemInfo)
public:
	CWrapStRgMemInfo()
	{
	}

	void Unwrap(stRgMemInfo_t** ao_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPSTRGMEMINFO)

BEGIN_COM_MAP(CWrapStRgMemInfo)
	COM_INTERFACE_ENTRY(IWrapStRgMemInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IWrapStRgMemInfo
public:
	STDMETHOD(get_DataSize)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_TotalSize)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumNulls)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(Reset)			(/*[out, retval]*/ long *pVal);
	STDMETHOD(Add)				(/*[in]*/ IWrapStRgMemInfo* ai_piObj, /*[out, retval]*/ long *pVal);
	STDMETHOD(Sub)				(/*[in]*/ IWrapStRgMemInfo* ai_piObj, /*[out, retval]*/ long *pVal);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

public:
	stRgMemInfo_t m_oMemInfo;	//Objeto con la informacion de memoria a cualquier nivel
};

#endif //__WRAPSTRGMEMINFO_H_
