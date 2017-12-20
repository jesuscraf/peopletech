//==============================================================================
//
// (c) Copyright  1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           Data Block
// File:             rm.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             28/07/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:
//
//   
// Definition:
//
//    This module defines...
//
// 11/07/97
//==============================================================================

#include "rm.hpp"
#include "rm.inl"
#include "m4stl.hpp"

#include "channelprocess.hpp"
#include "chlog.hpp"
#include "clgeniod.hpp"
#include "cstracesize.hpp"
#include "dsitem.hpp"
#include "dsres.hpp"
#include "memman.hpp"
#include "regpipe.hpp"
#include "slices_variable.hpp"
#include "slices_fixed.hpp"
#include "channel.hpp"


m4return_t ClItemRecordManager::SetLotOfItems(m4uint8_t* const DataBuffer,
                                              ClVectorRecordBase* const ai_pVRB,
                                              ClRegister_Pipe* ai_pRegPipe,
                                              const m4bool_t   ai_bAuxLoad)
{
    // Ojo con la precision (longitud de los items)
    m4uint16_t ai_Pos;
    m4pint32_t ai_iLength;

    m4int32_t  AuxSize;
    m4uint8_t* AuxPtr;
    m4uint32_t isvar;
    
    if (ai_bAuxLoad)
    {
        // Ojo que puedo tener blobs con valores por defecto y esto me los machaca
        // Tenog que recorrerme el registro y borrar los blobs. Lo mismo para los BlobFile
        DeleteBlobsRecord(DataBuffer);
        DestroyAllItemBlobFile(DataBuffer);
        InitRecord(DataBuffer);

        // Despues de hacer el init del registro necesito construir los BlobFile que he destruido
        CreateAllItemBlobFile(DataBuffer, ai_pVRB);
    }

    m4int32_t NumItem = ai_pRegPipe->GetNumItem();
	m4int32_t InitItem = ai_pRegPipe->GetInitItem();
	m4int32_t EndItem = NumItem;
	
	if (InitItem == -1)
	{
		InitItem = 0;
	}
	else
    {
		EndItem = ai_pRegPipe->GetEndItem();
		if (EndItem > NumItem)
		{
			EndItem = NumItem;
		}
	}

    for (m4int32_t i = InitItem; i < EndItem; i++)
    {
        ai_iLength = ai_pRegPipe->GetItemLength(i);
        ai_Pos = ai_pRegPipe->GetItemPos(i);
        
        if ((ai_iLength) &&
            (*ai_iLength != -1))
        {
            //isvar = EMData->ItemIsVariableLength(ai_Pos); CRG A Ver SI ESTO ANDA MEJOR
            isvar = IsItemBlob(DataBuffer,ai_Pos);    // Comprobacion dinamica
            
            if (isvar == M4_FALSE)
            {
                if (ai_pRegPipe->NumericItem(i))
				{
                    AuxSize = m_pSMData->ItemMaxSize(ai_Pos);
				}
                else
				{
                    AuxSize = min( m_pSMData->ItemMaxSize(ai_Pos), (m4uint32_t) (strlen( ai_pRegPipe->GetItemAddress(i) ) +1) ) ;
				}
                
                AuxPtr = DataBuffer+m_pSMData->ItemOffset(ai_Pos);
                memcpy(AuxPtr, ai_pRegPipe->GetItemAddress(i), AuxSize);
                SetItemNullLL(DataBuffer,ai_Pos, M4_FALSE);
            }
            else
            {
                //AuxSize = (*ai_iLength) + 1;
                AuxSize = strlen( ai_pRegPipe->GetItemAddress(i) )+ 1;
                if (SetItemValueByOffsetLL(DataBuffer,ai_Pos,
                    ai_pRegPipe->GetItemAddress(i),
                    0,
                    AuxSize,
                    M4_TRUE) != M4_SUCCESS)
				{
                    return M4_ERROR;
				}
            }
        }
        else
        {
            SetItemNull(DataBuffer,ai_Pos,M4_TRUE);
        }
    }
    
    return M4_SUCCESS;
}

