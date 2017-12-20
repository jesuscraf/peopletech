
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4lgadapi.hpp   
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
//    Este módulo contiene la declaración de la implementación logon adaptor
//
//
//==============================================================================


#include "m4oeinit_dll.hpp"

#include "cllgadap.hpp"
#include "m4lgadapdef.hpp"

#include "m4var.hpp"
#include "cldates.hpp"
#include "m4stl.hpp"

#include "licensemanagerinterface.hpp"


#include "usractaudit.hpp" // User actions auditory.


#ifndef __M4LGADAPI_HPP__
#define __M4LGADAPI_HPP__



typedef map<string, string, less<string> > CONNECTION_PROPERTIES_MAP ;
typedef CONNECTION_PROPERTIES_MAP::iterator CONNECTION_PROPERTIES_MAP_IT ;


typedef	enum	eM4CsTypes_tag
{
	eM4CsTypeClient = 0,
	eM4CsTypeServer,
	eM4CsTypeClientEmul,
	eM4CsTypeServerEmul

} eM4CsTypes_t ;

#define M4_MAX_USER_ALIAS	256
#define M4_MAX_USER_ID		30
#define M4_MAX_CLIENT_IP	64

//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClChannelManager ;
class	ClCsTrace ;


//=================================================================================
//
// ClLogonState
//
// Clase para el estado del logon adaptor
// Debajo están sus hijas
//
//=================================================================================



class	M4_DECL_M4OEINIT	ClLogonState
{
public:
	static ClLogonState* Instance( void );

	virtual m4return_t Logon
		(
		ClLogonAdaptor*		ai_LogonAdaptor,
		m4pchar_t			ai_pszUserName, 
		m4pchar_t			ai_pszPassword, 
		m4bool_t			ai_bUseCredential,
		m4language_t		ai_Language, 
		m4pchar_t			ai_pszLogonServer,
		m4uint32_t			ai_uiPortNumber,
		m4bool_t			ai_bUseDefaultRole,
		m4pchar_t			ai_pszSSLFile,
		m4bool_t			ai_bUseSSL
		)
	{ 
		return M4_ERROR; 
	}

	virtual m4return_t ChangePassword
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t		ai_pszOldPassword,
		m4pchar_t		ai_pszNewPassword
		)
	{
		return M4_ERROR; 
	}

	virtual m4uint8_t	CheckRoleAndOrg
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t		ai_pcRole,
		m4pchar_t		ai_pcOrg,
		m4pchar_t		*ao_ppcRSM,
		m4uint16_t		ai_iLength,
		m4bool_t		ai_bIsLevel2
		)
	{
		m4uint16_t	iLength ;

		if( ao_ppcRSM != NULL )
		{
			iLength = strlen( ai_pcRole ) ;

			if( iLength > ai_iLength )
			{
				iLength = ai_iLength ;
			}

			memcpy( *ao_ppcRSM, ai_pcRole, iLength ) ;
			( *ao_ppcRSM )[ iLength ] = '\0' ;
		}

		return( 1 ) ;
	}

	virtual	m4return_t	Confirm
		(
		ClLogonAdaptor* ai_LogonAdaptor
		)
	{
		return M4_ERROR; 
	}

	virtual m4uint8_t	CheckProjectTreeValidation
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t		ai_pcChannelId,
		m4pchar_t		ai_pcOrg
		)
	{
		return( 1 ) ;
	}

	virtual m4return_t LogOff
		(
		ClLogonAdaptor* ai_LogonAdaptor
		)
	{ 
		return M4_ERROR; 
	}

	virtual m4return_t SetRole
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t ai_iPos,
		m4bool_t ai_bSetAsDefaultRole = M4_FALSE
		)
	{
		return M4_ERROR;
	}

	virtual m4date_t GetDate 
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t ai_DateType
		)
	{
		return 0;
	}

	virtual m4pchar_t GetString
		(
		ClLogonAdaptor  *ai_LogonAdaptor,
		m4int32_t		ai_ItemDef
		);
	
	virtual m4int32_t GetInt
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t		ai_ItemDef,
		m4int32_t		ai_iDefault
		)
	{
		return ai_iDefault;
	}

	virtual m4int32_t GetNumRole
		(
		ClLogonAdaptor* ai_LogonAdaptor
		)
	{
		return -1;
	}

	virtual m4return_t GetRoleNameByPos
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t		ai_iPos,
		m4pcchar_t		*ao_pszRole
		)

	{
		return M4_ERROR;
	}

	virtual m4return_t GetIdRoleByPos
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t		ai_iPos,
		m4pcchar_t		*ao_pszIdRole
		)
	{
		return -1;
	}


