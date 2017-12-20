//## begin module%39169C7902F4.cm preserve=no
//## end module%39169C7902F4.cm

//## begin module%39169C7902F4.cp preserve=no
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
//## end module%39169C7902F4.cp

//## Module: cacheitfz%39169C7902F4; Package specification
//## Subsystem: M4Cache::inc%3796F4510086
//## Source file: d:\castration\m4cache\inc\cacheitfz.hpp

#ifndef __CACHEITFZ_HPP__
#define __CACHEITFZ_HPP__ 1

//## begin module%39169C7902F4.additionalIncludes preserve=no
//## end module%39169C7902F4.additionalIncludes

//## begin module%39169C7902F4.includes preserve=yes
//## end module%39169C7902F4.includes

//## begin module%39169C7902F4.declarations preserve=no
//## end module%39169C7902F4.declarations

//## begin module%39169C7902F4.additionalDeclarations preserve=yes
#include "m4types.hpp"

class ClDumpCacheVisitor;
//## end module%39169C7902F4.additionalDeclarations


//## begin ClBaseCacheInterface%39169C7902F3.preface preserve=yes
//## end ClBaseCacheInterface%39169C7902F3.preface

//## Class: ClBaseCacheInterface%39169C7902F3; Abstract
//## Category: M4Cache%3496B4B10159
//## Subsystem: M4Cache::inc%3796F4510086
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClBaseCacheInterface 
{
  //## begin ClBaseCacheInterface%39169C7902F3.initialDeclarations preserve=yes
  //## end ClBaseCacheInterface%39169C7902F3.initialDeclarations

  public:

    typedef enum { MAIN = 0, PARENT, DEPENDENCY } eRemoveType_t ;

    //## Other Operations (specified)
      //## Operation: Init%1279752268; C++
      virtual void Init (void ) = 0;

      //## Operation: PersistCache%-931447424; C++
      virtual m4return_t PersistCache (void ) = 0;

      //## Operation: DePersistCache%1323802659; C++
      virtual m4return_t DePersistCache (void ) = 0;

      //## Operation: DeleteCache%916270520; C++
      virtual m4return_t DeleteCache (void ) = 0;

      //## Operation: RemoveObjectById%1044334270; C++
      virtual m4return_t RemoveObjectById (m4pchar_t ai_pcId, m4puint32_t ai_piRemovedCount = 0) = 0;

      //## Operation: RemoveAll%1273277923; C++
      virtual m4return_t RemoveAll (m4date_t ai_dDate = 0) = 0;

      //## Operation: RemoveObjectByIdAndVersion%1640217511; C++
      virtual m4return_t RemoveObjectByIdAndVersion (m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType) = 0;

      //## Operation: GetNumObjects%-2009236157; C++
      virtual m4return_t GetNumObjects (m4uint32_t& ao_iNumObjects) = 0;

      //## Operation: GetNextId%-1682966853; C++
      virtual m4return_t GetNextId (m4pchar_t& ao_pcId, void*& ai_poIterator) = 0;

      //## Operation: GetType%-874120614; C++
      virtual m4uint16_t GetType (void ) = 0;

      //## Operation: Dump%962926291; C++
      virtual m4return_t Dump (ClDumpCacheVisitor& ai_oVisitor) = 0;

      virtual void Reload (void ) = 0;

    // Additional Public Declarations
      //## begin ClBaseCacheInterface%39169C7902F3.public preserve=yes
      //## end ClBaseCacheInterface%39169C7902F3.public

  protected:
    // Additional Protected Declarations
      //## begin ClBaseCacheInterface%39169C7902F3.protected preserve=yes
      //## end ClBaseCacheInterface%39169C7902F3.protected

  private:
    // Additional Private Declarations
      //## begin ClBaseCacheInterface%39169C7902F3.private preserve=yes
      //## end ClBaseCacheInterface%39169C7902F3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBaseCacheInterface%39169C7902F3.implementation preserve=yes
      //## end ClBaseCacheInterface%39169C7902F3.implementation

};

//## begin ClBaseCacheInterface%39169C7902F3.postscript preserve=yes
//## end ClBaseCacheInterface%39169C7902F3.postscript

// Class ClBaseCacheInterface 

//## begin module%39169C7902F4.epilog preserve=yes
//## end module%39169C7902F4.epilog


#endif
