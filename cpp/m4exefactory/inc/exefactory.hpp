//## begin module%34BA4505034B.cm preserve=no
//## end module%34BA4505034B.cm

//## begin module%34BA4505034B.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ExeFactory.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980112
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
//## end module%34BA4505034B.cp

//## Module: ExeFactory%34BA4505034B; Package specification
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Source file: d:\eduardoma\m4exefactory\inc\exefactory.hpp

#ifndef exefactory_h
#define exefactory_h 1

//## begin module%34BA4505034B.additionalIncludes preserve=no
//## end module%34BA4505034B.additionalIncludes

//## begin module%34BA4505034B.includes preserve=yes
#include <m4exefactory_dll.hpp>
#include <m4define.hpp>
#include <fifoq.hpp>
//## end module%34BA4505034B.includes

// LauncherInterface
#include <launcher.hpp>
// m4types
#include <m4types.hpp>
//## begin module%34BA4505034B.declarations preserve=no
//## end module%34BA4505034B.declarations

//## begin module%34BA4505034B.additionalDeclarations preserve=yes
#define M4_EXECUTOR_TEST		0x01
#define M4_EXECUTOR_CHANNEL		0x02
#define M4_EXECUTOR_ECHO		0x03
#define M4_EXECUTOR_JOB			0x04
#define M4_EXECUTOR_METHOD		0x05
#define M4_EXECUTOR_PERSIST		0x06
#define M4_EXECUTOR_LOAD		0x07
#define M4_EXECUTOR_UUSS		0x08
#define M4_EXECUTOR_SESSION		0x09
#define M4_EXECUTOR_JOB_GET     0x0a
#define M4_EXECUTOR_JOB_EXECUTE 0x0b
#define M4_EXECUTOR_PROXY       0x0c
#define M4_EXECUTOR_NODB		0x0d
#define M4_EXECUTOR_MD			0x0e
#define M4_EXECUTOR_ENV			0x0f
#define M4_EXECUTOR_CANCEL_JOB	0x10
#define M4_EXECUTOR_XML			0x11
#define M4_EXECUTOR_GP			M4_EXECUTOR_ENV


//## end module%34BA4505034B.additionalDeclarations


//## Class: ClExeFactory%34B9ED5F029F
//## Category: M4ExeFactory%34B9E4050160
//## Subsystem: M4ExeFactory::inc%37D781A3038C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34B9ED7F00CB;ClExecutiveForChannel { -> F}
//## Uses: <unnamed>%34BC963501C1;m4return_t { -> }
//## Uses: <unnamed>%34E0371F010E;ClExeForEcho { -> F}
//## Uses: <unnamed>%3516B4730135; { -> F}
//## Uses: <unnamed>%3528F73C03C3;ClExeForSessionChannel { -> F}
//## Uses: <unnamed>%3528FAAC0104;ClExeForUUSS { -> F}
//## Uses: <unnamed>%353F404700BD;ClExecutiveForProxy { -> F}
//## Uses: <unnamed>%3548AAFA000D;ClLauncherInterface { -> }
//## Uses: <unnamed>%3548B38A0164;ClLauncherForOLTP { -> F}
//## Uses: <unnamed>%3548BAB90174;ClLauncherForUUSS { -> F}
//## Uses: <unnamed>%355881060027;ClExecutiveForMetaData { -> F}

class M4_DECL_M4EXEFACTORY ClExeFactory 
{
  public:

    //## Other Operations (specified)
      //## Operation: Create%884605459; C++
      static m4return_t Create (m4uint32_t ai_uiServiceID, ClLauncherInterface *&ao_pExecutor, ClLauncherStats *ai_poStats, ClServiceQ *ai_poServiceQ = NULL, ClOutputQ *ai_poOutputQ = NULL);

  protected:
    //## Constructors (specified)
      //## Operation: ClExeFactory%884605458
      ClExeFactory ();

  private:
  private: //## implementation
};

// Class ClExeFactory 

//## begin module%34BA4505034B.epilog preserve=yes
//## end module%34BA4505034B.epilog


#endif
