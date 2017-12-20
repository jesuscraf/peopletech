//## begin module%37E244EB00C1.cm preserve=no
//## end module%37E244EB00C1.cm

//## begin module%37E244EB00C1.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:           M4JSJob
//	 File:                  jsjob.hpp
//	 Project:            Job Scheduler
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    Include file for the class ClJSJob
//
//	=============================================================================
//	=
//## end module%37E244EB00C1.cp

//## Module: jsjob%37E244EB00C1; Package specification
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Source file: C:\Trabajo\Server\m4jsexejob\inc\jsjob.hpp

#ifndef jsjob_h
#define jsjob_h 1

//## begin module%37E244EB00C1.additionalIncludes preserve=no
//## end module%37E244EB00C1.additionalIncludes

//## begin module%37E244EB00C1.includes preserve=yes
#include "m4stl.hpp"
#include "m4types.hpp"
#include <jsparameterconnector.hpp>
//## end module%37E244EB00C1.includes

// m4date
#include <m4date.hpp>
// jsexecutionstrategy
#include <jsexecutionstrategy.hpp>
// jssessionsettings
#include <jssessionsettings.hpp>
// m4jsjob_dll
#include <m4jsexejob_dll.hpp>
// jsparameter
#include <jsparameter.hpp>
//## begin module%37E244EB00C1.declarations preserve=no
//## end module%37E244EB00C1.declarations

//## begin module%37E244EB00C1.additionalDeclarations preserve=yes
//## end module%37E244EB00C1.additionalDeclarations


//## begin ParameterList%37A01C2F027B.preface preserve=yes
//## end ParameterList%37A01C2F027B.preface

//## Class: ParameterList%37A01C2F027B; Instantiated Class
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37A01C42020A;ClJSParameter { -> }

typedef map< M4ClString,ClJSParameter *,less < M4ClString > > ParameterList;

//## begin ParameterList%37A01C2F027B.postscript preserve=yes
//## end ParameterList%37A01C2F027B.postscript

//## begin ClJSJob%375BE73302C5.preface preserve=yes
//## end ClJSJob%375BE73302C5.preface

