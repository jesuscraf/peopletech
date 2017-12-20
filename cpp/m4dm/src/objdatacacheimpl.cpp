//## begin module%41E3BB3E005F.cm preserve=no
//## end module%41E3BB3E005F.cm

//## begin module%41E3BB3E005F.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
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
//## end module%41E3BB3E005F.cp

//## Module: objdatacacheimpl%41E3BB3E005F; Package body
//## Subsystem: m4dm::cache-auto::src%41E2B42502DC
//## Source file: Z:\m4dm\src\objdatacacheimpl.cpp

//## begin module%41E3BB3E005F.additionalIncludes preserve=no
//## end module%41E3BB3E005F.additionalIncludes

//## begin module%41E3BB3E005F.includes preserve=yes
//## end module%41E3BB3E005F.includes

// objdatacacheimpl
#include <objdatacacheimpl.hpp>
// objdatacacheinterface
#include <objdatacacheinterface.hpp>
// objdatacachemanager
#include <objdatacachemanager.hpp>
//## begin module%41E3BB3E005F.declarations preserve=no
//## end module%41E3BB3E005F.declarations

//## begin module%41E3BB3E005F.additionalDeclarations preserve=yes
//## end module%41E3BB3E005F.additionalDeclarations


// Class ClObjDataCacheBySessionId 



ClObjDataCacheBySessionId::ClObjDataCacheBySessionId (const m4string_t& ai_sSessionId, ClObjDataCache* ai_poObjDataCache)
  //## begin ClObjDataCacheBySessionId::ClObjDataCacheBySessionId%1105443274.hasinit preserve=no
  //## end ClObjDataCacheBySessionId::ClObjDataCacheBySessionId%1105443274.hasinit
  //## begin ClObjDataCacheBySessionId::ClObjDataCacheBySessionId%1105443274.initialization preserve=yes
  //## end ClObjDataCacheBySessionId::ClObjDataCacheBySessionId%1105443274.initialization
{
  //## begin ClObjDataCacheBySessionId::ClObjDataCacheBySessionId%1105443274.body preserve=yes
    m_poObjDataCache = ai_poObjDataCache;
    SetSessionId(ai_sSessionId);
  //## end ClObjDataCacheBySessionId::ClObjDataCacheBySessionId%1105443274.body
}


ClObjDataCacheBySessionId::~ClObjDataCacheBySessionId()
{
  //## begin ClObjDataCacheBySessionId::~ClObjDataCacheBySessionId%.body preserve=yes
  //## end ClObjDataCacheBySessionId::~ClObjDataCacheBySessionId%.body
}



//## Other Operations (implementation)
m4return_t ClObjDataCacheBySessionId::GetObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName, ClCMCRFactory* ai_poFactory, ClCompiledMCR* ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data*& ao_rpoChannelData)
{
  //## begin ClObjDataCacheBySessionId::GetObject%1105443272.body preserve=yes
    m4return_t retCode = M4_ERROR;
    if (NULL == m_poObjDataCache)
    {
        ao_rpoChannelData = NULL;
        return M4_ERROR;
    }

    // build the object id.
    m4string_t oid = BuildObjectId(ai_uiHandle, ai_pccName);

    // get this object from the cache.
    retCode = m_poObjDataCache->GetObject((m4pchar_t)oid.c_str(), ai_poFactory, ai_poCMCR, ai_poLaneArgument, ao_rpoChannelData);

    return retCode;
  //## end ClObjDataCacheBySessionId::GetObject%1105443272.body
}

m4return_t ClObjDataCacheBySessionId::PutObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName, ClChannel_Data* ai_poChannelData)
{
  //## begin ClObjDataCacheBySessionId::PutObject%1105443273.body preserve=yes
    m4return_t retCode = M4_ERROR;
    if (NULL == m_poObjDataCache)
    {
        return M4_ERROR;
    }

    // build the object id.
    m4string_t oid = BuildObjectId(ai_uiHandle, ai_pccName);

    // put this object in the cache.
    retCode = m_poObjDataCache->PutObject((m4pchar_t)oid.c_str(), ai_poChannelData);

    return retCode;
  //## end ClObjDataCacheBySessionId::PutObject%1105443273.body
}

m4string_t ClObjDataCacheBySessionId::BuildObjectId (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName)
{
  //## begin ClObjDataCacheBySessionId::BuildObjectId%1105443275.body preserve=yes
    char szBuffer[64];
    sprintf(szBuffer, "%u", ai_uiHandle);

    if (NULL == ai_pccName)
    {
        ai_pccName = "-";
    }

	// El handle debe ir antes del id para borrar por handle
    m4string_t oid = m_sSessionId + "@" + szBuffer + "@" + ai_pccName;

    return oid;
  //## end ClObjDataCacheBySessionId::BuildObjectId%1105443275.body
}

m4return_t ClObjDataCacheBySessionId::SetSessionId (const m4string_t& ai_sSessionId)
{
  //## begin ClObjDataCacheBySessionId::SetSessionId%1106737891.body preserve=yes
    m_sSessionId = ai_sSessionId;

    return M4_SUCCESS;
  //## end ClObjDataCacheBySessionId::SetSessionId%1106737891.body
}

m4return_t ClObjDataCacheBySessionId::DeleteObject (m4uint32_t ai_uiHandle, m4pcchar_t ai_pccName)
{
  //## begin ClObjDataCacheBySessionId::DeleteObject%1106737894.body preserve=yes
    m4return_t retCode = M4_ERROR;
    if (NULL == m_poObjDataCache)
    {
        return M4_ERROR;
    }

    // build the object id.
    m4string_t oid = BuildObjectId(ai_uiHandle, ai_pccName);

    // delete this object in the cache.
    retCode = m_poObjDataCache->RemoveObjectById((m4pchar_t)oid.c_str());

    return retCode;
  //## end ClObjDataCacheBySessionId::DeleteObject%1106737894.body
}

m4return_t ClObjDataCacheBySessionId::DeleteAll ()
{
  //## begin ClObjDataCacheBySessionId::DeleteAll%1106737895.body preserve=yes
    m4return_t retCode = M4_ERROR;
    if (NULL == m_poObjDataCache)
    {
        return M4_ERROR;
    }

    // delete all objects for this session id.
    retCode = m_poObjDataCache->RemoveObjectById((m4pchar_t)m_sSessionId.c_str());

    return retCode;
  //## end ClObjDataCacheBySessionId::DeleteAll%1106737895.body
}

// Additional Declarations
  //## begin ClObjDataCacheBySessionId%41E3BB140253.declarations preserve=yes
  //## end ClObjDataCacheBySessionId%41E3BB140253.declarations

//## begin module%41E3BB3E005F.epilog preserve=yes
//## end module%41E3BB3E005F.epilog
