
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4lgadapdef.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                15-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene los defines del logon adaptor
//
//
//==============================================================================




#ifndef __M4LGADAPDEF_HPP__
#define __M4LGADAPDEF_HPP__


#define	M4CL_MAX_CREDENTIAL_SIZE					255

#define	M4_MAX_ARCH_SIZE							128

#define M4_SESSION_CHANNEL_ID						"SCH_SESSION"
#define ROOT_NODE_SESSION_CHANNEL					"ROOT_SESSION"
#define ROLES_NODE_SESSION_CHANNEL					"APP_ROLE_SESSION"

#define SUPONGO_QUE_SOLO_HAY_UN_REGISTRO			0

#define M4_ITEM_USER								"ID_APP_USER"
#define M4_ITEM_LANGUAGE							"LANGUAGE"
#define M4_ITEM_DEV_LANGUAGE						"DEV_LANGUAGE"
#define M4_ITEM_CURRENT_ROLE_ID						"ID_APP_ROLE"
#define M4_ITEM_CURRENT_ROLE_RSM					"ID_RSM"
#define M4_ITEM_ID_KITUSER							"ID_KITUSER"

#define	M4_ITEM_ROLES_N_ROLE						"N_APP_ROLE"
#define	M4_ITEM_ROLES_ID_ROLE						"ID_APP_ROLE"
#define M4_ITEM_ROLES_RSM							"ID_RSM"
#define M4_ITEM_ROLES_IS_DEFAULT					"IS_DEFAULT_ROLE"

#define M4_ITEM_GMT_DELAY							"GMT"
#define M4_ITEM_ID_ORGANIZATION						"ID_ORGANIZATION"
#define M4_ITEM_LICENSE								"LICENSE"
#define M4_ITEM_IS_MULTIORG							"IS_MULTIORGANIZATION"
#define M4_ITEM_ROUND_CURRENCY						"ROUND_CURRENCY"
#define M4_ITEM_ROUND_NUMBER						"ROUND_NUMBER"
#define M4_ITEM_EXECUTE_REALSQL_MODE				"EXECUTE_REALSQL_MODE"
#define M4_ITEM_NUM_ROWS_DB_LIMIT					"NUM_ROWS_DB_LIMIT"

#define M4_MCH_START_DATE							"META_DATA_START_DATE"
#define M4_MCH_END_DATE								"META_DATA_END_DATE"
#define M4_MCH_CORR_DATE							"META_DATA_CORRECTION_DATE"

#define M4_LDB_START_DATE							"DATA_START_DATE"
#define M4_LDB_END_DATE								"DATA_END_DATE"
#define M4_LDB_CORR_DATE							"DATA_CORRECTION_DATE"

#define M4_VM_START_DATE							"EXECUTION_START_DATE"
#define M4_VM_END_DATE								"EXECUTION_END_DATE"
#define M4_VM_EXEC_DATE								"EXECUTION_DATE"

#define M4_LOGON_MECHANISM							"LOGON_MECHANISM" //bg 102349

// User actions auditory.
#define M4_ITEM_USR_AUDIT_ACTIVE					"USR_AUDIT_ACTIVE"
#define M4_ITEM_USR_AUDIT_CLIENT_MACHINE			"USR_AUDIT_CLIENT_MACHINE"
#define M4_ITEM_USR_AUDIT_SESSION_DATE				"USR_AUDIT_SESSION_DATE"
#define M4_ITEM_USR_AUDIT_CLIENT_TYPE				"USR_AUDIT_CLIENT_TYPE"
#define M4_ITEM_USR_TIME_AUDIT_RC_THRESHOLD			"USR_TIME_AUDIT_RC_THRESHOLD"
#define M4_ITEM_USR_AUDIT_CLIENT_TYPE				"USR_AUDIT_CLIENT_TYPE"

#define M4_ITEM_USER_ALIAS							"APP_USER_ALIAS"
#define M4_ITEM_DEBUG_USER							"ID_DEBUG_USER"

#define M4_ITEM_SYSTEM_INFO							"SYSTEM_INFO"

#define M4_ITEM_SRV_COM_APPS_LOCKED					"SRV_COM_APPS_LOCKED"

