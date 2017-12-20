
//==============================================================================
//
// (c) Copyright  1991-2005 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clmappings.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-2005
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the mappings for objects
//
//
//==============================================================================



#include "m4mdfac_dll.hpp"

#include "cltypes.hpp"
#include "cldatesbase.hpp"
#include "clpstack.hpp"
#include "facide.hpp"
#include "clprsstb.hpp"
#include "spoolstr.h"



#ifndef __CLMAPPINGS_HPP__
#define __CLMAPPINGS_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClChannelManager ;
class	ClAccess ;
class	ClAccessRecordSet ;
class	ClGenericIODriver ;
class	ClCacheableObject ;




//=================================================================================
//
// ClOneMap
//
// Clase que implementa la definición de un mapeo
//
//=================================================================================

class	ClOneMap : public ClIdentifier
{

protected:

    m4uint32_t      m_iSource ;
    m4uint32_t      m_iDestination ;
	ClStaticPoolStr	*m_poPool ;

public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetSource() ) ;
	}


// Funciones de inicialización ================================================
	
		ClOneMap( void ) ;
		~ClOneMap( void ) {} ;


// Funciones de lectura =======================================================

	m4pcchar_t	GetSource( void ) const
	{
		return( m_poPool->GetString( m_iSource ) ) ;
	}

	m4pcchar_t	GetDestination( void ) const
	{
		return( m_poPool->GetString( m_iDestination ) ) ;
	}

	m4bool_t	AreEqual( ClOneMap& ai_roOneMap )
	{
		if( ai_roOneMap.m_iSource != m_iSource || ai_roOneMap.m_iDestination != m_iDestination )
		{
			return( M4_FALSE ) ;
		}
		return( M4_TRUE ) ;
	}


// Funciones de escritura =====================================================

    m4return_t  SetSource( m4pcchar_t ai_pccSource )
	{
		return( M4MdFacSetPoolId( ai_pccSource, m_iSource, m_poPool ) ) ;
	}

    m4return_t  SetDestination( m4pcchar_t ai_pccDestination )
	{
		return( M4MdFacSetPoolId( ai_pccDestination, m_iDestination, m_poPool ) ) ;
	}

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poPool = ai_poPool ;
	}


// Funciones de serialización =================================================

	m4return_t	Serialize( ClGenericIODriver& IOD ) const ;
	m4return_t	DeSerialize( ClGenericIODriver& IOD, ClStaticPoolStr *ao_poPool ) ;

} ;




//=================================================================================
//
// ClMapGroup
//
// Clase que implementa la definición de un grupo de mapeos
//
//=================================================================================

class	ClMapGroup
{

protected:

	ClPointerStack		m_oMappings ;
	ClOneMap			*m_poMappingsArray ;

// Auxiliares
    ClCompIdentifier	m_oComparisonOneMap ;
    ClCompIdentifier	*m_poComparisonOneMap ;

public:

// Funciones de inicialización ================================================
	
		ClMapGroup( void ) ;
		~ClMapGroup( void ) ;

	m4return_t	Init( ClAccessRecordSet* ai_poRecordset, ClStaticPoolStr *ao_poPool ) ;


// Funciones de lectura =======================================================

	m4bool_t	AreEqual( ClMapGroup& ai_roMapGroup ) ;

	m4uint16_t  GetNumberOfMappings( void ) const
	{
		return( m_oMappings.GetLength() ) ;
	}

	m4return_t	GetMappingByPosition( m4uint16_t ai_iPosition, m4pcchar_t& ao_rpccSource, m4pcchar_t& ao_rpccDestination ) ;
	m4pcchar_t	GetMappingById( m4pcchar_t ai_pccSource, m4bool_t ai_bConvert ) ;

	m4uint32_t	GetSize( void ) ;


// Funciones de serialización =================================================

	m4return_t	Serialize( ClGenericIODriver& IOD ) const ;
	m4return_t	DeSerialize( ClGenericIODriver& IOD, ClStaticPoolStr *ao_poPool ) ;

} ;




//=================================================================================
//
// ClOrgMaps
//
// Clase que implementa la definición de los mapeos de una sociedad
//
//=================================================================================

class	ClOrgMaps : public ClIdentifier
{

protected:

    m4uint32_t      m_iOrganization ;
	m4int32_t		m_iMapGroupPosition ;

