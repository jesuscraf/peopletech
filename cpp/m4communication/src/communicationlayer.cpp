//## begin module%38D1FEB7030D.cm preserve=no
//## end module%38D1FEB7030D.cm

//## begin module%38D1FEB7030D.cp preserve=no
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
//## end module%38D1FEB7030D.cp

//## Module: CommunicationLayer%38D1FEB7030D; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\communicationlayer.cpp

//## begin module%38D1FEB7030D.additionalIncludes preserve=no
#include <m4trace.hpp>
//## end module%38D1FEB7030D.additionalIncludes

//## begin module%38D1FEB7030D.includes preserve=yes
#include <commactionsname.h>
#include <blocksynchronization.hpp>
#include <commworker.hpp>
//## end module%38D1FEB7030D.includes

// CriticalSection
#include <criticalsection.hpp>
// m4types
#include <m4types.hpp>
#include <basiclog.hpp>
// m4string
#include <m4string.hpp>
// commsocketpool
#include <commsocketpool.hpp>
// CommBin
#include <commbin.hpp>
// commprotocols
#include <commprotocols.hpp>
// CommAction
#include <commaction.hpp>
// CommTask
#include <commtask.hpp>
// CommunicationLayer
#include <communicationlayer.hpp>
// TransportLayerSynchronous
#include <transportlayersynchronous.hpp>
// CommComun
#include <commcomun.hpp>


//## begin module%38D1FEB7030D.declarations preserve=no
//## end module%38D1FEB7030D.declarations

//## begin module%38D1FEB7030D.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>

void miPropiomsSleep(int ai_iMiliseconds)
{
#ifdef _WINDOWS
		Sleep(ai_iMiliseconds);
#else
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = ai_iMiliseconds*1000;
		(void) select(2, NULL, NULL, NULL, &timeout);
#endif
}
//## end module%38D1FEB7030D.additionalDeclarations


// Class ClCommunicationImplementation 






//## begin ClCommunicationImplementation::m_poImplementation%38F5EBD5020D.role preserve=no  protected: static ClCommunicationImplementation {1 -> 1RHAN}
ClCommunicationImplementation *ClCommunicationImplementation::m_poImplementation = NULL;
//## end ClCommunicationImplementation::m_poImplementation%38F5EBD5020D.role











ClCommunicationImplementation::ClCommunicationImplementation (m4int_t ai_iControlPort, m4int_t ai_iNWorker)
  //## begin ClCommunicationImplementation::ClCommunicationImplementation%955103950.hasinit preserve=no
      : m_bReadyToRun(M4_FALSE), m_iFatalError(0), m_poWorker(NULL), m_iNWorker(1)
  //## end ClCommunicationImplementation::ClCommunicationImplementation%955103950.hasinit
  //## begin ClCommunicationImplementation::ClCommunicationImplementation%955103950.initialization preserve=yes
  ,m_CTL(ai_iControlPort),m_ActionQueue(5000)
  //## end ClCommunicationImplementation::ClCommunicationImplementation%955103950.initialization
{
  //## begin ClCommunicationImplementation::ClCommunicationImplementation%955103950.body preserve=yes
	m_iFatalError=m_CTL.Error();
	if(0!=m_iFatalError)
	{
		M4_SRV_ERROR(0,"Error inicializando CTL, ControlPort %0:s", ai_iControlPort );
	}
	else
	{
		m_iNWorker=ai_iNWorker;
		m4pchar_t pcEnvVar=getenv("M4_COMM_WORKER_NUMBER");
		if(pcEnvVar && strcmp(pcEnvVar,"0") )
		{
			m_iNWorker=atoi(pcEnvVar);
		}
		if(m_iNWorker<1)
			m_iNWorker=1;
		char pcThreadName[20];
		for(int i=0;i<m_iNWorker;i++)
		{
			m_WorkerPool[i]=new ClCommWorker (m_ActionQueue);
			m_WorkerPool[i]->SetID(i);
			sprintf(pcThreadName, "COMMWorker-%d", i);
			m_WorkerPool[i]->Start(pcThreadName);
		}

		m_poWorker=m_WorkerPool[0];

		m_poImplementation=this;
		m_MapMutex.Init();
		m_bReadyToRun=M4_TRUE;
	}
  //## end ClCommunicationImplementation::ClCommunicationImplementation%955103950.body
}


