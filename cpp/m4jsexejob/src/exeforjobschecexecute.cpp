//## begin module%34E2EB83027E.cm preserve=no
//## end module%34E2EB83027E.cm

//## begin module%34E2EB83027E.cp preserve=no
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
//## end module%34E2EB83027E.cp

//## Module: ExeForJobSchecEXECUTE%34E2EB83027E; Package body
//## Subsystem: M4JSExeJob::src%37F8BA6602AA
//## Source file: e:\m4server\m4jsexejob\src\exeforjobschecexecute.cpp

//## begin module%34E2EB83027E.additionalIncludes preserve=no
//## end module%34E2EB83027E.additionalIncludes

//## begin module%34E2EB83027E.includes preserve=yes
#include <m4jsres.hpp>
#include <m4string.hpp>
#include <m4jsdefinechannel.hpp>

#include <execdumpinfo.hpp>

#include "access.hpp"
#include "channel.hpp"
#include "cljschannelhandler.hpp"
#include "clsecurity.hpp"

//clexjsexcommands.hpp
#include <clexjsexcommands.hpp>
//executor
#include <executor.hpp>
//metadatasubsys
#include <metadatasubsys.hpp>
//navigatorf
#include <navigatorf.hpp>
//ClSecurity30
#include <clsecurity30.hpp>
//StatsJSExecute
#include <clstatsjsexecute.hpp>
//syncro
#include <syncro.hpp>
//jsjob
#include <jsjob.hpp>
//#include <clssinterfaz.hpp>
//logoninterface
#include <logoninterface.hpp>
//cljschannelhandler
#include <cljschannelhandler30.hpp>
//clparametershandler
#include <clparametershandler.hpp>

//## end module%34E2EB83027E.includes

// Executive
#include <executive.hpp>
// ExeForJobSchecEXECUTE
#include <exeforjobschecexecute.hpp>
#include "m4unicode.hpp"


//## begin module%34E2EB83027E.declarations preserve=no
//## end module%34E2EB83027E.declarations

//## begin module%34E2EB83027E.additionalDeclarations preserve=yes
//## end module%34E2EB83027E.additionalDeclarations


// Class ClExeForJobSchecEXECUTE 













ClExeForJobSchecEXECUTE::ClExeForJobSchecEXECUTE (ClEngine *ai_pEngine, ClStatistic *ai_poStats)
  //## begin ClExeForJobSchecEXECUTE::ClExeForJobSchecEXECUTE%887287313.hasinit preserve=no
  //## end ClExeForJobSchecEXECUTE::ClExeForJobSchecEXECUTE%887287313.hasinit
  //## begin ClExeForJobSchecEXECUTE::ClExeForJobSchecEXECUTE%887287313.initialization preserve=yes
  :ClExecutive(ai_pEngine,ai_poStats)
  //## end ClExeForJobSchecEXECUTE::ClExeForJobSchecEXECUTE%887287313.initialization
{
  //## begin ClExeForJobSchecEXECUTE::ClExeForJobSchecEXECUTE%887287313.body preserve=yes
	m_pcDumpFile = NULL;
	m_poChannelHandler=NULL;
	m_poSecurity=NULL;
	m_poParameterHandler=NULL;
	m_poJSExeCommand=NULL;
	m_poChannelGJDef=NULL;
	m_poChannelExec=NULL;
	m_poAccessGJDef=NULL;
	m_poAccessExec=NULL;
	m_poStatis=ai_poStats;
	m_poDumpInfo = new ExecDumpInfo;
	m_poDumpInfo->SetCurrentStage("Initial State",0);
	m_pcDumpFile = NULL;

  //## end ClExeForJobSchecEXECUTE::ClExeForJobSchecEXECUTE%887287313.body
}


