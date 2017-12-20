//==============================================================================
//
// (c) Copyright 1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             WrapStChannelMemInfo.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             24/05/99
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Implementation of CWrapStChannelMemInfo
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapstchannelmeminfo.h"
#include "wrapdef.h"


/////////////////////////////////////////////////////////////////////////////
// CWrapStChannelMemInfo

STDMETHODIMP CWrapStChannelMemInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapStChannelMemInfo };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

void CWrapStChannelMemInfo::Unwrap(stChannelMemInfo_t** ao_VarRef)
{
	*ao_VarRef = &m_oMemInfo;
}

STDMETHODIMP CWrapStChannelMemInfo::get_NumNodes(long* pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::get_NumNodes);

	*pVal = m_oMemInfo.m_nBlocks;

	TRACE2("Number of nodes returned %d", *pVal);
	LEAVE_METHOD(CWrapStChannelMemInfo::get_NumNodes, S_OK);
}

STDMETHODIMP CWrapStChannelMemInfo::get_NumBlocks(long* pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::get_NumBlocks);

	*pVal = m_oMemInfo.m_nBlocks;

	TRACE2("Number of blocks returned %d", *pVal);
	LEAVE_METHOD(CWrapStChannelMemInfo::get_NumBlocks, S_OK);
}

STDMETHODIMP CWrapStChannelMemInfo::get_NumRegisters(long* pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::get_NumRegisters);

	*pVal = m_oMemInfo.m_nRegisters;

	TRACE2("Number of registers returned %d", *pVal);
	LEAVE_METHOD(CWrapStChannelMemInfo::get_NumRegisters, S_OK);
}

STDMETHODIMP CWrapStChannelMemInfo::get_DataSize(long* pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::get_DataSize);

	*pVal = m_oMemInfo.m_nDataSize;

	TRACE2("Data Size returned %d", *pVal);
	LEAVE_METHOD(CWrapStChannelMemInfo::get_DataSize, S_OK);
}

STDMETHODIMP CWrapStChannelMemInfo::get_TotalSize(long* pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::get_TotalSize);

	*pVal = m_oMemInfo.m_nTotalSize;

	TRACE2("Total Size returned %d", *pVal);
	LEAVE_METHOD(CWrapStChannelMemInfo::get_TotalSize, S_OK);
}

STDMETHODIMP CWrapStChannelMemInfo::get_NumNulls(long* pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::get_NumNulls);

	*pVal = m_oMemInfo.m_nNulls;

	TRACE2("Number of Nulls returned %d", *pVal);
	LEAVE_METHOD(CWrapStChannelMemInfo::get_NumNulls, S_OK);
}

STDMETHODIMP CWrapStChannelMemInfo::Reset(long* pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::Reset);

	m_oMemInfo.Reset();

	LEAVE_METHOD(CWrapStChannelMemInfo::Reset, S_OK);
}

STDMETHODIMP CWrapStChannelMemInfo::Add(IWrapStChannelMemInfo* ai_piObj, long *pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::Add);

	stChannelMemInfo_t* pStObj;
	((CWrapStChannelMemInfo*)ai_piObj)->Unwrap(&pStObj);

	m_oMemInfo += *pStObj;

	LEAVE_METHOD(CWrapStChannelMemInfo::Add, S_OK);
}

STDMETHODIMP CWrapStChannelMemInfo::Sub(IWrapStChannelMemInfo* ai_piObj, long *pVal)
{
	ENTER_METHOD(CWrapStChannelMemInfo::Sub);

	stChannelMemInfo_t* pStObj;
	((CWrapStChannelMemInfo*)ai_piObj)->Unwrap(&pStObj);

	m_oMemInfo -= *pStObj;

	LEAVE_METHOD(CWrapStChannelMemInfo::Sub, S_OK);
}

