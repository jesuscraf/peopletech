#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             clfuvar.cpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             5/05/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    class FilterUtilityVariableArguments
//
//==============================================================================

#include "clfuvar.hpp"
#include "access.hpp"
#include "node_knl.hpp"
#include "channel.hpp"
#include "iexecutor.hpp"
#include "bookmark.hpp"

#include "regi_smp.hpp"
#include "mmarray.hpp"

#include "smarthan.hpp"

#include "px_block.hpp"
#include "blockref.hpp"

#include "clbsearch.hpp"

#include "chan_knl.hpp"
#include "dm_def.hpp"
#include "m4mdrt.hpp"


ClArgumentList_Nihil _ClFilterUtility::m_oArgumentNihil;

_ClFilterUtility::~_ClFilterUtility(void)
{
}

m4return_t _ClFilterUtility::CreateArray(const ClBlockReference &ai_br, _ClRegisterFilter * ai_pClSubFilter, ClMMArray * ai_pmmarray, ClAccessProvider *ai_pap)
{
	ClRegisterIndex aux_Index = 0;
	ClMMIndex aux_MMIndex = 0;
    m4uint32_t index = 0;
    m4return_t res;

	res = ai_pmmarray->RemoveAll();

    res = ai_pClSubFilter->Begin(aux_Index, aux_MMIndex, ai_br, ai_pap);
	while (aux_MMIndex != ClMMIndex_EOF)
	{
		if (IsVisible(aux_MMIndex, ai_br, ai_pap))
        {
			res = ai_pmmarray->Add(index, aux_MMIndex);
			index ++;
        }
        res = ai_pClSubFilter->Next(aux_Index, aux_MMIndex, ai_br, ai_pap);
        aux_Index ++;
    }
    ai_pmmarray->ResetNewElements();

    return M4_SUCCESS;
}

ClFilterUtility::ClFilterUtility(ClChannel *ai_pchannel, ClNodeIndex ai_NodeIndex)
{
    m_iNodeIndex = ai_NodeIndex;

    m_pClHandleForm = new ClSmartHandle(ai_pchannel);

    Init(ai_pchannel);
}

ClFilterUtility::ClFilterUtility(ClChannel *ai_pchannel, ClNodeIndex ai_NodeIndex, m4int32_t ai_iHandleForm)
{
    m_iNodeIndex = ai_NodeIndex;

    m_pClHandleForm = new ClSmartHandle(ai_iHandleForm, ai_pchannel);

    Init(ai_pchannel);
}

ClFilterUtility::ClFilterUtility(_ClFilterUtility * ai_pClFU)
{
    M4_ASSERT(ai_pClFU->Type() == _ClFilterUtility::LN4);

    m_iNodeIndex = ((ClFilterUtility *)ai_pClFU)->m_iNodeIndex;

    m_ClArguments = ai_pClFU->Arguments();

    m_pClHandleForm = ((ClFilterUtility *)ai_pClFU)->m_pClHandleForm->Attach();

    Init(0);
}

ClFilterUtility::~ClFilterUtility(void)
{
    m_pClHandleForm->Detach();
}

m4bool_t ClFilterUtility::IsVisible(ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{

	m4return_t res = 0;
	m4double_t dSliceMode = 0;
	m4VariantType v, vv, vHandle;


    ClAccess *paccess = ai_pap->GetpAccess ();

    if (ai_br->IsNihil ())
    {
        ClHandle NodeHandle = paccess->Node.Get(m_iNodeIndex)->GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr = paccess->GetpCMCR();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_FILTER_RAM_NIHIL, M4ObjNodeN(pmcr, NodeHandle));
        return M4_FALSE;
    }

    ClAccessBookMark oBookMark;
    res |= oBookMark.GetFrom (*paccess);

    //provisional: hago el follow:
    ClAccessRecordSet *pars = &paccess ->Node.Get (m_iNodeIndex)->RecordSet;
    pars->Follow (ai_br);

	ClNode *pn = paccess ->Node.Get (ai_br->GetpNode_Data ()->GetpExtInfo ()->Index);
	res |= pn->RecordSet.Follow (ai_br);

    m_pClHandleForm->GetHandle(vHandle);
    res |= paccess ->GetpExecutor()->Stack.Push (vHandle);

    m4int32_t iNumArgs = m_ClArguments.Count();

    if (iNumArgs)
    {
        m4int32_t i;
        for (i = 0; i < iNumArgs; i ++)
        {
            m_ClArguments[i].Get(vv);
            res |= paccess ->GetpExecutor()->Stack.Push (vv);
        }
    }

// Tengo creado un access sin filtros, por tanto se puede hacer un casting de MMIndex 
// a Index. Lo necesito hacer para saber mediante mi metodo LN4 si este registro es
// es visible.

    pars->Current.Index = 0;
    pars->Current.MoveToMM(ai_MMIndex);

	// Bug 0310446 El JIT se debe ejecutar en modo no tramos
	paccess->RunSliceMode.Get( dSliceMode );

	if( dSliceMode != 0.0 )
	{
		paccess->RunSliceMode.Set( 0.0 );
	}

    res |= pars->Current.Item["ExecuteJIT"].Call (0, (1 + iNumArgs));

	if( dSliceMode != 0.0 )
	{
		paccess->RunSliceMode.Set( dSliceMode );
	}

    res |= oBookMark.ApplyTo (*paccess);
    res |= paccess ->GetpExecutor()->Stack.Pop (v);

    if (res != M4_SUCCESS)
    {
        ClHandle NodeHandle = paccess->Node.Get(m_iNodeIndex)->GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr = paccess->GetpCMCR();

        DUMP_CHLOG_ERRORF(M4_CH_DM_RAM_FILTER_FAILURE, M4ObjNodeN(pmcr, NodeHandle));
        return M4_FALSE;
    }

    if (v.GetType() == M4CL_CPP_TYPE_NUMBER)
    {
        if (v.Data.DoubleData)
		{
		    return M4_TRUE;
		}
    }
    return M4_FALSE;
}

