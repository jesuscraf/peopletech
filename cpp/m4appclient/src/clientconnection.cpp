//## begin module%38ABD2A50383.cm preserve=no
//## end module%38ABD2A50383.cm

//## begin module%38ABD2A50383.cp preserve=no
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
//## end module%38ABD2A50383.cp

//## Module: clientconnection%38ABD2A50383; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: C:\m4server\m4appclient\src\clientconnection.cpp

//## begin module%38ABD2A50383.additionalIncludes preserve=no
//## end module%38ABD2A50383.additionalIncludes

//## begin module%38ABD2A50383.includes preserve=yes
#include <m4props.hpp>
#include <http_errors.hpp>

#include <clarraylibres.hpp>
#include <eventdisplayer.hpp>
#include <m4eventhandlers.hpp>
#include <clientapi.hpp>
//#include <singleselect.hpp>
#include <commsocketpool.hpp>
//## end module%38ABD2A50383.includes

// clientbasicdef
#include <clientbasicdef.hpp>
// clientconnection
#include <clientconnection.hpp>
// pduherram
#include <pduherram.hpp>
// clientsenderq
#include <clientsenderq.hpp>
// clientrequest
#include <clientrequest.hpp>
// clientsession
#include <clientsession.hpp>
// clientmonitor
#include <clientmonitor.hpp>
// FastQ
#include <fastfifoq.hpp>
// ConnBase
#include <connbase.hpp>
// iexecutable
#include <iexecutable.hpp>
// m4types
#include <m4types.hpp>
// syncro
#include <syncro.hpp>
// m4string
#include <m4string.hpp>
// m4communication
#include <m4communication.hpp>


//## begin module%38ABD2A50383.declarations preserve=no
//## end module%38ABD2A50383.declarations

//## begin module%38ABD2A50383.additionalDeclarations preserve=yes
#include <m4date.hpp>
#define M4_CLIENTAPI_ACTION_READED_PDU  "M4_CLIENTAPI_ACTION_READED_PDU"
#include <compressdatastorage.hpp>
//## end module%38ABD2A50383.additionalDeclarations



int SingleSelect (int ai_iSocket, unsigned long ai_Segundos = 0L, unsigned long ai_Microsegundos = 0L, m4bool_t ai_bRead = M4_TRUE, m4bool_t ai_bWrite = M4_TRUE, m4bool_t ai_bExceptions = M4_TRUE)
{
	struct	timeval	timeout;
	fd_set	mask;
#ifdef _WIN32
	int	num_sockets = 0;
#else
	int	num_sockets = ai_iSocket + 1;
#endif
	FD_ZERO(&mask);
	FD_SET(ai_iSocket, &mask);


	if ((ai_Segundos != 0L) || (ai_Microsegundos != 0L))
	{
		timeout.tv_sec = ai_Segundos;	
		timeout.tv_usec = ai_Microsegundos;	
	}

	return select(num_sockets ,
		 (ai_bRead)? &mask : (fd_set *)NULL,
		 (ai_bWrite)? &mask : (fd_set *)NULL,
		 (ai_bExceptions)? &mask : (fd_set *)NULL,
		(ai_Segundos == 0L && ai_Microsegundos == 0L)? NULL: &timeout);
}





// Class ClCCConnection 


ClCCConnection::ClCCConnection (M4ClString &ai_strHost, m4int_t ai_iHostPort, m4bool_t ai_bSSL, M4ClString &ai_strCACertPath, m4int_t ai_iCommsType)
  //## begin ClCCConnection::ClCCConnection%921763083.hasinit preserve=no
      : m_iRef(0), m_cType(M4_FALSE), m_cError(M4_SUCCESS), m_eConnStatus(M4_CONNECTION_STARTING), m_eTSAPStatus(M4_CONNECTION_NOTSAP), m_bRecursive(M4_TRUE), m_pTSAPSelect(NULL), m_pMainSession(NULL), m_hCommHandle(0), m_pComms(NULL), m_pExecutable(NULL), m_iCommsType(0)
  //## end ClCCConnection::ClCCConnection%921763083.hasinit
  //## begin ClCCConnection::ClCCConnection%921763083.initialization preserve=yes
  ,ClConnBase ( (m4uint32_t)0 )
  //## end ClCCConnection::ClCCConnection%921763083.initialization
{
  //## begin ClCCConnection::ClCCConnection%921763083.body preserve=yes

	// initialize event function member. bugid (0082028)
	m_pEventFunction = NULL;
    m_pMutex = NULL;

	m4return_t iRet;
	iRet=ClCCMonitor::GetClientMonitor(m_pClientMonitor);
	if(M4_NULL_COMMS_TYPE==ai_iCommsType)
		m_iCommsType=m_pClientMonitor->GetCommsType ();
	else
		m_iCommsType=ai_iCommsType;

	if(M4_ADVANCED_COMMS_TYPE==m_iCommsType)
	{
		m_pComms=ClClientAPI::GetComms();
        if (NULL == m_pComms)
        {
			m_cError=M4_ERROR;
            return;
        }
		m_pExecutable=new ClClientExecutableInterface(&m_PDUQueue);
        if (NULL == m_pExecutable)
        {
			m_cError=M4_ERROR;
            return;
        }
		m_pExecutable->SetConnection(this);
	}

	m_strHost=ai_strHost;
	m_iHostPort=ai_iHostPort;

	// bg 117797
	// Si nos conectamos a un dispatcher tendremos que esperar al reconnect para
	// asignar el servidor de aplicaciones, de momento se queda con el dispatcher.
	// Si además la conexión es http, el constructor hijo asignará el servidor
	// de aplicaciones, de momento se queda con el servidor web.
	m_strAuthServer = ai_strHost ;

	m_bSSL=ai_bSSL;
	m_strCACertPath=ai_strCACertPath;
	m_oProtocol.SetVersion(M4_PDU_40_VERSION);

	// We just need to create a TSAP (socket) if it is not an HTTP connection.
	// Old HTTP connection implementation (ClCCConnectionHTTP) will not work
	// with this change.
	if (m_iCommsType != M4_RAW_COMMS_TYPE)
	{
		iRet=CreateTSAP();
		if(iRet==M4_ERROR)
		{
			SetConnStatus(M4_CONNECTION_NOTSAP);
			SetTSAPStatus(M4_CONNECTION_NOTSAP);
			m_cError=M4_ERROR;
		}
		else
		{
			SetConnStatus(M4_CONNECTION_STARTING);
			SetTSAPStatus(M4_CONNECTION_TSAP_OK);
			m_cError=M4_SUCCESS;
		}
	}

	m_pMutex=new ClMutex;
	m_pMutex ->Init();
  //## end ClCCConnection::ClCCConnection%921763083.body
}


ClCCConnection::~ClCCConnection ()
{
  //## begin ClCCConnection::~ClCCConnection%881084619.body preserve=yes

	switch (GetConnStatus())
	{
	case M4_CONNECTION_STARTING :
	case M4_CONNECTION_RUNNING :
		m4TraceLevel(2,cerr<< "Error en connection!!"<<endl);
	case M4_CONNECTION_CLOSING :
	case M4_CONNECTION_START_CLOSING:
		SetTSAPStatus(M4_CONNECTION_CLOSING_TSAP);
		SetConnStatus(M4_CONNECTION_FINISHING);
		break;
	case M4_CONNECTION_FINISHING:
		break;
	case M4_CONNECTION_NOTSAP:
		break;
	default:
		m4Trace(cerr<<"Error en la codificacion del estado de la conexion"<<endl);
		break;
	}

	m4return_t iRet;
	if(!m_cType)
	{
		iRet = m_pClientMonitor->RemoveConnection( (m4handle_t)this);
	
		if (M4_ERROR == iRet)
        {
			m4TraceLevel(2,cerr<<"Couldn't remove connection from monitor."<<endl);
        }
	
		// We just need to create a TSAP (socket) if it is not an HTTP connection.
		// Old HTTP connection implementation (ClCCConnectionHTTP) will not work
		// with this change.
		if (m_iCommsType != M4_RAW_COMMS_TYPE)
		{
			iRet=DestroyTSAP();
			if(M4_ERROR==iRet)
			{m4Trace(cerr<<"Errror in DestroyTSAP"<<endl);}
		}

		m4TraceLevel(2,cerr <<"Removing All Sessions" <<endl);
		if(m_bRecursive)
			RemoveAllSessions();
				
	}

    if (NULL != m_pMutex)
    {
	    delete m_pMutex;
    }

	if(M4_ADVANCED_COMMS_TYPE==m_iCommsType)
	{
//		iRet=DestroyTSAP();
		if(NULL!=m_pExecutable)
			delete m_pExecutable;
	}
	if(NULL != m_pTSAPSelect)
    {
		delete m_pTSAPSelect;
    }
  //## end ClCCConnection::~ClCCConnection%881084619.body
}



//## Other Operations (implementation)
m4int64_t ClCCConnection::GetServerTime ()
{
  //## begin ClCCConnection::GetServerTime%879852408.body preserve=yes
	return 0;
  //## end ClCCConnection::GetServerTime%879852408.body
}

m4return_t ClCCConnection::GetTSAP (ClTSAP *&ao_pTSAP)
{
  //## begin ClCCConnection::GetTSAP%880446330.body preserve=yes

	if (m_poTSAP == NULL)
	{
		while(m_poTSAP == NULL)
		{
			ao_pTSAP =m_poTSAP= (ClTSAP *)ClClientAPI::m_poTSAPPool->GetSocket(m_strURL);
			if(m_poTSAP == NULL)
				Sleep(100);
		}
	}
	else
		ao_pTSAP=m_poTSAP;

	return M4_SUCCESS;
  //## end ClCCConnection::GetTSAP%880446330.body
}

m4return_t ClCCConnection::RemoveAllSessions ()
{
  //## begin ClCCConnection::RemoveAllSessions%887374532.body preserve=yes

	if (NULL != m_pMainSession)
	{
		m4TraceLevel(2,cerr << "Removing All Services" <<endl);
		m_pMainSession->RemoveAllServices();

		m4TraceLevel(2,cerr << "Removing Main Session" <<endl);
		RemoveSession((m4handle_t)m_pMainSession);
			
		return M4_SUCCESS;
	}
	else
	{
	//	m4Trace(cerr <<"MainSession no establecida o ya borrada." <<endl);
		return M4_ERROR;
	}

  //## end ClCCConnection::RemoveAllSessions%887374532.body
}

m4return_t ClCCConnection::AddSession (m4handle_t ai_hSessionId, ClCCSession *ai_pSession)
{
  //## begin ClCCConnection::AddSession%887452284.body preserve=yes
	return M4_SUCCESS;
  //## end ClCCConnection::AddSession%887452284.body
}

m4return_t ClCCConnection::RemoveSession (m4handle_t ai_hSession)
{
  //## begin ClCCConnection::RemoveSession%887452285.body preserve=yes

	m4return_t iRet;

	ClCCSession * pSession;
	iRet=m_pClientMonitor -> GetSession(ai_hSession,pSession);

	if (iRet == M4_ERROR) 
	{
		m4Trace(cerr <<"NO ENCUENTRO LA SESSION en el monitor.No puedo borrarla.\n");
		return M4_ERROR;
	}

	iRet=m_pClientMonitor -> RemoveSession(ai_hSession);
	if (iRet == M4_ERROR) 
	{m4Trace(cerr <<"NO PUEDO BORRAR LA SESSION del monitor\n");}
	else
	{m4TraceLevel(2,cerr << "Session Removed" << endl);}

	// Borro la session
	delete pSession;

	return M4_SUCCESS;
  //## end ClCCConnection::RemoveSession%887452285.body
}

m4return_t ClCCConnection::GetSession (m4handle_t ai_hSession, ClCCSession *ao_pSession)
{
  //## begin ClCCConnection::GetSession%890042316.body preserve=yes
	if (ai_hSession == (m4handle_t)m_pMainSession)
	{
		ao_pSession = m_pMainSession;
		return M4_SUCCESS;
	}
	else
		return M4_ERROR;
  //## end ClCCConnection::GetSession%890042316.body
}

m4return_t ClCCConnection::SetMainSession (ClCCSession *ai_pSession)
{
  //## begin ClCCConnection::SetMainSession%901562156.body preserve=yes
	if(m_pMainSession == NULL)
	{
		m_pMainSession=ai_pSession;
		return M4_SUCCESS;
	}
	else
		return M4_ERROR;
  //## end ClCCConnection::SetMainSession%901562156.body
}

m4char_t ClCCConnection::GetConnType ()
{
  //## begin ClCCConnection::GetConnType%921676538.body preserve=yes
	if(m_cType)
	{m4Trace(cerr<<"Error en GetConnType!!!!"<<endl);}
	return m_cType;
  //## end ClCCConnection::GetConnType%921676538.body
}

m4return_t ClCCConnection::SetMonitor (ClCCMonitor *ai_pMonitor)
{
  //## begin ClCCConnection::SetMonitor%921763084.body preserve=yes
	if(!ai_pMonitor)
		return M4_ERROR;
	if(m_pClientMonitor)
		return M4_WARNING;
	m_pClientMonitor=ai_pMonitor;
	return M4_SUCCESS;
  //## end ClCCConnection::SetMonitor%921763084.body
}

