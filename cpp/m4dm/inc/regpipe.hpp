//==============================================================================
//
// (c); Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             RegPipe.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             03/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN);
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef _REGPIPE_HPP_
#define _REGPIPE_HPP_

//#include "m4types.hpp"

// Este include no se puede quitar, porque la definición de ClHandle es distinta
// en Debug que en Release.
#include "handles.hpp"

#ifndef _M4CHCOMMON_HPP_
#include "m4types.hpp"
#endif

class _ClRegister_Base;

class M4_DECL_M4DM ClRegister_Pipe
{
public:

	ClRegister_Pipe(void);

	m4return_t InitPipe(m4int32_t ai_NumItem);

	m4return_t DestroyPipe(void);

	m4return_t CreateItemInPipe(m4int32_t ai_ItemIndex, m4pint32_t ai_ItemLengthPointer, m4int32_t ai_ItemSize, m4uint32_t ai_hItem, m4bool_t ia_bIsNum = M4_FALSE);

	m4pchar_t GetItemAddress(m4int32_t ai_ItemIndex);

	m4pchar_t* GetItemAddressPointer(m4int32_t ai_ItemIndex);

	m4uint16_t GetItemPos(m4int32_t ai_ItemIndex);

	m4return_t GetIsReady(m4bool_t &ao_bIsReady);

	m4return_t SetIsReady(m4bool_t ai_bIsReady);

	m4return_t SetItemPos(m4int32_t ai_ItemIndex, m4uint16_t ai_iPos);

	m4pint32_t GetItemLength(m4int32_t ai_ItemIndex);

	ClHandle * GetItemHandle(m4int32_t ai_ItemIndex);

	m4bool_t NumericItem(m4int32_t ai_ItemIndex);

	m4uint32_t GetNumItem();

	m4int32_t GetInitItem(){
		return m_InitItem;
	}
	m4int32_t GetEndItem(){
		return m_EndItem;
	}

protected:

    m4return_t SetLotOfItems(_ClRegister_Base * ai_pReg, m4int32_t ai_InitItem, m4int32_t ai_EndItem, m4bool_t ai_bAddReg = M4_TRUE);
	m4return_t SetLotOfItemsPartial (_ClRegister_Base * ai_pReg, m4int32_t ai_InitItem, m4int32_t ai_EndItem, m4bool_t ai_bAddReg = M4_FALSE);
    //En carga parcial debo inicializar el registro

public:
	m4pchar_t * m_ppItemsPointer;
	m4pchar_t Pipebuffer;
	m4int32_t m_NumItem;
	m4pint32_t * m_pItemsLengthPointer;
	m4uint16_t * m_pItemsPosition;
	ClHandle * m_pItemsHandle;
	m4bool_t * m_pNumericsItems;
	m4bool_t m_bIsReady, //Me dice si ya se han rellenado los campos de handle,  pos, etc.
        m_MaxRecords; // si se alcanza el límite de registros
	m4int32_t m_InitItem;
	m4int32_t m_EndItem;


friend class ClLoad;
friend class ClPersist;
};

#endif
