//## begin module%3427D3D701E1.cm preserve=no
//## end module%3427D3D701E1.cm

//## begin module%3427D3D701E1.cp preserve=no
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
//## end module%3427D3D701E1.cp

//## Module: cllist%3427D3D701E1; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\cllist.hpp

#ifndef cllist_h
#define cllist_h 1

//## begin module%3427D3D701E1.additionalIncludes preserve=no
//## end module%3427D3D701E1.additionalIncludes

//## begin module%3427D3D701E1.includes preserve=yes

#include "m4stl.hpp"
#include "m4thread.hpp"
//## end module%3427D3D701E1.includes

// syncronization
#include <m4syncronization.hpp>
// clarraylibres
#include <clarraylibres.hpp>

class ClActiveList;
class ClLocalMonitor;

//## begin module%3427D3D701E1.declarations preserve=no
//## end module%3427D3D701E1.declarations

//## begin module%3427D3D701E1.additionalDeclarations preserve=yes
//## end module%3427D3D701E1.additionalDeclarations


//## begin ActiveMap%3507C8470385.preface preserve=yes
//## end ActiveMap%3507C8470385.preface

//## Class: ActiveMap%3507C8470385
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3507C8B00277;map { -> }

typedef map <  M4Thread * ,m4objid_t , less < M4Thread * > > ActiveMap;

//## begin ActiveMap%3507C8470385.postscript preserve=yes
//## end ActiveMap%3507C8470385.postscript

//## begin ActiveIterator%3507C7DF0168.preface preserve=yes
//## end ActiveIterator%3507C7DF0168.preface

//## Class: ActiveIterator%3507C7DF0168
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3507CA530099;ActiveMap { -> }

typedef ActiveMap::iterator ActiveIterator;

//## begin ActiveIterator%3507C7DF0168.postscript preserve=yes
//## end ActiveIterator%3507C7DF0168.postscript

//## begin ClList%3430BFB102DB.preface preserve=yes
//## end ClList%3430BFB102DB.preface

//## Class: ClList%3430BFB102DB
//	Thread container base class for all the active members in the server. For
//	each type of diferent active members will be one difernet class definition.
//	this class is a son of template container  list class.
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%348E8E480105;ClActiveList { -> F}
//## Uses: <unnamed>%3507C83503B1;ActiveIterator { -> }

class ClList : public ClWriterReaders  //## Inherits: <unnamed>%35F40E5702B0
{
  //## begin ClList%3430BFB102DB.initialDeclarations preserve=yes
  //## end ClList%3430BFB102DB.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClList%880628514
      ClList (ClActiveList *ai_poActiveList = NULL);

    //## Destructor (specified)
      //## Operation: ~ClList%880628515
      ~ClList ();


    //## Other Operations (specified)
      //## Operation: AddItem%875610307
      m4return_t AddItem (M4Thread *ai_item);

      //## Operation: RemoveItem%875610308
      m4return_t RemoveItem (M4Thread *ai_pItem);

      //## Operation: GetItem%877008212
      M4Thread * GetItem ();

      //## Operation: ShutDown%877943017
      //	This method look all the items in the list and taking then as threads
      //	terminates all and removes  then from the list.
      m4return_t ShutDown ();

      //## Operation: ShutDownItem%878114669
      m4return_t ShutDownItem (M4Thread *ai_pItem);

      //## Operation: AddItem%880449708
      m4return_t AddItem (m4objid_t ai_lIdItem, M4Thread *ai_item);

      //## Operation: SetActiveList%880628516
      void SetActiveList (ClActiveList  *ai_poActiveList);

      //## Operation: StartUp%894962382
      m4return_t StartUp ();

    // Data Members for Associations

      //## Association: LocalMonitor::<unnamed>%34742C1F03B6
      //## Role: ClList::m_oIdGenerator%34742C2003A3
      //## begin ClList::m_oIdGenerator%34742C2003A3.role preserve=no  public: ClIdGenerator { -> VHAN}
      ClIdGenerator m_oIdGenerator;
      //## end ClList::m_oIdGenerator%34742C2003A3.role

      //## Association: LocalMonitor::<unnamed>%3507B96E01A8
      //## Role: ClList::m_oActiveMap%3507B976018C
      //## begin ClList::m_oActiveMap%3507B976018C.role preserve=no  public: ActiveMap { -> VHAN}
      ActiveMap m_oActiveMap;
      //## end ClList::m_oActiveMap%3507B976018C.role

      //## Association: LocalMonitor::<unnamed>%3507C41A034B
      //## Role: ClList::m_poList%3507C41B0361
      //## begin ClList::m_poList%3507C41B0361.role preserve=no  public: ClActiveList { -> RFHAN}
      ClActiveList *m_poList;
      //## end ClList::m_poList%3507C41B0361.role

    // Additional Public Declarations
      //## begin ClList%3430BFB102DB.public preserve=yes
      //## end ClList%3430BFB102DB.public

  protected:
    // Additional Protected Declarations
      //## begin ClList%3430BFB102DB.protected preserve=yes
      //## end ClList%3430BFB102DB.protected

  private:
    // Additional Private Declarations
      //## begin ClList%3430BFB102DB.private preserve=yes
      //## end ClList%3430BFB102DB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClList%3430BFB102DB.implementation preserve=yes
      //## end ClList%3430BFB102DB.implementation

};

//## begin ClList%3430BFB102DB.postscript preserve=yes
//## end ClList%3430BFB102DB.postscript

//## begin ClActiveList%346AFED603DE.preface preserve=yes
//## end ClActiveList%346AFED603DE.preface

//## Class: ClActiveList%346AFED603DE
//	A list for active objects that have been terminated but allready have not
//	exited.
//	This list will be visit by the supervisor to look for exited threads
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34D0661D0172;ClLocalMonitor { -> F}

class ClActiveList : public ClList  //## Inherits: <unnamed>%346AFF1601B9
{
  //## begin ClActiveList%346AFED603DE.initialDeclarations preserve=yes
  //## end ClActiveList%346AFED603DE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClActiveList%879781126
      ClActiveList (ClLocalMonitor *ai_poLocalMonitor);

      //## Operation: ClActiveList%880628524
      ClActiveList ();


    //## Other Operations (specified)
      //## Operation: ShutDown%879431912
      m4return_t ShutDown ();

      //## Operation: ShutDownItem%881748018
      m4return_t ShutDownItem (M4Thread *ai_pItem);

      //## Operation: Synchronize%887880514
      void Synchronize ();

    // Additional Public Declarations
      //## begin ClActiveList%346AFED603DE.public preserve=yes
      //## end ClActiveList%346AFED603DE.public

  protected:
    // Additional Protected Declarations
      //## begin ClActiveList%346AFED603DE.protected preserve=yes
      //## end ClActiveList%346AFED603DE.protected

  private:
    // Additional Private Declarations
      //## begin ClActiveList%346AFED603DE.private preserve=yes
      //## end ClActiveList%346AFED603DE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClActiveList%346AFED603DE.implementation preserve=yes
      //## end ClActiveList%346AFED603DE.implementation

};

//## begin ClActiveList%346AFED603DE.postscript preserve=yes
//## end ClActiveList%346AFED603DE.postscript

// Class ClList 

// Class ClActiveList 

//## begin module%3427D3D701E1.epilog preserve=yes
//## end module%3427D3D701E1.epilog


#endif
