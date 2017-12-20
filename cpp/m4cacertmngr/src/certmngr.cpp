//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cacertmngr.dll
// File:                certmngr.cpp   
// Project:             Mind3x      
// Author:              Meta Software M.S. , S.A
// Date:                03/07/2002
// Language:            C++
//
// Definition:
//    This module declares general API for the certificate manager
//
//==============================================================================

#include "certmngr.hpp"
#include "m4string.hpp"
#include "chmngauthent.hpp"
#include "chmngdomainauthent.hpp"
#include "m4cacertapi.hpp"
#include "dm.hpp"
#include "cllgadap.hpp"
#include "execontx.hpp"
#include "m4srvres.hpp"
#include "sessioncontext.hpp"
#include "clseschnexport.hpp"
#include <m4cryptc.hpp>
#include "m4uuid.hpp"
#include <time.h>

extern "C"{
#include "base32.h"
#include "compute.h"
}

#include "channel.hpp"
#include "node_knl.hpp"
#include "cldates.hpp"


// defines
#define DEF_WEBSSO_ALIAS "M4SSOWeb"
#define AUTHORIZED_ROLE "M4ADM"
#define CERT_TOKEN_SERIAL_PREFIX ";Serial="

#define	M4CH_DUMMY_B2	"\x2\x3\x8\x1\x9\x6\x5\xF\xD\x8\xF\x8\xB\x4\x7\x7\xF\x6\x4\xE\x4\xE\xE\x5\xB\xE\x6\x7\xF\x3\x7\x2\xB\x6\xC\x4\xD\xB\x6\x7\x8\x3\xE\x8\xA\xB\x7\x9\x6\x6\x7\x9\xB\x6\x4\x8\xE\xF\x3\x8\x8\xC\xA\x0\x4\x2\x6\x8\x4\xC\x4\xD\x9\x2\x8\x4\x8\x8\x6\x8\xF\x1\x1\xF\xC\x1\x7\xE"
#define	M4CH_DUMMY_F	"\x7\xB\xF\x3\x6\xA\xB\x7\x4\xC\xC\x1\x9\x4\xE\x6\x3\xD\x2\xF\xA\x2\x6\x4\x8\xC\xB\x3\x4\x2\x1\xF\xC\xA\xC\x3\x1\x7\x3\xC\x7\x5\x2\x1\xB\x5\x2\x1\xF\xF\xE\xA\x1\x5\x1\xD\x2\xD\x5\x6\xA\xB\x2\x0\x8\x3\xB\xF\x9\x8\x4\xC\x2\x4\x6\x8\xA\xA\x7\x9\x4\xF\x2\x1\xA\x5\xE\x3"
#define	M4CH_DUMMY_T	"\x9\x3\xB\x6\x3\x4\xC\xE\xE\x1\x7\x3\xA\x2\xF\x8\x9\xF\x1\x7\x4\xC\xB\x6\xD\xC\x1\xA\x7\x3\x9\xA\x1\xC\xB\x1\x5\x8\x3\x4\xC\xC\x1\x6\x3\xB\xF\xF\x4\x2\xA\x6\x8\x7\xD\xE\x6\x1\x7\xA\x4\x3\xE\x0\x3\x1\xA\x6\x4\x6\x7\xA\xA\x5\xC\x7\x2\xB\xE\xF\x1\x5\x1\xA\xC\xC\x2\x5"

// auxiliary function
static m4return_t DecodeRequestValue(m4string_t sCertificate2Way, m4string_t &sCertificate) {

	// prepares output buffer
	m4int_t		iLenOut = 0;
	m4pchar_t	pcCertReqDec = NULL;
			
	if (DecryptTwoWay(sCertificate2Way.c_str(), strlen(sCertificate2Way.c_str()), M4CH_DUMMY_B2 + 23, strlen (M4CH_DUMMY_B2 + 23), pcCertReqDec, iLenOut) != M4_SUCCESS) 
	{
		return M4_ERROR;
	}

	// recovers the result
	sCertificate = pcCertReqDec;

	delete [] pcCertReqDec;
	return M4_SUCCESS;
}


// item executor
ClCertManager::ClCertManager(ClExecuteItemI *ai_poItemExecutor)
{
	m_poItemExecutor = ai_poItemExecutor;
}

