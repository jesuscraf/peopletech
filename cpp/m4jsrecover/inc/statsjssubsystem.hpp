//## begin module%382BF539026F.cm preserve=no
//## end module%382BF539026F.cm

//## begin module%382BF539026F.cp preserve=no
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
//## end module%382BF539026F.cp

//## Module: statsjssubsystem%382BF539026F; Package specification
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Source file: D:\FuentesServidor\m4jsrecover\inc\statsjssubsystem.hpp

#ifndef statsjssubsystem_h
#define statsjssubsystem_h 1

//## begin module%382BF539026F.additionalIncludes preserve=no
//## end module%382BF539026F.additionalIncludes

//## begin module%382BF539026F.includes preserve=yes
#include <m4jsrecover_dll.hpp>
//## end module%382BF539026F.includes

// Statistics
#include <statistics.hpp>
// syncro
#include <syncro.hpp>
//## begin module%382BF539026F.declarations preserve=no
//## end module%382BF539026F.declarations

//## begin module%382BF539026F.additionalDeclarations preserve=yes
//## end module%382BF539026F.additionalDeclarations


//## begin ClStatsJSSubsystem%37F391A80160.preface preserve=yes
//## end ClStatsJSSubsystem%37F391A80160.preface

//## Class: ClStatsJSSubsystem%37F391A80160
//## Category: M4JSRecover%37F8B5B70134
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSRECOVER ClStatsJSSubsystem : public ClStatistic  //## Inherits: <unnamed>%37F39A8100BD
{
  //## begin ClStatsJSSubsystem%37F391A80160.initialDeclarations preserve=yes
  //## end ClStatsJSSubsystem%37F391A80160.initialDeclarations

  public:
    //## begin ClStatsJSSubsystem::eStatsJSSubsystem%37F398A601F0.preface preserve=yes
    //## end ClStatsJSSubsystem::eStatsJSSubsystem%37F398A601F0.preface

    //## Class: eStatsJSSubsystem%37F398A601F0
    //## Category: M4JSRecover%37F8B5B70134
    //## Subsystem: M4JSRecover::inc%37F8B6990107
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { JS_NEXT_START_DATE=ClStatistic::TOTAL_NUM,  JS_NEXT_STOP_DATE,  JS_CANCEL_FREQUENCY, JS_RECOVER_FREQUENCY, JS_EXPIRED_FREQUENCY,  JS_EXPIRED_RES_FREQUENCY , JS_NJOBS_EXPIRED, JS_NRESOURCES_EXPIRED, TOTAL_NUM} eStatsJSSubsystem;

    //## begin ClStatsJSSubsystem::eStatsJSSubsystem%37F398A601F0.postscript preserve=yes
    //## end ClStatsJSSubsystem::eStatsJSSubsystem%37F398A601F0.postscript

    //## Constructors (specified)
      //## Operation: ClStatsJSSubsystem%938709938
      ClStatsJSSubsystem ();

    //## Destructor (specified)
      //## Operation: ~ClStatsJSSubsystem%938709939
      ~ClStatsJSSubsystem ();


    //## Other Operations (specified)
      //## Operation: Reset%938709940
      m4return_t Reset ();

      //## Operation: GetStat%938709941
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%938709942
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

      //## Operation: AddJobsExpired%942405296
      m4return_t AddJobsExpired (m4uint32_t ai_uiJobs);

      //## Operation: AddResExpired%942405297
      m4return_t AddResExpired (m4uint32_t ai_uiJobs);

  public:
    // Additional Public Declarations
      //## begin ClStatsJSSubsystem%37F391A80160.public preserve=yes
      //## end ClStatsJSSubsystem%37F391A80160.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiNJobsExpired%382C096B02CD
      //## begin ClStatsJSSubsystem::m_uiNJobsExpired%382C096B02CD.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiNJobsExpired;
      //## end ClStatsJSSubsystem::m_uiNJobsExpired%382C096B02CD.attr

      //## Attribute: m_uiNResExpired%382C09E80359
      //## begin ClStatsJSSubsystem::m_uiNResExpired%382C09E80359.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_uiNResExpired;
      //## end ClStatsJSSubsystem::m_uiNResExpired%382C09E80359.attr

    // Data Members for Associations

      //## Association: M4JSRecover::<unnamed>%382C0DA902B1
      //## Role: ClStatsJSSubsystem::m_oMutex%382C0DAA021C
      //## begin ClStatsJSSubsystem::m_oMutex%382C0DAA021C.role preserve=no  protected: ClMutex { -> VHAN}
      ClMutex m_oMutex;
      //## end ClStatsJSSubsystem::m_oMutex%382C0DAA021C.role

    // Additional Protected Declarations
      //## begin ClStatsJSSubsystem%37F391A80160.protected preserve=yes
      //## end ClStatsJSSubsystem%37F391A80160.protected

  private:
    // Additional Private Declarations
      //## begin ClStatsJSSubsystem%37F391A80160.private preserve=yes
      //## end ClStatsJSSubsystem%37F391A80160.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStatsJSSubsystem%37F391A80160.implementation preserve=yes
      //## end ClStatsJSSubsystem%37F391A80160.implementation

};

//## begin ClStatsJSSubsystem%37F391A80160.postscript preserve=yes
//## end ClStatsJSSubsystem%37F391A80160.postscript

// Class ClStatsJSSubsystem 

//## begin module%382BF539026F.epilog preserve=yes
//## end module%382BF539026F.epilog


#endif
