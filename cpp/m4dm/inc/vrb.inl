#ifndef __VRB__INL__
#define __VRB__INL__

#include <stdio.h>

#include "emd.hpp"
#include "dsitem.hpp"
#include "vdelta.hpp"
#include "memman.hpp"
#include "threadobjs.hpp"
#include "rowsquota.hpp"

M4_INLINE RegisterStatus_t ClVectorRecordBase::RecordStatus(const regindex_t ai_Index) const
{
   M4_MSG_ASSERT(ai_Index<Count && ai_Index>=0,"Index fuera de rango en ClVectorRecordBase::RecordStatus");

    if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
        return M4_RS_INVALID;

   // DEBO RETORNAR AL CLIENTE EN CASO DE MOD+DELETED SOLO DELETED ??? CRG
   return IRM.GetStatus((m4uint8_t*)Index[ai_Index]);
}
M4_INLINE regindex_t ClVectorRecordBase::GetLastRecordValueIndex(const regindex_t ai_iIndex) const
{
   regindex_t LastRecordIndex;

   IRM.GetUserDataByOffset((m4uint8_t*)Index[ai_iIndex], &LastRecordIndex, 0, sizeof(LastRecordIndex));

   // LastRecord is not an absolute address. We need to store internaly as
   // a negative offset from the end of the index, so we need to calculate it
   LastRecordIndex = Size - LastRecordIndex;
   return LastRecordIndex;
}


M4_INLINE regindex_t ClVectorRecordBase::GetLastRecordValueIndexByIndex(regindex_t ai_Index) const
{
   regindex_t LastRecordIndex;

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   LastRecordIndex = GetLastRecordValueIndex(ai_Index);

   return LastRecordIndex;
}


M4_INLINE m4uint8_t* ClVectorRecordBase::_LastRecordToProcess(const regindex_t ai_iIndex) const
{
    M4_ASSERT(ai_iIndex < Count && ai_iIndex >= 0);
    
    void* ActiveRecord;
    
    if (UseCP == M4_TRUE)
    {
        RegisterStatus_t Status = RecordStatus(ai_iIndex);
        
        switch(Status)
        {
        case M4_RS_MODIFIED:
        case M4_RS_MODIFIED_AND_DELETED:
            // There is a last version
            ActiveRecord = Index[GetLastRecordValueIndexByIndex(ai_iIndex)];
            if (IRM.IsValid(ActiveRecord) != M4_TRUE)
                return NULL;
            break;
            
        case M4_RS_NORMAL:
        case M4_RS_DELETED:
            // Normal way
            ActiveRecord = Index[ai_iIndex];
            break;
            
        default:     // M4_RS_INVALID, M4_RS_NEW, M4_RS_NEW_AND_DELETED
            return NULL;
        }
    }
    else
    {
        // Normal way
		ActiveRecord = Index[ai_iIndex];
        if (IRM.IsValid(ActiveRecord) != M4_TRUE)
            return NULL;
    }

    return (m4uint8_t*)ActiveRecord;
}


M4_INLINE m4return_t ClVectorRecordBase::GetLastItemNullByPos(const m4uint16_t ai_ItemPos,
                                                              const regindex_t ai_Index,
                                                              m4bool_t&        ao_IsNull) const
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

    m4uint8_t* ActiveRecord = _LastRecordToProcess(ai_Index);
    if (ActiveRecord == NULL)
        return M4_ERROR;

    ao_IsNull = IRM.GetItemNull(ActiveRecord, ai_ItemPos);

    return M4_SUCCESS;
}

M4_INLINE m4return_t ClVectorRecordBase::GetItemNullByPos(const m4uint16_t ai_ItemPos,
                                                          const regindex_t ai_Index,
                                                          m4bool_t&        ao_IsNull) const
{
   M4_MSG_ASSERT(ai_Index<Count && ai_Index>=0,"Index fuera de rango en ClVectorRecordBase::GetItemNullByPos");
   M4_MSG_ASSERT(ai_ItemPos<m_pSMData->NItems(),"Item fuera de rango en ClVectorRecordBase::GetItemNullByPos");

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE) {
      ao_IsNull = M4_TRUE;
      return M4_ERROR;
   }

   ao_IsNull=IRM.GetItemNull((m4uint8_t*)Index[ai_Index],ai_ItemPos);
 
   return M4_SUCCESS;
}

M4_INLINE m4return_t ClVectorRecordBase::GetItemDataAddressByPos(const m4uint16_t ai_ItemPos,
                                                                 const regindex_t ai_Index,
                                                                 p_item_t&        ao_ItemAddress) const
{
   M4_MSG_ASSERT(ai_Index<Count && ai_Index>=0,"Index fuera de rango en ClVectorRecordBase::GetItemDataAddressByPos");
   M4_MSG_ASSERT(ai_ItemPos < m_pSMData->NItems(),"Item fuera de rango en ClVectorRecordBase::GetItemDataAddressByPos");

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE) {
      ao_ItemAddress = 0;
      return M4_ERROR;
   }

   return IRM.GetItemDataAddress((m4uint8_t*)Index[ai_Index],ai_ItemPos,ao_ItemAddress);
}

