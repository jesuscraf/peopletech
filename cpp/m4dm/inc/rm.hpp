#ifndef __RM__HPP__
#define __RM__HPP__

//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                rm.hpp
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

#ifndef _M4CHCOMMON_HPP_
#include "m4chcommon.hpp"
#endif

#include "dstypes.hpp"
#include "dm_types.hpp"
#include "sizer.hpp"


class ClRegister_Pipe;     // By Pablo
class ClGenericIODriver;
class ClExtendedMetaData;
class EMDStaticData;
class ClChannelProcess;
class ClDsItemBlob;
class ClVectorRecordBase;
class _ClMemoryManager;
class ClCsTraceSize;


// Pointer to a item of a record
typedef void *p_item_t;

// This is the struct pointed by a blob item. It holds the size of the actual
// data, and the data in a contiguous block of dynamically allocated memory.

// Alineamiento a 1 byte de la estructura
//#ifndef _aCC		// En aCC HP-UX, no valen los pragmas
//#pragma pack(1)
//#endif

class BlobData
{
public:
	m4uint32_t Size;
	
	// To make the next field "Data" aligned in a 8's multiple address. It depends of platform and compiler settings. Be carefull !!!
	// In Win32  :we have compiler settings with "struct member aligment = 8 bytes", then we don't need this filler.
	// In Solaris:we have compiler settings with "struct member aligment = 4 bytes", then we need this filler.
#ifdef _UNIX
	//#ifdef _M4SOLSPARC	//All Unix actually (but it depends, you must study every case, and decide if you need it or not.
	m4uint32_t Filler;	//Not used, only filler
	//#endif //_M4SOLSPARC
#endif //_UNIX
	
	m4uint8_t  Data[1];
	
	// UsedMemory
	m4return_t GetSize(ClSize& ao_size) const
	{
		ao_size += Size + sizeof(BlobData) - 1;
		return M4_SUCCESS;
	}	
};

typedef BlobData *BlobDataPtr;


// All the possible states of a record
const RegisterStatus_t M4_RS_NORMAL               = 0;
const RegisterStatus_t M4_RS_MODIFIED             = 1;
const RegisterStatus_t M4_RS_DELETED              = 2;
const RegisterStatus_t M4_RS_NEW                  = 3;
const RegisterStatus_t M4_RS_MODIFIED_AND_DELETED = 4;
const RegisterStatus_t M4_RS_NEW_AND_DELETED      = 5;
const RegisterStatus_t M4_RS_INVALID              = 6;

const RegisterStatus_t M4_RS_MASK                 = 7;		// Only 8 states allowed

// Flags del registro

// El estado del registro es new, pero no hay que persistirlo
// Es responabilidad del usuario ponerlo a M4_TRUE
// En el VRB se pone a M4_FALSE al llamar a las siguientes funciones miembro
//		InsertRecordsLL() : se inicializa el registro
//		SetItemValueByPosAndOffset()
//		SetItemSizeByPos()
//		CommitRecord
//		_SetDeleteRecordFlag 
const RegisterStatus_t M4_RF_NEW_NOT_PERSIST      = 8;		

const RegisterStatus_t M4_RF_IN_DB                = 16;		// Flag indicating that the record has to be persisted
const RegisterStatus_t M4_RF_PERSIST_LDB          = 32;		// Flag para marcar los registros persistidos
const RegisterStatus_t M4_RF_NEED_SERIAL          = 64;		// Indica que hay que serializar el registro
const RegisterStatus_t M4_RF_BRANCH_MODE_SERIAL   = 128;	// Indica que hay que activar el modo branch de serializacion

  
// This class uses a block of memory as the record, and manages the access to
// the internal items. It also manages the blob fields...
class ClItemRecordManager
{
public:
	// Uses a pointer to the metadata and other to the record buffer
	ClItemRecordManager(const EMDStaticData* const ai_pSMData);
	   
