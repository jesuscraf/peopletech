//## begin module%3AD329F700A6.cm preserve=no
//## end module%3AD329F700A6.cm

//## begin module%3AD329F700A6.cp preserve=no
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
//## end module%3AD329F700A6.cp

//## Module: msgcontainers%3AD329F700A6; Package body
//## Subsystem: M4Request::src%379439BF01B7
//## Source file: F:\integration\m4request\src\msgcontainers.cpp

//## begin module%3AD329F700A6.additionalIncludes preserve=no
//## end module%3AD329F700A6.additionalIncludes

//## begin module%3AD329F700A6.includes preserve=yes
//## end module%3AD329F700A6.includes

// ImsgContainer
#include <imsgcontainer.hpp>
// msgcontainers
#include <msgcontainers.hpp>
//## begin module%3AD329F700A6.declarations preserve=no
//## end module%3AD329F700A6.declarations

//## begin module%3AD329F700A6.additionalDeclarations preserve=yes
//## end module%3AD329F700A6.additionalDeclarations


// Class ClSingleMsgContainer 



//## Other Operations (implementation)
void ClSingleMsgContainer::StoreTraceMessage (m4string_t ai_strMessage)
{
  //## begin ClSingleMsgContainer::StoreTraceMessage%986914181.body preserve=yes
	m_strMessage += ai_strMessage ;
  //## end ClSingleMsgContainer::StoreTraceMessage%986914181.body
}

m4return_t ClSingleMsgContainer::GetTraceMessage (m4string_t &ao_strMessage)
{
  //## begin ClSingleMsgContainer::GetTraceMessage%986975096.body preserve=yes
	ao_strMessage = m_strMessage ;

	return M4_SUCCESS ;
  //## end ClSingleMsgContainer::GetTraceMessage%986975096.body
}

// Additional Declarations
  //## begin ClSingleMsgContainer%3AD328C901E2.declarations preserve=yes
  //## end ClSingleMsgContainer%3AD328C901E2.declarations

//## begin module%3AD329F700A6.epilog preserve=yes
//## end module%3AD329F700A6.epilog
