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
#include "compss_vector.hpp"
#include "blockref.hpp"
#include "node_knl.hpp"
#include "compst_map.hpp"
#include "chan_knl.hpp"
#include "m4mdrt.hpp"
#include "syncro.hpp"


///////////////////////////////////////////////////////////////////////////////
//
// ClComponentSubStore
//

//static 
const m4uint32_t _ClComponentSubStore_map::m_cPercentageDownSize = 10; //(10 por ciento)
const m4uint32_t _ClComponentSubStore_map::m_cPoolMinIncrement = 8;   //de 8 en 8

_ClComponentSubStore_map::_ClComponentSubStore_map (ClNode_Data *ai_pnode)
:
	m_pPool (&m_auxspace),
	m_iSize (1),
	m_iLast (0),
	m_iCount (0),
	m_auxspace (0)
{
	if( ai_pnode->GetpNodeDef()->GetpCMCR()->GetChannelIsCacheable() == 1 )
	{
		m_poPoolMutex = new ClMutex( M4_TRUE );
	}
	else
	{
		m_poPoolMutex = NULL;
	}
}    

_ClComponentSubStore_map::~_ClComponentSubStore_map ()
{
    RemoveAll ();

	if( m_poPoolMutex != NULL )
	{
		delete m_poPoolMutex ;
		m_poPoolMutex = NULL ;
	}
}


ClBlock_Channel_Proxy *_ClComponentSubStore_map::_Get_Readers (m4uint32_t ai_idreg, ClNode_Data *ai_pnode)
{
    ClBlock_Channel_Proxy *pb;

	if( m_poPoolMutex != NULL )
	{
		m_poPoolMutex->Lock();
	}

	pb = _vector_find (ai_idreg);

	if( m_poPoolMutex != NULL )
	{
		m_poPoolMutex->Unlock();
	}

    return pb;
}

ClBlock_Channel_Proxy *_ClComponentSubStore_map::_Get_Writer (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClNode_Data *ai_pnode, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid)
{
    ClBlock_Channel_Proxy *pb;

	if( m_poPoolMutex != NULL )
	{
		m_poPoolMutex->Lock();
	}

	pb = _vector_find (ai_idreg);

	if (!pb) {
		//ahora, si no existe, la liamos --> creamos el bloque:
		ClBlockReference component;
		ClBlock_Channel_Proxy::GetNewProxy (ai_pnode, ai_idblock, ai_idreg, ai_parentblockid, ai_parentregisterid, component);
		pb = component.GetpProxy ();

		m4uint32_t blockuniquekey;
		((ClComponentStore_RB_map *)ai_pnode->GetpBlockStore ())->NewElement (blockuniquekey);
		pb->ID.BlockUniqueKey = blockuniquekey;
		_vector_insert (ai_idreg, pb);

		M4_ASSERT (m_pPool [ai_idreg] == pb);
	}

	if( m_poPoolMutex != NULL )
	{
		m_poPoolMutex->Unlock();
	}

    return pb;
}


//no crea el bloque si no existe:
m4return_t _ClComponentSubStore_map::Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClNode_Data *ai_pnode, ClBlockReference &ao_component)
{
    m4return_t res;    

    ClBlock_Channel_Proxy *pb = _Get_Readers (ai_idreg, ai_pnode);

    if (pb) {
        ao_component.Set (pb);
        res = M4_SUCCESS;
    }
    else {
        ao_component.Reset ();
        res = M4_ERROR;
    }

	return res;
}


//crea el bloque si no existe:
m4return_t _ClComponentSubStore_map::Get (m4uint32_t ai_idblock, m4uint32_t ai_idreg, ClNode_Data *ai_pnode, m4uint32_t ai_parentblockid, m4uint32_t ai_parentregisterid, ClBlockReference &ao_component)
{
    ClBlock_Channel_Proxy *pb = _Get_Writer (ai_idblock, ai_idreg, ai_pnode, ai_parentblockid, ai_parentregisterid);
    M4_ASSERT (pb);

    ao_component.Set (pb);

	return M4_SUCCESS;
}


