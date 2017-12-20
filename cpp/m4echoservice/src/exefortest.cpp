//## begin module%3438FB3303B6.cm preserve=no
//## end module%3438FB3303B6.cm

//## begin module%3438FB3303B6.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ExecutiveTest.cpp
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
//## end module%3438FB3303B6.cp

//## Module: Executive_Test%3438FB3303B6; Package body
//## Subsystem: M4ExeFactory::src%37D781A303AA
//## Source file: d:\eduardoma\M4ExeFactory\version\src\ExeForTest.cpp

//## begin module%3438FB3303B6.additionalIncludes preserve=no
//## end module%3438FB3303B6.additionalIncludes

//## begin module%3438FB3303B6.includes preserve=yes
#include <navigatorf.hpp>
#include <m4cppobl.hpp>
#include <m4srvres.hpp>
#include <m4trace.hpp>
#include <m4log.hpp>
#include <clconfiguration.hpp>
//## end module%3438FB3303B6.includes

// Executive
#include <executive.hpp>
// Executive_Test
#include <exefortest.hpp>
//## begin module%3438FB3303B6.declarations preserve=no
//## end module%3438FB3303B6.declarations

//## begin module%3438FB3303B6.additionalDeclarations preserve=yes
//## end module%3438FB3303B6.additionalDeclarations


// Class ClEngineCacheForTest 


ClEngineCacheForTest::ClEngineCacheForTest (const m4uint16_t ai_uiTimer)
  //## begin ClEngineCacheForTest::ClEngineCacheForTest%876139083.hasinit preserve=no
      : m_uiLauncher(ai_uiTimer)
  //## end ClEngineCacheForTest::ClEngineCacheForTest%876139083.hasinit
  //## begin ClEngineCacheForTest::ClEngineCacheForTest%876139083.initialization preserve=yes
  //## end ClEngineCacheForTest::ClEngineCacheForTest%876139083.initialization
{
  //## begin ClEngineCacheForTest::ClEngineCacheForTest%876139083.body preserve=yes
  //## end ClEngineCacheForTest::ClEngineCacheForTest%876139083.body
}



//## Other Operations (implementation)
m4uint16_t ClEngineCacheForTest::GetLauncher ()
{
  //## begin ClEngineCacheForTest::GetLauncher%876139084.body preserve=yes
	return (m4uint16_t)m_uiLauncher;
  //## end ClEngineCacheForTest::GetLauncher%876139084.body
}

void ClEngineCacheForTest::SetLauncher (m4uint32_t ai_uiLauncher)
{
  //## begin ClEngineCacheForTest::SetLauncher%876139085.body preserve=yes
	m_uiLauncher = ai_uiLauncher;
  //## end ClEngineCacheForTest::SetLauncher%876139085.body
}

// Additional Declarations
  //## begin ClEngineCacheForTest%3438F90B0089.declarations preserve=yes
  //## end ClEngineCacheForTest%3438F90B0089.declarations

// Class ClExecutiveForTest 


ClExecutiveForTest::ClExecutiveForTest (ClEngine *ai_pEngine)
  //## begin ClExecutiveForTest::ClExecutiveForTest%876470324.hasinit preserve=no
  //## end ClExecutiveForTest::ClExecutiveForTest%876470324.hasinit
  //## begin ClExecutiveForTest::ClExecutiveForTest%876470324.initialization preserve=yes
  : ClExecutive(ai_pEngine) , m_iDelay ( 3 )
  //## end ClExecutiveForTest::ClExecutiveForTest%876470324.initialization
{
  //## begin ClExecutiveForTest::ClExecutiveForTest%876470324.body preserve=yes
  //## end ClExecutiveForTest::ClExecutiveForTest%876470324.body
}


ClExecutiveForTest::~ClExecutiveForTest ()
{
  //## begin ClExecutiveForTest::~ClExecutiveForTest%884769286.body preserve=yes
  //## end ClExecutiveForTest::~ClExecutiveForTest%884769286.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForTest::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExecutiveForTest::StartUp%876470325.body preserve=yes
	m4return_t iRet ;
	double dDelay ;
	
	iRet = ai_pConfig -> GetNumericValue ( "" , "DELAY" , dDelay ) ;

	if ( iRet == M4_SUCCESS )
		m_iDelay = (m4uint32_t)dDelay ;

	return M4_SUCCESS ;
  //## end ClExecutiveForTest::StartUp%876470325.body
}

