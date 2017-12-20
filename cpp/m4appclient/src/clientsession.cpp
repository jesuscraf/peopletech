//## begin module%347958E8020A.cm preserve=no
//## end module%347958E8020A.cm

//## begin module%347958E8020A.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              ClientSesion.cpp
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
//	==============================================================================
//## end module%347958E8020A.cp

//## Module: clientsession%347958E8020A; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: c:\m4server\m4appclient\src\clientsession.cpp

//## begin module%347958E8020A.additionalIncludes preserve=no
//## end module%347958E8020A.additionalIncludes

//## begin module%347958E8020A.includes preserve=yes
#include <http_errors.hpp>
#include <clientbasicdef.hpp>
#include <m4props.hpp>
#include <rawsessionid.hpp>
#include <m4cryptc.hpp>

#ifdef PDUSTATISTIC
#include "pdusize.h"
#endif // PDUSTATISTIC

#include <clientconnection.hpp>
#include <clientservice.hpp>
#include <clientrequest.hpp>
#include <clientsession.hpp>
#include <clienttag.hpp>
#include <syncro.hpp>
#include <m4string.hpp>
#include <m4log.hpp>
#include <controlpdus.hpp>
#include <servicepdus.hpp>
#include "m4unicode.hpp"
#include "m4buildversion.hpp"
#include "m4systeminfo.hpp"
#include "sessionpinger.hpp"


extern int	M4IsUnicode( void ) ;
extern void	M4SetUnicode( int ai_iUnidode ) ;

#define	M4CH_DUMMY_B2	"\x2\x3\x8\x1\x9\x6\x5\xF\xD\x8\xF\x8\xB\x4\x7\x7\xF\x6\x4\xE\x4\xE\xE\x5\xB\xE\x6\x7\xF\x3\x7\x2\xB\x6\xC\x4\xD\xB\x6\x7\x8\x3\xE\x8\xA\xB\x7\x9\x6\x6\x7\x9\xB\x6\x4\x8\xE\xF\x3\x8\x8\xC\xA\x0\x4\x2\x6\x8\x4\xC\x4\xD\x9\x2\x8\x4\x8\x8\x6\x8\xF\x1\x1\xF\xC\x1\x7\xE"


//## begin module%347958E8020A.declarations preserve=no
//## end module%347958E8020A.declarations


// Class ClCCSession 

//## begin ClCCSession::m_strClientID%3EF86A5C0280.attr preserve=no  private: static m4string_t {UA} 
m4string_t ClCCSession::m_strClientID;
//## end ClCCSession::m_strClientID%3EF86A5C0280.attr



ClCCSession::ClCCSession (ClCCConnection *ai_pConnection, ClCCUser *ai_pUser, m4uint32_t ai_uiControlTimeout, ClTimeoutFunction ai_pTimeoutFunction)
  //## begin ClCCSession::ClCCSession%880446332.hasinit preserve=no
      : m_eStatus(S_CLOSED), m_poDebugFile(NULL), m_uiControlTimeout(ai_uiControlTimeout)
  //## end ClCCSession::ClCCSession%880446332.hasinit
  //## begin ClCCSession::ClCCSession%880446332.initialization preserve=yes
  , m_pConnection (ai_pConnection), m_pUser(ai_pUser), m_pSessionPinger (NULL)
  //## end ClCCSession::ClCCSession%880446332.initialization
{
  //## begin ClCCSession::ClCCSession%880446332.body preserve=yes
	m_pConnection->SetMainSession(this);
	m_MutexDicServices.Init();
	m_pTimeoutFunction=ai_pTimeoutFunction;
	m_PDUMutex.Init();


//	m_pConnection->m_pMainSession=this;
  //## end ClCCSession::ClCCSession%880446332.body
}


ClCCSession::~ClCCSession ()
{
	if (m_pSessionPinger != NULL)
	{
		m_pSessionPinger->FinishPinger();
		m_pSessionPinger = NULL;
	}

  //## begin ClCCSession::~ClCCSession%881084616.body preserve=yes
	if(m_pConnection)
	{
		m_pConnection->SetRecursiveDelete(M4_FALSE);
		delete m_pConnection;
	}
	if(m_pUser)
		delete m_pUser;
	RemoveAllServices();



  //## end ClCCSession::~ClCCSession%881084616.body
}

void ClCCSession::PDU_Lock(m4uint8_t ai_iOpID)
{
	if (ai_iOpID != M4_ID_PDU_CANCEL)
	{
		m_PDUMutex.Lock();
	}
	if (NULL != m_pSessionPinger)
	{
		m_pSessionPinger->NotifyPDUCommunication(M4_TRUE);
	}

}

void ClCCSession::PDU_Unlock(m4uint8_t ai_iOpID)
{
	if (NULL != m_pSessionPinger)
	{
		m_pSessionPinger->NotifyPDUCommunication(M4_FALSE);
	}

	if (ai_iOpID != M4_ID_PDU_CANCEL)
	{
		m_PDUMutex.Unlock();
	}
}


