
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4lgadapi.cpp   
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
//    Este módulo contiene la implementación logon adaptor
//
//
//==============================================================================


#include "m4lgadapi.hpp"

#include "csres.hpp"
#include "chlog.hpp"
#include "handles.hpp"
#include "access.hpp"
#include "chan_knl.hpp"
#include "m4lang.hpp"
#include "m4cstrace.hpp"
#include "m4mdrt.hpp"
#include "certmngr.hpp"
#include "clseschnexport.hpp"
#include "m4objreg.hpp"
#include "m4objglb.hpp"
#include "m4cryptc.hpp"
#include "licensemanager.hpp"
#include "licensemanagerinterface.hpp"
#include "licres.hpp"
#include "m4win2ux.hpp"
#include "m4clock.hpp"
#include "tsap.hpp"
#include "m4props.hpp"
#include "iexecutor.hpp"

#ifndef _WINDOWS
#include <netdb.h>
#include <arpa/inet.h>
#endif

extern	m4date_t M4_DECL_M4OBJGLB g_dTimeCorrection ;

m4return_t	M4ExecuteItem( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut, m4pchar_t ai_pcRole, ClAccess **ao_ppoAccess )
{

	m4return_t			iResult = M4_SUCCESS;
	m4return_t			iReturn = M4_SUCCESS;
	m4uint16_t			i = 0;
	ClChannel			*poChannel = NULL;
	ClAccess			*poAccess = NULL;
	ClNode				*poNode = NULL;
	ClItem				*poItem = NULL;
	m4VariantType		vResult ;
	m4bool_t			bOwner = M4_TRUE;


	// Inicializamos la salida
	if( ao_ppoAccess != NULL && *ao_ppoAccess != NULL)
	{
		poAccess = *ao_ppoAccess;
		bOwner = M4_FALSE;
	}

	if (poAccess == NULL) {

		// Creamos el canal
		iResult = ai_poChannelManager->CreateChannel( poChannel ) ;

		if( iResult != M4_SUCCESS || poChannel == NULL )
		{
			return( M4_ERROR ) ;
		}

		// Ponemos el role
		if( ai_pcRole != NULL )
		{
			poChannel->Role_ID.Set( ai_pcRole ) ;
		}

		// Lo levantamos
		iResult = poChannel->BuildFromId( ai_pccM4Object ) ;

		if( iResult != M4_SUCCESS )
		{
			poChannel->Destroy() ;
			return( M4_ERROR ) ;
		}

		// Creamos el acceso
		poAccess = poChannel->CreateAccess() ;

		if( poAccess == NULL )
		{
			poChannel->Destroy() ;
			return( M4_ERROR ) ;
		}
	}

	// checks the node
	if (ai_pccNode == NULL) {
		if (ao_ppoAccess != NULL) {
			*ao_ppoAccess = poAccess;
		}
		else {
			if (bOwner) {
				poAccess->Destroy();
				poChannel->Destroy();
			}
		}
		return M4_SUCCESS;
	}

	// Cogemos el nodo
	poNode = &( poAccess->Node[ ai_pccNode ] ) ;

	if( poNode == NULL )
	{
		if (bOwner) {
			poAccess->Destroy();
			poChannel->Destroy();
		}
		return( M4_ERROR ) ;
	}

	// Cogemos el item
	poItem = &poNode->RecordSet.Current.Item[ ai_pccItem ] ;

	if( poItem->GetIndex() == M4DM_EOF_INDEX )
	{
		if (bOwner) {
			poAccess->Destroy();
			poChannel->Destroy();
		}
		return( M4_ERROR ) ;
	}
	
	// Ponemos los argumentos
	for( i = 0 ; i < ai_iLength ; i++ )
	{
		iResult = poItem->StackPush( ai_pvArguments[ i ] ) ;

		if( iResult != M4_SUCCESS )
		{
			if (bOwner) {
				poAccess->Destroy();
				poChannel->Destroy();
			}
			return( M4_ERROR ) ;
		}
	}

	// Ejecutamos el item
	iReturn = poItem->Call( NULL, ai_iLength ) ;

	// Sacamos los argumentos
	for( i = 0 ; i < ai_iOut ; i++ )
	{
		iResult = poItem->StackPop( ai_pvOuts[ i ] ) ;

		if( iResult != M4_SUCCESS )
		{
			if (bOwner) {
				poAccess->Destroy();
				poChannel->Destroy();
			}
			return( M4_ERROR ) ;
		}
	}

	// Sacamos el resultado
	iResult = poItem->StackPop( vResult ) ;

	if( iResult != M4_SUCCESS )
	{
		if (bOwner) {
			poAccess->Destroy();
			poChannel->Destroy();
		}
		return( M4_ERROR ) ;
	}

	if( ao_ppoAccess != NULL )
	{
		*ao_ppoAccess = poAccess ;
	}
	else
	{
		if (bOwner) {
			poAccess->Destroy();
			poChannel->Destroy();
		}
	}

	if( iReturn != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( vResult.Type == M4CL_CPP_TYPE_NUMBER )
	{
		iResult = m4return_t( vResult.Data.DoubleData ) ;
	}
	else
	{
		iResult = -1 ;
	}
	
	return( iResult ) ;
}

m4return_t	M4GetBinaryItem( ClAccess *ai_pAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pchar_t &ao_pOutput, m4uint32_t &ao_iOutLen)
{

	m4return_t			iResult ;
	ClNode				*poNode ;
	ClItem				*poItem;
	m4VariantType		vResult ;

	// Inicializamos la salida
	if( ai_pAccess == NULL )
	{
		return M4_ERROR;
	}

	// Cogemos el nodo
	poNode = &( ai_pAccess->Node[ ai_pccNode ] ) ;

	if( poNode == NULL )
	{
		return( M4_ERROR ) ;
	}

	// Cogemos el item
	poItem = & poNode->RecordSet.Current.Item[ ai_pccItem ] ;

	if( poItem->GetIndex() == M4DM_EOF_INDEX )
	{
		return( M4_ERROR ) ;
	}

	// comprobamos la longitud
	m4uint32_t iItemLen = poItem->Value.Size();

	if (ao_pOutput == NULL) {
		ao_pOutput = new m4char_t[iItemLen];
	}
	else {
		if (ao_iOutLen < iItemLen) {
			return M4_ERROR;
		}
	}

	// Leemos el valor del item
	iResult = poItem->Value.Get(ao_pOutput,iItemLen,0,&ao_iOutLen);

	return iResult;
}

m4return_t	M4SetBinaryItem( ClAccess *ai_pAccess, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pcchar_t ai_pInput, m4uint32_t ai_iInLen)
{

	m4return_t			iResult ;
	ClNode				*poNode ;
	ClItem				*poItem;
	m4VariantType		vResult ;

	// Inicializamos la salida
	if( ai_pAccess == NULL || ai_pInput == NULL)
	{
		return M4_ERROR;
	}

	// Cogemos el nodo
	poNode = &( ai_pAccess->Node[ ai_pccNode ] ) ;

	if( poNode == NULL )
	{
		return( M4_ERROR ) ;
	}

	// Cogemos el item
	poItem = & poNode->RecordSet.Current.Item[ ai_pccItem ] ;

	if( poItem->GetIndex() == M4DM_EOF_INDEX )
	{
		return( M4_ERROR ) ;
	}

	// Leemos el valor del item
	iResult = poItem->Value.Set(ai_pInput,ai_iInLen);

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return iResult;
}


//=================================================================================
//
// ClLogonState
//
// Clase para el estado del logon adaptor
// Debajo están sus hijas
//
//=================================================================================

ClLogLoggedOut*		ClLogLoggedOut :: _instance = 0 ;
ClLogWithoutRole*	ClLogWithoutRole :: _instance = 0 ;
ClLogOnline*		ClLogOnline :: _instance = 0 ;



ClLogLoggedOut*	ClLogLoggedOut :: Instance(	void )
{
	if (!_instance)
	{
		_instance = new ClLogLoggedOut;
	}

	return _instance;
}


ClLogWithoutRole*	ClLogWithoutRole :: Instance( void )
{
	if (!_instance)
	{
		_instance = new ClLogWithoutRole;
	}

	return _instance;
}


ClLogOnline*	ClLogOnline :: Instance( void )
{
	if (!_instance)
	{
		_instance = new ClLogOnline;
	}

	return _instance;
}




m4uint8_t	ClLogonState :: _CheckRoleAndOrg( ClLogonAdaptor* ai_LogonAdaptor, m4pchar_t ai_pcRole, m4pchar_t ai_pcOrg, m4pchar_t *ao_ppcRSM, m4uint16_t ai_iLength, m4bool_t ai_bIsLevel2 )
{

	m4return_t		iResult ;
	m4uint8_t		iTruncated = 0 ;
	m4uint16_t		iLength ;
	m4pchar_t		pcOut ;
	m4VariantType	avArguments[ 4 ] ;
	m4VariantType	avResults[ 1 ] ;


	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pcRole != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = ai_pcRole ;
	}
	else
	{
		avArguments[ 0 ].Data.PointerChar = "" ;
	}

	if( ai_pcOrg != NULL )
	{
		avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
		avArguments[ 1 ].Data.PointerChar = ai_pcOrg ;
	}
	else
	{
		avArguments[ 1 ].Type = M4CL_CPP_TYPE_NULL ;
		avArguments[ 1 ].Data.DoubleData = 0 ;
	}

	avArguments[ 2 ].Type = M4CL_CPP_TYPE_NUMBER ;

	if( ai_bIsLevel2 == M4_TRUE )
	{
		avArguments[ 2 ].Data.DoubleData = 2 ;
	}
	else
	{
		avArguments[ 2 ].Data.DoubleData = 1 ;
	}

	avArguments[ 3 ].Type = M4CL_CPP_TYPE_NULL ;
	avArguments[ 3 ].Data.DoubleData = 0 ;

	iResult = ai_LogonAdaptor->_ExecuteItem( NULL, "ROOT_SESSION", "CHECK_ORG_ROLE", avArguments, 4, avResults, 1, NULL, M4CL_SEC_SUPER_USER ) ;

	if( iResult != M4_SUCCESS )
	{
		if( iResult == M4_ERROR )
		{
			if( ai_pcRole == NULL )
			{
				ai_pcRole = "" ;
			}

			if( ai_pcOrg == NULL )
			{
				ai_pcOrg = "" ;
			}

			DUMP_CHLOG_ERRORF( M4_CH_MCR_INVALID_ROLE_ORG, ai_pcRole << ai_pcOrg ) ;
		}
		return( 0 ) ;
	}

	if( ao_ppcRSM != NULL )
	{
		pcOut = m4pchar_t( avResults[ 0 ] ) ;

		// El RSM nulo no es válido
		if( pcOut == NULL )
		{
			DUMP_CHLOG_ERROR( M4_CH_MCR_NULL_RSM ) ;
			return( 0 ) ;
		}

		iLength = strlen( pcOut ) ;

		if( iLength > ai_iLength )
		{
			iLength = ai_iLength ;
			iTruncated = 1 ;
		}

		memcpy( *ao_ppcRSM, pcOut, iLength ) ;
		( *ao_ppcRSM )[ iLength ] = '\0' ;

		if( iTruncated == 1 )
		{
			DUMP_CHLOG_WARNINGF( M4_CH_MCR_ATTR_TRUNCATED, M4CMPROP_CHANNEL_ID_RSM << pcOut << ai_iLength << *ao_ppcRSM ) ;
		}
	}

	return( 1 ) ;
}


