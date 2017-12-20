//## begin module%37D537B60026.cm preserve=no
//## end module%37D537B60026.cm

//## begin module%37D537B60026.cp preserve=no
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
//## end module%37D537B60026.cp

//## Module: jsAdministrator%37D537B60026; Package body
//## Subsystem: M4JSSubsystem::src%37BD2F0A02AE
//## Source file: D:\FuentesServidor\m4jssubsystem\src\jsadministrator.cpp

//## begin module%37D537B60026.additionalIncludes preserve=no
//## end module%37D537B60026.additionalIncludes

//## begin module%37D537B60026.includes preserve=yes
#include <cljssubsystem.hpp> //forward en hpp
#include <jssexecute.hpp> //forward en hpp
#include <jsjob.hpp> //forward en hpp
#include <jsrequest.hpp> //forward en hpp
#include <basiclog.hpp>
#include <m4jsres.hpp>
#include <m4eventhandlers.hpp>
//## end module%37D537B60026.includes

// syncro
#include <syncro.hpp>
// m4thread
#include <m4thread.hpp>
// m4date
#include <m4date.hpp>
// jsmapjobexe
#include <jsmapjobexe.hpp>
// jsAdministrator
#include <jsadministrator.hpp>


//## begin module%37D537B60026.declarations preserve=no
//## end module%37D537B60026.declarations

//## begin module%37D537B60026.additionalDeclarations preserve=yes
//Estados para el shutdown
/* Pasos para el shutdown
	Llega peticion de shutdown. Ponemos el estado a: JS_SHUTDOWN_PREPARING
	Llega confirmación de shutdown por el recuperador:
		- No hay ejecutores ocupados: Acabamos
		- Hay ejecutores ocupados:  estado a JS_SHUTDOWN_READY;
	Llega fin de trabajos:
		- Estado es JS_SHUTDOWN_READY entonces acabamos con todo
*/
#define JS_SHUTDOWN_NONE		0
#define JS_SHUTDOWN_PREPARING	1
#define JS_SHUTDOWN_READY		2

//Valores iniciales de frecuencia en segundos
#define JS_INITIAL_CANCELLATION_FREQUENCY		60
#define JS_INITIAL_EXPIRATION_FREQUENCY			60
#define JS_INITIAL_RES_EXPIRATION_FREQUENCY		86400	//1 dia
#define JS_INITIAL_RECOVER_FREQUENCY			60
#define JS_INITIAL_STRESS_RECOVER_FREQUENCY		5
//## end module%37D537B60026.additionalDeclarations


// Class ClJSAdministrator 


























ClJSAdministrator::ClJSAdministrator ()
  //## begin ClJSAdministrator::ClJSAdministrator%936690199.hasinit preserve=no
      : m_poJSSubsystem(NULL)
  //## end ClJSAdministrator::ClJSAdministrator%936690199.hasinit
  //## begin ClJSAdministrator::ClJSAdministrator%936690199.initialization preserve=yes
  //## end ClJSAdministrator::ClJSAdministrator%936690199.initialization
{
  //## begin ClJSAdministrator::ClJSAdministrator%936690199.body preserve=yes
	m_poInputQ = new ClJSQueue();
	m_poExecutorQ = new ClJSQueue();
	m_poCancelQ = new ClJSQueue();

	m_uiIntervalCancel = JS_INITIAL_CANCELLATION_FREQUENCY;
	m_uiIntervalExpired = JS_INITIAL_EXPIRATION_FREQUENCY;
	m_uiIntervalExpiredRes = JS_INITIAL_RES_EXPIRATION_FREQUENCY;
	m_uiIntervalRecover = JS_INITIAL_RECOVER_FREQUENCY;
	m_uiIntervalRecoverStress = JS_INITIAL_STRESS_RECOVER_FREQUENCY;

	//Ponemos todos a la misma hora
	m_oTimeNextCancel.now();
	m_oTimeNextExpired = m_oTimeNextCancel;
	m_oTimeNextRecover = m_oTimeNextCancel;
	m_oTimeNextExpiredRes = m_oTimeNextCancel;

	m_bInmediateRecovery = M4_FALSE;
	m_bRecoveringJobs = M4_FALSE;
	m_uiShutdownRequested = JS_SHUTDOWN_NONE;
	m_eMemoryStatus = JSMEMORY_GREEN;

	m_iPendingExecutors = 0;

	m_poTimeBeginJS = NULL;
	m_poTimeEndJS = NULL;

	m_bValidRecover = M4_FALSE;

	m_oMutex.Init();
  //## end ClJSAdministrator::ClJSAdministrator%936690199.body
}

