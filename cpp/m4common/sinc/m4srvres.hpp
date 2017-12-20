//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              channel
// File:                m4res.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                31-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the error codes for the channel
//
//
//==============================================================================


#ifndef __M4SRVRES_HPP__
#define __M4SRVRES_HPP__


#include <m4res.hpp>





//----------------------------------------------------------------------------
//		Errores del servidor
//----------------------------------------------------------------------------


#define M4_SRV_STARTUPOBL_ERROR						0x0001	+	M4_SRV_ERROR_BASE
#define M4_SRV_TSAP_ERROR							0x0002	+	M4_SRV_ERROR_BASE
#define M4_SRV_STARTUP_SUCCESS						0x0003	+	M4_SRV_ERROR_BASE
#define M4_SRV_SERVICE_ERROR						0x0004	+	M4_SRV_ERROR_BASE
#define M4_SRV_SCHED_NULL_Q							0x0005	+	M4_SRV_ERROR_BASE
#define M4_SRV_SEND_NULL_PDU						0x0006	+	M4_SRV_ERROR_BASE
#define M4_SRV_SEND_DISCONNECT						0x0007	+	M4_SRV_ERROR_BASE
#define M4_SRV_ERROR_PDU							0x0008	+	M4_SRV_ERROR_BASE
#define M4_SRV_SESSION_CHANNEL						0x0009	+	M4_SRV_ERROR_BASE
#define M4_SRV_SCHED_REQUEST_QUEUED					0x000A	+	M4_SRV_ERROR_BASE
#define M4_CRYPTFILE_ERROR							0x000B	+	M4_SRV_ERROR_BASE
#define M4_LOCKED_QUEUE								0x000C	+	M4_SRV_ERROR_BASE
#define M4_EXCEPTION_DUMP							0x000D	+	M4_SRV_ERROR_BASE
#define M4_EXCEPTION_UNKNOWN						0x000E	+	M4_SRV_ERROR_BASE

#define M4_SERVER_READY								0x0011	+	M4_SRV_ERROR_BASE
#define M4_EXCEPTION_CAUGHT							0x0012	+	M4_SRV_ERROR_BASE
#define M4_CANCELLATION_DETECTED					0x0013	+	M4_SRV_ERROR_BASE
#define M4_SERVER_DEBUG_INFO						0x0014  +   M4_SRV_ERROR_BASE
#define	M4_SERVER_TRACE								0x0015	+   M4_SRV_ERROR_BASE
#define M4_SERVER_SHUTDOWN							0x0016	+	M4_SRV_ERROR_BASE
#define M4_SRV_APPCTL_PARAM_ERROR					0x0017	+	M4_SRV_ERROR_BASE
#define M4_SRV_MEMORY_LEVEL							0x0018	+	M4_SRV_ERROR_BASE
#define M4_SRV_APPCTL_DISCONNECT					0x0019	+	M4_SRV_ERROR_BASE
#define M4_SRV_APPCTL_CONNECT						0x001A	+	M4_SRV_ERROR_BASE
#define M4_SRV_APPCTL_CONFIG						0x001B	+	M4_SRV_ERROR_BASE

//----------------------------------------------------------------------------
//			monitor local
//----------------------------------------------------------------------------

#define	M4_SRV_LM_ARGUMENT_ERROR					0x0001	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_DICTIONARY_ERROR					0x0002	+ 	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_ADD_SERVICE_ON_LINE				0x0003	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_GET_SCHEDULER						0x0004	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_REMOVE_CONNECTION					0x0005	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_DOWN_CONNECTION					0x0006	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_MANAGER_CHANGED					0x0007	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_NULL_ARGUMENT						0x0008	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COULD_NOT_GET_CONNECTION			0x0009	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SERVICE_NOT_CREATED				0x000A	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COMMAND_ALLREADY_EXIST			0x000B	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COMMAND_DOES_NOT_EXIST			0x000C	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_NOT_DEFINED_COMMAND				0x000D	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_NO_COMMANDS						0x000E	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COULDNT_GET_COMMAND				0x000F	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_EXECUTE_COMMAND					0x0010	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_EXECUTE_SENTENCE					0x0011	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_BAD_ARGUMENT						0x0012	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_INSERT_OPERATION					0x0013	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_OPERATION_NOT_INSERTED			0x0014	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_NOT_INITIALIZED					0X0015	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_NONE_INSTANCES_IN_STARTUP			0X0016	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_BAD_PORT_NUMBER					0x0017	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SHUTING_DOWN_SCHEDULER			0x0018  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SHUTING_DOWN_CONNMANAGER			0x0019  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SHUTING_DOWN_DISTRIBUTOR			0x001A  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SHUTING_DOWN_SENDER				0x001B  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SHUTING_DOWN_RECEIVER				0x001C  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SHUTING_DOWN_LAUNCHER				0x001D  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SHUTING_DOWN_ACCEPTOR				0x001E  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SHUTING_DOWN_ADMINISTRATOR		0x001F  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_CREATING_ADMINISTRATOR			0x0020  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_ACCEPTOR_ERROR					0x0021  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_NEW_CONNECTION_ERROR				0x0022  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_CREATE_LAUNCHER						0x0023  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_PDULOG_ERROR							0x0024  +	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SEND_REQUEST						0x0025	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_REGISTER_REQUEST					0x0026	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_SERVICE_REQUEST					0x0027	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COMM_BEGIN_ERROR					0x0028	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COMM_READ_PARAM_ERROR				0x0029	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COMM_WRITE_PARAM_ERROR			0x002A	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COMM_ACCEPT_PARAM_ERROR			0x002B	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_CONN_NOT_FOUND					0x002C	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_CONN_CLOSED_JET					0x002D	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_DUPPLIED_CONN_ID					0x002E	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COMM_WRITE_PARAM_XML_ERROR		0x002F	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_DUPPLIED_REQ_ID					0x0030	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_REQ_DESREG_JET					0x0031	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_CANCEL_NO_REQ						0x0032	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_XML_DESREG_JET					0x0033	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_ADDCOMWORK_ERROR					0x0034	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_COMM_PROCESS_PARAM_ERROR			0x0035	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_BEFORE_WRITE_PARAM_ERROR			0x0036	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_OPEN_CONN_ERROR					0x0037	+	M4_SRV_LM_ERROR_BASE
#define M4_SRV_LM_NO_SESSION_AVAILABLE_ERROR		0x0038	+	M4_SRV_LM_ERROR_BASE