m4return_t ClLogonState :: _Confirm( ClLogonAdaptor* ai_LogonAdaptor )
{
	return( ai_LogonAdaptor->_ExecuteItem( NULL, "ROOT_SESSION", "CONFIRM", NULL, 0, NULL, 0, NULL, NULL ) ) ;
}


m4uint8_t	ClLogonState :: _CheckProjectTreeValidation( ClLogonAdaptor* ai_LogonAdaptor, m4pchar_t ai_pcChannelId, m4pchar_t ai_pcOrg )
{

	m4return_t		iResult ;
	m4uint8_t		iTruncated = 0 ;
	m4VariantType	avArguments[ 2 ] ;


	if( ai_pcChannelId != NULL )
	{
		avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
		avArguments[ 0 ].Data.PointerChar = ai_pcChannelId ;
	}
	else
	{
		avArguments[ 0 ].Type = M4CL_CPP_TYPE_NULL ;
		avArguments[ 0 ].Data.DoubleData = 0 ;
	}

	if( ai_pcOrg != NULL )
	{
		avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
		avArguments[ 1 ].Data.PointerChar = ai_pcOrg ;
	}
	else
	{
		avArguments[ 1 ].Type = M4CL_CPP_TYPE_NULL ;
		avArguments[ 1 ].Data.DoubleData = 0 ;
	}

	iResult = ai_LogonAdaptor->_ExecuteItem( NULL, "PROJ_TREE_VALIDATION", "CHECK_T3_ORG", avArguments, 2, NULL, 0, NULL, M4CL_SEC_SUPER_USER ) ;

	if( iResult != M4_SUCCESS )
	{
		if( iResult == M4_ERROR )
		{
			if( ai_pcChannelId == NULL )
			{
				ai_pcChannelId = "" ;
			}

			if( ai_pcOrg == NULL )
			{
				ai_pcOrg = "" ;
			}

			DUMP_CHLOG_ERRORF( M4_CH_MCR_INVALID_ORG_T3, ai_pcChannelId << ai_pcOrg ) ;
		}
		return( 0 ) ;
	}

	return( 1 ) ;
}


m4int32_t	ClLogWithoutRole :: GetNumRole( ClLogonAdaptor* ai_LogonAdaptor )
{

	ClAccess	*poAccess = ai_LogonAdaptor->_GetSessionAccess() ;


	if( poAccess != NULL )
	{
		return( poAccess->Node[ ROLES_NODE_SESSION_CHANNEL ].RecordSet.Count() ) ;
	}

	return( 0 ) ;
}


m4return_t	ClLogWithoutRole :: GetRoleNameByPos( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_iPos, m4pcchar_t *ao_RoleName )
{

	ClAccess	*poAccess = ai_LogonAdaptor->_GetSessionAccess() ;


	if( poAccess != NULL )
	{
		ClRegister * pReg ;
		pReg = & ( poAccess->Node [ROLES_NODE_SESSION_CHANNEL].RecordSet.Register [ai_iPos] ) ;
		*ao_RoleName = (m4pcchar_t )pReg->Item[M4_ITEM_ROLES_N_ROLE].Value.Get() ;
	}
	else 
	{
		*ao_RoleName = 0 ;
	}

	return( *ao_RoleName ? M4_SUCCESS : M4_ERROR ) ;
}


m4return_t	ClLogWithoutRole :: GetIdRoleByPos( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_iPos, m4pcchar_t* ao_IdRole )
{

	ClAccess	*poAccess = ai_LogonAdaptor->_GetSessionAccess() ;


	if( poAccess != NULL )
	{
		ClRegister *pReg = & (poAccess->Node [ROLES_NODE_SESSION_CHANNEL].RecordSet.Register[ai_iPos]) ;
		*ao_IdRole = (m4pchar_t) pReg->Item [M4_ITEM_ROLES_ID_ROLE].Value.Get() ;
	}
	else 
	{
		*ao_IdRole = 0 ;
	}

	return( (*ao_IdRole) ? M4_SUCCESS : M4_ERROR ) ;
}


m4date_t ClLogOnline :: GetDate( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_ItemDef )
{

	m4date_t	*dDate = 0 ;
	ClAccess	*poAccess = ai_LogonAdaptor->_GetSessionAccess() ;


	if( poAccess != NULL )
	{
		ClRegister *pRegRoot = & (poAccess->Node [ ClHandle( ai_LogonAdaptor->m_iRootNode ) ].RecordSet.Register[SUPONGO_QUE_SOLO_HAY_UN_REGISTRO]) ;
		dDate = (m4date_t *) pRegRoot->Item [(ClItemIndex)ai_ItemDef].Value.Get() ;
	}
	else 
	{
		dDate = 0 ;
	}
	
	return( dDate ? m4date_t( *dDate ) : m4date_t (0) ) ;
}


m4int32_t	ClLogOnline :: GetInt( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_ItemDef, m4int32_t ai_iDefault )
{

	m4double_t	*pint = NULL ;
	ClAccess	*poAccess = NULL ;


	if( ai_ItemDef == -1 )
	{
		return( ai_iDefault ) ;
	}

	poAccess = ai_LogonAdaptor->_GetSessionAccess() ;

	if( poAccess != NULL )
	{
		ClRegister *pRegRoot = & (poAccess->Node [ ClHandle( ai_LogonAdaptor->m_iRootNode ) ].RecordSet.Register[SUPONGO_QUE_SOLO_HAY_UN_REGISTRO]) ;
		pint = (m4double_t *) pRegRoot->Item [(ClItemIndex)ai_ItemDef].Value.Get() ;
	}
	else 
	{
		pint = NULL ;
	}
		
	return( pint ? m4int32_t (*pint) : ai_iDefault ) ;
}





//=================================================================================
//
// ClLogonAdaptorImp
//
// Clase que implementa el logon adaptor
//
//=================================================================================


