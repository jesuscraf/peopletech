
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttrig.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                13-12-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary trigger
//
//
//==============================================================================



#include "mdfacglb.hpp"

#include "cldefine.hpp"
#include "facide.hpp"


#ifndef __CLTTRIG_HPP__
#define __CLTTRIG_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNitem ;



//=================================================================================
//
// ClTempTrigger
//
// Clase que define la representación temporal de los triggers de TI
//
//=================================================================================

class ClTempTrigger : public ClIdentifier
{

protected:

    m4uint8_t		m_iPrecedence ;
    m4uint8_t		m_iSpin ;
    m4uint8_t		m_iRelationship ;
    m4uint8_t		m_iContext ;
    m4uint8_t		m_iCallOrder ;

    m4char_t		m_acFatherItemId[ M4CL_MAX_ITEM_ID + 2 ] ;
    m4char_t		m_acSonIteImd   [ M4CL_MAX_ITEM_ID + 2 ] ;

// Variables
	ClTempNitem		*m_poFatherNitem ;
	ClTempNitem		*m_poSonNitem ;

// Ordenación
    m4char_t		m_acTiId[ M4CL_MAX_TI_ID + 2 ] ;

// Client server
    m4uint8_t		m_iCsType ;

public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetTiId() ) ;
	}

// Funciones de inicialización ================================================
	
        ClTempTrigger( void ) ;
        ~ClTempTrigger( void ) {} ;


// Funciones de lectura de la parte fija ======================================

    m4uint8_t    GetPrecedence( void ) const
    {
        return( m_iPrecedence ) ;
    }

    m4uint8_t    GetSpin( void ) const
    {
        return( m_iSpin ) ;
    }

    m4uint8_t    GetRelation( void ) const
    {
        return( m_iRelationship ) ;
    }

    m4uint8_t    GetContext( void ) const
    {
        return( m_iContext ) ;
    }

    m4uint8_t    GetCallOrder( void ) const
    {
        return( m_iCallOrder ) ;
    }

    m4pcchar_t    GetSonItemId( void ) const
    {
        return( m_acSonIteImd + 1 ) ;
    }

    m4pcchar_t    GetFatherItemId( void ) const
    {
        return( m_acFatherItemId + 1 ) ;
    }

	ClTempNitem*	GetFatherNitem( void ) const
	{
		return( m_poFatherNitem ) ;
	}

	ClTempNitem*	GetSonNitem( void ) const
	{
		return( m_poSonNitem ) ;
	}


// Funciones de escritura de parte fija =======================================

	m4return_t	SetFatherItemId( m4pcchar_t ai_pccFatherItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccFatherItemId, m_acFatherItemId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetSonItemId( m4pcchar_t ai_pccSonItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccSonItemId, m_acSonIteImd, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

    void    SetPrecedence( m4uint8_t ai_iPrecedence )
    {
        m_iPrecedence = ai_iPrecedence ;
    }

    void    SetSpin( m4uint8_t ai_iSpin )
    {
        m_iSpin = ai_iSpin ;
    }

    void    SetRelation( m4uint8_t ai_iRelationship )
    {
        m_iRelationship = ai_iRelationship ;
    }

    void    SetContext( m4uint8_t ai_iContext )
    {
        m_iContext = ai_iContext ;
    }

    void    SetCallOrder( m4uint8_t ai_iCallOrder )
    {
        m_iCallOrder = ai_iCallOrder ;
    }

	void	SetFatherNitem( ClTempNitem *ai_poNitem )
	{
		m_poFatherNitem = ai_poNitem ;
	}

	void	SetSonNitem( ClTempNitem *ai_poNitem )
	{
		m_poSonNitem = ai_poNitem ;
	}


// Funciones de ordenación ====================================================

	m4pcchar_t	GetTiId( void ) const
	{
		return( m_acTiId + 1 ) ;
	}

	m4return_t	SetTiId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccTiId, m_acTiId, M4CL_MAX_TI_ID, ai_bConvert ) ) ;
	}


// Funciones de client server =================================================

    void    SetCsType( m4uint8_t ai_iCsType )
    {
        m_iCsType = ai_iCsType ;
    }
    
    m4uint8_t	GetCsType( void )
    {
        return( m_iCsType ) ;
    }

} ;




#endif