//----------------------------------------------------------------------------
//			Receiver.
//----------------------------------------------------------------------------


//#define M4_SRV_DISCONNECTION						0x0001 + M4_SRV_RECEIVER_BASE
#define M4_SRV_ERROR_IN_PDU							0x0002 + M4_SRV_RECEIVER_BASE
#define M4_SRV_ERROR_CONTROL_PORT					0x0003 + M4_SRV_RECEIVER_BASE
// Error en el status del active tsap
#define M4_SRV_ERROR_IN_STATUS						0x0004 + M4_SRV_RECEIVER_BASE
#define M4_SRV_CONNECT_REGISTRATION					0x0005 + M4_SRV_RECEIVER_BASE
#define M4_SRV_DECOMPRESS_PDU_BAD_IDENTIFIER		0x0006 + M4_SRV_RECEIVER_BASE
#define M4_SRV_READED_SEVERAL_PDUS					0x0007 + M4_SRV_RECEIVER_BASE
		


//-----------------------------------------------------------------
//			SUBSYSTEMAS
//--------------------------------------------------------------

#define M4_SRV_SS_OBL_ERROR							0X0001	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_PROPERTY_ERROR					0x0002	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_TREE_ERROR						0x0003	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_TABLES_ERROR						0x0004	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_STATS_ERROR						0x0005	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_UUSS_ERROR						0x0006	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_USER_ERROR						0x0007	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_SESSION_ERROR						0x0008	+ M4_SRV_SS_ERROR_BASE
#define	M4_SRV_SS_ROLE_ERROR						0x0009	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_COMMAND_PARAM_ERROR				0x000A	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_CACHE_ERROR						0x000B	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_SHUTDOWN_ERROR					0x000C	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_SYNCHRONIZE_ERROR					0x000D	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_ATTRIBUTES_ERROR					0x000E	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_FACTORY_ERROR						0x000F	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_REGEN_MSG_ERROR					0x0010	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_REGEN_FAIL_ERROR					0x0011	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_REGEN_EXHAUST_ERROR				0x0012	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_PROPERTY_VALUE_ERROR				0x0013	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_INACTIVE							0x0014	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_UNKNOWN							0x0015	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_ACTION_UNKNOWN						0x0016	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_COMM_ERROR							0x0017	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_CONFIG_PROP_ERROR					0x0018	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_CONFIG_PROP_WARNING					0x0019	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_CONFIG_PROP_VALUEERROR				0x001A	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_CONFIG_PROP_VALUEWARNING				0x001B	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_CONFIG_PROP_BUFERROR					0x001C	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_TABLES_PROP_ERROR					0x001D	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_COMMAND_ID_ERROR						0x001E	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_COMMAND_MAX_INSTANCES				0x001F	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_BENCHFILE_ERROR						0x0020	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_DUMP_ERROR_OPEN						0x0021	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_DUMP_WRITE_ERROR						0x0022	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_DUMP_DIRECTORY_ERROR					0x0023	+ M4_SRV_SS_ERROR_BASE



//-----------------------------------------------------------
//	JOB SCHEDULER
// 	Definidos en el fichero m4jsres.hpp de la componente m4jssusbystem
//----------------------------------------------------------


//-----------------------------------------------------------------------------
//									API  CLIENT
//-----------------------------------------------------------------------------

