//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clappclientadapdic.hpp   
// Project:             Mind3x      
// Author:              Meta Software M.S. , S.A
// Date:                19/02/1997
// Language:            C++
//
// Definition:
//    This module declares the interface to the AppClient Adaptor
//
//==============================================================================

// Esta definición es específica para DIC

#ifndef _CLAPPCLIENTADAPDIC_HPP_
#define _CLAPPCLIENTADAPDIC_HPP_


#include "clappclientadap.hpp"
#include "clientapi.hpp"

#ifdef _WINDOWS
#include "csexternal.hpp"
#endif //_WINDOWS

#include "m4stl.hpp"

typedef map <string,m4handle_t, less<string> > ClientServiceMap;
typedef ClientServiceMap::iterator ClientServiceMapIt;


class ClAppClientAdapDIC : public ClAppClientAdap{

public:
	ClAppClientAdapDIC();
	ClAppClientAdapDIC(m4int_t ai_iCommsType, m4int32_t ai_iControlPort, m4int32_t ai_nCommWorkers);
	~ClAppClientAdapDIC();
	m4return_t Init(m4int32_t ai_uiTimeout)
	{
		if (m_pClClientAPI != NULL) {
			return m_pClClientAPI->SetControlTimeout(ai_uiTimeout);
		}
		return M4_ERROR;
	}
	m4return_t End (){ return M4_SUCCESS;}

// Para la ventana del time-out =====================
#ifdef _WINDOWS
	void SetTimeoutFunction(ClTimeoutFunction ai_hCSTimeoutProcedure) {
		m_pClClientAPI->SetTimeoutFunction(ai_hCSTimeoutProcedure);
	}

	void SetServerEventFunction(ClServerEventFunction ai_hCSServerEventProcedure) {
		m_pClClientAPI->SetServerEventFunction(ai_hCSServerEventProcedure);
	}
#endif //_WINDOWS
// ===========================================




// Para el logon adaptor =====================
	m4return_t	OpenSession( M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strUsername, M4ClString &ai_strPasswd, m4bool_t ai_bDefaultRole, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert )
	{
		m4return_t	iResult = M4_ERROR ;

		if (0 == m_hSessionId)
		{
			//Solo abrimos una sesion
			iResult = m_pClClientAPI->OpenSession( ai_strAddress, ai_uiPort, ai_strUsername, ai_strPasswd, ai_bDefaultRole, m_hSessionId, ao_oArgs, ai_bUseSSL, ai_strCACert ) ;

			if( iResult < 0 )
			{
				// Hay que resetear la sesión colega!!!
				m_hSessionId = 0 ;
			}
		}

		return( iResult );
	}

	m4return_t	OpenSession( M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strCredential,													  ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert )
	{
		m4return_t	iResult = M4_ERROR ;

		if (0 == m_hSessionId)
		{
			//Solo abrimos una sesion
			iResult = m_pClClientAPI->OpenSession( ai_strAddress, ai_uiPort, ai_strCredential, m_hSessionId, ao_oArgs, ai_bUseSSL, ai_strCACert ) ;

			if( iResult < 0 )
			{
				// Hay que resetear la sesión colega!!!
				m_hSessionId = 0 ;
			}
		}

		return( iResult );
	}


	m4return_t	EnableRole( m4pchar_t ai_pRoleId = NULL, m4bool_t ai_bDefault = M4_FALSE)
	{
		if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
			return M4_ERROR;
		return( m_pClClientAPI->EnableRole( m_hSessionId, ai_pRoleId, ai_bDefault ) ) ;
	}

	m4return_t	ChangePassword( M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd )
	{
		if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
			return M4_ERROR;
		return( m_pClClientAPI->ChangePassword( m_hSessionId, ai_oOldPasswd, ai_oNewPasswd ) ) ;
	}

	m4return_t	CloseSession( void );

	m4return_t	SetCommsLanguage( m4language_t ai_iLang ) ;
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
	void NotifyVMExecution(m4bool_t ai_bIsStart);
// ===========================================

protected:
	m4return_t CloseServices();
private:

	m4handle_t		m_hSessionId ;
	ClClientAPI		*m_pClClientAPI;

	ClientServiceMap m_oClientServiceMap;
};


#endif //_CLAPPCLIENTADAPDIC_HPP_

