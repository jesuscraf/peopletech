#include "wores.hpp"

//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             NewInterface.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of NewInterface
//
//==============================================================================

//#pragma warning( disable : 4786)	// Link Warning: Identifiers too long (>255)


#ifdef _M4_COM_METRICS
#include "stdafx.h"
#include "logcomobject.h"
#endif
#include "newinterface.h"
#include "wrapdef.h"
#include "rerror.h"

#ifdef _M4_COM_METRICS

#include "m4chwrap.h"
#include "wrapnode.h"
#include "wrapchannel.h"
#include "wrapaccess.h"
#include "wrapcolprop.h"
#include "wrapcvm.h"
#include "wrapoper.h"
#include "wrapitemdef.h"
#include "wrapnodedef.h"
#include "wrapitit.h"
#include "wraprgit.h"
#include "wrapchandef.h"
#include "wrapslit.h"
#include "wrapimap.h"
#include "wrapbookmark.h"
#include "wraprecordset.h"
#include "wrapcolitems.h"
#include "wrapitem.h"
#include "wrapregister.h"
#include "wrapslice.h"
#include "wrapitems.h"
#include "enumitem.h"
#include "wrapexecutor.h"
#include "iwrapfilter.h"
#include "iwrapcolargument.h"
#include "iwrapargument.h"
#include "wraprgit_blockwalker.h"
#include "wrapvmsign.h"
#include "wrapproxylistit.h"
#include "wrapbreakpoint.h"
#include "wraprunninginfo.h"

//
// The name of this macro is designed to make you believe the code is using
// some elegant, table-driven approach. Don't believe it. It's not true.
//
#define IMPLEMENT_LOG_MAP(classId, impClass)\
	if (InlineIsEqualGUID(classId, ai_ClassId)) {\
		CLogComObject<impClass>* pCom;\
		HRESULT hRes = M4_CREATE_INSTANCE(impClass, &pCom);\
		if (SUCCEEDED(hRes)) {\
			hRes = pCom->QueryInterface(ai_InterfaceId, (void**)pUnk);\
		}\
		return hRes;\
	}
 

/*
 * LogCreateInstance - Creates an instance of a particular COM object with 
 * build-in creation/destruction trace behaviour (see CLogComObject).
 */
HRESULT LogCreateInstance(REFCLSID ai_ClassId, REFIID ai_InterfaceId, IUnknown** pUnk)
{
	if (InlineIsEqualGUID(CLSID_WrapNode, ai_ClassId))
	{
		CLogComObject<CWrapNode>* pCom;
		HRESULT hRes = M4_CREATE_INSTANCE(CWrapNode, &pCom);

		if (SUCCEEDED(hRes)) {
			hRes = pCom->QueryInterface(ai_InterfaceId, (void**)pUnk);
		}
		return hRes;
	}
	IMPLEMENT_LOG_MAP(CLSID_WrapNode,      CWrapNode)
	IMPLEMENT_LOG_MAP(CLSID_WrapChannel,   CWrapChannel)
	IMPLEMENT_LOG_MAP(CLSID_WrapAccess,    CWrapAccess)
	IMPLEMENT_LOG_MAP(CLSID_WrapColProp,   CWrapColProp)
	IMPLEMENT_LOG_MAP(CLSID_WrapCVM,       CWrapCVM)
	IMPLEMENT_LOG_MAP(CLSID_WrapOper,      CWrapOper)
	IMPLEMENT_LOG_MAP(CLSID_WrapItemDef,   CWrapItemDef)
	IMPLEMENT_LOG_MAP(CLSID_WrapNodeDef,   CWrapNodeDef)
	IMPLEMENT_LOG_MAP(CLSID_WrapItIt,      CWrapItIt)
	IMPLEMENT_LOG_MAP(CLSID_WrapRgIt,      CWrapRgIt)
	IMPLEMENT_LOG_MAP(CLSID_WrapChanDef,   CWrapChanDef)
	IMPLEMENT_LOG_MAP(CLSID_WrapSlIt,      CWrapSlIt)
	IMPLEMENT_LOG_MAP(CLSID_WrapIMap,      CWrapIMap)
	IMPLEMENT_LOG_MAP(CLSID_WrapBookMark,  CWrapBookMark)
	IMPLEMENT_LOG_MAP(CLSID_WrapRecordSet, CWrapRecordSet)
	IMPLEMENT_LOG_MAP(CLSID_WrapColItems,  CWrapColItems)
	IMPLEMENT_LOG_MAP(CLSID_WrapItem,      CWrapItem)
	IMPLEMENT_LOG_MAP(CLSID_WrapRegister,  CWrapRegister)
	IMPLEMENT_LOG_MAP(CLSID_WrapSlice,     CWrapSlice)
	IMPLEMENT_LOG_MAP(CLSID_WrapItems,     CWrapItems)
	IMPLEMENT_LOG_MAP(CLSID_enumitem,      Cenumitem)
	IMPLEMENT_LOG_MAP(CLSID_WrapExecutor,  CWrapExecutor)

/*
	IMPLEMENT_LOG_MAP(CLSID_WrapFilter,      CWrapFilter)
	IMPLEMENT_LOG_MAP(CLSID_WrapColArgument, CWrapColArgument)
	IMPLEMENT_LOG_MAP(CLSID_WrapArgument,    CWrapArgument)

*/
	IMPLEMENT_LOG_MAP(CLSID_WrapRgIt_BlockWalker, CWrapRgIt_BlockWalker)
	IMPLEMENT_LOG_MAP(CLSID_WrapVMSign,           CWrapVMSign)
	IMPLEMENT_LOG_MAP(CLSID_WrapProxyListIt,      CWrapProxyListIt)
	IMPLEMENT_LOG_MAP(CLSID_WrapBreakpoint,       CWrapBreakpoint)
	IMPLEMENT_LOG_MAP(CLSID_WrapRunningFormInfo,  CWrapRunningFormInfo)


	// Shouldn't get here. Probably means you need to add a new
	// IMPLEMENT_LOG_MAP for the requested COM object.
    return E_UNEXPECTED;
}


#endif // _M4_COM_METRICS


m4return_t NewInterface(REFCLSID ai_ClassId, REFIID ai_InterfaceId, IUnknown** pNodCl)
{
	m4return_t RetVal = M4_SUCCESS;
	IUnknown* pUnk = NULL;
	HRESULT hr;

#ifdef _M4_COM_METRICS
	
	hr = LogCreateInstance(ai_ClassId, ai_InterfaceId, pNodCl);

	if (FAILED(hr))
	{
		FormatError(M4_CH_WO_CANT_CREATE_OBJECT, "Unable to create object of class %d",	ai_ClassId);
		RetVal = M4_ERROR;
	}
	else
	{
		return RetVal;
	}

	// If we reach this point our CLSID list is out of date.
	// We fall back into calling CoCreateInstance 
	// (default non-debug behaviour), but make sure somebody notices the
	// error.

#endif // _M4_COM_METRICS

	hr = CoCreateInstance(ai_ClassId, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnk);

	if (FAILED(hr))
	{
		FormatError(M4_CH_WO_CANT_CREATE_OBJECT, "Unable to create object of class %d", ai_ClassId);
		RetVal = M4_ERROR;
	}
	else
	{
		hr = pUnk->QueryInterface(ai_InterfaceId, (void**)pNodCl);

		if (FAILED(hr))
		{
			FormatError(M4_CH_WO_INTERFACE_UNSUPPORTED, "Unable to create object of class %d", ai_ClassId);
			RetVal = M4_ERROR;
		}

		pUnk->Release();
	}

	return RetVal;
}
