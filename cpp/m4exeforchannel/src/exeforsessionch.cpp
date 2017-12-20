//## begin module%3528FAD80114.cm preserve=no
//## end module%3528FAD80114.cm

//## begin module%3528FAD80114.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ExeFactory
//	 File:              ExeForSessionCh.cpp
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
//## end module%3528FAD80114.cp

//## Module: ExeForSessionCh%3528FAD80114; Package body
//## Subsystem: M4ExeForChannel::src%379C5C480311
//## Source file: D:\eduardoma\m4exeforchannel\src\exeforsessionch.cpp

//## begin module%3528FAD80114.additionalIncludes preserve=no
//## end module%3528FAD80114.additionalIncludes

//## begin module%3528FAD80114.includes preserve=yes
#include <channelutil.hpp>
#include <cs_result.hpp>
#include <m4srvres.hpp>
#include <m4trace.hpp>
#include <subsystemids.hpp>
#include <ssnames.hpp>
#include <conn_provider.hpp>
#include <iexecutor.hpp>
#include <clm4objservice.hpp>
#include <usinterface.hpp>
#include <access.hpp>
#include <channel.hpp>
#include <chan_knl.hpp>
#include <objdir.hpp> 
#include <execntxt.hpp>
#include <cldatstoiod.hpp>
#include <metadatasubsys.hpp>
#include <navigatorf.hpp>
#include "objdatacacheinterface.hpp"
#include "rawsessionid.hpp"
#include <clconfiguration.hpp>
#include <executiveforoe.hpp>
#include <exeforsessionch.hpp>
#include "licensemanagerinterface.hpp"


//## begin module%3528FAD80114.declarations preserve=no
//## end module%3528FAD80114.declarations

//## begin module%3528FAD80114.additionalDeclarations preserve=yes
const m4size_t M4SessionchExecDumpLevel = 4;
//## end module%3528FAD80114.additionalDeclarations


// Class ClExeForSessionChannel 


ClExeForSessionChannel::ClExeForSessionChannel (ClEngine  *ai_pEngine)
  //## begin ClExeForSessionChannel::ClExeForSessionChannel%892638088.hasinit preserve=no
      : m_pServerSessionChannel(NULL)
  //## end ClExeForSessionChannel::ClExeForSessionChannel%892638088.hasinit
  //## begin ClExeForSessionChannel::ClExeForSessionChannel%892638088.initialization preserve=yes
  , ClExecutiveForOE(ai_pEngine)
  //## end ClExeForSessionChannel::ClExeForSessionChannel%892638088.initialization
{
  //## begin ClExeForSessionChannel::ClExeForSessionChannel%892638088.body preserve=yes
	m_iMaxDumpLevel = M4SessionchExecDumpLevel;
  //## end ClExeForSessionChannel::ClExeForSessionChannel%892638088.body
}


ClExeForSessionChannel::~ClExeForSessionChannel ()
{
  //## begin ClExeForSessionChannel::~ClExeForSessionChannel%892638089.body preserve=yes
  //## end ClExeForSessionChannel::~ClExeForSessionChannel%892638089.body
}



//## Other Operations (implementation)
m4return_t ClExeForSessionChannel::Execute (ClImage &aio_pImage)
{
  //## begin ClExeForSessionChannel::Execute%892638091.body preserve=yes
	m4return_t					iResult;
	ClEngineArgs				*poEngineArgs;
	ClUSSessionInterface		*poSession;
	ClUSSessionTicketInterface	*poTicket;
	ClChannel					*pFrontSessionChannel;

	RecycleDumpInfo();
	m_poDumpInfo->SetCurrentStage("Initial phase", 0);
	
	// gets the status
	ClStatus *ai_poClStatus = NULL;
	aio_pImage.GetStatus(ai_poClStatus);
	aio_pImage.GetArgs(poEngineArgs);
	aio_pImage.GetEnv(poSession);

	poSession->GetSessionTicket(poTicket);
	pFrontSessionChannel = (ClChannel *)poTicket->GetTicketCode();
	
	// fix bug 0091910: session channel is not attached to current channel manager
	iResult = pFrontSessionChannel->Attach(m_pChannelManager);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForSessionChannel. Cannot attach session channel") ;
		return M4_FALSE;
	}

	// EFV Keeps the session channel alive
	ClAccess *poTempAccess = pFrontSessionChannel->CreateAccess();
	if (poTempAccess == NULL)
	{
		iResult = m_pObjectService->GetpConnProvider()->SetConnection();
		return M4_FALSE;
	}
	
	m4pcchar_t	pszChannelMethod = "";
	M4DataStorage *pInputDataStorage, *pOutputDataStorage;
	
	iResult = poEngineArgs->GetInputData (pInputDataStorage);
	if (iResult != M4_SUCCESS)	
	{
		SETCODEF (M4_ERR_SCH_DESERIAL, ERRORLOG, "SESSIONCH: Error getting data storage from engine args!!!" );
		
		return M4_FALSE;
	}
	
	iResult = poEngineArgs->GetOutputData (pOutputDataStorage);
	if (iResult != M4_SUCCESS)	
	{
		SETCODEF ( M4_ERR_SCH_DESERIAL, ERRORLOG, "SESSIONCH: Error getting data storage from engine args!!!" );
		
		return M4_FALSE;
	}
	
	
