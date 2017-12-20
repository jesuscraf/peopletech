//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ini 
// File:                regedit.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================

#ifndef __REGEDIT_HPP__
#define __REGEDIT_HPP__

#include "m4ini_dll.hpp"
#include "m4types.hpp"


M4_DECL_M4INI char GetRegKeyData (char *szRegKey, char *pszRegValue, char *pszData, unsigned long* pLen = NULL);

#endif