ClLogonAdaptorImp :: ClLogonAdaptorImp( void )
{
	m_bIsInit = M4_FALSE ;
	m_iSmOltp = 0 ;
	m_eStateValidateExecutionNp = STATE_VALIDATE_EXECUTION_NP_RED;
	m_poChannelManager = NULL ;
	m_poEnvironment = NULL ;
	m_poCsTrace = NULL ;
	m_poAPIClientAdaptor = NULL ;

	_ChangeState( ClLogLoggedOut::Instance(), M4CL_LOG_LOGGED_OUT ) ;

	*m_pzPublicCookie = '\0' ;
	*m_acCredential = '\0' ;


	m_iLanguage_Index = -1 ;
	m_iDevLanguage_Index = -1 ;
	m_iDate_Start_Index = -1 ;
	m_iDate_End_Index = -1 ;
	m_iDate_Corr_Index = -1 ;
	m_iLDB_Date_Start_Index = -1 ;
	m_iLDB_Date_End_Index = -1 ;
	m_iLDB_Date_Corr_Index = -1 ;
	m_iVMDate_Start_Index = -1 ;
	m_iVMDate_End_Index = -1 ;
	m_iVMDate_Corr_Index = -1 ;
	m_iUser_Name_Index = -1 ;
	m_iRSM_Index = -1 ;
	m_iRole_Index = -1 ;
	m_iOrganization_Index = -1 ;
	m_iLicense_Index = -1 ;
	m_iIs_MultiOrg_Index = -1 ;
	m_iRoundCurrency_Index = -1 ;
	m_iRoundNumber_Index = -1 ;
	m_iExecuteRealSQLMode_Index = -1 ;
	
	// User actions auditory.
	m_iUsrAuditActive_Index = -1 ;
	m_iUsrAuditSessionDate_Index = -1 ;
	m_iUserTimeRCThreshold_Index = -1 ;
	m_iUserAuditClientType_Index = -1 ;

	m_iUser_Alias_Name_Index = -1 ;
	m_iID_Debug_User_Index = -1 ;

	m_iSrvCOMAppsLocked_Index = -1;

	m_iUsr_Audit_Client_Pub_IP_Index = -1 ;
}



ClLogonAdaptorImp :: ~ClLogonAdaptorImp( void )
{
}