m4return_t _ClComponentSubStore_map::Add (m4uint32_t ai_idreg, const ClBlockReference &ai_br)
{
    _vector_insert (ai_idreg, ai_br.GetpProxy ());
	return M4_SUCCESS;
}

//devuelve la uk del bloque que elimina del mapa
m4uint32_t _ClComponentSubStore_map::Remove (m4uint32_t ai_idreg)
{
    ClBlock_Channel_Proxy *pb;
    pb = _vector_erase (ai_idreg);

    M4_ASSERT (pb);

	return pb->ID.BlockUniqueKey;
}

//actualiza indices:
	//'cambia' las claves del ai_idreg (NO incluido) al último: las incrementa en uno
m4return_t _ClComponentSubStore_map::Notify_ParentAddRegister (m4uint32_t ai_idreg)
{
    m4uint32_t i;
    ClBlock_Channel_Proxy *pbb;

    //añadimos una nueva entrada al final:
    if (m_iCount) {
	    //hacemos la inserción

        if (ai_idreg <= m_iLast) {
            m4uint32_t xxx = m_iLast + 1;
            M4_ASSERT (xxx);

            if (xxx >= m_iSize) {
                //no hay sitio -> a ampliar !!!
                _vector_newpool (xxx + m_cPoolMinIncrement);
            }

            //m_iLast ahora vale uno más que antes:
            //cuidado también que i es unsigned:
            for (i = m_iLast; (i >= ai_idreg) && (i <= m_iLast); --i) {
                pbb = m_pPool [i];
                if (pbb) {
                    M4_ASSERT (i == pbb->ID.RegisterId);
                    _SetRegisterId (pbb, i + 1);
                }
                m_pPool [i + 1] = pbb;
            }

            m_iLast = xxx;

            //ponemos a cero la nueva que ha quedado en medio:
            m_pPool [ai_idreg] = 0;
        }
    }

    return M4_SUCCESS;
}

//actualiza indices:
	//'cambia' las claves del ai_idreg (NO incluido) al último: las decrementa en uno
m4return_t _ClComponentSubStore_map::Notify_ParentRemoveRegister (m4uint32_t ai_idreg)
{
    if (m_iCount) {
        //pudiera estar vacio:
        m4uint32_t i;
        ClBlock_Channel_Proxy *pbb;        
        
        if (ai_idreg <= m_iLast) {
            if (m_pPool [ai_idreg]) {
                m_iCount --;    //se va a borrar un elemento válido
            }

            for (i = ai_idreg + 1; i <= m_iLast; ++i) {
                pbb = m_pPool [i];
                if (pbb) {
                    M4_ASSERT (i == pbb->ID.RegisterId);
                    _SetRegisterId (pbb, i - 1);
                }
                m_pPool [i - 1] = pbb;
            }

            //eliminamos la última entrada:
            m_pPool [m_iLast] = 0;
            m_iLast--;
        }
    }

    return M4_SUCCESS;
}

m4return_t _ClComponentSubStore_map::RemoveAll (void)
{
    _vector_deleteall ();
    m_auxspace = 0;
    m_iLast = 0;
    m_iCount = 0;

    return M4_SUCCESS;
}

m4uint32_t _ClComponentSubStore_map::Count (void)
{
    return m_iCount;
}

