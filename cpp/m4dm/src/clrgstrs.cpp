//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             clrgstrs.cpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             30/04/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class StaticFilter
//
//==============================================================================

#include "clrgstrs.h"
#include "mmmap.hpp"
#include "clfuvar.hpp"
#include "clfufast.hpp"
#include "px_block.hpp"
#include "channel.hpp"
#include "clbsearch.hpp"

#include "blockref.hpp"

#include "aregister.hpp"

#include "access.hpp"


// Class ClRegisterStaFilter 

//******************************************************************
//Constructores / Destructor
//******************************************************************


ClRegisterStaFilter::ClRegisterStaFilter(ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    _init ();

    m_pClFilterUtility = new ClFilterUtility(ai_pchannel, ai_nodeindex);
}


ClRegisterStaFilter::ClRegisterStaFilter(ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, m4int32_t ai_pHandleForm, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    _init ();

    m_pClFilterUtility = new ClFilterUtility(ai_pchannel, ai_nodeindex, ai_pHandleForm);
}


ClRegisterStaFilter::ClRegisterStaFilter (_ClFilterUtility * ai_pClFU, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    _init ();

    switch(ai_pClFU->Type())
    {
    case _ClFilterUtility::LN4:
        m_pClFilterUtility = new ClFilterUtility(ai_pClFU);
        break;
    case _ClFilterUtility::Operation:
        m_pClFilterUtility = new ClFiltUtilOp(ai_pClFU);
        break;
    case _ClFilterUtility::Sort:
        m_pClFilterUtility = new ClFiltSort(ai_pClFU);
        break;
    case _ClFilterUtility::Fast:
        m_pClFilterUtility = new ClFU_Fast(ai_pClFU);
        break;
    default:
        m_pClFilterUtility = new ClFU_Nihil;
    }
}

ClRegisterStaFilter::ClRegisterStaFilter (m4uint8_t ai_OpType, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    _init ();

    m_pClFilterUtility = new ClFiltUtilOp(ai_OpType);
}

ClRegisterStaFilter::ClRegisterStaFilter (_ClRegister_Base * ai_pClRegister, ClBSearchIndex * ai_pBSearchIndex, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    _init ();

    m_pClFilterUtility = new ClFiltSort(ai_pClRegister, ai_pBSearchIndex);
}

ClRegisterStaFilter::ClRegisterStaFilter (_ClRegister_Base * ai_pClRegister, m4int16_t ai_iNumberItems, m4pchar_t * ai_pItemsNames, m4uint8_t * ai_pOrders, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    _init ();

    m_pClFilterUtility = new ClFiltSort(ai_pClRegister, ai_iNumberItems, ai_pItemsNames, ai_pOrders);
}

ClRegisterStaFilter::ClRegisterStaFilter (ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, ClItemIndex ai_Index, m4VariantType ai_var, m4pchar_t ai_pName) : _ClRegisterFilter_Generic (ai_pName)
{
    _init ();

    m_pClFilterUtility = new ClFU_Fast(ai_pchannel, ai_nodeindex, ai_Index, ai_var);
}

ClRegisterStaFilter::ClRegisterStaFilter (ClChannel *ai_pchannel, ClNodeIndex ai_nodeindex, ClItemIndex ai_Index, m4VariantType ai_var, m4uint32_t ai_iOperation, m4pchar_t ai_pName ){
    _init ();

    m_pClFilterUtility = new ClFU_Fast(ai_pchannel, ai_nodeindex, ai_Index, ai_var, ai_iOperation);

}

void ClRegisterStaFilter::_init (void)
{
    m_pClCompMap = new ClComponentMap <ClMMArray>;

    M4_ASSERT (m_pClCompMap);
}

ClRegisterStaFilter::~ClRegisterStaFilter ()
{
    if (m_pClCompMap) {
        delete m_pClCompMap;
        m_pClCompMap = 0;
    }
}

//******************************************************************
//Funciones administrativas
//******************************************************************

