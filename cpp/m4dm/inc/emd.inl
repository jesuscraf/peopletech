#ifndef __EMD_INL__
#define __EMD_INL__

#include <stdio.h>

#include "dstypes.hpp"
#include "m4thread.hpp"

M4_INLINE int EMDStaticData::operator==(EMDStaticData& ai_OtherStaticMetaData)
{
	if (m_NodeHandle != ai_OtherStaticMetaData.m_NodeHandle)
		return 0;
    
	if (m_Scope != ai_OtherStaticMetaData.m_Scope)
		return 0;
    
	if (m_CsType != ai_OtherStaticMetaData.m_CsType)
		return 0;

	if (m_ChannelVersion != ai_OtherStaticMetaData.m_ChannelVersion)
		return 0;

	if (strcmp(m_pChannelId, ai_OtherStaticMetaData.m_pChannelId) != 0)
		return 0;
    
	return 1;
}

M4_INLINE m4bool_t EMDStaticData::ItemIsPersisted(const m4uint16_t ai_ItemPos) const
{
	return m_pIData[ai_ItemPos].IsPersisted;
}

M4_INLINE m4bool_t EMDStaticData::ItemLivesInFile(const m4uint16_t ai_ItemPos) const
{
	return m_pIData[ai_ItemPos].LivesInFile;
}

M4_INLINE m4uint32_t EMDStaticData::ItemOffset(const m4uint16_t ai_ItemPos) const
{
	return m_pIData[ai_ItemPos].Offset;
}

M4_INLINE m4uint32_t EMDStaticData::ItemMaxSize(const m4uint16_t ai_ItemPos) const
{
	return m_pIData[ai_ItemPos].MaxSize;
}

/* Alex 25/3/99
M4_INLINE m4int32_t ClExtendedMetaData::ItemPrecision(const m4uint16_t ai_ItemPos) const
{
	return SD->IData[ai_ItemPos].Precision;
}
*/

M4_INLINE m4uint8_t EMDStaticData::ItemCppType(const m4uint16_t ai_ItemPos) const
{
	return m_pIData[ai_ItemPos].CppType;
}

M4_INLINE m4uint32_t EMDStaticData::ItemHandle(const m4uint16_t ai_ItemPos) const
{
	return m_pIData[ai_ItemPos].Handle;
}

//EDU------------>>
// devuelve el IsVariableLength
M4_INLINE m4uint32_t EMDStaticData::ItemIsVariableLength(const m4uint16_t ai_ItemPos) const
{
	return m_pIData[ai_ItemPos].IsVariableLength;
}
//EDU------------<<

M4_INLINE m4uint16_t EMDStaticData::BlobMaskSize(void) const
{
	return m_DefaultBlobMaskSize;
}

M4_INLINE m4uint32_t EMDStaticData::RecordSize(void) const
{
	return m_RecordSize;
}

M4_INLINE m4uint8_t* EMDStaticData::BlobMask(void) const
{
	return m_pDefaultBlobMask;
} 

