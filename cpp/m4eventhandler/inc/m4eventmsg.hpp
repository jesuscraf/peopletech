//## begin module%37EA131B00CF.cm preserve=no
//## end module%37EA131B00CF.cm

//## begin module%37EA131B00CF.cp preserve=no
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
//## end module%37EA131B00CF.cp

//## Module: m4eventmsg%37EA131B00CF; Package specification
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Source file: e:\source\m4eventhandler\version\inc\m4eventmsg.hpp

#ifndef m4eventmsg_h
#define m4eventmsg_h 1

//## begin module%37EA131B00CF.additionalIncludes preserve=no
//## end module%37EA131B00CF.additionalIncludes

//## begin module%37EA131B00CF.includes preserve=yes
#include <m4eventhandler_dll.hpp>
#include <m4eventhandler_dll.hpp>

//## end module%37EA131B00CF.includes

// m4eventtopic
#include <m4eventtopic.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// stl
#include "m4stl.hpp"
//## begin module%37EA131B00CF.declarations preserve=no
//## end module%37EA131B00CF.declarations

//## begin module%37EA131B00CF.additionalDeclarations preserve=yes

#define M4_EVENT_TOPIC_TOKEN "Topic="
#define M4_EVENT_SUBJECT_TOKEN "Subject="
#define M4_EVENT_TOPIC_TOKEN_SEPARATOR '='
#define M4_EVENT_SUBJECT_TOKEN_SEPARATOR '='

#define M4_EVENT_FIELD_SEPARATOR " "

#define M4_DEFAULT_REASON "Unkown"
#define M4_DEFAULT_SOLUTION "Unkown"
#define M4_DEFAULT_SEVERITY "Low"
/*
-	Tipo del SS que lo lanza
-	ID del SS
-	Fecha/hora (out, pero no in)
-	Descripción (este será el subject inicialmente)
-	Razón
-	Posible solución (se supone que son problemas)
-	Grado de severidad ? (definir valores) (default severity, )
*/
class ClEventMsg;

M4_DECL_M4EVENTHANDLER m4return_t  CreateEventMessage(	ClEventMsg &ai_oMsg,
								const M4ClString & ai_strEventProducer,
								const M4ClString & ai_strProducerID,
								const M4ClString & ai_strSubject,
								const M4ClString & ai_strReason,
								const M4ClString & ai_strSolution,
								const M4ClString & ai_strSeverity
								); 

M4_DECL_M4EVENTHANDLER m4return_t GetDataFromEventMessage(	ClEventMsg &ai_oMsg,
								M4ClString & ai_strEventProducer,
								M4ClString & ai_strProducerID,
								M4ClString & ai_strSubject,
								M4ClString & ai_strCause,
								M4ClString & ai_strSolution,
								M4ClString & ai_strSeverity
								);

//## end module%37EA131B00CF.additionalDeclarations


//## begin ClEventSubject%37E73F2C019B.preface preserve=yes
//## end ClEventSubject%37E73F2C019B.preface

//## Class: ClEventSubject%37E73F2C019B
//	This interface is string-like. It is not shown for being straightforward.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB6B0102AF;M4ClString { -> }

typedef M4ClString ClEventSubject;

//## begin ClEventSubject%37E73F2C019B.postscript preserve=yes
//## end ClEventSubject%37E73F2C019B.postscript

//## begin ClEventHeader%37E73DB00285.preface preserve=yes
//## end ClEventHeader%37E73DB00285.preface

//## Class: ClEventHeader%37E73DB00285
//	This interface identifies an event with an ID (Topic), a human readable
//	description (Subject) and a priority.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB43500236;m4return_t { -> }

class M4_DECL_M4EVENTHANDLER ClEventHeader 
{
  //## begin ClEventHeader%37E73DB00285.initialDeclarations preserve=yes
  //## end ClEventHeader%37E73DB00285.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEventHeader%938164935; C++
      ClEventHeader (const M4ClString &ai_strTopic);

      //## Operation: ClEventHeader%938519904; C++
      ClEventHeader ();

    //## Destructor (specified)
      //## Operation: ~ClEventHeader%938164936
      ~ClEventHeader ();


    //## Other Operations (specified)
      //## Operation: GetTopic%938087490; C++
      //	This operation retrieves the topic from a event message header.
      m4return_t GetTopic (ClEventTopic &ao_oTopic);

      //## Operation: GetSubject%938087491; C++
      //	This operation retrieves the subject from a event message header.
      m4return_t GetSubject (ClEventSubject &ao_oSubject);

      //## Operation: SetSubject%938087493; C++
      //	This operation sets the subject from a event message header.
      m4return_t SetSubject (const ClEventSubject &ai_oSubject);

