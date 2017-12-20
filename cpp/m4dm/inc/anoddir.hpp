//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             anoddir.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             22/09/97
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

#ifndef __ANODDIR_HPP__
#define __ANODDIR_HPP__


#include "index.hpp"
#include "handles.hpp"

class ClChannelDef;

template <class T>
class ClNodeDirectory
{
public:
    T &operator[] (ClNodeIndex  ai_nodeindex);
    T &operator[] (ClHandle ai_nodehandle);
    T &operator[] (m4pcchar_t ai_pcNodeId);

    T *Get (ClNodeIndex  ai_nodeindex);
    T *Get (ClHandle ai_nodehandle);
    T *Get (m4pcchar_t ai_pcNodeId);

    m4uint16_t Count (void) const;

    ClNodeIndex Index;

    T &operator  *() const;
    T *operator ->() const;

    m4return_t Attach (ClChannelDef *ai_pchdef, T *ai_pbuffer)
    {
        Attach (ai_pchdef);
        Attach (ai_pbuffer);
        return M4_SUCCESS;
    }

    void Attach (T *ai_pbuffer)
    {
        m_aClNode = ai_pbuffer;
    }

    void Attach (ClChannelDef *ai_pchdef)
    {
        M4_ASSERT (ai_pchdef);
        m_pChannelDef = ai_pchdef;
    }

    m4return_t Detach (void);
	m4return_t GetSize (ClSize &ao_size);


protected:
    ClChannelDef *m_pChannelDef;
    T *m_pCurrentNode;

    ClNodeDirectory ();
    virtual ~ClNodeDirectory ();

    //void _Destroy (void);

private:
    T *m_aClNode;   // el array
};


#include "anoddir.inl"


#endif // __ANODDIR_HPP__ 
