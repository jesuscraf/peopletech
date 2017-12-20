//## begin module%39479D8B00A9.cm preserve=no
//## end module%39479D8B00A9.cm

//## begin module%39479D8B00A9.cp preserve=no
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
//## end module%39479D8B00A9.cp

//## Module: dspinvoker%39479D8B00A9; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: C:\WorkingFolder\m4dispatcher\inc\dspinvoker.hpp

#ifndef dspinvoker_h
#define dspinvoker_h 1

//## begin module%39479D8B00A9.additionalIncludes preserve=no
//## end module%39479D8B00A9.additionalIncludes

//## begin module%39479D8B00A9.includes preserve=yes
//## end module%39479D8B00A9.includes

// actioninvoker
#include <actioninvoker.hpp>
//## begin module%39479D8B00A9.declarations preserve=no
//## end module%39479D8B00A9.declarations

//## begin module%39479D8B00A9.additionalDeclarations preserve=yes
//## end module%39479D8B00A9.additionalDeclarations


//## begin ClDspInvokerAbstract%39479D15035D.preface preserve=yes
//## end ClDspInvokerAbstract%39479D15035D.preface

//## Class: ClDspInvokerAbstract%39479D15035D
//## Category: M4Dispatcher::Invoker%39F4478C01DE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39479D310190;ClActionInvoker { -> }

typedef ClActionInvoker <ClBaseActionExecutive*> ClDspInvokerAbstract;

//## begin ClDspInvokerAbstract%39479D15035D.postscript preserve=yes
//## end ClDspInvokerAbstract%39479D15035D.postscript

//## begin ClDispatcherInvoker%39479A8900CA.preface preserve=yes
//## end ClDispatcherInvoker%39479A8900CA.preface

//## Class: ClDispatcherInvoker%39479A8900CA
//## Category: M4Dispatcher::Invoker%39F4478C01DE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDispatcherInvoker : public ClDspInvokerAbstract  //## Inherits: <unnamed>%39479D5F020E
{
  //## begin ClDispatcherInvoker%39479A8900CA.initialDeclarations preserve=yes
  //## end ClDispatcherInvoker%39479A8900CA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDispatcherInvoker%960967034
      ClDispatcherInvoker ();

    //## Destructor (specified)
      //## Operation: ~ClDispatcherInvoker%960967035
      ~ClDispatcherInvoker ();


    //## Other Operations (specified)
      //## Operation: Instance%960967036
      static ClDispatcherInvoker * Instance ();

      //## Operation: FreeInstance%972579513
      static ClDispatcherInvoker * FreeInstance ();

    // Additional Public Declarations
      //## begin ClDispatcherInvoker%39479A8900CA.public preserve=yes
      //## end ClDispatcherInvoker%39479A8900CA.public

  protected:
    // Additional Protected Declarations
      //## begin ClDispatcherInvoker%39479A8900CA.protected preserve=yes
      //## end ClDispatcherInvoker%39479A8900CA.protected

  private:
    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%39479A95017C
      //## Role: ClDispatcherInvoker::m_poInstance%39479A970016
      //## begin ClDispatcherInvoker::m_poInstance%39479A970016.role preserve=no  private: static ClDispatcherInvoker { -> RHAN}
      static ClDispatcherInvoker *m_poInstance;
      //## end ClDispatcherInvoker::m_poInstance%39479A970016.role

    // Additional Private Declarations
      //## begin ClDispatcherInvoker%39479A8900CA.private preserve=yes
      //## end ClDispatcherInvoker%39479A8900CA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDispatcherInvoker%39479A8900CA.implementation preserve=yes
      //## end ClDispatcherInvoker%39479A8900CA.implementation

};

//## begin ClDispatcherInvoker%39479A8900CA.postscript preserve=yes
//## end ClDispatcherInvoker%39479A8900CA.postscript

// Class ClDispatcherInvoker 

//## begin module%39479D8B00A9.epilog preserve=yes
//## end module%39479D8B00A9.epilog


#endif