m4return_t ClCertManager::GetCertificate (m4pchar_t ai_pszUserName , m4pchar_t & ao_szCertificate , m4uint32_t &ao_iCertificateLen , m4pchar_t &ao_szSessContext , m4uint32_t &ao_iSessContextLen)
{
	if (m_poItemExecutor == NULL) {
		return M4_ERROR;
	}
	
	m4string_t szCertificate ;
	m4string_t strUserName ;

	ClChManagerAuthenticator oAuthenticator ( m_poItemExecutor, NULL ) ;

	if ( ai_pszUserName == NULL)  {
		return M4_ERROR;
	}

	strUserName = ai_pszUserName ;

	if ( M4_ERROR == GetM4Certificate ( & oAuthenticator , strUserName , szCertificate ) )
	{
		return M4_ERROR ;
	}

	m4uint32_t iOrigLen = ao_iCertificateLen;
	ao_iCertificateLen = szCertificate.size () ;

	if (ao_szCertificate == NULL) {
		ao_szCertificate = new m4char_t [ao_iCertificateLen + 1] ;
	}
	else {
		if (iOrigLen < ao_iCertificateLen) {
			return M4_WARNING;
		}
	}

	// to be implemented
	ao_iSessContextLen = 0;

	memcpy ( ao_szCertificate , szCertificate.c_str() , ao_iCertificateLen ) ;

	ao_szCertificate [ao_iCertificateLen] = 0;

	return M4_SUCCESS ;
}

m4return_t ClCertManager::CheckCertificate (m4pcchar_t ai_pcCertType, m4string_t &ao_sUserName , m4pchar_t ai_szCertificate , m4uint32_t ai_iCertificateLen)
{
	if (m_poItemExecutor == NULL) {
		return M4_ERROR;
	}
	
	m4return_t res = M4_SUCCESS;

	// for now just M4 type
	if ( strcmp ( "M4_CERTIFICATE" , ai_pcCertType ) )
	{
		return M4_ERROR ;
	}

	ClChManagerAuthenticator oAuthenticator ( m_poItemExecutor, NULL ) ;

	res = CheckM4Certificate( &oAuthenticator , ao_sUserName , ai_szCertificate);

	return res;
}


// exported functions
m4return_t GenerateKeys(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext) 
{
	m4pchar_t			pcPrivate = NULL, pcPublic = NULL;
	m4uint32_t			iPrivateLen = 0, iPublicLen = 0;
	m4return_t			iRes = M4_SUCCESS;
	m4pcchar_t			pccNodeId = NULL;
	
	// default return type
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// creates a new item execution environment
	ClVMRunContext *poContext = (ClVMRunContext*) ai_pvContext;

	ClLogonAdaptor *poLA = poContext->m_pLA;
	ClAccess *poAccess = poContext->m_pAccess;
	ai_pvReturn = -1;

	if (poLA == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateKeys: Invalid Logon Adaptor.");
		return M4_ERROR;
	}

	ClExecuteItemI *poItemExecutor = poLA->GetExecuteItem();
	if (poItemExecutor == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateKeys: Invalid Item Executor.");
		return M4_ERROR;
	}

	// Obtener el nombre del nodo.
	pccNodeId = poAccess->Node->GetpNodeDef()->Id();
	if (pccNodeId == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateKeys: Invalid Node Id.");
		delete poItemExecutor;
		return M4_ERROR;
	}

	// Generar claves pública y privada.
    iRes = GeneratePublicPrivateKey ( pcPrivate, iPrivateLen, pcPublic , iPublicLen);
	if (iRes != M4_SUCCESS) 
	{
		SETCODEF(M4_ERR_CACERTMNGR_GENERATE_KEYS, ERRORLOG,"#*s1*#%s#","GenerateKeys");
	}

	// sets the key pair
	if (iRes == M4_SUCCESS)
	{
		poItemExecutor->SetAccess(poAccess);
		
		iRes = poItemExecutor->SetBinaryItem(pccNodeId,"PRIVATE_KEY",pcPrivate,iPrivateLen);

		if (iRes != M4_SUCCESS)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateKeys: Error setting key pair.");
		}
	}
	
	// sets the key pair
	if (iRes == M4_SUCCESS)
	{
		iRes = poItemExecutor->SetBinaryItem(pccNodeId,"PUBLIC_KEY",pcPublic,iPublicLen);

		if (iRes != M4_SUCCESS)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateKeys: Error setting key pair.");
		}
	}

	// frees memory
	if (poItemExecutor != NULL)
	{
		delete poItemExecutor;
	}

	if (pcPublic != NULL)
	{
		delete [] pcPublic;
	}

	if (pcPrivate != NULL)
	{
		delete [] pcPrivate;
	}
	
	ai_pvReturn = iRes;
	return M4_SUCCESS;
}

