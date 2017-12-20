//## begin module%34929457013A.cm preserve=no
//## end module%34929457013A.cm

//## begin module%34929457013A.cp preserve=no
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
//## end module%34929457013A.cp

//## Module: SSFactory%34929457013A; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\ssfactory.cpp

//## begin module%34929457013A.additionalIncludes preserve=no
//## end module%34929457013A.additionalIncludes

//## begin module%34929457013A.includes preserve=yes
#include <m4srvres.hpp>
#include <gettsap.hpp>
#include <subsystemids.hpp>
//#include <m4loadll.hpp>
#include <cachesubsystem.hpp>
#include <m4servenv.hpp>
#include <cljssubsystem.hpp>
#include <clpconfiguration.hpp>
#include <oltpsubsys.hpp>
#include <stdio.h>
#include <ssnames.hpp>
#include <usservice.hpp>
#include <propertiestable.hpp>
#include <clssinterfaz.hpp>
#include <cllocalmonitor.hpp>
#include <subsystems.hpp>
#include <navigatorss.hpp>
#include <ssstats.hpp>
#include <proxysubsys.hpp>
#include <metadatasubsys.hpp>
#include <ldbsubsystem.hpp>
#include <clserviceinfo.hpp>
#include <userss.hpp>
#include <exefactory.hpp>
#include <loadestimatorss.hpp>
#include <controlmemoryss.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <controlpipesmkr.hpp>
#include <servicestats.hpp>
#include <proxspacsss.hpp>
#include <m4xmlsubsys.hpp>
#include <cachessubsystem.hpp>
#include <echoservicess.hpp>
#include <suplementaryservices.hpp>
#include <userservicesss.hpp>
#include <dispatcherss.hpp>
#include <clsubsystem.hpp>
#include <lmsubsystem.hpp>
#include <ssccsubsystems.hpp>
#include <m4srvtrace.h>
#include <tracess.hpp>
#include <snmpss.hpp>

#include "tracerequest.hpp"

#ifdef _UNIX
#include <unistd.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

#else
#include <io.h>
#endif
//## end module%34929457013A.includes

// syncro
#include <syncro.hpp>
// SSFactory
#include <ssfactory.hpp>
// clssfactoryinterfaz
#include <clssfactoryinterfaz.hpp>

class ClConfiguration;
class ClServEnv;
class ClLMStats;
class ClSchedulerListStats;

//## begin module%34929457013A.declarations preserve=no
//## end module%34929457013A.declarations

//## begin module%34929457013A.additionalDeclarations preserve=yes
#define CONNECTION_TYPE "CONNECTION_TYPE"
#define M4_EXECUTOR_JOB_CANCEL_SUBSYSTEM	16
#define MAX_SUBSYSTEMS_CACHED 10

static ClMutex sessMutex (M4_TRUE) ;


#define M4_SRV_TRACE_COMP_ID	"COMSRV"
//## end module%34929457013A.additionalDeclarations


// Class ClSSFactory 


ClSSFactory::ClSSFactory ()
  //## begin ClSSFactory::ClSSFactory%882023712.hasinit preserve=no
  //## end ClSSFactory::ClSSFactory%882023712.hasinit
  //## begin ClSSFactory::ClSSFactory%882023712.initialization preserve=yes
  //## end ClSSFactory::ClSSFactory%882023712.initialization
{
  //## begin ClSSFactory::ClSSFactory%882023712.body preserve=yes
  //## end ClSSFactory::ClSSFactory%882023712.body
}


ClSSFactory::~ClSSFactory ()
{
  //## begin ClSSFactory::~ClSSFactory%882023713.body preserve=yes
  //## end ClSSFactory::~ClSSFactory%882023713.body
}



