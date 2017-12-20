//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             execntxt.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "execntxt.hpp"

#include "executor.hpp"
#include "dm.hpp"


m4uint16_t ClOLTPExecutionContext::GetSerializationVersion(void)
{
   return 14;   // 2016-10-26: Cambian las estadísticas
}

ClOLTPExecutionContext::ClOLTPExecutionContext(ClChannelManager& ai_channelManager, ClAccess* const ai_pAccess)
	: m_poChannelManager(&ai_channelManager)
	, m_poAccess(ai_pAccess)
	, m_poExecutor(ai_channelManager.GetpExecutor())
	, m_bSendAccessPointer(M4_TRUE)
{
}

IExecutor* const ClOLTPExecutionContext::GetExecutor(void) const
{
   return m_poExecutor;
}

ClAccess* const ClOLTPExecutionContext::GetAccess(void) const
{
    return m_poAccess;
}

m4return_t ClOLTPExecutionContext::Serialize(ClGenericIODriver& IOD)
{

   // Serializo el ejecutor
   if (m_poExecutor->Serialize(IOD) != M4_SUCCESS)
      return M4_ERROR;

   // Serializo el puntero al access que se va a ejecutar
   // Este access debe haberse serializado antes
   m4bool_t existPreviously;

   if(GetbSendAccessPointer() == M4_TRUE) {
	   if (IOD.Write((void*)m_poAccess, &existPreviously) != M4_SUCCESS)
		  return M4_ERROR;
	   M4_ASSERT(existPreviously);
   }
   else {
	   m_poAccess = 0;
	   if (IOD.Write((void*)m_poAccess) != M4_SUCCESS)
		  return M4_ERROR;
   }

   return M4_SUCCESS;
}

m4return_t ClOLTPExecutionContext::DeSerialize(ClGenericIODriver& IOD)
{
   M4_ASSERT(m_poExecutor);
   
//   m_poExecutor->Reset();  //reentry

   // DeSerializo el ejecutor
   if (m_poExecutor->DeSerialize(IOD, m_poChannelManager) != M4_SUCCESS)
      return M4_ERROR;

   // DeSerializo el puntero al access que se va a ejecutar
   // Este access debe haberse DeSerializado antes
   m4bool_t existPreviously;

   if (IOD.Read((void**)&m_poAccess, &existPreviously) != M4_SUCCESS)
      return M4_ERROR;

   if(m_poAccess) {
	M4_ASSERT(existPreviously);
   }

   return M4_SUCCESS;
}
