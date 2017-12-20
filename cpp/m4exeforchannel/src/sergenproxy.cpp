//## begin module%3830035B032F.cm preserve=no
//## end module%3830035B032F.cm

//## begin module%3830035B032F.cp preserve=no
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
//## end module%3830035B032F.cp

//## Module: SerGenProxy%3830035B032F; Package body
//## Subsystem: M4ExeForChannel::Proxy::src%392274340168
//## Source file: D:\Work\v500\m4exeforchannel\src\sergenproxy.cpp

//## begin module%3830035B032F.additionalIncludes preserve=no
//## end module%3830035B032F.additionalIncludes

//## begin module%3830035B032F.includes preserve=yes
#include <ssnames.hpp>
#include <m4srvefcres.hpp>
#include <m4srvres.hpp>
//	Rose 98 Body Include Dependences
#include "blocksynchronization.hpp"
#include <proxspacsss.hpp>
#include <clm4objservice.hpp>
#include <dm.hpp>
#include <conn_provider.hpp>
#include <executor.hpp>
//## end module%3830035B032F.includes

// SerGenProxy
#include <sergenproxy.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// Generic_Proxy
#include <genproxy.hpp>

class ClCache;
class ClMutex;

//## begin module%3830035B032F.declarations preserve=no
//## end module%3830035B032F.declarations

//## begin module%3830035B032F.additionalDeclarations preserve=yes
#define OBL_PROXY_SPACE_SESSION_ID		"LAUNCHER_SESSION_ID"
#define OBL_PROXY_SPACE_NO_SESSION_ID	"NO SESSION ID"
#define OBL_PROXY_SPACE_USER_NAME		"LAUNCHER_USER_ID"
//## end module%3830035B032F.additionalDeclarations


// Class ClServerProxyManager 







//## begin ClServerProxyManager::m_oClProxyNotify%35C166BC00A3.role preserve=no  public: static ClProxyNotify {1 -> 1VHAN}
ClProxyNotify ClServerProxyManager::m_oClProxyNotify;
//## end ClServerProxyManager::m_oClProxyNotify%35C166BC00A3.role


ClServerProxyManager::ClServerProxyManager (ClCache *ai_poClCache)
  //## begin ClServerProxyManager::ClServerProxyManager%889780470.hasinit preserve=no
      : m_bStatus(M4_FALSE), m_iMaxNSpaces(-1), m_iNSpaces(0), m_poClCache(NULL), m_poMutAccess(NULL)
  //## end ClServerProxyManager::ClServerProxyManager%889780470.hasinit
  //## begin ClServerProxyManager::ClServerProxyManager%889780470.initialization preserve=yes
  //## end ClServerProxyManager::ClServerProxyManager%889780470.initialization
{
  //## begin ClServerProxyManager::ClServerProxyManager%889780470.body preserve=yes
	
	// Initializes the mutex.
	m_poMutAccess = new ClMutex(M4_TRUE);
	
	M4_ASSERT(m_poMutAccess != NULL);
	if (m_poMutAccess == NULL)
		return;
	
	// Creo mi propia cache de espacios proxy.
	ClCacheTree *poClCacheTree = new ClCacheTree;
	m_poClCache = new ClCacheTest(poClCacheTree);
	
	M4_ASSERT(m_poClCache != NULL);
	if (m_poClCache == NULL)
		return;
	
	m_bStatus =	M4_TRUE;
  //## end ClServerProxyManager::ClServerProxyManager%889780470.body
}


ClServerProxyManager::~ClServerProxyManager ()
{
  //## begin ClServerProxyManager::~ClServerProxyManager%889780471.body preserve=yes
	M4_ASSERT(m_poClCache != NULL);
	if (m_poClCache != NULL)
	{
		delete m_poClCache;
		m_poClCache = NULL;
	}
	
	// checks the mutex
	if (m_poMutAccess != NULL)
	{
		delete m_poMutAccess;
		m_poMutAccess = NULL;
	}
  //## end ClServerProxyManager::~ClServerProxyManager%889780471.body
}



