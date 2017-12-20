//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:               VMachin1.h
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
// 
//
// Definition:
//
//    Virtual machine de nivel 1 funciones de filtros
////
//==============================================================================

#ifndef _ClVM1CMCR_H
#define _ClVM1CMCR_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"

class ClNoCallableItem;

class ClVM1Mcr : public ClVVM{
	
	const m4VMState_t &m_oState1;
	ClVMachine1	*m_poVM;

public:

	ClVM1Mcr (const m4VMState_t &ai_oState):	m_oState1(ai_oState)
	{
	}

	void SetVM (ClVMachine1 *ai_poVM) {m_poVM = ai_poVM;}

    //----------------------------------------
    //MetaCanal
    //----------------------------------------
    m4return_t GetChannelId					(void);
    m4return_t GetChannelNumberOfNodes		(void);

	m4return_t GetThisNodeId				(void);
	m4return_t GetNodeNumberOfItems			(void);
	m4return_t GetNodeIndex					(void);
	m4return_t GetNodeItemId				(void);
	m4return_t GetNodeItemType				(void);
	m4return_t GetNodeItemScope				(void);
	m4return_t GetNodeItemSliceTotalize		(void);
	m4return_t GetNodeItemM4Type			(void);
	m4return_t GetNodeItemPrecision			(void);
	m4return_t GetNodeItemIndex				(void);
	m4return_t ExistNodeItemIndex			(void);
	m4return_t GetNodeItemScale				(void);
	m4return_t GetNodeItemInternalType		(void);
	m4return_t GetNodeItemReadObjectAlias	(void);
	m4return_t GetNodeItemWriteObjectAlias	(void);
	m4return_t GetNodeItemReadFieldId		(void);
	m4return_t GetNodeItemWriteFieldId		(void);
	m4return_t GetNodeItemReadObjectId		(void);
	m4return_t GetNodeItemWriteObjectId		(void);
	
	m4return_t GetChannelData				(void);
	m4return_t GetNodeData					(void);
	m4return_t GetItemData					(void);
	m4return_t GetRuleData					(void);

	m4return_t GetNodeItemPrecisionEx		(void);
	m4return_t GetNodeItemArgumentPos		(void);
	m4return_t GetNodeItemArgumentNumber	(void);
	m4return_t GetNodeItemArgument			(void);

	m4return_t GetNodeReadObjectId			(void);
	m4return_t GetNodeWriteObjectId			(void);

	m4return_t GetThisItemId				(void);

	m4return_t GetListInfo					(void);

private:
	//--------------------------------------------------------------------------
	//funciones internas para obtener datos de la definicion de un Item
	//--------------------------------------------------------------------------
	m4return_t	_GetChannelData	( m4int8_t ai_ChannelDefData,					m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError );
	m4return_t	_GetNodeData	( m4int8_t ai_NodeDefData,						m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError );
	m4return_t	_GetItemData	( m4int8_t ai_ItemDefData, m4uint16_t ai_iRule, m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError );

	// Find Node Item by (Pos & Scope) or by (Id)
	m4bool_t	_FindNodeItemByPosScope	( ClAccess *ai_poAccess,	const m4VariantType& ai_vNodo,  m4uint32_t ai_iPos, m4uint8_t ai_iScope,	ClNoCallableItem& ao_rItItem, m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError );
	m4bool_t	_FindNodeItemById		( ClAccess *ai_poAccess,	const m4VariantType& ai_vNodo,  m4pcchar_t ai_pcItemID,						ClNoCallableItem& ao_rItItem, m4pcchar_t ai_pccMethodName, m4bool_t ai_bDumpError );

};

#endif