//	Block for automatic deletion
	ClClientServerResult		*pCSResult;
	m4bool_t					carryOn = M4_TRUE;
	{
		m_poDumpInfo->SetCurrentStage("Preparing to Execute", 0);

		ClDataStoreIODriver IOD;
		IOD.Init(GEN_IO_MODE_READ,pInputDataStorage);

		// Reset
		m_poDumpInfo->SetCurrentStage("Reseting Old Directory");

		//
		// Create an object directory ... (begin the game)
		ClCMObjectDirectory oDirectory (m_pChannelManager);
		
		//
		m_poDumpInfo->m_pObjectDirectory = &oDirectory;

        // gets the session id.    
        m4string_t sFullSessionId, sSessionId, sUnused, sServerHost, sServerPort;
        poSession->GetSessionID(sFullSessionId);
        ClRawSessionIDGenerator oSessionIDGenerator;
        oSessionIDGenerator.ExtractVariables (sFullSessionId, sUnused, sUnused, sSessionId, sServerHost, sServerPort);

        // creates the objdata cache interface.
        IObjDataCache* poObjDataCache = m_pChannelManager->GetObjDataCache(sSessionId.c_str());

		
		// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
		ClStatistic*	pStats;
		m4string_t		sReqUniqueId;

		aio_pImage.GetStats( pStats );
		aio_pImage.GetReqUniqueId( sReqUniqueId );

		setTracesExtInfo( ( ClRequestStatistics* )pStats, pFrontSessionChannel, sServerHost.c_str(), sServerPort.c_str(), sSessionId.c_str(), sReqUniqueId.c_str(), OLTP_LOG );


    // Deserializo el Directory
		m_poDumpInfo->SetCurrentStage("Deserializing");
		if (oDirectory.DeSerialize(IOD, poObjDataCache, M4_TRUE) != M4_SUCCESS)
		{
			SETCODEF (M4_ERR_SCH_DESERIAL, ERRORLOG, "SESSIONCH: Error deserializing OIDs or objects !" );
			
			pCSResult = oDirectory.CreateClientServerResult();
			pCSResult->SetResponseCode(M4_CSR_ERROR_DESERIALIZE);
			
			carryOn = M4_FALSE;
		}

		
		resetTracesExtInfo( OLTP_LOG );
		

		// Cogemos el primer canal del directorio (en este caso solo hay uno pero si
		// hubiera más podríamos seguir con GetNextObjectIndexInDirectory()
		m4int16_t channelIndex = -1;

		if (carryOn)
		{
			channelIndex = oDirectory.GetFirstObjectIndex(ChannelObjectType);
			
			if (channelIndex == -1)
			{
				
				SETCODEF (M4_ERR_SCH_DESERIAL, ERRORLOG, "SESSIONCH: No Channel Found!" );
				
				pCSResult = oDirectory.CreateClientServerResult();
				pCSResult->SetResponseCode(M4_CSR_ERROR_NO_CHANNEL);
				
				carryOn = M4_FALSE;
			}
		}
		
		ClChannel				*poChannel;
		ClOLTPExecutionContext	*poOLTPExecutionContext = NULL;
		
		if (M4_TRUE == carryOn)
		{
			poChannel = (ClChannel*)oDirectory.GetObjectAddress(channelIndex);
			if ( NULL == poChannel )
			{
				
				SETCODEF (M4_ERR_SCH_DESERIAL, ERRORLOG, "SESSIONCH: Trying to access Channel position!" );
				
				pCSResult = oDirectory.CreateClientServerResult();
				pCSResult->SetResponseCode(M4_CSR_ERROR_NO_EXECONTEXT);
				
				carryOn = M4_FALSE;
			}
		}
		
		if (M4_TRUE == carryOn)
		{
			
			if (M4_ERROR == TChannelUtil::CheckChannelID (poChannel, "SCH_SESSION"))
			{
				SETCODEF (M4_ERR_SCH_DESERIAL, ERRORLOG, "SESSIONCH: Channel type other than Session Channel" );
				
				pCSResult = oDirectory.CreateClientServerResult();
				pCSResult->SetResponseCode(M4_CSR_ERROR_NO_EXECONTEXT);
				
				carryOn = M4_FALSE;
			}
		}
		
		if (M4_TRUE == carryOn)
		{
			m_poDumpInfo->SetCurrentStage("Inserts the session channel");

			/* Bug 0106203
			Hay que pasar el handle y no el puntero
			*/
			poChannel->Level2.InsertL2Channel("SCH_SESSION_SERVER", pFrontSessionChannel->GetHandle());
			
			// Localizamos el ClOLTPExecutionContext que sabe que Access hay que ejecutar
			m4int16_t OLTPExecutionContextIndex = oDirectory.GetFirstObjectIndex(OLTPExecutionContextType);
			
			if (OLTPExecutionContextIndex == -1)
			{
				SETCODEF (M4_ERR_SCH_DESERIAL, WARNINGLOG, "SESSIONCH: No OLTPExecutionContext found" );
				pCSResult = oDirectory.CreateClientServerResult();
				pCSResult->SetResponseCode(M4_CSR_ERROR_NO_EXECONTEXT);
				
				carryOn = M4_FALSE;
			}
			else
			{
				
				poOLTPExecutionContext = (ClOLTPExecutionContext* )oDirectory.GetObjectAddress(OLTPExecutionContextIndex );
				if ( NULL == poOLTPExecutionContext )
				{
					SETCODEF (M4_ERR_SCH_DESERIAL, ERRORLOG, "SESSIONCH: Trying to access OLTPExecutionContext position!" );
					
					pCSResult = oDirectory.CreateClientServerResult();
					pCSResult->SetResponseCode(M4_CSR_ERROR_NO_EXECONTEXT);
					
					carryOn = M4_FALSE;
				}
			}
		}
		
		// Preguntamos que Access tenemos que ejecutar
		if (M4_TRUE == carryOn)
		{
			pszChannelMethod = poChannel->GetpChannel_Data()->GetpChannelDef()->Id();
			
			// Obtenemos el ejecutor
			IExecutor* poExecutor = poOLTPExecutionContext->GetExecutor();

			m_poDumpInfo->SetCurrentStage("Executing");
			*m_poDumpInfo << "Executing method of channel " << pszChannelMethod << endl;
			*m_poDumpInfo << "Stack State: " << *poExecutor << endl;

			//
			// Hacemos la ejecución del método
			//
            poExecutor->SetpCancelStatus(ai_poClStatus);

			// Se registra la auditoría y se desregistra al final y se persiste
			if( m_pChannelManager != NULL )
			{
				m_pChannelManager->UserActionsAudit.SwitchOn();
			}

			iResult = poExecutor->Execute();   //Tantatachan!!!!

			if( m_pChannelManager != NULL )
			{
				m_pChannelManager->UserActionsAudit.SwitchOff( M4_TRUE ) ;
			}

			if (ai_poClStatus != NULL && iResult == M4CL_USER_ABORT_ALL_REQUEST)
			{
				m4char_t buffer[128];
				sprintf(buffer, "In UUSS execution. Cancelation processed in request id %d.", ai_poClStatus->GetIdObject() ) ;
				DUMP_CHLOG_DEBUGF ( M4_CANCELLATION_DETECTED , buffer);
			}

            poExecutor->SetpCancelStatus(NULL);

			if (iResult != M4_SUCCESS)
			{
				pCSResult = oDirectory.CreateClientServerResult();
				pCSResult->SetResponseCode(M4_CSR_ERROR_EXECUTE);
				
				m4char_t sResult[50];
				sprintf(sResult, "%d", iResult);
				pCSResult->AddMess(sResult);
				
				carryOn = M4_FALSE;
			}

			poChannel->Level2.EraseL2Instance("SCH_SESSION_SERVER", M4_FALSE);
		}
		
		//
		// Preparamos el output buffer
		//
		IOD.Init(GEN_IO_MODE_WRITE, pOutputDataStorage);

		m_poDumpInfo->SetCurrentStage("Serialize");

		ClLConn_Provider	*pConnProvider = m_pObjectService->GetpConnProvider();
		M4_ASSERT(NULL != pConnProvider);

		if (oDirectory.Serialize(IOD) != M4_SUCCESS)
		{
			SETCODEF (M4_ERR_SCH_SERIAL, ERRORLOG, "SESSIONCH: Error serializing OIDs or objects !" );
			iResult = pConnProvider->SetConnection();

			return M4_FALSE;
		}
		
		//-------------------------------------------------------------------------
		//En este punto, el SERVER va a indicarle al CLIENTE el estado Np (Session).		
		//El cliente se esta logando y el server va a devolver valores...
		//Podemos enviar al cliente cualquier cosa pq cuando el cliente pida la primera
		//OLTP o PROXY el server calculará y pasara el valor correcto del estado.
		eStateValidateExecutionNp_t eStateValidateExecutionNp = STATE_VALIDATE_EXECUTION_NP_GREEN;
		iResult = IOD.Write((m4int32_t) eStateValidateExecutionNp);
		if (iResult != M4_SUCCESS)	{
			iResult = pConnProvider->SetConnection();
			return M4_FALSE;
		}

		//-------------------------------------------------------------------------

		m_poDumpInfo->SetCurrentStage("Frees the logical connection");

		iResult = pConnProvider->SetConnection();
		if (iResult != M4_SUCCESS)	
		{
			return M4_FALSE;
		}

		m_poDumpInfo->SetCurrentStage("Releases the M4Objects");

	//	Frees all L2 instances
		iResult = TChannelUtil::CleanUpLinks(pFrontSessionChannel);
		if (iResult != M4_SUCCESS)	
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Cleaning Up links!!!" );
		}
	}
	
	// EFV deletes the temporal access
	poTempAccess->Destroy();

	// fix bug 0091910: session channel is not attached to current channel manager
	iResult = pFrontSessionChannel->Detach();
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForSessionChannel. Cannot attach session channel") ;
		return M4_FALSE;
	}

	return M4_TRUE;
  //## end ClExeForSessionChannel::Execute%892638091.body
}

