//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapitem.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapItem
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapitem.h"
#include "wrapitemdef.h"
#include "wrapcvm.h"
#include "m4types.hpp"
#include "m4define.hpp"
#include "newinterface.h"
#include "m4mdrt.hpp"
#include "variantref.h"
#include "cvdate.h"
#include "wrapdef.h"
#include "gstring.h"
#include "item.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapItem
CWrapItem::CWrapItem()
	: m_poItem(0)
{
}

CWrapItem::~CWrapItem()
{
	m_poItem = 0;
}

STDMETHODIMP CWrapItem::Init(VARIANT ai_varRef)
{
	ClVariantRef oVarRef(ai_varRef);

	m_poItem = (ClItem*)(void*)oVarRef;
	m_oIndex = m_poItem->GetIndex();

	TRACE3("Initialising with item address %p and index %d", m_poItem, m_oIndex);
	return S_OK;
}

STDMETHODIMP CWrapItem::get_IdItem(long* pVal)
{
	ENTER_METHOD(CWrapItem::get_IdItem);

	*pVal = m_oIndex;

	TRACE2("Returning item id %d", *pVal);
	LEAVE_METHOD(CWrapItem::get_IdItem, S_OK);
}

STDMETHODIMP CWrapItem::get_Name(BSTR* pVal)
{
	ENTER_METHOD(CWrapItem::get_Name);

	gstring tmpGstring((*m_poItem)[m_oIndex].ItemDef.Id());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItem::get_Name, S_OK);
	}
	else
	{
		SETCODE(M4_CH_WO_CANT_CREATE_STRING, ERRORLOG, "Cannot create a new string");
		LEAVE_METHOD(CWrapItem::get_Name, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItem::get_OldValue(VARIANT* pVal)
{
	m4VariantType var;

	ENTER_METHOD(CWrapItem::get_OldValue);

	(*m_poItem)[m_oIndex].OldValue.Get(var);

	var.GetVARIANT(*pVal);

	if (pVal->vt == VT_DATE && !pVal->date)
	{
		SETCODE(M4_CH_WO_INVALID_DATE, ERRORLOG, "Invalid date");
		LEAVE_METHOD(CWrapItem::get_OldValue, E_UNEXPECTED);
	}
	
	LEAVE_METHOD(CWrapItem::get_OldValue, S_OK);
}

STDMETHODIMP CWrapItem::get_coSlices(IWrapColSlices** pVal)
{
	ENTER_METHOD(CWrapItem::get_coSlices);

	// Cast this with the ColSlices interface
	// to allow access to this class as if it
	// were another class

	TRACE("Casting this object to an IWrapColSlices object");

	*pVal = (IWrapColSlices*) this;
	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapItem::get_coSlices, S_OK);
}

// IWrapColSlices
STDMETHODIMP CWrapItem::HasSlices(VARIANT_BOOL* ao_pb)
{
	ENTER_METHOD(CWrapItem::HasSlices);

	if ((*m_poItem)[m_oIndex].Value.HasSlices())
	{
		*ao_pb = VARIANT_TRUE;
	}
	else
	{
		*ao_pb = VARIANT_FALSE;
	}

	TRACE2("Returning HasSlices = %s", (*ao_pb)?"TRUE":"FALSE");
	LEAVE_METHOD(CWrapItem::HasSlices, S_OK);
}


STDMETHODIMP CWrapItem::get_Count(long *pVal)
{
	ENTER_METHOD(CWrapItem::get_Count);

	*pVal = m_poItem->Value.Count();

	TRACE2("The number of items = %d", *pVal);
	LEAVE_METHOD(CWrapItem::get_Count, S_OK);
}

STDMETHODIMP CWrapItem::DeleteAllSlices(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapItem::DeleteAllSlices);

	*ao_sRetVal = (*m_poItem)[m_oIndex].Value.DeleteAllSlices();

	TRACE2("Setting return value to %d", *ao_sRetVal);
	LEAVE_METHOD(CWrapItem::DeleteAllSlices, S_OK);
}

STDMETHODIMP CWrapItem::AddSlice(DATE ai_dStartDate, short* ao_sRetVal)
{
	ENTER_METHOD(CWrapItem::AddSlice);

	*ao_sRetVal = (*m_poItem)[m_oIndex].Value.AddSlice(DateToM4date(ai_dStartDate));

	TRACE3("Given date %g returning value %d", ai_dStartDate, *ao_sRetVal); 
	LEAVE_METHOD(CWrapItem::AddSlices, S_OK);
}

