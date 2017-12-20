//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             mmarray.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             29/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    MMIndex Array for RAM Indexes
//
//
//==============================================================================

#include "mmarray.hpp"

#include "regi_smp.hpp"
#include "px_block.hpp"
#include "blockref.hpp"
#include "chlog.hpp"

#include <string.h> 

#define RegisterBlock 10

ClMMArray::ClMMArray (void)
{
	m_pClMMIndex = 0;
	m_iCount = 0;
	m_iArraySize = 0;
    m_iNewElements = 0;
}

ClMMArray::~ClMMArray (void)
{
	//m_pClMMIndex = 0;
	//m_iCount = 0;
    RemoveAll ();
}


m4return_t ClMMArray::Get (m4uint32_t ai_iIndex, ClMMIndex &aio_MMIndex)
{
	if (m_pClMMIndex)
	{
		if (ai_iIndex < m_iCount)
		{
			aio_MMIndex = m_pClMMIndex[ai_iIndex];
			return M4_SUCCESS;
		}
		else
		{
			aio_MMIndex = ClMMIndex_EOF;
			return M4_ERROR;
		}
	}
	else
	{
		aio_MMIndex = ClMMIndex_EOF;
		return M4_ERROR;
	}
}

m4return_t ClMMArray::Set (m4uint32_t ai_iIndex, ClMMIndex ai_MMIndex)
{
	if (m_pClMMIndex)
	{
		if (ai_iIndex < m_iCount)
		{
			m_pClMMIndex[ai_iIndex] = ai_MMIndex;
			return M4_SUCCESS;
		}
		else
		{
			return M4_ERROR;
		}
	}
	else
	{
		return M4_ERROR;
	}
}

m4return_t ClMMArray::Copy(ClMMArray * ai_poMMArray)
{
    m4uint32_t i;

    m_iCount = ai_poMMArray->m_iCount;
    m_iArraySize = ai_poMMArray->m_iArraySize;
    m_iNewElements = ai_poMMArray->m_iNewElements;
    
    if (m_pClMMIndex)
        delete [] m_pClMMIndex;

    if (m_iCount)
    {
        m_pClMMIndex = new ClMMIndex [m_iArraySize];

        for (i = 0; i < m_iCount; i ++)
        {
            m_pClMMIndex[i] = ai_poMMArray->m_pClMMIndex[i];
        }
    }
    else
        m_pClMMIndex = 0;

    return M4_SUCCESS;
}

m4return_t ClMMArray::Remove (m4uint32_t ai_iIndex, ClMMIndex &aio_MMIndex)
{
	if (m_pClMMIndex)
	{
		if (m_iCount)
		{
            if (ai_iIndex < m_iCount -1)
            {
                if (m_pClMMIndex[ai_iIndex] == aio_MMIndex)
                {
    			    memmove(&m_pClMMIndex[ai_iIndex], &m_pClMMIndex[ai_iIndex + 1], (m_iCount - ai_iIndex - 1) * (sizeof (ClMMIndex)));
                    m_iCount--;
                }
            }
            else if (ai_iIndex == m_iCount -1)
            {
                if (m_pClMMIndex[ai_iIndex] == aio_MMIndex)
                    m_iCount--;
            }
            return M4_SUCCESS;
		}
		return M4_ERROR;
	}
	return M4_ERROR;
}

m4return_t ClMMArray::Add (m4uint32_t ai_iIndex, ClMMIndex ai_MMIndex)
{
    m_iNewElements ++;
	if ((m_iCount)&&(ai_iIndex<=m_iCount))
	{
		M4_ASSERT(m_pClMMIndex);

		if (m_iCount == m_iArraySize)
		{
			ClMMIndex * p_aux = 0;

			m_iArraySize += RegisterBlock;
			p_aux = new ClMMIndex[m_iArraySize]; //Tengo que contar el count!!!!!!!
			memcpy(p_aux, m_pClMMIndex, m_iCount * (sizeof (ClMMIndex)));

			delete [] m_pClMMIndex;

			m_pClMMIndex = p_aux;
		}
		else if (m_iCount > m_iArraySize)
			return M4_ERROR;

		if (ai_iIndex <= m_iCount - 1)
			memmove(&m_pClMMIndex[ai_iIndex + 1], &m_pClMMIndex[ai_iIndex], (m_iCount - ai_iIndex) * (sizeof (ClMMIndex)));

		m_iCount++;
		m_pClMMIndex [ai_iIndex] = ai_MMIndex;

		return M4_SUCCESS;
	}
	else
	{
		if (m_pClMMIndex)
			delete [] m_pClMMIndex;
		m_pClMMIndex = new ClMMIndex[RegisterBlock];
		m_pClMMIndex[0] = ai_MMIndex;
		m_iCount = 1;
		m_iArraySize = RegisterBlock;

		return M4_SUCCESS;
	}
}

