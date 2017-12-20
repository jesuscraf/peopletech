//## begin module%3A7EC0C40303.cm preserve=no
//## end module%3A7EC0C40303.cm

//## begin module%3A7EC0C40303.cp preserve=no
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
//## end module%3A7EC0C40303.cp

//## Module: dspappctlreq%3A7EC0C40303; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\dspappctlreq.hpp

#ifndef dspappctlreq_h
#define dspappctlreq_h 1

//## begin module%3A7EC0C40303.additionalIncludes preserve=no
//## end module%3A7EC0C40303.additionalIncludes

//## begin module%3A7EC0C40303.includes preserve=yes
//## end module%3A7EC0C40303.includes

// m4condition
#include <m4condition.hpp>
// dspreq
#include <dspreq.hpp>
//## begin module%3A7EC0C40303.declarations preserve=no
//## end module%3A7EC0C40303.declarations

//## begin module%3A7EC0C40303.additionalDeclarations preserve=yes
//## end module%3A7EC0C40303.additionalDeclarations


//## begin ClDspAppCtlRequest%3A7EBDDB0257.preface preserve=yes
//## end ClDspAppCtlRequest%3A7EBDDB0257.preface

//## Class: ClDspAppCtlRequest%3A7EBDDB0257
//## Category: M4Dispatcher::Agents::Request%39F45D18001D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDspAppCtlRequest : public ClDspRequest  //## Inherits: <unnamed>%3A7EBDEE0363
{
  //## begin ClDspAppCtlRequest%3A7EBDDB0257.initialDeclarations preserve=yes
  //## end ClDspAppCtlRequest%3A7EBDDB0257.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDspAppCtlRequest%981385465
      ClDspAppCtlRequest (m4uint64_t ai_iRequestID, ClLogSystem *ai_poLog);

    //## Destructor (specified)
      //## Operation: ~ClDspAppCtlRequest%981385466
      ~ClDspAppCtlRequest ();


    //## Other Operations (specified)
      //## Operation: WaitRead%981385467
      void WaitRead ();

      //## Operation: ReadReady%981385468
      void ReadReady ();

      //## Operation: BuildReqReply%981452486
      m4return_t BuildReqReply (ClSiteNodeContainer &ai_oSiteNodeContainer, ClDspAnswerGen *ai_poAnswerGen);

      //## Operation: SetConnectionID%981452487
      void SetConnectionID (m4uint32_t ai_iConnectionID);

    // Additional Public Declarations
      //## begin ClDspAppCtlRequest%3A7EBDDB0257.public preserve=yes
      //## end ClDspAppCtlRequest%3A7EBDDB0257.public

  protected:
    // Additional Protected Declarations
      //## begin ClDspAppCtlRequest%3A7EBDDB0257.protected preserve=yes
      //## end ClDspAppCtlRequest%3A7EBDDB0257.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bReadyRead%3A7EC00C00CE
      //## begin ClDspAppCtlRequest::m_bReadyRead%3A7EC00C00CE.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bReadyRead;
      //## end ClDspAppCtlRequest::m_bReadyRead%3A7EC00C00CE.attr

      //## Attribute: ai_iHARequestID%3A7FC1B2014D
      //## begin ClDspAppCtlRequest::ai_iHARequestID%3A7FC1B2014D.attr preserve=no  private: m4uint64_t {UA} 
      m4uint64_t ai_iHARequestID;
      //## end ClDspAppCtlRequest::ai_iHARequestID%3A7FC1B2014D.attr

    // Data Members for Associations

      //## Association: M4Dispatcher::Agents::Request::<unnamed>%3A7EC02A03B6
      //## Role: ClDspAppCtlRequest::m_oReadCondition%3A7EC02B01B8
      //## begin ClDspAppCtlRequest::m_oReadCondition%3A7EC02B01B8.role preserve=no  private: ClCondition { -> VHAN}
      ClCondition m_oReadCondition;
      //## end ClDspAppCtlRequest::m_oReadCondition%3A7EC02B01B8.role

    // Additional Private Declarations
      //## begin ClDspAppCtlRequest%3A7EBDDB0257.private preserve=yes
      //## end ClDspAppCtlRequest%3A7EBDDB0257.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDspAppCtlRequest%3A7EBDDB0257.implementation preserve=yes
      //## end ClDspAppCtlRequest%3A7EBDDB0257.implementation

};

//## begin ClDspAppCtlRequest%3A7EBDDB0257.postscript preserve=yes
//## end ClDspAppCtlRequest%3A7EBDDB0257.postscript

// Class ClDspAppCtlRequest 

//## begin module%3A7EC0C40303.epilog preserve=yes
//## end module%3A7EC0C40303.epilog


#endif
