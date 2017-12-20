//## begin module%395090CF0015.cm preserve=no
//## end module%395090CF0015.cm

//## begin module%395090CF0015.cp preserve=no
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
//## end module%395090CF0015.cp

//## Module: RecycleableCollector%395090CF0015; Package specification
//## Subsystem: RecycleableCollector::inc%395090AB0258
//## Source file: F:\integration\m4transport\inc\recycleablecollector.hpp

#ifndef recycleablecollector_h
#define recycleablecollector_h 1

//## begin module%395090CF0015.additionalIncludes preserve=no
//## end module%395090CF0015.additionalIncludes

//## begin module%395090CF0015.includes preserve=yes
#include "m4transport_dll.hpp"
#include "m4stl.hpp"
//## end module%395090CF0015.includes

// garbagecollectorinterface
#include <garbagecollectorinterface.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>
//## begin module%395090CF0015.declarations preserve=no
//## end module%395090CF0015.declarations

//## begin module%395090CF0015.additionalDeclarations preserve=yes
//## end module%395090CF0015.additionalDeclarations


//## begin ClMultimapGarbageable%39508DE90393.preface preserve=yes
//## end ClMultimapGarbageable%39508DE90393.preface

//## Class: ClMultimapGarbageable%39508DE90393; Instantiated Class
//## Category: ClRecycleableCollector%39508D970241
//## Subsystem: RecycleableCollector::inc%395090AB0258
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3951F25C0219;IGarbageable { -> }

typedef multimap< int,IGarbageable *,less < int > > ClMultimapGarbageable;

//## begin ClMultimapGarbageable%39508DE90393.postscript preserve=yes
//## end ClMultimapGarbageable%39508DE90393.postscript

//## begin ClRecycleableCollector%39508FB302C9.preface preserve=yes
//## end ClRecycleableCollector%39508FB302C9.preface

//## Class: ClRecycleableCollector%39508FB302C9
//## Category: ClRecycleableCollector%39508D970241
//## Subsystem: RecycleableCollector::inc%395090AB0258
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4TRANSPORT ClRecycleableCollector : public IGarbageCollector  //## Inherits: <unnamed>%3951E0B1023D
{
  //## begin ClRecycleableCollector%39508FB302C9.initialDeclarations preserve=yes
  //## end ClRecycleableCollector%39508FB302C9.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClRecycleableCollector%962001309
      ~ClRecycleableCollector ();


    //## Other Operations (specified)
      //## Operation: Recover%961582754
      IGarbageable * Recover (int ai_iType);

      //## Operation: DisposeOf%961667108; C++
      m4return_t DisposeOf (IGarbageable &ai_oGarbage);

      //## Operation: Purge%961667109; C++
      m4uint32_t Purge ();

      //## Operation: GetElement%968774985
      m4return_t GetElement (IRecycleable *&ao_poElement);

      //## Operation: SetElement%968774986
      m4return_t SetElement (IRecycleable &ai_oElement);

    // Additional Public Declarations
      //## begin ClRecycleableCollector%39508FB302C9.public preserve=yes
      //## end ClRecycleableCollector%39508FB302C9.public

  protected:
    // Data Members for Associations

      //## Association: ClRecycleableCollector::<unnamed>%39508FD3029D
      //## Role: ClRecycleableCollector::m_MultimapGarbageable%39508FD400D3
      //## begin ClRecycleableCollector::m_MultimapGarbageable%39508FD400D3.role preserve=no  protected: ClMultimapGarbageable { -> VHAN}
      ClMultimapGarbageable m_MultimapGarbageable;
      //## end ClRecycleableCollector::m_MultimapGarbageable%39508FD400D3.role

    // Additional Protected Declarations
      //## begin ClRecycleableCollector%39508FB302C9.protected preserve=yes
      //## end ClRecycleableCollector%39508FB302C9.protected

  private:
    // Additional Private Declarations
      //## begin ClRecycleableCollector%39508FB302C9.private preserve=yes
      //## end ClRecycleableCollector%39508FB302C9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRecycleableCollector%39508FB302C9.implementation preserve=yes
      //## end ClRecycleableCollector%39508FB302C9.implementation

};

//## begin ClRecycleableCollector%39508FB302C9.postscript preserve=yes
//## end ClRecycleableCollector%39508FB302C9.postscript

// Class ClRecycleableCollector 

//## begin module%395090CF0015.epilog preserve=yes
//## end module%395090CF0015.epilog


#endif