m4return_t	ClLogonAdaptorImp :: Init	(
											ClChannelManager	*ai_poChannelManager,
											ClVMBaseEnv			*ai_poEnvironment,
											ClCsTrace			*ai_poCsTrace,
											ClAppClientAdap		*ai_poAPIClientAdaptor,
											ClCacheDirectory	*ai_poCacheDirectory,
											eM4CsTypes_t		ai_eCsType
										)
{
	if( m_bIsInit == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}


	if( ai_poChannelManager == NULL || ai_poEnvironment == NULL || ai_poCsTrace == NULL )
	{
		return( M4_ERROR ) ;
	}

	if( ai_eCsType == eM4CsTypeClient )
	{
		if( ai_poAPIClientAdaptor == NULL )
		{
			return( M4_ERROR ) ;
		}
	}

	m_poChannelManager = ai_poChannelManager ;
	m_poEnvironment = ai_poEnvironment ;
	m_poCsTrace = ai_poCsTrace ;
	m_poAPIClientAdaptor = ai_poAPIClientAdaptor ;
	m_poCacheDirectory = ai_poCacheDirectory ;	// Puede ser NULL

	switch( ai_eCsType )
	{
		case	eM4CsTypeClient:

			_ChangeState( ClLogLoggedOut::Instance(), M4CL_LOG_LOGGED_OUT ) ;
			m_iSmOltp = 0 ;
			break;

		case	eM4CsTypeServer:

			_ChangeState( ClLogOnline::Instance(), M4CL_LOG_ONLINE ) ;
			m_iSmOltp = 0 ;
			break;

		case	eM4CsTypeClientEmul:

			_ChangeState( ClLogLoggedOut::Instance(), M4CL_LOG_LOGGED_OUT ) ;
			m_iSmOltp = 1 ;
			break;

		case	eM4CsTypeServerEmul:

			_ChangeState( ClLogOnline::Instance(), M4CL_LOG_ONLINE ) ;
			m_iSmOltp = 0 ;
			break;

		default:
			
			_ChangeState( ClLogLoggedOut::Instance(), M4CL_LOG_LOGGED_OUT ) ;
			m_iSmOltp = 0 ;
			break;
	}


	m_bIsInit = M4_TRUE ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClLogonAdaptorImp :: End( void )
{
	if( m_bIsInit == M4_FALSE )
	{
		return( M4_SUCCESS ) ;
	}


	_ChangeState( ClLogLoggedOut::Instance(), M4CL_LOG_LOGGED_OUT ) ;
	m_iSmOltp = 0 ;

	m_poCacheDirectory = NULL ;
	m_poAPIClientAdaptor = NULL ;
	m_poCsTrace = NULL ;
	m_poEnvironment = NULL ;
	m_poChannelManager = NULL ;


	m_bIsInit = M4_FALSE ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClLogonAdaptorImp :: Logon( m4pchar_t ai_pszUserName, m4pchar_t ai_pszPassword, m4language_t ai_Language, m4pchar_t ai_pszLogonServer, m4uint32_t ai_uiPortNumber, m4bool_t ai_bUseDefaultRole, m4pchar_t ai_pszSSLFile, m4bool_t ai_bUseSSL )
{
	return( _Logon( ai_pszUserName, ai_pszPassword, M4_FALSE, ai_Language, ai_pszLogonServer, ai_uiPortNumber, ai_bUseDefaultRole, ai_pszSSLFile, ai_bUseSSL ) ) ;
}


m4return_t	ClLogonAdaptorImp :: RestoreLogon( m4pchar_t ai_pszCredential, m4language_t ai_Language, m4pchar_t ai_pszLogonServer, m4uint32_t ai_uiPortNumber, m4pchar_t ai_pszSSLFile, m4bool_t ai_bUseSSL )
{
	if( m_iSmOltp == 1 )
	{
		DUMP_CHLOG_ERRORF( M4_CH_CS_NO_LOGON, ai_pszCredential ) ;
	}

	return( _Logon( "", ai_pszCredential, M4_TRUE, ai_Language, ai_pszLogonServer, ai_uiPortNumber, M4_TRUE, ai_pszSSLFile, ai_bUseSSL ) ) ;
}


m4return_t	ClLogonAdaptorImp :: LogOff( void )
{

	m4return_t	iResult ;
	m4pchar_t	pcUserId = NULL ;
		

	m_poCsTrace->Logoff( "", "" ) ;

	// User actions auditory.
	if( m_iSmOltp == 1 )
	{
		pcUserId = GetUsername() ;
		m_poChannelManager->UserActionsAudit.AddAction( NULL, NULL, ClUserAction::EV_LOGOFF, pcUserId, pcUserId, NULL, M4_FALSE ) ;
	}

	// En el logoff cliente se desactiva la auditoría y se persisten los eventos
	m_poChannelManager->UserActionsAudit.SwitchOff( M4_TRUE ) ;

	iResult = m_poLogonState->LogOff( this ) ;

// si es error tecnológico sacamos el error funcional
	if( iResult == M4_ERROR )
	{
		DUMP_CHLOG_ERROR( M4_CH_CS_NO_LOGOFF ) ;
	}

	return ( iResult ) ;
}


m4return_t	ClLogonAdaptorImp :: SetRole( m4int32_t ai_iPos, m4bool_t ai_bSetAsDefaultRole )
{

	m4return_t	iResult ;


	iResult = m_poLogonState->SetRole( this, ai_iPos, ai_bSetAsDefaultRole ) ;

// si es error tecnológico sacamos el error funcional
	if( iResult == M4_ERROR )
	{
		DUMP_CHLOG_ERRORF( M4_CH_CS_NO_SET_ROLE, ai_iPos ) ;
	}

	return ( iResult ) ;
}


m4return_t	ClLogonAdaptorImp :: ChangePassword( m4pchar_t ai_pszOldPassword, m4pchar_t ai_pszNewPassword )
{

	m4return_t	iResult ;


	if( ai_pszOldPassword == NULL )
	{
		ai_pszOldPassword = "" ;
	}

	if( ai_pszNewPassword == NULL )
	{
		ai_pszNewPassword = "" ;
	}

	iResult = m_poLogonState->ChangePassword( this, ai_pszOldPassword, ai_pszNewPassword ) ;

// si es error tecnológico sacamos el error funcional
	if( iResult == M4_ERROR )
	{
		DUMP_CHLOG_ERROR( M4_CH_CS_NO_CHANGE_PASSWORD ) ;
	}

	return ( iResult ) ;
}


m4uint8_t	ClLogonAdaptorImp :: CheckRoleAndOrg( m4pchar_t ai_pcRole, m4pchar_t ai_pcOrg, m4pchar_t *ao_ppcRSM, m4uint16_t ai_iLength, m4bool_t ai_bIsLevel2 )
{
	return ( m_poLogonState->CheckRoleAndOrg( this, ai_pcRole, ai_pcOrg, ao_ppcRSM, ai_iLength, ai_bIsLevel2 ) ) ;
}

m4return_t	ClLogonAdaptorImp :: Confirm( void )
{
	return ( m_poLogonState->Confirm( this ) ) ;
}

m4uint8_t	ClLogonAdaptorImp :: CheckProjectTreeValidation( m4pchar_t ai_pcChannelId, m4pchar_t ai_pcOrg )
{
	return ( m_poLogonState->CheckProjectTreeValidation( this, ai_pcChannelId, ai_pcOrg  ) ) ;
}

ClExecuteItemI * ClLogonAdaptorImp :: GetExecuteItem(  )
{
	ClChannel *poChannel = GetSessionChannelPointer();
	ClChannelManager *poCM = NULL;
	
	if (poChannel != NULL) {
		poCM = poChannel->GetpChannelManager();
	}

	if (poCM == NULL) {
		return NULL;
	}

	return new ClExecuteItem(poCM);
}

IClSesChnExport * ClLogonAdaptorImp :: GetSessionExport(  )
{
	return new ClSesChnExport;
}

m4return_t	ClLogonAdaptorImp :: BuildSessionChannel( void )
{

	m4return_t	iResult ;
	ClChannel	*poChannel = NULL ;


	if( m_poSessionChannel != NULL )
	{
		return( M4_SUCCESS ) ;
	}

	iResult = m_poChannelManager->CreateChannel( poChannel ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Set the ROLE of the session to "root"
	poChannel->Role_ID.Set( M4CL_SEC_SUPER_USER ) ;

	// Si es SM es de tipo BOTH. En otro caso según estaba

	if( m_iSmOltp == 1 )
	{
		m4VariantType	vCsType ;

		vCsType.Type = M4CL_CPP_TYPE_NUMBER ;
		vCsType.Data.DoubleData = M4CL_CSTYPE_BOTH ;
		poChannel->CsType.Set( vCsType ) ;
	}

	iResult = poChannel->BuildFromId( M4_SESSION_CHANNEL_ID ) ;

	if( iResult != M4_SUCCESS )
	{
		poChannel->Destroy() ;
		return( M4_ERROR ) ;
	}

	m_poSessionChannel = poChannel ;
	_InitIndex() ;

	return( M4_SUCCESS ) ;
}



ClChannel*	ClLogonAdaptorImp :: GetSessionChannel( void )
{

	m4return_t	iResult ;


	if( m_poSessionChannel != NULL )
	{
		return( m_poSessionChannel ) ;
	}

	iResult = BuildSessionChannel() ;
	// No hace falta comprobar el error, va por el puntero a NULL

	if( m_poSessionChannel == NULL )
	{
		DUMP_CHLOG_ERROR( M4_CH_CS_NO_SESSION_CHANNEL ) ;
		return( NULL ) ;
	}

	return( m_poSessionChannel ) ;
}



ClAccessRecordSet*	ClLogonAdaptorImp :: GetSessionRecordSet( void )
{

	ClAccess	*poAccess = _GetSessionAccess() ;


	if( poAccess != NULL )
	{
		return( &( poAccess->Node[ ClHandle( m_iRootNode ) ].RecordSet ) ) ;
	}

	return( NULL ) ;
}



m4return_t	ClLogonAdaptorImp :: SetSessionChannel( ClChannel* ai_poChannel )
{
	if( m_poSessionAccess != NULL )
	{
		m_poSessionAccess->Destroy() ;
		m_poSessionAccess = NULL ;
	}

	m_poSessionChannel = ai_poChannel ;

	if( m_poSessionChannel != NULL )
	{
		_InitIndex() ;
	}

	return( M4_SUCCESS ) ;
}



m4uint64_t	ClLogonAdaptorImp :: GetTimeStamp( void )
{
	ClAccessRecordSet	*poRecordSet = GetSessionRecordSet() ;

	if( poRecordSet != NULL )
	{
		return( poRecordSet->GetNodeTimeStamp() ) ;
	}
	return( 0 ) ;
}



m4pchar_t	ClLogonAdaptorImp :: GetRoleNameByPos(	m4int32_t ai_iPos )
{

	m4return_t	iResult ;
	m4pcchar_t	pccRoleName ;


	iResult = m_poLogonState->GetRoleNameByPos( this , ai_iPos, &pccRoleName ) ;

	if( iResult == M4_SUCCESS && pccRoleName != NULL )
	{
		return( m4pchar_t( pccRoleName ) ) ;
	}

	return( "" ) ;
}



m4pchar_t	ClLogonAdaptorImp :: GetIdRoleByPos( m4int32_t ai_iPos	)
{

	m4return_t	iResult ;
	m4pcchar_t	pccIdRole ;


	iResult = m_poLogonState->GetIdRoleByPos( this, ai_iPos, &pccIdRole ) ;

	if( iResult == M4_SUCCESS && pccIdRole != NULL )
	{
		return( m4pchar_t( pccIdRole ) ) ;
	}

	return( "" ) ;
}


m4return_t	ClLogonAdaptorImp :: InsertUserData( m4pchar_t ai_pszUser, m4language_t ai_language, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pszClientMachine, m4pchar_t ai_pcUserAlias, m4pchar_t ai_pcImpersonatorUser, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP )
{

	m4return_t			iResult ;
	m4return_t			iSet ;
	m4VariantType		vValue ;
	ClAccess			*poAccess ;
	ClRegister_Current	*poRegRoot ;


	CHECK_CHLOG_ERRORF( ai_pszUser == NULL, M4_ERROR, M4_CH_CS_INSERT_USER_DATA, ai_pszUser ) ;

// Vamos a dejar que se cree el canal sesión si no está.
// Esto es para que server funcione bien
    iResult = BuildSessionChannel() ;

	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_CS_INSERT_USER_DATA, ai_pszUser ) ;

	poAccess = _GetSessionAccess() ;

	CHECK_CHLOG_ERRORF( poAccess == NULL, M4_ERROR, M4_CH_CS_INSERT_USER_DATA, ai_pszUser ) ;

	poRegRoot = &( poAccess->Node.Get( ClHandle( m_iRootNode ) ) )->RecordSet.Current ;

	CHECK_CHLOG_ERRORF( poRegRoot == NULL, M4_ERROR, M4_CH_CS_INSERT_USER_DATA, ai_pszUser ) ;

	iResult = poRegRoot->Add() ;

	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_CH_CS_INSERT_USER_DATA, ai_pszUser ) ;


	iSet = M4_SUCCESS ;

	vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;


	// Usuario.
	vValue.Data.PointerChar = ai_pszUser ;
	if( m_iUser_Name_Index != -1 ) iSet |= poRegRoot->Item[ ClItemIndex( m_iUser_Name_Index ) ].Value.Set( vValue ) ;

	// Alias de usuario.
	vValue.Data.PointerChar = ai_pcUserAlias ;
	if( m_iUser_Alias_Name_Index != -1 ) iSet |= poRegRoot->Item[ ClItemIndex( m_iUser_Alias_Name_Index ) ].Value.Set( vValue ) ;

	// Usuario suplantador.
	vValue.Data.PointerChar = ai_pcImpersonatorUser ;
	if( m_iID_Debug_User_Index != -1 ) iSet |= poRegRoot->Item[ ClItemIndex( m_iID_Debug_User_Index ) ].Value.Set( vValue ) ;

	// Dirección IP pública del puesto cliente.
	vValue.Data.PointerChar = ai_pcClientPubIP;
	if( m_iUsr_Audit_Client_Pub_IP_Index != -1 ) iSet |= poRegRoot->Item[ ClItemIndex( m_iUsr_Audit_Client_Pub_IP_Index ) ].Value.Set( vValue ) ;

	// Role = M4ADM
	vValue.Data.PointerChar = M4CL_SEC_SUPER_USER ;
	if( m_iRole_Index != -1 ) iSet |= poRegRoot->Item[ ClItemIndex( m_iRole_Index ) ].Value.Set( vValue ) ;

	// RSM = M4ADM
	vValue.Data.PointerChar = M4CL_SEC_SUPER_USER ;
	if( m_iRSM_Index != -1 ) iSet |= poRegRoot->Item[ ClItemIndex( m_iRSM_Index ) ].Value.Set( vValue ) ;


	vValue.Type = M4CL_CPP_TYPE_NUMBER ;

	// Lenguaje
	vValue.Data.DoubleData = ai_language ;
	if( m_iLanguage_Index != -1 ) iSet |= poRegRoot->Item[ ClItemIndex( m_iLanguage_Index ) ].Value.Set( vValue ) ;

	// GMT
	vValue.Data.DoubleData = g_dTimeCorrection ;
	iSet |= poRegRoot->Item[ M4_ITEM_GMT_DELAY ].Value.Set( vValue ) ;

	//bg 102349. Logon Mechanism
	vValue.Data.DoubleData = ai_iLogonMechanism ;
	iSet |= poRegRoot->Item[ M4_LOGON_MECHANISM ].Value.Set( vValue ) ;
	
	// Fechas
	vValue.Type = M4CL_CPP_TYPE_DATE ;

    vValue.Data.DoubleData = M4CL_MINUS_INFINITE_IN_JULIAN ;
	if( m_iDate_Start_Index != -1 )		iSet |= poRegRoot->Item[ ClItemIndex( m_iDate_Start_Index )     ].Value.Set( vValue ) ;
	if( m_iLDB_Date_Start_Index != -1 )	iSet |= poRegRoot->Item[ ClItemIndex( m_iLDB_Date_Start_Index ) ].Value.Set( vValue ) ;
	if( m_iVMDate_Start_Index != -1 )	iSet |= poRegRoot->Item[ ClItemIndex( m_iVMDate_Start_Index )   ].Value.Set( vValue ) ;

    vValue.Data.DoubleData = M4CL_PLUS_INFINITE_IN_JULIAN ;
	if( m_iDate_End_Index != -1 )		iSet |= poRegRoot->Item[ ClItemIndex( m_iDate_End_Index )     ].Value.Set( vValue ) ;
	if( m_iLDB_Date_End_Index != -1 )	iSet |= poRegRoot->Item[ ClItemIndex( m_iLDB_Date_End_Index ) ].Value.Set( vValue ) ;
	if( m_iVMDate_End_Index != -1 )		iSet |= poRegRoot->Item[ ClItemIndex( m_iVMDate_End_Index )   ].Value.Set( vValue ) ;

    vValue.Data.DoubleData = ClActualDate() ;
	if( m_iDate_Corr_Index != -1 )		iSet |= poRegRoot->Item[ ClItemIndex( m_iDate_Corr_Index )     ].Value.Set( vValue ) ;
	if( m_iLDB_Date_Corr_Index != -1 )	iSet |= poRegRoot->Item[ ClItemIndex( m_iLDB_Date_Corr_Index ) ].Value.Set( vValue ) ;
	if( m_iVMDate_Corr_Index != -1 )	iSet |= poRegRoot->Item[ ClItemIndex( m_iVMDate_Corr_Index )   ].Value.Set( vValue ) ;

	iSet |= poRegRoot->Item[ M4_ITEM_USR_AUDIT_SESSION_DATE ].Value.Set( vValue ) ;
	
	// Session key para auditoría
	m4char_t	acAuditSessionKey[ 100 ] ;
	int			iYear, iMonth, iDay ;
	m4int32_t	lHour, lMinute, lSecond ;
	
	memset( acAuditSessionKey, 0, 100 ) ;
	ClDateToNumbers( vValue.Data.DoubleData, iYear, iMonth, iDay, lHour, lMinute, lSecond ) ;
	
	sprintf( acAuditSessionKey, "%s%d%02d%02d%02d%02d%02d", ai_pszUser, iYear, iMonth, iDay, lHour, lMinute, lSecond ) ;
	
	vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
	vValue.Data.PointerChar = acAuditSessionKey ;

	iSet |= poRegRoot->Item[ "USR_AUDIT_SESSION_KEY" ].Value.Set(vValue);

	// Máquina cliente para auditoría
	if( ai_pszClientMachine != NULL && *ai_pszClientMachine != '\0' )
	{
		vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
		vValue.Data.PointerChar = ai_pszClientMachine ;
	}
	else
	{
		vValue.Type = M4CL_CPP_TYPE_NULL ;
	}
	iSet |= poRegRoot->Item[ M4_ITEM_USR_AUDIT_CLIENT_MACHINE ].Value.Set( vValue ) ;


	// Información máquina cliente.
	if( ai_pcSystemInfo != NULL && *ai_pcSystemInfo != '\0' )
	{
		vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
		vValue.Data.PointerChar = ai_pcSystemInfo ;
	}
	else
	{
		vValue.Type = M4CL_CPP_TYPE_NULL ;
	}
	iSet |= poRegRoot->Item[ M4_ITEM_SYSTEM_INFO ].Value.Set( vValue ) ;

	// Información tipo de cliente.
	if( ai_pcClientType != NULL && *ai_pcClientType != '\0' )
	{
		vValue.Type = M4CL_CPP_TYPE_NUMBER ;
		vValue.Data.DoubleData = atoi(ai_pcClientType) ;
	}
	else
	{
		vValue.Type = M4CL_CPP_TYPE_NULL ;
	}
	iSet |= poRegRoot->Item[ M4_ITEM_USR_AUDIT_CLIENT_TYPE ].Value.Set( vValue ) ;

	CHECK_CHLOG_ERRORF( iSet != M4_SUCCESS, M4_ERROR, M4_CH_CS_INSERT_USER_DATA, ai_pszUser ) ;

	return( M4_SUCCESS ) ;
}


m4return_t ClLogonAdaptorImp :: SetConnectionParam( m4pcchar_t ai_pzParamName, m4pcchar_t ai_pzParamValue )
{
	string oAuxParamName, oAuxParamValue;

	oAuxParamName=ai_pzParamName;
	oAuxParamValue=ai_pzParamValue;
	
	CONNECTION_PROPERTIES_MAP_IT it;
	it = m_oConnectionPropertiesMap.find(oAuxParamName);
	if (it == m_oConnectionPropertiesMap.end())
	{
		m_oConnectionPropertiesMap.insert(CONNECTION_PROPERTIES_MAP::value_type(oAuxParamName,oAuxParamValue));
	}
	else
	{
		(*it).second = oAuxParamValue;
	}
	
	return M4_SUCCESS;
}


m4language_t	ClLogonAdaptorImp :: GetSessionLanguage( m4pcchar_t ai_pccThisT3Id, m4language_t ai_Language )
{

	m4return_t		iResult ;
	m4language_t	Language ;
	m4language_t	LanguageAux ;


// Nos vamos a conseguir el idioma si no somos el session channel
// y no nos lo dan por argumento

	if( ai_Language != M4CL_LANGUAGE_NONE )
	{
		Language = ai_Language ;
	}
	else
	{
		/* Bug 0085684
		Realmente no hace falta este cambio para el bug 0075632 y produce
		el bug 0085684, así que lo quitamos
		*/
		/* Bug 0075632
		Por defecto es inglés para que cuadre el cliente y el server en el caso de canal sesión
		*/
		Language = m_Language ;

		// para que en el canal sesión en server no sea recursivo
		if( m_poSessionChannel != NULL || ( ai_pccThisT3Id != NULL && strcmpi( ai_pccThisT3Id, M4_SESSION_CHANNEL_ID ) != 0 ) )
		{
			iResult = GetLanguage( LanguageAux ) ;

			if( iResult == M4_SUCCESS )
			{
				if( LanguageAux != 0 )
				{
					Language = LanguageAux;
				}
			}
		}
	}

	// We need a valid language
	M4ClLangInfo::Instance()->CheckAndSetValidLangId( Language ) ;

	return( Language ) ;
}



m4language_t	ClLogonAdaptorImp :: GetSessionDevLanguage( m4pcchar_t ai_pccThisT3Id, m4language_t ai_Language )
{

	m4return_t		iResult ;
	m4language_t	Language ;
	m4language_t	LanguageAux ;


// Nos vamos a conseguir el idioma si no somos el session channel
// y no nos lo dan por argumento

	if( ai_Language != M4CL_LANGUAGE_NONE )
	{
		Language = ai_Language ;
	}
	else
	{
		/* Bug 0085684
		Realmente no hace falta este cambio para el bug 0075632 y produce
		el bug 0085684, así que lo quitamos
		*/
		/* Bug 0075632
		Por defecto es inglés
		*/
		Language = m_Language ;

		// para que en el canal sesión en server no sea recursivo
		if( m_poSessionChannel != NULL || ( ai_pccThisT3Id != NULL && strcmpi( ai_pccThisT3Id, M4_SESSION_CHANNEL_ID ) != 0 ) )
		{
			iResult = GetDevLanguage( LanguageAux ) ;

			if( iResult == M4_SUCCESS )
			{
				if( LanguageAux != 0 )
				{
					Language = LanguageAux;
				}
			}
		}
	}

	// We need a valid language
	M4ClLangInfo::Instance()->CheckAndSetValidLangId( Language ) ;

	return( Language ) ;
}




// Funciones internas =========================================================


m4return_t ClLogonAdaptorImp :: _GetConnectionParam(m4pcchar_t ai_pzParamName,
                                              m4pchar_t  ao_pzParamValue)
{
   string oAuxParamName,
          oAuxParamValue;

   oAuxParamName=ai_pzParamName;

   CONNECTION_PROPERTIES_MAP_IT it=m_oConnectionPropertiesMap.find(oAuxParamName);

	if (it!=m_oConnectionPropertiesMap.end())
   {
		strcpy(ao_pzParamValue,(m4pchar_t)(((*it).second).c_str()));

      return M4_SUCCESS;
   }
   else 
      return M4_ERROR;
}


m4return_t	ClLogonAdaptorImp :: _UpdateRSM( m4pchar_t ai_pccRSM )
{

	m4return_t	iResult ;

	
	if( m_poSessionChannel == NULL )
	{
		return( M4_ERROR ) ;
	}

	_ChangeState( ClLogOnline :: Instance(), M4CL_LOG_ONLINE ) ;

	iResult = m_poSessionChannel->AttachRSM() ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERROR( M4_CH_CS_SET_MASK ) ;
		return( M4_ERROR ) ;
	}

	//---------------------------------------------------------------------
	//En el caso de DIC el estado lo tiene el LogonAdaptor, pero en SM se obtiene tras la inicializacion.
	m4uint8_t iSmOltp = _GetSmOltp();

	ClChannelManager * pChannelManager = NULL;
	if ((pChannelManager = _GetChannelManager()) != NULL)
	{
		//Si estamos aqui, es pq se ha podido inicializar la licencia correctamente.
		ILicenseManager * pLicenseManager = pChannelManager->GetpLicenseManager();
		eStateValidateExecutionNp_t eStateValidateExecutionNp;
		if (pLicenseManager != NULL) {
			if (iSmOltp == 1) {
				//En SM la inicializacion de la parte cliente de la licencia ya se hizo
				//cuando se inicializo la parte server.				
				eStateValidateExecutionNp = pLicenseManager->GetStateValidateExecutionNp();
			}
			else {
				//Importante:
				//En DIC la inicializacion de la parte cliente se debe hacer aqui.
				if (_InitClientLicense() != M4_SUCCESS) {
					DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
					return M4_ERROR;
				}

				//Si estamos en DIC es necesario dar el estado (que lo serializo el server) al cliente.
				//Es necesario hacer el set del estado en el cliente, por si alguien pregunta.
				eStateValidateExecutionNp = _GetStateValidateExecutionNp();
			}
			pLicenseManager->SetStateValidateExecutionNp(eStateValidateExecutionNp) ;
			//Ya puestos, dejamos en la log la informacion de la licencia.								
			string sLicenseSummaryEncrypted(pLicenseManager->GetLicenseSummaryEncrypted());
			if (sLicenseSummaryEncrypted.size() > 0) {
				DUMP_CHLOG_WARNINGF (M4_CH_LICENSE_INSTALLED, sLicenseSummaryEncrypted.c_str());
			}

			//En el logado validamos la expiracion de la licencia.
			if (pLicenseManager->ValidateLicense() == M4_FALSE) {
				DUMP_CHLOG_ERROR( M4_CH_LICENSE_EXPIRED );
				return M4_ERROR;
			}
		}
		else {
			//La licencia no puede estar a nulo.
			DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
			return M4_ERROR;
		}
	}
	//---------------------------------------------------------------------

	return( M4_SUCCESS ) ;
}


