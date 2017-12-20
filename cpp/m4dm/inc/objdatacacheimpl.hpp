//## begin module%41E3BB370011.cm preserve=no
//## end module%41E3BB370011.cm

//## begin module%41E3BB370011.cp preserve=no
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
//## end module%41E3BB370011.cp

//## Module: objdatacacheimpl%41E3BB370011; Package specification
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Source file: Z:\m4dm\inc\objdatacacheimpl.hpp

#ifndef objdatacacheimpl_h
#define objdatacacheimpl_h 1

//## begin module%41E3BB370011.additionalIncludes preserve=no
//## end module%41E3BB370011.additionalIncludes

//## begin module%41E3BB370011.includes preserve=yes
#include "m4dm_dll.hpp"
//## end module%41E3BB370011.includes

// objdatacacheinterface
#include <objdatacacheinterface.hpp>
// objdatacachemanager
#include <objdatacachemanager.hpp>
//## begin module%41E3BB370011.declarations preserve=no
//## end module%41E3BB370011.declarations

//## begin module%41E3BB370011.additionalDeclarations preserve=yes
//## end module%41E3BB370011.additionalDeclarations


//## begin ClObjDataCacheBySessionId%41E3BB140253.preface preserve=yes
//## end ClObjDataCacheBySessionId%41E3BB140253.preface

//## Class: ClObjDataCacheBySessionId%41E3BB140253
//## Category: m4dm::cache-auto%4236ED6C0371
//## Subsystem: m4dm::cache-auto::inc%41E2B41D02FB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4DM ClObjDataCacheBySessionId : public IObjDataCache  //## Inherits: <unnamed>%41E3BB6902B1
{
  //## begin ClObjDataCacheBySessionId%41E3BB140253.initialDeclarations preserve=yes
  //## end ClObjDataCacheBySessionId%41E3BB140253.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClObjDataCacheBySessionId%1105443274
      ClObjDataCacheBySessionId (const m4string_t& ai_sSessionId, ClObjDataCache* ai_poObjDataCache);

    //## Destructor (generated)
      virtual ~ClObjDataCacheBySessionId();


    //## Other Operations (specified)
      //## Operation: GetObject%1105443272
      virtual m4return_t GetObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName, ClCMCRFactory* ai_poFactory, ClCompiledMCR* ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data*& ao_rpoChannelData);

      //## Operation: PutObject%1105443273
      virtual m4return_t PutObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName, ClChannel_Data* ai_poChannelData);

      //## Operation: SetSessionId%1106737891
      m4return_t SetSessionId (const m4string_t& ai_sSessionId);

      //## Operation: DeleteObject%1106737894
      virtual m4return_t DeleteObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName);

      //## Operation: DeleteAll%1106737895
      virtual m4return_t DeleteAll ();

    // Additional Public Declarations
      //## begin ClObjDataCacheBySessionId%41E3BB140253.public preserve=yes
      //## end ClObjDataCacheBySessionId%41E3BB140253.public

  protected:

    //## Other Operations (specified)
      //## Operation: BuildObjectId%1105443275
      m4string_t BuildObjectId (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName);

    // Data Members for Class Attributes

      //## Attribute: m_sSessionId%41E3E66B034E
      //## begin ClObjDataCacheBySessionId::m_sSessionId%41E3E66B034E.attr preserve=no  protected: m4string_t {UA} 
      m4string_t m_sSessionId;
      //## end ClObjDataCacheBySessionId::m_sSessionId%41E3E66B034E.attr

    // Data Members for Associations

      //## Association: m4dm::<unnamed>%41E3E5AE035B
      //## Role: ClObjDataCacheBySessionId::m_poObjDataCache%41E3E5B4010B
      //## begin ClObjDataCacheBySessionId::m_poObjDataCache%41E3E5B4010B.role preserve=no  protected: ClObjDataCache { -> 1RHAN}
      ClObjDataCache *m_poObjDataCache;
      //## end ClObjDataCacheBySessionId::m_poObjDataCache%41E3E5B4010B.role

    // Additional Protected Declarations
      //## begin ClObjDataCacheBySessionId%41E3BB140253.protected preserve=yes
      //## end ClObjDataCacheBySessionId%41E3BB140253.protected

  private:
    // Additional Private Declarations
      //## begin ClObjDataCacheBySessionId%41E3BB140253.private preserve=yes
      //## end ClObjDataCacheBySessionId%41E3BB140253.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClObjDataCacheBySessionId%41E3BB140253.implementation preserve=yes
      //## end ClObjDataCacheBySessionId%41E3BB140253.implementation

};

//## begin ClObjDataCacheBySessionId%41E3BB140253.postscript preserve=yes
//## end ClObjDataCacheBySessionId%41E3BB140253.postscript

// Class ClObjDataCacheBySessionId 

//## begin module%41E3BB370011.epilog preserve=yes
//## end module%41E3BB370011.epilog


#endif