m4return_t ClCCConnection::CreateTSAP (m4pchar_t ai_pcAlterHostID, m4int_t ai_iAlterHostPort)
{
  //## begin ClCCConnection::CreateTSAP%921763085.body preserve=yes
	m4return_t iRet;
	if(m_iCommsType==M4_ADVANCED_COMMS_TYPE)
	{
		m4string_t thehost;
		if(NULL!=ai_pcAlterHostID)
			thehost=ai_pcAlterHostID;
		else
			thehost=m_strHost.c_str();
		m4int_t thePort;
		if(0!=ai_iAlterHostPort)
			thePort=ai_iAlterHostPort;
		else
			thePort=m_iHostPort;

		m_hCommHandle=(m4handle_t)ClIdGenerator::GetNewId();
		if(m_bSSL)
		{
			iRet=m_pComms->OpenConnection(m_hCommHandle,thehost,thePort,M4_COMM_PDU_PROTOCOL,m_strCACertPath);
		}
		else
		{
			iRet=m_pComms->OpenConnection(m_hCommHandle,thehost,thePort,M4_COMM_PDU_PROTOCOL);
		}

		if(M4_ERROR!=iRet)
		{
			m_pComms->SetProtocolVersion(m_hCommHandle,M4_PDU_40_VERSION);
			ClPDU *thePDU;
			while(m_pExecutable->m_pPDUQueue->Size())
			{	
				m_pExecutable->m_pPDUQueue->Pop(thePDU);
				M4_ASSERT(thePDU==NULL);
			}

			iRet=m_pComms->StartListening(m_hCommHandle,M4_CLIENTAPI_ACTION_READED_PDU,m_pExecutable);
			if(M4_ERROR==iRet)
				return M4_ERROR;
			else
				return M4_SUCCESS;
		}
		else
		{
			SETCODEF(M4_ERR_CONNECTION_ERROR, ERRORLOG, "Couldn't connect to server %s port %d", thehost.c_str(), thePort);
			return M4_ERROR;
		}
	}
	else
	{
		
		if(m_eTSAPStatus!=M4_CONNECTION_NOTSAP)
		{
			m4Trace(cerr<<"Error en la codificacion de Status"<<endl);
			return M4_ERROR;
		}

		m4tliid_t theTLI=NULL;

		if(m_bSSL)
		{
			if(m_strCACertPath.empty())
			{
				m4Trace(cerr<<"Error:NULL CaCert path"<<endl);
				m_cError=M4_ERROR;
				return M4_ERROR;
			}
			
			struct
			{
				m4pchar_t pc_cea_signature;
				m4pchar_t pc_cea_signature_path;
			} pData;
			
			pData.pc_cea_signature_path=NULL;
			pData.pc_cea_signature=new char[m_strCACertPath.size()+1];
			strcpy(pData.pc_cea_signature,m_strCACertPath.c_str());
			iRet = SocketInit( M4_SSL_CLIENT_CERTIFICATE,(void *)&pData);
			if(iRet!=M4_SUCCESS)
			{
				delete pData.pc_cea_signature;
				SETCODE(M4_ERR_INITSAP,ERRORLOG,"Error starting SSL comunication.Not valid CaCert file");
				m4Trace(cerr<<"starting SSL comunication.Not valid CaCert file"<<endl);
				m_cError=M4_ERROR;
				return M4_ERROR;
			}

			delete pData.pc_cea_signature;

			// Creacion de un TSAPSSL
			if(ai_iAlterHostPort)
			{
				m_poTSAP= new ClTSAP(M4_SSL_CLIENT, NULL,ai_pcAlterHostID, ai_iAlterHostPort);
			}
			else
			{
				m_poTSAP= new ClTSAP(M4_SSL_CLIENT, NULL,m_strHost, m_iHostPort);
			}
		}
		else
		{

			char auxBuffer[256];
			// Creacion de un TSAP
			if(ai_iAlterHostPort)
			{
				sprintf(auxBuffer,"pdu:%d//%s",ai_iAlterHostPort,ai_pcAlterHostID);
			//	m_poTSAP= new ClTSAP(ai_pcAlterHostID, ai_iAlterHostPort);
			}
			else
			{
				sprintf(auxBuffer,"pdu:%d//%s",m_iHostPort,m_strHost.c_str());
				//m_poTSAP  = new ClTSAP(m_strHost.c_str(), m_iHostPort);
			}
			m_strURL=auxBuffer;
			if(NULL==ClClientAPI::m_poTSAPPool)
				ClClientAPI::m_poTSAPPool=new ClCommSocketPoolBasicTSAP;
			theTLI=ClClientAPI::m_poTSAPPool->GetSocket(m_strURL);
			m_poTSAP  = (ClTSAP*)theTLI;
		}

		if (m_poTSAP == NULL || m_poTSAP->Error() == M4_TRUE)
		{
			if(ai_iAlterHostPort)
			{
				m4Trace(cerr << "Couldn't connect to server " <<ai_pcAlterHostID << " port " << ai_iAlterHostPort <<endl);
				SETCODEF(M4_ERR_CONNECTION_ERROR, ERRORLOG, "Couldn't connect to server %s port %d",m_strHost.c_str(),m_iHostPort);
			}
			else
			{
				m4Trace(cerr << "Couldn't connect to server " <<m_strHost.c_str() << " port " << m_iHostPort <<endl);
				SETCODEF(M4_ERR_OPENSES_TSAP,ERRORLOG,"Couldn't connect to server %s port %d", m_strHost.c_str(), m_iHostPort);
			}

			if (m_poTSAP)
			{
				m4Trace(cerr<< m_poTSAP->GetStringError()<<endl);
				delete m_poTSAP;
				m_poTSAP=NULL;
			}
			m_cError=M4_ERROR;
			return M4_ERROR;
		}
		else
			m4DebugExecute(if(m_bSSL) cout<<"Using SSL"<<endl);

		

		iRet=m_pClientMonitor->AddConnection((m4handle_t)this,this);

		if (M4_ERROR == iRet)
		{
			SETCODE(M4_ERR_OPENSES_ADDCONECT,ERRORLOG,"Failed adding connection in the monitor.");
			m_cError=M4_ERROR;
			iRet=DestroyTSAP();
			if(M4_ERROR==iRet)
			{m4Trace(cerr<<"Error deleting TSAP"<<endl);}
			m_poTSAP=NULL;
			return M4_ERROR;
		}

		m_eTSAPStatus=M4_CONNECTION_TSAP_OK;

		return M4_SUCCESS;
	}
  //## end ClCCConnection::CreateTSAP%921763085.body
}

m4return_t ClCCConnection::GetError ()
{
  //## begin ClCCConnection::GetError%921763086.body preserve=yes
	return m_cError;
  //## end ClCCConnection::GetError%921763086.body
}