//## Other Operations (implementation)
ClSSInterfaz * ClSSFactory::GenerateSubsystem (m4uint16_t ai_IType, ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSSFactory::GenerateSubsystem%882023714.body preserve=yes
//	ClSSStatsContainer * poStatsContainer = NULL;
	ClSSImplementation * poStatsContainer = NULL;
	ClSSInterfaz * poSubsystem = NULL ;
	static ClNavigatorSS * poNavigator = new ClNavigatorSS ; ;
//	ClSSImplementation * poSSImplementation = NULL ;


	ClSystem * poSystem = ClSystem::Instance () ;	

	if ( poSubsystem = m_oSubsystemsStorage.GetSubsystem( ai_IType ) )
		return poSubsystem ;


	switch ( ai_IType ) 
	{
	case 	M4_LM_SUBSYSTEM	:	

				poStatsContainer = InitializeLocalMonitorSS( ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Local Monitor Subsystem created" ) ;
				}
				break;

	case 	M4_SESSMANAGER_SUBSYSTEM :

				poStatsContainer = InitializeConnManagerSS( ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Session Manager Subsystem created" ) ;
				}
				break;

	case 	M4_SERVICELIST_SUBSYSTEM :

				poStatsContainer = new ClSuplementaryServicesSS( ai_poConfig, ai_poTables, poNavigator );

				if (poStatsContainer)
				{
					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Suplementary Services List Subsystem created" ) ;
				}
				break ;

	case	M4_ECHO_SERVICE_SUBSYSTEM :

				poStatsContainer = new ClECHOServiceSS( ai_poConfig, ai_poTables, poNavigator );

				if (poStatsContainer)
				{
					poStatsContainer->AttachObserver( poSystem ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Echo Service Subsystem created" ) ;
				}
				break ;

	case	M4_SERVICE_SUBSYSTEM :

				poStatsContainer = InitializeService( ai_poConfig, ai_poTables ) ;
				break;

	case M4_CACHES_SUBSYSTEM :		

				poStatsContainer = InitializeCachesSS( ai_poConfig, ai_poTables ) ;

				if (poStatsContainer)
				{
					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Caches Subsystem created" ) ;
				}
				break ;

	case M4_CACHE_SUBSYSTEM :		

			poStatsContainer = InitializeCacheSS( ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Distributor Subsystem created" ) ;
				}
				break;

	case M4_USERS_SUBSYSTEM :

				poStatsContainer = InitializeUsersSS(ai_poConfig, ai_poTables ) ;

				if (poStatsContainer)
				{
					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Users Subsystem created" ) ;
				}
				break ;

	case M4_USER_SUBSYSTEM :

				poStatsContainer = InitializeUserSS( ai_poConfig, ai_poTables ) ;
				break ;

	case M4_SESSION_SUBSYSTEM :

				poStatsContainer = InitializeSesionSS( ai_poConfig, ai_poTables ) ;
				break ;

	case M4_LDB_SUBSYSTEM :

				poStatsContainer = new ClLDBSubsystem( (ClConfiguration * ) ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz *)this ) ;
					poStatsContainer->AttachObserver(poSystem) ;

					//Registrar System a los eventos de la LDB
					((ClSSImplementation *) poStatsContainer)->AttachObserver( poSystem ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "LDB Subsystem created" ) ;
					cout << "LDB Subsystem created" << endl ;
				}

				break ;

	case M4_SESS_CH_SERVICE_SUBSYSTEM :

				poStatsContainer = InitializeSessChServiceSS( ai_poConfig, ai_poTables ) ;
				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz *)this ) ;
					poStatsContainer->AttachObserver( poSystem ) ;
				}
				break ;

	case M4_UUSS_SERVICE_SUBSYSTEM :

				poStatsContainer = InitializeUSServiceSS( ai_poConfig, ai_poTables ) ;

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz *)this ) ;
					poStatsContainer->AttachObserver( poSystem ) ;
				}
				break ;

	case M4_EXECUTOR_NODB_SUBSYSTEM	:
	case M4_EXECUTOR_UUSS_SUBSYSTEM	:

				poStatsContainer = InitializeUSExeSS( ai_poConfig, ai_poTables ) ;
				break ;

	case M4_PROXY_SUBSYSTEM :

				poStatsContainer = new ClProxySubSystem( ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					poStatsContainer->AttachObserver( poSystem ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Proxy Subsystem created" ) ;
					cout << "Proxy Subsystem created" << endl ;
				}
				break ;

	case M4_METADATA_SUBSYSTEM :

				poStatsContainer = new ClMetaDataSubsystem( ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					poStatsContainer->AttachObserver( poSystem ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Metadata Subsystem created" ) ;
					cout << "Metadata Subsystem created" << endl ;
				}
				break ;

	case M4_JOB_SCHEDULER_SUBSYSTEM:

				poStatsContainer = new ClJSSubsystem(ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Job Scheduler Subsystem created" ) ;
					cout << "Job Scheduler Subsystem created" << endl;
				}
				break ;

	case M4_SUPERVISORS_SUBSYSTEM :

//				poSSImplementation = new ClSupervisorsSS(ai_poConfig , ai_poTables) ;
				poStatsContainer = new ClSupervisorsSS(ai_poConfig, ai_poTables, poNavigator) ;

				if (poStatsContainer)
				{
					poStatsContainer -> SetFactory ( (ClSSFactoryInterfaz *) this ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Supervisors Subsystem created" ) ;
					cout << "Supervisors Subsystem created" << endl ;
				}
				break ;

	case M4_SUPERVISOR_SUBSYSTEM :

//				poSSImplementation = new ClSupervisorSS(ai_poConfig , ai_poTables) ;
				poStatsContainer = new ClSupervisorSS( ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					poStatsContainer -> SetFactory ( (ClSSFactoryInterfaz *) this ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Supervisor Subsystem created" ) ;
					cout << "Supervisor Subsystem created" << endl ;
				}
				break ;

	case M4_OLTP_SUBSYSTEM :
			
				poStatsContainer = new ClOLTPSubsystem( ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					poStatsContainer->AttachObserver( poSystem ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "OLTP Subsystem created" ) ;
					cout << "OLTP Subsystem created" << endl ;
				}
				break;

	case M4_LOAD_ESTIMATOR_SUBSYSTEM :
		
				poStatsContainer = new ClLoadEstimatorSS( ai_poConfig , ai_poTables , poNavigator ) ;

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz *)this ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Load Estimator Subsystem created" ) ;
					cout << "Load Estimator Subsystem created" << endl ;
				}
				break ;

	case M4_MEMORY_CONTROL_SUBSYSTEM :

				poStatsContainer = new ClCotrolMemorySS( ai_poConfig, ai_poTables, poNavigator ) ;

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz *)this ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Memory Controller Subsystem created" ) ;
					cout << "Memory Controller Subsystem created" << endl ;
				}
				break ;

	case M4_PROXY_SPACES_SUBSYSTEM :

				poStatsContainer = new ClProxySpacesSS( ai_poConfig, ai_poTables, /*(ClNavigatorFirewall*)*/poNavigator );

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz*)this );

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Proxy Spaces Subsystem created" ) ;
					cout << "Proxy Spaces Subsystem created" << endl;
				}
				break ;

	case M4_XML_SUBSYSTEM : 
				
				poStatsContainer = new ClM4XMLSubSystem( poNavigator, ai_poConfig, ai_poTables ) ;

				if (poStatsContainer)
				{
					poStatsContainer->AttachObserver( poSystem ) ;

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "XML Subsystem created" ) ;
					cout << "XML Subsystem created" << endl ;
				}
				break ;

	case M4_TRACE_SUBSYSTEM :

				poStatsContainer = new ClTraceSS( ai_poConfig, ai_poTables, poNavigator) ;

				if (poStatsContainer)
				{
					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Trace Subsystem created" ) ;
					cout << "Trace Subsystem created" << endl ;
				}
				break ;

