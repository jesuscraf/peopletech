//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             val_write.hpp
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
//	Write Interface for Value
//
//==============================================================================

#ifndef __VAL_WRITE__HPP__
#define __VAL_WRITE__HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4var.hpp"
#include "cltypes.hpp"

class _ClValue_Read_Slices;
class ClDateStack;
class _ClVirtual_Value_W;

class M4_DECL_M4DM _ClValue_Write
{
public:

//*************************************************************************
// FUNCIONALIDAD PARA SETVALUE Y SETPRIORITY
//*************************************************************************

    virtual m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, m4bool_t ai_bChangeStatus) = 0;
    virtual m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done, m4bool_t ai_bChangeStatus) = 0;
    virtual m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) = 0;
    virtual m4return_t SetNULL (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus) = 0;
    virtual m4return_t SetDefault (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus) = 0;

    virtual m4return_t SetPriority (_ClValue_Read_Slices * ai_pClVal, priority_t ai_prior) = 0;

//*************************************************************************
// FUNCIONALIDAD PARA ITEM DE TIPO BLOB
//*************************************************************************

    virtual m4return_t SetDescr(_ClValue_Read_Slices * ai_pClVal, m4pcchar_t ai_pcDescr) = 0;
	virtual m4return_t CreateTempFile(_ClValue_Read_Slices * ai_pClVal, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension) = 0;

//*************************************************************************
// COPIADO DE VALUES
//*************************************************************************

    virtual m4return_t CopyItem (_ClValue_Read_Slices * ai_pClVal, _ClValue_Read_Slices * ai_pClValRead) = 0;

//*************************************************************************
// FUNCIONALIDAD PARA TRAMOS
//*************************************************************************

    virtual m4return_t SetStartDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date) = 0;
    virtual m4return_t SetEndDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date) = 0;

    virtual m4return_t DeleteSlice (_ClValue_Read_Slices * ai_pClVal) = 0;
    virtual m4return_t DeleteAllSlices (_ClValue_Read_Slices * ai_pClVal) = 0;
    virtual m4return_t AllocSlices (_ClValue_Read_Slices * ai_pClVal, m4uint32_t ai_islices) = 0;
    virtual m4return_t AllocSlices (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack) = 0;
    virtual m4return_t AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate, m4date_t ai_enddate) = 0;
    virtual m4return_t AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate) = 0;
    virtual m4return_t SplitSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date) = 0;
    virtual m4return_t Compact (_ClValue_Read_Slices * ai_pClVal) = 0;
};

class M4_DECL_M4DM ClValue_Write : public _ClValue_Write
{
public:

    m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, m4bool_t ai_bChangeStatus);
    m4return_t SetDescr(_ClValue_Read_Slices * ai_pClVal, m4pcchar_t ai_pcDescr);

    m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done, m4bool_t ai_bChangeStatus);
    m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser);
    m4return_t SetNULL (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus);
    m4return_t SetDefault (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus);
    m4return_t SetPriority (_ClValue_Read_Slices * ai_pClVal, priority_t ai_prior);

    m4return_t SetStartDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date);
    m4return_t SetEndDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date);
    m4return_t SplitSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date);
    m4return_t AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate, m4date_t ai_enddate);
    m4return_t AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate);

    m4return_t DeleteSlice (_ClValue_Read_Slices * ai_pClVal);
    m4return_t DeleteAllSlices (_ClValue_Read_Slices * ai_pClVal);
    m4return_t AllocSlices (_ClValue_Read_Slices * ai_pClVal, m4uint32_t ai_islices);
    m4return_t AllocSlices (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack);
    m4return_t Compact (_ClValue_Read_Slices * ai_pClVal);


	m4return_t CopyItem (_ClValue_Read_Slices * ai_pClVal, _ClValue_Read_Slices * ai_pClValRead);

    m4return_t CreateTempFile(_ClValue_Read_Slices * ai_pClVal, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension);
};

class M4_DECL_M4DM ClValue_Write_Nihil : public _ClValue_Write
{
public:
    m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, m4bool_t ai_bChangeStatus);
    m4return_t SetDescr(_ClValue_Read_Slices * ai_pClVal, m4pcchar_t ai_pcDescr);

    m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done, m4bool_t ai_bChangeStatus);
    m4return_t Set (_ClValue_Read_Slices * ai_pClVal, const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser);
    m4return_t SetNULL (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus);
    m4return_t SetDefault (_ClValue_Read_Slices * ai_pClVal, m4bool_t ai_bChangeStatus);
    m4return_t SetStartDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date);
    m4return_t SetEndDate (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date);
    m4return_t SplitSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_date);
    m4return_t AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate, m4date_t ai_enddate);
    m4return_t DeleteSlice (_ClValue_Read_Slices * ai_pClVal);
    m4return_t DeleteAllSlices (_ClValue_Read_Slices * ai_pClVal);
    m4return_t AllocSlices (_ClValue_Read_Slices * ai_pClVal, m4uint32_t ai_islices);
    m4return_t AllocSlices (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack);
    m4return_t Compact (_ClValue_Read_Slices * ai_pClVal);
    m4return_t AddSlice (_ClValue_Read_Slices * ai_pClVal, m4date_t ai_startdate);
	m4return_t CopyItem (_ClValue_Read_Slices * ai_pClVal, _ClValue_Read_Slices * ai_pClValRead);
    m4return_t SetPriority (_ClValue_Read_Slices * ai_pClVal, priority_t ai_prior);

    m4return_t CreateTempFile(_ClValue_Read_Slices * ai_pClVal, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension)
    {
        return M4_ERROR;
    }

};

#endif //__VAL_WRITE__HPP__
