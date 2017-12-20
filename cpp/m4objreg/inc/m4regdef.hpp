//## begin module%3912A1F501AD.cm preserve=no
//## end module%3912A1F501AD.cm

//## begin module%3912A1F501AD.cp preserve=no
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
//## end module%3912A1F501AD.cp

//## Module: m4regdef%3912A1F501AD; Package specification
//## Subsystem: m4objreg::inc%3912A34F03C7
//## Source file: d:\new\m4objreg\inc\m4regdef.hpp

#ifndef __M4REGDEF_HPP__
#define __M4REGDEF_HPP__ 1

//## begin module%3912A1F501AD.additionalIncludes preserve=no
//## end module%3912A1F501AD.additionalIncludes

//## begin module%3912A1F501AD.includes preserve=yes
//## end module%3912A1F501AD.includes

//## begin module%3912A1F501AD.declarations preserve=no
//## end module%3912A1F501AD.declarations

//## begin module%3912A1F501AD.additionalDeclarations preserve=yes
//## end module%3912A1F501AD.additionalDeclarations


//## begin module%3912A1F501AD.epilog preserve=yes

// ======== DEFINES ============
// Para el REGISTRY

#define M4CH_MAX_CONNECTION_STRING                      500
#define M4CH_MAX_DATA_BASE_PARTNER                      20
#define M4CH_MAX_WORKING_DIRECTORY                      500
#define M4CH_MAX_DEBUG_DIRECTORY						500
#define M4CH_MAX_TEMP_DIRECTORY                         500
#define M4CH_MAX_TEMP_BUFFER                            500
#define M4CH_MAX_VERSION	                            256
#define M4CH_MAX_DB_PWD									256
#define M4CH_MAX_CON_EXEC								500
#define M4CH_MAX_TRANSPORT_PROTOCOL						20
#define M4CH_MAX_DATE_SEPARATOR							20


// Languages information in the registry
#define M4_LANG_ID_REGEDIT_KEY							"id"
#define M4_LANG_LCID_REGEDIT_KEY						"LCID"
#define M4_LANG_NAME_REGEDIT_KEY						"Name"
#define M4_LANG_DEFAULT_REGEDIT_KEY						"IsDefault"
#define M4_LANG_ERR_DATE_REGEDIT_KEY					"ErrorDateFormat"
#define M4_LANG_ERR_TIME_REGEDIT_KEY					"ErrorTimeFormat"


#define M4_INIT_USE_FIELD_UNICODE_TYPES					"UseFieldUnicodeTypes"
#define M4_INIT_CONNECTION_STRING		                "ConnectionString"
#define M4_INIT_VERSION					                "Version"
#define M4_INIT_TIME_UNIT					            "TimeUnit"
#define M4_INIT_WORK_DIR				                "WorkDir"
#define M4_INIT_DEBUG_DIR				                "DebugDir"
#define M4_INIT_SM_OLTP									"SMOLTP"
#define M4_INIT_CACHE_DIR				                "CacheDir"
#define M4_INIT_TEMP_DIR				                "TempDir"
#define M4_INIT_SYSTEM_DEBUG_ENABLE						"SystemDebugEnable"
#define M4_INIT_SYSTEM_DEBUG_DETAIL_LEVEL				"SystemDebugDetailLevel"
#define M4_INIT_SYSTEM_DEBUG_LEVEL						"SystemDebugLevel"
#define M4_INIT_MAX_DEBUG_PERIOD		                "MaxDebugPeriod"
#define M4_INIT_SHOW_DATA								"ShowData"
#define M4_INIT_LOCAL_TIME_CORRECTION					"LocalTimeCorrection"
#define M4_INIT_DB_PWD									"DBPassword"
#define M4_INIT_MAX_LDBINSP_SIZE						"MaxLDBInspSize" // in MB
#define M4_INIT_DDBB									"DataBase"
#define M4_INIT_MAX_NUM_CONN							"MaxNumConn"
#define M4_INIT_MAX_NUM_CONN_KIT						"MaxNumConnKit"
#define M4_INIT_MAX_NUM_CURSOR							"MaxNumCursor"
#define M4_INIT_MAX_NUM_STMT_CACHE						"MaxNumStmtCache"
#define M4_INIT_MAX_NUM_OBJECT							"MaxNumObject"
#define M4_INIT_MAX_NUM_OBJECT_TRANS					"MaxNumObjectTrans"
#define M4_INIT_ISOLATION_LEVEL							"IsolationLevel"
#define M4_INIT_CON_EXEC								"ConExec"
#define M4_INIT_BUFFER_SIZE								"BlobBufferSize"
#define M4_INIT_DEVELOPER_FLAG							"Developer"
#define M4_INIT_MULTIPLE_FILES_TRACE					"MultipleFilesTrace"
#define M4_INIT_CS_DEFAULT_TIMEOUT						"CSDefaultTimeOut"
#define M4_INIT_CS_GENERIC_TIMEOUT						"CSGenericTimeOut"
#define M4_INIT_CS_TRACE_ENABLED						"CSTraceEnabled"
#define M4_INIT_VB_TRACE_ENABLED						"VBTraceEnabled"
#define M4_INIT_CS_TRACE_SHOW_CACHE_OPERATIONS			"CSTraceShowCachedOperations"
#define M4_INIT_CS_LOGON_MECHANISM						"Mechanism"
#define M4_INIT_CS_DLL_MECHANISM						"DllMechanism"
#define M4_INIT_XML_DATE_SEPARATOR						"DateSeparator"

