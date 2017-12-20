#ifndef __DM__HPP__
#define __DM__HPP__

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           dm.hpp
// File:             dm.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             13/06/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "colprop.hpp"
#include "chan_dir.hpp"
#include "laneman.hpp"
#include "usractaudit.hpp" // User actions auditory.


class ClVMBaseEnv;
class ClAccess;
class ClAccessBookMark;
class ClLogonAdaptor;
class ClSession;
class ClPresentationFactory;
class ClDataCache;
class ClCache;
class ClHandle;
class ClCacheDirectory;
class ClCMCRFactory;
class ClClientMetaData;
class ClDMContext;
class ClVMStatisticsWrapper;
class IExecutor;
class ClCsTrace;
class IProxyManager;
class ClProxySpace;
class ILicenseManager;
class IObjDataCache;
class ClObjDataCacheBySessionId;
class M4JavaManager;
class M4DataStorage;


enum m4EmulationLevel {
	M4CL_EMULATE_NONE,
	M4CL_EMULATE_CLIENT_SERVER,
	M4CL_EMULATE_SINGLE_MACHINE,
	M4CL_EMULATE_FAT
};


///////////////////////////////////////////////////////////////////////////////
//
// ClChannelManager - DM - Gestor de Canales
//
///////////////////////////////////////////////////////////////////////////////
class M4_DECL_M4DM ClChannelManager: public ClChangeListener
{
	
private:

	m4bool_t				m_bStarted;	// ChannelManager Inicializado S/N
	
	ClDMContext				*m_poDMContext;		// Contexto C/S de DM
	
	ClVMBaseEnv				*m_poEnvironment ;	// Registry
	ClVMStatisticsWrapper	*m_poSttWrapper;	// Estadisticas
	ClCacheDirectory		*m_poCacheDirectory;// Directorio de Caches
	ClDataCache				*m_poDataCache;		// Cache de Datos
	ClCMCRFactory			*m_poFactory ;		// Factoria de MetaDatos
	ClPresentationFactory	*m_poPresFac;		// Factoria de Presentaciones
	IExecutor				*m_poExecutor;		// Ejecutor VM
	ClLogonAdaptor			*m_poLogonAdaptor;	// Logon Adaptor
	ClSession				*m_poSession;		// Session Object.
	ClCsTrace				*m_poCSTraces;		// Trazas C/S
	
	// Default CS type
	m4VariantType			DefaultCSType;
	
	// proxy
	IProxyManager*			m_poProxyManager;
	
	// Data Check 
	// NULL check - extended data
	ClHandle				*m_poDCNodeHandle;
	ClHandle				*m_poDCItemHandle;
	ClAccessBookMark		*m_poDCBookMark;
	ClChannel				*m_poDCChannel;
	m4bool_t				m_bIsDCEnabled;
	
	// Sólo en cliente
	ClChannel				*m_poMultiValidationChannel;	// Canal de MultiValidacion (solo en Cliente)

	// Función de callback para el stepper
	long					m_lfCallBack;

	//Para la gestion de licencias.
	ILicenseManager			*m_pILicenseManager;
	
private:

	void		_ResetState();		// Reset object state
	m4return_t	_CloneChannel( ClChannel* &ao_poChannel, ClChannel* ai_poSourceChannel, M4DataStorage *ai_poDataStorage ) ;

public:

	ClColProp				Property;		// Coleccion de Propiedades
	ClChannelDirectory		Channel;		// Directorio de Canales
	ClLaneManager			LaneManager;	// Gestor de Carriles
	
protected:

	ClObjDataCacheBySessionId	*m_poObjDataCache ;
	vector<m4uint32_t>			*m_pvDeletedAutos ;

public:

	ClChannelManager();
	~ClChannelManager();
	
	// Inicializacion y Destruccion
	m4return_t	Init(
		ClDMContext				*ai_poDMContext, 
		ClVMBaseEnv				*ai_poEnvironment,
		ClVMStatisticsWrapper	*ai_poSttWrapper,
		ClCacheDirectory		*ai_poCacheDirectory,
		ClCache					*ai_poDataCache,
		ClCMCRFactory			*ai_poFactory,
		ClPresentationFactory	*ai_poPresFac,
		IExecutor				*ai_poExecutor,
		ClLogonAdaptor			*ai_poLogonAdaptor,
		ClCsTrace				*ai_poCSTraces,
		IProxyManager* const	ai_pProxyManager,
		ILicenseManager			* ai_pILicenseManager
		);
	
	// @@@ Joseco.
	// Inicializacion y Destruccion
	m4return_t	Init(
		ClDMContext				*ai_poDMContext, 
		ClVMBaseEnv				*ai_poEnvironment,
		ClVMStatisticsWrapper	*ai_poSttWrapper,
		ClCacheDirectory		*ai_poCacheDirectory,
		ClCache					*ai_poDataCache,
		ClCMCRFactory			*ai_poFactory,
		ClPresentationFactory	*ai_poPresFac,
		IExecutor				*ai_poExecutor,
		ClSession				*ai_poSession,
		ClCsTrace				*ai_poCSTraces,
		IProxyManager* const	ai_pProxyManager,
		ILicenseManager			* ai_pILicenseManager
		);
	
	m4return_t	End( void );
	
	m4bool_t	Initialised() { return m_bStarted; }
	
