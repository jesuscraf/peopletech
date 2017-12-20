//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objserviceclientsm.cpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                29-02-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la Implementación
//	  de la Maquina Virtual
//
//
//==============================================================================

//Esta implementación es específica para Single Machine

#include "clm4objserviceclientsm.hpp"
#include "clm4objservicecontextclient.hpp"	//Contexto cliente
#include "clm4objservicecontextserver.hpp"	//Contexto server
#include "cachefac.hpp"						//Factoria de caches
#include "presfaccli.hpp"					//Factoria de presentaciones cliente
#include "m4lgadapi.hpp"					//Logon Adaptor
#include "executor.hpp"						//VM Executor
#include "m4regimpl.hpp"					//RegCliente
#include "m4mdfaci.hpp"						//Factoria de metadatos
#include "dm.hpp"							//ChannelManager
#include "sttwrap.hpp"						//ClVMStatisticsWrapper
#include "tablafuncdesc.hpp"				//ClFuncTableDesc
#include "tablatt.hpp"						//ClAtributeFuncTable
#include "m4mdadapimp.hpp"					//ClMDAdaptorImp
#include "cachedircli.hpp"					//ClCacheDirectoryClient
#include "m4cstrace.hpp"					//ClCsTrace
#include "m4trcfil.hpp"						//ClCsFileTrace
#include "cldmcontext.hpp"					//ClDMContextClient
#include "clexadap.hpp"						//ClExecAdaptor
#include "clldb_wrapper.hpp"				//ILdb, ClLdb_Wrapper
#include "ldbcontext.hpp"					//ClLdbContextEmul
#include "conn_provider_server.hpp"			//ClLConn_Provider_Server
#include "clappclientadapsm.hpp"			//Adaptador del Client Api SM
#include "clm4objservicecontextserver.hpp"	//Contexto server
#include "cachedirsrv.hpp"					//ClCacheDirectoryServer
#include "presfacsrv.hpp"					//Factoria de presentaciones server
#include "genproxy.hpp"						// Para el gestor de espacios proxys server.
#include "licensemanager.hpp"
#include "objdatacachemanager.hpp"
#include "replaceliterals.hpp"
#include "m4java.h"							//API Java




/** Crea un nuevo ClM4ObjServiceClientSM. Sólo puede crear objetos la factoria. */
ClM4ObjServiceClientSM::ClM4ObjServiceClientSM()
	: m_poLdbContext(0)
	, m_pServerProxyManager(0)
{}

/**
 * Inicializa un ClM4ObjServiceClientSM.
 *
 * @return m4return_t.
 */
