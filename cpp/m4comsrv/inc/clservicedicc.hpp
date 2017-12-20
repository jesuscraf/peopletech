//## begin module%342240620055.cm preserve=no
//## end module%342240620055.cm

//## begin module%342240620055.cp preserve=no
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
//## end module%342240620055.cp

//## Module: clservicedicc%342240620055; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\version\inc\clservicedicc.hpp

#ifndef CLSERVICEDICTIONARY_HPP
#define CLSERVICEDICTIONARY_HPP 1

//## begin module%342240620055.additionalIncludes preserve=no
//## end module%342240620055.additionalIncludes

//## begin module%342240620055.includes preserve=yes
//## end module%342240620055.includes

// clbasedictionary
#include <clbasedictionary.hpp>

class ClLocalMonitor;
class ClServiceInfo;

//## begin module%342240620055.declarations preserve=no
//## end module%342240620055.declarations

//## begin module%342240620055.additionalDeclarations preserve=yes
//## end module%342240620055.additionalDeclarations


//## begin ClServiceDictionary%3422406200B2.preface preserve=yes
//## end ClServiceDictionary%3422406200B2.preface

//## Class: ClServiceDictionary%3422406200B2
//	Dictionary of services. It is a single list of opened services.
//## Category: Dictionary%34183E8601C0
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3455E42100C8;ClServiceInfo { -> F}
//## Uses: <unnamed>%347EFB2C0394;ClLocalMonitor { -> F}
//## Uses: <unnamed>%377C7582000E;ClRequest { -> F}

class ClServiceDictionary : public ClBaseDictionary  //## Inherits: <unnamed>%34224062013F
{
  //## begin ClServiceDictionary%3422406200B2.initialDeclarations preserve=yes
  //## end ClServiceDictionary%3422406200B2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClServiceDictionary%876496968
      //	Default constructor
      ClServiceDictionary ();

    //## Destructor (specified)
      //## Operation: ~ClServiceDictionary%876496969
      ~ClServiceDictionary ();


    //## Other Operations (specified)
      //## Operation: AddServiceOnLine%876496970
      //	Add a new service to the list of available services of the server on line,
      //	creating both the bath and the general groups. Each group will have a queue,
      //	and a list of launchers attached to the service group
      ClServiceInfo * AddServiceOnLine (m4int32_t ai_lIdService, ClLocalMonitor *ai_poLocalMonitor, m4uint16_t ai_iMaxNumQItem = FastDefUpperBound);

      //## Operation: RemoveService%876496971
      //	Remove one service from the list of services. Inded this method put a
      //	nonactive flag in the service and put it into a list of services pending to
      //	be erased
      m4return_t RemoveService (m4int32_t ai_lIdService);

      //## Operation: DownItem%876496972
      //	Remove an item from the dictionary.
      m4return_t DownItem (m4int32_t ai_lIdService);

      //## Operation: CheckThreads%878200305
      void CheckThreads ();

      //## Operation: ShutDown%879854976
      //	Shut down a service in a controled way
      m4return_t ShutDown ();

    // Additional Public Declarations
      //## begin ClServiceDictionary%3422406200B2.public preserve=yes
      //## end ClServiceDictionary%3422406200B2.public

  protected:
    // Additional Protected Declarations
      //## begin ClServiceDictionary%3422406200B2.protected preserve=yes
      //## end ClServiceDictionary%3422406200B2.protected

  private:
    // Additional Private Declarations
      //## begin ClServiceDictionary%3422406200B2.private preserve=yes
      //## end ClServiceDictionary%3422406200B2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServiceDictionary%3422406200B2.implementation preserve=yes
      //## end ClServiceDictionary%3422406200B2.implementation

};

//## begin ClServiceDictionary%3422406200B2.postscript preserve=yes
//## end ClServiceDictionary%3422406200B2.postscript

// Class ClServiceDictionary 

//## begin module%342240620055.epilog preserve=yes
//## end module%342240620055.epilog


#endif
