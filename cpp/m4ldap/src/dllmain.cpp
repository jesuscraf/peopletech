//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ldap.dll
// File:             dllmain.cpp
// Project:          OLE Object integration
// Author:           Meta Software M.S. , S.A
// Date:             5/5/99
// Language:         C++
// Operating System: WINDOWS, UNIX 
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

//This DLL keeps the connection with LDAP.
//The structure of the Dll is:
//
//A set of exported functions:
//These func. connect LN4 (MVC) with the Dll. They are written with our beloved C language
//
//
//Here are the functions the end-user can call:
//
//
//


#include "m4types.hpp"
#include "m4stl.hpp"
#include "access.hpp"
#include "execontx.hpp"
#include "m4log.hpp"

#include "dllmain.hpp"
#include "errors.hpp"
#include "m4unicode.hpp"

// API Ldap
#include "ldap.h"

#ifdef _UNIX
#include "m4win2ux.hpp"
#endif


//Estructura del canal. Definiciones de nombre de nodos e items de datos
//Nodo padre, nodo de funciones
#define		M4LDAP_PARENT_NODE			"LDAP"
//Items de operación
#define 	M4LDAP_HANDLE				"HANDLE"		// Property que guarda el handle
#define		M4LDAP_ERRORNUM				"ERRORNUM"
#define		M4LDAP_ERRORDESC			"ERRORDESC"
#define		M4LDAP_TIMEOUT				"TIMEOUT"
#define		M4LDAP_LASTPENTRY			"LASTPROCCESSEDENTRY"
#define		M4LDAP_MAXENTRIES			"MAXENTRIES"
//Items de datos
#define 	M4LDAP_PARENT_DN			"DN"


//Nodo hijo
#define 	M4LDAP_CHILD_NODE			"LDAP_ATTRIBUTE_SET"
//No nos hace falta el conector
//#define 	M4LDAP_CHILD_DN				"DN"
#define 	M4LDAP_CHILD_KEY			"KEY"
#define 	M4LDAP_CHILD_VALUE			"VALUE"

// Definiciones para el canal particular de login LDAP
#define		M4LDAP_LOG_CHNL_CHILD_NODE	"SSC_LOGON_LDAP_PARAM"

// Campos nodo hijo
#define		M4LDAP_REP_PARAM_SFFIX        "USER_DN_SUFFIX"
#define		M4LDAP_REP_PARAM_PFIX         "USER_DN_PREFIX"
#define		M4LDAP_REP_PARAM_ATTR_NAME    "APP_ATTR_NAME"
#define		M4LDAP_REP_PARAM_ATTR_VALUE   "APP_ATTR_VALUE"
#define		M4LDAP_REP_PARAM_APP_IN_CHILD "APP_IN_CHILD_NODE"

#define     LDAP_APP_UNAVAILABLE -22
#define     LDAP_INV_CREDENTIALS -10


//Acceso a la función de decodificación de la clave.
#include "m4cryptc.hpp"
//Acceso a SSL
#include "ldap_ssl.h"

#define	M4CH_DUMMY_B2	"\x2\x3\x8\x1\x9\x6\x5\xF\xD\x8\xF\x8\xB\x4\x7\x7\xF\x6\x4\xE\x4\xE\xE\x5\xB\xE\x6\x7\xF\x3\x7\x2\xB\x6\xC\x4\xD\xB\x6\x7\x8\x3\xE\x8\xA\xB\x7\x9\x6\x6\x7\x9\xB\x6\x4\x8\xE\xF\x3\x8\x8\xC\xA\x0\x4\x2\x6\x8\x4\xC\x4\xD\x9\x2\x8\x4\x8\x8\x6\x8\xF\x1\x1\xF\xC\x1\x7\xE"

//Definiciones para Active directory
#define MSAD_ATTR_USR_PASSWORD "unicodePwd"
#define MSAD_ATTR_USR_ACCOUNT_CTRL "userAccountControl"
#define MSAD_FILTER_USR_ACCOUNT_CTRL "(userAccountControl=*)"
#define MSAD_FLAG_ACCOUNT_DISABLE 0x00000002
#define MSAD_ATTR_USR_ACCOUNT_NAME "sAMAccountName"
#define MSAD_FILTER_ALL "(objectclass=*)"
#define MSAD_ATTR_RDSE_NCONTEXT "RootDomainNamingContext"
#define MSAD_ATTR_DN "distinguishedName"
#define MSAD_FILTER_USR_ACCOUNT_NAME "(sAMAccountName=%s)"


const int M4AUTHENTICODE_MAX_NUM_PARAM = 10;

//Autentifica LDAP	
m4return_t LogonAuthenticode(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4int_t iRes = M4_ERROR;

	//Tendremos 8 argumentos.
	if (ai_iLongArg < M4AUTHENTICODE_MAX_NUM_PARAM) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E_BAD_PARAMETERS, ERRORLOG, "#*s1*#%s#", "INIT");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}	

	// Comprobamos los parámetros
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP HOST");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	if ( ai_pvArg[1].Type != M4CL_CPP_TYPE_NUMBER)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP PORT");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	if (( ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[2].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP USER");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	if (( ai_pvArg[3].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[3].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP PASSWORD");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	//Este argumento me indica si es conexión SSL.
	if ( ai_pvArg[4].Type != M4CL_CPP_TYPE_NUMBER)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP IS SSL CONNECTION");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	if ( ai_pvArg[10].Type != M4CL_CPP_TYPE_NUMBER)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP VERSION");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	m4int_t iIsSSL = (m4int_t) ai_pvArg[4].Data.DoubleData;
	//Comprobamos si la conexión es SSL.
	if (iIsSSL == 1) {
		iRes = _LogonAuthenticodeSSL (ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext);
	}
	else {
		iRes = _LogonAuthenticodeNoSSL (ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext);
	}

	//BG 101798
	if ( iRes != M4_SUCCESS ) 
	{
		// bg 262944 
		// SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "LDAP No authenticated");				
		SetReturnValue ( ai_pvReturn, iRes );
		//99940 
		return M4_SUCCESS;
	}

	//Las llamadas vuelcan en la pila el retorno. Aquí NO debe volcarse nada!!.

	//SetResultValues ( ai_pvContext, 0, "");
	//SetReturnValue (ai_pvReturn, M4_SUCCESS);	
	return M4_SUCCESS;
}


