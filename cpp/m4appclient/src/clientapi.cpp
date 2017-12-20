//## begin module%34E313FD0164.cm preserve=no
//## end module%34E313FD0164.cm

//## begin module%34E313FD0164.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%34E313FD0164.cp

//## Module: Clientapi%34E313FD0164; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: z:\m4appclient\src\clientapi.cpp

//## begin module%34E313FD0164.additionalIncludes preserve=no
//## end module%34E313FD0164.additionalIncludes

//## begin module%34E313FD0164.includes preserve=yes
#include <m4cryptc.hpp>
#include <commsocketpool.hpp>
// No funciona el FowardReference de Rose

#include <clienttag.hpp>

// No funciona el BodyReference de Rose
#include <m4date.hpp>
//#include <clientsession.hpp>
#include <clientreceiver.hpp>
#include <clientconnection.hpp>
#include <clientconnectionhttp.hpp>
#include <clientservice.hpp>
#ifdef _WINDOWS
// clientconnectionwininet
#include <clientconnectionwininet.hpp>
#endif
//## end module%34E313FD0164.includes

// reqdescriptor
#include <reqdescriptor.hpp>
// clientbasicdef
#include <clientbasicdef.hpp>
// clientapi
#include <clientapi.hpp>
// m4types
#include <m4types.hpp>

#ifdef _WINDOWS
// eh 166983
#include "regedit.hpp"
#include "m4regdef.hpp"
#endif

#ifndef _WINDOWS
#include <netdb.h>
#include <arpa/inet.h>
#endif

class ClCCMonitor;
class ClLogSystem;
class M4DataStorage;
class ClCommunication;



//## begin module%34E313FD0164.declarations preserve=no
//## end module%34E313FD0164.declarations

//## begin module%34E313FD0164.additionalDeclarations preserve=yes
class ClCCMonitor;
#ifndef SOCKET_ERROR
 #define SOCKET_ERROR -1
#endif
//## end module%34E313FD0164.additionalDeclarations


// Class ClClientAPI 

//## begin ClClientAPI::m_pszHostName%34E858D3031B.role preserve=no  protected: static m4pchar_t {1 -> 1VHAN}
m4pchar_t ClClientAPI::m_pszHostName = NULL;
//## end ClClientAPI::m_pszHostName%34E858D3031B.role





//## begin ClClientAPI::m_poComms%3957086A018F.role preserve=no  protected: static ClCommunication { -> RFHAN}
ClCommunication *ClClientAPI::m_poComms = NULL;
//## end ClClientAPI::m_poComms%3957086A018F.role

//## begin ClClientAPI::m_poTSAPPool%3A06D05B0270.role preserve=no  public: static ClCommSocketPoolBasicTSAP { -> RFHAN}
ClCommSocketPoolBasicTSAP *ClClientAPI::m_poTSAPPool = NULL;
//## end ClClientAPI::m_poTSAPPool%3A06D05B0270.role

ClClientAPI::ClClientAPI (m4pchar_t ai_pcHostname, m4int_t ai_iCommsType, m4int32_t ai_iControlPort, m4int32_t ai_nCommWorkers)
  //## begin ClClientAPI::ClClientAPI%889525732.hasinit preserve=no
      : m_pTimeoutFunction(NULL), m_pEventFunction(NULL)
  //## end ClClientAPI::ClClientAPI%889525732.hasinit
  //## begin ClClientAPI::ClClientAPI%889525732.initialization preserve=yes
  //## end ClClientAPI::ClClientAPI%889525732.initialization
{
  //## begin ClClientAPI::ClClientAPI%889525732.body preserve=yes
	m4char_t szHostName [128] ; 
	hostent* pHostInfo;
	char*    pcIPAddress = "";

	m_uiControlTimeout=TIMEOUT_ADMIN_SERV;

	m4return_t iRet;	

	iRet=ClCCMonitor::GetClientMonitor(m_pClientMonitor);  
	if(NULL==m_pClientMonitor)
	{
		m4Trace(cerr << "NULL Monitor"<<endl);
		return;
	}
	if(M4_ERROR==m_pClientMonitor->SetCommsType (ai_iCommsType))
	{
		m4Trace(cerr <<"ERROR starting Communication"<<endl);
	}
	
	if(M4_ADVANCED_COMMS_TYPE==m_pClientMonitor->GetCommsType ())
	{
		m_poComms=new ClCommunication(ai_iControlPort, ai_nCommWorkers);
		if(m_poComms->GetError())
		{
			delete m_poComms;
			m_poComms=NULL;
		}
	}
	else
	{
		m_poComms=NULL;
		m_poTSAPPool=new ClCommSocketPoolBasicTSAP;
	}

	if ( SOCKET_ERROR != gethostname ( szHostName , 128 ) )
	{
		// eh 272310
		pHostInfo = gethostbyname( szHostName ) ;
		if( pHostInfo != NULL )
		{
			pcIPAddress = inet_ntoa( *( in_addr* )pHostInfo->h_addr );
			if( pcIPAddress != NULL )
			{
				ClCCSession::SetClientID ( pcIPAddress );
			}
			else
			{
				ClCCSession::SetClientID( szHostName );
			}
		}
		else
		{
			ClCCSession::SetClientID( szHostName );
		}
	}

#ifdef _WINDOWS
	m_pAppMonitor = NULL;
#endif
  //## end ClClientAPI::ClClientAPI%889525732.body
}


ClClientAPI::~ClClientAPI ()
{
  //## begin ClClientAPI::~ClClientAPI%889525733.body preserve=yes
	if(!m_pszHostName)
		delete [] m_pszHostName;
	m_pszHostName=NULL;

	if(m_pClientMonitor)
	{
		m_pClientMonitor->StopCSC();
		m_pClientMonitor=NULL;
	}

	if(NULL!=m_poComms)
	{
		m_poComms->StopCommunicationLayer();
		delete m_poComms;
		m_poComms=NULL;
	}

#ifdef _WINDOWS
	if(NULL != m_pAppMonitor)
	{
		delete m_pAppMonitor;
		m_pAppMonitor = NULL;
	}
#endif

  //## end ClClientAPI::~ClClientAPI%889525733.body
}



//## Other Operations (implementation)
m4return_t ClClientAPI::EnableRole (m4handle_t ai_hSessionId, m4pchar_t ai_pRoleId, m4bool_t ai_bDefault)
{
  //## begin ClClientAPI::EnableRole%889525735.body preserve=yes
#ifdef TEST_ECHO_PROACTOR
	return M4_SUCCESS;
#endif // TEST_ECHO_PROACTOR
CRONOSTART;

	ClCCSession		*pSession=NULL;
	m4return_t		iRet;

	m4char_t bufferClock[50];
	strcpy(bufferClock,"EnableRole");
	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);

	if ( M4_SUCCESS == iRet)
		iRet=pSession -> EnableRole(ai_pRoleId, ai_bDefault);
 	else
	{
		SETCODE(M4_ERR_ENBROL_INVSESS,ERRORLOG,"Role not enabled\nInvalid session in EnableRole.");	
		strcpy(bufferClock,"EnableRole:Error por Invalid Session");
	}
CRONOSTOP(bufferClock);
	return iRet;
  //## end ClClientAPI::EnableRole%889525735.body
}

