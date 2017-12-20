#ifndef __INDEX__HPP__
#define __INDEX__HPP__

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             index.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             05/07/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef _M4CHCOMMON_HPP_
#include "m4types.hpp"
#endif


// Parece un descuido, porque luego el index es: 
//		m4uint32_t m_iIndex;
// #define M4DM_EOF_INDEX    -1
// Por lo tanto deberia ser: MAX_INT de m4uint32_t
#define M4DM_EOF_INDEX    0xffffffff


typedef m4uint32_t ClAccessIndex;

typedef m4uint32_t ClRegisterIndex;

typedef m4uint32_t ClItemIndex;

typedef m4uint32_t ClSliceIndex;

typedef m4uint32_t ClChildIndex;

typedef m4uint32_t ClNodeIndex;


#endif //__INDEX__HPP__
