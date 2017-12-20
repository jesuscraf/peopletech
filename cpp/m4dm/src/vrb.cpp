//==============================================================================
//
// (c) Copyright  1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           Data Block
// File:             vrb.cpp
// Project:          Canales
// Author:           Meta Software M.S., S.A
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
// 
//==============================================================================



// HACER QUE LOS BLOBS TAMBIEN ESTEN EN POSICIONES DE MEMORIA VALIDAS (x8)

#include "vrb.hpp"
#include "vrb.inl"
#include "dsres.hpp"
#include "chlog.hpp"
#include "clgeniod.hpp"
#include "vdelta.hpp"
#include "channelprocess.hpp"
#include "dsitem.hpp"
#include "sizer.hpp"
#include "memman.hpp"
#include "channel.hpp"


const regindex_t ClVectorRecordBase::MAX_COUNT_ALLOW = 2147483647;   // La maxima capacidad de un long



ClVectorRecordBase::ClVectorRecordBase(void)
	: IRM(0)
	, m_bKeepStatus(M4_FALSE)
	, m_pVVectorDelta(NULL)
	, m_bDeSerializingDeltas(M4_FALSE)
	, m_pVAdvancedSerial(NULL)
	, m_bUseCPDelta(M4_FALSE)
	, m_StateDelta(0)
	, Count(0)
	, m_iMaxCount(0)
	, Size(0)
	, GrowRecords(0)
	, LogCount(0)
	, Index(0)
	, m_poMemoryManager(0)
	, UseCP(M4_TRUE)
	, State(VS_NORMAL)
{
}

m4return_t ClVectorRecordBase::Init(const EMDStaticData* const ai_pSMData,
                                    const regindex_t          ai_iInitialRecords,
                                    const regindex_t          ai_iGrowRecords,
                                    const regindex_t          ai_iInitialLogRecords,
                                    const regindex_t          ai_iMaxCount)
{
	if (Index) return M4_ERROR;
	
	m_pSMData = ai_pSMData;
	
	// Initializing record manager.
	IRM.SetMetaData(m_pSMData);
	
	m_iMaxCount = ai_iMaxCount;
	Size  = ai_iInitialRecords + ai_iInitialLogRecords;
	LogIndex = Size - 1;
	GrowRecords = ai_iGrowRecords;
	
	Index = (ClMemPtr*)m_poMemoryManager->M4malloc(sizeof(ClMemPtr) * Size);
	if (!Index) {
		Size     = 0;
		LogIndex = 0;
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}


ClVectorRecordBase::~ClVectorRecordBase()
{
	if (Index) {
		ZapLL();
		
		m_poMemoryManager->M4free((m4uint8_t *)Index);
		
		// by Alex (Deltas) >>>>>>>>>>>>
		delete m_pVVectorDelta;
		// by Alex (Deltas) <<<<<<<<<<<<
	}
}

m4return_t ClVectorRecordBase::DeleteFromTransactionLog(regindex_t ao_TransactionIndex)
{
    if (ao_TransactionIndex==LogIndex+1)
        LogIndex++;
    else
        Index[ao_TransactionIndex]=0;
    
    LogCount--;
    
    return M4_SUCCESS;
}


m4return_t ClVectorRecordBase::AddRecordToTransactionLogWithoutCopy(regindex_t& ao_TLIndex)
{
    if (Count+1 > Size-LogCount)
    {
        // There is no more room for the new record
        // We can grow the size of the index ...
        if (GrowIndex(GrowRecords) != M4_SUCCESS)
            return M4_ERROR;
    }

    // Now, we have the space for the new transaction log record
    for (ao_TLIndex = Size-1; ao_TLIndex > LogIndex; ao_TLIndex--)
    {
        if (Index[ao_TLIndex]==0)
		{
            break;
		}
    }

    // Now ao_TLIndex is in the position of the new record
    Index[ao_TLIndex] = m_poMemoryManager->M4calloc(m_pSMData->RecordSize());
/*    if ((Index[ao_TLIndex] = calloc(1, m_pSMData->RecordSize())) == NULL)
        return M4_ERROR;
*/
    // Alex blob
    // Necesito llamar al constructor por defecto para los BlobFile
    if (m_pSMData->SomeItemLivesInFile() == M4_TRUE)
    {
        for (m4uint16_t iItem = 0; iItem < m_pSMData->NItems(); iItem++)
        {
            if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE)
            {
                m4uint32_t iItemOffset = m_pSMData->ItemOffset(iItem);
                new(((m4uint8_t*)Index[ao_TLIndex]) + iItemOffset) ClDsItemBlobFile(this, &IRM, (m4uint8_t*)Index[ao_TLIndex], iItem);
            }
        }
    }

    LogCount++;

    if (ao_TLIndex == LogIndex)
	{
        LogIndex--;
	}

    return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::AddRecordToTransactionLog(const regindex_t ai_Index,
                                                         regindex_t&      ao_TLIndex)
{
    if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
        return M4_ERROR;

    if (AddRecordToTransactionLogWithoutCopy(ao_TLIndex) != M4_SUCCESS)
        return M4_ERROR;

    // Set the pointer to the last value
    SetLastRecordValueIndex(ai_Index, ao_TLIndex);

    CopyRecordToOldValue(ai_Index, ao_TLIndex);

    IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_MODIFIED);
    
    return M4_SUCCESS;
}

void ClVectorRecordBase::CopyRecordToOldValue(const regindex_t ai_iIndex,
                                              const regindex_t ai_iLastIndex,
                                              const m4bool_t   ai_bCopyMappedBlobFileItems /* = M4_TRUE */)
{
    memcpy(Index[ai_iLastIndex], Index[ai_iIndex], m_pSMData->RecordSize());

    // Alex blob
    // Necesito llamar al constructor de copia para los BlobFile
    if (m_pSMData->SomeItemLivesInFile() == M4_TRUE)
    {
        m4uint16_t iNMappedItems = m_pSMData->NMappedItems();
        
        for (m4uint16_t iItem = 0; iItem < m_pSMData->NItems(); iItem++)
        {
            if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE)
            {
                m4uint32_t iItemOffset = m_pSMData->ItemOffset(iItem);
                m4bool_t bIsMapped = M4_FALSE;

                if (ai_bCopyMappedBlobFileItems == M4_FALSE)
                {
                    // No tengo que copiar los itemBlobFile que son mapped
                    // Esto solo se va ha hacer desde la deserializacion
                    // Arregla el error de interesarnos dos veces (una vez en la copia y otra vez
                    // en la deserializacion) por un fichero que esta en un old value (esto provoca
                    // que al destruir el canal no se borre el fichero, ya que se mantiene un interes)
                    for (m4uint16_t i = 0; i < iNMappedItems; i++)
                    {
                        if (m_pSMData->GetIndexOfMappedItem(i) == iItem)
                        {
                            bIsMapped = M4_TRUE;
                            break;
                        }
                    }
                }

                ClDsItemBlobFile* pDsItemBlobFile;

                if (ai_bCopyMappedBlobFileItems == M4_FALSE && bIsMapped == M4_TRUE)
                {
                    // Como no voy a llamar al constructor de copia necesito llamar al otro
                    // constructor, ya que la memoria se ha copiado (tengo los mismos valores que
                    // el otro registro)
                    pDsItemBlobFile = new(((m4uint8_t*)Index[ai_iLastIndex]) + iItemOffset) ClDsItemBlobFile(this, &IRM, (m4uint8_t*)Index[ai_iLastIndex], iItem);
                }
                else
                {
                    pDsItemBlobFile = new(((m4uint8_t*)Index[ai_iLastIndex]) + iItemOffset) ClDsItemBlobFile(*((ClDsItemBlobFile*)(((m4uint8_t*)Index[ai_iIndex]) + iItemOffset)), (m4uint8_t*)Index[ai_iLastIndex]);
                }
                
                // Como el ItemBlobFile esta en un oldValue le mando un evento para que se entere
                // y deje de prestar atencion a los eventos del fichero
                m4bool_t bItemIsOldValue = M4_TRUE;
                ClEventDescription oEventDescription(EVENT_TYPE_RM, (void*)&bItemIsOldValue);
                pDsItemBlobFile->EventHandler(oEventDescription);
            }
        }
    }
}

//EDU-------------->>
m4return_t ClVectorRecordBase::ItemHasSlices(const m4uint16_t ai_ItemPos,
                                             const regindex_t ai_Index,
                                             m4bool_t  &ao_HasSlices) const
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);
   M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE) {
	  ao_HasSlices = M4_FALSE;
      return M4_ERROR;
   }

   ao_HasSlices = IRM.ItemHasSlices((m4uint8_t*)Index[ai_Index],ai_ItemPos);
 
   return M4_SUCCESS;
}
//EDU--------------<<

