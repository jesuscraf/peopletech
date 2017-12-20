//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compstit_map.cpp
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


#include "compstit_map.hpp"

//#include "compst_map.hpp"

//provisional
//#include "px_block.hpp"

#include "blockref.hpp"



////////////////////////////////////////////////////////////////////////////////
// ClComponentStore_RB_map_ForwardIt
//

ClComponentStore_RB_map_ForwardIt::ClComponentStore_RB_map_ForwardIt (ClComponentStore_RB_map *ai_pcs)
{
    Attach (ai_pcs);
}

void ClComponentStore_RB_map_ForwardIt::Attach (ClComponentStore_RB_map *ai_pcs)
{
    M4_ASSERT (ai_pcs);

    m_pmap = ai_pcs->_GetpMap ();

    M4_ASSERT (m_pmap);
}

m4bool_t ClComponentStore_RB_map_ForwardIt::Get (ClBlockReference &ao_comp)
{
    return m_ssfi.Get (ao_comp);
}

m4bool_t ClComponentStore_RB_map_ForwardIt::Begin (ClBlockReference &ao_comp)
{
	m_it = m_pmap->begin ();
	if (m_it != m_pmap->end ()) {
		M4_ASSERT ((*m_it).second);
		m_ssfi.Attach ( (*m_it).second );
		if (m_ssfi.Begin (ao_comp)) {
			return M4_TRUE;			
		}
		else {
			return _getnextvalid (ao_comp);
		}
	}
	else {
		ao_comp.Reset ();
		return M4_FALSE;
	}    
}

m4bool_t ClComponentStore_RB_map_ForwardIt::_getnextvalid (ClBlockReference &ao_comp)
{
	m4bool_t isnoteof = m_ssfi.IsNotEOF ();

	for (;;) {
		if (isnoteof) {
			return m_ssfi.Get (ao_comp);
		}

		//el ss en el que estamos está vacio, vamos al siguiente:
		++m_it;
		if (m_it == m_pmap->end ()) {
			//hemos terminado
			ao_comp.Reset ();
			return M4_FALSE;
		}

		M4_ASSERT ( (*m_it).second );
		m_ssfi.Attach ( (*m_it).second );
		isnoteof = m_ssfi.Begin (ao_comp);
	}
}

m4bool_t ClComponentStore_RB_map_ForwardIt::End (ClBlockReference &ao_comp)
{
	//que forma más cutre de ir al último elemento:
	m_it = m_pmap->end ();
	if (m_pmap->size ()) {
		--m_it;
		M4_ASSERT ( (*m_it).second );
		m_ssfi.Attach ( (*m_it).second );
		if (m_ssfi.End (ao_comp)) {
			return M4_TRUE;
		}
		else {
			return _getprevvalid (ao_comp);
		}
	}
	else {
		ao_comp.Reset ();
		return M4_FALSE;
	}    
}

m4bool_t ClComponentStore_RB_map_ForwardIt::_getprevvalid (ClBlockReference &ao_comp)
{
	m4bool_t isnoteof = m_ssfi.IsNotEOF ();

	for (;;) {
		if (isnoteof) {
			return m_ssfi.Get (ao_comp);
		}

		//el ss en el que estamos está vacio, vamos al anterior:
		--m_it;
		if (m_it == m_pmap->end ())	{
			//hemos terminado
			ao_comp.Reset ();
			return M4_FALSE;
		}

		M4_ASSERT ( (*m_it).second );
		m_ssfi.Attach ( (*m_it).second );
		isnoteof = m_ssfi.End (ao_comp);
	}
}

m4bool_t ClComponentStore_RB_map_ForwardIt::Next (ClBlockReference &ao_comp)
{
	if (m_ssfi.IsNotEOF ()) {
		//no estamos en eof
		if (m_ssfi.Next (ao_comp))
			return M4_TRUE;
		else
			return _getnextvalid (ao_comp);
	}
	else {
		ao_comp.Reset ();
		return M4_FALSE;
	}
}

m4bool_t ClComponentStore_RB_map_ForwardIt::Prev (ClBlockReference &ao_comp)
{
	if (m_ssfi.IsNotEOF ()) {
		//no estamos en eof
		if (m_ssfi.Prev (ao_comp))
			return M4_TRUE;
		else
			return _getprevvalid (ao_comp);
	}
	else {
		ao_comp.Reset ();
		return M4_FALSE;
	}
}

m4bool_t ClComponentStore_RB_map_ForwardIt::IsNotEOF (void)
{
	return m_ssfi.IsNotEOF ();
}
m4return_t 
ClComponentStore_RB_map_ForwardIt::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClComponentStore_RB_map_ForwardIt);
		ao_size -= sizeof(m_ssfi);
	
	if (m_ssfi.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
// CreateIterator
//

_AClComponentStore_ForwardIt *ClComponentStore_RB_map::CreateIterator (void)
{
    return new ClComponentStore_RB_map_ForwardIt (this);
}