m4return_t ClClientAPI::OpenService (m4handle_t ai_hSessionId, m4pchar_t ai_szServiceId, m4handle_t &ao_hContext)
{
  //## begin ClClientAPI::OpenService%889525736.body preserve=yes
	ClCCSession		*pSession=NULL;
	m4return_t		iRet;
	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);

	if ( M4_SUCCESS == iRet)
		iRet=pSession -> AddService(ai_szServiceId, ao_hContext);
 	else
		SETCODEF(M4_ERR_OPENSERV_INVSES,ERRORLOG,"Service %s not open\nInvalid session in OpenService %s.\nNo connection to server.",ai_szServiceId,ai_szServiceId);
	return iRet;
  //## end ClClientAPI::OpenService%889525736.body
}

m4return_t ClClientAPI::OpenService (m4handle_t ai_hSessionId, m4uint32_t ai_ulServiceId, m4handle_t &ao_hContext)
{
  //## begin ClClientAPI::OpenService%898675801.body preserve=yes
	ClCCSession		*pSession=NULL;
	m4return_t		iRet;
	m4char_t	szServiceId [10] ;

	sprintf ( szServiceId , "%d" , ai_ulServiceId ) ;

	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);

	if ( M4_SUCCESS == iRet )
		iRet=pSession -> AddService(szServiceId, ao_hContext);
 	else
		SETCODEF(M4_ERR_OPENSERV_INVSES,ERRORLOG,"Service %s not open\nInvalid session in OpenService %s.",szServiceId,szServiceId);
	return iRet;
  //## end ClClientAPI::OpenService%898675801.body
}

m4return_t ClClientAPI::SyncRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout, m4bool_t ai_bLogInfo, ClLogSystem *ai_poLog, ClTimeoutFunction ai_pTimeoutFunction, ClRequestDescriptor *ao_poReqId)
{
  //## begin ClClientAPI::SyncRequest%889525737.body preserve=yes

	m4return_t		iRet;
	ClCCService *pService=NULL;

//	ao_strReqId ="probando" ;
	iRet=_PrepareRequest(ai_hSessionId,ai_hServiceContext,0,pService,ai_bLogInfo,ai_poLog);
	if(M4_SUCCESS==iRet)
	{
		if(m_pTimeoutFunction)
			iRet=pService -> SyncRequest (*ai_pInDataStore, *ao_pOutDataStore, ao_hRequestId, ai_bLogInfo, ai_poLog , ao_poReqId , ai_uiTimeout,  M4_ID_PDU_REQUEST, M4_FALSE,m_pTimeoutFunction);
		else
			if (ai_pTimeoutFunction)
				iRet=pService -> SyncRequest (*ai_pInDataStore, *ao_pOutDataStore, ao_hRequestId, ai_bLogInfo, ai_poLog , ao_poReqId , ai_uiTimeout,  M4_ID_PDU_REQUEST, M4_FALSE,ai_pTimeoutFunction);
			else
				iRet=pService -> SyncRequest (*ai_pInDataStore, *ao_pOutDataStore, ao_hRequestId, ai_bLogInfo, ai_poLog , ao_poReqId  ,ai_uiTimeout,  M4_ID_PDU_REQUEST, M4_FALSE);
	}
	return iRet;
  //## end ClClientAPI::SyncRequest%889525737.body
}

m4return_t ClClientAPI::AsyncRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo)
{
  //## begin ClClientAPI::AsyncRequest%889525738.body preserve=yes
	ClCCService		*pService=NULL;
	m4return_t iRet;
	ClLogSystem *poLog=NULL;
	iRet=_PrepareRequest(ai_hSessionId,ai_hServiceContext,0,pService,ai_bLogInfo,poLog);
	if(M4_SUCCESS==iRet)
		iRet=pService -> AsyncRequest (*ai_pInDataStore, ao_hRequestId, ai_bLogInfo,  M4_ID_PDU_REQUEST, M4_FALSE);
 	return iRet;
  //## end ClClientAPI::AsyncRequest%889525738.body
}

m4return_t ClClientAPI::ReceiveRequest (m4handle_t ai_hSessionId, m4handle_t ai_hRequestId, M4DataStorage *&ao_pOutDataStore, m4int32_t ai_lTimeOut, m4int16_t ai_lReceiveFlag, ClLogSystem *ai_poLog)
{
  //## begin ClClientAPI::ReceiveRequest%889525739.body preserve=yes
	m4return_t iRet;
	ClCCService		*pService=NULL;
	ClLogSystem *poLog=NULL;
	m4bool_t blog=M4_TRUE;
	iRet=_PrepareRequest(ai_hSessionId,0,ai_hRequestId,pService,blog,poLog);
	if(M4_SUCCESS==iRet)
		iRet=pService -> Receive (ai_hRequestId, *ao_pOutDataStore, ai_lTimeOut, ai_lReceiveFlag , ai_poLog );
	return	iRet;
  //## end ClClientAPI::ReceiveRequest%889525739.body
}

m4return_t ClClientAPI::CancelRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceId, m4handle_t ai_hRequestId, m4int32_t ai_uiTimeout, m4bool_t ai_bSeverity)
{
  //## begin ClClientAPI::CancelRequest%889525740.body preserve=yes
	ClCCService		*pService=NULL; 
	m4return_t		iRet;
	ClLogSystem *poLog=NULL;
	m4bool_t blog=M4_TRUE;
	iRet=_PrepareRequest(ai_hSessionId,ai_hServiceId,0,pService,blog,poLog);
	if(M4_SUCCESS==iRet)
		iRet = pService -> CancelRequest ( ai_hRequestId,ai_uiTimeout,ai_bSeverity) ;
	return iRet ;
  //## end ClClientAPI::CancelRequest%889525740.body
}

m4return_t ClClientAPI::CloseService (m4handle_t ai_hSessionId, m4handle_t ai_hContext)
{
  //## begin ClClientAPI::CloseService%889525741.body preserve=yes
	ClCCSession		*pSession=NULL;
	m4return_t		iRet;
	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);
	if ( M4_SUCCESS != iRet )
	{
		SETCODE(M4_ERR_CLOSESERV_INVSES,ERRORLOG,"Service not closed\nInvalid session in CloseService.");
		return M4_ERROR;
	}
	else
		return  pSession->RemoveService(ai_hContext);
  //## end ClClientAPI::CloseService%889525741.body
}

m4return_t ClClientAPI::DisableRole (m4handle_t ai_hSessionId, m4pchar_t ai_pRoleId)
{
  //## begin ClClientAPI::DisableRole%889525742.body preserve=yes
	ClCCSession		*pSession=NULL;
	m4return_t		iRet;
	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);
	if ( M4_SUCCESS != iRet)
		SETCODEF(M4_ERR_DISABROLE_INVSES,ERRORLOG,"Couldn't disable the role %s.Invalid session in DisableRole.",ai_pRoleId);
	return iRet;
  //## end ClClientAPI::DisableRole%889525742.body
}

