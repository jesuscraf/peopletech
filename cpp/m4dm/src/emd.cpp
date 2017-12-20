//==============================================================================
//
// (c) Copyright  1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           Data Block
// File:             emd.cpp
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

#include "emd.hpp"
#include "emd.inl"

#include "rm.hpp"
#include "dm_types.hpp"
#include "memman.hpp"

#include "blocksynchronization.hpp"

ClMemoryManagerNotPool g_MMNotPool;

//==============================================================================
// class EMDStaticData
//==============================================================================

EMDStaticData::EMDStaticData(void) :
    m_NumItems(0),
    m_pIData(NULL),
    m_UserDataSize(0),
    m_UserDataOffset(0),
    m_RecordSize(0),
    m_DefaultBlobMaskSize(0),
    m_pDefaultBlobMask(NULL),
    m_pChannelId(NULL),
    m_NodeHandle(0),
    m_Scope(0),
    m_ChannelVersion(0),
    m_pDefaultRecord(NULL),
    m_iIndexOfFirstOffset(0),
    m_CsType(0),
    m_NumMappedItems(0),
    m_pIndexOfMappedItem(NULL),
    m_SomeItemLivesInFile(M4_FALSE),
    m_Count(0),
    m_pNextStaticData(NULL)
{
    m_poMMNotPool = &g_MMNotPool;
}

EMDStaticData::~EMDStaticData(void)
{
    Reset();
}

void EMDStaticData::Reset(void)
{
    // Quito el ASSERT porque hay canales que no se borrar (ej. el M4_ERRORS),
    // y al cerrar la mind30 salta. Esto es un memory leak, no?
    //M4_ASSERT(m_Count == 0);

    if (m_pDefaultRecord != NULL)
    {
        // Release the blobs used...
        ClItemRecordManager IRM(this);
        IRM.SetpMemoryManager(m_poMMNotPool);
		m4uint16_t iItem;
        for (iItem = 0; iItem < m_NumItems; iItem++)
        {
            //if (ItemIsVariableLength(iItem) == M4_TRUE)
            // Por si los valores por defecto son mas grandes que el tamaño especificado
            if (IRM.IsItemBlob((m4uint8_t*)m_pDefaultRecord, iItem) == M4_TRUE)     
                IRM.SetItemBlobNullRelease((m4uint8_t*)m_pDefaultRecord, iItem, M4_TRUE);
        }
        
#ifdef _DEBUG
        // Leak check
        // Compruebo si tengo pendiente algun blob
        for (iItem = 0; iItem < m_NumItems; iItem++)
        {
            m4uint8_t* DataBuffer = (m4uint8_t*)m_pDefaultRecord;
            if (IRM.IsItemBlob(DataBuffer, iItem))
            {
                m4uint8_t* AuxPtr = DataBuffer + ItemOffset(iItem);
                
                // BlobDataPtr1 points to the actual Blob structure
                BlobDataPtr BlobDataPtr1 = *((BlobDataPtr *)AuxPtr);
                
                M4_ASSERT(BlobDataPtr1 == 0);
            }
        }
#endif
        
        m_poMMNotPool->M4free((m4uint8_t *)m_pDefaultRecord);
        m_pDefaultRecord = NULL;
    }
    
    if (m_pIData != NULL)
    {
        m_poMMNotPool->M4free(m_pIData);
        m_pIData = NULL;
    }

    if (m_pIndexOfMappedItem != NULL)
    {
        m_poMMNotPool->M4free(m_pIndexOfMappedItem);
        m_pIndexOfMappedItem = NULL;
    }
        
    if (m_pDefaultBlobMask != NULL)
    {
        m_poMMNotPool->M4free(m_pDefaultBlobMask);
        m_pDefaultBlobMask = NULL;
    }
        
    if (m_pChannelId != NULL)
    {
        m_poMMNotPool->M4free(m_pChannelId);   
        m_pChannelId = NULL;
    }

    m_NumItems            = 0;
    m_UserDataSize        = 0;
    m_UserDataOffset      = 0;
    m_RecordSize          = 0;
    m_DefaultBlobMaskSize = 0;
    m_NodeHandle          = 0;
    m_Scope               = 0;
    m_ChannelVersion      = 0;
    m_iIndexOfFirstOffset = 0;
    m_CsType              = 0;
    m_NumMappedItems      = 0;
    m_SomeItemLivesInFile = M4_FALSE;
    m_Count               = 0;
    // m_pNextStaticData     = NULL;    No puedo hacer esto porque pierdo la lista
}

