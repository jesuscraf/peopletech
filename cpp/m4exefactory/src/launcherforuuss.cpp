//## begin module%38D91FFE021A.cm preserve=no
//## end module%38D91FFE021A.cm

//## begin module%38D91FFE021A.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%38D91FFE021A.cp

//## Module: LauncherForUUSS%38D91FFE021A; Subprogram body
//## Subsystem: M4ExeFactory::src%37D781A303AA
//## Source file: D:\eduardoma\m4exefactory\src\launcherforuuss.cpp

//## begin module%38D91FFE021A.additionalIncludes preserve=no
//## end module%38D91FFE021A.additionalIncludes

//## begin module%38D91FFE021A.includes preserve=yes
//	Rose 98 Body Include Dependences
#include <connects.hpp>
#include <usinterface.hpp>
#include <usservice.hpp>
#include <commandrequest.hpp>
#include <boomrequest.hpp>
#include <exeforuuss.hpp>
#include <credentialsfactory.hpp>
#include <rawsessionid.hpp>
#include <sessioncontext.hpp>
#include <clseschnexport.hpp>
//## end module%38D91FFE021A.includes

// LauncherInterface
#include <launcher.hpp>
// LauncherForUUSS
#include <launcherforuuss.hpp>
// logsys
#include <logsys.hpp>
// User actions auditory.
#include "usractaudit.hpp"

//## begin module%38D91FFE021A.declarations preserve=no
//## end module%38D91FFE021A.declarations

//## begin module%38D91FFE021A.additionalDeclarations preserve=yes
//## end module%38D91FFE021A.additionalDeclarations


/* Bug 0196830
Se pone el número de argumentos de la request con defines por si se cambian
*/
#define	M4LOGON_USER_ARG_NUMBER				13
#define	M4LOGON_JS_ARG_NUMBER				 3
#define	M4LOGON_CREDENTIAL_ARG_NUMBER		 8


// Class ClLauncherForUUSS 


ClLauncherForUUSS::ClLauncherForUUSS (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats, ClServiceQ *ai_poServiceQ, ClOutputQ *ai_poOutputQ)
  //## begin ClLauncherForUUSS::ClLauncherForUUSS%894280650.hasinit preserve=no
      : m_poLogSystem(NULL)
  //## end ClLauncherForUUSS::ClLauncherForUUSS%894280650.hasinit
  //## begin ClLauncherForUUSS::ClLauncherForUUSS%894280650.initialization preserve=yes
  , ClLauncherInterface (ai_pExecutive, NULL , ai_poServiceQ  )
  //## end ClLauncherForUUSS::ClLauncherForUUSS%894280650.initialization
{
  //## begin ClLauncherForUUSS::ClLauncherForUUSS%894280650.body preserve=yes
	if ( ai_poStats )
	{
		delete ai_poStats ;
		m_pStats = NULL ;
	}
  //## end ClLauncherForUUSS::ClLauncherForUUSS%894280650.body
}



//## Other Operations (implementation)
m4return_t ClLauncherForUUSS::Destroy ()
{
  //## begin ClLauncherForUUSS::Destroy%894305683.body preserve=yes
//	ADD	EMN	980812	Eliminar esta funcion en el futuro
	if (NULL != m_pExecutive)
	{
		m_pExecutive->ShutDown();
	}

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::Destroy%894305683.body
}

m4return_t ClLauncherForUUSS::Initialize (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClLauncherForUUSS::Initialize%896286626.body preserve=yes
	return ClLauncherInterface::Initialize(ai_pNavigator, ai_pConfig);
  //## end ClLauncherForUUSS::Initialize%896286626.body
}

m4return_t ClLauncherForUUSS::_GetNextRequest (ClBaseRequest *&ao_poRequest)
{
  //## begin ClLauncherForUUSS::_GetNextRequest%943350227.body preserve=yes
	m4uint32_t			reqType;
	ClBaseRequest		*poItem;

	do
	{
		m_pServiceQ->Pop(poItem);

		if (IsTerminate() )
			return M4_ERROR;

		reqType = poItem->GetRequestType();
	}
	while (M4_COMMAND_REQ_TYPE != reqType);

	ao_poRequest = (ClBaseRequest *)poItem;

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_GetNextRequest%943350227.body
}