protected:
	friend class ClLogLoggedOut;
	friend class ClLogWithoutRole;
	friend class ClLogOnline;

	m4return_t _LogOff
		(
		ClLogonAdaptor* ai_LogonAdaptor
		);

	m4return_t _SetRole
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t ai_iPos,
		m4bool_t ai_bSetAsDefaultRole
		) ;

	m4return_t _ChangePassword
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t ai_pszOldPassword,
		m4pchar_t ai_pszNewPassword
		) ;

	m4uint8_t _CheckRoleAndOrg
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t		ai_pcRole,
		m4pchar_t		ai_pcOrg,
		m4pchar_t		*ao_ppcRSM,
		m4uint16_t		ai_iLength,
		m4bool_t		ai_bIsLevel2
		) ;

	m4return_t _Confirm
		(
		ClLogonAdaptor* ai_LogonAdaptor
		);

	m4uint8_t _CheckProjectTreeValidation
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t		ai_pcChannelId,
		m4pchar_t		ai_pcOrg
		) ;
};




class	M4_DECL_M4OEINIT	ClLogLoggedOut : public ClLogonState
{
public:
	static ClLogLoggedOut* Instance( void );

	virtual m4return_t Logon
		(
		ClLogonAdaptor*		ai_LogonAdaptor,
		m4pchar_t			ai_pszUserName, 
		m4pchar_t			ai_pszPassword, 
		m4bool_t			ai_bUseCredential,
		m4language_t		ai_Language, 
		m4pchar_t			ai_pszLogonServer,
		m4uint32_t			ai_uiPortNumber,
		m4bool_t			ai_bUseDefaultRole,
		m4pchar_t			ai_pszSSLFile,
		m4bool_t			ai_bUseSSL
		);
private:
	static ClLogLoggedOut* _instance;
};




class	M4_DECL_M4OEINIT	ClLogWithoutRole : public ClLogonState
{
public:
	static ClLogWithoutRole* Instance( void );

	virtual m4return_t LogOff
		(
		ClLogonAdaptor* ai_LogonAdaptor
		)
	{
		return _LogOff(ai_LogonAdaptor);
	}

	virtual m4return_t SetRole
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t ai_iPos,
		m4bool_t ai_bSetAsDefaultRole = M4_FALSE
		);

	virtual m4int32_t GetNumRole
		(
		ClLogonAdaptor* ai_LogonAdaptor
		);

	virtual m4return_t GetRoleNameByPos
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t		ai_iPos,
		m4pcchar_t		*ao_pszRole
		);

	virtual m4return_t GetIdRoleByPos
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t		ai_iPos,
		m4pcchar_t		*ao_pszIdRole
		);

	virtual m4return_t ChangePassword
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t		ai_pszOldPassword,
		m4pchar_t		ai_pszNewPassword
		)
	{
		return _ChangePassword( ai_LogonAdaptor, ai_pszOldPassword, ai_pszNewPassword ) ;
	}

private:
	static ClLogWithoutRole* _instance;
};



class	M4_DECL_M4OEINIT	ClLogOnline : public ClLogWithoutRole
{
public:
	static ClLogOnline* Instance( void );

