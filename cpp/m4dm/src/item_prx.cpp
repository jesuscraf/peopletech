//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             itnd_prx.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             08/09/97
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

//----------------------------------------------------------------------------
#include "dm_def.hpp"
#include "m4define.hpp"
#include "dmres.hpp"
//----------------------------------------------------------------------------

#include "item_prx.hpp"
#include "px_block.hpp"
#include "ch_block.hpp"
#include "dsitem.hpp"
#include "register.hpp"
#include "arecset.hpp"
#include "node_knl.hpp"
#include "chan_knl.hpp"
#include "sizerutils.hpp"
#include "chlog.hpp"
#include "access.hpp"
#include "m4mdrt.hpp"
#include "wproxy.hpp"
#include "v_contxt.hpp"
#include "clbsearch.hpp"

// instancia nula de un item proxy:
M4_DECL_M4DM _ClItemProxy_Nihil s_oClItemProxy_Nihil;

// instanciación o lo que sea de variables estáticas:
_ClSlices_Fixed _AClItemProxy::ms_pFLValue_Kernel;
_ClSlices_Variable _AClItemProxy::ms_pVLValue_Kernel;

_ClVirtual_Value_Nihil _ClItemProxy_Nihil::ms_vvnihil;

ClValue_Write		_AClItemProxy::ms_pClValueWrite;
ClValue_Write_Nihil _AClItemProxy::ms_pClValueWriteNihil;
ClValue_Read		_AClItemProxy::ms_pClValueRead;
ClValue_Read_Nihil  _AClItemProxy::ms_pClValueReadNihil;

///////////////////////////////////////////////////////////////////////////////
//
// _AClItemProxy
//

// gestión de memoria especializada, utilizamos un pool, en vez de alocar las
// instancias ItemInfo de una en una

void *_AClItemProxy::operator new (size_t, void *ai_pbuffer)
{
    return ai_pbuffer;  // dirección del pool
}

void _AClItemProxy::operator delete (void *)
{
}

m4return_t _AClItemProxy::Init (ClNodeDirectory_Data &ai_nodedir, ClCompiledMCR *ai_pcmcr, ClHandle ai_hitem)
{
    M.Handle = ai_hitem;

    //información de conexion:
    return M.ConnectItemInfo.Init (ai_nodedir, ai_pcmcr, ai_hitem);
}

m4return_t _AClItemProxy::InitValue (ClCompiledMCR *ai_pcmcr, m4bool_t ai_bsenable)
{
	if (ai_pcmcr->GetItemIsVariableLength (M.Handle)) {
		M.pKernel = &ms_pVLValue_Kernel;    // longitud variable
	}
	else {
		M.pKernel = &ms_pFLValue_Kernel;    // longitud fija
	}

    return InitSecurity (ai_pcmcr);
}

m4return_t _AClItemProxy::InitSecurity (ClCompiledMCR *ai_pcmcr)
{
	if (ai_pcmcr->GetItemSecUpdate(M.Handle))
		M.pWrite = &ms_pClValueWrite;
	else
		M.pWrite = &ms_pClValueWriteNihil;

	if (ai_pcmcr->GetItemSecRead(M.Handle))
		M.pRead = &ms_pClValueRead;
	else
		M.pRead = &ms_pClValueReadNihil;

	return M4_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// ClItemProxy_Block
//

// Metodo de validacion para comprobar que estamos bien posicionados.
m4return_t ClItemProxy_Block::Validation (const StKernelContext &ai_context, m4uint32_t ai_Offset)
{
    M4_ASSERT (ai_context.BlockReference);

    if (ai_context.BlockReference->IsNihil())
    {
        m4uint32_t ItemHandle  = ai_context.pItemDef_Interface->GetHandle();
        ClCompiledMCR * pmcr   = ai_context.pItemDef_Interface->GetpCMCR ();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_VALIDATION_NIHILBLOCK + ai_Offset,
            M4ObjNodeItemI(pmcr, ItemHandle));

        return M4_ERROR;
    }
    else
    {
        return M4_SUCCESS;
    }
}

m4return_t ClItemProxy_Block::GetDescription (const StKernelContext &ai_context, const void *&oi_Item) const
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.GetItemBlobFileDescription (M.iPos, 0, oi_Item);
}

