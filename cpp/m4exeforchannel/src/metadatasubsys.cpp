//## begin module%35A24233018B.cm preserve=no
//## end module%35A24233018B.cm

//## begin module%35A24233018B.cp preserve=no
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
//## end module%35A24233018B.cp

//## Module: MetaData_SubSystem%35A24233018B; Package body
//## Subsystem: M4ExeForChannel::Metadata::src%39227682006C
//## Source file: f:\v400\m4exeforchannel\src\metadatasubsys.cpp

//## begin module%35A24233018B.additionalIncludes preserve=no
//## end module%35A24233018B.additionalIncludes

//## begin module%35A24233018B.includes preserve=yes
#include <subsystemids.hpp>
#include "cldefine.hpp"
#include "cachesubsystem.hpp"
#include "mdupdate.hpp"
#include "cachedir.hpp"
#include "m4eventhandlers.hpp"
#include <clfactm4objservice.hpp>
#include <channelutil.hpp>
//	Rose 98 Body Include Dependences
#include "m4objreg.hpp"
#include <clm4objservice.hpp>
#include "dm.hpp"
#include <oltplauncher.hpp>
#include "exeformd.hpp"
#include "cloblconfiguration.hpp"
#include <m4srvtrace.h>
//## end module%35A24233018B.includes

// ClConfiguration
#include <clconfiguration.hpp>
// ExecutiveForOESS
#include <executiveforoess.hpp>
// executiveforoe
#include <executiveforoe.hpp>
// MetaData_SubSystem
#include <metadatasubsys.hpp>
// propertiestable
#include <propertiestable.hpp>
// clservicebasess
#include <clservicebasess.hpp>
#include "m4ldb.hpp"
#include "replaceliterals.hpp"


class ClNavigatorFirewall;
class ClCache;

//## begin module%35A24233018B.declarations preserve=no
//## end module%35A24233018B.declarations

//## begin module%35A24233018B.additionalDeclarations preserve=yes

const m4pchar_t M4_MDSUBSISTEM_UPDATE_LIST_MAX_SIZE		= "UPDATE_LIST_MAX_SIZE";
#define M4_SRV_TRACE_COMP_ID	"EXECHANNEL"
//## end module%35A24233018B.additionalDeclarations


// Class ClMetaDataSubsystem 

//## begin ClMetaDataSubsystem::M4MetaDataSubsystemName%3911A7D2018D.attr preserve=no  public: static m4pchar_t {VAC} "METADATA_SUBSYSTEM"
const m4pchar_t  ClMetaDataSubsystem::M4MetaDataSubsystemName = "METADATA_SUBSYSTEM";
//## end ClMetaDataSubsystem::M4MetaDataSubsystemName%3911A7D2018D.attr




ClMetaDataSubsystem::ClMetaDataSubsystem (ClConfiguration *  ai_poConfiguration, ClTables *  ai_poTables, ClNavigatorFirewall *  ai_pNavigator)
  //## begin ClMetaDataSubsystem::ClMetaDataSubsystem%899899318.hasinit preserve=no
      : m_pConfigurationForStartup(NULL), m_pNavigatorForStartup(NULL)
  //## end ClMetaDataSubsystem::ClMetaDataSubsystem%899899318.hasinit
  //## begin ClMetaDataSubsystem::ClMetaDataSubsystem%899899318.initialization preserve=yes
	  ,ClServiceBaseSS (ai_poConfiguration, ai_poTables, ai_pNavigator ), m_oExecutiveForMDCaches(NULL)
  //## end ClMetaDataSubsystem::ClMetaDataSubsystem%899899318.initialization
{
  //## begin ClMetaDataSubsystem::ClMetaDataSubsystem%899899318.body preserve=yes
	m_pNavigatorForStartup = ai_pNavigator;
	m_pConfigurationForStartup = ai_poConfiguration;
	// adds a new property to the configuration
	ai_poConfiguration->AddStringProperty("","REPOSITORY_KEY","");
  //## end ClMetaDataSubsystem::ClMetaDataSubsystem%899899318.body
}


ClMetaDataSubsystem::~ClMetaDataSubsystem ()
{
  //## begin ClMetaDataSubsystem::~ClMetaDataSubsystem%899899319.body preserve=yes
	ShutDown();
  //## end ClMetaDataSubsystem::~ClMetaDataSubsystem%899899319.body
}