ClCommunicationImplementation::~ClCommunicationImplementation ()
{
  //## begin ClCommunicationImplementation::~ClCommunicationImplementation%955103951.body preserve=yes
    ClCommWorker* poCommWorker = NULL;
	if(m_poImplementation)
	{
		StopCommunicationLayer();
		int counter=0;
		while(ClCommWorker::GetWorkerNumber()>0)
		{
			m4Trace(cerr <<"\rMal:"<< counter++<<flush);
			miPropiomsSleep(100);
		}

		for(int i=0;i<m_iNWorker;i++)
		{
            poCommWorker = m_WorkerPool[i];
            if (NULL != poCommWorker)
            {
			    while(!poCommWorker->IsExit())
			    {
				    m4Trace(cerr << "Muy Mal:" << counter++ << endl);
				    miPropiomsSleep(100);
			    }
			    delete poCommWorker;
                poCommWorker = NULL;
            }
		}
		m_poImplementation=NULL;
	}

	
	// bug 0103300.
	// espero a que el thread del ctl se haya muerto antes de salir.
	// el problema está en que si salimos antes, tenemos un GP al acceder al thread
	// en la clase principal func2 de m4thread.
	while(!m_CTL.IsExit())
	{
		miPropiomsSleep(100);
	}
  //## end ClCommunicationImplementation::~ClCommunicationImplementation%955103951.body
}



//## Other Operations (implementation)
m4return_t ClCommunicationImplementation::Push (ClCommAction *ai_Action)
{
  //## begin ClCommunicationImplementation::Push%955537528.body preserve=yes

	return m_ActionQueue.Push(ai_Action);
  //## end ClCommunicationImplementation::Push%955537528.body
}

m4tliid_t ClCommunicationImplementation::GetTLIID (m4handle_t ai_hConnectionID)
{
  //## begin ClCommunicationImplementation::GetTLIID%956910728.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	m4tliid_t thetliid=NULL;
	if(ite!=m_TLIMap.end())
		thetliid=((*ite).second)->m_TLIid;

	return thetliid;
  //## end ClCommunicationImplementation::GetTLIID%956910728.body
}

m4return_t ClCommunicationImplementation::AddConnectionID (m4handle_t ai_hConnectionID, m4tliid_t ai_TLIID, ClCommProtocol *ai_pCommProtocol, m4string_t ai_strURL)
{
  //## begin ClCommunicationImplementation::AddConnectionID%956910729.body preserve=yes

	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite!=m_TLIMap.end())
	{
		M4_SRV_DEBUG("ConnectionID %0:s ya en mapa",ai_hConnectionID);
		return M4_ERROR;
	}

	ClNetResource * theNetR=new ClNetResource;
    if (NULL == theNetR)
    {
		M4_SRV_DEBUG_N("ERROR: Memory Allocation error.");
        return M4_ERROR;
    }
	if(ai_pCommProtocol)
    {
		theNetR->m_CommProtocol=ai_pCommProtocol;
    }
	else
	{
		delete theNetR;
		M4_SRV_DEBUG_N("ERROR: Not valid Protocol");
		return M4_ERROR;
	}
	theNetR->m_TLIid=ai_TLIID;
	
	if(!ai_strURL.empty())
		theNetR->m_strURL =ai_strURL;
	m_TLIMap[ai_hConnectionID]=theNetR;
	return M4_SUCCESS;
  //## end ClCommunicationImplementation::AddConnectionID%956910729.body
}