//--------------------------------------------------------------------------
//Decodificación de la password
m4return_t _DecodePasswordTwoWay (m4pcchar_t ai_pPasswordTwoWay, m4pchar_t & ao_pPassword)
{
	m4return_t iRes = M4_ERROR;
	m4int_t	ioLenOut;

	//Ahora decodificamos la password con la clave en que se encriptó.
	//In: Passwd encriptada, Tamaño passwd encriptada,
	//In: Clave, Tamaño clave,
	//Out: Passwd desencriptada.
	//In: Tamaño del buffer de entrada (reservado para la clave decodificada).
	//Out: Tamaño de la clave desencriptada.
	iRes = DecryptTwoWay (	ai_pPasswordTwoWay ,
							strlen(ai_pPasswordTwoWay),
							M4CH_DUMMY_B2 + 23,
							strlen (M4CH_DUMMY_B2 + 23),
							ao_pPassword,
							ioLenOut);
	
	return iRes;

}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//--------------- SSL ------------------------------------------------------
//--------------------------------------------------------------------------
m4return_t _LogonAuthenticodeSSL (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,
	m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext) {

	m4int_t iRes = M4_ERROR;
	int iLength = -1 ;

	m4VariantType pvHandle;
	LDAP * pHandle = NULL;

	m4uint8_t	iVerifyCertificate = 0;	
	m4char_t	sPathDBCertificate[ M4CH_MAX_FILE + 1 ];

	ClVMRunContext*  pvmContext = NULL;

	//Del 0..4 están ya controlados. Pero SSL acepta 8 parámetros.
	
	//Este argumento me indica el puerto de conexión SSL.
	if ( ai_pvArg[5].Type != M4CL_CPP_TYPE_NUMBER)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP SSL PORT CONNECTION (SSL)");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	//Este argumento me indica si se verifican certificados SSL.
	if ( ai_pvArg[6].Type != M4CL_CPP_TYPE_NUMBER)	{		
		// Parametro no válido		
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP SSL VERIFY CERTIFICATE (SSL)");
		SetReturnValue (ai_pvReturn, M4_ERROR);			
		return M4_ERROR;
	}
	else {		
		iVerifyCertificate =  (int) ai_pvArg[6].Data.DoubleData;
	}

	//Este argumento me indica el path donde se encuentran los certificados SSL.
	//Es SSL, no puede llegar nulo, se habrá puesto en el Meta4Object un valor por defecto (WorkDir).
	if (( ai_pvArg[7].Type != M4CL_CPP_TYPE_STRING_VAR)	||
		(ai_pvArg[7].Data.PointerChar[0] == '\0') || (strlen(ai_pvArg[7].Data.PointerChar) == 0) ) {
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP SSL PATH DB CERTIFICATE (SSL)");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}
	else {
		sprintf(sPathDBCertificate, "%s%s", ai_pvArg[7].Data.PointerChar, M4_BACK_SLASH);		
	}

	//Este argumento me indica la password para la base de datos de certificados. (Cuando hay verificación se necesita).
	if (( ai_pvArg[8].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		(( ai_pvArg[8].Data.PointerChar[0] == '\0' ) && (iVerifyCertificate == 1))	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "LDAP PASSWORD DB CERTIFICATES: Password is needed (SSL)");
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP PASSWORD DB CERTIFICATES (SSL)");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	//Este argumento me indica el alias del certificado. (Cuando hay verificación se necesita).
	if (( ai_pvArg[9].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		(( ai_pvArg[9].Data.PointerChar[0] == '\0' ) && (iVerifyCertificate == 1))	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "LDAP ALIAS CERTIFICATE: Alias is needed (SSL)");
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP ALIAS CERTIFICATE (SSL)");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	//----------------------------------------------------------------------
	m4char_t	* pszPassword = NULL;
	iRes = _DecodePasswordTwoWay (ai_pvArg[3].Data.PointerChar, pszPassword);
	if (iRes == M4_ERROR) {
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "DECODING PASSWORD (SSL)");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}
	//----------------------------------------------------------------------

	//Veamos a qué init hay que llamar.
	if (iVerifyCertificate == 0) {
		//----------------------- SIN VERIFICAR CERTIFICADO --------------------
		//Con SSL pero sin verificación de certificados.
		//Path a la BD de certificados		
		if ((iRes = ldapssl_client_init (sPathDBCertificate, NULL)) != LDAP_SUCCESS ) {
			// Error en la llamada a ldap
			delete [] pszPassword;
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "LDAP INITIALIZATING: Can not get init cert db path (SSL No verify)");
			SetReturnValue (ai_pvReturn, M4_ERROR);
			return M4_ERROR;
		}
		//Usuario, Passwd, es SSL.
		if ( (pHandle = (LDAP *) ldapssl_init (ai_pvArg[0].Data.PointerChar, (int)ai_pvArg[5].Data.DoubleData, M4_TRUE)) == NULL ) {
			// Error en la llamada a ldap
			delete [] pszPassword;
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			SETCODEF(M4_LDAP_E_GET_SESSION_HANDLE, ERRORLOG, "LDAP INITIALIZATING: Init (SSL No verify)");
			SetReturnValue (ai_pvReturn, M4_ERROR);
			return M4_ERROR;
		}
		//------------------------------------------------------------------------------
	}
	else {	
		//----------------------- VERIFICANDO CERTIFICADO ------------------------------
		if ((iRes = ldapssl_clientauth_init (sPathDBCertificate, NULL, 1, sPathDBCertificate, NULL)) != LDAP_SUCCESS ) {
			// Error en la llamada a ldap
			delete [] pszPassword;
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "LDAP INITIALIZATING: Client authentication init DB (SSL Verify)");
			SetReturnValue (ai_pvReturn, M4_ERROR);
			return M4_ERROR;
		}
		//Usuario, Passwd, es SSL.
		if ( (pHandle = (LDAP *) ldapssl_init (ai_pvArg[0].Data.PointerChar, (int)ai_pvArg[5].Data.DoubleData, M4_TRUE)) == NULL ) {
			// Error en la llamada a ldap
			delete [] pszPassword;
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			SETCODEF(M4_LDAP_E_GET_SESSION_HANDLE, ERRORLOG, "LDAP INITIALIZATING: Init (SSL Verify)");
			SetReturnValue (ai_pvReturn, M4_ERROR);
			return M4_ERROR;
		}		
		if ((iRes = ldapssl_enable_clientauth (pHandle, "", ai_pvArg[8].Data.PointerChar, ai_pvArg[9].Data.PointerChar)) != LDAP_SUCCESS ) {
			// Error en la llamada a ldap
			delete [] pszPassword;			
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "LDAP INITIALIZATING: Client authentication Certificate Password and Alias (SSL Verify)");
			SetReturnValue (ai_pvReturn, M4_ERROR);
			return M4_ERROR;
		}
		//--------------------------------------------------------------------
	}

	//----------------------------------------------------------------------
	
	m4int_t iVersion = ai_pvArg[10].Data.DoubleData;
	
	if ((iVersion > LDAP_VERSION_MAX) || ( iVersion <= 0)){
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, WARNINGLOG, "#*s1*#%s#", "LDAP PROTOCOL VERSION");
		iVersion = LDAP_VERSION;
	}

	iRes = ldap_set_option( pHandle, LDAP_OPT_PROTOCOL_VERSION, &iVersion );
	if (iRes != LDAP_SUCCESS) {
		delete [] pszPassword;		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SetReturnValue (ai_pvReturn, M4_ERROR);
		iRes = ldap_unbind_s( pHandle );
		return M4_ERROR;
	}
	
	// bg 263557.
	int iTimeOut = 5000;

	iRes = ldap_set_option( pHandle, LDAP_X_OPT_CONNECT_TIMEOUT, &iTimeOut );
	if ( iRes != LDAP_SUCCESS) {
		delete [] pszPassword;		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SetReturnValue (ai_pvReturn, M4_ERROR);
		iRes = ldap_unbind_s( pHandle );
		return M4_ERROR;
	}

	//bg 105255. Pasamos el id usr y la pwd a UTF-8.
	m4pchar_t pcPwd = NULL, pcUsr = NULL;

	// UNICODE UTF8
	iLength = -1 ;
	pcUsr = M4CppToUtf8 ( ai_pvArg[2].Data.PointerChar, iLength );

	if ( pcUsr == NULL || *pcUsr == '\0' )
	{
		delete [] pszPassword;		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "Encoding user Id to UTF-8");
		return M4_ERROR;
	}
	
	// UNICODE UTF8
	iLength = -1 ;
	pcPwd = M4CppToUtf8 ( pszPassword, iLength );

	if ( pcPwd == NULL || *pcPwd == '\0' )
	{
		delete [] pcUsr;
		delete [] pszPassword;		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "Encoding password to UTF-8");
		return M4_ERROR;
	}

	//Continuamos con la autenticación...
	//Nos validamos.
	iRes = ldap_simple_bind_s( pHandle, pcUsr, pcPwd );

	delete [] pcUsr;
	delete [] pcPwd;

	if ( iRes != LDAP_SUCCESS ) {
		delete [] pszPassword;
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		// Bg 0124496
		SetReturnValue (ai_pvReturn, LDAP_INV_CREDENTIALS);
		iRes = ldap_unbind_s( pHandle );
		return LDAP_INV_CREDENTIALS;
	}	

	// -- Check if user has application availability --------------------------------------------------
	m4return_t iReturn = _checkUserApplAvailability (ai_pvArg[2].Data.PointerChar, pHandle, ai_pvContext);

	//Ahora hacemos la desconexión.
	iRes = ldap_unbind_s( pHandle );
	if ( iRes != LDAP_SUCCESS) 	{
		delete [] pszPassword;
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}	
	//----------------------------------------------------------------------

	//Siempre liberamos.
	delete [] pszPassword;

	SetResultValues ( ai_pvContext, 0, "");
	SetReturnValue (ai_pvReturn, M4_SUCCESS);
	return iReturn;
}
//--------------------------------------------------------------------------
//--------------- FIN SSL --------------------------------------------------
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//--------------- NO SSL ---------------------------------------------------
//--------------------------------------------------------------------------
m4return_t _LogonAuthenticodeNoSSL (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4int_t iRes = M4_ERROR;
	int iLength = -1 ;

	LDAP * pHandle = NULL;


	//----------------------------------------------------------------------
	m4char_t	* pszPassword = NULL;
	iRes = _DecodePasswordTwoWay (ai_pvArg[3].Data.PointerChar, pszPassword);
	if (iRes == M4_ERROR) {
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "DECODING PASSWORD (No SSL)");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	// get a handle to an LDAP connection 
	if ( ( pHandle = ldap_init(ai_pvArg[0].Data.PointerChar, (int) ai_pvArg[1].Data.DoubleData )) == NULL ) {
		// Error en la llamada a ldap
		delete [] pszPassword;
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SETCODEF(M4_LDAP_E_GET_SESSION_HANDLE, ERRORLOG, "LDAP INITIALIZATING: Init (No SSL)");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}	

	m4int_t iVersion = ai_pvArg[10].Data.DoubleData;
	
	if ((iVersion > LDAP_VERSION_MAX) || ( iVersion <= 0)){
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, WARNINGLOG, "#*s1*#%s#", "LDAP PROTOCOL VERSION");
		iVersion = LDAP_VERSION;
	}

	iRes = ldap_set_option( pHandle, LDAP_OPT_PROTOCOL_VERSION, &iVersion );
	if (iRes != LDAP_SUCCESS) {
		delete [] pszPassword;		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SetReturnValue (ai_pvReturn, M4_ERROR);
		iRes = ldap_unbind_s( pHandle );
		return M4_ERROR;
	}

	// bg 263557.
	int iTimeOut = 5000;

	iRes = ldap_set_option( pHandle, LDAP_X_OPT_CONNECT_TIMEOUT, &iTimeOut );
	if ( iRes != LDAP_SUCCESS) {
		delete [] pszPassword;		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SetReturnValue (ai_pvReturn, M4_ERROR);
		iRes = ldap_unbind_s( pHandle );
		return M4_ERROR;
	}

	//bg 105255. Pasamos el id usr y la pwd a UTF-8.
	m4pchar_t pcPwd = NULL, pcUsr = NULL;

	// UNICODE UTF8
	iLength = -1 ;
	pcUsr = M4CppToUtf8 ( ai_pvArg[2].Data.PointerChar, iLength );

	if ( pcUsr == NULL || *pcUsr == '\0' )
	{
		delete [] pszPassword;		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "Encoding user Id to UTF-8");
		return M4_ERROR;
	}
	
	// UNICODE UTF8
	iLength = -1 ;
	pcPwd = M4CppToUtf8 ( pszPassword, iLength );

	if ( pcPwd == NULL || *pcPwd == '\0' )
	{
		delete [] pcUsr;
		delete [] pszPassword;		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "Encoding password to UTF-8");
		return M4_ERROR;
	}

	//Nos validamos.
	iRes = ldap_simple_bind_s( pHandle, pcUsr, pcPwd );

	delete [] pcUsr;
	delete [] pcPwd;

	if ( iRes != LDAP_SUCCESS ) {
		delete [] pszPassword;		
		
		// bg 262944
		if( iRes != LDAP_INVALID_CREDENTIALS )
		{
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			SetReturnValue (ai_pvReturn, M4_ERROR);
			return M4_ERROR;
		}
		else
		{
			// Bg 0124496
			SetReturnValue (ai_pvReturn, LDAP_INV_CREDENTIALS);
		}

		iRes = ldap_unbind_s( pHandle );
		return LDAP_INV_CREDENTIALS;
	}	

	// -- Check if user has application availability --------------------------------------------------
	m4return_t iReturn = _checkUserApplAvailability (ai_pvArg[2].Data.PointerChar, pHandle, ai_pvContext);
	
	//Ahora hacemos la desconexión.
	iRes = ldap_unbind_s( pHandle );
	if ( iRes != LDAP_SUCCESS) 	{
		delete [] pszPassword;
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}	
	//----------------------------------------------------------------------

	//Siempre liberamos.
	delete [] pszPassword;	
	SetReturnValue (ai_pvReturn, iReturn);	

	return iReturn;
}
//--------------------------------------------------------------------------
//--------------- FIN NO SSL -----------------------------------------------
//--------------------------------------------------------------------------


// -- param:
// --		1. Ldap user´s DN
// --		2. Context
// -- return: 
// --		M4_SUCCESS, M_4ERROR
// -- Checks for LDAP User-Application availability ---------------------------------------------------