	virtual m4return_t LogOff
		(
		ClLogonAdaptor* ai_LogonAdaptor
		)
	{
		return _LogOff(ai_LogonAdaptor);
	}


	virtual m4date_t GetDate 
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t ai_DateType
		);
	
	virtual m4int32_t GetInt
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t		ai_ItemDef,
		m4int32_t		ai_iDefault
		);

	// We must undefine SetRole ( which is only defined in LogWithoutRole )

	// What do we do with the functions GetNumRole and GetRoleNameByPos ???
	virtual m4return_t SetRole
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4int32_t ai_iPos,
		m4bool_t ai_bSetAsDefaultRole = M4_FALSE
		)
	{
		// Bug 0135463 Se permite hacer SetRole por segunda vez
		return _SetRole( ai_LogonAdaptor, ai_iPos, ai_bSetAsDefaultRole ) ;
	}

	virtual m4return_t ChangePassword
		(
		ClLogonAdaptor*	ai_LogonAdaptor,
		m4pchar_t		ai_pszOldPassword,
		m4pchar_t		ai_pszNewPassword
		)
	{
		return _ChangePassword( ai_LogonAdaptor, ai_pszOldPassword, ai_pszNewPassword ) ;
	}

	virtual m4uint8_t	CheckRoleAndOrg
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t		ai_pcRole,
		m4pchar_t		ai_pcOrg,
		m4pchar_t		*ao_ppcRSM,
		m4uint16_t		ai_iLength,
		m4bool_t		ai_bIsLevel2
		)
	{
		return( _CheckRoleAndOrg( ai_LogonAdaptor, ai_pcRole, ai_pcOrg, ao_ppcRSM, ai_iLength, ai_bIsLevel2 ) ) ;
	}

	virtual	m4return_t	Confirm
		(
		ClLogonAdaptor* ai_LogonAdaptor
		)
	{
		return( _Confirm( ai_LogonAdaptor ) ) ;
	}

	virtual m4uint8_t	CheckProjectTreeValidation
		(
		ClLogonAdaptor* ai_LogonAdaptor,
		m4pchar_t		ai_pcChannelId,
		m4pchar_t		ai_pcOrg
		)
	{
		return( _CheckProjectTreeValidation( ai_LogonAdaptor, ai_pcChannelId, ai_pcOrg) ) ;
	}

private:
	static ClLogOnline* _instance;
};







//=================================================================================
//
// ClLogonAdaptorImp
//
// Clase que implementa el logon adaptor
//
//=================================================================================



class	M4_DECL_M4OEINIT	ClLogonAdaptorImp : public ClLogonAdaptor
{

public:

		ClLogonAdaptorImp() ;
		~ClLogonAdaptorImp() ;

	m4return_t	 Init	(
												ClChannelManager	*ai_poChannelManager,
												ClVMBaseEnv			*ai_poEnvironment,
												ClCsTrace			*ai_poCsTrace,
												ClAppClientAdap		*ai_poAPIClientAdaptor,
												ClCacheDirectory	*ai_poCacheDirectory,
												eM4CsTypes_t		ai_eCsType
											) ;

	m4return_t	 End( void ) ;


// Funciones mapeadas al Logon State ==========================================

	m4return_t	Logon       ( m4pchar_t ai_pszUserName, m4pchar_t ai_pszPassword,   m4language_t ai_Language, m4pchar_t ai_pszLogonServer, m4uint32_t ai_uiPortNumber, m4bool_t ai_bUseDefaultRole, m4pchar_t ai_pszSSLFile, m4bool_t ai_bUseSSL ) ;
	m4return_t	RestoreLogon(                           m4pchar_t ai_pszCredential, m4language_t ai_Language, m4pchar_t ai_pszLogonServer, m4uint32_t ai_uiPortNumber,                              m4pchar_t ai_pszSSLFile, m4bool_t ai_bUseSSL ) ;
	m4return_t	LogOff( void ) ;
	m4return_t	SetRole( m4int32_t ai_iPos, m4bool_t ai_bSetAsDefaultRole ) ;
	m4return_t	ChangePassword( m4pchar_t ai_pszOldPassword, m4pchar_t ai_pszNewPassword ) ;
	m4uint8_t	CheckRoleAndOrg( m4pchar_t ai_pcRole, m4pchar_t ai_pcOrg, m4pchar_t *ao_ppcRSM, m4uint16_t ai_iLength, m4bool_t ai_bIsLevel2 ) ;
	m4return_t	Confirm( void ) ;


// Funciones del Canal sesion =================================================

