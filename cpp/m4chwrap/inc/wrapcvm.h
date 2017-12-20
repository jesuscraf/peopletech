//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapcvm.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapCVM
//
//==============================================================================

#include "resource.h"       // main symbols
#include "dm.hpp"
#include "cachefac.hpp"
#include "vmcscontext.hpp"
#include "logcomobject.h"
#include "sessionpinger.hpp"


#ifndef __WRAPCVM_H_
#define __WRAPCVM_H_


class CWrapBookMark;
class CWrapExecutor;
class ClM4ObjService;
class M4ClLoaDll;


template <class T>
class CProxy_WrapCVMEvents : public IConnectionPointImpl<T, &DIID__WrapCVMEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:

	VOID Fire_MessageBox(BSTR ai_sTitle, BSTR ai_sMessage, INT ai_iType, INT* ao_piReturn)
	{
		*ao_piReturn = -1;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[3];
		int nConnections = m_vec.GetSize();
		VARIANT VarResult;

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[2] = ai_sTitle;
				pvars[1] = ai_sMessage;
				pvars[0] = ai_iType;
				DISPPARAMS disp = { pvars, NULL, 3, 0 };
				VarResult.lVal = *ao_piReturn;
				pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &VarResult, NULL, NULL);

				if (VarResult.lVal != *ao_piReturn)
				{
					*ao_piReturn = VarResult.lVal;
					break;
				}
			}
		}
		delete[] pvars;
	}

	VOID Fire_CountdownForTimeOut(INT ai_iSeconds, INT* ao_piReturn)
	{
		*ao_piReturn = -1;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();
		VARIANT VarResult;

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[0] = ai_iSeconds;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				VarResult.lVal = *ao_piReturn;
				pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &VarResult, NULL, NULL);

				if (VarResult.lVal != *ao_piReturn)
				{
					*ao_piReturn = VarResult.lVal;
					break;
				}
			}
		}
		delete[] pvars;
	}

	VOID Fire_CppException(BSTR ai_sDumpFile, INT* ao_piReturn)
	{
		*ao_piReturn = -1;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[3];
		int nConnections = m_vec.GetSize();
		VARIANT VarResult;

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[0] = ai_sDumpFile;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				VarResult.lVal = *ao_piReturn;
				pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &VarResult, NULL, NULL);

				if (VarResult.lVal != *ao_piReturn)
				{
					*ao_piReturn = VarResult.lVal;
					break;
				}
			}
		}
		delete[] pvars;
	}
};



/////////////////////////////////////////////////////////////////////////////
// CWrapCVM
class ATL_NO_VTABLE CWrapCVM : 
	public IM4MessageBoxProvider, IM4TimeoutEventProvider,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapCVM, &CLSID_WrapCVM>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapCVM, &IID_IWrapCVM, &LIBID_CHANNELWRAPPERLib>,
	public CProxy_WrapCVMEvents<CWrapCVM>,
	public IConnectionPointContainerImpl<CWrapCVM>,
    public IProvideClassInfo2Impl<&CLSID_WrapCVM, &DIID__WrapCVMEvents, &LIBID_CHANNELWRAPPERLib>
{
	DECLARE_LOG_AGGREGATABLE(CWrapCVM)
public:
	CWrapCVM();
	~CWrapCVM();

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPCVM)

BEGIN_COM_MAP(CWrapCVM)
	COM_INTERFACE_ENTRY(IWrapCVM)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