m4return_t _checkUserApplAvailability (m4pchar_t ai_pcDNUsrNode, LDAP * ai_pHandle, m4pvoid_t ai_pvContext){
		
	// -- Once the user is authenticated we get some LDAP repository definition info from M4O. --------
	// -- If attribute name and the value to check are not null it's required to ----------------------
	// -- ask for application execution user's rights in LDAP directory.-------------------------------	
	ClVMRunContext* pvmContext = (ClVMRunContext*) ai_pvContext;
	ClNode paramNode = (pvmContext->m_pAccess)->Node[M4LDAP_LOG_CHNL_CHILD_NODE];

	// -- LDAP repository definition params to get from M4O SSC_LOG_LDAP. -----------------------------
	m4pchar_t pcDNSuffix            = NULL;
	m4pchar_t pcDNPrefix            = NULL;
	m4pchar_t pcAppAttribName		= NULL;
	m4pchar_t pcAppAttribValue		= NULL;
	m4bool_t  bUserAppIsInChildNode	= false;
	m4VariantType fieldVal;

	pcDNSuffix       = (m4pchar_t) paramNode.RecordSet.Current.Item[M4LDAP_REP_PARAM_SFFIX].Value.Get( );
	pcDNPrefix       = (m4pchar_t) paramNode.RecordSet.Current.Item[M4LDAP_REP_PARAM_PFIX].Value.Get( );
	pcAppAttribName  = (m4pchar_t) paramNode.RecordSet.Current.Item[M4LDAP_REP_PARAM_ATTR_NAME].Value.Get( );
	pcAppAttribValue = (m4pchar_t) paramNode.RecordSet.Current.Item[M4LDAP_REP_PARAM_ATTR_VALUE].Value.Get( );
	paramNode.RecordSet.Current.Item[M4LDAP_REP_PARAM_APP_IN_CHILD].Value.Get(fieldVal);

	bUserAppIsInChildNode = (((m4double_t)fieldVal)==1)?true:false;	

	if (pcAppAttribName != NULL && pcAppAttribValue != NULL && 
		strlen(pcAppAttribName) > 0 && strlen(pcAppAttribValue) > 0){
		
		// -- Specifiying LDAP Entry attribute we are interested in. ----------------------------------
		m4pchar_t* ppcSearchAttrsToGet;
		ppcSearchAttrsToGet    = new m4pchar_t [2]; 
		ppcSearchAttrsToGet[0] = pcAppAttribName;
		ppcSearchAttrsToGet[1] = NULL;

		m4int_t iMaxEntries = 100;
		LDAPMessage* result;

		m4pchar_t pcSearchBaseEntry;
		m4pchar_t pcSearchFilter;
		m4pchar_t pcUserDN = NULL;
		m4return_t iRet = M4_ERROR ;
		
		// bg 117276.		
		while( pcDNSuffix != NULL && *pcDNSuffix == 32 )
		{
			pcDNSuffix++ ;
		}

		while( pcDNPrefix != NULL && *pcDNPrefix == 32 )
		{
			pcDNPrefix++ ;
		}

		if( ( (pcDNSuffix == NULL) ^ (pcDNPrefix == NULL) ) && _isADShortFormat( ai_pHandle, ai_pcDNUsrNode ) == M4_TRUE )
		{
			// Asumimos AD y formato dominio\usuario o usuario@dominio.com en vez de DN.			
			iRet = _getADUserDN( ai_pHandle, ai_pcDNUsrNode, pcUserDN ) ;

			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_LDAP_E_AD_RECOVERING_USER_DN, ERRORLOG, "#*s1*#%s#", ai_pcDNUsrNode );
				return M4_ERROR ;
			}
		}
		else
		{
			pcUserDN = ai_pcDNUsrNode ;
		}


		if (bUserAppIsInChildNode){ // -- Available applications are in a child of the user's node. ---

			pcSearchBaseEntry = pcUserDN;

			// -- Filter must be "attr=value". Search scope LDAP_SCOPE_ONELEVEL -----------------------
			pcSearchFilter = new m4char_t [strlen(pcAppAttribName) + strlen(pcAppAttribValue) + 2];
			memset  (pcSearchFilter, 0, strlen(pcAppAttribName) + strlen(pcAppAttribValue) + 2);
			sprintf (pcSearchFilter, "%s=%s", pcAppAttribName, pcAppAttribValue);
			
		}else{ // -- Available user's applications are in an attribute of its own node. ---------------

			pcSearchBaseEntry = strstr ( pcUserDN, "," );
			if ( pcSearchBaseEntry == NULL )
			{
				SETCODEF (M4_LDAP_E_INVALID_DN_CHECK_APP_AV, ERRORLOG, "#*s1*#%s#", pcUserDN );
				return M4_ERROR;
			}
			else
			{
				if ( pcSearchBaseEntry[1] != NULL )
				{
					pcSearchBaseEntry++;
				}
				else
				{
					SETCODEF (M4_LDAP_E_INVALID_DN_CHECK_APP_AV, ERRORLOG, "#*s1*#%s#", pcUserDN );
					return M4_ERROR;
				}
			}
			
			
			// -- Filter must be "&((preffix=user)(attr=value))". Search scope LDAP_SCOPE_ONELEVEL-----			
			m4uint16_t iDNLength  = strlen (pcUserDN);	// -- "preffix = user, suffix" --------
			m4uint16_t iNumBlanks = 0;
			m4uint16_t iCommaPos  = 0;
			m4uint16_t idx=0;

			for (idx=0; idx<iDNLength; idx++){
				if (pcUserDN[idx] == 44){
					iCommaPos = idx;
					break;
				}else if (pcUserDN[idx] == 32)
					iNumBlanks++;
			}
							
			// -- Filter length => &((preffix=user)(attr=value)) --------------------------------------
			// -- &((			---> 3
			// -- preffix=user	---> iCommaPos-iNumBlanks
			// -- )(			---> 2
			// -- attr			---> pcAppAttribName
			// -- =				---> 1
			// -- value			---> pcAppAttribValue
			// -- ))\0			---> 3

			m4uint16_t iFilterLength = (iCommaPos/*-iNumBlanks*/) + strlen(pcAppAttribName) + strlen(pcAppAttribValue) + 9;
			pcSearchFilter = new m4char_t [iFilterLength];
			memset (pcSearchFilter, 0, iFilterLength);
				
			strcat (pcSearchFilter, "(&(");
				
			// -- Blanks are not allowed. -------------------------------------------------------------
			/*
			m4uint8_t iCurPos = 3;
			for (idx=0; idx<iCommaPos; idx++){
				if (pcUserDN[idx] != 32){
					pcSearchFilter[iCurPos++] = pcUserDN[idx];
				}
			}
			*/
			strncpy( pcSearchFilter+3, pcUserDN, iCommaPos ) ;
			*( pcSearchFilter + 3 + iCommaPos ) = 0 ;
				
			sprintf (pcSearchFilter+3+(iCommaPos/*-iNumBlanks*/), ")(%s=%s))", pcAppAttribName,pcAppAttribValue);
		}

	
		// -- Perform the search. ---------------------------------------------------------------------
		m4int_t iRes = ldap_search_ext_s (ai_pHandle, pcSearchBaseEntry, LDAP_SCOPE_ONELEVEL, pcSearchFilter, ppcSearchAttrsToGet, M4_FALSE, NULL, NULL, NULL, iMaxEntries, &result);

		delete pcSearchFilter;
		delete ppcSearchAttrsToGet;	
		
		// ai_pcDNUsrNode is in short format so pcUserDN was allocated in _getUserDN
		if ( strcmp( pcUserDN, ai_pcDNUsrNode ) )
		{
			delete pcUserDN ;
		}

		if ( iRes != LDAP_SUCCESS){
			
			SETCODEF (M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));			
			return M4_ERROR;

		}else{
			
			if (ldap_first_entry (ai_pHandle, result) != NULL){
				return M4_SUCCESS;				
			}else{			
				SETCODEF (M4_LDAP_E_APP_UNAVAILABLE_TO_USER, ERRORLOG, "#*s1*#%s#%s#", ai_pcDNUsrNode, pcAppAttribValue);
				//bg 101798
				return LDAP_APP_UNAVAILABLE;				
			}

		}

	}else{
		return M4_SUCCESS;		
	}

}
// ----------------------------------------------------------------------------------------------------


// param:
//		1. Ldap Host
//		2. Ldap Port
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Init a Ldap session */

m4return_t LDAPInit(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4VariantType pvHandle;
	LDAP* pHandle;

	if (ai_iLongArg != 2) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E_BAD_PARAMETERS, ERRORLOG, "#*s1*#%s#", "INIT");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}

	// Comprobamos los parámetros
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP HOST");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	if ( ai_pvArg[1].Type != M4CL_CPP_TYPE_NUMBER)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP PORT");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}


	// get a handle to an LDAP connection 
	if ( ( pHandle = ldap_init( ai_pvArg[0].Data.PointerChar, (int)ai_pvArg[1].Data.DoubleData )) == NULL ) {
		// Error en la llamada a ldap
		SetResultValues ( ai_pvContext, -1, "Can´t get session handle");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		SETCODEF(M4_LDAP_E_GET_SESSION_HANDLE, ERRORLOG, "");
		return M4_SUCCESS;
    }
	
	// Metemos el Handle en un m4Variant
	pvHandle.Type = M4CL_CPP_TYPE_NUMBER;
	pvHandle.Data.DoubleData = (double) (long) pHandle;
	// Ponemos el handle en la propiedad, usando el contexto
/*	ClVMRunContext * pvmContext = (ClVMRunContext*)ai_pvContext;
	ClNode NodoR = (pvmContext->m_pAccess)->Node[M4LDAP_PARENT_NODE];
	NodoR.RecordSet.Current.Item[M4LDAP_HANDLE].Value.Set(pvHandle);*/
	(((ClVMRunContext*)ai_pvContext)->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Set(pvHandle);

	SetResultValues ( ai_pvContext, 0, "");
	SetReturnValue (ai_pvReturn, M4_SUCCESS);	
	return M4_SUCCESS;

}

// param:
//		1. Ldap Host
//		2. Ldap Port
//		3. SSL db path
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Init a Ldap SSL session */

