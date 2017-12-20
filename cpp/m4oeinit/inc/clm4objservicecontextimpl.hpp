//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objservicecontextimpl.hpp
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
//	  de la Maquina Virtual
//
//
//==============================================================================

//Esta definición es común para Client, Server y SM

#ifndef _CLM4OBJSERVICECONTEXTIMPL_
#define _CLM4OBJSERVICECONTEXTIMPL_

// Ojo: posible bug del compilador. Includes en este orden
#include "clextman.hpp"
#include "clm4objservicecontext.hpp"

class ClM4ObjServiceContextImpl : public ClM4ObjServiceContext
{

public:
	/**
	 * Acceder a Objetos y DLLs externas.
	 *
	 * @param ai_pccObjectName	Nombre del Objeto.
	 * @param ai_iObjectType	Tipo del Objeto.
	 * @return M4ClExternObject.
	 */
	virtual M4ClExternObject *	GetObject( m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType );

	/**
	 * Funciones de TimeOut. Solo para Cliente.
	 */
	virtual void		EnableTimeOut		(m4bool_t ai_bEnable = M4_TRUE) {}
	virtual m4bool_t	GetTimeOutState		() { return M4_FALSE; }
	virtual m4return_t	SetCSTimeoutFunction(m4pcchar_t ai_pcDLLName=0, m4pcchar_t ai_pcFuncName=0) { return M4_ERROR; }
	virtual void NotifyVMExecution(m4bool_t ai_bIsStart) {};

protected:

	ClM4ObjServiceContextImpl();

	/** Gestor de Objetos externos */
	M4ClExternObjectManager m_oExternObjectManager;

};


#endif	// _CLM4OBJSERVICECONTEXTIMPL_

