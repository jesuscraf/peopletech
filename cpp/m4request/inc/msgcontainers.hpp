//## begin module%3AD329E90042.cm preserve=no
//## end module%3AD329E90042.cm

//## begin module%3AD329E90042.cp preserve=no
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
//## end module%3AD329E90042.cp

//## Module: msgcontainers%3AD329E90042; Package specification
//## Subsystem: M4Request::inc%379439BF01B6
//## Source file: F:\integration\m4request\inc\msgcontainers.hpp

#ifndef msgcontainers_h
#define msgcontainers_h 1

//## begin module%3AD329E90042.additionalIncludes preserve=no
//## end module%3AD329E90042.additionalIncludes

//## begin module%3AD329E90042.includes preserve=yes
//## end module%3AD329E90042.includes

// ImsgContainer
#include <imsgcontainer.hpp>
//## begin module%3AD329E90042.declarations preserve=no
//## end module%3AD329E90042.declarations

//## begin module%3AD329E90042.additionalDeclarations preserve=yes
//## end module%3AD329E90042.additionalDeclarations


//## begin ClSingleMsgContainer%3AD328C901E2.preface preserve=yes
//## end ClSingleMsgContainer%3AD328C901E2.preface

//## Class: ClSingleMsgContainer%3AD328C901E2
//## Category: M4Request%35EFAF8303E2
//## Subsystem: M4Request::inc%379439BF01B6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSingleMsgContainer : public ClMessageContainer  //## Inherits: <unnamed>%3AD3290901DA
{
  //## begin ClSingleMsgContainer%3AD328C901E2.initialDeclarations preserve=yes
  //## end ClSingleMsgContainer%3AD328C901E2.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: StoreTraceMessage%986914181
      void StoreTraceMessage (m4string_t ai_strMessage);

      //## Operation: GetTraceMessage%986975096
      m4return_t GetTraceMessage (m4string_t &ao_strMessage);

    // Additional Public Declarations
      //## begin ClSingleMsgContainer%3AD328C901E2.public preserve=yes
      //## end ClSingleMsgContainer%3AD328C901E2.public

  protected:
    // Additional Protected Declarations
      //## begin ClSingleMsgContainer%3AD328C901E2.protected preserve=yes
      //## end ClSingleMsgContainer%3AD328C901E2.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_strMessage%3AD329270147
      //## begin ClSingleMsgContainer::m_strMessage%3AD329270147.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_strMessage;
      //## end ClSingleMsgContainer::m_strMessage%3AD329270147.attr

    // Additional Private Declarations
      //## begin ClSingleMsgContainer%3AD328C901E2.private preserve=yes
      //## end ClSingleMsgContainer%3AD328C901E2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSingleMsgContainer%3AD328C901E2.implementation preserve=yes
      //## end ClSingleMsgContainer%3AD328C901E2.implementation

};

//## begin ClSingleMsgContainer%3AD328C901E2.postscript preserve=yes
//## end ClSingleMsgContainer%3AD328C901E2.postscript

// Class ClSingleMsgContainer 

//## begin module%3AD329E90042.epilog preserve=yes
//## end module%3AD329E90042.epilog


#endif
