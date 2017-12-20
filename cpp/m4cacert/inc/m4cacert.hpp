//## begin module%3959D0D102B8.cm preserve=no
//## end module%3959D0D102B8.cm

//## begin module%3959D0D102B8.cp preserve=no
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

#ifndef m4cacerts_h
#define m4cacerts_h 1

#include "m4cacert_dll.hpp"
#include <caautenticator.hpp>
#include <openssl/x509.h>
#include <openssl/evp.h>

#include "m4ipcs.hpp"

// ClMutex
#include "syncro.hpp"


typedef map<m4uint32_t, EVP_PKEY*> DomainKeyMap_t; 

class ClCertificateKey
{
public :

	ClCertificateKey () ;

	// Obtener par de claves pública y privada.
	static EVP_PKEY * GetCertificateKey ( ClICaAuthenticator * ai_poAuthenticator ) ;

	// Establecer clave pública y privada.
	static m4return_t SetCertificateKey ( ClICaAuthenticator * ai_poAuthenticator ) ;

	// deprecado
	// m4return_t GenerateKeysPair ( m4pchar_t & ao_strPrivateKey , m4uint32_t & ao_iPrivLen , m4pchar_t & ao_strPublicKey , m4uint32_t & ao_iPubLen) ;


private:

	EVP_PKEY * _RecoverCertificatePrivateKey ( ClICaAuthenticator * ai_pAuthenticator )  ;

	ClCertificateKey ( ClICaAuthenticator * ai_poAuthenticator ) ;

	static EVP_PKEY * m_poCertificateKey ;

	static ClCertificateKey * m_poInstance ;


};


class ClCertificateDomainKey
{
public :

	ClCertificateDomainKey () ;

	// Obtener par de claves pública y privada.
	static EVP_PKEY * GetCertificateKey ( ClICaAuthenticator * ai_poAuthenticator ) ;

	// Establecer clave pública y privada.
	static m4return_t SetCertificateKey ( ClICaAuthenticator * ai_poAuthenticator ) ;

	// deprecado
	// m4return_t GenerateKeysPair ( m4pchar_t & ao_strPrivateKey , m4uint32_t & ao_iPrivLen , m4pchar_t & ao_strPublicKey , m4uint32_t & ao_iPubLen) ;


private:

	static EVP_PKEY * _RetrievePrivateDomainKey ( ClICaAuthenticator * ai_pAuthenticator )  ;
};



class M4_DECL_M4CACERT M4CaCert 
{

  public:

    M4CaCert ( ClICaAuthenticator * ai_poAuthenticator );

    ~M4CaCert ();

	m4return_t GetM4Certificate ( m4string_t ai_strUserName , m4string_t & ao_szCertificate, X509* *ao_poCert, m4bool_t ai_bCheckUser, m4bool_t ai_bExpire ) ;

	m4return_t VerifyCertificate ( m4string_t ao_szCertificate , m4string_t & ao_strUserName ) ;

	m4return_t ExportCertificate ( m4string_t ai_strUserName , m4string_t ai_strCertPath, m4string_t ai_strCertPass ) ;

	m4return_t CheckM4WSSOCertificate( m4string_t ai_sCertificate, m4string_t ai_sUserId, m4bool_t &ao_bGoOn, m4string_t &ao_sDomainId ) ;

  private:

	m4return_t _CreateCertificate ( m4string_t ai_strUserName , X509 *& ao_poX509, m4bool_t ai_bExpire ) ;
	
	m4return_t _CreateDomainCertificate ( m4string_t ai_strUserName , X509 *& ao_poX509, m4bool_t ai_bExpire ) ;	

	m4return_t _SerializeCertificate ( X509 * ai_poX509 , m4string_t & ao_szCertificate ) ;

    ClICaAuthenticator *	m_poAuthenticator;

	m4pchar_t ConvertStringFromUTF8( m4pchar_t ai_pcInput );

	void _InitializeOpenSSL();
	
	time_t _ConvertJulianDateToTime_t(m4double_t ai_dJulianDate);

	static m4bool_t m_bOpenSSLInitialized;

	static ClMutex m_oMtxInitialization ;
};


#endif
