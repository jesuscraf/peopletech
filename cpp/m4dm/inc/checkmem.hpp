//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             checkmem.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             02/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Checking & measure memory utilities 
//
//
//==============================================================================

#ifndef __CHECKMEM__HPP__
#define __CHECKMEM__HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"

#ifdef _WINDOWS
#include <crtdbg.h>
#endif

#ifdef _DEBUG
	#define  SET_CRT_DEBUG_FIELD(a)   _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
	#define  CLEAR_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
	#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
	#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

struct M4_DECL_M4DM StMemInfo
{
    StMemInfo ();

    m4uint32_t iAccess;
    m4uint32_t iChannel;
    m4uint32_t iNode_Channel;
    m4uint32_t iNode_Kernel;
    m4uint32_t iNode_Access;
    m4uint32_t iBlock_Channel;
    m4uint32_t iBlock_Proxy;
    m4uint32_t iRegister;
    m4uint32_t iItem;

    m4uint32_t iAccessSize;
    m4uint32_t iChannelSize;
    m4uint32_t iNode_ChannelSize;
    m4uint32_t iNode_AccessSize;
    m4uint32_t iBlock_ChannelSize;
    m4uint32_t iBlock_ProxySize;
    m4uint32_t iRegisterSize;
    m4uint32_t iItemSize;
};

M4_DECL_M4DM extern StMemInfo g_StMemInfo;

#endif //__CHECKMEM__HPP__