// ??? revisar inforamción de deltas
m4return_t ClVectorRecordBase::LastItemPriority(const m4uint16_t ai_ItemPos,
                                                const regindex_t ai_Index,
                                                m4char_t&        ao_iPriority) const
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());
    
    m4uint8_t* ActiveRecord = _LastRecordToProcess(ai_Index);
    if (ActiveRecord == NULL)
        return M4_ERROR;
    
    ao_iPriority = IRM.GetItemPriority(ActiveRecord, ai_ItemPos);
    
    return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::LastItemHasSlices(const m4uint16_t ai_ItemPos,
                                                 const regindex_t ai_Index,
                                                 m4bool_t  &ao_HasSlices) const
{
	ao_HasSlices = M4_FALSE;

    M4_ASSERT(ai_Index < Count && ai_Index >= 0);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());
    
    m4uint8_t* ActiveRecord = _LastRecordToProcess(ai_Index);
    if (ActiveRecord == NULL)
        return M4_ERROR;

    ao_HasSlices = IRM.ItemHasSlices(ActiveRecord, ai_ItemPos);

    return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::SetItemHasSlices(const m4uint16_t ai_ItemPos,
                                                const regindex_t ai_Index,
                                                const m4bool_t   ai_iHasSlices)
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);
   M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   IRM.SetItemHasSlices((m4uint8_t*)Index[ai_Index],ai_ItemPos,ai_iHasSlices);

   return M4_SUCCESS;
}

// Priority of the item (0..3)
m4return_t ClVectorRecordBase::GetItemPriority(const m4uint16_t ai_ItemPos,
                                               const regindex_t ai_Index,
                                               m4char_t&        ao_iPriority) const
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);
   M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   ao_iPriority=IRM.GetItemPriority((m4uint8_t*)Index[ai_Index],ai_ItemPos);

   return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::SetItemPriority(const m4uint16_t ai_ItemPos,
                                               const regindex_t ai_Index,
                                               const m4char_t   ai_iPriority)
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);
   M4_ASSERT(ai_ItemPos < m_pSMData->NItems());
   M4_ASSERT(ai_iPriority >= 0 && ai_iPriority <= 3);

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   IRM.SetItemPriority((m4uint8_t*)Index[ai_Index],ai_ItemPos,ai_iPriority);

   return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::GetItemIsUser(const m4uint16_t ai_ItemPos,
											 const regindex_t ai_Index,
											 m4bool_t&        ao_bIsUser) const
{
   ao_bIsUser = M4_FALSE;

   M4_ASSERT(ai_Index < Count && ai_Index >= 0);
   M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   ao_bIsUser=IRM.GetItemIsUser((m4uint8_t*)Index[ai_Index],ai_ItemPos);

   return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::SetItemIsUser(const m4uint16_t ai_ItemPos,
											 const regindex_t ai_Index,
											 const m4bool_t   ai_bIsUser)
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);
   M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   IRM.SetItemIsUser((m4uint8_t*)Index[ai_Index],ai_ItemPos,ai_bIsUser);

   return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::SetItemNullByPos(const m4uint16_t		ai_ItemPos,
                                                const regindex_t		ai_Index,
                                                const m4bool_t			ai_IsNull,
												      m4bool_t			ai_bChangeStatus)
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());
    
    m4return_t	iRet = M4_ERROR;
	m4bool_t	bIsNull = M4_TRUE ;


	iRet = GetItemNullByPos( ai_ItemPos, ai_Index, bIsNull ) ;

	if( iRet == M4_SUCCESS && ai_IsNull == bIsNull )
	{
		return M4_SUCCESS ;
	}

	SetItemIsUser( ai_ItemPos, ai_Index, M4_FALSE ) ;

    RegisterStatus_t Status = RecordStatus(ai_Index);
    
    if (UseCP)
    {
        switch (Status)
        {
        case M4_RS_NORMAL:
            regindex_t TLIndex;
            
			if( ai_bChangeStatus == M4_TRUE )
			{
				// Insert and initialize the transaction lon index
				if (AddRecordToTransactionLog(ai_Index, TLIndex) != M4_SUCCESS)
					return M4_ERROR;
            
				if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
				{
					IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index], M4_TRUE);   // Set the persist in db flag
				}
			}
            
            if (IRM.IsItemBlob((m4uint8_t*)Index[ai_Index], ai_ItemPos) == M4_TRUE)
			{
				/* Bug 0133251
				Si no se cambia el estado hay que liberar lo que hubiera
				Si se cambia no hay que liberar
				*/
				if( ai_bChangeStatus == M4_FALSE )
				{
	                iRet = IRM.SetItemBlobNullRelease((m4uint8_t*)Index[ai_Index],ai_ItemPos,ai_IsNull);
				}
				else
				{
	                iRet = IRM.SetItemBlobNullNoRelease((m4uint8_t*)Index[ai_Index],ai_ItemPos,ai_IsNull);
				}
			}
            else
			{
                iRet = IRM.SetItemNoBlobNull((m4uint8_t*)Index[ai_Index],ai_ItemPos,ai_IsNull);
			}

            break;
            
        case M4_RS_MODIFIED:

			if( ai_bChangeStatus == M4_TRUE )
			{
				if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
				{
					IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index], M4_TRUE);  // Set the persist in db flag
				}
			}
            
            if (IRM.IsItemBlob((m4uint8_t*)Index[ai_Index], ai_ItemPos) == M4_TRUE)
            {
                m4bool_t IsTheSame;
                BlobChanged(ai_ItemPos, ai_Index, IsTheSame);
                
                if (!IsTheSame)
				{
                    iRet = IRM.SetItemBlobNullRelease((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_IsNull);
				}
                else
				{
                    iRet = IRM.SetItemBlobNullNoRelease((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_IsNull);
				}
            }
            else
			{
                iRet = IRM.SetItemNoBlobNull((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_IsNull);
			}

            break;
            
        case M4_RS_NEW:
            iRet = IRM.SetItemNull((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_IsNull);
            break;
            
        default:   // M4_RS_DELETED, M4_RS_MODIFIED_AND_DELETED, M4_RS_NEW_AND_DELETED, M4_RS_INVALID
            return M4_ERROR;
        }
    }
    else
    {
		if( ai_bChangeStatus == M4_TRUE )
		{
			switch (Status)
			{
			case M4_RS_NORMAL:
			case M4_RS_MODIFIED:
				IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_MODIFIED); // ESTADO
				if (m_pSMData->ItemIsPersisted(ai_ItemPos))
					IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index], M4_TRUE);   // Set the persist in db flag
				break;
            
			case M4_RS_NEW:
				// Nothing to do here
				break;
            
			default:   // M4_RS_INVALID, M4_RS_DELETED, M4_RS_MODIFIED_AND_DELETED, M4_RS_NEW_AND_DELETED
				return M4_ERROR;
			}
		}

        iRet = IRM.SetItemNull((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_IsNull);
    }

    if (iRet != M4_ERROR)
    {
        if (Status != M4_RS_NEW && ai_bChangeStatus == M4_TRUE)
		{
			State |= VS_MODIFIED; // ESTADO
			if (m_pSMData->ItemIsPersisted(ai_ItemPos))
				State |= VS_MODIFIED_IN_DB;   // Set the persist in db flag
		}

        // by Alex (Deltas) >>>>>>>>>>>>>>
        // Se podría optimizar y en vez de mandar todo el registro como modificado,
        // mandar una marca de hacer nulo
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnModifiedByPos(ai_Index);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
    }

    return iRet;
}

