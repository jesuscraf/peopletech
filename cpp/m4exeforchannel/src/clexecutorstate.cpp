//	==============================================================================
//
//	 (c) Copyright  1991-1999 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ClExecutorState.cpp
//	 Project:           M4ExeForChannel
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980112
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================


#include "clexecutorstate.hpp"

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/AttributeList.hpp>

#include "access.hpp"
#include "clexecutorstate.hpp"
#include "m4filemanager.hpp"

#include "cldatstoiod.hpp"
#include "objdir.hpp"
#include "dm.hpp"
#include "channel.hpp"
#include "chandef.hpp"

#include "m4srvefcres.hpp"
#include "m4xmlutil.hpp"
#include "srvm4xmlsax.hpp"
#include "serverm4xml.hpp"

#include "clfactm4objservice.hpp"
#include "clm4objservice.hpp"
#include "m4cryptc.hpp"






//=========================================================
//======			ClExecutorState					=======
//=========================================================

// Datos miembros estaticos
ClCache	 *ClExecutorState::m_poCMCRCache = 0;
ClCache	 *ClExecutorState::m_poCSCRCache = 0;
ClCache	 *ClExecutorState::m_poPresentationCache = 0;
ClCache	 *ClExecutorState::m_poMapCache = 0;
ClCache	 *ClExecutorState::m_poDataCache = 0;


// Set Cache subsystems (static)
void ClExecutorState::SetCaches( ClCache *ai_poCMCRCache, ClCache *ai_poCSCRCache, 
			    ClCache *ai_poPresentationCache, ClCache *ai_poMapCache, ClCache *ai_poDataCache)
{
	m_poCMCRCache			= ai_poCMCRCache;
	m_poCSCRCache			= ai_poCSCRCache;
	m_poPresentationCache	= ai_poPresentationCache;
	m_poMapCache			= ai_poMapCache;
	m_poDataCache			= ai_poDataCache;
}

// Destructor
ClExecutorState::~ClExecutorState() 
{
	if (m_pcSessionId) {
		delete [] m_pcSessionId;
		m_pcSessionId = NULL;
	}
	m_uiSizeId = 0;
	m_pCO = NULL;
	if (m_poDataStorage) {
		M4DataStorage::DeleteDataStorage(m_poDataStorage);
		m_poDataStorage = NULL;
	}
	End();
	m_bUsing = M4_FALSE;
	m_bSerialized = M4_FALSE;
}

// Initializes state
m4return_t ClExecutorState::Init(ClCMInitParameters &aio_oCMInitParameters)
{
	m4return_t ret = M4_SUCCESS;
	m_poInitParameters = &aio_oCMInitParameters;
	m_poInitParameters->SetExecutorState(this);

	// Create and init ChannelManager & Environment
	ret = StartUp();
	if (ret != M4_SUCCESS)
		return M4_ERROR;

	// Create XML Context
	m_pXMLContext = new ClM4XMLContext(aio_oCMInitParameters);
	if (!m_pXMLContext)
		return M4_ERROR;
	
	return ret;
}

// Updates state
m4return_t ClExecutorState::Update(ClCMInitParameters &aio_oCMInitParameters)
{
	if (m_poInitParameters != NULL)
		return M4_SUCCESS;

	m_poInitParameters = &aio_oCMInitParameters;
	m_poInitParameters->SetExecutorState(this);

	// checks internal members
	if (GetXMLContext() != NULL)
	{
		GetXMLContext()->Update(aio_oCMInitParameters);
	}

	return M4_SUCCESS;
}

// Destroy state
m4return_t ClExecutorState::End()
{
	m4return_t ret = M4_SUCCESS;

	// Destroy XML Context
	if (m_pXMLContext) {

		// frees only logical connection
		m_pXMLContext->ReleaseLogicalConnection();

		delete m_pXMLContext;
		m_pXMLContext = NULL;
	}

	// Destroy ChannelManager & Environment
	ret = ShutDown();
	if (ret != M4_SUCCESS)
		return M4_ERROR;

	return ret;
}

void ClExecutorState::SetUsing(m4bool_t ai_bUsing)
{ 
	m_bUsing = ai_bUsing; 
	if (m_bUsing == M4_FALSE)
	{
		ClM4XMLContext* poContext = GetXMLContext();
		if (poContext != NULL)
		{
			poContext->ExecUnlock();
		}
	}
}

// Set the DataStorage with a new value, and freeze the previous
void ClExecutorState::SetDataStorage(M4DataStorage* ai_poDataStorage)
{
	if (m_poDataStorage) {
		M4DataStorage::DeleteDataStorage(m_poDataStorage);
	}
	m_poDataStorage = ai_poDataStorage;
}

// Set the Session Id (Object Id)
m4bool_t ClExecutorState::SetSessionId(m4pcchar_t ai_pccSessionId) {
	// Copy Id
	if (ai_pccSessionId) {
		if (m_pcSessionId)
			delete [] m_pcSessionId;
		m_uiSizeId = strlen(ai_pccSessionId) + 1;
		m_pcSessionId = new m4char_t[m_uiSizeId];
		if (m_pcSessionId)
			strncpy(m_pcSessionId, ai_pccSessionId, m_uiSizeId);
		return M4_TRUE;
	} else {
		m_uiSizeId = 0;
		m_pcSessionId = NULL;
	}
	return M4_FALSE;
}