//## Other Operations (implementation)
ClProxySpace* const ClServerProxyManager::GetSpace (const m4char_t* const ai_pcClProxySpaceID, const m4bool_t ai_bCreateNew)
{
  //## begin ClServerProxyManager::GetSpace%889702422.body preserve=yes

	// locks
	ClMutBlock oClMutBlock (m_poMutAccess);

	// local variables
	m4return_t iResult = M4_ERROR;

	// tries to find the data from cache
	ClCacheKey oClCacheKey((m4char_t *)ai_pcClProxySpaceID);
	ClCacheableObject* poClCacheableObject = NULL;
	m4return_t iScanResult = m_poClCache->GetObject(&oClCacheKey, poClCacheableObject, ClCacheInterface::GET_EXNBLOCK);
	ClProxySpaceContainer* poClProxySpaceContainer = (ClProxySpaceContainer*) poClCacheableObject;

	M4_ASSERT(iScanResult != M4_SUCCESS && poClProxySpaceContainer == NULL ||
			  iScanResult == M4_SUCCESS && poClProxySpaceContainer != NULL);

	// sets the running flag
	ClProxySpace *poClProxySpace = NULL;
	if (poClProxySpaceContainer != NULL)
		poClProxySpace = poClProxySpaceContainer->GetProxySpace();

	// returns if we are using a unique key
	if (poClProxySpace != NULL && strcmp(poClProxySpace->GetBaseID(),ai_pcClProxySpaceID))
	{
		// for consulting keep state
		poClProxySpace->SetIsRunning(M4_TRUE);
		poClProxySpace->SetUseStatus(M4_TRUE);
		return poClProxySpace;
	}

	// we now create a new space o recycle an old one
	if (!ai_bCreateNew)
	{
		// bugid: 0087048
        DUMP_CHLOG_ERRORF (M4_ERR_PROXY_GET_PXSPACE, ai_pcClProxySpaceID);
		return NULL;
	}

	// checks the max number of spaces
	if (m_iMaxNSpaces != -1 && m_iMaxNSpaces == m_iNSpaces)
	{
		DUMP_CHLOG_ERRORF ( M4_ERR_PROXY_GET_MAXNSPACE , m_iNSpaces);
		return NULL;
	}

	// generates a new proxy space
	// in the futures this will be parametrized
	if (poClProxySpace == NULL)
	{
		// Instances the new space
		// Como ahora el proxy space del servidor es tambien un susbsistema,
		// le digo al subsistema de proxy spaces que me lo cree
		poClProxySpace = m_pProxySpacesSS->CreateServerProxySpaceSS();

		/* Para crear un espacio proxy que no sea un subsistema
		poClProxySpace = new ClProxySpace(this);
		*/

		M4_ASSERT(poClProxySpace != NULL);
		if (poClProxySpace == NULL)
			return NULL;

		// sets the generic key
		if (poClProxySpace->SetBaseID(ai_pcClProxySpaceID) != M4_SUCCESS)
			return NULL;
	
		// generates the new container
		poClProxySpaceContainer = new ClProxySpaceContainer(poClProxySpace);
		M4_ASSERT(poClProxySpaceContainer != NULL);
		if (poClProxySpaceContainer == NULL)
			return NULL;
		
		// sets the container
		poClProxySpace->SetContainer(poClProxySpaceContainer);
	}

	// generates the unique key
	// it should be more complex in the future
	static m4uint32_t lIDCounter = 0;
	m4char_t pcUniqueKey[PROXY_MAX_ID];
	sprintf(pcUniqueKey,"%s-%ld",ai_pcClProxySpaceID,lIDCounter++);
	if (poClProxySpace->SetUniqueID(pcUniqueKey) != M4_SUCCESS)
		return NULL;
		
	// prepares the unique key
	ClCacheKey *poClCacheKey = new ClCacheKey(pcUniqueKey);
	poClCacheableObject = (ClCacheableObject*) poClProxySpaceContainer;

	if (iScanResult == M4_SUCCESS)
	{
		// adjusts the new key
		ClCacheKey *poOldClCacheKey = NULL;
		poClCacheableObject->GetKey(poOldClCacheKey);
		if (m_poClCache->UpdateObjectKey(poClCacheableObject,poClCacheKey) != M4_SUCCESS)
		{
		    // bugid: 0087048
			DUMP_CHLOG_ERRORF (M4_ERR_PROXY_GET_PXSPACE, pcUniqueKey);
			return NULL;
		}

		// deletes the old key
		delete poOldClCacheKey;
	}
	else
	{
		// puts the data back
		poClCacheableObject->SetKey(poClCacheKey);
		iResult = m_poClCache->UpdateObject(poClCacheableObject);
		M4_ASSERT(iResult == M4_SUCCESS);
		if (iResult != M4_SUCCESS)
			return NULL;
	}
	
	// adjusts the running status and returns
	m_iNSpaces++;

	// for consulting keep state
	poClProxySpace->SetIsRunning(M4_TRUE);
	poClProxySpace->SetUseStatus(M4_TRUE);

	return poClProxySpace;

  //## end ClServerProxyManager::GetSpace%889702422.body
}