// IWrapCVM
public:
	STDMETHOD(Convert)(BSTR ai_pcInString, BSTR *ao_pcOutString, short* ao_RetVal);
	STDMETHOD(ConvertOneWay)(BSTR ai_pcInString, BSTR *ao_pcOutString, short* ao_RetVal);
	STDMETHOD(IsDCEnabled)(/*[out, retval]*/ VARIANT_BOOL *ao_pFlag);
	STDMETHOD(SetDCEnabled)(/*[in]*/ VARIANT_BOOL ai_Flag);
	STDMETHOD(GetDCData)(/*[in]*/ /*[out]*/ IWrapBookMark *pBookMark, /*[out]*/ long *pNodeHandle, /*[out]*/ long *pItemHandle);
	STDMETHOD(IsDCData)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(SetCSTimeoutFunction)(/*[in]*/ BSTR sDLLName, /*[in]*/ BSTR sFuncName);
	STDMETHOD(ResetNextCSTimeOut)();
	STDMETHOD(GetGenCSTimeOut)(long *piTimeOut);
	STDMETHOD(GetNextCSTimeOut)(long *piTimeOut);
	STDMETHOD(GetDefaultCSTimeOut)(long *piTimeOut);
	STDMETHOD(SetGenCSTimeOut)(long iTimeOut);
	STDMETHOD(SetNextCSTimeOut)(long iTimeOut);
	STDMETHOD(SetDefaultCSTimeOut)(long iTimeOut);
	STDMETHOD(EnableTimeOut)(short);
	STDMETHOD(DebugMemLDB)(BSTR);
	STDMETHOD(DebugCounters)(short* ao_RetVal);
	STDMETHOD(get_EmulationLevel)(/*[out, retval]*/ EMULATE_TYPE *pVal);
	STDMETHOD(CleanDataCache)(short * ao_RetVal);
	STDMETHOD(CleanPresentationCache)(short * ao_RetVal);
	STDMETHOD(CleanCSCRCache)(short * ao_RetVal);
	STDMETHOD(CleanCMCRCache)(short * ao_RetVal);
	STDMETHOD(CleanLDB)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(CacheClean)(/*[in]*/CACHETYPE CacheType, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(CacheRemoveByObjectId)(/*[in]*/CACHETYPE CacheType, /*[in]*/ BSTR ai_stObjectId, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(NewCacheRemoveByObjectId)(/*[in]*/CACHETYPE CacheType, /*[in]*/ BSTR ai_stObjectId, /*[in]*/ VARIANT_BOOL ai_bLocalOnly, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(CacheRefresh)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(CleanAllCaches)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_SessionChannel)(/*[out, retval]*/ IWrapChannel* *pVal);
	STDMETHOD(SetRoleByPos)(/*[in]*/ long ai_iPos, /*[in, defaultvalue(VARIANT_FALSE)]*/ VARIANT_BOOL ai_bSetAsDefaultRole, /*[out,retval]*/short* ao_RetVal);
	STDMETHOD(get_NumRoles)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_RoleIDByPos)(/*[in]*/ long ai_iPos, /*[out, retval]*/ BSTR* pVal);
	STDMETHOD(get_RoleNameByPos)(/*[in]*/ long ai_iPos, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(ChangePassword)(/*[in]*/ BSTR ai_stOldPassword, /*[in]*/ BSTR ai_stNewPassword, /*[out,retval]*/ short* ao_RetVal);
	STDMETHOD(Logoff)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Logon)(/*[in]*/ BSTR ai_stUsername, /*[in]*/ BSTR ai_stPassword, /*[in]*/ long ai_iLanguage, /*[in]*/ BSTR ai_stLogonServer, /*[in]*/ long ai_iPortNumber, /*[in]*/ VARIANT_BOOL ai_bUseDefaultRole, /*[out,retval]*/ short* ao_RetVal);
	STDMETHOD(RegisterChannel)(/*[in]*/ IWrapChannel* ai_Channel, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(CreateChannel)(/*[out, retval]*/ IWrapChannel** ao_channel);
	STDMETHOD(Shutdown)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(Startup)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD(get_coProps)(/*[out, retval]*/ IWrapColProp** pVal);
	STDMETHOD(AttachDocNodeToTrackNode)(/*[in]*/ IWrapNode* ai_poWrapDocNode,/*[in]*/ IWrapNode* ai_poWrapTrackNode, /*[out, retval]*/ short* ao_pRetVal);
	STDMETHOD(DetachDocNodeToTrackNode)(/*[in]*/ IWrapNode* ai_poWrapDocNode,/*[in]*/ IWrapNode* ai_poWrapTrackNode, /*[out, retval]*/ short* ao_pRetVal);
	STDMETHOD(get_VMSign)(/*[out, retval]*/ IWrapVMSign* *pVal);
	STDMETHOD(get_DefaultCSType)(/*[out, retval]*/ CSTYPE *pVal);
	STDMETHOD(put_DefaultCSType)(/*[in]*/ CSTYPE newVal);
	STDMETHOD(get_ProxyListIt)(/* [retval][out] */ IWrapProxyListIt** pVal);
	STDMETHOD(GetProxyDebugView)(/* [in] */ BSTR ai_LocalId, /* [in] */ BSTR ai_RemoteId, /* [retval][out] */ IWrapChannel** pVal);
	STDMETHOD(GetMultiValidationChannel)(/*[out, retval]*/ IWrapChannel** ao_poChannel);
	STDMETHOD(get_Executor)(/*[out , retval]*/IWrapExecutor **pVal);

	STDMETHOD(CSTraceEnable)(/*[in]*/ VARIANT_BOOL ai_EnableIt);
	STDMETHOD(CSTraceBeginTag)(/*[in]*/ BSTR ai_stNewTag);
	STDMETHOD(CSTraceResetTag)();
	STDMETHOD(CSTraceSetOutputFileName)(/*[in]*/ BSTR ai_stFileName);
	STDMETHOD(CSTraceGetOutputFileName)(/*[out]*/ BSTR* ao_stFileName);
	STDMETHOD(CSTraceShowCachedOperations)(/*[in]*/ VARIANT_BOOL ai_ShowIt);

 	STDMETHOD(GetChannelCount)(/*[out, retval] */long *ao_iCount);
	STDMETHOD(GetChannelByPosition)(/*[in]*/ long ai_iPosition, /*[out, retval]*/ IWrapChannel** ao_channel);

	STDMETHOD(get_BuildVersion)(/*[out, retval]*/ BSTR* pVal);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	STDMETHOD(GetAccessByHandle)(/*[in]*/ long ai_lHandle, /*[out, retval]*/ IWrapAccess** ao_poAccess);
	STDMETHOD(ReportViewerEnd)(/*[in]*/ unsigned long ai_lTaskId, /*[out]*/ BSTR *ao_pcTrace, /*[out]*/ short *ao_plErrors, /*[out]*/ short *ao_plWarnings, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(ReportViewerInit)(/*[in]*/ BSTR ai_pcCommand, /*[in]*/ short ai_iLinkManager, /*[in]*/ short ai_iLinkDesign, /*[in]*/ short ai_iLinkData, /*[in]*/ IWrapAccess* ai_poDesignAccess, /*[in]*/ IWrapAccess* ai_poDataAccess, /*[out]*/ unsigned long *ao_plTaskId, /*[out]*/ BSTR *ao_pcOutput, /*[out]*/ short *ao_plErrors, /*[out]*/ short *ao_plWarnings, /*[out]*/ unsigned long *ao_plBreaks, /*[out]*/ short *ao_piMoreBreaks, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(RD_OpenSession)(/*[in]*/ short ai_iVersion, /*[in]*/ BSTR ai_pcCommand, /*[in]*/short ai_iLinkManager, /*[in]*/  IWrapAccess * ai_poAccess, /*[out]*/ long * ao_plSessionHandler, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(get_CacheDirectory)(/*[out, retval]*/ BSTR *ao_sDirectory);
	STDMETHOD(ArrayToPropertyBag)(/*[in]*/ BSTR ai_sOrganization, /*[in]*/ SAFEARRAY** ai_abArray, /*[out, retval]*/ ICPropBag ** ao_ppoPropertyBag);
	STDMETHOD(AddUserAction)(/*[in]*/ BSTR ai_sEvent, /*[in]*/ BSTR ai_sObjectId, /*[in]*/ BSTR ai_sObjectName, /*[in]*/ BSTR ai_sArguments, /*[in]*/ BSTR ai_sOrganization, /*[in]*/ BSTR ai_sRole, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(get_SessionTimeOut)(/*[out, retval]*/ double *pVal);
	STDMETHOD(ReplaceLiterals)(/*[in]*/ BSTR ai_sInput, /*[out]*/ BSTR *ai_psOutput, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(ReplaceOrgLiterals)(/*[in]*/ BSTR ai_sInput, /*[out]*/ BSTR *ai_psOutput, /*[in]*/ BSTR ai_sOrganization, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(DeleteRegistryKey)(/*[in]*/ BSTR ai_sKey, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(SaveRegistry)(/*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(GetRegistrySubKeyNames)(/*[in]*/ BSTR ai_sKey, /*[out]*/ SAFEARRAY** ao_psaKeys, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(GetRegistryValueNames)(/*[in]*/ BSTR ai_sKey, /*[out]*/ SAFEARRAY **ao_psaValues, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(SetRegistryValue)(/*[in]*/ BSTR ai_sKey, /*[in]*/ BSTR ai_sValue, /*[in]*/ BSTR ai_sData, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(GetRegistryValue)(/*[in]*/ BSTR ai_sKey, /*[in]*/ BSTR ai_sValue, /*[out]*/ BSTR *ai_psData, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(BuildXMLFromId)(/*[in]*/ BSTR ai_sM4ObjId, /*[in]*/ long ai_lMask, /*[in]*/ short ai_CsType, /*[in]*/ short ai_iLanguage, /*[out]*/ SAFEARRAY** ao_psaXML, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(LogClean)();
	STDMETHOD(LogGet)(/*[in]*/ long ai_lLanguage, /*[in]*/ long ai_lPosition, /*[out]*/ long *ao_plCode, /*[out]*/ short *ao_piType, /*[out]*/ DATE *ao_pdTime, /*[out]*/ BSTR *ao_psTextById, /*[out]*/ BSTR *ao_psTextByName, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(LogCount)(/*[out]*/ long *ao_plCount);
	STDMETHOD(PrintVBTrace)(/*[in]*/ SAFEARRAY ** ai_saArguments, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(IsEqualOrg)(/*[in]*/ BSTR ai_pcMapId, /*[in]*/ BSTR ai_pcOrganization1, /*[in]*/ BSTR ai_pcOrganization2, /*[in]*/ BSTR ai_pcDestination, /*[out]*/ VARIANT_BOOL *ao_bIsEquivalent, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(GetMappingById)(/*[in]*/ BSTR ai_pcMapId, /*[in]*/ BSTR ai_pcOrganization, /*[in]*/ BSTR ai_pcSource, /*[out]*/ BSTR *ao_pcMapping, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(CleanMapCache)(short * ao_RetVal);
	STDMETHOD(SetDownloadCallBack)(long ai_fCallBack);
	STDMETHOD(IsAllowed)(/*[in]*/ long ai_pILicenseManager, /*[in]*/ BSTR ai_sMeta4Object, /*[in]*/ BSTR ai_sIdNodeStructure, /*[in]*/ BSTR ai_sIdItem, /*[out]*/ short *ao_bIsAllowed, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(GetPropertyTransById)(/*[in]*/ long ai_pILicenseManager, /*[in]*/ BSTR ai_pPropertyId, /*[out]*/ BSTR * ao_pPropertyTrans, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(GetPropertyValueById)(/*[in]*/ long ai_pILicenseManager, /*[in]*/ BSTR ai_pPropertyId, /*[out]*/ BSTR * ao_pPropertyValue, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(GetStateValidateExecutionNp)(/*[in]*/ long ai_pILicenseManager, /*[out]*/ short *ao_State, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(GetpILicenseManager)(/*[out]*/ long * ao_pILicenseManager, /*[out, retval]*/ short *ao_RetVal);
	STDMETHOD(get_SessionContext)(BSTR* ao_sX509Certificate, BSTR* ao_sSessionContext, /*[out, retval]*/ short *pVal);
	static m4bool_t IsStepperDebugging(void);
	STDMETHOD(SetCallBack)(long ai_fCallBack);
	STDMETHOD(SentenceMerge)(BSTR ai_stChild, BSTR ai_stParent, BSTR* ao_stResult, short* ao_pReturn);
	STDMETHOD(BuildPresentationDefFromID)(/*[in]*/ BSTR ai_PresentationID, /*[in]*/ BSTR ai_Organization, /*[in]*/ BSTR ai_UseID, /*[out]*/ SAFEARRAY** ao_Presentation, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(BuildPresentationFromID)(/*[in]*/ BSTR ai_sPresentationId, /*[in]*/ BSTR ai_sOrganization, /*[in]*/ BSTR ai_sRoleId, /*[in]*/ short ai_iLanguage, /*[in]*/ VARIANT_BOOL ai_bDefinition, /*[out]*/ SAFEARRAY ** ao_abPresentation, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(BuildPresFromIdByLangType)(/*[in]*/ BSTR ai_PresentationId, /*[in]*/ BSTR ai_Organization, /*[in]*/ BSTR ai_UseId, /*[in]*/ short ai_iLangType, /*[out]*/ SAFEARRAY **ao_Presentation, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(BuildPresFromIdByLang)(BSTR ai_PresentationId, /*[in]*/ BSTR ai_Organization, /*[in]*/ BSTR ai_UseId, /*[in]*/ short ai_iLanguage, /*[out]*/ SAFEARRAY **ao_Presentation, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(GetXMLFromPresentationId)(/*[in]*/ BSTR ai_PresentationID, /*[in]*/ BSTR ai_Organization, /*[in]*/ BSTR ai_UseID, BSTR* ao_XML, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(BuildTypedPresentationFromId)(/*[in]*/ BSTR ai_sPresentationId, /*[in]*/ BSTR ai_sOrganization, /*[in]*/ BSTR ai_sRoleId, /*[in]*/ short ai_iLanguage, /*[in, out]*/ DATE* aio_dtDate, /*[out]*/ SAFEARRAY** ao_abPresentation, /*[in, out]*/ short* aio_iType, /*[out]*/ short* ao_iStyle, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(RestoreLogon)(BSTR ai_stCredential, long ai_iLanguage, BSTR ai_stLogonServer, long ai_iPortNumber, BSTR ai_stSSL, VARIANT_BOOL ai_bUseSSL, short * ao_RetVal);
	STDMETHOD(get_Credential)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IsMultiOrganization)(/*[out, retval]*/ short *pVal);
	STDMETHOD(SetConnectionParam)(/*[in]*/ BSTR ai_stParamName, /*[in]*/ BSTR ai_stParamValue, /*[out, retval]*/ short * ao_iRetVal);
	STDMETHOD(ClearFiles)();
	STDMETHOD(NewLogon)(BSTR ai_stUsername, /*[in]*/ BSTR ai_stPassword, /*[in]*/ long ai_iLanguage, /*[in]*/ BSTR ai_stLogonServer, /*[in]*/ long ai_iPortNumber, /*[in]*/ VARIANT_BOOL ai_bUseDefaultRole, /*[in]*/ BSTR ai_stSSL, /*[in]*/ VARIANT_BOOL ai_bUseSSL, /*[out,retval]*/ short* ao_RetVal);
	ClChannelManager *GetpChannelManager(void) { return m_poDM; }
	STDMETHOD(get_PtrFullInterface)(/*[out, retval]*/ long* retVal);
	STDMETHOD(AddUserTimedAction)(/*[in]*/ long ai_iEventNum, /*[in]*/ DATE aio_dtEventTime, /*[in]*/ BSTR ai_sOrganization, /*[in]*/ BSTR ai_sEventSenderContainer, /*[in]*/ BSTR ai_sEventSender, /*[in]*/ BSTR ai_sParentEventSenderContainer, /*[in]*/ BSTR ai_sParentEventSender, /*[in]*/ long ai_iTimeElapsed, /*[in]*/ BSTR ai_sBuffer, /*[in]*/ VARIANT_BOOL ai_bIsValid, /*[out, retval]*/ short *ao_piReturn);
	STDMETHOD(GetEventsCounter)(/*[out, retval]*/ long *ao_plCounter);
	STDMETHOD(ReplaceCache)(/*[in]*/ BSTR ai_stInstallHash, /*[out, retval]*/ short *ao_piReturn);
	
	m4int_t	OnMessageBox(m4pcchar_t ai_pccTitle, m4pcchar_t ai_pccMessage, m4uint32_t ai_iType);
	m4int_t	OnCountdownForTimeOut(m4int_t ai_iSeconds);
	m4int_t	OnCppException(m4pcchar_t ai_pccDumpFile);

private:
	ClChannelManager *m_poDM; 
	IWrapExecutor	 *m_poExecutor;
	ClM4ObjService   *m_poObjService;

public :

BEGIN_CONNECTION_POINT_MAP(CWrapCVM)
	CONNECTION_POINT_ENTRY(DIID__WrapCVMEvents)
END_CONNECTION_POINT_MAP()

};


#endif //__WRAPCVM_H_
