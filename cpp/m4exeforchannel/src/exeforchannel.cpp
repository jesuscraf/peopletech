//## begin module%34BA006C03C8.cm preserve=no
//## end module%34BA006C03C8.cm

//## begin module%34BA006C03C8.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ExecutiveChannel.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980112
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
//## end module%34BA006C03C8.cp

//## Module: Executive_Channel%34BA006C03C8; Package body
//## Subsystem: M4ExeForChannel::src%379C5C480311
//## Source file: D:\eduardoma\m4exeforchannel\src\ExeForChannel.cpp

//## begin module%34BA006C03C8.additionalIncludes preserve=no
//## end module%34BA006C03C8.additionalIncludes

//## begin module%34BA006C03C8.includes preserve=yes
#include "m4srvres.hpp"
#include <m4srvefcres.hpp>
#include "exechannel.hpp"
//	Rose 98 Body Include Dependences
#include <clm4objservice.hpp>
#include "dm.hpp"
#include "iexecutor.hpp"
#include "channel.hpp"
#include <conn_provider.hpp>

#include "objdatacacheinterface.hpp"
#include "rawsessionid.hpp"
//## end module%34BA006C03C8.includes

// executiveforoe
#include <executiveforoe.hpp>
// Executive_Channel
#include <exeforchannel.hpp>
// ClRequestStatistics
#include <request.hpp>

//## begin module%34BA006C03C8.declarations preserve=no
//## end module%34BA006C03C8.declarations

//## begin module%34BA006C03C8.additionalDeclarations preserve=yes
const m4size_t M4OLTPExecDumpLevel = 4;
// Serialize class Server implementation -----------------------------------------

// check sums routines
m4uint32_t ExecCheckSum(void *ai_p, m4uint32_t size, m4uint32_t &sum_old)
{
	M4_ASSERT(ai_p != NULL && size);
	m4puint8_t p = (m4puint8_t) ai_p;
	m4uint32_t sum=0;
	while(size--)
		sum+= *(p++);

	if (!sum_old)
		sum_old = sum;

	if (sum != sum_old)
	{
		M4_ASSERT(sum == sum_old);
	}

	return sum;
}

m4uint32_t ExecCheckSum(M4DataStorage *ai_poDataStorage, m4uint32_t &sum_old)
{
	return 0;

	/******************************************** actualizar para las nuevas pdus

	ai_poDataStorage->InitializeIteration();
	ClDataUnit *poDataUnit;
	m4uint32_t sum=0, total_len=0;
	m4uint16_t n_blocks=0;
	while(	ai_poDataStorage->GetNext(poDataUnit) == M4_SUCCESS)
	{
		n_blocks++;
		m4uint32_t size = poDataUnit->GetSize();
		total_len += size;
		m4puint8_t pBuffer = (m4puint8_t) poDataUnit->GetBuffer();
		while (size--)
			sum += *(pBuffer++);
	}
	M4_ASSERT(total_len);
#ifdef _DEBUG
	cout << "ExeForChannel: nblocks = " << n_blocks << 
		" total_len = " << total_len << " and sum = " << sum << endl;
#endif

		ai_poDataStorage->InitializeIteration();

	if (!sum_old)
		sum_old = sum;

	if (sum != sum_old)
		M4_ASSERT(sum == sum_old);

	return sum;

	*************************************************/ 
}

M4DataStorage * ExecNewDataStorage(M4DataStorage *ai_poDataStorage)
{

	M4_ASSERT ("ExecNewDataStorage: Avoid this call" == NULL);
	return NULL;

	/***********************************************************

	ai_poDataStorage->InitializeIteration();

	M4DataStorage *poNewDataStorage = M4DataStorage::GetNewDataStorage();

	ClDataUnit *poDataUnit;
	m4pchar_t pBuffer;

	while(	ai_poDataStorage->GetNext(poDataUnit) == M4_SUCCESS)
	{
		m4uint32_t size = poDataUnit->GetSize();
		m4return_t iResult = ai_poDataStorage->GetNewBuffer(pBuffer,size);
		M4_ASSERT( iResult == M4_SUCCESS);
		if (iResult != M4_SUCCESS)
			return NULL;
		memcpy(pBuffer,poDataUnit->GetBuffer(),size);
		poNewDataStorage->Add(pBuffer,size);
	}
	
	ai_poDataStorage->InitializeIteration();
	poNewDataStorage->InitializeIteration();

	return poNewDataStorage;

  ***************************************/
}

