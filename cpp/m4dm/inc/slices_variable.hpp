//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             slices_variable.hpp
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

#ifndef __SLI_VARIA__HPP__
#define __SLI_VARIA__HPP__

#include "m4types.hpp"
#include "cldstack.hpp"
#include "virt_val.hpp"

class StKernelContext;

class ClGenericIODriver;

class _ClSlices_Variable: public _ClVirtual_Value_W
{
public:

	m4return_t Count (const StKernelContext &ai_context, m4uint32_t &ao_count, void **ao_valueadd = 0);

	m4return_t GetAddress (const StKernelContext &ai_context, m4uint32_t ai_index, void **ao_address, void **ao_valueadd = 0);

	m4return_t GetDescription (const StKernelContext &ai_context, m4uint32_t ai_index, void **ao_address);

        m4return_t GetAddress_withstartaddress (const StKernelContext &ai_context, m4uint32_t ai_index, void **aio_address, void **ao_valueadd = 0);

	m4return_t GetStartDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd = 0);

	m4return_t GetEndDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd = 0);

	m4return_t GetEndDatePlusOne (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd = 0);

	m4return_t Size (const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t &ao_size, void **ao_valueadd = 0);

	m4return_t IsNull (const StKernelContext &ai_context, m4uint32_t ai_index, m4bool_t &ao_isnull, void **ao_valueadd = 0);

	m4return_t GetPriority (const StKernelContext &ai_context, m4uint32_t ai_index, priority_t &ao_priority, void **ao_valueadd = 0);

	m4return_t GetFirstIndex (const StKernelContext &ai_context, m4uint32_t &ao_index, void **ao_valueadd = 0);

	m4return_t GetLastIndex (const StKernelContext &ai_context, m4uint32_t &ao_index, void **ao_valueadd = 0);

	m4return_t GetFirstStartDate (const StKernelContext &ai_context, m4date_t &ao_date, void **ao_valueadd = 0);

	m4return_t GetLastEndDate (const StKernelContext &ai_context, m4date_t &ao_date, void **ao_valueadd = 0);

	m4return_t FindByDate (const StKernelContext &ai_context, m4date_t ai_date, m4uint32_t &ao_index, void **ao_valueadd = 0);

	m4return_t SetPriority (const StKernelContext &ai_context, m4uint32_t ai_index, priority_t ao_priority, void **ao_valueadd = 0);

	m4return_t SetNull (const StKernelContext &ai_context, m4uint32_t ai_index, m4bool_t ai_bSetSize, void **ao_valueadd = 0);

	m4return_t SetValue (const StKernelContext &ai_context, m4uint32_t ai_index, const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t &ao_retSize, void **ao_valueadd = 0);

	m4return_t SetDescription (const StKernelContext &ai_context, m4uint32_t ai_index, const void *ai_paddress);

	m4return_t SetStartDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t ai_date, void **ao_valueadd = 0);

	m4return_t SetEndDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t ai_date, void **ao_valueadd = 0);

	m4return_t Delete (const StKernelContext &ai_context, m4uint32_t &ai_index, void **ao_valueadd = 0);

	m4return_t DeleteAll (const StKernelContext &ai_context, void **ao_valueadd = 0);

	m4return_t Insert (const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t ai_count, void **ao_valueadd = 0);

	m4return_t AllocSlices (const StKernelContext &ai_context, ClDateStack *ai_stack, void **ao_valueadd = 0);

	m4return_t Serialize (ClGenericIODriver& IOD, m4pcchar_t ai_poPtrSlice, m4char_t ai_iCppType);

	m4return_t DeSerializedSize (ClGenericIODriver& IOD, m4uint32_t &ao_iSize);

	m4return_t DeSerialize (ClGenericIODriver& IOD, m4pcchar_t ai_poPtrSlice, m4char_t ai_iCppType, m4uint32_t m_Size);

//    m4return_t CopyItem(const StKernelContext &ai_Dest, StKernelContext &ai_Source, void **ao_valueadd = 0);

    m4return_t CreateTempFile(const StKernelContext &ai_context, m4uint32_t ai_index, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension)
    {
        return M4_ERROR;
    }

    m4return_t ChangedMask(const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t & ao_oChangedMask)
    {
        return M4_ERROR;
    }

private:

	m4return_t Compact(const StKernelContext &ai_context, void **ao_valueadd = 0);

	m4return_t SingleInsert(const StKernelContext &ai_context, m4uint32_t *SliceHeader,
                            m4uint32_t ai_index, void **ao_valueadd = 0);

	m4return_t MaskedOffset(m4uint32_t *Offset, m4uint32_t *MOffset,
                            void **ao_valueadd = 0);

	m4return_t FlagMaskedOffset(m4uint32_t *Offset, m4uint32_t *MOffset,
                                void **ao_valueadd = 0);

	m4uint32_t FreeSpace(const StKernelContext &ai_context, void **ao_valueadd = 0);

	m4return_t SetNoNull (const StKernelContext &ai_context, m4uint32_t ai_index,
                          void **ao_valueadd = 0);

    m4return_t ValidateIndex(m4uint32_t * aio_address, m4uint32_t ai_index,
                             const StKernelContext &ai_context, m4uint32_t ai_ErrorCode,
                             void **ao_valueadd = 0)
    {
        if (aio_address)
        {
	        m4uint32_t n = aio_address[1];
	        if (ai_index >= n)
            {
                DumpError(ai_context, ai_ErrorCode);
                return M4_ERROR;
            }
            return M4_SUCCESS;
        }
        return M4_ERROR;
    }
};

#endif  //__VAL_BASE__HPP__ 