ClJSAdministrator::ClJSAdministrator (ClJSSubsystem *ai_pJSSubsystem)
  //## begin ClJSAdministrator::ClJSAdministrator%937559777.hasinit preserve=no
      : m_poJSSubsystem(NULL)
  //## end ClJSAdministrator::ClJSAdministrator%937559777.hasinit
  //## begin ClJSAdministrator::ClJSAdministrator%937559777.initialization preserve=yes
  //## end ClJSAdministrator::ClJSAdministrator%937559777.initialization
{
  //## begin ClJSAdministrator::ClJSAdministrator%937559777.body preserve=yes
	m_poJSSubsystem = ai_pJSSubsystem;
	m_poInputQ = new ClJSQueue();
	m_poExecutorQ = new ClJSQueue();
	m_poCancelQ = new ClJSQueue();

	m_uiIntervalCancel = JS_INITIAL_CANCELLATION_FREQUENCY;
	m_uiIntervalExpired = JS_INITIAL_EXPIRATION_FREQUENCY;
	m_uiIntervalExpiredRes = JS_INITIAL_RES_EXPIRATION_FREQUENCY;
	m_uiIntervalRecover = JS_INITIAL_RECOVER_FREQUENCY;
	m_uiIntervalRecoverStress = JS_INITIAL_STRESS_RECOVER_FREQUENCY;

	//Ponemos todos a la misma hora
	m_oTimeNextCancel.now();
	m_oTimeNextExpired = m_oTimeNextCancel;
	m_oTimeNextRecover = m_oTimeNextCancel;
	m_oTimeNextExpiredRes = m_oTimeNextCancel;

	m_bInmediateRecovery = M4_FALSE;
	m_bRecoveringJobs = M4_FALSE;
	m_uiShutdownRequested = JS_SHUTDOWN_NONE;
	m_eMemoryStatus = JSMEMORY_GREEN;

	m_iPendingExecutors = 0;

	m_poTimeBeginJS = NULL;
	m_poTimeEndJS = NULL;

	m_bValidRecover = M4_FALSE;

	m_oMutex.Init();
  //## end ClJSAdministrator::ClJSAdministrator%937559777.body
}


ClJSAdministrator::~ClJSAdministrator ()
{
  //## begin ClJSAdministrator::~ClJSAdministrator%936966060.body preserve=yes
	if (m_poInputQ) {
		delete m_poInputQ;
		m_poInputQ = NULL;
	}
	if (m_poExecutorQ) {
		delete m_poExecutorQ;
		m_poExecutorQ = NULL;
	}
	if (m_poCancelQ) {
		delete m_poCancelQ;
		m_poCancelQ = NULL;
	}

  //## end ClJSAdministrator::~ClJSAdministrator%936966060.body
}



