#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             bnihil.cpp
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
//    This module defines...
//
//
//==============================================================================

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
//----------------------------------------------------------------------------

#include "bnihil.hpp"
#include "chlog.hpp"

#include "blockref.hpp"
#include "file_misc.hpp"
#include "clres.hpp"
#include "rm.hpp"


///////////////////////////////////////////////////////////////////////////////
//
// ClBlock_Nihil
//

// todo casca:

ClBlock_Nihil::ClBlock_Nihil ()
{
}

ClBlock_Nihil::~ClBlock_Nihil ()
{
}

ClBlock_Channel_Proxy *ClBlock_Nihil::GetChildBlock (m4uint16_t ai_pos)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return 0;
}

size_t ClBlock_Nihil::Count (void)
{
    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK_COUNT);
    return 0;
}

m4return_t ClBlock_Nihil::DeleteAll (void)
{
    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK_DELETE);

    return M4_ERROR;
}

m4return_t ClBlock_Nihil::CheckPoint (void)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::SetCheckPointMode (m4bool_t ai_operation)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::GetCheckPointMode (m4bool_t &ai_operation)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::CommitPersisted (VersionMark &ai_version)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::RollbackPersisted (void)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::B_AddRegister (ClMMIndex &ao_newindex)
{
    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK_ADD);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::R_InsertRegister (ClMMIndex ai_regindex)
{
    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK_ADD);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::R_DeleteRegister (ClMMIndex ai_regindex)
{
    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK_DELETE);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::R_Destroy (ClMMIndex ai_regindex)
{
    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK_DELETE);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::R_GetVersion (ClMMIndex ai_index, VersionMark &ai_vm)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::R_SetVersion (ClMMIndex ai_index, VersionMark &ai_vm)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::R_GetRecordPersist (ClMMIndex ai_index, m4bool_t &ai_persist)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::R_SetRecordPersist (ClMMIndex ai_index, m4bool_t ai_persist)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

//----------------------------------------------------------------------------