m4return_t GenerateSessionContext(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,
											 m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext) {

	m4return_t iRes = M4_ERROR;

	// checks the arguments
	if (ai_iLongArg < 1) {
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateSessionContext: Invalid number of argments: Less than 1.");
		return M4_ERROR;
	}
	
	ClVMRunContext *poContext = (ClVMRunContext*) ai_pvContext;
	ClLogonAdaptor *poLA = poContext->m_pLA;

	if (poLA == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateSessionContext: Invalid Logon Adaptor.");
		return M4_ERROR;
	}

	// gets the session access
	ClChannel *poChannel = poLA->GetSessionChannelPointer();
	if (poChannel == NULL) {
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateSessionContext: Invalid Session Object.");
		return M4_ERROR;
	}

	// gets the session export object
	IClSesChnExport *poExport = poLA->GetSessionExport();
	if (poExport == NULL) {
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "GenerateSessionContext: Invalid Session Export object.");
		return M4_ERROR;
	}

	// generates the context
	ClSessionContext oContext;
	iRes = poExport->Export(poChannel,oContext);
	if (iRes != M4_SUCCESS) {
		delete poExport;
		return M4_ERROR;
	}

	// serializes the context
	m4pchar_t pcBuffer = NULL;
	m4uint32_t iBufferLen;
	iRes = oContext.Serialize(pcBuffer,iBufferLen);
	if (iRes != M4_SUCCESS) {
		delete poExport;
		return M4_ERROR;
	}

	pcBuffer = new m4char_t[iBufferLen];
	iRes = oContext.Serialize(pcBuffer,iBufferLen);
	if (iRes != M4_SUCCESS) {
		delete poExport;
		return M4_ERROR;
	}

	// sets the output value
	ai_pvArg[0].Set(pcBuffer,M4_TRUE);
	ai_pvReturn = iRes;

	// deletes variables
	delete [] pcBuffer;
	delete poExport;

	return iRes;
}

m4return_t ExportKeysToFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,
											 m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext) {

	// default return type
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// checks the arguments
	if (ai_iLongArg != 2 && ai_iLongArg != 3)
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_N_ARGS, ERRORLOG, "#*s1*#%s#%d#%d#", "ExportKeysToFile", 2, (m4uint32_t)ai_iLongArg);
		return M4_SUCCESS;
	}

	// gets the output path
	string sFileName;
	if ((ai_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR) && (ai_pvArg[0].Data.PointerChar != NULL))
	{
		sFileName = (m4pchar_t) ai_pvArg[0].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_KEY_FILE, ERRORLOG,"#*s1*#%s#","ExportKeysToFile");
		return M4_SUCCESS;
	}

	// gets the output path
	string sKeyFilePass;
	if ((ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR) && (ai_pvArg[1].Data.PointerChar != NULL))
	{
		sKeyFilePass = (m4pchar_t) ai_pvArg[1].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_KEY_PASS, ERRORLOG,"#*s1*#%s#","ExportKeysToFile");
		return M4_SUCCESS;
	}

	// Verificar si viene el tercer argumento (Id. Certificado). Solo si estamos trabajando
	// con claves multidominio.
	m4int32_t	iCertificateId = 0;
	m4bool_t	bCertificate = M4_FALSE;
	if (ai_iLongArg == 3)
	{
		if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER)
		{
			bCertificate = M4_TRUE;
			iCertificateId = (m4uint32_t) ai_pvArg[2].Data.DoubleData;
		}
		else
		{
			SETCODEF(M4_ERR_CACERTMNGR_INVALID_CERTIFICATE_ID, ERRORLOG,"#*s1*#%s#","ExportKeysToFile");
			return M4_SUCCESS;
		}
	}

	// creates a new item execution environment
	ClVMRunContext *poContext = (ClVMRunContext*) ai_pvContext;

	ClLogonAdaptor *poLA = poContext->m_pLA;
	ClAccess *poAccess = poContext->m_pAccess;
	ai_pvReturn = -1;

	if (poLA == NULL)
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_LA, ERRORLOG, "#*s1*#%s#", "ExportKeysToFile");
		return M4_ERROR;
	}

	// checks the user role
	if (strcmp(poLA->GetRole(),AUTHORIZED_ROLE) != 0) {
		SETCODEF(M4_ERR_CACERTMNGR_NOT_AUTHORIZED, ERRORLOG, "#*s1*#%s#", "ExportKeysToFile", AUTHORIZED_ROLE);
		return M4_SUCCESS;
	}


	ClExecuteItemI *poItemExecutor = poLA->GetExecuteItem();

	if (poItemExecutor == NULL)
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_EXEC_ITEM, ERRORLOG, "#*s1*#%s#", "ExportKeysToFile");
		return M4_ERROR;
	}

	m4return_t iRes = M4_SUCCESS;

	// creates a authenticator
	ClICaAuthenticator * poAuthenticator = NULL;
	if (bCertificate == M4_FALSE)
	{
		poAuthenticator = new ClChManagerAuthenticator(poItemExecutor, NULL);
	}
	else
	{
		poAuthenticator = new ClChManagerDomainAuthenticator(poItemExecutor->GetChannelManager(), iCertificateId, NULL);
	}

	// creates the certificate factory
	iRes = ExportCertificate(poAuthenticator,DEF_WEBSSO_ALIAS,sFileName,sKeyFilePass);
	if (iRes != M4_SUCCESS) 
	{
		SETCODEF(M4_ERR_CACERTMNGR_CERT_EXPORT_FAIL, ERRORLOG,"#*s1*#%s#","ExportKeysToFile");

		delete poItemExecutor;
		delete poAuthenticator;
		return M4_SUCCESS;
	}
	
	// frees memory
	delete poItemExecutor;
	delete poAuthenticator;

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	ai_pvReturn = iRes;
	return iRes;
}


