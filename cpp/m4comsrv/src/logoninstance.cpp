//## begin module%35A2434200E6.cm preserve=no
//## end module%35A2434200E6.cm

//## begin module%35A2434200E6.cp preserve=no
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
//## end module%35A2434200E6.cp

//## Module: LogonInstance%35A2434200E6; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: D:\Work\v600\m4comsrv\src\logoninstance.cpp

//## begin module%35A2434200E6.additionalIncludes preserve=no
//## end module%35A2434200E6.additionalIncludes

//## begin module%35A2434200E6.includes preserve=yes
#include <statistics.hpp>
#include <userss.hpp>
#include <usservice.hpp>
#include <clarraylibres.hpp>
#include <ussession.hpp>
#include <logoninstance.hpp>
#include <logoninterface.hpp>

class ClUsersSS;
class ClUSServiceAPI;

//## begin module%35A2434200E6.declarations preserve=no
//## end module%35A2434200E6.declarations

//## begin module%35A2434200E6.additionalDeclarations preserve=yes
//## end module%35A2434200E6.additionalDeclarations


// Class ClLogonInstance 



ClLogonInstance::ClLogonInstance (ClUsersSS *ai_pUUSS)
  //## begin ClLogonInstance::ClLogonInstance%899809127.hasinit preserve=no
      : m_pUUSS(NULL), m_pService(NULL)
  //## end ClLogonInstance::ClLogonInstance%899809127.hasinit
  //## begin ClLogonInstance::ClLogonInstance%899809127.initialization preserve=yes
  //## end ClLogonInstance::ClLogonInstance%899809127.initialization
{
  //## begin ClLogonInstance::ClLogonInstance%899809127.body preserve=yes
	m_pUUSS = ai_pUUSS;
	m_pService = ClUSServiceAPI::Instance();
  //## end ClLogonInstance::ClLogonInstance%899809127.body
}

ClLogonInstance::ClLogonInstance ()
  //## begin ClLogonInstance::ClLogonInstance%899809134.hasinit preserve=no
      : m_pUUSS(NULL), m_pService(NULL)
  //## end ClLogonInstance::ClLogonInstance%899809134.hasinit
  //## begin ClLogonInstance::ClLogonInstance%899809134.initialization preserve=yes
  //## end ClLogonInstance::ClLogonInstance%899809134.initialization
{
  //## begin ClLogonInstance::ClLogonInstance%899809134.body preserve=yes
  //## end ClLogonInstance::ClLogonInstance%899809134.body
}



//## Other Operations (implementation)
m4return_t ClLogonInstance::OpenSession (const m4pchar_t ai_pszUsername, const m4pchar_t ai_pszPassword, ClUSSessionInterface *&ao_pSession, m4bool_t ai_bDo2Way, const m4pchar_t ai_pszClientMachine)
{
  //## begin ClLogonInstance::OpenSession%899466669.body preserve=yes
//	START CONTRACT CHECK
	M4_ASSERT(NULL != m_pUUSS);
	M4_ASSERT(NULL != m_pService);
//	END CONTRACT CHECK

	m4return_t			iRet;
	m4handle_t			hObject = 0;

	m4objid_t		sessionId;
	sessionId = ClIdGenerator::GetNewId();
	hObject = (m4handle_t)sessionId;

	// bDo2Way: fix bug 0088238
	iRet = m_pService->CreateUserSession(ai_pszUsername, ai_pszPassword, NULL, hObject, M4_FALSE, NULL, NULL, ai_bDo2Way, ai_pszClientMachine, NULL, NULL, NULL, NULL, NULL, NULL);
	if (M4_SUCCESS != iRet)
		return iRet;

	ao_pSession = (ClUSSessionInterface *)hObject;
	if (NULL == ao_pSession)
		return M4_ERROR;

	ClUSUserInterface	*pUser = ao_pSession->GetUser();
	if (NULL == pUser)
	{
		delete ao_pSession;
		return M4_ERROR;
	}

//	m_pUUSS->AddUserAccount(pUser);
	m_pUUSS->AddUserSession(pUser, ao_pSession);
//	m_pUUSS->NewUser(pUser, ao_pSession);

	return M4_SUCCESS;
  //## end ClLogonInstance::OpenSession%899466669.body
}

