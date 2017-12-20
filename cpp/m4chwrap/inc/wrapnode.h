
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapnode.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapNode
//
//==============================================================================
#include "resource.h"       // main symbols
#include "m4types.hpp"

#ifndef __WRAPNODE_H_
#define __WRAPNODE_H_ 

class ClNode;

/////////////////////////////////////////////////////////////////////////////
// CWrapNode
class ATL_NO_VTABLE CWrapNode : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapNode, &CLSID_WrapNode>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapNode, &IID_IWrapNode, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapNode();
	~CWrapNode();
	m4return_t Init(ClNode* ai_varRef);
	void Unwrap(ClNode** ao_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPNODE)

BEGIN_COM_MAP(CWrapNode)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IWrapNode)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWrapNode
public:
	STDMETHOD(get_ItemIterator)(/*[out, retval]*/ IWrapItIt* *pVal);
	STDMETHOD(get_NodeDef)(/*[out, retval]*/ IWrapNodeDef* *pVal);
	STDMETHOD(Load)(/*[out, retval]*/ short* ao_sRetVal);
    STDMETHOD(Persist)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_Operation)(/*[out, retval]*/ IWrapOper* *pVal);
	STDMETHOD(GetCurrentRecordSet)(/*[out, retval]*/ IWrapRecordSet** ao_RecordSet);
	STDMETHOD(get_coItems)(/*[out, retval]*/ IWrapColItems**pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(GetItemCount)(/*[out, retval]*/ int* ao_count);
	STDMETHOD(get_ConnectorType)(BYTE* pVal);
	STDMETHOD(get_ParentNodeId)(BSTR* pVal);
	STDMETHOD(get_ParentNode)(/*[out, retval]*/ IWrapNode* *pVal);
	STDMETHOD(get_AutoLoad)(/*[out, retval]*/ BYTE* pVal);
	STDMETHOD(put_AutoLoad)(/*[in]*/ BYTE newVal);
	STDMETHOD(UnLoad)(/*[in]*/VARIANT_BOOL* pVal, /*[out, retval]*/ short* ao_sRetVal);
    STDMETHOD(SendNodeMark)(/*[in]*/VARIANT_BOOL* pVal);
	STDMETHOD(ReadNodeItems)(/*[out]*/ SAFEARRAY** ao_avValues, /*[out, retval]*/ short* ao_piReturn);

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	ClNode* m_poNode;
	IWrapItIt* m_poItemIterator;
	IWrapColItems* m_piWrapColItems;
};

#endif //__WRAPNODE_H_