ClProxySpace * ClServerProxyManager::GetSpace (const m4uint32_t ai_iSessionId)
{
  //## begin ClServerProxyManager::GetSpace%941105291.body preserve=yes
	// Precondicion: Tiene que existir la cache
	M4_ASSERT (m_poClCache != NULL);
	if (m_poClCache == NULL)
		return NULL;

	ClProxySpace* poClProxySpace = NULL;

	// use mutex to invalidate multiple accesses
	{
		// locks the proxy cache access
		ClMutBlock oClMutBlock(GetpMutAccess());
		
		// gets the objects from the cache
		ClProxySpace*		pAuxProxySpace		= NULL;
		ClCacheableObject*	poCacheableObject	= NULL;
		void*				pOldRef				= NULL;
		m4return_t			iResult				= M4_SUCCESS;
		
		// scans the data
		for (;;)
		{
			iResult = m_poClCache->GetNext(poCacheableObject, pOldRef);
			if (iResult != M4_SUCCESS)
				break;
			
			// checks the end condition
			if (poCacheableObject == NULL)
				break;

			// have we found it?
			if (poClProxySpace != NULL)
				continue;
			
			// checks the execution state
			pAuxProxySpace = ((ClProxySpaceContainer*) poCacheableObject)->GetProxySpace();
			if (!pAuxProxySpace->IsRequestToAbort())
			{
				// checks the session id
				if (pAuxProxySpace->GetSessionId() == ai_iSessionId)
				{
					poClProxySpace = pAuxProxySpace;
				}
			}

			// goes to the next element
		}
	}

	return poClProxySpace;
  //## end ClServerProxyManager::GetSpace%941105291.body
}