#define M4_INIT_JAVA_HOME								"JavaHome"
#define M4_INIT_JAVA_CLASSPATH							"ClassPath"
#define M4_INIT_JAVA_ARGUEMTS							"JavaArgs"
#define M4_INIT_JAVA_TRACE								"JavaTrace"
#define M4_INIT_JAVA_ALLOW_IN_PROCESS					"AllowInProcess"
#define M4_INIT_JAVA_TIMEOUT							"Timeout"

#define M4_INIT_LDB_CACHE_MODE							"LDBCacheMode"

#define M4_INIT_SM_CACHE_REFRESH						"SMCacheRefresh"

#define M4_INIT_MD_CACHE_MODE							"MDCacheMode"
#define M4_INIT_MD_CACHE_MAX_MEMORY						"MDCacheMaxMemory"				
#define M4_INIT_MD_CACHE_MAX_NUM_OBJECTS				"MDCacheMaxNumObjects"
#define M4_INIT_MD_CACHE_REFRESH_RATIO					"MDCacheRefreshRatio"
#define M4_INIT_MD_CACHE_EXPIRY_RATIO					"MDCacheExpiryRatio"
#define M4_INIT_MD_CACHE_DEFAULT_MAX_PERIOD				"MDCacheDefaultMaxPeriod"

#define M4_INIT_DATA_CACHE_MODE							"DataCacheMode"				
#define M4_INIT_DATA_CACHE_MAX_MEMORY					"DataCacheMaxMemory"				
#define M4_INIT_DATA_CACHE_MAX_NUM_OBJECTS				"DataCacheMaxNumObjects"
#define M4_INIT_DATA_CACHE_REFRESH_RATIO				"DataCacheRefreshRatio"
#define M4_INIT_DATA_CACHE_EXPIRY_RATIO					"DataCacheExpiryRatio"
#define M4_INIT_DATA_CACHE_DEFAULT_MAX_PERIOD			"DataCacheDefaultMaxPeriod"

#define M4_INIT_MACHINE_USER_LICENCE					"UserLicence"
#define M4_INIT_UNICODE									"Unicode"
#define M4_INIT_TRANSPORT_DEFAULT_PROTOCOL				"DefaultProtocol"
#define M4_INIT_TRANSPORT_M4_CONTROLPORT                "ControlPort"
#define M4_INIT_TRANSPORT_M4_COMMWORKERS                "CommWorkers"
#define M4_INIT_TRANSPORT_M4_CONTROLPORT_DEFAULT_VALUE  "6666"
#define M4_INIT_TRANSPORT_M4_COMMWORKERS_DEFAULT_VALUE  "1"

#define M4_INIT_OBJDATA_CACHE_MAX_MEMORY                "ObjDataCacheMaxMemory"				
#define M4_INIT_OBJDATA_CACHE_MAX_NUM_OBJECTS           "ObjDataCacheMaxNumObjects"

// Nueva organización del registry

#define M4_NEW_MACHINE_REGEDIT_KEY						"SOFTWARE\\Meta4\\PeopleNet\\Machine"
#define M4_NEW_LANG_REGEDIT_KEY							"SOFTWARE\\Meta4\\PeopleNet\\Machine\\Languages"
#define M4_NEW_REGEDIT_KEY								"SOFTWARE\\Meta4\\PeopleNet"
#define M4_NEW_TRANSPORT_REGEDIT_KEY					"SOFTWARE\\Meta4\\PeopleNet\\Machine\\TransportProtocol"
#define M4_NEW_TRANSPORT_M4_REGEDIT_KEY                 "SOFTWARE\\Meta4\\PeopleNet\\Machine\\TransportProtocol\\M4"

#define M4_NEW_REGEDIT_KEY_SERVER						"\\APPServer\\"
#define M4_NEW_REGEDIT_KEY_CLIENT						"\\CLIENT\\"

#define M4_NEW_REGEDIT_KEY_CVM							"CVM"
#define M4_NEW_REGEDIT_KEY_CACHE						"CVM\\Cache"
#define M4_NEW_REGEDIT_KEY_CVM_TIME						"CVM\\TIME"
#define M4_NEW_REGEDIT_KEY_CVM_LDB						"CVM\\LDB"
#define M4_NEW_REGEDIT_KEY_CVM_LOG						"CVM\\LOG"
#define M4_NEW_REGEDIT_KEY_CVM_CLIENTSERVER				"CVM\\CLIENTSERVER"
#define M4_NEW_REGEDIT_KEY_TOOLS_XML					"TOOLS\\XML"
#define M4_NEW_REGEDIT_KEY_JAVA							"JAVA"


//## end module%3912A1F501AD.epilog


#endif
