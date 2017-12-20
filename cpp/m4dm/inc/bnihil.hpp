//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             bnihil.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             07/09/97
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

#ifndef __BNIHIL_HPP__
#define __BNIHIL_HPP__
//## begin module.includes preserve=yes

#include "px_block.hpp"
//## end module.includes preserve=yes
//## begin module.additionalDeclarations preserve=yes

class ClBlock_Nihil;
class ClBlock_Nihil_Memory;
class ClBlockReference;
class ClGenericIODriver;
class ClNode_Kernel;


extern ClBlock_Nihil		M4_DECL_M4DM g_oNihilBlock;
// nuevo para la memoria
extern ClBlock_Nihil_Memory M4_DECL_M4DM g_oNihilBlock_Memory;




///////////////////////////////////////////////////////////////////////////////
//
// ClBlock_Nihil
//

//## end module.additionalDeclarations
class ClBlock_Nihil : public ClBlock_Channel_Proxy
{
public:
	ClBlock_Nihil ();
	~ClBlock_Nihil ();
	
	virtual m4bool_t IsProxy (void) const;
	virtual m4bool_t IsNihil (void) const { return M4_TRUE; }
	
	virtual m4bool_t IsValid (void) const;
	
	virtual m4uint32_t IncRef (void)
	{
		return 1;	//para que nunca se elimine
	}

	virtual m4uint32_t DecRef (void)
	{
		return 1;	//para que nunca se elimine
	}

	virtual m4bool_t IsValidProxy (void) const
	{
		return M4_TRUE;	//para que no haya lios
	}

	//devuelve cierto:
	virtual m4bool_t IsLoaded (void) const { return M4_TRUE; }
	
	virtual ClPartialLoadData * GetpPL_Data(void);
	//Ordenacion de indices para la carga parcial
	virtual m4return_t R_PrepareIndexPL (const m4bool_t ai_bUp, const m4uint32_t ai_PrevReg);
	
	virtual void GetParentBlock (ClBlockReference &ao_br);
	
	virtual ClBlock_Channel* _CreateBlock(void);


protected:
    ClBlock_Channel_Proxy *GetChildBlock (m4uint16_t ai_pos);

    virtual size_t Count (void);
    virtual m4return_t DeleteAll (void);

    m4return_t B_Undo(void);
    virtual m4bool_t R_Undo(ClMMIndex ai_regindex);

    virtual m4return_t CheckPoint (void);
    virtual m4return_t CheckPoint (ClMMIndex ai_regindex);

    virtual m4return_t SetCheckPointMode (m4bool_t ai_operation);
    virtual m4return_t GetCheckPointMode (m4bool_t &ai_operation);
    virtual m4return_t CommitPersisted (VersionMark &ai_version);
    virtual m4return_t RollbackPersisted (void);

    virtual void UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n);
    void UpdateParentInfoIndexOfMyRBs (void);

    virtual m4return_t DestroyBlock (m4bool_t ai_notify = M4_TRUE);
    virtual m4return_t DestroyAllRegisters (m4bool_t ai_notify = M4_TRUE);
    virtual m4return_t DestroyBlockNoUpdateAccesses(void);
    virtual m4return_t DestroyAllRegistersNoUpdateAccesses (void);