m4return_t ClItemRecordManager::SetItemValueByOffsetLL(m4uint8_t* const DataBuffer,
                                                       const m4uint16_t  ai_ItemPos,
                                                       const void* const ai_Item,
                                                       const m4uint32_t  ai_Offset,
                                                       const m4uint32_t  ai_Size,
                                                       const m4bool_t    ai_ReleaseMemory,
                                                       m4uint32_t* const ao_pRetSize)
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));
        m4uint32_t iRetSize = (m4uint32_t)pBlobFile->SetValue(ai_Item, ai_Size, ai_Offset);
        if (ao_pRetSize != NULL)
		{
            *ao_pRetSize = iRetSize;
		}
        return (iRetSize == ai_Size) ? M4_SUCCESS : M4_ERROR;
    }

    m4uint8_t* AuxPtr = DataBuffer + m_pSMData->ItemOffset(ai_ItemPos);
    m4uint32_t iRealSize;
    m4return_t res = M4_SUCCESS;
    m4uint32_t iLastOffset = ai_Offset + ai_Size;
    
    if (IsItemBlob(DataBuffer,ai_ItemPos) == M4_TRUE)
    {
        // Como es un blob puede copiarlo entero, crecera si es necesario
        iRealSize = ai_Size;    
        
        // BlobDataPtr1 points to the actual Blob structure
        BlobDataPtr pBlobData = *((BlobDataPtr *)AuxPtr);
        
        // Siempre necesito reservar un nuevo blob para no perder el oldValue
        BlobDataPtr pAuxBlobData;  
        
        // Getting the memory for the new blob
        m4uint32_t iMaxSize;
        if (pBlobData)
		{
            iMaxSize = max(iLastOffset, pBlobData->Size);
		}
        else
		{
            iMaxSize = iLastOffset;
		}

        m4bool_t bPorCojonesHagoMalloc = (pBlobData == NULL) || 
                                         (ai_ReleaseMemory == M4_FALSE) ||
                                         (iLastOffset > pBlobData->Size) ? M4_TRUE : M4_FALSE;

        if (bPorCojonesHagoMalloc)
        {
            if ((pAuxBlobData = (BlobDataPtr)m_poMemoryManager->M4malloc( iMaxSize + sizeof(BlobData)-1 )) == 0)
			{
                return M4_ERROR;
			}
            
            // Copy the old data
            if (pBlobData)
            {
                memcpy(&pAuxBlobData->Data[0], &pBlobData->Data[0], pBlobData->Size);
                
                // Release the old value
                if (ai_ReleaseMemory == M4_TRUE)
				{
                    m_poMemoryManager->M4free((m4uint8_t *)pBlobData);
				}
            }
            pAuxBlobData->Size = iMaxSize;
        }
        else
		{
           pAuxBlobData=pBlobData;
		}
        
        // Copy the new data
        memcpy(&pAuxBlobData->Data[ai_Offset], ai_Item, ai_Size);

        // Assign the new blob
        *(BlobDataPtr*)AuxPtr = pAuxBlobData;
    }
    else
    {
        // Simple case
        // Nunca se convierte en blob, eso es tarea de SetItemSize
        
        if (iLastOffset <= m_pSMData->ItemMaxSize(ai_ItemPos))
		{
            iRealSize = ai_Size;
		}
        else
        {
            if (ai_Offset < m_pSMData->ItemMaxSize(ai_ItemPos))
            {
                iRealSize = m_pSMData->ItemMaxSize(ai_ItemPos) - ai_Offset;
                res = M4_WARNING;    // No se copia todo solo el maximo menos el offset !!!Warning!!!
            }
            else
			{
                return M4_ERROR;     // Estan intentando escribir fuera de rango
			}
        }
        memcpy(AuxPtr + ai_Offset, ai_Item, iRealSize);
    }
    
    if (ao_pRetSize)
	{
        *ao_pRetSize = iRealSize;
	}
    
    SetItemNullLL(DataBuffer,ai_ItemPos,M4_FALSE);
    return res;
}

m4return_t ClItemRecordManager::SetItemValueByOffset(m4uint8_t* const DataBuffer,
                                                     const m4uint16_t  ai_ItemPos,
                                                     const void* const ai_Item,
                                                     const m4uint32_t  ai_Offset,
                                                     const m4uint32_t  ai_Size,
                                                     m4uint32_t* const ao_pRetSize)
{
   return SetItemValueByOffsetLL(DataBuffer,ai_ItemPos, ai_Item, ai_Offset, ai_Size, M4_TRUE, ao_pRetSize);
}

m4return_t ClItemRecordManager::SetItemValueByOffsetNoRelease(m4uint8_t* const DataBuffer,
                                                              const m4uint16_t  ai_ItemPos,
                                                              const void* const ai_Item,
                                                              const m4uint32_t  ai_Offset,
                                                              const m4uint32_t  ai_Size,
                                                              m4uint32_t* const ao_pRetSize)
{
   return SetItemValueByOffsetLL(DataBuffer,ai_ItemPos, ai_Item, ai_Offset, ai_Size, M4_FALSE, ao_pRetSize);
}

m4return_t ClItemRecordManager::SetItemValue(m4uint8_t* const DataBuffer,
                                             const m4uint16_t  ai_ItemPos,
                                             const void* const ai_Item,
                                             const m4uint32_t  ai_Size,
                                             m4uint32_t* const ao_pRetSize)
{
   return SetItemValueByOffsetLL(DataBuffer,ai_ItemPos, ai_Item, 0, ai_Size, M4_TRUE, ao_pRetSize);
}

m4return_t ClItemRecordManager::SetItemValueNoRelease(m4uint8_t* const DataBuffer,
                                                      const m4uint16_t  ai_ItemPos,
                                                      const void* const ai_Item,
                                                      const m4uint32_t  ai_Size,
                                                      m4uint32_t* const ao_pRetSize)
{
   return SetItemValueByOffsetLL(DataBuffer,ai_ItemPos, ai_Item, 0, ai_Size, M4_FALSE, ao_pRetSize);
}