m4return_t ClFilterUtility::Init (ClChannel *ai_pchannel)
{
    return M4_SUCCESS;
}

m4return_t ClFilterUtility::Prepare(m4pchar_t ai_pForm, ClAccessProvider *ai_pap, m4uint32_t ai_iNumArgs, m4pchar_t * ai_pNameArgs, m4VariantType * ai_pValueArgs)
{
	m4return_t		res = 0;
    m4uint32_t		i;
    m4VariantType	v;
    m4VariantType	vv;
    m4VariantType	vFormula;
    m4VariantType	vHandle;


	/* Bug 0124762
	Primero hay que recuperar todas las cadenas por si se redimensiona el pool
	Después ya se pueden añadir los argumentos y ejecutar el método sin peligro
	*/

    vFormula.Type = M4CL_CPP_TYPE_STRING_VAR;
    vFormula.SetString(ai_pForm);    

    if (ai_iNumArgs)
    {
        vv.Type = M4CL_CPP_TYPE_STRING_VAR;

        m_ClArguments.CreateList(ai_iNumArgs);

        for (i = 0; i < ai_iNumArgs; i ++)
        {
            if(ai_pNameArgs!= NULL && ai_pNameArgs[i] != NULL)
			{
                m_ClArguments[i].SetName(ai_pNameArgs[i]);
			}

			if( ai_pValueArgs != NULL )
			{
				m_ClArguments[i].Set(ai_pValueArgs[i]);
			}
        }
    }


    ClAccess *paccess = ai_pap->GetpAccess ();
        
    res |= paccess ->GetpExecutor()->Stack.Push (vFormula);
    res |= paccess ->GetpExecutor()->Stack.Push (v);

    if (ai_iNumArgs)
    {
        for (i = 0; i < ai_iNumArgs; i ++)
        {
            m_ClArguments[i].GetName(vv.Data.PointerChar);
            res |= paccess ->GetpExecutor()->Stack.Push (vv);
        }
    }


    ClAccessRecordSet *pars = &paccess ->Node.Get (m_iNodeIndex)->RecordSet;

    res |= pars->Item["PrepareJIT"].Call(0, (ai_iNumArgs + 2));

	res |= paccess ->GetpExecutor()->Stack.Pop (vHandle);
	res |= paccess ->GetpExecutor()->Stack.Pop (v);

    if (res != M4_SUCCESS)
    {
        ClCompiledMCR * pmcr = paccess->GetpCMCR ();
		m4uint32_t NodeHandle = paccess->Node.Get(m_iNodeIndex)->GetpNode_Data()->GetpNodeDef()->GetHandle();
        DUMP_CHLOG_ERRORF(M4_CH_DM_RAM_FILTER_CREATION, M4ObjNodeN(pmcr, NodeHandle));
        return M4_ERROR;
    }

    if (v.Data.DoubleData == M4_SUCCESS)
    {
        m_pClHandleForm->SetHandle(vHandle);
		return M4_SUCCESS;
    }

    return M4_ERROR;
}

ClChannel * ClFilterUtility::GetpChannel (void)
{
    return m_pClHandleForm->m_pClChannel;
}


ClFiltUtilOp::ClFiltUtilOp(m4uint8_t ai_OpType)
{
    m_OpType = ai_OpType;
}

ClFiltUtilOp::ClFiltUtilOp(_ClFilterUtility * ai_pClFU)
{
    M4_ASSERT(ai_pClFU->Type() == Operation);

    m_OpType = ((ClFiltUtilOp *)ai_pClFU)->m_OpType;
}