#define M4_ERR_OPENSES_UNKOWN					0x0000 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_OPENSES_TSAP						0x0001 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_OPENSES_INVHOST					0x0002 + M4_SRV_API_CLIENTE_ERROR 	
#define M4_ERR_OPENSES_USERUNKOWN				0x0003 + M4_SRV_API_CLIENTE_ERROR 
#define M4_ERR_CONECTREQ_TIMEOUT				0x0004 + M4_SRV_API_CLIENTE_ERROR
#define M4_WAR_OPENSES_PASSEXP					0x0005 + M4_SRV_API_CLIENTE_ERROR
#define M4_WAR_OPENSES_INVTAG					0x0006 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_OPENSES_ADDCONECT				0x0007 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_SES_IN_DICT_YET					0x0008 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_INITSAP							0x0009 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_INITCSC							0x000A + M4_SRV_API_CLIENTE_ERROR
#define M4_WAR_SYNCREQ_TIMEOUT					0x000B + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_SYNCREQ_INVSESS					0x000C + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_ASYNCREQ_INVSES					0x000D + M4_SRV_API_CLIENTE_ERROR
#define M4_WAR_RECEIVE_TIMEOUT					0x000E + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_CLOSSES_INVSESS					0x000F + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_ENBROL_INVSESS					0x0010 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_ENABROLE_TIMEOUT					0x0011 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_DISABROLE_INVSES					0x0012 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_DISCOREQ_TIMEOUT					0x0013 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_CHANGPASSWD_TIMEOUT				0x0014 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_OPENSERV_INVSES					0x0015 + M4_SRV_API_CLIENTE_ERROR
#define	M4_ERR_ADDSERV							0x0016 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_CLOSESERV_INVSES					0x0017 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_CANCELREQ_TIMEOUT				0x0018 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_CANCELREQ_INVSES					0x0019 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_MONITOR						0x001A + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_SERV_IN_DIC					0x001B + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_SESS_NOT_IN_DIC					0x001C + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_SESS_IN_DIC					0x001D + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_REQ_IN_DIC					0x001E + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_PDU_FROM_QUEUE				0x001F + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_REMOVEREQ_INVSES					0x0020 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_GETREQ_REQNOTFOUND				0x0021 + M4_SRV_API_CLIENTE_ERROR
#define M4_WAR_INITCSC							0x0022 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_TSAP_ERROR						0x0023 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_PDU_ERROR						0x0024 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_CONNECTION_ERROR					0x0025 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_TSAPNOTFOUND_IN_MAP				0x0026 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_PCOND_NULL_IN_CONTROL_PDU		0x0027 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_REQ_NOT_DEL_FROM_DICT			0x0028 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_SES_NOT_DEL_FROM_DICT			0x0029 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_SERV_NOT_DEL_FROM_DICT			0x002A + M4_SRV_API_CLIENTE_ERROR
#define M4_WAR_NO_LOGINFO						0x002B + M4_SRV_API_CLIENTE_ERROR
#define M4_DEBUG_APICLIENT_TIME					0x002C + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_REQ_FROM_QUEUE				0x002D + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_TSAP_FROM_CONNECTION		0x002E + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_COMUNIC_ERROR_IN_SENDER			0x002F + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_PDU							0x0030 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_SERV_NOT_IN_DIC					0x0031 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_NULL_RECIEVER					0x0032 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_OPENSES_ADDSESS					0x0033 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_ERROR_FROM_SERVER				0x0034 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_REQ_NOT_DELETED					0x0035 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_REQUEST_CANCELED					0x0036 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_TAG_NOT_FOUND					0x0037 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_INCORRECT_TAG_TYPE				0x0038 + M4_SRV_API_CLIENTE_ERROR
#define M4_ERR_TAG_ERROR						0x0039 + M4_SRV_API_CLIENTE_ERROR