	m4return_t			BuildSessionChannel( void ) ;

	ClChannel*			GetSessionChannel( void ) ;
	ClAccessRecordSet*	GetSessionRecordSet( void ) ;

	m4return_t			SetSessionChannel( ClChannel* ai_poChannel ) ;

	m4uint64_t	GetTimeStamp( void ) ;

// Acceso a Items del Canal sesion ============================================

	m4pchar_t	GetUsername( void )
	{
		return( m_poLogonState->GetString( this, m_iUser_Name_Index ) ) ;
	}

	m4pchar_t	GetDebugUser( void )
	{
		return( m_poLogonState->GetString( this, m_iID_Debug_User_Index ) ) ;
	}

	m4return_t	GetLanguage( m4language_t& ai_Lingo )
	{
		ai_Lingo = 0 ;

		if( m_iLanguage_Index != -1 )
		{
			/* Bug 0251097
			En etapas tempranas de logon también hay que usar el lenguaje de logado
			*/
			ai_Lingo = m_poLogonState->GetInt( this, m_iLanguage_Index, m_Language ) ;
		}

		if( ai_Lingo == 0 )
		{
			/* Bug 0075632
			Si no viene en el canal sesión, que no debería pasar ponemos inglés
			para que cuadre el cliente y el server
			*/
			ai_Lingo = M4CL_LANGUAGE_DEFAULT ;
		}

		return( M4_SUCCESS ) ;
	}

	m4return_t	GetDevLanguage( m4language_t& ai_Lingo )
	{
		ai_Lingo = 0 ;

		if( m_iDevLanguage_Index != -1 )
		{
			ai_Lingo = m_poLogonState->GetInt( this, m_iDevLanguage_Index, 0 ) ;
		}

		if( ai_Lingo == 0 )
		{
			/* Bug 0075632
			Si no viene en el canal sesión es que estamos en CS y ponemos inglés
			*/
			ai_Lingo = M4CL_DEV_LANGUAGE_DEFAULT ;
		}

		return( M4_SUCCESS ) ;
	}

	m4pchar_t	GetRSM( void )
	{
		return( m_poLogonState->GetString( this, m_iRSM_Index ) ) ;
	}

	m4pchar_t	GetRole( void )
	{
		return( m_poLogonState->GetString( this, m_iRole_Index ) ) ;
	}

	m4pchar_t GetOrganization( void )
	{
		return m_poLogonState->GetString( this, m_iOrganization_Index ) ;
	}

	m4pchar_t GetLicense( void )
	{
		return m_poLogonState->GetString( this, m_iLicense_Index ) ;
	}

	m4uint8_t GetIsMultiOrganization( void )
	{
		/* Bug 0174240
		Es un campo cadena con valor "0" o "1"
		*/
		m4pchar_t pcIsMultiOrganization = m_poLogonState->GetString( this, m_iIs_MultiOrg_Index ) ;

		if( pcIsMultiOrganization == NULL || strcmp( pcIsMultiOrganization, "1" ) == 0 )
		{
			return( 1 ) ;
		}

		return( 0 ) ;
	}

	m4uint8_t GetRoundCurrency( void )
	{
		return( m4uint8_t( m_poLogonState->GetInt( this, m_iRoundCurrency_Index, 0 ) ) ) ;
	}

