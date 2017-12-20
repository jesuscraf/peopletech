//## begin module%3507B50C0309.cm preserve=no
//## end module%3507B50C0309.cm

//## begin module%3507B50C0309.cp preserve=no
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
//## end module%3507B50C0309.cp
 
//## Module: Executive_Proxy%3507B50C0309; Package body
//## Subsystem: M4ExeForChannel::Proxy::src%392274340168
//## Source file: D:\eduardoma\m4exeforchannel\src\exeforproxy.cpp

//## begin module%3507B50C0309.additionalIncludes preserve=no
//## end module%3507B50C0309.additionalIncludes

//## begin module%3507B50C0309.includes preserve=yes
#include <sergenproxy.hpp>
#include <exeproxy.hpp>
#include <navigatorf.hpp>
#include <chlog.hpp>
#include <dm.hpp>
#include <channel.hpp>
#include <proxysubsys.hpp>
#include <m4srvefcres.hpp>
#include <m4srvres.hpp>
#include <cloblconfiguration.hpp>
//	Rose 98 Body Include Dependences
#include "blocksynchronization.hpp"
//## end module%3507B50C0309.includes

// executiveforoe
#include <executiveforoe.hpp>
// Executive_Proxy
#include <exeforproxy.hpp>
// ClRequestStatistics
#include <request.hpp>
#include "rawsessionid.hpp"
//## begin module%3507B50C0309.declarations preserve=no
//## end module%3507B50C0309.declarations

//## begin module%3507B50C0309.additionalDeclarations preserve=yes
const m4size_t M4ProxyExecDumpLevel = 4;
//## end module%3507B50C0309.additionalDeclarations


// Class ClExecutiveForProxy 




ClExecutiveForProxy::ClExecutiveForProxy (ClEngine *ai_pEngine)
  //## begin ClExecutiveForProxy::ClExecutiveForProxy%889698662.hasinit preserve=no
      : m_poClServerProxyManager(NULL)
  //## end ClExecutiveForProxy::ClExecutiveForProxy%889698662.hasinit
  //## begin ClExecutiveForProxy::ClExecutiveForProxy%889698662.initialization preserve=yes
	,	ClExecutiveForOE(ai_pEngine)
  //## end ClExecutiveForProxy::ClExecutiveForProxy%889698662.initialization
{
  //## begin ClExecutiveForProxy::ClExecutiveForProxy%889698662.body preserve=yes
	m_iMaxDumpLevel = M4ProxyExecDumpLevel;
//	m_poDumpInfo->SetCurrentStage("Creation State", 0);
	m_pProxySubSystem = NULL;
	m_pStatus = NULL;
	m_pProxySubSystem = NULL;
	m_pStatus = NULL;
  //## end ClExecutiveForProxy::ClExecutiveForProxy%889698662.body
}


ClExecutiveForProxy::~ClExecutiveForProxy ()
{
  //## begin ClExecutiveForProxy::~ClExecutiveForProxy%889698663.body preserve=yes
	ShutDown();
  //## end ClExecutiveForProxy::~ClExecutiveForProxy%889698663.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForProxy::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExecutiveForProxy::StartUp%889698664.body preserve=yes
	m4return_t iRet ;
	m4char_t	szValue [ OBL_PATHVALUE_MAX_LEN ];
	size_t		iLenValue;


   m_poConfiguration=ai_pConfig;

   // Add the info about execution
   ai_pConfig->AddStringProperty("", PROP_EXECUTION_DESCRIPTION, "-");
   ai_pConfig->AddStringProperty("", PROP_OBJECT_LIST, "-");

	// gets the proxy server manager
	m_poClServerProxyManager = (ClServerProxyManager*) ai_pNavigator->GetSubsystemElement("PROXY_SPACES", "SERVER_PROXY_MANAGER" );
	if (m_poClServerProxyManager == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_PROXY_NO_PX_MANAGER, "Cannot find the proxy managerin proxy subsystem") ;
		return M4_ERROR;
	}

	iRet = ai_pConfig -> GetStringValue ( "" , M4ExecutiveDumpFilenameProp,   szValue , OBL_PATHVALUE_MAX_LEN, iLenValue ) ;
	if (iRet == M4_SUCCESS)
	{
		m_strDumpFilename = szValue;
	}
	else
	{
		SETCODEF ( M4_ERR_SCH_INITIALIZING, ERRORLOG , "%s: Cannot find OblProp <%s>" , m_strStringId.c_str(), M4ExecutiveDumpFilenameProp) ;
	}

	m_bInitialized = M4_TRUE;
	return M4_SUCCESS;

  //## end ClExecutiveForProxy::StartUp%889698664.body
}