m4return_t ClItemRecordManager::GetItemValueByOffset(m4uint8_t* const DataBuffer,
                                                     const m4uint16_t ai_ItemPos,
                                                     void*            ao_Item,
                                                     const m4uint32_t ai_Offset,
                                                     m4uint32_t&      ao_RealSize,
                                                     const m4uint32_t ai_MaxSize,
                                                     m4bool_t&        ao_IsNull) const
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));
        ao_RealSize = (m4uint32_t)pBlobFile->GetValue(ao_Item, ao_IsNull, ai_MaxSize, ai_Offset);
        return (ao_RealSize == ai_MaxSize) ? M4_SUCCESS : M4_ERROR;
    }
        
    m4uint8_t *AuxPtr = DataBuffer + m_pSMData->ItemOffset(ai_ItemPos);
    
    ao_IsNull = GetItemNull(DataBuffer,ai_ItemPos);
    
    if (IsItemBlob(DataBuffer,ai_ItemPos))
    {
        // BlobDataPtr1 points to the actual Blob structure
        BlobDataPtr BlobDataPtr1 = *((BlobDataPtr *)AuxPtr);
        
        if (BlobDataPtr1 == 0)
        {
            ao_RealSize = 0;
            
            return M4_SUCCESS;
        }
        
        if (ai_Offset > BlobDataPtr1->Size)
		{
            return M4_ERROR;
		}
        
        if (ai_Offset + ai_MaxSize <= BlobDataPtr1->Size)
		{
            ao_RealSize = ai_MaxSize;
		}
        else
		{
            ao_RealSize = BlobDataPtr1->Size - ai_Offset;
		}
        
        memcpy(ao_Item, &BlobDataPtr1->Data[ai_Offset], ao_RealSize);
    }
    else
    {
        // Simple case
        m4uint32_t MaxSize = m_pSMData->ItemMaxSize(ai_ItemPos);
        
        if (ai_Offset + ai_MaxSize <= MaxSize)
		{
            ao_RealSize = ai_MaxSize;
		}
        else
		{
            ao_RealSize = MaxSize - ai_Offset;
		}
        
        memcpy(ao_Item, AuxPtr + ai_Offset, ao_RealSize);
    }
    
    return M4_SUCCESS;
}

m4return_t ClItemRecordManager::GetItemValue(m4uint8_t* const DataBuffer,
                                             const m4uint16_t ai_ItemPos,
                                             void*            ao_Item,
                                             m4uint32_t&      ao_RealSize,
                                             const m4uint32_t ai_MaxSize,
                                             m4bool_t&        ao_IsNull) const
{
   return GetItemValueByOffset(DataBuffer,ai_ItemPos, ao_Item, 0, ao_RealSize, ai_MaxSize, ao_IsNull);
}

m4return_t ClItemRecordManager::SetItemSize(m4uint8_t* const DataBuffer,
                                            const m4uint16_t ai_ItemPos,
                                            const m4uint32_t ai_NewSize,
                                            const m4bool_t   ai_bRelease)
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));
        return pBlobFile->SetSize(ai_NewSize);
    }

    m4uint32_t iSourceSize;     // Tamaño actual del item
    m4return_t iRet = GetItemSize(DataBuffer, ai_ItemPos, iSourceSize);

    if (iRet == M4_SUCCESS && ai_NewSize != iSourceSize)
    {
        if (ai_NewSize > iSourceSize)
        {
            m4bool_t bIsNull;
            m4uint32_t iRealSize;       // Tamaño del item despues del SetItemValueByOffset
            m4uint8_t AuxBuffer[512];   // Buffer auxiliar, valido para la mayoria de los casos
            void* pBuffer;
            if (ai_NewSize > sizeof(AuxBuffer))
            {
                pBuffer = m_poMemoryManager->M4malloc(ai_NewSize);
                if (pBuffer == NULL)
				{
                    return M4_ERROR;
				}
            }
            else
			{
                pBuffer = AuxBuffer;
			}
            
            iRet = GetItemValueByOffset(DataBuffer, ai_ItemPos, pBuffer, 0, iRealSize, iSourceSize, bIsNull);
            
            // Compruebo si hay que convertir el item a blob
            if (iRet == M4_SUCCESS)
            {
                if (IsItemBlob(DataBuffer, ai_ItemPos) == M4_FALSE)
                {
                    // Cambio la mascara
                    SetBlobState(DataBuffer,ai_ItemPos);
                
                    // Machaco el valor para que cuando haga el SetItemValue crea
                    // que es un blob sin memoria
                    SetItemNullLL(DataBuffer,ai_ItemPos,M4_TRUE);
                
                    m4uint8_t* AuxPtr = DataBuffer + m_pSMData->ItemOffset(ai_ItemPos);
                    *(BlobDataPtr*)AuxPtr = 0;
                }

                iRet = SetItemValueByOffsetLL(DataBuffer,ai_ItemPos, pBuffer, 0, ai_NewSize, ai_bRelease);
            }
            
            if (ai_NewSize > sizeof(AuxBuffer))
			{
                m_poMemoryManager->M4free(pBuffer);
			}
        }
        else
        {
            if (IsItemBlob(DataBuffer, ai_ItemPos) == M4_TRUE)
            {
                // Si el nuevo tamaño es mas pequeño tengo que encoger el blob
                
                // Consigo el puntero al blob
                m4uint8_t* AuxPtr = DataBuffer + m_pSMData->ItemOffset(ai_ItemPos);
                BlobDataPtr pBlobData = *((BlobDataPtr *)AuxPtr);
                
                // Le cambio el tamaño al blob, de esta forma al reservar
                // la nueva memoria va a encoger el blob
                pBlobData->Size = ai_NewSize;
            }
        }
    }
    
    return iRet;
}

m4return_t ClItemRecordManager::GetItemSize(m4uint8_t* const DataBuffer,
                                            const m4uint16_t ai_ItemPos,
                                            m4uint32_t&      ao_Size) const
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));
        ao_Size = pBlobFile->GetSize();
		return M4_SUCCESS;
    }

    if (GetItemNull(DataBuffer,ai_ItemPos))
	{
        ao_Size = 0;
	}
    else
    {
        if (IsItemBlob(DataBuffer,ai_ItemPos))
        {
            m4uint8_t *AuxPtr = DataBuffer + m_pSMData->ItemOffset(ai_ItemPos);
            
            BlobDataPtr BlobDataPtr1;
            
            // Blob case
            // BlobDataPtr1 points to the actual Blob structure
            BlobDataPtr1 = *((BlobDataPtr *)AuxPtr);
            
            if (BlobDataPtr1 == 0)
            {
                ao_Size = 0;
                
                return M4_SUCCESS;
            }
            
            ao_Size = BlobDataPtr1->Size;
        }
        else
		{
            ao_Size = m_pSMData->ItemMaxSize(ai_ItemPos);
		}
    }
        
    return M4_SUCCESS;
}

