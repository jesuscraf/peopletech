//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cacert.dll
// File:                m4cacert.cpp   
// Project:             Mind3x      
// Author:              Meta Software M.S. , S.A
// Date:                28/06/2002
// Language:            C++
//
// Definition:
//    This module contains functions related to authentication certificates
//
//==============================================================================
#include <m4types.hpp>
#include <m4var.hpp>
#include <m4cacertdef.hpp>
#include <m4cacert.hpp>
#include <m4cryptc.hpp>
#include <_base64.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/pkcs12.h>
#include <m4log.hpp>
#include <m4srvres.hpp>
#include "m4unicode.hpp"

//M4Date
#include <m4date.hpp>

m4pcchar_t ErrorSSL ();
void Errores () ;
m4return_t ParseToken(m4string_t ai_sString, m4pcchar_t ai_pcToken, m4string_t &ao_sValue);

#define LINELEN 64
#define DEF_M4SSO_CERT_PREFIX "M4SSOCertVer=1.0"
#define DEF_M4SSO_CERT_PREFIX_2 "M4SSOCertVer=2.0"
#define DEF_M4SSO_CERT_PREFIX_3 "M4SSOCertVer=3.0"

// certificate prefixes
#define CERT_TOKEN_PREFIX ";Cert="
#define CERT_TOKEN_SIGN_PREFIX ";Sign="
#define CERT_TOKEN_SERIAL_PREFIX ";Serial="
#define CERT_TOKEN_KEY_PREFIX ";Key="

// internal token values
#define CERT_USER_NAME_PREFIX "USER="
#define CERT_TIME_START_PREFIX "TIMESTART="
#define CERT_TIME_END_PREFIX "TIMEEND="
#define CERT_DOMAIN_PREFIX "DOMAIN="
#define CERT_WEB_SECRET_KEY "WEBSECKEY="
#define CERT_JSAPI_SESSION_ID "JSAPISESSID="

// Certificate logic constants
#define CERT_SHORT_TERM_DURATION 1000000
#define CERT_LONG_TERM_DURATION 0x7FFFFFFF

#define CERT_WEBSSO_INDIRECT_DOMAIN "WEBSSO_INDIRECT_DOMAIN"

//-------------------------------------------------------------------------------------------
// [+] Global variables
//-------------------------------------------------------------------------------------------


FILE * f ;

// global certificate 
ClCertificateKey * ClCertificateKey::m_poInstance = NULL ;
EVP_PKEY * ClCertificateKey::m_poCertificateKey = NULL ;

unsigned char * cert ;
long ret1;


		
/* GetSerialFromCredential					This function gets the certificate identifier 
 *											from credential containing "Serial" (;Serial=z64)
 *
 * @param m4string_t	  ai_sCertificate	The credential
 * @param m4bool_t		 &ao_bHasSerial		If the credential contains the identifier
 * @return m4uint32_t						The identifier decoded if any, 0 if none
 */


m4uint32_t M4_DECL_M4CACERT GetSerialFromCredential ( m4string_t ai_sCertificate, m4bool_t &ao_bHasSerial )
{

	m4uint32_t iSerial = 0;
	ao_bHasSerial = M4_FALSE;
	m4pcchar_t pcCertValue = ai_sCertificate.c_str();

	C_Base64 oDecoder;
	m4int32_t iSerialLen = 0;

	m4pchar_t pcOriginalSerial = NULL; 
	m4string_t sEncodedSerial; 
	
	m4pchar_t pcSerial = NULL; 

	// finds out the identifier of the certificate from Serial=z64
	if (strncmp(CERT_TOKEN_SERIAL_PREFIX,pcCertValue,strlen(CERT_TOKEN_SERIAL_PREFIX)) != 0) 
	{
		
		// gets the serial rest of the string
		m4pcchar_t pcOriginalSerial = strstr(pcCertValue,CERT_TOKEN_SERIAL_PREFIX);

		// if no serial this is one of the older tokens, so get away
		if (pcOriginalSerial == NULL) 
		{
			ao_bHasSerial = M4_FALSE;
			return iSerial;
		}
		
		// else unwrap the serial				
	    if (ParseToken(pcOriginalSerial,CERT_TOKEN_SERIAL_PREFIX,sEncodedSerial) != M4_SUCCESS) 
		{
				return iSerial;
		}

    	// decodes the serial
		if (oDecoder.Decode((m4pchar_t)sEncodedSerial.c_str(),pcSerial,&iSerialLen) == 0) 
		{	
			SETCODEF(M4_ERR_CACERT_ERROR_DECODING_SERIAL, ERRORLOG,"#*s1*#%s#","GetSerialFromCredential");
			return iSerial;
		}

		// converts the serial to a long value.
		iSerial = atol(pcSerial); 
		ao_bHasSerial = M4_TRUE;
		
	} 
	return iSerial; 
}

// Start M4RSACleaner ------------------------ cleaner class

// this class helps to clean RSA object
class M4RSACleaner {

private:
	RSA *m_poRSA;

public:
	M4RSACleaner(RSA *poRSA);
	~M4RSACleaner();

	m4void_t Clear();

};

// the constructor
M4RSACleaner::M4RSACleaner(RSA *poRSA) : m_poRSA(poRSA) {}

// frees the RSA structure
M4RSACleaner::~M4RSACleaner() {
	Clear();
}

// clears the internal variable
m4void_t M4RSACleaner::Clear() {
	if (m_poRSA != NULL) {
		RSA_free (m_poRSA);
	}

	m_poRSA = NULL;
}


// End M4RSACleaner ------------------------ cleaner class

/* CheckM4WSSOCertificate		This function examines a password to see whether it is a pass
 *								or a certificate token. Domain value (if set) returns as well
 * @param ClICaAuthenticator	*ai_poAuthenticator		The authenticator object
 * @param m4string_t			 ai_sCertificate		The original password
 * @param m4string_t			 ai_sUserId				The user id	
 * @param m4bool_t				&ao_bIsValid			If it is valid (output arg)
 * @param m4string_t			&ao_sDomainName			The domain if any (output arg)
 * @return m4uint32_t									Error or success
 */

m4return_t M4_DECL_M4CACERT CheckM4WSSOCertificate( ClICaAuthenticator * ai_poAuthenticator, m4string_t ai_sCertificate, m4string_t ai_sUserId, m4bool_t &ao_bIsValid, m4string_t &ao_sDomainName)
{
	M4CaCert oCaCert ( ai_poAuthenticator ) ;

	return oCaCert.CheckM4WSSOCertificate ( ai_sCertificate, ai_sUserId, ao_bIsValid, ao_sDomainName) ;
}

m4return_t M4_DECL_M4CACERT ExportCertificate ( ClICaAuthenticator * ai_poAuthenticator , m4string_t ai_strUserName , m4string_t ai_strCertPath, m4string_t ai_strCertPass )
{
	M4CaCert oCaCert ( ai_poAuthenticator ) ;

	return oCaCert.ExportCertificate (ai_strUserName , ai_strCertPath, ai_strCertPass ) ;
}

