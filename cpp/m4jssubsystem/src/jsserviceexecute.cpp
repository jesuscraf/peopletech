//## begin module%37D379DF004C.cm preserve=no
//## end module%37D379DF004C.cm

//## begin module%37D379DF004C.cp preserve=no
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
//## end module%37D379DF004C.cp

//## Module: JSServiceExecute%37D379DF004C; Package body
//## Subsystem: M4JSSubsystem::src%37BD2F0A02AE
//## Source file: D:\eduardoma\m4jssubsystem\src\jsserviceexecute.cpp

//## begin module%37D379DF004C.additionalIncludes preserve=no
//## end module%37D379DF004C.additionalIncludes

//## begin module%37D379DF004C.includes preserve=yes
#include <request.hpp>
#include <subsystemids.hpp>
#include <clserviceinfo.hpp>
#include <cloblconfiguration.hpp>
#include <clssinterfaz.hpp>
#include <ssnames.hpp>
#include <basiclog.hpp>
#include <cljssubsystem.hpp> //forward en hpp
#include <cljslauncherexecutor.hpp> //para los defines de estados
#include <timeinterval.hpp>
//## end module%37D379DF004C.includes

// Statistics
#include <statistics.hpp>
// JSServiceExecute
#include <jsserviceexecute.hpp>
// JSSExecute
#include <jssexecute.hpp>
// clservicebasess
#include <clservicebasess.hpp>


//## begin module%37D379DF004C.declarations preserve=no
//## end module%37D379DF004C.declarations

//## begin module%37D379DF004C.additionalDeclarations preserve=yes
#define MAX_SIZE_STRING  256

//Valores de la obl
#define M4JS_OBL_TIMEOUT					"JS_TIMEOUT"
#define M4JS_OBL_EXECUTE_MAX_INSTANCES		"MAX_INSTANCES"
#define M4JS_OBL_EXECUTE_MIN_INSTANCES		"MIN_INSTANCES"
#define M4JS_OBL_EXECUTE_NUM_INSTANCES		"NUM_INSTANCES_EXECUTOR_JOB_EXECUTE"

#define M4JS_EXECUTE_MAX_INSTANCES			150
#define M4JS_EXECUTE_MIN_INSTANCES			50
#define M4JS_EXECUTE_NUM_INSTANCES			25
//## end module%37D379DF004C.additionalDeclarations


// Class ClJSServiceExecute 