_ClRegisterFilter *ClRegisterStaFilter::Copy (ClFilterInterface * ai_pClFI, m4bool_t ai_bTotal)
{
    _ClRegisterFilter * aux = _ClRegisterFilter_Generic::Copy (ai_pClFI, ai_bTotal);

    ClRegisterStaFilter *pClNewFilter = new ClRegisterStaFilter(m_pClFilterUtility, m_pName);

// Nueva llamada, copy los indices!!!!!
    if (ai_bTotal)
        pClNewFilter->m_pClCompMap->CopyMap(m_pClCompMap);

    pClNewFilter->AttachSubFilter(aux);
    aux->AttachFather(pClNewFilter);
    pClNewFilter->AttachInterface(ai_pClFI);
    return pClNewFilter;
}

//******************************************************************
//Funciones coumnes a todos los filtros.
//******************************************************************

m4return_t ClRegisterStaFilter::Next   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ClMMArray * p_mmarray = 0;
    m4return_t res;
    res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    if (!p_mmarray)
    {
        if (ai_br->IsNihil())
        {
            aio_MMIndex = ClMMIndex_EOF;
            return M4_SUCCESS;
        }

        UpdateFilter(ai_br, ai_pap);
        res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    }
    M4_ASSERT(p_mmarray);

    ClMMIndex AuxInd;
    m4uint32_t i, iCount;
	res = p_mmarray->Count(iCount);

    for (i = 0; i < iCount; i++)
    {
	    res = p_mmarray->Get(i, AuxInd);
        if (res == M4_SUCCESS)
        {
            if (AuxInd == aio_MMIndex)
                break;
        }
        else
            break;
    }

// Hago el Next:
    i++;
	res = p_mmarray->Get(i, AuxInd); //me da igual el res, si es error meto un EOF!!!
    aio_MMIndex = AuxInd;

	return res;
}

m4return_t ClRegisterStaFilter::Prev   (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ClMMArray * p_mmarray = 0;
    m4return_t res;
    res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    if (!p_mmarray)
    {
        if (ai_br->IsNihil())
        {
            aio_MMIndex = ClMMIndex_EOF;
            return M4_SUCCESS;
        }

        UpdateFilter(ai_br, ai_pap);
        res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    }

    M4_ASSERT(p_mmarray);

    ClMMIndex AuxInd;
    m4uint32_t i, iCount;
	res = p_mmarray->Count(iCount);

    for (i = 0; i < iCount; i++)
    {
	    res = p_mmarray->Get(i, AuxInd);
        if (res == M4_SUCCESS)
        {
            if (AuxInd == aio_MMIndex)
                break;
        }
        else
            break;
    }
//Hago el Previous:
    i--;
	res = p_mmarray->Get(i, AuxInd); //me da igual el res, si es error meto un EOF!!!
    aio_MMIndex = AuxInd;

	return res;
}

m4return_t ClRegisterStaFilter::Begin (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ClMMArray * p_mmarray = 0;
    m4return_t res;
    res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    if (!p_mmarray)
    {
        if (ai_br->IsNihil())
        {
            aio_MMIndex = ClMMIndex_EOF;
            return M4_SUCCESS;
        }

        UpdateFilter(ai_br, ai_pap);
        res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    }
    M4_ASSERT(p_mmarray);

	res = p_mmarray->Get(0, aio_MMIndex);

	return res; 
}

m4return_t ClRegisterStaFilter::End (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ClMMArray * p_mmarray = 0;
    m4return_t res;
    res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    if (!p_mmarray)
    {
        if (ai_br->IsNihil())
        {
            aio_MMIndex = ClMMIndex_EOF;
            return M4_SUCCESS;
        }

        UpdateFilter(ai_br, ai_pap);
        res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    }
    M4_ASSERT(p_mmarray);

	m4uint32_t index;
	res = p_mmarray->Count(index);

	index --;
	res = p_mmarray->Get(index, aio_MMIndex);

	return res; 
}