//EDU, prueba:
M4_INLINE m4return_t ClVectorRecordBase::GetItemDataAddressByPos_AndHasSlices(const m4uint16_t ai_ItemPos,
                                                                              const regindex_t ai_Index,
                                                                              p_item_t&        ao_ItemAddress,
                                                                              m4bool_t&        ao_HasSlices) const
{
    M4_MSG_ASSERT(ai_Index<Count && ai_Index>=0, "Index fuera de rango");
    M4_MSG_ASSERT(ai_ItemPos < m_pSMData->NItems(), "Item fuera de rango");
    
    if (IRM.IsValid(Index[ai_Index]) != M4_TRUE) {
		ao_ItemAddress = 0;
		ao_HasSlices = M4_FALSE;
		return M4_ERROR;
	}
    
    ao_HasSlices = IRM.ItemHasSlices((m4uint8_t*)Index[ai_Index], ai_ItemPos);
	
    return IRM.GetItemDataAddress((m4uint8_t*)Index[ai_Index], ai_ItemPos, ao_ItemAddress);
}

M4_INLINE regindex_t ClVectorRecordBase::GetCount(void) const
{
   return Count;
}

M4_INLINE void ClVectorRecordBase::OnModifiedByPos( const regindex_t ai_Index )
{
	if( m_bKeepStatus == M4_TRUE )
	{
		m_pVVectorDelta->OnModifiedByPos( ai_Index ) ;
	}

	SetChange() ;
}

M4_INLINE ClVVectorDelta* ClVectorRecordBase::GetpVVectorDelta( )
{
	if( m_bKeepStatus == M4_TRUE )
	{
		return m_pVVectorDelta;
	}

	return NULL ;
}

M4_INLINE void ClVectorRecordBase::FreeRecordMemory(const regindex_t ai_Index)
{
	// Antes de borrar el registro llamo al destructor de todo los BlobFile
    IRM.DestroyAllItemBlobFile((m4uint8_t*)Index[ai_Index]);
        
    #ifdef _DEBUG
    // Leak check
    // Compruebo si tengo pendiente algun blob
    m4uint16_t iNumItemsAux = m_pSMData->NItems();
    for (m4uint16_t iItemAux = 0; iItemAux < iNumItemsAux; iItemAux++)
    {
        m4uint8_t* DataBuffer = (m4uint8_t*)Index[ai_Index];
        if (IRM.IsItemBlob(DataBuffer, iItemAux))
        {
            m4uint8_t* AuxPtr = DataBuffer + m_pSMData->ItemOffset(iItemAux);
            
            // BlobDataPtr1 points to the actual Blob structure
            BlobDataPtr BlobDataPtr1 = *((BlobDataPtr *)AuxPtr);
           
            M4_ASSERT(BlobDataPtr1 == 0);
        }
    }
    #endif


    m_poMemoryManager->M4free((m4uint8_t *)Index[ai_Index]);
    //free(Index[ai_Index]);
}


M4_INLINE m4return_t ClVectorRecordBase::DeleteRecordLL(const regindex_t ai_Index)
{
   M4_MSG_ASSERT(ai_Index<Count && ai_Index>=0,"Index fuera de rango en ClVectorRecordBase::DeleteRecordLL");

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   // Frees the BLOBS of the data record
   IRM.DeleteBlobsRecord((m4uint8_t*)Index[ai_Index]);

   // Ok, now release the memory used by the "static" record
	
	//free(Index[ai_Index]);
	FreeRecordMemory(ai_Index);

   // Reuse the index
   if (ai_Index<Count-1)
   {
      memmove(&Index[ai_Index],&Index[ai_Index+1],sizeof(ClMemPtr)*(Count-ai_Index-1));
   }

   Count--;

   // by Alex (Deltas) >>>>>>>>>>>>>>
   if (m_bKeepStatus == M4_TRUE)
   {
      m_pVVectorDelta->DeleteByPos(ai_Index);
   }
   // by Alex (Deltas) <<<<<<<<<<<<<<

   SetChange() ;

   // Transaction rows quota control.
   ClRowsQuotaController* pRQC = (ClRowsQuotaController*)ClThreadObjects::Get(ClThreadObjects::ROWS_QUOTA_CONTROLLER);

   if (pRQC != NULL && MAX_COUNT_ALLOW == m_iMaxCount)
   {
	   pRQC->Free(1);
   }

   return M4_SUCCESS;
}

