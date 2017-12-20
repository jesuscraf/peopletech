//## begin module%39490E65004A.cm preserve=no
//## end module%39490E65004A.cm

//## begin module%39490E65004A.cp preserve=no
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
//## end module%39490E65004A.cp

//## Module: actionworker%39490E65004A; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\actionworker.hpp

#ifndef actionworker_h
#define actionworker_h 1

//## begin module%39490E65004A.additionalIncludes preserve=no
//## end module%39490E65004A.additionalIncludes

//## begin module%39490E65004A.includes preserve=yes
#include <clbaseaction.hpp>
//## end module%39490E65004A.includes


// m4thread
#include <m4thread.hpp>
// FastQ
#include <fastfifoq.hpp>
//## begin module%39490E65004A.declarations preserve=no
//## end module%39490E65004A.declarations

//## begin module%39490E65004A.additionalDeclarations preserve=yes
//## end module%39490E65004A.additionalDeclarations


//## begin ClActionsQ%3947BBD100A3.preface preserve=yes
//## end ClActionsQ%3947BBD100A3.preface

//## Class: ClActionsQ%3947BBD100A3; Instantiated Class
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef FastQueue< ClBaseAction * > ClActionsQ;

//## begin ClActionsQ%3947BBD100A3.postscript preserve=yes
//## end ClActionsQ%3947BBD100A3.postscript

//## begin ClActionWorker%39490D3902C9.preface preserve=yes
//## end ClActionWorker%39490D3902C9.preface

//## Class: ClActionWorker%39490D3902C9
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3949E37A016B;list { -> }

class M4_DECL_M4SERVERBASE ClActionWorker : public M4Thread  //## Inherits: <unnamed>%39490D740184
{
  //## begin ClActionWorker%39490D3902C9.initialDeclarations preserve=yes
  //## end ClActionWorker%39490D3902C9.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClActionWorker%961054365
      ClActionWorker (ClActionsQ &ai_poActionsQ);

    //## Destructor (specified)
      //## Operation: ~ClActionWorker%961054366
      ~ClActionWorker ();


    //## Other Operations (specified)
      //## Operation: ExecuteAction%961054368
      virtual void ExecuteAction (ClBaseAction &ai_oAction);

    // Additional Public Declarations
      //## begin ClActionWorker%39490D3902C9.public preserve=yes
      //## end ClActionWorker%39490D3902C9.public

  protected:
    // Additional Protected Declarations
      //## begin ClActionWorker%39490D3902C9.protected preserve=yes
      //## end ClActionWorker%39490D3902C9.protected

  private:

    //## Other Operations (specified)
      //## Operation: Run%961054367; C++
      void Run (void );

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%39490DEE0306
      //## Role: ClActionWorker::m_poActionsQueue%39490DEF019F
      //## begin ClActionWorker::m_poActionsQueue%39490DEF019F.role preserve=no  private: ClActionsQ { -> RHAN}
      ClActionsQ *m_poActionsQueue;
      //## end ClActionWorker::m_poActionsQueue%39490DEF019F.role

    // Additional Private Declarations
      //## begin ClActionWorker%39490D3902C9.private preserve=yes
      //## end ClActionWorker%39490D3902C9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClActionWorker%39490D3902C9.implementation preserve=yes
      //## end ClActionWorker%39490D3902C9.implementation

};

//## begin ClActionWorker%39490D3902C9.postscript preserve=yes
//## end ClActionWorker%39490D3902C9.postscript

//## begin ClActionWorkerList%394914BC0012.preface preserve=yes
//## end ClActionWorkerList%394914BC0012.preface

//## Class: ClActionWorkerList%394914BC0012
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%394914E90098;ClActionWorker { -> }

typedef list <ClActionWorker*> ClActionWorkerList;

//## begin ClActionWorkerList%394914BC0012.postscript preserve=yes
//## end ClActionWorkerList%394914BC0012.postscript

// Class ClActionWorker 

//## begin module%39490E65004A.epilog preserve=yes
//## end module%39490E65004A.epilog


#endif
