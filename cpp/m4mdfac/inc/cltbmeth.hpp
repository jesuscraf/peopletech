
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltbmeth.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                23-11-2007
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary business method
//
//
//==============================================================================



#include "mdfacglb.hpp"
#include "spoolstr.h"
#include "facide.hpp"


#ifndef __CLTBMETH_HPP__
#define __CLTBMETH_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNitem ;
class	ClTempBusinessArg ;
class	ClMdStatistics ;




//=================================================================================
//
// ClTempBusinessMethod
//
// Clase que implementa la definición de los métodos de negocio
//
//=================================================================================

class	ClTempBusinessMethod : public ClIdentifier
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;
	ClStaticPoolStr		*m_poNoUpperPool ;

// Fijos
    m4uint32_t			m_iMethodId ;
    m4uint32_t			m_iName ;
    m4uint32_t			m_iDescription ;

// Componentes
    ClReferenceStack	m_oArguments ;

// Auxiliares
	ClTempNitem			*m_poNitem ;


public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetMethodId() ) ;
	}


// Funciones de inicialización ================================================
	
        ClTempBusinessMethod( void ) ;
        ~ClTempBusinessMethod( void ) {} ;


// Funciones de lectura de la parte fija ======================================

    m4uint32_t  GetMethodIdHandle( void ) const
    {
		return( m_iMethodId ) ;
    }

    m4pcchar_t  GetMethodId( void ) const
    {
		return( m_poUpperPool->GetString( m_iMethodId ) ) ;
    }


// Funciones de escritura de parte fija =======================================

	m4return_t	SetMethodId( m4pcchar_t ai_pccMethodId )
	{
		return( M4MdFacSetPoolId( ai_pccMethodId, m_iMethodId, m_poUpperPool ) ) ;
	}

	m4return_t	SetName( m4pcchar_t ai_pccName )
	{
		return( M4MdFacSetPoolId( ai_pccName, m_iName, m_poNoUpperPool ) ) ;
	}

	m4return_t	SetDescription( m4pcchar_t ai_iDescription )
	{
		return( M4MdFacSetPoolId( ai_iDescription, m_iDescription, m_poNoUpperPool ) ) ;
	}

	void	SetNitem( ClTempNitem *ai_poNitem )
	{
		m_poNitem = ai_poNitem ;
	}


// Funciones de referencia ====================================================

    void	AddArgument( void )
	{
		m_oArguments.AddElement() ;
	}

	void	InitArguments( ClTempBusinessArg **ai_ppoArguments )
	{
		m_oArguments.Init( ( m4pvoid_t* ) ai_ppoArguments ) ;
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
} ;



#endif

