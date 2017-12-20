
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltngrp.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                30-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary node group
//
//
//==============================================================================



#include "mdfacglb.hpp"

#include "facide.hpp"
#include "cltypes.hpp"
#include "clpstack.hpp"
#include "spoolstr.h"



#ifndef __CLTNGRP_HPP__
#define __CLTNGRP_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNitem ;
class	ClMdStatistics ;


//=================================================================================
//
// ClTempGroup
//
// Clase que implementa la definición de los grupos
//
//=================================================================================

class	ClTempGroup : public ClIdentifier
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;

// Fijos 
    m4uint32_t			m_iGroupId ;

// Variables
    ClPointerStack		m_oNitems ;


public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetGroupId() ) ;
	}


// Funciones de inicialización ================================================
	
		ClTempGroup( void ) ;
		~ClTempGroup( void ) {} ;


// Funciones de lectura de la parte fija ======================================

	m4pcchar_t	GetGroupId( void ) const
	{
		return( m_poUpperPool->GetString( m_iGroupId ) ) ;
	}

	m4uint32_t	GetGroupIdHandle( void ) const
	{
		return( m_iGroupId ) ;
	}

		
// Funciones de escritura de parte fija =======================================

	m4return_t	SetGroupId( m4pcchar_t ai_pccGroupId )
	{
		return( M4MdFacSetPoolId( ai_pccGroupId, m_iGroupId, m_poUpperPool ) ) ;
	}


// Otras funciones de escritura ===============================================

    m4return_t	AddNitem( ClTempNitem* ai_poNitem ) ;

	void    AddNitemDelta( m4uint16_t ai_iDelta )
    {
        m_oNitems.AddDelta( ai_iDelta ) ;
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

    m4return_t	GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize ) ;

} ;


#endif