ClJSServiceExecute::ClJSServiceExecute (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClJSServiceExecute::ClJSServiceExecute%936606353.hasinit preserve=no
  //## end ClJSServiceExecute::ClJSServiceExecute%936606353.hasinit
  //## begin ClJSServiceExecute::ClJSServiceExecute%936606353.initialization preserve=yes
  :ClServiceBaseSS(ai_poConfiguration,ai_poTables,ai_poNavigator)
  //## end ClJSServiceExecute::ClJSServiceExecute%936606353.initialization
{
  //## begin ClJSServiceExecute::ClJSServiceExecute%936606353.body preserve=yes
	m_poStatistics = new ClStatsJSServiceExe();
	m_pcDumpFile = NULL;
 	m_oServiceName = "";
	m_oServiceName = "";
  //## end ClJSServiceExecute::ClJSServiceExecute%936606353.body
}


ClJSServiceExecute::~ClJSServiceExecute ()
{
  //## begin ClJSServiceExecute::~ClJSServiceExecute%939986156.body preserve=yes
	if (m_poStatistics) {
		delete m_poStatistics;
		m_poStatistics = NULL;
	}
  //## end ClJSServiceExecute::~ClJSServiceExecute%939986156.body
}



//## Other Operations (implementation)
m4return_t ClJSServiceExecute::Initialize ()
{
  //## begin ClJSServiceExecute::Initialize%936606354.body preserve=yes

	m4return_t ret;
	m4double_t uiInstances = 0;
	M4ClString strError;

	m_poStatistics = new ClStatsJSServiceExe;

	/////////////////
	//MAX_INSTANCES
	/////////////////
	ret = GetConfigNumericValue ( M4JS_OBL_EXECUTE_MAX_INSTANCES ,   uiInstances) ;
	if (ret == M4_SUCCESS ) {
		if (uiInstances<1 || uiInstances > M4JS_EXECUTE_MAX_INSTANCES) {
				strError = "Invalid value in JOB_SCHEDULER_EXECUTOR_SERVICE_SUBSYSTEM for ";
				strError += M4JS_OBL_EXECUTE_MAX_INSTANCES;
				strError += " .Valid values between: 1 - ";
				strError += M4JS_EXECUTE_MAX_INSTANCES;
				BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
				return M4_ERROR;
		}
		else
			m_uMaxInstances = (m4size_t) uiInstances;
	}
	else
	{
		strError = "Undefined in JOB_SCHEDULER_EXECUTOR_SERVICE_SUBSYSTEM: ";
		strError += M4JS_OBL_EXECUTE_MAX_INSTANCES;
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		return M4_ERROR;
	}

	/////////////////
	//MIN_INSTANCES
	/////////////////
	ret = GetConfigNumericValue ( M4JS_OBL_EXECUTE_MIN_INSTANCES ,   uiInstances) ;
	if (ret == M4_SUCCESS ) {
		if (uiInstances<1 || uiInstances > M4JS_EXECUTE_MIN_INSTANCES) {
				strError = "Invalid value in ";
				strError = "Invalid value in JOB_SCHEDULER_EXECUTOR_SERVICE_SUBSYSTEM for ";				
				strError += M4JS_OBL_EXECUTE_MIN_INSTANCES;
				strError += " .Valid values between: 1 - ";
				strError += M4JS_EXECUTE_MIN_INSTANCES;
				BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
				return M4_ERROR;
		}
		else
			m_uMinActiveInstances = (m4size_t) uiInstances;
	}
	else
	{
		strError = "Undefined in JOB_SCHEDULER_EXECUTOR_SERVICE_SUBSYSTEM: ";
		strError += M4JS_OBL_EXECUTE_MIN_INSTANCES;
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		return M4_ERROR;
	}

	if ( m_uMaxInstances < m_uMinActiveInstances)
		m_uMaxInstances = m_uMinActiveInstances;

	/////////////////
	//NUM_INSTANCES
	/////////////////
	ret = GetConfigNumericValue (M4JS_OBL_EXECUTE_NUM_INSTANCES, uiInstances);
	if (ret == M4_SUCCESS) {
		if (uiInstances<1 || uiInstances > M4JS_EXECUTE_NUM_INSTANCES) {
				strError = "Invalid value in JOB_SCHEDULER_EXECUTE_SERVICE_SUBSYSTEM for ";
				strError += M4JS_OBL_EXECUTE_NUM_INSTANCES;
				strError += " .Valid values: 1 - ";
				strError += M4JS_EXECUTE_NUM_INSTANCES;
				BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
				return M4_ERROR;
		}
	}
	else
	{
		strError = "In JOB_SCHEDULER_EXECUTE_SERVICE_SUBSYSTEM, undefined: ";
		strError += M4JS_OBL_EXECUTE_NUM_INSTANCES;
		strError += ". Default value = 1";
		BL_WARNINGF(M4_SRV_SS_PROPERTY_ERROR, strError);
	}

	// Se comprueba que el número de ejecutores a ser creados 
	// no es menor que el mínimo permitido para dar el servicio 
	// (MIN_INSTANCES) y que no es mayor que el número máximo 
	// (MAX_INSTANCES).
	if (uiInstances < m_uMinActiveInstances || uiInstances > m_uMaxInstances)
	{
		strError = "Invalid value in JOB_SCHEDULER_EXECUTE_SERVICE_SUBSYSTEM for ";
		strError += M4JS_OBL_EXECUTE_NUM_INSTANCES;
		strError += " . Valid values: ";
		strError += m_uMinActiveInstances;
		strError += " - ";
		strError += m_uMaxInstances;
		strError += " (user defined interval).";
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		return M4_ERROR;
	}

	SetConfigNumericProperty ( "EXECUTOR_COUNT", 0);
	SetConfigNumericProperty ( "EXITED_EXECUTORS", 0);

	return M4_SUCCESS;
  //## end ClJSServiceExecute::Initialize%936606354.body
}

m4return_t ClJSServiceExecute::GetStatisticAttribValue (m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClJSServiceExecute::GetStatisticAttribValue%936606355.body preserve=yes
	vector <m4objid_t> vChildId;
	ClSSInterfaz *poSubsystem ;
	m4uint32_t nChildId, i;

	ClDataUnit * poDataUnit ;
	ClDataUnitInterface * poDataUnitInterface ;
	m4pchar_t bufferDataUnit;

	//Variables para guardar temporalmente las estadisticas y luego
	//realizar los calculos oportunos y devolver el valor
	m4uint32_t uiNumTotal=0;
	m4uint32_t uiNumTotalTemp=0;
	M4ClTimeStamp StartDate, EndDate;
	m4time_t dStartDate, dEndDate, dTotalTime=0;
	//////
	m4bool_t bIsInactive; //Indica si el ejecutor esta operativo o no para ello miramos si tiene fecha de fin

	
	M4DataStorage *pDataStorageTemp = M4DataStorage::GetNewDataStorage();
	//Es una estadística calculada, luego le pregunto a cada hijo
	//el valor que tiene almancenado y realiza con el conjunto la opeación
	//correspondiente
	if ( M4_SUCCESS == GetSubsystemsId( vChildId) )  {
		nChildId = vChildId.size();
		for (i=0; i<nChildId; i++)  {
			poSubsystem = (ClSSInterfaz *)FindSubsystemById(vChildId[i]) ;
			if ( !poSubsystem )
				return M4_ERROR;

			switch(ai_iStatId) {

				case ClStatsJSServiceExe::NUM_JOBS:
					//Numero de Jobs completados
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::NUM_JOB_GROUPS_COMPLETE,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotal+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					//Numero de Jobs cancelados
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::NUM_BOX_INTERRUPTED,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotal+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					break;

				case ClStatsJSServiceExe::NUM_JOBS_CANCEL:
					//Numero de Jobs cancelados
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::NUM_BOX_INTERRUPTED,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotal+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					break;

				case ClStatsJSServiceExe::NUM_TASK_OK:
					//Numero de tareas ejecutadas correctamente
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::NUM_JOBS_SUCCESS,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotal+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					break;

				case ClStatsJSServiceExe::NUM_TASK_ERROR:
					//Numero de tareas ejecutadas con error
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::NUM_JOBS_ERROR,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotal+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					break;

				case ClStatsJSServiceExe::AVERAGE_TIME:
					//Tiempo medio de ejecución
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::M4_TOTAL_TIME_IN_EXECUTION,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotal+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					//Numero total de tareas lanzadas
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::NUM_JOBS_TOTAL,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotalTemp+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					break;

				case ClStatsJSServiceExe::AVERAGE_DELAY:
					//Retraso medio en comenzar una ejecución
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::M4_TOTAL_DELAY_IN_JOBS,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotal+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					//Numero total de jobs completados lanzados
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::NUM_JOB_GROUPS_COMPLETE,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotalTemp+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					//Numero de Jobs cancelados
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::NUM_BOX_INTERRUPTED,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotalTemp+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();
					break;

				case ClStatsJSServiceExe::NUM_EXECUTORS_USED:
					/*Para saber si estan activos o no miramos la fecha de fin y la de inicio
					Tenemos fecha fin pero no de incio*/

					//Leemos el estado de los ejecutores
					//Tiempo que comenzo el ejecutor: STATE-> statistic7
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClLauncherStats::CURRENT_STATE,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (strcmp(bufferDataUnit,ClLauncherStats::WaitState) == 0)
							uiNumTotal++;
						if (strcmp(bufferDataUnit,ClLauncherStats::ExecuteState) == 0)
							uiNumTotalTemp++;
					}
					pDataStorageTemp -> Clear();
					break;

				case ClStatsJSServiceExe::USE_PERCENT:
					//Porcentaje de uso de los ejecutores:
					
					//Tiempo en que el ejecutor trabaja
					poSubsystem -> GetStatisticAttribValue(poSubsystem -> GetssId () ,ClStatsJSEXECUTE::M4_TOTAL_TIME_IN_EXECUTION,pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit)
							uiNumTotal+=atol(bufferDataUnit);
					}
					pDataStorageTemp -> Clear();

					//Tiempo que comenzo el ejecutor: START_DATE -> attribute2
					poSubsystem -> GetSubsystemAttribValue (poSubsystem -> GetssId () ,2, pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					dStartDate = 0;
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit) {
							//----------------------------------------------------------
							// Quitamos las ' por : !!!!!!!!!!ÑAPA por la HA
							//----------------------------------------------------------
							for (m4uint32_t i=0; i<strlen(bufferDataUnit); i++) {
								if ((bufferDataUnit[i] == '\''))
									bufferDataUnit[i] = ':';
							}
							if (StartDate.set(bufferDataUnit) != M4_SUCCESS)
								StartDate.now();
						}
						else
							StartDate.now();
						StartDate.get(dStartDate);
					}
					pDataStorageTemp -> Clear();
					//Tiempo que finalizo el ejecutor: END_DATE -> attribute3
					poSubsystem -> GetSubsystemAttribValue (poSubsystem -> GetssId () ,3, pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					dEndDate = 0;
					bIsInactive = M4_FALSE;
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit) {
							//----------------------------------------------------------
							// Quitamos las ' por : !!!!!!!!!!ÑAPA por la HA
							//----------------------------------------------------------
							for (m4uint32_t i=0; i<strlen(bufferDataUnit); i++) {
								if ((bufferDataUnit[i] == '\''))
									bufferDataUnit[i] = ':';
							}
							if (EndDate.set(bufferDataUnit) != M4_SUCCESS)
								EndDate.now();
							else
								bIsInactive = M4_TRUE;
						}
						else
							EndDate.now();
						EndDate.get(dEndDate);
					}
					pDataStorageTemp -> Clear();
					//Calculamos el tiempo de vida del ejecutor
					dTotalTime += (dEndDate - dStartDate);

					//Miramos si esta ejecutando algo y el tiempo que lleva
					//Tiempo de comienzo de la ultima ejecucion: LAUNCHER_START -> attribute4
					poSubsystem -> GetSubsystemAttribValue (poSubsystem -> GetssId () ,4, pDataStorageTemp);
					pDataStorageTemp->InitializeIteration();
					dStartDate=0;
					if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
						poDataUnit = (ClDataUnit *)poDataUnitInterface;
						bufferDataUnit = poDataUnit->GetBuffer();
						if (bufferDataUnit) {
							//----------------------------------------------------------
							// Quitamos las ' por : !!!!!!!!!!ÑAPA por la HA
							//----------------------------------------------------------
							for (m4uint32_t i=0; i<strlen(bufferDataUnit); i++) {
								if ((bufferDataUnit[i] == '\''))
									bufferDataUnit[i] = ':';
							}
							if (StartDate.set (bufferDataUnit) == M4_SUCCESS)
								StartDate.get(dStartDate);
						}
					}
					pDataStorageTemp -> Clear();

					if (dStartDate) { //Esta ejecutando algo o ha acabado de ejecutar
						//Tiempo de finalizacion de la ultima ejecucion LAUNCHER_END -> attribute5
						poSubsystem -> GetSubsystemAttribValue (poSubsystem -> GetssId () ,5, pDataStorageTemp);
						pDataStorageTemp->InitializeIteration();
						if (pDataStorageTemp->GetNext (poDataUnitInterface) == M4_SUCCESS) {
							poDataUnit = (ClDataUnit *)poDataUnitInterface;
							bufferDataUnit = poDataUnit->GetBuffer();
							dEndDate=0;
							if (bufferDataUnit) {
								//----------------------------------------------------------
								// Quitamos las ' por : !!!!!!!!!!ÑAPA por la HA
								//----------------------------------------------------------
								for (m4uint32_t i=0; i<strlen(bufferDataUnit); i++) {
									if ((bufferDataUnit[i] == '\''))
										bufferDataUnit[i] = ':';
								}
								if (EndDate.set (bufferDataUnit) == M4_SUCCESS)
									dEndDate=1;
							}
						}
						pDataStorageTemp -> Clear();
						if ((!dEndDate) && (bIsInactive == M4_FALSE)) { //Tiene una ejecucion activa y no esta inoperante
							EndDate.now();
							EndDate.get(dEndDate);
							uiNumTotal += (m4uint32_t) ((dEndDate - dStartDate)*24*60*60);
						}
					}
					break;

				default:
					//estadistica no calculada. Llamo al de ClServiceBaseSS
					//para que llame al getStat de mis estadísticas.
					M4DataStorage::DeleteDataStorage(pDataStorageTemp);
					return ClServiceBaseSS::GetStatisticAttribValue(ai_iStatId, ao_poDataStorage);
			};
		}
	}

	m4uint32_t iSegundos;
	M4ClString strTemp;
	ClTimeInterval interval(0);
	//rellenamos el resultado 
	switch (ai_iStatId) {
		case ClStatsJSServiceExe::NUM_JOBS:
			//Numero total de Jobs
		case ClStatsJSServiceExe::NUM_JOBS_CANCEL:
			//Numero de Jobs cancelados
		case ClStatsJSServiceExe::NUM_TASK_OK:
			//Numero de tareas ejecutadas correctamente
		case ClStatsJSServiceExe::NUM_TASK_ERROR:
			//Numero de tareas ejecutadas con error
			ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
			poDataUnit->SetSize(MAX_SIZE_STRING);
			bufferDataUnit = poDataUnit->GetBuffer();
			sprintf(bufferDataUnit,"%d",uiNumTotal);
			break;
		case ClStatsJSServiceExe::NUM_EXECUTORS_USED:
			//uiNumTotal Ejecutores waiting
			//uiNumTotalTemp Ejecutores executing
			ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
			poDataUnit->SetSize(MAX_SIZE_STRING);
			bufferDataUnit = poDataUnit->GetBuffer();
			sprintf(bufferDataUnit,"%d out of %d",uiNumTotalTemp, uiNumTotalTemp+uiNumTotal);
			break;
		case ClStatsJSServiceExe::AVERAGE_TIME:
			//Tiempo medio de ejecución
			ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
			poDataUnit->SetSize(MAX_SIZE_STRING);
			bufferDataUnit = poDataUnit->GetBuffer();

			//Calculo el tiempo total en segundos por tarea
			if (uiNumTotalTemp)
				iSegundos = uiNumTotal/uiNumTotalTemp;
			else
				iSegundos = 0;
			//Establecemos el formato a h min sec
			interval.Set(iSegundos);
			interval.Get(strTemp);
			sprintf(bufferDataUnit, "%s", strTemp.c_str());
		break;

		case ClStatsJSServiceExe::AVERAGE_DELAY:
			//Retraso medio en comenzar una ejecución
			ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
			poDataUnit->SetSize(MAX_SIZE_STRING);
			bufferDataUnit = poDataUnit->GetBuffer();

			//Calculo el tiempo total en segundos por trabajo
			if (uiNumTotalTemp)
				iSegundos = uiNumTotal/uiNumTotalTemp;
			else
				iSegundos = 0;
			//Establecemos el formato a h min sec
			interval.Set(iSegundos);
			interval.Get(strTemp);
			sprintf(bufferDataUnit, "%s", strTemp.c_str());
			break;

		case ClStatsJSServiceExe::USE_PERCENT:
			//Porcentaje de uso de los ejecutores
			ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
			poDataUnit->SetSize(MAX_SIZE_STRING);
			bufferDataUnit = poDataUnit->GetBuffer();
			dTotalTime = dTotalTime * 24 * 60 *60; //pasamos a segundos
			if (dTotalTime)
				sprintf(bufferDataUnit,"%.2f",(uiNumTotal/dTotalTime)*100);
			else
				strcpy(bufferDataUnit,"0.00");
			break;

		default:
			M4DataStorage::DeleteDataStorage(pDataStorageTemp);
			return M4_ERROR;
	};

	M4DataStorage::DeleteDataStorage(pDataStorageTemp);
	return M4_SUCCESS;
  //## end ClJSServiceExecute::GetStatisticAttribValue%936606355.body
}