m4return_t ClLauncherForUUSS::_PrepareExecution (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForUUSS::_PrepareExecution%943350228.body preserve=yes
	ClBoomerangRequest		*boomRequest = (ClBoomerangRequest *)ai_poRequest;

	ClBaseRequest			*poItem;
	boomRequest->GetRequest(poItem);

	ClCommandRequest		*usRequest = (ClCommandRequest *)poItem;

	m4return_t			ret;
	ClLogSystem			*poReqLogSystem = NULL;
	ret = usRequest->GetLog(poReqLogSystem);
	if (M4_SUCCESS != ret || NULL == poReqLogSystem)
	{
		m_poLogSystem = SETLOGSYSTEM(m_pLog);
	}
	else
	{
		SETLOGSYSTEM(poReqLogSystem);
	}

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_PrepareExecution%943350228.body
}

m4return_t ClLauncherForUUSS::_ExecuteRequest (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForUUSS::_ExecuteRequest%943350229.body preserve=yes
	ClBoomerangRequest		*boomRequest = (ClBoomerangRequest *)ai_poRequest;

	ClBaseRequest			*poItem;
	boomRequest->GetRequest(poItem);

	ClCommandRequest		*usRequest = (ClCommandRequest *)poItem;
	m4uint32_t			usCommand;
	usRequest -> GetCommand(usCommand);

	ClUSSessionInterface	*poSessionObj = NULL;
	ClUSUserInterface		*poUserObj = NULL;

	m4bool_t				bDefRole = M4_FALSE;
	m4uint32_t				argLen = 0,
							indexArg = 0,
							uiArgs = usRequest->GetNumberOfArgs();
	m4return_t				iRet;
	m4handle_t				hObject;

	switch(usCommand)
	{
	case M4_US_AUTHENTICATE:
	case M4_US_CERT_LOGIN :

		iRet = _Authenticate(*usRequest, poUserObj, poSessionObj);
		
		usRequest->SetResult(iRet);
		
		// User actions auditory.
		// 9 -> No-default-role return code.
		if( iRet != M4_SUCCESS && iRet <= 20 && iRet != 9 )
		{
			iRet = _UserAuditoryLogonErrorAction( *usRequest ) ;
		}
		break;

	case M4_US_CHECK_ROLE:

		hObject = usRequest->GetHandle();
		poSessionObj = (ClUSSessionInterface *)hObject;
		iRet = _Authorize(*usRequest, poSessionObj);

		break;

	case M4_US_CHANGE_PWD:

		hObject = usRequest->GetHandle();
		poSessionObj = (ClUSSessionInterface *)hObject;
		iRet = _ChangePassword(*usRequest, poSessionObj);
	
		break;

	case M4_US_GET_CERT :

		iRet = _GetCertificate (*usRequest);

		break ;

	case M4_US_CLOSE_AUDITORY :

		iRet = _CloseAuditory (*usRequest);
		break ;

	case M4_US_CLEAN_AUDITORY :

		iRet = _CleanAuditory (*usRequest);
		break ;

	case M4_US_USR_AUDIT_LOGOFF :

		iRet = _UserAuditoryLogoffAction( *usRequest ) ; // User actions auditory.
		break ;

	default:
		return M4_ERROR;
		break;
	}

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_ExecuteRequest%943350229.body
}

m4return_t ClLauncherForUUSS::_ReplyRequest (ClBaseRequest *ai_poRequest)
{
  //## begin ClLauncherForUUSS::_ReplyRequest%943350230.body preserve=yes
	if (NULL != m_poLogSystem)
	{
		SETLOGSYSTEM(m_poLogSystem);
	}

	ClBoomerangRequest		*boomRequest = (ClBoomerangRequest *)ai_poRequest;
	m4return_t				iRet = boomRequest->Return();
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "EXE4UUSS: BoomRequest unable to return!!!!");
	}

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_ReplyRequest%943350230.body
}

class ClLauncherResetter {
private: 
	ClExeForUUSS *m_poExecutive;
public:
	ClLauncherResetter(ClExeForUUSS *ai_poLauncher) {
		m_poExecutive = ai_poLauncher;
	}
	~ClLauncherResetter() {
		if (m_poExecutive == NULL) {
			return;
		}
		m_poExecutive->Reset();
	}
};
	

