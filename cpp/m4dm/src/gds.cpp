//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                gds.cpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                28-07-1997
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Definitions used in the Generic Data Set module
//
// 11/07/97
//==============================================================================

#include "gds.hpp"


m4return_t ClGenericDataSet::InsertRecord(const regindex_t ai_Index, const regid_t ai_iRecordID, const regindex_t ai_Number/*=1*/)
{
	return ClVectorRecordBase::InsertRecordsLL(ai_Index, ai_iRecordID, ai_Number, M4_TRUE);
}

m4return_t ClGenericDataSet::AddRecord(regindex_t& ao_Index, const regid_t ai_iRecordID)
{
	ao_Index = GetCount();
	
	return InsertRecord(ao_Index, ai_iRecordID, 1);
}

m4return_t ClGenericDataSet::DeleteRecord(const regindex_t ai_Index)
{
	M4_MSG_ASSERT(ai_Index<GetCount() && ai_Index>=0,"Index fuera de rango en ClGenericDataSet::DeleteRecord()");
	
	return ClVectorRecordBase::DeleteRecord(ai_Index);
}

m4return_t ClGenericDataSet::DestroyRecord(const regindex_t ai_Index)
{
	M4_MSG_ASSERT(ai_Index<GetCount() && ai_Index>=0,"Index fuera de rango en ClGenericDataSet::DestroyRecord()");
	
	return ClVectorRecordBase::DestroyRecordLL(ai_Index);
}

void ClGenericDataSet::Zap(void)
{
	regindex_t TotalRecords = ClVectorRecordBase::GetCount();
	if (UsingCheckPoints()) {
		for (regindex_t regIndex = 0; regIndex < TotalRecords; regIndex++) {
			DeleteRecord(regIndex);
		}
	} else {
		for (regindex_t regIndex = 0; regIndex < TotalRecords; regIndex++) {
			DeleteRecord(0);
		}
	}
}