//## Other Operations (implementation)
void ClJSAdministrator::Run ()
{
  //## begin ClJSAdministrator::Run%936690198.body preserve=yes
	if (!m_poInputQ || !m_poExecutorQ || !m_poCancelQ)
		return;
	m4return_t ret;
	ClJSRequest *pOutputRequest = NULL;
	m4uint32_t uiTypeRequest;
	m4uint32_t uiNextTime; //en segundos
	
	//variables auxiliares.
	m4pvoid_t pParameter; //Se obtiene un void en los parámetros de JSRequest y luego hay que hacer un casting explicito
	m4uint32_t uiParameter; //Para obtener parámetros temporales
	ClExecutive *ExeTemp;
	ClJSJob *JobTemp;
	m4uint32_t eMemStatusTemp;

	//Le mando una peticion de limpieza total al Recuperador, por si 
	//nos habiamos caido, .... para la integridad de BD.
	pOutputRequest = new ClJSRequest(M4JS_REQ_INITIALIZE);
	//Esta request admite un parámetro:
		// 0: Primera Inicialización.
		// 1: Siguientes inicializaciones.
	pOutputRequest->AddParameter((m4uint32_t)0);
	m_poCancelQ->Push(pOutputRequest);

	M4ClString strTextEvent = m_poJSSubsystem->GetName();
	strTextEvent += "#";
	M4PublishEvent("65540",strTextEvent);

	while (!IsTerminate()) {
		
		//Cuanto tiempo falta para realizar la siguiente operación programada
		uiNextTime = ProcessClockTick();
		
		ret = m_poInputQ->Pop(m_poActualRequest, uiNextTime);
		if (ret == M4_ERROR)
			Terminate();
		else if (ret == M4_SUCCESS && m_poActualRequest)
		{
			//Hemos despertado sin Time-Out
			uiTypeRequest = m_poActualRequest->GetRequestType();
			switch (uiTypeRequest) {
				/////////////////////
				//Request provenientes del Subsistema o tarea del sistema
				/////////////////////
				case M4JS_REQ_SERVER_READY:
					m_bValidRecover = M4_TRUE;
					break;
				case M4JS_REQ_PET_NUM_EXECUTOR:
					if (m_poActualRequest->GetParameter(0,uiParameter) == M4_SUCCESS) {
						m_iPendingExecutors = uiParameter - m_oMapJobExe.GetNumExecutors();
						ProcessCommands();
					}
					break;
				case M4JS_REQ_MEMORY_STATE:
					if (m_poActualRequest->GetParameter(0,eMemStatusTemp) == M4_SUCCESS)
						ChangeMemoryStatus(	(eMemoryStatus) eMemStatusTemp);
					break;
				case M4JS_REQ_BEGIN_JS:
					if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS)
						//Inicio programado
						BeginJS((M4ClTimeStamp*)pParameter);
					else
						//Inicio inmediato
						BeginJS();
					break;
				case M4JS_REQ_END_JS:
					if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS)
						//Fin programado
						EndJS((M4ClTimeStamp*)pParameter);
					else
						//Fin inmediato
						EndJS();
					break;
				case M4JS_REQ_DEL_START_DATE:
					SetNextStartDate(NULL);
					break;
				case M4JS_REQ_DEL_STOP_DATE:
					SetNextStopDate(NULL);
					break;
				case M4JS_REQ_GP:
					if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS) {
						if (m_poActualRequest->GetParameter(1,uiParameter) == M4_SUCCESS) {
							if (uiParameter == 0) { //Ejecutor de trabajos
								ExeTemp = (ClExecutive*) pParameter;
								m_oMapJobExe.DeleteExecutor(ExeTemp);
							}
							if (uiParameter == 1) { //Ejecutor de cancelación
								//Si estabamos recuperando, borramos los waiting y cambiamos
								//la variable y ponemos un flag que se ha caido y cancelador
								//De forma que se reactivará el cancelador cuando no tengamos trabajos
								//en el JS, para mandarle una petición de limpieza.
								if (m_bRecoveringJobs == M4_TRUE) {
									m_oMapJobExe.DeleteWaitingJobs();
									m_bRecoveringJobs = M4_FALSE;
								}
								m_bValidRecover = M4_FALSE;
								/*Miramos si el cancelador no es válido y si es así
								le mandamos una request para que se inicialize y
								asi ya es válido*/
								InitializedRecover();
							}
						}
					}
					break;
				case M4JS_REQ_SHUTDOWN:
					//Petición de shutdown
					m4Trace (cout << "+++ Request Shutdown to JS +++" << endl);

					//Sola avisamos 1 vez al recuperador que vamos a cerrar
					if (JS_SHUTDOWN_NONE == m_uiShutdownRequested) {
						/*Si estamos recuperando, hasta que no recibamos el
						fin de nuevos jobs, para poder anular los nuevos jobs, 
						no mando la peticion de shutdown al recuperador*/

//En el caso de que este recuperando trabajos no hace correctamente el shutdown
//						if (M4_FALSE == m_bRecoveringJobs) 
						{
							pOutputRequest = new ClJSRequest(M4JS_REQ_SHUTDOWN);
							m_poCancelQ->Push(pOutputRequest);
						}
						m_uiShutdownRequested = JS_SHUTDOWN_PREPARING;
					}
					if ((M4_FALSE == m_bValidRecover) && (M4_FALSE == m_bRecoveringJobs)) {
						//Implica que quizás no tengamos recuperador, 
						//y como no estamos recuperando, intentamos acabar...
						m_uiShutdownRequested = JS_SHUTDOWN_READY;
						//Intentamos finalizar el JS
						TerminateJS();
					}
					break;

				/////////////////////
				//Requests provenientes del ejecutor
				/////////////////////
				case M4JS_REQ_BEGIN_JOB:
					if (BeginJob() == M4_ERROR) {
						BL_ERRORF(M4_JSADM_JOB, "BEGIN_JOB");
					}
					break;
				case M4JS_REQ_END_JOB:
					if (EndJob() == M4_ERROR) {
						BL_ERRORF(M4_JSADM_JOB, "END_JOB");
					}
					break;
				case M4JS_REQ_EXECUTOR_CREATED:
					if (ExecutorCreated() == M4_SUCCESS) {
					}
					break;
				case M4JS_REQ_EXECUTOR_DELETED:
					if (ExecutorDeleted() == M4_SUCCESS) {
					}
					break;

				/////////////////////
				//Requests provenientes del recuperador
				/////////////////////
				case M4JS_REQ_END_NEW_JOBS:
					EndNewJobs();
					break;
				case M4JS_REQ_NEW_JOB:
					NewJob();
					break;
				case M4JS_REQ_CONTROLED_INTERRUPT:
					if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS) {
						if (pParameter != NULL) {
							JobTemp = (ClJSJob*) pParameter;
							m_oMapJobExe.InterruptJob(ClJSJob::IRL_IRSU,JobTemp);
							delete JobTemp;
						}
					}
					break;
				case M4JS_REQ_UNCONTROLED_INTERRUPT:
					if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS) {
						if (pParameter == NULL) {
							//Interrupción de todos los jobs
							m_oMapJobExe.InterruptAllJobs(ClJSJob::IRL_INSU);
						}
						else {
							JobTemp = (ClJSJob*) pParameter;
							m_oMapJobExe.InterruptJob(ClJSJob::IRL_INSU, JobTemp);
							delete JobTemp;
						}
					}
					break;
				case M4JS_REQ_SHUTDOWN_OK:
					m_uiShutdownRequested = JS_SHUTDOWN_READY;
					//Intentamos finalizar el JS
					TerminateJS();
					break;	
				default:
					m4Trace(cout << "Request de JS en Administrador no reconocida" << endl);
					break;
			}
			if (m_poActualRequest != NULL) {
				delete m_poActualRequest;
				m_poActualRequest = NULL;
			}
		}
	}
	//Despierto a los que esten escuchando porque me voy
	m_poCancelQ->Lock();
	m_poCancelQ->ReleaseAll();
	m_poExecutorQ->Lock();
	m_poExecutorQ->ReleaseAll();
	m_poInputQ->Lock();
	m_poInputQ->ReleaseAll();

	strTextEvent = m_poJSSubsystem->GetName();
	strTextEvent += "#";
	M4PublishEvent("65541",strTextEvent);
  //## end ClJSAdministrator::Run%936690198.body
}

