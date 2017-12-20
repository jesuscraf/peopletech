//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ClientAPI
//	 File:              m4props.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				990113
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module.cp



// TAG Defines
	
// Antiguas. Hay que irlas cambiando
	#define M4_PROP_SERV_ARCH		"M4_PROP_SERV_ARCH"
	#define	M4_PROP_MD_COR_DATE		"M4_PROP_MD_COR_DATE"

// Nuevas. Hay que irlas usando

	#define M4_SESSION_LOG						"M4_SESSION_LOG"
	#define M4_SESS_USER_LANGUAGE				"M4_SESS_USER_LANGUAGE"
	#define M4_SESS_USER_ORGANIZATION			"M4_SESS_USER_ORGANIZATION"
	#define M4_SESS_USER_CRYPT2WAY				"M4_SESS_USER_CRYPT2WAY"
	#define M4_SSL_SESSION						"M4_SSL_SESSION"
	#define M4_DEBUG_FILENAME					"M4_DEBUG_FILENAME"
	#define	M4_PROP_SERVER_METADATA_MOD_DATE	"M4_PROP_SERVER_METADATA_MOD_DATE"
	#define M4_PROP_SERVER_ARCHITECTURE			"M4_PROP_SERVER_ARCHITECTURE"
	#define M4_PROP_LDB_VERSION					"M4_PROP_LDB_VERSION"
	#define M4_PROP_SERVER_VERSION				"M4_PROP_SERVER_VERSION"
	#define M4_PROP_SERVER_INTERNAL_VERSION		"M4_PROP_SERVER_INTERNAL_VERSION"
	#define M4_PROP_DEFAULT_ROLE				"M4_PROP_DEFAULT_ROLE"
	#define M4_PROP_SESSION_TIMEOUT				"M4_PROP_SESSION_TIMEOUT"
	#define M4_PROP_SERVER_ENCODING				"M4_PROP_SERVER_ENCODING"

// En obras ....

	#define M4_PROP_LAST_PDU_SENT				"M4_PROP_LAST_PDU_SENT"
	#define M4_PROP_LAST_PDU_RECIEVED			"M4_PROP_LAST_PDU_RECIEVED"

// Para HTTP

	#define M4_HTTP_PROP_SERVLET				"M4_HTTP_PROP_SERVLET"
	#define M4_HTTP_PROP_SERVER					"M4_HTTP_PROP_SERVER"
	#define M4_HTTP_PROP_SERVER_PORT			"M4_HTTP_PROP_SERVER_PORT"
	#define M4_HTTP_PROP_COOKIE					"M4_HTTP_PROP_COOKIE"
	#define M4_HTTP_PROP_COOKIE_ID				"M4_HTTP_PROP_COOKIE_ID"
	#define M4_HTTP_PROP_EXTRA_TIMEOUT			"M4_HTTP_PROP_EXTRA_TIMEOUT"


// Para reconexion
	#define M4_PARAM_COMM_IP					"M4_PARAM_COMM_IP"
	#define M4_PROP_SERVER_IP					"M4_PROP_SERVER_IP"
	#define M4_PROP_SERVER_PORT					"M4_PROP_SERVER_PORT"

	#define M4_PROP_SESSION_ID					"M4_PROP_SESSION_ID"

	#define M4_PROP_SERVER_CONN_ORIENTED		"M4_CONN_ORIENTED"
	#define M4_PROP_SERVER_NON_CONN_ORIENTED	"M4_NON_CONN_ORIENTED"

	#define M4_CLIENT_ID						"M4_CLIENT_ID"
	#define M4_CLIENT_PUB_IP					"M4_CLIENT_PUB_IP"
// Para compresion

	#define M4_COMPRESSION_TYPE					"M4_COMPRESSION_TYPE"
	#define M4_COMPRESSION_LEVEL				"M4_COMPRESSION_LEVEL"

// Credencial de session

	#define M4_PROP_SESSION_CREDENTIAL			"M4_PROP_SESSION_CREDENTIAL"

// Licencia: Estado de Nps

	#define M4_PROP_SERVER_LICENSE_STATE_VALIDATE_EXECUTION_NP	"M4_PROP_SERVER_LICENSE_STATE_VALIDATE_EXECUTION_NP"


// Para el tipo de logon: M4 o Kerberos.

	#define M4_LOGON_TYPE_PROP_NAME						"LogonType"
	#define M4_LOGON_TYPE					             0
	#define KERBEROS_LOGON_TYPE					         1

// Identificar conexiones cliente ligero

	#define M4_SESS_TCLOGIN						"M4_SESS_TCLOGIN"

// Chequeo versión tecnológica

	#define M4_TECH_VERSION						"M4_TECH_VERSION"
	#define M4_TECH_ENCODING					"M4_TECH_ENCODING"

// Soporte alias de ids de usuario

	#define M4_SESS_REAL_USER_ID				"M4_SESS_REAL_USER_ID"
	#define M4_SESS_USER_ALIAS					"M4_SESS_USER_ALIAS"

// Soporte suplantación de usuario

	#define M4_SESS_IMPERSONATOR_USER			"M4_SESS_IMPERSONATOR_USER"

// Establecer la codificación según la del server

	#define M4_SET_CLIENT_ENCODING				"M4_SET_CLIENT_ENCODING"

// Id usuario condificado con el encoding del server

	#define M4_RE_ENCODED_USER_ID				"M4_RE_ENCODED_USER_ID"

	// Establecer la información de la máquina cliente.
	#define M4_SYSTEM_INFO						"M4_SYSTEM_INFO"

	// Establecer tipo de cliente.
	#define M4_CLIENT_TYPE						"M4_CLIENT_TYPE"
	#define M4_CLIENT_TYPE_SM					"0"
	#define M4_CLIENT_TYPE_RC					"1"
	#define M4_CLIENT_TYPE_TC					"2"
	#define M4_CLIENT_TYPE_TC_CERT				"3"