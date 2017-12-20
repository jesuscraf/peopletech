//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compst_map.hpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             15/10/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class _ClComponentSubStore_xxx
//
//==============================================================================


#ifndef __COMPSS_VECTOR_HPP__
#define __COMPSS_VECTOR_HPP__

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "px_block.hpp"

class ClNode_Data;
class ClBlockReference;
class ClMutex;


////////////////////////////////////////////////////////////////////////////////
// _ClComponentSubStore_map
//

#define SS_MAP_FI_EOF ((m4uint32_t)-1)

class M4_DECL_M4DM _ClComponentSubStore_map
{
public:
    
    _ClComponentSubStore_map (ClNode_Data *ai_pnode);
	~_ClComponentSubStore_map ();

	m4return_t Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClNode_Data *ai_pnode, ClBlockReference &ao_component);
    m4return_t Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClNode_Data *ai_pnode, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid, ClBlockReference &ao_component);

    m4return_t Add (m4uint32_t ai_idreg, const ClBlockReference &ai_br);
    m4uint32_t Remove (m4uint32_t ai_idreg);

    m4return_t Notify_ParentAddRegister (m4uint32_t ai_idreg);
    m4return_t Notify_ParentRemoveRegister (m4uint32_t ai_idreg);

    m4return_t RemoveAll (void);
    m4uint32_t Count (void);
    void UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_icount);    

    m4uint32_t MemoryUsed (void);

    m4return_t Reserve(m4uint32_t ai_lSize);

	void ParentDestroyAllRegisters (void);

	m4return_t GetSize(ClSize &ao_size);

private:

	void _UpdateOneIndex (m4uint32_t ai_iold, m4uint32_t ai_inew, ClBlock_Channel_Proxy **ai_ppool = 0, m4uint32_t ai_poollenght = 0);
    static void _SetRegisterId (ClBlock_Channel_Proxy *ai_pbp, m4uint32_t ai_inew);
	void _changeindex (m4uint32_t ai_inew);

        ClBlock_Channel_Proxy **m_pPool;
        m4uint32_t m_iSize;     //tamaño del pool
        m4uint32_t m_iLast;     //posición del mayor elemento del pool
        m4uint32_t m_iCount;    //elementos guardados en el pool
		ClMutex* m_poPoolMutex;

        ClBlock_Channel_Proxy *m_auxspace;  //buffer para un elemento (optimización)
        
        void _vector_insert (m4uint32_t ai_id, ClBlock_Channel_Proxy *ai_pb);
        ClBlock_Channel_Proxy *_vector_erase (m4uint32_t ai_idreg);
        void _vector_deleteall (void);
        void _vector_newpool (m4uint32_t ai_inewsize);

    ClBlock_Channel_Proxy *_Get_Readers (m4uint32_t ai_idreg, ClNode_Data *ai_pnode);
    ClBlock_Channel_Proxy *_Get_Writer (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClNode_Data *ai_pnode, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid);

public:
        ClBlock_Channel_Proxy *_vector_find (m4uint32_t ai_id);
        ClBlock_Channel_Proxy *_getlast (m4uint32_t &ao_current);
        ClBlock_Channel_Proxy *_getprevvalid (m4uint32_t &aio_current);
        ClBlock_Channel_Proxy *_getnextvalid (m4uint32_t &aio_current);

public:
        static const m4uint32_t m_cPercentageDownSize;
        static const m4uint32_t m_cPoolMinIncrement;
};

#endif //__COMPSS_VECTOR_HPP__
