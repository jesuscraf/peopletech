//## begin module%38D91FF500EB.cm preserve=no
//## end module%38D91FF500EB.cm

//## begin module%38D91FF500EB.cp preserve=no
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
//## end module%38D91FF500EB.cp

//## Module: LauncherForUUSS%38D91FF500EB; Subprogram specification
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Source file: D:\eduardoma\m4exefactory\inc\launcherforuuss.hpp

#ifndef launcherforuuss_h
#define launcherforuuss_h 1

//## begin module%38D91FF500EB.additionalIncludes preserve=no
//## end module%38D91FF500EB.additionalIncludes

//## begin module%38D91FF500EB.includes preserve=yes
#include <statistics.hpp>
#include <fifoq.hpp>
//## end module%38D91FF500EB.includes

// LauncherInterface
#include <launcher.hpp>
// USInterface
#include <usinterface.hpp>
// CommandRequest
#include <commandrequest.hpp>
// logsys
#include <logsys.hpp>
//## begin module%38D91FF500EB.declarations preserve=no
//## end module%38D91FF500EB.declarations

//## begin module%38D91FF500EB.additionalDeclarations preserve=yes
//## end module%38D91FF500EB.additionalDeclarations


//## begin ClLauncherForUUSS%35444E0A03E3.preface preserve=yes
//## end ClLauncherForUUSS%35444E0A03E3.preface

//## Class: ClLauncherForUUSS%35444E0A03E3
//## Category: M4ExeFactory::Exec UUSS%3528F7DA0356
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%354494B601D0;ClExeForUUSS { -> F}
//## Uses: <unnamed>%35507EAC00EA;ClCommandRequest { -> }
//## Uses: <unnamed>%356AE92203A3;ClBoomerangRequest { -> F}
//## Uses: <unnamed>%356AF0E302E8;ClUSUserInterface { -> F}
//## Uses: <unnamed>%356C68CE018E;ClUSSessionInterface { -> }
//## Uses: <unnamed>%374C18850084;ClUSServiceAPI { -> F}
//## Uses: <unnamed>%374C191E0193;ClConfiguration { -> F}

class ClLauncherForUUSS : public ClLauncherInterface  //## Inherits: <unnamed>%35444E0E0376
{
  //## begin ClLauncherForUUSS%35444E0A03E3.initialDeclarations preserve=yes
  //## end ClLauncherForUUSS%35444E0A03E3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLauncherForUUSS%894280650; C++
      //	Parameterized constructor for class Scheduler.
      //## Concurrency: Synchonous
      //## Semantics:
      //	Performs generic initialization operation upon its attributes.
      ClLauncherForUUSS (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats = NULL, ClServiceQ *ai_poServiceQ = NULL, ClOutputQ *ai_poOutputQ = NULL);


    //## Other Operations (specified)
      //## Operation: Destroy%894305683
      m4return_t Destroy ();

      //## Operation: Initialize%896286626; C++
      m4return_t Initialize (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

    // Additional Public Declarations
      //## begin ClLauncherForUUSS%35444E0A03E3.public preserve=yes
//m4return_t Initialize (ClServiceQ *ai_pServiceQ, ClOutputQ *ai_pOutputQ, ClLog *ai_pLog, ClActiveClassStats *ai_pStats, ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig);
      //## end ClLauncherForUUSS%35444E0A03E3.public
  protected:

    //## Other Operations (specified)
      //## Operation: _GetNextRequest%943350227; C++
      m4return_t _GetNextRequest (ClBaseRequest *&ao_poRequest);

      //## Operation: _PrepareExecution%943350228; C++
      m4return_t _PrepareExecution (ClBaseRequest *ai_poRequest);

      //## Operation: _ExecuteRequest%943350229
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t _ExecuteRequest (ClBaseRequest *ai_poRequest);

      //## Operation: _ReplyRequest%943350230; C++
      m4return_t _ReplyRequest (ClBaseRequest *ai_poRequest);

      //## Operation: _Authenticate%953729581
      m4return_t _Authenticate (ClCommandRequest &aio_oAuthReq, ClUSUserInterface *&ao_poUserObj, ClUSSessionInterface *&ao_poSessionObj);

      //## Operation: _Authorize%953729582
      m4return_t _Authorize (ClCommandRequest &aio_oAuthReq, ClUSSessionInterface *&aio_poSessionObj);

      //## Operation: _CreateSession%953729583
      m4return_t _CreateSession (ClCommandRequest &aio_oAuthReq);

      //## Operation: _ChangePassword%953729584
      m4return_t _ChangePassword (ClCommandRequest &aio_oAuthReq, ClUSSessionInterface *&aio_poSessionObj);

      //## Operation: _GetStringArg%953831063
      m4return_t _GetStringArg (const ClCommandRequest &ai_oRequest, m4uint32_t ai_uiIndex, m4pchar_t &ao_strArgument);

      //## Operation: _GetBoolArg%953831064
      m4return_t _GetBoolArg (const ClCommandRequest &ai_oRequest, m4uint32_t ai_uiIndex, m4bool_t &ao_bArgument);

      //## Operation: _GetIntArg%953831065
      m4return_t _GetIntArg (const ClCommandRequest &ai_oRequest, m4uint32_t ai_uiIndex, m4int_t &ao_iArgument);

	  m4return_t _GetIntPtrArg(const ClCommandRequest &ai_oRequest, m4uint32_t ai_uiIndex, intptr_t &ao_iArgument);

    // Data Members for Associations

      //## Association: M4ExeFactory::Exec UUSS::<unnamed>%38D7C25402F0
      //## Role: ClLauncherForUUSS::m_poLogSystem%38D7C25500F3
      //## begin ClLauncherForUUSS::m_poLogSystem%38D7C25500F3.role preserve=no  protected: ClLogSystem { -> 1RHAN}
      ClLogSystem *m_poLogSystem;
      //## end ClLauncherForUUSS::m_poLogSystem%38D7C25500F3.role

    // Additional Protected Declarations
      //## begin ClLauncherForUUSS%35444E0A03E3.protected preserve=yes
	 m4return_t _GetCertificate (ClCommandRequest &aio_oAuthReq );

	 // Auditoría de sesión
	 m4return_t _CloseAuditory( ClCommandRequest &aio_oAuthReq );
	 m4return_t _CleanAuditory( ClCommandRequest &aio_oAuthReq );

	 // User actions auditory.
	 m4return_t _UserAuditoryLogoffAction( ClCommandRequest &aio_oAuthReq ) ;
	 m4return_t _UserAuditoryLogonErrorAction( ClCommandRequest &aio_oAuthReq ) ;

      //## end ClLauncherForUUSS%35444E0A03E3.protected

  private:
    // Additional Private Declarations
      //## begin ClLauncherForUUSS%35444E0A03E3.private preserve=yes
      //## end ClLauncherForUUSS%35444E0A03E3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLauncherForUUSS%35444E0A03E3.implementation preserve=yes
      //## end ClLauncherForUUSS%35444E0A03E3.implementation

};

//## begin ClLauncherForUUSS%35444E0A03E3.postscript preserve=yes
//## end ClLauncherForUUSS%35444E0A03E3.postscript

// Class ClLauncherForUUSS 

//## begin module%38D91FF500EB.epilog preserve=yes
//## end module%38D91FF500EB.epilog


#endif
