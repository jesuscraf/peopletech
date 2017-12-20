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

#ifndef exportingcfuntions_h
#define exportingcfuntions_h 1


#include <windows.h>
#include "m4xmladminclient_dll.hpp"

//-----------------------------------------------------------------------
// API for Server Monitor (implemented in Visual Basic)
// (1Functions exported by M4XMLAdminClient.dll)
//-----------------------------------------------------------------------

extern "C" M4_DECL_M4XMLADMINCLIENT long WINAPI M4InitConnect(LPSTR ai_szHost, int ai_iPort, LPSTR ai_szUser, LPSTR ai_szPassword);
extern "C" M4_DECL_M4XMLADMINCLIENT long WINAPI M4StopCommunication();
extern "C" M4_DECL_M4XMLADMINCLIENT long WINAPI M4ExecuteRequest(LPSTR ai_szCommand, LPSTR ai_szParameters, LPSTR ao_szAnswer, long FAR* ao_lAnswerSize);


#endif