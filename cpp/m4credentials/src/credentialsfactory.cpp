//## begin module%395A3AA503C3.cm preserve=no
//## end module%395A3AA503C3.cm

//## begin module%395A3AA503C3.cp preserve=no
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
//## end module%395A3AA503C3.cp

//## Module: CredentialsFactory%395A3AA503C3; Subprogram body
//## Subsystem: M4Credentials::src%3959D1180378
//## Source file: D:\eduardoma\m4credentials\src\credentialsfactory.cpp

//## begin module%395A3AA503C3.additionalIncludes preserve=no
//## end module%395A3AA503C3.additionalIncludes

//## begin module%395A3AA503C3.includes preserve=yes
#include "blocksynchronization.hpp"
#include "cldates.hpp"
//## end module%395A3AA503C3.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// SessionIDGenerator
#include <sessionidgenerator.hpp>
// CredentialsFactory
#include <credentialsfactory.hpp>
// m4credentials
#include <m4credentials.hpp>
// CriticalSection
#include <criticalsection.hpp>
//## begin module%395A3AA503C3.declarations preserve=no
//## end module%395A3AA503C3.declarations

//## begin module%395A3AA503C3.additionalDeclarations preserve=yes
//## end module%395A3AA503C3.additionalDeclarations


// Class ClCredentialsFactory 

//## begin ClCredentialsFactory::s_poFactoryInstance%395A3ED5032E.role preserve=no  private: static ClCredentialsFactory { -> 1RHAN}
ClCredentialsFactory *ClCredentialsFactory::s_poFactoryInstance;
//## end ClCredentialsFactory::s_poFactoryInstance%395A3ED5032E.role


//## begin ClCredentialsFactory::s_oSyncrhonizer%395A3F5A024A.role preserve=yes  private: static ClCriticalSection { -> 1VHAN}
ClCriticalSection ClCredentialsFactory::s_oSyncrhonizer(M4_TRUE);
//## end ClCredentialsFactory::s_oSyncrhonizer%395A3F5A024A.role


ClCredentialsFactory::ClCredentialsFactory (const ISessionIDGenerator &ai_oSessionIDGenerator)
  //## begin ClCredentialsFactory::ClCredentialsFactory%962187531.hasinit preserve=no
  //## end ClCredentialsFactory::ClCredentialsFactory%962187531.hasinit
  //## begin ClCredentialsFactory::ClCredentialsFactory%962187531.initialization preserve=yes
  : m_poSessionIDGenerator(&ai_oSessionIDGenerator)
  //## end ClCredentialsFactory::ClCredentialsFactory%962187531.initialization
{
  //## begin ClCredentialsFactory::ClCredentialsFactory%962187531.body preserve=yes
  //## end ClCredentialsFactory::ClCredentialsFactory%962187531.body
}


ClCredentialsFactory::~ClCredentialsFactory ()
{
  //## begin ClCredentialsFactory::~ClCredentialsFactory%962187532.body preserve=yes
  //## end ClCredentialsFactory::~ClCredentialsFactory%962187532.body
}



//## Other Operations (implementation)
M4Credentials * ClCredentialsFactory::CreateCredential (const m4string_t &ai_strMechanism, const m4string_t &ai_strLoginName, const m4string_t &ai_strPassword, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID)
{
  //## begin ClCredentialsFactory::CreateCredential%962187533.body preserve=yes
	ClCredentialsFactory	*poCredFactoryInstance;
	poCredFactoryInstance = Instance();
	if (0 == poCredFactoryInstance)
		return 0;

	return poCredFactoryInstance->_CreateCredential(ai_strMechanism, ai_strLoginName, ai_strPassword, ai_oSecondsFromNow, ao_strSessionID);
  //## end ClCredentialsFactory::CreateCredential%962187533.body
}

M4Credentials * ClCredentialsFactory::CreateCredential (const m4string_t &ai_strMechanism, const m4string_t &ai_strLoginName, const m4string_t &ai_strPassword, m4string_t &ao_strSessionID)
{
  //## begin ClCredentialsFactory::CreateCredential%962964032.body preserve=yes
	return CreateCredential (ai_strMechanism, ai_strLoginName, ai_strPassword, 0, ao_strSessionID);
  //## end ClCredentialsFactory::CreateCredential%962964032.body
}

M4Credentials * ClCredentialsFactory::CreateCredential (const m4string_t &ai_strSessionID)
{
  //## begin ClCredentialsFactory::CreateCredential%962621271.body preserve=yes
	ClCredentialsFactory	*poCredFactoryInstance;
	poCredFactoryInstance = Instance();
	if (0 == poCredFactoryInstance)
		return 0;

	return poCredFactoryInstance->_CreateCredential(ai_strSessionID);
  //## end ClCredentialsFactory::CreateCredential%962621271.body
}

