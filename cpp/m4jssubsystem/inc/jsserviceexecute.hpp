//## begin module%37D379D90314.cm preserve=no
//## end module%37D379D90314.cm

//## begin module%37D379D90314.cp preserve=no
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
//## end module%37D379D90314.cp

//## Module: JSServiceExecute%37D379D90314; Package specification
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Source file: D:\eduardoma\m4jssubsystem\inc\jsserviceexecute.hpp

#ifndef jsserviceexecute_h
#define jsserviceexecute_h 1

//## begin module%37D379D90314.additionalIncludes preserve=no
//## end module%37D379D90314.additionalIncludes

// Statistics
#include <statistics.hpp>
// JSSExecute
#include <jssexecute.hpp>
// clservicebasess
#include <clservicebasess.hpp>

class ClJSSubsystem;

//## begin module%37D379D90314.declarations preserve=no
//## end module%37D379D90314.declarations

//## begin module%37D379D90314.additionalDeclarations preserve=yes
//## end module%37D379D90314.additionalDeclarations


//## begin ClStatsJSServiceExe%37D4C1B10343.preface preserve=yes
//## end ClStatsJSServiceExe%37D4C1B10343.preface

//## Class: ClStatsJSServiceExe%37D4C1B10343
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClStatsJSServiceExe : public ClStatistic  //## Inherits: <unnamed>%37F39A9301A9
{
  //## begin ClStatsJSServiceExe%37D4C1B10343.initialDeclarations preserve=yes
  //## end ClStatsJSServiceExe%37D4C1B10343.initialDeclarations

  public:
    //## begin ClStatsJSServiceExe::eStatsJSServiceExe%37D4CA550236.preface preserve=yes
    //## end ClStatsJSServiceExe::eStatsJSServiceExe%37D4CA550236.preface

    //## Class: eStatsJSServiceExe%37D4CA550236
    //## Category: M4JSSubsystem%35F51CAB0378
    //## Subsystem: M4JSSubsystem%35F539720231
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { NUM_JOBS=ClStatistic::TOTAL_NUM  , NUM_JOBS_CANCEL, NUM_TASK_OK, NUM_TASK_ERROR, AVERAGE_TIME, AVERAGE_DELAY, USE_PERCENT, NUM_EXECUTORS_USED,TOTAL_NUM} eStatsJSServiceExe;

    //## begin ClStatsJSServiceExe::eStatsJSServiceExe%37D4CA550236.postscript preserve=yes
    //## end ClStatsJSServiceExe::eStatsJSServiceExe%37D4CA550236.postscript

    //## Constructors (specified)
      //## Operation: ClStatsJSServiceExe%936690188
      ClStatsJSServiceExe ();

    //## Destructor (specified)
      //## Operation: ~ClStatsJSServiceExe%936690189
      ~ClStatsJSServiceExe ();


    //## Other Operations (specified)
      //## Operation: Reset%936690190
      m4return_t Reset ();

      //## Operation: GetStat%936690191
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%936690192
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

  public:
    // Additional Public Declarations
      //## begin ClStatsJSServiceExe%37D4C1B10343.public preserve=yes
      //## end ClStatsJSServiceExe%37D4C1B10343.public

  protected:
    // Additional Protected Declarations
      //## begin ClStatsJSServiceExe%37D4C1B10343.protected preserve=yes
      //## end ClStatsJSServiceExe%37D4C1B10343.protected

  private:
    // Additional Private Declarations
      //## begin ClStatsJSServiceExe%37D4C1B10343.private preserve=yes
      //## end ClStatsJSServiceExe%37D4C1B10343.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStatsJSServiceExe%37D4C1B10343.implementation preserve=yes
      //## end ClStatsJSServiceExe%37D4C1B10343.implementation

};

//## begin ClStatsJSServiceExe%37D4C1B10343.postscript preserve=yes
//## end ClStatsJSServiceExe%37D4C1B10343.postscript

//## begin ClJSServiceExecute%37CEAA1B012A.preface preserve=yes
//## end ClJSServiceExecute%37CEAA1B012A.preface

//## Class: ClJSServiceExecute%37CEAA1B012A
//	Sercicio de los ejecutores de jobs. Se encarga de la creación y regeneración
//	de ejecutores.
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37D3C8BA01BC;ClJSSExecute { -> }
//## Uses: <unnamed>%37D4C1D00302;ClStatsJSServiceExe { -> }
//## Uses: <unnamed>%38329FC4016D;ClJSSubsystem { -> F}

