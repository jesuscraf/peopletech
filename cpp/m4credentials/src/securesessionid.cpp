//## begin module%396C919903B0.cm preserve=no
//## end module%396C919903B0.cm

//## begin module%396C919903B0.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%396C919903B0.cp

//## Module: SecureSessionID%396C919903B0; Subprogram body
//## Subsystem: M4Credentials::src%3959D1180378
//## Source file: F:\integration\m4credentials\src\securesessionid.cpp

//## begin module%396C919903B0.additionalIncludes preserve=no
//## end module%396C919903B0.additionalIncludes

//## begin module%396C919903B0.includes preserve=yes
//BODY INCLUDE
#include "stringutils.hpp"
//## end module%396C919903B0.includes

// SecureSessionID
#include <securesessionid.hpp>
// RawSessionID
#include <rawsessionid.hpp>
//## begin module%396C919903B0.declarations preserve=no
//## end module%396C919903B0.declarations

//## begin module%396C919903B0.additionalDeclarations preserve=yes
//## end module%396C919903B0.additionalDeclarations


// Class ClSecureSessionIDGenerator 


//## begin ClSecureSessionIDGenerator::M4MaxCipherNPasses%396EFDD603C1.attr preserve=no  protected: static m4uint_t {VAC} 4
const m4uint_t  ClSecureSessionIDGenerator::M4MaxCipherNPasses = 4;
//## end ClSecureSessionIDGenerator::M4MaxCipherNPasses%396EFDD603C1.attr

ClSecureSessionIDGenerator::ClSecureSessionIDGenerator (m4uint_t ai_iCipherNPasses)
  //## begin ClSecureSessionIDGenerator::ClSecureSessionIDGenerator%963411916.hasinit preserve=no
  //## end ClSecureSessionIDGenerator::ClSecureSessionIDGenerator%963411916.hasinit
  //## begin ClSecureSessionIDGenerator::ClSecureSessionIDGenerator%963411916.initialization preserve=yes
  //## end ClSecureSessionIDGenerator::ClSecureSessionIDGenerator%963411916.initialization
{
  //## begin ClSecureSessionIDGenerator::ClSecureSessionIDGenerator%963411916.body preserve=yes
	if (M4MaxCipherNPasses < ai_iCipherNPasses)
		m_iCipherNPasses = M4MaxCipherNPasses;
	else
		m_iCipherNPasses = ai_iCipherNPasses;
  //## end ClSecureSessionIDGenerator::ClSecureSessionIDGenerator%963411916.body
}



//## Other Operations (implementation)
m4return_t ClSecureSessionIDGenerator::CreateSessionID (const m4string_t &ai_strLoginName, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID) const
{
  //## begin ClSecureSessionIDGenerator::CreateSessionID%963411917.body preserve=yes
	m4return_t	iResult;
	m4string_t	strSessionID;
	iResult = ClRawSessionIDGenerator::CreateSessionID(ai_strLoginName, ai_oSecondsFromNow, strSessionID);
	if (M4_SUCCESS != iResult)
		return iResult;

	iResult = TStringUtils::Cipher(strSessionID, ao_strSessionID, "Meta4", m_iCipherNPasses);
	if (M4_SUCCESS != iResult)
	{
		ao_strSessionID.erase();
		return iResult;
	}

	return M4_SUCCESS;
  //## end ClSecureSessionIDGenerator::CreateSessionID%963411917.body
}

m4return_t ClSecureSessionIDGenerator::ExtractVariables (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername, m4string_t &ao_strServerName, m4string_t &ao_strServerSessionID, m4string_t &ao_strServerHost, m4string_t &ao_iServerBasePort) const
{
  //## begin ClSecureSessionIDGenerator::ExtractVariables%963411918.body preserve=yes
	m4return_t	iResult;
	m4string_t	strSessionID;
	iResult = TStringUtils::Decipher(ai_strSessionID, strSessionID, "Meta4", m_iCipherNPasses);
	if (M4_SUCCESS != iResult)
	{
		return iResult;
	}

	iResult = ClRawSessionIDGenerator::ExtractVariables(strSessionID, ao_strUsername,ao_strServerName, ao_strServerSessionID,ao_strServerHost, ao_iServerBasePort);
	if (M4_SUCCESS != iResult)
	{
		return iResult;
	}

	return M4_SUCCESS;
  //## end ClSecureSessionIDGenerator::ExtractVariables%963411918.body
}

// Additional Declarations
  //## begin ClSecureSessionIDGenerator%396C909903A7.declarations preserve=yes
  //## end ClSecureSessionIDGenerator%396C909903A7.declarations

//## begin module%396C919903B0.epilog preserve=yes
//## end module%396C919903B0.epilog