m4return_t M4_DECL_M4CACERT GetM4Certificate ( ClICaAuthenticator * ai_poAuthenticator , m4string_t ai_strUserName , m4string_t & ao_szCertificate )
{
	M4CaCert oCaCert ( ai_poAuthenticator ) ;

	return oCaCert.GetM4Certificate (ai_strUserName , ao_szCertificate, NULL, M4_TRUE, M4_TRUE ) ;
}

m4return_t  M4_DECL_M4CACERT CheckM4Certificate ( ClICaAuthenticator * ai_poAuthenticator , m4string_t & ao_strUserName , m4pchar_t  ai_szCertificate )
{
	M4CaCert oCaCert ( ai_poAuthenticator ) ;

	return oCaCert.VerifyCertificate ( ai_szCertificate , ao_strUserName ) ;
}

m4return_t M4_DECL_M4CACERT GeneratePublicPrivateKey ( m4pchar_t & ao_strPrivateKey , m4uint32_t & ao_iPrivLen  , m4pchar_t & ao_strPublicKey , m4uint32_t & ao_iPubLen)
{

	m4uint32_t Len  = 0 ;
    RSA *clave ; 
    EVP_PKEY * pkey ;
	unsigned char * p ;

	// eh 285628
	clave = RSA_generate_key(2048, 3, NULL, NULL);   
	pkey = EVP_PKEY_new() ;
	EVP_PKEY_assign_RSA( pkey , clave ) ;

	ao_iPrivLen = i2d_PrivateKey(pkey, NULL );

	ao_strPrivateKey = (char*) new unsigned char [ao_iPrivLen ] ;
	
	p = (unsigned char*) ao_strPrivateKey ;

	do
	{
		Len += i2d_PrivateKey(pkey, &p);

	}while ( Len < ao_iPrivLen ) ;


	if( ! ( ao_iPubLen = i2d_PublicKey(pkey, NULL ) ) )
	{
		SETCODEF ( M4_ERR_CACERT_SERIAL_KEY , ERRORLOG , "Error serializing key" ) ;
		return M4_ERROR ;
	}

	ao_strPublicKey = (char*) new unsigned char  [ ao_iPubLen ] ;

	p= (unsigned char*) ao_strPublicKey ;

	Len = 0 ;

	do
	{
		if (! ( Len += i2d_PublicKey(pkey, &p) ) )
		{
			SETCODEF ( M4_ERR_CACERT_SERIAL_KEY , ERRORLOG , "Error serializing key" ) ;
			return M4_ERROR ;
		}

	}while ( Len < ao_iPubLen ) ;

	return M4_SUCCESS ;
}


//-------------------------------------------------------------------------------------------
// [+] class: ClCertificateKey
//-------------------------------------------------------------------------------------------


ClCertificateKey::ClCertificateKey()
{};

ClCertificateKey::ClCertificateKey ( ClICaAuthenticator * ai_poAuthenticator )
{

	if ( ai_poAuthenticator )
		m_poCertificateKey = _RecoverCertificatePrivateKey ( ai_poAuthenticator ) ;

}

// Cache for classical certificates: Not managed
m4return_t ClCertificateKey::SetCertificateKey ( ClICaAuthenticator * ai_poAuthenticator ) 
{
	return M4_SUCCESS;
}

EVP_PKEY * ClCertificateKey::GetCertificateKey ( ClICaAuthenticator * ai_poAuthenticator )
{
	if ( ! m_poCertificateKey )
	{
		if ( ! m_poInstance )
		{
			m_poInstance = new ClCertificateKey ( ai_poAuthenticator );
		}
		else
		{
			SETCODEF ( M4_ERR_CACERT_KEY_NOT_INI , ERRORLOG , "Error getting Meta4 private key for certificate signature" ) ;
		}

	}

	return m_poCertificateKey ;

}

EVP_PKEY * ClCertificateKey::_RecoverCertificatePrivateKey ( ClICaAuthenticator * ai_poAuthenticator ) 
{
	EVP_PKEY * pkey ;
	m4pchar_t strPrivateKey = NULL, strPublicKey = NULL ;
	m4uint32_t iPrivLen = 0  , iPubLen = 0;

	
	if ( ! ai_poAuthenticator )
	{
		SETCODEF ( M4_ERR_CACERT_NOT_AUTHENTICATOR , ERRORLOG , "No Authenticator provided" ) ;
		return NULL ;
	}

	if ( M4_ERROR == ai_poAuthenticator -> GetKeysFromRepository ( strPublicKey , iPubLen , strPrivateKey , iPrivLen   ) )
	{
		SETCODEF ( M4_ERR_CACERT_NOT_KEYS_IN_REPOSITORY , ERRORLOG , "Error getting keys from repository" ) ;
		return NULL ;
	}

	pkey = d2i_PrivateKey(EVP_PKEY_RSA,NULL,(const unsigned char**) &strPrivateKey , iPrivLen   ) ;

	if( ! pkey )
	{
		SETCODEF ( M4_ERR_CACERT_KEY_DESERIALIZE_ERROR , ERRORLOG, "Error deserializing repository private key" ) ;
	}

	return pkey ;
}

//-------------------------------------------------------------------------------------------
// [+] class: ClCertificateDomainKey
//-------------------------------------------------------------------------------------------



ClCertificateDomainKey::ClCertificateDomainKey()
{};


/* SetCertificateKey			This function fills in the multidomain keys cache map. 
 *								
 * @param ClICaAuthenticator	*ai_poAuthenticator		The authenticator object
 * @return m4uint32_t									Error or success
 */