m4return_t ClJSServiceExecute::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClJSServiceExecute::ExecuteCommand%936606356.body preserve=yes
	M4ClString strFormtTimeOut;
	m4return_t ret = M4_ERROR;
	ClAdminTagList::iterator it;
	ClSSInterfaz *poSubsystem;
	INTERFACESMAP::iterator Iterator ;
	eStatus state ;
	ClTimeInterval interval(0);
	GetState(state) ;
	ClAdminCommandTag *newParam;
	m4char_t bufferTemp[MAX_SIZE_STRING];
	m4uint32_t uitimeout;

	if ( (SS_NOT_ACCESIBLE != state) && (SS_INACTIVE != state)) {

		switch (ai_cCommand) {
			case ClJSServiceExecute::JS_CHANGE_TIMEOUT:
				it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
				if (it == ai_oParamList.end())
					return ret;
				if ((*it)->m_tagValue) {
					// converts the value				
					strFormtTimeOut = (*it)->m_tagValue;
					if (interval.Set(strFormtTimeOut) == M4_SUCCESS) {
						//quitamos el parámetro que viene en texto y lo metemos en milisegundos
						ai_oParamList.clear();

						interval.Get(uitimeout);
						//Se pasa en milisegundos
						sprintf(bufferTemp, "%d", uitimeout*1000);
						newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_TIMEOUT", adminLong, bufferTemp);
						//Cambiamos el tipo a integer
						ai_oParamList.push_back(newParam);
						for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end() ; Iterator ++) {
							if (  poSubsystem = (*Iterator).second ) {
								ret = poSubsystem -> ExecuteCommand ( poSubsystem -> GetssId () ,ClJSSExecute::JS_CHANGE_TIMEOUT, ai_oParamList, ao_oResultList ) ;
								if (ret == M4_SUCCESS) {
									//Actualizamos al formato correcto, por si meten ej: 34343sec
									interval.Get(strFormtTimeOut);
									SetConfigStringProperty ( M4JS_OBL_TIMEOUT , strFormtTimeOut );
								}
							}
								
						}
					}
					else {
						BL_BEGIN(ERRORLOG, M4_JS_EXEC_COMMAND_FORMAT_PARAM_EXAMPLE);
						BL_OBJ << 1 << "9d 9h 9min 9sec" << "(1-9999)" << "250sec / 2d 3min / 4d 8h 10min 234sec";
						BL_END;
					}
				}
				break;
			case ClJSServiceExecute::JS_DELETE_TIMEOUT:
				for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end() ; Iterator ++) {
					if (  poSubsystem = (*Iterator).second ) {
						ret = poSubsystem -> ExecuteCommand ( poSubsystem -> GetssId () ,ClJSSExecute::JS_DELETE_TIMEOUT, ai_oParamList, ao_oResultList ) ;
						if (ret == M4_SUCCESS)
							SetConfigStringProperty ( M4JS_OBL_TIMEOUT , "Disabled" );
					}
				}
				break;
			default:
				return ClServiceBaseSS::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
		}
	}
	if (ret == M4_ERROR)
		BL_ERROR(M4_JS_EXEC_COMMAND);
	return ret;
  //## end ClJSServiceExecute::ExecuteCommand%936606356.body
}

