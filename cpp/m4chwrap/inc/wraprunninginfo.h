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
//    Declaration of the CWrapRunningFormInfo
//
//==============================================================================
#include "resource.h"       // main symbols



#ifndef __IWRAPRUNNINGFORM_H_
#define __IWRAPRUNNINGFORM_H_

#include "runningforminfo.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapRgIt
class ATL_NO_VTABLE CWrapRunningFormInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapRunningFormInfo, &CLSID_WrapRunningFormInfo>,
	public IDispatchImpl<IWrapRunningFormInfo, &IID_IWrapRunningFormInfo, &LIBID_CHANNELWRAPPERLib>


{
public:
	CWrapRunningFormInfo ();
	~CWrapRunningFormInfo ();


DECLARE_REGISTRY_RESOURCEID(IDR_WRAPRUNNINGINFO)

BEGIN_COM_MAP(CWrapRunningFormInfo)
    COM_INTERFACE_ENTRY(IWrapRunningFormInfo)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapRunningFormInfo)

END_COM_MAP()

public:
	void Init (ClRunningFormInfo *ai_poInfo);

	STDMETHOD(get_M4Object) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(get_Node) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(get_Item) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(get_Rule) (/*[out, retval]*/ BSTR* ao_pstName);
	STDMETHOD(get_Line) (/*[out, retval]*/ int* ao_piLine);
	STDMETHOD(get_StartDate) (/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_EndDate) (/*[out, retval]*/ DATE *pVal);


	STDMETHOD(GetDates) (/*[out]*/ DATE *pStart,/*[out]*/ DATE *pEnd,/*[out]*/ DATE *pRun, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_IsVariableArgs) (/*[out, retval]*/ VARIANT_BOOL* ao_pbVal);
	STDMETHOD(get_NumberOfArguments) (/*[out, retval]*/ int* ao_piVal);
	STDMETHOD(GetArguments) (/*[out]*/ SAFEARRAY** ao_saArguments, /*[out, retval]*/ short* ao_pbVal);
	STDMETHOD(GetArgument) (/*[in]*/ int ai_iPos, /*[out, retval]*/ VARIANT* ao_pvVal);
	STDMETHOD(get_M4ObjectHandle)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_AccessHandle)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_IsClientSide) (/*[out, retval]*/ VARIANT_BOOL* ao_pbVal);

	STDMETHOD(get_RuleTI) (/*[out, retval]*/ BSTR* ao_pstName);

protected:

	ClRunningFormInfo m_oInfo;

};

#endif //__IWRAPRUNNINGFORM_H_