m4return_t ClCCConnection::DestroyTSAP ()
{
  //## begin ClCCConnection::DestroyTSAP%921763087.body preserve=yes
	if(m_iCommsType==M4_ADVANCED_COMMS_TYPE)
	{
		m4return_t iRet=M4_SUCCESS;
		if ((NULL != m_pComms) && (GetTSAPStatus() != M4_CONNECTION_NOTSAP))
		{
			iRet=m_pComms->StopListening(m_hCommHandle);
			if (M4_ERROR==iRet)
			{
				m4Trace(cerr <<"CloseConn error. Esperando hasta que notifique el corte"<<endl);
				int iRetrys=0;
				while((GetConnStatus()!=M4_CONNECTION_CLOSING) && !m_iRef && (iRetrys<50))
				{
					Sleep(100);
					iRetrys++;
				}
			}
			else
            {
				iRet=m_pComms->CloseConnection(m_hCommHandle);
            }
		}

		return iRet;
	}

	switch(m_eTSAPStatus)
	{
		case M4_CONNECTION_CLOSING_TSAP: 
			// Perfecto
			break;
		case M4_CONNECTION_NOTSAP:
			//m4Trace(cerr<<"No TSAP for deleting"<<endl);
			return M4_ERROR;
			break;
		default:
			m_eTSAPStatus=M4_CONNECTION_CLOSING_TSAP;
			break;
	}


	if(!m_cType && m_poTSAP)
	{
		m_poTSAP->Close();
		delete m_poTSAP;
		m_poTSAP=NULL;
	}

	m_eTSAPStatus=M4_CONNECTION_NOTSAP;
	return M4_SUCCESS;
  //## end ClCCConnection::DestroyTSAP%921763087.body
}

eConnectionStatus ClCCConnection::GetConnStatus ()
{
  //## begin ClCCConnection::GetConnStatus%922184354.body preserve=yes
	return m_eConnStatus;
  //## end ClCCConnection::GetConnStatus%922184354.body
}

eConnectionStatus ClCCConnection::GetTSAPStatus ()
{
  //## begin ClCCConnection::GetTSAPStatus%922441070.body preserve=yes
	return m_eTSAPStatus;
  //## end ClCCConnection::GetTSAPStatus%922441070.body
}

m4return_t ClCCConnection::SetConnStatus (eConnectionStatus ai_eConnStatus)
{
  //## begin ClCCConnection::SetConnStatus%923902838.body preserve=yes
/*	if(ai_eConnStatus!=m_eConnStatus)	
		cerr << "antes "<<m_eConnStatus<<" ahora "<<ai_eConnStatus<<endl;
*/
	m_eConnStatus=ai_eConnStatus;
	return M4_SUCCESS;
  //## end ClCCConnection::SetConnStatus%923902838.body
}

m4return_t ClCCConnection::SetTSAPStatus (eConnectionStatus ai_eTSAPStatus)
{
  //## begin ClCCConnection::SetTSAPStatus%923902839.body preserve=yes
	m_eTSAPStatus=ai_eTSAPStatus;
	return M4_SUCCESS;
  //## end ClCCConnection::SetTSAPStatus%923902839.body
}

m4return_t ClCCConnection::NotifyIncidence (eRequestStatus ai_eIncidence)
{
  //## begin ClCCConnection::NotifyIncidence%923902842.body preserve=yes
	return m_pMainSession->NotifyAllServices(ai_eIncidence);
  //## end ClCCConnection::NotifyIncidence%923902842.body
}

m4return_t ClCCConnection::Reconnect (m4pchar_t ai_pcServerIP, m4int_t ai_iServerPort)
{
  //## begin ClCCConnection::Reconnect%927110475.body preserve=yes
	m4return_t iRet;
	m_eTSAPStatus=M4_CONNECTION_RECONNECTING;
	m_pClientMonitor->RemoveConnection((m4handle_t)this);
	iRet=DestroyTSAP();

	if ( m_bSSL )
		ai_iServerPort ++ ;

	m_strCurrentHost=ai_pcServerIP;
	m_iCurrentHostPort=ai_iServerPort;

	m_strHost=ai_pcServerIP;
	m_iHostPort=ai_iServerPort;

	// bg 117797
	m_strAuthServer = ai_pcServerIP ;

	iRet=CreateTSAP(ai_pcServerIP,ai_iServerPort);
	if(iRet==M4_ERROR)
	{
		SetConnStatus(M4_CONNECTION_NOTSAP);
		SetTSAPStatus(M4_CONNECTION_NOTSAP);
		m_cError=M4_ERROR;
	}
	else
	{
		SetConnStatus(M4_CONNECTION_TSAP_OK);
		SetTSAPStatus(M4_CONNECTION_TSAP_OK);
		m_cError=M4_SUCCESS;
	//	m_pClientMonitor->AddConnection((m4handle_t)this); Se hace dentro del CreateTSAP
	}
	return iRet;
  //## end ClCCConnection::Reconnect%927110475.body
}

m4return_t ClCCConnection::Recycle ()
{
  //## begin ClCCConnection::Recycle%942925902.body preserve=yes
	return M4_ERROR ;
  //## end ClCCConnection::Recycle%942925902.body
}

m4bool_t ClCCConnection::CanRecycle ()
{
  //## begin ClCCConnection::CanRecycle%942925903.body preserve=yes
	return M4_FALSE ;
  //## end ClCCConnection::CanRecycle%942925903.body
}

m4return_t ClCCConnection::Destroy ()
{
  //## begin ClCCConnection::Destroy%942925904.body preserve=yes
	return M4_ERROR ;
  //## end ClCCConnection::Destroy%942925904.body
}

m4bool_t ClCCConnection::CanDestroy ()
{
  //## begin ClCCConnection::CanDestroy%942925905.body preserve=yes
	return M4_FALSE ;
  //## end ClCCConnection::CanDestroy%942925905.body
}

m4return_t ClCCConnection::SetRecursiveDelete (m4bool_t ai_bRecursive)
{
  //## begin ClCCConnection::SetRecursiveDelete%947781333.body preserve=yes
	m_bRecursive=ai_bRecursive;
	return M4_SUCCESS; 
  //## end ClCCConnection::SetRecursiveDelete%947781333.body
}