m4return_t ClLauncherForUUSS::_Authenticate (ClCommandRequest &aio_oAuthReq, ClUSUserInterface *&ao_poUserObj, ClUSSessionInterface *&ao_poSessionObj)
{
  //## begin ClLauncherForUUSS::_Authenticate%953729581.body preserve=yes
	m4uint32_t				uiArgs = aio_oAuthReq.GetNumberOfArgs();
	ClUSSessionInterface	*poOrgSession =NULL ;
	m4return_t				iAuthRet,
							iSessRet;
	intptr_t				iConnection;
	m4uint32_t				uiArgLen = 0,
							uiIndexArg = uiArgs;
	ClConnInstance			*pConnection;
	m4pchar_t				szUserName,
							szNewUserPwd,
							szStrId,
							szUserLanguage = NULL,
							szUserOrganization = NULL,
							szClientMachine = NULL;
							
	m4pchar_t				szCertificate = NULL ,
							szSessContext = NULL ,
							szCertificateType ;
	m4uint32_t				iCertificateLen ;
	m4bool_t				bWithDefRole = M4_FALSE, bClone = M4_FALSE ;
	m4handle_t				uiSessionId;
	m4handle_t				hObject;
	m4bool_t				bUse2Way = M4_FALSE;
	m4int8_t				iLogonMechanism = -1;	// bg 102349.

	m4pchar_t				szLogonOrigin = NULL;   // luciag 
	m4int8_t				iLogonOrigin  = 0;		// luciag

	m4bool_t				bAuditSession = M4_FALSE;
	m4bool_t				bCheckConcurrency = M4_FALSE;
	m4char_t				acUserIdAlias[ M4_MAX_USER_ALIAS ] ;
	m4pchar_t				pcUserIdAlias = acUserIdAlias ;
	m4char_t				acImpersonatorUser[ M4_MAX_USER_ID ] ;
	m4pchar_t				pcImpersonatorUser = acImpersonatorUser ;
	m4pchar_t				szSystemInfo = NULL;
	m4pchar_t				szClientType = NULL;
	m4pchar_t				szClientPubIP = NULL;

	iAuthRet = ((ClExeForUUSS *)m_pExecutive)->Reset();
	if (iAuthRet != M4_SUCCESS) {
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run : Error reseting executor");
		aio_oAuthReq.SetResult(M4_ERROR);
		return M4_ERROR;
	}

	ClLauncherResetter oLauncherResetter((ClExeForUUSS *)m_pExecutive);
	
	*pcUserIdAlias = '\0' ;
	*pcImpersonatorUser = '\0' ;

	switch ( uiArgs )
	{
	case M4LOGON_USER_ARG_NUMBER :

		//	13th Argument	ClientPubIP
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szClientPubIP))
			return M4_ERROR;
		
		//	12th Argument	ClientType
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szClientType))
			return M4_ERROR;
		
		//	11th Argument	SystemInfo
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szSystemInfo))
			return M4_ERROR;
		
		//	10th Argument	LogonOrigin
		if (M4_SUCCESS == _GetStringArg(aio_oAuthReq, --uiIndexArg, szLogonOrigin))
		{
			// si lo puede recuperar lo convierte a entero para el authenticate
			if ( szLogonOrigin != NULL) 
			{
				iLogonOrigin = atoi( szLogonOrigin ) ;
			}
		}

		//	9th Argument	ClientMachine
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szClientMachine ))
			return M4_ERROR;

		//	8th Argument	UseWay
		if (M4_SUCCESS != _GetBoolArg(aio_oAuthReq, --uiIndexArg, bUse2Way ))
			return M4_ERROR;

		//	7th Argument	Organization
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szUserOrganization ))
			return M4_ERROR;

		//	6th Argument	Language
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szUserLanguage))
			return M4_ERROR;

		//	5th Argument	DEFROLE
		if (M4_SUCCESS != _GetBoolArg(aio_oAuthReq, --uiIndexArg, bWithDefRole))
			return M4_ERROR;

		//	4th Argument	CONNECTION
		if (M4_SUCCESS != _GetIntPtrArg(aio_oAuthReq, --uiIndexArg, iConnection))
			return M4_ERROR;

		pConnection = (ClConnection *)iConnection;
		if (pConnection)
		{
			m4uint32_t Status = pConnection -> GetStatus() ;
			if ( ( Status == CLOSSING ) || (Status == CLOSED ) || ( Status == BROKEN ) )
			{
				SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run : Clossing connection ; Authentication aborted" );
				aio_oAuthReq.SetResult ( M4_ERROR ) ;
				return M4_ERROR;
			}
		}

		//	3rd Argument	SESSIONID
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szStrId))
			return M4_ERROR;

		sscanf(szStrId, M4_PRIdPTR, &uiSessionId);

		//	2nd Argument	USERPWD
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szNewUserPwd))
			return M4_ERROR;

		//	1st Argument	USERNAME (NULL if USERPWD == CREDENTIAL)
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szUserName ))
			return M4_ERROR;

		if (NULL != szUserName)
		{
			iAuthRet = ((ClExeForUUSS *)m_pExecutive)->Authenticate(szUserName, szNewUserPwd, bUse2Way, ao_poUserObj, iLogonMechanism, pcUserIdAlias, pcImpersonatorUser, iLogonOrigin, szClientType, szClientPubIP);

			// En logado por usuario y password se audita y se chequea concurrencia
			bAuditSession = M4_TRUE ;
			bCheckConcurrency = M4_TRUE ;
		}
		else				// Validate User
		{
			M4Credentials * poCredential = ClCredentialsFactory::CreateCredential  ( szNewUserPwd ) ;

			iAuthRet = ((ClExeForUUSS *)m_pExecutive)->ValidateCredential(*poCredential, ao_poUserObj, poOrgSession);
			ao_poSessionObj = poOrgSession ;
			bClone = M4_TRUE ;

			// En clonado de sesión se audita pero no se chequea concurrencia
			bAuditSession = M4_TRUE ;
			bCheckConcurrency = M4_FALSE ;
		}
		break ;

	case M4LOGON_JS_ARG_NUMBER :
		//	ClientMachine
		// En jobscheduler el client machine ya debería estar en el canal sesión del usuario

		//	3rd Argument	CONNECTION
		if (M4_SUCCESS != _GetIntPtrArg(aio_oAuthReq, --uiIndexArg, iConnection)) 	return M4_ERROR;

		pConnection = (ClConnInstance *)iConnection;
		if (pConnection)
		{
			m4uint32_t Status = pConnection -> GetStatus () ;
			if ( ( Status == CLOSSING ) || (Status == CLOSED ) || ( Status == BROKEN ) )
			{
				SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run : Clossing connection ; Authentication aborted" );
				aio_oAuthReq.SetResult ( M4_ERROR ) ;
				return M4_ERROR;
			}
		}

		//	2nd Argument	SESSIONID
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szStrId))
			return M4_ERROR;

		sscanf(szStrId, M4_PRIdPTR, &uiSessionId);

		//	1st Argument	USERNAME
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szUserName))
			return M4_ERROR;

		iAuthRet = ((ClExeForUUSS *)m_pExecutive)->AuthenticateJob(szUserName, ao_poUserObj);

		// En logado de job scheduler no se audita ni se chequea concurrencia
		bAuditSession = M4_FALSE ;
		bCheckConcurrency = M4_FALSE ;

	break ;

	case M4LOGON_CREDENTIAL_ARG_NUMBER :
		//	8th Argument	ClientType
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szClientType ))
			return M4_ERROR;

		//	7th Argument	SystemInfo
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szSystemInfo ))
			return M4_ERROR;

		//	6th Argument	ClientMachine
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szClientMachine ))
			return M4_ERROR;

		//	5rd Argument	CONNECTION
		if (M4_SUCCESS != _GetIntPtrArg(aio_oAuthReq, --uiIndexArg, iConnection)) 	return M4_ERROR;

		pConnection = (ClConnInstance *)iConnection;
		if (pConnection)
		{
			m4uint32_t Status = pConnection -> GetStatus () ;
			if ( ( Status == CLOSSING ) || (Status == CLOSED ) || ( Status == BROKEN ) )
			{
				SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run : Clossing connection ; Authentication aborted" );
				aio_oAuthReq.SetResult ( M4_ERROR ) ;
				return M4_ERROR;
			}
		}
		//	4th Argument	SESSIONID
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szStrId))
			return M4_ERROR;

		sscanf(szStrId, "%d", &iCertificateLen);

		//	3th Argument	Language
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szSessContext ))
			return M4_ERROR;

		//	2nd Argument	certificado
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szCertificate ))
			return M4_ERROR;

		//	1st Argument	tipo de certificado
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szCertificateType ))
			return M4_ERROR;

		iAuthRet = ((ClExeForUUSS *)m_pExecutive)->Authenticate (ao_poUserObj, ao_poSessionObj, pConnection, szCertificateType, szCertificate , iCertificateLen , szSessContext ) ;

		// En logado por credencial se audita pero no se chequea concurrencia
		bAuditSession = M4_TRUE ;
		bCheckConcurrency = M4_FALSE ;

		break ;

	default :
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Wrong number of args for Authenticate!!!!");
		return M4_ERROR;
	}

	if (M4_SUCCESS > iAuthRet)
	{
		return iAuthRet;
	}


	// Si hay que auditar se comprueba si el usuario tiene activada la auditoría
	if( bAuditSession == M4_TRUE )
	{
		m4return_t	iResult = M4_ERROR ;
		m4string_t	sUser ;

		// bg 176316
		// Si hay suplantación el control de concurrencia no se realiza sobre el suplantado, sino sobre el suplantador.
		ao_poUserObj->GetUserImpersonator( sUser ) ;

		if( sUser == "" )
		{
			ao_poUserObj->GetUserId( sUser ) ;
		}

		iResult = ((ClExeForUUSS *)m_pExecutive)->CheckAuditory( sUser.c_str(), bCheckConcurrency, bAuditSession ) ;

		if( iResult != M4_SUCCESS )
		{
			return( iResult ) ;
		}
	}

	// if (M4_SUCCESS != iAuthRet)
	// bg 191019 (luciag). If auth successful (code 0) or password about to expire (code 20 and above), run CHECK_APP_ROLE and clone the session obj 
	if (M4_SUCCESS != iAuthRet && iAuthRet < 20)
	{
		bWithDefRole = M4_FALSE ;
		bClone = M4_FALSE ;
	}

	iSessRet = ((ClExeForUUSS *)m_pExecutive)->CreateSession(ao_poUserObj, szStrId, ao_poSessionObj, pConnection, szUserLanguage, szUserOrganization, bWithDefRole , bClone , szSessContext, iLogonMechanism, szClientMachine, bAuditSession, pcUserIdAlias, pcImpersonatorUser, iLogonOrigin, szSystemInfo, szClientType, szClientPubIP);


	if( poOrgSession )
	{
		poOrgSession ->RemoveRequest();
	}

	if (M4_SUCCESS <= iSessRet)
	{
		hObject = (m4handle_t)ao_poSessionObj;
		aio_oAuthReq.SetHandle(hObject);
//		ao_poSessionObj -> SetId(uiSessionId);
	}

	if (M4_SUCCESS != iSessRet)
		return iSessRet;

	return iAuthRet;
  //## end ClLauncherForUUSS::_Authenticate%953729581.body
}

