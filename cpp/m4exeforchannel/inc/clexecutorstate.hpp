//	==============================================================================
//
//	 (c) Copyright  1991-1999 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ClExecutorState.hpp
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

#ifndef _CLEXECUTORSTATE_HPP_
#define _CLEXECUTORSTATE_HPP_

#include "m4exeforchannel_dll.hpp"
#include "m4stl.hpp"
#include "datastorages.hpp"	//M4PDUS
#include "clcache.hpp"		//M4Cache
#include "cachebase.hpp"	//M4Cache
#include "m4cachetypes.hpp"		//M4ObjCacheDirectory
#include "clcofac.hpp"
#include "diskpers.hpp"

class ClM4XMLContext;
class ClCMInitParameters;
class ClChannelManager;
class ClM4ObjService;


//#define M4_CO_FACTORY_SESSION_DATA_CONTAINER	11		//clcache.hpp	(M4Cache)
//#define M4_CACHE_SESSION_STATE_CACHE_TYPE		0x10	//cachedir.hpp	(M4ObjCacheDirectory)


// Class to represent the Virtual Machine Executor State to persist.
// Is a Persistiable Object
class M4_DECL_M4EXEFORCHANNEL ClExecutorState : public ClPersistiableObject
{
public:

	ClExecutorState(void) : ClPersistiableObject ( 0 ) {
	//ClExecutorState(void) {
		Reset();
	}

	ClExecutorState(ClCachePolicy *ai_poPolicy, m4uint32_t  ai_iSizeObject = 0, ClCacheKey *  ai_poClCacheKey = NULL)
		: ClPersistiableObject ( 0 ) {
		Reset();
	}
	ClExecutorState(m4pcchar_t ai_pccSessionId)  
		: ClPersistiableObject ( 0 ) { // Constructor
		Reset();

		SetSessionId(ai_pccSessionId);
	}
	
	virtual ~ClExecutorState(); // Destructor

	m4return_t Init(ClCMInitParameters &aio_oCMInitParameters);
	m4return_t Update(ClCMInitParameters &aio_oCMInitParameters);
	m4return_t End();

	m4bool_t		SetSessionId(m4pcchar_t ai_pccSessionId);
	m4pchar_t		GetSessionId() const	{ return m_pcSessionId; }

	ClM4ObjService* GetM4ObjService() const { return m_poM4ObjService; }
	ClChannelManager* GetChannelManager() const { return m_poChannelManager; }
	ClM4XMLContext* GetXMLContext() const { return m_pXMLContext; }

	m4bool_t		HasStateDS() const	{ return (m_poDataStorage != NULL) ? M4_TRUE : M4_FALSE; }
	m4bool_t		HasState() const	{ return (m_poChannelManager != NULL) ? M4_TRUE : M4_FALSE; }

	m4return_t		InitDataStorage();
	void			SetDataStorage(M4DataStorage* ai_poDataStorage);
	M4DataStorage*	GetDataStorage() const	{ return m_poDataStorage; }

	m4bool_t		IsInUse() const					{ return m_bUsing; }
	void			SetUsing(m4bool_t ai_bUsing);

	m4bool_t		IsSerialized() const			{ return m_bSerialized; }
	void			SetSerialized(m4bool_t ai_bSerialized)	{ m_bSerialized = ai_bSerialized; }

	m4return_t		StartUp();
	m4return_t		ShutDown();

	void			SetReferencedFileList(list<string> *ai_pList) {	m_poListFileNames = ai_pList; }
	m4return_t		SerializeChannelManager(m4bool_t ai_bIgnoreRefs = M4_FALSE);
	m4return_t		DeSerializeChannelManager();

	virtual m4return_t Serialize	(ClGenericIODriver &  IOD);
	virtual m4return_t DeSerialize	(ClGenericIODriver &  IOD);

