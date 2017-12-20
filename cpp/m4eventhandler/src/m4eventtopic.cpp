//## begin module%37EA13700398.cm preserve=no
//## end module%37EA13700398.cm

//## begin module%37EA13700398.cp preserve=no
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
//## end module%37EA13700398.cp

//## Module: m4eventtopic%37EA13700398; Package body
//## Subsystem: M4EventHandler::src%37EA1203031B
//## Source file: e:\source\m4eventhandler\version\src\m4eventtopic.cpp

//## begin module%37EA13700398.additionalIncludes preserve=no
//## end module%37EA13700398.additionalIncludes

//## begin module%37EA13700398.includes preserve=yes
#include <m4trace.hpp>
//## end module%37EA13700398.includes

// m4eventtopic
#include <m4eventtopic.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
//## begin module%37EA13700398.declarations preserve=no
//## end module%37EA13700398.declarations

//## begin module%37EA13700398.additionalDeclarations preserve=yes
//## end module%37EA13700398.additionalDeclarations


// Class ClEventTopic 



ClEventTopic::ClEventTopic (const ClEventTopicID &ai_strTopic)
  //## begin ClEventTopic::ClEventTopic%938164933.hasinit preserve=no
  //## end ClEventTopic::ClEventTopic%938164933.hasinit
  //## begin ClEventTopic::ClEventTopic%938164933.initialization preserve=yes
  //## end ClEventTopic::ClEventTopic%938164933.initialization
{
  //## begin ClEventTopic::ClEventTopic%938164933.body preserve=yes
	if(ai_strTopic.empty())
	{ m4Trace(cerr<<"Empty topic."<<endl);}
	else
		m_pClEventTopicID=ai_strTopic;
  //## end ClEventTopic::ClEventTopic%938164933.body
}

ClEventTopic::ClEventTopic (m4pcchar_t ai_pccTopic)
  //## begin ClEventTopic::ClEventTopic%938164943.hasinit preserve=no
  //## end ClEventTopic::ClEventTopic%938164943.hasinit
  //## begin ClEventTopic::ClEventTopic%938164943.initialization preserve=yes
  //## end ClEventTopic::ClEventTopic%938164943.initialization
{
  //## begin ClEventTopic::ClEventTopic%938164943.body preserve=yes
	m_pClEventTopicID=ai_pccTopic;
  //## end ClEventTopic::ClEventTopic%938164943.body
}


ClEventTopic::~ClEventTopic ()
{
  //## begin ClEventTopic::~ClEventTopic%938164934.body preserve=yes
  //## end ClEventTopic::~ClEventTopic%938164934.body
}



//## Other Operations (implementation)
m4return_t ClEventTopic::GetTopicID (ClEventTopicID &ao_oTopicID)
{
  //## begin ClEventTopic::GetTopicID%937504393.body preserve=yes
	m4return_t iRet=M4_ERROR;;
	if(m_pClEventTopicID.empty()) 
	{
		m4Trace(cerr<<"Empty Topic."<<endl);
	}
	else
	{
		ao_oTopicID=m_pClEventTopicID;
		iRet=M4_SUCCESS;
	}
	return iRet;
  //## end ClEventTopic::GetTopicID%937504393.body
}

ClEventTopic  & ClEventTopic::operator = (const ClEventTopic &ai_oEventTopic)
{
  //## begin ClEventTopic::operator=%938591993.body preserve=yes
	m_pClEventTopicID=ai_oEventTopic.m_pClEventTopicID;
	return *this;
  //## end ClEventTopic::operator=%938591993.body
}

// Additional Declarations
  //## begin ClEventTopic%37E12DB203C9.declarations preserve=yes
  //## end ClEventTopic%37E12DB203C9.declarations

//## begin module%37EA13700398.epilog preserve=yes
//## end module%37EA13700398.epilog
