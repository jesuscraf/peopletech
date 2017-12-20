
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltrsm.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                4-9-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary RSM
//
//
//==============================================================================



#include "mdfacglb.hpp"

#include "spoolstr.h"
#include "facide.hpp"




#ifndef __CLTRSM_HPP__
#define __CLTRSM_HPP__





//=============================================================================
//
// ClTempRSM
//
// Clase que implementa los RSM temporales
//
//=============================================================================

class	ClTempRSM : public ClIdentifier
{

protected:

// Fijos
    m4uint32_t			m_iRSMId ;
    m4uint32_t			m_iMaskId ;

// Auxiliares
	ClStaticPoolStr		*m_poUpperPool ;


public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetRSMId() ) ;
	}


// Funciones de inicialización ================================================
	
        ClTempRSM( void ) ;
        ~ClTempRSM( void ) {} ;

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}


// Funciones de lectura de la parte fija ======================================

	m4uint32_t	GetRSMIdHandle( void ) const
	{
		return( m_iRSMId ) ;
	}

	m4uint32_t	GetMaskIdHandle( void ) const
	{
		return( m_iMaskId ) ;
	}

	m4pcchar_t	GetRSMId( void ) const
	{
		return( m_poUpperPool->GetString( m_iRSMId ) ) ;
	}


// Funciones de escritura de parte fija =======================================

	m4return_t  SetRSMId( m4pcchar_t ai_pccRSMId )
	{
		return( M4MdFacSetPoolId( ai_pccRSMId, m_iRSMId, m_poUpperPool ) ) ;
	}

	m4return_t  SetMaskId( m4pcchar_t ai_pccMaskId )
	{
		return( M4MdFacSetPoolId( ai_pccMaskId, m_iMaskId, m_poUpperPool ) ) ;
	}

} ;




#endif

