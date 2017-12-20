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

#include "access.hpp"
#include "bookmark.hpp"
#include "clfufast.hpp"
#include "channel.hpp"
//#include "clglobal.hpp"
//#include "vmtypes.hpp"
#include "node_knl.hpp"

#include "px_block.hpp"
#include "blockref.hpp"
#include "m4objglb.hpp"
#include "dmtypes.hpp"
#include "m4mdrt.hpp"



ClFU_Fast::ClFU_Fast(ClChannel *ai_pchannel, ClNodeIndex ai_NodeIndex, ClItemIndex ai_Index, m4VariantType ai_var, m4uint32_t ai_iOperation/*=M4CL_CASE_EQUAL_FUNCTION*/)
{
    m_iNodeIndex = ai_NodeIndex;
    // Correccion, antes era una comparacion case insensitive
    m_iCompFunc = ai_iOperation - 1;

    Index = ai_Index;
    m_Var = ai_var;

    Init(ai_pchannel);
}

ClFU_Fast::ClFU_Fast(_ClFilterUtility * ai_pClFU)
{
    M4_ASSERT(ai_pClFU->Type() == _ClFilterUtility::Fast);
    // Correccion, antes era una comparacion case insensitive
    m_iCompFunc = ((ClFU_Fast *)ai_pClFU)->m_iCompFunc;

    m_iNodeIndex = ((ClFU_Fast *)ai_pClFU)->m_iNodeIndex;

    Index = ((ClFU_Fast *)ai_pClFU)->Index;
    m_Var = ((ClFU_Fast *)ai_pClFU)->m_Var;

    Init(0);
}

m4bool_t ClFU_Fast::IsVisible (ClMMIndex ai_MMIndex, const ClBlockReference &ai_br, ClAccessProvider *ai_pap)
{
    m4VariantType v;
    m4return_t res = 0;

    ClAccess *paccess = ai_pap->GetpAccess ();

    ClAccessBookMark oBookMark;
    res |= oBookMark.GetFrom (*paccess);

    if (ai_br->IsNihil ())
    {
        ClHandle NodeHandle = paccess->Node.Get(m_iNodeIndex)->GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr = paccess->GetpCMCR();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_FILTER_RAM_NIHIL, M4ObjNodeN(pmcr, NodeHandle));

        return M4_FALSE;
    }

    ClNode *paccessnode = paccess ->Node.Get (ai_br->GetpNode_Data ()->GetpExtInfo ()->Index);
	M4_ASSERT (paccessnode);

	res |= paccessnode->RecordSet.Follow (ai_br);

    paccessnode->RecordSet.Register.Index = 0;
    paccessnode->RecordSet.Register.MoveToMM(ai_MMIndex);
    paccessnode->RecordSet.Register.Item.MoveTo(Index);
    paccessnode->RecordSet.Register.Item.Value.Get(v);

    res |= oBookMark.ApplyTo (*paccess);

    if (res != M4_SUCCESS)
    {
        ClHandle NodeHandle = paccess->Node.Get(m_iNodeIndex)->GetpNode_Data()->GetpNodeDef()->GetHandle();
        ClCompiledMCR * pmcr = paccess->GetpCMCR();

        DUMP_CHLOG_ERRORF(M4_CH_DM_RAM_FILTER_FAILURE, M4ObjNodeN(pmcr, NodeHandle));

        return M4_FALSE;
    }

    m4int32_t ret = g_apfM4VariantCompareFunctions[m_iCompFunc](&v, &m_Var,0);

    return ret ? M4_TRUE : M4_FALSE;
}

m4return_t ClFU_Fast::Init (ClChannel *ai_pChannel)
{
    return M4_SUCCESS;
}

m4return_t ClFU_Fast::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClFU_Fast);
	ao_size-=sizeof(m_Var);
	
	return m_Var.GetSize(ao_size);
}