m4return_t ClClientAPI::OpenSession (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strUsername, M4ClString &ai_strPasswd, m4bool_t ai_bDefaultRole, m4handle_t &ao_hSessionId, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert)
{
  //## begin ClClientAPI::OpenSession%889525745.body preserve=yes
CRONOSTART;
	if(ai_strUsername.empty())
	{
		m4Trace(cerr<< "Invalid username. The username is empty"<<endl);
		SETCODE(M4_ERR_OPENSES_UNKOWN,ERRORLOG,"Invalid username. The username is empty");
		return M4_ERROR;
	}
	
	m4return_t iRet;
	ClCCConnection *theconnection;
	m4pcchar_t pccNoValue = "SetTheValuePlease" ;

	iRet=_StartComunication(ai_strAddress,ai_uiPort,theconnection,ao_oArgs,ai_bUseSSL, ai_strCACert);

	// bg 156619. Añadimos los tags temporales para indicar que estamos interesados en sus valores
	// en caso de conectamos a un dispatcher.
	ao_oArgs->SetArg( "TMP_CANDIDATE_SERVER_HOST", m4ServerVT( pccNoValue, M4_TRUE ) ) ;
	ao_oArgs->SetArg( "TMP_CANDIDATE_SERVER_PORT", m4ServerVT( pccNoValue, M4_TRUE ) ) ;

	if(iRet!=M4_ERROR)
		iRet=_StartSession(ai_strUsername, (char*)ai_strPasswd.c_str()  , ai_strPasswd.size() , ai_bDefaultRole, ao_hSessionId,theconnection, ao_oArgs);

#ifdef _WINDOWS

	// eh 166983
	m4bool_t	bMultiuser = M4_FALSE ;
	m4char_t    acTempBuffer[ M4CH_MAX_TEMP_BUFFER + 1 ] ;

	memset( acTempBuffer, '\0', M4CH_MAX_TEMP_BUFFER + 1 ) ;
	
	if( GetRegKeyData( M4_NEW_MACHINE_REGEDIT_KEY, "MultiUser", acTempBuffer ) == TRUE )
    {
		if( *acTempBuffer == '1' )
		{
			bMultiuser = M4_TRUE ;
		}
	}

	// -- Application monitor will be launched in case of login success, password is going to caducate or
	// -- is already caducated.
	if( bMultiuser == M4_FALSE && ( M4_SUCCESS == iRet || 20 <= iRet || 1 == iRet ) )
	{

		m4return_t	iMtorRet ;
		m4pchar_t	pcBuffer = new m4char_t[1024] ;
		M4ClString	strServlet ;
		M4ClString	strServer( "" ) ;
		m4uint32_t	uiServerPort = 0 ;
		ClCCSession	*pSession = NULL ;

		// -- Only if http wininet communications.
		if( M4_ERROR != ao_oArgs->GetArg( M4_HTTP_PROP_SERVLET, pcBuffer, 1024 ) )
		{
			strServlet = pcBuffer ;

			// bg 156619
			if( M4_ERROR != ao_oArgs->GetArg( "TMP_CANDIDATE_SERVER_HOST", pcBuffer, 1024 ) )
			{
				if( strcmp( pcBuffer, pccNoValue ) )
				{
					strServer = pcBuffer ;
				}
			}
			
			if( strServer == "" )
			{
				if( M4_ERROR != ao_oArgs->GetArg( M4_HTTP_PROP_SERVER, pcBuffer, 1024 ) )
				{
					strServer = pcBuffer ;
				}
			}
			
			if( strServer != "" )
			{
				if( M4_ERROR != ao_oArgs->GetArg( "TMP_CANDIDATE_SERVER_PORT", pcBuffer, 1024 ) )
				{
					if( strcmp( pcBuffer, pccNoValue ) )
					{
						uiServerPort = atoi( pcBuffer ) ;

						// bg 173228
						if( ai_bUseSSL == M4_TRUE )
						{
							uiServerPort++ ;
						}
					}
				}
				
				if( uiServerPort == 0 )
				{
					if( M4_ERROR != ao_oArgs->GetArg( M4_HTTP_PROP_SERVER_PORT, pcBuffer, 1024 ) )
					{
						uiServerPort = atoi( pcBuffer ) ;
					}
				}

				if( uiServerPort != 0 )
				{
					// -- Get the user session credential.
					iMtorRet = m_pClientMonitor->GetSession( ao_hSessionId, pSession ) ;

					if( iMtorRet == M4_SUCCESS )
					{
						if( NULL == m_pAppMonitor )
						{
							m_pAppMonitor = new ClAppMonitor( ai_strAddress, ai_uiPort, strServer, uiServerPort, strServlet, ai_bUseSSL, pSession->m_strCredential.c_str() ) ;
						}

						iMtorRet = m_pAppMonitor->StartMonitor() ;

						if( M4_ERROR == iMtorRet )
						{
							SETCODEF( M4_ERR_STARTING_MIND_MONITOR, ERRORLOG, "#*s1*#%s#", ClAppMonitor::ms_pccAppMonitorExeName ) ;

							// -- Not returning error, client applicaction will continue starting.
						}
						else
						{
							SETCODEF( M4_DEB_START_MIND_MONITOR_SUCCESS, DEBUGINFOLOG, "#*s1*#%s#", ClAppMonitor::ms_pccAppMonitorExeName ) ;
						}
					}
					else
					{
						SETCODE( M4_ERR_GETTING_USER_SESSION, ERRORLOG, "#*s1*#") ;
					}
				}
			}
		}

		delete pcBuffer;

	}

#endif
	
	ao_oArgs->DelTag( "TMP_CANDIDATE_SERVER_HOST" ) ;
	ao_oArgs->DelTag( "TMP_CANDIDATE_SERVER_PORT" ) ;

CRONOSTOP("OpenSession:");
	return iRet;
  //## end ClClientAPI::OpenSession%889525745.body
}


#define	M4CH_DUMMY_B3	"\x8\x3\x7\x5\xC\xA\xA\x4\xF\x5\xF\x9\x6\x3\x8\x1\x1\x2\x4\x9\x5\xF\xA\x9\x6\x2\x2\xF\x8\x4\xB\x4\xF\xE\x1\xB\x4\xE\x6\x7\xB\xE\x8\xE\x3\x1\x5\x5\x9\xE\x4\xA\x3\xE\x1\x4\x9\x7\x9\x9\xF\x4\x6\x0\x1\xC\x7\x2\x7\x2\x9\x8\x4\x4\x1\x1\x9\xF\x6\xD\x2\xA\x8\xF\xA\xF\x5\x5"


m4return_t ClClientAPI::ChangePassword (m4handle_t ai_hSessionId, M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd)
{
  //## begin ClClientAPI::ChangePassword%892638101.body preserve=yes
	m4return_t		iRet = M4_SUCCESS;
	m4pchar_t		pUserName = NULL;
	m4int_t			iLength = 0;
	m4pchar_t		pcEncryptOldPassword = NULL;
	m4pchar_t		pcEncryptNewPassword = NULL;
	ClCCSession		* pSession;

	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);
	
	if (M4_SUCCESS == iRet)
	{
		// Se encrypta twoway la password antigua.
		iRet = EncryptTwoWay(ai_oOldPasswd.c_str(), strlen(ai_oOldPasswd.c_str()) * sizeof(m4char_t), M4CH_DUMMY_B3 + 23, strlen(M4CH_DUMMY_B3 + 23) * sizeof(m4char_t), pcEncryptOldPassword, iLength);
		
		if (M4_SUCCESS == iRet)
		{
			// Se encrypta twoway la password nueva.
			iRet = EncryptTwoWay(ai_oNewPasswd.c_str(), strlen(ai_oNewPasswd.c_str()) * sizeof(m4char_t), M4CH_DUMMY_B3 + 23, strlen(M4CH_DUMMY_B3 + 23) * sizeof(m4char_t), pcEncryptNewPassword, iLength);
			
			if (M4_SUCCESS == iRet)
			{
				M4ClString strOldPasswdEnc(pcEncryptOldPassword);
				M4ClString strNewPasswdEnc(pcEncryptNewPassword);

				iRet = pSession->ChangePassword(strOldPasswdEnc, strNewPasswdEnc);
			}
			else
			{
				m4Trace(cerr<<"Error encrypting password"<<endl);
				SETCODE(M4_ERR_OPENSES_UNKOWN,ERRORLOG,"Error encrypting password");
			}
		}
		else
		{
			m4Trace(cerr<<"Error encrypting password"<<endl);
			SETCODE(M4_ERR_OPENSES_UNKOWN,ERRORLOG,"Error encrypting password");
		}
	} 	
	else
	{
		SETCODE(M4_ERR_CLOSSES_INVSESS,ERRORLOG,"Couldn't close the session\nInvalid session in CloseSession.");
	}

	delete pcEncryptOldPassword;
	delete pcEncryptNewPassword;

	return iRet;
  //## end ClClientAPI::ChangePassword%892638101.body
}

