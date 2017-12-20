//## begin module%34223FFC020A.cm preserve=no
//## end module%34223FFC020A.cm

//## begin module%34223FFC020A.cp preserve=no
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
//## end module%34223FFC020A.cp

//## Module: clbaseobject%34223FFC020A; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\clbaseobject.hpp

#ifndef __CLBASEOBJECT__HPP__
#define __CLBASEOBJECT__HPP__ 1

//## begin module%34223FFC020A.additionalIncludes preserve=no
#include <m4serverbase_dll.hpp>
//## end module%34223FFC020A.additionalIncludes

//## begin module%34223FFC020A.includes preserve=yes
#include <m4win.hpp>
//## end module%34223FFC020A.includes

// m4types
#include <m4types.hpp>
// clautogarbageable
#include <clautogarbageable.hpp>

class M4_DECL_M4SERVERBASE ClStatistic;

//## begin module%34223FFC020A.declarations preserve=no
//## end module%34223FFC020A.declarations

//## begin module%34223FFC020A.additionalDeclarations preserve=yes
//## end module%34223FFC020A.additionalDeclarations


//## begin m4objid_t%39EAECCA0357.preface preserve=yes
//## end m4objid_t%39EAECCA0357.preface

//## Class: m4objid_t%39EAECCA0357
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef m4uint64_t m4objid_t;

//## begin m4objid_t%39EAECCA0357.postscript preserve=yes
//## end m4objid_t%39EAECCA0357.postscript

//## begin ClBaseObject%3422401402D5.preface preserve=yes
//## end ClBaseObject%3422401402D5.preface

//## Class: ClBaseObject%3422401402D5
//	This is a base class used in all dictionaries for the base class dictionary.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%345F08730278;m4return_t { -> }
//## Uses: <unnamed>%37947DB90361;m4uint32_t { -> F}
//## Uses: <unnamed>%39EAECE800CF;m4objid_t { -> }

class M4_DECL_M4SERVERBASE ClBaseObject : public ClAutoGarbageable  //## Inherits: <unnamed>%38205DC8022E
{
  //## begin ClBaseObject%3422401402D5.initialDeclarations preserve=yes
  //## end ClBaseObject%3422401402D5.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClBaseObject%890086939
      virtual ~ClBaseObject ();


    //## Other Operations (specified)
      //## Operation: GetId%887966446
      //	Return the object Id
      m4objid_t GetId ();

      //## Operation: SetId%887966447
      //	Sets the id to that object
      void SetId (m4objid_t ai_lId);

      //## Operation: GetStats%890086940
      ClStatistic * GetStats ();

    // Additional Public Declarations
      //## begin ClBaseObject%3422401402D5.public preserve=yes
      //## end ClBaseObject%3422401402D5.public

  protected:
    //## Constructors (specified)
      //## Operation: ClBaseObject%890086938
      ClBaseObject (ClStatistic *ai_poStatistic = NULL);

    // Data Members for Class Attributes

      //## Attribute: m_lId%343B9B2703B9
      //## begin ClBaseObject::m_lId%343B9B2703B9.attr preserve=no  protected: m4objid_t {UA} 
      m4objid_t m_lId;
      //## end ClBaseObject::m_lId%343B9B2703B9.attr

    // Data Members for Associations

      //## Association: Dictionary::<unnamed>%350D6FB5034E
      //## Role: ClBaseObject::m_poStatistic%350D6FB70166
      //## begin ClBaseObject::m_poStatistic%350D6FB70166.role preserve=no  protected: ClStatistic { -> RFHAN}
      ClStatistic *m_poStatistic;
      //## end ClBaseObject::m_poStatistic%350D6FB70166.role

    // Additional Protected Declarations
      //## begin ClBaseObject%3422401402D5.protected preserve=yes
      //## end ClBaseObject%3422401402D5.protected

  private:
    // Additional Private Declarations
      //## begin ClBaseObject%3422401402D5.private preserve=yes
      //## end ClBaseObject%3422401402D5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBaseObject%3422401402D5.implementation preserve=yes
      //## end ClBaseObject%3422401402D5.implementation

};

//## begin ClBaseObject%3422401402D5.postscript preserve=yes
//## end ClBaseObject%3422401402D5.postscript

// Class ClBaseObject 

//## begin module%34223FFC020A.epilog preserve=yes
//## end module%34223FFC020A.epilog


#endif
