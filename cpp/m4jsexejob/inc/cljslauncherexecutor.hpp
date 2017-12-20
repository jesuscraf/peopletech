//## begin module%37FB5F0D0034.cm preserve=no
//## end module%37FB5F0D0034.cm

//## begin module%37FB5F0D0034.cp preserve=no
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
//## end module%37FB5F0D0034.cp

//## Module: ClJSLauncherExecutor%37FB5F0D0034; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: E:\maite_work\m4jsexejob\inc\cljslauncherexecutor.hpp

#ifndef cljslauncherexecutor_h
#define cljslauncherexecutor_h 1

//## begin module%37FB5F0D0034.additionalIncludes preserve=no
//## end module%37FB5F0D0034.additionalIncludes

//## begin module%37FB5F0D0034.includes preserve=yes
#include <m4jsexejob_dll.hpp>
#include <statistics.hpp>
#include <fifoq.hpp>
//## end module%37FB5F0D0034.includes

// LauncherInterface
#include <launcher.hpp>
// ClAdminTag
#include <cladmintag.hpp>

class M4RuntimeException;
class ClJSRequest;
class ClExeForJobSchecEXECUTE;
class ClJSJob;

//## begin module%37FB5F0D0034.declarations preserve=no
//## end module%37FB5F0D0034.declarations

//## begin module%37FB5F0D0034.additionalDeclarations preserve=yes
#define LAUNCHER_STATE_WAITING "Waiting"
#define LAUNCHER_STATE_EXECUTING "Executing"
//## end module%37FB5F0D0034.additionalDeclarations


//## begin ClJSLauncherExecutor%37FB5DA103DE.preface preserve=yes
//## end ClJSLauncherExecutor%37FB5DA103DE.preface

//## Class: ClJSLauncherExecutor%37FB5DA103DE
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3804746701B6;ClExeForJobSchecEXECUTE { -> F}
//## Uses: <unnamed>%3804CA910100;ClJSRequest { -> F}
//## Uses: <unnamed>%380C9168005D;ClAdminCommandTag { -> }
//## Uses: <unnamed>%3805FD1200EB;M4ClTimeStamp { -> F}
//## Uses: <unnamed>%37FDAA280231;ClJSJob { -> F}
//## Uses: <unnamed>%37FC5EA20084;M4RuntimeException { -> F}

class M4_DECL_M4JSEXEJOB ClJSLauncherExecutor : public ClLauncherInterface  //## Inherits: <unnamed>%383BA82F0061
{
  //## begin ClJSLauncherExecutor%37FB5DA103DE.initialDeclarations preserve=yes
  //## end ClJSLauncherExecutor%37FB5DA103DE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSLauncherExecutor%939810520
      ClJSLauncherExecutor (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats = NULL, ClLog *ai_poLog = NULL, ClServiceQ *ai_poServiceQ = NULL, ClOutputQ *ai_poOutputQ = NULL);


    //## Other Operations (specified)
      //## Operation: Initialize%939810523
      m4return_t Initialize (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Destroy%939810524
      m4return_t Destroy ();

      //## Operation: _GetNextRequest%943433950
      m4return_t _GetNextRequest (ClBaseRequest *&ao_poRequest);

      //## Operation: _PrepareExecution%943433951
      m4return_t _PrepareExecution (ClBaseRequest *ai_poRequest);

      //## Operation: _ExecuteRequest%943433952
      m4return_t _ExecuteRequest (ClBaseRequest *ai_poRequest);

      //## Operation: _ReplyRequest%943433953
      m4return_t _ReplyRequest (ClBaseRequest *ai_poRequest);

      //## Operation: _CleanUp%943433954
      m4return_t _CleanUp (ClBaseRequest *ai_poRequest);

    // Additional Public Declarations
      //## begin ClJSLauncherExecutor%37FB5DA103DE.public preserve=yes
      //## end ClJSLauncherExecutor%37FB5DA103DE.public

  protected:
    // Additional Protected Declarations
      //## begin ClJSLauncherExecutor%37FB5DA103DE.protected preserve=yes
	  ClOutputQ * m_pOutputQ ;
      //## end ClJSLauncherExecutor%37FB5DA103DE.protected

  private:
    // Additional Private Declarations
      //## begin ClJSLauncherExecutor%37FB5DA103DE.private preserve=yes
      //## end ClJSLauncherExecutor%37FB5DA103DE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSLauncherExecutor%37FB5DA103DE.implementation preserve=yes
      //## end ClJSLauncherExecutor%37FB5DA103DE.implementation

};

//## begin ClJSLauncherExecutor%37FB5DA103DE.postscript preserve=yes
//## end ClJSLauncherExecutor%37FB5DA103DE.postscript

// Class ClJSLauncherExecutor 

//## begin module%37FB5F0D0034.epilog preserve=yes
//## end module%37FB5F0D0034.epilog


#endif