// Returns object size
m4uint32_t	ClExecutorState::GetSize( void ) const {
	m4uint32_t uiSize = sizeof(ClExecutorState) + m_uiSizeId;
	if ( m_poDataStorage )
		uiSize += m_poDataStorage->Size();
	return uiSize;
}

// Returns object version
m4date_t ClExecutorState::GetVersion( void ) const {
	return ClMinusInfiniteDate();
}


// Initialize Data Storage (creates or clear a Data Storage)
m4return_t ClExecutorState::InitDataStorage()
{
	// DeSerializamos el DataStorage
	if ( !m_poDataStorage ) {
		m_poDataStorage = M4DataStorage::GetNewDataStorage();
	}
	if ( m_poDataStorage ) {
		m_poDataStorage->Clear();
		return M4_SUCCESS;
	}
	return M4_ERROR;
}

// Cumplimos el Interfaz de Fechas de la Cache, pero no lo usamos...
m4date_t ClExecutorState::GetStartDate( void ) const {
	static m4date_t s_dDate = ClMinusInfiniteDate();
    return s_dDate;
}
m4date_t ClExecutorState::GetEndDate( void ) const {
	static m4date_t s_dDate = ClPlusInfiniteDate();
    return s_dDate;
}
m4date_t ClExecutorState::GetCorStartDate( void ) const {
	static m4date_t s_dDate = ClMinusInfiniteDate();
	return s_dDate;
}
m4date_t ClExecutorState::GetCorEndDate( void ) const {
	static m4date_t s_dDate = ClPlusInfiniteDate();
    return s_dDate;
}


