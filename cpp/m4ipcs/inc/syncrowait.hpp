//## begin module%37443D8A03DC.cm preserve=no
//## end module%37443D8A03DC.cm

//## begin module%37443D8A03DC.cp preserve=no
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
//## end module%37443D8A03DC.cp

//## Module: syncrowait%37443D8A03DC; Package specification
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//	d:\fuentesservidor\m4ipcs\version\inc
//## Source file: D:\FuentesServidor\m4ipcs\version\inc\syncrowait.hpp

#ifndef syncrowait_h
#define syncrowait_h 1

//## begin module%37443D8A03DC.additionalIncludes preserve=no
//## end module%37443D8A03DC.additionalIncludes

//## begin module%37443D8A03DC.includes preserve=yes
//## end module%37443D8A03DC.includes

// boolcondition
#include <boolcondition.hpp>
// syncro
#include <syncro.hpp>
//## begin module%37443D8A03DC.declarations preserve=no
//## end module%37443D8A03DC.declarations

//## begin module%37443D8A03DC.additionalDeclarations preserve=yes
//## end module%37443D8A03DC.additionalDeclarations


//## begin ClSyncroWait%37443D1A01A1.preface preserve=yes
//## end ClSyncroWait%37443D1A01A1.preface

//## Class: ClSyncroWait%37443D1A01A1
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClSyncroWait : public ClSynObject  //## Inherits: <unnamed>%3744447E0237
{
  //## begin ClSyncroWait%37443D1A01A1.initialDeclarations preserve=yes
  //## end ClSyncroWait%37443D1A01A1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSyncroWait%927221338
      ClSyncroWait ();

    //## Destructor (specified)
      //## Operation: ~ClSyncroWait%927221339
      ~ClSyncroWait ();


    //## Other Operations (specified)
      //## Operation: Init%927221340
      m4return_t Init (m4int32_t ai_iMaxState = 1, m4int32_t ai_iInitialState = 1);

      //## Operation: Lock%927221335; C++
      m4return_t Lock (void );

      //## Operation: Lock%927221337; C++
      m4return_t Lock (m4uint32_t ai_iTimeOut);

      //## Operation: Unlock%927221336; C++
      m4return_t Unlock (void );

    // Additional Public Declarations
      //## begin ClSyncroWait%37443D1A01A1.public preserve=yes
      //## end ClSyncroWait%37443D1A01A1.public

  protected:
    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3744448F0232
      //## Role: ClSyncroWait::m_poBoolCond%3744449001C5
      //## begin ClSyncroWait::m_poBoolCond%3744449001C5.role preserve=no  public: ClBooleanCondition {1 -> 1RHN}
      ClBooleanCondition *m_poBoolCond;
      //## end ClSyncroWait::m_poBoolCond%3744449001C5.role

      //## Association: M4Ipcs::<unnamed>%3744453B001C
      //## Role: ClSyncroWait::m_poMutex%3744453B017B
      //## begin ClSyncroWait::m_poMutex%3744453B017B.role preserve=no  public: ClMutex {1 -> 1RHN}
      ClMutex *m_poMutex;
      //## end ClSyncroWait::m_poMutex%3744453B017B.role

    // Additional Protected Declarations
      //## begin ClSyncroWait%37443D1A01A1.protected preserve=yes
      //## end ClSyncroWait%37443D1A01A1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bIsInitialized%374445D40008
      //## begin ClSyncroWait::m_bIsInitialized%374445D40008.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsInitialized;
      //## end ClSyncroWait::m_bIsInitialized%374445D40008.attr

      //## Attribute: m_iActualState%3745105601B4
      //## begin ClSyncroWait::m_iActualState%3745105601B4.attr preserve=no  private: m4int32_t {UA} 1
      m4int32_t m_iActualState;
      //## end ClSyncroWait::m_iActualState%3745105601B4.attr

      //## Attribute: m_iMaxState%37451069039C
      //## begin ClSyncroWait::m_iMaxState%37451069039C.attr preserve=no  private: m4int32_t {UA} 1
      m4int32_t m_iMaxState;
      //## end ClSyncroWait::m_iMaxState%37451069039C.attr

    // Additional Private Declarations
      //## begin ClSyncroWait%37443D1A01A1.private preserve=yes
      //## end ClSyncroWait%37443D1A01A1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSyncroWait%37443D1A01A1.implementation preserve=yes
      //## end ClSyncroWait%37443D1A01A1.implementation

};

//## begin ClSyncroWait%37443D1A01A1.postscript preserve=yes
//## end ClSyncroWait%37443D1A01A1.postscript

// Class ClSyncroWait 

//## begin module%37443D8A03DC.epilog preserve=yes
//## end module%37443D8A03DC.epilog


#endif
