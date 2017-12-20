//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             regi_smp.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             12/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//
//
//==============================================================================

#include "rblck_it.hpp"

#include "px_block.hpp"
#include "compst_common.hpp"
#include "compstit_common.hpp"
#include "aregister.hpp"
#include "bnihil.hpp"   //para el g_oNihilBlock

#include "blockref.hpp"

ClRegisterFilter_BlockWalker::ClRegisterFilter_BlockWalker (
    _AClComponentStore *ai_pComponentStore,
    _ClRegister_Base *ai_pClRegister) : _ClRegisterFilter_Generic ()
{
    //como se ve este 'filter' tiene estado:
    m_pComponentStore = ai_pComponentStore;
    M4_ASSERT (m_pComponentStore);

    m_pComponentStore_ForwardIt = m_pComponentStore->CreateIterator ();
    M4_ASSERT (m_pComponentStore_ForwardIt);
    
    m_pClRegister = ai_pClRegister;
    M4_ASSERT (m_pClRegister);    
}

ClRegisterFilter_BlockWalker::~ClRegisterFilter_BlockWalker ()
{
    M4_ASSERT (m_pComponentStore);
    M4_ASSERT (m_pComponentStore_ForwardIt);

    m_pComponentStore->DestroyIterator (m_pComponentStore_ForwardIt);
}

m4return_t ClRegisterFilter_BlockWalker::Next (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4return_t res = M4_SUCCESS;

    //si estoy en eof paso de todo
    if (aio_MMIndex != ClMMIndex_EOF) {

        //no modificar aio_Index, se hace en el register

        res = m_pSubFilter->Next (ai_Index, aio_MMIndex, ai_br, ai_pap);

        if (aio_MMIndex == ClMMIndex_EOF) {
            aio_MMIndex = _GetNextValid (ai_pap);
        }
    }

    //M4_ASSERT (0);

    return res;
}

m4return_t ClRegisterFilter_BlockWalker::Prev (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4return_t res = M4_SUCCESS;

    //si estoy en eof paso de todo
    if (aio_MMIndex != ClMMIndex_EOF) {

        //no modificar aio_Index, se hace en el register

        res = m_pSubFilter->Prev (ai_Index, aio_MMIndex, ai_br, ai_pap);

        if (aio_MMIndex == ClMMIndex_EOF) {
            aio_MMIndex = _GetPrevValid (ai_pap);
        }
    }

    return res;
}

m4return_t ClRegisterFilter_BlockWalker::Begin (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //no modificar aio_Index, se hace en el register

    m4return_t res = M4_SUCCESS;

    //me voy al primer bloque
    //ClBlock_Channel_Proxy *pblock;
	ClBlockReference br;

    m_pComponentStore_ForwardIt->Begin (br);
    if (br) {
        res = m_pSubFilter->Begin (ai_Index, aio_MMIndex, br, ai_pap);

        if (aio_MMIndex == ClMMIndex_EOF) {
            aio_MMIndex = _GetNextValid (ai_pap);   //aqui se actualiza el bloque en el registro
        }
        else {
            m_pClRegister->Item.SetCurrentBlock (br);
                //de esta forma no tiene efectos laterales (poner el Index a eof)
        }
    }
    else {
        //como que no hay bloques, a eof directos:
        aio_MMIndex = ClMMIndex_EOF;
        m_pClRegister->Item.SetCurrentBlock (&g_oNihilBlock);
            //de esta forma no tiene efectos laterales (poner el Index a eof)
    }

    return res;
}

m4return_t ClRegisterFilter_BlockWalker::End (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //no modificar aio_Index, se hace en el register

    m4return_t res = M4_SUCCESS;

    //me voy al primer bloque
    //ClBlock_Channel_Proxy *pblock;
	ClBlockReference br;

    m_pComponentStore_ForwardIt->End (br);
    if (br) {
        res = m_pSubFilter->End (ai_Index, aio_MMIndex, br, ai_pap);

        if (aio_MMIndex == ClMMIndex_EOF) {
            aio_MMIndex = _GetPrevValid (ai_pap);   //aqui se actualiza el bloque en el registro
        }
        else {
            m_pClRegister->Item.SetCurrentBlock (br);
                //de esta forma no tiene efectos laterales (poner el Index a eof)
        }
    }
    else {
        //como que no hay bloques, a eof directos:
        aio_MMIndex = ClMMIndex_EOF;
        m_pClRegister->Item.SetCurrentBlock (&g_oNihilBlock);
            //de esta forma no tiene efectos laterales (poner el Index a eof)
    }

    return res;
}

