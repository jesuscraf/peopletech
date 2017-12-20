//## begin module%38F3165F02AA.cm preserve=no
//## end module%38F3165F02AA.cm

//## begin module%38F3165F02AA.cp preserve=no
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
//## end module%38F3165F02AA.cp

//## Module: executiveforoe%38F3165F02AA; Subprogram body
//## Subsystem: M4ExeForChannel::src%379C5C480311
//## Source file: e:\mybuild\m4exeforchannel\src\executiveforoe.cpp

//## begin module%38F3165F02AA.additionalIncludes preserve=no
//## end module%38F3165F02AA.additionalIncludes

//## begin module%38F3165F02AA.includes preserve=yes
//	Rose 98 Body Include Dependences
#include "blocksynchronization.hpp"
#include <metadatasubsys.hpp>
#include <m4log.hpp>
#include <access.hpp>
#include <objdir.hpp>
#include <dm.hpp>
#include <channel.hpp>
#include <clm4objservice.hpp>
#include <clfactm4objservice.hpp>
#include <conn_provider.hpp>
#include "cloblconfiguration.hpp"
#include "iexecutor.hpp"
#include <m4srvtrace.h>
#include "clcache.hpp"
//## end module%38F3165F02AA.includes

// Executive
#include <executive.hpp>
// m4string
#include <m4string.hpp>
// executiveforoe
#include <executiveforoe.hpp>
// ExecDumpInfo
#include <execdumpinfo.hpp>
#include "m4unicode.hpp"
#include "threadobjs.hpp"
#include "rowsquota.hpp"

class ClNavigatorFirewall;
class ClConfiguration;

//## begin module%38F3165F02AA.declarations preserve=no
//## end module%38F3165F02AA.declarations

//## begin module%38F3165F02AA.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID	"EXECHANNEL"
//## end module%38F3165F02AA.additionalDeclarations


// Class ClExecutiveForOE 


//## begin ClExecutiveForOE::M4ExecutiveDumpFilenameProp%3912BEBF0153.attr preserve=no  protected: static m4pchar_t {VAC} "DUMP_FILE"
const m4pchar_t  ClExecutiveForOE::M4ExecutiveDumpFilenameProp = "DUMP_FILE";
//## end ClExecutiveForOE::M4ExecutiveDumpFilenameProp%3912BEBF0153.attr

//## begin ClExecutiveForOE::M4ExecutiveStringIdProp%3912BF000265.attr preserve=no  protected: static m4pchar_t {UAC} "STRINGID"
const m4pchar_t  ClExecutiveForOE::M4ExecutiveStringIdProp = "STRINGID";
//## end ClExecutiveForOE::M4ExecutiveStringIdProp%3912BF000265.attr

//## begin ClExecutiveForOE::M4ExecutiveDataCacheProp%3912BF010343.attr preserve=no  protected: static m4pchar_t {UAC} "DATA_CACHE"
const m4pchar_t  ClExecutiveForOE::M4ExecutiveDataCacheProp = "DATA_CACHE";
//## end ClExecutiveForOE::M4ExecutiveDataCacheProp%3912BF010343.attr






ClExecutiveForOE::ClExecutiveForOE (ClEngine *ai_pEngine, ClStatistic *ai_poStats)
  //## begin ClExecutiveForOE::ClExecutiveForOE%952963880.hasinit preserve=no
      : m_iMaxDumpLevel(0), m_pChannelManager(NULL), m_poDumpInfo(NULL), m_pObjectService(NULL)
  //## end ClExecutiveForOE::ClExecutiveForOE%952963880.hasinit
  //## begin ClExecutiveForOE::ClExecutiveForOE%952963880.initialization preserve=yes
  , ClExecutive(ai_pEngine, ai_poStats)
  //## end ClExecutiveForOE::ClExecutiveForOE%952963880.initialization
{
  //## begin ClExecutiveForOE::ClExecutiveForOE%952963880.body preserve=yes
  //## end ClExecutiveForOE::ClExecutiveForOE%952963880.body
}

ClExecutiveForOE::ClExecutiveForOE ()
  //## begin ClExecutiveForOE::ClExecutiveForOE%952963882.hasinit preserve=no
      : m_iMaxDumpLevel(0), m_pChannelManager(NULL), m_poDumpInfo(NULL), m_pObjectService(NULL)
  //## end ClExecutiveForOE::ClExecutiveForOE%952963882.hasinit
  //## begin ClExecutiveForOE::ClExecutiveForOE%952963882.initialization preserve=yes
		, ClExecutive(NULL)
  //## end ClExecutiveForOE::ClExecutiveForOE%952963882.initialization
{
  //## begin ClExecutiveForOE::ClExecutiveForOE%952963882.body preserve=yes
  //## end ClExecutiveForOE::ClExecutiveForOE%952963882.body
}