ClExeForJobSchecEXECUTE::~ClExeForJobSchecEXECUTE ()
{
  //## begin ClExeForJobSchecEXECUTE::~ClExeForJobSchecEXECUTE%887389853.body preserve=yes
	
	if (m_poDumpInfo != NULL)
	{
		delete m_poDumpInfo;
		m_poDumpInfo = NULL;
	}


  //## end ClExeForJobSchecEXECUTE::~ClExeForJobSchecEXECUTE%887389853.body
}



//## Other Operations (implementation)
m4return_t ClExeForJobSchecEXECUTE::StartUp (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig)
{
  //## begin ClExeForJobSchecEXECUTE::StartUp%887287315.body preserve=yes

	ClLogonInterface	*poLogon;
	ClCache				*poCMCRCache = NULL ;
	ClCache				*poCSCRCache = NULL;
	ClCache				*poPresentationCache = NULL;
	ClCache				*poMapCache = NULL;
	ClCache				*poDataCache = NULL;
	size_t				iLenValue = 0;
	m4char_t			pszValue[512];
	m4return_t			iRet = M4_SUCCESS;

	//Estableciento caches para agilizar la construcción de los canales
	ClSSInterfaz * pMetadataSubsystemInterfaz = (ClSSInterfaz*)ai_pNavigator->GetSubsystemByName(ClMetaDataSubsystem::M4MetaDataSubsystemName);
	if (NULL != pMetadataSubsystemInterfaz)
	{
		poCMCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache);
		poCSCRCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache);
		poPresentationCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache);
		poMapCache = (ClCache*)pMetadataSubsystemInterfaz->GetElement(ClExecutiveForMDCaches::M4ExecutiveForMDMapCache);
		
		// DATACACHE
		iRet = ai_pConfig->GetStringValue("", ClExecutiveForMDCaches::M4ExecutiveForMDDataCache, pszValue, sizeof(pszValue), iLenValue);
		if (iRet == M4_SUCCESS)
		{
			if ( ! ( poDataCache = ( ClCache * )ai_pNavigator-> GetSubsystemElement ( pszValue , "CACHE") ) )
			{
				SETCODEF ( M4_SRV_SS_CACHE_ERROR , ERRORLOG , "MetaDataSubsystem. Cannot find cache <%s>, in caches subsystem" , pszValue ) ;
			}
		}
	}
	else
	{
		SETCODEF(M4_ERROR_METADATA_JS, ERRORLOG, "#*s1*#");
	}

	//Creamos el puntero al manejador de canales
	//que se encarga de todas las operaciones relacionadas
	//con canales
	m_poChannelHandler=new ClJSChannelHandler30();

	//Inicializamos el entorno de lo meta4object
	ClM4ObjService *poObjService;
	m4return_t ret=m_poChannelHandler->BuildEnvToChannels(poObjService,poCMCRCache, poCSCRCache, poPresentationCache, poMapCache, poDataCache);
	
	if (M4_ERROR == ret)
	{
		ShutDown();
		return M4_ERROR;
	}
	
	//Con este método obtenemos el logon interface
	ret=GetLogonInterface(ai_pNavigator,poLogon);

	if (M4_ERROR==ret)
	{
		ShutDown();
		return M4_ERROR;
	}

	//Creamos el puntero a la clase que maneja la seguridad
	m_poSecurity=new ClSecurity30(poObjService,poLogon,(ClStatsJSEXECUTE *)m_poStatis);

	if (m_poSecurity==NULL)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"#*s1*#poSecurity#");

		ShutDown();
		return M4_ERROR;
	}

	//Creamos los meta4object asociados al ejecutor
	ret = BuildMeta4Objects("C4_JS_SCHED_TASKS", "C4_JS_TASK_EXE");
	if (M4_SUCCESS != ret)
	{
		ShutDown();
		return M4_ERROR;
	}

	//Creamos el objeto comando que es el encargado de realizar
	//las acciones necesarias para ejecutar una tarea
	m_poJSExeCommand = new ClJSExeCommand(m_poSecurity, m_poChannelGJDef, m_poChannelExec, m_poAccessGJDef, m_poAccessExec, m_poChannelHandler);
	if (NULL==m_poJSExeCommand)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"#*s1*#m_poJSExeCommand#");

		ShutDown();
		return M4_ERROR;
	}

	//Inicializamos el manejador de parámetros
	m_poParameterHandler=new ClJSParametersHandlers(m_poAccessExec);
	if (NULL == m_poParameterHandler)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG, "#*S1*#m_poParametersHadlers#");

		ShutDown();
		return M4_ERROR;
	}

	//Pasamos el valor al comando
	m_poJSExeCommand->SetParameterHandler(m_poParameterHandler);

	//Como cuando creamos los canales cogemos una conexión lógica
	//en propiedad debemos liberarla hasta que ejecutemos una tarea
	//para que pueda ser aprovechada por otro servicio
	m_poChannelHandler->SetConnection();

	return M4_SUCCESS;
	
  //## end ClExeForJobSchecEXECUTE::StartUp%887287315.body
}

