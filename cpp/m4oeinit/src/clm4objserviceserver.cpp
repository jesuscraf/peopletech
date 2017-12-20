//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objserviceserver.cpp
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

//Esta implementación es específica para Server

#include "clm4objserviceserver.hpp"

#include "clm4objservicecontextserver.hpp"	//Contexto cliente
#include "cachefac.hpp"						//Factoria de caches
#include "presfacsrv.hpp"					//Factoria de presentaciones server
#include "m4lgadapi.hpp"					//Logon Adaptor
#include "conn_provider.hpp"				//Gestor de conexiones
#include "executor.hpp"						//VM Executor
#include "m4regimpl.hpp"					//RegServer
#include "m4mdfaci.hpp"						//Factoria de metadatos
#include "dm.hpp"							//ChannelManager
#include "sttwrap.hpp"						//ClVMStatisticsWrapper
#include "tablafuncdesc.hpp"				//ClFuncTableDesc
#include "tablatt.hpp"						//ClAtributeFuncTable
#include "m4mdadapimp.hpp"					//ClMDAdaptorImp
#include "cachedirsrv.hpp"					//ClCacheDirectoryServer
#include "m4cstrace.hpp"					//ClCsTrace
#include "m4trcfil.hpp"						//ClCsFileTrace
#include "cldmcontext.hpp"					//ClDMContextServer
#include "m4ldb.hpp"						//ILdb
#include "conn_provider_server.hpp"			//ClLConn_Provider_Server
#include "ldbcontext.hpp"					//ClLdbContextServer
#include "clldb_wrapper.hpp"				//ILdb, ClLdb_Wrapper
#include "licensemanager.hpp"
#include "m4props.hpp"
#include "m4servenv.hpp"


ClM4ObjServiceServer::ClM4ObjServiceServer() 
:	m_poLdbContext(0), m_bLDBOwner(M4_FALSE)
{}

