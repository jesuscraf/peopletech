//## begin module%3459D95C0041.cm preserve=no
//## end module%3459D95C0041.cm

//## begin module%3459D95C0041.cp preserve=no
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
//## end module%3459D95C0041.cp

//## Module: LMStats%3459D95C0041; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\lmstats.hpp

#ifndef lmstats_h
#define lmstats_h 1

//## begin module%3459D95C0041.additionalIncludes preserve=no
//## end module%3459D95C0041.additionalIncludes

//## begin module%3459D95C0041.includes preserve=yes
//## end module%3459D95C0041.includes

// Statistics
#include <statistics.hpp>
// Request
#include <request.hpp>

class ClLocalMonitor;

//## begin module%3459D95C0041.declarations preserve=no
//## end module%3459D95C0041.declarations

//## begin module%3459D95C0041.additionalDeclarations preserve=yes
#define LM_DEF_STRING_SIZE	50
//## end module%3459D95C0041.additionalDeclarations


//## Class: ClLMVisitor%345872230390
//	Base class for Local moitor visitors
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3458766300CD;friend ClLocalMonitor { -> F}
//## Uses: <unnamed>%3781F293033B;ClRequest { -> F}
//## Uses: <unnamed>%3781F388017B;ClLogicalService { -> F}

class ClLMVisitor : public ClVisitor  //## Inherits: <unnamed>%3459D28E0227
{
  public:
    //## Constructors (specified)
      //## Operation: ClLMVisitor%879606157
      ClLMVisitor ();

    //## Destructor (specified)
      //## Operation: ~ClLMVisitor%878302313
      ~ClLMVisitor ();


    //## Other Operations (specified)
      //## Operation: VisitLM%878200307
      virtual void VisitLM (ClLocalMonitor &ai_oLocalMonitor);

  protected:
  private:
  private: //## implementation
};

//## begin ClLMStats%3459D7B40092.preface preserve=yes
//## end ClLMStats%3459D7B40092.preface

//## Class: ClLMStats%3459D7B40092
//	Base class for Local Monitor Statistics. This class has a pool of common
//	statistics both for administration and supervision.
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%350D0E610341;ClStatAverageContainer { -> }
//## Uses: <unnamed>%36A4825E000D;ClPDU { -> F}
//## Uses: <unnamed>%38159C00013B;ClRequest { -> }
//## Uses: <unnamed>%3815B3A202FE;ClRequestStatistics { -> }

class ClLMStats : public ClStatistic  //## Inherits: <unnamed>%3459D8F503B4
{
  //## begin ClLMStats%3459D7B40092.initialDeclarations preserve=yes
  //## end ClLMStats%3459D7B40092.initialDeclarations

  public:
    //## begin ClLMStats::eLocalMonitorStatsID%34DEE3A30335.preface preserve=yes
    //## end ClLMStats::eLocalMonitorStatsID%34DEE3A30335.preface

    //## Class: eLocalMonitorStatsID%34DEE3A30335
    //## Category: LocalMonitor%379EC99A01CC
    //## Subsystem: version::inc%379ECBD70299
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { PROCESSED_REQUEST = ClStatistic::TOTAL_NUM ,  FIF_M_AVG_REC_TIME, FIV_S_AVG_REC_TIME, THIR_S_AVG_REC_TIME, FIV_M_AVG_REC_TIME, FIF_M_AVG_SEND_TIME, FIV_S_AVG_SEND_TIME, THIR_S_AVG_SEND_TIME, FIV_M_AVG_SEND_TIME, FIF_M_AVG_TOTAL_TIME, FIV_S_AVG_TOTAL_TIME, THIR_S_AVG_TOTAL_TIME, FIV_M_AVG_TOTAL_TIME, FIF_M_AVG_REG_TIME, FIV_S_AVG_REG_TIME, THIR_S_AVG_REG_TIME, FIV_M_AVG_REG_TIME , FIF_M_AVG_DEREG_TIME, FIV_S_AVG_DEREG_TIME, THIR_S_AVG_DEREG_TIME, FIV_M_AVG_DEREG_TIME ,FIF_M_AVG_GETSESS_TIME, FIV_S_AVG_GETSESS_TIME, THIR_S_AVG_GETSESS_TIME, FIV_M_AVG_GETSESS_TIME , FIF_M_AVG_INPUT_BYTES, FIV_S_AVG_INPUT_BYTES, THIR_S_AVG_INPUT_BYTES, FIV_M_AVG_INPUT_BYTES ,FIF_M_AVG_OUTPUT_BYTES, FIV_S_AVG_OUTPUT_BYTES, THIR_S_AVG_OUTPUT_BYTES, FIV_M_AVG_OUTPUT_BYTES , FIF_M_AVG_PROCESSED, FIV_S_AVG_PROCESSED, THIR_S_AVG_PROCESSED, FIV_M_AVG_PROCESSED , NUM_CONNECTION, TOTAL_NUM } eLocalMonitorStatsID;