m4return_t ClCCConnection::_GetAnswerData (ClCCRequest *ai_pRequest, m4bool_t &ao_bReSend)
{
  //## begin ClCCConnection::_GetAnswerData%950783982.body preserve=yes
	m4uint32_t uiControlTimeout=ai_pRequest->m_uiTimeout;
	m4uint32_t uiPopTimeout;
	ClPDU * poPDU=NULL;
	m4return_t iRet=M4_SUCCESS;
	ClTSAP *pTSAP;
	m4bool_t bRetry=M4_TRUE;
	m4Trace(cout << "\rReading PDU     "<<flush);
	while(bRetry)
	{
		switch(m_iCommsType)
		{
		case M4_ADVANCED_COMMS_TYPE:
			
			 uiPopTimeout=uiControlTimeout/1000 ;
			if(uiPopTimeout==0)
				uiPopTimeout=1;
			iRet=m_PDUQueue.Pop(poPDU,uiPopTimeout);
			if(NULL==poPDU)
			{
				if(iRet==M4_WARNING) 
				{	m4Trace(cerr<<"Timeout!!!!"<< endl);}
				else
				{
					iRet=M4_ERROR;
				}
			}
			ao_bReSend = M4_FALSE;
		break;

		case M4_CLASSIC_COMMS_TYPE:
			uiControlTimeout=uiControlTimeout?uiControlTimeout:-1;
			ai_pRequest -> GetCondition() -> Wait(uiControlTimeout);
			return M4_SUCCESS;
		break;
		case M4_RAW_COMMS_TYPE:
			{
				GetTSAP(pTSAP);
				int iSelectReturn=SingleSelect(pTSAP->GetSystemDescriptor(),uiControlTimeout/1000,(uiControlTimeout%1000)*1000);
				switch(iSelectReturn)
				{
				case 1:
					iRet=_GetAnswerPDU(poPDU, ao_bReSend, uiControlTimeout, ai_pRequest->m_bRetrying);
					break;
				case -1:
					m4Trace(cerr << "Error en SingleSelect "<<endl);
					iRet=M4_ERROR;
					break;
				case 0:
					iRet=M4_ERROR;
					m4Trace(cerr << "Timeout"<<endl);
					break;
				default:
					m4Trace(cerr << "Unkown error "<< iSelectReturn<<endl);
					iRet=M4_ERROR;
					break;
				}
				ReleaseTSAP();
			}
		break;
		}
				
		switch(iRet)
		{
		case M4_SUCCESS:
		{
			if(poPDU->GetVersion()==0)
				m_oProtocol.SetVersion(0);

			m4int_t	requestIdfromPDU = poPDU -> GetRequestId();
			m4int_t	requestIdfromRequest= ai_pRequest->GetRequestId ();
			if(0==requestIdfromPDU )
			{
				GetEvent(poPDU);		// Por aqui no va a pasar nunca, ya se ha procesado en ProccessPDU
				return M4_SUCCESS;
			}
			else
			{
				if(requestIdfromPDU!=requestIdfromRequest)
				{
					m4Trace(cerr <<"\nPOP RequestIDfromPDU="<<requestIdfromPDU);
					m4Trace(cerr <<" RequestIDfromRequest="<<requestIdfromRequest);
					m4Trace(cerr <<" RequestIDfromInputPDU="<<ai_pRequest->GetInputPDU()->GetRequestId ()<<endl);
					iRet=M4_ERROR;
					continue;
				}
				else
					bRetry=M4_FALSE;

				m4uint16_t pduType = poPDU -> GetTypePDU();
				m4uint16_t pduId=poPDU->GetIdPDU();
				ai_pRequest->SetPDUOutput(poPDU);
						
				switch(pduId)
				{
				case M4_ID_PDU_ACK:
					m4Trace(cout << "\rReaded  OK !                 \n"<<flush);
					ai_pRequest -> SetState(REPLY_ARRIVED);
					break;
				case M4_ID_PDU_ERROR:
				case M4_ID_PDU_NEWERROR:
					m4Trace(cout << "\rReaded   Error!                 \n"<<flush);
					ai_pRequest -> SetState(ERROR_ARRIVED);
					break;
				default:
					if (M4_TYPE_PDU_CONTROL != pduType)
					{
						ai_pRequest-> SetState(REPLY_ARRIVED);
						m4Trace(cout << "\r Readed  OK !                 \n"<<flush);
						m4uint32_t uiCompressionType=GetCompressionType();
						
                        iRet=_DecompressData(poPDU, uiCompressionType);
                        if(M4_ERROR==iRet)
						{
							m4Trace(cerr << "Error decompressing Data"<<endl);
							ai_pRequest->SetState(BROKEN_CONNECTION);
							bRetry=M4_FALSE;
						}
					}
					else
					{
						m4Trace(cerr << "\rERROR invalid PDU: Id:" << pduId <<endl);
						ai_pRequest-> SetState(BROKEN_CONNECTION);
						bRetry=M4_FALSE;
					}
					break;
				}

			}
		}
		break;
		case M4_ERROR:
			ai_pRequest-> SetState(BROKEN_CONNECTION);
			bRetry=M4_FALSE;
		break;
		case M4_WARNING:
			bRetry=M4_FALSE;
			break;
		default:
			bRetry=M4_FALSE;
			break;
		}
	}
	return iRet;
  //## end ClCCConnection::_GetAnswerData%950783982.body
}

m4return_t ClCCConnection::_SendData (ClCCRequest *ai_pRequest)
{
  //## begin ClCCConnection::_SendData%950783983.body preserve=yes
	m4return_t iRet;

	ai_pRequest->SetState(SENDING);
		// Ahora serializo la PDU y la mando al servidor.
	ClPDU*  pPDU  = ai_pRequest -> GetInputPDU();

	m4uint16_t pduType = pPDU -> GetTypePDU();
//	m4uint16_t pduId=ao_poPDU->GetIdPDU();

	if (M4_TYPE_PDU_CONTROL != pduType)
	{
		m4uint32_t uiCompressionType=GetCompressionType();
		m4uint32_t uiCompressionLevel=M4_COMPRESSION_CLASSIC_DEFAULT_LEVEL;	

        iRet=_CompressData(pPDU,uiCompressionType,uiCompressionLevel);
		if (M4_ERROR==iRet)
		{
			ai_pRequest->SetState(BROKEN_CONNECTION);
			return iRet;
		}
	}	

	m4Trace(cout << "Writting PDU" <<flush);
	switch(m_iCommsType)
	{
	case M4_ADVANCED_COMMS_TYPE:
		{
			if((GetConnStatus()==M4_CONNECTION_CLOSING) || (GetConnStatus()==M4_CONNECTION_FINISHING))
			{
				ai_pRequest->SetState(BROKEN_CONNECTION);
				SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Connection interrupted.");
				return M4_ERROR;
			}
			
			ClPDU*	pPDU  = ai_pRequest -> GetInputPDU();
			iRet=m_pComms->Write(m_hCommHandle,pPDU->GetRequestId(),pPDU);
			if(GetConnStatus()==M4_CONNECTION_START_CLOSING)
				SetConnStatus(M4_CONNECTION_FINISHING);
		}
		break;
	case M4_RAW_COMMS_TYPE:
			iRet= _SendRawData(ai_pRequest);
		break;
	case M4_CLASSIC_COMMS_TYPE:
	{
		ClCondition* pCondition = new ClCondition();
		pCondition -> Init();
		
		ai_pRequest -> SetCondition(pCondition);

		ClCCMonitor *theMonitor;
		ClCCMonitor::GetClientMonitor(theMonitor);

		theMonitor -> GetSendQueue() -> Push(ai_pRequest);
		iRet=M4_SUCCESS;
	}
	break;
	}
	if(M4_SUCCESS==iRet)
	{
		ai_pRequest -> SetState(WAITING_FOR_REPLY);
		m4Trace(cout << "\rWritten PDU  " <<flush);
	}
	else
	{
		ai_pRequest -> SetState(BROKEN_CONNECTION);
		m4Trace(cout << "\rError Writting PDU  " <<flush);
	}

	return iRet;
  //## end ClCCConnection::_SendData%950783983.body
}