/*
M4_INLINE void EMDStaticData::SetUserDataSize(const m4uint16_t ai_UserDataSize)
{
    ClMutBlock MutEMDBlock(&ClExtendedMetaData::m_soMutEMD);
    
    m4uint32_t NewHeaderSize;
    m4uint32_t OldHeaderSize;
    
    // Old size of the header
    OldHeaderSize =
        M4_ROUND_TO_8(M4CL_EMD_NUMBER_OF_MASKS * m_DefaultBlobMaskSize +
        sizeof(RegisterStatus_t)) +
        m_UserDataSize;

    // New size should be aligned too...
    NewHeaderSize =
        M4_ROUND_TO_8(M4CL_EMD_NUMBER_OF_MASKS * m_DefaultBlobMaskSize +
        sizeof(RegisterStatus_t)) +
        M4_ROUND_TO_8(ai_UserDataSize);
    
    if (NewHeaderSize != OldHeaderSize)
    {
        for (m4uint16_t i = 0; i < m_NumItems; i++)
            m_pIData[i].Offset += NewHeaderSize - OldHeaderSize;
        
        // Change the default value of the record
        m4uint32_t NewRecordSize = m_RecordSize + (NewHeaderSize - OldHeaderSize);
        
        if (m_pDefaultRecord != NULL)
        {
            m4char_t* NewDefaultRecord = (m4char_t*)calloc(1, NewRecordSize);
            
            // Copy of default blobmask, null mask status, slices and priorities
            memcpy(NewDefaultRecord,
                m_pDefaultRecord,
                m_DefaultBlobMaskSize * M4CL_EMD_NUMBER_OF_MASKS + sizeof(RegisterStatus_t));
            
            // Default item values
            memcpy(&NewDefaultRecord[NewHeaderSize],
                &m_pDefaultRecord[OldHeaderSize],
                m_RecordSize - OldHeaderSize);
            
            free(m_pDefaultRecord);
            
            m_pDefaultRecord = NewDefaultRecord;
        }
        
        m_RecordSize = NewRecordSize;
    }
    
    m_UserDataSize = M4_ROUND_TO_8(ai_UserDataSize);
}
*/

M4_INLINE m4uint16_t EMDStaticData::UDataSize(void) const
{
   return m_UserDataSize;
}

M4_INLINE m4uint16_t EMDStaticData::UDataOffset(void) const
{
   return m_UserDataOffset;
}

M4_INLINE m4uint16_t EMDStaticData::NItems(void) const
{
   return m_NumItems;
}

M4_INLINE m4uint16_t EMDStaticData::NMappedItems(void) const
{
   return m_NumMappedItems;
}

M4_INLINE m4char_t* EMDStaticData::DefaultRecord(void) const
{
   return m_pDefaultRecord;
}

/*
M4_INLINE void ClExtendedMetaData::InitVector(void)
{
    ClMutBlock MutEMDBlock(&m_soMutEMD);
    
    for (m4uint32_t index=0; index < MAX_HASH_ENTRIES; index++)
    {
        Hash[index].m_Count = 0;
        Hash[index].m_pNextStaticData = NULL;	
    }
    
    m_bVectorInitialized = M4_TRUE;
}
*/

M4_INLINE EMDStaticData* ClExtendedMetaData::FindMetaData(m4char_t*        ai_ChannelId,
                                                          const m4uint32_t ai_iNodeHandle,
                                                          const m4uint8_t  ai_iScope,
                                                          const m4date_t   ai_dChannelVersion,
                                                          const m4uint8_t  ai_iCsType,
                                                          m4uint32_t&      ao_iIndex)
{
    // Find the index in the hash table: we assume that the scope is less than 4
    M4_ASSERT(ai_iScope < 4);
    
    m4char_t*  ps1 = ai_ChannelId;
    m4uint32_t iThreadId = 0;
    
#ifdef _DEBUG
    m4uint32_t iLong=0;
    while ((*ps1!='&') && (*ps1!=0) && (iLong<20))
    {
        if ((*ps1>='0') && (*ps1<='9'))
            iThreadId=(iThreadId << 4) +(*ps1)-'0';
        else
        {
            if ((*ps1>='a') && (*ps1<='f'))
                iThreadId=(iThreadId << 4) +(*ps1)-'a'+10;
            else
                M4_ASSERT(0);
        }
        
        /*
        *(ps2++)=*(ps1++);
        */
        iLong++;
        
        ps1++;
    }
    
    M4_ASSERT((iLong<20) && (*ps1!=0));
    
    //	*ps2=0;
#else
    while (*ps1!='&')
    {
        if ((*ps1>='0') && (*ps1<='9'))
            iThreadId=(iThreadId << 4) +(*ps1)-'0';
        else
            iThreadId=(iThreadId << 4) +(*ps1)-'a'+10;
        
        ps1++;
    }
    
    //  *(ps2++)=*(ps1++);
    //	*ps2=0;
#endif
    
    ao_iIndex = (iThreadId + ((ai_iNodeHandle & (~0x00000003) | ai_iScope))) & (MAX_HASH_ENTRIES - 1);
    
    EMDStaticData* pCurrentStaticData = &Hash[ao_iIndex]; 
    
    if (pCurrentStaticData->m_Count == 0)
    {
        if (pCurrentStaticData->m_pNextStaticData == NULL)
            pCurrentStaticData = NULL;
        else
            pCurrentStaticData = pCurrentStaticData->m_pNextStaticData;
    }
    
    for (; pCurrentStaticData != NULL; pCurrentStaticData = pCurrentStaticData->m_pNextStaticData)
    {
        if (pCurrentStaticData->m_CsType != ai_iCsType)
            continue;
        
        if (pCurrentStaticData->m_ChannelVersion != ai_dChannelVersion)
            continue;
        
        if (pCurrentStaticData->m_NodeHandle != ai_iNodeHandle)
            continue;
        
        if (pCurrentStaticData->m_Scope != ai_iScope)
            continue;

        if (strcmp(pCurrentStaticData->m_pChannelId, ai_ChannelId) != 0)
            continue;

        break;
    }

    return pCurrentStaticData;
}