ClExecutiveForOE::~ClExecutiveForOE ()
{
  //## begin ClExecutiveForOE::~ClExecutiveForOE%952963881.body preserve=yes
	if (M4_TRUE == m_bInitialized)
	{
		ShutDown();
	}
  //## end ClExecutiveForOE::~ClExecutiveForOE%952963881.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForOE::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExecutiveForOE::StartUp%952690403.body preserve=yes
	if ( M4_TRUE == m_bInitialized )
		return M4_ERROR;

	if ((NULL != m_pChannelManager) || (NULL != m_poSession) || (NULL != m_pObjectService))
		return M4_ERROR;

	if ((NULL == ai_pNavigator) || (NULL == ai_pConfig))
		return M4_ERROR;
	
	ClCache	 *poCMCRCache = 0, 
			 *poCSCRCache = 0,
			 *poPresentationCache = 0,
			 *poMapCache = 0;

	size_t		iLenValue;
	m4char_t	szValue [ OBL_PATHVALUE_MAX_LEN ];
	m4return_t	iRet;
	
	iRet = ai_pConfig -> GetStringValue ( "" , M4ExecutiveStringIdProp,   szValue , OBL_PATHVALUE_MAX_LEN, iLenValue ) ;
	if (iRet == M4_SUCCESS)
	{
		m_strStringId = szValue;
	}
	else
	{
		SETCODEF ( M4_ERR_SCH_INITIALIZING, ERRORLOG , "Unknown Executor: Cannot find OblProp <%s>" , m_strStringId.c_str(), M4ExecutiveStringIdProp) ;
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

//Get MD caches from MetadataSubsystem
	ClSSInterfaz * pMetadataSubsystemInterfaz = (ClSSInterfaz*)ai_pNavigator->GetSubsystemByName(ClMetaDataSubsystem::M4MetaDataSubsystemName);
	if (NULL != pMetadataSubsystemInterfaz)
	{
		poCMCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache);
		poCSCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache);
		poPresentationCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache);
		poMapCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDMapCache);
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
//		sprintf(buffer, "%s. Cannot find OblProp '%s'" , m_strStringId.c_str(), M4ExecutiveDataCacheProp ) ;
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", M4ExecutiveDataCacheProp<< m_strStringId.c_str());
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , WARNINGLOG, buffer);
	}

// Now we may have the pointers to the caches

	m_pObjectService = ClFactM4ObjService::CreateObjService(poCMCRCache, poCSCRCache, poPresentationCache, poMapCache, poDataCache, M4_TRUE);
	if (NULL == m_pObjectService)
	{
		SETCODEF (M4_ERR_SCH_INITIALIZING, ERRORLOG, "%s: Error initializing object service", m_strStringId.c_str() );
		ShutDown();
		return M4_ERROR;
	}

	m_pChannelManager = m_pObjectService->GetpChannelManager();

	m_bInitialized = M4_TRUE;

	return M4_SUCCESS;
  //## end ClExecutiveForOE::StartUp%952690403.body
}

m4return_t ClExecutiveForOE::ShutDown ()
{
  //## begin ClExecutiveForOE::ShutDown%952690405.body preserve=yes
	if (M4_TRUE != m_bInitialized)
		return M4_ERROR;
		
	m_bInitialized = M4_FALSE;
	if ((NULL == m_pChannelManager) || (NULL == m_poSession) || (NULL == m_pObjectService))
		return M4_ERROR;

	Reset();

	m4return_t ret;
	ret=m_pChannelManager->SetSessionChannel(NULL);
	
	if (M4_ERROR ==ret)
	{
		SETCODEF (M4_ERR_SCH_RELEASING, ERRORLOG, "%s: Error when we destroy channel manager", m_strStringId.c_str() );

		return M4_ERROR;
	}

	ClFactM4ObjService::DestroyObjService(m_pObjectService);

	m_pChannelManager = NULL;

	return M4_SUCCESS;
  //## end ClExecutiveForOE::ShutDown%952690405.body
}

