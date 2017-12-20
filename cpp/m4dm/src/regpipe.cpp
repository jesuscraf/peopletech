//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             RegPipe.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             03/03/98
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

#include "regpipe.hpp"
#include "aregister.hpp"
#include <string.h>

ClRegister_Pipe::ClRegister_Pipe(void)
{
	m_ppItemsPointer = NULL;
	m_NumItem = 0;
	m_pItemsPosition = NULL;
	m_pItemsHandle = NULL;
	m_pNumericsItems = NULL;
	m_pItemsLengthPointer = NULL;
	m_bIsReady = M4_FALSE;
	m_InitItem=-1;
	m_EndItem=-1;
    m_MaxRecords = M4_FALSE;
}


m4return_t ClRegister_Pipe::InitPipe(m4int32_t ai_NumItem)
{
	m_NumItem = ai_NumItem;

	m_ppItemsPointer = new m4pchar_t [ai_NumItem];
	memset (m_ppItemsPointer, '\0', m_NumItem * sizeof (m4pchar_t));

	m_pItemsPosition = new m4uint16_t[ai_NumItem];
	memset (m_pItemsPosition, '\0', m_NumItem * sizeof (m4int16_t));

	m_pItemsHandle = new ClHandle [ai_NumItem];

	m_pNumericsItems = new m4bool_t [ai_NumItem];
	memset (m_pNumericsItems, '\0', m_NumItem * sizeof (m4bool_t));

	m_pItemsLengthPointer = new m4pint32_t[ai_NumItem];
	memset (m_pItemsLengthPointer, '\0', m_NumItem * sizeof (m4pint32_t));

	m_NumItem = 0;

    return M4_SUCCESS;
}

m4return_t ClRegister_Pipe::DestroyPipe(void)
{
	delete []m_ppItemsPointer;
	delete []m_pItemsPosition;
	delete []m_pItemsHandle;
	delete []m_pNumericsItems;
	delete []m_pItemsLengthPointer;

	return M4_SUCCESS;
}

m4return_t ClRegister_Pipe::CreateItemInPipe(m4int32_t ai_ItemIndex, m4pint32_t ai_ItemLengthPointer, m4int32_t ai_ItemSize, m4uint32_t ai_hItem, m4bool_t ai_bIsNum)
{
	m_ppItemsPointer[ai_ItemIndex] = new m4char_t [ai_ItemSize];
	//Bug 0077794
	//Nos protegemos si no hemos podido reservar memoria.
	//La BDL cldinarr.cpp llama a este miembro..
	if (m_ppItemsPointer[ai_ItemIndex] == NULL) {
		DUMP_CHLOG_ERRORF(M4_CH_DM_INTERNAL_ERROR, __FILE__ << __LINE__ << "CreateItemInPipe" );
		return M4_ERROR;
	}

	m_pItemsHandle[ai_ItemIndex] = ai_hItem;
	m_pNumericsItems[ai_ItemIndex] = ai_bIsNum;
	m_pItemsLengthPointer[ai_ItemIndex] = ai_ItemLengthPointer;
	memset (m_ppItemsPointer[ai_ItemIndex], '\0', ai_ItemSize);

    m_NumItem ++;

	return M4_SUCCESS;
}

m4pchar_t ClRegister_Pipe::GetItemAddress(m4int32_t ai_ItemIndex)
{
	return m_ppItemsPointer[ai_ItemIndex];
}

// UNIOCDE DB
m4pchar_t* ClRegister_Pipe::GetItemAddressPointer(m4int32_t ai_ItemIndex)
{
	return &m_ppItemsPointer[ai_ItemIndex];
}

m4pint32_t ClRegister_Pipe::GetItemLength(m4int32_t ai_ItemIndex)
{
	return m_pItemsLengthPointer[ai_ItemIndex];
}

ClHandle * ClRegister_Pipe::GetItemHandle(m4int32_t ai_ItemIndex)
{
	return &m_pItemsHandle[ai_ItemIndex];
}

m4bool_t ClRegister_Pipe::NumericItem(m4int32_t ai_ItemIndex)
{
	return m_pNumericsItems[ai_ItemIndex];
}

m4uint32_t ClRegister_Pipe::GetNumItem()
{
	return m_NumItem;
}

m4uint16_t ClRegister_Pipe::GetItemPos(m4int32_t ai_ItemIndex)
{
	return m_pItemsPosition[ai_ItemIndex];
}

m4return_t ClRegister_Pipe::SetItemPos(m4int32_t ai_ItemIndex, m4uint16_t ai_iPos)
{
	m_pItemsPosition[ai_ItemIndex] = ai_iPos;
	return M4_SUCCESS;
}

m4return_t ClRegister_Pipe::GetIsReady(m4bool_t &ao_bIsReady)
{
	ao_bIsReady = m_bIsReady;
	return M4_SUCCESS;
}

m4return_t ClRegister_Pipe::SetIsReady(m4bool_t ai_bIsReady)
{
	m_bIsReady = ai_bIsReady;
	return M4_SUCCESS;
}

m4return_t ClRegister_Pipe::SetLotOfItems(_ClRegister_Base * ai_pReg,m4int32_t ai_InitItem, m4int32_t ai_EndItem,m4bool_t ai_bAddReg)
{
   	if (ai_pReg){
		m_InitItem = ai_InitItem;
		m_EndItem = ai_EndItem;
	    return ai_pReg->SetLotOfItems(this, ai_bAddReg);
	}
	return M4_ERROR;
}

m4return_t ClRegister_Pipe::SetLotOfItemsPartial (_ClRegister_Base * ai_pReg,m4int32_t ai_InitItem, m4int32_t ai_EndItem,m4bool_t ai_bAddReg)
{
	if (ai_pReg){
		m_InitItem = ai_InitItem;
		m_EndItem = ai_EndItem;
		return ai_pReg->SetLotOfItemsPartial(this, ai_bAddReg);
	}
	return M4_ERROR;
}