m4uint32_t ClJSAdministrator::ProcessClockTick ()
{
  //## begin ClJSAdministrator::ProcessClockTick%937559774.body preserve=yes

	////////////////
	//Este método nos sirve para saber que es lo que tenemos que hacer ahora mismo
	//y calcular que es lo próximo que hay que hacer
	//Devolvemos el tiempo en segundos que falta para la siguiente operacion.
	/////////////////
	ClSSImplementation::eStatus	JSStatus;
	m_poJSSubsystem->GetState(JSStatus);

	if ((JS_SHUTDOWN_NONE != m_uiShutdownRequested) || 
		(m_bValidRecover == M4_FALSE) ||
		(JSStatus == ClSSImplementation::SS_INACTIVE))
		//Estamos en shutdown, luego no procesamos peticiones
		//O estamos con un recuperador todavia no válido y por lo 
		//tanto no mandamos más peticiones
		//O estamos en un estado inoperativo, porque algun servicio
		//a hecho un shutdown
		return 0; //Esperamos infinito, hasta que llegue una petición.

	ClJSRequest *poRequest;
	M4ClTimeStamp oActualTime;
	oActualTime.now();
	m4uint32_t uiNumExecutors = 0;

	m4double_t dTimeTemp = 0; //Variable temporal

	//Tiempo que hay que esperar para la siguiente operacion en segundos
	m4double_t dNextTimeSeg = 0;

	m4double_t dTimeNow = 0; //Tiempo actual en segundos
	if (oActualTime.get(dTimeNow) != M4_SUCCESS)
		return 1;

	M4ClTimeStamp oNextTimeJS;

	//PARADA DEL SUBSISTEMA
	if (GetNextStopDate(oNextTimeJS) == M4_SUCCESS) {
		if (oNextTimeJS <= oActualTime) {
			SetNextStopDate(NULL);
			EndJS();
			//Refrescamos el status porque ha podido cambiar
			m_poJSSubsystem->GetState(JSStatus);
		}
		else {
			//Tenemos una hora para finalización
			oNextTimeJS.get(dTimeTemp);
			if ((dNextTimeSeg > (dTimeTemp-dTimeNow)) || (dNextTimeSeg==0))
				 dNextTimeSeg = dTimeTemp - dTimeNow;
		}
	}

	//ARRANQUE DEL SUBSISTEMA
	if (GetNextStartDate(oNextTimeJS) == M4_SUCCESS) {
		if (oNextTimeJS <= oActualTime) {
			SetNextStartDate(NULL);
			BeginJS();
			//Refrescamos el status porque ha podido cambiar
			m_poJSSubsystem->GetState(JSStatus);
		}
		else {
			//Tenemos una hora para finalización
			oNextTimeJS.get(dTimeTemp);
			if ((dNextTimeSeg > (dTimeTemp-dTimeNow)) || (dNextTimeSeg==0))
				 dNextTimeSeg = dTimeTemp - dTimeNow;
		}
	}

	//A partir de aqui utilizamos variables que pueden ser accedidas desde
	//otro thread (ej: el subsistema).
	m_oMutex.Lock();

	//PROCESOS QUE SOLO OCURREN CUANDO ESTOY ACTIVO O LA MEMORIA ES VERDE
	if ( (JSStatus == ClSSImplementation::SS_ACTIVE) 
		&& (m_eMemoryStatus == JSMEMORY_GREEN)) {
		
		//EXPIRACION DE TRABAJOS
		if (m_oTimeNextExpired <= oActualTime) {
			m_oTimeNextExpired.now();
			m_oTimeNextExpired.add_seconds(m_uiIntervalExpired);
			m4Trace(cout << "JOB SCHEDULER: Expirando Trabajos" << endl);
			poRequest = new ClJSRequest(M4JS_REQ_FIND_EXPIRED);
			m_poCancelQ->Push(poRequest);
		}
		m_oTimeNextExpired.get(dTimeTemp);
		if ((dNextTimeSeg > (dTimeTemp-dTimeNow)) || (dNextTimeSeg==0))
			 dNextTimeSeg = dTimeTemp - dTimeNow;


		//EXPIRACION DE RECURSOS
		if (m_oTimeNextExpiredRes <= oActualTime) {
			m_oTimeNextExpiredRes.now();
			m_oTimeNextExpiredRes.add_seconds(m_uiIntervalExpiredRes);
			m4Trace(cout << "JOB SCHEDULER: Expirando Recursos" << endl);
			poRequest = new ClJSRequest(M4JS_REQ_FIND_RES_EXPIRED);
			m_poCancelQ->Push(poRequest);
		}
		m_oTimeNextExpiredRes.get(dTimeTemp);
		if ((dNextTimeSeg > (dTimeTemp-dTimeNow)) || (dNextTimeSeg==0))
			 dNextTimeSeg = dTimeTemp - dTimeNow;


		//RECUPERACION DE TRABAJOS
			/*Precondiciones de recuperacion:
				-- Subsistema activo y Memoria Verde
				-- No estoy ya recuperando
				-- Ejecutores libres
			*/
		if (m_bRecoveringJobs == M4_FALSE) 
		{
			uiNumExecutors = m_oMapJobExe.GetFreeExecutors();
			if (uiNumExecutors > 0) 
			{
				if (m_oTimeNextRecover <= oActualTime) 
				{
					m_oTimeNextRecover.now();
				
					// Siempre actualizamos el tiempo para saber cuando es la próxima vez
					// de recuperación
					// Bug 0288866. Se elimina la recuperación exprés y se actualzará el
					// siguiente ciclo de recuperación (m_oTimeNextRecover) cuando se reciba la notificación
					// de ejecución terminada (EndJob).
					m_oTimeNextRecover.add_seconds(m_uiIntervalRecover);

					m_bRecoveringJobs = M4_TRUE;
					m4Trace(cout << "JOB SCHEDULER: Recuperando Trabajos" << endl);
					poRequest = new ClJSRequest(M4JS_REQ_FIND_JOBS);
					poRequest -> AddParameter(uiNumExecutors);
					m_poCancelQ->Push(poRequest);
				}

				m_oTimeNextRecover.get(dTimeTemp);
				if ((dNextTimeSeg > (dTimeTemp-dTimeNow)) || (dNextTimeSeg==0))
					 dNextTimeSeg = dTimeTemp - dTimeNow;
			}
		}
	}

	//CANCELACION
	/*Aunque la memoria sea distinta de verde o no este activo sigo buscando
	cancelaciones porque pueden quedar ejecuciones dentro del ejecutor*/
	if (m_oMapJobExe.GetUsedExecutors()>0) {
		if (m_oTimeNextCancel <= oActualTime) {
			m_oTimeNextCancel.now();
			m_oTimeNextCancel.add_seconds(m_uiIntervalCancel);
			m4Trace(cout << "JOB SCHEDULER: Buscando cancelaciones" << endl);
			poRequest = new ClJSRequest(M4JS_REQ_FIND_CANCEL);
			m_poCancelQ->Push(poRequest);
		}
		m_oTimeNextCancel.get(dTimeTemp);
		if ((dNextTimeSeg > (dTimeTemp-dTimeNow)) || (dNextTimeSeg==0))
			 dNextTimeSeg = dTimeTemp - dTimeNow;
	}

	m_oMutex.Unlock();

	///////////////////////////
	//Calculo en segundos de la siguiente operacion, si hay que realizar alguna
	//////////////////////
	m4uint32_t uiTimeSeg = 0;
	if (dNextTimeSeg != 0) {
		//Hay que realizar alguna operación
		//retornamos segundos
		//necesitados un entero, ya que 0.32 debe pasarse a 1.
		uiTimeSeg = (m4uint32_t) (dNextTimeSeg * 24 * 60 * 60);
		if (uiTimeSeg == 0)
			uiTimeSeg = 1;
	}
	return uiTimeSeg;
  //## end ClJSAdministrator::ProcessClockTick%937559774.body
}