M4_INLINE m4return_t ClVectorRecordBase::SetStatus(const regindex_t       ai_iIndex,
                                                   const RegisterStatus_t ai_NewStatus)
{
    // No puedo cambierle el estado a un registro invalido
    if (IRM.IsValid(Index[ai_iIndex]) != M4_TRUE)
        return M4_ERROR;

    RegisterStatus_t recStat = IRM.GetStatus((m4uint8_t*)Index[ai_iIndex]);
    if (recStat == ai_NewStatus)
        return M4_SUCCESS;

    // Si uso transacciones tengo que dejar consistente el oldRecord
    if (UseCP == M4_TRUE)
    {
        // Si el nuevo estado no es M4_RS_MODIFIED ni M4_RS_MODIFIED_AND_DELETED
        // tengo que encargarme de borrar el oldValue
        if (ai_NewStatus != M4_RS_MODIFIED && ai_NewStatus != M4_RS_MODIFIED_AND_DELETED)
        {
            // Borro el oldValue
            if (DestroyOldValue(ai_iIndex) != M4_SUCCESS)
                return M4_ERROR;
        }
        else
        {
            // Si me quieren poner a modificado tengo que comprobar si tenia oldValue, ya que
            // en caso de no ternerlo tengo que crearlo (todo registro modificado tiene oldValue)
            // Insert and initialize the transaction log index
            if (recStat != M4_RS_MODIFIED && recStat != M4_RS_MODIFIED_AND_DELETED)
            {
                regindex_t iLastIndex;
                if (AddRecordToTransactionLog(ai_iIndex, iLastIndex) != M4_SUCCESS)
                    return M4_ERROR;
            }
        }
    }
    else
    {
        // Hay estados que no estan permitidos si no se usan transacciones
        switch (ai_NewStatus)
        {
        case M4_RS_NORMAL:
        case M4_RS_MODIFIED:
        case M4_RS_NEW:
        case M4_RS_INVALID:
            break;

        default:    // M4_RS_DELETED, M4_RS_MODIFIED_AND_DELETED, M4_RS_NEW_AND_DELETED
            return M4_ERROR;
        }
    }
    
    // En el resto de los casos puedo cambiar el estado sin miedo a perder el oldValue
    IRM.SetStatus((m4uint8_t*)Index[ai_iIndex], ai_NewStatus);

	// Tengo que actualizar el State del VRB para reflejar el estado del registro.
	switch (ai_NewStatus) {
	case M4_RS_NORMAL:
		// No hay que actualizar el estado del VRB.
		//State |= VS_NORMAL;
		break;
		
	case M4_RS_MODIFIED:
		State |= VS_MODIFIED;
		break;
		
	case M4_RS_NEW:
		State |= VS_NEW;
		break;
		
	case M4_RS_INVALID:
		// No hay que actualizar el estado del VRB.
		break;
		
	case M4_RS_DELETED:
		State |= VS_DELETED;
		break;
		
	case M4_RS_MODIFIED_AND_DELETED:
		// Hay que actualizar dos estados.
		State |= VS_MODIFIED;
		State |= VS_DELETED;
		
	case M4_RS_NEW_AND_DELETED:
		// Hay que actualizar dos estados.
		State |= VS_NEW;
		State |= VS_DELETED;
		break;
		
	default:
		return M4_ERROR;
	}

    // by Alex (Deltas) >>>>>>>>>>>>>>
    // ??? Tengo que marcarlo como modificado ya que ha cambiado su estado
    // ??? se podria intentear mandar una marca de normal
    if (m_bKeepStatus == M4_TRUE)
	{
        m_pVVectorDelta->OnModifiedByPos(ai_iIndex);
	}
    // by Alex (Deltas) <<<<<<<<<<<<<<

	SetChange() ;

    return M4_SUCCESS;
}



M4_INLINE m4return_t ClVectorRecordBase::_SetDeleteRecordFlag(const regindex_t ai_Index)
{
    // No podemos activar el flag de borrado si no estamos usando transacciones
    // En caso de no usar transaccinoes hay que borrar el regsitro directamente
    if (UseCP == M4_FALSE)
        return M4_ERROR;

    RegisterStatus_t Status = RecordStatus(ai_Index);
    
    switch (Status)
    {
    case M4_RS_NORMAL :
        IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_DELETED);
        
        // by Alex (Deltas) >>>>>>>>>>>>>>
        // ??? posible optimización, ya que sólo cambia el estado
        // ??? tener un bit para indicar que ha cambiado 
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnDeletedByPos(ai_Index);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
        
        break;
        
    case M4_RS_NEW :
		IRM.SetNewNotPersist((m4uint8_t*)Index[ai_Index], M4_FALSE);
        IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_NEW_AND_DELETED);
        
        // by Alex (Deltas) >>>>>>>>>>>>>>
        // ??? posible optimización, ya que sólo cambia el estado
        // ??? tener un bit para indicar que ha cambiado 
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnDeletedByPos(ai_Index);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
        
        break;
        
    case M4_RS_MODIFIED :
        IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_MODIFIED_AND_DELETED);
        
        // by Alex (Deltas) >>>>>>>>>>>>>>
        // ??? posible optimización, ya que sólo cambia el estado
        // ??? tener un bit para indicar que ha cambiado 
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnDeletedByPos(ai_Index);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
        
        break;
        
    case M4_RS_DELETED:
    case M4_RS_MODIFIED_AND_DELETED:
    case M4_RS_NEW_AND_DELETED:
        // Nothing to do here
        break;

    default:    // M4_RS_INVALID
        return M4_ERROR;
    }
    
    return M4_SUCCESS;
}


M4_INLINE m4return_t ClVectorRecordBase::DeleteRecord(const regindex_t ai_Index)
{
   State|=VS_DELETED; // ESTADO 

   if (!UseCP)
   {
      return DeleteRecordLL(ai_Index);
   }
   else
   {
      return _SetDeleteRecordFlag(ai_Index);
   }
}


M4_INLINE m4return_t ClVectorRecordBase::DestroyRecordLL(const regindex_t ai_Index)
{
    // No actualizo el estado
    if (!UseCP)
	{
        return DeleteRecordLL(ai_Index);
	}
    else
    {
        m4uint8_t AuxState=State;
        
        DeleteRecord(ai_Index); // No verify of return type is a must
        
        m4bool_t RecordIsDeleted;
        
        m4return_t Res=CommitRecord(ai_Index,RecordIsDeleted);
        
        State=AuxState;
        
        return Res;
    }
}


M4_INLINE m4bool_t ClVectorRecordBase::IndexFull(const regindex_t ai_Number) const
{
	return ( (Count+ai_Number>LogIndex) ? M4_TRUE : M4_FALSE ); // Deberia hacerse uno mas tarde CRG, no problema
}

// Fix bug 0178686
// compute new index dynamically
M4_INLINE regindex_t ClVectorRecordBase::ComputeDynamicGrow(const regindex_t ai_Grow)
{
	// checks normal case
	// just in case you wondering about what 304 means, 
	// it is the first multiple of 8 greater than 300, which is the normal
	// limit for a block load
	if (Count <= 304)
	{
		return ai_Grow;
	}

	// If you have more than 300, why not 1000 more? This will save lots of rellocations
	if( ai_Grow < 1000 )
	{
		return 1000;
	}

	return ai_Grow;
}