m4return_t ClExeForJobSchecEXECUTE::Reset ()
{
  //## begin ClExeForJobSchecEXECUTE::Reset%887287317.body preserve=yes

	return M4_SUCCESS;

  //## end ClExeForJobSchecEXECUTE::Reset%887287317.body
}

m4return_t  ClExeForJobSchecEXECUTE::ShutDown ()
{
  //## begin ClExeForJobSchecEXECUTE::ShutDown%887287324.body preserve=yes

	//HAcemos un delete de los punteros a los objeto
	//seguridad,manipulador de canales , manipulador de parámetros
	//y al comando

	if (NULL != m_poParameterHandler)
	{
		delete m_poParameterHandler;

		m_poParameterHandler= NULL;
	}

	if (NULL !=m_poJSExeCommand)
	{
		delete m_poJSExeCommand;

		m_poJSExeCommand=NULL;
	}

	if (NULL != m_poSecurity)
	{
		delete m_poSecurity;

		m_poSecurity=NULL;
	}


	if (m_poDumpInfo != NULL)
	{
		delete m_poDumpInfo;
		m_poDumpInfo = NULL;
	}

	//Destruimos los canales asociados al ejecutor
	m4return_t ret1,ret2;

	ret1=DestroyMeta4Objects();

	if (m_poChannelHandler != NULL)
	{

		ret2=m_poChannelHandler->DestroyEnvToChannels();

	}

	
	if (NULL != m_poChannelHandler)
	{
		delete m_poChannelHandler;

		m_poChannelHandler=NULL;
	}

	if(ret1 == M4_SUCCESS || ret2 == M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	return M4_SUCCESS;

  //## end ClExeForJobSchecEXECUTE::ShutDown%887287324.body
}

m4return_t  ClExeForJobSchecEXECUTE::GetLogonInterface (ClNavigatorFirewall *ai_pNavigator, ClLogonInterface*  &ai_poLogon)
{
  //## begin ClExeForJobSchecEXECUTE::GetLogonInterface%900415011.body preserve=yes

	if (ai_pNavigator==NULL)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"#*s1*#ai_poNavigator#");

		return M4_ERROR;
	}

	ai_poLogon=(ClLogonInterface *)ai_pNavigator->GetSubsystemElement("USERS","LOGON_INTERFACE");

	if (ai_poLogon == NULL)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG,"#*S1*#poLogon#");

		STORELOGS();

		return M4_ERROR;
	}


	return M4_SUCCESS;

  //## end ClExeForJobSchecEXECUTE::GetLogonInterface%900415011.body
}