m4return_t ClRegisterStaFilter::MoveTo (ClRegisterIndex ai_newindex, ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ClMMArray * p_mmarray = 0;
    m4return_t res;
    res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    if (!p_mmarray)
    {
        if (ai_br->IsNihil())
        {
            aio_MMIndex = ClMMIndex_EOF;
            return M4_SUCCESS;
        }

        UpdateFilter(ai_br, ai_pap);
        res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    }
    M4_ASSERT(p_mmarray);

	m4uint32_t index = ai_newindex;
	res = p_mmarray->Get(index, aio_MMIndex);

	return res; 
}

m4return_t ClRegisterStaFilter::MoveToEOF (ClRegisterIndex &ai_Index, ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    //EDU aio_Index.SetEOF ();
    aio_MMIndex = ClMMIndex_EOF;

    return M4_SUCCESS;
}

m4return_t ClRegisterStaFilter::Count (m4uint32_t &ao_count, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    ClMMArray * p_mmarray = 0;
    m4return_t res;
    res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    if (!p_mmarray)
    {
        if ((ai_br->IsNihil()) ||
            ((!ai_br->IsLoaded()) &&
            (ai_pap->GetpAccess()->GetAutoLoadMode() == M4CL_AUTOLOAD_OFF)))
        {
            ao_count = 0;
            return M4_SUCCESS;
        }

        UpdateFilter(ai_br, ai_pap);
        res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
    }
    M4_ASSERT(p_mmarray);

	res = p_mmarray->Count(ao_count);

	return res; 
}

//lo mantenemos por compatibilidad
m4bool_t ClRegisterStaFilter::IsVisible(ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    return m_pClFilterUtility->IsVisible(ai_MMIndex, ai_br, ai_pap);
}

m4return_t ClRegisterStaFilter::InstallFormula (m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs)
{
    return m_pClFilterUtility->Prepare (ai_pForm, ai_pap, ai_iNumArgs, ai_pNameArgs, ai_pValueArgs);
}

//******************************************************************
//Funciones propias de los filtros estaticos.
//******************************************************************

m4return_t ClRegisterStaFilter::UpdateIndex (ClRegisterIndex &aio_Index, ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	if (ai_MMIndex == ClMMIndex_EOF) {
		aio_Index = M4DM_EOF_INDEX;
		return M4_ERROR;
	}

	ClMMArray * p_mmarray = 0;
	m4return_t res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
	if (!p_mmarray) {
		if (ai_br->IsNihil()) {
			aio_Index = M4DM_EOF_INDEX;
			return M4_ERROR;
		}
		
		UpdateFilter(ai_br, ai_pap);
		res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
	}
	M4_ASSERT(p_mmarray);
	
	m4uint32_t i = 0;
	ClMMIndex aux_MMIndex = 0;
	while (aux_MMIndex != ClMMIndex_EOF) {
		res = p_mmarray->Get(i, aux_MMIndex);
		if ((m4uint32_t)aux_MMIndex == (m4uint32_t)ai_MMIndex)
		{
			aio_Index = i;
			return M4_SUCCESS;
		}
		i ++;
	}
	aio_Index = M4DM_EOF_INDEX;
	return M4_ERROR;
}

