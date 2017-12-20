//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapslit.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapSlIt
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapslit.h"
#include "index.hpp"
#include "val.hpp"
#include "wrapdef.h"
#include "cvdate.h"
#include "newinterface.h"
#include "rerror.h"


/////////////////////////////////////////////////////////////////////////////
// CWrapSlIt

STDMETHODIMP CWrapSlIt::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapSlIt };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

CWrapSlIt::CWrapSlIt()
	: m_poValue(0)
	, m_IsClone(M4_FALSE)
{
}

CWrapSlIt::~CWrapSlIt()
{
	if (m_IsClone == M4_TRUE)
	{
		delete m_poValue;
		m_IsClone = M4_FALSE;
	}

	m_poValue = 0;
}

void CWrapSlIt::SetIsClone(m4bool_t ai_flag)
{
	m_IsClone = ai_flag;
}

m4bool_t CWrapSlIt::GetIsClone(void)
{
	return m_IsClone;
}

STDMETHODIMP CWrapSlIt::HasSlices(VARIANT_BOOL* ao_pb)
{
	ENTER_METHOD(CWrapSlIt::HasSlices);

	if (m_poValue->HasSlices())
	{
		*ao_pb = VARIANT_TRUE;
	}
	else
	{
		*ao_pb = VARIANT_FALSE;
	}

	TRACE2("Returning HasSlices = %s", (*ao_pb)?"TRUE":"FALSE");
	LEAVE_METHOD(CWrapSlIt::HasSlices, S_OK);
}

STDMETHODIMP CWrapSlIt::get_Count(long *pVal)
{
	ENTER_METHOD(CWrapSlIt::get_Count);

	*pVal = m_poValue->Count();

	TRACE2("The number of items = %d", *pVal);
	LEAVE_METHOD(CWrapSlIt::get_Count, S_OK);
}

STDMETHODIMP CWrapSlIt::RemoveAll(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapSlIt::RemoveAll);

	*ao_sRetVal = m_poValue->DeleteAllSlices();

	TRACE2("Setting return value to %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapSlIt::RemoveAll, S_OK);
}

STDMETHODIMP CWrapSlIt::Add(DATE ai_dStartDate, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapSlIt::Add);

	*ao_sRetVal = m_poValue->AddSlice(DateToM4date(ai_dStartDate));

	TRACE3("Given date %g returning value %d", ai_dStartDate, *ao_sRetVal); 
	LEAVE_METHOD(CWrapSlIt::Add, S_OK);
}

m4return_t CWrapSlIt::GetSliceByDate(DATE ai_dDate, IWrapSlIt** pVal)
{
	m4bool_t noteof = m_poValue->MoveTo(DateToM4date(ai_dDate));

	if (noteof)
	{
		*pVal = this;
		((IUnknown*)*pVal)->AddRef();
		return M4_SUCCESS;
	}
	else
	{
		*pVal = 0;
		return M4_ERROR;
	}
}

m4return_t CWrapSlIt::GetSliceByPos(int ai_iId, IWrapSlIt** pVal)
{
	m4bool_t noteof = m_poValue->MoveTo((ClSliceIndex)ai_iId);

	if (noteof)
	{
		*pVal = this;
		((IUnknown*)*pVal)->AddRef();
		return M4_SUCCESS;
	}
	else
	{
		*pVal = 0;
		return M4_ERROR;
	}
}

STDMETHODIMP CWrapSlIt::Item(VARIANT ai_var, IWrapSlIt** ao_item)
{
	ENTER_METHOD(CWrapSlIt::Item);

	m4return_t eof;

	switch (ai_var.vt)
	{
	case VT_DATE:
		eof = GetSliceByDate(ai_var.date, ao_item);
		break;

	case VT_UI1:
		eof = GetSliceByPos(ai_var.bVal, ao_item);
		break;

	case VT_I2:
		eof = GetSliceByPos(ai_var.iVal, ao_item);
		break;

	case VT_I4:
		eof = GetSliceByPos(ai_var.lVal, ao_item);
		break;

	default:
		FormatError(M4_CH_WO_INVALID_GET_SLICE, "Cannot get a slice given variant type %d", ai_var.vt);
		LEAVE_METHOD(CWrapSlIt::Item, E_UNEXPECTED);
		break;
	}

	if (eof != M4_SUCCESS)
	{
		FormatError(M4_CH_WO_NO_SUCH_ITEM, "Cannot move to slice");
		LEAVE_METHOD(CWrapSlIt::Item, E_UNEXPECTED);
	}

	LEAVE_METHOD(CWrapSlIt::Item, S_OK);
}

STDMETHODIMP CWrapSlIt::Remove(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapSlIt::Remove);

	*ao_sRetVal = m_poValue->DeleteSlice();

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapSlIt::Remove, S_OK);
}

STDMETHODIMP CWrapSlIt::get_StartDate(DATE* pVal)
{
	ENTER_METHOD(CWrapSlIt::get_StartDate);

	*pVal = M4dateToDate(m_poValue->GetStartDate());

	TRACE2("Returning date %g", *pVal);
	LEAVE_METHOD(CWrapSlIt::get_StartDate, S_OK);
}

STDMETHODIMP CWrapSlIt::get_EndDate(DATE* pVal)
{
	ENTER_METHOD(CWrapSlIt::get_EndDate);

	*pVal = M4dateToDate(m_poValue->GetEndDate());

	TRACE2("Returning date %g", *pVal);
	LEAVE_METHOD(CWrapSlIt::get_EndDate, S_OK);
}