m4return_t ClExeForJobSchecEXECUTE::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExeForJobSchecEXECUTE::ExecuteCommand%936777418.body preserve=yes

		m4return_t					ret=M4_ERROR;
		m4uint32_t					iSize; //Para el comando de obtener memoria usada
		ClAdminTagList::iterator	IteratorParamCommand;
		m4uint32_t					uiTimeOut = 0;

		switch (ai_cCommand)
		{
			case CANCEL:
				//Este comando es invocado cuando el 
				//usario o el administrador quiere interrumpir
				//la tarea
				ret=m_poJSExeCommand->Cancel();
				break;
			case SETIMEOUT:
				//Este comando modifica el tiempo máximo
				//que se estará ejecutando una tarea
				
				//Recuperamos el tiempo de TIME OUT 
				//en los parámetros del comando
				IteratorParamCommand = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));

				if (IteratorParamCommand == ai_oParamList.end())
					ret=M4_ERROR;
				else
				{
					// Obtenemos el valor del parámetro
					//que es una cadena y lo convertimos a un entero
					uiTimeOut= atoi((*IteratorParamCommand)->m_tagValue);
					
					//Comprobamos que el valor que nos dan es distinto 
					//de cero si fuera igual a 0  consideramos el valor 
					//que se tiene por defecto que es infinito
					if (uiTimeOut != 0)
						ret=m_poJSExeCommand->SetTimeOut(uiTimeOut);
				}
				break;
			case DELETE_TIMEOUT:
				  //Cuando no queramos que la ejecución tenga
				  //un tiempo máximo permitido invocamos a este
				  //comando
				 ret=m_poJSExeCommand->SetTimeOut();
				 break;
			case ClExecutive::USED_MEMORY:
				iSize = m_poJSExeCommand->GetUsedMemory();
				if (iSize != -1) {
					// Preparo el parametro de salida
					m4char_t szSize[256];
					sprintf(szSize, "%0.2f", iSize/1024.0);
					
					ClAdminCommandTag* pAdminCommandTag =
						ClAdminCommandTag::GetNewAdminCommandTag (0, "USED_MEMORY", adminLong, szSize, M4_TRUE);
					if (pAdminCommandTag != NULL) {
						ao_oResultList.push_back(pAdminCommandTag);
						ret = M4_SUCCESS;
					}
				}
				break;
			default:
				ret=ClExecutive::ExecuteCommand(ai_cCommand,ai_oParamList,ao_oResultList);
			break;
		}

		return ret;
  //## end ClExeForJobSchecEXECUTE::ExecuteCommand%936777418.body
}