m4return_t ClClientAPI::CloseSession (m4handle_t ai_hSessionId)
{
  //## begin ClClientAPI::CloseSession%889525746.body preserve=yes

#ifdef  TEST_ECHO_PROACTOR
	return M4_SUCCESS;
#endif //  TEST_ECHO_PROACTOR
	ClCCSession		*pSession=NULL;
	m4return_t		iRet;

	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);

	if ( M4_SUCCESS == iRet)
	{
		m4return_t iRet2 = m_pClientMonitor->RemoveSession(ai_hSessionId);
		if (NULL != (pSession->m_poDebugFile))  
			pSession->m_poDebugFile->close();

		ClCCSession::eSessionStatus	sessionStatus;
		pSession->GetStatus(sessionStatus);
		if (( ClCCSession::S_ACTIVE == sessionStatus) || ( ClCCSession::S_OPEN == sessionStatus))
		{

#ifdef _WINDOWS

			m4bool_t bIsAlive = M4_TRUE ;
			m4return_t iMtorRet = M4_FALSE ;

			// -- Only can have value if http wininet communications.
			if( NULL != m_pAppMonitor )
			{
				// -- Check if monitor is running.
				iMtorRet = m_pAppMonitor->IsAlive( bIsAlive ) ;
					
				if( M4_ERROR == iMtorRet )
				{
					SETCODEF( M4_ERR_CHECKING_MIND_MONITOR_RUNNING, ERRORLOG, "#*s1*#%s#", ClAppMonitor::ms_pccAppMonitorExeName ) ;
					
					// -- Stop monitor because it could be running.
					m4return_t iCloseMtorRet ;
					iCloseMtorRet = m_pAppMonitor->StopMonitor() ;

					if( M4_ERROR == iCloseMtorRet )
					{
						SETCODEF( M4_ERR_STOPPING_MIND_MONITOR, ERRORLOG, "#*s1*#%s#", ClAppMonitor::ms_pccAppMonitorExeName ) ;
					}

				}
				else if( M4_FALSE == bIsAlive )
				{
					SETCODEF( M4_ERR_MIND_MONITOR_STATUS_CLOSED, ERRORLOG, "#*s1*#%s#", ClAppMonitor::ms_pccAppMonitorExeName ) ;
				}

				if( M4_ERROR == iMtorRet || M4_FALSE == bIsAlive )
				{
					// -- If monitor is not running or if we suspect of this, we restart it again.
					iMtorRet = m_pAppMonitor->StartMonitor() ;

					if( M4_ERROR == iMtorRet )
					{
						SETCODEF( M4_ERR_STARTING_MIND_MONITOR, ERRORLOG, "#*s1*#%s#", ClAppMonitor::ms_pccAppMonitorExeName ) ;

						// -- If monitor can not start, we have to close the session manually.
						SETCODE( M4_DEB_SELF_CLOSING_SESSION, DEBUGINFOLOG, "#*s1*#" ) ;

						iRet = pSession->Close();
					}
					else
					{
						// -- Delegate the session closing in monitor application.
						SETCODEF( M4_DEB_START_MIND_MONITOR_SUCCESS, DEBUGINFOLOG, "#*s1*#%s#", ClAppMonitor::ms_pccAppMonitorExeName ) ;
						SETCODE( M4_DEB_DELEGATE_CLOSING_SESSION, DEBUGINFOLOG, "#*s1*#" ) ;
					}

				}
				else
				{
					// -- Delegate the session closing in monitor application.
					SETCODE( M4_DEB_DELEGATE_CLOSING_SESSION, DEBUGINFOLOG, "#*s1*#" ) ;
				}
			}
			else
			{
				iRet= pSession->Close();
			}

#else
			iRet= pSession->Close();
#endif

			if(M4_ERROR == iRet)
			{
#ifndef TEST_CLOSE_TSAP
				m4Trace(cerr <<"Error en Close de Session.Error en DisconectRequest" <<endl);
#else
				delete pSession;
				iRet=M4_SUCCESS;
#endif // TEST_CLOSE_TSAP
			}
			else
			{
				delete pSession;
			}
		}
		else
		{
			SETCODE(M4_ERR_CLOSSES_INVSESS,ERRORLOG,"Couldn't close the session\nInvalid session in CloseSession.");
			iRet=M4_ERROR;
		}
	}


	return iRet;
  //## end ClClientAPI::CloseSession%889525746.body
}

m4return_t ClClientAPI::RemoveRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceId, m4handle_t ai_hRequestId)
{
  //## begin ClClientAPI::RemoveRequest%899719991.body preserve=yes

	ClCCService		*pService=NULL; 

	m4return_t		iRet;
	ClLogSystem *poLog=NULL;
	m4bool_t blog=M4_TRUE;

	iRet=_PrepareRequest(ai_hSessionId,ai_hServiceId,0,pService,blog,poLog);
	if(M4_SUCCESS==iRet)
		iRet = pService -> RemoveRequest ( ai_hRequestId ) ;

	return iRet ;
  //## end ClClientAPI::RemoveRequest%899719991.body
}

m4return_t ClClientAPI::SetTimeoutFunction (ClTimeoutFunction ai_pTimeoutFunction)
{
  //## begin ClClientAPI::SetTimeoutFunction%911845632.body preserve=yes
	m4return_t ret=M4_SUCCESS;
	if(m_pTimeoutFunction)
	{
		m4TraceLevel(2,cerr<<"TimeoutFunction alredy set.\nTimeoutFunction changed.\n");
		ret=M4_WARNING;
	}
	m_pTimeoutFunction=ai_pTimeoutFunction;
	return ret;
  //## end ClClientAPI::SetTimeoutFunction%911845632.body
}

m4return_t ClClientAPI::GetTimeoutFunction (ClTimeoutFunction &ao_pTimeoutFunction)
{
  //## begin ClClientAPI::GetTimeoutFunction %911845633.body preserve=yes
	m4return_t ret=M4_SUCCESS;
	if(!m_pTimeoutFunction)
	{	
		m4Trace(cerr<<"TimeoutFunction=NULL\n");
		ret=M4_WARNING;
	}
	ao_pTimeoutFunction=m_pTimeoutFunction;
	return ret;
  //## end ClClientAPI::GetTimeoutFunction %911845633.body
}

