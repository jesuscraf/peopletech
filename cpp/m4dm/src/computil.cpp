//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             computil.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             23/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Utilities over class ComponentStore
//
//
//==============================================================================


#include "computil.hpp"

#include "compst_common.hpp"
#include "compstit_common.hpp"
#include "blockref.hpp"

#include "px_block.hpp"
#include "recorset.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"
#include "nodedef.hpp"

m4return_t ClCStoreUtilities::ForEach (_AClComponentStore *ai_pcstore, pf_t ai_pf, void *ai_parg)
{
    M4_ASSERT (ai_pcstore);
    M4_ASSERT (ai_pf);

    m4return_t res = M4_SUCCESS;
    _AClComponentStore_ForwardIt *pfi;

    pfi = ai_pcstore->CreateIterator ();
    M4_ASSERT (pfi);

    ClBlockReference blockref;
	m4bool_t isnoteof = pfi->Begin (blockref);

    while (isnoteof) {
        res = (*ai_pf) (blockref, ai_parg);
        if (res == M4_ERROR)
            break;

        isnoteof = pfi->Next (blockref);
    }
    
    ai_pcstore->DestroyIterator (pfi);

    return res;
}

//------------------------------------------------------------------------------
//Delete
//------------------------------------------------------------------------------

// no se usa, pero bueno, por si acaso:
m4return_t ClCStoreUtilities::DeleteOneSafe (ClBlockReference &ai_blockref, void *)
{
	M4_ASSERT (!ai_blockref.IsNull ());    
    //secuencia correcta para la destrucción !
        ai_blockref->DestroySafe ();
        ai_blockref.Invalide ();

    return M4_SUCCESS;
}


m4return_t ClCStoreUtilities::DeleteOneFast (ClBlockReference &ai_blockref, void *)
{
	M4_ASSERT (!ai_blockref.IsNull ());
    //OJO !
    //secuencia correcta para la destrucción !
        ai_blockref->DestroyFast ();
        ai_blockref.Invalide ();
        
    return M4_SUCCESS;
}

//------------------------------------------------------------------------------
//SetCheckPointMode
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::SetCheckPointModeOne (ClBlockReference &ai_blockref, void *ai_pbnewmode)
{
	M4_ASSERT (!ai_blockref.IsNull ());
    return ai_blockref->SetCheckPointMode (*(m4bool_t *)ai_pbnewmode);
}

//------------------------------------------------------------------------------
//CheckPoint
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::CheckPointOne (ClBlockReference &ai_blockref, void *)
{
	M4_ASSERT (!ai_blockref.IsNull ());
    return ai_blockref->CheckPoint ();
}

m4return_t ClCStoreUtilities::UndoOne (ClBlockReference &ai_blockref, void *)
{
	M4_ASSERT (!ai_blockref.IsNull ());
    return ai_blockref->B_Undo ();
}

//------------------------------------------------------------------------------
//SetKeepStatus
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::SetKeepStatusOne (ClBlockReference &ai_blockref, void *ai_pKeepStatusInfo)
{
	M4_ASSERT (!ai_blockref.IsNull ());
	ClKeepStatusInfo* pKeepStatusInfo = (ClKeepStatusInfo*)ai_pKeepStatusInfo;
    return ai_blockref->SetKeepStatus(pKeepStatusInfo->m_bKeepIt, pKeepStatusInfo->m_iVectorCount);
}

