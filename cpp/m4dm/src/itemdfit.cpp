//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             itemdfit.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             29/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines ...
//
//
//==============================================================================

#include "itemdfit.hpp"

#include "colitdef2.hpp"
#include "item_prx.hpp"
//#include "itemdef.hpp"


//////////////////////////////////////////////////////////////////////////////
//
// ClItemProxyIt
//

ClItemProxyIt::ClItemProxyIt ()
{ 
    m_pClColItemDef_Kernel = 0;    
}

ClItemProxyIt::ClItemProxyIt (ClColItemDef_Kernel *ai_pClColItemDef_Kernel)
{
    Attach (ai_pClColItemDef_Kernel);
}

ClItemProxyIt::ClItemProxyIt (ClColItemDef_Kernel &ai_ClColItemDef_Kernel)
{
    Attach (ai_ClColItemDef_Kernel);
}

void ClItemProxyIt::Attach (ClColItemDef_Kernel *ai_pClColItemDef_Kernel)
{
    m_pClColItemDef_Kernel = ai_pClColItemDef_Kernel;

    M4_ASSERT (m_pClColItemDef_Kernel);
}

void ClItemProxyIt::Attach (ClColItemDef_Kernel &ai_ClColItemDef_Kernel)
{
    m_pClColItemDef_Kernel = &ai_ClColItemDef_Kernel;

    M4_ASSERT (m_pClColItemDef_Kernel);
}

ClItemProxyIt::~ClItemProxyIt ()
{
}

_AClItemProxy *ClItemProxyIt::operator [] (ClItemIndex ai_itemindex)
{
    M4_ASSERT (m_pClColItemDef_Kernel);

    SetpItemProxy (m_pClColItemDef_Kernel->GetByIndex (ai_itemindex));

    return GetpItemProxy ();
}

_AClItemProxy *ClItemProxyIt::operator [] (ClHandle ai_handle)
{
    M4_ASSERT (m_pClColItemDef_Kernel);

    SetpItemProxy (m_pClColItemDef_Kernel->GetByHandle (ai_handle));

    return GetpItemProxy ();
}

_AClItemProxy *ClItemProxyIt::operator [] (m4pcchar_t ai_pcId)
{
    M4_ASSERT (m_pClColItemDef_Kernel);

    SetpItemProxy (m_pClColItemDef_Kernel->GetById (ai_pcId));

    return GetpItemProxy ();
}

//////////////////////////////////////////////////////////////////////////////
//
// ClItemProxyIt_Random
//

ClItemProxyIt_Random::~ClItemProxyIt_Random ()
{
}

m4uint16_t ClItemProxyIt_Random::Count (void) const
{
    M4_ASSERT (m_pClColItemDef_Kernel);

    return m_pClColItemDef_Kernel->Count ();
}

_AClItemProxy *ClItemProxyIt_Random::Begin (void)
{
    Index = 0;
    return operator [] (Index);
}

_AClItemProxy *ClItemProxyIt_Random::End (void)
{
    Index = Count () - 1;

    return operator [] (Index);
}

m4bool_t ClItemProxyIt_Random::Next (void)
{
	if (Index != M4DM_EOF_INDEX) {
		ClHandle handle;
		
		++Index;
		M4_ASSERT (m_pClColItemDef_Kernel);
		SetpItemProxy (m_pClColItemDef_Kernel->GetByIndex (Index));
		
		if (&s_oClItemProxy_Nihil == GetpItemProxy()) {
			Index = M4DM_EOF_INDEX;
		}
	}
	
	return (Index != M4DM_EOF_INDEX);
}

m4bool_t ClItemProxyIt_Random::Prev (void)
{
    if (Index != M4DM_EOF_INDEX) {
        ClHandle handle;

        --Index;
        M4_ASSERT (m_pClColItemDef_Kernel);
        SetpItemProxy (m_pClColItemDef_Kernel->GetByIndex (Index));

        if (&s_oClItemProxy_Nihil == GetpItemProxy ()) {
            Index = M4DM_EOF_INDEX;
        }
    }

    return (Index != M4DM_EOF_INDEX);
}

m4return_t ClItemProxyIt_Random::GetSize(ClSize &ao_size)
{
	ao_size +=sizeof(ClItemProxyIt_Random);
	
	ao_size -=sizeof(ClItemProxyIt);
	return ClItemProxyIt::GetSize(ao_size);
}

m4return_t ClItemProxyIt::GetSize(ClSize &ao_size)
{
	ao_size +=sizeof(ClItemProxyIt);
	ao_size -=sizeof(_withitemproxy);
	
	return _withitemproxy::GetSize(ao_size);
}

