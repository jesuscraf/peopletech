//## begin module%37DE02D50248.cm preserve=no
//## end module%37DE02D50248.cm

//## begin module%37DE02D50248.cp preserve=no
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
//## end module%37DE02D50248.cp

//## Module: Generic_M4XML%37DE02D50248; Package body
//## Subsystem: M4ExeForChannel::XML::src%3826C5A30000
//## Source file: D:\Work\v400\m4exeforchannel\src\exeform4xml.cpp

//## begin module%37DE02D50248.additionalIncludes preserve=no
//## end module%37DE02D50248.additionalIncludes

//## begin module%37DE02D50248.includes preserve=yes
#include "m4srvefcres.hpp"
#include <navigatorf.hpp>
#include "dm.hpp"
#include "iexecutor.hpp"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <clm4objservice.hpp>
#include "m4srvres.hpp"
#include "m4xmlsubsys.hpp"
#include <metadatasubsys.hpp>
#include "cloblconfiguration.hpp"
#include "blocksynchronization.hpp"
#include <clexecutorstate.hpp>
#include <request.hpp>
#include <m4srvtrace.h>
#include <executiveforoe.hpp>
#include <m4xmlutil.hpp>
#include <exeform4xml.hpp>
#include "m4unicode.hpp"


//## begin module%37DE02D50248.declarations preserve=no
//## end module%37DE02D50248.declarations

//## begin module%37DE02D50248.additionalDeclarations preserve=yes
#include <exem4xml.hpp>
// ClRequestStatistics
#include <request.hpp>
#include "rawsessionid.hpp"

static void Dump(M4DataStorage *ai_poDataStorage, m4pchar_t ai_pcFileName)
{
   ClDataUnitInterface *poDataUnit = NULL;

   ofstream out;

   // UNICODE FILE
   eUniFileEncodingType_t eEncoding = M4UniANSI ;
   M4Open( out, ai_pcFileName, M4UniWrite, eEncoding );

   ai_poDataStorage->InitializeIteration();

   while (ai_poDataStorage->GetNext(poDataUnit)==M4_SUCCESS)
   {
      m4uint32_t size=poDataUnit->GetSize();

      m4puint8_t pBuffer=(m4puint8_t) poDataUnit->GetBuffer();

	  out.write((char *)pBuffer,size);
   }

   ai_poDataStorage->InitializeIteration();
}

const m4size_t M4XMLExecDumpLevel = 4;


#define M4_SRV_TRACE_COMP_ID	"EXECHANNEL"
//## end module%37DE02D50248.additionalDeclarations


// Class ClExecutiveForM4XML 







ClExecutiveForM4XML::ClExecutiveForM4XML (ClEngine *ai_pEngine)
  //## begin ClExecutiveForM4XML::ClExecutiveForM4XML%937296809.hasinit preserve=no
      : m_poStateManager(NULL), m_poConfig(NULL), m_poM4XMLSSInterface(NULL)
  //## end ClExecutiveForM4XML::ClExecutiveForM4XML%937296809.hasinit
  //## begin ClExecutiveForM4XML::ClExecutiveForM4XML%937296809.initialization preserve=yes
  ,	ClExecutiveForOE(ai_pEngine)
  //## end ClExecutiveForM4XML::ClExecutiveForM4XML%937296809.initialization
{
  //## begin ClExecutiveForM4XML::ClExecutiveForM4XML%937296809.body preserve=yes
	m_iMaxDumpLevel = M4XMLExecDumpLevel;
	m_poParser = NULL;
  //## end ClExecutiveForM4XML::ClExecutiveForM4XML%937296809.body
}


