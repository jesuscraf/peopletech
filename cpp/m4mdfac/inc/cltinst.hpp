
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltinst.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                24-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the instances of a t3
//
//
//==============================================================================



#include "mdfacglb.hpp"
#include "cltypes.hpp"
#include "facide.hpp"
#include "spoolstr.h"



#ifndef __CLTINST_HPP__
#define __CLTINST_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClMdStatistics ;




//=================================================================================
//
// ClTempInstance
//
// Clase que implementa la definición de las instancias
//
//=================================================================================

class	ClTempInstance : public ClIdentifier
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;

// Fijos
    m4uint32_t			m_iInstanceId ;
    m4uint32_t			m_iT3Id ;
    m4uint32_t			m_iRoleId ;
    m4uint32_t			m_iOrgId ;

	m4uint8_t			m_iScope ;
	m4uint8_t			m_iOpenMode ;
	m4uint8_t			m_iCsAccess ;
	m4uint8_t			m_iUseRole ;
	m4uint8_t			m_iOrgType ;


public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetInstanceId() ) ;
	}


// Funciones de inicialización ================================================
	
	ClTempInstance( void ) ;
    ~ClTempInstance( void ) {} ;


// Funciones de lectura de la parte fija ======================================

	m4pcchar_t	GetInstanceId( void ) const
    {
		return( m_poUpperPool->GetString( m_iInstanceId ) ) ;
    }

    m4uint32_t  GetInstanceIdHandle( )
    {
        return( m_iInstanceId ) ;
    }

	m4pchar_t	GetT3Id( void ) const
    {
		return( m_poUpperPool->GetString( m_iT3Id ) ) ;
    }

	m4pchar_t	GetRoleId( void ) const
    {
		return( m_poUpperPool->GetString( m_iRoleId ) ) ;
    }

	m4pchar_t	GetOrgId( void ) const
    {
		return( m_poUpperPool->GetString( m_iOrgId ) ) ;
    }

	m4uint8_t	GetScope( void ) const
    {
        return( m_iScope ) ;
    }

	m4uint8_t	GetOpenMode( void ) const
    {
        return( m_iOpenMode ) ;
    }

	m4uint8_t	GetCsAccess( void ) const
    {
        return( m_iCsAccess ) ;
    }

	m4uint8_t	GetUseRole( void ) const
    {
        return( m_iUseRole ) ;
    }

	m4uint8_t	GetOrgType( void ) const
    {
        return( m_iOrgType ) ;
    }


// Funciones de escritura de parte fija =======================================

	m4return_t  SetInstanceId( m4pcchar_t ai_pccInstanceId )
	{
		return( M4MdFacSetPoolId( ai_pccInstanceId, m_iInstanceId, m_poUpperPool ) ) ;
	}

	m4return_t  SetT3Id( m4pcchar_t ai_pccT3Id )
	{
		return( M4MdFacSetPoolId( ai_pccT3Id, m_iT3Id, m_poUpperPool ) ) ;
	}

	m4return_t  SetRoleId( m4pcchar_t ai_pccRoleId )
	{
		return( M4MdFacSetPoolId( ai_pccRoleId, m_iRoleId, m_poUpperPool ) ) ;
	}

	m4return_t  SetOrgId( m4pcchar_t ai_pccOrgId )
	{
		return( M4MdFacSetPoolId( ai_pccOrgId, m_iOrgId, m_poUpperPool ) ) ;
	}

	void SetScope( m4uint8_t ai_iScope )
	{
		m_iScope = ai_iScope ;
	}

	void SetOpenMode( m4uint8_t ai_iOpenMode )
	{
		m_iOpenMode = ai_iOpenMode ;
	}

	void SetCsAccess( m4uint8_t ai_iCsAccess )
	{
		m_iCsAccess = ai_iCsAccess ;
	}

	void SetUseRole( m4uint8_t ai_iUseRole )
	{
		m_iUseRole = ai_iUseRole ;
	}

	void SetOrgType( m4uint8_t ai_iOrgType )
	{
		m_iOrgType = ai_iOrgType ;
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

