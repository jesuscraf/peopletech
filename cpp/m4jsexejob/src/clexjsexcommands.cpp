//## begin module%34FFEB0301FB.cm preserve=no
//## end module%34FFEB0301FB.cm

//## begin module%34FFEB0301FB.cp preserve=no
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
//## end module%34FFEB0301FB.cp

//## Module: ExJSExCommands%34FFEB0301FB; Package body
//## Subsystem: M4JSExeJob::src%37F8BA6602AA
//## Source file: e:\m4server\m4jsexejob\src\ClExJSExCommands.cpp

//## begin module%34FFEB0301FB.additionalIncludes preserve=no
//## end module%34FFEB0301FB.additionalIncludes

//## begin module%34FFEB0301FB.includes preserve=yes
#include "m4stl.hpp"
#include <m4jsres.hpp>
#include <m4log.hpp>
#include  <exechannel.hpp>
#include <m4jsdefinechannel.hpp>

// syncro
#include <syncro.hpp>
// ExJSExCommands
#include <clexjsexcommands.hpp>
// ExeForJobSchecEXECUTE	
#include <exeforjobschecexecute.hpp>
//ClStatsJSExecute
#include <clstatsjsexecute.hpp>
//access
#include <access.hpp>
//Channel
#include <channel.hpp>
//Security
#include <clsecurity.hpp>
//M4Date
#include <m4date.hpp>
//file_finder
#include <file_finder.hpp>
//clparametershandler
#include <clparametershandler.hpp>
//exvariables
#include <exvariables.hpp>
//cljschannelhandler
#include <cljschannelhandler.hpp>
//m4eventhandlers
#include <m4eventhandlers.hpp>
#include "sttlib.hpp"
#include "sttwrap.hpp"
#include "m4regimpl.hpp"
#include "m4thread.hpp"
#include "file_manager.hpp"
#include "m4unicode.hpp"
// ClSecurity30
#include <clsecurity30.hpp>
#include <threadobjs.hpp>
#include <rowsquota.hpp>
//## end module%34FFEB0301FB.includes


class ClChannelManager;
class ClNode;

//## begin module%34FFEB0301FB.declarations preserve=no
//## end module%34FFEB0301FB.declarations

//## begin module%34FFEB0301FB.additionalDeclarations preserve=yes

#define M4JS_LOG_SIZE                       10000

#define M4JS_LOG_ERROR						"_error_"


///////////////////////////////////////////////////
///////////ESTADOS DE EJECUCIÓN ///////////////////
///////////////////////////////////////////////////

#define M4_PREPARE_EXECUTION 0 //Preparando la ejecución del 
							   //de la tarea
#define M4_EXECUTING_JIT     1 //Ejecutando el código JIT
                               //asociado a la tarea
#define M4_END_EXECUTING_JIT 2 //Finalizada la ejecución del
                               //código JIT
#define M4_TASK_ABORTED      3 //Indica que la tarea ha sido
                               //abortado por el usuario o la
                               //herramienta de administración
#define M4_TASK_TIMEOUT      4 //Indica que la tarea ha finalizado
                               //porque se ha consumido el TIMEOUT 
							   //asignado al ejecutor

//## end module%34FFEB0301FB.additionalDeclarations


// Class ClJSExeCommand 
























ClJSExeCommand::ClJSExeCommand (ClSecurity *ai_poSecurity, ClChannel *ai_poChannelJGDef, ClChannel *ai_poChannelExec, ClAccess *ai_poAccessJGDef, ClAccess *ai_poAccessExec, ClJSChannelHandler *ai_poChannelHandler)
	: m_RowsQuotaCtrl(M4_FALSE, 0, M4_TRUE)
  //## begin ClJSExeCommand::ClJSExeCommand%889099940.hasinit preserve=no
  //## end ClJSExeCommand::ClJSExeCommand%889099940.hasinit
  //## begin ClJSExeCommand::ClJSExeCommand%889099940.initialization preserve=yes
  //## end ClJSExeCommand::ClJSExeCommand%889099940.initialization
{
  //## begin ClJSExeCommand::ClJSExeCommand%889099940.body preserve=yes

	m_poSecurity=ai_poSecurity;

	//Inicializamos el time out a infinito
	m_uiTimeOut=ClStatus::InfiniteTimeOut;

	//Inicializamos los canales y los accesos a canales

	//Obtenemos el acceso al canal de definiciones de Box
	m_poAccessGJDef=ai_poAccessJGDef;

	//Obtenemos el acceso al canal de ejecuciones
	m_poAccessExec=ai_poAccessExec;

	//Puntero al canal de definiciones de Box
	m_poChannelGJDef=ai_poChannelJGDef;

	//Puntero al canal de ejecuciones
	m_poChannelExec=ai_poChannelExec;

	//Puntero al manejador de canales
	m_poChannelHandler=ai_poChannelHandler;

	//Inicializamos a nulo los nodos de los canales
	
	m_poNodeJGDef=NULL;

	m_poNodeJDef=NULL;

	m_poNodeGExec=NULL;

	m_poNodeJExec=NULL;
	
	// Bug 0168677: Inicializamos el canal y el acceso al JIT.
	m_poChannelJIT = NULL;
	m_poAccessJIT = NULL;

	m_sEmailRecipients = "";

	//Inicializamos el mutex que protege la variable 
	//que almacena el estado interno del ejecutor
	m_poMutexStatus.Init();

	//INICIALIZAMOS EL ESTADO INTERNO DE EJECUCIÓN 
	SetInternalStatus(M4_PREPARE_EXECUTION);

	m_poDumpInfo = NULL;

  //## end ClJSExeCommand::ClJSExeCommand%889099940.body
}


ClJSExeCommand::~ClJSExeCommand ()
{
  //## begin ClJSExeCommand::~ClJSExeCommand%889099941.body preserve=yes
  //## end ClJSExeCommand::~ClJSExeCommand%889099941.body
}



//## Other Operations (implementation)
m4return_t ClJSExeCommand::Execute (m4uint32_t ai_uiIdBox, m4uint32_t ai_uiIdJob, m4uint32_t ai_uiIdBoxExe, ClStatsJSEXECUTE *aio_poStats)
{
  //## begin ClJSExeCommand::Execute%889099942.body preserve=yes
	
	m4VariantType	arg;
	m4return_t		ret;
	
	// Puntero al nodo de ficheros externos. Lo inicializamos a null 
	// por las razones que se explica en la función AddDateEnd de 
	// esta misma clase.
	SETCODEF(M4_DEBUG_INIT_EXECUTION, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);
	
	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Initializing execution");
	}

	SETCODEF(M4_DEBUG_LOAD_ALL_META4OBJECTS, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);
	
	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Loading all meta4objects");
	}

	// Llamamos al método que carga los canales que necesitamos para ejecutar 
	// la tarea.
	ret = LoadAllMeta4Objects(ai_uiIdBox, ai_uiIdJob, ai_uiIdBoxExe);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_LOAD_ALL_META4OBJECTS, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);

		// Como la tarea no puede ser lanzada porque se ha producido un error
		// tenemos que incrementar el número de tareas lanzadad con error
		// en las estadísticas.
		if (aio_poStats != NULL)
		{
			aio_poStats->AddJobsError(1);
		}

		// Vaciar y destruir canales.
		ReleaseAllMeta4Objects();
	
		return M4_ERROR;
	}

	SETCODEF(M4_DEBUG_GET_INFORMATION_SECURITY, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);

	// Obtenemos el usuario y el rol para poder establecer la 
	// seguridad.
	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Calling ChangeSecurity");
	}

	SETCODEF(M4_DEBUG_CHANGE_SECURITY, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);

	/* Bug 0263523
	Sitio unico de limpieza de errores al principio
	*/
	CLEAN();

	// Cambiamos la seguridad de la máquina virtual y obtenemos 
	// los canales de ejecución de tareas con la nueva seguridad.
	m4return_t iSecurityError = ChangeSecurity();

	// Bug 164043 El M4Object C4_JS_EXE_JIT se debe construir después de establecer la sesión
	ret = _BuildJITChannel();
	if (ret != M4_SUCCESS)
	{
		SETCODEF(M4_ERROR_CREATE_META4OBJECT_EXEC, ERRORLOG, "#*S1*#C4_JS_EXE_JIT#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);
		
		// Bug 238451.
		// Como la tarea no puede ser lanzada porque se ha producido un error.
		// tenemos que incrementar el número de tareas lanzadad con error.
		// en las estadísticas.
		if (aio_poStats != NULL)
		{
			aio_poStats->AddJobsError(1);
		}
		
		// Recuperamos el estado inicial de la máquina virtual.
		// en cuanto a seguridad.
		m_poSecurity->ReleaseConn();
		
		// Vaciar y destruir canales.
		ReleaseAllMeta4Objects();
		
		return M4_ERROR;
	}

	// Nos guardamos el valor de retorno del establecimiento de la
	// seguridad. El objetivo de esto es para poder grabar adecuadamente
	// el valor de los parámetros societarios. Si la seguridad fallase,
	// al grabar los parámetros de la tarea en base de datos, se debería
	// poner el valor "candidato" del parámetro (el que me viene en la 
	// tarea). Si la seguridad no fallase, al resolver el valor del
	// parámetro societario, se debería averiguar su valor real, 
	// desestimando el valor candidato.
	ClListParameterVariant oListParameter;
	ClListParameterVariant oListParameterOut;

	arg.Type = M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = iSecurityError;
	oListParameter.push_back(&arg);

	ret = m_poChannelHandler->ExecuteMethod("SET_SECURITY", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);
	if (ret != M4_SUCCESS)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "SET_SECURITY", "C4_JS_TASK_EXE");
					
		// Bug 238451.
		// Como la tarea no puede ser lanzada porque se ha producido un error.
		// tenemos que incrementar el número de tareas lanzadad con error.
		// en las estadísticas.
		if (aio_poStats != NULL)
		{
			aio_poStats->AddJobsError(1);
		}

		// Recuperamos el estado inicial de la máquina virtual.
		// en cuanto a seguridad.
		m_poSecurity->ReleaseConn();

		// Vaciar y destruir canales.
		ReleaseAllMeta4Objects();
		
		return M4_ERROR;
	}
		
	// Obtenemos de la lista de salida el puntero a variant
	// en el que almacenamos el valor de  retorno del método
	// LN4 que estamos invocando.
	ClListParameterVariant::iterator oIteratorParamOut;
	oIteratorParamOut = oListParameterOut.begin();
	if (oIteratorParamOut != oListParameterOut.end())
	{
		m4VariantType * poReturnArg = *oIteratorParamOut;
		if (poReturnArg->Data.DoubleData != 0)
		{
			delete poReturnArg;
			
			// Bug 238451.
			// Como la tarea no puede ser lanzada porque se ha producido un error.
			// tenemos que incrementar el número de tareas lanzadad con error.
			// en las estadísticas.
			if (aio_poStats != NULL)
			{
				aio_poStats->AddJobsError(1);
			}
			
			// Recuperamos el estado inicial de la máquina virtual.
			// en cuanto a seguridad.
			m_poSecurity->ReleaseConn();
			
			// Vaciar y destruir canales.
			ReleaseAllMeta4Objects();

			return M4_WARNING;
		}
		delete  poReturnArg;
	}	
	
	// Borramos las listas de parámetros.
	oListParameter.clear();
	oListParameterOut.clear();
	
	// Independientemente de si el establecimiento de la seguridad 
	// ha fallado o no, la inicialización de la tarea debe realizarse.
	// De esta forma, el usuario podrá ver la fecha inicio, parámetros
	// de la tarea, subtareas, etc. Por ello, la verificación de si
	// el establecimiento de la seguridad ha ido bien o no, no se hará
	// hasta que se empiece a ejecutar.
	SETCODEF(M4_DEBUG_FIND_JGEXEC, DEBUGINFOLOG, "#*s1*#%d#%d#", ai_uiIdBox, ai_uiIdBoxExe);

	// Obtener los destinatarios de las notificaciones.
	_GetEmailRecipients();

	// Modificamos el estado del Box según las circunstancias actuales.
	ret = InitializeGroupExecution(ai_uiIdBox, ai_uiIdJob, ai_uiIdBoxExe, aio_poStats);

	if (M4_SUCCESS != ret)
	{
		// Como la tarea no puede ser lanzada porque se ha producido un error
		// tenemos que incrementar el número de tareas lanzadas con error
		// en las estadísticas.
		if (aio_poStats != NULL)
		{
			((ClStatsJSEXECUTE *)aio_poStats)->AddJobsError(1);
		}
		
		// Bug 238451.
		// Recuperamos el estado inicial de la máquina virtual.
		// en cuanto a seguridad.
		m_poSecurity->ReleaseConn();

		// Vaciar y destruir canales.
		ReleaseAllMeta4Objects();
		
		return M4_ERROR;
	}

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Calling RecordJobs");
	}

	SETCODEF(M4_DEBUG_ADD_NEW_EXECUTION, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);

	// Bug 0274693. Notificar el inicio de la ejecución de la planificación.
	_SendInitExecutionMail(ai_uiIdBox, ai_uiIdBoxExe);

	// Ahora que ya hemos encontrado el job group definition
	// y el job group execution registramos el nuevo box.
	ret = RecordJobs(ai_uiIdBox, ai_uiIdBoxExe, ai_uiIdJob, aio_poStats);

	if (M4_SUCCESS != ret)
	{
		if (M4_ERROR == ret)
		{
			// Si la función devuelve un mensaje de error quiere decir
			// que no ha podido persistir el canal.
			AddDateEnd(aio_poStats,M4_FALSE);
		}
		else
		{
			// La función nos ha devuelto un error pero no está relacionado
			// con la persistencia.
			AddDateEnd(aio_poStats);
		}

		// Bug 238451.
		// Recuperamos el estado inicial de la máquina virtual.
		// en cuanto a seguridad.
		m_poSecurity->ReleaseConn();

		// Vaciamos y destruir canales.
		ReleaseAllMeta4Objects();

		return M4_ERROR;
	}

	// Ahora es el momento de comprobar si la seguridad ha 
	// fallado.
	if (M4_SUCCESS != iSecurityError)
	{
		if (M4_VIOLATION_SECURITY_JS == iSecurityError)
		{
			m4char_t strID[50];
			M4ClString strTextEvent = m_stServiceName;
			sprintf(strID, "#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);
			strTextEvent += strID;
			M4PublishEvent("65554",strTextEvent);
		}

		// Almacenamos en el campo LOG un mensaje de error por 
		// no poder cambiar la seguridad.
		SETCODEF(M4_ERROR_CHANGE_SECURITY, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);

		// Almacenamos la fecha de fin del job y el código con el que 
		// ha finalizado, en este caso error.
		AddDateEnd(aio_poStats);

		// Recuperamos los mensajes de error que se hayan generado en la
		// pila de la log y lo almacenamos en base de datos.
		RecoverLog(ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);

		// Recuperamos el estado inicial de la máquina virtual
		// en cuanto a seguridad.
		m_poSecurity->ReleaseConn();

		// Vaciamos y destruir canales.
		ReleaseAllMeta4Objects();

		return M4_ERROR;
	}

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Preparing to execute JIT");
	}

	// Ahora levantamos con la nueva seguridad el canal donde se
	// ejecuta el método just in time.

	SETCODEF(M4_DEBUG_EXECUTION, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);
	
	// Se registra la auditoría y se desregistra al final y se persiste
	ClChannelManager *poChannelManager = NULL ;

	poChannelManager = m_poChannelHandler->GetChannelManager();

	if( poChannelManager != NULL )
	{
		poChannelManager->UserActionsAudit.SwitchOn();
	}
	
	// Starts transaction rows quota control.
	m4uint32_t iRowsQuotaCurrent, iRowsQuotaPeak, iRowsQuotaLimit;
	iRowsQuotaLimit = rowsQuotaProlog();

	// Comenzamos la ejecución de la tarea.
	m4return_t retExecution = ExecuteJob(aio_poStats, ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);

	// Ends transaction rows quota control.
	rowsQuotaEpilog(iRowsQuotaCurrent, iRowsQuotaPeak);

	if( poChannelManager != NULL )
	{
		poChannelManager->UserActionsAudit.SwitchOff( M4_TRUE ) ;
	}

	// Si no logramos ejecutar la tarea porque falla alguna operación
	// canales recuperamos trazas que nos indiquen el fallo 
	// liberamos la conexión lógica ,destruimos el canal de ejecución 
	// y liberamos datos de los canales.
	if (M4_ERROR == retExecution)
	{

		AddDateEnd(aio_poStats);

		RecoverLog(ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);

		//Recuperamos el estado inicial de la máquina virtual
		//en cuanto a seguridad
		m_poSecurity->ReleaseConn();

		// Vaciar y destruir canales.
		ReleaseAllMeta4Objects();

		return M4_ERROR;
	}

	m4return_t retDateEndBox=M4_SUCCESS;

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute JIT: calling AddDateEnd");
	}

	SETCODEF(M4_DEBUG_FINISH_EXECUTION, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);

	//Llamamos al método que finaliza la tarea y el trabajo
	//si la tarea es la última o si se ha interrumpido
	retDateEndBox = AddDateEnd(aio_poStats, M4_TRUE, M4_FALSE, iRowsQuotaCurrent, iRowsQuotaPeak, iRowsQuotaLimit);

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Terminates and exit");
	}

	//Recuperamos todos los mensajes almacenados en la pila de la log
	//y los volcamos a bases de datos
	RecoverLog(ai_uiIdJob, ai_uiIdBoxExe, ai_uiIdBox);
	
	m4bool_t IsAnyError=M4_FALSE;

	
	//FINALIZACION DE LA EJECUCIÓN DE UN JOB

	//A continuación liberamos el contenido de los canales
	//destruimos el canal de ejecución del JIT
	SETCODEF(M4_DEBUG_DESTROY_META4OBJECTS, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);

	//Recuperamos el estado inicial de la máquina virtual
	//en cuanto a seguridad
	m_poSecurity->ReleaseConn();

	// Vaciar y destruir canales.
	ret = ReleaseAllMeta4Objects();
	if (M4_SUCCESS != ret)
	{		
		IsAnyError=M4_TRUE;
	}

	SETCODEF(M4_DEBUG_FINISH_SUCCESS, DEBUGINFOLOG, "#*s1*#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);

	switch (retExecution)
	{
		case M4_SUCCESS:
		case M4_ERROR:
		case M4_WARNING:
			{
				 if (IsAnyError == M4_TRUE)
				 {
					  ret=M4_ERROR;
				 }
			}
		break;
		case M4_ABORT_TASK:
			{
				if (IsAnyError == M4_TRUE)
				{
					ret=M4_ABORT_AND_ERROR;
				}
				else
				{
					ret=M4_ABORT_TASK;
				}
			}
		break;
	}

	return ret;
	
  //## end ClJSExeCommand::Execute%889099942.body
}

