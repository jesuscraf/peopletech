//==============================================================================
//
// (c) Copyright 1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             WrapChannelMemInfo.cpp
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

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapchannelmeminfo.h"
#include "wrapdef.h"
#include "wrapaccess.h"
#include "wrapnode.h"
#include "wraprecordset.h"
#include "iwraprgit.h"
#include "wrapstchannelmeminfo.h"
#include "wrapstnodememinfo.h"
#include "wrapstrsmeminfo.h"
#include "wrapstrgmeminfo.h"

/////////////////////////////////////////////////////////////////////////////
// CWrapChannelMemInfo

STDMETHODIMP CWrapChannelMemInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapBookMark };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

STDMETHODIMP CWrapChannelMemInfo::MemInfoChannel(/*[in]*/IWrapAccess* ai_piAccess, /*[in]*/IWrapStChannelMemInfo* ai_piStChannelMemInfo, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CWrapChannelMemInfo::MemInfoChannel);

    ClAccess* pAccess;
	stChannelMemInfo_t* pStChannelMemInfo;

	((CWrapAccess*)ai_piAccess)->Unwrap(&pAccess);
	((CWrapStChannelMemInfo*)ai_piStChannelMemInfo)->Unwrap(&pStChannelMemInfo);
	
	TRACE2("Access pointer has address %p",	pAccess);
	TRACE2("stChannelMemInfo pointer has address %p", pStChannelMemInfo);

	//Medir memoria
	m4return_t ret=m_oMemInfo.GetMemInfo (pAccess, *pStChannelMemInfo);	//Medida

	*ao_RetVal = short( (ret==M4_SUCCESS) ? S_OK : S_FALSE );

	LEAVE_METHOD(CWrapChannelMemInfo::MemInfoChannel, S_OK);
}


STDMETHODIMP CWrapChannelMemInfo::MemInfoNode(/*[in]*/IWrapNode* ai_piNode,	/*[in]*/IWrapStNodeMemInfo* ai_pStNodeMemInfo,	/*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CWrapChannelMemInfo::MemInfoNode);

    ClNode* pNode;
	stNodeMemInfo_t* pStNodeMemInfo;

	((CWrapNode*)ai_piNode)->Unwrap(&pNode);
	((CWrapStNodeMemInfo*)ai_pStNodeMemInfo)->Unwrap(&pStNodeMemInfo);
	
	TRACE2("Node pointer has address %p", pNode);
	TRACE2("StNodeMemInfo pointer has address %p", pStNodeMemInfo);

	//Medir memoria
	m4return_t ret=m_oMemInfo.GetMemInfo (pNode, *pStNodeMemInfo);	//Medida

	*ao_RetVal = short( (ret==M4_SUCCESS) ? S_OK : S_FALSE );

	LEAVE_METHOD(CWrapChannelMemInfo::MemInfoNode, S_OK);
}


STDMETHODIMP CWrapChannelMemInfo::MemInfoRecordSet(/*[in]*/IWrapRecordSet* ai_piRS, /*[in]*/IWrapStRSMemInfo* ai_pStRSMemInfo, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CWrapChannelMemInfo::MemInfoRecordSet);

    ClAccessRecordSet* paRS;	//ClAccessRecordSet deriva de ClRecordSet
	stRSMemInfo_t* pStRSMemInfo;

	((CWrapRecordSet*)ai_piRS)->Unwrap(&paRS);
	((CWrapStRSMemInfo*)ai_pStRSMemInfo)->Unwrap(&pStRSMemInfo);
	ClRecordSet* pRS = (ClRecordSet*)paRS;			//ClAccessRecordSet deriva de ClRecordSet

	TRACE2("RecordSet pointer has address %p",pRS);
	TRACE2("StRSMemInfo pointer has address %p", pStRSMemInfo);

	//Medir memoria
	m4return_t ret=m_oMemInfo.GetMemInfo (pRS, *pStRSMemInfo);	//Medida

	*ao_RetVal = short( (ret==M4_SUCCESS) ? S_OK : S_FALSE );

	LEAVE_METHOD(CWrapChannelMemInfo::MemInfoRecordSet, S_OK);
}

STDMETHODIMP CWrapChannelMemInfo::MemInfoRegister(/*[in]*/IWrapRgIt* ai_piRegister,/*[in]*/IWrapStRgMemInfo* ai_pStRgMemInfo, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CWrapChannelMemInfo::MemInfoRegister);

    _ClRegister_Base* pRg;
	stRgMemInfo_t* pStRgMemInfo;

	pRg = ((CIWrapRgIt*)ai_piRegister)->GetpRegister();
	((CWrapStRgMemInfo*)ai_pStRgMemInfo)->Unwrap(&pStRgMemInfo);
	
	TRACE2("Register pointer has address %p", pRg);
	TRACE2("StRgMemInfo pointer has address %p", pStRgMemInfo);

	//Medir memoria
	m4return_t ret=m_oMemInfo.GetMemInfo (pRg, *pStRgMemInfo);	//Medida

	*ao_RetVal = short( (ret==M4_SUCCESS) ? S_OK : S_FALSE );

	LEAVE_METHOD(CWrapChannelMemInfo::MemInfoRegister, S_OK);
}

