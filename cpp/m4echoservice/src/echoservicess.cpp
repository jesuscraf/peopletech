//## begin module%38DB44E2030D.cm preserve=no
//## end module%38DB44E2030D.cm

//## begin module%38DB44E2030D.cp preserve=no
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
//## end module%38DB44E2030D.cp

//## Module: echoservicess%38DB44E2030D; Package body
//## Subsystem: M4echoservice::src%38DF87440246
//## Source file: F:\integration\m4echoservice\src\echoservicess.cpp

//## begin module%38DB44E2030D.additionalIncludes preserve=no
//## end module%38DB44E2030D.additionalIncludes

//## begin module%38DB44E2030D.includes preserve=yes
#include <exeforecho.hpp>
#include <oltplauncher.hpp>
#include <cloblconfiguration.hpp>
#include <clsubsystem.hpp>
#include <exefortest.hpp>
//## end module%38DB44E2030D.includes

// clexecutivebasess
#include <clexecutivebasess.hpp>
// clservicebasess
#include <clservicebasess.hpp>
// echoservicess
#include <echoservicess.hpp>
//## begin module%38DB44E2030D.declarations preserve=no
//## end module%38DB44E2030D.declarations

//## begin module%38DB44E2030D.additionalDeclarations preserve=yes
const m4pchar_t NUM_EXECUTOR_INSTANCES	=	"NUM_EXECUTOR_INSTANCES";
//const m4uint32_t MAX_M4_ECHO_EXECUTOR_INSTANCES	 =	50 ;
const m4uint32_t MAX_ECHO_SERVICE_SUBSYSTEM_NUM_INSTANCES  =	50 ;


//## end module%38DB44E2030D.additionalDeclarations


// Class ClECHOServiceSS 

ClECHOServiceSS::ClECHOServiceSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClECHOServiceSS::ClECHOServiceSS%953897286.hasinit preserve=no
  //## end ClECHOServiceSS::ClECHOServiceSS%953897286.hasinit
  //## begin ClECHOServiceSS::ClECHOServiceSS%953897286.initialization preserve=yes
  :ClServiceBaseSS (ai_poConfiguratoin, ai_poTables, ai_poNavigator )
  //## end ClECHOServiceSS::ClECHOServiceSS%953897286.initialization
{
  //## begin ClECHOServiceSS::ClECHOServiceSS%953897286.body preserve=yes
  //## end ClECHOServiceSS::ClECHOServiceSS%953897286.body
}


ClECHOServiceSS::~ClECHOServiceSS ()
{
  //## begin ClECHOServiceSS::~ClECHOServiceSS%953897287.body preserve=yes
  //## end ClECHOServiceSS::~ClECHOServiceSS%953897287.body
}



//## Other Operations (implementation)
ClSSInterfaz * ClECHOServiceSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClECHOServiceSS::GenerateSubsystem%953897293.body preserve=yes
	ClExecutiveBaseSS * poExecutiveSS;


	switch ( ai_lType  )
	{
	case M4_EXECUTOR_TEST_SUBSYSTEM :
	case M4_EXECUTOR_ECHO_SUBSYSTEM	:

		poExecutiveSS = new ClECHOExecutorSS( ai_poConfiguration , ai_poTables , m_poNavigator , this) ;

		poExecutiveSS  -> SetUseExceptions ( m_bUseExceptions ) ;

		poExecutiveSS  -> SetServiceQ ( (ClServiceQ*)m_poServiceQ ) ;

		break ;
	default :

		SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "ECHO Service Subsystem Don't know how to make subsystem type %d" , ai_lType ) ;
		return NULL ;
	}
	
	return poExecutiveSS  -> GetInterfaz ();

  //## end ClECHOServiceSS::GenerateSubsystem%953897293.body
}

m4return_t ClECHOServiceSS::Initialize ()
{
  //## begin ClECHOServiceSS::Initialize%954230997.body preserve=yes
	m4double_t iNumInstances ;
	m4char_t			szType [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t	iLen;

	if ( GetConfigStringValue ( "TYPE" , szType , OBL_PROPVALUE_MAX_LEN , iLen ) == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
		return M4_ERROR ;
	}


	if ( GetConfigNumericValue ( NUM_INSTANCES , iNumInstances ) == M4_SUCCESS )
	{

		if ( iNumInstances >= MAX_ECHO_SERVICE_SUBSYSTEM_NUM_INSTANCES  )
		{
			SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "NUM_INSTANCES property of subsystem %s is asigned to %d, and it is and illegal value. Mximun allowed is %d", szType, iNumInstances , MAX_ECHO_SERVICE_SUBSYSTEM_NUM_INSTANCES  ) ;
			return M4_ERROR ;
		}
	}
	
	return ClServiceBaseSS::Initialize () ;
  //## end ClECHOServiceSS::Initialize%954230997.body
}

