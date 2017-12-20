//## begin module%34223FFC0093.cm preserve=no
//## end module%34223FFC0093.cm

//## begin module%34223FFC0093.cp preserve=no
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
//## end module%34223FFC0093.cp

//## Module: clbasedictionary%34223FFC0093; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\clbasedictionary.hpp

#ifndef _CLBASEDICTIONARY_HPP_
#define _CLBASEDICTIONARY_HPP_ 1

//## begin module%34223FFC0093.additionalIncludes preserve=no
//## end module%34223FFC0093.additionalIncludes

//## begin module%34223FFC0093.includes preserve=yes
 
#include "m4stl.hpp"
//## end module%34223FFC0093.includes

// syncro
#include <syncro.hpp>
// clbaseobject
#include <clbaseobject.hpp>

class ClIdGenerator;

//## begin module%34223FFC0093.declarations preserve=no
//## end module%34223FFC0093.declarations

//## begin module%34223FFC0093.additionalDeclarations preserve=yes
//## end module%34223FFC0093.additionalDeclarations


//## begin ClDictionary%37943E690125.preface preserve=yes
//## end ClDictionary%37943E690125.preface

//## Class: ClDictionary%37943E690125
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37943EA30255;ClBaseObject { -> }

typedef map <m4uint64_t , ClBaseObject * , less < m4uint64_t > > ClDictionary;

//## begin ClDictionary%37943E690125.postscript preserve=yes
//## end ClDictionary%37943E690125.postscript

//## begin IterDictionary%37943DFA0356.preface preserve=yes
//## end IterDictionary%37943DFA0356.preface

//## Class: IterDictionary%37943DFA0356
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A406F8800AA;ClDictionary { -> }

typedef ClDictionary::iterator IterDictionary;

//## begin IterDictionary%37943DFA0356.postscript preserve=yes
//## end IterDictionary%37943DFA0356.postscript

//## begin IdList%3A3FA6CB00FB.preface preserve=yes
//## end IdList%3A3FA6CB00FB.preface

//## Class: IdList%3A3FA6CB00FB
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef deque < m4objid_t > IdList;

//## begin IdList%3A3FA6CB00FB.postscript preserve=yes
//## end IdList%3A3FA6CB00FB.postscript

//## begin ClBaseDictionary%34223FFE0258.preface preserve=yes
//## end ClBaseDictionary%34223FFE0258.preface

//## Class: ClBaseDictionary%34223FFE0258
//	The base classs container wich all dictionary types derive from.
//	It has all basic method for add remove and get items shorted by their ids.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%342240140304;ClBaseObject { -> }
//## Uses: <unnamed>%36A34D4202F9;ClIdGenerator { -> F}
//## Uses: <unnamed>%3A3FA6DF0316;IdList { -> }

class M4_DECL_M4SERVERBASE ClBaseDictionary 
{
  //## begin ClBaseDictionary%34223FFE0258.initialDeclarations preserve=yes
  //## end ClBaseDictionary%34223FFE0258.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBaseDictionary%-2015032989; C++
      //	Default constructor.
      ClBaseDictionary ();


    //## Other Operations (specified)
      //## Operation: AddItem%-769986080; C++
      //	Put into the dictionary an object looking for a valid id
      m4int64_t AddItem (ClBaseObject* ai_poBaseObject);

      //## Operation: AddItem%-179606071; C++
      //	Add a pair of value Id - Object in a dictionary
      m4return_t AddItem (m4int64_t ai_Id, ClBaseObject* ai_poBaseObject);

      //## Operation: RemoveItem%1696311704; C++
      //	Removes an entry of the dictionary with the passed Id
      m4return_t RemoveItem (m4int64_t ai_Id);

      //## Operation: DownItem%-1498754811; C++
      //	Shuts down an item by controled way
      m4return_t DownItem (m4int64_t ai_Id);

      //## Operation: GetItem%1028276320; C++
      //	Return the item refered by the id passed
      ClBaseObject* GetItem (m4int64_t ai_Id);

      //## Operation: GetNumItems%-1250346923; C++
      //	Returns the number of items in the
      m4uint64_t GetNumItems ();

      //## Operation: Lock%893077207
      void Lock ();

      //## Operation: Unlock%893077208
      void Unlock ();

      //## Operation: GetObjectList%977242950
      m4return_t GetObjectList (IdList &ai_oObjList);

    // Data Members for Class Attributes

      //## Attribute: m_iNumItems%3441FA1A0276
      //	The number of items the dictionary has
      //## begin ClBaseDictionary::m_iNumItems%3441FA1A0276.attr preserve=no  public: m4int32_t {UA} 
      m4int32_t m_iNumItems;
      //## end ClBaseDictionary::m_iNumItems%3441FA1A0276.attr

    // Data Members for Associations

      //## Association: Dictionary::<unnamed>%353B45D702A9
      //## Role: ClBaseDictionary::m_oMutex%353B45D900AD
      //## begin ClBaseDictionary::m_oMutex%353B45D900AD.role preserve=no  public: ClMutex { -> VHAN}
      ClMutex m_oMutex;
      //## end ClBaseDictionary::m_oMutex%353B45D900AD.role

    // Additional Public Declarations
      //## begin ClBaseDictionary%34223FFE0258.public preserve=yes
      //## end ClBaseDictionary%34223FFE0258.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%37947F9A0287
      //## Role: ClBaseDictionary:: m_mapDictionary%37947F9B00A7
      //## begin ClBaseDictionary:: m_mapDictionary%37947F9B00A7.role preserve=no  protected: ClDictionary { -> UHAN}
      ClDictionary m_mapDictionary;
      //## end ClBaseDictionary:: m_mapDictionary%37947F9B00A7.role

    // Additional Protected Declarations
      //## begin ClBaseDictionary%34223FFE0258.protected preserve=yes
      //## end ClBaseDictionary%34223FFE0258.protected

  private:

    //## Other Operations (specified)
      //## Operation: FindItem%-651399399; C++
      //	Get the iterator pointing to the searched record
//      IterDictionary FindItem (m4int64_t ai_Id);

    // Additional Private Declarations
      //## begin ClBaseDictionary%34223FFE0258.private preserve=yes
      //## end ClBaseDictionary%34223FFE0258.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBaseDictionary%34223FFE0258.implementation preserve=yes
      //## end ClBaseDictionary%34223FFE0258.implementation

};

//## begin ClBaseDictionary%34223FFE0258.postscript preserve=yes
//## end ClBaseDictionary%34223FFE0258.postscript

//## begin IterDictionary%342240100371.preface preserve=yes
//## end IterDictionary%342240100371.preface

//## Class: IterDictionary%342240100371
//	The iterator definition of the dictionary template class
//## Category: Dictionary%34183E8601C0
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef ClDictionary::iterator IterDictionary;

//## begin IterDictionary%342240100371.postscript preserve=yes
//## end IterDictionary%342240100371.postscript

// Class ClBaseDictionary 

//## begin module%34223FFC0093.epilog preserve=yes
//## end module%34223FFC0093.epilog


#endif
