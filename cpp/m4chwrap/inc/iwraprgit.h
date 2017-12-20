//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             iwraprgit.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CIWrapRgIt
//
//==============================================================================
#include "resource.h"       // main symbols
#include "m4types.hpp"

#include "iwraprgitfilter.h"

#ifndef __IWRAPRGIT_H_
#define __IWRAPRGIT_H_

class _ClRegister_Base;

/////////////////////////////////////////////////////////////////////////////
// CIWrapRgIt
class ATL_NO_VTABLE CIWrapRgIt : 
	//public CComObjectRootEx<CComSingleThreadModel>,
	//public CComCoClass<CIWrapRgIt, &CLSID_WrapRgIt>,
	public IDispatchImpl<IWrapRgIt, &IID_IWrapRgIt, &LIBID_CHANNELWRAPPERLib>,
	public IDispatchImpl<IEnumVARIANT, &IID_IEnumVARIANT, &LIBID_CHANNELWRAPPERLib>,
    //public IDispatchImpl<IWrapRgItFilter, &IID_IWrapRgItFilter, &LIBID_CHANNELWRAPPERLib>

    public CIWrapRgItFilter
{
public:
	CIWrapRgIt();
	~CIWrapRgIt();

	void Init(_ClRegister_Base* ai_VarRef);
	void SetIsClone(m4bool_t ai_flag);
	m4bool_t GetIsClone(void);
	_ClRegister_Base* GetpRegister(void) { return m_poRegister; }
	

/*
DECLARE_REGISTRY_RESOURCEID(IDR_WRAPRGIT)

BEGIN_COM_MAP(CIWrapRgIt)
	COM_INTERFACE_ENTRY(IWrapRgIt)
	COM_INTERFACE_ENTRY(IEnumVARIANT)
    COM_INTERFACE_ENTRY(IWrapRgItFilter)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapRgIt)
	COM_INTERFACE_ENTRY2(IDispatch, IEnumVARIANT)
    COM_INTERFACE_ENTRY2(IDispatch, IWrapRgItFilter)
END_COM_MAP()
*/

// IWrapRgIt
public:
	STDMETHOD(BSearch)(/*[in]*/ SAFEARRAY ** ai_saArguments, VARIANT_BOOL* ao_pbFound, VARIANT_BOOL* ao_pbVal );
    //pipo
	STDMETHOD(Find)(/*[in]*/ SAFEARRAY ** ai_saArguments, /*[out, retval]*/ long* ao_pbVal );
	STDMETHOD(Destroy)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(DestroyAll)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(DeleteAll)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_ItemIterator)(/*[out, retval]*/ IWrapItIt* *pVal);

    STDMETHOD(CheckPoint)(/*[out, retval]*/ short* ao_RetVal);
    STDMETHOD(Undo)(/*[out, retval]*/ short* ao_RetVal);
    //EDU:
	    STDMETHOD(Free)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_Index)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Operation)(/*[out, retval]*/ IWrapOper* *pVal);
	STDMETHOD(get_IsEof)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(Remove)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_Exist)(/*[out, retval]*/ VARIANT_BOOL* ao_pbVal);
	STDMETHOD(get_New)(/*[out, retval]*/ IWrapRgIt** ao_poRegister);
    //EDU:
        STDMETHOD(get_Insert)(/*[out, retval]*/ IWrapRgIt** ao_poRegister);
	STDMETHOD(get_Count)(/*[out, retval]*/ long* ao_iCount);
	STDMETHOD(Item)(/*[in]*/ int ai_iIndex, /*[out, retval]*/ IWrapRgIt** ao_poRegister);
	STDMETHOD(_NewEnum)(/*[out, retval]*/ IUnknown** ao_unk);
	STDMETHOD(Clone)(/*[out]*/ IEnumVARIANT** ppenum);
	STDMETHOD(Reset)();
	STDMETHOD(Skip)(/*[in]*/ ULONG celt);

	STDMETHOD(Begin)(/*[out, retval]*/ IWrapRgIt** ao_poRegister);
	STDMETHOD(Next) (/*[out, retval]*/ IWrapRgIt** ao_poRegister);
	STDMETHOD(End)  (/*[out, retval]*/ IWrapRgIt** ao_poRegister);
	STDMETHOD(Prev) (/*[out, retval]*/ IWrapRgIt** ao_poRegister);

	STDMETHOD(Next) (/*[in]*/ ULONG celt, /*[in]*/ VARIANT* rgvar, /*[out]*/ ULONG* pceltFetched);

    STDMETHOD(get_Copy)(/*[out, retval]*/ IWrapRgIt* *pVal);
    STDMETHOD(get_Copy_Total)(/*[out, retval]*/ IWrapRgIt* *pVal);

    STDMETHOD(MoveTo)(/*[in]*/ int ai_iIndex, /*[out, retval]*/ short* ao_piReturn);
    STDMETHOD(MoveToEOF)(/*[out, retval]*/ short* ao_piReturn);
    STDMETHOD(MoveToFirst)(/*[out, retval]*/ short* ao_piReturn);

	//filter interface:
    STDMETHOD(get_Filter)(/*[out, retval]*/ IWrapRgItFilter* *pVal);

    STDMETHOD(ReviewNewRegisters)(/*[out, retval]*/ long * ao_iIndex);

    STDMETHOD(CopyRecord)(/*[out]*/ BSTR * ai_pFileName,
                  /*[out, retval]*/ short* ao_sRetVal);

    STDMETHOD(PasteRecord)(VARIANT_BOOL* ai_pbNewRecord,
                           VARIANT_BOOL* ai_pbKeepMarks,
                           BSTR *        ai_pFileName,
                           VARIANT_BOOL* ai_pbDeleteFile,
                           VARIANT_BOOL* ai_pbDestroyRecords,
                   /*[in]*/VARIANT       ai_pExclusionNodeName,
                           VARIANT_BOOL* ai_pbKeepAccesses,
         /*[out, retval]*/ short *       ao_sRetVal);

	STDMETHOD(get_IsLoaded)(/*[out, retval]*/ VARIANT_BOOL* ai_pbIsLoaded);

	STDMETHOD(get_NewNotPersist)(/*[out]*/ VARIANT_BOOL * pbNewNotPersist);
	STDMETHOD(put_NewNotPersist)(/*[in]*/ VARIANT_BOOL * pbNewNotPersist);

	STDMETHOD(get_RecordID)(/*[out, retval]*/ long* ao_plRecordId);
	STDMETHOD(IndexOf)(/*[in]*/ long ai_lRecordId, /*[out, retval]*/ long* ao_piIndex);
	STDMETHOD(ReadRegisters)(/*[out]*/ SAFEARRAY** ao_avValues, /*[out]*/ long* ao_iFirst, /*[out, retval]*/ short* ao_piReturn);

protected:
	m4bool_t m_IsClone;
	_ClRegister_Base* m_poRegister;
	IWrapItIt* m_poItemIterator;
	IWrapOper* m_pOperation;		// Used to cache operation objects
};

#endif //__WRAPRGIT_H_
