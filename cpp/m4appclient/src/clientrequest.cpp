//## begin module%34795AB403E2.cm preserve=no
//## end module%34795AB403E2.cm

//## begin module%34795AB403E2.cp preserve=yes
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              ClientRequest.cpp
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
//	=========================================================
//	=====================
//## end module%34795AB403E2.cp

//## Module: clientrequest%34795AB403E2; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: e:\source\m4appclient\src\clientrequest.cpp

//## begin module%34795AB403E2.additionalIncludes preserve=no
//## end module%34795AB403E2.additionalIncludes

//## begin module%34795AB403E2.includes preserve=yes
#define	M4_CC_SERVICE_REQ	0
#include <clientbasicdef.hpp>
//## end module%34795AB403E2.includes

// clientrequest
#include <clientrequest.hpp>
// BaseRequest
#include <baserequest.hpp>

class ClCCService;

//## begin module%34795AB403E2.declarations preserve=no
//## end module%34795AB403E2.declarations

//## begin module%34795AB403E2.additionalDeclarations preserve=yes
//## end module%34795AB403E2.additionalDeclarations


// Class ClCCRequest 






ClCCRequest::ClCCRequest (ClCCService* ai_pService)
  //## begin ClCCRequest::ClCCRequest%880965772.hasinit preserve=no
      : m_poDebugFile(NULL), m_uiTimeout(TIMEOUT_ADMIN_SERV)
  //## end ClCCRequest::ClCCRequest%880965772.hasinit
  //## begin ClCCRequest::ClCCRequest%880965772.initialization preserve=yes
  	, ClServiceRequest(M4_CC_SERVICE_REQ, NULL)
  //## end ClCCRequest::ClCCRequest%880965772.initialization
{
  //## begin ClCCRequest::ClCCRequest%880965772.body preserve=yes

	m_poCondition  = NULL;
	m_pService    = ai_pService;
//	DEL	EMN	980507	m_pSession = ai_pSession;
	m_iState       = BUILDING;

  //## end ClCCRequest::ClCCRequest%880965772.body
}


ClCCRequest::~ClCCRequest ()
{
  //## begin ClCCRequest::~ClCCRequest%881084622.body preserve=yes

	UnsetCondition();

	m_poDebugFile=NULL;
  //## end ClCCRequest::~ClCCRequest%881084622.body
}



//## Other Operations (implementation)
m4return_t ClCCRequest::GetService (ClCCService *&ao_pService)
{
  //## begin ClCCRequest::GetService%884854701.body preserve=yes

//	MOD	EMN	980507	return m_pService;
	ao_pService = m_pService;

	return M4_SUCCESS;
  //## end ClCCRequest::GetService%884854701.body
}

eRequestStatus ClCCRequest::GetState ()
{
  //## begin ClCCRequest::GetState%881751289.body preserve=yes

	return m_iState;

  //## end ClCCRequest::GetState%881751289.body
}

void ClCCRequest::SetState (eRequestStatus ai_iState)
{
  //## begin ClCCRequest::SetState%881751290.body preserve=yes

	m_iState = ai_iState;

  //## end ClCCRequest::SetState%881751290.body
}

ClCondition * ClCCRequest::GetCondition ()
{
  //## begin ClCCRequest::GetCondition%907327031.body preserve=yes
	return m_poCondition;
  //## end ClCCRequest::GetCondition%907327031.body
}

void ClCCRequest::SetCondition (ClCondition *pCondition)
{
  //## begin ClCCRequest::SetCondition%907327032.body preserve=yes
	m_poCondition = pCondition;
  //## end ClCCRequest::SetCondition%907327032.body
}

m4return_t ClCCRequest::UnsetCondition ()
{
  //## begin ClCCRequest::UnsetCondition%907327033.body preserve=yes
	if (m_poCondition != NULL)
	{
		delete m_poCondition;
		m_poCondition = NULL;
	}

	return M4_SUCCESS;
  //## end ClCCRequest::UnsetCondition%907327033.body
}

//## begin module%34795AB403E2.epilog preserve=yes
//## end module%34795AB403E2.epilog