m4return_t ClClientAPI::Ping (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert)
{
  //## begin ClClientAPI::Ping%927272837.body preserve=yes
	CRONOSTART;

	m4return_t iRet;
	ClCCConnection *pConnection;

	iRet=_StartComunication(ai_strAddress,ai_uiPort,pConnection,ao_oArgs,ai_bUseSSL, ai_strCACert);

	if(M4_SUCCESS==iRet)
	{
		M4ClString strUsername("M4ADM");

	
		ClCCUser		*pUser = new ClCCUser(strUsername, (char*)strUsername.c_str() , strUsername.size() );
		ClCCSession		*pSession = new ClCCSession(pConnection, pUser,m_uiControlTimeout,m_pTimeoutFunction);

		ClCCAdminService	*pAdmService;
		ClCCService			*pService;
		if (M4_ERROR == pSession->GetService (ADMIN_SERVICE_ID, pService))
		{
			pService = new ClCCAdminService(ADMIN_SERVICE, pSession);
			m4handle_t	hConnectionService = (m4handle_t) pService;
		}
		
		m4handle_t	hAdmServiceId = (m4handle_t)pService;
		pAdmService = (ClCCAdminService *)pService;

		iRet = pAdmService->ConnectRequest(hAdmServiceId,ao_oArgs,m_uiControlTimeout,m_pTimeoutFunction);
		if ( M4_SUCCESS == iRet )
		{
			iRet = pAdmService->DisconnectRequest(hAdmServiceId,m_uiControlTimeout,m_pTimeoutFunction);
		}
		else
		{
			CRONOSTOP("Opensession:Connect:Error");
			m4Trace(cerr << "Error in ConnectRequest"<<endl);
		}
	}
	else
    {
        m4Trace (cerr<< "Eror connecting to "<<ai_strAddress <<":"<< ai_uiPort<<endl);
    }

	return iRet;
  //## end ClClientAPI::Ping%927272837.body
}

m4return_t ClClientAPI::SetServerEventFunction (ClServerEventFunction ai_ServerEventFunction)
{
  //## begin ClClientAPI::SetServerEventFunction%929691988.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	m_pEventFunction=ai_ServerEventFunction;
	ClCCMonitor	*theMonitor;
	ClCCMonitor::GetClientMonitor(theMonitor);
	if (!theMonitor)
	{
		m4Trace(cerr<< "NULL Monitor"<<endl);
		iRet=M4_ERROR;
	}
	else
		if(!theMonitor->m_poDistributor)
		{
			if(M4_CLASSIC_COMMS_TYPE==m_pClientMonitor->GetCommsType ())
			{
				m4Trace(cerr<< "NULL Distibutor"<<endl);
				iRet=M4_ERROR;
			}
			else
				iRet=M4_SUCCESS;
		}
		else
			iRet=theMonitor->m_poDistributor->SetServerEventFunction(ai_ServerEventFunction);
	return iRet;

  //## end ClClientAPI::SetServerEventFunction%929691988.body
}

m4return_t ClClientAPI::OpenSession (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strCredential, m4handle_t &ao_hSessionId, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert)
{
  //## begin ClClientAPI::OpenSession%942079100.body preserve=yes
CRONOSTART;

	m4return_t iRet;
	ClCCConnection *theconnection;
	iRet=_StartComunication(ai_strAddress,ai_uiPort,theconnection,ao_oArgs,ai_bUseSSL, ai_strCACert);

	M4ClString tempuser;
	if(iRet!=M4_ERROR)
		iRet=_StartSession(tempuser, (char*)ai_strCredential.c_str() ,ai_strCredential.size() , M4_FALSE, ao_hSessionId, theconnection,ao_oArgs);

	CRONOSTOP("OpenSession:");
	return iRet;
  //## end ClClientAPI::OpenSession%942079100.body
}

m4return_t ClClientAPI::Init (m4uint32_t ai_uiControlTimeout)
{
  //## begin ClClientAPI::Init%947519631.body preserve=yes
	m_uiControlTimeout=ai_uiControlTimeout;
	m4int16_t		iHostName = 0;
	m4int16_t		powerOfTwo=1;
	m4int16_t		iterator = 0;

	if ( NULL != m_pszHostName)
	{
		while (16 < iterator && '\0' != m_pszHostName[iterator])
		{
			iHostName += powerOfTwo * (m4int16_t)m_pszHostName[iterator++];
			powerOfTwo *= 2;
		}
	}

	m_pClientMonitor->m_IDSite = iHostName;

	m4return_t iRet=M4_SUCCESS;
//REM JAV 8-7-98 	iRet=m_pClientMonitor->InitCSC(NULL);
	return iRet;
  //## end ClClientAPI::Init%947519631.body
}

m4return_t ClClientAPI::GetProps (m4handle_t ai_hSessionId, ClTagList *ao_cTagMap)
{
  //## begin ClClientAPI::GetProps%947519633.body preserve=yes
	ClCCSession		*pSession=NULL;
	m4return_t		iRet;
	
	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);

	if ( M4_SUCCESS == iRet )
		iRet=pSession->GetProps(ao_cTagMap);
	return iRet;
  //## end ClClientAPI::GetProps%947519633.body
}

m4return_t ClClientAPI::SetProps (m4handle_t ai_hSessionId, ClTagList *ao_cTagMap)
{
  //## begin ClClientAPI::SetProps%947519634.body preserve=yes
	ClCCSession		*pSession;
	m4return_t		iRet=M4_ERROR;
	
	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);
	if ( M4_SUCCESS == iRet )
		iRet=pSession->SetProps(ao_cTagMap);
	return iRet;	
  //## end ClClientAPI::SetProps%947519634.body
}

m4return_t ClClientAPI::SetControlTimeout (m4uint32_t ai_uiControlTimeout)
{
  //## begin ClClientAPI::SetControlTimeout%947519635.body preserve=yes
	m_uiControlTimeout=ai_uiControlTimeout;
	return M4_SUCCESS;
  //## end ClClientAPI::SetControlTimeout%947519635.body
}

m4return_t ClClientAPI::GetControlTimeout (m4uint32_t &ao_uiControlTimeout)
{
  //## begin ClClientAPI::GetControlTimeout%947519636.body preserve=yes
	ao_uiControlTimeout=m_uiControlTimeout;
	return M4_SUCCESS;
  //## end ClClientAPI::GetControlTimeout%947519636.body
}

void ClClientAPI::GetError (m4handle_t ai_hSessionId, m4return_t ai_uiError, M4ClString &ao_strDescription)
{
  //## begin ClClientAPI::GetError%947519637.body preserve=yes
  //## end ClClientAPI::GetError%947519637.body
}

