//## begin module%37EA121D000C.cm preserve=no
//## end module%37EA121D000C.cm

//## begin module%37EA121D000C.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%37EA121D000C.cp

//## Module: m4eventhandler%37EA121D000C; Package specification
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Source file: e:\source\m4eventhandler\inc\m4eventhandler.hpp

#ifndef m4eventhandler_h
#define m4eventhandler_h 1

//## begin module%37EA121D000C.additionalIncludes preserve=no
//## end module%37EA121D000C.additionalIncludes

//## begin module%37EA121D000C.includes preserve=yes
#include <m4eventhandler_dll.hpp>
//## end module%37EA121D000C.includes

// m4eventtopic
#include <m4eventtopic.hpp>
// m4eventmsg
#include <m4eventmsg.hpp>
// m4types
#include <m4types.hpp>
//## begin module%37EA121D000C.declarations preserve=no
//## end module%37EA121D000C.declarations

//## begin module%37EA121D000C.additionalDeclarations preserve=yes
//## end module%37EA121D000C.additionalDeclarations


//## begin IEventConsumer%37E670B4007A.preface preserve=yes
//## end IEventConsumer%37E670B4007A.preface

//## Class: IEventConsumer%37E670B4007A; Abstract
//	This interface allows a derived class access to the m4 event handling
//	subscription mechanism.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB43140045;m4return_t { -> }
//## Uses: <unnamed>%37EB56090061;m4handle_t { -> }

class M4_DECL_M4EVENTHANDLER IEventConsumer 
{
  //## begin IEventConsumer%37E670B4007A.initialDeclarations preserve=yes
  //## end IEventConsumer%37E670B4007A.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetConsumerID%938087495
      virtual m4return_t GetConsumerID (m4handle_t &ao_hConsumerID) = 0;

    // Additional Public Declarations
      //## begin IEventConsumer%37E670B4007A.public preserve=yes
      //## end IEventConsumer%37E670B4007A.public

  protected:
    // Additional Protected Declarations
      //## begin IEventConsumer%37E670B4007A.protected preserve=yes
      //## end IEventConsumer%37E670B4007A.protected

  private:
    // Additional Private Declarations
      //## begin IEventConsumer%37E670B4007A.private preserve=yes
      //## end IEventConsumer%37E670B4007A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IEventConsumer%37E670B4007A.implementation preserve=yes
      //## end IEventConsumer%37E670B4007A.implementation

};

//## begin IEventConsumer%37E670B4007A.postscript preserve=yes
//## end IEventConsumer%37E670B4007A.postscript

//## begin ISubscriber%37E10513004A.preface preserve=yes
//## end ISubscriber%37E10513004A.preface

//## Class: ISubscriber%37E10513004A; Abstract
//	This interface provide a minimal API for event subscribing through the
//	M4EventHandling mechanism.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E6751A0250;ClEventTopic { -> }
//## Uses: <unnamed>%37E8B41D03BA;ClEventMsgHistory { -> }
//## Uses: <unnamed>%37EA0F7B03D8;IEventConsumer { -> }
//## Uses: <unnamed>%37EB42FC03BC;IEventConsumer { -> }
//## Uses: <unnamed>%37EB430801B0;m4bool_t { -> }
//## Uses: <unnamed>%37EB4316017E;m4return_t { -> }
//## Uses: <unnamed>%37EB5644011A;ClEventMsg { -> }

class M4_DECL_M4EVENTHANDLER ISubscriber
{
  //## begin ISubscriber%37E10513004A.initialDeclarations preserve=yes
  //## end ISubscriber%37E10513004A.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ISubscriber%938164928
      virtual ~ISubscriber ();


    //## Other Operations (specified)
      //## Operation: SubscribeTopic%938087496; C++
      //	This operation subscribe a client to a number of events related to a topic
      //	under some advanced search criteria.
      virtual m4return_t SubscribeTopic (const IEventConsumer &ai_oEventClient, const ClEventTopic &ai_oEventTopic, m4bool_t ai_bHistory = M4_FALSE) = 0;

