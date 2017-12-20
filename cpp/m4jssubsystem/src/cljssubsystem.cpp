//## begin module%36E4009D03A4.cm preserve=no
//## end module%36E4009D03A4.cm

//## begin module%36E4009D03A4.cp preserve=no
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
//## end module%36E4009D03A4.cp

//## Module: ClJSSubsystem%36E4009D03A4; Package body
//## Subsystem: M4JSSubsystem::src%37BD2F0A02AE
//## Source file: D:\FuentesServidor\m4jssubsystem\src\cljssubsystem.cpp

//## begin module%36E4009D03A4.additionalIncludes preserve=no
//## end module%36E4009D03A4.additionalIncludes

//## begin module%36E4009D03A4.includes preserve=yes
#include <syncro.hpp> //Porque es forward en hpp el ClMutex
#include <jsadministrator.hpp> //Porque es forward en hpp
#include <m4servenv.hpp> //Para el GetEnviron
#include <basiclog.hpp>
#include <configtree.hpp>
#include <timeinterval.hpp>
#include <m4eventhandlers.hpp>
//## end module%36E4009D03A4.includes

// JSServiceCancel
#include <jsservicecancel.hpp>
// JSServiceExecute
#include <jsserviceexecute.hpp>
// ClJSSubsystem
#include <cljssubsystem.hpp>
// ClOblConfiguration
#include <cloblconfiguration.hpp>
// clssinterfaz
#include <clssinterfaz.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// statsjssubsystem
#include <statsjssubsystem.hpp>


//## begin module%36E4009D03A4.declarations preserve=no
//## end module%36E4009D03A4.declarations

//## begin module%36E4009D03A4.additionalDeclarations preserve=yes
//Entradas de la OBL
#define M4JS_OBL_SUBSYSTEM_DUMPFILE						"DUMP_FILE"
#define M4JS_OBL_SUBSYSTEM_SERVER_NAME					"SERVER_NAME"
#define M4JS_OBL_SUBSYSTEM_SERVICE_NAME					"SERVICE_NAME"
#define M4JS_OBL_SUBSYSTEM_ACTIVE						"ACTIVE"
#define M4JS_OBL_SUBSYSTEM_STRINGID						"STRINGID"
#define M4JS_OBL_CANCELLATION_FREQUENCY					"CANCELLATION_FREQUENCY"
#define M4JS_OBL_RECOVERING_FREQUENCY					"RECOVERING_FREQUENCY"
#define M4JS_OBL_EXPIRATION_FREQUENCY					"EXPIRATION_FREQUENCY"
#define M4JS_OBL_ATTACHMENT_EXPIRATION_FREQUENCY		"ATTACHMENT_EXPIRATION_FREQUENCY"
#define M4JS_OBL_EXCEPTION_HANDLING						"EXCEPTION_HANDLING"
#define M4JS_OBL_CRITICAL_SERVICE						"CRITICAL_SERVICE"

//Valores por defecto en la obl
#define M4JS_SERVICE_NAME							"Job Scheduler"
#define M4JS_SERVER_NAME							"M4Server"
#define M4JS_DUMP_FILE								"js_dump.txt"


#define MAX_JS_BUFFER	255
//## end module%36E4009D03A4.additionalDeclarations


// Class ClJSSubsystem 