	m4uint8_t GetRoundNumber( void )
	{
		return( m4uint8_t( m_poLogonState->GetInt( this, m_iRoundNumber_Index, 0 ) ) ) ;
	}

	m4uint8_t GetExecuteRealSQLMode( void )
	{
		return( m4uint8_t( m_poLogonState->GetInt( this, m_iExecuteRealSQLMode_Index, 0 ) ) ) ;
	}

	m4date_t	GetStartDate( void )
	{
		m4date_t	dDate ;
		return( ( ( ( dDate = m_poLogonState->GetDate( this, m_iDate_Start_Index ) ) == 0 ) ? M4CL_MINUS_INFINITE_IN_JULIAN : dDate ) ) ;
	}

	m4date_t	GetEndDate( void )
	{
		m4date_t	dDate ;
		return( ( ( ( dDate = m_poLogonState->GetDate( this, m_iDate_End_Index ) ) == 0 ) ? M4CL_PLUS_INFINITE_IN_JULIAN : dDate ) ) ;
	}

	m4date_t GetCorrDate( void )
	{
		m4date_t	dDate ;
		return( ( ( ( dDate = m_poLogonState->GetDate( this, m_iDate_Corr_Index ) ) == 0 ) ? ClActualDate() : dDate ) ) ;
	}

	m4date_t GetVMStartDate( void )
	{
		m4date_t	dDate ;
		return(  ( ( ( dDate = m_poLogonState->GetDate( this, m_iVMDate_Start_Index ) ) == 0 ) ? M4CL_MINUS_INFINITE_IN_JULIAN : dDate ) ) ;
	}

	m4date_t GetVMEndDate( void )
	{
		m4date_t	dDate ;
		return( ( ( ( dDate = m_poLogonState->GetDate( this, m_iVMDate_End_Index ) ) == 0 ) ? M4CL_PLUS_INFINITE_IN_JULIAN : dDate ) ) ;
	}

	m4date_t GetVMCorrDate( void )
	{
		m4date_t	dDate ;
		return( ( ( ( dDate = m_poLogonState->GetDate( this, m_iVMDate_Corr_Index ) ) == 0 ) ? ClActualDate() : dDate ) ) ;
	}

	m4date_t GetLDBStartDate( void )
	{
		m4date_t	dDate ;
		return( ( ( ( dDate = m_poLogonState->GetDate( this, m_iLDB_Date_Start_Index ) ) == 0 ) ? M4CL_MINUS_INFINITE_IN_JULIAN : dDate ) ) ;
	}

	m4date_t GetLDBEndDate( void )
	{
		m4date_t	dDate ;
		return( ( ( ( dDate = m_poLogonState->GetDate( this, m_iLDB_Date_End_Index ) ) == 0 ) ? M4CL_PLUS_INFINITE_IN_JULIAN : dDate ) ) ;
	}

	m4date_t GetLDBCorrDate( void )
	{
		m4date_t	dDate ;
		return( ( ( ( dDate = m_poLogonState->GetDate( this, m_iLDB_Date_Corr_Index ) ) == 0 ) ? ClActualDate() : dDate ) ) ;
	}

// Roles ======================================================================

	m4int32_t	GetNumberOfRoles( void )
	{
		return( m_poLogonState->GetNumRole( this ) ) ;
	}

	m4pchar_t	GetRoleNameByPos( m4int32_t ai_iPos ) ;
	m4pchar_t	GetIdRoleByPos( m4int32_t ai_iPos ) ;

	
// Varias =====================================================================

	LogonStateId	GetCurrentState( void )
	{
		return( m_poCurrentState ) ;
	}

    m4int16_t GetServerArchitecture( void )
	{
	    return( m_uiServerArchitect ) ;
	}

	m4pchar_t GetPublicCookie( void )
	{
	    return m_pzPublicCookie;
	}

	m4pchar_t	GetCredential( void )
	{
		return( m_acCredential ) ;
	}

