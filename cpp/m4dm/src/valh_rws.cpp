//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             valh_rws.cpp
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

#include "valh_rws.hpp"
#include "val_write.hpp"
#include "item_prx.hpp"

#include "v_contxt.hpp"

m4uint32_t _ClValue_ReadWrite_Slices::s_idefault;

m4return_t _ClValue_ReadWrite_Slices::Set (const m4VariantType &ai_var)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->Set(this, ai_var, M4_TRUE);
}

m4return_t _ClValue_ReadWrite_Slices::SetNoChangeStatus (const m4VariantType &ai_var)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->Set(this, ai_var, M4_FALSE);
}

m4return_t _ClValue_ReadWrite_Slices::SetDescr(m4pcchar_t ai_pcDescr)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->SetDescr(this, ai_pcDescr);
}

m4return_t _ClValue_ReadWrite_Slices::Set (const m4VariantType &ai_var, priority_t ai_prior, m4bool_t &done)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->Set(this, ai_var, ai_prior, done, M4_TRUE);
}
m4return_t _ClValue_ReadWrite_Slices::Set (const void *ai_paddress, m4uint32_t ai_size, m4uint32_t ai_offset, m4uint32_t * ao_retSize)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->Set(this, ai_paddress, ai_size, ai_offset, ao_retSize, M4_TRUE, M4_FALSE);
}
m4return_t _ClValue_ReadWrite_Slices::SetNULL (void)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->SetNULL(this, M4_TRUE);
}
m4return_t _ClValue_ReadWrite_Slices::SetDefault (void)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->SetDefault(this, M4_TRUE);
}
m4return_t _ClValue_ReadWrite_Slices::SetStartDate (m4date_t ai_date)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->SetStartDate(this, ai_date);
}
m4return_t _ClValue_ReadWrite_Slices::SetEndDate (m4date_t ai_date)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->SetEndDate(this, ai_date);
}
m4return_t _ClValue_ReadWrite_Slices::SplitSlice (m4date_t ai_date)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->SplitSlice(this, ai_date);
}
m4return_t _ClValue_ReadWrite_Slices::AddSlice (m4date_t ai_startdate, m4date_t ai_enddate)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->AddSlice(this, ai_startdate, ai_enddate);
}
m4return_t _ClValue_ReadWrite_Slices::DeleteSlice (void)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->DeleteSlice(this);
}
m4return_t _ClValue_ReadWrite_Slices::DeleteAllSlices (void)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->DeleteAllSlices(this);
}
m4return_t _ClValue_ReadWrite_Slices::AllocSlices (m4uint32_t ai_islices)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->AllocSlices (this, ai_islices);
}
m4return_t _ClValue_ReadWrite_Slices::AllocSlices (ClDateStack *ai_stack)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->AllocSlices(this, ai_stack);
}
m4return_t _ClValue_ReadWrite_Slices::Compact (void)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->Compact(this);
}
m4return_t _ClValue_ReadWrite_Slices::AddSlice (m4date_t ai_startdate)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->AddSlice (this, ai_startdate);
}
m4return_t _ClValue_ReadWrite_Slices::CopyItem (_ClValue_Read_Slices * ai_pClValRead)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->CopyItem (this, ai_pClValRead);
}
m4return_t _ClValue_ReadWrite_Slices::SetPriority (priority_t ai_prior)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->SetPriority (this, ai_prior);
}

m4return_t _ClValue_ReadWrite_Slices::CreateTempFile(m4pchar_t const ai_pDirectory,
													 m4pchar_t const ai_pExtension)
{
    return ((_ClValue_Write *)m_oContext.pClItemProxy->GetpWrite ())->CreateTempFile(this,
        ai_pDirectory, ai_pExtension);
}