void _ClComponentSubStore_map::UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_icount)
{
	M4_ASSERT (ai_pindex_qtc);

    //M4_ASSERT (m_mapblocks.size () == ai_icount);
		//cuando hagamos el cambio a 'bajo demanda', el mapa puede tener menos elementos
	M4_ASSERT (Count () <= ai_icount);    

	if (m_iCount) {
		//algoritmo cutroso:    
		if (ai_icount) {
		    //me hago una copia del vector: // que pasada!
		    ClBlock_Channel_Proxy **pauxpool = new ClBlock_Channel_Proxy *[m_iLast + 1];
		    memcpy (pauxpool, m_pPool, sizeof (ClBlock_Channel_Proxy *) * (m_iLast + 1));

            //m_iLast puede cambiar de valor durante el bucle, nos guardamos
            //su valor original para no perderle:
            m4uint32_t soygilipollas = m_iLast;

			for (m4uint32_t i = 0; i < ai_icount; i++) {
				if (ai_pindex_qtc [i] != i) {
					_UpdateOneIndex (ai_pindex_qtc [i], i, pauxpool, soygilipollas);
				}
			}

            //coño! el delete!
            delete []pauxpool;
		}
	}
}

//el registro que estaba en la posicione ai_iold pasa a la posición ai_inew
//ai_iold: donde estaba antes
//ai_inew: donde queremos que vaya

void _ClComponentSubStore_map::_UpdateOneIndex (m4uint32_t ai_iold, m4uint32_t ai_inew, ClBlock_Channel_Proxy **ai_ppool /* = 0 */, m4uint32_t ai_poollenght /* = 0*/)
{
	ClBlock_Channel_Proxy *pbp;
	if (ai_iold <= ai_poollenght) {
		if (ai_ppool) {
			pbp = ai_ppool [ai_iold];
		}
		else {
			pbp = m_pPool [ai_iold];
		}
	}
	else {
		pbp = 0;
	}

	if (pbp) {
		//MAPBLOCKS::iterator it;
		//it = m_mapblocks.find (ai_inew);
		//if (m_mapblocks.end () == it) {
        if (0 == _vector_find (ai_inew)) {
			//no está el destino, lo insertamos:
			//m_mapblocks.insert (MAPBLOCKS::value_type (ai_inew, pbp));
            _vector_insert (ai_inew, pbp);

			//hay que conservar el número de elementos en el mapa
			//-->hay que eliminar la entrada old            
			//it = m_mapblocks.find (ai_iold);
			//if (m_mapblocks.end () != it)
            if (_vector_find (ai_iold) == pbp) {
			    //m_mapblocks.erase (it);	//y le elimino del mapa
                _vector_erase (ai_iold);
            }
		}
		else {
			//si está
			//(*it).second = pbp;
            M4_ASSERT (ai_inew < m_iSize);
            m_pPool [ai_inew] = pbp;
		}

		_SetRegisterId (pbp, ai_inew);
	}
    else
    {
		//MAPBLOCKS::iterator it;
		//it = m_mapblocks.find (ai_inew);
        pbp = _vector_find (ai_inew);

        //le elimino del mapa destino, para no quedarme referencias chungas.
        //ya que en el origen no estaba.
        //if (m_mapblocks.end () != it) {
        if (pbp) {
		    //m_mapblocks.erase (it);
            _vector_erase (ai_inew);
			//--m_iCount; del SubStore_RB_Map, mi papa
            //esta actualización la hace mi papa
		}
	}
}

//static
void _ClComponentSubStore_map::_SetRegisterId (ClBlock_Channel_Proxy *ai_pbp, m4uint32_t ai_inew)
{
    ai_pbp->ID.RegisterId = ai_inew;

    ClNode_Data *pnode = ai_pbp->GetpNode_Data ();
    ClNode_Data *pchildnode;    

    m4uint32_t buk = ai_pbp->ID.BlockUniqueKey;

    m4uint16_t nchildren = pnode->GetpExtInfo ()->ChildNodeRB.Count ();

    //como a este bloque le estamos cambiando el ID del registro, luego hay que actualizar
    //en sus hijos el ID de su padre:    
    for (m4uint16_t j = 0; j < nchildren; j++) {
        pchildnode = pnode->GetpExtInfo ()->ChildNodeRB [(ClChildIndex)j].GetAddress ();
        M4_ASSERT (pchildnode->GetpBlockStore ());

        _ClComponentSubStore_map *pss = ( (ClComponentStore_RB_map *) pchildnode->GetpBlockStore () )->GetSS_NoCreate (buk);

        //es posible que pss sea 0 si en el hijo todavía no hemos insertado ningún registro
        if (pss) {
			pss->_changeindex (ai_inew);
        }
    }
}

