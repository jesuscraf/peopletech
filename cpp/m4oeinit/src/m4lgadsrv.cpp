
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4lgadsrv.cpp   
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
//    Este módulo contiene la implementación logon adaptor parte cliente
//
//
//==============================================================================


#include "m4lgadapi.hpp"
#include "access.hpp"

m4pchar_t	ClLogonState :: GetString( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_ItemDef )
{

	ClAccess	*poAccess = NULL ;

	
	if( ai_ItemDef == -1 )
	{
		return( NULL ) ;
	}

	poAccess = ai_LogonAdaptor->_GetSessionAccess() ;

	if( poAccess != NULL )
	{
		ClRegister *pRegRoot = & (poAccess->Node [ ClHandle( ai_LogonAdaptor->m_iRootNode ) ].RecordSet.Register[SUPONGO_QUE_SOLO_HAY_UN_REGISTRO]) ;
		return (m4pchar_t) pRegRoot->Item [(ClItemIndex)ai_ItemDef].Value.Get() ;
	}

	return( NULL ) ;
}


m4return_t	ClLogonState :: _LogOff( ClLogonAdaptor* ai_LogonAdaptor )
{
//	DUMP_CHLOG_DEBUG( M4_CH_CS_LOGOFF_NOTIFY ) ;
	return( M4_ERROR ) ;
}


m4return_t	ClLogonState :: _SetRole( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_iPos, m4bool_t ai_bSetAsDefaultRole )
{
	return( M4_ERROR ) ;
}

m4return_t	ClLogonState :: _ChangePassword( ClLogonAdaptor* ai_LogonAdaptor, m4pchar_t ai_pszOldPassword, m4pchar_t ai_pszNewPassword )
{
//	DUMP_CHLOG_DEBUG( M4_CH_CS_LOGOFF_NOTIFY ) ;
	return( M4_ERROR ) ;
}


m4return_t	ClLogLoggedOut :: Logon
(
	ClLogonAdaptor*		ai_LogonAdaptor,
	m4pchar_t			ai_pszUserName, 
	m4pchar_t			ai_pszPassword, 
	m4bool_t			ai_bUseCredential,
	m4language_t		ai_Language, 
	m4pchar_t			ai_pzApplicationServerName,
	m4uint32_t			ai_uiApplicationServerPort,
	m4bool_t	        ai_bUseDefaultRole,
	m4pchar_t			ai_pszSSLFile,
	m4bool_t			ai_bUseSSL
)
{
//	DUMP_CHLOG_DEBUG( M4_CH_CS_LOGOFF_NOTIFY ) ;
	return( M4_ERROR ) ;
}


m4return_t ClLogWithoutRole :: SetRole( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_iPos,	m4bool_t ai_bSetAsDefaultRole )
{
//	DUMP_CHLOG_DEBUG( M4_CH_CS_LOGOFF_NOTIFY ) ;
	return( M4_ERROR ) ;
}