// HTTP WinINet.
// "Error ejecutando la función InternetOpen.\n \nError al intentar establecer una sesión e inicializar las funciones de WinINet: %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_INTERNET_OPEN					0x003C + M4_SRV_API_CLIENTE_ERROR
// "Error ejecutando la función InternetConnect.\n \nError al intentar establecer una conexión con el servidor %0:s y el puerto %1:d: %2:s. \nDebe consultar con el administrador.\n"
#define M4_ERR_INTERNET_CONNECT					0x003D + M4_SRV_API_CLIENTE_ERROR
// "Error intentando construir URL.\n \nError al intentar construir la URL con los parámetros server=%0:s, puesto=%1:d y useSSL=%2:d.\nDebe consultar con el administrador.\n"
#define M4_ERR_BUILD_URL						0x003E + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función InternetCloseHandle.\n \nError al intentar cerrar un handle: %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_INTERNET_CLOSE_HANDLE			0x003F + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función HttpOpenRequest.\n \nError al intentar crear una petición HTTP contra la URL %0:s: %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_HTTP_OPEN_REQUEST				0x0040 + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función HttpSendRequestEx.\n \nError al intentar mandar una petición HTTP: %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_HTTP_SEND_REQUEST				0x0041 + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función InternetWriteFile.\n \nError al escribir datos de la PDU, %0:d bytes, en la petición HTTP: %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_INTERNET_WRITE_FILE				0x0042 + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función HttpEndRequest.\n \nError al intentar finalizar una petición HTTP: %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_HTTP_END_REQUEST					0x0043 + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función InternetQueryDataAvailable.\n \nError al intentar averiguar la cantidad de datos disponible en el servidor: %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_INTERNET_QUERY_DATA_AVAILABLE	0x0044 + M4_SRV_API_CLIENTE_ERROR
// "No se han encontrado datos disponibles.\n \nNo hay datos disponibles en el servidor como consecuencia de una petición HTTP.\nDebe consultar con el administrador.\n"
#define M4_ERR_NO_INTERNET_DATA_AVAILABLE		0x0045 + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función InternetReadFile.\n \nError al intentar leer los datos de una petición HTTP: %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_INTERNET_READ_FILE				0x0046 + M4_SRV_API_CLIENTE_ERROR
// "Error al contruir PDU.\n \nError al intentar contruir la PDU a partir de los datos de la petición HTTP.\nDebe consultar con el administrador.\n"
#define M4_ERR_BUILD_PDU						0x0047 + M4_SRV_API_CLIENTE_ERROR
// "Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicación."
#define	M4_ERR_NO_MEMORY						0x0048 + M4_SRV_API_CLIENTE_ERROR
// "Error recuperando PDU de la petición HTTP.\n \n Se ha producido un error al intentar recuperar el tamaño de la cabecera de la PDU.\nDebe consultar con el administrador.\n"
#define	M4_ERR_PDU_GET_HEAD_SIZE				0x0049 + M4_SRV_API_CLIENTE_ERROR
// "Error recuperando PDU de la petición HTTP.\n \n Se ha producido un error al intentar crear la PDU con los datos de la petición.\nDebe consultar con el administrador.\n"
#define	M4_ERR_PDU_GET_PDU						0x004A + M4_SRV_API_CLIENTE_ERROR
// "Error construyendo PDU.\n \n Se ha producido un error al leer el siguiente bloque de la PDU.\nDebe consultar con el administrador.\n"
#define	M4_ERR_PDU_GET_NEXT_SIZE_BLOCK			0x004B + M4_SRV_API_CLIENTE_ERROR
// "Error construyendo PDU.\n \n Se ha producido un error al añadir un nuevo bloque a la PDU.\nDebe consultar con el administrador.\n"
#define	M4_ERR_PDU_ADD_BLOCK					0x004C + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función InternetSetOption.\n \nError al intentar establecer el parámetro %0:s: %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_INTERNET_SET_OPTION				0x004D + M4_SRV_API_CLIENTE_ERROR
// "Error al ejecutar la función HttpQueryInfo.\n \nError al intentar obtener información (%0:s) de la cabecera de la petición HTTP: %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_HTTP_QUERY_INFO					0x004E + M4_SRV_API_CLIENTE_ERROR
// "Error al intentar recuperar información de la cabecera HTTP.\n \nError al intentar obtener información asociada a la cabecera de la petición.\nDebe consultar con el administrador.\n"
#define M4_ERR_CHECK_HEADER_INFORMATION			0x004F + M4_SRV_API_CLIENTE_ERROR
// "Se ha obtenido un error HTTP_STATUS_SERVER_ERROR.\n \nSe ha obtenido un error HTTP_STATUS_SERVER_ERROR como respuesta a un petición HTTP: %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_HTTP_STATUS_SERVER_ERROR			0x0050 + M4_SRV_API_CLIENTE_ERROR
// "Error ejecutando la función InternetSetStatusCallback.\n \nError al intentar establecer una función de callback asíncrona: %0:s\nDebe consultar con el administrador.\n"
#define M4_ERR_INTERNET_SET_STATUS_CALLBACK		0x0051 + M4_SRV_API_CLIENTE_ERROR
// Error iniciando comunicaciones.\n \nSe ha producido un error al establecer la conexión con la pasarela HTTP "%0:s" en el puerto "%1:d" a través de una conexión SSL.\nConsulte con su administrador si dicha pasarela se encuentra iniciada y es accesible desde este sistema.\n"
#define M4_ERR_SENDING_TO_GATEWAY_SSL			0x0052 + M4_SRV_API_CLIENTE_ERROR
// "Error iniciando comunicaciones.\n \nSe ha producido un error al establecer la conexión con la pasarela HTTP "%0:s" en el puerto "%1:d" a través de una conexión no SSL.\nConsulte con su administrador si dicha pasarela se encuentra iniciada y es accesible desde este sistema.\n"
#define M4_ERR_SENDING_TO_GATEWAY_NO_SSL		0x0053 + M4_SRV_API_CLIENTE_ERROR
// "Se ha producido un error por timeout.\n \nSe ha obtenido un error en el gateway por timeout como respuesta a un petición HTTP: %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_M4GATEWAY_SOCKET_TIMEOUT			0x0054 + M4_SRV_API_CLIENTE_ERROR