#define M4_ITEM_USR_AUDIT_CLIENT_PUB_IP				"USR_AUDIT_CLIENT_PUB_IP"

// CRG Protocol parameters definitions
#define TRANSP_PROT_TYPE_TAG                       "TransportProtocolType"
#define TRANSP_PROT_TYPE_VALUE_M4                  "M4"
#define TRANSP_PROT_TYPE_VALUE_M4_NUM               0
#define TRANSP_PROT_TYPE_VALUE_HTTP                "HTTP"
#define TRANSP_PROT_TYPE_VALUE_HTTP_NUM            1
#define DEFAULT_TRANSP_PROT_TYPE_VALUE             TRANSP_PROT_TYPE_VALUE_M4_NUM
#define DEFAULT_SSL_FILE                           "Cacert.pem"

// Compression

#define TRANSP_PROT_COMPRES_TYPE_TAG               "CompressionType" // Values from 0 to 2
#define TRANSP_PROT_COMPRES_TYPE_VALUE_NONE        "None"
#define TRANSP_PROT_COMPRES_TYPE_VALUE_NONE_APIC    M4_COMPRESSION_TYPE_NULL
#define TRANSP_PROT_COMPRES_TYPE_VALUE_CLASSIC     "Classic" 
#define TRANSP_PROT_COMPRES_TYPE_VALUE_CLASSIC_APIC    M4_COMPRESSION_TYPE_CLASSIC
#define TRANSP_PROT_COMPRES_TYPE_VALUE_ZDELTA      "Zdelta" 
#define TRANSP_PROT_COMPRES_TYPE_VALUE_ZDELTA_APIC M4_COMPRESSION_TYPE_ZDELTA
#define DEFAULT_TRANSP_PROT_COMPRES_TYPE_VALUE     TRANSP_PROT_COMPRES_TYPE_VALUE_NONE_APIC

// compression level is deprecated. Set default to high compression.
/*
#define TRANSP_PROT_COMPRES_LEVEL_TAG              "CompressionLevel" // Values from 0 to 2
#define TRANSP_PROT_COMPRES_LEVEL_VALUE_LOW        "Low"
#define TRANSP_PROT_COMPRES_LEVEL_VALUE_LOW_APIC    M4_COMPRESSION_LEVEL_LOW
#define TRANSP_PROT_COMPRES_LEVEL_VALUE_MED        "Medium" 
#define TRANSP_PROT_COMPRES_LEVEL_VALUE_MED_APIC    M4_COMPRESSION_LEVEL_MEDIUM
#define TRANSP_PROT_COMPRES_LEVEL_VALUE_HIGH       "High" 
#define TRANSP_PROT_COMPRES_LEVEL_VALUE_HIGH_APIC   M4_COMPRESSION_LEVEL_HIGH
*/
#define DEFAULT_TRANSP_PROT_COMPRES_LEVEL_VALUE    M4_COMPRESSION_CLASSIC_DEFAULT_LEVEL

// M4 Specific (none for now)

// HTTP Specific
#define TRANSP_PROT_WEB_SERVER_NAME_TAG            "WebServerName" // No default value
#define TRANSP_PROT_WEB_SERVER_PORT_TAG            "WebServerPort"
#define DEFAULT_TRANSP_PROT_WEB_SERVER_PORT        "80"
#define TRANSP_PROT_WEB_SERVER_SECURE_PORT_TAG     "WebServerSecurePort"
#define DEFAULT_TRANSP_PROT_WEB_SERVER_SECURE_PORT "443" // ????
#define TRANSP_PROT_SERVLET_NAME_TAG               "ServletName"
#define DEFAULT_TRANSP_PROT_SERVLET_NAME           "/servlet/M4Servlet" // File name
#define TRANSP_PROT_APP_SERVER_NON_SECURE_PORT_TAG "AppServerNonSecurePort"
#define M4_COOKIE_ID                               "M4Servlet" // Cookie to use
#define HTTP_EXTRA_TIMEOUT                         (5000.0) // In msecs

// Logon type.
#define LOGON_TYPE_TAG								"LogonType" // Values: 0 (M4 Logon), 1 (Kerberos Logon)
// End CRG




#endif