m4return_t ClServerProxyManager::ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock)
{
  //## begin ClServerProxyManager::ReleaseSpace%889702423.body preserve=yes
		
	// puts back in cache
	M4_ASSERT (ai_poClProxySpace != NULL);

	// locks
	if (ai_bDoLock)
		m_poMutAccess->Lock();

	// if we don't have a unique ID the space is already released
	if (ai_poClProxySpace->GetUniqueID() == NULL)
	{
		if (ai_bDoLock)
			m_poMutAccess->Unlock();
		return M4_SUCCESS;
	}

	m4return_t iResult = M4_SUCCESS, iResult2 = M4_SUCCESS;

	// releases the session channel
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% must channge to copy
	iResult = ai_poClProxySpace->SetSessionChannel(NULL,0,NULL);
	M4_ASSERT(iResult == M4_SUCCESS);
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% must channge to copy

	// fix bug 0085633
	// release the VM interruption mode
	ClChannelManager *poOS = ai_poClProxySpace->GetChannelManager();
	if (poOS != NULL && poOS->GetpExecutor() != NULL) {
		poOS->GetpExecutor()->RequestToAbortWithClStatus(M4_FALSE);
	}

	// resets the key
	ClCacheableObject *poClCacheableObject = (ClCacheableObject*) ai_poClProxySpace->GetProxySpaceContainer();
	if (iResult == M4_SUCCESS && ai_poClProxySpace->GetUseStatus() == M4_FALSE)
	{

#ifdef _DEBUG
		cerr << "Effective release space (" << ai_poClProxySpace->GetUniqueID() << ") handler " << (void*) ai_poClProxySpace << endl;
		cerr.flush();
#endif
		// releases the session id
		ai_poClProxySpace->SetSessionId(0,NULL);

		// resets the status
		ai_poClProxySpace->SetUseStatus(M4_TRUE);

		// frees the logical connection
		iResult2 = ai_poClProxySpace->FreeLogicalConnection();

		/* Bug 0063193
		Hay que liberar los accesos de nivel 2 del canal sesión
		*/
		/* Bug 0099344
		Hay que borrar los accesos cuando efectivamente se muere el espacio proxy
		*/
		iResult = ai_poClProxySpace->ReleaseSessionAcceses() ;

		// releases all channels
		iResult = poOS->Reset();
		M4_ASSERT(iResult == M4_SUCCESS);

		// updates the cache key
		if (iResult == M4_SUCCESS)
		{
			ClCacheKey *poClCacheKey = new ClCacheKey(ai_poClProxySpace->GetBaseID());
			ClCacheKey *poClOldCacheKey;
			poClCacheableObject->GetKey(poClOldCacheKey);
			iResult = m_poClCache->UpdateObjectKey(poClCacheableObject,poClCacheKey);
			delete poClOldCacheKey;
		}

		// resets the unique id
		if (iResult == M4_SUCCESS)
			iResult = ai_poClProxySpace->ResetUniqueID();

		if (iResult == M4_SUCCESS)
		{
			// adjusts the counter
			m_iNSpaces--;
		}
	}

	// puts the object back
	if (iResult == M4_SUCCESS && poClCacheableObject->IsReferenced()) {
		iResult = m_poClCache->PutObject(poClCacheableObject);
	}

	// releases the reference to the outside world
	if (iResult == M4_SUCCESS) {
		ai_poClProxySpace->SetIsRunning(M4_FALSE);
	}
	
	// resets the mutex
	if (ai_bDoLock)
	{
		m_poMutAccess->Unlock();

		// sends an event to the server
		if (iResult == M4_SUCCESS)
			iResult = SendEvent();
	}

	M4_ASSERT(iResult == M4_SUCCESS);

	if (iResult2 != M4_SUCCESS) {
		return iResult2;
	}
		
	return iResult;
  //## end ClServerProxyManager::ReleaseSpace%889702423.body
}

m4return_t ClServerProxyManager::ReleaseSpace (const m4uint32_t ai_iSessionId)
{
  //## begin ClServerProxyManager::ReleaseSpace%942076255.body preserve=yes
	return ReleaseSpace(ai_iSessionId,0);
  //## end ClServerProxyManager::ReleaseSpace%942076255.body
}

ClMutex * ClServerProxyManager::GetpMutAccess ()
{
  //## begin ClServerProxyManager::GetpMutAccess%901615380.body preserve=yes
	M4_ASSERT(m_poMutAccess != NULL);
	return m_poMutAccess;
  //## end ClServerProxyManager::GetpMutAccess%901615380.body
}

m4return_t ClServerProxyManager::SendEvent ()
{
  //## begin ClServerProxyManager::SendEvent%901872838.body preserve=yes
	//	return M4_SUCCESS;
	return m_oClProxyNotify.SendEvent(ClProxyNotify::PX_EVENT_UPDATE,NULL);
  //## end ClServerProxyManager::SendEvent%901872838.body
}

void ClServerProxyManager::SetMaxNSpaces (const m4int32_t ai_iMaxNSpaces)
{
  //## begin ClServerProxyManager::SetMaxNSpaces%916914831.body preserve=yes
	m_iMaxNSpaces = ai_iMaxNSpaces;
  //## end ClServerProxyManager::SetMaxNSpaces%916914831.body
}