void ExecCopyToBuffer(m4puint8_t aio_pBuffer, M4DataStorage *ai_poDataStorage)
{
	M4_ASSERT ("ExecCopyToBuffer: Avoid this call" == NULL);
	return;

	/***********************************************************

	ai_poDataStorage->InitializeIteration();
	m4uint32_t lPos = 0;

	ClDataUnit *poDataUnit;

	while(	ai_poDataStorage->GetNext(poDataUnit) == M4_SUCCESS)
	{
		m4uint32_t lSize = poDataUnit->GetSize();
		memcpy(aio_pBuffer+lPos,poDataUnit->GetBuffer(),lSize);
		lPos += lSize;
	}
	
	ai_poDataStorage->InitializeIteration();
	
	***********************************************************/
}

M4DataStorage * ExecNewDataStorage(m4puint8_t pBuffer, m4uint32_t lSize)
{
	M4_ASSERT ("ExecNewDataStorage: Avoid this call" == NULL);
	return NULL;

	/***********************************************************
	M4DataStorage *poNewDataStorage = M4DataStorage::GetNewDataStorage();

	m4pchar_t pNewBuffer;
	m4return_t iResult = poNewDataStorage->GetNewBuffer(pNewBuffer,lSize);
	M4_ASSERT( iResult == M4_SUCCESS);
	if (iResult != M4_SUCCESS)
		return NULL;
	memcpy(pNewBuffer,pBuffer,lSize);
	poNewDataStorage->Add(pNewBuffer,lSize);
	poNewDataStorage->InitializeIteration();

	return poNewDataStorage;

    ***************************************************/
}

// constructor
ClExecSerializableObjectTestServer::ClExecSerializableObjectTestServer(iostream *a_poArgs)
: ClExecSerializableObject(a_poArgs)
{
	// initializes internal members
	m_oData.m_acChannelName = m_oData.m_acNodeName = m_oData.m_acItemName = 0;
	m_oData.m_iNResult = 0;
}

// destructor
ClExecSerializableObjectTestServer::~ClExecSerializableObjectTestServer()
{
	if (m_oData.m_acItemName)
	{
		delete m_oData.m_acItemName;
		m_oData.m_acItemName = 0;
	}

	if (m_oData.m_acNodeName)
	{
		delete m_oData.m_acNodeName;
		m_oData.m_acNodeName = 0;
	}

	if (m_oData.m_acChannelName)
	{
		delete m_oData.m_acChannelName;
		m_oData.m_acChannelName = 0;
	}
}

// serialize method
m4return_t ClExecSerializableObjectTestServer::Serialize()
{
	// invoques the destructor 
	this->~ClExecSerializableObjectTestServer();
	
	// local variables
	m4char_t **ppBase = m_oData.m_aacOutput;
	m4int32_t iSize,iNBlocks=m_oData.m_iNResult,iCount;
	
	// resets the stream
	m_pIOStream->seekp(0,ios::beg);
	m_pIOStream->seekg(0,ios::beg);

	// writes the number of results
	m_pIOStream->write((char*)&iNBlocks,sizeof(iNBlocks));

	// writes the result
	for (iCount=0;iCount<iNBlocks;iCount++)
	{
		iSize = strlen(ppBase[iCount])+1;
		m_pIOStream->write((char*)&iSize,sizeof(iSize));
		m_pIOStream->write(ppBase[iCount],iSize);
	}

	// flushes and returns 
	m_pIOStream->flush();
	return 0;
}

// deserialize method
m4return_t ClExecSerializableObjectTestServer::Deserialize()
{
	// local variables
	m4int32_t iSize;

	m_pIOStream->sync();

	// deserialize objects
	m_pIOStream->read((char*)&iSize,sizeof(iSize));
	m_oData.m_acChannelName = new m4char_t[iSize];
	m_pIOStream->read((char*)m_oData.m_acChannelName,iSize);

	m_pIOStream->read((char*)&iSize,sizeof(iSize));
	m_oData.m_acNodeName = new m4char_t[iSize];
	m_pIOStream->read((char*)m_oData.m_acNodeName,iSize);

	m_pIOStream->read((char*)&m_oData.m_iRegisterN,sizeof(m_oData.m_iRegisterN));

	m_pIOStream->read((char*)&m_oData.m_iNRequest,sizeof(m_oData.m_iNRequest));
	
	m_pIOStream->read((char*)&iSize,sizeof(iSize));
	m_oData.m_acItemName = new m4char_t[iSize];
	m_pIOStream->read((char*)m_oData.m_acItemName,iSize);

	return 0;
}

