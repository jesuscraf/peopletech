//## begin module%377C88510390.cm preserve=no
//## end module%377C88510390.cm

//## begin module%377C88510390.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%377C88510390.cp

//## Module: m4tratini%377C88510390; Package specification
//## Subsystem: M4Log::inc%377C883D0066
//	d:\compon\m4log\version\inc
//## Source file: D:\ricardo\m4log\version\inc\m4tratini.hpp

#ifndef m4tratini_h
#define m4tratini_h 1

//## begin module%377C88510390.additionalIncludes preserve=no
//## end module%377C88510390.additionalIncludes

//## begin module%377C88510390.includes preserve=yes


#include "m4win.hpp"
#include "m4string.hpp"
#include "m4types.hpp"
//## end module%377C88510390.includes

//## begin module%377C88510390.declarations preserve=no
//## end module%377C88510390.declarations

//## begin module%377C88510390.additionalDeclarations preserve=yes
//## end module%377C88510390.additionalDeclarations


//## begin module%377C88510390.epilog preserve=yes




#ifdef _UNIX
  #include "m4ini.hpp"
#endif

m4int32_t	M4GetPrivateProfileString(

						M4ClString lpAppName, // points to section name
						M4ClString lpKeyName, // points to key name
						M4ClString lpDefault, // points to default string
						M4ClString &lpReturnedString, // points to destination buffer
//						m4int32_t nSize, // size of destination buffer
						M4ClString lpFileName // points to initialization filename
					);


m4int32_t	M4GetPrivateProfileInt( M4ClString lpAppName,
					// address of section name
							M4ClString lpKeyName,
					// address of key name
							m4int32_t nDefault,
					// return value if key name is not found
							
							M4ClString lpFileName
					// address of initialization filename
						);

m4int32_t  M4GetPrivateProfileSection(m4pchar_t  lpAppName,
									  m4pchar_t &lpReturnedString,
									  m4int32_t  nSize,
									  m4pchar_t lpFileName);


 
//## end module%377C88510390.epilog


#endif
