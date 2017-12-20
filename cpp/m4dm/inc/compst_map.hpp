//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compst_map.hpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             03/09/98
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



#ifndef __COMPST_MAP_HPP__
#define __COMPST_MAP_HPP__

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "compst_common.hpp"
#include "reusing.hpp"

class _AClComponentStore_ForwardIt;
class ClNode_Data;
class ClBlockReference;
class _ClComponentSubStore_map;
class ClMutex;


class M4_DECL_M4DM ClComponentStore_RB_map : public _AClComponentStore
{
public:
    ClComponentStore_RB_map (ClNode_Data *ai_pnode);
   ~ClComponentStore_RB_map ();

    m4return_t Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component);
    m4return_t Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid);

    m4return_t NewElement (m4uint32_t &ao_id);
    m4return_t Remove (m4uint32_t ai_idblock, m4uint32_t ai_idreg);

    m4return_t Notify_ParentAddRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg);
    m4return_t Notify_ParentRemoveRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg);

    m4return_t RemoveAll (void);
	m4return_t ParentDestroyAllRegisters (m4uint32_t ai_idblock);
	m4return_t ParentDestroyBlock (m4uint32_t ai_idblock);

    m4uint32_t Count (void);
    void UpdateIndex (m4uint32_t ai_idblock, m4uint32_t *ai_pindex_qtc, m4uint32_t ai_icount);

    _AClComponentStore_ForwardIt *CreateIterator (void);    

    m4uint32_t MemoryUsed (void);

    m4return_t Reserve(m4uint32_t ai_lSize);
    m4return_t ForceLock( void );

    Type GetType (void) const { return RB; }

	m4return_t GetSize(ClSize &ao_size);

public:
	typedef map<m4uint32_t, _ClComponentSubStore_map *, less<m4uint32_t> > MAPSS;
	
private:	
    MAPSS		m_mapss;
	ClMutex*	m_poMapssMutex;

	static void s_remove (MAPSS::value_type ai_vt);

	ClReusingBlockUKey UKProvider;
    m4uint32_t m_iCount;

public:
	MAPSS *_GetpMap (void)
	{
		return &m_mapss;
	}

	_ClComponentSubStore_map *GetSS_NoCreate (m4uint32_t ai_idblock);
	_ClComponentSubStore_map *GetSS_CreateIfNoExist (m4uint32_t ai_idblock);
};


#endif //__COMPST_MAP_HPP__