m4return_t ClJSExeCommand::RecordJobs (m4uint32_t ai_idJobGroupDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobDEF, ClStatsJSEXECUTE *ai_poStats)
{
  //## begin ClJSExeCommand::RecordJobs%893687424.body preserve=yes

	//En esta función sólo devolvemos error en el caso 
	//que no podamos persistir el canal , en los otros casos 
	//devolvemos un mensaje de advertencia
	m4VariantType arg;
	m4return_t ret;

	// Se introduce un nuevo registro en la tabla de subtareas o jobs 
	// para esta nueva ejecución. Para ello, se llama al método CREATE_SUBTASK_EXE.
	ClListParameterVariant oListParameter;
	ClListParameterVariant oListParameterOut;

	arg.Type = M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = ai_idJobDEF;	// Filtramos por el local_id de la tarea.
	oListParameter.push_back(&arg);

	ret = m_poChannelHandler->ExecuteMethod("CREATE_SUBTASK_EXE", oListParameter, m_poNodeJExec, m_poAccessExec, oListParameterOut);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "CREATE_SUBTASK_EXE", "C4_JS_TASK_EXE");
		return M4_WARNING;
	}
		
	// Obtenemos de la lista de salida el puntero a variant
	// en el que almacenamos el valor de  retorno del método
	// LN4 que estamos invocando.
	ClListParameterVariant::iterator oIteratorParamOut;
	oIteratorParamOut = oListParameterOut.begin();

	if (oIteratorParamOut != oListParameterOut.end())
	{
		m4VariantType * poReturnArg = *oIteratorParamOut;

		if (poReturnArg->Data.DoubleData != 0)
		{
			delete poReturnArg;
			return M4_WARNING;
		}
		delete  poReturnArg;
	}	
	
	// Borramos las listas de parámetros.
	oListParameter.clear();
	oListParameterOut.clear();

	//Rellenamos la fecha en la que se lanza 
	//la ejecución
	M4ClTimeStamp  date_submit;

	date_submit.now();
	
	m4double_t date_submit_canal;

	date_submit.get(date_submit_canal);

	arg.Type=M4CL_CPP_TYPE_DATE;
	arg.Data.DoubleData=date_submit_canal;
	
	// Para actualizar la fecha de inicio de ejecución de la subtarea, se
	// llama al método SET_START_DATE.
	oListParameter.push_back(&arg);
	ret = m_poChannelHandler->ExecuteMethod("SET_START_DATE", oListParameter, m_poNodeJExec, m_poAccessExec, oListParameterOut);
	
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#SET_START_DATE#N4_JS_SUBTASK_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		return M4_WARNING;
	}

	// Obtenemos de la lista de salida el puntero a variant
	// en el que almacenamos el valor de  retorno del método
	// LN4 que estamos invocando.
	oIteratorParamOut = oListParameterOut.begin();

	if (oIteratorParamOut != oListParameterOut.end())
	{
		m4VariantType * poReturnArg = *oIteratorParamOut;

		if (poReturnArg->Data.DoubleData != 0)
		{
			delete poReturnArg;
			return M4_WARNING;
		}
		delete  poReturnArg;
	}	
	
	// Borramos las listas de parámetros.
	oListParameter.clear();
	oListParameterOut.clear();

	ret=m_poChannelHandler->PersistChannel(m_poChannelExec);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PERSIST_CHANNEL,ERRORLOG,"#*s1*#C4_JS_TASK_EXE#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClJSExeCommand::RecordJobs%893687424.body
}

m4return_t ClJSExeCommand::AddDateEnd (ClStatsJSEXECUTE *ai_poStats, m4bool_t ai_bPersistChannelExec, m4bool_t ai_bAnyError, m4uint32_t ai_iNumRows, m4uint32_t ai_iPeakRows, m4uint32_t ai_iLimitRows)
{
  //## begin ClJSExeCommand::AddDateEnd%916848072.body preserve=yes

	m4VariantType arg;
	m4return_t ret;
	m4bool_t IsAnyError=M4_FALSE;
	
	ClListParameterVariant oListParameter;
	ClListParameterVariant oListParameterOut;
	ClListParameterVariant::iterator oIteratorParamOut;
	
	M4ClTimeStamp DateEnd;
	DateEnd.now();
	m4double_t date;
	DateEnd.get(date);
	
	//Comprobamos en que condiciones hemos 
	//finalizado la tarea
	if (ai_bAnyError == M4_TRUE)
	{
		// Cuando entramos por este punto es porque se ha producido
		// una situación de error y el job ha finalizado con error

		// Añadimos un job finalizado con error a las estadísticas
		if (ai_poStats != NULL)
		{
			((ClStatsJSEXECUTE *)ai_poStats)->AddJobsError(1);
		}

		// Añadimos la fecha de fin de la subtarea.
		arg.Type = M4CL_CPP_TYPE_DATE;
		arg.Data.DoubleData = date;

		// Se llama al método SET_END_DATE que se encarga de actualizar 
		// la fecha de fin de la ejecución de la subtarea.
		oListParameter.push_back(&arg);
		ret = m_poChannelHandler->ExecuteMethod("SET_END_DATE", oListParameter, m_poNodeJExec, m_poAccessExec, oListParameterOut);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_SET_VALUE_ERROR,ERRORLOG,"#*s1*#END_DATETIME#N4_JS_SUBTASK_EXE#C4_JS_TASK_EXE#");
			SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");
		}

		// Obtenemos de la lista de salida el puntero a variant
		// en el que almacenamos el valor de  retorno del método
		// LN4 que estamos invocando.
		oIteratorParamOut = oListParameterOut.begin();

		if (oIteratorParamOut != oListParameterOut.end())
		{
			m4VariantType * poReturnArg = *oIteratorParamOut;

			if (poReturnArg->Data.DoubleData != 0)
			{
				delete poReturnArg;
				return M4_ERROR;
			}
			delete  poReturnArg;
		}

		// Borramos las listas.
		oListParameter.clear();
		oListParameterOut.clear();

		//Añadimos un flag en BBDDD que 
		//indica que ha finalizado erróneamente
		arg.Type=M4CL_CPP_TYPE_NUMBER;
		arg.Data.DoubleData=-1.0;

		ret = m_poChannelHandler->SetData(m_poNodeJExec, "VM_EXIT_FLAG", arg);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#VM_EXIT_FLAG#N4_SUBTASK_EXE#C4_JS_TASK_EXE#");
			SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");
		}
	}

	// Sets rows quota statistics.
	arg.Type = M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = ai_iNumRows;

	ret = m_poChannelHandler->SetData(m_poNodeJExec, "QUOTA_ROWS", arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#QUOTA_ROWS#N4_JS_SUBTASK_EXE#C4_JS_TASK_EXE#");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");
	}

	arg.Data.DoubleData = ai_iPeakRows;

	ret = m_poChannelHandler->SetData(m_poNodeJExec, "QUOTA_PEAK_ROWS", arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#QUOTA_PEAK_ROWS#N4_JS_SUBTASK_EXE#C4_JS_TASK_EXE#");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");
	}

	arg.Data.DoubleData = ai_iLimitRows;

	ret = m_poChannelHandler->SetData(m_poNodeJExec, "QUOTA_MAX_ROWS", arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#QUOTA_MAX_ROWS#N4_JS_SUBTASK_EXE#C4_JS_TASK_EXE#");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");
	}

	 //Si no ha fallado previamente el método
	 //que graba en BBDD los datos almacenado 
	 //en el canal de ejecuciones
	if (M4_TRUE == ai_bPersistChannelExec)
	{
		ret = m_poChannelHandler->PersistChannel(m_poChannelExec);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_PERSIST_CHANNEL, ERRORLOG, "#*s1*#C4_JS_TASK_EXE#");
			SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");

			return M4_ERROR;
	   }
	}
	 

	// Grabamos en BBDD los ficheros 
	// a los que se asocia la acción de copia
    ret = RecoverFileAfterPersist();
    if (M4_SUCCESS != ret)	
    {
	   SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");

	   IsAnyError=M4_TRUE;
	}
   
	// Determinamos si el job que estamos ejecutando es el último 
	// del box
	SETCODEF(M4_DEBUG_IS_LAST_IN_BOX , DEBUGINFOLOG , "#*s1*#");
	
	ret = m_poChannelHandler->GetData(m_poNodeJDef, "PR_IS_LAST_TASK", arg, NULL);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");
		IsAnyError=M4_TRUE;
	}

	if (arg.Type == '\0')
	{
		SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");
		IsAnyError=M4_TRUE;
	}

	m4double_t bIsLastInBox = arg.Data.DoubleData;

	// Obtenemos el estado interno del ejecutor 
	// de manera que si es abortado modificamos el estado 
	// del trabajo y no ejecutamos ninguna tarea suya más.
	m4uint32_t status = GetInternalStatus();

	if (M4_NO_IS_LAST_IN_BOX != bIsLastInBox || M4_TASK_ABORTED == status || M4_TASK_TIMEOUT  == status)
	{
	    
	     if (M4_TASK_ABORTED == status || M4_TASK_TIMEOUT == status)
		 {
			// Si entramos por este punto significa que la tarea ha 
			// sido interrumpida y tenemos que incrementar el múmero
			// de tareas interrumpidas.
			if (ai_poStats != NULL)
			{
				((ClStatsJSEXECUTE *)ai_poStats)->AddBoxInterrupted(1);
			}

			arg.Type = M4CL_CPP_TYPE_NUMBER;

			// Distinguimos si ha sido abortado por el usuario
			// o por interrupción por consumo del tiempo máximo 
			// de ejecución.
			if (M4_TASK_ABORTED == status)
			{
				arg.Data.DoubleData = STATUS_ABORTED_TOTAL;
			}
			else
			{
				arg.Data.DoubleData = STATUS_INTERRUPTED;
			}

			// Para guardar el estado se llama al método SET_STATUS.
			oListParameter.push_back(&arg);
			ret = m_poChannelHandler->ExecuteMethod("SET_STATUS", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);
			if (M4_SUCCESS != ret)
			{
				SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");

			    IsAnyError=M4_TRUE;
			}
			
		   	oIteratorParamOut = oListParameterOut.begin();
			if (oIteratorParamOut != oListParameterOut.end())
			{
				m4VariantType * poReturnArg = *oIteratorParamOut;

				if (poReturnArg->Data.DoubleData != 0)
				{
					delete poReturnArg;
					return M4_ERROR;
				}
				delete  poReturnArg;
			}

			// Borramos las listas.
			oListParameter.clear();
			oListParameterOut.clear();
		 }
		 else
		 {
			//Si entramos por este camino significa que el trabajo ha sido
			//completado y podemos incrementar en uno las estadísticas
			if (ai_poStats != NULL)
			{
				((ClStatsJSEXECUTE *)ai_poStats)->AddJobsGroupCompleted(1);
			}

		    arg.Type=M4CL_CPP_TYPE_NUMBER;
		    arg.Data.DoubleData=STATUS_EXECUTED;
			oListParameter.push_back(&arg);
			ret = m_poChannelHandler->ExecuteMethod("SET_STATUS", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);

			if (M4_SUCCESS != ret)
			{
				SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#STATUS#N4_TASK_EXE#C4_JS_TASK_EXE#");
				SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");

				IsAnyError=M4_TRUE;
			}

		   	oIteratorParamOut = oListParameterOut.begin();
			if (oIteratorParamOut != oListParameterOut.end())
			{
				m4VariantType * poReturnArg = *oIteratorParamOut;

				if (poReturnArg->Data.DoubleData != 0)
				{
					delete poReturnArg;
					return M4_ERROR;
				}
				delete  poReturnArg;
			}

			// Borramos las listas.
			oListParameter.clear();
			oListParameterOut.clear();
		 }

		//Grabamos la fecha en la que finalizó 
		//el trabajo
		arg.Type=M4CL_CPP_TYPE_DATE;
		arg.Data.DoubleData=date;

		oListParameter.push_back(&arg);
		ret = m_poChannelHandler->ExecuteMethod("SET_END_DATE", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#END_DATETIME#N4_JS_TASK_EXE#C4_JS_TASK_EXE#");
			SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");

			IsAnyError=M4_TRUE;
		}

		oIteratorParamOut = oListParameterOut.begin();
		if (oIteratorParamOut != oListParameterOut.end())
		{
			m4VariantType * poReturnArg = *oIteratorParamOut;

			if (poReturnArg->Data.DoubleData != 0)
			{
				delete poReturnArg;
				return M4_ERROR;
			}
			delete  poReturnArg;
		}

		// Borramos las listas.
		oListParameter.clear();
		oListParameterOut.clear();

		// Se actualiza el código de retorno de la ejecución de la tarea
		// (VM_EXIT_FLAG) mediante el método SET_VM_EXIT_FLAG.
		// Se utilizan listas distintas a las anteriores porque si no
		// el método al ejeuctar da un error.
		ClListParameterVariant oListParameter2, oListParameterOut2;
		ret = m_poChannelHandler->ExecuteMethod("SET_VM_EXIT_FLAG", oListParameter2, m_poNodeGExec, m_poAccessExec, oListParameterOut2);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#VM_EXIT_FLAG#N4_JS_TASK_EXE#C4_JS_TASK_EXE#");
			SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END, ERRORLOG, "#*s1*#");

			IsAnyError = M4_TRUE;
		}

		oIteratorParamOut = oListParameterOut2.begin();
		if (oIteratorParamOut != oListParameterOut2.end())
		{
			m4VariantType * poReturnArg = *oIteratorParamOut;

			if (poReturnArg->Data.DoubleData != 0)
			{
				delete poReturnArg;
				return M4_ERROR;
			}
			delete  poReturnArg;
		}

		// Borramos las listas.
		oListParameter2.clear();
		oListParameterOut2.clear();

		// El siguiente paso es grabar los parámetros de entrada/salida
		// de la tarea. Recordemos que al principio de la ejecución 
		// cuando se estaban actualizando la fecha de inicio y estado de la 
		// tarea (compuesta), ya se graban los parámetros de entrada con su
		// valor, por tanto no queremos grabarlos de nuevo, ya están.
		// Pero los parámetros de salida se grabaron con valor null 
		// porque no se había realizado la ejecución de las subtareas de la tarea.
		// Ahora, es el momento de grabarlos con su valor correspondiente.
		m4uint32_t bInParamValue = 0;
		ret = RecoverCompoundJobParams(bInParamValue);
		if (ret != M4_SUCCESS)
		{
			SETCODEF(M4_ERROR_CLIENT_PARAMETERHANDLER, ERRORLOG, "#*S1*#");
			return M4_ERROR;
		}
	}
	
	if (M4_TRUE==ai_bPersistChannelExec )
	{

		ret=m_poChannelHandler->PersistChannel(m_poChannelExec);

		if (	M4_SUCCESS != ret)
		{
			SETCODEF(M4_PERSIST_CHANNEL, ERRORLOG, "#*s1*#C4_JS_TASK_EXE#");
			SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");

			return M4_ERROR;
	   }
	}
   
	if (M4_TRUE ==IsAnyError)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClJSExeCommand::AddDateEnd%916848072.body
}