m4return_t ClCertificateDomainKey::SetCertificateKey(ClICaAuthenticator * ai_poAuthenticator)
{
	AuthenticatorType_t		eType;
	EVP_PKEY *				poNewDomainKey = NULL;
	EVP_PKEY *				poLocalDomainKey = NULL;
	m4uint32_t				iIdCertificate = 0; 
	m4return_t				iRet = M4_SUCCESS;

	if (ai_poAuthenticator == NULL)
	{
		SETCODEF ( M4_ERR_CACERT_NOT_AUTHENTICATOR , ERRORLOG , "No Authenticator provided" ) ;
		return M4_ERROR;
	}

	// Obtener tipo de autenticator.
	eType = ai_poAuthenticator->GetType(); 	
	if (eType == m4sso_classical)	
	{
		// Existe un desajuste entre el modelo clásico y el multidominio
		SETCODEF ( M4_ERR_CACERT_KEY_SOURCE_MISMATCH , ERRORLOG , "Mismatch between classical and multidomain" ) ;
		return M4_ERROR;
	} 

	// Ir a por el identificador de certificado almacenado en el objeto autenticator.
	iRet = ai_poAuthenticator->GetCertificateId(iIdCertificate);
	if (iRet != M4_SUCCESS)
	{
		// No se ha podido obtener el identificador.
		SETCODEF ( M4_ERR_CACERT_CANNOT_GET_CERTIFICATE_ID , ERRORLOG , "Cannot obtain certificate id" ) ;
		return M4_ERROR;
	}

	// Obtener claves.
	poNewDomainKey = _RetrievePrivateDomainKey(ai_poAuthenticator);
	if (poNewDomainKey == NULL) 
	{
		// No se han podido obtener las claves del repositorio
		SETCODEF ( M4_ERR_CACERT_NOT_KEYS_IN_REPOSITORY , ERRORLOG , "Error getting keys from repository" ) ;
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

/* SetCertificateKey			This function recovers the certificate key for a certain id 
 *								
 * @param ClICaAuthenticator	*ai_poAuthenticator		The authenticator object
 * @return EVP_PKEY *									The private key object
 */

EVP_PKEY * ClCertificateDomainKey::GetCertificateKey(ClICaAuthenticator * ai_poAuthenticator)
{
	AuthenticatorType_t		eType;
	m4uint32_t				iCertificateId = 0; 
	m4return_t				iRet = M4_SUCCESS;
	EVP_PKEY *				poDomainKey = NULL;
	
	if (ai_poAuthenticator == NULL)
	{
		SETCODEF ( M4_ERR_CACERT_NOT_AUTHENTICATOR , ERRORLOG , "No Authenticator provided" ) ;
		return NULL;
	}

	// Obtener tipo de autenticator.
	eType = ai_poAuthenticator->GetType(); 	
	if (eType == m4sso_classical)	
	{
		// Existe un desajuste entre el modelo clásico y el multidominio
		SETCODEF ( M4_ERR_CACERT_KEY_SOURCE_MISMATCH , ERRORLOG , "Mismatch between classical and multidomain" ); 
		return NULL;
	} 

	// Obtener id. certificado.
	iRet = ai_poAuthenticator->GetCertificateId(iCertificateId);
	if (iRet != M4_SUCCESS)
	{
		// No se ha podido obtener el identificador.
		SETCODEF ( M4_ERR_CACERT_CANNOT_GET_CERTIFICATE_ID , ERRORLOG , "Cannot obtain certificate id" ) ;
		return NULL;
	}

	poDomainKey = _RetrievePrivateDomainKey(ai_poAuthenticator);
	if (poDomainKey == NULL)
	{
			SETCODEF ( M4_ERR_CACERT_NOT_KEYS_IN_REPOSITORY , ERRORLOG , "Error getting keys from repository" ) ;
			return NULL;
	}

	return poDomainKey;
}


EVP_PKEY * ClCertificateDomainKey::_RetrievePrivateDomainKey ( ClICaAuthenticator * ai_poAuthenticator ) 
{
	
	EVP_PKEY * pkey ;
	m4pchar_t strPrivateKey = NULL, strPublicKey = NULL ;
	m4uint32_t iPrivLen = 0  , iPubLen = 0;

	
	// Supposedly we are correctly positioned, if not, we will have to do something about it	
	if ( M4_ERROR == ai_poAuthenticator -> GetKeysFromRepository ( strPublicKey , iPubLen , strPrivateKey , iPrivLen   ) )
	{
		SETCODEF ( M4_ERR_CACERT_NOT_KEYS_IN_REPOSITORY , ERRORLOG , "Error getting keys from repository" ) ;
		return NULL ;
	}

	pkey = d2i_PrivateKey(EVP_PKEY_RSA,NULL,(const unsigned char**) &strPrivateKey , iPrivLen   ) ;

	if( ! pkey )
	{
		SETCODEF ( M4_ERR_CACERT_KEY_DESERIALIZE_ERROR , ERRORLOG, "Error deserializing repository private key" ) ;
	}

	return pkey ;

}

//-------------------------------------------------------------------------------------------
// [+] class: M4CaCert
//-------------------------------------------------------------------------------------------

// Bg 143438
m4bool_t M4CaCert::m_bOpenSSLInitialized = M4_FALSE;
ClMutex M4CaCert::m_oMtxInitialization(M4_TRUE);


M4CaCert::M4CaCert ( ClICaAuthenticator * ai_poAuthenticator )
{
	m_poAuthenticator = ai_poAuthenticator  ;
}

M4CaCert::~M4CaCert ()
{
}

// releases instance of encoder string
class M4EncodedContainer {

public:
	C_Base64 *encoder;
	m4pchar_t value;

	M4EncodedContainer(C_Base64 *encoder) {
		this->encoder = encoder;
		value = NULL;
	}

	~M4EncodedContainer() {
		if (value != NULL) {
			if (encoder != NULL) {
				encoder->FreeBuffer(value);
			}
		}
		value = NULL;
		encoder = NULL;
	}
};
	

/**
 * CheckM4WSSOCertificate: from a Meta4 SSO credential, this method says if valid and for what domain
 * @param m4string_t 	 ai_sCertificate	Credential
 * @param m4string_t 	 ai_sUserId			User (in SSO it is a fixed value)
 * @param m4bool_t   	&ao_bIsValid		If it is a valid credential
 * @param m4string_t 	&ao_sDomainName		What domain it is valid for (if none returns "")
 * @return				status of operation
 */

m4return_t M4CaCert::CheckM4WSSOCertificate( m4string_t ai_sCertificate, m4string_t ai_sUserId, m4bool_t &ao_bIsValid, m4string_t &ao_sDomainName  ) 
{
	_InitializeOpenSSL();

	// sets the default output values
	ao_bIsValid = M4_FALSE;	
	ao_sDomainName = "";
	m4uint32_t uiVersion = -1;

	// checks the initial value
	m4pcchar_t pcCertValue = ai_sCertificate.c_str();
	if (strncmp(DEF_M4SSO_CERT_PREFIX,pcCertValue,strlen(DEF_M4SSO_CERT_PREFIX)) != 0) {
		
		if (strncmp(DEF_M4SSO_CERT_PREFIX_2,pcCertValue,strlen(DEF_M4SSO_CERT_PREFIX_2)) != 0) {

			if (strncmp(DEF_M4SSO_CERT_PREFIX_3,pcCertValue,strlen(DEF_M4SSO_CERT_PREFIX_3)) != 0) {
				// not a certificate, lets things go on...
				return M4_SUCCESS;
			}
			else {
				uiVersion = 3;
			}
		}
		else {
			uiVersion = 2;
		}
	}
	else {
		uiVersion = 1;
	}

	// gets the signature
	m4pcchar_t pcEncodedSignature = strstr(pcCertValue,CERT_TOKEN_SIGN_PREFIX);
	if (pcEncodedSignature == NULL) {
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}
	pcEncodedSignature += strlen(CERT_TOKEN_SIGN_PREFIX);

	// decodes the signature
	C_Base64 decoder;
	m4int32_t iSignatureLen = 0;
	M4EncodedContainer oSignature(&decoder);

	if (decoder.Decode((m4pchar_t)pcEncodedSignature, oSignature.value, &iSignatureLen) == 0) {
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}

	// gets the signed value
	m4int32_t iToSignLen = pcEncodedSignature - strlen(CERT_TOKEN_SIGN_PREFIX) - pcCertValue;
	m4string_t sEncToSign(pcCertValue,iToSignLen);

	// hashes the input, previous to the signature
	EVP_MD_CTX     mdCtx;

	// generate the SHA1 hash to validate
	EVP_DigestInit(&mdCtx,EVP_sha1());
	EVP_DigestUpdate(&mdCtx, sEncToSign.c_str(),iToSignLen);
	m4uchar_t sha1Digest[SHA_DIGEST_LENGTH];
	m4uint_t iDigLen = 0;

	if (EVP_DigestFinal(&mdCtx, sha1Digest, &iDigLen) != 1) {
		SETCODEF(M4_ERR_CACERT_CHECK_CERT_FAIL, ERRORLOG,"#*s1*#%s#",ErrorSSL());
		return M4_ERROR;
	}

	// recovers the private key
	EVP_PKEY *poPrivateKey = NULL;

	// we get the key from the correct place
	AuthenticatorType_t eType; 
	eType = m_poAuthenticator->GetType(); 	
	if (eType == m4sso_classical)	
	{
		if (( poPrivateKey = ClCertificateKey::GetCertificateKey ( m_poAuthenticator ) ) == NULL)
		{
			SETCODEF(M4_ERR_CACERT_CHECK_CERT_FAIL, ERRORLOG,"#*s1*#%s#",ErrorSSL());
			return M4_ERROR ;
		}
	} 
	else
	{
		if (( poPrivateKey = ClCertificateDomainKey::GetCertificateKey ( m_poAuthenticator ) ) == NULL )
		{	
			SETCODEF(M4_ERR_CACERT_CHECK_CERT_FAIL, ERRORLOG,"#*s1*#%s#",ErrorSSL());
			return M4_ERROR ;
		}
	}

	// So we got the key!
	RSA *poRSA  = EVP_PKEY_get1_RSA(poPrivateKey); 
	M4RSACleaner oRSACleaner(poRSA);
	
	// verifies the signature
	if (RSA_verify(NID_sha1, sha1Digest, SHA_DIGEST_LENGTH, (m4puchar_t)oSignature.value,
		iSignatureLen, poRSA) != 1) {

		SETCODEF(M4_ERR_CACERT_CHECK_CERT_FAIL, ERRORLOG,"#*s1*#%s#",ErrorSSL());
		return M4_ERROR;
	}

	// gets the certificate token
	m4string_t sCertToken;
	if (ParseToken(pcCertValue,CERT_TOKEN_PREFIX,sCertToken) != M4_SUCCESS) 
	{
		SETCODEF(M4_ERR_CACERT_CHECK_NO_CERT_TOKEN, ERRORLOG,"#*s1*#");
		return M4_ERROR;
	}

	// gets the key token
	m4string_t sKeyToken;
	if (uiVersion >= 2 && ParseToken(pcCertValue,CERT_TOKEN_KEY_PREFIX,sKeyToken) != M4_SUCCESS) 
	{
		SETCODEF(M4_ERR_CACERT_CHECK_NO_KEY_TOKEN, ERRORLOG,"#*s1*#");
		return M4_ERROR;
	}

	// decodes the key
	M4EncodedContainer oEncKey(&decoder);
	m4int32_t iEncKeyLen = 0;
	if (uiVersion >= 2 && decoder.Decode((m4pchar_t)sKeyToken.c_str(),oEncKey.value,&iEncKeyLen) == 0) 
	{
	
		SETCODEF(M4_ERR_CACERT_INVALID_KEY, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}

	// ok, if we have an encrypted key we need to decrypt it
	m4int_t iKeyLen = 0;
	m4string_t sKey;

	if (oEncKey.value != NULL)
	{
		// we use RSA do decrypt the key
		m4pchar_t pcBuffer = new m4char_t[RSA_size(poRSA) + 1];

		if ((iKeyLen = RSA_private_decrypt(iEncKeyLen, (m4uchar_t *)oEncKey.value, (m4uchar_t *)pcBuffer,
			poRSA, RSA_PKCS1_PADDING)) == -1)
		{
			SETCODEF(M4_ERR_CACERT_INVALID_KEY, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
			return M4_ERROR;
		}

		// ok, now we have a valid key !!!
		sKey.assign(pcBuffer, iKeyLen);
		delete[] pcBuffer;
	}

	// decodes the certificate
	m4int32_t iCertificateLen = 0;
	M4EncodedContainer oCertificate(&decoder);

	if (decoder.Decode((m4pchar_t)sCertToken.c_str(),oCertificate.value,&iCertificateLen) == 0) 
	{
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}

	// deciphers the certificate token (hereafter token)
	m4string_t sOriginalToken;
	m4int_t iOriginalTokenLen = 0;

	// ok, now we could decrypt in two different ways

	// no key provided, so we'll use the original private certificate
	if (sKey.size() == 0) 
	{
		m4pchar_t pcBuffer = new m4char_t[RSA_size(poRSA) + 1];
		pcBuffer[0] = '\0';
		if ((iOriginalTokenLen = RSA_private_decrypt(iCertificateLen, (m4uchar_t *)oCertificate.value, (m4uchar_t *)pcBuffer,
			poRSA, RSA_PKCS1_PADDING)) == -1)
		{
			SETCODEF(M4_ERR_CACERT_INVALID_M4CERT, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
			return M4_ERROR;
		}
		sOriginalToken.assign(pcBuffer, iOriginalTokenLen);
		delete[] pcBuffer;
	}
	else 
	{
		// now we have a key generated at client side. So we'll use it
		m4pchar_t pcBuffer = NULL;
		if (DecryptTwoWayRaw(oCertificate.value, iCertificateLen, sKey.c_str(), iKeyLen, pcBuffer, iOriginalTokenLen) != M4_SUCCESS)
		{
			SETCODEF(M4_ERR_CACERT_INVALID_M4CERT, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
			return M4_ERROR;
		}
		sOriginalToken.assign(pcBuffer);
		delete[] pcBuffer;
	}

	// all done with decipher
	oRSACleaner.Clear();
	if (uiVersion >= 3)
	{
		// retrieves the value from UTF-8
		sOriginalToken = ConvertStringFromUTF8((m4pchar_t)sOriginalToken.c_str());
	}

	// gets the user token
	m4string_t sUserInToken;
	if (ParseToken(sOriginalToken,CERT_USER_NAME_PREFIX,sUserInToken) != M4_SUCCESS) {
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}

	// checks the user name (case insensitive)
	if (stricmp(sUserInToken.c_str(),ai_sUserId.c_str()) != 0) {
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT_USER, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}

	// reads the domain from the certificate, which must match the domain in the db
	m4string_t sDomainInToken;

	// if the domain has been sent
	if (ParseToken(sOriginalToken,CERT_DOMAIN_PREFIX,sDomainInToken) == M4_SUCCESS)
	{

		// we read what is written in the database
		m4string_t sDomainInDatabase;
		m_poAuthenticator->GetDomainId(sDomainInDatabase);

		// if the domain is not the indirect domain, the information in the database
		// must match the information in the certificate to avoid malicious usage
		if (stricmp(sDomainInDatabase.c_str(),CERT_WEBSSO_INDIRECT_DOMAIN) != 0) 
		{
			if (sDomainInDatabase != sDomainInToken)
			{
				// @TODO find out how to show both the original and the bad one.
				SETCODEF(M4_ERR_CACERT_DOMAIN_MISMATCH, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
				return M4_ERROR;
			}
		} 

		// if this is the indirect domain, then the true domain is what comes in the token.
		ao_sDomainName = sDomainInToken;	
	}

	// gets the time token
	m4string_t sTimeStartInToken;
	if (ParseToken(sOriginalToken,CERT_TIME_START_PREFIX,sTimeStartInToken) != M4_SUCCESS) {
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT_TIME, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}
	
	m4string_t sTimeEndInToken;
	if (ParseToken(sOriginalToken,CERT_TIME_END_PREFIX,sTimeEndInToken) != M4_SUCCESS) {
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT_TIME, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}

	// calculates the expiration time
	m4uint32_t tExpTimeStart = (m4uint32_t) atol(sTimeStartInToken.c_str());
	m4uint32_t tExpTimeEnd   = (m4uint32_t) atol(sTimeEndInToken.c_str());

	// compares with the current time
	m4uint32_t  tNow = (m4uint32_t) time(NULL);

	if (tNow < tExpTimeStart || tNow > tExpTimeEnd) {
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT_EXPIRED, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;
	}

	// bugid 0171365 - makes a date validity check
	m4double_t dNow; 
	M4ClTimeStamp DateNow;

	M4ClTimeStamp DateStart;
	m4double_t dStartDateInDatabase, dStartDateCorrected; 
	M4ClTimeStamp DateEnd;
	m4double_t dEndDateInDatabase, dEndDateCorrected; 
	m4uint32_t uiOneDayInMinutes=(m4uint32_t)24*60; 

	m_poAuthenticator->GetDates(dStartDateInDatabase, dEndDateInDatabase);
	
	DateStart.set(dStartDateInDatabase); 
	DateStart.sub_minutes(uiOneDayInMinutes);
    DateStart.get(dStartDateCorrected);

	DateEnd.set(dEndDateInDatabase); 
    DateEnd.add_minutes(uiOneDayInMinutes);
    DateEnd.get(dEndDateCorrected);
	
	DateNow.now();
	DateNow.get(dNow);

	if (dNow < dStartDateCorrected || dNow > dEndDateCorrected)
	{
		SETCODEF(M4_ERR_CACERT_INVALID_M4CERT_EXPIRED, ERRORLOG,"#*s1*#%s#","CheckM4WSSOCertificate");
		return M4_ERROR;	
	}

	// gets encryption security items
	// gets the time token
	if (this->m_poAuthenticator != NULL)
	{
		m4string_t sWebSecretKey;
		ParseToken(sOriginalToken, CERT_WEB_SECRET_KEY, sWebSecretKey);
		if (sWebSecretKey.size() != 0)
		{
			this->m_poAuthenticator->SetSessionProperty(M4_SESSION_WEB_SECRET_KEY_PROPERTY, sWebSecretKey, M4_TRUE);
		}

		m4string_t sJSAPISessionId;
		ParseToken(sOriginalToken, CERT_JSAPI_SESSION_ID, sJSAPISessionId);
		if (sJSAPISessionId.size() != 0)
		{
			this->m_poAuthenticator->SetSessionProperty(M4_SESSION_JSAPI_UUID_PROPERTY, sJSAPISessionId, M4_TRUE);
		}
	}

	// we got a valid certificate!!! Session granted!
	ao_bIsValid = M4_TRUE;
	return M4_SUCCESS;
}

	/*
	 * ConvertString	  Converts from UTF-8 to local encoding
	 * @param m4pchar_t	  ai_pcInput		The input string
	 * @return			  The output string
	 */
m4pchar_t M4CaCert::ConvertStringFromUTF8(m4pchar_t ai_pcInput) 
{
	int iLen = -1;
	return M4Utf8ToCpp(ai_pcInput, iLen);
}

	/*
	 * ExportCertificate	  Exporta a un fichero PKCS12 un certificado de usuario. 
	 * @param m4string_t	  ai_strUserName		Valor del usuario a nombre del que va el cert
	 * @param m4string_t	  ai_strCertPath		Ruta del fichero PKCS12
	 * @param m4string_t	  ai_strCertPass		Contraseña del fichero PKCS12.
	 * @return				  Error or success
	 */

m4return_t M4CaCert::ExportCertificate ( m4string_t ai_strUserName , m4string_t ai_strCertPath, m4string_t ai_strCertPass ) 
{
	_InitializeOpenSSL();

	// gets the certificate from the repository
	X509 * poX509 = NULL;
	m4string_t strCert;

	m4return_t iRes = M4_ERROR;

	iRes = GetM4Certificate(ai_strUserName, strCert, &poX509, M4_FALSE, M4_FALSE);

	if (iRes != M4_SUCCESS) {
		return iRes;
	}

	if (poX509 == NULL) {
		SETCODEF(M4_ERR_CACERT_CERT_EXPORT_FAIL, ERRORLOG,"#*s1*#%s#%s#",ai_strCertPath.c_str(),ErrorSSL());
		return M4_ERROR;
	}

	// export to file
	PKCS12 *poPKCS12 = NULL;

	// gets the private KEY
	EVP_PKEY *poPrivateKey = NULL;
	if (m_poAuthenticator->GetType() == m4sso_classical)
	{
		poPrivateKey = ClCertificateKey::GetCertificateKey(m_poAuthenticator);
	}
	else
	{
		poPrivateKey = ClCertificateDomainKey::GetCertificateKey(m_poAuthenticator);
	}

	if (poPrivateKey == NULL)
	{
		X509_free(poX509);
		return M4_ERROR ;
	}

	poPKCS12 = PKCS12_create((m4pchar_t)ai_strCertPass.c_str(), (m4pchar_t)ai_strUserName.c_str(), poPrivateKey, poX509, NULL, 0,0,0,0,0);
	if (poPKCS12 == NULL) {
		
		X509_free(poX509);

		Errores();
		return M4_ERROR;
	}

	BIO *bio = NULL;
	if ((bio = BIO_new_file(ai_strCertPath.c_str(), "wb")) == NULL) {
		
		PKCS12_free(poPKCS12);
		X509_free(poX509);
		SETCODEF(M4_ERR_CACERT_CERT_EXPORT_FAIL, ERRORLOG,"#*s1*#%s#%s#",ai_strCertPath.c_str(),ErrorSSL());
		return M4_ERROR;
	}

	if (i2d_PKCS12_bio(bio, poPKCS12) == 0) {

		BIO_free_all(bio);
		PKCS12_free(poPKCS12);
		X509_free(poX509);
		SETCODEF(M4_ERR_CACERT_CERT_EXPORT_FAIL, ERRORLOG,"#*s1*#%s#%s#",ai_strCertPath.c_str(),ErrorSSL());
		return M4_ERROR;
	}
	
	BIO_free_all(bio);
	PKCS12_free(poPKCS12);
	X509_free(poX509);

	return iRes;
}

	/*
	 * GetM4Certificate		  Crea un certificado X.509 que será serializado al fichero PKCS12.
	 * @param m4string_t	  ai_strUserName
	 * @param m4string_t	 &ao_szCertificate
	 X509* *ao_poCert
	 * @param m4bool_t		  ai_bExpire
	 * @param m4string_t	  ai_strDomainName
	 * @param long			  ai_lCertificateID
	 */

m4return_t M4CaCert::GetM4Certificate ( m4string_t ai_strUserName , m4string_t & ao_szCertificate, X509* *ao_poCert, m4bool_t ai_bCheckUser, m4bool_t ai_bExpire ) 
{
	m4return_t iRet ;
	X509 * poX509 ;

	// Bug 0171478.
	_InitializeOpenSSL();

	if ( ! m_poAuthenticator )
	{
		SETCODEF ( M4_ERR_CACERT_NOT_AUTHENTICATOR , ERRORLOG , "No Authenticator provided" ) ;
		return M4_ERROR ;
	}


	if ( ai_strUserName.empty () )
	{
		SETCODEF ( M4_ERR_CACERT_NO_USER_NAME , ERRORLOG , "No user name for certificate generation" ) ;
		return M4_ERROR ;
	}
	

	if ( ai_bCheckUser && ( M4_SUCCESS > (iRet = m_poAuthenticator ->AuthenticateUser ( ai_strUserName ) ) ) )
	{
		SETCODEF ( M4_ERR_UUSS_USERNAME , ERRORLOG , "#*s1#%s#" , ai_strUserName.c_str () ) ;
		return iRet ;
	}

	// create the certificate
	AuthenticatorType_t eType; 
	eType = m_poAuthenticator->GetType(); 	
	if (eType == m4sso_classical)
    {
        if ( M4_ERROR == ( iRet =  _CreateCertificate ( ai_strUserName , poX509, ai_bExpire ) ) )
        {
        	return M4_ERROR ;
        }
    } 
    else 
    {
        if ( M4_ERROR == ( iRet =  _CreateDomainCertificate ( ai_strUserName , poX509, M4_TRUE ) ) )		
        {
        	return M4_ERROR ;
        }
    }

	// put the certificate in the PKCS12 file
	if (ao_poCert == NULL) 
	{
		if( M4_ERROR == _SerializeCertificate ( poX509 , ao_szCertificate ) )
		{
			return M4_ERROR ;
		}
	}
	else {
		// export the certificate
		*ao_poCert = poX509;
	}

	return iRet ;

}

/**
 * _CreateCertificate: This function generates an Unqualified X509 
 * @param m4string_t	  ai_strUserName
 * @param X509			*&ao_szCertificate
 * @param m4bool_t		  ai_bExpire
 */
m4return_t M4CaCert::_CreateCertificate ( m4string_t ai_strUserName , X509 *& ao_szCertificate, m4bool_t ai_bExpire ) 
{
	X509 * poCert ;
	long serie = 1L;
	int ret ;
	X509_NAME * nombre=NULL;
	EVP_PKEY * pkey = NULL ;
	X509_NAME_ENTRY * ne, * a = NULL ;

	poCert = X509_new() ;

	//establecer la versión del x509
	X509_set_version( poCert , 1 );


	//número de serie
	ASN1_INTEGER_set(X509_get_serialNumber ( poCert) , serie);

	X509_gmtime_adj ( X509_get_notBefore(poCert) , 0 ) ;

	if (ai_bExpire) {
		// short term (magic number?)
		X509_gmtime_adj ( X509_get_notAfter(poCert) , CERT_SHORT_TERM_DURATION ) ;
	}
	else {
		// I hope I'll be retired
		ASN1_TIME_set(X509_get_notAfter(poCert), CERT_LONG_TERM_DURATION);
	}

	//firma del certificado

	if (! ( pkey = ClCertificateKey::GetCertificateKey ( m_poAuthenticator ) ) )
	{
		return M4_ERROR ;
	}

	
	X509_set_pubkey ( poCert , pkey ) ;

	Errores () ;

	if ( ! (nombre=X509_get_subject_name(poCert) ) )
	{
		return M4_ERROR ;
	}

	if (! ( ne = X509_NAME_ENTRY_new () ) )
	{
		return M4_ERROR ;
	}

	a = ne ;

	if (! ( ne = X509_NAME_ENTRY_create_by_NID( &a, NID_commonName, V_ASN1_APP_CHOOSE, (unsigned char*)ai_strUserName.c_str (), ai_strUserName.size () ) ) )
	{
		SETCODEF ( M4_ERR_CACERT_ADD_USER_NAME , ERRORLOG , "#*s1#%s#" , ai_strUserName.c_str () ) ;
		return M4_ERROR ;
	}

	if( X509_NAME_add_entry ( nombre, ne, 0 , 0 ) != 1)
	{
		SETCODEF ( M4_ERR_CACERT_ADD_USER_NAME , ERRORLOG , "#*s1#%s#" , ai_strUserName.c_str () ) ;
		return M4_ERROR ;
	}

    X509_set_issuer_name(poCert ,nombre);

    X509_sign(poCert , pkey, EVP_sha1 () );

	if ( ( ret = X509_verify ( poCert , pkey ) ) != 1)
	{
		SETCODEF ( M4_ERR_CACERT_VERIFY , ERRORLOG , "Error verifying just created certificate" ) ;
		return M4_ERROR ;
	}

   Errores () ;

	ao_szCertificate = poCert ;

	return M4_SUCCESS ;

}

/**
 * _CreateDomainCertificate: This function generates a X509 qualified with the domain
 * @param m4string_t	  ai_strUserName
 * @param X509			*&ao_szCertificate
 * @param m4bool_t		  ai_bExpire
 * @param m4string_t	  ai_strDomainName
 * @param long			  ai_lCertificateID
 */
m4return_t M4CaCert::_CreateDomainCertificate ( m4string_t ai_strUserName , X509 *& ao_szCertificate, m4bool_t ai_bExpire )
										  
{
	X509 *		poCert = NULL;
	long		serie = 1L;
	int			ret ;
	X509_NAME * nombre=NULL;
	EVP_PKEY *	pkey = NULL ;
	m4return_t	iRet = M4_SUCCESS;

	X509_NAME_ENTRY * ne_user, * a_user = NULL ;
	X509_NAME_ENTRY * ne_org, * a_org = NULL ;

	m4double_t		ldt_start = 0; 
	m4double_t		ldt_end = 0; 
	m4double_t		dLocalDateNow = 0;
	time_t			t_lifestart = 0;
	time_t			t_lifeend = 0;

	poCert = X509_new() ;

	// Another default values
	m4string_t  strDomainId = "";
	m4uint32_t lCertificateID = 0; 

	// establecer la versión del x509
	X509_set_version( poCert , 1 );

	// obtener los valores necesarios
	iRet = m_poAuthenticator->GetDates(ldt_start, ldt_end); 
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	iRet = m_poAuthenticator->GetCertificateId(lCertificateID); 
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	iRet = m_poAuthenticator->GetDomainId(strDomainId);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// número de serie. 
	ASN1_INTEGER_set (X509_get_serialNumber ( poCert ) , lCertificateID );

	// si expira el certificado.
	if (ai_bExpire == M4_TRUE) 
	{
		dLocalDateNow = ClActualDate((m4uint8_t)M4CL_TIME_LOCAL_TIME);
		if (ldt_start < dLocalDateNow)
		{
			ldt_start = dLocalDateNow;
		}
		
		if (ldt_end < ldt_start)
		{
			SETCODEF ( M4_ERR_CACERT_WRONG_CERT_DURATION , ERRORLOG , "The chosen date causes an invalid certificate to be generated" ) ;
			return M4_ERROR ;
		}
		
		t_lifestart = _ConvertJulianDateToTime_t(ldt_start);
		ASN1_TIME_set(X509_get_notBefore(poCert), t_lifestart);
		
		t_lifeend = _ConvertJulianDateToTime_t(ldt_end);
		ASN1_TIME_set(X509_get_notAfter(poCert), t_lifeend );
	}
	else
	{
		ASN1_TIME_set(X509_get_notAfter(poCert), CERT_LONG_TERM_DURATION );
	}

	if (! ( pkey = ClCertificateDomainKey::GetCertificateKey ( m_poAuthenticator ) ) )
	{	
		return M4_ERROR ;
	}
	
	X509_set_pubkey ( poCert , pkey ) ;

	Errores () ;

	if ( ! (nombre=X509_get_subject_name(poCert) ) )
	{
		return M4_ERROR ;
	}


	/* Establecer en el SubjectDN la linea CN=usuario */
	if (! ( ne_user = X509_NAME_ENTRY_new () ) )
	{
		return M4_ERROR ;
	}
	a_user = ne_user ;

	if (! ( ne_user = X509_NAME_ENTRY_create_by_NID( &a_user, NID_commonName, V_ASN1_APP_CHOOSE, (unsigned char*)ai_strUserName.c_str (), ai_strUserName.size () ) ) )
	{
		SETCODEF ( M4_ERR_CACERT_ADD_USER_NAME , ERRORLOG , "#*s1#%s#" , ai_strUserName.c_str () ) ;
		return M4_ERROR ;
	}

	if( X509_NAME_add_entry ( nombre, ne_user, 0 , 0 ) != 1)
	{
		SETCODEF ( M4_ERR_CACERT_ADD_USER_NAME , ERRORLOG , "#*s1#%s#" , ai_strUserName.c_str () ) ;
		return M4_ERROR ;
	}

	/* Establecer el issuer name que va a ser también CN=usuario */
    X509_set_issuer_name(poCert ,nombre);

	/* Añadir para el SubjectDN O=dominio */
	if ( strDomainId.empty() )
	{		
		SETCODEF ( M4_ERR_CACERT_EMPTY_DOMAIN , ERRORLOG , "The domain is empty" ) ;
		return M4_ERROR; 
	}

	if (! ( ne_org = X509_NAME_ENTRY_create_by_NID( &a_org, NID_organizationName, V_ASN1_APP_CHOOSE, (unsigned char*)strDomainId.c_str (), strDomainId.size () ) ) )
	{
		SETCODEF ( M4_ERR_CACERT_ADD_DOMAIN_NAME , ERRORLOG , "#*s1#%s#" , strDomainId.c_str () ) ;
		return M4_ERROR ;
	}

	if( X509_NAME_add_entry ( nombre, ne_org, 0 , 0 ) != 1)
	{
		SETCODEF ( M4_ERR_CACERT_ADD_DOMAIN_NAME , ERRORLOG , "#*s1#%s#" , strDomainId.c_str () ) ;
		return M4_ERROR ;
	}

	// sign the certificate
    X509_sign(poCert , pkey, EVP_sha1 () );

	if ( ( ret = X509_verify ( poCert , pkey ) ) != 1)
	{
		SETCODEF ( M4_ERR_CACERT_VERIFY , ERRORLOG , "Error verifying just created certificate" ) ;
		return M4_ERROR ;
	}

    Errores () ;

	ao_szCertificate = poCert ;

	return M4_SUCCESS ;

}

m4return_t M4CaCert::_SerializeCertificate ( X509 * ai_poX509 , m4string_t & ao_szCertificate ) 
{
	int ret ;
	unsigned char  * szX509 , * p ;
	C_Base64 oEncoder ;
	m4pchar_t base64buf = NULL;
	m4uint32_t iBufLen , numBlocks ;



	if ( ! ( ret = i2d_X509( ai_poX509 , (unsigned char**) NULL ) ) )
	{
		SETCODEF ( M4_ERR_CACERT_SERIALIZE_CERT , ERRORLOG , "Error serializing ccertificate" ) ;
		return M4_ERROR ;
	}

	
	szX509 = (unsigned char*) malloc( ret * 2);

	p = szX509 ;
	
	if ( ! ( ret = i2d_X509( ai_poX509 ,  &p ) ) )
	{
		SETCODEF ( M4_ERR_CACERT_SERIALIZE_CERT , ERRORLOG , "Error serializing ccertificate" ) ;
		return M4_ERROR ;
	}


	Errores () ;


	if ((ret %3)==0)
		numBlocks=(ret/3);
	else
		numBlocks=(ret/3)+1;

	if (! ( iBufLen = (numBlocks*4+(numBlocks*4/LINELEN)) + 1 ) )
	{
		SETCODEF ( M4_ERR_CACERT_SERIALIZE_CERT , ERRORLOG , "Error serializing ccertificate" ) ;
		return M4_ERROR ;
	}


	if ( M4_ERROR == oEncoder.Code ( (char*) szX509, ret , base64buf) )
	{
		SETCODEF ( M4_ERR_CACERT_ENCODING_CERT , ERRORLOG , "Error encoding certificate to base64 format" ) ;
		return M4_ERROR ;
	}

	ao_szCertificate = base64buf ;

	free (szX509) ;

	// fix bug 0089603
	oEncoder.FreeBuffer (base64buf) ;


	X509_free ( ai_poX509 ) ;

	return M4_SUCCESS ;
}

/*
 *	This is called from ClExeForUUSS::Authenticate()
 */
m4return_t M4CaCert::VerifyCertificate ( m4string_t ao_szCertificate , m4string_t & ao_strUserName ) 
{

	C_Base64 oEncoder ;
	const unsigned char * p,  * old_p ;
	m4int32_t length ;
	X509 * poCert = NULL , * a = NULL ;
	EVP_PKEY * pkey ;
	m4return_t iRet = M4_ERROR ;
	X509_NAME *	name ;
	X509_NAME_ENTRY * ne ;

	// Bug 0171478.
	_InitializeOpenSSL();

	if ( M4_ERROR == oEncoder.Decode ( (char*) ao_szCertificate.c_str() , (char*&) p , &length ) )
	{
		SETCODEF ( M4_ERR_CACERT_DECODING_CERT , ERRORLOG , "Error decoding certificate from base64 format" ) ;
		return M4_ERROR ;
	}

	old_p = p;
 
	a = X509_new() ;

	if (! ( poCert = d2i_X509(NULL, &p ,length) ) )
	{
		SETCODEF ( M4_ERR_CACERT_DESERIALIZE_CERT , ERRORLOG , "Error deserializing certificate" ) ;
		return M4_ERROR ;
	}

	Errores () ;

	if (! ( pkey = ClCertificateKey::GetCertificateKey ( (ClICaAuthenticator *) m_poAuthenticator ) ) )
	{
		return M4_ERROR ;
	}

	if ( poCert && pkey )
	{
		if ( X509_verify ( poCert , pkey ) != 1 )
		{
			SETCODEF ( M4_ERR_CACERT_VERIFY , ERRORLOG , "Error verifying just created certificate" ) ;
			return M4_ERROR ;
		}
	}

	// fix bug 0089603
	oEncoder.FreeBuffer ( (char*) old_p) ;

	name = X509_get_issuer_name(poCert);

	ne = X509_NAME_get_entry(name, 0 );

	ao_strUserName = (char*) ne->value->data ;
	X509_free ( poCert ) ;

	if ( M4_SUCCESS > (iRet = m_poAuthenticator ->AuthenticateUser ( ao_strUserName ) ) )
	{
		SETCODEF ( M4_ERR_UUSS_USERNAME , ERRORLOG , "#*s1#%s#" , ao_strUserName.c_str () ) ;
		return iRet ;
	}

	return iRet ;

}

// Bg 143438
void M4CaCert::_InitializeOpenSSL()
{

	if(m_bOpenSSLInitialized == M4_TRUE)
	{
		return;
	}

	m_oMtxInitialization.Lock();

	if(m_bOpenSSLInitialized == M4_TRUE)
	{
		m_oMtxInitialization.Unlock();
		return;
	}

	// initializa crypto modules
	SSLeay_add_all_algorithms();
	ERR_load_crypto_strings();

	// initializa otras cosas
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms () ;

	m_bOpenSSLInitialized = M4_TRUE;

	m_oMtxInitialization.Unlock();
}

/**
 * _ConvertJulianDateToTime_t: This function converts a julian date to time_t 
 (integral value representing the number of seconds elapsed since 00:00 hours, 
 Jan 1, 1970 UTC).
 * @param m4double_t	  ai_dJulianDate
 * @return time_t
 */
time_t M4CaCert::_ConvertJulianDateToTime_t(m4double_t ai_dJulianDate)
{
	M4ClTimeStamp   oDateStart;
	m4string_t		sDateStart; 
	int				iYears, iMonths, iDays;
	m4int32_t		iHours, iMinutes, iSeconds;
	struct tm		stDate;
	time_t			t_lifeDate;
	
	// debug
	// M4ClTimeStamp   oDate;
	// M4ClString		sDate;
	// oDate.set(ai_dJulianDate);
	// oDate.get(sDate);
	// debug

	// Obtener año, mes, día, hora, minutos y segundos.
	ClDateToNumbers(ai_dJulianDate, iYears, iMonths, iDays, iHours, iMinutes, iSeconds);

	// Rellenar una estructura tm.
	stDate.tm_year = iYears - 1900;
	stDate.tm_mon = iMonths - 1;
	stDate.tm_mday = iDays;
	stDate.tm_hour = iHours;
	stDate.tm_min = iMinutes;
	stDate.tm_sec = iSeconds;
	
	t_lifeDate = mktime(&stDate);

	// debug
	// m4date_t dateGmLastWrite = ClTime_tToDate(t_lifeDate, M4CL_TIME_GM_TIME);
	// oDate.set(dateGmLastWrite);
	// oDate.get(sDate);
	// debug

	return t_lifeDate;
}

m4return_t ParseToken(m4string_t ai_sString, m4pcchar_t ai_pcToken, m4string_t &ao_sValue) {
	// finds the position
	m4pcchar_t pcPos = strstr(ai_sString.c_str(),ai_pcToken);
	if (pcPos == NULL) {
		return M4_ERROR;
	}
	pcPos += strlen(ai_pcToken);

	m4pcchar_t pcEndPos = strstr(pcPos,";");
	if (pcEndPos == NULL) {
		pcEndPos = pcPos + strlen(pcPos);
	}

	m4int_t iLen = pcEndPos - pcPos;
	m4pchar_t pcBuffer = new m4char_t[iLen+1];
	strncpy(pcBuffer,pcPos,iLen);
	pcBuffer[iLen] = '\0';

	ao_sValue = pcBuffer;
	delete [] pcBuffer;

	return M4_SUCCESS;
}

m4pcchar_t ErrorSSL ()
{
	m4string_t strError;
	m4uint32_t iCode;
	char vcBuffer [1000]; // enough following ssl doc's;

	m4bool_t bInit = M4_TRUE;

	while ( iCode = ERR_get_error () )
	{
		if (bInit) {
			bInit = M4_FALSE;
		}
		else {
			strError.append(". ");
		}

		ERR_error_string(iCode , vcBuffer);
		strError.append(vcBuffer);
	}

	return strError.c_str();
}

void Errores ()
{
	int ret ;

	while ( ret = ERR_get_error () )
	{
#ifdef _DEBUG
	char buf [1000] ;
	ERR_error_string(ret , buf);
	cout << buf << endl ;
#endif
	}
}