      //## Operation: operator=%938424972; C++
      ClEventHeader & operator = (const ClEventHeader &ai_oEventHeader);

    // Additional Public Declarations
      //## begin ClEventHeader%37E73DB00285.public preserve=yes
      //## end ClEventHeader%37E73DB00285.public

  protected:
    // Additional Protected Declarations
      //## begin ClEventHeader%37E73DB00285.protected preserve=yes
      //## end ClEventHeader%37E73DB00285.protected

  private:
    // Data Members for Associations

      //## Association: M4EventHandler::<unnamed>%37EA101F0301
      //## Role: ClEventHeader::<m_pClEventSubject>%37EA102002DB
      //## begin ClEventHeader::<m_pClEventSubject>%37EA102002DB.role preserve=no  private: ClEventSubject { -> VHAN}
      ClEventSubject m_pClEventSubject;
      //## end ClEventHeader::<m_pClEventSubject>%37EA102002DB.role

      //## Association: M4EventHandler::<unnamed>%37EA10740014
      //## Role: ClEventHeader::<m_pClEventTopic>%37EA107401FF
      //## begin ClEventHeader::<m_pClEventTopic>%37EA107401FF.role preserve=no  private: ClEventTopic { -> VHAN}
      ClEventTopic m_pClEventTopic;
      //## end ClEventHeader::<m_pClEventTopic>%37EA107401FF.role

    // Additional Private Declarations
      //## begin ClEventHeader%37E73DB00285.private preserve=yes
      //## end ClEventHeader%37E73DB00285.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventHeader%37E73DB00285.implementation preserve=yes
      //## end ClEventHeader%37E73DB00285.implementation

};

//## begin ClEventHeader%37E73DB00285.postscript preserve=yes
//## end ClEventHeader%37E73DB00285.postscript

//## begin ClEventMsg%37E10FC403A3.preface preserve=yes
//## end ClEventMsg%37E10FC403A3.preface

//## Class: ClEventMsg%37E10FC403A3
//	This interface provides public access to a simple event message consisting
//	of an event header. In later implementations this interface should be
//	extended with additional fields.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB434D0304;m4return_t { -> }

class M4_DECL_M4EVENTHANDLER ClEventMsg 
{
  //## begin ClEventMsg%37E10FC403A3.initialDeclarations preserve=yes
  //## end ClEventMsg%37E10FC403A3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEventMsg%938164937; C++
      ClEventMsg (const M4ClString &ai_strTopic);

      //## Operation: ClEventMsg%939283917; C++
      ClEventMsg (m4pchar_t ai_pcBuffer);

    //## Destructor (specified)
      //## Operation: ~ClEventMsg%938164938
      ~ClEventMsg ();


    //## Other Operations (specified)
      //## Operation: GetHeader%938087482; C++
      m4return_t GetHeader (ClEventHeader &ao_oEventHeader);

      //## Operation: SetHeader%938591992; C++
      m4return_t SetHeader (ClEventHeader &ai_oEventHeader);

      //## Operation: Serialize%939206937; C++
      m4return_t Serialize (m4pcchar_t ao_pcBuffer, m4int_t ai_iBufferSize, m4int_t &ao_iOutputSize);

    // Additional Public Declarations
      //## begin ClEventMsg%37E10FC403A3.public preserve=yes
      //## end ClEventMsg%37E10FC403A3.public

  protected:
    // Data Members for Associations

      //## Association: M4EventHandler::<unnamed>%37EA3CA8017F
      //## Role: ClEventMsg::<m_pClEventHeader>%37EA3CA902CA
      //## begin ClEventMsg::<m_pClEventHeader>%37EA3CA902CA.role preserve=no  protected: ClEventHeader { -> VHAN}
      ClEventHeader m_pClEventHeader;
      //## end ClEventMsg::<m_pClEventHeader>%37EA3CA902CA.role

    // Additional Protected Declarations
      //## begin ClEventMsg%37E10FC403A3.protected preserve=yes
      //## end ClEventMsg%37E10FC403A3.protected

  private:
    // Additional Private Declarations
      //## begin ClEventMsg%37E10FC403A3.private preserve=yes
      //## end ClEventMsg%37E10FC403A3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventMsg%37E10FC403A3.implementation preserve=yes
      //## end ClEventMsg%37E10FC403A3.implementation

};

//## begin ClEventMsg%37E10FC403A3.postscript preserve=yes
//## end ClEventMsg%37E10FC403A3.postscript

//## begin EventList%37EB4494034A.preface preserve=yes
//## end EventList%37EB4494034A.preface

//## Class: EventList%37EB4494034A; Instantiated Class
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB44A4034D;ClEventMsg { -> }

typedef vector< ClEventMsg * > EventList;