m4return_t ClLauncherForUUSS::_Authorize (ClCommandRequest &aio_oAuthReq, ClUSSessionInterface *&aio_poSessionObj)
{
  //## begin ClLauncherForUUSS::_Authorize%953729582.body preserve=yes
	m4uint32_t				uiArgs = aio_oAuthReq.GetNumberOfArgs();
	if (2 < uiArgs)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run: Wrong number of args for Authorize!!!!");
		return M4_ERROR;
	}

	m4return_t				iRet;
	m4uint32_t				uiArgLen = 0,
							uiIndexArg = uiArgs;
	m4pchar_t				szArgument,
							szRoleID = NULL,
							szWithDefRole;
	m4bool_t				bWithDefRole = M4_FALSE;

	switch (uiArgs)
	{
	case 2:
//	2nd Argument	SET DEF ROLE?
		iRet = aio_oAuthReq.GetArg(--uiIndexArg, szArgument, uiArgLen);
		if ((M4_SUCCESS != iRet) || (NULL == szArgument))
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run : Error in 2nd parameter for Check Role!!!!");
			return M4_ERROR;
		}

		szWithDefRole = szArgument;
		if (0 < szWithDefRole[0])
			bWithDefRole = M4_TRUE;

	case 1:
//	1st Argument	ROLE ID
		iRet = aio_oAuthReq.GetArg(--uiIndexArg, szArgument, uiArgLen);
		if ((M4_SUCCESS != iRet) || (NULL == szArgument))
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run : Error in 1st parameter for Check Role!!!!");
			return M4_ERROR;
		}

		szRoleID = szArgument;
		break;

	}

	iRet = ((ClExeForUUSS *)m_pExecutive)->CheckRole(*aio_poSessionObj, szRoleID, bWithDefRole);
	aio_oAuthReq.SetResult(iRet);

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_Authorize%953729582.body
}