// Additional Declarations
  //## begin ClECHOServiceSS%38DB43230166.declarations preserve=yes
  //## end ClECHOServiceSS%38DB43230166.declarations

// Class ClECHOExecutorSS 

ClECHOExecutorSS::ClECHOExecutorSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClECHOExecutorSS::ClECHOExecutorSS%953897288.hasinit preserve=no
  //## end ClECHOExecutorSS::ClECHOExecutorSS%953897288.hasinit
  //## begin ClECHOExecutorSS::ClECHOExecutorSS%953897288.initialization preserve=yes
  :ClExecutiveBaseSS (ai_poConfiguratoin, ai_poTables, ai_poNavigator , ai_poServiceSS)
  //## end ClECHOExecutorSS::ClECHOExecutorSS%953897288.initialization
{
  //## begin ClECHOExecutorSS::ClECHOExecutorSS%953897288.body preserve=yes
  //## end ClECHOExecutorSS::ClECHOExecutorSS%953897288.body
}


ClECHOExecutorSS::~ClECHOExecutorSS ()
{
  //## begin ClECHOExecutorSS::~ClECHOExecutorSS%953897289.body preserve=yes
  //## end ClECHOExecutorSS::~ClECHOExecutorSS%953897289.body
}



//## Other Operations (implementation)
m4return_t ClECHOExecutorSS::Initialize ()
{
  //## begin ClECHOExecutorSS::Initialize%953897290.body preserve=yes
	ClLauncherStats *    poStats ;
	ClEngine *			poEngine;
	ClExecutive	*		poExecutive;
	m4double_t			iNumInstances ;
	m4char_t			szType [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t			iLen ;
	m4int16_t iType ;

	if ( GetConfigStringValue( "TYPE" , szType , OBL_PROPVALUE_MAX_LEN , iLen) == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
		return M4_ERROR ;
	}

	if ( GetConfigNumericValue ( NUM_INSTANCES , iNumInstances ) == M4_SUCCESS )
	{
		SETCODEF ( M4_SRV_STARTUPOBL_ERROR, WARNINGLOG , "NUM_INSTANCES property found in subsystem %s. To define %s Subsystem instances, it is necesary to include this property in subsystem above", szType, szType) ;
	}
	
	iType = m_poTables -> GetId ( szType ) ;

	if ( iType == M4_ERROR )
	{
		return M4_ERROR ;
	}

	m_poStatistics = poStats = new ClLauncherStats ;

	poEngine = NULL;

	switch (iType)
	{
	case M4_EXECUTOR_ECHO_SUBSYSTEM :
		poExecutive = new ClExeForEcho(poEngine);
        m_szInstanceOrder = ++m_iNumOfInstances;
		break ;
	case M4_EXECUTOR_TEST_SUBSYSTEM :
		poExecutive = new ClExecutiveForTest(poEngine);
		break ;
	}
	//Para que los miembros m_poOutputQ y m_poServiceQ estén inicializados se los ha tenido que dar 
	//el SS de servicio correspondiente en el Generate SS

	m_poLauncher = new ClLauncherForOLTP(  poExecutive , poStats, /*ai_poLog, (ClServiceQ*)m_poServiceQ*/m_poServiceSS );

	ClExecutiveBaseSS::Initialize() ;

	return M4_SUCCESS ;
  //## end ClECHOExecutorSS::Initialize%953897290.body
}

m4return_t ClECHOExecutorSS::Start ()
{
  //## begin ClECHOExecutorSS::Start%953897291.body preserve=yes
	static m4uint32_t iLauncher = 1;
	m4char_t szLauncher [100];

	sprintf ( szLauncher  , "Executor ECHO number %d" , iLauncher ) ;

	m_poLauncher -> Start ( szLauncher ) ;

	return ClExecutiveBaseSS::Start () ;
  //## end ClECHOExecutorSS::Start%953897291.body
}

// Additional Declarations
  //## begin ClECHOExecutorSS%38DB4346004E.declarations preserve=yes
m4int32_t ClECHOExecutorSS::m_iNumOfInstances=0;
  //## end ClECHOExecutorSS%38DB4346004E.declarations

//## begin module%38DB44E2030D.epilog preserve=yes
//## end module%38DB44E2030D.epilog