// receives the request password and the user
m4return_t IsValidM4SSOCert(m4VariantType *aio_pvArg, m4uint32_t ai_iLongArg,
											 m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext) 
{
	// default return type
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

	// checks the arguments 
	if (ai_iLongArg < 3 || ai_iLongArg > 4)
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_N_ARGS, ERRORLOG, "#*s1*#%s#%d#%d#", "IsValidM4SSOCert", 4, (m4uint32_t)ai_iLongArg);
		return M4_ERROR;
	} 

	// gets the certificate
	m4string_t sCertificate2Way;
	if ((aio_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR) && (aio_pvArg[0].Data.PointerChar != NULL))
	{
		sCertificate2Way = (m4pchar_t) aio_pvArg[0].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_M4CERT_ARG, ERRORLOG,"#*s1*#%s#","IsM4SSOCert");
		return M4_SUCCESS;
	}

	// gets the user
	m4string_t sUserId;
	if ((aio_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR) && (aio_pvArg[1].Data.PointerChar != NULL))
	{
		sUserId = (m4pchar_t) aio_pvArg[1].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_USER_ARG, ERRORLOG,"#*s1*#%s#","IsM4SSOCert");
		return M4_SUCCESS;
	}

	// gets the go on output argument
	if ((aio_pvArg[2].Type != M4CL_CPP_TYPE_NULL) && (aio_pvArg[2].Type != M4CL_CPP_TYPE_NUMBER)) 
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_ISVALID_ARG, ERRORLOG,"#*s1*#%s#","IsM4SSOCert");
		return M4_SUCCESS;
	}

	// sets the default output value to stop
	aio_pvArg[2].Type = M4CL_CPP_TYPE_NUMBER;
	aio_pvArg[2].Data.DoubleData = 0;

	// sets the default domain value to null
	if (ai_iLongArg == 4)
	{
		aio_pvArg[3].Type = M4CL_CPP_TYPE_NULL;
	}

	// creates a new item execution environment
	ClVMRunContext *poContext = (ClVMRunContext*) ai_pvContext;
	if (poContext == NULL) {

		// Not inside VM?
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_LA, ERRORLOG, "#*s1*#%s#", "IsValidM4SSOCert");
		return M4_SUCCESS;
	}

	ClLogonAdaptor *poLA = poContext->m_pLA;
	ClAccess *poAccess = poContext->m_pAccess;
	ClChannel *poSessionChannel = poAccess->GetpChannel()->GetpChannelManager()->GetSessionChannel();
	ai_pvReturn = -1;

	if (poLA == NULL)
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_LA, ERRORLOG, "#*s1*#%s#", "IsValidM4SSOCert");
		return M4_SUCCESS;
	}

	// checks the state
	if (poLA->GetCurrentState() != M4CL_LOG_ONLINE) {
		// initial logon, no SSO here
		ai_pvReturn.Data.DoubleData = M4_SUCCESS;
		return M4_SUCCESS;
	}

	// decodes the argument
	m4string_t sCertificate;
	m4return_t iRes = DecodeRequestValue(sCertificate2Way,sCertificate);
	if (iRes != M4_SUCCESS) {
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_M4CERT_ARG, ERRORLOG,"#*s1*#%s#","IsM4SSOCert");
		return M4_SUCCESS;
	}

	// gets the execution item
	ClExecuteItemI *poItemExecutor = poLA->GetExecuteItem();

	if (poItemExecutor == NULL)
	{
		SETCODEF(M4_ERR_CACERTMNGR_INVALID_EXEC_ITEM, ERRORLOG, "#*s1*#%s#", "IsValidM4SSOCert");
		return M4_SUCCESS;
	}

	m4bool_t bHasSerial = M4_FALSE; 
	m4bool_t bIsValid = M4_FALSE;
	m4string_t strDomain = ""; 
	
	m4uint32_t iSerial = GetSerialFromCredential ( sCertificate , bHasSerial ); 

	// if there is no serial, it is one of the old certificates

	if ( !bHasSerial ) 
	{		
		ClChManagerAuthenticator oAuthenticator( poItemExecutor, poSessionChannel ); 

		// checks the certificate
		iRes = CheckM4WSSOCertificate(&oAuthenticator, sCertificate, sUserId, bIsValid, strDomain);
		if (iRes != M4_SUCCESS) {
			// frees memory
			delete poItemExecutor;

			SETCODEF(M4_ERR_CACERTMNGR_INVALID_M4CERT_CHECK, ERRORLOG,"#*s1*#%s#","IsValidM4SSOCert");
			return M4_SUCCESS;
		}
	}
	else
	{
		// else it is one of the new certificates	
		ClChManagerDomainAuthenticator oAuthenticator(poItemExecutor->GetChannelManager(), iSerial, poSessionChannel);
		// checks the certificate
		iRes = CheckM4WSSOCertificate(&oAuthenticator, sCertificate, sUserId, bIsValid, strDomain);
		if (iRes != M4_SUCCESS) 
		{
			// frees memory
			delete poItemExecutor;

			SETCODEF(M4_ERR_CACERTMNGR_INVALID_M4CERT_CHECK, ERRORLOG,"#*s1*#%s#","IsValidM4SSOCert");
			return M4_SUCCESS;
		}	
	}
	
	// frees memory
	delete poItemExecutor;

	// sets the output value
	aio_pvArg[2].Data.DoubleData = (bIsValid ? 1 : 0);	

	// sets the domain value if the cert is valid and the domain is not null		 	
	if (ai_iLongArg == 4 && bIsValid == M4_TRUE) 
	{
		if ( !strDomain.empty() ) 
		{
			aio_pvArg[3].Type=M4CL_CPP_TYPE_STRING_VAR;
			aio_pvArg[3].Data.PointerChar = new m4char_t[strlen(strDomain.c_str()) + 1];			
			strcpy(aio_pvArg[3].Data.PointerChar, strDomain.c_str());
		}
	}

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;

	return M4_SUCCESS;
}