class ClJSServiceExecute : public ClServiceBaseSS  //## Inherits: <unnamed>%37D7C304032A
{
  //## begin ClJSServiceExecute%37CEAA1B012A.initialDeclarations preserve=yes
  //## end ClJSServiceExecute%37CEAA1B012A.initialDeclarations

  public:
    //## begin ClJSServiceExecute::eJSServiceExecuteCommands%382FEEC000E0.preface preserve=yes
    //## end ClJSServiceExecute::eJSServiceExecuteCommands%382FEEC000E0.preface

    //## Class: eJSServiceExecuteCommands%382FEEC000E0
    //## Category: M4JSSubsystem%35F51CAB0378
    //## Subsystem: M4JSSubsystem%35F539720231
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {JS_CHANGE_TIMEOUT = ClServiceBaseSS::TOTAL_COMMANDS, JS_DELETE_TIMEOUT, TOTAL_COMMANDS} eJSServiceExecuteCommands;

    //## begin ClJSServiceExecute::eJSServiceExecuteCommands%382FEEC000E0.postscript preserve=yes
    //## end ClJSServiceExecute::eJSServiceExecuteCommands%382FEEC000E0.postscript

    //## Constructors (specified)
      //## Operation: ClJSServiceExecute%936606353
      ClJSServiceExecute (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClJSServiceExecute%939986156
      ~ClJSServiceExecute ();


    //## Other Operations (specified)
      //## Operation: Initialize%936606354
      m4return_t Initialize ();

      //## Operation: GetStatisticAttribValue%936606355
      m4return_t GetStatisticAttribValue (m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage);

      //## Operation: ExecuteCommand%936606356
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%936606357
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%936606358
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GenerateSubsystem%936606359
      ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: Stop%937812722
      m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: SetDumpFile%940237696
      m4return_t SetDumpFile (m4pchar_t ai_pcDumpFile);

      //## Operation: SetIdentification%942840133
      m4return_t SetIdentification (M4ClString& ai_ServerName, M4ClString& ai_ServiceName);

  public:
    // Additional Public Declarations
      //## begin ClJSServiceExecute%37CEAA1B012A.public preserve=yes
	  void SetOutputQ ( ClOutputQ * aiOutputQ ) 
	  {
		  m_poOutputQ = aiOutputQ ;
	  }
      //## end ClJSServiceExecute%37CEAA1B012A.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_pcDumpFile%380AE5E7037D
      //## begin ClJSServiceExecute::m_pcDumpFile%380AE5E7037D.attr preserve=no  protected: m4pchar_t {UA} 
      m4pchar_t m_pcDumpFile;
      //## end ClJSServiceExecute::m_pcDumpFile%380AE5E7037D.attr

      //## Attribute: m_oServerName%3832CDA1017A
      //## begin ClJSServiceExecute::m_oServerName%3832CDA1017A.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServerName;
      //## end ClJSServiceExecute::m_oServerName%3832CDA1017A.attr

      //## Attribute: m_oServiceName%3832CDA80011
      //## begin ClJSServiceExecute::m_oServiceName%3832CDA80011.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServiceName;
      //## end ClJSServiceExecute::m_oServiceName%3832CDA80011.attr

    // Additional Protected Declarations
      //## begin ClJSServiceExecute%37CEAA1B012A.protected preserve=yes
      //## end ClJSServiceExecute%37CEAA1B012A.protected

  private:
    // Additional Private Declarations
      //## begin ClJSServiceExecute%37CEAA1B012A.private preserve=yes
	  ClOutputQ * m_poOutputQ ;
      //## end ClJSServiceExecute%37CEAA1B012A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSServiceExecute%37CEAA1B012A.implementation preserve=yes
      //## end ClJSServiceExecute%37CEAA1B012A.implementation

};

//## begin ClJSServiceExecute%37CEAA1B012A.postscript preserve=yes
//## end ClJSServiceExecute%37CEAA1B012A.postscript

// Class ClStatsJSServiceExe 

// Class ClJSServiceExecute 

//## begin module%37D379D90314.epilog preserve=yes
//## end module%37D379D90314.epilog


#endif


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClJSServiceExecute::SetOutputQueue%939986154.body preserve=no
	SetServiceQ((ClServiceQ *)ai_pJSQueue);
	return M4_SUCCESS;
//## end ClJSServiceExecute::SetOutputQueue%939986154.body

//## begin ClJSServiceExecute::SetInputQueue%939986155.body preserve=no
	SetOutputQ((ClOutputQ *)ai_pJSQueue);
	return M4_SUCCESS;
//## end ClJSServiceExecute::SetInputQueue%939986155.body

#endif