m4return_t ClCCConnection::_SendRawData (ClCCRequest *ai_pRequest)
{
  //## begin ClCCConnection::_SendRawData%950783987.body preserve=yes
	ClPDU*  pPDU ;
	pPDU = ai_pRequest -> GetInputPDU();
	ClTSAP *ai_pTSAP;					
	GetTSAP(ai_pTSAP);
	
	int	iSent;
	m4uint32_t  iSizeBlock;
	m4uint32_t  iTotalSize = 0 ;
	m4pchar_t   pBufferPDU;
	eRequestStatus iStatus ;
	
	int bloque = 0;

	//m4Trace(cout <<"Sending PDU .."<< pPDU->GetRequestId()<<flush);
	if( M4_SUCCESS == pPDU -> GetNextBlock( iTotalSize , pBufferPDU , iSizeBlock ) )
	{
		m4TraceLevel(2,cout<<"Sent-Chunk:"<<iSizeBlock<<endl);
		iSent = ai_pTSAP -> Write ( pBufferPDU , iSizeBlock );
		
		if ( (int)iSizeBlock != iSent )
		{
			// Se ha producido un error y hay que abortar;
			m4Trace(cerr << "Se ha producido un error al enviar " << iSizeBlock << " se han enviado " <<(m4int32_t)iSent<<endl);
			SETCODE(M4_ERR_COMUNIC_ERROR_IN_SENDER,ERRORLOG,"Comunnication error in Sender");
			return M4_ERROR;
		}
		
		if(ai_pRequest->m_poDebugFile!= NULL)
		{
			m4TraceLevel(2,cout << "Escribiendo en debugfile" <<endl);
			ai_pRequest->m_poDebugFile->write( pBufferPDU , iSizeBlock );
		}
		
		iTotalSize += iSizeBlock ;
		bloque++;
	}
	
	
	while ( M4_SUCCESS == pPDU -> GetNextBlock( iTotalSize , pBufferPDU , iSizeBlock ) )
	{
		iStatus = ai_pRequest -> GetState ( )  ;
		
		m4pchar_t partialbuffer=pBufferPDU;
		m4int32_t partialBufferSize=(iSizeBlock>M4_MAX_CHUNK_SIZE)? M4_MAX_CHUNK_SIZE:iSizeBlock;
		m4uint32_t alreadySent=0;
		
		while(alreadySent<iSizeBlock)
		{
			m4TraceLevel(2,cout<<"Sent-Chunk:"<<iSizeBlock<<endl);
			iSent = ai_pTSAP -> Write ( partialbuffer , partialBufferSize );
			
			if ( partialBufferSize != iSent )
			{
				// Se ha producido un error;
				switch(iSent)
				{
				case -2:
					if(partialBufferSize>1024)
					{
						partialBufferSize/=2;
						m4Trace(cout << "New ChunkSize " << partialBufferSize<<endl);
						SETCODEF(M4_ERR_COMUNIC_ERROR_IN_SENDER,DEBUGINFOLOG,"New ChunkSize %d",partialBufferSize);
					}
					else
					{
						SETCODE(M4_ERR_COMUNIC_ERROR_IN_SENDER,ERRORLOG,"Comunnication error in Sender:Cannot sent 1024 bytes.");
						return M4_ERROR;
					}
					break;
				case -1:
					m4Trace(cout << "\nError en _SendRawData"<<endl);
					m4Trace(cout << "\niSizeBlock : " << partialBufferSize);
					m4Trace(cout << "\niSent    : " << iSent);
					SETCODEF(M4_ERR_COMUNIC_ERROR_IN_SENDER,ERRORLOG,"Comunnication error in Sender: %d",(int)iSent);
					return M4_ERROR;
					break;
				default:
					m4Trace(cout << "\nError en _SendRawData "<<endl);
					m4Trace(cout << "\niSizeBlock : " << partialBufferSize);
					m4Trace(cout << "\niSent    : " << iSent);
					SETCODEF(M4_ERR_COMUNIC_ERROR_IN_SENDER,ERRORLOG,"Comunnication error in Sender: %d",(int)iSent);
					return M4_ERROR;
					break;
				}
			}
			else
			{
				partialbuffer+=partialBufferSize;
				alreadySent+=partialBufferSize;
				partialBufferSize=((iSizeBlock-alreadySent)>partialBufferSize)?partialBufferSize:iSizeBlock-alreadySent;
			}
		}
		
		if(ai_pRequest->m_poDebugFile!= NULL)
		{
			m4TraceLevel(2,cout << "Escribiendo en debugfile" <<endl);
			ai_pRequest->m_poDebugFile->write( pBufferPDU , iSizeBlock );
		}
		iTotalSize += iSizeBlock ;
		bloque++;
	};
	
	m4TraceLevel(2,cout<<"{WriteTotalsize="<<iTotalSize<<":PDUTotalSize="<<pPDU->GetPDUSize()<<"}"<<endl);
	

	return M4_SUCCESS;
  //## end ClCCConnection::_SendRawData%950783987.body
}

m4return_t ClCCConnection::_CompressData (ClPDU *ai_pPDU, m4uint32_t ai_uiCompressType, m4uint32_t ai_uiCompressLevel)
{
  //## begin ClCCConnection::_CompressData%951147243.body preserve=yes
	m4return_t iRet = M4_ERROR;
	M4DataStorage* pDecompressData = ai_pPDU->GetDataStorage(M4_FALSE);

	M4DataStorage* pCompressData = M4DataStorage::GetNewDataStorage();
	
	iRet = M4DSCompress(*pCompressData,*pDecompressData,"Z",ai_uiCompressType,ai_uiCompressLevel);
	if (M4_SUCCESS == iRet)
    {
	    ai_pPDU->SetDataStorage(pCompressData);
    }
    else
    {
        M4DataStorage::DeleteDataStorage(pCompressData);
    }

    return M4_SUCCESS;
  //## end ClCCConnection::_CompressData%951147243.body
}