m4return_t ClJSAdministrator::GetInterval (m4uint32_t ai_uiIntervalType, m4uint32_t &ao_uiInterval)
{
  //## begin ClJSAdministrator::GetInterval%938709944.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	m_oMutex.Lock();
	switch (ai_uiIntervalType) {
		case JS_INTERVAL_STRESS_RECOVER:
			ao_uiInterval = m_uiIntervalRecoverStress;
			break;
		case JS_INTERVAL_RECOVER:
			ao_uiInterval = m_uiIntervalRecover;
			break;
		case JS_INTERVAL_CANCELLATION:
			ao_uiInterval = m_uiIntervalCancel;
			break;
		case JS_INTERVAL_EXPIRATION:
			ao_uiInterval = m_uiIntervalExpired;
			break;
		case JS_INTERVAL_RESOURCE_EXPIRATION:
			ao_uiInterval = m_uiIntervalExpiredRes;
			break;
		default:
			ret = M4_WARNING;
	};
	m_oMutex.Unlock();
	return ret;
  //## end ClJSAdministrator::GetInterval%938709944.body
}

m4return_t ClJSAdministrator::SetInterval (m4uint32_t ai_uiIntervalType, m4uint32_t ai_uiInterval)
{
  //## begin ClJSAdministrator::SetInterval%938709949.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	if (ai_uiInterval>0) {
		m_oMutex.Lock();
		switch (ai_uiIntervalType) {
			case JS_INTERVAL_STRESS_RECOVER:
				m_uiIntervalRecoverStress = ai_uiInterval;
				break;
			case JS_INTERVAL_RECOVER:
				m_uiIntervalRecover = ai_uiInterval;
				break;
			case JS_INTERVAL_CANCELLATION:
				m_uiIntervalCancel = ai_uiInterval;
				break;
			case JS_INTERVAL_EXPIRATION:
				m_uiIntervalExpired = ai_uiInterval;
				break;
			case JS_INTERVAL_RESOURCE_EXPIRATION:
				m_uiIntervalExpiredRes = ai_uiInterval;
				break;
			default:
				ret = M4_WARNING;
		};
		m_oMutex.Unlock();
	}
	else
		ret = M4_ERROR;
	return ret;
  //## end ClJSAdministrator::SetInterval%938709949.body
}

m4return_t ClJSAdministrator::BeginJob ()
{
  //## begin ClJSAdministrator::BeginJob%939122283.body preserve=yes
	m4return_t ret = M4_ERROR;
	m4pvoid_t pParameter;
	ClExecutive *ExeTemp;
	ClJSJob *ActualTask;
	if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS) {
		ExeTemp = (ClExecutive*) pParameter;
		if (m_poActualRequest->GetParameter(1,pParameter) == M4_SUCCESS) {
			ActualTask = (ClJSJob*) pParameter;
			ret = m_oMapJobExe.UpdateExecutor(ExeTemp, ActualTask);
		}
	}
	return ret;
  //## end ClJSAdministrator::BeginJob%939122283.body
}