m4return_t LDAPInitSSL(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4VariantType pvHandle;
	LDAP* pHandle;
	m4int_t iRes = M4_ERROR;

	if (ai_iLongArg != 3) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E_BAD_PARAMETERS, ERRORLOG, "#*s1*#%s#", "INIT SSL");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}

	// Comprobamos los parámetros
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP HOST");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	if ( ai_pvArg[1].Type != M4CL_CPP_TYPE_NUMBER)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP PORT");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	if (( ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[2].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP SSL CERT DB Path");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	// get a handle to an LDAP connection 
	if ((iRes = ldapssl_client_init (ai_pvArg[2].Data.PointerChar, NULL)) != LDAP_SUCCESS ) {
		// Error en la llamada a ldap		
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "LDAP INITIALIZATING: Can not get init cert db path (SSL No verify)");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_SUCCESS;
	} 
	//Usuario, Passwd, es SSL.
	if ( (pHandle = (LDAP *) ldapssl_init (ai_pvArg[0].Data.PointerChar, (int)ai_pvArg[1].Data.DoubleData, M4_TRUE)) == NULL ) {
		// Error en la llamada a ldap		
		SETCODEF(M4_LDAP_E_GET_SESSION_HANDLE, ERRORLOG, "LDAP INITIALIZATING: Init (SSL No verify)");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_SUCCESS;
	}
	
	// Metemos el Handle en un m4Variant
	pvHandle.Type = M4CL_CPP_TYPE_NUMBER;
	pvHandle.Data.DoubleData = (double) (long) pHandle;
	// Ponemos el handle en la propiedad, usando el contexto
	(((ClVMRunContext*)ai_pvContext)->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Set(pvHandle);

	SetResultValues ( ai_pvContext, 0, "");
	SetReturnValue (ai_pvReturn, M4_SUCCESS);
	return M4_SUCCESS;
}

// param:
//		1. Ldap User
//		2. Ldap Passwd
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Authenticate to the directory */

m4return_t LDAPBind(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	LDAP* pHandle = NULL;
	m4VariantType pvHandle;
	ClVMRunContext*  pvmContext = NULL;
	m4int_t iRes;

	if (ai_iLongArg != 2) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E_BAD_PARAMETERS, ERRORLOG, "#*s1*#%s#","BIND");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	// Comprobamos los parámetros
	if ( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) {
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP USER");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	if ( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR) {
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP PASSWORD");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;
	}


	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}

	// bg 263557.
	int iTimeOut = 5000;

	iRes = ldap_set_option( pHandle, LDAP_X_OPT_CONNECT_TIMEOUT, &iTimeOut );
	if ( iRes != LDAP_SUCCESS) {
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		return M4_SUCCESS;
	}

	iRes = ldap_simple_bind_s( pHandle, ai_pvArg[0].Data.PointerChar, ai_pvArg[1].Data.PointerChar );

	if ( iRes != LDAP_SUCCESS ) {
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		return M4_SUCCESS;
	}	

	SetResultValues ( ai_pvContext, 0, "");
	SetReturnValue (ai_pvReturn, M4_SUCCESS);	
	return M4_SUCCESS;
}



// param:
//		1. Filter
//		2. Attributes
//		3. Find Base
//		4. Scope
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Searchs entries in the directory */

m4return_t LDAPFind(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	LDAPMessage* result;
	LDAPMessage* entrada;
	m4pchar_t pcDn;
	m4pchar_t pcAtributo;
	BerElement* ber;
	m4pchar_t* ppcValores;
	
	m4int_t i;
	m4int_t iRes;
	LDAP* pHandle = NULL;
	m4VariantType pvHandle;
	ClVMRunContext*  pvmContext = NULL;
	m4VariantType vTimeOut;
	struct timeval* stTimeOut = NULL;
	m4VariantType vMaxEntries;
	m4int_t iMaxEntries;
	m4pchar_t* ppcAttrs;	//Atributos que se desean obtener en la búsqueda
		
	int iLength = -1 ;
	m4pchar_t pcData = NULL ;

	if (ai_iLongArg != 4) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "FIND");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	if ( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","FILTER");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;
	}

	if ( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","ATTRIBUTES");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;
	}

	if ( ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","FIND BASE");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;
	}

	if ( ai_pvArg[3].Type != M4CL_CPP_TYPE_NUMBER)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","FIND SCOPE");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;
	}

	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}
	
	// Obtenemos el timeout de la propiedad
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_TIMEOUT].Value.Get(vTimeOut);

	//99330
	if (vTimeOut.Data.DoubleData > 0){
		stTimeOut = new timeval;
		// Set seconds
		stTimeOut->tv_sec = (long)vTimeOut.Data.DoubleData;
		// Set micrseconds
		stTimeOut->tv_usec = 0;
	}

	// Obtenemos el MaxEntries de la propiedad
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_MAXENTRIES].Value.Get(vMaxEntries);
	iMaxEntries = (int)vMaxEntries.Data.DoubleData;
	

	// Sacamos el tipo de scope de la búsqueda
	// Si no es correcto (No está en el rango
	// correcto) por defecto es LDAP_SCOPE_SUBTREE	
	if (( ai_pvArg[3].Data.DoubleData > 2) || ( ai_pvArg[3].Data.DoubleData < 0))
		ai_pvArg[3].Data.DoubleData = 2;

	// Obtenemos los atributo que queremos buscar
	if (( ai_pvArg[1].Data.PointerChar[0] == NULL) || ( ai_pvArg[1].Data.PointerChar[0] == 3))
		// Lo del 3 es por si lo llamamos directamente del canal de LDAP
		// Si es cadena vacía al API le pasamos NULL
		ppcAttrs = NULL;
	else
		// Creamos el array de atributos
		ppcAttrs = Parse (ai_pvArg[1].Data.PointerChar, "#");
		


	// comenzamos la busqueda
	iRes = ldap_search_ext_s( pHandle, ai_pvArg[2].Data.PointerChar, (int)(ai_pvArg[3].Data.DoubleData),
		ai_pvArg[0].Data.PointerChar, ppcAttrs, M4_FALSE, NULL, NULL, stTimeOut, iMaxEntries, &result );

	// Liberamos la memoria asignada para el array de atributos si se le ha asignado en el parser
	if (ppcAttrs != NULL) {
		for (i=0; ppcAttrs[i] != NULL; i++)
			delete ppcAttrs[i];
		delete ppcAttrs;
	}

	// Sacamos el nodo raiz del canal usando el contexto
	ClNode NodoR = (pvmContext->m_pAccess)->Node[M4LDAP_PARENT_NODE];
	
	// Vaciamos los nodos del canal
	m4int_t iNumReg = NodoR.RecordSet.Count();
	for (i = 0; i < iNumReg; i++) {
		NodoR.RecordSet.Current.MoveTo(0);
		NodoR.RecordSet.Current.Destroy();
	}


	if ( iRes != LDAP_SUCCESS)	
	{
			SetReturnValue (ai_pvReturn, M4_ERROR);	
			SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			return M4_SUCCESS;
	}
	else 	
	{
	// Search OK.

		// Por cada entrada obtenemos nombre (dn) + todos los atributos y valores
		for ( entrada = ldap_first_entry( pHandle, result );
			entrada != NULL;
			entrada = ldap_next_entry( pHandle, entrada ) ) 
			{
				// Por cada entrada creamos el registro raiz y su bloque de atributos 
				if ( (pcDn = ldap_get_dn( pHandle, entrada )) != NULL ) 
				{
					// Metemos un registro raiz con el DN
					NodoR.RecordSet.Current.Add();
					NodoR.RecordSet.Current.Item[M4LDAP_PARENT_DN].Value.Set(pcDn);
					ldap_memfree( pcDn );
					// Ponemos un checkpoint para controlar cuando se
					// modifique o borre el registro en el registro
					// del nodo padre
					NodoR.RecordSet.Current.CheckPoint();
					
					// Sacamos el nodo hijo usando el contexto
					ClNode NodoS = (pvmContext->m_pAccess)->Node[M4LDAP_CHILD_NODE];
					
					for ( pcAtributo = ldap_first_attribute( pHandle, entrada, &ber );
						pcAtributo != NULL; 
						pcAtributo = ldap_next_attribute( pHandle, entrada, ber ) )
						//Ya tenemos el name
						//Por cada par atributo valor creamos un registro
						{
							if ( ( ppcValores = ldap_get_values(pHandle, entrada, pcAtributo) ) != NULL )
							{
								//Si hay atributos
								for ( i=0; ppcValores[i]!=NULL; i++ )
								{
									NodoS.RecordSet.Current.Add();
								//	NodoS.RecordSet.Current.Item["DN"].Value.Set(pcDn);
									NodoS.RecordSet.Current.Item[M4LDAP_CHILD_KEY].Value.Set(pcAtributo);
									
									// bg 186417
									pcData = M4ANSIToCpp( ppcValores[i], iLength ) ;
									NodoS.RecordSet.Current.Item[M4LDAP_CHILD_VALUE].Value.Set(pcData);

									delete[] pcData;
									iLength = -1;
									
									// Ponemos un checkpoint para controlar cuando se
									// modifique o borre el registro en el registro
									// del nodo hijo
									NodoS.RecordSet.Current.CheckPoint();
								}
								ldap_value_free( ppcValores );
							}
						}

					ldap_memfree(pcAtributo);
					
				}

				//El ber no lo queremos para nada
				if ( ber != NULL )	ber_free( ber, 0 );

			}
		// el mensaje no lo queremos para nada
		ldap_msgfree( result );
	}
		
	SetReturnValue (ai_pvReturn, M4_SUCCESS);	
	SetResultValues (ai_pvContext, 0, "");
	return M4_SUCCESS;
}



// param:
//		none
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Unbind ldap server */

m4return_t LDAPRelease(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	LDAP* pHandle = NULL;
	m4VariantType pvHandle;
	ClVMRunContext*  pvmContext = NULL;
	m4int_t iRes;
	
	if (ai_iLongArg != 0){
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "RELEASE");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}
	
	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}
	
	// Hacemos el unbind
	iRes = ldap_unbind_s( pHandle );
	if ( iRes != LDAP_SUCCESS) 	{
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		return M4_SUCCESS;
	}

	// Ponemos el handle en la propiedad a NULL, usando el contexto
	pvHandle.Type = M4CL_CPP_TYPE_NUMBER;
	pvHandle.Data.DoubleData = NULL;
	( ((ClVMRunContext*)ai_pvContext)->m_pRecordSet )->Current.Item[M4LDAP_HANDLE].Value.Set(pvHandle);

	SetReturnValue (ai_pvReturn, M4_SUCCESS);	
	SetResultValues (ai_pvContext, 0, "" );
	return M4_SUCCESS;
}



// param:
//		none
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Persists all the changes made in the M4Object */

