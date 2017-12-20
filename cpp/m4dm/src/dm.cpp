//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           dm.cpp
// File:             dm.cpp
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
//==============================================================================

//## begin module.includes preserve=yes
#include "cldmcontext.hpp"
#include "m4stl.hpp"

#include "m4mdfac.hpp"
#include "dm_def.hpp"
#include "dm.hpp"
#include "node_knl.hpp"
#include "m4log.hpp"
#include "cltypes.hpp"
#include "iproxymanager.hpp"
#include "cllgadap.hpp"
#include "datacache.hpp"
#include "chan_knl.hpp"
#include "dumpcache.hpp"
#include "m4mdfac.hpp"
#include "access.hpp"
#include "m4cstrace.hpp"	// M4OEInit (abstract)
#include "cachedir.hpp"		// M4ObjCacheDirectory (abstract)
#include "m4objreg.hpp"
#include "m4mdrt.hpp"
#include "objdatacachemanager.hpp"
#include "objdatacacheimpl.hpp"
#include "m4unicode.hpp"
#include "m4thread.hpp"
#include "m4clock.hpp"
#include "m4jmanager.hpp"
#include "datastorages.hpp"
#include "cldatstoiod.hpp"
#include "chandata_ser.hpp"


//## end module.includes preserve=yes


///////////////////////////////////////////////////////////////////////////////
//
// ClChannelManager
//
ClChannelManager :: ClChannelManager()
: UserActionsAudit( this )
{
	// Reseteamos el estado del objeto
	_ResetState();
}

ClChannelManager :: ~ClChannelManager ()
{
	m4return_t	iResult ;

	iResult = End() ;

	if( m_poJavaManager != NULL )
	{
		delete( m_poJavaManager ) ;
		m_poJavaManager = NULL ;
	}

	_ResetState();
}

// Resetea el estado del objeto
void ClChannelManager :: _ResetState()
{
	m_bStarted = M4_FALSE;	// ChannelManager Inicializado S/N

	m_poDMContext		= NULL;	// Contexto C/S de DM

    m_poEnvironment		= NULL;	// Registry
	m_poSttWrapper		= NULL;	// Estadisticas
	m_poCacheDirectory	= NULL;	// Directorio de Caches
	m_poDataCache		= NULL; // Cache de Datos
    m_poFactory			= NULL;	// Factoria de MetaDatos
	m_poPresFac			= NULL;	// Factoria de Presentaciones
	m_poExecutor		= NULL;	// Ejecutor VM
	m_poLogonAdaptor	= NULL;	// Logon Adaptor
	m_poSession			= NULL;	// Session Object
	m_poCSTraces		= NULL;	// Trazas C/S

	m4VariantType var( 0.0 );	// Por poner algo
	SetDefaultCSType(var);

	m_poProxyManager	= NULL;
	m_poDCNodeHandle	= NULL;
	m_poDCItemHandle	= NULL;
	m_poDCBookMark		= NULL;
	m_poDCChannel		= NULL;
	m_bIsDCEnabled		= M4_FALSE;

	m_poMultiValidationChannel = NULL;

	m_lfCallBack = 0;

	m_pILicenseManager = NULL ;
	m_poObjDataCache = NULL ;
	m_pvDeletedAutos = NULL ;

	m_poJavaManager = NULL;
}


// Inicializacion del DM
m4return_t	ClChannelManager :: Init(
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
	ILicenseManager			*ai_pILicenseManager
	)
{
	m4uint8_t	iMode ;
	m4uint32_t	iMaxMemory ;
	m4uint32_t	iMaxObjects ;
	m4uint32_t	iRatio ;
	m4pcchar_t	pccDirectory ;


	if ( Initialised() ) {
		return M4_SUCCESS;
	}

	// Chequeo de parametros de entrada
	if (!ai_poDMContext	|| !ai_poEnvironment	|| !ai_poSttWrapper		|| !ai_poFactory	||
		!ai_poPresFac	|| !ai_poExecutor		|| !ai_poLogonAdaptor	|| !ai_poCSTraces	 ) {
		return M4_ERROR;
	}

	// Nos cacheamos los punteros necesarios
	m_poDMContext		= ai_poDMContext;		// Contexto C/S de DM
    m_poEnvironment		= ai_poEnvironment;		// Registry
	m_poSttWrapper		= ai_poSttWrapper;		// Estadisticas
    m_poFactory			= ai_poFactory;			// Factoria de MetaDatos
	m_poPresFac			= ai_poPresFac;			// Factoria de Presentaciones
	m_poExecutor		= ai_poExecutor;		// Ejecutor VM
	m_poLogonAdaptor	= ai_poLogonAdaptor;	// Logon Adaptor
	m_poCSTraces		= ai_poCSTraces;		// Trazas C/S
	m_poProxyManager	= ai_pProxyManager;		// Gestor de espacios proxy.
	m_pILicenseManager	= ai_pILicenseManager;

	// No hay que controlar el error de esto porque no es grave
	m_poCSTraces->StartupM4ObjectEngine();

	// Ponemos el Tipo CS por defecto
	m4uint32_t CST = m_poDMContext->GetCSType();
	m4VariantType var;
	var.Type = M4CL_CPP_TYPE_NUMBER;
	var.Data.DoubleData = CST;
	SetDefaultCSType(var);

	m_poCacheDirectory = ai_poCacheDirectory ;

	// jcr las cachés
	// No importa si la caché es nula

	iMode = m_poEnvironment->GetDataDiskCache() ;

	if( iMode != M4CL_CACHE_NONE )
	{
		iMaxMemory = m_poEnvironment->GetDataCacheMaxMemory();
		iMaxObjects = m_poEnvironment->GetDataCacheMaxNumObjects() ;
		iRatio = m_poEnvironment->GetDataCacheRefreshRatio() ;
		pccDirectory = m_poEnvironment->GetCacheDirectory() ;

		if( ai_poDataCache != NULL )
		{
			m_poDataCache = new ClDataCache( ai_poDataCache, pccDirectory, iMode, iMaxMemory, iMaxObjects, iRatio, 0, 0 ) ;

			CHECK_CHLOG_DEBUGF( m_poDataCache == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClDataCache ) ) << __LINE__ << __FILE__ ) ;

			if( m_poCacheDirectory != NULL )
			{
				m_poCacheDirectory->Attach( m_poDataCache ) ;
			}
		}
	}

	// Si todo va bien estamos inicializados
	m_bStarted = M4_TRUE ;

	return( M4_SUCCESS ) ;
}


