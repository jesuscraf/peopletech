//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             px_block.inl
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             07/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//      Estas son las funciones que se llaman desde ClValue, para llamarlas con exito
//      previamente el ClRegister debería estar posicionado (NO EOF), para hacer esto
//      hay que haber insertado registros, lo que provoca que el proxy desaparezca,
//      dando paso al bloque real.
//      La conclusión es que todas estan funciones devuelven error, porque el ClRegister
//      desde el que se llama está necesariamente a EOF.
//      ... a no ser que nos refiramos a items de bloque, so listo.
//
//==============================================================================

#include "px_block.hpp"

#include "m4mdrt.hpp"
#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "arecset.hpp"

#include "ch_block.hpp"

//-----------------------------------------------------------------

//## end module.includes preserve=yes
//-----------------------------------------------------------------------------
// con el truco del almendruco ...
// si el item está conectado, nos vamos al padre a pedir el valor, retrasando un poco
// más la creación del bloque

m4return_t ClBlock_Channel_Proxy::B_GetPersist (m4bool_t &ai_persist)
{
    if (m_pClBlock == 0) {
        if (!_CreateBlock()) return M4_ERROR;
    }

    return m_pClBlock->B_GetPersist (ai_persist);
}

m4return_t ClBlock_Channel_Proxy::B_SetPersist (m4bool_t ai_persist)
{
    if (m_pClBlock == 0) {
        if (!_CreateBlock()) return M4_ERROR;
    }

    return m_pClBlock->B_SetPersist (ai_persist);
}

m4return_t ClBlock_Channel_Proxy::B_SetReLoad (m4bool_t ai_rl)
{
    if (m_pClBlock == 0) {
        if (!_CreateBlock()) return M4_ERROR;
    }

    return m_pClBlock->B_SetReLoad (ai_rl);
}
m4return_t ClBlock_Channel_Proxy::B_GetReLoad (m4bool_t &ao_rl)
{
    if (m_pClBlock == 0) {
        ao_rl = M4_FALSE;
		return M4_SUCCESS;
    }

    return m_pClBlock->B_GetReLoad (ao_rl);
}

//-----------------------------------------------------------------

//-----------------------------------------------------------------

m4return_t ClBlock_Channel_Proxy::R_GetVersion (ClMMIndex ai_index, VersionMark &ai_vm)
{
//## begin ClBlock_Channel_Proxy::R_GetVersion%2131716234.body preserve=yes
    if (m_pClBlock) {
        return m_pClBlock->R_GetVersion (ai_index, ai_vm);
    }

    return M4_ERROR;
//## end ClBlock_Channel_Proxy::R_GetVersion%2131716234.body
}

m4return_t ClBlock_Channel_Proxy::R_SetVersion (ClMMIndex ai_index, VersionMark &ai_vm)
{
//## begin ClBlock_Channel_Proxy::R_SetVersion%1363108261.body preserve=yes
    if (m_pClBlock) {
        return m_pClBlock->R_SetVersion (ai_index, ai_vm);
    }

    return M4_ERROR;
//## end ClBlock_Channel_Proxy::R_SetVersion%1363108261.body
}

m4return_t ClBlock_Channel_Proxy::R_GetRecordPersist (ClMMIndex ai_index, m4bool_t& ai_persist)
{
//## begin ClBlock_Channel_Proxy::R_GetRecordPersist%-2073683858.body preserve=yes
    if (m_pClBlock) {
        ai_persist = m_pClBlock->R_GetRecordPersist(ai_index);
	return M4_SUCCESS;
    }

    return M4_ERROR;
//## end ClBlock_Channel_Proxy::R_GetRecordPersist%-2073683858.body
}

m4return_t ClBlock_Channel_Proxy::R_SetRecordPersist (ClMMIndex ai_index, m4bool_t ai_persist)
{
//## begin ClBlock_Channel_Proxy::R_SetRecordPersist%-194248509.body preserve=yes
    if (m_pClBlock) {
        return m_pClBlock->R_SetRecordPersist (ai_index, ai_persist);
    }

    return M4_ERROR;
//## end ClBlock_Channel_Proxy::R_SetRecordPersist%-194248509.body
}

