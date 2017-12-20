//## begin module%34223FF70056.cm preserve=no
//## end module%34223FF70056.cm

//## begin module%34223FF70056.cp preserve=no
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
//## end module%34223FF70056.cp

//## Module: clarraylibres%34223FF70056; Package specification
//## Subsystem: M4IdGenerator::inc%37943573005E
//## Source file: F:\integration\m4idgenerator\inc\clarraylibres.hpp

#ifndef __CLARRAYLIBRES__HPP__
#define __CLARRAYLIBRES__HPP__ 1

//## begin module%34223FF70056.additionalIncludes preserve=no
#include "m4idgenerator_dll.hpp"
//## end module%34223FF70056.additionalIncludes

//## begin module%34223FF70056.includes preserve=yes
#ifdef _KCC
using std::deque;
#endif

#include <m4stl.hpp>
#include <m4string.hpp>
//## end module%34223FF70056.includes

// syncro
#include <syncro.hpp>
//## begin module%34223FF70056.declarations preserve=no
//## end module%34223FF70056.declarations

//## begin module%34223FF70056.additionalDeclarations preserve=yes
//## end module%34223FF70056.additionalDeclarations


//## begin dqArray%34223FFA03C0.preface preserve=yes
//## end dqArray%34223FFA03C0.preface

//## Class: dqArray%34223FFA03C0; Instantiated Class
//## Category: Dictionary%34183E8601C0
//## Subsystem: M4IdGenerator::inc%37943573005E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef deque< m4int32_t > dqArray;

//## begin dqArray%34223FFA03C0.postscript preserve=yes
//## end dqArray%34223FFA03C0.postscript

//## begin ClFreeArray%34223FFB0055.preface preserve=yes
//## end ClFreeArray%34223FFB0055.preface

//## Class: ClFreeArray%34223FFB0055
//	It's an util class wich porpouse is recover old IDs and reuse it in new
//	objects instead of going through all the list looking for free IDs. When an
//	object is deleted its ID become free and it is inserted in thit list waiting
//	for a new insertion for be used. If no ID apears in this list the ID
//	generator look for the higher ID and uses the inmediatly next for the new
//	object.
//## Category: M4IdGenerator%35EEBC57009C
//## Subsystem: M4IdGenerator::inc%37943573005E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClFreeArray 
{
  //## begin ClFreeArray%34223FFB0055.initialDeclarations preserve=yes
  //## end ClFreeArray%34223FFB0055.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClFreeArray%627743275; C++
      //	Default constructor
      ClFreeArray ();

    //## Destructor (specified)
      //## Operation: ~ClFreeArray%1086045097; C++
      //	Destructor
      ~ClFreeArray ();


    //## Other Operations (specified)
      //## Operation: GetFreeOne%-1480728616; C++
      //	Gets a handle and delete it from the array of free handles
      m4uint32_t GetFreeOne ();

      //## Operation: AddFreeOne%1522906610; C++
      //	Add a new free handle to the array
      void AddFreeOne (m4uint32_t ai_iFreeIndex);

    // Additional Public Declarations
      //## begin ClFreeArray%34223FFB0055.public preserve=yes
      //## end ClFreeArray%34223FFB0055.public

  protected:
    // Data Members for Associations

      //## Association: M4IdGenerator::<unnamed>%36E9344502F3
      //## Role: ClFreeArray::m_oMutex%36E934460268
      //## begin ClFreeArray::m_oMutex%36E934460268.role preserve=no  protected: static ClMutex { -> VHAN}
      static ClMutex m_oMutex;
      //## end ClFreeArray::m_oMutex%36E934460268.role

    // Additional Protected Declarations
      //## begin ClFreeArray%34223FFB0055.protected preserve=yes
      //## end ClFreeArray%34223FFB0055.protected

  private:
    // Data Members for Associations

      //## Association: Dictionary::<unnamed>%34223FFB012F
      //## Role: ClFreeArray::m_aiArray%34223FFB015E
      //	vector of ints of 32 bits
      //## begin ClFreeArray::m_aiArray%34223FFB015E.role preserve=no  private: dqArray {1 -> 1VHAPN}
      dqArray m_aiArray;
      //## end ClFreeArray::m_aiArray%34223FFB015E.role

    // Additional Private Declarations
      //## begin ClFreeArray%34223FFB0055.private preserve=yes
      //## end ClFreeArray%34223FFB0055.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFreeArray%34223FFB0055.implementation preserve=yes
      //## end ClFreeArray%34223FFB0055.implementation

};