m4return_t	ClLogonAdaptorImp :: _ExecuteItem( m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut, m4pcchar_t ai_pcInstance, m4pchar_t ai_pcRole )
{

	m4return_t			iResult ;
	m4return_t			iReturn ;
	m4uint16_t			i ;
	ClChannel			*poChannel ;
	ClAccess			*poAccess ;
	ClAccess			*poSessionAccess ;
	ClNode				*poNode ;
	IExecutor			*poExecutor ;
	m4VariantType		vResult ;


	if( ai_pccM4Object != NULL && *ai_pccM4Object != '0' && strcmpi( ai_pccM4Object, M4_SESSION_CHANNEL_ID ) != 0 )
	{
		iResult = m_poChannelManager->CreateChannel( poChannel ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( poChannel == NULL )
		{
			return( M4_ERROR ) ;
		}

		if( ai_pcRole != NULL )
		{
			poChannel->Role_ID.Set( ai_pcRole ) ;
		}

		iResult = poChannel->BuildFromId( ai_pccM4Object ) ;

		if( iResult != M4_SUCCESS )
		{
			poChannel->Destroy() ;
			return( M4_ERROR ) ;
		}

		poAccess = poChannel->CreateAccess() ;

		if( poAccess == NULL )
		{
			poChannel->Destroy() ;
			return( M4_ERROR ) ;
		}
	}
	else
	{
		if( m_poSessionChannel == NULL )
		{
			return( M4_ERROR ) ;
		}

		poAccess = m_poSessionChannel->CreateAccess() ;

		if( poAccess == NULL )
		{
			return( M4_ERROR ) ;
		}

		// Lo ponemos a NULL para saber que no hay que destruirlo
		poChannel = NULL ;
	}

	poNode = &( poAccess->Node[ ai_pccNode ] ) ;

	if( poNode == NULL )
	{

		DUMP_CHLOG_ERRORF(M4_CH_CS_INVALID_SESSION_NODE, ai_pccNode << poAccess->GetpChannel()->GetpChannelDef()->Id() ) ;

		poAccess->Destroy() ;
		if( poChannel != NULL )
		{
			poChannel->Destroy() ;
		}
		return( M4_ERROR ) ;
	}

	if( ai_pcInstance != NULL && *ai_pcInstance != '\0' )
	{
		if( m_poSessionChannel == NULL )
		{
			poAccess->Destroy() ;
			if( poChannel != NULL )
			{
				poChannel->Destroy() ;
			}
			return( M4_ERROR ) ;
		}

		poSessionAccess = m_poSessionChannel->CreateAccess() ;

		if( poSessionAccess == NULL )
		{
			poAccess->Destroy() ;
			if( poChannel != NULL )
			{
				poChannel->Destroy() ;
			}
			return( M4_ERROR ) ;
		}

		iResult = poChannel->Level2.InsertL2Access( ai_pcInstance, poSessionAccess, M4_TRUE ) ;

		if( iResult != M4_SUCCESS )
		{
			poAccess->Destroy() ;
			if( poChannel != NULL )
			{
				poChannel->Destroy() ;
			}
			return( M4_ERROR ) ;
		}
	}
    
	poNode->RecordSet.Item[ ai_pccItem ] ;

	if( poNode->RecordSet.Item.GetIndex() == M4DM_EOF_INDEX )
	{

		DUMP_CHLOG_ERRORF(M4_CH_CS_INVALID_SESSION_ITEM, ai_pccItem << ai_pccNode << poAccess->GetpChannel()->GetpChannelDef()->Id() ) ;

		poAccess->Destroy() ;
		if( poChannel != NULL )
		{
			poChannel->Destroy() ;
		}
		return( M4_ERROR ) ;
	}

	/* Bug 0266622
	Se consigue el ejecutor del channelmanager, porque el canal sesión no tiene channelmanager.
	*/
	poExecutor = m_poChannelManager->GetpExecutor() ;
	
	for( i = 0 ; i < ai_iLength ; i++ )
	{
		iResult = poExecutor->Stack.Push( ai_pvArguments[ i ] ) ;

		if( iResult != M4_SUCCESS )
		{
			poAccess->Destroy() ;
			if( poChannel != NULL )
			{
				poChannel->Destroy() ;
			}
			return( M4_ERROR ) ;
		}
	}

	// jcrizacion
	iReturn = poNode->RecordSet.Item.Call( poExecutor, ai_iLength ) ;

	if( ai_pcInstance != NULL && *ai_pcInstance != '\0' )
	{
		iResult = poChannel->Level2.EraseL2Instance( ai_pcInstance, M4_FALSE ) ;

		// No se comprueba el resultado, porque debe ir bien siempre
	}

	for( i = 0 ; i < ai_iOut ; i++ )
	{
		iResult = poExecutor->Stack.Pop( ai_pvOuts[ i ] ) ;

		if( iResult != M4_SUCCESS )
		{
			poAccess->Destroy() ;
			if( poChannel != NULL )
			{
				poChannel->Destroy() ;
			}
			return( M4_ERROR ) ;
		}
	}

	iResult = poExecutor->Stack.Pop( vResult ) ;

	if( iResult != M4_SUCCESS )
	{
		poAccess->Destroy() ;
		if( poChannel != NULL )
		{
			poChannel->Destroy() ;
		}
		return( M4_ERROR ) ;
	}

	poAccess->Destroy() ;
	if( poChannel != NULL )
	{
		poChannel->Destroy() ;
	}

	if( iReturn != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( vResult.Type == M4CL_CPP_TYPE_NUMBER )
	{
		iResult = m4return_t( vResult.Data.DoubleData ) ;
	}
	else
	{
		iResult = -1 ;
	}
	
	return( iResult ) ;
}


m4return_t	ClLogonAdaptorImp :: _Logon( m4pchar_t ai_pszUserName, m4pchar_t ai_pszPassword, m4bool_t ai_bUseCredential, m4language_t ai_Language, m4pchar_t ai_pszLogonServer, m4uint32_t ai_uiPortNumber, m4bool_t ai_bUseDefaultRole, m4pchar_t ai_pszSSLFile, m4bool_t ai_bUseSSL )
{

	m4return_t	iResult ;
	m4char_t	acHostName[256] = "";
	hostent*	pHostInfo;
	char*		pcIPAddress;

	m_poCsTrace->Logon( ai_pszUserName, ai_pszLogonServer ) ;

	iResult = m_poLogonState->Logon( this, ai_pszUserName, ai_pszPassword, ai_bUseCredential, ai_Language, ai_pszLogonServer, ai_uiPortNumber, ai_bUseDefaultRole, ai_pszSSLFile, ai_bUseSSL ) ;

	// En el logon cliente se activa la auditoría (hasta el logoff)
	m_poChannelManager->UserActionsAudit.SwitchOn() ;

	// User actions auditory.
	if( m_iSmOltp == 1 )
	{
		// 9 -> No-default-role return code.
		if( iResult != M4_SUCCESS && iResult <= 20 && iResult != 9 )
		{
			// Init sockets, to be able to obtain host name.
			SocketInit();
			
			//En el tamaño del buffer incluimos el nulo final.
			M4_gethostname( acHostName, 256 ) ;

			// eh 272310
			pHostInfo = gethostbyname( acHostName ) ;
			if( pHostInfo != NULL )
			{
				pcIPAddress = inet_ntoa( *( in_addr* )pHostInfo->h_addr );
				if( pcIPAddress != NULL )
				{
					strncpy( acHostName, pcIPAddress, 255 );
					acHostName[ 255 ] = 0;
				}
			}

			m_poChannelManager->UserActionsAudit.PersistLogonError( ai_pszUserName, acHostName, M4_CLIENT_TYPE_SM ) ;
		}
	}


	// si es error tecnológico sacamos el error funcional
	if( iResult == M4_ERROR )
	{
		DUMP_CHLOG_ERRORF( M4_CH_CS_NO_LOGON, ai_pszUserName ) ;
	}

	return ( iResult ) ;
}


m4return_t	ClLogonAdaptorImp :: _Authenticate( m4pchar_t ai_pcUserId, m4pchar_t ai_pcPassword, m4bool_t ai_bUseDefaultRole, m4int8_t &ao_iLogonMechanism, m4pchar_t ao_pcRealUserId, m4pchar_t ao_pcImpersonatedUser )
{

	m4VariantType	avArguments[ 6 ] ;
	m4VariantType	avResults[ 3 ] ;
	m4return_t		iResult = M4_SUCCESS ;
	
	// Inicializamos argumentos de vuelta.
	ao_iLogonMechanism = -1;
	*ao_pcRealUserId = '\0' ;
	*ao_pcImpersonatedUser = '\0' ;
	
	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pcUserId != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = ai_pcUserId ;
	}
	else
	{
		avArguments[ 0 ].Data.PointerChar = "" ;
	}

	//Password one-way
	avArguments[ 1 ].Type = M4CL_CPP_TYPE_NULL ;

	//Password two-way
	avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pcPassword != NULL )
	{
		avArguments[ 2 ].Data.PointerChar = ai_pcPassword ;
	}
	else
	{
		avArguments[ 2 ].Data.PointerChar = "" ;
	}

	// eh 175325. Soporte para suplantación de usuarios.

	iResult = _ExecuteItem( "SSC_APPUSER", "SSC_APPUSER", "USER_AUTHENTICATION", avArguments, 6, avResults, 3, NULL, M4CL_SEC_SUPER_USER );
	
	
	if (avResults[0].Type == M4CL_CPP_TYPE_NUMBER)
	{
		ao_iLogonMechanism = (m4int8_t) avResults[0].Data.DoubleData;
	}


	// La autenticación nos devuelve el id usuario real, si procede.
	if( avResults[1].Type == M4CL_CPP_TYPE_STRING_VAR && avResults[1].Data.PointerChar != NULL )
	{
		m4int_t iLen = strlen( avResults[1].Data.PointerChar ) ;
		if( iLen < M4_MAX_USER_ID )
		{
			strcpy( ao_pcRealUserId, avResults[1].Data.PointerChar ) ;
		}
	}

	// La autenticación nos devuelve el usuario a suplantar, si procede.
	if( avResults[2].Type == M4CL_CPP_TYPE_STRING_VAR && avResults[2].Data.PointerChar != NULL )
	{
		m4int_t iLen = strlen( avResults[2].Data.PointerChar ) ;
		if( iLen < M4_MAX_USER_ID )
		{
			strcpy( ao_pcImpersonatedUser, avResults[2].Data.PointerChar ) ;
		}
	}

	return iResult;
}


