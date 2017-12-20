//==============================================================================
//
// (c) Copyright 1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             WrapChannelMemInfo.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             24/05/99
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapChannelMemInfo
//
//==============================================================================
// WrapChannelMemInfo.h : Declaration of the CWrapChannelMemInfo

#ifndef __WRAPCHANNELMEMINFO_H_
#define __WRAPCHANNELMEMINFO_H_

#include "resource.h"       // main symbols
#include "logcomobject.h"
#include "meminfo.hpp"

/////////////////////////////////////////////////////////////////////////////
// CWrapChannelMemInfo
class ATL_NO_VTABLE CWrapChannelMemInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapChannelMemInfo, &CLSID_WrapChannelMemInfo>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapChannelMemInfo, &IID_IWrapChannelMemInfo, &LIBID_CHANNELWRAPPERLib>
{
	DECLARE_LOG_AGGREGATABLE(CWrapChannelMemInfo)
public:
	CWrapChannelMemInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPCHANNELMEMINFO)

BEGIN_COM_MAP(CWrapChannelMemInfo)
	COM_INTERFACE_ENTRY(IWrapChannelMemInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IWrapChannelMemInfo
public:
	STDMETHOD(MemInfoChannel)	(/*[in]*/IWrapAccess*		ai_piAccess,/*[in]*/IWrapStChannelMemInfo*	ai_pStChannelMemInfo,	/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(MemInfoNode)		(/*[in]*/IWrapNode*		ai_piNode,	/*[in]*/IWrapStNodeMemInfo *	ai_pStNodeMemInfo,		/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(MemInfoRecordSet)	(/*[in]*/IWrapRecordSet*	ai_piRS,	/*[in]*/IWrapStRSMemInfo *		ai_pStRSMemInfo,		/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(MemInfoRegister)	(/*[in]*/IWrapRgIt*		ai_piRegister,/*[in]*/IWrapStRgMemInfo*		ai_pStRgMemInfo,		/*[out, retval]*/ short* ao_RetVal);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	ClChannelMemInfo m_oMemInfo;
};

#endif //__WRAPCHANNELMEMINFO_H_