// Clone this executor state using serialize/deserialize
m4return_t ClExecutorState::Clone(m4pchar_t ai_pccSessionId, ClCMInitParameters &aio_oInitParameters, ClExecutorState *&ao_poExecutorState)
{
	// Region critica
	//static ClMutex oLocalMutex(M4_TRUE);

	//ClMutBlock oBlock(&oLocalMutex);

	m4return_t ret = M4_SUCCESS;
	ao_poExecutorState = NULL;
	
	if (!ai_pccSessionId)
		return M4_ERROR;

	// Create & init a new ExecutorState
	ao_poExecutorState = new ClExecutorState(ai_pccSessionId);
	ret = ao_poExecutorState->Init( aio_oInitParameters );
	if (ret != M4_SUCCESS) {
		// salida limpia
		delete ao_poExecutorState;
		ao_poExecutorState = NULL;
		return M4_ERROR;
	}

	// Backup serialized state
	//m4bool_t bOrigenSerialized = IsSerialized();
	//SetSerialized(M4_FALSE);

	// User Actions Auditory.
	// Evitamos que se serialicen eventos que puedan quedar
	m_poChannelManager->UserActionsAudit.Reset() ;

	// Serializar el Antiguo ChannelManager (& Context)
	ret = SerializeChannelManager(M4_TRUE);
	if (ret != M4_SUCCESS) {
		// salida limpia
		ret = M4_ERROR;
	}

	if (ret == M4_SUCCESS) {
		// Movemos el DataStorage (datos serializados) del Origen al Destino
		ao_poExecutorState->SetDataStorage( GetDataStorage() );

		// Deserializar sobre el Nuevo ChannelManager (& Context)
		ret = ao_poExecutorState->DeSerializeChannelManager();
	}

	// Clean DataStorage (always)
	ao_poExecutorState->SetDataStorage(NULL);
	m_poDataStorage = NULL;	// manual clean
	
	// Restore serialized state
	//SetSerialized( bOrigenSerialized );

#ifdef _DEBUG
	//Contamos el no. de canales Serializados/Deserializados en cada ChannelManager
	m4uint32_t iNumChanOrig = m_poChannelManager->Channel.Count();
	m4uint32_t iNumChanDest = ao_poExecutorState->GetChannelManager()->Channel.Count();
	if (iNumChanOrig != iNumChanDest) {
		cout << "Warning: ClExecutorState::Clone()" << endl;
		cout << "- No. Channels Serialized   " <<  iNumChanOrig << endl;
		cout << "- No. Channels DeSerialized " <<  iNumChanDest << endl;
		return M4_ERROR;
	}
#endif

	if (ret != M4_SUCCESS) {
		// salida limpia
		delete ao_poExecutorState;
		ao_poExecutorState = NULL;
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

// Serialize ChannelManager
m4return_t ClExecutorState::SerializeChannelManager(m4bool_t ai_bIgnoreRefs)
{
	m4return_t ret = M4_SUCCESS;

	// checks for references
	// this should block the ChannelManager
	if (ai_bIgnoreRefs == M4_FALSE &&  m_pCO->IsReferenced())
		return M4_ERROR;
	
	if ( !HasState() ) {
		if ( HasStateDS() )
			return M4_SUCCESS;
		return M4_ERROR;
	}

	// Inicializamos el DataStorage: le creamos o le reseteamos segun haga falta
	ret = InitDataStorage();	// En la forma antigua
	if (ret != M4_SUCCESS) {
		return M4_ERROR;
	}
	M4DataStorage* poOutputDataStorage = GetDataStorage();	// Antigua forma

	// Creamos el IODriver que sabe como serializar.
	ClDataStoreIODriver oDSIOD;

	// Inicializamos IODriver, usando el DataStorage (salida de datos)
	ret = oDSIOD.Init(GEN_IO_MODE_WRITE, poOutputDataStorage);

#if _M4_DEBUG > 10
	// CheckSum de comprobación
	if (ret == M4_SUCCESS) {
		ret = oDSIOD.InitCheckSum();
	}
#endif

	if (ret == M4_SUCCESS) {
		//	Creamos ObjectDirectory temporal: ClCMObjectDirectory	- gestor de objetos a serializar
		ClCMObjectDirectory oObjDir(m_poChannelManager);
		// Activamos Serialización de los Niveles 2, para este Channel Manager
		oObjDir.SetForceL2RelationSerialize(M4_TRUE);
		
		ClChannel*	pChanAux = NULL;
		m4pcchar_t	pcAuxChanId = 0;
		m4uint32_t	i;
		ClChannelDirectory* pChanDir = &( m_poChannelManager->Channel );
		m4uint32_t iNumChan = pChanDir->Count ();	//No. de canales

#if _M4_DEBUG > 10
		//Contamos el no. de canales Serializados en el ChannelManager
		if (iNumChan <= 1) {
			cout << "Warning: ClExecutorState::SerializeChannelManager() - only " << iNumChan << " serialized" << endl;
			//return M4_ERROR;
		}
#endif
	
		// Recorremos todos los Canales del ChannelManager
		for ( i = 0; i < iNumChan; i++ ) {
			pChanAux = (*pChanDir)[i];

			if (pChanAux->IsBuilt())
				pcAuxChanId = pChanAux->GetpChannelDef ()->Id ();
			else
				pcAuxChanId = 0;

			// Deberiamos saltar los canales del sistema - pero ya estan detachados del Channel Manager
				
			// Marcamos el Canal para serializar todos los registros
			pChanAux->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ALL);
			
			// Marcamos el Canal para serializar todos los Items (mapped, etc.)
			pChanAux->SetItemSerializationMode(M4CL_ITEM_SERIAL_ALL);
			
			// Añadimos el Canal
			ret = ( oObjDir.IncludeChannel(pChanAux,0) == -1 ) ? M4_ERROR : M4_SUCCESS;		// Incluimos el Channel
			if (ret == M4_SUCCESS) {
				// Añadimos todos los Access del Canal
				ret = oObjDir.IncludeAllAccessAndCreateOID(*pChanAux, M4_TRUE);	// Incluimos todos los Access
			}
			
			if (ret != M4_SUCCESS) {
				break;
			}
		}
		
		// Serializamos el ObjectDirectory
		if (ret == M4_SUCCESS) {
			oObjDir.SetKeepChannelCsTypeOnRemote(M4_TRUE);  // keeps CS types, para que no se borren al destruir el ObjectDirectory
			
			ClM4XMLInfo::SetFileInfoByThread("SerializeChannelManager-Serialize-Start");
			ret = oObjDir.Serialize(oDSIOD);				// Sesion
			ClM4XMLInfo::SetFileInfoByThread("SerializeChannelManager-Serialize-End");
		}

		// Serializamos el Contexto de XML
		if ( m_pXMLContext && ret==M4_SUCCESS ) {
			ClM4XMLInfo::SetFileInfoByThread("SerializeChannelManager-ContxSerialize-Start");
			ret = m_pXMLContext->Serialize(oDSIOD);
			ClM4XMLInfo::SetFileInfoByThread("SerializeChannelManager-ContxSerialize-End");
		}

#if _M4_DEBUG > 10
		// CheckSum de comprobación
		if (ret == M4_SUCCESS)
			ret = oDSIOD.CreateCheckSum(poOutputDataStorage);
#endif

		// Cerramos el IODriver
		oDSIOD.End();

		// Deberiamos tambien restaurar el tipo de Serializacion (de registro y de item) para cada Canal
		//m4uint8_t GetRecordSerializationMode(void) { return m_iRecSerMod; }
		//m4uint8_t GetItemSerializationMode(void) { return m_iIteSerMod; }
		//oObjDir.SetForceL2RelationSerialize(M4_FALSE);	//Desactivamos Serialización de los Niveles 2
	}
	// De esta forma tenemos en el objeto ExecutorState - el ID y el Estado (object directory) serializado.

	return ret;

}

// DeSerialize ChannelManager
m4return_t ClExecutorState::DeSerializeChannelManager()
{
	m4return_t ret;

	if ( HasState() ) {
		// Reaprovechamos el antiguo estado
		ret = ResetChannelManager();
		if (ret != M4_SUCCESS || !m_pXMLContext) {
			return M4_ERROR;
		}
	} else {
		// Construimos el estado de la nada
		ret = StartUp();
		if (ret != M4_SUCCESS) {
			return M4_ERROR;
		}
	}

	M4DataStorage* poInputDataStorage = M4DataStorage::GetNewDataStorage( GetDataStorage()->GetDataUnitInterfaceQueue() );

	/* Bug 0099862
	Hay que recorrer los FILE_UNIT avanzando el iterador siempre
	*/
	poInputDataStorage->m_bFinishSerialization = M4_TRUE ;

	// Creamos el IODriver que sabe como serializar.
	ClDataStoreIODriver oDSIOD;	// Sabe como serializar

	// Inicializamos IODriver, usando el DataStorage (salida de datos)
	ret = oDSIOD.Init(GEN_IO_MODE_READ, poInputDataStorage);

#if _M4_DEBUG > 10
	// CheckSum de comprobación
	// checks the check sum
	M4_ASSERT( oDSIOD.CheckSum(poInputDataStorage) == M4_TRUE );
#endif

	// Creamos ObjectDirectory temporal: ClCMObjectDirectory	- gestor de objetos a deserializar
	ClCMObjectDirectory oObjDir(m_poChannelManager);

	if (ret == M4_SUCCESS) {
		// Deserializamos el Estado
		ClM4XMLInfo::SetFileInfoByThread("DeserializeChannelManager-Deserialize-Start");
		ret = oObjDir.DeSerialize(oDSIOD, NULL, M4_TRUE);		// Sesion
		ClM4XMLInfo::SetFileInfoByThread("DeserializeChannelManager-Deserialize-End");

#if _M4_DEBUG > 10
		// Volcado de los estados a disco... para depuracion (ver comentario en el inicio)
		if ( ret != M4_SUCCESS ) {
			// Error en Deserializacion
			static int iCountError = 0;
			{
				ClMutBlock oMut(&m_oMutex);
				iCountError++;
			}
			m4char_t pszFileName[100];
			sprintf(pszFileName, "c:\\temp\\state_%d_%s.sal", iCountError, ai_pccSessionId );

			cerr << "Error " << iCountError << " deserialize state: " << ai_pccSessionId << endl;
			cerr.flush();

			// Volcamos a disco el estado que dio problemas
			ClFileIODriver oFileIOD;
			oFileIOD.Init(GEN_IO_MODE_WRITE, pszFileName);
			poExecutorState->Serialize(oFileIOD);	// Ojo con este serialize que no es reentrante para el mismo objeto
			oFileIOD.End(M4_TRUE);
		} else {
			// OK en Serializacion
			static int iCountSucces = 0;
			{
				ClMutBlock oMut(&m_oMutex);
				iCountSucces++;
			}
			m4char_t pszFileName[100];
			sprintf(pszFileName, "c:\\temp\\state_ok_%d_%s.sal", iCountSucces, ai_pccSessionId );

			cerr << "OK " << iCountSucces << " deserialize state: " << ai_pccSessionId << endl;
			cerr.flush();

			// Volcamos a disco el estado que dio problemas
			ClFileIODriver oFileIOD;
			oFileIOD.Init(GEN_IO_MODE_WRITE, pszFileName);
			poExecutorState->Serialize(oFileIOD);	// Ojo con este serialize que no es reentrante para el mismo objeto
			oFileIOD.End(M4_TRUE);
		}
#endif

		// Deserializamos el Contexto
		if ( ret==M4_SUCCESS ) {
			
			//temporal
			ClCMInitParameters oInitParameters;
			ClCMInitParameters *poInitParameters = NULL;
			if (m_poInitParameters == NULL)
				poInitParameters = &oInitParameters;
			else
				poInitParameters = m_poInitParameters;

			// Create XML Context if doesn't exist
			if (m_pXMLContext == NULL)
			{
				m_pXMLContext = new ClM4XMLContext( *poInitParameters );
				if (!m_pXMLContext)
					return M4_ERROR;
			}

			ClM4XMLInfo::SetFileInfoByThread("DeserializeChannelManager-ContxDeserialize-Start");
			ret = m_pXMLContext->Deserialize(oDSIOD);
			ClM4XMLInfo::SetFileInfoByThread("DeserializeChannelManager-ContxDeserialize-End");
		}
		//	-	Cerramos el IODriver
		oDSIOD.End();

		// Marcamos todos los objetos, para poder destruir el ObjectDirectory sin destruirlos
		oObjDir.SetKeepAliveAll(M4_TRUE);
	}

#if _M4_DEBUG > 10
	//Contamos el no. de canales DeSerializados en el ChannelManager
	m4uint32_t iNumChan = m_poChannelManager->Channel.Count();
	if (iNumChan <= 1) {
		cout << "Warning: ClExecutorState::DeSerializeChannelManager() - only " << iNumChan << " deserialized" << endl;
		//return M4_ERROR;
	}
#endif


	// Borramos el DataStorage Temporal
	if (poInputDataStorage) {
		M4DataStorage::DeleteDataStorage( poInputDataStorage );
		poInputDataStorage = NULL;
	}

	return ret;
}


#define	M4CH_DUMMY_E	"\x7\xB\x5\x7\x1\x9\x7\xA\xC\x8\xF\x3\xD\xF\xA\x1\x8\x1\x2\x4\x5\x4\xB\xD\x8\xA\xD\x2\x5\x3\x3\xA\x9\x1\xE\x4\x8\x7\xF\xC\x7\xD\x4\xB\xD\x6\x6\x8\x3\x7\x7\x5\xF\x1\xF\xD\x5\xE\x6\x1\xD\xE\xA\x0\x9\x1\x4\x7\x6\xC\x1\xB\x4\xE\x8\xA\x5\x7\x7\xB\x8\x9\x5\x7\xB\xB\x2\x6"




// Serialize the object using the IODriver
m4return_t ClExecutorState::Serialize(ClGenericIODriver &  IOD)
{
	m4return_t ret = M4_SUCCESS;
	m4bool_t bTempSerial = M4_FALSE;

	if ( !HasState() ) {
		// No hay channelManager
		if ( !HasStateDS() )	// Tenemos que tener estado en forma de DataStorage
			return M4_ERROR;
	}

	// =================================
	// Si el estado no esta serializado (tenemos ChannelManager)
	//		generamos estado serializado (temporal): ChannelManager -> DataStorage
	if ( !IsSerialized() ) {
		ret = SerializeChannelManager();
		if (ret != M4_SUCCESS)
			return M4_ERROR;
		bTempSerial = M4_TRUE;
		//SetSerialized(M4_TRUE);
	}
	// =================================

	// Serializamos los campos del objeto
	//ret = IOD.Write(m_uiSizeId);
	//if (ret == M4_ERROR)
	//	return M4_ERROR;

	// Serializamos el Id
	ret = IOD.Write(m_pcSessionId, m_uiSizeId);
	if (ret == M4_ERROR)
		return M4_ERROR;

	// Serializamos flag Boolean
	ret = IOD.Write(m_bSerialized);
	if (ret == M4_ERROR)
		return M4_ERROR;

	// Serializamos flag Boolean
	ret = IOD.Write(m_bUsing);
	if (ret == M4_ERROR)
		return M4_ERROR;

	// Serializamos el DataStorage: No.bloques + {Bloques}*
	ClDataUnitInterface *poDataUnit = NULL;	// Para recorrer las unidades del DataStorage
	m4uint32_t size = 0;
    m4int_t iEncryptedSize = 0;
	m4pchar_t pBuffer = NULL;
    m4pchar_t pcEncryptedBuffer = NULL;


	// resets serialization boolean
	// m4pdus requirement
	m_poDataStorage->m_bFinishSerialization=M4_TRUE;
	m_poDataStorage->InitializeIteration();

	// Serializamos el No. de bloques
	m4uint32_t uiNumBlocks = m_poDataStorage->Blocks();
	ret = IOD.Write( uiNumBlocks );
	if (ret == M4_ERROR)
		return M4_ERROR;

	m4int16_t iType = 0;

	// Recorremos las unidades o bloques del DataStorage, y las volcamos
	while ( m_poDataStorage->GetNext( poDataUnit ) == M4_SUCCESS) {
		
		// Serializamos el tipo del bloque:
		iType = poDataUnit->GetType();
		ret = IOD.Write( iType );
		if (ret == M4_ERROR)
			return M4_ERROR;

		// Serializamos el bloque, segun el tipo
		if ( iType == M4_FILE_UNIT ) {
			// Buffer con fichero asociado
			pBuffer = ((ClFileUnit *)poDataUnit)->GetFileName();
			if (pBuffer) {

				if (m_poListFileNames != NULL)
				{
					//Guardamos los nombres de los ficheros en una lista
					m_poListFileNames->push_back(pBuffer);
				}

				/* Bug 0099863
				Hay que copiar también el 0 del final
				*/
				size = strlen(pBuffer) + 1;
				ClFileManager::Instance() -> SetInterestInFile(pBuffer);
			} else {
				size = 0;
			}
		} else {
			// Buffer normal
			size	= poDataUnit->GetSize();
			pBuffer	= poDataUnit->GetBuffer();
		}
		

		/* Bug 0102060
		Los datos de los XML que van a fichero deben ir encriptados
		*/
		ret = EncryptTwoWayRaw( pBuffer, size, M4CH_DUMMY_E + 23, strlen( M4CH_DUMMY_E + 23 ) * sizeof( m4char_t ), pcEncryptedBuffer, iEncryptedSize ) ;

		if( ret != M4_SUCCESS )
		{
            DUMP_CHLOG_ERROR( M4_ERR_XML_NO_ENCRYPT ) ;
			return( M4_ERROR ) ;
		}
        
        ret = IOD.Write(pcEncryptedBuffer, iEncryptedSize);
		delete [] pcEncryptedBuffer ;

		if (ret != M4_SUCCESS) {
			ret = M4_ERROR;
			break;
		}
	}

	// =================================
	// Una vez serializado y si es temporal... tiramos a la basura el estado serializado (temporal)
	if ( bTempSerial ) {
		//SetSerialized(M4_FALSE);
		SetDataStorage(NULL);
	}
	// =================================

	return ret;
}

// DeSerialize the object using the IODriver - Build the object
m4return_t ClExecutorState::DeSerialize(ClGenericIODriver &  IOD)
{
	m4return_t	ret = M4_SUCCESS;
	m4uint32_t	uiSizeId = 0;
	m4pchar_t	pcSessionId = NULL;

	// Read the Session Id: size+buffer
	ret = IOD.ReadSize(uiSizeId);
	if (ret == M4_ERROR)
		return M4_ERROR;

	pcSessionId = new m4char_t[uiSizeId];
	ret = IOD.ReadBuffer(pcSessionId);
	if (ret == M4_ERROR) {
		delete [] pcSessionId;
		return M4_ERROR;
	}

	// Read flag Boolean
	ret = IOD.Read(m_bSerialized);
	if (ret == M4_ERROR)
		return M4_ERROR;

	// Read flag Boolean
	ret = IOD.Read(m_bUsing);
	if (ret == M4_ERROR)
		return M4_ERROR;

	// Inicializamos el DataStorage
	ret = InitDataStorage();
	if ( ret != M4_SUCCESS )
		return M4_ERROR;

	// Leemos el no. de bloques que tendra el DataStorage
	m4uint32_t	uiNumBlocks = 0;
	ret = IOD.Read(uiNumBlocks);
	if (ret == M4_ERROR) {
		delete [] pcSessionId;
		return M4_ERROR;
	}

	m4pchar_t poBuffer = NULL;
    m4pchar_t pcDecryptedBuffer = NULL;
	m4uint32_t	uiSizeDS = 0;
    m4int_t iDecryptedSize = 0;
	m4int16_t	iType = 0;
	
	// Leemos cada bloque, y lo enganchamos al DataStorage
	for (m4uint32_t i=0; i<uiNumBlocks; ++i) {

		// Leemos el tipo del bloque que tendra el DataStorage
		ret = IOD.Read(iType);
		if (ret == M4_ERROR) {
			delete [] pcSessionId;
			return M4_ERROR;
		}
		
		// Leemos el tamaño de cada bloque
		ret = IOD.ReadSize(uiSizeDS);
		if (ret == M4_ERROR) {
			delete [] pcSessionId;
			return M4_ERROR;
		}

		// Leemos el bloque: filename or data
		poBuffer = new m4char_t[uiSizeDS];
		if (!poBuffer) {
			delete [] pcSessionId;
		}
		ret = IOD.ReadBuffer( poBuffer );
		if (ret == M4_ERROR) {
			delete [] pcSessionId;
			delete [] poBuffer;
			return M4_ERROR;
		}

		/* Bug 0102060
		Los datos de los XML que vienen de fichero están encriptados
		*/
		ret = DecryptTwoWayRaw( poBuffer, uiSizeDS, M4CH_DUMMY_E + 23, strlen( M4CH_DUMMY_E + 23 ) * sizeof( m4char_t ), pcDecryptedBuffer, iDecryptedSize ) ;
        delete [] poBuffer ;

		if( ret != M4_SUCCESS )
		{
			delete [] pcSessionId;
            DUMP_CHLOG_ERROR( M4_ERR_XML_NO_DECRYPT ) ;
			return( M4_ERROR ) ;
		}

		if ( iType == M4_FILE_UNIT ) {
			// Si es un FileUnit, añadimos indicando el nombre del archivo
			// esto por dentro ya incrementa las referencias al archivo en la M4FileManager
			ret = m_poDataStorage->AddFileUnit(pcDecryptedBuffer);
			// Decremento las referencias al archivo... por si era temporal... para que no se borre
			ClFileManager::Instance() -> UnSetInterestInFile(pcDecryptedBuffer);
			if (ret == M4_SUCCESS) {
				// En este caso no se cachea el buffer(nombre)... pero en el siguiente SI... vaya mamonazo
				delete [] pcDecryptedBuffer;
			}

		} else {
			// Añadimos el buffer al DataStorage
			ret = m_poDataStorage->Add(pcDecryptedBuffer, iDecryptedSize);
		}
		if (ret == M4_ERROR) {
			delete [] pcSessionId;
			delete [] pcDecryptedBuffer;
			return M4_ERROR;
		}
		//delete [] pcDecryptedBuffer; - Se cachea el puntero el mamonazo del DataStorage
	}

	//SetSessionId( pcSessionId );
	//delete [] pcSessionId;

	// Optimizacion:
	m_pcSessionId = pcSessionId;
	m_uiSizeId = uiSizeId;

	//SetSerialized(M4_TRUE);

	// =================================
	// Como el estado esta serializado
	//		generamos estado deserializado: DataStorage -> ChannelManager
	ret = DeSerializeChannelManager();
	if (ret != M4_SUCCESS)
		return M4_ERROR;
	
	// Si no debe estar serializado =>
	if ( !IsSerialized() ) {
		//SetSerialized(M4_FALSE);
		SetDataStorage(NULL);	// Eliminamos el DataStorage
	}
	// =================================

	return ret;
}

m4return_t ClExecutorState::ResetChannelManager()
{
	// Reset current channelmanager
	m4return_t iResult = M4_SUCCESS;

	// releases the session channel
	if (m_poChannelManager->SetSessionChannel(NULL) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_SET_SESSION_CHANNEL_BASE );
		iResult = M4_ERROR;
	}

	// releases the channels
	iResult = m_poChannelManager->Reset();
	if (iResult != M4_SUCCESS)	
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_RESET_DM );
		iResult = M4_ERROR;
	}

	return iResult;
}

