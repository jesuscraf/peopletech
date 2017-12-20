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

#include "valh_rs.hpp"
#include "val_read.hpp"
#include "item_prx.hpp"

#include "v_contxt.hpp"


ClTotalize <_ClValue_Read_Slices, m4VariantType, ClGetValue> & _ClValue_Read_Slices::GetpSliceFooter(void)
{
    if (!m_pSliceFooter)
    {
        m_pSliceFooter = new ClTotalize <_ClValue_Read_Slices, m4VariantType, ClGetValue>;
        m_pSliceFooter->Init (this);
    }

    M4_ASSERT(m_pSliceFooter);

    return *m_pSliceFooter;
}

void *_ClValue_Read_Slices::Get (m4return_t * ao_pdone)
{
    return ((_ClValue_Read *)m_oContext.pClItemProxy->GetpRead ()->Get(this, ao_pdone));
}

m4pchar_t _ClValue_Read_Slices::GetDescr(void)
{
    return ((_ClValue_Read *)m_oContext.pClItemProxy->GetpRead ())->GetDescr(this);
}

m4return_t _ClValue_Read_Slices::Get (m4VariantType &ao_var, m4return_t * ao_pdone)
{
    return ((_ClValue_Read *)m_oContext.pClItemProxy->GetpRead ())->Get(this, ao_var, ao_pdone);
}

m4return_t _ClValue_Read_Slices::Get (m4pchar_t ao_buffer, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize, m4return_t * ao_pdone)
{
    return ((_ClValue_Read *)m_oContext.pClItemProxy->GetpRead ())->Get(this, ao_buffer, ai_size, ai_offset, ao_retSize, ao_pdone);
}

m4return_t _ClValue_Read_Slices::Get (m4date_t ai_date1, m4date_t ai_date2, m4VariantType &ao_var, m4return_t * ao_pdone)
{
    return ((_ClValue_Read *)m_oContext.pClItemProxy->GetpRead ())->Get(this, ai_date1, ai_date2, ao_var, ao_pdone);
}

m4return_t 
_ClValue_Read_Slices::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_ClValue_Read_Slices);
		ao_size-=sizeof(ClValue_State);
		
	if (ClValue_State::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_pSliceFooter){
		if (m_pSliceFooter->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	
	return M4_SUCCESS;
}

m4return_t _ClValue_Read_Slices::ChangedMask(m4uint32_t & ao_oChangedMask)
{
    return ((_ClValue_Read *)m_oContext.pClItemProxy->GetpRead ())->ChangedMask(this,
        ao_oChangedMask);
}

