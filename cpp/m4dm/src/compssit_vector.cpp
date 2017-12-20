//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compssit_vector.cpp
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
//    class ClComponentStore iterator
//
//==============================================================================


#include "compssit_vector.hpp"

//#include "compst_map.hpp"

//provisional
#include "px_block.hpp"

#include "blockref.hpp"

////////////////////////////////////////////////////////////////////////////////
// _ClComponentSubStore_map_ForwardIt
//

/*
_ClComponentSubStore_map_ForwardIt::_ClComponentSubStore_map_ForwardIt ()
{
}

_ClComponentSubStore_map_ForwardIt::_ClComponentSubStore_map_ForwardIt (_ClComponentSubStore_map *ai_pcs)
{
    Attach (ai_pcs);
}
*/

void _ClComponentSubStore_map_ForwardIt::Attach (_ClComponentSubStore_map *ai_pcs)
{
    M4_ASSERT (ai_pcs);

    m_pSS = ai_pcs;

    M4_ASSERT (m_pSS);
}


m4bool_t _ClComponentSubStore_map_ForwardIt::Get (ClBlockReference &ao_comp)
{
    /*
	if (m_it != m_pmap->end ()) {
		ao_comp.Set ((*m_it).second);
		return M4_TRUE;
	}
	else {
		ao_comp.Reset ();
		return M4_FALSE;
	}
    */
    ClBlock_Channel_Proxy *pb;
    pb = m_pSS->_vector_find (m_iCurrent);
    if (pb) {
        ao_comp.Set (pb);
        return M4_TRUE;
    }
    else {
        ao_comp.Reset ();
        return M4_FALSE;
    }
}

m4bool_t _ClComponentSubStore_map_ForwardIt::Begin (ClBlockReference &ao_comp)
{
    m_iCurrent = 0;

    ClBlock_Channel_Proxy *pb;
    pb = m_pSS->_getnextvalid (m_iCurrent);
    if (pb) {
        ao_comp.Set (pb);
        return M4_TRUE;
    }
    else {
        ao_comp.Reset ();
        return M4_FALSE;
    }
}

m4bool_t _ClComponentSubStore_map_ForwardIt::End (ClBlockReference &ao_comp)
{
    ClBlock_Channel_Proxy *pb;
    pb = m_pSS->_getlast (m_iCurrent);
    if (pb) {
        ao_comp.Set (pb);
        return M4_TRUE;
    }
    else {
        ao_comp.Reset ();
        return M4_FALSE;
    }
}

m4bool_t _ClComponentSubStore_map_ForwardIt::Next (ClBlockReference &ao_comp)
{
    ++m_iCurrent;

    ClBlock_Channel_Proxy *pb;
    pb = m_pSS->_getnextvalid (m_iCurrent);
    if (pb) {
        ao_comp.Set (pb);
        return M4_TRUE;
    }
    else {
        ao_comp.Reset ();
        return M4_FALSE;
    }
}

m4bool_t _ClComponentSubStore_map_ForwardIt::Prev (ClBlockReference &ao_comp)
{
    --m_iCurrent;

    ClBlock_Channel_Proxy *pb;
    pb = m_pSS->_getprevvalid (m_iCurrent);
    if (pb) {
        ao_comp.Set (pb);
        return M4_TRUE;
    }
    else {
        ao_comp.Reset ();
        return M4_FALSE;
    }
}

m4bool_t _ClComponentSubStore_map_ForwardIt::IsNotEOF (void)
{
	return M4_BOOL(m_iCurrent != SS_MAP_FI_EOF);
}
m4return_t 
_ClComponentSubStore_map_ForwardIt::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_ClComponentSubStore_map_ForwardIt);

	return M4_SUCCESS;
}