// Set the ChannelManger with a new value, and freeze the previous
m4return_t ClExecutorState::SetChannelManager(ClChannelManager* ai_poChannelManager)
{
	m4return_t ret = M4_SUCCESS;
	if (m_poChannelManager) {
		if (m_poChannelManager == ai_poChannelManager)
			return M4_SUCCESS;	// optimizacion
		ret = ShutDown();
	}
	if (ret == M4_SUCCESS) {
		m_poChannelManager = ai_poChannelManager;
	}
	return ret;
}

// Set the XML Context with a new value, and freeze the previous
m4return_t ClExecutorState::SetXMLContext(ClM4XMLContext* ai_pXMLContext)
{
	m4return_t ret = M4_SUCCESS;
	if (m_pXMLContext) {
		if (m_pXMLContext == ai_pXMLContext)
			return M4_SUCCESS;	// optimizacion
		delete m_pXMLContext;
	}
	m_pXMLContext = ai_pXMLContext;
	return ret;
}

// Initialize ChannelManger
m4return_t ClExecutorState::StartUp()
{
	m4return_t iResult = M4_SUCCESS;

	// Creamos llamando a la Factoria
	m_poM4ObjService = ClFactM4ObjService::CreateObjService(m_poCMCRCache, m_poCSCRCache, m_poPresentationCache, m_poMapCache, m_poDataCache, M4_TRUE);

	if (!m_poM4ObjService)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INIT_CACHE );
		iResult = M4_ERROR;
	} else {
		// Nos quedamos el Gestor de Canales
		m_poChannelManager = m_poM4ObjService->GetpChannelManager();
	}

	return iResult;
}