m4return_t LDAPPersist(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn,	m4pvoid_t ai_pvContext)
{
	ClVMRunContext* pvmContext;
	LDAP* pHandle = NULL;
	m4VariantType pvHandle;
	LDAPMod *pMod=NULL;
	m4VariantType pDn;
	m4int_t iRes = LDAP_OPERATIONS_ERROR;
	
	m4bool_t bNew = M4_FALSE;
	m4bool_t bDeleted = M4_FALSE;

	// Sacamos el nodo raiz del canal usando el contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}
	
/*
//	AL FINAL NO LO HACEMOS CON FILTROS EN RAM, PERO SE PODRIA
	//Ponemos un filtro de modificados
	//Se indica explicitamente el tipo del argumento para que no haya 
	//problemas de ambigüedades con AddNew
	oNodoP.RecordSet.Current.Filter.AddNew ((m4uint8_t)M4DM_OPERATION_UPDATE);
	oNodoH.RecordSet.Current.Filter.AddNew ((m4uint8_t)M4DM_OPERATION_UPDATE);
	oNodoP.RecordSet.Current.Filter.UpdateFilter();
	oNodoH.RecordSet.Current.Filter.UpdateFilter();

	// PRUEBECILLA DE LOS FILTROS
		oNodoP.RecordSet.Current.MoveTo (0);
		int n = 0;
		while (!oNodoP.RecordSet.IsEOF())
		{
			oNodoP.RecordSet.Current++;
			n++;
		}
		SetResultValues (ai_pvContext, n, "" );
	
		//Los filtros vienen definidos asi en la MVC:
		//enum EnumOperation_t
		//{
		//	M4DM_OPERATION_NONE   = 0,
		//	M4DM_OPERATION_DELETE = 1,
		//	M4DM_OPERATION_INSERT = 2,
		//	M4DM_OPERATION_UPDATE = 4
		//}; 
		
	//quitamos los filtros
	oNodoP.RecordSet.Current.Filter.DeleteAll();
	oNodoH.RecordSet.Current.Filter.DeleteAll();
*/	

	// Sacamos el nodo padre
	ClNode oNodoP = (pvmContext->m_pAccess)->Node[M4LDAP_PARENT_NODE];
	oNodoP.RecordSet.Current.MoveTo (0);

	// Nos recorremos el canal
	m4int_t iNumReg = oNodoP.RecordSet.Count();
	for (m4int_t i = 0; i < iNumReg; i++)
	{
		oNodoP.RecordSet.Current.Item[M4LDAP_PARENT_DN].Value.Get(pDn);

		// Si el dn está vacío no se puede hacer ninguna operacion. Damos un error
		if ((pDn.Data.PointerChar != NULL) && (pDn.Data.PointerChar[0] != NULL)) 
		{

			// Sacamos el nodo hijo
			ClNode oNodoH = (pvmContext->m_pAccess)->Node[M4LDAP_CHILD_NODE];
	
			// Miramos que ha pasado con la entrada
			oNodoP.RecordSet.Current.IsNew(bNew);
			oNodoP.RecordSet.Current.IsDeleted(bDeleted);
			// El Dn no se puede modificar directamente

			// Comprobamos si se ha añadido la entrada
			if (bNew)
			{
				// Construimos la estructura de atributos y añadimos la entrada
				LDAPMod ** mods;
				// Reservamos espacio para los punteros a estructuras LDAPMod
				mods = new LDAPMod*[oNodoH.RecordSet.Count() + 1];
			
				// Creamos y rellenamos las estructuras LDAPMod necesarias
				if ( BuildLDAPMod ( mods, &(oNodoH.RecordSet)) == M4_ERROR) 
				{
					// La entrada tiene los atributos mal construidos, no es correcta, paramos y error
					delete mods;
					SetLastPEntry (ai_pvContext, i);
					SetReturnValue (ai_pvReturn, M4_ERROR);	
					SetResultValues (ai_pvContext, iRes, "There is an entry with invalid attributes" );
					SETCODEF(M4_LDAP_E_INVALID_ENTRY_ATTRIBUTES, ERRORLOG, "#*s1*#%d#", i+1);
					return M4_SUCCESS;
				};
			
				// Realizamos la inserción
				iRes = ldap_add_s( pHandle, pDn, mods);
			
				// liberamos la memoria asignada
				for (m4int_t j=0; mods[j] != NULL; j++){
					delete mods[j]->mod_values;
					delete mods[j];
				}
				delete mods;
					
				// Comprobamos errores
				if (iRes != LDAP_SUCCESS){
					SetLastPEntry (ai_pvContext, i);
					SetReturnValue (ai_pvReturn, M4_ERROR);	
					SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
					SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
					return M4_SUCCESS;
				};
				// Si se ha ido bien ponemos el checkpoint en el registro y sus hijos
				oNodoH.RecordSet.CheckPoint();
				oNodoP.RecordSet.Current.CheckPoint();
				// Y ya estamos en el siguiente
				//97155 oNodoP.RecordSet.Current.Next();

			}

			// Comprobamos si se ha borrado la entrada
			else if (bDeleted) {
		
				// Borramos la entrada
				iRes = ldap_delete_s( pHandle, pDn );
				if ( iRes != LDAP_SUCCESS)	{
					SetLastPEntry (ai_pvContext, i);
					SetReturnValue (ai_pvReturn, M4_ERROR);	
					SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
					SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
					return M4_SUCCESS;			
				}; 
				// Si se ha ido bien destruimos el registro y su hijo
				// No usamos CheckPoint en este caso porque no destruye el bloque hijo
				oNodoP.RecordSet.Current.Destroy();
				// Y ya estamos en el siguiente
			}

			// Comprobamos si se ha modificado
			else {
			
				// Construimos la estructura de atributos y modificamos la entrada
				LDAPMod ** mods;
				// Reservamos espacio para los punteros a estructuras LDAPMod
				mods = new LDAPMod*[oNodoH.RecordSet.Count() + 1];
			
				// Creamos y rellenamos las estructuras LDAPMod necesarias
				if ( BuildLDAPMod ( mods, &(oNodoH.RecordSet)) == M4_ERROR)
				{
					// La entrada tiene los atributos mal construidos, no es correcta, paramos y error					
					delete mods;
					SetLastPEntry (ai_pvContext, i);
					SetReturnValue (ai_pvReturn, M4_ERROR);	
					SetResultValues (ai_pvContext, iRes, "There is an entry with invalid attributes" );
					SETCODEF(M4_LDAP_E_INVALID_ENTRY_ATTRIBUTES, ERRORLOG, "#*s1*#%d#", i+1);
					return M4_SUCCESS;
				}
			
				// Ponemos iRes a exito por si acaso la inserción no se
				// llega a realizar al no haberse modificado nada y no  
				// considerar un error anterior
				iRes = LDAP_SUCCESS;

				// Comprobamos si se ha modificado el nodo hijo de atributos
				if (mods[0] != NULL) {
					// Realizamos la inserción
					iRes = ldap_modify_s( pHandle, pDn, mods);
				}
			
				// liberamos la memoria asignada
				for (m4int_t j=0; mods[j] != NULL; j++){
						delete mods[j]->mod_values;
						delete mods[j];
				}
				delete mods;

				// Comprobamos errores
				if (iRes != LDAP_SUCCESS){
					SetLastPEntry (ai_pvContext, i);
					SetReturnValue (ai_pvReturn, M4_ERROR);	
					SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
					SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
					return M4_SUCCESS;
				};
				// Si se ha ido bien ponemos el checkpoint en el registro y sus hijos
				oNodoH.RecordSet.CheckPoint();
				oNodoP.RecordSet.Current.CheckPoint();
			}
		
			// Y ya estamos en el siguiente
			oNodoP.RecordSet.Current.Next();
		}
		else
		{
			// La entrada tiene el dn vacío, no es correcta, paramos y error
			SetLastPEntry (ai_pvContext, i);
			SetReturnValue (ai_pvReturn, M4_ERROR);	
			SetResultValues (ai_pvContext, iRes, "There is an entry without DN" );
			SETCODEF(M4_LDAP_E_INVALID_ENTRY_DN, ERRORLOG, "#*s1*#%d#", i+1);
			return M4_SUCCESS;
		}
	
	} // FOR

	// Nos movemos al principio para refrescar la presentación
	// tanto en el padre como en el hijo
	// Fue el bug 0028037
	oNodoP.RecordSet.Current.Begin();
	
	SetLastPEntry (ai_pvContext, 0);
	SetReturnValue (ai_pvReturn, M4_SUCCESS);	
	SetResultValues (ai_pvContext, 0, "" );
	return M4_SUCCESS;
}

/* Construye la estructura LDAPMod con las modificaciones, borrados e
inserciones de atributos en el nodo hijo */
// Si existe alguna incoherencia en el nodo hijo devuelve M4_ERROR,
// si construye todo correctamente devuelve M4_SUCCESS

m4return_t BuildLDAPMod (LDAPMod ** mods, ClAccessRecordSet* rs){

	m4VariantType pKey;
	m4VariantType pValue;
	m4int_t i;
	
	m4bool_t bNew = M4_FALSE;
	m4bool_t bModified = M4_FALSE;
	m4bool_t bDeleted = M4_FALSE;
	
	rs->Current.MoveTo(0);
	m4int_t iNumReg = rs->Count();
  	// Rellenamos las estructuras LDAPMod
	for ( i = 0 ; iNumReg > 0; rs->Current.Next(), iNumReg-- ) {
		
		rs->Current.IsNew(bNew);
		rs->Current.IsDeleted(bDeleted);
		rs->Current.IsModified(bModified);

		// Sacamos los valores
		rs->Current.Item[M4LDAP_CHILD_KEY].Value.Get(pKey);
		rs->Current.Item[M4LDAP_CHILD_VALUE].Value.Get(pValue);

		if ( (pKey.Data.PointerChar != NULL ) && (pKey.Data.PointerChar[0] != NULL ))
		{

			if (bNew) {
			
				// Asignamos los valores a la LDAPMod
				mods[i] = new LDAPMod;
				mods[i]->mod_op = LDAP_MOD_ADD;
				mods[i]->mod_type = pKey.Data.PointerChar;
				mods[i]->mod_values = new char*[2];
				mods[i]->mod_values[0] = pValue.Data.PointerChar;
				mods[i]->mod_values[1] = NULL;
				i++;
			}
		
			else if (bDeleted) {
		
				// Asignamos los valores a la LDAPMod
				mods[i] = new LDAPMod;
				mods[i]->mod_op = LDAP_MOD_DELETE;
				mods[i]->mod_type = pKey.Data.PointerChar;
				mods[i]->mod_values = new char*[2];
				mods[i]->mod_values[0] = pValue.Data.PointerChar;
				mods[i]->mod_values[1] = NULL;
				i++;
			}

			else if (bModified) {

				// Asignamos los valores a la LDAPMod
				mods[i] = new LDAPMod;
				mods[i]->mod_op = LDAP_MOD_REPLACE;
				mods[i]->mod_type = pKey.Data.PointerChar;
				mods[i]->mod_values = new char*[2];
				mods[i]->mod_values[0] = pValue.Data.PointerChar;
				mods[i]->mod_values[1] = NULL;
				i++;
			}
		}
		else
			// La entrada tiene algún atributo con KEY vacío
			return M4_ERROR;
		
	}

	mods[i] = NULL;
	return M4_SUCCESS;
}



