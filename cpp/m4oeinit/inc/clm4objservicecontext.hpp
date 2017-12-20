//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objservicecontext.hpp
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
//    Este módulo contiene la Interfaz del Contexto
//	  de la Maquina Virtual
//
//
//==============================================================================

//Esta interfaz es común para Client, Server y SM

#ifndef _CLM4OBJSERVICECONTEXT_
#define _CLM4OBJSERVICECONTEXT_

#include "m4oeinit_dll.hpp"
#include "m4types.hpp"
#include "cldefine.hpp"

class ClLogonAdaptor;
class M4ClExternObject;
class ClVMSign;
class IExecutor;


class ClM4ObjServiceContext
{

public:
	virtual ~ClM4ObjServiceContext() {}

	/**
	 * Acceder a Objetos y DLLs externas.
	 *
	 * @param ai_pccObjectName	Nombre del Objeto.
	 * @param ai_iObjectType	Tipo del Objeto.
	 * @return M4ClExternObject.
	 */
	virtual M4ClExternObject *	GetObject( m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType ) = 0;

	/**
	 * Funciones de TimeOut. Solo para Cliente.
	 */
	virtual void		EnableTimeOut		(m4bool_t ai_bEnable = M4_TRUE) = 0;
	virtual m4bool_t	GetTimeOutState		() = 0;
	virtual m4return_t	SetCSTimeoutFunction(m4pcchar_t ai_pcDLLName=0, m4pcchar_t ai_pcFuncName=0) = 0;
	virtual void NotifyVMExecution(m4bool_t ai_bIsStart) = 0;

protected:
	ClM4ObjServiceContext() {}

};




#endif	// _CLM4OBJSERVICECONTEXT_

