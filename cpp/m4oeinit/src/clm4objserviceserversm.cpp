//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objserviceserversm.cpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                06-04-2000
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

//Esta implementación es específica para Single Machine, para el lado servidor.

#include "clm4objserviceserversm.hpp"

//#include "clm4objservicecontextclient.hpp"	//Contexto cliente
#include "clm4objservicecontextserver.hpp"	//Contexto server
#include "cachefac.hpp"						//Factoria de caches
#include "presfaccli.hpp"					//Factoria de presentaciones server
#include "m4lgadapi.hpp"					//Logon Adaptor
#include "executor.hpp"						//VM Executor
#include "m4regimpl.hpp"					//RegServer
#include "m4mdfaci.hpp"						//Factoria de metadatos
#include "dm.hpp"							//ChannelManager
#include "sttwrap.hpp"						//ClVMStatisticsWrapper
#include "tablafuncdesc.hpp"				//ClFuncTableDesc
#include "tablatt.hpp"						//ClAtributeFuncTable
#include "cachedirsrv.hpp"					//ClCacheDirectoryServer
#include "m4cstrace.hpp"					//ClCsTrace
#include "m4trcfil.hpp"						//ClCsFileTrace
#include "cldmcontext.hpp"					//ClDMContextServer
#include "clexadap.hpp"						//ClExecAdaptor

#include "clldb_wrapper.hpp"				//ILdb, ClLdb_Wrapper
#include "conn_provider_server.hpp"			//ClLConn_Provider_Server

#include "clm4objservicecontextserver.hpp"	//Contexto server
#include "cachedirsrv.hpp"					//ClCacheDirectoryServer
#include "presfacsrv.hpp"					//Factoria de presentaciones server

#include "genproxy.hpp"						// Para el gestor de espacios proxys server.

#include "licensemanager.hpp"
#include "licres.hpp"

/** Crea un nuevo ClM4ObjServiceServerSM. Sólo puede crear objetos la factoria. */
ClM4ObjServiceServerSM::ClM4ObjServiceServerSM()
{}

/**
 * Inicializa un ClM4ObjServiceServerSM.
 *
 * @param ai_pLDB			Base de datos lógica.
 * @param ai_pRegistry		Objeto para acceder al registro. Se comparte con la parte cliente de SM.
 * @param ai_pLogonAdaptor	Adaptador para el logon. Se comparte con la parte cliente de SM.
 * @param ai_pConnProvider	Proveedor de conexiones. Se comparte con la parte cliente de SM.
 * @param ai_pCacheCMCR		Cache de metadatos. Puede compartirse, sino se la crea internamente.
 * @param ai_pCacheCSCR		Cache de seguridad. Puede compartirse, sino se la crea internamente.
 * @param ai_pCachePres		Cache de presentaciones. Puede compartirse, sino se la crea internamente.
 * @param ai_pCacheData		Cache de datos. Puede compartirse, sino se la crea internamente.
 * @return m4return_t.
 */
