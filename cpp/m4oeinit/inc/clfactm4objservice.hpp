//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clfactm4objservice.hpp
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
//    Este módulo contiene la definición de 
//	  la Factoria de Maquinas Virtuales
//
//
//==============================================================================

//Esta definición es comun para Client, Server y SingleMachine

#ifndef _CLFACTM4OBJSERVICE_HPP_
#define _CLFACTM4OBJSERVICE_HPP_

#include "m4oeinit_dll.hpp"
#include "m4stl.hpp"
#include "m4ipcs.hpp"	//m4ipcs

class ClM4ObjService;
class ClCache;
class IExecutor;
class ILdb;



/**
 * Esta clase implementa: 
 * - Factoría de M4ObjServices (VM+DM[+LDB])
 * - Factoría de Ejecutores
 * - Obtención de LDB
 * - Información del tipo de configuracion
 * En un futuro podría implementar otras factorías.
 */
class M4_DECL_M4OEINIT ClFactM4ObjService {
public:
	/** Instacia unica del Singletton */
	static ClFactM4ObjService* GetInstance();

	static ClM4ObjService* GetMasterObjService()
	{
		return m_pM4ObjServiceMaster;
	}

	/**
	 *	Factoria de M4ObjServices: Implementacion distinta en SM, Client y Server
	 */
	static ClM4ObjService* CreateObjService(
									ClCache *ai_pCacheCMCR,
									ClCache *ai_pCacheCSCR,
									ClCache *ai_pCachePres,
									ClCache *ai_pCacheMap,
									ClCache *ai_pCacheData,
									m4bool_t ai_bServer
									);

	static m4return_t	DestroyObjService(ClM4ObjService*& aio_pM4ObjService);

	/**
	 *	Factoria de Ejecutores: Implementacion distinta en SM, Client y Server
	 */
	static IExecutor*	CreateExecutor();
	static m4return_t	DestroyExecutor(IExecutor* aio_pExecutor);

	/**
	 * Obtener la LDB
	 */
	static ILdb*		GetpLDB();

	/**
	 * Tipos de Inicializador (DLL). Indica la configuracion del entorno.
	 * - M4ObjClient = DLL DIC o DLL SM Conectada
	 * - M4ObjServer = DLL Server
	 * - M4ObjSM	 = DLL SM Emulacion (No Conectada)
	 */
	enum eM4ObjConfig {M4ObjUndefType=0, M4ObjClientType, M4ObjServerType, M4ObjEmulType};

	/**
	 * Obtener el tipo de Configuración del entorno.
	 */
	static eM4ObjConfig GetM4ObjConfig();

private:
	ClFactM4ObjService() {}		// Singletton

	/**
	 * Se crea la LDB.
	 * Solo implementada en Server para el subsystema de LDB
	 * en DIC y SM devuelve M4_ERROR y se creará cuando se necesite
	 */
	static m4return_t CreateLDB(ClCache *ai_pCacheCMCR,
								ClCache *ai_pCacheCSCR,
								ClCache *ai_pCachePres,
								ClCache *ai_pCacheMap,
								ClCache *ai_pCacheData);

	static ClMutex				m_oMutex;	// Mutex para sincronizacion interna
	static ClFactM4ObjService	*m_pFactM4ObjService;	// Singletton

	/**
	 *
	 *	Representa el primer ObjectService creado.
	 *	En Server:	Es el ObjectService que crea la LDB.
	 *	En SM:		Es el ObjectService creado en la parte cliente de SM
	 *	
	 *	En Server, DIC y SM, a partir de él se clonan los nuevos ejecutores 
	 *	en el método CreateExecutor.
	 */
	static ClM4ObjService		*m_pM4ObjServiceMaster; 

	/** 
	 *  Este puntero solo se utiliza en SM porque hay que guardar 2 contextos que se les pasa
	 *  a los ejecutores creados con CreateExecutor en SM. Guardará el ObjectService Server
	 */
	static ClM4ObjService		*m_pM4ObjServiceMasterSMServer; 
	
	/** No. de Instancias de ObjServices creados por la factoría */
	static m4uint32_t			m_NumM4ObjServices;

	
	/**
	 *  Handler to NP changes
	 */
	static m4void_t NPEventHandler(m4int32_t ai_iState);

};

#endif	// _CLFACTM4OBJSERVICE_HPP_
