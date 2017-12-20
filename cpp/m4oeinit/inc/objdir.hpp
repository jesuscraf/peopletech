//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                ObjDir.hpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                10/10/97
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Definitions used in the class for storing other objects in serialization
//    
//
//==============================================================================

#ifndef _OBJDIR_HPP_
#define _OBJDIR_HPP_

#include "m4oeinit_dll.hpp"
#include "m4stl.hpp"
#include "cldntfbl.hpp"
#include "index.hpp"
#include "level2relation.hpp"


// Para minimizar referencias circulares
class ClObjectId;
class ClChannelManager;
class ClCompiledMCR;
class ClChannel;
class ClOLTPExecutionContext;
class ClGenericIODriver;
class ClLaneArgumnent;
class ClClientServerResult;
class ClChannelProcess;
class ClCsTraceSize;
class IObjDataCache;

typedef set< m4uint16_t, less< m4uint16_t > >	SetObjDirIndex_t;
typedef SetObjDirIndex_t::iterator				ItSetObjDirIndex_t;



class PtrWithId
{
public:
    PtrWithId(void);
    void Init(m4bool_t ai_bDestroy = M4_TRUE);
    
    void*       m_poPtr;                // Ptr to the object.
    ClObjectId* m_poOId;                // Identification of it
    m4bool_t    m_bCreatedByDirectory;  // The Directory only delete the objects created by it
    m4bool_t    m_bDoSerialize;         // Indica si tengo que serializar el objeto, o no
    m4uint32_t  m_iSerializedSize;      // Tamaño del objeto serializado

    // Solo para canales >>>>>>>>>>>>
    m4uint32_t  m_iRemoteId;            // Id del canal en el servidor (en proxis puede ser distinto)
    m4uint8_t   m_iIsDebug;             // Indica si es una vista de debug de un canal back
    m4uint32_t  m_iBlockToSerialize;    // Bloque a serializar en un root multibloque (carriles)
    m4uint32_t  m_iRegisterToSerialize; // Registro a serializar en un root multibloque (carriles)
    ClChannelProcess* m_pChannelProc;   // Proceso a realizar mientras se serializa el canal
    // Solo para canales <<<<<<<<<<<<

    m4return_t DumpInfo(ostream &ai_oStream);
};

M4_DECL_M4OEINIT ostream & operator << (ostream &ao_Output, PtrWithId &ai_oPtrWithId);


/**
 * Esta clase nos permite agrupar los objetos que queremos mandar del cliente al servidor y viceversa.
 * Se encarga de crear los objetos cuando es necesario, y de engancharlos con los que ya existen.
 */
class M4_DECL_M4OEINIT ClCMObjectDirectory
{
public:
	ClCMObjectDirectory(ClChannelManager* ai_oChannelManager);
	
	m4return_t Reset(ClChannelManager* ai_oChannelManager);
	
	~ClCMObjectDirectory(void);
	
	/**
	 * Añade un canal al objDir y le crea un OID para identificarlo a la vuelta.
	 * Si el canal ya estaba en el objDir no se añade (no se permiten repeticiones).
	 * Podemos especificarle el identificador remoto y si queremos que se serialize a la ida.
	 *
	 * @param ai_pChannel		Canal que queremos añadir.
	 * @param ai_iRemoteId		Identificador remoto del canal (ej para proxy o depuración).
	 * @return Devuelve el índice donde esta el canal dentro del objDir. -1 si no se ha podido añadir.
	 */
	m4int16_t IncludeChannel(ClChannel* const ai_pChannel, m4uint32_t ai_iRemoteId);
	
	m4int16_t IncludeAccessAndCreateOID(ClAccess* const ai_poAccess, m4uint32_t ai_iRemoteId);
	
	m4return_t IncludeAllAccessAndCreateOID(ClChannel& ai_roChannel, m4bool_t ai_bIncludeAlways);
	
	m4int16_t IncludeOLTPExecutionContextAndCreateOID(ClOLTPExecutionContext& ai_roOLTPExecutionContext);
	