m4return_t ClLogonInstance::OpenSessionEx (const m4pchar_t ai_pszUsername, m4pchar_t ai_pszRoleId, ClUSSessionInterface *&ao_pSession)
{
  //## begin ClLogonInstance::OpenSessionEx%899466670.body preserve=yes
//	START CONTRACT CHECK
	M4_ASSERT(NULL != m_pUUSS);
	M4_ASSERT(NULL != m_pService);
//	END CONTRACT CHECK

	m4return_t			iRet;
	m4handle_t			hObject;

	iRet = m_pService->CreateUserSession(ai_pszUsername, NULL, hObject, NULL);
	if (M4_SUCCESS != iRet)
		return iRet;

	ao_pSession = (ClUSSessionInterface *)hObject;
	if (NULL == ao_pSession)
		return M4_ERROR;

	ClUSUserInterface	*pUser = ao_pSession->GetUser();
	if (NULL == pUser)
	{
		delete ao_pSession;
		return M4_ERROR;
	}

//	m_pUUSS->AddUserAccount(pUser);
	m_pUUSS->AddUserSession(pUser, ao_pSession);

//	m_pUUSS->NewUser(pUser, ao_pSession);

	iRet = m_pService->CheckRole(ao_pSession, ai_pszRoleId, hObject);

	return iRet;
  //## end ClLogonInstance::OpenSessionEx%899466670.body
}

m4return_t ClLogonInstance::EnableRole (ClUSSessionInterface *ai_pSession, m4pchar_t ai_pszRoleId)
{
  //## begin ClLogonInstance::EnableRole%899466671.body preserve=yes
//	START CONTRACT CHECK
	M4_ASSERT(NULL != m_pUUSS);
	M4_ASSERT(NULL != m_pService);
//	END CONTRACT CHECK

	m4return_t			iRet;
	m4handle_t			hObject;

	iRet = m_pService->CheckRole(ai_pSession, ai_pszRoleId, hObject);

	return iRet;
  //## end ClLogonInstance::EnableRole%899466671.body
}

m4return_t ClLogonInstance::CloseSession (ClUSSessionInterface *ai_pSession, m4bool_t ai_bStrict)
{
  //## begin ClLogonInstance::CloseSession%899809125.body preserve=yes
	ClUSUserInterface * poUser ;
//	START CONTRACT CHECK
	M4_ASSERT(NULL != m_pUUSS);
	M4_ASSERT(NULL != m_pService);
//	END CONTRACT CHECK


	// Conseguir el ID a partir de la implementación

	if  ( ai_pSession == NULL )
	{
		return M4_ERROR;
	}

	m4objid_t lIdSession , lIdUser = 0;
	
	lIdSession = ai_pSession -> GetId ();

	if ( poUser = ai_pSession -> GetUser () )
	{
		lIdUser = poUser -> GetId () ;
	}

	// Fin obtención id session
	if (M4_TRUE == ai_bStrict)
		m_pUUSS->EndUserSession(lIdSession , lIdUser );
	else
	{
		ClStatistic	*pStats = ai_pSession->GetStats();
		if (NULL != pStats)
			delete pStats;
		ClUSUserInterface	*pUser = ai_pSession->GetUser();
		pStats = pUser->GetStats();
		if (NULL != pStats)
			delete pStats;
		delete	pUser;
		delete ai_pSession;
	}

	return M4_SUCCESS;
  //## end ClLogonInstance::CloseSession%899809125.body
}

m4return_t ClLogonInstance::CloseAuditory (ClUSSessionInterface *ai_pSession)
{
	m4return_t	iResult = m_pService->CloseAuditory(ai_pSession);

	return( iResult );
}


m4return_t ClLogonInstance::CleanAuditory (void)
{
	m4return_t	iResult = m_pService->CleanAuditory() ;

	return( iResult );
}

// Additional Declarations
  //## begin ClLogonInstance%359CC3270345.declarations preserve=yes
  //## end ClLogonInstance%359CC3270345.declarations

//## begin module%35A2434200E6.epilog preserve=yes
//## end module%35A2434200E6.epilog