/* Put result property values in root node */

m4void_t SetResultValues (m4pvoid_t ai_pvContext, m4int_t ai_iRetVal,
							m4pchar_t ai_pccRetDesc	){
	
	m4VariantType pvVarNum(ai_iRetVal);
	m4VariantType pvVarStr(ai_pccRetDesc, M4_TRUE);
	
	// Ponemos el valor de retorno en la propiedad usando el contexto
	// Metemos el valor en un m4Variant para meterlo en la property
	( ((ClVMRunContext*)ai_pvContext)->m_pRecordSet )->Current.Item[M4LDAP_ERRORNUM].Value.Set(pvVarNum);

	//Ponemos la descripción del retorno en la propiedad, usando el contexto
	//Metemos la cadena const del error en una cadena para meterla en la property
	( ((ClVMRunContext*)ai_pvContext)->m_pRecordSet )->Current.Item[M4LDAP_ERRORDESC].Value.Set(pvVarStr);

}



/* Put last procesed entry property value in root node */

m4void_t SetLastPEntry (m4pvoid_t ai_pvContext, m4int_t ai_iPEntry)
{
	
	m4VariantType pvVarNum(ai_iPEntry);
		
	// Ponemos el valor de retorno en la propiedad usando el contexto
	// Metemos el valor en un m4Variant para meterlo en la property
	( ((ClVMRunContext*)ai_pvContext)->m_pRecordSet )->Current.Item[M4LDAP_LASTPENTRY].Value.Set(pvVarNum);

}



/* Set the result value of the function */

m4void_t SetReturnValue (m4VariantType &ai_pvReturn, m4int_t ai_iRetVal){
	
	// Ponemos en el variant de retorno el valor
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = ai_iRetVal;

}



/* Reset all the modify flags */
/*
//Por si hace falta
m4void_t ResetModify (m4pvoid_t ai_pvContext){

	// Reseteamos todos los flags de modificación
	ClVMRunContext* pvmContext;
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(  ( (pvmContext->m_pAccess)->GetpChannel() )->GetpChannel_Data()  )->CheckPoint();	
}
*/

// param:
//		DN para borrar.
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Borra una entrada del directorio*/
m4return_t LDAPDeleteEntry(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4int_t iRes;
	LDAP* pHandle = NULL;
	m4VariantType pvHandle;
	ClVMRunContext*  pvmContext = NULL;		

	// Comprobamos el número de parámetros
	if (ai_iLongArg != 1) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E_BAD_PARAMETERS, ERRORLOG, "#*s1*#%s#","DELETE_ENTRY");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	// Comprobamos los parámetros
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP USER");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;

	}

	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}
	
	// Borramos la entrada
	iRes = ldap_delete_s( pHandle, ai_pvArg[0].Data.PointerChar );

	if ( iRes != LDAP_SUCCESS)	
	{
			SetReturnValue (ai_pvReturn, M4_ERROR);	
			SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			return M4_SUCCESS;
	}
	else 	
	{
			SetReturnValue (ai_pvReturn, M4_SUCCESS);	
			SetResultValues (ai_pvContext, 0, "" );
			return M4_SUCCESS;
	}
}


// param:
//		DN para modifica.
//      Attributo que se modifica. (Attributo = DN es caso especial!!!) 
//      Value para modificar. 
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Modifica una entrada del directorio*/
m4return_t LDAPUpdateEntry(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4int_t iRes;
	LDAP* pHandle = NULL;
	m4VariantType pvHandle;
	ClVMRunContext*  pvmContext = NULL;	
	// Estructura de modificaciones
	LDAPMod * mods[2];
	// Array de valores de atributos. El ultimo debe ser NULL
	m4pchar_t values[2];
   
	if (ai_iLongArg != 3) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "UPDATEENTRY");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","DN");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;
	}

	if (( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[1].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","ATTRIBUTE NAME");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;
	}

	if ( ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR) {
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","ATTRIBUTE VALUE");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_SUCCESS;
	}

	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}
	
	// Si lo que se modifica es el DN
	if (! strcmp(ai_pvArg[1].Data.PointerChar,"dn") || ! strcmp(ai_pvArg[1].Data.PointerChar,"DN")
		|| ! strcmp(ai_pvArg[1].Data.PointerChar,"dN") || ! strcmp(ai_pvArg[1].Data.PointerChar,"Dn"))
    {
		// modificamos completamente la nueva dn con otra dn.
		iRes = ldap_modrdn2_s(pHandle, ai_pvArg[0].Data.PointerChar, ai_pvArg[2].Data.PointerChar,1);
    }
	else
	{
		mods[0] = new LDAPMod;
		// Se indica la operación de remplazo.
		mods[0]->mod_op = LDAP_MOD_REPLACE; 
		// Atributo que se va a modificar
		mods[0]->mod_type = ai_pvArg[1].Data.PointerChar;
		// Valor al que se cambia.
		values[0]=ai_pvArg[2].Data.PointerChar;
		values[1]=NULL;
		mods[0]->mod_values = (m4pchar_t*)values;

		mods[1]=NULL;

		iRes = ldap_modify_s(pHandle, ai_pvArg[0].Data.PointerChar, (LDAPMod **)mods);
	
		// Liberamos memoria
		// No usamos ldap_mods_free (&(mods), 1); porque las cadenas usadas no las 
		// reservamos nosotros.
		delete mods[0];
	}

	// Comprobamos errores
	if ( iRes != LDAP_SUCCESS)	
	{
			SetReturnValue (ai_pvReturn, M4_ERROR);	
			SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			return M4_SUCCESS;
	}
	else 	
	{
			SetReturnValue (ai_pvReturn, M4_SUCCESS);	
			SetResultValues (ai_pvContext, 0, "" );
			return M4_SUCCESS;
	}
}

// param:
//		VERSION LDAP version.
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Asigna el valor de la version LDAP. */
/* 98073 */
m4return_t LDAPSetVersion(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	LDAP* pHandle = NULL;
	ClVMRunContext* pvmContext = NULL;
	m4VariantType pvHandle;
	m4int_t iRes;

	if (ai_iLongArg != 1) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "SET_VERSION");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_ERROR;
	}

	if (ai_pvArg[0].Type != M4CL_CPP_TYPE_NUMBER) {
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","OPTION");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		return M4_ERROR;
	}

	if ((ai_pvArg[0].Data.DoubleData > LDAP_VERSION_MAX) || ( ai_pvArg[0].Data.DoubleData <= 0)){
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, WARNINGLOG, "#*s1*#%s#", "LDAP PROTOCOL VERSION");
		ai_pvArg[0].Data.DoubleData = LDAP_VERSION;
	}

	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);	
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_ERROR;
	}
	

	m4int_t iVersion = (int)ai_pvArg[0].Data.DoubleData;

	iRes = ldap_set_option( pHandle, LDAP_OPT_PROTOCOL_VERSION, &iVersion );

	if (iRes != LDAP_SUCCESS) {
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}

	SetReturnValue (ai_pvReturn, M4_SUCCESS);	
	SetResultValues (ai_pvContext, 0, "");

	return M4_SUCCESS;
}

// param:
//		1. Ldap User DN
//		2. Ldap New passwd
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Resets AD User account password */
/* Required SSL connection and admin user rights */

m4return_t LDAPResetADPassword(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4VariantType pvHandle;
	ClVMRunContext* pvmContext = NULL;
	LDAP* pHandle;
	m4int_t iRes = M4_ERROR;

	if (ai_iLongArg != 2) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E_BAD_PARAMETERS, ERRORLOG, "#*s1*#%s#", "LDAP AD Reset Password");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}

	// Comprobamos los parámetros
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP User DN");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	if (( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[1].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP User password");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}

	LDAPMod *aAttrToMod[2];
	LDAPMod theAttr;
	berval *aAttrValue[2];
	berval pwdBerVal;	
	
	// Build an array of LDAPMod.	
	aAttrToMod[0] = &theAttr;
	aAttrToMod[1] = NULL;
	
	theAttr.mod_op   = LDAP_MOD_REPLACE | LDAP_MOD_BVALUES;
	theAttr.mod_type = MSAD_ATTR_USR_PASSWORD;
	theAttr.mod_vals.modv_bvals = aAttrValue;

	aAttrValue[0] = &pwdBerVal;
	aAttrValue[1] = NULL;
    
	m4pchar_t pszQuotedPwd = new m4char_t[strlen(ai_pvArg[1].Data.PointerChar)+3]; //Pwd + quotes + '\0'

	sprintf (pszQuotedPwd, "\"%s\"",ai_pvArg[1].Data.PointerChar);

	m4pchar_t pPwdUTF16 = NULL;
	
	m4int_t iLength = _charpToUTF16LE (pszQuotedPwd, (void*&)pPwdUTF16);

	if (iLength > 0){	

		pwdBerVal.bv_len = iLength;
		pwdBerVal.bv_val = pPwdUTF16;

		iRes = ldap_modify_s(pHandle,ai_pvArg[0].Data.PointerChar,aAttrToMod);

		delete pszQuotedPwd;
		delete pPwdUTF16;

		if ( iRes != LDAP_SUCCESS)	
		{
				SetReturnValue (ai_pvReturn, M4_ERROR);	
				SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
				SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
				return M4_SUCCESS;
		}
		else 	
		{
				SetReturnValue (ai_pvReturn, M4_SUCCESS);	
				SetResultValues (ai_pvContext, 0, "" );
				return M4_SUCCESS;
		}
	}else{
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "Invalid password");
		return M4_SUCCESS;
	}
}

// param:
//		1. Ldap User DN
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Enables AD User account */

