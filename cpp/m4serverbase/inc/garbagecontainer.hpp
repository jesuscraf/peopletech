//## begin module%39B7B9C50153.cm preserve=no
//## end module%39B7B9C50153.cm

//## begin module%39B7B9C50153.cp preserve=no
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
//## end module%39B7B9C50153.cp

//## Module: garbagecontainer%39B7B9C50153; Package specification
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Source file: F:\integration\m4serverbase\inc\garbagecontainer.hpp

#ifndef garbagecontainer_h
#define garbagecontainer_h 1

//## begin module%39B7B9C50153.additionalIncludes preserve=no
//## end module%39B7B9C50153.additionalIncludes

//## begin module%39B7B9C50153.includes preserve=yes
#include <m4serverbase_dll.hpp>
#include <fastfifoq.hpp>
//## end module%39B7B9C50153.includes

// CriticalSection
#include <criticalsection.hpp>
// garbagecollectorinterface
#include <garbagecollectorinterface.hpp>

class IGarbageable;

//## begin module%39B7B9C50153.declarations preserve=no
//## end module%39B7B9C50153.declarations

//## begin module%39B7B9C50153.additionalDeclarations preserve=yes
//## end module%39B7B9C50153.additionalDeclarations


//## Class: TGabageableContainer%3950C36E03BF; Instantiated Class
//## Category: M4ServerBase::GarbageCollector%3820504A014C
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3950C36E03C1;IGarbageable { -> F}

typedef FastQueue< IGarbageable * > TGabageableContainer;

//## begin ClGarbageContainer%39B8D825019B.preface preserve=yes
//## end ClGarbageContainer%39B8D825019B.preface

//## Class: ClGarbageContainer%39B8D825019B
//## Category: M4ServerBase::GarbageCollector%3820504A014C
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClGarbageContainer : public IGarbageCollector  //## Inherits: <unnamed>%39B8DA5B004B
{
  //## begin ClGarbageContainer%39B8D825019B.initialDeclarations preserve=yes
  //## end ClGarbageContainer%39B8D825019B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClGarbageContainer%968401157
      ClGarbageContainer (m4int_t ai_iContainerSize);

    //## Destructor (specified)
      //## Operation: ~ClGarbageContainer%968401158
      ~ClGarbageContainer ();


    //## Other Operations (specified)
      //## Operation: GetElement%968401153
      m4return_t GetElement (IRecycleable *&ao_poElement);

      //## Operation: SetElement%968401154
      m4return_t SetElement (IRecycleable &ai_oElement);

      //## Operation: DisposeOf%968401155; C++
      m4return_t DisposeOf (IGarbageable &ai_oGarbage);

      //## Operation: Purge%968401156; C++
      m4uint32_t Purge ();

    // Additional Public Declarations
      //## begin ClGarbageContainer%39B8D825019B.public preserve=yes
      //## end ClGarbageContainer%39B8D825019B.public

  protected:

    //## Other Operations (specified)
      //## Operation: _Purge%968401159; C++
      m4uint32_t _Purge ();

    // Data Members for Associations

      //## Association: M4ServerBase::GarbageCollector::<unnamed>%39B8D983034D
      //## Role: ClGarbageContainer::m_oGarbageContainer%39B8D984020E
      //## begin ClGarbageContainer::m_oGarbageContainer%39B8D984020E.role preserve=no  protected: TGabageableContainer { -> VHAN}
      TGabageableContainer m_oGarbageContainer;
      //## end ClGarbageContainer::m_oGarbageContainer%39B8D984020E.role

      //## Association: M4ServerBase::GarbageCollector::<unnamed>%39B8E00603C2
      //## Role: ClGarbageContainer::m_oBlocker%39B8E00702E7
      //## begin ClGarbageContainer::m_oBlocker%39B8E00702E7.role preserve=no  protected: ClCriticalSection { -> VHAN}
      ClCriticalSection m_oBlocker;
      //## end ClGarbageContainer::m_oBlocker%39B8E00702E7.role

    // Additional Protected Declarations
      //## begin ClGarbageContainer%39B8D825019B.protected preserve=yes
      //## end ClGarbageContainer%39B8D825019B.protected

  private:
    // Additional Private Declarations
      //## begin ClGarbageContainer%39B8D825019B.private preserve=yes
      //## end ClGarbageContainer%39B8D825019B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClGarbageContainer%39B8D825019B.implementation preserve=yes
      //## end ClGarbageContainer%39B8D825019B.implementation

};

//## begin ClGarbageContainer%39B8D825019B.postscript preserve=yes
//## end ClGarbageContainer%39B8D825019B.postscript

// Class ClGarbageContainer 

//## begin module%39B7B9C50153.epilog preserve=yes
//## end module%39B7B9C50153.epilog


#endif
