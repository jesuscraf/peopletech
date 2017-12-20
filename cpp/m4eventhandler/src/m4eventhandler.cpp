//## begin module%37EA121F03DA.cm preserve=no
//## end module%37EA121F03DA.cm

//## begin module%37EA121F03DA.cp preserve=no
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
//## end module%37EA121F03DA.cp

//## Module: m4eventhandler%37EA121F03DA; Package body
//## Subsystem: M4EventHandler::src%37EA1203031B
//## Source file: e:\source\m4eventhandler\src\m4eventhandler.cpp

//## begin module%37EA121F03DA.additionalIncludes preserve=no
//## end module%37EA121F03DA.additionalIncludes

//## begin module%37EA121F03DA.includes preserve=yes
//## end module%37EA121F03DA.includes

// m4eventtopic
#include <m4eventtopic.hpp>
// m4eventmsg
#include <m4eventmsg.hpp>
// m4eventhandler
#include <m4eventhandler.hpp>
// m4types
#include <m4types.hpp>
//## begin module%37EA121F03DA.declarations preserve=no
//## end module%37EA121F03DA.declarations

//## begin module%37EA121F03DA.additionalDeclarations preserve=yes
//## end module%37EA121F03DA.additionalDeclarations


// Class IPublisher 

IPublisher::~IPublisher ()
{
  //## begin IPublisher::~IPublisher%938164926.body preserve=yes
  //## end IPublisher::~IPublisher%938164926.body
}



//## Other Operations (implementation)
m4return_t IPublisher::PublishAnonymousEvent (ClEventTopic ai_Topid, ClEventSubject ai_Subject)
{
  //## begin IPublisher::PublishAnonymousEvent%940924602.body preserve=yes
	return M4_ERROR;
  //## end IPublisher::PublishAnonymousEvent%940924602.body
}

// Additional Declarations
  //## begin IPublisher%37E1036401B3.declarations preserve=yes
  //## end IPublisher%37E1036401B3.declarations

// Class ISubscriber 

ISubscriber::~ISubscriber ()
{
  //## begin ISubscriber::~ISubscriber%938164928.body preserve=yes
  //## end ISubscriber::~ISubscriber%938164928.body
}


// Additional Declarations
  //## begin ISubscriber%37E10513004A.declarations preserve=yes
  //## end ISubscriber%37E10513004A.declarations

// Class IEventConsumer 

// Additional Declarations
  //## begin IEventConsumer%37E670B4007A.declarations preserve=yes
  //## end IEventConsumer%37E670B4007A.declarations

// Class IEventHandler 

//## begin IEventHandler::m_sEventHandlerInstance%37EB804A01FB.role preserve=no  protected: static IEventHandler { -> 1RHAN}
IEventHandler *IEventHandler::m_sEventHandlerInstance = NULL;
//## end IEventHandler::m_sEventHandlerInstance%37EB804A01FB.role


IEventHandler::IEventHandler ()
  //## begin IEventHandler::IEventHandler%938087500.hasinit preserve=no
  //## end IEventHandler::IEventHandler%938087500.hasinit
  //## begin IEventHandler::IEventHandler%938087500.initialization preserve=yes
  //## end IEventHandler::IEventHandler%938087500.initialization
{
  //## begin IEventHandler::IEventHandler%938087500.body preserve=yes
  //## end IEventHandler::IEventHandler%938087500.body
}


IEventHandler::~IEventHandler ()
{
  //## begin IEventHandler::~IEventHandler%938087501.body preserve=yes
  //## end IEventHandler::~IEventHandler%938087501.body
}



//## Other Operations (implementation)
IEventHandler * IEventHandler::GetEHInstance ()
{
  //## begin IEventHandler::GetEHInstance%938087502.body preserve=yes
	return m_sEventHandlerInstance;
  //## end IEventHandler::GetEHInstance%938087502.body
}

// Additional Declarations
  //## begin IEventHandler%37E671BA0063.declarations preserve=yes
  //## end IEventHandler%37E671BA0063.declarations

// Class IEventProducer 

// Additional Declarations
  //## begin IEventProducer%37E73CB4032D.declarations preserve=yes
  //## end IEventProducer%37E73CB4032D.declarations

//## begin module%37EA121F03DA.epilog preserve=yes
//## end module%37EA121F03DA.epilog
