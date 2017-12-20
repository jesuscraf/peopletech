//## begin module%3742D0200387.cm preserve=no
//## end module%3742D0200387.cm

//## begin module%3742D0200387.cp preserve=no
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
//## end module%3742D0200387.cp

//## Module: syncronization%3742D0200387; Package specification
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//	d:\fuentesservidor\m4ipcs\version\inc
//## Source file: D:\FuentesServidor\m4ipcs\version\inc\m4syncronization.hpp

#ifndef _M4SYNCRONIZATION_HPP
#define _M4SYNCRONIZATION_HPP 1

//## begin module%3742D0200387.additionalIncludes preserve=no
//## end module%3742D0200387.additionalIncludes

//## begin module%3742D0200387.includes preserve=yes
#include <m4ipcs_dll.hpp>
//## end module%3742D0200387.includes

// m4types
#include <m4types.hpp>
//## begin module%3742D0200387.declarations preserve=no
//## end module%3742D0200387.declarations

//## begin module%3742D0200387.additionalDeclarations preserve=yes

#ifdef _UNIX
/*
 * _USE_ALGORITHM define si se va a usar el algoritmo que
 * condition y mutex.
 *
 * Si estamos en HP version 10.20, no existen los objetos rwlock_t,
 * y ha de usarse por fuerza el método anterior.
 *
 * Si estamos en Windows, usamos un algoritmo específico para Windows.
 *
 * En el resto de los casos, usamos rwlock_t
 */

#if defined _USE_ALGORITHM || defined _M4HPUX1020_

#include <pthread.h>


typedef struct
{
	pthread_mutex_t	m;
	int				rwlock;
	pthread_cond_t	readers_ok;
	unsigned	int	waiting_writers;
	pthread_cond_t	writer_ok;
} rwl_t;

class ClWriterReaders
{
  public:
      ClWriterReaders();
      ~ClWriterReaders();
      void EnterReader();
      void EnterWriter();
      void LeaveReader();
      void LeaveWriter();
  protected:
  private:
	  rwl_t		m_rwlp;
	  m4bool_t	m_bError;
};

#else	// De _ALGORITHM or _M4HPUX1020_
#include <pthread.h>
#ifdef _SOLARIS
#include <synch.h>
#endif


class ClWriterReaders
{
  public:
      ClWriterReaders();
      ~ClWriterReaders();
      void EnterReader();
      void EnterWriter();
      void LeaveReader();
      void LeaveWriter();
  protected:
  private:
#ifdef _SOLARIS
	  rwlock_t			m_rwlock;
#else
	  pthread_rwlock_t	m_rwlock;
#endif
	  m4bool_t			m_bError;
};

#endif	// De _ALGORITHM or _M4HPUX1020_

#else	// No UNIX, luego windows

#include <windows.h>
//## end module%3742D0200387.additionalDeclarations


//## begin ClWriterReaders%3742D0200392.preface preserve=yes
//## end ClWriterReaders%3742D0200392.preface

//## Class: ClWriterReaders%3742D0200392
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClWriterReaders
{
  //## begin ClWriterReaders%3742D0200392.initialDeclarations preserve=yes
  //## end ClWriterReaders%3742D0200392.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClWriterReaders%-337753720; C++
      ClWriterReaders ();

    //## Destructor (specified)
      //## Operation: ~ClWriterReaders%-1216499026; C++
      ~ClWriterReaders ();


    //## Other Operations (specified)
      //## Operation: EnterReader%1757037252; C++
      void EnterReader ();

      //## Operation: EnterWriter%-1659769292; C++
      void EnterWriter ();

      //## Operation: LeaveReader%-2086358376; C++
      void LeaveReader ();

      //## Operation: LeaveWriter%-1432057576; C++
      void LeaveWriter ();

    // Additional Public Declarations
      //## begin ClWriterReaders%3742D0200392.public preserve=yes
      //## end ClWriterReaders%3742D0200392.public

  protected:
    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743CFAF0055
      //## Role: ClWriterReaders::m_bError%3743CFAF03DB
      //## begin ClWriterReaders::m_bError%3743CFAF03DB.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bError;
      //## end ClWriterReaders::m_bError%3743CFAF03DB.role

    // Additional Protected Declarations
      //## begin ClWriterReaders%3742D0200392.protected preserve=yes
      //## end ClWriterReaders%3742D0200392.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: hReaderEvent%3742D02003C3
      //## begin ClWriterReaders::hReaderEvent%3742D02003C3.attr preserve=no  private: HANDLE {VA} 
      HANDLE hReaderEvent;
      //## end ClWriterReaders::hReaderEvent%3742D02003C3.attr

      //## Attribute: hMutex%3742D02003C5
      //## begin ClWriterReaders::hMutex%3742D02003C5.attr preserve=no  private: HANDLE {VA} 
      HANDLE hMutex;
      //## end ClWriterReaders::hMutex%3742D02003C5.attr

      //## Attribute: hWriterMutex%3742D02003C7
      //## begin ClWriterReaders::hWriterMutex%3742D02003C7.attr preserve=no  private: HANDLE {VA} 
      HANDLE hWriterMutex;
      //## end ClWriterReaders::hWriterMutex%3742D02003C7.attr

      //## Attribute: iCounter%3742D02003C9
      //## begin ClWriterReaders::iCounter%3742D02003C9.attr preserve=no  private: long {VA} 
      long iCounter;
      //## end ClWriterReaders::iCounter%3742D02003C9.attr

    // Additional Private Declarations
      //## begin ClWriterReaders%3742D0200392.private preserve=yes
      //## end ClWriterReaders%3742D0200392.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClWriterReaders%3742D0200392.implementation preserve=yes
      //## end ClWriterReaders%3742D0200392.implementation

};

//## begin ClWriterReaders%3742D0200392.postscript preserve=yes
//## end ClWriterReaders%3742D0200392.postscript

// Class ClWriterReaders 

//## begin module%3742D0200387.epilog preserve=yes

#endif	// De Windows

//## end module%3742D0200387.epilog


#endif