m4return_t ClExecutiveForProxy::Execute (ClImage &aio_pImage)
{
  //## begin ClExecutiveForProxy::Execute%889698665.body preserve=yes
	RecycleDumpInfo();
	m_poDumpInfo->SetCurrentStage("Initial phase", 0);

	M4_ASSERT(m_poClServerProxyManager != NULL);

	// local variables
	m4return_t iResult;
	ClEngineArgs *poEngineArgs;

	aio_pImage.GetArgs(poEngineArgs);
	M4DataStorage *poInputDataStorage, *poOutputDataStorage;

	iResult = poEngineArgs->GetInputData (poInputDataStorage);
	if (iResult != M4_SUCCESS)	
	{
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_ENGINE_PARAM , "Error getting input data storage from engine args" ) ;
		return iResult;
	}

	iResult = poEngineArgs->GetOutputData (poOutputDataStorage);
	if (iResult != M4_SUCCESS)	
	{
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_ENGINE_PARAM , "Error getting output data storage from engine args" ) ;
		return iResult;
	}

	// gets the session channel
	ClEngineExecEnv *poClEngineExecEnv = NULL;
	aio_pImage.GetEnv(poClEngineExecEnv);

	if (0 == poClEngineExecEnv)
	{
		DUMP_CHLOG_ERRORF(M4_ERR_PROXY_ENGINE_PARAM , "Error getting user session from Environment" ) ;
		return M4_ERROR;
	}

	m4string_t	strUsername;
	poClEngineExecEnv->GetUser()->GetName(strUsername);
	
	ClUSSessionTicketInterface *poTicketInterface;
	iResult = poClEngineExecEnv->GetSessionTicket(poTicketInterface);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForProxy. Cannot get ticket from from environment.");
		return iResult;
	}

	ClChannel *poSessionChannel = (ClChannel *)poTicketInterface->GetTicketCode();
	if (poSessionChannel == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL ,  "ExecutiveForProxy. Cannot get session channel from ticket.");
		return iResult;
	}

	// gets the status
	ClStatus *poClStatus = NULL;
	aio_pImage.GetStatus(poClStatus);

	{
		ClMutBlock oMut(m_oMutAccess);
		m_pStatus = poClStatus;
	}
	// Asignamos el booleancondition para que se pueda hacer pause
	if (m_pStatus != NULL)
		m_pStatus->SetBoolCondIntrVal(m_pCondIntrVal);


	// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
	m4string_t	sFullSessionId, sSessionId, sUnused, sServerHost, sServerPort;
	poClEngineExecEnv->GetSessionID( sFullSessionId );
	ClRawSessionIDGenerator oSessionIDGenerator;
	oSessionIDGenerator.ExtractVariables( sFullSessionId, sUnused, sUnused, sSessionId, sServerHost, sServerPort );

	ClStatistic*	pStats;
	m4string_t		sReqUniqueId;

	aio_pImage.GetStats( pStats );
	aio_pImage.GetReqUniqueId( sReqUniqueId );

	setTracesExtInfo( ( ClRequestStatistics* )pStats, poSessionChannel, sServerHost.c_str(), sServerPort.c_str(), sSessionId.c_str(), sReqUniqueId.c_str(), OLTP_LOG );

	// Starts transaction rows quota control.
	rowsQuotaProlog(poSessionChannel, poTicketInterface, M4_FALSE);


	// La auditoría de errores se registra y se desregistra dentro
	iResult = ExecForProxy(poInputDataStorage, poOutputDataStorage, m_poClServerProxyManager, poSessionChannel, m_poSession->GetId(), strUsername.c_str(), poClStatus, m_poDumpInfo,m_poConfiguration);

	
	// Ends transaction rows quota control.
	rowsQuotaEpilog((ClRequestStatistics*)pStats, poTicketInterface);

	resetTracesExtInfo( OLTP_LOG );


	if (iResult != M4_SUCCESS)
		DUMP_CHLOG_ERRORF ( M4_ERR_PROXY_EXECUTOR , "Error executing proxy executor command" ) ;
	{
		if (m_pStatus != NULL)
		{
			if (m_pStatus->GetIntrValue() != ClStatus::STATUS_INTR_NORMAL)
			{
				m4char_t buffer[128];
				sprintf(buffer, "In proxy executor. Cancelation detected/processed in request id %d.", m_pStatus->GetIdObject() ) ;
				DUMP_CHLOG_ERRORF ( M4_CANCELLATION_DETECTED , buffer);
			}

			// Asignamos el booleancondition para que se pueda hacer pause
			m_pStatus->SetBoolCondIntrVal(NULL);
		    {
			    ClMutBlock oMut(m_oMutAccess);
				m_pStatus = NULL;
	        }
		}
	}

	m_poDumpInfo->clear();
	
	return iResult;
  //## end ClExecutiveForProxy::Execute%889698665.body
}