void _ClComponentSubStore_map::_changeindex (m4uint32_t ai_inew)
{
	if (m_iCount) {
		m4uint32_t i = 0;
		ClBlock_Channel_Proxy *pb;

		while (i <= m_iLast) {
			pb = m_pPool [i];
			if (pb) {
				pb->ID.Parent_RegisterIndex = ai_inew;
			}
			++i;
		}
	}
}

m4uint32_t _ClComponentSubStore_map::MemoryUsed (void)
{
	//más o menos:
    return sizeof (_ClComponentSubStore_map) + m_iSize * sizeof (ClBlock_Channel_Proxy *);
}

m4return_t _ClComponentSubStore_map::Reserve(m4uint32_t ai_lSize)
{
    _vector_newpool (ai_lSize);

    return M4_SUCCESS;
}

void _ClComponentSubStore_map::ParentDestroyAllRegisters (void)
{
	if (m_iCount) {
		m4uint32_t i = 0;
		ClBlock_Channel_Proxy *pb;

		while (i <= m_iLast) {
			pb = m_pPool [i];
			if (pb) {
				pb->DestroyProxy (M4_FALSE);     //llama a delete de pchildblock
			}
			++i;
		}

		//ahora vaciamos el substore de golpe, más optimo
		RemoveAll ();
	}
}

//-----------------------------------------------------------------------------


ClBlock_Channel_Proxy *_ClComponentSubStore_map::_vector_find (m4uint32_t ai_id)
{
    ClBlock_Channel_Proxy *pb;

    if ((ai_id <= m_iLast) && m_iCount) {
        pb = m_pPool [ai_id];
    }
    else {
        pb = 0;
    }

    return pb;
}

void _ClComponentSubStore_map::_vector_insert (m4uint32_t ai_idreg, ClBlock_Channel_Proxy *ai_pb)
{
	//hacemos la inserción
    if (ai_idreg >= m_iSize) {
        //no hay sitio -> a ampliar !!!
        _vector_newpool (ai_idreg + m_cPoolMinIncrement);
    }

    M4_ASSERT (ai_idreg < m_iSize);
    M4_ASSERT (0 == m_pPool [ai_idreg]);

    m_pPool [ai_idreg] = ai_pb;
    m_iCount++;
    if (ai_idreg > m_iLast) {
        m_iLast = ai_idreg;
    }

	//comprobación esquizofrénica:
    M4_ASSERT (ai_idreg < m_iSize);
    M4_ASSERT (m_pPool [ai_idreg] == ai_pb);
}

//devuelve el elemento que elimina del mapa
ClBlock_Channel_Proxy *_ClComponentSubStore_map::_vector_erase (m4uint32_t ai_idreg)
{
	//debe existir:
	M4_ASSERT (m_iCount);
    M4_ASSERT (ai_idreg <= m_iLast);
    ClBlock_Channel_Proxy *pb = m_pPool [ai_idreg];
    M4_ASSERT (pb);

    m_pPool [ai_idreg] = 0;
    --m_iCount;

    if (m_iCount) {
        //busco el nuevo 'last'
        M4_ASSERT (ai_idreg <= m_iLast);
        if (ai_idreg == m_iLast) {        
            m4uint32_t i;
            for (i = m_iLast - 1; i < m_iLast /*i es unsigned*/; --i) {
                if (m_pPool [i]) {
                    m_iLast = i;
                    break;
                }
            }
        }

        if (0 == m_iLast) {
            M4_ASSERT (1 == m_iCount);
            //utilizamos el array molón de un elemento:
            if (m_pPool != &m_auxspace) {
                m_auxspace = m_pPool [0];
                _vector_deleteall ();
                M4_ASSERT (1 == m_iSize);
            }
        }
        else {
            //vemos si tenemos 'muchos' elementos vacios:            
            //cuando el porcentage libre esta por encima del fijado (~10%)
            //se tira el pool actual y se coge uno más pequeño
            M4_ASSERT (m_iSize >= m_iLast);
            if ( (m_iSize - m_iLast) > (m_cPoolMinIncrement * 2) ) {
                if ( (m_iSize - m_iLast) * 100 > m_iSize * m_cPercentageDownSize ) {
                    //bajamos a un array inferior
                    _vector_newpool (m_iLast + m_cPoolMinIncrement);
                }
            }
        }
    }
    else {
        //array vacio:
        _vector_deleteall ();
        m_auxspace = 0;
        m_iLast = 0;
    }

	return pb;
}

