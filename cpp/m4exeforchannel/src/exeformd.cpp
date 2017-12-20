//## begin module%35582A24004D.cm preserve=no
//## end module%35582A24004D.cm

//## begin module%35582A24004D.cp preserve=no
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
//## end module%35582A24004D.cp

//## Module: Executive_MDServer%35582A24004D; Package body
//## Subsystem: M4ExeForChannel::Metadata::src%39227682006C
//## Source file: f:\v400\m4exeforchannel\src\exeformd.cpp

//## begin module%35582A24004D.additionalIncludes preserve=no
//## end module%35582A24004D.additionalIncludes

//## begin module%35582A24004D.includes preserve=yes
#include "exechannel.hpp"
#include <m4srvefcres.hpp>
#include "blocksynchronization.hpp"
#include <clm4objservice.hpp>
#include <metadatasubsys.hpp>
#include "dm.hpp"
#include "iexecutor.hpp"
#include "channel.hpp"
#include <conn_provider.hpp>
#include <executiveforoe.hpp>
#include <exeformd.hpp>
#include <usinterface.hpp>
#include "rawsessionid.hpp"




//## begin module%35582A24004D.declarations preserve=no
//## end module%35582A24004D.declarations

//## begin module%35582A24004D.additionalDeclarations preserve=yes
const m4size_t M4MetadataExecDumpLevel = 4;
//## end module%35582A24004D.additionalDeclarations


// Class ClExecutiveForMetaData 


ClExecutiveForMetaData::ClExecutiveForMetaData (ClEngine *ai_pEngine)
  //## begin ClExecutiveForMetaData::ClExecutiveForMetaData%894978816.hasinit preserve=no
      : m_pMetaDataSubSystem(NULL)
  //## end ClExecutiveForMetaData::ClExecutiveForMetaData%894978816.hasinit
  //## begin ClExecutiveForMetaData::ClExecutiveForMetaData%894978816.initialization preserve=yes
  , ClExecutiveForOE(ai_pEngine)
  //## end ClExecutiveForMetaData::ClExecutiveForMetaData%894978816.initialization
{
  //## begin ClExecutiveForMetaData::ClExecutiveForMetaData%894978816.body preserve=yes
	m_iMaxDumpLevel = M4MetadataExecDumpLevel;
  //## end ClExecutiveForMetaData::ClExecutiveForMetaData%894978816.body
}


ClExecutiveForMetaData::~ClExecutiveForMetaData ()
{
  //## begin ClExecutiveForMetaData::~ClExecutiveForMetaData%894978817.body preserve=yes
	ShutDown();
  //## end ClExecutiveForMetaData::~ClExecutiveForMetaData%894978817.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForMetaData::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExecutiveForMetaData::StartUp%894978812.body preserve=yes
	if ( M4_TRUE == m_bInitialized )
		return M4_ERROR;

	m4return_t	iResult = ClExecutiveForOE::StartUp(ai_pNavigator, ai_pConfig);

//Get MD caches from MetadataSubsystem
	m_pMetaDataSubSystem = (ClSSInterfaz *)ai_pNavigator->GetSubsystemByName (ClMetaDataSubsystem::M4MetaDataSubsystemName);

	return iResult;
  //## end ClExecutiveForMetaData::StartUp%894978812.body
}

m4return_t ClExecutiveForMetaData::Execute (ClImage &aio_pImage)
{
  //## begin ClExecutiveForMetaData::Execute%894978813.body preserve=yes
	RecycleDumpInfo();
	m_poDumpInfo->SetCurrentStage("Initial phase", 0);

	// gets the status
	ClStatus *poClStatus = NULL;
	aio_pImage.GetStatus(poClStatus);

	// set status
	{
		ClMutBlock oMut(m_oMutAccess);
		m_pStatus = poClStatus;
	}
	// local variables
	m4return_t iResult;
	ClEngineArgs *poEngineArgs;

	aio_pImage.GetArgs(poEngineArgs);
	M4DataStorage *poInputDataStorage, *poOutputDataStorage;

	iResult = poEngineArgs->GetInputData (poInputDataStorage);
	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error getting data storage from engine args!!!" << endl;
#endif
		if (m_pStatus != NULL)
		{
	        ClMutBlock oMut(m_oMutAccess);
		    m_pStatus = NULL;
		}
		return iResult;
	}

	iResult = poEngineArgs->GetOutputData (poOutputDataStorage);
	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error getting data storage from engine args!!!" << endl;
