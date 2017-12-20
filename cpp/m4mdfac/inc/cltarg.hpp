
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltarg.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary argument
//
//
//==============================================================================



#include "mdfacglb.hpp"

#include "cldefine.hpp"
#include "spoolstr.h"



#ifndef __CLTARG_HPP__
#define __CLTARG_HPP__



//=============================================================================
//
// ClTempArgument
//
// Clase que implementa los argumentos temporales
//
//=============================================================================

class	ClTempArgument
{

protected:

// Fijos
	m4uint32_t			m_iArgumentId ;
	m4uint32_t			m_iName ;
	m4uint16_t			m_iPrecision ;
	m4uint8_t			m_iType ;
	m4uint8_t			m_iM4Type ;

	/* Bug 0071091
	El tamaño debe permitir ordenes de más de 128
	*/
	m4uint32_t			m_iOrder ;

// Auxiliares
	m4uint8_t			m_iRealOrder ;

	ClStaticPoolStr		*m_poUpperPool ;
	ClStaticPoolStr		*m_poNoUpperPool ;

// Ordenación
    m4char_t			m_acTiId  [ M4CL_MAX_TI_ID   + 2 ] ;
    m4char_t			m_acItemId[ M4CL_MAX_ITEM_ID + 2 ] ;


public:


// Funciones de inicialización ================================================
	
		ClTempArgument( void ) ;
		~ClTempArgument( void ) {} ;

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}

	void	SetNoUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poNoUpperPool = ai_poPool ;
	}


// Funciones de lectura de la parte fija ======================================

	m4uint32_t	GetArgumentIdHandle( void ) const
	{
		return( m_iArgumentId ) ;
	}

	m4pcchar_t	GetArgumentId( void ) const
	{
		return( m_poUpperPool->GetString( m_iArgumentId ) ) ;
	}

	m4uint32_t	GetNameHandle( void ) const
	{
		return( m_iName ) ;
	}

	m4uint16_t	GetPrecision( void ) const
	{
		return( m_iPrecision ) ;
	}

	m4uint8_t	GetType( void ) const
	{
		return( m_iType ) ;
	}

	m4uint8_t	GetM4Type( void ) const
	{
		return( m_iM4Type ) ;
	}

    m4uint32_t	GetOrder( void ) const
	{
		return( m_iOrder ) ;
	}

    m4uint8_t	GetRealOrder( void ) const
	{
		return( m_iRealOrder ) ;
	}


// Funciones de escritura de parte fija =======================================

	m4return_t	SetArgumentId( m4pcchar_t ai_pccArgumentId )
	{
		return( M4MdFacSetPoolId( ai_pccArgumentId, m_iArgumentId, m_poUpperPool ) ) ;
	}

	m4return_t	SetName( m4pcchar_t ai_pccName )
	{
		return( M4MdFacSetPoolId( ai_pccName, m_iName, m_poNoUpperPool ) ) ;
	}

	void    SetPrecision( m4uint16_t ai_iPrecision )
    {
	    m_iPrecision = ai_iPrecision ;
    }

	void    SetType( m4uint8_t ai_iType )
    {
	    m_iType = ai_iType ;
    }

    void    SetM4Type( m4uint8_t ai_iM4Type )
    {
	    m_iM4Type = ai_iM4Type ;
    }

    void    SetOrder( m4uint32_t ai_iOrder )
    {
	    m_iOrder = ai_iOrder ;
    }

    void    SetRealOrder( m4uint8_t ai_iRealOrder )
    {
	    m_iRealOrder = ai_iRealOrder ;
    }


// Funciones de ordenación ====================================================

	m4pcchar_t	GetTiId( void ) const
	{
		return( m_acTiId + 1 ) ;
	}

	m4pcchar_t	GetItemId( void ) const
	{
		return( m_acItemId + 1 ) ;
	}

	m4return_t	SetTiId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccTiId, m_acTiId, M4CL_MAX_TI_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetItemId( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccItemId, m_acItemId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

} ;


#endif

