//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapslit.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapSlIt
//
//==============================================================================
#include "resource.h"       // main symbols
#include "m4types.hpp"

#ifndef __WRAPSLIT_H_
#define __WRAPSLIT_H_

class ClValue;

/////////////////////////////////////////////////////////////////////////////
// CWrapSlIt
class ATL_NO_VTABLE CWrapSlIt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapSlIt, &CLSID_WrapSlIt>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapSlIt, &IID_IWrapSlIt, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapSlIt();
	~CWrapSlIt();

	void Init(ClValue* ai_VarRef);
	void SetIsClone(m4bool_t ai_flag);
	m4bool_t GetIsClone(void);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPSLIT)

BEGIN_COM_MAP(CWrapSlIt)
	COM_INTERFACE_ENTRY(IWrapSlIt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// IWrapSlIt
public:
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_StartDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_EndDate)(/*[in]*/ DATE ai_date);
	STDMETHOD(get_EndDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_StartDate)(/*[in]*/ DATE ai_date);
	STDMETHOD(HasSlices)(/*[out, retval]*/ VARIANT_BOOL* ao_pb);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
	STDMETHOD(RemoveAll)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(Add)(/*[in]*/ DATE ai_dStartDate, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(Remove)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(_NewEnum)(/*[out, retval]*/ IUnknown** ao_unk);
	STDMETHOD(Clone)(/*[out]*/ IEnumVARIANT** ppenum);
	STDMETHOD(Reset)();
	STDMETHOD(Skip)(/*[in]*/ ULONG celt);
	STDMETHOD(Next)(/*[in]*/ ULONG celt, /*[in]*/ VARIANT* rgvar, /*[out]*/ ULONG* pceltFetched);
	STDMETHOD(get_Copy)(/*[out, retval]*/ IWrapSlIt* *pVal);
	STDMETHOD(Item)(VARIANT ai_var, IWrapSlIt** ao_item);

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	m4return_t GetSliceByPos(/*[in]*/ int ai_iId, /*[out, retval]*/ IWrapSlIt** ao_poWrapSlice);
	m4return_t GetSliceByDate(/*[in]*/ DATE ai_dDate, /*[out, retval]*/ IWrapSlIt** ao_poWrapSlice);

	m4bool_t m_IsClone;
	ClValue* m_poValue;
};

#endif //__WRAPSLIT_H_