//## Other Operations (implementation)
m4return_t ClMetaDataSubsystem::GetCommandsId (ClAdminTagList &  aio_oCommandList)
{
  //## begin ClMetaDataSubsystem::GetCommandsId%899899320.body preserve=yes
	if (ClServiceBaseSS::GetCommandsId(aio_oCommandList) != M4_SUCCESS)
		return M4_ERROR;
	
	//---------------------------------------------------------------
	// insert commands for this subsystem
	//---------------------------------------------------------------
	ClAdminTag *newCommand;

	newCommand = ClAdminTag::GetNewAdminTag(CLEAN_CACHE, "M4_MDSUBSYSTEM_CLEAN_CACHE", M4_FALSE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) return M4_FALSE;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(REFRESH_UPDATE_LIST, "M4_MDSUBSYSTEM_REFRESH_UPDATE_LIST", M4_FALSE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) return M4_FALSE;
	aio_oCommandList.push_back(newCommand);

	return  M4_SUCCESS;
  //## end ClMetaDataSubsystem::GetCommandsId%899899320.body
}

m4return_t ClMetaDataSubsystem::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &  ai_oParamList, ClAdminTagList &  ao_oResultList)
{
  //## begin ClMetaDataSubsystem::ExecuteCommand%899899321.body preserve=yes
	m4return_t iRet = M4_ERROR;
	m4uint32_t iSessionId = 0;

	if (ai_cCommand < ClServiceBaseSS::TOTAL_COMMANDS)
		return ClServiceBaseSS::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
	
	switch (ai_cCommand)
	{
	case CLEAN_CACHE:		
		iRet = CleanCaches();
		break;

	case REFRESH_UPDATE_LIST:		
		iRet = RefreshUpdateList();
		break;
		
	default:
		M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;
//		SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "Meta Data Subsystem: Invalid Administrator List SubSystem command Id: %d", ai_cCommand );
		break;		
	}
	
	return iRet;
  //## end ClMetaDataSubsystem::ExecuteCommand%899899321.body
}

m4return_t ClMetaDataSubsystem::Initialize ()
{
  //## begin ClMetaDataSubsystem::Initialize%899899323.body preserve=yes
	if ( M4_ERROR == m_oExecutiveForMDCaches.StartUp(m_pNavigatorForStartup ,GetConfiguration() ) )
	{
		return M4_ERROR ;
	}

	return ClServiceBaseSS::Initialize () ;
  //## end ClMetaDataSubsystem::Initialize%899899323.body
}

m4return_t ClMetaDataSubsystem::ShutDown ()
{
  //## begin ClMetaDataSubsystem::ShutDown%899899324.body preserve=yes
	m_pNavigatorForStartup = 0;
	m_pConfigurationForStartup = 0;
		  
	return M4_SUCCESS;
  //## end ClMetaDataSubsystem::ShutDown%899899324.body
}

m4return_t ClMetaDataSubsystem::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &  aio_oParamList)
{
  //## begin ClMetaDataSubsystem::GetCommandParams%899899325.body preserve=yes
	m4return_t retVal=M4_ERROR;
	ClAdminCommandTag *newParam=NULL;

	//-----------------------------------------------------------------------
	// check if the command is a base class command.
	//-----------------------------------------------------------------------
	if(ai_cCommand < ClServiceBaseSS::TOTAL_COMMANDS)
		return ClServiceBaseSS::GetCommandParams (ai_cCommand, aio_oParamList);
	
	//-----------------------------------------------------------------------
	// build the parameters list.
	//-----------------------------------------------------------------------
	switch (ai_cCommand)
	{
    case CLEAN_CACHE:
		// There are no parameters for this command.
		retVal=M4_SUCCESS;
		break;
		
    case REFRESH_UPDATE_LIST:
		// There are no parameters for this command.
		retVal=M4_SUCCESS;
		break;

	default:
		M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;
//		SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "MetaData Subsystem: Invalid Administrator List SubSystem command Id: %d", ai_cCommand );
		break;		
	}

	return retVal;
  //## end ClMetaDataSubsystem::GetCommandParams%899899325.body
}

void *  ClMetaDataSubsystem::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClMetaDataSubsystem::GetElement%899899333.body preserve=yes
	if (!ai_szElement)
		return NULL;

	if (strcmpi(ai_szElement,ClExecutiveForMDCaches::M4ExecutiveForMDUpdateList) == 0)
		return ((void*)m_oExecutiveForMDCaches.GetUpdateList());
	else if (strcmpi(ai_szElement, ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache) == 0)
		return ((void*)m_oExecutiveForMDCaches.GetCMCRCache());
	else if (strcmpi(ai_szElement, ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache) == 0)
		return ((void*)m_oExecutiveForMDCaches.GetCSCRCache());
	else if (strcmpi(ai_szElement, ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache) == 0)
		return ((void*)m_oExecutiveForMDCaches.GetPresentationCache());
	else if (strcmpi(ai_szElement, ClExecutiveForMDCaches::M4ExecutiveForMDMapCache) == 0)
		return ((void*)m_oExecutiveForMDCaches.GetMapCache());
	else
		return NULL;
  //## end ClMetaDataSubsystem::GetElement%899899333.body
}

