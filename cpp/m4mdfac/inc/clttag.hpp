
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttag.hpp   
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
//    En este módulo se declara el objeto para representar los tags
//
//
//==============================================================================


#include "mdfacglb.hpp"
#include "sliarray.hpp"
#include "cldefine.hpp"



#ifndef __CLTTAG_HPP__
#define __CLTTAG_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNitem ;



//=================================================================================
//
// ClTempTag
//
// Clase que implementa la definición de los tag
//
//=================================================================================

class	ClTempTag : public ClSlicedElement
{

protected:

	m4char_t		m_acTagId		[ M4CL_MAX_TAG_ID  + 2 ] ;
	m4char_t		m_acTiId		[ M4CL_MAX_TI_ID   + 2 ] ;
	m4char_t		m_acItemId		[ M4CL_MAX_ITEM_ID + 2 ] ;
	m4char_t		m_acRuleId		[ M4CL_MAX_RULE_ID + 2 ] ;
	m4char_t		m_acDimItemId	[ M4CL_MAX_ITEM_ID + 2 ] ;
	m4char_t		m_acZValue		[ M4CL_MAX_ZVALUE  + 2 ] ;

	m4date_t		m_dStartDate ;
	m4date_t		m_dEndDate ;

	m4char_t		m_cOperator ;

	ClTempNitem		*m_poNitem ;


public:


// Funciones de inicialización ================================================
	
		ClTempTag( void ) ;
		~ClTempTag( void ) {} ;


// Funciones de lectura =======================================================

	m4pcchar_t	GetTagId( void ) const
	{
		return( m_acTagId + 1 ) ;
	}

	m4pcchar_t	GetTiId( void ) const
	{
		return( m_acTiId + 1 ) ;
	}

	m4pcchar_t	GetItemId( void ) const
	{
		return( m_acItemId + 1 ) ;
	}

	m4pcchar_t	GetRuleId( void ) const
	{
		return( m_acRuleId + 1 ) ;
	}

	m4pcchar_t	GetDimItemId( void ) const
	{
		return( m_acDimItemId + 1 ) ;
	}

	m4pcchar_t	GetZValue( void ) const
	{
		return( m_acZValue + 1 ) ;
	}

	m4date_t	GetStartDate( void ) const
	{
		return( m_dStartDate ) ;
	}

	m4date_t	GetEndDate( void ) const
	{
		return( m_dEndDate ) ;
	}

	m4char_t	GetOperator( void ) const
	{
		return( m_cOperator ) ;
	}

	ClTempNitem*	GetNitem( void ) const
	{
		return( m_poNitem ) ;
	}


// Funciones de escritura =====================================================

	m4return_t	SetTagId( m4pcchar_t ai_pccTagId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccTagId, m_acTagId, M4CL_MAX_TAG_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetTiId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccTiId, m_acTiId, M4CL_MAX_TI_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetItemId( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccItemId, m_acItemId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetRuleId( m4pcchar_t ai_pccRuleId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccRuleId, m_acRuleId, M4CL_MAX_RULE_ID, ai_bConvert ) ) ;
	}

	m4return_t  SetDimItemId( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccItemId, m_acDimItemId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

	m4return_t  SetZValue( m4pcchar_t ai_pccZValue, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccZValue, m_acZValue, M4CL_MAX_ZVALUE, M4_FALSE ) ) ;
	}

	void    SetStartDate( m4date_t ai_dStartDate )
	{
		m_dStartDate = ai_dStartDate ;
	}

	void    SetEndDate( m4date_t ai_dEndDate )
	{
		m_dEndDate = ai_dEndDate ;
	}

	void    SetOperator( m4char_t ai_cOperator )
	{
		m_cOperator = ai_cOperator ;
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