m4return_t ClExecutiveForOE::Reset ()
{
  //## begin ClExecutiveForOE::Reset%952690406.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return M4_ERROR;

	// releases the channels
	m4return_t iResult;
	if(m_poDumpInfo!=NULL)
		m_poDumpInfo->clear();

	m_pObjectService->GetpExecutor()->Reset();

	iResult= m_pChannelManager->Reset();
	if (iResult != M4_SUCCESS)	
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "%s: Reseting channel manager!!!", m_strStringId.c_str() );
		return M4_ERROR;
	}

	m_poSession = NULL;

	return M4_SUCCESS;
  //## end ClExecutiveForOE::Reset%952690406.body
}

m4return_t ClExecutiveForOE::DumpLevel (const m4int16_t ai_iLevel)
{
  //## begin ClExecutiveForOE::DumpLevel%952690407.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return M4_ERROR;

	if (0 > ai_iLevel || m_iMaxDumpLevel <= (m4size_t)ai_iLevel)
		return M4_ERROR;

	// prepares to append to file string
	ofstream oDumpFile;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( oDumpFile, m_strDumpFilename.c_str(), M4UniAppend, eEncoding );

	_DumpHeader(oDumpFile);
	_DumpLevel(ai_iLevel, oDumpFile);

	oDumpFile.close();

	return M4_SUCCESS;
  //## end ClExecutiveForOE::DumpLevel%952690407.body
}

m4return_t ClExecutiveForOE::ReleaseResources ()
{
  //## begin ClExecutiveForOE::ReleaseResources%952690408.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return M4_ERROR;

	ClUSSessionTicketInterface	*poTicket;
	m_poSession->GetSessionTicket(poTicket);
	if (NULL == poTicket)
		return M4_ERROR;

	ClChannel		*poSessionChannel;
	poSessionChannel = (ClChannel *)poTicket->GetTicketCode();
	if (NULL == poSessionChannel)
		return M4_ERROR;

	/* Bug 0129436
	Hay que hacer siempre el detach del canal sesión, aunque esto falle
	*/
	m4return_t iResult1 = m_pObjectService->GetpConnProvider()->SetConnection(NULL,M4_TRUE) ;
	m4return_t iResult2 = poSessionChannel->DetachAndLeaveChannelManager() ;

	if( iResult1 != M4_SUCCESS || iResult2 != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return M4_SUCCESS;

  //## end ClExecutiveForOE::ReleaseResources%952690408.body
}

m4return_t ClExecutiveForOE::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExecutiveForOE::ExecuteCommand%957346241.body preserve=yes
	m4return_t iRet;
	
	ClMutBlock oMut(m_oMutAccess);
	
	// Precondicion: si no estoy ejecutando nada, no puedo hacer nada
	if (m_pStatus == NULL)
	{
		SETCODEF (M4_ERR_SCH_INITIALIZING, ERRORLOG, "Not Executing");
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
		//Sea cual fuera el resultado de poner el estado a paused hay que reestablecerlo
		m_pStatus->SetIntrValue(ClStatus::STATUS_INTR_RESUMING);		// Continuo la ejecucion

		break;
		
	default:
		iRet = M4_ERROR;
	}
	
	return iRet;
  //## end ClExecutiveForOE::ExecuteCommand%957346241.body
}

void ClExecutiveForOE::_DumpHeader (ostream &ai_oOstream)
{
  //## begin ClExecutiveForOE::_DumpHeader%953028446.body preserve=yes
	if ( M4_TRUE != m_bInitialized )
		return;

	// writes generic information
	ai_oOstream << endl;
	ai_oOstream << "==============================================" << endl;
	ai_oOstream << m_strStringId << " executor dump info file for executor " << (void*) this << endl;
	ai_oOstream << "==============================================" << endl;
	ai_oOstream << endl;

	m4char_t pcDate[128];
	ClActualDate(pcDate,sizeof(pcDate)-1);
	ai_oOstream << "Actual time: " << pcDate << endl;

  //## end ClExecutiveForOE::_DumpHeader%953028446.body
}