ClCache *  ClMetaDataSubsystem::GetCMCRCache ()
{
  //## begin ClMetaDataSubsystem::GetCMCRCache%957518537.body preserve=yes
	return m_oExecutiveForMDCaches.GetCMCRCache();
  //## end ClMetaDataSubsystem::GetCMCRCache%957518537.body
}

ClCache *  ClMetaDataSubsystem::GetCSCRCache ()
{
  //## begin ClMetaDataSubsystem::GetCSCRCache%957518538.body preserve=yes
	return m_oExecutiveForMDCaches.GetCSCRCache();
  //## end ClMetaDataSubsystem::GetCSCRCache%957518538.body
}

ClCache *  ClMetaDataSubsystem::GetPresentationCache ()
{
  //## begin ClMetaDataSubsystem::GetPresentationCache%957518539.body preserve=yes
	return m_oExecutiveForMDCaches.GetPresentationCache();
  //## end ClMetaDataSubsystem::GetPresentationCache%957518539.body
}

ClCache *  ClMetaDataSubsystem::GetMapCache ()
{
  //## begin ClMetaDataSubsystem::GetPresentationCache%957518539.body preserve=yes
	return m_oExecutiveForMDCaches.GetMapCache();
  //## end ClMetaDataSubsystem::GetPresentationCache%957518539.body
}

m4return_t ClMetaDataSubsystem::RefreshUpdateList ()
{
  //## begin ClMetaDataSubsystem::RefreshUpdateList%957518540.body preserve=yes
	return m_oExecutiveForMDCaches.RefreshUpdateList(m_pConfigurationForStartup);
  //## end ClMetaDataSubsystem::RefreshUpdateList%957518540.body
}

m4return_t ClMetaDataSubsystem::CleanCaches ()
{
  //## begin ClMetaDataSubsystem::CleanCaches%957518541.body preserve=yes
	return m_oExecutiveForMDCaches.CleanCaches();
  //## end ClMetaDataSubsystem::CleanCaches%957518541.body
}

ClMDUpdateList * ClMetaDataSubsystem::GetUpdateList ()
{
  //## begin ClMetaDataSubsystem::GetUpdateList%957518542.body preserve=yes
	return m_oExecutiveForMDCaches.GetUpdateList();
  //## end ClMetaDataSubsystem::GetUpdateList%957518542.body
}

ClSSInterfaz * ClMetaDataSubsystem::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClMetaDataSubsystem::GenerateSubsystem%957518543.body preserve=yes
	ClExecutiveBaseSS * poExecutiveSS;

	if ( ai_lType == M4_EXECUTOR_MD_SUBSYSTEM)
	{
		poExecutiveSS = new ClExecutiveForMetaDataSS( ai_poConfiguration , ai_poTables , m_poNavigator , this) ;

		poExecutiveSS  -> SetUseExceptions ( m_bUseExceptions ) ;

		poExecutiveSS  -> SetServiceQ ( m_poServiceQ ) ;

		return poExecutiveSS  -> GetInterfaz ();
	}
	else
	{
		SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "Metadata Service Subsystem Don't know how to make subsystem type %d" , ai_lType ) ;
		return NULL ;
	}
	
  //## end ClMetaDataSubsystem::GenerateSubsystem%957518543.body
}

// Additional Declarations
  //## begin ClMetaDataSubsystem%35A239CD0306.declarations preserve=yes
  //## end ClMetaDataSubsystem%35A239CD0306.declarations

// Class ClExecutiveForMetaDataSS 

//## begin ClExecutiveForMetaDataSS::s_szMetaDataInstanceOrder%3912EC2E0220.attr preserve=no  protected: static m4size_t {UA} 0
m4size_t ClExecutiveForMetaDataSS::s_szMetaDataInstanceOrder = 0;
//## end ClExecutiveForMetaDataSS::s_szMetaDataInstanceOrder%3912EC2E0220.attr

ClExecutiveForMetaDataSS::ClExecutiveForMetaDataSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClExecutiveForMetaDataSS::ClExecutiveForMetaDataSS%957518544.hasinit preserve=no
  //## end ClExecutiveForMetaDataSS::ClExecutiveForMetaDataSS%957518544.hasinit
  //## begin ClExecutiveForMetaDataSS::ClExecutiveForMetaDataSS%957518544.initialization preserve=yes
  :ClExecutiveForOESS (ai_poConfiguratoin, ai_poTables, ai_poNavigator , ai_poServiceSS)
  //## end ClExecutiveForMetaDataSS::ClExecutiveForMetaDataSS%957518544.initialization
{
  //## begin ClExecutiveForMetaDataSS::ClExecutiveForMetaDataSS%957518544.body preserve=yes
  //## end ClExecutiveForMetaDataSS::ClExecutiveForMetaDataSS%957518544.body
}


