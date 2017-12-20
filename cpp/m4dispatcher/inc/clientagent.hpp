//## begin module%396B5C760166.cm preserve=no
//## end module%396B5C760166.cm

//## begin module%396B5C760166.cp preserve=no
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
//## end module%396B5C760166.cp

//## Module: clientagent%396B5C760166; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\clientagent.hpp

#ifndef clientagent_h
#define clientagent_h 1

//## begin module%396B5C760166.additionalIncludes preserve=no
//## end module%396B5C760166.additionalIncludes

//## begin module%396B5C760166.includes preserve=yes
//## end module%396B5C760166.includes

// dspclientreqpdu
#include <dspclientreqpdu.hpp>
// dspagent
#include <dspagent.hpp>
//## begin module%396B5C760166.declarations preserve=no
//## end module%396B5C760166.declarations

//## begin module%396B5C760166.additionalDeclarations preserve=yes
//## end module%396B5C760166.additionalDeclarations


//## begin ClClientAgent%396B588D038D.preface preserve=yes
//## end ClClientAgent%396B588D038D.preface

//## Class: ClClientAgent%396B588D038D
//## Category: M4Dispatcher%3923B57F0208
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39740D0B0214;ClDspClientRequestPDU { -> }

class ClClientAgent : public ClDspAgent  //## Inherits: <unnamed>%396B59B602B8
{
  //## begin ClClientAgent%396B588D038D.initialDeclarations preserve=yes
  //## end ClClientAgent%396B588D038D.initialDeclarations

  public:
    //## begin ClClientAgent::eClientAgentActions%396C618402CA.preface preserve=yes
    //## end ClClientAgent::eClientAgentActions%396C618402CA.preface

    //## Class: eClientAgentActions%396C618402CA
    //## Category: M4Dispatcher%3923B57F0208
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { PROCESS_PDU_ID = ClDspAgent::TOTAL_ACTION } eClientAgentActions;

    //## begin ClClientAgent::eClientAgentActions%396C618402CA.postscript preserve=yes
    //## end ClClientAgent::eClientAgentActions%396C618402CA.postscript

    //## Constructors (specified)
      //## Operation: ClClientAgent%963314069
      ClClientAgent (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent);

    //## Destructor (specified)
      //## Operation: ~ClClientAgent%963314070
      ~ClClientAgent ();


    //## Other Operations (specified)
      //## Operation: ProcessAction%963314071
      virtual m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: Start%963314072
      m4return_t Start ();

      //## Operation: RedirectClientConnection%963314073
      m4return_t RedirectClientConnection (ClDspClientRequestPDU *ai_poClientRequest);

  public:
    // Additional Public Declarations
      //## begin ClClientAgent%396B588D038D.public preserve=yes
      //## end ClClientAgent%396B588D038D.public

  protected:
    // Additional Protected Declarations
      //## begin ClClientAgent%396B588D038D.protected preserve=yes
      //## end ClClientAgent%396B588D038D.protected

  private:
    // Additional Private Declarations
      //## begin ClClientAgent%396B588D038D.private preserve=yes
      //## end ClClientAgent%396B588D038D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClClientAgent%396B588D038D.implementation preserve=yes
      //## end ClClientAgent%396B588D038D.implementation

};

//## begin ClClientAgent%396B588D038D.postscript preserve=yes
//## end ClClientAgent%396B588D038D.postscript

// Class ClClientAgent 

//## begin module%396B5C760166.epilog preserve=yes
//## end module%396B5C760166.epilog


#endif
