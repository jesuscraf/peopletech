//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_nod_s.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             24/05/98
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

#ifndef __CH_NOD_S_HPP__
#define __CH_NOD_S_HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"

class ClNodeDef;
class ClCompiledMCR;
class ClBSearchIndex;

class ClSize;

///////////////////////////////////////////////////////////////////////////////
//
// ClSearchIndex_Current
//

class M4_DECL_M4DM ClSearchIndex_Current
{
public:
    ClSearchIndex_Current () :
      m_pCurrentSearchIndex (0)
    {
    }

    virtual ~ClSearchIndex_Current ()
    {
    }

    ClBSearchIndex *GetpSearchIndex (void)
    { 
        return m_pCurrentSearchIndex; 
    }

    m4return_t SetpSearchIndex (ClBSearchIndex *ai_psi);

	m4return_t GetSize(ClSize &ao_size);

private:
    //indice activo en el nodo:
    ClBSearchIndex *m_pCurrentSearchIndex;
};


///////////////////////////////////////////////////////////////////////////////
//
// ClSearchIndex_List
//

class M4_DECL_M4DM ClSearchIndex_List
{
public:
    ClSearchIndex_List () :
        m_pSearchIndexes (0)
    {
    }

    virtual ~ClSearchIndex_List ();

    m4return_t Init (ClNodeDef *ai_pnodedef, ClCompiledMCR *ai_pcmcr);

    m4return_t Update (ClCompiledMCR *poCMCR);

    ClBSearchIndex *GetSearchIndexByPos (m4uint16_t ai_ipos);
    ClBSearchIndex *GetSearchIndexById (m4uint32_t ai_id);

	m4return_t GetSize(ClSize &ao_size);

private:
    ClBSearchIndex *m_pSearchIndexes;

    m4uint16_t m_iCount;
};

#endif // __CH_NOD_S_HPP__