ClExecutiveForM4XML::~ClExecutiveForM4XML ()
{
  //## begin ClExecutiveForM4XML::~ClExecutiveForM4XML%937296810.body preserve=yes
	ShutDown();
  //## end ClExecutiveForM4XML::~ClExecutiveForM4XML%937296810.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForM4XML::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExecutiveForM4XML::StartUp%937296811.body preserve=yes

	if ( M4_TRUE == m_bInitialized )
		return M4_ERROR;

	m4return_t iResult = ClExecutiveForOE::StartUp(ai_pNavigator, ai_pConfig);

	// local variables 
	m_poConfig = ai_pConfig;

	// inits properties
    ai_pConfig->AddStringProperty("",PROP_EXECUTION_DESCRIPTION,"");

    // gets the M4XMLSubSystem
	m_poM4XMLSSInterface = (ClSSInterfaz*) ai_pNavigator->GetSubsystemByName (M4_XML_SUBSYSTEM_NAME);
	if (m_poM4XMLSSInterface == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_INIT_GET_SUBSYS , M4_XML_SUBSYSTEM_NAME);
		return M4_ERROR;
	}

	// gets the state manager
	m_poStateManager = (ClExecutorStateManager*) m_poM4XMLSSInterface->GetElement(M4XML_SS_ELM_STATE_CACHE);
	if (m_poStateManager == NULL)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_GET_STACHE );
		return M4_ERROR;
	}

	// instances the parser
	SAXParser* poParser = new SAXParser;
	poParser->setValidationScheme(SAXParser::Val_Never);

	m_poParser = (void*) poParser;

	ClCache	 *poCMCRCache = 0, 
			 *poCSCRCache = 0,
			 *poPresentationCache = 0,
			 *poMapCache = 0;

	size_t		iLenValue;
	m4char_t	szValue [ OBL_PATHVALUE_MAX_LEN ];
	m4return_t	iRet;
	
	iRet = ai_pConfig -> GetStringValue ( "" , M4ExecutiveStringIdProp,   szValue , OBL_PROPERTY_MAX_LEN, iLenValue ) ;
	if (iRet == M4_SUCCESS)
	{
		m_strStringId = szValue;
	}
	else
	{
		SETCODEF ( M4_ERR_SCH_INITIALIZING, ERRORLOG , "Unknown Executor: Cannot find OblProp <%s>" , m_strStringId.c_str(), M4ExecutiveStringIdProp) ;
	}

	iRet = ai_pConfig -> GetStringValue ( "" , M4ExecutiveDumpFilenameProp,   szValue , OBL_PROPVALUE_MAX_LEN, iLenValue ) ;
	if (iRet == M4_SUCCESS)
	{
		m_strDumpFilename = szValue;
	}
	else
	{
		SETCODEF ( M4_ERR_SCH_INITIALIZING, ERRORLOG , "%s: Cannot find OblProp <%s>" , m_strStringId.c_str(), M4ExecutiveDumpFilenameProp) ;
	}

	//Get MD caches from MetadataSubsystem
	ClSSInterfaz * pMetadataSubsystemInterfaz = (ClSSInterfaz*)ai_pNavigator->GetSubsystemByName(ClMetaDataSubsystem::M4MetaDataSubsystemName);
	if (NULL != pMetadataSubsystemInterfaz)
	{
		poCMCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache);
		poCSCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache);
		poPresentationCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache);
		poMapCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDMapCache);
		m_oCMInitValues.SetMetadataSubsystem((ClMetaDataSubsystem*)pMetadataSubsystemInterfaz->m_poImplementation);		
	}
	else
	{
		//Cambiar el coderr
		SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "%s: Cannot find <%s> Subsystem" , m_strStringId.c_str(), ClMetaDataSubsystem::M4MetaDataSubsystemName ) ;
	}

	//Search for the caches in the obl & navigator 
	m4uint16_t iCounter = 0;
	ClCache *poDataCache = 0;
	
	iRet = ai_pConfig -> GetStringValue ( "" , M4ExecutiveDataCacheProp,   szValue , sizeof(szValue) ,iLenValue ) ;
	if (iRet == M4_SUCCESS)
	{
		if ( ! ( poDataCache = ( ClCacheTest * )ai_pNavigator-> GetSubsystemElement ( szValue , "CACHE" ) ) )
		{
			m4char_t buffer[128];
			sprintf(buffer,"%s. Cannot find cache <%s>, in caches subsystem" , m_strStringId.c_str(), szValue);
			SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , buffer ) ;
		}
	}
	else
	{
//		m4char_t buffer[128];
//		sprintf(buffer, "%s. Cannot find OblProp <%s>" , m_strStringId.c_str(), M4ExecutiveDataCacheProp ) ;
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", M4ExecutiveDataCacheProp<< m_strStringId.c_str());
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG, buffer);
	}

	// Now we may have the pointers to the caches
	// wait for the state manager SetCaches
	// m_oCMInitValues.SetCaches(poCMCRCache, poCSCRCache, poPresentationCache, poDataCache);
		

	static m4bool_t bStateInit = M4_FALSE;
	if (bStateInit == M4_FALSE)
	{
		ClExecutorState::SetCaches(poCMCRCache, poCSCRCache, poPresentationCache, poMapCache, poDataCache);
		bStateInit = M4_TRUE;
	}

	return iResult;
  //## end ClExecutiveForM4XML::StartUp%937296811.body
}

