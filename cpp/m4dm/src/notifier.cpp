//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             notifier.cpp
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


#include "notifier.hpp"
#include "sizer.hpp"

#include "not_cli.hpp"

#define M4_VECTOR_GROW_RATIO    10;


_AClNotifier::_AClNotifier ()
{
    m_pClientVector = 0;
    m_iCount = 0;
    m_iLast = 0;

// Creo espacio para 10 clientes;
    GrowVector();
}

_AClNotifier::~_AClNotifier ()
{
    if (m_pClientVector)
        delete m_pClientVector;
    m_iCount = 0;
    m_iLast = 0;
}

void _AClNotifier::Attach (_AClClient_Notifier *ai_pclient)
{
    m4uint32_t i;

    for (i = 0; i < m_iCount; i ++)
    {
        if (m_pClientVector[i] == 0)
        {
            m_pClientVector[i] = ai_pclient;
            if (i > m_iLast)
                m_iLast = i;
            return;
        }
    }

    // Si llego hasta aqui es porque no tengo sitio y tengo que crecer.
    // como i apunta al siguiente libre lo utilizo.

    GrowVector();
    m_pClientVector[i] = ai_pclient;
    m_iLast = i;
}

void _AClNotifier::Detach (_AClClient_Notifier *ai_pclient)
{
    m4uint32_t i;

    for (i = 0; i < m_iCount; i ++)
    {
        if (m_pClientVector[i] == ai_pclient)
        {
            m_pClientVector[i] = 0;
            if ((m_iLast) && (i == m_iLast))
                m_iLast --;
            return;
        }
    }
}

void _AClNotifier::GrowVector (void)
{
    m4uint32_t i, iNewCount = m_iCount + M4_VECTOR_GROW_RATIO;
    _AClClient_Notifier ** pAuxVector = new _AClClient_Notifier * [iNewCount];

    if (m_pClientVector)
    {
        memcpy(pAuxVector, m_pClientVector, sizeof(_AClClient_Notifier *) * m_iCount);
        delete m_pClientVector;
    }

    m_pClientVector = pAuxVector;

    for (i = m_iCount; i < iNewCount; i++)
        m_pClientVector[i] = 0;

    m_iCount = iNewCount;
}

m4return_t 
_AClNotifier::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_AClNotifier);
    ao_size+=sizeof(_AClClient_Notifier *) * m_iCount;

	return M4_SUCCESS;
}


