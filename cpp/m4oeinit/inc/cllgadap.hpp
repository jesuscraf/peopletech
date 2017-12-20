
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                cllgadap.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                15-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene el interfaz abstracto del logon adaptor
//
//
//==============================================================================


#include "m4types.hpp"
#include "cldefine.hpp"
#include "m4serial.hpp"
#include "clsubjct.hpp"
#include "m4oeinit_dll.hpp"
#include "m4clock.hpp"
#include "licensemanagerinterface.hpp"


#ifndef __CLLGADAP_HPP__
#define __CLLGADAP_HPP__




enum LogonStateId
{
	M4CL_LOG_LOGGED_OUT,
	M4CL_LOG_ONLINE,
	M4CL_LOG_WITHOUT_ROLE
} ;


//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClAccessRecordSet ;
class	ClAppClientAdap ;
class	ClChannel ;
class	ClAccess ;
class	ClChannel ;
class	ClLogonState ;
class	ClChannelManager ;
class	ClVMBaseEnv ;
class	ClCacheDirectory ;
class	m4VariantType ;
class	IClSesChnExport ;

//=================================================================================
//
// ClExecuteItemI
//
// Clase para el interfaz abstracto para ejecuciones de items
//
//=================================================================================

class ClExecuteItemI {
public:
	virtual m4return_t Execute( m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut, m4pchar_t ai_pcRole, ClAccess **ao_ppoAccess ) = 0;
	virtual ClChannelManager* GetChannelManager() = 0;
	virtual m4return_t GetBinaryItem( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pchar_t &ao_pOutput, m4uint32_t &ao_iOutLen) = 0;
	virtual m4return_t SetBinaryItem( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pcchar_t ai_pInput, m4uint32_t ai_iInLen) = 0;
	virtual void SetAccess (ClAccess *ai_poAccess) = 0;

	// destructor
	virtual ~ClExecuteItemI();
};

m4return_t	M4ExecuteItem( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut, m4pchar_t ai_pcRole, ClAccess **ao_ppoAccess );

class M4_DECL_M4OEINIT ClExecuteItem : public ClExecuteItemI {
public:
	ClExecuteItem (ClChannelManager *ai_poChannelManager);
	virtual ~ClExecuteItem();

	virtual m4return_t Execute( m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut, m4pchar_t ai_pcRole, ClAccess **ao_ppoAccess );
	virtual ClChannelManager* GetChannelManager();
	virtual m4return_t GetBinaryItem( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pchar_t &ao_pOutput, m4uint32_t &ao_iOutLen);
	virtual m4return_t SetBinaryItem( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pcchar_t ai_pInput, m4uint32_t ai_iInLen);
	virtual void SetAccess (ClAccess *ai_poAccess);

protected:
	void Reset ();

private:
	ClChannelManager* m_poChannelManager;
	ClAccess *m_poAccess;
	m4bool_t m_bOwner;
};
//=================================================================================
//
// ClLogonAdaptor
//
// Clase para el interfaz abstracto del logon adaptor
//
//=================================================================================


class	ClLogonAdaptor : public ClSubject
{

protected:

	ClLogonAdaptor()
	{
		m_dSessionTimeout = 0 ;
	    m_uiServerArchitect = M4_ARCHITECTURE ;
		m_iRootNode = 0 ;
		m_Language = M4CL_LANGUAGE_DEFAULT ;
		m_poSessionAccess = NULL ;
		m_poSessionChannel = NULL ;
		m_poCacheDirectory = NULL ;
	}

public:

	virtual ~ClLogonAdaptor() {} ;

