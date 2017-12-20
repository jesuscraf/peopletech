//## begin module%342013820134.cm preserve=no
//## end module%342013820134.cm

//## begin module%342013820134.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			ConnectionManager
//	 File:				SessManager.cpp
//	 Project:           M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971003
//	 Language:			C++
//	 Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%342013820134.cp

//## Module: SessManager%342013820134; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\sessmanager.cpp

//## begin module%342013820134.additionalIncludes preserve=no
//## end module%342013820134.additionalIncludes

//## begin module%342013820134.includes preserve=yes
#include <m4servenv.hpp>
#include <m4srvres.hpp>
#include <m4trace.hpp>
#include <m4props.hpp>
#include <subsystemids.hpp>
//	Rose 98 Forward Decl Includes
#include <cllocalmonitor.hpp>
#include <clienttag.hpp>
#include <request.hpp>
#include <connects.hpp>
#include <connects40.hpp>
#include <controlpdus.hpp>
#include <ussession.hpp>
#include <ususer.hpp>
#include <userss.hpp>
#include <commandrequest.hpp>
#include <boomrequest.hpp>
#include <clssinvoker.hpp>
#include <m4actionconstants.hpp>
#include <m4srvtrace.h>
#include <m4cryptc.hpp>
#include <blocksynchronization.hpp>
#include <syncro.hpp>
#include <statistics.hpp>
#include <csutil.hpp>
#include <fifoq.hpp>
#include <usservice.hpp>
#include <sessmanager.hpp>
#include "m4buildversion.hpp"

extern int	M4IsUnicode( void ) ;


//## begin module%342013820134.declarations preserve=no
//## end module%342013820134.declarations

//## begin module%342013820134.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID	"COMSRV"

#define  M4_ERR_UUSS_DENY		M4_SRV_UUSS_ERROR_BASE + 0x000B
//## end module%342013820134.additionalDeclarations


// Class ClCSSessManager 



//## begin ClCSSessManager::s_oPendingReqs%3552D7A303C3.role preserve=no  protected: static ClRequestRegistry {0..n -> 1VHAN}
ClRequestRegistry ClCSSessManager::s_oPendingReqs;
//## end ClCSSessManager::s_oPendingReqs%3552D7A303C3.role



ClCSSessManager::ClCSSessManager (ClLocalMonitor* ai_pMonitor, ClControlQ *ai_pControlQ, ClStatistic *ai_pStats)
  //## begin ClCSSessManager::ClCSSessManager%874517660.hasinit preserve=no
  //## end ClCSSessManager::ClCSSessManager%874517660.hasinit
  //## begin ClCSSessManager::ClCSSessManager%874517660.initialization preserve=yes
  :	ClActiveClass(ai_pStats), m_pControlQ(ai_pControlQ)
  //## end ClCSSessManager::ClCSSessManager%874517660.initialization
{
  //## begin ClCSSessManager::ClCSSessManager%874517660.body preserve=yes
	m_pServiceAPI = ClUSServiceAPI::Instance();
	m_iState = SessManagerActive ;
  //## end ClCSSessManager::ClCSSessManager%874517660.body
}


ClCSSessManager::~ClCSSessManager ()
{
  //## begin ClCSSessManager::~ClCSSessManager%874517661.body preserve=yes
  //## end ClCSSessManager::~ClCSSessManager%874517661.body
}



//## Other Operations (implementation)
void ClCSSessManager::Run (void )
{
  //## begin ClCSSessManager::Run%875868001.body preserve=yes
	m4string_t				szSessionID ;
	ClControlItem			item;
	ClLocalMonitor			*pLocalMonitor = ClLocalMonitor::Instance();
	m4return_t				error;
	m4bool_t				reply;
	ClRequest				* poRequest;
	ClConnInstance			* poConnection ;
	ClSessManagerStats		*pStats;
	pStats = (ClSessManagerStats *)m_pStats;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	ClParamList * poParamList  ;
	ClRequestStatistics		*pReqStats ;


//	pLog = SETLOGSYSTEM (NULL );

	while (!IsTerminate())
	{
		error = M4_ERROR;
		reply = M4_TRUE;

//		SETLOGSYSTEM ( pLog ) ;

		m_pControlQ->Pop(item);

		if ( IsTerminate() || NULL == item)
			return;

//		pLog = SETLOGSYSTEM ( ((ClRequest*)item ) -> GetLog () ) ;

		m4uint32_t		reqType = item->GetRequestType();
		switch (reqType)
		{
		case M4_CONTROL_REQ_TYPE:
			poRequest = (ClRequest *)item ;

			if ( pReqStats = (ClRequestStatistics*)poRequest->GetStats() )
			{
				pReqStats -> Sett ( ClRequestStatistics::InService );
			}
			poConnection =  poRequest -> GetpConnection();

			if ( poConnection )
			{
				if ( ( (poConnection -> GetStatus() ) == CLOSSING ) || ( (poConnection -> GetStatus() ) == CLOSED ) )
				{
					break ;
				}
			}

			error = _ProcessServiceReq (item, reply);

			break;
		case M4_COMMAND_REQ_TYPE:
			error = _ProcessCommandReq (item, reply);
			break;

		default:
			break;

		}

		if (reply)
		{
//				ClProtocol oProtocol ;
			ClRequest	*pduRequest	= (ClRequest *)item;
			poConnection =  pduRequest -> GetpConnection() ;
			ClUSSessionInterface * poSession ;
			ClPDU	*outputPDU;
			m4uint8_t opID;

			if (M4_SUCCESS > error)
			{
				pduRequest -> SetError ( error ) ;

				outputPDU = pduRequest -> GetOutputPDU () ;
			}
			else
			{
				outputPDU = pduRequest -> GetOutputPDU () ;

				if (! outputPDU )
				{
					outputPDU = poConnection -> m_oProtocol.GetPDU(M4_ID_PDU_ACK);
					pduRequest -> SetPDUOutput(outputPDU);
				}

	//	Construcción del Taglist
				ClTagList				oTagList;
				ClPDU			*inputPDU = pduRequest->GetInputPDU();
				M4DataStorage	*poDataStorage;
				poDataStorage = inputPDU->GetDataStorage(M4_TRUE);



				if(poDataStorage)
				{
					oTagList.DataStorage2TagList(poDataStorage);
				}


				if ( 0 < oTagList.GetSize() )
				{
					_UpdateTagList(oTagList, pduRequest);
					poDataStorage = outputPDU->GetDataStorage(M4_TRUE);
					oTagList.TagList2DataStorage(poDataStorage);
				}
				
				oTagList.Clear();
	
				// Sólo se actualiza el estado de la Ack si realmente lo es
				opID = outputPDU -> GetIdPDU() & 0x00ff;
				if (M4_ID_PDU_ACK == opID)
				{
					( (ClAckPDU *) outputPDU ) -> SetWhy ( error ) ;
				}
			}

			
			//Para la reconexión- En lugar del id debería ir la credencial
			if ( poSession = pduRequest -> GetpSession () )
			{
				if ( M4_SUCCESS == poSession -> GetSessionID (szSessionID ) )
				{
					outputPDU -> SetCredential( szSessionID ) ;
				}
			}


			poInvoker -> GetParamList ( poParamList  ) ;

			poParamList -> SetPointer ( "REQUEST" , pduRequest  ) ;

			if ( pReqStats = (ClRequestStatistics*)pduRequest->GetStats() )
			{
				pReqStats -> Sett ( ClRequestStatistics::InSSQueueOut );
			}

			poInvoker -> ExecuteAction ( SEND_REQUEST_ACTION , poParamList ) ;


		}

	}
  //## end ClCSSessManager::Run%875868001.body
}

