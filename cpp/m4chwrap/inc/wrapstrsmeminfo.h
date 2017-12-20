// WrapStRSMemInfo.h : Declaration of the CWrapStRSMemInfo

#ifndef __WRAPSTRSMEMINFO_H_
#define __WRAPSTRSMEMINFO_H_

#include "resource.h"       // main symbols
#include "logcomobject.h"
#include "meminfo.hpp"
#include "wrapstrgmeminfo.h"

/////////////////////////////////////////////////////////////////////////////
// CWrapStRSMemInfo
class ATL_NO_VTABLE CWrapStRSMemInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapStRSMemInfo, &CLSID_WrapStRSMemInfo>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapStRSMemInfo, &IID_IWrapStRSMemInfo, &LIBID_CHANNELWRAPPERLib>
{
	DECLARE_LOG_AGGREGATABLE(CWrapStRSMemInfo)
public:
	CWrapStRSMemInfo()
	{
	}

	void Unwrap(stRSMemInfo_t** ao_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPSTRSMEMINFO)

BEGIN_COM_MAP(CWrapStRSMemInfo)
	COM_INTERFACE_ENTRY(IWrapStRSMemInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IWrapStRSMemInfo
public:
	STDMETHOD(get_DataSize)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_TotalSize)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumNulls)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumRegisters)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(Reset)			(/*[out, retval]*/ long *pVal);
	STDMETHOD(Add)				(/*[in]*/ IWrapStRSMemInfo* ai_piObj, /*[out, retval]*/ long *pVal);
	STDMETHOD(Sub)				(/*[in]*/ IWrapStRSMemInfo* ai_piObj, /*[out, retval]*/ long *pVal);

public:
	stRSMemInfo_t m_oMemInfo;	//Objeto con la informacion de memoria a nivel de RecordSet
};

#endif //__WRAPSTRSMEMINFO_H_