	// Insertar los canales amigos para un canal dado en el Object Directory
	m4return_t InsertAllPartnersInObjDir(ClChannel *ai_pChannel);
	
	// Nos devuelve un puntero al objeto que nos permite devolver los errores
	ClClientServerResult* CreateClientServerResult(m4bool_t ai_bForceSerialize=M4_FALSE);
	
	// Crea identificadores validos para los objetos
	static ClObjectId* CreateChannelOId(const ClChannel& ai_pChannel);
	static ClObjectId* CreateMetaChannelOId(const ClCompiledMCR& ai_pCompiledMCR, const ClChannel& ai_pChannel);
	static ClObjectId* CreateAccessOId(const ClChannel& ai_pChannel, const ClAccessIndex& ai_accessIndex);
	static ClObjectId* CreateOLTPExecutionContextOId(void);
	static ClObjectId* CreateClientServerResultOId(void);
	
	// Info about the objects included in directory
	// These methods can be used by the receiver to get some info about the objects after 
	// or before deserializing them
	
	// Devuelve el número total de objetos, en el Directory
	m4uint16_t GetCount(void) const;
	
	// Devuleve el número de objetos que hay del tipo especificado, en el Directory
	m4uint16_t GetCountByType(const m4ObjectType_t ai_eObjectType) const;
	
	// Fija la dirección de un objeto del directorio. Sirve para que en la 
	// deserialización no se cree un objeto nuevo, sino que se deserialize
	// sobre uno existente (ojo el objeto debe estar pelado antes de deserializar)
	m4return_t SetObjectAddress(const ClObjectId& ai_roObjectId, void* ai_poPtr);
	
	/**
	 * Busca en el directorio, empezando desde el principio, un objeto de tipo ai_eObjectType con el nombre ai_string.
	 * Si ai_string = 0 devolverá el primer objeto del tipo especificado.
	 *
	 * @param ai_eObjectType	Tipo del objeto que se busca en el objDir.
	 * @param ai_string			Nombre del objeto que se busca en el objDir. Si no se especifica, la búsqueda
								se hará sólo por tipo.
	 * @return Índice del objeto, dentro del objDir. -1 si no se encuentra ninguno.
	 */
	m4int16_t GetFirstObjectIndex(const m4ObjectType_t ai_eObjectType, const m4char_t* const ai_string = 0);
	
	/**
	 * Busca en el directorio, a partir del último objeto encontrado, un objeto con el mismo tipo y nombre fijados por
	 * el método GetFirstObjectIndex.
	 *
	 * @return Índice del objeto, dentro del objDir. -1 si no se encuentra ninguno.
	 */
	m4int16_t GetNextObjectIndex(void);
	
	// Devuelve la dirección en memoria de un objeto
	void*     GetObjectAddress(m4uint16_t ai_iObjectIndex) const;
	// Devuelve el índice del objeto cuya direccion en memeoria es ai_pObject (si no está = -1)
	m4int16_t GetObjectIndex  (const void* const ai_pObject) const;
	
	// Devuleve el tipo de un objeto
	m4return_t GetObjectType(m4uint16_t ai_iObjectIndex, m4ObjectType_t& ao_iType) const;
	
	// Devuelve el OID de un objeto
	m4return_t GetObjectId(m4uint16_t ai_iObjectIndex, ClObjectId*& ao_poObjectId) const;
	
	void SetKeepAlive(m4uint16_t ai_iIndex, m4bool_t ai_bKeepAlive);
	
	void SetKeepAliveAll(m4bool_t   ai_bKeepAlive);
	
	void SetDoSerialize(m4uint16_t ai_iIndex, m4bool_t ai_bDoSerialize);
	
	// Funciones para determinar que bloque hay que serializar en un root multibloque (carriles)
	m4return_t SetRootBlockToSerialize(const ClChannel* const ai_pChannel,
									   m4uint32_t ai_iBlock,
									   m4uint32_t ai_iRegister);
	
	m4uint32_t GetRootBlockToSerialize(const ClChannel* const ai_pChannel) const;
	