m4return_t ClVectorRecordBase::AddRecordAndSetLDBItems(regindex_t&      ao_Index,
                                                       ClRegister_Pipe* ai_pRegPipe,
                                                       const m4bool_t   ai_bAddRegister,
                                                       const regid_t    ai_iRecordID,
                                                       const m4bool_t   ai_bAuxLoad)
{
    m4bool_t AuxCP = UseCP;
    
    UseCP = M4_FALSE;
    
    // Como en InsertRecordsLL se cambia el State, lo guardamos para poder deshacer el cambio
    m4uint8_t auxState = State;
    
    if (ai_bAddRegister)
    {
        ao_Index = Count;
        m4return_t iRet = InsertRecordsLL(Count, ai_iRecordID, 1, M4_TRUE);
        if (iRet != M4_SUCCESS)
            return iRet;
    }
    else
    {
        // Si lo que voy a hacer es modificar el registro tengo que borrar el oldValue si lo tuviera
        // Para hacer esto puedo utilizar el CommitRecord
        if (AuxCP == M4_TRUE)
        {
            m4bool_t bRecordIsDeleted;
            
            UseCP = AuxCP;
            if (CommitRecord(ao_Index, bRecordIsDeleted) != M4_SUCCESS)
                return M4_ERROR;
            
            M4_MSG_ASSERT(bRecordIsDeleted == M4_FALSE, "No se deberia borrar ningun registro");
            UseCP = M4_FALSE;
        }
        
        // by Alex (Deltas) >>>>>>>>>>>>>>
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnModifiedByPos(ao_Index);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
    }
    
    // Ojo, cuando se llama a SetLotOfItems el registro no puede tener oldValue
    M4_ASSERT(_ExistsOldValue(ao_Index, AuxCP) == M4_FALSE);
    
    if (IRM.IsValid(Index[ao_Index]) != M4_TRUE)
        return M4_ERROR;

    if (IRM.SetLotOfItems((m4uint8_t*)Index[ao_Index], this, ai_pRegPipe, ai_bAuxLoad) != M4_SUCCESS)
        return M4_ERROR;

    IRM.SetStatus((m4uint8_t*)Index[ao_Index], M4_RS_NORMAL);
    
    // Recuperamos el state a su estado original
    State = auxState;
    
    UseCP = AuxCP;
    
    return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::GetItemValueByPos(const m4uint16_t ai_ItemPos,
                                                 void*            oi_Item,
                                                 const regindex_t ai_Index,
                                                 m4uint32_t&      ao_RealSize,
                                                 const m4uint32_t ai_MaxSize,
                                                 m4bool_t&        ao_IsNull) const
{
   return GetItemValueByPosAndOffset(ai_ItemPos,
                                     oi_Item,
                                     ai_Index,
                                     0,
                                     ao_RealSize,
                                     ai_MaxSize,
                                     ao_IsNull);
}

m4return_t ClVectorRecordBase::GetItemValueByPosAndOffset(const m4uint16_t ai_ItemPos,
                                                          void*            oi_Item,
                                                          const regindex_t ai_Index,
                                                          const m4uint32_t ai_Offset,
                                                          m4uint32_t&      ao_RealSize,
                                                          const m4uint32_t ai_MaxSize,
                                                          m4bool_t&        ao_IsNull) const
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);
   M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

   if (IRM.IsValid((m4uint8_t*)Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   return IRM.GetItemValueByOffset((m4uint8_t*)Index[ai_Index],
                                   ai_ItemPos,
                                   oi_Item,
                                   ai_Offset,
                                   ao_RealSize,
                                   ai_MaxSize,
                                   ao_IsNull);
}

m4return_t ClVectorRecordBase::SetItemValueByPosAndOffset(const m4uint16_t  ai_ItemPos,
                                                          const void* const ai_Item,
                                                          const regindex_t  ai_Index,
                                                          const m4uint32_t  ai_Offset,
                                                          const m4uint32_t  ai_Size,
                                                          m4uint32_t* const ao_pRetSize,
														  m4bool_t			ai_bChangeStatus,
														  m4bool_t			ai_bIsUser)
{
    M4_ASSERT(ai_Index >= 0 && ai_Index < Count);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

    m4return_t iRet = M4_ERROR;

    if( ai_Size == sizeof( m4double_t ) && IRM.IsItemBlob( (m4uint8_t*)Index[ai_Index], ai_ItemPos ) == M4_FALSE )
	{
		m4uint32_t	iSize = 0 ;
		m4bool_t	bIsNull = M4_TRUE ;
		m4uint8_t	abBuffer[ sizeof( m4double_t ) + 1 ] ;

		iRet = GetItemValueByPosAndOffset( ai_ItemPos, &abBuffer, ai_Index, ai_Offset, iSize, sizeof( m4double_t ), bIsNull ) ;

		if( iRet == M4_SUCCESS && bIsNull == M4_FALSE && iSize == sizeof( m4double_t ) && memcmp( ai_Item, abBuffer, sizeof( m4double_t ) ) == 0 )
		{
			return M4_SUCCESS ;
		}
	}

	SetItemIsUser( ai_ItemPos, ai_Index, ai_bIsUser ) ;

    RegisterStatus_t Status = RecordStatus(ai_Index);
    
    if (UseCP)
    {
        switch (Status)
        {
        case M4_RS_NORMAL:
            regindex_t TLIndex;
            
			if( ai_bChangeStatus == M4_TRUE )
			{
				// Insert and initialize the transaction log index
				if (AddRecordToTransactionLog(ai_Index, TLIndex) != M4_SUCCESS)
					return M4_ERROR;
            
				if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
				{
					IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index], M4_TRUE);   // Set the persist in db flag
				}
			}
            
            if (IRM.IsItemBlob((m4uint8_t*)Index[ai_Index],ai_ItemPos) == M4_TRUE)
			{
				/* Bug 0133251
				Si no se cambia el estado hay que liberar lo que hubiera
				Si se cambia no hay que liberar
				*/
				if( ai_bChangeStatus == M4_FALSE )
				{
					iRet = IRM.SetItemValueByOffset((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_Item, ai_Offset, ai_Size, ao_pRetSize);
				}
				else
				{
					iRet = IRM.SetItemValueByOffsetNoRelease((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_Item, ai_Offset, ai_Size);
				}
			}
            else
			{
                iRet = IRM.SetItemValueByOffset((m4uint8_t*)Index[ai_Index],ai_ItemPos, ai_Item, ai_Offset, ai_Size, ao_pRetSize);
			}
            
            break;
            
        case M4_RS_MODIFIED:

			if( ai_bChangeStatus == M4_TRUE )
			{
				if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
				{
					IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index],M4_TRUE);  // Set the persist in db flag
				}
			}
            
            if (IRM.IsItemBlob((m4uint8_t*)Index[ai_Index],ai_ItemPos) == M4_TRUE)
            {
                m4bool_t bIsTheSame;
                BlobChanged(ai_ItemPos, ai_Index, bIsTheSame);
                
                if (bIsTheSame == M4_FALSE)
				{
                    iRet = IRM.SetItemValueByOffset((m4uint8_t*)Index[ai_Index],ai_ItemPos, ai_Item, ai_Offset, ai_Size, ao_pRetSize);
				}
                else
				{
                    iRet = IRM.SetItemValueByOffsetNoRelease((m4uint8_t*)Index[ai_Index],ai_ItemPos, ai_Item, ai_Offset, ai_Size);
				}
            }
            else
			{
                iRet = IRM.SetItemValueByOffset((m4uint8_t*)Index[ai_Index],ai_ItemPos, ai_Item, ai_Offset, ai_Size, ao_pRetSize);
			}
            
            break;
            
        case M4_RS_NEW :

			if( ai_bChangeStatus == M4_TRUE )
			{
				IRM.SetNewNotPersist((m4uint8_t*)Index[ai_Index], M4_FALSE);
			}

            iRet = IRM.SetItemValueByOffset((m4uint8_t*)Index[ai_Index],ai_ItemPos, ai_Item, ai_Offset, ai_Size, ao_pRetSize);
            break;
            
        default:    // M4_RS_DELETED, M4_RS_MODIFIED_AND_DELETED, M4_RS_NEW_AND_DELETED, M4_RS_INVALID
            return M4_ERROR;
        }
    }
    else
    {
		if( ai_bChangeStatus == M4_TRUE )
		{
			switch (Status)
			{
			case M4_RS_NORMAL:
				IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_MODIFIED); // ESTADO

			case M4_RS_MODIFIED:
				if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
				{
					IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index], M4_TRUE); // Set the persist in db flag
				}

				break;
            
			case M4_RS_NEW:
				IRM.SetNewNotPersist((m4uint8_t*)Index[ai_Index], M4_FALSE);
				break;
            
			default:    // M4_RS_INVALID, M4_RS_DELETED, M4_RS_MODIFIED_AND_DELETED, M4_RS_NEW_AND_DELETED
				return M4_ERROR;
			}
		}
        
        iRet = IRM.SetItemValueByOffset((m4uint8_t*)Index[ai_Index],ai_ItemPos, ai_Item, ai_Offset, ai_Size, ao_pRetSize);
    }

    if (iRet != M4_ERROR)
    {
        if (Status != M4_RS_NEW && ai_bChangeStatus == M4_TRUE)
		{
            State |= VS_MODIFIED; // ESTADO
			if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
			{
				State |= VS_MODIFIED_IN_DB;   // Set the persist in db flag
			}
		}

        // by Alex (Deltas) >>>>>>>>>>>>>>
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnModifiedByPos(ai_Index);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
    }
    
    return iRet;
}

m4return_t ClVectorRecordBase::GetItemBlobFileDescription(const m4uint16_t  ai_ItemPos,
                                                          const regindex_t  ai_Index,
                                                          const void* & ao_ItemAddress) const
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        m4uint8_t* DataBuffer = (m4uint8_t*)Index[ai_Index];
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));
        ao_ItemAddress = pBlobFile->m_Description.GetDescription();
        return M4_SUCCESS;
    }

    return M4_ERROR;
}

m4return_t ClVectorRecordBase::SetItemBlobFileDescription(const m4uint16_t  ai_ItemPos,
                                                          const void* const ai_Item,
                                                          const regindex_t  ai_Index)
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        m4uint8_t* DataBuffer = (m4uint8_t*)Index[ai_Index];
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));
        return pBlobFile->m_Description.SetDescription((const m4char_t* const) ai_Item);
    }

    return M4_ERROR;
}

ClDsItemBlobFile* const ClVectorRecordBase::GetItemBlobFile(const m4uint16_t  ai_ItemPos,
                                                            const regindex_t  ai_Index) const
{
    // Alex blob
    if (m_pSMData->ItemLivesInFile(ai_ItemPos) == M4_TRUE)
    {
        m4uint8_t* DataBuffer = (m4uint8_t*)Index[ai_Index];
        ClDsItemBlobFile* pBlobFile = (ClDsItemBlobFile*)(DataBuffer + m_pSMData->ItemOffset(ai_ItemPos));
        return pBlobFile;
    }

    return NULL;
}

