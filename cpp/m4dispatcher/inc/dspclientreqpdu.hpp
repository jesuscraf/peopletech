//## begin module%396C4E2B00AB.cm preserve=no
//## end module%396C4E2B00AB.cm

//## begin module%396C4E2B00AB.cp preserve=no
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
//## end module%396C4E2B00AB.cp

//## Module: dspclientreqpdu%396C4E2B00AB; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: C:\WorkingFolder\m4dispatcher\inc\dspclientreqpdu.hpp

#ifndef dspclientreqpdu_h
#define dspclientreqpdu_h 1

//## begin module%396C4E2B00AB.additionalIncludes preserve=no
//## end module%396C4E2B00AB.additionalIncludes

//## begin module%396C4E2B00AB.includes preserve=yes
//## end module%396C4E2B00AB.includes

// dspreq
#include <dspreq.hpp>

//## begin module%396C4E2B00AB.declarations preserve=no
//## end module%396C4E2B00AB.declarations

//## begin module%396C4E2B00AB.additionalDeclarations preserve=yes
//## end module%396C4E2B00AB.additionalDeclarations


//## begin ClDspClientRequestPDU%396C494402E2.preface preserve=yes
//## end ClDspClientRequestPDU%396C494402E2.preface

//## Class: ClDspClientRequestPDU%396C494402E2
//## Category: M4Dispatcher::Agents::Request%39F45D18001D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%396C4DF303AE;ClPDU { -> F}
//## Uses: <unnamed>%396C5EAD0365;ClTagList { -> F}

class ClDspClientRequestPDU : public ClDspRequest  //## Inherits: <unnamed>%396DB0CF0084
{
  //## begin ClDspClientRequestPDU%396C494402E2.initialDeclarations preserve=yes
  //## end ClDspClientRequestPDU%396C494402E2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDspClientRequestPDU%963388999
      ClDspClientRequestPDU (void *ai_poBuffer, m4uint64_t ai_iRequestID, m4uint32_t ai_iConnectionID);

    //## Destructor (specified)
      //## Operation: ~ClDspClientRequestPDU%963989738
      ~ClDspClientRequestPDU ();


    //## Other Operations (specified)
      //## Operation: BuildReqReply%963825076
      m4return_t BuildReqReply (ClSiteNodeContainer &ai_oSiteNodeContainer, ClDspAnswerGen *ai_poAnswerGen);


	  m4return_t GetClientType( eDspClientTypes& ao_iClientType );

    // Additional Public Declarations
      //## begin ClDspClientRequestPDU%396C494402E2.public preserve=yes
      //## end ClDspClientRequestPDU%396C494402E2.public

  protected:
    // Additional Protected Declarations
      //## begin ClDspClientRequestPDU%396C494402E2.protected preserve=yes
      //## end ClDspClientRequestPDU%396C494402E2.protected

  private:
    // Additional Private Declarations
      //## begin ClDspClientRequestPDU%396C494402E2.private preserve=yes
      //## end ClDspClientRequestPDU%396C494402E2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDspClientRequestPDU%396C494402E2.implementation preserve=yes
      //## end ClDspClientRequestPDU%396C494402E2.implementation

};

//## begin ClDspClientRequestPDU%396C494402E2.postscript preserve=yes
//## end ClDspClientRequestPDU%396C494402E2.postscript

// Class ClDspClientRequestPDU 

//## begin module%396C4E2B00AB.epilog preserve=yes
//## end module%396C4E2B00AB.epilog


#endif