      //## Operation: GetEventMsg%937504397; C++
      //	This operations allows a client to retrieve an event from the handler. An
      //	event topic must be provided to specify a particular kind of event.
      virtual m4return_t GetEventMsg (const IEventConsumer &ai_oEventClient, ClEventMsg &ao_oEventMsg, const ClEventTopic &ai_oEventTopic) = 0;

      //## Operation: UnsubscribeTopic%938424970; C++
      //	This operation subscribe a client to a number of events related to a topic
      //	under some advanced search criteria.
      virtual m4return_t UnsubscribeTopic (const IEventConsumer &ai_oEventClient, const ClEventTopic &ai_oEventTopic) = 0;

      //## Operation: GetEventMsg%938424971; C++
      //	This operations allows a client to retrieve an event from the handler. An
      //	event topic must be provided to specify a particular kind of event.
      virtual m4return_t GetEventMsg (const IEventConsumer &ai_oEventClient, ClEventMsgHistory &ao_oEventMsgHistory, const ClEventTopic &ai_oEventTopic) = 0;

      //## Operation: GetEventMsg%943015144; C++
      //	This operations allows a client to retrieve an event from the handler. An
      //	event topic must be provided to specify a particular kind of event.
      virtual m4return_t GetEventMsg (const IEventConsumer &ai_oEventClient, m4pchar_t ao_pBuffer, m4int_t &ai_oBufferSize, const ClEventTopic &ai_oEventTopic) = 0;

    // Additional Public Declarations
      //## begin ISubscriber%37E10513004A.public preserve=yes
      //## end ISubscriber%37E10513004A.public

  protected:
    // Additional Protected Declarations
      //## begin ISubscriber%37E10513004A.protected preserve=yes
      //## end ISubscriber%37E10513004A.protected

  private:
    // Additional Private Declarations
      //## begin ISubscriber%37E10513004A.private preserve=yes
      //## end ISubscriber%37E10513004A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ISubscriber%37E10513004A.implementation preserve=yes
      //## end ISubscriber%37E10513004A.implementation

};

//## begin ISubscriber%37E10513004A.postscript preserve=yes
//## end ISubscriber%37E10513004A.postscript

//## begin IEventProducer%37E73CB4032D.preface preserve=yes
//## end IEventProducer%37E73CB4032D.preface

//## Class: IEventProducer%37E73CB4032D; Abstract
//	This interface allows a derived class access to the m4 event handling
//	publication mechanism.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB431C02DB;m4return_t { -> }
//## Uses: <unnamed>%37EB560B01D7;m4handle_t { -> }

class M4_DECL_M4EVENTHANDLER IEventProducer 
{
  //## begin IEventProducer%37E73CB4032D.initialDeclarations preserve=yes
  //## end IEventProducer%37E73CB4032D.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetProducerID%938087498; C++
      virtual m4return_t GetProducerID (m4handle_t &ao_hProducerID) = 0;

    // Additional Public Declarations
      //## begin IEventProducer%37E73CB4032D.public preserve=yes
      //## end IEventProducer%37E73CB4032D.public

  protected:
    // Additional Protected Declarations
      //## begin IEventProducer%37E73CB4032D.protected preserve=yes
      //## end IEventProducer%37E73CB4032D.protected

  private:
    // Additional Private Declarations
      //## begin IEventProducer%37E73CB4032D.private preserve=yes
      //## end IEventProducer%37E73CB4032D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IEventProducer%37E73CB4032D.implementation preserve=yes
      //## end IEventProducer%37E73CB4032D.implementation

};

//## begin IEventProducer%37E73CB4032D.postscript preserve=yes
//## end IEventProducer%37E73CB4032D.postscript

//## begin IPublisher%37E1036401B3.preface preserve=yes
//## end IPublisher%37E1036401B3.preface

//## Class: IPublisher%37E1036401B3; Abstract
//	This interface provide a minimal API for event publishing through the M4Event
//	Handling mechanism.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E73CCA013A;IEventProducer { -> }
//## Uses: <unnamed>%37EB43190309;m4return_t { -> }
//## Uses: <unnamed>%37E8B3760099;ClEventMsgHistory { -> }

