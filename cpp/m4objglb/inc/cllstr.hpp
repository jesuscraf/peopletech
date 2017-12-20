
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                cllstr.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                26-05-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo implementa la clase para cadenas largas
//
//
//==============================================================================


#include "m4objglb_dll.hpp"

#include "cldefine.hpp"



#ifndef __CLLSTR_HPP__
#define __CLLSTR_HPP__




//=============================================================================
//
// ClLongString
//
// Clase que implementa una cadena dimensionable para los fórmulas
//
//=============================================================================

class	M4_DECL_M4OBJGLB	ClLongString
{

protected:

	m4uint32_t	m_iSize ;
	m4uint32_t	m_iLength ;
	m4uint32_t	m_iDelta ;
	m4uint32_t	m_iInitSize ;
	m4pchar_t	m_pcString ;


	m4return_t	_Increment( m4uint32_t ai_iDelta ) ;

public:

// Funciones de inicialización ================================================
	
		ClLongString( void ) ;
		~ClLongString( void ) ;

	void		Reset( void ) ;
	m4return_t	Init( m4uint32_t ai_iSize, m4uint32_t ai_iDelta ) ;


// Funciones de lectura de la parte fija ======================================

	m4uint32_t	GetLength( void ) const
	{
		if( m_iLength == 0 )
		{
			return( 0 ) ;
		}

		return( m_iLength - 1 ) ;
	}

	m4pchar_t	GetString( void ) const
	{
		return( m_pcString ) ;
	}

// Otras funciones de escritura ===============================================

	m4return_t	Concat( m4pcchar_t ai_pccString, m4uint32_t ai_iLength, m4uint8_t ai_iMode = M4MDRT_CODE_MODE_PLAIN ) ;
	m4return_t	Concat( m4pcchar_t ai_pccString, m4uint8_t ai_iMode = M4MDRT_CODE_MODE_PLAIN ) ;
	m4pchar_t	Concat( m4uint32_t ai_iSize ) ;


// Funciones varias ===========================================================

	m4pchar_t	Clone( void ) const ;

} ;



#endif
