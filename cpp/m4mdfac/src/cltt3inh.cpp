
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltt3inh.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                28-06-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se implementa el objeto para representar la herencia de T3s
//
//
//==============================================================================



#include "cltt3inh.hpp"



//=============================================================================
//
// ClTempT3Inherit
//
// Clase que implementa la definición de la herencia de T3s
//
//=============================================================================

// Funciones de inicialización ================================================
	
ClTempT3Inherit :: ClTempT3Inherit( void )
{
	m_iBaseT3Id = 0 ;
	m_iLevel = 0 ;
	m_iIndex = 0 ;

	m_poUpperPool = NULL ;
}