//## begin EventList%37EB4494034A.postscript preserve=yes
//## end EventList%37EB4494034A.postscript

//## begin ClEventMsgHistory%37E8B04901BC.preface preserve=yes
//## end ClEventMsgHistory%37E8B04901BC.preface

//## Class: ClEventMsgHistory%37E8B04901BC
//	This interface acts as a container for Event Messages. Its functionality
//	includes access to an ordered and limited collection of event messages and
//	navigation in different directions.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB434A02AF;m4return_t { -> }
//## Uses: <unnamed>%37EB605A0041;m4pcchar_t { -> }

class M4_DECL_M4EVENTHANDLER ClEventMsgHistory 
{
  //## begin ClEventMsgHistory%37E8B04901BC.initialDeclarations preserve=yes
  //## end ClEventMsgHistory%37E8B04901BC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEventMsgHistory%938164939
      ClEventMsgHistory ();

    //## Destructor (specified)
      //## Operation: ~ClEventMsgHistory%938164940
      ~ClEventMsgHistory ();


    //## Other Operations (specified)
      //## Operation: GetSize%938087483
      //	This operation returns then number of items in the container. The upperbound
      //	(maximum size) is prededined in construction time.
      m4size_t GetSize ();

      //## Operation: AddNextEventMsg%938087484; C++
      //	This operation inserts a newer message into the container
      m4return_t AddNextEventMsg (const ClEventMsg &ai_oEventMsg);

      //## Operation: AddPriorEventMsg%938087485; C++
      //	This operation inserts an older message into the container
      m4return_t AddPriorEventMsg (const ClEventMsg &ai_oEventMsg);

      //## Operation: GetFirstEventMsg%938087486; C++
      //	This operation retrieves the oldest message in the container.
      m4return_t GetFirstEventMsg (ClEventMsg &ao_oEventMsg);

      //## Operation: GetNextEventMsg%938087487; C++
      //	This operation retrieves the next message.
      m4return_t GetNextEventMsg (ClEventMsg &ao_oEventMsg);

      //## Operation: GetLastEventMsg%938087488; C++
      //	This operation retrieves the most recent message in the container.
      m4return_t GetLastEventMsg (ClEventMsg &ao_oEventMsg);

      //## Operation: GetPriorEventMsg%938087489; C++
      //	This operation retrieves the previous message.
      m4return_t GetPriorEventMsg (ClEventMsg &ao_oEventMsg);

      //## Operation: Serialize%938164941; C++
      m4return_t Serialize (m4pcchar_t ao_pcBuffer, m4int_t ai_iBufferSize, m4int_t &ao_iOutputSize);

      //## Operation: Deserialize%938164942; C++
      m4return_t Deserialize (m4pcchar_t ai_pcBuffer, m4int_t ai_iBufferSize);

    // Additional Public Declarations
      //## begin ClEventMsgHistory%37E8B04901BC.public preserve=yes
      //## end ClEventMsgHistory%37E8B04901BC.public

  protected:
    // Data Members for Associations

      //## Association: M4EventHandler::<unnamed>%37F4991402E4
      //## Role: ClEventMsgHistory::m_iCounter%37F499160251
      //## begin ClEventMsgHistory::m_iCounter%37F499160251.role preserve=no  protected: m4int32_t { -> UHAN}
      m4int32_t m_iCounter;
      //## end ClEventMsgHistory::m_iCounter%37F499160251.role

    // Additional Protected Declarations
      //## begin ClEventMsgHistory%37E8B04901BC.protected preserve=yes
      //## end ClEventMsgHistory%37E8B04901BC.protected

  private:
    // Data Members for Associations

      //## Association: M4EventHandler::<unnamed>%37EA390303CC
      //## Role: ClEventMsgHistory::m_EventList%37EA39040192
      //## begin ClEventMsgHistory::m_EventList%37EA39040192.role preserve=no  private: EventList { -> 1VHAN}
      EventList m_EventList;
      //## end ClEventMsgHistory::m_EventList%37EA39040192.role

    // Additional Private Declarations
      //## begin ClEventMsgHistory%37E8B04901BC.private preserve=yes
//	  m4pchar_t pcBuffer;
//	  m4uint_t  uiBufferSize;
      //## end ClEventMsgHistory%37E8B04901BC.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventMsgHistory%37E8B04901BC.implementation preserve=yes
      //## end ClEventMsgHistory%37E8B04901BC.implementation

};

//## begin ClEventMsgHistory%37E8B04901BC.postscript preserve=yes
//## end ClEventMsgHistory%37E8B04901BC.postscript

// Class ClEventHeader 

// Class ClEventMsg 

// Class ClEventMsgHistory 

//## begin module%37EA131B00CF.epilog preserve=yes
//## end module%37EA131B00CF.epilog


#endif
