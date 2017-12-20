
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltt3al.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                18-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the alias of a t3
//
//
//==============================================================================



#include "cltt3al.hpp"



//=================================================================================
//
// ClTempT3Alias
//
// Clase que implementa la definición de los alias de T3
//
//=================================================================================

ClTempT3Alias :: ClTempT3Alias( void )
{
    m_iT3AliasId = 0 ;
    m_iInstanceId = 0 ;

    m_acT3Id  [ 0 ] = '\0' ;
    m_acT3Id  [ 1 ] = '\0' ;
    m_acRoleId[ 0 ] = '\0' ;
    m_acRoleId[ 1 ] = '\0' ;
    m_acOrgId [ 0 ] = '\0' ;
    m_acOrgId [ 1 ] = '\0' ;

    m_iScope = 0 ;
    m_iOpenMode = 0 ;
	m_iCsAccess = 0 ;
	m_iOrgType = 0 ;
	m_iLevel = 0 ;

    m_poUpperPool = NULL ;
}















