//## begin module%3912C5830283.cm preserve=no
//## end module%3912C5830283.cm

//## begin module%3912C5830283.cp preserve=no
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
//## end module%3912C5830283.cp

//## Module: clsubjct%3912C5830283; Package specification
//## Subsystem: m4sysall::inc%3912C5D60363
//## Source file: d:\new\m4sysall\inc\clsubjct.hpp

#ifndef __CLSUBJCT_HPP__
#define __CLSUBJCT_HPP__ 1

//## begin module%3912C5830283.additionalIncludes preserve=no
//## end module%3912C5830283.additionalIncludes

//## begin module%3912C5830283.includes preserve=yes

#include "m4sysall_dll.hpp"
#include "m4define.hpp"
#include "m4stl.hpp"
#include "clobserv.hpp"

// NAMESPACEs don´t supported in Sun CC 4.2
//namespace ObserverPattern
//{
//## end module%3912C5830283.includes

//## begin module%3912C5830283.declarations preserve=no
//## end module%3912C5830283.declarations

//## begin module%3912C5830283.additionalDeclarations preserve=yes
//## end module%3912C5830283.additionalDeclarations


//## begin ClSubject%3912C5830291.preface preserve=yes
//## end ClSubject%3912C5830291.preface

//## Class: ClSubject%3912C5830291
//	NAMESPACEs don´t supported in Sun CC 4.2
//	namespace ObserverPattern
//	{
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL ClSubject 
{
  //## begin ClSubject%3912C5830291.initialDeclarations preserve=yes
  //## end ClSubject%3912C5830291.initialDeclarations

    //## Class: OBSERVERLIST%3912C584004F; private
    //## Category: m4sysall%3912C5830151
    //## Subsystem: m4sysall%3912C5830147
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef list<ClObserver*> OBSERVERLIST;

  public:
    //## Destructor (specified)
      //## Operation: ~ClSubject%-2095366624; C++
      //	-- {AddDecl: 146} region.unprotectedFunction [717..790]
      virtual ~ClSubject (void );


    //## Other Operations (specified)
      //## Operation: Attach%-1449382659; C++
      //	-- {AddDecl: 147} region.unprotectedFunction [791..908]
      virtual void Attach (ClObserver* ai_poClObserver);

      //## Operation: Detach%-1250960091; C++
      //	-- {AddDecl: 148} region.unprotectedFunction [909..1022]
      virtual void Detach (ClObserver* ai_poClObserver);

      //## Operation: Notify%-928315364; C++
      //	-- {AddDecl: 149} region.unprotectedFunction [1023..1278]
      virtual void Notify (void );

    // Additional Public Declarations
      //## begin ClSubject%3912C5830291.public preserve=yes
      //## end ClSubject%3912C5830291.public

  protected:
    //## Constructors (specified)
      //## Operation: ClSubject%554791384; C++
      ClSubject ();

    // Additional Protected Declarations
      //## begin ClSubject%3912C5830291.protected preserve=yes
      //## end ClSubject%3912C5830291.protected

  private:
    // Data Members for Associations

      //## Association: m4sysall::<unnamed>%3912C5840050
      //## Role: ClSubject::m_oObserverList%3912C5840052
      //## begin ClSubject::m_oObserverList%3912C5840052.role preserve=no  private: ClSubject::OBSERVERLIST {1 -> 1VHAPN}
      OBSERVERLIST m_oObserverList;
      //## end ClSubject::m_oObserverList%3912C5840052.role

    // Additional Private Declarations
      //## begin ClSubject%3912C5830291.private preserve=yes
      //## end ClSubject%3912C5830291.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSubject%3912C5830291.implementation preserve=yes
      //## end ClSubject%3912C5830291.implementation

};

//## begin ClSubject%3912C5830291.postscript preserve=yes
//## end ClSubject%3912C5830291.postscript

// Class ClSubject 

//## begin module%3912C5830283.epilog preserve=yes
//## end module%3912C5830283.epilog


#endif