	m4return_t	InsertUserData( m4pchar_t ai_pszUser, m4language_t ai_language, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pszClientMachine, m4pchar_t ai_pcUserAlias, m4pchar_t ai_pcImpersonatorUser, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP ) ;

	m4return_t	SetConnectionParam( m4pcchar_t ai_pzParamName, m4pcchar_t ai_pzParamValue ) ;

	m4language_t	GetSessionLanguage   ( m4pcchar_t ai_pccThisT3Id, m4language_t ai_Language = M4CL_LANGUAGE_NONE ) ;
	m4language_t	GetSessionDevLanguage( m4pcchar_t ai_pccThisT3Id, m4language_t ai_Language = M4CL_LANGUAGE_NONE ) ;


protected:

// Funciones ==================================================================

	m4uint8_t _GetSmOltp( void ) const
	{
		return( m_iSmOltp ) ;
	}

	eStateValidateExecutionNp_t	_GetStateValidateExecutionNp( void )
	{
		return m_eStateValidateExecutionNp;
	}

	void	_SetStateValidateExecutionNp( eStateValidateExecutionNp_t ai_eStateValidateExecutionNp )
	{
		m_eStateValidateExecutionNp = ai_eStateValidateExecutionNp;
	}

	ClAppClientAdap*	_GetAPIClientAdaptor( void )
	{
		return( m_poAPIClientAdaptor ) ;
	}

	ClChannelManager*	_GetChannelManager( void )
	{
		return( m_poChannelManager ) ;
	}

	ClVMBaseEnv*	_GetEnvironment( void )
	{
		return( m_poEnvironment ) ;
	}

    void	_SetPublicCookie( m4pchar_t ai_pzCookie )
	{
	    strcpy( m_pzPublicCookie, ai_pzCookie ) ;
	}

	m4return_t	_GetConnectionParam( m4pcchar_t ai_pzParamName, m4pchar_t  ao_pzParamValue ) ;

	m4return_t	_UpdateRSM( m4pchar_t ai_pccRSM ) ;
	m4return_t	_ExecuteItem( m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut, m4pcchar_t ai_pcInstance, m4pchar_t ai_pcRole ) ;
	m4return_t	_Logon( m4pchar_t ai_pszUserName, m4pchar_t ai_pszPassword, m4bool_t ai_bUseCredential, m4language_t ai_Language, m4pchar_t ai_pszLogonServer, m4uint32_t ai_uiPortNumber, m4bool_t ai_bUseDefaultRole, m4pchar_t ai_pszSSLFile, m4bool_t ai_bUseSSL ) ;
	m4return_t	_Authenticate( m4pchar_t ai_pcUserId, m4pchar_t ai_pcPassword, m4bool_t ai_bUseDefaultRole, m4int8_t &ao_iLogonMechanism, m4pchar_t ao_pcRealUserId, m4pchar_t ao_pcImpersonatedUser );
	m4return_t	_EnableRole( m4pchar_t ai_pcIdRole, m4bool_t ai_bSetAsDefaultRole ) ;
	m4return_t	_ValidatePassword( m4pchar_t ai_pcPassword, m4pchar_t ai_pcRole ) ;
	m4return_t	_ValidateServerPassword( m4pchar_t ai_pcOldPassword, m4pchar_t ai_pcNewPassword, m4pchar_t ai_pcNewPasswordTwoWay, m4pchar_t ai_pcRole ) ;
	m4return_t	_RefreshSessionChannel( m4bool_t ai_bUseDefaultRole ) ;
	m4return_t	_InitClientLicense( void );
	m4return_t	_SetDefaultRoleAsActive( void );
	m4return_t	_SetRoleAsActive( m4int32_t ai_iPos ) ;

	ClAccess*	_GetSessionAccess( void ) ;

	void _ChangeState( ClLogonState* ai_LogonState, LogonStateId ai_LogonStateId )
	{
		m_poCurrentState = ai_LogonStateId ;
		m_poLogonState = ai_LogonState ;
	}