m4return_t ClJSServiceExecute::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClJSServiceExecute::GetCommandParams%936606357.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	ClAdminCommandTag *newParam;
	m4double_t dValue=0;
	m4char_t bufferTemp[MAX_SIZE_STRING];
	size_t iLenValue;

	switch (ai_cCommand) {
		case ClJSServiceExecute::JS_CHANGE_TIMEOUT:
			GetConfigStringValue ( M4JS_OBL_TIMEOUT,   bufferTemp , MAX_SIZE_STRING ,iLenValue ) ;
			if (stricmp(bufferTemp, "Disabled") == 0)
				bufferTemp[0] = '\0';
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_TIMEOUT", adminText, bufferTemp);
			if (newParam == NULL) 
				return M4_ERROR;
			aio_oParamList.push_back(newParam);
			break;
		default:
			return ClServiceBaseSS::GetCommandParams (ai_cCommand, aio_oParamList);
	}
	return ret;
  //## end ClJSServiceExecute::GetCommandParams%936606357.body
}

m4return_t ClJSServiceExecute::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClJSServiceExecute::GetCommandsId%936606358.body preserve=yes
	if (ClServiceBaseSS::GetCommandsId (aio_oCommandList) == M4_ERROR)
		return M4_ERROR;

	ClAdminTag *newCommand;

	newCommand = ClAdminTag::GetNewAdminTag(ClJSServiceExecute::JS_CHANGE_TIMEOUT, "JS_CHANGE_TIMEOUT", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(ClJSServiceExecute::JS_DELETE_TIMEOUT, "JS_DELETE_TIMEOUT", M4_FALSE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	return M4_SUCCESS;
  //## end ClJSServiceExecute::GetCommandsId%936606358.body
}

ClSSInterfaz * ClJSServiceExecute::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClJSServiceExecute::GenerateSubsystem%936606359.body preserve=yes
	ClSSInterfaz *poInterfaz = NULL;
	ClJSSExecute *poSSExecute;
	size_t iLenValue;
	m4char_t szValue [ OBL_PROPVALUE_MAX_LEN ] ;
	
	//Leemos el time-out de ejecucion que viene en minutos
	m4double_t TimeOut=0;
	M4ClString strFormatTimeOut,strError;
	ClTimeInterval interval(0);

	m4return_t iRet = GetConfigStringValue ( M4JS_OBL_TIMEOUT,   szValue , OBL_PROPVALUE_MAX_LEN ,iLenValue ) ;
	if (iRet == M4_SUCCESS ) {
		//Reformateamos lo introducido por el cliente por si mete por ej: 343434sec
		strFormatTimeOut = szValue;
		interval.Set(strFormatTimeOut);
		interval.Get(strFormatTimeOut);
	}
	else {
		strFormatTimeOut = "Disabled";
		strError = "Undefined: ";
		strError += M4JS_OBL_TIMEOUT;
		strError += ". Default value: ";
		strError += strFormatTimeOut;
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
	}

	//Comprobamos que el valor que se nos ha pasado es correcto
	if (stricmp(strFormatTimeOut.c_str(), "Disabled") != 0) {
		if (interval.Set(strFormatTimeOut) != M4_SUCCESS) {
			strError = "Format error in ";
			strError += M4JS_OBL_TIMEOUT;
			BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
			return NULL;
		}
	}

	SetConfigStringProperty ( M4JS_OBL_TIMEOUT , strFormatTimeOut);
	//Lo pasamos a los hijos
	ai_poConfiguration -> SetStringValue ( "" , M4JS_OBL_TIMEOUT , strFormatTimeOut );

	switch (ai_lType) {
		case M4_EXECUTOR_JOB_EXECUTE_SUBSYSTEM28:
			ai_poConfiguration -> SetNumericValue ( "" , "USE_28" , 1 );
		case M4_EXECUTOR_JOB_EXECUTE_SUBSYSTEM:
			poSSExecute = new ClJSSExecute(ai_poConfiguration, ai_poTables,m_poNavigator);
			if (poSSExecute != NULL) {
				poInterfaz = new ClSSInterfaz ( poSSExecute  );
				if (poInterfaz != NULL) {
					poSSExecute -> SetInterfaz ( poInterfaz ) ;
					poSSExecute -> SetOutputQ( (ClOutputQ*) m_poOutputQ);
					poSSExecute -> SetServiceQ( (ClServiceQ*) m_poServiceQ);
					poSSExecute -> SetDumpFile(m_pcDumpFile);
					poSSExecute -> SetUseExceptions(GetUseExceptions());
					poSSExecute -> SetIdentification(m_oServerName, m_oServiceName);
				}
				else
					delete poSSExecute;
			}
			break;
		default:
			poInterfaz = NULL;
			break;
	};
	return poInterfaz;
  //## end ClJSServiceExecute::GenerateSubsystem%936606359.body
}