m4return_t ClLauncherForUUSS::_CreateSession (ClCommandRequest &aio_oAuthReq)
{
  //## begin ClLauncherForUUSS::_CreateSession%953729583.body preserve=yes
	return M4_ERROR;
  //## end ClLauncherForUUSS::_CreateSession%953729583.body
}

m4return_t ClLauncherForUUSS::_ChangePassword (ClCommandRequest &aio_oAuthReq, ClUSSessionInterface *&aio_poSessionObj)
{
  //## begin ClLauncherForUUSS::_ChangePassword%953729584.body preserve=yes
	m4uint32_t				uiArgs = aio_oAuthReq.GetNumberOfArgs();
	if (2 != uiArgs)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "EXE4UUSS: Wrong number of args for ChangePassword!!!!");
		return M4_ERROR;
	}

	m4return_t				iRet;
	m4uint32_t				uiArgLen = 0,
							uiIndexArg = 0;
	m4pchar_t				szArgument,
							szOldUserPwd,
							szNewUserPwd;
	

//	1st Argument	Old Password
	iRet = aio_oAuthReq.GetArg(uiIndexArg++, szArgument, uiArgLen);
	if ((M4_SUCCESS != iRet) || (NULL == szArgument))
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run : Error in 1st parameter for Change Pwd!!!!");
		return M4_ERROR;
	}

	szOldUserPwd = szArgument;

