#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             reg_filt.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             04/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Simple filter code
//
//
//==============================================================================

#include "reg_filt.hpp"

#include "clfuvar.hpp"
#include "clfufast.hpp"

//#include "arecset.hpp"
//#include "ch_node.hpp"
#include "channel.hpp"
#include "iexecutor.hpp"

#include "px_block.hpp"

#include "blockref.hpp"


ClRegisterDynFilter::ClRegisterDynFilter (ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    m_pClFilterUtility = new ClFilterUtility(ai_pchannel, ai_inodeindex);
}

ClRegisterDynFilter::ClRegisterDynFilter (ClChannel *ai_pchannel, ClNodeIndex ai_inodeindex, m4int32_t ai_pHandleForm, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    m_pClFilterUtility = new ClFilterUtility(ai_pchannel, ai_inodeindex, ai_pHandleForm);
}

ClRegisterDynFilter::ClRegisterDynFilter (_ClFilterUtility * ai_pClFU, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    switch(ai_pClFU->Type())
    {
    case _ClFilterUtility::LN4:
        m_pClFilterUtility = new ClFilterUtility(ai_pClFU);
        break;
    case _ClFilterUtility::Operation:
        m_pClFilterUtility = new ClFiltUtilOp(ai_pClFU);
        break;
    case _ClFilterUtility::Fast:
        m_pClFilterUtility = new ClFU_Fast(ai_pClFU);
        break;
    default:
        m_pClFilterUtility = new ClFU_Nihil;
    }
}

ClRegisterDynFilter::ClRegisterDynFilter (m4uint8_t ai_OpType, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    m_pClFilterUtility = new ClFiltUtilOp(ai_OpType);
}

ClRegisterDynFilter::ClRegisterDynFilter (ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, ClItemIndex ai_Index, m4VariantType ai_var, m4pchar_t ai_pName)  : _ClRegisterFilter_Generic (ai_pName)
{
    m_pClFilterUtility = new ClFU_Fast(ai_pchannel, ai_nodeindex, ai_Index, ai_var);
}

ClRegisterDynFilter::ClRegisterDynFilter (ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, ClItemIndex ai_Index, m4VariantType ai_var,m4uint32_t ai_iOperation, m4pchar_t ai_pName)  : _ClRegisterFilter_Generic (ai_pName)
{
    m_pClFilterUtility = new ClFU_Fast(ai_pchannel, ai_nodeindex, ai_Index, ai_var, ai_iOperation);
}

m4return_t ClRegisterDynFilter::Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	m_pSubFilter->Next(ai_Index, aio_MMIndex, ai_br, ai_pap);
	return FindFirstVisible(ai_Index, aio_MMIndex, ai_br, ai_pap);
}

m4return_t ClRegisterDynFilter::Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	m_pSubFilter->Prev(ai_Index, aio_MMIndex, ai_br, ai_pap);
	return FindLastVisible(ai_Index, aio_MMIndex, ai_br, ai_pap);
}

m4return_t ClRegisterDynFilter::Begin (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	m_pSubFilter->Begin(ai_Index, aio_MMIndex, ai_br, ai_pap);
	return FindFirstVisible(ai_Index, aio_MMIndex, ai_br, ai_pap);
}

m4return_t ClRegisterDynFilter::End (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	m_pSubFilter->End(ai_Index, aio_MMIndex, ai_br, ai_pap);

	return FindLastVisible(ai_Index, aio_MMIndex, ai_br, ai_pap);
}

m4return_t ClRegisterDynFilter::MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	if (ai_newindex == M4DM_EOF_INDEX) {
		return MoveToEOF(ai_Index, aio_MMIndex, ai_br, ai_pap);
	}
	
	ClRegisterIndex aux_index = 0;
	m4return_t res = Begin(ai_Index, aio_MMIndex, ai_br, ai_pap);
	if (res != M4_SUCCESS)
	{
		aio_MMIndex = ClMMIndex_EOF;
		aux_index = M4DM_EOF_INDEX;
		return M4_ERROR;
	}
	
	while (aux_index != ai_newindex)
	{
		res = Next(ai_Index, aio_MMIndex, ai_br, ai_pap);
		if (res != M4_SUCCESS)
		{
			aio_MMIndex = ClMMIndex_EOF;
			aux_index = M4DM_EOF_INDEX;
			return M4_ERROR;
		}
		aux_index++;
	}
	if (aio_MMIndex == ClMMIndex_EOF)
	{
		aux_index = M4DM_EOF_INDEX;
	}
	
	return M4_SUCCESS;
}