	void	_InitIndex( void ) ;


// Miembros ===================================================================

	m4bool_t			m_bIsInit ;

	m4uint8_t			m_iSmOltp ;
	eStateValidateExecutionNp_t m_eStateValidateExecutionNp;
	ClChannelManager	*m_poChannelManager ;
	ClVMBaseEnv			*m_poEnvironment ;
	ClCsTrace			*m_poCsTrace ;
	ClAppClientAdap		*m_poAPIClientAdaptor ;

	LogonStateId		m_poCurrentState ;
	ClLogonState		*m_poLogonState ;

    m4char_t			m_pzPublicCookie[ 256 ] ;

	m4char_t			m_acCredential[ M4CL_MAX_CREDENTIAL_SIZE + 1 ] ;

    CONNECTION_PROPERTIES_MAP m_oConnectionPropertiesMap ;


	m4int32_t			m_iLanguage_Index ;
	m4int32_t			m_iDevLanguage_Index ;
	m4int32_t			m_iDate_Start_Index ;
	m4int32_t			m_iDate_End_Index ;
	m4int32_t			m_iDate_Corr_Index ;
	m4int32_t			m_iLDB_Date_Start_Index ;
	m4int32_t			m_iLDB_Date_End_Index ;
	m4int32_t			m_iLDB_Date_Corr_Index ;
	m4int32_t			m_iVMDate_Start_Index ;
	m4int32_t			m_iVMDate_End_Index ;
	m4int32_t			m_iVMDate_Corr_Index ;
	m4int32_t			m_iUser_Name_Index ;
	m4int32_t			m_iRSM_Index ;
	m4int32_t			m_iRole_Index ;
	m4int32_t			m_iOrganization_Index ;
	m4int32_t			m_iLicense_Index ;
	m4int32_t			m_iIs_MultiOrg_Index ;
	m4int32_t			m_iRoundCurrency_Index ;
	m4int32_t			m_iRoundNumber_Index ;
	m4int32_t			m_iExecuteRealSQLMode_Index ;
	m4int32_t			m_iUsrAuditActive_Index ;
	m4int32_t			m_iUsrAuditSessionDate_Index ;
	m4int32_t			m_iUserTimeRCThreshold_Index ;
	m4int32_t			m_iUserAuditClientType_Index ;
	m4int32_t			m_iSrvCOMAppsLocked_Index;

public:
	// new validation functions
	m4uint8_t	CheckProjectTreeValidation( m4pchar_t ai_pcChannelId, m4pchar_t ai_pcOrg ) ;
	virtual ClExecuteItemI * GetExecuteItem();
	virtual IClSesChnExport * GetSessionExport();

	// User actions auditory.
	m4int32_t GetUserActionsAuditStatus( void )
	{
		return(  m_poLogonState->GetInt( this, m_iUsrAuditActive_Index, -1 ) ) ;
	}

	m4date_t GetUserActionsAuditSessionDate( void )
	{
		return( m_poLogonState->GetDate( this, m_iUsrAuditSessionDate_Index) ) ;
	}

	m4int32_t GetUserTimeRCThreshold( void )
	{
		return(  m_poLogonState->GetInt( this, m_iUserTimeRCThreshold_Index, -1 ) ) ;
	}

	m4int32_t GetUserAuditClientType( void )
	{
		return(  m_poLogonState->GetInt( this, m_iUserAuditClientType_Index, -1 ) ) ;
	}

	m4pchar_t GetSrvCOMAppsLocked( void )
	{
		return(  m_poLogonState->GetString( this, m_iSrvCOMAppsLocked_Index ) ) ;
	}

protected:
	m4int32_t			m_iUser_Alias_Name_Index ;
	m4int32_t			m_iID_Debug_User_Index ;
	m4int32_t			m_iUsr_Audit_Client_Pub_IP_Index;
} ;


#endif


