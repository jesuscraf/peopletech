//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clptload.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================

#ifndef __CLPTLOAD_HPP__
#define __CLPTLOAD_HPP__

#include "defmain.hpp"
#include "channel.hpp"
#include "cursor.hpp"
#include "m4notifier.hpp"

class ILdbContext;

struct StPartialLoad
{
	ClCursor *m_pCursor;
	ClChannel *m_pChannel;
	void *m_pBlock;
	m4int32_t m_idLConn;
	m4int32_t m_iReadRegisters;
	m4bool_t m_bOpenCursor;

	StPartialLoad (ClCursor *ai_pCursor, m4int32_t ai_idLConn)
	{
		m_pCursor = ai_pCursor;
		m_pChannel = NULL;
		m_pBlock = NULL;
		m_idLConn = ai_idLConn;
		m_iReadRegisters = 0;
		m_bOpenCursor = M4_FALSE;
	}

	~StPartialLoad (void)
	{
		Close ();
	}

	void Close ();

	void SetOpenCursor (m4bool_t ai_bOpenCursor)
	{
		m_bOpenCursor = ai_bOpenCursor;
	}

	m4bool_t GetOpenCursor (void)
	{
		return (m_bOpenCursor);
	}

	void SetReadRegister (m4int32_t ai_iReadRegisters)
	{
		m_iReadRegisters = ai_iReadRegisters;
	}

	m4int32_t GetReadRegister (void)
	{
		return (m_iReadRegisters);
	}

	void SetBlock (void *ai_pBlock)
	{
		m_pBlock = ai_pBlock;
	}

	void *GetBlock (void)
	{
		return (m_pBlock);
	}

	void SetChannel (ClChannel *ai_pChannel)
	{
		m_pChannel = ai_pChannel;
	}

	ClChannel *GetChannel (void)
	{
		return (m_pChannel);
	}
};


typedef StPartialLoad*		PStPartialLoad;


class VcPartialLoad : public vector<PStPartialLoad>, public ClNotifiable
{
public:
	VcPartialLoad (void)
	{
		m_pContext = NULL;
	}

	m4return_t Init (ILdbContext *ai_pContext)
	{
		m_pContext = ai_pContext;
		return M4_SUCCESS;
	}

	PStPartialLoad FindPartialLoad (ClCursor *ai_pCursor, m4int32_t ai_idLConn);
	m4return_t ClosePartialLoad (ClCursor *ai_pCursor);
	m4return_t ErasePartialLoad (ClCursor *ai_pCursor);
	m4return_t DiscardResults (m4int32_t ai_idLConn, m4bool_t ai_bIgnoreChannel);
	virtual m4bool_t Notify (ClEvent &);	// Notificador.

protected:

	m4return_t	_ClosePartialLoad (ClEvent &ai_Event);
	m4bool_t	_DettachIfPossible(ClChannel *ai_pChannel);

	typedef VcPartialLoad::iterator		itVcPartialLoad;
	ILdbContext	*m_pContext;
};
		
	

#endif