void ClItemRecordManager::DeleteBlobsRecord (m4uint8_t* const DataBuffer,
                                             const m4bool_t   ai_bOnlyMappedItems /*= M4_FALSE*/)
{
    m4uint16_t  iNItems;
    if (ai_bOnlyMappedItems == M4_TRUE)
	{
        iNItems = m_pSMData->NMappedItems();
	}
    else
	{
        iNItems = m_pSMData->NItems();
	}

    m4uint16_t iItem;
    for (m4uint16_t i = 0; i < iNItems; i++)
    {
        if (ai_bOnlyMappedItems == M4_TRUE)
		{
            iItem = m_pSMData->GetIndexOfMappedItem(i);
		}
        else
		{
            iItem = i;
		}

        if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE)
		{
			/* Bug 0098967
			Hay que poner el blob a nulo para que se quite el interes sobre el fichero
			porque si lo que vienen es nulo nunca se quita el interes y luego no se puede
			acceder a su valor porque se marca como nulo
			*/
			ClDsItemBlobFile	*pBlobFile = NULL ;

            pBlobFile = (ClDsItemBlobFile*)( DataBuffer + m_pSMData->ItemOffset( iItem ) ) ;

			if( pBlobFile->GetNull() == M4_FALSE )
			{
				pBlobFile->SetNull( M4_TRUE ) ;
			}

            continue;
		}

        if (IsItemBlob (DataBuffer, iItem))
        {
            m4uint8_t *AuxPtr = DataBuffer + m_pSMData->ItemOffset(iItem);
            
            // BlobDataPtr1 points to the actual Blob structure
            BlobDataPtr BlobDataPtr1 = *((BlobDataPtr *)AuxPtr);
            
            if (BlobDataPtr1 != 0)
			{
                m_poMemoryManager->M4free((m4uint8_t *)BlobDataPtr1);
			}
            
            SetItemNullLL(DataBuffer, iItem, M4_TRUE); // xxxx
            *(BlobDataPtr*)AuxPtr = NULL;
        }
    }
}

m4return_t ClItemRecordManager::_SerializeMasks(m4uint8_t* const DataBuffer,
                                                ClGenericIODriver& IOD)
{
   m4uint16_t NMappedItems = m_pSMData->NMappedItems();
   m4uint16_t iItem;
   m4uint8_t* pMask;

   if (NMappedItems == 0)
   {
      return M4_SUCCESS;
   }

   m4uint32_t iSize = ((NMappedItems * M4CL_EMD_NUMBER_OF_MASKS - 1) / 8) + 1;

   if ((pMask = (m4uint8_t*)IOD.GetTempBuffer(iSize)) == 0)
   {
      return M4_ERROR;
   }

   memset(pMask, 0, iSize);   // Me aseguro de que el buffer esta limpito

   m4uint16_t iActualBit = 0;

   for (m4uint16_t i = 0; i < NMappedItems; i++)
   {
      iItem = m_pSMData->GetIndexOfMappedItem(i);

      pMask[iActualBit / 8] |= (((m4uint8_t)IsItemBlob(DataBuffer,iItem)) << (iActualBit % 8));
      iActualBit++;
      pMask[iActualBit / 8] |= (((m4uint8_t)GetItemNull(DataBuffer,iItem)) << (iActualBit % 8));
      iActualBit++;
      pMask[iActualBit / 8] |= (((m4uint8_t)ItemHasSlices(DataBuffer,iItem)) << (iActualBit % 8));
      iActualBit++;

      M4_ASSERT(GetItemPriority(DataBuffer,iItem) >= 0 && GetItemPriority(DataBuffer,iItem) <=3);

      pMask[iActualBit / 8] |= ((GetItemPriority(DataBuffer,iItem) & 0x1) << (iActualBit % 8));
      iActualBit++;
      pMask[iActualBit / 8] |= (((GetItemPriority(DataBuffer,iItem)>>1) & 0x1) << (iActualBit % 8));
      iActualBit++;

      pMask[iActualBit / 8] |= (((m4uint8_t)GetItemIsUser(DataBuffer,iItem)) << (iActualBit % 8));
      iActualBit++;
   }
      
   return IOD.Write(pMask, iSize);
}

