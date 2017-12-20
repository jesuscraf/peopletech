//## begin module%38205C6F0174.cm preserve=no
//## end module%38205C6F0174.cm

//## begin module%38205C6F0174.cp preserve=no
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
//## end module%38205C6F0174.cp

//## Module: garbagecollectorinterface%38205C6F0174; Package specification
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Source file: F:\integration\m4serverbase\inc\garbagecollectorinterface.hpp

#ifndef garbagecollectorinterface_h
#define garbagecollectorinterface_h 1

//## begin module%38205C6F0174.additionalIncludes preserve=no
//## end module%38205C6F0174.additionalIncludes

//## begin module%38205C6F0174.includes preserve=yes
const int DefRecycleSize = 1024 ;
//## end module%38205C6F0174.includes

// garbageableinterface
#include <garbageableinterface.hpp>
//## begin module%38205C6F0174.declarations preserve=no
//## end module%38205C6F0174.declarations

//## begin module%38205C6F0174.additionalDeclarations preserve=yes
//## end module%38205C6F0174.additionalDeclarations


//## begin IRecycleCollector%39B899C5008C.preface preserve=yes
//## end IRecycleCollector%39B899C5008C.preface

//## Class: IRecycleCollector%39B899C5008C
//## Category: M4ServerBase::GarbageCollector%3820504A014C
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class IRecycleCollector 
{
  //## begin IRecycleCollector%39B899C5008C.initialDeclarations preserve=yes
  //## end IRecycleCollector%39B899C5008C.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetElement%968401145
      virtual m4return_t GetElement (IRecycleable *&ao_poElement) = 0;

      //## Operation: SetElement%968401146
      virtual m4return_t SetElement (IRecycleable &ai_oElement) = 0;

    // Additional Public Declarations
      //## begin IRecycleCollector%39B899C5008C.public preserve=yes
      //## end IRecycleCollector%39B899C5008C.public

  protected:
    // Additional Protected Declarations
      //## begin IRecycleCollector%39B899C5008C.protected preserve=yes
      //## end IRecycleCollector%39B899C5008C.protected

  private:
    // Additional Private Declarations
      //## begin IRecycleCollector%39B899C5008C.private preserve=yes
      //## end IRecycleCollector%39B899C5008C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IRecycleCollector%39B899C5008C.implementation preserve=yes
      //## end IRecycleCollector%39B899C5008C.implementation

};

//## begin IRecycleCollector%39B899C5008C.postscript preserve=yes
//## end IRecycleCollector%39B899C5008C.postscript

//## Class: IGarbageCollector%382050CF02B5
//	This interface provides a basic API for recollection garbageable objects. No
//	data-storage or recycling policy is enforced and the public interface
//	supports "lazy processing" through the Purge method.
//## Category: M4ServerBase::GarbageCollector%3820504A014C
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%382050CF02B6;IGarbageable { -> }

class IGarbageCollector : public IRecycleCollector  //## Inherits: <unnamed>%39B8A0D10194
{
  public:

    //## Other Operations (specified)
      //## Operation: DisposeOf%941644999; C++
      virtual m4return_t DisposeOf (IGarbageable &ai_oGarbage) = 0;

      //## Operation: Purge%941645000; C++
      virtual m4uint32_t Purge () = 0;

  protected:
  private:
  private: //## implementation
};

// Class IRecycleCollector 

// Class IGarbageCollector 

// Class IGarbageCollector 

//## begin IGarbageCollector::<m_pClAutoGarbageable>%3821AC760060.role preserve=no  private: static ClAutoGarbageable { -> RHGA}
//## end IGarbageCollector::<m_pClAutoGarbageable>%3821AC760060.role

//## begin module%38205C6F0174.epilog preserve=yes
//## end module%38205C6F0174.epilog


#endif


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin IRecycleCollector::GetElement%968401145.body preserve=no
	return M4_ERROR ;
//## end IRecycleCollector::GetElement%968401145.body

//## begin IRecycleCollector::SetElement%968401146.body preserve=no
	return M4_ERROR ;
//## end IRecycleCollector::SetElement%968401146.body

#endif
