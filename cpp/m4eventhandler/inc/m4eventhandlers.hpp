//## begin module%37EF83EC00DA.cm preserve=no
//## end module%37EF83EC00DA.cm

//## begin module%37EF83EC00DA.cp preserve=no
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
//## end module%37EF83EC00DA.cp

//## Module: m4eventhandlers%37EF83EC00DA; Package specification
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Source file: e:\source\m4eventhandler\inc\m4eventhandlers.hpp

#ifndef m4eventhandlers_h
#define m4eventhandlers_h 1

//## begin module%37EF83EC00DA.additionalIncludes preserve=no
//## end module%37EF83EC00DA.additionalIncludes

//## begin module%37EF83EC00DA.includes preserve=yes
//## end module%37EF83EC00DA.includes

// m4eventhandler
#include <m4eventhandler.hpp>
#include <m4eventhandler_dll.hpp>
// m4types
#include <m4types.hpp>
// stl
#include "m4stl.hpp"
// syncro
#include <syncro.hpp>

class ClLogSystem;

//## begin module%37EF83EC00DA.declarations preserve=no
//## end module%37EF83EC00DA.declarations

//## begin module%37EF83EC00DA.additionalDeclarations preserve=yes

M4_DECL_M4EVENTHANDLER m4return_t M4SubscribeTopic(m4char_t *ai_szTopic  );
M4_DECL_M4EVENTHANDLER m4return_t M4UnsubscribeTopic(m4char_t *ai_szTopic);
M4_DECL_M4EVENTHANDLER m4handle_t M4GetEventHandle(m4char_t *ai_szTopic);
M4_DECL_M4EVENTHANDLER m4return_t M4CloseEventHandle(m4handle_t ai_hEventHandle) ;
M4_DECL_M4EVENTHANDLER m4return_t M4GetEventSubject(m4handle_t ai_hEventHandle , char *  ao_pszSubject ,  long ai_lSize , long & ao_lSubject );
M4_DECL_M4EVENTHANDLER m4return_t M4PublishEvent(m4pchar_t ai_szTopic,m4pchar_t  ai_szSubject );

//## end module%37EF83EC00DA.additionalDeclarations


//## begin SubscriptorMap%37EFA3A101E0.preface preserve=yes
//## end SubscriptorMap%37EFA3A101E0.preface

//## Class: SubscriptorMap%37EFA3A101E0; Instantiated Class
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EFA4CB00A8;m4handle_t { -> }
//## Uses: <unnamed>%37EFA4CD00D3;m4int32_t { -> }

typedef map< m4handle_t,m4int32_t,less <m4handle_t > > SubscriptorMap;

//## begin SubscriptorMap%37EFA3A101E0.postscript preserve=yes
//## end SubscriptorMap%37EFA3A101E0.postscript

//## begin ClLogBasedEventHandler%37EF859903BC.preface preserve=yes
//## end ClLogBasedEventHandler%37EF859903BC.preface