m4return_t ClVectorRecordBase::SetItemSizeByPos(const m4uint16_t ai_ItemPos,
                                                const regindex_t ai_Index,
                                                const m4uint32_t ai_NewSize)
{
    M4_ASSERT(ai_Index >= 0 && ai_Index < Count);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());
    
    m4return_t iRet = M4_ERROR;

    RegisterStatus_t Status = RecordStatus(ai_Index);
    
    if (UseCP)
    {
        switch (Status)
        {
        case M4_RS_NORMAL:
            regindex_t TLIndex;
            
            // Insert and initialize the transaction log index
            if (AddRecordToTransactionLog(ai_Index, TLIndex) != M4_SUCCESS)
                return M4_ERROR;
            
            if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
			{
                IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index], M4_TRUE);   // Set the persist in db flag
			}
            
            iRet = IRM.SetItemSize((m4uint8_t*)Index[ai_Index],ai_ItemPos, ai_NewSize, M4_FALSE);

            break;
            
        case M4_RS_MODIFIED:
            if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
			{
                IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index],M4_TRUE);  // Set the persist in db flag
			}
            
            if (IRM.IsItemBlob((m4uint8_t*)Index[ai_Index],ai_ItemPos) == M4_TRUE)
            {
                m4bool_t bIsTheSame;
                BlobChanged(ai_ItemPos, ai_Index, bIsTheSame);
                
                if (bIsTheSame == M4_FALSE)
				{
                    iRet = IRM.SetItemSize((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_NewSize, M4_TRUE);
				}
                else
				{
                    iRet = IRM.SetItemSize((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_NewSize, M4_FALSE);
				}
            }
            else
			{
                iRet = IRM.SetItemSize((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_NewSize, M4_FALSE);
			}

            break;

        case M4_RS_NEW :
			IRM.SetNewNotPersist((m4uint8_t*)Index[ai_Index], M4_FALSE);
            iRet = IRM.SetItemSize((m4uint8_t*)Index[ai_Index], ai_ItemPos, ai_NewSize, M4_TRUE);
            break;
            
        default:    // M4_RS_DELETED, M4_RS_MODIFIED_AND_DELETED, M4_RS_NEW_AND_DELETED, M4_RS_INVALID
            return M4_ERROR;
        }
    }
    else
    {
        switch (Status)
        {
        case M4_RS_NORMAL:
        case M4_RS_MODIFIED:
            IRM.SetStatus((m4uint8_t*)Index[ai_Index], M4_RS_MODIFIED); // ESTADO
            
            if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
			{
                IRM.SetModifiedInDB((m4uint8_t*)Index[ai_Index],M4_TRUE); // Set the persist in db flag
			}

            break;
            
        case M4_RS_NEW:
			IRM.SetNewNotPersist((m4uint8_t*)Index[ai_Index], M4_FALSE);
            break;
            
        default:    // M4_RS_INVALID, M4_RS_DELETED, M4_RS_MODIFIED_AND_DELETED, M4_RS_NEW_AND_DELETED
            return M4_ERROR;
        }

        iRet = IRM.SetItemSize((m4uint8_t*)Index[ai_Index],ai_ItemPos, ai_NewSize, M4_TRUE);
    }

    if (iRet != M4_ERROR)
    {
        State |= VS_MODIFIED; // ESTADO
        if (m_pSMData->ItemIsPersisted(ai_ItemPos) == M4_TRUE)
		{
            State |= VS_MODIFIED_IN_DB;   // Set the persist in db flag
		}

        // by Alex (Deltas) >>>>>>>>>>>>>>
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnModifiedByPos(ai_Index);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
    }

    return iRet;
}

m4return_t ClVectorRecordBase::GetItemSizeByPos(const m4uint16_t ai_ItemPos,
                                                const regindex_t ai_Index,
                                                m4uint32_t       &ao_Size) const
{
   M4_ASSERT(ai_Index < Count && ai_Index >= 0);
   M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

   // Segun Edu, no es necesario tomar en cuenta el status, por
   // lo tanto se simplifica el modelo
   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE) {
      ao_Size = 0;
      return M4_ERROR;
   }

   return IRM.GetItemSize((m4uint8_t*)Index[ai_Index],ai_ItemPos,ao_Size);
}

m4return_t ClVectorRecordBase::GetLastItemSizeByPos(const m4uint16_t ai_ItemPos,
                                                    const regindex_t ai_Index,
                                                    m4uint32_t       &ao_Size) const
{
    M4_ASSERT(ai_Index < Count && ai_Index >= 0);
    M4_ASSERT(ai_ItemPos < m_pSMData->NItems());

    m4uint8_t* ActiveRecord = _LastRecordToProcess(ai_Index);
    if (ActiveRecord == NULL)
        return M4_ERROR;

    return IRM.GetItemSize(ActiveRecord, ai_ItemPos, ao_Size);
}

