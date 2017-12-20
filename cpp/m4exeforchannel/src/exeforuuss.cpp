//## begin module%3528FAFB01CF.cm preserve=no
//## end module%3528FAFB01CF.cm

//## begin module%3528FAFB01CF.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4exeFactory
//	 File:              ExeForUUSS.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980407
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
//## end module%3528FAFB01CF.cp

//## Module: ExeForUUSS%3528FAFB01CF; Package body
//## Subsystem: M4ExeForChannel::UUSS::src%392278340161
//## Source file: D:\Work\v600\m4exeforchannel\src\exeforuuss.cpp

//## begin module%3528FAFB01CF.additionalIncludes preserve=no
//## end module%3528FAFB01CF.additionalIncludes

//## begin module%3528FAFB01CF.includes preserve=yes
#include <cldefine.hpp>
#include <m4trace.hpp>
#include <cldates.hpp>
#include <m4srvres.hpp>
//	Rose 98 Body Include Dependences
#include <clm4objservice.hpp>
#include <metadatasubsys.hpp>
#include <connects.hpp>
#include <ussession.hpp>
#include <ususer.hpp>
#include <channel.hpp>
#include <access.hpp>
#include <iexecutor.hpp>
#include <dm.hpp>
#include <sessionidgenerator.hpp>
#include <rawsessionid.hpp>
#include <clssinvoker.hpp>
#include <m4actionconstants.hpp>
#include <ussession40.hpp>
#include <m4lang.hpp>
#include "objdir.hpp"
#include "cldatstoiod.hpp"
#include "m4cacertdef.hpp"
#include "certmngr.hpp"
#include "chmngauthent.hpp"
#include "cllgadap.hpp"
#include <sessioncontext.hpp>
#include <clseschnexport.hpp>
#include <m4cryptc.hpp>
#include <m4objglb.hpp>
//## end module%3528FAFB01CF.includes

// executiveforoe
#include <executiveforoe.hpp>
// ChannelUtil
#include <channelutil.hpp>
// ExeForUUSS
#include <exeforuuss.hpp>
// m4credentials
#include <m4credentials.hpp>
#include <licensemanagerinterface.hpp>
#include <licres.hpp>
// Compression Types/Levels CONST values
#include <clientapi.hpp>
#include <m4servenv.hpp>
#include <cloblconfiguration.hpp>



#define	M4CH_DUMMY_B2	"\x2\x3\x8\x1\x9\x6\x5\xF\xD\x8\xF\x8\xB\x4\x7\x7\xF\x6\x4\xE\x4\xE\xE\x5\xB\xE\x6\x7\xF\x3\x7\x2\xB\x6\xC\x4\xD\xB\x6\x7\x8\x3\xE\x8\xA\xB\x7\x9\x6\x6\x7\x9\xB\x6\x4\x8\xE\xF\x3\x8\x8\xC\xA\x0\x4\x2\x6\x8\x4\xC\x4\xD\x9\x2\x8\x4\x8\x8\x6\x8\xF\x1\x1\xF\xC\x1\x7\xE"
#define	M4CH_DUMMY_B3	"\x8\x3\x7\x5\xC\xA\xA\x4\xF\x5\xF\x9\x6\x3\x8\x1\x1\x2\x4\x9\x5\xF\xA\x9\x6\x2\x2\xF\x8\x4\xB\x4\xF\xE\x1\xB\x4\xE\x6\x7\xB\xE\x8\xE\x3\x1\x5\x5\x9\xE\x4\xA\x3\xE\x1\x4\x9\x7\x9\x9\xF\x4\x6\x0\x1\xC\x7\x2\x7\x2\x9\x8\x4\x4\x1\x1\x9\xF\x6\xD\x2\xA\x8\xF\xA\xF\x5\x5"


//## begin module%3528FAFB01CF.declarations preserve=no
//## end module%3528FAFB01CF.declarations

//## begin module%3528FAFB01CF.additionalDeclarations preserve=yes

// class ClExeForUUSSVMReseter

ClExeForUUSSVMResetter::ClExeForUUSSVMResetter(IExecutor* poExecutor) 
: m_poExecutor(poExecutor)
{}

ClExeForUUSSVMResetter::~ClExeForUUSSVMResetter() 
{
	if (m_poExecutor == NULL) 
	{
		return;
	}

	m_poExecutor->Stack.Reset() ;
}

//## end module%3528FAFB01CF.additionalDeclarations


// Class ClExeForUUSS 


ClExeForUUSS::ClExeForUUSS (ClEngine *ai_pEngine)
  //## begin ClExeForUUSS::ClExeForUUSS%895136725.hasinit preserve=no
  //## end ClExeForUUSS::ClExeForUUSS%895136725.hasinit
  //## begin ClExeForUUSS::ClExeForUUSS%895136725.initialization preserve=yes
  : ClExecutiveForOE(ai_pEngine), m_poSessionIDGenerator(NULL)
  //## end ClExeForUUSS::ClExeForUUSS%895136725.initialization
{
  //## begin ClExeForUUSS::ClExeForUUSS%895136725.body preserve=yes
	m_iMaxDumpLevel = 0;
	m_poSessionIDGenerator = new ClRawSessionIDGenerator();

	m_bAuditInit = M4_FALSE ;
	m_sHost = "" ;
	m_iPort = 0 ;
	m_poAuditNode = NULL ;
	m_poAuditAccess = NULL ;
	m_poAuditChannel = NULL ;
  //## end ClExeForUUSS::ClExeForUUSS%895136725.body
}


ClExeForUUSS::~ClExeForUUSS ()
{
  //## begin ClExeForUUSS::~ClExeForUUSS%895136726.body preserve=yes

	// Se destruye el canal de auditoría de sesiones si existe
	if( m_poAuditAccess != NULL )
	{
		m_poAuditAccess->Destroy() ;
		m_poAuditAccess = NULL ;
	}

	if( m_poAuditChannel != NULL )
	{
		m_poAuditChannel->Destroy() ;
		m_poAuditChannel = NULL ;
	}

	m_poAuditNode = NULL ;
	m_sHost = "" ;
	m_iPort = 0 ;

	if (M4_TRUE == m_bInitialized)
	{
		ShutDown();
	}

	if (m_poSessionIDGenerator)
	{
		delete m_poSessionIDGenerator;
		m_poSessionIDGenerator = NULL ;
	}
  //## end ClExeForUUSS::~ClExeForUUSS%895136726.body
}



//## Other Operations (implementation)
m4return_t ClExeForUUSS::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExeForUUSS::StartUp%895136727.body preserve=yes
	m4return_t ret = ClExecutiveForOE::StartUp(ai_pNavigator, ai_pConfig);
		
	if (M4_ERROR == TChannelUtil::SetAdmMode(*m_pChannelManager))
	{
		SETCODEF (M4_ERR_SCH_INITIALIZING, ERRORLOG, "SESSIONCH: Error Enabling Adm mode" );
		ShutDown();
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClExeForUUSS::StartUp%895136727.body
}

m4return_t ClExeForUUSS::Execute (ClImage &aio_pImage)
{
  //## begin ClExeForUUSS::Execute%895136728.body preserve=yes
	if (NULL == m_pEngine)
		return M4_ERROR;

	if (M4_FALSE == m_pEngine->Process(aio_pImage))
	{
		return M4_ERROR;
	}

	return  M4_SUCCESS;
  //## end ClExeForUUSS::Execute%895136728.body
}

m4return_t ClExeForUUSS::Reset ()
{
  //## begin ClExeForUUSS::Reset%895136730.body preserve=yes

	if (m_pChannelManager == NULL) {
		return M4_ERROR;
	}

	ClChannel *poSessionChannel = m_pChannelManager->GetSessionChannel();
	if (poSessionChannel == NULL) {
		return M4_ERROR;
	}

	poSessionChannel->Property.DeleteProperty(M4_SESSION_JSAPI_UUID_PROPERTY);
	poSessionChannel->Property.DeleteProperty(M4_SESSION_WEB_SECRET_KEY_PROPERTY);

	return M4_SUCCESS;
  //## end ClExeForUUSS::Reset%895136730.body
}

