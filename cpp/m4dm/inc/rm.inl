
#ifndef _RM_INL_
#define _RM_INL_

#include "emd.hpp"
#include "dsitem.hpp"
#include "memman.hpp"

M4_INLINE ClItemRecordManager::ClItemRecordManager(const EMDStaticData* const ai_pSMData) :
    m_pSMData(ai_pSMData)
{
}

M4_INLINE RegisterStatus_t ClItemRecordManager::GetStatus(m4uint8_t* const DataBuffer) const
{
   return (*(RegisterStatus_t *)(DataBuffer + M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize())) & M4_RS_MASK;
}


M4_INLINE m4bool_t ClItemRecordManager::IsValid(void* ai_DataBuffer) const
{
   return (GetStatus((m4uint8_t *)ai_DataBuffer) == M4_RS_INVALID ? M4_FALSE : M4_TRUE);
}


M4_INLINE void ClItemRecordManager::SetMetaData(const EMDStaticData* const ai_pSMData)
{
    m_pSMData = ai_pSMData;
}

M4_INLINE m4bool_t ClItemRecordManager::IsItemBlob(m4uint8_t* const DataBuffer,
                                                   const m4uint16_t ai_ItemPos) const
{
   if (*(DataBuffer+(ai_ItemPos / 8)) & (1 << (ai_ItemPos % 8)))
   {
      return M4_TRUE;
   }
   else
   {
      return M4_FALSE;
   }
}
 
//EDU------------->>
// Determinamos si un item tiene o no tramos de la siguiente (peregrina) forma:
// tiene tramos (es posible que tenga) sí actualmente el item es un blob, pero 
// inicialmente no lo era.

// CRG NUEVA (9/1/98) esta es mantenida por el usuario ...
M4_INLINE m4bool_t ClItemRecordManager::ItemHasSlices(m4uint8_t* const DataBuffer,
                                                      const m4uint16_t ai_ItemPos) const
{
    // Alex blob
    /*
    ??? No llamo l afuncion porque entraria en una recursion
    ??? sigo el camino habitual
    if (EMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = DataBuffer + EMData->ItemOffset(ai_ItemPos);
        return pBlobFile->GetHasSlices(ai_bIsNull);
    }
    */

   if ((*(DataBuffer+2*m_pSMData->BlobMaskSize()+(ai_ItemPos / 8)) & (1 << (ai_ItemPos % 8)))!=0)
   {
      return M4_TRUE;
   }
   else
   {
      return M4_FALSE;
   }
}
//EDU-------------<<

M4_INLINE void ClItemRecordManager::SetItemHasSlices(m4uint8_t* const DataBuffer,
                                                     const m4uint16_t ai_ItemPos,
                                                     const m4bool_t   ai_iHasSlices)
{
    // Alex blob
    /*
    ??? No llamo l afuncion porque entraria en una recursion
    ??? sigo el camino habitual
    if (EMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = DataBuffer + EMData->ItemOffset(ai_ItemPos);
        return pBlobFile->SetHasSlices(ai_bIsNull);
    }
    */
    
   if (ai_iHasSlices)
   {
      *(DataBuffer+2*m_pSMData->BlobMaskSize()+(ai_ItemPos / 8))|=(1 << (ai_ItemPos % 8));
   }
   else
   {
      *(DataBuffer+2*m_pSMData->BlobMaskSize()+(ai_ItemPos / 8))&=~(1 << (ai_ItemPos % 8));
   }
}

M4_INLINE m4char_t ClItemRecordManager::GetItemPriority(m4uint8_t* const DataBuffer,
                                                        const m4uint16_t ai_ItemPos) const
{
    // Alex blob
    /*
    ??? No llamo l afuncion porque entraria en una recursion
    ??? sigo el camino habitual
    if (EMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = DataBuffer + EMData->ItemOffset(ai_ItemPos);
        return pBlobFile->GetPriority(ai_bIsNull);
    }
    */

   m4int32_t RetPrioridad;

   RetPrioridad = *(DataBuffer + 3 * m_pSMData->BlobMaskSize() + (2 * ai_ItemPos / 8));
   RetPrioridad&=( (1 << (2*ai_ItemPos % 8)) | (1 << ((2*ai_ItemPos % 8) + 1)) );
   RetPrioridad>>=(2*ai_ItemPos % 8);

   return (m4char_t)RetPrioridad;
}