//## Class: ClLogBasedEventHandler%37EF859903BC
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4EVENTHANDLER ClLogBasedEventHandler : public IEventHandler  //## Inherits: <unnamed>%37EF85B60043
{
  //## begin ClLogBasedEventHandler%37EF859903BC.initialDeclarations preserve=yes
  //## end ClLogBasedEventHandler%37EF859903BC.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClLogBasedEventHandler%938424978
      ~ClLogBasedEventHandler ();


    //## Other Operations (specified)
      //## Operation: SubscribeTopic%938424979; C++
      //	This operation subscribe a client to a number of events related to a topic
      //	under some advanced search criteria.
      m4return_t SubscribeTopic (const IEventConsumer &ai_oEventClient, const ClEventTopic &ai_oEventTopic, m4bool_t ai_bHistory = M4_FALSE);

      //## Operation: UnsubscribeTopic%938424980; C++
      //	This operation subscribe a client to a number of events related to a topic
      //	under some advanced search criteria.
      m4return_t UnsubscribeTopic (const IEventConsumer &ai_oEventClient, const ClEventTopic &ai_oEventTopic);

      //## Operation: GetEventMsg%938424981; C++
      //	This operations allows a client to retrieve an event from the handler. An
      //	event topic must be provided to specify a particular kind of event.
      m4return_t GetEventMsg (const IEventConsumer &ai_oEventClient, ClEventMsg &ao_oEventMsg, const ClEventTopic &ai_oEventTopic);

      //## Operation: PublishEventMsg%938424982; C++
      //	This operation inserts a new event message into an Event Handler
      m4return_t PublishEventMsg (const IEventProducer &ai_oEventClient, const ClEventMsg &ai_oEventMsg);

      //## Operation: GetEHInstance%938507460
      static IEventHandler * GetEHInstance ();

      //## Operation: GetEventMsg%938507461; C++
      //	This operations allows a client to retrieve an event from the handler. An
      //	event topic must be provided to specify a particular kind of event.
      m4return_t GetEventMsg (const IEventConsumer &ai_oEventClient, ClEventMsgHistory &ao_oEventMsgHistory, const ClEventTopic &ai_oEventTopic);

      //## Operation: PublishEventMsg%938507462; C++
      //	This operation inserts a collection of event messages into an Event Handler
      m4return_t PublishEventMsg (const IEventProducer &ai_oEventClient, const ClEventMsgHistory &ai_oEventHistory);

      //## Operation: Deserialize%939113832; C++
      m4return_t Deserialize (m4pcchar_t ai_pcBuffer, m4int_t ai_iBufferSize);

      //## Operation: PublishAnonymousEvent%940586603; C++
      static m4return_t PublishAnonymousEvent (ClEventTopic ai_Topid, ClEventSubject ai_Subject);

      //## Operation: GetEventMsg%943015143; C++
      //	This operations allows a client to retrieve an event from the handler. An
      //	event topic must be provided to specify a particular kind of event.
      m4return_t GetEventMsg (const IEventConsumer &ai_oEventClient, m4pchar_t ao_pBuffer, m4int_t &ai_oBufferSize, const ClEventTopic &ai_oEventTopic);

    // Additional Public Declarations
      //## begin ClLogBasedEventHandler%37EF859903BC.public preserve=yes
      //## end ClLogBasedEventHandler%37EF859903BC.public

  protected:
    //## Constructors (specified)
      //## Operation: ClLogBasedEventHandler%938424977
      ClLogBasedEventHandler ();

    // Data Members for Associations

      //## Association: M4EventHandler::<unnamed>%37EF84C200A6
      //## Role: ClLogBasedEventHandler::m_poLogSystem%37EF84C20287
      //## begin ClLogBasedEventHandler::m_poLogSystem%37EF84C20287.role preserve=no  protected: static ClLogSystem {1 -> 1RFHAN}
      static ClLogSystem *m_poLogSystem;
      //## end ClLogBasedEventHandler::m_poLogSystem%37EF84C20287.role

      //## Association: M4EventHandler::<unnamed>%37EFA3C4000A
      //## Role: ClLogBasedEventHandler::m_pSubscriptorMap%37EFA3C4028B
      //## begin ClLogBasedEventHandler::m_pSubscriptorMap%37EFA3C4028B.role preserve=no  protected: SubscriptorMap { -> UHAN}
      SubscriptorMap m_pSubscriptorMap;
      //## end ClLogBasedEventHandler::m_pSubscriptorMap%37EFA3C4028B.role

      //## Association: M4EventHandler::<unnamed>%37F07DDE00A7
      //## Role: ClLogBasedEventHandler::m_oMutex%37F07DDE035A
      //## begin ClLogBasedEventHandler::m_oMutex%37F07DDE035A.role preserve=no  protected: static ClMutex { -> UHAN}
      static ClMutex m_oMutex;
      //## end ClLogBasedEventHandler::m_oMutex%37F07DDE035A.role

      //## Association: M4EventHandler::<unnamed>%37F0C5920119
      //## Role: ClLogBasedEventHandler::m_EventCounter%37F0C5930053
      //## begin ClLogBasedEventHandler::m_EventCounter%37F0C5930053.role preserve=no  protected: static m4uint32_t {1 -> 1UHAN}
      static m4uint32_t m_EventCounter;
      //## end ClLogBasedEventHandler::m_EventCounter%37F0C5930053.role

    // Additional Protected Declarations
      //## begin ClLogBasedEventHandler%37EF859903BC.protected preserve=yes
      //## end ClLogBasedEventHandler%37EF859903BC.protected

  private:
    // Additional Private Declarations
      //## begin ClLogBasedEventHandler%37EF859903BC.private preserve=yes
      //## end ClLogBasedEventHandler%37EF859903BC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLogBasedEventHandler%37EF859903BC.implementation preserve=yes
      //## end ClLogBasedEventHandler%37EF859903BC.implementation

};

