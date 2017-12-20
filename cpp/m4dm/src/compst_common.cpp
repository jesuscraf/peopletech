//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compst_common.cpp
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

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "compst_common.hpp"

    #include "px_block.hpp" //de momento.

///////////////////////////////////////////////////////////////////////////////
//
// _AClComponentStore
//

_AClComponentStore::_AClComponentStore (ClNode_Data *ai_pnode)
{
    m_pNode = ai_pnode;
}

_AClComponentStore::~_AClComponentStore ()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// ClComponentStore_Mono
//

ClComponentStore_Mono::ClComponentStore_Mono (ClNode_Data *ai_pnode) : _AClComponentStore (ai_pnode)
{
    m_pProxy = 0;
}

ClComponentStore_Mono::~ClComponentStore_Mono ()
{
    RemoveAll ();
}

//devuelve lo que haya:
m4return_t ClComponentStore_Mono::Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component)
{
    if (m_pProxy) {
        if ( (m_pProxy->ID.BlockId == ai_idblock) &&
            (m_pProxy->ID.RegisterId == ai_idreg) ) 
        {
            ao_component.Set (m_pProxy);
            return M4_SUCCESS;
        }
    }

    ao_component.Reset ();
    return M4_ERROR;
}

m4return_t ClComponentStore_Mono::Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid)
{
    if (Get (ai_idblock, ai_idreg, ao_component) == M4_ERROR) {
		ClBlock_Channel_Proxy::GetNewProxy (m_pNode, ai_idblock, ai_idreg, ai_parentblockid, ai_parentregisterid, ao_component);

        m_pProxy = ao_component.GetpProxy ();
        M4_ASSERT (m_pProxy);
        m_pProxy->ID.BlockUniqueKey = 0;
    }

    return M4_SUCCESS;
}


m4return_t ClComponentStore_Mono::Notify_ParentAddRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg)
{
	//me la pela
	return M4_SUCCESS;
}

m4return_t ClComponentStore_Mono::Notify_ParentRemoveRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg)
{
	//me la pela
	return M4_SUCCESS;
}

/*
m4return_t ClComponentStore_Mono::Add (m4uint32_t ai_idblock, m4uint32_t ai_idreg, const ClBlockReference &ai_br, m4uint32_t &ao_id)
{
    m_pProxy = ai_br.GetpProxy ();

        //m_pProxy->IncRef ();    //para que no se borre sólo, de momento.

    ao_id = 0;

    M4_ASSERT (m_pProxy);

    return M4_SUCCESS;
}
*/

m4return_t ClComponentStore_Mono::Remove (m4uint32_t ai_idblock, m4uint32_t ai_idreg)
{
    return RemoveAll ();
}

m4return_t ClComponentStore_Mono::RemoveAll (void)
{
    m_pProxy = 0;

    return M4_SUCCESS;
}

m4return_t ClComponentStore_Mono::ParentDestroyAllRegisters (m4uint32_t ai_idblock)
{
	//me la pela que en mi padre se haya destruido todos los registros
	return M4_SUCCESS;
}

m4return_t ClComponentStore_Mono::ParentDestroyBlock (m4uint32_t ai_blockuniquekey)
{
	//me la pela que en mi padre se haya destruido el bloque
	return M4_SUCCESS;
}

m4uint32_t ClComponentStore_Mono::Count (void)
{
    return (m_pProxy)? 1 : 0;
}

void ClComponentStore_Mono::UpdateIndex (m4uint32_t ai_idblock, m4uint32_t *ai_pindex_qtc, m4uint32_t ai_icount)
{
    M4_ASSERT (0);

    //Si hemos llegado hasta aquí, quiere decir que tenemos un padre y estamos
    //unidos a él en modo RB. Un poco raro si tenemos en cuenta que yo soy 'mono'
    //(osea, free).
}

m4uint32_t ClComponentStore_Mono::MemoryUsed (void)
{
    return sizeof (ClComponentStore_Mono);
}

/*
m4return_t ClComponentStore_Mono::ParentBlockCreated (m4uint32_t ai_parentidblock, m4uint32_t ai_parentidreg, m4uint32_t ai_uk)
{
	//no puede ser cierto que me hayan llamado
	M4_ASSERT (0);

	return M4_ERROR;
}
*/

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*
void ClComponentStore_Mono::_UpdateOneIndex (m4uint32_t ai_blockid, m4uint32_t ai_iold, m4uint32_t ai_inew)
{
}
*/
m4return_t 
_AClComponentStore::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_AClComponentStore);
	return M4_SUCCESS;

}

m4return_t 
ClComponentStore_Mono::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClComponentStore_Mono);
		ao_size-=sizeof(_AClComponentStore);

	if (_AClComponentStore::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_pProxy){
		if (m_pProxy->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}