// Inicializacion del DM
m4return_t	ClChannelManager :: Init(
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
	ILicenseManager			*ai_pILicenseManager
	) 
{
	m4uint8_t	iMode ;
	m4uint32_t	iMaxMemory ;
	m4uint32_t	iMaxObjects ;
	m4uint32_t	iRatio ;
	m4pcchar_t	pccDirectory ;


	if ( Initialised() ) {
		return M4_SUCCESS;
	}

	// Chequeo de parametros de entrada
	if (!ai_poDMContext	|| !ai_poEnvironment	|| !ai_poSttWrapper		|| !ai_poFactory	||
		!ai_poPresFac	|| !ai_poExecutor		|| !ai_poSession	|| !ai_poCSTraces	 ) {
		return M4_ERROR;
	}

	// Nos cacheamos los punteros necesarios
	m_poDMContext		= ai_poDMContext;		// Contexto C/S de DM
    m_poEnvironment		= ai_poEnvironment;		// Registry
	m_poSttWrapper		= ai_poSttWrapper;		// Estadisticas
    m_poFactory			= ai_poFactory;			// Factoria de MetaDatos
	m_poPresFac			= ai_poPresFac;			// Factoria de Presentaciones
	m_poExecutor		= ai_poExecutor;		// Ejecutor VM
	m_poSession			= ai_poSession;			// Session Object
	m_poCSTraces		= ai_poCSTraces;		// Trazas C/S
	m_poProxyManager	= ai_pProxyManager;		// Gestor de espacios proxy.
	m_pILicenseManager	= ai_pILicenseManager;

	// No hay que controlar el error de esto porque no es grave
	m_poCSTraces->StartupM4ObjectEngine();

	// Ponemos el Tipo CS por defecto
	m4uint32_t CST = m_poDMContext->GetCSType();
	m4VariantType var;
	var.Type = M4CL_CPP_TYPE_NUMBER;
	var.Data.DoubleData = CST;
	SetDefaultCSType(var);

	m_poCacheDirectory = ai_poCacheDirectory ;

	// jcr las cachés
	// No importa si la caché es nula

	iMode = m_poEnvironment->GetDataDiskCache() ;

	if( iMode != M4CL_CACHE_NONE )
	{
		iMaxMemory = m_poEnvironment->GetDataCacheMaxMemory();
		iMaxObjects = m_poEnvironment->GetDataCacheMaxNumObjects() ;
		iRatio = m_poEnvironment->GetDataCacheRefreshRatio() ;
		pccDirectory = m_poEnvironment->GetCacheDirectory() ;

		if( ai_poDataCache != NULL )
		{
			m_poDataCache = new ClDataCache( ai_poDataCache, pccDirectory, iMode, iMaxMemory, iMaxObjects, iRatio, 0, 0 ) ;

			CHECK_CHLOG_DEBUGF( m_poDataCache == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClDataCache ) ) << __LINE__ << __FILE__ ) ;

			if( m_poCacheDirectory != NULL )
			{
				m_poCacheDirectory->Attach( m_poDataCache ) ;
			}
		}
	}

	// Si todo va bien estamos inicializados
	m_bStarted = M4_TRUE ;

	return( M4_SUCCESS ) ;
}