// Close ChannelManger
m4return_t ClExecutorState::ShutDown()
{
	m4return_t iResult = M4_SUCCESS;

	// Destruimos llamando a la Factoria
	if (m_poM4ObjService != NULL)
		iResult = ClFactM4ObjService::DestroyObjService(m_poM4ObjService);

	// closes the m4objectservice
	if (iResult != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_ENV_END );
#ifdef _DEBUG
		cerr << "ExeForM4XML: Error when destroy environment!!" << endl;
#endif
		return iResult;
	}
		
	// releases memory
	m_poChannelManager = NULL;
	m_poM4ObjService = NULL;

	return iResult;
}




//=========================================================
//======		ClExecutorStateDataContainer		=======
//=========================================================

// Creates Object Id using a key
ClObjectId * ClExecutorStateDataContainer::CreateObjectIdFromKey(void)
{
	m4pchar_t pcId;

	pcId = m_poClCacheKey->Getm_pszKey();
	ClTypedId oTypeID(pcId);
	ClUniqueKey oUniqueKey("");
	ClEnvironmentId oEnvId( GetEnvId() );
	ClObjectId * poObjID = new ClObjectId (ExecutorStateObjectType, &oEnvId, &oTypeID, &oUniqueKey);
	
	return poObjID;
}

