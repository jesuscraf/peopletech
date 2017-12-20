//## begin module%347958850027.cm preserve=no
//## end module%347958850027.cm

//## begin module%347958850027.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ClientAPI
//	 File:              ClientMonitor.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980313
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
//## end module%347958850027.cp

//## Module: clientmonitor%347958850027; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: e:\mybuild\m4appclient\src\clientmonitor.cpp

//## begin module%347958850027.additionalIncludes preserve=no
//## end module%347958850027.additionalIncludes

//## begin module%347958850027.includes preserve=yes
#include "m4stl.hpp"
#include <clientbasicdef.hpp>
#include <clientreceiverq.hpp>
#include <clientsenderq.hpp>

// Porque Rose falla
#include <clientreceiver.hpp>
#include <clientsender.hpp>

class ClClientAPI;
//## end module%347958850027.includes

// clientmonitor
#include <clientmonitor.hpp>
// syncro
#include <syncro.hpp>
// m4types
#include <m4types.hpp>
// m4log
#include <m4log.hpp>

class ClCCService;
class ClCCRequest;
class ClCCUser;
class ClCCConnection;
class ClCCSession;

//## begin module%347958850027.declarations preserve=no
//## end module%347958850027.declarations

//## begin module%347958850027.additionalDeclarations preserve=yes
#define MAX_SIZE_REG_DATA 255
//## end module%347958850027.additionalDeclarations


// Class ClCCMonitor 


//## begin ClCCMonitor::m_TSAPIsReady%34CCC324001B.attr preserve=no  private: static m4bool_t {VA} M4_FALSE
m4bool_t ClCCMonitor::m_TSAPIsReady = M4_FALSE;
//## end ClCCMonitor::m_TSAPIsReady%34CCC324001B.attr




//## begin ClCCMonitor::m_oMutexInitCSC%35ADB2B8001B.attr preserve=yes  private: static ClMutex {UA} 
ClMutex ClCCMonitor::m_oMutexInitCSC(M4_TRUE);
//## end ClCCMonitor::m_oMutexInitCSC%35ADB2B8001B.attr

//## begin ClCCMonitor::m_iCommunicationType%396ED850003D.attr preserve=no  protected: static m4int_t {UA} 0
m4int_t ClCCMonitor::m_iCommunicationType = 0;
//## end ClCCMonitor::m_iCommunicationType%396ED850003D.attr

//## begin ClCCMonitor::m_bFirstTimeCommInit%39F6FA4E03DC.attr preserve=no  private: static m4bool_t {UA} M4_TRUE
m4bool_t ClCCMonitor::m_bFirstTimeCommInit = M4_TRUE;
//## end ClCCMonitor::m_bFirstTimeCommInit%39F6FA4E03DC.attr


//## begin ClCCMonitor::m_pMonitor%348593F100D9.role preserve=no  private: static ClCCMonitor { -> RHAN}
ClCCMonitor *ClCCMonitor::m_pMonitor;
//## end ClCCMonitor::m_pMonitor%348593F100D9.role



m4int_t ClCCMonitor::m_iNRefs=0;






//## begin ClCCMonitor::M4FirstRequestNumber%38AABD7B0138.role preserve=no  private: static m4handle_t {1 -> 1UHANC}
const m4handle_t  ClCCMonitor::M4FirstRequestNumber = 0x10000000;
//## end ClCCMonitor::M4FirstRequestNumber%38AABD7B0138.role



ClCCMonitor::ClCCMonitor ()
  //## begin ClCCMonitor::ClCCMonitor%879852433.hasinit preserve=no
  //## end ClCCMonitor::ClCCMonitor%879852433.hasinit
  //## begin ClCCMonitor::ClCCMonitor%879852433.initialization preserve=yes
  //## end ClCCMonitor::ClCCMonitor%879852433.initialization
{
  //## begin ClCCMonitor::ClCCMonitor%879852433.body preserve=yes

//	GetCommsType ();
//	SetCommsType(m_iCommunicationType);

	m_hRequestNumber=M4FirstRequestNumber;
	m_NRequestMutex.Init();

	m4return_t iRet;

	iRet=m_MutexDicSessions.Init();
	iRet=m_MutexDicRequests.Init();
	iRet=m_MutexDicServices.Init();

	
  //## end ClCCMonitor::ClCCMonitor%879852433.body
}