m4return_t ClBlock_Nihil::R_GetStatus (const ClMMIndex ai_index, RegisterStatus_t &ao_riStatus)
{
	ao_riStatus = M4_RS_INVALID;
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::R_SetStatus (const ClMMIndex ai_index, const RegisterStatus_t ai_NewStatus)
{
    return M4_ERROR;
}

m4bool_t ClBlock_Nihil::R_IsModified    (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsNew         (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsUpdated     (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsDeleted     (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsModifiedByDDBB (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsNewByDDBB      (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsUpdatedByDDBB  (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsDeletedByDDBB  (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsModified    (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsNew         (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsUpdated     (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsDeleted     (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsModifiedByDDBB (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsNewByDDBB      (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsUpdatedByDDBB  (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::R_IsDeletedByDDBB  (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::B_IsModified    (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::B_IsNew         (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::B_IsUpdated     (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::B_IsDeleted     (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::B_IsModifiedByDDBB (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::B_IsNewByDDBB      (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::B_IsUpdatedByDDBB  (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil::B_IsDeletedByDDBB  (void) const
{
    return M4_FALSE;
}

m4bool_t ClBlock_Nihil::R_GetNewNotPersist(ClMMIndex ai_regindex) const
{
    return M4_FALSE;
}

m4return_t ClBlock_Nihil::R_SetNewNotPersist(ClMMIndex ai_regindex, const m4bool_t ai_bNewNotPersist)
{
    return M4_ERROR;
}


m4bool_t ClBlock_Nihil::R_IsValid (ClMMIndex ai_index)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_FALSE;
}

m4return_t ClBlock_Nihil::R_SetUpdated (ClMMIndex)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_FALSE;
}

m4return_t ClBlock_Nihil::B_GetPersist (m4bool_t &)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::B_SetPersist (m4bool_t)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

m4bool_t ClBlock_Nihil::IsProxy (void) const
{
//  sin lanzar warning al log, please!
    return M4_FALSE;
}

m4bool_t ClBlock_Nihil::IsValid (void) const
{
//  sin lanzar warning al log, please!
    return M4_FALSE;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void ClBlock_Nihil::UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n)
{
}

void ClBlock_Nihil::UpdateParentInfoIndexOfMyRBs (void)
{
}

//----------------------------------------------------------------------------
//
m4return_t ClBlock_Nihil::B_Undo(void)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4bool_t ClBlock_Nihil::R_Undo(ClMMIndex ai_regindex)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_FALSE;
}

m4return_t ClBlock_Nihil::CheckPoint (ClMMIndex ai_regindex)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

//-----------------------------------------------------------------------------
// los destroys:
//-----------------------------------------------------------------------------

m4return_t ClBlock_Nihil::DestroyBlock (m4bool_t ai_notify)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::DestroyAllRegisters (m4bool_t ai_notify)
{
    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK_DELETE);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::DestroyBlockNoUpdateAccesses (void)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::DestroyAllRegistersNoUpdateAccesses (void)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_ERROR;
}

ClPartialLoadData *ClBlock_Nihil::GetpPL_Data(void)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return 0;
}

m4return_t ClBlock_Nihil::R_PrepareIndexPL (const m4bool_t ai_bUp, const m4uint32_t ai_PrevReg)
{
//    DUMP_CHLOG_WARNING(M4_CH_DM_NIHILBLOCK);
    return M4_FALSE;
}

void ClBlock_Nihil::GetParentBlock (ClBlockReference &ao_br)
{
	ao_br.Reset ();
	ao_br.Set(this);
}

ClBlock_Channel* ClBlock_Nihil::_CreateBlock(void)
{
	DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK);
	return 0;
}

// Copy/Paste de una rama de una canal utilizando serializacion
m4return_t ClBlock_Nihil::CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClGenericIODriver& IOD)
{
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::PasteRecordWithDeSerialize(m4uint32_t&        aio_iRecordIndex,
                                                     const m4bool_t     ai_bKeepMarks,
						     const m4bool_t     ai_bDestroyRecords,
						     const m4char_t*    ai_vpExclusionNodeName[],
						     const m4uint16_t   ai_iNumExclusionNodes,
						     ClGenericIODriver& IOD)
{
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClFileName& ao_oFileName)
{
    ao_oFileName.SetFileName(0);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil::PasteRecordWithDeSerialize(m4uint32_t&       aio_iRecordIndex,
						     const m4bool_t    ai_bKeepMarks,
						     const m4bool_t    ai_bDestroyRecords,
						     const m4char_t*   ai_vpExclusionNodeName[],
						     const m4uint16_t  ai_iNumExclusionNodes,
						     const ClFileName& ai_oFileName,
						     const m4bool_t    ai_bDeleteFile)
{
    return M4_ERROR;
}

//*******************************************************************************
// BOLQUE NIHIL ESPECIAL PARA EL CASO DE QUE NO SE CREE BLOQUE POR MAXNUMBLOCKS
//*******************************************************************************

///////////////////////////////////////////////////////////////////////////////
//
// ClBlock_Nihil_Memory
//

// todo casca:

ClBlock_Nihil_Memory::ClBlock_Nihil_Memory ()
{
}

ClBlock_Nihil_Memory::~ClBlock_Nihil_Memory ()
{
}

ClBlock_Channel_Proxy *ClBlock_Nihil_Memory::GetChildBlock (m4uint16_t ai_pos)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return 0;
}

size_t ClBlock_Nihil_Memory::Count (void)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_COUNT_MEMORY);
    return 0;
}

m4return_t ClBlock_Nihil_Memory::DeleteAll (void)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_DELETE_MEMORY);

    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::CheckPoint (void)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::SetCheckPointMode (m4bool_t ai_operation)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::GetCheckPointMode (m4bool_t &ai_operation)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::CommitPersisted (VersionMark &ai_version)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::RollbackPersisted (void)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::B_AddRegister (ClMMIndex &ao_newindex)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_ADD_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::R_InsertRegister (ClMMIndex ai_regindex)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_ADD_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::R_DeleteRegister (ClMMIndex ai_regindex)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_DELETE_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::R_Destroy (ClMMIndex ai_regindex)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_DELETE_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::R_GetVersion (ClMMIndex ai_index, VersionMark &ai_vm)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::R_SetVersion (ClMMIndex ai_index, VersionMark &ai_vm)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::R_GetRecordPersist (ClMMIndex ai_index, m4bool_t &ai_persist)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::R_SetRecordPersist (ClMMIndex ai_index, m4bool_t ai_persist)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

//----------------------------------------------------------------------------

m4return_t ClBlock_Nihil_Memory::R_GetStatus (const ClMMIndex ai_index, RegisterStatus_t &ao_riStatus)
{
	ao_riStatus = M4_RS_INVALID;
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::R_SetStatus (const ClMMIndex ai_index, const RegisterStatus_t ai_NewStatus)
{
    return M4_ERROR;
}

m4bool_t ClBlock_Nihil_Memory::R_IsModified    (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsNew         (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsUpdated     (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsDeleted     (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsModifiedByDDBB (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsNewByDDBB      (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsUpdatedByDDBB  (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsDeletedByDDBB  (const ClMMIndex ai_index) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsModified    (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsNew         (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsUpdated     (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsDeleted     (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsModifiedByDDBB (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsNewByDDBB      (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsUpdatedByDDBB  (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::R_IsDeletedByDDBB  (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::B_IsModified    (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::B_IsNew         (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::B_IsUpdated     (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::B_IsDeleted     (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::B_IsModifiedByDDBB (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::B_IsNewByDDBB      (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::B_IsUpdatedByDDBB  (void) const
{
    return M4_FALSE;
}
m4bool_t ClBlock_Nihil_Memory::B_IsDeletedByDDBB  (void) const
{
    return M4_FALSE;
}

m4bool_t ClBlock_Nihil_Memory::R_IsValid (ClMMIndex ai_index)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_FALSE;
}

m4return_t ClBlock_Nihil_Memory::R_SetUpdated (ClMMIndex)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_FALSE;
}

m4return_t ClBlock_Nihil_Memory::B_GetPersist (m4bool_t &)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::B_SetPersist (m4bool_t)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

m4bool_t ClBlock_Nihil_Memory::IsProxy (void) const
{
//  sin lanzar warning al log, please!
    return M4_FALSE;
}

m4bool_t ClBlock_Nihil_Memory::IsValid (void) const
{
//  sin lanzar warning al log, please!
    return M4_FALSE;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void ClBlock_Nihil_Memory::UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n)
{
}

void ClBlock_Nihil_Memory::UpdateParentInfoIndexOfMyRBs (void)
{
}

//----------------------------------------------------------------------------
//
m4return_t ClBlock_Nihil_Memory::B_Undo(void)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4bool_t ClBlock_Nihil_Memory::R_Undo(ClMMIndex ai_regindex)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_FALSE;
}

m4return_t ClBlock_Nihil_Memory::CheckPoint (ClMMIndex ai_regindex)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

//-----------------------------------------------------------------------------
// los destroys:
//-----------------------------------------------------------------------------

m4return_t ClBlock_Nihil_Memory::DestroyBlock (m4bool_t ai_notify)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::DestroyAllRegisters (m4bool_t ai_notify)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_DELETE);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::DestroyBlockNoUpdateAccesses (void)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::DestroyAllRegistersNoUpdateAccesses (void)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

ClPartialLoadData *ClBlock_Nihil_Memory::GetpPL_Data(void)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return 0;
}

m4return_t ClBlock_Nihil_Memory::R_PrepareIndexPL (const m4bool_t ai_bUp, const m4uint32_t ai_PrevReg)
{
    DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_FALSE;
}

void ClBlock_Nihil_Memory::GetParentBlock (ClBlockReference &ao_br)
{
	ao_br.Reset ();
    ao_br.Set(this);
}

ClBlock_Channel* ClBlock_Nihil_Memory::_CreateBlock(void)
{
	DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
	return 0;
}

// Copy/Paste de una rama de una canal utilizando serializacion
m4return_t ClBlock_Nihil_Memory::CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClGenericIODriver& IOD)
{
	DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::PasteRecordWithDeSerialize(m4uint32_t&        aio_iRecordIndex,
							    const m4bool_t     ai_bKeepMarks,
							    const m4bool_t     ai_bDestroyRecords,
							    const m4char_t*    ai_vpExclusionNodeName[],
							    const m4uint16_t   ai_iNumExclusionNodes,
							    ClGenericIODriver& IOD)
{
	DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
	return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClFileName& ao_oFileName)
{
	DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
    ao_oFileName.SetFileName(0);
    return M4_ERROR;
}

m4return_t ClBlock_Nihil_Memory::PasteRecordWithDeSerialize(m4uint32_t&       aio_iRecordIndex,
							    const m4bool_t    ai_bKeepMarks,
							    const m4bool_t    ai_bDestroyRecords,
							    const m4char_t*   ai_vpExclusionNodeName[],
							    const m4uint16_t  ai_iNumExclusionNodes,
							    const ClFileName& ai_oFileName,
							    const m4bool_t    ai_bDeleteFile)
{
	DUMP_CHLOG_ERROR(M4_CH_DM_NIHILBLOCK_MEMORY);
	return M4_ERROR;
}
