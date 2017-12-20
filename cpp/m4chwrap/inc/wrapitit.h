//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapitit.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapItIt
//
//==============================================================================

#include "resource.h"       // main symbols
#include "m4types.hpp"

#ifndef __WRAPITIT_H_
#define __WRAPITIT_H_

class ClItem;

/////////////////////////////////////////////////////////////////////////////
// CWrapItIt
class ATL_NO_VTABLE CWrapItIt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapItIt, &CLSID_WrapItIt>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapItIt, &IID_IWrapItIt, &LIBID_CHANNELWRAPPERLib>,
	public IDispatchImpl<IEnumVARIANT, &IID_IEnumVARIANT, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapItIt();
	~CWrapItIt();

	void Init(ClItem* ai_poItem);
	void SetIsClone(m4bool_t ai_flag);
	m4bool_t GetIsClone(void);


DECLARE_REGISTRY_RESOURCEID(IDR_WRAPITIT)

BEGIN_COM_MAP(CWrapItIt)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IWrapItIt)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapItIt)
	COM_INTERFACE_ENTRY2(IUnknown, IEnumVARIANT)
END_COM_MAP()

// IWrapItIt
public:
	STDMETHOD(SetValue)(/*[in]*/ VARIANT ai_oVariant, /*[out]*/ VARIANT* ao_poVariant, /*[out]*/ ULONGLONG* ao_plOldTime, /*[out]*/ ULONGLONG* ao_plNewTime, /*[out, retval]*/ short* ao_piReturn);
	STDMETHOD(CallTrackingChanges)(/*[in]*/ int ai_iNumArgs, /*[out]*/ VARIANT_BOOL *ao_pbChanges, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_Copy)(/*[out, retval]*/ IWrapItIt* *pVal);
	STDMETHOD(Call)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(CallWithArgs)(/*[in]*/ int ai_iNumArgs, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_OldValue)(/*[out, retval]*/ VARIANT* pVal);
//	STDMETHOD(get_Binary)(/*[out, retval]*/ VARIANT* pVal);
//	STDMETHOD(put_Binary)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IdItem)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_ItemDef)(/*[out, retval]*/ IWrapItemDef* *pVal);
	STDMETHOD(_NewEnum)(/*[out, retval]*/ IUnknown** ao_unk);
	STDMETHOD(Item)(/*[in]*/ VARIANT ai_var, /*[out,retval]*/ IWrapItIt** ao_item);
	STDMETHOD(get_Count)(/*[out, retval]*/ long* pVal);
	STDMETHOD(Clone)(/*[out]*/ IEnumVARIANT** ppenum);
	STDMETHOD(Reset)();
	STDMETHOD(Skip)(/*[in]*/ ULONG celt);
	STDMETHOD(Next)(/*[in]*/ ULONG celt, /*[in]*/ VARIANT* rgvar, /*[out]*/ ULONG* pceltFetched);
	STDMETHOD(get_SliceIterator)(IWrapSlIt** pVal);
	STDMETHOD(get_RecordSetFooterValue)(/*[in]*/int ai_operation,/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(get_SliceFooterValue)(/*[in]*/int ai_operation,/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(get_Executor)(/*[out , retval]*/IWrapExecutor **pVal);

	void UnWrap (ClItem ** ao_poItem);

// ISupportsErrorInfo
public:
	STDMETHOD(GetDescr)(/*[out, retval]*/ BSTR *Descr);
	STDMETHOD(SetDescr)(/*[in]*/ BSTR Descr);
	STDMETHOD(ChangedMask)(/*[out, retval]*/ long *pMask);
	STDMETHOD(CreateBlobFile)(/*[out]*/ VARIANT_BOOL *pRet, /*[in]*/ BSTR DirName, /*[in]*/ BSTR Extension);
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
	STDMETHOD(ReadItemIndexes)(/*[out]*/ SAFEARRAY** ao_avValues, /*[out, retval]*/ short* ao_piReturn);

private:
	m4bool_t m_IsClone;

	m4return_t GetItemByPos(int ai_pos, IWrapItIt** ao_item);
	m4return_t GetItemByName(BSTR ai_name, IWrapItIt** ao_item);

	ClItem* m_poItem;
	IWrapSlIt* m_poSliceIterator;

	IWrapItemDef* m_pItemDef; // Used to cache ItemDef objects
};

#endif //__WRAPITIT_H_