m4return_t ClRegisterStaFilter::UpdateOneIndex(ClMMIndex &aio_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4uint32_t n;

    m_pSubFilter->UpdateOneIndex(aio_MMIndex, ai_br, ai_pap);

	if (aio_MMIndex != ClMMIndex_EOF)
	{
		ClMMArray * p_mmarray = 0;
		m4return_t res;
		res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
		if (!p_mmarray)
		{
            if (ai_br->IsNihil())
            {
                aio_MMIndex = ClMMIndex_EOF;
                return M4_ERROR;
            }

			UpdateFilter(ai_br, ai_pap);
			res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
		}
		M4_ASSERT(p_mmarray);

		m4uint32_t i = 0;
		ClMMIndex aux_MMIndex = 0;

		res = p_mmarray->Get(i, aux_MMIndex);
		while ((m4uint32_t)aux_MMIndex != (m4uint32_t)aio_MMIndex)
		{
			i++;
			res = p_mmarray->Get(i, aux_MMIndex);
			if (res != M4_SUCCESS)
				return M4_ERROR;
		}

        if (m_pClFilterUtility->Type() != _ClFilterUtility::Sort)
        {
		    if (aux_MMIndex == aio_MMIndex)
		    {
			    if (!m_pClFilterUtility->IsVisible(aio_MMIndex, ai_br, ai_pap))
			    {
				    res = p_mmarray->Remove (i, aux_MMIndex);
			    }
		    }
	        return M4_SUCCESS;
	    }
        else
        {
            res = p_mmarray->Count(n);
            if (n > 1)
            {
                if (p_mmarray->GetNewElements() == 1)
                {
                    m4uint32_t indexnext;
                    m4int32_t count = m_pClFilterUtility->GetSearchIndex()->Count();
                    m4int32_t it;
                    m4VariantType * vV = new m4VariantType[count];
                    for (it = 0; it < count; it++)
                    {
                        ClHandle h = m_pClFilterUtility->GetSearchIndex()->Get(it)->GetItemHandle();
                        m_pClFilter->GetpRegister()->Item[h].Value.Get(vV[it]);
                    }
                    m_pClFilter->GetpRegister()->MoveTo(ClRegisterIndex(0));
                    m_pClFilter->Find(vV, 0, &indexnext);
                    indexnext++;
			        res = p_mmarray->Remove(i, aio_MMIndex);
			        res = p_mmarray->Add(indexnext, aio_MMIndex);

			        //Con el consentimiento de Pablo. 
			        //Ojo, lo pone como ordenado a pelo aunque podría no estarlo!!
			        m_pClFilterUtility->SetSorted();
			        p_mmarray->ResetNewElements();

			        return M4_SUCCESS;
                }
            }
            else
            {
                return M4_SUCCESS;
            }
        }
    }
	return M4_ERROR;
}

m4return_t ClRegisterStaFilter::UpdateThisFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	ClMMArray * p_mmarray = 0;
	m4return_t res;
	res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);

	if (!p_mmarray)
	{
        if (ai_br->IsNihil())
        {
            return M4_SUCCESS;
        }

		res = m_pClCompMap->Add(ai_br->ID.BlockUniqueKey, p_mmarray);
	}

    m4uint32_t n;
    m_pSubFilter->Count(n, ai_br, ai_pap);
	if (n)
	{
		res = p_mmarray->Init(n, ai_br, ai_pap, m_pSubFilter);
		return m_pClFilterUtility->CreateArray(ai_br, m_pSubFilter, p_mmarray, ai_pap);
	}
	else
		return M4_SUCCESS;
}

m4return_t ClRegisterStaFilter::UpdateFilter(const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (ai_br->IsNihil())
        return M4_SUCCESS;

    m_pSubFilter->UpdateFilter(ai_br, ai_pap);

    return UpdateThisFilter(ai_br, ai_pap);
}

void ClRegisterStaFilter::Notify_DestroyRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
	if (ai_mmindex != ClMMIndex_EOF)
	{
		//se lo paso al siguiente por si le interesa:
		M4_ASSERT (m_pSubFilter);
		m_pSubFilter->Notify_DestroyRegister (ai_mmindex, ai_br, ai_pap);

		//Lo que hay que hacer es leer el array del Store, restar uno al count y decremetra
		//los registros >= que ai_mmindex

		ClMMArray * p_mmarray = 0;
		m4return_t res;
		res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
		if (!p_mmarray)
		{
            if (ai_br->IsNihil())
                return;
			UpdateFilter(ai_br, ai_pap);
			res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
		}
        else
        {
		    m4uint32_t i = 0;
		    ClMMIndex aux_MMIndex = 0;

		    res = p_mmarray->Get(i, aux_MMIndex);
		    while ((m4uint32_t)aux_MMIndex != (m4uint32_t)ai_mmindex)
		    {
			    i++;
			    res = p_mmarray->Get(i, aux_MMIndex);
			    if (res != M4_SUCCESS)
				    break;
		    }
            //estoy posicionado en el primero no valido o igual al MMindex que me pasan
            //Borro si lo encuentro, si no, solo decremento los indices
            if (aux_MMIndex == ai_mmindex) {
	            res = p_mmarray->Remove(i, ai_mmindex);
            }

            res = p_mmarray->Notify_DeleteRegister(ai_mmindex);
        }
	}
}

