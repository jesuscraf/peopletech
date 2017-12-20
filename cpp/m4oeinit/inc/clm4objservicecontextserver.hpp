//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                clm4objservicecontextserver.hpp
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
//	  de la Maquina Virtual Server
//
//
//==============================================================================

//Esta definición es específica para Server


#ifndef _CLM4OBJSERVICECONTEXTSERVER_
#define _CLM4OBJSERVICECONTEXTSERVER_

#include "clm4objservicecontextimpl.hpp"


class ClM4ObjServiceContextServer : public ClM4ObjServiceContextImpl
{

public:
	ClM4ObjServiceContextServer() ;

};



#endif	// _CLM4OBJSERVICECONTEXTSERVER_