m4return_t ClExecutiveForM4XML::Execute (ClImage &aio_pImage)
{
  //## begin ClExecutiveForM4XML::Execute%937296812.body preserve=yes
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
		DUMP_CHLOG_ERROR ( M4_ERR_XML_GET_INPUT_DATA );
#ifdef _DEBUG
		cerr << "Error getting data storage from engine args!!!" << endl;
#endif
		return iResult;
	}

	iResult = poEngineArgs->GetOutputData (poOutputDataStorage);
	if (iResult != M4_SUCCESS)	
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_GET_OUTPUT_DATA );
#ifdef _DEBUG
		cerr << "Error getting data storage from engine args!!!" << endl;
#endif
		return iResult;
	}

	// gets the status
	ClStatus *poClStatus = NULL;
	aio_pImage.GetStatus(poClStatus);

	// set status
	{
		ClMutBlock oMut(m_oMutAccess);
		m_pStatus = poClStatus;
		if (m_pStatus != NULL)
			m_pStatus->SetBoolCondIntrVal(m_pCondIntrVal);
	}

	// sets the session channel
	ClEngineExecEnv *poClEngineExecEnv = NULL;
	aio_pImage.GetEnv(poClEngineExecEnv);

	ClUSSessionTicketInterface *poTicketInterface;
	iResult = poClEngineExecEnv->GetSessionTicket(poTicketInterface);
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForM4XML. Cannot get ticket from from environment.");
		return iResult;
	}

	ClChannel *poSessionChannel = (ClChannel *)poTicketInterface->GetTicketCode();
	if (poSessionChannel == NULL)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForM4XML. Cannot get session channel from ticket.");
		return iResult;
	}


	m_poDumpInfo->SetCurrentStage("Sets generic execution info");

	// sets the xml subsystem info
	iResult = SetSessionInfo();
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForM4XML. Cannot set session channel info in subsystem") ;
	}

	m_poDumpInfo->SetCurrentStage("Executes the method: ExecChannel");

	// execution info
	ClM4XMLExecInfo oInfo(this,&aio_pImage);
	
	// Connect status
	m_oCMInitValues.SetStatus(poClStatus);
	m_oCMInitValues.SetStateManager(m_poStateManager);
	m_oCMInitValues.SetSessionId(m_poSession->GetId());
	m_oCMInitValues.SetSessionChannel(poSessionChannel);
	m_oCMInitValues.SetInputDataStorage(poInputDataStorage);
	m_oCMInitValues.SetExecDumpInfo(m_poDumpInfo);


	// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
	m4string_t	sFullSessionId, sSessionId, sUnused, sServerHost, sServerPort;
	poClEngineExecEnv->GetSessionID( sFullSessionId );
	ClRawSessionIDGenerator oSessionIDGenerator;
	oSessionIDGenerator.ExtractVariables( sFullSessionId, sUnused, sUnused, sSessionId, sServerHost, sServerPort );

	ClStatistic*	pStats;
	m4string_t		sReqUniqueId;

	aio_pImage.GetStats( pStats );
	aio_pImage.GetReqUniqueId( sReqUniqueId );

	setTracesExtInfo( ( ClRequestStatistics* )pStats, poSessionChannel, sServerHost.c_str(), sServerPort.c_str(), sSessionId.c_str(), sReqUniqueId.c_str(), XML_LOG );

	// Starts transaction rows quota control.
	rowsQuotaProlog(poSessionChannel, poTicketInterface, M4_FALSE);

	// execution info
	oInfo.SetFileInfo("StartExecute");

	iResult = ExecM4XML(m_oCMInitValues,(SAXParser*)m_poParser,poInputDataStorage,poOutputDataStorage,m_poDumpInfo,&oInfo);

	// Reset status
	m_pObjectService->GetpExecutor()->SetpCancelStatus(NULL);

	// Ends transaction rows quota control.
	rowsQuotaEpilog((ClRequestStatistics*)pStats, poTicketInterface);
	
	resetTracesExtInfo( XML_LOG );


	// WARNING DUMP affects output pdu's
