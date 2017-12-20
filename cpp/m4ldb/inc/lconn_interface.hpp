//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                lconn_interface.hpp   
// Project:             mind 4.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-02-00
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module defines the interface of the Logical Connection.
//
//
//==============================================================================

#ifndef _LCONN_INTERFACE_HPP
#define _LCONN_INTERFACE_HPP

#include "m4ldb_dll.hpp"

#include "m4types.hpp"
#include "ldb_def.hpp"

class ClAccessRecordSet;
class ClVMStatistics;
class m4VariantType;

class M4_DECL_M4LDB ClLConn_Interface {
public:
	ClLConn_Interface(void){}
	virtual ~ClLConn_Interface(void){}
	virtual m4return_t Init (/*??????????????????????*/)=0;
	virtual m4return_t End (void)=0;
	
	// Carga.
	virtual m4return_t Load		(ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem = 0, m4VariantType *ai_pParam = 0, m4int32_t ai_numParam = 0, m4int32_t ai_idRConn = M4LDB_NULL_INDEX, m4int32_t   *ao_iNumData = 0,m4int32_t   ai_iFirstData = 0) = 0;
	virtual m4return_t SysLoad	(ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_indSelect = M4LDB_NULL_INDEX, m4int32_t ai_lSysDebugLevel = M4LDB_DEBUG_LEVEL_2 ) = 0;
	
	// Persistencia.
	virtual m4return_t InsertBlk (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics,m4bool_t &ao_bStopExecution  ) = 0;
	virtual m4return_t UpdateBlk (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics,m4bool_t &ao_bStopExecution  ) = 0;
	virtual m4return_t DeleteBlk (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics,m4bool_t &ao_bStopExecution  ) = 0;

	// Gestión de transacciones.
	virtual m4return_t BeginTransaction (ClAccessRecordSet *ai_pRecordSet) = 0;
	virtual m4return_t EndTransaction (ClAccessRecordSet *ai_pRecordSet, m4int32_t ai_iAction, m4bool_t &ao_bFree) = 0; 

	// Ejecuciones directas.
  	virtual m4return_t ExecuteSQL (ClAccessRecordSet *ai_pRecordSet, ClVMStatistics *ai_poStatistics, m4int32_t ai_hItem, m4bool_t ai_bReal, m4VariantType *ai_pParam = 0, m4int32_t ai_numParam = 0, m4int32_t ai_idRConn = M4LDB_NULL_INDEX,m4int32_t ai_lMask = 0) = 0;

	//Bug:63638
	virtual m4int32_t GetNumTrans(void) = 0;

protected:
};

#endif