m4return_t ClClientAPI::_StartComunication (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, ClCCConnection * &ao_poConnection, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert)
{
  //## begin ClClientAPI::_StartComunication%947519638.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	m4bool_t bHTTP=M4_FALSE;
	m4uint32_t uiCompressionLevel,uiCompressionType=0;
	M4ClString strServlet;
	M4ClString strServer;
	m4uint32_t uiServerPort;
	if(ai_strAddress.empty())
	{
		m4Trace(cerr<<"Invalid host:empty hostname"<<endl);
		SETCODE(M4_ERR_INITSAP,ERRORLOG,"Invalid host:empty hostname");
		return M4_ERROR;
	}
	if(ao_oArgs)
	{
		m4pchar_t thebuffer=new m4char_t[1024];
		if(M4_ERROR!=ao_oArgs->GetArg(M4_HTTP_PROP_SERVLET,thebuffer,1024))
		{
			bHTTP=M4_TRUE;
			strServlet=thebuffer;
			
			if(M4_ERROR==ao_oArgs->GetArg(M4_HTTP_PROP_SERVER,thebuffer,1024))
			{
			//	m4Trace(cerr<<"Error finding "<<M4_HTTP_PROP_SERVER<<endl);
			//	iRet=M4_ERROR;
				strServer="";
			}
			else
			{
				strServer=thebuffer;
				if(M4_ERROR==ao_oArgs->GetArg(M4_HTTP_PROP_SERVER_PORT,thebuffer,1024))
				{
				//	m4Trace(cerr<<"Error finding "<<M4_HTTP_PROP_SERVER_PORT<<endl);
				//	iRet=M4_ERROR;
					uiServerPort=0;
				}
				else
					uiServerPort=atoi(thebuffer);
			}
		}
		delete thebuffer;
		m4double_t thedouble;
		if(M4_ERROR!=ao_oArgs->GetArg(M4_COMPRESSION_TYPE,thedouble))
		{
			uiCompressionType=(m4uint32_t)thedouble;
			if(M4_ERROR!=ao_oArgs->GetArg(M4_COMPRESSION_LEVEL,thedouble))
			{
				uiCompressionLevel=(m4uint32_t)thedouble;

			}
		}
	}

	if(M4_SUCCESS==iRet)
	{
		if (!bHTTP)
			ao_poConnection = new ClCCConnection(ai_strAddress,ai_uiPort,ai_bUseSSL,ai_strCACert);
		else
		{
#ifdef _WINDOWS
			ao_poConnection = new ClWinInetConnection(ai_strAddress, ai_uiPort, strServer, uiServerPort, strServlet, ai_bUseSSL, ai_strCACert);
#else
			ao_poConnection = new ClCCConnectionHTTP(ai_strAddress,ai_uiPort,strServer,uiServerPort,strServlet,ai_bUseSSL,ai_strCACert);
#endif
		}

		if( !ao_poConnection || ( ao_poConnection->GetError()!= M4_SUCCESS))
		{
			m4Trace(cerr<<"Error Connecting to "<<ai_strAddress.c_str()<< ":" << ai_uiPort<<endl);
			if(ao_poConnection)
				delete ao_poConnection;
			ao_poConnection=NULL;
			iRet=M4_ERROR;
		}
		else
		{
			if(uiCompressionType)
			{
				ao_poConnection->SetCompressionType(uiCompressionType);
				ao_poConnection->SetCompressionLevel(uiCompressionLevel);
			}
		}
	}
	return iRet;
  //## end ClClientAPI::_StartComunication%947519638.body
}

m4return_t ClClientAPI::_StartSession (M4ClString &ai_strUsername, m4pchar_t ai_strPasswd, m4uint32_t ai_iPasswdLen, m4bool_t ai_bDefaultRole, m4handle_t &ao_hSessionId, ClCCConnection *  ai_poConnection, ClTagList *ao_oArgs)
{
  //## begin ClClientAPI::_StartSession%947519639.body preserve=yes
	m4pchar_t pEncryptedPassword;
	m4return_t iRet=M4_SUCCESS;
	m4uint32_t iPasswdLen ;
	m4char_t   acBuffer[ 1024 ] ;
	m4pchar_t  pcBuffer = acBuffer ;



	if(m_pClientMonitor->GetCommsType()==M4_RAW_COMMS_TYPE)
	{
		ao_oArgs->SetTag(M4_PROP_SERVER_NON_CONN_ORIENTED);
	}

	// gets the passwords as they are
	pEncryptedPassword=ai_strPasswd;
	iPasswdLen = ai_iPasswdLen ;

	if(M4_SUCCESS==iRet)
	{
		ClCCUser		*pUser = new ClCCUser(ai_strUsername, pEncryptedPassword , iPasswdLen );
		ClCCSession		*pSession = new ClCCSession(ai_poConnection, pUser,m_uiControlTimeout,m_pTimeoutFunction);
		
		ao_hSessionId = (m4handle_t)pSession;
		m4handle_t oldSessionID=ao_hSessionId ;
		iRet = m_pClientMonitor->AddSession(ao_hSessionId, pSession);
		if (M4_SUCCESS == iRet)
		{
			m4bool_t bConnect=M4_TRUE;
			while(bConnect)
			{
#ifndef TEST_ECHO_PROACTOR
				iRet = pSession -> Open(ao_hSessionId,ao_oArgs,ai_bDefaultRole);
#else
				iRet=M4_SUCCESS;
#endif // TEST_ECHO_PROACTOR
				
				if ( iRet<0)
				{
					ao_hSessionId=0;
					m4return_t iRet2 = m_pClientMonitor->RemoveSession( oldSessionID);
					if (M4_ERROR == iRet2)
					{	m4TraceLevel(2,cerr<<"Couldn't remove connection from monitor."<<endl);}
					else
						delete pSession;
					pSession=NULL;
					bConnect=M4_FALSE;
				}
				else
				{

					if(iRet==M4_RETRY_CONNECT)
					{
						if(ao_oArgs)
						{
							m4pchar_t pcHost=new m4char_t[256];
							m4pchar_t pcPort=new m4char_t[256];
							iRet=ao_oArgs->GetArg("CANDIDATE_SERVER_HOST",pcHost,256);
							if(M4_SUCCESS==iRet)
							{
								ao_oArgs->DelTag("CANDIDATE_SERVER_HOST");

								// bg 156619
								if( M4_ERROR != ao_oArgs->GetArg( "TMP_CANDIDATE_SERVER_HOST", pcBuffer, 1024 ) )
								{
									ao_oArgs->SetArg( "TMP_CANDIDATE_SERVER_HOST", pcHost ) ;
								}

								m4int_t iPort;
								
								iRet=ao_oArgs->GetArg("CANDIDATE_SERVER_PORT",pcPort,256);
								if(M4_SUCCESS==iRet)
								{
									iPort=atoi(pcPort);
									ao_oArgs->DelTag("CANDIDATE_SERVER_PORT");

									// bg 156619
									if( M4_ERROR != ao_oArgs->GetArg( "TMP_CANDIDATE_SERVER_PORT", pcBuffer, 1024 ) )
									{
										ao_oArgs->SetArg( "TMP_CANDIDATE_SERVER_PORT", pcPort ) ;
									}

									iRet=ai_poConnection->Reconnect(pcHost,iPort);
									if(M4_ERROR==iRet)
										bConnect=M4_FALSE;
									m4Trace(cout << "Reconnection to "<<pcHost<<":"<<iPort<<endl)
									m4DebugExecute(if(M4_ERROR==iRet) cerr<< "Error reconecting to "<< pcHost<< " port "<< iPort<<endl);
								}
								else
									m4Trace(cerr<< "Error, couldn't find M4_PROP_SERVER_PORT"<<endl);
							}
							else
								m4Trace(cerr<< "Error, couldn't find M4_PROP_SERVER_IP"<<endl);
							delete pcHost;
							delete pcPort;
						}
						else
						{
							iRet=M4_ERROR;
							m4Trace(cerr<<"NULL ClTagList.Couldn't Reconnect"<<endl);
						}
					}
					else
					{
						bConnect=M4_FALSE;
						m4DebugExecute(if(iRet!=M4_SUCCESS) cerr<<"open returns "<<(m4int_t)iRet<<endl);
						ai_poConnection->SetServerEventFunction(m_pEventFunction);
					}
				}
			}

			if((ao_oArgs) && pSession)
			{
				pSession -> SetProps(ao_oArgs);
				pSession -> GetProps(ao_oArgs);
			}
		}
		else
		{
			SETCODE(M4_ERR_OPENSES_ADDSESS,ERRORLOG,"OpenSession failed adding session in the monitor.");
			delete pSession;
		}
	}

	return iRet;
  //## end ClClientAPI::_StartSession%947519639.body
}