// "Se la producido un error arrancando la aplicación monitor "%0:s".\nPara mayor información consulte el resto de mensajes."
#define M4_ERR_STARTING_MIND_MONITOR			0x0070 + M4_SRV_API_CLIENTE_ERROR
// "La aplicación monitor "%0:s" se inició satisfactoriamente."
#define M4_DEB_START_MIND_MONITOR_SUCCESS		0x0071 + M4_SRV_API_CLIENTE_ERROR
// "Se la producido un error cerrando la aplicación monitor "%0:s".\nPara mayor información consulte el resto de mensajes."
#define M4_ERR_STOPPING_MIND_MONITOR			0x0072 + M4_SRV_API_CLIENTE_ERROR
// "Se ha producido el error "%0:d" durante la ejecución de "%1:s": %2:s\nDebe consultar con el administrador."
#define M4_ERR_CODE_DESC_WIN_FUNC_ERROR			0x0073 + M4_SRV_API_CLIENTE_ERROR
// "Se la producido un error comprobando si la aplicación monitor "%0:s" se encuentra en ejecución.\nPara garantizar su funcionalidad Se iniciará de nuevo. Para mayor información consulte el resto de mensajes."
#define M4_ERR_CHECKING_MIND_MONITOR_RUNNING	0x0074 + M4_SRV_API_CLIENTE_ERROR
// "La aplicación monitor "%0:s" no se encuentra en ejecución.\nPara garantizar su funcionalidad Se iniciará de nuevo."
#define M4_ERR_MIND_MONITOR_STATUS_CLOSED		0x0075 + M4_SRV_API_CLIENTE_ERROR
// "Procediendo a cerrar la sesión del usuario en el servidor de aplicaciones"
#define M4_DEB_SELF_CLOSING_SESSION				0x0076 + M4_SRV_API_CLIENTE_ERROR
// "Delegando el cierre de la sesion en la aplicación monitor."
#define M4_DEB_DELEGATE_CLOSING_SESSION			0x0077 + M4_SRV_API_CLIENTE_ERROR
// "Error recuperando la sesión de usuario durante el inicio de la aplicación monitor. La aplicación monitor no se iniciará.\nDebe consultar con el administrador."
#define M4_ERR_GETTING_USER_SESSION				0x0078 + M4_SRV_API_CLIENTE_ERROR
// "Error encriptando los parámetros de la aplicación. La aplicación monitor no se iniciará.\nDebe consultar con el administrador."
#define M4_ERR_ENCRYPTING_PARAMS				0x0079 + M4_SRV_API_CLIENTE_ERROR
// "La aplicación monitor "%0:s" no se encuentra ni en el directorio actual ("%1:s") ni en el de la instalación por defecto de la aplicación ("%2:s"). La aplicación monitor no se iniciará.\nDebe consultar con el administrador."
#define M4_ERR_MIND_MONITOR_EXE_NOT_FOUND		0x007A + M4_SRV_API_CLIENTE_ERROR
// "Error lanzando la aplicación monitor desde la ruta "%0:s".\nDebe consultar con el administrador."
#define M4_ERR_CAN_NOT_LAUNCH_MIND_MONITOR_EXE	0x007B + M4_SRV_API_CLIENTE_ERROR
// "La aplicación monitor se iniciará desde la ruta "%0:s"."
#define M4_DEB_PATH_MIND_MONITOR				0x007C + M4_SRV_API_CLIENTE_ERROR

// "Incompatibilidad de versiones.\n \nLa versión actual de la tecnología no es compatible con la versión del servidor al que se conecta.\nDebe consultar con el administrador."
#define M4_ERR_MISMATCHED_VERSIONS				0x0090 + M4_SRV_API_CLIENTE_ERROR
// "Incompatibilidad de versiones.\n \nLa versión actual de la tecnología "%0:s", no es compatible con la versión del servidor "%1:s" al que se conecta.\nDebe consultar con el administrador."
#define M4_ERR_MISMATCHED_VERSIONS_VERBOSE		0x0091 + M4_SRV_API_CLIENTE_ERROR
// "Incompatibilidad de codificación de caracteres.\n \nLa codificación de caracteres actual de la tecnología no es compatible con la codificación de caracteres del servidor al que se conecta.\nDebe consultar con el administrador."
#define M4_ERR_MISMATCHED_ENCODINGS				0x0092 + M4_SRV_API_CLIENTE_ERROR

//-----------------------------------------------------------------------------
//									M4EXEFORCHANNEL
//-----------------------------------------------------------------------------

// see m4exeforchannel/version/inc/m4srvefcres.hpp
// PROXY's
#define M4_ERR_PROXY_OUT_OF_MEMORY				0x0000 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_CACHE					0x0001 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_ENGINE_PARAM				0x0002 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_PXSPACE_CACHE			0x0003 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_SUBSYS					0x0004 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_OBJDIR_DESER				0x0005 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_OBJDIR_SER					0x0006 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_CREATE_CHANNEL				0x0007 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_BUILD_CHANNEL				0x0008 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_IOD_WRITE					0x0009 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_IOD_READ					0x000A + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_REL_SPACE					0x000B + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_GET_PXSPACE				0x000C + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_WRONG_STATE				0x000D + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_PX_MANAGER				0x000E + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_EXECUTOR					0x000F + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_SESSION_CHANNEL			0x0010 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_BUILD_PXSPACE				0x0011 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_INF_PROXY_GENERIC_0					0x0012 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_EXEC_COMMAND				0x0013 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_DEBUG_VIEW					0x0014 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_GET_MAXNSPACE				0x0015 + M4_SRV_EXE_FOR_CHANNEL_BASE
// Parece que hay mensajes en el logmsg.ini que no están aquí. Los respeto
#define M4_ERR_PROXY_NO_SE_BIEN_1				0x0016 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_SE_BIEN_2				0x0017 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_SE_BIEN_3				0x0018 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_SE_BIEN_4				0x0019 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_BAD_DEBUG_VIEW_VERSION		0x001A + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_SUCH_M4OBJ				0x001B + M4_SRV_EXE_FOR_CHANNEL_BASE

//-----------------------------------------------------------------------------
//									SECURITY
//-----------------------------------------------------------------------------