#endif
		if( m_pStatus )
		{
	        ClMutBlock oMut(m_oMutAccess);
		    m_pStatus = NULL;
		}
		return iResult;
	}

	m_poDumpInfo->SetCurrentStage("Sets the session channel");

	// sets the session channel
	ClEngineExecEnv *poClEngineExecEnv = NULL;
	aio_pImage.GetEnv(poClEngineExecEnv);

	ClUSSessionTicketInterface *poTicketInterface;
	iResult = poClEngineExecEnv->GetSessionTicket(poTicketInterface);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForMetaData. Cannot get ticket from from environment.");
		if( m_pStatus )
		{
	        ClMutBlock oMut(m_oMutAccess);
		    m_pStatus = NULL;
		}
		return iResult;
	}

	ClChannel *poSessionChannel = (ClChannel *)poTicketInterface->GetTicketCode();
	if (poSessionChannel == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForMetaData. Cannot get session channel from ticket.");
		if( m_pStatus )
		{
	        ClMutBlock oMut(m_oMutAccess);
		    m_pStatus = NULL;
		}
		return iResult;
	}

	iResult = poSessionChannel->Attach(m_pChannelManager);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForMetaData. Cannot attach session channel") ;
		if( m_pStatus )
		{
	        ClMutBlock oMut(m_oMutAccess);
		    m_pStatus = NULL;
		}
		return iResult;
	}

	m_pChannelManager->SetSessionChannel(poSessionChannel);

	m_poDumpInfo->SetCurrentStage("Sets the md subsystem info");

	// sets the md subsystem info
	iResult = SetSessionInfo();
	if (iResult != M4_SUCCESS)
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForMetaData. Cannot set session info in subsystem") ;

	m_poDumpInfo->SetCurrentStage("Executes the ExecMetaData");

	// Connect status
	m_pObjectService->GetpExecutor()->SetpCancelStatus(poClStatus);


	// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
	m4string_t	sFullSessionId, sSessionId, sUnused, sServerHost, sServerPort;
	poClEngineExecEnv->GetSessionID( sFullSessionId );
	ClRawSessionIDGenerator oSessionIDGenerator;
	oSessionIDGenerator.ExtractVariables( sFullSessionId, sUnused, sUnused, sSessionId, sServerHost, sServerPort );

	ClStatistic*	pStats;
	m4string_t		sReqUniqueId;

	aio_pImage.GetStats( pStats );
	aio_pImage.GetReqUniqueId( sReqUniqueId );

	setTracesExtInfo( ( ClRequestStatistics* )pStats, poSessionChannel, sServerHost.c_str(), sServerPort.c_str(), sSessionId.c_str(), sReqUniqueId.c_str(), MD_LOG );


	// Se registra la auditoría y se desregistra al final y se persiste
	if( m_pChannelManager != NULL )
	{
		m_pChannelManager->UserActionsAudit.SwitchOn();
	}

	iResult = ExecMetaData(
					m_pObjectService->GetpCMCRFactory(),
					m_pObjectService->GetpPresFactory(),
					m_pObjectService->GetpCacheDirectory(),
					m_pChannelManager,
					poInputDataStorage,
					poOutputDataStorage,
					m_pMetaDataSubSystem ? m_pMetaDataSubSystem->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDUpdateList) : NULL,/*m_pMetaDataSubSystem->GetUpdateList()*/
					m_poDumpInfo);

	if( m_pChannelManager != NULL )
	{
		m_pChannelManager->UserActionsAudit.SwitchOff( M4_TRUE ) ;
	}

	
	resetTracesExtInfo( MD_LOG );


	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error executing method!!!" << endl;