m4return_t ClServerProxyManager::ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID)
{
  //## begin ClServerProxyManager::ReleaseActiveSpace%926076992.body preserve=yes
	M4_ASSERT (m_poClCache != NULL);
	if (m_poClCache == NULL)
		return M4_ERROR;

	m4return_t iResult = M4_SUCCESS, iStatus = M4_SUCCESS;
		
	// locks
	ClMutBlock oClMutBlock (m_poMutAccess);

	// gets the objects from the cache
	ClCacheableObject *poCacheableObject = NULL;
	void *pOldRef = NULL;
	
	// scans the data
	for (;;)
	{
		iResult = m_poClCache->GetNext(poCacheableObject,pOldRef);
		if (iResult != M4_SUCCESS)
			break;
		
		// checks the end condition
		if (poCacheableObject == NULL)
			break;
		
		// proccesses error state
		if (iStatus != M4_SUCCESS)
			continue;
		
		// checks the session id
		ClProxySpace *poClProxySpace = ((ClProxySpaceContainer*) poCacheableObject)->GetProxySpace();
		m4pchar_t pcUniqueID = poClProxySpace->GetUniqueID();
		if (pcUniqueID == NULL || strcmp(pcUniqueID,ai_pcClProxySpaceID))
			continue;
		
		// stops the proxy execution
		poClProxySpace->RequestToAbort();

		// Is it busy ?
		if (poClProxySpace->GetIsRunning() == M4_FALSE) {
			ReleaseSpace(poClProxySpace,M4_FALSE);
		}
		
		// goes to the next element
	}
	
	return (iResult != M4_SUCCESS || iStatus != M4_SUCCESS) ? M4_ERROR : M4_SUCCESS;

  //## end ClServerProxyManager::ReleaseActiveSpace%926076992.body
}

void ClServerProxyManager::SetProxySpacesSS (ClProxySpacesSS& ai_oProxySpacesSS)
{
  //## begin ClServerProxyManager::SetProxySpacesSS%943008107.body preserve=yes
	m_pProxySpacesSS = &ai_oProxySpacesSS;
  //## end ClServerProxyManager::SetProxySpacesSS%943008107.body
}

m4return_t ClServerProxyManager::Detach (ClProxySpace* ai_poProxySpace)
{
  //## begin ClServerProxyManager::Detach%951403969.body preserve=yes
	if (ai_poProxySpace == NULL)
		return M4_SUCCESS;

	ClCacheableObject* poClCacheableObject = ai_poProxySpace->GetProxySpaceContainer();
	if (poClCacheableObject == NULL)
		return M4_ERROR;

	m4return_t iResult = m_poClCache->RemoveObject(poClCacheableObject);

	return iResult;
  //## end ClServerProxyManager::Detach%951403969.body
}

m4return_t ClServerProxyManager::ReleaseSpace (const m4uint32_t ai_iSessionId, const m4uint32_t ai_iRequestId)
{
  //## begin ClServerProxyManager::ReleaseSpace%994267998.body preserve=yes
	M4_ASSERT (m_poClCache != NULL);
	if (m_poClCache == NULL)
		return M4_ERROR;

	m4return_t		iResult				= M4_SUCCESS;
	m4return_t		iStatus				= M4_SUCCESS;
	m4bool_t		bProxySpaceFound	= M4_FALSE;
			
	// use mutex to invalidate multiple accesses
	{
		// locks the proxy cache access
		ClMutBlock oClMutBlock(GetpMutAccess());
		
		// gets the objects from the cache
		ClProxySpace*		poClProxySpace		= NULL;
		ClCacheableObject*	poCacheableObject	= NULL;
		void*				pOldRef				= NULL;
		
		// scans the data
		for (;;)
		{
			iResult = m_poClCache->GetNext(poCacheableObject, pOldRef);
			if (iResult != M4_SUCCESS)
				break;
			
			// checks the end condition
			if (poCacheableObject == NULL)
				break;
			
			// proccesses error state
			if (iStatus != M4_SUCCESS)
				continue;

			// checks the execution state
			poClProxySpace = ((ClProxySpaceContainer*) poCacheableObject)->GetProxySpace();
			if (!poClProxySpace->IsRequestToAbort())
			{
				// checks the session id
				m4bool_t bBreak = M4_FALSE;
				if (ai_iSessionId != 0)
				{
					m4uint32_t iCurrentId = poClProxySpace->GetSessionId();
					if (iCurrentId != 0 && iCurrentId == ai_iSessionId)
					{
						bBreak = M4_TRUE;
					}
				}

				// checks the request id
				if (ai_iRequestId != 0 && bBreak == M4_TRUE)
				{
					m4uint32_t iRequestId = poClProxySpace->GetRequestId();
					if (iRequestId == 0 || iRequestId != ai_iRequestId)
					{
						bBreak = M4_FALSE;
					}
				}

				if (bBreak == M4_FALSE)
					continue;
			}
			
			// Stops the proxy execution
			poClProxySpace->RequestToAbort();

			// Is it busy ?
			if (poClProxySpace->GetIsRunning() == M4_FALSE) {
				ReleaseSpace(poClProxySpace,M4_FALSE);
			}
			
			// goes to the next element
		}
	}

	// returns
	M4_ASSERT(iResult == M4_SUCCESS && iStatus == M4_SUCCESS);
	return (iResult != M4_SUCCESS || iStatus != M4_SUCCESS || !bProxySpaceFound) ? M4_ERROR : M4_SUCCESS;
  //## end ClServerProxyManager::ReleaseSpace%994267998.body
}

