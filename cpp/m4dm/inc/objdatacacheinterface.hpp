//## begin module%41E3BA50005C.cm preserve=no
//## end module%41E3BA50005C.cm

//## begin module%41E3BA50005C.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.hpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%41E3BA50005C.cp

//## Module: objdatacacheinterface%41E3BA50005C; Package specification
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Source file: Z:\m4dm\inc\objdatacacheinterface.hpp

#ifndef objdatacacheinterface_h
#define objdatacacheinterface_h 1

//## begin module%41E3BA50005C.additionalIncludes preserve=no
//## end module%41E3BA50005C.additionalIncludes

//## begin module%41E3BA50005C.includes preserve=yes
#include "m4dm_dll.hpp"
#include "chan_knl.hpp"         // ClChannel_Data
//## end module%41E3BA50005C.includes

// m4types
#include <m4types.hpp>
//## begin module%41E3BA50005C.declarations preserve=no
//## end module%41E3BA50005C.declarations

//## begin module%41E3BA50005C.additionalDeclarations preserve=yes
//## end module%41E3BA50005C.additionalDeclarations


//## begin IObjDataCache%41E3B9E2003B.preface preserve=yes
//## end IObjDataCache%41E3B9E2003B.preface

//## Class: IObjDataCache%41E3B9E2003B
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%41E3BE5A021F;m4return_t { -> }

class M4_DECL_M4DM IObjDataCache 
{
  //## begin IObjDataCache%41E3B9E2003B.initialDeclarations preserve=yes
  //## end IObjDataCache%41E3B9E2003B.initialDeclarations

  public:
    //## Destructor (generated)
      virtual ~IObjDataCache();


    //## Other Operations (specified)
      //## Operation: GetObject%1105443270
      virtual m4return_t GetObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName, ClCMCRFactory* ai_poFactory, ClCompiledMCR* ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data*& ao_rpoChannelData) = 0;

      //## Operation: PutObject%1105443271
      virtual m4return_t PutObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName, ClChannel_Data* ai_poChannelData) = 0;

      //## Operation: DeleteObject%1106737892
      virtual m4return_t DeleteObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName) = 0;

      //## Operation: DeleteAll%1106737893
      virtual m4return_t DeleteAll () = 0;

    // Additional Public Declarations
      //## begin IObjDataCache%41E3B9E2003B.public preserve=yes
      //## end IObjDataCache%41E3B9E2003B.public

  protected:
    // Additional Protected Declarations
      //## begin IObjDataCache%41E3B9E2003B.protected preserve=yes
      //## end IObjDataCache%41E3B9E2003B.protected

  private:
    // Additional Private Declarations
      //## begin IObjDataCache%41E3B9E2003B.private preserve=yes
      //## end IObjDataCache%41E3B9E2003B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IObjDataCache%41E3B9E2003B.implementation preserve=yes
      //## end IObjDataCache%41E3B9E2003B.implementation

};

//## begin IObjDataCache%41E3B9E2003B.postscript preserve=yes
//## end IObjDataCache%41E3B9E2003B.postscript

// Class IObjDataCache 

//## begin module%41E3BA50005C.epilog preserve=yes
//## end module%41E3BA50005C.epilog


#endif