STDMETHODIMP CWrapItem::GetSliceByDate(DATE ai_dDate, IWrapSlice** pVal)
{
	ENTER_METHOD(CWrapItem::GetSliceByDate);

	m4bool_t noteof = (*m_poItem)[m_oIndex].Value.MoveTo(DateToM4date(ai_dDate));

	if (noteof)
	{
		if(NewInterface(CLSID_WrapSlice, IID_IWrapSlice, (IUnknown**)pVal) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapItem::GetSliceByDate, E_UNEXPECTED);
		}

		ClVariantRef oVarRef((void*)&((*m_poItem)[m_oIndex].Value));

		(*pVal)->Init(oVarRef);

		LEAVE_METHOD(CWrapItem::GetSliceByDate, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItem::GetSliceByDate, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItem::GetSliceByPos(int ai_iId, IWrapSlice** pVal)
{
	ENTER_METHOD(CWrapItem::GetSliceByPos);

	m4bool_t noteof = (*m_poItem)[m_oIndex].Value.MoveTo((ClSliceIndex)ai_iId);

	if (noteof)
	{
		if (NewInterface(CLSID_WrapSlice, IID_IWrapSlice, (IUnknown**)pVal) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapItem::GetSliceByPos, E_UNEXPECTED);
		}
		else
		{
			ClVariantRef oVarRef((void*) &((*m_poItem)[m_oIndex].Value));
	
			(*pVal)->Init(oVarRef);

			LEAVE_METHOD(CWrapItem::GetSliceByPos, S_OK);
		}
	}
	else
	{
		LEAVE_METHOD(CWrapItem::GetSliceByPos, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItem::get_Value(VARIANT* ao_poVariant)
{
	m4VariantType var;

	ENTER_METHOD(CWrapItem::get_Value);

	(*m_poItem)[m_oIndex].Value.Get(var);

	var.GetVARIANT(*ao_poVariant);

	if (ao_poVariant->vt == VT_DATE && !ao_poVariant->date)
	{
		SETCODE(M4_CH_WO_INVALID_DATE, ERRORLOG, "Invalid date");
		LEAVE_METHOD(CWrapItem::get_Value, E_UNEXPECTED);
	}

	LEAVE_METHOD(CWrapItem::get_Value, S_OK);
}

STDMETHODIMP CWrapItem::put_Value(VARIANT ai_oVariant)
{
	ENTER_METHOD(CWrapItem::put_Value);

	m4VariantType var(ai_oVariant);

	m_poItem->Value.Set(var);
	
	LEAVE_METHOD(CWrapItem::put_Value, S_OK);
}

STDMETHODIMP CWrapItem::Call(short* ao_sRetVal)
{
	ENTER_METHOD(CWrapItem::Call);

	*ao_sRetVal = (*m_poItem)[m_oIndex].Call(0, -1, M4_TRUE);	//0=default executor, -1=args in stack, M4_true=enable debug

	TRACE2("Item: Call returns %d", *ao_sRetVal);

	LEAVE_METHOD(CWrapItem::Call, S_OK);
}

STDMETHODIMP CWrapItem::get_ItemDef(IWrapItemDef** pVal)
{
	ENTER_METHOD(CWrapItem::get_ItemDef);

	if (NewInterface(CLSID_WrapItemDef, IID_IWrapItemDef, (IUnknown**)pVal) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapItem::get_ItemDef, E_UNEXPECTED);
	}

	((CWrapItemDef*)(*pVal))->Init(&((*m_poItem)[m_oIndex]));

	LEAVE_METHOD(CWrapItem::get_ItemDef, S_OK);
}

STDMETHODIMP CWrapItem::get_FooterValue(VARIANT* pVal)
{
	ENTER_METHOD(CWrapItem::get_FooterValue);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapExecutor::get_FooterValue" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapItem::get_FooterValue, S_OK);
}

STDMETHODIMP CWrapItem::CreateBlobFile(VARIANT_BOOL * pRet, BSTR DirName, BSTR Extension)
{
	ENTER_METHOD(CWrapItem::CreateBlobFile);

	gstring tmpDirName(DirName);
	gstring tmpExtension(Extension);

	m4return_t res;

	res = (*m_poItem)[m_oIndex].Value.CreateTempFile((m4pchar_t)tmpDirName,(m4pchar_t)tmpExtension);

	*pRet = (res == M4_SUCCESS) ? VARIANT_TRUE : VARIANT_FALSE;

	LEAVE_METHOD(CWrapItem::CreateBlobFile, S_OK);
}

STDMETHODIMP CWrapItem::ChangedMask(long * pMask)
{
	ENTER_METHOD(CWrapItem::ChangedMask);

	m4return_t res;
	m4uint32_t iMask = 0;

	res = (*m_poItem)[m_oIndex].Value.ChangedMask(iMask);

	if (res == M4_SUCCESS)
	{
		*pMask = iMask;

		LEAVE_METHOD(CWrapItem::ChangedMask, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItem::ChangedMask, S_FALSE);
	}
}

STDMETHODIMP CWrapItem::SetDescr(BSTR Descr)
{
	ENTER_METHOD(CWrapItem::SetDescr);

	gstring tmpDescr(Descr);
	m4return_t res;

	res = (*m_poItem)[m_oIndex].Value.SetDescr(tmpDescr);

	if (res == M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapItem::SetDescr, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItem::SetDescr, S_FALSE);
	}
}

STDMETHODIMP CWrapItem::GetDescr(BSTR * Descr)
{
	ENTER_METHOD(CWrapItem::GetDescr);

	m4pchar_t pcDescr = (*m_poItem)[m_oIndex].Value.GetDescr();

	gstring tmpDescr(pcDescr!=NULL?pcDescr:"");

	*Descr = tmpDescr;

	LEAVE_METHOD(CWrapItem::GetDescr, S_OK);
}