m4return_t ClRegisterFilter_BlockWalker::MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //a mano:
    Begin (ai_newindex, aio_MMIndex, ai_br, ai_pap);

    if (aio_MMIndex != ClMMIndex_EOF) {
        while (ai_newindex) {
            
            //al loro, pq durante la ejecución se puede haber modificado 
            //el bloque activo en el registro
            const ClBlockReference &br = m_pClRegister->GetCurrentBlockRef ();

            Next (ai_newindex, aio_MMIndex, br, ai_pap);
            if (aio_MMIndex == ClMMIndex_EOF) {
                //no podemos ir al registro que nos han pedido
                break;
            }
            ai_newindex --;
        }
    }
    
    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_BlockWalker::MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    aio_MMIndex = ClMMIndex_EOF;

    return M4_SUCCESS;
}

m4return_t ClRegisterFilter_BlockWalker::Count (m4uint32_t &ao_count, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ao_count = 0;

    //me creo un nuevo fi, para que el interno no cambie su estado:
    _AClComponentStore_ForwardIt *pfi = m_pComponentStore->CreateIterator ();

    M4_ASSERT (pfi);

    //ClBlock_Channel_Proxy *pblock;
	ClBlockReference br;
	
	pfi->Begin (br);
    while (br) {
        //comprobamos si es válido, para que el Count no dispare la creación del bloque:
        if (br->IsValid ()) {            
            ao_count += br->Count ();
        }

        pfi->Next (br);
    }

    m_pComponentStore->DestroyIterator (pfi);

    return M4_SUCCESS;
}


ClMMIndex ClRegisterFilter_BlockWalker::_GetNextValid (ClAccessProvider *ai_pap)
{
    //ClBlock_Channel_Proxy *pblock;
	ClBlockReference br;

    ClMMIndex mmindex (ClMMIndex_EOF);
    ClRegisterIndex index;

    for (;;) {
        m_pComponentStore_ForwardIt->Next (br);

        if (br) {
            m_pSubFilter->Begin (index, mmindex, br, ai_pap);
            if (mmindex != ClMMIndex_EOF) {
                //terminamos
                //M4_ASSERT (pblock);
                //m_pClRegister->SetCurrentBlock (pblock);
                    //esta llamada pone el Index a EOF, cuidado !!

                //comprobación:
                M4_ASSERT (m_pClRegister->GetpNode_Data () == br->GetpNode_Data ());
                m_pClRegister->Item.SetCurrentBlock (br);
                    //de esta forma no tiene efectos laterales (poner el Index a eof)
                break;
            }
        }
        else {
            //no quedan más bloques, a eof
            M4_ASSERT (mmindex == ClMMIndex_EOF);
            m_pClRegister->Item.SetCurrentBlock (&g_oNihilBlock);
                //de esta forma no tiene efectos laterales (poner el Index a eof)
            break;
        }
    }

    return mmindex;
}

ClMMIndex ClRegisterFilter_BlockWalker::_GetPrevValid (ClAccessProvider *ai_pap)
{
    //ClBlock_Channel_Proxy *pblock;
	ClBlockReference br;

    ClMMIndex mmindex;
    ClRegisterIndex index;

    for (;;) {
        m_pComponentStore_ForwardIt->Prev (br);

        if (br) {
            m_pSubFilter->End (index, mmindex, br, ai_pap);
            if (mmindex != ClMMIndex_EOF) {
                //terminamos
                //M4_ASSERT (pblock);
                //m_pClRegister->SetCurrentBlock (pblock);
                    //esta llamada pone el Index a EOF, cuidado !!

                //comprobación:
                M4_ASSERT (m_pClRegister->GetpNode_Data () == br->GetpNode_Data ());
                m_pClRegister->Item.SetCurrentBlock (br);
                    //de esta forma no tiene efectos laterales (poner el Index a eof)
                break;
            }
        }
        else {
            //no quedan más bloques, a eof
            M4_ASSERT (mmindex == ClMMIndex_EOF);
            m_pClRegister->Item.SetCurrentBlock (&g_oNihilBlock);
                //de esta forma no tiene efectos laterales (poner el Index a eof)
            break;
        }
    }

    return mmindex;
}

_ClRegisterFilter * ClRegisterFilter_BlockWalker::Copy (ClFilterInterface * ai_pClFI)
{
	_ClRegisterFilter * aux = _ClRegisterFilter_Generic::Copy (ai_pClFI);

    ClRegisterFilter_BlockWalker *pClNewFilter = new ClRegisterFilter_BlockWalker (m_pComponentStore, m_pClRegister);

    pClNewFilter->AttachSubFilter(aux);
    return pClNewFilter;
}
m4return_t 
ClRegisterFilter_BlockWalker::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClRegisterFilter_BlockWalker);
		ao_size-=sizeof(_ClRegisterFilter_Generic);
	
	if (_ClRegisterFilter_Generic::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	if (m_pComponentStore_ForwardIt){
		if (m_pComponentStore_ForwardIt->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}