//	2nd Argument	New Password
	iRet = aio_oAuthReq.GetArg(uiIndexArg++, szArgument, uiArgLen);
	if ((M4_SUCCESS != iRet) || (NULL == szArgument))
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run : Error in 1st parameter for Change Pwd!!!!");
		return M4_ERROR;
	}

	szNewUserPwd = szArgument;

	iRet = ((ClExeForUUSS *)m_pExecutive)->ChangePassword(*aio_poSessionObj, szOldUserPwd, szNewUserPwd);
	aio_oAuthReq.SetResult(iRet);

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_ChangePassword%953729584.body
}

m4return_t ClLauncherForUUSS::_GetStringArg (const ClCommandRequest &ai_oRequest, m4uint32_t ai_uiIndex, m4pchar_t &ao_strArgument)
{
  //## begin ClLauncherForUUSS::_GetStringArg%953831063.body preserve=yes
	m4pchar_t		szArgument = NULL;
	m4uint32_t		uiArgLen;

	if (M4_SUCCESS != ai_oRequest.GetArg(ai_uiIndex, szArgument, uiArgLen))
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Error in parameter %d!!!!", ai_uiIndex);
		return M4_ERROR;
	}

	ao_strArgument = szArgument;

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_GetStringArg%953831063.body
}

m4return_t ClLauncherForUUSS::_GetBoolArg (const ClCommandRequest &ai_oRequest, m4uint32_t ai_uiIndex, m4bool_t &ao_bArgument)
{
  //## begin ClLauncherForUUSS::_GetBoolArg%953831064.body preserve=yes
	m4pchar_t		szArgument;

	if (M4_SUCCESS != _GetStringArg(ai_oRequest, ai_uiIndex, szArgument))
		return M4_ERROR;

	ao_bArgument = (m4bool_t)szArgument[0];

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_GetBoolArg%953831064.body
}

m4return_t ClLauncherForUUSS::_GetIntArg (const ClCommandRequest &ai_oRequest, m4uint32_t ai_uiIndex, m4int_t &ao_iArgument)
{
  //## begin ClLauncherForUUSS::_GetIntArg%953831065.body preserve=yes
	m4pchar_t		szArgument;

	if (M4_SUCCESS != _GetStringArg(ai_oRequest, ai_uiIndex, szArgument))
		return M4_ERROR;

	sscanf(szArgument, "%d", &ao_iArgument);

	return M4_SUCCESS;
  //## end ClLauncherForUUSS::_GetIntArg%953831065.body
}

m4return_t ClLauncherForUUSS::_GetIntPtrArg(const ClCommandRequest &ai_oRequest, m4uint32_t ai_uiIndex, intptr_t &ao_iArgument)
{
	//## begin ClLauncherForUUSS::_GetIntArg%953831065.body preserve=yes
	m4pchar_t		szArgument;
	
	if (M4_SUCCESS != _GetStringArg(ai_oRequest, ai_uiIndex, szArgument))
		return M4_ERROR;

	sscanf(szArgument, M4_PRIdPTR, &ao_iArgument);

	return M4_SUCCESS;
	//## end ClLauncherForUUSS::_GetIntArg%953831065.body
}

// Additional Declarations
  //## begin ClLauncherForUUSS%35444E0A03E3.declarations preserve=yes