M4_INLINE EMDStaticData* ClExtendedMetaData::FindOrInsertMetaData(m4char_t*        ai_ChannelId,
                                                                  const m4uint32_t ai_iNodeHandle,
                                                                  const m4uint8_t  ai_iScope,
                                                                  const m4date_t   ai_dChannelVersion,
                                                                  const m4uint8_t  ai_iCsType)
{
    m4uint32_t iIndex;
    
    EMDStaticData* pStaticMetaData =
        FindMetaData(ai_ChannelId, ai_iNodeHandle, ai_iScope, ai_dChannelVersion, ai_iCsType, iIndex);

    if (pStaticMetaData == NULL)
    {
        // Not Found, create a new one if needed ...
        if (Hash[iIndex].m_Count > 0)
        {
            // The first one in the list is used...
            pStaticMetaData = new EMDStaticData;

            if (pStaticMetaData != NULL)
            {
                // put it on the list ...
                pStaticMetaData->m_Count = 0;
                
                // Put it on the current list in the second position...
                EMDStaticData* pAuxStaticMetaData = Hash[iIndex].m_pNextStaticData;
                Hash[iIndex].m_pNextStaticData = pStaticMetaData;
                pStaticMetaData->m_pNextStaticData = pAuxStaticMetaData;
            }
        }
        else
        {
            // The first one in the list is free
            pStaticMetaData = &Hash[iIndex];
        }
    }

    if (pStaticMetaData != NULL)
        pStaticMetaData->m_Count++;

    return pStaticMetaData;
}

M4_INLINE m4char_t* EMDStaticData::ChannelId(void) const
{
    if (m_pChannelId != NULL)
        return m_pChannelId;
    else
        return "";
}

M4_INLINE m4uint32_t EMDStaticData::NodeHandle(void) const
{
	return m_NodeHandle;
}

M4_INLINE m4uint16_t EMDStaticData::GetIndexOfMappedItem(const m4uint16_t ai_mappedItem) const
{
   M4_ASSERT (ai_mappedItem < m_NumMappedItems);

   return m_pIndexOfMappedItem[ai_mappedItem];
}

M4_INLINE m4bool_t EMDStaticData::SomeItemLivesInFile(void) const
{
	return m_SomeItemLivesInFile;
}

M4_INLINE m4int16_t EMDStaticData::IndexOfFirstOffset(void) const
{
	return m_iIndexOfFirstOffset; 
}
 
M4_INLINE m4uint8_t EMDStaticData::Scope(void) const
{
    return m_Scope;
}

M4_INLINE m4uint8_t EMDStaticData::CsType(void) const
{
    return m_CsType;
}

#endif
