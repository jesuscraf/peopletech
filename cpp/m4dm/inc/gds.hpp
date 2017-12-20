#ifndef __GENERICDATASET_HPP__
#define __GENERICDATASET_HPP__

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

#include "vrb.hpp"


// Default memory using policy
const int M4_DEFAULT_INITIAL_RECORDS     = 64;
const int M4_DEFAULT_GROW_RECORDS        = 64;
const int M4_DEFAULT_INITIAL_LOG_RECORDS = 0;

// This class manages the storage of a virtual table in memory, made as
// an array of columns by rows. The elements of this array are values of Items.

// WARNING: the methods dont verify that the user has initialized
// the object before the use...

// WARNING: This class is not multithread safe. The user must be shure
// that no concurrent access is done.


#ifdef _DEBUG
#include "m4dm_dll.hpp"

class M4_DECL_M4DM ClGenericDataSet : public ClVectorRecordBase
#else
class ClGenericDataSet : public ClVectorRecordBase
#endif
{
public:
        /**
	 * Inserts a record (or set of records) in a specific position.
	 *
	 * @param ai_Index	Índice donde se quiere insertar el registro (o conjunto de registros).
	 * @param ai_iRecordID	Id que tendrá el registro. Si se insertan más de uno, tendrán Ids consecutivos.
	 * @param ai_Number	Cuantos registros se quieren insertar. Por defecto sólo uno.
	 * @return m4return_t.
	 */
        m4return_t InsertRecord(const regindex_t ai_Index, const regid_t ai_iRecordID, const regindex_t ai_Number = 1);
        
        /**
	 * Añade un registro al final.
	 *
	 * @param ao_Index	Valor de retorno, indica donde se insertó el registro.
	 * @param ai_iRecordID	Id que tendrá el registro.
	 * @return m4return_t.
	 */
        m4return_t AddRecord(regindex_t& ao_Index, const regid_t ai_iRecordID);
        
        // Deletes one record by index
        m4return_t DeleteRecord(const regindex_t ai_Index);
        
        // Destroy one record by index
        m4return_t DestroyRecord(const regindex_t ai_Index);
        
        // Deletes all the records
        void Zap(void);
};

#endif