m4return_t ClCommunicationImplementation::VerifyConnection (m4handle_t ai_hConectionID, ClParamList * &ao_pParamList, ClCommAction *ai_poAction, m4bool_t ai_bNoError)
{
  //## begin ClCommunicationImplementation::VerifyConnection%956915208.body preserve=yes
	m4return_t iRet=M4_SUCCESS;

	m4tliid_t theTLI;
	ClCommProtocol *theprotocol;
	if(M4_ERROR==GetTLIandProtocol(ai_hConectionID,theTLI,theprotocol))
	{
		M4_SRV_ERROR_N(0,"Conection not found");
		return M4_ERROR;
	}
	iRet=ai_poAction->PrepareTask(ai_hConectionID,theTLI,theprotocol);
	switch(iRet)
	{
	case M4_WARNING:
		ao_pParamList=new ClParamList ;
		ai_poAction->SetParams(ao_pParamList);
		ao_pParamList->SetPointer(M4_PARAM_TLIID,theTLI);
		ao_pParamList->SetPointer(M4_PARAM_COMM_PROTOCOL,theprotocol);
		ao_pParamList->SetInt(M4_PARAM_SESSIONID,ai_hConectionID);
		ao_pParamList->SetInt(M4_PARAM_CONNECTIONID,ai_hConectionID);
	break;
	case M4_SUCCESS:
		ao_pParamList=NULL;
		break;
	case M4_ERROR:
		break;
	}
	return iRet;
  //## end ClCommunicationImplementation::VerifyConnection%956915208.body
}

ClCommProtocol * ClCommunicationImplementation::GetProtocol (m4handle_t ai_hConnectionID)
{
  //## begin ClCommunicationImplementation::GetProtocol%958483734.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	ClCommProtocol *theProtocol=NULL;
	if(ite!=m_TLIMap.end())
		theProtocol=((*ite).second)->m_CommProtocol;

	return theProtocol;
  //## end ClCommunicationImplementation::GetProtocol%958483734.body
}

ClCommunicationImplementation * ClCommunicationImplementation::GetInstance ()
{
  //## begin ClCommunicationImplementation::GetInstance%955613890.body preserve=yes
	return m_poImplementation;
  //## end ClCommunicationImplementation::GetInstance%955613890.body
}

m4return_t ClCommunicationImplementation::GetTLIandProtocol (m4handle_t ai_hConnectionID, m4tliid_t &ao_TLIID, ClCommProtocol * &ao_pProtocol)
{
  //## begin ClCommunicationImplementation::GetTLIandProtocol%958483735.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite!=m_TLIMap.end())
	{
		ao_pProtocol=((*ite).second)->m_CommProtocol;
		ao_TLIID=((*ite).second)->m_TLIid;
		return M4_SUCCESS;
	}

	ao_pProtocol=NULL;
	ao_TLIID=NULL;
	return M4_ERROR;

  //## end ClCommunicationImplementation::GetTLIandProtocol%958483735.body
}

m4return_t ClCommunicationImplementation::RemoveConnectionID (m4handle_t ai_hConnectionID)
{
  //## begin ClCommunicationImplementation::RemoveConnectionID%958548776.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite!=m_TLIMap.end())
	{
		delete ( (ClTSAP*)((*ite).second)->m_TLIid ) ;
		delete ((*ite).second);
		m_TLIMap.erase(ite);
		return M4_SUCCESS;
	}

	return M4_ERROR;
  //## end ClCommunicationImplementation::RemoveConnectionID%958548776.body
}

m4return_t ClCommunicationImplementation::AddReadingTask (m4handle_t ai_hConnectionID, ClCommReadingTask *ai_pReadingTask)
{
  //## begin ClCommunicationImplementation::AddReadingTask%958548780.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite==m_TLIMap.end())
	{
		M4_SRV_ERROR(0,"ConnectionID %0:s not found",ai_hConnectionID);
		return M4_ERROR;
	}

	ClNetResource * theNetR=(*ite).second;
	theNetR->m_ReadingTask=ai_pReadingTask;

	return M4_SUCCESS;
  //## end ClCommunicationImplementation::AddReadingTask%958548780.body
}

