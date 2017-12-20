//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              All
// File:                m4sqlunx.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-03-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Mappings of Windows-style declarations and typedefs for unix.
//    Similar to the sqlunx.h from InterSolv 3.0 ODBC Drivers
//
//==============================================================================

#ifndef __M4SQLUNX_H__
#define __M4SQLUNX_H__

#ifdef _UNIX

// Tipos de Windows definidos por Meta4
typedef void			VOID;

typedef m4bool_t		BOOL;
typedef BOOL                 *PBOOL;
typedef BOOL                 *LPBOOL;

typedef long			LONG;
typedef unsigned long	ULONG;
typedef ULONG			*PULONG;
typedef unsigned short  USHORT;
typedef USHORT			*PUSHORT;
typedef unsigned char	UCHAR;
typedef UCHAR			*PUCHAR;
typedef char			*PSZ;

typedef unsigned long        DWORD;

typedef unsigned char        BYTE;
typedef unsigned short       WORD;
typedef float                FLOAT;
typedef FLOAT                *PFLOAT;
typedef BYTE                 *PBYTE;
typedef BYTE                 *LPBYTE;
typedef int                  *PINT;
typedef int                  *LPINT;
typedef WORD                 *PWORD;
typedef WORD                 *LPWORD;
typedef long                 *LPLONG;
typedef DWORD		         *PDWORD;

typedef DWORD                *LPDWORD;
typedef void 		         *LPVOID;
typedef const  void		     *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef char *          LPSTR;
typedef char *          LPTSTR;
typedef const char *    LPCSTR;
typedef const char *	LPCTSTR;
typedef char *			LPWSTR;

typedef UINT		WPARAM;
typedef LONG		LPARAM;
typedef LONG		LRESULT;

typedef void *		HANDLE;
typedef int			HWND;
typedef HANDLE		HGLOBAL;

typedef  long   HINSTANCE;
typedef  long   HCURSOR;



// Tipos de Windows definidos por InterSolv ODBC 3.0 - sqlunx.h
typedef char WCHAR;

/*
// InterSolv ODBC 3.0 - sqlunx.h

// Unix versions of Wintel declaration modifiers

#define NEAR
#define FAR
#define EXPORT
#define PASCAL
#define VOID void
#define CALLBACK
#define _cdecl

// Windows-style typedefs

typedef VOID * HANDLE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef long LONG;
typedef int BOOL;
typedef VOID * LPVOID;
typedef VOID * PVOID;
typedef VOID * HMODULE;
typedef int GLOBALHANDLE;
typedef int (*FARPROC)();
typedef char *LPSTR;
typedef const char * LPCSTR;
typedef VOID * HINSTANCE;
typedef VOID * HWND;
typedef unsigned int WPARAM;
typedef unsigned long LPARAM;
typedef VOID * HKEY; 
typedef VOID * PHKEY;
typedef BYTE * LPBYTE;
typedef char CHAR;
typedef BOOL * LPBOOL;
typedef DWORD * LPDWORD;
typedef char * LPWSTR;
typedef const char * LPCWSTR;
typedef char TCHAR;
//typedef char WCHAR;
typedef char VCHAR;
typedef TCHAR * LPTSTR;
typedef const TCHAR* LPCTSTR;
*/

#endif	// _UNIX


#endif	// __M4SQLUNX_H__
