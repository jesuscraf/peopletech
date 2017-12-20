
//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltitinh.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                09-03-2009
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se declara el objeto para representar la sobreescritura de items
//
//
//==============================================================================


#include "mdfacglb.hpp"
#include "cldefine.hpp"
#include "facide.hpp"



#ifndef __CLTITINH_HPP__
#define __CLTITINH_HPP__



//=================================================================================
//
// ClTempItemInherit
//
// Clase que implementa la definición de la sobreescritura de items
//
//=================================================================================

class	ClTempItemInherit : public ClIdentifier
{

protected:

	m4char_t	m_acTiId	[ M4CL_MAX_TI_ID     + 2 ] ;
	m4char_t	m_acItemId	[ M4CL_MAX_ITEM_ID   + 2 ] ;
	m4pchar_t	m_pcItemName ;


public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetTiId() ) ;
	}

// Funciones de inicialización ================================================
	
		ClTempItemInherit( void ) ;

		~ClTempItemInherit( void )
		{
			if( m_pcItemName != NULL )
			{
				delete( m_pcItemName ) ;
				m_pcItemName = NULL ;
			}
		}


// Funciones de lectura =======================================================

	m4pcchar_t	GetTiId( void ) const
	{
		return( m_acTiId + 1 ) ;
	}

	m4pcchar_t	GetItemId( void ) const
	{
		return( m_acItemId + 1 ) ;
	}

	m4pcchar_t	GetItemName( void ) const
	{
		if( m_pcItemName == NULL )
		{
			return( "" ) ;
		}
		return( m_pcItemName + 1 ) ;
	}


// Funciones de escritura =====================================================

	m4return_t	SetTiId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccTiId, m_acTiId, M4CL_MAX_TI_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetItemId( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccItemId, m_acItemId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetItemName( m4pcchar_t ai_pccItemName )
	{
		return( M4MdFacSetLocalId( ai_pccItemName, m_pcItemName, M4_FALSE ) ) ;
	}

} ;


#endif