#if _M4_DEBUG > 10
	Dump(poInputDataStorage ,"srvexinput.xml");
	Dump(poOutputDataStorage,"srvexoutput.xml");
#endif

	if (iResult != M4_SUCCESS)	
	{
		// DUMP_CHLOG_ERROR reported internally
		DUMP_CHLOG_ERROR ( M4_ERR_XML_EXECUTE );
#ifdef _DEBUG
		cerr << "Error executing method!!!" << endl;
#endif
	}

	// sets the status
	if (poClStatus != NULL && iResult == M4CL_USER_ABORT_ALL_REQUEST)
	{
		m4char_t pcBuffer[128];
		if (M4_TRUE == poClStatus->HasTimedOut())
		{
			sprintf(pcBuffer, "In xml executor. Timeout reached in request id %d.", 
				poClStatus->GetIdObject() ) ;
		}
		else
		{
			sprintf(pcBuffer, "In xml executor. Cancelation processed in request id %d.",
				poClStatus->GetIdObject() ) ;
		}

		DUMP_CHLOG_DEBUGF ( M4_CANCELLATION_DETECTED , pcBuffer);

	}

	if (m_pStatus != NULL)
    {
        ClMutBlock oMut(m_oMutAccess);
		m_pStatus->SetBoolCondIntrVal(NULL);
        m_pStatus = NULL;
    }
	// reset session info
	m_poDumpInfo->SetCurrentStage("Resets session info.");

	iResult = SetSessionInfo();
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForM4XML. Cannot set session channel info in subsystem") ;
	}

	m_poDumpInfo->SetCurrentStage("Initial phase", 0);

	// checks for abort execution
	m4return_t iResult2 = M4_SUCCESS;
	if (m_pStatus != NULL && m_pStatus->GetValue())
	{
		ClAdminTagList oOutArgs, oInArgs;
		ClAdminCommandTag *newCommand;

		m4char_t pcSessionId[16];
		sprintf(pcSessionId,"%ld",m_poSession->GetId());

		newCommand = ClAdminCommandTag::GetNewAdminCommandTag(0, "XML_SS_SESSION_ID", adminText, pcSessionId);
		if (newCommand == NULL) 
			return M4_ERROR;
		oOutArgs.push_back(newCommand);
		
		iResult2 = m_poM4XMLSSInterface->ExecuteCommand(m_poM4XMLSSInterface -> GetssId() , ClM4XMLSubSystem::XML_SS_STATE_DEL_BY_SESSION_ID,oOutArgs,oInArgs);
	}

	if (iResult == M4_SUCCESS)
		iResult = iResult2;

	// execution info
	oInfo.SetFileInfo("EndExecutor");
	
	return iResult;

  //## end ClExecutiveForM4XML::Execute%937296812.body
}