void ClExecutiveForOE::_DumpLevel (m4int16_t ai_iLevel, ostream &ai_oOstream)
{
  //## begin ClExecutiveForOE::_DumpLevel%953028447.body preserve=yes
	m4string_t		strStageDesc;
	switch(ai_iLevel)
	{
	case 0: // info state string
		{
			ai_oOstream << "Dumping state string: " << endl;
			ai_oOstream << "<" << ai_iLevel << "> <";
			if(m_poDumpInfo!=NULL)
				m_poDumpInfo->GetCurrentStage(strStageDesc);

			if (!strStageDesc.empty())
				ai_oOstream << strStageDesc;
			else
				ai_oOstream << "UNKNOWN State!!!";

			ai_oOstream << ">" << endl;
			if(m_poDumpInfo!=NULL)
			{
				ai_oOstream << "-- Dumping tags" << m_poDumpInfo;
				ai_oOstream << "-- End Dumping tags" << endl;
			}
			else
				ai_oOstream << "-- No Dump Info"<< endl;
			

			break;
		}

	case 1: // list of active channels
		{
			ai_oOstream << "Dumping list of active channels" << endl;
			if (m_pChannelManager != NULL)
				ai_oOstream << *m_pChannelManager;
			else
				ai_oOstream << "No Channel Manager initialized" << endl;
			break;
		}

	case 2: // list of objects in the directory
		{
			if(m_poDumpInfo!=NULL)
			{
				ai_oOstream << "Dumping list of objects in directory" << endl;
				if (NULL != m_poDumpInfo->m_pObjectDirectory)
					ai_oOstream << *(m_poDumpInfo->m_pObjectDirectory);
				else
					ai_oOstream << "No Object Directory initialized" << endl;
			}
			else
				ai_oOstream << "-- No Dump Info"<< endl;
			break;
		}

	case 3: // VM stack dump
		{
			ai_oOstream << "Dumping VM state" << endl;
			if (m_pChannelManager != NULL)
				ai_oOstream << m_pObjectService->GetpExecutor();
			else
				ai_oOstream << "No VM state dump information" << endl;
			break;
		}

	default:
		{
			ai_oOstream << "Invalid argument " << ai_iLevel << " passed to dump routine." << endl;
			break;
		}

	}

	ai_oOstream << endl;
  //## end ClExecutiveForOE::_DumpLevel%953028447.body
}

void ClExecutiveForOE::RecycleDumpInfo ()
{
  //## begin ClExecutiveForOE::RecycleDumpInfo%1005730073.body preserve=yes
	if(m_poDumpInfo!=NULL)
		delete m_poDumpInfo;
	m_poDumpInfo=new ExecDumpInfo;
  //## end ClExecutiveForOE::RecycleDumpInfo%1005730073.body
}

m4return_t ClExecutiveForOE::GetSessionValue( ClChannel* ai_pSessionCh, m4pchar_t ai_pcItem, void* &ao_pVal )
{
	ClAccess*	poAccess;
	ClNode*		poNode;
	m4bool_t	bRet;

	// synchronozes the shared session object access
	ClMutBlock oMut(m_oMut);

	if( NULL == ai_pSessionCh || NULL == ai_pcItem )
	{
		return M4_ERROR;
	}

	poAccess = ai_pSessionCh->CreateAccess();

	if( NULL == poAccess )
	{
		return M4_ERROR;
	}

	poNode = &( poAccess->Node[ "ROOT_SESSION" ] );

	if( NULL == poNode )
	{
		poAccess->Destroy();
		return M4_ERROR;
	}
	
	bRet = poNode->RecordSet.Current.Begin();

	if( M4_FALSE == bRet )
	{
		poAccess->Destroy();
		return M4_ERROR;
	}
	
	ao_pVal = poNode->RecordSet.Item[ ai_pcItem ].Value.GetWithOutSec();

	poAccess->Destroy();


	return M4_SUCCESS;
}


// eh 279583. Adds user-timed-actions-auditory primary key columns in trace files.
// New column values are allocated in different objects depending on the log file where they are written.
//	For m4(executor).log	- Sets the values in a per thread log-object.
//	For m4benchmark.log		- Sets the values in the request statistics.
//	For m4logfile.log		- Sets the values in the logsystem thread object.