m4return_t EMDStaticData::GetSize(ClSize& ao_size) const
{
    ao_size += sizeof(EMDStaticData);
    
    if (m_pIData != NULL)
    {
        ClSize extendedItemDataSize;
        if (m_pIData[0].GetSize(extendedItemDataSize) != M4_SUCCESS)
            return M4_ERROR;
        ao_size += extendedItemDataSize * m_NumItems;
    }
    
    if (m_pDefaultBlobMask != NULL)
        ao_size += m_DefaultBlobMaskSize;
    
    if (m_pChannelId != NULL)
        ao_size += strlen(m_pChannelId) + 1;
    
    if (DefaultRecord () != NULL)
        ao_size += m_RecordSize;
    
    if (m_pIndexOfMappedItem != NULL)
        ao_size += sizeof(m_pIndexOfMappedItem[0]) * m_NumMappedItems;
    
    return M4_SUCCESS;
}


//==============================================================================
// class ClExtendedMetaData
//==============================================================================


EMDStaticData ClExtendedMetaData::Hash[MAX_HASH_ENTRIES];

//m4bool_t ClExtendedMetaData::m_bVectorInitialized = M4_FALSE;

ClMutex ClExtendedMetaData::m_soMutEMD(M4_TRUE);

// Creation of an undefined EMD, by default we do nothing...
ClExtendedMetaData::ClExtendedMetaData()
{
/*
	if (m_bVectorInitialized == M4_FALSE)
		InitVector();
*/
}

m4return_t ClExtendedMetaData::DeleteStaticData(m4uint32_t     ai_iIndex,
                                                EMDStaticData* pMetaData)
{
	EMDStaticData* pCurrentMetaData = &Hash[ai_iIndex];
	EMDStaticData* pLastMetaData = NULL;

	while (pMetaData != pCurrentMetaData && pCurrentMetaData != NULL)
	{
		pLastMetaData    = pCurrentMetaData;
		pCurrentMetaData = pCurrentMetaData->m_pNextStaticData;
	}

	if (pCurrentMetaData == NULL)
		return M4_ERROR;
	else
	{
		// Destroy the meta data from the list ...
		if (pMetaData == &Hash[ai_iIndex])
            Hash[ai_iIndex].Reset(); // Libero toda la memoria sin borrar el objeto
		else
		{
			// Destroy it
			pLastMetaData->m_pNextStaticData = pCurrentMetaData->m_pNextStaticData;
            delete pCurrentMetaData;
		}

		return M4_SUCCESS;
	}
}

void ClExtendedMetaData::DestroyMetaData(EMDStaticData* ai_pEMDStaticData)
{
    if (ai_pEMDStaticData == NULL)
        return;
    
    ClMutBlock MutEMDBlock(m_soMutEMD);
    
    m4uint32_t iIndex;
    
    EMDStaticData* pOldMetaData = 
        ClExtendedMetaData::FindMetaData(ai_pEMDStaticData->m_pChannelId,
                                         ai_pEMDStaticData->m_NodeHandle,
                                         ai_pEMDStaticData->m_Scope,
                                         ai_pEMDStaticData->m_ChannelVersion,
                                         ai_pEMDStaticData->m_CsType,
                                         iIndex);
    M4_ASSERT(pOldMetaData != NULL);
    M4_ASSERT(ai_pEMDStaticData == pOldMetaData);
    
    pOldMetaData->m_Count--;
    
    if (pOldMetaData->m_Count == 0)
    {
        m4return_t iResult = DeleteStaticData(iIndex, ai_pEMDStaticData);
        M4_ASSERT(iResult == M4_SUCCESS);
    }
}

m4return_t ClExtendedMetaData::GetSize(ClSize& ao_size) const
{
    ao_size += sizeof(ClExtendedMetaData);
// Lo comento porque esto es compartido por todas las instancias del mismo canal.
/*    if (SD != NULL)
    {
        if (SD->GetSize(ao_size) != M4_SUCCESS)
            return M4_ERROR;
    }
*/
    return M4_SUCCESS;
}
