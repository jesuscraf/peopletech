
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jhandledata.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                21-01-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements a class for java object handle data.
//
//
//==============================================================================

#include "m4jhandledata.hpp"


m4pchar_t	M4CloneString( m4pcchar_t ai_pccSource, m4int32_t ai_iLength = -1 ) ;
void		M4FreeString( m4pchar_t &aio_rpcString ) ;


//=================================================================================
//
// M4JavaHandleData
//
// Clase que define los datos asociados a un handle de objeto java
//
//=================================================================================

// Funciones de inicialización ================================================

M4JavaHandleData::M4JavaHandleData( m4pcchar_t ai_pccClassName, M4JavaContext *ai_poContext )
{
	m_pcClassName = M4CloneString( ai_pccClassName ) ;
	m_poContext = ai_poContext ;
}


M4JavaHandleData::~M4JavaHandleData( void )
{
	M4FreeString( m_pcClassName ) ;
}