// Class ClServerProxySpaceSS 

ClServerProxySpaceSS::ClServerProxySpaceSS (ClServerProxyManager* ai_pServerProxyManager, ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClServerProxySpaceSS::ClServerProxySpaceSS%943008106.hasinit preserve=no
  //## end ClServerProxySpaceSS::ClServerProxySpaceSS%943008106.hasinit
  //## begin ClServerProxySpaceSS::ClServerProxySpaceSS%943008106.initialization preserve=yes
  : ClProxySpace(ai_pServerProxyManager, 0)
  , ClSSImplementation(ai_poConfiguratoin, ai_poTables, ai_poNavigator , M4_TRUE )
  //## end ClServerProxySpaceSS::ClServerProxySpaceSS%943008106.initialization
{
  //## begin ClServerProxySpaceSS::ClServerProxySpaceSS%943008106.body preserve=yes
  //## end ClServerProxySpaceSS::ClServerProxySpaceSS%943008106.body
}



//## Other Operations (implementation)
m4return_t ClServerProxySpaceSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClServerProxySpaceSS::GetCommandsId%943376569.body preserve=yes
	return M4_SUCCESS;
  //## end ClServerProxySpaceSS::GetCommandsId%943376569.body
}

m4return_t ClServerProxySpaceSS::SetSessionId (m4uint32_t ai_iSessionId, m4pcchar_t ai_pcUserName)
{
  //## begin ClServerProxySpaceSS::SetSessionId%943439079.body preserve=yes
	m4return_t iRet = ClProxySpace::SetSessionId(ai_iSessionId, ai_pcUserName);

	// Actualizamos los atributos para mostrar en el monitor de administracion
	if (iRet == M4_SUCCESS)
	{
		m4char_t pcBuffer[128] = "";

		if (ai_iSessionId)
			sprintf(pcBuffer, "%d", ai_iSessionId);				// translates the values
		else
			strcpy(pcBuffer, OBL_PROXY_SPACE_NO_SESSION_ID);	// no session id !!!

		iRet = SetConfigStringProperty( OBL_PROXY_SPACE_SESSION_ID, pcBuffer);
		if (iRet != M4_SUCCESS)
			SETCODEF(M4_SRV_SS_OBL_ERROR, ERRORLOG, "Can't reset property value <%s>", OBL_PROXY_SPACE_SESSION_ID);
	}

	return iRet;
  //## end ClServerProxySpaceSS::SetSessionId%943439079.body
}

void ClServerProxySpaceSS::SetUserName (m4pcchar_t ai_pcUserName)
{
  //## begin ClServerProxySpaceSS::SetUserName%943439080.body preserve=yes
	ClProxySpace::SetUserName(ai_pcUserName);

	// Actualizamos los atributos para mostrar en el monitor de administracion
	m4return_t iRet = SetConfigStringProperty( OBL_PROXY_SPACE_USER_NAME, (m4char_t*)GetUserName());
	if (iRet != M4_SUCCESS)
		SETCODEF(M4_SRV_SS_OBL_ERROR, ERRORLOG, "Can't reset property value <%s>", OBL_PROXY_SPACE_USER_NAME);
  //## end ClServerProxySpaceSS::SetUserName%943439080.body
}