m4return_t ClM4ObjServiceClientSM::Init() 
{
	m4return_t iRet = M4_SUCCESS;
	m_poRegistry					= new ClM4ObjRegSM();
	iRet = m_poRegistry->Init();
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//////////////////////////////////
	// Creacion de objetos
	//////////////////////////////////

	//LDB. Se comparte en la parte Client y server
	m_pLDB									= new ClLdb_Wrapper();

	//Se crea un LicenseManager cliente, pero no se inicializa, 
	//se inicializara en el server de la SM.
	//Lo cambiamos para crear una de tipo server que se compartirá.
	m_pLicenseManager						= new ClLicenseManagerServer();

	m_poLdbContext							= new ClLdbContextEmul();

	m_pExecutor								= new ClExecutor(); 

	//LogonAdaptor. Se comparte en la parte cliente y server 
	m_pLogonAdaptor							= new ClLogonAdaptorImp();	

	m_pClientAPI							= new ClAppClientAdapSM();
		
	m_poM4ObjServiceContext					= new ClM4ObjServiceContextClient(m_pClientAPI);

	m_poFactory								= new ClFactoryClientEmul();
		
	m_poChannelManager						= new ClChannelManager();

	m_poStatisticsWrap						= new ClVMStatisticsWrapper(M4_TRUE /* Es client */);

	ClFuncTableDesc	*poFunctionTable		= ClFuncTableDesc::GetInstance();

	ClAtributeFuncTable	*poAtributeTable	= ClAtributeFuncTable::GetInstance();

	m_poCsTrace								= NULL ;

	m4int8_t iCsTraceLevel					= m_poRegistry->GetCsTraceLevel() ;

	m4int8_t iVbTraceLevel					= m_poRegistry->GetVbTraceLevel() ;

	if( (iCsTraceLevel != 0) || (iVbTraceLevel != 0) )
		m_poCsTrace							= new ClCsFileTrace(iCsTraceLevel, iVbTraceLevel) ;	// Trazas a fichero
		else
		m_poCsTrace							= new ClCsTrace() ;		// Trazas a Log
	
	m_poMDAdaptor							= new ClMDAdaptorImp();
	m_poExecAdap							= new ClExecAdaptor();

	m_poCacheDirectory						= new ClCacheDirectoryClient();
	m_poCacheFactory						= new ClCacheFactory();
			
	m_poPresentationFac						= new ClPresentationFactoryClient();

	m_poConnProvider						= new ClLConn_Provider_Server();	/* Gestor de Conexiones */

	m_pProxyManager							= new ClClientProxyManager(this, m_pClientAPI);	// El proxy manager para el lado cliente.
	m_pServerProxyManager					= new ClServerProxyManagerEmul();				// El proxy manager para el lado servidor.


	//////////////////////////////////
	// Inicializacion de objetos
	//////////////////////////////////

	//Inicialización de la traza
	iRet = m_poCsTrace->Init(m_poRegistry	/* Registry inicializado */ );
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización de la Factoria de Caches
	iRet = m_poCacheFactory->Init(m_poRegistry	/* Registry inicializado */ );
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}
	iRet = m_poCacheFactory->CreateCaches();
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	iRet = ((ClAppClientAdapSM*)m_pClientAPI)->Init(this, m_poCacheFactory->GetCMCRCache(), m_poCacheFactory->GetCSCRCache(), m_poCacheFactory->GetPresCache(), m_poCacheFactory->GetMapCache(), m_poCacheFactory->GetDataCache());
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización del Directorio de Caches
	iRet = ((ClCacheDirectoryClient*) m_poCacheDirectory)->Init(m_poRegistry, /* Registry inicializado */
						m_poStatisticsWrap, 
						m_poMDAdaptor);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}
	//Inicialización de la Factoria de MetaDatos
	iRet = ((ClFactoryClientEmul*) m_poFactory)->Init(
									m_poChannelManager,
									m_poRegistry, 
									m_pLogonAdaptor,					//Logon Adaptor
									m_poCacheFactory->GetCMCRCache(),	//MDCache. La factoria tiene que estar inicializada
									m_poCacheFactory->GetCSCRCache(),	//SecCache. La factoria tiene que estar inicializada
									m_poCacheFactory->GetMapCache(),	//MapCache. La factoria tiene que estar inicializada
									m_poCacheDirectory,					//Tiene que estar inicializado
									m_poStatisticsWrap,
									poFunctionTable,
									poAtributeTable,
									m_poCsTrace,
									m_poMDAdaptor);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}
	
	//Inicialización DM (Data Manager)
	m_poDMContext = new ClDMContextClient( M4_TRUE /* Si es Single Machine */);

	iRet = m_poChannelManager->Init(
		m_poDMContext,
		m_poRegistry,
		m_poStatisticsWrap,
		m_poCacheDirectory,
		m_poCacheFactory->GetDataCache(),   //Data cache
		m_poFactory,
		m_poPresentationFac,
		m_pExecutor,
		m_pLogonAdaptor,
		m_poCsTrace,
		m_pProxyManager,
		m_pLicenseManager
		);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//LDB
	iRet = m_pLDB->Init(m_poRegistry,
				  m_poFactory,
				  m_poLdbContext);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	iRet = m_pLDB->SetCacheDirectory(m_poCacheDirectory);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	iRet = M4SetLiteralsCacheDirectory(m_poCacheDirectory);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización Presentaciones
	iRet = ((ClPresentationFactoryClient*)m_poPresentationFac)->Init(
						m_poRegistry, /* Registry inicializado */
					    m_poCsTrace, 
					    m_poStatisticsWrap,
						m_poChannelManager,
					    m_poMDAdaptor, 
					    m_poCacheDirectory, /* CacheDirectory inicializado */
					    m_poCacheFactory->GetPresCache() /*PresCache*/ /*La factoria tiene que estar inicializada*/
						);

	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}
	
	iRet = m_poExecAdap->Init(m_poCsTrace, 
					m_pClientAPI, 
					m_poRegistry);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	iRet = m_poConnProvider->Init(m_pLDB);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización Ejecutor VM
	m_pVMCSContext	= new ClVMCSContextClient(m_poM4ObjServiceContext,
											m_poStatisticsWrap, 
											m_poConnProvider, 
											m_poExecAdap, 
											M4_TRUE /*Si es SM*/
											);
	iRet = m_pExecutor->Init(m_pVMCSContext);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización Adaptor de MetaDatos
	iRet = ((ClMDAdaptorImp*) m_poMDAdaptor)->Init(m_poRegistry, /* Registry inicializado */
					m_pClientAPI, 
                    m_poChannelManager,  /* DM inicializado */
					m_poCsTrace);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización Logon Adaptor
	iRet = ((ClLogonAdaptorImp*) m_pLogonAdaptor)->Init	(m_poChannelManager,
						m_poRegistry,
						m_poCsTrace,
						m_pClientAPI, //Solo necesario en DIC
						m_poCacheDirectory, //Solo necesario en DIC
						eM4CsTypeClientEmul);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	// Terminamos la inicialización del ProxyManager.
	m_pProxyManager->SetServerArch(m_pLogonAdaptor->GetServerArchitecture());

	m_pProxyManager->SetCaches(
						m_poCacheFactory->GetCMCRCache(),
						m_poCacheFactory->GetCSCRCache(),
						m_poCacheFactory->GetPresCache(),
						m_poCacheFactory->GetMapCache(),
						m_poCacheFactory->GetDataCache() ) ;

	m_pServerProxyManager->SetCaches(
						m_poCacheFactory->GetCMCRCache(),
						m_poCacheFactory->GetCSCRCache(),
						m_poCacheFactory->GetPresCache(),
						m_poCacheFactory->GetMapCache(),
						m_poCacheFactory->GetDataCache() ) ;

	return iRet;

}