// La función End libera cosas en cualquier caso
m4return_t	ClChannelManager :: End( void )
{
	// Si no está inicializado salimos
	if ( !Initialised() ) {
		return(	M4_SUCCESS ) ;
	}

#ifdef M4_VM_CLIENT
	if (m_poProxyManager != NULL)
	{
		delete m_poProxyManager;
		m_poProxyManager = NULL;
	}
#endif

	//ShutDown correcto en Single Machine con canales Proxy (11/5/01)
	m_poProxyManager = NULL;

	// No hay que controlar el error de esto porque no es grave
	m_poCSTraces->ShutdownM4ObjectEngine();

	DumpChannels("ClChannelManager::End");
	DumpCaches	("ClChannelManager::End");

	// En cualquier caso estamos desinicializados
	m_bStarted = M4_FALSE ;

	// Esto sólo debería existir en cliente
	if( m_poMultiValidationChannel != NULL )
	{
		m_poMultiValidationChannel->Destroy() ;
		m_poMultiValidationChannel = NULL ;
	}

	// me cepillo todos los canales (incluido el metachannel) que han quedado abiertos, cosa
	// que no debiera ocurrir:

    m4uint32_t	i, j, n ;
    ClChannel	*pChannel ;
	m4pchar_t	pListToDestroy[] = { M4_INSTANCE_M4_ERRORS, NULL } ;

    n = Channel.Count() ;

    for( i = 0 ; i < n ; i++ )
	{
        pChannel = Channel[ i ] ;

        M4_ASSERT (pChannel);
        M4_ASSERT (pChannel->GetpChannelManager () == this);

		// deletes specific channels
		for( j = 0; pListToDestroy [ j ] != NULL ; j++ )
		{
			if( pChannel->IsBuilt() && !strcmp( pListToDestroy[ j ], pChannel->GetpCMCR()->GetChannelId() ) )
			{
				if( pChannel->Destroy() == M4_SUCCESS )
				{
					n-- ;
					i-- ;
					break ;
				}
			}
		}

		if( pListToDestroy[ j ] != NULL )
		{
			continue ;
		}

        // para evitar la llamada a ::NotifyDestroy, que en este caso sobra
        pChannel->DetachAndKeepChannel() ;

		// otra chapuza más para arreglar el gp de destrucíón doble
        // delete pChannel;        
    }

    Channel.Clear() ;

	// jcr las cachés
	if( m_poDataCache != NULL )
	{
		if( m_poCacheDirectory != NULL )
		{
			m_poCacheDirectory->Detach( m_poDataCache ) ;
		}
		delete( m_poDataCache ) ;
		m_poDataCache = NULL ;
	}

	m_poCacheDirectory = NULL ;

	if( m_poObjDataCache != NULL )
	{
		delete( m_poObjDataCache ) ;
		m_poObjDataCache = NULL ;
	}

	if( m_pvDeletedAutos != NULL )
	{
		delete( m_pvDeletedAutos ) ;
		m_pvDeletedAutos = NULL ;
	}

	// Bug 0280561 Hay que cerrar el canal de auditoría antes que las cachés
	// User actions auditory.
	UserActionsAudit.End() ;

    return(	M4_SUCCESS ) ;
}


M4JavaManager*	ClChannelManager::GetJavaManager()
{
	if( m_poJavaManager == NULL )
	{
		m_poJavaManager = new M4JavaManager( m_poDMContext->GetCSType() ) ;
		CHECK_CHLOG_ERRORF( m_poJavaManager == NULL, NULL, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( M4JavaManager ) ) << __LINE__ << __FILE__ ) ;
	}
	return( m_poJavaManager ) ;
}


void	ClChannelManager::ReleaseEmptyJavaContexts( void )
{

	m4uint32_t	i = 0 ;
	m4uint32_t	iLength = 0 ;
	ClChannel	*poChannel = NULL ;


	iLength = Channel.Count() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poChannel = Channel[ i ] ;

		if( poChannel != NULL )
		{
			poChannel->ReleaseJavaContexts( M4_TRUE ) ;
		}
	}
}


IObjDataCache*	ClChannelManager::GetObjDataCache( m4pcchar_t ai_pccSession )
{
	if( m_poObjDataCache == NULL )
	{
		m_poObjDataCache = new ClObjDataCacheBySessionId( ai_pccSession, ClObjDataCacheManager::GetObjDataCache( m_poEnvironment ) ) ;
		CHECK_CHLOG_DEBUGF( m_poObjDataCache == NULL, NULL, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClObjDataCacheBySessionId ) ) << __LINE__ << __FILE__ ) ;
	}
	else
	{
		m_poObjDataCache->SetSessionId( ai_pccSession ) ;
	}

	return( m_poObjDataCache ) ;
}