m4return_t ClExeForUUSS::Authenticate (m4pchar_t ai_pszUserId, m4pchar_t ai_pszPasswd, const m4bool_t ai_bUse2Way, ClUSUserInterface *&ao_pUser, m4int8_t &ao_iLogonMech, m4pchar_t ao_pcUserIdAlias, m4pchar_t ao_pcImpersonatorUser, m4int8_t ai_iLogonOrigin, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP)
{
  //## begin ClExeForUUSS::Authenticate%896786271.body preserve=yes
	ClChannel		*poUsersChannel;
	ClAccess		*poAccess;
	m4return_t		iResult, iRet ;
	m4VariantType	avArguments[ 6 ], res ;
	m4VariantType	avResults[ 3 ] ;
	m4pchar_t		pcSessionUserId = NULL ;
	
	*ao_pcUserIdAlias = '\0' ;
	*ao_pcImpersonatorUser = '\0' ;

	//	START CONTRACT CHECK
	if ((NULL == ai_pszUserId) || (NULL == ai_pszPasswd))
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error in parameters");
		return M4_ERROR;
	}
	//	END CONTRACT CHECK

	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 0 ].Data.PointerChar = ai_pszUserId ;

	// Password one-way
	avArguments[ 1 ].Type = M4CL_CPP_TYPE_NULL ;

	// Password two-way
	avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 2 ].Data.PointerChar = ai_pszPasswd ;


	m4Trace( cout << "EXE4UUSS: Authenticate +" << endl );

	ao_pUser = NULL;
	m_pChannelManager->CreateChannel(poUsersChannel);
	if (NULL == poUsersChannel)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error creating Channel");
		return M4_ERROR;
	}

	TDestroyer		oUsersChTerminator(poUsersChannel, m_pObjectService->GetpConnProvider());

//	Nuevo en 180
	poUsersChannel->Role_ID.Set( "M4ADM" ) ;
	iRet = poUsersChannel->BuildFromId("SSC_APPUSER");

	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authentication: Error building USERS channel");
		return iRet;
	}

	poAccess=poUsersChannel->CreateAccess();
	if (NULL == poAccess)	//Creamos un acceso por cada canal
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error creating Access");
		return M4_ERROR;
	}

	oUsersChTerminator.SetAccess(poAccess);

	ClNode *poNode=&(poAccess->Node["SSC_APPUSER"]);
	if (NULL == poNode )
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error creating Node");
		return M4_ERROR;
	}

	// eh 175325. Soporte para suplantación de usuarios.

	iResult = _ExecuteItem( poUsersChannel, poAccess, poNode, "USER_AUTHENTICATION", avArguments, 6, NULL, NULL, avResults, 3 ) ;	

	
//	MOD	990618	if ( (M4_SUCCESS != iResult) && (M4ErrLogonExpiredPwd != iResult) && (M4ErrLogonDaysToExpire != iResult) )
	if (M4_SUCCESS > iResult)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG,  "Authentication: Error in credentials" );
		return iResult;
	}

	iRet = poNode->RecordSet.Current.Item["N_APP_USER"].Value.Get(res);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authentication: Error N_APP_USER");
		return iRet;
	}

//	MOD	EMN	990623	m4pchar_t		pUserName, pUserPwd;
	m4pchar_t		pUserName;

	/* Bug 0124303
	Si viene algo no cadena se da el warning y se tomo el id como nombre
	*/
	if( res.Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		pUserName = res.Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, WARNINGLOG, "Authentication: NULL value for N_APP_USER");
		pUserName = ai_pszUserId;
	}
	

	//bg 102349.
	if (avResults[0].Type == M4CL_CPP_TYPE_NUMBER)
	{
		ao_iLogonMech = (m4int8_t) avResults[0].Data.DoubleData;
	}


	// Valores finales para la sesión.
	pcSessionUserId = ai_pszUserId ;

	// Si hay logon con alias.
	if( avResults[1].Type == M4CL_CPP_TYPE_STRING_VAR && avResults[1].Data.PointerChar != NULL )
	{
		m4int_t iLen = strlen( ai_pszUserId ) ;
		if( iLen < M4_MAX_USER_ALIAS )
		{
			pcSessionUserId = avResults[1].Data.PointerChar ;
			strcpy( ao_pcUserIdAlias, ai_pszUserId ) ;
		}
	}

	// Si hay logon con suplantación.
	if( avResults[2].Type == M4CL_CPP_TYPE_STRING_VAR && avResults[2].Data.PointerChar != NULL )
	{
		pcSessionUserId = avResults[2].Data.PointerChar ;

		// y además autenticación mediante alias, dejamos como suplantador el ID real.
		if( avResults[1].Type == M4CL_CPP_TYPE_STRING_VAR && avResults[1].Data.PointerChar != NULL )
		{
			m4int_t iLen = strlen( avResults[1].Data.PointerChar ) ;
			if( iLen < M4_MAX_USER_ID )
			{
				strcpy( ao_pcImpersonatorUser, avResults[1].Data.PointerChar ) ;
			}
		}
		else
		{
			m4int_t iLen = strlen( ai_pszUserId ) ;
			if( iLen < M4_MAX_USER_ID )
			{
				strcpy( ao_pcImpersonatorUser, ai_pszUserId ) ;
			}
		}
	}


	// eh 286131. Autorización IP cliente
	if( ai_pcClientPubIP != NULL && *ai_pcClientPubIP != 0 )
	{
		avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR;
		
		// Authentication user.
		if( *ao_pcImpersonatorUser != '\0' )
		{
			avArguments[ 0 ].Data.PointerChar = ao_pcImpersonatorUser; 
		}
		else
		{
			avArguments[ 0 ].Data.PointerChar = pcSessionUserId; 			
		}
		
		// Client IP.
		avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR;
		avArguments[ 1 ].Data.PointerChar = ai_pcClientPubIP;

		// Client type Id.
		avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR;
		avArguments[ 2 ].Data.PointerChar = ai_pcClientType;

		// Logon mechanism.
		avArguments[ 3 ].Type = avResults[ 0 ].Type;
		avArguments[ 3 ].Data.DoubleData = avResults[ 0 ].Data.DoubleData;

		iRet = _ExecuteItem( poUsersChannel, poAccess, poNode, "USER_AUTHORIZATION", avArguments, 4, NULL, NULL, NULL, 0 );	

		if( M4_SUCCESS > iRet )
		{
			return iRet;
		}
	}


	ao_pUser = new ClUSUser( pcSessionUserId, pUserName, new ClUSUserStats, ao_pcUserIdAlias, ao_pcImpersonatorUser);


	m4Trace( cout << "EXE4UUSS: Authenticate -" << endl );

	return iResult;
  //## end ClExeForUUSS::Authenticate%896786271.body
}

m4return_t ClExeForUUSS::AuthenticateJob (m4pchar_t ai_pszUserId, ClUSUserInterface *&ao_pUser)
{
  //## begin ClExeForUUSS::AuthenticateJob%951849538.body preserve=yes
//	START CONTRACT CHECK
	if (NULL == ai_pszUserId)
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error in parameters");

		return M4_ERROR;
	}
//	END CONTRACT CHECK

	m4return_t		iResult, iRet ;
	m4VariantType	avArguments[ 2 ], res ;


	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 0 ].Data.PointerChar = ai_pszUserId;

	ClChannel		*poUsersChannel;
	ClAccess		*poAccess;

	ao_pUser = NULL;
	m_pChannelManager->CreateChannel(poUsersChannel);
	if (NULL == poUsersChannel)
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error creating Channel");

		return M4_ERROR;
	}

	TDestroyer		oUsersChTerminator(poUsersChannel, m_pObjectService->GetpConnProvider());

	poUsersChannel->Role_ID.Set( "M4ADM" ) ;
	iRet = poUsersChannel->BuildFromId("SSC_APPUSER");

	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authentication: Error building USERS channel");
		return iRet;
	}

	poAccess=poUsersChannel->CreateAccess();
	if (NULL == poAccess)	//Creamos un acceso por cada canal
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error creating Access");
		return M4_ERROR;
	}

	oUsersChTerminator.SetAccess(poAccess);

	ClNode *poNode=&(poAccess->Node["SSC_APPUSER"]);
	if (NULL == poNode )
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error creating Node");
		return M4_ERROR;
	}

	iResult = _ExecuteItem( poUsersChannel, poAccess, poNode, "_AUTHENTICODE_JOB", avArguments, 1, NULL) ;
	if (M4_SUCCESS > iResult)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG,  "Authentication: Error in credentials" );
		return iResult;
	}

	iRet = poNode->RecordSet.Current.Item["N_APP_USER"].Value.Get(res);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authentication: Error N_APP_USER");
		return iRet;
	}


	m4pchar_t		pUserName;

	/* Bug 0124303
	Si viene algo no cadena se da el warning y se tomo el id como nombre
	*/
	if( res.Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		pUserName = res.Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, WARNINGLOG, "Authentication: NULL value for N_APP_USER");
		pUserName = ai_pszUserId;
	}

	ao_pUser = new ClUSUser(ai_pszUserId, pUserName, new ClUSUserStats);

	return M4_SUCCESS;
  //## end ClExeForUUSS::AuthenticateJob%951849538.body
}

