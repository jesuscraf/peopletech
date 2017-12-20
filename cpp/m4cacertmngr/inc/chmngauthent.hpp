
#ifndef chmngauthent_hpp
#define chmngauthent_hpp

#include <caautenticator.hpp>
#include <m4cacertmngr_dll.hpp>

class ClExecuteItemI;

class M4_DECL_M4CACERTMNGR ClChManagerAuthenticator: public ClICaAuthenticator
{

public:

	ClChManagerAuthenticator ( ClExecuteItemI *ai_poItemExecutor, ClChannel *ai_poSessionChannel) ;

	AuthenticatorType_t GetType() ;

	m4return_t AuthenticateUser ( m4string_t ai_szUserName ) ;

	m4return_t GetKeysFromRepository ( m4pchar_t &ao_pcPublicKey, m4uint32_t &ao_iPublicKeyLen, m4pchar_t &ao_pcPrivateKey, m4uint32_t &ao_iPrivateKeyLen);

	m4return_t GetCertificateId(m4uint32_t &ao_iCertificateId);

	m4return_t GetDomainId(string &ao_sDomainId);

	m4return_t GetDates(m4double_t &ao_dStartDate, m4double_t &ao_dEndDate);

	m4void_t   SetSessionProperty(m4pcchar_t ai_sPropertyId, m4string_t &ai_sPropertyValue, m4bool_t ai_bIsPublic);

private :

	ClExecuteItemI * m_poItemExecutor ;

	ClChannel * m_poSessionChannel;

} ;

#endif

