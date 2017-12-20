//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             virtual_value_R.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             03/12/97
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

#ifndef __VIRT_VAL__HPP__
#define __VIRT_VAL__HPP__

#include "m4types.hpp"
#include "gm_intrf.hpp"
#include "cldstack.hpp"

#include "m4var.hpp"

class StKernelContext;

class _ClVirtual_Value_R
{
public:

	virtual m4return_t Count (const StKernelContext &ai_context, m4uint32_t &ao_count, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetAddress (const StKernelContext &ai_context, m4uint32_t ai_index, void **ao_address, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetDescription (const StKernelContext &ai_context, m4uint32_t ai_index, void **ao_address){ return M4_SUCCESS; };

        virtual m4return_t GetAddress_withstartaddress (const StKernelContext &ai_context, m4uint32_t ai_index, void **aio_address, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetStartDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetEndDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetEndDatePlusOne (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t Size (const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t &ao_size, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t IsNull (const StKernelContext &ai_context, m4uint32_t ai_index, m4bool_t &ao_isnull, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetPriority (const StKernelContext &ai_context, m4uint32_t ai_index, priority_t &ao_priority, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetFirstIndex (const StKernelContext &ai_context, m4uint32_t &ao_index, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetLastIndex (const StKernelContext &ai_context, m4uint32_t &ao_index, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetFirstStartDate (const StKernelContext &ai_context, m4date_t &ao_date, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t GetLastEndDate (const StKernelContext &ai_context, m4date_t &ao_date, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t FindByDate (const StKernelContext &ai_context, m4date_t ai_date, m4uint32_t &ao_index, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t HasSlices (const StKernelContext &ai_context, m4bool_t &ao_hasslices, void **ao_valueadd = 0) { return M4_ERROR; };

    virtual m4return_t ChangedMask(const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t & ao_oChangedMask) { return M4_ERROR; };

    void DumpError(const StKernelContext &ai_context, m4uint32_t ai_ErrorCode);
};

class _ClVirtual_Value_W: public _ClVirtual_Value_R
{
public:

	virtual m4return_t SetPriority (const StKernelContext &ai_context, m4uint32_t ai_index, priority_t ao_priority, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t SetNull (const StKernelContext &ai_context, m4uint32_t ai_index, m4bool_t ai_bSetSize, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t SetValue (const StKernelContext &ai_context, m4uint32_t ai_index, const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t &ao_retSize, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t SetDescription (const StKernelContext &ai_context, m4uint32_t ai_index, const void *ai_paddress){ return M4_SUCCESS; };

	virtual m4return_t SetStartDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t ai_date, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t SetEndDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t ai_date, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t Delete (const StKernelContext &ai_context, m4uint32_t &ai_index, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t DeleteAll (const StKernelContext &ai_context, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t Insert (const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t ai_count, void **ao_valueadd = 0) { return M4_ERROR; };

	virtual m4return_t AllocSlices (const StKernelContext &ai_context, ClDateStack *ai_stack, void **ao_valueadd = 0) { return M4_ERROR; };
};

//EDU:
// Clase nula, no hace nada y devuelve error siempre. Se utiliza cuando el item está en eof

class _ClVirtual_Value_Nihil : public _ClVirtual_Value_W
{
public:
    // No implementa nada devuelve todo error como la base.
};

#endif
