
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapstnodememinfo.h"
#include "wrapdef.h"


/////////////////////////////////////////////////////////////////////////////
// CWrapStNodeMemInfo

STDMETHODIMP CWrapStNodeMemInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapStNodeMemInfo };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

void CWrapStNodeMemInfo::Unwrap(stNodeMemInfo_t** ao_VarRef)
{
	*ao_VarRef = &m_oMemInfo;
}

STDMETHODIMP CWrapStNodeMemInfo::get_NumBlocks(long* pVal)
{
	ENTER_METHOD(CWrapStNodeMemInfo::get_NumBlocks);

	*pVal = m_oMemInfo.m_nBlocks;

	TRACE2("Number of blocks returned %d", *pVal);
	LEAVE_METHOD(CWrapStNodeMemInfo::get_NumBlocks, S_OK);
}

STDMETHODIMP CWrapStNodeMemInfo::get_NumRegisters(long* pVal)
{
	ENTER_METHOD(CWrapStNodeMemInfo::get_NumRegisters);

	*pVal = m_oMemInfo.m_nRegisters;

	TRACE2("Number of registers returned %d", *pVal);
	LEAVE_METHOD(CWrapStNodeMemInfo::get_NumRegisters, S_OK);
}

STDMETHODIMP CWrapStNodeMemInfo::get_DataSize(long* pVal)
{
	ENTER_METHOD(CWrapStNodeMemInfo::get_DataSize);

	*pVal = m_oMemInfo.m_nDataSize;

	TRACE2("Data Size returned %d", *pVal);
	LEAVE_METHOD(CWrapStNodeMemInfo::get_DataSize, S_OK);
}

STDMETHODIMP CWrapStNodeMemInfo::get_TotalSize(long* pVal)
{
	ENTER_METHOD(CWrapStNodeMemInfo::get_TotalSize);

	*pVal = m_oMemInfo.m_nTotalSize;

	TRACE2("Total Size returned %d", *pVal);
	LEAVE_METHOD(CWrapStNodeMemInfo::get_TotalSize, S_OK);
}

STDMETHODIMP CWrapStNodeMemInfo::get_NumNulls(long* pVal)
{
	ENTER_METHOD(CWrapStNodeMemInfo::get_NumNulls);

	*pVal = m_oMemInfo.m_nNulls;

	TRACE2("Number of Nulls returned %d", *pVal);
	LEAVE_METHOD(CWrapStNodeMemInfo::get_NumNulls, S_OK);
}

STDMETHODIMP CWrapStNodeMemInfo::Reset(long* pVal)
{
	ENTER_METHOD(CWrapStNodeMemInfo::Reset);

	m_oMemInfo.Reset();

	LEAVE_METHOD(CWrapStNodeMemInfo::Reset, S_OK);
}

STDMETHODIMP CWrapStNodeMemInfo::Add(IWrapStNodeMemInfo* ai_piObj, long *pVal)
{
	ENTER_METHOD(CWrapStNodeMemInfo::Add);

	stNodeMemInfo_t* pStObj;
	((CWrapStNodeMemInfo*)ai_piObj)->Unwrap(&pStObj);

	m_oMemInfo += *pStObj;

	LEAVE_METHOD(CWrapStNodeMemInfo::Add, S_OK);
}

STDMETHODIMP CWrapStNodeMemInfo::Sub(IWrapStNodeMemInfo* ai_piObj, long *pVal)
{
	ENTER_METHOD(CWrapStNodeMemInfo::Sub);

	stNodeMemInfo_t* pStObj;
	((CWrapStNodeMemInfo*)ai_piObj)->Unwrap(&pStObj);

	m_oMemInfo -= *pStObj;

	LEAVE_METHOD(CWrapStNodeMemInfo::Sub, S_OK);
}