m4return_t ClCommunicationImplementation::GetReadingTask (m4handle_t ai_hConnectionID, ClCommReadingTask * &ao_pReadingTask)
{
  //## begin ClCommunicationImplementation::GetReadingTask%958548781.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite==m_TLIMap.end())
	{
		ao_pReadingTask=NULL;
		M4_SRV_ERROR(0,"ConnectionID %0:s not found",ai_hConnectionID);
		return M4_ERROR;
	}

	ClNetResource * theNetR=(*ite).second;
	if(NULL==theNetR->m_ReadingTask)
	{
		ao_pReadingTask=NULL;
		return M4_ERROR;
	}

	ao_pReadingTask=theNetR->m_ReadingTask;

	return M4_SUCCESS;
  //## end ClCommunicationImplementation::GetReadingTask%958548781.body
}

m4return_t ClCommunicationImplementation::StopCommunicationLayer ()
{
  //## begin ClCommunicationImplementation::StopCommunicationLayer%958646098.body preserve=yes
	if(m_bReadyToRun)
	{
		int i;
		for(i=0;i<m_iNWorker;i++)
		{
			m_WorkerPool[i]->Stop();
			Push((ClCommAction *)NULL);
		}

		while(ClCommWorker::GetWorkerNumber()>1)
		{
			if(m_ActionQueue.Size()<50)
				Push((ClCommAction *)NULL);
			else
				break;
			miPropiomsSleep(100);
		}
		m_bReadyToRun=M4_FALSE;
	}
	return M4_SUCCESS;
  //## end ClCommunicationImplementation::StopCommunicationLayer%958646098.body
}

m4return_t ClCommunicationImplementation::AddConnectionID (m4handle_t ai_hConnectionID, m4tliid_t ai_TLIID, m4int_t ai_iCommProtocolType, m4string_t ai_strURL)
{
  //## begin ClCommunicationImplementation::AddConnectionID%958981534.body preserve=yes
	ClCommProtocol *theProtocol;
	switch(ai_iCommProtocolType)
	{
	case M4_COMM_PDU_HTTP_PROTOCOL:
		M4_SRV_DEBUG_X_N(2,"Using PDU HTTP Protocol");
		theProtocol=new ClCommPDUHTTPProtocol;
		break;
	case M4_COMM_PDU_PROTOCOL:
		M4_SRV_DEBUG_X_N(2,"Using PDU Protocol");
		theProtocol=new ClCommPDUProtocol;
		break;
	case M4_COMM_ASCII_PROTOCOL:
		M4_SRV_DEBUG_X_N(2,"Using ASCII Protocol");
		theProtocol=new ClCommASCIIProtocol;
		break;
	default:
		M4_SRV_DEBUG("Invalid CommProtocol %0:s",ai_iCommProtocolType);
		return M4_ERROR;
		break;
	}

	return AddConnectionID(ai_hConnectionID,ai_TLIID,theProtocol,ai_strURL);
  //## end ClCommunicationImplementation::AddConnectionID%958981534.body
}

m4return_t ClCommunicationImplementation::AddPort (m4int_t ai_iPort, ClCommTask *ai_poTask)
{
  //## begin ClCommunicationImplementation::AddPort%959072021.body preserve=yes
	if(M4_ERROR==IsPortInUse(ai_iPort))
	{
		m_AcceptedPortsMap[ai_iPort]=ai_poTask;
		return M4_SUCCESS;
	}
	else
		return M4_ERROR;
  //## end ClCommunicationImplementation::AddPort%959072021.body
}

m4return_t ClCommunicationImplementation::RemovePort (m4int_t ai_iPort)
{
  //## begin ClCommunicationImplementation::RemovePort%959072022.body preserve=yes
	m4return_t iRet;
	AcceptedPorts::iterator ite;
	ite=m_AcceptedPortsMap.find(ai_iPort);
	if(ite==m_AcceptedPortsMap.end())
		iRet=M4_ERROR;
	else
	{
		m_AcceptedPortsMap.erase(ite);
		iRet=M4_SUCCESS;
	}
	return iRet;
  //## end ClCommunicationImplementation::RemovePort%959072022.body
}