m4return_t ClItemRecordManager::Serialize(ClGenericIODriver&      IOD,
                                          const m4uint8_t         ai_iIteSerMod,
										  ClChannelProcess* const ai_pChannelProcess,
										  ClCsTraceSize*    const ai_pCsTraceSize,
                                          m4uint8_t*        const DataBuffer)
{
	m4return_t ret = M4_ERROR;

    if (ai_pChannelProcess) {
		ret = ai_pChannelProcess->Pre_Record_Process(this);
        if (ret != M4_SUCCESS) {
            DUMP_CHLOG_DEBUG(M4_CH_DS_PRE_PROCESS_RECORD);
            return ret;
        }
    }
	
    // Status
    CHECK_CHLOG_ERROR(IOD.Write(*(RegisterStatus_t *)(DataBuffer + M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize())) != M4_SUCCESS,
        M4_ERROR,
        M4_CH_DS_SERIALIZE_RECORD);
	
    // Si el registro es invalido no tengo que serializar nada mas
    if (IsValid(DataBuffer) == M4_TRUE) {
        // Serialize the masks
        switch(ai_iIteSerMod) {
        case M4CL_ITEM_SERIAL_ALL:
			CHECK_CHLOG_ERROR(IOD.Write(DataBuffer, M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize()) != M4_SUCCESS,
				M4_ERROR,
				M4_CH_DS_SERIALIZE_RECORD);
            break;
            
        case M4CL_ITEM_SERIAL_MAPPED:
			CHECK_CHLOG_ERROR(_SerializeMasks(DataBuffer, IOD) != M4_SUCCESS,
				M4_ERROR,
				M4_CH_DS_SERIALIZE_RECORD);
            break;
			
        default:
            M4_ASSERT(0);
            return M4_ERROR;
        }
		
		// RecordID
		ret = IOD.Write((m4uint32_t)GetRecordID(DataBuffer));
		if (ret != M4_SUCCESS) {
			DUMP_CHLOG_ERROR(M4_CH_DS_SERIALIZE_RECORD);
			return ret;
		}
		
		// Si el scope es de registro tenemos que serializar la fecha de última modificación
		if (m_pSMData->Scope() == M4CL_ITEM_SCOPE_REGISTER) {
			VersionMark oVersionMark;
			
			GetUserDataByOffset(DataBuffer,
								&oVersionMark,
								sizeof(regindex_t) + sizeof(regid_t),   // VersionMark offset
								sizeof(VersionMark));
			
			ret = IOD.Write(oVersionMark.Date);
			if (ret != M4_SUCCESS) {
				DUMP_CHLOG_ERROR(M4_CH_DS_SERIALIZE_RECORD);
				return ret;
			}
		}
		
        // Serialize only the items that are not null
        m4uint16_t iItem = 0;
        m4uint16_t NItems = 0;
        m4uint32_t Size = 0;
        m4bool_t   ExistsPreviously = M4_FALSE;
        
        switch(ai_iIteSerMod) {
        case M4CL_ITEM_SERIAL_ALL:
            NItems = m_pSMData->NItems();
            break;
            
        case M4CL_ITEM_SERIAL_MAPPED:
            NItems = m_pSMData->NMappedItems();
            break;
        }

		if (ai_pCsTraceSize) {
			ai_pCsTraceSize->SetOffsetIni(IOD.GetActualOffset());
		}

        for (m4uint16_t i = 0; i < NItems; i++) {
            switch(ai_iIteSerMod) {
            case M4CL_ITEM_SERIAL_ALL:
                iItem = i;
                break;
                
            case M4CL_ITEM_SERIAL_MAPPED:
                iItem = m_pSMData->GetIndexOfMappedItem(i);
                break;
            }
            
            if (GetItemNull(DataBuffer, iItem) == M4_TRUE) {
				continue;
			}

            // If the item is not null
            
            // Alex blob
            if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE) {
                ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(iItem));
				ret = pBlobFile->Serialize(IOD);
                if (ret != M4_SUCCESS) return ret;
                continue;
            }
            
            if (IsItemBlob(DataBuffer, iItem) == M4_TRUE) {
                BlobDataPtr BlobAddress;
                GetItemBlobAddress(DataBuffer,iItem, BlobAddress);
                
                // Special:
                // As the blobs can be multi-pointed (by the actual record and the
                // "lastvalue" of the log, we must take it into account
                CHECK_CHLOG_ERRORF(IOD.WritePrivate(BlobAddress, &ExistsPreviously) != M4_SUCCESS,
                    M4_ERROR,
                    M4_CH_DS_SERIALIZE_ITEM,
                    m_pSMData->ItemHandle(iItem));
                
                if (ExistsPreviously == M4_FALSE) {
                    if (ItemHasSlices(DataBuffer,iItem)) {
                        if (m_pSMData->ItemIsVariableLength(iItem)) {
                            _ClSlices_Variable vvvv;
                            
                            CHECK_CHLOG_ERRORF(vvvv.Serialize(IOD, (m4pchar_t)&BlobAddress->Data[0], m_pSMData->ItemCppType(iItem)) != M4_SUCCESS,
                                M4_ERROR,
                                M4_CH_DS_SERIALIZE_ITEM,
                                m_pSMData->ItemHandle(iItem));
                        } else {
                            _ClSlices_Fixed ffff;
                            
                            CHECK_CHLOG_ERRORF(ffff.Serialize(IOD, (m4pchar_t)&BlobAddress->Data[0], m_pSMData->ItemCppType(iItem)) != M4_SUCCESS,
                                M4_ERROR,
                                M4_CH_DS_SERIALIZE_ITEM,
                                m_pSMData->ItemHandle(iItem));
                        }
                    } else {
                        GetItemSize(DataBuffer,iItem, Size); 
                        
                        CHECK_CHLOG_ERRORF(IOD.Write(&BlobAddress->Data[0], Size) != M4_SUCCESS,
                            M4_ERROR,
                            M4_CH_DS_SERIALIZE_ITEM,
                            m_pSMData->ItemHandle(iItem));
                    }
                    
                    /* POSIBLE OPTIMIZACION 
                    RegisterStatus_t Status=GetStatus();
                    
					 if (Status!=M4_RS_MODIFIED &&
					 Status!=M4_RS_MODIFIED_AND_DELETED)
					 IOD.RemoveAddress(BlobAddress);
                    */
                }
            } else {
                switch (m_pSMData->ItemCppType(iItem)) {
                case M4CL_CPP_TYPE_STRING:
                    GetItemSize(DataBuffer, iItem, Size); // ???? o PRECISION ????
                    
                    CHECK_CHLOG_ERRORF(IOD.Write(&DataBuffer[m_pSMData->ItemOffset(iItem)], Size) != M4_SUCCESS,
                        M4_ERROR,
                        M4_CH_DS_SERIALIZE_ITEM,
                        m_pSMData->ItemHandle(iItem));
                    break;
                    
                case M4CL_CPP_TYPE_NUMBER:
                case M4CL_CPP_TYPE_DATE:
                    CHECK_CHLOG_ERRORF(IOD.Write(*(m4double_t*)&DataBuffer[m_pSMData->ItemOffset(iItem)]) != M4_SUCCESS,
                        M4_ERROR,
                        M4_CH_DS_SERIALIZE_ITEM,
                        m_pSMData->ItemHandle(iItem));
                    break;
                    
                default:
                    M4_ASSERT(0);
                    return M4_ERROR;
                }
            }
        }

		if (ai_pCsTraceSize) {
			ai_pCsTraceSize->SetOffsetEnd(IOD.GetActualOffset());
		}
    }

    if (ai_pChannelProcess) {
		ret = ai_pChannelProcess->Post_Record_Process(this);
        if (ret != M4_SUCCESS) {
            DUMP_CHLOG_DEBUG(M4_CH_DS_POST_PROCESS_RECORD);
            return ret;
        }
    }
    
    return M4_SUCCESS;
}