m4return_t	ClChannelManager::AddDeletedAuto( m4uint32_t ai_iRemoteId )
{
	if( m_pvDeletedAutos == NULL )
	{
		m_pvDeletedAutos = new vector<m4uint32_t> ;
		CHECK_CHLOG_DEBUGF( m_pvDeletedAutos == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( vector<m4uint32_t> ) ) << __LINE__ << __FILE__ ) ;
	}

	if( m_pvDeletedAutos->size() == 0 )
	{
		// Reservamos un espacio de 20. Ya es dificil que se mueran 20 canales sin una transacción
		m_pvDeletedAutos->reserve( 20 ) ;
	}

	m_pvDeletedAutos->push_back( ai_iRemoteId ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClChannelManager::SerializeDeletedAutos( ClGenericIODriver& IOD )
{

	m4uint32_t	i = 0 ;
	m4uint32_t	iLength = 0 ;
	m4uint32_t	iRemoteId = 0 ;


	if( m_pvDeletedAutos == NULL )
	{
		iLength = 0 ;
	}
	else
	{
		iLength = m_pvDeletedAutos->size() ;
	}


	if ( IOD.Write( iLength ) == M4_ERROR )
	{
		return( M4_ERROR ) ;
	}

	if( iLength > 0 )
	{
		for( i = 0 ; i < iLength ; i++ )
		{
			iRemoteId = m_pvDeletedAutos->back() ;

			if ( IOD.Write( iRemoteId ) == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			m_pvDeletedAutos->pop_back() ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClChannelManager :: CreateChannel( ClChannel* &ao_poChannel )
{
	// Construye el identificador único a partir de un contador
	static	m4uint32_t	s_iHandle = 0 ;
	static	ClMutex		s_oMutex( M4_TRUE ) ;

	m4uint32_t	iHandle = 0 ;

	s_oMutex.Lock() ;
	iHandle = ++s_iHandle ;
	s_oMutex.Unlock() ;

	ao_poChannel = new ClChannel( this, iHandle ) ;
	CHECK_CHLOG_DEBUGF( ao_poChannel == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClChannel ) ) << __LINE__ << __FILE__ ) ;

    return( M4_SUCCESS ) ;
}


m4return_t	ClChannelManager :: CreateChannel( ClChannel* &ao_poChannel, m4uint32_t ai_iHandle )
{
	// Utiliza el identificador único que se le pasa
	ao_poChannel = new ClChannel( this, ai_iHandle ) ;
	CHECK_CHLOG_DEBUGF( ao_poChannel == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClChannel ) ) << __LINE__ << __FILE__ ) ;

    return( M4_SUCCESS ) ;
}


m4return_t	ClChannelManager :: CloneChannel( ClChannel* &ao_poChannel, ClChannel* ai_poSourceChannel )
{

	m4return_t		iResult = M4_ERROR ;
	M4DataStorage	*poDataStorage = NULL ;


	ao_poChannel = NULL ;

	CHECK_CHLOG_ERRORF( ai_poSourceChannel == NULL, NULL, M4_CH_DM_NULL_ARGUMENT, "ai_poSourceChannel" << "ClChannelManager::CloneChannel" << __LINE__ << __FILE__ ) ;

	poDataStorage = M4DataStorage::GetNewDataStorage() ;
	CHECK_CHLOG_ERRORF( poDataStorage == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( M4DataStorage ) ) << __LINE__ << __FILE__ ) ;

	iResult = _CloneChannel( ao_poChannel, ai_poSourceChannel, poDataStorage ) ;

	M4DataStorage::DeleteDataStorage( poDataStorage ) ;

	return( iResult ) ;
}