m4return_t ClJSExeCommand::RecoverFileAfterPersist ()
{
  //## begin ClJSExeCommand::RecoverFileAfterPersist%941096539.body preserve=yes

	//Accedemos al nodo de ficheros externos en el canal de ejecuciones
	ClNode *poNodeExternalFile;
	m4return_t ret;
	m4VariantType arg;

	ret = m_poChannelHandler->GetNode("N4_JS_RESOURCE_EXE", m_poAccessExec, poNodeExternalFile);

	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}


	//Llamamos al método que graba en la base de datos los ficheros externos
	ClListParameterVariant oListParameter,oListParameterOut;
	ret = m_poChannelHandler->ExecuteMethod("SAVE_BLOB_CONTENTS", oListParameter, poNodeExternalFile, m_poAccessExec, oListParameterOut);

	if (M4_SUCCESS != ret)
	{
        SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "SAVE_BLOB_CONTENTS", "C4_JS_TASK_EXE");
		return M4_ERROR;
	}

	//Obtenemos de la lista de salida el puntero a variant
	//en el que almacenamos el valor de  retorno del método
	//LN4 que estamos invocando
	ClListParameterVariant::iterator oIteratorParamOut;
	oIteratorParamOut=oListParameterOut.begin();

	if (oIteratorParamOut != oListParameterOut.end())
	{
		m4VariantType *poreturnarg=*oIteratorParamOut;

		if (-1 == poreturnarg->Data.DoubleData )
		{
			delete poreturnarg;
		    SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "SAVE_BLOB_CONTENTS", "C4_JS_TASK_EXE");
			return M4_ERROR;
		}

		delete  poreturnarg;
	}

	//Modificar en la versión 4.0
	poNodeExternalFile->RecordSet.Current.Begin();

	//Recorremos los recursos externos , los que sean de copia
	//se graba su contenido en BBDD y si no existía previamente a la 
	//ejecución de la tarea el fichero se borran de fichero y se elimina 
	//la ruta de acceso grabada en BBDD
	while (m_poChannelHandler->IsEndedNode(poNodeExternalFile) != M4_TRUE)
	{
		ret = m_poChannelHandler->GetData(poNodeExternalFile, "RES_STORAGE_TYPE", arg, NULL);

		if (M4_SUCCESS != ret)
		{
			return M4_ERROR;
		}

		if (0==strcmp(arg.Data.PointerChar,M4_OWN_TYPE_ACTION_RESOURCE))
		{
			ret = m_poChannelHandler->GetData(poNodeExternalFile, "SEARCH_PATH", arg, NULL);

            
            if ((M4_ERROR != ret) && (arg.Type != M4CL_CPP_TYPE_STRING_VAR))
            {
				SETCODEF(M4_TYPE_PARM_ERROR, ERRORLOG, "#*S1*#%s#%s#", "SEARCH_PATH", "M4CL_CPP_TYPE_STRING_VAR");
                ret = M4_ERROR;
            }
            
			if (M4_ERROR != ret)
			{
				m4VariantType IsErasableFile;
				ret = m_poChannelHandler->GetData(poNodeExternalFile, "IS_ERASABLE", IsErasableFile, NULL);

				if (M4_SUCCESS == ret)
				{
					if (IsErasableFile.Data.DoubleData == 1)
					{
						if (M4RemoveFile(arg.Data.PointerChar) == -1)
						{
							SETCODEF(M4_DELETE_FILE_EXTERNAL,ERRORLOG,"#*S1*#%s#",arg.Data.PointerChar);
						}
					}
					
					//Aquí modificamos el contenido de BBDD 
					// por una cadena vacía donde teníamos
					//la ruta de acceso al fichero
					arg.Type=M4CL_CPP_TYPE_STRING_VAR;
					*arg.Data.PointerChar='\0';

					ret = m_poChannelHandler->SetData(poNodeExternalFile, "SEARCH_PATH", arg);
				}
			}
		}
		m_poChannelHandler->GetNextData(poNodeExternalFile);
	}
											
			
	return M4_SUCCESS;


  //## end ClJSExeCommand::RecoverFileAfterPersist%941096539.body
}

m4return_t ClJSExeCommand::AddLogMessage (m4pchar_t ai_pBuffer, m4uint32_t ai_idJobDef)
{
  //## begin ClJSExeCommand::AddLogMessage%917867871.body preserve=yes

	m4VariantType arg;
	m4pchar_t bufferAux;
	m4return_t ret;
	m4return_t valreturn=M4_SUCCESS;
	m4uint32_t uiLenBuffer;

	//Creamos una lista donde almacenamos los valores por los que vamos a filtrar
	ClListParameterVariant oListParameter, oListParameterOut;

	// Necesitamos posicionarnos en el registro correspondiente
	// de la subtarea (o job) que estamos ejecutando. Para ello
	// ejecutamos el método GO_TO_SUBTASK_EXE pasando el local_id.
	// Filtramos por local_id.
	arg.Type = M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = ai_idJobDef;
	oListParameter.push_back(&arg);

	ret = m_poChannelHandler->ExecuteMethod("GO_TO_SUBTASK_EXE", oListParameter, m_poNodeJExec, m_poAccessExec, oListParameterOut);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GO_TO_SUBTASK_EXE", "C4_JS_TASK_EXE");
		return M4_ERROR;
	}
	
	// Recuperamos el valor de retorno
	m4VariantType * poArg = NULL;
	ClListParameterVariant::iterator oIteratorOut;
	oIteratorOut = oListParameterOut.begin();

	// Comprobacion que tenemos algo en la lista.
	if (oIteratorOut != oListParameterOut.end()) 
	{
		poArg = *oIteratorOut;

		if (poArg->Data.DoubleData != 0)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GO_TO_SUBTASK_EXE", "C4_JS_TASK_EXE");
			delete poArg;

			return M4_ERROR;
		}

		// Nos tenemos que encargar de borrar el puntero
		// al objeto variant que nos devuelve en la lista
		delete poArg;
		poArg = NULL;
	}

	//Al añadir los mensajes de error tenemos que tener en cuenta si durante
	//la ejecución del job se almacenó alguno para concatenarlos con los nuevos 
	//mensajes
	ret = m_poChannelHandler->GetData(m_poNodeJExec, "LOG_MESSAGES", arg, NULL);

	if(M4_ERROR == ret)
	{
		SETCODEF(M4_GET_VALUE_ERROR, ERRORLOG, "#*s1*#LOG_MESSAGES#N4_JS_SUBTASK_EXE#C4_JS_TASK_EXE#");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");

		return M4_ERROR;
	}

	m4uint32_t uiNoTraces=strcmp(ai_pBuffer,"No log trace");

	if(M4_SUCCESS == ret)
	{
		if (0== uiNoTraces)
		{
			bufferAux=new m4char_t[strlen(arg.Data.PointerChar)+1];
			strcpy(bufferAux,arg.Data.PointerChar);
		}
		else
		{
			uiLenBuffer=strlen(arg.Data.PointerChar)+strlen(ai_pBuffer)+4;
			bufferAux=new m4char_t[uiLenBuffer];
			sprintf(bufferAux,"%s\r\n%s\0",arg.Data.PointerChar,ai_pBuffer);
		}
	}
	else
	{
		bufferAux=new m4char_t[strlen(ai_pBuffer)+1];
		strcpy(bufferAux,ai_pBuffer);
	}
		

	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar=bufferAux;

	ret = m_poChannelHandler->SetData(m_poNodeJExec, "LOG_MESSAGES", arg);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#LOG_MESSAGES#N4_JS_TASK_EXE#C4_JS_TASK_EXE#");
		SETCODEF(M4_ERROR_CLIENT_RECOVER_LOG_DATE_END,ERRORLOG,"#*s1*#");

		valreturn = M4_ERROR;
	}

	delete [] bufferAux;

	return valreturn;

  //## end ClJSExeCommand::AddLogMessage%917867871.body
}

m4void_t ClJSExeCommand::SetDumpInfo (ExecDumpInfo *ai_poDumpInfo)
{
  //## begin ClJSExeCommand::SetDumpInfo%921585523.body preserve=yes
	m_poDumpInfo = ai_poDumpInfo;
  //## end ClJSExeCommand::SetDumpInfo%921585523.body
}

m4return_t ClJSExeCommand::AddParams (ClNode *ai_poNodeExeJIT, m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobGroupDEF)
{
  //## begin ClJSExeCommand::AddParams%927125794.body preserve=yes


	m4return_t	ret=m_poParametersHandler->Initialize();

	//Recuperamos el código Apdu
	m4VariantType apdu;

	// Primero se llama a un método que obtiene el código LN4 a ejecutarse
	// con los alias ya resueltos.
	m4VariantType vIdSubTask;
	ClListParameterVariant oListParameter;
	ClListParameterVariant oListParameterOut;

	// Obtenemos el id_subtask para poder filtrar al ejecutar el método.
	ret = m_poChannelHandler->GetData(m_poNodeJDef, "ID_SUBTASK", vIdSubTask, NULL);
	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}
	   
	// Para obtener el código se llama al método GET_FULL_CODE.
	oListParameter.push_back(&vIdSubTask);
	ret = m_poChannelHandler->ExecuteMethod("GET_FULL_CODE", oListParameter, ai_poNodeExeJIT, m_poAccessJIT, oListParameterOut);

	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}
	
	// Obtenemos de la lista de salida el puntero a variant
	// en el que almacenamos el valor de  retorno del método
	// LN4 que estamos invocando.
	ClListParameterVariant::iterator oIteratorParamOut;
	oIteratorParamOut = oListParameterOut.begin();

	if (oIteratorParamOut != oListParameterOut.end())
	{
		m4VariantType * poReturnArg = *oIteratorParamOut;

		if (poReturnArg->Data.DoubleData != 0)
		{
			delete poReturnArg;
			return M4_ERROR;
		}
		delete  poReturnArg;
	}
	
	// Borramos las listas.
	oListParameter.clear();
	oListParameterOut.clear();

	ret = m_poChannelHandler->GetData(ai_poNodeExeJIT, "_R_E_S_E_R_V_E_D_F_O_R_J_I_T__", apdu, NULL);

	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}

   //Invocamos al manejador de parámetros para que 
	//realice las operaciones con parámetros previas a la 
   //ejecución del JIT
   ret=m_poParametersHandler->AddParams(apdu, ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
	
   return ret;

  //## end ClJSExeCommand::AddParams%927125794.body
}

m4return_t ClJSExeCommand::RecoverParams ()
{
  //## begin ClJSExeCommand::RecoverParams%927125795.body preserve=yes

	//Invocamos al manejador de parámetro para que recupere 
	//los parámetros de salida una vez se ha ejecutado el JIT
	m4return_t ret=m_poParametersHandler->RecoverParams();
	
	return ret;

  //## end ClJSExeCommand::RecoverParams%927125795.body
}