m4return_t ClExecutiveForProxy::ShutDown ()
{
  //## begin ClExecutiveForProxy::ShutDown%889698666.body preserve=yes

	if (m_bInitialized != M4_TRUE)
		return M4_ERROR;

	if (m_poClServerProxyManager != NULL)
		m_poClServerProxyManager = NULL;

	return M4_SUCCESS;

  //## end ClExecutiveForProxy::ShutDown%889698666.body
}

m4return_t ClExecutiveForProxy::Reset ()
{
  //## begin ClExecutiveForProxy::Reset%889698667.body preserve=yes

	return M4_SUCCESS;

  //## end ClExecutiveForProxy::Reset%889698667.body
}

m4return_t ClExecutiveForProxy::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExecutiveForProxy::ExecuteCommand%922189097.body preserve=yes
	m4return_t iRet;
	
	ClMutBlock oMut(m_oMutAccess);
	
	// Precondicion: si no estoy ejecutando nada, no puedo hacer nada
	if (m_pStatus == NULL)
	{
		DUMP_CHLOG_ERROR(M4_ERR_PROXY_NOT_EXECUTING);
		return M4_ERROR;
	}
	
	switch (ai_cCommand)
	{
	// Comandos heredados del ClExecutive
	case ClExecutive::QUIT:
		iRet = m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_QUIT);
		break;
		
	case ClExecutive::PAUSE:
		iRet = m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_PAUSING);
		break;
		
	case ClExecutive::RESUME:
		iRet = m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);
		break;
		
	case ClExecutive::USED_MEMORY:
		iRet = m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_PAUSING);				// Paro la ejecucion
		if (iRet == M4_SUCCESS)
		{
			// Consigo el ClChannelManager asociado a este ejecutor
			ClProxySpace* pProxySpace = m_poClServerProxyManager->GetSpace(m_poSession->GetId());
			if (pProxySpace == NULL)
			{
				iRet = M4_ERROR;
				break;
			}

			ClChannelManager* pChannelManager = pProxySpace->GetChannelManager();

			m4uint32_t iSize = 0;
			if (pChannelManager != NULL)
			{
				iSize = pChannelManager->GetSize();									// Mido la memoria
//				iRet = m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);		// Continuo la ejecucion
			}
			else
			{
//				m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);			// Continuo la ejecucion
				iRet = M4_ERROR;
			}

			// Preparo el parametro de salida
			m4char_t szSize[256];
			sprintf(szSize, "%0.2f", iSize/1024.0);
			
			ClAdminCommandTag* pAdminCommandTag =
				ClAdminCommandTag::GetNewAdminCommandTag (0, "USED_MEMORY", adminLong, szSize, M4_TRUE);
			
			if (pAdminCommandTag != NULL)
				ao_oResultList.push_back(pAdminCommandTag);
			else
				iRet = M4_ERROR;
		}

		m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);			// Continuo la ejecucion
		break;
		
	default:
		iRet = M4_ERROR;
	}
	
	return iRet;
  //## end ClExecutiveForProxy::ExecuteCommand%922189097.body
}

m4return_t ClExecutiveForProxy::ReleaseResources ()
{
  //## begin ClExecutiveForProxy::ReleaseResources%942686704.body preserve=yes
	if (m_poSession == NULL)
		return M4_ERROR;
	
	ClUSSessionTicketInterface *poTicket = NULL;
	m_poSession->GetSessionTicket(poTicket);

	// releases the session channel
	m4return_t iResult = M4_SUCCESS, iResult2 = M4_SUCCESS, iResult3 = M4_SUCCESS;
	if (poTicket == NULL)
		iResult = M4_ERROR;
	else
	{
		ClChannel *poSessionChannel = NULL;
		poSessionChannel = (ClChannel *)poTicket->GetTicketCode();
		
		if (poSessionChannel == NULL)
			iResult = M4_ERROR;
		else
		{
			if(m_poDumpInfo!=NULL)
			{
				ClProxySpace *poProxySpace = m_poDumpInfo->m_pProxySpace;
				if (poProxySpace != NULL)
				{
					iResult2 = poProxySpace->FreeLogicalConnection();
					ClServerProxyManager *poProxyManager = (ClServerProxyManager*)poProxySpace->GetProxyManager();
					if (poProxyManager != NULL)
						iResult3 = poProxyManager->Detach(poProxySpace);
				
					// marks proxy space
					((ClServerProxySpaceSS*)poProxySpace)->SetInvalidState(M4_TRUE) ;
				}
			}

			// leaves the channel manager
			iResult = poSessionChannel->DetachAndLeaveChannelManager();
		}
	}

	if (iResult2 != M4_SUCCESS || iResult3 != M4_SUCCESS)
		iResult = M4_ERROR;

	return iResult;
  //## end ClExecutiveForProxy::ReleaseResources%942686704.body
}

//## begin module%3507B50C0309.epilog preserve=yes
//## end module%3507B50C0309.epilog