m4return_t ClExecutiveForOE::setTracesExtInfo( ClRequestStatistics* ai_pStats, ClChannel* ai_pSessionCh, m4pcchar_t ai_pcHost, m4pcchar_t ai_pcPort, m4pcchar_t ai_pcSession, m4pcchar_t ai_pcReqUniqueId, eLogFile_t ai_iLogType )
{
	m4return_t		iRet;
	M4ThreadLogger*	pThreadLog = NULL;
	ClLogSystem*	pLogSystem = NULL;
	m4pchar_t		pcUsrAudKey = NULL;
	int				iServerPort, iSessionId;
	m4uint32_t		iUserInfo;


	pThreadLog = M4ThreadLogger::GetInstance( ai_iLogType );

	if( pThreadLog != NULL )
	{
		pLogSystem = GETPTHREADLOGSYSTEM();

		iRet = GetSessionValue( ai_pSessionCh, "USR_AUDIT_SESSION_KEY", ( void*& )pcUsrAudKey );
		
		if( M4_SUCCESS == iRet && NULL != pcUsrAudKey )
		{
			 pThreadLog->SetSessionKey( pcUsrAudKey );
			 pLogSystem->SetSessionKey( pcUsrAudKey );
			 ai_pStats->SetSessionKey( pcUsrAudKey );
		}
		
		pThreadLog->SetServerName( ai_pcHost );
		pLogSystem->SetServerName( ai_pcHost );

		iServerPort	= atoi( ai_pcPort );
		
		pThreadLog->SetServerPort( iServerPort );
		pLogSystem->SetServerPort( iServerPort );

		iSessionId	= atoi( ai_pcSession );

		pThreadLog->SetSessionId( iSessionId );
		pLogSystem->SetSessionId( iSessionId );
		
		iUserInfo = ( ai_pStats )->GetUserInfo();

		pThreadLog->SetClickNum( iUserInfo );
		pLogSystem->SetClickNum( iUserInfo );

		pThreadLog->SetReqUniqueId( (m4pchar_t)ai_pcReqUniqueId );
		pLogSystem->SetReqUniqueId( (m4pchar_t)ai_pcReqUniqueId );
	}


	return M4_SUCCESS;
}

// Resets current extended info.

m4return_t ClExecutiveForOE::resetTracesExtInfo( eLogFile_t ai_iLogType )
{
	M4ThreadLogger*	pThreadLog = NULL;
	ClLogSystem*	pLogSystem = NULL;
	

	pThreadLog = M4ThreadLogger::GetInstance( ai_iLogType );

	if( pThreadLog != NULL )
	{
		pThreadLog->Reset();

		pLogSystem = GETPTHREADLOGSYSTEM();

		pLogSystem->ResetUserTimeAudit();
	}

	
	return M4_SUCCESS;
}

m4return_t ClExecutiveForOE::rowsQuotaProlog(ClChannel* ai_pSessionCh, ClUSSessionTicketInterface* ai_pSessionTck, m4bool_t ai_bIsOLTP)
{
	m4return_t iRet = M4_SUCCESS;


	ai_pSessionTck->Lock();

	ClRowsQuotaController* pRQC = (ClRowsQuotaController*)ai_pSessionTck->GetRowsQuotaCtrl();

	if (pRQC != NULL)
	{
		pRQC->Reset(ai_bIsOLTP);
	}
	else
	{
		void* pvLimit = NULL;

		iRet = GetSessionValue(ai_pSessionCh, "TRANSACTION_ROWS_QUOTA", pvLimit);

		if (iRet == M4_SUCCESS && pvLimit != NULL)
		{
			m4uint32_t iLimit = (m4uint32_t)*((m4double_t*)pvLimit);

			if (iLimit > 0)
			{
				pRQC = new ClRowsQuotaController(M4_TRUE, iLimit, ai_bIsOLTP);
				ai_pSessionTck->SetRowsQuotaCtrl(pRQC);
			}
		}
	}

	ai_pSessionTck->Unlock();


	ClThreadObjects::Put(ClThreadObjects::ROWS_QUOTA_CONTROLLER, pRQC);


	return iRet;
}

void ClExecutiveForOE::rowsQuotaEpilog(ClRequestStatistics* ai_pReqStats, ClUSSessionTicketInterface* ai_pSessionTck)
{
	ClRowsQuotaController* pRQC = (ClRowsQuotaController*)ai_pSessionTck->GetRowsQuotaCtrl();

	if (pRQC != NULL)
	{
		ai_pReqStats->SetRowsQuotaCurrent(pRQC->GetCount());
		ai_pReqStats->SetRowsQuotaPeak(pRQC->GetPeak());
		ai_pReqStats->SetRowsQuotaLimit(pRQC->GetLimit());
	}
}

// Additional Declarations
  //## begin ClExecutiveForOE%38C8D8420200.declarations preserve=yes
  //## end ClExecutiveForOE%38C8D8420200.declarations

//## begin module%38F3165F02AA.epilog preserve=yes
//## end module%38F3165F02AA.epilog
