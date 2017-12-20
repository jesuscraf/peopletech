
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttiinh.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                14-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se declara el objeto para representar la herencia de TIs
//
//
//==============================================================================


#include "mdfacglb.hpp"
#include "cldefine.hpp"



#ifndef __CLTTIINH_HPP__
#define __CLTTIINH_HPP__




//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempTi ;



//=================================================================================
//
// ClTempTiInherit
//
// Clase que implementa la definición de la herencia de TIs
//
//=================================================================================

class	ClTempTiInherit
{

protected:

	m4char_t	m_acTiId[ M4CL_MAX_TI_ID + 2 ] ;
	ClTempTi	*m_poBaseTi ;

	m4uint8_t	m_iLevel ;


public:


// Funciones de inicialización ================================================
	
		ClTempTiInherit( void ) ;
		~ClTempTiInherit( void ) {} ;


// Funciones de lectura =======================================================

	m4pcchar_t	GetTiId( void ) const
	{
		return( m_acTiId + 1 ) ;
	}

	ClTempTi	*GetBaseTi( void ) const
	{
		return( m_poBaseTi ) ;
	}

	m4uint8_t	GetLevel( void ) const
	{
		return( m_iLevel ) ;
	}


// Funciones de escritura =====================================================

	m4return_t	SetTiId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccTiId, m_acTiId, M4CL_MAX_TI_ID, ai_bConvert ) ) ;
	}

	void    SetBaseTi( ClTempTi *ai_poBaseTi )
	{
		m_poBaseTi = ai_poBaseTi ;
	}

	void    SetLevel( m4uint8_t ai_iLevel )
	{
		m_iLevel = ai_iLevel ;
	}

} ;


#endif

