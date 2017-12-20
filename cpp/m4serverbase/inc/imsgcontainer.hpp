//## begin module%3AD31D49022F.cm preserve=no
//## end module%3AD31D49022F.cm

//## begin module%3AD31D49022F.cp preserve=no
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
//## end module%3AD31D49022F.cp

//## Module: ImsgContainer%3AD31D49022F; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\imsgcontainer.hpp

#ifndef imsgcontainer_h
#define imsgcontainer_h 1

//## begin module%3AD31D49022F.additionalIncludes preserve=no
//## end module%3AD31D49022F.additionalIncludes

//## begin module%3AD31D49022F.includes preserve=yes
#include <m4types.hpp>
//## end module%3AD31D49022F.includes

// m4string
#include <m4string.hpp>
//## begin module%3AD31D49022F.declarations preserve=no
//## end module%3AD31D49022F.declarations

//## begin module%3AD31D49022F.additionalDeclarations preserve=yes
//## end module%3AD31D49022F.additionalDeclarations


//## begin ClMessageContainer%3AD3177D01EF.preface preserve=yes
//## end ClMessageContainer%3AD3177D01EF.preface

//## Class: ClMessageContainer%3AD3177D01EF; Abstract
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3AD32B770062;m4string_t { -> }

class ClMessageContainer 
{
  //## begin ClMessageContainer%3AD3177D01EF.initialDeclarations preserve=yes
  //## end ClMessageContainer%3AD3177D01EF.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClMessageContainer%988274771
      virtual ~ClMessageContainer ();


    //## Other Operations (specified)
      //## Operation: StoreTraceMessage%986914177
      virtual void StoreTraceMessage (m4string_t ai_strMessage) = 0;

      //## Operation: GetTraceMessage%986975095
      virtual m4return_t GetTraceMessage (m4string_t &ao_strMessage) = 0;

    // Additional Public Declarations
      //## begin ClMessageContainer%3AD3177D01EF.public preserve=yes
      //## end ClMessageContainer%3AD3177D01EF.public

  protected:
    // Additional Protected Declarations
      //## begin ClMessageContainer%3AD3177D01EF.protected preserve=yes
      //## end ClMessageContainer%3AD3177D01EF.protected

  private:
    // Additional Private Declarations
      //## begin ClMessageContainer%3AD3177D01EF.private preserve=yes
      //## end ClMessageContainer%3AD3177D01EF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMessageContainer%3AD3177D01EF.implementation preserve=yes
      //## end ClMessageContainer%3AD3177D01EF.implementation

};

//## begin ClMessageContainer%3AD3177D01EF.postscript preserve=yes
//## end ClMessageContainer%3AD3177D01EF.postscript

// Class ClMessageContainer 

inline ClMessageContainer::~ClMessageContainer ()
{
  //## begin ClMessageContainer::~ClMessageContainer%988274771.body preserve=yes
  //## end ClMessageContainer::~ClMessageContainer%988274771.body
}


//## begin module%3AD31D49022F.epilog preserve=yes
//## end module%3AD31D49022F.epilog


#endif