ClJSSubsystem::ClJSSubsystem (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClJSSubsystem::ClJSSubsystem%920912504.hasinit preserve=no
  //## end ClJSSubsystem::ClJSSubsystem%920912504.hasinit
  //## begin ClJSSubsystem::ClJSSubsystem%920912504.initialization preserve=yes
   :ClSSImplementation(ai_poConfiguration,ai_poTables,ai_poNavigator)
  //## end ClJSSubsystem::ClJSSubsystem%920912504.initialization
{
  //## begin ClJSSubsystem::ClJSSubsystem%920912504.body preserve=yes
	m_oServerName = M4JS_SERVER_NAME;
	m_oStrDumpFile = M4JS_DUMP_FILE;
	m_poJSAdministrator = NULL;
	m_poStatistics = new ClStatsJSSubsystem();
  //## end ClJSSubsystem::ClJSSubsystem%920912504.body
}


ClJSSubsystem::~ClJSSubsystem ()
{
  //## begin ClJSSubsystem::~ClJSSubsystem%936606352.body preserve=yes
	if (m_poJSAdministrator) {
		delete m_poJSAdministrator;
		m_poJSAdministrator = NULL;
	}

	if (m_poStatistics) {
		delete m_poStatistics;
		m_poStatistics = NULL;
	}

  //## end ClJSSubsystem::~ClJSSubsystem%936606352.body
}



//## Other Operations (implementation)
m4return_t  ClJSSubsystem::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClJSSubsystem::ExecuteCommand%920912506.body preserve=yes
	m4return_t ret = M4_ERROR;
	ClAdminTagList::iterator it;
	m4uint32_t iNewInterval;
	M4ClTimeStamp *pNewDate;
	ClJSRequest *pJSRequest;
	ClSSInterfaz *poSubsystem;
	INTERFACESMAP::iterator Iterator ;
	eStatus state ;
	ClTimeInterval interval(0);	
	GetState(state) ;

	if ( (SS_NOT_ACCESIBLE != state) && (SS_INACTIVE != state)) {
		switch (ai_cCommand) {
			case ClJSSubsystem::JS_CANCEL_FREQUENCY:
				it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
				if (it != ai_oParamList.end()) {
					// converts the value
					interval.Set((*it)->m_tagValue);
					interval.Get(iNewInterval);
					if (iNewInterval>0)
						ret = m_poJSAdministrator->SetInterval(ClJSAdministrator::JS_INTERVAL_CANCELLATION, iNewInterval);
					else {
						BL_BEGIN(ERRORLOG, M4_JS_EXEC_COMMAND_FORMAT_PARAM_EXAMPLE); 
						BL_OBJ << 1 << "9d 9h 9min 9sec" << "(1-9999)" << "250sec / 2d 3min / 4d 8h 10min 234sec";
						BL_END;
					}
				}
				break;
			case ClJSSubsystem::JS_RECOVER_FREQUENCY:
				it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
				if (it != ai_oParamList.end()) {
					// converts the value
					interval.Set((*it)->m_tagValue);
					interval.Get(iNewInterval);
					if (iNewInterval>0)
						ret = m_poJSAdministrator->SetInterval(ClJSAdministrator::JS_INTERVAL_RECOVER, iNewInterval);
					else {
						BL_BEGIN(ERRORLOG, M4_JS_EXEC_COMMAND_FORMAT_PARAM_EXAMPLE); 
						BL_OBJ << 1 << "9d 9h 9min 9sec" << "(1-9999)" << "250sec / 2d 3min / 4d 8h 10min 234sec";
						BL_END;
					}
				}
				break;			
			case ClJSSubsystem::JS_EXPIRATION_FREQUENCY:
				it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
				if (it != ai_oParamList.end()) {
					// converts the value
					interval.Set((*it)->m_tagValue);
					interval.Get(iNewInterval);
					if (iNewInterval>0)
						ret = m_poJSAdministrator->SetInterval(ClJSAdministrator::JS_INTERVAL_EXPIRATION, iNewInterval);
					else {
						BL_BEGIN(ERRORLOG, M4_JS_EXEC_COMMAND_FORMAT_PARAM_EXAMPLE); 
						BL_OBJ << 1 << "9d 9h 9min 9sec" << "(1-9999)" << "250sec / 2d 3min / 4d 8h 10min 234sec";
						BL_END;
					}
				}
				break;
			case ClJSSubsystem::JS_RES_EXPIRATION_FREQUENCY:
				it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
				if (it != ai_oParamList.end()) {
					// converts the value
					interval.Set((*it)->m_tagValue);
					interval.Get(iNewInterval);
					if (iNewInterval>0)
						ret = m_poJSAdministrator->SetInterval(ClJSAdministrator::JS_INTERVAL_RESOURCE_EXPIRATION, iNewInterval);
					else {
						BL_BEGIN(ERRORLOG, M4_JS_EXEC_COMMAND_FORMAT_PARAM_EXAMPLE); 
						BL_OBJ << 1 << "9d 9h 9min 9sec" << "(1-9999)" << "250sec / 2d 3min / 4d 8h 10min 234sec";
						BL_END;
					}
				}
				break;
			case ClJSSubsystem::JS_BEGIN_JS:
				it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
				//Escenarios:
				//Caso1: Si no vienen argumentos es pq desde el monitor deseamos hacer el start.
				//Caso2: Si viene un argumento y es 1 es pq desde SNMP deseamos hacer start.
				//	Si viene un argumento o es pq desde SNMP deseamos hacer un stop.
				//Si no hay argumentos es pq estamos ejecutando el comando desde el monitor.
				if (it == ai_oParamList.end()) {
					pJSRequest = new ClJSRequest(M4JS_REQ_BEGIN_JS);
					m_poJSAdministrator->GetInputQueue()->Push(pJSRequest);
					ret = M4_SUCCESS;
				}
				else {
					//Si hay argumentos tenemos que comprobar el valor.
					m4pchar_t pcStart = (m4pchar_t)((*it)->m_tagValue);
					if (pcStart != NULL) {
						if (!strcmp(pcStart, "1")) {
							//Haremos un start!.
							pJSRequest = new ClJSRequest(M4JS_REQ_BEGIN_JS);
							m_poJSAdministrator->GetInputQueue()->Push(pJSRequest);
							ret = M4_SUCCESS;
						}
						else {
							//Si es 0 hacemos stop.
							if (!strcmp(pcStart, "0")) {							
								pJSRequest = new ClJSRequest(M4JS_REQ_END_JS);
								m_poJSAdministrator->GetInputQueue()->Push(pJSRequest);
								ret = M4_SUCCESS;
							}
							else {
								//Si no es ni 1 ni 0 damos error.
								ret = M4_ERROR;
							}
						}
					}
					else {
						//El argumento no puede ser nulo.
						ret = M4_ERROR;
					}
				}
				break;
			case ClJSSubsystem::JS_END_JS:
				pJSRequest = new ClJSRequest(M4JS_REQ_END_JS);
				m_poJSAdministrator->GetInputQueue()->Push(pJSRequest);
				ret = M4_SUCCESS;
				break;
			case ClJSSubsystem::JS_BEGIN_JS_AT:
				it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
				if (it != ai_oParamList.end()) {
					// converts the value
					pNewDate = new M4ClTimeStamp();
					if (pNewDate->set((*it)->m_tagValue) == M4_SUCCESS) {
						pJSRequest = new ClJSRequest(M4JS_REQ_BEGIN_JS);
						pJSRequest-> AddParameter(pNewDate);
						m_poJSAdministrator->GetInputQueue()->Push(pJSRequest);
						ret = M4_SUCCESS;
					}
					else {
						BL_BEGIN(ERRORLOG, M4_JS_EXEC_COMMAND_FORMAT_PARAM); 
						BL_OBJ << 1 << "yyyy/mm/dd-hh:mm:ss"; 
						BL_END;
						delete pNewDate;
					}
				}
				break;
			case ClJSSubsystem::JS_END_JS_AT:
				it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
				if (it != ai_oParamList.end()) {
					// converts the value
					pNewDate = new M4ClTimeStamp();
					if (pNewDate->set((*it)->m_tagValue) == M4_SUCCESS) {
						pJSRequest = new ClJSRequest(M4JS_REQ_END_JS);
						pJSRequest-> AddParameter(pNewDate);
						m_poJSAdministrator->GetInputQueue()->Push(pJSRequest);
						ret = M4_SUCCESS;
					}
					else {
						BL_BEGIN(ERRORLOG, M4_JS_EXEC_COMMAND_FORMAT_PARAM); 
						BL_OBJ << 1 << "yyyy/mm/dd-hh:mm:ss"; 
						BL_END;
						delete pNewDate;
					}
				}
				break;
			case ClJSSubsystem::JS_DELETE_START_DATE:
				pJSRequest = new ClJSRequest(M4JS_REQ_DEL_START_DATE);
				m_poJSAdministrator->GetInputQueue()->Push(pJSRequest);
				ret = M4_SUCCESS;
				break;
			case ClJSSubsystem::JS_DELETE_STOP_DATE:
				pJSRequest = new ClJSRequest(M4JS_REQ_DEL_STOP_DATE);
				m_poJSAdministrator->GetInputQueue()->Push(pJSRequest);
				ret = M4_SUCCESS;
				break;
			case ClJSSubsystem::JS_DISABLE_EXCEPTIONS:
				for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end() ; Iterator ++) {
					if (  poSubsystem = (*Iterator).second ) {
						poSubsystem -> ExecuteCommand(poSubsystem -> GetssId () ,ClServiceBaseSS::DISABLE_EXCEPTIONS, ai_oParamList, ao_oResultList);
						SetConfigStringProperty (  "EXCEPTION_HANDLING" , "Disabled" ) ;
						ret = M4_SUCCESS;
					}
				}
				break;	 
			case ClJSSubsystem::JS_ENABLE_EXCEPTIONS:
				for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end() ; Iterator ++) {
					if (  poSubsystem = (*Iterator).second ) {
						poSubsystem -> ExecuteCommand(poSubsystem -> GetssId () ,ClServiceBaseSS::ENABLE_EXCEPTIONS, ai_oParamList, ao_oResultList);
						SetConfigStringProperty ( "EXCEPTION_HANDLING" , "Enabled" ) ;
						ret = M4_SUCCESS;
					}
				}
				break;
			default:
				ret = ClSSImplementation::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
				break;
		}
	}
	if (ret == M4_ERROR)
		BL_ERROR(M4_JS_EXEC_COMMAND);

	return ret;
  //## end ClJSSubsystem::ExecuteCommand%920912506.body
}

m4return_t ClJSSubsystem::Synchronize ()
{
  //## begin ClJSSubsystem::Synchronize%920912508.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	if (m_poJSAdministrator) {
		if ( m_poJSAdministrator->IsRunning())
			ret =  ClSSImplementation::Synchronize ();
	}
	return ret;
  //## end ClJSSubsystem::Synchronize%920912508.body
}

m4return_t ClJSSubsystem::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClJSSubsystem::GetCommandsId%920912509.body preserve=yes

	if (ClSSImplementation::GetCommandsId (aio_oCommandList) != M4_SUCCESS)
		return M4_ERROR;

	ClAdminTag *newCommand;

	newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_CANCEL_FREQUENCY, "JS_CHANGE_CANCELLATION_FREQUENCY", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_RECOVER_FREQUENCY, "JS_CHANGE_RECOVER_FREQUENCY", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_EXPIRATION_FREQUENCY, "JS_CHANGE_EXPIRATION_FREQUENCY", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_RES_EXPIRATION_FREQUENCY, "JS_CHANGE_RES_EXPIRATION_FREQUENCY" , M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	//Comandos disponibles para el cambio de estado depende del estado actual
	eStatus	JSStatus;
	GetState(JSStatus);
	if (JSStatus == SS_PAUSED) {
		newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_BEGIN_JS, "JS_BEGIN_JS" , M4_FALSE);
		if (newCommand == NULL) 
			return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}

	if (JSStatus == SS_ACTIVE) {
		newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_END_JS, "JS_END_JS" , M4_FALSE);
		if (newCommand == NULL) 
			return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}

	newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_BEGIN_JS_AT, "JS_BEGIN_JS_AT" , M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_END_JS_AT, "JS_END_JS_AT" , M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	M4ClTimeStamp NextTime;

	if (m_poJSAdministrator->GetNextStartDate(NextTime) == M4_SUCCESS) {
		newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_DELETE_START_DATE, "JS_DELETE_START_DATE" , M4_FALSE);
		if (newCommand == NULL) 
			return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}

	if (m_poJSAdministrator->GetNextStopDate(NextTime) == M4_SUCCESS) {
		newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_DELETE_STOP_DATE, "JS_DELETE_STOP_DATE" , M4_FALSE);
		if (newCommand == NULL) 
			return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}

	//Comandos de gestion de excepciones
	m4char_t ExceptionHandle [OBL_PROPVALUE_MAX_LEN] ;
	size_t iLen ;
	GetConfigStringValue ( "EXCEPTION_HANDLING" , ExceptionHandle , OBL_PROPVALUE_MAX_LEN , iLen ) ;
	if (strcmpi("ENABLED", ExceptionHandle)==0) {
		//Comando de disable exceptions
		newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_DISABLE_EXCEPTIONS, "TURN_OFF_EXCEPTIONS" , M4_FALSE);
		if (newCommand == NULL) 
			return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}
	else {
		//Comando de enable exceptions
		newCommand = ClAdminTag::GetNewAdminTag(ClJSSubsystem::JS_ENABLE_EXCEPTIONS, "TURN_ON_EXCEPTIONS" , M4_FALSE);
		if (newCommand == NULL) 
			return M4_ERROR;
		aio_oCommandList.push_back(newCommand);
	}

	return M4_SUCCESS;
  //## end ClJSSubsystem::GetCommandsId%920912509.body
}

m4return_t ClJSSubsystem::ShutDown ()
{
  //## begin ClJSSubsystem::ShutDown%920912510.body preserve=yes
	return ClSSImplementation::ShutDown();
  //## end ClJSSubsystem::ShutDown%920912510.body
}