// Serialize class Client implementation -----------------------------------------

// constructor
ClExecSerializableObjectTestClient::ClExecSerializableObjectTestClient(iostream *a_poArgs)
: ClExecSerializableObject(a_poArgs)
{
	m_oData.m_aacOutput = 0;
}

// destructor
ClExecSerializableObjectTestClient::~ClExecSerializableObjectTestClient()
{
	// local variables
	m4char_t **ppBase = m_oData.m_aacOutput;
	m4int32_t iNBlocks=m_oData.m_iNResult;

	if (! ppBase)
		return;

	while (iNBlocks--)
		delete ppBase[iNBlocks];

	delete ppBase;

	m_oData.m_aacOutput = 0;
	m_oData.m_iNResult = 0;;
}


// serialize method
m4return_t ClExecSerializableObjectTestClient::Serialize()
{
	// invoques the destructor
	this->~ClExecSerializableObjectTestClient();

	// local variables
	m4int32_t iSize;

	// resets the stream
	m_pIOStream->seekp(0,ios::beg);
	m_pIOStream->seekg(0,ios::beg);

	// deserialize objects
	iSize = strlen(m_oData.m_acChannelName)+1;
	m_pIOStream->write((char*)&iSize,sizeof(iSize));
	m_pIOStream->write((char*)m_oData.m_acChannelName,iSize);

	iSize = strlen(m_oData.m_acNodeName)+1;
	m_pIOStream->write((char*)&iSize,sizeof(iSize));
	m_pIOStream->write((char*)m_oData.m_acNodeName,iSize);

	m_pIOStream->write((char*)&m_oData.m_iRegisterN,sizeof(m_oData.m_iRegisterN));
	
	m_pIOStream->write((char*)&m_oData.m_iNRequest,sizeof(m_oData.m_iNRequest));
	
	iSize = strlen(m_oData.m_acItemName)+1;
	m_pIOStream->write((char*)&iSize,sizeof(iSize));
	m_pIOStream->write((char*)m_oData.m_acItemName,iSize);

	m_pIOStream->flush();

	return 0;
}

// deserialize method
m4return_t ClExecSerializableObjectTestClient::Deserialize()
{
	// local variables
	m4int32_t iSize;
	m4int32_t iCount, iNBlocks;
	m4char_t **ppBase;
	
	m_pIOStream->sync();

	// gets the number of elements 
	m_pIOStream->read((char*)&iNBlocks,sizeof(iNBlocks));
	m_oData.m_iNResult = iNBlocks;

	if (iNBlocks > 0)
	{
		ppBase = m_oData.m_aacOutput = new m4char_t * [iNBlocks];

		for (iCount=0;iCount<iNBlocks;iCount++)
		{
			m_pIOStream->read((char*)&iSize,sizeof(iSize));
			ppBase[iCount] = new m4char_t[iSize];
			m_pIOStream->read(ppBase[iCount],iSize);
		}
	}

	return 0;
}

// ClExecutiveInitBDL methods -----------------------------------------------------------
ClExecutiveInitBDL::ClExecutiveInitBDL()
{
}

ClExecutiveInitBDL::~ClExecutiveInitBDL()
{
}

//## end module%34BA006C03C8.additionalDeclarations


// Class ClExecutiveForChannel 


ClExecutiveForChannel::ClExecutiveForChannel (ClEngine *ai_pEngine)
  //## begin ClExecutiveForChannel::ClExecutiveForChannel%884623036.hasinit preserve=no
      : m_poConfiguration(NULL)
  //## end ClExecutiveForChannel::ClExecutiveForChannel%884623036.hasinit
  //## begin ClExecutiveForChannel::ClExecutiveForChannel%884623036.initialization preserve=yes
	,	ClExecutiveForOE(ai_pEngine)
  //## end ClExecutiveForChannel::ClExecutiveForChannel%884623036.initialization
{
  //## begin ClExecutiveForChannel::ClExecutiveForChannel%884623036.body preserve=yes
	m_iMaxDumpLevel = M4OLTPExecDumpLevel;
  //## end ClExecutiveForChannel::ClExecutiveForChannel%884623036.body
}


