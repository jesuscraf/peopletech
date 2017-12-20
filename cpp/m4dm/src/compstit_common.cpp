//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compstit_common.cpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             09/03/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class ClComponentStore
//
//==============================================================================


#include "compstit_common.hpp"
#include "compst_common.hpp"

#include "blockref.hpp"

//provisional
#include "px_block.hpp"

////////////////////////////////////////////////////////////////////////////////
// _AClComponentStore_ForwardIt
//

const m4int32_t _AClComponentStore_ForwardIt::c_EOF = -1;


_AClComponentStore_ForwardIt::_AClComponentStore_ForwardIt ()
{
}

void _AClComponentStore::DestroyIterator (_AClComponentStore_ForwardIt *ai_pi)
{
    delete ai_pi;   //por el tema de el objeto se construya y libere en el mismo heap de memoria
}

////////////////////////////////////////////////////////////////////////////////
// ClComponentStore_Mono_ForwardIt
//
// nota: el root sólo tiene un elemento, asi que este iterador es bastante tonto
//

ClComponentStore_Mono_ForwardIt::ClComponentStore_Mono_ForwardIt (ClComponentStore_Mono *ai_pcs)
{
    Attach (ai_pcs);
}

void ClComponentStore_Mono_ForwardIt::Attach (ClComponentStore_Mono *ai_pcs)
{
    M4_ASSERT (ai_pcs);

    m_pClComponentStore = ai_pcs;

    M4_ASSERT (m_pClComponentStore);
}

m4bool_t ClComponentStore_Mono_ForwardIt::Get (ClBlockReference &ao_comp)
{
    if (m_icurrent != c_EOF) {
        m_pClComponentStore->Get (0, 0, ao_comp);
    }
	else {
		ao_comp.Reset ();
	}	

    return ao_comp.IsNull ()? M4_FALSE : M4_TRUE;
}

m4bool_t ClComponentStore_Mono_ForwardIt::Begin (ClBlockReference &ao_comp)
{
    m_icurrent = 0;
	m_pClComponentStore->Get (0, 0, ao_comp);

    return ao_comp.IsNull ()? M4_FALSE : M4_TRUE;
}

m4bool_t ClComponentStore_Mono_ForwardIt::End (ClBlockReference &ao_comp)
{
    m_icurrent = 0;
	m_pClComponentStore->Get (0, 0, ao_comp);
    
	return ao_comp.IsNull ()? M4_FALSE : M4_TRUE;
}

m4bool_t ClComponentStore_Mono_ForwardIt::Next (ClBlockReference &ao_comp)
{
    m_icurrent = c_EOF;

	ao_comp.Reset ();
    return M4_FALSE;
}

m4bool_t ClComponentStore_Mono_ForwardIt::Prev (ClBlockReference &ao_comp)
{
    m_icurrent = c_EOF;

	ao_comp.Reset ();
    return M4_FALSE;
}
m4return_t 
ClComponentStore_Mono_ForwardIt::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClComponentStore_Mono_ForwardIt);

	return M4_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
// CreateIterator
//

_AClComponentStore_ForwardIt *ClComponentStore_Mono::CreateIterator (void)
{
    return new ClComponentStore_Mono_ForwardIt (this);
}