ClCCMonitor::~ClCCMonitor ()
{
  //## begin ClCCMonitor::~ClCCMonitor%881161849.body preserve=yes

	m4return_t iRet;
	switch(m_iCommunicationType)
	{
	case 	M4_CLASSIC_COMMS_TYPE:
		iRet=m_poDistributor -> Terminate();
		iRet=m_poReceiver    -> Terminate();
		iRet=m_poSender      -> Terminate();
		
		delete m_poReceiverQ;
		delete m_poSendQ;
		
		delete m_poDistributor;
		delete m_poReceiver;
		delete m_poSender;
	case M4_RAW_COMMS_TYPE:
		SocketFinish();
		break;
	default:
		break;
	}
	m_bFirstTimeCommInit=M4_TRUE;
  //## end ClCCMonitor::~ClCCMonitor%881161849.body
}



//## Other Operations (implementation)
m4return_t ClCCMonitor::InitCSC ()
{
  //## begin ClCCMonitor::InitCSC%879852431.body preserve=yes

	if (m_pMonitor == NULL)
	{
		m_oMutexInitCSC.Lock();
		if(m_pMonitor == NULL)
		{
			m_pMonitor = new ClCCMonitor();
			m_iNRefs=1;
			M4_ASSERT(m_pMonitor != NULL);
			if(!m_TSAPIsReady)
			{
				m_oMutexInitCSC.Unlock();
				return M4_ERROR;
			}
		}
		m_oMutexInitCSC.Unlock();
		return M4_SUCCESS;
	}
	else
	{
		SETCODE(M4_WAR_INITCSC,WARNINGLOG, "Monitor alredy initializated."); 
		return M4_WARNING;
	}
  //## end ClCCMonitor::InitCSC%879852431.body
}

m4return_t ClCCMonitor::StopCSC ()
{
  //## begin ClCCMonitor::StopCSC%885808224.body preserve=yes

	if (m_pMonitor == NULL)
		return M4_ERROR;
	m_oMutexInitCSC.Lock();
	m_iNRefs--;
	if(m_iNRefs==0)
	{
		delete m_pMonitor;
		m_pMonitor = NULL;
	}
	m_oMutexInitCSC.Unlock();
	return M4_SUCCESS;

  //## end ClCCMonitor::StopCSC%885808224.body
}

m4return_t ClCCMonitor::GetClientMonitor (ClCCMonitor *&ao_pMonitor)
{
  //## begin ClCCMonitor::GetClientMonitor%880446334.body preserve=yes
	m_oMutexInitCSC.Lock();
	if (NULL == m_pMonitor)
	{
		m_oMutexInitCSC.Unlock();
		InitCSC();
	}
	else
	{
		m_iNRefs++;
		m_oMutexInitCSC.Unlock();
	}

	ao_pMonitor = m_pMonitor;
	
	return M4_SUCCESS;
  //## end ClCCMonitor::GetClientMonitor%880446334.body
}

ClCCSendQ* ClCCMonitor::GetSendQueue ()
{
  //## begin ClCCMonitor::GetSendQueue%881225505.body preserve=yes
	return m_poSendQ;
  //## end ClCCMonitor::GetSendQueue%881225505.body
}

ClCCReceiverQ* ClCCMonitor::GetReceiverQueue ()
{
  //## begin ClCCMonitor::GetReceiverQueue%881685995.body preserve=yes
	return m_poReceiverQ;
  //## end ClCCMonitor::GetReceiverQueue%881685995.body
}

m4return_t ClCCMonitor::GetService (m4handle_t ai_hServiceId, ClCCService *&ao_pService)
{
  //## begin ClCCMonitor::GetService%881257609.body preserve=yes

	m_MutexDicServices.Lock();
	map<m4handle_t, ClCCService*, less<m4handle_t> >::const_iterator iterador;
	iterador = m_dicServices.begin();
	iterador = m_dicServices.find(ai_hServiceId);

	if (iterador == m_dicServices.end())
	{
		ao_pService = NULL;
		m_MutexDicServices.Unlock();
		return M4_ERROR;
	}

	ClCCService* pService = (*iterador).second;
	if (pService == NULL)
	{
		SETCODE(M4_ERR_NULL_SERV_IN_DIC,ERRORLOG,"Inconsistency in internal dictionary\nNULL Service in monitor dictionary");
		m_MutexDicServices.Unlock();
		return M4_ERROR;
	}
	ao_pService = pService;
	m_MutexDicServices.Unlock();

	return M4_SUCCESS;
  //## end ClCCMonitor::GetService%881257609.body
}