m4return_t LDAPEnableADAccount(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4VariantType pvHandle;
	ClVMRunContext* pvmContext = NULL;
	LDAP* pHandle;
	m4int_t iRes = M4_ERROR;

	if (ai_iLongArg != 1) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E_BAD_PARAMETERS, ERRORLOG, "#*s1*#%s#", "LDAP AD Enable Account");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}

	// Comprobamos los parámetros
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP User DN");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}
	
 	LDAPMessage *pSearchResults, *pFirstEntry; 
 	m4pchar_t   *apszAccountCtrlValue; 	
	m4int_t     iAccountCtrlValue;
	m4pchar_t   aAttrToGet[2];

	// Obtenemos el valor del atributo "userAccountControl"
	aAttrToGet[0] = MSAD_ATTR_USR_ACCOUNT_CTRL;
	aAttrToGet[1] = NULL;

	iRes = ldap_search_ext_s (pHandle, ai_pvArg[0].Data.PointerChar, LDAP_SCOPE_SUBTREE, MSAD_FILTER_USR_ACCOUNT_CTRL, aAttrToGet, M4_FALSE, NULL, NULL, NULL, 1, &pSearchResults);

	if (iRes != LDAP_SUCCESS){
		ldap_msgfree (pSearchResults);
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		return M4_SUCCESS;
 	}
 
	pFirstEntry = ldap_first_entry (pHandle, pSearchResults);
 	
	apszAccountCtrlValue = ldap_get_values (pHandle, pFirstEntry, MSAD_ATTR_USR_ACCOUNT_CTRL);	
	iAccountCtrlValue    = atoi (apszAccountCtrlValue[0]);
	iAccountCtrlValue   &= ~MSAD_FLAG_ACCOUNT_DISABLE;
	  
	ldap_value_free (apszAccountCtrlValue);
	ldap_msgfree (pSearchResults);

	// Activamos la cuenta en el AD, mediante el atributo userAccountControl
	LDAPMod   *aAttrToMod[2];
	LDAPMod   theAttr;
	m4pchar_t aAttrValue[2];

	m4char_t pszAccountCtrlValue[4];
	aAttrValue[0] = itoa (iAccountCtrlValue, pszAccountCtrlValue, 10);
	aAttrValue[1] = NULL;

	theAttr.mod_op     = LDAP_MOD_REPLACE;
	theAttr.mod_type   = MSAD_ATTR_USR_ACCOUNT_CTRL;	
	theAttr.mod_values = aAttrValue;
	
	aAttrToMod[0] = &theAttr;
	aAttrToMod[1] = NULL;

	iRes = ldap_modify_s( pHandle, ai_pvArg[0].Data.PointerChar, aAttrToMod );

	if (iRes != LDAP_SUCCESS){
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
		return M4_SUCCESS;
	}
	
	SetReturnValue (ai_pvReturn, M4_SUCCESS);	
	SetResultValues (ai_pvContext, 0, "" );
	return M4_SUCCESS;
}

// param:
//		1. Ldap User DN
//		2. Ldap Old passwd
//		3. Ldap New passwd
// return: 
//		M4_SUCCESS, M_4ERROR
//
/* Changes AD User account password */

m4return_t LDAPChangeADPassword(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4VariantType pvHandle;
	ClVMRunContext* pvmContext = NULL;
	LDAP* pHandle;
	m4int_t iRes = M4_ERROR;

	if (ai_iLongArg != 3) {
		// Parametros incorrectos
		SETCODEF(M4_LDAP_E_BAD_PARAMETERS, ERRORLOG, "#*s1*#%s#", "LDAP AD Change Password");
		SetReturnValue (ai_pvReturn, M4_ERROR);
		return M4_ERROR;
	}

	// Comprobamos los parámetros
	if (( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[0].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP User DN");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	if (( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[1].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP User old password");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	if (( ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR) ||
		( ai_pvArg[2].Data.PointerChar[0] == '\0' )	)	{
		// Parametro no válido
		SETCODEF(M4_LDAP_E_INVALID_ARGUMENT, ERRORLOG, "#*s1*#%s#","LDAP User new password");
		SetResultValues ( ai_pvContext, -2, "Invalid argument");
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		return M4_SUCCESS;
	}

	// Obtenemos el Handle del contexto
	pvmContext = (ClVMRunContext*)ai_pvContext;
	(pvmContext->m_pRecordSet)->Current.Item[M4LDAP_HANDLE].Value.Get(pvHandle);
	pHandle = (LDAP*)(long)pvHandle.Data.DoubleData;

	// Conmprobamos si el handle es valido
	if (!pHandle) {
		// El handle no es correcto
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SetResultValues (ai_pvContext, -1, "Session is not initialized" );
		SETCODEF(M4_LDAP_E_SESSION_NOT_INITIALIZED, ERRORLOG, "");
		return M4_SUCCESS;
	}
	
	LDAPMod modNewPassword;
	LDAPMod modOldPassword;
	LDAPMod *modEntry[3];
	berval newPwdBerVal;
	berval oldPwdBerVal;
	berval *newPwd_attr[2];
	berval *oldPwd_attr[2];

	modEntry[0] = &modOldPassword;
	modEntry[1] = &modNewPassword;
	modEntry[2] = NULL;

	modNewPassword.mod_op   = LDAP_MOD_ADD | LDAP_MOD_BVALUES;
	modNewPassword.mod_type = MSAD_ATTR_USR_PASSWORD;
	modNewPassword.mod_vals.modv_bvals = newPwd_attr;

	modOldPassword.mod_op   = LDAP_MOD_DELETE | LDAP_MOD_BVALUES;
	modOldPassword.mod_type = MSAD_ATTR_USR_PASSWORD;
	modOldPassword.mod_vals.modv_bvals = oldPwd_attr;

	newPwd_attr[0] = &newPwdBerVal;
	newPwd_attr[1] = NULL;
	oldPwd_attr[0] = &oldPwdBerVal;
	oldPwd_attr[1] = NULL;

	m4pchar_t pszQuotedOldPwd = new m4char_t[strlen(ai_pvArg[1].Data.PointerChar)+3]; //Pwd + quotes + '\0'
	sprintf (pszQuotedOldPwd, "\"%s\"",ai_pvArg[1].Data.PointerChar);
	m4pchar_t pOldPwdUTF16 = NULL;

	m4int_t iOldLength = _charpToUTF16LE (pszQuotedOldPwd, (void*&)pOldPwdUTF16);

	m4pchar_t pszQuotedNewPwd = new m4char_t[strlen(ai_pvArg[2].Data.PointerChar)+3]; //Pwd + quotes + '\0'
	sprintf (pszQuotedNewPwd, "\"%s\"",ai_pvArg[2].Data.PointerChar);
	m4pchar_t pNewPwdUTF16 = NULL;

	m4int_t iNewLength = _charpToUTF16LE (pszQuotedNewPwd, (void*&)pNewPwdUTF16);
	
	if (iOldLength > 0 && iNewLength > 0){
		
		oldPwdBerVal.bv_len = iOldLength;
		oldPwdBerVal.bv_val = pOldPwdUTF16;

		newPwdBerVal.bv_len = iNewLength;
		newPwdBerVal.bv_val = pNewPwdUTF16;

		// Perform single modify.
		iRes = ldap_modify_s(pHandle, ai_pvArg[0].Data.PointerChar, modEntry );

		delete pszQuotedOldPwd;
		delete pOldPwdUTF16;
		delete pszQuotedNewPwd;
		delete pNewPwdUTF16;

		if ( iRes != LDAP_SUCCESS)	
		{
			SetReturnValue (ai_pvReturn, M4_ERROR);	
			SetResultValues (ai_pvContext, iRes, ldap_err2string (iRes) );
			SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes));
			return M4_SUCCESS;
		}
		else 	
		{
			SetReturnValue (ai_pvReturn, M4_SUCCESS);	
			SetResultValues (ai_pvContext, 0, "" );
			return M4_SUCCESS;
		}
	}else{
		SetReturnValue (ai_pvReturn, M4_ERROR);	
		SETCODEF(M4_LDAP_E, ERRORLOG, "#*s1*#%s#", "Invalid password");
		return M4_SUCCESS;
	}
}


// JUAN RAMON DIAZ POL
//
// Funcion que desglosa una cadena con elementos separados por un token en un array con 
// cada elemento en una posición y acabado en NULL
//	
// Argumentos:
//		1. Cadena con las palabras separadas por un caracter
//		separador definido en el segundo parametro. (char *)
//		2. Elemento de separacion (char *).
//			Debe ser de longitud 1,
//			sólo se reconocen separadores de longitud 1.
//
// Salida:
//		3. Array donde se guardan los elementos acabado 
//		en NULL (char**)

m4pchar_t* Parse( m4pchar_t pcCad, const m4pchar_t pcSep)
{	
	m4pchar_t vacia = " ";
	
	// Array resultado
	m4pchar_t* ppcRes;

	// Contamos el número de elementos
	m4int_t iNumElem = 1;  //Al menos tenemos uno
	m4int_t cont_nulos;
	m4int_t id_valido = 0;

	if (strlen(pcCad) != 0) 
		for (m4uint_t i=0; i<strlen(pcCad); i++){
			
			if ((pcCad[i]!=' ')&&(pcCad[i]!= pcSep[0] )) {
				id_valido=1;
			}
			
			if ( (pcCad[i]== pcSep[0] ) && 
			   ( (pcCad[i+1]!= pcSep[0] ) && (id_valido==1) &&
					((i+1)!=strlen(pcCad) )	) ) {
					
					iNumElem++;
			}
		}
		
	if (id_valido==1) {
		// Asignamos memoria al array
		ppcRes = new m4pchar_t [iNumElem + 1];

		m4pchar_t pcToken;

		pcToken = strtok( pcCad, pcSep );
	
	
		for (m4int_t j = 0; pcToken != NULL; j++ )	{

			cont_nulos=0;		
			if (pcToken[0]==' ') {
				cont_nulos++;			
				for (m4int_t k = 1; k<=(m4int_t)strlen(pcToken); k++ ) {
					if (pcToken[k]==' '){
						cont_nulos++;
					}
					else k=strlen(pcToken)+1;
				}
			
				if (cont_nulos==(m4int_t)strlen(pcToken)){
					j--;
					iNumElem--;
				}
			
			}
		
			if (cont_nulos!=(m4int_t)strlen(pcToken)){
			
				// Asignamos memoria a la cadena
				ppcRes[j] = new m4char_t [strlen(pcToken)+1];
				// La copiamos
		
				strcpy(ppcRes[j], pcToken);
				strcat(ppcRes[j],"\0");
			}	
			
			pcToken = strtok( NULL, pcSep );
	
		}

		// metemos el NULL al final
		ppcRes[iNumElem] = NULL;
	}
	
	else {

		ppcRes = NULL;
	}

	return ppcRes;

}

// param:
//		1. Char string to convert
//		2. Output string as UTF16 Little Endian 
// return: 
//		M4_ERROR, output size in bytes.
//