//## Other Operations (implementation)
m4return_t ClCCSession::Open (m4handle_t &ao_hSessionId, ClTagList *ao_oTagList, m4bool_t ai_bDefaultRole)
{
  //## begin ClCCSession::Open%879852416.body preserve=yes
//	############### START COMMON CODE ###########################
	m4return_t			iRet;
	m4pchar_t			strX509Cert = new m4char_t [ 1000 ] ;
	m4uint32_t			iX509CertBufLen = 1000 ;
	m4pchar_t			strSessionContext = new m4char_t [ 1000 ] ;
	m4uint32_t			iSessContextBufLen = 1000;
	ClCCAdminService	*pAdmService;
	ClCCService			*pService;
	m4ServerVT			vtCandidateServer ;
	ClSystemInfo		oSystemInfo;
	string				sSystemInfo;
	m4double_t			dSessionTimeOut;

	if (M4_ERROR == GetService (ADMIN_SERVICE_ID, pService))
	{
		pService = new ClCCAdminService(ADMIN_SERVICE, this);
		m4handle_t	hConnectionService = (m4handle_t) pService;
		m_MutexDicServices.Lock();
		m_dicServices[ADMIN_SERVICE_ID] = pService;
		m_MutexDicServices.Unlock();
	}

	m4handle_t	hAdmServiceId = (m4handle_t)pService;
	pAdmService = (ClCCAdminService *)pService;
//	############### END COMMON CODE ###########################
CRONOSTART;

	if(NULL==ao_oTagList)
		ao_oTagList=new ClTagList;

	ao_oTagList-> SetArg ( M4_CLIENT_ID , (char*) m_strClientID.c_str() ) ;

	// HTTP: para que el dispatcher no cierre la conexión en el reconnect.
	ao_oTagList-> SetArg ( "M4_CLOSE_CONNECTION" , "0" ) ;

	// Solicitamos al server su version y encoding
	ao_oTagList->SetArg( M4_TECH_VERSION , "0" ) ;
	ao_oTagList->SetArg( M4_TECH_ENCODING , (m4double_t) 0 ) ;

	// Establecer la información de sistema de la máquina cliente
	oSystemInfo.Serialize(sSystemInfo);
	ao_oTagList-> SetArg ( M4_SYSTEM_INFO, (char*) sSystemInfo.c_str() ) ;
	
	// Establecer el tipo de cliente (Rich Client = 1)
	ao_oTagList-> SetArg ( M4_CLIENT_TYPE, M4_CLIENT_TYPE_RC ) ;
	
	iRet = pAdmService->ConnectRequest(hAdmServiceId,ao_oTagList,m_uiControlTimeout,m_pTimeoutFunction);
	if ( M4_SUCCESS != iRet )
	{
		CRONOSTOP("Opensession:Connect:Error");
		return iRet;
	}

	if(ao_oTagList)
	{
		// bg 175581
		if( M4_SUCCESS==ao_oTagList->GetArg("CANDIDATE_SERVER_HOST", vtCandidateServer ) )
		{
			if( vtCandidateServer.GetType() == M4_SVT_TYPE_DOUBLE )
			{
				SETCODEF( M4_ERR_CONNECTION_ERROR, ERRORLOG, "No servers available in server site" ) ;
				return M4_ERROR ;
			}
			else
			{
				return M4_RETRY_CONNECT ;
			}
		}
	}

	m4pchar_t pcBuffer=new m4char_t[256];

	// Enh 163151.
	iRet = _CheckServerVersion( ao_oTagList ) ;

	if( iRet != M4_SUCCESS )
	{
		return M4_ERROR ;
	}

	ao_oTagList->DelTag( M4_TECH_VERSION ) ;

	m4bool_t	bEncodingChanged = M4_FALSE;
	iRet = _CheckServerEncoding( ao_oTagList, ao_hSessionId, bEncodingChanged ) ;

	if( iRet != M4_SUCCESS )
	{
		return M4_ERROR ;
	}

	ao_oTagList->DelTag( M4_TECH_ENCODING ) ;

CRONOSTOP("Opensession:Tiempo solo del Connect");

CRONORESET;

	// Do kerberos Logon or M4 Logon. If Kerberos Logon, the ticket will be inserted into
	// user password.
	ClCCSession * oSession = (ClCCSession *) ao_hSessionId;
	ClCCUser * oUser = NULL;
	if (oSession->GetUser(oUser) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	m4pchar_t pszLogonType = new m4char_t[8];
	if (ao_oTagList)
	{
		if (ao_oTagList->GetArg(M4_LOGON_TYPE_PROP_NAME, pszLogonType, 8) != M4_SUCCESS)
		{
			sprintf(pszLogonType, "%d", M4_LOGON_TYPE);
		}
	}

	delete pszLogonType;

	if (ao_hSessionId) 
	{		
		ClCCSession *oSession = (ClCCSession*) ao_hSessionId;
		ClCCUser *oUser = NULL;
		if (oSession->GetUser(oUser) != M4_SUCCESS)
		{
			// session without user????
			return M4_ERROR;
		}
		
		iRet = oUser->Encrypt2Way();

		// adds the property for server
		if (iRet == M4_SUCCESS && ao_oTagList != NULL)
		{
			if (ao_oTagList->SetTag(M4_SESS_USER_CRYPT2WAY) != M4_SUCCESS)
			{
				m4Trace(cerr<<"Error sending 2way tag"<<endl);
				SETCODE(M4_ERR_OPENSES_UNKOWN,ERRORLOG,"Error sending 2way tag");
				return M4_ERROR;
			}
		}
		
		if (iRet != M4_SUCCESS) 
		{
			m4Trace(cerr<<"Error encrypting password"<<endl);
			SETCODE(M4_ERR_OPENSES_UNKOWN,ERRORLOG,"Error encrypting password");
			return iRet;
		}
	}

	iRet = pAdmService->AuthenticateRequest(hAdmServiceId, strX509Cert , iX509CertBufLen ,	strSessionContext , iSessContextBufLen , ao_oTagList,ai_bDefaultRole,m_uiControlTimeout,m_pTimeoutFunction);

	//De omento no se devuelve el certificado en el login 
	delete [] strX509Cert ;
	delete [] strSessionContext ;


	if ( M4_SUCCESS != iRet && iRet < 0)
	{
		CRONOSTOP("Opensession:Tiempo desde Authenticate:Error");
	}
	else
	{
		CRONOSTOP("Opensession:Authenticate");

		m_eStatus = S_OPEN;

		if (ao_oTagList)
		{
			if(M4_SUCCESS==ao_oTagList->GetArg(M4_PROP_SESSION_ID,pcBuffer,256))
			{
				m_strSessionID=pcBuffer;
			}

			if( bEncodingChanged == M4_TRUE )
			{
				m4pchar_t pcUserName = NULL;
				oUser->GetName( pcUserName );

				// Informamos al dispatcher de que el id usuario ha sido recodificado.
				ao_oTagList->SetArg( M4_RE_ENCODED_USER_ID, pcUserName );
			}

			if (m_pSessionPinger == NULL)
			{
				// Timeout del server en minutos
				if( M4_SUCCESS != ao_oTagList->GetArg( M4_PROP_SESSION_TIMEOUT, dSessionTimeOut ) )
				{
					dSessionTimeOut = 0 ;
				}

				// Si es 0 no está activo => No hay que hacer gestión de ping
				if (dSessionTimeOut != 0)
				{
					m_pSessionPinger = new ClSessionPinger(this, dSessionTimeOut);
					m_pSessionPinger->Start();
				}
			}
		}
	}

	delete pcBuffer;

//	############### START COMMON CODE ###########################
	
	return iRet;
//	############### END COMMON CODE ###########################
  //## end ClCCSession::Open%879852416.body
}

m4return_t ClCCSession::AddService (m4pchar_t ai_SZServiceType, m4handle_t &ao_hServiceId)
{
  //## begin ClCCSession::AddService%887374527.body preserve=yes

	// El servicio contiene un puntero a la sesion 
	// a la que pertenece.
	ClCCService* pService = new ClCCService(ai_SZServiceType, this);
	m4return_t iRet=M4_SUCCESS;

	if (NULL != pService)
	{
		// Se registra el servicio que acaba de ser creado.
		m4handle_t idService = (m4handle_t) pService;
		m_MutexDicServices.Lock();
		m_dicServices[idService] = pService;
		m_MutexDicServices.Unlock();

		// Debe registrarse tambien en el monitor, 
		// en este caso el diccionario almacena el  
		// handle, no el código del servicio.
		ClCCMonitor	*theMonitor;
		ClCCMonitor::GetClientMonitor(theMonitor);
		theMonitor -> AddService(idService, pService);

		// Se devuelve el handle del servicio.
		ao_hServiceId = idService;
	}
	else
	{
		SETCODEF(M4_ERR_ADDSERV,ERRORLOG,"Service %s couldn't be created.",ai_SZServiceType);
		iRet=M4_ERROR;
	}
	return iRet;
  //## end ClCCSession::AddService%887374527.body
}

m4return_t ClCCSession::EnableRole (m4pchar_t ai_pRoleId, m4bool_t ai_bIsDef)
{
  //## begin ClCCSession::EnableRole%887374523.body preserve=yes
//	############### START COMMON CODE ###########################
	ClCCAdminService	*pAdmService;
	ClCCService			*pService;

	if (M4_ERROR == GetService (ADMIN_SERVICE_ID, pService))
		return M4_ERROR;
	pAdmService = (ClCCAdminService *)pService;
	m4handle_t	hAdmServiceId = (m4handle_t)pService;
//	############### END COMMON CODE ###########################

	m4return_t iRet = pAdmService->SetRoleRequest(hAdmServiceId, ai_pRoleId, ai_bIsDef,m_uiControlTimeout,m_pTimeoutFunction);
	if ( M4_SUCCESS != iRet )
		return iRet;

	m_eStatus = S_ACTIVE;
//	############### START COMMON CODE ###########################
	
	return M4_SUCCESS;
//	############### END COMMON CODE ###########################
  //## end ClCCSession::EnableRole%887374523.body
}

m4return_t ClCCSession::DisableRole (m4pchar_t ai_pRoleId)
{
  //## begin ClCCSession::DisableRole%887374524.body preserve=yes
	return M4_SUCCESS;
  //## end ClCCSession::DisableRole%887374524.body
}

m4return_t ClCCSession::RemoveService (m4handle_t ai_hService)
{
  //## begin ClCCSession::RemoveService%887374528.body preserve=yes
	// Antes de nada compruebo que el diccionario 
	// contiene una request con ese identificador.
	// Si no, devuelvo error.
	ClCCService* pService;
	GetService(ai_hService, pService);
	if (pService == NULL)
	{
		return M4_ERROR;
	}

	// Antes de cerrar el servicio habra que comprobar 
	// si existen operaciones pendientes de respuesta 
	// y actuar en consecuencia. 

	// Por ahora lo que hacemos es destruir las operaciones 
	// pendientes (lo hace el destructor del servicio).

	// Borro la entrada del diccionario.
	m_MutexDicServices.Lock();
	m4int16_t ret = m_dicServices.erase(ai_hService);
	m_MutexDicServices.Unlock();

	m4return_t iRet=M4_SUCCESS;

	if( ret != 1)
	{
		m4Trace(cerr <<"Service not deleted from dict" <<endl);
		SETCODE(M4_ERR_SERV_NOT_DEL_FROM_DICT,ERRORLOG,"Service not deleted from Session dictionary");
		iRet=M4_ERROR;
	}

	// Borro la entrada del diccionario del monitor.
	ClCCMonitor	*theMonitor;
	ClCCMonitor::GetClientMonitor(theMonitor);
	theMonitor -> RemoveService( (m4handle_t) pService);

	// Destruyo el servicio.
	delete pService;

	return iRet;
  //## end ClCCSession::RemoveService%887374528.body
}

m4return_t ClCCSession::Close ()
{
  //## begin ClCCSession::Close%879852417.body preserve=yes
//	############### START COMMON CODE ###########################

	ClCCAdminService	*pAdmService;
	ClCCService			*pService;

	m4return_t			iRet;

	if (M4_ERROR == GetService (ADMIN_SERVICE_ID, pService))
	{
		pService = new ClCCAdminService(ADMIN_SERVICE, this);
		m4handle_t	hConnectionService = (m4handle_t) pService;
		m_MutexDicServices.Lock();
		m_dicServices[ADMIN_SERVICE_ID] = pService;
		m_MutexDicServices.Unlock();
	}
	m_pConnection->SetConnStatus(M4_CONNECTION_START_CLOSING);
	m4handle_t	hAdmServiceId;
	pAdmService = (ClCCAdminService *)pService;
//	############### END COMMON CODE ###########################

	if (m_pSessionPinger != NULL)
	{
		m_pSessionPinger->FinishPinger();
		m_pSessionPinger = NULL;
	}

	//m_pConnection->SetConnStatus(M4_CONNECTION_CLOSING);
	iRet = pAdmService->DisconnectRequest(hAdmServiceId,m_uiControlTimeout,m_pTimeoutFunction);

//	############### START COMMON CODE ###########################

	return iRet;
//	############### END COMMON CODE ###########################
  //## end ClCCSession::Close%879852417.body
}

m4return_t ClCCSession::RemoveAllServices ()
{
  //## begin ClCCSession::RemoveAllServices%887374530.body preserve=yes

	// Recorre la lista de servicios abiertos y los 
	// destruye. Antes los borra del diccionario del 
	// monitor.

	typedef map<m4handle_t, ClCCService*, less<m4handle_t> >::iterator CI;

	ClCCMonitor	*theMonitor;
	ClCCMonitor::GetClientMonitor(theMonitor);

	m4handle_t		serviceName;
	ClCCService*	pService;
	m4handle_t		serviceId; 
	
	m_MutexDicServices.Lock();
	CI p = m_dicServices.begin();

	for (; p != m_dicServices.end();)
	{
		serviceName = (*p).first;
		pService = (*p).second;
	
		// Borro la entrada del diccionario del monitor.
		// Para eso calculo antes el handle.
		 serviceId = (m4handle_t) pService;

		 if(serviceName != ADMIN_SERVICE_ID) // El  AdminServ no se guarda en el monitor
			theMonitor -> RemoveService(serviceName);

		// Borro el el servicio del mapa propio
		m_dicServices.erase(p);

		p = m_dicServices.begin(); 
		
		m4TraceLevel(2,cerr << "Removing service " << serviceName << endl);
		delete pService;
	}

	m_MutexDicServices.Unlock();
	
	return M4_SUCCESS;

  //## end ClCCSession::RemoveAllServices%887374530.body
}

m4return_t ClCCSession::GetConnection (ClCCConnection *&ao_pConnection)
{
  //## begin ClCCSession::GetConnection%887374531.body preserve=yes
	ao_pConnection = m_pConnection;

	return M4_SUCCESS;
  //## end ClCCSession::GetConnection%887374531.body
}

m4return_t ClCCSession::GetUser (ClCCUser *&ao_pUser)
{
  //## begin ClCCSession::GetUser%890127478.body preserve=yes
	ao_pUser = m_pUser;

	return M4_SUCCESS;
  //## end ClCCSession::GetUser%890127478.body
}

void ClCCSession::GetStatus (eSessionStatus &ao_eStatus)
{
  //## begin ClCCSession::GetStatus%890732553.body preserve=yes
	ao_eStatus = m_eStatus;
  //## end ClCCSession::GetStatus%890732553.body
}

m4return_t ClCCSession::GetProps (ClTagList  *ao_cTagMap)
{
  //## begin ClCCSession::GetProps%898770811.body preserve=yes
	m4return_t iRet;
	if(ao_cTagMap)
	{
		iRet=m_oTagMap.StartIteration();
		if(M4_SUCCESS==iRet)
		{
			m4char_t pcTagName[256];
			m4ServerVT oVT1;
			while(m_oTagMap.GetNextTag(pcTagName,256,oVT1)!=M4_ERROR)
				iRet=ao_cTagMap->SetArg(pcTagName,oVT1);
		}
	}
	else
		m4Trace(cerr<<"NULL TagList"<<endl);

	return iRet;
  //## end ClCCSession::GetProps%898770811.body
}

m4return_t ClCCSession::GetService (m4handle_t ai_hServiceId, ClCCService *&ao_pService)
{
  //## begin ClCCSession::GetService%887374529.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	m_MutexDicServices.Lock();
	map<m4handle_t, ClCCService*, less<m4handle_t> >::const_iterator iterador;
	iterador = m_dicServices.begin();
	iterador = m_dicServices.find(ai_hServiceId);
	
	if (iterador != m_dicServices.end())
	{
		ao_pService = (*iterador).second;
		m_MutexDicServices.Unlock();
	}
	else
	{
		m_MutexDicServices.Unlock();
		if(ADMIN_SERVICE_ID!=ai_hServiceId)
			SETCODEF( M4_ERR_SERV_NOT_IN_DIC,ERRORLOG,"Service %d isn't in the Session  dictionary.",ai_hServiceId);
		iRet=M4_ERROR;
	}
	return iRet;
  //## end ClCCSession::GetService%887374529.body
}

m4return_t ClCCSession::SetProps (ClTagList *ai_cTagMap)
{
  //## begin ClCCSession::SetProps%898770812.body preserve=yes
	m4return_t iRet;
	m4ServerVT oVT1;
	if(ai_cTagMap)
	{
		if(M4_SUCCESS==ai_cTagMap->GetArg(M4_DEBUG_FILENAME,oVT1))
		{
			m4Trace(cout<<"DebugFile On"<<endl);
			if(!oVT1.Data.PointerChar)
			{
				// UNICODE FILE
				int iLength = -1 ;
				char* pcFileName = M4CppToANSI( oVT1.Data.PointerChar, iLength ) ;

				m_poDebugFile= new fstream(pcFileName, ios::out) ;
				delete [] pcFileName ;
				
				m4DebugExecute(if(!m_poDebugFile) cerr<<"Cannont open "<<oVT1.Data.PointerChar<<" file"<<endl);
			}
			else
				m4Trace(cerr<<"Null debug_filename"<<endl);
		}

		iRet=ai_cTagMap->StartIteration();
		if(M4_SUCCESS==iRet)
		{
			m4char_t pcTagName[256];
			while(ai_cTagMap->GetNextTag(pcTagName,256,oVT1)!=M4_ERROR)
				iRet=m_oTagMap.SetArg(pcTagName,oVT1);
		}
	}
	else
		m4Trace(cerr<<"NULL TagList"<<endl);


	return iRet;
  //## end ClCCSession::SetProps%898770812.body
}

m4return_t ClCCSession::ChangePassword (M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd)
{
  //## begin ClCCSession::ChangePassword%898848502.body preserve=yes
//	############### START COMMON CODE ###########################
	ClCCAdminService	*pAdmService;
	ClCCService			*pService;
//	ADD	EMN	980503
	m4return_t			iRet;

	if (M4_ERROR == GetService (ADMIN_SERVICE_ID, pService))
		return M4_ERROR;
	m4handle_t	hAdmServiceId;
	pAdmService = (ClCCAdminService *)pService;
//	############### END COMMON CODE ###########################
	iRet = pAdmService->ChangePasswordRequest(hAdmServiceId,ai_oOldPasswd , ai_oNewPasswd,m_uiControlTimeout,m_pTimeoutFunction);
	if ( M4_SUCCESS != iRet )
		return iRet;


	m_eStatus = S_ACTIVE;
//	############### START COMMON CODE ###########################
	
	return M4_SUCCESS;
//	############### END COMMON CODE ###########################
  //## end ClCCSession::ChangePassword%898848502.body
}

m4return_t ClCCSession::SendPing(m4uint32_t ai_iuPingTimeOut)
{
	ClCCAdminService	*pAdmService;
	ClCCService			*pService;
	m4return_t			iRet;

	if (M4_ERROR == GetService (ADMIN_SERVICE_ID, pService))
		return M4_ERROR;
	m4handle_t	hAdmServiceId;
	pAdmService = (ClCCAdminService *)pService;
	iRet = pAdmService->PingRequest(hAdmServiceId, ai_iuPingTimeOut, m_uiControlTimeout, m_pTimeoutFunction);

	if ( M4_SUCCESS != iRet )
		return iRet;

	return M4_SUCCESS;
}

void ClCCSession::NotifyVMExecution(m4bool_t ai_bIsStart)
{
	if (NULL != m_pSessionPinger)
	{
		m_pSessionPinger->NotifyVMExecution(ai_bIsStart);
	}
}

m4return_t ClCCSession::NotifyAllServices (eRequestStatus ai_eIncidence)
{
  //## begin ClCCSession::NotifyAllServices%923902841.body preserve=yes
	m4return_t	iRet;
	
	m_MutexDicServices.Lock();
	typedef map<m4handle_t, ClCCService*, less<m4handle_t> >::iterator CI;

	CI p = m_dicServices.begin();

	ClCCMonitor	*theMonitor;
	ClCCMonitor::GetClientMonitor(theMonitor);

	for (; p != m_dicServices.end();p++)
	{
		m4handle_t serviceName = (*p).first;
		ClCCService* pService = (*p).second;
		iRet=pService->NotifyAllRequest(ai_eIncidence);
		if(M4_ERROR==iRet)
		{m4Trace(cerr<<"Error notify request"<<endl);}

		m4TraceLevel(2,cerr << "Notify request of " << serviceName << " service."<<endl);
	}
	m_MutexDicServices.Unlock();

	return M4_SUCCESS;
  //## end ClCCSession::NotifyAllServices%923902841.body
}

m4return_t ClCCSession::_GetAnswerData (ClCCRequest *ai_pRequest, m4bool_t &ao_bReSend)
{
  //## begin ClCCSession::_GetAnswerData%950783980.body preserve=yes
	m4string_t strCredential ;


	m4return_t iRet=m_pConnection->_GetAnswerData (ai_pRequest, ao_bReSend);

	// bg 136877
	if(ao_bReSend == M4_TRUE)
	{
		return M4_SUCCESS;
	}

	ClPDU *thPDU=ai_pRequest->GetOutputPDU();
	if(NULL!=thPDU)
		thPDU->GetCredential(strCredential);

	if ( ! strCredential.empty() )

	{
		SetCredential ( strCredential ) ;

	}

	return iRet;
  //## end ClCCSession::_GetAnswerData%950783980.body
}

m4return_t ClCCSession::_SendData (ClCCRequest *ai_pRequest)
{
  //## begin ClCCSession::_SendData%950783981.body preserve=yes
	ClPDU *thPDU=ai_pRequest->GetInputPDU();
	if(NULL!=thPDU)
		thPDU->SetCredential(m_strCredential);
	
	m4pchar_t pcName = NULL ;
	m_pUser->GetName( pcName ) ;
	m4string_t	strName = pcName ;
	ai_pRequest -> SetUniqueRequestID ( m_strServerHost , m_strServerBasePort , m_strServerSessionId, strName ) ;

	m4return_t iRet=m_pConnection->_SendData(ai_pRequest);
	return iRet;
  //## end ClCCSession::_SendData%950783981.body
}

m4return_t ClCCSession::GetServerSessionParams (m4string_t &ao_strServerHost, m4string_t &ao_strServerBasePort, m4string_t &ao_strServerSessionId)
{
  //## begin ClCCSession::GetServerSessionParams%986815274.body preserve=yes
	if ( ao_strServerHost.empty() )
	{
	}

	ao_strServerHost = m_strServerHost ;
	ao_strServerBasePort = m_strServerBasePort ;
	ao_strServerSessionId = m_strServerSessionId ;

	return M4_SUCCESS ;
  //## end ClCCSession::GetServerSessionParams%986815274.body
}

m4return_t ClCCSession::SetCredential (m4string_t ai_strCredential)
{
  //## begin ClCCSession::SetCredential%986815277.body preserve=yes
	m4string_t strUsername;
	m4string_t strServerName;
	ClRawSessionIDGenerator oIdGenerator;
    m4return_t retCode = M4_ERROR;


	m_strCredential = ai_strCredential ;
    retCode = oIdGenerator.ExtractVariables(ai_strCredential, strUsername, strServerName, m_strServerSessionId, m_strServerHost, m_strServerBasePort);

	return retCode;


  //## end ClCCSession::SetCredential%986815277.body
}

void ClCCSession::SetClientID (m4string_t ai_strClientID)
{
  //## begin ClCCSession::SetClientID%1056466952.body preserve=yes
	m_strClientID  = ai_strClientID ;
  //## end ClCCSession::SetClientID%1056466952.body
}

m4return_t ClCCSession::GetSessContext (m4pchar_t &ao_strX509Cert, m4uint32_t &aoi_iX509CertBufLen, m4pchar_t &ao_strSessionContext, m4uint32_t &aoi_iSessContextBufLen)
{
  //## begin ClCCSession::GetSessContext%1056466953.body preserve=yes
	m4return_t iRet ;
	ClCCAdminService	*pAdmService;
	ClCCService			*pService;
	ClCCMonitor *theMonitor;
	ClCCMonitor::GetClientMonitor(theMonitor);

	if (M4_ERROR == GetService (ADMIN_SERVICE_ID, pService))
	{
		pService = new ClCCAdminService(ADMIN_SERVICE, this);
		m4handle_t	hConnectionService = (m4handle_t) pService;
		m_MutexDicServices.Lock();
		m_dicServices[ADMIN_SERVICE_ID] = pService;
		m_MutexDicServices.Unlock();
	}

	m4handle_t	hAdmServiceId = (m4handle_t)pService;
	pAdmService = (ClCCAdminService *)pService;
CRONOSTART;


	iRet = pAdmService->AuthenticateRequest(hAdmServiceId , ao_strX509Cert, aoi_iX509CertBufLen, ao_strSessionContext, aoi_iSessContextBufLen ,NULL,M4_FALSE,m_uiControlTimeout,m_pTimeoutFunction, M4_GET_CERTIFICATE_FLAG );
	if ( M4_SUCCESS != iRet )
	{
		CRONOSTOP("Opensession:Tiempo desde Authenticate:Error");
		if(iRet>0)
			m_eStatus = S_OPEN;
		return iRet;
	}
CRONOSTOP("Opensession: Authenticate");

return iRet ;
  //## end ClCCSession::GetSessContext%1056466953.body
}

m4return_t ClCCSession::Open (m4uchar_t *ai_szCertificate, m4uint32_t ai_iCertificateLen, m4uchar_t *ai_szSessContext, m4handle_t &ao_hSessionId, ClTagList *ao_oTagList, m4bool_t ai_bDefaultRole)
{
  //## begin ClCCSession::Open%1056466954.body preserve=yes
//	############### START COMMON CODE ###########################
	m4return_t			iRet;
	m4pchar_t			strX509Cert = new m4char_t [ 1000 ] ;
	m4uint32_t			iX509CertBufLen = 1000;
	m4pchar_t			strSessionContext = new m4char_t [ 1000 ] ;
	m4uint32_t			iSessContextBufLen = 1000 ;
	ClCCAdminService	*pAdmService;
	ClCCService			*pService;
	m4ServerVT			vtCandidateServer ;
	ClSystemInfo		oSystemInfo;
	string				sSystemInfo;
	m4double_t			dSessionTimeOut;

	if (M4_ERROR == GetService (ADMIN_SERVICE_ID, pService))
	{
		pService = new ClCCAdminService(ADMIN_SERVICE, this);
		m4handle_t	hConnectionService = (m4handle_t) pService;
		m_MutexDicServices.Lock();
		m_dicServices[ADMIN_SERVICE_ID] = pService;
		m_MutexDicServices.Unlock();
	}

	m4handle_t	hAdmServiceId = (m4handle_t)pService;
	pAdmService = (ClCCAdminService *)pService;
//	############### END COMMON CODE ###########################
CRONOSTART;

	if(NULL==ao_oTagList)
		ao_oTagList=new ClTagList;

	ao_oTagList-> SetArg ( M4_CLIENT_ID , (char*) m_strClientID.c_str() ) ;

	// HTTP: para que el dispatcher no cierre la conexión en el reconnect.
	ao_oTagList-> SetArg ( "M4_CLOSE_CONNECTION" , "0" ) ;

	// Establecer la información de sistema de la máquina cliente
	oSystemInfo.Serialize(sSystemInfo);
	ao_oTagList-> SetArg ( M4_SYSTEM_INFO, (char*) sSystemInfo.c_str() ) ;

	// Establecer el tipo de cliente (Rich Client = 1)
	ao_oTagList-> SetArg ( M4_CLIENT_TYPE, M4_CLIENT_TYPE_RC ) ;
	
	iRet = pAdmService->ConnectRequest(hAdmServiceId,ao_oTagList,m_uiControlTimeout,m_pTimeoutFunction);
	if ( M4_SUCCESS != iRet )
	{
		CRONOSTOP("Opensession:Connect:Error");
		return iRet;
	}

	if(ao_oTagList)
	{
		// bg 175581
		if( M4_SUCCESS==ao_oTagList->GetArg("CANDIDATE_SERVER_HOST", vtCandidateServer ) )
		{
			if( vtCandidateServer.GetType() == M4_SVT_TYPE_DOUBLE )
			{
				SETCODEF( M4_ERR_CONNECTION_ERROR, ERRORLOG, "No servers available in server site" ) ;
				return M4_ERROR ;
			}
			else
			{
				return M4_RETRY_CONNECT ;
			}
		}
	}

	m4pchar_t pcBuffer=new m4char_t[256];

CRONOSTOP("Opensession:Tiempo solo del Connect");

CRONORESET;

	iRet = pAdmService->AuthenticateRequest(hAdmServiceId, (char*)ai_szCertificate , ai_iCertificateLen , (char*)ai_szSessContext , strX509Cert , iX509CertBufLen ,	strSessionContext , iSessContextBufLen , ao_oTagList,ai_bDefaultRole,m_uiControlTimeout,m_pTimeoutFunction , M4_CERTIFICATE_LOGIN );
//  iRet = pAdmService->AuthenticateRequest (m4handle_t& ao_hRID, m4pchar_t ai_strX509Cert, m4uint32_t ai_iX509CertLen,  m4pchar_t ai_strSessionContext , m4pchar_t ao_strX509Cert, m4uint32_t & aoi_iX509CertBufLen,  m4pchar_t ao_strSessionContext, m4uint32_t & aoi_iSessContextBufLen, ClTagList *ao_oTag = NULL, m4bool_t ai_bDefaultRole = M4_FALSE, m4uint32_t ai_uiTimeout = TIMEOUT_ADMIN_SERV, ClTimeoutFunction ai_pTimeoutFunction = NULL, m4uint16_t ai_iFlags = 0);

	delete [] strX509Cert ;
	delete [] strSessionContext ;

	if ( M4_SUCCESS != iRet && iRet < 0)
	{
		CRONOSTOP("Opensession:Tiempo desde Authenticate:Error");
	}
	else
	{
		CRONOSTOP("Opensession:Authenticate");

		m_eStatus = S_OPEN;

		if (ao_oTagList)
		{
			if(M4_SUCCESS==ao_oTagList->GetArg(M4_PROP_SESSION_ID,pcBuffer,256))
			{
				m_strSessionID=pcBuffer;
			}

			if (m_pSessionPinger == NULL)
			{
				// Timeout del server en minutos
				if( M4_SUCCESS != ao_oTagList->GetArg( M4_PROP_SESSION_TIMEOUT, dSessionTimeOut ) )
				{
					dSessionTimeOut = 0 ;
				}

				// Si es 0 no está activo => No hay que hacer gestión de ping
				if (dSessionTimeOut != 0)
				{
					m_pSessionPinger = new ClSessionPinger(this, dSessionTimeOut);
					m_pSessionPinger->Start();
				}
			}
		}
	}

	delete pcBuffer;
//	############### START COMMON CODE ###########################
	
	return iRet;
//	############### END COMMON CODE ###########################
  //## end ClCCSession::Open%1056466954.body
}

m4return_t ClCCSession::ConnectRequest (m4handle_t &ao_hSessionId, ClTagList *ao_oTagList)
{
  //## begin ClCCSession::ConnectRequest%1105443232.body preserve=yes
	m4return_t iRet = M4_SUCCESS;
	ClCCAdminService * pAdmService = NULL;
	ClCCService	* pService = NULL;

	if (GetService(ADMIN_SERVICE_ID, pService) == M4_ERROR)
	{
		pService = new ClCCAdminService(ADMIN_SERVICE, this);
		m4handle_t	hConnectionService = (m4handle_t) pService;
		m_MutexDicServices.Lock();
		m_dicServices[ADMIN_SERVICE_ID] = pService;
		m_MutexDicServices.Unlock();
	}

	m4handle_t	hAdmServiceId = (m4handle_t) pService;
	pAdmService = (ClCCAdminService *) pService;

	if (ao_oTagList == NULL)
	{
		ao_oTagList = new ClTagList;
	}

	ao_oTagList-> SetArg(M4_CLIENT_ID, (m4pchar_t) m_strClientID.c_str());

	iRet = pAdmService->ConnectRequest(hAdmServiceId, ao_oTagList, m_uiControlTimeout, m_pTimeoutFunction);

	return iRet;
  //## end ClCCSession::ConnectRequest%1105443232.body
}


m4return_t ClCCSession::_CheckServerVersion( ClTagList *ao_oTagList )
{
	m4return_t	iRet = M4_ERROR ;
	m4char_t	acServerVer[ 255 ] ;
	m4pchar_t	pcServerVer = acServerVer ;
	m4pcchar_t	pccClientVer ;
	m4pchar_t	pcVersionsList ;
	m4pchar_t	pcSingleVersion ;


	if( ao_oTagList == NULL )
	{
		SETCODE( M4_ERR_MISMATCHED_VERSIONS, ERRORLOG ) ;
		return M4_ERROR ;
	}


	// Recuperamos la versión del servidor.
	if( ao_oTagList->GetArg( M4_TECH_VERSION, pcServerVer, 254 ) != M4_SUCCESS )
	{
		SETCODE( M4_ERR_MISMATCHED_VERSIONS, ERRORLOG ) ;
		return M4_ERROR ;
	}

	if( *pcServerVer == '0' )
	{
		SETCODE( M4_ERR_MISMATCHED_VERSIONS, ERRORLOG ) ;
		return M4_ERROR ;
	}
	

	// Recuperamos la versión tecnológica.
	pccClientVer = GetMindVersion() ;

	// Copia para no 
	pcVersionsList = new m4char_t[ strlen( pcServerVer ) + 1 ] ;
	strcpy( pcVersionsList, pcServerVer ) ;

	pcSingleVersion = strtok( pcVersionsList, ";" ) ;

	while( pcSingleVersion != NULL )
	{
		if( strcmpi( pcSingleVersion, pccClientVer ) == 0 )
		{
			iRet = M4_SUCCESS ;
			break ;
		}

		pcSingleVersion = strtok( NULL, ";" ) ;
	}
	
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_ERR_MISMATCHED_VERSIONS_VERBOSE, ERRORLOG, "#*s1*#%s#%s#", pccClientVer, acServerVer ) ;
	}

	delete pcVersionsList ;

	return iRet ;
}