m4return_t ClJSSubsystem::Initialize ()
{
  //## begin ClJSSubsystem::Initialize%920912511.body preserve=yes
	size_t iLenValue;
	m4char_t szValue [ OBL_PATHVALUE_MAX_LEN ] ;
	m4return_t iRet;
	M4ClString strError, strServiceName;

	//////////////////////////
	//Me subscribo para recibir evento server_ready , etc..
	///////////////////////////
	ClSSInterfaz *poSubsystem = (ClSSInterfaz *) m_poNavigator->GetSubsystemById(0);
	if (poSubsystem)
		poSubsystem->AttachObserver(this);
	else
		return M4_ERROR;

	/////////////////////
	//Nombre del servidor
	/////////////////////
	iRet = GetEnviron ( "SERVER" , M4JS_OBL_SUBSYSTEM_SERVER_NAME ,   szValue , OBL_PATHVALUE_MAX_LEN) ;
	if (iRet == M4_SUCCESS )
		m_oServerName = szValue;
	else {
		strError = "Undefined: ";
		strError += M4JS_OBL_SUBSYSTEM_SERVER_NAME;
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		return M4_ERROR;
	}
	/*Comprobamos que el tamaño no supera 50, ya que es una restricción del campo
	donde se guarda en BD la identificación de JS*/
	if (m_oServerName.size()>50) {
		strError = "Max. size 50 excedeed for property: ";
		strError += M4JS_OBL_SUBSYSTEM_SERVER_NAME;
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		return M4_ERROR;
	}

	/////////////////////////////
	//Fichero para realizar dumps
	/////////////////////////////
	iRet = GetConfigStringValue ( M4JS_OBL_SUBSYSTEM_DUMPFILE ,   szValue , OBL_PATHVALUE_MAX_LEN ,iLenValue ) ;
	if (iRet == M4_SUCCESS )
		m_oStrDumpFile = szValue;
	else {
		strError = "Undefined: ";
		strError += M4JS_OBL_SUBSYSTEM_DUMPFILE;
		strError += ". Default value: ";
		strError += m_oStrDumpFile;
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
	}

	///////////////////////////
	//Nombre del servicio de JS
	///////////////////////////
	strServiceName = M4JS_SERVICE_NAME;
	iRet = GetConfigStringValue ( M4JS_OBL_SUBSYSTEM_SERVICE_NAME ,   szValue , OBL_PATHVALUE_MAX_LEN ,iLenValue ) ;
	if (iRet == M4_SUCCESS )
		strServiceName = szValue;
	else {
		strError = "Undefined: ";
		strError += M4JS_OBL_SUBSYSTEM_SERVICE_NAME;
		strError += ". Default value: ";
		strError += strServiceName;
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
	}
	//Comprobamos que no existe otro JS con el mismo nombre
	if (m_poNavigator->GetSubsystemByName(strServiceName) != NULL) {
		//Existe otro subsystema con este nombre
		strError = M4JS_OBL_SUBSYSTEM_SERVICE_NAME;
		strError += " with value: ";
		strError += strServiceName;
		strError += " already defined in JOB_SCHEDULER_SUBSYSTEM";
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		return M4_ERROR;
	}
	/*Comprobamos que el tamaño no supera 50, ya que es una restricción del campo
	donde se guarda en BD la identificación de JS*/
	if (strServiceName.size()>50) {
		strError = "Max. size 50 excedeed for property: ";
		strError += M4JS_OBL_SUBSYSTEM_SERVICE_NAME;
		strError += " in JOB_SCHEDULER_SUBSYSTEM";
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		return M4_ERROR;
	}
	//Establecemos el nombre al subsystema
	SetName(strServiceName);
	//Tambien cambiamos el stringId para que sea lo que aparezca en la herramienta de administracion
	SetConfigStringProperty (  M4JS_OBL_SUBSYSTEM_STRINGID, strServiceName);

	/////////////////
	//Activo JS o no
	/////////////////
//	M4ClString strActive;
	m4bool_t bJSActive = M4_FALSE;
	iRet = GetConfigStringValue (  M4JS_OBL_SUBSYSTEM_ACTIVE , szValue , OBL_PATHVALUE_MAX_LEN ,iLenValue ) ;
	if (iRet == M4_SUCCESS ) {
		if (strcmpi("TRUE", szValue)==0)
			bJSActive = M4_TRUE;
		else {
			if (strcmpi("FALSE", szValue)!=0) {
				strError = "Invalid value for ";
				strError += M4JS_OBL_SUBSYSTEM_ACTIVE;
				strError += ". Posible values: \"True\"/\"False\". Default value: \"False\".";
				BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
			}
		}	
	}
	else
	{
		strError = "Undefined: ";
		strError += M4JS_OBL_SUBSYSTEM_ACTIVE;
		strError += ". Default value: \"False\".";
		BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
	}

	if (bJSActive == M4_FALSE)
		EndJS();
	else
		BeginJS();

	m_poJSAdministrator = new ClJSAdministrator(this);
	if (m_poJSAdministrator==NULL)
		return M4_ERROR;

	/*
    ////////////
	Miramos si tenemos establecidos intervlos para el administrador
	CANCELLATION_FREQUENCY
	RECOVERING_FREQUENCY
	EXPIRATION_FREQUENCY
	ATTACHMENT_EXPIRATION_FREQUENCY
	//////////////
    */
	m4uint32_t uiValue;
	ClTimeInterval interval(0);
	iRet = GetConfigStringValue ( M4JS_OBL_CANCELLATION_FREQUENCY , szValue , OBL_PATHVALUE_MAX_LEN ,iLenValue ) ;
	if (iRet == M4_SUCCESS ) {
		interval.Set(szValue);
		interval.Get(uiValue);
		if (uiValue>0)
			m_poJSAdministrator->SetInterval(ClJSAdministrator::JS_INTERVAL_CANCELLATION, uiValue);
		else {
			strError = "Invalid value for ";
			strError += M4JS_OBL_CANCELLATION_FREQUENCY;
			BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		}
	}

	iRet = GetConfigStringValue ( M4JS_OBL_RECOVERING_FREQUENCY , szValue , OBL_PATHVALUE_MAX_LEN ,iLenValue ) ;
	if (iRet == M4_SUCCESS ) {
		interval.Set(szValue);
		interval.Get(uiValue);
		if (uiValue>0)
			m_poJSAdministrator->SetInterval(ClJSAdministrator::JS_INTERVAL_RECOVER, uiValue);
		else {
			strError = "Invalid value for ";
			strError += M4JS_OBL_RECOVERING_FREQUENCY;
			BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		}
	}
	iRet = GetConfigStringValue ( M4JS_OBL_EXPIRATION_FREQUENCY , szValue , OBL_PATHVALUE_MAX_LEN ,iLenValue ) ;
	if (iRet == M4_SUCCESS ) {
		interval.Set(szValue);
		interval.Get(uiValue);
		if (uiValue>0)
			m_poJSAdministrator->SetInterval(ClJSAdministrator::JS_INTERVAL_EXPIRATION, uiValue);
		else {
			strError = "Invalid value for ";
			strError += M4JS_OBL_EXPIRATION_FREQUENCY;
			BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		}
	}
	iRet = GetConfigStringValue ( M4JS_OBL_ATTACHMENT_EXPIRATION_FREQUENCY , szValue , OBL_PATHVALUE_MAX_LEN ,iLenValue ) ;
	if (iRet == M4_SUCCESS ) {
		interval.Set(szValue);
		interval.Get(uiValue);
		if (uiValue>0)
			m_poJSAdministrator->SetInterval(ClJSAdministrator::JS_INTERVAL_RESOURCE_EXPIRATION, uiValue);
		else {
			strError = "Invalid value for ";
			strError += M4JS_OBL_ATTACHMENT_EXPIRATION_FREQUENCY;
			BL_ERRORF(M4_SRV_SS_PROPERTY_ERROR, strError);
		}
	}

	//////////////
	//Generacion del cancelador, recuperador ..
	//////////////
//		BEGIN JOB_SCHEDULER_CANCEL_SERVICE SUBSYSTEM\n
	M4ClString strConfigCancel = "\
TYPE=\"JOB_SCHEDULER_CANCEL_SERVICE\";\
SERVICE_ID=\"M4_INTERNAL_SERVICE\";\
STRINGID=\"Cancel Service\"";

	ClConfiguration * poConfiguration = new ClM4ConfigTree(strConfigCancel);
	ClSSInterfaz *newSSCancel = GenerateSubsystem(M4_SERVICE_JOB_CANCEL, poConfiguration, m_poTables);
	if (newSSCancel == NULL)
		return M4_ERROR;
	if (AddSubsystem(newSSCancel) == M4_ERROR)
		return M4_ERROR;
	if (newSSCancel -> Initialize() == M4_ERROR)
		return M4_ERROR;
	return newSSCancel -> Init(poConfiguration);
  //## end ClJSSubsystem::Initialize%920912511.body
}