/*
	case M4_DMS_SUBSYSTEM	:
				poStatsContainer = 
					(ClSSImplementation*) new ClDMSSSLN( ai_poConfig, ai_poTables, poNavigator );

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz*)this );

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "DMS Subsystem created" ) ;
					cout << "DMS Subsystem created" << endl ;
				}
				break ;

	case M4_DMS_MANAGER_SUBSYSTEM :

				poStatsContainer = new ClDMSSSManager( ai_poConfig, ai_poTables, poNavigator );

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz*)this );

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "DMS Manager Subsystem created" ) ;
					cout << "DMS Manager Subsystem created" << endl ;
				}
				break ;
*/
	case M4_DISPATCHERSS_SUBSYSTEM:

				poStatsContainer = new ClDispatcherSS( ai_poConfig, ai_poTables, poNavigator );

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz*)this );

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Dispatcher Subsystem created" ) ;
					cout << "Dispatcher Subsystem created" << endl ;
				}
				break ;

	case M4_SNMP_SUBSYSTEM:

				poStatsContainer = new M4SnmpSS( ai_poConfig, ai_poTables, poNavigator );

				if (poStatsContainer)
				{
					poStatsContainer->SetFactory( (ClSSFactoryInterfaz*)this );

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "SNMP Subsystem created" ) ;
					cout << "SNMP Subsystem created" << endl ;
				}
				break ;

/*
	case M4_DMSFS_SUBSYSTEM:

				poStatsContainer = new ClDMSSSFS( i_poConfig, ai_poTables, poNavigator ;

				if (poStatsContainer)
				{
					poStatsContainer ->SetFactory( (ClSSFactoryInterfaz*) this );

					SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "DMS File System Subsystem created" ) ;
					cout << "DMS File System Subsystem created" << endl ;
				}
				break ;
*/
		
	default :
			
//			SETCODEF ( M4_SRV_SS_TABLES_ERROR , ERRORLOG , "Trying to generate a wrong type subsystem especified, type = %d" , ai_IType ) ;

			return NULL ;
	}	
	
	if (! poStatsContainer ) 
	{
		//					SETCODEF ( M4_SRV_SS_FACTORY_ERROR , DEBUGINFO , "Failure on subsystem generation. Subsystem type = %d" , ai_IType ) ;
		return NULL ;
	}

	poSubsystem = poStatsContainer -> GetInterfaz () ;

	poSubsystem -> AttachInterfaceImplementation( poStatsContainer ) ;

	poSubsystem -> SetType ( ai_IType ) ;

	poSubsystem -> SetConfiguration ( ai_poConfig ) ;

	return poSubsystem ;
  //## end ClSSFactory::GenerateSubsystem%882023714.body
}

void ClSSFactory::InitializeComponent (ClConfiguration *ai_poConfig)
{
  //## begin ClSSFactory::InitializeComponent%882023715.body preserve=yes
  //## end ClSSFactory::InitializeComponent%882023715.body
}

ClSSImplementation* ClSSFactory::InitializeLocalMonitorSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
{
  //## begin ClSSFactory::InitializeLocalMonitorSS%882023719.body preserve=yes
	ClLocalMonitorSS * poSubsystem ;
	ClLocalMonitor * poLocalMonitor ;
	ClLMStats * poStats;

/*
	El logon se hace al llamar a start no a initialize
	poSystem = ClSystem::Instance ( ) ;

	if ( M4_ERROR == poSystem -> LogOn ( ) )
	{
		return NULL ;
	}
*/

	poStats = new ClLMStats ;
	
	poSubsystem = new ClLocalMonitorSS (ai_poConfig , ai_poTables , ai_poNavigator);

	poLocalMonitor = ClLocalMonitor::Instance ( ) ;

	poLocalMonitor -> SetStats ( poStats ) ;

	poSubsystem -> SetLocalMonitor ( poLocalMonitor ) ;

	SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Comunication Local Monitor Subsystem created" ) ;

	cout << "Comunication Local Monitor Subsystem created" << endl ;


  return poSubsystem ;

  //## end ClSSFactory::InitializeLocalMonitorSS%882023719.body
}

ClSSImplementation * ClSSFactory::InitializeService (ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeService%882023720.body preserve=yes
  ClServiceBaseSS * poSubsystem ;
  ClLocalMonitor * poLocalMonitor ;
  double iNumService ;
  m4return_t iRet ;
  ClServiceInterface * poService ;
  m4char_t szServiceId [OBL_PROPERTY_MAX_LEN] ;
  size_t iLen ;
  ClNavigatorFirewall * poNavigator = new ClNavigatorSS() ;


	poSubsystem = new ClServiceBaseSS (ai_poConfig , ai_poTables , poNavigator);

	iRet = ai_poConfig -> GetStringValue ( "" , SERVICE_ID , szServiceId , OBL_PROPERTY_MAX_LEN , iLen ) ;

	if ( iRet == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_WARNING , "Configuration property: %0:s not found in Subsystem %1:s","SERVICE_ID"<< "SERVICE_SUBSYSTEM" );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "None Service Id property especified for Service Type Subsystem" );

		return NULL ;
	}

	iNumService = ai_poTables -> GetId ( szServiceId ) ;

	if ( iNumService == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_TABLES_PROP_ERROR, "No %0:s value, specified for property: '%1:s' in obl '%2:s'","ID" << szServiceId <<"tables" );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Physical Service Id property %s not defined" , szServiceId );

		return NULL ;
	}

	poLocalMonitor = ClLocalMonitor::Instance ( ) ;

	poService = poLocalMonitor -> CreateService ( (m4uint32_t) iNumService ) ;

	if ( ! poService )
	{
		return NULL ;
	}

