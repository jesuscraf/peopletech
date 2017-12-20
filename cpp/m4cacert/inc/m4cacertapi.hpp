

#ifndef M4CACERTAPI
#define M4CACERTAPI


#include <m4types.hpp>
#include <caautenticator.hpp>
#include <m4cacert_dll.hpp>
#include <m4variant.hpp>




m4return_t M4_DECL_M4CACERT CheckM4WSSOCertificate( ClICaAuthenticator * ai_poAuthenticator, m4string_t ai_sCertificate, m4string_t ai_sUserId, m4bool_t &bIsValid,  m4string_t &ao_sDomainId ) ;

m4return_t M4_DECL_M4CACERT ExportCertificate ( ClICaAuthenticator * ai_poAuthenticator , m4string_t ai_strUserName , m4string_t ai_strCertPath, m4string_t ai_strCertPass ) ;

m4return_t M4_DECL_M4CACERT GetM4Certificate ( ClICaAuthenticator * ai_poAuthenticator , m4string_t ai_strUserName , m4string_t & ao_szCertificate ) ;

m4return_t M4_DECL_M4CACERT CheckM4Certificate( ClICaAuthenticator * ai_poAuthenticator , m4string_t & ao_strUserName , m4pchar_t  ai_szCertificate ) ;

m4return_t M4_DECL_M4CACERT GeneratePublicPrivateKey ( m4pchar_t & ao_strPrivateKey , m4uint32_t & ao_iPrivLen  , m4pchar_t & ao_strPublicKey , m4uint32_t & ao_iPubLen) ;

m4uint32_t M4_DECL_M4CACERT GetSerialFromCredential ( m4string_t ai_sCertificate, m4bool_t &bHasSerial );

#endif

