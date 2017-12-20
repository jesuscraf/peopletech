//-- {CycleCode: 83} file [0..1524]
//-- {AddDecl: 84} module.includes preserve=yes [0..681]
//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         m4sysalloc
// File:           m4sysalloc.hpp
// Project:
// Author:         Meta Software M.S. , S.A
// Date:             14 sept 99
// Language:        C++
// Operating System: all
// Design Document:
//
//
// Definition:
//
//    Module to allocate memory (header)
//    It also declares functions to treat locales
////
//==============================================================================

#ifndef _M4SYSALLOC_HPP_
#define _M4SYSALLOC_HPP_

#include "m4sysall_dll.hpp"

#include "m4types.hpp"
//## end module.includes preserve=yes
//-- {InsertRegion: 85} module.vulnerableDeclarations [682..1524]
//## begin module.epilog preserve=yes

M4_DECL_M4SYSALL void* M4SysAllocMem(m4uint32_t ai_iSize);
M4_DECL_M4SYSALL void M4SysFreeMem(void* ai_pvBuffer);

M4_DECL_M4SYSALL m4char_t* M4SysAllocString(m4uint32_t ai_iSize);
M4_DECL_M4SYSALL void M4SysFreeString(char* ai_pvBuffer);



#endif //_M4SYSALLOC_HPP_
 
//## end module.epilog