m4return_t ClCommunicationImplementation::IsPortInUse (m4int_t ai_iPort)
{
  //## begin ClCommunicationImplementation::IsPortInUse%959072023.body preserve=yes
	m4return_t iRet;
	AcceptedPorts::iterator ite;
	ite=m_AcceptedPortsMap.find(ai_iPort);
	if(ite==m_AcceptedPortsMap.end())
		iRet=M4_ERROR;
	else
		iRet=M4_SUCCESS;
	return iRet;
  //## end ClCommunicationImplementation::IsPortInUse%959072023.body
}

ClCommTask * ClCommunicationImplementation::GetPortTask (m4int_t ai_iPort)
{
  //## begin ClCommunicationImplementation::GetPortTask%959072024.body preserve=yes
	AcceptedPorts::iterator ite;
	ite=m_AcceptedPortsMap.find(ai_iPort);
	if(ite==m_AcceptedPortsMap.end())
		return NULL;
	else
		return (*ite).second;
  //## end ClCommunicationImplementation::GetPortTask%959072024.body
}

m4return_t ClCommunicationImplementation::GetURL (m4handle_t ai_hConnectionID, m4string_t& ao_strURL)
{
  //## begin ClCommunicationImplementation::GetURL%959072026.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite!=m_TLIMap.end())
	{
		ao_strURL=((*ite).second)->m_strURL;
		return M4_SUCCESS;
	}

	return M4_ERROR;
  //## end ClCommunicationImplementation::GetURL%959072026.body
}

ClTransportLayerSynchronous & ClCommunicationImplementation::GetCTL ()
{
  //## begin ClCommunicationImplementation::GetCTL%964095362.body preserve=yes
	return m_CTL;
  //## end ClCommunicationImplementation::GetCTL%964095362.body
}

m4return_t ClCommunicationImplementation::GetWorkerStats (m4int_t ai_iWorkerID, m4uint_t &ao_uiExecutedActions, m4uint_t &ao_uiExecutingTime, m4uint_t &ao_uiWaitingTime, m4uint_t &ao_uiExternalExecutingTime)
{
  //## begin ClCommunicationImplementation::GetWorkerStats%964423558.body preserve=yes

	ClCommWorker *poWorker=m_WorkerPool[ai_iWorkerID];
	if(NULL!=poWorker)
	{
		ao_uiExecutedActions=poWorker->GetExecutedActions ();
		ao_uiExecutingTime=poWorker->GetExecutionTime ();
		ao_uiWaitingTime=poWorker->GetWaitingTime ();
		ao_uiExternalExecutingTime=poWorker->GetExternalExecutingTime ();
		return M4_SUCCESS;
	}
	else
	{
		M4_SRV_ERROR(0,"Invalid WorkerID %0:s", ai_iWorkerID );
		return M4_ERROR;
	}

	
  //## end ClCommunicationImplementation::GetWorkerStats%964423558.body
}

m4return_t ClCommunicationImplementation::AddWrittingTask (m4handle_t ai_hConnectionID, ClCommWrittingTask *ai_pWrittingTask)
{
  //## begin ClCommunicationImplementation::AddWrittingTask%965806163.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite==m_TLIMap.end())
	{
		M4_SRV_ERROR(0,"ConnectionID %0:s not found",ai_hConnectionID);
		return M4_ERROR;
	}

	ClNetResource * theNetR=(*ite).second;
	M4_ASSERT(theNetR->m_WrittingTask==NULL);
	theNetR->m_WrittingTask=ai_pWrittingTask;

	return M4_SUCCESS;
  //## end ClCommunicationImplementation::AddWrittingTask%965806163.body
}

