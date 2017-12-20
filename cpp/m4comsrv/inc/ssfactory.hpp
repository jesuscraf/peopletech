//## begin module%3492945502BE.cm preserve=no
//## end module%3492945502BE.cm

//## begin module%3492945502BE.cp preserve=no
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
//## end module%3492945502BE.cp

//## Module: SSFactory%3492945502BE; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\ssfactory.hpp

#ifndef ssfactory_h
#define ssfactory_h 1

//## begin module%3492945502BE.additionalIncludes preserve=no
//## end module%3492945502BE.additionalIncludes

//## begin module%3492945502BE.includes preserve=yes
#include "m4stl.hpp"
#include <clssimplementacion.hpp>
//## end module%3492945502BE.includes

// syncro
#include <syncro.hpp>
// clssfactoryinterfaz
#include <clssfactoryinterfaz.hpp>

class ClConfiguration;
class ClServEnv;
class ClServiceInfo;
class ClLMStats;
class ClNavigatorSS;
class ClSchedulerListStats;

//## begin module%3492945502BE.declarations preserve=no
//## end module%3492945502BE.declarations

//## begin module%3492945502BE.additionalDeclarations preserve=yes
//## end module%3492945502BE.additionalDeclarations


//## begin SUBSYSTEMCACHE%36D19EC6038D.preface preserve=yes
//## end SUBSYSTEMCACHE%36D19EC6038D.preface

//## Class: SUBSYSTEMCACHE%36D19EC6038D
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef multimap <m4objid_t , ClSSInterfaz * ,  less < m4objid_t > > SUBSYSTEMCACHE;

//## begin SUBSYSTEMCACHE%36D19EC6038D.postscript preserve=yes
//## end SUBSYSTEMCACHE%36D19EC6038D.postscript

//## begin ClSubsystemCaches%36D19BFB0122.preface preserve=yes
//## end ClSubsystemCaches%36D19BFB0122.preface

//## Class: ClSubsystemCaches%36D19BFB0122
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%371F69E8008D;ClUSSessionInterface { -> F}

class ClSubsystemCaches 
{
  //## begin ClSubsystemCaches%36D19BFB0122.initialDeclarations preserve=yes
  //## end ClSubsystemCaches%36D19BFB0122.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSubsystemCaches%919708146
      ClSubsystemCaches ();

    //## Destructor (specified)
      //## Operation: ~ClSubsystemCaches%919708147
      ~ClSubsystemCaches ();


    //## Other Operations (specified)
      //## Operation: GetSubsystem%919708148
      ClSSInterfaz * GetSubsystem (m4uint32_t ai_iType);

      //## Operation: StoreSubsystem%919708149
      void StoreSubsystem (ClSSInterfaz *ai_poSubsystem);

      //## Operation: StoreSessionForWaiting%919764797
      void StoreSessionForWaiting (ClSSInterfaz *ai_poSessionSS);

      //## Operation: Synchronize%919764798
      m4return_t Synchronize ();

    // Additional Public Declarations
      //## begin ClSubsystemCaches%36D19BFB0122.public preserve=yes
      //## end ClSubsystemCaches%36D19BFB0122.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%36D19D31018C
      //## Role: ClSubsystemCaches::m_oExclusion%36D19D320043
      //## begin ClSubsystemCaches::m_oExclusion%36D19D320043.role preserve=no  protected: ClMutex { -> UHAN}
      ClMutex m_oExclusion;
      //## end ClSubsystemCaches::m_oExclusion%36D19D320043.role

      //## Association: Subsystem::<unnamed>%36D19F2B0248
      //## Role: ClSubsystemCaches::m_oCache%36D19F2C0203
      //## begin ClSubsystemCaches::m_oCache%36D19F2C0203.role preserve=no  protected: SUBSYSTEMCACHE { -> UFHAN}
      SUBSYSTEMCACHE m_oCache;
      //## end ClSubsystemCaches::m_oCache%36D19F2C0203.role

      //## Association: Subsystem::<unnamed>%36D279EC0396
      //## Role: ClSubsystemCaches::m_oClosedSessions%36D279EE006E
      //## begin ClSubsystemCaches::m_oClosedSessions%36D279EE006E.role preserve=no  protected: SUBSYSTEMCACHE { -> UHAN}
      SUBSYSTEMCACHE m_oClosedSessions;
      //## end ClSubsystemCaches::m_oClosedSessions%36D279EE006E.role

    // Additional Protected Declarations
      //## begin ClSubsystemCaches%36D19BFB0122.protected preserve=yes
      //## end ClSubsystemCaches%36D19BFB0122.protected

  private:
    // Additional Private Declarations
      //## begin ClSubsystemCaches%36D19BFB0122.private preserve=yes
      //## end ClSubsystemCaches%36D19BFB0122.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSubsystemCaches%36D19BFB0122.implementation preserve=yes
      //## end ClSubsystemCaches%36D19BFB0122.implementation

};

//## begin ClSubsystemCaches%36D19BFB0122.postscript preserve=yes
//## end ClSubsystemCaches%36D19BFB0122.postscript

//## begin ClSSFactory%348FFBC101BB.preface preserve=yes
//## end ClSSFactory%348FFBC101BB.preface