M4_INLINE void ClItemRecordManager::SetItemPriority(m4uint8_t* const DataBuffer,
                                                    const m4uint16_t ai_ItemPos,
                                                    const m4char_t   ai_iPriority)
{
    // Alex blob
    /*
    ??? No llamo l afuncion porque entraria en una recursion
    ??? sigo el camino habitual
    if (EMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = DataBuffer + EMData->ItemOffset(ai_ItemPos);
        return pBlobFile->SetPriority(ai_bIsNull);
    }
    */

   // The priority should be from 0 to 3 !!!!!!!!

   // Clearing the corresponding bits...
   *(DataBuffer+3*m_pSMData->BlobMaskSize()+(2*ai_ItemPos / 8))&=
       (~((1 << (2*ai_ItemPos % 8)) | (1 << ((2*ai_ItemPos % 8) + 1))));

   // Assigning it again
    *(DataBuffer + 3 * m_pSMData->BlobMaskSize() + (2 * ai_ItemPos / 8)) |=
        (ai_iPriority << (2 * ai_ItemPos % 8));
}

M4_INLINE m4bool_t ClItemRecordManager::GetItemIsUser(m4uint8_t* const DataBuffer,
                                                      const m4uint16_t ai_ItemPos) const
{
   if ((*(DataBuffer+5*m_pSMData->BlobMaskSize()+(ai_ItemPos / 8)) & (1 << (ai_ItemPos % 8)))!=0)
   {
      return M4_TRUE;
   }
   else
   {
      return M4_FALSE;
   }
}

M4_INLINE void ClItemRecordManager::SetItemIsUser(m4uint8_t* const DataBuffer,
                                                     const m4uint16_t ai_ItemPos,
                                                     const m4bool_t   ai_bIsUser)
{
   if (ai_bIsUser)
   {
      *(DataBuffer+5*m_pSMData->BlobMaskSize()+(ai_ItemPos / 8))|=(1 << (ai_ItemPos % 8));
   }
   else
   {
      *(DataBuffer+5*m_pSMData->BlobMaskSize()+(ai_ItemPos / 8))&=~(1 << (ai_ItemPos % 8));
   }
}

M4_INLINE void ClItemRecordManager::SetBlobState(m4uint8_t* const DataBuffer,
                                                 const m4uint16_t ai_ItemPos)
{
   *(DataBuffer+(ai_ItemPos / 8)) |= (1 << (ai_ItemPos % 8));
}

M4_INLINE void ClItemRecordManager::SetNoBlobState(m4uint8_t* const DataBuffer,
                                                   const m4uint16_t ai_ItemPos)
{
   *(DataBuffer+(ai_ItemPos / 8)) &= (~((1 << (ai_ItemPos % 8))));
}

M4_INLINE void ClItemRecordManager::SetItemNullLL(m4uint8_t* const DataBuffer,
                                                  const m4uint16_t ai_ItemPos,
                                                  const m4bool_t   ai_IsNull)
{
   if (ai_IsNull)
   {
      *(DataBuffer+m_pSMData->BlobMaskSize()+(ai_ItemPos / 8))|=(1 << (ai_ItemPos % 8));
   }
   else
   {
      *(DataBuffer+m_pSMData->BlobMaskSize()+(ai_ItemPos / 8))&=~(1 << (ai_ItemPos % 8));
   }
}

M4_INLINE m4return_t ClItemRecordManager::SetItemBlobNull(m4uint8_t* const DataBuffer,
                                                          const m4uint16_t ai_ItemPos,
                                                          const m4bool_t   ai_Release,
                                                          const m4bool_t   ai_IsNull)
{
    M4_ASSERT(IsItemBlob(DataBuffer, ai_ItemPos) == M4_TRUE);

    m4uint8_t *AuxPtr = DataBuffer + m_pSMData->ItemOffset(ai_ItemPos);
    // BlobDataPtr1 points to the actual Blob structure
    BlobDataPtr BlobDataPtr1 = *((BlobDataPtr *)AuxPtr);
    
    if (BlobDataPtr1 != 0)
    {
        if (ai_IsNull)
        {
            if (ai_Release)
			{
                m_poMemoryManager->M4free((m4uint8_t *)BlobDataPtr1);
			}
            
            *((BlobDataPtr*)AuxPtr)=0;
            
            SetItemNullLL(DataBuffer,ai_ItemPos,M4_TRUE);
        }
    }
    else
    {
        // SITUACION ERRONEA !!!!!
        if (!ai_IsNull)
		{
            return M4_ERROR;
		}
    }
    
    return M4_SUCCESS;
}