//	DEL	EMN	990524	poSubsystem -> SetServiceQueue ( poService -> GetServiceQ ( ) ) ;

	poSubsystem -> SetService ( poService ) ;

	poSubsystem -> SetServiceQ ( poService -> GetServiceQ() ) ;

	if ( M4_ERROR == poSubsystem -> Initialize ( ) )
	{
		return NULL ;
	}

	SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Service Subsystem created" ) ;

	cout << "\nPhysical service " << szServiceId << " has been created" << endl ;

  return poSubsystem ;

  //## end ClSSFactory::InitializeService%882023720.body
}

m4return_t ClSSFactory::ShutDownSubsystem (ClSSInterfaz *ai_poSubsystem)
{
  //## begin ClSSFactory::ShutDownSubsystem%882023721.body preserve=yes
	ClLocalMonitor * poLocalMonitor = NULL ;
	m4uint32_t iType ;
	m4return_t iRet = M4_SUCCESS ;





	iType = ai_poSubsystem -> GetType ( ) ;

	poLocalMonitor = ClLocalMonitor::Instance ( ) ;


	switch ( iType )
	{
	case M4_LM_SUBSYSTEM	:	


		break ;

	case M4_RECEIVER_SUBSYSTEM :

		break;

	case M4_SENDER_SUBSYSTEM	:	

		break;

	case M4_SENDERLIST_SUBSYSTEM	:	

		break;

	case M4_ACEPTOR_SUBSYSTEM :

		break;

	case M4_SESSMANAGER_SUBSYSTEM :

		break;

	case M4_SCHEDULER_SUBSYSTEM :

		break;

	case M4_DISTRIBUTOR_SUBSYSTEM :

		break;

	case M4_SERVICE_SUBSYSTEM :

		break;

	case M4_ADMIN_ACCEPTOR_SUBSYSTEM :

		break ;
	case M4_EXECUTOR_NODB_SUBSYSTEM	:
	case M4_EXECUTOR_UUSS_SUBSYSTEM	:
//	case M4_LAUNCHER_SUBSYSTEM :
/*
		poLauncher = ( (ClExecutiveSS * ) poStatsContainer ) -> GetLauncher ( ) ;

		poLauncher -> Terminate ( );

		poLauncher -> Destroy ( ) ;

		delete ( poLauncher ) ;

		( (ClExecutiveSS * ) poStatsContainer ) -> SetLauncher ( NULL ) ;

		//iRet = poLocalMonitor -> ShutDownLauncher ( ( (ClExecutiveSS * ) poStatsContainer ) -> GetLauncher ( ) ) ;
*/
		break;

	case M4_USERS_SUBSYSTEM :

		break ;

	case M4_USER_SUBSYSTEM :

		break ;

	case M4_SESSION_SUBSYSTEM	:

	
		iRet = ai_poSubsystem -> ShutDown ( ) ;

		m_oSubsystemsStorage.StoreSessionForWaiting ( ai_poSubsystem ) ;

		return 	iRet ;

		break ;

	case M4_ROLE_SUBSYSTEM :


		break ;

	case M4_LDB_SUBSYSTEM :

		break ;

	case M4_UUSS_SERVICE_SUBSYSTEM :

		break ;

	case M4_REQUEST_SUBSYSTEM :

		iRet = ai_poSubsystem -> ShutDown ( ) ;

		m_oSubsystemsStorage.StoreSubsystem ( ai_poSubsystem ) ;

		return 	iRet ;

		break ;

	case M4_SERVER_SUBSYSTEM :

		return M4_SUCCESS ;
	}				

	if ( iRet == M4_ERROR )
	{
		return M4_ERROR ;
	}


	iRet =  ai_poSubsystem -> ShutDown ( ) ;
	if( iRet == M4_ERROR )
	{
		return M4_ERROR ;
	}


	delete ai_poSubsystem ;

	return iRet ;
  //## end ClSSFactory::ShutDownSubsystem%882023721.body
}

ClSSImplementation* ClSSFactory::InitializeConnManagerSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
{
  //## begin ClSSFactory::InitializeConnManagerSS%882177029.body preserve=yes
  ClSessManagerSS * poSubsystem = NULL ;



	poSubsystem = new ClSessManagerSS (ai_poConfig, ai_poTables , ai_poNavigator);

	return poSubsystem ;


  //## end ClSSFactory::InitializeConnManagerSS%882177029.body
}