m4return_t ClJSExeCommand::RecoverExternalResource (m4uint32_t ai_uiIdJobGDef, m4uint32_t ai_uiIdJobDef, m4uint32_t ai_uiIdBoxExe, m4uint32_t ai_uiIdJobExec)
{
  //## begin ClJSExeCommand::RecoverExternalResource%927648836.body preserve=yes
	m4return_t ret;
	m4uint32_t numRegister;
	m4VariantType patharg,typeactionarg,arg,idblobarg;
	ClFile oFile;
		

	//Accedemos al nodo en los que se almacena la información sobre los recursos
	//para posteriormente recuperar los ficheros externos que la ejecución ha generado

	ClNode *poNodeResourceJIT;	
	ret = m_poChannelHandler->GetNode("N4_JS_EXTERNAL_FILES", m_poAccessJIT, poNodeResourceJIT);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_CLIENT_STRUCTURES, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);

		return M4_ERROR;
	}

	numRegister = poNodeResourceJIT->RecordSet.Current.Count();

	SETCODEF(M4_DEBUG_ASOCIATED_RESOURCES, DEBUGINFOLOG, "#*s1*#%d#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdJobGDef, ai_uiIdBoxExe, numRegister);
	
	//Si no tenemos ningún registro significa que no tenemos recursos externos 
	//para esa ejecución
	if (0 == numRegister)
	{
		return M4_SUCCESS;
	}

	ret=poNodeResourceJIT->RecordSet.Current.Begin();

	if (M4_ERROR == ret)
	{
		SETCODEF(M4_ERROR_MOVE_NODE_EXECUTEJOB, ERRORLOG, "#*s1*#N4_JS_EXTERNAL_FILES#C4_JS_EXE_JIT#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
		SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);

		return M4_ERROR;
	}

	//Obtenemos nodo de recursos
	//Obtenemos el nodo donde se almacenan los ficheros creados por la tarea
	ClNode *poNodeExternalFiles;
	ret = m_poChannelHandler->GetNode("N4_JS_RESOURCE_EXE", m_poAccessExec, poNodeExternalFiles);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_CLIENT_STRUCTURES, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);

		return M4_ERROR;
	}

	// while (! poNodeResourceJIT->RecordSet.Current.Index.IsEOF())
	while (poNodeResourceJIT->RecordSet.Current.Index != M4DM_EOF_INDEX)
	{
		//Obtenemos el path del recurso
		ret = poNodeResourceJIT->RecordSet.Current.Item["FILE_PATH"].Value.Get(patharg);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_GET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#FILE_PATH#N4_JS_EXTERNAL_FILES#C4_JS_EXE_JIT#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
			SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);

			return M4_ERROR;
		}


		if ('\0'== patharg.Type)
		{
			SETCODEF(M4_ERROR_VALUE_NULL_EXECUTEJOB, ERRORLOG, "#*s1*#FILE_PATH#N4_JS_EXTERNAL_FILES#C4_JS_EXE_JIT#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
			SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG,"#*s1*#%d#%d#%d#",ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);

			return M4_ERROR;
		}

		//Obtenemos el tipo de acción a realizar con el recurso
		ret = poNodeResourceJIT->RecordSet.Current.Item["FILE_STORAGE_TYPE"].Value.Get(typeactionarg);

		if (M4_ERROR == ret)
		{
			SETCODEF(M4_ERROR_GET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#FILE_STORAGE_TYPE#N4_JS_EXTERNAL_FILES#C4_JS_EXE_JIT#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
			SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
			
			return M4_ERROR;
		}


		if ('\0'== typeactionarg.Type)
		{
			SETCODEF(M4_ERROR_VALUE_NULL_EXECUTEJOB, ERRORLOG, "#*s1*#FILE_STORAGE_TYPE#N4_JS_EXTERNAL_FILE#C4_JS_EXE_JIT#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
			SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);

			return M4_ERROR;
		}

		//Obtenemos si el fichero existía previamente a la declaración
		m4bool_t  existfile;

		ret = poNodeResourceJIT->RecordSet.Current.Item["EXISTS_FILE"].Value.Get(arg);

		if (M4_ERROR == ret)
		{
			SETCODEF(M4_ERROR_GET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#EXISTS_FILE#N_JS_EXTERNAL_FILE#C4_JS_EXE_JIT#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
			SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);

			return M4_ERROR;
		}

		if ('\0'== arg.Type)
		{
			SETCODEF(M4_ERROR_VALUE_NULL_EXECUTEJOB, ERRORLOG, "#*s1*#EXISTS_FILE#N4_JS_EXTERNAL_FILES#C4_JS_EXE_JIT#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
			SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);

			return M4_ERROR;
		}

		//Almacenamos el valor que indica si el fichero existía previamente o no
		switch ((m4uint32_t)arg.Data.DoubleData)
		{
			case 0:
						existfile=M4_FALSE;
				   
			  break;
			case  1:
				         existfile=M4_TRUE;

			  break;
			default:
				     existfile=M4_TRUE;
				break;
		}


		ret = oFile.SetFileName(patharg.Data.PointerChar);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_FIND_FILE_EXTERNAL, ERRORLOG, "#*s1*#%s#%d#%d#%d#", patharg.Data.PointerChar, ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
		}
		else
		{

			//Si se trata de un fichero temporal tenemos que eliminar 
			// todos los recursos que concuerda  con  la plantilla 

			if (strcmp(typeactionarg.Data.PointerChar,M4_TEMP_TYPE_ACTION_RESOURCE) == 0)
			{			
				//Borramos el fichero solamente si no existía previamente
				if ( M4_FALSE == existfile )
				{
					ret=oFile.Delete();

					if (M4_SUCCESS != ret)
					{
						SETCODEF(M4_DELETE_FILE_EXTERNAL, ERRORLOG, "#*s1*#%s#%d#%d#%d#", patharg.Data.PointerChar, ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					}
				}
			}
			else
			{
				//Las restantes tipos de acciones llevan asociado
				//la creación de un nuevo registro en el nodo de recursos
				//y el rellenar campos de ese nodo
				ret = CreatePrimaryKeyExec(poNodeExternalFiles, ai_uiIdJobGDef, ai_uiIdBoxExe, ai_uiIdJobDef, ai_uiIdJobExec);

				if (M4_SUCCESS != ret)
				{
					SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					return M4_ERROR;
				}

				//Almacenamos el tipo de acción que tenemos
				ret = poNodeExternalFiles->RecordSet.Current.Item["RES_STORAGE_TYPE"].Value.Set(typeactionarg);

				if (M4_SUCCESS != ret)
				{
					SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#RES_STORAGE_TYPE#N4_JS_RESOURCE_EXE#C4_JS_TASKS_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#RES_STORAGE_TYPE#N4_JS_RESOURCE_EXE#C4_JS_TASKS_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					return M4_ERROR;
				}
									
				//Obtenemos el nombre corto
				M4ClString oFileName;
				ret=oFile.GetShortName(oFileName);

				if (M4_ERROR == ret)
				{
					SETCODEF(M4_ERROR_GET_NAME_FILE, ERRORLOG, "#*s1*#%s#%d#%d#%d#", patharg.Data.PointerChar, ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					return M4_ERROR;
				}

				//Almacenamos el nombre del recurso
				arg.Type=M4CL_CPP_TYPE_STRING_VAR;
				arg.Data.PointerChar=(m4pchar_t)oFileName;

				ret = poNodeExternalFiles->RecordSet.Current.Item["RES_NAME"].Value.Set(arg);
					
				if (M4_SUCCESS != ret)
				{
					SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#RES_NAME#N4_JS_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					return M4_ERROR;
				}

				//Almacenamos el tamaño del fichero
				arg.Type=M4CL_CPP_TYPE_NUMBER;
				arg.Data.DoubleData=oFile.GetSize();

				ret = poNodeExternalFiles->RecordSet.Current.Item["CONTENT_SIZE"].Value.Set(arg);

				if (M4_SUCCESS != ret)
				{
					SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#CONTENT_SIZE#N4_JS_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					return M4_ERROR;
				}

				m4uint16_t IsTypeProp,IsTypePropRef;

				IsTypePropRef=strcmp(typeactionarg.Data.PointerChar,M4_OWNREF_TYPE_ACTION_RESOURCE);

				//Almacenamos el path completa del fichero aunque luego 
				//el método que graba los blob si se trata de una acción de copia
				//borra este path
				arg.Type=M4CL_CPP_TYPE_STRING_VAR;
				arg.Data.PointerChar=(char *)oFile.GetFileName();

				ret = poNodeExternalFiles->RecordSet.Current.Item["SEARCH_PATH"].Value.Set(arg);
					
				if (M4_SUCCESS != ret)
				{				
					SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#SEARCH_PATH#N4_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					return M4_ERROR;
				}

				//Almacenamos si el fichero es borrable o no
				// 1-> Es borrable
				//0 -> No es borrable	
				arg.Type=M4CL_CPP_TYPE_NUMBER;

				if (M4_TRUE == existfile)
				{
					arg.Data.DoubleData=0;
				}
				else
				{
					arg.Data.DoubleData=1;
				}

				ret = poNodeExternalFiles->RecordSet.Current.Item["IS_ERASABLE"].Value.Set(arg);

				if (M4_SUCCESS != ret)
				{
					SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#IS_ERASABLE#N4_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
					return M4_ERROR;
				}	

				if ((strcmp(typeactionarg.Data.PointerChar,M4_REF_TYPE_ACTION_RESOURCE)==0 ))
				{
					M4ClTimeStamp oDateExpired;
					m4double_t datetoexpired;

					oDateExpired.set("4000-01-01 00:00:00");
					oDateExpired.get(datetoexpired);

					arg.Type=M4CL_CPP_TYPE_DATE;
					arg.Data.DoubleData=datetoexpired;
				
					poNodeExternalFiles->RecordSet.Current.Item["EXPIRATION_DATE"].Value.Set(arg);

					if (M4_SUCCESS != ret)
					{
						SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#EXPIRATION_DATE#N4_RESOURCE_EXES#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
						SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG,"#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
						return M4_ERROR;
					}
				}
				
				IsTypeProp=strcmp(typeactionarg.Data.PointerChar,M4_OWN_TYPE_ACTION_RESOURCE);
				
				if ((IsTypeProp ==0) || (IsTypePropRef == 0))
				{
					//Almacenamos la fecha de expiración
					M4ClTimeStamp DateExpired;
					m4double_t uiDateExpired;

					ret = poNodeResourceJIT->RecordSet.Current.Item["DAY_TO_EXPIRE"].Value.Get(arg);

					if (M4_SUCCESS != ret)
					{
						SETCODEF(M4_ERROR_GET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
						SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG,"#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
						return M4_ERROR;
					}

					DateExpired.now();

					//Convertimos los dias que esperamos a borrarlo en minutos
					m4uint32_t uiDelayMinutes=(m4uint32_t)arg.Data.DoubleData*24*60;

					DateExpired.add_minutes(uiDelayMinutes);
					DateExpired.get(uiDateExpired);

					arg.Type=M4CL_CPP_TYPE_DATE;
					arg.Data.DoubleData=uiDateExpired;

					ret = poNodeExternalFiles->RecordSet.Current.Item["EXPIRATION_DATE"].Value.Set(arg);

					if (M4_SUCCESS != ret)
					{		
						SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#EXPIRATION_DATE#N4_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
						SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
						return M4_ERROR;
					}

					//Almacenamos el STATUS del blob
					arg.Type=M4CL_CPP_TYPE_NUMBER;
					arg.Data.DoubleData=STATUS_NO_ASIGNADO;

					ret = poNodeExternalFiles->RecordSet.Current.Item["STATUS"].Value.Set(arg);

					if (M4_SUCCESS != ret)
					{		
						SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#STATUS#N4_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
						SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
						return M4_ERROR;
					}
				}
			}
		}
		
		poNodeResourceJIT->RecordSet.Current.Next();
	}

	return M4_SUCCESS ;

  //## end ClJSExeCommand::RecoverExternalResource%927648836.body
}

m4return_t ClJSExeCommand::InitializeGroupExecution (m4uint32_t ai_idJobGroupDEF, m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, ClStatsJSEXECUTE *ai_poStats)
{
  //## begin ClJSExeCommand::InitializeGroupExecution%894391143.body preserve=yes
		
	m4VariantType arg;
	M4ClTimeStamp 	DateNow;
	m4double_t    datetonumber;
	m4VariantType vSchedDate;
	m4return_t	ret;

	//Rellenamos las estadísticas con los datos que conocemos
	//tales como el identificador del trabajo , el identificador
	//de la tarea 
	//Rellenamos la estadística del identificador del trabajo y la tarea actual
	if (ai_poStats != NULL)
	{
		ai_poStats->ActiveIdentifiersJob(ai_idJobGroupDEF, ai_idJobDEF, ai_idBoxExe);
	}

	// Obtenemos la fecha de planificación del canal de ejecuciones,
	// del nodo de ejecucion de tareas.
	ret = m_poChannelHandler->GetData(m_poNodeGExec, "PLANNED_DATETIME", vSchedDate, NULL);
	
	if (ret != M4_SUCCESS)
	{
		SETCODEF(M4_ERROR_GET_ITEM_FINDJOB,ERRORLOG, "#*s1*#PLANNED_DATETIME#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
		return M4_ERROR;
	}

	//Obtenemos el estatus de ejecución  del job group
	ret = m_poChannelHandler->GetData(m_poNodeGExec, "STATUS", arg, NULL);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_GET_ITEM_FINDJOB,ERRORLOG,"#*s1*#STATUS#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
		return M4_ERROR;
	}

	ClListParameterVariant oListParameter;
	ClListParameterVariant oListParameterOut;
	if (!strcmp(arg.Data.PointerChar, "3") ||	// STATUS_CANCEL
	!strcmp(arg.Data.PointerChar, "10") ||		// STATUS_ABORTED
	!strcmp(arg.Data.PointerChar, "7")	||		// STATUS_CANCELED_TOTAL
	!strcmp(arg.Data.PointerChar, "11") )		// STATUS_ABORTED_TOTAL
	{
		// Si la tarea está en estado cancelando (3) o abortando (10),
		// se actualiza el estado a cancelado (7) o abortado (11).
		// En el caso de que se encuentre en estado cancelado (7) o
		// abortado (11), sólo se actualizan las fechas de inicio 
		// y fin de la tarea.
		if (!strcmp(arg.Data.PointerChar, "3") ||	// STATUS_CANCEL
			!strcmp(arg.Data.PointerChar, "10")) 	// STATUS_ABORTED
		{
			//Cancelamos el job definitivamente para que nadie pueda 
			//cambiar su estado
			arg.Type=M4CL_CPP_TYPE_NUMBER;
			arg.Data.DoubleData=STATUS_CANCELED_TOTAL;
	    
			// Para guardar el estado se llama al método SET_STATUS.
			oListParameter.push_back(&arg);
			ret = m_poChannelHandler->ExecuteMethod("SET_STATUS", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);

			if (M4_SUCCESS != ret)
			{
				SETCODEF(M4_ERROR_SET_ITEM_FINDJOB, ERRORLOG, "#*s1*#STATUS#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
				return M4_ERROR;
			}
		
			// Obtenemos de la lista de salida el puntero a variant
			// en el que almacenamos el valor de  retorno del método
			// LN4 que estamos invocando.
			ClListParameterVariant::iterator oIteratorParamOut;
			oIteratorParamOut = oListParameterOut.begin();

			if (oIteratorParamOut != oListParameterOut.end())
			{
				m4VariantType * poReturnArg = *oIteratorParamOut;

				if (poReturnArg->Data.DoubleData != 0)
				{
					delete poReturnArg;
					return M4_ERROR;
				}
				delete  poReturnArg;
			}

			// Borramos las listas de parámetros.
			oListParameter.clear();
			oListParameterOut.clear();

			//Actualizamos el retraso que llevamos en la ejecución de la tarea
			//las estadísticas
			if (ai_poStats != NULL)
			{
				m4double_t DateExecution;
			
				// Convertimos el double del variant en un timestamp
				DateNow.set(vSchedDate.Data.DoubleData);
				DateNow.get(DateExecution);

				ai_poStats->SetTimeDelay(DateExecution);
			
				//Aumentamos las estadísticas de numero de trabajos 
				//interrumpidos
				ai_poStats->AddBoxInterrupted(1);
			}
	
			SETCODEF(M4_INTERRUPTED_JOBGROUP, DEBUGINFOLOG, "#*s1*#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);	
		}
		
		//Si estamos lanzando la primera tarea del trabajo
		//tendremos que poner fecha de inicio . Lo que comprobamos es 
		//si el campo fecha de inicio está vacío en cuyo caso lo rellenamos
		m4VariantType argDate;
	
		DateNow.now();
		DateNow.get(datetonumber);

		ret = m_poChannelHandler->GetData(m_poNodeGExec, "START_DATETIME", argDate, NULL);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_GET_ITEM_FINDJOB,ERRORLOG,"#*s1*#START_DATETIME#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
			return M4_ERROR;
		}

		if ('\0'==argDate.Data.DoubleData)
		{
			argDate.Type=M4CL_CPP_TYPE_DATE;
			argDate.Data.DoubleData=datetonumber;

			// Se llama al método SET_START_DATE que se encarga de axtualizar la fecha de inicio
			// de la ejecución de la tarea y de la subtarea con local_id = 0.
			oListParameter.push_back(&argDate);
			ret = m_poChannelHandler->ExecuteMethod("SET_START_DATE", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);

    		if (M4_SUCCESS != ret)
			{
				SETCODEF(M4_ERROR_SET_ITEM_FINDJOB, ERRORLOG, "#*s1*#START_DATETIME#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
				return M4_ERROR;
			}

			// Obtenemos de la lista de salida el puntero a variant
			// en el que almacenamos el valor de  retorno del método
			// LN4 que estamos invocando.
			ClListParameterVariant::iterator oIteratorParamOut;
			oIteratorParamOut = oListParameterOut.begin();

			if (oIteratorParamOut != oListParameterOut.end())
			{
				m4VariantType * poReturnArg = *oIteratorParamOut;

				if (poReturnArg->Data.DoubleData != 0)
				{
					delete poReturnArg;
					return M4_ERROR;
				}
				delete  poReturnArg;
			}
		}
		
		// Borramos las listas de parámetros.
		oListParameter.clear();
		oListParameterOut.clear();

		//Lo mismo ocurre con la fecha de fin comprobamos di el 
		//campo está vacío en cuyo caso lo tenemos que rellenar
		m_poChannelHandler->GetData(m_poNodeGExec, "END_DATETIME", argDate, NULL);

		if ( '\0'==argDate.Data.DoubleData )
		{
			argDate.Type=M4CL_CPP_TYPE_DATE;
			argDate.Data.DoubleData=datetonumber;

			// Se llama al método SET_END_DATE que se encarga de actualizar la 
			// fecha de fin de la ejecución de la tarea y de la subtarea con
			// local_id = 0.
			oListParameter.push_back(&argDate);
			ret = m_poChannelHandler->ExecuteMethod("SET_END_DATE", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);

			if (M4_SUCCESS != ret)
			{
				SETCODEF(M4_ERROR_SET_ITEM_FINDJOB, ERRORLOG, "#*s1*#END_DATETIME#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
				return M4_ERROR;
			}

			// Obtenemos de la lista de salida el puntero a variant
			// en el que almacenamos el valor de  retorno del método
			// LN4 que estamos invocando.
			ClListParameterVariant::iterator oIteratorParamOut;
			oIteratorParamOut = oListParameterOut.begin();

			if (oIteratorParamOut != oListParameterOut.end())
			{
				m4VariantType * poReturnArg = *oIteratorParamOut;

				if (poReturnArg->Data.DoubleData != 0)
				{
					delete poReturnArg;
					return M4_ERROR;
				}
				delete  poReturnArg;
			}
		}
		
		// Bug 0278384. Notificar el fin de la ejecución de la planificación.
		_SendEndExecutionMail(ai_idJobGroupDEF, ai_idBoxExe);

		//A continuación persistimos los cambios realizados
		ret=m_poChannelHandler->PersistChannel(m_poChannelExec);

		if (M4_SUCCESS != ret)
		{		
			SETCODEF(M4_ERROR_PERSIST_META4OBJECT_FINDJOB, ERRORLOG, "#*s1*#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
			return M4_ERROR;
		}

		// Se devuelve error para que no continue la ejecución de 
		// la tarea.
		return M4_ERROR;
	}
	else if (!strcmp(arg.Data.PointerChar, "1"))// STATUS_WAITING
	{
		//Si el estado es pendiente entonces 
		//cambiamos a ejecutandose sino 

		//Actualizamos el retraso que llevamos en la ejecución de la tarea
		//las estadísticas
		if (ai_poStats != NULL)
		{
			ai_poStats->SetTimeDelay(vSchedDate.Data.DoubleData);
		}
			
		//Modificamos el estado a ejecutandose
		arg.Type=M4CL_CPP_TYPE_NUMBER;
		arg.Data.DoubleData=STATUS_EXECUTE;

		// Para guardar el estado se llama al método SET_STATUS.
		oListParameter.push_back(&arg);
		ret = m_poChannelHandler->ExecuteMethod("SET_STATUS", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_SET_ITEM_FINDJOB, ERRORLOG, "#*s1*#STATUS#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
			return M4_ERROR;
		}

		// Obtenemos de la lista de salida el puntero a variant
		// en el que almacenamos el valor de  retorno del método
		// LN4 que estamos invocando.
		ClListParameterVariant::iterator oIteratorParamOut;
		oIteratorParamOut = oListParameterOut.begin();

		if (oIteratorParamOut != oListParameterOut.end())
		{
			m4VariantType * poReturnArg = *oIteratorParamOut;

			if (poReturnArg->Data.DoubleData != 0)
			{
				delete poReturnArg;
				return M4_ERROR;
			}
			delete  poReturnArg;
		}
	
		// Borramos las listas de parámetros.
		oListParameter.clear();
		oListParameterOut.clear();

		DateNow.now();
		DateNow.get(datetonumber);

		//Grabamos la fecha en la que se lanza
		arg.Type=M4CL_CPP_TYPE_DATE;	
		arg.Data.DoubleData=datetonumber;

		// Se llama al método SET_START_DATE que se encarga de axtualizar la fecha de inicio
		// de la ejecución de la tarea y de la subtarea con local_id = 0.
		oListParameter.push_back(&arg);
		ret = m_poChannelHandler->ExecuteMethod("SET_START_DATE", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_SET_ITEM_FINDJOB, ERRORLOG, "#*s1*#START_DATETIME#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
			return M4_ERROR;
		}

		// Obtenemos de la lista de salida el puntero a variant
		// en el que almacenamos el valor de  retorno del método
		// LN4 que estamos invocando.
		oIteratorParamOut = oListParameterOut.begin();
		if (oIteratorParamOut != oListParameterOut.end())
		{
			m4VariantType * poReturnArg = *oIteratorParamOut;

			if (poReturnArg->Data.DoubleData != 0)
			{
				delete poReturnArg;
				return M4_ERROR;
			}
			delete  poReturnArg;
		}
		
		// Borramos las listas.
		oListParameter.clear();
		oListParameterOut.clear();
		
		// En el momento de la inicialización de la tarea, y sólo 
		// para la primera subtarea de la tarea, se deben grabar en
		// base de datos los parámetros de la tarea.
		// En el caso de los posibles parámetros de salida, como no
		// hemos ejecutado todavía la subtarea, no tienen valor asignado
		// con lo que queremos que su valor sea null.
		// Los parámetros de salida se graban también.
		m4uint32_t bOutParamValue = 0;
		m4uint32_t bInParamValue = 1;
		ret = RecoverCompoundJobParams(bInParamValue, bOutParamValue);
		if (ret != M4_SUCCESS)
		{
			SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
			return M4_ERROR;
		}

		//Persistimos los cambios del canal del job group exec
		ret = m_poChannelHandler->PersistChannel(m_poChannelExec);
		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_PERSIST_META4OBJECT_FINDJOB, ERRORLOG, "#*s1*#C4_JS_TASK_EXE#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
			return M4_ERROR;
		}
	}
	
	// Borramos las listas.
	oListParameter.clear();
	oListParameterOut.clear();

	return M4_SUCCESS;
	
  //## end ClJSExeCommand::InitializeGroupExecution%894391143.body
}

m4return_t ClJSExeCommand::CreatePrimaryKeyExec (ClNode *ai_poNode, m4uint32_t ai_uiIdJobGDef, m4uint32_t ai_uiIdBoxExe, m4uint32_t ai_uiIdJobDef, m4uint32_t ai_uiIdJobExec)
{
  //## begin ClJSExeCommand::CreatePrimaryKeyExec%927794195.body preserve=yes

	m4VariantType arg;
	m4return_t ret;

	ret=m_poChannelHandler->CreateNewData(ai_poNode);

	if (M4_ERROR == ret)
	{
		SETCODEF(M4_ADD_REGISTER_ERROR, ERRORLOG, "#*s1*#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
		return M4_ERROR;
	}

	//Añadimos el campo ID_JOB_GROUP_DEF
	arg.Type=M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData=ai_uiIdJobGDef;

	ret = m_poChannelHandler->SetData(ai_poNode, "ID_SCHED_TASK", arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#ID_SCHED_TASK#N4_JS_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
		return M4_ERROR;
	}

	//Añadimos el campo ID_JOB_GROUP_EXEC
	arg.Type=M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = ai_uiIdBoxExe;
	ret = m_poChannelHandler->SetData(ai_poNode, "ID_TASK_EXE", arg);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#ID_TASK_EXE#N4_JS_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
		return M4_ERROR;
	}
	
	//Añadimos el campo ID_JOB_EXEC siempre que sea 
	//distinto de cero
	if (ai_uiIdJobExec != 0)
	{
		arg.Type=M4CL_CPP_TYPE_NUMBER;
		arg.Data.DoubleData=ai_uiIdJobExec;
		ret = m_poChannelHandler->SetData(ai_poNode, "ID_SUBTASK_EXE", arg);

		if (M4_SUCCESS != ret)
		{
			SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#ID_SUBTASK_EXE#N4_RESOURCE_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJobDef, ai_uiIdBoxExe, ai_uiIdJobGDef);
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;

  //## end ClJSExeCommand::CreatePrimaryKeyExec%927794195.body
}

m4return_t ClJSExeCommand::ReLoadChannel (m4uint32_t ai_uiIdJobGroupDef, m4uint32_t ai_uiIdJobDef, m4uint32_t ai_uiIdBoxExe)
{
  //## begin ClJSExeCommand::ReLoadChannel%929100413.body preserve=yes

	m4return_t ret;
	m4VariantType *argfilter1,*argfilter2;
	m4VariantType arg;

	//Creamos una lista donde almacenamos los valores por los que vamos a filtrar
	ClListParameterVariant oListParameter, oListParameterOut;

	argfilter1=new m4VariantType;
	argfilter2=new m4VariantType;

	//Almacenamos el identificador del grupo de definiciones
	argfilter1->Type=M4CL_CPP_TYPE_NUMBER;
	argfilter1->Data.DoubleData=ai_uiIdJobGroupDef;

	oListParameter.push_back(argfilter1);

	argfilter2->Type = M4CL_CPP_TYPE_NUMBER;
	argfilter2->Data.DoubleData = ai_uiIdBoxExe;

	oListParameter.push_back(argfilter2);

	ret = m_poChannelHandler->ReloadChannelExecution (m_poChannelExec, m_poAccessExec, "N4_JS_TASK_EXE", m_poNodeGExec, "N4_JS_SUBTASK_EXE", oListParameter, m_poNodeJExec);

	// Vaciamos la lista
	oListParameter.clear();

	// Necesitamos posicionarnos en el registro correspondiente
	// de la subtarea (o job) que estamos ejecutando. Para ello
	// ejecutamos el método GO_TO_SUBTASK_EXE pasando el local_id.
	// Filtramos por local_id.
	arg.Type = M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = ai_uiIdJobDef;
	oListParameter.push_back(&arg);

	ret = m_poChannelHandler->ExecuteMethod("GO_TO_SUBTASK_EXE", oListParameter, m_poNodeJExec, m_poAccessExec, oListParameterOut);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GO_TO_SUBTASK_EXE", "C4_JS_TASK_EXE");
		delete argfilter1;
		delete argfilter2;
	
		return M4_ERROR;
	}
	
	// Recuperamos el valor de retorno
	m4VariantType * poArg = NULL;
	ClListParameterVariant::iterator oIteratorOut;
	oIteratorOut = oListParameterOut.begin();

	// Comprobacion que tenemos algo en la lista.
	if (oIteratorOut != oListParameterOut.end()) 
	{
		poArg = *oIteratorOut;

		if (poArg->Data.DoubleData != 0)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "GO_TO_SUBTASK_EXE", "C4_JS_TASK_EXE");
			delete argfilter1;
			delete argfilter2;
			delete poArg;

			return M4_ERROR;
		}

		// Nos tenemos que encargar de borrar el puntero
		// al objeto variant que nos devuelve en la lista
		delete poArg;
		poArg = NULL;
	}

	delete argfilter1;
	delete argfilter2;
	
	//ADD FJSM 1999-11-25 Vaciamos la lista
	oListParameter.clear();
	oListParameterOut.clear();
	
	return ret;

  //## end ClJSExeCommand::ReLoadChannel%929100413.body
}

m4return_t ClJSExeCommand::RecoverLog (m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobGroupDEF)
{
  //## begin ClJSExeCommand::RecoverLog%935686407.body preserve=yes

	m4return_t				iResult = M4_ERROR;
	m4return_t				iError = M4_SUCCESS;
	m4bool_t				bError = M4_FALSE;
	m4uint32_t				i = 0;
	m4uint32_t				iLength = 0;
	m4uint32_t				iSize = M4JS_LOG_SIZE;
	m4int32_t				iCopied = 0;
	m4pchar_t				pcBuffer = NULL;
	string					sLogs ( "" ); 
	m4VariantType			vVmExitFlag;
	
	/* Bug 0263523
	Los errores se recuperan sin gestión de marcas.
	Se vuelcan todos y al final se borran.
	*/
	iLength = GET_ERROR_QUEUE_SIZE() ;

	if( iLength > 0 )
	{
		pcBuffer = new m4char_t[ iSize + 1 ] ;

		if( pcBuffer == NULL )
		{
			SETCODEF( M4_ERROR_GET_TEXT_ERRORLOG, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF );
			return M4_ERROR;
		}

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = GETH_N_ERRORTYPE( i, pcBuffer, iSize, iCopied ) ;

			if( iResult != M4_SUCCESS && iCopied != iSize - 1 )
			{
				SETCODEF( M4_ERROR_GET_TYPE_ERRORLOG, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF );
			}

			if( bError == M4_FALSE && strcmpi( pcBuffer, M4JS_LOG_ERROR ) == 0 )
			{
				/* Bug 0264810
				Si hay cualquier error se marca como erroneo, no sólo si es funcional
				*/
				bError = M4_TRUE;
			}

			sLogs.append( "\r\n " );
			sLogs.append( pcBuffer );

			iResult = GETH_N_ERRORCODE( i, pcBuffer, iSize, iCopied ) ;

			if( iResult != M4_SUCCESS && iCopied != iSize - 1 )
			{
				SETCODEF( M4_ERROR_GET_CODE_ERRORLOG, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF );
			}

			sLogs.append( " " );
			sLogs.append( pcBuffer );

			iResult = GETH_N_ERRORTEXT( i, pcBuffer, iSize, iCopied ) ;

			if( iResult != M4_SUCCESS && iCopied != iSize - 1 )
			{
				SETCODEF( M4_ERROR_GET_TEXT_ERRORLOG, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF );
			}

			sLogs.append( " " );
			sLogs.append( pcBuffer );
		}

		delete pcBuffer;
		CLEAN();
	}
	else
	{
		sLogs.append( "No log trace" );
	}

    //Volcamos datos de traceo para la captura de excepciones
 	if( m_poDumpInfo != NULL )
	{
		m_poDumpInfo->SetCurrentStage( "JS Exec Commands: ExecuteJob Calling AddLogMessage" );
	}
	
	iResult = AddLogMessage( (m4pchar_t)sLogs.c_str(), ai_idJobDEF );

	if( iResult != M4_SUCCESS )
	{
		SETCODEF( M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#LOG#N4_SUBTASK_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF );
		iError = M4_ERROR;
	}

	if( bError == M4_TRUE )
	{
		vVmExitFlag.SetDouble( -1 );

		iResult = m_poChannelHandler->SetData( m_poNodeJExec, "VM_EXIT_FLAG", vVmExitFlag );

		if( iResult != M4_SUCCESS )
		{
			SETCODEF( M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#VM_EXIT_FLAG#N4_SUBTASK_EXEC#C4_JS_TASK_EXE#%d#%d#%d#", ai_idJobDEF, ai_idJobGroupDEF, ai_idBoxExe );
			SETCODEF( M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF );
			iError = M4_ERROR;
		}
	}

	// Bug 0274693. Notificar el fin de la ejecución de la planificación.
	_SendEndExecutionMail(ai_idJobGroupDEF, ai_idBoxExe);

	iResult = m_poChannelHandler->PersistChannel( m_poChannelExec );

	if( iResult != M4_SUCCESS )
	{
		SETCODEF( M4_PERSIST_CHANNEL, ERRORLOG, "#*s1*#C4_JS_TASK_EXE" );
		iError = M4_ERROR;
	}

	return iError;
  //## end ClJSExeCommand::RecoverLog%935686407.body
}

m4return_t ClJSExeCommand::Cancel ()
{
  //## begin ClJSExeCommand::Cancel%936777416.body preserve=yes

	//Almacenamos el valor de la variable
	//que marca el estado interno del ejecutor
	m_poMutexStatus.Lock();

	m4uint32_t aux=m_poInternalStatus;

	m_poMutexStatus.Unlock();

	if (M4_EXECUTING_JIT==aux)
	{
		m_poChannelHandler->AbortMVC();
	}

	return M4_SUCCESS;

  //## end ClJSExeCommand::Cancel%936777416.body
}

m4return_t ClJSExeCommand::SetInternalStatus (m4uint32_t ai_uiInternalStatus)
{
  //## begin ClJSExeCommand::SetInternalStatus%936777417.body preserve=yes

		m4return_t  ret=M4_SUCCESS;

	m_poMutexStatus.Lock();

	m_poInternalStatus=ai_uiInternalStatus;

	if (ai_uiInternalStatus==M4_EXECUTING_JIT)
	{
		//Si vamo a comenzar a ejecutar el  jit 
		//establecemos el TIME OUT
	
		//Comprobamos si hay TimeOut establecido
		if (ClStatus::InfiniteTimeOut != m_uiTimeOut)
		{

			ret=m_poChannelHandler->SetTimeOutInMVC(m_uiTimeOut);

			if (M4_SUCCESS != ret)
			{

				SETCODEF(M4_ERROR_JS_SET_TIMEOUT,ERRORLOG,"#*S1*#");
			}

		}

	}

	m_poMutexStatus.Unlock();


	return ret;

  //## end ClJSExeCommand::SetInternalStatus%936777417.body
}

m4uint32_t ClJSExeCommand::GetInternalStatus ()
{
  //## begin ClJSExeCommand::GetInternalStatus%938539043.body preserve=yes

	m_poMutexStatus.Lock();

	m4uint32_t aux=m_poInternalStatus;

	m_poMutexStatus.Unlock();

	return aux;

  //## end ClJSExeCommand::GetInternalStatus%938539043.body
}

m4return_t ClJSExeCommand::SetTimeOut (m4uint32_t ai_uiTimeOut)
{
  //## begin ClJSExeCommand::SetTimeOut%940319367.body preserve=yes

	//Modificamos el TimeOut
	m_poMutexStatus.Lock();

	if (ai_uiTimeOut != 0)
	{
		m_uiTimeOut=ai_uiTimeOut;
	}
	else
	{
		m_uiTimeOut=ClStatus::InfiniteTimeOut;
	}

	m_poMutexStatus.Unlock();

	return M4_SUCCESS;

  //## end ClJSExeCommand::SetTimeOut%940319367.body
}

m4return_t ClJSExeCommand::Reset (m4bool_t &ao_bIsCancel, m4bool_t &ao_bIsTimeOut)
{
  //## begin ClJSExeCommand::Reset%940319368.body preserve=yes

		//Obtenemos primero si la máquina virtual ha finalizado
	 //por TIME OUT
		
		//Bloqueamos la ejecución para que nadie 
		//puede modificar el TimeOut o establecerlo de nuevo
		//por seguridad
		m_poMutexStatus.Lock();

			m4return_t ret=M4_SUCCESS;

		ret=m_poChannelHandler->ResetMVC(ao_bIsCancel,ao_bIsTimeOut);

		//Desbloquemos el mutex que guardaba la sección crítica
		m_poMutexStatus.Unlock();


	return ret;

  //## end ClJSExeCommand::Reset%940319368.body
}

m4return_t ClJSExeCommand::LoadAllMeta4Objects (m4uint32_t ai_uiIdBox, m4uint32_t ai_uiIdJob, m4uint32_t ai_uiIdBoxExe)
{
  //## begin ClJSExeCommand::LoadAllMeta4Objects%941010640.body preserve=yes

	m4VariantType	arg, entryarg1, entryarg2, arg_filter;
	m4VariantType *	poArg=NULL;
	m4return_t		ret;
	m4VariantType	vIdBox, vIdJob;

	//Lista a través de la cual pasamos los valores
	//para filtrar el canal
	ClListParameterVariant oListParameter;
	ClListParameterVariant oListParameterOut;
	
	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Loading the box Meta4Object");
	}

	SETCODEF(M4_DEBUG_GET_NODE, DEBUGINFOLOG , "#*s1*#N4_JS_SCHED_TASKS#C4_JS_SCHED_TASKS#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);

	//Cargamos el canal de definiciones
	ret = m_poChannelHandler->GetNode("N4_JS_SCHED_TASKS", m_poAccessGJDef, m_poNodeJGDef);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_LOAD_CHANNEL_EXEC_ERROR, ERRORLOG, "#*s1*#C4_JS_SCHED_TASKS#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);
		return M4_ERROR;
	}

	// Nos colocamos en el primer registro.
	m_poNodeJGDef->RecordSet.Current.Begin();

	// Obtenemos el nodo de la definición de las subtareas que es un nodo hijo
	// del nodo de la definición de las tareas (N4_JS_SCHED_TASKS).
	ret = m_poChannelHandler->GetNode("N4_JS_SUBTASKS", m_poAccessGJDef, m_poNodeJDef);
	
	if (ret != M4_SUCCESS)
	{
		SETCODEF(M4_LOAD_CHANNEL_EXEC_ERROR, ERRORLOG, "#*s1*#C4_JS_SCHED_TASKS#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);
		return M4_ERROR;
	}

	// Nos colocamos en el primer registro.
	m_poNodeJDef->RecordSet.Current.Begin();
	
	// Por último, se carga la tarea y la subtarea que se va a ejecutar
	// mediante un método del nodo de las tareas (N4_JS_SCHED_TASKS).
	// Este método se denomina LOAD_SCHED_TASK.
	// Filtramos por IdBox.
	vIdBox.Type = M4CL_CPP_TYPE_NUMBER;
	vIdBox.Data.DoubleData = ai_uiIdBox;
	oListParameter.push_back(&vIdBox);

	// Filtramos por IdJob.
	vIdJob.Type = M4CL_CPP_TYPE_NUMBER;
	vIdJob.Data.DoubleData = ai_uiIdJob;
	oListParameter.push_back(&vIdJob);

	ret = m_poChannelHandler->ExecuteMethod("LOAD_SCHED_TASK", oListParameter, m_poNodeJGDef, m_poAccessGJDef, oListParameterOut);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "LOAD_SCHED_TASK", "C4_JS_SCHED_TASKS");
		return M4_ERROR;
	}
	
	// Recuperamos el valor de retorno
	ClListParameterVariant::iterator oIteratorOut;
	oIteratorOut = oListParameterOut.begin();

	// Comprobacion que tenemos algo en la lista.
	if (oIteratorOut != oListParameterOut.end()) 
	{
		poArg = *oIteratorOut;

		if (poArg->Data.DoubleData != 0)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "LOAD_SCHED_TASK", "C4_JS_SCHED_TASKS");
			delete poArg;

			return M4_ERROR;
		}

		// Nos tenemos que encargar de borrar el puntero
		// al objeto variant que nos devuelve en la lista
		delete poArg;
		poArg = NULL;
	}
	
	//Volvemos a borrar la lista para reutilizarla
	oListParameter.clear();
	oListParameterOut.clear();

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Loading the registry channel");
	}

	SETCODEF(M4_DEBUG_GET_NODE, DEBUGINFOLOG, "#*s1*#N4_JS_TASKS_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);

	//Obtenemos el nodo raiz del canal de  executions
	//que es el conjunto de job group execution
	ret = m_poChannelHandler->GetNode("N4_JS_TASK_EXE", m_poAccessExec, m_poNodeGExec);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_CREATE_NODE_EXEC,ERRORLOG,"#*s1*#N4_JS_TASKS_EXE#C4_JS_TASK_EXE#%d#%d#%d#",ai_uiIdJob,ai_uiIdBox,ai_uiIdBoxExe);
		return M4_ERROR;
	}
 
	// Nos situamos en el primer registro del nodo.
	m_poNodeGExec->RecordSet.Current.Begin();

	SETCODEF(M4_DEBUG_FILTER_META4OBJECT, DEBUGINFOLOG, "#*s1*#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);

	//Cargamos el canal de ejecuciones del job group
	//Para ello llamamos a un método LN4 que nos hace la 
	//carga filtrando por los valores que le demos en los 
	//parámetros del método

	//Le pasamos el primer parámetro
	//que es el identificador del trabajo
	arg_filter.Type=M4CL_CPP_TYPE_NUMBER;
	arg_filter.Data.DoubleData=ai_uiIdBox;

	oListParameter.push_back(&arg_filter);

    //Le pasamos un sengundo parámetro que es el
	//identificador de la ejecución de la tarea
	entryarg1.Type=M4CL_CPP_TYPE_NUMBER;
	entryarg1.Data.DoubleData=ai_uiIdBoxExe;

	oListParameter.push_back(&entryarg1);

	//Llamamos al método que hace la Load del canal
	SETCODEF(M4_DEBUG_LOAD_META4OBJECT, DEBUGINFOLOG, "#*s1*#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);

	ret = m_poChannelHandler->ExecuteMethod("LOAD_TASK_EXE", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "LOAD_TASK_EXE", "C4_JS_TASK_EXE");

		return M4_ERROR;
	}

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: Execute Loading the exec Meta4Object");
	}

	//Recuperamos el valor de retorno
	oIteratorOut=oListParameterOut.begin();

	//Comprobacion que tenemos algo en la lista ADD FJSM 199-11-25
	if (oIteratorOut != oListParameterOut.end()) 
	{
		poArg=*oIteratorOut;

		if (poArg->Data.DoubleData != 0)
		{
			SETCODEF(M4_LOAD_CHANNEL_EXEC_ERROR, ERRORLOG, "#*S1*#C4_JS_TASK_EXE#%d#%d#%d#", ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);
			return M4_ERROR;
		}

		//Nos tenemos que encargar de borrar el puntero
		//al objeto variant que nos devuelve en la lista
		delete poArg;
	}

	//ADD FJSM 1999-11-25 Borramos la lista
	oListParameter.clear();
	oListParameterOut.clear();

	//Cargamos el nodo de ejecuciones de tareas
	ret = m_poChannelHandler->GetNode("N4_JS_SUBTASK_EXE", m_poAccessExec, m_poNodeJExec);

	if (M4_SUCCESS  != ret)
	{
		SETCODEF(M4_ERROR_CREATE_NODE_EXEC, ERRORLOG,"#*s1*#N4_JS_SUBTASKS_EXE#C4_JS_TASK_EXE#%d#%d#%d#",ai_uiIdJob, ai_uiIdBox, ai_uiIdBoxExe);
		return M4_ERROR;
	}

	// Situarse al principio del nodo.
	m_poNodeJExec->RecordSet.Current.Begin();

	return M4_SUCCESS;

  //## end ClJSExeCommand::LoadAllMeta4Objects%941010640.body
}