m4return_t	ClLogonAdaptorImp :: _EnableRole( m4pchar_t ai_pcIdRole, m4bool_t ai_bSetAsDefaultRole )
{

	m4VariantType	avArguments[ 2 ] ;


	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pcIdRole != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = ai_pcIdRole ;
	}
	else
	{
		avArguments[ 0 ].Data.PointerChar = "" ;
	}

	avArguments[ 1 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 1 ].Data.DoubleData = ai_bSetAsDefaultRole ;

	return( _ExecuteItem( NULL, "ROOT_SESSION", "_CHECK_APP_ROLE", avArguments, 2, NULL, 0, NULL, M4CL_SEC_SUPER_USER ) ) ;
}


m4return_t	ClLogonAdaptorImp :: _ValidatePassword( m4pchar_t ai_pcPassword, m4pchar_t ai_pcRole )
{

	m4VariantType	avArguments[ 1 ] ;


	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pcPassword != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = ai_pcPassword ;
	}
	else
	{
		avArguments[ 0 ].Data.PointerChar = "" ;
	}

	return( _ExecuteItem( "SSC_VALIDATE", "SSC_VALIDATE", "VALIDATE_CLIENT", avArguments, 1, NULL, 0, M4_SESSION_CHANNEL_ID, ai_pcRole ) ) ;
}


