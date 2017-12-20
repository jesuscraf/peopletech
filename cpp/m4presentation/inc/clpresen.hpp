//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                clpresen.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:               C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the class of the factory of compiled metachannels
//
//
//==============================================================================
 
#include "clprsstb.hpp"
#include "cldates.hpp"
#include "m4presentation_dll.hpp"
#include "cllstr.hpp"

#ifndef __PRESENTATION_HPP__
#define __PRESENTATION_HPP__

#define M4_VM_PRES_VER	( 3 + ( 812 << 6 ) )

class	ClChannelManager;
class	ClCacheableObject;
class	CBufferASN;

class M4_DECL_M4PRESENTATION ClPresentation : public ClPersistiableObject
{

private:

	//Chunk de la Presentacion Real
	//m4uint8_t * m_piPresChunk;
	//m4uint32_t m_iSize;
	CBufferASN	*m_pBufferASN;

	//Cache
	ClCacheableObject * m_pCO;

public:

	ClPresentation( void ) ;
	ClPresentation( CBufferASN	*ai_pBufferASN ) ;
	~ClPresentation( void ) ;

	void Destroy(m4bool_t ai_bRemoveFromCache = M4_FALSE);

	m4uint32_t	GetSize( void ) const ;

	m4uchar_t *	GetBuffer( void ) const ;

	CBufferASN * GetBufferASN( void ) const ;

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

    m4return_t  Serialize( ClGenericIODriver &IOD ) ;
    m4return_t  DeSerialize( ClGenericIODriver &IOD ) ;

	void SetCO( ClCacheableObject * ai_pCO )
	{
		m_pCO = ai_pCO;
	}

	ClCacheableObject*	GetCO( void ) const
    {
        return( m_pCO ) ;
    }

	m4date_t	GetVersion( void ) const;
	m4uint16_t	GetStyle( void ) const;

	m4uint32_t	GetNumT3 ( void ) const;
	m4pchar_t	GetIdT3ByIndex ( m4uint32_t iIndex ) const;
	m4date_t	GetVersionT3ByIndex ( m4uint32_t iIndex ) const;

	m4uint32_t	GetNumInclude ( void ) const;
	string		GetIdIncludeByPos ( m4uint32_t ai_iPos ) const;
	m4date_t	GetVersionIncludeByPos( m4uint32_t ai_iPos );

	m4return_t BuildXML( ClLongString &ao_roXML, m4pcchar_t ai_pccOrganization, ClChannelManager *ai_poChannelManager ) const;
} ;



#endif