void ClJSSubsystem::SetConfiguration (ClConfiguration *ai_poConfiguration)
{
  //## begin ClJSSubsystem::SetConfiguration%920912514.body preserve=yes
  //## end ClJSSubsystem::SetConfiguration%920912514.body
}

m4return_t ClJSSubsystem::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClJSSubsystem::Stop%929441246.body preserve=yes
	m4return_t ret;
	
	
	SetState ( ClSSImplementation::SS_SHUTING_DOWN , SS_STR_SHUTTING_DOWN ) ;

	ClJSQueue *pAdminQueue = NULL;
	if (m_poJSAdministrator != NULL) {
		pAdminQueue = m_poJSAdministrator->GetInputQueue();
		if (pAdminQueue) {
			ClJSRequest *poRequest = new ClJSRequest(M4JS_REQ_SHUTDOWN);
			pAdminQueue->Push(poRequest);
			if (0 == ai_lDeferedTime) {
				//Solamente mandamos interrumpir a lo bruto cuando el tiempo que
				//nos dan es 0, es decir , hay que pararse ya mismo..
				ClJSRequest *poRequestInterrupt = new ClJSRequest(M4JS_REQ_UNCONTROLED_INTERRUPT);
				poRequestInterrupt->AddParameter((m4pvoid_t)NULL);
				pAdminQueue->Push(poRequestInterrupt);
			}
		}
	}
	
	ret = ClSSImplementation::Stop (ai_lDeferedTime);

	return ret;
  //## end ClJSSubsystem::Stop%929441246.body
}

ClSSInterfaz * ClJSSubsystem::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClJSSubsystem::GenerateSubsystem%936606366.body preserve=yes
	ClSSInterfaz *poInterfaz;
	ClJSServiceExecute *poServiceExecute;
	ClJSServiceCancel *poServiceCancel;
	ClJSQueue *pAdminQueue = NULL;
	M4ClString strServiceName = GetName();
	m4char_t szValue [OBL_PROPVALUE_MAX_LEN] ;
	size_t iLen ;

	/*********
	Leemos si tenemos que tener control de excepciones en los servicios
	y se lo pasamos a los servicicios
	***********/
	m4double_t dValue=0;
	m4return_t ret;
	m4bool_t bExceptions = M4_FALSE;
	M4ClString strExceptions;	
	ret = GetConfigStringValue ( M4JS_OBL_EXCEPTION_HANDLING , szValue , OBL_PROPVALUE_MAX_LEN , iLen ) ;
	if ( ret == M4_ERROR ) {
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "Property %d missed for service %s", M4JS_OBL_EXCEPTION_HANDLING, strServiceName.c_str()) ;
		//Establecemos el valor por defecto a Disabled
		strExceptions = "Disabled";
	}
	else {
		if (strcmpi("ENABLED", szValue)==0) {
			dValue = 1;
			bExceptions = M4_TRUE ;
			strExceptions = "Enabled";
		}
		else {
			if (strcmpi("DISABLED", szValue)!=0) {
				SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Incorrect value for property %d in service %s", M4JS_OBL_EXCEPTION_HANDLING, strServiceName.c_str()) ;
				return NULL;
			}
			else
				strExceptions = "Disabled";
		}
	}
	SetConfigStringProperty ( M4JS_OBL_EXCEPTION_HANDLING , strExceptions ) ;
	ai_poConfiguration -> SetNumericValue ( "" , "EXCEPTIONS" , dValue ) ;

	/*********
	Miramos si el servicio va a ser crítico y se lo pasamos a los servicios
	***********/
	M4ClString strCritical;	
	ret = GetConfigStringValue ( M4JS_OBL_CRITICAL_SERVICE , szValue , OBL_PROPVALUE_MAX_LEN , iLen ) ;
	if ( ret == M4_ERROR ) {
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "Property %d missed for service %s", M4JS_OBL_CRITICAL_SERVICE, strServiceName.c_str()) ;
		//Establecemos el valor por defecto a FALSE
		strCritical = "False";
	}
	else {
		if (strcmpi("True", szValue)==0) {
			strCritical = "True"; 
		}
		else {
			if (strcmpi("False", szValue)!=0) {
				SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Incorrect value for property %d in service %s", M4JS_OBL_CRITICAL_SERVICE, strServiceName.c_str()) ;
				return NULL;
			}
			else
				strCritical = "False";	
		}
	}
	SetConfigStringProperty (  M4JS_OBL_CRITICAL_SERVICE , strCritical ) ;
	ai_poConfiguration -> SetStringValue ( "" , M4JS_OBL_CRITICAL_SERVICE , strCritical ) ;

	switch (ai_lType) {
		case M4_SERVICE_JOB_EXECUTE:
			poServiceExecute = new ClJSServiceExecute(ai_poConfiguration, ai_poTables, m_poNavigator);
			poInterfaz = new ClSSInterfaz ( poServiceExecute  );
			poServiceExecute -> SetInterfaz ( poInterfaz ) ;
			if (m_poJSAdministrator) {
				pAdminQueue = m_poJSAdministrator->GetInputQueue();
				poServiceExecute->SetOutputQ( (ClOutputQ*)pAdminQueue);
				pAdminQueue = m_poJSAdministrator->GetExecutorQueue();
				poServiceExecute->SetServiceQ((ClServiceQ*)pAdminQueue);
				poServiceExecute->SetDumpFile(m_oStrDumpFile);
				poServiceExecute->SetUseExceptions ( bExceptions );
				poServiceExecute -> SetIdentification(m_oServerName, strServiceName);
			}
			//Me subscribo para recibir eventos: SERVICE_SHUTDOWN....
			poServiceExecute->AttachObserver(this);
			break;
		case M4_SERVICE_JOB_CANCEL:
			poServiceCancel = new ClJSServiceCancel(ai_poConfiguration, ai_poTables, m_poNavigator);
			poInterfaz = new ClSSInterfaz ( poServiceCancel  );
			poServiceCancel -> SetInterfaz ( poInterfaz ) ;
			if (m_poJSAdministrator) {
				pAdminQueue = m_poJSAdministrator->GetInputQueue();
				poServiceCancel->SetOutputQ((ClOutputQ*)pAdminQueue);
				pAdminQueue = m_poJSAdministrator->GetCancelQueue();
				poServiceCancel -> SetServiceQ((ClServiceQ*)pAdminQueue);
				poServiceCancel -> SetDumpFile(m_oStrDumpFile);
				poServiceCancel -> SetIdentification(m_oServerName, strServiceName);
				poServiceCancel -> SetUseExceptions ( bExceptions );
				//Las estadisticas del servicio y su ejecutor se muestran
				//en el subsistema de JS
				poServiceCancel -> SetStats(m_poStatistics);
			}
			//Me subscribo para recibir eventos: SERVICE_SHUTDOWN...
			poServiceCancel->AttachObserver(this);
			break;
		default:
			poInterfaz = NULL;
			break;
	};
	return poInterfaz;
  //## end ClJSSubsystem::GenerateSubsystem%936606366.body
}

