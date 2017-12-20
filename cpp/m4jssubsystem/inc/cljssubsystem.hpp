//## begin module%36E4009F0221.cm preserve=no
//## end module%36E4009F0221.cm

//## begin module%36E4009F0221.cp preserve=no
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
//## end module%36E4009F0221.cp

//## Module: ClJSSubsystem%36E4009F0221; Package specification
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Source file: D:\FuentesServidor\m4jssubsystem\inc\cljssubsystem.hpp

#ifndef cljssubsystem_h
#define cljssubsystem_h 1

//## begin module%36E4009F0221.additionalIncludes preserve=no
//## end module%36E4009F0221.additionalIncludes

//## begin module%36E4009F0221.includes preserve=yes
#include <m4jssubsystem_dll.hpp>
//## end module%36E4009F0221.includes

// JSServiceCancel
#include <jsservicecancel.hpp>
// JSServiceExecute
#include <jsserviceexecute.hpp>
// ClOblConfiguration
#include <cloblconfiguration.hpp>
// clssinterfaz
#include <clssinterfaz.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// statsjssubsystem
#include <statsjssubsystem.hpp>

class ClJSAdministrator;

//## begin module%36E4009F0221.declarations preserve=no
//## end module%36E4009F0221.declarations

//## begin module%36E4009F0221.additionalDeclarations preserve=yes
//## end module%36E4009F0221.additionalDeclarations


//## begin ClJSSubsystem%36E3C96F0280.preface preserve=yes
//## end ClJSSubsystem%36E3C96F0280.preface

//## Class: ClJSSubsystem%36E3C96F0280
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37CF9D8E03B8;ClJSServiceExecute { -> }
//## Uses: <unnamed>%37D7C5D10262;ClJSServiceCancel { -> }
//## Uses: <unnamed>%37EB72BC033C;ClSSInterfaz { -> }
//## Uses: <unnamed>%37EB733201B5;ClOblConfiguration { -> }
//## Uses: <unnamed>%37F391B8031C;ClStatsJSSubsystem { -> }

class M4_DECL_M4JSSUBSYSTEM ClJSSubsystem : public ClSSImplementation  //## Inherits: <unnamed>%37CEB23F0158
{
  //## begin ClJSSubsystem%36E3C96F0280.initialDeclarations preserve=yes
  //## end ClJSSubsystem%36E3C96F0280.initialDeclarations

  public:
    //## begin ClJSSubsystem::eJSSubsystemCommands%37F3AAD40116.preface preserve=yes
    //## end ClJSSubsystem::eJSSubsystemCommands%37F3AAD40116.preface

    //## Class: eJSSubsystemCommands%37F3AAD40116
    //## Category: M4JSSubsystem%35F51CAB0378
    //## Subsystem: M4JSSubsystem%35F539720231
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {JS_CANCEL_FREQUENCY=ClSSImplementation::TOTAL_COMMANDS, JS_RECOVER_FREQUENCY, JS_EXPIRATION_FREQUENCY, JS_RES_EXPIRATION_FREQUENCY, JS_BEGIN_JS, JS_END_JS, JS_BEGIN_JS_AT, JS_END_JS_AT, JS_DELETE_START_DATE, JS_DELETE_STOP_DATE, JS_ENABLE_EXCEPTIONS, JS_DISABLE_EXCEPTIONS, TOTAL_COMMANDS} eJSSubsystemCommands;

    //## begin ClJSSubsystem::eJSSubsystemCommands%37F3AAD40116.postscript preserve=yes
    //## end ClJSSubsystem::eJSSubsystemCommands%37F3AAD40116.postscript

    //## Constructors (specified)
      //## Operation: ClJSSubsystem%920912504
      ClJSSubsystem (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClJSSubsystem%936606352
      ~ClJSSubsystem ();


    //## Other Operations (specified)
      //## Operation: ExecuteCommand%920912506
      m4return_t  ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: Synchronize%920912508
      m4return_t Synchronize ();

      //## Operation: GetCommandsId%920912509
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: ShutDown%920912510
      m4return_t ShutDown ();

      //## Operation: Initialize%920912511
      m4return_t Initialize ();

      //## Operation: SetConfiguration%920912514
      void SetConfiguration (ClConfiguration *ai_poConfiguration);

      //## Operation: Stop%929441246
      m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: GenerateSubsystem%936606366
      ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: Start%936966059
      m4return_t Start ();

      //## Operation: GetStatisticAttribValue%938709943
      m4return_t GetStatisticAttribValue (m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage);

      //## Operation: GetCommandParams%938709950
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetServerName%940337545
      m4return_t GetServerName (M4ClString& ao_oServerName);

      //## Operation: IsReadyToClose%940521657
      virtual m4bool_t IsReadyToClose ();

      //## Operation: AddNewExecutor%940853234
      m4return_t AddNewExecutor ();

      //## Operation: Update%940853235
      virtual m4void_t Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: BeginJS%940940805
      m4return_t BeginJS ();

      //## Operation: EndJS%940940806
      m4return_t EndJS ();

  public:
    // Additional Public Declarations
      //## begin ClJSSubsystem%36E3C96F0280.public preserve=yes
      //## end ClJSSubsystem%36E3C96F0280.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_oStrDumpFile%36EE7CA00073
      //## begin ClJSSubsystem::m_oStrDumpFile%36EE7CA00073.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oStrDumpFile;
      //## end ClJSSubsystem::m_oStrDumpFile%36EE7CA00073.attr

      //## Attribute: m_oServerName%380C68E70102
      //## begin ClJSSubsystem::m_oServerName%380C68E70102.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServerName;
      //## end ClJSSubsystem::m_oServerName%380C68E70102.attr

    // Data Members for Associations

      //## Association: M4JSSubsystem::<unnamed>%37D8E67F0399
      //## Role: ClJSSubsystem::m_poJSAdministrator%37D8E681017F
      //## begin ClJSSubsystem::m_poJSAdministrator%37D8E681017F.role preserve=no  protected: ClJSAdministrator { -> RFHAN}
      ClJSAdministrator *m_poJSAdministrator;
      //## end ClJSSubsystem::m_poJSAdministrator%37D8E681017F.role

    // Additional Protected Declarations
      //## begin ClJSSubsystem%36E3C96F0280.protected preserve=yes
      //## end ClJSSubsystem%36E3C96F0280.protected

  private:
    // Additional Private Declarations
      //## begin ClJSSubsystem%36E3C96F0280.private preserve=yes
      //## end ClJSSubsystem%36E3C96F0280.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSSubsystem%36E3C96F0280.implementation preserve=yes
      //## end ClJSSubsystem%36E3C96F0280.implementation

};

//## begin ClJSSubsystem%36E3C96F0280.postscript preserve=yes
//## end ClJSSubsystem%36E3C96F0280.postscript

// Class ClJSSubsystem 

//## begin module%36E4009F0221.epilog preserve=yes
//## end module%36E4009F0221.epilog


#endif