m4return_t ClJSAdministrator::EndJob ()
{
  //## begin ClJSAdministrator::EndJob%939122284.body preserve=yes
	m4pvoid_t pParameter;
	ClExecutive *ExeTemp; //Ejecutor que ha finalizado la tarea.
	ClJSJob *ActualTask, *RootTask, *NextTask=NULL;

	ClJSRequest *poRequest; //request de salida para mandar otra tarea

	//obtengo los datos de la request que acaba de llegar
	if (m_poActualRequest->GetParameter(0,pParameter) != M4_SUCCESS || pParameter==NULL)
		return M4_ERROR;
	ExeTemp = (ClExecutive*) pParameter;
	
	if (m_poActualRequest->GetParameter(1,pParameter) != M4_SUCCESS || pParameter==NULL)
		return M4_ERROR;
	ActualTask = (ClJSJob*) pParameter;

	m_oMapJobExe.UpdateExecutor(ExeTemp, NULL);
	//Esta funcion, solamente puede dar Error cuando ExeTemp no exista en el mapa
	//pero aun asi nos interesa procesar el trabajo cuya tarea ha finalizado

	RootTask = ActualTask->GetRoot();

	/*Pregunto si ya se ha finalizado el trabajo entero*/
	if (RootTask->IsFinished() == M4_TRUE)
	{
		// Bug 0288866. Actualizar el tiempo de la siguiente recuperación.
		m_oTimeNextRecover.now();

		m_oMapJobExe.DeleteJob(RootTask);
		
		// Bug 0286430. Desmarcar la ejecución por si no se ha podido ejecutar.
		poRequest = new ClJSRequest(M4JS_REQ_UNMARK_JOB);
		poRequest->AddParameter(RootTask);
		m_poCancelQ->Push(poRequest);

	}
	else
		/*Solicitamos al Job el siguiente task que hay que ejecutar
		Se lo solicito al mapa y no directamente al Job para
		Actualizar variables internas del mapa de nº de tareas activa.*/
		NextTask = m_oMapJobExe.GetNextTask(RootTask);

	if (NextTask == NULL) {
		//No quedan más tareas de este Job por ejecutar.

		//Tenemos un ejecutor libre. Buscamos tareas.
		if (m_bRecoveringJobs == M4_TRUE)
			//Si estoy recuperando solo busco por si hay alguna cancelada
			//ya que las waiting las buscara en EndNewJobs
			m_oMapJobExe.GetCanceledJob(NextTask);
		else
			//Busco cualquier tarea pendiente (cancelada, waiting)
			m_oMapJobExe.GetPendingJob(NextTask);
	}

	//no es else porque dentro del if anterior se puede volver a asignar
	//la varible NextTask
	if (NextTask) { 
		/*Quedan tareas, luego mando la siguiente al ejecutor. 
		Esta tarea puede provenir de dos sitios:
			1)-- El trabajo de la tarea que acaba de finalizar, tiene más tareas
			2)-- Hemos borrado el trabajo actual, y al pedir trabajos pendientes 
				nos han devuelto uno.
		????? Solicitar trabajos de mas prioridad ??????*/
		poRequest = new ClJSRequest(M4JS_REQ_EXECUTE_JOB);
		poRequest->AddParameter(NextTask);
		m_poExecutorQ->Push(poRequest);
	}
	else {
		/*Miramos si el cancelador no es válido y si es así
		le mandamos una request para que se inicialize y
		asi ya es válido*/
		InitializedRecover();

		//Intentamos finalizar puesto que puede ser la última tarea
		TerminateJS();
		//Intentamos procesar commandos (del / add de comandos)
		ProcessCommands();
	}

	return M4_SUCCESS;
  //## end ClJSAdministrator::EndJob%939122284.body
}

m4return_t ClJSAdministrator::ExecutorCreated ()
{
  //## begin ClJSAdministrator::ExecutorCreated%939122285.body preserve=yes
	m4return_t ret = M4_ERROR;
	m4pvoid_t pParameter;
	m4uint32_t uiParameter;
	ClExecutive *ExeTemp;
	if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS && pParameter != NULL) {
		if (m_poActualRequest->GetParameter(1,uiParameter) == M4_SUCCESS) {
			if (uiParameter == 0) { //Ejecutor de trabajos
				ExeTemp = (ClExecutive*) pParameter;
				ret = m_oMapJobExe.AddExecutor(ExeTemp);
			}
			if (uiParameter == 1) { //Ejecutor de cancelación
				//????????
			}
		}
	}
	return ret;
  //## end ClJSAdministrator::ExecutorCreated%939122285.body
}

