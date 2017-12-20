// M4LogSys.h: Definition of the M4LogSys class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_M4LOGSYS_H__4A283D1E_CC71_4EED_BFCE_D11A3B928E5C__INCLUDED_)
#define AFX_M4LOGSYS_H__4A283D1E_CC71_4EED_BFCE_D11A3B928E5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// M4LogSys

class M4LogSys : 
	public IDispatchImpl<IM4LogSys, &IID_IM4LogSys, &LIBID_M4LOGSYSVBLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<M4LogSys,&CLSID_M4LogSys>
{
public:
	M4LogSys() {}
BEGIN_COM_MAP(M4LogSys)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IM4LogSys)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(M4LogSys) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_M4LogSys)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IM4LogSys
public:
	STDMETHOD(LogSetCode)(/*[in]*/ long icode, /*[in]*/ short irango, /*[in, optional, defaultvalue("")]*/ BSTR pmessage, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(DestroyLogManager)(/*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(InitializeLogManager)(/*[in]*/ BSTR iniFileName, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(IsEmptyLogStack)(/*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(Log_Pop_Back)(/*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(Disable_TraceInfo_Logs)(/*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(Enable_TraceInfo_Logs)(/*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(GetH_LastErrorText)(/*[out]*/ BSTR* ErrorText, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(GetH_LastErrorTime)(/*[out]*/ BSTR *ErrorTime, /*[out,retval]*/ short *ao_iRetVal);
	STDMETHOD(GetH_LastErrorType)(/*[out]*/ BSTR* ErrorType, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(GetH_DecimalLastErrorCode)(/*[out]*/ long* ErrorCode, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(M4ChWrapGetErrorStringByParamsByLang)(/*[in]*/ long ErrorCode,/*[in, out]*/ SAFEARRAY** ParamsArray, /*[in]*/ unsigned char pos_params, /*[in]*/ IDispatch* oWrapCVM, /*[in, out]*/ BSTR* strError, /*[in]*/ long lLang, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(M4ChWrapGetErrorStringByParams)(/*[in]*/ long ErrorCode, /*[in, out]*/ SAFEARRAY** ParamsArray, /*[in]*/ unsigned char pos_params, /*[in]*/ IDispatch* oWrapCVM, /*[in, out]*/ BSTR* strError, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(M4ChWrapGetErrorStringByLang)(/*[in]*/ long ErrorCode, /*[in]*/ BSTR strParams, /*[in]*/ unsigned char pos_params, /*[in]*/ IDispatch* oWrapCVM, /*[in, out]*/ BSTR* strError, /*[in]*/ long lLang, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(M4ChWrapGetErrorString)(/*[in]*/ long ErrorCode, /*[in]*/ BSTR strParams, /*[in]*/ unsigned char pos_params, /*[in]*/ IDispatch* oWrapCVM, /*[in, out]*/ BSTR* strError, /*[out, retval]*/ short* ao_iRetVal);
	STDMETHOD(M4ChWrapErrorLookup)(/*[in]*/ IDispatch* oWrapCVM, /*[in, out]*/ SAFEARRAY ** ErrorCodes);
	STDMETHOD(DeserializeParameters)(/*[in]*/ BSTR ai_psInString, /*[in, out]*/ long *aio_piInState, /*[in]*/ short ai_iByIdParamPos, /*[out]*/ SAFEARRAY **ai_asParams, /*[out]*/ long *ao_piSource, /*[out]*/ long *ao_piError, /*[out]*/ long *ao_piExtra, /*[in]*/ long *ai_iByParseOption, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(SerializeParameters)(/*[in]*/ SAFEARRAY **ai_asParams, /*[in]*/ long ai_iSource, /*[out]*/ BSTR *ao_psOutString, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(FormatStringByParams)(/*[in]*/ BSTR ai_psInString, /*[in]*/ SAFEARRAY **ai_saParams, /*[in]*/ short ai_iByIdParamPos, /*[out]*/ BSTR *ao_psOutString, /*[out, retval]*/ short *ao_iRetVal);
	STDMETHOD(FormatString)(/*[in]*/ BSTR ai_psInString, /*[in]*/ BSTR ai_psParams, /*[in]*/ short ai_iByIdParamPos, /*[out]*/ BSTR *ao_psOutString, /*[out, retval]*/ short *ao_iRetVal);
};

#endif // !defined(AFX_M4LOGSYS_H__4A283D1E_CC71_4EED_BFCE_D11A3B928E5C__INCLUDED_)