#define M4_ERR_UUSS_USERNAME					0x0000 + M4_SRV_UUSS_ERROR_BASE //	Username not in Database
#define M4_ERR_UUSS_PASSWORD					0x0001 + M4_SRV_UUSS_ERROR_BASE //	Password not valid
#define M4_ERR_UUSS_MAX_CONN					0x0002 + M4_SRV_UUSS_ERROR_BASE //	Max number of connections reached
#define M4_ERR_UUSS_ADM_LOCK					0x0003 + M4_SRV_UUSS_ERROR_BASE //	User locked by the administrator
#define M4_ERR_UUSS_MAX_RET						0x0004 + M4_SRV_UUSS_ERROR_BASE //	Too many attempts to connect
#define M4_ERR_UUSS_PWD_EXP						0x0005 + M4_SRV_UUSS_ERROR_BASE //	Password Expired
#define M4_ERR_SCH_INITIALIZING					0x0006 + M4_SRV_UUSS_ERROR_BASE //	Initialization error
#define M4_ERR_SCH_RELEASING					0x0007 + M4_SRV_UUSS_ERROR_BASE //	Error while releasing resources
#define M4_ERR_SCH_DESERIAL						0x0008 + M4_SRV_UUSS_ERROR_BASE //	Error while deserializating channel
#define M4_ERR_SCH_EXECUTE						0x0009 + M4_SRV_UUSS_ERROR_BASE //	Error while deserializating channel
#define M4_ERR_SCH_SERIAL						0x000A + M4_SRV_UUSS_ERROR_BASE //	Error while deserializating channel
#define M4_ERR_UUSS_UNKNOWN						0x0054 + M4_SRV_UUSS_ERROR_BASE //	Unknown

// m4cacert
// "Error getting Meta4 private key for certificate signature" 
#define M4_ERR_CACERT_KEY_NOT_INI				0x0155 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error serializing key"
#define M4_ERR_CACERT_SERIAL_KEY				0x0156 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "No Authenticator provided"
#define M4_ERR_CACERT_NOT_AUTHENTICATOR			0x0157 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error deserializing repository private key" 
#define M4_ERR_CACERT_KEY_DESERIALIZE_ERROR		0x0158 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "No user name for certificate generation"
#define M4_ERR_CACERT_NO_USER_NAME				0x0159 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error getting keys from repository"
#define M4_ERR_CACERT_NOT_KEYS_IN_REPOSITORY	0x0160 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// UNKNOWN
#define M4_ERR_CACERT_CERT_SIGN_ERROR			0x0161 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error adding user name to certificate" 
#define M4_ERR_CACERT_ADD_USER_NAME				0x0162 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error verifying just created certificate"
#define M4_ERR_CACERT_VERIFY					0x0163 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error serializing certificate"
#define M4_ERR_CACERT_SERIALIZE_CERT			0x0164 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error encoding certificate to base64 format"
#define M4_ERR_CACERT_ENCODING_CERT				0x0165 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error decoding certificate from base64 format"
#define M4_ERR_CACERT_DECODING_CERT				0x0166 + M4_SRV_UUSS_ERROR_BASE //	Unknown
// "Error deserializing certificate"
#define M4_ERR_CACERT_DESERIALIZE_CERT			0x0167 + M4_SRV_UUSS_ERROR_BASE //	Unknown

// "Error ejecutando la función de exportación.\n \nNo se ha podido exportar el fichero de claves %0:s. Error interno: %1:s\nCompruebe directorio, nombre y permisos.\n"
#define M4_ERR_CACERT_CERT_EXPORT_FAIL			0x0168 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nEl certificado no es válido.\nCompruebe el certificado.\n"
#define M4_ERR_CACERT_INVALID_M4CERT			0x0169 + M4_SRV_UUSS_ERROR_BASE
// "Error validando certificado.\n \nEl certificado no es válido.\nCompruebe otros errores:%0:s.\n"
#define M4_ERR_CACERT_CHECK_CERT_FAIL			0x016A + M4_SRV_UUSS_ERROR_BASE
// "Error validando certificado.\n \nNo se ha encontrado el token del certificado.\nConsulte con su administrador.\n"
#define M4_ERR_CACERT_CHECK_NO_CERT_TOKEN		0x016B + M4_SRV_UUSS_ERROR_BASE
// "Error validando certificado.\n \nEl usuario en el certificado no es válido.\nConsulte con su administrador.\n"
#define M4_ERR_CACERT_INVALID_M4CERT_USER		0x016C + M4_SRV_UUSS_ERROR_BASE
// "Error validando certificado.\n \nEl tiempo de expiración en el certificado no es válido.\nConsulte con su administrador.\n"
#define M4_ERR_CACERT_INVALID_M4CERT_TIME		0x016D + M4_SRV_UUSS_ERROR_BASE
// "Error validando certificado.\n \nEl certificado ha expirado o los reloges de máquina origen y destino están desincronizados.\nConsulte con su administrador.\n"
#define M4_ERR_CACERT_INVALID_M4CERT_EXPIRED	0x016E + M4_SRV_UUSS_ERROR_BASE