m4return_t ClJSAdministrator::ExecutorDeleted ()
{
  //## begin ClJSAdministrator::ExecutorDeleted%939912563.body preserve=yes
	m4return_t ret = M4_ERROR;
	m4pvoid_t pParameter;
	m4uint32_t uiParameter;
	ClExecutive *ExeTemp;
	if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS && pParameter != NULL) {
		if (m_poActualRequest->GetParameter(1,uiParameter) == M4_SUCCESS) {
			if (uiParameter == 0) { //Ejecutor de trabajos
				ExeTemp = (ClExecutive*) pParameter;
				ret = m_oMapJobExe.DeleteExecutor(ExeTemp);
			}
			if (uiParameter == 1) { //Ejecutor de cancelación
				//????????
			}
		}
	}
	return ret;
  //## end ClJSAdministrator::ExecutorDeleted%939912563.body
}

m4return_t ClJSAdministrator::TerminateJS ()
{
  //## begin ClJSAdministrator::TerminateJS%939213538.body preserve=yes
	/*Para realizar el Shutdown tenemos que:
		- Estar en un estado READY:
			- Haber recibido la confirmación del cancelador
		- No tener ejecutores ocupados
	*/
	m4return_t ret = M4_ERROR;
	if (m_oMapJobExe.GetUsedExecutors()==0 && m_uiShutdownRequested == JS_SHUTDOWN_READY) {
		Terminate();
		ret = M4_SUCCESS;
		m4Trace (cout << "+++ Terminate Job Scheduler +++" << endl);
		BL_WARNINGF(M4_JSADM_JS, "JS_SHUTDOWN");
	}
	//Cuando se va a finalizar el JS devolvemos M4_SUCCESS
	return ret;
  //## end ClJSAdministrator::TerminateJS%939213538.body
}

m4return_t ClJSAdministrator::EndJS (M4ClTimeStamp *ai_poTime)
{
  //## begin ClJSAdministrator::EndJS%939213539.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	if (ai_poTime == NULL) {
		ret = m_poJSSubsystem->EndJS();
		if (ret == M4_SUCCESS) {
			M4ClString strTextEvent = m_poJSSubsystem->GetName();
			strTextEvent += "#";
			strTextEvent += SS_STR_PAUSED ;
			strTextEvent += "#";
			M4PublishEvent("65543",strTextEvent);
		}
	}
	else
		SetNextStopDate(ai_poTime);

	return ret;
  //## end ClJSAdministrator::EndJS%939213539.body
}

m4return_t ClJSAdministrator::BeginJS (M4ClTimeStamp *ai_poTime)
{
  //## begin ClJSAdministrator::BeginJS%939213540.body preserve=yes
	if (m_uiShutdownRequested != JS_SHUTDOWN_NONE)
		return M4_ERROR;

	m4return_t ret = M4_SUCCESS;

	if (ai_poTime == NULL) {
		ret = m_poJSSubsystem->BeginJS();
		if ( ret == M4_SUCCESS) {
			//Ponemos todos a la misma hora, para que se haga las operaciones
			//en el orden adecuado
			m_oTimeNextCancel.now();
			m_oTimeNextExpired = m_oTimeNextCancel;
			m_oTimeNextRecover = m_oTimeNextCancel;
			m_oTimeNextExpiredRes = m_oTimeNextCancel;
			m_bInmediateRecovery = M4_TRUE;
			M4ClString strTextEvent = m_poJSSubsystem->GetName();
			strTextEvent += "#";
			strTextEvent += SS_STR_ACTIVE ;
			strTextEvent += "#";
			M4PublishEvent("65543",strTextEvent);
		}
	}
	else
		SetNextStartDate(ai_poTime);

	return ret;
  //## end ClJSAdministrator::BeginJS%939213540.body
}

m4return_t ClJSAdministrator::NewJob ()
{
  //## begin ClJSAdministrator::NewJob%939213541.body preserve=yes
	m4return_t ret = M4_ERROR;
	m4pvoid_t pParameter;
	ClJSJob *JobTemp;
	if (m_poActualRequest->GetParameter(0,pParameter) == M4_SUCCESS && pParameter != NULL) {
		JobTemp = (ClJSJob*) pParameter;
		ret = m_oMapJobExe.AddJob(JobTemp);
	}
	return ret;
  //## end ClJSAdministrator::NewJob%939213541.body
}

