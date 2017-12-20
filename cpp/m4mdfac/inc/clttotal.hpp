
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttotal.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                18-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se declara el objeto para representar los totales
//
//
//==============================================================================


#include "mdfacglb.hpp"
#include "sliarray.hpp"
#include "cldefine.hpp"
#include "facide.hpp"



#ifndef __CLTTOTAL_HPP__
#define __CLTTOTAL_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNitem ;



//=================================================================================
//
// ClTempTotal
//
// Clase que implementa la definición de los totales
//
//=================================================================================

class	ClTempTotal : public ClSlicedElement
{

protected:

	m4char_t		m_acTiId		[ M4CL_MAX_TI_ID   + 2 ] ;
	m4char_t		m_acItemId		[ M4CL_MAX_ITEM_ID + 2 ] ;
	m4char_t		m_acItemUsedId	[ M4CL_MAX_ITEM_ID + 2 ] ;

	m4date_t		m_dStartDate ;
	m4date_t		m_dEndDate ;

	m4uint8_t		m_iHasBackPay ;
	m4int8_t		m_iSumType ;

	ClTempNitem		*m_poNitem ;


public:


// Funciones de inicialización ================================================
	
		ClTempTotal( void ) ;
		~ClTempTotal( void ) {} ;


// Funciones de lectura =======================================================

	m4pcchar_t	GetTiId( void ) const
	{
		return( m_acTiId + 1 ) ;
	}

	m4pcchar_t	GetItemId( void ) const
	{
		return( m_acItemId + 1 ) ;
	}

	m4pcchar_t	GetItemUsedId( void ) const
	{
		return( m_acItemUsedId + 1 ) ;
	}

	m4date_t	GetStartDate( void ) const
	{
		return( m_dStartDate ) ;
	}

	m4date_t	GetEndDate( void ) const
	{
		return( m_dEndDate ) ;
	}

	m4uint8_t	GetHasBackPay( void ) const
	{
		return( m_iHasBackPay ) ;
	}

	m4int8_t	GetSumType( void ) const
	{
		return( m_iSumType ) ;
	}

	ClTempNitem*	GetNitem( void ) const
	{
		return( m_poNitem ) ;
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

	m4return_t  SetItemUsedId( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccItemId, m_acItemUsedId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

	void    SetStartDate( m4date_t ai_dStartDate )
	{
		m_dStartDate = ai_dStartDate ;
	}

	void    SetEndDate( m4date_t ai_dEndDate )
	{
		m_dEndDate = ai_dEndDate ;
	}

	void    SetHasBackPay( m4uint8_t ai_iHasBackPay )
	{
		m_iHasBackPay = ai_iHasBackPay ;
	}

	void    SetSumType( m4int8_t ai_iSumType )
	{
		m_iSumType = ai_iSumType ;
	}

	void	SetNitem( ClTempNitem *ai_poNitem )
	{
		m_poNitem = ai_poNitem ;
	}


// Funciones varias ===========================================================

	m4bool_t	IsEqual( ClSlicedElement *ai_poElement ) ;
	m4return_t	CheckConsistency( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo ) ;
	m4return_t	CheckOverLap( ClSlicedElement *ai_poElement, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4pvoid_t ai_pvInfo ) ;

} ;


#endif

