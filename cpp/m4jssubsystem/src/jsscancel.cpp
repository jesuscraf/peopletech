//## begin module%37D7C6390186.cm preserve=no
//## end module%37D7C6390186.cm

//## begin module%37D7C6390186.cp preserve=no
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
//## end module%37D7C6390186.cp

//## Module: JSSCancel%37D7C6390186; Package body
//## Subsystem: M4JSSubsystem::src%37BD2F0A02AE
//## Source file: D:\FuentesServidor\m4jssubsystem\version\src\jsscancel.cpp

//## begin module%37D7C6390186.additionalIncludes preserve=no
//## end module%37D7C6390186.additionalIncludes

//## begin module%37D7C6390186.includes preserve=yes
#include <basiclog.hpp>
//## end module%37D7C6390186.includes

// JSSCancel
#include <jsscancel.hpp>
// clexecutivebasess
#include <clexecutivebasess.hpp>
// StatsJSEXECUTE
#include <clstatsjsexecute.hpp>
// CcExecutor
#include <ccexecutor.hpp>
// LauncherForCancelExecutor28
#include <launcherccexe28.hpp>
// ExeForJobSchecEXECUTE
#include <exeforjobschecexecute.hpp>
//## begin module%37D7C6390186.declarations preserve=no
//## end module%37D7C6390186.declarations

//## begin module%37D7C6390186.additionalDeclarations preserve=yes
//## end module%37D7C6390186.additionalDeclarations


// Class ClJSSCancel 




ClJSSCancel::ClJSSCancel (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClJSSCancel::ClJSSCancel%936888044.hasinit preserve=no
  //## end ClJSSCancel::ClJSSCancel%936888044.hasinit
  //## begin ClJSSCancel::ClJSSCancel%936888044.initialization preserve=yes
  :ClExecutiveBaseSS(ai_poConfiguration,ai_poTables,ai_poNavigator , NULL )
  //## end ClJSSCancel::ClJSSCancel%936888044.initialization
{
  //## begin ClJSSCancel::ClJSSCancel%936888044.body preserve=yes
	m_pcDumpFile = NULL;
	m_oServiceName = "";
	m_oServiceName = "";
  //## end ClJSSCancel::ClJSSCancel%936888044.body
}



//## Other Operations (implementation)
m4return_t ClJSSCancel::Initialize ()
{
  //## begin ClJSSCancel::Initialize%936888045.body preserve=yes
	m4double_t uiInstances = 0;
	M4ClString strError;

	ClServiceQ *poServiceQ;
	ClOutputQ *poOutputQ;
	poServiceQ = (ClServiceQ*) m_poServiceQ;
	poOutputQ = (ClOutputQ*) m_poOutputQ;

	ClLauncherInterface *poLauncher;
	ClEngine	*pEngine = NULL;
	ClExecutive	*pExecutive;

//	pEngine = new ClEngineForJobSchecEXECUTE (poStats);
//	if (pEngine == NULL)
//		return M4_ERROR;
	pExecutive = new ClCcExecutor (pEngine,m_poStatistics);
	if (pExecutive == NULL)
		return M4_ERROR;
	poLauncher = new ClLauncherForCancelExecutor28(pExecutive , m_poStatistics, NULL, poServiceQ, poOutputQ);
	if (poLauncher == NULL)
		return M4_ERROR;

//	((ClCcExecutor*)pExecutive) -> SetDumpFile(m_pcDumpFile);

	((ClCcExecutor*)pExecutive) -> SetIdentification(m_oServerName, m_oServiceName);

	poLauncher -> SetUseException ( GetUseExceptions() ) ;

	m_poLauncher = poLauncher;

	return M4_SUCCESS;
  //## end ClJSSCancel::Initialize%936888045.body
}

m4return_t ClJSSCancel::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClJSSCancel::ExecuteCommand%936888046.body preserve=yes
	return ClExecutiveBaseSS::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
  //## end ClJSSCancel::ExecuteCommand%936888046.body
}

m4return_t ClJSSCancel::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClJSSCancel::GetCommandParams%936888047.body preserve=yes
	return ClExecutiveBaseSS::GetCommandParams (ai_cCommand, aio_oParamList);
  //## end ClJSSCancel::GetCommandParams%936888047.body
}

m4return_t ClJSSCancel::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClJSSCancel::GetCommandsId%936888048.body preserve=yes
	return ClExecutiveBaseSS::GetCommandsId (aio_oCommandList);
  //## end ClJSSCancel::GetCommandsId%936888048.body
}

m4return_t ClJSSCancel::Start ()
{
  //## begin ClJSSCancel::Start%936888049.body preserve=yes
	/*if (ClExecutiveBaseSS::Start() == M4_ERROR)
		return M4_ERROR;*/
	if (m_poLauncher -> Initialize(m_poNavigator, GetConfiguration() )==M4_ERROR)
		return M4_ERROR;
	m_poLauncher -> Start();
	m4Trace(printf("Executor for EXECUTOR_JS_CANCEL starting\n"));

	return M4_SUCCESS;
  //## end ClJSSCancel::Start%936888049.body
}

m4return_t ClJSSCancel::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClJSSCancel::Stop%937812721.body preserve=yes
	return M4_SUCCESS ;
  //## end ClJSSCancel::Stop%937812721.body
}

m4return_t ClJSSCancel::SetDumpFile (m4pchar_t ai_pcDumpFile)
{
  //## begin ClJSSCancel::SetDumpFile%940237695.body preserve=yes
	m_pcDumpFile = ai_pcDumpFile;
	return M4_SUCCESS;
  //## end ClJSSCancel::SetDumpFile%940237695.body
}

m4return_t ClJSSCancel::SetIdentification (M4ClString& ai_ServerName, M4ClString& ai_ServiceName)
{
  //## begin ClJSSCancel::SetIdentification%940337547.body preserve=yes
	m_oServerName = ai_ServerName;
	m_oServiceName = ai_ServiceName;
	return M4_SUCCESS;
  //## end ClJSSCancel::SetIdentification%940337547.body
}

// Additional Declarations
  //## begin ClJSSCancel%37D7C4D2011B.declarations preserve=yes
  //## end ClJSSCancel%37D7C4D2011B.declarations

//## begin module%37D7C6390186.epilog preserve=yes
//## end module%37D7C6390186.epilog
