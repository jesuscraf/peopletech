//## begin module%3950C43A01A4.cm preserve=no
//## end module%3950C43A01A4.cm

//## begin module%3950C43A01A4.cp preserve=no
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
//## end module%3950C43A01A4.cp

//## Module: RequestCollection%3950C43A01A4; Subprogram specification
//## Subsystem: M4Request::inc%379439BF01B6
//## Source file: F:\integration\m4request\inc\requestcollection.hpp

#ifndef requestcollection_h
#define requestcollection_h 1

//## begin module%3950C43A01A4.additionalIncludes preserve=no
//## end module%3950C43A01A4.additionalIncludes

//## begin module%3950C43A01A4.includes preserve=yes
#include <m4request_dll.hpp>
#include <m4types.hpp>
#include <m4string.hpp>
#include <garbagecontainer.hpp>

class ClRequest;
class IGarbageable;

//## begin module%3950C43A01A4.declarations preserve=no
//## end module%3950C43A01A4.declarations

//## begin module%3950C43A01A4.additionalDeclarations preserve=yes
//## end module%3950C43A01A4.additionalDeclarations


//## begin ClRequestCollector%3950C36E03A0.preface preserve=yes
//## end ClRequestCollector%3950C36E03A0.preface

//## Class: ClRequestCollector%3950C36E03A0
//	Especialización de la clase de Garbage Collector para uso en el servidor.
//	En principio será una única clase puesto que también pretende ser una única
//	instancia de Garbage Collector.
//## Category: M4Request%35EFAF8303E2
//## Subsystem: M4Request::inc%379439BF01B6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3950C36E03A2;ClBlockSync { -> F}
//## Uses: <unnamed>%3950C36E03A3;IGarbageable { -> F}
//## Uses: <unnamed>%3960B8BF0398;ofstream { -> F}
//## Uses: <unnamed>%3960C02F035A;m4pchar_t { -> }
//## Uses: <unnamed>%3960C0CB021E;friend ClRequest { -> F}

class M4_DECL_M4REQUEST ClRequestCollector : public ClGarbageContainer  //## Inherits: <unnamed>%39B8E19D020F
{
  //## begin ClRequestCollector%3950C36E03A0.initialDeclarations preserve=yes
  //## end ClRequestCollector%3950C36E03A0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRequestCollector%961594594
      ClRequestCollector (const m4string_t &ai_strFileName = "", m4int_t ai_iCollectorSize = FastDefUpperBound);

    //## Destructor (specified)
      //## Operation: ~ClRequestCollector%961594595
      ~ClRequestCollector ();


    //## Other Operations (specified)
      //## Operation: CreateRequest%962640090
      ClRequest * CreateRequest (m4uint32_t ai_uiReqType, m4pchar_t ai_szServiceId, m4uint64_t ai_uiStartRecordTime = 0, m4uint32_t ai_uiReqId = 0, const m4string_t &ai_strDumpFile = "") const;

    // Additional Public Declarations
      //## begin ClRequestCollector%3950C36E03A0.public preserve=yes
        m4uint32_t Purge();
      //## end ClRequestCollector%3950C36E03A0.public

  protected:
    // Data Members for Associations

      //## Association: M4Request::<unnamed>%3960B88E0301
      //## Role: ClRequestCollector::m_strDumpFile%3960B88F0082
      //## begin ClRequestCollector::m_strDumpFile%3960B88F0082.role preserve=no  protected: m4string_t { -> 1VHAN}
      m4string_t m_strDumpFile;
      //## end ClRequestCollector::m_strDumpFile%3960B88F0082.role

    // Additional Protected Declarations
      //## begin ClRequestCollector%3950C36E03A0.protected preserve=yes
      //## end ClRequestCollector%3950C36E03A0.protected

  private:
    // Additional Private Declarations
      //## begin ClRequestCollector%3950C36E03A0.private preserve=yes
      //## end ClRequestCollector%3950C36E03A0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRequestCollector%3950C36E03A0.implementation preserve=yes
      //## end ClRequestCollector%3950C36E03A0.implementation

};

//## begin ClRequestCollector%3950C36E03A0.postscript preserve=yes
//## end ClRequestCollector%3950C36E03A0.postscript

// Class ClRequestCollector 

//## begin module%3950C43A01A4.epilog preserve=yes
//## end module%3950C43A01A4.epilog


#endif