void ClItemRecordManager::CopyGroupOfMasks(m4uint8_t* const ai_pSourceDataBuffer,
                                           m4uint8_t*       ao_pDestinationDataBuffer,
                                           m4uint16_t const ai_iItemFrom,
                                           m4uint16_t const ai_iItemTo)
{
    for (m4uint16_t iItem = ai_iItemFrom; iItem <= ai_iItemTo; iItem++)
    {
        if (IsItemBlob(ai_pSourceDataBuffer, iItem))
		{
            SetBlobState(ao_pDestinationDataBuffer, iItem);
		}
        else
		{
            SetNoBlobState(ao_pDestinationDataBuffer, iItem);
		}
        
        SetItemNullLL(ao_pDestinationDataBuffer, iItem, GetItemNull(ai_pSourceDataBuffer, iItem));
        SetItemHasSlices(ao_pDestinationDataBuffer, iItem, ItemHasSlices(ai_pSourceDataBuffer, iItem));
        SetItemPriority(ao_pDestinationDataBuffer, iItem, GetItemPriority(ai_pSourceDataBuffer, iItem));
    }
}

m4return_t ClItemRecordManager::_DeSerializeMasks(m4uint8_t*   const DataBuffer,
                                                  ClGenericIODriver& IOD)
{
    m4uint16_t NMappedItems = m_pSMData->NMappedItems();
    m4uint16_t iItem;
    m4char_t   cPriority;
    m4uint8_t* pMask;
    m4uint32_t iSize;
    
    if (NMappedItems == 0)
	{
        return M4_SUCCESS;
	}
    
    if (IOD.ReadSize(iSize) != M4_SUCCESS)
	{
        return M4_ERROR;
	}
    
    if (iSize != ((NMappedItems * M4CL_EMD_NUMBER_OF_MASKS - 1) / 8) + 1)
	{
        return M4_ERROR;
	}
    
    if ((pMask = (m4uint8_t*)IOD.GetTempBuffer(iSize)) == 0)
        return M4_ERROR;
    
    if (IOD.ReadBuffer((void*)pMask) != M4_SUCCESS)
	{
        return M4_ERROR;
	}
    
    m4uint16_t iActualBit = 0;
    
    for (m4uint16_t i = 0; i < NMappedItems; i++)
    {
        iItem = m_pSMData->GetIndexOfMappedItem(i);
        
        if (pMask[iActualBit / 8] & (0x1 << (iActualBit % 8)))
		{
            SetBlobState(DataBuffer, iItem);
		}
        else
		{
            SetNoBlobState(DataBuffer, iItem);
		}
        iActualBit++;
        
        SetItemNullLL(DataBuffer,iItem, (m4bool_t)((pMask[iActualBit / 8] & (0x1 << (iActualBit % 8))) != 0));
        iActualBit++;
        
        SetItemHasSlices(DataBuffer,iItem, (m4bool_t)((pMask[iActualBit / 8] & (0x1 << (iActualBit % 8))) != 0));
        iActualBit++;
        
        if (pMask[iActualBit / 8] & (0x1 << (iActualBit % 8)))
		{
            cPriority = 0x1;
		}
        else
		{
            cPriority = 0x0;
		}
        iActualBit++;
        
        if (pMask[iActualBit / 8] & (0x1 << (iActualBit % 8)))
		{
            cPriority |= 0x2;
		}
        iActualBit++;
        
        SetItemPriority(DataBuffer,iItem, cPriority);

        SetItemIsUser(DataBuffer,iItem, (m4bool_t)((pMask[iActualBit / 8] & (0x1 << (iActualBit % 8))) != 0));
        iActualBit++;
    }
    
    return M4_SUCCESS;
}

void ClItemRecordManager::CleanupDeserialError(const m4uint16_t ai_iItem,
                                               m4uint8_t* const ai_pOldDataBuffer,
                                               m4uint8_t* const ai_pDataBuffer)
{
   // Put it in a normal state (no old val)
   SetStatus(ai_pDataBuffer, M4_RS_NORMAL);

   // Restore the old masks
   CopyGroupOfMasks(ai_pOldDataBuffer, ai_pDataBuffer, ai_iItem, m_pSMData->NItems()-1);
}

