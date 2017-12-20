
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttrig.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                13-12-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the temporary trigger
//
//
//==============================================================================




#include <stddef.h>
#include "clttrig.hpp"



//=================================================================================
//
// ClTempTrigger
//
// Clase que define la representación temporal de los triggers de TI
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempTrigger :: ClTempTrigger( void )
{
	m_iPrecedence = 0 ;
	m_iSpin = 0 ;
	m_iRelationship = 0 ;
	m_iContext = 0 ;
	m_iCallOrder = 0 ;

	m_acFatherItemId[ 0 ] = '\0' ;
	m_acFatherItemId[ 1 ] = '\0' ;
	m_acSonIteImd   [ 0 ] = '\0' ;
	m_acSonIteImd   [ 1 ] = '\0' ;

	m_poFatherNitem = NULL ;
	m_poSonNitem = NULL ;

	m_acTiId[ 0 ] = '\0' ;
	m_acTiId[ 1 ] = '\0' ;

	m_iCsType = 0 ;
}