// Serialize object state using IODriver
m4return_t ClExecutorStateDataContainer::Serialize(ClGenericIODriver& IOD)
{
	m4return_t ret;

	ret = ClExecutorStateContainerBase_t::Serialize(IOD);

	if (M4_ERROR == ret)
		return M4_ERROR;

	// Serialize object data
	ret = IOD.Write(m_dT3Version);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

// saves the value to disk
// local implementation
m4return_t ClExecutorStateDataContainer::SaveValueToDisk ( void )
{
	m_oListFileNames.clear();
	m_pObject->SetReferencedFileList(&m_oListFileNames);

	// invoques que parent's method
	m4return_t iRet = M4_SUCCESS;
	iRet = ClExecutorStateContainerBase_t::SaveValueToDisk();

	m_pObject->SetReferencedFileList(NULL);

	return iRet;
}

// deletes the value from disk
m4return_t ClExecutorStateDataContainer::DeleteValueFromDisk ( void )
{
	if (!m_pObject)
	{
		//Decrementamos las referencias de los ficheros apuntados por m_poObject
		ClFileManager* myFileManager = ClFileManager::Instance();

		//Copiamos los nombres de los ficheros a la lista del contenedor
		list<string>::iterator itListFileNames;

		for (itListFileNames = m_oListFileNames.begin(); itListFileNames != m_oListFileNames.end(); itListFileNames++) {
			myFileManager->UnSetInterestInFile((*itListFileNames).c_str());
		}

		//Borramos la lista de nombres de ficheros en el objeto contenedor
		m_oListFileNames.clear();
	}

	// invoques que parent's method
	return ClExecutorStateContainerBase_t::DeleteValueFromDisk();
}

// DeSerialize object state using IODriver
m4return_t ClExecutorStateDataContainer::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t ret;

	ret = ClExecutorStateContainerBase_t::DeSerialize(IOD);

	if (M4_ERROR == ret)
		return M4_ERROR;

	// Deserialize object data
	ret = IOD.Read(m_dT3Version);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

// Check if it matches a particular condition
m4bool_t ClExecutorStateDataContainer::MatchCondition (void * ai_pCondition)
{

	// checks parameters
	if (ai_pCondition == NULL)
	{
		return M4_FALSE;
	}

	ClExecutorStateDataMatcherById *poMatcher = (ClExecutorStateDataMatcherById*) ai_pCondition;

	// processes and returns the result
	return poMatcher->Match(this);

}

//=========================================================
//======		ClExecutorStateDataMatcherById				=======
//=========================================================

// constructor
ClExecutorStateDataMatcherById::ClExecutorStateDataMatcherById(string &ai_sPrefix, string &ai_sId)
{
	m_sId = ai_sId;
	m_sPrefix = ai_sPrefix;
}

// comprator
m4bool_t ClExecutorStateDataMatcherById::Match(ClExecutorStateDataContainer *ai_poRef)
{
	// if we receive a null parameter we assume distinct
	if (ai_poRef == NULL)
	{
		return M4_FALSE;
	}

	ClCacheKey *poKey = NULL;
	ai_poRef->GetKey(poKey);
	if (poKey == NULL)
	{
		return M4_FALSE;
	}

	m4pchar_t pcRefKey = poKey->Getm_pszKey();
	if (pcRefKey == NULL)
	{
		return M4_FALSE;
	}

	// checks the prefix
	if (strncmp(pcRefKey, m_sPrefix.c_str(), m_sPrefix.size()) != 0) 
	{
		return M4_FALSE;
	}

	// returns the difference
	return (strncmp(m_sId.c_str(), pcRefKey, m_sId.size()) == 0) ? M4_FALSE : M4_TRUE;
}

//=========================================================
//======		ClExecutorStateCache				=======
//=========================================================
#define M4_SESSION_CACHE_NAME	"m4sescac"		// Name for the session cache (cache de estados)
// see too diskpers.cpp - where we indicate the name of directory where store states.

// Constructor
ClExecutorStateCache::ClExecutorStateCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio)
	: ClExecutorStateCacheBase_t(ai_poCache, ai_pPath, M4_SESSION_CACHE_NAME, ai_iMode, ai_iMaxMemory, ai_iNumObjects, ai_iRefreshRatio, 0, 0)
{
	static ClExecutorStateCacheableObjectFactory * pCoFactory = new ClExecutorStateCacheableObjectFactory();
	SetPersistor(pCoFactory);
	SetCOFactory(pCoFactory);
}

