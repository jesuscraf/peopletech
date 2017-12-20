//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                vrb.hpp
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
//    Definitions used in the Generic Data Set module
//
// 11/07/97
//==============================================================================

#ifndef __VRB__HPP__
#define __VRB__HPP__


#include "m4dm_dll.hpp"
#include "m4chcommon.hpp"
#include "dm_types.hpp"
#include "rm.hpp"
#include "dm.hpp"
#include "changelistener.hpp"


class ClBlock_Channel_Proxy;
class ClGenericIODriver;
class ClVVectorDelta;
class ClVectorDelta;
class ClDeltaInfo;
class ClDsItemBlobFile;
class ClSize;
class EMDStaticData;
class ClCsTraceSize;


// Definimos un nuevo tipo de error
//#define M4_ERROR   -1
//#define M4_WARNING  1
//#define M4_SUCCESS  0
#define M4_VRB_ERROR_MAX_COUNT -2
#define M4_VRB_ERROR_ROWS_QUOTA_EXCEEDED -3


// Status flags at the vector level
const m4uint8_t VS_NORMAL         = 0;
const m4uint8_t VS_MODIFIED       = 1;
const m4uint8_t VS_DELETED        = 2;
const m4uint8_t VS_NEW            = 4;
const m4uint8_t VS_MODIFIED_IN_DB = 8;

class _ClMemoryManager;




class M4_DECL_M4DM ClVectorRecordBase : public ClChangeListener
{
public:

	static const regindex_t MAX_COUNT_ALLOW;

	// Standard constructor, the user can optionally set the allocation policy.
	// The user has to assign the structure of the data in the table as the
	// definition on a node in a metachannel
	// To create one void object
	// This constructor shoud be used in case you want to allocate the
	// space for the table later or if you wish to "import" it
	ClVectorRecordBase(void);

	void SetpMemoryManager(_ClMemoryManager* const ai_poMemoryManager)
	{
		m_poMemoryManager = ai_poMemoryManager;
		IRM.SetpMemoryManager(ai_poMemoryManager);
	}
	
	_ClMemoryManager * GetpMemoryManager(void)
	{
		return m_poMemoryManager;
	}
	
	m4return_t Init(const EMDStaticData* const ai_pSMData,
					const regindex_t           ai_iInitialRecords,
					const regindex_t           ai_iGrowRecords,
					const regindex_t           ai_iInitialLogRecords,
					const regindex_t           ai_iMaxCount);
	
	// Releases used recurses
	virtual ~ClVectorRecordBase();

	// This comment is valid for all the setter/getter methods used:
	// As you can see, the optional last parameter is an integer, which
	// needs to be used when you are working with variable sized items.
	// In the setter funcions, you are specifiyng the size of the first
	// part of the BLOB you are assigning. In the getter functions, you
	// are telling the maximum size of the buffer wich is going to receive
	// the value of the item. In all cases, if you wish to continue the
	// assignation/read of the value of a blob item, you must use the next
	// group of methods (AppendItemValue, GetNextItemValuePart).
	
	// The following setter/getter funcions use the "position" of the
	// item in the item list, except for the ones wich are not used
	
	// Sets the item as null, or not null
	// If te item is a null blob and you try to make it not null,
	// the result is an error !!!
	m4return_t SetItemNullByPos(const m4uint16_t		ai_ItemPos,
								const regindex_t		ai_Index,
								const m4bool_t			ai_IsNull,
								      m4bool_t			ai_bChangeStatus);
	
	// Gets if the item as null, or not
	m4return_t GetItemNullByPos(const m4uint16_t ai_ItemPos,
								const regindex_t ai_Index,
								m4bool_t&        ao_IsNull) const;
	
	// Gets if the last version of an item was null, or not
	m4return_t GetLastItemNullByPos(const m4uint16_t ai_ItemPos,
									const regindex_t ai_Index,
									m4bool_t&        ao_IsNull) const;
	
	//EDU-------------->>
	m4return_t ItemHasSlices(const m4uint16_t ai_ItemPos,
							 const regindex_t ai_Index,
							 m4bool_t         &ao_HasSlices) const;
	//EDU--------------<<
	
	m4return_t LastItemHasSlices(const m4uint16_t ai_ItemPos,
								 const regindex_t ai_Index,
								 m4bool_t         &ao_HasSlices) const;
	// (NEW 9/1/98)
	m4return_t SetItemHasSlices(const m4uint16_t ai_ItemPos,
								const regindex_t ai_Index,
								const m4bool_t   ai_iHasSlices);