void ClRegisterStaFilter::Notify_InsertRegister (ClMMIndex ai_mmindex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (m_pClFilterUtility->Type() == _ClFilterUtility::Sort)
        m_pClFilterUtility->SetNotSorted();

	if (ai_mmindex != ClMMIndex_EOF)
	{
		//se lo paso al siguiente por si le interesa:
		M4_ASSERT (m_pSubFilter);
		m_pSubFilter->Notify_InsertRegister (ai_mmindex, ai_br, ai_pap);

		//Lo que hay que hacer es leer el array del Store, restar uno al count y decremetra
		//los registros >= que ai_mmindex

		ClMMArray * p_mmarray = 0;
		m4return_t res;
		res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
		if (!p_mmarray)
		{
            if (ai_br->IsNihil())
                return;
			UpdateFilter(ai_br, ai_pap);
			res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);
		}
        else
        {
		    m4uint32_t i = 0;
		    ClMMIndex aux_MMIndex = 0;

		    res = p_mmarray->Get(i, aux_MMIndex);
            if (m_pClFilterUtility->Type() != _ClFilterUtility::Sort)
            {
		        while ((m4uint32_t)aux_MMIndex < (m4uint32_t)ai_mmindex)
		        {
			        i++;
			        res = p_mmarray->Get(i, aux_MMIndex);
			        if (res != M4_SUCCESS)
				        break;
		        }
            }
            else
                res = p_mmarray->Count(i);
            res = p_mmarray->Notify_AddRegister(ai_mmindex);
            res = p_mmarray->Add(i, ai_mmindex);
        }
	}
}

void ClRegisterStaFilter::Notify_DestroyBlock_After (const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (m_pClFilterUtility->Type() == _ClFilterUtility::Sort)
        m_pClFilterUtility->SetNotSorted();

	M4_ASSERT (m_pSubFilter);
	m_pSubFilter->Notify_DestroyBlock_After (ai_br, ai_pap);

	ClMMArray * p_mmarray = 0;
	m4return_t res;
	res = m_pClCompMap->Get(ai_br->ID.BlockUniqueKey, p_mmarray);

    if (p_mmarray)
		res = m_pClCompMap->Remove(ai_br->ID.BlockUniqueKey);

    return;
}

void ClRegisterStaFilter::Notify_ReleaseAll_Before (ClAccessProvider *ai_pap)
{
    if (m_pClFilterUtility->Type() == _ClFilterUtility::Sort)
        m_pClFilterUtility->SetNotSorted();

	M4_ASSERT (m_pSubFilter);
	m_pSubFilter->Notify_ReleaseAll_Before (ai_pap);

	m4return_t res = m_pClCompMap->RemoveAll();

    return;
}

void ClRegisterStaFilter::AttachRegister(_ClRegister_Base * ai_pReg)
{
    _ClRegisterFilter_Generic::AttachRegister(ai_pReg);

    if (m_pClFilterUtility->Type() == _ClFilterUtility::Sort)
        m_pClFilterUtility->AttachRegister(ai_pReg);
}



m4return_t 
ClRegisterStaFilter::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClRegisterStaFilter);
		ao_size-=sizeof(_ClRegisterFilter_Generic);
	
	if (_ClRegisterFilter_Generic::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (m_pClCompMap){
		if (m_pClCompMap->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}

// Additional Declarations
  //## begin ClRegisterStaFilter.declarations preserve=yes
  //## end ClRegisterStaFilter.declarations



//## begin module.epilog preserve=yes
//## end module.epilog

