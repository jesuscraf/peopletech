//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
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
//	=========================================================
//	=====================
//## end module.cp

//## Module: requestdic; Package specification
//## Subsystem: CPM4ComSrv::version
//## Source file: D:\eduardoma\M4ComSrv\version\inc\requestdic.hpp

#ifndef requestdic_h
#define requestdic_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// clbasedictionary
#include <clbasedictionary.hpp>

class ClRequest;

//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


//## Class: ClRequestDic
//	ClRequestDic is a class used for contain a list of
//	request. It's father is the ClBaseDictionary class witch
//	has implemented the nedeed methods to add, remove and so
//	on, items.
//	It will be used both in the Local Monitor for take a
//	reference to all the current request in the hole server
//	and in each session to have a list of the request
//	sending through each session.
//## Category: M4ComSrv::Dictionary
//## Subsystem: CPM4ComSrv::version
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>; ClRequest { -> F}

class ClRequestDic : public ClBaseDictionary  //## Inherits: <unnamed>
{
  //## begin ClRequestDic.initialDeclarations preserve=yes
  //## end ClRequestDic.initialDeclarations

  public:
    // Data Members for Associations

      //## Association: M4ComSrv::Subsystem::UsersSubsystem::<unnamed>%353F0CA402D8
      //## Role: ClRequestDic::<m_pClUSSessionInterface>
      //## begin ClRequestDic::<m_pClUSSessionInterface>.role preserve=no  public: ClUSSessionInterface {1 -> 1RHGA}
      //## end ClRequestDic::<m_pClUSSessionInterface>.role

    // Additional Public Declarations
      //## begin ClRequestDic.public preserve=yes
      //## end ClRequestDic.public

  protected:
    // Additional Protected Declarations
      //## begin ClRequestDic.protected preserve=yes
      //## end ClRequestDic.protected

  private:
    // Additional Private Declarations
      //## begin ClRequestDic.private preserve=yes
      //## end ClRequestDic.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClRequestDic.implementation preserve=yes
      //## end ClRequestDic.implementation

};

//## begin ClRequestDic.postscript preserve=yes
//## end ClRequestDic.postscript



// Class ClRequestDic 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif
