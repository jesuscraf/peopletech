//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compst_common.hpp
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


#ifndef __COMPST_COMMON_HPP__
#define __COMPST_COMMON_HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"

#include "blockref.hpp"

class _AClComponentStore_ForwardIt;
class ClNode_Data;
class ClBlock_Channel_Proxy;

class ClSize;
////////////////////////////////////////////////////////////////////////////////
// _AClComponentStore
//

class M4_DECL_M4DM _AClComponentStore
{
public:
    _AClComponentStore (ClNode_Data *ai_pnodedata);
    virtual ~_AClComponentStore ();

    virtual m4return_t Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component) = 0;
	virtual m4return_t Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid) = 0;

    //virtual m4return_t Add (m4uint32_t ai_idblock, m4uint32_t ai_idreg, const ClBlockReference &ai_br, m4uint32_t &ao_id) = 0;
    virtual m4return_t Remove (m4uint32_t ai_idblock, m4uint32_t ai_idreg) = 0;

    virtual m4return_t Notify_ParentAddRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg) = 0;
    virtual m4return_t Notify_ParentRemoveRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg) = 0;

    virtual m4return_t RemoveAll (void) = 0;
	virtual m4return_t ParentDestroyAllRegisters (m4uint32_t ai_idblock) = 0;
	virtual m4return_t ParentDestroyBlock (m4uint32_t ai_idblock) = 0;

	//virtual m4return_t ParentBlockCreated (m4uint32_t ai_parentidblock, m4uint32_t ai_parentidreg, m4uint32_t ai_uk) = 0;

    virtual m4uint32_t Count (void) = 0;
    virtual void UpdateIndex (m4uint32_t ai_idblock, m4uint32_t *ai_pindex_qtc, m4uint32_t ai_icount) = 0;

    virtual _AClComponentStore_ForwardIt *CreateIterator (void) = 0;
    void DestroyIterator (_AClComponentStore_ForwardIt *ai_pi);

    virtual m4uint32_t MemoryUsed (void) = 0;

    virtual m4return_t Reserve(m4uint32_t ai_lSize) = 0;
    virtual m4return_t ForceLock( void ) = 0;

    //virtual void _UpdateOneIndex (m4uint32_t ai_blockid, m4uint32_t ai_iold, m4uint32_t ai_inew) = 0;

    enum Type {MONO, RB};
    virtual Type GetType (void) const = 0;
	virtual m4return_t GetSize(ClSize &ao_size);


protected:
    ClNode_Data *m_pNode;
};

////////////////////////////////////////////////////////////////////////////////
// ClComponentStore_Mono
//

class M4_DECL_M4DM ClComponentStore_Mono : public _AClComponentStore
{
public:
    ClComponentStore_Mono (ClNode_Data *ai_pnodedata);
   ~ClComponentStore_Mono ();

    m4return_t Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component);
    m4return_t Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid);

    //m4return_t Add (m4uint32_t ai_idblock, m4uint32_t ai_idreg, const ClBlockReference &ai_br, m4uint32_t &ao_id);
    m4return_t Remove (m4uint32_t ai_idblock, m4uint32_t ai_idreg);

    m4return_t Notify_ParentAddRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg);
    m4return_t Notify_ParentRemoveRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg);

    m4return_t RemoveAll (void);
	m4return_t ParentDestroyAllRegisters (m4uint32_t ai_idblock);
	m4return_t ParentDestroyBlock (m4uint32_t ai_idblock);

	//m4return_t ParentBlockCreated (m4uint32_t ai_parentidblock, m4uint32_t ai_parentidreg, m4uint32_t ai_uk);

    m4uint32_t Count (void);
    void UpdateIndex (m4uint32_t ai_idblock, m4uint32_t *ai_pindex_qtc, m4uint32_t ai_icount);

    _AClComponentStore_ForwardIt *CreateIterator (void);

    m4uint32_t MemoryUsed (void);

    m4return_t Reserve(m4uint32_t ai_lSize)
    {
        return M4_ERROR;
    }

    m4return_t ForceLock( void )
	{
        return M4_ERROR;
	}
    //void _UpdateOneIndex (m4uint32_t ai_blockid, m4uint32_t ai_iold, m4uint32_t ai_inew);

    Type GetType (void) const { return MONO; }

	m4return_t GetSize(ClSize &ao_size);

private:
    //ClBlockReference m_Component;
    ClBlock_Channel_Proxy *m_pProxy;
};

#endif //__COMPST_COMMON_HPP__