class M4_DECL_M4EVENTHANDLER IPublisher
{
  //## begin IPublisher%37E1036401B3.initialDeclarations preserve=yes
  //## end IPublisher%37E1036401B3.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~IPublisher%938164926
      virtual ~IPublisher ();


    //## Other Operations (specified)
      //## Operation: PublishEventMsg%937504390; C++
      //	This operation inserts a new event message into an Event Handler
      virtual m4return_t PublishEventMsg (const IEventProducer &ai_oEventClient, const ClEventMsg &ai_oEventMsg) = 0;

      //## Operation: PublishEventMsg%938087499; C++
      //	This operation inserts a collection of event messages into an Event Handler
      virtual m4return_t PublishEventMsg (const IEventProducer &ai_oEventClient, const ClEventMsgHistory &ai_oEventHistory) = 0;

      //## Operation: PublishAnonymousEvent%940924602; C++
      static m4return_t PublishAnonymousEvent (ClEventTopic ai_Topid, ClEventSubject ai_Subject);

    // Additional Public Declarations
      //## begin IPublisher%37E1036401B3.public preserve=yes
      //## end IPublisher%37E1036401B3.public

  protected:
    // Additional Protected Declarations
      //## begin IPublisher%37E1036401B3.protected preserve=yes
      //## end IPublisher%37E1036401B3.protected

  private:
    // Additional Private Declarations
      //## begin IPublisher%37E1036401B3.private preserve=yes
      //## end IPublisher%37E1036401B3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IPublisher%37E1036401B3.implementation preserve=yes
      //## end IPublisher%37E1036401B3.implementation

};

//## begin IPublisher%37E1036401B3.postscript preserve=yes
//## end IPublisher%37E1036401B3.postscript

//## begin IEventHandler%37E671BA0063.preface preserve=yes
//## end IEventHandler%37E671BA0063.preface

//## Class: IEventHandler%37E671BA0063; Abstract
//	This interface comprises both the publisher and subscriber role in the meta4
//	event Handling mechanism.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB432003D1;m4return_t { -> }

class M4_DECL_M4EVENTHANDLER IEventHandler : public IPublisher, //## Inherits: <unnamed>%37E671E80087
    	public ISubscriber  //## Inherits: <unnamed>%37E671EA00C6
{
  //## begin IEventHandler%37E671BA0063.initialDeclarations preserve=yes
  //## end IEventHandler%37E671BA0063.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~IEventHandler%938087501
      ~IEventHandler ();


    //## Other Operations (specified)
      //## Operation: GetEHInstance%938087502
      static IEventHandler * GetEHInstance ();

    // Additional Public Declarations
      //## begin IEventHandler%37E671BA0063.public preserve=yes
      //## end IEventHandler%37E671BA0063.public

  protected:
    //## Constructors (specified)
      //## Operation: IEventHandler%938087500
      IEventHandler ();

    // Data Members for Associations

      //## Association: M4EventHandler::<unnamed>%37EB80490073
      //## Role: IEventHandler::m_sEventHandlerInstance%37EB804A01FB
      //## begin IEventHandler::m_sEventHandlerInstance%37EB804A01FB.role preserve=no  protected: static IEventHandler { -> 1RHAN}
      static IEventHandler *m_sEventHandlerInstance;
      //## end IEventHandler::m_sEventHandlerInstance%37EB804A01FB.role

    // Additional Protected Declarations
      //## begin IEventHandler%37E671BA0063.protected preserve=yes
      //## end IEventHandler%37E671BA0063.protected

  private:
    // Additional Private Declarations
      //## begin IEventHandler%37E671BA0063.private preserve=yes
      //## end IEventHandler%37E671BA0063.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IEventHandler%37E671BA0063.implementation preserve=yes
      //## end IEventHandler%37E671BA0063.implementation

};

//## begin IEventHandler%37E671BA0063.postscript preserve=yes
//## end IEventHandler%37E671BA0063.postscript

// Class IEventConsumer 

// Class ISubscriber 

// Class IEventProducer 

// Class IPublisher 

// Class IEventHandler 

//## begin module%37EA121D000C.epilog preserve=yes
//## end module%37EA121D000C.epilog


#endif
