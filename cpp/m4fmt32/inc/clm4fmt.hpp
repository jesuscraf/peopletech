//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             CLM4FMT.HPP
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98 - 12/06/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines the member functions of the M4Fmt class, which are
//    called by the exported functions of the DLL
//
//==============================================================================

#ifndef _CLM4FMT_HPP_
#define _CLM4FMT_HPP_

#include "m4types.hpp"
#include "plugin.hpp"

/////////////////////////////////////////////////////////////////////////////////

struct StFMT_TaskEnv;
struct StMIT_Inspector;

class CLM4FMT
{

public:
	CLM4FMT();
	
	virtual ~CLM4FMT (void)
	{
		_M4FMTTerminate ();
	}
	
	void _M4FMTInitialice(void);
	void _M4FMTTerminate(void);
	m4int16_t _M4FMTCreateNewFormat (m4char_t* a_pcNombre, m4int16_t * a_piH);
	m4int16_t _M4FMTSetProperty (m4int16_t a_iHandle, m4char_t* a_pcNombre, m4char_t* a_pcValor);
	m4int16_t _M4FMTDestroyFormat (m4int16_t a_iHandle);
	m4int16_t _M4FMTCreateAddFormat (m4int16_t a_iHandleIn, m4char_t* a_pcNombre, m4int16_t * a_piHandleOut);
	m4int16_t _M4FMTConvertFmtToFmt (m4int16_t a_iHandleIn, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4int16_t a_iHandleOut, m4char_t* a_pcOut,  m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita);
	m4int16_t _M4FMTApplyFormat (m4int16_t a_iHandle, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita);
	m4int16_t _M4FMTConvertIsoToWin (m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita);
	m4int16_t _M4FMTConvertWinToIso (m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita);
	m4int16_t _M4FMTConvertFmtToIso (m4int16_t a_iHandle, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita);
	m4int16_t _M4FMTConvertUserToFmt (m4int16_t a_iHandleIn, m4char_t* a_pcIn, m4int16_t a_iTipo, m4int16_t a_iIsSQLM4, m4int16_t a_iHandleOut, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t * a_piLonEscrita);
	m4int16_t _M4FMTConvertSQLToM4 (m4int16_t a_iTipoSql, m4int16_t  *a_piTipoM4);
	m4int16_t _M4FMTGetFormatName (m4int16_t a_iHandle, m4char_t* a_pcOut, m4int16_t a_iLonOut, m4int16_t  *a_piLonEscrita);
	m4int16_t _M4FMTGetFormatHandle (m4int16_t  *a_piHandle, m4char_t* a_pcIn);
	m4int16_t _M4FMTGetErrorName (m4int16_t a_iError, m4char_t* a_pcErrorName, m4int16_t a_iLonErrorName, m4int16_t  *a_iErrorNamel);

private:

	StFMT_TaskEnv* m_poTaskEnv;
	StMIT_Inspector* m_poInsp;  

};


#endif