	// Every object included in the directory, and the directory itself ...
	// Serializa primero todos los OID y luego todos los objetos
	m4return_t Serialize(ClGenericIODriver& IOD);
	
	// Para que busque los objetos en en la maquina virtual me tienen que dar el remoteID
	m4return_t DeSerialize(ClGenericIODriver& IOD, IObjDataCache* ai_pIObjDataCache, m4bool_t ai_bSetSttLevel);
	
	m4uint16_t GetSerializationVersion(void);
	
	// Only the OIDs ...
	m4return_t SerializeOId  (ClGenericIODriver& IOD);
	m4return_t DeSerializeOId(ClGenericIODriver& IOD);
	
	// Only the objects ...
	m4return_t SerializeObject  (ClGenericIODriver& IOD);
	m4return_t DeSerializeObject(ClGenericIODriver& IOD, IObjDataCache* ai_pIObjDataCache, m4bool_t ai_bSetSttLevel);
	
	m4uint32_t GetSerializedSize(m4uint16_t ai_iIndex) const;
	
	// Especifica un procesamiento que hay que hacer mientras se serializa el canal
	m4return_t SetChannelProcess(m4uint16_t ai_iIndex, ClChannelProcess* const ai_pChannelProc);
	
	// dumps the actual list of objects
	m4return_t DumpInfo(ostream &ai_pStream);
	
	m4return_t GetSize(ClSize &ao_size);

	// Elimina los objetos que no son canales
	void RemoveNonChannelObjects();

	void SetForceL2RelationSerialize(m4bool_t ai_bValue) { m_bForceL2RelationSerialize = ai_bValue; }
	m4bool_t GetForceL2RelationSerialize() const { return m_bForceL2RelationSerialize; }

	void SetKeepChannelCsTypeOnRemote(m4bool_t ai_bValue) { m_bKeepChannelCsTypeOnRemote = ai_bValue; }
	m4bool_t GetKeepChannelCsTypeOnRemote() const { return m_bKeepChannelCsTypeOnRemote; }

	/** Quita del array el objeto en ai_iIndex, lo destruye si el propietario es el directory. */
	void Remove(const m4int16_t ai_iIndex);

	/**
	 * Este método debe llamarse en el servidor siempre que hemos parado la ejeucción por un punto de ruptura.
	 * Se encarga de comprobar los objetos que hay en el objDir y si es la primer transacción de depuración que hacen,
	 * actualiza el remote ID y los marca para que no serializen a la vuelta al cliente.
	 *
	 * @return ClOLTPExecutionContext*	Como el ejecutor hay que meterlo al final del todo (para que ya esté el puntero al
	 *		recordset) se saca del objDir y se devuelve para que después se pueda volver a meter al final (depues de
	 *		los canales y accesos que han aparecido durante la ejecución).
	 */
	ClOLTPExecutionContext* const CheckFirstDebugTransaction(void);

	/*
	Función que chequea que los canales y accesos del objdir existan.
	Si no existen se pone el puntero a NULL y el remoteid a cero
	*/
	m4return_t	CheckObjectExistence( void ) ;

	// Inclusion de objetos
	m4int16_t IncludeMetaChannel(ClCompiledMCR* ai_pCompiledMCR, ClChannel* ai_pChannel);

private:
	m4return_t Init(ClChannelManager* ai_oChannelManager);
	
	m4int16_t Find(const ClObjectId& ai_pObjectId);
	
	ClChannel* _FindChannel(m4uint16_t ai_iIndex, m4uint16_t ai_iFirstChannelIndex, SetObjDirIndex_t& ai_rsetObjDirIndex);
	
	ClAccess*  _FindAccess(m4uint16_t ai_iAccessIndex, m4uint16_t ai_iChannelIndex);
	
	void Swap(m4uint16_t iNewIndex, m4uint16_t iOldIndex);
	
	// Creacion de objetos
	ClChannel* CreateAndBuildChannel(	m4uint16_t				ai_iIndex,
										ClLaneArgumnent* const	ai_pLaneArgument,
										m4uint8_t				ai_iLanguage,
										m4bool_t				ai_bIsLevel2,
										m4char_t* const			ai_pcRSM,
										m4double_t				ai_iCsType,
										IObjDataCache*			ai_pIObjDataCache);
	
