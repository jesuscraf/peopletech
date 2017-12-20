//## begin module%38ECB2500365.cm preserve=no
//## end module%38ECB2500365.cm

//## begin module%38ECB2500365.cp preserve=no
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
//## end module%38ECB2500365.cp

//## Module: userservicesss%38ECB2500365; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\userservicesss.cpp

//## begin module%38ECB2500365.additionalIncludes preserve=no
//## end module%38ECB2500365.additionalIncludes

//## begin module%38ECB2500365.includes preserve=yes
#include <clsubsystem.hpp>
#include <boomrequest.hpp>
#include <commandrequest.hpp>
#include <subsystemids.hpp>
#include <executive.hpp>
#include <cloblconfiguration.hpp>
#include <exefactory.hpp>
#include <exeforsessionch.hpp>
#include <oltplauncher.hpp>
#include <rawsessionid.hpp>
#include <credentialsfactory.hpp>
#include <m4srvtrace.h>
#include <clservicebasess.hpp>
#include <userservicesss.hpp>
#include <clexecutivebasess.hpp>
#include <sessionidgenerator.hpp>

class ClUSServiceAPI;

//## begin module%38ECB2500365.declarations preserve=no
//## end module%38ECB2500365.declarations

//## begin module%38ECB2500365.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID	"COMSRV"
//## end module%38ECB2500365.additionalDeclarations


// Class ClUSServiceSS 


ClUSServiceSS::ClUSServiceSS (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClUSServiceSS::ClUSServiceSS%895476052.hasinit preserve=no
  //## end ClUSServiceSS::ClUSServiceSS%895476052.hasinit
  //## begin ClUSServiceSS::ClUSServiceSS%895476052.initialization preserve=yes
  : ClServiceBaseSS ( ai_poConfiguration,  ai_poTables, ai_poNavigator )
  //## end ClUSServiceSS::ClUSServiceSS%895476052.initialization
{
  //## begin ClUSServiceSS::ClUSServiceSS%895476052.body preserve=yes
	m_poSessionIDGenerator = NULL ;
  //## end ClUSServiceSS::ClUSServiceSS%895476052.body
}


ClUSServiceSS::~ClUSServiceSS ()
{
  //## begin ClUSServiceSS::~ClUSServiceSS%895476053.body preserve=yes
      if ( m_poService )
	  {
		delete m_poService;

		m_poService = NULL ;
	  }

		if ( m_poServiceQ )
		{
			delete m_poServiceQ;

			m_poServiceQ = NULL ;
		}
  //## end ClUSServiceSS::~ClUSServiceSS%895476053.body
}



//## Other Operations (implementation)
m4return_t ClUSServiceSS::Start ()
{
  //## begin ClUSServiceSS::Start%927628813.body preserve=yes
	ClSystem * poSystem ;
	
	if ( M4_ERROR == ClServiceBaseSS::Start () )
	{
		return M4_ERROR ;
	}

	poSystem = ClSystem::Instance () ;

	AttachObserver ( poSystem ) ;


	return poSystem-> LogOn () ;

  //## end ClUSServiceSS::Start%927628813.body
}

m4return_t ClUSServiceSS::EmptyQueue ()
{
  //## begin ClUSServiceSS::EmptyQueue%930666545.body preserve=yes
	static ClMutex oMutex (M4_TRUE) ;
	ClBoomerangRequest		*boomRequest;
	ClCommandRequest		*usRequest;
	ClServiceQ	*poServiceQ;

	
	if (NULL == m_poService)
		return M4_ERROR ;
	

	poServiceQ = (ClServiceQ *) m_poServiceQ ;

	oMutex.Lock() ;
	poServiceQ -> Lock();

	ClServiceItem		poItem = NULL ;

	while (0 < poServiceQ ->Size())
	{
		if ( M4_SUCCESS == poServiceQ->Pop(poItem , M4_FALSE ,0) )
		{

			boomRequest = (ClBoomerangRequest*)poItem;

			boomRequest->GetRequest(poItem);

			usRequest = (ClCommandRequest *) poItem ;

			usRequest -> SetResult(M4_ERROR);

			boomRequest->Return();
		}

	}

	oMutex.Unlock() ;
	return M4_SUCCESS ;
  //## end ClUSServiceSS::EmptyQueue%930666545.body
}

m4return_t ClUSServiceSS::Initialize ()
{
  //## begin ClUSServiceSS::Initialize%947603787.body preserve=yes
	ClServiceQ * poServiceQ ;

	poServiceQ = m_poServiceQ ;

	m_poSessionIDGenerator  = new ClRawSessionIDGenerator ;

	if( ! ClCredentialsFactory::Instance ( *m_poSessionIDGenerator   ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == ClServiceBaseSS::Initialize () )
	{
		return M4_ERROR ;
	}

	m_poServiceQ = poServiceQ ;

	//El servicio no es un servicio como el resto y por tanto las estadísticas difieren
	m_poVisitor = NULL;

	m_poStatistics  = NULL ;

	return M4_SUCCESS ;
  //## end ClUSServiceSS::Initialize%947603787.body
}

void ClUSServiceSS::SetService (ClUSServiceAPI *ai_poServiceAPI)
{
  //## begin ClUSServiceSS::SetService%954922423.body preserve=yes
  //## end ClUSServiceSS::SetService%954922423.body
}

ClSSInterfaz * ClUSServiceSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClUSServiceSS::GenerateSubsystem%955035285.body preserve=yes
	ClExecutiveBaseSS * poExecutiveSS;


	if ( ai_lType == M4_EXECUTOR_UUSS_SUBSYSTEM)
	{
		poExecutiveSS = new ClUSExecutiveSS( ai_poConfiguration , ai_poTables , m_poNavigator , this) ;

		poExecutiveSS  -> SetUseExceptions ( m_bUseExceptions ) ;

		poExecutiveSS  -> SetServiceQ ( (ClServiceQ*)m_poServiceQ ) ;

		return poExecutiveSS  -> GetInterfaz ();
	}
	else
	{
		SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "ECHO Service Subsystem Don't know how to make subsystem type %d" , ai_lType ) ;
		return NULL ;
	}
	
  //## end ClUSServiceSS::GenerateSubsystem%955035285.body
}