void ClCSSessManager::Accept (ClActiveClassVisitor &ai_oStats)
{
  //## begin ClCSSessManager::Accept%877356011.body preserve=yes
	ClSessManagerVisitor	&poSMVisitor = (ClSessManagerVisitor &)ai_oStats;
	poSMVisitor.VisitSessManager(*this);
  //## end ClCSSessManager::Accept%877356011.body
}

m4return_t ClCSSessManager::_ProcessServiceReq (ClBaseRequest *&aio_pRequest, m4bool_t &ao_bReply, ClTagList *ao_poArgs)
{
  //## begin ClCSSessManager::_ProcessServiceReq%922096963.body preserve=yes
	if (SessManagerActive != m_iState)
	{
		SETCODEF ( M4_ERR_UUSS_DENY, ERRORLOG , "" ); 
		return M4_ERROR;
	}
	m4uint32_t				PDUversion ;
	m4objid_t lIdSession , lIdUser = 0;
	ClLocalMonitor			*poLocalMonitor ;
	ClConnInstance			*controlConnection;
	ClUSSessionInterface	*controlUsSession;
	ClUSUserInterface		*controlUsUser;
	ClRequest				*controlPduRequest;
	m4handle_t				hRequest;
	ClPDU					*controlPDU;
	m4return_t				error = M4_SUCCESS,
							iRet;
	ClLogSystem				*poLog;
	m4bool_t				bTag=M4_FALSE;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	ClParamList * poParamList ;
	m4pchar_t buffer = new m4char_t [1024] ;
	M4DataStorage			*pDataStorage = NULL;
	ClConnectPDU			*connectPDU;
	ClTagList				oTagList;
	m4ServerVT				ao_vArg	;
	m4pchar_t				pcLanguage = NULL;
	m4pchar_t				pcClientID = NULL;
	m4pchar_t				pcOrganization = NULL;
	m4pchar_t				pcClientMachine = NULL;
	m4pchar_t				pCertificate = NULL , pSessContext =NULL ;
	m4uint32_t				iCertificateLen , iSessContextLen ;
	m4uint32_t				iConnVersion = M4_PDU_40_VERSION  ;	
	m4pchar_t				szCredential = NULL;
	m4int_t					iLenOut ;
	m4uint16_t				iFlags ;
	ClUSUserInterface *		poUser ;
	m4string_t				strUser;
	m4bool_t				bDo2Way = M4_FALSE;
	m4pchar_t				pcSystemInfo = NULL;
	m4pchar_t				pcClientType = NULL;
	m4pchar_t				pcClientPubIP = NULL;
	ClPDU *					pOutputPDU = NULL;
	m4uint32_t				uiElapsedTime;

	poLocalMonitor = ClLocalMonitor::Instance ( ) ;

	ao_bReply = M4_TRUE;
	controlPduRequest = (ClRequest *)aio_pRequest;
	controlConnection = controlPduRequest -> GetpConnection ( ) ;
	poLog = controlPduRequest -> GetLog ( ) ;
	if (NULL == controlConnection)
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Connection == NULL" ); 
		return M4_ERROR;
	}
	
	controlUsSession = ( ClUSSessionInterface *) controlPduRequest->GetpSession();

	controlPDU = controlPduRequest->GetInputPDU();
	if ( NULL == controlPDU )
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: controlPDU == NULL" ); 
		return M4_ERROR;
	}

	PDUversion = controlPDU -> GetVersion () ;

	m4uint16_t			pduType;
	m4uint8_t			opID;

	pduType = controlPDU->GetIdPDU();
	opID = pduType & 0x00ff;
	switch(opID)
	{

	case M4_ID_PDU_CONNECT:			//	PSL NEGOCIATION

		connectPDU = (ClConnectPDU *)controlPDU;
		pDataStorage = connectPDU->GetDataStorage();
		//En el momento de hacerse el connect pasa de pending a Active

		if(pDataStorage )
		{
			oTagList.DataStorage2TagList(pDataStorage );
		}

		// Si no llega el tag M4_PROP_SERVER_NON_CONN_ORIENTED cambiamos la conexión a ClConnection.
		if ( M4_ERROR == oTagList.GetArg (M4_PROP_SERVER_NON_CONN_ORIENTED, ao_vArg ))
		{
			ClConnection *poConnection= new ClConnection ( (ClConnection40*)controlConnection );

			if( M4_SUCCESS == poLocalMonitor -> m_oConDictionary.Replace ( controlConnection -> GetId () , poConnection ) )
			{

				controlPduRequest -> SetConnection ( poConnection  );

				delete controlConnection ;
	
				controlConnection = poConnection ;
			}
			//Si la función replace devolviera error, la conexión ya estaría cerrada y pendiente de ser borrada
		}


		if ( M4_SUCCESS == oTagList.GetArg (M4_CLIENT_ID, ao_vArg ) )
		{
			if (ao_vArg.Data.PointerChar != NULL)
			{
				pcClientID = ao_vArg.Data.PointerChar;

				controlConnection -> SetIP ( pcClientID  ) ;
			}
		}

		controlConnection -> m_oProtocol.SetVersion ( PDUversion );

		controlConnection -> SetStatus ( ACTIVE ) ;

		break;

	case M4_ID_PDU_AUTHENTICATION:			//	USER IDENTIFICATION

		ClAuthenticationPDU	*authenticPDU;
		authenticPDU = (ClAuthenticationPDU *)controlPDU;
		pDataStorage = controlPDU->GetDataStorage();

		if( pDataStorage )
		{
			oTagList.DataStorage2TagList(pDataStorage );
		}

		m4pchar_t	userPasswd,userId;
		authenticPDU->GetUserId(userId);
		authenticPDU->GetPassword(userPasswd);

		m4bool_t	bDefaultRole;
		bDefaultRole = M4_FALSE;
		bDefaultRole = authenticPDU->GetDefaultRole();

		iFlags = authenticPDU -> GetFlags () ;

		hRequest = (m4handle_t)controlPduRequest;
		iRet = s_oPendingReqs.RegisterReq(hRequest, controlPduRequest);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Registering Request" ); 
			error = M4_ERROR;
			break;
		}

		// Asignamos los valores de compresión en el objeto conexión, ya que la PDU de
		// AUTHENTICATE puede llegar por una conexión distinta a la de CONNECT.
		if (M4_SUCCESS == oTagList.GetArg (M4_COMPRESSION_TYPE, ao_vArg))
		{			
			controlConnection->SetCompressionType (ao_vArg.Data.DoubleData);
		}		
		if (M4_SUCCESS == oTagList.GetArg (M4_COMPRESSION_LEVEL, ao_vArg))
		{
			controlConnection->SetCompressionLevel (ao_vArg.Data.DoubleData);
		}		
	

		switch ( iFlags )
		{

		case M4_NORMAL_LOGIN :
			{
				//		m4objid_t		sessionId;
				//		sessionId = ClIdGenerator::GetNewId();
				//		hRequest = (m4handle_t)sessionId;
				
				// Fix bug 0032737
				m4ServerVT	ao_vArgLang	;
				if ( M4_SUCCESS == oTagList.GetArg (M4_SESS_USER_LANGUAGE, ao_vArgLang ) )
				{
					if (ao_vArgLang.Data.PointerChar != NULL)
						pcLanguage = ao_vArgLang.Data.PointerChar;
				}
				
				m4ServerVT	ao_vArgOrg	;
				if ( M4_SUCCESS == oTagList.GetArg (M4_SESS_USER_ORGANIZATION, ao_vArgOrg ) )
				{
					if (ao_vArgOrg.Data.PointerChar != NULL)
						pcOrganization = ao_vArgOrg.Data.PointerChar;
				}
				
				if ( M4_SUCCESS == oTagList.GetArg (M4_SESS_USER_CRYPT2WAY, ao_vArg ) )
				{
					bDo2Way = M4_TRUE;
				}
				
				m4ServerVT	ao_vArgClientMachine	;
				if ( M4_SUCCESS == oTagList.GetArg (M4_CLIENT_ID, ao_vArgClientMachine ) )
				{
					if (ao_vArgClientMachine.Data.PointerChar != NULL)
						pcClientMachine = ao_vArgClientMachine.Data.PointerChar;
				}
				
				//  --- LogonOrigin (a.k.a Entry Mode) 
				// 
				//  VB = "0"
				//  Corba = "1" (no utilizado)
				//  C++ con XML (solo normal - no certificado) = 2
			
				m4pchar_t	pcLogonOrigin = "0";

				m4ServerVT	ao_vArgLogonOrigin; 

				if ( M4_SUCCESS == oTagList.GetArg (M4_SESS_TCLOGIN, ao_vArgLogonOrigin ) )
				{	

					// solo tiene que venir, no tiene que venir con contenido
					
						pcLogonOrigin = "2";
				
				}

				m4ServerVT	ao_vArgSystemInfo	;
				if ( M4_SUCCESS == oTagList.GetArg (M4_SYSTEM_INFO, ao_vArgSystemInfo ) )
				{
					if (ao_vArgSystemInfo.Data.PointerChar != NULL)
						pcSystemInfo = ao_vArgSystemInfo.Data.PointerChar;
				}

				m4ServerVT	ao_vArgClientType	;
				if ( M4_SUCCESS == oTagList.GetArg (M4_CLIENT_TYPE, ao_vArgClientType ) )
				{
					if (ao_vArgClientType.Data.PointerChar != NULL)
						pcClientType = ao_vArgClientType.Data.PointerChar;
				}

				m4ServerVT	ao_vArgClientPubIP;
				if ( M4_SUCCESS == oTagList.GetArg (M4_CLIENT_PUB_IP, ao_vArgClientPubIP ) )
				{
					if (ao_vArgClientPubIP.Data.PointerChar != NULL)
						pcClientPubIP = ao_vArgClientPubIP.Data.PointerChar;
				}

				if ( ! userId )
				{
					iRet = DecryptTwoWay(userPasswd, strlen(userPasswd), controlConnection->GetIP(), strlen(controlConnection->GetIP()), szCredential, iLenOut);
				}
				else
				{
					szCredential = new m4char_t[strlen(userPasswd) + 1];
					strcpy(szCredential , userPasswd);
				}
				
				if ( iRet != M4_ERROR )
				{
					iRet = m_pServiceAPI->CreateUserSession(userId, szCredential, controlConnection, hRequest, bDefaultRole, pcLanguage, pcOrganization, bDo2Way, pcClientMachine, pcLogonOrigin, pcSystemInfo, pcClientType, pcClientPubIP, poLog, m_pControlQ);
				}
			}
			
			break;
		case M4_GET_CERTIFICATE_FLAG :

			if( ! controlUsSession )
			{
				SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: No Session in GetCertificate" ); 
				error = M4_ERROR ;
				//			error = M4_NO_SESSION_FOR_REQUEST ;
				return M4_ERROR ;
			}
			if ( ! ( poUser = controlUsSession ->GetUser () ) )
				return M4_ERROR ;
			poUser -> GetName (strUser);
			iRet = m_pServiceAPI->GenerateCertificate (controlUsSession , (char*)strUser.c_str(), hRequest , poLog, m_pControlQ) ;
			
			break ;

		case M4_CERTIFICATE_LOGIN :

			iRet = authenticPDU -> GetSessContext (pSessContext , iSessContextLen) ;

			if ( M4_SUCCESS != authenticPDU -> GetCertificate  ( pCertificate , iCertificateLen) )
			{
				return M4_ERROR ;
			}

			m4ServerVT	ao_vArgClientMachine	;
			if ( M4_SUCCESS == oTagList.GetArg (M4_CLIENT_ID, ao_vArgClientMachine ) )
			{
				if (ao_vArgClientMachine.Data.PointerChar != NULL)
					pcClientMachine = ao_vArgClientMachine.Data.PointerChar;
			}

			m4ServerVT	ao_vArgSystemInfo	;
			if ( M4_SUCCESS == oTagList.GetArg (M4_SYSTEM_INFO, ao_vArgSystemInfo ) )
			{
				if (ao_vArgSystemInfo.Data.PointerChar != NULL)
					pcSystemInfo = ao_vArgSystemInfo.Data.PointerChar;
			}

			m4ServerVT	ao_vArgClientType	;
			if ( M4_SUCCESS == oTagList.GetArg (M4_CLIENT_TYPE, ao_vArgClientType ) )
			{
				if (ao_vArgClientType.Data.PointerChar != NULL)
					pcClientType = ao_vArgClientType.Data.PointerChar;
			}

			iRet = m_pServiceAPI -> CreateUserSession ("M4_CERTIFICATE" , pCertificate , iCertificateLen, pSessContext, pcClientMachine , pcSystemInfo, pcClientType, controlConnection, hRequest, poLog, m_pControlQ ) ;

			break ;
		}

		if (M4_ERROR != iRet)
		{
			ao_bReply = M4_FALSE;
		}
		else
		{
			ClRequest *copyOfRequest;
			if ( ! userId )
			{
				SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Creating Session for null user"); 
			}
			else
			{
				SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Creating Session for user %s",  userId); 
			}
			s_oPendingReqs.DeregisterReq(hRequest, copyOfRequest);
			error = iRet;
		}

		delete[] szCredential;

		break;

	case M4_ID_PDU_PING:

		if ( ! controlUsSession )
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: No Session in Ping" );
			error = M4_ERROR ;
			return M4_ERROR;
		}

		uiElapsedTime = ((ClPingPDU *)controlPDU) -> GetElapsedTime();

		pOutputPDU = controlConnection -> m_oProtocol.GetPDU(M4_ID_PDU_PING);
		controlPduRequest -> SetPDUOutput(pOutputPDU);
		((ClPingPDU *)pOutputPDU) -> SetElapsedTime( uiElapsedTime );

		error = M4_SUCCESS ;
		break;

	case M4_ID_PDU_CHANGEPASSWORD:			//	CHANGE PASSWORD