m4return_t ClCCMonitor::GetRequest (m4handle_t ai_hRequestId, ClCCRequest *&ao_pRequest)
{
  //## begin ClCCMonitor::GetRequest%881742137.body preserve=yes

	m_MutexDicRequests.Lock();
	map<m4handle_t, ClCCRequest*, less<m4handle_t> >::const_iterator iterador;
	iterador = m_dicRequests.begin();
	iterador = m_dicRequests.find(ai_hRequestId);

	if (iterador == m_dicRequests.end())
	{
		m4TraceLevel(2,cerr <<"No encontre la request\n");
		m_MutexDicRequests.Unlock();
		return M4_ERROR;
	}

	ClCCRequest* pRequest = (*iterador).second;
	if (pRequest == NULL)
	{
		SETCODE(M4_ERR_NULL_REQ_IN_DIC,ERRORLOG,"NULL Request found in monitor dictionary.");
		m_MutexDicRequests.Unlock();
		return M4_ERROR;
	}
	ao_pRequest = pRequest;
	m_MutexDicRequests.Unlock();
	return M4_SUCCESS;
  //## end ClCCMonitor::GetRequest%881742137.body
}

m4return_t ClCCMonitor::GetSession (m4handle_t ai_hSessionId, ClCCSession *&ao_pSession)
{
  //## begin ClCCMonitor::GetSession%889785669.body preserve=yes
	map<m4handle_t, ClCCSession*, less<m4handle_t> >::const_iterator iterador;
	iterador = m_dicSessions.begin();
	iterador = m_dicSessions.find(ai_hSessionId);

	if (iterador == m_dicSessions.end())
	{
		SETCODE(M4_ERR_SESS_NOT_IN_DIC,ERRORLOG,"Session not found in monitor dictionary.\nNo connection to server.");
		return M4_ERROR;
	}

	ClCCSession* pSession = (*iterador).second;
	if (pSession == NULL)
	{
		SETCODE(M4_ERR_NULL_SESS_IN_DIC,ERRORLOG,"NULL Session found in monitor dictionary.\nNo connection to server.");
		return M4_ERROR;
	}
	ao_pSession = pSession;
	return M4_SUCCESS;
  //## end ClCCMonitor::GetSession%889785669.body
}

m4return_t ClCCMonitor::AddService (m4handle_t ai_hServiceId, ClCCService* ai_pService)
{
  //## begin ClCCMonitor::AddService%881686000.body preserve=yes

	m_MutexDicServices.Lock();
	map<m4handle_t, ClCCService*, less<m4handle_t> >::const_iterator iterador;
	iterador = m_dicServices.begin();
	iterador = m_dicServices.find(ai_hServiceId);

	if (iterador != m_dicServices.end())
	{
		m_MutexDicServices.Unlock();
		return M4_ERROR;
	}
	m_dicServices[ai_hServiceId] = ai_pService;
	m_MutexDicServices.Unlock();
	return M4_SUCCESS;
  //## end ClCCMonitor::AddService%881686000.body
}

m4return_t ClCCMonitor::AddRequest (m4handle_t ai_hRequestId, ClCCRequest* ai_pRequest)
{
  //## begin ClCCMonitor::AddRequest%881742135.body preserve=yes

	m_MutexDicRequests.Lock();
	map<m4handle_t, ClCCRequest*, less<m4handle_t> >::const_iterator iterador;
	iterador = m_dicRequests.begin();
	iterador = m_dicRequests.find(ai_hRequestId);

	if (iterador != m_dicRequests.end())
	{
		m_MutexDicRequests.Unlock();
		return M4_ERROR;
	}
	m_dicRequests[ai_hRequestId] = ai_pRequest;
	m_MutexDicRequests.Unlock();
	return M4_SUCCESS;
  //## end ClCCMonitor::AddRequest%881742135.body
}

m4return_t ClCCMonitor::AddSession (m4handle_t ai_hSessionId, ClCCSession *ai_pSession)
{
  //## begin ClCCMonitor::AddSession%882869291.body preserve=yes

	m_MutexDicSessions.Lock();

	map<m4handle_t, ClCCSession*, less<m4handle_t> >::const_iterator iterador;
	iterador = m_dicSessions.begin();
	iterador = m_dicSessions.find(ai_hSessionId);

	if (iterador != m_dicSessions.end())
	{
		m_MutexDicSessions.Unlock();
		SETCODE(M4_ERR_SES_IN_DICT_YET,ERRORLOG,"The session is in the dictionary yet.");
		return M4_ERROR;
	}

	m_dicSessions[ai_hSessionId] = ai_pSession;
	m_MutexDicSessions.Unlock();

	return M4_SUCCESS;

  //## end ClCCMonitor::AddSession%882869291.body
}

