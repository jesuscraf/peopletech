//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             regi_smp.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             04/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Simple filter code
//
//
//==============================================================================

#include "regi_smp.hpp"
#include "px_block.hpp"

#include "accesspr.hpp"

#include "blockref.hpp"


ClFU_Nihil _ClRegisterFilter_Terminator::m_ClFU_Nihil;

_ClRegisterFilter::_ClRegisterFilter ()
{
}

_ClRegisterFilter::~_ClRegisterFilter ()
{
}

ClRegisterFilter_Simple::ClRegisterFilter_Simple ()
{
}

ClRegisterFilter_Simple::~ClRegisterFilter_Simple ()
{
}

m4return_t ClRegisterFilter_Simple::Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (aio_MMIndex != ClMMIndex_EOF)
	{
        m4uint32_t n;
        Count (n, ai_br, ai_pap);

        if (n)
		{
            if (aio_MMIndex < (n-1))
                ++aio_MMIndex;
            else
                aio_MMIndex = ClMMIndex_EOF;
        }
        else
            aio_MMIndex = ClMMIndex_EOF;
    }

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_Simple::Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (aio_MMIndex != ClMMIndex_EOF) {
        m4uint32_t n;
        Count (n, ai_br, ai_pap);

        if (n) {
            if (aio_MMIndex > 0)
                --aio_MMIndex;
            else
                aio_MMIndex = ClMMIndex_EOF;
        }
        else
            aio_MMIndex = ClMMIndex_EOF;
    }

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_Simple::Begin (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4uint32_t n;
    Count (n, ai_br, ai_pap);

    if (n)
        aio_MMIndex = 0;
    else
        aio_MMIndex = ClMMIndex_EOF;

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_Simple::End (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4uint32_t n;
    Count (n, ai_br, ai_pap);

    if (n)
        aio_MMIndex = n - 1;
    else
        aio_MMIndex = ClMMIndex_EOF;

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_Simple::MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4uint32_t n;
    Count (n, ai_br, ai_pap);

    if (ai_newindex < n)
        aio_MMIndex = ai_newindex;
    else
        aio_MMIndex = ClMMIndex_EOF;

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_Simple::MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    aio_MMIndex = ClMMIndex_EOF;

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_Simple::Count (m4uint32_t &ao_count, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ao_count = ai_br->Count ();

    return M4_SUCCESS;
}

m4bool_t ClRegisterFilter_Simple::IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //devuelve siempre cierto
    return M4_TRUE;
}

m4return_t ClRegisterFilter_Simple::UpdateIndex  (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    aio_Index = ai_MMIndex;
    return M4_SUCCESS;
}

m4return_t 
_ClRegisterFilter_Generic::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(_ClRegisterFilter_Generic);

	if (m_pName){
		ao_size+=strlen (m_pName)+1;
	}

	if (m_pClFilterUtility){
		if (m_pClFilterUtility->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	if (m_pSubFilter){
		if (m_pSubFilter->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	// ¿tengo que contar el Father?.
	// ¿ y el filterInterface?.

	return M4_SUCCESS;
}