m4return_t ClJSExeCommand::ReleaseAllMeta4Objects ()
{
  //## begin ClJSExeCommand::ReleaseAllMeta4Objects%941010641.body preserve=yes

	// Bug 0167072: Destruir el M4Object C4_JS_EXE_JIT.
	// Bug 164043 Sólo si ha sido construido
	m4return_t iRetJit = M4_SUCCESS;

	if( m_poChannelJIT != NULL && m_poAccessJIT != NULL )
	{
		iRetJit = m_poChannelHandler->DestroyChannel(m_poAccessJIT, m_poChannelJIT);
	}

	m4return_t ret1 = m_poChannelHandler->ReleaseChannel(m_poChannelExec);

	m4return_t ret2 = m_poChannelHandler->ReleaseChannel(m_poChannelGJDef);
	
	if (M4_SUCCESS != iRetJit || M4_SUCCESS != ret1 || M4_SUCCESS != ret2)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClJSExeCommand::ReleaseAllMeta4Objects%941010641.body
}

m4return_t ClJSExeCommand::ExecuteJob (ClStatsJSEXECUTE *ai_poStats, m4uint32_t ai_idJobDEF, m4uint32_t ai_idBoxExe, m4uint32_t ai_idJobGroupDEF)
{
  //## begin ClJSExeCommand::ExecuteJob%900945535.body preserve=yes

	m4VariantType arg,entryarg1,entryarg2;
	m4VariantType apdu;
	m4VariantType arglog;
	

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: ExecuteJob Preparing to execute");
	}

	//Antes de comenzar a añadir parámetros 
	//tenemos que llamar al método que inicializa
	//el nodo de parámetros del canal de JIT
	m4return_t ret=m_poParametersHandler->SetExecutionAccess(m_poAccessJIT);

	if (M4_ERROR == ret)
	{
		SETCODEF(M4_ERROR_CLIENT_STRUCTURES, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		return M4_ERROR;
	}

	//Creamos los registros para los parámetros de salida
	ClNode * poNodePJIT;
	ret = m_poChannelHandler->GetNode("N4_JS_EXE_JIT", m_poAccessJIT, poNodePJIT);
	if (NULL == poNodePJIT)
	{
		SETCODEF(M4_ERROR_CREATE_NODE_EXECUTEJOB, ERRORLOG, "#*s1*#N4_JS_EXE_JIT#C4_JS_EXE_JIT#%d#%d#%d#",ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		SETCODEF(M4_ERROR_CLIENT_STRUCTURES, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		return M4_ERROR;
	}

	ret = AddParams(poNodePJIT, ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);

	if (M4_ERROR == ret)
	{
		SETCODEF(M4_ERROR_CLIENT_STRUCTURES, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		return M4_ERROR;
	}

	//Recuperamos el número de paraámetros que hemos 
	//alamacenado en la pila de MVC
	m4uint32_t num_parameter=m_poParametersHandler->GetNumberParameters();

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: ExecuteJob Code after set parameters");
	}
	
	//FIN DEL CODIGO DE PASO DE PARAMETROS 

	//Aqui definimos una variable que actua como un flag
	//indicando posteriormente si la ejecución ha resultado un éxito o no 
	m4bool_t is_good_exec=M4_TRUE;

	//Ponemos el cronometro a contar para saber
	//lo que tarda la ejecución de la tarea
	M4ClCrono cronometro(M4_TRUE);
	cronometro.Start();

	//Ahora nos vamos a situar en el nodo que tiene 
	//método de just in time y los parametros

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: ExecuteJob Calling RegisterDescriptorInMap");
	}

	//Activamos el estado de ejecución interna del ejecutor
	//a ejecución del JIT
	SetInternalStatus(M4_EXECUTING_JIT);

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: ExecuteJob Executing JIT");
	}

	//Estos son las listas en donde van los parámetros de entrada y salida 
	//del método en este caso son vacías
	ClListParameterVariant oListParameter,oParameterOut;

//	oListParameter.clear();

//	oParameterOut.clear();

	// Se identifica a la tarea dentro del canal EXE_JIT.
	m4VariantType vBoxId;
	vBoxId.Type = M4CL_CPP_TYPE_NUMBER;
	vBoxId.Data.DoubleData = ai_idJobGroupDEF;
	ret = m_poChannelHandler->SetData(poNodePJIT, "PR_SCHED_TASK_ID", vBoxId);
	if (ret != M4_SUCCESS)
	{
		SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#PR_SCHED_TASK_ID#N4_JS_EXE_JIT#C4_JS_EXE_JIT#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		return M4_ERROR;
	}

	// Bug 0287809. Identificar la ejecucion.
	m4VariantType vBoxExeId;
	vBoxExeId.Type = M4CL_CPP_TYPE_NUMBER;
	vBoxExeId.Data.DoubleData = ai_idBoxExe;
	ret = m_poChannelHandler->SetData(poNodePJIT, "PR_TASK_EXE_ID", vBoxExeId);
	if (ret != M4_SUCCESS)
	{
		SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#PR_TASK_EXE_ID#N4_JS_EXE_JIT#C4_JS_EXE_JIT#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		return M4_ERROR;
	}

	// Activamos las estadísticas si es necesario
	m4char_t	acStatisticsFile[ M4CH_MAX_FILE + 1 ] ;

	*acStatisticsFile = '\0' ;
	ret = _ActivateStatistics( acStatisticsFile ) ;

	if( ret != M4_SUCCESS )
	{
		*acStatisticsFile = '\0' ;
		SETCODE( M4_JS_STT_NO_ACTIVATE, ERRORLOG ) ;
	}


	//Ejecutamos el código en just in time
	m4return_t retJIT=m_poChannelHandler->ExecuteCodeLN4(poNodePJIT,m_poAccessJIT,oParameterOut,num_parameter);

	
	// Desactivamos siempre las estadísticas
	m_poChannelJIT->SetStatisticsLevel( M4_VM_STATISTICS_DISABLED ) ;


	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: ExecuteJob After Executing JIT");
	}

	//Activamos el estado de finalizado la ejecución del JIT
	//en la variable interna de estado del ejecutor
	SetInternalStatus(M4_END_EXECUTING_JIT);

	//Limpiamos la máquina virtual de flag de cancelación
	//y de TIME OUT
	m4bool_t IsCancel,IsTimeOut;
	Reset(IsCancel,IsTimeOut);

	//Paramos el cronòmetro y recogemos el tiempo que ha tardado en ejecutarse el JIT
	m4double_t tiempo=cronometro.Stop();

	//Almacenamos el código de retorno de la ejecución 
	m4VariantType *poutarg;
	m4VariantType outarg;

	ClListParameterVariant::iterator oIteratorParameter;
	oIteratorParameter=oParameterOut.begin();

	// checks for funcional errors
	// fix bug 0090103
	if (oIteratorParameter!=oParameterOut.end()) {

		poutarg=*oIteratorParameter;

		if (poutarg->Type == M4CL_CPP_TYPE_NUMBER && ((m4return_t)poutarg->Data.DoubleData) == M4_ERROR) {
			SETCODEF(M4_EXECUTE_METHOD_FUNC_ERROR, ERRORLOG, "#*S1*#%s#", "ClJSExeCommand::ExecuteJob");
			is_good_exec=M4_FALSE;
		}

		//Borramos el puntero al variant que nos devuelve el código 
		//de retorno del JIT
		if (poutarg)
			delete poutarg;
	}

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: ExecuteJob Saves the execution return code");
	}
	
	
	//Recargamos el canal por si durante la ejecución del job se han generado trazas
	ret = ReLoadChannel(ai_idJobGroupDEF, ai_idJobDEF, ai_idBoxExe);

	if (M4_ERROR == ret)
	{
		SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		return M4_ERROR;
	}	


	if( *acStatisticsFile != '\0' )
	{
		// Asignamos aquí las estadísticas para estar posicionados. El retorno da igual
		ret = m_poChannelHandler->SetData( m_poNodeJExec, "STATISTICS_LOG", acStatisticsFile ) ;

		if( ret != M4_SUCCESS )
		{
			remove( acStatisticsFile ) ;
			SETCODE( M4_JS_STT_NO_SAVE, ERRORLOG ) ;
		}
		else
		{
			// Se deja que el ciclo de vida lo controle el filemanager
			ClFileManager::Instance()->SetFileManagerOwnerOfTheFile( acStatisticsFile, M4_TRUE ) ;
		}
	}


	//Independientemente de como finalizamos tenemos que recuperar
	//todos los recursos que se hayan creado hasta el momemto
	//para que no queden ficheros perdidos que no se puedan borrar
	
	// Necesitamos el identificador de la ejecución de la tarea
	ret = m_poChannelHandler->GetData(m_poNodeJExec, "ID_SUBTASK_EXE", arg, NULL);
	m4uint32_t uiIdJobExecution=(m4uint32_t)arg.Data.DoubleData;

	ret = RecoverExternalResource(ai_idJobGroupDEF, ai_idJobDEF, ai_idBoxExe, uiIdJobExecution);
	
	// Si se ha retornado el código de cancelación o si se ha 
	// retornado un código de error pero la máquina virtual 
	// tiene el flag a cancelación modificamos el estado del box
	// a abortado total.
	// Si se ha abortado la ejecución de la tarea, isCancel = 1 e
	// isTimeOut = 0.
	// Si ha habido un timeout, isCancel = 1 e isTimeOut = 1.
	if (M4_TRUE == IsCancel || M4_TRUE == IsTimeOut)
	{
		// Almacenamos resultados sobre la ejecución del job.
		// Primero informamos sobre que la ejecución ha sido abortada.
		if (M4_TRUE == IsTimeOut)
		{
			SETCODEF(M4_CLIENT_TIMEOUT_JOB, ERRORLOG, "#*S1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		}
		else
		{
			SETCODEF(M4_CLIENT_ABORTED_JOB, ERRORLOG, "#*S1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);			
		}

		// Almacenamos lo que haya almacenado en el resultado del JIT
		ret = m_poChannelHandler->GetData(poNodePJIT, "JIT_RESULT", arg, NULL);
		if (M4_ERROR == ret)
		{
			SETCODEF(M4_ERROR_GET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#JIT_RESULT#N4_JS_EXE_JIT#C4_JS_EXE_JIT#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
			SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
			
			return M4_ERROR;
		}

		if (m_poDumpInfo != NULL)
		{
			m_poDumpInfo->SetCurrentStage("JS Exec Commands: ExecuteJob Persist in error");
		}

		// Modificamos el estado a interno según haya sido 
		// interrupción por cancelación o por TIMEOUT.
		if (M4_TRUE == IsTimeOut)
		{
			SetInternalStatus(M4_TASK_TIMEOUT);

			m4char_t strID[50];
			M4ClString strTextEvent = m_stServiceName;
			sprintf(strID, "#%d#%d#", ai_idJobDEF, ai_idBoxExe);
			strTextEvent += strID;
			M4PublishEvent("65542",strTextEvent);
		}
		else
		{
			SetInternalStatus(M4_TASK_ABORTED);
		}

		return M4_ABORT_TASK;
	}

	if (M4_SUCCESS != retJIT)
	{
		SETCODEF(M4_ERROR_EXECUTE_METHODLN4_EXECUTEJOB, ERRORLOG, "#*s1*#EXE_JIT_WITH_PARAMS#C4_JS_EXE_JIT#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
	
		is_good_exec=M4_FALSE;
	}

	//Recuperamos los parametros de salida
	ret=RecoverParams();

	if (M4_ERROR == ret)
	{
		SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);

		return M4_ERROR;
	}

	
	//Almacenamos el código de retorno que será el que 
	//nos indique la MVC como valor retornado en la llamada
	//al método C++ que ejecuta el JIT
	arg.Type=M4CL_CPP_TYPE_NUMBER;

	if (is_good_exec==M4_TRUE)
	{
		//Aumentamos en uno el número de jobs
		//completados con éxito almacenados en las estadísticas
		if (ai_poStats != NULL)
		{
			ai_poStats->AddJobsSuccess(1);
		}
		arg.Data.DoubleData=0;
	}
	else
	{
		arg.Data.DoubleData=-1;

		//Aumentamos en uno el número de jobs fracasados
		if (ai_poStats != NULL)
		{		
			ai_poStats->AddJobsError(1);
		}
	}

	ret = m_poChannelHandler->SetData(m_poNodeJExec, "VM_EXIT_FLAG", arg);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#VM_EXIT_FLAG#N4_SUBTASK_EXEC#C4_JS_TASK_EXE#%d#%d#%d#", ai_idJobDEF, ai_idJobGroupDEF, ai_idBoxExe);
		SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);

		return M4_ERROR;
	}

	//inicio del código de grabar la fecha 
	//de finalizacion del job group exec y del job
	M4ClTimeStamp	date_ended ;
	m4double_t date_ended_canal;

    date_ended.now();
	date_ended.get(date_ended_canal);

	arg.Type=M4CL_CPP_TYPE_DATE;
	arg.Data.DoubleData=date_ended_canal;

	// Se llama al método SET_END_DATE que se encarga de axtualizar la fecha de inicio
	// de la ejecución de la tarea y de la subtarea con local_id = 0.
	oListParameter.clear();
	oParameterOut.clear();

	oListParameter.push_back(&arg);
	ret = m_poChannelHandler->ExecuteMethod("SET_END_DATE", oListParameter, m_poNodeJExec, m_poAccessExec, oParameterOut);

	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_SET_ITEM_EXECUTEJOB, ERRORLOG, "#*s1*#END_DATETIME#N4_JS_TASK_EXE#C4_JS_TASK_EXE#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);
		SETCODEF(M4_ERROR_CLIENT_RECOVER_EXECUTION, ERRORLOG, "#*s1*#%d#%d#%d#", ai_idJobDEF, ai_idBoxExe, ai_idJobGroupDEF);

		return M4_ERROR;
	}
	
	// Obtenemos de la lista de salida el puntero a variant
	// en el que almacenamos el valor de  retorno del método
	// LN4 que estamos invocando.
	oIteratorParameter = oParameterOut.begin();
	if (oIteratorParameter != oParameterOut.end())
	{
		m4VariantType * poReturnArg = *oIteratorParameter;

		if (poReturnArg->Data.DoubleData != 0)
		{
			delete poReturnArg;
			return M4_ERROR;
		}
		delete  poReturnArg;
	}

	if (m_poDumpInfo != NULL)
	{
		m_poDumpInfo->SetCurrentStage("JS Exec Commands: ExecuteJob Terminated");
	}

	if (is_good_exec==M4_TRUE)
	{
		return M4_SUCCESS ;
	}
	else
	{
		return M4_WARNING;
	}

	
  //## end ClJSExeCommand::ExecuteJob%900945535.body
}

