//## begin module%35643A2300E1.cm preserve=no
//## end module%35643A2300E1.cm

//## begin module%35643A2300E1.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Protocol
//	 File:              BoomRequest.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980521
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%35643A2300E1.cp

//## Module: BoomRequest%35643A2300E1; Subprogram body
//## Subsystem: M4Request::src%379439BF01B7
//## Source file: F:\integration\m4request\src\boomrequest.cpp

//## begin module%35643A2300E1.additionalIncludes preserve=no
//## end module%35643A2300E1.additionalIncludes

//## begin module%35643A2300E1.includes preserve=yes
//## end module%35643A2300E1.includes

// BoomRequest
#include <boomrequest.hpp>
//## begin module%35643A2300E1.declarations preserve=no
//## end module%35643A2300E1.declarations

//## begin module%35643A2300E1.additionalDeclarations preserve=yes
//## end module%35643A2300E1.additionalDeclarations


// Class ClBoomerangRequest 




ClBoomerangRequest::ClBoomerangRequest ()
  //## begin ClBoomerangRequest::ClBoomerangRequest%895761854.hasinit preserve=no
      : m_pSourceQ(NULL), m_pReturn(NULL), m_pRequest(NULL)
  //## end ClBoomerangRequest::ClBoomerangRequest%895761854.hasinit
  //## begin ClBoomerangRequest::ClBoomerangRequest%895761854.initialization preserve=yes
  , ClBaseRequest(0)
  //## end ClBoomerangRequest::ClBoomerangRequest%895761854.initialization
{
  //## begin ClBoomerangRequest::ClBoomerangRequest%895761854.body preserve=yes
	m_poMsgContainer = NULL ;
  //## end ClBoomerangRequest::ClBoomerangRequest%895761854.body
}


ClBoomerangRequest::~ClBoomerangRequest ()
{
  //## begin ClBoomerangRequest::~ClBoomerangRequest%895761855.body preserve=yes
	if (NULL != m_pRequest)
	{
		delete m_pRequest;
		m_pRequest = NULL;
	}

	if (NULL != m_pReturn)
	{
		delete m_pReturn;
		m_pReturn = NULL;
	}
  //## end ClBoomerangRequest::~ClBoomerangRequest%895761855.body
}



//## Other Operations (implementation)
m4return_t ClBoomerangRequest::Return ()
{
  //## begin ClBoomerangRequest::Return%895761856.body preserve=yes
	if ( NULL != m_pReturn )
	{
		// bg 167189
		m_pReturn->Lock();
		m_pReturn->Notify();
		m_pReturn->Unlock();
	}
	else
	{
		if ( NULL == m_pSourceQ )
		{
			return M4_ERROR;
		}
		else
		{
			ClControlQ *pThreadSafeQ = m_pSourceQ;
			m_pSourceQ = NULL;
			pThreadSafeQ->Push(this);
		}
	}

	return M4_SUCCESS;
  //## end ClBoomerangRequest::Return%895761856.body
}

m4return_t ClBoomerangRequest::Throw (ClServiceQ *ai_pTarget, ClControlQ *ai_pSource)
{
  //## begin ClBoomerangRequest::Throw%895761857.body preserve=yes
	ClBaseRequest		*item = this;

	if (NULL == ai_pTarget)
	{
		return M4_ERROR;
	}

	if (NULL != ai_pSource)
	{
		m_pSourceQ = ai_pSource;
		ai_pTarget->Push(item);

		return M4_SUCCESS;
	}

	// bg 167189
	m_pReturn = new ClBooleanCondition();
	m_pReturn->Init();
	m_pReturn->Lock();

	ai_pTarget->Push(item);

	m_pReturn->Wait();
	m_pReturn->Unlock();
	
	delete m_pReturn;
	m_pReturn = NULL;

	return M4_SUCCESS;
  //## end ClBoomerangRequest::Throw%895761857.body
}

m4return_t ClBoomerangRequest::Recycle ()
{
  //## begin ClBoomerangRequest::Recycle%941721567.body preserve=yes
return M4_ERROR ;
  //## end ClBoomerangRequest::Recycle%941721567.body
}

m4bool_t ClBoomerangRequest::CanRecycle ()
{
  //## begin ClBoomerangRequest::CanRecycle%941721568.body preserve=yes
	return M4_FALSE ;
  //## end ClBoomerangRequest::CanRecycle%941721568.body
}

m4return_t ClBoomerangRequest::Destroy ()
{
  //## begin ClBoomerangRequest::Destroy%941721569.body preserve=yes
	delete this ;
	return M4_SUCCESS ;
  //## end ClBoomerangRequest::Destroy%941721569.body
}

m4bool_t ClBoomerangRequest::CanDestroy ()
{
  //## begin ClBoomerangRequest::CanDestroy%941721570.body preserve=yes
	return M4_TRUE ;
  //## end ClBoomerangRequest::CanDestroy%941721570.body
}

// Additional Declarations
  //## begin ClBoomerangRequest%356434DF0083.declarations preserve=yes
  //## end ClBoomerangRequest%356434DF0083.declarations

//## begin module%35643A2300E1.epilog preserve=yes
//## end module%35643A2300E1.epilog
