//## begin module%3507CDFF0340.cm preserve=no
//## end module%3507CDFF0340.cm

//## begin module%3507CDFF0340.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%3507CDFF0340.cp

//## Module: Generic_Proxy%3507CDFF0340; Package body
//## Subsystem: M4Oeinit::src%39211CE701B5
//## Source file: D:\Work\v600\m4oeinit\src\genproxy.cpp

//## begin module%3507CDFF0340.additionalIncludes preserve=no
//## end module%3507CDFF0340.additionalIncludes

//## begin module%3507CDFF0340.includes preserve=yes
#include "m4stl.hpp"
#include <cs_result.hpp>
// Channel_Kernel
#include <chan_knl.hpp>
// ObjDir
#include <objdir.hpp>
// Executor
#include <executor.hpp>
// ClDatStoIod
#include <cldatstoiod.hpp>
// dm
#include <dm.hpp>

#include "blocksynchronization.hpp"

// m4condition
#include <m4condition.hpp>
// proxy emulation
#include <exeproxy.hpp>
#include "m4lgadapi.hpp"
#include "m4objreg.hpp"
#include "channel.hpp"
#include "cldmcontext.hpp"

#include "clm4objservice.hpp"
#include "clfactm4objservice.hpp"
#include "clappclientadap.hpp"
#include "m4mdrt.hpp"
#include "clm4objservice.hpp"
#include "access.hpp"
//## end module%3507CDFF0340.includes

// Generic_Proxy
#include <genproxy.hpp>
// IProxyManager
#include <iproxymanager.hpp>
// ClCache
#include <clcache.hpp>


//## begin module%3507CDFF0340.declarations preserve=no
//## end module%3507CDFF0340.declarations

//## begin module%3507CDFF0340.additionalDeclarations preserve=yes
//## end module%3507CDFF0340.additionalDeclarations


// Class ClProxyManager 






ClProxyManager::ClProxyManager ()
  //## begin ClProxyManager::ClProxyManager%897069668.hasinit preserve=no
      : m_poCMCRCache(NULL), m_poCSCRCache(NULL), m_poPresentationCache(NULL), m_poMapCache(NULL), m_poDatacache(NULL)
  //## end ClProxyManager::ClProxyManager%897069668.hasinit
  //## begin ClProxyManager::ClProxyManager%897069668.initialization preserve=yes
  //## end ClProxyManager::ClProxyManager%897069668.initialization
{
  //## begin ClProxyManager::ClProxyManager%897069668.body preserve=yes
  //## end ClProxyManager::ClProxyManager%897069668.body
}


ClProxyManager::~ClProxyManager ()
{
  //## begin ClProxyManager::~ClProxyManager%897069669.body preserve=yes
  //## end ClProxyManager::~ClProxyManager%897069669.body
}



//## Other Operations (implementation)
void ClProxyManager::SetCaches (ClCache* const ai_poCMCRCache, ClCache* const ai_poCSCRCache, ClCache* const ai_poPresentationCache, ClCache* const ai_poMapCache, ClCache* const ai_poDatacache)
{
  //## begin ClProxyManager::SetCaches%897069670.body preserve=yes
	m_poCMCRCache			= ai_poCMCRCache;
	m_poCSCRCache			= ai_poCSCRCache;
	m_poPresentationCache	= ai_poPresentationCache;
	m_poMapCache			= ai_poMapCache;
	m_poDatacache			= ai_poDatacache;

  //## end ClProxyManager::SetCaches%897069670.body
}

void ClProxyManager::GetCaches (ClCache * &ao_poCMCRCache, ClCache * &ao_poCSCRCache, ClCache * &ao_poPresentationCache, ClCache * &ao_poMapCache, ClCache * &ao_poDatacache) const
{
  //## begin ClProxyManager::GetCaches%897069671.body preserve=yes
	ao_poCMCRCache			= m_poCMCRCache;
	ao_poCSCRCache			= m_poCSCRCache;
	ao_poPresentationCache	= m_poPresentationCache;
	ao_poMapCache			= m_poMapCache;
	ao_poDatacache			= m_poDatacache;
  //## end ClProxyManager::GetCaches%897069671.body
}

void ClProxyManager::Reset ()
{
  //## begin ClProxyManager::Reset%994166232.body preserve=yes
  //## end ClProxyManager::Reset%994166232.body
}

m4return_t ClProxyManager::EraseAllL2Instances (const m4char_t* const ai_pcClProxySpaceID, m4uint32_t ai_iRemoteId)
{
  //## begin ClProxyManager::EraseAllL2Instances%1025256483.body preserve=yes
	ClProxySpace	*poProxySpace = GetSpace( ai_pcClProxySpaceID, M4_FALSE ) ;

	if( poProxySpace != NULL )
	{
		return( poProxySpace->EraseAllL2Instances( ai_iRemoteId ) ) ;
	}

	return( M4_ERROR ) ;
  //## end ClProxyManager::EraseAllL2Instances%1025256483.body
}

// Additional Declarations
  //## begin ClProxyManager%3507E576022F.declarations preserve=yes
  //## end ClProxyManager%3507E576022F.declarations

// Class ClProxySpace 



















ClProxySpace::ClProxySpace (ClProxyManager* const ai_poClProxyManager, ClM4ObjService* const ai_pObjService)
  //## begin ClProxySpace::ClProxySpace%889780466.hasinit preserve=no
      : m_pcBaseID(0), m_bStatus(M4_FALSE), m_pcUniqueID(0), m_bUseStatus(M4_FALSE), m_bObjServiceIsLocal(M4_FALSE), m_pcUserName(0), m_iSessionId(0), m_bRequestToAbort(M4_FALSE), m_uiRequestId(0), m_bIsRunning(M4_FALSE), m_pObjService(0), m_poClProxySpaceContainer(0), m_poClProxyList(0), m_poClProxyManager(ai_poClProxyManager)
  //## end ClProxySpace::ClProxySpace%889780466.hasinit
  //## begin ClProxySpace::ClProxySpace%889780466.initialization preserve=yes
  //## end ClProxySpace::ClProxySpace%889780466.initialization
{
  //## begin ClProxySpace::ClProxySpace%889780466.body preserve=yes

	SetUserName(NULL);

	// allocates variables
	if (ai_pObjService) {
		m_pObjService = ai_pObjService;
	}
	else {
		// Si no me han pasado un objService me lo creo.
		// Este se encarga de inicializar caches, channelManager, ...

		ClCache *poCacheCMCR = NULL ;
		ClCache *poCacheCSCR = NULL ;
		ClCache *poCachePres = NULL ;
		ClCache *poCacheMap = NULL ;
		ClCache *poCacheData = NULL ;

		ai_poClProxyManager->GetCaches( poCacheCMCR, poCacheCSCR, poCachePres, poCacheMap, poCacheData ) ;

		m_pObjService = ClFactM4ObjService::CreateObjService( poCacheCMCR, poCacheCSCR, poCachePres, poCacheMap, poCacheData, M4_TRUE ) ;
		if (!m_pObjService)
			return;

		m_bObjServiceIsLocal = M4_TRUE;
	}

	SetUseStatus(M4_TRUE);
	m_bStatus = M4_TRUE;

	m_vpSessionAccesses.reserve(10);
  //## end ClProxySpace::ClProxySpace%889780466.body
}


ClProxySpace::~ClProxySpace ()
{
  //## begin ClProxySpace::~ClProxySpace%889780467.body preserve=yes
	m_bStatus = M4_FALSE;

	// deletes the proxy list
	if (m_poClProxyList != NULL)
	{
		delete m_poClProxyList;
		m_poClProxyList = NULL;
	}

	if (m_pcUniqueID != NULL)
	{
		delete [] m_pcUniqueID;
		m_pcUniqueID = NULL;
	}

	if (m_pcBaseID != NULL)
	{
		delete [] m_pcBaseID;
		m_pcBaseID = NULL;
	}

	if (m_poClProxySpaceContainer != NULL)
	{
		delete m_poClProxySpaceContainer;
		m_poClProxySpaceContainer = NULL;
	}

	// Checks for the objService member (local or not).
	if (m_pObjService) {
		if (m_bObjServiceIsLocal == M4_TRUE) {
			ClFactM4ObjService::DestroyObjService(m_pObjService);
			m_pObjService = 0;
			m_bObjServiceIsLocal = M4_FALSE;
		}
	}

  //## end ClProxySpace::~ClProxySpace%889780467.body
}



//## Other Operations (implementation)
m4return_t ClProxySpace::SetContainer (ClProxySpaceContainer *ai_poClProxySpaceContainer)
{
  //## begin ClProxySpace::SetContainer%891866790.body preserve=yes
	if (m_poClProxySpaceContainer != NULL)
		return M4_ERROR;

	m_poClProxySpaceContainer = ai_poClProxySpaceContainer;
	return M4_SUCCESS;
  //## end ClProxySpace::SetContainer%891866790.body
}

ClProxyManager * ClProxySpace::GetProxyManager ()
{
  //## begin ClProxySpace::GetProxyManager%892460577.body preserve=yes
	return m_poClProxyManager;
  //## end ClProxySpace::GetProxyManager%892460577.body
}

void ClProxySpace::SetUserName (m4pcchar_t ai_pcUserName)
{
  //## begin ClProxySpace::SetUserName%931259996.body preserve=yes
	if (m_pcUserName == NULL)
		m_pcUserName = new m4char_t [PROXY_MAX_USERNAME+1];

	if (ai_pcUserName != NULL)
	{
		strncpy(m_pcUserName,ai_pcUserName,PROXY_MAX_USERNAME);
		m_pcUserName[PROXY_MAX_USERNAME] = '\0';
	}
	else
		strcpy(m_pcUserName,"NO USER");

  //## end ClProxySpace::SetUserName%931259996.body
}