m4return_t	ClLogonAdaptorImp :: _ValidateServerPassword( m4pchar_t ai_pcOldPassword, m4pchar_t ai_pcNewPassword, m4pchar_t ai_pcNewPasswordTwoWay, m4pchar_t ai_pcRole )
{

	m4VariantType	avArguments[ 3 ] ;


	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pcNewPassword != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = ai_pcNewPassword ;
	}
	else
	{
		avArguments[ 0 ].Data.PointerChar = "" ;
	}

	avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pcOldPassword != NULL )
	{
		avArguments[ 1 ].Data.PointerChar = ai_pcOldPassword ;
	}
	else
	{
		avArguments[ 1 ].Data.PointerChar = "" ;
	}

	avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pcNewPasswordTwoWay != NULL )
	{
		avArguments[ 2 ].Data.PointerChar = ai_pcNewPasswordTwoWay ;
	}
	else
	{
		avArguments[ 2 ].Data.PointerChar = "" ;
	}


	return( _ExecuteItem( "SSC_VALIDATE", "SSC_VALIDATE", "_VALIDATE_SERVER", avArguments, 3, NULL, 0, M4_SESSION_CHANNEL_ID, ai_pcRole ) ) ;
}


m4return_t	ClLogonAdaptorImp :: _RefreshSessionChannel( m4bool_t ai_bUseDefaultRole )
{

	m4return_t			iResult ;
	m4bool_t			iIsNotEof ;
	m4VariantType		vValue ;
	ClAccess			*poAccess ;
	ClNode				*poNode ;
	ClRegister_Current	*poUserRegister ;


	poAccess = _GetSessionAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	poUserRegister = &( poAccess->Node[ ClHandle( m_iRootNode ) ].RecordSet.Current ) ;

	if( poUserRegister == NULL )
	{
		return( M4_ERROR ) ;
	}

	iIsNotEof = poUserRegister->Begin() ;

	if( iIsNotEof == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	vValue.Type = M4CL_CPP_TYPE_NUMBER ;

	if( ai_bUseDefaultRole == 1 )
	{
		vValue.Data.DoubleData = 0 ;
	}
	else
	{
		vValue.Data.DoubleData = 1 ;
	}

	poNode = &( poAccess->Node[ ClHandle( m_iRootNode ) ] ) ;

	if( poNode == NULL )
	{
		return( M4_ERROR ) ;
	}

	poNode->RecordSet.Item[ "_REFRESH" ] ;

	if( poNode->RecordSet.Item.GetIndex() == M4DM_EOF_INDEX )
	{
		return( M4_ERROR ) ;
	}

	iResult = poNode->RecordSet.Item.StackPush( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poNode->RecordSet.Item.Call() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}
	
	iResult = poNode->RecordSet.Item.StackPop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = m4return_t( vValue.Data.DoubleData ) ;

	return( iResult ) ;
}

m4return_t	ClLogonAdaptorImp :: _InitClientLicense( void )
{
	m4pchar_t pcLicense = GetLicense();
	if (pcLicense == NULL) {
		DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
		return( M4_ERROR );
	}
	m4int32_t iLicenseSize = strlen(pcLicense);

	//Inicializamos la licencia del cliente.
	ILicenseManager * pLicenseManager = _GetChannelManager()->GetpLicenseManager();
	if ((pLicenseManager != NULL) && (pLicenseManager->Init(pcLicense, iLicenseSize, NULL) == M4_ERROR)) {
		DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
		return( M4_ERROR );
	}	

	return ( M4_SUCCESS );
}


m4return_t	ClLogonAdaptorImp :: _SetDefaultRoleAsActive( void )
{

	m4return_t			iResult ;
	m4bool_t			iIsNotEof ;
	m4VariantType		vValue ;
	ClAccess			*poAccess ;
	ClRegister_Current	*poUserRegister ;


	poAccess = _GetSessionAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	poUserRegister = &( poAccess->Node[ ClHandle( m_iRootNode ) ].RecordSet.Current ) ;

	if( poUserRegister == NULL )
	{
		return( M4_ERROR ) ;
	}

	iIsNotEof = poUserRegister->Begin() ;

	if( iIsNotEof == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	iResult = poUserRegister->Item[ M4_ITEM_CURRENT_ROLE_RSM ].Value.Get( vValue ) ;

	if( iResult != M4_SUCCESS || vValue.Type != M4CL_CPP_TYPE_STRING_VAR )
	{
		return( M4_ERROR ) ;
	}

//
// Call the method of the channel which applies the mask
// based on the current role to the chan nel
//

	iResult = _UpdateRSM( vValue.Data.PointerChar ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClLogonAdaptorImp :: _SetRoleAsActive( m4int32_t ai_iPos )
{

	m4return_t			iResult ;
	m4bool_t			iIsNotEof ;
	m4VariantType		vValue ;
	ClAccess			*poAccess ;
	ClRegister_Current	*poUserRegister ;
	ClRegister_Current	*poRolesRegister ;


	poAccess = _GetSessionAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	poUserRegister = &( poAccess->Node[ ClHandle( m_iRootNode ) ].RecordSet.Current ) ;

	if( poUserRegister == NULL )
	{
		return( M4_ERROR ) ;
	}

	iIsNotEof = poUserRegister->Begin() ;

	if( iIsNotEof == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	poRolesRegister = &( poAccess->Node[ ROLES_NODE_SESSION_CHANNEL ].RecordSet.Current ) ;

	if( poRolesRegister == NULL )
	{
		return( M4_ERROR ) ;
	}

	iIsNotEof = poRolesRegister->MoveTo( ai_iPos ) ;

	if( iIsNotEof == M4_FALSE )
	{
		DUMP_CHLOG_ERRORF( M4_CH_CS_NOT_VALID_ROL_REGISTER, ai_iPos ) ;
		return( M4_ERROR ) ;
	}


// Set the current role name in the user channel
	
	iResult = poRolesRegister->Item[ M4_ITEM_ROLES_ID_ROLE ].Value.Get( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poUserRegister->Item[ M4_ITEM_CURRENT_ROLE_ID ].Value.Set( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

// Set the current RSM in the user channel

	iResult = poRolesRegister->Item[ M4_ITEM_ROLES_RSM ].Value.Get( vValue ) ;

	if( iResult != M4_SUCCESS || vValue.Type != M4CL_CPP_TYPE_STRING_VAR )
	{
		return( M4_ERROR ) ;
	}

	iResult = poUserRegister->Item[ M4_ITEM_CURRENT_ROLE_RSM ].Value.Set( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


//
// Call the method of the channel which applies the mask
// based on the current role to the chan nel
//

	iResult = _UpdateRSM( vValue.Data.PointerChar ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


ClAccess*	ClLogonAdaptorImp :: _GetSessionAccess( void )
{
	if( m_poSessionAccess == NULL )
	{
		if( m_poSessionChannel != NULL )
		{
			m_poSessionChannel->CreateAccess( m_poSessionAccess ) ;
		}
	}

	return( m_poSessionAccess ) ;
}



void	ClLogonAdaptorImp :: _InitIndex( void )
{

	m4uint32_t		iHandle = 0 ;
	ClCompiledMCR	*poCMCR = NULL ;


	poCMCR = m_poSessionChannel->GetpChannel_Data()->GetpExtInfo()->GetpCMCR();
	m_iRootNode = poCMCR->GetChannelNodeHandleById( ROOT_NODE_SESSION_CHANNEL, M4_TRUE ) ;

	M4_ASSERT( m_iRootNode != 0 ) ;

	if( m_iRootNode != 0 )
    {
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_LANGUAGE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iLanguage_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_DEV_LANGUAGE,				M4_TRUE ) ;
		if( iHandle != 0 ) m_iDevLanguage_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_MCH_START_DATE,				M4_TRUE ) ;
		if( iHandle != 0 ) m_iDate_Start_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_MCH_END_DATE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iDate_End_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_MCH_CORR_DATE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iDate_Corr_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_LDB_START_DATE,				M4_TRUE ) ;
		if( iHandle != 0 ) m_iLDB_Date_Start_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_LDB_END_DATE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iLDB_Date_End_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_LDB_CORR_DATE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iLDB_Date_Corr_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_VM_START_DATE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iVMDate_Start_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_VM_END_DATE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iVMDate_End_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_VM_EXEC_DATE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iVMDate_Corr_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_USER,						M4_TRUE ) ;
		if( iHandle != 0 ) m_iUser_Name_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_CURRENT_ROLE_RSM,			M4_TRUE ) ;
		if( iHandle != 0 ) m_iRSM_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_CURRENT_ROLE_ID,			M4_TRUE ) ;
		if( iHandle != 0 ) m_iRole_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_ID_ORGANIZATION,			M4_TRUE ) ;
		if( iHandle != 0 ) m_iOrganization_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_LICENSE,					M4_TRUE ) ;
		if( iHandle != 0 ) m_iLicense_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_IS_MULTIORG,				M4_TRUE ) ;
		if( iHandle != 0 ) m_iIs_MultiOrg_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_ROUND_CURRENCY,			M4_TRUE ) ;
		if( iHandle != 0 ) m_iRoundCurrency_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_ROUND_NUMBER,				M4_TRUE ) ;
		if( iHandle != 0 ) m_iRoundNumber_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_EXECUTE_REALSQL_MODE,		M4_TRUE ) ;
		if( iHandle != 0 ) m_iExecuteRealSQLMode_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_USR_AUDIT_ACTIVE,			M4_TRUE ) ;
		if( iHandle != 0 ) m_iUsrAuditActive_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_USR_AUDIT_SESSION_DATE,	M4_TRUE ) ;
		if( iHandle != 0 ) m_iUsrAuditSessionDate_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_USR_TIME_AUDIT_RC_THRESHOLD,	M4_TRUE ) ;
		if( iHandle != 0 ) m_iUserTimeRCThreshold_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_USR_AUDIT_CLIENT_TYPE,	M4_TRUE ) ;
		if( iHandle != 0 ) m_iUserAuditClientType_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_USER_ALIAS,				M4_TRUE ) ;
		if( iHandle != 0 ) m_iUser_Alias_Name_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_DEBUG_USER,				M4_TRUE ) ;
		if( iHandle != 0 ) m_iID_Debug_User_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_SRV_COM_APPS_LOCKED,		M4_TRUE ) ;
		if( iHandle != 0 ) m_iSrvCOMAppsLocked_Index = poCMCR->GetItemIndex( iHandle ) ;
		iHandle = poCMCR->GetNodeItemHandleById( m_iRootNode, M4_ITEM_USR_AUDIT_CLIENT_PUB_IP,	M4_TRUE ) ;
		if( iHandle != 0 ) m_iUsr_Audit_Client_Pub_IP_Index = poCMCR->GetItemIndex( iHandle ) ;
    }
}


