//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             notifier.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             10/02/98
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

#ifndef __NOTIFIER_HPP__
#define __NOTIFIER_HPP__

#include "m4types.hpp"

class _AClClient_Notifier;
class ClSize;

class _AClNotifier
{
public:
    _AClNotifier ();
    virtual ~_AClNotifier ();

    void Attach (_AClClient_Notifier *ai_pclient);
    void Detach (_AClClient_Notifier *ai_pclient);

	virtual m4return_t GetSize(ClSize &ao_size);

    _AClClient_Notifier ** m_pClientVector;
    m4uint32_t m_iCount, m_iLast;

private:

    void GrowVector (void);
};

#endif //__NOTIFIER_HPP__