M4_INLINE m4return_t ClItemRecordManager::SetItemBlobNullRelease(m4uint8_t* const DataBuffer,
                                                                 const m4uint16_t ai_ItemPos,
                                                                 const m4bool_t   ai_IsNull)
{
   return SetItemBlobNull(DataBuffer,
                          ai_ItemPos,
                          M4_TRUE,
                          ai_IsNull);
}

M4_INLINE m4return_t ClItemRecordManager::SetItemBlobNullNoRelease(m4uint8_t* const DataBuffer,
                                                                   const m4uint16_t ai_ItemPos,
                                                                   const m4bool_t   ai_IsNull)
{
   return SetItemBlobNull(DataBuffer,
                          ai_ItemPos,
                          M4_FALSE,
                          ai_IsNull);
}


M4_INLINE m4return_t ClItemRecordManager::SetItemNoBlobNull(m4uint8_t* const DataBuffer,
                                                            const m4uint16_t ai_ItemPos,
                                                            const m4bool_t   ai_IsNull)
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));
        return pBlobFile->SetNull(ai_IsNull);
    }

   SetItemNullLL(DataBuffer,ai_ItemPos,ai_IsNull);

   return M4_SUCCESS;
}

M4_INLINE m4return_t ClItemRecordManager::SetItemNull(m4uint8_t* const DataBuffer,
                                                      const m4uint16_t ai_ItemPos,
                                                      const m4bool_t   ai_IsNull)
{
   if (IsItemBlob(DataBuffer,ai_ItemPos))
   {
      return SetItemBlobNull(DataBuffer,ai_ItemPos,M4_TRUE,ai_IsNull);
   }

    // Alex blob
    //SetItemNullLL(DataBuffer, ai_ItemPos, ai_IsNull);
    return SetItemNoBlobNull(DataBuffer, ai_ItemPos, ai_IsNull);
}

M4_INLINE m4bool_t ClItemRecordManager::GetItemNull(m4uint8_t* const DataBuffer,
                                                    const m4uint16_t ai_ItemPos) const
{
    // Alex blob
    /*
    ??? No llamo l afuncion porque entraria en una recursion
    ??? sigo el camino habitual
    if (EMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = DataBuffer + EMData->ItemOffset(ai_ItemPos);
        return pBlobFile->GetNull(ai_bIsNull);
    }
    */

   if ((*(DataBuffer+m_pSMData->BlobMaskSize()+(ai_ItemPos / 8)) & (1 << (ai_ItemPos % 8)))!=0)
   {
      return M4_TRUE;
   }
   else
   {
      return M4_FALSE;
   }
}

M4_INLINE void ClItemRecordManager::InitRecordNoDefaultValues(m4uint8_t* const DataBuffer)
{
   ////EDU
   if (m_pSMData->BlobMaskSize()>0)
   {
      memcpy(DataBuffer,m_pSMData->BlobMask(),m_pSMData->BlobMaskSize());
      // Initially, all the fields are nulls
      memset(&DataBuffer[m_pSMData->BlobMaskSize()],0xff,m_pSMData->BlobMaskSize());
      // Setting the ItemHasSlices to false 
      memset(&DataBuffer[2*m_pSMData->BlobMaskSize()],0x00,m_pSMData->BlobMaskSize());
      // Setting the Priorities to 3
      memset(&DataBuffer[3*m_pSMData->BlobMaskSize()],0xff,2*m_pSMData->BlobMaskSize());
   }
}