void ClServerProxySpaceSS::SetInvalidState (m4bool_t ai_bState)
{
  //## begin ClServerProxySpaceSS::SetInvalidState%951838050.body preserve=yes
	if (ai_bState == M4_TRUE)
		SetState(ClSSImplementation::SS_INACTIVE,SS_STR_INACTIVE) ;
	else
		SetState(ClSSImplementation::SS_ACTIVE,SS_STR_ACTIVE) ;
  //## end ClServerProxySpaceSS::SetInvalidState%951838050.body
}

m4return_t ClServerProxySpaceSS::FreeLogicalConnection ()
{
  //## begin ClServerProxySpaceSS::FreeLogicalConnection%958548748.body preserve=yes
	return m_pObjService->GetpConnProvider()->SetConnection();
  //## end ClServerProxySpaceSS::FreeLogicalConnection%958548748.body
}

// Additional Declarations
  //## begin ClServerProxySpaceSS%380B45C701ED.declarations preserve=yes
  //## end ClServerProxySpaceSS%380B45C701ED.declarations

// Class ClProxyNotify::ClGenericHost 

// Additional Declarations
  //## begin ClProxyNotify::ClGenericHost%35C0AF0F01FF.declarations preserve=yes
  //## end ClProxyNotify::ClGenericHost%35C0AF0F01FF.declarations

// Class ClProxyNotify 



//## begin ClProxyNotify::<m_pClServerProxyManager>%35C166BC0053.role preserve=no  public: static ClServerProxyManager {1 -> 1RHGA}
//## end ClProxyNotify::<m_pClServerProxyManager>%35C166BC0053.role

ClProxyNotify::ClProxyNotify ()
  //## begin ClProxyNotify::ClProxyNotify%901810341.hasinit preserve=no
      : m_oEventHandler(NULL), m_poClGenericHost(NULL)
  //## end ClProxyNotify::ClProxyNotify%901810341.hasinit
  //## begin ClProxyNotify::ClProxyNotify%901810341.initialization preserve=yes
  //## end ClProxyNotify::ClProxyNotify%901810341.initialization
{
  //## begin ClProxyNotify::ClProxyNotify%901810341.body preserve=yes
  //## end ClProxyNotify::ClProxyNotify%901810341.body
}


ClProxyNotify::~ClProxyNotify ()
{
  //## begin ClProxyNotify::~ClProxyNotify%901810342.body preserve=yes
	m_oEventHandler = NULL;
	m_poClGenericHost = NULL;
  //## end ClProxyNotify::~ClProxyNotify%901810342.body
}



//## Other Operations (implementation)
m4return_t ClProxyNotify::RegisterReceiver (EventHandler ai_pEventHandler, ClGenericHost *ai_poGenericHost)
{
  //## begin ClProxyNotify::RegisterReceiver%901810340.body preserve=yes
 
	M4_ASSERT(ai_pEventHandler != NULL && ai_poGenericHost != NULL);

	if (m_oEventHandler != NULL)
		return M4_ERROR;

	m_oEventHandler = ai_pEventHandler;

	m_poClGenericHost = ai_poGenericHost;

	return M4_SUCCESS;
  //## end ClProxyNotify::RegisterReceiver%901810340.body
}

m4return_t ClProxyNotify::SendEvent (EventType ai_iEventType, EventParam ai_oEventParam)
{
  //## begin ClProxyNotify::SendEvent%901810343.body preserve=yes

	// just ignores if we don't have receivers registered
	if (m_oEventHandler == NULL)
		return M4_SUCCESS;

	return m_oEventHandler(ai_iEventType,ai_oEventParam);
  //## end ClProxyNotify::SendEvent%901810343.body
}

ClProxyNotify::ClGenericHost * ClProxyNotify::GetHost ()
{
  //## begin ClProxyNotify::GetHost%901810345.body preserve=yes
	M4_ASSERT(m_poClGenericHost != NULL);
	return m_poClGenericHost;
  //## end ClProxyNotify::GetHost%901810345.body
}

// Additional Declarations
  //## begin ClProxyNotify%35C084650114.declarations preserve=yes
  //## end ClProxyNotify%35C084650114.declarations

//## begin module%3830035B032F.epilog preserve=yes
//## end module%3830035B032F.epilog