	// Changes the metadata
    void SetMetaData(const EMDStaticData* const ai_pSMData);
	   
	void SetpMemoryManager(_ClMemoryManager* const ai_poMemoryManager)
	{
		m_poMemoryManager = ai_poMemoryManager;
	}
	   
	_ClMemoryManager * GetpMemoryManager(void)
	{
		return m_poMemoryManager;
	}
	   
	// Initializates the internal values of a new record
	// The method assumes that the buffer is initialized to the default values...
	// We only support default values not blobs...
	void InitRecord(m4uint8_t* const DataBuffer); 
	
	// Initializates the internal values of a new record
	// The method assumes that the buffer is filled with 0's (NULLS)
	void InitRecordNoDefaultValues(m4uint8_t* const DataBuffer);
	
	// Llama a los constructores de los BlobFile, si los hubiera
	void CreateAllItemBlobFile(m4uint8_t* const          DataBuffer,
							   ClVectorRecordBase* const ai_pVRB);
	
	// Llama a los destructores de los BlobFile, si los hubiera
	void DestroyAllItemBlobFile(m4uint8_t* const DataBuffer);
	
	m4return_t SetItemValue(m4uint8_t* const  DataBuffer,
							const m4uint16_t  ai_ItemPos,
							const void* const ai_Item,
							const m4uint32_t  ai_Size = 0,
							m4uint32_t* const ao_pRetSize = NULL);
	
	m4return_t SetItemValueNoRelease(m4uint8_t* const  DataBuffer,
									 const m4uint16_t  ai_ItemPos,
									 const void* const ai_Item,
									 const m4uint32_t  ai_Size = 0,
									 m4uint32_t* const ao_pRetSize = NULL);
	
	m4return_t SetItemValueByOffset(m4uint8_t* const  DataBuffer,
									const m4uint16_t  ai_ItemPos,
									const void* const ai_Item,
									const m4uint32_t  ai_Offset,
									const m4uint32_t  ai_Size,
									m4uint32_t* const ao_pRetSize = NULL);
	
	m4return_t SetItemValueByOffsetNoRelease(m4uint8_t* const  DataBuffer,
											 const m4uint16_t  ai_ItemPos,
											 const void* const ai_Item,
											 const m4uint32_t  ai_Offset,
											 const m4uint32_t  ai_Size,
											 m4uint32_t* const ao_pRetSize = NULL);
	
	m4return_t GetItemValue(m4uint8_t* const DataBuffer,
							const m4uint16_t ai_ItemPos,
							void*            ao_Item,
							m4uint32_t&      ao_RealSize,
							const m4uint32_t ai_MaxSize,
							m4bool_t&        ao_IsNull) const;
	
	m4return_t GetItemValueByOffset(m4uint8_t* const DataBuffer,
									const m4uint16_t ai_ItemPos,
									void*            ao_Item,
									const m4uint32_t ai_Offset,
									m4uint32_t&      ao_RealSize,
									const m4uint32_t ai_MaxSize,
									m4bool_t&        ao_IsNull) const;
	
	// If NULL, 0
	m4return_t GetItemSize(m4uint8_t* const DataBuffer,
						   const m4uint16_t ai_ItemPos,
						   m4uint32_t&      ao_Size) const;
	
	m4return_t SetItemSize(m4uint8_t* const DataBuffer,
						   const m4uint16_t ai_ItemPos,
						   const m4uint32_t ai_NewSize,
						   const m4bool_t   ai_bRelease);
	
	// Only complete reads
	void GetUserDataByOffset(const m4uint8_t* const DataBuffer,
							 void*            const ao_UserData,
							 const m4uint16_t       ai_Offset,
							 const m4uint16_t       ai_Size) const; 
	
	// It only permits complete writes
	void SetUserDataByOffset(m4uint8_t*       const DataBuffer,
							 const void*      const ai_UserData,
							 const m4uint16_t       ai_Offset,
							 const m4uint16_t       ai_Size); 