m4return_t ClItemProxy_Block::GetAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices) const
{
	m4return_t	iResult = M4_SUCCESS ;

	*ao_address = NULL ;
	ao_hasslices = M4_FALSE ;

	if (!ai_context.BlockReference->m_pClBlock) {
		//------------------
		//truco:
		ClHandle hc = ai_context.BlockReference->GetpNode_Data()->GetpNodeDef()->GetpCMCR()->GetItemInverseConnector(M.Handle);
		
		if (hc) {
			ClNoCallableItem item (M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK);
			//item.Attach ();
			item.SetCurrentBlock(ai_context.BlockReference.GetpProxy());
			
			item.MoveTo(M.Handle);
			
			ClNoCallableItem itemparent;

			/* Bug 0114798
			Hay que pasar el error hacia arriba
			*/
			iResult = item.InitAsMyParent( itemparent ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			/* Bug 0275637
			Se comprueban los tipos cpp y no m4
			*/
			if (itemparent.ItemDef.CppType() == item.ItemDef.CppType()) {
				*ao_address = itemparent.Value.Get();
				
				//sin tramos
				//ver comentario en B_GetHasSlices, px_block.cpp:
				ao_hasslices = M4_FALSE;
				
				return M4_SUCCESS;
			}
		}

		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.GetItemDataAddressByPos_AndHasSlices(M.iPos, 0, *ao_address, ao_hasslices);
}

m4return_t ClItemProxy_Block::GetSize (const StKernelContext &ai_context, m4uint32_t &ao_size) const
{

	m4return_t	iResult = M4_SUCCESS ;

	ao_size = 0 ;


	if (!ai_context.BlockReference->m_pClBlock) {
		//------------------
		//truco:
		ClHandle hc = ai_context.BlockReference->GetpNode_Data()->GetpNodeDef()->GetpCMCR()->GetItemInverseConnector(M.Handle);
		
		if (hc != 0) {
			ClNoCallableItem item (M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK);
			//item.Attach ();
			item.SetCurrentBlock (ai_context.BlockReference.GetpProxy());
			
			item.MoveTo (M.Handle);
			
			ClNoCallableItem itemparent;

			/* Bug 0114798
			Hay que pasar el error hacia arriba
			*/
			iResult = item.InitAsMyParent( itemparent ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			/* Bug 0275637
			Se comprueban los tipos cpp y no m4
			*/
			if (itemparent.ItemDef.CppType() == item.ItemDef.CppType()) {
				ao_size = itemparent.Value.Size ();
				return M4_SUCCESS;
			}
		}
		
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.GetItemSizeByPos (M.iPos, 0, ao_size);
}

m4return_t ClItemProxy_Block::SetValueByOffset (const StKernelContext &ai_context, const void* oi_Item, const m4uint32_t ai_Size, m4uint32_t ai_offset, m4uint32_t *ao_pretsize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) const
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}

	return ai_context.BlockReference->m_pClBlock->Block.SetItemValueByPosAndOffset(M.iPos, oi_Item, 0, ai_offset, ai_Size, ao_pretsize, ai_bChangeStatus, ai_bIsUser);
}

m4return_t ClItemProxy_Block::SetDescription (const StKernelContext &ai_context, const void* oi_Item) const
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.SetItemBlobFileDescription(M.iPos, oi_Item, 0);
}

m4return_t ClItemProxy_Block::SetSize (const StKernelContext &ai_context, m4uint32_t ai_newsize) const
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.SetItemSizeByPos(M.iPos, 0, ai_newsize);
}

m4return_t ClItemProxy_Block::GetOldAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices) const
{
	if (ai_context.BlockReference->m_pClBlock) {
		return ai_context.BlockReference->m_pClBlock->Block.GetLastItemDataAddressByPos_AndHasSlices (M.iPos, 0, *ao_address, ao_hasslices);
	}
	
	//si no hay bloque, no se ha podido hacer una asignación sobre el item => no tiene oldvalue
	//ni de coña: devolvemos 0
	
	*ao_address = 0;
	ao_hasslices = M4_FALSE;
	return M4_SUCCESS;
}

m4return_t ClItemProxy_Block::GetOldSize (const StKernelContext &ai_context, m4uint32_t &ao_size) const
{
	if (ai_context.BlockReference->m_pClBlock) {
		return ai_context.BlockReference->m_pClBlock->Block.GetLastItemSizeByPos (M.iPos, 0, ao_size);
	}
	
	//si no hay bloque, no se ha podido hacer una asignación sobre el item => no tiene oldvalue
	//ni de coña: devolvemos 0
	
	ao_size = 0;
	return M4_SUCCESS;
}