m4return_t ClJSExeCommand::ChangeSecurity ()
{
  //## begin ClJSExeCommand::ChangeSecurity%941096542.body preserve=yes
		
	m4VariantType credential;

	//Obtenemos la credencial
	m4return_t ret = m_poChannelHandler->GetData(m_poNodeJDef, "PR_CONTEXT", credential, NULL);
	
	if (M4_SUCCESS != ret)
	{
		return M4_ERROR;
	}

	ret=m_poSecurity->ChangeSecurity(credential.Data.PointerChar);

	if (M4_SUCCESS != ret)
	{
		return ret;
	}

	return M4_SUCCESS;

  //## end ClJSExeCommand::ChangeSecurity%941096542.body
}

void ClJSExeCommand::SetParameterHandler (ClJSParametersHandlers *ai_poParmeterHandler)
{
  //## begin ClJSExeCommand::SetParameterHandler%941717920.body preserve=yes

	m_poParametersHandler=ai_poParmeterHandler;

  //## end ClJSExeCommand::SetParameterHandler%941717920.body
}

m4return_t ClJSExeCommand::SetIdentification (const M4ClString &ai_stServerName, const M4ClString &ai_stServiceName)
{
  //## begin ClJSExeCommand::SetIdentification%942839182.body preserve=yes

	m_stServerName=ai_stServerName;
	m_stServiceName=ai_stServiceName;

	return M4_SUCCESS;

  //## end ClJSExeCommand::SetIdentification%942839182.body
}

