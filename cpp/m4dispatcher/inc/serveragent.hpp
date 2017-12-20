//## begin module%396B5C8C0153.cm preserve=no
//## end module%396B5C8C0153.cm

//## begin module%396B5C8C0153.cp preserve=no
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
//## end module%396B5C8C0153.cp

//## Module: serveragent%396B5C8C0153; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\serveragent.hpp

#ifndef serveragent_h
#define serveragent_h 1

//## begin module%396B5C8C0153.additionalIncludes preserve=no
//## end module%396B5C8C0153.additionalIncludes

//## begin module%396B5C8C0153.includes preserve=yes
//## end module%396B5C8C0153.includes

// dspagent
#include <dspagent.hpp>
//## begin module%396B5C8C0153.declarations preserve=no
//## end module%396B5C8C0153.declarations

//## begin module%396B5C8C0153.additionalDeclarations preserve=yes
//## end module%396B5C8C0153.additionalDeclarations


//## begin ClServerAgent%396B58810137.preface preserve=yes
//## end ClServerAgent%396B58810137.preface

//## Class: ClServerAgent%396B58810137
//## Category: M4Dispatcher::Agents%39F451CA0292
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClServerAgent : public ClDspAgent  //## Inherits: <unnamed>%396B59B401D9
{
  //## begin ClServerAgent%396B58810137.initialDeclarations preserve=yes
  //## end ClServerAgent%396B58810137.initialDeclarations

  public:
    //## begin ClServerAgent::eServerAgentActions%396C2AE20071.preface preserve=yes
    //## end ClServerAgent::eServerAgentActions%396C2AE20071.preface

    //## Class: eServerAgentActions%396C2AE20071
    //## Category: M4Dispatcher::Agents%39F451CA0292
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { UPDATE_CONFIGURATION_ACTION_ID = ClDspAgent::TOTAL_ACTION , CHECK_SERVER_CONNECTION_ACTION_ID } eServerAgentActions;

    //## begin ClServerAgent::eServerAgentActions%396C2AE20071.postscript preserve=yes
    //## end ClServerAgent::eServerAgentActions%396C2AE20071.postscript

    //## Constructors (specified)
      //## Operation: ClServerAgent%963314064
      ClServerAgent (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent);

    //## Destructor (specified)
      //## Operation: ~ClServerAgent%963314065
      ~ClServerAgent ();


    //## Other Operations (specified)
      //## Operation: ProcessAction%963314066
      virtual m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: Start%963314067
      m4return_t Start ();

      //## Operation: UpdateConfiguration%963314068
      m4return_t UpdateConfiguration (ClSiteNodeInformation &ai_oSiteNode);

      //## Operation: CheckServerConnection%969372803
      m4return_t CheckServerConnection (ClParamList *ai_poParamList);

  public:
    // Additional Public Declarations
      //## begin ClServerAgent%396B58810137.public preserve=yes
      //## end ClServerAgent%396B58810137.public

  protected:
    // Additional Protected Declarations
      //## begin ClServerAgent%396B58810137.protected preserve=yes
      //## end ClServerAgent%396B58810137.protected

  private:
    // Additional Private Declarations
      //## begin ClServerAgent%396B58810137.private preserve=yes
      //## end ClServerAgent%396B58810137.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServerAgent%396B58810137.implementation preserve=yes
      //## end ClServerAgent%396B58810137.implementation

};

//## begin ClServerAgent%396B58810137.postscript preserve=yes
//## end ClServerAgent%396B58810137.postscript

// Class ClServerAgent 

//## begin module%396B5C8C0153.epilog preserve=yes
//## end module%396B5C8C0153.epilog


#endif