#endif
		if( m_pStatus )
		{
	        ClMutBlock oMut(m_oMutAccess);
		    m_pStatus = NULL;
		}
		return iResult;
	}

	if (poClStatus != NULL && poClStatus->GetIntrValue() != ClStatus::STATUS_INTR_NORMAL)
	{
		m4char_t buffer[128];
		sprintf(buffer, "In metadata executor. Cancelation detected/processed in request id %d.", poClStatus->GetIdObject() ) ;
		DUMP_CHLOG_DEBUGF ( M4_CANCELLATION_DETECTED , buffer);
	}
		
	if( m_pStatus )
	{
        ClMutBlock oMut(m_oMutAccess);
	    m_pStatus = NULL;
	}
	// Disconnect status
	m_pObjectService->GetpExecutor()->SetpCancelStatus(NULL);
	
	iResult = SetSessionInfo();
	if (iResult != M4_SUCCESS)
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForMetaData. Cannot set session channel info in subsystem") ;

	m_poDumpInfo->SetCurrentStage("Frees the session channel");

	// frees the session channel
	m_pChannelManager->SetSessionChannel(NULL);
	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error releasing session channel!!!" << endl;
#endif
		return iResult;
	}

	iResult = poSessionChannel->Detach();
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForMetaData. Cannot detach session channel") ;
		return iResult;
	}

	m_poDumpInfo->SetCurrentStage("Frees logical connection");

	// frees the logical connection
	iResult = m_pObjectService->GetpConnProvider()->SetConnection();
	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error releasing logical connection!!!" << endl;
#endif
		return iResult;
	}

	m_poDumpInfo->SetCurrentStage("Initial phase", 0);

	return iResult;
  //## end ClExecutiveForMetaData::Execute%894978813.body
}

m4return_t ClExecutiveForMetaData::ShutDown ()
{
  //## begin ClExecutiveForMetaData::ShutDown%894978814.body preserve=yes
	if (m_bInitialized != M4_TRUE)
		return M4_ERROR;

	// free resource
	Reset();

	// returns
	return ClExecutiveForOE::ShutDown();
  //## end ClExecutiveForMetaData::ShutDown%894978814.body
}

m4return_t ClExecutiveForMetaData::SetSessionInfo ()
{
  //## begin ClExecutiveForMetaData::SetSessionInfo%921232510.body preserve=yes
	m4return_t iResult = M4_SUCCESS;

	// reset members
	m_pObjectService->GetpExecutor()->RequestToAbortWithClStatus(M4_FALSE);

	return M4_SUCCESS;
  //## end ClExecutiveForMetaData::SetSessionInfo%921232510.body
}

m4return_t ClExecutiveForMetaData::ExecuteQuit ()
{
  //## begin ClExecutiveForMetaData::ExecuteQuit%921232519.body preserve=yes
	m_pObjectService->GetpExecutor()->RequestToAbortWithClStatus(M4_TRUE);

	return M4_SUCCESS;
  //## end ClExecutiveForMetaData::ExecuteQuit%921232519.body
}

m4return_t ClExecutiveForMetaData::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExecutiveForMetaData::ExecuteCommand%922189096.body preserve=yes
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
		iRet = m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_PAUSING);			// Paro la ejecucion
		if (iRet == M4_SUCCESS)
		{
			m4uint32_t iSize = m_pChannelManager->GetSize();						// Mido la memoria

			// Preparo el parametro de salida
			m4char_t szSize[256];
			sprintf(szSize, "%d", iSize);

			ClAdminCommandTag* pAdminCommandTag =
				ClAdminCommandTag::GetNewAdminCommandTag (0, "USED_MEMORY", adminLong, szSize, M4_TRUE);

			if (pAdminCommandTag != NULL)
				ao_oResultList.push_back(pAdminCommandTag);
			else
				iRet = M4_ERROR;
		}
		
		m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);		// Continuo la ejecucion
		break;
		
	default:
		iRet = M4_ERROR;
	}
	
	return iRet;
  //## end ClExecutiveForMetaData::ExecuteCommand%922189096.body
}

m4return_t ClExecutiveForMetaData::Reset ()
{
  //## begin ClExecutiveForMetaData::Reset%958560869.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return M4_ERROR;

	// releases the channels
	if(m_poDumpInfo!=NULL)
		m_poDumpInfo->clear();

	m_poSession = NULL;

	return M4_SUCCESS;
  //## end ClExecutiveForMetaData::Reset%958560869.body
}

//## begin module%35582A24004D.epilog preserve=yes
//## end module%35582A24004D.epilog
