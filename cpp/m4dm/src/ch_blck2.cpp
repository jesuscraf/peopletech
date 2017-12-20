//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             ch_block.inl
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             06/09/97
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

#include "ch_block.hpp"


//-----------------------------------------------------------------

//******************************************************************************
//  FUNCIONES DE OPERACION
//******************************************************************************


m4return_t ClBlock_Channel::R_GetStatus (const ClMMIndex ai_index, RegisterStatus_t &ao_riStatus)
{
    ao_riStatus = Records.RecordStatus(ai_index);
	return M4_SUCCESS;
}

m4return_t ClBlock_Channel::R_SetStatus (const ClMMIndex ai_index, const RegisterStatus_t ai_NewStatus)
{
    return Records.SetStatus(ai_index, ai_NewStatus);
}

m4bool_t ClBlock_Channel::R_IsModified (const ClMMIndex ai_index) const
{
    RegisterStatus_t status = Records.RecordStatus (ai_index);
//    bismodified = ( (status != M4_RS_NORMAL && status != M4_RS_NEW_AND_DELETED) ? M4_TRUE : M4_FALSE);
	// El NewAndDeleted debía estar aqui, para que esos registros no se persistieran.
	// Ahora se controla arriba.
    m4bool_t bismodified = ( (status != M4_RS_NORMAL ) ? M4_TRUE : M4_FALSE);
    return bismodified;
}

m4bool_t ClBlock_Channel::R_IsNew(const ClMMIndex ai_index) const
{
	return Records.IsRecordNew(ai_index);
}

m4bool_t ClBlock_Channel::R_IsUpdated(const ClMMIndex ai_index) const
{
	return Records.IsRecordModified(ai_index);
}

m4bool_t ClBlock_Channel::R_IsDeleted(const ClMMIndex ai_index) const
{
	return Records.IsRecordDeleted(ai_index);
}

m4bool_t ClBlock_Channel::R_IsUpdatedByDDBB(const ClMMIndex ai_index) const
{
	return Records.IsRecordModifiedInDB(ai_index);
}

m4bool_t ClBlock_Channel::B_IsModified (void) const
{
    m4bool_t bismodified;

    m4uint8_t status;
    Block.Status(status);
    bismodified = (status != VS_NORMAL) ? M4_TRUE : M4_FALSE;

    return bismodified;
}

m4bool_t ClBlock_Channel::B_IsNew(void) const
{
	return Block.IsNew();
}

m4bool_t ClBlock_Channel::B_IsUpdated(void) const
{
	return Block.IsModified();
}

m4bool_t ClBlock_Channel::B_IsDeleted(void) const
{
	return Block.IsDeleted();
}

m4bool_t ClBlock_Channel::B_IsUpdatedByDDBB(void) const
{
	return Block.IsModifiedInDB();
}

m4bool_t ClBlock_Channel::R_IsModified (void) const
{
    m4uint8_t status;
    Records.Status(status);

    m4bool_t bismodified = ( (status != VS_NORMAL) ? M4_TRUE : M4_FALSE );

    if (!bismodified) {        
        Block.Status(status);
        bismodified = (status != VS_NORMAL) ? M4_TRUE : M4_FALSE;
    }

    return bismodified;
}

m4bool_t ClBlock_Channel::R_IsNew(void) const
{
	return Records.IsNew();
}

m4bool_t ClBlock_Channel::R_IsUpdated(void) const
{
	return Records.IsModified();
}

m4bool_t ClBlock_Channel::R_IsDeleted(void) const
{
	return Records.IsDeleted();
}
m4bool_t ClBlock_Channel::R_IsNewAndDeleted(const ClMMIndex ai_index) const
{
	m4uint8_t status = Records.RecordStatus(ai_index);
	
	//parte nueva:
	return (status == M4_RS_NEW_AND_DELETED)? M4_TRUE : M4_FALSE;
}

m4bool_t ClBlock_Channel::R_IsUpdatedByDDBB(void) const
{
	return Records.IsModifiedInDB();
}

m4return_t ClBlock_Channel::B_RefreshFlags   (void)
{
    m4return_t res = Block.RefreshStatus();
    res |= Records.RefreshStatus();

    return res;
}

//******************************************************************************
//******************************************************************************
