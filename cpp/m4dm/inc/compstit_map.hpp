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


#ifndef __COMPSTIT_MAP_HPP__
#define __COMPSTIT_MAP_HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"

#include "compstit_common.hpp"
#include "compst_map.hpp"

#include "compssit_vector.hpp"

class ClBlock_Channel_Proxy;

//class _ClComponentSubStore_map;
class ClComponentStore_RB_map;

class ClBlockReference;

////////////////////////////////////////////////////////////////////////////////
// ClComponentStore_RB_map_ForwardIt
//

class M4_DECL_M4DM ClComponentStore_RB_map_ForwardIt : public _AClComponentStore_ForwardIt
{
public:    
    void Attach (ClComponentStore_RB_map *);

    //ClBlock_Channel_Proxy *operator * ();
	m4bool_t Get   (ClBlockReference &ao_comp);

    m4bool_t Begin (ClBlockReference &ao_comp);
    m4bool_t End   (ClBlockReference &ao_comp);
    m4bool_t Next  (ClBlockReference &ao_comp);
    m4bool_t Prev  (ClBlockReference &ao_comp);

	m4bool_t IsNotEOF (void);

	m4return_t GetSize(ClSize &ao_size);

private:
    m4bool_t _getprevvalid (ClBlockReference &ao_comp);
    m4bool_t _getnextvalid (ClBlockReference &ao_comp);

    _ClComponentSubStore_map_ForwardIt m_ssfi;
	ClComponentStore_RB_map::MAPSS::iterator m_it;
	ClComponentStore_RB_map::MAPSS *m_pmap;



//por seguridad se impide la construcción del objeto fuera de sus store respectivo
    
    //ClComponentStore_RB_map
    ClComponentStore_RB_map_ForwardIt (ClComponentStore_RB_map *);
    
    //_AClComponentStore:
    ~ClComponentStore_RB_map_ForwardIt () {}

friend class _AClComponentStore;
friend class ClComponentStore_RB_map;
};

#endif // __COMPSTIT_MAP_HPP__
