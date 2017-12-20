//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                ds.hpp
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

#include "rec_tran.hpp"
#include "m4chcommon.hpp"


m4return_t ClRecordWithTransactions::Init(const EMDStaticData* const ai_pSMData)
{
	m4return_t iRet = ClVectorRecordBase::Init(ai_pSMData, 1, 0, 1, 1);
	if (iRet != M4_SUCCESS) return iRet;
	//EDU:los RWT los pongo sin transacciones --> items de bloque y de nodo
	//Comentado y que Dios reparta suerte, que me voy de vacaciones
	//ClVectorRecordBase::UseCheckPoints (M4_FALSE);
	
	iRet = InsertRecordsLL(0, -1, 1);
	if (iRet != M4_SUCCESS) return M4_ERROR;
	
	iRet = SetStatus(0, M4_RS_NORMAL);
	if (iRet != M4_SUCCESS) return iRet;
	
	SetStatus(VS_NORMAL);
	
	return iRet;
}