// Search and get an object from cache
m4return_t ClExecutorStateCache::GetObject(m4pchar_t ai_pcIdSession, ClExecutorState* &ao_rpExecutorState, m4bool_t &ao_bFound, ClCacheInterface::eGetMode ai_eGetMode, m4int32_t ai_iTimeOut)
{
	m4return_t ret = M4_SUCCESS;
	m4uint16_t iVerCounter; //No utilizamos su valor de vuelta

	// Lo hacemos estatico... porque no lo usamos... y hay que cumplir el interfaz del template de la Caches
	static m4date_t s_dStartDate	= ClMinusInfiniteDate();
	static m4date_t s_dEndDate		= ClPlusInfiniteDate();
	static m4date_t s_dCorDate		= ClMinusInfiniteDate();


	// Esta funcion es reentrante: devuelve una referencia al objeto cacheable exclusiva y no bloqueante. ClCacheInterface::eGetMode
	ret = _GetObject(ai_pcIdSession, s_dStartDate, s_dEndDate, s_dCorDate, iVerCounter, ao_rpExecutorState, ai_eGetMode, ai_iTimeOut);

	if (ret == M4_SUCCESS)
		ao_bFound = M4_TRUE;
	else
		ao_bFound = M4_FALSE;

	return M4_SUCCESS;
}