m4return_t ClRegisterDynFilter::MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    aio_MMIndex = ClMMIndex_EOF;

    return M4_SUCCESS;
}

m4return_t ClRegisterDynFilter::Count (m4uint32_t &ao_count, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4uint32_t n = 0;
	m4return_t res;
	ClRegisterIndex aux_Index;
    ClMMIndex aux_MMIndex(0);

//	ActualIndex = Index;//Me guardo el valor del indice para volver a ponerlo al final

    res = Begin(aux_Index, aux_MMIndex, ai_br, ai_pap);
	if (res != M4_SUCCESS)
    {
        return M4_ERROR;
    }

	while (aux_MMIndex != ClMMIndex_EOF)
	{
		res = Next(aux_Index, aux_MMIndex, ai_br, ai_pap);
	    if (res != M4_SUCCESS)
        {
            n = 0;
            return M4_ERROR;
        }

		n++;
	}
	ao_count = n;

    return M4_SUCCESS;
}

//provisional para pruebas:

m4return_t ClRegisterDynFilter::FindLastVisible(ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	while ((aio_MMIndex != ClMMIndex_EOF)&&(aio_MMIndex >= 0))
	{
		if (IsVisible(aio_MMIndex, ai_br, ai_pap))
			break;
		m_pSubFilter->Prev(ai_Index, aio_MMIndex, ai_br, ai_pap);
	}

    return M4_SUCCESS;
}

m4return_t ClRegisterDynFilter::FindFirstVisible(ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	while (aio_MMIndex != ClMMIndex_EOF)
	{
		if (IsVisible(aio_MMIndex, ai_br, ai_pap))
			break;
		m_pSubFilter->Next(ai_Index, aio_MMIndex, ai_br, ai_pap);
	}

    return M4_SUCCESS;
}

m4bool_t ClRegisterDynFilter::IsVisible(ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    return m_pClFilterUtility->IsVisible(ai_MMIndex, ai_br, ai_pap);
}

m4return_t ClRegisterDynFilter::InstallFormula (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs)
{
    return m_pClFilterUtility->Prepare (ai_pForm, ai_pap, ai_iNumArgs, ai_pNameArgs, ai_pValueArgs);
}

//Dado un MMIndex calculo su Index correspondiente. No es recursivo!!!
m4return_t ClRegisterDynFilter::UpdateIndex (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	if (ai_MMIndex != ClMMIndex_EOF)
	{
		ClMMIndex aux_mmindex(0);
		m4return_t res = Begin (aio_Index, aux_mmindex, ai_br, ai_pap);
		if (res != M4_SUCCESS)
		{
			aio_Index = M4DM_EOF_INDEX;
			return M4_ERROR;
		}
		
		aio_Index = 0;
		
		while (((m4uint32_t)aux_mmindex) != ((m4uint32_t)ai_MMIndex)&&(aux_mmindex != ClMMIndex_EOF))
		{
			res = Next(aio_Index, aux_mmindex, ai_br, ai_pap);
			if (res != M4_SUCCESS)
			{
				aio_Index = M4DM_EOF_INDEX;
				return M4_ERROR;
			}
			
			aio_Index ++;
		}
		if (aux_mmindex == ClMMIndex_EOF)
		{
			aio_Index = M4DM_EOF_INDEX;
		}
	} else {
		aio_Index = M4DM_EOF_INDEX;
	}
	
	return M4_SUCCESS;
}

_ClRegisterFilter * ClRegisterDynFilter::Copy (ClFilterInterface * ai_pClFI, m4bool_t ai_bTotal)
{
	_ClRegisterFilter * aux = _ClRegisterFilter_Generic::Copy (ai_pClFI);

    ClRegisterDynFilter *pClNewFilter = new ClRegisterDynFilter(m_pClFilterUtility, m_pName);

    pClNewFilter->AttachSubFilter(aux);
    aux->AttachFather(pClNewFilter);
    pClNewFilter->AttachInterface(ai_pClFI);
    return pClNewFilter;
}

m4return_t ClRegisterDynFilter::UpdateFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	return m_pSubFilter->UpdateFilter(ai_br, ai_pap);
}
m4return_t 
ClRegisterDynFilter::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClRegisterDynFilter);
		ao_size-=sizeof(_ClRegisterFilter_Generic);
		ao_size-=sizeof(m_vHandleFormula);
		ao_size-=sizeof(m_vFormula);

	if (_ClRegisterFilter_Generic::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_vHandleFormula.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (m_vFormula.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


