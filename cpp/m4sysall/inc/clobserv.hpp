//## begin module%3912C58301A1.cm preserve=no
//## end module%3912C58301A1.cm

//## begin module%3912C58301A1.cp preserve=no
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
//## end module%3912C58301A1.cp

//## Module: clobserv%3912C58301A1; Package specification
//## Subsystem: m4sysall::inc%3912C5D60363
//## Source file: d:\new\m4sysall\inc\clobserv.hpp

#ifndef __CLOBSERV_HPP__
#define __CLOBSERV_HPP__ 1

//## begin module%3912C58301A1.additionalIncludes preserve=no
//## end module%3912C58301A1.additionalIncludes

//## begin module%3912C58301A1.includes preserve=yes

#include "m4sysall_dll.hpp"
#include "m4define.hpp"
//## end module%3912C58301A1.includes

//## begin module%3912C58301A1.declarations preserve=no
//## end module%3912C58301A1.declarations

//## begin module%3912C58301A1.additionalDeclarations preserve=yes

// NAMESPACEs don't supported in Sun CC 4.2
//namespace ObserverPattern
//{
	class ClSubject;

//## end module%3912C58301A1.additionalDeclarations


//## begin ClObserver%3912C58301A2.preface preserve=yes
//## end ClObserver%3912C58301A2.preface

//## Class: ClObserver%3912C58301A2; Abstract
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL ClObserver 
{
  //## begin ClObserver%3912C58301A2.initialDeclarations preserve=yes
  //## end ClObserver%3912C58301A2.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClObserver%-1665901480; C++
      virtual ~ClObserver (void )
      {
        //## begin ClObserver::~ClObserver%-1665901480.body preserve=yes
        //## end ClObserver::~ClObserver%-1665901480.body
      }


    //## Other Operations (specified)
      //## Operation: Update%1443282925; C++
      virtual void Update (ClSubject* ai_poClSubject) = 0;

    // Additional Public Declarations
      //## begin ClObserver%3912C58301A2.public preserve=yes
      //## end ClObserver%3912C58301A2.public

  protected:
    //## Constructors (specified)
      //## Operation: ClObserver%-750865850; C++
      ClObserver ()
        //## begin ClObserver::ClObserver%-750865850.hasinit preserve=no
        //## end ClObserver::ClObserver%-750865850.hasinit
        //## begin ClObserver::ClObserver%-750865850.initialization preserve=yes
        //## end ClObserver::ClObserver%-750865850.initialization
      {
        //## begin ClObserver::ClObserver%-750865850.body preserve=yes
        //## end ClObserver::ClObserver%-750865850.body
      }

    // Additional Protected Declarations
      //## begin ClObserver%3912C58301A2.protected preserve=yes
      //## end ClObserver%3912C58301A2.protected

  private:
    // Additional Private Declarations
      //## begin ClObserver%3912C58301A2.private preserve=yes
      //## end ClObserver%3912C58301A2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClObserver%3912C58301A2.implementation preserve=yes
      //## end ClObserver%3912C58301A2.implementation

};

//## begin ClObserver%3912C58301A2.postscript preserve=yes
//## end ClObserver%3912C58301A2.postscript

//## begin module%3912C58301A1.epilog preserve=yes
//## end module%3912C58301A1.epilog


#endif
