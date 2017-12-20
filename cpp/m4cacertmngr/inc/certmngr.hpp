//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cacertmngr.dll
// File:                certmngr.hpp   
// Project:             Mind3x      
// Author:              Meta Software M.S. , S.A
// Date:                03/07/2002
// Language:            C++
//
// Definition:
//    This module declares general API for the certificate manager
//
//==============================================================================



#ifndef _CERTMNGR_HPP_
#define _CERTMNGR_HPP_


#include "m4cacertmngr_dll.hpp"
#include "m4types.hpp"
#include "m4string.hpp"
#include <access.hpp>

class ClExecuteItemI;
class m4VariantType;

class M4_DECL_M4CACERTMNGR ClCertManager {

public:

	ClCertManager(ClExecuteItemI *ai_poItemExecutor);

	m4return_t GetCertificate (m4pchar_t ai_pszUserName , m4pchar_t &ao_szCertificate , m4uint32_t &ao_iCertificateLen , m4pchar_t & ao_szSessContext , m4uint32_t &ao_iSessContextLen);

	m4return_t CheckCertificate (m4pcchar_t ai_pcCertType, m4string_t &ao_sUserName , m4pchar_t ai_szCertificate , m4uint32_t ai_iCertificateLen);

private:
	ClExecuteItemI *m_poItemExecutor;
};

// exported functions
extern "C" M4_DECL_M4CACERTMNGR m4return_t GenerateKeys				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4CACERTMNGR m4return_t GenerateSessionContext	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4CACERTMNGR m4return_t ExportKeysToFile			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4CACERTMNGR m4return_t IsValidM4SSOCert			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4CACERTMNGR m4return_t Generate2FASecretKey		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4CACERTMNGR m4return_t Check2FAVerificationCode	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4CACERTMNGR m4return_t EncodeSecretKeyTicket	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4CACERTMNGR m4return_t DecodeSecretKeyTicket	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

// internal utility functins
m4void_t CertManagerSetSessionChannelProperty(ClChannel *ai_poSessionChannel, m4pcchar_t ai_sPropertyId, m4string_t &ai_sPropertyValue, m4bool_t ai_bIsPublic);

#endif