m4return_t ClExeForUUSS::ValidateCredential (M4Credentials &ai_poCredential, ClUSUserInterface *&ao_pUser, ClUSSessionInterface *&ao_poSession)
{
  //## begin ClExeForUUSS::ValidateCredential%942249961.body preserve=yes
		ClParamList * poParamList ;
		ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
		m4return_t iRet ;


	ao_pUser = NULL;
		
	poInvoker -> GetParamList ( poParamList  ) ;
	
	
	poParamList ->SetPointer ( "CREDENTIAL" , (void*) &ai_poCredential) ;
	
	if ( M4_SUCCESS == (iRet = poInvoker ->ExecuteAction ( RECOVER_SESSION_ACTION , poParamList ) ) )
	{
		iRet = poParamList -> GetPointer ( "SESSION" , (void*&) ao_poSession) ;
	}
	poInvoker -> SetParamList ( *poParamList  ) ;
	
	if ( iRet == M4_SUCCESS )
	{
	
		ao_pUser = ao_poSession -> GetUser () ;
	}
	
	return iRet ;

  //## end ClExeForUUSS::ValidateCredential%942249961.body
}

m4return_t ClExeForUUSS::CheckRole (ClUSSessionInterface &ai_oSession, m4pchar_t ai_pszRolId, m4bool_t ai_bIdDef)
{
  //## begin ClExeForUUSS::CheckRole%896786272.body preserve=yes
	ClUSUserInterface	*pUser = ai_oSession.GetUser();

//	START CONTRACT CHECK
	if (NULL == pUser)
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Session Information Missing or corrupt");
		return M4_ERROR;
	}
//	END CONTRACT CHECK

	m4return_t				iResult ;
	const m4uint32_t		uiArgs = 3;
	m4VariantType			avArguments[ uiArgs ] ;

	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_pszRolId != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = ai_pszRolId;
	}
	else
	{
		avArguments[ 0 ].Data.PointerChar = "" ;
	}

	avArguments[ 1 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 1 ].Data.DoubleData = ai_bIdDef;

	m4bool_t	bExternal = (NULL != ai_oSession.GetConnection())?M4_TRUE:M4_FALSE;
	avArguments[ 2 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 2 ].Data.DoubleData = bExternal;	// PERSIST ONLY EXTERNAL LOGONS

	m4return_t		iRet;
	m4bool_t		bRet;

	ClChannel		*poSessionCh;
	ClAccess		*poAccess;

	m4Trace( cout << "EXE4UUSS: CheckRole +" << endl );

	ClUSSessionTicketInterface	*poTicket;
	ai_oSession. GetSessionTicket(poTicket);
	poSessionCh = (ClChannel *)poTicket->GetTicketCode();
	if (NULL == poSessionCh)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Session Information Missing or corrupt");
		return M4_ERROR;
	}

	TDestroyer		oSessionChTerminator(poSessionCh, m_pObjectService->GetpConnProvider());
	oSessionChTerminator.SetDestroy(M4_FALSE);

	iRet = poSessionCh->Attach(m_pChannelManager);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Attaching Channel");
		return iRet;
	}


	poAccess=poSessionCh->CreateAccess();
	if (NULL == poAccess)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Error creating Access");
		return M4_ERROR;
	}

	oSessionChTerminator.SetAccess(poAccess);
	
	ClNode *poNode=&(poAccess->Node["ROOT_SESSION"]);
	if (NULL == poNode)
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Error creating Access to Root node");
		return M4_ERROR;
	}

	bRet = poNode->RecordSet.Current.Begin();
	if (M4_TRUE != bRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Error Begin");
		return M4_ERROR;
	}

	ClChannel	*pVMSessionChannel = m_pChannelManager->GetSessionChannel();
	if (NULL == pVMSessionChannel)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Session Information Missing or corrupt");
		return M4_ERROR;
	}

	m_pChannelManager->SetSessionChannel(poSessionCh);
	
	// bg 284582
	m_pChannelManager->Channel.Detach(pVMSessionChannel);

	iResult = _ExecuteItem( poSessionCh, poAccess, poNode, "_CHECK_APP_ROLE", avArguments, uiArgs, NULL) ;
	if (M4_SUCCESS != iResult)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Invalid Role");
		m_pChannelManager->Channel.Attach(pVMSessionChannel);
		m_pChannelManager->SetSessionChannel(pVMSessionChannel);
		return iResult;
	}

	iRet = poSessionCh->AttachRSM();
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: AttachRSM");
		m_pChannelManager->Channel.Attach(pVMSessionChannel);
		m_pChannelManager->SetSessionChannel(pVMSessionChannel);
		return iRet;
	}

	m_pChannelManager->Channel.Attach(pVMSessionChannel);
	m_pChannelManager->SetSessionChannel(pVMSessionChannel);

	if (M4_TRUE == ai_bIdDef)
	{
		iRet = SetDefRole(*pUser, ai_pszRolId);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Setting Default Role");
			return M4_ERROR;
		}

	}

	m4Trace( cout << "EXE4UUSS: CheckRole -" << endl );

	return iResult;
  //## end ClExeForUUSS::CheckRole%896786272.body
}

m4return_t ClExeForUUSS::CreateSession (ClUSUserInterface *&ai_poUser, const M4ClString &ai_strSessionId, ClUSSessionInterface *&ao_pSession, ClConnInstance *ai_poConnection, m4pchar_t ai_pcLanguage, m4pchar_t ai_pcOrganization, m4bool_t ai_bDefRole, m4bool_t ai_bClone, m4pchar_t ai_szSessContext, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pcClientMachine, m4bool_t ai_bAuditSession, m4pchar_t ai_pcUserIdAlias, m4pchar_t ai_pcImpersonatorUser,  m4int8_t ai_iLogonOrigin, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP)
{
  //## begin ClExeForUUSS::CreateSession%897296727.body preserve=yes
	m4return_t		iRet;
	m4return_t		iWarning = M4_SUCCESS;
	m4bool_t		b40 = M4_FALSE ;
	m4uint32_t		iSessionID ;	
	ClChannel		*poSessionChannel , *poOrgSessionChannel;
	m4string_t		strUserID;
	m4string_t		strUserName;
	m4string_t		strSessionID;


	ai_poUser->GetUserId(strUserID);
	ai_poUser->GetName(strUserName);

	// bg 162540. Auditoria acciones usuario
	// Adelantamos la generación de la credencial para insertarla en el canal sesión.
	if ( M4_ERROR == _CreateCredential(strUserID, 0 /* Infinite */, strSessionID) )
	{
		return M4_ERROR ;
	}

	if ( ! ai_bClone )
	{
		iRet = _CreateSessionChannel (*ai_poUser, ai_bDefRole, ai_pcLanguage, ai_pcOrganization , poSessionChannel , ai_szSessContext, ai_iLogonMechanism, ai_pcClientMachine, strSessionID.c_str(), ai_pcUserIdAlias, ai_pcImpersonatorUser, ai_iLogonOrigin, ai_pcSystemInfo, ai_pcClientType, ai_pcClientPubIP) ;
	}
	else
	{
		ClUSSessionTicketInterface	*poTicket;

		if (! ao_pSession )
		{
			return M4_ERROR ;
		}
		
		if ( M4_ERROR ==  ao_pSession -> GetSessionTicket (poTicket) ) 
		{
			return M4_ERROR ;
		}

		poOrgSessionChannel = (ClChannel *)poTicket->GetTicketCode();

		if ( ! poOrgSessionChannel )
		{
			return M4_ERROR ;
		}

		if ( M4_SUCCESS == ( iRet = _CloneSessionChannel ( poOrgSessionChannel, poSessionChannel ) ) )
		{
			ai_poUser = new ClUSUser (strUserID, strUserName , new ClUSUserStats ) ;
		}
	}

	if ( M4_ERROR == iRet )
	{
		return M4_ERROR;
	}

	// copy properties from executive session channel to the user session channel
	if (m_pChannelManager != NULL && m_pChannelManager->GetSessionChannel() != NULL) {
		ClChannel *poExecutiveSessionChannel = m_pChannelManager->GetSessionChannel();
		m4VariantType oProperty;

		// sets private property
		if (poExecutiveSessionChannel->Property.GetPropValue(M4_SESSION_JSAPI_UUID_PROPERTY, oProperty) == M4_SUCCESS) {
			m4return_t iResult = poSessionChannel->Property.SetOrNewProperty(M4_SESSION_JSAPI_UUID_PROPERTY, oProperty, M4_PROP_LN4_STATE);
			if (iResult != M4_SUCCESS)
			{
				m4string_t sError = m4string_t("Authenticate: Error creating property in session object for property ") + M4_SESSION_JSAPI_UUID_PROPERTY;
				SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, sError.c_str());
			}
		}

		// sets public properties
		if (poExecutiveSessionChannel->Property.GetPropValue(M4_SESSION_WEB_SECRET_KEY_PROPERTY, oProperty) == M4_SUCCESS) {
			m4return_t iResult = poSessionChannel->Property.SetOrNewProperty(M4_SESSION_WEB_SECRET_KEY_PROPERTY, oProperty, M4_PROP_LN4_STATE);
			if (iResult != M4_SUCCESS)
			{
				m4string_t sError = m4string_t("Authenticate: Error creating property in session object for property ") + M4_SESSION_WEB_SECRET_KEY_PROPERTY;
				SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, sError.c_str());
			}
		}
	}

	/* jcr bug 0060966
	Si el chequeo del role devuelve warning hay que subirlo hacia arriba.
	*/
	iWarning = iRet ;

	if ( M4_ERROR == _GetIDFromCredential ( strSessionID , iSessionID ) )
	{
		return M4_ERROR ;
	}


	if ( ai_poConnection )
	{
		if ( M4_CONNECTION_ORIENTED_PROTOCOL != ai_poConnection -> GetConnProtocol( ) )
		{
			b40 = M4_TRUE ;
		}
	}

	
	m4uint32_t lCompType  = M4_COMPRESSION_TYPE_NULL;
	m4uint32_t lCompLevel = M4_COMPRESSION_CLASSIC_DEFAULT_LEVEL;

	if (ai_poConnection)
	{
		lCompType  = ai_poConnection->GetCompressionType ( );
		lCompLevel = ai_poConnection->GetCompressionlevel( );
	}

	if ( ! b40 )
	{
		ao_pSession = new ClUSSession( iSessionID , strSessionID,(ClConnBase*) ai_poConnection, ai_poUser, new ClUSSessionStats, new ClUSSessionTicket((m4pchar_t)poSessionChannel), m_pChannelManager->GetEnvironment(), lCompType, lCompLevel, ai_bAuditSession);
	}
	else 
	{
		ao_pSession = new ClUSSession40( iSessionID , strSessionID, ai_poUser, new ClUSSessionStats, new ClUSSessionTicket((m4pchar_t)poSessionChannel), m_pChannelManager->GetEnvironment(), lCompType, lCompLevel, ai_bAuditSession);
	}

	// Auditoría de sesión
	if( ai_bAuditSession == M4_TRUE )
	{
		m4return_t	iResult = M4_ERROR ;
		m4string_t	sUser ;

		// bg 176316
		// Si hay suplantación el control de concurrencia no se realiza sobre el suplantado, sino sobre el suplantador.
		ai_poUser->GetUserImpersonator( sUser ) ;

		if( sUser == "" )
		{
			ai_poUser->GetUserId( sUser ) ;
		}

		// No se comprueba error
		iResult = OpenAuditory( sUser.c_str(), iSessionID ) ;
	}
   
	return iWarning;
  //## end ClExeForUUSS::CreateSession%897296727.body
}




