//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             valh_rs.hpp
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

#ifndef __VALH_RS__HPP__
#define __VALH_RS__HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4var.hpp"
#include "val_ge.hpp"

//template <class T, class S>
//class ClTotalize;
#include "totalize.hpp"

class M4_DECL_M4DM _ClValue_Read_Slices : public ClValue_State
{
public:

    _ClValue_Read_Slices (_ClGM_Interface *ai_pMemory_Interface, ClItemDef *ai_pitemdef) : 
        ClValue_State (ai_pMemory_Interface, ai_pitemdef),
        m_pSliceFooter (0)
    {
    }

   ~_ClValue_Read_Slices()
    {
        if (m_pSliceFooter)
            delete m_pSliceFooter;
    }

    m4pchar_t GetDescr(void);

    m4return_t ChangedMask(m4uint32_t & ao_oChangedMask);

    void *Get (m4return_t * ao_pdone = 0);
    m4return_t Get (m4VariantType &ao_var, m4return_t * ao_pdone = 0);
    m4return_t Get (m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone = 0);
    m4return_t Get (m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, m4return_t * ao_pdone = 0);


    ClTotalize <_ClValue_Read_Slices, m4VariantType, ClGetValue> & GetpSliceFooter(void);

	m4return_t GetSize(ClSize &ao_size);

protected:
    //comentado para pruebas:

    ClTotalize <_ClValue_Read_Slices, m4VariantType, ClGetValue> * m_pSliceFooter; // totalizador
};

M4_DECL_M4DM ostream & operator<< (ostream &ai_ofs, _ClValue_Read_Slices &ai_value);

#endif //__VALH_RS__HPP__