ClExecutiveForMetaDataSS::~ClExecutiveForMetaDataSS ()
{
  //## begin ClExecutiveForMetaDataSS::~ClExecutiveForMetaDataSS%957518545.body preserve=yes
  //## end ClExecutiveForMetaDataSS::~ClExecutiveForMetaDataSS%957518545.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForMetaDataSS::Initialize ()
{
  //## begin ClExecutiveForMetaDataSS::Initialize%957518546.body preserve=yes
	m4return_t		iRet = ClExecutiveForOESS::Initialize ();
	if (M4_SUCCESS != iRet)
		return M4_ERROR;
	
	ClLauncherStats *    poStats ;
	ClExecutive	*		poExecutive;

	m_poStatistics = poStats = new ClLauncherStats ;

	poExecutive = new ClExecutiveForMetaData(NULL);

	//Para que los miembros m_poOutputQ y m_poServiceQ estén inicializados se los ha tenido que dar 
	//el SS de servicio correspondiente en el Generate SS

	m_poLauncher = new ClLauncherForOLTP(  poExecutive , poStats, /*ai_poLog, m_poServiceQ*/m_poServiceSS);

	m_szInstanceOrder = ++s_szMetaDataInstanceOrder;

	return M4_SUCCESS ;
  //## end ClExecutiveForMetaDataSS::Initialize%957518546.body
}

// Additional Declarations
  //## begin ClExecutiveForMetaDataSS%3912D84401EE.declarations preserve=yes
  //## end ClExecutiveForMetaDataSS%3912D84401EE.declarations

// Class ClExecutiveForMDCaches 

//## begin ClExecutiveForMDCaches::M4ExecutiveForMDCachesId%3912AE850218.attr preserve=no  protected: static m4pchar_t {VAC} "ExecutiveForMDCaches"
const m4pchar_t  ClExecutiveForMDCaches::M4ExecutiveForMDCachesId = "ExecutiveForMDCaches";
//## end ClExecutiveForMDCaches::M4ExecutiveForMDCachesId%3912AE850218.attr

//## begin ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache%3912EF4E002C.attr preserve=no  public: static m4pchar_t {UAC} "CMCR_CACHE"
const m4pchar_t  ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache = "CMCR_CACHE";
//## end ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache%3912EF4E002C.attr

//## begin ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache%3912EF4F031C.attr preserve=no  public: static m4pchar_t {UAC} "CSCR_CACHE"
const m4pchar_t  ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache = "CSCR_CACHE";
//## end ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache%3912EF4F031C.attr

//## begin ClExecutiveForMDCaches::M4ExecutiveForMDDataCache%3912EF51001C.attr preserve=no  public: static m4pchar_t {UAC} "DATA_CACHE"
const m4pchar_t  ClExecutiveForMDCaches::M4ExecutiveForMDDataCache = "DATA_CACHE";
//## end ClExecutiveForMDCaches::M4ExecutiveForMDDataCache%3912EF51001C.attr

//## begin ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache%3912EF7C015E.attr preserve=no  public: static m4pchar_t {UAC} "PRES_CACHE"
const m4pchar_t  ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache = "PRES_CACHE";
//## end ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache%3912EF7C015E.attr

const m4pchar_t  ClExecutiveForMDCaches::M4ExecutiveForMDMapCache = "MAP_CACHE";

//## begin ClExecutiveForMDCaches::M4ExecutiveForMDUpdateList%392BC55201CF.attr preserve=no  public: static m4pchar_t {UAC} "UPDATE_LIST"
const m4pchar_t  ClExecutiveForMDCaches::M4ExecutiveForMDUpdateList = "UPDATE_LIST";
//## end ClExecutiveForMDCaches::M4ExecutiveForMDUpdateList%392BC55201CF.attr






ClExecutiveForMDCaches::ClExecutiveForMDCaches (ClEngine *ai_pEngine, ClStatistic *ai_poStats)
  //## begin ClExecutiveForMDCaches::ClExecutiveForMDCaches%957518525.hasinit preserve=no
      : m_pRegistry(NULL), m_pCSCRCache(NULL), m_pCMCRCache(NULL), m_pPresentationCache(NULL), m_pMapCache(NULL)
  //## end ClExecutiveForMDCaches::ClExecutiveForMDCaches%957518525.hasinit
  //## begin ClExecutiveForMDCaches::ClExecutiveForMDCaches%957518525.initialization preserve=yes
	  ,	ClExecutiveForOE(ai_pEngine), m_pUpdateList(NULL)
  //## end ClExecutiveForMDCaches::ClExecutiveForMDCaches%957518525.initialization
{
  //## begin ClExecutiveForMDCaches::ClExecutiveForMDCaches%957518525.body preserve=yes
  //## end ClExecutiveForMDCaches::ClExecutiveForMDCaches%957518525.body
}


ClExecutiveForMDCaches::~ClExecutiveForMDCaches ()
{
  //## begin ClExecutiveForMDCaches::~ClExecutiveForMDCaches%957518526.body preserve=yes
  //## end ClExecutiveForMDCaches::~ClExecutiveForMDCaches%957518526.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForMDCaches::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExecutiveForMDCaches::StartUp%957518527.body preserve=yes
	size_t iLenValue;
	m4char_t pszValue [ OBL_PATHVALUE_MAX_LEN ] ;

	if ( M4_TRUE == m_bInitialized )
		return M4_ERROR;

	if ((NULL != m_pChannelManager) || (NULL != m_poSession) || (NULL != m_pObjectService))
		return M4_ERROR;

	if ((NULL == ai_pNavigator) || (NULL == ai_pConfig))
		return M4_ERROR;
	
	m_strStringId = M4ExecutiveForMDCachesId;

	m4return_t	iRet;
	iRet = ai_pConfig -> GetStringValue ( "" , ClExecutiveForOE::M4ExecutiveDumpFilenameProp,   pszValue , OBL_PROPVALUE_MAX_LEN, iLenValue ) ;
	if (iRet == M4_SUCCESS)
	{
		m_strDumpFilename = pszValue;
	}
	else
	{
		SETCODEF ( M4_ERR_SCH_INITIALIZING, ERRORLOG , "%s: Cannot find OblProp <%s>" , m_strStringId.c_str(), ClExecutiveForOE::M4ExecutiveDumpFilenameProp) ;
	}

	//Environment
	m_pRegistry = new ClM4ObjRegServer;

	// initialize components
	iRet = m_pRegistry->Init();
	if (iRet != M4_SUCCESS)
	{
#ifdef _DEBUG
		cerr << "Error in Init!!" << endl;
#endif
		return M4_ERROR;
	}
	
	//CMCRCACHE
	m4char_t			acAux [10] ;
	ClAdminCommandTag	*poParam;
	ClAdminTagList		oParamList,
						oResultList;
	ClCache				*poCache = 0, *poDataCache = 0;
	iRet = ai_pConfig->GetStringValue ( "", ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache ,   pszValue , sizeof(pszValue) ,iLenValue ) ;
	if (iRet == M4_SUCCESS )
	{
		if ( ! ( poCache = ( ClCache *)ai_pNavigator-> GetSubsystemElement ( pszValue , "CACHE" ) ) )
		{
			SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find cache <%s>, in caches subsystem" , pszValue ) ;
		}

		m_pCMCRCache = poCache;

		//Si no la caché de MD está a NONE => la marcamos como inactiva.
		if (M4CL_CACHE_NONE == m_pRegistry->GetMDDiskCache())
		{
			//Llamamos al comando CHANGE_CACHE_STATUS del subsistema de la caché ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache.
			sprintf (acAux , "%d" , ClServiceBaseSS::SS_INACTIVE ) ;
			poParam=ClAdminCommandTag::GetNewAdminCommandTag (ClCacheSubSystem::CACHE_STATUS, "CACHE_STATUS", adminShort, acAux);
			oParamList.push_back(poParam);

			ClSSInterfaz * pSubsBase = (ClSSInterfaz *) ai_pNavigator->GetSubsystemByName(pszValue);

			pSubsBase->ExecuteCommand(pSubsBase-> GetssId () , ClCacheSubSystem::CHANGE_CACHE_STATUS, oParamList, oResultList);
		}
	}
	else
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache << "METADATA_SUBSYSTEM" );
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find OblProp <%s>" , ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache ) ;
	}

	//CSCRCACHE
	iRet = ai_pConfig->GetStringValue ( "", ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache ,   pszValue , sizeof(pszValue) ,iLenValue ) ;

	if (iRet == M4_SUCCESS )
	{
		if ( ! ( poCache = ( ClCache * )ai_pNavigator-> GetSubsystemElement ( pszValue , "CACHE") ) )
		{
			SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find cache <%s>, in caches subsystem" , pszValue ) ;
		}

		m_pCSCRCache = poCache;

		//Si no la caché de MD está a NONE => la marcamos como inactiva.
		if (M4CL_CACHE_NONE == m_pRegistry->GetMDDiskCache())
		{
			//Llamamos al comando CHANGE_CACHE_STATUS del subsistema de la caché ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache.
			sprintf (acAux , "%d" , ClServiceBaseSS::SS_INACTIVE ) ;
			poParam=ClAdminCommandTag::GetNewAdminCommandTag (ClCacheSubSystem::CACHE_STATUS, "CACHE_STATUS", adminShort, acAux);
			oParamList.push_back(poParam);

			ClSSInterfaz * pSubsBase = (ClSSInterfaz *) ai_pNavigator->GetSubsystemByName(pszValue);

			pSubsBase->ExecuteCommand(pSubsBase -> GetssId() , ClCacheSubSystem::CHANGE_CACHE_STATUS, oParamList, oResultList);
		}
	}
	else
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache<< "METADATA_SUBSYSTEM" );
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find OblProp <%s>" , ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache ) ;
	}


	
	//PRESCACHE
	iRet = ai_pConfig->GetStringValue ( "", ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache ,   pszValue , sizeof(pszValue) ,iLenValue ) ;

	if (iRet == M4_SUCCESS )
	{
		if ( ! ( poCache = ( ClCache * )ai_pNavigator-> GetSubsystemElement ( pszValue , "CACHE") ) )
		{
			SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find cache <%s>, in caches subsystem" , pszValue ) ;
		}

		m_pPresentationCache = poCache;

		//Si no la caché de MD está a NONE => la marcamos como inactiva.
		if (M4CL_CACHE_NONE == m_pRegistry->GetMDDiskCache())
		{
			//Llamamos al comando CHANGE_CACHE_STATUS del subsistema de la caché ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache.
			sprintf (acAux , "%d" , ClServiceBaseSS::SS_INACTIVE ) ;
			poParam=ClAdminCommandTag::GetNewAdminCommandTag (ClCacheSubSystem::CACHE_STATUS, "CACHE_STATUS", adminShort, acAux);
			oParamList.push_back(poParam);

			ClSSInterfaz * pSubsBase = (ClSSInterfaz *) ai_pNavigator->GetSubsystemByName(pszValue);

			pSubsBase->ExecuteCommand(pSubsBase -> GetssId () , ClCacheSubSystem::CHANGE_CACHE_STATUS, oParamList, oResultList);
		}

	}
	else
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache  << "METADATA_SUBSYSTEM" );
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find OblProp <%s>" , ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache ) ;
	}


	//MAPCACHE
	iRet = ai_pConfig->GetStringValue ( "", ClExecutiveForMDCaches::M4ExecutiveForMDMapCache ,   pszValue , sizeof(pszValue) ,iLenValue ) ;

	if (iRet == M4_SUCCESS )
	{
		if ( ! ( poCache = ( ClCache * )ai_pNavigator-> GetSubsystemElement ( pszValue , "CACHE") ) )
		{
			SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find cache <%s>, in caches subsystem" , pszValue ) ;
		}

		m_pMapCache = poCache;

		//Si no la caché de MD está a NONE => la marcamos como inactiva.
		if (M4CL_CACHE_NONE == m_pRegistry->GetMDDiskCache())
		{
			//Llamamos al comando CHANGE_CACHE_STATUS del subsistema de la caché ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache.
			sprintf (acAux , "%d" , ClServiceBaseSS::SS_INACTIVE ) ;
			poParam=ClAdminCommandTag::GetNewAdminCommandTag (ClCacheSubSystem::CACHE_STATUS, "CACHE_STATUS", adminShort, acAux);
			oParamList.push_back(poParam);

			ClSSInterfaz * pSubsBase = (ClSSInterfaz *) ai_pNavigator->GetSubsystemByName(pszValue);

			pSubsBase->ExecuteCommand(pSubsBase -> GetssId () , ClCacheSubSystem::CHANGE_CACHE_STATUS, oParamList, oResultList);
		}

	}
	else
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", ClExecutiveForMDCaches::M4ExecutiveForMDMapCache  << "METADATA_SUBSYSTEM" );
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find OblProp <%s>" , ClExecutiveForMDCaches::M4ExecutiveForMDMapCache ) ;
	}


	//DATACACHE
	iRet = ai_pConfig->GetStringValue ( "", ClExecutiveForMDCaches::M4ExecutiveForMDDataCache ,   pszValue , sizeof(pszValue) ,iLenValue ) ;
	
	if (iRet == M4_SUCCESS)
	{
		if ( ! ( poCache = ( ClCache * )ai_pNavigator-> GetSubsystemElement ( pszValue , "CACHE") ) )
		{
			SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find cache <%s>, in caches subsystem" , pszValue ) ;
		}

		// the order in OBL should be first CMCRCache and second CSCRCache!!
		poDataCache = poCache;

		//Si no la caché de MD está a NONE => la marcamos como inactiva.
		if (M4CL_CACHE_NONE == m_pRegistry->GetDataDiskCache())
		{
			//Llamamos al comando CHANGE_CACHE_STATUS del subsistema de la caché ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache.
			sprintf (acAux , "%d" , ClServiceBaseSS::SS_INACTIVE ) ;
			poParam=ClAdminCommandTag::GetNewAdminCommandTag (ClCacheSubSystem::CACHE_STATUS, "CACHE_STATUS", adminShort, acAux);
			oParamList.push_back(poParam);

			ClSSInterfaz * pSubsBase = (ClSSInterfaz *) ai_pNavigator->GetSubsystemByName(pszValue);

			pSubsBase->ExecuteCommand(pSubsBase -> GetssId () , ClCacheSubSystem::CHANGE_CACHE_STATUS, oParamList, oResultList);
		}

	}
	else
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", ClExecutiveForMDCaches::M4ExecutiveForMDDataCache << "METADATA_SUBSYSTEM" );
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find OblProp <%s>" , ClExecutiveForMDCaches::M4ExecutiveForMDDataCache ) ;
	}

	//UPDATELISTMAXSIZE
	m4double_t	dAux;
	iRet = ai_pConfig->GetNumericValue ( "", M4_MDSUBSISTEM_UPDATE_LIST_MAX_SIZE , dAux) ;
	
	m4uint32_t iUpdateListMaxSize = 0;
	if (iRet == M4_SUCCESS)
	{
		iUpdateListMaxSize = (m4uint32_t)dAux;
	}
	else
	{
		M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_WARNING , "Configuration property: %0:s not found in Subsystem %1:s",M4_MDSUBSISTEM_UPDATE_LIST_MAX_SIZE << "METADATA_SUBSYSTEM" << iUpdateListMaxSize );
