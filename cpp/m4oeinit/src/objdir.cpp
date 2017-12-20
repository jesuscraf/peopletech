//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                ObjDir.cpp
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
//    Storing other objects in serialization
//    
//
//==============================================================================

#include "access.hpp"
#include "arecset.hpp"
#include "bnihil.hpp"
#include "chan_knl.hpp"
#include "channel.hpp"
#include "cldmcontext.hpp"	// M4DM
#include "cllgadap.hpp"
#include "cs_result.hpp"
#include "csres.hpp"
#include "cstracesize.hpp"
#include "dm.hpp"
#include "dsres.hpp"
#include "execntxt.hpp"
#include "executor.hpp"
#include "groupinfo.hpp"
#include "m4cstrace.hpp"
#include "m4lgadapdef.hpp"
#include "m4mdfac.hpp"
#include "m4mdrt.hpp"
#include "node_knl.hpp"
#include "objdir.hpp"
#include "recorset.hpp"
#include "sttlib.hpp"
#include "sttwrap.hpp"		// M4DM
#include "vmstepper.hpp"
#include "objdatacacheinterface.hpp"
#include "m4unicode.hpp"
#include "usractaudit.hpp"

/**
 * Si la línea de abajo está descomentada se utilizará un cronómetro para medir los tiempos de la serialización
 * y la deserialización. Los tiempos tomados se iran añadiendo al fichero definido en GET_SERIAL_TIME_FILE.
 * En este fichero en cada línea encotramos dos columnas la primera indica si es una serialización (S) o una
 * deserialización (D), y la segunda indica el tiempo en milisegundos.
 */
//#define GET_SERIAL_TIME 1
#ifdef GET_SERIAL_TIME
#define GET_SERIAL_TIME_FILE "c:\\temp\\SerialTime.txt"
#include "m4stl.hpp"
#include "sttcommon.hpp"
#endif


#define M4CL_OD_MAX_LONG_STR	50
#define M4_OD_INIT_OBJECTS	16


// ----------------------------------------------------------
// class PtrWithId
// ----------------------------------------------------------

PtrWithId::PtrWithId(void) :
	m_poOId(NULL)
{
	Init();
}

void PtrWithId::Init(m4bool_t ai_bDestroy /*= M4_TRUE*/)
{
	m_poPtr                = 0;
	
	if (ai_bDestroy == M4_TRUE) delete m_poOId;
	
	m_poOId                = 0;
	m_bCreatedByDirectory  = M4_FALSE;
	m_bDoSerialize         = M4_TRUE;
	m_iSerializedSize      = 0;
	
	m_iRemoteId            = 0;

	m_iIsDebug             = ClChannel::IS_NOT_DEBUG;
	m_iBlockToSerialize    = 0;
	m_iRegisterToSerialize = 0;
	m_pChannelProc         = 0;
}

m4return_t PtrWithId::DumpInfo(ostream &ai_oStream)
{
	ai_oStream << endl;
	ai_oStream << "     m_poPtr                 = <" << m_poPtr                << ">" << endl;
	ai_oStream << "     m_poOId                 = <" << m_poOId                << ">" << endl;
	ai_oStream << "     m_bCreatedByDirectory   = <" << m_bCreatedByDirectory  << ">" << endl;
	ai_oStream << "     m_bDoSerialize          = <" << m_bDoSerialize         << ">" << endl;
	ai_oStream << "     m_iSerializedSize       = <" << m_iSerializedSize      << ">" << endl;
	ai_oStream << "     m_iRemoteId             = <" << m_iRemoteId            << ">" << endl;
	ai_oStream << "     m_iIsDebug              = <" << m_iIsDebug             << ">" << endl;
	ai_oStream << "     m_iBlockToSerialize     = <" << m_iBlockToSerialize    << ">" << endl;
	ai_oStream << "     m_iRegisterToSerialize  = <" << m_iRegisterToSerialize << ">" << endl;
	ai_oStream << "     m_pChannelProc          = <" << m_pChannelProc         << ">" << endl;

	// dumps the ID info
	if (m_poOId != NULL)
		ai_oStream << *m_poOId;

	ai_oStream << endl;

	return M4_SUCCESS;
}

ostream & operator << (ostream &ao_Output, PtrWithId &ai_oPtrWithId)
{
	m4return_t iRes = ai_oPtrWithId.DumpInfo(ao_Output);
	if (iRes != M4_SUCCESS)
		ao_Output << endl << "     Error dumping PtrWithId Object." << endl;
	return ao_Output;
}

// ----------------------------------------------------------
// class ClCMObjectDirectory
// ----------------------------------------------------------

// 2005-04-04 se serializa el estado de Np de la licencia dentro del objdir
//#define CLCMOBJECTDIRECTORY_SERIALIZE_VERSION 19
// 2005-06-08 Mejora de ancho de banda. Se optimiza la serialización de enteros
//#define CLCMOBJECTDIRECTORY_SERIALIZE_VERSION 20
// 2009-10-21 USer actions auditory.
// #define CLCMOBJECTDIRECTORY_SERIALIZE_VERSION 21
// 2015-12-21 User actions time auditory.
#define CLCMOBJECTDIRECTORY_SERIALIZE_VERSION 22


// 21/10/98 Cambios en access, se envia/reciben las fechas de ejecución...
//#define CLACCESS_SERIALIZE_VERSION 3
// 2005-06-06 Mejora de ancho de banda. No se deserializa la posición del item
#define CLACCESS_SERIALIZE_VERSION 4




ClCMObjectDirectory::ClCMObjectDirectory(ClChannelManager* ai_oChannelManager)
	: m_iSize(M4_OD_INIT_OBJECTS)
{
	m_oObjectPtr = new PtrWithId [m_iSize];
	
	Init(ai_oChannelManager);

	m_bProhibitL2RelationSerialize = M4_FALSE;
}


m4return_t ClCMObjectDirectory::Init(ClChannelManager* ai_oChannelManager)
{
	m_poChannelManager = ai_oChannelManager;
	
	// Versiones de serialización
	m_iSerializationVersion = CLCMOBJECTDIRECTORY_SERIALIZE_VERSION +
							  CLACCESS_SERIALIZE_VERSION +
							  ClChannel::GetSerializationVersion() +
							  ClOLTPExecutionContext::GetSerializationVersion();
	
	for (m4uint16_t i = UndefinedObjectType; i < LastObjectType; i++) m_iObjTypeCount[i] = 0;
	
	m_iCount = 0;
	m_iLastObjectIndex = 0;
	m_iLastObjectType  = UndefinedObjectType;
	m_pLastChannelName[0] = 0;
	SetForceL2RelationSerialize(M4_FALSE);
	SetKeepChannelCsTypeOnRemote(M4_FALSE);
		
	return M4_SUCCESS;
}
 
m4return_t ClCMObjectDirectory::Reset(ClChannelManager* ai_oChannelManager)
{
    _RemoveAllObj();

    return Init(ai_oChannelManager);
}

ClCMObjectDirectory::~ClCMObjectDirectory(void)
{
    _RemoveAllObj();

    delete[] m_oObjectPtr;
}

void ClCMObjectDirectory::RemoveNonChannelObjects(void)
{
	vector<ClLevel2Relation>::iterator i;
	m4bool_t bDelete;
	
	for (m4int16_t iIndex = m_iCount-1; iIndex >= 0 ; iIndex--) {
		// Los objetos los borro si los ha creado el Directory
		if (m_oObjectPtr[iIndex].m_bCreatedByDirectory == M4_TRUE) {
			if (m_oObjectPtr[iIndex].m_poPtr != NULL) {
				switch ( m_oObjectPtr[iIndex].m_poOId->GetType() ) {
				case ChannelObjectType:
					break;
					
				case AccessObjectType:
					// deletes all but L2 relations
					bDelete = M4_TRUE;
					for (i = m_oLevel2RelationVector.begin(); i != m_oLevel2RelationVector.end(); i++) {
						if ((*i).m_iAccessIndex == iIndex) {
							bDelete = M4_FALSE;
						}
					}
						
					if (bDelete) {
						Remove(iIndex);
					}
					break;
						
				default:
					Remove(iIndex);
					break;
				}
			}
		}
	}
}

void ClCMObjectDirectory::_RemoveAllObj(void)
{
	// El ChannelEraser no borra los accesos normales, asi que estos los tinen que borrar
	// el objDir. Lo que voy a hacer es decir al objDir que no se haga responsable de los
	// accesos de nivel2, de esta forma elimino todos los accesos que tenga en el objDir
	// y los de nivel2 ya los borrara el ChannelEraser
	vector<ClLevel2Relation>::iterator i = m_oLevel2RelationVector.begin();
	for (; i != m_oLevel2RelationVector.end(); i++) {
		m_oObjectPtr[(*i).m_iAccessIndex].m_bCreatedByDirectory = M4_FALSE;
	}

	for (m4int16_t iIndex = m_iCount-1; iIndex >= 0 ; iIndex--) {
		Remove(iIndex);
	}

	// Elimino todas las relaciones de nivel2
	m_oLevel2RelationVector.clear();
}

void ClCMObjectDirectory::Remove(const m4int16_t ai_iIndex)
{
	M4_ASSERT(ai_iIndex >= 0 && ai_iIndex < m_iCount);

	eObjectType eType = m_oObjectPtr[ai_iIndex].m_poOId->GetType();
	ClChannel *poChannel = NULL;
	ClAccess *poAccess = NULL;
	ClOLTPExecutionContext *poContext = NULL;
	ClClientServerResult *poResult = NULL;


	if( m_oObjectPtr[ai_iIndex].m_poPtr != NULL )
	{
		// Los objetos los borro si los ha creado el Directory
		if (m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory == M4_TRUE)
		{
			switch (eType) {
			case MetaChannelObjectType:
				((ClCompiledMCR*)(m_oObjectPtr[ai_iIndex].m_poPtr))->Destroy();
				break;
				
			case ChannelObjectType:
				{
					poChannel = (ClChannel*) m_oObjectPtr[ai_iIndex].m_poPtr ;

					/* Bug 0105827
					Puede que el canal se haya borrado por los enganches de nivel 2
					*/
					m4int32_t	iPosition = FindChannelInCM( poChannel ) ;

					if( iPosition != -1 )
					{
						ClChannelEraser oChannelEraser;
						oChannelEraser.Add(poChannel);
						oChannelEraser.Destroy();
					}
				}
				break;
				
			case AccessObjectType:
				poAccess = (ClAccess*)m_oObjectPtr[ai_iIndex].m_poPtr;
				poAccess->Destroy();
				break;
				
			case OLTPExecutionContextType:
				poContext = (ClOLTPExecutionContext*)m_oObjectPtr[ai_iIndex].m_poPtr;
				delete poContext;
				break;
				
			case ClientServerResultObjectType:
				poResult = (ClClientServerResult*)m_oObjectPtr[ai_iIndex].m_poPtr ;
				delete poResult;
				break;
			}
		}
	}
	
	// Limpio toda la informacion referente al objeto
	m_iObjTypeCount[eType]--;
	m_oObjectPtr[ai_iIndex].Init();
	
	m_iCount--;
	
	// No permitimos que se queden huecos
	if (ai_iIndex < m_iCount) {
		memmove(&m_oObjectPtr[ai_iIndex], &m_oObjectPtr[ai_iIndex + 1], (m_iCount - ai_iIndex) * sizeof(PtrWithId));
	}
}

/**
 * Añade un nuevo objeto en el array. Si fuera necesario duplica el tamaño del array.
 *
 * @param ai_iIndex	Posición donde se quiere insertar el objeto en el array.
 */
void ClCMObjectDirectory::_Insert(m4uint16_t ai_iIndex)
{
	M4_ASSERT (ai_iIndex <= m_iCount);

	if (m_iCount == m_iSize) {
		PtrWithId* pAuxPtrWithId = m_oObjectPtr;
		m_iSize <<= 1;	// Multiplicamos por 2.
		m_oObjectPtr = new PtrWithId [m_iSize];
		memcpy(m_oObjectPtr, pAuxPtrWithId, m_iCount * sizeof(PtrWithId));
		delete[] pAuxPtrWithId;
	}
	
	if (ai_iIndex < m_iCount) {
		memmove(&m_oObjectPtr[ai_iIndex + 1], &m_oObjectPtr[ai_iIndex], (m_iCount - ai_iIndex) * sizeof(PtrWithId));
		m_oObjectPtr[ai_iIndex].Init(M4_FALSE);
	}

	m_iCount++;
}

void ClCMObjectDirectory::Swap(m4uint16_t iNewIndex, m4uint16_t iOldIndex)
{
   M4_ASSERT (iNewIndex < m_iCount);
   M4_ASSERT (iOldIndex < m_iCount);

   PtrWithId aux = m_oObjectPtr[iNewIndex];
   m_oObjectPtr[iNewIndex] = m_oObjectPtr[iOldIndex];
   m_oObjectPtr[iOldIndex] = aux;
}

m4int16_t ClCMObjectDirectory::GetObjectIndex(const void* const ai_pObject) const
{
	for (m4uint16_t i = 0; i < m_iCount; i++) {
		if (m_oObjectPtr[i].m_poPtr == ai_pObject) {
			return i;
		}
	}
	
	return -1;
}

m4int32_t ClCMObjectDirectory::FindChannelInCM(m4uint32_t ai_iHandle)
{
	m4int32_t ChannelCount = m_poChannelManager->Channel.Count();
	
	for (m4int32_t i = 0; i < ChannelCount; i++) {
		if (m_poChannelManager->Channel[i]->GetHandle() == ai_iHandle) {
			return i;
		}
	}
	
	return -1;
}

/* Bug 0105827
Hay que poder buscar por el puntero y no por el handle, porque sino existe no
se puede acceder al handle
*/
m4int32_t ClCMObjectDirectory::FindChannelInCM(ClChannel *ai_poChannel)
{
	m4int32_t ChannelCount = m_poChannelManager->Channel.Count();
	
	for (m4int32_t i = 0; i < ChannelCount; i++) {
		if (m_poChannelManager->Channel[i] == ai_poChannel) {
			return i;
		}
	}
	
	return -1;
}

m4int16_t ClCMObjectDirectory::GetFirstObjectIndex(const m4ObjectType_t  ai_eObjectType, const m4char_t* const ai_string)
{
	m_iLastObjectIndex = -1;
	
	for (m4int16_t i = 0; i < m_iCount; i++)
	{
		if (m_oObjectPtr[i].m_poOId->GetType() == ai_eObjectType)
		{ 
			if (ai_string) {
				if (strcmp(m_oObjectPtr[i].m_poOId->GetTypedId(), ai_string) == 0)
				{
					strcpy(m_pLastChannelName, ai_string);
					
					m_iLastObjectIndex = i;
					m_iLastObjectType = ai_eObjectType;
					
					break;
				}
			}
			else
			{
				m_pLastChannelName[0] = 0;
				
				m_iLastObjectIndex = i;
				m_iLastObjectType = ai_eObjectType;
				
				break;
			}
		}
	}
	
	return m_iLastObjectIndex;
}


m4int16_t ClCMObjectDirectory::GetNextObjectIndex(void)
{
	m4int16_t i = m_iLastObjectIndex + 1;
	
	m_iLastObjectIndex = -1; 
	
	for ( ; i < m_iCount; i++)
	{
		if (m_oObjectPtr[i].m_poOId && m_oObjectPtr[i].m_poOId->GetType() == m_iLastObjectType)
		{
			if (m_pLastChannelName[0])
			{
				if (strcmp(m_oObjectPtr[i].m_poOId->GetTypedId(), m_pLastChannelName) == 0)
				{
					m_iLastObjectIndex=i;
					break;
				}
			}
			else
			{
				m_iLastObjectIndex = i;
				break;
			}
		}
	}
	
	return m_iLastObjectIndex;
}

m4int16_t ClCMObjectDirectory::Find(const ClObjectId& ai_ObjectId)
{
	for (m4uint16_t i = 0; i < m_iCount; i++) {
		if (*m_oObjectPtr[i].m_poOId == ai_ObjectId) {
			return i;
		}
	}
		
	return -1;
}

m4return_t ClCMObjectDirectory::SetObjectAddress(const ClObjectId& ai_roObjectId, void* ai_poPtr)
{
   m4int16_t iIndexOfObject;

   if ((iIndexOfObject = Find(ai_roObjectId)) == -1)
       return M4_ERROR;

   m_oObjectPtr[iIndexOfObject].m_poPtr = ai_poPtr;

   return M4_SUCCESS;
}

