//## begin module%3742D01F037B.cm preserve=no
//## end module%3742D01F037B.cm

//## begin module%3742D01F037B.cp preserve=no
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
//## end module%3742D01F037B.cp

//## Module: m4condition%3742D01F037B; Package specification
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//	d:\fuentesservidor\m4ipcs\version\inc
//## Source file: D:\FuentesServidor\m4ipcs\version\inc\m4condition.hpp

#ifndef _M4CONDITION_HPP
#define _M4CONDITION_HPP 1

//## begin module%3742D01F037B.additionalIncludes preserve=no
//## end module%3742D01F037B.additionalIncludes

//## begin module%3742D01F037B.includes preserve=yes

#ifdef _UNIX
	#include  <pthread.h>
	#include  <sys/errno.h>
	#include  <sys/time.h>
	#include  <time.h>
	typedef pthread_mutex_t m4mutex_t;
	typedef pthread_cond_t m4cond_t;
#elif defined (WINDOWS) || defined (_WINDOWS)
	#include <windows.h>
	typedef HANDLE m4cond_t;
	typedef HANDLE m4mutex_t;
#endif

#define	M4_COND_SIGNALED	2
#define	M4_COND_TIMEOUT		3
#define	M4_COND_ERROR		4

#include <m4ipcs_dll.hpp>
//## end module%3742D01F037B.includes

// m4types
#include <m4types.hpp>
//## begin module%3742D01F037B.declarations preserve=no
//## end module%3742D01F037B.declarations

//## begin module%3742D01F037B.additionalDeclarations preserve=yes
//## end module%3742D01F037B.additionalDeclarations


//## begin ClCondition%3742D01F038F.preface preserve=yes
//## end ClCondition%3742D01F038F.preface

//## Class: ClCondition%3742D01F038F
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClCondition 
{
  //## begin ClCondition%3742D01F038F.initialDeclarations preserve=yes
  //## end ClCondition%3742D01F038F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCondition%-1099979947; C++
      ClCondition ();

    //## Destructor (specified)
      //## Operation: ~ClCondition%858029057; C++
      ~ClCondition ();


    //## Other Operations (specified)
      //## Operation: Init%-196230685; C++
      m4return_t Init ();

      //## Operation: Wait%-733722643; C++
      m4int16_t Wait (m4int32_t millisecs = -1);

      //## Operation: Signal%715533459; C++
      m4return_t Signal (void );

      //## Operation: Close%-216214797; C++
      m4return_t Close ();

      //## Operation: Reset%-686858407; C++
      m4return_t Reset ();

    // Additional Public Declarations
      //## begin ClCondition%3742D01F038F.public preserve=yes
      //## end ClCondition%3742D01F038F.public

  protected:
    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743CCBE0002
      //## Role: ClCondition::m_create%3743CCBE0338
      //## begin ClCondition::m_create%3743CCBE0338.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_create;
      //## end ClCondition::m_create%3743CCBE0338.role

      //## Association: M4Ipcs::<unnamed>%3743CCF302B1
      //## Role: ClCondition::m_terminate%3743CCF40173
      //## begin ClCondition::m_terminate%3743CCF40173.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_terminate;
      //## end ClCondition::m_terminate%3743CCF40173.role

    // Additional Protected Declarations
      //## begin ClCondition%3742D01F038F.protected preserve=yes
      //## end ClCondition%3742D01F038F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_hHandleCondVar%3742D0200322
      //## begin ClCondition::m_hHandleCondVar%3742D0200322.attr preserve=no  private: m4cond_t {VA} 
      m4cond_t m_hHandleCondVar;
      //## end ClCondition::m_hHandleCondVar%3742D0200322.attr

    // Additional Private Declarations
      //## begin ClCondition%3742D01F038F.private preserve=yes
	#ifdef _UNIX
		m4mutex_t m_hHandleMutexCond;
	#endif
      //## end ClCondition%3742D01F038F.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCondition%3742D01F038F.implementation preserve=yes
      //## end ClCondition%3742D01F038F.implementation

};

//## begin ClCondition%3742D01F038F.postscript preserve=yes
//## end ClCondition%3742D01F038F.postscript

// Class ClCondition 

//## begin module%3742D01F037B.epilog preserve=yes
//## end module%3742D01F037B.epilog


#endif
