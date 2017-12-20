//## begin module%3A7EC0CF02F5.cm preserve=no
//## end module%3A7EC0CF02F5.cm

//## begin module%3A7EC0CF02F5.cp preserve=no
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
//## end module%3A7EC0CF02F5.cp

//## Module: dspappctlreq%3A7EC0CF02F5; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\dspappctlreq.cpp

//## begin module%3A7EC0CF02F5.additionalIncludes preserve=no
//## end module%3A7EC0CF02F5.additionalIncludes

//## begin module%3A7EC0CF02F5.includes preserve=yes
#include <m4memdbg.h>
#include <m4srvtrace.h>
#include <m4dpchres.hpp>
#include <dspanswergen.hpp>
//## end module%3A7EC0CF02F5.includes

// m4condition
#include <m4condition.hpp>
// dspreq
#include <dspreq.hpp>
// dspappctlreq
#include <dspappctlreq.hpp>
//## begin module%3A7EC0CF02F5.declarations preserve=no
//## end module%3A7EC0CF02F5.declarations

//## begin module%3A7EC0CF02F5.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID "DPCH"
//## end module%3A7EC0CF02F5.additionalDeclarations


// Class ClDspAppCtlRequest 




ClDspAppCtlRequest::ClDspAppCtlRequest (m4uint64_t ai_iRequestID, ClLogSystem *ai_poLog)
  //## begin ClDspAppCtlRequest::ClDspAppCtlRequest%981385465.hasinit preserve=no
  //## end ClDspAppCtlRequest::ClDspAppCtlRequest%981385465.hasinit
  //## begin ClDspAppCtlRequest::ClDspAppCtlRequest%981385465.initialization preserve=yes
  :ClDspRequest ( NULL, ai_iRequestID, 0 , ai_poLog )
  //## end ClDspAppCtlRequest::ClDspAppCtlRequest%981385465.initialization
{
  //## begin ClDspAppCtlRequest::ClDspAppCtlRequest%981385465.body preserve=yes
	m_oReadCondition.Init();

	m_bReadyRead = M4_FALSE ;

  //## end ClDspAppCtlRequest::ClDspAppCtlRequest%981385465.body
}


ClDspAppCtlRequest::~ClDspAppCtlRequest ()
{
  //## begin ClDspAppCtlRequest::~ClDspAppCtlRequest%981385466.body preserve=yes
	if (NULL != m_poBufferIn)
    {
        delete m_poBufferIn;
        m_poBufferIn = NULL;
    }
  //## end ClDspAppCtlRequest::~ClDspAppCtlRequest%981385466.body
}



//## Other Operations (implementation)
void ClDspAppCtlRequest::WaitRead ()
{
  //## begin ClDspAppCtlRequest::WaitRead%981385467.body preserve=yes
	while(m_bReadyRead == M4_FALSE)
	m_oReadCondition.Wait(-1);
  //## end ClDspAppCtlRequest::WaitRead%981385467.body
}

void ClDspAppCtlRequest::ReadReady ()
{
  //## begin ClDspAppCtlRequest::ReadReady%981385468.body preserve=yes
	m_bReadyRead=M4_TRUE;
	m_oReadCondition.Signal();
  //## end ClDspAppCtlRequest::ReadReady%981385468.body
}

m4return_t ClDspAppCtlRequest::BuildReqReply (ClSiteNodeContainer &ai_oSiteNodeContainer, ClDspAnswerGen *ai_poAnswerGen)
{
  //## begin ClDspAppCtlRequest::BuildReqReply%981452486.body preserve=yes
	if ( M4_ERROR == ai_poAnswerGen->GenerateAnswer (ai_oSiteNodeContainer, m_poBufferIn ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMINAGENTSTARTXMLANSWER,
			"Error starting Server Configuration. Cannot generate XML answer." );			
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClDspAppCtlRequest::BuildReqReply%981452486.body
}

void ClDspAppCtlRequest::SetConnectionID (m4uint32_t ai_iConnectionID)
{
  //## begin ClDspAppCtlRequest::SetConnectionID%981452487.body preserve=yes
  //## end ClDspAppCtlRequest::SetConnectionID%981452487.body
}

// Additional Declarations
  //## begin ClDspAppCtlRequest%3A7EBDDB0257.declarations preserve=yes
  //## end ClDspAppCtlRequest%3A7EBDDB0257.declarations

//## begin module%3A7EC0CF02F5.epilog preserve=yes
//## end module%3A7EC0CF02F5.epilog