	ClStaticPoolStr	*m_poPool ;
	ClPointerStack	*m_poMapGroups ;

public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetOrganization() ) ;
	}


// Funciones de inicialización ================================================
	
		ClOrgMaps( void ) ;
		~ClOrgMaps( void ) {} ;


// Funciones de lectura =======================================================

	m4pcchar_t	GetOrganization( void ) const
	{
		return( m_poPool->GetString( m_iOrganization ) ) ;
	}

	ClMapGroup*  GetMapGroup( void ) const ;


// Funciones de escritura =====================================================

    m4return_t  SetOrganization( m4pcchar_t ai_pccOrganization )
	{
		return( M4MdFacSetPoolId( ai_pccOrganization, m_iOrganization, m_poPool ) ) ;
	}

	void	SetMapGroupPosition( m4uint32_t ai_iMapGroupPosition )
	{
		m_iMapGroupPosition = ai_iMapGroupPosition ;
	}

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poPool = ai_poPool ;
	}

	void	SetMapGroups( ClPointerStack *ai_poMapgroups )
	{
		m_poMapGroups = ai_poMapgroups ;
	}


// Funciones de serialización =================================================

	m4return_t	Serialize( ClGenericIODriver& IOD ) const ;
	m4return_t	DeSerialize( ClGenericIODriver& IOD, ClStaticPoolStr *ao_poPool, ClPointerStack *ai_poMapGroups ) ;

} ;




//=================================================================================
//
// ClMappings
//
// Clase que implementa la definición de los mapeos de un objeto
// Inplementa un objeto cacheable
//
//=================================================================================

class	M4_DECL_M4MDFAC	ClMappings : public ClPersistiableObject
{

protected:

	m4uint32_t			m_iSize ;
	m4date_t			m_dVersion ;
	ClPointerStack		m_oMapGroups ;
	ClPointerStack		m_oOrgMappings ;
	ClOrgMaps			*m_poOrgMappingsArray ;

// Auxiliares
    ClCompIdentifier	m_oComparisonOrgMapping ;
    ClCompIdentifier	*m_poComparisonOrgMapping ;

// De cache
	ClCacheableObject	*m_pCO ;

// Pool de cadenas
	ClStaticPoolStr		m_oPool ;

public:


// Funciones de inicialización ================================================
	
		ClMappings( void ) ;
		~ClMappings( void ) ;

	m4return_t	Init( m4pcchar_t ai_pccMapId, ClChannelManager *ai_poChannelManager ) ;

	void Destroy( m4bool_t ai_bRemoveFromCache = M4_FALSE ) ;


// Funciones de lectura =======================================================

	m4pcchar_t	GetMappingById( m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccSource, m4bool_t ai_bConvert ) ;

	static	m4return_t	IsEquivalentOrganization( ClMappings* ai_poMappings1, ClMappings* ai_poMappings2, m4pcchar_t ai_pccOrganization1, m4pcchar_t ai_pccOrganization2, m4pcchar_t ai_pccDestination, m4bool_t ai_bConvert, m4bool_t& ao_rbIsEquivalent ) ;


// Funciones de cache =========================================================

	m4uint32_t	GetSize( void ) const
	{
		return( m_iSize ) ;
	}

	m4date_t	GetVersion( void ) const
	{
		return( m_dVersion ) ;
	}

	m4date_t    GetStartDate( void ) const
	{
		return( M4CL_MINUS_INFINITE_IN_JULIAN ) ; 
	}

	m4date_t    GetEndDate( void ) const
	{
		return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
	}

	m4date_t    GetCorStartDate( void ) const
	{
		return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
	}

	m4date_t    GetCorEndDate( void ) const
	{
		return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
	}

	void RemoveFromCache( void ) ;

	void SetCO( ClCacheableObject *ai_pCO )
	{
		m_pCO = ai_pCO;
	}

	ClCacheableObject*	GetCO( void ) const
    {
        return( m_pCO ) ;
    }

// Funciones de ClPersistiableObject ==========================================

	m4return_t  Serialize( ClGenericIODriver &IOD ) ;
    m4return_t  DeSerialize( ClGenericIODriver &IOD ) ;

protected:

	m4return_t	_Init( m4pcchar_t ai_pccMapId, ClAccess *ai_poAccess ) ;

} ;




#endif