m4return_t ClCommunicationImplementation::GetWrittingTask (m4handle_t ai_hConnectionID, ClCommWrittingTask * &ao_pWrittingTask)
{
  //## begin ClCommunicationImplementation::GetWrittingTask%965806164.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite==m_TLIMap.end())
	{
		ao_pWrittingTask=NULL;
		M4_SRV_ERROR(0,"ConnectionID %0:s not found",ai_hConnectionID);
		return M4_ERROR;
	}

	ClNetResource * theNetR=(*ite).second;
	ao_pWrittingTask=theNetR->m_WrittingTask;
	if(NULL==ao_pWrittingTask)
	{
		ao_pWrittingTask=(ClCommWrittingTask*)m_oBin.Get(M4_COMM_WRITTING_TASK);
		ao_pWrittingTask->SetState(START_WRITTING);
		ao_pWrittingTask->SetConnectionID(ai_hConnectionID);
		ao_pWrittingTask->SetCommProtocol(theNetR->m_CommProtocol);
		ao_pWrittingTask->SetTLIID(theNetR->m_TLIid);
		theNetR->m_WrittingTask=ao_pWrittingTask;
		return M4_SUCCESS;
	}
	else
	{
		eCommTaskState etheState=ao_pWrittingTask->GetState();
		if(etheState!=NONE)
		{
		//	ao_pWrittingTask=NULL;
			return M4_ERROR;
		}
		else
			ao_pWrittingTask->SetState(START_WRITTING);
		return M4_SUCCESS;
	}
  //## end ClCommunicationImplementation::GetWrittingTask%965806164.body
}

m4return_t ClCommunicationImplementation::AddWorker ()
{
  //## begin ClCommunicationImplementation::AddWorker%967794005.body preserve=yes
	WorkerMap::iterator Iterator ;
	ClCommWorker * poWorker ;
	m4uint32_t iNumWorker ;
	m4char_t pcThreadName [20] ;


	iNumWorker = m_WorkerPool.size () ;

	sprintf(pcThreadName,"COMMWorker-%d",iNumWorker );

	poWorker = new ClCommWorker ( m_ActionQueue ) ;

	poWorker -> SetID ( iNumWorker ) ;

	poWorker -> Start ( pcThreadName ) ;

	m_WorkerPool [iNumWorker] = poWorker ;

	return M4_SUCCESS;
  //## end ClCommunicationImplementation::AddWorker%967794005.body
}

m4return_t ClCommunicationImplementation::RemoveWorker ()
{
  //## begin ClCommunicationImplementation::RemoveWorker%967794006.body preserve=yes
	WorkerMap::iterator Iterator ;
	ClCommWorker * poWorker ;
	m4uint32_t iNumWorker ;


	iNumWorker = m_WorkerPool.size () ;

	Iterator = m_WorkerPool.find (iNumWorker - 1);

	if ( Iterator == m_WorkerPool.end() )
		return M4_ERROR ;

	poWorker = (*Iterator).second ;

	poWorker -> Terminate ( M4_TRUE ) ;

	m_WorkerPool.erase ( Iterator ) ;

	return M4_SUCCESS;
  //## end ClCommunicationImplementation::RemoveWorker%967794006.body
}

m4int_t ClCommunicationImplementation::GetNWorker ()
{
  //## begin ClCommunicationImplementation::GetNWorker%967794010.body preserve=yes
	return m_WorkerPool.size();
  //## end ClCommunicationImplementation::GetNWorker%967794010.body
}

m4int_t ClCommunicationImplementation::GetNPendingActions ()
{
  //## begin ClCommunicationImplementation::GetNPendingActions%967794015.body preserve=yes
	return m_ActionQueue.Size();
  //## end ClCommunicationImplementation::GetNPendingActions%967794015.body
}

m4return_t ClCommunicationImplementation::SetNSocketPool (m4string_t &ai_strURL, m4int_t ai_iNSocket)
{
  //## begin ClCommunicationImplementation::SetNSocketPool%971164984.body preserve=yes
//	return m_SocketPool.SetNSocketPool (ai_strURL, ai_iNSocket);
	return M4_ERROR;
  //## end ClCommunicationImplementation::SetNSocketPool%971164984.body
}

m4return_t ClCommunicationImplementation::SetProtocolVersion (m4handle_t ai_hConnectionID, m4int_t ai_iProtocolVersion)
{
  //## begin ClCommunicationImplementation::SetProtocolVersion%973247122.body preserve=yes
	ClCommProtocol *theProtocol=GetProtocol(ai_hConnectionID);
	if(NULL!=theProtocol)
		return theProtocol->SetVersion(ai_iProtocolVersion);
	else
		return M4_ERROR;

  //## end ClCommunicationImplementation::SetProtocolVersion%973247122.body
}

