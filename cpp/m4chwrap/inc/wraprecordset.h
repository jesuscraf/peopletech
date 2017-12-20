
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wraprecordset.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapRecordSet
//
//==============================================================================

#include "resource.h"       // main symbols

#ifndef __WRAPRECORDSET_H_
#define __WRAPRECORDSET_H_

class ClAccessRecordSet;

/////////////////////////////////////////////////////////////////////////////
// CWrapRecordSet
class ATL_NO_VTABLE CWrapRecordSet : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapRecordSet, &CLSID_WrapRecordSet>,
	public IDispatchImpl<IWrapRecordSet, &IID_IWrapRecordSet, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapRecordSet();
	~CWrapRecordSet();
	void Init(ClAccessRecordSet* ai_VarRef);
	void Unwrap(ClAccessRecordSet** ao_VarRef);
	
DECLARE_REGISTRY_RESOURCEID(IDR_WRAPRECORDSET)

BEGIN_COM_MAP(CWrapRecordSet)
	COM_INTERFACE_ENTRY(IWrapRecordSet)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWrapRecordSet
public:
	STDMETHOD(Arrange)(/*[out, retval]*/ short* ao_Retval);
	STDMETHOD(SetCurrentSearchIndexById)(/*[in]*/ long ai_IndexId, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(SetCurrentSearchIndexByPos)(/*[in]*/ short ai_iPos, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Destroy)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_ItemIterator)(/*[out, retval]*/ IWrapItIt* *pVal);
	STDMETHOD(get_RegisterIterator)(/*[out, retval]*/ IWrapRgIt* *pVal);
	STDMETHOD(get_CurrentIterator)(/*[out, retval]*/ IWrapRgIt* *pVal);
	STDMETHOD(Load)(/*[out, retval]*/ short* ao_sRetVal);
    STDMETHOD(Persist)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(CheckPoint)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(Undo)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_Operation)(/*[out, retval]*/ IWrapOper* *pVal);
	STDMETHOD(get_coItems)(/*[out, retval]*/ IWrapColItems** pVal);
	STDMETHOD(MoveCurrent)(/*[in]*/ BSTR ai_stBase, /*[in]*/ long ai_iOffset, /*[out, retval]*/ VARIANT_BOOL* ao_pb);
	STDMETHOD(SetCurrentRegister)(/*[in]*/ IWrapRegister* ai_poRegister, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(GetCurrentRegister)(/*[out, retval]*/ IWrapRegister** ao_poRegister);
// Blow this away..
	STDMETHOD(InsertRegister)(/*[in]*/ IWrapRegister* ai_iRegister);
	STDMETHOD(FindRegister)(/*[out, retval]*/ IWrapRegister** ao_piRegister);
	STDMETHOD(ExistRegister)(/*[out, retval]*/ VARIANT_BOOL* ao_pbVal);
	STDMETHOD(NewRegister)(/*[out, retval]*/ IWrapRegister** ao_poRegister);
	STDMETHOD(GetRecordCount)(/*[out, retval]*/ int* ao_iCount);
	STDMETHOD(GetRegister)(/*[in]*/ int ai_iIndex, /*[out, retval]*/ IWrapRegister** ao_poRegister);

    STDMETHOD(get_Copy)(IWrapRecordSet** pVal);
    STDMETHOD(Follow)(IWrapRecordSet** pVal);
    
// blown away...

    STDMETHOD(SendBlockMark)(/*[in]*/VARIANT_BOOL* pVal);
	STDMETHOD(ReadBlockItems)(/*[out]*/ SAFEARRAY** ao_avValues, /*[out]*/ short* ao_piLastLoadStatus, /*[out]*/ long* ao_piLastLoadRows, /*[out]*/ long* ao_piLastLoadFetchs, /*[out, retval]*/ short* ao_piReturn);
	STDMETHOD(ReadChildrenTimeStamps)(/*[out]*/ SAFEARRAY** ao_alTimes, /*[out, retval]*/ short* ao_piReturn);

private:
	ClAccessRecordSet* m_poRecordSet;
	IWrapRgIt* m_poRegisterIterator;
	IWrapRgIt* m_poCurrentIterator;
	IWrapItIt* m_poItemIterator;
};


#endif //__WRAPRECORDSET_H_
