//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             newinterface.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//   Declaration of the NewInterface function
//
//==============================================================================
#include <atlbase.h>
#include "m4types.hpp"

#ifndef _NEWINTERFACE_H
#define _NEWINTERFACE_H

#include "logcomobject.h"

m4return_t NewInterface(REFCLSID ai_ClassId, REFIID ai_InterfaceId, IUnknown** pNodCl);

#endif