	// Priority of the item (0..3) (NEW 9/1/98)
	m4return_t GetItemPriority(const m4uint16_t ai_ItemPos,
							   const regindex_t ai_Index,
							   m4char_t&        ao_iPriority) const;
	
	// (NEW 9/1/98)
	m4return_t SetItemPriority(const m4uint16_t ai_ItemPos,
							   const regindex_t ai_Index,
							   const m4char_t   ai_iPriority);
	
	m4return_t LastItemPriority(const m4uint16_t ai_ItemPos,
								const regindex_t ai_Index,
								m4char_t&        ao_iPriority) const;

	m4return_t GetItemIsUser(const m4uint16_t ai_ItemPos,
							   const regindex_t ai_Index,
							   m4bool_t&        ao_bIsUser) const;
	
	m4return_t SetItemIsUser(const m4uint16_t ai_ItemPos,
							   const regindex_t ai_Index,
							   const m4bool_t   ai_bIsUser);
	
	// Gets the value of an item of the record (by position).
	// If NULL, returns SUCCESS and sets ao_IsNull to 1 
	m4return_t GetItemValueByPos(const m4uint16_t ai_ItemPos,
								 void*            oi_Item,
								 const regindex_t ai_Index,
								 m4uint32_t&      ao_RealSize,
								 const m4uint32_t ai_MaxSize,
								 m4bool_t&        ao_IsNull) const;

	// Pablo 23/2/99
	// Funciones para los items de tipo Blob.
	
	m4return_t GetItemBlobFileDescription(const m4uint16_t ai_ItemPos,
										  const regindex_t ai_Index,
										  const void* &    ao_ItemAddress) const;
	
	m4return_t SetItemBlobFileDescription(const m4uint16_t  ai_ItemPos,
										  const void* const ai_Item,
										  const regindex_t  ai_Index);
	
	ClDsItemBlobFile* const GetItemBlobFile(const m4uint16_t ai_ItemPos, const regindex_t ai_Index) const;

	// Sets one section of the value of an item of the record (by position).
	// If NULL, returns SUCCESS and sets ao_IsNull to 1 
	m4return_t GetItemValueByPosAndOffset(const m4uint16_t ai_ItemPos,
										  void*            oi_Item,
										  const regindex_t ai_Index,
										  const m4uint32_t ai_Offset,
										  m4uint32_t&      ao_RealSize,
										  const m4uint32_t ai_MaxSize,
										  m4bool_t&        ao_IsNull) const;
	
	m4return_t SetItemValueByPosAndOffset(const m4uint16_t  ai_ItemPos,
										  const void* const ai_Item,
										  const regindex_t  ai_Index,
										  const m4uint32_t  ai_Offset,
										  const m4uint32_t  ai_Size,
										  m4uint32_t* const ao_pRetSize,
										  m4bool_t			ai_bChangeStatus,
										  m4bool_t			ai_bIsUser);
	
	// Nueva funcion para la load, ojo que ahora tambien para la autoload (cuidadin con los deltas)
	m4return_t AddRecordAndSetLDBItems(regindex_t&      ai_Index,
									   ClRegister_Pipe* ai_pRegPipe,
									   const m4bool_t   ai_bAddRegister,
									   const regid_t    ai_iRecordID,
									   const m4bool_t   ai_bAuxLoad);
	
	// If is NULL, is not anymore
	m4return_t SetItemSizeByPos(const m4uint16_t ai_ItemPos,
								const regindex_t ai_Index,
								const m4uint32_t ai_NewSize);
	
	// Gets the actual size of the item data by position
	// If NULL, returns SUCCESS and sets the ao_Size to 0
	m4return_t GetItemSizeByPos(const m4uint16_t ai_ItemPos,
								const regindex_t ai_Index,
								m4uint32_t       &ao_Size) const;
	
	// Gets the last commited size of the item data by position
	// If NULL, returns SUCCESS and sets the ao_Size to 0
	m4return_t GetLastItemSizeByPos(const m4uint16_t ai_ItemPos,
									const regindex_t ai_Index,
									m4uint32_t       &ao_Size) const;
	
