//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             cvdate.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:
//
// Definition:
//    Header for date conversion routines
//
//==============================================================================

#ifndef __CVDATE__H__
#define __CVDATE__H__

#include "m4date_dll.hpp"
#include <wtypes.h>
#include "m4types.hpp"


#ifdef _WINDOWS
DATE		M4_DECL_M4DATE M4dateToDate( m4date_t ai_dDate );
m4date_t	M4_DECL_M4DATE DateToM4date( DATE ai_dDate );
#endif

#endif	// __CVDATE__H__ 