m4return_t ClJSSubsystem::Start ()
{
  //## begin ClJSSubsystem::Start%936966059.body preserve=yes
	ClSSInterfaz * poControlMemorySS = NULL ;

	poControlMemorySS = (ClSSInterfaz *) m_poNavigator -> GetSubsystemByType ( M4_MEMORY_CONTROL_SUBSYSTEM ) ;

	if ( poControlMemorySS )
		poControlMemorySS -> AttachObserver ( this ) ;

	if (m_poJSAdministrator->GetInputQueue() == NULL ||
		m_poJSAdministrator->GetCancelQueue() == NULL ||
		m_poJSAdministrator->GetExecutorQueue() == NULL)
		return M4_ERROR;
	m_poJSAdministrator -> Start();
	return ClSSImplementation::Start();
  //## end ClJSSubsystem::Start%936966059.body
}

m4return_t ClJSSubsystem::GetStatisticAttribValue (m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage)
{
  //## begin ClJSSubsystem::GetStatisticAttribValue%938709943.body preserve=yes
	if (!m_poJSAdministrator)
		return M4_ERROR;

	m4return_t ret = M4_SUCCESS;

	ClDataUnit *poDataUnit ;
	m4pchar_t bufferDataUnit = NULL;
	m4uint32_t iSegundos=0;

	//Para las estadisticas de next_start y next_stop.
	M4ClTimeStamp oTimeTemp;
	M4ClString strTimeTemp;

	switch (ai_iStatId) {
		case ClStatsJSSubsystem::JS_NEXT_START_DATE:
			if (m_poJSAdministrator->GetNextStartDate(oTimeTemp) == M4_SUCCESS)
				oTimeTemp.get(strTimeTemp);
			else
				strTimeTemp = "-";
			ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
			poDataUnit->SetSize(strlen(strTimeTemp.c_str())+1);
			bufferDataUnit = poDataUnit->GetBuffer();
			strcpy(bufferDataUnit, strTimeTemp.c_str());
			break;
		case ClStatsJSSubsystem::JS_NEXT_STOP_DATE:
			if (m_poJSAdministrator->GetNextStopDate(oTimeTemp) == M4_SUCCESS)
				oTimeTemp.get(strTimeTemp);
			else
				strTimeTemp = "-";
			ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
			poDataUnit->SetSize(strlen(strTimeTemp.c_str())+1);
			bufferDataUnit = poDataUnit->GetBuffer();
			strcpy(bufferDataUnit, strTimeTemp.c_str());
			break;
		case ClStatsJSSubsystem::JS_CANCEL_FREQUENCY:
			ret = m_poJSAdministrator->GetInterval(ClJSAdministrator::JS_INTERVAL_CANCELLATION, iSegundos);
			if (ret == M4_SUCCESS) {
				ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
				poDataUnit->SetSize(MAX_JS_BUFFER);
				bufferDataUnit = poDataUnit->GetBuffer();
				ClTimeInterval interval(iSegundos);
				interval.Get(strTimeTemp);
				strcpy(bufferDataUnit, strTimeTemp.c_str());
			}			
			break;
		case ClStatsJSSubsystem::JS_RECOVER_FREQUENCY:
			ret = m_poJSAdministrator->GetInterval(ClJSAdministrator::JS_INTERVAL_RECOVER, iSegundos);
			if (ret == M4_SUCCESS) {
				ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
				poDataUnit->SetSize(MAX_JS_BUFFER);
				bufferDataUnit = poDataUnit->GetBuffer();
				ClTimeInterval interval(iSegundos);
				interval.Get(strTimeTemp);
				strcpy(bufferDataUnit, strTimeTemp.c_str());
			}			
			break;
		case ClStatsJSSubsystem::JS_EXPIRED_FREQUENCY:
			ret = m_poJSAdministrator->GetInterval(ClJSAdministrator::JS_INTERVAL_EXPIRATION, iSegundos);
			if (ret == M4_SUCCESS) {
				ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
				poDataUnit->SetSize(MAX_JS_BUFFER);
				bufferDataUnit = poDataUnit->GetBuffer();
				ClTimeInterval interval(iSegundos);
				interval.Get(strTimeTemp);
				strcpy(bufferDataUnit, strTimeTemp.c_str());
			}			
			break;
		case ClStatsJSSubsystem::JS_EXPIRED_RES_FREQUENCY:
			ret = m_poJSAdministrator->GetInterval(ClJSAdministrator::JS_INTERVAL_RESOURCE_EXPIRATION, iSegundos);
			if (ret == M4_SUCCESS) {
				ao_poDataStorage -> GetNewDataUnit ( poDataUnit ) ;
				poDataUnit->SetSize(MAX_JS_BUFFER);
				bufferDataUnit = poDataUnit->GetBuffer();
				ClTimeInterval interval(iSegundos);
				interval.Get(strTimeTemp);
				strcpy(bufferDataUnit, strTimeTemp.c_str());
			}			
			break;
		default:
			return ClSSImplementation::GetStatisticAttribValue(ai_iStatId, ao_poDataStorage);
	};


	//----------------------------------------------------------
	// Check for the syntax of the data in this statistic.
	// Some values conflicts with the VB client administrator 
	// protocol
	//----------------------------------------------------------
	if (bufferDataUnit != NULL) {
		for (m4uint32_t i=0; i<strlen(bufferDataUnit); i++)
		{
			if ((bufferDataUnit[i] == ':') ||
				(bufferDataUnit[i] == ',') ||
				(bufferDataUnit[i] == ';'))
			{
				bufferDataUnit[i] = '\'';
			}
		}
	}
	return ret;
  //## end ClJSSubsystem::GetStatisticAttribValue%938709943.body
}

