
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttiref.cpp   
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
//    En este módulo se implementa el objeto para representar las referencias a Tis
//
//
//==============================================================================


#include <stddef.h>
#include "clttiref.hpp"



//=================================================================================
//
// ClTempTiReference
//
// Clase que implementa la definición de las referencias de Tis
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempTiReference :: ClTempTiReference( void )
{
	m_poNode = NULL ;
	m_iReferences = 0 ;
}