m4return_t ClCCMonitor::RemoveService (m4handle_t ai_hServiceId)
{
  //## begin ClCCMonitor::RemoveService%881686001.body preserve=yes
	m_MutexDicServices.Lock();

	m4int16_t ret = m_dicServices.erase(ai_hServiceId);
	m_MutexDicServices.Unlock();
	if(ret!=1)
	{
		m4Trace(cerr<<"Couldn't delete Service from monitor dictionary.");
		SETCODE(M4_ERR_SERV_NOT_DEL_FROM_DICT,ERRORLOG,"Couldn't delete Service from monitor dictionary.");
		return M4_ERROR;
	}
	return M4_SUCCESS;
  //## end ClCCMonitor::RemoveService%881686001.body
}

m4return_t ClCCMonitor::RemoveRequest (m4handle_t ai_hRequestId)
{
  //## begin ClCCMonitor::RemoveRequest%881742136.body preserve=yes

	m_MutexDicRequests.Lock();
	m4int16_t ret = m_dicRequests.erase(ai_hRequestId);
	m_MutexDicRequests.Unlock();
	if (ret != 1)
	{
		m4Trace(cerr <<"Couldn't delete Request from monitor dictionary."<<endl);
		SETCODE(M4_ERR_REQ_NOT_DEL_FROM_DICT,ERRORLOG,"Couldn't delete Request from monitor dictionary.");
		return M4_ERROR;
	}
	
	return M4_SUCCESS;

  //## end ClCCMonitor::RemoveRequest%881742136.body
}

