//## begin module%37FB0CA700B1.cm preserve=no
//## end module%37FB0CA700B1.cm

//## begin module%37FB0CA700B1.cp preserve=no
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
//## end module%37FB0CA700B1.cp

//## Module: servicestats%37FB0CA700B1; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: F:\integration\m4subsystems\inc\servicestats.hpp

#ifndef servicestats_h
#define servicestats_h 1

//## begin module%37FB0CA700B1.additionalIncludes preserve=no
//## end module%37FB0CA700B1.additionalIncludes

//## begin module%37FB0CA700B1.includes preserve=yes
#include <m4subsystems_dll.hpp>
#include <clssinterfaz.hpp>
//## end module%37FB0CA700B1.includes

// Statistics
#include <statistics.hpp>
// LauncherInterface
#include <launcher.hpp>

class ClServiceInterface;
class ClServiceBaseSS;

//## begin module%37FB0CA700B1.declarations preserve=no
//## end module%37FB0CA700B1.declarations

//## begin module%37FB0CA700B1.additionalDeclarations preserve=yes
#define SERVICE_DEF_STRING_SIZE  50
//## end module%37FB0CA700B1.additionalDeclarations


//## begin ClServiceStats%35050300034E.preface preserve=yes
//## end ClServiceStats%35050300034E.preface

//## Class: ClServiceStats%35050300034E
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37442FEC033F;ClRequest { -> F}

class M4_DECL_M4SUBSYSTEMS ClServiceStats : public ClLauncherStats  //## Inherits: <unnamed>%37FCA9120205
{
  //## begin ClServiceStats%35050300034E.initialDeclarations preserve=yes
  //## end ClServiceStats%35050300034E.initialDeclarations

  public:
    //## begin ClServiceStats::eServiceStatsId%35050828011C.preface preserve=yes
    //## end ClServiceStats::eServiceStatsId%35050828011C.preface

    //## Class: eServiceStatsId%35050828011C
    //## Category: M4Subsystems%37D39FC60059
    //## Subsystem: M4Subsystems::inc%37D3A21301DC
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {QUEUED_ITEMS = ClLauncherStats::TOTAL_NUM, FIV_SEC_QUEUED_ITEMS, THIR_SEC_QUEUED_ITEMS, FIV_MIN_QUEUED_ITEMS , TOTAL_NUM} eServiceStatsId;

    //## begin ClServiceStats::eServiceStatsId%35050828011C.postscript preserve=yes
    //## end ClServiceStats::eServiceStatsId%35050828011C.postscript

    //## Constructors (specified)
      //## Operation: ClServiceStats%889521621; C++
      ClServiceStats (ClServiceBaseSS *ai_pServiceSS = NULL);

    //## Destructor (specified)
      //## Operation: ~ClServiceStats%889521622
      ~ClServiceStats ();


    //## Other Operations (specified)
      //## Operation: SetQueuedItems%889521620
      m4void_t SetQueuedItems (ClStatNumericalType ai_uNewValue);

      //## Operation: Reset%889521626
      m4return_t Reset ();

      //## Operation: ResetStat%889521627
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

