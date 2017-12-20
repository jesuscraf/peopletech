//## begin module%3972CDCA021D.cm preserve=no
//## end module%3972CDCA021D.cm

//## begin module%3972CDCA021D.cp preserve=no
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
//## end module%3972CDCA021D.cp

//## Module: dspxmlreq%3972CDCA021D; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: C:\WorkingFolder\m4dispatcher\src\dspxmlreq.cpp

//## begin module%3972CDCA021D.additionalIncludes preserve=no
//## end module%3972CDCA021D.additionalIncludes

//## begin module%3972CDCA021D.includes preserve=yes
#include <dspanswergen.hpp>

#include <m4memdbg.h>
#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%3972CDCA021D.includes

// dspxmlreq
#include <dspxmlreq.hpp>
// dspreq
#include <dspreq.hpp>
//## begin module%3972CDCA021D.declarations preserve=no
//## end module%3972CDCA021D.declarations

//## begin module%3972CDCA021D.additionalDeclarations preserve=yes
//## end module%3972CDCA021D.additionalDeclarations


// Class ClDspXMLRequest 

ClDspXMLRequest::ClDspXMLRequest (void *ai_poBuffer, m4uint64_t ai_iRequestID, m4uint32_t ai_iConnectionID)
  //## begin ClDspXMLRequest::ClDspXMLRequest%963825077.hasinit preserve=no
  //## end ClDspXMLRequest::ClDspXMLRequest%963825077.hasinit
  //## begin ClDspXMLRequest::ClDspXMLRequest%963825077.initialization preserve=yes
  :ClDspRequest ( ai_poBuffer, ai_iRequestID, ai_iConnectionID )
  //## end ClDspXMLRequest::ClDspXMLRequest%963825077.initialization
{
  //## begin ClDspXMLRequest::ClDspXMLRequest%963825077.body preserve=yes
  //## end ClDspXMLRequest::ClDspXMLRequest%963825077.body
}


ClDspXMLRequest::~ClDspXMLRequest ()
{
  //## begin ClDspXMLRequest::~ClDspXMLRequest%963825078.body preserve=yes
	if ( m_poBufferIn )
		M4_MDBG_DELETE( (m4pchar_t) m_poBufferIn, delete ( (m4pchar_t) m_poBufferIn ) );
	if ( m_poBufferOut ) 
		M4_MDBG_DELETE( (m4pchar_t) m_poBufferOut, delete ( (m4pchar_t) m_poBufferOut ) );

  //## end ClDspXMLRequest::~ClDspXMLRequest%963825078.body
}



//## Other Operations (implementation)
m4return_t ClDspXMLRequest::BuildReqReply (ClSiteNodeContainer &ai_oSiteNodeContainer, ClDspAnswerGen *ai_poAnswerGen)
{
  //## begin ClDspXMLRequest::BuildReqReply%963825079.body preserve=yes
	return ai_poAnswerGen ->GenerateAnswer ( ai_oSiteNodeContainer ,  m_poBufferOut , GetResult ()  , GetLog() ) ;  //## end ClDspXMLRequest::BuildReqReply%963825079.body
  //## end ClDspXMLRequest::BuildReqReply%963825079.body
}

// Additional Declarations
  //## begin ClDspXMLRequest%396DB0370193.declarations preserve=yes
  //## end ClDspXMLRequest%396DB0370193.declarations

//## begin module%3972CDCA021D.epilog preserve=yes
//## end module%3972CDCA021D.epilog