m4return_t	ClChannelManager :: _CloneChannel( ClChannel* &ao_poChannel, ClChannel* ai_poSourceChannel, M4DataStorage *ai_poDataStorage )
{

	ClChannel_Data			*poChannelData = NULL ;
	ClDataStoreIODriver		oInputIOD ;
	ClDataStoreIODriver		oOutputIOD ;
	ClChannelDataSerializer	oSerializer ;
	ClChannelDataSerializer	oDeSerializer ;


	ao_poChannel = NULL ;

	CHECK_CHLOG_ERRORF( ai_poSourceChannel == NULL, NULL, M4_CH_DM_NULL_ARGUMENT, "ai_poSourceChannel" << "ClChannelManager::_CloneChannel" << __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( ai_poDataStorage == NULL, NULL, M4_CH_DM_NULL_ARGUMENT, "ai_poDataStorage" << "ClChannelManager::_CloneChannel" << __LINE__ << __FILE__ ) ;

	m4return_t iResult = oInputIOD.Init( GEN_IO_MODE_WRITE, ai_poDataStorage ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = oSerializer.Serialize( oInputIOD, ai_poSourceChannel->GetpChannel_Data(), M4_FALSE, M4CL_RECORD_SERIAL_ALL, M4CL_ITEM_SERIAL_ALL  );
	oInputIOD.End();

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = oOutputIOD.Init( GEN_IO_MODE_READ, ai_poDataStorage ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	poChannelData = new ClChannel_Data() ;
	CHECK_CHLOG_ERRORF( poChannelData == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClChannel_Data ) ) << __LINE__ << __FILE__ ) ;

	iResult = oDeSerializer.DeSerialize( oOutputIOD, m_poFactory, NULL, NULL, poChannelData ) ;
	oOutputIOD.End();

	if( iResult != M4_SUCCESS )
	{
		delete( poChannelData ) ;
		return( M4_ERROR ) ;
	}

	iResult = CreateChannel( ao_poChannel ) ;

	if( iResult != M4_SUCCESS || ao_poChannel == NULL )
	{
		delete( poChannelData ) ;
		return( M4_ERROR ) ;
	}

	iResult = ao_poChannel->BuildFromChannelData( poChannelData ) ;

	if( iResult != M4_SUCCESS )
	{
		if( ao_poChannel->GetpChannel_Data() == NULL )
		{
			delete( poChannelData ) ;
		}
		ao_poChannel->Destroy() ;
		ao_poChannel = NULL ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClChannelManager :: Reset ()
{
	ClChannelEraser oEraser;

	return oEraser.Destroy(Channel);
}

ClChannel*	ClChannelManager :: GetChannelByHandle( m4int32_t ai_hChannel )
{

	m4uint32_t	i = 0 ;
	m4uint32_t	iLength = 0 ;
	ClChannel	*poChannel = NULL ;


	iLength = Channel.Count() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poChannel = Channel[ i ] ;

		if( poChannel != NULL && poChannel->GetHandle() == ai_hChannel )
		{
			return( poChannel ) ;
		}
	}

	return( NULL ) ;
}


ClAccess* ClChannelManager::GetAccessByHandle( m4int32_t ai_hAccess )
{

	m4uint32_t	i = 0 ;
	m4uint32_t	j = 0 ;
	m4uint32_t	iChannelLength = 0 ;
	m4uint32_t	iAccessLength = 0 ;
	ClChannel	*poChannel = NULL ;
	ClAccess	*poAccess = NULL ;


	iChannelLength = Channel.Count() ;

	for( i = 0 ; i < iChannelLength ; i++ )
	{
		poChannel = Channel[ i ] ;

		if( poChannel != NULL )
		{
			iAccessLength = poChannel->Access.Count() ;

			for( j = 0 ; j < iAccessLength ; j++ )
			{
				poAccess = poChannel->Access[ j ] ;

				if( poAccess != NULL && poAccess->GetHandle() == ai_hAccess )
				{
					return( poAccess ) ;
				}
			}
		}
	}
	
	return( NULL ) ;
}


ClChannel*	ClChannelManager :: GetSessionChannel(void)
{
	return m_poLogonAdaptor->GetSessionChannelPointer();
}

m4return_t	ClChannelManager :: SetSessionChannel(ClChannel* ai_poChannel)
{
	return m_poLogonAdaptor->SetSessionChannel(ai_poChannel);
}

void ClChannelManager :: DumpChannels(m4pchar_t ai_pcCalledFrom, ostream *ai_pStream)
{
	static m4bool_t bFileCreated = M4_FALSE;
	m4uint8_t		iDebugLevel;
	m4uint32_t		iLevel2;
    m4pcchar_t		pccPath;
    m4char_t		acFile[ M4CH_MAX_FILE + 1 ];
	m4VariantType	CsType ;
    ostream			*pStream = ai_pStream;
	m4uint32_t		iTotalSize = 0;


	iDebugLevel = ClVMDebugEnv::getInstance()->getDebugLevel() ;
	
	if( iDebugLevel < 2 )
	{
		return;
	}
    

	if (ai_pStream == NULL)
	{
		pccPath = m_poEnvironment->GetDebugDirectory() ;
		
		if( pccPath != 0 && *pccPath != '\0' )
		{
			sprintf( acFile, "%s%s%s", pccPath, M4_BACK_SLASH, "ChannelManager.txt" ) ;
		}
		else
		{
			sprintf( acFile, "%s", "ChannelManager.txt" ) ;
		}
		
		// UNICODE FILE
		int iLength = -1 ;
		char* pcFileName = M4CppToANSI( acFile, iLength ) ;

		if (M4_FALSE == bFileCreated)
		{
			pStream = new ofstream( pcFileName ) ;
			bFileCreated = M4_TRUE;
		}
		else
		{
			pStream = new ofstream( pcFileName, ios::out|ios::app) ;
		}

		delete [] pcFileName ;
	}

    if( pStream != NULL )
    {
		ostream &Output = *pStream;

		m4uint32_t iNumChan, i;
		ClChannel * pChanAux;

		iNumChan = Channel.Count ();

		if (ai_pcCalledFrom == 0)
			ai_pcCalledFrom = "";

        m4char_t acTime[ M4_SECOND_END + 1 ] ;
        ClActualDate( acTime, M4_SECOND_END + 1 ) ;

		Output << "============ Dumping Meta4Object Engine Meta4Objects ===============================================" << endl;
		Output << "   Called from <" << ai_pcCalledFrom << ">" << endl;
		Output << "   Number of Meta4Objects = <" << iNumChan << ">" << endl;
		Output << "   This = <" << this << ">" << endl;
		Output << "   Current Thread = <" << M4Thread::GetCurrentIdThread() << ">" << endl;
		Output << "   GMT Time = <" << acTime << ">" << endl;
		Output << "   TickCount = <" << (m4uint32_t)ClPrecisionTicker::GetTimeTick() << ">" << endl;

		for ( i = 0; i < iNumChan; i++ )
		{
			ClSize			iSize;
			pChanAux = Channel[i];

			Output << "   ------ Meta4Object " << i << endl;

			if (M4_TRUE == pChanAux->IsBuilt())
			{
				m4pchar_t pcBuildType;

				switch (pChanAux->GetpChannel_Data()->GetpExtInfo()->MCR.BuildType())
				{
					case ClMCROwner::ID:
						pcBuildType = "ID";
						break;
					case ClMCROwner::File:
						pcBuildType = "File";
						break;
					case ClMCROwner::Access:
						pcBuildType = "Access";
						break;
					case ClMCROwner::Data:
						pcBuildType = "Data";
						break;
					case ClMCROwner::MCR:
						pcBuildType = "MCR";
						break;
					default:
						pcBuildType = "NotBuilt";
						Output << "          BuildType = <" << "NotBuilt" << ">" << endl;
						continue;
						break;
				};

				pChanAux->CsType.Get( CsType ) ;
				pChanAux->GetSize( iSize ) ;
				iTotalSize += iSize;

				Output << "          Id = <" << pChanAux->GetpChannelDef()->Id() << ">" << endl;
				Output << "          This = <" << pChanAux << ">" << endl;
				Output << "          Handle = <" << pChanAux->GetHandle() << ">" << endl;
				Output << "          BuildType = <" << pcBuildType << ">" << endl;
				Output << "          CsType = <" << CsType.Data.DoubleData << ">" << endl;
				Output << "          Size = <" << m4uint32_t( iSize ) << ">" << endl;
				Output << "          Properties = <" << pChanAux->Property << ">" << endl;
				Output << "          Access Count = <" << pChanAux->Access.Count() << ">" << endl;

				// dump accesses
				{
					Output << "          Accesses = <";

					m4bool_t bFirst = M4_TRUE;
					m4int32_t nAccesses = pChanAux->Access.Count();
					for (m4int32_t i=0; i<nAccesses; ++i) 
					{
						if (bFirst == M4_TRUE) 
						{
							bFirst = M4_FALSE;
						}
						else 
						{
							Output << ",";
						}
						Output << pChanAux->Access[i];
					}

					Output << ">" << endl;
				}

				Output << "          Self Access = <" << pChanAux->Level2.IsSelfAccessCreated() << ">" << endl;

				iLevel2 = pChanAux->Level2.m_oInstanceMap.Count();
				if (iLevel2>0)
				{
					Output << "          Level2 Count = <" << iLevel2 << ">" << endl;
					while (iLevel2>0)
					{
						iLevel2--;
						Output << "               <" 
							<< pChanAux->Level2.m_oInstanceMap.GetInstanceName(iLevel2) 
							<< " " << pChanAux->Level2.m_oInstanceMap.GetAccess(iLevel2) 
							<< " " << pChanAux->Level2.m_oInstanceMap.GetChannelName(iLevel2) 
							<< " " << pChanAux->Level2.m_oInstanceMap.GetChannelHandle(iLevel2) 
							<< ">" << endl;
					}
				}
			}
			else
			{
				Output << "          BuildType = <" << "NotBuilt" << ">" << endl;
			}
		}

		Output << "   Meta4Object Engine <" << this << "> Total Size = <" << iTotalSize << ">" << endl;

		if (ai_pStream == NULL)
		{
			delete ((ofstream*) pStream);
		}
	}
}


void ClChannelManager :: DumpCaches(m4pchar_t ai_pcCalledFrom, ostream *ai_pStream)
{
	static m4bool_t bFileCreated = M4_FALSE;
	m4uint8_t	iDebugLevel;
    m4pcchar_t	pccPath;
    m4char_t	acFile[ M4CH_MAX_FILE + 1 ];
    ostream     *pStream = ai_pStream;


	iDebugLevel = ClVMDebugEnv::getInstance()->getDebugLevel() ;
	
	if( iDebugLevel < 2 )
	{
		return;
	}

    
	if (ai_pStream == NULL)
	{
		pccPath = m_poEnvironment->GetDebugDirectory() ;
		
		if( pccPath != 0 && *pccPath != '\0' )
		{
			sprintf( acFile, "%s%s%s", pccPath, M4_BACK_SLASH, "CacheDump.txt" ) ;
		}
		else
		{
			sprintf( acFile, "%s", "CacheDump.txt" ) ;
		}
		
		// UNICODE FILE
		int iLength = -1 ;
		char* pcFileName = M4CppToANSI( acFile, iLength ) ;

		if (M4_FALSE == bFileCreated)
		{
			pStream = new ofstream( pcFileName ) ;
			bFileCreated = M4_TRUE;
		}
		else
		{
			pStream = new ofstream( pcFileName, ios::out|ios::app) ;
		}

		delete [] pcFileName ;
	}

    if( pStream != NULL )
    {
		ostream &Output = *pStream;

		ClDumpCacheVisitor oCacheVisitor(pStream);

		oCacheVisitor.SetDumpDates(M4_FALSE);
		
		ClBaseCacheInterface * pCache = 0;

		if (ai_pcCalledFrom == 0)
			ai_pcCalledFrom = "";

	    Output << "============ Dumping Caches ===============================================" << endl;
		Output << "   Called from <" << ai_pcCalledFrom << ">" << endl;

		Output << endl << "Metadata Cache (CMCR)-------------" << endl;
		pCache = (ClBaseCacheInterface *) m_poFactory->MDFac_GetMDCache();
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}

		Output << endl << "Security Cache (CSCR)-------------" << endl;
		pCache = (ClBaseCacheInterface *) m_poFactory->MDFac_GetSecCache();
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}

		Output << endl << "Map Cache-------------" << endl;
		pCache = (ClBaseCacheInterface *) m_poFactory->MDFac_GetMapCache();
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}

/* No podemos depender de la presentation. Lo suyo sería que nos pasasen las cachés
		Output << endl << "Presentation Cache-------------" << endl;
		pCache = (ClBaseCacheInterface *)GetPresFac()->GetCache();
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}
*/
		Output << endl << "Data Cache-------------" << endl;
		pCache = (ClBaseCacheInterface *)GetCache();
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}
		
		Output <<  endl << endl;

		if (ai_pStream == NULL)
		{
			delete ((ofstream*) pStream);
		}
	}
}

