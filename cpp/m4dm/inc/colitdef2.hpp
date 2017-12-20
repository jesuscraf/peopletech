//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             colitdef2.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             09/08/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines class ClNodeDef, static info about a node
//
//
//==============================================================================

//## begin module.includes preserve=yes
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "index.hpp"
#include "handles.hpp"

//## end module.includes preserve=yes

#ifndef __COLITDEF2_HPP__
#define __COLITDEF2_HPP__

//## begin module.additionalDeclarations preserve=yes
class ClCompiledMCR;
class ClNodeDef;
class ClItemProxy_Register;
class ClNodeDirectory_Data;
class _AClItemProxy;
class ClNode_Data;
//## end module.additionalDeclarations

// se coge como patron:
#define PATRONITEMPROXY ClItemProxy_Register



//////////////////////////////////////////////////////////////////////////////
//
// ClColItemDef_Kernel
//

class M4_DECL_M4DM ClColItemDef_Kernel
{
public:
    ClColItemDef_Kernel ();
   ~ClColItemDef_Kernel ();

    m4return_t Init (ClNodeDef &ai_nodedef, ClNodeDirectory_Data &ai_nodedir);
    void CalcPos (void);
    void Refresh (ClNode_Data *ai_pchildnode);

    // Inicializacion de la implementacion de tramos
	m4return_t SetSliceMode (m4bool_t ai_bsenable);
	m4return_t InitSecurity (void);

    void Destroy (void);

    // la pongo inline, a ver si se nota:
    _AClItemProxy *GetByIndex (ClItemIndex ai_itemindex);
    _AClItemProxy *GetByHandle (ClHandle ai_handle);
    _AClItemProxy *GetById (m4pcchar_t ai_pcId);

    m4uint16_t Count (void) const
    {
        return m_iCount;
    }

    m4bool_t IsValid (void)
    {
        return (m_pClNodeDef)? M4_TRUE : M4_FALSE;
    }
	m4return_t GetSize(ClSize &ao_size);

private:
    void _CreateItemProxy (m4uint16_t ai_type, m4uint16_t ai_scope,_AClItemProxy *ai_paddress);

    PATRONITEMPROXY *m_pPool;
    m4uint16_t m_iCount;

    //Utilizado para búsquedas:
    ClNodeDef *m_pClNodeDef;
};

#endif // __COLITDEF2_HPP__
