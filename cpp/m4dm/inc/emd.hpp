#ifndef _EMD_HPP_
#define _EMD_HPP_

//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
// 
// Module:
// File:                emd.hpp
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
// 11/07/97
//==============================================================================
#include "m4dm_dll.hpp"
#ifndef _M4CHCOMMON_HPP_
#include "m4chcommon.hpp"
#endif

#include "syncro.hpp"
#include "sizer.hpp"

class ClMetaChannelToExtendedMetaData;
class ClMemoryManagerNotPool;
class _ClMemoryManager;


#define M4CL_EMD_NUMBER_OF_MASKS 6U


// Metadata of each item (generic)
struct ExtendedItemData
{
    m4uint32_t MaxSize;         // Maximum size for fixed sized ...
    // Alex 25/3/99 m4int32_t  Precision;       // Maximum permitted
    m4uint8_t  CppType;
    m4uint32_t Handle;
    m4uint8_t  IsVariableLength;// Is a BLOB ?
    m4bool_t   IsPersisted;
    m4bool_t   LivesInFile;
    m4uint32_t Offset;

    // UsedMemory
    m4return_t GetSize(ClSize& ao_size) const
    {
        ao_size += sizeof(ExtendedItemData);
        return M4_SUCCESS;
    }

};


// The value of MAX_HASH_ENTRIES must be a power of two ! ! !
/*
/#ifdef M4_VM_CLIENT 
   #define MAX_HASH_ENTRIES 256 
#else
   #define MAX_HASH_ENTRIES (8*1024)
#endif
   */
#define MAX_HASH_ENTRIES (8*1024)

class EMDStaticData
{
public:
    EMDStaticData(void);
    ~EMDStaticData(void);

    void Reset(void);

    int operator== (EMDStaticData& ai_OtherStaticMetaData);

    m4uint8_t* BlobMask(void) const;
    m4uint16_t BlobMaskSize(void) const;
    m4uint32_t ItemOffset(const m4uint16_t ai_ItemPos) const;
    m4bool_t   ItemLivesInFile(const m4uint16_t ai_ItemPos) const;
    m4uint32_t ItemIsVariableLength(const m4uint16_t ai_ItemPos) const;
    m4char_t*  DefaultRecord(void) const;
    m4uint16_t NItems(void) const;
    m4int16_t  IndexOfFirstOffset(void) const;
    m4uint32_t RecordSize(void) const;
    m4uint16_t UDataSize(void) const;
    m4uint16_t UDataOffset(void) const;
    m4uint32_t ItemMaxSize(const m4uint16_t ai_ItemPos) const;
    m4uint16_t NMappedItems(void) const;
    m4uint16_t GetIndexOfMappedItem(const m4uint16_t ai_mappedItem) const;
    m4uint32_t ItemHandle(const m4uint16_t ai_ItemPos) const;
    m4uint8_t  ItemCppType(const m4uint16_t ai_ItemPos) const;
    m4bool_t   ItemIsPersisted(const m4uint16_t ai_ItemPos) const;
    //void       SetUserDataSize(const m4uint16_t ai_UserDataSize);
    m4char_t*  ChannelId(void) const;
    m4bool_t   SomeItemLivesInFile(void) const;
    m4uint32_t NodeHandle(void) const;
    m4uint8_t  Scope(void) const;
    m4uint8_t  CsType(void) const;
	
    // UsedMemory
    m4return_t GetSize(ClSize& ao_size) const;

    _ClMemoryManager * GetpMemoryManager(void)
    {
        return m_poMMNotPool;
    }

private:
    m4uint16_t        m_NumItems;
    ExtendedItemData* m_pIData;
    m4uint16_t        m_UserDataSize;
    m4uint16_t        m_UserDataOffset;
    m4uint32_t        m_RecordSize;
    m4uint16_t        m_DefaultBlobMaskSize;
    m4uint8_t*        m_pDefaultBlobMask;
    m4char_t*         m_pChannelId;
    m4uint32_t        m_NodeHandle;
    m4uint8_t         m_Scope;
    m4date_t          m_ChannelVersion;
    m4char_t*         m_pDefaultRecord; // The default value record (used in inserts)...
    m4int16_t         m_iIndexOfFirstOffset;

    // Info about the CsType and the mapped items
    m4uint8_t         m_CsType;
    m4uint16_t        m_NumMappedItems;
    m4uint16_t*       m_pIndexOfMappedItem;
    m4bool_t          m_SomeItemLivesInFile;   // In this EMD there is at least an item that lives in file
    m4uint32_t        m_Count;
    EMDStaticData*    m_pNextStaticData;

    _ClMemoryManager * m_poMMNotPool;

    friend class ClMetaChannelToExtendedMetaData;
    friend class ClExtendedMetaData;
};


/*
typedef struct
{
	EMDStaticData *EMDSD;
} EMDSDCounter;
*/

// Metadata of one record with all the precalculated things
// This is going to be used by all the classes that need optimization for
// speed
#ifdef _DEBUG
class M4_DECL_M4DM ClExtendedMetaData
#else
class ClExtendedMetaData
#endif
{
public:
    static EMDStaticData* FindMetaData(char*            ai_ChannelId,
                                       const m4uint32_t ai_iNodeHandle,
                                       const m4uint8_t  ai_iScope,
                                       const m4date_t   ai_dChannelVersion,
                                       const m4uint8_t  ai_iCsType,
                                       m4uint32_t&      ao_iIndex);
    
    static EMDStaticData* FindOrInsertMetaData(m4char_t*        ai_ChannelId,
                                               const m4uint32_t ai_iNodeHandle,
                                               const m4uint8_t  ai_iScope,
                                               const m4date_t   ai_dChannelVersion,
                                               const m4uint8_t  ai_iCsType);

    static void DestroyMetaData(EMDStaticData* ai_pEMDStaticData);
    
    // Esta funcion da la memoria utilizada por un objeto de la clase ClExtendedMetaData
    // Es importante no confundirla con lo que sería el tamaño de un registro o el tamaño
    // de un item del registro (un nombre mas apropiado podria ser UsedMemory)
    m4return_t GetSize(ClSize& ao_size) const;

    static ClMutex m_soMutEMD;

private:
    // No dejamos que se creen instancias de esta clase
    // El constructor inicializa el vector estatico, pero no hace falta ya que al crear el
    // vector estatico ya se llama al constructor de cada objeto
    ClExtendedMetaData();

    //static void InitVector(void);

    static m4return_t DeleteStaticData(m4uint32_t     ai_iIndex,
                                       EMDStaticData* pMetaData);

    static EMDStaticData Hash[MAX_HASH_ENTRIES];
    //static m4bool_t      m_bVectorInitialized;

    friend class ClMetaChannelToExtendedMetaData;
};


#ifdef _M4_USING_INLINE
#include "emd.inl"
#endif

#endif
