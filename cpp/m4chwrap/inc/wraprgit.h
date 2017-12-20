//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wraprgit.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapRgIt
//
//==============================================================================
#include "resource.h"       // main symbols

#include "iwraprgit.h"

#ifndef __WRAPRGIT_H_
#define __WRAPRGIT_H_

class _ClRegister_Base;


/////////////////////////////////////////////////////////////////////////////
// CWrapRgIt
class ATL_NO_VTABLE CWrapRgIt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapRgIt, &CLSID_WrapRgIt>,

    public CIWrapRgIt
{
public:
	CWrapRgIt();
	~CWrapRgIt();

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPRGIT)

BEGIN_COM_MAP(CWrapRgIt)
    COM_INTERFACE_ENTRY(IWrapRgIt)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
    COM_INTERFACE_ENTRY(IWrapRgItFilter)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapRgIt)
	COM_INTERFACE_ENTRY2(IUnknown, IEnumVARIANT)
    COM_INTERFACE_ENTRY2(IDispatch, IWrapRgItFilter)
END_COM_MAP()

};

#endif //__WRAPRGIT_H_
