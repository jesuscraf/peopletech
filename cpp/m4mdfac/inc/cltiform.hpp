
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltiform.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                12-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary formula of an item
//
//
//==============================================================================



#include "mdfacglb.hpp"
#include "cldefine.hpp"
#include "facide.hpp"


#ifndef __CLTIFORM_HPP__
#define __CLTIFORM_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempTi ;
class	ClTempNitem ;
class	IFuncTableDesc ;



//=============================================================================
//
// ClTempItemFormula
//
// Clase que implementa las fórmulas de item temporales
//
//=============================================================================

class	ClTempItemFormula : public ClTwoIdentifier
{

protected:

// Fijos
	ClTempTi		*m_poTi ;
	m4char_t		m_acFormulaId[ M4CL_MAX_RULE_ID + 2 ] ;
	m4uint8_t	    m_iType ;
	m4uint8_t	    m_iPriority ;
	m4uint8_t	    m_iIsMetarule ;
	m4uint8_t	    m_iIsRetroactivity ;
	m4uint8_t	    m_iBatchType ;
	m4uint8_t	    m_iIsBatchMetarule ;
	m4uint8_t	    m_iIsEvent ;
	m4uint8_t	    m_iIsCorrect ;
	m4int32_t		m_iDBOrder ;
	m4pchar_t	    m_pcPolish ;
	m4pcchar_t	    m_pccObject ;
	m4pcchar_t	    m_pccMethod ;

	m4date_t		m_dStartDate ;
	m4date_t		m_dEndDate ;

// Variables
	ClTempNitem		*m_poNitem ;

// Auxiliares
	m4bool_t		m_bIsDatabase ;

// Ordenación
    m4char_t		m_acTiId[ M4CL_MAX_TI_ID + 2 ] ;
    m4char_t		m_acItemId[ M4CL_MAX_ITEM_ID + 2 ] ;

public:


	m4pcchar_t	GetParentIdentifier( void ) const
	{
		return( GetTiId() ) ;
	}

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetItemId() ) ;
	}

// Funciones de inicialización ================================================
	
		ClTempItemFormula( void ) ;
		~ClTempItemFormula( void ) {} ;


// Funciones de lectura de la parte fija ======================================

	ClTempTi*	GetTi( void ) const
	{
		return( m_poTi ) ;
	}

	m4pcchar_t	GetFormulaId( void ) const
	{
		return( m_acFormulaId + 1 ) ;
	}

    m4uint8_t   GetType( void ) const
    {
		return( m_iType ) ;
    }

    m4uint8_t   GetPriority( void ) const
    {
		return( m_iPriority ) ;
    }

    m4uint8_t   GetIsMetarule( void ) const
    {
		return( m_iIsMetarule ) ;
    }

    m4uint8_t   GetIsRetroactivity( void ) const
    {
		return( m_iIsRetroactivity ) ;
    }

    m4uint8_t   GetBatchType( void ) const
    {
		return( m_iBatchType ) ;
    }

    m4uint8_t   GetIsBatchMetarule( void ) const
    {
		return( m_iIsBatchMetarule ) ;
    }

    m4uint8_t	GetIsEvent( void ) const
    {
		return( m_iIsEvent ) ;
    }

    m4uint8_t	GetIsCorrect( void ) const
    {
		return( m_iIsCorrect ) ;
    }

    m4int32_t   GetDBOrder( void ) const
    {
		return( m_iDBOrder ) ;
    }

	m4pchar_t	GetPolish( void ) const
	{
		return( m_pcPolish ) ;
	}

	m4pcchar_t	GetFormObject( void ) const
	{
		return( m_pccObject ) ;
	}

	m4pcchar_t	GetMethod( void ) const
	{
		return( m_pccMethod ) ;
	}

	m4date_t	GetStartDate( void ) const
	{
		return( m_dStartDate ) ;
	}

	m4date_t	GetEndDate( void ) const
	{
		return( m_dEndDate ) ;
	}

	ClTempNitem*	GetNitem( void ) const
	{
		return( m_poNitem ) ;
	}

	m4bool_t	GetIsDatabase( void ) const
	{
		return( m_bIsDatabase ) ;
	}


// Funciones de escritura de parte fija =======================================

	void	SetTi( ClTempTi *ai_poTi )
	{
		m_poTi = ai_poTi ;
	}

	m4return_t	SetFormulaId( m4pcchar_t ai_pccFormulaId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccFormulaId, m_acFormulaId, M4CL_MAX_RULE_ID, ai_bConvert ) ) ;
	}

    void	SetType( m4uint8_t ai_iType )
    {
        m_iType = ai_iType ;
    }

    void	SetPriority( m4uint8_t ai_iPriority )
    {
        m_iPriority = ai_iPriority ;
    }

    void	SetIsMetarule( m4uint8_t ai_iIsMetarule )
    {
        m_iIsMetarule = ai_iIsMetarule ;
    }

    void	SetIsRetroactivity( m4uint8_t ai_iIsRetroactivity )
    {
        m_iIsRetroactivity = ai_iIsRetroactivity ;
    }

    void	SetBatchType( m4uint8_t ai_iBatchType )
    {
        m_iBatchType = ai_iBatchType ;
    }

    void	SetIsBatchMetarule( m4uint8_t ai_iIsBatchMetarule )
    {
        m_iIsBatchMetarule = ai_iIsBatchMetarule ;
    }

    void    SetIsEvent( m4uint8_t ai_iIsEvent )
    {
	    m_iIsEvent = ai_iIsEvent ;
    }

    void    SetIsCorrect( m4uint8_t ai_iIsCorrect )
    {
	    m_iIsCorrect = ai_iIsCorrect ;
    }

    void	SetDBOrder( m4int32_t ai_iDBOrder )
    {
        m_iDBOrder = ai_iDBOrder ;
    }

    void	SetPolish( m4pchar_t ai_pcPolish, IFuncTableDesc *ai_poTable ) ;

    void	SetObject( m4pcchar_t ai_pccObject )
    {
        m_pccObject = ai_pccObject ;
    }
	
    void	SetMethod( m4pcchar_t ai_pccMethod, IFuncTableDesc *ai_poTable ) ;
	
    void	SetStartDate( m4date_t ai_dStartDate )
    {
        m_dStartDate = ai_dStartDate ;
    }

    void    SetEndDate( m4date_t ai_dEndDate )
    {
        m_dEndDate = ai_dEndDate ;
    }

	void	SetNitem( ClTempNitem *ai_poNitem )
	{
		m_poNitem = ai_poNitem ;
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

