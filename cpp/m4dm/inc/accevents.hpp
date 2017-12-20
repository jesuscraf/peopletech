//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             accevents.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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

#ifndef __ACCEVENTS__HPP__
#define __ACCEVENTS__HPP__


class ClChannel;

class ClAccessEventSink
{
public:
	virtual void Invalidate() = 0;
	virtual void Refresh(ClChannel* poChannel) = 0;
};

#endif  //__ACCEVENTS__HPP__ 