//		SETCODEF ( M4_SRV_SS_OBL_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find OblProp <%s>" , M4_MDSUBSISTEM_UPDATE_LIST_MAX_SIZE ) ;
		iUpdateListMaxSize = 200;
	}

	if (m_pUpdateList == 0)
	{
		//La primera vez traemos todos y luego cortamos. Se podria optimizar, pero es solo una vez...
		m_pUpdateList = new ClMDUpdateList(ClMinusInfiniteDate(), iUpdateListMaxSize);
	}

	m_pObjectService = ClFactM4ObjService::CreateObjService(m_pCMCRCache, m_pCSCRCache, m_pPresentationCache, m_pMapCache, poDataCache, M4_TRUE);
	if (NULL == m_pObjectService)
	{
		return M4_ERROR;
	}

	ILdb* pLDB = m_pObjectService->GetpLDB();

	if( pLDB != NULL )
	{
		// Se establece el directorio de cachés a la bdl
		pLDB->SetCacheDirectory(m_pObjectService->GetpCacheDirectory());
	}

	M4SetLiteralsCacheDirectory(m_pObjectService->GetpCacheDirectory());

	m_pChannelManager = m_pObjectService->GetpChannelManager();

	if (M4_ERROR == TChannelUtil::SetAdmMode(*m_pChannelManager))
	{
		SETCODEF (M4_ERR_SCH_INITIALIZING, ERRORLOG, "SESSIONCH: Error Enabling Adm mode" );
		/*No abortamos.
		ShutDown();
		return M4_ERROR;*/
	}

	RefreshUpdateList(ai_pConfig);

	m_bInitialized = M4_TRUE;

	return M4_SUCCESS;
  //## end ClExecutiveForMDCaches::StartUp%957518527.body
}