m4return_t ClM4ObjServiceServerSM::Init(
			ILdb* const ai_pLDB, 
			ILicenseManager * ai_pLicenseManager,
			ClVMBaseEnv* const ai_pRegistry, 
			ClLogonAdaptor* const ai_pLogonAdaptor, 
			ClLConn_Provider* const ai_pConnProvider, 
			ClCache* const ai_pCacheCMCR, 
			ClCache* const ai_pCacheCSCR, 
			ClCache* const ai_pCachePres, 
			ClCache* const ai_pCacheMap, 
			ClCache* const ai_pCacheData) 
{
	m4return_t iRet = M4_ERROR;

	//////////////////////////////////
	// Comprobamos y asignamos los parámetros obligatorios.
	//////////////////////////////////
	if (!ai_pRegistry) return M4_ERROR;
	m_poRegistry = ai_pRegistry;

	if (!ai_pLogonAdaptor) return M4_ERROR;
	m_pLogonAdaptor = ai_pLogonAdaptor;

	if (!ai_pConnProvider) return M4_ERROR;
	m_poConnProvider = ai_pConnProvider;

	// Nos guardamos las caches que nos han pasado.
	ClCache* pCacheCMCR = ai_pCacheCMCR;
	ClCache* pCacheCSCR = ai_pCacheCSCR;
	ClCache* pCachePres = ai_pCachePres;
	ClCache* pCacheMap  = ai_pCacheMap;
	ClCache* pCacheData = ai_pCacheData;


	//La LDB es la de la parte cliente que nos la pasan
	m_pLDB = ai_pLDB;

	//El cambio supone que tendremos un único ClLicenseManager de tipo server!.
	//Importante:
	//Cuando se crea un espacio Proxy en SM se crea un ClM4ObjServiceServerSM.
	//Solo podemos tener un ClLicenseManager tanto para cliente como para server!.
	m_pLicenseManager = ai_pLicenseManager;

	//////////////////////////////////
	// Creacion de objetos
	//////////////////////////////////
	m_pExecutor						= new ClExecutor(); 
	m_poM4ObjServiceContext			= new ClM4ObjServiceContextServer();
	m_poFactory						= new ClFactoryServerEmul();
	m_poChannelManager				= new ClChannelManager();
	m_poStatisticsWrap				= new ClVMStatisticsWrapper(M4_FALSE /* No es client*/);
	m_poCsTrace						= new ClCsTrace() ;		// Trazas vacias
	m_poPresentationFac				= new ClPresentationFactoryServer();
	m_poCacheDirectory				= new ClCacheDirectoryServer();


	//////////////////////////////////
	// Creación e inicialización de las caches.
	// Depende de si nos han pasado caches o no.
	//////////////////////////////////

	/*Siempre utilizo las caches que me pasan, si no viene ninguna, supongo que no se quiere
	utilizar caches*/
	/*if (!pCacheCMCR && !pCacheCSCR && !pCachePres && !pCacheData) {
		m_poCacheFactory	= new ClCacheFactory();

		//Inicialización de la Factoria de Caches
		iRet = m_poCacheFactory->CreateCaches();
		if (iRet != M4_SUCCESS) {
			End();
			return iRet;
		}

		pCacheCMCR = m_poCacheFactory->GetCMCRCache();
		pCacheCSCR = m_poCacheFactory->GetCSCRCache();
		pCachePres = m_poCacheFactory->GetPresCache();
		pCacheData = m_poCacheFactory->GetDataCache();
	}*/


	//////////////////////////////////
	// Inicializacion de objetos
	//////////////////////////////////

	//Inicialización de la traza
	iRet = m_poCsTrace->Init(m_poRegistry	/* Registry inicializado */ );
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización del Directorio de Caches
	iRet = ((ClCacheDirectoryServer*)m_poCacheDirectory)->Init(m_poRegistry, /* Registry inicializado */
															   m_poStatisticsWrap, 
															   m_poChannelManager);
	if (iRet != M4_SUCCESS) {
		End();
		return iRet;
	}

	//Inicialización de la Factoria de MetaDatos
	iRet = ((ClFactoryServerEmul*)m_poFactory)->Init(m_poChannelManager,	
													 m_poRegistry, 
													 m_pLogonAdaptor,		// Logon Adaptor.
													 pCacheCMCR,			// MDCache. La factoría tiene que estar inicializada.
													 pCacheCSCR,			// SecCache. La factoría tiene que estar inicializada.
													 pCacheMap,				// MapCache. La factoría tiene que estar inicializada.
													 m_poCacheDirectory,	// Tiene que estar inicializado.
													 m_poStatisticsWrap,
													 ClFuncTableDesc::GetInstance(),
													 ClAtributeFuncTable::GetInstance(),
													 m_poCsTrace
													 );
	if (iRet != M4_SUCCESS) {
		End();
		return iRet;
	}

	((ClFactoryServer*)m_poFactory)->SetLdb( ai_pLDB ) ;

	//Inicialización DM (Data Manager)
	m_poDMContext = new ClDMContextServer(M4_TRUE /* Es Single Machine */);
	iRet = m_poChannelManager->Init(m_poDMContext,
									m_poRegistry,
									m_poStatisticsWrap,
									m_poCacheDirectory,
									pCacheData,	// Data cache.
									m_poFactory,
									m_poPresentationFac,
									m_pExecutor,
									m_pLogonAdaptor,
									m_poCsTrace,
									0,			// No le pasamos proxy manager.
									m_pLicenseManager
									);
	if (iRet != M4_SUCCESS) {
		End();
		return iRet;
	}

	//Inicialización Presentaciones
	iRet = ((ClPresentationFactoryServer*)m_poPresentationFac)->Init(
						m_poRegistry,			// Registry inicializado.
					    m_poCsTrace, 
					    m_poStatisticsWrap, 
					    m_poChannelManager, 
					    m_poCacheDirectory,		// CacheDirectory inicializado.
					    pCachePres				// PresCache. La factoría tiene que estar inicializada.
						);
	if (iRet != M4_SUCCESS) {
		End();
		return iRet;
	}
	
	//Inicialización Ejecutor VM
	m_pVMCSContext = new ClVMCSContextServer(m_poM4ObjServiceContext,
											 m_poStatisticsWrap, 
											 m_poConnProvider, 
											 M4_TRUE /*Si es SM*/
											);

	iRet = m_pExecutor->Init(m_pVMCSContext);
	if (iRet != M4_SUCCESS) {
		End();
		return iRet;
	}


	//Sólo se inicializa una vez.
	if (ai_pLicenseManager->IsInitialized() == M4_FALSE) {
		//Es monothread (Pq es SM). Ya tenemos sesion, podemos hacer el init.
		iRet = m_pLicenseManager->Init( M4_FALSE, this, NULL );
		if (iRet != M4_SUCCESS) {
			DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
			End();
			return iRet;
		}
	}

	return iRet;
}