      //## Operation: GetStat%889521628
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_oValue);

      //## Operation: GetAvgQueuedItems%937815750
      ClStatNumericalType GetAvgQueuedItems ();

      //## Operation: AddnQueuedItems%937815752
      void AddnQueuedItems (ClStatNumericalType ai_lTransactions);

    // Data Members for Class Attributes

      //## Attribute: m_fTPSFiveSec%350E4DD103DB
      //## begin ClServiceStats::m_fTPSFiveSec%350E4DD103DB.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fTPSFiveSec;
      //## end ClServiceStats::m_fTPSFiveSec%350E4DD103DB.attr

      //## Attribute: m_fTPSThirtySec%38352733007D
      //## begin ClServiceStats::m_fTPSThirtySec%38352733007D.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fTPSThirtySec;
      //## end ClServiceStats::m_fTPSThirtySec%38352733007D.attr

      //## Attribute: m_fTPSFiveMin%38352738035F
      //## begin ClServiceStats::m_fTPSFiveMin%38352738035F.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fTPSFiveMin;
      //## end ClServiceStats::m_fTPSFiveMin%38352738035F.attr

      //## Attribute: m_fAvgExecFiveSec%350E4ED20019
      //## begin ClServiceStats::m_fAvgExecFiveSec%350E4ED20019.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fAvgExecFiveSec;
      //## end ClServiceStats::m_fAvgExecFiveSec%350E4ED20019.attr

      //## Attribute: m_fAvgExecThirtySec%3835274C01CE
      //## begin ClServiceStats::m_fAvgExecThirtySec%3835274C01CE.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fAvgExecThirtySec;
      //## end ClServiceStats::m_fAvgExecThirtySec%3835274C01CE.attr

      //## Attribute: m_fAvgExecFiveMin%383527530047
      //## begin ClServiceStats::m_fAvgExecFiveMin%383527530047.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fAvgExecFiveMin;
      //## end ClServiceStats::m_fAvgExecFiveMin%383527530047.attr

      //## Attribute: m_fWaitTimeFiveSec%350E51C8033A
      //## begin ClServiceStats::m_fWaitTimeFiveSec%350E51C8033A.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fWaitTimeFiveSec;
      //## end ClServiceStats::m_fWaitTimeFiveSec%350E51C8033A.attr

      //## Attribute: m_fWaitTimeThirtySec%3835276202E8
      //## begin ClServiceStats::m_fWaitTimeThirtySec%3835276202E8.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fWaitTimeThirtySec;
      //## end ClServiceStats::m_fWaitTimeThirtySec%3835276202E8.attr

      //## Attribute: m_fWaitTimeFiveMin%3835276501AB
      //## begin ClServiceStats::m_fWaitTimeFiveMin%3835276501AB.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fWaitTimeFiveMin;
      //## end ClServiceStats::m_fWaitTimeFiveMin%3835276501AB.attr

      //## Attribute: m_fSSUseFiveSec%37E5F7DA02CF
      //## begin ClServiceStats::m_fSSUseFiveSec%37E5F7DA02CF.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fSSUseFiveSec;
      //## end ClServiceStats::m_fSSUseFiveSec%37E5F7DA02CF.attr

      //## Attribute: m_fSSUseThirtySec%3835278301B9
      //## begin ClServiceStats::m_fSSUseThirtySec%3835278301B9.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fSSUseThirtySec;
      //## end ClServiceStats::m_fSSUseThirtySec%3835278301B9.attr

      //## Attribute: m_fSSUseFiveMin%3835278503E2
      //## begin ClServiceStats::m_fSSUseFiveMin%3835278503E2.attr preserve=no  public: ClStatPercentType {UA} 0
      ClStatPercentType m_fSSUseFiveMin;
      //## end ClServiceStats::m_fSSUseFiveMin%3835278503E2.attr

  public:
    // Additional Public Declarations
      //## begin ClServiceStats%35050300034E.public preserve=yes
      //## end ClServiceStats%35050300034E.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uQueuedItems%350503F301A9
      //## begin ClServiceStats::m_uQueuedItems%350503F301A9.attr preserve=no  protected: ClStatNumericalType {UA} 0
      ClStatNumericalType m_uQueuedItems;
      //## end ClServiceStats::m_uQueuedItems%350503F301A9.attr

    // Additional Protected Declarations
      //## begin ClServiceStats%35050300034E.protected preserve=yes
      //## end ClServiceStats%35050300034E.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_AvgQueuedItems%37E5F86E0278
      //## begin ClServiceStats::m_AvgQueuedItems%37E5F86E0278.attr preserve=no  private: ClStatAverageContainer {UA} 
      ClStatAverageContainer m_AvgQueuedItems;
      //## end ClServiceStats::m_AvgQueuedItems%37E5F86E0278.attr

    // Additional Private Declarations
      //## begin ClServiceStats%35050300034E.private preserve=yes
      //## end ClServiceStats%35050300034E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServiceStats%35050300034E.implementation preserve=yes
      //## end ClServiceStats%35050300034E.implementation

};

//## begin ClServiceStats%35050300034E.postscript preserve=yes
//## end ClServiceStats%35050300034E.postscript

//## Class: ClServiceVisitor%344F543C0115
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3505188C02A6;ClServiceStats { -> }
//## Uses: <unnamed>%37FB17920024;ClServiceInterface { -> F}
//## Uses: <unnamed>%37FCC0CA0039;ClServiceBaseSS { -> F}

class M4_DECL_M4SUBSYSTEMS ClServiceVisitor : public ClVisitor  //## Inherits: <unnamed>%344F544703E2
{
  public:

    //## Other Operations (specified)
      //## Operation: VisitService%878029379
      void VisitService (ClSSInterfaz &ai_oServiceSS, ClServiceInterface& ai_oService);

  protected:

    //## Other Operations (specified)
      //## Operation: _GetNumericalValue%943003259; C++
      ClStatNumericalType _GetNumericalValue (ClSSInterfaz &ai_oSubsystem, m4uint_t ai_iStatId);

      //## Operation: _GetPercentValue%943003260; C++
      ClStatPercentType _GetPercentValue (ClSSInterfaz &ai_oSubsystem, m4uint_t ai_iStatId);

  private:
  private: //## implementation
};

// Class ClServiceStats 

// Class ClServiceVisitor 

//## begin module%37FB0CA700B1.epilog preserve=yes
//## end module%37FB0CA700B1.epilog


#endif
