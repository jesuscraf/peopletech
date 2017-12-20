//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             WrapProxyListIt.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapProxyListIt
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapproxylistit.h"
#include "newinterface.h"
#include "gstring.h"
#include "wrapdef.h"
#include "rerror.h"
#include "channel.hpp"
#include "genproxy.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapProxyListIt
CWrapProxyListIt::CWrapProxyListIt() 
	: m_poProxyList(0)
{
}

CWrapProxyListIt::~CWrapProxyListIt()
{
}

void CWrapProxyListIt::Init(ClProxyList* ai_poProxyList)
{
	m_poProxyList = ai_poProxyList;
}

STDMETHODIMP CWrapProxyListIt::Item(long ai_iIndex, IWrapProxyListIt** ao_item)
{
	ENTER_METHOD(CWrapProxyListIt::Item);

	ClProxyList* poProxyList = m_poProxyList->Get(ai_iIndex);
	if (poProxyList != NULL)
	{
		NewInterface(CLSID_WrapProxyListIt, IID_IWrapProxyListIt, (IUnknown**)ao_item);
		((CWrapProxyListIt*)(*ao_item))->Init(poProxyList);

		LEAVE_METHOD(CWrapProxyListIt::Item, S_OK);
	}
	else
	{
		RaiseError("Index out of range");
		LEAVE_METHOD(CWrapProxyListIt::Item, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapProxyListIt::get_Count(long* pVal)
{
	ENTER_METHOD(CWrapProxyListIt::get_Count);

	*pVal = m_poProxyList->Count();

	TRACE2("The number of items is %d", *pVal);
	LEAVE_METHOD(CWrapProxyListIt::get_Count, S_OK);

}

STDMETHODIMP CWrapProxyListIt::get_Child(/* [retval][out] */ IWrapProxyListIt** pVal)
{
	ENTER_METHOD(CWrapProxyListIt::get_Child);

	ClProxyList* poProxyList = m_poProxyList->GetChild(0);
	if (poProxyList != NULL)
	{
		NewInterface(CLSID_WrapProxyListIt, IID_IWrapProxyListIt, (IUnknown**)pVal);
		((CWrapProxyListIt*)(*pVal))->Init(poProxyList);

		LEAVE_METHOD(CWrapProxyListIt::get_Child, S_OK);
	}
	else
	{
		RaiseError("Trying to obtain child of leaf node");
		LEAVE_METHOD(CWrapProxyListIt::get_Child, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapProxyListIt::IsLeaf(/* [retval][out] */ VARIANT_BOOL* pVal)
{
	ENTER_METHOD(CWrapProxyListIt::IsLeaf);

	*pVal = ((m_poProxyList->GetChild(0) == NULL) ? VARIANT_FALSE : VARIANT_TRUE);

	LEAVE_METHOD(CWrapProxyListIt::IsLeaf, S_OK);
}

STDMETHODIMP CWrapProxyListIt::get_Id(BSTR* pVal)
{
	ENTER_METHOD(CWrapProxyListIt::get_Id);

	gstring gstrId(m_poProxyList->GetId());
	*pVal = gstrId;

	LEAVE_METHOD(CWrapProxyListIt::get_Id, S_OK);
}

STDMETHODIMP CWrapProxyListIt::get_LocalUniqueId(BSTR* pVal)
{
	ENTER_METHOD(CWrapProxyListIt::get_LocalUniqueId);

	if (m_poProxyList->GetLocalUniqueId() == NULL)
	{
		gstring gstrId("");
		*pVal = gstrId;
	}
	else
	{
		m4char_t pcId[PROXY_MAX_VIEW_ID];
		sprintf(pcId,"%ld", m_poProxyList->GetLocalUniqueId());
		gstring gstrId(pcId);
		*pVal = gstrId;
	}

	LEAVE_METHOD(CWrapProxyListIt::get_LocalUniqueId, S_OK);
}

STDMETHODIMP CWrapProxyListIt::get_RemoteUniqueId(BSTR* pVal)
{
	ENTER_METHOD(CWrapProxyListIt::get_RemoteUniqueId);

	m4char_t pcId[PROXY_MAX_VIEW_ID];

	sprintf(pcId,"%ld", m_poProxyList->GetRemoteUniqueId());
	gstring gstrId(pcId);
	*pVal = gstrId;

	LEAVE_METHOD(CWrapProxyListIt::get_RemoteUniqueId, S_OK);
}