m4return_t ClExecutiveForM4XML::ShutDown ()
{
  //## begin ClExecutiveForM4XML::ShutDown%937296813.body preserve=yes

	// local variables
	m4return_t iResult;

	// deletes the parser
	if (m_poParser != NULL)
	{
		delete (SAXParser*) m_poParser;
		m_poParser = NULL;
	}

	// free resource
	iResult = Reset();

	m4return_t iResult2 = ClExecutiveForOE::ShutDown();

	return (iResult != M4_SUCCESS || iResult2 != M4_SUCCESS) ? M4_ERROR : M4_SUCCESS;
  //## end ClExecutiveForM4XML::ShutDown%937296813.body
}

m4return_t ClExecutiveForM4XML::SetSessionInfo ()
{
  //## begin ClExecutiveForM4XML::SetSessionInfo%937296815.body preserve=yes
	m4return_t iResult = M4_SUCCESS;

	/*
	if (m_pM4ConnSubSystem != NULL)
	{
		iResult = ((ClM4ConnSubSystem*)m_pM4ConnSubSystem)->SetInfo(this);
		if (iResult != M4_SUCCESS)
			return iResult;
	}
	*/
		
	// reset members
	m_pObjectService->GetpExecutor()->RequestToAbortWithClStatus(M4_FALSE);

	return M4_SUCCESS;
  //## end ClExecutiveForM4XML::SetSessionInfo%937296815.body
}

m4return_t ClExecutiveForM4XML::ExecuteQuit ()
{
  //## begin ClExecutiveForM4XML::ExecuteQuit%937296816.body preserve=yes
	m_pObjectService->GetpExecutor()->RequestToAbortWithClStatus(M4_TRUE);
	
	return M4_SUCCESS;
  //## end ClExecutiveForM4XML::ExecuteQuit%937296816.body
}

m4return_t ClExecutiveForM4XML::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExecutiveForM4XML::ExecuteCommand%937296819.body preserve=yes
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

			// actualizar (EFV)
			ClAdminCommandTag* pAdminCommandTag = NULL;
				// ClAdminCommandTag::GetNewAdminCommandTag (0, "USED_MEMORY", adminLong, szSize, M4_TRUE);

			if (pAdminCommandTag != NULL)
				ao_oResultList.push_back(pAdminCommandTag);
			else
			{
				DUMP_CHLOG_ERROR ( M4_ERR_XML_DUMP_CMD_TAG );
				iRet = M4_ERROR;
			}
		}

		iRet = m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);		// Continuo la ejecucion

		break;
		
	default:
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_DUMP_UNK_COMMAND,  ai_cCommand );
		iRet = M4_ERROR;
	}
	
	return iRet;
  //## end ClExecutiveForM4XML::ExecuteCommand%937296819.body
}

m4return_t ClExecutiveForM4XML::SetExecInfo (m4pcchar_t ai_pcInfo)
{
  //## begin ClExecutiveForM4XML::SetExecInfo%945092093.body preserve=yes
	if (ai_pcInfo == NULL || m_poConfig == NULL) 
		return M4_SUCCESS;

	if (m_poConfig->SetStringValue("",PROP_EXECUTION_DESCRIPTION, (m4pchar_t)ai_pcInfo) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_XML_SET_PROP_ERROR, PROP_EXECUTION_DESCRIPTION );
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClExecutiveForM4XML::SetExecInfo%945092093.body
}

// Additional Declarations
  //## begin ClExecutiveForM4XML%37DDFDE60166.declarations preserve=yes
  //## end ClExecutiveForM4XML%37DDFDE60166.declarations

// Class ClM4XMLExecInfo 





//## begin ClM4XMLExecInfo::m_oOutFile%39E340C7022C.attr preserve=no  private: static ofstream {UA} 
ofstream ClM4XMLExecInfo::m_oOutFile;
//## end ClM4XMLExecInfo::m_oOutFile%39E340C7022C.attr

