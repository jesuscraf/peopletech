
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapstrsmeminfo.h"
#include "wrapdef.h"


/////////////////////////////////////////////////////////////////////////////
// CWrapStRSMemInfo

STDMETHODIMP CWrapStRSMemInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapStRSMemInfo };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

void CWrapStRSMemInfo::Unwrap(stRSMemInfo_t** ao_VarRef)
{
	*ao_VarRef = &m_oMemInfo;
}


STDMETHODIMP CWrapStRSMemInfo::get_NumRegisters(long* pVal)
{
	ENTER_METHOD(CWrapStRSMemInfo::get_NumRegisters);

	*pVal = m_oMemInfo.m_nRegisters;

	TRACE2("Number of registers returned %d", *pVal);
	LEAVE_METHOD(CWrapStRSMemInfo::get_NumRegisters, S_OK);
}

STDMETHODIMP CWrapStRSMemInfo::get_DataSize(long* pVal)
{
	ENTER_METHOD(CWrapStRSMemInfo::get_DataSize);

	*pVal = m_oMemInfo.m_nDataSize;

	TRACE2("Data Size returned %d", *pVal);
	LEAVE_METHOD(CWrapStRSMemInfo::get_DataSize, S_OK);
}

STDMETHODIMP CWrapStRSMemInfo::get_TotalSize(long* pVal)
{
	ENTER_METHOD(CWrapStRSMemInfo::get_TotalSize);

	*pVal = m_oMemInfo.m_nTotalSize;

	TRACE2("Total Size returned %d", *pVal);
	LEAVE_METHOD(CWrapStRSMemInfo::get_TotalSize, S_OK);
}

STDMETHODIMP CWrapStRSMemInfo::get_NumNulls(long* pVal)
{
	ENTER_METHOD(CWrapStRSMemInfo::get_NumNulls);

	*pVal = m_oMemInfo.m_nNulls;

	TRACE2("Number of Nulls returned %d", *pVal);
	LEAVE_METHOD(CWrapStRSMemInfo::get_NumNulls, S_OK);
}

STDMETHODIMP CWrapStRSMemInfo::Reset(long* pVal)
{
	ENTER_METHOD(CWrapStRSMemInfo::Reset);

	m_oMemInfo.Reset();

	LEAVE_METHOD(CWrapStRSMemInfo::Reset, S_OK);
}

STDMETHODIMP CWrapStRSMemInfo::Add(IWrapStRSMemInfo* ai_piObj, long *pVal)
{
	ENTER_METHOD(CWrapStRSMemInfo::Add);

	stRSMemInfo_t* pStObj;
	((CWrapStRSMemInfo*)ai_piObj)->Unwrap(&pStObj);

	m_oMemInfo += *pStObj;

	LEAVE_METHOD(CWrapStRSMemInfo::Add, S_OK);
}

STDMETHODIMP CWrapStRSMemInfo::Sub(IWrapStRSMemInfo* ai_piObj, long *pVal)
{
	ENTER_METHOD(CWrapStRSMemInfo::Sub);

	stRSMemInfo_t* pStObj;
	((CWrapStRSMemInfo*)ai_piObj)->Unwrap(&pStObj);

	m_oMemInfo -= *pStObj;

	LEAVE_METHOD(CWrapStRSMemInfo::Sub, S_OK);
}