m4return_t ClM4ObjServiceServer::SetParams(m4bool_t ai_bLDBOwner) {
	//Indicamos si tenemos que crear/destruir la ldb
	m_bLDBOwner = ai_bLDBOwner;
	return M4_SUCCESS;
}
m4return_t ClM4ObjServiceServer::Init(ILdb *ai_pLDB, ILicenseManager * ai_pLicenseManager, ClCache *ai_pCacheCMCR, ClCache *ai_pCacheCSCR, ClCache *ai_pCachePres, ClCache *ai_pCacheMap, ClCache *ai_pCacheData) {

	m4return_t iRet = M4_SUCCESS;

	////////////////////////////////////////////
	// Creacion e Inicializacion de Registry
	////////////////////////////////////////////
	m_poRegistry = new ClM4ObjRegServer();

	iRet = m_poRegistry->Init();
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}


	// Nos guardamos las caches que nos han pasado.
	ClCache* pCacheCMCR = ai_pCacheCMCR;
	ClCache* pCacheCSCR = ai_pCacheCSCR;
	ClCache* pCachePres = ai_pCachePres;
	ClCache* pCacheMap  = ai_pCacheMap;
	ClCache* pCacheData = ai_pCacheData;

	//////////////////////////////////
	// Creacion de objetos
	//////////////////////////////////
	m_pExecutor								= new ClExecutor(); 

	m_pLogonAdaptor							= new ClLogonAdaptorImp();

	m_poM4ObjServiceContext					= new ClM4ObjServiceContextServer();
	
	m_poFactory								= new ClFactoryServer();

	m_poChannelManager						= new ClChannelManager();

	m_poStatisticsWrap						= new ClVMStatisticsWrapper(M4_FALSE /* Es server */);

	ClFuncTableDesc	*poFunctionTable		= ClFuncTableDesc::GetInstance();

	ClAtributeFuncTable	*poAtributeTable	= ClAtributeFuncTable::GetInstance();

	m_poCsTrace								= new ClCsTrace() ;		// Trazas vacias

	m_poCacheDirectory						= new ClCacheDirectoryServer();
		
	m_poPresentationFac						= new ClPresentationFactoryServer();

	m_poConnProvider						= new ClLConn_Provider_Server();	/* Gestor de Conexiones */

	//Reutilizamos el mecanismo que tiene la BDL para obtener la licencia.
	if (m_bLDBOwner == M4_FALSE)
		m_pLicenseManager = ai_pLicenseManager;
	else {
		m_pLicenseManager = new ClLicenseManagerServer();
	}

	//////////////////////////////////
	// Creación e inicialización de las caches.
	// Depende de si nos han pasado caches o no.
	//////////////////////////////////

	/*Siempre utilizo las caches que me pasan, si no viene ninguna, supongo que no se quiere
	utilizar caches*/
	/*if (!pCacheCMCR || !pCacheCSCR || !pCachePres || !pCacheData) {
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
	iRet = ((ClCacheDirectoryServer*) m_poCacheDirectory)->Init(m_poRegistry, /* Registry inicializado */
						m_poStatisticsWrap, 
						m_poChannelManager);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización de la Factoria de MetaDatos
	//Logon ¿¿?¿?¿?¿
	iRet = ((ClFactoryServer*)m_poFactory)->Init	(	
										m_poChannelManager,	
										m_poRegistry, 
										m_pLogonAdaptor, //Logon Adaptor
										pCacheCMCR, /*MDCache*/
										pCacheCSCR, /*SecCache*/
										pCacheMap, /*MapCache*/
										m_poCacheDirectory,				/*Tiene que estar inicializado*/
										m_poStatisticsWrap,
										poFunctionTable,
										poAtributeTable,
										m_poCsTrace) ;

	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}


	//Inicialización DM (Data Manager)
	m_poDMContext = new ClDMContextServer( M4_FALSE /* No es Single Machine */);
	iRet = m_poChannelManager->Init(
		m_poDMContext,
		m_poRegistry,
		m_poStatisticsWrap,
		m_poCacheDirectory,
		pCacheData,   //Data cache
		m_poFactory,
		m_poPresentationFac,
		m_pExecutor,
		m_pLogonAdaptor,
		m_poCsTrace,
		0,			// No le pasamos proxy manager.
		m_pLicenseManager
		);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización Presentaciones
	iRet = ((ClPresentationFactoryServer*)m_poPresentationFac)->Init(
						m_poRegistry, /* Registry inicializado */
					    m_poCsTrace, 
					    m_poStatisticsWrap, 
					    m_poChannelManager, 
					    m_poCacheDirectory, /* CacheDirectory inicializado */
					    pCachePres /*PresCache*/ /*La factoria tiene que estar inicializada*/
						);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//////////////////////////////////
	// LDB si no se pasa como parámetro hay que crearla
	//////////////////////////////////
	if (m_bLDBOwner == M4_FALSE)
	{
		m_pLDB = ai_pLDB;
	}
	else
	{
		//Creo e inicializo la LDB
		m_pLDB			= new ClLdb_Wrapper();
		m_poLdbContext	= new ClLdbContextServer();
		iRet = m_pLDB->Init(m_poRegistry,
					  m_poFactory,
					  m_poLdbContext);
		if (M4_SUCCESS != iRet) {
			End();
			return M4_ERROR;
		}
	}

	((ClFactoryServer*)m_poFactory)->SetLdb( ai_pLDB ) ;


	iRet = m_poConnProvider->Init( (ClLdb_Wrapper*)m_pLDB);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}
	
	//Inicialización Ejecutor VM
	m_pVMCSContext	= new ClVMCSContextServer(m_poM4ObjServiceContext,
										m_poStatisticsWrap, 
										m_poConnProvider, 
										M4_FALSE /*No es SM*/
										);

	iRet = m_pExecutor->Init(m_pVMCSContext);
	if (M4_SUCCESS != iRet) {
		End();
		return M4_ERROR;
	}

	//Inicialización Logon Adaptor
	iRet = ((ClLogonAdaptorImp*)m_pLogonAdaptor)->Init	(m_poChannelManager,
						m_poRegistry,
						m_poCsTrace,
						NULL, //ClientApi Solo necesario en DIC
						m_poCacheDirectory, //CacheDirectory Solo necesario en DIC
						eM4CsTypeServer);

	//Activación de las cachés
	iRet = ((ClCacheDirectoryServer*) m_poCacheDirectory)->ActivateCaches();

	return iRet;
}

m4return_t ClM4ObjServiceServer::End() {

	m4return_t iRet = M4_SUCCESS;

	//////////////////////////////////
	// Finalizacion de objetos
	//////////////////////////////////
	if (m_poCsTrace) {
		if (M4_SUCCESS != m_poCsTrace->End()) {
			iRet = M4_ERROR;
		}
	}

	if (m_pLogonAdaptor) {
		if (M4_SUCCESS != ((ClLogonAdaptorImp*)m_pLogonAdaptor)->End()) {
			iRet = M4_ERROR;
		}
	}
	//m_pExecutor->End(); No lo tiene definido

	if (m_bLDBOwner == M4_TRUE) {
		if (m_pLDB) {
			m_pLDB->End();
			if (m_pLDB) {
				delete m_pLDB;
				m_pLDB = 0;
			}
		}
		if (m_poLdbContext) {
			delete m_poLdbContext;
			m_poLdbContext = 0;
		}
	}

	//Reutilizamos el mecanismo de LDB.
	if (m_bLDBOwner == M4_TRUE) {
		if (m_pLicenseManager != NULL) {
			m_pLicenseManager->End();
			if (m_pLicenseManager) {
				delete m_pLicenseManager;
				m_pLicenseManager = NULL;
			}
		}
	}

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
		if (M4_SUCCESS != ((ClFactoryServer*)m_poFactory)->End()) {
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

	if (m_poConnProvider) {
		if (M4_SUCCESS != m_poConnProvider->End()) {
			iRet = M4_ERROR;
		}
	}
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
	return iRet;
}