M4_INLINE m4return_t ClVectorRecordBase::GrowIndex(const regindex_t ai_Grow)
{
    ClMemPtr *NewIndex;
	ClMemPtr *OldIndex = Index;

	regindex_t iGrow = ComputeDynamicGrow(ai_Grow);
    
    if ((NewIndex=(ClMemPtr *)m_poMemoryManager->M4malloc(sizeof(ClMemPtr)*(Size+iGrow)))!=0)
    {
        if (Index != NULL)
        {
            // Copy of the previous indexes
            memcpy(NewIndex,Index,sizeof(ClMemPtr)*(Count));
            
            // Copy of the transaction log. In case we are noty using it
            // no problem: LogIndex equals Size-1
            if (LogIndex<Size-1)
			{
                memcpy(&NewIndex[LogIndex+iGrow+1], &Index[LogIndex+1], sizeof(ClMemPtr)*(Size-LogIndex-1));
			}

        }
        
		Index     = NewIndex;
        LogIndex += iGrow;
        Size     += iGrow;
		
		// bg 0187757
		if(OldIndex != NULL)
		{
			m_poMemoryManager->M4free((m4uint8_t *)OldIndex);
		}
    }
    else
	{
        return M4_ERROR;
	}
    
    return M4_SUCCESS;
}


// Guarantees a minimun size 
M4_INLINE m4return_t ClVectorRecordBase::Reserve(const regindex_t ai_iSize)
{
	if( ai_iSize > Size )
	{
		return GrowIndex( ai_iSize - Size );
	}
    return M4_SUCCESS;
}


// Inserts a number of records, beggining in the index specified
M4_INLINE m4return_t ClVectorRecordBase::InsertRecordsLL(const regindex_t ai_Index,
                                                         const regid_t    ai_iRecordID,
                                                         const regindex_t ai_Number,
                                                         const m4bool_t   ai_iInit)
{
    M4_ASSERT(ai_Index <= Count && ai_Index >= 0);
    M4_ASSERT(ai_Number >0);
    
    if (Count + ai_Number > m_iMaxCount)
	{
        return M4_VRB_ERROR_MAX_COUNT;
	}

	// Transaction rows quota control.
	ClRowsQuotaController* pRQC = (ClRowsQuotaController*)ClThreadObjects::Get(ClThreadObjects::ROWS_QUOTA_CONTROLLER);

	if (pRQC != NULL && MAX_COUNT_ALLOW == m_iMaxCount)
	{
		m4bool_t	bAllowed = pRQC->TryAlloc(ai_Number);

		if (bAllowed == M4_FALSE)
		{
			// Avoid locking post error tech operations.
			pRQC->SwitchOff();

			return(M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED);
		}
	}


    State |= VS_NEW; // CUIDADO CRG
    
    if (IndexFull(ai_Number))
    {
        if (GrowIndex(ai_Number+GrowRecords)!=M4_SUCCESS)
            return M4_ERROR;
    }
    
    // Move the neccesary indexes
    if (ai_Index < Count)
	{
        memmove(&Index[ai_Number+ai_Index], &Index[ai_Index], sizeof(ClMemPtr) * (Count - ai_Index));
	}
    
    // Cycle to allocate the memory for the static part of the records
    for (regindex_t i = ai_Index; i < ai_Index + ai_Number; i++)
    {
        Index[i] = m_poMemoryManager->M4calloc(m_pSMData->RecordSize());

		if (ai_iInit)
		{
			// Initialize the new record
			IRM.InitRecord((m4uint8_t*)Index[i]);
			
			IRM.SetNewNotPersist((m4uint8_t*)Index[ai_Index], M4_FALSE);	// Por defecto es persistible
			IRM.SetStatus((m4uint8_t*)Index[i], M4_RS_NEW);					// New, allways with state
			
			IRM.SetRecordID((m4uint8_t*)Index[i], ai_iRecordID + i - ai_Index);
			
			// by Alex (Deltas) >>>>>>>>>>>>>>
			// No hace falta hacer tratamientos de deltas, ya que como registro
			// nuevo se va a marcar automáticamente al insertarlo
			// by Alex (Deltas) <<<<<<<<<<<<<<
		}
		
		// Llamar a los constructores de los BlobFile
		IRM.CreateAllItemBlobFile((m4uint8_t*)Index[i], this);
    }
        
    Count += ai_Number;
    
    // by Alex (Deltas) >>>>>>>>>>>>>>
    if (m_bKeepStatus == M4_TRUE)
    {
        if (m_pVVectorDelta->InsertByPos(ai_Index, ai_Number) != M4_SUCCESS)
            return M4_ERROR;
    }
    // by Alex (Deltas) <<<<<<<<<<<<<<
    
	SetChange() ;

    return M4_SUCCESS;
}