// "Error validando certificado.\n \nError decodificando el identificador de certificado a partir del valor en Base64.\nConsulte con su administrador.\n"
#define M4_ERR_CACERT_ERROR_DECODING_SERIAL		0x016F + M4_SRV_UUSS_ERROR_BASE
// "Error actualizando claves.\n \nNo ha sido posible determinar el identificador de certificado.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_CANNOT_GET_CERTIFICATE_ID	0x0170 + M4_SRV_UUSS_ERROR_BASE
// "Información de depuración.\n \nNo se ha podido añadir la información de las claves al mapa.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_WARN_ADD_KEYS_TO_MAP		0x0171 + M4_SRV_UUSS_ERROR_BASE
// "Error actualizando claves.\n \nNo ha sido posible actualizar las claves en el mapa.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_UPDATING_KEYS_IN_MAP		0x0172 + M4_SRV_UUSS_ERROR_BASE
// "Error validando certificado.\n \nExiste un desajuste entre el modelo multidominio y el clásico.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_KEY_SOURCE_MISMATCH		0x0173 + M4_SRV_UUSS_ERROR_BASE
// "Error validando certificado.\n \nSe ha proporcionado un certificado con información inconsistente.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_DOMAIN_MISMATCH			0x0174 + M4_SRV_UUSS_ERROR_BASE
// "Error creando certificado.\n \nError insertando el nombre de dominio %0:s en el certificado.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_ADD_DOMAIN_NAME			0x0175 + M4_SRV_UUSS_ERROR_BASE
// "Error creando certificado.\n \nEl nombre del dominio está vacío.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_EMPTY_DOMAIN				0x0176 + M4_SRV_UUSS_ERROR_BASE
// "Error creando certificado.\n \nSe ha escogido una fecha que genera un certificado sin validez.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_WRONG_CERT_DURATION		0x0177 + M4_SRV_UUSS_ERROR_BASE
// "Error validando certificado.\n \nCertificado sin clave de encriptación. Un certificado de esta versión debe incluir la clave criptográfica.\nConsulte con su administrador.\n""
#define M4_ERR_CACERT_CHECK_NO_KEY_TOKEN		0x0178 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nLa clave enviada en el certificado no es válida.\nCompruebe el certificado.\n"
#define M4_ERR_CACERT_INVALID_KEY				0x0179 + M4_SRV_UUSS_ERROR_BASE

// m4cacertmngr

// "Error ejecutando la función %0:s.\n \nLogon Adapter no válido.\nDebe consultar con el administrador.\n"
#define M4_ERR_CACERTMNGR_INVALID_LA			0x0200 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nItem de ejecución no válido.\nDebe consultar con el administrador.\n"
#define M4_ERR_CACERTMNGR_INVALID_EXEC_ITEM		0x0201 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nNúmero inválido de argumentos. Número de argumentos esperado es %1:d y se han recibido %2:d.\nDebe consultar con el administrador.\n"
#define M4_ERR_CACERTMNGR_INVALID_N_ARGS		0x0202 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nParámetro nombre de fichero de claves es inválido.\nCompruebe el nombre del fichero.\n"
#define M4_ERR_CACERTMNGR_INVALID_KEY_FILE		0x0203 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nParámetro clave de fichero de claves es inválido.\nCompruebe el parámetro.\n"
#define M4_ERR_CACERTMNGR_INVALID_KEY_PASS		0x0204 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nNo se ha podido exportar el fichero de claves.\nCompruebe otros errores.\n"
#define M4_ERR_CACERTMNGR_CERT_EXPORT_FAIL		0x0205 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nError validando certificado.\nCompruebe el certificado y el usuario.\n"
#define M4_ERR_CACERTMNGR_INVALID_M4CERT_CHECK	0x0206 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nEl usuario de aplicación no es válido.\nCompruebe el usuario.\n"
#define M4_ERR_CACERTMNGR_INVALID_USER_ARG		0x0207 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nEl parámetro certificado no es válido.\nCompruebe el parámetro.\n"
#define M4_ERR_CACERTMNGR_INVALID_M4CERT_ARG	0x0208 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nEl parámetro 3 de salida certificado no es válido. Debe ser de tipo entero o nulo.\nCompruebe el parámetro.\n"
#define M4_ERR_CACERTMNGR_INVALID_ISVALID_ARG	0x0209 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nEl usuario no tiene permisos para ejecutar esta acción. Se exige el rol %1:s.\nCompruebe su rol.\n"
#define M4_ERR_CACERTMNGR_NOT_AUTHORIZED		0x020A + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nParámetro id certificado no es inválido.\nCompruebe el parámetro.\n"
#define M4_ERR_CACERTMNGR_INVALID_CERTIFICATE_ID 0x020B + M4_SRV_UUSS_ERROR_BASE
// "Error renovando el certificado.\n \nNo ha sido posible actualizar las claves nuevas.\nConsulte con su administrador.\n"
#define M4_ERR_CACERTMNGR_UPDATE_KEYS			0x020C + M4_SRV_UUSS_ERROR_BASE
// "Error eliminando el certificado.\n \nNo ha sido posible eliminar las claves antiguas.\nConsulte con su administrador.\n"
#define M4_ERR_CACERTMNGR_DELETE_KEYS			0x020D + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función %0:s.\n \nNo se ha podido generar las claves.\nCompruebe otros errores.\n"
#define M4_ERR_CACERTMNGR_GENERATE_KEYS			0x020E + M4_SRV_UUSS_ERROR_BASE