//		ClChangePasswordPDU	*changePwdPDU;
//		changePwdPDU = (ClChangePasswordPDU *)controlPDU;

		if( ! controlUsSession )
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: No Session in ChangePassword" ); 
			error = M4_ERROR ;
//			error = M4_NO_SESSION_FOR_REQUEST ;
			return M4_ERROR ;
		}

		m4pchar_t	oldPassword, newPasswd;
		if ( controlConnection )
		{
			iConnVersion = controlConnection -> m_oProtocol.GetVersion() ;
		}
		if ( iConnVersion == M4_PDU_32_VERSION )
//		if ( M4_CONNECTION_ORIENTED_PROTOCOL == controlConnection -> GetConnProtocol () )
		{
			((ClChangePasswordPDU *)controlPDU) ->GetCurrentPassword(oldPassword);
			((ClChangePasswordPDU *)controlPDU) ->GetNewPassword(newPasswd);
		}
		else
		{
			((ClNewChangePasswordPDU *)controlPDU) ->GetCurrentPassword(oldPassword);
			((ClNewChangePasswordPDU *)controlPDU) ->GetNewPassword(newPasswd);
		}

		hRequest = (m4handle_t)controlUsSession;
		iRet = s_oPendingReqs.RegisterReq(hRequest, controlPduRequest);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Registering Request" ); 
			error = M4_ERROR;
			break;
		}

		iRet = m_pServiceAPI->ChangePassword(controlUsSession, oldPassword, newPasswd, hRequest, poLog, m_pControlQ);
		if (M4_ERROR != iRet)
		{
			ao_bReply = M4_FALSE;
		}
		else
		{
			ClRequest *copyOfRequest;
			m4string_t	strUsername;
			controlUsSession->GetUser()->GetUserId(strUsername);
			if (  strUsername.c_str() )
			{
				SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Creating ChangePassword Command for user %s",   strUsername.c_str() ); 
			}
			s_oPendingReqs.DeregisterReq(hRequest, copyOfRequest);
			error = iRet;
		}

		break;

	case M4_ID_PDU_SETROLE:
		if ( NULL == controlUsSession )
		{
			error = M4_ERROR ;
//			error = M4_NO_SESSION_FOR_REQUEST ;
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: No Session in SetRole" ); 
			break;
		}

		m4pchar_t		ptrRole;
		m4bool_t		defRole;
		m4uint16_t		roleLen;
		m4pchar_t		pszRoleId;
		defRole = M4_FALSE;

		if ( controlConnection )
		{
			iConnVersion = controlConnection -> m_oProtocol.GetVersion() ;
		}
		if ( iConnVersion == M4_PDU_32_VERSION )