    //## begin ClLMStats::eLocalMonitorStatsID%34DEE3A30335.postscript preserve=yes
    //## end ClLMStats::eLocalMonitorStatsID%34DEE3A30335.postscript

    //## Constructors (specified)
      //## Operation: ClLMStats%887021576
      ClLMStats (m4bool_t ai_bOuttoFile = M4_FALSE);

    //## Destructor (specified)
      //## Operation: ~ClLMStats%887021577
      ~ClLMStats ();


    //## Other Operations (specified)
      //## Operation: Reset%879417135
      m4return_t Reset ();

      //## Operation: GetStat%886498831
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_oValue);

      //## Operation: SetStat%886498832
      m4return_t SetStat (ClStatHandle &ai_hStatId, const m4pchar_t ao_pszValue);

      //## Operation: ResetStat%886498833
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

      //## Operation: AddProcessedRequests%890061476
      m4void_t AddProcessedRequests (m4uint32_t ai_NewValue);

      //## Operation: AddAvgRecTime%983982351
      m4void_t AddAvgRecTime (ClStatAverageType ai_NewValue);

      //## Operation: AddAvgSendTime%983982352
      m4void_t AddAvgSendTime (ClStatAverageType ai_NewValue);

      //## Operation: AddAvgRegTime%983982353
      m4void_t AddAvgRegTime (ClStatAverageType ai_NewValue);

      //## Operation: AddAvgGetSessTime%983982354
      m4void_t AddAvgGetSessTime (ClStatAverageType ai_NewValue);

      //## Operation: AddAvgDeRegTime%983982355
      m4void_t AddAvgDeRegTime (ClStatAverageType ai_NewValue);

      //## Operation: AddAvgTotalTime%983982356
      m4void_t AddAvgTotalTime (ClStatAverageType ai_NewValue);

      //## Operation: AddAvgInputBytes%983982357
      m4void_t AddAvgInputBytes (ClStatAverageType ai_NewValue);

      //## Operation: AddAvgOutputBytes%983982358
      m4void_t AddAvgOutputBytes (ClStatAverageType ai_NewValue);

      //## Operation: AddAvgProcessedReq%983982359
      m4void_t AddAvgProcessedReq (ClStatAverageType ai_NewValue);

      //## Operation: AddInputBytes%983982360
      m4void_t AddInputBytes (m4int32_t ai_NewValue);

      //## Operation: AddOutputBytes%983982361
      m4void_t AddOutputBytes (m4int32_t ai_NewValue);

      //## Operation: GetProcessedReq%983982362
      m4uint32_t GetProcessedReq ();

      //## Operation: GetLastInputBytes%983982363
      m4uint32_t GetLastInputBytes ();

      //## Operation: GetLastOutputBytes%983982364
      m4uint32_t GetLastOutputBytes ();

      //## Operation: Computer%984138917
      m4return_t Computer (ClRequest *ai_poRequest);

      //## Operation: AddRecTime%984138918
      m4void_t AddRecTime (ClStatTimeType ai_iTime);

      //## Operation: AddSendTime%984138919
      m4void_t AddSendTime (ClStatTimeType ai_iTime);

      //## Operation: AddRegTime%984138920
      m4void_t AddRegTime (ClStatTimeType ai_iTime);

      //## Operation: AddDeRegTime%984138921
      m4void_t AddDeRegTime (ClStatTimeType ai_iTime);

      //## Operation: AddGetSessTime%984138922
      m4void_t AddGetSessTime (ClStatTimeType ai_iTime);

      //## Operation: AddTotalTime%984138923
      m4void_t AddTotalTime (ClStatTimeType ai_iTime);

      //## Operation: GetLastRecTime%984138924
      ClStatTimeType GetLastRecTime ();

      //## Operation: GetLastSendTime%984138925
      ClStatTimeType GetLastSendTime ();

      //## Operation: GetLastRegTime%984138926
      ClStatTimeType GetLastRegTime ();

      //## Operation: GetLastDeRegTime%984138927
      ClStatTimeType GetLastDeRegTime ();

      //## Operation: GetLastGetSessTime%984138928
      ClStatTimeType GetLastGetSessTime ();

      //## Operation: GetLastTotalTime%984138929
      ClStatTimeType GetLastTotalTime ();

      //## Operation: AddNewConnections%993565383
      m4void_t AddNewConnections (m4int32_t ai_iNumConn);

      //## Operation: GetNumConn%993565384
      m4uint32_t GetNumConn ();

  public:
    // Additional Public Declarations
      //## begin ClLMStats%3459D7B40092.public preserve=yes
      //## end ClLMStats%3459D7B40092.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_poRequestOutputFile%381045E303D1
      //## begin ClLMStats::m_poRequestOutputFile%381045E303D1.attr preserve=no  protected: m4pchar_t {UA} 
      m4pchar_t m_poRequestOutputFile;
      //## end ClLMStats::m_poRequestOutputFile%381045E303D1.attr

    // Data Members for Associations

      //## Association: LocalMonitor::<unnamed>%3AA653CB01BE
      //## Role: ClLMStats::m_oAvgRecTime%3AA653CB0364
      //## begin ClLMStats::m_oAvgRecTime%3AA653CB0364.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgRecTime;
      //## end ClLMStats::m_oAvgRecTime%3AA653CB0364.role

      //## Association: LocalMonitor::<unnamed>%3AA653FF03A4
      //## Role: ClLMStats::m_oAvgSendTime%3AA654000279
      //## begin ClLMStats::m_oAvgSendTime%3AA654000279.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgSendTime;
      //## end ClLMStats::m_oAvgSendTime%3AA654000279.role

      //## Association: LocalMonitor::<unnamed>%3AA654280371
      //## Role: ClLMStats::m_oAvgRegTime%3AA65429016A
      //## begin ClLMStats::m_oAvgRegTime%3AA65429016A.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgRegTime;
      //## end ClLMStats::m_oAvgRegTime%3AA65429016A.role

      //## Association: LocalMonitor::<unnamed>%3AA6544A009E
      //## Role: ClLMStats::m_oAvgDeRegTime%3AA6544A02C6
      //## begin ClLMStats::m_oAvgDeRegTime%3AA6544A02C6.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgDeRegTime;
      //## end ClLMStats::m_oAvgDeRegTime%3AA6544A02C6.role

      //## Association: LocalMonitor::<unnamed>%3AA6546C00BB
      //## Role: ClLMStats::m_oAvgGetSessTime%3AA6546C02EC
      //## begin ClLMStats::m_oAvgGetSessTime%3AA6546C02EC.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgGetSessTime;
      //## end ClLMStats::m_oAvgGetSessTime%3AA6546C02EC.role

      //## Association: LocalMonitor::<unnamed>%3AA6554701C5
      //## Role: ClLMStats::m_oAvgTotalTime%3AA6554800D7
      //## begin ClLMStats::m_oAvgTotalTime%3AA6554800D7.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgTotalTime;
      //## end ClLMStats::m_oAvgTotalTime%3AA6554800D7.role

      //## Association: LocalMonitor::<unnamed>%3AA6562902A6
      //## Role: ClLMStats::m_oAvgInputBytes%3AA6562A00E5
      //## begin ClLMStats::m_oAvgInputBytes%3AA6562A00E5.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgInputBytes;
      //## end ClLMStats::m_oAvgInputBytes%3AA6562A00E5.role

      //## Association: LocalMonitor::<unnamed>%3AA6564902F2
      //## Role: ClLMStats::m_oAvgOutputBytes%3AA6564A0164
      //## begin ClLMStats::m_oAvgOutputBytes%3AA6564A0164.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgOutputBytes;
      //## end ClLMStats::m_oAvgOutputBytes%3AA6564A0164.role

      //## Association: LocalMonitor::<unnamed>%3AA657920165
      //## Role: ClLMStats::m_oAvgProcessedReq%3AA657930076
      //## begin ClLMStats::m_oAvgProcessedReq%3AA657930076.role preserve=no  protected: ClStatAverageContainer { -> VHAN}
      ClStatAverageContainer m_oAvgProcessedReq;
      //## end ClLMStats::m_oAvgProcessedReq%3AA657930076.role

    // Additional Protected Declarations
      //## begin ClLMStats%3459D7B40092.protected preserve=yes
      //## end ClLMStats%3459D7B40092.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oTotalRequests%350D15460136
      //## begin ClLMStats::m_oTotalRequests%350D15460136.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_oTotalRequests;
      //## end ClLMStats::m_oTotalRequests%350D15460136.attr

      //## Attribute: m_bOuttoFile%35178CAF00D1
      //## begin ClLMStats::m_bOuttoFile%35178CAF00D1.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bOuttoFile;
      //## end ClLMStats::m_bOuttoFile%35178CAF00D1.attr

      //## Attribute: m_oLastInputBytes%3AA65FFC034D
      //## begin ClLMStats::m_oLastInputBytes%3AA65FFC034D.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_oLastInputBytes;
      //## end ClLMStats::m_oLastInputBytes%3AA65FFC034D.attr

      //## Attribute: m_oLastOutputBytes%3AA65FFD03BC
      //## begin ClLMStats::m_oLastOutputBytes%3AA65FFD03BC.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_oLastOutputBytes;
      //## end ClLMStats::m_oLastOutputBytes%3AA65FFD03BC.attr

      //## Attribute: m_oLastRecTime%3AA8C68903E7
      //## begin ClLMStats::m_oLastRecTime%3AA8C68903E7.attr preserve=no  private: ClStatTimeType {UA} 
      ClStatTimeType m_oLastRecTime;
      //## end ClLMStats::m_oLastRecTime%3AA8C68903E7.attr

      //## Attribute: m_oLastSendTime%3AA8C6A10360
      //## begin ClLMStats::m_oLastSendTime%3AA8C6A10360.attr preserve=no  private: ClStatTimeType {UA} 
      ClStatTimeType m_oLastSendTime;
      //## end ClLMStats::m_oLastSendTime%3AA8C6A10360.attr

      //## Attribute: m_oLastRegTime%3AA8C6A30100
      //## begin ClLMStats::m_oLastRegTime%3AA8C6A30100.attr preserve=no  private: ClStatTimeType {UA} 
      ClStatTimeType m_oLastRegTime;
      //## end ClLMStats::m_oLastRegTime%3AA8C6A30100.attr

      //## Attribute: m_oLastDeRegTime%3AA8C6A40183
      //## begin ClLMStats::m_oLastDeRegTime%3AA8C6A40183.attr preserve=no  private: ClStatTimeType {UA} 
      ClStatTimeType m_oLastDeRegTime;
      //## end ClLMStats::m_oLastDeRegTime%3AA8C6A40183.attr

      //## Attribute: m_oLastGetSessTime%3AA8C6A501E9
      //## begin ClLMStats::m_oLastGetSessTime%3AA8C6A501E9.attr preserve=no  private: ClStatTimeType {UA} 
      ClStatTimeType m_oLastGetSessTime;
      //## end ClLMStats::m_oLastGetSessTime%3AA8C6A501E9.attr

      //## Attribute: m_oLastTotalTime%3AA8C6A90339
      //## begin ClLMStats::m_oLastTotalTime%3AA8C6A90339.attr preserve=no  private: ClStatTimeType {UA} 
      ClStatTimeType m_oLastTotalTime;
      //## end ClLMStats::m_oLastTotalTime%3AA8C6A90339.attr

      //## Attribute: m_iNumConnections%3B389B1101BF
      //## begin ClLMStats::m_iNumConnections%3B389B1101BF.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iNumConnections;
      //## end ClLMStats::m_iNumConnections%3B389B1101BF.attr

    // Additional Private Declarations
      //## begin ClLMStats%3459D7B40092.private preserve=yes
      //## end ClLMStats%3459D7B40092.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLMStats%3459D7B40092.implementation preserve=yes
      //## end ClLMStats%3459D7B40092.implementation

};

