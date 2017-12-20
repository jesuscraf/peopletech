//## begin module%377768450171.cm preserve=no
//## end module%377768450171.cm

//## begin module%377768450171.cp preserve=no
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
//## end module%377768450171.cp

//## Module: windowstsap%377768450171; Package specification
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//	d:\compon\m4tsap\version\inc\win32
//## Source file: d:\compon\m4tsap\version\inc\win32\windowstsap.hpp

#ifndef windowstsap_h
#define windowstsap_h 1

//## begin module%377768450171.additionalIncludes preserve=no
//## end module%377768450171.additionalIncludes

//## begin module%377768450171.includes preserve=yes

#include "m4types.hpp"
#include "tsapimplfactory.hpp"
#include "windowstsapimpl.hpp"
#include "windowstsapselectimpl.hpp"
#include "ssltsapimpl.hpp"
//## end module%377768450171.includes

// tsapimpl
#include <tsapimpl.hpp>
// selectimpl
#include <selectimpl.hpp>
//## begin module%377768450171.declarations preserve=no
//## end module%377768450171.declarations

//## begin module%377768450171.additionalDeclarations preserve=yes

class	ClTSAPImpl;

//## end module%377768450171.additionalDeclarations


//## begin ClWindowsTSAPImplFactory%37776845017E.preface preserve=yes
//## end ClWindowsTSAPImplFactory%37776845017E.preface

//## Class: ClWindowsTSAPImplFactory%37776845017E
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377768450185;ClTSAPImpl { -> }
//## Uses: <unnamed>%377768450187;ClTSAPSelectImpl { -> }

class ClWindowsTSAPImplFactory : public ClTSAPImplFactory  //## Inherits: <unnamed>%377768450193
{
  //## begin ClWindowsTSAPImplFactory%37776845017E.initialDeclarations preserve=yes
  //## end ClWindowsTSAPImplFactory%37776845017E.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: CreateTSAPImpl%926707699; C++
      ClTSAPImpl* CreateTSAPImpl ()
      {
        //## begin ClWindowsTSAPImplFactory::CreateTSAPImpl%926707699.body preserve=yes
		return new ClWindowsTSAPImpl;
      
        //## end ClWindowsTSAPImplFactory::CreateTSAPImpl%926707699.body
      }

      //## Operation: CreateTSAPSelectImpl%1493039993; C++
      ClTSAPSelectImpl* CreateTSAPSelectImpl ()
      {
        //## begin ClWindowsTSAPImplFactory::CreateTSAPSelectImpl%1493039993.body preserve=yes
		return new ClWindowsTSAPSelectImpl;
      
        //## end ClWindowsTSAPImplFactory::CreateTSAPSelectImpl%1493039993.body
      }

      //## Operation: CreateTSAPSSLImpl%1641939259; C++
      ClTSAPImpl* CreateTSAPSSLImpl (int ai_iMode, void* ai_pData)
      {
        //## begin ClWindowsTSAPImplFactory::CreateTSAPSSLImpl%1641939259.body preserve=yes
		return new ClSSLTSAPImpl(ai_iMode, ai_pData);
      
        //## end ClWindowsTSAPImplFactory::CreateTSAPSSLImpl%1641939259.body
      }

    // Additional Public Declarations
      //## begin ClWindowsTSAPImplFactory%37776845017E.public preserve=yes
      //## end ClWindowsTSAPImplFactory%37776845017E.public

  protected:
    // Additional Protected Declarations
      //## begin ClWindowsTSAPImplFactory%37776845017E.protected preserve=yes
      //## end ClWindowsTSAPImplFactory%37776845017E.protected

  private:
    // Additional Private Declarations
      //## begin ClWindowsTSAPImplFactory%37776845017E.private preserve=yes
      //## end ClWindowsTSAPImplFactory%37776845017E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClWindowsTSAPImplFactory%37776845017E.implementation preserve=yes
      //## end ClWindowsTSAPImplFactory%37776845017E.implementation

};

//## begin ClWindowsTSAPImplFactory%37776845017E.postscript preserve=yes
//## end ClWindowsTSAPImplFactory%37776845017E.postscript

//## begin module%377768450171.epilog preserve=yes
//## end module%377768450171.epilog


#endif
