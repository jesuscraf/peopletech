//## begin module%37D379C60104.cm preserve=no
//## end module%37D379C60104.cm

//## begin module%37D379C60104.cp preserve=no
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
//## end module%37D379C60104.cp

//## Module: JSSExecute%37D379C60104; Package body
//## Subsystem: M4JSSubsystem::src%37BD2F0A02AE
//## Source file: D:\FuentesServidor\m4jssubsystem\src\jssexecute.cpp

//## begin module%37D379C60104.additionalIncludes preserve=no
//## end module%37D379C60104.additionalIncludes

//## begin module%37D379C60104.includes preserve=yes
#include <exeforjobschecexecute.hpp> //body only
#include <cljslauncherexecutor.hpp> //body only
#include <basiclog.hpp>
#include <cloblconfiguration.hpp>
#include <timeinterval.hpp>
//## end module%37D379C60104.includes

// Executive
#include <executive.hpp>
// JSSExecute
#include <jssexecute.hpp>
// clexecutivebasess
#include <clexecutivebasess.hpp>
// StatsJSEXECUTE
#include <clstatsjsexecute.hpp>
//## begin module%37D379C60104.declarations preserve=no
//## end module%37D379C60104.declarations

//## begin module%37D379C60104.additionalDeclarations preserve=yes
#define MAX_SIZE_STRING  256

//Valores de la obl
#define M4JS_OBL_TIMEOUT					"JS_TIMEOUT"

//## end module%37D379C60104.additionalDeclarations


// Class ClJSSExecute 





ClJSSExecute::ClJSSExecute (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClJSSExecute::ClJSSExecute%936606367.hasinit preserve=no
  //## end ClJSSExecute::ClJSSExecute%936606367.hasinit
  //## begin ClJSSExecute::ClJSSExecute%936606367.initialization preserve=yes
  :ClExecutiveBaseSS(ai_poConfiguration,ai_poTables,ai_poNavigator , NULL)
  //## end ClJSSExecute::ClJSSExecute%936606367.initialization
{
  //## begin ClJSSExecute::ClJSSExecute%936606367.body preserve=yes
	m_pcDumpFile = NULL;
	m_poExecutive = NULL;
	m_oServiceName = "";
	m_oServiceName = "";
  //## end ClJSSExecute::ClJSSExecute%936606367.body
}



//## Other Operations (implementation)
m4return_t ClJSSExecute::Initialize ()
{
  //## begin ClJSSExecute::Initialize%936606368.body preserve=yes
	
	ClLauncherStats *poStats = NULL;
	ClLauncherInterface *poLauncher = NULL;
	ClServiceQ *poServiceQ = NULL;
	ClOutputQ *poOutputQ = NULL;
	ClEngine	*pEngine=NULL;
	ClExecutive	*pExecutive = NULL;

	poStats=new ClStatsJSEXECUTE();
	if (poStats == NULL)
		return M4_ERROR;
	poServiceQ = (ClServiceQ*) m_poServiceQ;
	poOutputQ = (ClOutputQ*) m_poOutputQ;

	//Ya NO Usamos ejecutores 2.8
	pExecutive = new ClExeForJobSchecEXECUTE(pEngine,poStats);

	if (pExecutive == NULL)
		return M4_ERROR;
	poLauncher = new ClJSLauncherExecutor(pExecutive , poStats, NULL, poServiceQ, poOutputQ);
	if (poLauncher == NULL)
		return M4_ERROR;

	poLauncher -> SetUseException ( GetUseExceptions() ) ;

	m_poLauncher = poLauncher;

	m_poStatistics = poStats;

	m_poExecutive = pExecutive;

//	((ClExeForJobSchecEXECUTE*)m_poExecutive) -> SetIdentification(m_oServerName, m_oServiceName);
	((ClExeForJobSchecEXECUTE*)m_poExecutive) -> SetDumpFile(m_pcDumpFile);

	if (m_poLauncher -> Initialize(m_poNavigator, GetConfiguration() )==M4_ERROR)
		return M4_ERROR;

	//Time-out inicial
	ClAdminCommandTag *newParam;
	ClAdminTagList listParamIn, listParamOut;
	m4char_t bufferTemp[MAX_SIZE_STRING];
	size_t iLenValue;
	m4char_t szValue [ OBL_PROPVALUE_MAX_LEN ] ;
	m4uint32_t msTimeOut;
	M4ClString FormatTime;
	ClTimeInterval interval(0);

	GetConfigStringValue ( M4JS_OBL_TIMEOUT,   szValue , OBL_PROPVALUE_MAX_LEN ,iLenValue ) ;
	FormatTime = szValue;
	if (stricmp(FormatTime.c_str(), "Disabled") != 0) {
		//Tenemos time-out
		if (interval.Set(FormatTime) == M4_SUCCESS) {
			interval.Get(msTimeOut);
			//Lo obtenemso en segundos y lo pasamos a milisegundos
			msTimeOut = msTimeOut*1000;
			sprintf(bufferTemp, "%d", msTimeOut);
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_TIMEOUT", adminLong, bufferTemp);
			listParamIn.push_back(newParam);
			ExecuteCommand(ClJSSExecute::JS_CHANGE_TIMEOUT,listParamIn,listParamOut);
		}
	}

	return M4_SUCCESS;
  //## end ClJSSExecute::Initialize%936606368.body
}

m4return_t ClJSSExecute::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClJSSExecute::ExecuteCommand%936606369.body preserve=yes
	m4return_t iRet = M4_ERROR;

	M4ClString sUser;
	ClAdminCommandTag* pAdminCommandTag;

	ClSSImplementation::eStatus	Status;
	GetState(Status);

	if ( (SS_NOT_ACCESIBLE != Status) && (SS_INACTIVE != Status)) {
		switch (ai_cCommand) {
			case ClJSSExecute::JS_CANCEL_JOB:
				iRet = m_poExecutive -> ExecuteCommand (ClExeForJobSchecEXECUTE::CANCEL, ai_oParamList, ao_oResultList);
				break;
			case ClJSSExecute::JS_CHANGE_TIMEOUT:
				iRet = m_poExecutive -> ExecuteCommand (ClExeForJobSchecEXECUTE::SETIMEOUT, ai_oParamList, ao_oResultList);
				break;
			case ClJSSExecute::JS_DELETE_TIMEOUT:
				iRet = m_poExecutive -> ExecuteCommand (ClExeForJobSchecEXECUTE::DELETE_TIMEOUT, ai_oParamList, ao_oResultList);
				break;
			case ClExecutiveBaseSS::GET_USED_MEMORY:
				if (((ClStatsJSEXECUTE*)m_poStatistics)->GetUser(sUser) == M4_SUCCESS) {
					//Existe un usuario, luego se esta realizando una ejecucion				
					iRet = m_poExecutive -> ExecuteCommand (ClExecutive::USED_MEMORY, ai_oParamList, ao_oResultList);
				}
				else {
					pAdminCommandTag = ClAdminCommandTag::GetNewAdminCommandTag (0, "USED_MEMORY", adminLong, "0", M4_TRUE);
					if (pAdminCommandTag != NULL) {
						ao_oResultList.push_back(pAdminCommandTag);
						iRet = M4_SUCCESS;
					}
				}
				break;
			default:
				iRet = ClExecutiveBaseSS::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
		};
	}
	return iRet ;
  //## end ClJSSExecute::ExecuteCommand%936606369.body
}

