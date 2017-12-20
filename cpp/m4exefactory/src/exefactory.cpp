//## begin module%34BA450401E4.cm preserve=no
//## end module%34BA450401E4.cm

//## begin module%34BA450401E4.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ExeFactory.cpp
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
//## end module%34BA450401E4.cp

//## Module: ExeFactory%34BA450401E4; Package body
//## Subsystem: M4ExeFactory::src%37D781A303AA
//## Source file: d:\eduardoma\m4exefactory\src\exefactory.cpp

//## begin module%34BA450401E4.additionalIncludes preserve=no
//## end module%34BA450401E4.additionalIncludes

//## begin module%34BA450401E4.includes preserve=yes
//	Rose 98 Body Include Dependences
#include <oltplauncher.hpp>
#include <exeformd.hpp>
#include <exeforproxy.hpp>
#include <exeforchannel.hpp>
#include <exeforsessionch.hpp>
#include <launcherforuuss.hpp>
#include <exeforuuss.hpp>
#include <exeform4xml.hpp>
//## end module%34BA450401E4.includes

// LauncherInterface
#include <launcher.hpp>
// ExeFactory
#include <exefactory.hpp>
// m4types
#include <m4types.hpp>
//## begin module%34BA450401E4.declarations preserve=no
//## end module%34BA450401E4.declarations

//## begin module%34BA450401E4.additionalDeclarations preserve=yes
//## end module%34BA450401E4.additionalDeclarations


// Class ClExeFactory 

ClExeFactory::ClExeFactory ()
  //## begin ClExeFactory::ClExeFactory%884605458.hasinit preserve=no
  //## end ClExeFactory::ClExeFactory%884605458.hasinit
  //## begin ClExeFactory::ClExeFactory%884605458.initialization preserve=yes
  //## end ClExeFactory::ClExeFactory%884605458.initialization
{
  //## begin ClExeFactory::ClExeFactory%884605458.body preserve=yes
  //## end ClExeFactory::ClExeFactory%884605458.body
}



//## Other Operations (implementation)
m4return_t ClExeFactory::Create (m4uint32_t ai_uiServiceID, ClLauncherInterface *&ao_pExecutor, ClLauncherStats *ai_poStats, ClServiceQ *ai_poServiceQ, ClOutputQ *ai_poOutputQ)
{
  //## begin ClExeFactory::Create%884605459.body preserve=yes
	m4return_t	res;
	ClEngine	*pEngine;
	ClExecutive	*pExecutive;

	switch(ai_uiServiceID)
	{
	case M4_EXECUTOR_CHANNEL:
		pEngine = NULL;
		pExecutive = new ClExecutiveForChannel(pEngine);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, /*ai_poLog, ai_poServiceQ*/NULL );
		break;
/*	case M4_EXECUTOR_ECHO :
		pEngine = NULL;
		pExecutive = new ClExeForEcho(pEngine);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, ai_poServiceQ, ai_poOutputQ );
		break;

//ADD OSCAR 3-6-99
	case M4_EXECUTOR_TEST :
		pEngine = NULL;
		pExecutive = new ClExecutiveForTest(pEngine);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, NULL, ai_poOutputQ );
		break;
*/
	case M4_EXECUTOR_XML :
		pEngine = NULL;
		pExecutive = new ClExecutiveForM4XML (pEngine);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, /*ai_poLog, ai_poServiceQ*/NULL);
		break;

#ifdef EXE_FACTORY_TEST_EXECUTOR
	case M4_EXECUTOR_METHOD:
		pEngine = NULL;
		pExecutive = new ClGenericExecutiveForMethod(pEngine);
		ao_pExecutor = new ClLauncherForOLTP( pExecutive , ai_poStats, /*ai_poLog, */ai_poServiceQ, ai_poOutputQ );
		break;
	case M4_EXECUTOR_PERSIST:
		pEngine = NULL;
		pExecutive = new ClExecutiveForPersist(pEngine);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, /*ai_poLog, */ai_poServiceQ, ai_poOutputQ );
		break;
	case M4_EXECUTOR_LOAD:
		pEngine = NULL;
		pExecutive = new ClExecutiveForLoad(pEngine);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, /*ai_poLog, */ai_poServiceQ, ai_poOutputQ );
		break;
#endif

	case M4_EXECUTOR_UUSS :
		pEngine = NULL;
		pExecutive = new ClExeForUUSS(pEngine);
		ao_pExecutor = new ClLauncherForUUSS(  pExecutive , ai_poStats, /*ai_poLog, */ai_poServiceQ, ai_poOutputQ);
		break;

	case M4_EXECUTOR_SESSION:
		pExecutive = new ClExeForSessionChannel(NULL);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, /*ai_poLog, ai_poServiceQ*/NULL);
		break;
	case M4_EXECUTOR_PROXY :
		pEngine = NULL;
		pExecutive = new ClExecutiveForProxy(pEngine);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, /*ai_poLog, ai_poServiceQ*/NULL);
		break;
	case M4_EXECUTOR_MD :
		pEngine = NULL;
		pExecutive = new ClExecutiveForMetaData(pEngine);
		ao_pExecutor = new ClLauncherForOLTP(  pExecutive , ai_poStats, /*ai_poLog, ai_poServiceQ*/NULL);
		break;

	default:
		ao_pExecutor = NULL ;
		res = M4_ERROR;
	}

	return 	M4_SUCCESS;
  //## end ClExeFactory::Create%884605459.body
}

//## begin module%34BA450401E4.epilog preserve=yes
//## end module%34BA450401E4.epilog