m4return_t ClM4ObjServiceServerSM::End() 
{
	m4return_t iRet = M4_SUCCESS;

	//////////////////////////////////
	// Finalizacion de objetos
	//////////////////////////////////

	if (m_poCsTrace) {
		if (M4_SUCCESS != m_poCsTrace->End()) {
			iRet = M4_ERROR;
		}
	}

	////La parte server de la SM tiene su propio LicenseManager, lo eliminamos.
	////Ahora No
	//if (m_pLicenseManager != NULL) {
	//	if (m_pLicenseManager->End() != M4_SUCCESS) {
	//		iRet = M4_ERROR;
	//	}
	//}

	//m_pExecutor->End(); No lo tiene definido
	//m_poExecAdap->End(); No lo tiene definido
	if (m_poPresentationFac) {
		if (M4_SUCCESS != ((ClPresentationFactoryServer*)m_poPresentationFac)->End()){
			iRet = M4_ERROR;
		}
	}

	if (m_poChannelManager) {
		if (M4_SUCCESS != m_poChannelManager->End()) {
			iRet = M4_ERROR;
		}
	}
	if (m_poFactory) {
		if (M4_SUCCESS != ((ClFactoryServerEmul*)m_poFactory)->End()) {
			iRet = M4_ERROR;
		}
	}

	if (m_poCacheDirectory) {
		if (M4_SUCCESS != ((ClCacheDirectoryServer*) m_poCacheDirectory)->End()){
			iRet = M4_ERROR;
		}
	}

	if (m_poCacheFactory) {
		if (M4_SUCCESS != m_poCacheFactory->DestroyCaches()) {
			iRet = M4_ERROR;
		}
	}

	//////////////////////////////////
	// Borrado de objetos
	//////////////////////////////////
	if (m_pExecutor) {
		delete m_pExecutor;
		m_pExecutor = 0;
	}
	if (m_poM4ObjServiceContext) {
		delete m_poM4ObjServiceContext;
		m_poM4ObjServiceContext = 0;
	}
	if (m_poFactory) {
		delete m_poFactory;
		m_poFactory = 0;
	}
	//if (m_pLicenseManager != NULL) {
	//	delete m_pLicenseManager;
	//	m_pLicenseManager = NULL;
	//}
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
	if (m_poDMContext) {
		delete m_poDMContext;
		m_poDMContext = 0;
	}
	if (m_pVMCSContext) {
		delete m_pVMCSContext;
		m_pVMCSContext = 0;
	}
	return iRet;
}

