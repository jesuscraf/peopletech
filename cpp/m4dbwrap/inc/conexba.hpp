//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                conex.hpp   
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

#ifndef  _CONEXBA_HPP_
#define  _CONEXBA_HPP_


#include "m4dbwrapdef.hpp"


class ClDBConnection_Base
{
public:

	virtual m4return_t Init (void) = 0;

	virtual m4return_t End (void) = 0;

	virtual m4return_t Connect (m4pcchar_t ai_pConnStr) = 0;

	// desconecta de la base de datos
	virtual m4return_t Disconnect (void) = 0;

	virtual m4return_t Transact (eTransType_t ai_eTransType) = 0;

	virtual m4return_t GetErrorString (m4return_t ai_retcode, m4int32_t &ao_iErrorCode, m4pchar_t *ao_ppcErrorString) = 0;

	virtual m4return_t SetIsolationLevel (eIsolationLevel_t ai_eILevel) = 0;

	virtual m4return_t ExecuteSQL (m4pcchar_t ai_pSQL) = 0;

	friend class ClCursor_Base;
}; 


#endif
