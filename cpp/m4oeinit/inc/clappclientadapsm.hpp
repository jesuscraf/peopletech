//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clappclientadapsm.hpp   
// Project:             Mind3x      
// Author:              Meta Software M.S. , S.A
// Date:                19/02/1997
// Language:            C++
//
// Definition:
//    This module declares the interface to the AppClient Adaptor
//
//==============================================================================

// Esta definición es específica para SM

#ifndef _CLAPPCLIENTADAPSM_HPP_
#define _CLAPPCLIENTADAPSM_HPP_


#include "clappclientadap.hpp"

#define M4_CLIENTAPADPTSM_SERVICE_NULL						0
#define M4_CLIENTAPADPTSM_SERVICE_CONTEXT_OLTP				1
#define M4_CLIENTAPADPTSM_SERVICE_CONTEXT_PROXY				2
#define M4_CLIENTAPADPTSM_SERVICE_CONTEXT_MD				3

class ClLdb_Wrapper;
class ClCache;
class ClM4ObjServiceClientSM;
class ClM4ObjService;

class ClAppClientAdapSM : public ClAppClientAdap{

public:
	ClAppClientAdapSM();
	~ClAppClientAdapSM();
	m4return_t Init(ClM4ObjServiceClientSM* ai_ObjServiceSM, ClCache *ai_pCacheCMCR, ClCache *ai_pCacheCSCR, ClCache *ai_pCachePres, ClCache *ai_pCacheMap, ClCache *ai_pCacheData);

	m4return_t Init (m4int32_t ai_uiTimeout){return M4_ERROR;}
	m4return_t End ();

// Para la ventana del time-out =====================
#ifdef _WINDOWS
	void SetTimeoutFunction(ClTimeoutFunction ai_hCSTimeoutProcedure) {};

	void SetServerEventFunction(ClServerEventFunction ai_hCSServerEventProcedure) {};
#endif //_WINDOWS
// ===========================================

// Para el logon adaptor =====================
	m4return_t	OpenSession( M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strUsername, M4ClString &ai_strPasswd, m4bool_t ai_bDefaultRole, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert )
	{
		if (m_hSessionId == 0) //Solo abrimos una sesion
		{
			m_hSessionId = 1;
			return M4_TRUE;
		}
		return M4_ERROR;
	}

	m4return_t	OpenSession( M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strCredential,													  ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert )
	{
		if (m_hSessionId == 0) //Solo abrimos una sesion
		{
			m_hSessionId = 1;
			return M4_TRUE;
		}
		return M4_ERROR;
	}

	m4return_t	EnableRole( m4pchar_t ai_pRoleId = NULL, m4bool_t ai_bDefault = M4_FALSE)
	{
		if (m_hSessionId == 0) //Tenemos que tener abierta una sesion
			return M4_ERROR;
		return M4_SUCCESS; //??
	}

	m4return_t	ChangePassword( M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd )
	{
		if (m_hSessionId == 0) //Tenemos que tener abierta una sesion
			return M4_ERROR;

		return M4_SUCCESS; //??
	}

	m4return_t	CloseSession( void );

	m4return_t	SetCommsLanguage( m4language_t ai_iLang ) 
	{
		return M4_SUCCESS;
	}
// ===========================================

// Genericas para los adaptadores =====================
    m4return_t OpenService (m4pchar_t ai_szServiceId, m4handle_t &ao_hContext);
	
	m4return_t SyncRequest (m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout = 00, m4bool_t ai_bLogInfo = M4_FALSE, ClLogSystem *ai_poLog = NULL, ClTimeoutFunction ai_pTimeoutFunction = NULL);
	m4return_t SyncRequest (m4pchar_t ai_szServiceId, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout = 00, m4bool_t ai_bLogInfo = M4_FALSE, ClLogSystem *ai_poLog = NULL, ClTimeoutFunction ai_pTimeoutFunction = NULL);
	
	m4return_t AsyncRequest(m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo = M4_FALSE);
	m4return_t AsyncRequest(m4pchar_t ai_szServiceId, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo = M4_FALSE);
	
	m4return_t ReceiveRequest (m4handle_t ai_hRequestId, M4DataStorage *&ao_pOutDataStore, m4int32_t ai_lTimeOut, m4int16_t ai_lReceiveFlag, ClLogSystem *ai_poLog = NULL);

	m4return_t GetSessionContext( m4pchar_t &ao_pcX509Certificate, m4uint32_t &ai_riX509CertificateLen, m4pchar_t &ao_pcSessionContext, m4uint32_t &ai_riSessionContextLen ) ;
// ===========================================
	// Gestión de TimeOut =====================
	void NotifyVMExecution(m4bool_t ai_bIsStart) {}
// ===========================================

protected:
	m4return_t CloseServices();

private:

	m4handle_t				m_hSessionId;
	ClM4ObjServiceClientSM	*m_pObjServiceSMClient;
	ClM4ObjService			*m_pObjServiceSMServer;
	m4bool_t				m_bOpenOLTP;
	m4bool_t				m_bOpenProxy;
	m4bool_t				m_bOpenMD;

	ClCache					*m_pCacheCMCR;
	ClCache					*m_pCacheCSCR;
	ClCache					*m_pCachePres;
	ClCache					*m_pCacheMap;
	ClCache					*m_pCacheData;

	m4bool_t m_bInit;
};


#endif //_CLAPPCLIENTADAPDIC_HPP_