m4return_t ClCommunicationImplementation::GetURLAndHostName (m4handle_t ai_hConnectionID, m4string_t& ao_strURL, m4string_t &ao_strHostName)
{
  //## begin ClCommunicationImplementation::GetURLAndHostName%977486321.body preserve=yes
	ClBlockSync	oBlocker(m_MapMutex);

	ClTLIMap::iterator ite=	m_TLIMap.find(ai_hConnectionID);
	if(ite!=m_TLIMap.end())
	{
		ao_strURL=((*ite).second)->m_strURL;
		m4tliid_t theTLIID=((*ite).second)->m_TLIid;
		char buffer[256];
		if(M4_SUCCESS==m_CTL.GetTLIHostname(theTLIID,buffer,sizeof(buffer)))
		{
			ao_strHostName=buffer;
			return M4_SUCCESS;
		}
		else
			return M4_ERROR;
	}

	return M4_ERROR;
  //## end ClCommunicationImplementation::GetURLAndHostName%977486321.body
}

m4bool_t ClCommunicationImplementation::IsShuttingDown ()
{
  //## begin ClCommunicationImplementation::IsShuttingDown%988885058.body preserve=yes
	return M4_FALSE;
  //## end ClCommunicationImplementation::IsShuttingDown%988885058.body
}

// Additional Declarations
  //## begin ClCommunicationImplementation%38DB2BE001D1.declarations preserve=yes
m4bool_t ClCommunicationImplementation::IsShuttedDown ()
{
	if(m_bReadyToRun)
		return M4_FALSE;
	else
	{
		for(int i=0;i<m_iNWorker;i++)
			if(!m_WorkerPool[i]->IsExit())
				return M4_FALSE;
		return M4_TRUE;
	}
}
  //## end ClCommunicationImplementation%38DB2BE001D1.declarations

// Class ClNetResource 






ClNetResource::ClNetResource ()
  //## begin ClNetResource::ClNetResource%958548777.hasinit preserve=no
      : m_TLIid(NULL), m_CommProtocol(NULL), m_ReadingTask(NULL), m_WrittingTask(NULL)
  //## end ClNetResource::ClNetResource%958548777.hasinit
  //## begin ClNetResource::ClNetResource%958548777.initialization preserve=yes
  //## end ClNetResource::ClNetResource%958548777.initialization
{
  //## begin ClNetResource::ClNetResource%958548777.body preserve=yes
  //## end ClNetResource::ClNetResource%958548777.body
}


ClNetResource::~ClNetResource ()
{
  //## begin ClNetResource::~ClNetResource%958548778.body preserve=yes

	if(NULL!=m_ReadingTask)
		ClCommunicationImplementation::GetInstance()->m_oBin.Release(m_ReadingTask);
	if(NULL!=m_WrittingTask)
	{
		eCommTaskState thestate=m_WrittingTask->GetState();
		if ( (thestate!=START_WRITTING) && (thestate!=WRITTING) && (thestate!=FINISHING_WRITTING))
		{
			m_WrittingTask->SetState(CLOSING);
			if(NULL!=m_CommProtocol)
			{
				delete m_CommProtocol;
			}
			ClCommunicationImplementation::GetInstance()->m_oBin.Release(m_WrittingTask);
		}
		else
		{
			m_WrittingTask->SetState(CLOSING);
		}
	}
	else
	{
		if(NULL!=m_CommProtocol)
		{
			delete m_CommProtocol;
		}
	}

  //## end ClNetResource::~ClNetResource%958548778.body
}


// Additional Declarations
  //## begin ClNetResource%390FF4FF0129.declarations preserve=yes

  //## end ClNetResource%390FF4FF0129.declarations

//## begin module%38D1FEB7030D.epilog preserve=yes
//## end module%38D1FEB7030D.epilog
