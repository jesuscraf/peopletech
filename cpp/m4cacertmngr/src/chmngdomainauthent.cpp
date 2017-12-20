
#include "certmngr.hpp"
#include "chmngauthent.hpp"
#include "chmngdomainauthent.hpp"
#include "m4log.hpp"
#include "m4srvres.hpp"
#include "cllgadap.hpp"
#include "m4variant.hpp"


ClChManagerDomainAuthenticator::ClChManagerDomainAuthenticator(ClChannelManager * ai_poChannelManager, m4int32_t ai_iCertificateId, ClChannel *ai_poSessionChannel)
{
	// Guardar id. certificado.
	m_iCertificateId = ai_iCertificateId;

	// inits channel manager
	m_poSessionChannel = ai_poSessionChannel;
	
	// Inicializar.
	m_bIsLoaded = M4_FALSE;

	// Cargar datos del certificado.
	if (_LoadCertificateData(ai_poChannelManager) == M4_SUCCESS)
	{
		m_bIsLoaded = M4_TRUE;
	}
}

AuthenticatorType_t ClChManagerDomainAuthenticator::GetType()
{
	return m4sso_multidomain;
}

m4return_t ClChManagerDomainAuthenticator::AuthenticateUser(m4string_t ai_szUserName)
{
	SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "AuthenticateUser: not implemented");
	return M4_ERROR;
}

m4return_t ClChManagerDomainAuthenticator::GetKeysFromRepository(m4pchar_t &ao_pcPublicKey, m4uint32_t &ao_iPublicKeyLen, m4pchar_t &ao_pcPrivateKey, m4uint32_t &ao_iPrivateKeyLen)
{
	// Inicializar.
	ao_pcPublicKey = NULL;
	ao_pcPrivateKey = NULL;

	// Verificar que se ha podido cargar la información del certificado.
	if (m_bIsLoaded == M4_FALSE)
	{
		return M4_ERROR;
	}

	// Devolver las clave pública.
	if (m_sPublicKey.size() != 0)
	{	
		ao_iPublicKeyLen = m_sPublicKey.size();
		ao_pcPublicKey = new char[ao_iPublicKeyLen];
		m_sPublicKey.copy(ao_pcPublicKey, ao_iPublicKeyLen);
	}

	// Devolver las claves privada.
	if (m_sPrivateKey.size() != 0)
	{
		ao_iPrivateKeyLen = m_sPrivateKey.size();
		ao_pcPrivateKey = new char[ao_iPrivateKeyLen];
		m_sPrivateKey.copy(ao_pcPrivateKey, ao_iPrivateKeyLen);
	}

	return M4_SUCCESS;
}

m4return_t ClChManagerDomainAuthenticator::GetCertificateId(m4uint32_t &ao_iCertificateId)
{
	if (m_bIsLoaded == M4_FALSE)
	{
		return M4_ERROR;
	}

	ao_iCertificateId = m_iCertificateId;
	return M4_SUCCESS;
}

m4return_t ClChManagerDomainAuthenticator::GetDomainId(string &ao_sDomainId)
{
	if (m_bIsLoaded == M4_FALSE)
	{
		return M4_ERROR;
	}

	// Devolver el dominio asociado al certificado.
	ao_sDomainId = m_sDomainId;
	return M4_SUCCESS;
}

m4return_t ClChManagerDomainAuthenticator::GetDates(m4double_t &ao_dStartDate, m4double_t &ao_dEndDate)
{
	if (m_bIsLoaded == M4_FALSE)
	{
		return M4_ERROR;
	}

	// Devolver las fechas del certificado.
	ao_dStartDate = m_dDtStart;
	ao_dEndDate = m_dDtEnd;
	return M4_SUCCESS;
}

