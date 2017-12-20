//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_block.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             01/09/97
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

#include "ch_block.hpp"
#include "node_knl.hpp"
#include "nodedef.hpp"
#include "dsitem.hpp"


const m4uint16_t VERSION_MARK_OFFSET     = 0;
const m4uint16_t OLD_VERSION_MARK_OFFSET = sizeof(VersionMark);


void ClBlock_Channel::Attach (const ClNode_Data * const ai_pNodeData)
{
	M4_ASSERT (ai_pNodeData);
	
	ClNodeDef *pNodeDef = ai_pNodeData->GetpNodeDef();
	M4_ASSERT (pNodeDef);
	M4_ASSERT (pNodeDef->GetHandle());
	
	m4uint32_t NumRows = pNodeDef->NumRows();
	
	Block.Init(ai_pNodeData->GetpSMDataBlock());
	if (pNodeDef->MaxNumRecords()) {
		Records.Init(ai_pNodeData->GetpSMDataRecord(), 8, 8, 0, pNodeDef->MaxNumRecords());

	} else {
		Records.Init(ai_pNodeData->GetpSMDataRecord(), 8, 8, 0, ClVectorRecordBase::MAX_COUNT_ALLOW);
	}
	
	if (NumRows) {
		if (!m_pClPL_Data) {
			m_pClPL_Data = new ClPartialLoadData(NumRows);
		}
	}
}

m4return_t ClBlock_Channel::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClBlock_Channel);
	// De momento para estimar.

	ao_size -= sizeof(ClRecordWithTransactions);
	m4return_t iRet = Block.GetSize(ao_size);
	if (iRet != M4_SUCCESS) return iRet;

	ao_size -= sizeof(ClGenericDataSet);
	iRet = Records.GetSize(ao_size);
	if (iRet != M4_SUCCESS) return iRet;

	if (m_pClPL_Data){
		iRet = m_pClPL_Data->GetSize(ao_size);
		if (iRet !=M4_SUCCESS) return iRet;
	}
	return iRet;
}

m4return_t ClBlock_Channel::B_Undo(void)
{
	m4return_t ret = Block.Rollback();
	if (ret != M4_SUCCESS) return ret;
	
	ret = Records.Rollback();
	if (ret != M4_SUCCESS) return ret;
	
	regindex_t iRecordsCount = Records.GetCount();
	for (regindex_t iRecord = 0; iRecord < iRecordsCount; iRecord++) {
		VersionMark oldVersion;
		
		ret = Records.GetUserDataByPosAndOffset(&oldVersion, OLD_VERSION_MARK_OFFSET, iRecord, sizeof(VersionMark));
		if (ret != M4_SUCCESS) return ret;
		
		ret = Records.SetUserDataByPosAndOffset(&oldVersion, VERSION_MARK_OFFSET, iRecord, sizeof(VersionMark));
		if (ret != M4_SUCCESS) return ret;
	}
	
	return ret;
}

m4return_t ClBlock_Channel::RollbackPersisted(void)
{
	// ??? Ojo que esto solo se tiene que hacer para el rollback del persistFlag
	m4bool_t   AuxDirty;

	m4return_t ret = SetBlockPersistLDB(M4_FALSE);
	if (ret != M4_SUCCESS) return ret;

	regindex_t  numRecords = Records.GetCount();
	VersionMark version;

	for (regindex_t index = 0; index < numRecords; index++) {
		AuxDirty = R_GetRecordPersist(index);

		if (AuxDirty == M4_TRUE) {
			ret = R_SetRecordPersist(index, M4_FALSE);
			if (ret != M4_SUCCESS) return ret;

			// Restauramos el valor que tenia antes
			ret = Records.GetUserDataByPosAndOffset(&version, OLD_VERSION_MARK_OFFSET, index, sizeof(VersionMark));
			if (ret != M4_SUCCESS) return ret;

			ret = Records.SetUserDataByPosAndOffset(&version, VERSION_MARK_OFFSET, index, sizeof(VersionMark));
			if (ret != M4_SUCCESS) return ret;
		}
	}
	return ret;
}

m4return_t ClBlock_Channel::R_GetVersion(ClMMIndex ai_iIndex, VersionMark& ao_Version)
{
	return Records.GetUserDataByPosAndOffset(&ao_Version, VERSION_MARK_OFFSET, ai_iIndex, sizeof(VersionMark));
}

m4return_t ClBlock_Channel::R_SetVersion(ClMMIndex ai_iIndex, VersionMark& ai_Version)
{
	Records.SetRecordVersion(ai_iIndex, ai_Version);
	return M4_SUCCESS;
}
