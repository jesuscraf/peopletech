//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapregister.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapRegister
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapregister.h"
#include "newinterface.h"
#include "variantref.h"
#include "wrapdef.h"
#include "wrapitit.h"
#include "wrapoper.h"
#include "register.hpp"
#include "wores.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapRegister
CWrapRegister::CWrapRegister()
	: m_poRegister(0)
	, m_poItemIterator(0)
{
}

CWrapRegister::~CWrapRegister()
{
	// Como guardamos un ptr interno mantenemos un AddRef sobre
	// el objeto. Cuando terminamos...
	if (m_poItemIterator)
	{
		m_poItemIterator->Release();
		m_poItemIterator = 0;
	}

	m_poRegister = 0;
}

STDMETHODIMP CWrapRegister::Init(VARIANT ai_VarRef)
{
	ClVariantRef oVarRef(ai_VarRef);

	m_poRegister = (ClRegister*)(void*)oVarRef;
	m_oIndex = m_poRegister->Index;

	TRACE3("Wrapping register address %p with index %d", m_poRegister, m_oIndex);
	return S_OK;
}

STDMETHODIMP CWrapRegister::Unwrap(VARIANT* ao_VarRef)
{
	ClVariantRef oVarRef((void*) &((*m_poRegister)[m_oIndex]));

	*ao_VarRef = oVarRef;
	return S_OK;
}

STDMETHODIMP CWrapRegister::get_CoItems(IWrapColItems** pVal)
{
	ENTER_METHOD(CWrapRegister::get_CoItems);

	if(NewInterface(CLSID_WrapColItems, IID_IWrapColItems, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapRegister::get_CoItems, E_UNEXPECTED);
	}

	ClVariantRef oVarRef((void*) &((*m_poRegister)[m_oIndex].Item));

	(*pVal)->Init(oVarRef);

	LEAVE_METHOD(CWrapRegister::get_CoItems, S_OK);
}

STDMETHODIMP CWrapRegister::Delete(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapRegister::Delete);

	(*m_poRegister)[m_oIndex].Delete();
	*ao_sRetVal = M4_SUCCESS;

	LEAVE_METHOD(CWrapRegister::Delete, S_OK);
}

STDMETHODIMP CWrapRegister::get_IsEof(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapRegister::get_IsEof);

	if((*m_poRegister)[m_oIndex].Index == M4DM_EOF_INDEX)
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	LEAVE_METHOD(CWrapRegister::get_IsEof, S_OK);
}

STDMETHODIMP CWrapRegister::get_Operation(IWrapOper** pVal)
{
	ENTER_METHOD(CWrapRegister::get_Operation);

	if (NewInterface(CLSID_WrapOper, IID_IWrapOper, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapRegister::get_Operation, E_UNEXPECTED);
	}

	((CWrapOper*)(*pVal))->Init(&((*m_poRegister)[m_oIndex].Operation));

	LEAVE_METHOD(CWrapRegister::get_Operation, S_OK);
}

STDMETHODIMP CWrapRegister::get_Index(long* pVal)
{
	ENTER_METHOD(CWrapRegister::get_Index);

	*pVal = m_oIndex;

	TRACE2("Returning index %d", *pVal);
	LEAVE_METHOD(CWrapRegister::get_Index, S_OK);
}

STDMETHODIMP CWrapRegister::Free(short* ao_RetVal)
{
	ENTER_METHOD(CWrapRegister::Free);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapExecutor::Free" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapRegister::Free, S_OK);
}

STDMETHODIMP CWrapRegister::get_WrapItems(IWrapItems** pVal)
{
	ENTER_METHOD(CWrapRegister::get_WrapItems);

	if (NewInterface(CLSID_WrapItems, IID_IWrapItems, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapRegister::get_WrapItems, E_UNEXPECTED);
	}

	ClVariantRef oVarRef((void*)&((*m_poRegister)[m_oIndex].Item));

	(*pVal)->Init(oVarRef);

	LEAVE_METHOD(CWrapRegister::get_WrapItems, S_OK);
}

STDMETHODIMP CWrapRegister::get_ItemIterator(IWrapItIt** pVal)
{
	ENTER_METHOD(CWrapRegister::get_ItemIterator);

	if (!m_poItemIterator)
	{
#ifdef M4_NEWINTERFACE
		if (NewInterface(CLSID_WrapItIt, IID_IWrapItIt, (IUnknown**) &m_poItemIterator) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapRegister::get_ItemIterator, E_UNEXPECTED);
		}
#else
		HRESULT hRes;
		if (FAILED(hRes = M4_CREATE_INTERFACE(CWrapItIt, &m_poItemIterator))) 
		{
			LEAVE_METHOD(CWrapRegister::get_ItemIterator, hRes);
		}
#endif

		((CWrapItIt*)m_poItemIterator)->Init(&(*m_poRegister)[m_oIndex].Item);
	}

	*pVal = m_poItemIterator;

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapRegister::get_ItemIterator, S_OK);
}

STDMETHODIMP CWrapRegister::SendRegisterMark(VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapRegister::SendRegisterMark);

    m4bool_t bBranchMode = (*(m4bool_t *)pVal) ? M4_TRUE : M4_FALSE;

    m_poRegister->SendRegisterMark(bBranchMode);

	LEAVE_METHOD(CWrapRegister::SendRegisterMark, S_OK);
}