m4return_t ClJSServiceExecute::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClJSServiceExecute::Stop%937812722.body preserve=yes
	return ClServiceBaseSS::Stop (ai_lDeferedTime);
  //## end ClJSServiceExecute::Stop%937812722.body
}

m4return_t ClJSServiceExecute::SetDumpFile (m4pchar_t ai_pcDumpFile)
{
  //## begin ClJSServiceExecute::SetDumpFile%940237696.body preserve=yes
	m_pcDumpFile = ai_pcDumpFile;
	return M4_SUCCESS;
  //## end ClJSServiceExecute::SetDumpFile%940237696.body
}

m4return_t ClJSServiceExecute::SetIdentification (M4ClString& ai_ServerName, M4ClString& ai_ServiceName)
{
  //## begin ClJSServiceExecute::SetIdentification%942840133.body preserve=yes
	m_oServerName = ai_ServerName;
	m_oServiceName = ai_ServiceName;
	return M4_SUCCESS;
  //## end ClJSServiceExecute::SetIdentification%942840133.body
}

// Additional Declarations
  //## begin ClJSServiceExecute%37CEAA1B012A.declarations preserve=yes
  //## end ClJSServiceExecute%37CEAA1B012A.declarations

// Class ClStatsJSServiceExe 

ClStatsJSServiceExe::ClStatsJSServiceExe ()
  //## begin ClStatsJSServiceExe::ClStatsJSServiceExe%936690188.hasinit preserve=no
  //## end ClStatsJSServiceExe::ClStatsJSServiceExe%936690188.hasinit
  //## begin ClStatsJSServiceExe::ClStatsJSServiceExe%936690188.initialization preserve=yes
  //## end ClStatsJSServiceExe::ClStatsJSServiceExe%936690188.initialization
{
  //## begin ClStatsJSServiceExe::ClStatsJSServiceExe%936690188.body preserve=yes
	m_uiNumberOfStats = ClStatsJSServiceExe::TOTAL_NUM ;
  //## end ClStatsJSServiceExe::ClStatsJSServiceExe%936690188.body
}