m4return_t ClItemRecordManager::DeSerialize(ClGenericIODriver&   IOD,
                                            const m4uint8_t      ai_iIteSerMod,
                                            m4uint8_t*     const DataBuffer,
											ClCsTraceSize* const ai_pCsTraceSize)
{
    // For use in case of an error deserializing
    m4uint8_t* AuxOldBlobMask = (m4uint8_t*)m_poMemoryManager->M4malloc(M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize());
    if (AuxOldBlobMask == NULL) {
       DUMP_CHLOG_ERROR(M4_CH_DS_DESERIALIZE_RECORD);
       return M4_ERROR;
    }
    
    // Store the original values
    memcpy(AuxOldBlobMask, DataBuffer, M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize());

    // Para saber en que parte de la funcion se ha producido el error
    m4uint8_t iStep = 0;     // A nivel de registro

    // Status
    m4return_t iRet =
        IOD.Read(*(RegisterStatus_t *)(DataBuffer + M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize()));

    m4uint16_t iItem = 0;

    // Si el registro es invalido no tengo que deserializar nada mas
    if (IsValid(DataBuffer) == M4_TRUE && iRet == M4_SUCCESS) {
        m4uint32_t AuxSize;

        // Deserialize the blobs and nulls mask
        switch(ai_iIteSerMod) {
        case M4CL_ITEM_SERIAL_ALL:
            iRet = IOD.ReadSize(AuxSize);
            
            if (iRet == M4_SUCCESS) {
                if (AuxSize != (m4uint32_t)(M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize()))
                    iRet = M4_ERROR;
            }

            if (iRet == M4_SUCCESS) {
                iRet = IOD.ReadBuffer(DataBuffer);
			}
            break;
            
        case M4CL_ITEM_SERIAL_MAPPED:
            iRet = _DeSerializeMasks(DataBuffer, IOD);
            break;
            
        default:
            M4_ASSERT(0);
            iRet = M4_ERROR;
        }       
        
        // RecordID
        regid_t iRecordID;
        if (iRet == M4_SUCCESS) {
            iRet = IOD.Read((m4uint32_t&)iRecordID);
		}

        if (iRet == M4_SUCCESS) {
            SetRecordID(DataBuffer, iRecordID);
        
            // Si el scope es de registro tenemos que deserializar la fecha de última modificación
            if (m_pSMData->Scope() == M4CL_ITEM_SCOPE_REGISTER) {
                VersionMark oNewVersionMark;
            
                iRet = IOD.Read(oNewVersionMark.Date);

                if (iRet == M4_SUCCESS) {
                    SetRecordVersion(DataBuffer, oNewVersionMark);
				}
            }
        }

        if (iRet == M4_SUCCESS) {
            iStep = 1;     // A nivel de item
		}

        // DeSerialize only the items that are not null
        m4uint16_t  NItems;
        m4uint32_t  Size;
        m4uint32_t  AuxOffset;
        m4bool_t    ExistsPreviously;
        BlobDataPtr BlobDataAddress;

        switch(ai_iIteSerMod) {
        case M4CL_ITEM_SERIAL_ALL:
            NItems = m_pSMData->NItems();
            break;
            
        case M4CL_ITEM_SERIAL_MAPPED:
            NItems = m_pSMData->NMappedItems();
            break;
        }

		if (ai_pCsTraceSize) {
			ai_pCsTraceSize->SetOffsetIni(IOD.GetActualOffset());
		}

        for (m4uint16_t i = 0; i < NItems && iRet == M4_SUCCESS; i++) {
            switch(ai_iIteSerMod) {
            case M4CL_ITEM_SERIAL_ALL:
                iItem = i;
                break;
                
            case M4CL_ITEM_SERIAL_MAPPED:
                iItem = m_pSMData->GetIndexOfMappedItem(i);
                break;
            }

            // Comprobamos si es un item nulo
            if (GetItemNull(DataBuffer, iItem) == M4_FALSE) {
                // Alex blob file
                if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE) {
                    ClDsItemBlobFile* pBlobFile =
                        (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(iItem));
                    iRet = pBlobFile->DeSerialize(IOD);
                    continue;
                }
                
                // Comprobamos si es un blob en memoria
                if (IsItemBlob(DataBuffer, iItem) == M4_TRUE) {
                    iRet = IOD.ReadPrivate((void **)&BlobDataAddress, &ExistsPreviously);
                    
                    if (iRet == M4_SUCCESS) {
                        if (BlobDataAddress == NULL) {
                            if (ExistsPreviously == M4_FALSE) {
                                AuxOffset = IOD.GetActualOffset();
                                
                                if (ItemHasSlices(DataBuffer, iItem)) {
                                    if (m_pSMData->ItemIsVariableLength(iItem) == M4_TRUE) {
                                        _ClSlices_Variable vvvv;
                                        
                                        vvvv.DeSerializedSize(IOD, AuxSize);
                                        
                                        BlobDataAddress = (BlobDataPtr)m_poMemoryManager->M4malloc(AuxSize + sizeof(BlobData)-1);
                                        if (BlobDataAddress == NULL) {
                                            iRet = M4_ERROR;
										}

                                        if (iRet == M4_SUCCESS) {
                                            iRet = vvvv.DeSerialize(IOD, (m4pchar_t)&BlobDataAddress->Data[0], m_pSMData->ItemCppType(iItem), AuxSize);
										}
                                    } else {
                                        _ClSlices_Fixed ffff;
                                        
                                        ffff.DeSerializedSize(IOD,AuxSize);
                                        
                                        BlobDataAddress = (BlobDataPtr)m_poMemoryManager->M4malloc(AuxSize + sizeof(BlobData)-1);
                                        if (BlobDataAddress == NULL) {
                                            iRet = M4_ERROR;
										}

                                        if (iRet == M4_SUCCESS) {
                                            iRet = ffff.DeSerialize(IOD, (m4pchar_t)&BlobDataAddress->Data[0], m_pSMData->ItemCppType(iItem), AuxSize);
										}
                                    }
                                } else {
                                    // Getting the neccesary memory
                                    iRet = IOD.ReadSize(AuxSize);

                                    if (iRet == M4_SUCCESS) {
                                        BlobDataAddress = (BlobDataPtr)m_poMemoryManager->M4malloc(AuxSize + sizeof(BlobData)-1);
                                        if (BlobDataAddress == NULL) {
                                            iRet = M4_ERROR;
										}
                                    }
                                    
                                    // Assign the blob value
                                    if (iRet == M4_SUCCESS) {
                                        iRet = IOD.ReadBuffer(&BlobDataAddress->Data[0]);
									}
                                }

                                // Sets the size of the blob created
                                if (iRet == M4_SUCCESS) {
                                    BlobDataAddress->Size = AuxSize;
                                
                                    /* POSIBLE OPTIMIZACION, QUE HAY QUE ANALIZAR UN POCO MAS
                                    RegisterStatus_t Status=GetStatus();
                            
                                      if (Status==M4_RS_MODIFIED ||
                                      Status==M4_RS_MODIFIED_AND_DELETED)
                                    */
                                    iRet = IOD.StoreAddressPrivate(BlobDataAddress, AuxOffset);
                                }
                            }
                            //else
                            //    ; // Is a null one ??????
                        }
                        
                        // Assign the PTR to the blob
                        m4uint8_t*   AuxPtr = &DataBuffer[m_pSMData->ItemOffset(iItem)];
                        BlobDataPtr* BDP    = (BlobDataPtr *)AuxPtr;
                        
                        *BDP = BlobDataAddress;
                    }
                    continue;
                }

                // Tiene que ser un item normal
                switch (m_pSMData->ItemCppType(iItem)) {
                case M4CL_CPP_TYPE_STRING:
                    iRet = IOD.ReadSize(AuxSize);
                    
                    if (iRet == M4_SUCCESS) {
                        GetItemSize(DataBuffer, iItem, Size); // ???? o PRECISION ????
                        if (AuxSize > Size) {
                            iRet =  M4_ERROR;
						}
                    }
                    
                    if (iRet == M4_SUCCESS) {
                        iRet = IOD.ReadBuffer(&DataBuffer[m_pSMData->ItemOffset(iItem)]);
					}
                    break;
                    
                case M4CL_CPP_TYPE_NUMBER:
                case M4CL_CPP_TYPE_DATE:
                    iRet = IOD.Read(*(m4double_t*)&DataBuffer[m_pSMData->ItemOffset(iItem)]);
                    break;
                    
                default:
                    M4_ASSERT(0);
                    iRet = M4_ERROR;
                }
            } else {
                if (IsItemBlob(DataBuffer, iItem) == M4_TRUE) {
                    iRet = SetItemBlobNullNoRelease(DataBuffer, iItem, M4_TRUE);
				}
            }
        }

		if (ai_pCsTraceSize) {
			ai_pCsTraceSize->SetOffsetEnd(IOD.GetActualOffset());
		}
    }

    // Tratamiento de error
    if (iRet != M4_SUCCESS) {
        switch(iStep) {
        case 0:     // A nivel de registro
            CleanupDeserialError(0, AuxOldBlobMask, DataBuffer);
            DUMP_CHLOG_ERROR(M4_CH_DS_DESERIALIZE_RECORD);
            break;

        case 1:     // A nivel de item
            CleanupDeserialError(iItem, AuxOldBlobMask, DataBuffer);
            DUMP_CHLOG_ERRORF(M4_CH_DS_SERIALIZE_ITEM, m_pSMData->ItemHandle(iItem));
            break;

        default:
            M4_ASSERT(0);
        }
    }

    m_poMemoryManager->M4free(AuxOldBlobMask);

    return iRet;
}

m4return_t ClItemRecordManager::GetSize(ClSize& ao_size) const
{
    ao_size += sizeof(ClItemRecordManager);
    return M4_SUCCESS;
}

void ClItemRecordManager::SetRecordVersion(m4uint8_t* const  DataBuffer,
                                           const VersionMark ai_Version)
{
    // Antes de poner el nuevo VersinoMark guardo el antiguo
    VersionMark oAuxVersionMark;

    // Calculamos el offset del VersionMark y del OldVersionMark
    const m4uint32_t iVMOffset    = sizeof(regindex_t) + sizeof(regid_t);
    const m4uint32_t iOldVMOffset = sizeof(regindex_t) + sizeof(regid_t) + sizeof(VersionMark);
    const m4uint32_t iVMSize = sizeof(VersionMark);

    GetUserDataByOffset(DataBuffer, &oAuxVersionMark, iVMOffset, iVMSize);
    
    SetUserDataByOffset(DataBuffer, &oAuxVersionMark, iOldVMOffset, iVMSize);
    
    // Fijamos el nuevo VersionMark
    SetUserDataByOffset(DataBuffer, &ai_Version, iVMOffset, iVMSize);
}