m4uint32_t ClJSExeCommand::GetUsedMemory ()
{
  //## begin ClJSExeCommand::GetUsedMemory%951331610.body preserve=yes
	m4uint32_t iRet;
	m_poMutexStatus.Lock();
	iRet = m_poChannelHandler->GetUsedMemory();
	m_poMutexStatus.Unlock();
	return iRet;
  //## end ClJSExeCommand::GetUsedMemory%951331610.body
}

m4return_t ClJSExeCommand::RecoverCompoundJobParams (m4uint32_t ai_bInParamValue, m4uint32_t ai_bOutParamValue)
{
  //## begin ClJSExeCommand::RecoverCompoundJobParams%958997021.body preserve=yes

	m4return_t	ret;

	// Se invoca al manejador de parámetros para que 
	// realice la propagación de los parámetros de la
	// tarea compuesta (a partir de los parámetros de las
	// subtareas) y grabe en base de datos.
	ret = m_poParametersHandler->RecoverCompoundJobParams(ai_bInParamValue, ai_bOutParamValue);
	
	return ret;

  //## end ClJSExeCommand::RecoverCompoundJobParams%958997021.body
}

// Additional Declarations
  //## begin ClJSExeCommand%34FE920503A3.declarations preserve=yes
  //## end ClJSExeCommand%34FE920503A3.declarations

//## begin module%34FFEB0301FB.epilog preserve=yes
//## end module%34FFEB0301FB.epilog


