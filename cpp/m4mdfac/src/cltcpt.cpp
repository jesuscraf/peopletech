
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltcpt.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-05-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se define el objeto para representar los conceptos
//
//
//==============================================================================


#include "cltcpt.hpp"



//=================================================================================
//
// ClTempConcept
//
// Clase que implementa la definición de los conceptos
//
//=================================================================================



// Funciones de inicialización ================================================
	
ClTempConcept :: ClTempConcept( void )
{
	m_acTiId    [ 0 ] = '\0' ;
	m_acTiId    [ 1 ] = '\0' ;
	m_acItemId  [ 0 ] = '\0' ;
	m_acItemId  [ 1 ] = '\0' ;
	m_pcItemName = NULL ;

	m_iHasBackPay = 0 ;
}