	m4int16_t _IncludeClientServerResult(ClClientServerResult* ai_pCSResult);
	
	// Añade al final un objeto cualquiera al array del directorio. Devuelve el
	// indice donde se encuentra en el ObjDir (-1 == error, < count-1 == ya existia)
	m4int16_t _Include(PtrWithId& ai_oPtrWithId);
	
	void _Insert(m4uint16_t ai_iIndex);
	
	// Quita del vector todos los objetos, destrye los que pertenecen al directorio
	void _RemoveAllObj(void);
	
	m4int32_t FindChannelInCM(m4uint32_t ai_iHandle);
	m4int32_t FindChannelInCM(ClChannel *ai_poChannel);
	
	/**
	 * Comprueba la versión del metacanal.
	 * En caso de no coincidir se prepara el objDir para indicar al cliente que metacanales están desactualizados.
	 *
	 * @param ai_pChannel	Canal al que le vamos a comprobar la fecha del metacanal.
	 * @param ai_iIndex		Posición donde se encuentra el oid del canal, servirá para reportar el error.
	 * @return m4return_t.
	 */
	m4return_t CheckChannelVersion(ClChannel* const ai_pChannel, m4uint16_t ai_iIndex);
	
	m4return_t DeSerializeAllObjByType(ClGenericIODriver& IOD, m4ObjectType_t ai_eObjectType);
	
	m4return_t DeSerializeOneClientServerResultObj(ClGenericIODriver& IOD, m4uint16_t ai_iIndex);
	
	m4return_t DeSerializeOneMetaChannelObj(ClGenericIODriver& IOD, m4uint16_t ai_iIndex);
	
	m4return_t DeSerializeOneChannelObj(ClGenericIODriver& IOD, m4uint16_t ai_iIndex, ClCsTraceSize* const ai_pCsTraceSice, IObjDataCache* ai_pIObjDataCache, m4bool_t ai_bSetSttLevel);
	
	m4return_t DeSerializeOneAccessObj (ClGenericIODriver& IOD, m4uint16_t ai_iIndex);
	
	m4return_t DeSerializeOneOLTPExecutionContext(ClGenericIODriver& IOD, m4uint16_t ai_iIndex);

	m4return_t SerializeDeletedAutos( ClGenericIODriver& IOD ) ;
	m4return_t DeSerializeDeletedAutos( ClGenericIODriver& IOD, IObjDataCache* ai_pIObjDataCache ) ;

	m4return_t SerializeLicenseState( ClGenericIODriver& IOD ) ;
	m4return_t DeSerializeLicenseState( ClGenericIODriver& IOD ) ;

	/**
	 * Serializa los enlaces de nivel2 entre los canales que viajan en el objDir.
	 * Sólo los serializa si estamos en cliente o en el server en depuración, o cuando me fuerzan
	 * a hacerlo (por ejemplo en el ejecutor de XML, al persistir el estado a disco).
	 *
	 * @param IOD	IODriver sobre el que se escribe la serialización.
	 * @return m4return_t.
	 */
	m4return_t SerializeL2Relation(ClGenericIODriver& IOD);

	/**
	 * Deserializa los enlaces de nivel2 entre los canales que viajan en el objDir.
	 * Si estamos en cliente en depuración hace una copia de los enlaces de nivel2
	 * para, al restaurarlos, poder encontrar los que se han borrado.
	 *
	 * @param IOD	IODriver del que se lee la serialización.
	 * @return m4return_t.
	 */
	m4return_t DeSerializeL2Relation(ClGenericIODriver& IOD);

	/**
	 * Restaura las instancias de nievel2 entre los canales que hay en el objDir.
	 * Si estamos en cliente en depuración también se encarga de borrar las instancias de nivel2 que han desaparecido.
	 *
	 * @return m4return_t.
	 */
	m4return_t RestoreL2Relation(void);