m4int_t _charpToUTF16LE (m4pchar_t ai_pszSource, void*& ao_pTarget){
	
	if (!ai_pszSource){
		ao_pTarget = NULL;
		return M4_ERROR;
	}

	m4int_t iSrcLen        = strlen (ai_pszSource);
	m4uchar_t iWCSize      = sizeof (wchar_t);
	m4int_t iUTF16Size     = iSrcLen * 2;
	m4bool_t bLittleEndian = M4_TRUE;

	#ifdef _UNIX
		bLittleEndian = M4_FALSE;
	#endif

	m4pchar_t pcTarget = new m4char_t[iUTF16Size];

	if (pcTarget == NULL){
		ao_pTarget = NULL;
		return M4_ERROR;
	}

	ao_pTarget = pcTarget;

	wchar_t wcCurrent;
	m4pchar_t pcCurrentWC = NULL;
	
	for (m4int_t iIdx=0; iIdx<iSrcLen; iIdx++){

		if (mbtowc (&wcCurrent, ai_pszSource+iIdx, 1) > -1){
			
			pcCurrentWC = (m4pchar_t)(&wcCurrent);

			if (!bLittleEndian){
				*pcTarget++ = *(pcCurrentWC+iWCSize-1);
				*pcTarget++ = *(pcCurrentWC+iWCSize-2);
			}else{
				*pcTarget++ = *(pcCurrentWC+iWCSize-2);
				*pcTarget++ = *(pcCurrentWC+iWCSize-1);
			}
		}else{
			delete pcTarget;
			ao_pTarget = NULL;
			return M4_ERROR;			
		}

	}
	
	return (iUTF16Size);
}

// param:
//		1. Ldap session handle
//		2. User in short format (domain\usr or usr@domain.com)
//		3. USer DN Output string. Caller must free it.
// return: 
//		M4_TRUE / M4_FALSE
//
/* Returns the user DN:                       						*/
/* Gets the Root DSE in order to know the base of the LDAP tree.	*/
/* Searchs the user based in that base path, and returns a copy		*/
/* of the distinguised name.										*/

m4return_t _getADUserDN ( LDAP * ai_pHandle, m4pcchar_t ai_pccUser, m4pchar_t &ao_pcUserDN)
{
	m4int_t iRes ;
	m4pchar_t ppcSearchAttrsToGet[2]; 
	LDAPMessage* pSrchResult = NULL ;
	LDAPMessage* pSrchEntry = NULL ;
	m4pchar_t pcSrchAttr = NULL ;
	BerElement* pBer = NULL ; 
	m4pchar_t* ppcValues = NULL ;
	m4pchar_t pcRootDomainNCtx = NULL ;
	m4pchar_t pcUserID = NULL ;
	m4pchar_t pcFilterAccountName = NULL ;

		
	if( ao_pcUserDN != NULL )
	{
		delete ao_pcUserDN ;
		ao_pcUserDN = NULL ;
	}

	// First Search: the root DSE.

	iRes = ldap_set_option( ai_pHandle, LDAP_OPT_REFERRALS, LDAP_OPT_OFF ) ;

	if( iRes != LDAP_SUCCESS ) { 
		SETCODEF( M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes) ) ;
		return( M4_ERROR ) ; 
	}

	ppcSearchAttrsToGet[0] = MSAD_ATTR_RDSE_NCONTEXT ;
	ppcSearchAttrsToGet[1] = NULL ;

	iRes = ldap_search_ext_s( ai_pHandle, "", LDAP_SCOPE_BASE, MSAD_FILTER_ALL, ppcSearchAttrsToGet, M4_FALSE, NULL, NULL, NULL, 1, &pSrchResult) ;

	if( iRes != LDAP_SUCCESS )
	{
		SETCODEF( M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes) ) ;
		return( M4_ERROR ) ; 
	}

	pSrchEntry = ldap_first_entry( ai_pHandle, pSrchResult ) ;

	if ( pSrchEntry == NULL )
	{ 
		ldap_memfree( pSrchResult ) ;
		return M4_ERROR ;
	}

	pcSrchAttr = ldap_first_attribute( ai_pHandle, pSrchEntry, &pBer ) ;

	if ( pcSrchAttr == NULL ) 
	{ 				
		if ( pBer != NULL ) { 
			ber_free( pBer, 0 ) ;
		} 
		ldap_memfree( pSrchResult ) ;

		return M4_ERROR ;
	}

	ppcValues = ldap_get_values( ai_pHandle, pSrchEntry, pcSrchAttr ) ;

	if ( ppcValues == NULL ) 
	{ 
		ldap_memfree( pcSrchAttr ) ;
		if ( pBer != NULL ) { 
			ber_free( pBer, 0 ) ;
		} 
		ldap_memfree( pSrchResult ) ;

		return M4_ERROR ;
	}

	// Do a copy of Root Domain Naming Context value.

	pcRootDomainNCtx = new m4char_t[strlen( ppcValues[0] ) + 1] ;
	strcpy( pcRootDomainNCtx, ppcValues[0] ) ;

	// Free search result structs.

	ldap_value_free( ppcValues ) ;
	ldap_memfree( pcSrchAttr ) ;
	if ( pBer != NULL ) { 
		ber_free( pBer, 0 ) ;
	} 
	ldap_memfree( pSrchResult ) ;

	
	// Second Search: the user.
	
	iRes = _getADUserID( ai_pccUser, pcUserID ) ;

	if( iRes == M4_ERROR )
	{
		SETCODEF( M4_LDAP_E_AD_EXTRACTING_USER_ID, ERRORLOG, "#*s1*#%s#", ai_pccUser ) ;

		delete pcRootDomainNCtx ;
	}

	pcFilterAccountName = new m4char_t[ strlen( MSAD_FILTER_USR_ACCOUNT_NAME ) + strlen(pcUserID) + 1 ] ;

	sprintf( pcFilterAccountName, MSAD_FILTER_USR_ACCOUNT_NAME, pcUserID ) ;

	delete pcUserID ;

	ppcSearchAttrsToGet[0] = MSAD_ATTR_DN ;

	iRes = ldap_search_ext_s(ai_pHandle, pcRootDomainNCtx, LDAP_SCOPE_SUBTREE, pcFilterAccountName, ppcSearchAttrsToGet, M4_FALSE, NULL, NULL, NULL, 1, &pSrchResult) ;

	delete pcFilterAccountName ;
	
	if( iRes != LDAP_SUCCESS )
	{
		SETCODEF( M4_LDAP_E, ERRORLOG, "#*s1*#%s#", ldap_err2string (iRes) ) ;

		delete pcRootDomainNCtx ;

		return( M4_ERROR ) ; 
	}

	pSrchEntry = ldap_first_entry( ai_pHandle, pSrchResult ) ;

	if ( pSrchEntry == NULL )
	{ 
		delete pcRootDomainNCtx ;
		ldap_memfree( pSrchResult ) ;

		return M4_ERROR ;
	}

	pcSrchAttr = ldap_first_attribute( ai_pHandle, pSrchEntry, &pBer ) ;

	if ( pcSrchAttr == NULL ) 
	{ 
		delete pcRootDomainNCtx ;
		if ( pBer != NULL ) { 
			ber_free( pBer, 0 ) ;
		} 
		ldap_memfree( pSrchResult ) ;

		return M4_ERROR ;
	}

	ppcValues = ldap_get_values( ai_pHandle, pSrchEntry, pcSrchAttr ) ;

	if ( ppcValues == NULL ) 
	{ 
		delete pcRootDomainNCtx ;
		ldap_memfree( pcSrchAttr ) ;
		if ( pBer != NULL ) { 
			ber_free( pBer, 0 ) ;
		} 
		ldap_memfree( pSrchResult ) ;

		return M4_ERROR ;
	}

	// Output DN argument.

	ao_pcUserDN = new m4char_t[strlen( ppcValues[0] ) + 1] ;
	strcpy( ao_pcUserDN, ppcValues[0] ) ;

	// Free search result structs.

	delete pcRootDomainNCtx ;
	ldap_value_free( ppcValues ) ;
	ldap_memfree( pcSrchAttr ) ;
	if ( pBer != NULL ) { 
		ber_free( pBer, 0 ) ;
	} 
	ldap_memfree( pSrchResult ) ;


	return( M4_SUCCESS ); 
}


// param:
//		1. Ldap session handle
//		2. User, must be in short format (domain\usr or usr@domain.com)
//		   or long format (distinguised name)
// return: 
//		M4_TRUE / M4_FALSE
//
/* Checks if user string is in short format									*/
/* Characters @ and \ could be in the DN so we cannot parse the string		*/
/* We assume it is a DN so we launch a search in that node, no return or	*/
/* error means it is in short format										*/

m4bool_t _isADShortFormat( LDAP * ai_pHandle, m4pcchar_t ai_pcUsr )
{

	m4int_t iRes = LDAP_SUCCESS ;
	LDAPMessage* pSrchResult = NULL ;

	iRes = ldap_search_ext_s( ai_pHandle, ai_pcUsr, LDAP_SCOPE_ONELEVEL, MSAD_FILTER_ALL, NULL, M4_FALSE, NULL, NULL, NULL, 1, &pSrchResult ) ;

	if( iRes == LDAP_INVALID_DN_SYNTAX )
	{
		return M4_TRUE ;
	}
	
	if( iRes == LDAP_SUCCESS )
	{
		ldap_memfree( pSrchResult ) ;
	}

	return M4_FALSE ;

}

// param:
//		1. User, must be in short format (domain\usr or usr@domain.com)
//		2. Output pointer with user Id. Caller must free it.
// return: 
//		M4_TRUE / M4_FALSE
//
/* Returns user Id.															*/

m4return_t _getADUserID( m4pcchar_t ai_pccUser, m4pchar_t &ao_pcUserID )
{

	m4return_t iRet = M4_ERROR ;
	m4int_t iLen = 0 ;
	m4pchar_t pcPos = NULL ;
	
	
	if( ao_pcUserID != NULL )
	{
		delete ao_pcUserID ;
		ao_pcUserID = NULL ;
	}
	
	// Try userId@domain.com format.

	pcPos = (m4pchar_t)strstr( ai_pccUser, "@" ) ;

	if( pcPos != NULL )
	{
		iLen = pcPos - ai_pccUser ;
		pcPos = (m4pchar_t)ai_pccUser ;
	}
	else
	{
		// Try domain\userId format.
		pcPos = (m4pchar_t)strstr( ai_pccUser, "\\" ) ;

		if( pcPos != NULL )
		{
			pcPos++ ;
			iLen = strlen( pcPos ) ;
		}
		else
		{
			iRet = M4_ERROR ;
		}
	}

	if( iLen > 0 )
	{
		ao_pcUserID = new m4char_t[iLen + 1] ;

		memcpy( ao_pcUserID, pcPos, iLen ) ;
		*( ao_pcUserID + iLen ) = 0 ;

		iRet = M4_SUCCESS ;
	}

	return iRet ;
	
}