ClProxySpaceContainer * ClProxySpace::GetProxySpaceContainer ()
{
  //## begin ClProxySpace::GetProxySpaceContainer%892460578.body preserve=yes
	return m_poClProxySpaceContainer;
  //## end ClProxySpace::GetProxySpaceContainer%892460578.body
}

ClChannelManager * ClProxySpace::GetChannelManager ()
{
  //## begin ClProxySpace::GetChannelManager%892460579.body preserve=yes
	ClChannelManager* pChannelManager = m_pObjService->GetpChannelManager();
	return pChannelManager;
  //## end ClProxySpace::GetChannelManager%892460579.body
}

const m4pchar_t ClProxySpace::GetUniqueID () const
{
  //## begin ClProxySpace::GetUniqueID%892624819.body preserve=yes

	// we use this to know if the proxy is already released or not
	return m_pcUniqueID;
  //## end ClProxySpace::GetUniqueID%892624819.body
}

const m4pchar_t ClProxySpace::GetBaseID () const
{
  //## begin ClProxySpace::GetBaseID%892624820.body preserve=yes
	return m_pcBaseID;
  //## end ClProxySpace::GetBaseID%892624820.body
}

m4return_t ClProxySpace::SetBaseID (const m4char_t* const ai_pcID)
{
  //## begin ClProxySpace::SetBaseID%892624821.body preserve=yes
	if (m_pcBaseID != NULL)
		delete [] m_pcBaseID;
	m_pcBaseID = new m4char_t [strlen(ai_pcID) + 1];
	if (m_pcBaseID == NULL)
		return M4_ERROR;
	strcpy(m_pcBaseID,ai_pcID);
	return M4_SUCCESS;
  //## end ClProxySpace::SetBaseID%892624821.body
}

m4return_t ClProxySpace::SetUniqueID (const m4char_t* const ai_pcID)
{
  //## begin ClProxySpace::SetUniqueID%892624822.body preserve=yes
	if (m_pcUniqueID != NULL)
		delete [] m_pcUniqueID;
	m_pcUniqueID = new m4char_t [strlen(ai_pcID) + 1];
	if (m_pcUniqueID == NULL)
		return M4_ERROR;
	strcpy(m_pcUniqueID,ai_pcID);
	return M4_SUCCESS;
  //## end ClProxySpace::SetUniqueID%892624822.body
}

void ClProxySpace::SetUseStatus (const m4bool_t ai_bUseStatus)
{
  //## begin ClProxySpace::SetUseStatus%892624823.body preserve=yes
	m_bUseStatus = ai_bUseStatus;
  //## end ClProxySpace::SetUseStatus%892624823.body
}

m4bool_t ClProxySpace::GetUseStatus ()
{
  //## begin ClProxySpace::GetUseStatus%892624824.body preserve=yes
	return m_bUseStatus;
  //## end ClProxySpace::GetUseStatus%892624824.body
}

m4return_t ClProxySpace::ResetUniqueID ()
{
  //## begin ClProxySpace::ResetUniqueID%893671198.body preserve=yes

	// resets to the base id
	if (m_pcUniqueID != NULL)
	{
		delete [] m_pcUniqueID;
		m_pcUniqueID = NULL;
	}

	return M4_SUCCESS;

  //## end ClProxySpace::ResetUniqueID%893671198.body
}

m4return_t ClProxySpace::SetSessionChannel (ClChannel *ai_poSessionChannel, m4uint32_t ai_iSessionId, m4pcchar_t ai_pcUserName)
{
  //## begin ClProxySpace::SetSessionChannel%897069672.body preserve=yes
	if (m_pObjService == NULL)
		return M4_ERROR;

	m4int32_t	i = 0 ;
	m4int32_t	iLength = 0 ;
	ClAccess	*poAccess = NULL ;

	ClChannelManager* pChannelManager = m_pObjService->GetpChannelManager();

	// resets to original values
	if (ai_poSessionChannel == NULL)
	{
		ClChannel *poSessionChannel = pChannelManager->GetSessionChannel();
		if (poSessionChannel != NULL && !(pChannelManager->GetDMContext()->ExecutingInClient()))
		{
			/* Bug 0163795 
			Hay que borrar los niveles 2
			*/
			poSessionChannel->Level2.EraseAllL2Instances( M4_TRUE, M4_FALSE, pChannelManager ) ;

			if( !(pChannelManager->GetDMContext()->IsSM())) {				
				/*Solo se ejecuta en Server. En SM, se utiliza la función _TeleTransportTo, que se llama
				despues de ExeForProxy del ClClientAppSM*/

				pChannelManager->SetSessionChannel( NULL ) ;

				/* Bug 0063193
				Hay que guardarse los accesos de nivel 2 sobre el canal sesión
				para restaurarlos a la vuelta
				Sólo si el espacio está en uso
				*/
				m_vpSessionAccesses.clear() ;

				iLength = poSessionChannel->Access.Count();

				for( i = 0 ; i < iLength ; i++ )
				{
					// Hay que recorrer de atrás adelante
					poAccess = poSessionChannel->Access[ iLength - i - 1 ] ;

					if( poAccess != NULL )
					{
						poSessionChannel->Access._Remove( poAccess ) ;
						m_vpSessionAccesses.push_back( poAccess ) ;
					}
				}

				// Se saca del canal sesión sin comprobar niveles 2
				if( poSessionChannel->DetachFromChannelManager() != M4_SUCCESS )
				{
					DUMP_CHLOG_ERRORF( M4_SRV_SESSION_CHANNEL , "Proxy: Cannot detach session channel" ) ;
					return( M4_ERROR ) ;
				}
			}
		}

		return M4_SUCCESS;
	}

	// writes a message
	m4char_t buffer[128];
	sprintf(buffer, "Connected to proxy space <%s> - unique <%d> - handler <%p>",  GetBaseID(), GetUniqueID(), this);
	DUMP_CHLOG_DEBUGF ( M4_INF_PROXY_GENERIC_0 , buffer);

#ifdef _DEBUG
	cerr << "Connected to proxy space " << GetBaseID() << " (" << GetUniqueID() << ") handler " << this << endl;
	cerr.flush();
#endif
	
	// sets the actual SessionId
	m4return_t iResult = SetSessionId(ai_iSessionId,ai_pcUserName);
	if (iResult != M4_SUCCESS)
		return M4_ERROR;

	// sets the session channel
	if (!(pChannelManager->GetDMContext()->ExecutingInClient())
		&& (pChannelManager->GetDMContext()->IsSM())) {
		// Estoy en la parte servidor de SM.
		ai_poSessionChannel->_TeleTransportTo(pChannelManager);
	}
	else {
		if (pChannelManager->GetSessionChannel() == NULL)
		{
			// the session channel in SM still lives in the fron channel manager
			if (ai_poSessionChannel->Attach(pChannelManager) != M4_SUCCESS) {
				DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "Proxy: Cannot attach session channel") ;
				return M4_ERROR;
			}

			pChannelManager->SetSessionChannel(ai_poSessionChannel);

			/* Bug 0063193
			Hay que restaurar los accesos de nivel 2 sobre el canal sesión
			*/
			// Para que se cree el accesso del logonadaptor sobre el canal sesión
			pChannelManager->GetLogonAdaptor()->GetSessionRecordSet() ;

			iLength = m_vpSessionAccesses.size() ;

			for( i = 0 ; i < iLength ; i++ )
			{
				poAccess = m_vpSessionAccesses[ iLength - i - 1 ] ;

				if( poAccess != NULL )
				{
					ai_poSessionChannel->Access._Add( poAccess ) ;
				}
			}

			m_vpSessionAccesses.clear() ;
		}
	}

	return M4_SUCCESS;

  //## end ClProxySpace::SetSessionChannel%897069672.body
}

