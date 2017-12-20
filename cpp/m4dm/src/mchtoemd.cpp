//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                mchtoemd.cpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                28-07-1997
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
// 
//
//==============================================================================

#include "mchtoemd.hpp"
#include "dm_types.hpp"
#include "rm.hpp"

#include "emd.hpp"
#include "m4mdrt.hpp"
#include "m4thread.hpp"
#include "dsitem.hpp"
#include "handles.hpp"
#include "memman.hpp"

#include "blocksynchronization.hpp"

void ClMetaChannelToExtendedMetaData::AssignNumberOfItemsNotMethodByScope(
        const ClCompiledMCR& ai_oMetaChannel,
        const m4uint32_t     ai_iNodeHandle,
        const m4uint8_t      ai_iScope,
        EMDStaticData* const ai_pStaticMetaData)
{
    m4uint32_t ItemHandle;
    m4uint16_t Item;
    m4uint16_t TotalItems = ai_oMetaChannel.GetNodeNumberOfItems(ai_iNodeHandle);
    
    ai_pStaticMetaData->m_NumItems = 0;
    ai_pStaticMetaData->m_NumMappedItems = 0;
    
    for (Item = 0; Item < TotalItems; Item++)
    {
        ItemHandle = ai_oMetaChannel.GetNodeItemHandleByPosition(ai_iNodeHandle, Item);
        
        if (ai_oMetaChannel.GetItemType(ItemHandle)  != M4CL_ITEM_TYPE_METHOD &&
            ai_oMetaChannel.GetItemScope(ItemHandle) == ai_iScope)
        {
            ai_pStaticMetaData->m_NumItems++;
            if (ai_oMetaChannel.GetItemCsType(ItemHandle) == M4CL_CSTYPE_MAPPED)
                ai_pStaticMetaData->m_NumMappedItems++;
        }
    }
}

static void CompleteChannelId(const ClCompiledMCR& ai_oMetaChannel,
                              m4uint32_t           ai_iNodeHandle,
                              m4pchar_t            ao_psCompleteChannelId)
{
    sprintf(ao_psCompleteChannelId,"%x&%s",
        (m4uint32_t) M4Thread::GetCurrentIdThread(),
        (char*)ai_oMetaChannel.GetChannelId());
    
    if (ai_oMetaChannel.GetLaneFatherChannelId() != NULL &&
        strcmp(ai_oMetaChannel.GetLaneSonNodeId(), ai_oMetaChannel.GetNodeId(ai_iNodeHandle)) == 0)
    {
        strcat(ao_psCompleteChannelId, "&");
        strcat(ao_psCompleteChannelId, ai_oMetaChannel.GetLaneFatherChannelId());
        strcat(ao_psCompleteChannelId, "&");
        strcat(ao_psCompleteChannelId, ai_oMetaChannel.GetLaneFatherNodeId());
    }
}

#ifndef WIN64
   #define SIZE_OF_PTR (4)
#else
   #define SIZE_OF_PTR (8)
#endif

// Returns 1, 4 or 8
static m4uint16_t AlignmentOfItem(ExtendedItemData& ai_ItemMetaData)
{
    if (ai_ItemMetaData.LivesInFile == M4_TRUE)
		return 8;	// Alineamos a 8 que es lo que tiene que devolver el new.
    else
    {
        if (ai_ItemMetaData.IsVariableLength)
            return SIZE_OF_PTR;
        else
        {
            if (ai_ItemMetaData.CppType == M4CL_CPP_TYPE_NUMBER ||
                ai_ItemMetaData.CppType == M4CL_CPP_TYPE_DATE)
                return sizeof(double);
            else
                return 1; // Is 1, but REMEMBER: 
                          // needs to be aligned because the item could go to a sliced mode
        }
    }
}

