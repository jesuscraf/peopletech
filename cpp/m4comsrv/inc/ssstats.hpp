//## begin module%34EDD1BF0132.cm preserve=no
//## end module%34EDD1BF0132.cm

//## begin module%34EDD1BF0132.cp preserve=no
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
//## end module%34EDD1BF0132.cp

//## Module: ssstats%34EDD1BF0132; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\ssstats.hpp

#ifndef ssstats_h
#define ssstats_h 1

//## begin module%34EDD1BF0132.additionalIncludes preserve=no
//## end module%34EDD1BF0132.additionalIncludes

//## begin module%34EDD1BF0132.includes preserve=yes
//## end module%34EDD1BF0132.includes

#include "m4stl.hpp"
// syncro
#include <syncro.hpp>
// SessManager
#include <sessmanager.hpp>

class ClActiveClassStats;

//## begin module%34EDD1BF0132.declarations preserve=no
//## end module%34EDD1BF0132.declarations

//## begin module%34EDD1BF0132.additionalDeclarations preserve=yes
//## end module%34EDD1BF0132.additionalDeclarations


//## begin ClActiveListStats%34EDC7E90204.preface preserve=yes
//## end ClActiveListStats%34EDC7E90204.preface

//## Class: ClActiveListStats%34EDC7E90204
//	These class is a base class for all the subsystems witch means a group of
//	subsystems such as a list of senders or distributors or receivers...
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34EDD28B004F;ClActiveClassStats { -> F}

class ClActiveListStats 
{
  //## begin ClActiveListStats%34EDC7E90204.initialDeclarations preserve=yes
  //## end ClActiveListStats%34EDC7E90204.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClActiveListStats%888317705
      ClActiveListStats ();

    //## Destructor (specified)
      //## Operation: ~ClActiveListStats%888317706
      ~ClActiveListStats ();


    //## Other Operations (specified)
      //## Operation: AddActiveClassStat%888225156
      void AddActiveClassStat (ClActiveClassStats *ai_poStat);

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%34EDD09D0135
      //## Role: ClActiveListStats::m_oStatsList%34EDD09D03A2
      //## begin ClActiveListStats::m_oStatsList%34EDD09D03A2.role preserve=no  public: list { -> VHAN}
      list < ClActiveClassStats * >  m_oStatsList;
      //## end ClActiveListStats::m_oStatsList%34EDD09D03A2.role

      //## Association: Subsystem::<unnamed>%374531BB0305
      //## Role: ClActiveListStats::m_oMutex%374531BD0004
      //## begin ClActiveListStats::m_oMutex%374531BD0004.role preserve=no  public: ClMutex { -> VHAN}
      ClMutex m_oMutex;
      //## end ClActiveListStats::m_oMutex%374531BD0004.role

    // Additional Public Declarations
      //## begin ClActiveListStats%34EDC7E90204.public preserve=yes
      //## end ClActiveListStats%34EDC7E90204.public

  protected:
    // Additional Protected Declarations
      //## begin ClActiveListStats%34EDC7E90204.protected preserve=yes
      //## end ClActiveListStats%34EDC7E90204.protected

  private:
    // Additional Private Declarations
      //## begin ClActiveListStats%34EDC7E90204.private preserve=yes
      //## end ClActiveListStats%34EDC7E90204.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClActiveListStats%34EDC7E90204.implementation preserve=yes
      //## end ClActiveListStats%34EDC7E90204.implementation

};

//## begin ClActiveListStats%34EDC7E90204.postscript preserve=yes
//## end ClActiveListStats%34EDC7E90204.postscript

//## begin ClSessManagerListStats%34EDCCB6034C.preface preserve=yes
//## end ClSessManagerListStats%34EDCCB6034C.preface

//## Class: ClSessManagerListStats%34EDCCB6034C
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSessManagerListStats : public ClSessManagerStats, //## Inherits: <unnamed>%34EDCD8C00A0
    	public ClActiveListStats  //## Inherits: <unnamed>%34EDCE140150
{
  //## begin ClSessManagerListStats%34EDCCB6034C.initialDeclarations preserve=yes
  //## end ClSessManagerListStats%34EDCCB6034C.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClSessManagerListStats%34EDCCB6034C.public preserve=yes
      //## end ClSessManagerListStats%34EDCCB6034C.public

  protected:
    // Additional Protected Declarations
      //## begin ClSessManagerListStats%34EDCCB6034C.protected preserve=yes
      //## end ClSessManagerListStats%34EDCCB6034C.protected

  private:
    // Additional Private Declarations
      //## begin ClSessManagerListStats%34EDCCB6034C.private preserve=yes
      //## end ClSessManagerListStats%34EDCCB6034C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSessManagerListStats%34EDCCB6034C.implementation preserve=yes
      //## end ClSessManagerListStats%34EDCCB6034C.implementation

};

//## begin ClSessManagerListStats%34EDCCB6034C.postscript preserve=yes
//## end ClSessManagerListStats%34EDCCB6034C.postscript

// Class ClActiveListStats 

// Class ClSessManagerListStats 

//## begin module%34EDD1BF0132.epilog preserve=yes
//## end module%34EDD1BF0132.epilog


#endif