m4return_t ClCCConnection::_DecompressData (ClPDU *ai_pPDU, m4uint32_t ai_uiCompressType)
{
  //## begin ClCCConnection::_DecompressData%951147244.body preserve=yes
	m4return_t iRet = M4_ERROR;
	M4DataStorage* pCompressData = ai_pPDU->GetDataStorage(M4_TRUE);

	M4DataStorage* pDecompressData = M4DataStorage::GetNewDataStorage();
	
	iRet = M4DSDecompress(*pCompressData,*pDecompressData,"Z",ai_uiCompressType);
	if (M4_SUCCESS == iRet)
    {
	    ai_pPDU->SetDataStorage(pDecompressData);
    }
    else
    {
        M4DataStorage::DeleteDataStorage(pDecompressData);
    }
	
	return M4_SUCCESS;
  //## end ClCCConnection::_DecompressData%951147244.body
}

m4return_t ClCCConnection::_GetAnswerPDU (ClPDU *&ao_pPDU, m4bool_t &ao_bReSend, m4int32_t ai_iTimeout, m4bool_t ai_bRetrying, m4pchar_t ai_pcBuffer, m4uint_t ai_uiBufferSize)
{
  //## begin ClCCConnection::_GetAnswerPDU%951317374.body preserve=yes

	m4return_t iRet;
	ClTSAP *pTSAP;
	iRet=GetTSAP(pTSAP);

	ClGetPDUTSAP* pPDUFactory = new ClGetPDUTSAP(pTSAP, &m_oProtocol);

	iRet=pPDUFactory -> GetPDU(ao_pPDU,ai_pcBuffer,ai_uiBufferSize);

	delete pPDUFactory;

	ao_bReSend = M4_FALSE;

	return iRet;
	
  //## end ClCCConnection::_GetAnswerPDU%951317374.body
}

m4return_t ClCCConnection::GetEvent (ClPDU *ai_poPDU)
{
  //## begin ClCCConnection::GetEvent%959326859.body preserve=yes
		M4DataStorage * poInputDataStorage;
		ClDataUnit     *poDataUnit;
		//Si el ID de request es 0 se trata de una PDU de Evento del server
		poInputDataStorage = ai_poPDU ->GetDataStorage();
		
		poInputDataStorage ->InitializeIteration();//Esto es para empezar a leer 
		//del data storage de entrada
		if (poInputDataStorage->GetNext((ClDataUnitInterface*&)poDataUnit)==M4_SUCCESS )
		{
			m4pchar_t buffer=NULL;
			m4int_t size=0;
			if ((buffer=poDataUnit->GetBuffer())!=NULL)
			{
				size=poDataUnit->GetSize();
				if((size>9) && (!strncmp(buffer,"@@EVENT@@",9)))
				{
					m4Trace(cout<< "EVENTOS!!!!!"<<endl);
					
					IEventHandler *theEH=ClLogBasedEventHandler::GetEHInstance();
					if(theEH)
						((ClLogBasedEventHandler*)theEH)->Deserialize(buffer+9,size-9);
#ifdef _DEBUG
					ClEventMsgHistory theMsgHis;
					theMsgHis.Deserialize(buffer+9,size);
					m4int_t nEvents=theMsgHis.GetSize();
					if(nEvents)
					{
						m4Trace(cout << "Han llegado "<<nEvents<<" eventos" <<endl);
						
					}
#endif
				}
				else
				{
					SETCODEF ( M4_ERR_CONNECTION_ERROR , DEBUGINFOLOG , "Server Shuting down in %s minutes" , buffer  ) ;
					
					if ( m_pEventFunction )		//Si se ha asignado la función para 
					{							//mostrar eventos recibidos del server, se procesan
#ifdef _WINDOWS
						//Lanza un thread para simular que la ventana que se abre es modal
						ClServerEventDisplayer * poDisplayer = new ClServerEventDisplayer ( m_pEventFunction , buffer [0] , buffer +1 ) ;
						poDisplayer-> Start () ;
#else
						m4Trace(cout <<"Server Shuting down in "<< buffer<<  " minutes"<<endl);
#endif
					}
				}
			
			}
		}
	return M4_SUCCESS;

  //## end ClCCConnection::GetEvent%959326859.body
}

m4return_t ClCCConnection::SetServerEventFunction (ClServerEventFunction ai_EventFunction)
{
  //## begin ClCCConnection::SetServerEventFunction%959326860.body preserve=yes
	m_pEventFunction =ai_EventFunction;
	return M4_SUCCESS;
  //## end ClCCConnection::SetServerEventFunction%959326860.body
}

m4return_t ClCCConnection::ReleaseTSAP ()
{
  //## begin ClCCConnection::ReleaseTSAP%973687120.body preserve=yes
	if(NULL==m_poTSAP)
		return M4_ERROR;
	m4tliid_t theTLI=(m4tliid_t )m_poTSAP;
	m_poTSAP=NULL;
	m_eTSAPStatus=M4_CONNECTION_NOTSAP;
	return ClClientAPI::m_poTSAPPool->ReleaseSocket(m_strURL,theTLI);	
  //## end ClCCConnection::ReleaseTSAP%973687120.body
}

m4return_t ClCCConnection::DetectedBrokenConn ()
{
  //## begin ClCCConnection::DetectedBrokenConn%982054917.body preserve=yes
	SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Connection interrupted.");

	m4Trace(cout << "\rBroken connection                 \n"<<flush);
	SetConnStatus(M4_CONNECTION_CLOSING);
	m_iRef=1;
	if(m_pEventFunction)
	{
						
		m4Trace(cerr << "Calling BrokenConnFunction"<<endl);
		m4char_t buffer[2048];

		if((!m_strHost.empty()) && (2048>m_strHost.size()+10))
		{
			sprintf(buffer,"%s:%d",m_strHost.c_str(),m_iHostPort);
#ifdef _WINDOWS
			//Lanza un thread para simular que la ventana que se abre es modal
			ClServerEventDisplayer * poDisplayer = new ClServerEventDisplayer ( m_pEventFunction , 2 , buffer ) ;
			poDisplayer-> Start () ;
#else
			m4Trace(cout <<"Broken connection: "<< buffer<<endl);
#endif
		}
	}
	return M4_SUCCESS;
  //## end ClCCConnection::DetectedBrokenConn%982054917.body
}

m4return_t ClCCConnection::GetCurrentHost (M4ClString &ao_strCurrentHost)
{
  //## begin ClCCConnection::GetCurrentHost%1101820652.body preserve=yes
	ao_strCurrentHost = m_strCurrentHost;
	return M4_SUCCESS;
  //## end ClCCConnection::GetCurrentHost%1101820652.body
}

m4return_t ClCCConnection::SetCurrentHost (const M4ClString &ai_strCurrentHost)
{
  //## begin ClCCConnection::SetCurrentHost%1101820653.body preserve=yes
	m_strCurrentHost = ai_strCurrentHost;
	return M4_SUCCESS;
  //## end ClCCConnection::SetCurrentHost%1101820653.body
}

m4return_t ClCCConnection::GetHost (M4ClString &ao_strHost)
{
  //## begin ClCCConnection::GetHost%1101820654.body preserve=yes
	ao_strHost = m_strHost;
	return M4_SUCCESS;
  //## end ClCCConnection::GetHost%1101820654.body
}