ClSSImplementation* ClSSFactory::InitializeCacheSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, m4pchar_t ao_szCacheName)
{
  //## begin ClSSFactory::InitializeCacheSS%882177036.body preserve=yes
	ClCacheSubSystem * poSubsystem = NULL;
	m4char_t szCacheName [ OBL_PROPERTY_MAX_LEN ] ;
	m4size_t iLen ;


	
	if ( M4_ERROR == ai_poConfig -> GetStringValue ( "" , CACHE_NAME , szCacheName , OBL_PROPERTY_MAX_LEN , iLen ) )
	{
		M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_WARNING , "Configuration property: %0:s not found in Subsystem %1:s","CACHE_NAME" << "CACHE_SUBSYSTEM" << M4_CACHE_TREE );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "none cache name for cache, it was named %s" , M4_CACHE_TREE ) ;

		strcpy ( szCacheName , M4_CACHE_TREE ) ;	
	}

	poSubsystem = new ClCacheSubSystem ( ai_poConfig, ai_poTables , ai_poNavigator );

	poSubsystem -> GetInterfaz () -> SetName ( szCacheName ) ;


	return poSubsystem ;
  //## end ClSSFactory::InitializeCacheSS%882177036.body
}

ClSSImplementation * ClSSFactory::InitializeLauncherSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeLauncherSS%884599287.body preserve=yes
	ClExecutiveBaseSS * poSubsystem ;
	ClLauncherInterface * poLauncher ;
	ClSystem * poSystem ;
	double lServiceId ;
	m4char_t szType [ OBL_PROPERTY_MAX_LEN ] ;
	m4return_t iRet;
	m4int16_t iType ;
	ClNavigatorSS * pNavigator = NULL ;
	ClLocalMonitor * poLocalMonitor = NULL ;
	m4char_t szServiceId [OBL_PROPERTY_MAX_LEN] ;
	size_t iLen ;
	m4char_t szHolderType [MAXNAMEPROP] ;

	//Get executor type from configuration

	iRet = ai_poConfig -> GetType ( szType , OBL_PROPERTY_MAX_LEN ) ;

	if ( iRet == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","TYPE" << "EXECUTOR ??" );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "None executor type defined, property %s missed", M4_SUBSYSTEM_TYPE ) ;

		return NULL ;
	}

	//Get service number to attach the executor

	iRet = ai_poConfig -> GetStringValue ( "" , SERVICE_ID , szServiceId , OBL_PROPERTY_MAX_LEN , iLen ) ;

	if ( iRet == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", SERVICE_ID << szType );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "None Service Id especified for executor, property %s missed", SERVICE_ID ) ;

		lServiceId = 1 ;
	}

	lServiceId = ai_poTables -> GetId ( szServiceId ) ;

	if ( lServiceId == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_TABLES_PROP_ERROR, "No %0:s value, specified for property: '%1:s' in obl '%2:s'","ID" << szServiceId << "tables");
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Physical Service Id : %s not found in physical services table", szServiceId ) ;

		return NULL ;
	}
	//Get Id of executor type property

	iType = ai_poTables -> GetId ( szType ) ;

	if ( iType == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_TABLES_ERROR , ERRORLOG , "No Id on tables of subsystem type %s" , szType ) ;

		return NULL ;
	}
	
	//Call to local Monitor CreateLauncher function

	poLocalMonitor = ClLocalMonitor::Instance ( ) ;

//	pNavigator = new ClNavigatorSS ;

	poLauncher = poLocalMonitor -> CreateLauncher ( (m4uint32_t) lServiceId , (m4char_t) iType , pNavigator, ai_poConfig );//, szDllName , szFuncName ) ;

	if  (! poLauncher )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "Launcher type %s not created" ,szType  ) ;

		return NULL ;
	}


	poSubsystem = new ClExecutiveBaseSS (ai_poConfig, ai_poTables , NULL , NULL);

//	poSubsystem -> SetLauncher ( poLauncher ) ;

	poSubsystem -> Initialize ( ) ;



/*----------------------------------------------------------
	//Registrar las estadísticas a las del subsystema de ejecutores
-------------------------------------------------------------*/
	switch ( (m4uint32_t)lServiceId )
	{
	case 1: //	OLTP = 1
		strcpy ( szHolderType , "OLTP_SERVICE" ) ;
		break;

	case 2: //	METADATA = 2
		strcpy ( szHolderType , "METADATA_SERVICE" );
		break;

	case 3: //	SESSION = 3
		strcpy ( szHolderType , "SESSION_CHANNEL_SERVICE" );
		break;

	case 4: //	PROXY = 4
		strcpy ( szHolderType , "CONVERSATIONAL_SERVICE" ) ;
		break;

	case 5: //	ECHO = 5
	case 6: //	FRONTON = 6
		strcpy ( szHolderType , "ECHO_SERVICE" );
		break;

	case 7: //	JS_LISTENER = 7
		strcpy ( szHolderType , "JOB_SCHEDULER_LISTENER_SERVICE" ) ;
		break;

	case 8: //	JS_EXECUTOR = 8
		strcpy ( szHolderType , "JOB_SCHEDULER_EXECUTOR_SERVICE") ;
		break;

	case 9: //	FAST_JS_EXECUTOR =9
		break;

	case 10: //	JS_CANCEL = 10
		strcpy ( szHolderType , "JOB_SCHEDULER_CANCEL_SERVICE" ) ;
		break;
	}

