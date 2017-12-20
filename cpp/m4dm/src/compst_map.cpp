//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             compst_map.cpp
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

#include "dm_def.hpp"
#include "m4define.hpp"
#include "compst_map.hpp"
#include "compss_vector.hpp"
#include "blockref.hpp"
#include "m4stl.hpp"
#include "node_knl.hpp"  
#include "chan_knl.hpp"
#include "sizerutils.hpp"
#include "m4mdrt.hpp"
#include "syncro.hpp"


///////////////////////////////////////////////////////////////////////////////
//
// ClComponentStore_RB_map
//

ClComponentStore_RB_map::ClComponentStore_RB_map (ClNode_Data *ai_pnode) : _AClComponentStore (ai_pnode)
{
    m_iCount = 0;
	m_poMapssMutex = NULL;

	if( ai_pnode->GetpNodeDef()->GetpCMCR()->GetChannelIsCacheable() == 1 )
	{
		ForceLock();
	}
}

ClComponentStore_RB_map::~ClComponentStore_RB_map ()
{
    RemoveAll ();

	if( m_poMapssMutex != NULL )
	{
		delete m_poMapssMutex ;
		m_poMapssMutex = NULL ;
	}
}

//si no existe, no crea el proxy
m4return_t ClComponentStore_RB_map::Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component)
{
    _ClComponentSubStore_map *pss = GetSS_NoCreate (ai_idblock);
	if (pss) {
		pss->Get (ai_idblock, ai_idreg, m_pNode, ao_component);
	}
	else {
		ao_component.Reset ();
	}

    return (ao_component.IsNull ())? M4_ERROR : M4_SUCCESS;
}

//si no existe, crea el proxy
m4return_t ClComponentStore_RB_map::Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClBlockReference &ao_component, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid)
{
    _ClComponentSubStore_map *pss = GetSS_CreateIfNoExist (ai_idblock);
	M4_ASSERT (pss);	//tiene que existir por narices
	pss->Get (ai_idblock, ai_idreg, m_pNode, ai_parentblockid, ai_parentregisterid, ao_component);

    return (ao_component.IsNull ())? M4_ERROR : M4_SUCCESS;
}


_ClComponentSubStore_map *ClComponentStore_RB_map::GetSS_NoCreate (m4uint32_t ai_idblock)
{
	_ClComponentSubStore_map *pss;

	MAPSS::iterator it;

	if( m_poMapssMutex != NULL )
	{
		m_poMapssMutex->Lock();
	}

	it = m_mapss.find (ai_idblock);

	if (m_mapss.end () == it) {
        pss = 0;
	}
	else {
		pss = (*it).second;
		M4_ASSERT (pss);
	}

	if( m_poMapssMutex != NULL )
	{
		m_poMapssMutex->Unlock();
	}

	return pss;
}

_ClComponentSubStore_map *ClComponentStore_RB_map::GetSS_CreateIfNoExist (m4uint32_t ai_idblock)
{
	_ClComponentSubStore_map *pss;

	MAPSS::iterator it;

	if( m_poMapssMutex != NULL )
	{
		m_poMapssMutex->Lock();
	}

	it = m_mapss.find (ai_idblock);

	if (m_mapss.end () == it) {
		//no está, lo creamos
		pss = new _ClComponentSubStore_map (m_pNode);
		m_mapss.insert (MAPSS::value_type (ai_idblock, pss));
		M4_ASSERT (pss);
	}
	else {
		pss = (*it).second;
		M4_ASSERT (pss);
	}
    
	if( m_poMapssMutex != NULL )
	{
		m_poMapssMutex->Unlock();
	}
	
	return pss;
}

//actualiza indices:
m4return_t ClComponentStore_RB_map::Notify_ParentAddRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg)
{
    _ClComponentSubStore_map *pss = GetSS_NoCreate (ai_idblock);
	if (pss)
		return  pss->Notify_ParentAddRegister (ai_idreg);
	else
		return M4_SUCCESS;
}

//actualiza indices:
m4return_t ClComponentStore_RB_map::Notify_ParentRemoveRegister (m4uint32_t ai_idblock, m4uint32_t ai_idreg)
{
    _ClComponentSubStore_map *pss = GetSS_NoCreate (ai_idblock);
	if (pss)
		return  pss->Notify_ParentRemoveRegister (ai_idreg);
	else
		return M4_SUCCESS;
}

m4return_t ClComponentStore_RB_map::NewElement (m4uint32_t &ao_id)
{
    m4return_t res;

	res = UKProvider.Get (ao_id);
	if (M4_SUCCESS == res) {
		m_iCount++;
	}
	else {
		//problema, nos hemos quedado sin claves, hay que compactar el mapa:
		//...
	}

    return res;
}

m4return_t ClComponentStore_RB_map::Remove (m4uint32_t ai_idblock, m4uint32_t ai_idreg)
{
    _ClComponentSubStore_map *pss = GetSS_NoCreate (ai_idblock);
	M4_ASSERT (pss);

	m4uint32_t uk = pss->Remove (ai_idreg);
	UKProvider.Reuse (uk);
    m_iCount--;

    return M4_SUCCESS;
}

/*static*/
void ClComponentStore_RB_map::s_remove (MAPSS::value_type ai_vt)
{
    ai_vt.second->RemoveAll ();

	delete ai_vt.second;	//yo creo el ss y yo le destruyo
}