m4return_t ClCCConnection::SetHost (const M4ClString &ai_strHost)
{
  //## begin ClCCConnection::SetHost%1101820655.body preserve=yes
	m_strHost = ai_strHost;
	return M4_SUCCESS;
  //## end ClCCConnection::SetHost%1101820655.body
}

m4return_t ClCCConnection::GetAuthServer (M4ClString &ao_strAuthSrv)
{
  //## begin ClCCConnection::GetAuthServer%1173278265.body preserve=yes
	ao_strAuthSrv = m_strAuthServer ;
	return M4_SUCCESS ;
  //## end ClCCConnection::GetAuthServer%1173278265.body
}

// Class ClClientExecutableInterface 



ClClientExecutableInterface::ClClientExecutableInterface (ClCPDUQueue *ai_PDUQueue)
  //## begin ClClientExecutableInterface::ClClientExecutableInterface%959326861.hasinit preserve=no
      : m_pPDUQueue(NULL)
  //## end ClClientExecutableInterface::ClClientExecutableInterface%959326861.hasinit
  //## begin ClClientExecutableInterface::ClClientExecutableInterface%959326861.initialization preserve=yes
  //## end ClClientExecutableInterface::ClClientExecutableInterface%959326861.initialization
{
  //## begin ClClientExecutableInterface::ClClientExecutableInterface%959326861.body preserve=yes
	m_pPDUQueue=ai_PDUQueue;
  //## end ClClientExecutableInterface::ClClientExecutableInterface%959326861.body
}



//## Other Operations (implementation)
m4return_t ClClientExecutableInterface::ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList)
{
  //## begin ClClientExecutableInterface::ExecuteAction%959326858.body preserve=yes
	if(!strcmp(ai_oActionString,M4_CLIENTAPI_ACTION_READED_PDU))
	{
		m4return_t retValue,iRet;
		iRet=ai_poParamList->GetM4Return(M4_PARAM_RETURN_RESULT,retValue);
		if(M4_ERROR!=iRet)
		{
			if(M4_ERROR==retValue)
			{
				m_pPDUQueue->Push((ClPDU *)NULL);
				if(m_pConnection)
					m_pConnection->DetectedBrokenConn();
				else
				{m4Trace(cerr << "No conn"<<endl);}
			}
			else
			{
				void *thepointer;
				iRet=ai_poParamList->GetPointer(M4_PARAM_READED_PDU,thepointer);
				ai_poParamList->DeleteParam(M4_PARAM_READED_PDU);
				//delete ai_poParamList;
				if(M4_ERROR!=iRet)
				{
					ClPDU * thePDU=(ClPDU *)thepointer;
					if(NULL!=thePDU)
					{
						iRet=ProcessPDU(thePDU);
						if(iRet==M4_SUCCESS)	
							m_pPDUQueue->Push(thePDU);
						switch(m_pConnection->GetConnStatus())
						{
							case M4_CONNECTION_START_CLOSING:
								break; // Esta empezando el cierre
							case M4_CONNECTION_FINISHING:
								return M4_ERROR;
							break;
							default:
								break;
						}
					}
					else
					{m4Trace(cerr<< "NULL PDU"<<endl);}
				}
			}
		}
	}
	else
	{
		m4Trace(cerr <<"Accion desconocida "<<ai_oActionString<<endl);
	}
	return M4_SUCCESS;
  //## end ClClientExecutableInterface::ExecuteAction%959326858.body
}

m4return_t ClClientExecutableInterface::ProcessPDU (ClPDU *ai_poPDU)
{
  //## begin ClClientExecutableInterface::ProcessPDU%963819983.body preserve=yes
	m4int_t iRequestID=ai_poPDU->GetRequestId ();
	if(0==iRequestID)
	{
		M4DataStorage * poInputDataStorage;
		ClDataUnit     *poDataUnit;
		//Si el ID de request es 0 se trata de una PDU de Evento del server
		poInputDataStorage = ai_poPDU ->GetDataStorage();
		
		poInputDataStorage ->InitializeIteration();//Esto es para empezar a leer 
		//del data storage de entrada
		if (poInputDataStorage->GetNext((ClDataUnitInterface*&)poDataUnit)==M4_SUCCESS )
		{
			m4pchar_t buffer=NULL;
			m4int_t size=0;
			if ((buffer=poDataUnit->GetBuffer())!=NULL)
			{
				size=poDataUnit->GetSize();
				if((size>9) && (!strncmp(buffer,"@@EVENT@@",9)))
				{
					m4Trace(cout<< "EVENTOS!!!!!"<<endl);
					
					IEventHandler *theEH=ClLogBasedEventHandler::GetEHInstance();
					if(theEH)
						((ClLogBasedEventHandler*)theEH)->Deserialize(buffer+9,size-9);
#ifdef _DEBUG
					ClEventMsgHistory theMsgHis;
					theMsgHis.Deserialize(buffer+9,size);
					m4int_t nEvents=theMsgHis.GetSize();
					if(nEvents)
					{
						m4Trace(cout << "Han llegado "<<nEvents<<" eventos" <<endl);
					}
#endif
				}
				else
				{
					SETCODEF ( M4_ERR_CONNECTION_ERROR , DEBUGINFOLOG , "Server Shuting down in %s minutes" , buffer  ) ;
					
					if ( m_pConnection&& m_pConnection->m_pEventFunction )		//Si se ha asignado la función para 
					{							//mostrar eventos recibidos del server, se procesan
#ifdef _WINDOWS
						//Lanza un thread para simular que la ventana que se abre es modal
						ClServerEventDisplayer * poDisplayer = new ClServerEventDisplayer ( m_pConnection->m_pEventFunction , buffer [0] , buffer +1 ) ;
						poDisplayer-> Start () ;
#else
						cout <<"Server Shuting down in "<< buffer<<  " minutes"<<endl;
#endif
					}
					else
						cout <<"Server Shuting down in "<< buffer<<  " minutes"<<endl;
				}
				
			}
		}
		return M4_ERROR;
	}
	else
	{
		return M4_SUCCESS;
	}
	
  //## end ClClientExecutableInterface::ProcessPDU%963819983.body
}

m4return_t ClClientExecutableInterface::SetConnection (ClCCConnection *ai_pConnection)
{
  //## begin ClClientExecutableInterface::SetConnection%963819984.body preserve=yes
	m_pConnection=ai_pConnection;
	return M4_SUCCESS;
  //## end ClClientExecutableInterface::SetConnection%963819984.body
}

// Additional Declarations
  //## begin ClClientExecutableInterface%392E2A1603BB.declarations preserve=yes
  //## end ClClientExecutableInterface%392E2A1603BB.declarations

//## begin module%38ABD2A50383.epilog preserve=yes
//## end module%38ABD2A50383.epilog
