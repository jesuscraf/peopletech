//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clfactm4objservice.cpp   
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

//Esta implementación es la parte comun para Client, Server y SingleMachine

#include "clfactm4objservice.hpp"
#include "clm4objservice.hpp"

// Inicializacion de miembros static
ClFactM4ObjService *	ClFactM4ObjService::m_pFactM4ObjService = 0;
ClMutex					ClFactM4ObjService::m_oMutex(M4_TRUE);
ClM4ObjService*			ClFactM4ObjService::m_pM4ObjServiceMaster = 0; 
ClM4ObjService*			ClFactM4ObjService::m_pM4ObjServiceMasterSMServer = 0; 
m4uint32_t				ClFactM4ObjService::m_NumM4ObjServices = 0;

// Singletton
ClFactM4ObjService*  ClFactM4ObjService::GetInstance() {
	if (!m_pFactM4ObjService) {
		m_oMutex.Lock();
		if (!m_pFactM4ObjService) {
			m_pFactM4ObjService = new ClFactM4ObjService();
		}
		m_oMutex.Unlock();
	}
	return m_pFactM4ObjService;
}


