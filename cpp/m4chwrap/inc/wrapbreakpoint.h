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



#ifndef __IWRAPBREAKPOINT_H_
#define __IWRAPBREAKPOINT_H_

#include "breakpoint.hpp"

/////////////////////////////////////////////////////////////////////////////
// CWrapRgIt
class ATL_NO_VTABLE CWrapBreakpoint : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapBreakpoint, &CLSID_WrapBreakpoint>,
	public IDispatchImpl<IWrapBreakpoint, &IID_IWrapBreakpoint, &LIBID_CHANNELWRAPPERLib>


{
public:
	CWrapBreakpoint ();
	~CWrapBreakpoint ();

	void Init (ClBreakpoint *ai_poBreakpoint);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPBREAKPOINT)

BEGIN_COM_MAP(CWrapBreakpoint)
    COM_INTERFACE_ENTRY(IWrapBreakpoint)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapBreakpoint)

END_COM_MAP()

public:

	STDMETHOD(get_M4Object) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(put_M4Object) (/*[in]*/ BSTR ai_pstName);
	STDMETHOD(get_Node) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(put_Node) (/*[in]*/ BSTR ai_pstName);
	STDMETHOD(get_Item) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(put_Item) (/*[in]*/ BSTR ai_pstName);
	STDMETHOD(get_Rule) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(put_Rule) (/*[in]*/ BSTR ai_pstName);
	STDMETHOD(get_Line) (/*[out, retval]*/ int* ao_piLine);
	STDMETHOD(put_Line) (/*[in]*/ int ai_piLine);
	STDMETHOD(get_StartDate) (/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_StartDate) (/*[in]*/ DATE pVal);
	STDMETHOD(get_EndDate) (/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_EndDate) (/*[in]*/ DATE pVal);
	STDMETHOD(get_Active) (/*[out, retval]*/ VARIANT_BOOL* ao_pbVal);
	STDMETHOD(put_Active) (/*[in]*/ VARIANT_BOOL ai_bVal);
	STDMETHOD(get_Value) (/*[out, retval]*/ int* ao_piVal);
	STDMETHOD(put_Value) (/*[in]*/ int ai_iVal);
	STDMETHOD(get_RuleTI) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(put_RuleTI) (/*[in]*/ BSTR ai_pstName);

	void UnWrap (ClBreakpoint ** ao_poBP);

protected:
	ClBreakpoint m_oBreakpoint;

};

#endif //__IWRAPBREAKPOINT_H_