m4return_t ClClientAPI::_PrepareRequest (m4handle_t ai_hSessionId, m4handle_t ai_hServiceContext, m4handle_t ai_hRequestID, ClCCService * &ao_pService, m4bool_t &ai_bLogInfo, ClLogSystem * &ai_poLog)
{
  //## begin ClClientAPI::_PrepareRequest%948190724.body preserve=yes
//	if (GET_REMOTE_LOG())
//	Se ha activado por defecto 10-3-99
	if(M4_TRUE)
	{
		if(!ai_poLog)
			ai_poLog=GETPTHREADLOGSYSTEM();
		ai_bLogInfo=M4_TRUE;
	}
	else
	{
		m4TraceLevel(2,cerr <<"RemoteLog=Off" <<endl);
	}

		m4return_t		iRet;

#ifdef  TEST_ECHO_PROACTOR
	iRet = m_pClientMonitor->GetService(ai_hServiceContext, ao_pService);
	return iRet;
#endif  // TEST_ECHO_PROACTOR
	
	ClCCSession		*pSession=NULL;
	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);
	if ( M4_SUCCESS == iRet)
	{
		ClCCSession::eSessionStatus eSessionStatus;
		pSession->GetStatus(eSessionStatus);
		if(eSessionStatus==ClCCSession::S_CLOSED)
		{
			SETCODE(M4_ERR_SYNCREQ_INVSESS,ERRORLOG,"Couldn't realize synchronous request\nInvalid Session in SyncRequest.\nNo connection to server.");
			m4Trace(cerr << "Session Closed" <<endl);
			iRet=M4_ERROR;
		}
		else
		{
			if(ai_hServiceContext)
				iRet = m_pClientMonitor->GetService(ai_hServiceContext, ao_pService);
			else
			{
				ClCCRequest *pRequest=NULL;
				iRet = m_pClientMonitor->GetRequest(ai_hRequestID,pRequest);
				if(M4_SUCCESS==iRet)
					iRet=pRequest->GetService(ao_pService);
			}
		}
	}
 	else
	{
		m4Trace(cout << "Invalid session in SyncRequest." <<endl);
		SETCODE(M4_ERR_SYNCREQ_INVSESS,ERRORLOG,"Couldn't realize synchronous request\nInvalid Session in SyncRequest.\nNo connection to server.");
	}

	return iRet;

  //## end ClClientAPI::_PrepareRequest%948190724.body
}

ClCommunication * ClClientAPI::GetComms ()
{
  //## begin ClClientAPI::GetComms%962005377.body preserve=yes
	return m_poComms;
  //## end ClClientAPI::GetComms%962005377.body
}

m4return_t ClClientAPI::SetCommsType (m4int_t ai_iCommsType)
{
  //## begin ClClientAPI::SetCommsType%972486058.body preserve=yes
	if(m_pClientMonitor==NULL)
		return M4_ERROR;
	else
		return m_pClientMonitor->SetCommsType(ai_iCommsType);
  //## end ClClientAPI::SetCommsType%972486058.body
}

m4return_t ClClientAPI::OpenSession (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, M4ClString &ai_strUsername, M4ClString &ai_strPasswd, m4bool_t ai_bDefaultRole, m4handle_t &ao_hSessionId, m4pchar_t &ao_strM4X509Cert, m4uint32_t &aoi_iM4X509CertBufLeN, ClTagList *ao_oArgs, m4bool_t  ai_bUseSSL, M4ClString ai_strCACert)
{
  //## begin ClClientAPI::OpenSession%1023207538.body preserve=yes
CRONOSTART;
	if(ai_strUsername.empty())
	{
		m4Trace(cerr<< "Invalid username. The username is empty"<<endl);
		SETCODE(M4_ERR_OPENSES_UNKOWN,ERRORLOG,"Invalid username. The username is empty");
		return M4_ERROR;
	}
	m4return_t iRet;
	ClCCConnection *theconnection;
	iRet=_StartComunication(ai_strAddress,ai_uiPort,theconnection,ao_oArgs,ai_bUseSSL, ai_strCACert);

	if(iRet!=M4_ERROR)
		iRet=_StartSession(ai_strUsername, (char*) ai_strPasswd.c_str() , ai_strPasswd.size(), ai_bDefaultRole, ao_hSessionId,theconnection, ao_oArgs);

CRONOSTOP("OpenSession:");
	return iRet;
  //## end ClClientAPI::OpenSession%1023207538.body
}

m4return_t ClClientAPI::OpenSession (M4ClString &ai_strAddress, m4uint32_t ai_uiPort, m4pchar_t ai_strX509Cert, m4uint32_t ai_iX509CertLen, m4handle_t &ao_hSessionId, m4pchar_t &ao_strOutSessionContext, m4uint32_t ai_iOutSessContextBufLen, m4pchar_t ai_strSessionContext, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, M4ClString ai_strCACert)
{
  //## begin ClClientAPI::OpenSession%1023207539.body preserve=yes
CRONOSTART;

	m4return_t iRet;
	ClCCConnection *theconnection;
	iRet=_StartComunication(ai_strAddress,ai_uiPort,theconnection,ao_oArgs,ai_bUseSSL, ai_strCACert);

	M4ClString tempuser;
	if(iRet!=M4_ERROR)
//		iRet=_StartSession(tempuser, (char*) ai_strX509Cert ,ai_iX509CertLen , M4_FALSE, ao_hSessionId, theconnection,ao_oArgs);
		iRet=_StartSession ((m4uchar_t *) ai_strX509Cert , (m4uint32_t) ai_iX509CertLen , (m4uchar_t *) ai_strSessionContext ,M4_TRUE , ao_hSessionId, theconnection,ao_oArgs ) ;

	CRONOSTOP("OpenSession:");
	return iRet;
  //## end ClClientAPI::OpenSession%1023207539.body
}

m4return_t ClClientAPI::GetSessContext (m4handle_t ai_hSessionId, m4pchar_t &  ao_strX509Cert, m4uint32_t &  aoi_iX509CertBufLen, m4pchar_t &  ao_strSessionContext, m4uint32_t &  aoi_iSessContextBufLen)
{
  //## begin ClClientAPI::GetSessContext%1023207540.body preserve=yes
	ClCCSession		*pSession;
	m4return_t		iRet;
	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);

	if ( iRet == M4_ERROR )
		return M4_ERROR ;

	return pSession -> GetSessContext (ao_strX509Cert, aoi_iX509CertBufLen, ao_strSessionContext, aoi_iSessContextBufLen) ;