//		if ( M4_CONNECTION_ORIENTED_PROTOCOL == controlConnection -> GetConnProtocol () )
		{
			roleLen = ((ClSetRolePDU *)controlPDU)->GetRole(ptrRole);
		}
		else
		{
			roleLen = ((ClNewSetRolePDU *)controlPDU)->GetRole(ptrRole);
		}
		if (NULL == ptrRole)
			pszRoleId = NULL;
		else
		{
		if ( iConnVersion == M4_PDU_32_VERSION )
//			if ( M4_CONNECTION_ORIENTED_PROTOCOL == controlConnection -> GetConnProtocol () )
			{
				defRole = ((ClSetRolePDU *)controlPDU)->IsDefaultRole();
			}
			else
			{
				defRole = ((ClNewSetRolePDU *)controlPDU)->IsDefaultRole();
			}

			pszRoleId = ptrRole;
		}

		hRequest = (m4handle_t)controlUsSession;
		iRet = s_oPendingReqs.RegisterReq(hRequest, controlPduRequest);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Registering Request" ); 
			error = M4_ERROR;
			break;
		}

		iRet = m_pServiceAPI->CheckRole(controlUsSession, pszRoleId, hRequest, defRole, poLog, m_pControlQ);
		if (M4_ERROR != iRet)
		{
			ao_bReply = M4_FALSE;
		}
		else
		{
			ClRequest *copyOfRequest;
			m4string_t	strUsername;
			controlUsSession->GetUser()->GetUserId(strUsername);
			if ( strUsername.c_str() )
			{
				SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Creating ChangePassword Command for null user" ,   strUsername.c_str()  ); 
			}
			s_oPendingReqs.DeregisterReq(hRequest, copyOfRequest);
			error = iRet;
		}

		break;

	case M4_ID_PDU_CANCEL:			//	REQUEST CANCELATION

//		ClCancelPDU		*cancelPDU;
//		cancelPDU = (ClCancelPDU *)controlPDU;

		m4objid_t	reqId;
		m4objid_t	srvreqid ;

		if ( controlConnection )
		{
			iConnVersion = controlConnection -> m_oProtocol.GetVersion() ;
		}
		if ( iConnVersion == M4_PDU_32_VERSION )