/*----------------------------------------------------------
	//Registrar las estadísticas a las del subsystema de ejecutores
-------------------------------------------------------------*/
	switch ( (m4uint32_t)lServiceId )
	{
	case 1: //	OLTP = 1
		strcpy ( szHolderType , "OLTP_SERVICE" ) ;
		break;

	case 2: //	METADATA = 2
		strcpy ( szHolderType , "METADATA_SERVICE" );
		break;

	case 3: //	SESSION = 3
		strcpy ( szHolderType , "SESSION_CHANNEL_SERVICE" );
		break;

	case 4: //	PROXY = 4
		strcpy ( szHolderType , "CONVERSATIONAL_SERVICE" ) ;
		break;

	case 5: //	ECHO = 5
	case 6: //	FRONTON = 6
		strcpy ( szHolderType , "ECHO_SERVICE" );
		break;

	case 7: //	JS_LISTENER = 7
		strcpy ( szHolderType , "JOB_SCHEDULER_LISTENER_SERVICE" ) ;
		break;

	case 8: //	JS_EXECUTOR = 8
		strcpy ( szHolderType , "JOB_SCHEDULER_EXECUTOR_SERVICE") ;
		break;

	case 9: //	FAST_JS_EXECUTOR =9
		break;

	case 10: //	JS_CANCEL = 10
		strcpy ( szHolderType , "JOB_SCHEDULER_CANCEL_SERVICE" ) ;
		break;
	}

	poSystem = ClSystem::Instance () ;
/*
	if ( poListSubsystem = poSystem -> GetSubsystemRoot () -> FindSubsystemByName ( szHolderType  ) )
	{
		( ( ClServiceStats *) ((ClSSStatsContainer*)poListSubsystem -> GetSubsystem ( ) ) -> GetStats ( ) ) -> AddActiveClassStat ( ( ClActiveClassStats * ) poSubsystem -> GetStats ( ) ) ;
	}
*/
// >>> EFV 230399
	// the launcher have to delete the navigator (shutdown)
	// delete pNavigator ;
// <<< EFV 230399

	cout << "\nLauncher of class " << szType  << " attached to the service " << szServiceId  << "\n" << flush;

	return poSubsystem ;
  //## end ClSSFactory::InitializeLauncherSS%884599287.body
}

ClSSImplementation * ClSSFactory::InitializeUsersSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeUsersSS%885483669.body preserve=yes
	ClNavigatorSS	*poNavigatorSS = new ClNavigatorSS  ;

	ClUsersSS		*poSubsystem = new ClUsersSS (  ai_poConfig , ai_poTables ,poNavigatorSS ) ;

	cout << "Users Subsystem created" << endl ;
	
	return poSubsystem ;
  //## end ClSSFactory::InitializeUsersSS%885483669.body
}

ClSSImplementation * ClSSFactory::InitializeUserSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeUserSS%885483670.body preserve=yes
    static ClNavigatorSS	*poNavigatorSS = new ClNavigatorSS  ;

	ClIdentitySS	*poSubsystem = new ClIdentitySS (ai_poConfig , ai_poTables, poNavigatorSS );

	return poSubsystem ;

  //## end ClSSFactory::InitializeUserSS%885483670.body
}

ClSSImplementation * ClSSFactory::InitializeSesionSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeSesionSS%885483671.body preserve=yes
    static ClNavigatorSS	*poNavigatorSS = new ClNavigatorSS  ;

    ClSessionSS		*poSubsystem = new ClSessionSS (ai_poConfig , ai_poTables, poNavigatorSS );

	return poSubsystem ;
  //## end ClSSFactory::InitializeSesionSS%885483671.body
}

ClSSImplementation * ClSSFactory::InitializeSessChServiceSS (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeSessChServiceSS%895476026.body preserve=yes
	ClLocalMonitor * poLocalMonitor =NULL ;
	m4int32_t iNumService ;
	m4return_t iRet ;
	ClServiceInterface * poService = NULL ;
	ClSystem * poSystem = NULL ;
	m4char_t szServiceId [OBL_PROPERTY_MAX_LEN];
	size_t iLen ;

	poSystem = ClSystem::Instance ( ) ;


	ClNavigatorSS		*poNavigatorSS = new ClNavigatorSS  ;
	ClSessChServiceSS	*poSessChServiceSS = new ClSessChServiceSS ( ai_poConfiguration,  ai_poTables, poNavigatorSS );

	iRet = ai_poConfiguration -> GetStringValue ( "" , SERVICE_ID , szServiceId , OBL_PROPERTY_MAX_LEN , iLen ) ;

	if ( iRet == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s",SERVICE_ID<< "SESSION_CHANNEL_SERVICE" );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "None Service Id property especified for Service Type Subsystem" );

		return NULL ;
	}

	iNumService = ai_poTables -> GetId ( szServiceId ) ;

	if ( iNumService == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_TABLES_PROP_ERROR, "No %0:s value, specified for property: '%1:s' in obl '%2:s'","ID" << szServiceId << "tables");
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "None Physical Service Id for Service %s" , szServiceId );

		return NULL ;
	}

	poLocalMonitor = ClLocalMonitor::Instance ( ) ;
  
/* Este código se va a ejecutar en el initialize del servicio

	poService = poLocalMonitor -> CreateService ( (m4uint32_t) iNumService ) ;

	if ( ! poService )
	{
		return NULL ;
	}

	poSessChServiceSS->SetServiceQ (poService -> GetServiceQ ( ));

	poSessChServiceSS -> SetService ( poService ) ;

	poSessChServiceSS->SetOutputQ(poLocalMonitor -> GetOutputQueue());

	if ( M4_ERROR == poSessChServiceSS -> Initialize ( ) )
	{
		return NULL ;
	}
*/
	SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Session Meta4Object Service Subsystem created" ) ;

	cout << "Session Meta4Object Service Subsystem created" << endl ;


	return poSessChServiceSS ;

  //## end ClSSFactory::InitializeSessChServiceSS%895476026.body
}