//## begin ClLMStats%3459D7B40092.postscript preserve=yes
//## end ClLMStats%3459D7B40092.postscript

//## begin ClLMGBVisitor%37CCEF680351.preface preserve=yes
//## end ClLMGBVisitor%37CCEF680351.preface

//## Class: ClLMGBVisitor%37CCEF680351
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClLMGBVisitor : public ClLMVisitor  //## Inherits: <unnamed>%37CCEF7D018E
{
  //## begin ClLMGBVisitor%37CCEF680351.initialDeclarations preserve=yes
  //## end ClLMGBVisitor%37CCEF680351.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLMGBVisitor%936177779
      ClLMGBVisitor ();

    //## Destructor (specified)
      //## Operation: ~ClLMGBVisitor%936177780
      ~ClLMGBVisitor ();


    //## Other Operations (specified)
      //## Operation: VisitLM%936177781
      void VisitLM (ClLocalMonitor &ai_oLocalMonitor);

    // Additional Public Declarations
      //## begin ClLMGBVisitor%37CCEF680351.public preserve=yes
      //## end ClLMGBVisitor%37CCEF680351.public

  protected:
    // Additional Protected Declarations
      //## begin ClLMGBVisitor%37CCEF680351.protected preserve=yes
      //## end ClLMGBVisitor%37CCEF680351.protected

  private:
    // Additional Private Declarations
      //## begin ClLMGBVisitor%37CCEF680351.private preserve=yes
      //## end ClLMGBVisitor%37CCEF680351.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLMGBVisitor%37CCEF680351.implementation preserve=yes
      //## end ClLMGBVisitor%37CCEF680351.implementation

};

//## begin ClLMGBVisitor%37CCEF680351.postscript preserve=yes
//## end ClLMGBVisitor%37CCEF680351.postscript

// Class ClLMVisitor 

// Class ClLMStats 

// Class ClLMGBVisitor 

//## begin module%3459D95C0041.epilog preserve=yes
//## end module%3459D95C0041.epilog


#endif
