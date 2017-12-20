// WrapStChannelMemInfo.h : Declaration of the CWrapStChannelMemInfo

#ifndef __WRAPSTCHANNELMEMINFO_H_
#define __WRAPSTCHANNELMEMINFO_H_

#include "resource.h"       // main symbols
#include "logcomobject.h"
#include "meminfo.hpp"
#include "wrapstnodememinfo.h"

/////////////////////////////////////////////////////////////////////////////
// CWrapStChannelMemInfo
class ATL_NO_VTABLE CWrapStChannelMemInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapStChannelMemInfo, &CLSID_WrapStChannelMemInfo>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapStChannelMemInfo, &IID_IWrapStChannelMemInfo, &LIBID_CHANNELWRAPPERLib>
	//public CWrapStNodeMemInfo
{
	DECLARE_LOG_AGGREGATABLE(CWrapStChannelMemInfo)
public:
	CWrapStChannelMemInfo()
	{
	}

	void Unwrap(stChannelMemInfo_t** ao_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPSTCHANNELMEMINFO)

BEGIN_COM_MAP(CWrapStChannelMemInfo)
	COM_INTERFACE_ENTRY(IWrapStChannelMemInfo)
	//COM_INTERFACE_ENTRY(IWrapStNodeMemInfo)
	//COM_INTERFACE_ENTRY(IWrapStRSMemInfo)
	//COM_INTERFACE_ENTRY(IWrapStRgMemInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IWrapStChannelMemInfo
public:
	STDMETHOD(get_DataSize)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_TotalSize)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumNulls)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumRegisters)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumBlocks)	(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumNodes)		(/*[out, retval]*/ long *pVal);
	STDMETHOD(Reset)			(/*[out, retval]*/ long *pVal);
	STDMETHOD(Add)				(/*[in]*/ IWrapStChannelMemInfo* ai_piObj, /*[out, retval]*/ long *pVal);
	STDMETHOD(Sub)				(/*[in]*/ IWrapStChannelMemInfo* ai_piObj, /*[out, retval]*/ long *pVal);

public:
	stChannelMemInfo_t m_oMemInfo;	//Objeto con la informacion de memoria a nivel de Canal

};

#endif //__WRAPSTCHANNELMEMINFO_H_
