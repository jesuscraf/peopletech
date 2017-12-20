// WrapStNodeMemInfo.h : Declaration of the CWrapStNodeMemInfo

#ifndef __WRAPSTNODEMEMINFO_H_
#define __WRAPSTNODEMEMINFO_H_

#include "resource.h"       // main symbols
#include "logcomobject.h"
#include "meminfo.hpp"
#include "wrapstrsmeminfo.h"

/////////////////////////////////////////////////////////////////////////////
// CWrapStNodeMemInfo
class ATL_NO_VTABLE CWrapStNodeMemInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapStNodeMemInfo, &CLSID_WrapStNodeMemInfo>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapStNodeMemInfo, &IID_IWrapStNodeMemInfo, &LIBID_CHANNELWRAPPERLib>
	//public CWrapStRSMemInfo
{
	DECLARE_LOG_AGGREGATABLE(CWrapStNodeMemInfo)
public:
	CWrapStNodeMemInfo()
	{
	}

	void Unwrap(stNodeMemInfo_t** ao_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPSTNODEMEMINFO)

BEGIN_COM_MAP(CWrapStNodeMemInfo)
	COM_INTERFACE_ENTRY(IWrapStNodeMemInfo)
	//COM_INTERFACE_ENTRY(IWrapStRSMemInfo)
	//COM_INTERFACE_ENTRY(IWrapStRgMemInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IWrapStNodeMemInfo
public:
	STDMETHOD(get_DataSize)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_TotalSize)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumNulls)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumRegisters)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumBlocks)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(Reset)			(/*[out, retval]*/ long *pVal);
	STDMETHOD(Add)				(/*[in]*/ IWrapStNodeMemInfo* ai_piObj, /*[out, retval]*/ long *pVal);
	STDMETHOD(Sub)				(/*[in]*/ IWrapStNodeMemInfo* ai_piObj, /*[out, retval]*/ long *pVal);

public:
	stNodeMemInfo_t m_oMemInfo;	//Objeto con la informacion de memoria a nivel de Nodo
};

#endif //__WRAPSTNODEMEMINFO_H_