	// Returns the address of an item by position. It works for static
	// and blob data.
	// If NULL, returns SUCCESS and sets the ao_ItemAddress to NULL
	m4return_t GetItemDataAddressByPos(const m4uint16_t ai_ItemPos,
									   const regindex_t ai_Index,
									   p_item_t         &ao_ItemAddress) const;

	//EDU, prueba:
	m4return_t GetItemDataAddressByPos_AndHasSlices(const m4uint16_t ai_ItemPos,
													const regindex_t ai_Index,
													p_item_t&        ao_address,
													m4bool_t&        ao_HasSlices) const;
	
	// Returns the address of the last value of an item by position. 
	// It works for static and blob data.      
	// If NULL, returns SUCCESS and sets the ao_ItemAddress to NULL
	m4return_t GetLastItemDataAddressByPos(const m4uint16_t ai_ItemPos,
										   const regindex_t ai_Index,
										   p_item_t&        ao_ItemAddress) const;
	
	//EDU, prueba:
	m4return_t GetLastItemDataAddressByPos_AndHasSlices(const m4uint16_t ai_ItemPos,
														const regindex_t ai_Index,
														p_item_t&        ao_ItemAddress,
														m4bool_t&        ao_HasSlices) const;
	
	// Only complete reads
	m4return_t GetUserDataByPosAndOffset(void*            ao_UserData,
										 const m4uint16_t ai_Offset,
										 const regindex_t ai_Index,
										 const m4uint16_t ai_Size) const;
	
	// It only permits complete writes
	m4return_t SetUserDataByPosAndOffset(void*            ai_UserData,
										 const m4uint16_t ai_Offset,
										 const regindex_t ai_Index,
										 const m4uint16_t ai_Size);
	
	// Returns the state of the record
	// (RS_NORMAL, RS_MODIFIED, RS_DELETED, RS_NEW, ...)
	RegisterStatus_t RecordStatus(const regindex_t ai_Index) const;
	
	// Tells us if the record was modified
	m4bool_t IsRecordModified(const regindex_t ai_Index) const;
	
	// Tells us if the record was modified in the database (NEW 9/1/98)
	m4bool_t IsRecordModifiedInDB(const regindex_t ai_Index) const;
	
	// Tells us if the record is a new one
	m4bool_t IsRecordNew(const regindex_t ai_Index) const;
	
	// Tells us if the record is logically deleted (only if transactions are used)
	m4bool_t IsRecordDeleted(const regindex_t ai_Index) const;
	
	m4bool_t IsRecordValid(const regindex_t ai_Index) const;
	
	// Returns the state of the vector. This is a mask ot hte following constants
	// (VS_MODIFIED, VS_DELETED, VS_NEW)
	m4return_t Status(m4uint8_t& ao_Status) const;
	
	// Tells us if any record of the vector was modified
	m4bool_t IsModified(void) const;
	
	// Tells us if any record of the vector was modified for the DB (NEW 9/1/98)
	m4bool_t IsModifiedInDB(void) const;
	
	// Tells us if any record of the vector is a new one
	m4bool_t IsNew(void) const;
	
	// Tells us if any record is logically deleted 
	m4bool_t IsDeleted(void) const;
	
	m4bool_t GetRecordPersistLDB(const regindex_t ai_Index) const;
	m4return_t SetRecordPersistLDB(const regindex_t ai_Index, const m4bool_t ai_IsPersisted);
	
	// Defines if we are using checkpoints or not (default yes)
	m4return_t UseCheckPoints(m4bool_t ai_Use = M4_TRUE);
	
	// If there is another transaction flying, is commited, and a ne
	// one is initiated
	m4return_t CheckPoint(void);
	
	// Commits the last transaction
	m4return_t Commit(void);
	
	m4return_t CommitRecord(regindex_t ai_iIndex, m4bool_t& ai_iRecordDeleted);
	
	// Returns the DataBlock to the state of the last checkpoint
	m4return_t Rollback(void);
	
	// Undoes changes made to the actual or specified register
	m4return_t Undo(regindex_t ai_Index);
	
	// Pablo
	// Nueva funcion que recalcula el estado del VRB preguntando a cada uno de su registros
	m4return_t RefreshStatus(void);
	
	// Are we using breakpoionts ??
	m4bool_t UsingCheckPoints(void) const;
	
	void SetUseCPDelta(const m4bool_t ai_bUseCPDelta);
	
