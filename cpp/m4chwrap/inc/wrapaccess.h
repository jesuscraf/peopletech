//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapaccess.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapAccess
//
//==============================================================================
#include "resource.h"       // main symbols
#include "accevents.hpp"
#include "cpchannelwrapper.h"

#ifndef __WRAPACCESS_H_
#define __WRAPACCESS_H_

class ClAccess;
class ClCMObjectDirectory;
class CWrapExecutor;
/////////////////////////////////////////////////////////////////////////////
// CWrapAccess
class ATL_NO_VTABLE CWrapAccess : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapAccess, &CLSID_WrapAccess>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapAccess, &IID_IWrapAccess, &LIBID_CHANNELWRAPPERLib>,
	public IDispatchImpl<IWrapColNodes, &IID_IWrapColNodes, &LIBID_CHANNELWRAPPERLib>,
	public IDispatchImpl<IWrapStack, &IID_IWrapStack, &LIBID_CHANNELWRAPPERLib>,
    public CProxy_WrapAccessEvents<CWrapAccess>,
    public IConnectionPointContainerImpl<CWrapAccess>,
    public IProvideClassInfo2Impl<&CLSID_WrapAccess, &DIID__WrapAccessEvents, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapAccess();
	~CWrapAccess();

	void Init(ClAccess* ai_VarRef, bool ai_bIsOwner = true);
	void Unwrap(ClAccess** ao_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPACCESS)

BEGIN_COM_MAP(CWrapAccess)
	COM_INTERFACE_ENTRY(IWrapAccess)
	COM_INTERFACE_ENTRY(IWrapColNodes)
	COM_INTERFACE_ENTRY(IWrapStack)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapStack)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapColNodes)
	COM_INTERFACE_ENTRY2(IDispatch, IWrapAccess)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CWrapAccess)
    CONNECTION_POINT_ENTRY(DIID__WrapAccessEvents)
END_CONNECTION_POINT_MAP()

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

public:
	STDMETHOD(ReadTimeStamps)(/*[out]*/ UINT* ao_plSession, /*[out]*/ UINT* ao_plProperties, /*[out]*/ SAFEARRAY** ao_alTimes, /*[out, retval]*/ short* ao_piReturn);
	STDMETHOD(get_LastTimeStamp)(/*[out, retval]*/ ULONGLONG *ao_plTime);
	STDMETHOD(ReadAllTimeStamps)(/*[out]*/ ULONGLONG* ao_plSession, /*[out]*/ ULONGLONG* ao_plProperties, /*[out]*/ SAFEARRAY** ao_alTimes, /*[out, retval]*/ short* ao_piReturn);
	STDMETHOD(ExecuteReport)(/*[in]*/BSTR ai_sCommandString, /*[in]*/short ai_iManager, /*[in]*/short ai_iDesigner, /*[in]*/short ai_iData, /*[in]*/VARIANT ai_vDesigner, /*[out]*/BSTR* ao_psFile, /*[out, retval]*/short* ao_piReturn);
	STDMETHOD(ReadProperties)(/*[out]*/ SAFEARRAY** ao_avValues, /*[out, retval]*/ short* ao_piReturn);
	STDMETHOD(get_GetChannelAsOwner)(/*[out, retval]*/ IWrapChannel** pVal);
	STDMETHOD(LoadFromSttFile)(/*[in]*/ BSTR ai_pstFileName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(PersistToFileAdvanced)(/*[in]*/ BSTR ai_pstFileName, /*[in]*/ VARIANT_BOOL ai_bSerializeMCR, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(GetCurrentNode)(/*[out,retval]*/ IWrapNode ** ao_poNode);
	STDMETHOD(put_SttLabel)(/*[in]*/ BSTR ai_pstName);
	STDMETHOD(get_SttLabel)(/*[out, retval]*/ BSTR* ao_pstLabel);
	STDMETHOD(GetNodeByHandle)(/*[in]*/ long ai_iHandle, /*[out,retval]*/ IWrapNode** ao_poNode);
	STDMETHOD(DeSerialize)(/*[in]*/ BSTR ai_pstFileName, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Serialize)(/*[in]*/ BSTR ai_pstFilename, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_SttStatus)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_SttStatus)(/*[in]*/ short newVal);
	STDMETHOD(get_InstanceMap)(/*[out, retval]*/ IWrapIMap* *pVal);
	STDMETHOD(get_GetChannel)(/*[out, retval]*/ IWrapChannel** pVal);
	STDMETHOD(Free)(/*[out, retval]*/ short* ao_retval);
//EDU: the return of Release:
	STDMETHOD(Release)(/*[out, retval]*/ short* ao_retval);
	STDMETHOD(Load)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(UnLoad)(/*[out, retval]*/ short * ao_sRetVal);
    STDMETHOD(Persist)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(LoadFromFile)(/*[in]*/ BSTR ai_pstFileName, /*[out, retval]*/ short* ao_sRetVal);
    STDMETHOD(PersistToFile)(/*[in]*/ BSTR ai_pstFileName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(PersistToTempFile)(/*[out]*/ BSTR * ao_pstFileName, /*[out, retval]*/ short* ao_sRetVal);

	STDMETHOD(get_RunSliceMode)(/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(put_RunSliceMode)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_WorkDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_WorkDate)(/*[in]*/ DATE newVal);
	STDMETHOD(get_ExecutionStartDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_ExecutionStartDate)(/*[in]*/ DATE newVal);
	STDMETHOD(get_ExecutionEndDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_ExecutionEndDate)(/*[in]*/ DATE newVal);
	STDMETHOD(CheckPoint)(/*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD(get_coProps)(/*[out, retval]*/ IWrapColProp** pVal);
	STDMETHOD(Reset)();
	STDMETHOD(Push)(/*[in]*/ VARIANT ai_varref, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Pop)(/*[out]*/ VARIANT* ao_varref, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_Stack)(/*[out, retval]*/ IWrapStack* *pVal);
	STDMETHOD(get_CoNodes)(/*[out, retval]*/ IWrapColNodes** pVal);
	STDMETHOD(GetRecordSet)(/*[in]*/ BSTR ai_bstrNodeName, /*[out, retval]*/ IWrapRecordSet** ao_poRecordSet);
	STDMETHOD(GetNodeById)(/* [in] */ int ai_iId, /* [out, retval] */ IWrapNode** ao_poNode);
	STDMETHOD(GetNodeByName)(/* [in] */ BSTR ai_bstrName, /* [out, retval] */ IWrapNode** ao_poNode);
	STDMETHOD(get_Count)(/*[out, retval]*/ long* ao_iCount);
	STDMETHOD(get_Handle)(/*[out, retval]*/ long* ao_iHandle);
	STDMETHOD(get_Executor)(/*[out , retval]*/IWrapExecutor **pVal);
    STDMETHOD(DebugDump)(/*[in]*/ BSTR ai_stFilename);
	STDMETHOD(CanUnloadNow)(/*[out, retval]*/ VARIANT_BOOL *pVal);

private:
	ClAccess* m_poAccess;
	IWrapExecutor *m_poExecutor;
	bool m_bIsOwner;
};

#endif //__WRAPACCESS_H_
