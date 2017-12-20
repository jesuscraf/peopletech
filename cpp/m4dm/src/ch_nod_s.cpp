//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_nod_s.cpp
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

#include "m4mdrt.hpp"
#include "clbsearch.hpp"
#include "nodedef.hpp"

#include "ch_nod_s.hpp"

///////////////////////////////////////////////////////////////////////////////
//
// ClSearchIndex_Current
//

/*
ClSearchIndex_Current::ClSearchIndex_Current ()
{
    m_pCurrentSearchIndex = 0;
}

ClSearchIndex_Current::~ClSearchIndex_Current ()
{
}

//indices:
ClBSearchIndex *ClSearchIndex_Current::GetpSearchIndex (void)   
{ 
    return m_pCurrentSearchIndex; 
}
*/

m4return_t ClSearchIndex_Current::SetpSearchIndex (ClBSearchIndex *ai_psi)
{
	m_pCurrentSearchIndex = ai_psi; 
	return M4_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// ClSearchIndex_List
//

/*
ClSearchIndex_List::ClSearchIndex_List ()
{
    m_pSearchIndexes = 0;
}
*/

ClSearchIndex_List::~ClSearchIndex_List ()
{
    if (m_pSearchIndexes) {
        delete [] m_pSearchIndexes;
        m_pSearchIndexes = 0;
    }
}

m4return_t ClSearchIndex_List::Init (ClNodeDef *ai_pnodedef, ClCompiledMCR *ai_pcmcr)
{
    m4return_t  res = M4_SUCCESS;

    M4_ASSERT (ai_pnodedef);
    // ya tiene que estar inicializado:
    ClHandle hn = ai_pnodedef->GetHandle ();
    M4_ASSERT (hn);

    m_iCount = ai_pnodedef->NumberOfIndexes ();

    if (m_iCount) {
        m4uint16_t i;
        m_pSearchIndexes = new ClBSearchIndex [m_iCount];

        m4uint32_t h;
        m4uint32_t id;

        for (i = 0; i < m_iCount; i++) {
            h = ai_pcmcr->GetNodeIndexHandleByPosition (hn, i);
            id = ai_pcmcr->GetNodeIndexIdByPosition (hn, i);
            
            res = m_pSearchIndexes[i].Init (ai_pcmcr, h, id);
            if (res == M4_ERROR)
                break;
        }
    }

    return res;
}

m4return_t ClSearchIndex_List::Update (ClCompiledMCR *ai_pcmcr)
{
    m4uint16_t i;
    for (i = 0; i < m_iCount; i++) {
        m_pSearchIndexes[i].Update (ai_pcmcr);
    }

    return M4_SUCCESS;
}

ClBSearchIndex *ClSearchIndex_List::GetSearchIndexByPos (m4uint16_t ai_ipos)
{
    if (ai_ipos < m_iCount) {
        return m_pSearchIndexes + ai_ipos;
    }
    else
        return 0;
}

ClBSearchIndex *ClSearchIndex_List::GetSearchIndexById (m4uint32_t ai_id)
{
    //búsqueda lineal por los ids:
    m4uint16_t i;

    ClBSearchIndex *psi = m_pSearchIndexes;

    for (i = 0; i < m_iCount; i++) {        
        if (ai_id == psi->Id ())
            return psi;

        psi++;
    }

    return 0;
}

m4return_t 
ClSearchIndex_List::GetSize(ClSize &ao_size)
{
	ao_size += sizeof (ClSearchIndex_List);

	for (m4uint32_t i=0;i<m_iCount;++i)
	{
		if (m_pSearchIndexes[i].GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;

}

m4return_t 
ClSearchIndex_Current::GetSize(ClSize &ao_size)
{
	ao_size += sizeof (ClSearchIndex_Current);
	if (m_pCurrentSearchIndex){
		if (m_pCurrentSearchIndex->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}