	// Sets the item as null, or not null
	m4return_t SetItemNull(m4uint8_t* const DataBuffer,
						   const m4uint16_t ai_ItemPos,
						   const m4bool_t   ai_IsNull = M4_TRUE);
	
	// Gets if the item as null, or not null
	m4bool_t GetItemNull(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos) const;
	
	// If NULL, returns NULL
	m4return_t GetItemDataAddress(m4uint8_t* const DataBuffer,
								  const m4uint16_t ai_ItemPos,
								  p_item_t&        ao_ItemAddress) const; 
	
	void SetBlobState(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos);
	
	RegisterStatus_t GetStatus(m4uint8_t* const DataBuffer) const; 
	void             SetStatus(m4uint8_t* const DataBuffer, RegisterStatus_t ai_NewStatus); 
	
	m4bool_t IsValid(void* DataBuffer) const;
	
	m4bool_t GetModifiedInDB(m4uint8_t* const DataBuffer) const;
	void     SetModifiedInDB(m4uint8_t* const DataBuffer, const m4bool_t ai_bIsModified);
	
	m4bool_t GetPersistLDB(m4uint8_t* const DataBuffer) const;
	void     SetPersistLDB(m4uint8_t* const DataBuffer, const m4bool_t ai_bIsPersisted);
	
	regid_t GetRecordID(m4uint8_t* const DataBuffer) const;
	void    SetRecordID(m4uint8_t* const DataBuffer, const regid_t ai_iRecordID);

	m4bool_t GetNewNotPersist(m4uint8_t* const DataBuffer) const;
	void     SetNewNotPersist(m4uint8_t* const DataBuffer, const m4bool_t ai_bNewNotPersist);
	
	// Phisical record size
	m4uint32_t GetRecordSize(void) const;
	
	// Frees the memory used by the blobs of a record
	void DeleteBlobsRecord(m4uint8_t* const DataBuffer, const m4bool_t ai_bOnlyMappedItems = M4_FALSE);
	
	m4bool_t IsItemBlob(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos) const;
	
	// The new version (7/1/98) of the slices status is mantained by the user...
	m4bool_t ItemHasSlices(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos) const;
	
	void SetItemHasSlices(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos, const m4bool_t   ai_iHasSlices);
	
	// Priority of the item (0..3)
	m4char_t GetItemPriority(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos) const;
	
	void SetItemPriority(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos, const m4char_t ai_iPriority);

	m4bool_t GetItemIsUser(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos) const;

	void SetItemIsUser(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos, const m4bool_t ai_bIsUser);
	
	// If NULL, returns NULL
	m4return_t GetItemBlobAddress(m4uint8_t* const DataBuffer,
								  const m4uint16_t ai_ItemPos,
								  BlobDataPtr&     ao_BlobAddress) const; 
	m4return_t SetItemBlobNullRelease(m4uint8_t* const DataBuffer,
									  const m4uint16_t ai_ItemPos,
									  const m4bool_t   ai_IsNull);
	m4return_t SetItemBlobNullNoRelease(m4uint8_t* const DataBuffer,
										const m4uint16_t ai_ItemPos,
										const m4bool_t   ai_IsNull);
	m4return_t SetItemNoBlobNull(m4uint8_t* const DataBuffer,
								 const m4uint16_t ai_ItemPos,
								 const m4bool_t   ai_IsNull);
	
	m4return_t SetLotOfItems(m4uint8_t*          const DataBuffer,
							 ClVectorRecordBase* const ai_pVRB,
							 ClRegister_Pipe*          ai_pRegPipe,
							 const m4bool_t            ai_bAuxLoad = M4_FALSE);
	
	// Serializacion >>>>>>>>>>>>>>>>>>>>>
	m4return_t Serialize(ClGenericIODriver&      IOD,
						 const m4uint8_t         ai_iIteSerMod,
						 ClChannelProcess* const ai_pChannelProcess,
						 ClCsTraceSize*    const ai_pCsTraceSize,
						 m4uint8_t*        const DataBuffer);
	
