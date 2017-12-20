//## begin module%396C4E0B0358.cm preserve=no
//## end module%396C4E0B0358.cm

//## begin module%396C4E0B0358.cp preserve=no
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
//## end module%396C4E0B0358.cp

//## Module: dspreq%396C4E0B0358; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\dspreq.cpp

//## begin module%396C4E0B0358.additionalIncludes preserve=no
//## end module%396C4E0B0358.additionalIncludes

//## begin module%396C4E0B0358.includes preserve=yes
//## end module%396C4E0B0358.includes

// m4types
#include <m4types.hpp>
// dspreq
#include <dspreq.hpp>
// logsys
#include <logsys.hpp>

class ClDspAnswerGen;

//## begin module%396C4E0B0358.declarations preserve=no
//## end module%396C4E0B0358.declarations

//## begin module%396C4E0B0358.additionalDeclarations preserve=yes
//## end module%396C4E0B0358.additionalDeclarations


// Class ClDspRequest 








ClDspRequest::ClDspRequest (void *ai_poBuffer, m4uint64_t ai_iRequestID, m4uint32_t ai_iConnectionID, ClLogSystem *ai_poLog)
  //## begin ClDspRequest::ClDspRequest%963825074.hasinit preserve=no
      : m_bOwnLog(M4_FALSE)
  //## end ClDspRequest::ClDspRequest%963825074.hasinit
  //## begin ClDspRequest::ClDspRequest%963825074.initialization preserve=yes
  //## end ClDspRequest::ClDspRequest%963825074.initialization
{
  //## begin ClDspRequest::ClDspRequest%963825074.body preserve=yes
	m_poBufferIn = ai_poBuffer ;
	m_poBufferOut = NULL ;

	if ( ai_poLog )
	{
		m_poLog = ai_poLog ;
	}
	else
	{
		m_poLog = new ClLogSystem (GETLOGMANAGER () , M4_TRUE , M4_TRUE ) ;
		m_bOwnLog = M4_TRUE ;
	}

	m_iConnectionID = ai_iConnectionID ;
	m_iRequestID = ai_iRequestID ;
	m_iRet = M4_SUCCESS ;
  //## end ClDspRequest::ClDspRequest%963825074.body
}


ClDspRequest::~ClDspRequest ()
{
  //## begin ClDspRequest::~ClDspRequest%963989739.body preserve=yes
	if ( m_bOwnLog )
		delete m_poLog ;
  //## end ClDspRequest::~ClDspRequest%963989739.body
}



//## Other Operations (implementation)
ClLogSystem * ClDspRequest::GetLog ()
{
  //## begin ClDspRequest::GetLog%979719939.body preserve=yes
	return m_poLog ;
  //## end ClDspRequest::GetLog%979719939.body
}

// Additional Declarations
  //## begin ClDspRequest%396D796103E7.declarations preserve=yes
  //## end ClDspRequest%396D796103E7.declarations

//## begin module%396C4E0B0358.epilog preserve=yes
//## end module%396C4E0B0358.epilog
