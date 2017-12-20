//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapslice.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapSlice
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapslice.h"
#include "newinterface.h"
#include "variantref.h"
#include "cvdate.h"
#include "wrapdef.h"
#include "val.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapSlice

//
// Beggining of definitions of classes for
// slice interface IWrapSlice
//
CWrapSlice::CWrapSlice()
	: m_poValue(0)
{
}

CWrapSlice::~CWrapSlice()
{
	m_poValue = 0;
}

STDMETHODIMP CWrapSlice::Delete(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapSlice::Delete);

	*ao_sRetVal = (*m_poValue)[m_oIndex].DeleteSlice();

	TRACE2("Returning %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapSlice::Delete, S_OK);
}

STDMETHODIMP CWrapSlice::get_StartDate(DATE* pVal)
{
	ENTER_METHOD(CWrapSlice::get_StartDate);

	*pVal = M4dateToDate((*m_poValue)[m_oIndex].GetStartDate());

	TRACE2("Returning date %g", *pVal);
	LEAVE_METHOD(CWrapSlice::get_StartDate, S_OK);
}

STDMETHODIMP CWrapSlice::get_EndDate(DATE* pVal)
{
	ENTER_METHOD(CWrapSlice::get_EndDate);

	*pVal = M4dateToDate((*m_poValue)[m_oIndex].GetEndDate());

	TRACE2("Returning date %g", *pVal);
	LEAVE_METHOD(CWrapSlice::get_EndDate, S_OK);
}

STDMETHODIMP CWrapSlice::put_StartDate(DATE ai_date)
{
	ENTER_METHOD(CWrapSlice::put_StartDate);
	TRACE2("Putting date %g", ai_date);

	(*m_poValue)[m_oIndex].SetStartDate(DateToM4date(ai_date));

	LEAVE_METHOD(CWrapSlice::put_StartDate, S_OK);
}

STDMETHODIMP CWrapSlice::put_EndDate(DATE ai_date)
{
	ENTER_METHOD(CWrapSlice::put_EndDate);
	TRACE2("Putting date %g", ai_date);

	(*m_poValue)[m_oIndex].SetEndDate(M4dateToDate(ai_date));

	LEAVE_METHOD(CWrapSlice::put_EndDate, S_OK);
}

STDMETHODIMP CWrapSlice::get_Value(VARIANT* ao_poVariant)
{
	m4VariantType var;
	
	ENTER_METHOD(CWrapSlice::get_Value);

	(*m_poValue)[m_oIndex].Get(var);
	var.GetVARIANT(*ao_poVariant);

	LEAVE_METHOD(CWrapSlice::get_Value, S_OK);
}

STDMETHODIMP CWrapSlice::put_Value(VARIANT ai_oVariant)
{
	ENTER_METHOD(CWrapSlice::put_Value);

	m4VariantType var(ai_oVariant);
	(*m_poValue)[m_oIndex].Set(var);
	
	LEAVE_METHOD(CWrapSlice::put_Value, S_OK);
}

STDMETHODIMP CWrapSlice::Init(VARIANT ai_varRef)
{
	ClVariantRef oVarRef(ai_varRef);

	m_poValue = (ClValue*)(void*)oVarRef;
	m_oIndex = m_poValue->Index;

	TRACE3("Wrapping value address %p index %d", m_poValue, m_oIndex);
	return S_OK;
}