//*******************************************************************
// NUEVAS FUNCIONES DE OPERACION
//*******************************************************************

    virtual m4return_t R_GetStatus (const ClMMIndex ai_index, RegisterStatus_t &ao_riStatus);
    virtual m4return_t R_SetStatus (const ClMMIndex ai_index, const RegisterStatus_t ai_NewStatus);

    virtual m4bool_t R_IsModified    (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsNew         (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsUpdated     (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsDeleted     (const ClMMIndex ai_index) const;

    virtual m4bool_t R_IsModifiedByDDBB (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsNewByDDBB      (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsUpdatedByDDBB  (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsDeletedByDDBB  (const ClMMIndex ai_index) const;

    virtual m4bool_t R_IsModified    (void) const;
    virtual m4bool_t R_IsNew         (void) const;
    virtual m4bool_t R_IsUpdated     (void) const;
    virtual m4bool_t R_IsDeleted     (void) const;

    virtual m4bool_t R_IsModifiedByDDBB (void) const;
    virtual m4bool_t R_IsNewByDDBB      (void) const;
    virtual m4bool_t R_IsUpdatedByDDBB  (void) const;
    virtual m4bool_t R_IsDeletedByDDBB  (void) const;

    virtual m4bool_t B_IsModified    (void) const;
    virtual m4bool_t B_IsNew         (void) const;
    virtual m4bool_t B_IsUpdated     (void) const;
    virtual m4bool_t B_IsDeleted     (void) const;

    virtual m4bool_t B_IsModifiedByDDBB (void) const;
    virtual m4bool_t B_IsNewByDDBB      (void) const;
    virtual m4bool_t B_IsUpdatedByDDBB  (void) const;
    virtual m4bool_t B_IsDeletedByDDBB  (void) const;

    virtual m4bool_t R_GetNewNotPersist(ClMMIndex ai_regindex) const;
    virtual m4return_t R_SetNewNotPersist(ClMMIndex ai_regindex, const m4bool_t ai_bNewNotPersist);

//*******************************************************************
//*******************************************************************

    virtual m4bool_t R_IsValid    (ClMMIndex ai_index);

    virtual m4return_t R_SetUpdated (ClMMIndex ai_index);

    virtual m4return_t R_GetVersion (ClMMIndex ai_index, VersionMark &ai_vm);
    virtual m4return_t R_SetVersion (ClMMIndex ai_index, VersionMark &ai_vm);
    virtual m4return_t R_GetRecordPersist (ClMMIndex ai_index, m4bool_t &ai_persist);
    virtual m4return_t R_SetRecordPersist (ClMMIndex ai_index, m4bool_t ai_persist);

    virtual m4return_t B_GetPersist (m4bool_t &ai_persist);
    virtual m4return_t B_SetPersist (m4bool_t ai_persist);

    virtual m4return_t B_AddRegister (ClMMIndex &ao_newindex);
    virtual m4return_t R_InsertRegister (ClMMIndex ai_regindex);
    virtual m4return_t R_DeleteRegister (ClMMIndex ai_regindex);
    virtual m4return_t R_Destroy (ClMMIndex ai_regindex);


public:
    // Copy/Paste de una rama de una canal utilizando serializacion
    virtual m4return_t CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClGenericIODriver& IOD);

    virtual m4return_t PasteRecordWithDeSerialize(m4uint32_t&        aio_iRecordIndex,
						  const m4bool_t     ai_bKeepMarks,
						  const m4bool_t     ai_bDestroyRecords,
						  const m4char_t*    ai_vpExclusionNodeName[],
						  const m4uint16_t   ai_iNumExclusionNodes,
						  ClGenericIODriver& IOD);

    virtual m4return_t CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClFileName& ao_oFileName);

    virtual m4return_t PasteRecordWithDeSerialize(m4uint32_t&       aio_iRecordIndex,
						  const m4bool_t    ai_bKeepMarks,
						  const m4bool_t    ai_bDestroyRecords,
						  const m4char_t*   ai_vpExclusionNodeName[],
						  const m4uint16_t  ai_iNumExclusionNodes,
						  const ClFileName& ai_oFileName,
						  const m4bool_t    ai_bDeleteFile);

friend class ClNode_Kernel;
friend class ClRecordSet;
friend class _ClRegister_Base;
friend class ClItemProxy_Register;
friend class ClItemProxy_Block;
friend class ClItemProxy_Node;
};



//*******************************************************************************
// BOLQUE NIHIL ESPECIAL PARA EL CASO DE QUE NO SE CREE BLOQUE POR MAXNUMBLOCKS
//*******************************************************************************

class ClBlock_Nihil_Memory : public ClBlock_Channel_Proxy
{
public:
    ClBlock_Nihil_Memory ();
   ~ClBlock_Nihil_Memory ();

    virtual m4bool_t IsProxy (void) const;
    virtual m4bool_t IsNihil (void) const { return M4_TRUE; }

    virtual m4bool_t IsValid (void) const;

	virtual m4uint32_t IncRef (void)
	{
		return 1;	//para que nunca se elimine
	}

	virtual m4uint32_t DecRef (void)
	{
		return 1;	//para que nunca se elimine
	}

	virtual m4bool_t IsValidProxy (void) const
	{
		return M4_TRUE;	//para que no haya lios
	}

    //devuelve cierto:
    virtual m4bool_t IsLoaded (void) const { return M4_TRUE; }

    virtual ClPartialLoadData * GetpPL_Data(void);
    //Ordenacion de indices para la carga parcial
	virtual m4return_t R_PrepareIndexPL (const m4bool_t ai_bUp, const m4uint32_t ai_PrevReg);

	virtual void GetParentBlock (ClBlockReference &ao_br);

	virtual ClBlock_Channel* _CreateBlock(void);


protected:
    ClBlock_Channel_Proxy *GetChildBlock (m4uint16_t ai_pos);

    virtual size_t Count (void);
    virtual m4return_t DeleteAll (void);

    m4return_t B_Undo(void);
    virtual m4bool_t R_Undo(ClMMIndex ai_regindex);

    virtual m4return_t CheckPoint (void);
    virtual m4return_t CheckPoint (ClMMIndex ai_regindex);

    virtual m4return_t SetCheckPointMode (m4bool_t ai_operation);
    virtual m4return_t GetCheckPointMode (m4bool_t &ai_operation);
    virtual m4return_t CommitPersisted (VersionMark &ai_version);
    virtual m4return_t RollbackPersisted (void);

    virtual void UpdateIndex (m4uint32_t *ai_pindex_qtc, m4uint32_t ai_n);
    void UpdateParentInfoIndexOfMyRBs (void);

    virtual m4return_t DestroyBlock (m4bool_t ai_notify = M4_TRUE);
    virtual m4return_t DestroyAllRegisters (m4bool_t ai_notify = M4_TRUE);
    virtual m4return_t DestroyBlockNoUpdateAccesses(void);
    virtual m4return_t DestroyAllRegistersNoUpdateAccesses (void);

    m4return_t R_GetAddress_andhasslices (const m4uint16_t ai_ItemPos, ClMMIndex ai_index, void **ao_address, m4bool_t &ao_hasslices) { return M4_ERROR; }

//*******************************************************************
// NUEVAS FUNCIONES DE OPERACION
//*******************************************************************

    virtual m4return_t R_GetStatus (const ClMMIndex ai_index, RegisterStatus_t &ao_riStatus);
    virtual m4return_t R_SetStatus (const ClMMIndex ai_index, const RegisterStatus_t ai_NewStatus);

    virtual m4bool_t R_IsModified    (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsNew         (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsUpdated     (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsDeleted     (const ClMMIndex ai_index) const;

    virtual m4bool_t R_IsModifiedByDDBB (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsNewByDDBB      (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsUpdatedByDDBB  (const ClMMIndex ai_index) const;
    virtual m4bool_t R_IsDeletedByDDBB  (const ClMMIndex ai_index) const;

    virtual m4bool_t R_IsModified    (void) const;
    virtual m4bool_t R_IsNew         (void) const;
    virtual m4bool_t R_IsUpdated     (void) const;
    virtual m4bool_t R_IsDeleted     (void) const;

    virtual m4bool_t R_IsModifiedByDDBB (void) const;
    virtual m4bool_t R_IsNewByDDBB      (void) const;
    virtual m4bool_t R_IsUpdatedByDDBB  (void) const;
    virtual m4bool_t R_IsDeletedByDDBB  (void) const;

    virtual m4bool_t B_IsModified    (void) const;
    virtual m4bool_t B_IsNew         (void) const;
    virtual m4bool_t B_IsUpdated     (void) const;
    virtual m4bool_t B_IsDeleted     (void) const;

    virtual m4bool_t B_IsModifiedByDDBB (void) const;
    virtual m4bool_t B_IsNewByDDBB      (void) const;
    virtual m4bool_t B_IsUpdatedByDDBB  (void) const;
    virtual m4bool_t B_IsDeletedByDDBB  (void) const;

//*******************************************************************
//*******************************************************************

    virtual m4bool_t R_IsValid    (ClMMIndex ai_index);

    virtual m4return_t R_SetUpdated (ClMMIndex ai_index);

    virtual m4return_t R_GetVersion (ClMMIndex ai_index, VersionMark &ai_vm);
    virtual m4return_t R_SetVersion (ClMMIndex ai_index, VersionMark &ai_vm);
    virtual m4return_t R_GetRecordPersist (ClMMIndex ai_index, m4bool_t &ai_persist);
    virtual m4return_t R_SetRecordPersist (ClMMIndex ai_index, m4bool_t ai_persist);

    virtual m4return_t B_GetPersist (m4bool_t &ai_persist);
    virtual m4return_t B_SetPersist (m4bool_t ai_persist);

    virtual m4return_t B_AddRegister (ClMMIndex &ao_newindex);
    virtual m4return_t R_InsertRegister (ClMMIndex ai_regindex);
    virtual m4return_t R_DeleteRegister (ClMMIndex ai_regindex);
    virtual m4return_t R_Destroy (ClMMIndex ai_regindex);

public:
    // Copy/Paste de una rama de una canal utilizando serializacion
    virtual m4return_t CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClGenericIODriver& IOD);

    virtual m4return_t PasteRecordWithDeSerialize(m4uint32_t&        aio_iRecordIndex,
						  const m4bool_t     ai_bKeepMarks,
						  const m4bool_t     ai_bDestroyRecords,
						  const m4char_t*    ai_vpExclusionNodeName[],
						  const m4uint16_t   ai_iNumExclusionNodes,
						  ClGenericIODriver& IOD);

    virtual m4return_t CopyRecordWithSerialize(const m4uint32_t ai_iRecordIndex, ClFileName& ao_oFileName);

    virtual m4return_t PasteRecordWithDeSerialize(m4uint32_t&       aio_iRecordIndex,
                                                  const m4bool_t    ai_bKeepMarks,
                                                  const m4bool_t    ai_bDestroyRecords,
                                                  const m4char_t*   ai_vpExclusionNodeName[],
                                                  const m4uint16_t  ai_iNumExclusionNodes,
                                                  const ClFileName& ai_oFileName,
                                                  const m4bool_t    ai_bDeleteFile);

friend class ClNode_Kernel;
friend class ClRecordSet;
friend class _ClRegister_Base;
friend class ClItemProxy_Register;
friend class ClItemProxy_Block;
friend class ClItemProxy_Node;
};

#endif // __BNIHIL_HPP__ 