m4return_t ClChManagerDomainAuthenticator::_LoadCertificateData(ClChannelManager * ai_poChannelManager)
{
	ClChannel			* poChannel = NULL;
	ClAccess			* poAccess = NULL;
	ClNode				* poNode = NULL;
	ClItem				* poItem = NULL;
	m4return_t			iResult = M4_SUCCESS;
	m4uint32_t			iArgNum = 1;
	m4uint32_t			iCount = 0;
	m4uint32_t			iPublicKeyLen = 0;
	m4uint32_t			iPrivateKeyLen = 0;
	m4uint32_t			iOutLen = 0;
	m4pchar_t			pPublicKey = NULL;
	m4pchar_t			pPrivateKey = NULL;
	m4VariantType		vCertificateId;
	m4VariantType		vDomainId;
	m4VariantType		vDtStart;
	m4VariantType		vDtEnd;
	m4VariantType		vRes;
	
	// Obtener el channel manager.
	if (ai_poChannelManager == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error in initialization, ai_poChannelManager == NULL");
		return M4_ERROR;
	}

	// Crear el canal.
	iResult = ai_poChannelManager->CreateChannel(poChannel);
	if (iResult != M4_SUCCESS || poChannel == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error creating channel");
		return M4_ERROR;
	}

	// Levantar el canal.
	iResult = poChannel->BuildFromId("SSC_ULOGON");
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error building channel");
		return M4_ERROR;
	}

	// Crear el acceso.
	poAccess = poChannel->CreateAccess();
	if (poAccess == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error creating channel access");
		return M4_ERROR;
	}

	// Obtener el nodo.
	poNode = &(poAccess->Node["SSC_DOMAIN_KEYS"]);
	if (poNode == NULL)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error getting node");
		return M4_ERROR;
	}

	// Cargar los datos del certificado.
	vCertificateId.Type = M4CL_CPP_TYPE_NUMBER;
	vCertificateId.Data.DoubleData = m_iCertificateId;		
	iResult = poNode->RecordSet.Current.Item["LOAD_CERTIFICATE"].StackPush(vCertificateId);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error loading certificate data.");
		return M4_ERROR;
	}

	iResult = poNode->RecordSet.Current.Item["LOAD_CERTIFICATE"].Call();
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error loading certificate data.");
		return M4_ERROR;
	}
	
	iResult = poNode->RecordSet.Current.Item[ "LOAD_CERTIFICATE" ].StackPop(vRes);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error loading certificate data.");
		return M4_ERROR;
	}
	
	if (vRes.Data.DoubleData != 0)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Error loading certificate data.");
		return M4_ERROR;
	}
	
	// Verificar que hay registro.
	iCount = poNode->RecordSet.Current.Count();
	if (iCount == 0)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: Certificate data not found.");
		return M4_ERROR;
	}
	
	// Obtener el dominio.
	iResult = poNode->RecordSet.Current.Item["ID_DOMAIN"].Value.Get(vDomainId);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: id domain not found.");
		return M4_ERROR;
	}

	if (vDomainId.Type != M4CL_M4_TYPE_NULL && vDomainId.Data.PointerChar != NULL)
	{
		m_sDomainId = (m4pchar_t) vDomainId.Data.PointerChar;
	}

	// Obtener la fecha inicio del certificado.
	iResult = poNode->RecordSet.Current.Item["DT_START"].Value.Get(vDtStart);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: start date not found.");
		return M4_ERROR;
	}

	if (vDtStart.Type != M4CL_CPP_TYPE_NULL)
	{
		m_dDtStart = (m4double_t) vDtStart.Data.DoubleData;
	}
	
	// Obtener la fecha fin del certificado.
	iResult = poNode->RecordSet.Current.Item["DT_END"].Value.Get(vDtEnd);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: end date not found.");
		return M4_ERROR;
	}

	if (vDtEnd.Type != M4CL_CPP_TYPE_NULL)
	{
		m_dDtEnd = (m4double_t) vDtEnd.Data.DoubleData;
	}
	
	// Obtener la clave pública.
	poItem = & poNode->RecordSet.Current.Item["PUBLIC_KEY"];
	if (poItem->GetIndex() == M4DM_EOF_INDEX)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: error getting public key item.");
		return M4_ERROR;
	}

	// Comprobar la longitud
	iPublicKeyLen = poItem->Value.Size();
	pPublicKey = new m4char_t[iPublicKeyLen];
	
	// Leer el valor del item
	iResult = poItem->Value.Get(pPublicKey, iPublicKeyLen, 0, &iOutLen);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: error getting public key item value.");
		delete [] pPublicKey;
		return M4_ERROR;
	}

	// Guardar clave pública.
	m_sPublicKey.assign(pPublicKey, iPublicKeyLen);
	delete [] pPublicKey;

	// Obtener la clave privada.
	poItem = & poNode->RecordSet.Current.Item["PRIVATE_KEY"];
	if (poItem->GetIndex() == M4DM_EOF_INDEX)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: error getting private key item.");
		return M4_ERROR;
	}

	// Comprobar la longitud
	iPrivateKeyLen = poItem->Value.Size();
	pPrivateKey = new m4char_t[iPrivateKeyLen];
	
	// Leer el valor del item
	iOutLen = 0;
	iResult = poItem->Value.Get(pPrivateKey, iPrivateKeyLen, 0, &iOutLen);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "_LoadCertificateData: error getting private key item.");
		delete [] pPrivateKey;
		return M4_ERROR;
	}

	// Guardar clave privada.
	m_sPrivateKey.assign(pPrivateKey, iPrivateKeyLen);
	delete [] pPrivateKey;

	// Destruir el acceso y el canal.
	poAccess->Destroy();
	poChannel->Destroy();

	return M4_SUCCESS;
}

m4void_t   ClChManagerDomainAuthenticator::SetSessionProperty(m4pcchar_t ai_sPropertyId, m4string_t &ai_sPropertyValue, m4bool_t ai_bIsPublic)
{
	CertManagerSetSessionChannelProperty(m_poSessionChannel, ai_sPropertyId, ai_sPropertyValue, ai_bIsPublic);
}