m4return_t ClProxySpace::GetProxyView (ClChannel *aio_poChannel)
{
  //## begin ClProxySpace::GetProxyView%895830822.body preserve=yes

	// the server shouldn´t ask for a view
	if (GetProxyManager()->IsServerMode() == M4_TRUE) {
		return M4_ERROR;
	}

	// checks the session
	ClClientProxyManager *poClClientProxyManager = (ClClientProxyManager*) GetProxyManager();
	if (poClClientProxyManager == NULL) return M4_ERROR;

	// gets the context
	m4handle_t hContextID = poClClientProxyManager->GetContext();
	if (hContextID == 0) return M4_ERROR;

	// generates the query
	M4DataStorage *poDataStorageOut = M4DataStorage::GetNewDataStorage();
	M4DataStorage *poDataStorageInp = M4DataStorage::GetNewDataStorage();

	ClDataStoreIODriver IODOut;
	m4return_t iRet = IODOut.Init(GEN_IO_MODE_WRITE, poDataStorageOut, ((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());
	if (iRet != M4_SUCCESS) return iRet;

	// writes the command type
	m4int16_t ieCommand = (m4int16_t)ClProxyManager::PROXY_GET_VIEW;
	iRet = IODOut.Write(ieCommand);
	if (iRet != M4_SUCCESS) return iRet;

	// writes the proxy name
	iRet = IODOut.Write(m_pcUniqueID);
	if (iRet != M4_SUCCESS)	return iRet;

	// gets the channel id
	m4pchar_t pcChannelId = (m4pchar_t) aio_poChannel->GetpChannelDef()->Id();

	// writes the channel id
	iRet = IODOut.Write(pcChannelId);
	if (iRet != M4_SUCCESS) return iRet;

	// generates the directory
	ClCMObjectDirectory* poDirectory = new ClCMObjectDirectory(m_pObjService->GetpChannelManager());
	if (poDirectory == NULL) {
		return M4_ERROR;
	}

	// adds the current channel without serialization
	m4int16_t i = poDirectory->IncludeChannel(aio_poChannel, 0);
	if (i == -1) {
		return M4_ERROR;
	}
	poDirectory->SetDoSerialize(i, M4_FALSE);

	// serializes the directory
	iRet = poDirectory->Serialize(IODOut);
	if (iRet != M4_SUCCESS) return iRet;

	// closes the IODriver
	IODOut.End();

	// sends the request to server
	m4handle_t hRequestID = 0;
	iRet = poClClientProxyManager->GetClientAPI()->SyncRequest(hContextID,  0,  0, poDataStorageOut, poDataStorageInp, hRequestID, m_pObjService->GetpRegistry()->GetGenCSTimeOut());
	if (iRet != M4_SUCCESS) return iRet;

	ClDataStoreIODriver IODInp;
	iRet = IODInp.Init(GEN_IO_MODE_READ,poDataStorageInp,((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());
	if (iRet != M4_SUCCESS) return iRet;

	// reads the first desearilzation state
	if (IODInp.Read(iRet) != M4_SUCCESS)
		return M4_ERROR;
	if (iRet != M4_SUCCESS) return iRet;

	// deserializes the directory
	iRet = poDirectory->DeSerialize(IODInp, NULL, M4_TRUE);
	if (iRet != M4_SUCCESS) return iRet;

	// check the status
	m4int16_t iIndex = poDirectory->GetFirstObjectIndex(ClientServerResultObjectType);
	ClChannel* poLocalChannel = 0;
	if (iIndex > -1) {
		i = 0;
		ClClientServerResult* pCSResult = (ClClientServerResult*)poDirectory->GetObjectAddress(iIndex);

		if( pCSResult != NULL )
		{
			switch (pCSResult->GetResponseCode())
			{
			case M4_CSR_ERROR_CHANNEL_VERSION: // invalid cache
				poLocalChannel = 0;
				const m4char_t* sErrMessage;
				
				while (sErrMessage = pCSResult->GetMess(i++))
				{
					iIndex = poDirectory->GetFirstObjectIndex(ChannelObjectType, sErrMessage);
					poLocalChannel = (ClChannel*)poDirectory->GetObjectAddress(iIndex);
					
					poLocalChannel->GetpCMCR()->RemoveFromCache();
				}
				break;

			default:
				// no other errors could be sent by the directory
				return M4_ERROR;
			}
		}
	}

	// deletes the directory
	delete poDirectory;

	// returns the error state
	if (iRet != M4_SUCCESS) 
		return iRet;
	
	// reads the second desearilzation state
	if (IODInp.Read(iRet) != M4_SUCCESS)
		return M4_ERROR;
	if (iRet != M4_SUCCESS) return iRet;

	m4uint32_t lRemoteViewID;
	iRet = IODInp.Read(lRemoteViewID);
	if (iRet != M4_SUCCESS) return iRet;

	// evaluates the remote id
	m4char_t pcRemoteId[PROXY_MAX_VIEW_ID];
	sprintf(pcRemoteId,"%ld",lRemoteViewID);

	// Releases memory.
	if (poDataStorageInp) {
		M4DataStorage::DeleteDataStorage(poDataStorageInp); 
	}
	if (poDataStorageOut) {
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
	}

	return iRet;
  //## end ClProxySpace::GetProxyView%895830822.body
}

ClChannel * ClProxySpace::GetProxyDebugView (m4pcchar_t ai_pcLocalId, const m4uint32_t ai_iRemoteId)
{
  //## begin ClProxySpace::GetProxyDebugView%898022272.body preserve=yes

	// checks arguments
	if (!ai_pcLocalId || !ai_iRemoteId) {
		return NULL;
	}

	// the server shouldn´t ask for a debug view
	if (GetProxyManager()->IsServerMode() == M4_TRUE) {
		return NULL;
	}

	// checks the session
	ClClientProxyManager *poClClientProxyManager = (ClClientProxyManager*) GetProxyManager();
	if (!poClClientProxyManager) {
		return NULL;
	}

	ClChannelManager* const pChannelManager = m_pObjService->GetpChannelManager();

	/* Mejora 0056440.
	 * Ahora siempre se va al servidor a buscar el canal. De esta forma siempre devolvemos
	 * un canal con el último estado del canal en el servidor.
	// Detects another instances of the debug view.
	// Puede que el canal back ya este creado porque lo hemos pedido antes.
	// No nos vamos a enganchar a un canal de depuración de un método mapped (OLTP o proxy). Por esto tenemos
	// que comprobar que no está activo el modo de depuración.
	m4double_t dCsType = 0;
	ClChannelDirectory& channelDirectory = pChannelManager->Channel;
	m4uint32_t lNChannels = channelDirectory.Count();
	ClChannel* poClChannel = 0;
	for (m4uint32_t lCount = 0; lCount < lNChannels; lCount++) {
		poClChannel = channelDirectory[lCount];
		if (!poClChannel->GetRemoteId() || poClChannel->IsBuilt() == M4_FALSE || poClChannel->IsDebugEnabled() == M4_TRUE)
			continue;

		// checks for debug views
		poClChannel->CsType.Get(dCsType);
		if (poClChannel->GetDebugMode() != ClChannel::IS_DEBUG_VIEW)
			continue;

		// checks the name
		if (ai_iRemoteId == poClChannel->GetRemoteId()) {
			return poClChannel;		// returns always the same objects for the same RemoteId
		}
	}
	*/

	/* Bug 0153732
	No se debe crear el canal back en cliente.
	Ahora se manda sólo el id y se construye en server, que manda el metacanal y el canal
	*/
	// gets the context
	m4handle_t hContextID = poClClientProxyManager->GetContext();
	if (hContextID == 0) {
		return( NULL ) ;
	}

	// generates the query
	M4DataStorage* poDataStorageOut = M4DataStorage::GetNewDataStorage();
	M4DataStorage* poDataStorageInp = M4DataStorage::GetNewDataStorage();
	ClDataStoreIODriver IOD;

	m4return_t iRet = IOD.Init(GEN_IO_MODE_WRITE,poDataStorageOut,((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());
	if( iRet != M4_SUCCESS ) {
		return( NULL ) ;
	}

	// writes the command type
	m4int16_t ieCommand = (m4int16_t)ClProxyManager::PROXY_GET_DEBUG_VIEW;
	iRet = IOD.Write(ieCommand);
	if( iRet != M4_SUCCESS ) {
		return( NULL ) ;
	}

	// writes the proxy name
	iRet = IOD.Write(m_pcUniqueID);
	if( iRet != M4_SUCCESS ) {
		return( NULL ) ;
	}

	// writes the remote channel id
	iRet = IOD.Write(ai_iRemoteId);
	if( iRet != M4_SUCCESS ) {
		return( NULL ) ;
	}

	// sends the request to server
	m4handle_t hRequestID = 0;
	iRet = poClClientProxyManager->GetClientAPI()->SyncRequest(hContextID,  0,  0, poDataStorageOut, poDataStorageInp, hRequestID, m_pObjService->GetpRegistry()->GetGenCSTimeOut());
	if( iRet != M4_SUCCESS ) {
		return( NULL ) ;
	}

	iRet = IOD.Init(GEN_IO_MODE_READ,poDataStorageInp,((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());
	if( iRet != M4_SUCCESS ) {
		return( NULL ) ;
	}
	
	if (IOD.Read(iRet) != M4_SUCCESS) {
		return( NULL ) ;
	}
	if( iRet != M4_SUCCESS ) {
		return( NULL ) ;
	}

	// generates the directory
	ClCMObjectDirectory objDir(pChannelManager);

	// deserialize the directory
	iRet = objDir.DeSerialize(IOD, NULL, M4_TRUE);
	IOD.End();
	if( iRet != M4_SUCCESS ) {
		return( NULL ) ;
	}

	// Releases memory.
	if (poDataStorageInp) {
		M4DataStorage::DeleteDataStorage(poDataStorageInp); 
	}
	if (poDataStorageOut) {
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
	}

	m4int16_t iChannelIndex = objDir.GetFirstObjectIndex( ChannelObjectType ) ;

	if( iChannelIndex == -1 )
	{
		return( NULL ) ;
	}

	/* Bug 0155794
	El objdir no debe destruir este canal
	*/
	objDir.SetKeepAlive( iChannelIndex, M4_TRUE ) ;

	ClChannel* poDebugView = (ClChannel*) objDir.GetObjectAddress( iChannelIndex ) ;

	// Changes channel state.
	// A partir de este momento el canal ya no hace más transacciones al servidor.
	poDebugView->SetDebugMode(ClChannel::IS_DEBUG_VIEW);

	// Garantizamos que la AutoLoad no está activada
	iRet = poDebugView->SetAutoLoadMode(M4_FALSE);
	if( iRet != M4_SUCCESS ) {
		poDebugView->Destroy() ;
		return( NULL ) ;
	}

	return poDebugView;
  //## end ClProxySpace::GetProxyDebugView%898022272.body
}

m4return_t ClProxySpace::EndDebugMethod ()
{
  //## begin ClProxySpace::EndDebugMethod%964538634.body preserve=yes
	// the server shouldn´t ask for a view
	if (GetProxyManager()->IsServerMode() == M4_TRUE) {
		return M4_ERROR;
	}

	// checks the session
	ClClientProxyManager *poClClientProxyManager = (ClClientProxyManager*) GetProxyManager();
	if (!poClClientProxyManager) {
		return M4_ERROR;
	}

	// gets the context
	m4handle_t hContextID = poClClientProxyManager->GetContext();
	if (!hContextID) {
		return M4_ERROR;
	}

	// generates the query
	M4DataStorage* poDataStorageOut = M4DataStorage::GetNewDataStorage();
	M4DataStorage* poDataStorageInp = M4DataStorage::GetNewDataStorage();
	ClDataStoreIODriver IOD;

	m4return_t iRet = IOD.Init(GEN_IO_MODE_WRITE, poDataStorageOut, ((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());
	if (iRet != M4_SUCCESS) return iRet;

	// writes the command type
	m4int16_t ieCommand = (m4int16_t)ClProxyManager::PROXY_END_DEBUG_METHOD;
	iRet = IOD.Write(ieCommand);
	if (iRet != M4_SUCCESS) return iRet;

	// writes the proxy name
	iRet = IOD.Write(m_pcUniqueID);
	if (iRet != M4_SUCCESS) return iRet;

	ClChannelManager* const pChannelManager = GetChannelManager();
	ClChannelDirectory& chanDir = pChannelManager->Channel;
	ClCMObjectDirectory objDir(pChannelManager);

	/*
	 * Dejamos en el estado correcto a los canales.
	 * Dependiendo del estado del canal se harán las siguientes acciones:
	 * IS_NOT_DEBUG - Como no era un canal de depuración no se hace nada con él.
	 * IS_DEBUG_FIRST_TIME - Es la vista Front de un canal que hemos depurado su vista Back. Todavía falta la última
	 *		transacción para sicronizarla con el servidor (para esto el canal se pasa a IS_DEBUG_LAST_TIME,
	 *		y se añade al objDir). También hay que sincronizar sus accesos que tengan remoteId.
	 * IS_DEBUG - El canal es una vista Back que hemos depurado. Hay que borrarlo (sólo si tiene un solo acceso).
	 *		Si tiene más de un acceso es que lo tienen cogido las presentaciones. Borramos el acceso que se
	 *		utilizó para depurar y ponemos el canal como IS_NOT_DEBUG. El canal se borrará cuando lo borren
	 *		las presentaciones.
	 * IS_DEBUG_LAST_TIME - Es un error, no puede quedar ningún canal en este estado de una transacción anterior.
	 * IS_DEBUG_VIEW - Es una vista del inspector, se deja como está.
	 */
	ClChannelEraser channelEraser;
	ClChannel* pChannel = 0;
	ClAccess* pAccess = 0;
	m4int16_t j = -1;
	m4uint32_t k = 0;
	m4uint32_t iRemoteId = 0;
	m4uint32_t iAccessCount = 0;
	m4uint32_t iChannelCount = chanDir.Count();
	m4uint32_t i;

	for (i = 0; i < iChannelCount; i++) {
		pChannel = chanDir[i];
		switch (pChannel->GetDebugMode()) {
		case ClChannel::IS_NOT_DEBUG:
		case ClChannel::IS_DEBUG_VIEW:
			break;

		case ClChannel::IS_DEBUG_FIRST_TIME:
			{
				// Hay que comprobar si tiene remoteId. Si no lo tiene es porque ha habido algún error y el canal
				// no llego a crearse en el servidor (ej. si ha habido un error por la versión del metacanal).
				iRemoteId = pChannel->GetRemoteId();
				if (iRemoteId) {
					pChannel->SetDebugMode(ClChannel::IS_DEBUG_LAST_TIME);
					j = objDir.IncludeChannel(pChannel, iRemoteId);
					if (j == -1) return M4_ERROR;
					objDir.SetDoSerialize(j, M4_FALSE);

					iAccessCount = pChannel->Access.Count();
					for (k = 0; k < iAccessCount; k++) {
						pAccess = pChannel->Access[k];
						iRemoteId = pAccess->GetRemoteId();
						if (iRemoteId) {
							j = objDir.IncludeAccessAndCreateOID(pAccess, iRemoteId);
							if (j == -1) return M4_ERROR;
							objDir.SetDoSerialize(j, M4_FALSE);	 // No hace falta mandar los objetos al servidor, sólo nos interesa al vuelta.
						}
					}
				} else {
					pChannel->SetDebugMode(ClChannel::IS_NOT_DEBUG);
				}

				//Si existe al menos un canal con este modo de depuración, indica que estamos en la última sincronización
				// de en la ejecución de un método mapped en la cual hemos utilizado el espacio de depuración para guardar
				// partes back de canales.
				//Prohibimos que desde el espacio de depuración se transporte ninguna relación de nivel 2. Podrían
				// existir si estamos detenidos en un punto de ruptura de un método en el server y le damos al botón
				// de continuar la ejecución. Si en el server se hay relaciones de nivel 2 entre los canales involucra
				// dos en la ejecución que ya enviamos en el objdir desde el espacio de depuración, cuando se termina
				// la ejecución mapped, no se pasa por el PrepareExecutionInterruptionReturn y se podrían haber enviado
				// las relaciones de nivel 2 al espacio de depuración.
				objDir.SetbProhibitL2RelationSerialize(M4_TRUE);

				break;
			}

		case ClChannel::IS_DEBUG:

			pChannel->SetRemoteId( 0 ) ;
			pChannel->SetDebugMode( ClChannel::IS_NOT_DEBUG ) ;

			/* Bug 0061405
			El canal sesión no se debe borrar
			*/
			if( pChannelManager!= NULL && pChannel != pChannelManager->GetSessionChannel() )
			{
				/* Bug 0069602
				Hay que borrar todos los accesos que sean de depuración
				Nos basamos en que tengan modo de depuración
				*/
				iAccessCount = pChannel->Access.Count() ;

				for( k = 0 ; k < iAccessCount ; k++ )
				{
					// Ojo que hay que borrar de atrás adelante
					pAccess = pChannel->Access[ iAccessCount - k - 1 ] ;

					/* Bug 0069695
					Se borran los accesos que sean de depuración
					*/
					if( pAccess == NULL || pAccess->GetDebugMode() != ClChannel::IS_DEBUG )
					{
						continue;
					}

					pAccess->SetRemoteId( 0 ) ;
					pAccess->SetDebugMode( ClChannel::IS_NOT_DEBUG ) ;

					// Bug 0059142.
					// Sólo destruyo el acceso si no es de nivel2, en tal caso lo hará el channelEraser.
					if( pChannelManager->IsLevel2Access( pAccess ) == M4_FALSE )
					{
						pAccess->Destroy() ;
					}
				}

				// Si el canal no tiene más accesos hay que borrarlo
				iAccessCount = pChannel->Access.Count() ;

				if( iAccessCount == 0 )
				{
					iRet = channelEraser.Add( pChannel ) ;
					if( iRet != M4_SUCCESS )
					{
						return( iRet ) ;
					}
				} 
			}

			break;

		case ClChannel::IS_DEBUG_LAST_TIME:
			// No puede haber ninguno pendiente de una transacción anterior.

		default:
			return M4_ERROR;
		}
	}

	iRet = channelEraser.Destroy(M4_FALSE);		// M4_FALSE para que no borre los accesos que son referencias externas.
	if (iRet != M4_SUCCESS) return iRet;

	if (objDir.GetCount()) {
		iRet = objDir.Serialize(IOD);
		IOD.End();
		if (iRet != M4_SUCCESS) return iRet;

		// sends the request to server
		m4handle_t hRequestID = 0;
		iRet = poClClientProxyManager->GetClientAPI()->SyncRequest(hContextID, 0, 0, poDataStorageOut, poDataStorageInp, hRequestID, m_pObjService->GetpRegistry()->GetGenCSTimeOut());
		if (iRet != M4_SUCCESS) return iRet;

		iRet = IOD.Init(GEN_IO_MODE_READ, poDataStorageInp, poClClientProxyManager->GetServerArch());
		if (iRet != M4_SUCCESS) return iRet;
		
		if (IOD.Read(iRet) != M4_SUCCESS) return M4_ERROR;
		if (iRet != M4_SUCCESS) return iRet;

		// deserialize the directory
		// Bug 0308265 No se deserializa el estado de las estadísticas
		iRet = objDir.DeSerialize(IOD, NULL, M4_FALSE);
		if (iRet != M4_SUCCESS) return iRet;

		// Como ya ha terminado la depuración del método remoto, tengo que poner el remoteId de los canales OLTP a 0.
		// Para PROXY no hay que resetear
		// Para AUTOS hay que resetear sólo los accesos y dejar los canales
		j = objDir.GetFirstObjectIndex(ChannelObjectType);
		for (; j != -1; j = objDir.GetNextObjectIndex()) {
			pChannel = (ClChannel*)objDir.GetObjectAddress(j);

			m4uint8_t iCsExeType = pChannel->GetpChannelDef()->CsExeType() ;
			if (iCsExeType == M4CL_CSEXETYPE_OLTP || iCsExeType == M4CL_CSEXETYPE_AUTO) {

				if( iCsExeType == M4CL_CSEXETYPE_OLTP )
				{
					pChannel->SetRemoteId(0);
				}

				iAccessCount = pChannel->Access.Count();
				for (i = 0; i < iAccessCount; i++) {
					pChannel->Access[i]->SetRemoteId(0);
				}
			}
		}
	}
	IOD.End();

	// Releases memory.
	if (poDataStorageInp) {
		M4DataStorage::DeleteDataStorage(poDataStorageInp); 
	}
	if (poDataStorageOut) {
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
	}

	return iRet;
  //## end ClProxySpace::EndDebugMethod%964538634.body
}

m4bool_t ClProxySpace::IsServerMode ()
{
  //## begin ClProxySpace::IsServerMode%896103145.body preserve=yes
	return m_poClProxyManager->IsServerMode();
  //## end ClProxySpace::IsServerMode%896103145.body
}

ClProxyList * ClProxySpace::GetProxyList ()
{
  //## begin ClProxySpace::GetProxyList%897898147.body preserve=yes
	if (GetProxyManager()->IsServerMode() == M4_TRUE) {
		return NULL;
	}


	// generates a new one
	if (m_poClProxyList != NULL)
	{
		delete m_poClProxyList;
		m_poClProxyList = NULL;
	}

	// checks the session
	ClClientProxyManager *poClClientProxyManager = (ClClientProxyManager*) GetProxyManager();
	
	if (poClClientProxyManager == NULL)
		return NULL;

	// gets the session and service
	m4handle_t hContextID = poClClientProxyManager->GetContext();
	if (hContextID == 0)
		return NULL;

	// generates the query
	M4DataStorage *poDataStorageOut = M4DataStorage::GetNewDataStorage();
	M4DataStorage *poDataStorageInp = M4DataStorage::GetNewDataStorage();

	ClDataStoreIODriver IODOut;
	m4return_t iResult;
	iResult = IODOut.Init(GEN_IO_MODE_WRITE,poDataStorageOut,((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());

	if (iResult != M4_SUCCESS)
		return NULL;

	// writes the command type
	m4int16_t ieCommand = (m4int16_t)ClProxyManager::PROXY_GET_LIST;
	if (IODOut.Write(ieCommand) != M4_SUCCESS)
		return NULL;

	// writes the proxy name
	if (IODOut.Write(m_pcUniqueID) != M4_SUCCESS)
		return NULL;

	IODOut.End();

	// sends the request to server
	m4handle_t hRequestID = 0;
	iResult = poClClientProxyManager->GetClientAPI()->SyncRequest(hContextID,  0,  0, poDataStorageOut, poDataStorageInp, hRequestID, m_pObjService->GetpRegistry()->GetGenCSTimeOut());

	if (iResult != M4_SUCCESS)
		return NULL;

	ClDataStoreIODriver IODInp;
	iResult = IODInp.Init(GEN_IO_MODE_READ,poDataStorageInp,((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());

	if (iResult != M4_SUCCESS)
		return NULL;

	if (IODInp.Read(iResult) != M4_SUCCESS)
		return NULL;
	if (iResult != M4_SUCCESS)
		return NULL;

	// reads the protocol
	iResult = ReadProxyList(&IODInp);
	IODInp.End();
	if (iResult != M4_SUCCESS)
		return NULL;

	// Releases memory.
	if (poDataStorageInp) {
		M4DataStorage::DeleteDataStorage(poDataStorageInp); 
	}
	if (poDataStorageOut) {
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
	}

	return m_poClProxyList;
  //## end ClProxySpace::GetProxyList%897898147.body
}

m4return_t ClProxySpace::ReadProxyList (ClDataStoreIODriver *ai_poIOD, ClProxyList *ai_poClProxyList)
{
  //## begin ClProxySpace::ReadProxyList%897898148.body preserve=yes
	// the server shouldn´t ask for a ReadProxyList
	if (GetProxyManager()->IsServerMode() == M4_TRUE) {
		return M4_ERROR;
	}

	// gets the number of remote objects
	m4int32_t iNChannels=0;
	m4return_t iResult = ai_poIOD->Read(iNChannels);
	if (iResult != M4_SUCCESS)
		return iResult;

	// loops
	m4bool_t bDoChild = M4_TRUE;
	for (m4int32_t iCount = 0; iCount < iNChannels; iCount++) {
		// instances the debug list
		ClProxyList *poActual = new ClProxyList;
		if (poActual == NULL)
			return M4_ERROR;

		// reads the id
		m4char_t pcId[PROXY_MAX_VIEW_ID] = "";
		iResult = ai_poIOD->Read(pcId);
		if (iResult != M4_SUCCESS)
			return iResult;

		// sets the id
		iResult = poActual->SetId(pcId);
		if (iResult != M4_SUCCESS)
			return iResult;

		// reads the unique id
		m4uint32_t iRemoteId;
		iResult = ai_poIOD->Read(iRemoteId);
		if (iResult != M4_SUCCESS) return iResult;

		// sets the id
		poActual->SetRemoteUniqueId(iRemoteId);

		// sets the local unique id
		ClChannelDirectory& channelDirectory = m_pObjService->GetpChannelManager()->Channel;
		m4int32_t iNChannels2 = channelDirectory.Count();
		for (m4int32_t iCount2 = 0; iCount2 < iNChannels2; iCount2++) {
			ClChannel *poClChannel = channelDirectory[iCount2];
			if (poClChannel->IsBuilt() == M4_FALSE)
				continue;

			m4uint32_t iId2 = poClChannel->GetRemoteId();
			if (!iId2) continue;

			if (iRemoteId == iId2) {
				poActual->SetLocalUniqueId(poClChannel->GetHandle());
				break;
			}
		}

		// reads the child's
		iResult = ReadProxyList(ai_poIOD,poActual);
		if (iResult != M4_SUCCESS) return iResult;

		// inserts at the same level
		if (ai_poClProxyList == NULL) {
			if (m_poClProxyList == NULL) {
				m_poClProxyList = poActual;
			} else {
				iResult = m_poClProxyList->Insert(poActual);
			}
		} else {
			if (bDoChild == M4_TRUE) {
				iResult = ai_poClProxyList->InsertChild(poActual);
			} else {
				iResult = ai_poClProxyList->Insert(poActual);
			}
		}

		// resets flag
		if (bDoChild == M4_TRUE)
			bDoChild = M4_FALSE;

		// checks the error status
		if (iResult != M4_SUCCESS)
			return iResult;

		// resets references
		ai_poClProxyList = poActual;
	}

	// returns
	return M4_SUCCESS;

  //## end ClProxySpace::ReadProxyList%897898148.body
}

m4uint32_t ClProxySpace::GetSessionId ()
{
  //## begin ClProxySpace::GetSessionId%901533963.body preserve=yes
	return m_iSessionId;
  //## end ClProxySpace::GetSessionId%901533963.body
}

void ClProxySpace::RequestToAbort ()
{
  //## begin ClProxySpace::RequestToAbort%928757333.body preserve=yes

	// checks the executor
	if (m_pObjService != NULL)
		m_pObjService->GetpExecutor()->RequestToAbortWithClStatus(M4_TRUE);

	SetUseStatus(M4_FALSE);
	SetRequestToAbort(M4_TRUE);
  //## end ClProxySpace::RequestToAbort%928757333.body
}

m4return_t ClProxySpace::SetSessionId (m4uint32_t ai_iSessionId, m4pcchar_t ai_pcUserName)
{
  //## begin ClProxySpace::SetSessionId%901533964.body preserve=yes

	// checks the reset mode
	if (ai_iSessionId == 0)
	{
		if (m_iSessionId)
		{
#ifdef _DEBUG	
			cout << "*** Disconnecting from session id " << m_iSessionId << endl;
#endif
			m_iSessionId = 0;
			SetUserName("NO USER");
		}

		return SetRequestToAbort(M4_FALSE);
	}

	if (m_iSessionId != 0 && m_iSessionId != ai_iSessionId)
		return M4_ERROR;
	if (m_iSessionId == 0)
	{
		m_iSessionId = ai_iSessionId;
		SetUserName(ai_pcUserName);
#ifdef _DEBUG	
		cout << "*** Connected to session id " << m_iSessionId << endl;
#endif
	}

	return M4_SUCCESS;
  //## end ClProxySpace::SetSessionId%901533964.body
}

m4pcchar_t ClProxySpace::GetUserName ()
{
  //## begin ClProxySpace::GetUserName%931259995.body preserve=yes
	return m_pcUserName;
  //## end ClProxySpace::GetUserName%931259995.body
}

m4return_t ClProxySpace::FreeLogicalConnection ()
{
  //## begin ClProxySpace::FreeLogicalConnection%958471562.body preserve=yes
	// Esta función sólo debe llamarse desde el servidor.
	return M4_ERROR;
  //## end ClProxySpace::FreeLogicalConnection%958471562.body
}

m4return_t ClProxySpace::EraseAllL2Instances (m4uint32_t ai_iRemoteId)
{
  //## begin ClProxySpace::EraseAllL2Instances%1025256484.body preserve=yes

	// checks arguments
	if (!ai_iRemoteId) {
		return M4_ERROR;
	}

	// the server shouldn´t ask for a view
	if (GetProxyManager()->IsServerMode() == M4_TRUE) {
		return M4_ERROR;
	}

	// checks the session
	ClClientProxyManager *poClClientProxyManager = (ClClientProxyManager*) GetProxyManager();
	if (!poClClientProxyManager) {
		return M4_ERROR;
	}

	// gets the context
	m4handle_t hContextID = poClClientProxyManager->GetContext();
	if (hContextID == 0) {
		return M4_ERROR;
	}

	// generates the query
	M4DataStorage* poDataStorageOut = M4DataStorage::GetNewDataStorage();
	M4DataStorage* poDataStorageInp = M4DataStorage::GetNewDataStorage();
	ClDataStoreIODriver IOD;

	m4return_t iRet = IOD.Init(GEN_IO_MODE_WRITE,poDataStorageOut,((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());
	if (iRet != M4_SUCCESS) return M4_ERROR;

	// writes the command type
	m4int16_t ieCommand = (m4int16_t)ClProxyManager::PROXY_ERASE_ALL_L2_INSTANCES;
	iRet = IOD.Write(ieCommand);
	if (iRet != M4_SUCCESS) return M4_ERROR;

	// writes the proxy name
	iRet = IOD.Write(m_pcUniqueID);
	if (iRet != M4_SUCCESS) return M4_ERROR;

	// writes the remote channel id
	iRet = IOD.Write(ai_iRemoteId);
	if (iRet != M4_SUCCESS) return M4_ERROR;

	// sends the request to server
	m4handle_t hRequestID = 0;
	iRet = poClClientProxyManager->GetClientAPI()->SyncRequest(hContextID,  0,  0, poDataStorageOut, poDataStorageInp, hRequestID, m_pObjService->GetpRegistry()->GetGenCSTimeOut());
	if (iRet != M4_SUCCESS) return M4_ERROR;

	iRet = IOD.Init(GEN_IO_MODE_READ,poDataStorageInp,((ClClientProxyManager*)m_poClProxyManager)->GetServerArch());
	if (iRet != M4_SUCCESS) return M4_ERROR;
	
	if (IOD.Read(iRet) != M4_SUCCESS) {
		return M4_ERROR;
	}
	if (iRet != M4_SUCCESS) return M4_ERROR;

	// Releases memory.
	if (poDataStorageInp) {
		M4DataStorage::DeleteDataStorage(poDataStorageInp); 
	}
	if (poDataStorageOut) {
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
	}

	return M4_SUCCESS;

  //## end ClProxySpace::EraseAllL2Instances%1025256484.body
}

m4return_t ClProxySpace::ReleaseSessionAcceses ()
{
  //## begin ClProxySpace::ReleaseSessionAcceses%1025256485.body preserve=yes

  /* Bug 0063193
  Función para liberar los accessos de nivel dos sobre el canal sesión
  Lo que se hace es volverlos a atachar al canal sesión
  y borrarlos al liberar el canal sesión del channelmanager
	*/

	m4int32_t			i = 0 ;
	m4int32_t			iLength = 0 ;
	ClAccess			*poAccess = NULL ;
	ClChannel			*poSessionChannel = NULL ;
	ClChannelManager	*poChannelManager = NULL ;

	iLength = m_vpSessionAccesses.size() ;
	if( iLength > 0 )
	{
		poChannelManager = GetChannelManager() ;
		poAccess = m_vpSessionAccesses[ 0 ] ;

		if( poChannelManager == NULL || poAccess == NULL )
		{
			return( M4_ERROR ) ;
		}

		poSessionChannel = poAccess->GetpChannel() ;

		if( poSessionChannel == NULL )
		{
			return( M4_ERROR ) ;
		}

		poSessionChannel->Attach( poChannelManager ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poAccess = m_vpSessionAccesses[ iLength - i - 1 ] ;

			if( poAccess != NULL )
			{
				poSessionChannel->Access._Add( poAccess ) ;
			}
		}

		m_vpSessionAccesses.clear() ;

		if( poSessionChannel->Detach() != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_SRV_SESSION_CHANNEL , "Proxy: Cannot detach session channel" ) ;
			return( M4_ERROR ) ;
		}

	}

	return( M4_SUCCESS ) ;

  //## end ClProxySpace::ReleaseSessionAcceses%1025256485.body
}

// Class ClClientProxyManager 






ClClientProxyManager::ClClientProxyManager (ClM4ObjService* const ai_pObjService, ClAppClientAdap* const ai_pClClientAPI)
  //## begin ClClientProxyManager::ClClientProxyManager%889780468.hasinit preserve=no
      : m_hContextID(0), m_iServerArch(-1), m_poClientAPI(ai_pClClientAPI), m_pObjService(ai_pObjService)
  //## end ClClientProxyManager::ClClientProxyManager%889780468.hasinit
  //## begin ClClientProxyManager::ClClientProxyManager%889780468.initialization preserve=yes
  //## end ClClientProxyManager::ClClientProxyManager%889780468.initialization
{
  //## begin ClClientProxyManager::ClClientProxyManager%889780468.body preserve=yes

	m4uint8_t i ;

	for ( i = 0; i < MAX_NUM_PROXY_SPACES; i++)
		m_vProxySpace[i] = 0;
  //## end ClClientProxyManager::ClClientProxyManager%889780468.body
}


ClClientProxyManager::~ClClientProxyManager ()
{
  //## begin ClClientProxyManager::~ClClientProxyManager%889780469.body preserve=yes
	for (m4uint8_t i = 0; i < MAX_NUM_PROXY_SPACES; i++) {
		if (m_vProxySpace[i]) {
			delete m_vProxySpace[i];
			m_vProxySpace[i] = 0;
		}
	}
  //## end ClClientProxyManager::~ClClientProxyManager%889780469.body
}



//## Other Operations (implementation)
ClProxySpace* const ClClientProxyManager::GetSpace (const m4char_t* const ai_pcClProxySpaceID, const m4bool_t ai_bCreateNew)
{
  //## begin ClClientProxyManager::GetSpace%889702424.body preserve=yes
	m4uint8_t spaceIndex = 0;
	if (strcmp(ai_pcClProxySpaceID, "Proxy")) {
		spaceIndex = 1;
	}

	if (m_vProxySpace[spaceIndex] != NULL)
		return m_vProxySpace[spaceIndex];
	
	if (!ai_bCreateNew)
		return NULL;

	m_vProxySpace[spaceIndex] = CreateSpace(ai_pcClProxySpaceID);
	
	return m_vProxySpace[spaceIndex];
  //## end ClClientProxyManager::GetSpace%889702424.body
}

m4return_t ClClientProxyManager::ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock)
{
  //## begin ClClientProxyManager::ReleaseSpace%889702425.body preserve=yes

	m4return_t			iResult = M4_ERROR;
	m4uint8_t			i = 0;
	m4uint16_t			iLength = 0;
	m4handle_t			hRequestID = 0;
	m4pchar_t			pccUniqueId = NULL;
	m4pchar_t			pccTemp = NULL;
	ClDataStoreIODriver IODOut;
	ClDataStoreIODriver IODInp;


	if( ai_poClProxySpace == NULL )
	{
		return( M4_ERROR ) ;
	}

	/* Bug 0129436
	Primero se guarda el id del espacio y se borra en cliente
	*/
	pccUniqueId = ai_poClProxySpace->GetUniqueID() ;

	if( pccUniqueId != NULL )
	{
		iLength = strlen( pccUniqueId ) ;
		pccTemp = new m4char_t[ iLength + 1 ] ;

		if( pccTemp != NULL )
		{
			memcpy( pccTemp, pccUniqueId, iLength + 1 ) ;
		}
	}

	// Deletes the proxy space
	for( i = 0 ; i < MAX_NUM_PROXY_SPACES ; i++ )
	{
		if( m_vProxySpace[i] == ai_poClProxySpace )
		{
			delete m_vProxySpace[i] ;
			m_vProxySpace[i] = NULL ;
			break;
		}
	}

	if( pccTemp == NULL )
	{
		return( M4_ERROR ) ;
	}


	// generates the codified stream
	M4DataStorage *poDataStorageOut = M4DataStorage::GetNewDataStorage();
	iResult = IODOut.Init(GEN_IO_MODE_WRITE, poDataStorageOut, m_iServerArch);

	if (iResult != M4_SUCCESS)
	{
		delete [] pccTemp ;
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
		return iResult;
	}

	// writes the command type
	m4int16_t ieCommand = (m4int16_t)ClProxyManager::PROXY_RELEASE_SPACE;
	iResult = IODOut.Write(ieCommand);

	if( iResult != M4_SUCCESS)
	{
		delete [] pccTemp ;
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
		return iResult;
	}

	// writes the proxy name
	iResult = IODOut.Write(pccTemp);
	delete [] pccTemp ;

	if( iResult != M4_SUCCESS)
	{
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
		return iResult;
	}

	IODOut.End();

	// sends the request to server
	M4DataStorage *poDataStorageInp = M4DataStorage::GetNewDataStorage();

	iResult = m_poClientAPI->SyncRequest(m_hContextID,  0,  0, poDataStorageOut, poDataStorageInp, hRequestID, m_pObjService->GetpRegistry()->GetGenCSTimeOut());
	M4DataStorage::DeleteDataStorage(poDataStorageOut); 

	if (iResult != M4_SUCCESS)
	{
		M4DataStorage::DeleteDataStorage(poDataStorageInp); 
		return iResult;
	}

	iResult = IODInp.Init(GEN_IO_MODE_READ, poDataStorageInp, m_iServerArch);

	if (iResult != M4_SUCCESS)
	{
		M4DataStorage::DeleteDataStorage(poDataStorageInp); 
		return iResult;
	}

	IODInp.Read(iResult);
	IODInp.End();

	M4DataStorage::DeleteDataStorage(poDataStorageInp); 

	return iResult;
  //## end ClClientProxyManager::ReleaseSpace%889702425.body
}

ClAppClientAdap* const ClClientProxyManager::GetClientAPI ()
{
  //## begin ClClientProxyManager::GetClientAPI%896180929.body preserve=yes
	return m_poClientAPI;
  //## end ClClientProxyManager::GetClientAPI%896180929.body
}

m4handle_t ClClientProxyManager::GetContext () const
{
  //## begin ClClientProxyManager::GetContext%896180931.body preserve=yes
	return m_hContextID;
  //## end ClClientProxyManager::GetContext%896180931.body
}

void ClClientProxyManager::SetServerArch (const m4uint16_t ai_iServerArch)
{
  //## begin ClClientProxyManager::SetServerArch%897554919.body preserve=yes
	m_iServerArch = ai_iServerArch;
  //## end ClClientProxyManager::SetServerArch%897554919.body
}

m4uint16_t ClClientProxyManager::GetServerArch () const
{
  //## begin ClClientProxyManager::GetServerArch%897554920.body preserve=yes
	return m_iServerArch;
  //## end ClClientProxyManager::GetServerArch%897554920.body
}

m4return_t ClClientProxyManager::ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID)
{
  //## begin ClClientProxyManager::ReleaseActiveSpace%949597711.body preserve=yes
	return M4_SUCCESS;
  //## end ClClientProxyManager::ReleaseActiveSpace%949597711.body
}

ClProxySpace* const ClClientProxyManager::CreateSpace (const m4char_t* const ai_pProxySpaceID)
{
  //## begin ClClientProxyManager::CreateSpace%958471561.body preserve=yes
	m4return_t iResult = M4_ERROR;
	
	if (m_hContextID == 0) {
		m_iServerArch = m_pObjService->GetpLogonAdaptor()->GetServerArchitecture();
		iResult = m_poClientAPI->OpenService(M4CL_PROXY_SERVICE, m_hContextID);
		if (M4_SUCCESS != iResult)
			return NULL;
	}
	

	// generates the codified stream
	M4DataStorage *poDataStorageOut = M4DataStorage::GetNewDataStorage();
	M4DataStorage *poDataStorageInp = M4DataStorage::GetNewDataStorage();
	
	ClDataStoreIODriver IODOut;
	iResult = IODOut.Init(GEN_IO_MODE_WRITE,poDataStorageOut, m_iServerArch);
	
	if (iResult != M4_SUCCESS)
		return NULL;
	
	// Writes the command type.
	m4int16_t ieCommand = (m4int16_t)ClProxyManager::PROXY_GET_SPACE;
	if (IODOut.Write(ieCommand) != M4_SUCCESS)
		return NULL;
	
	// Writes the proxy name.
	if (IODOut.Write(ai_pProxySpaceID) != M4_SUCCESS)
		return NULL;
	
	IODOut.End();
	
	// Sends the request to server.
	m4handle_t hRequestID = 0;
	iResult = m_poClientAPI->SyncRequest(m_hContextID, 0, 0, poDataStorageOut, poDataStorageInp, hRequestID, m_pObjService->GetpRegistry()->GetGenCSTimeOut());
	if (iResult != M4_SUCCESS)
		return NULL;
	
	ClDataStoreIODriver IODInp;
	iResult = IODInp.Init(GEN_IO_MODE_READ,poDataStorageInp, m_iServerArch);
	
	if (iResult != M4_SUCCESS)
		return NULL;
	
	IODInp.Read(iResult);
	if (iResult != M4_SUCCESS)
		return NULL;
	
	m4char_t pcRemoteProxyID[PROXY_MAX_ID];
	IODInp.Read(pcRemoteProxyID);
	if (iResult != M4_SUCCESS)
		return NULL;
	
	IODInp.End();
	
	
	// generates the new proxy space
	ClProxySpace* poClProxySpace = new ClProxySpace(this, m_pObjService);
	if (poClProxySpace == NULL)
		return NULL;
	
	poClProxySpace->SetBaseID(ai_pProxySpaceID);
	poClProxySpace->SetUniqueID(pcRemoteProxyID);

	// Releases memory.
	if (poDataStorageInp) {
		M4DataStorage::DeleteDataStorage(poDataStorageInp); 
	}
	if (poDataStorageOut) {
		M4DataStorage::DeleteDataStorage(poDataStorageOut); 
	}

	return poClProxySpace;
  //## end ClClientProxyManager::CreateSpace%958471561.body
}

void ClClientProxyManager::Reset ()
{
  //## begin ClClientProxyManager::Reset%994165400.body preserve=yes
	// reset conexion handle
	m_hContextID = 0;
  //## end ClClientProxyManager::Reset%994165400.body
}

// Class ClProxyList 







ClProxyList::ClProxyList ()
  //## begin ClProxyList::ClProxyList%897898134.hasinit preserve=no
      : m_poNext(NULL), m_poChild(NULL), m_pcId(NULL), m_iLocalUniqueId(0), m_iRemoteUniqueId(0)
  //## end ClProxyList::ClProxyList%897898134.hasinit
  //## begin ClProxyList::ClProxyList%897898134.initialization preserve=yes
  //## end ClProxyList::ClProxyList%897898134.initialization
{
  //## begin ClProxyList::ClProxyList%897898134.body preserve=yes
  //## end ClProxyList::ClProxyList%897898134.body
}


ClProxyList::~ClProxyList ()
{
  //## begin ClProxyList::~ClProxyList%897898135.body preserve=yes
	// frees the next element
	if (m_poNext != NULL)
	{
		delete m_poNext;
		m_poNext = NULL;
	}

	// frees the childs
	if (m_poChild != NULL)
	{
		delete m_poChild;
		m_poChild = NULL;
	}
	
	// deletes members
	if (m_pcId != NULL)
	{
		delete [] m_pcId;
		m_pcId = NULL;
	}
  //## end ClProxyList::~ClProxyList%897898135.body
}



//## Other Operations (implementation)
m4return_t ClProxyList::SetId (m4pchar_t ai_pcId)
{
  //## begin ClProxyList::SetId%897898136.body preserve=yes

	// copies the entry
	m_pcId = new m4char_t [strlen(ai_pcId) + 1];
	if (m_pcId == NULL)
		return M4_ERROR;
	strcpy(m_pcId,ai_pcId);
	return M4_SUCCESS;
  //## end ClProxyList::SetId%897898136.body
}

void ClProxyList::SetLocalUniqueId (m4uint32_t ai_iLocalUniqueId)
{
  //## begin ClProxyList::SetLocalUniqueId%897898137.body preserve=yes
	m_iLocalUniqueId = ai_iLocalUniqueId;
  //## end ClProxyList::SetLocalUniqueId%897898137.body
}

void ClProxyList::SetRemoteUniqueId (m4uint32_t ai_iRemoteUniqueId)
{
  //## begin ClProxyList::SetRemoteUniqueId%897898138.body preserve=yes
	m_iRemoteUniqueId = ai_iRemoteUniqueId;
  //## end ClProxyList::SetRemoteUniqueId%897898138.body
}

m4return_t ClProxyList::Insert (ClProxyList *ai_poItem)
{
  //## begin ClProxyList::Insert%897898139.body preserve=yes
	// scans the last element
	ClProxyList *poActual = this;
	for(;;)
	{
		if (poActual->m_poNext == NULL)
			break;
		poActual = poActual->m_poNext;
	}

	// copies the element
	poActual->m_poNext = ai_poItem;
	return M4_SUCCESS;
  //## end ClProxyList::Insert%897898139.body
}

m4return_t ClProxyList::InsertChild (ClProxyList *ai_poItem)
{
  //## begin ClProxyList::InsertChild%897898145.body preserve=yes
	// inserts an element in the current leaf
	if (m_poChild == NULL)
	{
		m_poChild = ai_poItem;
		return M4_SUCCESS;
	}

	// inserts at the same level
	return m_poChild->Insert(ai_poItem);

  //## end ClProxyList::InsertChild%897898145.body
}

m4int32_t ClProxyList::Count ()
{
  //## begin ClProxyList::Count%897898140.body preserve=yes
	// counts elements in this leaf
	m4int32_t iCount = 1;
	ClProxyList *poActual = this;
	for (;;)
	{
		if (poActual->m_poNext == NULL)
			break;
		iCount++;
		poActual = poActual->m_poNext;
	}
	return iCount;
  //## end ClProxyList::Count%897898140.body
}

ClProxyList * ClProxyList::Get (m4int32_t ai_iIndex)
{
  //## begin ClProxyList::Get%897898141.body preserve=yes
	// gets the nth element in this leaf
	m4int32_t iCount = ai_iIndex;
	ClProxyList *poActual = this;
	while(iCount)
	{
		poActual = poActual->m_poNext;
		if (poActual == NULL)
			break;
		iCount--;
	}
	return poActual;
  //## end ClProxyList::Get%897898141.body
}

ClProxyList * ClProxyList::GetChild (m4int32_t ai_iIndex)
{
  //## begin ClProxyList::GetChild%897898146.body preserve=yes
	if (m_poChild == NULL)
		return NULL;
	return m_poChild->Get(ai_iIndex);
  //## end ClProxyList::GetChild%897898146.body
}

const m4pchar_t ClProxyList::GetId ()
{
  //## begin ClProxyList::GetId%897898142.body preserve=yes
	return m_pcId;
  //## end ClProxyList::GetId%897898142.body
}

m4uint32_t ClProxyList::GetLocalUniqueId () const
{
  //## begin ClProxyList::GetLocalUniqueId%897898143.body preserve=yes
	return m_iLocalUniqueId;
  //## end ClProxyList::GetLocalUniqueId%897898143.body
}

m4uint32_t ClProxyList::GetRemoteUniqueId () const
{
  //## begin ClProxyList::GetRemoteUniqueId%897898144.body preserve=yes
	return m_iRemoteUniqueId;
  //## end ClProxyList::GetRemoteUniqueId%897898144.body
}

// Additional Declarations
  //## begin ClProxyList%35800E7E014F.declarations preserve=yes
  //## end ClProxyList%35800E7E014F.declarations

// Class ClProxySpaceContainer 



ClProxySpaceContainer::ClProxySpaceContainer (ClProxySpace *ai_poClProxySpace)
  //## begin ClProxySpaceContainer::ClProxySpaceContainer%891866786.hasinit preserve=no
  //## end ClProxySpaceContainer::ClProxySpaceContainer%891866786.hasinit
  //## begin ClProxySpaceContainer::ClProxySpaceContainer%891866786.initialization preserve=yes
  : m_poClProxySpace(ai_poClProxySpace)
  //## end ClProxySpaceContainer::ClProxySpaceContainer%891866786.initialization
{
  //## begin ClProxySpaceContainer::ClProxySpaceContainer%891866786.body preserve=yes

	// initializes the key
	m_poClCacheKey = NULL;
  //## end ClProxySpaceContainer::ClProxySpaceContainer%891866786.body
}


ClProxySpaceContainer::~ClProxySpaceContainer ()
{
  //## begin ClProxySpaceContainer::~ClProxySpaceContainer%891866787.body preserve=yes
	m_poClCacheKey = NULL;
  //## end ClProxySpaceContainer::~ClProxySpaceContainer%891866787.body
}



//## Other Operations (implementation)
m4return_t ClProxySpaceContainer::GetKey (ClCacheKey * &  ao_poClCacheKey)
{
  //## begin ClProxySpaceContainer::GetKey%891866788.body preserve=yes
	if (m_poClCacheKey == NULL)
		return M4_ERROR;
	ao_poClCacheKey = m_poClCacheKey;
	return M4_SUCCESS;
  //## end ClProxySpaceContainer::GetKey%891866788.body
}

m4return_t ClProxySpaceContainer::SetKey (ClCacheKey *ai_poClCacheKey)
{
  //## begin ClProxySpaceContainer::SetKey%891866789.body preserve=yes
	m_poClCacheKey = ai_poClCacheKey;
	if (m_poClCacheKey == NULL)
		return M4_ERROR;
	return M4_SUCCESS;
  //## end ClProxySpaceContainer::SetKey%891866789.body
}

ClProxySpace * ClProxySpaceContainer::GetProxySpace ()
{
  //## begin ClProxySpaceContainer::GetProxySpace%891950692.body preserve=yes
	return m_poClProxySpace;
  //## end ClProxySpaceContainer::GetProxySpace%891950692.body
}

// Class ClServerProxyManagerEmul 



ClServerProxyManagerEmul::ClServerProxyManagerEmul ()
  //## begin ClServerProxyManagerEmul::ClServerProxyManagerEmul%949597696.hasinit preserve=no
      : m_poSessionChannel(NULL)
  //## end ClServerProxyManagerEmul::ClServerProxyManagerEmul%949597696.hasinit
  //## begin ClServerProxyManagerEmul::ClServerProxyManagerEmul%949597696.initialization preserve=yes
  //## end ClServerProxyManagerEmul::ClServerProxyManagerEmul%949597696.initialization
{
  //## begin ClServerProxyManagerEmul::ClServerProxyManagerEmul%949597696.body preserve=yes
	for (m4uint8_t i = 0; i < MAX_NUM_PROXY_SPACES; i++)
		m_vProxySpace[i] = 0;
  //## end ClServerProxyManagerEmul::ClServerProxyManagerEmul%949597696.body
}


ClServerProxyManagerEmul::~ClServerProxyManagerEmul ()
{
  //## begin ClServerProxyManagerEmul::~ClServerProxyManagerEmul%949597697.body preserve=yes
	//ShutDown correcto en Single Machine con canales Proxy (11/5/01)
	for (m4uint8_t i = 0; i < MAX_NUM_PROXY_SPACES; i++) {
		if (m_vProxySpace[i]) {
			delete m_vProxySpace[i];
			m_vProxySpace[i] = 0;
		}
	}
  //## end ClServerProxyManagerEmul::~ClServerProxyManagerEmul%949597697.body
}



//## Other Operations (implementation)
ClProxySpace* const ClServerProxyManagerEmul::GetSpace (const m4char_t* const ai_pcClProxySpaceID, const m4bool_t ai_bCreateNew)
{
  //## begin ClServerProxyManagerEmul::GetSpace%949597698.body preserve=yes
	m4uint8_t i ;
	ClProxySpace *poProxySpace = NULL;

	for ( i = 0; i < MAX_NUM_PROXY_SPACES; i++) {
		if (m_vProxySpace[i] && strcmp(m_vProxySpace[i]->GetUniqueID(), ai_pcClProxySpaceID) == 0)
		{
			poProxySpace = m_vProxySpace[i];
			break;
		}
	}

	if (poProxySpace != NULL)
	{
		poProxySpace->SetUseStatus(M4_TRUE);
		return poProxySpace;
	}
	
	// creates the space
	if (ai_bCreateNew == M4_FALSE) {
		// new error
		return NULL;
	}

	ClProxySpace* poClProxySpace = new ClProxySpace(this, NULL);

	if (poClProxySpace == NULL)
		return NULL;

	poClProxySpace->SetBaseID(ai_pcClProxySpaceID);
	poClProxySpace->SetUniqueID(ai_pcClProxySpaceID);

	// saves the reference
	for (i = 0; i < MAX_NUM_PROXY_SPACES; i++) {
		if (!m_vProxySpace[i]) {
			m_vProxySpace[i] = poClProxySpace;
			break;
		}
	}

	return poClProxySpace;
  //## end ClServerProxyManagerEmul::GetSpace%949597698.body
}

ClProxySpace * ClServerProxyManagerEmul::GetSpace (const m4uint32_t ai_iSessionId)
{
  //## begin ClServerProxyManagerEmul::GetSpace%949597699.body preserve=yes
	for (m4uint8_t i = 0; i < MAX_NUM_PROXY_SPACES; i++) {
		if (m_vProxySpace[i]->GetSessionId() == ai_iSessionId)
			return m_vProxySpace[i];
	}

	return NULL;
  //## end ClServerProxyManagerEmul::GetSpace%949597699.body
}

m4return_t ClServerProxyManagerEmul::ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock)
{
  //## begin ClServerProxyManagerEmul::ReleaseSpace%949597700.body preserve=yes
	//if (m_poProxySpace == NULL)
	//	return M4_SUCCESS;
	m4uint8_t i;
	ClProxySpace* pProxySpace = 0;

	for ( i = 0; i < MAX_NUM_PROXY_SPACES; i++) {
		if (m_vProxySpace[i] == ai_poClProxySpace)
			break;
	}

	m4return_t iResult = m_vProxySpace[i]->SetSessionChannel(NULL, 0, NULL);
	if (iResult != M4_SUCCESS)
		return iResult;

	if (m_vProxySpace[i]->GetUseStatus() == M4_TRUE)
		return M4_SUCCESS;

	m_vProxySpace[i]->SetUseStatus(M4_TRUE);

	ClChannelManager *poClChannelManager = m_vProxySpace[i]->GetChannelManager();
	iResult = poClChannelManager->Reset();
	return iResult;
  //## end ClServerProxyManagerEmul::ReleaseSpace%949597700.body
}

m4return_t ClServerProxyManagerEmul::ReleaseSpace (const m4uint32_t ai_iSessionId)
{
  //## begin ClServerProxyManagerEmul::ReleaseSpace%949597701.body preserve=yes
	for (m4uint8_t i = 0; i < MAX_NUM_PROXY_SPACES; i++) {
		if (m_vProxySpace[i]->GetSessionId() == ai_iSessionId)
			return ReleaseSpace(m_vProxySpace[i], M4_FALSE);
	}

	return M4_ERROR;
  //## end ClServerProxyManagerEmul::ReleaseSpace%949597701.body
}

ClMutex * ClServerProxyManagerEmul::GetpMutAccess ()
{
  //## begin ClServerProxyManagerEmul::GetpMutAccess%949597702.body preserve=yes
	return NULL;
  //## end ClServerProxyManagerEmul::GetpMutAccess%949597702.body
}

m4return_t ClServerProxyManagerEmul::SendEvent ()
{
  //## begin ClServerProxyManagerEmul::SendEvent%949597703.body preserve=yes
	return M4_SUCCESS;
  //## end ClServerProxyManagerEmul::SendEvent%949597703.body
}

void ClServerProxyManagerEmul::SetMaxNSpaces (const m4int32_t ai_iMaxNSpaces)
{
  //## begin ClServerProxyManagerEmul::SetMaxNSpaces%949597704.body preserve=yes
  //## end ClServerProxyManagerEmul::SetMaxNSpaces%949597704.body
}

m4return_t ClServerProxyManagerEmul::ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID)
{
  //## begin ClServerProxyManagerEmul::ReleaseActiveSpace%949597705.body preserve=yes
	m4uint8_t i;

	for ( i = 0; i < MAX_NUM_PROXY_SPACES; i++) {
		if (m_vProxySpace[i] && strcmp(m_vProxySpace[i]->GetUniqueID(), ai_pcClProxySpaceID) == 0)
			break;
	}

	m_vProxySpace[i]->SetUseStatus(M4_FALSE);
	return ReleaseSpace(m_vProxySpace[i], M4_FALSE);
  //## end ClServerProxyManagerEmul::ReleaseActiveSpace%949597705.body
}

// Additional Declarations
  //## begin ClServerProxyManagerEmul%3899B3CC0296.declarations preserve=yes
  //## end ClServerProxyManagerEmul%3899B3CC0296.declarations

//## begin module%3507CDFF0340.epilog preserve=yes
//## end module%3507CDFF0340.epilog