ClSSImplementation * ClSSFactory::InitializeUSServiceSS (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeUSServiceSS%895476027.body preserve=yes
	ClUSServiceAPI		*poServiceAPI = NULL;
	ClSSInterfaz 		*poSubsystem ;
	ClServiceQ			*poServiceQueue = NULL ;
	ClServiceStats		*poStats = NULL ;
	ClUSQueue			*poUSQueue = NULL ;
	ClCacheTest			*poCacheTest = NULL ;
	ClSystem			*poSystem;
	ClLocalMonitor		*poLocalMonitor ;


	ClNavigatorSS	*poNavigatorSS = new ClNavigatorSS  ;
	ClUSServiceSS	*poUSServiceSS = new ClUSServiceSS ( ai_poConfiguration,  ai_poTables, poNavigatorSS );


	poServiceAPI = ClUSServiceAPI::Instance ( )  ;

	if ( ! poServiceAPI )
	{
		return NULL ;
	}

	poUSQueue = new ClUSQueue ;

	poSystem = ClSystem::Instance ( ) ;

	if ( ! ( poSubsystem = poSystem -> FindSubsystemByType  ( M4_USERS_SUBSYSTEM ) ) )
	{
		return NULL ;
	}

	poCacheTest = NULL;

	poServiceAPI -> Initialize ( poUSQueue ) ;

	poUSServiceSS->SetServiceQ (poUSQueue);

	poUSServiceSS -> SetService ( poServiceAPI ) ;

	poLocalMonitor = ClLocalMonitor::Instance ();

/*	if ( M4_ERROR == poUSServiceSS -> Initialize ( ) )
	{
		return NULL ;
	}
*/

	SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Users Service Subsystem created" ) ;


	cout << "Users Service Subsystem created" << endl ;


	return poUSServiceSS ;
  //## end ClSSFactory::InitializeUSServiceSS%895476027.body
}

ClSSImplementation * ClSSFactory::InitializeCachesSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeCachesSS%895659110.body preserve=yes
	ClCachesSubsystem * poStatsContainer = NULL;
	ClNavigatorSS * poNavigatorSS = NULL ;
	ClSystem * poSystem ;
	

	poSystem = ClSystem::Instance ( ) ;

	poNavigatorSS = new ClNavigatorSS  ;

	poStatsContainer = new ClCachesSubsystem ( ai_poConfig , ai_poTables , poNavigatorSS ) ;

	if ( poStatsContainer )
	{
		if ( M4_ERROR == poStatsContainer ->Initialize ( ) )
		{
			return NULL ;
		}
	}

	return poStatsContainer ;
  //## end ClSSFactory::InitializeCachesSS%895659110.body
}

ClSSImplementation* ClSSFactory::InitializeUSExeSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables)
{
  //## begin ClSSFactory::InitializeUSExeSS%895738255.body preserve=yes
return NULL ;
  /*	ClLauncherInterface * poLauncher ;
	ClLogSystem * poLog = NULL ;
	ClLauncherStats * poStats = NULL ;
	ClExecutiveBaseSS * poSubsystem ;
	ClUSServiceAPI * poService = NULL;
	ClSubsystemBase  * poUSServiceSS ;
	ClSystem * poSystem ;
	static m4uint32_t IdThread = 1; 



	//log para el launcher
	poLog = new ClLogSystem ;
	
	//estadísticas para el launcher
	poStats = new ClLauncherStats ;

	//Conseuir la cola de servicio del servicio de usuarios
	poService = ClUSServiceAPI::Instance ( )  ;

	poSystem = ClSystem::Instance ( ) ;

	if ( ! ( poUSServiceSS = poSystem -> GetSubsystemRoot () -> FindSubsystemByType  ( M4_UUSS_SERVICE_SUBSYSTEM ) ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Couldn't find Users subsystem" ) ;

		return NULL ;
	}

	ClExeFactory::Create ( M4_EXECUTOR_UUSS_SUBSYSTEM , poLauncher ,  poStats , ( ( ClUSServiceSS * ) poUSServiceSS ) -> GetServiceQ ( ) , ( ( ClUSServiceSS * ) poUSServiceSS )->GetOutputQ() ) ;

	
//	poLocalMonitor = ClLocalMonitor::Instance ( ) ;


	if ( ! poLauncher )
	{
		return NULL ;
	}

	poSubsystem = new ClExecutiveBaseSS (ai_poConfig , ai_poTables , NULL );

	poSubsystem -> SetConfiguration ( ai_poConfig ) ;

	poSubsystem -> SetLauncher ( poLauncher ) ;

//	poSubsystem -> SetLog ( poLog ) ;

//	poSubsystem -> Initialize ( ) ;





	return poSubsystem ;
	*/
  //## end ClSSFactory::InitializeUSExeSS%895738255.body
}

m4return_t ClSSFactory::Synchronize ()
{
  //## begin ClSSFactory::Synchronize%919764796.body preserve=yes
	return m_oSubsystemsStorage.Synchronize () ;
  //## end ClSSFactory::Synchronize%919764796.body
}

// Additional Declarations
  //## begin ClSSFactory%348FFBC101BB.declarations preserve=yes
  //## end ClSSFactory%348FFBC101BB.declarations

// Class ClSubsystemCaches 





