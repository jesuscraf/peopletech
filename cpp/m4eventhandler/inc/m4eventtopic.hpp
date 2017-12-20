//## begin module%37EA137201E3.cm preserve=no
//## end module%37EA137201E3.cm

//## begin module%37EA137201E3.cp preserve=no
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
//## end module%37EA137201E3.cp

//## Module: m4eventtopic%37EA137201E3; Package specification
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Source file: e:\source\m4eventhandler\version\inc\m4eventtopic.hpp

#ifndef m4eventtopic_h
#define m4eventtopic_h 1

//## begin module%37EA137201E3.additionalIncludes preserve=no
//## end module%37EA137201E3.additionalIncludes

//## begin module%37EA137201E3.includes preserve=yes
#include <m4eventhandler_dll.hpp>
//## end module%37EA137201E3.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
//## begin module%37EA137201E3.declarations preserve=no
//## end module%37EA137201E3.declarations

//## begin module%37EA137201E3.additionalDeclarations preserve=yes
const m4pchar_t	M4UniversalEventTopic	= "com.meta4";
const m4pchar_t	M4AdminEventTopic		= "com.meta4.admin";
//## end module%37EA137201E3.additionalDeclarations


//## Class: ClEventTopicID%37E7419B0057
//	This interface is string-like. It is not shown for being straightforward.
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef M4ClString ClEventTopicID;

//## begin ClEventTopic%37E12DB203C9.preface preserve=yes
//## end ClEventTopic%37E12DB203C9.preface

//## Class: ClEventTopic%37E12DB203C9
//	This interface works as a complex ID for events. This ID is made up by the
//	concatenation of different IDs in such a way that it describes a
//	hierarchical path. It allows also the wildcard symbol (*) which means "all
//	instances in the previous path".
//## Category: M4EventHandler%37E10323003D
//## Subsystem: M4EventHandler::inc%37EA11FD01DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB435300FA;m4return_t { -> }
//## Uses: <unnamed>%37EB6B0702C2;M4ClString { -> }

class M4_DECL_M4EVENTHANDLER ClEventTopic 
{
  //## begin ClEventTopic%37E12DB203C9.initialDeclarations preserve=yes
  //## end ClEventTopic%37E12DB203C9.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEventTopic%938164933; C++
      ClEventTopic (const ClEventTopicID &ai_strTopic);

      //## Operation: ClEventTopic%938164943; C++
      ClEventTopic (m4pcchar_t ai_pccTopic = M4UniversalEventTopic);

    //## Destructor (specified)
      //## Operation: ~ClEventTopic%938164934
      ~ClEventTopic ();


    //## Other Operations (specified)
      //## Operation: GetTopicID%937504393; C++
      m4return_t GetTopicID (ClEventTopicID &ao_oTopicID);

      //## Operation: operator=%938591993; C++
      ClEventTopic  & operator = (const ClEventTopic &ai_oEventTopic);

    // Additional Public Declarations
      //## begin ClEventTopic%37E12DB203C9.public preserve=yes
      //## end ClEventTopic%37E12DB203C9.public

  protected:
    // Additional Protected Declarations
      //## begin ClEventTopic%37E12DB203C9.protected preserve=yes
      //## end ClEventTopic%37E12DB203C9.protected

  private:
    // Data Members for Associations

      //## Association: M4EventHandler::<unnamed>%37EA10E20045
      //## Role: ClEventTopic::<m_pClEventTopicID>%37EA10E20207
      //## begin ClEventTopic::<m_pClEventTopicID>%37EA10E20207.role preserve=no  private: ClEventTopicID { -> VHAN}
      ClEventTopicID m_pClEventTopicID;
      //## end ClEventTopic::<m_pClEventTopicID>%37EA10E20207.role

    // Additional Private Declarations
      //## begin ClEventTopic%37E12DB203C9.private preserve=yes
      //## end ClEventTopic%37E12DB203C9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventTopic%37E12DB203C9.implementation preserve=yes
      //## end ClEventTopic%37E12DB203C9.implementation

};

//## begin ClEventTopic%37E12DB203C9.postscript preserve=yes
//## end ClEventTopic%37E12DB203C9.postscript

// Class ClEventTopic 

//## begin module%37EA137201E3.epilog preserve=yes
//## end module%37EA137201E3.epilog


#endif
