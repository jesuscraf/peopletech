//## begin module%36E651ED0260.cm preserve=no
//## end module%36E651ED0260.cm

//## begin module%36E651ED0260.cp preserve=no
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
//## end module%36E651ED0260.cp

//## Module: LauncherForCancelExecutor28%36E651ED0260; Package specification
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Source file: D:\FuentesServidor\m4jsrecover\inc\launcherccexe28.hpp

#ifndef launcherforcancelexecutor28_h
#define launcherforcancelexecutor28_h 1

//## begin module%36E651ED0260.additionalIncludes preserve=no
//## end module%36E651ED0260.additionalIncludes

//## begin module%36E651ED0260.includes preserve=yes
#include <m4jsrecover_dll.hpp>
#include <statistics.hpp>
#include <fifoq.hpp>
//## end module%36E651ED0260.includes

// LauncherInterface
#include <launcher.hpp>
// Executive
#include <executive.hpp>

//## begin module%36E651ED0260.declarations preserve=no
//## end module%36E651ED0260.declarations

//## begin module%36E651ED0260.additionalDeclarations preserve=yes
//## end module%36E651ED0260.additionalDeclarations


//## begin ClLauncherForCancelExecutor28%36E4EB2702BC.preface preserve=yes
//## end ClLauncherForCancelExecutor28%36E4EB2702BC.preface

//## Class: ClLauncherForCancelExecutor28%36E4EB2702BC
//## Category: M4JSRecover%37F8B5B70134
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36E654260132;ClExecutive { -> }
//## Uses: <unnamed>%36E6545500FD;ClCcExecutor { -> F}
//## Uses: <unnamed>%381813A3013E;ClJSRequest { -> F}
//## Uses: <unnamed>%381813AB026C;M4ClTimeStamp { -> F}
//## Uses: <unnamed>%381813AD0319;ClJSJob { -> F}

class M4_DECL_M4JSRECOVER ClLauncherForCancelExecutor28 : public ClLauncherInterface  //## Inherits: <unnamed>%38181366017C
{
  //## begin ClLauncherForCancelExecutor28%36E4EB2702BC.initialDeclarations preserve=yes
  //## end ClLauncherForCancelExecutor28%36E4EB2702BC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLauncherForCancelExecutor28%921053038
      ClLauncherForCancelExecutor28 (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats = NULL, ClLog *ai_poLog = NULL, ClServiceQ *ai_poServiceQ = NULL, ClOutputQ *ai_poOutputQ = NULL);


    //## Other Operations (specified)
      //## Operation: Destroy%921053041
      m4return_t Destroy ();

      //## Operation: Initialize%941102054
      m4return_t Initialize (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Run%943459244
      void Run ();

    // Additional Public Declarations
      //## begin ClLauncherForCancelExecutor28%36E4EB2702BC.public preserve=yes
      //## end ClLauncherForCancelExecutor28%36E4EB2702BC.public

  protected:

    //## Other Operations (specified)
      //## Operation: _ExecuteRequest%943433795
      m4return_t _ExecuteRequest (ClBaseRequest *ai_poRequest);

      //## Operation: _PrepareExecution%943433796
      m4return_t _PrepareExecution (ClBaseRequest *ai_poRequest);

      //## Operation: _ReplyRequest%943433797
      m4return_t _ReplyRequest (ClBaseRequest *ai_poRequest);

      //## Operation: _GetNextRequest%943433798
      m4return_t _GetNextRequest (ClBaseRequest *&ao_poRequest);

      //## Operation: _CleanUp%943433799
      m4return_t _CleanUp (ClBaseRequest *ai_poRequest);

    // Additional Protected Declarations
      //## begin ClLauncherForCancelExecutor28%36E4EB2702BC.protected preserve=yes
	  ClOutputQ * m_pOutputQ ;
      //## end ClLauncherForCancelExecutor28%36E4EB2702BC.protected

  private:
    // Additional Private Declarations
      //## begin ClLauncherForCancelExecutor28%36E4EB2702BC.private preserve=yes
      //## end ClLauncherForCancelExecutor28%36E4EB2702BC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLauncherForCancelExecutor28%36E4EB2702BC.implementation preserve=yes
      //## end ClLauncherForCancelExecutor28%36E4EB2702BC.implementation

};

//## begin ClLauncherForCancelExecutor28%36E4EB2702BC.postscript preserve=yes
//## end ClLauncherForCancelExecutor28%36E4EB2702BC.postscript

// Class ClLauncherForCancelExecutor28 

//## begin module%36E651ED0260.epilog preserve=yes
//## end module%36E651ED0260.epilog


#endif
