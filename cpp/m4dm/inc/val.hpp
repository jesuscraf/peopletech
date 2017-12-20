//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             val.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             12/12/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    class ClValue
//
//
//==============================================================================

#include "gm_intrf.hpp"
#include "valh_rs.hpp"
#include "valh_rws.hpp"
#include "m4dm_dll.hpp"


#ifndef __VAL_HPP__
#define __VAL_HPP__


class M4_DECL_M4DM ClValue : public _ClValue_ReadWrite_Slices
{
public:
    ClValue (ClItemDef *ai_pitemdef) :
        _ClValue_ReadWrite_Slices (&s_gmval, ai_pitemdef)
    {
    }

    ClValue (const ClValue &ai_value);

    ClValue &operator= (const ClValue &ai_value);
    ClValue &operator[] (ClSliceIndex ai_sliceindex);
    ClValue &operator[] (m4date_t ai_date);
    ClValue &operator++ ();     //pre
    ClValue &operator-- ();     //pre

private:
    static _ClGM_Value s_gmval;
};

class M4_DECL_M4DM ClOldValue : public _ClValue_Read_Slices
{
public:
    ClOldValue (ClItemDef *ai_pitemdef) :
        _ClValue_Read_Slices (&s_gmoldval, ai_pitemdef)
    {
    }

    ClOldValue (const ClValue &ai_value);

    ClOldValue &operator= (const ClValue &ai_value);
    ClOldValue &operator[] (ClSliceIndex ai_sliceindex);
    ClOldValue &operator[] (m4date_t ai_date);
    ClOldValue &operator++ ();     //pre
    ClOldValue &operator-- ();     //pre

private:
    static _ClGM_OldValue s_gmoldval;
};

#endif //__VAL_HPP__
