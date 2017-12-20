
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapitem.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapItem
//
//==============================================================================
#include "resource.h"       // main symbols
#include "index.hpp"

#ifndef __WRAPITEM_H_
#define __WRAPITEM_H_

class ClItem;

/////////////////////////////////////////////////////////////////////////////
// CWrapItem
class ATL_NO_VTABLE CWrapItem : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapItem, &CLSID_WrapItem>,
	public IDispatchImpl<IWrapItem, &IID_IWrapItem, &LIBID_CHANNELWRAPPERLib>,
	public IDispatchImpl<IWrapColSlices, &IID_IWrapColSlices, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapItem();
	~CWrapItem();

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPITEM)

BEGIN_COM_MAP(CWrapItem)
	COM_INTERFACE_ENTRY(IWrapItem)
	COM_INTERFACE_ENTRY(IWrapColSlices)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapItem)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapColSlices)
END_COM_MAP()


// IWrapItem
public:
	STDMETHOD(GetDescr)(/*[out, retval]*/ BSTR *Descr);
	STDMETHOD(SetDescr)(/*[in]*/ BSTR Descr);
	STDMETHOD(ChangedMask)(/*[out, retval]*/ long *pMask);
	STDMETHOD(CreateBlobFile)(/*[out]*/ VARIANT_BOOL *pRet, /*[in]*/ BSTR DirName, /*[in]*/ BSTR Extension);
	STDMETHOD(Call)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_coSlices)(/*[out, retval]*/ IWrapColSlices** pVal);
	STDMETHOD(Init)(/*[in]*/ VARIANT ai_varRef);
	STDMETHOD(get_OldValue)(/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IdItem)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_FooterValue)(/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(get_ItemDef)(/*[out, retval]*/ IWrapItemDef* *pVal);
	STDMETHOD(GetSliceByPos)(/*[in]*/ int ai_iId, /*[out, retval]*/ IWrapSlice** ao_poWrapSlice);
	STDMETHOD(HasSlices)(/*[out, retval]*/ VARIANT_BOOL* ao_pb);
	STDMETHOD(GetSliceByDate)(/*[in]*/ DATE ai_dDate, /*[out, retval]*/ IWrapSlice** ao_poWrapSlice);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
	STDMETHOD(DeleteAllSlices)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(AddSlice)(/*[in]*/ DATE ai_dStartDate, /*[out, retval]*/ short* ao_sRetVal);

private:
	ClItem* m_poItem;
	ClItemIndex m_oIndex;
};

#endif //__WRAPITEM_H_