m4return_t ClExeForUUSS::ChangePassword (ClUSSessionInterface &ai_oSession, m4pchar_t ai_pszOldPassword, m4pchar_t ai_pszNewPassword)
{
  //## begin ClExeForUUSS::ChangePassword%897296728.body preserve=yes
	m4int_t				iLength = 0;
	m4pchar_t			pcOpenOldPassword = NULL;
	m4pchar_t			pcOpenNewPassword = NULL;
	m4pchar_t			pcNewPasswordTwoWay = NULL;
	m4char_t			acOldPassword[2 * M4_MAX_LEN_ONE_WAY + 1];
	m4char_t			acNewPassword[2 * M4_MAX_LEN_ONE_WAY + 1];
	m4string_t			strUserID;
	m4string_t			strUserImpersonator;
	ClUSUserInterface	* pUser = NULL;
	m4return_t			iResult, iRet;
	m4VariantType		avArguments[ 3 ];


//	START CONTRACT CHECK
	if (NULL == ai_pszOldPassword || NULL == ai_pszNewPassword)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "ChangePassword: Error in parameters");
		return M4_ERROR;
	}
//	END CONTRACT CHECK

	// Desencriptar la password antigua que viene encriptada twoway.
	iRet = DecryptTwoWay(ai_pszOldPassword, strlen(ai_pszOldPassword) * sizeof(m4char_t), M4CH_DUMMY_B3 + 23, strlen(M4CH_DUMMY_B3 + 23) * sizeof(m4char_t), pcOpenOldPassword, iLength);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "ChangePassword: Error desencripting password");
		return iRet;
	}

	// Desencriptar la password nueva que viene encriptada twoway.
	iRet = DecryptTwoWay(ai_pszNewPassword, strlen(ai_pszNewPassword) * sizeof(m4char_t), M4CH_DUMMY_B3 + 23, strlen(M4CH_DUMMY_B3 + 23) * sizeof(m4char_t), pcOpenNewPassword, iLength);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "ChangePassword: Error desencripting password");
		delete pcOpenOldPassword;
		return iRet;
	}

	// Encriptar estas passwords oneway para pasarlas como argumento de entrada al método
	// _VALIDATE_SERVER.
	
	// Obtener el usuario y añadirlo a la contraseña.
	pUser = ai_oSession.GetUser();
	if (pUser == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "ChangePassword: Error getting user session");
		delete pcOpenOldPassword;
		delete pcOpenNewPassword;
		return M4_ERROR;
	}

	// Obtener usuario.
	pUser->GetUserId(strUserID);
	if (strUserID.empty() == M4_TRUE)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "ChangePassword: Error getting user id");
		delete pcOpenOldPassword;
		delete pcOpenNewPassword;
		return M4_ERROR;
	}

	// Obtener usuario suplantador.
	pUser->GetUserImpersonator(strUserImpersonator);
	if (strUserImpersonator.empty() == M4_FALSE)
	{
		strUserID = strUserImpersonator;
	}

	iRet = M4ConvertPassword(strUserID.c_str(), pcOpenOldPassword, acOldPassword, 2 * M4_MAX_LEN_ONE_WAY);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "ChangePassword: Error encripting password");
		delete pcOpenOldPassword;
		delete pcOpenNewPassword;
		return iRet;
	}
	
	delete pcOpenOldPassword;

	iRet = M4ConvertPassword(strUserID.c_str(), pcOpenNewPassword, acNewPassword, 2 * M4_MAX_LEN_ONE_WAY);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "ChangePassword: Error encripting password");
		delete pcOpenNewPassword;
		return iRet;
	}
	
	// Encriptar two-way la contraseña nueva.
	iResult = EncryptTwoWay( pcOpenNewPassword, strlen( pcOpenNewPassword ) * sizeof( m4char_t ), M4CH_DUMMY_B2 + 23, strlen( M4CH_DUMMY_B2 + 23 ) * sizeof(m4char_t), pcNewPasswordTwoWay, iLength ) ;

	if (iResult != M4_SUCCESS)
	{
		return M4_ERROR ;
	}

	delete pcOpenNewPassword;


	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 0 ].Data.PointerChar = acNewPassword;

	avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 1 ].Data.PointerChar = acOldPassword;

	avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 2 ].Data.PointerChar = pcNewPasswordTwoWay;

	ClUSSessionTicketInterface	*poTicket;
	ClChannel					*poSessionCh;

	ai_oSession.GetSessionTicket(poTicket);
	poSessionCh = (ClChannel *)poTicket->GetTicketCode();

	TDestroyer		oUsersChTerminator(poSessionCh, m_pObjectService->GetpConnProvider());
	oUsersChTerminator.SetDestroy(M4_FALSE);

	iRet = poSessionCh->Attach(m_pChannelManager);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Changing Password: Attaching Channel");
		delete pcNewPasswordTwoWay;
		return iRet;
	}

	ClChannel*	pVMSessionChannel ;
	pVMSessionChannel = m_pChannelManager->GetSessionChannel() ;
	m_pChannelManager->SetSessionChannel( poSessionCh ) ;

	// bg 287006
	m_pChannelManager->Channel.Detach(pVMSessionChannel);

	iResult = _AtomicExecuteItem( "SSC_VALIDATE", "SSC_VALIDATE", "_VALIDATE_SERVER", avArguments, 3, "SCH_SESSION", poSessionCh) ;

	delete pcNewPasswordTwoWay;

	m_pChannelManager->Channel.Attach(pVMSessionChannel);
	m_pChannelManager->SetSessionChannel( pVMSessionChannel ) ;

	if (M4_SUCCESS != iResult)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Changing Password: Cannot Change Pwd");
		return iResult;
	}

	return iResult;
  //## end ClExeForUUSS::ChangePassword%897296728.body
}

