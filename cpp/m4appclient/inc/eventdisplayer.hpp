//## begin module%379856D80386.cm preserve=no
//## end module%379856D80386.cm

//## begin module%379856D80386.cp preserve=no
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
//## end module%379856D80386.cp

//## Module: eventdisplayer%379856D80386; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\source\m4appclient\inc\eventdisplayer.hpp

#ifndef eventdisplayer_h
#define eventdisplayer_h 1

//## begin module%379856D80386.additionalIncludes preserve=no
//## end module%379856D80386.additionalIncludes

//## begin module%379856D80386.includes preserve=yes
//## end module%379856D80386.includes

// m4thread
#include <m4thread.hpp>
// clientbasicdef
#include <clientbasicdef.hpp>
//## begin module%379856D80386.declarations preserve=no
//## end module%379856D80386.declarations

//## begin module%379856D80386.additionalDeclarations preserve=yes
//## end module%379856D80386.additionalDeclarations


//## begin ClServerEventDisplayer%377356290170.preface preserve=yes
//## end ClServerEventDisplayer%377356290170.preface

//## Class: ClServerEventDisplayer%377356290170
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClServerEventDisplayer : public M4Thread  //## Inherits: <unnamed>%37735643022C
{
  //## begin ClServerEventDisplayer%377356290170.initialDeclarations preserve=yes
  //## end ClServerEventDisplayer%377356290170.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClServerEventDisplayer%930306996; C++
      ClServerEventDisplayer (ClServerEventFunction ai_pServerEventFunction, m4uint32_t ai_iEventType, void *ai_pParam);

    //## Destructor (specified)
      //## Operation: ~ClServerEventDisplayer%941097608; C++
      ~ClServerEventDisplayer ();


    //## Other Operations (specified)
      //## Operation: Run%930306997; C++
      void Run (void );

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%37985B50002E
      //## Role: ClServerEventDisplayer::m_pServerEventFunction%37985B500292
      //## begin ClServerEventDisplayer::m_pServerEventFunction%37985B500292.role preserve=no  public: ClServerEventFunction { -> UHAN}
      ClServerEventFunction m_pServerEventFunction;
      //## end ClServerEventDisplayer::m_pServerEventFunction%37985B500292.role

    // Additional Public Declarations
      //## begin ClServerEventDisplayer%377356290170.public preserve=yes
      //## end ClServerEventDisplayer%377356290170.public

  protected:
    // Additional Protected Declarations
      //## begin ClServerEventDisplayer%377356290170.protected preserve=yes
      //## end ClServerEventDisplayer%377356290170.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iEventType%37735DAA0321
      //## begin ClServerEventDisplayer::m_iEventType%37735DAA0321.attr preserve=no  private: m4uint32_t {UA} ai_iEventType
      m4uint32_t m_iEventType;
      //## end ClServerEventDisplayer::m_iEventType%37735DAA0321.attr

      //## Attribute: m_pParam%37735E0A0076
      //## begin ClServerEventDisplayer::m_pParam%37735E0A0076.attr preserve=no  private: void * {UA} ai_pParam
      void *m_pParam;
      //## end ClServerEventDisplayer::m_pParam%37735E0A0076.attr

    // Additional Private Declarations
      //## begin ClServerEventDisplayer%377356290170.private preserve=yes
      //## end ClServerEventDisplayer%377356290170.private

// bg 114359
#ifndef _UNIX
	  m4LCID_t m_uiLCID ;
#endif

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServerEventDisplayer%377356290170.implementation preserve=yes
      //## end ClServerEventDisplayer%377356290170.implementation

};

//## begin ClServerEventDisplayer%377356290170.postscript preserve=yes
//## end ClServerEventDisplayer%377356290170.postscript

// Class ClServerEventDisplayer 

//## begin module%379856D80386.epilog preserve=yes
//## end module%379856D80386.epilog


#endif
