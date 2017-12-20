//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clappclientadap.hpp   
// Project:             Mind3x      
// Author:              Meta Software M.S. , S.A
// Date:                19/02/1997
// Language:            C++
//
// Definition:
//    This module declares the interface to the AppClient Adaptor
//
//==============================================================================

// Esta interfaz es común para Client (DIC y SM)

#ifndef _CLAPPCLIENTADAP_HPP_
#define _CLAPPCLIENTADAP_HPP_

#include "m4types.hpp"
#include "m4string.hpp"
#include "clienttag.hpp" //ClTagList
#include "logsys.hpp" //ClLogSystem
#include "clientapi.hpp" //ClTimeoutFunction

class ClAppClientAdap {

protected:
	ClAppClientAdap(){}

public:
	virtual ~ClAppClientAdap() {}
	virtual m4return_t Init(m4int32_t ai_uiTimeout) = 0;
	virtual m4return_t End () = 0;

// Para la ventana del time-out =====================
#ifdef _WINDOWS
	virtual void SetTimeoutFunction(ClTimeoutFunction ai_hCSTimeoutProcedure) = 0;
	virtual void SetServerEventFunction(ClServerEventFunction ai_hCSServerEventProcedure) = 0;
#endif //_WINDOWS
// ===========================================

// Para el logon adaptor =====================
	virtual m4return_t	OpenSession( M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strUsername, M4ClString &ai_strPasswd, m4bool_t ai_bDefaultRole, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert ) = 0;

	virtual m4return_t	OpenSession( M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strCredential,													  ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert ) = 0;

	virtual m4return_t	EnableRole( m4pchar_t ai_pRoleId  = NULL, m4bool_t ai_bDefault = M4_FALSE) = 0;

	virtual m4return_t	ChangePassword( M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd ) = 0;

	virtual m4return_t	CloseSession( void ) = 0;

	virtual m4return_t	SetCommsLanguage( m4language_t ai_iLang ) = 0;
// ===========================================

// Genericas para los adaptadores =====================
    virtual m4return_t OpenService (m4pchar_t ai_szServiceId, m4handle_t &ao_hContext) = 0;

	virtual m4return_t SyncRequest (m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout = 00, m4bool_t ai_bLogInfo = M4_FALSE, ClLogSystem *ai_poLog = NULL, ClTimeoutFunction ai_pTimeoutFunction = NULL) = 0;
	virtual m4return_t SyncRequest (m4pchar_t ai_szServiceId, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout = 00, m4bool_t ai_bLogInfo = M4_FALSE, ClLogSystem *ai_poLog = NULL, ClTimeoutFunction ai_pTimeoutFunction = NULL) = 0;

	virtual m4return_t AsyncRequest(m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo = M4_FALSE) = 0;
	virtual m4return_t AsyncRequest(m4pchar_t ai_szServiceId, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo = M4_FALSE) = 0;

	virtual m4return_t ReceiveRequest (m4handle_t ai_hRequestId, M4DataStorage *&ao_pOutDataStore, m4int32_t ai_lTimeOut, m4int16_t ai_lReceiveFlag, ClLogSystem *ai_poLog = NULL) = 0;

	virtual m4return_t GetSessionContext( m4pchar_t &ao_pcX509Certificate, m4uint32_t &ai_riX509CertificateLen, m4pchar_t &ao_pcSessionContext, m4uint32_t &ai_riSessionContextLen ) = 0 ;
// ===========================================
// Gestión de TimeOut =====================
	virtual void NotifyVMExecution(m4bool_t ai_bIsStart) = 0;
// ===========================================

protected:
	virtual m4return_t CloseServices()  = 0;
};


#endif //_CLAPPCLIENTADAP_HPP_

