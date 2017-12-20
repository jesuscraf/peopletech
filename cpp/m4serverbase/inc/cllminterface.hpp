//## begin module%37D37A680099.cm preserve=no
//## end module%37D37A680099.cm

//## begin module%37D37A680099.cp preserve=no
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
//## end module%37D37A680099.cp

//## Module: cllminterface%37D37A680099; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: D:\eduardoma\m4serverbase\inc\cllminterface.hpp

#ifndef cllminterface_h
#define cllminterface_h 1

//## begin module%37D37A680099.additionalIncludes preserve=no
//## end module%37D37A680099.additionalIncludes

//## begin module%37D37A680099.includes preserve=yes
#include <statistics.hpp>
//## end module%37D37A680099.includes

// clserviceinterface
//#include <clserviceinterface.hpp>
//## begin module%37D37A680099.declarations preserve=no
//## end module%37D37A680099.declarations

//## begin module%37D37A680099.additionalDeclarations preserve=yes
class ClServiceInterface ;
//## end module%37D37A680099.additionalDeclarations


//## begin ClLocalMonitorInterface%37D379300069.preface preserve=yes
//## end ClLocalMonitorInterface%37D379300069.preface

//## Class: ClLocalMonitorInterface%37D379300069
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37D39223030C;ClServiceInterface { -> }

class ClLocalMonitorInterface 
{
  //## begin ClLocalMonitorInterface%37D379300069.initialDeclarations preserve=yes
  //## end ClLocalMonitorInterface%37D379300069.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: CreateService%936606375
      //	it's an administrator method that creates a new service and put it into the
      //	apropiate dictionary.
      virtual ClServiceInterface * CreateService (m4int32_t ai_IdService, m4uint16_t ai_iNumQItems = 0) = 0;

      //## Operation: Synchronize%936613829
      virtual void Synchronize () = 0;

      //## Operation: GetStats%936613833
      virtual ClStatistic * GetStats () = 0;

      //## Operation: ShutDown%936613834
      //	when the administrator want to shut down the server station, must use this
      //	method.
      //	By using it before shout down the sistem, the services are closed.
      virtual m4return_t ShutDown () = 0;

    // Additional Public Declarations
      //## begin ClLocalMonitorInterface%37D379300069.public preserve=yes
      //## end ClLocalMonitorInterface%37D379300069.public

  protected:
    // Additional Protected Declarations
      //## begin ClLocalMonitorInterface%37D379300069.protected preserve=yes
      //## end ClLocalMonitorInterface%37D379300069.protected

  private:
    // Additional Private Declarations
      //## begin ClLocalMonitorInterface%37D379300069.private preserve=yes
      //## end ClLocalMonitorInterface%37D379300069.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLocalMonitorInterface%37D379300069.implementation preserve=yes
      //## end ClLocalMonitorInterface%37D379300069.implementation

};

//## begin ClLocalMonitorInterface%37D379300069.postscript preserve=yes
//## end ClLocalMonitorInterface%37D379300069.postscript

// Class ClLocalMonitorInterface 

//## begin module%37D37A680099.epilog preserve=yes
//## end module%37D37A680099.epilog


#endif