m4return_t ClExecutiveForMDCaches::ShutDown ()
{
  //## begin ClExecutiveForMDCaches::ShutDown%957518528.body preserve=yes
	m_pCMCRCache =  m_pCSCRCache = m_pPresentationCache = m_pMapCache = 0;

	if (NULL != m_pRegistry)
	{
		delete m_pRegistry;
		m_pRegistry = NULL;
	}
	
	ClFactM4ObjService::DestroyObjService(m_pObjectService);
	m_pObjectService = NULL;	
	m_pChannelManager = NULL;

	return M4_SUCCESS;
  //## end ClExecutiveForMDCaches::ShutDown%957518528.body
}

m4return_t ClExecutiveForMDCaches::Reset ()
{
  //## begin ClExecutiveForMDCaches::Reset%957518529.body preserve=yes
	return M4_SUCCESS;
  //## end ClExecutiveForMDCaches::Reset%957518529.body
}

m4return_t ClExecutiveForMDCaches::RefreshUpdateList (ClConfiguration *ai_pConfig)
{
  //## begin ClExecutiveForMDCaches::RefreshUpdateList%957518530.body preserve=yes
	m4return_t		ret;
	m4VariantType	vOldRepositoryKey;

	if (!m_pChannelManager)
		return M4_ERROR;

	//Prueba de testeo
	M4_ASSERT(m_pUpdateList);

	if (!m_pUpdateList)
		return M4_ERROR;

	ClCacheDirectory * poCacheDir = ((ClChannelManager *)m_pChannelManager)->GetCacheDirectory();

	if (!poCacheDir)
		return M4_ERROR;

	poCacheDir->DePersistDir();

	vOldRepositoryKey = poCacheDir->GetRepositoryKey();

	if (vOldRepositoryKey.Type != M4CL_CPP_TYPE_STRING_VAR)
	{
		//Si no es correcta cogemos la del mdupdate porque las caches no están activadas.
		vOldRepositoryKey = m_pUpdateList->GetRepositoryKey();
	}

	ret = m_pUpdateList->RefreshList((ClChannelManager *)m_pChannelManager);

	/* Bug 0115934
	Hay que liberar la conexión debase de datos para no gastar recursos
	*/
	ClLConn_Provider	*poPovider = m_pObjectService->GetpConnProvider() ;

	if( poPovider != NULL )
	{
		poPovider->SetConnection( NULL ) ;
	}

	ClMDUpdateDeltasList * poMDUpdateDeltasList = 0;

	ret = m_pUpdateList->CreateUpdateDeltasList( poCacheDir->GetLastModificationDate(), poCacheDir->GetRepositoryKey(), poMDUpdateDeltasList);

	if ( (M4_SUCCESS == ret) && (poMDUpdateDeltasList) )
	{
		ret = poCacheDir->ActualizeCachesByUpdatedList(poMDUpdateDeltasList);

		poCacheDir->PersistDir();

		poCacheDir->PersistCaches();

		//De momento, para asegurarnos de que aquí la RK son iguales.
		poCacheDir->GetRepositoryKey().Set(poMDUpdateDeltasList->GetRepositoryKey());

		delete poMDUpdateDeltasList;
	}

	ret = m_pUpdateList->CutList();

	if (vOldRepositoryKey.Type == M4CL_CPP_TYPE_STRING_VAR)
	{
		if ( (!(m_pUpdateList->GetRepositoryKey().Data.PointerChar)) || (strcmp(vOldRepositoryKey.Data.PointerChar, m_pUpdateList->GetRepositoryKey().Data.PointerChar) != 0) )
		{
			//Ha cambiado la clave de repositorio. Lanzamos el evento 65555 (ver logmsg.ini).
			m4pchar_t pcEvent = "65555";
			m4pchar_t pcMessage = "";
			ret = M4PublishEvent(pcEvent,pcMessage);
		}
	}

	if (m_pUpdateList->GetRepositoryKey().Type == M4CL_CPP_TYPE_STRING_VAR)
	{
		ai_pConfig->SetStringValue( "", "REPOSITORY_KEY", m_pUpdateList->GetRepositoryKey().Data.PointerChar);
	}
	else
	{
		ai_pConfig->SetStringValue( "", "REPOSITORY_KEY", "UNKNOWN");
	}

	return M4_SUCCESS;
  //## end ClExecutiveForMDCaches::RefreshUpdateList%957518530.body
}