m4return_t ClJSSubsystem::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClJSSubsystem::GetCommandParams%938709950.body preserve=yes
	m4uint32_t intervalo=0;
	ClAdminCommandTag *newParam;
	m4return_t ret = M4_SUCCESS;
	m4char_t bufferTemp[MAX_JS_BUFFER];
	M4ClTimeStamp oTimeTemp;
	M4ClString strTemp;
	ClTimeInterval interval(0);

	switch (ai_cCommand) {
		case ClJSSubsystem::JS_CANCEL_FREQUENCY:
			m_poJSAdministrator->GetInterval(ClJSAdministrator::JS_INTERVAL_CANCELLATION, intervalo);
			interval.Set(intervalo);
			interval.Get(strTemp);
			strcpy(bufferTemp, strTemp.c_str());
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_PARAM_CANCELLATION_FREQUENCY", adminText, bufferTemp);
			if (newParam == NULL) 
				return M4_ERROR;
			aio_oParamList.push_back(newParam);
			break;
		case ClJSSubsystem::JS_RECOVER_FREQUENCY:
			m_poJSAdministrator->GetInterval(ClJSAdministrator::JS_INTERVAL_RECOVER, intervalo);
			interval.Set(intervalo);
			interval.Get(strTemp);
			strcpy(bufferTemp, strTemp.c_str());
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_PARAM_RECOVER_FREQUENCY", adminText, bufferTemp);
			if (newParam == NULL) 
				return M4_ERROR;
			aio_oParamList.push_back(newParam);
			break;
		case ClJSSubsystem::JS_EXPIRATION_FREQUENCY:
			m_poJSAdministrator->GetInterval(ClJSAdministrator::JS_INTERVAL_EXPIRATION, intervalo);
			interval.Set(intervalo);
			interval.Get(strTemp);
			strcpy(bufferTemp, strTemp.c_str());
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_PARAM_EXPIRATION_FREQUENCY", adminText, bufferTemp);
			if (newParam == NULL) 
				return M4_ERROR;
			aio_oParamList.push_back(newParam);
			break;
		case ClJSSubsystem::JS_RES_EXPIRATION_FREQUENCY:
			m_poJSAdministrator->GetInterval(ClJSAdministrator::JS_INTERVAL_RESOURCE_EXPIRATION, intervalo);
			interval.Set(intervalo);
			interval.Get(strTemp);
			strcpy(bufferTemp, strTemp.c_str());
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_PARAM_RES_EXPIRATION_FREQUENCY", adminText, bufferTemp);
			if (newParam == NULL) 
				return M4_ERROR;
			aio_oParamList.push_back(newParam);
			break;
		case ClJSSubsystem::JS_BEGIN_JS_AT:
			if (m_poJSAdministrator->GetNextStartDate(oTimeTemp) == M4_SUCCESS) {
				oTimeTemp.get(strTemp);
				if (strlen(strTemp.c_str()) < MAX_JS_BUFFER)
					strcpy(bufferTemp, strTemp.c_str());
				else
					sprintf(bufferTemp, "");
			}
			else
				sprintf(bufferTemp, "");
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_PARAM_NEXT_START_DATE", adminTimeStamp, bufferTemp);
			if (newParam == NULL) 
				return M4_ERROR;
			aio_oParamList.push_back(newParam);
			break;
		case ClJSSubsystem::JS_END_JS_AT:
			if (m_poJSAdministrator->GetNextStopDate(oTimeTemp) == M4_SUCCESS) {
				oTimeTemp.get(strTemp);
				if (strlen(strTemp.c_str()) < MAX_JS_BUFFER)
					strcpy(bufferTemp, strTemp.c_str());
				else
					sprintf(bufferTemp, "");
			}
			else
				sprintf(bufferTemp, "");
			newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "JS_PARAM_NEXT_STOP_DATE", adminTimeStamp, bufferTemp);
			if (newParam == NULL) 
				return M4_ERROR;
			aio_oParamList.push_back(newParam);
			break;
		default:
			ret = ClSSImplementation::GetCommandParams (ai_cCommand, aio_oParamList);
	};

	return ret;
  //## end ClJSSubsystem::GetCommandParams%938709950.body
}