	// Serializacion >>>>>>>>>>>>>>>>>>>>>
    /**
	 * Para poder inicializar el uso de deltas de todos los niveles del canal.
	 *
	 * @param ai_bKeepIt		Si hay que mantener deltas o no.
	 * @param ai_iVectorCount	En caso de mantener deltas, cuantos vectores queremos.
	 * @return m4return_t.
	 */
	m4return_t SetKeepStatus(const m4bool_t ai_keepStatus, const m4uint8_t ai_iVectorCount);

	m4bool_t GetKeepStatus(void) const;
	
	// ao_thereIsAnyChange : inidca al nivel superior si se modifico algun registro del VRB, de esta
	// forma sabe si tiene que serializar los registros o no hace falta. Usando absolutos esta valor
	// siempre será M4_TRUE, ya que hay que serializar todos los registros
	m4return_t SerializeStaticPart(ClGenericIODriver& IOD,
								   const m4uint8_t    ai_iRecSerMod,
								   ClChannelProcess*  ai_pChannelProcess,
								   m4bool_t&          ao_thereIsAnyChange,
								   const m4uint8_t    ai_iVectorDelta);
      
	m4return_t SerializeRecord(ClGenericIODriver&      IOD,
							   m4uint8_t&              aio_iRecordSerializationMode,
							   const m4uint8_t         ai_iIteSerMod,
							   ClChannelProcess* const ai_pChannelProcess,
							   ClCsTraceSize*    const ai_pCsTraceSize,
							   const regindex_t        ai_iIndex,
							   const m4uint8_t         ai_iVectorDelta);
	
	// Assigns the satic part of the object, and initalizes it if neccesary
	m4return_t DeSerializeStaticPart(ClGenericIODriver& IOD, m4bool_t& ao_bThereIsAnyChange);

	m4return_t DeSerializeFirstStaticPart(ClGenericIODriver& IOD); 

	m4return_t DeSerializeSecondStaticPart(ClGenericIODriver& IOD, m4bool_t& ao_bThereIsAnyChange);

	m4return_t DeSerializeRecord(ClGenericIODriver&   IOD,
								 const m4uint8_t      ai_iIteSerMod,
								 const regindex_t     ai_iIndex,
								 ClDeltaInfo&         ao_oDeltaInfo,
								 ClCsTraceSize* const ai_pCsTraceSize);
	
	// Indica cuantos deltas hay que deserializar
	regindex_t GetCountDelta(void) const;
	
	// Nos indica si estamos en medio de una deserializacion de deltas
	m4bool_t IsDeSerializingDeltas(void) const;
	
	// Analiza la informacion de deltas y nos devuelve los cambios que tenemos que hacer en el canal para
	// que se pudan empezar a deserializar deltas
	m4return_t GetDeltaInfo(m4uint32_t ao_vMovedIndex[], regindex_t& ao_iNumDeletedIndex, m4bool_t& ao_bArrange);
	
	/**
	 * You must call this function to end the DeSerialize.
	 *
	 * @param ai_iVectorDeltaCount	Número de vectores de deltas que hay que crear despues de la deserialización.
	 * @return m4return_t.
	 */
	m4return_t EndDeSerialize(const m4uint8_t ai_iVectorDeltaCount);
	
	// Serializacion avanzada: nos permite marcar concretamente que es lo que se va a a serializar
	void ResetSerialMarks(void);
	
	m4return_t GetNeedSerial(const regindex_t ai_Index, m4bool_t& ao_bNeedSerial, m4bool_t& ao_bBranchMode) const;
	m4return_t SetNeedSerial(const regindex_t ai_Index, const m4bool_t ai_bNeedSerial, const m4bool_t ai_bBranchMode = M4_FALSE);
	
	m4return_t TestAndSetNeedSerial(const regindex_t ai_Index,
									m4bool_t&        ao_bBeforeNeedSerial,
									m4bool_t&        ao_bBeforeBranchMode,
									const m4bool_t   ai_bNeedSerial,
									const m4bool_t   ai_bBranchMode = M4_FALSE);
	
	m4return_t PrepareAdvanzedSerial(void);
	// Serializacion <<<<<<<<<<<<<<<<<<<<<
	
	// NEW 13/1/98 (EDU): swap used in quicksort.
	void UpdateIndex (m4uint32_t *ai_pindex_qtc);
	
	m4return_t PrepareIndexPL (const m4bool_t ai_bUp, const m4uint32_t ai_PrevReg);
	