//## begin ClLogBasedEventHandler%37EF859903BC.postscript preserve=yes
//## end ClLogBasedEventHandler%37EF859903BC.postscript

//## begin ClServerEventHandler%37EF83480107.preface preserve=yes
//## end ClServerEventHandler%37EF83480107.preface

//## Class: ClServerEventHandler%37EF83480107
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4EVENTHANDLER ClServerEventHandler : public ClLogBasedEventHandler  //## Inherits: <unnamed>%37EF83680149
{
  //## begin ClServerEventHandler%37EF83480107.initialDeclarations preserve=yes
  //## end ClServerEventHandler%37EF83480107.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClServerEventHandler%938424975
      ClServerEventHandler ();

    //## Destructor (specified)
      //## Operation: ~ClServerEventHandler%938424976
      ~ClServerEventHandler ();

    // Additional Public Declarations
      //## begin ClServerEventHandler%37EF83480107.public preserve=yes
      //## end ClServerEventHandler%37EF83480107.public

  protected:
    // Additional Protected Declarations
      //## begin ClServerEventHandler%37EF83480107.protected preserve=yes
      //## end ClServerEventHandler%37EF83480107.protected

  private:
    // Additional Private Declarations
      //## begin ClServerEventHandler%37EF83480107.private preserve=yes
      //## end ClServerEventHandler%37EF83480107.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServerEventHandler%37EF83480107.implementation preserve=yes
      //## end ClServerEventHandler%37EF83480107.implementation

};

//## begin ClServerEventHandler%37EF83480107.postscript preserve=yes
//## end ClServerEventHandler%37EF83480107.postscript

//## begin ClClientEventHandler%37EF837002B3.preface preserve=yes
//## end ClClientEventHandler%37EF837002B3.preface

//## Class: ClClientEventHandler%37EF837002B3
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4EVENTHANDLER ClClientEventHandler : public ClLogBasedEventHandler  //## Inherits: <unnamed>%37EF837E0353
{
  //## begin ClClientEventHandler%37EF837002B3.initialDeclarations preserve=yes
  //## end ClClientEventHandler%37EF837002B3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClClientEventHandler%938424973
      ClClientEventHandler ();

    //## Destructor (specified)
      //## Operation: ~ClClientEventHandler%938424974
      ~ClClientEventHandler ();

    // Additional Public Declarations
      //## begin ClClientEventHandler%37EF837002B3.public preserve=yes
      //## end ClClientEventHandler%37EF837002B3.public

  protected:
    // Additional Protected Declarations
      //## begin ClClientEventHandler%37EF837002B3.protected preserve=yes
      //## end ClClientEventHandler%37EF837002B3.protected

  private:
    // Additional Private Declarations
      //## begin ClClientEventHandler%37EF837002B3.private preserve=yes
      //## end ClClientEventHandler%37EF837002B3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClClientEventHandler%37EF837002B3.implementation preserve=yes
      //## end ClClientEventHandler%37EF837002B3.implementation

};

//## begin ClClientEventHandler%37EF837002B3.postscript preserve=yes
//## end ClClientEventHandler%37EF837002B3.postscript

// Class ClLogBasedEventHandler 

// Class ClServerEventHandler 

// Class ClClientEventHandler 

//## begin module%37EF83EC00DA.epilog preserve=yes
//## end module%37EF83EC00DA.epilog


#endif