m4return_t ClExecutiveForTest::Execute (ClImage &aio_pImage)
{
  //## begin ClExecutiveForTest::Execute%876470326.body preserve=yes
	ClEngineArgs			*theArgs;
//	ClEngineCacheForTest	*theCacheForTest;
//	m4uint32_t				launcherID;

	aio_pImage.GetArgs(theArgs);
//	if (NULL != theCacheForTest)
//		launcherID = theCacheForTest->GetLauncher();
	if (NULL != theArgs)
	{
		M4DataStorage		*inDataStorage, *outDataStorage;
		ClDataUnit			*dataUnit;
		m4pchar_t			pszString;

		theArgs->GetInputData(inDataStorage);
		theArgs->GetOutputData(outDataStorage);

		m4Trace ( cout << "TEST +" << endl );

		inDataStorage->InitializeIteration();
		
		outDataStorage->GetNewDataUnit(dataUnit);
		if (NULL != dataUnit)
		{
			dataUnit->SetSize(28);
			pszString = dataUnit->GetBuffer();

			sprintf(pszString, "Input PDU Size: %010d", inDataStorage->Size());
		}

		outDataStorage->GetNewDataUnit(dataUnit);
		if (NULL != dataUnit)
		{
			dataUnit->SetSize(22);
			pszString = dataUnit->GetBuffer();

//			sprintf(pszString, "Launcher:%02d", launcherID);
		}
	}

//	ADD	EMN	980710
/*#ifdef _UNIX
			 struct timeval timeout ={m_iDelay, 0};

			(void) select(2, NULL, NULL, NULL, &timeout);
#else
*/
		m4sleep (m_iDelay ) ;
//#endif
//	END	ADD	EMN
	SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "EXE4TEST: Executing test") ;
	m4Trace ( cout << "TEST -" << endl );
	
	return M4_SUCCESS;
  //## end ClExecutiveForTest::Execute%876470326.body
}

m4return_t ClExecutiveForTest::ShutDown ()
{
  //## begin ClExecutiveForTest::ShutDown%876470327.body preserve=yes
	return 1;
  //## end ClExecutiveForTest::ShutDown%876470327.body
}

m4return_t ClExecutiveForTest::Reset ()
{
  //## begin ClExecutiveForTest::Reset%884854924.body preserve=yes
	return M4_SUCCESS;
  //## end ClExecutiveForTest::Reset%884854924.body
}

m4return_t ClExecutiveForTest::DumpLevel (const m4int16_t ai_iLevel)
{
  //## begin ClExecutiveForTest::DumpLevel%921077498.body preserve=yes
	return M4_SUCCESS;
  //## end ClExecutiveForTest::DumpLevel%921077498.body
}

// Additional Declarations
  //## begin ClExecutiveForTest%343DDF5E0344.declarations preserve=yes
  //## end ClExecutiveForTest%343DDF5E0344.declarations

// Class ClEngineForTest 

ClEngineForTest::ClEngineForTest ()
  //## begin ClEngineForTest::ClEngineForTest%878205110.hasinit preserve=no
  //## end ClEngineForTest::ClEngineForTest%878205110.hasinit
  //## begin ClEngineForTest::ClEngineForTest%878205110.initialization preserve=yes
  //## end ClEngineForTest::ClEngineForTest%878205110.initialization
{
  //## begin ClEngineForTest::ClEngineForTest%878205110.body preserve=yes
  //## end ClEngineForTest::ClEngineForTest%878205110.body
}



//## Other Operations (implementation)
bool ClEngineForTest::Process (ClImage &a_oImage)
{
  //## begin ClEngineForTest::Process%878205111.body preserve=yes
	return true;
  //## end ClEngineForTest::Process%878205111.body
}

m4return_t ClEngineForTest::Init ()
{
  //## begin ClEngineForTest::Init%893773032.body preserve=yes
	return M4_SUCCESS;
  //## end ClEngineForTest::Init%893773032.body
}

m4return_t ClEngineForTest::End ()
{
  //## begin ClEngineForTest::End%893773033.body preserve=yes
	return M4_SUCCESS;
  //## end ClEngineForTest::End%893773033.body
}

// Additional Declarations
  //## begin ClEngineForTest%3458584D0314.declarations preserve=yes
  //## end ClEngineForTest%3458584D0314.declarations

//## begin module%3438FB3303B6.epilog preserve=yes
//## end module%3438FB3303B6.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClEngineLogForTest::ClEngineLogForTest%884601779.initialization preserve=no
  : ClEngineLog(ai_pLog)
//## end ClEngineLogForTest::ClEngineLogForTest%884601779.initialization

#endif
