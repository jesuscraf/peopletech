//==============================================================================
//
// (c) Copyright  1991-2017 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             rowsquota.cpp
// Project:          CVM - DM
// Author:           Meta Software M.S. , S.A
// Date:             
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//==============================================================================

#include "rowsquota.hpp"
#include "blocksynchronization.hpp"

// --------------------------------------------
// -- Class ClRowsQuotaController
// --------------------------------------------

ClRowsQuotaController::ClRowsQuotaController(m4bool_t ai_bEnabled, m4uint32_t ai_iRowsLimit, m4bool_t ai_bOLTPMode)
{
	m_bEnabled = ai_bEnabled;
	m_iLimit = ai_iRowsLimit;
	m_bOLTPMode = ai_bOLTPMode;

	m_iOLTPCount = 0;	
	m_iRestCount = 0;
	m_iPeak = 0;

	m_oMutex.Init();
}

ClRowsQuotaController::~ClRowsQuotaController() {}

m4bool_t ClRowsQuotaController::TryAlloc(m4uint32_t ai_iNumRows)
{
	ClMutBlock oBlock(m_oMutex);

	if (m_bEnabled == M4_FALSE)
	{
		return M4_TRUE;
	}

	m4uint32_t iTotal = m_iRestCount + m_iOLTPCount;

	if ((iTotal + ai_iNumRows) <= m_iLimit)
	{
		m4uint32_t* pCounter = (m_bOLTPMode == M4_TRUE) ? &m_iOLTPCount : &m_iRestCount;

		*pCounter += ai_iNumRows;

		iTotal += ai_iNumRows;

		if (m_iPeak < iTotal)
		{
			m_iPeak = iTotal;
		}

		return M4_TRUE;
	}
	else
	{
		return M4_FALSE;
	}
}

m4bool_t ClRowsQuotaController::Free(m4uint32_t ai_iNumRows)
{
	ClMutBlock oBlock(m_oMutex);

	if (m_bEnabled == M4_FALSE)
	{
		return M4_TRUE;
	}

	m4uint32_t* pCounter = (m_bOLTPMode == M4_TRUE) ? &m_iOLTPCount : &m_iRestCount;

	m4bool_t bRet = M4_TRUE;
	
	if (*pCounter >= ai_iNumRows)
	{
		*pCounter -= ai_iNumRows;
	}
	else
	{
		*pCounter = 0;
		bRet = M4_FALSE;
	}

	return bRet;
}

void ClRowsQuotaController::SwitchOn()
{
	ClMutBlock oBlock(m_oMutex);

	m_bEnabled = M4_TRUE;
}

m4bool_t ClRowsQuotaController::SwitchOff()
{
	ClMutBlock oBlock(m_oMutex);

	m4bool_t bEnabled = m_bEnabled;

	m_bEnabled = M4_FALSE;

	return bEnabled;
}

void ClRowsQuotaController::Reset(m4bool_t ai_bOLTPMode)
{
	ClMutBlock oBlock(m_oMutex);

	reset(ai_bOLTPMode);
}

void ClRowsQuotaController::Reset(m4bool_t ai_bOLTPMode, m4uint32_t ai_iRowsLimit)
{
	ClMutBlock oBlock(m_oMutex);

	m_iLimit = ai_iRowsLimit;

	reset(ai_bOLTPMode);
}

m4uint32_t ClRowsQuotaController::GetLimit()
{
	return m_iLimit;
}

m4uint32_t ClRowsQuotaController::GetCount()
{
	ClMutBlock oBlock(m_oMutex);

	return m_iRestCount + m_iOLTPCount;
}

m4uint32_t ClRowsQuotaController::GetPeak()
{
	return m_iPeak;
}

void ClRowsQuotaController::reset(m4bool_t ai_bOLTPMode)
{
	m_bOLTPMode = ai_bOLTPMode;

	m_iOLTPCount = 0;
	m_iPeak = m_iRestCount;

	m_bEnabled = M4_TRUE;
}

