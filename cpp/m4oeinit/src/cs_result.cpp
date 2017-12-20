//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                CS_Result.cpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                19/06/98
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Implementation of the Object used to return a result from a C/S transaction
//
// 
//==============================================================================

#include "cs_result.hpp"

#include <string.h>
#include <stdlib.h>

#include "clgeniod.hpp"
#include "sttwrap.hpp"
#include "dm.hpp"
#include "iexecutor.hpp"


ClClientServerResult::ClClientServerResult(void)
	: m_iNumberOfMessages(0)
	, m_iResponse(M4_CSR_SUCCESS)
{
	for (m4uint16_t i = 0; i < CS_RESULT_MAX_MESSAGES; i++) {
		m_pszMessage[i] = 0;
	}
}

void ClClientServerResult::Reset(void)
{
	for (m4uint16_t i = 0; i < m_iNumberOfMessages; i++) {
		delete m_pszMessage[i];
		m_pszMessage[i] = 0;
	}
	m_iNumberOfMessages = 0;
}
     
ClClientServerResult::~ClClientServerResult(void)
{
   Reset();
}

void ClClientServerResult::SetResponseCode(const m4return_t ai_iResponse)
{
	if (m_iResponse == M4_CSR_SUCCESS && ai_iResponse != M4_CSR_SUCCESS) {
		m_iResponse = ai_iResponse;
	}
}

m4return_t ClClientServerResult::GetResponseCode(void) const
{
   return m_iResponse;
}

m4return_t ClClientServerResult::AddMess(const m4char_t* const ai_pszMessage)
{
	if (m_iNumberOfMessages < CS_RESULT_MAX_MESSAGES) {
		m4uint16_t iSize = strlen(ai_pszMessage);
		
		if (iSize <= CS_RESULT_MAX_MESSAGE_SIZE) {
			m_pszMessage[m_iNumberOfMessages] = new m4char_t[iSize + 1];
			if (m_pszMessage[m_iNumberOfMessages]) {
				strcpy(m_pszMessage[m_iNumberOfMessages++], ai_pszMessage);
				return M4_SUCCESS;
			}
		}
	}
	
	return M4_ERROR;
}

const m4char_t* const ClClientServerResult::GetMess(const m4uint16_t ai_iMessageIndex)
{
	if (ai_iMessageIndex < m_iNumberOfMessages) {
		return m_pszMessage[ai_iMessageIndex];
	}
	return 0;
}

m4uint16_t ClClientServerResult::NumberOfMessages(void) const
{
   return m_iNumberOfMessages;
}

m4return_t ClClientServerResult::Serialize(ClGenericIODriver& IOD, ClChannelManager *ai_poChannelManager, m4bool_t ai_bSendStatistics)
{
	m4return_t iRet = IOD.Write(m_iResponse);
	if (iRet != M4_SUCCESS) return iRet;
	
	iRet = IOD.Write(m_iNumberOfMessages);
	if (iRet != M4_SUCCESS) return iRet;
	
	for (m4uint16_t i = 0 ; i < m_iNumberOfMessages; i++) {
		iRet = IOD.Write(m_pszMessage[i]);
		if (iRet != M4_SUCCESS) return iRet;
	}

	/* Bug 0102945
	Se chequea que el ChannelManager no sea nulo
	*/
	if( ai_poChannelManager == NULL )
	{
		ai_bSendStatistics = M4_FALSE ;
	}

	iRet = IOD.Write( ai_bSendStatistics ) ;
	if (iRet != M4_SUCCESS) return iRet;

	if( ai_bSendStatistics == M4_TRUE )
	{
		iRet = ai_poChannelManager->GetpExecutor()->GetSttWrap()->Serialize( IOD ) ;
		if (iRet != M4_SUCCESS) return iRet;
	}
		
	return M4_SUCCESS;
}

m4return_t ClClientServerResult::DeSerialize(ClGenericIODriver& IOD, ClChannelManager *ai_poChannelManager)
{
	Reset();
	
	m4return_t iRet = IOD.Read(m_iResponse);
	if (iRet != M4_SUCCESS) return iRet;
	
	m4uint16_t iAuxNumberOfMessages = 0;
	iRet = IOD.Read(iAuxNumberOfMessages);
	if (iRet != M4_SUCCESS) return iRet;
	
	M4_ASSERT(iAuxNumberOfMessages <= CS_RESULT_MAX_MESSAGES);
	
	if (iAuxNumberOfMessages > CS_RESULT_MAX_MESSAGES) return M4_ERROR;
	
	m4char_t pszAuxCad[CS_RESULT_MAX_MESSAGE_SIZE + 1];
	
	for (m4uint16_t i = 0; i < iAuxNumberOfMessages; i++) {
		iRet = IOD.Read(pszAuxCad);
		if (iRet != M4_SUCCESS) return iRet;
		
		iRet = AddMess(pszAuxCad);
		if (iRet != M4_SUCCESS) return iRet;
	}
	
	m4bool_t bHasStatistics = M4_FALSE ;

	iRet = IOD.Read( bHasStatistics ) ;
	if (iRet != M4_SUCCESS) return iRet;

	if( bHasStatistics == M4_TRUE )
	{
		iRet = ai_poChannelManager->GetpExecutor()->GetSttWrap()->DeSerialize( IOD, ai_poChannelManager ) ;
		if (iRet != M4_SUCCESS) return iRet;
	}
		
	return M4_SUCCESS;
}