// Additional Declarations
  //## begin ClUSServiceSS%3560106502E7.declarations preserve=yes
  //## end ClUSServiceSS%3560106502E7.declarations

// Class ClSessChServiceSS 

ClSessChServiceSS::ClSessChServiceSS (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClSessChServiceSS::ClSessChServiceSS%895476050.hasinit preserve=no
  //## end ClSessChServiceSS::ClSessChServiceSS%895476050.hasinit
  //## begin ClSessChServiceSS::ClSessChServiceSS%895476050.initialization preserve=yes
  : ClServiceBaseSS ( ai_poConfiguration,  ai_poTables, ai_poNavigator )
  //## end ClSessChServiceSS::ClSessChServiceSS%895476050.initialization
{
  //## begin ClSessChServiceSS::ClSessChServiceSS%895476050.body preserve=yes
  //## end ClSessChServiceSS::ClSessChServiceSS%895476050.body
}


ClSessChServiceSS::~ClSessChServiceSS ()
{
  //## begin ClSessChServiceSS::~ClSessChServiceSS%895476051.body preserve=yes
//OSCAR de momento no lo borro porque esto ocasiona un core en el shutdown del servidor
  /*
	if ( m_poService )
	{
		delete m_poService ;

		m_poService =NULL ;
	}
*/
	if ( m_poServiceQ )
	{
		delete m_poServiceQ ;

		m_poServiceQ = NULL ;
	}
  //## end ClSessChServiceSS::~ClSessChServiceSS%895476051.body
}



//## Other Operations (implementation)
ClSSInterfaz * ClSessChServiceSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClSessChServiceSS::GenerateSubsystem%955035284.body preserve=yes
	ClExecutiveBaseSS * poExecutiveSS;


	if ( ai_lType == M4_EXECUTOR_SESSION_SUBSYSTEM	)
	{
		poExecutiveSS = new ClSessChExecutiveSS( ai_poConfiguration , ai_poTables , m_poNavigator , this) ;

		poExecutiveSS  -> SetUseExceptions ( m_bUseExceptions ) ;

		poExecutiveSS  -> SetServiceQ ( (ClServiceQ*)m_poServiceQ ) ;

		return poExecutiveSS  -> GetInterfaz ();
	}
	else
	{
		SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "ECHO Service Subsystem Don't know how to make subsystem type %d" , ai_lType ) ;
		return NULL ;
	}
  //## end ClSessChServiceSS::GenerateSubsystem%955035284.body
}

