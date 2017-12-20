//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             dm_types.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             02/07/97
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

#ifndef __DM_TYPES__HPP__
#define __DM_TYPES__HPP__

//#include "m4types.hpp"
// Si ya se ha definido _M4CHCOMMON_HPP_, ya está incluido m4types.hpp
#ifndef _M4CHCOMMON_HPP_
#include "m4types.hpp"
#endif

typedef void*      ClMemPtr;
typedef m4int32_t  regindex_t;
typedef m4uint32_t regid_t;

typedef struct
{
   m4date_t Date;
} VersionMark;

#endif // __DM_TYPES__HPP__ 