//
// Genera una clave secreta para autentificación de 2 factores
//
m4return_t	Generate2FASecretKey( m4VariantType *aio_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	m4return_t	iResult = M4_ERROR ;
	m4int_t		iLength = -1 ;
	m4int_t		iSize = -1 ;
	m4pchar_t	pcEncryptedKey = NULL ;
	m4char_t	acBytes[ M4CL_VM_SIZE_UUID + 1 ] ;
	uint8_t		acKey[ 32 + 1 ] ;

	
	ai_pvReturn.SetNull() ;

	if( ai_iLongArg != 1 )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_N_ARGS, ERRORLOG, "#*s1*#%s#%d#%d#", "Generate2FASecretKey", 1, ai_iLongArg ) ;
		return( M4_ERROR ) ;
	}

	aio_pvArg[ 0 ].SetNull() ;


	// Se ontienen los bytes aleatorios de longitud 24
	memset( acBytes, 0, M4CL_VM_SIZE_UUID + 1 ) ;
	iResult = GetUUID( acBytes, sizeof( m4char_t ) * ( M4CL_VM_SIZE_UUID + 1 ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_SUCCESS ) ;
	}

	// Se codifica una clave de 16 bytes a partir de los 10 bytes aleatorios del 5 al 14
	// Si se quiere hacer de 32 basta con poner los 20 bytes aleatorios del 5 al 24
	iLength = 10 ;
	memset( acKey, 0, 32 + 1 ) ;
	base32_encode( (const uint8_t *)acBytes + 4, iLength, acKey, sizeof( uint8_t ) * ( 32 + 1 ) ) ;

	iResult = EncryptTwoWay( acBytes + 4, iLength, M4CH_DUMMY_F + 23, strlen( M4CH_DUMMY_F + 23 ) * sizeof(m4char_t), pcEncryptedKey, iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		SETCODEF( M4_ERR_CACERTMNGR_ENCRYPT_ERROR, ERRORLOG, "#*s1*#%s#", "Generate2FASecretKey" ) ;
		return( M4_SUCCESS ) ;
	}

	ai_pvReturn.SetString( (const m4pchar_t)acKey ) ;
	aio_pvArg[ 0 ].SetString( pcEncryptedKey ) ;

	delete( pcEncryptedKey ) ;

	return( M4_SUCCESS ) ;
}