/*
Función que activa las estadísticas si es posible.
Para ello consulta el item STATISTICS_LEVEL
Como etiqueta pone el id del trabajo, dado por el item ID_TASK
Si se han podido activar las estadísticas devuelve éxito y
como parámetro de salida el fichero de estadísticas
*/
m4return_t ClJSExeCommand::_ActivateStatistics( m4pchar_t ao_pcStatisticsFile ) const
{

	m4return_t		iResult = M4_SUCCESS ;
	m4uint16_t		iStatisticsLevel = M4_VM_STATISTICS_DISABLED ;
	m4VariantType	vStatisticsLevel ;
	m4VariantType	vStatisticsDefault ;
	m4VariantType	vStatisticsTaskId ;
	m4pcchar_t		pccDirectory = NULL ;
	ClVMStatistics	*poStatistics = NULL ;
	ClVMBaseEnv		*poEnvironment = NULL ;


	*ao_pcStatisticsFile = '\0' ;

	vStatisticsLevel.SetDouble( 0 ) ;
	vStatisticsDefault.SetDouble( 0 ) ;
	vStatisticsTaskId.SetNull() ;


	iResult = m_poChannelHandler->GetData( m_poNodeJGDef, "STATISTICS_LEVEL", vStatisticsLevel, &vStatisticsDefault ) ;

	if( iResult != M4_SUCCESS || vStatisticsLevel.Type != M4CL_CPP_TYPE_NUMBER )
	{
		return( M4_ERROR ) ;
	}
	
	iStatisticsLevel = m4uint16_t( vStatisticsLevel.Data.DoubleData ) & M4_VM_STATISTICS_ENABLED ;

	if( iStatisticsLevel == M4_VM_STATISTICS_DISABLED )
	{
		return( M4_SUCCESS ) ;
	}

	if( m_poChannelJIT == NULL )
	{
		return( M4_ERROR ) ;
	}


	// Se activan las estadísticas
	iResult = m_poChannelJIT->SetStatisticsLevel( iStatisticsLevel ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Bug 0316990
	iResult = m_poChannelJIT->GetpChannelManager()->GetSttWrapper()->SetStatisticsLevel( iStatisticsLevel, m_poChannelJIT->GetpChannelManager() ) ;

	if( iResult != M4_SUCCESS )
	{
		m_poChannelJIT->SetStatisticsLevel( M4_VM_STATISTICS_DISABLED ) ;
		return( M4_ERROR ) ;
	}

	poStatistics = m_poChannelJIT->GetpChannelManager()->GetSttWrapper()->GetStatistics() ;

	if( poStatistics == NULL )
	{
		m_poChannelJIT->SetStatisticsLevel( M4_VM_STATISTICS_DISABLED ) ;
		return( M4_ERROR ) ;
	}


	// Se manda a fichero
	poEnvironment = m_poChannelJIT->GetpChannelManager()->GetEnvironment() ;

	if( poEnvironment != NULL )
	{
		pccDirectory = poEnvironment->GetTempDirectory() ;
	}

	if( pccDirectory != NULL )
	{
		sprintf( ao_pcStatisticsFile, "%s%s~js%d.stt", pccDirectory, M4_BACK_SLASH, M4Thread::GetCurrentIdThread() ) ;
	}
	else
	{
		sprintf( ao_pcStatisticsFile, "~js%d.stt", M4Thread::GetCurrentIdThread() ) ;
	}

	iResult = poStatistics->sttSetIntermediateFile( ao_pcStatisticsFile ) ;

	if( iResult != M4_SUCCESS )
	{
		*ao_pcStatisticsFile = '\0' ;
		m_poChannelJIT->SetStatisticsLevel( M4_VM_STATISTICS_DISABLED ) ;
		return( M4_ERROR ) ;
	}


	// Se pone la etiqueta
	vStatisticsDefault.Type = M4CL_CPP_TYPE_STRING_VAR ;
	vStatisticsDefault.Data.PointerChar = "JOB_SCHEDULER" ;

	iResult = m_poChannelHandler->GetData( m_poNodeJGDef, "ID_TASK", vStatisticsTaskId, &vStatisticsDefault ) ;
	
	if( iResult != M4_SUCCESS || vStatisticsTaskId.Type != M4CL_CPP_TYPE_STRING_VAR || vStatisticsTaskId.Data.PointerChar == NULL )
	{
		vStatisticsTaskId.Data.PointerChar = vStatisticsDefault.Data.PointerChar ;
	}

	iResult = m_poChannelJIT->SetStatisticsLabel( vStatisticsTaskId.Data.PointerChar ) ;


	return( M4_SUCCESS ) ;
}


m4return_t ClJSExeCommand::_BuildJITChannel()
{
	m4return_t	iRet = M4_SUCCESS;
	
	// Construir el M4O C4_JS_EXE_JIT.
	iRet = m_poChannelHandler->BuildChannel("C4_JS_EXE_JIT", m_poChannelJIT, m_poAccessJIT);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClJSExeCommand::_GetEmailRecipients()
{
	m4return_t							iRet = M4_SUCCESS;
	ClChannel *							poChannel = NULL;
	ClAccess *							poAccess = NULL;
	ClNode *							poNode = NULL;
	m4VariantType						vAutoNotifyExec;
	m4VariantType						vUsersToNotifyExec;
	m4VariantType						vDefaultUsersToNotifyExec;
	m4VariantType						vUserExecutor;
	m4VariantType						vUserList;
	m4VariantType						vMailRecipents;
	m4VariantType						vRes;
	string								sUserList;

	// Inicializaciones.
	m_sEmailRecipients = "";

	// Construir M4O de notificaciones.
	iRet = m_poChannelHandler->BuildChannel("C4_JS_USER_NOTIFICATION", poChannel, poAccess);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	poNode = &(poAccess->Node["N4_JS_USER_NOTIFICATION"]);
	if (poNode == NULL)
	{
		m_poChannelHandler->DestroyChannel(poAccess, poChannel);

		SETCODEF(M4_ERROR_CREATE_NODE, ERRORLOG, "#*S1*#%s#C4_JS_USER_NOTIFICATION#", "N4_JS_USER_NOTIFICATION");
		return M4_ERROR;
	}

	// Obtener la información de notificación de la planificación.
	iRet = m_poChannelHandler->GetData(m_poNodeGExec, "AUTO_NOTIFY_EXEC", vAutoNotifyExec, NULL);
	if (iRet == M4_ERROR)
	{
		m_poChannelHandler->DestroyChannel(poAccess, poChannel);
		SETCODEF(M4_GET_VALUE_ERROR, ERRORLOG, "#*s1*#AUTO_NOTIFY_EXEC#N4_JS_TASK_EXE#C4_JS_TASK_EXE#");
		return M4_ERROR;
	}
	
	// Usuario que ejecuta la planificación.
	iRet = m_poChannelHandler->GetData(m_poNodeGExec, "USER_EXECUTOR", vUserExecutor, NULL);
	if (iRet == M4_ERROR || vUserExecutor.Type != M4CL_CPP_TYPE_STRING_VAR || vUserExecutor.Data.PointerChar == NULL )
	{
		m_poChannelHandler->DestroyChannel(poAccess, poChannel);
		SETCODEF(M4_GET_VALUE_ERROR, ERRORLOG, "#*s1*#USER_EXECUTOR#N4_JS_TASK_EXE#C4_JS_TASK_EXE#");
		return M4_ERROR;
	}

	// Autonotificaciones.
	if (vAutoNotifyExec.Data.DoubleData == 1)
	{
		sUserList = vUserExecutor.Data.PointerChar;
	}

	vDefaultUsersToNotifyExec.SetNull();
	iRet = m_poChannelHandler->GetData(m_poNodeGExec, "MAILS_TO_NOTIFY_EXEC", vUsersToNotifyExec, &vDefaultUsersToNotifyExec);
	if (iRet == M4_ERROR)
	{
		m_poChannelHandler->DestroyChannel(poAccess, poChannel);

		SETCODEF(M4_GET_VALUE_ERROR, ERRORLOG, "#*s1*#MAILS_TO_NOTIFY_EXEC#N4_JS_TASK_EXE#C4_JS_TASK_EXE#");
		return M4_ERROR;
	}
	
	if (vUsersToNotifyExec.Data.PointerChar != NULL)
	{
		if (sUserList.length() != 0)
		{
			sUserList.append(";");
		}

		sUserList.append(vUsersToNotifyExec.Data.PointerChar);
	}

	vUserList.Type = M4CL_CPP_TYPE_STRING_VAR;
	vUserList.Data.PointerChar = (m4pchar_t) sUserList.c_str();
	if (poNode->RecordSet.Current.Item["GET_EMAIL_RECIPIENTS"].StackPush(vUserExecutor) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item["GET_EMAIL_RECIPIENTS"].StackPush(vUserList) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item["GET_EMAIL_RECIPIENTS"].StackPush(vMailRecipents) != M4_SUCCESS)
	{
		m_poChannelHandler->DestroyChannel(poAccess, poChannel);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item["GET_EMAIL_RECIPIENTS"].Call() != M4_SUCCESS)
	{
		m_poChannelHandler->DestroyChannel(poAccess, poChannel);
		return M4_ERROR;
	}
	
	// Obtener resultado.
	if (poNode->RecordSet.Current.Item["GET_EMAIL_RECIPIENTS"].StackPop(vMailRecipents) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item["GET_EMAIL_RECIPIENTS"].StackPop(vRes) != M4_SUCCESS)
	{
		m_poChannelHandler->DestroyChannel(poAccess, poChannel);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		m_poChannelHandler->DestroyChannel(poAccess, poChannel);
		return M4_ERROR;
	}

	if ((vMailRecipents.Type != M4CL_CPP_TYPE_NULL) && (vMailRecipents.Data.PointerChar != NULL))
	{
		m_sEmailRecipients = vMailRecipents.Data.PointerChar;
	}

	// Cerrar el M4O de notificaciones.
	m_poChannelHandler->DestroyChannel(poAccess, poChannel);
	
	return M4_SUCCESS;
}

m4return_t ClJSExeCommand::_SendInitExecutionMail(m4uint32_t ai_idJobGroupDEF, m4uint32_t ai_idBoxExe)
{
	m4return_t							iRet = M4_SUCCESS;
	ClListParameterVariant				oListParameter;
	ClListParameterVariant				oListParameterOut;
	ClListParameterVariant::iterator	oIteratorOut;
	m4VariantType						vEmailRecipients;
	m4VariantType *						poArg = NULL;
	
	// Comprobar que tenemos destinatarios a quien mandar el correo.
	if (m_sEmailRecipients.length() == 0)
	{
		SETCODEF(M4_ERROR_GET_EMAIL_RECIPIENTS, DEBUGINFOLOG, "#*S1*#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
		return M4_ERROR;
	}	

	vEmailRecipients.Type = M4CL_CPP_TYPE_STRING_VAR;
	vEmailRecipients.Data.PointerChar = (m4pchar_t) m_sEmailRecipients.c_str();;
	oListParameter.push_back(&vEmailRecipients);
	iRet = m_poChannelHandler->ExecuteMethod("SEND_START_EXECUTION_MAIL", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "SEND_START_EXECUTION_MAIL", "C4_JS_TASK_EXE");
		return M4_ERROR;
	}

	// Recuperamos el valor de retorno
	oIteratorOut = oListParameterOut.begin();
	if (oIteratorOut != oListParameterOut.end()) 
	{
		poArg = *oIteratorOut;
		if (poArg->Data.DoubleData != 0)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "SEND_START_EXECUTION_MAIL", "C4_JS_TASK_EXE");
			delete poArg;
			return M4_ERROR;
		}
		delete poArg;
		poArg = NULL;
	}
	
	oListParameter.clear();
	oListParameterOut.clear();
	
	return M4_SUCCESS;
}

m4return_t ClJSExeCommand::_SendEndExecutionMail(m4uint32_t ai_idJobGroupDEF, m4uint32_t ai_idBoxExe)
{
	m4return_t							iRet = M4_SUCCESS;
	m4VariantType						vIsLastTask;
	m4VariantType						vEmailRecipients;
	m4uint32_t							iStatus = 0;
	ClListParameterVariant				oListParameter;
	ClListParameterVariant				oListParameterOut;
	ClListParameterVariant::iterator	oIteratorOut;
	m4VariantType *						poArg = NULL;

	// Comprobar que tenemos destinatarios a quien mandar el correo.
	if (m_sEmailRecipients.length() == 0)
	{
		SETCODEF(M4_ERROR_GET_EMAIL_RECIPIENTS, DEBUGINFOLOG, "#*S1*#%d#%d#", ai_idJobGroupDEF, ai_idBoxExe);
		return M4_ERROR;
	}	

	iRet = m_poChannelHandler->GetData(m_poNodeJDef, "PR_IS_LAST_TASK", vIsLastTask, NULL);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_GET_VALUE_ERROR, ERRORLOG, "#*s1*#PR_IS_LAST_TASK#N4_JS_TASK_EXE#C4_JS_TASK_EXE#");
		return M4_ERROR;
	}
	
	// Obtenemos el estado interno del ejecutor 
	iStatus = GetInternalStatus();
	
	// Mandamos el correo de ejecución finalizada en el caso en el que sea la úlitma subtarea de la ejecución
	// y también en el caso en el que no sea la última subtarea pero que se haya abortado o haya dado timeout.
	if (vIsLastTask.Data.DoubleData == 1 || iStatus == M4_TASK_ABORTED || iStatus == M4_TASK_TIMEOUT)
	{
		vEmailRecipients.Type = M4CL_CPP_TYPE_STRING_VAR;
		vEmailRecipients.Data.PointerChar = (m4pchar_t) m_sEmailRecipients.c_str();
		oListParameter.push_back(&vEmailRecipients);

		iRet = m_poChannelHandler->ExecuteMethod("SEND_END_EXECUTION_MAIL", oListParameter, m_poNodeGExec, m_poAccessExec, oListParameterOut);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "SEND_END_EXECUTION_MAIL", "C4_JS_TASK_EXE");
			return M4_ERROR;
		}
	}

	// Recuperamos el valor de retorno
	oIteratorOut = oListParameterOut.begin();
	if (oIteratorOut != oListParameterOut.end()) 
	{
		poArg = *oIteratorOut;
		if (poArg->Data.DoubleData != 0)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR, ERRORLOG, "#*S1*#%s#%s#", "SEND_END_EXECUTION_MAIL", "C4_JS_TASK_EXE");
			delete poArg;
			return M4_ERROR;
		}
		delete poArg;
		poArg = NULL;
	}
	
	oListParameter.clear();
	oListParameterOut.clear();

	return M4_SUCCESS;
}
	
m4uint32_t ClJSExeCommand::rowsQuotaProlog()
{
	m4uint32_t	iRowsQuotaLimit = 0;

	ClAccess*	poAccess = ((ClSecurity30*)m_poSecurity)->m_poAccessUSer;
	if (NULL != poAccess)
	{
		ClNode *poNode = &(poAccess->Node["ROOT_SESSION"]);
		if (NULL != poNode)
		{
			m4bool_t bRet = poNode->RecordSet.Current.Begin();
			if (M4_TRUE == bRet)
			{
				m4double_t*	pdRowsQuotaLimit = (m4double_t*)poNode->RecordSet.Item["TRANSACTION_ROWS_LIGHT_QUOTA"].Value.GetWithOutSec();
				if (NULL != pdRowsQuotaLimit)
				{
					iRowsQuotaLimit = (m4uint32_t)*pdRowsQuotaLimit;
				}
			}
		}
	}

	if (iRowsQuotaLimit > 0)
	{
		m_RowsQuotaCtrl.Reset(M4_TRUE, iRowsQuotaLimit);
		ClThreadObjects::Put(ClThreadObjects::ROWS_QUOTA_CONTROLLER, &m_RowsQuotaCtrl);
	}
	else
	{
		m_RowsQuotaCtrl.SwitchOff();
		ClThreadObjects::Put(ClThreadObjects::ROWS_QUOTA_CONTROLLER, NULL);
	}

	return iRowsQuotaLimit;
}

void ClJSExeCommand::rowsQuotaEpilog(m4uint32_t& ao_iNumRows, m4uint32_t& ao_iPeakRows)
{
	if (m_RowsQuotaCtrl.GetLimit() > 0)
	{
		ao_iNumRows = m_RowsQuotaCtrl.GetCount();
		ao_iPeakRows = m_RowsQuotaCtrl.GetPeak();
	}
	else
	{
		ao_iNumRows = 0;
		ao_iPeakRows = 0;
	}
}