// We assume that the from record is of the same type
// Esto solo copia las cadenas de tamaño variable, por eso nos saltamos los blobFile
M4_INLINE void ClItemRecordManager::CopyBlobsFromRecord(m4uint8_t* const DataBuffer,
                                                        void*            ai_pFromRecord)
{
    ClItemRecordManager OtherIRM(m_pSMData);
    m4uint32_t iBiggestBlobSize=0;
    void       *Buffer = NULL;
    
    for (m4uint16_t iItem = 0; iItem < m_pSMData->NItems(); iItem++)
    {
        if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE)
		{
            continue;
		}
        
        if (m_pSMData->ItemIsVariableLength(iItem))
        {
            if (OtherIRM.GetItemNull((m4uint8_t*)ai_pFromRecord, iItem) == M4_FALSE)
            {
                m4uint32_t iSize;
                
                OtherIRM.GetItemSize((m4uint8_t*)ai_pFromRecord, iItem, iSize);
                
                if (iSize > 0)
                {
                    if (iSize > iBiggestBlobSize)
                    {
                        //if (iBiggestBlobSize>0)
                        if (Buffer)
						{
                            m_poMemoryManager->M4free((m4uint8_t *)Buffer);
						}
                        
                        Buffer = m_poMemoryManager->M4malloc(iSize);
                        iBiggestBlobSize = iSize;
                    }
                    M4_ASSERT(Buffer);
                    
                    m4bool_t bIsNull;
                    
                    OtherIRM.GetItemValue((m4uint8_t*)ai_pFromRecord, iItem, Buffer, iSize, iSize, bIsNull);
                    
                    M4_ASSERT(bIsNull == M4_FALSE);
                    
                    SetItemValueNoRelease(DataBuffer, iItem, Buffer, iSize);
                }
                else
				{
                    SetItemValueNoRelease(DataBuffer, iItem, "valid address", 0);
				}
            }
        }
    }
    
    if (iBiggestBlobSize > 0)
	{
        m_poMemoryManager->M4free((m4uint8_t *)Buffer);
	}
}

M4_INLINE void ClItemRecordManager::InitRecord(m4uint8_t* const DataBuffer)
{
   // Init the header
   InitRecordNoDefaultValues(DataBuffer);
   
   // Put the default values:
   // Static part of record...
   if (m_pSMData->DefaultRecord() != NULL)
   {
      // BlobMask and null bitmask
      memcpy(DataBuffer,
         m_pSMData->DefaultRecord(),
         m_pSMData->BlobMaskSize()*M4CL_EMD_NUMBER_OF_MASKS);
      
      // Field default values (from item o to the end)
      
      m4uint16_t iIndexOfFirstOff = m_pSMData->IndexOfFirstOffset();
      m4uint32_t iFirstOffset=m_pSMData->ItemOffset(iIndexOfFirstOff);
      memcpy(&DataBuffer[iFirstOffset],
            &m_pSMData->DefaultRecord()[iFirstOffset],
            m_pSMData->RecordSize()-iFirstOffset);
         
         // Blobs !
      CopyBlobsFromRecord(DataBuffer,(void*)(m_pSMData->DefaultRecord()));
      
   }
}

M4_INLINE void ClItemRecordManager::CreateAllItemBlobFile(m4uint8_t* const          DataBuffer,
                                                          ClVectorRecordBase* const ai_pVRB)
{
    // Llamar a los constructores de los BlobFile
    if (m_pSMData->SomeItemLivesInFile() == M4_TRUE)
    {
        m4uint16_t iNumItems = m_pSMData->NItems();
        for (m4uint16_t iItem = 0; iItem < iNumItems; iItem++)
        {
            if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE)
            {
                new(DataBuffer + m_pSMData->ItemOffset(iItem)) ClDsItemBlobFile(
                    ai_pVRB,
                    this,
                    DataBuffer,
                    iItem);
            }
        }
    }
}

M4_INLINE void ClItemRecordManager::DestroyAllItemBlobFile(m4uint8_t* const DataBuffer)
{
	// Llamar a los destructores de los BlobFile
    if (m_pSMData->SomeItemLivesInFile() == M4_TRUE)
    {
        m4uint16_t iNumItems = m_pSMData->NItems();
	    for (m4uint16_t iItem = 0; iItem < iNumItems; iItem++)
	    {
		    if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE)
			{
			    delete (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(iItem));
			}
	    }
    }
}