m4return_t ClJSAdministrator::EndNewJobs ()
{
  //## begin ClJSAdministrator::EndNewJobs%939213542.body preserve=yes
	ClJSJob *JobTemp;
	ClJSRequest *poRequest;

	/*Si estamos en estado parado (porque nos han mandado parar o shutdown)
	o la memoria es distinta de VERDE:
	devolvemos los trabajos al recuperador para que los desmarque
	como cogidos, pero los que estan cancelados los mandamos ejecutar*/
	ClSSImplementation::eStatus	JSStatus;
	m_poJSSubsystem->GetState(JSStatus);
	if (JSStatus != ClSSImplementation::SS_ACTIVE || m_eMemoryStatus != JSMEMORY_GREEN) {
		//obtengo los cancelados
		while (m_oMapJobExe.GetCanceledJob(JobTemp) == M4_SUCCESS) {
			//Mandamos al ejecutor las tareas.
			poRequest = new ClJSRequest(M4JS_REQ_EXECUTE_JOB);
			poRequest->AddParameter(JobTemp);
			m_poExecutorQ->Push(poRequest);
		}
		//obtengo los waiting
		while (m_oMapJobExe.GetWaitingJob(JobTemp) == M4_SUCCESS) {
			/*indicamos al recuperador que desmarque los jobs
			como cogidos */

			 //No se borra fisicamente el job, solamente se saca del mapa, pues
			//su status es SEND.
			m_oMapJobExe.DeleteJob(JobTemp);
			poRequest = new ClJSRequest(M4JS_REQ_UNMARK_JOB);
			poRequest->AddParameter(JobTemp);
			m_poCancelQ->Push(poRequest);
		}
	}
	else {
		//Obtengo los cancelados y los waiting
		while (m_oMapJobExe.GetPendingJob(JobTemp) == M4_SUCCESS) {
			//Mandamos al ejecutor las tareas.
			poRequest = new ClJSRequest(M4JS_REQ_EXECUTE_JOB);
			poRequest->AddParameter(JobTemp);
			m_poExecutorQ->Push(poRequest);
		}
		///???Paralelización de tareas
	}

	if (m_uiShutdownRequested == JS_SHUTDOWN_PREPARING) {
		//Hemos recibido anteriormente una petición de shutdown, luego
		//le indicamos al recuperador que se va a realizar un shutdown
		poRequest = new ClJSRequest(M4JS_REQ_SHUTDOWN);
		m_poCancelQ->Push(poRequest);
	}
	
	//Borramos o creamos ejecutores si estan pendientes
	ProcessCommands();
	m_bRecoveringJobs = M4_FALSE;

	return M4_SUCCESS;
  //## end ClJSAdministrator::EndNewJobs%939213542.body
}

m4void_t ClJSAdministrator::ProcessCommands ()
{
  //## begin ClJSAdministrator::ProcessCommands%940853232.body preserve=yes
	if ((m_iPendingExecutors == 0) || (m_uiShutdownRequested != JS_SHUTDOWN_NONE))
		//No hay comandos pendientes o como estoy en shutdown no los proceso.
		return;

	ClJSRequest *poRequest;
	ClSSImplementation::eStatus	JSStatus;
	m_poJSSubsystem->GetState(JSStatus);

	if (m_iPendingExecutors > 0) {
		/*Hay que crear. Condiciones:
			-- Estado activo
			-- Memoria verde
			*/
		if (JSStatus == ClSSImplementation::SS_ACTIVE) {
			if (m_eMemoryStatus == JSMEMORY_GREEN) {
				while ((m_iPendingExecutors != 0) && (m_poJSSubsystem->AddNewExecutor()==M4_SUCCESS)) {
					//Creamos un ejecutor.....
					m_iPendingExecutors--;
				}
			}
		}
	}
	else {
		/*Hay que borrar. Condiciones:
			-- No estoy recuperando jobs
			-- Tengo ejecutores libres
		*/
		if (m_bRecoveringJobs == M4_FALSE) {
			while ((m_iPendingExecutors != 0) && (m_oMapJobExe.ReqDelExecutor() == M4_SUCCESS)) {
				//Borramos ejecutor
				poRequest = new ClJSRequest(M4JS_REQ_DEL_EXECUTOR);
				m_poExecutorQ->Push(poRequest);
				m_iPendingExecutors++;
			}
		}
	}
  //## end ClJSAdministrator::ProcessCommands%940853232.body
}

m4return_t ClJSAdministrator::ChangeMemoryStatus (eMemoryStatus ai_eMemStatus)
{
  //## begin ClJSAdministrator::ChangeMemoryStatus%940853236.body preserve=yes
	m4return_t ret = M4_WARNING;
	if (m_eMemoryStatus != ai_eMemStatus) {
		m_eMemoryStatus = ai_eMemStatus;

		//Ponemos todos a la misma hora, para que se haga las operaciones
		//en el orden adecuado
		m_oTimeNextCancel.now();
		m_oTimeNextExpired = m_oTimeNextCancel;
		m_oTimeNextRecover = m_oTimeNextCancel;
		m_oTimeNextExpiredRes = m_oTimeNextCancel;
		return ret;
	}
	return ret;
  //## end ClJSAdministrator::ChangeMemoryStatus%940853236.body
}

m4void_t ClJSAdministrator::InitializedRecover ()
{
  //## begin ClJSAdministrator::InitializedRecover%941043667.body preserve=yes
	ClJSRequest *pOutputRequest;

	/*Solamente inicializamos cuando todavía no se ha hecho y no 
	tenemos ningun trabajo en el JS, para evitar carreras, en caso 
	de desmarcar trabajos que estan en el ejecutor pero todavia no estan
	executing, u otras consideraciones*/
	if ((m_bValidRecover == M4_FALSE) && (m_oMapJobExe.GetUsedExecutors()==0)) {
		m_bValidRecover = M4_TRUE;

		//Le decimos al siguiente que se regenera que limpie la BD
		pOutputRequest = new ClJSRequest(M4JS_REQ_INITIALIZE);
		//Esta request admite un parámetro: 
			// 0: Primera Inicialización.
			// 1: Siguientes inicializaciones.
		pOutputRequest->AddParameter((m4uint32_t)1);
		m_poCancelQ->Push(pOutputRequest);
	}
  //## end ClJSAdministrator::InitializedRecover%941043667.body
}

// Additional Declarations
  //## begin ClJSAdministrator%37D537650106.declarations preserve=yes
  //## end ClJSAdministrator%37D537650106.declarations

//## begin module%37D537B60026.epilog preserve=yes
//## end module%37D537B60026.epilog
