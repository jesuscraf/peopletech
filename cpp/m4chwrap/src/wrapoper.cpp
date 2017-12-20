//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapoper.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapOper
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapoper.h"
#include "operation.hpp"
#include "wrapdef.h"


/////////////////////////////////////////////////////////////////////////////
// CWrapOper
STDMETHODIMP CWrapOper::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapOper };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

CWrapOper::CWrapOper()
	: m_pOperation(0)
{
}

CWrapOper::~CWrapOper()
{
	m_pOperation = 0;
}

void CWrapOper::Init(_ClOperation_Base* ai_VarRef)
{
	m_pOperation = ai_VarRef;

	TRACE2("Wrapped operation has address %p", m_pOperation);
}

STDMETHODIMP CWrapOper::get_IsDelete(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapOper::get_IsDelete);

	if (m_pOperation->IsDeleted())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapOper::get_IsDelete, S_OK);
}

STDMETHODIMP CWrapOper::get_IsInsert(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapOper::get_IsInsert);

	if (m_pOperation->IsNew())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapOper::get_IsInsert, S_OK);
}

STDMETHODIMP CWrapOper::get_IsUpdate(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapOper::get_IsUpdate);

	if (m_pOperation->IsUpdated())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapOper::get_IsUpdate, S_OK);
}

STDMETHODIMP CWrapOper::get_IsModified(VARIANT_BOOL * pVal)
{
	ENTER_METHOD(CWrapOper::get_IsModified);

	if (m_pOperation->IsModified())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapOper::get_IsModified, S_OK);
}

STDMETHODIMP CWrapOper::get_IsDelete_DB(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapOper::get_IsDelete_DB);

	if (m_pOperation->IsDeletedByDDBB())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapOper::get_IsDelete_DB, S_OK);
}

STDMETHODIMP CWrapOper::get_IsInsert_DB(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapOper::get_IsInsert_DB);

	if (m_pOperation->IsNewByDDBB())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapOper::get_IsInsert_DB, S_OK);
}

STDMETHODIMP CWrapOper::get_IsUpdate_DB(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapOper::get_IsUpdate_DB);

	if (m_pOperation->IsUpdatedByDDBB())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapOper::get_IsUpdate_DB, S_OK);
}

STDMETHODIMP CWrapOper::get_IsModified_DB(VARIANT_BOOL * pVal)
{
	ENTER_METHOD(CWrapOper::get_IsModified_DB);

	if (m_pOperation->IsModifiedByDDBB())
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapOper::get_IsModified_DB, S_OK);
}