// dump operator
ostream & operator << (ostream &ao_Output, ClChannelManager &ai_oChannelManager)
{
	ai_oChannelManager.DumpChannels("Dumping info",&ao_Output);
	ai_oChannelManager.DumpCaches("Dumping info",&ao_Output);
	return ao_Output;
}

// Data Check public functions

void ClChannelManager :: SetDCData(ClChannel *ai_poChannel, ClAccessBookMark *ai_poBookMark, ClHandle *ai_poNodeHandle, ClHandle *ai_poItemHandle)
{
	m_poDCChannel    = ai_poChannel;
	m_poDCBookMark   = ai_poBookMark;
	m_poDCNodeHandle = ai_poNodeHandle;
	m_poDCItemHandle = ai_poItemHandle;
}


m4bool_t ClChannelManager :: IsDCData()
{
	return (m_bIsDCEnabled && m_poDCChannel != NULL) ? M4_TRUE : M4_FALSE;
}

void ClChannelManager :: GetDCData(ClChannel *&ao_poChannel, ClAccessBookMark *&ao_poBookMark, ClHandle *&ao_poNodeHandle, ClHandle *&ao_poItemHandle)
{
	ao_poChannel	= m_poDCChannel;
	ao_poBookMark   = m_poDCBookMark;
	ao_poNodeHandle = m_poDCNodeHandle;
	ao_poItemHandle = m_poDCItemHandle;
}