M4_INLINE m4return_t ClVectorRecordBase::FreeMemoryFromActualAndLast(regindex_t     ai_Index,
                                                                     m4bool_t       ai_deleteStaticPart /*= M4_TRUE*/,
                                                                     const m4bool_t ai_bOnlyMappedItems /*= M4_FALSE*/)
{
    BlobDataPtr BlobPtrLast;
    BlobDataPtr BlobPtrActual;
    
    if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
        return M4_ERROR;
    
    regindex_t LastIndex = GetLastRecordValueIndex(ai_Index);
    
    if (IRM.IsValid(Index[LastIndex]) != M4_TRUE)
        return M4_ERROR;
    
    // Cycle freeing the blobs of the actual and last
    m4uint16_t iItem;
    m4uint16_t iNItems;
    if (ai_bOnlyMappedItems == M4_TRUE)
	{
        iNItems = m_pSMData->NMappedItems();
	}
    else
	{
        iNItems = m_pSMData->NItems();
	}
    
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

		/* Bug 0098967
		Hay que poner el blob a nulo para que se quite el interes sobre el fichero
		porque si lo que vienen es nulo nunca se quita el interes y luego no se puede
		acceder a su valor porque se marca como nulo
		Hay que hacerlo tanto para el old value como el normal
		*/
        if( m_pSMData->ItemLivesInFile( iItem ) == M4_TRUE )
		{
			ClDsItemBlobFile	*pBlobFile = NULL ;

            pBlobFile = (ClDsItemBlobFile*)( ( (m4uint8_t*)Index[ LastIndex ] ) + m_pSMData->ItemOffset( iItem ) ) ;

			if( pBlobFile->GetNull() == M4_FALSE )
			{
				pBlobFile->SetNull( M4_TRUE ) ;
			}

            pBlobFile = (ClDsItemBlobFile*)( ( (m4uint8_t*)Index[ ai_Index ] ) + m_pSMData->ItemOffset( iItem ) ) ;

			if( pBlobFile->GetNull() == M4_FALSE )
			{
				pBlobFile->SetNull( M4_TRUE ) ;
			}

            continue;
		}

        IRM.GetItemBlobAddress((m4uint8_t*)Index[ai_Index], iItem, BlobPtrActual);
        IRM.GetItemBlobAddress((m4uint8_t*)Index[LastIndex], iItem, BlobPtrLast);
        
        m4uint32_t iItemOffset = m_pSMData->ItemOffset(iItem);
        
        if (BlobPtrLast != NULL)
        {
            //free(BlobPtrLast);
            IRM.SetItemBlobNullRelease((m4uint8_t*)Index[LastIndex], iItem, M4_TRUE);
            
            if (BlobPtrLast != BlobPtrActual)
            {
                if (BlobPtrActual != 0)
                {
                    //free(BlobPtrActual);
                    IRM.SetItemBlobNullRelease((m4uint8_t*)Index[ai_Index], iItem, M4_TRUE);
                }
            }
            else
			{
                *((void**)(((m4uint8_t*)Index[ai_Index]) + iItemOffset)) = 0;
			}
        }
        else
        {
            if (BlobPtrActual != NULL)
            {
                //free(BlobPtrActual);
                IRM.SetItemBlobNullRelease((m4uint8_t*)Index[ai_Index], iItem, M4_TRUE);
            }
        }
    }
    
    // Frees the static part of records
    if (ai_deleteStaticPart)
    {
        //free(Index[ai_Index]);
		FreeRecordMemory(ai_Index);
        //free(Index[LastIndex]);
		FreeRecordMemory(LastIndex);
    }
    
    return M4_SUCCESS;
}

M4_INLINE m4bool_t ClVectorRecordBase::IsRecordValid(const regindex_t ai_Index) const
{
    M4_MSG_ASSERT(ai_Index<Count && ai_Index>=0,"Index fuera de rango en ClVectorRecordBase::IsRecordValid");

    return IRM.IsValid(Index[ai_Index]);
}


M4_INLINE void ClVectorRecordBase::ZapLL(void)
{
   // Release the used memory ...
   for (regindex_t i=0; i<Count; i++)
   {
      // by Alex (Deltas) >>>>>>>>>>>>>>
      // Como ZapLL se llama sólo desde el destructor, para liberar toda la memoria
      // No hace falta que llame m_vectorDeltas->DeleteByPos(i), al final del
      // destructor elimino m_vectorDeltas
      // by Alex (Deltas) <<<<<<<<<<<<<<

      if (IsRecordValid(i) == M4_TRUE)
      {
         if (ExistsOldValue(i) == M4_TRUE)
         {
            // In this case we must take into account that there should be
            // the last version in the transaction log !!!
            FreeMemoryFromActualAndLast(i, M4_TRUE);
         }
         else
         {
            IRM.DeleteBlobsRecord((m4uint8_t*)Index[i]);

            // Ok, now release the memory used by the "static" record
            //free(Index[i]);
			FreeRecordMemory(i);
         }
      }
      else
      {
         //free(Index[i]);
		 FreeRecordMemory(i);
      }
   }

   // Transaction rows quota control.
   ClRowsQuotaController* pRQC = (ClRowsQuotaController*)ClThreadObjects::Get(ClThreadObjects::ROWS_QUOTA_CONTROLLER);

   if (pRQC != NULL && MAX_COUNT_ALLOW == m_iMaxCount)
   {
	   pRQC->Free(Count);
   }

   LogIndex=Size-1;
   LogCount=0;
   Count=0;
}


M4_INLINE void ClVectorRecordBase::SetLastRecordValueIndex(const regindex_t ai_Index1,
                                                           const regindex_t ai_Index)
{
   // ai_Index is an absolute address. We need to store internaly as
   // a negative offset from the end of the index, so we need to calculate it
   regindex_t Index1 = Size - ai_Index;
   IRM.SetUserDataByOffset((m4uint8_t*)Index[ai_Index1],&Index1, 0, sizeof(Index1));
}