//	return M4_ERROR ;
  //## end ClClientAPI::GetSessContext%1023207540.body
}

m4return_t ClClientAPI::_StartSession (m4uchar_t *ai_szCertificate, m4uint32_t ai_CertificateLen, m4uchar_t *ai_szSessContext, m4bool_t ai_bDefaultRole, m4handle_t &ao_hSessionId, ClCCConnection *ai_poConnection, ClTagList *ao_oArgs)
{
  //## begin ClClientAPI::_StartSession%1105443234.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	M4ClString nulo = "" ;



	if(m_pClientMonitor->GetCommsType()==M4_RAW_COMMS_TYPE)
	{
		ao_oArgs->SetTag(M4_PROP_SERVER_NON_CONN_ORIENTED);
	}
	

	ClCCUser		*pUser = new ClCCUser(nulo , "" , 0 );
	ClCCSession		*pSession = new ClCCSession(ai_poConnection, pUser,m_uiControlTimeout,m_pTimeoutFunction);
		
	ao_hSessionId = (m4handle_t)pSession;
	m4handle_t oldSessionID=ao_hSessionId ;
	iRet = m_pClientMonitor->AddSession(ao_hSessionId, pSession);
	if (M4_SUCCESS == iRet)
	{
		m4bool_t bConnect=M4_TRUE;
		while(bConnect)
		{
#ifndef TEST_ECHO_PROACTOR
			iRet = pSession -> Open(ai_szCertificate, ai_CertificateLen , ai_szSessContext, ao_hSessionId,ao_oArgs,ai_bDefaultRole);
#else
			iRet=M4_SUCCESS;
#endif // TEST_ECHO_PROACTOR				
			if ( iRet<0)
			{
				ao_hSessionId=0;
				m4return_t iRet2 = m_pClientMonitor->RemoveSession( oldSessionID);
				if (M4_ERROR == iRet2)
				{	m4TraceLevel(2,cerr<<"Couldn't remove connection from monitor."<<endl);}
				else
					delete pSession;
				pSession=NULL;
				bConnect=M4_FALSE;
			}
			else
			{
				if(iRet==M4_RETRY_CONNECT)
				{
					if(ao_oArgs)
					{
						m4pchar_t pcHost=new m4char_t[256];
						m4pchar_t pcPort=new m4char_t[256];
						iRet=ao_oArgs->GetArg("CANDIDATE_SERVER_HOST",pcHost,256);
						if(M4_SUCCESS==iRet)
						{
							ao_oArgs->DelTag("CANDIDATE_SERVER_HOST");
							m4int_t iPort;
							
							iRet=ao_oArgs->GetArg("CANDIDATE_SERVER_PORT",pcPort,256);
							if(M4_SUCCESS==iRet)
							{
								iPort=atoi(pcPort);
								ao_oArgs->DelTag("CANDIDATE_SERVER_PORT");
								iRet=ai_poConnection->Reconnect(pcHost,iPort);
								if(M4_ERROR==iRet)
									bConnect=M4_FALSE;
								m4Trace(cout << "Reconnection to "<<pcHost<<":"<<iPort<<endl)
								m4DebugExecute(if(M4_ERROR==iRet) cerr<< "Error reconecting to "<< pcHost<< " port "<< iPort<<endl);
							}
							else
								m4Trace(cerr<< "Error, couldn't find M4_PROP_SERVER_PORT"<<endl);
						}
						else
							m4Trace(cerr<< "Error, couldn't find M4_PROP_SERVER_IP"<<endl);
						delete pcHost;
						delete pcPort;
					}
					else
					{
						iRet=M4_ERROR;
						m4Trace(cerr<<"NULL ClTagList.Couldn't Reconnect"<<endl);
					}
				}
				else
				{
					bConnect=M4_FALSE;
					m4DebugExecute(if(iRet!=M4_SUCCESS) cerr<<"open returns "<<(m4int_t)iRet<<endl);
					ai_poConnection->SetServerEventFunction(m_pEventFunction);
				}
			}
		}

		if((ao_oArgs) && pSession)
		{
			pSession -> SetProps(ao_oArgs);
			pSession -> GetProps(ao_oArgs);
		}
	}
	else
	{
		SETCODE(M4_ERR_OPENSES_ADDSESS,ERRORLOG,"OpenSession failed adding session in the monitor.");
		delete pSession;
	}

return iRet;
  //## end ClClientAPI::_StartSession%1105443234.body
}

m4return_t ClClientAPI::ConnectRequest (M4ClString &aio_strHostName, m4uint32_t &aio_uiPort, ClTagList *ao_oArgs, m4bool_t ai_bUseSSL, const M4ClString &ai_strCACert)
{
  //## begin ClClientAPI::ConnectRequest%1105443233.body preserve=yes
	m4return_t iRet = M4_SUCCESS;
	ClCCConnection * poConnection = NULL;
	iRet = _StartComunication(aio_strHostName, aio_uiPort, poConnection, ao_oArgs, ai_bUseSSL, ai_strCACert);
	if (iRet != M4_SUCCESS)
	{
		m4Trace(cerr << "Error  executing _StartComunication()."<<endl);
		return M4_ERROR;
	}

	ClCCSession	* pSession = new ClCCSession(poConnection, NULL, m_uiControlTimeout, m_pTimeoutFunction);
	m4handle_t hSessionId = (m4handle_t) pSession;
	iRet = pSession->ConnectRequest(hSessionId, ao_oArgs);
	if (iRet != M4_SUCCESS)
	{
		m4Trace(cerr << "ConnectRequest error."<< endl);
		delete pSession;
		return M4_ERROR;
	}
	
	// Get host name and port from argument list.
	m4pchar_t pcHostName = new m4char_t[256];
	m4pchar_t pcPort = new m4char_t[256];
	if (ao_oArgs)
	{
		if (ao_oArgs->GetArg("CANDIDATE_SERVER_HOST", pcHostName, 256) == M4_SUCCESS)
		{
			aio_strHostName = pcHostName;
		}

		if (ao_oArgs->GetArg("CANDIDATE_SERVER_PORT", pcPort, 256) == M4_SUCCESS)
		{
			aio_uiPort = atoi(pcPort);
		}
	}

	delete pcHostName;
	delete pcPort;
	delete pSession;
		
	return M4_SUCCESS;
  //## end ClClientAPI::ConnectRequest%1105443233.body
}


m4return_t ClClientAPI::SetCommsLanguage( m4language_t ai_iLang )
{
	m4return_t iRet = M4_ERROR ;
	
	// Communicationes http tienen este atributo nulo
	if( m_poComms != NULL )
	{
		iRet = m_poComms->SetCommsLanguage( ai_iLang ) ;
	}

	return iRet ;
}

void ClClientAPI::NotifyVMExecution(m4handle_t ai_hSessionId, m4bool_t ai_bIsStart)
{
	ClCCSession		*pSession=NULL;
	m4return_t		iRet;

	iRet = m_pClientMonitor->GetSession(ai_hSessionId, pSession);

	if ( M4_SUCCESS == iRet)
	{
		pSession->NotifyVMExecution(ai_bIsStart);
	}
}

// Additional Declarations
  //## begin ClClientAPI%35050F9600A6.declarations preserve=yes
  //## end ClClientAPI%35050F9600A6.declarations

//## begin module%34E313FD0164.epilog preserve=yes
//## end module%34E313FD0164.epilog