m4return_t ClCCSession::_CheckServerEncoding( ClTagList *ao_oTagList, m4handle_t ai_hSessionId, m4bool_t& ao_bChanged )
{
	m4double_t	dServerEncoding ;
	m4double_t	dSetClientEncoding = 0 ;


	ao_bChanged = M4_FALSE;

	if( ao_oTagList == NULL )
	{
		SETCODE( M4_ERR_MISMATCHED_ENCODINGS, ERRORLOG ) ;
		return M4_ERROR ;
	}

	// Recuperamos el encoding del servidor.
	if( ao_oTagList->GetArg( M4_TECH_ENCODING, dServerEncoding ) != M4_SUCCESS )
	{
		SETCODE( M4_ERR_MISMATCHED_ENCODINGS, ERRORLOG ) ;
		return M4_ERROR ;
	}

	// bg 202923. 
	// Nos piden establecer el mismo encoding que el servidor.
	if( ao_oTagList->GetArg( M4_SET_CLIENT_ENCODING, dSetClientEncoding ) == M4_SUCCESS )
	{
		ao_oTagList->DelTag( M4_SET_CLIENT_ENCODING ) ;

		if( 1 == (int)dSetClientEncoding )
		{
			if( M4IsUnicode() != dServerEncoding )
			{
				// bg 203115
				// Primero codificamos el id usuario conforme al encoding del server.
				ClCCSession*	pSession = ( ClCCSession* )ai_hSessionId;
				ClCCUser*		pUser = NULL;
				m4pchar_t		pcUserName = NULL;
				m4pchar_t		pcEncUserName = NULL;
				int				iLength = -1;

				if( pSession->GetUser( pUser ) != M4_SUCCESS )
				{
					SETCODE( M4_ERR_MISMATCHED_ENCODINGS, ERRORLOG );
					return M4_ERROR;
				}

				pUser->GetName( pcUserName );

				if( dServerEncoding == 1 )
				{
					pcEncUserName = M4CppToUtf8( pcUserName, iLength ) ;
				}
				else
				{
					pcEncUserName = M4CppToANSI( pcUserName, iLength );
				}

				pUser->SetUserName( pcEncUserName );

				// Después establecemos el nuevo encoding.
				M4SetUnicode( (int)dServerEncoding ) ;

				ao_bChanged = M4_TRUE;
			}

			return M4_SUCCESS ;
		}
	}

	// Comparamos encodings.
	if( M4IsUnicode() != (int)dServerEncoding )
	{
		SETCODE( M4_ERR_MISMATCHED_ENCODINGS, ERRORLOG ) ;
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
}

// Additional Declarations
  //## begin ClCCSession%34472F2B0183.declarations preserve=yes
  //## end ClCCSession%34472F2B0183.declarations

// Class ClCCUser 




ClCCUser::ClCCUser (M4ClString &ai_strUsername, m4pchar_t ai_strPasswd, m4uint32_t ai_iPasswdLen)
  //## begin ClCCUser::ClCCUser%881084617.hasinit preserve=no
      : m_pszUsername(NULL), m_pszPasswd(NULL)
  //## end ClCCUser::ClCCUser%881084617.hasinit
  //## begin ClCCUser::ClCCUser%881084617.initialization preserve=yes
  //## end ClCCUser::ClCCUser%881084617.initialization
{
  //## begin ClCCUser::ClCCUser%881084617.body preserve=yes
	if(ai_strUsername.size())
	{
		m_pszUsername=new m4char_t[ai_strUsername.size() +1];
		strcpy(m_pszUsername,ai_strUsername.c_str());
	}
	if(ai_strPasswd)
	{
		m_pszPasswd=new m4char_t[ai_iPasswdLen +1];
		memcpy(m_pszPasswd,ai_strPasswd,ai_iPasswdLen);
		m_pszPasswd [ai_iPasswdLen ] = 0 ;
		m_iPasswdLen = ai_iPasswdLen +1;
	}
  //## end ClCCUser::ClCCUser%881084617.body
}


ClCCUser::~ClCCUser ()
{
  //## begin ClCCUser::~ClCCUser%881084618.body preserve=yes
	if(m_pszUsername)
		delete m_pszUsername;
	if(m_pszPasswd)
		delete m_pszPasswd;

  //## end ClCCUser::~ClCCUser%881084618.body
}



//## Other Operations (implementation)
m4return_t ClCCUser::AddSession (m4handle_t ai_hConnectionId, m4handle_t ai_hSessionId)
{
  //## begin ClCCUser::AddSession%887628421.body preserve=yes
	return M4_SUCCESS;
  //## end ClCCUser::AddSession%887628421.body
}

m4return_t ClCCUser::RemoveAllSessions ()
{
  //## begin ClCCUser::RemoveAllSessions%887628422.body preserve=yes
//	MOD	EMN	980316
/*
	typedef map<m4handle_t, ClCCSession	*, less<m4handle_t> >::const_iterator SessionIt;

	for (SessionIt sessionIterator= m_dicSessions.begin(); sessionIterator != m_dicSessions.end(); sessionIterator++)
	{
		ClCCSession* pSession = (*sessionIterator).second;

		// Borro la entrada del diccionario del monitor.
		// Para eso calculo antes el handle.

		pSession -> CloseAllServices();
		// Borro el servicio
		delete pSession;
	}
*/
	return M4_SUCCESS;
  //## end ClCCUser::RemoveAllSessions%887628422.body
}

m4return_t ClCCUser::RemoveSession (m4handle_t ai_hConnectionId)
{
  //## begin ClCCUser::RemoveSession%887628423.body preserve=yes
	return M4_SUCCESS;
  //## end ClCCUser::RemoveSession%887628423.body
}

m4return_t ClCCUser::GetName (m4pchar_t &ao_pszName)
{
  //## begin ClCCUser::GetName%887731065.body preserve=yes
	ao_pszName = m_pszUsername;

	return M4_SUCCESS;
  //## end ClCCUser::GetName%887731065.body
}

m4return_t ClCCUser::GetPasswd (m4pchar_t &ao_pszPasswd, m4uint32_t &ao_iPasswdLen)
{
  //## begin ClCCUser::GetPasswd%887731066.body preserve=yes
	ao_pszPasswd = m_pszPasswd;
	ao_iPasswdLen = m_iPasswdLen ;

	return M4_SUCCESS;
  //## end ClCCUser::GetPasswd%887731066.body
}

m4return_t ClCCUser::GetSession (m4handle_t ai_hConnection)
{
  //## begin ClCCUser::GetSession%890042317.body preserve=yes
	return M4_SUCCESS;
  //## end ClCCUser::GetSession%890042317.body
}


m4return_t ClCCUser::Encrypt2Way ()
{
  //## begin ClCCUser::Encrypt2Way%1056466950.body preserve=yes
	m4int_t		iSizeEncrypted = 0;
	m4pchar_t	pcEncryptedPass = NULL;
	
	if (m_pszPasswd == NULL)
	{
		return M4_ERROR;
	}

	// Encript password.
	if (EncryptTwoWay(m_pszPasswd, m_iPasswdLen - 1, M4CH_DUMMY_B2 + 23, strlen(M4CH_DUMMY_B2 + 23), pcEncryptedPass, iSizeEncrypted) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// updates structure
	delete [] m_pszPasswd;
	m_iPasswdLen = iSizeEncrypted + 1;
	m_pszPasswd = new m4char_t[m_iPasswdLen];
	memcpy(m_pszPasswd, pcEncryptedPass, iSizeEncrypted);
	m_pszPasswd[iSizeEncrypted] = '\0';

	delete [] pcEncryptedPass;

	return M4_SUCCESS;
  //## end ClCCUser::Encrypt2Way%1056466950.body
}

m4return_t ClCCUser::SetPassword (m4pchar_t ai_pcPassword, m4uint32_t ai_iPasswLen)
{
  //## begin ClCCUser::SetPassword%1101820656.body preserve=yes
	if (m_pszPasswd != NULL)
	{
		delete [] m_pszPasswd;
	}

	m_pszPasswd = ai_pcPassword;
	m_iPasswdLen = ai_iPasswLen;
	return M4_SUCCESS;
  //## end ClCCUser::SetPassword%1101820656.body
}

m4return_t ClCCUser::SetUserName (m4pchar_t ai_pcUserName)
{
  //## begin ClCCUser::SetUserName%1101820657.body preserve=yes
	if (m_pszUsername != NULL)
	{
		delete [] m_pszUsername;
	}

	m_pszUsername = ai_pcUserName;
	return M4_SUCCESS;
  //## end ClCCUser::SetUserName%1101820657.body
}

// Additional Declarations
  //## begin ClCCUser%344739B502B9.declarations preserve=yes
  //## end ClCCUser%344739B502B9.declarations

//## begin module%347958E8020A.epilog preserve=yes
//## end module%347958E8020A.epilog