m4return_t ClExeForUUSS::SetDefRole (ClUSUserInterface &ai_oUser, m4pchar_t ai_pszRolId)
{
  //## begin ClExeForUUSS::SetDefRole%918564929.body preserve=yes
	ClChannel		*poUsersChannel;
	ClAccess		*poUsersAccess;
	m4return_t		iRet;
	m4bool_t		bRet = M4_FALSE;

	m_pChannelManager->CreateChannel(poUsersChannel);
	if (NULL == poUsersChannel)
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Set Default Role: Error creating Channel");
		return M4_ERROR;
	}
	
	TDestroyer		oUsersChTerminator(poUsersChannel, m_pObjectService->GetpConnProvider());

	iRet = poUsersChannel->BuildFromId("SSC_APPUSER");

	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authentication: Error building USERS channel");
		return iRet;
	}

	poUsersAccess=poUsersChannel->CreateAccess();

	if (NULL == poUsersAccess)	//Creamos un acceso por cada canal
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Set Default Role: Error creating Access");
		return M4_ERROR;
	}

	oUsersChTerminator.SetAccess(poUsersAccess);

	ClNode *poUsersNode=&(poUsersAccess->Node["SSC_APPUSER"]);
	if (NULL == poUsersNode )
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Set Default Role: Error creating Node");
		return M4_ERROR;
	}

	//	APLICAMOS UN FILTRO PARA CARGAR SOLAMENTE EL USUARIO SOLICITADO

	m4char_t		buffer[256] = "FROM &M4RSC_APPUSER A WHERE A.ID_APP_USER = '";
	m4string_t		strUserID;
	ai_oUser.GetUserId(strUserID);
	strcat(buffer, strUserID.c_str());
	strcat(buffer, "'");

	m4VariantType	arg;
	
	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar = buffer;

	iRet = poUsersNode->RecordSet.Current.Item["Dyn_Filter"].Value.Set(arg);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authentication: Error Dyn Filter");
		return iRet;
	}

	iRet = poUsersChannel->Load();
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authentication: Error Load");
		return iRet;
	}

	bRet = poUsersNode->RecordSet.Current.Begin();
	if (M4_TRUE != bRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authentication: Error Begin");
		return iRet;
	}

	m4uint32_t	userCount = poUsersNode->RecordSet.Register.Count();
	if (0 == userCount)		//	ID_APP_USER INVALID
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG,  "Authentication: Username Not Found" );

		return M4_ERROR;
	}
		
	poUsersNode=&(poUsersAccess->Node["SSC_APPUSER"]);

	if (NULL == poUsersNode )
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Set Default Role: Cannot access APP_USER node");
		return M4_ERROR;
	}

	bRet = poUsersNode->RecordSet.Current.Begin();
	if (M4_TRUE != bRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Set Default Role: Begin");
		return iRet;
	}

	arg.Data.PointerChar = ai_pszRolId;
	iRet = poUsersNode->RecordSet.Current.Item["ID_APP_ROLE_DEF"].Value.Set(arg);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Set Default Role: ID_APP_ROLE_DEF");
		return iRet;
	}

	iRet = poUsersChannel->Persist();
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Set Default Role: Persist");
		return iRet;
	}

	m4Trace( cout << ai_pszRolId << " set as Def Role." << endl );

	return M4_SUCCESS;
  //## end ClExeForUUSS::SetDefRole%918564929.body
}