//		if ( M4_CONNECTION_ORIENTED_PROTOCOL == controlConnection -> GetConnProtocol () )
		{
			reqId = ((ClCancelPDU *)controlPDU )->GetCancelRequestId();
		}
		else
		{
			reqId = ((ClNewCancelPDU *)controlPDU )->GetCancelRequestId();
		}

		// bg 196771
		if( reqId != 0 )
		{
			iRet = controlPduRequest ->GenerateId( reqId , srvreqid ) ;
		}
		else
		{
			srvreqid = 0; // Request actual.
			iRet = M4_SUCCESS;
		}

		if ( M4_ERROR == iRet )
		{
			error = M4_ERROR ;
		}
		else
		{
			poInvoker -> GetParamList ( poParamList  ) ;

			if ( controlUsSession )
			{
				poParamList -> SetInt64 ( "SESSIONID" , (m4int64_t) controlUsSession -> GetId () ) ;
			}

			poParamList -> SetInt64 ( "REQUESTID" , (m4uint64_t)srvreqid ) ;

			iRet = poInvoker -> ExecuteAction ( CANCEL_USER_REQUEST_ACTION , poParamList  ) ;;	

//			poInvoker -> SetParamList ( *poParamList  ) ;

//		iRet= m_pUUSS->CancelUserRequest(controlUsSession, reqId);
			if (M4_SUCCESS == iRet)
				error = M4_SUCCESS;
		}
		break;

///////////////////////////////////////////
//	CONTROL.DISCONNECTION REQUEST 
	case M4_ID_PDU_DISCONNECT:

		if ( NULL == controlUsSession )
		{
			SETCODEF ( M4_ERR_UUSS_UNKNOWN , WARNINGLOG , "SESSMGR: Connection without Session. The session %d could be clossed jet" , (m4uint32_t) controlPduRequest -> GetSessionId() ); 
			ClLocalMonitor	*pLocalMonitor = ClLocalMonitor::Instance();
			
			// Si la conexión es no orientada a sesión, podría estar utilizandose
			// por otro cliente desde el M4GatewayServlet.
			if (controlConnection->GetConnProtocol( ) == M4_CONNECTION_ORIENTED_PROTOCOL){				
				pLocalMonitor->RemoveConnection(controlConnection);
			}

//			error = M4_NO_SESSION_FOR_REQUEST ;
			error = M4_ERROR ;
			break;
		}
		

		lIdSession = controlUsSession -> GetId ();

		if ( controlUsUser = controlUsSession -> GetUser () )
		{
			lIdUser = controlUsUser -> GetId () ;
		}

		poInvoker -> GetParamList ( poParamList  ) ;

		poParamList -> SetDouble ( "SESSIONID" , (m4int64_t) lIdSession ) ;

		poParamList -> SetDouble ( "USERID" , (m4int64_t) lIdUser ) ;

		poInvoker -> ExecuteAction ( END_USER_SESSION_ACTION , poParamList) ;

		poInvoker -> SetParamList ( *poParamList  ) ;

		error = M4_SUCCESS;
		break;

	default:		//	POR DEFECTO SE ENVIA ERROR
		break;

	}

	delete buffer ;
	return error;

  //## end ClCSSessManager::_ProcessServiceReq%922096963.body
}

m4return_t ClCSSessManager::_ProcessCommandReq (ClBaseRequest *&aio_pRequest, m4bool_t &ao_bReply, ClTagList *ao_poArgs)
{
  //## begin ClCSSessManager::_ProcessCommandReq%922096964.body preserve=yes
	ClConnInstance			*cmdConnection;
	ClUSSessionInterface	*cmdUsSession;
	ClUSSessionStats		*poStats;
	ClRequest				*cmdPduRequest;
	m4handle_t				hRequest;
	ClBaseRequest			*baseRequest;
	ClCommandRequest		*usRequest;
	ClBoomerangRequest		*boomRequest;
	ClUSUser				*userInfo;
	m4return_t				iRet,
							error = M4_SUCCESS;
	m4uint32_t				iNumArgs ,
							uiCredLen ,
							uiSessContextLen;
	m4bool_t				bTag=M4_FALSE;
	ClParamList * poParamList ;
	m4pchar_t				szCredential,
							szSessContext ;
	ClPDU					*outputPDU = NULL ;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance ( ) ;



	boomRequest = (ClBoomerangRequest *)aio_pRequest;
	boomRequest->GetRequest(baseRequest);
	hRequest = (m4handle_t)boomRequest->GetRequestId();

	boomRequest->ResetRequest();
	delete boomRequest;

	ao_bReply = M4_FALSE;
	aio_pRequest = NULL;

	usRequest = (ClCommandRequest *)baseRequest;
	if ( NULL == usRequest )
	{
		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: usRequest == NULL" ); 
		return M4_ERROR;
	}

	iRet = s_oPendingReqs.DeregisterReq(hRequest, cmdPduRequest);
	if ( M4_SUCCESS != iRet )
	{
		delete usRequest;

		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Deregistering Request" ); 
		return M4_ERROR;
	}

	ao_bReply = M4_TRUE;
	aio_pRequest = cmdPduRequest;
	cmdConnection = cmdPduRequest -> GetpConnection ( ) ;
	if ( NULL == cmdConnection )
	{
		delete usRequest;

		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: commandConnection == NULL" ); 
		return M4_ERROR;
	}

	iRet = usRequest->GetResult();
	if (0 > iRet)
	{
		delete usRequest;

		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: Error from UUSS" ); 
		return iRet;
	}

	error = iRet;
	hRequest = usRequest->GetHandle();
	cmdUsSession = (ClUSSessionInterface *)hRequest;
	cmdPduRequest -> SetSession ( cmdUsSession ) ;
	if (NULL == cmdUsSession)
	{
		delete usRequest;

		SETCODEF ( M4_ERR_UUSS_UNKNOWN , ERRORLOG , "SESSMGR: No session for user" ); 
		return M4_ERROR;
	}

	m4uint32_t				commandOp;
	iRet = usRequest->GetCommand(commandOp);
	switch(commandOp)
	{
	case M4_US_AUTHENTICATE:	//	0x00000001
	case M4_US_CERT_LOGIN :		//	0x00000004

		userInfo = (ClUSUser *)cmdUsSession->GetUser();


		cmdUsSession -> SetHost ( (char*) cmdConnection -> GetHost () ) ;

		if ( poStats = (ClUSSessionStats *) cmdUsSession -> GetStats () )
		{
			//Hay que contabilizar la petición en curso
			poStats  -> AddnPendingReqs ( 1 ) ;
		}
		poInvoker -> GetParamList ( poParamList  ) ;

		poParamList -> SetPointer ( "USER" , userInfo ) ;
		poParamList -> SetPointer ( "SESSION" , cmdUsSession ) ;
		poParamList -> SetPointer ( "CONNECTION" , cmdConnection ) ;

		poInvoker -> ExecuteAction ( NEW_USER_ACTION , poParamList) ;

		poInvoker -> SetParamList ( *poParamList  ) ;

		if ( M4_CONNECTION_ORIENTED_PROTOCOL == cmdConnection -> GetConnProtocol () )
		{
			((ClConnection *) cmdConnection ) -> SetMainSesion(cmdUsSession);
		}

		break;

	case M4_US_CHANGE_PWD:	//	0x00000003

		break;

	case M4_US_CHECK_ROLE:		//		0x00000002

		break;

	case M4_US_GET_CERT:		//		0x00000005

		iNumArgs = usRequest  -> GetNumberOfArgs () ;

		iRet = usRequest  -> GetArg ( --iNumArgs , szCredential , uiCredLen ) ;

		if ( szCredential && ( iRet == M4_SUCCESS ) )
		{
			if ( ! outputPDU )
				outputPDU = cmdConnection -> m_oProtocol.GetPDU(M4_ID_PDU_ACK);

			((ClPDUWithData*)outputPDU )-> SetCertificate (szCredential , uiCredLen  ) ;

		}

		iRet = usRequest  -> GetArg ( --iNumArgs , szSessContext , uiSessContextLen ) ;

		if ( szSessContext && ( iRet == M4_SUCCESS ) )
		{
			if ( ! outputPDU )
				outputPDU = cmdConnection -> m_oProtocol.GetPDU(M4_ID_PDU_ACK);

			((ClPDUWithData*)outputPDU )-> SetSessContext (szSessContext , uiSessContextLen ) ;
		}

		if ( outputPDU )
			cmdPduRequest -> SetPDUOutput ( outputPDU ) ;

		break ;

	default:
		error = M4_ERROR;
		break;
	}

	delete usRequest;

	return error;
  //## end ClCSSessManager::_ProcessCommandReq%922096964.body
}