ClExecutiveForChannel::~ClExecutiveForChannel ()
{
  //## begin ClExecutiveForChannel::~ClExecutiveForChannel%884605453.body preserve=yes
	ShutDown();
  //## end ClExecutiveForChannel::~ClExecutiveForChannel%884605453.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForChannel::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExecutiveForChannel::StartUp%884605454.body preserve=yes
	if ( M4_TRUE == m_bInitialized )
		return M4_ERROR;

	// local variables 
	m4return_t iResult = ClExecutiveForOE::StartUp(ai_pNavigator, ai_pConfig);
	if (M4_ERROR == iResult)
		return M4_ERROR;
	
	m_poConfiguration=ai_pConfig;
	
	// Add the info about execution
	ai_pConfig->AddStringProperty("", PROP_EXECUTION_DESCRIPTION, "-");
	ai_pConfig->AddStringProperty("", PROP_OBJECT_LIST, "-");
	
	return iResult;
  //## end ClExecutiveForChannel::StartUp%884605454.body
}

m4return_t ClExecutiveForChannel::Execute (ClImage &aio_pImage)
{
  //## begin ClExecutiveForChannel::Execute%884605455.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return M4_ERROR;
	RecycleDumpInfo();

	m_poDumpInfo->SetCurrentStage("Initial phase", 0);

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
		return iResult;
	}

	iResult = poEngineArgs->GetOutputData (poOutputDataStorage);
	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error getting data storage from engine args!!!" << endl;
#endif
		return iResult;
	}

	// -- Dispatcher OLTP executor checking.
	ClDataUnitInterface*	pUnit;
	ClDataUnit*				pDataUnit;
	char					acResponse[] = "Check Unit processed";
	m4uint_t				iLen;

	poInputDataStorage->InitializeIteration();
	while( poInputDataStorage->GetNext( pUnit ) != M4_ERROR )
	{
		if( pUnit->GetType() == M4_CHECK_UNIT )
		{
			pDataUnit = (ClDataUnit*)pUnit;
			poOutputDataStorage->GetNewDataUnit( pDataUnit );
			iLen = strlen( acResponse );
			pDataUnit->SetSize( iLen );
			memcpy( pDataUnit->GetBuffer(), acResponse, iLen );

			return M4_SUCCESS;
		}
	}
	poInputDataStorage->InitializeIteration();


	// gets the status
	ClStatus *poClStatus = NULL;
	aio_pImage.GetStatus(poClStatus);

	{
		ClMutBlock oMut(m_oMutAccess);
		m_pStatus = poClStatus;
		if (m_pStatus != NULL)
			m_pStatus->SetBoolCondIntrVal(m_pCondIntrVal);
	}

	m_poDumpInfo->SetCurrentStage("Sets the session channel");

	// sets the session channel
	ClEngineExecEnv *poClEngineExecEnv = NULL;
	aio_pImage.GetEnv(poClEngineExecEnv);

	ClUSSessionTicketInterface *poTicketInterface;
	iResult = poClEngineExecEnv->GetSessionTicket(poTicketInterface);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForChannel. Cannot get ticket from from environment.");
		return iResult;
	}

	ClChannel *poSessionChannel = (ClChannel *)poTicketInterface->GetTicketCode();
	if (poSessionChannel == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForChannel. Cannot get session channel from ticket.");
		return iResult;
	}

	iResult = poSessionChannel->Attach(m_pChannelManager);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForChannel. Cannot attach session channel") ;
		return iResult;
	}

	m_pChannelManager->SetSessionChannel(poSessionChannel);

	m_poDumpInfo->SetCurrentStage("Sets the oltp subsystem info");

	// sets the oltp subsystem info
	iResult = _SetSessionInfo();
	if (iResult != M4_SUCCESS)
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForChannel. Cannot set session channel info in subsystem") ;

	m_poDumpInfo->SetCurrentStage("Executes the method: ExecChannel");

	ClLConn_Provider		*pConnProvider = m_pObjectService->GetpConnProvider();

    // gets the session id.    
    m4string_t sFullSessionId, sSessionId, sUnused, sServerHost, sServerPort;
    poClEngineExecEnv->GetSessionID(sFullSessionId);
    ClRawSessionIDGenerator oSessionIDGenerator;
    oSessionIDGenerator.ExtractVariables (sFullSessionId, sUnused, sUnused, sSessionId, sServerHost, sServerPort);

    // creates the objdata cache interface.
    IObjDataCache* poObjDataCache = m_pChannelManager->GetObjDataCache(sSessionId.c_str());


	// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
	ClStatistic*	pStats;
	m4string_t		sReqUniqueId;

	aio_pImage.GetStats( pStats );
	aio_pImage.GetReqUniqueId( sReqUniqueId );

	setTracesExtInfo( ( ClRequestStatistics* )pStats, poSessionChannel, sServerHost.c_str(), sServerPort.c_str(), sSessionId.c_str(), sReqUniqueId.c_str(), OLTP_LOG );


	// Se registra la auditoría y se desregistra al final y se persiste
	m_pChannelManager->UserActionsAudit.SwitchOn();
         
	// Starts transaction rows quota control.
	rowsQuotaProlog(poSessionChannel, poTicketInterface, M4_TRUE);


    iResult = ExecChannel(m_pChannelManager,poInputDataStorage,poOutputDataStorage,poClStatus,m_poDumpInfo,m_poConfiguration, pConnProvider, poObjDataCache);


	// Ends transaction rows quota control.
	rowsQuotaEpilog((ClRequestStatistics*)pStats, poTicketInterface);

	m_pChannelManager->UserActionsAudit.SwitchOff( M4_TRUE ) ;

	
	resetTracesExtInfo( OLTP_LOG );


	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error executing method!!!" << endl;
