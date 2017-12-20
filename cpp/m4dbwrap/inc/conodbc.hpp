//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                conodbc.hpp   
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

#ifndef  _CONEXODBC_HPP_
#define  _CONEXODBC_HPP_


#include "conexba.hpp"


#ifdef _UNIX	// UNIX

#else			// WINDOWS
	#ifndef  _WINDOWS
	#define _WINDOWS
	#endif
#endif

#define ODBCVER 0x0351

#include "sqlext.h"
#include "sqlucode.h"

#include "syncro.hpp"


#ifdef DBWRAP_SERIALIZE
	#ifndef DRIVER_OCI
		#pragma message ("DBWrap with lock")
	#endif
	extern ClMutex g_oODBCAccess;
	#define M4DBW_ODBC_SAFE(X)		{g_oODBCAccess.Lock(); X; g_oODBCAccess.Unlock();}
	#define M4DBW_ODBC_CALL(X)		{g_oODBCAccess.Lock(); while( ( X ) == SQL_STILL_EXECUTING ) ; g_oODBCAccess.Unlock();}
#else
	#define M4DBW_ODBC_SAFE(X)		X
	#define M4DBW_ODBC_CALL(X)		while( ( X ) == SQL_STILL_EXECUTING )
#endif

#define M4DBW_ODBC_ERROR(X)		(m4return_t) ((X == SQL_SUCCESS) ? M4_SUCCESS : ((X == SQL_SUCCESS_WITH_INFO) ? M4_WARNING : ((X == SQL_NO_DATA) ? M4LDB_SQL_NO_DATA_FOUND : M4_ERROR)))



class ClDBConnection_odbc: public ClDBConnection_Base
{
public:
	ClDBConnection_odbc (eDDBBType_t ai_eDDBB);

	~ClDBConnection_odbc (void) {};

	m4return_t Init (void);

	m4return_t End (void);

	m4return_t Connect (m4pcchar_t ai_pConnStr);

	m4return_t Disconnect (void);

	m4return_t Transact (eTransType_t ai_eTransType);

	m4return_t GetErrorString (m4return_t ai_retcode, m4int32_t &ao_iErrorCode, m4pchar_t *ao_ppcErrorString);

	m4return_t SetIsolationLevel (eIsolationLevel_t ai_eILevel);

	m4return_t ExecuteSQL (m4pcchar_t ai_pSQL);

protected:
	m4return_t _UseBrandDriver (HDBC ai_hDbc);

private:

	HENV m_hEnv;
	HDBC m_hDbc;

	eDDBBType_t m_eDDBB;

	SQLSMALLINT m_indError;

	friend class ClCursor_odbc;
}; 

#endif