	/*

	virtual m4uint8_t  GetType (void  );

	virtual m4return_t LoadValueFromDisk ();
	virtual m4return_t SaveValueToDisk ();
	virtual m4return_t DeleteValueFromDisk ();
	virtual m4return_t DeleteValueFromRam ();
	*/

	// Interfaz para Cache
	m4uint32_t	GetSize( void ) const;
    m4date_t    GetStartDate( void ) const;
	m4date_t    GetEndDate( void ) const;
	m4date_t    GetCorStartDate( void ) const;
	m4date_t    GetCorEndDate( void ) const;

	void SetCO( ClCacheableObject * ai_pCO ) {
		m_pCO = ai_pCO;
	}
	ClCacheableObject*	GetCO( void ) const {
        return m_pCO;
    }

	m4date_t    GetVersion( void ) const;

	// Clone this executor state using serialize/deserialize
	m4return_t	Clone(m4pchar_t ai_pccSessionId, ClCMInitParameters &aio_oInitParameters, ClExecutorState *&ao_poExecutorState);

private:
	m4uint32_t			m_uiSizeId;			// Size Id
	m4pchar_t			m_pcSessionId;		// Id

	M4DataStorage	*	m_poDataStorage;	// DataStorage with serialized state

	ClM4XMLContext *	m_pXMLContext;		// XML Context

	ClChannelManager *	m_poChannelManager;	// ChannelManager with all channels (not serialized)
	ClM4ObjService *	m_poM4ObjService;	// M4ObjService

	m4bool_t			m_bUsing;			// Flag to indicate if the state is currently in use.
	m4bool_t			m_bSerialized;		// Flag to indicate if the state is currently Serialized or no.

	// Interfaz para Cache
	ClCacheableObject * m_pCO;			// Cacheable Object

	// Info
	ClCMInitParameters * m_poInitParameters; // configuration object

	list<string>	*m_poListFileNames;

protected:
	m4return_t		SetChannelManager(ClChannelManager* ai_poChannelManager);
	m4return_t		ResetChannelManager();
	m4return_t		SetXMLContext(ClM4XMLContext* ai_pXMLContext);

	// reset internal members
	void Reset()
	{
		m_uiSizeId = 0;
		m_pcSessionId = NULL;
		m_poDataStorage = NULL;
		m_pXMLContext = NULL;
		m_poListFileNames = NULL;

		m_pCO = NULL;
		m_bUsing = M4_FALSE;
		m_bSerialized = M4_FALSE;

		// Auxiliar data
		m_poChannelManager = NULL;
		m_poM4ObjService = NULL;
		m_poInitParameters = NULL;
	}

public:
	static void SetCaches( ClCache *ai_poCMCRCache, ClCache *ai_poCSCRCache, ClCache *ai_poPresentationCache, ClCache *ai_poMapCache, ClCache *ai_poDataCache );

private:
	// Cache subsystems
	static ClCache	 *m_poCMCRCache;
	static ClCache	 *m_poCSCRCache;
	static ClCache	 *m_poPresentationCache;
	static ClCache	 *m_poMapCache;
	static ClCache	 *m_poDataCache;
};





// Class to represent the Container for "ExecutorState's"

// Instanciamos template (ClCacheableObject), que contiene un puntero al objeto Persistible
typedef ClDatedContainer <ClExecutorState> ClExecutorStateContainerBase_t;
// Derivamos de la instanciación, y redefinimos lo necesario
// Definimos el nuevo objeto Cacheable-Contenedor

class M4_DECL_M4EXEFORCHANNEL ClExecutorStateDataContainer : public ClExecutorStateContainerBase_t
{
private:
	m4date_t m_dT3Version;	// Version del Container

public:
	ClExecutorStateDataContainer(ClCachePolicy *ai_poPolicy = 0, m4uint32_t  ai_iSizeObject = 0, ClCacheDatedKey *ai_poClCacheKey = 0) 
		: ClExecutorStateContainerBase_t(ai_poPolicy, ai_iSizeObject, ai_poClCacheKey)
	{
		m_dT3Version = 0;
	};
	virtual m4return_t Serialize(ClGenericIODriver& IOD);

	virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

	virtual m4return_t SaveValueToDisk ( void );

	virtual m4return_t DeleteValueFromDisk ( void );

	//virtual m4return_t SetValue ( ClChannel_Data * ai_pObject );

	virtual m4uint8_t GetType(void) {	// Factory type
		return M4_CO_FACTORY_SESSION_DATA_CONTAINER;	//clcache.hpp
	}

	// Creates Object Id
	virtual ClObjectId * CreateObjectIdFromKey(void);	// Obligatorio

	virtual m4bool_t MatchCondition (void * ai_pCondition);

private:
	list<string>	m_oListFileNames;
};

// class to find out a matching condition
class M4_DECL_M4EXEFORCHANNEL ClExecutorStateDataMatcherById
{
private:
	string m_sId;
	string m_sPrefix;

public:
	// constructor
	ClExecutorStateDataMatcherById(string &ai_sPrefix, string &ai_sId);

	// comparator
	m4bool_t Match(ClExecutorStateDataContainer *ai_poRef);
};

// Class to represent the Cache for "ExecutorState's" using Container + Object(ClExecutorState)

// Instanciamos template<Contenido-Persistible, Contenedor-Cacheable>
typedef ClBaseCache<ClExecutorState, ClExecutorStateDataContainer> ClExecutorStateCacheBase_t;
// Derivamos y redefinimos lo necesario
// Definimos la Cache especializada

class M4_DECL_M4EXEFORCHANNEL ClExecutorStateCache : public ClExecutorStateCacheBase_t 
{
public:
	// Constructor
	ClExecutorStateCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio);

	// Destructor
	~ClExecutorStateCache () { };

	virtual m4uint16_t GetType(void)	// Returns the Cache Type
	{
		return M4_CACHE_SESSION_STATE_CACHE_TYPE;	//m4cachetypes.hpp (m4cache)
	}

	// Get a reference to an object in Cache
	m4return_t GetObject(m4pchar_t ai_pcIdSession, ClExecutorState* &ao_rpExecutorState, m4bool_t &ao_bFound, ClCacheInterface::eGetMode ai_eGetMode = ClCacheInterface::GET_SHARED, m4int32_t ai_iTimeOut = -1);

	// Return a reference (that is not used yet) to an object in Cache
	m4return_t PutObject (m4pchar_t ai_pcIdSession, ClExecutorState* ao_poObject, m4date_t ai_dMaxPeriod = 0);

	m4return_t ReplaceObject (ClExecutorState* ao_poOldObject, m4pchar_t ai_pcIdSession, ClExecutorState* ao_poNewObject, m4date_t ai_dMaxPeriod = 0);

	// removes all other objects in the current session and excludes the specified one
	m4return_t RemoveAllOtherObjectsInSession(string &ai_sSubSessionId);

	// Remove an object by Id
	virtual m4return_t RemoveObjectById (m4pchar_t ai_pcIdPresentation, m4puint32_t ai_piRemovedCount = NULL);	// Obligatorios

	// Remove an object by Id and Version (not used here, only to complain interface)
	virtual m4return_t RemoveObjectByIdAndVersion (m4pchar_t ai_pcIdPresentation, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType);	// Obligatorios
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class ClExecutorStateCacheableObjectFactory : public ClBaseCacheableObjectFactory, public ClDiskPersistor
{
public:
	ClExecutorStateCacheableObjectFactory(void)
	{
	};

	~ClExecutorStateCacheableObjectFactory(void)
	{
	};

private:
	virtual m4return_t CreateObject(m4uint8_t ai_iType, ClCacheableObject * &aio_pObject)
	{
		aio_pObject = new ClExecutorStateDataContainer();
		((ClExecutorStateDataContainer*)aio_pObject)->SetPersistor(this);
		return M4_SUCCESS;
	}
};

#endif	//_CLEXECUTORSTATE_HPP_
