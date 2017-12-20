//## begin module%3777684500B3.cm preserve=no
//## end module%3777684500B3.cm

//## begin module%3777684500B3.cp preserve=no
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
//## end module%3777684500B3.cp

//## Module: tsapimplfactory%3777684500B3; Package specification
//## Subsystem: M4Tsap::inc%37776840008D
//	d:\compon\m4tsap\version\inc
//## Source file: d:\compon\m4tsap\version\inc\tsapimplfactory.hpp

#ifndef tsapimplfactory_h
#define tsapimplfactory_h 1

//## begin module%3777684500B3.additionalIncludes preserve=no
//## end module%3777684500B3.additionalIncludes

//## begin module%3777684500B3.includes preserve=yes

#include "m4types.hpp"
//## end module%3777684500B3.includes

// tsapimpl
#include <tsapimpl.hpp>
// selectimpl
#include <selectimpl.hpp>
//## begin module%3777684500B3.declarations preserve=no
//## end module%3777684500B3.declarations

//## begin module%3777684500B3.additionalDeclarations preserve=yes

class ClTSAPImpl;
class ClTSAPSelectImpl;

//## end module%3777684500B3.additionalDeclarations


//## begin ClTSAPImplFactory%377768440343.preface preserve=yes
//## end ClTSAPImplFactory%377768440343.preface

//## Class: ClTSAPImplFactory%377768440343; Abstract
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3777684500C7;ClTSAPImpl { -> }
//## Uses: <unnamed>%3777684500D1;ClTSAPSelectImpl { -> }

class ClTSAPImplFactory 
{
  //## begin ClTSAPImplFactory%377768440343.initialDeclarations preserve=yes
  //## end ClTSAPImplFactory%377768440343.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: CreateTSAPImpl%-243197977; C++
      virtual ClTSAPImpl* CreateTSAPImpl () = 0;

      //## Operation: CreateTSAPSelectImpl%-1359520524; C++
      virtual ClTSAPSelectImpl* CreateTSAPSelectImpl () = 0;

      //## Operation: CreateTSAPSSLImpl%-727063395; C++
      virtual ClTSAPImpl* CreateTSAPSSLImpl (int ai_iMode, void* ai_pData) = 0;

    // Additional Public Declarations
      //## begin ClTSAPImplFactory%377768440343.public preserve=yes
      //## end ClTSAPImplFactory%377768440343.public

  protected:
    // Additional Protected Declarations
      //## begin ClTSAPImplFactory%377768440343.protected preserve=yes
      //## end ClTSAPImplFactory%377768440343.protected

  private:
    // Additional Private Declarations
      //## begin ClTSAPImplFactory%377768440343.private preserve=yes
      //## end ClTSAPImplFactory%377768440343.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTSAPImplFactory%377768440343.implementation preserve=yes
      //## end ClTSAPImplFactory%377768440343.implementation

};

//## begin ClTSAPImplFactory%377768440343.postscript preserve=yes
//## end ClTSAPImplFactory%377768440343.postscript

// Class ClTSAPImplFactory 

//## begin module%3777684500B3.epilog preserve=yes
//## end module%3777684500B3.epilog


#endif
