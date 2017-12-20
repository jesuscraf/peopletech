//## begin module%37FB10550378.cm preserve=no
//## end module%37FB10550378.cm

//## begin module%37FB10550378.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%37FB10550378.cp

//## Module: eStatus%37FB10550378; Package specification
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Source file: D:\Channel\M4Appctl\inc\estatus.hpp

#ifndef estatus_h
#define estatus_h 1

//## begin module%37FB10550378.additionalIncludes preserve=no
//## end module%37FB10550378.additionalIncludes

//## begin module%37FB10550378.includes preserve=yes
//## end module%37FB10550378.includes

//## begin module%37FB10550378.declarations preserve=no
//## end module%37FB10550378.declarations

//## begin module%37FB10550378.additionalDeclarations preserve=yes
#define M4APPCTL_CONFIGURATION_FILE  "../conf/m4appctl.conf"
#define M4APPCTL_CONFIG_RELATIVE_PATH  "/conf/m4appctl.conf"

#ifdef _WINDOWS
	#define M4APPCTL_BIN_RELATIVE_DIR  "\\bin\\"
#endif


#define GLOBAL_SECTION			"global"
//#define CONTROL_SERVICE_PORT	"ctl_service_port"
#define ADMIN_SERVICE_PORT		"admin_service_port"
#define CONTROL_PORT			"communication_control_port"
#define CONF_FILE				"cnf"
#define LAUNCHER_PATH			"launcher_path"
//#define M4APPSERVER_LIST		"m4appserver_list"
//#define TIMEOUT_STARTUP			"timeout_startup"
//#define TIMEOUT_SHUTDOWN		"timeout_shutdown"
//#define WAIT_CONNECT_TIME		"wait_connect_time"
#define PROCESS_NAME			"name"
//#define CRITICAL				"critical"
#define NOT_INITIALIZED  -1
#define LABEL  "label"
#define CNF  "cnf"
#define PROCESS_LIST  "process_list"
#define HOST "localhost"
#define AUTOMATIC_STARTUP  "automatic_startup"
#define LAUNCHER  "launcher_path"
#define CONTROL_BYTE 4
#define APP_INFO_MAX_BUFFER 4096
#define LOGSYS_FILE "logsys_file"
#define TEMP_STATUS_FILE "temp_status_file"
#define ARGV "argv"
#define SERVER_STARTUP "server_startup"
#define SITE_ID	"site_id"

//## end module%37FB10550378.additionalDeclarations


//## begin eStatus%37FB0F9E0068.preface preserve=yes
//## end eStatus%37FB0F9E0068.preface

//## Class: eStatus%37FB0F9E0068
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {STOPPED=1,STARTING_UP,STOPPING,RUNNING} eStatus;

//## begin eStatus%37FB0F9E0068.postscript preserve=yes
//## end eStatus%37FB0F9E0068.postscript

//## begin eActionAfterShutDown%380F15C50244.preface preserve=yes
//## end eActionAfterShutDown%380F15C50244.preface

//## Class: eActionAfterShutDown%380F15C50244
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {RETRY=1,RETRY_AFTER_WAIT,CRITICAL_ERROR} eActionAfterShutDown;

//## begin eActionAfterShutDown%380F15C50244.postscript preserve=yes
//## end eActionAfterShutDown%380F15C50244.postscript

//## begin module%37FB10550378.epilog preserve=yes
//## end module%37FB10550378.epilog


#endif