// "Error ejecutando la función "%0:s".\n \nEl parámetro clave secreta no es válido."
#define M4_ERR_CACERTMNGR_INVALID_SECRET_KEY_ARG		0x020F + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función "%0:s".\n \nEl parámetro código de verificación no es válido."
#define M4_ERR_CACERTMNGR_INVALID_VERIFICATION_CODE_ARG	0x0210 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función "%0:s".\n \nEl parámetro intervalo de tiempo no es válido."
#define M4_ERR_CACERTMNGR_INVALID_INTERVAL_ARG			0x0211 + M4_SRV_UUSS_ERROR_BASE
// "Error ejecutando la función "%0:s".\n \nEl parámetro ticket no es válido."
#define M4_ERR_CACERTMNGR_INVALID_TICKET_ARG			0x0212 + M4_SRV_UUSS_ERROR_BASE
// "Ticket de generación de clave secreta incorrecto.\n \nEl ticket para la generación de clave secreta del doble factor de autenticación tiene un formato incorrecto.\nConsulte con su administrador."
#define M4_ERR_CACERTMNGR_INVALID_TICKET				0x0213 + M4_SRV_UUSS_ERROR_BASE
// "Ticket de generación de clave secreta obsoleto.\n \nEl ticket para la generación de clave secreta del doble factor de autenticación del usuario "%0:s" ha caducado por tiempo.\nConsulte con su administrador."
#define M4_ERR_CACERTMNGR_OLD_DATED_TICKET				0x0214 + M4_SRV_UUSS_ERROR_BASE
// "Error encriptando datos.\n \nNo se han podido encriptar los datos en la función "%0:s"."
#define M4_ERR_CACERTMNGR_ENCRYPT_ERROR					0x0215 + M4_SRV_UUSS_ERROR_BASE
// "Error desencriptando datos.\n \nNo se han podido desencriptar los datos en la función "%0:s"."
#define M4_ERR_CACERTMNGR_DECRYPT_ERROR					0x0216 + M4_SRV_UUSS_ERROR_BASE



//----------------------------------------------------------------------------
//                                 LDBSUBSYSTEM
//----------------------------------------------------------------------------

#define M4_ERR_LDBSUB_POINTERNULL               0x0000 + M4_SRV_LDBSUB_ERROR_BASE // El puntero que hemos obtenido es nulo
#define M4_ERR_LDBSUB_INITENV                   0x0001 + M4_SRV_LDBSUB_ERROR_BASE // No se puede inicializar el enviroment de la máquina virtual
#define M4_ERR_LDBSUB_INITLDB                   0x0002 + M4_SRV_LDBSUB_ERROR_BASE // No se ha podido inicializar la BDL
#define M4_ERR_LDBSUB_ENDLDB                    0x0003 + M4_SRV_LDBSUB_ERROR_BASE // No se ha podido finalizar la BDL
#define M4_ERR_LDBSUB_ENDENV                    0x0004 + M4_SRV_LDBSUB_ERROR_BASE // No se ha podido finalizar el enviroment de la máquina virtual

//----------------------------------------------------------------------------
//                                 EVENT
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//                                 XML ADMIN: CLIENT
//----------------------------------------------------------------------------

#define M4_ERR_XMLADMCLNT_OPENCONNECT			0x0010 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_WRITEMSG				0x0011 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_FAILLISTEN			0x0012 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_FAILCLOSE				0x0013 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOMONITORPARAM		0x0014 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOCOMMANDPARAM		0x0015 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOCOMMANDANSWER		0x0016 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOFREEMEMORY			0x0017 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOINITXMLLIB			0x0018 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_XMLEXCEPTION			0x0019 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_XMLPARSEERROR			0x001A + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOXMLANSWER			0x001B + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_XMLANSWERERROR		0x001C + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOINITCONNECTPARAM	0x001D + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_COMMUNICALAYERFAIL	0x001E + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMS	0x001F + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NODPCHCOMMANDPARAMSBIS	0x0020 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NODPCHCOMMAND			0x0021 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NODPCHCOMMANDBIS		0x0022 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOCOMMANDANSWERMATCH	0x0023 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOXMLANSWERPARAM		0x0024 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOINSERTCOMMENT		0x0025 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOMAKEELEMENT			0x0026 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOELEMENTATTRIBUTE	0x0027 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_NOCLOSEELEMENT		0x0028 + M4_SRV_ADMINISTRATOR_ERROR_BASE
#define M4_ERR_XMLADMCLNT_AUTHENTICATION		0x0029 + M4_SRV_ADMINISTRATOR_ERROR_BASE

//----------------------------------------------------------------------------
//                                 ROLLING FILE
//----------------------------------------------------------------------------
#define M4_ERR_ROLLING_FILE_NO_VALID_FILE_PATH		0x0000 + M4_SRV_SERVERBASE_ERROR_BASE
#define M4_ERR_ROLLING_FILE_BACKUP_ERROR			0x0001 + M4_SRV_SERVERBASE_ERROR_BASE
#define M4_ERR_ROLLING_FILE_CANNOT_OPEN_FILE		0x0002 + M4_SRV_SERVERBASE_ERROR_BASE
#define M4_ERR_ROLLING_FILE_RENAME_ERROR			0x0003 + M4_SRV_SERVERBASE_ERROR_BASE
#define M4_ERR_ROLLING_FILE_DELETE_ERROR			0x0004 + M4_SRV_SERVERBASE_ERROR_BASE
#define M4_ERR_ROLLING_FILE_MAX_FILE_NUM_WARN		0x0005 + M4_SRV_SERVERBASE_ERROR_BASE
#define M4_ERR_ROLLING_FILE_MAX_FILE_SIZE_WARN		0x0006 + M4_SRV_SERVERBASE_ERROR_BASE
#define M4_ERR_TSAP_NO_OPEN_SOCKECT					0x0007 + M4_SRV_SERVERBASE_ERROR_BASE


//----------------------------------------------------------------------------
//                                 AUDITORY
//----------------------------------------------------------------------------
#define M4_ERR_AUDIT_OPEN_SESSION					0x0000 + M4_AUDIT_ERROR_BASE
#define M4_ERR_AUDIT_CLOSE_SESSION					0x0001 + M4_AUDIT_ERROR_BASE




#endif