m4return_t ClMetaChannelToExtendedMetaData::ConvertMetaChannelToExtendedMetaDataByScope(
        const ClCompiledMCR& ai_oMetaChannel,
        const m4uint32_t     ai_iNodeHandle,
        const m4uint8_t      ai_iScope,
        EMDStaticData*&      ao_pStaticMetaData)
{
    ClMutBlock MutEMDBlock(ClExtendedMetaData::m_soMutEMD);
    
    m4uint32_t ItemHandle;
    
    m4char_t psCompleteChannelId[256 * 2];
    CompleteChannelId(ai_oMetaChannel, ai_iNodeHandle, psCompleteChannelId);
    
    ao_pStaticMetaData =
        ClExtendedMetaData::FindOrInsertMetaData(psCompleteChannelId,
        ai_iNodeHandle,
        ai_iScope,
        ai_oMetaChannel.GetChannelVersion(),
        ai_oMetaChannel.GetChannelCsType());
    
    if (ao_pStaticMetaData == NULL)
        return M4_ERROR;

    // Si ya existia he aumentado sus referencias y no me hace falta hacer nada mas
    if (ao_pStaticMetaData->m_Count > 1)
        return M4_SUCCESS;
    
    // Calculamos el numero de items para el scope definido, y si usamos front back tambien
    // nos actualiza cuantos items mapped hay para el scope definido
    AssignNumberOfItemsNotMethodByScope(ai_oMetaChannel,      
        ai_iNodeHandle,      
        ai_iScope,      
        ao_pStaticMetaData);

    if (ao_pStaticMetaData->m_NumItems > 0)
    {
        // Reservamos la memoria para los items
        ao_pStaticMetaData->m_pIData = (ExtendedItemData*)ao_pStaticMetaData->GetpMemoryManager()->M4malloc(ao_pStaticMetaData->m_NumItems * sizeof(ExtendedItemData));
        if (ao_pStaticMetaData->m_pIData == NULL)
        {
            ao_pStaticMetaData->Reset();
            ao_pStaticMetaData = NULL;
            return M4_ERROR;
        }
        
        // Reservamos la memoria para el vector con los indices de los items mapped
        if (ao_pStaticMetaData->m_NumMappedItems > 0)
        {
            ao_pStaticMetaData->m_pIndexOfMappedItem = (m4uint16_t*)ao_pStaticMetaData->GetpMemoryManager()->M4malloc(ao_pStaticMetaData->m_NumMappedItems * sizeof(m4uint16_t));
            if (ao_pStaticMetaData->m_pIndexOfMappedItem == NULL)
            {
                ao_pStaticMetaData->Reset();
                ao_pStaticMetaData = NULL;
                return M4_ERROR;
            }
        }

        ao_pStaticMetaData->m_DefaultBlobMaskSize = (ao_pStaticMetaData->m_NumItems / 8) + 1;
        
        // Default blob mask
        ao_pStaticMetaData->m_pDefaultBlobMask = ao_pStaticMetaData->GetpMemoryManager()->M4calloc(ao_pStaticMetaData->m_DefaultBlobMaskSize, 1);
        if (ao_pStaticMetaData->m_pDefaultBlobMask == NULL)
        {
            ao_pStaticMetaData->Reset();
            ao_pStaticMetaData = NULL;
            return M4_ERROR;
        }
    }

    //
    // Calculo del tamaño de la cabecera
    //
    ao_pStaticMetaData->m_UserDataOffset = M4CL_EMD_NUMBER_OF_MASKS * ao_pStaticMetaData->m_DefaultBlobMaskSize +
        sizeof(RegisterStatus_t);
    ao_pStaticMetaData->m_UserDataOffset = M4_ROUND_TO_8(ao_pStaticMetaData->m_UserDataOffset);
    
    //ao_pStaticMetaData->m_UserDataSize = 0;
    switch(ai_iScope)
    {
    case M4CL_ITEM_SCOPE_NODE:
        // oldValue(4) + recordID(4) -> 8
        ao_pStaticMetaData->m_UserDataSize = sizeof(regindex_t) + sizeof(regid_t);
        break;

    case M4CL_ITEM_SCOPE_BLOCK:
        // oldValue(4) + recordID(4) -> 8
        ao_pStaticMetaData->m_UserDataSize = sizeof(regindex_t) + sizeof(regid_t);
        break;

    case M4CL_ITEM_SCOPE_REGISTER:
        // oldValue(4) + recordID(4) + version(8) + oldVersion(8) +  -> 24
        ao_pStaticMetaData->m_UserDataSize =
            sizeof(regindex_t) + sizeof(regid_t) + sizeof(VersionMark) + sizeof(VersionMark);
        break;

    default:
        M4_ASSERT(0);
        return M4_ERROR;
    }
    ao_pStaticMetaData->m_UserDataSize = M4_ROUND_TO_8(ao_pStaticMetaData->m_UserDataSize);
    
    ao_pStaticMetaData->m_RecordSize = ao_pStaticMetaData->m_UserDataOffset + ao_pStaticMetaData->m_UserDataSize;
    M4_ASSERT((ao_pStaticMetaData->m_RecordSize & 0x7) == 0);
    
    // The first item is aligned to a multiple of 8's address
    //ao_pStaticMetaData->m_RecordSize = (ao_pStaticMetaData->m_RecordSize + 7) & 0xfffffff8;
    
    //
    // Calculo de los items
    //
    m4uint16_t MappedItem = 0;
    m4uint16_t NumItems   = 0;
    m4uint16_t TotalItems = ai_oMetaChannel.GetNodeNumberOfItems(ai_iNodeHandle);
    
    ao_pStaticMetaData->m_SomeItemLivesInFile = M4_FALSE;
    
    m4uint32_t iNumItemsAlignedTo8   = 0; // Doubles, dates, including BLOB files ! ! !
    m4uint32_t iNumItemsLivingInFile = 0; // BLOB Files
    m4uint32_t iNumItemsAlignedTo4   = 0; // Ptrs
    m4uint32_t iNumItemsAlignedTo1   = 0; // Fixed strings
    m4uint16_t Item; 
    for (Item = 0; Item < TotalItems; Item++)
    {
        ItemHandle = ai_oMetaChannel.GetNodeItemHandleByPosition(ai_iNodeHandle, Item);
        
        if (ai_oMetaChannel.GetItemType(ItemHandle) != M4CL_ITEM_TYPE_METHOD &&
            ai_oMetaChannel.GetItemScope(ItemHandle) == ai_iScope)
        {
            if (ai_oMetaChannel.GetItemCsType(ItemHandle) == M4CL_CSTYPE_MAPPED)
            {
                ao_pStaticMetaData->m_pIndexOfMappedItem[MappedItem] = NumItems;
                MappedItem++;
            }
            
            ao_pStaticMetaData->m_pIData[NumItems].MaxSize = ai_oMetaChannel.GetItemMaxSize(ItemHandle);
            ao_pStaticMetaData->m_pIData[NumItems].CppType = ai_oMetaChannel.GetItemCppType(ItemHandle);
            ao_pStaticMetaData->m_pIData[NumItems].Handle  = ItemHandle;
            
            // Alex blob
            // Para saber si un es un enlace a un ficher (ItemBlobFile)
            ao_pStaticMetaData->m_pIData[NumItems].LivesInFile = ( ( (ai_oMetaChannel.GetItemM4Type(ItemHandle))==M4CL_M4_TYPE_BLOB ) ? M4_TRUE : M4_FALSE );
            
            // Para saber si es un item de longitud variable (un ItemBlob)
            ao_pStaticMetaData->m_pIData[NumItems].IsVariableLength = ai_oMetaChannel.GetItemIsVariableLength(ItemHandle);
            
            // Count the number of items by alignment
            switch (AlignmentOfItem(ao_pStaticMetaData->m_pIData[NumItems]))
            {
            case 1 :
                iNumItemsAlignedTo1++; 
                break;
                
            case 4 :
                iNumItemsAlignedTo4++; 
                break;
                
            case 8 :
                iNumItemsAlignedTo8++; 
				if (ao_pStaticMetaData->m_pIData[NumItems].LivesInFile)
				   iNumItemsLivingInFile++;
                break;
                
            default :
                M4_ASSERT(0);
                return M4_ERROR;
                break;
            }
            
            // Asignacion de bits de header por defecto, otros
            // Alex blob
            if (ao_pStaticMetaData->m_pIData[NumItems].LivesInFile == M4_TRUE)
            {
                ao_pStaticMetaData->m_SomeItemLivesInFile = M4_TRUE;
                ao_pStaticMetaData->m_pIData[NumItems].IsVariableLength = M4_FALSE;
            }
            else
            {
                if (ao_pStaticMetaData->m_pIData[NumItems].IsVariableLength)
                    ao_pStaticMetaData->m_pDefaultBlobMask[NumItems / 8] |= (1 << (NumItems % 8));
            }
            
            ao_pStaticMetaData->m_pIData[NumItems].IsPersisted = M4_BOOL(ai_oMetaChannel.GetItemAffectsDB(ItemHandle));
            
            NumItems++;
        }
    }
    
    // Now we have all the items inserted in the item vector, and the total number
    // of items by their alignment. We can sort them by alignment, and recalc the
    // offsets and recordsize...

    //
    // Calculamos los offset del los items (ordenandolos segun el alineamiento 8,4,1)
    //
    if (NumItems > 0)
    {
        // Ofssets for each type
        m4uint32_t iOffsetAlignedTo8 = ao_pStaticMetaData->m_RecordSize;
        m4uint32_t iOffsetAlignedTo4 = iOffsetAlignedTo8 + 
                                       (iNumItemsAlignedTo8 - iNumItemsLivingInFile) * sizeof(double) +
                                       (iNumItemsLivingInFile) * M4_ROUND_TO_8(ClDsItemBlobFile::GetStaticSize(ClHandle(0), ai_oMetaChannel));
        m4uint32_t iOffsetAlignedTo1 = iOffsetAlignedTo4 + iNumItemsAlignedTo4 * 4;
        
        m4uint32_t iNumOfItemsAlignedTo1Processed = 0;
        
        for (m4uint16_t iItemIterator = 0; iItemIterator < NumItems; iItemIterator++)
        {
            switch (AlignmentOfItem(ao_pStaticMetaData->m_pIData[iItemIterator]))
            {
            case 1 :
                ao_pStaticMetaData->m_pIData[iItemIterator].Offset = iOffsetAlignedTo1;
                
                iNumOfItemsAlignedTo1Processed++;
                
#ifndef WIN64
                if (iNumOfItemsAlignedTo1Processed == iNumItemsAlignedTo1 && ao_pStaticMetaData->m_pIData[iItemIterator].MaxSize >= 4)
#else
                if (iNumOfItemsAlignedTo1Processed == iNumItemsAlignedTo1 && ao_pStaticMetaData->m_pIData[iItemIterator].MaxSize >= 8)
#endif
                {
                   // Is the last one !!! (no need for alignment of size if size is more than 4 or 8)
                    iOffsetAlignedTo1 += ao_pStaticMetaData->m_pIData[iItemIterator].MaxSize; 
                }
                else
                {
#ifndef WIN64
                    iOffsetAlignedTo1 += M4_ROUND_TO_4(ao_pStaticMetaData->m_pIData[iItemIterator].MaxSize);
#else
                    iOffsetAlignedTo1 += M4_ROUND_TO_8(ao_pStaticMetaData->m_pIData[iItemIterator].MaxSize);
#endif
                }
                break;
                
            case 4 :
                ao_pStaticMetaData->m_pIData[iItemIterator].Offset = iOffsetAlignedTo4;
				
                iOffsetAlignedTo4 += 4;
                break;
                
            case 8 :
                ao_pStaticMetaData->m_pIData[iItemIterator].Offset = iOffsetAlignedTo8;
				
                if (ao_pStaticMetaData->m_pIData[iItemIterator].LivesInFile == M4_TRUE)
                {
                    iOffsetAlignedTo8 += M4_ROUND_TO_8(ClDsItemBlobFile::GetStaticSize(ClHandle(ao_pStaticMetaData->m_pIData[iItemIterator].Handle), ai_oMetaChannel));
                }
				else
                    iOffsetAlignedTo8 += 8;   // sizeof(double);
                break;
                
            default :
                return M4_ERROR;
                break;
            }
            
            // Save the index of the first offset 
            // (could be used in some circumstances)
            if (ao_pStaticMetaData->m_pIData[iItemIterator].Offset == ao_pStaticMetaData->m_RecordSize)
                ao_pStaticMetaData->m_iIndexOfFirstOffset = iItemIterator;
        }
        
        // Recalc the real record size
        ao_pStaticMetaData->m_RecordSize = iOffsetAlignedTo1;
    }
    else
        ao_pStaticMetaData->m_iIndexOfFirstOffset = -1;
        
    M4_ASSERT(NumItems   == ao_pStaticMetaData->m_NumItems);
    M4_ASSERT(MappedItem == ao_pStaticMetaData->m_NumMappedItems);
    
    ao_pStaticMetaData->m_pChannelId = (m4char_t*)ao_pStaticMetaData->GetpMemoryManager()->M4malloc(strlen(psCompleteChannelId) + 1);
    if (ao_pStaticMetaData->m_pChannelId == NULL)
    {
        ao_pStaticMetaData->Reset();
        ao_pStaticMetaData = NULL;
        return M4_ERROR;
    }
    
    strcpy(ao_pStaticMetaData->m_pChannelId, psCompleteChannelId);
    
    ao_pStaticMetaData->m_NodeHandle     = ai_iNodeHandle;
    ao_pStaticMetaData->m_Scope          = ai_iScope;
    ao_pStaticMetaData->m_ChannelVersion = ai_oMetaChannel.GetChannelVersion();
    ao_pStaticMetaData->m_CsType         = ai_oMetaChannel.GetChannelCsType();

    //
    // Default value of the records
    //
    m4VariantType DefaultValue;

    if (NumItems > 0)
    {
        // Buffer for holding it
        ao_pStaticMetaData->m_pDefaultRecord = (m4char_t *) ao_pStaticMetaData->GetpMemoryManager()->M4calloc(1, ao_pStaticMetaData->m_RecordSize);
        if (ao_pStaticMetaData->m_pDefaultRecord == NULL)
        {
            ao_pStaticMetaData->Reset();
            ao_pStaticMetaData = NULL;
            return M4_ERROR;
        }
        
        ClItemRecordManager IRM(ao_pStaticMetaData);
        IRM.SetpMemoryManager(ao_pStaticMetaData->GetpMemoryManager());
        
        // Initializing it
        IRM.InitRecordNoDefaultValues((m4uint8_t*)ao_pStaticMetaData->m_pDefaultRecord);
        
        m4bool_t HasDefaults = M4_FALSE;
        m4uint16_t TotalItems = ai_oMetaChannel.GetNodeNumberOfItems(ai_iNodeHandle);
        for (Item = 0, NumItems = 0; Item < TotalItems; Item++)
        {
            ItemHandle = ai_oMetaChannel.GetNodeItemHandleByPosition(ai_iNodeHandle, Item);
            
            if (ai_oMetaChannel.GetItemType(ItemHandle)  != M4CL_ITEM_TYPE_METHOD &&
                ai_oMetaChannel.GetItemScope(ItemHandle) == ai_iScope)
            {
                // Alex blob
                // Los blob file no pueden tener valores por defecto
                if (ao_pStaticMetaData->m_pIData[NumItems].LivesInFile == M4_TRUE)
                {
                    NumItems++;
                    continue;
                }
                
                ai_oMetaChannel.GetItemDefaultValue(ItemHandle, DefaultValue);
                
                switch (DefaultValue.Type)
                {
                case M4CL_CPP_TYPE_NULL : 
                    // ??? I think I have nothing to do
                    break;
                    
                case M4CL_CPP_TYPE_NUMBER :
                case M4CL_CPP_TYPE_DATE :
                    IRM.SetItemValue((m4uint8_t*)ao_pStaticMetaData->m_pDefaultRecord,
                                     NumItems,
                                     &DefaultValue.Data.DoubleData,
                                     sizeof(DefaultValue.Data.DoubleData));
                    HasDefaults = M4_TRUE;
                    break;
                    
                case M4CL_CPP_TYPE_STRING_VAR :
                    IRM.SetItemValue((m4uint8_t*)ao_pStaticMetaData->m_pDefaultRecord,
                                     NumItems,
                                     DefaultValue.Data.PointerChar,
                                     strlen(DefaultValue.Data.PointerChar)+1);
                    HasDefaults = M4_TRUE;
                    break;
                    
                case M4CL_M4_TYPE_BLOB:
                    M4_ASSERT(0);
                    return M4_ERROR;
                    break;
                }
                NumItems++;
            }
        }
        
        if (HasDefaults == M4_FALSE)
        {
            ao_pStaticMetaData->GetpMemoryManager()->M4free((m4uint8_t *)ao_pStaticMetaData->m_pDefaultRecord);
            ao_pStaticMetaData->m_pDefaultRecord = NULL;
        }
    }

    return M4_SUCCESS;
}