//## Class: ClSSFactory%348FFBC101BB
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3492C1F50331;ClConfiguration { -> F}
//## Uses: <unnamed>%34953B300240;ClServiceInfo { -> F}
//## Uses: <unnamed>%34BB5BAA0283;ClExeFactory { -> F}
//## Uses: <unnamed>%34C88C4D0111;ClUsersSS { -> F}
//## Uses: <unnamed>%34E0116D0037;ClLDBSubsystem { -> F}
//## Uses: <unnamed>%353CC95B01FE;ClSchedulerListStats { -> F}
//## Uses: <unnamed>%3540B08B0356;ClLMStats { -> F}
//## Uses: <unnamed>%3545E48F00D5;ClCacheSubSystem { -> F}
//## Uses: <unnamed>%3561CE1803E5;ClNavigatorSS { -> F}
//## Uses: <unnamed>%356AF9430042;ClServEnv { -> F}
//## Uses: <unnamed>%356AF9AD018F;ClExecutive { -> F}
//## Uses: <unnamed>%35863A24013C; { -> F}
//## Uses: <unnamed>%35A336E401AC; { -> F}
//## Uses: <unnamed>%35A3B7180235; { -> F}
//## Uses: <unnamed>%36A45C1001A7;ClRequest { -> F}
//## Uses: <unnamed>%36A4A68D0125;ClLocalMonitor { -> F}
//## Uses: <unnamed>%36D291D301F0;ClConnection { -> F}
//## Uses: <unnamed>%36E94BE30206; { -> F}
//## Uses: <unnamed>%37554A540074;ClUSServiceAPI { -> F}
//## Uses: <unnamed>%377B9DE90177; { -> F}
//## Uses: <unnamed>%38D7AE440012;ClLocalMonitorSS { -> F}

class ClSSFactory : public ClSSFactoryInterfaz  //## Inherits: <unnamed>%37DE79020266
{
  //## begin ClSSFactory%348FFBC101BB.initialDeclarations preserve=yes
  //## end ClSSFactory%348FFBC101BB.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSSFactory%882023712
      ClSSFactory ();

    //## Destructor (specified)
      //## Operation: ~ClSSFactory%882023713
      ~ClSSFactory ();


    //## Other Operations (specified)
      //## Operation: GenerateSubsystem%882023714
      ClSSInterfaz * GenerateSubsystem (m4uint16_t ai_IType, ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: InitializeComponent%882023715
      void InitializeComponent (ClConfiguration *ai_poConfig);

      //## Operation: InitializeLocalMonitorSS%882023719
      ClSSImplementation* InitializeLocalMonitorSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

      //## Operation: InitializeService%882023720
      ClSSImplementation * InitializeService (ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: ShutDownSubsystem%882023721
      m4return_t ShutDownSubsystem (ClSSInterfaz *ai_poSubsystem);

      //## Operation: InitializeConnManagerSS%882177029
      ClSSImplementation* InitializeConnManagerSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

      //## Operation: InitializeCacheSS%882177036
      ClSSImplementation* InitializeCacheSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, m4pchar_t ao_szCacheName = NULL);

      //## Operation: InitializeLauncherSS%884599287
      ClSSImplementation * InitializeLauncherSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: InitializeUsersSS%885483669
      ClSSImplementation * InitializeUsersSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: InitializeUserSS%885483670
      ClSSImplementation * InitializeUserSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: InitializeSesionSS%885483671
      ClSSImplementation * InitializeSesionSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: InitializeSessChServiceSS%895476026
      ClSSImplementation * InitializeSessChServiceSS (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables);

      //## Operation: InitializeUSServiceSS%895476027
      ClSSImplementation * InitializeUSServiceSS (ClConfiguration *ai_poConfiguration,  ClTables *ai_poTables);

      //## Operation: InitializeCachesSS%895659110
      ClSSImplementation * InitializeCachesSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: InitializeUSExeSS%895738255
      ClSSImplementation* InitializeUSExeSS (ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: Synchronize%919764796
      m4return_t Synchronize ();

    // Additional Public Declarations
      //## begin ClSSFactory%348FFBC101BB.public preserve=yes
      //## end ClSSFactory%348FFBC101BB.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%36D19C4C0223
      //## Role: ClSSFactory::m_oSubsystemsStorage%36D19C4D01DE
      //## begin ClSSFactory::m_oSubsystemsStorage%36D19C4D01DE.role preserve=no  protected: ClSubsystemCaches { -> UHAN}
      ClSubsystemCaches m_oSubsystemsStorage;
      //## end ClSSFactory::m_oSubsystemsStorage%36D19C4D01DE.role

    // Additional Protected Declarations
      //## begin ClSSFactory%348FFBC101BB.protected preserve=yes
      //## end ClSSFactory%348FFBC101BB.protected

  private:
    // Additional Private Declarations
      //## begin ClSSFactory%348FFBC101BB.private preserve=yes
      //## end ClSSFactory%348FFBC101BB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSFactory%348FFBC101BB.implementation preserve=yes
      //## end ClSSFactory%348FFBC101BB.implementation

};

//## begin ClSSFactory%348FFBC101BB.postscript preserve=yes
//## end ClSSFactory%348FFBC101BB.postscript

// Class ClSubsystemCaches 

// Class ClSSFactory 

//## begin module%3492945502BE.epilog preserve=yes
//## end module%3492945502BE.epilog


#endif