	/**
	 * Comprueba si es un canal de depuración.
	 * Dependiendo del modo de depuración, fija el modo de serialización de los ítems y de los regsitros.
	 *
	 * @param ai_i	Índice en el objDir del canal sobre el que se quiere hacer al comprobación.
	 */
	void CheckDebugModeAndSetSerialMode(m4uint16_t ai_i);

	/**
	 * Comprueba si el canal existe antes de hacer la serialización.
	 * Puede que el canal se borrase por ser una instancia de nivel2 (ej, creo la instancia en cliente y en el
	 * servidor llamo a EraseAllLevel2Instances). Para comprobar esto voy a buscar el canal en el channelManager.
	 * Esto sólo lo voy a hacer en caso de que yo sea el dueño del objeto (yo lo he creado).
	 *
	 * @param ai_i	Índice en el objDir del canal sobre el que se quiere hacer al comprobación.
	 */
	void CheckChannelExistBeforeSerial(m4uint16_t ai_i);

	
	//
	// Data members.
	//
	/** ChannelManager donde se buscarán y crearán los canales. */
	ClChannelManager* m_poChannelManager;

	/** Array con los objetos. Crece dinámicamente, duplicando su tamaño cada vez. */
	PtrWithId*        m_oObjectPtr;

	/** Tamaño actual del array de objetos. */
	m4uint16_t        m_iSize;

	/** Número de objetos en el objDir. */
	m4uint16_t        m_iCount;

	/** Número de objetos de cada tipo, en el objDir. */
	m4uint8_t         m_iObjTypeCount[LastObjectType];

	/** Versión de serialización. Determina la compatibilidad entre el cliente y el servidor. */
	m4uint16_t        m_iSerializationVersion;
	
	// Data members for GetFirst...() GetNext...() functions.
	m4int16_t        m_iLastObjectIndex;
	m4ObjectType_t   m_iLastObjectType;
	m4char_t         m_pLastChannelName[81];
	
	/**
	 * Para dar soporte a las transacciones multicanal necesito un vector para
	 * guardarme los enlaces de nivel2 entre los canales que viajan.
	 * Los enlaces de nivel2 es lo primero que se serializa porque esta información se va a utilizar
	 * a la hora de buscar los canales antes de su deserialización.
	 */
	ClLevel2RealtionVector m_oLevel2RelationVector;

	/**
	 * Vector para guardar los enlaces de nivel2 antes de deserializar en el cliente.
	 * Se utiliza durante la depuraión de métodos remotos para borrar los enlaces que desaparecen en el servidor.
	 * Es necesario porque hasta después de haber deserializado los objetos no se restauran los enlaces
	 * de nivel2, y necesitamos conservar los que hemos serializado (están en m_oLevel2RelationVector) para
	 * saber cuales se han borrado.
	 */
	ClLevel2RealtionVector m_oLevel2RelationVectorBeforeDeser;
	
	/** Permite serializar los enlaces de nivel2 estando en el lado del servidor (Añadido para el cliente ligero). */
	m4bool_t m_bForceL2RelationSerialize;		
	m4bool_t m_bKeepChannelCsTypeOnRemote;	// Permite que los canales se construyan con el mismo CsType en el lado remoto

	m4bool_t m_bProhibitL2RelationSerialize;
public:
	m4bool_t GetbProhibitL2RelationSerialize(void) {
		return m_bProhibitL2RelationSerialize;
	}
	void SetbProhibitL2RelationSerialize(m4bool_t ai_bProhibitL2RelationSerialize) {
		m_bProhibitL2RelationSerialize = ai_bProhibitL2RelationSerialize;
	}

private:
	// User actions auditory.
	m4return_t SerializeUserActionsAudit( ClGenericIODriver& IOD ) ;
	m4return_t DeserializeUserActionsAudit( ClGenericIODriver& IOD ) ;
};

M4_DECL_M4OEINIT ostream & operator << (ostream &ao_Output, ClCMObjectDirectory &ai_oObjectDirectory);

#endif	// _OBJDIR_HPP_