M4_INLINE m4return_t ClVectorRecordBase::SameBlobByIndex(const m4uint16_t ai_ItemPos,
                                                         const regindex_t ai_Index1,
                                                         const regindex_t ai_Index2,
                                                         m4bool_t&        IsTheSame) const
{
   p_item_t Ptr1,Ptr2;
   if (IRM.IsValid(Index[ai_Index1]) != M4_TRUE)
      return M4_ERROR;
   IRM.GetItemDataAddress((m4uint8_t*)Index[ai_Index1],ai_ItemPos,Ptr1);

   if (IRM.IsValid(Index[ai_Index2]) != M4_TRUE)
      return M4_ERROR;
   IRM.GetItemDataAddress((m4uint8_t*)Index[ai_Index2],ai_ItemPos,Ptr2);

   IsTheSame = ( (Ptr1==Ptr2) ? M4_TRUE : M4_FALSE );

   return M4_SUCCESS;
}


M4_INLINE m4return_t ClVectorRecordBase::BlobChanged(const m4uint16_t ai_ItemPos,
                                                     const regindex_t ai_Index,
                                                     m4bool_t&        IsTheSame) const
{
   regindex_t LastValueIndex = GetLastRecordValueIndexByIndex(ai_Index);

   return SameBlobByIndex(ai_ItemPos, LastValueIndex, ai_Index, IsTheSame);
}


M4_INLINE m4return_t ClVectorRecordBase::UndoLL(const regindex_t ai_Index,
                                                m4bool_t&        ao_RecordWasDeleted)
{
    regindex_t  LastIndex;
    BlobDataPtr BlobPtrLast;
    BlobDataPtr BlobPtrActual;
    m4uint16_t  Item;
    m4uint16_t  NumItems;
    
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);
    if (UseCP)
    {
        RegisterStatus_t Status = RecordStatus(ai_Index);
        
        switch (Status)
        {
        case M4_RS_NORMAL:
        case M4_RS_INVALID:
            // Nothing to do here
            ao_RecordWasDeleted = M4_FALSE;
            break;
            
        case M4_RS_DELETED:
            IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_NORMAL);
            
            // by Alex (Deltas) >>>>>>>>>>>>>>
            if (m_bKeepStatus == M4_TRUE)
			{
                m_pVVectorDelta->OffDeletedByPos(ai_Index);
            
				// ??? Tengo que marcarlo como modificado ya que ha cambiado su estado
				// ??? se podria intentear mandar una marca de normal
                m_pVVectorDelta->OnModifiedByPos(ai_Index);
			}
            // by Alex (Deltas) <<<<<<<<<<<<<<

			SetChange() ;

            ao_RecordWasDeleted = M4_FALSE;
            break;
            
        case M4_RS_MODIFIED:
        case M4_RS_MODIFIED_AND_DELETED:
            // Undo of modifications
            // Cycle freeing the blobs of the actual record wich are
            // different from the last value
            LastIndex = GetLastRecordValueIndex(ai_Index);
            if (IRM.IsValid(Index[LastIndex]) != M4_TRUE)
                return M4_ERROR;
            
            NumItems = m_pSMData->NItems();
            for (Item = 0; Item < NumItems; Item++)
            {
                IRM.GetItemBlobAddress((m4uint8_t*)Index[ai_Index], Item, BlobPtrActual);
                IRM.GetItemBlobAddress((m4uint8_t*)Index[LastIndex], Item, BlobPtrLast);
                
                if (BlobPtrActual != 0)
                {
                    if (BlobPtrLast != BlobPtrActual)
                    {
                        //free(BlobPtrActual);
                        IRM.SetItemBlobNullRelease((m4uint8_t*)Index[ai_Index], Item, M4_TRUE);
                    }
                    else
                    {
                        #ifdef _DEBUG
                        // Leak check
                        // Para dejar consistente la comprobacion de los blobs
                        IRM.SetItemBlobNullNoRelease((m4uint8_t*)Index[ai_Index], Item, M4_TRUE);
                        #endif
                    }
                }
            }
            
            // Frees the static part of the actual record
            //free(Index[ai_Index]);
            FreeRecordMemory(ai_Index);
            
            // The actual record becomes the last record
            Index[ai_Index] = Index[LastIndex];
            
            // Alex blob
            // Si tengo algun ItemBlob tengo que decirle que ya no esta en un oldValue
            if (m_pSMData->SomeItemLivesInFile() == M4_TRUE)
            {
                for (Item = 0; Item < m_pSMData->NItems(); Item++)
                {
                    if (m_pSMData->ItemLivesInFile(Item) == M4_TRUE)
                    {
                        m4uint32_t iItemOffset = m_pSMData->ItemOffset(Item);
                        ClDsItemBlobFile* pDsItemBlobFile = (ClDsItemBlobFile*)(((m4uint8_t*)Index[ai_Index]) + iItemOffset);
                        
                        // Como el ItemBlobFile ya no esta en un oldValue le mando un evento para que se entere
                        // y vuelva a prestar atencion a los eventos del fichero
                        m4bool_t bItemIsOldValue = M4_FALSE;
                        ClEventDescription oEventDescription(EVENT_TYPE_RM, (void*)&bItemIsOldValue);
                        pDsItemBlobFile->EventHandler(oEventDescription);
                    }
                }
            }
            
            // Update the transaction log (delete the entry)
            DeleteFromTransactionLog(LastIndex);
            
            // Set normal state
            IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_NORMAL);
            
            // by Alex (Deltas) >>>>>>>>>>>>>>
            // ??? Existe un problema: no puedo detectar si el registro vino normal o ya modificado
            // Si vino normal puedo quitar la marca de modificado porque no hace falta enviarlo
            // Si vino modificado tengo que mantener la marca de modificado porque se restauro el registro de log
            // Esto es antes de cualquier comit, ya que despues siempre depende de la marca de modificado
            // Para arreglarlo puedo añadir un bit más en el vectorDelta que indique si el registro vino Normal,
            //    Si normal entonces quitar la marca de modificado (no se tiene que enviar)
            //    Sino mantener la marca ya que se ha modificado realmente
            // Cuando se haga el primer comit hay que quitarle la marca de normal para que si se hace un undo
            // despues del comit se haga según la marca de modificado
            // Por ahora no se va a implemetnar, luego después de un undo los modificados siguen con la marca
            // de modificados, luego siempre se mandan
            
            // ??? Tengo que marcarlo como modificado ya que ha cambiado su estado
            // ??? se podria intentear mandar una marca de normal
            if (m_bKeepStatus == M4_TRUE)
			{
                m_pVVectorDelta->OnModifiedByPos(ai_Index);
			}
            // by Alex (Deltas) <<<<<<<<<<<<<<

			SetChange() ;

            ao_RecordWasDeleted = M4_FALSE;
            break;
            
        case M4_RS_NEW:
        case M4_RS_NEW_AND_DELETED:
            ao_RecordWasDeleted = M4_TRUE;  // Este es el unico caso en el que se borra el registro
            return DeleteRecordLL(ai_Index);
            break;
            
        default:
            ao_RecordWasDeleted = M4_FALSE;
            return M4_ERROR;
      }
   }
   
   return M4_SUCCESS;
}

