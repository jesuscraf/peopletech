//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objserviceclientsm.hpp
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
//    Este módulo contiene la Definición
//	  de la Maquina Virtual (M4Object & Data Services) SM
//
//
//==============================================================================

//Esta definición es específica para SM (Single Machine)

#ifndef _CLM4OBJSERVICECLIENTSM_HPP_
#define _CLM4OBJSERVICECLIENTSM_HPP_

#include "m4oeinit_dll.hpp"
#include "m4types.hpp"
#include "clm4objserviceclient.hpp"

class ILdb;
class ILdbContext;
class ClFactM4ObjService;
class ClAppClientAdap;
class ClMDAdaptor;
class ClServerProxyManagerEmul;

//================================================================
// Definición de la Maquina Virtual (M4Object & Data Services) SM
//================================================================

class ClM4ObjServiceClientSM : public ClM4ObjServiceClient {
public:
	/** Nos da acceso al gestor de espacios proxy del lado servidor. */
	ClServerProxyManagerEmul* const GetpServerProxyManagerEmu() { return m_pServerProxyManager; }


protected:
	ILdbContext*				m_poLdbContext;

	/** Gestor de espacios proxy para el lado servidor cuando estamos en emulación (SM). */
	ClServerProxyManagerEmul*	m_pServerProxyManager;


	/** Crea un nuevo ClM4ObjServiceClientSM. Sólo puede crear objetos la factoria. */
	ClM4ObjServiceClientSM();

	/** Destructor. Permito derivaciones. */
	virtual ~ClM4ObjServiceClientSM() {}

	/**
	 * Inicializa un ClM4ObjServiceClientSM. Sólo puede inicializar objetos la factoria. 
	 *
	 * @return m4return_t.
	 */
	m4return_t Init();

	/** Finaliza un ClM4ObjServiceClientSM. Sólo puede finalizar objetos la factoria. 
	 *
	 * @return m4return_t.
	 */
	virtual m4return_t End();


	friend class ClFactM4ObjService;
};

#endif	// _CLM4OBJSERVICECLIENTSM_HPP_
