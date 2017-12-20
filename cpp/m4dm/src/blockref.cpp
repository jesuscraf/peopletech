//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             blockref.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             10/09/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    ClBlockReference
//
//
//==============================================================================

#include "blockref.hpp"

#include "px_block.hpp"

void ClBlockReference::DecRef (void)
{
}

void ClBlockReference::IncRef (void)
{
}

m4return_t ClBlockReference::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClBlockReference);
	return M4_SUCCESS;
}
