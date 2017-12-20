//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             rerror.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             10/11/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the raise error functions
//
//==============================================================================

//
// Include files
//
#include "gstring.h"

#ifndef _RERROR_H
#define _RERROR_H

HRESULT RaiseError(gstring ai_st);
HRESULT FormatError(m4int32_t ai_error, m4pcchar_t ai_sz, ...);

#endif
