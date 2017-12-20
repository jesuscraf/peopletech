//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objserviceserversm.hpp
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
//    Este módulo contiene la Definición
//	  de la Maquina Virtual (M4Object & Data Services) SM para el lado servidor.
//
//
//==============================================================================

//Esta definición es específica para SM Server(Single Machine)

#ifndef _CLM4OBJSERVICESERVERSM_HPP_
#define _CLM4OBJSERVICESERVERSM_HPP_

#include "m4oeinit_dll.hpp"
#include "m4types.hpp"
#include "clm4objservice.hpp"

class ClFactM4ObjService;
class ClVMBaseEnv;
class ClLogonAdaptor;
class ClLConn_Provider;
class ILicenseManager;

//================================================================
// Definición de la Maquina Virtual (M4Object & Data Services) SM
//================================================================

class ClM4ObjServiceServerSM : public ClM4ObjService {

protected:
	/** Crea un nuevo ClM4ObjServiceClientSM. Sólo puede crear objetos la factoria. */
	ClM4ObjServiceServerSM();

	/** Destructor. Permito derivaciones. */
	virtual ~ClM4ObjServiceServerSM() {}

	/**
	 * Inicializa un ClM4ObjServiceServerSM.
	 *
	 * @param ai_pLDB			Base de datos lógica.
	 * @param ai_pRegistry		Objeto para acceder al registro. Se comparte con la parte clieten de SM.
	 * @param ai_pLogonAdaptor	Adaptador para el logon. Se comparte con la parte clieten de SM.
	 * @param ai_pConnProvider	Proveedor de conexiones. Se comparte con la parte clieten de SM.
	 * @param ai_pCacheCMCR		Cache de metadatos. Puede compartirse, sino se la crea internamente.
	 * @param ai_pCacheCSCR		Cache de seguridad. Puede compartirse, sino se la crea internamente.
	 * @param ai_pCachePres		Cache de presentaciones. Puede compartirse, sino se la crea internamente.
	 * @param ai_pCacheData		Cache de datos. Puede compartirse, sino se la crea internamente.
	 * @return m4return_t.
	 */
	m4return_t Init(ILdb* const ai_pLDB, ILicenseManager * ai_pLicenseManager, ClVMBaseEnv* const ai_pRegistry, ClLogonAdaptor* const ai_pLogonAdaptor, ClLConn_Provider* const ai_pConnProvider, ClCache* const ai_pCacheCMCR, ClCache* const ai_pCacheCSCR, ClCache* const ai_pCachePres, ClCache* const ai_pCacheMap, ClCache* const ai_pCacheData);

	/** Finaliza un ClM4ObjServiceServerSM.
	 *
	 * @return m4return_t.
	 */
	virtual m4return_t End();


	friend class ClFactM4ObjService;
};

#endif	// _CLM4OBJSERVICESERVERSM_HPP_