ClObjectId* ClCMObjectDirectory::CreateMetaChannelOId(const ClCompiledMCR& ai_pCompiledMCR, const ClChannel& ai_pChannel)
{
	// ??? Create on object ID which refers to the MetaChannel
   // ??? Esto lo hago por ahora ya que los objetos no tienen identificadores asociados

   m4char_t id[M4CL_OD_MAX_LONG_STR];

   /* Bug 0100859
   Los metacanales de canales dinámicos se deben distinguir entre dos instancias distintas
   del mismo canal dinámico. No vale poner solo el nombre, hay que poner la versión
   */
   ClDateToString( ai_pCompiledMCR.GetChannelVersion(), id, M4_SECOND_END + 1 ) ;
   ClEnvironmentId oEnvironmentId( id ) ;

   // Preparamos el TypedId = Nombre del canal
   ClTypedId oTypedID ( ai_pCompiledMCR.GetChannelId() );

   // Preparamos el UniqueKey = Handle del canal
   sprintf(id, "%d", ai_pChannel.GetHandle());

   ClUniqueKey oUniqueKey( id ) ;

   // Componemos el OID
   ClObjectId* pObjID = new ClObjectId(MetaChannelObjectType, &oEnvironmentId, &oTypedID, &oUniqueKey);

   return pObjID;
}

ClObjectId* ClCMObjectDirectory::CreateChannelOId(const ClChannel& ai_pChannel)
{
   m4char_t id[M4CL_OD_MAX_LONG_STR];

	// ??? Create on object ID which refers to the channel
   // ??? Esto lo hago por ahora ya que los objetos no tienen identificadores asociados

   // Preparamos el EnvironmentId = Fecha del metacanal
   // ??? ojo que esto es un vil truco, estoy metiendo en el environment las fecha del metacanal
   // ??? habra que areglarlo, pero por ahora parece que hay prisa
   ClChannel* pAuxChannel=(ClChannel*)&ai_pChannel;
   /*
   Bug 0083569
   La fecha se manda ahora en formato ISO
   */
   ClDateToString( pAuxChannel->GetpCMCR()->GetChannelVersion(), id, M4_SECOND_END + 1 ) ;
   ClEnvironmentId oEnvironmentId( id ) ;

   // Preparamos el TypedId = Nombre del canal
   ClTypedId oTypedID ( ai_pChannel.GetpChannelDef()->Id() ) ;

   // Preparamos el UniqueKey = Handle del canal
   sprintf(id, "%d", ai_pChannel.GetHandle());

   ClUniqueKey oUniqueKey( id ) ;    

   // Componemos el OID
   ClObjectId* pObjID = new ClObjectId(ChannelObjectType, &oEnvironmentId, &oTypedID, &oUniqueKey);

   return pObjID;
}

ClObjectId* ClCMObjectDirectory::CreateAccessOId(const ClChannel&     ai_pChannel,
                                                 const ClAccessIndex& ai_accessIndex)
{
   m4char_t id[M4CL_OD_MAX_LONG_STR];

	// ??? Create on object ID which refers to the channel
   // ??? Esto lo hago por ahora ya que los objetos no tienen identificadores asociados

   // Preparamos el EnvoironmentId = Handle del canal
   sprintf(id, "%d", ai_pChannel.GetHandle() );
   
   ClEnvironmentId oEnvironmentId( id ) ;

   // Preparamos el TypedId = Nombre del canal
   ClTypedId oTypedID( ai_pChannel.GetpChannelDef()->Id() ) ;

   // Preparamos el UniqueKey = Index del indice en el canal
   sprintf(id, "%d", (m4uint32_t)ai_accessIndex);

   ClUniqueKey oUniqueKey( id ) ;    

   // Componemos el OID
   ClObjectId* pObjID = new ClObjectId(AccessObjectType, &oEnvironmentId, &oTypedID, &oUniqueKey);

   return pObjID;
}

ClObjectId* ClCMObjectDirectory::CreateOLTPExecutionContextOId(void)
{
	// ??? Create on object ID which refers to the OLTPExecutionContext
   // ??? Esto lo hago por ahora ya que los objetos no tienen identificadores asociados

   // Preparamos el TypedId = "OLTPExecutionContext"
   ClTypedId oTypedID ("OEC");

   // Componemos el OID
   ClObjectId* pObjID = new ClObjectId(OLTPExecutionContextType, 0, &oTypedID, 0);

   return pObjID;
}

ClObjectId* ClCMObjectDirectory::CreateClientServerResultOId(void)
{
	// ??? Create on object ID which refers to the OLTPExecutionContext
   // ??? Esto lo hago por ahora ya que los objetos no tienen identificadores asociados

   // Preparamos el TypedId = "ClientServerResult"
   ClTypedId oTypedID ("CSR");

   // Componemos el OID
   ClObjectId* pObjID = new ClObjectId(ClientServerResultObjectType, 0, &oTypedID, 0);

   return pObjID;
}

/**
 * Añade un canal al objDir y le crea un OID para identificarlo a la vuelta.
 * Si el canal ya estaba en el objDir no se añade (no se permiten repeticiones).
 * Podemos especificarle el identificador remoto y si queremos que se serialize a la ida.
 *
 * @param ai_pChannel		Canal que queremos añadir.
 * @param ai_iRemoteId		Identificador remoto del canal (ej para proxy o depuración).
 * @return Devuelve el índice donde esta el canal dentro del objDir. -1 si no se ha podido añadir.
 */
m4int16_t ClCMObjectDirectory::IncludeChannel(ClChannel* const ai_pChannel, m4uint32_t ai_iRemoteId)
{
	M4_ASSERT (ai_pChannel);
	
	PtrWithId oPtrWithId;
	
	oPtrWithId.m_poPtr     = ai_pChannel;
	oPtrWithId.m_iRemoteId = ai_iRemoteId;
	
	// Lo primero de todo es comprobar que no esta en el directorio
	m4int16_t iChannelIndex = GetObjectIndex(ai_pChannel);

	/* Bug 0068822
	Aunque el canal exista hay que insertar sus amigos
	*/
	if (iChannelIndex == -1)
	{
		// Tenemos que comprobar si es un canal dinamico para incluir el metacanal
		ClCompiledMCR* pBackMCR = ai_pChannel->GetpBackMCR();
		if (pBackMCR) {
			if (IncludeMetaChannel(pBackMCR, ai_pChannel) == -1) {
				return -1;
			}
		}
		
		// Creamos OID para el Channel
		oPtrWithId.m_poOId = CreateChannelOId(*ai_pChannel);
		if (!oPtrWithId.m_poOId) return -1;
		
		oPtrWithId.m_iIsDebug = ai_pChannel->GetDebugMode();
		
		iChannelIndex = _Include(oPtrWithId);
	}
	
	m4bool_t bExecutingInClient = M4_FALSE ;

	if( m_poChannelManager != NULL )
	{
		bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient() ;
	}

	// Si el canal se ha incluido correctamente tengo que intentar añadir a sus amigos
	// Esto da soporte a transacciones multicanal, pero no del server al cliente
	if (iChannelIndex != -1 && bExecutingInClient == M4_TRUE) {
		if (InsertAllPartnersInObjDir(ai_pChannel) != M4_SUCCESS ) {
			return -1;
		}
	}
        
	return iChannelIndex;
}

/**
 * Comprueba si el canal existe.
 * Puede que el canal se borrase por ser una instancia de nivel2 (ej, creo la instancia en cliente y en el
 * servidor llamo a EraseAllLevel2Instances). Para comprobar esto voy a buscar el canal en el channelManager.
 * Esto sólo lo voy a hacer cuando estoy en el servidor y yo soy el dueño del objeto (yo lo he creado).
 *
 * Cuando encuentro un canal borrado voy a inhabilitarlo para que el objDir no intente operar con él (voy a hacer lo
 * mismo con sus accesos). También voy a poner a 0 su remoteId ya qus si se trata de un proxy no lo voy a encontrar
 * en las siguientes transacciones.
 *
 * @param ai_i	Índice en el objDir del canal sobre el que se quiere hacer al comprobación.
 */
void ClCMObjectDirectory::CheckChannelExistBeforeSerial(m4uint16_t ai_i)
{
	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();
	if (bExecutingInClient == M4_TRUE) return;

	// Si el flag m_bCreatedByDirectory está a M4_FALSE todavía no me puedo fiar porque puede tratarse de un canal proxy.
	// El objDir se desentiende de estos canales al crearlos, sin embargo también hay que hacer la comprobación en estos canales.
	if (m_oObjectPtr[ai_i].m_poPtr && (m_oObjectPtr[ai_i].m_bCreatedByDirectory == M4_TRUE || m_oObjectPtr[ai_i].m_iRemoteId)) {

		/* Bug 0105827
		Si no existe es mejor no acceder al handle y si al puntero
		*/
		m4int32_t i = FindChannelInCM( (ClChannel*)m_oObjectPtr[ai_i].m_poPtr );
		if (i == -1) {

			//Bug:0063274   Begin  -----------------------------------------------
			// La función ClCMObjectDirectory::SerializeL2Relation no comprueba si las relaciones del canal que va a serializar hacia
			//  el cliente pueden pertenecer a un canal que ha sido destruido.
			// El mensaje de error que se vuelca solo tiene sentido si no estamos depurando el canal, puesto que en caso de depuración
			//  en el cliente tenemos la vista back del canal, al igual que en la parte sever
			if(m_oObjectPtr[ai_i].m_iIsDebug == ClChannel::IS_NOT_DEBUG) {
			//Bug:0063274   End    -----------------------------------------------
				// Reportamos el error.
				DUMP_CHLOG_ERRORF(M4_CS_SERIALIZE_DELETED_L2, m_oObjectPtr[ai_i].m_poOId->GetTypedId());
			}

			// Como el canal se ha borrado lo inhabilito.
			m_oObjectPtr[ai_i].m_poPtr = 0;
			m_oObjectPtr[ai_i].m_bDoSerialize = M4_FALSE;
			m_oObjectPtr[ai_i].m_iRemoteId = 0;
			
			
			// También tengo que buscar si había algún acceso que perteneciera a este chanal,
			// porque también estará borrado. No puedo acceder al acceso para comprobar el puntero
			// al canal, así que se comprobará usando el OID.

			// Como voy a utilizar las funciones GetFirs y GetNext guardo los valores anteriores para no fastidiar
			// al que me pueda estar usando.
			const m4int16_t      iObjectIndex = m_iLastObjectIndex;
			const m4ObjectType_t iObjectType  = m_iLastObjectType;

			const m4char_t* const pszClientChannelHandleInChannelOID = m_oObjectPtr[ai_i].m_poOId->GetUniqueKey();
			const m4char_t* pszClientChannelHandleInAccessOID = 0;
			for (i = GetFirstObjectIndex(AccessObjectType); i != -1; i = GetNextObjectIndex()) {
				pszClientChannelHandleInAccessOID = m_oObjectPtr[i].m_poOId->GetEnvironmentId();
				
				if (!strcmp(pszClientChannelHandleInAccessOID, pszClientChannelHandleInChannelOID)) {
					// Este acceso también está borrado. Hay que deshabilitarlo.
					m_oObjectPtr[i].m_poPtr = 0;
					m_oObjectPtr[i].m_bDoSerialize = M4_FALSE;
					m_oObjectPtr[i].m_iRemoteId = 0;
				}
			}

			m_iLastObjectIndex = iObjectIndex;	// Se ha visto alterado al iterar por los accesos.
			m_iLastObjectType  = iObjectType;	// Se ha visto alterado al iterar por los accesos.
		}
	}
}

/**
 * Comprueba si es un canal de depuración.
 * Dependiendo del modo de depuración, fija el modo de serialización de los ítems y de los regsitros.
 *
 * @param ai_i	Índice en el objDir del canal sobre el que se quiere hacer al comprobación.
 */
void ClCMObjectDirectory::CheckDebugModeAndSetSerialMode(m4uint16_t ai_i)
{
	ClChannel* const pChannel = (ClChannel*)m_oObjectPtr[ai_i].m_poPtr;
	if (!pChannel) return;

	const ClChannel::eDebugMode_t eDebugMode = pChannel->GetDebugMode();

	switch (eDebugMode) {
	case ClChannel::IS_NOT_DEBUG:
		// Si NO es un canal de debug respeto el modo de serializacion que ha
		// fijado el clexadap según el syncType del ítem.
		break;
		
	case ClChannel::IS_DEBUG_FIRST_TIME:
		{
			m4uint8_t iCsExeType = pChannel->GetpChannelDef()->CsExeType() ;

			m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();
			if (bExecutingInClient == M4_FALSE)
			{
				// Si el canal está en el modo de depuración IS_DEBUG_FIRST_TIME y estoy serializando en el servidor es que
				// no se ha parado en ningún breakpoint. En este caso devuelvo el canal normal y le quito el modo de depuración.
				M4_ASSERT(m_oObjectPtr[ai_i].m_bDoSerialize == M4_TRUE);

				pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_DELTA);
				
				pChannel->SetDebugMode(ClChannel::IS_DEBUG_LAST_TIME);
				m_oObjectPtr[ai_i].m_iIsDebug = ClChannel::IS_DEBUG_LAST_TIME;

				// Como no he parado en ningún breakpoint no se va a crear canal de depuración en el cliente, así que
				// reseteo los remoteId de los objetos (canales y sus accesos) OLTP
				// Para PROXY no hay que resetear
				// Para AUTOS hay que resetear sólo los accesos y dejar los canales
				if (iCsExeType == M4CL_CSEXETYPE_OLTP || iCsExeType == M4CL_CSEXETYPE_AUTO)
				{
					if( iCsExeType == M4CL_CSEXETYPE_OLTP )
					{
						m_oObjectPtr[ai_i].m_iRemoteId = 0;
					}
					
					ClAccess* pAccess = 0;
					m_iLastObjectIndex = ai_i;
					m_iLastObjectType = AccessObjectType;
					for (m4int16_t i = GetNextObjectIndex(); i != -1; i = GetNextObjectIndex()) {
						pAccess = (ClAccess*)m_oObjectPtr[i].m_poPtr;
						if (pAccess->GetpChannel() == pChannel) {
							m_oObjectPtr[i].m_iRemoteId = 0;
						}
					}
				}
			}
			else
			{
				// Cuando mando por primera vez un canal de depuracion del cliente al servidor lo hago como siempre.
				if (iCsExeType == M4CL_CSEXETYPE_OLTP)
				{
					pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ALL);
				}
				else if (iCsExeType == M4CL_CSEXETYPE_AUTO)
				{
					// Si es auto se mandan deltas
					pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_DELTA);
				}
				else
				{
					pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_DELTA);
				}
			}
			pChannel->SetItemSerializationMode(M4CL_ITEM_SERIAL_MAPPED);
		}
		break;

	case ClChannel::IS_DEBUG_SECOND_TIME:
		// Estoy parado en un breakpoint, como es la primera vez no mando deltas, sino un absoluto.
		// Esto sólo tiene sentido cuando vuelvo del servidor.
		M4_ASSERT(m_poChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE);

		pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ALL);
		pChannel->SetItemSerializationMode(M4CL_ITEM_SERIAL_ALL);
		
		pChannel->SetDebugMode(ClChannel::IS_DEBUG);
		m_oObjectPtr[ai_i].m_iIsDebug = ClChannel::IS_DEBUG;
		break;
		
	case ClChannel::IS_DEBUG:
		// Como no es la primera vez que lo mando, sólo mando los registros que han cambiado.
		pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_DELTA);
		pChannel->SetItemSerializationMode(M4CL_ITEM_SERIAL_ALL);
		break;
 
	case ClChannel::IS_DEBUG_LAST_TIME:
		{
			// Estoy haciendo la última transacción para sincronizar la parte front y la back.
			// Desde el servidor hago una serialización normal, desde el cliente no se serializa nada.
			m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();

			if (bExecutingInClient == M4_FALSE)
			{
				pChannel->SetRecordSerializationMode(M4CL_RECORD_SERIAL_DELTA);
				pChannel->SetItemSerializationMode(M4CL_ITEM_SERIAL_MAPPED);
			}
			else
			{
				M4_ASSERT(m_oObjectPtr[ai_i].m_bDoSerialize == M4_FALSE);
			}
		}
		break;

	default:
		M4_ASSERT(0);
	}
}

