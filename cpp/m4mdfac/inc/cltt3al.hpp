
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltt3al.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                18-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the alias of a t3
//
//
//==============================================================================



#include "mdfacglb.hpp"

#include "facide.hpp"
#include "cltypes.hpp"
#include "cldefine.hpp"
#include "spoolstr.h"



#ifndef __CLTT3AL_HPP__
#define __CLTT3AL_HPP__



//=================================================================================
//
// ClTempT3Alias
//
// Clase que implementa la definición de los alias de T3
//
//=================================================================================

class	ClTempT3Alias : public ClIdentifier
{

protected:

	m4uint32_t      m_iT3AliasId ;
	m4uint32_t      m_iInstanceId ;
	m4char_t        m_acT3Id  [ M4CL_MAX_T3_ID   + 2 ] ;
	m4char_t        m_acRoleId[ M4CL_MAX_ROLE_ID + 2 ] ;
	m4char_t        m_acOrgId [ M4CL_MAX_ORG_ID  + 2 ] ;
	m4uint8_t	    m_iScope ;
	m4uint8_t	    m_iOpenMode ;
	m4uint8_t	    m_iCsAccess ;
	m4uint8_t	    m_iOrgType ;

	m4uint8_t	    m_iLevel ;

    ClStaticPoolStr *m_poUpperPool ;

// Ordenación
	m4char_t		m_acNodeId[ M4CL_MAX_NODE_ID + 2 ] ;


public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetT3AliasId() ) ;
	}


// Funciones de inicialización ================================================
	
	ClTempT3Alias( void ) ;
    ~ClTempT3Alias( void ) {} ;


// Funciones de lectura de la parte fija ======================================

    m4uint32_t  GetT3AliasIdHandle( void ) const
    {
		return( m_iT3AliasId ) ;
    }

    m4pcchar_t  GetT3AliasId( void ) const
    {
		return( m_poUpperPool->GetString( m_iT3AliasId ) ) ;
    }

    m4uint32_t  GetInstanceIdHandle( void ) const
    {
		return( m_iInstanceId ) ;
    }

    m4pchar_t  GetInstanceId( void ) const
    {
		return( m_poUpperPool->GetString( m_iInstanceId ) ) ;
    }

    m4pchar_t  GetT3Id( void )
    {
		return( m_acT3Id + 1 ) ;
    }

    m4pchar_t  GetRoleId( void )
    {
		return( m_acRoleId + 1 ) ;
    }

    m4pchar_t  GetOrgId( void )
    {
		return( m_acOrgId + 1 ) ;
    }

    m4uint8_t   GetScope( void ) const
    {
        return( m_iScope ) ;
    }

    m4uint8_t   GetOpenMode( void ) const
    {
        return( m_iOpenMode ) ;
    }

    m4uint8_t   GetCsAccess( void ) const
    {
        return( m_iCsAccess ) ;
    }

    m4uint8_t   GetOrgType( void ) const
    {
        return( m_iOrgType ) ;
    }

    m4uint8_t   GetLevel( void ) const
    {
        return( m_iLevel ) ;
    }


// Funciones de escritura de parte fija =======================================

	m4return_t  SetT3AliasId( m4pcchar_t ai_pccT3AliasId )
	{
		return( M4MdFacSetPoolId( ai_pccT3AliasId, m_iT3AliasId, m_poUpperPool ) ) ;
	}

	m4return_t  SetInstanceId( m4pcchar_t ai_pccInstanceId )
	{
		return( M4MdFacSetPoolId( ai_pccInstanceId, m_iInstanceId, m_poUpperPool ) ) ;
	}

	m4return_t  SetT3Id( m4pcchar_t ai_pccT3, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccT3, m_acT3Id, M4CL_MAX_T3_ID, ai_bConvert ) ) ;
	}

	m4return_t  SetRoleId( m4pcchar_t ai_pccRole, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccRole, m_acRoleId, M4CL_MAX_ROLE_ID, ai_bConvert ) ) ;
	}

	m4return_t  SetOrgId( m4pcchar_t ai_pccOrg, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccOrg, m_acOrgId, M4CL_MAX_ORG_ID, ai_bConvert ) ) ;
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

	void SetOrgType( m4uint8_t ai_iOrgType )
	{
		m_iOrgType = ai_iOrgType ;
	}

	void SetLevel( m4uint8_t ai_iLevel )
	{
		m_iLevel = ai_iLevel ;
	}


// Funciones de serialización =================================================

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}


// Funciones de ordenación ====================================================

	m4pcchar_t	GetNodeId( void ) const
	{
		return( m_acNodeId + 1 ) ;
	}

	m4return_t	SetNodeId( m4pcchar_t ai_pccNodeId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccNodeId, m_acNodeId, M4CL_MAX_NODE_ID, ai_bConvert ) ) ;
	}

} ;



#endif