	// Get de los Atributos
	ClDMContext*			GetDMContext() const			{ return m_poDMContext; }
	ClVMBaseEnv*			GetEnvironment( void ) const	{ return m_poEnvironment; }
	ClVMStatisticsWrapper*	GetSttWrapper() const			{ return m_poSttWrapper; }
	ClCacheDirectory *		GetCacheDirectory(void)const	{ return m_poCacheDirectory; }
	ClCMCRFactory*			GetpFactory( void ) const		{ return m_poFactory; }
	ClPresentationFactory*	GetPresFac(void) const			{ return m_poPresFac; }
	IExecutor *				GetpExecutor() const			{ return m_poExecutor; }
	ClLogonAdaptor*			GetLogonAdaptor(void) const		{ return m_poLogonAdaptor; }
	ClSession*				GetSession(void) const			{ return m_poSession; }
	ClDataCache *			GetCache(void) const			{ return m_poDataCache; }
	ClCsTrace* const		GetCsTrace() const				{ return m_poCSTraces; }
	ILicenseManager*		GetpLicenseManager() 			{ return m_pILicenseManager; }
	M4JavaManager*			GetJavaManager() ;

	void	ReleaseEmptyJavaContexts( void ) ;

	IObjDataCache*	GetObjDataCache( m4pcchar_t ai_pccSession ) ;

	m4return_t	AddDeletedAuto( m4uint32_t ai_iRemoteId ) ;
	m4return_t	SerializeDeletedAutos( ClGenericIODriver& IOD ) ;

	m4return_t	CreateChannel( ClChannel* &ao_poChannel ) ;
	m4return_t	CreateChannel( ClChannel* &ao_poChannel, m4uint32_t ai_iHandle ) ;
	m4return_t	CloneChannel ( ClChannel* &ao_poChannel, ClChannel* ai_poSourceChannel ) ;
	
	m4uint32_t	GetSize(void);
	
	m4return_t	Reset();
	
	ClChannel*	GetChannelByHandle( m4int32_t ai_hChannel );
	ClAccess*	GetAccessByHandle( m4int32_t ai_hAccess );

	// session channel
	ClChannel* GetSessionChannel(void);
	m4return_t SetSessionChannel(ClChannel* ai_poChannel);
	
	void	SetDefaultCSType(m4VariantType ai_CSType){
		DefaultCSType = ai_CSType;
	}

	m4VariantType	GetDefaultCSType(void)
	{
		return DefaultCSType;
	}
	
	m4EmulationLevel EmulationLevel(void);
	
	
	// proxy
	ClProxySpace * GetProxySpace(void);
	
	IProxyManager * GetProxyManager(void) const
	{
		return m_poProxyManager;
	}

	void ReleaseProxy(void);
	
	void DumpChannels(m4pchar_t ai_pcCalledFrom, ostream *ai_pStream=NULL);
	void DumpCaches(m4pchar_t ai_pcCalledFrom, ostream *ai_pStream=NULL);
	
	// Data check public functions
	void		SetDCData(ClChannel *ai_poChannel=NULL, ClAccessBookMark *ai_poBookMark=NULL, ClHandle *ai_poNodeHandle=NULL, ClHandle *ai_poItemHandle=NULL);
	void		GetDCData(ClChannel *&ao_poChannel, ClAccessBookMark *&ao_poBookMark, ClHandle *&ao_poNodeHandle, ClHandle *&ao_poItemHandle);

	void		SetDCEnabled (m4bool_t ai_bFlag=M4_TRUE)
	{
		m_bIsDCEnabled = ai_bFlag;
	}
	m4bool_t	IsDCEnabled()
	{
		return m_bIsDCEnabled;
	}

	m4bool_t	IsDCData();
	
	ClChannel * GetMultiValidationChannel(void);

	long	GetCallBack(void) const {
		return( m_lfCallBack ) ;
	}
	
	void	SetCallBack(long ai_lfCallBack) {
		m_lfCallBack = ai_lfCallBack ;
	}
	
	/**
	 * Bug 0059142.
	 * Comprueba si un acceso es de nivel2. Es decir, si el acceso está en alguna lista de enlaces de nivel2
	 * de alguno de los canales del channelManager.
	 *
	 * @param ai_pAccess	Puntero al acceso que se quiere saber si es de nivel2.
	 * @return	m4bool_t	M4_TRUE --> el acceso es de nivel2, M4_FALSE --> el canal no es de nivel2.
	 */
	m4bool_t IsLevel2Access(const ClAccess* const ai_pAccess);

public:

	// User actions auditory.
	ClUserActionsAudit	UserActionsAudit ;
	
private:

	M4JavaManager	*m_poJavaManager;
	
	friend class ClChannel;
};

M4_DECL_M4DM ostream & operator << (ostream &ao_Output, ClChannelManager &ai_oChannelManager);



// generic channel eraser
class M4_DECL_M4DM ClChannelEraser
{
public:

	ClChannelEraser();
	~ClChannelEraser();

	typedef list< ClChannel * > ClChannelList;
	typedef ClChannelList::iterator itChannelList;

	typedef map< ClAccess * , ClChannel *, less<ClAccess*> > ClAccessMap;
	typedef ClAccessMap::iterator itAccessMap;

	m4return_t Add(ClChannel *ai_poRef);
	m4return_t Del(ClChannel *ai_poRef);

	m4return_t Destroy(m4bool_t ai_bIgnoreReferences = M4_FALSE);
	m4return_t Destroy(ClChannelDirectory &oDir);

private:

	void BuildAccessMap();
	m4bool_t IsExternalReferenced(m4bool_t ai_bIgnoreReferences, ClChannel *ai_pChannel);
	void AddUsedChannel(ClChannel *ai_pChannel);

	ClChannelList m_oChannelList;
	ClChannelList m_oUsedChannelsList;
	ClAccessMap  m_oAccessMap;

	/**
	 * Sirve para identificar que no intentamos borrar canales de ChannelManager distintos.
	 * Se le da valor con el ChannelManager del primer canal que se añada (siempre que sea distinto de 0).
	 */
	ClChannelManager* m_pChannelManager;
};

#endif  //__DM__HPP__ 
