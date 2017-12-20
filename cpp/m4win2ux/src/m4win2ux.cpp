//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4win2ux
// File:                m4win2ux.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                04-11-1998
// Language:            C++
// Operating System:    WINDOWS, UNIX (Solaris, HP-UX)
// Design Document:     XXX.DOC
//
//
// Definition:
//     Implementation of Windows functions in Unix
//
//==============================================================================

#include "m4win2ux.hpp"

//#ifndef _M4_USING_INLINE
//	#include "m4win2ux.inl"
//#endif

//M4_DECL_M4WIN2UX
ClMutex g_oM4ecvtMutex(M4_TRUE);	// Mutex para region critica:	M4_ecvt
ClMutex g_oM4fcvtMutex(M4_TRUE);	// Mutex para region critica:	M4_fcvt

