
#ifndef caautenticator
#define caautenticator 1

#include <m4types.hpp>
#include <m4string.hpp>


// Tipo de autenticador.
typedef enum {
	
	m4sso_classical	= 0,
	m4sso_multidomain =	1

} AuthenticatorType_t;


class ClICaAuthenticator
{

public:

	virtual m4return_t AuthenticateUser ( m4string_t ai_szUserName ) = 0 ;

	virtual m4return_t GetKeysFromRepository ( m4pchar_t &ao_pcPublicKey, m4uint32_t &ao_iPublicKeyLen, m4pchar_t &ao_pcPrivateKey, m4uint32_t &ao_iPrivateKeyLen) = 0 ;

	virtual AuthenticatorType_t GetType() = 0;

	virtual m4return_t GetCertificateId(m4uint32_t &ao_iCertificateId) = 0;

	virtual m4return_t GetDomainId(string &ao_sDomainId) = 0;

	virtual m4return_t GetDates(m4double_t &ao_dStartDate, m4double_t &ao_dEndDate) = 0;

	virtual m4void_t   SetSessionProperty(m4pcchar_t ai_sPropertyId, m4string_t &ai_sPropertyValue, m4bool_t ai_bIsPublic) = 0;

} ;

#endif

