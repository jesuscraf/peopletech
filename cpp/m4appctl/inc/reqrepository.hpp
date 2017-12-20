//## begin module%3A815FA601D6.cm preserve=no
//## end module%3A815FA601D6.cm

//## begin module%3A815FA601D6.cp preserve=no
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
//## end module%3A815FA601D6.cp

//## Module: reqrepository%3A815FA601D6; Package specification
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Source file: F:\integration\M4Appctl\inc\reqrepository.hpp

#ifndef reqrepository_h
#define reqrepository_h 1

//## begin module%3A815FA601D6.additionalIncludes preserve=no
//## end module%3A815FA601D6.additionalIncludes

//## begin module%3A815FA601D6.includes preserve=yes
//## end module%3A815FA601D6.includes

#include "m4stl.hpp"
// CriticalSection
#include <criticalsection.hpp>
// appctlreq
#include <appctlreq.hpp>
//## begin module%3A815FA601D6.declarations preserve=no
//## end module%3A815FA601D6.declarations

//## begin module%3A815FA601D6.additionalDeclarations preserve=yes
//## end module%3A815FA601D6.additionalDeclarations


//## begin ClReqMap%3A8160430164.preface preserve=yes
//## end ClReqMap%3A8160430164.preface

//## Class: ClReqMap%3A8160430164
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A81608B0244;map { -> }

typedef map <m4uint32_t , ClAppCtlRequest * , less <m4uint32_t> > ClReqMap;

//## begin ClReqMap%3A8160430164.postscript preserve=yes
//## end ClReqMap%3A8160430164.postscript

//## begin ClReqRepository%3A815E55029A.preface preserve=yes
//## end ClReqRepository%3A815E55029A.preface

//## Class: ClReqRepository%3A815E55029A
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A816527024A;ClAppCtlRequest { -> }

class ClReqRepository 
{
  //## begin ClReqRepository%3A815E55029A.initialDeclarations preserve=yes
  //## end ClReqRepository%3A815E55029A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClReqRepository%981535910
      ClReqRepository ();


    //## Other Operations (specified)
      //## Operation: AddRequest%981535911
      m4return_t AddRequest (m4uint32_t ai_iRequestID, ClAppCtlRequest *ai_poRequest);

      //## Operation: GetRequest%981535912
      m4return_t GetRequest (m4uint32_t ai_iRequestID, ClAppCtlRequest *&ao_poRequest);

      //## Operation: RemoveRequest%981535913
      m4return_t RemoveRequest (m4uint32_t ai_iRequestID, ClAppCtlRequest *&ao_poRequest);

    // Additional Public Declarations
      //## begin ClReqRepository%3A815E55029A.public preserve=yes
      //## end ClReqRepository%3A815E55029A.public

  protected:
    // Additional Protected Declarations
      //## begin ClReqRepository%3A815E55029A.protected preserve=yes
      //## end ClReqRepository%3A815E55029A.protected

  private:
    // Data Members for Associations

      //## Association: M4Appctl::<unnamed>%3A815F41013B
      //## Role: ClReqRepository::m_oReqMap%3A815F4103A9
      //## begin ClReqRepository::m_oReqMap%3A815F4103A9.role preserve=no  private: ClReqMap { -> VHAN}
      ClReqMap m_oReqMap;
      //## end ClReqRepository::m_oReqMap%3A815F4103A9.role

      //## Association: M4Appctl::<unnamed>%3A815F72002D
      //## Role: ClReqRepository::m_oExclusive%3A815F72022D
      //## begin ClReqRepository::m_oExclusive%3A815F72022D.role preserve=no  private: ClCriticalSection { -> VHAN}
      ClCriticalSection m_oExclusive;
      //## end ClReqRepository::m_oExclusive%3A815F72022D.role

    // Additional Private Declarations
      //## begin ClReqRepository%3A815E55029A.private preserve=yes
      //## end ClReqRepository%3A815E55029A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClReqRepository%3A815E55029A.implementation preserve=yes
      //## end ClReqRepository%3A815E55029A.implementation

};

//## begin ClReqRepository%3A815E55029A.postscript preserve=yes
//## end ClReqRepository%3A815E55029A.postscript

// Class ClReqRepository 

//## begin module%3A815FA601D6.epilog preserve=yes
//## end module%3A815FA601D6.epilog


#endif
