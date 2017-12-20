
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltnicon.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary connector of nitems
//
//
//==============================================================================




#include "cltypes.hpp"
#include "clpstack.hpp"
#include "mdfacglb.hpp"



#ifndef __CLTNICON_HPP__
#define __CLTNICON_HPP__





//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNitem ;
class	ClTempConnector ;
class	ClTempTrigger ;
class	ClStaticPoolStr ;
class	ClMdStatistics ;



//=================================================================================
//
// ClTempNitemConn
//
// Clase que implementa la definición de los conectores de item
//
//=================================================================================

class	ClTempNitemConn
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;

// Fijos
    m4uint8_t			m_iPrecedence ;
    m4uint8_t			m_iSpin ;
    m4uint8_t			m_iRelationship ;
    m4uint8_t			m_iContext ;
    m4uint8_t			m_iCsType ;


// Auxiliares
    m4uint8_t           m_iIsOwn ;

    m4uint8_t           m_iCallOrder ;
    m4uint8_t			m_iIsOverWritten ;

    ClTempConnector		*m_poConnector ;
    ClTempNitem         *m_poFatherNitem ;
    ClTempNitem         *m_poSonNitem ;

    ClPointerStack      m_oArgumentMappings ;

// Client server
    m4uint8_t			m_iIsToCompile ;

// Ordenación
    m4char_t			m_acFatherNodeId[ M4CL_MAX_NODE_ID + 2 ] ;
    m4char_t			m_acFatherItemId[ M4CL_MAX_ITEM_ID + 2 ] ;


public:

    m4return_t  _Delete( void ) ;

// Funciones de inicialización ================================================
	
        ClTempNitemConn( void ) ;
        ~ClTempNitemConn( void ) ;



// Funciones de lectura de la parte fija ======================================

    m4uint8_t GetPrecedence( void ) const
    {
        return( m_iPrecedence ) ;
    }

    m4uint8_t GetSpin( void ) const
    {
        return( m_iSpin ) ;
    }

    m4uint8_t GetRelation( void ) const
    {
        return( m_iRelationship ) ;
    }

    m4uint8_t GetContext( void ) const
    {
        return( m_iContext ) ;
    }


// Otras funciones de lectura ================================================

    m4uint8_t GetIsOwn( void ) const
    {
        return( m_iIsOwn ) ;
    }

    m4uint8_t GetCallOrder( void ) const
    {
        return( m_iCallOrder ) ;
    }

	m4uint8_t	GetIsOverWritten( void ) const
	{
		return( m_iIsOverWritten ) ;
	}

    ClTempConnector*  GetConnector( void ) const
    {
        return( m_poConnector ) ;
    }

    ClTempNitem*     GetSonNitem( void ) const
    {
        return( m_poSonNitem ) ;
    }

    ClTempNitem*     GetFatherNitem( void ) const
    {
        return( m_poFatherNitem ) ;
    }

	m4uint16_t	GetNumberOfArgMappings( void ) const
	{
		return( m_oArgumentMappings.GetLength() );
	}

    StArgumentMappig_t* GetArgMappingByPosition( m4uint16_t ai_iPosition ) ;


// Funciones de escritura de parte fija =======================================

    void   SetPrecedence( m4uint8_t ai_iPrecedence )
    {
        m_iPrecedence = ai_iPrecedence ;
    }

    void   SetSpin( m4uint8_t ai_iSpin )
    {
        m_iSpin = ai_iSpin ;
    }

    void   SetRelation( m4uint8_t ai_iRelationship )
    {
        m_iRelationship = ai_iRelationship ;
    }

    void   SetContext( m4uint8_t ai_iContext )
    {
        m_iContext = ai_iContext ;
    }


// Otras funciones de escritura ===============================================

    void    SetIsOwn( m4uint8_t ai_iIsOwn )
    {
        m_iIsOwn = ai_iIsOwn ;
    }

    void    SetCallOrder( m4uint8_t ai_iCallOrder )
    {
        m_iCallOrder = ai_iCallOrder ;
    }

	void	SetIsOverWritten( m4uint8_t ai_iIsOverWritten )
	{
		m_iIsOverWritten = ai_iIsOverWritten ;
	}

    void    SetConnector( ClTempConnector* ai_poConnector )
    {
        m_poConnector = ai_poConnector ;
    }

    void    SetFatherNitem( ClTempNitem* ai_poFatherNitem )
    {
        m_poFatherNitem = ai_poFatherNitem ;
    }

    void    SetSonNitem( ClTempNitem* ai_poSonNitem )
    {
        m_poSonNitem = ai_poSonNitem ;
    }


    m4return_t	AddArgMapping( StArgumentMappig_t *ai_pstArgumentMapping ) ;


// Funciones de serialización =================================================

	m4uint32_t	GetHandle( void ) const
	{
		return( m_iHandle ) ;
	}

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}

	m4return_t	PreGetSize( void ) ;
    m4return_t	GetSize   ( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile   ( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize ) ;


// Funciones de client server =================================================

    void    SetCsType( m4uint8_t ai_iCsType )
    {
        m_iCsType = ai_iCsType ;
    }
    
	void    SetIsToCompile( m4uint8_t ai_iIsToCompile )
    {
        m_iIsToCompile = ai_iIsToCompile ;
    }

    m4uint8_t	GetCsType( void )
    {
        return( m_iCsType ) ;
    }

    m4uint8_t	GetIsToCompile( void )
    {
        return( m_iIsToCompile ) ;
    }


// Funciones de ordenación ====================================================

	m4pcchar_t	GetFatherNodeId( void ) const
	{
		return( m_acFatherNodeId + 1 ) ;
	}

	m4pcchar_t	GetFatherItemId( void ) const
	{
		return( m_acFatherItemId + 1 ) ;
	}

	m4return_t	SetFatherNodeId( m4pcchar_t ai_pccNodeId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccNodeId, m_acFatherNodeId, M4CL_MAX_NODE_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetFatherItemId( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccItemId, m_acFatherItemId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}


// Varias =====================================================================

	void	OrderArgumentMappings( void ) ;
    void	FillConnectorWithTrigger( ClTempTrigger* a_poTrigger ) ;

} ;



#endif

