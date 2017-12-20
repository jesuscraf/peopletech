
#include "certmngr.hpp"
#include "chmngauthent.hpp"
#include "m4log.hpp"
#include "m4srvres.hpp"
#include "cllgadap.hpp"
#include "m4variant.hpp"
//M4Date
#include <m4date.hpp>

#define M4SSO_SERIAL_NUM 1L
#define M4SSO_DOMAIN ""

ClChManagerAuthenticator::ClChManagerAuthenticator  ( ClExecuteItemI *ai_poItemExecutor, ClChannel *ai_poChannel)
{
	m_poItemExecutor = ai_poItemExecutor ;
	m_poSessionChannel = ai_poChannel;
}

AuthenticatorType_t ClChManagerAuthenticator::GetType()
{
	return m4sso_classical;
}

m4return_t ClChManagerAuthenticator::AuthenticateUser ( m4string_t ai_szUserName ) 
{
	// check members
	if (m_poItemExecutor == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error in initialization");
		return M4_ERROR;
	}

	m4VariantType	avArguments[ 3 ];
	avArguments[ 0 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 0 ].Data.PointerChar = (m4pchar_t) ai_szUserName.c_str();
	avArguments[ 1 ].Type = M4CL_CPP_TYPE_STRING_VAR ;
	avArguments[ 1 ].Data.PointerChar = "";
	avArguments[ 2 ].Type = M4CL_CPP_TYPE_NUMBER ;
	avArguments[ 2 ].Data.DoubleData = M4_TRUE;

	m4return_t iRes = m_poItemExecutor->Execute("SSC_APPUSER","SSC_APPUSER","_AUTHENTICODE",avArguments,3,NULL,0,NULL,NULL);

	if (M4_SUCCESS > iRes)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG,  "Authentication: Error in credentials" );
		return iRes;
	}

	return iRes;
}

m4return_t ClChManagerAuthenticator::GetKeysFromRepository ( m4pchar_t &ao_pcPublicKey, m4uint32_t &ao_iPublicKeyLen, m4pchar_t &ao_pcPrivateKey, m4uint32_t &ao_iPrivateKeyLen)
{

	// check members
	if (m_poItemExecutor == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error in initialization");
		return M4_ERROR;
	}

	m4return_t iRes = m_poItemExecutor->Execute("SSC_ULOGON","SSC_ULOGON_PASSW","GET_DEFAULT_PASSW",NULL,0,NULL,0,NULL,NULL);

	if (M4_SUCCESS != iRes)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG,  "Authentication: Error in credentials" );
		return iRes;
	}

	iRes = m_poItemExecutor->GetBinaryItem("SSC_ULOGON_PASSW","PUBLIC_KEY",ao_pcPublicKey,ao_iPublicKeyLen);

	if (M4_SUCCESS != iRes)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG,  "Authentication: Error reading private/public keys");
		return iRes;
	}

	iRes = m_poItemExecutor->GetBinaryItem("SSC_ULOGON_PASSW","PRIVATE_KEY",ao_pcPrivateKey,ao_iPrivateKeyLen);

	if (M4_SUCCESS != iRes)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG,  "Authentication: Error reading private/public keys");
		return iRes;
	}

	return iRes;
}

m4return_t ClChManagerAuthenticator::GetCertificateId(m4uint32_t &ao_iCertificateId)
{
	ao_iCertificateId = M4SSO_SERIAL_NUM;
	return M4_SUCCESS;
}

m4return_t ClChManagerAuthenticator::GetDomainId(string &ao_sDomainId)
{
	ao_sDomainId = M4SSO_DOMAIN;
	return M4_SUCCESS;
}

m4return_t ClChManagerAuthenticator::GetDates(m4double_t &ao_dStartDate, m4double_t &ao_dEndDate)
{
	M4ClTimeStamp	oEndDate;
	m4double_t		dEndDate = 0;

	oEndDate.set("2038-01-19 00:00:00");
	oEndDate.get(dEndDate);

	ao_dStartDate = 0;
	ao_dEndDate = dEndDate;

	return M4_SUCCESS;
}

m4void_t   ClChManagerAuthenticator::SetSessionProperty(m4pcchar_t ai_sPropertyId, m4string_t &ai_sPropertyValue, m4bool_t ai_bIsPublic)
{
	if (m_poSessionChannel == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Authenticate: Error in initialization. Channel is null");
		return;
	}

	CertManagerSetSessionChannelProperty(m_poSessionChannel, ai_sPropertyId, ai_sPropertyValue, ai_bIsPublic);
}