m4return_t ClComponentStore_RB_map::RemoveAll (void)
{
    // Hacer el casting porque for_each devuelve
	// un puntero a una funcion ...
    (void)for_each (m_mapss.begin (), m_mapss.end (), ClComponentStore_RB_map::s_remove);
    
    m_mapss.clear ();
	UKProvider.Reset ();

    m_iCount = 0;

    return M4_SUCCESS;
}

//nos llaman cuando en el bloque padre se quieren borrar todos los registros:
//aquí lo que se hace es vaciar de un tirón las estructuras que mantienen
//los bloques hijos de este en el que estamos borrando todos los registros

m4return_t ClComponentStore_RB_map::ParentDestroyAllRegisters (m4uint32_t ai_parentblockuniquekey)
{
    //consigo el substore de mi bloque
    //el store de mi hijo RB es ClComponentStore_RB_map por narices
	_ClComponentSubStore_map *pss;
    pss = GetSS_NoCreate (ai_parentblockuniquekey);

	if (pss) {
/*
		//antes de llamar a pss->ParentDestroyAllRegisters :
		m_iCount -= pss->Count ();
		pss->ParentDestroyAllRegisters ();
*/
        ClBlock_Channel_Proxy * pb;
        m4uint16_t              i, n;
        m4uint32_t              j;
        ClNode_Data *           pchildnode;

        while (pss->Count ())
        {
            pb = pss->_getlast(j);
            n = m_pNode->GetpExtInfo ()->ChildNodeBB.Count ();

            for (i = 0; i < n; i ++)
            {
                pchildnode = m_pNode->GetpExtInfo ()->ChildNodeBB [(ClChildIndex)i].GetAddress ();
                pchildnode ->GetpBlockStore ()->ParentDestroyBlock (pb->ID.BlockUniqueKey);
            }
			pb->DestroyProxy (M4_FALSE);     //llama a delete de pchildblock
            pss->Remove (j);
            -- m_iCount;
        }
        pss->RemoveAll ();
	}
	
	return M4_SUCCESS;
}

m4return_t ClComponentStore_RB_map::ParentDestroyBlock (m4uint32_t ai_parentblockuniquekey)
{
	m4return_t res;

	//mismo caso en el que se destruyen todos los registros
	res = ParentDestroyAllRegisters (ai_parentblockuniquekey);

	//y además aquí irá código para eliminar la entrada ai_parentblockuniquekey de mi array primario
	//de bloques
	MAPSS::iterator it;

	it = m_mapss.find(ai_parentblockuniquekey);
	if (it != m_mapss.end ()) {
		_ClComponentSubStore_map *pss = (*it).second;
		M4_ASSERT (pss);
		M4_ASSERT (pss->Count () == 0);	//acabamos de vaciarlo, asi que no tiene que tener elementos

		delete pss;	//yo creo el ss y yo le destruyo

		m_mapss.erase (it);	//y le elimino del mapa
	}

	return res;
}

m4uint32_t ClComponentStore_RB_map::Count (void)
{
    return m_iCount;
}

void ClComponentStore_RB_map::UpdateIndex (m4uint32_t ai_idblock, m4uint32_t *ai_pindex_qtc, m4uint32_t ai_icount)
{
    _ClComponentSubStore_map *pss = GetSS_NoCreate (ai_idblock);
    //bueno, pues puede ser que no exista:
    if (pss) {
	    m4uint32_t c = pss->Count ();
        pss->UpdateIndex (ai_pindex_qtc, ai_icount);
	    m_iCount -= (c - pss->Count ());
    }
}

m4uint32_t ClComponentStore_RB_map::MemoryUsed (void)
{
    m4uint32_t isize = sizeof (ClComponentStore_RB_map) + m_mapss.size () * (sizeof (_ClComponentSubStore_map *) + sizeof (m4uint32_t) + sizeof (void *)*2);

	MAPSS::iterator it;
	it = m_mapss.begin ();
	while (it != m_mapss.end ()) {
		isize += (*it).second->MemoryUsed ();
		it++;
	}

    return isize;
}

m4return_t ClComponentStore_RB_map::Reserve(m4uint32_t ai_lSize)
{
	MAPSS::iterator it;
	it = m_mapss.begin ();
	while (it != m_mapss.end ()) {
		(*it).second->Reserve (ai_lSize);
		it++;
	}

    return M4_SUCCESS;
}


m4return_t ClComponentStore_RB_map::ForceLock( void )
{
	if( m_poMapssMutex == NULL )
	{
		m_poMapssMutex = new ClMutex( M4_TRUE );
	}
    return M4_SUCCESS;
}


m4return_t ClComponentStore_RB_map::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(ClComponentStore_RB_map);

		ao_size -= sizeof(_AClComponentStore);
	if (_AClComponentStore::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

		ao_size -= sizeof(UKProvider);
	if (UKProvider.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

// MAPSS
		ao_size -= sizeof(m_mapss);
	if (GetMapStructureSize(m_mapss,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	MAPSS::iterator			it;
	for (it = m_mapss.begin();it!=m_mapss.end();++it)
	{
		if ((*it).second){
			if ((*it).second->GetSize(ao_size)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
}