// CODIGO MEJORAO !!! CRG

M4_INLINE m4bool_t ClVectorRecordBase::IsRecordDeleted(const regindex_t ai_Index) const
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);

    RegisterStatus_t Status = RecordStatus(ai_Index);
    
    if (Status == M4_RS_DELETED || Status == M4_RS_MODIFIED_AND_DELETED || Status == M4_RS_NEW_AND_DELETED) {
        return M4_TRUE;
    }

    return M4_FALSE;
}

M4_INLINE m4bool_t ClVectorRecordBase::IsRecordModified(const regindex_t ai_Index) const
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);

    RegisterStatus_t Status = RecordStatus(ai_Index);
    
    if (Status == M4_RS_MODIFIED) return M4_TRUE;

    return M4_FALSE;
}

M4_INLINE m4bool_t ClVectorRecordBase::IsRecordModifiedInDB(const regindex_t ai_Index) const
{
   M4_MSG_ASSERT(ai_Index<Count && ai_Index>=0,"Index fuera de rango en ClVectorRecordBase::IsRecordModifiedInDB");

   return IRM.GetModifiedInDB((m4uint8_t*)Index[ai_Index]);
}

M4_INLINE m4bool_t ClVectorRecordBase::IsRecordNew(const regindex_t ai_Index) const
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);

    RegisterStatus_t Status = RecordStatus(ai_Index);
    
    if (Status == M4_RS_NEW)
	{
		return M4_TRUE;
	}

    return M4_FALSE;
}

M4_INLINE m4bool_t ClVectorRecordBase::GetRecordPersistLDB(const regindex_t ai_Index) const
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);

   return IRM.GetPersistLDB((m4uint8_t*)Index[ai_Index]);
}

M4_INLINE m4return_t ClVectorRecordBase::SetRecordPersistLDB(const regindex_t ai_Index, const m4bool_t ai_IsPersisted)
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
   {
	   return M4_ERROR;
   }

   IRM.SetPersistLDB((m4uint8_t*)Index[ai_Index], ai_IsPersisted);

   return M4_SUCCESS;
}

M4_INLINE m4bool_t ClVectorRecordBase::UsingCheckPoints(void) const
{
   return UseCP;
}

M4_INLINE void ClVectorRecordBase::SetUseCPDelta(const m4bool_t ai_bUseCPDelta)
{
    m_bUseCPDelta = ai_bUseCPDelta;
}

M4_INLINE m4return_t ClVectorRecordBase::Status(m4uint8_t& ao_Status) const
{
   ao_Status = State;

   return M4_SUCCESS; 
}

M4_INLINE m4bool_t ClVectorRecordBase::IsModified(void) const
{
	return (State & VS_MODIFIED) ? M4_TRUE : M4_FALSE;
}

M4_INLINE m4bool_t ClVectorRecordBase::IsModifiedInDB(void) const
{
	return (State & VS_MODIFIED_IN_DB) ? M4_TRUE : M4_FALSE;
}

M4_INLINE m4bool_t ClVectorRecordBase::IsNew(void) const
{
	return (State & VS_NEW) ? M4_TRUE : M4_FALSE;
}

M4_INLINE m4bool_t ClVectorRecordBase::IsDeleted(void) const
{
	return (State & VS_DELETED) ? M4_TRUE : M4_FALSE;
}

M4_INLINE m4bool_t ClVectorRecordBase::IsDeSerializingDeltas(void) const
{
   return m_bDeSerializingDeltas;
}

M4_INLINE regindex_t ClVectorRecordBase::GetCountDelta(void) const
{
   M4_ASSERT(IsDeSerializingDeltas() == M4_TRUE);

   return m_iCountDelta;
}

M4_INLINE void ClVectorRecordBase::ResetStatus(void)
{
	if (State != VS_NORMAL) {
		for (regindex_t i = 0; i < Count; i++) {
			SetStatus(i, M4_RS_NORMAL);
		}
		State = VS_NORMAL;
	}
}

