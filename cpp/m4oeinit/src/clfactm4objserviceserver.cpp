 //==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clfactm4objserviceserver.cpp   
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

//Esta implementación es la parte específica para Server

#include "clfactm4objservice.hpp"
#include "clm4objserviceserver.hpp"
#include "executor.hpp"
#include "m4log.hpp"
#include "m4objreg.hpp"
#include "m4logsys.hpp"
#include "m4props.hpp"
#include "m4servenv.hpp"
#include "licensemanager.hpp"
#include "licensemanagerthread.hpp"
#include "licres.hpp"
#include "chlog.hpp"

#include "blocksynchronization.hpp"

/**
 * Obtener el tipo de Configuración del entorno. => Server
 */
ClFactM4ObjService::eM4ObjConfig ClFactM4ObjService::GetM4ObjConfig()
{
	return ClFactM4ObjService::M4ObjServerType;	
}


// Creación de VM Server
ClM4ObjService *ClFactM4ObjService::CreateObjService(
									ClCache *ai_pCacheCMCR,
									ClCache *ai_pCacheCSCR,
									ClCache *ai_pCachePres,
									ClCache *ai_pCacheMap,
									ClCache *ai_pCacheData,
									m4bool_t ai_bServer)
{
	ClMutBlock oMutex(m_oMutex); //Bloqueamos porque se puede crear la LDB

	ClM4ObjService* pM4ObjService = 0;

	if (0 == m_NumM4ObjServices) {
		//Inicializamos la M4Log
		m4char_t    acWorkingDirectory[ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;
		m4char_t    acTempBuffer[ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;
		if (ClVMBaseEnv::ReadWorkingDirectory(M4_NEW_REGEDIT_KEY_SERVER, acWorkingDirectory, M4CH_MAX_WORKING_DIRECTORY + 1, M4_FALSE) == M4_ERROR) {
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

	if (CreateLDB(NULL, NULL, NULL, NULL, NULL) == M4_SUCCESS){

		pM4ObjService = new ClM4ObjServiceServer();
		ClM4ObjServiceServer *pM4ObjServiceServerTemp = (ClM4ObjServiceServer *) pM4ObjService;

		if (pM4ObjServiceServerTemp->Init(m_pM4ObjServiceMaster->GetpLDB(), m_pM4ObjServiceMaster->GetpLicenseManager(), ai_pCacheCMCR, ai_pCacheCSCR, ai_pCachePres, ai_pCacheMap, ai_pCacheData) == M4_SUCCESS) {
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
			m_pM4ObjServiceMaster->End();
			delete m_pM4ObjServiceMaster;
			m_pM4ObjServiceMaster = 0;
			DESTROYLOGMANAGER();
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

	//Le pasamos el contexto del master
	m4return_t iRet = pExecutor->Init(m_pM4ObjServiceMaster->m_pVMCSContext);
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
	
	m4return_t iRet = M4_SUCCESS;

	if (!m_pM4ObjServiceMaster) {

		//Creamos un objservice para cachearnos la LDB
		m_pM4ObjServiceMaster = new ClM4ObjServiceServer();

		ClM4ObjServiceServer *pM4ObjServiceServer = (ClM4ObjServiceServer *) m_pM4ObjServiceMaster;
		pM4ObjServiceServer->SetParams(M4_TRUE); //Indicamos que se cree la LDB
		if (pM4ObjServiceServer->Init(0, NULL, ai_pCacheCMCR, ai_pCacheCSCR, ai_pCachePres, ai_pCacheMap, ai_pCacheData) != M4_SUCCESS) {
			delete m_pM4ObjServiceMaster;
			m_pM4ObjServiceMaster = 0;
			return M4_ERROR;
		}

		//Reutilizamos el mecanismo de BDL para la licencia.
		//Reseteamos el flag que indica la propidad de BDL.		
		ClM4ObjServiceServer * pM4ObjServiceLicense = new ClM4ObjServiceServer();
		pM4ObjServiceLicense->SetParams(M4_FALSE);
		if (pM4ObjServiceLicense->Init(pM4ObjServiceServer->GetpLDB(), pM4ObjServiceServer->GetpLicenseManager(), ai_pCacheCMCR, ai_pCacheCSCR, ai_pCachePres, ai_pCacheMap, ai_pCacheData) != M4_SUCCESS) {
			delete pM4ObjServiceLicense;
			pM4ObjServiceLicense = NULL;
			return M4_ERROR;
		}
		
		ILicenseManager * pLicenseManager = pM4ObjServiceLicense->GetpLicenseManager();
		if (pLicenseManager != NULL) {

			// initializes the internal state to RED
			NPEventHandler(STATE_VALIDATE_EXECUTION_NP_RED);

			//Con el primer argumento indicamos que es multithread!.
			ClExecuteNPNotifier *poNotifier = new ClExecuteNPNotifier(&NPEventHandler);

			iRet = pLicenseManager->Init(M4_TRUE, pM4ObjServiceLicense, poNotifier);
			if (M4_SUCCESS != iRet) {
				DUMP_CHLOG_ERROR( M4_CH_LICENSE_INCORRECT );
				return M4_ERROR;
			}
		}
	}
	return iRet;
}

m4void_t ClFactM4ObjService::NPEventHandler(m4int32_t ai_iState)
{
	m4char_t sStateValidateExecutionNp[2];
	sprintf(sStateValidateExecutionNp, "%d", ai_iState);
	SetEnviron("SERVER", "LICENSE_STATE_VALIDATE_EXECUTION_NP", sStateValidateExecutionNp, M4_FALSE );			
}


ILdb*	ClFactM4ObjService::GetpLDB() {	
	ClMutBlock oMutex(m_oMutex);
	
	if (CreateLDB(NULL, NULL, NULL, NULL, NULL) == M4_SUCCESS) {
		return m_pM4ObjServiceMaster->GetpLDB();
	}
	else
		return 0;
}