m4return_t ClJSSExecute::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClJSSExecute::GetCommandParams%936606370.body preserve=yes
	ClAdminCommandTag *newParam;
	switch (ai_cCommand) {	
		case GET_USED_MEMORY :
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "USED_MEMORY", adminLong, "", M4_TRUE);
			M4_ASSERT(newParam != NULL);
			if (newParam == NULL) 
				return M4_ERROR;
			aio_oParamList.push_back(newParam);
			break ;
	}
	return M4_SUCCESS;
  //## end ClJSSExecute::GetCommandParams%936606370.body
}

m4return_t ClJSSExecute::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClJSSExecute::GetCommandsId%936606371.body preserve=yes
	ClAdminTag *newCommand;
	M4ClString sUser;
	if (((ClStatsJSEXECUTE*)m_poStatistics)->GetUser(sUser) == M4_SUCCESS) {
		//Existe un usuario, luego se esta realizando una ejecucion
		newCommand = ClAdminTag::GetNewAdminTag(ClJSSExecute::JS_CANCEL_JOB, "JS_CANCEL_JOB", M4_FALSE);
		if (newCommand == NULL) 
			return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}
	newCommand = ClAdminTag::GetNewAdminTag(ClExecutiveBaseSS::GET_USED_MEMORY, "GET_USED_MEMORY", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);
	return M4_SUCCESS;
  //## end ClJSSExecute::GetCommandsId%936606371.body
}

m4return_t ClJSSExecute::Start ()
{
  //## begin ClJSSExecute::Start%936606372.body preserve=yes
	/*if (ClExecutiveBaseSS::Start() == M4_ERROR)
		return M4_ERROR;*/
	m_poLauncher -> Start("JS Executor");
	m4Trace(printf("Executor for EXECUTOR_JS starting\n"));


    // bugid: 0096170
    // LLamamos a la clase abuela para que registre el subsistema en snmp.
    // No llamamos a la clase padre porque el código que hay allí parece que le
    // molesta al jobscheduler (eso lo deducimos porque esta llamada está comentada
    // al principio de esta funcion).
    ClSSImplementation::Start();

	return M4_SUCCESS;
  //## end ClJSSExecute::Start%936606372.body
}

m4return_t ClJSSExecute::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClJSSExecute::Stop%937812723.body preserve=yes
	return ClExecutiveBaseSS::Stop (ai_lDeferedTime);
  //## end ClJSSExecute::Stop%937812723.body
}

m4return_t ClJSSExecute::SetDumpFile (m4pchar_t ai_pcDumpFile)
{
  //## begin ClJSSExecute::SetDumpFile%940237697.body preserve=yes
	m_pcDumpFile = ai_pcDumpFile;
	return M4_SUCCESS;
  //## end ClJSSExecute::SetDumpFile%940237697.body
}

m4return_t ClJSSExecute::SetIdentification (M4ClString& ai_ServerName, M4ClString& ai_ServiceName)
{
  //## begin ClJSSExecute::SetIdentification%942840134.body preserve=yes
	m_oServerName = ai_ServerName;
	m_oServiceName = ai_ServiceName;
	return M4_SUCCESS;
  //## end ClJSSExecute::SetIdentification%942840134.body
}

// Additional Declarations
  //## begin ClJSSExecute%37CE8D260166.declarations preserve=yes
  //## end ClJSSExecute%37CE8D260166.declarations

//## begin module%37D379C60104.epilog preserve=yes
//## end module%37D379C60104.epilog
