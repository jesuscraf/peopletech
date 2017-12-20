//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clfactm4objserviceclient.cpp   
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
//    Este módulo contiene la implementación de 
//	  la Factoria de Maquinas Virtuales
//
//
//==============================================================================

//Esta implementación es la parte específica para DIC

#include "clfactm4objservice.hpp"
#include "clm4objserviceclient.hpp"
#include "m4objreg.hpp"
#include "m4log.hpp"
#include "cldefine.hpp"
#include "executor.hpp"
#include "m4filemanager.hpp"
#include "m4logsys.hpp"

#include "blocksynchronization.hpp"

/**
 * Obtener el tipo de Configuración del entorno. => Client
 */
ClFactM4ObjService::eM4ObjConfig ClFactM4ObjService::GetM4ObjConfig()
{
	return ClFactM4ObjService::M4ObjClientType;
}


// Creación de VM DIC
ClM4ObjService* ClFactM4ObjService::CreateObjService(
									ClCache *ai_pCacheCMCR,
									ClCache *ai_pCacheCSCR,
									ClCache *ai_pCachePres,
									ClCache *ai_pCacheMap,
									ClCache *ai_pCacheData,
									m4bool_t ai_bServer)
{
	// Seccion crítica
	ClMutBlock oMutex(m_oMutex);

	ClM4ObjService* pM4ObjService = 0;

	if (0 == m_NumM4ObjServices) {
		//Inicializamos la M4Log
		m4char_t    acWorkingDirectory[ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;
		m4char_t    acTempBuffer[ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;
		if (ClVMBaseEnv::ReadWorkingDirectory(M4_NEW_REGEDIT_KEY_CLIENT, acWorkingDirectory, M4CH_MAX_WORKING_DIRECTORY + 1, M4_FALSE) == M4_ERROR) {
			MessageBox( NULL, "Could not read working directory.", "InitError", MB_OK ) ;
			strcpy (acWorkingDirectory, "");
			sprintf( acTempBuffer, "%s", M4CH_LOGSYS_FILE ) ;
		}
		else {
			sprintf( acTempBuffer, "%s%s%s", acWorkingDirectory, M4_BACK_SLASH, M4CH_LOGSYS_FILE ) ;
		}
		INITIALIZELOGMANAGER(acTempBuffer);

		//Inicializamos la M4LogSys
		LogsysSetWorkingDir( acWorkingDirectory ) ;
	}
	
	pM4ObjService = new ClM4ObjServiceClient();
	if (pM4ObjService) {
		if (M4_SUCCESS == ((ClM4ObjServiceClient*)pM4ObjService)->Init()) {
			if (0 == m_pM4ObjServiceMaster) {
				m_pM4ObjServiceMaster = pM4ObjService;
				// Inits the file manager default temp dir 
				m4return_t iResult = ClFileManager::Instance()->SetDefaultDirectory( (char*)m_pM4ObjServiceMaster->GetpRegistry()->GetTempDirectory() );
				//if( iResult != M4_SUCCESS ) {
				//	DUMP_CHLOG_ERROR( M4_CH_MCR_ERR_SET_PDU_DEF_PATH ) ;
				//	return( M4_ERROR ) ;
				//}
			}
			m_NumM4ObjServices++;
		}
		else {
			delete pM4ObjService;
			pM4ObjService = 0;
		}
	}

	return pM4ObjService;
}

m4return_t ClFactM4ObjService::DestroyObjService(ClM4ObjService*& aio_pM4ObjService) {
	m4return_t iRet = M4_ERROR;
	if (aio_pM4ObjService && m_NumM4ObjServices) {
		//Si hemos creado alguna instancia podemos borrar

		// Seccion crítica
		ClMutBlock oMutex(m_oMutex);

		m_NumM4ObjServices--;

		if (aio_pM4ObjService != m_pM4ObjServiceMaster) {
			//No es el objservice master, luego podemos liquidarlo sin problemas
			iRet = aio_pM4ObjService->End();
			delete aio_pM4ObjService;
		}
		if (0 == m_NumM4ObjServices) {
			//Como ya no quedan referencias nos podemos cargar también el master
			iRet = m_pM4ObjServiceMaster->End();
			delete m_pM4ObjServiceMaster;
			m_pM4ObjServiceMaster = 0;
			DESTROYLOGMANAGER();

			/* Bug 0077666
			Hay que ir cerrando el thread de eventos de fichero
			*/
			ClFileEventBroker::EndThread() ;
		}
		if (iRet == M4_SUCCESS) {
			aio_pM4ObjService = 0;
		}
	}
	return iRet;
}


/* Creamos un nuevo ejecutor a partir del objservice master, si no existe todavia
devolvemos un error */
IExecutor* ClFactM4ObjService::CreateExecutor() {
	ClMutBlock oMutex(m_oMutex);

	if (!m_pM4ObjServiceMaster)
		return 0; //No tenemos master

	IExecutor *pExecutor = new ClExecutor();

	m4return_t iRet;
	//Le pasamos el contexto del master
	iRet = pExecutor->Init(m_pM4ObjServiceMaster->m_pVMCSContext);
	if (M4_SUCCESS != iRet) {
		delete pExecutor;
		pExecutor = 0;
	}
	else
		m_NumM4ObjServices++;

	return pExecutor;
}

/*Destruimos un ejecutor y disminuimos el número de referencias*/
m4return_t ClFactM4ObjService::DestroyExecutor(IExecutor* aio_pExecutor) {
	if (0  != aio_pExecutor && m_NumM4ObjServices) {
		//Si hemos creado alguna instancia podemos borrar
		return M4_ERROR;
	}

	//aio_pExecutor->End(); No tiene end
	delete aio_pExecutor;
	//aio_pExecutor = 0;
	m_oMutex.Lock();
	m_NumM4ObjServices--;
	m_oMutex.Unlock();
	return M4_SUCCESS;
}

m4return_t ClFactM4ObjService::CreateLDB(ClCache *ai_pCacheCMCR,
								ClCache *ai_pCacheCSCR,
								ClCache *ai_pCachePres,
								ClCache *ai_pCacheMap,
								ClCache *ai_pCacheData) {
	return M4_ERROR;
}

ILdb*	ClFactM4ObjService::GetpLDB() {
	ClMutBlock oMutex(m_oMutex);
	if (m_pM4ObjServiceMaster) 
		return m_pM4ObjServiceMaster->GetpLDB();
	else
		return 0;
}

m4void_t ClFactM4ObjService::NPEventHandler(m4int32_t ai_iState)
{}
