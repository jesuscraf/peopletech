//## begin module%392A9578026F.cm preserve=no
//## end module%392A9578026F.cm

//## begin module%392A9578026F.cp preserve=no
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
//## end module%392A9578026F.cp

//## Module: BlockSynchronization%392A9578026F; Subprogram specification
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//	d:\fuentesservidor\m4ipcs\version\inc
//## Source file: D:\eduardoma\m4ipcs\inc\blocksynchronization.hpp

#ifndef blocksynchronization_h
#define blocksynchronization_h 1

//## begin module%392A9578026F.additionalIncludes preserve=no
//## end module%392A9578026F.additionalIncludes

//## begin module%392A9578026F.includes preserve=yes
#include "m4ipcs_dll.hpp"
//## end module%392A9578026F.includes

// syncro
#include <syncro.hpp>
//## begin module%392A9578026F.declarations preserve=no
//## end module%392A9578026F.declarations

//## begin module%392A9578026F.additionalDeclarations preserve=yes
//## end module%392A9578026F.additionalDeclarations


//## Class: ClBlockSync%392A934A010F
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%392AA1750287;ClSynObject { -> }

class ClBlockSync 
{
  public:
    //## Constructors (specified)
      //## Operation: ClBlockSync%959072717
      ClBlockSync (ClSynObject &ai_oSyncObject)
        //## begin ClBlockSync::ClBlockSync%959072717.hasinit preserve=no
        //## end ClBlockSync::ClBlockSync%959072717.hasinit
        //## begin ClBlockSync::ClBlockSync%959072717.initialization preserve=yes
		: m_poSyncObject(&ai_oSyncObject)
        //## end ClBlockSync::ClBlockSync%959072717.initialization
      {
        //## begin ClBlockSync::ClBlockSync%959072717.body preserve=yes
		  m_poSyncObject->Lock();
        //## end ClBlockSync::ClBlockSync%959072717.body
      }

      //## Operation: ClBlockSync%959182045
      ClBlockSync (ClSynObject *ai_poSyncObject)
        //## begin ClBlockSync::ClBlockSync%959182045.hasinit preserve=no
        //## end ClBlockSync::ClBlockSync%959182045.hasinit
        //## begin ClBlockSync::ClBlockSync%959182045.initialization preserve=yes
		: m_poSyncObject(ai_poSyncObject)
        //## end ClBlockSync::ClBlockSync%959182045.initialization
      {
        //## begin ClBlockSync::ClBlockSync%959182045.body preserve=yes
		  if (NULL != m_poSyncObject)
			  m_poSyncObject->Lock();
        //## end ClBlockSync::ClBlockSync%959182045.body
      }

    //## Destructor (specified)
      //## Operation: ~ClBlockSync%959072718
      ~ClBlockSync ()
      {
        //## begin ClBlockSync::~ClBlockSync%959072718.body preserve=yes
		  if (NULL != m_poSyncObject)
			  m_poSyncObject->Unlock();
        //## end ClBlockSync::~ClBlockSync%959072718.body
      }

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_poSyncObject%392AA0B20073
      //## begin ClBlockSync::m_poSyncObject%392AA0B20073.attr preserve=no  protected: ClSynObject *const  {VA} 
      ClSynObject *const  m_poSyncObject;
      //## end ClBlockSync::m_poSyncObject%392AA0B20073.attr

  private:
    //## Constructors (specified)
      //## Operation: ClBlockSync%959072719
      ClBlockSync ()
        //## begin ClBlockSync::ClBlockSync%959072719.hasinit preserve=no
        //## end ClBlockSync::ClBlockSync%959072719.hasinit
        //## begin ClBlockSync::ClBlockSync%959072719.initialization preserve=yes
		: m_poSyncObject(NULL)
        //## end ClBlockSync::ClBlockSync%959072719.initialization
      {
        //## begin ClBlockSync::ClBlockSync%959072719.body preserve=yes
        //## end ClBlockSync::ClBlockSync%959072719.body
      }

  private: //## implementation
};

//## begin ClMutBlock%392AA30802D6.preface preserve=yes
//## end ClMutBlock%392AA30802D6.preface

//## Class: ClMutBlock%392AA30802D6; Instantiated Class
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%392AA321014B;ClBlockSync { -> }

typedef ClBlockSync ClMutBlock;

//## begin ClMutBlock%392AA30802D6.postscript preserve=yes
//## end ClMutBlock%392AA30802D6.postscript

// Class ClBlockSync 


//## begin module%392A9578026F.epilog preserve=yes
//## end module%392A9578026F.epilog


#endif