//------------------------------------------------------------------------------
//ResetSerialMarks
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::ResetSerialMarks (ClBlockReference &ai_blockref, void *)
{
	M4_ASSERT (!ai_blockref.IsNull ());
    ai_blockref->ResetSerialMarks();
    return M4_SUCCESS;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
m4return_t ClCStoreUtilities::FindRecordAndSetVersion(ClBlockReference& ai_blockref, 
                                                      void*             ai_oChangeRecVerInf)
{
	M4_ASSERT (!ai_blockref.IsNull ());

    ClChangeRecordVersionInfo* oRVI = (ClChangeRecordVersionInfo*)ai_oChangeRecVerInf;
    ai_blockref->FindRecordAndSetVersion(oRVI->m_iRecordId, oRVI->m_dNewVersion, oRVI->m_bContinue);

    // Hacemos un truquito para parar el ForEach (se para cuando hay error)
    if (oRVI->m_bContinue == M4_FALSE)
        return M4_ERROR;

    return M4_SUCCESS;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
m4return_t ClCStoreUtilities::MarkBlockAndParentRecordsForSerialize(ClBlockReference& ai_blockref,
                                                                    void*             ai_bBranchMode)
{
    M4_ASSERT (!ai_blockref.IsNull ());
    ai_blockref->MarkBlockAndParentRecordsForSerialize(*(m4bool_t *)ai_bBranchMode,
                                                       M4_PXB_SEND_ALL_RECORDS);
    return M4_SUCCESS;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::CommitPersistedOne (ClBlockReference &ai_blockref, void *ai_pbKeepIt)
{
	M4_ASSERT (!ai_blockref.IsNull ());
    return ai_blockref->CommitPersisted (*(VersionMark *)ai_pbKeepIt);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::RollbackPersistedOne (ClBlockReference &ai_blockref, void *ai_pbKeepIt)
{
	M4_ASSERT (!ai_blockref.IsNull ());
    return ai_blockref->RollbackPersisted ();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::ResetOne (ClBlockReference &ai_blockref, void *)
{
	M4_ASSERT (!ai_blockref.IsNull ());
    return ai_blockref->ResetProxy ();
}
m4return_t ClCStoreUtilities::ReLoadOne (ClBlockReference &ai_blockref, void *ai_pRS)
{
	m4bool_t bRl ;
	ClAccessRecordSet *ai_prs = (ClAccessRecordSet *)ai_pRS;

	ai_blockref->B_GetReLoad(bRl);
	if (bRl)
	{
		if (ai_prs->Follow(ai_blockref)!=M4_SUCCESS){
			return M4_ERROR;
		}
		if (ai_prs->Register.DestroyAll()!=M4_SUCCESS){
			return M4_ERROR;
		}
		
		m4uint8_t	ReLoadMode = ai_prs->GetpAccess()->GetpCMCR()->GetNodeDBReload(ai_prs->GetpNodeDef()->GetHandle());
		
		ai_prs->SetFlagReLoad (M4_FALSE); // 
		
		switch (ReLoadMode){
		case M4CL_NODE_DB_RELOAD_LOAD_BLK:
			// No utilizo el bloque que me pasan por si hubiera cambiado en el Follow. Se lo pido al RS.
			ai_prs->GetCurrentBlock()->_SetIsLoaded (M4_TRUE);
			return ai_prs->Load_Block();
		case M4CL_NODE_DB_RELOAD_LOAD_PRG:
			// No utilizo el bloque que me pasan por si hubiera cambiado en el Follow. Se lo pido al RS.
			ai_prs->GetCurrentBlock()->_SetIsLoaded (M4_TRUE);
			return ai_prs->Load_Prg();
		case M4CL_NODE_DB_RELOAD_NONE:
		default:
			return M4_SUCCESS;
		}
	}

	return M4_SUCCESS;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::CheckModify (ClBlockReference &ai_blockref, void *ao_pvbModified)
{
	M4_ASSERT (!ai_blockref.IsNull ());
// No porque el recorrido de bloques no para
//	*(m4bool_t *)ao_pvbModified = M4_FALSE ;
    return ai_blockref->CheckModify (*(m4bool_t *)ao_pvbModified);
}


//------------------------------------------------------------------------------
// Resfreco de marcas de bloque, llamada a Carlos
//------------------------------------------------------------------------------

m4return_t ClCStoreUtilities::RefreshFlags (ClBlockReference &ai_blockref, void *ao_piOperation)
{
    return ai_blockref->RefreshFlags (*(m4uint8_t *)ao_piOperation);
//    return M4_SUCCESS;
}