M4_INLINE m4return_t ClVectorRecordBase::UseCheckPoints(m4bool_t ai_Use)
{
	m4return_t iRet = M4_SUCCESS;

	if (UseCP && !ai_Use) {
		// Change from transaction to not transaction mode
		iRet = Commit();
	} else {
		if (!UseCP && ai_Use) {
			// From not transaction to transaction mode: reset the status...
			ResetStatus();
		}
	}
	
	UseCP = ai_Use;
	
	return iRet;
}

M4_INLINE m4return_t ClVectorRecordBase::GetLastItemDataAddressByPos(const m4uint16_t ai_ItemPos,
                                                                     const regindex_t ai_Index,
                                                                     p_item_t         &ao_ItemAddress) const
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());
    
    m4uint8_t* ActiveRecord = _LastRecordToProcess(ai_Index);
    if (!ActiveRecord)
	{
		return M4_ERROR;
	}
    
    return IRM.GetItemDataAddress(ActiveRecord, ai_ItemPos, ao_ItemAddress);
}

//EDU, prueba:
M4_INLINE m4return_t ClVectorRecordBase::GetLastItemDataAddressByPos_AndHasSlices(const m4uint16_t ai_ItemPos,
                                                                                  const regindex_t ai_Index,
                                                                                  p_item_t&        ao_ItemAddress,
                                                                                  m4bool_t&        ao_HasSlices) const
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());
    
    m4uint8_t* ActiveRecord = _LastRecordToProcess(ai_Index);
    if (!ActiveRecord)
	{
		return M4_ERROR;
	}
    
    ao_HasSlices = IRM.ItemHasSlices(ActiveRecord, ai_ItemPos);
    
    return IRM.GetItemDataAddress(ActiveRecord, ai_ItemPos, ao_ItemAddress);
}

M4_INLINE m4return_t ClVectorRecordBase::SetNeedSerial(const regindex_t ai_Index,
                                                       const m4bool_t   ai_bNeedSerial,
                                                       const m4bool_t   ai_bBranchMode)
{
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < Count, "Index fuera de rango");

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
   {
	   return M4_ERROR;
   }

   IRM.SetNeedSerial((m4uint8_t*)Index[ai_Index],ai_bNeedSerial, ai_bBranchMode);

   return M4_SUCCESS;
}


M4_INLINE void ClVectorRecordBase::ResetSerialMarks(void)
{
    // Si tengo informacion de deltas para serializacion avanzada, la borro
    delete m_pVAdvancedSerial;
    m_pVAdvancedSerial = NULL;
    
   for (regindex_t i = 0; i < Count; i++)
   {
      SetNeedSerial(i, M4_FALSE, M4_FALSE);
   }
}

M4_INLINE void ClVectorRecordBase::FindRecordAndSetVersion(const m4uint32_t  ai_iRecordId,
                                                           const VersionMark ai_iNewVersion,
                                                           m4bool_t&         ao_bContinue)
{
    ao_bContinue = M4_TRUE;

    for (regindex_t i = 0; i < Count && ao_bContinue == M4_TRUE; i++)
    {
        m4uint8_t* pBuffer = (m4uint8_t*)Index[i];

        if (ai_iRecordId == IRM.GetRecordID(pBuffer))
        {
            IRM.SetRecordVersion(pBuffer, ai_iNewVersion);
            ao_bContinue = M4_FALSE;
        }
    }
}

M4_INLINE m4return_t ClVectorRecordBase::GetNeedSerial(const regindex_t ai_Index,
                                                       m4bool_t&        ao_bNeedSerial,
                                                       m4bool_t&        ao_bBranchMode) const
{
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < Count, "Index fuera de rango");

   IRM.GetNeedSerial((m4uint8_t*)Index[ai_Index],ao_bNeedSerial, ao_bBranchMode);

   return M4_SUCCESS;
}


M4_INLINE m4return_t ClVectorRecordBase::TestAndSetNeedSerial(const regindex_t ai_Index,
                                                              m4bool_t&        ao_bBeforeNeedSerial,
                                                              m4bool_t&        ao_bBeforeBranchMode,
                                                              const m4bool_t   ai_bNeedSerial,
                                                              const m4bool_t   ai_bBranchMode)
{
   M4_MSG_ASSERT(ai_Index >= 0 && ai_Index < Count, "Index fuera de rango");

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
   {
	   return M4_ERROR;
   }

   IRM.TestAndSetNeedSerial((m4uint8_t*)Index[ai_Index], ao_bBeforeNeedSerial, ao_bBeforeBranchMode, ai_bNeedSerial, ai_bBranchMode);

   return M4_SUCCESS;
}

M4_INLINE const EMDStaticData* const ClVectorRecordBase::GetpExtendedMetaData(void)
{
	return m_pSMData;
}

M4_INLINE regid_t ClVectorRecordBase::GetRecordID(const regindex_t ai_iIndex) const
{
    return IRM.GetRecordID((m4uint8_t*)Index[ai_iIndex]);
}

M4_INLINE void ClVectorRecordBase::SetRecordID(const regindex_t ai_iIndex, const regid_t ai_iRecordID)
{
    IRM.SetRecordID((m4uint8_t*)Index[ai_iIndex], ai_iRecordID);
}

M4_INLINE m4bool_t ClVectorRecordBase::GetRecordNewNotPersist(const regindex_t ai_iIndex) const
{
	return IRM.GetNewNotPersist((m4uint8_t*)Index[ai_iIndex]);
}

M4_INLINE void ClVectorRecordBase::SetRecordNewNotPersist(const regindex_t ai_iIndex, const m4bool_t ai_bNewNotPersist)
{
	IRM.SetNewNotPersist((m4uint8_t*)Index[ai_iIndex], ai_bNewNotPersist);
}


#endif