m4return_t ClExeForJobSchecEXECUTE::Execute (ClJSJob *ai_poJSJob)
{
  //## begin ClExeForJobSchecEXECUTE::Execute%939286200.body preserve=yes

	m4return_t  ret=M4_SUCCESS;

	//Primero comprobamos si el puntero que nos pasan es correcto
	if (NULL != ai_poJSJob)
	{
		//Después comprobamos si nos pasan el box 
		//Esto tiene un razón con vista al futuro
		//Cuando los ejecutores esté dirigidos por la 
		//clase ClBox con lo que cuando queramos inicializar
		//el box nos tienen que enviar al propio box
		// y cuando ejecutemos el job nos envían el job
		if (M4_FALSE==ai_poJSJob->IsRoot())
		{

			//Desregistramos las estadísticas que tuvieramos de 
			//la ejecución anterior tal como la fecha de inicio
			//la fecha de fin y la duración de la ejecución
			((ClStatsJSEXECUTE *)m_poStatis)->DesactiveLastExecution();

			//Registramos la fecha actual para almacenarla
			//en las estadísticas como la fecha de inicio de la ejecución
			M4ClTimeStamp DateNow;
			DateNow.now();

			//Convertimos la fecha a juliano
			m4double_t dDateNow;
			DateNow.get(dDateNow);

			//Almacenmos el valor en las estadísticas
			((ClStatsJSEXECUTE *)m_poStatis)->SetDateStartExecution(dDateNow);

			if (m_poDumpInfo != NULL)
			{
				m_poDumpInfo->SetCurrentStage("JS Executor 30: Proccess Preparing to execute");
			}

			//Le pasamos el trabajo al manejador de canales
			m_poParameterHandler->SetBox(ai_poJSJob);
					
			//Obtenemos el identificador de la definición del grupo de tareas
			m4uint32_t IdBox;
			ai_poJSJob->GetSchedJobId(IdBox);

			//Obtenemos el identificador de la definición de la tarea
			m4uint32_t IdJob;
			ai_poJSJob->GetId(IdJob);

			// Obtenemos el identificador de la ejecución.
			m4uint32_t IdBoxExe;
			ai_poJSJob->GetExecutionId(IdBoxExe);

			//Invocamos el método de la clase ClJSExeCommands
			//que sabe lo que tenemos que hacer en cada paso de
			//la ejecución
			m4return_t ret = m_poJSExeCommand->Execute(IdBox, IdJob, IdBoxExe, (ClStatsJSEXECUTE *) m_poStatis);

			//Si la ejecución ha sido interrumpida por cualquier causa
			//no seguimos ejecutando la siguiente tarea del box
			if ((M4_ABORT_TASK == ret) ||(M4_ABORT_AND_ERROR == ret))
			{
				//Esto establece un flag que es interpretado por el 
				//administrador para no seguir lanzando tareas del trabajo
				ai_poJSJob->SetInterruptionRequestLevel(ClJSJob::IRL_INSS);
			
				//Si la ejecución ha finalizado con interrupción
				//pero no se ha producido ningún error recuperando datos
				//devolvemos success.
				if (M4_ABORT_TASK == ret)
				{
					ret=M4_SUCCESS;
				}
				else
				{
					ret=M4_ERROR;
				}
			}
				
			m_poJSExeCommand->SetDumpInfo(NULL);
			
			if (m_poDumpInfo != NULL)
			{
				m_poDumpInfo->SetCurrentStage("JS Executor 30: Proccess Post Execution");
			}

			//Registramos la fecha en la que finaliza la ejecución 
			DateNow.now();
			DateNow.get(dDateNow);
			
			//Desregistramos las estadísticas asociadas a los ejecutores 
			//activos tal como el identificador de la tarea , del trabajo 
			//y el usuario bajo la cual se ejecuta
			((ClStatsJSEXECUTE *)m_poStatis)->DesactiveExecution();

			//Almacenamos el valor en la estadística correspondiente
			((ClStatsJSEXECUTE *)m_poStatis)->SetDateEndExecution(dDateNow);

			//Ahora podemos calcular cuanto ha tardado la ejecución
			((ClStatsJSEXECUTE *)m_poStatis)->SetLastTimeJob();

			//También incrementamos el tiempo total empleado
			//por el ejecutor en ejecutar tareas
			((ClStatsJSEXECUTE *)m_poStatis)->SetTimeInExecution();
		}

		//Este metodo es necesario invocarlo para que el puntero
		//interno de la clase ClBox apunte a la siguiente tarea del
		//trabajo. Si no hay más tareas el administrador lo detectará
		ai_poJSJob->ExecuteOn();

		return ret ;
	}

	return M4_ERROR;
	
  //## end ClExeForJobSchecEXECUTE::Execute%939286200.body
}

m4return_t ClExeForJobSchecEXECUTE::BuildMeta4Objects (M4ClString ai_oNameFirstChannel, M4ClString ai_oNameSecondChannel)
{
  //## begin ClExeForJobSchecEXECUTE::BuildMeta4Objects%940836428.body preserve=yes
	
	//Invocamos al manipulador de canales para que construya 
	//los canales cuyos indentificadores nos pasan por parámetro
	m4return_t ret=m_poChannelHandler->BuildChannel(ai_oNameFirstChannel,m_poChannelGJDef,m_poAccessGJDef);
	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}

	ret = m_poChannelHandler->BuildChannel(ai_oNameSecondChannel, m_poChannelExec, m_poAccessExec);
	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClExeForJobSchecEXECUTE::BuildMeta4Objects%940836428.body
}