//
// Valida un código de verificación para autentificación de 2 factores
//
m4return_t	Check2FAVerificationCode( m4VariantType *aio_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	m4return_t		iResult = M4_ERROR ;
	m4int_t			iSize = 0 ;
	int				i = 0 ;
	int				iCode = 0 ;
	int				iVerificationCode = 0 ;
	unsigned long	iWindow = 0 ;
	m4pchar_t		pcBytes = NULL ;
	m4pcchar_t		pccEncryptedKey = NULL ;

	
	ai_pvReturn.SetDouble( M4_FALSE ) ;

	if( ai_iLongArg != 3 )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_N_ARGS, ERRORLOG, "#*s1*#%s#%d#%d#", "Check2FAVerificationCode", 3, ai_iLongArg ) ;
		return( M4_ERROR ) ;
	}

	aio_pvArg[ 2 ].SetNull() ;

	if( aio_pvArg[ 0 ].Type != M4CL_CPP_TYPE_STRING_VAR || aio_pvArg[ 0 ].Data.PointerChar == NULL || *aio_pvArg[ 0 ].Data.PointerChar == '\0' )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_SECRET_KEY_ARG, ERRORLOG, "#*s1*#%s#", "Check2FAVerificationCode" ) ;
		return( M4_SUCCESS ) ;
	}

	pccEncryptedKey = aio_pvArg[ 0 ].Data.PointerChar ;

	if( aio_pvArg[ 1 ].Type != M4CL_CPP_TYPE_NUMBER )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_VERIFICATION_CODE_ARG, ERRORLOG, "#*s1*#%s#", "Check2FAVerificationCode" ) ;
		return( M4_SUCCESS ) ;
	}

	iVerificationCode = int( aio_pvArg[ 1 ].Data.DoubleData ) ;


	if( iVerificationCode < 0 || iVerificationCode >= 1000000 )
	{
		// Si el código no es positivo de 6 dígitos no es válido
		return( M4_SUCCESS ) ;
	}

	iResult = DecryptTwoWay( pccEncryptedKey, strlen( pccEncryptedKey ), M4CH_DUMMY_F + 23, strlen( M4CH_DUMMY_F + 23 ) * sizeof(m4char_t), pcBytes, iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		SETCODEF( M4_ERR_CACERTMNGR_DECRYPT_ERROR, ERRORLOG, "#*s1*#%s#", "Check2FAVerificationCode" ) ;
		return( M4_SUCCESS ) ;
	}

	// Se calcula la ventana actual de 30 segundos
	long ll = time( NULL ) ;
	iWindow = time( NULL ) / 30 ;

	// Se verifica pa la ventana actual
	iCode = compute_code( (uint8_t*)pcBytes, iSize, iWindow ) ;

	if( iCode == iVerificationCode )
	{
		aio_pvArg[ 2 ].SetDouble( 0 ) ;
		ai_pvReturn.SetDouble( M4_TRUE ) ;
	}
	else
	{
		// Se verifica para las ventanas adyacentes hasta 6 horas por cada lado
		for( i = 1 ; i < 720 ; i++ )
		{
			iCode = compute_code( (uint8_t*)pcBytes, iSize, iWindow - i ) ;
		
			if( iCode == iVerificationCode )
			{
				aio_pvArg[ 2 ].SetDouble( -i ) ;
				ai_pvReturn.SetDouble( M4_TRUE ) ;
				break ;
			}

			iCode = compute_code( (uint8_t*)pcBytes, iSize, iWindow + i ) ;
		
			if( iCode == iVerificationCode )
			{
				aio_pvArg[ 2 ].SetDouble( i ) ;
				ai_pvReturn.SetDouble( M4_TRUE ) ;
				break ;
			}
		}
	}

	delete( pcBytes ) ;
	return( M4_SUCCESS ) ;
}



