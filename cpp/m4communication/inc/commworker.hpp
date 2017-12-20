//## begin module%38DB639B0292.cm preserve=no
//## end module%38DB639B0292.cm

//## begin module%38DB639B0292.cp preserve=no
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
//## end module%38DB639B0292.cp

//## Module: CommWorker%38DB639B0292; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\commworker.hpp

#ifndef commworker_h
#define commworker_h 1

//## begin module%38DB639B0292.additionalIncludes preserve=no
//## end module%38DB639B0292.additionalIncludes

//## begin module%38DB639B0292.includes preserve=yes
//## end module%38DB639B0292.includes

// m4thread
#include <m4thread.hpp>
// m4types
#include <m4types.hpp>
// CommAction
#include <commaction.hpp>
// m4crono
#include <m4crono.hpp>
// m4clock
#include <m4clock.hpp>
//## begin module%38DB639B0292.declarations preserve=no
//## end module%38DB639B0292.declarations

//## begin module%38DB639B0292.additionalDeclarations preserve=yes
//## end module%38DB639B0292.additionalDeclarations


//## begin ClCommWorker%38DA016D0136.preface preserve=yes
//## end ClCommWorker%38DA016D0136.preface

//## Class: ClCommWorker%38DA016D0136
//## Category: M4Communication::M4COML%38D1FB110288
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommWorker : public M4Thread  //## Inherits: <unnamed>%38DA021C0029
{
  //## begin ClCommWorker%38DA016D0136.initialDeclarations preserve=yes
  //## end ClCommWorker%38DA016D0136.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommWorker%954259092; C++
      ClCommWorker (ClActionQueue &ai_oActionQ);


    //## Other Operations (specified)
      //## Operation: Run%961088169; C++
      virtual void Run (void );

      //## Operation: Stop%961088170
      m4return_t Stop ();

      //## Operation: GetLastActionExecutionTime%964423553
      m4uint_t GetLastActionExecutionTime ();

      //## Operation: GetExecutionTime%964423554
      m4uint_t GetExecutionTime ();

      //## Operation: GetWaitingTime%964423555
      m4uint_t GetWaitingTime ();

      //## Operation: GetExecutedActions%964423556
      m4uint_t GetExecutedActions ();

      //## Operation: GetExternalExecutingTime%965977801
      m4uint_t GetExternalExecutingTime ();

      //## Operation: IsReady%967794007
      m4bool_t IsReady ();

      //## Operation: SetID%967794008; C++
      m4return_t SetID (m4int_t ai_iWorkerID);

      //## Operation: GetID%967794009
      m4int_t GetID ();

    // Additional Public Declarations
      //## begin ClCommWorker%38DA016D0136.public preserve=yes
	  static m4int_t GetWorkerNumber();
      //## end ClCommWorker%38DA016D0136.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::<unnamed>%38E0D57600C8
      //## Role: ClCommWorker::m_poActionQueue%38E0D5760280
      //## begin ClCommWorker::m_poActionQueue%38E0D5760280.role preserve=no  protected: ClActionQueue { -> RHAN}
      ClActionQueue *m_poActionQueue;
      //## end ClCommWorker::m_poActionQueue%38E0D5760280.role

      //## Association: M4Communication::M4COML::<unnamed>%3949137F0262
      //## Role: ClCommWorker::m_bShuttingDown%394913800028
      //## begin ClCommWorker::m_bShuttingDown%394913800028.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bShuttingDown;
      //## end ClCommWorker::m_bShuttingDown%394913800028.role

      //## Association: M4Communication::M4COML::<unnamed>%397BF0C5029B
      //## Role: ClCommWorker::m_msLastActionExecutionTime%397BF0C60166
      //## begin ClCommWorker::m_msLastActionExecutionTime%397BF0C60166.role preserve=no  protected: m4millisec_t { -> VHAN}
      m4millisec_t m_msLastActionExecutionTime;
      //## end ClCommWorker::m_msLastActionExecutionTime%397BF0C60166.role

      //## Association: M4Communication::M4COML::<unnamed>%397BF13A000E
      //## Role: ClCommWorker::m_msExecutionTime%397BF13A02AE
      //## begin ClCommWorker::m_msExecutionTime%397BF13A02AE.role preserve=no  protected: m4millisec_t { -> VHAN}
      m4millisec_t m_msExecutionTime;
      //## end ClCommWorker::m_msExecutionTime%397BF13A02AE.role

      //## Association: M4Communication::M4COML::<unnamed>%397BF1F20121
      //## Role: ClCommWorker::m_msWaitingTime%397BF1F203B7
      //## begin ClCommWorker::m_msWaitingTime%397BF1F203B7.role preserve=no  protected: m4millisec_t { -> VHAN}
      m4millisec_t m_msWaitingTime;
      //## end ClCommWorker::m_msWaitingTime%397BF1F203B7.role

      //## Association: M4Communication::M4COML::<unnamed>%397BF97E0206
      //## Role: ClCommWorker::m_uiExecutedActions%397BF97E03B4
      //## begin ClCommWorker::m_uiExecutedActions%397BF97E03B4.role preserve=no  protected: m4uint32_t { -> VHAN}
      m4uint32_t m_uiExecutedActions;
      //## end ClCommWorker::m_uiExecutedActions%397BF97E03B4.role

      //## Association: M4Communication::M4COML::<unnamed>%3993BC740027
      //## Role: ClCommWorker::m_msExternalExecutingTime%3993BC7501E2
      //## begin ClCommWorker::m_msExternalExecutingTime%3993BC7501E2.role preserve=no  protected: m4millisec_t { -> VHAN}
      m4millisec_t m_msExternalExecutingTime;
      //## end ClCommWorker::m_msExternalExecutingTime%3993BC7501E2.role

      //## Association: M4Communication::M4COML::<unnamed>%39AA8100033F
      //## Role: ClCommWorker::m_oExecutingCrono%39AA8101014C
      //## begin ClCommWorker::m_oExecutingCrono%39AA8101014C.role preserve=no  protected: ClCrono { -> VHAN}
      ClCrono m_oExecutingCrono;
      //## end ClCommWorker::m_oExecutingCrono%39AA8101014C.role

      //## Association: M4Communication::M4COML::<unnamed>%39AA811B006D
      //## Role: ClCommWorker::m_oWaitingCrono%39AA811B033F
      //## begin ClCommWorker::m_oWaitingCrono%39AA811B033F.role preserve=no  protected: ClCrono { -> VHAN}
      ClCrono m_oWaitingCrono;
      //## end ClCommWorker::m_oWaitingCrono%39AA811B033F.role

      //## Association: M4Communication::M4COML::<unnamed>%39AA81400143
      //## Role: ClCommWorker::m_oExtExecutingCrono%39AA8141005E
      //## begin ClCommWorker::m_oExtExecutingCrono%39AA8141005E.role preserve=no  protected: ClCrono { -> VHAN}
      ClCrono m_oExtExecutingCrono;
      //## end ClCommWorker::m_oExtExecutingCrono%39AA8141005E.role

      //## Association: M4Communication::M4COML::<unnamed>%39AA86690188
      //## Role: ClCommWorker::m_oExternalClock%39AA866A00C1
      //## begin ClCommWorker::m_oExternalClock%39AA866A00C1.role preserve=no  protected: ClCommExternalClock { -> VHAN}
      ClCommExternalClock m_oExternalClock;
      //## end ClCommWorker::m_oExternalClock%39AA866A00C1.role

    // Additional Protected Declarations
      //## begin ClCommWorker%38DA016D0136.protected preserve=yes
	  static void AddedWorker();
	  static void DelWorker();

	  static ClMutex m_oActiveWorkerMutex;
	  static m4int_t m_iNActivesWorker;
      //## end ClCommWorker%38DA016D0136.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bIsReady%39AF5EA00096
      //## begin ClCommWorker::m_bIsReady%39AF5EA00096.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsReady;
      //## end ClCommWorker::m_bIsReady%39AF5EA00096.attr

      //## Attribute: m_iWorkerID%39AF5F4A016C
      //## begin ClCommWorker::m_iWorkerID%39AF5F4A016C.attr preserve=no  private: m4int_t {UA} 
      m4int_t m_iWorkerID;
      //## end ClCommWorker::m_iWorkerID%39AF5F4A016C.attr

    // Additional Private Declarations
      //## begin ClCommWorker%38DA016D0136.private preserve=yes
      //## end ClCommWorker%38DA016D0136.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommWorker%38DA016D0136.implementation preserve=yes
      //## end ClCommWorker%38DA016D0136.implementation

};

//## begin ClCommWorker%38DA016D0136.postscript preserve=yes
//## end ClCommWorker%38DA016D0136.postscript

// Class ClCommWorker 

//## begin module%38DB639B0292.epilog preserve=yes
//## end module%38DB639B0292.epilog


#endif
