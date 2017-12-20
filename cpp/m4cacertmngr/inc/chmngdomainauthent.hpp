
#ifndef chmngdomainauthent_hpp
#define chmngdomainauthent_hpp

#include <caautenticator.hpp>
#include <m4cacertmngr_dll.hpp>

class ClExecuteItemI;

class M4_DECL_M4CACERTMNGR ClChManagerDomainAuthenticator: public ClICaAuthenticator
{

public:

	ClChManagerDomainAuthenticator(ClChannelManager * ai_poChannelManager, m4int32_t ai_iCertificateId, ClChannel *ai_poSessionChannel);

	AuthenticatorType_t GetType();
	
	m4return_t AuthenticateUser(m4string_t ai_szUserName);

	m4return_t GetKeysFromRepository(m4pchar_t &ao_pcPublicKey, m4uint32_t &ao_iPublicKeyLen, m4pchar_t &ao_pcPrivateKey, m4uint32_t &ao_iPrivateKeyLen);

	m4return_t GetCertificateId(m4uint32_t &ao_iCertificateId);

	m4return_t GetDomainId(string &ao_sDomainId);

	m4return_t GetDates(m4double_t &ao_dStartDate, m4double_t &ao_dEndDate);

	m4void_t   SetSessionProperty(m4pcchar_t ai_sPropertyId, m4string_t &ai_sPropertyValue, m4bool_t ai_bIsPublic);


private:

	// Id. certificado.
	m4int32_t		m_iCertificateId;

	// Id. dominio.
	string			m_sDomainId;
	
	// Fecha inicio certificado.
	m4double_t		m_dDtStart;

	// Fecha fin certificado.
	m4double_t		m_dDtEnd;

	// Clave pública.
	string			m_sPublicKey;
	
	// Clave privada.
	string			m_sPrivateKey;

	// Flag indicando si se ha cargado correctamente la información del certificado.
	m4bool_t		m_bIsLoaded;

	// Carga información de un certificado.
	m4return_t _LoadCertificateData(ClChannelManager * ai_poChannelManager);

	// The session channel
	ClChannel *m_poSessionChannel;
};

#endif