// Insertar los canales amigos(partners) para un canal dado(ai_pChannel) en el Object Directory(this)
m4return_t ClCMObjectDirectory::InsertAllPartnersInObjDir(ClChannel *ai_pChannel)
{
	m4int16_t iChannelIndex;
	if (!ai_pChannel) return M4_ERROR;

	vector<ClChannel*>::iterator i;
	vector<ClChannel*>& vPartner = ai_pChannel->m_oGroupInfo.getPartnerVector();
	
	for (i = vPartner.begin(); i != vPartner.end(); i++) {
		/* Bug 0297803
		Si el partner está ya en el directorio no lo volvemos a incluir
		(puede provocar Stack Overflow por dependencias circulares)
		*/
		iChannelIndex = GetObjectIndex(*i);
		if (iChannelIndex == -1) {
			/* Bug 0102234
			Hay que poner el remoteid para los autos
			*/
			if (IncludeChannel(*i, (*i)->GetRemoteId()) == -1) {
				return M4_ERROR;
			}
		}
	}
	
	return M4_SUCCESS;
}

m4int16_t ClCMObjectDirectory::IncludeMetaChannel(ClCompiledMCR* ai_pCompiledMCR, ClChannel* ai_pChannel)
{
   M4_ASSERT(ai_pCompiledMCR);

    // Lo primero de todo es comprobar que no esta en el directorio
    m4int16_t iIndex = GetObjectIndex(ai_pCompiledMCR);
    if (iIndex != -1)
        return iIndex;    // YA EXISTE !!!

   PtrWithId oPtrWithId;
   oPtrWithId.m_poPtr = ai_pCompiledMCR;

   // Creamos OID para el MetaChannel
   oPtrWithId.m_poOId = CreateMetaChannelOId(*ai_pCompiledMCR, *ai_pChannel);
   if (oPtrWithId.m_poOId == 0)
      return -1;

   return _Include(oPtrWithId);
}

m4int16_t ClCMObjectDirectory::_Include(PtrWithId& ai_oPtrWithId)
{
    M4_ASSERT(ai_oPtrWithId.m_poPtr);
    M4_ASSERT(ai_oPtrWithId.m_poOId);

    m4int16_t iNewIndex = m_iCount;

    _Insert(iNewIndex);
    
    m_oObjectPtr[iNewIndex] = ai_oPtrWithId;
    
    // TRATAMIENTO ESTANDAR, PARA OTRO SITIO ...
    m_iObjTypeCount[ai_oPtrWithId.m_poOId->GetType()]++;
    
    return iNewIndex;
}

m4int16_t ClCMObjectDirectory::IncludeAccessAndCreateOID(ClAccess* const ai_poAccess, m4uint32_t ai_iRemoteId)
{
	M4_ASSERT(ai_poAccess);
	
	// Lo primero de todo es comprobar que no esta en el directorio
	m4int16_t iAccessIndex = GetObjectIndex(ai_poAccess);
	if (iAccessIndex != -1) {
		return iAccessIndex;    // YA EXISTE !!!
	}
	
	// Ahora tengo que buscar el channel del access
	// Error si no lo encuentro, el Channel siempre debe añadirse antes que sus Access
	ClChannel* poChannel = ai_poAccess->GetpChannel();
	M4_ASSERT(poChannel);
	
	m4int16_t iChannelIndex = GetObjectIndex(poChannel);
	if (iChannelIndex == -1) return -1;   // No se encontro el channel del access
	
	// Busco en el canal cual es la posicion que ocupa el access
	m4uint32_t iAccessCount = poChannel->Access.Count();
	for (ClAccessIndex i = 0; i < iAccessCount; i++)
	{
		if (poChannel->Access[i] == ai_poAccess)
		{
			PtrWithId oPtrWithId;
			oPtrWithId.m_poPtr = ai_poAccess;
			
			// El id remoto.
			if (!ai_iRemoteId)
			{
				oPtrWithId.m_iRemoteId = ai_poAccess->GetRemoteId();
			}
			else
			{
				oPtrWithId.m_iRemoteId = ai_iRemoteId;
			}

			// Creamos OID para el Access
			oPtrWithId.m_poOId = CreateAccessOId(*poChannel, i);
			if (!oPtrWithId.m_poOId) return -1;
			
			return _Include(oPtrWithId);
		}
	}
	
	M4_ASSERT(0);
	return -1;     // No se ha encontrado el access en el channel
}

m4return_t ClCMObjectDirectory::IncludeAllAccessAndCreateOID(ClChannel& ai_roChannel, m4bool_t ai_bIncludeAlways)
{
	// Busco en el Directory el Channel
	// Error si no lo encuentro, el Channel siempre debe añadirse antes que sus Access
	m4int16_t iChannelIndex = GetObjectIndex(&ai_roChannel);
	
	if (iChannelIndex == -1)
		return M4_ERROR;    // No se encontro el channel del access
	
	ClAccessIndex NumAccess = ai_roChannel.Access.Count();
	
	m4int16_t iAccessIndex;
	PtrWithId oPtrWithId;
	for (ClAccessIndex i = 0; i < NumAccess; i++) {

		ClAccess *poAccess = ai_roChannel.Access[i] ;

		if( poAccess == NULL )
		{
			continue ;
		}

		// Lo primero de todo es comprobar que no esta en el directorio
		iAccessIndex = GetObjectIndex(poAccess);
		if (iAccessIndex != -1) continue;    // YA EXISTE !!!

		/* Bug 0070551
		En el caso de ejecución XML siempre hay que serializar todos los accesos.
		*/
		if (ai_bIncludeAlways == M4_FALSE)
		{
			/* Bug 0069695
			Sólo viajan los accesos que tengan remote id
			*/
			if( poAccess->GetRemoteId() == 0 )
			{
				continue ;
			}
		}
		
		oPtrWithId.m_poPtr = poAccess;
		
		// La direccion remota.
		oPtrWithId.m_iRemoteId = poAccess->GetRemoteId();
		
		// Creamos OID para el Access
		oPtrWithId.m_poOId = CreateAccessOId(ai_roChannel, i);
		if (oPtrWithId.m_poOId == 0) return M4_ERROR;
		
		if (_Include(oPtrWithId) == -1) return M4_ERROR;
		
		oPtrWithId.Init(M4_FALSE);         
	}
	
	return M4_SUCCESS;
}

m4int16_t ClCMObjectDirectory::IncludeOLTPExecutionContextAndCreateOID(ClOLTPExecutionContext& ai_roOLTPExecutionContext)
{
    // Lo primero de todo es comprobar que no esta en el directorio
    m4int16_t iIndex = GetObjectIndex(&ai_roOLTPExecutionContext);
    if (iIndex != -1)
        return iIndex;    // YA EXISTE !!!

    PtrWithId oPtrWithId;
    oPtrWithId.m_poPtr = &ai_roOLTPExecutionContext;
    
    // Creamos el OID para el OLTPExecutionContext
    oPtrWithId.m_poOId = CreateOLTPExecutionContextOId();
    if (oPtrWithId.m_poOId == 0)
        return -1;
    
    return _Include(oPtrWithId);
}

m4int16_t ClCMObjectDirectory::_IncludeClientServerResult(ClClientServerResult* ai_pCSResult)
{
   M4_ASSERT(ai_pCSResult);

    // Lo primero de todo es comprobar que no esta en el directorio
    m4int16_t iIndex = GetObjectIndex(ai_pCSResult);
    if (iIndex != -1)
        return iIndex;    // YA EXISTE !!!

   PtrWithId oPtrWithId;
   oPtrWithId.m_poPtr = ai_pCSResult;

   // Creamos el OID para el ClientServerResult
   oPtrWithId.m_poOId = CreateClientServerResultOId();
   if (oPtrWithId.m_poOId == 0)
      return M4_ERROR;

   return _Include(oPtrWithId);
}

m4uint16_t ClCMObjectDirectory::GetCount(void) const
{
   return m_iCount;
}

m4uint16_t ClCMObjectDirectory::GetCountByType(const m4ObjectType_t ai_eObjectType) const
{
   return m_iObjTypeCount[ai_eObjectType];
}

void* ClCMObjectDirectory::GetObjectAddress(m4uint16_t ai_iIndex) const
{
   M4_ASSERT( ai_iIndex < m_iCount);

   return m_oObjectPtr[ai_iIndex].m_poPtr;
}

m4return_t ClCMObjectDirectory::GetObjectType(m4uint16_t ai_iIndex, m4ObjectType_t&  ao_iType) const
{
   M4_ASSERT( ai_iIndex < m_iCount);

   ao_iType = m_oObjectPtr[ai_iIndex].m_poOId->GetType();

   return M4_SUCCESS;
}

m4return_t ClCMObjectDirectory::GetObjectId(m4uint16_t ai_iIndex, ClObjectId*& ao_poObjectId) const 
{
   M4_ASSERT( ai_iIndex < m_iCount);

   ao_poObjectId = m_oObjectPtr[ai_iIndex].m_poOId;

   return M4_SUCCESS;
}


// !!!!!!!!!!!!!!!!!!!!! SERIALIZATION/DESERIALIZATION !!!!!!!!!!!!!!!!!

m4return_t ClCMObjectDirectory::SetRootBlockToSerialize(const ClChannel* const ai_pChannel,
                                                        m4uint32_t ai_iBlock,
                                                        m4uint32_t ai_iRegister)
{
   m4int16_t i = GetObjectIndex(ai_pChannel);
   if (i == -1)
      return M4_ERROR;  // El objeto no existe

   M4_MSG_ASSERT(m_oObjectPtr[i].m_poOId->GetType() == ChannelObjectType, "The object is not a Channel");

   m_oObjectPtr[i].m_iBlockToSerialize = ai_iBlock;
   m_oObjectPtr[i].m_iRegisterToSerialize = ai_iRegister;


   return M4_SUCCESS;
}

m4uint32_t ClCMObjectDirectory::GetRootBlockToSerialize(const ClChannel* const ai_pChannel) const
{
   m4int16_t i = GetObjectIndex(ai_pChannel);
   if (i == -1)
      return -1;  // El objeto no existe

   M4_MSG_ASSERT(m_oObjectPtr[i].m_poOId->GetType() == ChannelObjectType, "The object is not a Channel");

   return m_oObjectPtr[i].m_iBlockToSerialize;
}

m4uint16_t ClCMObjectDirectory::GetSerializationVersion(void)
{
   return m_iSerializationVersion;
}

m4return_t ClCMObjectDirectory::Serialize(ClGenericIODriver& IOD)
{
#ifdef GET_SERIAL_TIME	
	ClSttClock clock;
#endif

	// Serilizo los miembros necesarios del Directory
	m4return_t iRet = IOD.Write(m_iSerializationVersion);
	if (iRet != M4_SUCCESS) return iRet;
	
	iRet = IOD.Write(m_bKeepChannelCsTypeOnRemote);
	if (iRet != M4_SUCCESS) return iRet;
	
	iRet = IOD.Write(m_bProhibitL2RelationSerialize);
	if (iRet != M4_SUCCESS) return iRet;
	
	m4int16_t  i = -1;
	ClChannel* pChannel = 0;
	
	m4bool_t bExecutingInClient = (m_poChannelManager) ?
		(m_poChannelManager->GetDMContext()->ExecutingInClient()) : M4_FALSE;

	if (bExecutingInClient == M4_TRUE) {
		// Para dar soporte de transacciones multicanal antes de empezar la serializacion tengo que buscar todos
		// los canales que están marcados para viajar incondicionalmente. Pero sólo en la parte del cliente.
		m4int32_t iChannelCount = m_poChannelManager->Channel.Count();
		for (i = 0; i < iChannelCount; i++) {
			pChannel = m_poChannelManager->Channel[i];
			
			if (pChannel->m_oGroupInfo.HaveToTravel() == M4_TRUE) {
				/* Bug 0102234
				Hay que poner el remoteid para los autos
				*/
				if (IncludeChannel(pChannel, pChannel->GetRemoteId()) == -1) {
					return M4_ERROR;
				}
			}
		}

		// Hay que poner todos los canales con SetKepSatutsOnRemote = M4_TRUE. Todos
		// los canales me tienen que devolver deltas.
		for (i = GetFirstObjectIndex(ChannelObjectType); i != -1; i = GetNextObjectIndex()) {
			pChannel = (ClChannel*)m_oObjectPtr[i].m_poPtr;
			if (!pChannel) continue;
			pChannel->GetpChannel_Data()->SetKeepStatusOnRemote(M4_TRUE);

			// bg 114200. Meta4Object's delta cache in server side could be erroneous due to a communication failure.
			// We have to set the remote id to 0, before serialize it in SerializeOId method.

			/* Bug 0116384
			Sólo hay que hacerlo si se serializa el canal, porque en depuración
			nos podemos haber quedados desincronizados temporalmente y luego nos recuperaremos
			*/
			if ( pChannel->m_bIsSynchronized == M4_FALSE && m_oObjectPtr[i].m_bDoSerialize == M4_TRUE )
			{
				ClCompiledMCR *poCMCR = pChannel->GetpChannel_Data()->GetpChannelDef( )->GetpCMCR( ) ;

				if ( poCMCR != NULL )
				{
					m4uint8_t iCSType = poCMCR->GetChannelCsExeType( );
										
					if ( iCSType == M4CL_CSEXETYPE_AUTO )
					{
						// Re-Build M4O in server side based on client side data.
						m_oObjectPtr[i].m_iRemoteId = 0;
						pChannel->SetRemoteId ( 0 );
						pChannel->GetpChannel_Data()->SetKeepStatus ( M4_FALSE, 1 );
					}
					else if ( iCSType == M4CL_CSEXETYPE_PROXY )
					{
						// In case of proxy M4Os we can't re-build server side M4O.
						DUMP_CHLOG_ERRORF(M4_CS_SERIALIZE_PROXY_ERROR,M4Obj(pChannel->GetpCMCR()));
						return M4_ERROR;
					}
				}
			}
			// bg 114200.
		}
	}


	// Se serializan los idremotos de los canales autos que se han muerto
	iRet = SerializeDeletedAutos(IOD);
	if (iRet != M4_SUCCESS) return iRet;

	// Se serializa el estado de la licencia
	iRet = SerializeLicenseState(IOD);
	if (iRet != M4_SUCCESS) return iRet;


	iRet = SerializeL2Relation(IOD);
	if (iRet != M4_SUCCESS) return iRet;
	
	iRet = SerializeOId(IOD);
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_DEBUG(M4_CS_SERIALIZE_OBJECTIDENTIFIER);
		return iRet;
	}
	
	iRet = SerializeObject(IOD);
	if (iRet != M4_SUCCESS) return iRet;

	// User actions auditory.
	iRet = SerializeUserActionsAudit(IOD);
	if (iRet != M4_SUCCESS) return iRet;


#ifdef GET_SERIAL_TIME
	double dMiliSg = clock.GetDifference();
	ofstream ofs;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( ofs, GET_SERIAL_TIME_FILE, M4UniAppend, eEncoding );

	ofs << "S\t" << dMiliSg << endl;
#endif
	
	if (bExecutingInClient == M4_FALSE)
	{
		for (i = GetFirstObjectIndex(ChannelObjectType); i != -1; i = GetNextObjectIndex())
		{
			pChannel = (ClChannel*)m_oObjectPtr[i].m_poPtr;
			if (!pChannel) continue;
			
			pChannel->SetStatisticsLevel(M4_VM_STATISTICS_DISABLED);
		}
		
		if (m_poChannelManager != NULL)
		{
			m_poChannelManager->GetSttWrapper()->DestroyStatistics();
		}
	}
	else
	{
		/**
		 * Corrige el bug 0060062.
		 * Si estamos terminando una depuración activamos el modo de depuración porque este estado se actualiza con la
		 * deserialización del ejecutor, y cuando estamos haciendo esta última transacción el ejecutor no viaja.
		 * Podemos habernos quedado con el modo desactivado si justo antes de terminar hemos ejecutado una reentrada o
		 * algo en el inmediate window.
		 */
		ClExecutor* const pExecutor = (ClExecutor*)(m_poChannelManager->GetpExecutor());
		m4bool_t bStepperActive = pExecutor->GetpStepper()->GetActive();
		
		// SetKeepChannelCsTypeOnRemote is used only in server.
		// stepper mode is not compatible (avoid serialization)
		if (GetKeepChannelCsTypeOnRemote() == M4_FALSE)
		{
			iRet = IOD.Write(bStepperActive);
		}
		else
		{
			if (bStepperActive == M4_TRUE)
				return M4_ERROR;
		}
	}
	
	return iRet;
}


