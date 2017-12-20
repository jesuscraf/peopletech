//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objserviceserver.hpp
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
//	  de la Maquina Virtual (M4Object & Data Services) Server
//
//
//==============================================================================

//Esta definición es específica para Server

#ifndef _CLM4OBJSERVICESERVER_HPP_
#define _CLM4OBJSERVICESERVER_HPP_

#include "m4oeinit_dll.hpp"
#include "m4types.hpp"
#include "clm4objservice.hpp"

class ILdb;
class ClCache;
class ClFactM4ObjService;
class ILdbContext;
class ILicenseManager;


//================================================================
// Definición de la Maquina Virtual (M4Object & Data Services) Server
//================================================================

class ClM4ObjServiceServer : public ClM4ObjService{

protected:
	ClM4ObjServiceServer(); //Solo puede crear objetos la factoria.
	virtual ~ClM4ObjServiceServer() {}
	virtual m4return_t Init(ILdb *ai_pLDB, ILicenseManager * ai_pLicenseManager, ClCache *ai_pCacheCMCR, ClCache *ai_pCacheCSCR, ClCache *ai_pCachePres, ClCache *ai_pCacheMap, ClCache *ai_pCacheData);
	virtual m4return_t End();
	virtual m4return_t SetParams(m4bool_t ai_bLDBOwner);

	ILdbContext					*m_poLdbContext;

	m4bool_t					m_bLDBOwner;

	friend class ClFactM4ObjService; //Para que pueda crear objetos.
};

#endif	// _CLM4OBJSERVICECLIENT_HPP_