//
// Genera un ticket temporal encriptado para la generación de la clave secreta de un usuario
//
m4return_t	EncodeSecretKeyTicket( m4VariantType *aio_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	m4return_t	iResult = M4_ERROR ;
	m4int_t		iLength = -1 ;
	m4int_t		iSize = -1 ;
	m4date_t	dInterval = 0 ;
	m4date_t	dNow = 0 ;
	m4pcchar_t	pccUser = NULL ;
	m4pchar_t	pcTicket = NULL ;
	m4pchar_t	pcEncryptedTicket = NULL ;


	ai_pvReturn.SetNull() ;

	if( ai_iLongArg != 2 )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_N_ARGS, ERRORLOG, "#*s1*#%s#%d#%d#", "EncodeSecretKeyTicket", 2, ai_iLongArg ) ;
		return( M4_ERROR ) ;
	}

	if( aio_pvArg[ 0 ].Type != M4CL_CPP_TYPE_STRING_VAR || aio_pvArg[ 0 ].Data.PointerChar == NULL || *aio_pvArg[ 0 ].Data.PointerChar == '\0' )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_USER_ARG, ERRORLOG, "#*s1*#%s#", "EncodeSecretKeyTicket" ) ;
		return( M4_SUCCESS ) ;
	}

	pccUser = aio_pvArg[ 0 ].Data.PointerChar ;

	if( aio_pvArg[ 1 ].Type != M4CL_CPP_TYPE_NUMBER )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_INTERVAL_ARG, ERRORLOG, "#*s1*#%s#", "EncodeSecretKeyTicket" ) ;
		return( M4_SUCCESS ) ;
	}

	dInterval = aio_pvArg[ 1 ].Data.DoubleData ;


	iLength = strlen( pccUser ) ;
	pcTicket = new m4char_t[ M4_SECOND_END + 1 + iLength + 1 ] ;

	ClActualDate( dNow ) ;
	ClDateToString( dNow + dInterval, pcTicket, M4_SECOND_END + 1 ) ;
	memcpy( pcTicket + M4_SECOND_END + 1, pccUser, iLength + 1 ) ;

	iResult = EncryptTwoWay( pcTicket, M4_SECOND_END + 1 + iLength + 1, M4CH_DUMMY_T + 23, strlen( M4CH_DUMMY_T + 23 ) * sizeof(m4char_t), pcEncryptedTicket, iSize ) ;

	delete( pcTicket ) ;

	if( iResult != M4_SUCCESS )
	{
		SETCODEF( M4_ERR_CACERTMNGR_ENCRYPT_ERROR, ERRORLOG, "#*s1*#%s#", "EncodeSecretKeyTicket" ) ;
		return( M4_SUCCESS ) ;
	}

	// Hay que eliminar los carateres raros
	if( *pcEncryptedTicket < '0' )
	{
		*pcEncryptedTicket += 10 ;
	}
	else
	{
		*pcEncryptedTicket -= 6 ;
	}

	ai_pvReturn.SetString( pcEncryptedTicket ) ;
	delete( pcEncryptedTicket ) ;

	return( M4_SUCCESS ) ;
}



