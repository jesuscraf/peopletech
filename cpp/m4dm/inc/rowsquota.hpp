//==============================================================================
//
// (c) Copyright  1991-2017 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             rowsquota.hpp
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

#ifndef __ROWSQUOTA__HPP__
#define __ROWSQUOTA__HPP__

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include <m4thread.hpp>

// --------------------------------------------
// -- Class ClRowsQuotaController
// --------------------------------------------

class M4_DECL_M4DM ClRowsQuotaController
{
public:

	ClRowsQuotaController(m4bool_t ai_bEnabled, m4uint32_t ai_iRowsLimit, m4bool_t ai_bOLTPMode);

	~ClRowsQuotaController();

	// Checks if an amount of registers could be added without exceeding the rows quota. If so it allocates them.
	m4bool_t TryAlloc(m4uint32_t ai_iNumRows);

	// Frees quota.
	m4bool_t Free(m4uint32_t ai_iNumRows);

	// Enables the quota controller.
	void SwitchOn();

	// Disables the quota controller.
	m4bool_t SwitchOff();

	// Prepares the controller for a new controlled transaction.
	void Reset(m4bool_t ai_bOLTPMode);

	// Prepares the controller too but setting the quota limit.
	void Reset(m4bool_t ai_bOLTPMode, m4uint32_t ai_iRowsLimit);

	// Gets the quota limit.
	m4uint32_t GetLimit();

	// Gets the quota used.
	m4uint32_t GetCount();

	// Gets the rows used peak value.
	m4uint32_t GetPeak();

private:

	// Controller active.
	m4bool_t	m_bEnabled;

	// Quota limit.
	m4uint32_t	m_iLimit;

	// OLTP behavior: Registers are destroyed after transactions.
	m4bool_t	m_bOLTPMode;

	// Consumed registers in OLTP or JS transactions.
	m4uint32_t	m_iOLTPCount;

	// Consumed registers in Proxy or XML transactions.
	m4uint32_t	m_iRestCount;

	// Peak value of consumed registers in a transaction.
	m4uint32_t	m_iPeak;

	// Avoids increasing/decreasing the counter concurrently.
	ClMutex		m_oMutex;

	// Internal reset.
	void reset(m4bool_t ai_bOLTPMode);
};

#endif  

