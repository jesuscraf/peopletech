//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             val_read.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             27/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//
//
//==============================================================================

#ifndef __VAL_READ__HPP__
#define __VAL_READ__HPP__
#include "m4dm_dll.hpp"
#include "dmres.hpp"

#include "m4types.hpp"
#include "index.hpp"
#include "m4var.hpp"

//class m4VariantType;
class _ClValue_Read_Slices;
class ClDateStack;


class M4_DECL_M4DM _ClValue_Read
{
public:

//*************************************************************************
// FUNCIONALIDAD PARA GETVALUE
//*************************************************************************

    virtual void *     Get (_ClValue_Read_Slices * ai_ClVal, m4return_t * ao_pdone = 0) = 0;
    virtual m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4VariantType &ao_var, m4return_t * ao_pdone = 0) = 0;
    virtual m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone = 0) = 0;
    virtual m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, m4return_t * ao_pdone = 0) = 0;

//*************************************************************************
// FUNCIONALIDAD PARA ITEM DE TIPO BLOB
//*************************************************************************

    virtual m4pchar_t  GetDescr(_ClValue_Read_Slices * ai_ClVal) = 0;
    virtual m4return_t ChangedMask(_ClValue_Read_Slices * ai_pClVal, m4uint32_t & ao_oChangedMask) = 0;

//*************************************************************************
// FUNCIONALIDAD PARA TRAMOS
//*************************************************************************

    virtual m4return_t FillDateStack (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack) = 0;
};

class M4_DECL_M4DM ClValue_Read : public _ClValue_Read
{
public:

    void *     Get (_ClValue_Read_Slices * ai_ClVal, m4return_t * ao_pdone = 0);
    m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4VariantType &ao_var, m4return_t * ao_pdone = 0);
    m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone = 0);
    m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, m4return_t * ao_pdone = 0);

    m4pchar_t  GetDescr(_ClValue_Read_Slices * ai_ClVal);
    m4return_t ChangedMask(_ClValue_Read_Slices * ai_pClVal, m4uint32_t & ao_oChangedMask);

    m4return_t FillDateStack (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack);
};

class M4_DECL_M4DM ClValue_Read_Nihil : public _ClValue_Read
{
public:

    void *     Get (_ClValue_Read_Slices * ai_ClVal, m4return_t * ao_pdone = 0) { return 0; };
    m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4VariantType &ao_var, m4return_t * ao_pdone = 0);
    m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone = 0);
    m4return_t Get (_ClValue_Read_Slices * ai_ClVal, m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, m4return_t * ao_pdone = 0);

    m4pchar_t  GetDescr(_ClValue_Read_Slices * ai_ClVal) { return 0; }
    m4return_t ChangedMask(_ClValue_Read_Slices * ai_pClVal, m4uint32_t & ao_oChangedMask)
    {
        return M4_ERROR;
    }

    m4return_t FillDateStack (_ClValue_Read_Slices * ai_pClVal, ClDateStack *ai_stack)
    {
        return M4_ERROR;
    }
};

#endif // __VAL_READ__HPP__
