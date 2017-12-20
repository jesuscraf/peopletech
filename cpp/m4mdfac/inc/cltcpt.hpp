
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltcpt.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se declara el objeto para representar los conceptos
//
//
//==============================================================================


#include "mdfacglb.hpp"
#include "cldefine.hpp"
#include "facide.hpp"



#ifndef __CLTCPT_HPP__
#define __CLTCPT_HPP__



//=================================================================================
//
// ClTempConcept
//
// Clase que implementa la definición de los conceptos
//
//=================================================================================

class	ClTempConcept : public ClIdentifier
{

protected:

	m4char_t	m_acTiId	[ M4CL_MAX_TI_ID     + 2 ] ;
	m4char_t	m_acItemId	[ M4CL_MAX_ITEM_ID   + 2 ] ;
	m4pchar_t	m_pcItemName ;

	m4uint8_t	m_iHasBackPay ;


public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetTiId() ) ;
	}

// Funciones de inicialización ================================================
	
		ClTempConcept( void ) ;

		~ClTempConcept( void )
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

	m4uint8_t	GetHasBackPay( void ) const
	{
		return( m_iHasBackPay ) ;
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

	void    SetHasBackPay( m4uint8_t ai_iHasBackPay )
	{
		m_iHasBackPay = ai_iHasBackPay ;
	}

} ;


#endif