m4bool_t ClFiltUtilOp::IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    if (ai_br->IsNihil ())
    {
        ClAccess *paccess = ai_pap->GetpAccess ();
        ClCompiledMCR * pmcr  = paccess->GetpCMCR ();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_FILTER_RAM_NIHIL2, M4Obj(pmcr));

        return M4_FALSE;
    }

    switch (m_OpType)
    {
        case M4DM_OP_TYPE_MODIFY:   return ai_br->R_IsModified (ai_MMIndex);
                                    break;
        case M4DM_OP_TYPE_NEW:      return ai_br->R_IsNew      (ai_MMIndex);
                                    break;
        case M4DM_OP_TYPE_UPDATE:   return ai_br->R_IsUpdated  (ai_MMIndex);
                                    break;
        case M4DM_OP_TYPE_DELETE:   return ai_br->R_IsDeleted  (ai_MMIndex);
                                    break;
        case M4DM_OP_TYPE_NOTMODIFY:return M4_BOOL( !(ai_br->R_IsModified (ai_MMIndex)) );
                                    break;
        case M4DM_OP_TYPE_NOTNEW:   return M4_BOOL( !(ai_br->R_IsNew      (ai_MMIndex)) );
                                    break;
        case M4DM_OP_TYPE_NOTUPDATE:return M4_BOOL( !(ai_br->R_IsUpdated  (ai_MMIndex)) );
                                    break;
        case M4DM_OP_TYPE_NOTDELETE:return M4_BOOL( !(ai_br->R_IsDeleted  (ai_MMIndex)) );
                                    break;
        default:                    return M4_FALSE;
    }
}

ClFiltSort::ClFiltSort(_ClRegister_Base * ai_pClRegister, ClBSearchIndex * ai_pBSearchIndex)
{
    m_pClRegister   = ai_pClRegister;
    m_pBSearchIndex = new ClSmartSearchIndex (ai_pBSearchIndex);
    m_bSorted = M4_FALSE;
}

ClFiltSort::ClFiltSort(_ClRegister_Base * ai_pClRegister, m4int16_t ai_iNumberItems, m4pchar_t * ai_pItemsNames, m4uint8_t * ai_pOrders)
{
    m_bSorted = M4_FALSE;
    m_pClRegister   = ai_pClRegister;
    m_pBSearchIndex = new ClSmartSearchIndex ();
    ClBSearchIndex * aux_ClBSI = new ClBSearchIndex;
    m4return_t res = aux_ClBSI->Init(ai_pClRegister, ai_iNumberItems, ai_pItemsNames, ai_pOrders);
    if (res != M4_SUCCESS)
    {
        // Ya no vuelco error, lo hago a mas bajo nivel (clbsearch.cpp)
        delete aux_ClBSI;
        aux_ClBSI = 0;
    }
    else
        m_pBSearchIndex->SetIndex(aux_ClBSI);
}

ClFiltSort::ClFiltSort(_ClFilterUtility * ai_pClFU)
{
    M4_ASSERT(ai_pClFU->Type() == _ClFilterUtility::Sort);

//    m_pClRegister = ((ClFiltSort *)ai_pClFU)->m_pClRegister;
    m_pClRegister = 0;
    m_pBSearchIndex = ((ClFiltSort *)ai_pClFU)->m_pBSearchIndex->Attach();
    m_bSorted = M4_FALSE;
}

ClBSearchIndex * ClFiltSort::GetSearchIndex(void)
{
    return m_pBSearchIndex->GetIndex();
}
ClFiltSort::~ClFiltSort(void)
{
    m_pBSearchIndex->Detach();
}

m4return_t ClFilterUtility::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClFilterUtility);
	ao_size-=sizeof(m_ClArguments);
	
	// !!!!!!!!!OJO. Aqui puedo estar contando mas de una vez lo mismo.
	// La opcion era contarlo o no contarlo, pero como es mas normal que no haya copias a que si las haya,
	// lo cuento.
	// 
	if (m_pClHandleForm) {
		if (m_pClHandleForm->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}

	return m_ClArguments.GetSize(ao_size);
}

m4return_t 
ClFiltUtilOp::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClFiltUtilOp);

	return M4_SUCCESS;
}

m4return_t 
ClFiltSort::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClFiltSort);
    if (m_pBSearchIndex){
	    if (m_pBSearchIndex->GetSize(ao_size)!=M4_SUCCESS){
		    return M4_ERROR;
	    }
    }

	return M4_SUCCESS;
}
m4return_t 
ClFU_Nihil::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClFU_Nihil);

	return M4_SUCCESS;
}