ClStatsJSServiceExe::~ClStatsJSServiceExe ()
{
  //## begin ClStatsJSServiceExe::~ClStatsJSServiceExe%936690189.body preserve=yes
  //## end ClStatsJSServiceExe::~ClStatsJSServiceExe%936690189.body
}



//## Other Operations (implementation)
m4return_t ClStatsJSServiceExe::Reset ()
{
  //## begin ClStatsJSServiceExe::Reset%936690190.body preserve=yes
	//return ClStatistic::Reset();
	return M4_SUCCESS;
  //## end ClStatsJSServiceExe::Reset%936690190.body
}

m4return_t ClStatsJSServiceExe::GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData)
{
  //## begin ClStatsJSServiceExe::GetStat%936690191.body preserve=yes

	//Chequeamos si el handle del subsistema 
	//que nos pasan coincide con el handle del subsistema
	//que recibe las estadísticas
	if (M4_ERROR == _CheckStatID(ai_hStatId,M4_SERVICE_JOB_EXECUTE))
		return M4_ERROR;

	//Obtenemos el identificador de la estadística 
	//que queremos
	m4uint32_t lowIndex=ai_hStatId.GetLowID();

//	if (ClStatistic::TOTAL_NUM>lowIndex)
//		return ClStatistic::GetStat(ai_hStatId,ao_poData);

	if (ao_poData.GetSize()>0)
		return M4_ERROR;

/*	ao_poData.SetSize(SIZE_STRING);

	m4pchar_t buffer=ao_poData.GetBuffer();


	eJSExecuteStats statsIndex=(eJSExecuteStats) lowIndex;

	//Copiamos en le buffer del Data Unit el valor 
	//que corresponda a la estadística que queremos

	m_poMutex.Lock();

	switch (statsIndex)
	{
	};

	m_poMutex.Unlock();*/

	return M4_SUCCESS;

  //## end ClStatsJSServiceExe::GetStat%936690191.body
}

m4return_t ClStatsJSServiceExe::ResetStat (ClStatHandle &ai_hStatId)
{
  //## begin ClStatsJSServiceExe::ResetStat%936690192.body preserve=yes
	//Chequeamos si el handle del subsistema 
	//que nos pasan coincide con el handle del subsistema
	//que recibe las estadísticas
	if (M4_ERROR == _CheckStatID(ai_hStatId,M4_SERVICE_JOB_EXECUTE))
		return M4_ERROR;

	//Obtenemos el identificador de la estadística 
	//que queremos
	m4uint32_t lowIndex=ai_hStatId.GetLowID();

//	if (ClStatistic::TOTAL_NUM>lowIndex)
//		return ClStatistic::ResetStat(ai_hStatId);

	return M4_SUCCESS;
  //## end ClStatsJSServiceExe::ResetStat%936690192.body
}

// Additional Declarations
  //## begin ClStatsJSServiceExe%37D4C1B10343.declarations preserve=yes
  //## end ClStatsJSServiceExe%37D4C1B10343.declarations

//## begin module%37D379DF004C.epilog preserve=yes
//## end module%37D379DF004C.epilog
