//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                m4sqltypes.h
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    UNIX
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module... define Microsft Sql types for Unix
//
//
//==============================================================================

#ifndef  __M4SQLTYPES__H__
#define  __M4SQLTYPES__H__


#ifdef _UNIX


#ifdef M4_ODBC_INTERSOLV

	#if (M4_ODBC_INTERSOLV == 0x0200)
	// Tipos SQL de Microsoft redefinidos para UNIX, solo para ODBC de InterSolv 2.0
	// pero no para la version 3.0 de InterSolv

	/* API declaration data types */
	typedef unsigned char   SQLCHAR;

	#if (ODBCVER >= 0x0300)
		typedef signed char     SQLSCHAR;
		typedef unsigned char   SQLDATE;
		typedef unsigned char   SQLDECIMAL;
		typedef double          SQLDOUBLE;
		typedef double          SQLFLOAT;
	#endif

	typedef long            SQLINTEGER;
	typedef unsigned long   SQLUINTEGER;

	#if (ODBCVER >= 0x0300)
		typedef unsigned char   SQLNUMERIC;
	#endif

	typedef void *          SQLPOINTER;

	#if (ODBCVER >= 0x0300)
		typedef float           SQLREAL;
	#endif

	typedef short           SQLSMALLINT;
	typedef unsigned short  SQLUSMALLINT;

	#if (ODBCVER >= 0x0300)
		typedef unsigned char   SQLTIME;
		typedef unsigned char   SQLTIMESTAMP;
		typedef unsigned char   SQLVARCHAR;
	#endif

	/* function return type */
	typedef SQLSMALLINT     SQLRETURN;

	/* generic data structures */
	#if (ODBCVER >= 0x0300)
		#if defined(WINDOWS)
			typedef void*					SQLHANDLE;
		#else
			typedef SQLINTEGER				SQLHANDLE;
		#endif	/* defined(WINDOWS) */
		typedef SQLHANDLE               SQLHENV;
		typedef SQLHANDLE               SQLHDBC;
		typedef SQLHANDLE               SQLHSTMT;
		typedef SQLHANDLE               SQLHDESC;
	#else
		#if defined(WINDOWS)
		typedef void*					SQLHENV;
		typedef void*					SQLHDBC;
		typedef void*					SQLHSTMT;
		#else
		typedef SQLINTEGER              SQLHENV;
		typedef SQLINTEGER              SQLHDBC;
		typedef SQLINTEGER              SQLHSTMT;
		#endif  /* defined(WINDOWS) */
	#endif /* ODBCVER >= 0x0300 */

	#endif // M4_ODBC_INTERSOLV_2_0


	// ODBC InterSolv 3.0
	#if (M4_ODBC_INTERSOLV == 0x0300)
	#include "m4types.hpp"
	#include "sqlunx.h"
	#endif	// M4_ODBC_INTERSOLV_3_0

#endif	// M4_ODBC_INTERSOLV


#else	// Windows


	#ifdef WINDOWS
	#include "sqltypes.h"
	#endif



#endif // _UNIX




#endif // __M4SQLTYPES__H__
