//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objservicecontextclient.cpp
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
//    Este módulo contiene la Implementación del Contexto
//	  de la Maquina Virtual
//
//
//==============================================================================

//Esta definición es específica y común para Client (DIC y SM)

#include "clm4objservicecontextclient.hpp"
#include "cllgadap.hpp"
#include "clappclientadap.hpp"



// Específico para Windows
#ifdef _WINDOWS

#include "csexternal.hpp"	// m4csdll

static m4int32_t TimeoutWindow(m4int32_t ai_iCurrent) {
	return TimeoutDisplay("The current timeout is %ld seconds.", ai_iCurrent);
}

static m4int32_t ServerEventWindow(const long ai_lServerEvent, const void * ai_pParam ) {
	return ServerEventDisplay(ai_lServerEvent, ai_pParam );
}

#endif //_WINDOWS




ClM4ObjServiceContextClient::ClM4ObjServiceContextClient(ClAppClientAdap *ai_pClientAPI) 
 : ClM4ObjServiceContextImpl()
{
	m_bEnableTimeOut = M4_FALSE;
	m_poClientAPI = ai_pClientAPI;

	// timeout window handlers
#ifdef _WINDOWS
	m_hCSTimeoutLibrary = 0;
	m_hCSTimeoutProcedure     = (FARPROC)TimeoutWindow;
	m_hCSServerEventProcedure = (FARPROC)ServerEventWindow;
#endif //_WINDOWS
}

void ClM4ObjServiceContextClient::EnableTimeOut(m4bool_t ai_bEnable) {
#ifdef _WINDOWS
	m_bEnableTimeOut = ai_bEnable;
	if (ai_bEnable)	{
		if (m_hCSTimeoutProcedure != NULL)
			m_poClientAPI->SetTimeoutFunction((ClTimeoutFunction)m_hCSTimeoutProcedure);

		if (m_hCSServerEventProcedure != NULL)
			m_poClientAPI->SetServerEventFunction((ClServerEventFunction)m_hCSServerEventProcedure);
	}
	else {
		m_poClientAPI->SetTimeoutFunction(NULL);
		m_poClientAPI->SetServerEventFunction(NULL);
	}
#endif //_WINDOWS
}

m4return_t ClM4ObjServiceContextClient::SetCSTimeoutFunction (m4pcchar_t ai_pcDLLName, m4pcchar_t ai_pcFuncName) {
	m4return_t iRet = M4_ERROR;

#ifdef _WINDOWS

	HINSTANCE hNewLibrary = NULL;
	FARPROC hNewProcedure = NULL;
	iRet = M4_SUCCESS;

	if (ai_pcDLLName != NULL && ai_pcFuncName != NULL && *ai_pcDLLName != '\0' && *ai_pcFuncName != '\0') {
		// opens the DLL
		hNewLibrary = LoadLibrary(ai_pcDLLName);
		if (hNewLibrary == NULL)
			return M4_ERROR;
		
		// gets the method
		hNewProcedure = GetProcAddress(hNewLibrary, ai_pcFuncName);
		if (hNewProcedure == NULL)
			return M4_ERROR;
	}
	else {
		// sets the default value
		hNewProcedure = (FARPROC)TimeoutWindow;
		hNewLibrary = NULL;
	}

	// frees the old library
	if (m_hCSTimeoutLibrary != NULL) {
		if (! FreeLibrary(m_hCSTimeoutLibrary) )
			return M4_ERROR;
	}

	// reset members
	m_hCSTimeoutLibrary = hNewLibrary;
	m_hCSTimeoutProcedure = hNewProcedure;

	// reuses the old one (not exported to LN4 yet)
	m_hCSServerEventProcedure = (FARPROC)ServerEventWindow;

	// enables timeout
	EnableTimeOut(M4_TRUE);
#endif	// _WINDOWS

	return iRet;
}

void ClM4ObjServiceContextClient::NotifyVMExecution(m4bool_t ai_bIsStart)
{
	m_poClientAPI->NotifyVMExecution(ai_bIsStart);
}