m4return_t ClVectorRecordBase::GetUserDataByPosAndOffset(void*            ao_UserData,
                                                         const m4uint16_t ai_Offset,
                                                         const regindex_t ai_Index,
                                                         const m4uint16_t ai_Size) const
{
   M4_MSG_ASSERT(ai_Index<Count && ai_Index>=0,"Index fuera de rango en ClVectorRecordBase::GetUserDataByPosAndOffset");

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   IRM.GetUserDataByOffset((m4uint8_t*)Index[ai_Index],
                           ao_UserData,
                           sizeof(regindex_t) + sizeof(regid_t) + ai_Offset,
                           ai_Size);
   return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::SetUserDataByPosAndOffset(void*            ai_UserData,
                                                         const m4uint16_t ai_Offset,
                                                         const regindex_t ai_Index,
                                                         const m4uint16_t ai_Size)
{
   M4_MSG_ASSERT(ai_Index<Count,"Index fuera de rango en ClVectorRecordBase::SetUserDataByPosAndOffset");

   if (IRM.IsValid(Index[ai_Index]) != M4_TRUE)
      return M4_ERROR;

   IRM.SetUserDataByOffset((m4uint8_t*)Index[ai_Index],
                           ai_UserData,
                           sizeof(regindex_t) + sizeof(regid_t) + ai_Offset,
                           ai_Size);
   return M4_SUCCESS;
}

void ClVectorRecordBase::SetRecordVersion(const regindex_t  ai_iIndex,
                                          const VersionMark ai_Version)
{
    M4_ASSERT(ai_iIndex < Count);

    IRM.SetRecordVersion((m4uint8_t*)Index[ai_iIndex], ai_Version);

    // by Alex (Deltas) >>>>>>>>>>>>>>
    // Lo marco como modificado pora poder detectar cuando
    // cambia la versión del regsitro, sin que cambie nada más
    if (m_bKeepStatus == M4_TRUE)
	{
        m_pVVectorDelta->OnModifiedByPos(ai_iIndex);
	}
    // by Alex (Deltas) <<<<<<<<<<<<<<

	SetChange() ;
}

m4return_t ClVectorRecordBase::CheckPoint(void)
{
   if (UseCP)
   {
      if (State != VS_NORMAL)
	  {
         if (Commit() != M4_SUCCESS)
            return M4_ERROR;
	  }
   }
   else
   {
      ResetStatus(); // ESTADO
	  }

   return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::Commit(void)
{
    if (State == VS_NORMAL)
        return M4_SUCCESS;

    if (!UseCP) // ESTADO
    {
        ResetStatus();
        return M4_SUCCESS;
    }

    m4bool_t RecordIsDeleted = M4_FALSE;

    for (regindex_t regIndex = 0; regIndex < Count;)
    {
        if (CommitRecord(regIndex,RecordIsDeleted)!=M4_SUCCESS)
            return M4_ERROR;
        
        if (!RecordIsDeleted)
		{
            regIndex++;
		}
    }
    
    State = VS_NORMAL;
    
    return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::Rollback(void)
{
   if (!UseCP)
   {
      ResetStatus(); // ESTADO
   }
   else
   {
      m4bool_t RecordWasDeleted;

      if (State==VS_NORMAL)
         return M4_SUCCESS;

      for (regindex_t i=0; i<Count;)
      {
         UndoLL(i,RecordWasDeleted);

         if (!RecordWasDeleted)
		 {
            i++;
		 }
      }

      State=VS_NORMAL;
   }

   return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::Undo(regindex_t ai_Index)
{
	M4_ASSERT(ai_Index>=0);

   if (UseCP) // ESTADO
   {
      m4bool_t RecordWasDeleted;

      return UndoLL(ai_Index,RecordWasDeleted);
   }
   else
   {
      return M4_SUCCESS;
   }
}

m4return_t ClVectorRecordBase::CommitRecord(regindex_t ai_iIndex,
                                            m4bool_t&  ai_iRecordDeleted)
{
    regindex_t LastIndex;
    
    RegisterStatus_t Status = RecordStatus(ai_iIndex);
    
    switch (Status)
    {
    case M4_RS_NORMAL:
    case M4_RS_INVALID:
        // Nothing to do here
        ai_iRecordDeleted = M4_FALSE;
        break;
        
    case M4_RS_DELETED:
    case M4_RS_NEW_AND_DELETED:
        DeleteRecordLL(ai_iIndex);
        ai_iRecordDeleted = M4_TRUE;    // Caso en el que se borra el registro
        break;
        
    case M4_RS_MODIFIED:
        // Commit modifications

        // Borro el oldValue
        if (DestroyOldValue(ai_iIndex) != M4_SUCCESS)
            return M4_SUCCESS;

        // Set normal state
        IRM.SetStatus((m4uint8_t*)Index[ai_iIndex], M4_RS_NORMAL);
        
        // by Alex (Deltas) >>>>>>>>>>>>>>
        // ??? Lo marco como modificado porque ha cambiado su estado, se enviará el
        // registro entero, puedo optimizarlo mandando una marca de estado normal
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnModifiedByPos(ai_iIndex);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
        
        ai_iRecordDeleted = M4_FALSE;
        break;
        
    case M4_RS_MODIFIED_AND_DELETED:
        // Delete phisicaly actual and last
        if (UseCP)
        {
            LastIndex = GetLastRecordValueIndex(ai_iIndex);
            
            FreeMemoryFromActualAndLast(ai_iIndex, M4_TRUE);
            
            DeleteFromTransactionLog(LastIndex);
        }
        else
		{
            IRM.DeleteBlobsRecord((m4uint8_t*)Index[ai_iIndex]);
		}
        
        // Delete the last record (index)
        if (ai_iIndex<Count-1)
		{
            memmove(&Index[ai_iIndex],&Index[ai_iIndex+1],sizeof(ClMemPtr)*(Count-ai_iIndex-1));
		}
        
        Count--;

        // by Alex (Deltas) >>>>>>>>>>>>>>
        // Como no se usa DeleteRecordLL(ai_iIndex) tengo qe hacer aquí el borrado
        // de la información del delta
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->DeleteByPos(ai_iIndex);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
        
        ai_iRecordDeleted = M4_TRUE;    // Caso en el que se borra el registro
        break;
        
    case M4_RS_NEW:
		IRM.SetNewNotPersist((m4uint8_t*)Index[ai_iIndex], M4_FALSE);
        IRM.SetStatus((m4uint8_t*)Index[ai_iIndex], M4_RS_NORMAL);
        
        // by Alex (Deltas) >>>>>>>>>>>>>>
        // ??? Lo marco como modificado porque ha cambiado su estado, se enviará el
        // registro entero, puedo optimizarlo mandando una marca de estado normal
        if (m_bKeepStatus == M4_TRUE)
		{
            m_pVVectorDelta->OnModifiedByPos(ai_iIndex);
		}
        // by Alex (Deltas) <<<<<<<<<<<<<<

		SetChange() ;
        
        ai_iRecordDeleted = M4_FALSE;
        break;

    default:
        ai_iRecordDeleted = M4_FALSE;
        return M4_ERROR;
   }

   return M4_SUCCESS;
}

// Nueva funcion que recalcula el estado del VRB preguntando a cada uno de su registros
m4return_t ClVectorRecordBase::RefreshStatus(void)
{
    m4int32_t i;

    State = VS_NORMAL;

    for (i = 0; i < Count; i++)
    {
        switch (IRM.GetStatus((m4uint8_t*)Index[i]))
        {
        case M4_RS_NORMAL:
            break;
        case M4_RS_MODIFIED:
            State |= VS_MODIFIED;
			if (IRM.GetModifiedInDB((m4uint8_t*)Index[i]))
			{
				State |= VS_MODIFIED_IN_DB;
			}
            break;
        case M4_RS_DELETED:
            State |= VS_DELETED;
            break;
        case M4_RS_NEW:
            State |= VS_NEW;
            break;
        case M4_RS_MODIFIED_AND_DELETED:
            State |= VS_MODIFIED;
            State |= VS_DELETED;
			if (IRM.GetModifiedInDB((m4uint8_t*)Index[i]))
				State |= VS_MODIFIED_IN_DB;
            break;
        case M4_RS_NEW_AND_DELETED:
            State |= VS_NEW;
            State |= VS_DELETED;
            break;
        default:
            break;
        }
    }

    return M4_SUCCESS;
}

m4bool_t ClVectorRecordBase::ExistsOldValue(const regindex_t ai_iIndex) const
{
   if (IRM.IsValid((m4uint8_t*)Index[ai_iIndex]) != M4_TRUE)
      return M4_FALSE;

   return _ExistsOldValue(ai_iIndex,UseCP);
}

m4bool_t ClVectorRecordBase::_ExistsOldValue(const m4uint32_t ai_iIndex,
                                             const m4bool_t ai_bUseCP) const
{
   // Si no uso transacciones es imposible que tenga OldValue
   if (ai_bUseCP == M4_FALSE)
       return M4_FALSE;

   RegisterStatus_t recStat = IRM.GetStatus((m4uint8_t*)Index[ai_iIndex]);

   return (recStat == M4_RS_MODIFIED || recStat == M4_RS_MODIFIED_AND_DELETED) ? M4_TRUE : M4_FALSE;
}

m4return_t ClVectorRecordBase::DestroyOldValue(const regindex_t ai_iIndex)
{
    if (IRM.IsValid((m4uint8_t*)Index[ai_iIndex]) != M4_TRUE)
        return M4_ERROR;

    if (ExistsOldValue(ai_iIndex) == M4_FALSE)
        return M4_SUCCESS;

    regindex_t iLastIndex = GetLastRecordValueIndex(ai_iIndex);
    if (IRM.IsValid((m4uint8_t*)Index[iLastIndex]) != M4_TRUE)
        return M4_ERROR;

    return DestroyOldValueLL(ai_iIndex, iLastIndex);
}

m4return_t ClVectorRecordBase::DestroyOldValueLL(const regindex_t ai_iIndex,
                                                 const regindex_t ai_iLastIndex)
{
    // Cycle freeing the blobs of the oldRecord wich are different from the actual value
    BlobDataPtr BlobPtrActual;
    BlobDataPtr BlobPtrLast;
    m4uint16_t  NumItems = m_pSMData->NItems();
    for (m4uint16_t Item = 0; Item < NumItems; Item++)
    {
        IRM.GetItemBlobAddress((m4uint8_t*)Index[ai_iIndex], Item, BlobPtrActual);
        IRM.GetItemBlobAddress((m4uint8_t*)Index[ai_iLastIndex], Item, BlobPtrLast);
        
        if (BlobPtrLast != 0)
        {
            if (BlobPtrLast != BlobPtrActual)
            {
                if (IRM.SetItemBlobNullRelease((m4uint8_t*)Index[ai_iLastIndex], Item, M4_TRUE) != M4_SUCCESS)
                    return M4_ERROR;
            }
            else
            {
                #ifdef _DEBUG
                // No liberamos porque esta apuntando a él el record,
                // pero lo ponemos a NULL para que quede consistente
                //IRM.SetItemBlobNullNoRelease((m4uint8_t*)Index[LastIndex], Item, M4_TRUE);
                m4uint8_t* AuxPtr = (m4uint8_t*)Index[ai_iLastIndex] + m_pSMData->ItemOffset(Item);
                *((BlobDataPtr*)AuxPtr) = NULL;
                #endif
            }
        }
    }
    
    // Frees the static part of the last record
    FreeRecordMemory(ai_iLastIndex);
    
    // Update the transaction log (delete the entry)
    DeleteFromTransactionLog(ai_iLastIndex);

    return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::SerializeStaticPart(ClGenericIODriver& IOD, 
                                                   const m4uint8_t    ai_iRecSerMod,
												   ClChannelProcess*  ai_pChannelProcess,		
                                                   m4bool_t&          ao_thereIsAnyChange,
                                                   const m4uint8_t    ai_iVectorDelta)
{
	m4return_t iRet = M4_ERROR;

    if (ai_pChannelProcess) {
		iRet = ai_pChannelProcess->Pre_VRBStaticPart_Process(this);
        if (iRet != M4_SUCCESS) {
            DUMP_CHLOG_DEBUG(M4_CH_DS_PRE_PROCESS_VRB_STATIC);
            return M4_ERROR;
        }
    }

    // Segun el modo vemos si tenemos un vector de deltas pendiente para serializar
    ClVectorDelta* pAuxVDelta = 0;

    switch (ai_iRecSerMod) {
    case M4CL_RECORD_SERIAL_ALL:
        break;

    case M4CL_RECORD_SERIAL_DELTA:
        if (m_bKeepStatus == TRUE) {
            pAuxVDelta = m_pVVectorDelta->GetVector(ai_iVectorDelta);
        }
        break;

    case M4CL_RECORD_SERIAL_ADVANCED:
        pAuxVDelta = m_pVAdvancedSerial;
        break;

    default:
        M4_ASSERT(0);
        goto RETURN_ERROR;
    }

    // Indico si voy a serializar un vector de deltas
    iRet = IOD.Write((m4bool_t) (pAuxVDelta ? M4_TRUE : M4_FALSE));
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    iRet = IOD.Write(Count);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    if (pAuxVDelta)
	{
        iRet = pAuxVDelta->Serialize(IOD);  // Serializo el vector de deltas
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
        
        // Indico al nivel superior si ha habido cambios en el vector de deltas
        ao_thereIsAnyChange = pAuxVDelta->ThereIsAnyChange();
    }
	else
	{
        ao_thereIsAnyChange = M4_TRUE;   // Con absolutos es como si todo fueran cambios
	}

	iRet = IOD.Write(UseCP);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
    
    iRet = IOD.Write(State);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    if (ai_pChannelProcess) {
        iRet = ai_pChannelProcess->Post_VRBStaticPart_Process(this);
#ifdef _DEBUG
        if (iRet != M4_SUCCESS) {
            DUMP_CHLOG_DEBUG(M4_CH_DS_POST_PROCESS_VRB_STATIC);
		}
#endif
    }

	return iRet;

RETURN_ERROR:
	DUMP_CHLOG_ERROR(M4_CH_DS_SERIALIZE_VRB_STATIC);
    return iRet;
}

m4return_t ClVectorRecordBase::SerializeRecord(ClGenericIODriver&      IOD,
                                               m4uint8_t&              aio_iRecSerMod,
                                               const m4uint8_t         ai_iIteSerMod,
											   ClChannelProcess* const ai_pChannelProcess,
											   ClCsTraceSize*    const ai_pCsTraceSize,
                                               const regindex_t        ai_iIndex,
											   const m4uint8_t         ai_iVectorDelta)
{
    M4_ASSERT(ai_iIndex >= 0 && ai_iIndex < Size);
    M4_ASSERT(ai_iIndex < Count);
    
	m4return_t ret = M4_ERROR;

    if (ai_pChannelProcess) {
		ret = ai_pChannelProcess->Pre_VRBRecord_Process(this, (m4uint8_t*)Index[ai_iIndex], ai_iIndex);
        if (ret != M4_SUCCESS) {
            DUMP_CHLOG_DEBUGF(M4_CH_DS_PRE_PROCESS_VRB_RECORD, ai_iIndex);
            return ret;
        }
    }
	
    switch (aio_iRecSerMod)
    {
    case M4CL_RECORD_SERIAL_ALL:
        break;
		
    case M4CL_RECORD_SERIAL_DELTA:
		if (m_bKeepStatus == M4_TRUE) {
			// Comprobamos si existe el vector de deltas. Esto sólo es necesario porque cuando estoy depurando puede que el
			// bloque se crease en el server (no existía en el cliente), con lo que sólo va a tener un vector de deltas.
			// Cuando termian la depuración voy a pedir el segundo vector y me va a devolver 0.
			ClVectorDelta* pAuxVDelta = m_pVVectorDelta->GetVector(ai_iVectorDelta);
			if (pAuxVDelta)
			{
				// I need to send the record only if is a new record or is a modified record.
				ClDeltaInfo& oDeltaInfo = pAuxVDelta->Get(ai_iIndex);
				if (oDeltaInfo.IsInserted() == M4_FALSE && oDeltaInfo.IsModified() == M4_FALSE) return M4_SUCCESS;
			}
        }
        break;
        
    case M4CL_RECORD_SERIAL_ADVANCED:
        if (m_pVAdvancedSerial != NULL) {
            // I need to send the record only if is a new record or is a modified record
            ClDeltaInfo& oDeltaInfo = m_pVAdvancedSerial->Get(ai_iIndex);
            
            if (oDeltaInfo.IsInserted() == M4_FALSE && oDeltaInfo.IsModified() == M4_FALSE) return M4_SUCCESS;
			
            // TODO: deberia hacerse aqui, o la decision de cambiar el modo de
            // serializacion la deberia tomar otro (px_block)
            m4bool_t bNeedSerial;
            m4bool_t bBranchMode;
            GetNeedSerial(ai_iIndex, bNeedSerial, bBranchMode);
            
            if (bBranchMode == M4_TRUE)
			{
                aio_iRecSerMod = M4CL_RECORD_SERIAL_ALL;
			}
        }
        break;
		
    default:
        M4_ASSERT(0);
        return M4_ERROR;
    }
	
    //
    // Hago una serialización normal
    //
    
    // Calls IRM to serialize the record
	ret = IRM.Serialize(IOD, ai_iIteSerMod, ai_pChannelProcess, ai_pCsTraceSize, (m4uint8_t*)Index[ai_iIndex]);
    if (ret != M4_SUCCESS) {
        DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_VRB_RECORD, ai_iIndex);
        return ret;
    }
	
    // Serializo el OldValue, si lo tuviera
    if (_ExistsOldValue(ai_iIndex, UseCP) == M4_TRUE) {
        regindex_t oldValueIndex = GetLastRecordValueIndex(ai_iIndex);
        
		ret = IRM.IsValid((m4uint8_t*)Index[oldValueIndex]);
        if (ret != M4_TRUE) return ret;
		
		ret = IRM.Serialize(IOD, ai_iIteSerMod, ai_pChannelProcess, ai_pCsTraceSize, (m4uint8_t*)Index[oldValueIndex]);
        if (ret != M4_SUCCESS) {
            DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_VRB_RECORD, oldValueIndex);
            return ret;
        }
    }
	
    if (ai_pChannelProcess) {
		ret = ai_pChannelProcess->Post_VRBRecord_Process(this);
        if (ret != M4_SUCCESS) {
            DUMP_CHLOG_DEBUGF(M4_CH_DS_POST_PROCESS_VRB_RECORD, ai_iIndex);
            return ret;
        }
    }
	
    return ret;
}

m4return_t ClVectorRecordBase::DeSerializeStaticPart(ClGenericIODriver& IOD, m4bool_t& ao_bThereIsAnyChange)
{
    m4return_t iRet = DeSerializeFirstStaticPart(IOD);
    if (iRet != M4_SUCCESS) return iRet;

    return DeSerializeSecondStaticPart(IOD, ao_bThereIsAnyChange);
}

m4return_t ClVectorRecordBase::DeSerializeFirstStaticPart(ClGenericIODriver& IOD)
{
	// Durante la deserialización no quiero acumular cambios. Este miembro se volverá a asignar en
	// en el EndDeserialize con el valor que corresponda.
	m_bKeepStatus = M4_FALSE;

	M4_ASSERT(m_bDeSerializingDeltas == M4_FALSE);
    CHECK_CHLOG_ERROR(IOD.Read(m_bDeSerializingDeltas) != M4_SUCCESS,
        M4_ERROR,
        M4_CH_DS_DESERIALIZE_VRB_STATIC);

   return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::DeSerializeSecondStaticPart(ClGenericIODriver& IOD, m4bool_t& ao_bThereIsAnyChange)
{
    m4return_t iRet = IOD.Read(m_iCountDelta);
    if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
    if (m_bDeSerializingDeltas == M4_TRUE)
	{
        // Preparo un vector de deltas para hacer la deserializacion
		iRet = CreateVVectorDelta(m_iCountDelta, GrowRecords, 1);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;

        // Deserializo la informacion de deltas
		ClVectorDelta* pVectorDelta = m_pVVectorDelta->GetVector(0);
		iRet = pVectorDelta->DeSerialize(IOD);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
		ao_bThereIsAnyChange = pVectorDelta->ThereIsAnyChange();
		m_bDeSerializingDeltas = ao_bThereIsAnyChange;
    }
	else
	{
        // Es una deserialización normal de un absoluto
		
        ao_bThereIsAnyChange = M4_TRUE;   // Cuando tratamos absolutos siempre hay cambios
		
        // Un ClRecordWithTransactions ya tiene 1 registro desde su creacion
        // Si el Count es 0 se trata de un ClGenericDataSet, y por lo tanto hace falta crear los registos
        // Al crear los registros ya se actualiza el count
        if (Count == 0 && m_iCountDelta > 0) {
            // Le digo que lo inicialice para que le ponga los valores por defecto a los items que no son mapped
            iRet = InsertRecordsLL(0, -1, m_iCountDelta, M4_TRUE);
            M4_ASSERT(iRet != M4_VRB_ERROR_MAX_COUNT);

			if( iRet == M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED )
			{
				return iRet;
			}
        }
    }
	
	iRet = IOD.Read(m_bUseCPDelta);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	iRet = IOD.Read(m_StateDelta);
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
    return iRet;
	
RETURN_ERROR:
	DUMP_CHLOG_DEBUG(M4_CH_DS_DESERIALIZE_VRB_STATIC);
    return iRet;
}

m4return_t ClVectorRecordBase::GetDeltaInfo(m4uint32_t  ao_vMovedIndex[],
                                            regindex_t& ao_iNumDeletedIndex,
                                            m4bool_t&   ao_bArrange)
{
    M4_ASSERT(IsDeSerializingDeltas() == M4_TRUE);
    M4_ASSERT(Count > 0);   // Si antes no tenia registros no hay que hacer nada
    
    // En principio no hay que reordenar nada
    ao_bArrange = M4_FALSE;

    // Si no hay ningun delta tampoco hago nada, simplemente se borran todos los registros
    if (m_iCountDelta > 0) {
        //
        // Creo e inicializo un vector para marcar los registros que hay que borrar
        // Tambien inicializo el vector de movimientos, como si no hubiera que mover nada
        //
        m4bool_t* vDeletedIndex = new m4bool_t [Count];
        if (!vDeletedIndex) return M4_ERROR;
        regindex_t i;
        for (i = 0; i < Count; i++) {
            ao_vMovedIndex[i] = i;
            vDeletedIndex[i] = M4_TRUE;
        }
        
        //
        // Analizo la informacion de deltas para dar valores correctos a los vectores
        //
        //regindex_t iIns = 0;
        regindex_t iMov = 0;
        regindex_t iClientIndex;
        
        ClDeltaInfo oDeltaInfo;
		ClVectorDelta* pVectorDelta = m_pVVectorDelta->GetVector(0);

        for (i = 0 ; i < m_iCountDelta && iMov < Count ; i++) {
            oDeltaInfo = pVectorDelta->Get(i);
            
            if (oDeltaInfo.IsInserted() == M4_FALSE) {
                iClientIndex = oDeltaInfo.ClientIndex();
                if ((regindex_t)ao_vMovedIndex[iMov] != iClientIndex) {
                    ao_vMovedIndex[iMov] = iClientIndex;
                    ao_bArrange = M4_TRUE;
                }
                vDeletedIndex[iClientIndex] = M4_FALSE;
                iMov++;
            }
            /* ??? creo que las inserciones las puedo hacer desde dentro , ¿ pero gano algo ?
            else
            {
                M4_ASSERT(iIns < Count);

                ao_vInsertedIndex[iIns] = i;
                iIns++;
            }
            */
        }
        
        ao_iNumDeletedIndex = Count - iMov;
        
        // Ya tengo al principio del vector todos los movimientos
        // Tengo que asegurar que los borrados han quedado todos al final
        for (i = 0; i < Count && iMov < Count; i++) {
            if (vDeletedIndex[i] == M4_TRUE) {
                ao_vMovedIndex[iMov] = i;
                iMov++;
            }
		}
            
        delete[] vDeletedIndex;
    }
	else
	{
        ao_iNumDeletedIndex = Count;  // Tengo que borrar todos los registros
	}
    
    return M4_SUCCESS;
}

m4return_t ClVectorRecordBase::DeSerializeRecord(ClGenericIODriver&   IOD,
                                                 const m4uint8_t      ai_iIteSerMod,
                                                 const regindex_t     ai_iIndex,
                                                 ClDeltaInfo&         ao_oDeltaInfo,
												 ClCsTraceSize* const ai_pCsTraceSize)
{
	m4return_t iRet = M4_ERROR;
    regindex_t iLastIndex = -1;

    
	if( IsDeSerializingDeltas() == M4_TRUE )
	{
		for(;;)
		{
			//
			// Tengo que deserializar un delta de un registro
			//
			ao_oDeltaInfo = m_pVVectorDelta->GetVector(0)->Get(ai_iIndex);
        
			if( ao_oDeltaInfo.IsInvalid() == M4_TRUE )
			{
				// Estamos en una serializcion avanzada
				// Hay que insertar el registro y marcarlo como invalido para
				// que al intentar trabajar con el de un error
				iRet = InsertRecordsLL(ai_iIndex, -1, 1, M4_FALSE);
				if (iRet != M4_SUCCESS) {
					goto RETURN_ERROR;
				}

				IRM.SetStatus((m4uint8_t*)Index[ai_iIndex], M4_RS_INVALID);

				return M4_SUCCESS;
			}

			if( ao_oDeltaInfo.IsInserted() == M4_TRUE )
			{
				// Es un registro nuevo, tengo que insertarlo
				// Le digo que lo inicialice para que le ponga los valores por defecto a los items que no son mapped
				iRet = InsertRecordsLL(ai_iIndex, -1, 1, M4_TRUE);
				if (iRet != M4_SUCCESS) {
					goto RETURN_ERROR;
				}
				break;
			}

			if( ao_oDeltaInfo.IsModified() == M4_TRUE )
			{
				SetChange() ;
				break;  // No tengo que hacer nada especial, entro por la deserializacion normal
			}

			if( ao_oDeltaInfo.IsDeleted() == M4_TRUE )
			{
				// Es una marca de borrado, modifico el estado del registro
				iRet = _SetDeleteRecordFlag(ai_iIndex);
				if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			}

			// Si llego hasta aqui es porque no ha habido ningun cambio sobre el registro
			return M4_SUCCESS;
		}
	}
	else
	{
		SetChange() ;
	}

    m4bool_t bOnlyMappedItems;
	bOnlyMappedItems = (ai_iIteSerMod == M4CL_ITEM_SERIAL_MAPPED) ? M4_TRUE : M4_FALSE;

    // Tengo que borrar los blobs antes de la deserializacion, no quiero perdidas de memoria
    // Si existia oldValue borro sus blobs, pero no lo borro por si puedo reutilizarlo
    if (_ExistsOldValue(ai_iIndex, UseCP) == M4_TRUE)
	{
        iLastIndex = GetLastRecordValueIndex(ai_iIndex);
        FreeMemoryFromActualAndLast(ai_iIndex, M4_FALSE, bOnlyMappedItems);
    }
	else
	{
        IRM.DeleteBlobsRecord((m4uint8_t*)Index[ai_iIndex], bOnlyMappedItems); // ESTO ESTA BIEN ??? CRG
	}

    //
    // Hago una deserialización normal
    //
	iRet = IRM.DeSerialize(IOD, ai_iIteSerMod, (m4uint8_t*)Index[ai_iIndex], ai_pCsTraceSize);
    if (iRet != M4_SUCCESS) goto RETURN_ERROR;

    // DeSerializo el oldValue, si lo tenia en remoto
    // Por tratarse del oldValue del remoto tenogo que usar el UseCP del remoto
    if (_ExistsOldValue(ai_iIndex, m_bUseCPDelta) == M4_TRUE)
	{
        if (iLastIndex == -1) {
            // Antes no tenia oldValue, creo uno nuevo
			iRet = AddRecordToTransactionLogWithoutCopy(iLastIndex);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
            
            // Si solo se serializaron los items mapped tengo que copiar los valores para
            // los items que no son mapped. Si estoy serializando todos los items no hay
            // problema ya que se van a deserializar todos
            if (ai_iIteSerMod == M4CL_ITEM_SERIAL_MAPPED) {
                // Me interesa que copie los valores por defecto para los items que no son mapped
                // El resto de valores se sobreescriben al deserializar
                CopyRecordToOldValue(ai_iIndex, iLastIndex, M4_FALSE);
            }
        }
        
        // Connect the old value with the actual one
        SetLastRecordValueIndex(ai_iIndex, iLastIndex);
        
        // DeSerialize de oldValue
		iRet = IRM.DeSerialize(IOD, ai_iIteSerMod, (m4uint8_t*)Index[iLastIndex], ai_pCsTraceSize);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;

        // Alex blob
        // Si tengo algun ItemBlob tengo que decirle que esta en un oldValue
        if (m_pSMData->SomeItemLivesInFile() == M4_TRUE) {
            for (m4uint16_t iItem = 0; iItem < m_pSMData->NItems(); iItem++)
			{
                if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE)
				{
                    m4uint32_t iItemOffset = m_pSMData->ItemOffset(iItem);
                    ClDsItemBlobFile* pDsItemBlobFile = (ClDsItemBlobFile*)(((m4uint8_t*)Index[iLastIndex]) + iItemOffset);
                    
                    // Como el ItemBlobFile esta en un oldValue le mando un evento para que se entere
                    // y deje de prestar atencion a los eventos del fichero
                    m4bool_t bItemIsOldValue = M4_TRUE;
                    ClEventDescription oEventDescription(EVENT_TYPE_RM, (void*)&bItemIsOldValue);
                    pDsItemBlobFile->EventHandler(oEventDescription);
                }
            }
        }
    }
	else
	{
        // Si antes tenia oldValue y ahora no, tengo que borrarlo
        // LLamo a DestroyOldValueLL para que se borren los blobs que no eran mapped
        if (iLastIndex != -1)
		{
            iRet = DestroyOldValueLL(ai_iIndex, iLastIndex);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
        }
    }

    // Hacemos un reset de PersdistLDB mark para todos los registros
    IRM.SetPersistLDB((m4uint8_t*)Index[ai_iIndex], M4_FALSE);

    return M4_SUCCESS;

RETURN_ERROR:
	// User readable error saying rows quota exceeded already registered.
	if( iRet != M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED )
	{
		DUMP_CHLOG_ERRORF(M4_CH_DS_DESERIALIZE_VRB_RECORD, ai_iIndex);
	}
	return iRet;
}

m4return_t ClVectorRecordBase::EndDeSerialize(const m4uint8_t ai_iVectorDeltaCount)
{
	m4return_t iRet = M4_SUCCESS;

    // Update the values of the static part
    UseCP = m_bUseCPDelta;
    State = m_StateDelta;

    m_bDeSerializingDeltas = M4_FALSE;
    
    // Si antes tenia informacion de deltas se ha quedado inservible. Tengo que borrarla (si solo tengo
    // un vector de deltas), o hacer un reset del m_iActiveVector (si tego mas de un vector de deltas).
	
	// EFV & MFS
	// Fix bug 0063669
	// If we have more than one vector of deltas we have to reset only the first one
	m_bKeepStatus = M4_FALSE;
	if (ai_iVectorDeltaCount == 0 || (Count == 0 && m_pVVectorDelta && m_pVVectorDelta->Count() <= ai_iVectorDeltaCount))
	{
		delete m_pVVectorDelta;	
		m_pVVectorDelta = 0;
	}
	else
	{
		// we only need a new vector if we have registers
		if (Count != 0 || m_pVVectorDelta != 0)
		{
			iRet = CreateVVectorDelta(Count, GrowRecords, ai_iVectorDeltaCount);
			m_bKeepStatus = M4_TRUE;
		}

		if (iRet != M4_SUCCESS) {
			DUMP_CHLOG_ERROR(M4_CH_DS_DESERIALIZE_VRB_END);
		}
	}

    return iRet;
}

m4return_t ClVectorRecordBase::CreateVVectorDelta(const regindex_t ai_iInitialRecords,
												  const regindex_t ai_iGrowRecords,
												  const m4uint8_t ai_iVectorDeltaCount)
{
	if (!m_pVVectorDelta) {
		m_pVVectorDelta = new ClVVectorDelta;
		if (!m_pVVectorDelta) return M4_ERROR;
	}
	
	m4return_t iRet = m_pVVectorDelta->Init(ai_iInitialRecords, ai_iGrowRecords, ai_iVectorDeltaCount);
	if (iRet != M4_SUCCESS) {
		delete m_pVVectorDelta;
		m_pVVectorDelta = 0;
	}

	return iRet;
}

void ClVectorRecordBase::UpdateIndex(m4uint32_t *ai_pindex_qtc)
{    
    M4_ASSERT (Count);

//nos pasan el mapa de transformación de indice Q{0,1,2,3}
//  ej: si se tratara de invertir el índice el mapa sería: Q{3,2,1,0}
//vamos a aplicar la misma transformación sobre el Indice (que puede estar
//ordenado de otra forma (no sólo 0,1,2,3)
//la formula será: I[i] = I[Q[i]]

//la mejora que queda por hacer consiste en hacer la asignación 'in situ', sin utilizar
//arrays intermedios:

   ClMemPtr *paux = new ClMemPtr [Count];

   register m4uint32_t i = Count - 1;    

   do {
      paux [i] = Index [ai_pindex_qtc [i]];
   } while (i--);

   memcpy (Index, paux, Count * sizeof (ClMemPtr));
   delete paux;

   // by Alex (Deltas) >>>>>>>>>>>>>>
   // Reordeno la información de los deltas
   if (m_bKeepStatus == M4_TRUE)
   {
      m_pVVectorDelta->UpdateIndex(ai_pindex_qtc);
   }
   // by Alex (Deltas) <<<<<<<<<<<<<<

   SetChange() ;
}

m4return_t ClVectorRecordBase::PrepareIndexPL (const m4bool_t   ai_bUp,
                                               const m4uint32_t ai_PrevReg)
{
   ClMemPtr *paux = new ClMemPtr [ai_PrevReg];
   m4uint32_t m_isize = sizeof (ClMemPtr);
   if (ai_bUp)
   {
	   memcpy (paux, &Index[Count - ai_PrevReg], ai_PrevReg * m_isize);
	   memmove (&Index[ai_PrevReg], Index, (Count - ai_PrevReg) * m_isize);
	   memcpy (Index, paux, ai_PrevReg * m_isize);
   }
   else
   {
	   memcpy (paux, Index, ai_PrevReg * m_isize);
	   memmove (Index, &Index[ai_PrevReg], (Count - ai_PrevReg) * m_isize);
	   memcpy (&Index[Count - ai_PrevReg], paux, ai_PrevReg * m_isize);
   }

   return M4_SUCCESS;
}

// by Alex (Deltas) >>>>>>>>>>>>
m4return_t ClVectorRecordBase::SetKeepStatus(const m4bool_t  ai_keepStatus, const m4uint8_t ai_iVectorCount)
{
	if (Index) {
		if( m_pVVectorDelta != NULL )
		{
			delete m_pVVectorDelta;
			m_pVVectorDelta = NULL;
		}
		
		// Si no hay registros en el VRB no tiene sentido añadir informacion de deltas, ya que todo van a
		// ser inserciones. Nos ahorramos el tratamiento y el enviar informacion extra
		if (ai_keepStatus == M4_TRUE && Count > 0) {
			m_pVVectorDelta = new ClVVectorDelta;
			if (!m_pVVectorDelta) return M4_ERROR;
			
			m4return_t iRet = m_pVVectorDelta->Init(Count, GrowRecords, ai_iVectorCount);
			if (iRet != M4_SUCCESS) {
				delete m_pVVectorDelta;
				m_pVVectorDelta = NULL;
				return M4_ERROR;
			}
			m_bKeepStatus = M4_TRUE;
		} else {
			m_bKeepStatus = M4_FALSE;
		}
	}
	return M4_SUCCESS;
}

m4bool_t ClVectorRecordBase::GetKeepStatus(void) const
{
    return m_bKeepStatus;
}
// by Alex (Deltas) <<<<<<<<<<<<

m4return_t ClVectorRecordBase::PrepareAdvanzedSerial(void)
{
	// El VRB prepara un vector de deltas con los registros marcados para serializar
	// El resto de los registros se marcan en el vector de deltas como invalidos,
	// de esta forma no se serializan pero en el otro extremo si se crean (con estado invalido)
	// De esta forma seguimos manteniendo la posicion y el numero de registros dentro del VRB
    M4_ASSERT(m_pVAdvancedSerial == NULL);

	m4return_t iRet = M4_SUCCESS;
	m4bool_t bNeedSerial;
	m4bool_t bBranchMode;
	
	if (Count > 0) {
        // Creo un nuevo vector de deltas para la serializacion avanzada
		m_pVAdvancedSerial = new ClVectorDelta;
        if (!m_pVAdvancedSerial) goto RETURN_ERROR;
		
		iRet = m_pVAdvancedSerial->Init(Count, GrowRecords);
        if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
		for (regindex_t i = 0; i < Count; i++) {
			iRet = GetNeedSerial(i, bNeedSerial, bBranchMode);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			
			if (bNeedSerial == M4_TRUE) {
                m_pVAdvancedSerial->Get(i).ClientIndex(VD_NEW_RECORD);    // En el otro extremo se inserta un nuevo registro
			} else {
                m_pVAdvancedSerial->Get(i).OnInvalid();      // No se intenta deserializar y se marca como invalido
			}
			// Si se inserta
		}
        m_pVAdvancedSerial->ThereIsAnyChange(M4_TRUE);
	}
	
	return iRet;

RETURN_ERROR:
	if (m_pVAdvancedSerial) {
		delete m_pVAdvancedSerial;
		m_pVAdvancedSerial = 0;
	}
	return iRet;
}

regindex_t ClVectorRecordBase::GetIndexByRecordAddress(const void* const ai_pDataBuffer,
                                                       m4bool_t&         ao_oldValue) const
{
    // Busco en los registros normales
	regindex_t i;
    for (i = 0; i < Count; i++)
    {
        if (Index[i] == ai_pDataBuffer)
        {
            ao_oldValue = M4_FALSE;
            return i;
        }
    }
    
    // Busco en los oldValues
    regindex_t oldIndex = LogIndex + 1;
    for (i = 0; i < LogCount; i++, oldIndex++)
    {
        if (Index[oldIndex] == ai_pDataBuffer)
        {
            ao_oldValue = M4_TRUE;
            return oldIndex;
        }
    }
    
    return -1;
}

m4return_t ClVectorRecordBase::GetSize(ClSize& ao_size) const
{
    //
    // Primero calculo el tamaño del VRB
    //
    ao_size += sizeof(ClVectorRecordBase);
    
    if (Index != NULL)
	{
        ao_size += sizeof(Index[0]) * Size;  // Tamaño del vector de registros
	}
    
    ao_size -= sizeof(m_pSMData);
    if (m_pSMData->GetSize(ao_size) != M4_SUCCESS)
        return M4_ERROR;
    
    ao_size -= sizeof(IRM);
    if (IRM.GetSize(ao_size) != M4_SUCCESS)
        return M4_ERROR;
    
    if (m_pVVectorDelta != NULL)
    {
        if (m_pVVectorDelta->GetSize(ao_size) != M4_SUCCESS)
            return M4_ERROR;
    }
    
    //
    // Ahora calculo el tamaño de todos los registros. Ojo que un registro y su oldValue
    // pueden apuntar al mismo blob, este no debe ser contado dos veces
    //
    BlobDataPtr       pBlob;
    BlobDataPtr       pBlobOldValue;
    ClDsItemBlobFile* pBlobFile;
    ClDsItemBlobFile* pBlobFileOldValue;
    m4uint16_t        iNumItems = m_pSMData->NItems();
    m4uint16_t        iItem;
    regindex_t        iOldValueIndex;
    for (regindex_t iIndex = 0; iIndex < Count; iIndex++)
    {
        // Tamaño del registro
        ao_size += m_pSMData->RecordSize();

        // Tamaño del oldValue
        m4bool_t bExistsOldValue = ExistsOldValue(iIndex);
        if (bExistsOldValue == M4_TRUE)
        {
            ao_size += m_pSMData->RecordSize();
            
            iOldValueIndex = GetLastRecordValueIndex(iIndex);   // Indice del oldValue
        }

        // Tamaño de sus blobs en memoria
        for (iItem = 0; iItem < iNumItems; iItem++)
        {
            if (m_pSMData->ItemLivesInFile(iItem) == M4_TRUE)
            {
                pBlobFile = (ClDsItemBlobFile*)(((m4uint8_t*)Index[iIndex]) + m_pSMData->ItemOffset(iItem));
                M4_ASSERT(pBlobFile != NULL);

                if (pBlobFile->GetUsedMemory(ao_size) != M4_SUCCESS)
                    return M4_ERROR;

                // En caso de que tenga oldValue compruebo si apuntan al mismo o a distintos blobFile
                if (bExistsOldValue == M4_TRUE)
                {
                    pBlobFileOldValue = (ClDsItemBlobFile*)(((m4uint8_t*)Index[iOldValueIndex]) + m_pSMData->ItemOffset(iItem));
                    M4_ASSERT(pBlobFileOldValue != NULL);

                    if (pBlobFile->GetValueAddress() != pBlobFileOldValue->GetValueAddress())
                    {
                        if (pBlobFileOldValue->GetUsedMemory(ao_size) != M4_SUCCESS)
                            return M4_ERROR;
                    }
                }
            }
            else
            {
                IRM.GetItemBlobAddress((m4uint8_t*)Index[iIndex], iItem, pBlob);
                if (pBlob != NULL)
                {
                    if (pBlob->GetSize(ao_size) != M4_SUCCESS)
                        return M4_ERROR;
                }
                
                // En caso de que tenga oldValue compruebo si apuntan al mismo o a distintos blob
                if (bExistsOldValue == M4_TRUE)
                {
                    IRM.GetItemBlobAddress((m4uint8_t*)Index[iOldValueIndex], iItem, pBlobOldValue);
                    if (pBlobOldValue != NULL && pBlobOldValue != pBlob)
                    {
                        if (pBlobOldValue->GetSize(ao_size) != M4_SUCCESS)
                            return M4_ERROR;
                    }
                }
            }
        }
    }
    return M4_SUCCESS;
}