M4_INLINE void ClItemRecordManager::GetUserDataByOffset(const m4uint8_t* const DataBuffer,
                                                        void*            const ao_UserData,
                                                        const m4uint16_t       ai_Offset,
                                                        const m4uint16_t       ai_Size) const
{
   M4_ASSERT(ai_Offset + ai_Size <= m_pSMData->UDataSize());

   memcpy(ao_UserData, DataBuffer + m_pSMData->UDataOffset() + ai_Offset, ai_Size);
}

M4_INLINE void ClItemRecordManager::SetUserDataByOffset(m4uint8_t*       const DataBuffer,
                                                        const void*      const ai_UserData,
                                                        const m4uint16_t       ai_Offset,
                                                        const m4uint16_t       ai_Size)
{
    M4_ASSERT(ai_Offset + ai_Size <= m_pSMData->UDataSize());
    
    memcpy(DataBuffer + m_pSMData->UDataOffset() + ai_Offset, ai_UserData, ai_Size);
}

M4_INLINE m4return_t ClItemRecordManager::GetItemDataAddress(m4uint8_t* const DataBuffer,
                                                             const m4uint16_t ai_ItemPos,
                                                             p_item_t&        ao_ItemAddress) const
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));

        ao_ItemAddress = (void*)pBlobFile->GetValueAddress();
        return M4_SUCCESS;
    }

   m4uint8_t *AuxPtr = DataBuffer + m_pSMData->ItemOffset(ai_ItemPos);

   if (IsItemBlob(DataBuffer,ai_ItemPos) == M4_TRUE)
   {
      // BlobDataPtr1 points to the actual Blob structure
      BlobDataPtr BlobDataPtr1 = *((BlobDataPtr *)AuxPtr);

      if (BlobDataPtr1 == 0)
	  {
         ao_ItemAddress = 0;
	  }
      else
	  {
         ao_ItemAddress = (p_item_t)&BlobDataPtr1->Data[0];
	  }
   }
   else
   {
      if (GetItemNull(DataBuffer,ai_ItemPos) == M4_TRUE)
	  {
         ao_ItemAddress = 0;
	  }
      else
	  {
         ao_ItemAddress = (p_item_t)AuxPtr;
	  }
   }

   return M4_SUCCESS;
}


M4_INLINE m4return_t ClItemRecordManager::GetItemBlobAddress(m4uint8_t* const DataBuffer,
                                                             const m4uint16_t ai_ItemPos,
                                                             BlobDataPtr&     ao_BlobAddress) const
{
    if (IsItemBlob(DataBuffer, ai_ItemPos) == M4_TRUE)
    {
        m4uint8_t *AuxPtr = DataBuffer + m_pSMData->ItemOffset(ai_ItemPos);
        // BlobDataPtr1 points to the actual Blob structure
        ao_BlobAddress = *((BlobDataPtr *)AuxPtr);
    }
    else
	{
        ao_BlobAddress = NULL;
	}
    
    return M4_SUCCESS;
}

M4_INLINE m4uint32_t ClItemRecordManager::GetRecordSize(void) const
{
   return m_pSMData->RecordSize();
}

M4_INLINE void ClItemRecordManager::_SetFlag(m4uint8_t* const DataBuffer,
                                             const m4uint16_t ai_iFlag,
                                             const m4bool_t   ai_bIsModified)
{
   if (ai_bIsModified == M4_TRUE)
   {
      *(RegisterStatus_t *)(DataBuffer + M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize()) |= ai_iFlag;
   }
   else
   {
      *(RegisterStatus_t *)(DataBuffer + M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize()) &= ~ai_iFlag;
   }
}



M4_INLINE void ClItemRecordManager::SetModifiedInDB(m4uint8_t* const DataBuffer,
                                                    const m4bool_t ai_bIsModified)
{
   _SetFlag(DataBuffer,M4_RF_IN_DB, ai_bIsModified);
}


M4_INLINE void ClItemRecordManager::SetStatus(m4uint8_t* const DataBuffer,
                                              RegisterStatus_t ai_NewStatus)
{
   *(RegisterStatus_t *)(DataBuffer + M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize()) &= ~M4_RS_MASK; 
   *(RegisterStatus_t *)(DataBuffer + M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize()) |= ai_NewStatus;

   if (ai_NewStatus==M4_RS_NORMAL)
   {
      SetModifiedInDB(DataBuffer,M4_FALSE); // Reset the persist in db flag
   }
}