// Put Object into Cache, but don't persit object into disk
m4return_t ClExecutorStateCache::PutObject (m4pchar_t ai_pcIdSession, ClExecutorState* ao_poObject, m4date_t ai_dMaxPeriod)
{
	m4return_t ret = M4_SUCCESS;

	ret = _PutObject(ai_pcIdSession, 0, ao_poObject, ai_dMaxPeriod, 0, M4_FALSE);
	
	return ret;
}

m4return_t ClExecutorStateCache::ReplaceObject (ClExecutorState* ao_poOldObject, m4pchar_t ai_pcIdSession, ClExecutorState* ao_poNewObject, m4date_t ai_dMaxPeriod)
{
	m4return_t ret = M4_SUCCESS;

	ret = _ReplaceObject(ao_poOldObject, ai_pcIdSession, 0, ao_poNewObject, ai_dMaxPeriod, 0, M4_FALSE);
	
	return ret;
}

// removes all other objects in the current session and excludes the specified one
m4return_t ClExecutorStateCache::RemoveAllOtherObjectsInSession(string &ai_sSubSessionId)
{
	// computes the key to preserve
	int iPos = ai_sSubSessionId.find("-");
	if (iPos < 0) 
	{
		return M4_ERROR;
	}

	// creates the matcher
	string sPrefix = ai_sSubSessionId.substr(0, iPos + 1);
	ClExecutorStateDataMatcherById oMatcher(sPrefix, ai_sSubSessionId);

	// deletes objects
	return m_poCache->RemoveByCondition(&oMatcher);
}

// Remove objects from cache for a given Id
m4return_t ClExecutorStateCache::RemoveObjectById( m4pchar_t ai_pcId, m4puint32_t ai_piRemovedCount)
{
	// Esta funcion es reentrante... porque la Cache lo es.

	//Componemos la parte de la clave que lleva el id de la presentacion.
	m4return_t ret = M4_SUCCESS;

	if (!ai_pcId)
		return M4_ERROR;

	m4pchar_t pcId = new m4char_t[ strlen(ai_pcId) + 2];

	//sprintf(pcId, "%s@", ai_pcId);
	strcpy(pcId, ai_pcId);
	strupr(pcId);

    //Hacemos remove de todos los objetos que tengan en su clave la cadena.
	ret = m_poCache->RemoveAll(pcId, ai_piRemovedCount);	// Esta funcion es reentrante

	delete [] pcId;	

	return ret;
}

// Remove objects from cache for a given Id and Version
m4return_t ClExecutorStateCache::RemoveObjectByIdAndVersion( m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType )
{
	// Esta funcion es reentrante... porque la Cache lo es.

	m4return_t ret = M4_SUCCESS;

	ret = RemoveObjectById( ai_pcId );

	return ret;
}

