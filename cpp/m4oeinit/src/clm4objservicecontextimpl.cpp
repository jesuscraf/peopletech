//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objservicecontextimpl.cpp
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

//Esta implementación es común para Client, Server y SingleMachine

#include "clm4objservicecontextimpl.hpp"
//#include "chlog.hpp"	// M4ObjLog

/** Constructor */
ClM4ObjServiceContextImpl::ClM4ObjServiceContextImpl() {

}

/**
 * Acceder a Objetos y DLLs externas.
 *
 * @param ai_pccObjectName	Nombre del Objeto.
 * @param ai_iObjectType	Tipo del Objeto.
 * @return M4ClExternObject.
 */
M4ClExternObject *ClM4ObjServiceContextImpl::GetObject( m4pcchar_t ai_pccObjectName, m4uint32_t ai_iObjectType ) {
    M4ClExternObject    *poResult ;

    poResult = m_oExternObjectManager.GetObject( ai_pccObjectName, ai_iObjectType ) ;

//	CHECK_CHLOG_ERRORF( poResult == NULL, NULL, M4_CH_MCR_NO_GET_OBJECT, ai_pccObjectName << ai_iObjectType ) ;

    return( poResult ) ;
}
