//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compstit_map.hpp
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
//    class ClComponentStore Forward Iterators
//
//==============================================================================


#ifndef __COMPSSIT_VECTOR_HPP__
#define __COMPSSIT_VECTOR_HPP__
#include "m4dm_dll.hpp"

#include "m4types.hpp"

#include "compstit_common.hpp"

#include "compss_vector.hpp"

class ClBlock_Channel_Proxy;

class _ClComponentSubStore_map;
//class ClComponentStore_RB_map;

class ClBlockReference;

////////////////////////////////////////////////////////////////////////////////
// _ClComponentSubStore_map_ForwardIt
//

class M4_DECL_M4DM _ClComponentSubStore_map_ForwardIt : public _AClComponentStore_ForwardIt
{
public:    
    _ClComponentSubStore_map_ForwardIt (_ClComponentSubStore_map *ai_pcs) :
        m_pSS (ai_pcs),
        m_iCurrent (SS_MAP_FI_EOF)
    {
    }
    _ClComponentSubStore_map_ForwardIt () :
        m_pSS (0),
        m_iCurrent (SS_MAP_FI_EOF)
    {
    }

    void Attach (_ClComponentSubStore_map *);

    //ClBlock_Channel_Proxy *operator * ();
	m4bool_t Get   (ClBlockReference &ao_comp);

    m4bool_t Begin (ClBlockReference &ao_comp);
    m4bool_t End   (ClBlockReference &ao_comp);
    m4bool_t Next  (ClBlockReference &ao_comp);
    m4bool_t Prev  (ClBlockReference &ao_comp);

	m4bool_t IsNotEOF (void);

    virtual ~_ClComponentSubStore_map_ForwardIt () {}

	m4return_t GetSize(ClSize &ao_size);


private:
	//_ClComponentSubStore_map::MAPBLOCKS::iterator m_it;
	//_ClComponentSubStore_map::MAPBLOCKS *m_pmap;
    m4uint32_t m_iCurrent;
    _ClComponentSubStore_map *m_pSS;
};

#endif //__COMPSSIT_VECTOR_HPP__