//
// Obtiene el usuario de un ticket temporal encriptado para la generación de la clave secreta si sigue siendo válido
//
m4return_t	DecodeSecretKeyTicket( m4VariantType *aio_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{

	m4return_t	iResult = M4_ERROR ;
	m4int_t		iSize = -1 ;
	m4date_t	dDate = 0 ;
	m4date_t	dNow = 0 ;
	m4pchar_t	pcEncryptedTicket = NULL ;
	m4pchar_t	pcTicket = NULL ;


	ai_pvReturn.SetNull() ;

	if( ai_iLongArg != 1 )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_N_ARGS, ERRORLOG, "#*s1*#%s#%d#%d#", "DecodeSecretKeyTicket", 1, ai_iLongArg ) ;
		return( M4_ERROR ) ;
	}

	if( aio_pvArg[ 0 ].Type != M4CL_CPP_TYPE_STRING_VAR || aio_pvArg[ 0 ].Data.PointerChar == NULL || *aio_pvArg[ 0 ].Data.PointerChar == '\0' )
	{
		SETCODEF( M4_ERR_CACERTMNGR_INVALID_TICKET_ARG, ERRORLOG, "#*s1*#%s#", "DecodeSecretKeyTicket" ) ;
		return( M4_SUCCESS ) ;
	}

	pcEncryptedTicket = aio_pvArg[ 0 ].Data.PointerChar ;


	// Hay que volver a poner los carateres raros
	if( *pcEncryptedTicket <= '9' )
	{
        *pcEncryptedTicket -= 10 ;
	}
    else
	{
		*pcEncryptedTicket += 6 ;
	}

	iResult = DecryptTwoWay( pcEncryptedTicket, strlen( pcEncryptedTicket ), M4CH_DUMMY_T + 23, strlen( M4CH_DUMMY_T + 23 ) * sizeof(m4char_t), pcTicket, iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		SETCODEF( M4_ERR_CACERTMNGR_DECRYPT_ERROR, ERRORLOG, "#*s1*#%s#", "DecodeSecretKeyTicket" ) ;
		return( M4_SUCCESS ) ;
	}

	if( iSize < M4_SECOND_END + 1 )
	{
		delete( pcTicket ) ;
		SETCODE( M4_ERR_CACERTMNGR_INVALID_TICKET, ERRORLOG ) ;
		return( M4_SUCCESS ) ;
	}

	dDate = ClStringToDate( pcTicket ) ;
	ClActualDate( dNow ) ;

	if( dNow > dDate )
	{
		delete( pcTicket ) ;
		SETCODEF( M4_ERR_CACERTMNGR_OLD_DATED_TICKET, ERRORLOG, "#*s1*#%s#", pcTicket + M4_SECOND_END + 1 ) ;
		return( M4_SUCCESS ) ;
	}

	ai_pvReturn.SetString( pcTicket + M4_SECOND_END + 1 ) ;
	delete( pcTicket ) ;

	return( M4_SUCCESS ) ;
}

m4void_t CertManagerSetSessionChannelProperty(ClChannel *ai_poSessionChannel, m4pcchar_t ai_sPropertyId, m4string_t &ai_sPropertyValue, m4bool_t ai_bIsPublic)
{
	if (ai_poSessionChannel == NULL)
	{
		// no session in context so we could ignore
		// this should not happen when at login time
		return;
	}

	// we have session object so we're at login time

	// lets check parameters
	if (ai_sPropertyId == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error in initialization. Sessin property id is null");
		return;
	}

	// sets session channel property
	if (ai_bIsPublic == M4_TRUE)
	{
		m4return_t iResult = ai_poSessionChannel->Property.SetOrNewProperty((m4pchar_t)ai_sPropertyId, (const m4pchar_t)ai_sPropertyValue.c_str(), M4_PROP_LN4_STATE);
		if (iResult != M4_SUCCESS)
		{
			m4string_t sError = m4string_t("Authenticate: Error creating property in session object for property ") + ai_sPropertyId;
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, sError.c_str());
			return;
		}
	}
	else
	{
		m4return_t iResult = ai_poSessionChannel->Property.NewPrivateProperty((m4pchar_t)ai_sPropertyId, M4_PROP_READ | M4_PROP_WRITE);
		if (iResult != M4_SUCCESS)
		{
			m4string_t sError = m4string_t("Authenticate: Error creating property in session object for property ") + ai_sPropertyId;
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, sError.c_str());
			return;
		}

		iResult = ai_poSessionChannel->Property.SetPrivatePropValue((m4pchar_t)ai_sPropertyId, (const m4pchar_t)ai_sPropertyValue.c_str());
		if (iResult != M4_SUCCESS)
		{
			m4string_t sError = m4string_t("Authenticate: Error creating property in session object for property ") + ai_sPropertyId;
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, sError.c_str());
			return;
		}
	}
}

