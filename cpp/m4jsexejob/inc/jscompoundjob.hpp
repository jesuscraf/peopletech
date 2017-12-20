//## begin module%37E602C000AA.cm preserve=no
//## end module%37E602C000AA.cm

//## begin module%37E602C000AA.cp preserve=no
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
//## end module%37E602C000AA.cp

//## Module: jscompoundjob%37E602C000AA; Package specification
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Source file: C:\Trabajo\Server\m4jsexejob\inc\jscompoundjob.hpp

#ifndef jscompoundjob_h
#define jscompoundjob_h 1

//## begin module%37E602C000AA.additionalIncludes preserve=no
//## end module%37E602C000AA.additionalIncludes

//## begin module%37E602C000AA.includes preserve=yes
#include "m4stl.hpp"
//## end module%37E602C000AA.includes

// m4jsjob_dll
#include <m4jsexejob_dll.hpp>
// jsjob
#include <jsjob.hpp>
//## begin module%37E602C000AA.declarations preserve=no
//## end module%37E602C000AA.declarations

//## begin module%37E602C000AA.additionalDeclarations preserve=yes
//## end module%37E602C000AA.additionalDeclarations


//## begin TaskList%37A0241D031A.preface preserve=yes
//## end TaskList%37A0241D031A.preface

//## Class: TaskList%37A0241D031A; Instantiated Class
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37A0243300E1;ClJSJob { -> }

typedef map< m4uint32_t,ClJSJob *,less < m4uint32_t > > TaskList;

//## begin TaskList%37A0241D031A.postscript preserve=yes
//## end TaskList%37A0241D031A.postscript

//## begin ClJSCompoundJob%375BE75101E2.preface preserve=yes
//## end ClJSCompoundJob%375BE75101E2.preface

//## Class: ClJSCompoundJob%375BE75101E2
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSCompoundJob : public ClJSJob  //## Inherits: <unnamed>%37A0258B0320
{
  //## begin ClJSCompoundJob%375BE75101E2.initialDeclarations preserve=yes
  //## end ClJSCompoundJob%375BE75101E2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSCompoundJob%937821612
      //	Constructor de la clase.
      //	Recibe 4 parámetros:
      //	   - Identificador del trabajo (id)
      //	   - Identificador de la ejecución (idExecution)
      ClJSCompoundJob (const m4uint_t &ai_uiId, const m4uint_t &ai_uiIdExecution = 0);

    //## Destructor (specified)
      //## Operation: ~ClJSCompoundJob%938600373
      virtual ~ClJSCompoundJob ();


    //## Other Operations (specified)
      //## Operation: AddTask%933264300
      virtual m4return_t AddTask (ClJSJob *ai_poTask, const m4uint_t &ai_uiLocalId = 0);

      //## Operation: GetNumberOfTasks%938517343
      virtual m4uint32_t GetNumberOfTasks ();

      //## Operation: PrintOn%938424583
      void PrintOn (ostream &ai_ostream);

      //## Operation: PrintAllTasksOn%938517344
      void PrintAllTasksOn (ostream &aio_ostream);

      //## Operation: SetStatus%938600375
      m4return_t SetStatus (const eJobStatus &ai_eStatus);

      //## Operation: GetNextExecutableTask%938600380
      ClJSJob * GetNextExecutableTask ();

      //## Operation: GetTaskWithLocalId%939135440
      ClJSJob * GetTaskWithLocalId (const m4uint32_t &ai_uiLocalId);

      //## Operation: PropagateAllParamValues%941619586
      virtual m4return_t PropagateAllParamValues ();

    // Additional Public Declarations
      //## begin ClJSCompoundJob%375BE75101E2.public preserve=yes
      //## end ClJSCompoundJob%375BE75101E2.public

  protected:
    // Data Members for Associations

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37A0243D0117
      //## Role: ClJSCompoundJob::m_mTasks%37A0243E035F
      //## begin ClJSCompoundJob::m_mTasks%37A0243E035F.role preserve=no  protected: TaskList { -> 1VHAN}
      TaskList m_mTasks;
      //## end ClJSCompoundJob::m_mTasks%37A0243E035F.role

    // Additional Protected Declarations
      //## begin ClJSCompoundJob%375BE75101E2.protected preserve=yes
      //## end ClJSCompoundJob%375BE75101E2.protected

  private:
    // Additional Private Declarations
      //## begin ClJSCompoundJob%375BE75101E2.private preserve=yes
      //## end ClJSCompoundJob%375BE75101E2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSCompoundJob%375BE75101E2.implementation preserve=yes
      //## end ClJSCompoundJob%375BE75101E2.implementation

};

//## begin ClJSCompoundJob%375BE75101E2.postscript preserve=yes
//## end ClJSCompoundJob%375BE75101E2.postscript

// Class ClJSCompoundJob 

//## begin module%37E602C000AA.epilog preserve=yes
//## end module%37E602C000AA.epilog


#endif