m4return_t ClJSSubsystem::GetServerName (M4ClString& ao_oServerName)
{
  //## begin ClJSSubsystem::GetServerName%940337545.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	if ( strcmp( m_oServerName.c_str(), "Undefined" ) == 0 ) {
		ret = M4_WARNING;
	}
	ao_oServerName = m_oServerName;
	return ret;
  //## end ClJSSubsystem::GetServerName%940337545.body
}

m4bool_t ClJSSubsystem::IsReadyToClose ()
{
  //## begin ClJSSubsystem::IsReadyToClose%940521657.body preserve=yes
	m4bool_t ret = M4_FALSE;
	/*Pregunto por IsRunning en lugar de IsExit porque, puede ocurrir
	que el thread no haya sido todavia creado y el IsExit nos devolveria M4_FALSE*/
	if (m_poJSAdministrator)
	{
		if (m_poJSAdministrator->IsRunning()==M4_FALSE)
			ret = ClSSImplementation::IsReadyToClose();
	}
	return ret;
  //## end ClJSSubsystem::IsReadyToClose%940521657.body
}

m4return_t ClJSSubsystem::AddNewExecutor ()
{
  //## begin ClJSSubsystem::AddNewExecutor%940853234.body preserve=yes
	m4return_t ret = M4_ERROR;
	ClSSInterfaz *pServiceExecute = FindSubsystemByType(M4_SERVICE_JOB_EXECUTE);
	if (pServiceExecute) {
		ret = ((ClJSServiceExecute*)pServiceExecute)->AddNewExecutive();
	}
	return ret;
  //## end ClJSSubsystem::AddNewExecutor%940853234.body
}

m4void_t ClJSSubsystem::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClJSSubsystem::Update%940853235.body preserve=yes
	ClJSQueue *pAdminQueue = NULL;
	ClJSRequest *poRequest= NULL;
	M4ClString strTextEvent;
	switch (ai_iEvent) {
//		case CLOSE_SESSION:
//		case DB_RYPTURE_CONNECTION:
//		case CANCELED_REQUEST:
		case SERVICE_SHUTDOWN:
			SetState(SS_INACTIVE , SS_STR_INACTIVE);
			m4Trace(cout << "+++++ JOB SCHEDULER: INACTIVE +++++" << endl);
			strTextEvent = GetName();
			strTextEvent += "#";
			strTextEvent += SS_STR_INACTIVE ;
			strTextEvent += "#";
			M4PublishEvent("65543",strTextEvent);
			break;
		case GREEN_MEMORY_LEVEL:
			if (m_poJSAdministrator != NULL) {
				pAdminQueue = m_poJSAdministrator->GetInputQueue();
				if (pAdminQueue) {
					poRequest = new ClJSRequest(M4JS_REQ_MEMORY_STATE);
					poRequest->AddParameter((m4uint32_t)ClJSAdministrator::JSMEMORY_GREEN);
					pAdminQueue->Push(poRequest);
				}
			}
			break;
		case YELLOW_MEMORY_LEVEL:
			if (m_poJSAdministrator != NULL) {
				pAdminQueue = m_poJSAdministrator->GetInputQueue();
				if (pAdminQueue) {
					poRequest = new ClJSRequest(M4JS_REQ_MEMORY_STATE);
					poRequest->AddParameter((m4uint32_t)ClJSAdministrator::JSMEMORY_YELLOW);
					pAdminQueue->Push(poRequest);
				}
			}
			break;
		case RED_MEMORY_LEVEL:
			if (m_poJSAdministrator != NULL) {
				pAdminQueue = m_poJSAdministrator->GetInputQueue();
				if (pAdminQueue) {
					poRequest = new ClJSRequest(M4JS_REQ_MEMORY_STATE);
					poRequest->AddParameter((m4uint32_t)ClJSAdministrator::JSMEMORY_RED);
					pAdminQueue->Push(poRequest);
				}
			}
			break;
//		case MEMORY_ALERT:
		case SERVER_READY:
			if (m_poJSAdministrator != NULL) {
				pAdminQueue = m_poJSAdministrator->GetInputQueue();
				if (pAdminQueue) {
					poRequest = new ClJSRequest(M4JS_REQ_SERVER_READY);
					pAdminQueue->Push(poRequest);
				}
			}
			break;
//		case ADMIN_SHUTDOWN:
//		case TCP_PORT_BUSY:
//		case CONFIG_ERROR:
	};
  //## end ClJSSubsystem::Update%940853235.body
}

m4return_t ClJSSubsystem::BeginJS ()
{
  //## begin ClJSSubsystem::BeginJS%940940805.body preserve=yes
	m4return_t ret = M4_ERROR;
	eStatus	JSStatus;
	GetState(JSStatus);
	if (JSStatus == SS_PAUSED) {
		SetState(SS_ACTIVE , SS_STR_ACTIVE);
		m4Trace(cout << "+++++ JOB SCHEDULER: ACTIVED +++++" << endl);
		ret = M4_SUCCESS;
	}
	return ret;
  //## end ClJSSubsystem::BeginJS%940940805.body
}

m4return_t ClJSSubsystem::EndJS ()
{
  //## begin ClJSSubsystem::EndJS%940940806.body preserve=yes
	m4return_t ret = M4_ERROR;
	eStatus	JSStatus;
	GetState(JSStatus);
	if (JSStatus == SS_ACTIVE) {
 		SetState(SS_PAUSED , SS_STR_PAUSED);
		m4Trace(cout << "+++++ JOB SCHEDULER: PAUSED +++++" << endl);
 		ret = M4_SUCCESS;
	}
	return ret;
  //## end ClJSSubsystem::EndJS%940940806.body
}

// Additional Declarations
  //## begin ClJSSubsystem%36E3C96F0280.declarations preserve=yes
  //## end ClJSSubsystem%36E3C96F0280.declarations

//## begin module%36E4009D03A4.epilog preserve=yes
//## end module%36E4009D03A4.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClJSSubsystem::ShutDown%945176653.body preserve=no
	Stop(0);
	return ClSSImplementation::ShutDown();
//## end ClJSSubsystem::ShutDown%945176653.body

#endif