//## begin ClFreeArray%34223FFB0055.postscript preserve=yes
//## end ClFreeArray%34223FFB0055.postscript

//## begin ClIdGenerator%34742BFC022F.preface preserve=yes
//## end ClIdGenerator%34742BFC022F.preface

//## Class: ClIdGenerator%34742BFC022F
//	Generates ids to index the items in the maps
//## Category: M4IdGenerator%35EEBC57009C
//## Subsystem: M4IdGenerator::inc%37943573005E
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IDGENERATOR ClIdGenerator
{
  //## begin ClIdGenerator%34742BFC022F.initialDeclarations preserve=yes
  //## end ClIdGenerator%34742BFC022F.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetNewId%880118278
      static m4uint32_t GetNewId ();

      //## Operation: SetNewId%880118279
      static m4return_t SetNewId (m4uint32_t ai_iId);

      //## Operation: GetInstanceName%967533896
      static m4string_t GetInstanceName ();

      //## Operation: SetInstanceName%967533897
      static m4return_t SetInstanceName (m4string_t ai_strName);

      //## Operation: SetServerKey%986803333
      static void SetServerKey (m4string_t ai_strHost, m4string_t ai_strBasePort);

      //## Operation: GetServerKey%986803334
      static m4return_t GetServerKey (m4string_t &ao_strHost, m4string_t &ao_strBasePort);

    // Additional Public Declarations
      //## begin ClIdGenerator%34742BFC022F.public preserve=yes
      //## end ClIdGenerator%34742BFC022F.public

  protected:
    // Data Members for Associations

      //## Association: M4IdGenerator::<unnamed>%36E939D90314
      //## Role: ClIdGenerator::m_oLibres%36E939DA015D
      //## begin ClIdGenerator::m_oLibres%36E939DA015D.role preserve=no  protected: static ClFreeArray { -> VHAN}
      static ClFreeArray m_oLibres;
      //## end ClIdGenerator::m_oLibres%36E939DA015D.role

    // Additional Protected Declarations
      //## begin ClIdGenerator%34742BFC022F.protected preserve=yes
      //## end ClIdGenerator%34742BFC022F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iMaxId%34742C650398
      //## begin ClIdGenerator::m_iMaxId%34742C650398.attr preserve=no  private: static m4uint32_t {UA} 1
      static m4uint32_t m_iMaxId;
      //## end ClIdGenerator::m_iMaxId%34742C650398.attr

      //## Attribute: m_strInstance%39AB8C2A0390
      //## begin ClIdGenerator::m_strInstance%39AB8C2A0390.attr preserve=no  private: static m4string_t {UA} ""
      static m4pchar_t m_pcInstance;
      //## end ClIdGenerator::m_strInstance%39AB8C2A0390.attr

      //## Attribute: m_strHost%3AD16BC10015
      //## begin ClIdGenerator::m_strHost%3AD16BC10015.attr preserve=no  private: static m4string_t {VA} ""
      static m4pchar_t m_pcHost;
      //## end ClIdGenerator::m_strHost%3AD16BC10015.attr

      //## Attribute: m_strBasePort%3AD16BD8000E
      //## begin ClIdGenerator::m_strBasePort%3AD16BD8000E.attr preserve=no  private: static m4string_t {VA} ""
      static m4pchar_t m_pcBasePort;
      //## end ClIdGenerator::m_strBasePort%3AD16BD8000E.attr

    // Additional Private Declarations
      //## begin ClIdGenerator%34742BFC022F.private preserve=yes
      //## end ClIdGenerator%34742BFC022F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClIdGenerator%34742BFC022F.implementation preserve=yes
      //## end ClIdGenerator%34742BFC022F.implementation

};

//## begin ClIdGenerator%34742BFC022F.postscript preserve=yes
//## end ClIdGenerator%34742BFC022F.postscript

// Class ClFreeArray 

// Class ClIdGenerator 

//## begin module%34223FF70056.epilog preserve=yes
//## end module%34223FF70056.epilog


#endif
