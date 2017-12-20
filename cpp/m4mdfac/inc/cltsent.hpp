
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltsent.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                13-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary sentence
//
//
//==============================================================================


#include "m4facdef.hpp"
#include "mdfacglb.hpp"

#include "cltypes.hpp"
#include "spoolstr.h"
#include "facide.hpp"



#ifndef __CLTSENT_HPP__
#define __CLTSENT_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClMdStatistics ;



//=================================================================================
//
// ClTempSentence
//
// Clase que implementa la definición de las sentencias
//
//=================================================================================

class	ClTempSentence : public ClIdentifier
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;
	ClStaticPoolStr		*m_poNoUpperPool ;

// Fijos
    m4uint32_t			m_iSentenceId ;
    m4uint32_t			m_iGroupObjects ;
    m4uint32_t			m_iApiSql1 ;
    m4uint32_t			m_iApiSql2 ;
    m4uint32_t			m_iApiSql3 ;
    m4uint32_t			m_iApiSql4 ;
    m4uint32_t			m_iMaxNumCol ;

// Client server
    m4uint8_t			m_iIsToCompile ;

public:


	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetSentenceId() ) ;
	}


// Funciones de inicialización ================================================
	
        ClTempSentence( void ) ;
        ~ClTempSentence( void ) {} ;


// Funciones de lectura de la parte fija ======================================

	m4uint32_t	GetSentenceIdHandle( void ) const
	{
		return( m_iSentenceId ) ;
	}

	m4pcchar_t	GetSentenceId( void ) const
	{
		return( m_poUpperPool->GetString( m_iSentenceId ) ) ;
	}

	m4pcchar_t	GetGroupObjects( void ) const
	{
		return( m_poUpperPool->GetString( m_iGroupObjects ) ) ;
	}
		
	m4pcchar_t	GetApiSql1( void ) const
	{
		return( m_poNoUpperPool->GetString( m_iApiSql1 ) ) ;
	}
		
// Funciones de escritura de parte fija =======================================

	m4return_t  SetSentenceId( m4pcchar_t ai_pccSentenceId )
	{
		return( M4MdFacSetPoolId( ai_pccSentenceId, m_iSentenceId, m_poUpperPool ) ) ;
	}

	void	SetSentenceId( m4uint32_t ai_iSentenceId )
	{
		m_iSentenceId = ai_iSentenceId ;
	}

    m4return_t  SetGroupObjects( m4pcchar_t ai_pccGroupObjects )
	{
		return( M4MdFacSetPoolId( ai_pccGroupObjects, m_iGroupObjects, m_poUpperPool ) ) ;
	}

    m4return_t  SetApiSql1( m4pcchar_t ai_pccApiSql )
	{
		return( M4MdFacSetPoolId( ai_pccApiSql, m_iApiSql1, m_poNoUpperPool ) ) ;
	}

    m4return_t  SetApiSql2( m4pcchar_t ai_pccApiSql )
	{
		return( M4MdFacSetPoolId( ai_pccApiSql, m_iApiSql2, m_poNoUpperPool ) ) ;
	}
	
    m4return_t  SetApiSql3( m4pcchar_t ai_pccApiSql )
	{
		return( M4MdFacSetPoolId( ai_pccApiSql, m_iApiSql3, m_poNoUpperPool ) ) ;
	}
	
    m4return_t  SetApiSql4( m4pcchar_t ai_pccApiSql )
	{
		return( M4MdFacSetPoolId( ai_pccApiSql, m_iApiSql4, m_poNoUpperPool ) ) ;
	}
	
    void    SetMaxNumCol( m4uint32_t ai_iMaxNumCol )
    {
        m_iMaxNumCol = ai_iMaxNumCol ;
    }


// Funciones de serialización =================================================

	m4uint32_t	GetHandle( void ) const
	{
		return( m_iHandle ) ;
	}

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}

	void	SetNoUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poNoUpperPool = ai_poPool ;
	}

    m4return_t	GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize ) ;


// Funciones de client server =================================================

	void    SetIsToCompile( m4uint8_t ai_iIsToCompile )
    {
        m_iIsToCompile = ai_iIsToCompile ;
    }

    m4uint8_t	GetIsToCompile( void )
    {
        return( m_iIsToCompile ) ;
    }


} ;


#endif