m4uint32_t
ClChannelManager :: GetSize()
{
	ClSize	oSize;
	oSize += sizeof(ClChannelManager);
		
	oSize -= sizeof(Channel);
		if (Channel.GetSize(oSize)!=M4_SUCCESS){
			return 0;
		}

	oSize -= sizeof(DefaultCSType);
		if (DefaultCSType.GetSize(oSize)!=M4_SUCCESS){
			return 0;
		}

	oSize -= sizeof(LaneManager);
		if (LaneManager.GetSize(oSize)!=M4_SUCCESS){
			return 0;
		}
	
	oSize -= sizeof(Property);
		if (Property.GetSize(oSize)!=M4_SUCCESS){
			return 0;
		}

	return oSize;
}

ClProxySpace * ClChannelManager::GetProxySpace(void)
{
	if (!GetDMContext()->ExecutingInClient())
		return NULL;

	return m_poProxyManager->GetSpace("Proxy", M4_FALSE);
}

void ClChannelManager::ReleaseProxy(void)
{
	// fix bug 0060415
	// called from logoff to release pending state
	// obs: should release the channels? actually the channels must be destroyed manually
	if (m_poProxyManager != NULL)
	{
		m_poProxyManager->Reset();
	}
}

ClChannel * ClChannelManager::GetMultiValidationChannel( void )
{

	m4return_t iRet;
	
	if(!m_poMultiValidationChannel)
	{
		CreateChannel(m_poMultiValidationChannel);

		iRet= m_poMultiValidationChannel->BuildFromId("SCH_MULTI_VALIDATE");

		if(iRet==M4_ERROR)
		{
			m_poMultiValidationChannel->Destroy();
			m_poMultiValidationChannel = NULL;
		}
	}

	return m_poMultiValidationChannel;
}


m4EmulationLevel ClChannelManager::EmulationLevel(void)	{

	//
	// If we are in a distributed client build or a server
	// then there is no emulation
	//

	if (!GetDMContext()->IsSM() ){
		return M4CL_EMULATE_NONE;
	} else {
		//
		// if we are in an emulator build, return the type of emulation
		//
		if (M4CL_CSTYPE_BOTH == (m4int32_t)GetDefaultCSType())
		{
			return M4CL_EMULATE_FAT;
		}
		//else if (GetDMContext()->IsSM())
		//{
			return M4CL_EMULATE_SINGLE_MACHINE;
		//}
		//else
		//{
		//	return M4CL_EMULATE_CLIENT_SERVER;
		//}
	}

}

/**
 * Bug 0059142.
 * Comprueba si un acceso es de nivel2. Es decir, si el acceso está en alguna lista de enlaces de nivel2
 * de alguno de los canales del channelManager.
 *
 * @param ai_pAccess	Puntero al acceso que se quiere saber si es de nivel2.
 * @return	m4bool_t	M4_TRUE --> el acceso es de nivel2, M4_FALSE --> el canal no es de nivel2.
 */