m4return_t ClExeForSessionChannel::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExeForSessionChannel::StartUp%953028451.body preserve=yes
	m4return_t iRet = ClExecutiveForOE::StartUp(ai_pNavigator, ai_pConfig);
	if (M4_SUCCESS != iRet)
		return M4_ERROR;

	ClUSSessionInterface *poSession = ( ClUSSessionInterface * ) ai_pNavigator -> GetSubsystemElement ( SERVER_SESSION ) ;
	if ( ! poSession )
	{
		SETCODEF ( M4_ERR_SCH_INITIALIZING, ERRORLOG , "SESSIONCH: No server session for session channel executor startup\nServer DataBase cannot use channel executors" ) ;
		ShutDown();
		return M4_ERROR ;
	}

	ClUSSessionTicketInterface *poTicket;
	poSession->GetSessionTicket(poTicket);
	if ( ! poTicket )
	{
		SETCODEF ( M4_ERR_SCH_INITIALIZING, ERRORLOG , "SESSIONCH: No server session ticket" ) ;
		ShutDown();
		return M4_ERROR ;
	}

	m_pServerSessionChannel = (ClChannel *)poTicket->GetTicketCode();

	if (! m_pServerSessionChannel )
	{
		ShutDown();
		return M4_ERROR ;
	}

	return m_pChannelManager->SetSessionChannel(m_pServerSessionChannel);
  //## end ClExeForSessionChannel::StartUp%953028451.body
}

// Additional Declarations
  //## begin ClExeForSessionChannel%3528F72501C0.declarations preserve=yes
  //## end ClExeForSessionChannel%3528F72501C0.declarations

//## begin module%3528FAD80114.epilog preserve=yes
//## end module%3528FAD80114.epilog
