//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             valh_rws.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             28/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//	Read Interface for Value
//
//==============================================================================

#ifndef __VALH_RWS__HPP__
#define __VALH_RWS__HPP__
#include "m4dm_dll.hpp"
#include "valh_rs.hpp"
#include "m4var.hpp"
#include "m4types.hpp"

class _ClVirtual_Value_W;
class ClDateStack;

//Clase base de Value de Escritura

class M4_DECL_M4DM _ClValue_ReadWrite_Slices : public _ClValue_Read_Slices
{
public:
    static m4uint32_t s_idefault;

public:
    _ClValue_ReadWrite_Slices (_ClGM_Interface *ai_pMemory_Interface, ClItemDef *ai_pitemdef) : 
        _ClValue_Read_Slices (ai_pMemory_Interface, ai_pitemdef)
    {
    }

    m4return_t Set (const m4VariantType &ai_var);
    m4return_t SetNoChangeStatus (const m4VariantType &ai_var);

    m4return_t SetDescr(m4pcchar_t ai_pcDescr);

	m4return_t CreateTempFile(m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension);

    m4return_t Set (const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done);

    m4return_t Set (const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset = 0, m4uint32_t * ao_retSize = &s_idefault);
    m4return_t SetNULL (void);
    m4return_t SetDefault (void);

    m4return_t SetStartDate (m4date_t ai_date);
    m4return_t SetEndDate (m4date_t ai_date);
    m4return_t SplitSlice (m4date_t ai_date);
    m4return_t AddSlice (m4date_t ai_startdate, m4date_t ai_enddate);
    
    m4return_t DeleteSlice (void);
    m4return_t DeleteAllSlices (void);
    m4return_t AllocSlices (m4uint32_t ai_islices);
    m4return_t AllocSlices (ClDateStack *ai_stack);

    m4return_t Compact (void);

    m4return_t AddSlice (m4date_t ai_startdate);

	m4return_t CopyItem (_ClValue_Read_Slices * ai_pClValRead);

    m4return_t SetPriority (priority_t ai_prior = 3);
};

#endif //__VALH_RWS__HPP__