m4return_t ClExeForJobSchecEXECUTE::DestroyMeta4Objects ()
{
  //## begin ClExeForJobSchecEXECUTE::DestroyMeta4Objects%940836429.body preserve=yes

	//Invocamos al manipulador de canales para que destruya
	//los canales asociados al ejecutor
	
	m4return_t ret1, ret2;
	m4bool_t IsAnyProblem=M4_FALSE;

	if (m_poAccessGJDef != NULL && m_poChannelGJDef!= NULL)
	{
		ret1=m_poChannelHandler->DestroyChannel(m_poAccessGJDef,m_poChannelGJDef);
	}

	if (m_poAccessExec != NULL && m_poChannelExec != NULL)
	{
		ret2 = m_poChannelHandler->DestroyChannel(m_poAccessExec,m_poChannelExec);
	}

	if (ret1 == M4_ERROR || ret2 == M4_ERROR || IsAnyProblem == M4_TRUE)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClExeForJobSchecEXECUTE::DestroyMeta4Objects%940836429.body
}

void ClExeForJobSchecEXECUTE::SetDumpFile (m4pchar_t ai_pcDumpFile)
{
  //## begin ClExeForJobSchecEXECUTE::SetDumpFile%940836430.body preserve=yes

	m_pcDumpFile = ai_pcDumpFile;

  //## end ClExeForJobSchecEXECUTE::SetDumpFile%940836430.body
}

m4return_t ClExeForJobSchecEXECUTE::DumpLevel (const m4int16_t ai_iLevel)
{
  //## begin ClExeForJobSchecEXECUTE::DumpLevel%940836432.body preserve=yes
	if (ai_iLevel < 0 || ai_iLevel >= JS_EXEC_DUMP_LEVEL)
		return M4_ERROR;

	if (m_pcDumpFile == NULL)
		return M4_ERROR;

	// prepares to append to file
	ofstream Output;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( Output, m_pcDumpFile, M4UniAppend, eEncoding);

	if (!Output)
	{
		return M4_ERROR;
	}

	// writes generic information
	Output << endl;
	Output << "==========================================" << endl;
	Output << "Job Scheduler executor dump info file for executor " << (void*) this << endl;
	Output << "==========================================" << endl;

	// writes the dump object
	//m_poDumpInfo->iDumpLevel = ai_iLevel;
	Output << m_poDumpInfo;
	
	return M4_SUCCESS;

  //## end ClExeForJobSchecEXECUTE::DumpLevel%940836432.body
}

m4return_t ClExeForJobSchecEXECUTE::ReleaseResources ()
{
  //## begin ClExeForJobSchecEXECUTE::ReleaseResources%942231905.body preserve=yes

	//Cuando finalizamos con un excepción que se produce en el Launcher
	//invocamos este método para limpiar las conexiones lógicas y en general
	//el contexto de ejecución.
	if (m_poSecurity != NULL )
	{

		return m_poSecurity->ReleaseResources();
	}

	return M4_ERROR;

  //## end ClExeForJobSchecEXECUTE::ReleaseResources%942231905.body
}

m4return_t ClExeForJobSchecEXECUTE::SetIdentification (const M4ClString &ai_stServerName, const M4ClString &ai_stServiceName)
{
  //## begin ClExeForJobSchecEXECUTE::SetIdentification%942839183.body preserve=yes

	return (m_poJSExeCommand->SetIdentification(ai_stServerName,ai_stServiceName));

  //## end ClExeForJobSchecEXECUTE::SetIdentification%942839183.body
}

m4return_t ClExeForJobSchecEXECUTE::Execute (ClImage &aio_pImage)
{
  //## begin ClExeForJobSchecEXECUTE::Execute%945176654.body preserve=yes
	return M4_ERROR;
  //## end ClExeForJobSchecEXECUTE::Execute%945176654.body
}

// Additional Declarations
  //## begin ClExeForJobSchecEXECUTE%34E2D14402D6.declarations preserve=yes
  //## end ClExeForJobSchecEXECUTE%34E2D14402D6.declarations

//## begin module%34E2EB83027E.epilog preserve=yes
//## end module%34E2EB83027E.epilog