m4return_t ClItemProxy_Block::CreateTempFile(const StKernelContext &ai_context, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension)
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	
	ClDsItemBlobFile* IBF = ai_context.BlockReference->m_pClBlock->Block.GetItemBlobFile(M.iPos, 0);
	if (!IBF) return M4_ERROR;

	return IBF->CreateTempFile(ai_pDirectory, ai_pExtension);
}

m4return_t ClItemProxy_Block::ChangedMask(const StKernelContext &ai_context, m4uint32_t & ao_oChangedMask)
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}

	ClDsItemBlobFile* IBF = ai_context.BlockReference->m_pClBlock->Block.GetItemBlobFile(M.iPos, 0);
	if (!IBF) return M4_ERROR;

	ClComparisonResultMask CRM;
	IBF->ChangedMask(CRM);
	ao_oChangedMask = (m4uint32_t) CRM;
	return M4_SUCCESS;
}

m4return_t ClItemProxy_Block::IsNull (const StKernelContext &ai_context, m4bool_t &ao_isnull)
{

	m4return_t	iResult = M4_SUCCESS ;

	ao_isnull = M4_TRUE ;


	if (!ai_context.BlockReference->m_pClBlock) {
		//------------------
		//truco:
		ClHandle hc = ai_context.BlockReference->GetpNode_Data()->GetpNodeDef()->GetpCMCR()->GetItemInverseConnector(M.Handle);
		
		if (hc != 0) {
			ClNoCallableItem item (M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK);
			//item.Attach ();
			item.SetCurrentBlock (ai_context.BlockReference.GetpProxy());
			
			item.MoveTo (M.Handle);
			
			ClNoCallableItem itemparent;

			/* Bug 0114798
			Hay que pasar el error hacia arriba
			*/
			iResult = item.InitAsMyParent( itemparent ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			itemparent.Value.IsNull (ao_isnull);
			return M4_SUCCESS;
		}

		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.GetItemNullByPos (M.iPos, 0, ao_isnull);
}

m4return_t ClItemProxy_Block::SetNull (const StKernelContext &ai_context, m4bool_t ai_bChangeStatus)
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}

	return ai_context.BlockReference->m_pClBlock->Block.SetItemNullByPos (M.iPos, 0, M4_TRUE, ai_bChangeStatus);
}

m4return_t ClItemProxy_Block::OldIsNull (const StKernelContext &ai_context, m4bool_t &ao_isnull)
{
	if (ai_context.BlockReference->m_pClBlock) {
		return ai_context.BlockReference->m_pClBlock->Block.GetLastItemNullByPos (M.iPos, 0, ao_isnull);
	}
	
	//si no hay bloque, no se ha podido hacer una asignación sobre el item => no tiene oldvalue
	//ni de coña:
	
	ao_isnull = M4_TRUE;
	return M4_SUCCESS;
}

m4return_t ClItemProxy_Block::GetHasSlices (const StKernelContext &ai_context, m4bool_t &ao_hasslices)
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.ItemHasSlices (M.iPos, 0, ao_hasslices);
}

m4return_t ClItemProxy_Block::SetHasSlices (const StKernelContext &ai_context, m4bool_t ai_hasslices)
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.SetItemHasSlices (M.iPos, 0, ai_hasslices);
}

m4return_t ClItemProxy_Block::OldHasSlices (const StKernelContext &ai_context, m4bool_t &ao_isnull)
{
	if (ai_context.BlockReference->m_pClBlock) {
		return ai_context.BlockReference->m_pClBlock->Block.LastItemHasSlices (M.iPos, 0, ao_isnull);
	}
	
	//si no hay bloque, no se ha podido hacer una asignación sobre el item => no tiene oldvalue
	//ni de coña:
	
	ao_isnull = M4_TRUE;
	return M4_SUCCESS;
}

m4return_t ClItemProxy_Block::GetPriority (const StKernelContext &ai_context, m4char_t &ao_priority)
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.GetItemPriority (M.iPos, 0, ao_priority);
}

m4return_t ClItemProxy_Block::SetPriority (const StKernelContext &ai_context, m4char_t ai_priority)
{
	if (!ai_context.BlockReference->m_pClBlock) {
		if (!ai_context.BlockReference->_CreateBlock()) return M4_ERROR;
	}
	return ai_context.BlockReference->m_pClBlock->Block.SetItemPriority (M.iPos, 0, ai_priority);
}