m4return_t ClExeForUUSS::_AtomicExecuteItem (m4pcchar_t ai_pccM4Object, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4pcchar_t ai_pcInstance, ClChannel *ai_pcChannel)
{
  //## begin ClExeForUUSS::_AtomicExecuteItem%926504939.body preserve=yes
	m4return_t			iResult ;
	ClChannel			*poChannel ;
	ClAccess			*poAccess ;
	ClNode				*poNode ;
	m4VariantType		vResult ;

	if( ai_pccM4Object != NULL && *ai_pccM4Object != '0' && strcmpi( ai_pccM4Object, "SCH_SESSION" ) != 0 )
	{
		iResult = m_pChannelManager->CreateChannel( poChannel ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( poChannel == NULL )
		{
			return( M4_ERROR ) ;
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
			return( M4_ERROR ) ;
	}

	poNode = &( poAccess->Node[ ai_pccNode ] ) ;

	if( poNode == NULL )
	{
		poAccess->Destroy() ;
		if( poChannel != NULL )
		{
			poChannel->Destroy() ;
		}
		return( M4_ERROR ) ;
	}

	iResult = _ExecuteItem(poChannel, poAccess, poNode, ai_pccItem, ai_pvArguments, ai_iLength, ai_pcInstance, ai_pcChannel);

	poAccess->Destroy() ;
	if( poChannel != NULL )
	{
		poChannel->Destroy() ;
	}

	return( iResult ) ;
  //## end ClExeForUUSS::_AtomicExecuteItem%926504939.body
}

m4return_t ClExeForUUSS::_SetLicenseValue( ClNode * ai_poNode )
{
	//-------------------------------------------------------------------
	//Server:
	//El server deja el valor en un item del canal session.
	//Aqui damos valor al item del canal sesion donde dejaremos la licencia.
	//Esta licencia se deserializará al cliente, de forma que al cliente
	//le llegará la licencia.

	m4pcchar_t pLicenseEncrypted = NULL;
	ILicenseManager * pLicenseManager = m_pChannelManager->GetpLicenseManager();
	if ((pLicenseManager != NULL) && (ai_poNode != NULL)) {
		m4VariantType vLicenseEncrypted;
		vLicenseEncrypted.Type = M4CL_CPP_TYPE_STRING_VAR ;
		vLicenseEncrypted.Data.PointerChar = NULL;

		vLicenseEncrypted.Data.PointerChar = m_pChannelManager->GetpLicenseManager()->GetpLicenseEncrypted();
		if (vLicenseEncrypted.Data.PointerChar != NULL) {
			ClItem &rItem = ai_poNode->RecordSet.Register.Item;
			ClItemIndex iLicenseIndex = rItem["LICENSE"].GetIndex();
			if (iLicenseIndex == M4DM_EOF_INDEX) {				
				DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
				return( M4_ERROR );
			}
			//Todo parace correcto, establecemos el valor.
			if (rItem.Value.Set(vLicenseEncrypted) != M4_SUCCESS) {
				DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
				return( M4_ERROR );
			}
		}
		//En el logado validamos la expiracion de la licencia.
		if (pLicenseManager->ValidateLicense() == M4_FALSE) {
			DUMP_CHLOG_ERROR( M4_CH_LICENSE_EXPIRED );
			return M4_ERROR;
		}

		return M4_SUCCESS;
	}
	//-------------------------------------------------------------------

	DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
	return( M4_ERROR );
}

m4return_t ClExeForUUSS::_ExecuteItem (ClChannel *ai_poChannel, ClAccess *ai_poAccess, ClNode *ai_poNode, m4pcchar_t ai_pccItem, m4VariantType *ai_pvArguments, m4uint16_t ai_iLength, m4pcchar_t ai_pcInstance, ClChannel *ai_pcChannel, m4VariantType *ai_pvOuts, m4uint16_t ai_iOut)
{
  //## begin ClExeForUUSS::_ExecuteItem%926504940.body preserve=yes
	m4return_t			iResult ;
	m4uint16_t			i ;
	ClChannel			*poSessionChannel ;
	ClAccess			*poSessionAccess ;
	m4VariantType		vResult ;
	IExecutor			*poExecutor ;

	poExecutor = ai_poAccess->GetpExecutor() ;

	ClExeForUUSSVMResetter oVMResetter(poExecutor);

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		iResult = ai_poNode->RecordSet.Item[ ai_pccItem ].StackPush( ai_pvArguments[ i ] ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	if( ai_pcInstance != NULL && *ai_pcInstance != '\0' )
	{
		if( m_pChannelManager == NULL )
		{
			return( M4_ERROR ) ;
		}

		if (NULL == ai_pcChannel)
		{
			poSessionChannel = m_pChannelManager->GetSessionChannel() ;

			if( poSessionChannel == NULL )
			{
				return( M4_ERROR ) ;
			}
		}
		else
		{
			poSessionChannel = ai_pcChannel;
		}

		poSessionAccess = poSessionChannel->CreateAccess() ;

		if( poSessionAccess == NULL )
		{
			return( M4_ERROR ) ;
		}

		iResult = ai_poChannel->Level2.InsertL2Access( ai_pcInstance, poSessionAccess, M4_TRUE ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}
    
	ai_poNode->RecordSet.Item[ ai_pccItem ] ;

	if( ai_poNode->RecordSet.Item.GetIndex() == M4DM_EOF_INDEX)
	{
		return( M4_ERROR ) ;
	}
	
	iResult = ai_poNode->RecordSet.Item[ ai_pccItem ].Call( poExecutor, ai_iLength ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( ai_pcInstance != NULL && *ai_pcInstance != '\0' )
	{
		iResult = ai_poChannel->Level2.EraseL2Instance( ai_pcInstance, M4_FALSE ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	//bg 102349.
	for( i = 0 ; i < ai_iOut ; i++ )
	{		
		iResult = ai_poNode->RecordSet.Item[ ai_pccItem ].StackPop( ai_pvOuts[ i ] ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	iResult = ai_poNode->RecordSet.Item[ ai_pccItem ].StackPop( vResult ) ;

	if( vResult.Type == M4CL_CPP_TYPE_NUMBER )
	{
		iResult = m4return_t( vResult.Data.DoubleData ) ;
	}
	else
	{
		iResult = M4_ERROR ;
	}

	return iResult;
  //## end ClExeForUUSS::_ExecuteItem%926504940.body
}

m4return_t ClExeForUUSS::_CreateCredential (const m4string_t &ai_strUserId, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID)
{
  //## begin ClExeForUUSS::_CreateCredential%942249964.body preserve=yes
	if (ai_strUserId.empty())
		return M4_ERROR; 

	return m_poSessionIDGenerator->CreateSessionID(ai_strUserId, ai_oSecondsFromNow, ao_strSessionID);
  //## end ClExeForUUSS::_CreateCredential%942249964.body
}

m4return_t ClExeForUUSS::_GetUserFromCredential (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername)
{
  //## begin ClExeForUUSS::_GetUserFromCredential%942249965.body preserve=yes
	m4string_t strServerName , strSessId , strHost , strBasePort ;
	if (ai_strSessionID.empty())
		return M4_ERROR; 

	return m_poSessionIDGenerator->ExtractVariables(ai_strSessionID, ao_strUsername , strServerName , strSessId , strHost , strBasePort);
  //## end ClExeForUUSS::_GetUserFromCredential%942249965.body
}

m4return_t ClExeForUUSS::_GetIDFromCredential (const m4string_t &ai_strSessionID, m4uint32_t &ao_iSessionID)
{
  //## begin ClExeForUUSS::_GetIDFromCredential%967533909.body preserve=yes
	m4int_t ret ;
	m4string_t strSessionID , strUserName , strServerName , strHost , strBasePort;


	if (ai_strSessionID.empty())
		return M4_ERROR; 

	if ( M4_ERROR == m_poSessionIDGenerator->ExtractVariables (ai_strSessionID, strUserName , strServerName ,strSessionID, strHost , strBasePort) )
	{
		return M4_ERROR ;
	}


	ret = sscanf ( strSessionID.c_str () , "%d" , & ao_iSessionID ) ;

	return( ret < 1 )?M4_ERROR : M4_SUCCESS ;
  //## end ClExeForUUSS::_GetIDFromCredential%967533909.body
}

m4return_t ClExeForUUSS::_CreateSessionChannel (ClUSUserInterface &ai_oUser, m4bool_t ai_bDefRole, m4pchar_t ai_pcLanguage, m4pchar_t ai_pcOrganization, ClChannel *&ao_poSessionChannel, m4pchar_t ai_szSessContext, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pcClientMachine, m4pcchar_t ai_pccCredential, m4pchar_t ai_pcUserIdAlias, m4pchar_t ai_pcImpersonatorUser, m4int8_t ai_iLogonOrigin, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pchar_t ai_pcClientPubIP)
{
  //## begin ClExeForUUSS::_CreateSessionChannel%972994487.body preserve=yes
	m4string_t		strUserID;
	m4return_t		iRet;
	m4string_t		strSessionID, strUserName, strServerName, strHost, strBasePort ;

	
	
	ao_poSessionChannel = NULL ;

	m_pChannelManager->CreateChannel(ao_poSessionChannel);

	if (NULL == ao_poSessionChannel)
	{

		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Error creating Access");

		return M4_ERROR;
	}

	TDestroyer		oSessionChTerminator(ao_poSessionChannel, m_pObjectService->GetpConnProvider());

	//Dejamos el idioma indefinido.
	m4language_t iLanguage = 0;

	// sets the language property
	if (ai_pcLanguage != NULL)
	{
		iLanguage = atoi(ai_pcLanguage);

		// We need a valid language
		M4ClLangInfo::Instance()->CheckAndSetValidLangId( iLanguage ) ;
		
		ao_poSessionChannel->Meta_Language.Set(iLanguage);
		ao_poSessionChannel->Data_Language.Set(iLanguage);
	}

	// sets the organization property
	if (ai_pcOrganization != NULL)
	{

		// We need a valid organization
//		M4ClLangInfo::Instance()->CheckAndSetValidLangId( iLanguage ) ;
		
		ao_poSessionChannel->Organization.Set(ai_pcOrganization);
	}

	iRet = ao_poSessionChannel ->BuildFromId("SCH_SESSION");

	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Creating SessionCh: building SESSION channel");
		return iRet;
	}

	ai_oUser.GetUserId(strUserID);

	// Auditoria acciones usuario.
	iRet = _GetTokensFromCredential( ai_pccCredential, strUserName, strServerName, strSessionID, strHost, strBasePort ) ;

	//Bug 0095543, 
	//Se añaden dos argumentos mas.

	//bg 102349.
	//Se añade un argumento más (mecanismo de logon).
	//bg 162540. Auditoria acciones usuario.
	//Se añade otro argumento más (credencial).
	//User Id alias.
	if ( M4_ERROR == TChannelUtil::InitializeSession(*m_pChannelManager, *ao_poSessionChannel, strUserID.c_str(), iLanguage, ai_pcOrganization, "", ai_iLogonMechanism, ai_pcClientMachine, strHost.c_str(), strSessionID.c_str(), ai_pcUserIdAlias, ai_pcImpersonatorUser, ai_iLogonOrigin, ai_pcSystemInfo, ai_pcClientType, strBasePort.c_str(), ai_pcClientPubIP) )
		return M4_ERROR;

	oSessionChTerminator.SetDestroy(M4_FALSE);

	ClAccess	*poAccess=ao_poSessionChannel->CreateAccess();
	if (NULL == poAccess)	//Creamos un acceso por cada canal
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Error creating Access");
		return M4_ERROR;
	}

	oSessionChTerminator.SetAccess(poAccess);
	ClNode *poNode=&(poAccess->Node["ROOT_SESSION"]);
	if (NULL == poNode )
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Error creating Node");
		return M4_ERROR;
	}

	ClChannel	*pVMSessionChannel = m_pChannelManager->GetSessionChannel();
	if (NULL == pVMSessionChannel)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Session Information Missing or corrupt");
		return M4_ERROR;
	}

	/* Bug 0225328
	Se quita el canal sesión del sistema para la ejecución y se vuelve a poner despues
	*/
	m_pChannelManager->SetSessionChannel(ao_poSessionChannel);
	m_pChannelManager->Channel.Detach(pVMSessionChannel);

	if (_SetLicenseValue( poNode ) == M4_ERROR) {
		m_pChannelManager->Channel.Attach(pVMSessionChannel);
		m_pChannelManager->SetSessionChannel(pVMSessionChannel);
		return M4_ERROR;
	}

	if (ai_szSessContext || ai_bDefRole)
	{
		m4return_t		iResult ;
		m4VariantType	avArguments[ 2 ] ;

		avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
		avArguments[ 0 ].Data.PointerChar = "" ;

		avArguments[ 1 ].Type = M4CL_CPP_TYPE_NUMBER ;
		avArguments[ 1 ].Data.DoubleData = M4_FALSE;

		iResult = _ExecuteItem( ao_poSessionChannel, poAccess, poNode, "_CHECK_APP_ROLE", avArguments, 2, NULL) ;
		if (M4_SUCCESS != iResult)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: Invalid Role");
			m_pChannelManager->Channel.Attach(pVMSessionChannel);
			m_pChannelManager->SetSessionChannel(pVMSessionChannel);
			return iResult;
		}

		iRet = ao_poSessionChannel ->AttachRSM();
		if (M4_SUCCESS != iRet)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: AttachRSM");
			m_pChannelManager->Channel.Attach(pVMSessionChannel);
			m_pChannelManager->SetSessionChannel(pVMSessionChannel);
			return iRet;
		}

		if (ai_szSessContext == 0) {
			iResult = _ExecuteItem( ao_poSessionChannel, poAccess, poNode, "USR_INF", NULL, 0, NULL) ;
			if (M4_SUCCESS != iResult)
			{
				SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Checking Role: User Inf");
				m_pChannelManager->Channel.Attach(pVMSessionChannel);
				m_pChannelManager->SetSessionChannel(pVMSessionChannel);
				return iResult;
			}
		}
	}

	/* Bug 0210169
	A partir de aquí hay que sincronizar valores con el canal sesión server
	*/
	iRet = poNode->RecordSet.Current.Item["SYNCHRONIZE_SERVER"].Value.Set(1);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Error set SYNCHRONIZE_SERVER");
	}

	m_pChannelManager->Channel.Attach(pVMSessionChannel);
	m_pChannelManager->SetSessionChannel(pVMSessionChannel);

	if ( ai_szSessContext )
	{
		ClUSSessionInterface * poSession = NULL ;

		ClSessionContext oSessContext;

		if ( M4_ERROR == oSessContext.Deserialize (ai_szSessContext ) )
		{
			return M4_ERROR ;
		}
		
		ClSesChnExport oChnExport;
		oChnExport.Import( ao_poSessionChannel , oSessContext ) ;

	}

	return iRet;

  //## end ClExeForUUSS::_CreateSessionChannel%972994487.body
}

m4return_t ClExeForUUSS::_CloneSessionChannel (ClChannel *ai_poOrgSessionChannel, ClChannel *&ao_poDstSessionChannel)
{
  //## begin ClExeForUUSS::_CloneSessionChannel%972994488.body preserve=yes
	M4DataStorage		*poDataStorage = NULL;
	ClChannel			*pVMSessionChannel ;


	//Libero al channel manager de canal sesión para que el clone, me quede como canal sessión del cahnnel manager.	
	if ( pVMSessionChannel = m_pChannelManager->GetSessionChannel() )
	{
		m_pChannelManager->SetSessionChannel(NULL)  ;
	}

	//Atacho el channel manager al canal sesion original
	ai_poOrgSessionChannel -> Attach (m_pChannelManager);


	if (ai_poOrgSessionChannel == NULL || ai_poOrgSessionChannel->IsBuilt() == M4_FALSE)
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "Invalid Original Session Channel. Cloning" ) ;
		m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
		return M4_ERROR;
	}

	poDataStorage = M4DataStorage::GetNewDataStorage();
	if (poDataStorage == NULL)
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "EXEFORUUSS: Error getting DataStorage" ) ;
		m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
		return M4_ERROR;
	}

	// serializes
	{
		ClDataStoreIODriver oDSIOD;
		if (oDSIOD.Init(GEN_IO_MODE_WRITE, poDataStorage) != M4_SUCCESS)
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "EXEFORUUSS: Error Initializing IOD" ) ;
			M4DataStorage::DeleteDataStorage( poDataStorage );
			m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
			return M4_ERROR;
		}

		ClCMObjectDirectory oObjDir(m_pChannelManager);
		oObjDir.SetForceL2RelationSerialize(M4_TRUE);
		ai_poOrgSessionChannel -> SetRecordSerializationMode(M4CL_RECORD_SERIAL_ALL);
		ai_poOrgSessionChannel -> SetItemSerializationMode(M4CL_ITEM_SERIAL_ALL);
		
		if ( oObjDir.IncludeChannel(ai_poOrgSessionChannel,0) == M4_ERROR )
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "EXEFORUUSS: Error Setting Session Channel into IOD" ) ;
			M4DataStorage::DeleteDataStorage( poDataStorage );
			m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
			return M4_ERROR;
		}
		
		if ( oObjDir.IncludeAllAccessAndCreateOID(*ai_poOrgSessionChannel, M4_TRUE) == M4_ERROR )
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "EXEFORUUSS: Error Initializing IOD" ) ;
			M4DataStorage::DeleteDataStorage( poDataStorage );
			m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
			return M4_ERROR;
		}

		oObjDir.SetKeepChannelCsTypeOnRemote(M4_TRUE);
		if ( oObjDir.Serialize(oDSIOD) != M4_SUCCESS)
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "EXEFORUUSS: Error Serializing IOD" ) ;
			M4DataStorage::DeleteDataStorage( poDataStorage );
			m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
			return M4_ERROR;
		}
	}

	// deserializes
	{
		ClDataStoreIODriver oDSIOD;
		if (oDSIOD.Init(GEN_IO_MODE_READ, poDataStorage) != M4_SUCCESS)
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "EXEFORUUSS: Error Initializing IOD" ) ;
			M4DataStorage::DeleteDataStorage( poDataStorage );
			m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
			return M4_ERROR;
		}

		ClCMObjectDirectory oObjDir(m_pChannelManager);
		if (oObjDir.DeSerialize(oDSIOD, NULL, M4_TRUE) != M4_SUCCESS)
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "EXEFORUUSS: Error Deserializing IOD" ) ;
			M4DataStorage::DeleteDataStorage( poDataStorage );
			return M4_ERROR;
			m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
	}

		// keeps the channel alive
		// oObjDir.SetKeepAliveAll(M4_TRUE);
		if ( ! ( ao_poDstSessionChannel = m_pChannelManager -> GetSessionChannel () ) )
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "EXEFORUUSS: Error Getting Cloned SessionChannel from Channel Manager" ) ;
			M4DataStorage::DeleteDataStorage( poDataStorage );
			m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
			return M4_ERROR ;
		}

		ao_poDstSessionChannel ->Detach () ;

	}

	m_pChannelManager->SetSessionChannel(pVMSessionChannel)  ;
	// ends
	M4DataStorage::DeleteDataStorage( poDataStorage );
	return M4_SUCCESS;
  //## end ClExeForUUSS::_CloneSessionChannel%972994488.body
}