//## begin ClM4XMLExecInfo::m_oOutMut%39E340C70286.attr preserve=no  private: static ClMutex {UA} 
ClMutex ClM4XMLExecInfo::m_oOutMut;
//## end ClM4XMLExecInfo::m_oOutMut%39E340C70286.attr

//## begin ClM4XMLExecInfo::m_iCounter%39E340C702EA.attr preserve=no  private: static m4uint32_t {UA} 0
m4uint32_t ClM4XMLExecInfo::m_iCounter = 0;
//## end ClM4XMLExecInfo::m_iCounter%39E340C702EA.attr

//## begin ClM4XMLExecInfo::m_iTimeOff%39E340C70344.attr preserve=no  private: static m4millisec_t {UA} 0
m4millisec_t ClM4XMLExecInfo::m_iTimeOff = 0;
//## end ClM4XMLExecInfo::m_iTimeOff%39E340C70344.attr


ClM4XMLExecInfo::ClM4XMLExecInfo (ClExecutiveForM4XML* ai_poExecutor, ClImage *aio_pImage)
  //## begin ClM4XMLExecInfo::ClM4XMLExecInfo%945092096.hasinit preserve=no
      : m_sBenchPK("OFF"), m_iLocalPK(0), m_iInternalCounter(0), m_poImage(aio_pImage), m_poExecutor(ai_poExecutor)
  //## end ClM4XMLExecInfo::ClM4XMLExecInfo%945092096.hasinit
  //## begin ClM4XMLExecInfo::ClM4XMLExecInfo%945092096.initialization preserve=yes
  //## end ClM4XMLExecInfo::ClM4XMLExecInfo%945092096.initialization
{
  //## begin ClM4XMLExecInfo::ClM4XMLExecInfo%945092096.body preserve=yes
  //## end ClM4XMLExecInfo::ClM4XMLExecInfo%945092096.body
}


ClM4XMLExecInfo::~ClM4XMLExecInfo ()
{
  //## begin ClM4XMLExecInfo::~ClM4XMLExecInfo%945092099.body preserve=yes
	SetInfo("");
  //## end ClM4XMLExecInfo::~ClM4XMLExecInfo%945092099.body
}



//## Other Operations (implementation)
m4return_t ClM4XMLExecInfo::SetDebugInfo (m4pcchar_t ai_pcInfo)
{
  //## begin ClM4XMLExecInfo::SetDebugInfo%971105389.body preserve=yes
	if (m_poImage == NULL || IsInit() == M4_FALSE)
		return M4_SUCCESS;
	
	ClStatistic	*poReqGenStats = NULL;
	m_poImage->GetStats(poReqGenStats);

	assert(poReqGenStats != NULL);

	if (poReqGenStats == NULL)
		return M4_ERROR;

	ClRequestStatistics	*poReqStats = (ClRequestStatistics*)poReqGenStats;

	// needs extended code
#ifdef M4XML_INFO
	poReqStats->m_strBenchPK = ai_pcInfo;
#endif

	m_sBenchPK = ai_pcInfo;

	return M4_SUCCESS;
  //## end ClM4XMLExecInfo::SetDebugInfo%971105389.body
}

m4return_t ClM4XMLExecInfo::SetFileName (m4pcchar_t ai_pcFileName)
{
  //## begin ClM4XMLExecInfo::SetFileName%971105390.body preserve=yes
	if (IsInit() == M4_TRUE)
		return M4_SUCCESS;
	SetInit();

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( m_oOutFile, ai_pcFileName, M4UniWrite, eEncoding );

	return ((m_oOutFile.rdbuf()->is_open() != 0) ? M4_SUCCESS : M4_ERROR);
  //## end ClM4XMLExecInfo::SetFileName%971105390.body
}

// Additional Declarations
  //## begin ClM4XMLExecInfo%38551BFF0320.declarations preserve=yes
  //## end ClM4XMLExecInfo%38551BFF0320.declarations

//## begin module%37DE02D50248.epilog preserve=yes
//## end module%37DE02D50248.epilog