// Additional Declarations
  //## begin ClSessChServiceSS%356010B60153.declarations preserve=yes
  //## end ClSessChServiceSS%356010B60153.declarations

// Class ClUSExecutiveSS 

ClUSExecutiveSS::ClUSExecutiveSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClUSExecutiveSS::ClUSExecutiveSS%955035286.hasinit preserve=no
  //## end ClUSExecutiveSS::ClUSExecutiveSS%955035286.hasinit
  //## begin ClUSExecutiveSS::ClUSExecutiveSS%955035286.initialization preserve=yes
  : ClExecutiveBaseSS ( ai_poConfiguratoin,  ai_poTables, ai_poNavigator , ai_poServiceSS)
  //## end ClUSExecutiveSS::ClUSExecutiveSS%955035286.initialization
{
  //## begin ClUSExecutiveSS::ClUSExecutiveSS%955035286.body preserve=yes
  //## end ClUSExecutiveSS::ClUSExecutiveSS%955035286.body
}


ClUSExecutiveSS::~ClUSExecutiveSS ()
{
  //## begin ClUSExecutiveSS::~ClUSExecutiveSS%955035287.body preserve=yes
  //## end ClUSExecutiveSS::~ClUSExecutiveSS%955035287.body
}



//## Other Operations (implementation)
m4return_t ClUSExecutiveSS::Initialize ()
{
  //## begin ClUSExecutiveSS::Initialize%955035288.body preserve=yes
	ClLauncherStats *    poStats ;
//	ClServiceQ *		poServiceQ ;
//	ClOutputQ *			poOutputQ ;
//	ClEngine *			poEngine;
//	ClExecutive	*		poExecutive;
	m4double_t			iNumInstances ;
	m4char_t			szType [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t			iLen ;

	if ( GetConfigStringValue( "TYPE" , szType , OBL_PROPVALUE_MAX_LEN , iLen) == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","TYPE"<< "UUSS_EXECUTOR" );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
		return M4_ERROR ;
	}

	if ( GetConfigNumericValue ( NUM_INSTANCES , iNumInstances ) == M4_SUCCESS )
	{
		SETCODEF ( M4_SRV_STARTUPOBL_ERROR, WARNINGLOG , "NUM_INSTANCES property found in subsystem %s. To define %s Subsystem instances, it is necesary to include this property in subsystem above", szType, szType) ;
	}
	

	m_poStatistics = poStats = new ClLauncherStats ;
/*
	poEngine = NULL;

	poExecutive = new ClExeForUUSS(poEngine);

	//Para que los miembros m_poOutputQ y m_poServiceQ estén inicializados se los ha tenido que dar 
	//el SS de servicio correspondiente en el Generate SS

	m_poLauncher = new ClLauncherForOLTP(  poExecutive , poStats, (ClServiceQ*)m_poServiceQ, (ClOutputQ*)m_poOutputQ );
*/
	ClExeFactory::Create ( M4_EXECUTOR_UUSS_SUBSYSTEM , m_poLauncher ,  poStats , (ClServiceQ*)m_poServiceQ, NULL) ;

	if (! m_poLauncher )
		return M4_ERROR ;

    m_szInstanceOrder = ++m_iNumOfInstances;

	return ClExecutiveBaseSS::Initialize () ;

  //## end ClUSExecutiveSS::Initialize%955035288.body
}

m4return_t ClUSExecutiveSS::Start ()
{
  //## begin ClUSExecutiveSS::Start%955035292.body preserve=yes
	static m4uint32_t iLauncher = 1;
	m4char_t szLauncher [100];
	m4return_t ret ;


	if ( M4_SUCCESS == ( ret = ClExecutiveBaseSS::Start () ) )
	{

		sprintf ( szLauncher  , "User Service Executor number %d" , iLauncher ) ;

//		m_poLauncher -> Start ( szLauncher ) ;
	}

	return ret ;
  //## end ClUSExecutiveSS::Start%955035292.body
}

// Additional Declarations
  //## begin ClUSExecutiveSS%38ECB36B00CD.declarations preserve=yes
m4int32_t ClUSExecutiveSS::m_iNumOfInstances=0;
  //## end ClUSExecutiveSS%38ECB36B00CD.declarations

// Class ClSessChExecutiveSS 

ClSessChExecutiveSS::ClSessChExecutiveSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClSessChExecutiveSS::ClSessChExecutiveSS%955035289.hasinit preserve=no
  //## end ClSessChExecutiveSS::ClSessChExecutiveSS%955035289.hasinit
  //## begin ClSessChExecutiveSS::ClSessChExecutiveSS%955035289.initialization preserve=yes
  : ClExecutiveBaseSS  ( ai_poConfiguratoin,  ai_poTables, ai_poNavigator , ai_poServiceSS)
  //## end ClSessChExecutiveSS::ClSessChExecutiveSS%955035289.initialization
{
  //## begin ClSessChExecutiveSS::ClSessChExecutiveSS%955035289.body preserve=yes
  //## end ClSessChExecutiveSS::ClSessChExecutiveSS%955035289.body
}


ClSessChExecutiveSS::~ClSessChExecutiveSS ()
{
  //## begin ClSessChExecutiveSS::~ClSessChExecutiveSS%955035290.body preserve=yes
  //## end ClSessChExecutiveSS::~ClSessChExecutiveSS%955035290.body
}



//## Other Operations (implementation)
m4return_t ClSessChExecutiveSS::Initialize ()
{
  //## begin ClSessChExecutiveSS::Initialize%955035291.body preserve=yes
	ClLauncherStats *    poStats ;
//	ClServiceQ *		poServiceQ ;
//	ClOutputQ *			poOutputQ ;
	ClEngine *			poEngine;
	ClExecutive	*		poExecutive;
	m4double_t			iNumInstances ;
	m4char_t			szType [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t			iLen ;

	if ( GetConfigStringValue( "TYPE" , szType , OBL_PROPVALUE_MAX_LEN , iLen) == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","TYPE" << "SERVICE_EXECUTOR" );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
		return M4_ERROR ;
	}

	if ( GetConfigNumericValue ( NUM_INSTANCES , iNumInstances ) == M4_SUCCESS )
	{
		SETCODEF ( M4_SRV_STARTUPOBL_ERROR, WARNINGLOG , "NUM_INSTANCES property found in subsystem %s. To define %s Subsystem instances, it is necesary to include this property in subsystem above", szType, szType) ;
	}
	

	m_poStatistics = poStats = new ClLauncherStats ;

	poEngine = NULL;

	poExecutive = new ClExeForSessionChannel(poEngine);

	//Para que los miembros m_poOutputQ y m_poServiceQ estén inicializados se los ha tenido que dar 
	//el SS de servicio correspondiente en el Generate SS

	m_poLauncher = new ClLauncherForOLTP(  poExecutive , poStats, m_poServiceSS);
/*
	ClExeFactory::Create ( M4_EXECUTOR_SESSION_SUBSYSTEM , m_poLauncher ,  poStats , (ClServiceQ*)m_poServiceQ, (ClOutputQ*)m_poOutputQ  ) ;
*/
	if (! m_poLauncher )
		return M4_ERROR ;

    m_szInstanceOrder = ++m_iNumOfInstances;

    return ClExecutiveBaseSS::Initialize () ;
  //## end ClSessChExecutiveSS::Initialize%955035291.body
}

m4return_t ClSessChExecutiveSS::Start ()
{
  //## begin ClSessChExecutiveSS::Start%955035293.body preserve=yes
	static m4uint32_t iLauncher = 1;
	m4char_t szLauncher [100];
	m4return_t ret ;


	if ( M4_SUCCESS == ( ret = ClExecutiveBaseSS::Start () ) )
	{

		sprintf ( szLauncher  , "Session Channel Service Executor number %d" , iLauncher ) ;

//		m_poLauncher -> Start ( szLauncher ) ;
	}

	return ret ;
  //## end ClSessChExecutiveSS::Start%955035293.body
}

// Additional Declarations
  //## begin ClSessChExecutiveSS%38ECB38301F4.declarations preserve=yes
m4int32_t ClSessChExecutiveSS::m_iNumOfInstances=0;
  //## end ClSessChExecutiveSS%38ECB38301F4.declarations

//## begin module%38ECB2500365.epilog preserve=yes
//## end module%38ECB2500365.epilog
