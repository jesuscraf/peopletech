//## begin module%3528FACC0375.cm preserve=no
//## end module%3528FACC0375.cm

//## begin module%3528FACC0375.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ExeFactory
//	 File:              exeforsessionch.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980407
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%3528FACC0375.cp

//## Module: ExeForSessionCh%3528FACC0375; Package specification
//## Subsystem: M4ExeForChannel::inc%379C5C480310
//## Source file: D:\eduardoma\m4exeforchannel\inc\exeforsessionch.hpp

#ifndef exeforsessionch_h
#define exeforsessionch_h 1

//## begin module%3528FACC0375.additionalIncludes preserve=no
//## end module%3528FACC0375.additionalIncludes

//## begin module%3528FACC0375.includes preserve=yes
#include "m4exeforchannel_dll.hpp"
#include <clconfiguration.hpp>
#include <executiveforoe.hpp>

class ClChannel;
class ClImage;

//## begin module%3528FACC0375.declarations preserve=no
//## end module%3528FACC0375.declarations

//## begin module%3528FACC0375.additionalDeclarations preserve=yes
//## end module%3528FACC0375.additionalDeclarations


//## begin ClExeForSessionChannel%3528F72501C0.preface preserve=yes
//## end ClExeForSessionChannel%3528F72501C0.preface

//## Class: ClExeForSessionChannel%3528F72501C0
//## Category: M4ExeForChannel::UUSS%38F32B29020C
//## Subsystem: M4ExeForChannel::inc%379C5C480310
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%373C1CD90367;ClImage { -> F}
//## Uses: <unnamed>%373C1D0A0209;ClUSSessionTicketInterface { -> F}
//## Uses: <unnamed>%35D01E490049;ClUSSessionInterface { -> F}
//## Uses: <unnamed>%3744534D0313;ClCMObjectDirectory { -> F}
//## Uses: <unnamed>%37445354024B;ClDataStoreIODriver { -> F}
//## Uses: <unnamed>%3744535700F0;ClAccess { -> F}
//## Uses: <unnamed>%374453650295;ClOLTPExecutionContext { -> F}
//## Uses: <unnamed>%374453690255;IExecutor { -> F}
//## Uses: <unnamed>%37D79F7300E7;ClConfiguration { -> }
//## Uses: <unnamed>%37D8DF4603D5;TChannelUtil { -> F}
//## Uses: <unnamed>%37D8DFC2026A;ClChannel_Kernel { -> F}
//## Uses: <unnamed>%38F5F1BB0040;ClM4ObjService { -> F}

class M4_DECL_M4EXEFORCHANNEL ClExeForSessionChannel : public ClExecutiveForOE  //## Inherits: <unnamed>%38C8F9B601FB
{
  //## begin ClExeForSessionChannel%3528F72501C0.initialDeclarations preserve=yes
  //## end ClExeForSessionChannel%3528F72501C0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExeForSessionChannel%892638088; C++
      ClExeForSessionChannel (ClEngine  *ai_pEngine);

    //## Destructor (specified)
      //## Operation: ~ClExeForSessionChannel%892638089
      ~ClExeForSessionChannel ();


    //## Other Operations (specified)
      //## Operation: Execute%892638091
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

      //## Operation: StartUp%953028451
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

    // Additional Public Declarations
      //## begin ClExeForSessionChannel%3528F72501C0.public preserve=yes
      //## end ClExeForSessionChannel%3528F72501C0.public

  protected:
    // Data Members for Associations

      //## Association: M4ExeForChannel::UUSS::<unnamed>%38CE307B002A
      //## Role: ClExeForSessionChannel::m_pServerSessionChannel%38CE307C01F8
      //## begin ClExeForSessionChannel::m_pServerSessionChannel%38CE307C01F8.role preserve=no  protected: ClChannel { -> 1RFHAN}
      ClChannel *m_pServerSessionChannel;
      //## end ClExeForSessionChannel::m_pServerSessionChannel%38CE307C01F8.role

    // Additional Protected Declarations
      //## begin ClExeForSessionChannel%3528F72501C0.protected preserve=yes
      //## end ClExeForSessionChannel%3528F72501C0.protected

  private:
    // Additional Private Declarations
      //## begin ClExeForSessionChannel%3528F72501C0.private preserve=yes
      //## end ClExeForSessionChannel%3528F72501C0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExeForSessionChannel%3528F72501C0.implementation preserve=yes
      //## end ClExeForSessionChannel%3528F72501C0.implementation

};

//## begin ClExeForSessionChannel%3528F72501C0.postscript preserve=yes
//## end ClExeForSessionChannel%3528F72501C0.postscript

// Class ClExeForSessionChannel 

//## begin module%3528FACC0375.epilog preserve=yes
//## end module%3528FACC0375.epilog


#endif