void ClCSSessManager::SetState (m4uint32_t ai_iState)
{
  //## begin ClCSSessManager::SetState%941813353.body preserve=yes
	m_iState = ai_iState ;
  //## end ClCSSessManager::SetState%941813353.body
}

void ClCSSessManager::_UpdateTagList (ClTagList &aio_oArgs, ClRequest *ai_poRequest)
{
  //## begin ClCSSessManager::_UpdateTagList%942259213.body preserve=yes
	m4return_t			iRet;
	ClConnection		*controlConnection = NULL;
	ClUSSessionInterface * poSession = NULL ;
	ClPDU * poOutputPDU = NULL ;

	if (NULL != ai_poRequest)
	{
		controlConnection = (ClConnection *)ai_poRequest->GetpConnection();
		poSession = ai_poRequest ->GetpSession () ;
		poOutputPDU = ai_poRequest -> GetOutputPDU () ;
//		ClUSSessionInterface	*poSession = (ClUSSessionInterface	*)controlConnection->GetMainSession();
	}


	iRet=aio_oArgs.StartIteration();
	if(iRet!=M4_ERROR)
	{
		m4char_t pcTagName[64];
		
		m4ServerVT oVT1;
		m4Trace(cout <<aio_oArgs.GetSize()<<" tags arrived."<<endl);
		m4pcchar_t pcSeparator = "_";
		m4pchar_t  pcTok=NULL,pcTokAux;
		
		while(aio_oArgs.GetNextTag(pcTagName,64,oVT1)!=M4_ERROR)
		{
/* RM OSCAR 11-11-99

  La propiedad METADATA_MOD_DATE ha dejado de usarse

			if (!strcmp(pcTagName,M4_PROP_MD_COR_DATE) )
			{
				m4char_t pcDate[100];
				GetEnviron("SERVER","METADATA_MOD_DATE",pcDate, 100);
				M4ClString sDate(pcDate);
				M4ClTimeStamp tsCleanCacheDate2(sDate);
				m4time_t	modTime;
				tsCleanCacheDate2.get(modTime);
				if (M4_ERROR==aio_oArgs.SetArg(pcTagName,modTime))
					m4Trace(cerr<<"Error in SetArg:"<<pcTagName<<endl);
				continue;
			}
END OSCAR 11-11-99
*/
			if (NULL != controlConnection)
			{
				
				if (!strcmp(pcTagName, M4_PROP_SESSION_CREDENTIAL) )
				{
					if (NULL != poSession)
					{
						m4string_t				strSessionID;

						poSession->GetSessionID(strSessionID);

						/* Fix Bug 0106381
						Si se usa directamente la cadena no hace una copia y se
						queda apuntando a la dirección de memoria que luego se pierde
						*/
						oVT1.SetString((m4pchar_t)strSessionID.c_str());
						aio_oArgs.SetArg(pcTagName, oVT1);

						poOutputPDU -> SetCredential ( strSessionID ) ;
					}

					continue;
				}

				
				if (!strcmp(pcTagName,M4_COMPRESSION_TYPE) )
				{
					switch(oVT1.GetType())
					{
					case M4_SVT_TYPE_STRING:
					case M4_SVT_TYPE_STRING_VAR:
						controlConnection->SetCompressionType(atoi(oVT1.Data.PointerChar));
					break;
					case M4_SVT_TYPE_NUMBER:
						controlConnection->SetCompressionType((m4uint32_t)oVT1.Data.DoubleData);
					break;
					}
					continue;
				}

				if (!strcmp(pcTagName,M4_COMPRESSION_LEVEL) )
				{
					switch(oVT1.GetType())
					{
					case M4_SVT_TYPE_STRING:
					case M4_SVT_TYPE_STRING_VAR:
						controlConnection->SetCompressionLevel(atoi(oVT1.Data.PointerChar));
					break;
					case M4_SVT_TYPE_NUMBER:
						controlConnection->SetCompressionLevel((m4uint32_t)oVT1.Data.DoubleData);
					break;
					}
					continue;
				}


				if (!strcmp(pcTagName,M4_HTTP_PROP_SERVLET) )
				{
					controlConnection->SetClientType(M4_HTTP_PROTOCOL);

					continue;
				}

				// Bg 134521
 				if (!strcmp(pcTagName,M4_SESS_TCLOGIN) )
				{
					controlConnection->SetClientType(M4_PROTOCOL_TC);

					continue;
				}


				// Se asigna el timeout de la sessión
				if (!strcmp(pcTagName, M4_PROP_SESSION_TIMEOUT) )
				{
					m4double_t dTimeout = ClUSSession::GetTimeout();
					aio_oArgs.SetArg(pcTagName, dTimeout);

					continue;
				}

				// Enh 163151.
				if( !strcmp( pcTagName, M4_TECH_VERSION ) )
				{
					aio_oArgs.SetArg( pcTagName, (m4pchar_t) GetMindDBVersion() ) ;

					continue;
				}

				if( !strcmp( pcTagName, M4_TECH_ENCODING ) )
				{
					aio_oArgs.SetArg( pcTagName, (m4double_t) M4IsUnicode() ) ;
					continue;
				}

				if( !strcmp( pcTagName, M4_SESS_REAL_USER_ID ) )
				{
					if( NULL != poSession )
					{
						m4string_t	sUserID ;
						poSession->GetUser()->GetUserId( sUserID ) ;
						oVT1.SetString( (m4pchar_t) sUserID.c_str() ) ;
						aio_oArgs.SetArg( pcTagName, oVT1 ) ;
					}

					continue;
				}

				if( !strcmp( pcTagName, M4_SESS_USER_ALIAS ) )
				{
					if( NULL != poSession )
					{
						m4string_t	sUserAlias ;
						poSession->GetUser()->GetUserAlias( sUserAlias ) ;
						oVT1.SetString( (m4pchar_t) sUserAlias.c_str() ) ;
						aio_oArgs.SetArg( pcTagName, oVT1 ) ;
					}

					continue;
				}

				if( !strcmp( pcTagName, M4_SESS_IMPERSONATOR_USER ) )
				{
					if( NULL != poSession )
					{
						m4string_t	sUserImpersonator ;
						poSession->GetUser()->GetUserImpersonator( sUserImpersonator ) ;
						oVT1.SetString( (m4pchar_t) sUserImpersonator.c_str() ) ;
						aio_oArgs.SetArg( pcTagName, oVT1 ) ;
					}

					continue;
				}
			}


			m4char_t pcTagNameTok[64];
			strcpy(pcTagNameTok,pcTagName);
			m4TraceLevel(2,cout <<"Me ha llegado el tag "<<pcTagName<<endl);
			pcTok = strtok(pcTagNameTok, pcSeparator);
			if (! pcTok )
				continue ;
			if(strcmp(pcTok,"M4"))		
			{
				m4Trace(cerr<<"Unknown Tag type:"<<pcTok<<endl);
				continue;
			}

			pcTok = strtok(NULL, pcSeparator);
			if (! pcTok )
				continue ;
			if(strcmp(pcTok,"PROP"))
			{
				m4Trace(cerr<<"Manual Process"<<endl)
				continue;
			}
			
			
			pcTok+=strlen(pcTok)+1;
			pcTokAux=strtok(NULL,pcSeparator);
			if (! pcTokAux )
				continue ;
			pcTok+=strlen(pcTokAux)+1;
			m4char_t pcBuffer[32];
			m4TraceLevel(2,cout<<":GetEnviron("<<pcTokAux<<","<<pcTok<<")"<<endl);
			iRet=GetEnviron ( pcTokAux , pcTok , pcBuffer, 32 ) ;
			if(M4_ERROR!=iRet)
			{
				oVT1.SetString(pcBuffer);

				if (M4_ERROR==aio_oArgs.SetArg(pcTagName,oVT1))
				{
					m4Trace(cerr<<"Error in SetArg:"<<pcTagName<<endl);
					SETCODEF(M4_ERR_TAG_ERROR,ERRORLOG,"Error setting tag %s",pcTagName);
				}
			}
			else
			{
				M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", pcTok << pcTokAux);
//				SETCODEF( M4_SRV_SS_OBL_ERROR , ERRORLOG , "Error find %s,%s in environment",pcTokAux,pcTok) ;
				m4Trace(cerr<<"Error in:GetEnviron("<<pcTokAux<<","<<pcTok<<")"<<endl);
			}

		}
	}


  //## end ClCSSessManager::_UpdateTagList%942259213.body
}

