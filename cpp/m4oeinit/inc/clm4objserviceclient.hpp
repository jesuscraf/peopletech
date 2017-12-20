//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objserviceclient.hpp
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
//    Este m�dulo contiene la Definici�n
//	  de la Maquina Virtual (M4Object & Data Services) DIC
//
//
//==============================================================================

//Esta definici�n es espec�fica para Client (DIC)

#ifndef _CLM4OBJSERVICECLIENT_HPP_
#define _CLM4OBJSERVICECLIENT_HPP_

#include "m4oeinit_dll.hpp"
#include "m4types.hpp"
#include "clm4objservice.hpp"

class ClAppClientAdap;
class ClMDAdaptor;
class ClFactM4ObjService;
class ClCache;
class ClClientProxyManager;


//================================================================
// Definici�n de la Maquina Virtual (M4Object & Data Services) DIC
//================================================================

class ClM4ObjServiceClient : public ClM4ObjService{

protected:
	ClMDAdaptor*			m_poMDAdaptor;

	/** ProxyManager que gestiona los ("el" porque en cliente s�lo hay uno) espacios proxy del lado cliente. */
	ClClientProxyManager*	m_pProxyManager;


	/** Crea un nuevo ClM4ObjServiceClient. S�lo puede crear objetos la factoria. */
	ClM4ObjServiceClient();

	/** Destructor. Permito derivaciones. */
	virtual ~ClM4ObjServiceClient() {}

	/**
	 * Inicializa un ClM4ObjServiceClient.
	 *
	 * @return m4return_t.
	 */
	m4return_t Init();

	/** Finaliza un ClM4ObjServiceClient.
	 *
	 * @return m4return_t.
	 */
	virtual m4return_t End();


	friend class ClFactM4ObjService; //Solo puede crear objetos la factoria.
};

#endif	// _CLM4OBJSERVICECLIENT_HPP_