/*
Se serializan los idremotos de los canales autos que se han muerto
*/
m4return_t ClCMObjectDirectory::SerializeDeletedAutos( ClGenericIODriver& IOD )
{

	m4return_t	iResult = M4_ERROR ;


	/* Bug 0102945
	Se chequea que el ChannelManager no sea nulo
	*/
	if( m_poChannelManager == NULL )
	{
		if( IOD.Write( m4uint32_t( 0 ) ) == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}

		return( M4_SUCCESS ) ;
	}

	iResult = m_poChannelManager->SerializeDeletedAutos( IOD ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


/*
Se deserializan los idremotos de los canales autos y se borran de la caché
*/
m4return_t ClCMObjectDirectory::DeSerializeDeletedAutos( ClGenericIODriver& IOD, IObjDataCache* ai_pIObjDataCache )
{

	m4uint32_t	i = 0 ;
	m4uint32_t	iLength = 0 ;
	m4uint32_t	iRemoteId = 0 ;


	if( IOD.Read( iLength ) != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	for( i = 0 ; i < iLength ; i++ )
	{
		if( IOD.Read( iRemoteId ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( ai_pIObjDataCache != NULL )
		{
			ai_pIObjDataCache->DeleteObject( iRemoteId, "" ) ;
		}
	}

	return( M4_SUCCESS ) ;
}

// User actions auditory.
m4return_t ClCMObjectDirectory::SerializeUserActionsAudit( ClGenericIODriver& IOD )
{
	m4return_t	iResult = M4_ERROR ;


	if( m_poChannelManager == NULL )
	{
		return M4_SUCCESS ;
	}

	iResult = m_poChannelManager->UserActionsAudit.Serialize( IOD ) ;


	return iResult ;
}

// User actions auditory.
m4return_t ClCMObjectDirectory::DeserializeUserActionsAudit( ClGenericIODriver& IOD )
{
	m4return_t	iResult = M4_SUCCESS ;


	if( m_poChannelManager == NULL )
	{
		return M4_SUCCESS ;
	}
	
	iResult = m_poChannelManager->UserActionsAudit.Deserialize( IOD ) ;


	return iResult ;
}



m4return_t ClCMObjectDirectory::SerializeLicenseState( ClGenericIODriver& IOD )
{

	m4bool_t		bSendState = M4_FALSE ;
	m4int8_t		iState = STATE_VALIDATE_EXECUTION_NP_RED ;
	ClDMContext		*poContext = NULL ;
	ILicenseManager	*pLicenseManager = NULL ;


	/* Bug 0102945
	Se chequea que el ChannelManager no sea nulo
	*/
	if( m_poChannelManager != NULL )
	{
		poContext = m_poChannelManager->GetDMContext() ;

		if( poContext != NULL && poContext->ExecutingInClient() == M4_FALSE && poContext->IsSM() == M4_FALSE )
		{
			pLicenseManager = m_poChannelManager->GetpLicenseManager() ;

			if( pLicenseManager != NULL )
			{
				iState = pLicenseManager->GetStateValidateExecutionNp() ;
				bSendState = M4_TRUE ;
			}
		}
	}

	if( IOD.Write( bSendState ) == M4_ERROR )
	{
		return M4_ERROR;
	}

	if( bSendState == M4_TRUE )
	{
		if( IOD.Write( iState ) == M4_ERROR )
		{
			return M4_ERROR;
		}
	}

	return( M4_SUCCESS ) ;
}


/*
En este punto, el CLIENTE va a leer el estado de NP que le ha pasado el SERVER.
*/
m4return_t ClCMObjectDirectory::DeSerializeLicenseState( ClGenericIODriver& IOD )
{

	m4return_t		iResult = M4_ERROR ;
	m4bool_t		bSentState = M4_FALSE ;
	m4int8_t		iState = STATE_VALIDATE_EXECUTION_NP_RED ;
	ClDMContext		*poContext = NULL ;
	ILicenseManager	*pLicenseManager = NULL ;


	if( IOD.Read( bSentState ) != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( bSentState == M4_TRUE )
	{
		if( IOD.Read( iState ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		/* Bug 0102945
		Se chequea que el ChannelManager no sea nulo
		*/
		if( m_poChannelManager != NULL  )
		{
			poContext = m_poChannelManager->GetDMContext() ;

			if( poContext != NULL && poContext->ExecutingInClient() == M4_TRUE && poContext->IsSM() == M4_FALSE )
			{
				pLicenseManager = m_poChannelManager->GetpLicenseManager() ;

				if( pLicenseManager != NULL )
				{
					iResult = pLicenseManager->SetStateValidateExecutionNp( eStateValidateExecutionNp_t( iState ) ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
	}

	return( M4_SUCCESS ) ;
}




/**
 * Serializa los enlaces de nivel2 entre los canales que viajan en el objDir.
 * Sólo los serializa si estamos en cliente o en el server en depuración, o cuando me fuerzan
 * a hacerlo (por ejemplo en el ejecutor de XML, al persistir el estado a disco).
 *
 * @param IOD	IODriver sobre el que se escribe la serialización.
 * @return m4return_t.
 */
m4return_t ClCMObjectDirectory::SerializeL2Relation(ClGenericIODriver& IOD)
{
	// checks serialization state
	if(GetbProhibitL2RelationSerialize() == M4_TRUE)
	{
		return M4_SUCCESS;
	}

	// if error reporting we don't have a channel manager
	if (m_poChannelManager == NULL)
		return M4_SUCCESS;

	ClExecutor* const pExecutor = (ClExecutor*)(m_poChannelManager->GetpExecutor());
	m4bool_t bStepperActive = pExecutor->GetpStepper()->GetActive();
	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();

	if (m_bForceL2RelationSerialize == M4_FALSE && bExecutingInClient == M4_FALSE && bStepperActive == M4_FALSE) {
		return M4_SUCCESS;
	}

	m_oLevel2RelationVector.clear();


	m4return_t iRet = M4_ERROR;
	ClChannel* pChannel = 0;
	m4int16_t  iChildChannelIndex = 0;
	m4int16_t  iAccessIndex = 0;
	m4uint32_t iLevel2ChannelIndex = 0;
	m4uint32_t iLevel2ChannelCount = 0;
	ClAccess*  pAccess = 0;
	m4char_t*  sInstance = 0;
	
	ClLevel2Relation oLevel2Relation;
	//m_oLevel2RelationVector.clear();

	m4int16_t iChannelIndex = GetFirstObjectIndex(ChannelObjectType);
	for (; iChannelIndex != -1; iChannelIndex = GetNextObjectIndex()) {

		//Bug:0063274   Begin  -----------------------------------------------
		// La función ClCMObjectDirectory::SerializeL2Relation no comprueba si las relaciones del canal que va a serializar hacia
		//  el cliente pueden pertenecer a un canal que ha sido destruido.
		// El mensaje de error que se vuelca solo tiene sentido si no estamos depurando el canal, puesto que en caso de depuración
		//  en el cliente tenemos la vista back del canal, al igual que en la parte sever
		CheckChannelExistBeforeSerial(iChannelIndex);
		//Bug:0063274   End    -----------------------------------------------

		// Me recorro todas las conexiones de nivel2.
		pChannel = (ClChannel*)m_oObjectPtr[iChannelIndex].m_poPtr;
		if (!pChannel) continue;

		/* Bug 0269640
		Se manda el self access si está el acceso en el directorio
		*/
		pAccess = pChannel->Level2.GetSelfAccess();

		if( pAccess != NULL )
		{
			iAccessIndex = GetObjectIndex(pAccess);

			if (iAccessIndex != -1)
			{
				oLevel2Relation.m_iParentIndex = iChannelIndex;
				oLevel2Relation.m_iChildIndex  = iChannelIndex;
				oLevel2Relation.m_iAccessIndex = iAccessIndex;
				oLevel2Relation.SetInstance("");
				
				m_oLevel2RelationVector.push_back(oLevel2Relation);
			}
		}

		iLevel2ChannelCount = pChannel->Level2.m_oInstanceMap.Count();
		
		for (iLevel2ChannelIndex = 0; iLevel2ChannelIndex < iLevel2ChannelCount; iLevel2ChannelIndex++)	{
			// Por cada canal de nivel2 compruebo si está en el directorio.

			iRet = pChannel->Level2.m_oInstanceMap.GetInstanceAndAccess(iLevel2ChannelIndex, (const char *&)sInstance, pAccess);
			if (iRet != M4_SUCCESS) return iRet;
			
			iChildChannelIndex = GetObjectIndex(pAccess->GetpChannel());
			
			if (iChildChannelIndex != -1) {
				//Bug:0063895 Begin --------------------------------------------
				// No se estaba poniendo el RemoteId de Access se incluyen en el final del objdir como
				//  consecuencia de una relación de nivel 2
				m4uint32_t iRemoteId = 0;
				if(!bExecutingInClient)
					iRemoteId = pAccess->GetHandle();
				//Bug:0063895 End   --------------------------------------------


				// Como si existe el canal de nivel2, tengo que añadir la conexion de nivel2 en el
				// directorio. Ademas tengo que añadir el accesso al objDir para que se serialize.
				iAccessIndex = IncludeAccessAndCreateOID(pAccess, iRemoteId);
				if (iAccessIndex == -1) return M4_ERROR;
				
				oLevel2Relation.m_iParentIndex = iChannelIndex;
				oLevel2Relation.m_iChildIndex  = iChildChannelIndex;
				oLevel2Relation.m_iAccessIndex = iAccessIndex;
				oLevel2Relation.SetInstance(sInstance);
				
				m_oLevel2RelationVector.push_back(oLevel2Relation);
			}
		}
	}
	
	iRet = m_oLevel2RelationVector.Serialize(IOD);

	return iRet;
}

/**
 * Deserializa los enlaces de nivel2 entre los canales que viajan en el objDir.
 * Si estamos en cliente en depuración hace una copia de los enlaces de nivel2
 * para, al restaurarlos, poder encontrar los que se han borrado.
 *
 * @param IOD	IODriver del que se lee la serialización.
 * @return m4return_t.
 */
m4return_t ClCMObjectDirectory::DeSerializeL2Relation(ClGenericIODriver& IOD)
{
	// checks serialization state
	if(GetbProhibitL2RelationSerialize() == M4_TRUE)
	{
		return M4_SUCCESS;
	}

	// Los niveles2 hay que deserializarlos cuando estoy en el servidor, o cuando en el cliente estoy depurando.
	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();

	if (bExecutingInClient == M4_TRUE)
	{
		ClExecutor* const pExecutor = (ClExecutor*)(m_poChannelManager->GetpExecutor());
		m4bool_t bStepperActive = pExecutor->GetpStepper()->GetActive();

		if (bStepperActive == M4_FALSE)
		{
			return M4_SUCCESS;
		}
		else
		{
			m_oLevel2RelationVectorBeforeDeser = m_oLevel2RelationVector;
		}
	}

	return m_oLevel2RelationVector.DeSerialize(IOD);
}

/**
 * Restaura las instancias de nievel2 entre los canales que hay en el objDir.
 * Si estamos en cliente en depuración también se encarga de borrar las instancias de nivel2 que han desaparecido.
 *
 * @return m4return_t.
 */
m4return_t ClCMObjectDirectory::RestoreL2Relation(void)
{
	// checks serialization state
	if(GetbProhibitL2RelationSerialize() == M4_TRUE)
	{
		return M4_SUCCESS;
	}

	m4return_t iRet = M4_SUCCESS;
	ClChannel* pChannel = 0;
	ClAccess*  pAccess = 0;
	ClExecutor* const pExecutor = (ClExecutor*)(m_poChannelManager->GetpExecutor());
	m4bool_t bStepperActive = pExecutor->GetpStepper()->GetActive();
	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();

	if (bExecutingInClient == M4_TRUE && bStepperActive == M4_FALSE) {
		return M4_SUCCESS;
	}

	// Primero añadimos todos los enlaces de nivel2 que hemos deserializado.
	// Restauro las conexiones sin comprobar si me devuelve error. Si devuelve
	// error es porque la conexión ya estaba establecida, pues perfecto.
	vector<ClLevel2Relation>::iterator i = m_oLevel2RelationVector.begin();
	for (; i != m_oLevel2RelationVector.end(); i++) {
		pChannel = (ClChannel*)m_oObjectPtr[(*i).m_iParentIndex].m_poPtr;
		pAccess  =  (ClAccess*)m_oObjectPtr[(*i).m_iAccessIndex].m_poPtr;

		/* Bug 0269640
		Si viene un selfaccess se asigna
		*/
		if (pAccess->GetpChannel() == pChannel)
		{
			pChannel->Level2.SetSelfAcces(pAccess);
		}
		else
		{
			pChannel->Level2.InsertL2Access((*i).GetInstance(), pAccess, M4_FALSE);
		}
	}

	if (bExecutingInClient == M4_FALSE || bStepperActive == M4_FALSE) {
		return M4_SUCCESS;
	}

	// Después comprobamos los que hay que borrar. Esta comprobación la hacemos después de haberlos añadido
	// porque si hacemos primero el borrado pueden verse alterados los índices que nos indican quien es el
	// padre y quien es el hijo en el enlace de nivel2.
	// Para saber si se ha borrado busco los enlaces de nivel2 que serializé a la ida con los que he deserializado
	// a la vuelta. Si no encuentro alguno, es porque se ha borrado en el otro extremo.
	vector<ClLevel2Relation>::iterator j;

	for (i = m_oLevel2RelationVector.begin(); i != m_oLevel2RelationVector.end(); i++) {
		j = m_oLevel2RelationVectorBeforeDeser.begin();
		for (; j != m_oLevel2RelationVectorBeforeDeser.end(); j++) {
			if (strcmp((*i).GetInstance(), (*j).GetInstance()) == 0) {
				// La he encontrado.
				m_oLevel2RelationVectorBeforeDeser.erase(j);	// Puedo borrar sin preocuparme del iterador porque salgo del bucle.
				break;
			}
		}
	}

	// Como he ido borrando los enlaces de nivel2 que encontraba, los que quedan en
	// "m_oLevel2RelationVectorBeforeDeser" son los que se han borrado.
	ClChannel* pChildChannel = 0;
	m4uint32_t iChildAccessCount = 0;
	i = m_oLevel2RelationVectorBeforeDeser.begin();

	for (; i != m_oLevel2RelationVectorBeforeDeser.end(); i++) {
		// Como se ha borrado en el otro extremo hay que borrarlo en este lado.
		// Ojo porque al borrar el enlace de nivel2 se borra el acceso y el canal
		// (si no tenía mas accesos que el de nivel2).
		
		// Al borrar el acceso y el canal hay que deshabilitarlos también en el objDir (para que no se
		// intente acceder a ellos).
		// Preguntamos el número de accesos que tiene el canal hijo para saber si al borrar el enlace de
		// nivel2 se va a borrar también el canal (esto sucederá si sólo tiene un acceso, el de nivel2).
		// Sólo deshabilitamos el canal si realmente se ha borrado.
		pChildChannel = (ClChannel*)m_oObjectPtr[(*i).m_iChildIndex].m_poPtr;
		iChildAccessCount = pChildChannel->Access.Count();
		
		pChannel = (ClChannel*)m_oObjectPtr[(*i).m_iParentIndex].m_poPtr;
		iRet = pChannel->Level2.EraseL2Instance((*i).GetInstance(), M4_TRUE);	// M4_TRUE para destruir el canal.
		if (iRet != M4_SUCCESS) return iRet;
		
		m_oObjectPtr[(*i).m_iAccessIndex].m_poPtr = 0;
		if (iChildAccessCount < 2) {
			m_oObjectPtr[(*i).m_iChildIndex].m_poPtr = 0;
		}
	}

	return iRet;
}


m4return_t ClCMObjectDirectory::SerializeOId(ClGenericIODriver& IOD)
{
	m4return_t iRet = IOD.Write(m_iCount);
	if (iRet != M4_SUCCESS) return M4_ERROR;
	
	// Serilizo los OID, en el orden en el que estan en el vector
	for (m4uint16_t i = 0; i < m_iCount; i++) {
		m_oObjectPtr[i].m_iSerializedSize = IOD.GetActualOffset();
		
		iRet = m_oObjectPtr[i].m_poOId->Serialize(IOD);
		if (iRet != M4_SUCCESS) return M4_ERROR;
		
		switch (m_oObjectPtr[i].m_poOId->GetType()) {
		case ChannelObjectType:
			CheckChannelExistBeforeSerial(i);
			CheckDebugModeAndSetSerialMode(i);
			
			iRet = IOD.Write(m_oObjectPtr[i].m_iIsDebug);
			if (iRet != M4_SUCCESS) return M4_ERROR;
			
		case AccessObjectType:
			iRet = IOD.Write(m_oObjectPtr[i].m_iRemoteId);
			if (iRet != M4_SUCCESS) return M4_ERROR;
		}
		
		iRet = IOD.Write(m_oObjectPtr[i].m_bDoSerialize);
		if (iRet != M4_SUCCESS) return M4_ERROR;
		
		m_oObjectPtr[i].m_iSerializedSize = IOD.GetActualOffset() - m_oObjectPtr[i].m_iSerializedSize;
	}
	
	return iRet;
}

m4return_t ClCMObjectDirectory::SerializeObject(ClGenericIODriver& IOD)
{
	ClChannel* pChannel      = 0;
	m4uint32_t iActualOffset = 0;
	m4return_t iRet          = M4_SUCCESS;
	m4bool_t   bIsLevel2     = M4_FALSE;
	m4pchar_t  pcRSM         = 0;
	m4double_t dLanguage     = 0;
	m4bool_t bExecutingInClient = (m_poChannelManager)? m_poChannelManager->GetDMContext()->ExecutingInClient() : M4_FALSE;
	ClExecutor* pExecutor = 0;
	m4bool_t bStepperActive = M4_FALSE;
	m4bool_t	bSendStatistics = M4_TRUE ;
	m4int16_t	j = 0 ;

	
	ClCsTraceSize* pCsTraceSice = 0;
	ClCsTrace* const pCsTrace = (m_poChannelManager)? m_poChannelManager->GetCsTrace() : NULL;

	if (pCsTrace) {
		pCsTraceSice = pCsTrace->CreateSerializeCsTraceSize();
		if (pCsTraceSice) {
			pCsTraceSice->SetOffsetIni(IOD.GetActualOffset());
		}
	}

	for (m4uint16_t i = 0; i < m_iCount; i++) {
		iActualOffset = IOD.GetActualOffset();
		
		/**************************/
		iRet = IOD.ResetPrivateInfo();
		if (iRet != M4_SUCCESS) return iRet;
		/**************************/
		
		if (m_oObjectPtr[i].m_bDoSerialize == M4_TRUE) {
			switch (m_oObjectPtr[i].m_poOId->GetType()) {
			case MetaChannelObjectType:
				iRet = ((ClCompiledMCR*)m_oObjectPtr[i].m_poPtr)->GetCMCRWrapper()->Serialize(IOD);
				if (iRet != M4_SUCCESS) {
					DUMP_CHLOG_DEBUGF(M4_CS_SERIALIZE_METACHANNEL, M4Obj((ClCompiledMCR*)m_oObjectPtr[i].m_poPtr));
					return iRet;
				}
				break;
				
			case ChannelObjectType:
				pChannel = (ClChannel*)m_oObjectPtr[i].m_poPtr;
				
				// Si el canal esta encarrilado necesito mandar informacion para hacer la build
				// Solo tengo que enviarlo del cliente al servidor, o si estoy depurando.
				pExecutor = (ClExecutor*)(m_poChannelManager->GetpExecutor());
				bStepperActive = pExecutor->GetpStepper()->GetActive();
				if ( (bExecutingInClient == M4_TRUE || bStepperActive == M4_TRUE) &&
						pChannel->GetpCMCR()->GetLaneFatherNodeId() ) {
					iRet = IOD.Write((m4bool_t)M4_TRUE);
					
					if (iRet == M4_SUCCESS) {
						ClLaneArgumnent laneArgument;
						laneArgument.m_pccFather     = (m4char_t*)pChannel->GetpCMCR()->GetLaneFatherChannelId();
						laneArgument.m_pccFatherNode = (m4char_t*)pChannel->GetpCMCR()->GetLaneFatherNodeId();
						laneArgument.m_pccNode       = (m4char_t*)pChannel->GetpCMCR()->GetLaneSonNodeId();
						
						iRet = laneArgument.Serialize(IOD);
						
						laneArgument.Reset(M4_FALSE);

						// sets the information for stepper
						// fix bug 0063042
						if (bExecutingInClient == M4_TRUE && bStepperActive == M4_TRUE)
						{
							// block
							m4VariantType vBlock(m_oObjectPtr[i].m_iBlockToSerialize);
							iRet = pChannel->Property.SetOrNewProperty(M4CMPROP_CHANNEL_LANE_BLOCK_NUM,vBlock,M4_PROP_STATIC_NO_MAPPED);
							if (iRet != M4_SUCCESS)
								break;

							// register
							m4VariantType vReg(m_oObjectPtr[i].m_iRegisterToSerialize);
							iRet = pChannel->Property.SetOrNewProperty(M4CMPROP_CHANNEL_LANE_REG_NUM,vReg,M4_PROP_STATIC_NO_MAPPED);
							if (iRet != M4_SUCCESS)
								break;
						}
					}
				}
				else
				{
					iRet = IOD.Write((m4bool_t)M4_FALSE);
				}
				
				// jcr Serializo si es de nivel 2 o no
				bIsLevel2 = pChannel->GetIsLevel2() ;
				iRet = IOD.Write( bIsLevel2 ) ;
				
				// Va el RSM para la seguridad
				pChannel->RSM_ID.Get( pcRSM ) ;
				
				if (pcRSM != NULL)
				{
					iRet = IOD.Write((m4bool_t)M4_FALSE);	// Indico que la seguridad no es nulo
					if (iRet == M4_SUCCESS) {
						iRet = IOD.Write(pcRSM);
					}
				}
				else
				{
					iRet = IOD.Write((m4bool_t)M4_TRUE);	// Indico que la seguridad es nula. No debería pasar
				}
				
				// Va el idioma de metadatos
				pChannel->Meta_Language.Get( dLanguage ) ;
				iRet = IOD.Write( m4uint8_t( dLanguage ) ) ;
				
				
				// Serializo el CsType si me han pedido que en el otro extremo se mantenga
				if (GetKeepChannelCsTypeOnRemote() == M4_TRUE && iRet == M4_SUCCESS) {
					m4VariantType varCsType;
					iRet = pChannel->CsType.Get(varCsType);
					
					if (iRet == M4_SUCCESS) {
						iRet = IOD.Write((m4double_t)varCsType);
					}
				}
				
				if (iRet == M4_SUCCESS) {
					iRet = pChannel->Serialize(IOD,
						m_oObjectPtr[i].m_pChannelProc, pCsTraceSice,
						m_oObjectPtr[i].m_iBlockToSerialize,
						m_oObjectPtr[i].m_iRegisterToSerialize);
				}
				
				// Tratamiento de error
				if (iRet != M4_SUCCESS) {
					DUMP_CHLOG_DEBUGF(M4_CH_DS_SERIALIZE_CHANNEL, M4Obj(pChannel->GetpCMCR()));
					return iRet;
				}
				break;
				
			case AccessObjectType:
				iRet = ((ClAccess*)m_oObjectPtr[i].m_poPtr)->Serialize(IOD);
				if (iRet != M4_SUCCESS) {
					DUMP_CHLOG_DEBUGF(M4_CS_SERIALIZE_ACCESS, M4Obj(((ClAccess*)m_oObjectPtr[i].m_poPtr)->GetpChannel()->GetpCMCR()));
					return iRet;
				}
				break;
				
			case OLTPExecutionContextType:
				iRet = ((ClOLTPExecutionContext*)m_oObjectPtr[i].m_poPtr)->Serialize(IOD);
				if (iRet != M4_SUCCESS) {
					DUMP_CHLOG_DEBUG(M4_CS_SERIALIZE_OLTPEXECUTIONCONTEXT);
					return iRet;
				}
				break;
				
			case ClientServerResultObjectType:

				/*
				Si ya se manda un ExecutionContext no hace falta mandar las estadísticas
				*/
				bSendStatistics = M4_TRUE ;

				for( j = GetFirstObjectIndex( OLTPExecutionContextType ) ; j != -1; j = GetNextObjectIndex() )
				{
					if( m_oObjectPtr[ j ].m_bDoSerialize == M4_TRUE )
					{
						bSendStatistics = M4_FALSE ;
						break ;
					}
				}

				iRet = ((ClClientServerResult*)m_oObjectPtr[i].m_poPtr)->Serialize(IOD, m_poChannelManager, bSendStatistics );
				if (iRet != M4_SUCCESS) {
					DUMP_CHLOG_DEBUG(M4_CS_SERIALIZE_CLIENTSERVERRESULT);
					return iRet;
				}
				break;
				
			default:
				M4_ASSERT("Incorrect type of object" == NULL);
				return M4_ERROR;
			}
		}
	
		m_oObjectPtr[i].m_iSerializedSize += IOD.GetActualOffset() - iActualOffset;
	}

	if (pCsTraceSice) {
		pCsTraceSice->SetOffsetEnd(IOD.GetActualOffset());
	}
	
	return M4_SUCCESS;
}

m4return_t ClCMObjectDirectory::DeSerialize(ClGenericIODriver& IOD, IObjDataCache* ai_pIObjDataCache, m4bool_t ai_bSetSttLevel)
{
#ifdef GET_SERIAL_TIME
	ClSttClock clock;
#endif

	// Deserilizo los miembros del Directory
	m4uint16_t iSerializationVersion;
	m4return_t iRet = IOD.Read(iSerializationVersion);
	if (iRet != M4_SUCCESS) return iRet;
	
	// Comprobamos que trabajamos con la version de serializacion correcta
	CHECK_CHLOG_ERRORF(iSerializationVersion != m_iSerializationVersion,
		M4_ERROR,
		M4_CH_DS_INCORRECT_SERIALIZE_VERSION,
		iSerializationVersion << m_iSerializationVersion);
	
	iRet = IOD.Read(m_bKeepChannelCsTypeOnRemote);
	if (iRet != M4_SUCCESS)	return iRet;

	iRet = IOD.Read(m_bProhibitL2RelationSerialize);
	if (iRet != M4_SUCCESS) return iRet;


	// Se deserializan los idremotos de los canales autos que se han muerto
	iRet = DeSerializeDeletedAutos(IOD, ai_pIObjDataCache);
	if (iRet != M4_SUCCESS) return iRet;

	// Se deserializa el estado de la licencia
	iRet = DeSerializeLicenseState(IOD);
	if (iRet != M4_SUCCESS) return iRet;

	
	iRet = DeSerializeL2Relation(IOD);
	if (iRet != M4_SUCCESS)	return iRet;
	
	iRet = DeSerializeOId(IOD);	// First : all the Ids
	if (iRet != M4_SUCCESS) {
		DUMP_CHLOG_DEBUG(M4_CS_DESERIALIZE_OBJECTIDENTIFIER);
		return iRet;
	}
	
	iRet = DeSerializeObject(IOD, ai_pIObjDataCache, ai_bSetSttLevel);	// Second : all the objects
	if (iRet != M4_SUCCESS) return iRet;

	iRet = DeserializeUserActionsAudit(IOD);
	if (iRet != M4_SUCCESS) return iRet;


#ifdef GET_SERIAL_TIME
	double dMiliSg = clock.GetDifference();
	ofstream ofs;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( ofs, GET_SERIAL_TIME_FILE, M4UniAppend, eEncoding );

	ofs << "D\t" << dMiliSg << endl;
#endif

	iRet = RestoreL2Relation();
	if (iRet != M4_SUCCESS) return iRet;

	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();

	if (bExecutingInClient == M4_TRUE)
	{
		ClChannel* pChannel = 0;
		for (m4int16_t i = GetFirstObjectIndex(ChannelObjectType); i != -1; i = GetNextObjectIndex())
		{
			pChannel = (ClChannel*)GetObjectAddress(i);
			if (pChannel)
			{
				// Borro las relaciones de amistad de los canales implicados en la transacción.
				pChannel->m_oGroupInfo.DelAllPartners();
				
				// Borro las marcas de serializacion de los canales implicados en la transacción.
				pChannel->GetpChannel_Data()->ResetSerialMarks();
			}
		}
	}
	else
	{
		/**
		 * Corrige el bug 0060062.
		 * Si estamos terminando una depuración activamos el modo de depuración porque este estado se actualiza con la
		 * deserialización del ejecutor, y cuando estamos haciendo esta última transacción el ejecutor no viaja.
		 * Podemos habernos quedado con el modo desactivado si justo antes de terminar hemos ejecutado una reentrada o
		 * algo en el inmediate window.
		 */
		ClExecutor* const pExecutor = (ClExecutor*)(m_poChannelManager->GetpExecutor());
		m4bool_t bStepperActive = pExecutor->GetpStepper()->GetActive();

		// SetKeepChannelCsTypeOnRemote is used only in server.
		// stepper mode is not compatible (avoid serialization)
		if (GetKeepChannelCsTypeOnRemote() == M4_FALSE)
		{
			iRet = IOD.Read(bStepperActive);
		}
		else
		{
			if (bStepperActive == M4_TRUE)
			{
				return M4_ERROR;
			}
		}
		
		pExecutor->GetpStepper()->SetActive(bStepperActive);
	}
	
	return iRet;
}

m4return_t ClCMObjectDirectory::DeSerializeOId(ClGenericIODriver& IOD)
{
	m4uint16_t iCountSerial;
	m4return_t iRet = IOD.Read(iCountSerial);
	if (iRet != M4_SUCCESS) return iRet;
	
	m4int16_t iOldIndex = 0;
	m4uint32_t iBeforeOffset = 0;
	m4int16_t i ;
	for ( i = 0; i < iCountSerial; i++) {
		iBeforeOffset = IOD.GetActualOffset();
				
		ClObjectId* pAuxOId = new ClObjectId();
		if (!pAuxOId) return M4_ERROR;  // No hay memoria
		
		iRet = pAuxOId->DeSerialize(IOD);
		if (iRet != M4_SUCCESS)
		{
			delete pAuxOId;
			return iRet;
		}
		
		iOldIndex = Find(*pAuxOId);
		if (iOldIndex == -1)
		{
			_Insert(i);
			m_oObjectPtr[i].m_poOId = pAuxOId;
			m_iObjTypeCount[pAuxOId->GetType()]++;
		}
		else
		{
			delete pAuxOId;   // Como ya tengo el OId borro el auxiliar
			if (iOldIndex != i) {
				Swap(i, iOldIndex);
			}
		}
		
		switch (m_oObjectPtr[i].m_poOId->GetType())
		{
		case ChannelObjectType:
			iRet = IOD.Read(m_oObjectPtr[i].m_iIsDebug);
			if (iRet != M4_SUCCESS) return iRet;
			
		case AccessObjectType:
			iRet = IOD.Read(m_oObjectPtr[i].m_iRemoteId);
			if (iRet != M4_SUCCESS) return iRet;
		}
		
		iRet = IOD.Read(m_oObjectPtr[i].m_bDoSerialize);
		if (iRet != M4_SUCCESS) return iRet;
		
		m_oObjectPtr[i].m_iSerializedSize = IOD.GetActualOffset() - iBeforeOffset;
	}
	
	// Los objetos que se han quedado a partir de m_iCountSerial es que no vienen serializados, los borro
	// Si estoy en cliete y se trata de un canal de depuración lo intento borrar.
	for ( ; i < m_iCount; ) Remove(m_iCount - 1);
	
	return iRet;
}

m4return_t ClCMObjectDirectory::DeSerializeObject(ClGenericIODriver& IOD, IObjDataCache* ai_pIObjDataCache, m4bool_t ai_bSetSttLevel)
{
	m4return_t iRet = M4_SUCCESS;
	m4uint32_t iActualOffset;
	
	ClCsTraceSize* pCsTraceSice = 0;
	ClCsTrace* pCsTrace = m_poChannelManager->GetCsTrace();
	if (pCsTrace) {
		pCsTraceSice = pCsTrace->CreateDeSerializeCsTraceSize();
		if (pCsTraceSice) {
			pCsTraceSice->SetOffsetIni(IOD.GetActualOffset());
		}
	}


	for (m4int16_t i = 0; i < m_iCount; i++) {
		M4_ASSERT(m_oObjectPtr[i].m_poOId);
		
		iRet = IOD.ResetPrivateInfo();
		if (iRet != M4_SUCCESS) return iRet;
		
		iActualOffset = IOD.GetActualOffset();
		
		switch (m_oObjectPtr[i].m_poOId->GetType()) {
		case MetaChannelObjectType:
			if (m_oObjectPtr[i].m_bDoSerialize == M4_TRUE) {
				iRet = DeSerializeOneMetaChannelObj(IOD, i);
				if (iRet != M4_SUCCESS) {
					DUMP_CHLOG_DEBUGF(M4_CS_DESERIALIZE_METACHANNEL, m_oObjectPtr[i].m_poOId->GetTypedId());
					return iRet;
				}
			}
			break;
			
		case ChannelObjectType:
			iRet = DeSerializeOneChannelObj(IOD, i, pCsTraceSice, ai_pIObjDataCache, ai_bSetSttLevel);
			if (iRet != M4_SUCCESS) {
				DUMP_CHLOG_DEBUGF(M4_CH_DS_DESERIALIZE_CHANNEL, m_oObjectPtr[i].m_poOId->GetTypedId());
				return iRet;
			}
			break;
			
		case AccessObjectType:
			iRet = DeSerializeOneAccessObj(IOD, i);
			if (iRet != M4_SUCCESS) {
				DUMP_CHLOG_DEBUGF(M4_CS_DESERIALIZE_ACCESS, m_oObjectPtr[i].m_poOId->GetTypedId());
				return iRet;
			}
			break;
			
		case OLTPExecutionContextType:
			if (m_oObjectPtr[i].m_bDoSerialize == M4_TRUE) {
				iRet = DeSerializeOneOLTPExecutionContext(IOD, i);
				if (iRet != M4_SUCCESS) {
					DUMP_CHLOG_DEBUG(M4_CS_DESERIALIZE_OLTPEXECUTIONCONTEXT);
					return iRet;
				}
			}
			break;
			
		case ClientServerResultObjectType:
			if (m_oObjectPtr[i].m_bDoSerialize == M4_TRUE) {
				iRet = DeSerializeOneClientServerResultObj(IOD, i);
				if (iRet != M4_SUCCESS) {
					DUMP_CHLOG_DEBUG(M4_CS_DESERIALIZE_CLIENTSERVERRESULT);
					return iRet;
				}
			}
			break;
			
		default:
			M4_ASSERT("Incorrect type of object" == NULL);
			return M4_ERROR;
		}
		
		m_oObjectPtr[i].m_iSerializedSize += IOD.GetActualOffset() - iActualOffset;
	}

	if (pCsTraceSice) {
		pCsTraceSice->SetOffsetEnd(IOD.GetActualOffset());
	}
	
	return iRet;
}

m4return_t ClCMObjectDirectory::DeSerializeOneMetaChannelObj(ClGenericIODriver& IOD, m4uint16_t ai_iIndex)
{
    M4_ASSERT( ai_iIndex < m_iCount);

    // Intento hacer la deserialización sobre un objeto ya existente (normalmente a la vuelta del server)
    ClCompiledMCR* pCompiledMCR = (ClCompiledMCR*)m_oObjectPtr[ai_iIndex].m_poPtr;
    
    ClCMCRWrapper* pCMCRWrapper = NULL; 
    
    if (pCompiledMCR == NULL)
    {
        pCMCRWrapper = new ClCMCRWrapper();
        if (pCMCRWrapper == NULL)
            return M4_ERROR;

        if (pCMCRWrapper->DeSerialize(IOD) != M4_SUCCESS)
        {
            delete pCMCRWrapper;
            return M4_ERROR;
        }
        
        pCompiledMCR = new ClCompiledMCR;
        if (pCompiledMCR == NULL)
        {
            delete pCMCRWrapper;
            return M4_ERROR;
        }
        
        if (pCompiledMCR->SetCMCRWrapper(pCMCRWrapper) != M4_SUCCESS)
        {
            delete pCMCRWrapper;
            delete pCompiledMCR;
            return M4_ERROR;
        }

        m_oObjectPtr[ai_iIndex].m_poPtr = pCompiledMCR;
        m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory = M4_TRUE;
        m_oObjectPtr[ai_iIndex].m_bDoSerialize = M4_FALSE;  // No tengo por que enviar el metacanal de vuelta
    }
    else
    {
        pCMCRWrapper = pCompiledMCR->GetCMCRWrapper();
        if (pCMCRWrapper == NULL)
		{
            return M4_ERROR;
		}

        if (pCMCRWrapper->DeSerialize(IOD) != M4_SUCCESS)
		{
            return M4_ERROR;
		}
    }
        
    return M4_SUCCESS;
}

m4return_t ClCMObjectDirectory::DeSerializeOneClientServerResultObj(ClGenericIODriver& IOD, m4uint16_t ai_iIndex)
{
    M4_ASSERT( ai_iIndex < m_iCount);
    
    // Intento hacer la deserialización sobre un objeto ya existente (normalmente a la vuelta del server)
    ClClientServerResult* pClientServerResult = (ClClientServerResult*)m_oObjectPtr[ai_iIndex].m_poPtr;
    
    if (pClientServerResult == NULL)
    {
        pClientServerResult = new ClClientServerResult;
        
        if (pClientServerResult == NULL)
            return M4_ERROR;
        
        m_oObjectPtr[ai_iIndex].m_poPtr = pClientServerResult;
        m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory = M4_TRUE;
    }
    
    return pClientServerResult->DeSerialize(IOD, m_poChannelManager);
}

ClChannel* ClCMObjectDirectory::_FindChannel(m4uint16_t ai_iIndex, m4uint16_t ai_iFirstChannelIndex, SetObjDirIndex_t& ai_rsetObjDirIndex)
{
	/* Bug 0269341
	Avoid circular references.
	*/
	pair< ItSetObjDirIndex_t, bool >	p = ai_rsetObjDirIndex.insert( ai_iIndex );

	if( p.second == false )
	{
		return NULL;
	}


	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();
	m4uint32_t iRemoteId = m_oObjectPtr[ai_iIndex].m_iRemoteId;
	ClChannel* pChannel = (ClChannel*)m_oObjectPtr[ai_iIndex].m_poPtr;
	
	if (!pChannel) {
		// Intento localizar el canal en el lado server.
		if (iRemoteId && bExecutingInClient == M4_FALSE) {
			// Estoy en server buscando un canal proxy o de depuración.
			m4int32_t i = FindChannelInCM(iRemoteId);

			if( i > - 1 )
			{
				pChannel = m_poChannelManager->Channel[i];
				M4_ASSERT (pChannel != NULL);
				m_oObjectPtr[ai_iIndex].m_poPtr = pChannel;
			}
			else
			{
				// Si no está en el channelmanager todavía se puede buscar en la caché
				// porque puede ser un canal auto
				return( NULL ) ;
			}
		}
	}
	else
	{
		if (bExecutingInClient == M4_TRUE)
		{
			// El canal ya existe. Ajusto el proxy remoto.
			pChannel->SetRemoteId(iRemoteId);
		}
	}
	
	// Si todavia no he encontrado el canal lo busco entre las instancias de nivel2
	if (!pChannel)
	{
		m4int16_t iParentIndex = -1;
		vector<ClLevel2Relation>::iterator it = m_oLevel2RelationVector.begin();
		for (; it != m_oLevel2RelationVector.end(); it++)
		{
			if (ai_iIndex == (*it).m_iChildIndex)
			{
				// Yo soy un canal hijo de nivel2 de otro canal que va en el directorio
				// Suponemos que mi padre ya se ha deserializado. Si esto no fuera ciero vamos muy mal
				// Tengo que comprobar si ya existo, o tengo que crearme. Para esto me busco por el
				// nombre de la instancia de nivel2 en mi padre
				iParentIndex = (*it).m_iParentIndex;
				if (iParentIndex == ai_iFirstChannelIndex)
					break;  // Estamos entrando en un ciclo, asi que mejor paramos
				
				ClChannel* pParentChannel = _FindChannel(iParentIndex, ai_iFirstChannelIndex, ai_rsetObjDirIndex); 
				if (pParentChannel)
				{
					ClAccess* pAccessInstance;
					if (pParentChannel->Level2.m_oInstanceMap.Find((*it).GetInstance(), &pAccessInstance) == M4_SUCCESS)
					{
						// Lo he encontrado. Tengo que deserializar sobre este
						pChannel = pAccessInstance->GetpChannel();
						m_oObjectPtr[ai_iIndex].m_poPtr = pChannel;
					}
				}
				break;
			}
		}
	}
	
	return pChannel;
}

ClAccess* ClCMObjectDirectory::_FindAccess(m4uint16_t ai_iAccessIndex, m4uint16_t ai_iChannelIndex)
{
	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();
	m4uint32_t iRemoteId = m_oObjectPtr[ai_iAccessIndex].m_iRemoteId;
	ClAccess* pAccess = (ClAccess*)m_oObjectPtr[ai_iAccessIndex].m_poPtr;
	
	if (!pAccess) {
		// Intento localizar el acceso en el lado server.
		if (iRemoteId && bExecutingInClient == M4_FALSE) {
			ClChannel* pChannel = (ClChannel*)m_oObjectPtr[ai_iChannelIndex].m_poPtr;
			M4_ASSERT(pChannel);
			
			ClAccessIndex NumAccess = pChannel->Access.Count();
			for (ClAccessIndex i = 0; i < NumAccess; i++) {
				if (pChannel->Access[i]->GetHandle() == iRemoteId) {
					pAccess = pChannel->Access[i];
					break;
				}
			}
			
			M4_ASSERT(pAccess);
			m_oObjectPtr[ai_iAccessIndex].m_poPtr = pAccess;
		}
	}
	else
	{
		if (bExecutingInClient == M4_TRUE)
		{
			// El acceso ya existe. Ajusto el proxy remoto.
			pAccess->SetRemoteId(iRemoteId);
		}
	}
	
	// Si todavia no he encontrado el acceso lo busco entre las instancias de nivel2
	if (!pAccess)
	{
		vector<ClLevel2Relation>::iterator it = m_oLevel2RelationVector.begin();
		for (; it != m_oLevel2RelationVector.end(); it++)
		{
			if (ai_iAccessIndex == (*it).m_iAccessIndex)
			{
				// Yo soy un acceso hijo de nivel2 de otro canal que va en el directorio
				// Suponemos que mi padre ya se ha deserializado. Si esto no fuera ciero vamos muy mal
				// Tengo que comprobar si ya existo, o tengo que crearme. Para esto me busco por el
				// nombre de la instancia de nivel2 en mi padre

				SetObjDirIndex_t	setObjDirIndex;
				ClChannel* pParentChannel = _FindChannel((*it).m_iParentIndex, ai_iChannelIndex, setObjDirIndex);
				if (pParentChannel != NULL)
				{
					ClAccess* pAccessInstance;

					if (pParentChannel->Level2.m_oInstanceMap.Find((*it).GetInstance(), &pAccessInstance) == M4_SUCCESS)
					{
						// Lo he encontrado. Tengo que deserializar sobre este
						pAccess = pAccessInstance;
						m_oObjectPtr[ai_iAccessIndex].m_poPtr = pAccess;
					}
				}
				break;
			}
		}
	}
	
	return pAccess;
}

m4return_t ClCMObjectDirectory::DeSerializeOneChannelObj(ClGenericIODriver& IOD, m4uint16_t ai_iIndex, ClCsTraceSize* const ai_pCsTraceSice, IObjDataCache* ai_pIObjDataCache, m4bool_t ai_bSetSttLevel)
{
	M4_ASSERT( ai_iIndex < m_iCount);
	
	const ClChannel::eDebugMode_t eDebugMode = (ClChannel::eDebugMode_t)m_oObjectPtr[ai_iIndex].m_iIsDebug;
	m4return_t       iRet = M4_SUCCESS;
	m4bool_t         bIsLane = M4_FALSE;
	m4pchar_t        pcRSM = 0;
	m4char_t         pStaticRSM [M4CL_MAX_RSM_ID * 2];
	m4bool_t         bRoleIsNULL = M4_TRUE;
	m4bool_t         bIsLevel2 = M4_FALSE;
	m4uint8_t        iLanguage = 0;
	m4double_t       iCsType = 0;
	ClLaneArgumnent* pLaneArgument = 0;
	ClLaneArgumnent  laneArgument;
	
	// Antes de nada deserializo si me han mandado informacion para encarrilar el canal.
	// Sólo tengo que enviarlo del cliente al servidor.
	if (m_oObjectPtr[ai_iIndex].m_bDoSerialize == M4_TRUE)
	{
		iRet = IOD.Read(bIsLane);
		if (iRet != M4_SUCCESS) return iRet;
		
		// Si el canal esta encarrilado he tenido que mandar informacion para hacer la build
		if (bIsLane == M4_TRUE)
		{
			iRet = laneArgument.DeSerialize(IOD);
			if (iRet != M4_SUCCESS) return iRet;
			pLaneArgument = &laneArgument;

			// receives lane info
			// fix bug 0063042
			ClChannel *pChannel = (ClChannel*)m_oObjectPtr[ai_iIndex].m_poPtr;
			if (pChannel != NULL)
			{
				m4VariantType vValue;
				if (pChannel->Property.GetPropValue(M4CMPROP_CHANNEL_LANE_BLOCK_NUM,vValue) == M4_SUCCESS)
				{
					m_oObjectPtr[ai_iIndex].m_iBlockToSerialize = (long)vValue;
					pChannel->Property.DeletePrivateProperty(M4CMPROP_CHANNEL_LANE_BLOCK_NUM);
				}
					
				if (pChannel->Property.GetPropValue(M4CMPROP_CHANNEL_LANE_REG_NUM,vValue) == M4_SUCCESS)
				{
					m_oObjectPtr[ai_iIndex].m_iRegisterToSerialize = (long)vValue;
					pChannel->Property.DeletePrivateProperty(M4CMPROP_CHANNEL_LANE_REG_NUM);
				}
			}
		}
		
		// jcr Leemos el nivel 2
		iRet = IOD.Read(bIsLevel2);
		if (iRet != M4_SUCCESS) return iRet;
		
		// Leemos la  seguridad
		iRet = IOD.Read(bRoleIsNULL);
		if (iRet != M4_SUCCESS) return iRet;
		
		if (bRoleIsNULL == M4_FALSE) {
			iRet = IOD.Read(pStaticRSM, M4CL_MAX_RSM_ID * 2);
			if (iRet != M4_SUCCESS) return iRet;
			
			pcRSM = pStaticRSM;
		}
		
		// jcr Leemos el idioma de metadatos
		iRet = IOD.Read(iLanguage);
		if (iRet != M4_SUCCESS) return iRet;
		
		// Deserializo el CsType si me han pedido que en el otro extremo se mantenga
		if (GetKeepChannelCsTypeOnRemote() == M4_TRUE) {
			iRet = IOD.Read(iCsType);
			if (iRet != M4_SUCCESS) return iRet;
		}
	}
	
	SetObjDirIndex_t	setObjDirIndex;
	ClChannel* pChannel = _FindChannel(ai_iIndex, ai_iIndex, setObjDirIndex);
	
	// If the channel is not found in the ChannelManager then create and build a new one.
	if (!pChannel) {
		pChannel = CreateAndBuildChannel(ai_iIndex, pLaneArgument, iLanguage, bIsLevel2, pcRSM, iCsType, ai_pIObjDataCache);
		if (!pChannel) return M4_ERROR;
		
		m_oObjectPtr[ai_iIndex].m_poPtr = pChannel;
	}
	
	// Fijo el nivel de depuración del canal.
	pChannel->SetDebugMode(eDebugMode);
	
	// Ahora podemos intentar deserializar el canal
	if (m_oObjectPtr[ai_iIndex].m_bDoSerialize == M4_TRUE) {
		iRet = pChannel->DeSerialize(IOD, m_oObjectPtr[ai_iIndex].m_iBlockToSerialize, m_oObjectPtr[ai_iIndex].m_iRegisterToSerialize, ai_pCsTraceSice, ai_bSetSttLevel);
		if (iRet != M4_SUCCESS) return iRet;
	}
	else
	{
		m_oObjectPtr[ai_iIndex].m_bDoSerialize = M4_TRUE;  // A la vuelta tendré que enviar algo, digo yo
	}
	
	// si es el canal sesion securizo la máquina virtual
	if (m_poChannelManager != NULL && 
			m_poChannelManager->GetSessionChannel() == NULL &&
			ai_iIndex == 0 &&
			strcmp(pChannel->GetpChannelDef()->Id(),M4_SESSION_CHANNEL_ID) == 0)
	{
		m_poChannelManager->SetSessionChannel(pChannel);
		SetKeepAlive(ai_iIndex,M4_TRUE);
	}
	
	return iRet;
}

m4return_t ClCMObjectDirectory::DeSerializeOneAccessObj(ClGenericIODriver& IOD, m4uint16_t ai_iIndex)
{
	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();

	// Look for a Channel in the Directory with the same name, warning only the first
	m4int16_t iChannelIndex = GetFirstObjectIndex(ChannelObjectType, m_oObjectPtr[ai_iIndex].m_poOId->GetTypedId());

	while (iChannelIndex != -1 &&
			strcmp(m_oObjectPtr[iChannelIndex].m_poOId->GetUniqueKey(), m_oObjectPtr[ai_iIndex].m_poOId->GetEnvironmentId()) != 0) {
		iChannelIndex = GetNextObjectIndex();
	}

	/* Bug 0063324
	Al buscar un access en cliente hay que comprobar que puede ser uno nuevo creado en server
	En ese caso hay que utilizar el remoteid para encontrarlo
	*/
	if( iChannelIndex < 0 && bExecutingInClient == M4_TRUE )
	{
		iChannelIndex = GetFirstObjectIndex( ChannelObjectType, m_oObjectPtr[ ai_iIndex ].m_poOId->GetTypedId() ) ;
		while( iChannelIndex != -1 )
		{
			ClChannel *poChannel = (ClChannel*) m_oObjectPtr[ iChannelIndex ].m_poPtr ;

			if( poChannel != NULL )
			{
				if( (m4uint32_t) atoi( m_oObjectPtr[ ai_iIndex ].m_poOId->GetEnvironmentId() ) == poChannel->GetRemoteId() )
				{
					break;
				}
			}
		}
	}

	if (iChannelIndex < 0) return M4_ERROR;     // There isn't a Channel with the same name

	ClChannel* pChannel = (ClChannel*)m_oObjectPtr[iChannelIndex].m_poPtr;

	ClAccess* pAccess = _FindAccess(ai_iIndex, iChannelIndex);

	const ClChannel::eDebugMode_t eDebugMode = pChannel->GetDebugMode();

	// Si todavía no he encontrado el acceso creo uno nuevo 
	if (!pAccess)
	{
		pAccess = pChannel->CreateAccess();
		if (!pAccess) return M4_ERROR;  // The Channel could not create a new Access
		
		// Checks for proxy channels, o si se trata de un OLTP debug.
		// Para accesos los AUTO tienen el mismo tratamiento que los OLTP
		m4uint8_t  iCSExeType = pChannel->GetpChannelDef()->CsExeType();

		if (iCSExeType == M4CL_CSEXETYPE_PROXY || eDebugMode != ClChannel::IS_NOT_DEBUG)
		{
			m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory = M4_FALSE;	// lets it alive
			
			if (bExecutingInClient == M4_TRUE && eDebugMode == ClChannel::IS_DEBUG)
			{
				m4uint32_t iRemoteId = m_oObjectPtr[ai_iIndex].m_iRemoteId;
				M4_ASSERT(iRemoteId);
				pAccess->SetRemoteId(iRemoteId);
				/* Bug 0069695
				Se marca el acceso como de depuración
				*/
				pAccess->SetDebugMode(ClChannel::IS_DEBUG);
			}
			else
			{
				m_oObjectPtr[ai_iIndex].m_iRemoteId = pAccess->GetHandle();
			}
		}
		else
		{
			m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory = M4_TRUE;
		}

		m_oObjectPtr[ai_iIndex].m_poPtr = pAccess;
	}

	if (m_oObjectPtr[ai_iIndex].m_bDoSerialize == M4_FALSE)
	{
		m_oObjectPtr[ai_iIndex].m_bDoSerialize = M4_TRUE;  // A la vuelta tendré que enviar algo, digo yo
		return M4_SUCCESS;
	}

	/* Alex 20/1/00
	 * Un canal de debug en el lado server es el mismo que el back (son la misma instancia),
	 * Necesito cambiar el modo de serializacion de los items ya que entre el cliente y el back
	 * la serializacion es mapped, pero entre el debug del cliente y el debug del servidor la
	 * serializacion DEBE ser full (para deserializar el acceso se pregunta el modo al
	 * canal y como la instancia es la misma contesta que el modo es mapped).
	 */
	m4uint8_t iOldIteSerMod;
	m4bool_t bChangeItemSerialMode = M4_BOOL(eDebugMode == ClChannel::IS_DEBUG || eDebugMode == ClChannel::IS_DEBUG_SECOND_TIME);
	if (bChangeItemSerialMode == M4_TRUE)
	{
		iOldIteSerMod = pChannel->GetItemSerializationMode();
		pChannel->SetItemSerializationMode(M4CL_ITEM_SERIAL_ALL);
	}
	
	m4return_t iRet = pAccess->DeSerialize(IOD);
	if (iRet != M4_SUCCESS) return iRet;
	
	/* Alex 20/1/00
	 * Restauro los valores del modo de serializacion
	 */
	if (bChangeItemSerialMode == M4_TRUE)
	{
		pChannel->SetItemSerializationMode(iOldIteSerMod);
	}
	
	// Una vez deserializado el acceso, si el canal es un DOC y en el cliente esta a nihil
	// tengo que hacer un SetCurrentBlock, del RecordSet del nodo encarrilado, a nihil
	if (m_oObjectPtr[iChannelIndex].m_iRegisterToSerialize == -1)
	{
		ClRecordSet& recordSet = pAccess->Node[pChannel->GetpCMCR()->GetLaneSonNodeId()].RecordSet;
		recordSet.SetCurrentBlock(&g_oNihilBlock);
	}
	
	return iRet;
}

m4return_t ClCMObjectDirectory::DeSerializeOneOLTPExecutionContext(ClGenericIODriver& IOD, m4uint16_t ai_iIndex)
{
   // Intento hacer la deserialización sobre un objeto ya existente (normalmente a la vuelta del server)
   ClOLTPExecutionContext* pOLTPExeCtxt = (ClOLTPExecutionContext*)m_oObjectPtr[ai_iIndex].m_poPtr;

   if (pOLTPExeCtxt == NULL)
   {
      // Create the new OLTPExecutionContextType

      pOLTPExeCtxt = new ClOLTPExecutionContext(*m_poChannelManager, 0);
      if (pOLTPExeCtxt == NULL)
         return M4_ERROR;  // There is not memory

      m_oObjectPtr[ai_iIndex].m_poPtr = pOLTPExeCtxt;
      m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory = M4_TRUE;
   }

   return pOLTPExeCtxt->DeSerialize(IOD);
}

/**
 * Comprueba la versión del metacanal.
 * En caso de no coincidir se prepara el objDir para indicar al cliente que metacanales están desactualizados.
 *
 * @param ai_pChannel	Canal al que le vamos a comprobar la fecha del metacanal.
 * @param ai_iIndex		Posición donde se encuentra el oid del canal, servirá para reportar el error.
 * @return m4return_t.
 */
m4return_t ClCMObjectDirectory::CheckChannelVersion(ClChannel* const ai_pChannel, m4uint16_t ai_iIndex)
{
	// Version con la que se creo el canal
	// EnvironmentId = Fecha del metacanal
	// ??? ojo que esto es un vil truco, estoy metiendo en el environment las fecha del metacanal
	// ??? habra que arreglarlo, pero por ahora parece que hay prisa
	/*
	Bug 0083569
	La fecha se manda ahora en formato ISO
	*/
	m4date_t dCreateVersion = ClStringToDate(m_oObjectPtr[ai_iIndex].m_poOId->GetEnvironmentId());
	
	// Version real que tiene que tener el canal
	// La saco del metacanal del canal que acabo de crear
	m4date_t dRealVersion = ai_pChannel->GetpCMCR()->GetChannelVersion();
	
	if (fabs(dRealVersion - dCreateVersion) >= M4_LN4_DATE_PRECISION)
	{
		// Error las versiones no son correctas, el metacanal en el cliente es inválido.
		// Creamos un objeto para reportar los errores.
		ClClientServerResult* pCSResult = CreateClientServerResult();
		if (pCSResult == 0) return M4_ERROR;
		
		pCSResult->SetResponseCode(M4_CSR_ERROR_CHANNEL_VERSION);
		pCSResult->AddMess(m_oObjectPtr[ai_iIndex].m_poOId->GetTypedId());
		
		// Comprobamos el resto de metacanales
		m_iLastObjectIndex = ai_iIndex;
		m_iLastObjectType  = ChannelObjectType;
		
		return M4_ERROR;
		// TODO: Ahora es recursivo va a fallar.
		// Por ahora devolvemos error. Estamos perdiendo si en había otro canal con fecha inválida en la transacción.

		m4int16_t  iIndex = GetNextObjectIndex();
		ClChannel* pChannel = 0;

		// Si el canal ya existe no hace falta que compruebe el metacanal.
		for (; iIndex > -1; iIndex = GetNextObjectIndex())
		{
			if (m_oObjectPtr[iIndex].m_poPtr == 0)
			{
				pChannel = CreateAndBuildChannel((m4uint16_t)iIndex, NULL, M4CL_LANGUAGE_NONE, M4_FALSE, NULL, M4CL_CSTYPE_BACK, NULL);

				delete pChannel;
				m_iLastObjectIndex = iIndex;
				m_iLastObjectType  = ChannelObjectType;
			}
		}
		
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}

/**
 * Crea un nuevo canal y hace la build.
 * También comprueba la versión del canal, en caso de que no sea dinámico.
 * Actualiza el estado del objDir para saber si este se tiene que encargar de la destrucción del canal.
 *
 * @param ai_iIndex			Posición que corresponde al canal.
 * @param ai_pLaneArgument	Información para encarrila el canal.
 * @param ai_bIsLevel2		Si es un canal de nivel 2.
 * @param ai_pcRSM			Máscara de seguridad.
 * @param ai_iCsType		Tipo cliente servidor (FRONT, BACK, ...) con el que se tiene que construir el canal,
 *							en caso de que nos fuercen a mantener el mismo cstype en ambos extremos de la transacción.
 * @param ai_pIObjDataCache Cache de canales auto
 * @return Puntero al canal que se ha construido, 0 si ha habido error.
 */
ClChannel* ClCMObjectDirectory::CreateAndBuildChannel(
										m4uint16_t				ai_iIndex,
										ClLaneArgumnent* const	ai_pLaneArgument,
										m4uint8_t				ai_iLanguage,
										m4bool_t				ai_bIsLevel2,
										m4char_t* const			ai_pcRSM,
										m4double_t				ai_iCsType,
										IObjDataCache*			ai_pIObjDataCache)
{

	m4bool_t bExecutingInClient = m_poChannelManager->GetDMContext()->ExecutingInClient();
	ClChannel::eDebugMode_t eDebugMode = (ClChannel::eDebugMode_t)m_oObjectPtr[ai_iIndex].m_iIsDebug;
	ClChannel* pChannel = 0;
	ClChannel* pLaneChannel = 0;
	m4return_t iRet = M4_ERROR;
	m4int16_t i = -1;
	m4double_t dCsType = M4CL_CSTYPE_BACK;
	m4uint8_t iCsExeType = (m4uint8_t)-1;
	m4uint32_t iRemoteId = 0;
	m4pcchar_t pccChannelId = NULL ;
	ClChannel_Data *poChannelData = NULL ;
	ClCMCRFactory *poFactoy = NULL ;


	pccChannelId = m_oObjectPtr[ ai_iIndex ].m_poOId->GetTypedId() ;
	
	// Si me han dado informacion para encarrilar el canal, le añado el metacanal del padre
	if (ai_pLaneArgument)
	{
		// Construimos el metacanal encarrilador
		// Hago una build del canal lane y le pido el metacanal,
		// lo destruire al terminar de construir el canal doc
		iRet = m_poChannelManager->CreateChannel(pLaneChannel);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
		// Esta es la parte back (servidor)
		pLaneChannel->CsType.Set(m4VariantType((m4double_t)M4CL_CSTYPE_BACK));   
		
		/* Bug 0175300
		El carril no puede ser construido sin calcular el RSM
		*/
		iRet = pLaneChannel->BuildFromId(ai_pLaneArgument->m_pccFather);
		if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		
		// Ya tengo los parametros preparados para construir el canal doc
		ai_pLaneArgument->m_poFatherCMCR = pLaneChannel->GetpCMCR();
	}

	// Si ya tiene un remoteid hay que crearlo con ese handle
	iRemoteId = m_oObjectPtr[ ai_iIndex ].m_iRemoteId ;

	if( ai_pIObjDataCache != NULL && iRemoteId != 0 )
	{
		iRet = m_poChannelManager->CreateChannel(pChannel, iRemoteId);
	}
	else
	{
		iRet = m_poChannelManager->CreateChannel(pChannel);
	}
	if (iRet != M4_SUCCESS) goto RETURN_ERROR;
	
	// Antes de hacer la build del canal tengo que comprobar si tengo que aplicarle el mismo CsType
	// que tenia en el otro extremo, o tengo que aplicarle el que corresponde con el environment actual.
	if (GetKeepChannelCsTypeOnRemote() == M4_FALSE)
	{
		if (bExecutingInClient == M4_TRUE)
		{
			switch (m_oObjectPtr[ai_iIndex].m_iIsDebug)
			{
			case ClChannel::IS_NOT_DEBUG:
				dCsType = M4CL_CSTYPE_FRONT;	// Esta es la parte front (cliente).
				break;
				
			case ClChannel::IS_DEBUG:
				dCsType = M4CL_CSTYPE_DEBUG;
				break;
				
			case ClChannel::IS_DEBUG_FIRST_TIME:
			default:
				M4_ASSERT(0);
				goto RETURN_ERROR;
			}
		}
	}
	else
	{
		dCsType = ai_iCsType;
	}
	pChannel->CsType.Set(m4VariantType(dCsType));
	
	// jcr Ponemos que es serializado para que no se compruebe el Role y la sociedada
	// Si va el role porque si no se comprueba no se rellena
	pChannel->SetSerialized() ;
	
	// jcr Asignamos el nivel 2
	if (ai_bIsLevel2 == M4_TRUE) {
		pChannel->SetIsLevel2() ;
	}
	
	// Va el rsm para poner la seguridad
	if (ai_pcRSM != NULL) {
		pChannel->RSM_ID.Set( ai_pcRSM ) ;
	}

	// La sociedad debe ser nula para que no se haga el mapeo por sociedad
	// No hace falta ponerla porque el canal se construye ya con sociedad nula
	// pChannel->Organization.SetNull() ;
	
	// Va el lenguaje de metadatos
	if (ai_iLanguage != M4CL_LANGUAGE_NONE) {
		pChannel->Meta_Language.Set( ai_iLanguage ) ;
	}
	
	// Look for a MetaChannel in the Directory with the same name, warning only the first.
	// Esto lo hacemos para saber como hacer la build del canal. Si hay un metacanal con el mismo
	// nombre es que es un canal dinámico, y lo construiremos a partir de este metacanl. Por el
	// contrario si no hay un metacanal con el mismo nombre lo construiremos normalmente con un
	// BuildFromID.

	/* Bug 0100036
	El metacanal si existe es siempre justo el anterior al canal y no el primero que se pille
	i = GetFirstObjectIndex (MetaChannelObjectType, m_oObjectPtr[ai_iIndex].m_poOId->GetTypedId());
	*/

	i = -1 ;
	if( ai_iIndex > 0 )
	{
		if( m_oObjectPtr[ ai_iIndex - 1 ].m_poOId->GetType() == MetaChannelObjectType )
		{
			if( strcmp( m_oObjectPtr[ ai_iIndex - 1 ].m_poOId->GetTypedId(), pccChannelId ) == 0 )
			{
				i = ai_iIndex - 1 ;
			}
		}
	}

	if (i == -1)
	{
		if( ai_pIObjDataCache != NULL && iRemoteId != 0 )
		{
			poFactoy = m_poChannelManager->GetpFactory() ;

			iRet = ai_pIObjDataCache->GetObject( iRemoteId, pccChannelId, poFactoy, NULL, ai_pLaneArgument, poChannelData ) ;

			if( iRet != M4_SUCCESS || poChannelData == NULL )
			{
				DUMP_CHLOG_ERRORF( M4_CS_NO_RESTORE_REMOTE_M4OBJ, pccChannelId << iRemoteId ) ;
				goto RETURN_ERROR;
			}

			iRet = pChannel->BuildFromChannelData( poChannelData ) ;
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;

			/* Bug 0102663
			Hay que decir al canal que es de la caché
			*/
			pChannel->GetpChannel_Data()->GetpExtInfo()->MCR.SetBuildType(ClMCROwner::Data);
		}
		else
		{
			iRet = pChannel->BuildFromIdNoSec(pccChannelId, ai_pLaneArgument);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
			
			// Una vez creado el canal tengo que asegurarme que la fecha de su metacanal es correcta.
			iRet = CheckChannelVersion(pChannel, ai_iIndex);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;
		}
	}
	else
	{
		if( ai_pIObjDataCache != NULL && iRemoteId != 0 )
		{
			poFactoy = m_poChannelManager->GetpFactory() ;

			iRet = ai_pIObjDataCache->GetObject( iRemoteId, pccChannelId, poFactoy, (ClCompiledMCR*)m_oObjectPtr[ i ].m_poPtr, ai_pLaneArgument, poChannelData ) ;

			if( iRet != M4_SUCCESS || poChannelData == NULL )
			{
				DUMP_CHLOG_ERRORF( M4_CS_NO_RESTORE_REMOTE_M4OBJ, pccChannelId << iRemoteId ) ;
				goto RETURN_ERROR;
			}

			iRet = pChannel->BuildFromChannelData( poChannelData ) ;
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;

			/* Bug 0102663
			Hay que decir al canal que es de la caché y que el propietario del mcr
			*/
			pChannel->GetpChannel_Data()->GetpExtInfo()->MCR.SetBuildType(ClMCROwner::Data);
		}
		else
		{
			/* Bug 0154987
			Hay que pasar el carril si lo hay
			*/
			iRet = pChannel->BuildFromMCR((ClCompiledMCR*)m_oObjectPtr[i].m_poPtr, ai_pLaneArgument);
			if (iRet != M4_SUCCESS) goto RETURN_ERROR;

			// Hay que decir al canal que se encargue de la destrucción del mcr
			pChannel->GetpChannel_Data()->GetpExtInfo()->MCR.SetBuildType(ClMCROwner::ID);
		}
		
		// Como es un canal dinámico hacemos que el objDir no se encargue de su destrucción
		m_oObjectPtr[i].m_bCreatedByDirectory = M4_FALSE;
	}
	
	iCsExeType = pChannel->GetpChannelDef()->CsExeType() ;

	// Si es auto se le asigna la cache de autos para que al destruirse se guarde en la cache
	if( iCsExeType == M4CL_CSEXETYPE_AUTO )
	{
		pChannel->SetObjDataCache( ai_pIObjDataCache ) ;
	}

	// Tengo que comprobar si el objDir tiene que gestionar el canal que acaba de crear.
	// Checks for proxy channels, o si se trata de un OLTP debug.
	if (iCsExeType == M4CL_CSEXETYPE_PROXY || eDebugMode != ClChannel::IS_NOT_DEBUG)
	{
		m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory = M4_FALSE;	// lets it alive

		if (bExecutingInClient == M4_TRUE && eDebugMode == ClChannel::IS_DEBUG)
		{
			// Estoy en cliente se esta creando un canal de depuración que viene del servidor.
			// Hay que asignarle el remoteId que tiene en el servidor.
			M4_ASSERT(m_oObjectPtr[ai_iIndex].m_iRemoteId);
			pChannel->SetRemoteId(m_oObjectPtr[ai_iIndex].m_iRemoteId);
		}
		else
		{
			m_oObjectPtr[ai_iIndex].m_iRemoteId = pChannel->GetHandle();
		}
	}
	else
	{
		m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory = M4_TRUE;

		// Para AUTOS hay que mantener los remoteids
		if( iCsExeType == M4CL_CSEXETYPE_AUTO )
		{
			m_oObjectPtr[ai_iIndex].m_iRemoteId = pChannel->GetHandle();
		}
	}
	
	if (pLaneChannel) {
		pLaneChannel->Destroy();
	}

	return pChannel;
	
RETURN_ERROR:
	if (pChannel) {
		pChannel->Destroy();
	}
	if (pLaneChannel) {
		pLaneChannel->Destroy();
	}
	return 0;
}

ClClientServerResult* ClCMObjectDirectory::CreateClientServerResult(m4bool_t ai_bForceSerialize)
{
	ClClientServerResult* pCSResult = 0;
	
	m4int16_t iIndex = GetFirstObjectIndex(ClientServerResultObjectType);
	
	if (iIndex == -1)
	{
		// Todavia no existe, creo uno nuevo
		pCSResult = new ClClientServerResult();
		if (pCSResult)
		{
			iIndex = _IncludeClientServerResult(pCSResult);
			if (iIndex == -1)
			{
				M4_ASSERT(0);
				delete pCSResult;
				pCSResult = 0;
			}
			
			m_oObjectPtr[iIndex].m_bCreatedByDirectory = M4_TRUE;
		}
		// Si he creado un ClientServerResult es porque se va ha retornar un error,
		// compruebo si necesito enviar de vuelta algun objeto
		if (!ai_bForceSerialize)
		{
			for (m4uint16_t i = 0; i < m_iCount; i++)
			{
				if (i != iIndex)
				{
					m_oObjectPtr[i].m_bDoSerialize = M4_FALSE;

					// Si es un canal auto se le desasigna la cache de autos para que al destruirse no se guarde en la cache
					if( m_oObjectPtr[i].m_poOId->GetType() == ChannelObjectType )
					{
						ClChannel	*pChannel = (ClChannel*) m_oObjectPtr[i].m_poPtr ;

						if( pChannel != NULL )
						{
							m4uint8_t	iCsExeType = pChannel->GetpChannelDef()->CsExeType() ;

							if( iCsExeType == M4CL_CSEXETYPE_AUTO )
							{
								pChannel->SetObjDataCache( NULL ) ;

								/* Bug 0102663
								Si no viene de la caché hay que poner el remoteid a 0 para que no vaya a cliente
								*/
								if( pChannel->GetpChannel_Data()->GetpExtInfo()->MCR.BuildType() != ClMCROwner::Data )
								{
									m_oObjectPtr[i].m_iRemoteId = 0 ;
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		pCSResult = (ClClientServerResult*)m_oObjectPtr[iIndex].m_poPtr;

		/* Bug 0067552
		Cuando el error de versión da en la deserialización de cliente
		no se ha deserializado el result y hay que crearlo a mano
		*/
		if( pCSResult == NULL )
		{
			pCSResult = new ClClientServerResult() ;

			if( pCSResult != NULL )
			{
				m_oObjectPtr[iIndex].m_poPtr = pCSResult ;
				m_oObjectPtr[iIndex].m_bCreatedByDirectory = M4_TRUE;
			}
		}
	}
	
	M4_ASSERT(pCSResult);
	return pCSResult;
}

// dumps the actual list of objects
m4return_t ClCMObjectDirectory::DumpInfo(ostream &ai_pStream)
{

	ai_pStream << endl << "Object Directory dumping procedure starting" << endl;

	ai_pStream << "There are <" << m_iCount << "> objects in the directory." << endl;

	for (m4uint16_t i = 0; i < m_iCount; i++)
	{
		ai_pStream << "Object <" << i << ">" << endl;
		ai_pStream << m_oObjectPtr[i];
	}

	ai_pStream << "Object Directory dumping finishing" << endl;

	return M4_SUCCESS;
}

ostream & operator << (ostream &ao_Output, ClCMObjectDirectory &ai_oObjectDirectory)
{
	m4return_t iRes = ai_oObjectDirectory.DumpInfo(ao_Output);
	if (iRes != M4_SUCCESS)
		ao_Output << "Error " << iRes << " dumping directory info." << endl;
	return ao_Output;
}

void ClCMObjectDirectory::SetKeepAlive(m4uint16_t ai_iIndex,
                                       m4bool_t   ai_bKeepAlive)
{
   m_oObjectPtr[ai_iIndex].m_bCreatedByDirectory = M4_BOOL(!ai_bKeepAlive);
}

void ClCMObjectDirectory::SetKeepAliveAll(m4bool_t   ai_bKeepAlive)
{
	m4uint16_t iObjCount = GetCount();
	for (m4uint16_t iCount = 0; iCount < iObjCount; iCount++) {
		// don´t delete the channel at the end
		SetKeepAlive(iCount, M4_TRUE);
	}

}

void ClCMObjectDirectory::SetDoSerialize(m4uint16_t ai_iIndex,
                                         m4bool_t   ai_bDoSerialize)
{
   m_oObjectPtr[ai_iIndex].m_bDoSerialize = ai_bDoSerialize;
}

m4uint32_t ClCMObjectDirectory::GetSerializedSize(m4uint16_t ai_iIndex) const
{
    return m_oObjectPtr[ai_iIndex].m_iSerializedSize;
}

m4return_t ClCMObjectDirectory::SetChannelProcess(m4uint16_t ai_iIndex, ClChannelProcess* const ai_pChannelProc)
{
	if (m_oObjectPtr[ai_iIndex].m_poOId == NULL ||
			m_oObjectPtr[ai_iIndex].m_poPtr == NULL ||
			m_oObjectPtr[ai_iIndex].m_poOId->GetType() != ChannelObjectType)
	{
		return M4_ERROR;
	}
	
	m_oObjectPtr[ai_iIndex].m_pChannelProc = ai_pChannelProc;
	return M4_SUCCESS;
}

/**
 * Este método debe llamarse en el servidor siempre que hemos parado la ejeucción por un punto de ruptura.
 * Se encarga de comprobar los objetos que hay en el objDir y si es la primer transacción de depuración que hacen,
 * actualiza el remote ID y los marca para que no serializen a la vuelta al cliente.
 *
 * @return ClOLTPExecutionContext*	Como el ejecutor hay que meterlo al final del todo (para que ya esté el puntero al
 *		recordset) se saca del objDir y se devuelve para que después se pueda volver a meter al final (depues de
 *		los canales y accesos que han aparecido durante la ejecución).
 */
ClOLTPExecutionContext* const ClCMObjectDirectory::CheckFirstDebugTransaction(void)
{
	m4uint16_t i = 0;
	ClChannel* pChannel = 0;
	ClAccess* pAccess = 0;
	ClOLTPExecutionContext* pOLTPExeCntxt = 0;


	for (i = 0; i < m_iCount; i++)
	{
		switch(m_oObjectPtr[i].m_poOId->GetType())
		{
		case ChannelObjectType:
			if (!m_oObjectPtr[i].m_poPtr) break;

			pChannel = (ClChannel*) m_oObjectPtr[i].m_poPtr ;

			if(m_oObjectPtr[i].m_iIsDebug == ClChannel::IS_NOT_DEBUG)
			{
				m_oObjectPtr[i].m_iIsDebug = ClChannel::IS_DEBUG_FIRST_TIME;
				pChannel->SetDebugMode(ClChannel::IS_DEBUG_FIRST_TIME);

				if(m_oObjectPtr[i].m_iRemoteId == 0)
				{
					m_oObjectPtr[i].m_iRemoteId = pChannel->GetHandle();
				}
			}

			if (m_oObjectPtr[i].m_iIsDebug == ClChannel::IS_DEBUG_FIRST_TIME)
			{
				m_oObjectPtr[i].m_bDoSerialize = M4_FALSE;
				m_oObjectPtr[i].m_poPtr = 0;	// Para poder añadirlo otra vez en el objDir y que no lo encuentre.
			}
			break;

		case AccessObjectType:
			if (!m_oObjectPtr[i].m_poPtr) break;

			pAccess = (ClAccess*) m_oObjectPtr[i].m_poPtr;

			if(m_oObjectPtr[i].m_iRemoteId == 0)
			{
				m_oObjectPtr[i].m_iRemoteId = pAccess->GetHandle();
			}

			if (pAccess->GetpChannel()->GetDebugMode() == ClChannel::IS_DEBUG_FIRST_TIME)
			{
				m_oObjectPtr[i].m_bDoSerialize = M4_FALSE;
				m_oObjectPtr[i].m_poPtr = 0;	// Para poder añadirlo otra vez en el objDir y que no lo encuentre.
			}
			break;

		case OLTPExecutionContextType:
			pOLTPExeCntxt = (ClOLTPExecutionContext*)(m_oObjectPtr[i].m_poPtr);
			m_oObjectPtr[i].m_bCreatedByDirectory = M4_FALSE;
			Remove(i);
			i -= 1;
			break;

		case MetaChannelObjectType:
		case ClientServerResultObjectType:
			break;

		default:
			M4_ASSERT("Incorrect type of object" == NULL);
			return 0;
		}
	}
	return pOLTPExeCntxt;
}


/* Bug 0069695
Función que chequea que los canales y accesos del objdir existan.
Si no existen se pone el puntero a NULL y el remoteid a cero
*/
m4return_t	ClCMObjectDirectory::CheckObjectExistence( void )
{

	m4return_t				iResult = M4_SUCCESS ;
	m4uint16_t				i = 0 ;
	m4int32_t				j, k ;
	m4int32_t				iNumChannel = 0;
	m4int32_t				iNumAccess = 0;
	m4bool_t				bFounded = M4_FALSE ;
	ClAccess				*pAccess = NULL ;
	ClChannel				*pChannel = NULL ;
	ClOLTPExecutionContext	*pOLTPExeCntxt = NULL ;


	for( i = 0 ; i < m_iCount ; i++ )
	{
		switch( m_oObjectPtr[ i ].m_poOId->GetType() )
		{
			case ChannelObjectType:

				if( m_oObjectPtr[i].m_poPtr == NULL )
				{
					break ;
				}

				pChannel = (ClChannel*) m_oObjectPtr[i].m_poPtr ;

				if( m_poChannelManager != NULL )
				{
					// Se busca el canal
					bFounded = M4_FALSE ;

					iNumChannel = m_poChannelManager->Channel.Count() ;
					for( j = 0 ; j < iNumChannel && bFounded == M4_FALSE ; j++ )
					{
						if( m_poChannelManager->Channel[ j ] == pChannel )
						{
							bFounded = M4_TRUE ;
						}
					}

					// Si no existe el canal hay que resetearlo
					if( bFounded == M4_FALSE )
					{
						m_oObjectPtr[ i ].m_poPtr = 0 ;
						m_oObjectPtr[ i ].m_bDoSerialize = M4_FALSE ;
						m_oObjectPtr[ i ].m_iRemoteId = 0 ;
						break ;
					}
				}
				break;

			case AccessObjectType:

				if( m_oObjectPtr[i].m_poPtr == NULL )
				{
					break ;
				}

				pAccess = (ClAccess*) m_oObjectPtr[i].m_poPtr;

				if( m_poChannelManager != NULL )
				{
					// Se busca el acceso
					bFounded = M4_FALSE;

					iNumChannel = m_poChannelManager->Channel.Count() ;
					for( j = 0 ; j < iNumChannel && bFounded == M4_FALSE ; j++ )
					{
						pChannel = m_poChannelManager->Channel[ j ] ;

						iNumAccess = pChannel->Access.Count();
						for( k = 0 ; k < iNumAccess && bFounded == M4_FALSE ; k++ )
						{
							if( pChannel->Access[ k ] == pAccess )
							{
								bFounded = M4_TRUE ;
							}
						}
					}

					/* Bug 0087766
					En el caso de que el canal esté marcado para borrar se destruye
					y no se manda a cliente.
					*/
					if( bFounded == M4_TRUE )
					{
						m4VariantType	vDelete ;

						iResult = pAccess->Property.GetPropValue( "DELETE_ON_EXIT", vDelete ) ;

						if( iResult == M4_SUCCESS && vDelete.Type == M4CL_CPP_TYPE_NUMBER && vDelete.Data.DoubleData == M4_TRUE )
						{
							pAccess->Destroy() ;
							bFounded = M4_FALSE ;
						}
					}

					// Si no existe el access hay que resetearlo
					if( bFounded == M4_FALSE )
					{
						m_oObjectPtr[ i ].m_poPtr = 0 ;
						m_oObjectPtr[ i ].m_bDoSerialize = M4_FALSE ;
						m_oObjectPtr[ i ].m_iRemoteId = 0 ;
						break ;
					}
				}
				break;

			/* Bug 0085972
			También hay que comprobar que el access delcontexto exista y
			sino marcar como que no se mande. Antes esto se hacía sólo para
			el caso de interrupción de ejecución.
			*/
			case OLTPExecutionContextType:

				if( m_oObjectPtr[i].m_poPtr == NULL )
				{
					break ;
				}

				pOLTPExeCntxt = (ClOLTPExecutionContext*) m_oObjectPtr[i].m_poPtr;
				
				pAccess = pOLTPExeCntxt->GetAccess() ;

				if( pAccess == NULL )
				{
					break ;
				}

				if( m_poChannelManager != NULL )
				{
					// Se busca el acceso
					bFounded = M4_FALSE;

					iNumChannel = m_poChannelManager->Channel.Count() ;
					for( j = 0 ; j < iNumChannel && bFounded == M4_FALSE ; j++ )
					{
						pChannel = m_poChannelManager->Channel[ j ] ;

						iNumAccess = pChannel->Access.Count();
						for( k = 0 ; k < iNumAccess && bFounded == M4_FALSE ; k++ )
						{
							if( pChannel->Access[ k ] == pAccess )
							{
								bFounded = M4_TRUE ;
							}
						}
					}

					// Si no existe el access hay que resetearlo
					if( bFounded == M4_FALSE )
					{
						pOLTPExeCntxt->SetbSendAccessPointer( M4_FALSE ) ;
					}
				}
				break;

			default:
				break ;
		}
	}

	return( M4_SUCCESS ) ;
}