STDMETHODIMP CWrapSlIt::put_StartDate(DATE ai_date)
{
	ENTER_METHOD(CWrapSlIt::put_StartDate);

	TRACE2("Putting date %g", ai_date);

	m_poValue->SetStartDate(DateToM4date(ai_date));
	LEAVE_METHOD(CWrapSlIt::put_StartDate, S_OK);
}

STDMETHODIMP CWrapSlIt::put_EndDate(DATE ai_date)
{
	ENTER_METHOD(CWrapSlIt::put_EndDate);

	TRACE2("Putting date %g", ai_date);

	m_poValue->SetEndDate(M4dateToDate(ai_date));
	LEAVE_METHOD(CWrapSlIt::put_EndDate, S_OK);
}

STDMETHODIMP CWrapSlIt::get_Value(VARIANT* ao_poVariant)
{
	m4VariantType var;
	
	ENTER_METHOD(CWrapSlIt::get_Value);

	m_poValue->Get(var);
	var.GetVARIANT(*ao_poVariant);

	LEAVE_METHOD(CWrapSlIt::get_Value, S_OK);
}

STDMETHODIMP CWrapSlIt::put_Value(VARIANT ai_oVariant)
{
	ENTER_METHOD(CWrapSlIt::put_Value);

	m4VariantType var(ai_oVariant);
	m_poValue->Set(var);
	
	LEAVE_METHOD(CWrapSlIt::put_Value, S_OK);
}

void CWrapSlIt::Init(ClValue* ai_varRef)
{
	m_poValue = ai_varRef;
	TRACE2("Wrapping value address %p", m_poValue);
}

STDMETHODIMP CWrapSlIt::_NewEnum(IUnknown** pVal)
{
	ENTER_METHOD(CWrapSlIt::_NewEnum);

	*pVal = (IEnumVARIANT*) this;

	((IUnknown*)*pVal)->AddRef();
	
	LEAVE_METHOD(CWrapSlIt::_NewEnum, S_OK);
}

STDMETHODIMP CWrapSlIt::Next(ULONG celt, VARIANT* rgvar, ULONG* pceltFetched)
{
	ULONG l2 = 0;

	ENTER_METHOD(CWrapSlIt::Next);

	if (pceltFetched != NULL)
	{
		*pceltFetched = 0;
	}
	
	m4uint32_t Count = m_poValue->Count();

	ClSliceIndex oIndex = m_poValue->Index + 1;

	if (oIndex < Count)
    {
		m_poValue->MoveTo(oIndex);

		VariantInit(&rgvar[0]);

		rgvar[0].vt = VT_UNKNOWN;
		rgvar[0].punkVal = (IWrapSlIt*) this;
		((IUnknown*)rgvar[0].punkVal)->AddRef();

		// Set count of elements retrieved.
		if (pceltFetched != NULL)
		{
			*pceltFetched = 1;
		}

		l2++;
	}

	LEAVE_METHOD(CWrapSlIt::Next, (l2 < celt) ? S_FALSE : S_OK);
}

STDMETHODIMP CWrapSlIt::Skip(ULONG celt)
{
	ENTER_METHOD(CWrapSlIt::Skip);

	m4uint32_t Count = m_poValue->Count();

	ClSliceIndex oIndex = m_poValue->Index + celt;

	if (oIndex > Count)
	{
		oIndex = Count - 1;

		LEAVE_METHOD(CWrapSlIt::Skip, S_FALSE);
	}

	m_poValue->MoveTo(oIndex);

	LEAVE_METHOD(CWrapSlIt::Skip, S_OK);
}

STDMETHODIMP CWrapSlIt::Reset()
{
	ENTER_METHOD(CWrapSlIt::Reset);

	m_poValue->MoveTo((ClItemIndex)0);

	LEAVE_METHOD(CWrapSlIt::Reset, S_OK);
}

STDMETHODIMP CWrapSlIt::Clone(IEnumVARIANT** ppenum)
{
	ENTER_METHOD(CWrapSlIt::Clone);

	ClValue* poValue = new ClValue(*m_poValue);

	if (poValue)
	{
		if (NewInterface(CLSID_WrapSlIt, IID_IEnumVARIANT, (IUnknown**)ppenum) != M4_SUCCESS)
		{
			delete poValue;
			LEAVE_METHOD(CWrapSlIt::Clone, E_UNEXPECTED);
		}

		*poValue = *m_poValue;

		((CWrapSlIt*)(*ppenum))->Init(m_poValue);
		((CWrapSlIt*)(*ppenum))->SetIsClone(M4_TRUE);

		LEAVE_METHOD(CWrapSlIt::Clone, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANNOT_CLONE_SLICE, "Cannot create a clone of the slice");
		LEAVE_METHOD(CWrapSlIt::Clone, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapSlIt::get_Copy(IWrapSlIt** pVal)
{
	ENTER_METHOD(CWrapSlIt::get_Copy);

	ClValue* poValue = new ClValue(*m_poValue);

	if (poValue)
	{
		if (NewInterface(CLSID_WrapSlIt, IID_IWrapSlIt, (IUnknown**) pVal) != M4_SUCCESS)
		{
			delete poValue;
			LEAVE_METHOD(CWrapSlIt::get_Copy, E_UNEXPECTED);
		}

		((CWrapSlIt*)*pVal)->Init(poValue);
		((CWrapSlIt*)(*pVal))->SetIsClone(M4_TRUE);

		LEAVE_METHOD(CWrapSlIt::get_Copy, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_CANNOT_COPY_SLICE, "Cannot create a copy of the slice");
		LEAVE_METHOD(CWrapSlIt::get_Copy, E_UNEXPECTED);
	}
}