ClMDUpdateList * ClExecutiveForMDCaches::GetUpdateList ()
{
  //## begin ClExecutiveForMDCaches::GetUpdateList%957518531.body preserve=yes
	  return m_pUpdateList;
  //## end ClExecutiveForMDCaches::GetUpdateList%957518531.body
}

ClCache *  ClExecutiveForMDCaches::GetCMCRCache ()
{
  //## begin ClExecutiveForMDCaches::GetCMCRCache%957518532.body preserve=yes
	return m_pCMCRCache;
  //## end ClExecutiveForMDCaches::GetCMCRCache%957518532.body
}

ClCache *  ClExecutiveForMDCaches::GetCSCRCache ()
{
  //## begin ClExecutiveForMDCaches::GetCSCRCache%957518533.body preserve=yes
	return m_pCSCRCache;
  //## end ClExecutiveForMDCaches::GetCSCRCache%957518533.body
}

m4return_t ClExecutiveForMDCaches::CleanCaches ()
{
  //## begin ClExecutiveForMDCaches::CleanCaches%957518534.body preserve=yes
	m4return_t ret;

	ClMDUpdateDeltasList * poMDUpdateDeltasList = 0;

	ClCacheDirectory * poCacheDir = ((ClChannelManager *)m_pChannelManager)->GetCacheDirectory();

	if (!poCacheDir)
		return M4_ERROR;

	ret = poCacheDir->CleanCaches(M4_CACHE_CMCR_CACHE_TYPE | M4_CACHE_CSCR_CACHE_TYPE | M4_CACHE_PRES_CACHE_TYPE | M4_CACHE_MAP_CACHE_TYPE);

	return ret;
  //## end ClExecutiveForMDCaches::CleanCaches%957518534.body
}

ClCache *  ClExecutiveForMDCaches::GetPresentationCache ()
{
  //## begin ClExecutiveForMDCaches::GetPresentationCache%957518535.body preserve=yes
	return m_pPresentationCache;
  //## end ClExecutiveForMDCaches::GetPresentationCache%957518535.body
}

ClCache *  ClExecutiveForMDCaches::GetMapCache ()
{
	return m_pMapCache;
}

// Additional Declarations
  //## begin ClExecutiveForMDCaches%391296AE038F.declarations preserve=yes
  //## end ClExecutiveForMDCaches%391296AE038F.declarations

//## begin module%35A24233018B.epilog preserve=yes
//## end module%35A24233018B.epilog
