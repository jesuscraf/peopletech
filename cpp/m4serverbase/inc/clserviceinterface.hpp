//## begin module%37D387D00094.cm preserve=no
//## end module%37D387D00094.cm

//## begin module%37D387D00094.cp preserve=no
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
//## end module%37D387D00094.cp

//## Module: clserviceinterface%37D387D00094; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\version\inc\clserviceinterface.hpp

#ifndef clserviceinterface_h
#define clserviceinterface_h 1

//## begin module%37D387D00094.additionalIncludes preserve=no
//## end module%37D387D00094.additionalIncludes

//## begin module%37D387D00094.includes preserve=yes
//## end module%37D387D00094.includes

// Statistics
#include <statistics.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// FIFOQ
#include <fifoq.hpp>
// ServiceQ
#include <serviceq.hpp>

class ClServiceStats;

//## begin module%37D387D00094.declarations preserve=no
//## end module%37D387D00094.declarations

//## begin module%37D387D00094.additionalDeclarations preserve=yes
#define SERVICE_DEF_STRING_SIZE  50
//## end module%37D387D00094.additionalDeclarations


//## begin ClServiceInterface%37D385E80269.preface preserve=yes
//## end ClServiceInterface%37D385E80269.preface

//## Class: ClServiceInterface%37D385E80269
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37D3905A007A;ClServiceQ { -> }
//## Uses: <unnamed>%37D3A6780266;ClOutputQ { -> }
//## Uses: <unnamed>%37FA1B28038A;ClServiceStats { -> F}
//## Uses: <unnamed>%37FB224F01F3;ClStatNumericalType { -> }

class ClServiceInterface : public ClBaseObject  //## Inherits: <unnamed>%37FB1ABE0374
{
  //## begin ClServiceInterface%37D385E80269.initialDeclarations preserve=yes
  //## end ClServiceInterface%37D385E80269.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetServiceQ%936606377
      virtual ClServiceQ * GetServiceQ () = 0;

      //## Operation: GetOutputQ%936606378
      virtual ClOutputQ * GetOutputQ () = 0;

      //## Operation: SetOutputQ%936606379; C++
      virtual void SetOutputQ (ClOutputQ *ai_pOutputQ) = 0;

      //## Operation: SetServiceQ%936606380
      virtual void SetServiceQ (ClServiceQ *ai_poService) = 0;

      //## Operation: Disable%938596864
      //	Disables a service. When a service has been disabled won't proccess any more
      //	request, and it will be erased by the Local monitor thread when no request
      //	remind inside.
      void Disable ();

      //## Operation: GetQueuedItems%939201740
      virtual ClStatNumericalType GetQueuedItems () = 0;

    // Additional Public Declarations
      //## begin ClServiceInterface%37D385E80269.public preserve=yes
      //## end ClServiceInterface%37D385E80269.public

  protected:
    // Additional Protected Declarations
      //## begin ClServiceInterface%37D385E80269.protected preserve=yes
      //## end ClServiceInterface%37D385E80269.protected

  private:
    // Additional Private Declarations
      //## begin ClServiceInterface%37D385E80269.private preserve=yes
      //## end ClServiceInterface%37D385E80269.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServiceInterface%37D385E80269.implementation preserve=yes
      //## end ClServiceInterface%37D385E80269.implementation

};

//## begin ClServiceInterface%37D385E80269.postscript preserve=yes
//## end ClServiceInterface%37D385E80269.postscript

// Class ClServiceInterface 


//## Other Operations (inline)
inline void ClServiceInterface::Disable ()
{
  //## begin ClServiceInterface::Disable%938596864.body preserve=yes
  //## end ClServiceInterface::Disable%938596864.body
}

//## begin module%37D387D00094.epilog preserve=yes
//## end module%37D387D00094.epilog


#endif