m4return_t ClCSSessManager::ProcessItem (ClBaseRequest* ai_poItem)
{
  //## begin ClCSSessManager::ProcessItem%954935297.body preserve=yes
	return m_pControlQ -> Push ( ai_poItem ) ;
  //## end ClCSSessManager::ProcessItem%954935297.body
}

// Additional Declarations
  //## begin ClCSSessManager%34200ECB03D4.declarations preserve=yes
  //## end ClCSSessManager%34200ECB03D4.declarations

// Class ClSessManagerVisitor 

ClSessManagerVisitor::ClSessManagerVisitor ()
  //## begin ClSessManagerVisitor::ClSessManagerVisitor%879608294.hasinit preserve=no
  //## end ClSessManagerVisitor::ClSessManagerVisitor%879608294.hasinit
  //## begin ClSessManagerVisitor::ClSessManagerVisitor%879608294.initialization preserve=yes
  : ClActiveClassVisitor()
  //## end ClSessManagerVisitor::ClSessManagerVisitor%879608294.initialization
{
  //## begin ClSessManagerVisitor::ClSessManagerVisitor%879608294.body preserve=yes
  //## end ClSessManagerVisitor::ClSessManagerVisitor%879608294.body
}


// Additional Declarations
  //## begin ClSessManagerVisitor%344B7DF50123.declarations preserve=yes
  //## end ClSessManagerVisitor%344B7DF50123.declarations

// Class ClSessManagerStats 










ClSessManagerStats::ClSessManagerStats ()
  //## begin ClSessManagerStats::ClSessManagerStats%884336004.hasinit preserve=no
      : m_nIncorrectPDUs(0), m_nUnknownHosts(0), m_nUnknownUsers(0), m_nForgedSignatures(0), m_nInvalidUserAuthentications(0), m_nInvalidTimeMarks(0), m_nInvalidProtocolVersions(0), m_nDeniedConnections(0), m_nTimeouts(0)
  //## end ClSessManagerStats::ClSessManagerStats%884336004.hasinit
  //## begin ClSessManagerStats::ClSessManagerStats%884336004.initialization preserve=yes
  //## end ClSessManagerStats::ClSessManagerStats%884336004.initialization
{
  //## begin ClSessManagerStats::ClSessManagerStats%884336004.body preserve=yes
	m_uiNumberOfStats = TOTAL_NUM;
  //## end ClSessManagerStats::ClSessManagerStats%884336004.body
}


ClSessManagerStats::~ClSessManagerStats ()
{
  //## begin ClSessManagerStats::~ClSessManagerStats%884336005.body preserve=yes
  //## end ClSessManagerStats::~ClSessManagerStats%884336005.body
}



//## Other Operations (implementation)
m4return_t ClSessManagerStats::Reset ()
{
  //## begin ClSessManagerStats::Reset%884195142.body preserve=yes
   	ClActiveClassStats::Reset();

	m_nIncorrectPDUs = 0;
	m_nUnknownHosts = 0; 
	m_nUnknownUsers = 0;
	m_nForgedSignatures = 0;
	m_nInvalidUserAuthentications = 0;
	m_nInvalidTimeMarks = 0;
	m_nInvalidProtocolVersions = 0;
	m_nDeniedConnections = 0;
	m_nTimeouts = 0;

	return M4_SUCCESS;
  //## end ClSessManagerStats::Reset%884195142.body
}