M4_INLINE m4bool_t ClItemRecordManager::_GetFlag(m4uint8_t* const DataBuffer,
                                                 const m4uint16_t ai_iFlag) const
{
   if ((*(RegisterStatus_t *)(DataBuffer + M4CL_EMD_NUMBER_OF_MASKS * m_pSMData->BlobMaskSize())) & ai_iFlag)
   {
      return M4_TRUE;
   }
   else
   {
      return M4_FALSE;
   }
}

M4_INLINE m4bool_t ClItemRecordManager::GetModifiedInDB(m4uint8_t* const DataBuffer) const
{
   return _GetFlag(DataBuffer,M4_RF_IN_DB);
}

M4_INLINE m4bool_t ClItemRecordManager::GetPersistLDB(m4uint8_t* const DataBuffer) const
{
   return _GetFlag(DataBuffer, M4_RF_PERSIST_LDB);
}

M4_INLINE void ClItemRecordManager::SetPersistLDB(m4uint8_t* const DataBuffer,
                                                  const m4bool_t ai_bIsPersisted)
{
   _SetFlag(DataBuffer, M4_RF_PERSIST_LDB, ai_bIsPersisted);
}

M4_INLINE m4bool_t ClItemRecordManager::GetNewNotPersist(m4uint8_t* const DataBuffer) const
{
	return _GetFlag(DataBuffer, M4_RF_NEW_NOT_PERSIST);
}

M4_INLINE void ClItemRecordManager::SetNewNotPersist(m4uint8_t* const DataBuffer, const m4bool_t ai_bNewNotPersist)
{
	_SetFlag(DataBuffer, M4_RF_NEW_NOT_PERSIST, ai_bNewNotPersist);
}

M4_INLINE void ClItemRecordManager::GetNeedSerial(m4uint8_t* const DataBuffer,
                                                  m4bool_t& ao_bNeedSerial,
                                                  m4bool_t& ao_bBranchMode) const
{
   ao_bNeedSerial = _GetFlag(DataBuffer,M4_RF_NEED_SERIAL);
   ao_bBranchMode = _GetFlag(DataBuffer,M4_RF_BRANCH_MODE_SERIAL);
}

M4_INLINE void ClItemRecordManager::SetNeedSerial(m4uint8_t* const DataBuffer,
                                                  const m4bool_t ai_bNeedSerial,
                                                  const m4bool_t ai_bBranchMode)
{
   _SetFlag(DataBuffer,M4_RF_NEED_SERIAL, ai_bNeedSerial);
   _SetFlag(DataBuffer,M4_RF_BRANCH_MODE_SERIAL, ai_bBranchMode);
}

M4_INLINE void ClItemRecordManager::TestAndSetNeedSerial(m4uint8_t* const DataBuffer,
                                                         m4bool_t&        ao_bBeforeNeedSerial,
                                                         m4bool_t&        ao_bBeforeBranchMode,
                                                         const m4bool_t   ai_bNeedSerial,
                                                         const m4bool_t   ai_bBranchMode)
{
   ao_bBeforeNeedSerial = _GetFlag(DataBuffer,M4_RF_NEED_SERIAL);
   ao_bBeforeBranchMode = _GetFlag(DataBuffer,M4_RF_BRANCH_MODE_SERIAL);

   _SetFlag(DataBuffer,M4_RF_NEED_SERIAL, ai_bNeedSerial);
   _SetFlag(DataBuffer,M4_RF_BRANCH_MODE_SERIAL, ai_bBranchMode);
}

// Luis 11/1 / 99
M4_INLINE const EMDStaticData* const ClItemRecordManager::GetpEMDStaticData(void) const
{
	return m_pSMData;
}

M4_INLINE regid_t ClItemRecordManager::GetRecordID(m4uint8_t* const DataBuffer) const
{
    regid_t iRecordID;
    GetUserDataByOffset(DataBuffer, &iRecordID, sizeof(regindex_t), sizeof(regid_t));
    return iRecordID;
}

M4_INLINE void ClItemRecordManager::SetRecordID(m4uint8_t* const DataBuffer,
                                                const regid_t    ai_iRecordID)
{
    SetUserDataByOffset(DataBuffer, (void*)&ai_iRecordID, sizeof(regindex_t), sizeof(regid_t));
}

#endif