m4return_t ClItemProxy_Block::OldPriority (const StKernelContext &ai_context, m4char_t &ao_priority)
{
	if (ai_context.BlockReference->m_pClBlock) {
		return ai_context.BlockReference->m_pClBlock->Block.LastItemPriority (M.iPos, 0, ao_priority);
	}
	
	//si no hay bloque, no se ha podido hacer una asignación sobre el item => no tiene oldvalue
	//ni de coña:
	
	ao_priority = 3;
	return M4_SUCCESS;
}

m4return_t ClItemProxy_Block::GetIsUser (const StKernelContext &ai_context, m4bool_t &ao_bIsUser)
{
	m4return_t	iResult = M4_SUCCESS ;

	ao_bIsUser = M4_FALSE;

	if (!ai_context.BlockReference->m_pClBlock) {
		//------------------
		//truco:
		ClHandle hc = ai_context.BlockReference->GetpNode_Data()->GetpNodeDef()->GetpCMCR()->GetItemInverseConnector(M.Handle);
		
		if (hc) {
			ClNoCallableItem item (M4DM_SCOPE_NODE | M4DM_SCOPE_BLOCK);
			//item.Attach ();
			item.SetCurrentBlock(ai_context.BlockReference.GetpProxy());
			
			item.MoveTo(M.Handle);
			
			ClNoCallableItem itemparent;

			/* Bug 0114798
			Hay que pasar el error hacia arriba
			*/
			iResult = item.InitAsMyParent( itemparent ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			/* Bug 0275637
			Se comprueban los tipos cpp y no m4
			*/
			if (itemparent.ItemDef.CppType() == item.ItemDef.CppType()) {
				return itemparent.Value.m_oContext.pMemory_Interface->GetIsUser (itemparent.Value.m_oContext, ao_bIsUser);
			}
		}
	}
	else {
		return ai_context.BlockReference->m_pClBlock->Block.GetItemIsUser (M.iPos, 0, ao_bIsUser);
	}
	return M4_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// ClItemProxy_Register
//

// Metodo de validacion para comprobar que estamos bien posicionados.
m4return_t ClItemProxy_Register::Validation (const StKernelContext &ai_context, m4uint32_t ai_Offset)
{
    //Con esta comprobacion simplifico el codigo, ya que no lo tengo que hacer
    //al entrar en las funciones.
    if (ai_context.BlockReference->m_pClBlock == 0)
    {
        // Si el bloque es NIHIL saco mensaje.
        if (ai_context.BlockReference->IsNihil ())
        {
            m4uint32_t ItemHandle  = ai_context.pItemDef_Interface->GetHandle();
            ClCompiledMCR * pmcr   = ai_context.pItemDef_Interface->GetpCMCR ();

            DUMP_CHLOG_WARNINGF(M4_CH_DM_VALIDATION_NIHILBLOCK + ai_Offset, M4ObjNodeItemI(pmcr, ItemHandle));
        }

        return M4_ERROR;
    }

    if ((*ai_context.m_piMMIndex) != ClMMIndex_EOF)
        return M4_SUCCESS;

    else
    {
        m4uint32_t ItemHandle  = ai_context.pItemDef_Interface->GetHandle();
        ClCompiledMCR * pmcr   = ai_context.pItemDef_Interface->GetpCMCR ();

        DUMP_CHLOG_WARNINGF(M4_CH_DM_VALIDATION_REGEOF + ai_Offset,
            M4ObjNodeItemI(pmcr, ItemHandle));

        return M4_ERROR;
    }
}

m4return_t ClItemProxy_Register::GetDescription (const StKernelContext &ai_context, const void *&oi_Item) const
{
    return ai_context.BlockReference->m_pClBlock->Records.GetItemBlobFileDescription (M.iPos, *ai_context.m_piMMIndex, oi_Item);
}

m4return_t ClItemProxy_Register::GetAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices) const
{
    return ai_context.BlockReference->m_pClBlock->Records.GetItemDataAddressByPos_AndHasSlices (M.iPos, *ai_context.m_piMMIndex, *ao_address, ao_hasslices);
}

m4return_t ClItemProxy_Register::GetSize (const StKernelContext &ai_context, m4uint32_t &ao_size) const
{
    return ai_context.BlockReference->m_pClBlock->Records.GetItemSizeByPos (M.iPos, *ai_context.m_piMMIndex, ao_size);
}

m4return_t ClItemProxy_Register::SetValueByOffset (const StKernelContext &ai_context, const void* oi_Item, const m4uint32_t ai_Size, m4uint32_t ai_offset, m4uint32_t *ao_pretsize, m4bool_t ai_bChangeStatus, m4bool_t ai_bIsUser) const
{
	
	m4uint32_t		i = 0 ;
	m4uint32_t		iLength = 0 ;
	ClBSearchIndex*	poSearchIndex = NULL ;
	ClBSItemIndex*	poSearchIndexItem = NULL ;

	// Bug 0288635 Sólo se marca como no ordenado si el item es del índice
	if( ai_context.BlockReference->GetSearchIndexId() != -1 )
	{
		poSearchIndex = ai_context.pNode_Data->GetpSearchIndex() ;

		if( poSearchIndex != NULL )
		{
			iLength = poSearchIndex->Count() ;

			for( i = 0 ; i < iLength ; i++ )
			{
				poSearchIndexItem = poSearchIndex->Get( i ) ;

				if( poSearchIndexItem != NULL && poSearchIndexItem->GetItemHandle() == M.Handle )
				{
					//marco el bloque como 'NO ordenado' si se modifica un item del índice
					ai_context.BlockReference->SetNotSorted() ;
					break ;
				}
			}
		}
	}

	return ai_context.BlockReference->m_pClBlock->Records.SetItemValueByPosAndOffset(M.iPos, oi_Item, *ai_context.m_piMMIndex, ai_offset, ai_Size, ao_pretsize, ai_bChangeStatus, ai_bIsUser);
}

m4return_t ClItemProxy_Register::SetDescription (const StKernelContext &ai_context, const void* oi_Item) const
{
    return ai_context.BlockReference->m_pClBlock->Records.SetItemBlobFileDescription (M.iPos, oi_Item, *ai_context.m_piMMIndex);
}

m4return_t ClItemProxy_Register::SetSize (const StKernelContext &ai_context, m4uint32_t ai_newsize) const
{
    return ai_context.BlockReference->m_pClBlock->Records.SetItemSizeByPos (M.iPos, *ai_context.m_piMMIndex, ai_newsize);
}

m4return_t ClItemProxy_Register::GetOldAddress_andhasslices (const StKernelContext &ai_context, void **ao_address, m4bool_t &ao_hasslices) const
{
    return ai_context.BlockReference->m_pClBlock->Records.GetLastItemDataAddressByPos_AndHasSlices (M.iPos, *ai_context.m_piMMIndex, *ao_address, ao_hasslices);
}

m4return_t ClItemProxy_Register::GetOldSize (const StKernelContext &ai_context, m4uint32_t &ao_size) const
{
    return ai_context.BlockReference->m_pClBlock->Records.GetLastItemSizeByPos(M.iPos, *ai_context.m_piMMIndex, ao_size);
}

m4return_t ClItemProxy_Register::CreateTempFile(const StKernelContext &ai_context, m4pchar_t const ai_pDirectory, m4pchar_t const ai_pExtension)
{
	ClDsItemBlobFile* IBF = ai_context.BlockReference->m_pClBlock->Records.GetItemBlobFile(M.iPos, *ai_context.m_piMMIndex);
	if (!IBF) return M4_ERROR;

	return IBF->CreateTempFile(ai_pDirectory, ai_pExtension);
}

m4return_t ClItemProxy_Register::ChangedMask(const StKernelContext &ai_context, m4uint32_t & ao_oChangedMask)
{
	ClDsItemBlobFile* IBF = ai_context.BlockReference->m_pClBlock->Records.GetItemBlobFile(M.iPos, *ai_context.m_piMMIndex);
	if (!IBF) return M4_ERROR;
	
	ClComparisonResultMask CRM;
	IBF->ChangedMask(CRM);
        ao_oChangedMask = (m4uint32_t) CRM;
        return M4_SUCCESS;
}

m4return_t ClItemProxy_Register::IsNull (const StKernelContext &ai_context, m4bool_t &ao_isnull)
{
    return ai_context.BlockReference->m_pClBlock->Records.GetItemNullByPos(M.iPos, *ai_context.m_piMMIndex, ao_isnull);
}

m4return_t ClItemProxy_Register::SetNull (const StKernelContext &ai_context, m4bool_t ai_bChangeStatus)
{
    return ai_context.BlockReference->m_pClBlock->Records.SetItemNullByPos(M.iPos, *ai_context.m_piMMIndex, M4_TRUE, ai_bChangeStatus);
}

m4return_t ClItemProxy_Register::OldIsNull (const StKernelContext &ai_context, m4bool_t &ao_isnull)
{
    return ai_context.BlockReference->m_pClBlock->Records.GetLastItemNullByPos(M.iPos, *ai_context.m_piMMIndex, ao_isnull);
}

m4return_t ClItemProxy_Register::GetHasSlices (const StKernelContext &ai_context, m4bool_t &ao_hasslices)
{
    return ai_context.BlockReference->m_pClBlock->Records.ItemHasSlices(M.iPos, *ai_context.m_piMMIndex, ao_hasslices);
}

m4return_t ClItemProxy_Register::SetHasSlices (const StKernelContext &ai_context, m4bool_t ai_hasslices)
{
    return ai_context.BlockReference->m_pClBlock->Records.SetItemHasSlices(M.iPos, *ai_context.m_piMMIndex, ai_hasslices);
}

m4return_t ClItemProxy_Register::OldHasSlices (const StKernelContext &ai_context, m4bool_t &ao_isnull)
{
    return ai_context.BlockReference->m_pClBlock->Records.LastItemHasSlices(M.iPos, *ai_context.m_piMMIndex, ao_isnull);
}

m4return_t ClItemProxy_Register::GetPriority (const StKernelContext &ai_context, m4char_t &ao_priority)
{
    return ai_context.BlockReference->m_pClBlock->Records.GetItemPriority(M.iPos, *ai_context.m_piMMIndex, ao_priority);
}

m4return_t ClItemProxy_Register::SetPriority (const StKernelContext &ai_context, m4char_t ai_priority)
{
    return ai_context.BlockReference->m_pClBlock->Records.SetItemPriority(M.iPos, *ai_context.m_piMMIndex, ai_priority);
}

m4return_t ClItemProxy_Register::OldPriority (const StKernelContext &ai_context, m4char_t &ao_priority)
{
    return ai_context.BlockReference->m_pClBlock->Records.LastItemPriority(M.iPos, *ai_context.m_piMMIndex, ao_priority);
}

m4return_t ClItemProxy_Register::GetIsUser (const StKernelContext &ai_context, m4bool_t &ao_bIsUser)
{
    return ai_context.BlockReference->m_pClBlock->Records.GetItemIsUser(M.iPos, *ai_context.m_piMMIndex, ao_bIsUser);
}

////////////////////////////////////////////////////////////
// class _withitemproxy

_withitemproxy :: _withitemproxy ()
{
    m_pClItemProxy = &s_oClItemProxy_Nihil;
}

ClConnectItemInfo * _withitemproxy::GetpConnectItemInfo (void) const
{
    return m_pClItemProxy->GetpConnectItemInfo ();
}

m4uint16_t _withitemproxy::GetPos (void) const
{
    return m_pClItemProxy->GetPos ();
}

void _withitemproxy::SetEOF (void)
{
    m_pClItemProxy = &s_oClItemProxy_Nihil;
}

m4return_t 
_withitemproxy::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_withitemproxy);
	return M4_SUCCESS;
}

m4return_t 
_AClItemProxy::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_AClItemProxy);
		ao_size-=sizeof(M);
	
	if (M.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}
m4return_t 
StItemProxyData::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(StItemProxyData);

	ao_size-=sizeof(Handle);
	if (Handle.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	ao_size-=sizeof(ConnectItemInfo);
	if (ConnectItemInfo.GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

m4return_t 
_ClItemProxy_Nihil::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(_ClItemProxy_Nihil);
		ao_size-=sizeof(_AClItemProxy);
	
	if (_AClItemProxy::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
m4return_t 
ClItemProxy_Register::GetSize(ClSize &ao_size)
{
//	ClSizerLabel oSL(ao_size,"ClItemProxy_Register");
	ao_size+=sizeof(ClItemProxy_Register);
		ao_size-=sizeof(_AClItemProxy);
	
	if (_AClItemProxy::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t 
ClItemProxy_Node::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClItemProxy_Node);
		ao_size-=sizeof(_AClItemProxy);
	
	if (_AClItemProxy::GetSize(ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