ClSubsystemCaches::ClSubsystemCaches ()
  //## begin ClSubsystemCaches::ClSubsystemCaches%919708146.hasinit preserve=no
  //## end ClSubsystemCaches::ClSubsystemCaches%919708146.hasinit
  //## begin ClSubsystemCaches::ClSubsystemCaches%919708146.initialization preserve=yes
  //## end ClSubsystemCaches::ClSubsystemCaches%919708146.initialization
{
  //## begin ClSubsystemCaches::ClSubsystemCaches%919708146.body preserve=yes
	  m_oExclusion.Init ( ) ;
  //## end ClSubsystemCaches::ClSubsystemCaches%919708146.body
}


ClSubsystemCaches::~ClSubsystemCaches ()
{
  //## begin ClSubsystemCaches::~ClSubsystemCaches%919708147.body preserve=yes
  //## end ClSubsystemCaches::~ClSubsystemCaches%919708147.body
}



//## Other Operations (implementation)
ClSSInterfaz * ClSubsystemCaches::GetSubsystem (m4uint32_t ai_iType)
{
  //## begin ClSubsystemCaches::GetSubsystem%919708148.body preserve=yes
	SUBSYSTEMCACHE::iterator Iterator ;
	ClSSInterfaz * poSubsystem = NULL ;


	m_oExclusion.Lock () ;

	Iterator = m_oCache.find ( ai_iType ) ;

	if ( Iterator == m_oCache.end ( ) )
	{
		m_oExclusion.Unlock () ;
		return NULL ;
	}

	poSubsystem = (*Iterator ).second ;

	m_oCache.erase (Iterator ) ;

	m_oExclusion.Unlock () ;

	poSubsystem -> Activate ( );

	poSubsystem -> Activate () ;

	
	return poSubsystem ;
  //## end ClSubsystemCaches::GetSubsystem%919708148.body
}

void ClSubsystemCaches::StoreSubsystem (ClSSInterfaz *ai_poSubsystem)
{
  //## begin ClSubsystemCaches::StoreSubsystem%919708149.body preserve=yes
	m4uint32_t iType ;
	m4objid_t lId ;
	m4uint32_t iNumSubsystems ;
	ClSSInterfaz * poSubsystem ;
	SUBSYSTEMCACHE::iterator Iterator ;


	iType = ai_poSubsystem->GetType ( ) ;

	m_oExclusion.Lock () ;

	iNumSubsystems = m_oCache.size() ;

	if ( iNumSubsystems > MAX_SUBSYSTEMS_CACHED )
	{

		Iterator = m_oCache.begin() ;

		poSubsystem = ( *Iterator).second ;

		lId = poSubsystem -> GetId ( ) ;

		m_oCache.erase ( Iterator ) ;


		delete poSubsystem ;

	}

	m_oCache.insert ( SUBSYSTEMCACHE::value_type ( iType , ai_poSubsystem ) ) ;

	m_oExclusion.Unlock () ;
/*OSCAR 17-3-99
	eSTE iD SE DEBERÍA DE RECICLAR AUNQUE DE MOMENTO NO SE HAGA
	if ( (lId) && ( M4_REQUEST_SUBSYSTEM != iType ) )
	{
		ClIdGenerator::SetNewId ( lId ) ;
	}
	OSCAR 17-3-99 */

  //## end ClSubsystemCaches::StoreSubsystem%919708149.body
}

void ClSubsystemCaches::StoreSessionForWaiting (ClSSInterfaz *ai_poSessionSS)
{
  //## begin ClSubsystemCaches::StoreSessionForWaiting%919764797.body preserve=yes

	sessMutex.Lock ( ) ;

	m4objid_t lId = ai_poSessionSS -> GetId ( ) ;
	m_oClosedSessions.insert ( SUBSYSTEMCACHE::value_type ( lId, ai_poSessionSS ) ) ;
	sessMutex.Unlock ( ) ;
  //## end ClSubsystemCaches::StoreSessionForWaiting%919764797.body
}

m4return_t ClSubsystemCaches::Synchronize ()
{
  //## begin ClSubsystemCaches::Synchronize%919764798.body preserve=yes
	SUBSYSTEMCACHE::iterator Iterator, IterAux ;
	ClSSInterfaz* poSubsystem = NULL;
	ClStatistic* poStatistics = NULL;

    ClTraceRequest tRequest("SubsystemCaches.Synchronize");
    m4uint32_t iClosedSessions = 0;

    tRequest.StartCount();
	sessMutex.Lock () ;

	Iterator = m_oClosedSessions.begin ( ) ;

	IterAux = Iterator ;

	while ( IterAux != m_oClosedSessions.end ( ) )
	{
		IterAux ++ ;
        iClosedSessions++;

		if ( poSubsystem = (*Iterator).second )
		{

			poSubsystem -> SetConfiguration ( NULL ) ;

			poStatistics = poSubsystem -> GetStats() ;

			if ( poStatistics )
			{
				delete poStatistics ;

			}

			StoreSubsystem ( poSubsystem ) ;

			m_oClosedSessions.erase ( Iterator ) ;
		}
		Iterator = IterAux ;
	}
	sessMutex.Unlock ( ) ;

    tRequest.StopCount();

    if (iClosedSessions > 0)
    {
        char msg[256];
        sprintf (msg, "\tClosedSessions = %lu", iClosedSessions);
        tRequest.SetMsg(msg);
        tRequest.Trace();
    }

	return M4_SUCCESS ;
  //## end ClSubsystemCaches::Synchronize%919764798.body
}

// Additional Declarations
  //## begin ClSubsystemCaches%36D19BFB0122.declarations preserve=yes
  //## end ClSubsystemCaches%36D19BFB0122.declarations

//## begin module%34929457013A.epilog preserve=yes
//## end module%34929457013A.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