void _ClComponentSubStore_map::_vector_deleteall (void)
{
    if (m_pPool != &m_auxspace) {
        M4_ASSERT (m_pPool);
        delete m_pPool;

        m_pPool = &m_auxspace;
        m_iSize = 1;
    }
    else {
        M4_ASSERT (1 == m_iSize);
        M4_ASSERT (0 == m_iLast);
        M4_ASSERT (m_iCount <= 1);
    }    
}

void _ClComponentSubStore_map::_vector_newpool (m4uint32_t ai_inewsize)
{
    M4_ASSERT (ai_inewsize > 1);

    ClBlock_Channel_Proxy **pauxpool = new ClBlock_Channel_Proxy *[ai_inewsize];

    memcpy (pauxpool, m_pPool, (m_iLast + 1) * sizeof (ClBlock_Channel_Proxy *));
    //rellenamos con ceros los nuevos huecos:
    memset (pauxpool + m_iLast + 1, 0, (ai_inewsize - m_iLast - 1) * sizeof (ClBlock_Channel_Proxy *));
    
    _vector_deleteall ();
    m_auxspace = 0;
    m_pPool = pauxpool;
    m_iSize = ai_inewsize;
}

ClBlock_Channel_Proxy *_ClComponentSubStore_map::_getnextvalid (m4uint32_t &aio_current)
{
	if (m_iCount) {
		ClBlock_Channel_Proxy *pb;
		while (aio_current <= m_iLast) {
			pb = m_pPool [aio_current];
			if (pb)
				return pb;

			++aio_current;
		}
	}

    aio_current = SS_MAP_FI_EOF;
    return 0;
}

ClBlock_Channel_Proxy *_ClComponentSubStore_map::_getprevvalid (m4uint32_t &aio_current)
{
	if (m_iCount) {
		ClBlock_Channel_Proxy *pb;

		//ojo que son unsigned:
		while (aio_current <= m_iLast) {
			pb = m_pPool [aio_current];
			if (pb)
				return pb;

			--aio_current;
		}
	}

    aio_current = SS_MAP_FI_EOF;
    return 0;
}

ClBlock_Channel_Proxy *_ClComponentSubStore_map::_getlast (m4uint32_t &ao_current)
{
    if (m_iCount) {
        ao_current = m_iLast;
        M4_ASSERT (m_pPool [m_iLast]);
        return m_pPool [m_iLast];
    }
    else {
        ao_current = SS_MAP_FI_EOF;
        return 0;
    }
}

m4return_t _ClComponentSubStore_map::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_ClComponentSubStore_map);

	// Aqui contamos los punteros a proxys y el tamaño de los proxys que hay en el nodo.
	// En Proxyfactory se descuentan estos tamaños.
	ao_size+=m_iSize*sizeof(ClBlock_Channel_Proxy *);

	if (m_pPool)
	{
		for (m4uint32_t i=0;i<=m_iLast;++i){
			if (m_pPool[i]){
				if (m_pPool[i]->GetSize(ao_size)!=M4_SUCCESS){
					return M4_ERROR;
				}
			}
		}

	}

	return M4_SUCCESS;
}