	m4return_t DeSerialize(ClGenericIODriver&   IOD,
						   const m4uint8_t      ai_iIteSerMod,
						   m4uint8_t*     const DataBuffer,
						   ClCsTraceSize* const ai_pCsTraceSize);
	
	// Serializacion avanzada: nos permite marcar concretamente que es lo que se va a a serializar
	void GetNeedSerial(m4uint8_t* const DataBuffer,
					   m4bool_t& ai_bNeedSerial,
					   m4bool_t& ai_bBranchMode) const;
	
	void SetNeedSerial(m4uint8_t* const DataBuffer,
					   const m4bool_t ai_iNeedSerial,
					   const m4bool_t ai_bBranchMode = M4_FALSE);
	
	void TestAndSetNeedSerial(m4uint8_t* const DataBuffer,
							  m4bool_t&        ao_bBeforeNeedSerial,
							  m4bool_t&        ao_bBeforeBranchMode,
							  const m4bool_t   ai_bNeedSerial,
							  const m4bool_t   ai_bBranchMode = M4_FALSE);
	// Serializacion <<<<<<<<<<<<<<<<<<<<<
	
	// UsedMemory
	m4return_t GetSize(ClSize& ao_size) const;
	
	// Luis 11/1/99
	const EMDStaticData* const GetpEMDStaticData(void) const;
	
	void SetRecordVersion(m4uint8_t* const DataBuffer, const VersionMark ai_Version);
	   
protected:
	// Sets the item as null, or not null
	void SetItemNullLL(m4uint8_t* const DataBuffer,
					   const m4uint16_t ai_ItemPos,
					   const m4bool_t   ai_IsNull);
	   
private:
	void CopyGroupOfMasks(m4uint8_t*   const ai_pSourceDataBuffer,
						  m4uint8_t*         ao_pDestinationDataBuffer,
						  m4uint16_t   const ai_iItemFrom,
						  m4uint16_t   const ai_iItemTo);
	
	void CleanupDeserialError(m4uint16_t const ai_iItem,
							  m4uint8_t* const ai_pOldDataBuffer,
							  m4uint8_t* const ai_pDataBuffer);
	
	void SetNoBlobState(m4uint8_t* const DataBuffer, const m4uint16_t ai_ItemPos);
	
	m4bool_t _GetFlag(m4uint8_t* const DataBuffer, const m4uint16_t iFlag) const;
	
	void _SetFlag(m4uint8_t* const DataBuffer,
				  const m4uint16_t ai_iFlag,
				  const m4bool_t   ai_bIsModified);
	
	void CopyBlobsFromRecord(m4uint8_t* const DataBuffer, void* ai_pFromRecord);
	
	m4return_t SetItemBlobNull(m4uint8_t* const DataBuffer,
							   const m4uint16_t ai_ItemPos,
							   const m4bool_t   ai_Release,
							   const m4bool_t   ai_IsNull);
	
	m4return_t SetItemValueByOffsetLL(m4uint8_t* const  DataBuffer,
									  const m4uint16_t  ai_ItemPos,
									  const void* const ai_Item,
									  const m4uint32_t  ai_Offset,
									  const m4uint32_t  ai_Size,
									  const m4bool_t    ai_ReleaseBlobMemory,
									  m4uint32_t* const ao_pRetSize = NULL);
	
	m4return_t _SerializeMasks  (m4uint8_t* const DataBuffer, ClGenericIODriver& IOD);
	m4return_t _DeSerializeMasks(m4uint8_t* const DataBuffer, ClGenericIODriver& IOD);
	
	// Data members
	const EMDStaticData* m_pSMData;
	//Gestor de Memoria
	_ClMemoryManager * m_poMemoryManager;
	
	friend class ClDsItemBlob;
};

 
#ifdef _M4_USING_INLINE
#include "rm.inl"
#endif


#endif 
 