	//EDU: prueba, la hago pública (antes era privada:)
	void ResetStatus(void);
	//m4return_t ResetStatusRecord(const regindex_t ai_iIndex);
	//EDU: prueba, nueva función cutre:
	void SetStatus (m4uint8_t ai_status) { State = ai_status; }
	
	// Cambia el estado de un registro, y deja consistente el oldValue
	m4return_t SetStatus(const regindex_t ai_Index, const RegisterStatus_t ai_NewStatus); 
	
	// Luis 11/1/99
	const EMDStaticData* const GetpExtendedMetaData(void); 
	
	regid_t GetRecordID(const regindex_t ai_iIndex) const;
	void    SetRecordID(const regindex_t ai_iIndex, const regid_t    ai_iRecordID);
	
	m4bool_t GetRecordNewNotPersist(const regindex_t ai_iIndex) const;
	void     SetRecordNewNotPersist(const regindex_t ai_iIndex, const m4bool_t ai_bNewNotPersist);
	
	// Esta funcion da la memoria utilizada por un objeto de la clase ClVectorRecordBase
	// (es decir la suma de la memoria usada por los registros, por los blobs en memoria, ...)
	// Es importante no confundirla con lo que sería el tamaño de un registro o el tamaño
	// del vector de registros (un nombre mas apropiado podria ser UsedMemory)
	m4return_t GetSize(ClSize& ao_size) const;
	
	void SetRecordVersion(const regindex_t ai_iIndex, const VersionMark ai_Version);
	
	void FindRecordAndSetVersion(const m4uint32_t ai_iRecordId, const VersionMark ai_iNewVersion, m4bool_t& ao_bContinue);

	/** Devuelve el número de registros usados (no se cuentan los oldVaules). */
	regindex_t GetCount(void) const;

	// Guarantees a minimun size 
	m4return_t ClVectorRecordBase::Reserve(const regindex_t ai_iSize);

	/* Bug 0102510 Para poder poner deltas en carga parcial con serialización */
	void	OnModifiedByPos( const regindex_t ai_Index ) ;

	ClVVectorDelta* ClVectorRecordBase::GetpVVectorDelta( ) ;


protected:
	m4bool_t ExistsOldValue(const regindex_t ai_iIndex) const;
	
	m4return_t DestroyRecordLL(const regindex_t ai_Index);
	m4return_t DestroyOldValue(const regindex_t ai_iIndex);
	m4return_t DestroyOldValueLL(const regindex_t ai_iIndex, const regindex_t ai_iLastIndex);
	
	m4bool_t _ExistsOldValue(const m4uint32_t ai_iIndex, const m4bool_t ai_bUseCP) const;
	
	m4return_t DeleteRecordLL(const regindex_t ai_Index); 
	m4return_t DeleteRecord  (const regindex_t ai_Index); 
	
	// Inserts a number of records, begining in the index specified
	m4return_t InsertRecordsLL(const regindex_t ai_Index,
				   const regid_t    ai_iRecordID,
				   const regindex_t ai_Number,
				   const m4bool_t   ai_iInit = M4_TRUE);

	// Phisical delete of all the records
	void ZapLL(void);
	
	// returns the index of the last value of a record, by it index
	regindex_t GetLastRecordValueIndexByIndex(regindex_t ai_Index) const; 


private:
	// Adds a new record to the transaction log, without copying the original, nor changing
	// any state
	m4return_t AddRecordToTransactionLogWithoutCopy(regindex_t& ao_TLIndex);
	
	// Low level functions are used internally
	m4bool_t IndexFull(const regindex_t ai_Number = 0) const; 
	
	// computes dynamically the number of indexes to grow the vector
	regindex_t ComputeDynamicGrow(const regindex_t ai_Grow);

	// Grows the size of the index, in the area of the used records
	m4return_t GrowIndex(const regindex_t ai_Grow); 
	
	// Makes a copy in the transaction log of the actual value of a
	// record. It returns in ao_TLIndex the index of the new record (from 0)
	m4return_t AddRecordToTransactionLog(const regindex_t ai_Index, regindex_t& ao_TLIndex);

	// Copia el contenido del registro al oldValue. El oldValue ya debe existir
	void CopyRecordToOldValue(const regindex_t ai_iIndex,
							  const regindex_t ai_iLastIndex,
							  const m4bool_t   ai_bCopyMappedBlobFileItems = M4_TRUE);
	