// Additional Declarations
  //## begin ClExeForUUSS%355ABEBE02E5.declarations preserve=yes
m4return_t ClExeForUUSS::GetCertificate (m4pchar_t ai_pszUserName , m4pchar_t & ao_szCertificate ) 
{
	m4pchar_t szSessContext;
	if (m_pChannelManager == NULL) {
		return M4_ERROR;
	}

	if (ai_pszUserName == NULL) {
		return M4_ERROR;
	}

	m4uint32_t iCertSize=0, iSessionSize=0;

	ClExecuteItem oItemExecutor(m_pChannelManager);
	ClCertManager oCertManager(&oItemExecutor);

	return oCertManager.GetCertificate( ai_pszUserName, ao_szCertificate, iCertSize, szSessContext, iSessionSize );

}
	
m4return_t ClExeForUUSS::Authenticate (ClUSUserInterface *&ao_poUser, ClUSSessionInterface *&ao_pSession, ClConnInstance *ai_poConnection, m4pchar_t ai_pszCA , m4pchar_t ai_szCertificate , m4uint32_t ai_iCertificateLen , m4pchar_t ai_szSessContext ) 
{
	m4return_t		iRet;
	m4return_t		iWarning = M4_SUCCESS;
	m4bool_t		b40 = M4_FALSE ;
	m4string_t 		strUserID;
	m4string_t		strUserName;
	m4string_t		strSessionID;

	ClSessionContext		oSessContext;

	m4Trace( cout << "EXE4UUSS: Authenticate Cert +" << endl );

	ClExecuteItem oItemExecutor(m_pChannelManager);
	ClCertManager oCertManager(&oItemExecutor);

	if ( M4_ERROR == ( iRet = oCertManager.CheckCertificate( ai_pszCA, strUserID, ai_szCertificate , ai_iCertificateLen  ) ) )
	{
		return M4_ERROR ;
	}


	if ( M4_ERROR == oSessContext.Deserialize ( ai_szSessContext ) )
	{
		// luciag: ¿no sé por qué estaba comentado? 
		m4Trace( cout << "EXE4UUSS: Failed" << endl );
		return M4_ERROR ;
	}
		
	if ( M4_ERROR == oSessContext.GetAtribute ( "N_APP_USER" , strUserName ) )
	{
		strUserName =  strUserID ;
	}
 
	ao_poUser = new ClUSUser( strUserID , strUserName , new ClUSUserStats);

	m4Trace( cout << "EXE4UUSS: Authenticate Cert -" << endl );

	return iRet;
}


// ============================================================================
// Funciones de auditoría de sesión
// ============================================================================