#endif
	}

	if (poClStatus != NULL)
	{
		if (poClStatus->GetIntrValue() != ClStatus::STATUS_INTR_NORMAL)
		{
			m4char_t buffer[128];
			sprintf(buffer, "In oltp executor. Cancelation detected/processed in request id %d.", poClStatus->GetIdObject() ) ;
			DUMP_CHLOG_DEBUGF ( M4_CANCELLATION_DETECTED , buffer);
		}
		
        {
	        ClMutBlock oMut(m_oMutAccess);
			m_pStatus->SetBoolCondIntrVal(NULL);
            m_pStatus = NULL;
        }
	}
		
	iResult = _SetSessionInfo();
	if (iResult != M4_SUCCESS)
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForChannel. Cannot set session channel info in subsystem") ;

	m_poDumpInfo->SetCurrentStage("Frees the session channel");
	
	// frees the session channel
	iResult = m_pChannelManager->SetSessionChannel(NULL);
	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error releasing session channel!!!" << endl;
#endif
	}

	iResult = poSessionChannel->Detach();
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForChannel. Cannot detach session channel") ;
		
		// bg 243291
		poSessionChannel->DetachFromChannelManager();
		// return iResult;
	}

	m_poDumpInfo->SetCurrentStage("Frees the logical connection");

	// frees logical connection
	iResult = pConnProvider->SetConnection();
	if (iResult != M4_SUCCESS)	
	{
#ifdef _DEBUG
		cerr << "Error releasing logical connection!!!" << endl;
#endif
	}

	m_poDumpInfo->SetCurrentStage("Initial phase", 0);

	return iResult;
  //## end ClExecutiveForChannel::Execute%884605455.body
}

m4return_t ClExecutiveForChannel::ExecuteQuit ()
{
  //## begin ClExecutiveForChannel::ExecuteQuit%921058575.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return M4_ERROR;

	m_pObjectService->GetpExecutor()->RequestToAbortWithClStatus(M4_TRUE);

	return M4_SUCCESS;
  //## end ClExecutiveForChannel::ExecuteQuit%921058575.body
}

m4return_t ClExecutiveForChannel::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExecutiveForChannel::ExecuteCommand%922189098.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return M4_ERROR;

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
			ClChannelManager* pChannelManager = m_pChannelManager;

			m4uint32_t iSize = pChannelManager->GetSize();						// Mido la memoria

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
		//Sea cual fuera el resultado de poner el estado a paused hay que reestablecerlo
		m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);		// Continuo la ejecucion


		break;
		
	default:
		iRet = M4_ERROR;
	}
	
	return iRet;
  //## end ClExecutiveForChannel::ExecuteCommand%922189098.body
}

m4return_t ClExecutiveForChannel::_SetSessionInfo ()
{
  //## begin ClExecutiveForChannel::_SetSessionInfo%957346237.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return M4_ERROR;

	m4return_t iResult = M4_SUCCESS;

	// reset members
	m_pObjectService->GetpExecutor()->RequestToAbortWithClStatus(M4_FALSE);

	return M4_SUCCESS;
  //## end ClExecutiveForChannel::_SetSessionInfo%957346237.body
}

//## begin module%34BA006C03C8.epilog preserve=yes
//## end module%34BA006C03C8.epilog