	// Deletes the transaction log record pointed by ao_TransactionIndex
	m4return_t DeleteFromTransactionLog(regindex_t ao_TransactionIndex);
	
	// Sets the pointer to the last value of a modified record,
	// it asssumes that you are possitioned in the record previously
	void SetLastRecordValueIndex(const regindex_t ai_Index1, const regindex_t ai_Index); 
	
	// Gets the index to the last value of a modified record
	regindex_t GetLastRecordValueIndex(const regindex_t ai_iIndex) const; 
	
	// Devuelve el puntero al oldValue
	m4uint8_t* _LastRecordToProcess(const regindex_t ai_iIndex) const;
	
	// Tells us if the blob pointed by two records is the same
	m4return_t SameBlobByIndex(const m4uint16_t ai_ItemPos,
							   const regindex_t ai_Index1,
							   const regindex_t ai_Index2,
							   m4bool_t&        IsTheSame) const;
	
	// Tells us if a blob has changed
	m4return_t BlobChanged(const m4uint16_t ai_ItemPos,
						   const regindex_t ai_Index,
						   m4bool_t&        IsTheSame) const;
	
	// Undoes the modifications in a record, and tells us if it was removed
	m4return_t UndoLL(const regindex_t ai_Index,
					  m4bool_t&        ao_RecordWasDeleted); 
	
	// Frees the memoty used by the static part and the blobs if the
	// record specified and the last version. It is assumed that the
	// record is modified and the last version exists.
	// If ai_deleteStaticPart==M4_TRUE then only delete the blobs
	m4return_t FreeMemoryFromActualAndLast(regindex_t     ai_Index,
										   m4bool_t       ai_deleteStaticPart = M4_TRUE,
										   const m4bool_t ai_bOnlyMappedItems = M4_FALSE);
	
	m4return_t _SetDeleteRecordFlag(const regindex_t ai_Index);
	
	void FreeRecordMemory(const regindex_t ai_Index);
	
	// Para usar desde ClDsItemBlobFile
	regindex_t GetIndexByRecordAddress(const void* const ai_pDataBuffer, m4bool_t& ao_oldValue) const;

	/**
	 * Crea el vector de vectores de deltas. En caso de que este ya exista se limita a volver a
	 * inicializar los vectores de deltas.
	 *
	 * @param ai_iInitialRecords	Número de registros que se quiere que tengan los vectores de deltas.
	 * @param ai_iGrowRecords		En cuantos registros se quiere crecer cuando nos quedemos sin espacio.
	 * @param ai_iVectorDeltaCount	Cuantos vectores de deltas hay que crear o resetear.
	 */
	m4return_t CreateVVectorDelta(const regindex_t ai_iInitialRecords,
								  const regindex_t ai_iGrowRecords,
								  const m4uint8_t  ai_iVectorDeltaCount);

	
	friend class ClDsItemBlobFile;
	
	ClMemPtr            *Index;
	regindex_t          Count;        // Numero de registros normales
	regindex_t          m_iMaxCount;  // Numero máximo de registros normales
	regindex_t          Size;         // Tamaño del vector de punteros a registros
	regindex_t          LogIndex;     // Donde empiezan los oldValues
	regindex_t          LogCount;     // Numero de oldValues
	regindex_t          GrowRecords;  // En cunato crece el VRB cuando se queda sin espacio para mas regsitros
	
	const EMDStaticData* m_pSMData;
	
	ClItemRecordManager IRM;
	
	m4bool_t            UseCP;        // Si usamos transacciones en memoria, o no (M4_TRUE ==> podemos hacer undo)
	m4uint8_t           State;
	
	// Deltas >>>>>>>>>>>>>>>>
	m4bool_t        m_bKeepStatus;     // Indica si tenemos que guardar los cambios de los registros
	ClVVectorDelta* m_pVVectorDelta;   // Contiene la infomación sobre todos los vectores de deltas de cada registro
	m4bool_t        m_bDeSerializingDeltas;
	
	ClVectorDelta*  m_pVAdvancedSerial; // Vector de deltas que se utiliza para serializacion avanzada
	
	regindex_t m_iCountDelta;
	m4bool_t   m_bUseCPDelta;
	m4uint8_t  m_StateDelta;
	// Deltas <<<<<<<<<<<<<<<<
	
	// Gestor de Memoria
	_ClMemoryManager * m_poMemoryManager;
};


#ifdef _M4_USING_INLINE
#include "vrb.inl"
#endif

#endif
