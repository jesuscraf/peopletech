
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltrsm.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                04-09-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines functions for the temporary RSMs
//
//
//==============================================================================


#include "cltrsm.hpp"



//=============================================================================
//
// ClTempRSM
//
// Clase que implementa los RSM temporales
//
//=============================================================================

// Funciones de inicialización ================================================

ClTempRSM :: ClTempRSM( void )
{
	m_iRSMId = 0 ;
	m_iMaskId = 0 ;
	m_poUpperPool = NULL ;
}