m4return_t ClMMArray::Count (m4uint32_t &ao_iCount)
{
	ao_iCount = m_iCount;
	return M4_SUCCESS;
}

m4return_t ClMMArray::RemoveAll (void)
{
    if (m_pClMMIndex)
    {
        delete [] m_pClMMIndex;
        m_pClMMIndex = 0;
    }    
    m_iCount = 0;
	m_iArraySize = 0;
    ResetNewElements ();

    return M4_SUCCESS;
}

m4return_t ClMMArray::Set (m4uint32_t * ai_pIndexArray, m4uint32_t ai_number, const ClBlockReference &ai_br, ClAccessProvider *ai_pap, _ClRegisterFilter * ai_pClSubFilter)
{
    M4_ASSERT(ai_pIndexArray);

	if (ai_number >= m_iArraySize)
	{
		while (ai_number >= m_iArraySize)
			m_iArraySize += RegisterBlock;
		delete [] m_pClMMIndex;
		m_pClMMIndex = new ClMMIndex[m_iArraySize];
	}

    m4uint32_t i;
    ClMMIndex aux_Index = 0;
    ClRegisterIndex aio_Index;
    m4return_t res;

    for (i = 0; i < ai_number; i++)
    {
        res = ai_pClSubFilter->MoveTo(ai_pIndexArray[i], aio_Index, aux_Index, ai_br, ai_pap);
        if (res != M4_SUCCESS)
        {
    		delete [] m_pClMMIndex;
			m_pClMMIndex = 0;
            return M4_ERROR;
        }

        m_pClMMIndex[i] = aux_Index;
    }

    m_iCount = ai_number;
    ResetNewElements ();

    return M4_SUCCESS;
}

m4return_t ClMMArray::Init (m4uint32_t ai_number, const ClBlockReference &ai_br, ClAccessProvider *ai_pap, _ClRegisterFilter * ai_pClSubFilter)
{
	if (ai_number >= m_iArraySize)
	{
		while (ai_number >= m_iArraySize)
			m_iArraySize += RegisterBlock;
		delete [] m_pClMMIndex;
		m_pClMMIndex = new ClMMIndex[m_iArraySize];
	}

    m4uint32_t i;
    ClMMIndex aux_Index = 0;
    ClRegisterIndex aio_Index;
    m4return_t res;

    for (i = 0; i < ai_number; i++)
    {
        res = ai_pClSubFilter->MoveTo(i, aio_Index, aux_Index, ai_br, ai_pap);
        if (res != M4_SUCCESS)
        {
    		delete [] m_pClMMIndex;
            return M4_ERROR;
        }

        m_pClMMIndex[i] = aux_Index;
    }

    m_iCount = ai_number;
    ResetNewElements ();

    return M4_SUCCESS;
}

m4return_t ClMMArray::Notify_DeleteRegister (ClMMIndex &aio_MMIndex)
{
	if (m_pClMMIndex)
	{
		if (m_iCount)
		{
			m4uint32_t i;
            for (i = 0; i < m_iCount; i++)
			{
                if (((m4uint32_t)m_pClMMIndex[i]) > ((m4uint32_t)aio_MMIndex))
				    --(m_pClMMIndex[i]);
			}
            return M4_SUCCESS;
		}
		return M4_ERROR;
	}
	return M4_ERROR;
}

m4return_t ClMMArray::Notify_AddRegister (ClMMIndex ai_MMIndex)
{
	if (m_pClMMIndex)
	{
	    m4uint32_t i;
        for (i = 0; i < m_iCount; i++)
	    {
            if (((m4uint32_t)m_pClMMIndex[i]) >= ((m4uint32_t)ai_MMIndex))
    		    ++(m_pClMMIndex[i]);
	    }

	    return M4_SUCCESS;
	}
	return M4_ERROR;
}

m4uint32_t  ClMMArray::GetNewElements (void)
{
    return m_iNewElements;
}

void ClMMArray::ResetNewElements (void)
{
    m_iNewElements = 0;
}

m4return_t 
ClMMArray::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClMMArray);

	ao_size+=m_iArraySize*sizeof(*m_pClMMIndex);

	return M4_SUCCESS;
}


