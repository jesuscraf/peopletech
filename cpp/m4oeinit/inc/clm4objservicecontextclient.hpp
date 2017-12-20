//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objservicecontextclient.hpp
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
//    Este módulo contiene la Definición del Contexto
//	  de la Maquina Virtual Client (DIC y SM)
//
//
//==============================================================================

//Esta definición es específica para Client (DIC y SM)

#ifndef _CLM4OBJSERVICECONTEXTCLIENT_
#define _CLM4OBJSERVICECONTEXTCLIENT_

#include "clm4objservicecontextimpl.hpp"

#ifdef _WINDOWS
	#include "windows.h"
#endif

class ClAppClientAdap;


class ClM4ObjServiceContextClient : public ClM4ObjServiceContextImpl
{

public:
	ClM4ObjServiceContextClient(ClAppClientAdap *ai_pClientAPI);

	void		EnableTimeOut(m4bool_t ai_bEnable = M4_TRUE);
	m4bool_t	GetTimeOutState() { return m_bEnableTimeOut; }
	m4return_t	SetCSTimeoutFunction (m4pcchar_t ai_pcDLLName=0, m4pcchar_t ai_pcFuncName=0);
	void		NotifyVMExecution(m4bool_t ai_bIsStart);

protected:
		// timeout window
	m4bool_t m_bEnableTimeOut;
	ClAppClientAdap *m_poClientAPI;

	// timeout window handlers
#ifdef _WINDOWS
	HINSTANCE m_hCSTimeoutLibrary;
	FARPROC m_hCSTimeoutProcedure;
	FARPROC m_hCSServerEventProcedure;
#endif

};



#endif	// _CLM4OBJSERVICECONTEXTCLIENT_