/** Finaliza un ClM4ObjServiceClientSM.
 *
 * @return m4return_t.
 */
m4return_t ClM4ObjServiceClientSM::End() 
{
	m4return_t iRet = M4_SUCCESS;

	//Persistimos el CacheDir (Sólo en DIC y SM)
	if (m_poCacheDirectory) {
		m_poCacheDirectory->PersistCaches();
	}

    // Destruimos la cache de autos.
    ClObjDataCacheManager::Destroy();

	//////////////////////////////////
	// Finalizacion de objetos
	//////////////////////////////////
	if (m_poCsTrace) {
		if (M4_SUCCESS != m_poCsTrace->End()) {
			iRet = M4_ERROR;
		}
	}

	if (m_pLogonAdaptor) {
		if (M4_SUCCESS != ((ClLogonAdaptorImp*) m_pLogonAdaptor)->End()) {
			iRet = M4_ERROR;
		}
	}
	//((ClMDAdaptorImp*) m_poMDAdaptor)->End(); No lo tiene definido
	//m_pExecutor->End(); No lo tiene definido
	//m_poExecAdap->End(); No lo tiene definido
	if (m_poPresentationFac) {
		if (M4_SUCCESS != ((ClPresentationFactoryClient*)m_poPresentationFac)->End()){
			iRet = M4_ERROR;
		}
	}

	if (m_pLDB) {
		if (M4_SUCCESS != m_pLDB->End()) {
			iRet = M4_ERROR;
		}
	}

	//Eliminamos la parte cliente de la licencia de SM.
	if (m_pLicenseManager != NULL) {
		if (M4_SUCCESS != m_pLicenseManager->End()) {
			iRet = M4_ERROR;
		}
	}

	//ShutDown correcto en Single Machine con canales Proxy (11/5/01)
	// ----
	if (m_pProxyManager) {
		delete m_pProxyManager;
		m_pProxyManager = 0;
	}

	//El destructor del ClServerProxyManagerEmul ahora incorpora un bucle que destruye todos sus
	// espacios Proxys. La destrucción de su espacio "Proxy" desencadena la destrucción del ClM4ObjServiceServerSM, 
	// el cual fue creado al construir el primer ClproxySpace, al hacer la BuildFromId del primer canal Proxy.
	//El ClServerProxyManagerEmul::ReleaseSpace resetea el ChannelManager, pero no destruye el espacio Proxy, lo deja
	// por si se vuelve a crear un nuevo canal proxy. Este espacio proxy en el server en Single Machine nunca era liberado.
	if (m_pServerProxyManager) {
		delete m_pServerProxyManager;
		m_pServerProxyManager = 0;
	}
	// ----

	if (m_poChannelManager) {
		if (M4_SUCCESS != m_poChannelManager->End()) {
			iRet = M4_ERROR;
		}
	}
	if (m_poFactory) {
		if (M4_SUCCESS != ((ClFactoryClientEmul*)m_poFactory)->End()) {
			iRet = M4_ERROR;
		}
	}

	if (m_poCacheDirectory) {
		if (M4_SUCCESS != ((ClCacheDirectoryClient*) m_poCacheDirectory)->End()){
			iRet = M4_ERROR;
		}
	}

	if (m_pClientAPI) {
		if (M4_SUCCESS != m_pClientAPI->End()) {
			iRet = M4_ERROR;
		}
	}

	if (m_poCacheFactory) {
		if (M4_SUCCESS != m_poCacheFactory->DestroyCaches()) {
			iRet = M4_ERROR;
		}
	}

	// Se libera la máquina virtual de Java en el caso en el que haya sido
	// creada en ejecución.
	M4JVM_FreeJVM();
	//////////////////////////////////
	// Borrado de objetos
	//////////////////////////////////
	if (m_poRegistry) {
		delete m_poRegistry;
		m_poRegistry = 0;
	}
	if (m_pExecutor) {
		delete m_pExecutor;
		m_pExecutor = 0;
	}
	if (m_pLogonAdaptor) {
		delete m_pLogonAdaptor;
		m_pLogonAdaptor = 0;
	}
	if (m_pClientAPI) {
		delete m_pClientAPI;
		m_pClientAPI = 0;
	}
	if (m_poM4ObjServiceContext) {
		delete m_poM4ObjServiceContext;
		m_poM4ObjServiceContext = 0;
	}
	if (m_poFactory) {
		delete m_poFactory;
		m_poFactory = 0;
	}
	if (m_poChannelManager) {
		delete m_poChannelManager;
		m_poChannelManager = 0;
	}
	if (m_poStatisticsWrap) {
		delete m_poStatisticsWrap;
		m_poStatisticsWrap = 0;
	}
	if (m_poCsTrace) {
		delete m_poCsTrace;
		m_poCsTrace = 0;
	}
	if (m_poMDAdaptor) {
		delete m_poMDAdaptor;
		m_poMDAdaptor = 0;
	}
	if (m_poExecAdap) {
		delete m_poExecAdap;
		m_poExecAdap = 0;
	}
	if (m_poCacheDirectory) {
		delete m_poCacheDirectory;
		m_poCacheDirectory = 0;
	}
	if (m_poCacheFactory) {
		delete m_poCacheFactory;
		m_poCacheFactory = 0;
	}
	if (m_poPresentationFac) {
		delete m_poPresentationFac;
		m_poPresentationFac = 0;
	}
	if (m_poConnProvider) {
		delete m_poConnProvider;
		m_poConnProvider = 0;
	}
	if (m_poDMContext) {
		delete m_poDMContext;
		m_poDMContext = 0;
	}
	if (m_pVMCSContext) {
		delete m_pVMCSContext;
		m_pVMCSContext = 0;
	}
	if (m_pLDB) {
		delete m_pLDB;
		m_pLDB = 0;
	}
	if (m_pLicenseManager) {
		delete m_pLicenseManager;
		m_pLicenseManager = 0;
	}
	if (m_poLdbContext) {
		delete m_poLdbContext;
		m_poLdbContext = 0;
	}

	return iRet;
}