	virtual	m4return_t		Logon       ( m4pchar_t ai_pszUserName, m4pchar_t ai_pszPassword,   m4language_t ai_Language, m4pchar_t ai_pszLogonServer, m4uint32_t ai_uiPortNumber, m4bool_t ai_bUseDefaultRole, m4pchar_t ai_pszSSLFile, m4bool_t ai_bUseSSL ) = 0 ;
	virtual	m4return_t		RestoreLogon(                           m4pchar_t ai_pszCredential, m4language_t ai_Language, m4pchar_t ai_pszLogonServer, m4uint32_t ai_uiPortNumber,                              m4pchar_t ai_pszSSLFile, m4bool_t ai_bUseSSL ) = 0 ;
	virtual	m4return_t		LogOff( void ) = 0 ;
	virtual	m4return_t		SetRole( m4int32_t ai_iPos, m4bool_t ai_bSetAsDefaultRole = M4_FALSE ) = 0 ;
	virtual	m4return_t		ChangePassword( m4pchar_t ai_pszOldPassword, m4pchar_t ai_pszNewPassword ) = 0 ;
	virtual	m4uint8_t		CheckRoleAndOrg( m4pchar_t ai_pcRole, m4pchar_t ai_pcOrg, m4pchar_t *ao_ppcRSM, m4uint16_t ai_iLength, m4bool_t ai_bIsLevel2 ) = 0 ;
	virtual	m4return_t		Confirm( void ) = 0 ;

	virtual	ClChannel*		GetSessionChannelPointer( void )
	{
		return( m_poSessionChannel ) ;
	}

	virtual	m4return_t			BuildSessionChannel( void ) = 0 ;
	
	virtual	ClChannel*			GetSessionChannel( void ) = 0 ;
	virtual	ClAccessRecordSet*	GetSessionRecordSet( void ) = 0 ;
	virtual	m4return_t			SetSessionChannel( ClChannel* ai_poChannel ) = 0 ;

	virtual	m4uint64_t		GetTimeStamp( void ) = 0 ;

	virtual	m4pchar_t		GetUsername( void ) = 0 ;
	virtual	m4pchar_t		GetDebugUser( void ) = 0 ;
	virtual	m4return_t		GetLanguage( m4language_t& ai_Lingo ) = 0 ;
	virtual	m4return_t		GetDevLanguage( m4language_t& ai_Lingo ) = 0 ;
	virtual	m4pchar_t		GetRSM(void) = 0 ;
	virtual	m4pchar_t		GetRole(void) = 0 ;
	virtual	m4pchar_t		GetOrganization( void ) = 0 ;
	virtual	m4pchar_t		GetLicense( void ) = 0 ;
	virtual	m4uint8_t		GetIsMultiOrganization( void ) = 0 ;
	virtual	m4uint8_t		GetRoundCurrency( void ) = 0 ;
	virtual	m4uint8_t		GetRoundNumber( void ) = 0 ;
	virtual	m4uint8_t		GetExecuteRealSQLMode( void ) = 0 ;

	virtual	m4date_t		GetStartDate( void ) = 0 ;
	virtual	m4date_t		GetEndDate( void ) = 0 ;
	virtual	m4date_t		GetCorrDate( void ) = 0 ;
	virtual	m4date_t		GetVMStartDate( void ) = 0 ;
	virtual	m4date_t		GetVMEndDate( void ) = 0 ;
	virtual	m4date_t		GetVMCorrDate( void ) = 0 ;
	virtual	m4date_t		GetLDBStartDate( void ) = 0 ;
	virtual	m4date_t		GetLDBEndDate( void ) = 0 ;
	virtual	m4date_t		GetLDBCorrDate( void ) = 0 ;	

	virtual	m4int32_t		GetNumberOfRoles( void ) = 0 ;
	virtual	m4pchar_t		GetRoleNameByPos( m4int32_t ai_iPos ) = 0 ;
	virtual	m4pchar_t		GetIdRoleByPos( m4int32_t ai_iPos ) = 0 ;

	virtual	LogonStateId	GetCurrentState( void ) = 0 ;
    virtual	m4int16_t		GetServerArchitecture( void ) = 0 ;

	virtual	m4pchar_t		GetPublicCookie( void ) = 0 ;
	virtual	m4pchar_t		GetCredential( void ) = 0 ;

	virtual	m4return_t		InsertUserData( m4pchar_t ai_pszUser, m4language_t ai_language, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pszClientMachine, m4pchar_t ai_pcUserAlias, m4pchar_t ai_pcImpersonatorUser, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP ) = 0 ;

	virtual	m4return_t		SetConnectionParam( m4pcchar_t ai_pzParamName, m4pcchar_t ai_pzParamValue ) = 0 ;

	virtual m4language_t	GetSessionLanguage   ( m4pcchar_t ai_pccThisT3Id, m4language_t ai_Language = M4CL_LANGUAGE_NONE ) = 0 ;
	virtual m4language_t	GetSessionDevLanguage( m4pcchar_t ai_pccThisT3Id, m4language_t ai_Language = M4CL_LANGUAGE_NONE ) = 0 ;

protected:

	virtual	m4uint8_t			_GetSmOltp( void ) const = 0 ;
	virtual eStateValidateExecutionNp_t	_GetStateValidateExecutionNp( void ) = 0;
	virtual	void				_SetStateValidateExecutionNp( eStateValidateExecutionNp_t ai_eStateValidateExecutionNp ) = 0;
	virtual	ClAppClientAdap*	_GetAPIClientAdaptor( void ) = 0 ;
	virtual	ClChannelManager*	_GetChannelManager( void ) = 0 ;
	virtual	ClVMBaseEnv*		_GetEnvironment( void ) = 0 ;

	virtual	ClAccess*			_GetSessionAccess( void ) = 0 ;


	virtual	void				_ChangeState( ClLogonState* ai_LogonState, LogonStateId ai_LogonStateId ) = 0 ;
	virtual	void				_InitIndex( void ) = 0;

	virtual	void				_SetPublicCookie( m4pchar_t ai_pzCookie ) = 0 ;
	virtual	m4return_t			_GetConnectionParam( m4pcchar_t ai_pzParamName, m4pchar_t  ao_pzParamValue ) = 0 ;

	virtual	m4return_t			_ExecuteItem( m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut, m4pcchar_t ai_pcInstance, m4pchar_t ai_pcRole ) = 0 ;
	virtual	m4return_t			_Authenticate( m4pchar_t ai_pcUserId, m4pchar_t ai_pcPassword, m4bool_t ai_bUseDefaultRole, m4int8_t &ao_iLogonMechanism, m4pchar_t ao_pcRealUserId, m4pchar_t ao_pcImpersonatedUser ) = 0 ;
	virtual	m4return_t			_EnableRole( m4pchar_t ai_pcIdRole, m4bool_t ai_bSetAsDefaultRole ) = 0 ;
	virtual	m4return_t			_ValidatePassword( m4pchar_t ai_pcPassword, m4pchar_t ai_pcRole ) = 0 ;
	virtual	m4return_t			_ValidateServerPassword( m4pchar_t ai_pcOldPassword, m4pchar_t ai_pcNewPassword, m4pchar_t ai_pcNewPasswordTwoWay, m4pchar_t ai_pcRole ) = 0 ;
	virtual	m4return_t			_RefreshSessionChannel( m4bool_t ai_bUseDefaultRole ) = 0 ;
	virtual	m4return_t			_InitClientLicense( void ) = 0 ;
	virtual	m4return_t			_SetDefaultRoleAsActive( void ) = 0 ;
	virtual	m4return_t			_SetRoleAsActive( m4int32_t ai_iPos ) = 0 ;

// Amigos =====================================================================

	friend class ClLogonState ;
	friend class ClLogLoggedOut ;
	friend class ClLogWithoutRole ;
	friend class ClLogOnline ;
	friend class ClChannelManager ;
	friend class Common_Test ;


// Miembros ===================================================================

    m4double_t			m_dSessionTimeout ;
    m4int16_t			m_uiServerArchitect ;
	m4uint32_t			m_iRootNode ;
	m4language_t		m_Language ;
	ClAccess			*m_poSessionAccess ;
	ClChannel			*m_poSessionChannel ;
	ClCacheDirectory	*m_poCacheDirectory ;

// new validation functions
public:
	virtual	m4uint8_t		CheckProjectTreeValidation( m4pchar_t ai_pcChannelId, m4pchar_t ai_pcOrg ) = 0 ;
	virtual ClExecuteItemI * GetExecuteItem() = 0;
	virtual IClSesChnExport * GetSessionExport() = 0;

    m4double_t GetSessionTimeout( void )
	{
	    return( m_dSessionTimeout ) ;
	}

	// User actions auditory.
	virtual	m4int32_t	GetUserActionsAuditStatus( void ) = 0 ;
	virtual	m4date_t	GetUserActionsAuditSessionDate( void ) = 0 ;
	virtual	m4int32_t	GetUserTimeRCThreshold( void ) = 0 ;
	virtual	m4int32_t	GetUserAuditClientType( void ) = 0 ;

	virtual m4pchar_t	GetSrvCOMAppsLocked( void ) = 0;

} ;


#endif