//## Class: ClJSJob%375BE73302C5; Abstract
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSJob 
{
  //## begin ClJSJob%375BE73302C5.initialDeclarations preserve=yes
  //## end ClJSJob%375BE73302C5.initialDeclarations

  public:
    //## begin ClJSJob::eJobStatus%37F486520152.preface preserve=yes
    //## end ClJSJob::eJobStatus%37F486520152.preface

    //## Class: eJobStatus%37F486520152
    //## Category: M4JSExeJob::M4JSJob%379F03BD0354
    //## Subsystem: M4JSExeJob::m4jsjob%37E244C503BF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { StatusWaiting, StatusExecuting, StatusExecuted, StatusExecutionError } eJobStatus;

    //## begin ClJSJob::eJobStatus%37F486520152.postscript preserve=yes
    //## end ClJSJob::eJobStatus%37F486520152.postscript

    //## begin ClJSJob::eInterruptRequestLevel%37F486A80160.preface preserve=yes
    //## end ClJSJob::eInterruptRequestLevel%37F486A80160.preface

    //## Class: eInterruptRequestLevel%37F486A80160
    //## Category: M4JSExeJob::M4JSJob%379F03BD0354
    //## Subsystem: M4JSExeJob::m4jsjob%37E244C503BF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { IRLNone, IRL_IRSU, IRL_IRSS, IRL_INSU, IRL_INSS} eInterruptRequestLevel;

    //## begin ClJSJob::eInterruptRequestLevel%37F486A80160.postscript preserve=yes
    //## end ClJSJob::eInterruptRequestLevel%37F486A80160.postscript

    //## Constructors (specified)
      //## Operation: ClJSJob%938164381
      ClJSJob ();

    //## Destructor (specified)
      //## Operation: ~ClJSJob%938600374
      virtual ~ClJSJob ();


    //## Other Operations (specified)
      //## Operation: SetParent%938073480
      m4return_t SetParent (ClJSJob *ai_poParent = NULL);

      //## Operation: GetParent%938073481
      ClJSJob * GetParent ();

      //## Operation: GetId%938164382
      m4return_t GetId (m4uint32_t &ao_uiId);

      //## Operation: SetId%938164386
      m4return_t SetId (const m4uint32_t &ai_uiId);

      //## Operation: SetExecutionId%938164389
      m4return_t SetExecutionId (const m4uint32_t &ai_uiIdExecution);

      //## Operation: GetExecutionId%938164385
      m4return_t GetExecutionId (m4uint32_t &ao_uiIdExecution);

      //## Operation: SetLocalId%938164388
      m4return_t SetLocalId (const m4uint32_t &ai_uiLocalId);

      //## Operation: GetLocalId%938164384
      m4return_t GetLocalId (m4uint32_t &ao_uiLocalId);

      //## Operation: SetSchedJobId%938164387
      m4return_t SetSchedJobId (const m4uint32_t &ai_uiIdSchedJob);

      //## Operation: GetSchedJobId%938164383
      m4return_t GetSchedJobId (m4uint32_t &ao_uiIdSchedJob);

      //## Operation: SetScheduledTime%938073482
      m4return_t SetScheduledTime (const M4ClTimeStamp &ai_dtScheduledTime);

      //## Operation: GetScheduledTime%938073483
      m4return_t GetScheduledTime (M4ClTimeStamp &ao_dtScheduledTime);

      //## Operation: CalculateDepthLevel%938424584
      m4int_t CalculateDepthLevel (m4int_t &aio_uiDepthLevel);

      //## Operation: GetNumberOfTasks%938517342
      virtual m4uint32_t GetNumberOfTasks ();

      //## Operation: PrintOn%938517346
      virtual void PrintOn (ostream &ai_ostream) = 0;

      //## Operation: operator<<%938517345
      friend ostream& operator << (ostream &aio_ostream, ClJSJob &ai_oJob);

      //## Operation: SetStatus%938600376
      virtual m4return_t SetStatus (const eJobStatus &ai_eStatus);

      //## Operation: GetStatus%938600377
      m4return_t GetStatus (eJobStatus &ao_eStatus);

      //## Operation: SetInterruptionRequestLevel%938600378
      m4return_t SetInterruptionRequestLevel (const eInterruptRequestLevel &ai_eIRLevel);

      //## Operation: GetInterruptionRequestLevel%938600379
      m4return_t GetInterruptionRequestLevel (eInterruptRequestLevel &ao_eIRLevel);

      //## Operation: GetNextExecutableTask%938600382
      virtual ClJSJob * GetNextExecutableTask () = 0;

      //## Operation: GetTaskWithLocalId%939135437
      virtual ClJSJob * GetTaskWithLocalId (const m4uint32_t &ai_uiLocalId) = 0;

      //## Operation: GetParamWithName%939135438
      ClJSParameter * GetParamWithName (const M4ClString &ai_stName);

      //## Operation: ExecuteOn%938766907
      m4return_t ExecuteOn ();

      //## Operation: AddParameter%938766922
      m4return_t AddParameter (ClJSParameter *ai_pParameter = NULL);

      //## Operation: AddParameterConnection%939135436
      m4return_t AddParameterConnection (const m4uint32_t &ai_uiLocalId1, const M4ClString &ai_stParamName1, const m4uint32_t &ai_uiLocalId2, const M4ClString &ai_stParamName2);

      //## Operation: PrintAllParametersOn%939045077
      void PrintAllParametersOn (ostream &aio_ostream);

      //## Operation: IsRoot%939812332
      m4bool_t IsRoot ();

      //## Operation: GetRoot%939812334
      ClJSJob * GetRoot ();

      //## Operation: IsFinished%939812333
      m4bool_t IsFinished ();

      //## Operation: operator ==%939980388; C++
      m4bool_t operator == (const ClJSJob &ai_oJob);

      //## Operation: AddTask%940243354
      virtual m4return_t AddTask (ClJSJob *ai_poTask, const m4uint_t &ai_uiLocalId = 0);

      //## Operation: GetFirstInputParameter%941561489
      ClJSParameter * GetFirstInputParameter ();

      //## Operation: GetNextInputParameter%941561490
      ClJSParameter * GetNextInputParameter ();

      //## Operation: GetFirstOutputParameter%941561491
      ClJSParameter * GetFirstOutputParameter ();

      //## Operation: GetNextOutputParameter%941561492
      ClJSParameter * GetNextOutputParameter ();

      //## Operation: PropagateAllParamValues%941619584
      virtual m4return_t PropagateAllParamValues ();

      //## Operation: SetParamValue%941737616
      m4return_t SetParamValue (const M4ClString &ai_stParamName, const M4ClString &ai_stParamValue);

    // Data Members for Associations

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37F3422B02B8
      //## Role: ClJSJob::m_poExecutionStrategy%37F3422D02D9
      //## begin ClJSJob::m_poExecutionStrategy%37F3422D02D9.role preserve=no  public: ClJSExecutionStrategy { -> 1RHAN}
      ClJSExecutionStrategy *m_poExecutionStrategy;
      //## end ClJSJob::m_poExecutionStrategy%37F3422D02D9.role

  public:
    // Additional Public Declarations
      //## begin ClJSJob%375BE73302C5.public preserve=yes
      //## end ClJSJob%375BE73302C5.public

  protected:

    //## Other Operations (specified)
      //## Operation: FindNextParameter%941619585
      ClJSParameter * FindNextParameter (ClJSParameter::eParameterFlowType ai_eType);

    // Data Members for Class Attributes

      //## Attribute: m_uiId%375BE79F01C6
      //## begin ClJSJob::m_uiId%375BE79F01C6.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiId;
      //## end ClJSJob::m_uiId%375BE79F01C6.attr

      //## Attribute: m_uiIdSchedJob%37A0234800F7
      //## begin ClJSJob::m_uiIdSchedJob%37A0234800F7.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIdSchedJob;
      //## end ClJSJob::m_uiIdSchedJob%37A0234800F7.attr

      //## Attribute: m_uiIdExecution%37A0328C02D7
      //## begin ClJSJob::m_uiIdExecution%37A0328C02D7.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiIdExecution;
      //## end ClJSJob::m_uiIdExecution%37A0328C02D7.attr

      //## Attribute: m_uiLocalId%375BF58901F0
      //## begin ClJSJob::m_uiLocalId%375BF58901F0.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiLocalId;
      //## end ClJSJob::m_uiLocalId%375BF58901F0.attr

      //## Attribute: m_eStatus%379EDAF703D7
      //## begin ClJSJob::m_eStatus%379EDAF703D7.attr preserve=no  protected: eJobStatus {UA} 
      eJobStatus m_eStatus;
      //## end ClJSJob::m_eStatus%379EDAF703D7.attr

      //## Attribute: m_eInterruptionRequestLevel%37F22F20015F
      //## begin ClJSJob::m_eInterruptionRequestLevel%37F22F20015F.attr preserve=no  protected: eInterruptRequestLevel {UA} 
      eInterruptRequestLevel m_eInterruptionRequestLevel;
      //## end ClJSJob::m_eInterruptionRequestLevel%37F22F20015F.attr

      //## Attribute: m_bParameterIteratorIsValid%381F24B702F2
      //## begin ClJSJob::m_bParameterIteratorIsValid%381F24B702F2.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bParameterIteratorIsValid;
      //## end ClJSJob::m_bParameterIteratorIsValid%381F24B702F2.attr

      //## Attribute: m_ParameterIterator%381F24CE03B3
      //## begin ClJSJob::m_ParameterIterator%381F24CE03B3.attr preserve=no  protected: ParameterList::iterator {UA} 
      ParameterList::iterator m_ParameterIterator;
      //## end ClJSJob::m_ParameterIterator%381F24CE03B3.attr

    // Data Members for Associations

      //## Association: M4JSExeJob::M4JSJob::defineContexto%375CD9B10339
      //## Role: ClJSJob::m_oSessionSettings%375CD9B203DA
      //## begin ClJSJob::m_oSessionSettings%375CD9B203DA.role preserve=no  protected: ClJSSessionSettings { -> 1VHAN}
      ClJSSessionSettings m_oSessionSettings;
      //## end ClJSJob::m_oSessionSettings%375CD9B203DA.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37A01E260043
      //## Role: ClJSJob::m_mParameters%37A01E2601E8
      //## begin ClJSJob::m_mParameters%37A01E2601E8.role preserve=no  protected: ParameterList { -> 1VHAN}
      ParameterList m_mParameters;
      //## end ClJSJob::m_mParameters%37A01E2601E8.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E9DE0F012F
      //## Role: ClJSJob::m_oStartTime%37E9DE100054
      //## begin ClJSJob::m_oStartTime%37E9DE100054.role preserve=no  protected: M4ClTimeStamp { -> 1VHAN}
      M4ClTimeStamp m_oStartTime;
      //## end ClJSJob::m_oStartTime%37E9DE100054.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E9DE39039C
      //## Role: ClJSJob::m_oEndTime%37E9DE3A02E9
      //## begin ClJSJob::m_oEndTime%37E9DE3A02E9.role preserve=no  protected: M4ClTimeStamp { -> 1VHAN}
      M4ClTimeStamp m_oEndTime;
      //## end ClJSJob::m_oEndTime%37E9DE3A02E9.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37EA55530362
      //## Role: ClJSJob::m_oScheduledTime%37EA555402AF
      //## begin ClJSJob::m_oScheduledTime%37EA555402AF.role preserve=no  protected: M4ClTimeStamp { -> 1VHAN}
      M4ClTimeStamp m_oScheduledTime;
      //## end ClJSJob::m_oScheduledTime%37EA555402AF.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37EA55E300AC
      //## Role: ClJSJob::m_poParent%37EA55E500C3
      //## begin ClJSJob::m_poParent%37EA55E500C3.role preserve=no  protected: ClJSJob { -> 1RFHAN}
      ClJSJob *m_poParent;
      //## end ClJSJob::m_poParent%37EA55E500C3.role

    // Additional Protected Declarations
      //## begin ClJSJob%375BE73302C5.protected preserve=yes
      //## end ClJSJob%375BE73302C5.protected

  private:
    // Additional Private Declarations
      //## begin ClJSJob%375BE73302C5.private preserve=yes
      //## end ClJSJob%375BE73302C5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSJob%375BE73302C5.implementation preserve=yes
      //## end ClJSJob%375BE73302C5.implementation

};

//## begin ClJSJob%375BE73302C5.postscript preserve=yes
//## end ClJSJob%375BE73302C5.postscript

// Class ClJSJob 

//## begin module%37E244EB00C1.epilog preserve=yes
//## end module%37E244EB00C1.epilog


#endif