m4return_t ClLauncherForUUSS::_GetCertificate (ClCommandRequest &aio_oAuthReq )
{
	ClUSSessionTicketInterface	*poTicket;
	ClChannel					*poSessionChannel;
	ClSessionContext			oSessContext;
	m4uint32_t					uiArgs = aio_oAuthReq.GetNumberOfArgs();
	m4return_t					iRet;
	m4uint32_t					uiArgLen = 0,
								uiIndexArg = uiArgs ;
	m4pchar_t					szUserName ;
	m4string_t					strUserName ;
	m4pchar_t					szCertificate = NULL,
								szSessContext = NULL ;
	ClUSSessionInterface		*poSession ;
	m4uint32_t					iSessLen ;

	if (1 != uiArgs)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "EXE4UUSS: Wrong number of args for GetCertificate!!!!");
		return M4_ERROR;
	}



//	1st Argument	USERNAME (NULL if USERPWD == CREDENTIAL)
		if (M4_SUCCESS != _GetStringArg(aio_oAuthReq, --uiIndexArg, szUserName ))
			return M4_ERROR;

	if ( ! ( poSession = (ClUSSessionInterface *) aio_oAuthReq.GetHandle ( ) ) )
	{
		return M4_ERROR ;
	}


	poSession ->GetSessionTicket( poTicket ) ;
	if (NULL == poTicket)
		return M4_ERROR;
	
	poSessionChannel = (ClChannel *)poTicket->GetTicketCode();
	if (NULL == poSessionChannel)
		return M4_ERROR;
		
	ClSesChnExport oSesChnExport;
	if ( M4_ERROR == oSesChnExport.Export( poSessionChannel , oSessContext ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == oSessContext.GetAtribute ( "ID_APP_USER" , strUserName ) )
	{
		return M4_ERROR ;
	}

/*Usamos el contexto de sesión para guardar el nombre de usuario, que lo necesitaremos
para la información de usuario una vez logado con credencial */

	if ( M4_ERROR == oSessContext.SetAtribute ( "N_APP_USER" , szUserName ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == oSessContext.Serialize (szSessContext , iSessLen ) )
	{
		return M4_ERROR ;
	}

	szSessContext = new m4char_t [ iSessLen +1 ] ;

	if ( M4_ERROR == oSessContext.Serialize (szSessContext , ++iSessLen ) )
	{
		return M4_ERROR ;
	}

	iRet = ((ClExeForUUSS *)m_pExecutive)->GetCertificate ((char*)strUserName.c_str() , szCertificate ) ;

	if (M4_SUCCESS <= iRet )
	{
		aio_oAuthReq.SetHandle ((m4handle_t)szCertificate ) ;
	}

	if ( szSessContext )
		uiArgLen = strlen ( szSessContext ) ;

	aio_oAuthReq.AddArg (szSessContext , uiArgLen ) ;

	uiArgLen  = 0 ;
	if ( szCertificate )
		uiArgLen = strlen ( szCertificate ) ;

	aio_oAuthReq.AddArg (szCertificate , uiArgLen ) ;
	aio_oAuthReq.SetResult(iRet);

	return M4_SUCCESS;
}


m4return_t	ClLauncherForUUSS::_CloseAuditory( ClCommandRequest &aio_oAuthReq )
{

	m4return_t				iResult = M4_ERROR ;
	m4uint32_t				uiArgs = 0 ;
	m4uint32_t				iSession = 0 ;
	m4string_t				sUser ;
	ClUSUserInterface		*poUser = NULL ;
	ClUSSessionInterface	*poSession = NULL ;


	uiArgs = aio_oAuthReq.GetNumberOfArgs() ;

	if( uiArgs != 0 )
	{
		SETCODEF( M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run: Wrong number of args for CloseAuditory!!!!" ) ;
		return( M4_ERROR ) ;
	}

	poSession = (ClUSSessionInterface *) aio_oAuthReq.GetHandle() ;

	if( poSession == NULL )
	{
		return( M4_ERROR ) ;
	}

	poUser = poSession->GetUser() ;

	if( poUser != NULL )
	{
		// bg 176316
		// Si hay suplantación el control de concurrencia no se realiza sobre el suplantado, sino sobre el suplantador.
		poUser->GetUserImpersonator( sUser ) ;

		if( sUser == "" )
		{
			poUser->GetUserId( sUser ) ;
		}
	}

	iSession = poSession->GetId() ;

	iResult = ((ClExeForUUSS *)m_pExecutive)->CloseAuditory( sUser.c_str(), iSession ) ;
	aio_oAuthReq.SetResult( iResult ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClLauncherForUUSS::_CleanAuditory( ClCommandRequest &aio_oAuthReq )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	uiArgs = 0 ;


	uiArgs = aio_oAuthReq.GetNumberOfArgs() ;

	if( uiArgs != 0 )
	{
		SETCODEF( M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Run: Wrong number of args for CleanAuditory!!!!" ) ;
		return( M4_ERROR ) ;
	}

	iResult = ((ClExeForUUSS *)m_pExecutive)->CleanAuditory() ;
	aio_oAuthReq.SetResult( iResult ) ;

	return( M4_SUCCESS ) ;
}

// User actions auditory.
m4return_t	ClLauncherForUUSS::_UserAuditoryLogoffAction( ClCommandRequest &aio_oAuthReq )
{
	m4return_t					iRet = M4_ERROR ;
	ClUSSessionInterface		*poSession = NULL ;
	ClUSSessionTicketInterface	*poTicket ;
	ClChannel					*poSessionChannel ;
	m4VariantType				vValue ;
	m4string_t					strSessionID ;
	ClUSUserInterface			*poUser = NULL ;
	m4string_t					sUser = "" ;


	poSession = ( ClUSSessionInterface* ) aio_oAuthReq.GetHandle() ;

	if( poSession == NULL )
	{
		return M4_ERROR ;
	}

	poSession ->GetSessionTicket( poTicket ) ;

	if( NULL == poTicket )
	{
		return M4_ERROR ;
	}
	
	poSessionChannel = ( ClChannel* ) poTicket->GetTicketCode() ;

	if( NULL == poSessionChannel )
	{
		return M4_ERROR ;
	}

	poSession->GetSessionID ( strSessionID ) ;
	poUser = poSession->GetUser() ;

	if( poUser != NULL )
	{
		poUser->GetUserId( sUser ) ;
	}

	iRet = ((ClExeForUUSS*)m_pExecutive)->ExeUserAuditoryLogoffAction( poSessionChannel, sUser.c_str() ) ;

	aio_oAuthReq.SetResult( iRet ) ;


	return M4_SUCCESS ;
}


m4return_t	ClLauncherForUUSS::_UserAuditoryLogonErrorAction( ClCommandRequest &aio_oAuthReq )
{
	m4return_t	iRet = M4_ERROR ;
	m4uint32_t	uiNumArgs ;
	m4pchar_t	pcClient = NULL ;
	m4pchar_t	pcUser = NULL ;
	m4pchar_t	pcClientType = NULL ;

	uiNumArgs = aio_oAuthReq.GetNumberOfArgs() ;


	switch( uiNumArgs )
	{
	case M4LOGON_USER_ARG_NUMBER:

		if( M4_SUCCESS != _GetStringArg( aio_oAuthReq, 11, pcClientType ) )
		{
			return M4_ERROR ;
		}
		if( M4_SUCCESS != _GetStringArg( aio_oAuthReq, 8, pcClient ) )
		{
			return M4_ERROR ;
		}
		if( M4_SUCCESS != _GetStringArg(aio_oAuthReq, 0, pcUser ) )
		{
			return M4_ERROR;
		}

		break ;

	case M4LOGON_JS_ARG_NUMBER:

		if( M4_SUCCESS != _GetStringArg(aio_oAuthReq, 0, pcUser ) )
		{
			return M4_ERROR;
		}

		break ;

	case M4LOGON_CREDENTIAL_ARG_NUMBER :

		if( M4_SUCCESS != _GetStringArg( aio_oAuthReq, 7, pcClientType ) )
		{
			return M4_ERROR ;
		}
		if( M4_SUCCESS != _GetStringArg( aio_oAuthReq, 5, pcClient ) )
		{
			return M4_ERROR ;
		}
		if( M4_SUCCESS != _GetStringArg(aio_oAuthReq, 3, pcUser ) )
		{
			return M4_ERROR;
		}

		break ;

	default :

		return M4_ERROR ;
	}

	iRet = ((ClExeForUUSS*)m_pExecutive)->ExeUserAuditoryLogonErrorAction( pcUser, pcClient, pcClientType ) ;
	
	/* Bug 0162564
	No se debe machacar el resultado de la request
	aio_oAuthReq.SetResult( iRet ) ;
	*/


	return M4_SUCCESS ;
}
  //## end ClLauncherForUUSS%35444E0A03E3.declarations

//## begin module%38D91FFE021A.epilog preserve=yes
//## end module%38D91FFE021A.epilog