m4return_t ClCredentialsFactory::DestroyCredential (M4Credentials &ai_oCredential)
{
  //## begin ClCredentialsFactory::DestroyCredential%962187534.body preserve=yes
	ClCredentialsFactory	*poCredFactoryInstance;
	poCredFactoryInstance = Instance();
	if (0 == poCredFactoryInstance)
		return M4_ERROR;

	return poCredFactoryInstance->_DestroyCredential(ai_oCredential);
  //## end ClCredentialsFactory::DestroyCredential%962187534.body
}

void ClCredentialsFactory::Reset ()
{
  //## begin ClCredentialsFactory::Reset%963411914.body preserve=yes
	ClBlockSync		oBlocker(s_oSyncrhonizer);

	if (0 != s_poFactoryInstance)
	{
		delete s_poFactoryInstance;
		s_poFactoryInstance = 0;
	}
	
	m_poSessionIDGenerator = 0;
  //## end ClCredentialsFactory::Reset%963411914.body
}

ClCredentialsFactory * ClCredentialsFactory::Instance (const ISessionIDGenerator &ai_oSessionIDGenerator)
{
  //## begin ClCredentialsFactory::Instance%962187535.body preserve=yes
	if (0 == &ai_oSessionIDGenerator)
		return 0;

	ClBlockSync		oBlocker(s_oSyncrhonizer);

	if (0 == s_poFactoryInstance)
		s_poFactoryInstance = new ClCredentialsFactory(ai_oSessionIDGenerator);

	return s_poFactoryInstance;
  //## end ClCredentialsFactory::Instance%962187535.body
}

M4Credentials * ClCredentialsFactory::_CreateCredential (const m4string_t &ai_strMechanism, const m4string_t &ai_strLoginName, const m4string_t &ai_strPassword, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID)
{
  //## begin ClCredentialsFactory::_CreateCredential%962266647.body preserve=yes
	M4Credentials::TRoleInfoList	aNewRoleInfoList;
	m4return_t		iRet = _GetRoleList(aNewRoleInfoList);
	if (M4_SUCCESS != iRet)
		return 0;

	iRet = m_poSessionIDGenerator->CreateSessionID(ai_strLoginName, ai_oSecondsFromNow, ao_strSessionID);
	if (M4_SUCCESS != iRet)
		return 0;

	return new M4Credentials(ai_strLoginName, aNewRoleInfoList, ao_strSessionID);
  //## end ClCredentialsFactory::_CreateCredential%962266647.body
}

M4Credentials * ClCredentialsFactory::_CreateCredential (const m4string_t &ai_strSessionID)
{
  //## begin ClCredentialsFactory::_CreateCredential%962621272.body preserve=yes
	M4Credentials * poCredential ;
	m4uint32_t iServerSessionID  ;
	M4Credentials::TRoleInfoList	aNewRoleInfoList;
	m4return_t		iRet = _GetRoleList(aNewRoleInfoList);
	if (M4_SUCCESS != iRet)
		return 0;

	m4string_t		strLoginName , strServerName , strhostName , strBasePort , strServerSessionnID;
	iRet = m_poSessionIDGenerator->ExtractVariables(ai_strSessionID, strLoginName, strServerName , strServerSessionnID, strhostName , strBasePort );
	if (M4_SUCCESS != iRet)
		return 0;

	poCredential = new M4Credentials(strLoginName, aNewRoleInfoList, ai_strSessionID);

	sscanf ( strServerSessionnID.c_str() , "%d" , &iServerSessionID ) ;

	poCredential -> SetSessId ( iServerSessionID ) ;

	return poCredential ;
  //## end ClCredentialsFactory::_CreateCredential%962621272.body
}

m4return_t ClCredentialsFactory::_DestroyCredential (M4Credentials &ai_oCredential)
{
  //## begin ClCredentialsFactory::_DestroyCredential%962266648.body preserve=yes
	M4Credentials	*poCredential = &ai_oCredential;
	if (0 == poCredential)
		return M4_ERROR;

	delete poCredential;
	return M4_SUCCESS;
  //## end ClCredentialsFactory::_DestroyCredential%962266648.body
}

m4return_t ClCredentialsFactory::_GetRoleList (M4Credentials::TRoleInfoList &ao_oRoleList)
{
  //## begin ClCredentialsFactory::_GetRoleList%963411915.body preserve=yes
	ClRoleInfo	aNewRoleInfo("RoleID", "RSMID");

	ao_oRoleList.push_back(aNewRoleInfo);

	return M4_SUCCESS;
  //## end ClCredentialsFactory::_GetRoleList%963411915.body
}

// Additional Declarations
  //## begin ClCredentialsFactory%395A3A6F02E9.declarations preserve=yes
  //## end ClCredentialsFactory%395A3A6F02E9.declarations

//## begin module%395A3AA503C3.epilog preserve=yes
//## end module%395A3AA503C3.epilog
