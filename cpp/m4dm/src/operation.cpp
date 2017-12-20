//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           operation.cpp
// File:             operation.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
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

#include "operation.hpp"
#include "node_knl.hpp"

#include "register.hpp"
#include "recorset.hpp"

#include "rec_tran.hpp"

#include "px_block.hpp"
#include "access.hpp"

///////////////////////////////////////////////////////////////////////////////
//
// _ClOperation_Base
//

m4return_t 
_ClOperation_Base::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(m4pchar_t);		// Funciones virtuales.

	return M4_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//
// ClOperation_Register
//

ClOperation_Register::ClOperation_Register ()
{
    m_pClRegister = 0;
}

void ClOperation_Register::Attach (_ClRegister_Base *ai_pClRegister)
{
    m_pClRegister = ai_pClRegister;
}

ClOperation_Register &ClOperation_Register::operator = (ClOperation_Register &ai_op)
{
    m_pClRegister = ai_op.m_pClRegister;
    return (*this);
}


m4return_t ClOperation_Register::SetUpdated (void)
{
    m_pClRegister->SetUpdated ();
    return M4_SUCCESS;
}
m4return_t 
ClOperation_Register::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(m_pClRegister);
	if (_ClOperation_Base::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4bool_t ClOperation_Register::IsValid (void) const
{
    m4bool_t bis;
    m_pClRegister->IsValid (bis);
    return bis;
}

//********************************************************
// FUNCIONES COMUNES
//********************************************************

m4bool_t ClOperation_Register::IsModified (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
        return m_pClRegister->GetCurrentBlock ()->R_IsModified (m_pClRegister->GetMMIndex ());
    else
        return M4_FALSE;
}

m4bool_t ClOperation_Register::IsNew (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
        return m_pClRegister->GetCurrentBlock ()->R_IsNew (m_pClRegister->GetMMIndex ());
    else
        return M4_FALSE;
}

m4bool_t ClOperation_Register::IsUpdated (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
        return m_pClRegister->GetCurrentBlock ()->R_IsUpdated (m_pClRegister->GetMMIndex ());
    else
        return M4_FALSE;
}

m4bool_t ClOperation_Register::IsDeleted (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
        return m_pClRegister->GetCurrentBlock ()->R_IsDeleted (m_pClRegister->GetMMIndex ());
    else
        return M4_FALSE;
}

m4bool_t ClOperation_Register::IsNewAndDeleted (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
        return m_pClRegister->GetCurrentBlock ()->R_IsNewAndDeleted (m_pClRegister->GetMMIndex ());
    else
        return M4_FALSE;
}

m4bool_t ClOperation_Register::IsModifiedByDDBB (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
    {
        if (m_pClRegister->GetpNode_Data()->GetpNodeDef()->AffectsDB())
            return ((IsNew () || IsDeleted () || IsUpdatedByDDBB()) ? M4_TRUE : M4_FALSE); 
    }

    return M4_FALSE;
}

m4bool_t ClOperation_Register::IsNewByDDBB (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
        return ((m_pClRegister->GetpNode_Data()->GetpNodeDef()->AffectsDB() && IsNew ()) ? M4_TRUE : M4_FALSE);
    else
        return M4_FALSE;
}

m4bool_t ClOperation_Register::IsUpdatedByDDBB (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
        return m_pClRegister->GetCurrentBlock ()->R_IsUpdatedByDDBB (m_pClRegister->GetMMIndex ());
    else
        return M4_FALSE;
}

m4bool_t ClOperation_Register::IsDeletedByDDBB (void) const
{
    if (m_pClRegister->GetMMIndex () != ClMMIndex_EOF)
        return ((m_pClRegister->GetpNode_Data()->GetpNodeDef()->AffectsDB() && IsDeleted ()) ? M4_TRUE : M4_FALSE);
    else
        return M4_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
// ClOperation_RecordSet
//

ClOperation_RecordSet::ClOperation_RecordSet ()
{
    m_pClRecordSet = 0;
}

void ClOperation_RecordSet::Attach (_ClRecordSet_Base *ai_prs)
{
    M4_ASSERT (ai_prs);

    m_pClRecordSet = ai_prs;
}

ClOperation_RecordSet &ClOperation_RecordSet::operator = (ClOperation_RecordSet &ai_op)
{
    m_pClRecordSet = ai_op.m_pClRecordSet;

    return (*this);
}

m4return_t 
ClOperation_RecordSet::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(m_pClRecordSet);
	if (_ClOperation_Base::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

//********************************************************
// FUNCIONES COMUNES
//********************************************************

m4bool_t ClOperation_RecordSet::IsModified (void) const
{
    return ((m_pClRecordSet->GetCurrentBlock ()->B_IsModified () ||
        m_pClRecordSet->GetCurrentBlock ()->R_IsModified ()) ? M4_TRUE : M4_FALSE);
}

m4bool_t ClOperation_RecordSet::IsNew (void) const
{
    return ((m_pClRecordSet->GetCurrentBlock ()->B_IsNew () ||
        m_pClRecordSet->GetCurrentBlock ()->R_IsNew ()) ? M4_TRUE : M4_FALSE);
}

m4bool_t ClOperation_RecordSet::IsUpdated (void) const
{
    return ((m_pClRecordSet->GetCurrentBlock ()->B_IsUpdated () ||
        m_pClRecordSet->GetCurrentBlock ()->R_IsUpdated ()) ? M4_TRUE : M4_FALSE);
}

m4bool_t ClOperation_RecordSet::IsDeleted (void) const
{
    return ((m_pClRecordSet->GetCurrentBlock ()->B_IsDeleted () ||
        m_pClRecordSet->GetCurrentBlock ()->R_IsDeleted ()) ? M4_TRUE : M4_FALSE);
}

m4bool_t ClOperation_RecordSet::IsModifiedByDDBB (void) const
{
    return ((m_pClRecordSet->GetCurrentBlock ()->B_IsModifiedByDDBB () ||
        m_pClRecordSet->GetCurrentBlock ()->R_IsModifiedByDDBB ()) ? M4_TRUE : M4_FALSE);
}

m4bool_t ClOperation_RecordSet::IsNewByDDBB (void) const
{
    return ((m_pClRecordSet->GetCurrentBlock ()->B_IsNewByDDBB () ||
        m_pClRecordSet->GetCurrentBlock ()->R_IsNewByDDBB ()) ? M4_TRUE : M4_FALSE );
}

m4bool_t ClOperation_RecordSet::IsUpdatedByDDBB (void) const
{
    return ((m_pClRecordSet->GetCurrentBlock ()->B_IsUpdatedByDDBB () ||
        m_pClRecordSet->GetCurrentBlock ()->R_IsUpdatedByDDBB ()) ? M4_TRUE : M4_FALSE);
}

m4bool_t ClOperation_RecordSet::IsDeletedByDDBB (void) const
{
    return ((m_pClRecordSet->GetCurrentBlock ()->B_IsDeletedByDDBB () ||
        m_pClRecordSet->GetCurrentBlock ()->R_IsDeletedByDDBB ()) ? M4_TRUE : M4_FALSE);
}

// VRB de bloque (items de bloque)

m4bool_t ClOperation_RecordSet::BIsModified    (void) const
{
    return (m_pClRecordSet->GetCurrentBlock ()->B_IsModified ());
}
m4bool_t ClOperation_RecordSet::BIsNew         (void) const
{
    return (m_pClRecordSet->GetCurrentBlock ()->B_IsNew ());
}
m4bool_t ClOperation_RecordSet::BIsUpdated     (void) const
{
    return (m_pClRecordSet->GetCurrentBlock ()->B_IsUpdated ());
}
m4bool_t ClOperation_RecordSet::BIsDeleted     (void) const
{
    return (m_pClRecordSet->GetCurrentBlock ()->B_IsDeleted ());
}
m4bool_t ClOperation_RecordSet::BIsModifiedByDDBB (void) const
{
    return (m_pClRecordSet->GetCurrentBlock ()->B_IsModifiedByDDBB ());
}
m4bool_t ClOperation_RecordSet::BIsNewByDDBB      (void) const
{
    return (m_pClRecordSet->GetCurrentBlock ()->B_IsNewByDDBB ());
}
m4bool_t ClOperation_RecordSet::BIsUpdatedByDDBB  (void) const
{
    return (m_pClRecordSet->GetCurrentBlock ()->B_IsUpdatedByDDBB ());
}
m4bool_t ClOperation_RecordSet::BIsDeletedByDDBB  (void) const
{
    return (m_pClRecordSet->GetCurrentBlock ()->B_IsDeletedByDDBB ());
}

//////////////////////////////////////////////////////////////////////////////
//
// ClOperation_Node
//


ClOperation_Node::ClOperation_Node (void)
{
    m_pClNode_Data = 0;
}

m4bool_t ClOperation_Node::IsModified (void) const
{
    return ClOperation_User::IsModified ();
}

m4bool_t ClOperation_Node::IsNew (void) const
{
    return ClOperation_User::IsNew ();
}

m4bool_t ClOperation_Node::IsUpdated (void) const
{
    return ClOperation_User::IsUpdated ();
}

m4bool_t ClOperation_Node::IsDeleted (void) const
{
    return ClOperation_User::IsDeleted ();
}

m4bool_t ClOperation_Node::IsModifiedByDDBB (void) const
{
    return ClOperation_User::IsModifiedByDDBB ();
}

m4bool_t ClOperation_Node::IsNewByDDBB (void) const
{
    return ClOperation_User::IsNewByDDBB ();
}

m4bool_t ClOperation_Node::IsUpdatedByDDBB (void) const
{
    return ClOperation_User::IsUpdatedByDDBB ();
}

m4bool_t ClOperation_Node::IsDeletedByDDBB (void) const
{
    return ClOperation_User::IsDeletedByDDBB ();
}

// RWT de nodo (items de nodo)
m4bool_t ClOperation_Node::NIsModified    (void) const
{
//    return m_pClNode_Data->GetpRWT()->IsModified();
    return M4_FALSE;
}

m4bool_t ClOperation_Node::NIsNew(void) const
{
	return m_pClNode_Data->GetpRWT()->IsNew();
}

m4bool_t ClOperation_Node::NIsUpdated(void) const
{
	return m_pClNode_Data->GetpRWT()->IsModified();
}

m4bool_t ClOperation_Node::NIsDeleted     (void) const
{
	return m_pClNode_Data->GetpRWT()->IsDeleted();
}

m4bool_t ClOperation_Node::NIsModifiedByDDBB (void) const
{
//    return m_pClNode_Data->GetpRWT()->IsModifiedByDDBB();
    return M4_FALSE;
}

m4bool_t ClOperation_Node::NIsNewByDDBB      (void) const
{
//    return m_pClNode_Data->GetpRWT()->IsNewByDDBB();
    return M4_FALSE;
}

m4bool_t ClOperation_Node::NIsUpdatedByDDBB  (void) const
{
//    return m_pClNode_Data->GetpRWT()->IsUpdatedByDDBB();
    return M4_FALSE;
}

m4bool_t ClOperation_Node::NIsDeletedByDDBB  (void) const
{
//    return m_pClNode_Data->GetpRWT()->IsDeletedByDDBB();
    return M4_FALSE;
}

void       ClOperation_Node::Attach  (ClNode_Data *ai_pClNode_Data)
{
    m_pClNode_Data = ai_pClNode_Data;
}

m4return_t ClOperation_Node::GetSize (ClSize &ao_size)
{
    return M4_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
//
// ClOperation_User
//

m4return_t ClOperation_User::GetSize(ClSize &ao_size)
{
	ao_size += sizeof(m_Operation);
	return M4_SUCCESS;
}