m4bool_t ClChannelManager::IsLevel2Access(const ClAccess* const ai_pAccess)
{
	ClChannel* pChannel = 0;
	m4uint32_t level2AccessCount = 0;
	m4uint32_t channelCount = Channel.Count();
	ClAccess* pLevel2Access;
	for (m4uint32_t i = 0; i < channelCount; i++) {
		pChannel = Channel[i];
		level2AccessCount = pChannel->Level2.m_oInstanceMap.Count();
		for (m4uint32_t j = 0; j < level2AccessCount; j++) {
			pLevel2Access = pChannel->Level2.m_oInstanceMap.GetAccess(j);
			if (ai_pAccess == pLevel2Access) return M4_TRUE;
		}
	}
	return M4_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
// ClChannelEraser
//

ClChannelEraser::ClChannelEraser()
	: m_pChannelManager(0)
{
}

ClChannelEraser::~ClChannelEraser()
{
	Destroy();
}

m4return_t ClChannelEraser::Add(ClChannel *ai_poRef)
{
	if (!ai_poRef) return M4_SUCCESS;
	
	itChannelList it = find(m_oChannelList.begin(),m_oChannelList.end(),ai_poRef);

	if (it != m_oChannelList.end()) return M4_SUCCESS;

	// Sirve para identificar que no intentamos borrar canales de ChannelManager distintos.
	// Se le da valor con el ChannelManager del primer canal que se añada (siempre que sea distinto de 0).
	// Si se intenta añadir un canal que no pertenece al mismo ChannelManager no se mete en la lista.
	// Un ejemplo de esta situación es le canal sesión en un espacio proxy en SM. Cuando se termina de ejecutar un método
	// se desatacha el canal sesión, pero puede que este esté conectado por nivel2 por otro canal. Si no hacemos esta comprobación
	// y cerramos el espacio proxy, se borra el canal (indirectamente por estar conectado por L2). Al volver a la parte cliente
	// si se accede al canal sesión da un GP porque ya está borrado.
	if (!m_pChannelManager) {
		m_pChannelManager = ai_poRef->GetpChannelManager();
	} else if (m_pChannelManager != ai_poRef->GetpChannelManager()) {
		return M4_SUCCESS;
	}

	m_oChannelList.push_back(ai_poRef);

	// adds level2 instances
	m4uint32_t iNAccess = ai_poRef->Level2.m_oInstanceMap.Count();
	while (iNAccess--) {
		Add(ai_poRef->Level2.m_oInstanceMap.GetAccess(iNAccess)->GetpChannel());
	}

	return M4_SUCCESS;
}

m4return_t ClChannelEraser::Del(ClChannel *ai_poRef)
{
	if (ai_poRef == NULL)
		return M4_SUCCESS;

	itChannelList it = find(m_oChannelList.begin(),m_oChannelList.end(),ai_poRef);

	if (it == m_oChannelList.end())
		return M4_ERROR;

	m_oChannelList.erase(it);
	return M4_SUCCESS;
}

void ClChannelEraser::AddUsedChannel(ClChannel *ai_pChannel)
{
	// checks the list
	itChannelList it = find(m_oUsedChannelsList.begin(),m_oUsedChannelsList.end(),ai_pChannel);
	if (it != m_oUsedChannelsList.end())
		return;

	m_oUsedChannelsList.push_back(ai_pChannel);

	// adds Level 2 instances
	m4uint32_t iNAccess = ai_pChannel->Level2.m_oInstanceMap.Count();
	while (iNAccess--)
		AddUsedChannel(ai_pChannel->Level2.m_oInstanceMap.GetAccess(iNAccess)->GetpChannel());
}

void ClChannelEraser::BuildAccessMap()
{
	ClAccess *pAccess = NULL ;

	m_oAccessMap.clear();

	// includes the normal access list
	itChannelList it;
	for (it = m_oChannelList.begin(); it != m_oChannelList.end(); ++it)
	{
		m4uint32_t iNAccess = (*it)->Access.Count();
		while (iNAccess--)
			m_oAccessMap.insert(ClAccessMap::value_type((*it)->Access[iNAccess],*it));
	}

	// deletes references to L2 accesses
	for (it = m_oChannelList.begin(); it != m_oChannelList.end(); ++it)
	{
		m4uint32_t iNAccess = (*it)->Level2.m_oInstanceMap.Count();
		while (iNAccess--)
		{
			pAccess = (*it)->Level2.m_oInstanceMap.GetAccess(iNAccess);
			m_oAccessMap.erase(pAccess);
		}

		/* Bug 0106520
		El selfaccess tambien es un nivel 2 y hay que eliminar su referencia
		*/
		pAccess = (*it)->Level2.GetSelfAccess() ;

		if( pAccess != NULL )
		{
			m_oAccessMap.erase( pAccess ) ;
		}
	}

	// generates a list of used channels
	m_oUsedChannelsList.clear();
	
	for (itAccessMap it2 = m_oAccessMap.begin(); it2 != m_oAccessMap.end(); ++it2)
		AddUsedChannel((*it2).second);
}

m4return_t ClChannelEraser::Destroy(m4bool_t ai_bIgnoreReferences)
{	
	if (!ai_bIgnoreReferences)
		BuildAccessMap();

	// destrois L2 connection
	itChannelList it;
	for (it = m_oChannelList.begin(); it != m_oChannelList.end(); ++it)
	{
		if (IsExternalReferenced(ai_bIgnoreReferences,*it))
			continue;

		(*it)->Level2.EraseAllL2Instances(M4_FALSE, M4_FALSE, NULL);
	}

	// destroys external accesses
	for (it = m_oChannelList.begin(); it != m_oChannelList.end(); ++it)
	{
		if (IsExternalReferenced(ai_bIgnoreReferences,*it))
			continue;

		m4uint32_t iNAccess = (*it)->Access.Count();
		for (m4uint32_t iCounter2=0;iCounter2<iNAccess;iCounter2++)
			(*it)->Access[0]->Destroy();
	}

	// releases all channels
	m4return_t iRes = M4_SUCCESS;
	for (it = m_oChannelList.begin(); it != m_oChannelList.end(); ++it)
	{
		if (IsExternalReferenced(ai_bIgnoreReferences,*it))
			continue;

		iRes = (*it)->Destroy();
		M4_ASSERT(iRes == M4_SUCCESS);

		if (iRes != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF(M4_CH_DM_ERASER_DESTROY, M4Obj((*it)->GetpChannelDef ()->GetpCMCR ()));
		}
	}

	m_oChannelList.clear();
	m_oAccessMap.clear();
	m_oUsedChannelsList.clear();

	return iRes;
}

m4return_t ClChannelEraser::Destroy(ClChannelDirectory &oDir)
{
	m4uint32_t iNChannels = oDir.Count();
	while(iNChannels--)
		Add(oDir[iNChannels]);

	return Destroy(M4_TRUE);
}

m4bool_t ClChannelEraser::IsExternalReferenced(m4bool_t ai_bIgnoreReferences, ClChannel *ai_pChannel)
{
	if (ai_bIgnoreReferences)
		return M4_FALSE;

	// checks the list
	itChannelList it = find(m_oUsedChannelsList.begin(),m_oUsedChannelsList.end(),ai_pChannel);
	
	if (it == m_oUsedChannelsList.end())
		return M4_FALSE;

	return M4_TRUE;
}