m4return_t ClSessManagerStats::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClSessManagerStats::GetStat%886596294.body preserve=yes
	m4uint16_t				lowIndex;
	eSessManagerStatsID		statIndex;
	m4return_t				res = M4_TRUE;

	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_SESSMANAGER_SUBSYSTEM))
		return M4_ERROR;

	lowIndex = ai_hStatId.GetLowID();
	if (INCORRECT_PDUS_ID > lowIndex)
		return ClActiveClassStats::GetStat(ai_hStatId, ao_poData);

	statIndex = (eSessManagerStatsID)lowIndex;

	if (0 < ao_poData.GetSize())
		return M4_ERROR;

	ao_poData.SetSize(DEF_STRING_SIZE);
	m4pchar_t	buffer = ao_poData.GetBuffer();


	switch(statIndex)
	{
	case INCORRECT_PDUS_ID:
		sprintf(buffer, "%010d", m_nIncorrectPDUs);
		break;
	case UNKNOWN_HOSTS_ID:
		sprintf(buffer, "%010d", m_nUnknownHosts);
		break;
	case UNKNOWN_USERS_ID:
		sprintf(buffer, "%010d", m_nUnknownUsers);
		break;
	case INVALID_TIME_MARKS_ID:
		sprintf(buffer, "%010d", m_nInvalidTimeMarks);
		break;
	case INVALID_PROTOCOLV_ID:
		sprintf(buffer, "%010d", m_nInvalidProtocolVersions);
		break;
	case INVALID_USER_AUT_ID:
		sprintf(buffer, "%010d", m_nInvalidUserAuthentications);
		break;
	case FORGED_SIGN_ID:
		sprintf(buffer, "%010d", m_nForgedSignatures);
		break;
	case DENIED_CONNECTS_ID:
		sprintf(buffer, "%010d", m_nDeniedConnections);
		break;
	case TIMEOUTS_ID:
		sprintf(buffer, "%010d", m_nTimeouts);
		break;
	default:
		sprintf(buffer, "%010d", 0);
		res = M4_FALSE;
	}

	return res;
  //## end ClSessManagerStats::GetStat%886596294.body
}

m4return_t ClSessManagerStats::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClSessManagerStats::ResetStat%886596296.body preserve=yes
	m4uint16_t				lowIndex;
	eSessManagerStatsID		statIndex;
	m4return_t				res = M4_TRUE;

	if (M4_ERROR == _CheckStatID(ai_hStatId, M4_SESSMANAGER_SUBSYSTEM))
		return M4_ERROR;

	lowIndex = ai_hStatId.GetLowID();
	statIndex = (eSessManagerStatsID)lowIndex;

	switch(statIndex)
	{
	case INCORRECT_PDUS_ID:
		m_nIncorrectPDUs = 0;
		break;
	case UNKNOWN_HOSTS_ID:
		m_nUnknownHosts = 0;
		break;
	case UNKNOWN_USERS_ID:
		m_nUnknownUsers = 0;
		break;
	case INVALID_TIME_MARKS_ID:
		m_nInvalidTimeMarks = 0;
		break;
	case INVALID_PROTOCOLV_ID:
		m_nInvalidProtocolVersions = 0;
		break;
	case INVALID_USER_AUT_ID:
		m_nInvalidUserAuthentications = 0;
		break;
	case FORGED_SIGN_ID:
		m_nForgedSignatures = 0;
		break;
	case DENIED_CONNECTS_ID:
		m_nDeniedConnections = 0;
		break;
	case TIMEOUTS_ID:
		m_nTimeouts = 0;
		break;
	default:
		res = M4_FALSE;
	}

	return res;
  //## end ClSessManagerStats::ResetStat%886596296.body
}

// Additional Declarations
  //## begin ClSessManagerStats%346220D303B0.declarations preserve=yes
  //## end ClSessManagerStats%346220D303B0.declarations

// Class ClRequestRegistry 



//## begin ClRequestRegistry::s_oSync%36F0D89802B6.role preserve=yes  protected: static ClMutex {1 -> 1VHAN}
ClMutex ClRequestRegistry::s_oSync(M4_TRUE);
//## end ClRequestRegistry::s_oSync%36F0D89802B6.role

ClRequestRegistry::ClRequestRegistry ()
  //## begin ClRequestRegistry::ClRequestRegistry%897503491.hasinit preserve=no
  //## end ClRequestRegistry::ClRequestRegistry%897503491.hasinit
  //## begin ClRequestRegistry::ClRequestRegistry%897503491.initialization preserve=yes
  //## end ClRequestRegistry::ClRequestRegistry%897503491.initialization
{
  //## begin ClRequestRegistry::ClRequestRegistry%897503491.body preserve=yes
  //## end ClRequestRegistry::ClRequestRegistry%897503491.body
}


ClRequestRegistry::~ClRequestRegistry ()
{
  //## begin ClRequestRegistry::~ClRequestRegistry%897503492.body preserve=yes
  //## end ClRequestRegistry::~ClRequestRegistry%897503492.body
}



//## Other Operations (implementation)
m4return_t ClRequestRegistry::RegisterReq (m4handle_t ai_hKey, ClRequest *ai_pRequest)
{
  //## begin ClRequestRegistry::RegisterReq%897503493.body preserve=yes
	pair<ClMapOfHandle::iterator, bool>		mapRes;
	ClMapOfHandle::value_type				mapEntry ( ai_hKey , ai_pRequest );

//	mapEntry.first = ai_hKey;
//	mapEntry.second = aio_pRequest;
//	ADD	EMN	990318

	// fix bug 0095081
	ClMutBlock oBlock(&s_oSync);

	mapRes = m_oContainer.insert(mapEntry);
				
	if (FALSE == mapRes.second)
		return M4_ERROR;
	else
		return M4_SUCCESS;
  //## end ClRequestRegistry::RegisterReq%897503493.body
}

m4return_t ClRequestRegistry::DeregisterReq (m4handle_t ai_hKey, ClRequest *&ao_pRequest)
{
  //## begin ClRequestRegistry::DeregisterReq%897503495.body preserve=yes
	ClMapOfHandle::iterator					mapIter;

//	ADD	EMN	990318
	ClMutBlock oBlock(&s_oSync);
	if ( m_oContainer.end() == (mapIter = m_oContainer.find(ai_hKey)))
		return M4_ERROR;

	ao_pRequest = (*mapIter).second;

	m_oContainer.erase(mapIter);
				
	return M4_SUCCESS;
  //## end ClRequestRegistry::DeregisterReq%897503495.body
}

m4return_t ClRequestRegistry::FindReq (m4handle_t ai_hKey, ClRequest *&ao_pRequest)
{
  //## begin ClRequestRegistry::FindReq%897503496.body preserve=yes
	ClMapOfHandle::iterator					mapIter;

	ClMutBlock oBlock(&s_oSync);

	if ( m_oContainer.end() == (mapIter = m_oContainer.find(ai_hKey)))
		return M4_ERROR;

	ao_pRequest = (*mapIter).second;

	return M4_SUCCESS;
  //## end ClRequestRegistry::FindReq%897503496.body
}

// Additional Declarations
  //## begin ClRequestRegistry%357ECDBD001C.declarations preserve=yes
  //## end ClRequestRegistry%357ECDBD001C.declarations

//## begin module%342013820134.epilog preserve=yes
//## end module%342013820134.epilog