//=================================================================================
//
// ClExecute item Interface
//
// Classe implementación para la interfaz
//
//=================================================================================

// virtual destructor implementation
ClExecuteItemI::~ClExecuteItemI( )
{}

//=================================================================================
//
// ClExecute item
//
// Classe implementación para la interfaz
//
//=================================================================================
ClExecuteItem::ClExecuteItem(ClChannelManager *ai_poChannelManager )
{
	m_poChannelManager = ai_poChannelManager;
	m_poAccess = NULL;
	m_bOwner = M4_TRUE;
}

ClExecuteItem::~ClExecuteItem( )
{
	Reset();
	m_poChannelManager = NULL;
}

void ClExecuteItem::Reset()
{
	if (m_poAccess == NULL) {
		return;
	}

	if (m_bOwner) {
		ClChannel *poChannel = m_poAccess->GetpChannel();
		m_poAccess->Destroy();
		poChannel->Destroy();
	}

	m_poAccess = NULL;
	m_bOwner = M4_TRUE;
}

void ClExecuteItem::SetAccess (ClAccess *ai_poAccess)
{
	Reset();
	m_poAccess = ai_poAccess;
	m_bOwner = M4_FALSE;
}

	
m4return_t ClExecuteItem::Execute( m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut, m4pchar_t ai_pcRole, ClAccess **ao_ppoAccess )
{
	Reset();

	m4return_t iRes = M4ExecuteItem( m_poChannelManager, ai_pccM4Object, ai_pccNode, ai_pccItem, ai_pvArguments, ai_iLength, ai_pvOuts, ai_iOut, ai_pcRole, &m_poAccess );
	if (ao_ppoAccess != NULL) {
		*ao_ppoAccess = m_poAccess;
	}
	return iRes;
}

ClChannelManager* ClExecuteItem::GetChannelManager()
{
	return m_poChannelManager;
}

m4return_t ClExecuteItem::GetBinaryItem( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pchar_t &ao_pOutput, m4uint32_t &ao_iOutLen)

{
	if (m_poAccess == NULL) {
		return M4_ERROR;
	}

	return M4GetBinaryItem(m_poAccess, ai_pccNode, ai_pccItem, ao_pOutput, ao_iOutLen);
}

m4return_t ClExecuteItem::SetBinaryItem( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4pcchar_t ai_pInput, m4uint32_t ai_iInLen)

{
	if (m_poAccess == NULL) {
		return M4_ERROR;
	}

	return M4SetBinaryItem(m_poAccess, ai_pccNode, ai_pccItem, ai_pInput, ai_iInLen);
}