m4return_t ClCCMonitor::RemoveSession (m4handle_t ai_hSessionId)
{
  //## begin ClCCMonitor::RemoveSession%884854703.body preserve=yes
	m_MutexDicSessions.Lock();
	m4int16_t ret = m_dicSessions.erase(ai_hSessionId);
	m_MutexDicSessions.Unlock();
	if (ret !=1)
	{
		m4Trace(cerr <<"Couldn't delete Session from monitor dictionary."<<endl);
		SETCODE(M4_ERR_SES_NOT_DEL_FROM_DICT,ERRORLOG,"Couldn't delete Session from monitor dictionary.");
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClCCMonitor::RemoveSession%884854703.body
}

m4return_t ClCCMonitor::CancelRequest (m4handle_t ai_hRID)
{
  //## begin ClCCMonitor::CancelRequest%879876511.body preserve=yes
	return M4_SUCCESS;
  //## end ClCCMonitor::CancelRequest%879876511.body
}

m4return_t ClCCMonitor::InitTSAP ()
{
  //## begin ClCCMonitor::InitTSAP%885835377.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	
	if (m_TSAPIsReady == M4_TRUE)
		return	iRet;
	
	// Inits the TSAP library
	iRet= SocketInit();
	m_TSAPIsReady=(iRet==M4_SUCCESS)?M4_TRUE:M4_FALSE;
	return iRet;
  //## end ClCCMonitor::InitTSAP%885835377.body
}

m4return_t ClCCMonitor::AddConnection (m4handle_t ai_hConnectionId, ClCCConnection *ai_pConnection)
{
  //## begin ClCCMonitor::AddConnection%890042318.body preserve=yes
	if(m_iCommunicationType!=M4_CLASSIC_COMMS_TYPE)
		return M4_SUCCESS;
	m4return_t iRet;
	if (m_poReceiver)
	{
		iRet=m_poReceiver->AddConnection(ai_pConnection);
		return M4_SUCCESS;
	}
	else
	{
		SETCODE(M4_ERR_NULL_RECIEVER,ERRORLOG," OpenSession failed.\nNULL Receiver in.");
		return M4_ERROR;
	}
  //## end ClCCMonitor::AddConnection%890042318.body
}

m4return_t ClCCMonitor::RemoveConnection (m4handle_t ai_hConnectionId)
{
  //## begin ClCCMonitor::RemoveConnection%890042319.body preserve=yes
	if(m_iCommunicationType!=M4_CLASSIC_COMMS_TYPE)
		return M4_SUCCESS;
	m4return_t iRet;
	if (m_poReceiver)
	{
		iRet=m_poReceiver->RemoveConnection((ClCCConnection *)ai_hConnectionId);
		return iRet;
	}
	else
		return M4_ERROR;
  //## end ClCCMonitor::RemoveConnection%890042319.body
}

m4return_t ClCCMonitor::GetRequestNumber (m4handle_t &ao_hRequestNumber)
{
  //## begin ClCCMonitor::GetRequestNumber%950715095.body preserve=yes
	m4return_t iRet;

	iRet=m_NRequestMutex.Lock();
	if(M4_ERROR==iRet)
		return M4_ERROR;

	ao_hRequestNumber=m_hRequestNumber++;
	if(!m_hRequestNumber)
		m_hRequestNumber=M4FirstRequestNumber;
	iRet=m_NRequestMutex.Unlock();
	if(M4_ERROR==iRet)
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClCCMonitor::GetRequestNumber%950715095.body
}

m4int_t ClCCMonitor::GetCommsType ()
{
  //## begin ClCCMonitor::GetCommsType%963558168.body preserve=yes
	if(M4_NULL_COMMS_TYPE== m_iCommunicationType)
	{
		char *value=getenv("M4_RAW");
		if(NULL==value)
		{
			m_iCommunicationType=M4_ADVANCED_COMMS_TYPE;
			m4Trace(cout << "M4_ADVANCED_COMMS_TYPE"<<endl);
		}
		else
		{
			if(!strcmp(value,"M4_ADVANCED_COMMS_TYPE") )
			{
				m_iCommunicationType=M4_ADVANCED_COMMS_TYPE;
				m4Trace(cout << "Using M4_ADVANCED_COMMS_TYPE"<<endl);
			}
			else
			{
				if(!strcmp(value,"M4_CLASSIC_COMMS_TYPE") )
				{
					m_iCommunicationType=M4_CLASSIC_COMMS_TYPE;
					m4Trace(cout << "Using M4_CLASSIC_COMMS_TYPE"<<endl);
				}
				else
				{
					if(!strcmp(value,"M4_RAW_COMMS_TYPE") )
					{
						m_iCommunicationType=M4_RAW_COMMS_TYPE;
						m4Trace(cout << "Using M4_RAW_COMMS_TYPE"<<endl);
					}
					else
					{
						m_iCommunicationType=M4_ADVANCED_COMMS_TYPE;
						m4Trace(cout << "Using M4_ADVANCED_COMMS_TYPE"<<endl);
					}
				}
			}
		}	
	}
	
	return m_iCommunicationType;
  //## end ClCCMonitor::GetCommsType%963558168.body
}

m4return_t ClCCMonitor::SetCommsType (m4int_t ai_iCommsType)
{
  //## begin ClCCMonitor::SetCommsType%972486059.body preserve=yes
	if(m_bFirstTimeCommInit)
	{
		if(M4_ADVANCED_COMMS_TYPE!=ai_iCommsType)
		{
			if(InitTSAP()==M4_ERROR) 
			{
				m4Trace(cerr<< "Error inicializando TSAP"<<endl);
				SETCODE(M4_ERR_INITSAP,ERRORLOG,"Error starting comunication in ClCCMonitor.");
			}
		}

		if(M4_CLASSIC_COMMS_TYPE==ai_iCommsType)
		{
			m_poReceiverQ   = new ClCCReceiverQ(1000);
			m_poSendQ       = new ClCCSendQ(1000);

			m_poReceiver    = new ClCCReceiver(m_poReceiverQ);
			m_poSender      = new ClCCSender(m_poSendQ);
			m_poDistributor = new ClCCDistributor(m_poReceiverQ);
			
			m_poDistributor -> Start();
			m_poReceiver    -> Start();
			m_poSender      -> Start();
		}
		else
		{
			m_poReceiverQ   = NULL;
			m_poSendQ  =  NULL; 
			m_poDistributor = NULL; 
			m_poReceiver  = NULL; 
			m_poSender = NULL;
		}
		m_bFirstTimeCommInit=M4_FALSE;
	}
	else
	{
		if(ai_iCommsType!=m_iCommunicationType)
		{
			cerr << "Not available"<<endl;
			return M4_ERROR;
		}
	}
	m_iCommunicationType=ai_iCommsType;
	return M4_SUCCESS;
  //## end ClCCMonitor::SetCommsType%972486059.body
}

//## begin module%347958850027.epilog preserve=yes
//## end module%347958850027.epilog
