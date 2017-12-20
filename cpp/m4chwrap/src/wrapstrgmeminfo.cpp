// WrapStRgMemInfo.cpp : Implementation of CWrapStRgMemInfo

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapstrgmeminfo.h"
#include "wrapdef.h"

/////////////////////////////////////////////////////////////////////////////
// CWrapStRgMemInfo

STDMETHODIMP CWrapStRgMemInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapStRgMemInfo };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

void CWrapStRgMemInfo::Unwrap(stRgMemInfo_t** ao_VarRef)
{
	*ao_VarRef = &m_oMemInfo;
}

STDMETHODIMP CWrapStRgMemInfo::get_DataSize(long* pVal)
{
	ENTER_METHOD(CWrapStRgMemInfo::get_DataSize);

	*pVal = m_oMemInfo.m_nDataSize;

	TRACE2("Data Size returned %d", *pVal);
	LEAVE_METHOD(CWrapStRgMemInfo::get_DataSize, S_OK);
}

STDMETHODIMP CWrapStRgMemInfo::get_TotalSize(long* pVal)
{
	ENTER_METHOD(CWrapStRgMemInfo::get_TotalSize);

	*pVal = m_oMemInfo.m_nTotalSize;

	TRACE2("Total Size returned %d", *pVal);
	LEAVE_METHOD(CWrapStRgMemInfo::get_TotalSize, S_OK);
}

STDMETHODIMP CWrapStRgMemInfo::get_NumNulls(long* pVal)
{
	ENTER_METHOD(CWrapStRgMemInfo::get_NumNulls);

	*pVal = m_oMemInfo.m_nNulls;

	TRACE2("Number of Nulls returned %d", *pVal);
	LEAVE_METHOD(CWrapStRgMemInfo::get_NumNulls, S_OK);
}

STDMETHODIMP CWrapStRgMemInfo::Reset(long* pVal)
{
	ENTER_METHOD(CWrapStRgMemInfo::Reset);

	m_oMemInfo.Reset();

	LEAVE_METHOD(CWrapStRgMemInfo::Reset, S_OK);
}

STDMETHODIMP CWrapStRgMemInfo::Add(IWrapStRgMemInfo* ai_piObj, long *pVal)
{
	ENTER_METHOD(CWrapStRgMemInfo::Add);

	stRgMemInfo_t* pStObj;
	((CWrapStRgMemInfo*)ai_piObj)->Unwrap(&pStObj);

	m_oMemInfo += *pStObj;

	LEAVE_METHOD(CWrapStRgMemInfo::Add, S_OK);
}

STDMETHODIMP CWrapStRgMemInfo::Sub(IWrapStRgMemInfo* ai_piObj, long *pVal)
{
	ENTER_METHOD(CWrapStRgMemInfo::Sub);

	stRgMemInfo_t* pStObj;
	((CWrapStRgMemInfo*)ai_piObj)->Unwrap(&pStObj);

	m_oMemInfo -= *pStObj;

	LEAVE_METHOD(CWrapStRgMemInfo::Sub, S_OK);
}