/*
Inicializa las estructuras de auditoría.
Obtiene el identificador de la máquina.
Construye el canal de auditoría de sesiones
*/
m4return_t	ClExeForUUSS::_InitAuditory( void )
{

	m4return_t	iResult = M4_ERROR ;
	m4char_t	szBuffer[ OBL_PROPVALUE_MAX_LEN + 1 ] ;


	if( m_bAuditInit == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}


	// Se obtiene maquina y puerto
	iResult = GetEnviron( "COMMUNICATION_SERVER", LOCAL_HOST_NAME, szBuffer, OBL_PROPVALUE_MAX_LEN ) ;

	if( iResult != M4_SUCCESS || *szBuffer == '\0' )
	{
		// Si no está rellena la propiedad se toma del sistema
		gethostname( szBuffer, OBL_PROPVALUE_MAX_LEN ) ;
	}

	m_sHost = szBuffer ;

	iResult = GetEnviron( "COMMUNICATION_SERVER", BASE_PORT, szBuffer, OBL_PROPVALUE_MAX_LEN ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}
	
	sscanf( szBuffer, "%d", &m_iPort ) ;


	// Se crea el canal
	iResult = m_pChannelManager->CreateChannel( m_poAuditChannel ) ;

	if( iResult != M4_SUCCESS || m_poAuditChannel == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = m_poAuditChannel->BuildFromId( "SSC_SESSION_AUDITING" ) ;

	if( iResult != M4_SUCCESS )
	{
		m_poAuditChannel->Destroy() ;
		m_poAuditChannel = NULL ;
		return( M4_ERROR ) ;
	}

	iResult = m_poAuditChannel->CreateAccess( m_poAuditAccess ) ;

	if( iResult != M4_SUCCESS || m_poAuditAccess == NULL )
	{
		m_poAuditAccess = NULL ;
		m_poAuditChannel->Destroy() ;
		m_poAuditChannel = NULL ;
		return( M4_ERROR ) ;
	}

	m_poAuditNode = &( m_poAuditAccess->Node[ "SSC_SESSION_AUDITING" ] ) ;

	if( m_poAuditNode == NULL )
	{
		m_poAuditAccess->Destroy() ;
		m_poAuditAccess = NULL ;
		m_poAuditChannel->Destroy() ;
		m_poAuditChannel = NULL ;
		return( M4_ERROR ) ;
	}

	m_bAuditInit = M4_TRUE ;

	return( M4_SUCCESS ) ;
}


/*
Borra los registros de la tabla de auditoría asociados a este servidor
Llama al método ClenServer del canal de auditoría de sesiones.
*/
m4return_t	ClExeForUUSS::CleanAuditory( void )
{

	m4return_t		iResult = M4_ERROR ;
	m4VariantType	avArguments[ 2 ] ;


	iResult = _InitAuditory() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 0 ].Data.PointerChar = (m4pchar_t) m_sHost.c_str() ;

	avArguments[ 1 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 1 ].Data.DoubleData = m_iPort ;

	iResult = _ExecuteItem( m_poAuditChannel, m_poAuditAccess, m_poAuditNode, "CLOSE_SERVER", avArguments, 2, NULL, NULL, NULL, 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


/*
Chequea si un usuario puede logarse o no dependiendo del número de seiones abiertas.
Llama al método CheckSession del canal de auditoría de sesiones.
*/
m4return_t ClExeForUUSS::CheckAuditory( m4pcchar_t ai_sUser, m4bool_t ai_bCheckConcurrency, m4bool_t &ao_rbAuditSession )
{

	m4return_t		iResult = M4_ERROR ;
	m4VariantType	avArguments[ 5 ] ;
	m4VariantType	avOuts[ 1 ] ;


	ao_rbAuditSession = M4_FALSE ;


	iResult = _InitAuditory() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_sUser != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = (m4pchar_t) ai_sUser ;
	}
	else
	{
		avArguments[ 0 ].Type = M4CL_CPP_TYPE_NULL ;
	}

	avArguments[ 1 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 1 ].Data.DoubleData = ai_bCheckConcurrency ;

	avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 2 ].Data.PointerChar = (m4pchar_t) m_sHost.c_str() ;

	avArguments[ 3 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 3 ].Data.DoubleData = m_iPort ;

	avArguments[ 4 ].Type = M4CL_CPP_TYPE_NULL ;

	iResult = _ExecuteItem( m_poAuditChannel, m_poAuditAccess, m_poAuditNode, "CHECK_SESSION", avArguments, 5, NULL, NULL, avOuts, 1 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( iResult ) ;
	}

	if( avOuts[ 0 ].Data.DoubleData == M4_TRUE )
	{
		ao_rbAuditSession = M4_TRUE ;
	}
	else
	{
		ao_rbAuditSession = M4_FALSE ;
	}

	return( M4_SUCCESS ) ;
}


/*
Añade un registro a la tabla de auditoría de sesiones.
Llama al método OpenSession del canal de auditoría de sesiones.
*/
m4return_t	ClExeForUUSS::OpenAuditory( m4pcchar_t ai_sUser, m4uint32_t ai_iSession )
{

	m4return_t		iResult = M4_ERROR ;
	m4VariantType	avArguments[ 4 ] ;


	iResult = _InitAuditory() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_sUser != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = (m4pchar_t) ai_sUser ;
	}
	else
	{
		avArguments[ 0 ].Type = M4CL_CPP_TYPE_NULL ;
	}

	avArguments[ 1 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 1 ].Data.DoubleData = ai_iSession ;

	avArguments[ 2 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 2 ].Data.PointerChar = (m4pchar_t) m_sHost.c_str() ;

	avArguments[ 3 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 3 ].Data.DoubleData = m_iPort ;

	iResult = _ExecuteItem( m_poAuditChannel, m_poAuditAccess, m_poAuditNode, "OPEN_SESSION", avArguments, 4, NULL, NULL, NULL, 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


/*
Borra un registro a la tabla de auditoría de sesiones.
Llama al método CloseSession del canal de auditoría de sesiones.
*/
m4return_t	ClExeForUUSS::CloseAuditory( m4pcchar_t ai_sUser, m4uint32_t ai_iSession )
{

	m4return_t		iResult = M4_ERROR ;
	m4VariantType	avArguments[ 2 ] ;


	iResult = _InitAuditory() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;

	if( ai_sUser != NULL )
	{
		avArguments[ 0 ].Data.PointerChar = (m4pchar_t) ai_sUser ;
	}
	else
	{
		avArguments[ 0 ].Type = M4CL_CPP_TYPE_NULL ;
	}

	avArguments[ 1 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 1 ].Data.DoubleData = ai_iSession ;

	iResult = _ExecuteItem( m_poAuditChannel, m_poAuditAccess, m_poAuditNode, "CLOSE_SESSION", avArguments, 2, NULL, NULL, NULL, 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


// User actions auditory.
m4return_t	ClExeForUUSS::ExeUserAuditoryLogoffAction( ClChannel* ai_poSessionChannel, m4pcchar_t ai_sUser )
{

	ClChannel	*pVMSessionChannel = NULL ;
	ClChannel	*poClonedSessionChannel = NULL ;


	if( ai_poSessionChannel == NULL )
	{
		return( M4_ERROR ) ;
	}

	if( ai_poSessionChannel->GetpChannelManager() != NULL || ai_poSessionChannel->Access.Count() > 0 )
	{
		// Bug 0306530 Si el canal sesión está siendo utilizado se hace una copia
		m4return_t iResult = m_pChannelManager->CloneChannel( poClonedSessionChannel, ai_poSessionChannel ) ;

		if( iResult != M4_SUCCESS || poClonedSessionChannel == NULL )
		{
			return( M4_ERROR ) ;
		}

		// Se quita del channelmanager para que esté como en un caso normal
		poClonedSessionChannel->Detach() ;

		ai_poSessionChannel = poClonedSessionChannel ;
	}

	/* Bug 0165968
	Hay que evitar dejar el canal sesión del usuario en el channelmanager del ejecutor
	Para nuestros propósitos no hace falta hacer atach y detach, sólo hay que establecer
	el canal sesión
	Hay que garantizar que no se ejecuta ningún método en el canal sesión.
	*/
	pVMSessionChannel = m_pChannelManager->GetSessionChannel() ;
	m_pChannelManager->SetSessionChannel( ai_poSessionChannel ) ;

	m_pChannelManager->UserActionsAudit.SwitchOn() ;
	m_pChannelManager->UserActionsAudit.AddAction( NULL, NULL, ClUserAction::EV_LOGOFF, ai_sUser, ai_sUser, NULL, M4_FALSE ) ;
	m_pChannelManager->UserActionsAudit.SwitchOff( M4_TRUE ) ;

	// Reestablecemos el canal sesión original.
	m_pChannelManager->SetSessionChannel( pVMSessionChannel ) ;

	if( poClonedSessionChannel != NULL )
	{
		poClonedSessionChannel->Destroy() ;
	}

	return M4_SUCCESS ;
}


m4return_t ClExeForUUSS::ExeUserAuditoryLogonErrorAction( m4pchar_t ai_sUser, m4pchar_t ai_pcClient, m4pchar_t ai_pcClientType )
{
	m4return_t	iRet = M4_ERROR ;

	iRet = m_pChannelManager->UserActionsAudit.PersistLogonError( ai_sUser, ai_pcClient, ai_pcClientType ) ;

	return iRet ;
}


m4return_t ClExeForUUSS::_GetTokensFromCredential( const m4string_t &ai_strCredential, m4string_t &ao_strUserName, m4string_t &ao_strServerName, m4string_t &ao_strSessionID, m4string_t &ao_strHost, m4string_t &ao_strBasePort )
{
	m4int_t iRet ;

	if( ai_strCredential.empty() )
	{
		return M4_ERROR ; 
	}

	iRet = m_poSessionIDGenerator->ExtractVariables( ai_strCredential, ao_strUserName, ao_strServerName, ao_strSessionID, ao_strHost, ao_strBasePort ) ;


	return iRet ;
}

  //## end ClExeForUUSS%355ABEBE02E5.declarations
//## begin module%3528FAFB01CF.epilog preserve=yes
//## end module%3528FAFB01CF.epilog
