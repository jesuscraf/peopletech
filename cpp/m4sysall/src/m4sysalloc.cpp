//-- {CycleCode: 161} file [0..936]
//-- {AddDecl: 162} module.includes preserve=yes [0..610]
//## begin module.includes preserve=yes
//m4sysalloc.cpp
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         m4sysalloc
// File:                m4sysalloc.cpp
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
//    Module to allocate memory
////
//==============================================================================
#include "m4sysalloc.hpp"
//## end module.includes preserve=yes
//-- {InsertRegion: 163} module.vulnerableDeclarations [611..936]
//## begin module.epilog preserve=yes

void* M4SysAllocMem(m4uint32_t ai_iSize){
	return (void*)new m4char_t [ai_iSize];
}

void M4SysFreeMem(void* ai_pvBuffer){
	delete [] (char*)ai_pvBuffer;
}

m4char_t* M4SysAllocString(m4uint32_t ai_iSize){
	return new m4char_t [ai_iSize];
}

void M4SysFreeString(char* ai_pvBuffer){
	delete [] ai_pvBuffer;
}
//## end module.epilog
