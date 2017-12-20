
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltnode.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                11-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary node
//
//
//==============================================================================


#include "cltypes.hpp"
#include "clpstack.hpp"
#include "cltalias.hpp"
#include "cltt3al.hpp"
#include "cltindex.hpp"
#include "cltngrp.hpp"
#include "cltplug.hpp"
#include "facide.hpp"


#ifndef __CLTNODE_HPP__
#define __CLTNODE_HPP__




//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempConnector ;
class   ClTempAlias ;
class   ClTempT3Alias ;
class   ClTempFilter ;
class   ClTempPlug ;
class   ClTempNodeInherit ;
class   ClTempNitem ;
class   ClTempTi ;
class	ClTempChannel ;
class	ClMdStatistics ;




//=================================================================================
//
// ClTempNode
//
// Clase que implementa la definición de los nodos
//
//=================================================================================

class	ClTempNode : public ClIdentifier
{

protected:

// Serializacion
	m4uint32_t				m_iHandle ;
	ClStaticPoolStr			*m_poUpperPool ;
	ClStaticPoolStr			*m_poNoUpperPool ;

// Fijos
    m4uint32_t	            m_iNodeId ;
    m4uint32_t	            m_iName ;
    m4uint32_t	            m_iGroupObjects ;
    m4uint32_t              m_iNumRows ;
    m4int32_t               m_iNumKeepRows ;
    m4uint32_t              m_iNumRowsDB ;
    m4uint32_t              m_iOleDispId ;
    m4uint32_t              m_iOleNodeInterId ;
    m4uint32_t              m_iMaxNumBlocks ;
    m4uint32_t              m_iMaxNumRecords ;

    m4uint32_t              m_iConnectorsStart ;
    m4uint32_t              m_iInvConnectorsStart ;
    m4uint32_t              m_iGroupsStart ;
    m4uint32_t              m_iPlugsStart ;
    m4uint32_t              m_iIndexesStart ;
    m4uint32_t              m_iAliasStart ;
    m4uint32_t              m_iT3AliasStart ;
    m4uint32_t              m_iDmdsStart ;
    m4uint32_t              m_iQBFFiltersStart ;
    m4uint32_t              m_iFindFiltersStart ;
    m4uint32_t              m_iSysSentencesStart ;
    m4uint32_t              m_iSysHintsStart ;

    m4uint16_t              m_iSCRStart ;
    m4uint16_t              m_iIndex ;
    m4uint16_t              m_iType ;
    m4uint16_t              m_iOrder ;

    m4uint8_t               m_iCapacity ;
    m4uint8_t               m_iIsRoot ;
    m4uint8_t               m_iIsRealRoot ;
    m4uint8_t               m_iAutoload ;
    m4uint8_t               m_iIsOrdered ;
    m4uint8_t               m_iIsVisible ;
    m4uint8_t               m_iDynFilter ;
    m4uint8_t               m_iIdAutoFilter ;
    m4uint8_t               m_iCsType ;
    m4uint8_t               m_iSyncType ;
    m4uint8_t				m_iIsExternal ;
    m4uint8_t				m_iIsQBFFilter ;
    m4uint8_t				m_iAffectsDB ;
    m4uint8_t				m_iDBReload ;

/*  m4uint16_t              m_iItems ;  auxiliar */
    m4uint16_t              m_iNodeItems ;
    m4uint16_t              m_iBlockItems ;
    m4uint16_t              m_iRegisterItems ;

// Variables
    ClReferenceStack		m_oInherits ;
    ClReferenceStack		m_oNitems ;
    ClReferenceStack		m_oConnectors ;
    ClReferenceStack		m_oInvConnectors ;
    ClReferenceStack		m_oIndexes ;
    ClReferenceStack		m_oPlugs ;
    ClReferenceStack		m_oAlias ;
    ClReferenceStack		m_oT3Alias ;
    ClReferenceStack		m_oFilters ;

    ClPointerStack          m_oGroups ;
    ClPointerStack          m_oDmds ;
    ClPointerStack          m_oSysSentences ;
    ClPointerStack          m_oSysHints ;
	
// Auxiliares
    ClTempNitem             *m_poStartDate ;
    ClTempNitem             *m_poEndDate ;
    ClTempNitem             *m_poCorStartDate ;
    ClTempNitem             *m_poCorEndDate ;
    ClTempNitem             *m_poProjection ;
    ClTempNitem             *m_poFilterStartDate ;
    ClTempNitem             *m_poFilterEndDate ;
    ClTempNitem             *m_poFilterCorStartDate ;
    ClTempNitem             *m_poFilterCorEndDate ;
    ClTempNitem             *m_poCurrencyType ;
    ClTempNitem             *m_poCurrencyExchangeDate ;
    ClTempNitem             *m_poDmdChangeReason ;
    ClTempNitem             *m_poDmdComponent ;
    ClTempNitem             *m_poDmdValue ;
    ClTempNitem             *m_poLoadSQL ;
    ClTempNitem             *m_poPriority ;
    ClTempNitem             *m_poImputeDate ;
    ClTempNitem             *m_poPayDate ;
    ClTempNitem             *m_poPayType ;
    ClTempNitem             *m_poPayFrequency ;
    ClTempNitem             *m_poImputePayType ;
    ClTempNitem             *m_poImputePayFrequency ;
    ClTempNitem             *m_poClosingDate ;
    ClTempNitem             *m_poSysParam ;
    ClTempNitem             *m_poBDLChecks ;
    ClTempNitem             *m_poRAMOrderBy ;
    ClTempNitem             *m_poOrganization ;

    ClTempNitem             *m_poLoadBlk ;
    ClTempNitem             *m_poDeleteBlk ;
    ClTempNitem             *m_poUpdateBlk ;
    ClTempNitem             *m_poInsertBlk ;

	m4uint8_t				m_iLevel ;

    m4uint16_t              m_iItemsInGroups ;

    m4int16_t               m_iConnectedDegree ;

	ClTempNitem				*m_poBackPayNitem ;
	ClTempNitem				*m_poRevNitem ;
	ClTempNode				*m_poRevNode ;

    ClTempIndex             *m_poDefaultIndex ;

    ClTempChannel           *m_poChannel ;
    ClTempTi                *m_poTi ;

    m4uint16_t              m_iConnectedNumber[ M4CL_NODE_CONNECTOR_MAX_TYPE ] ;
    m4uint16_t              m_iConnectedStart [ M4CL_NODE_CONNECTOR_MAX_TYPE ] ;

    ClCompIdentifier		*m_poComparisonNitem ;
    ClCompIdentifier		m_oComparisonNitem ;

    ClCompIdentifier		*m_poComparisonAlias ;
    ClCompIdentifier		m_oComparisonAlias ;

    ClCompIdentifier		*m_poComparisonT3Alias ;
    ClCompIdentifier		m_oComparisonT3Alias ;

    ClCompIdentifier		*m_poComparisonGroup ;
    ClCompIdentifier		m_oComparisonGroup ;

// Client server
    m4uint8_t               m_iIsToCompile ;
	m4uint16_t				m_iConnectorsToCompile ;
	m4uint16_t				m_iInvConnectorsToCompile ;
	m4uint16_t				m_iItemsToCompile ;
	m4uint16_t				m_iSysSentencesToCompile ;
	m4uint16_t				m_iSysHintsToCompile ;
	m4uint16_t				m_iAliasToCompile ;

	m4uint16_t				m_iQBFFilters ;
	m4uint16_t				m_iFindFilters ;

public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetNodeId() ) ;
	}


// Funciones de inicialización ================================================
	
		ClTempNode( void ) ;
		~ClTempNode( void ) ;

	m4return_t	Delete( void ) ;


// Funciones de lectura de la parte fija ======================================

	m4uint32_t	GetNodeIdHandle( void ) const
	{
		return( m_iNodeId ) ;
	}

	m4pcchar_t	GetNodeId( void ) const
	{
		return( m_poUpperPool->GetString( m_iNodeId ) ) ;
	}

	m4pcchar_t	GetName( void ) const
	{
		return( m_poNoUpperPool->GetString( m_iName ) ) ;
	}


// Otras funciones de lectura ================================================

	m4pcchar_t	GetTiId( void ) const ;
	m4uint32_t	GetTiIdHandle( void ) const ;

	ClTempTi*   GetTi( void ) const
	{
		return( m_poTi ) ;
	}

    m4uint8_t	GetIsRoot( void ) const
    {
        return( m_iIsRoot ) ;
    }

    m4uint8_t	GetIsRealRoot( void ) const
    {
        return( m_iIsRealRoot ) ;
    }

    m4uint8_t	GetDynFilter( void ) const
    {
        return( m_iDynFilter ) ;
    }

    m4uint8_t	GetIsQBFFilter( void ) const
    {
        return( m_iIsQBFFilter ) ;
    }

	m4uint8_t	GetLevel( void ) const
	{
		return( m_iLevel ) ;
	}

    m4int16_t   GetItemsInGroups( void ) const
    {
        return( m_iItemsInGroups ) ;
    }

    m4int16_t   GetConnectedDegree( void ) const
    {
        return( m_iConnectedDegree ) ;
    }

	ClTempNitem*	GetBackPayNitem( void ) const
	{
		return( m_poBackPayNitem ) ;
	}

	ClTempNitem*	GetRevNitem( void ) const ;
	ClTempNode*		GetRevNode( void ) const ;

    ClTempChannel*	GetChannel( void ) const
    {
        return( m_poChannel ) ;
    }

    m4uint16_t  GetNumberOfInherits( void ) const
    {
        return( m_oInherits.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfNitems( void ) const
    {
        return( m_oNitems.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfConnectors( void ) const
    {
        return( m_oConnectors.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfInvConnectors( void ) const
    {
        return( m_oInvConnectors.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfIndexes( void ) const
    {
        return( m_oIndexes.GetLength() ) ;
    }

	m4uint16_t  GetNumberOfPlugs( void ) const
    {
        return( m_oPlugs.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfAlias( void ) const
    {
        return( m_oAlias.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfT3Alias( void ) const
    {
        return( m_oT3Alias.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfFilters( void ) const
    {
        return( m_oFilters.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfSysSentences( void ) const
    {
        return( m_oSysSentences.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfSysHints( void ) const
    {
        return( m_oSysHints.GetLength() ) ;
    }

	
    ClTempNodeInherit*	GetInheritByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempNitem*		GetNitemByPosition			( m4uint16_t ai_iPosition ) ;
	ClTempConnector*	GetConnectorByPosition		( m4uint16_t ai_iPosition ) ;
	ClTempConnector*	GetInvConnectorByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempIndex*		GetIndexByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempPlug*			GetPlugByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempNode*			GetAliasByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempT3Alias*		GetT3AliasByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempFilter*		GetFilterByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempNitem*		GetSysSentenceByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempNitem*		GetSysHintByPosition		( m4uint16_t ai_iPosition ) ;

	ClTempNitem*		GetNitemById	( m4pcchar_t ai_pccNitemId,		m4bool_t ai_bConvert ) ;
	ClTempNode*			GetAliasById	( m4pcchar_t ai_pccAliasId,		m4bool_t ai_bConvert ) ;
	ClTempT3Alias*		GetT3AliasById	( m4pcchar_t ai_pccT3AliasId,	m4bool_t ai_bConvert ) ;
	ClTempGroup*		GetGroupById	( m4pcchar_t ai_pccGroupId,		m4bool_t ai_bConvert ) ;

	ClTempConnector*	GetConnectorBySonNode( ClTempNode *ai_poNode ) ;


// Funciones de escritura de parte fija =======================================

	m4return_t	SetNodeId( m4pcchar_t ai_pccNodeId )
	{
		return( M4MdFacSetPoolId( ai_pccNodeId, m_iNodeId, m_poUpperPool ) ) ;
	}

	m4return_t	SetName( m4pcchar_t ai_pccNodeName )
	{
		return( M4MdFacSetPoolId( ai_pccNodeName, m_iName, m_poNoUpperPool ) ) ;
	}

    m4return_t  SetGroupObjects( m4pcchar_t ai_pccGroupObjects )
	{
		return( M4MdFacSetPoolId( ai_pccGroupObjects, m_iGroupObjects, m_poUpperPool ) ) ;
	}

    void    SetNumRows( m4uint32_t ai_iNumRows )
    {
        m_iNumRows = ai_iNumRows ;
    }
	
    void    SetNumKeepRows( m4int32_t ai_iNumKeepRows )
    {
        m_iNumKeepRows = ai_iNumKeepRows ;
    }

    void    SetNumRowsDB( m4uint32_t ai_iNumRowsDB )
    {
        m_iNumRowsDB = ai_iNumRowsDB ;
    }

    void    SetOleDispId( m4uint32_t ai_iOleDispId )
    {
        m_iOleDispId = ai_iOleDispId ;
    }

    m4return_t  SetOleNodeInterId( m4pcchar_t ai_pccOleNodeInterId )
	{
		return( M4MdFacSetPoolId( ai_pccOleNodeInterId, m_iOleNodeInterId, m_poUpperPool ) ) ;
	}

    void    SetMaxNumBlocks( m4uint32_t ai_iMaxNumBlocks )
    {
        m_iMaxNumBlocks = ai_iMaxNumBlocks ;
    }

    void    SetMaxNumRecords( m4uint32_t ai_iMaxNumRecords )
    {
        m_iMaxNumRecords = ai_iMaxNumRecords ;
    }

    void    SetSCRStart( m4uint16_t ai_iSCRStart )
    {
        m_iSCRStart = ai_iSCRStart ;
    }

    void    SetIndex( m4uint16_t ai_iIndex )
    {
        m_iIndex = ai_iIndex ;
    }
	
    void    SetType( m4uint16_t ai_iType )
    {
        m_iType = ai_iType ;
    }
	
    void    SetOrder( m4uint16_t ai_iOrder )
    {
        m_iOrder = ai_iOrder ;
    }

    void    SetCapacity( m4uint8_t ai_iCapacity )
    {
        m_iCapacity = ai_iCapacity ;
    }

    void    SetIsRoot( m4uint8_t ai_iIsRoot )
    {
        m_iIsRoot = ai_iIsRoot ;
    }

	void    SetIsRealRoot( m4uint8_t ai_iIsRealRoot )
    {
        m_iIsRealRoot = ai_iIsRealRoot ;
    }

    void    SetAutoload( m4uint8_t ai_iAutoload )
    {
        m_iAutoload = ai_iAutoload ;
    }

    void    SetIsOrdered( m4uint8_t ai_iIsOrdered )
    {
        m_iIsOrdered = ai_iIsOrdered ;
    }
    
    void    SetIsVisible( m4uint8_t ai_iIsVisible )
    {
        m_iIsVisible = ai_iIsVisible ;
    }

    void    SetDynFilter( m4uint8_t ai_iDynFilter )
    {
        m_iDynFilter = ai_iDynFilter ;
    }

    void    SetIdAutoFilter( m4uint8_t ai_iIdAutoFilter )
    {
        m_iIdAutoFilter = ai_iIdAutoFilter ;
    }

    void    SetIsExternal( m4uint8_t ai_iIsExternal )
    {
        m_iIsExternal = ai_iIsExternal ;
    }

    void    SetIsQBFFilter( m4uint8_t ai_iIsQBFFilter )
    {
        m_iIsQBFFilter = ai_iIsQBFFilter ;
    }

    void    SetAffectsDB( m4uint8_t ai_iAffectsDB )
    {
        m_iAffectsDB = ai_iAffectsDB ;
    }

    void    SetDBReload( m4uint8_t ai_iDBReload )
    {
        m_iDBReload = ai_iDBReload ;
    }


// Otras funciones de escritura ===============================================

	void    SetLevel( m4uint8_t ai_iLevel )
	{
		m_iLevel = ai_iLevel ;
	}

    void    SetConnectedDegree( m4int16_t ai_iConnectedDegree )
    {
        m_iConnectedDegree = ai_iConnectedDegree ;
    }

	void	SetBackPayNitem( ClTempNitem *ai_poBackPayNitem )
	{
		m_poBackPayNitem = ai_poBackPayNitem ;
	}

	void	SetRevNitem( ClTempNitem *ai_poRevNitem )
	{
		m_poRevNitem = ai_poRevNitem ;
	}

	void	SetRevNode( ClTempNode *ai_poRevNode )
	{
		m_poRevNode = ai_poRevNode ;
	}

    void    SetChannel( ClTempChannel *ai_poChannel )
    {
        m_poChannel = ai_poChannel ;
    }

    void    SetTi( ClTempTi *ai_poTi ) ;

    m4return_t	AddGroup( ClTempGroup* ai_poGroup ) ;

    void    IncrementConnectedDegree( void )
    {
        m_iConnectedDegree++ ;
    }

    void    DecrementConnectedDegree( void )
    {
        m_iConnectedDegree-- ;
    }


// Funciones de impresión =====================================================

	m4pcchar_t	iNode( void ) const ;
	m4pcchar_t	nNode( void ) const ;
	m4pcchar_t	iM4Obj( void ) const ;
	m4pcchar_t	nM4Obj( void ) const ;


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

	m4return_t	PreGetSize( void ) ;
    m4return_t	GetSize   ( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile   ( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize ) ;


// Funciones de client server =================================================

    void    SetCsType( m4uint8_t ai_iCsType )
    {
        m_iCsType = ai_iCsType ;
    }
    
    void    SetSyncType( m4uint8_t ai_iSyncType )
    {
        m_iSyncType = ai_iSyncType ;
    }
    
	void    SetIsToCompile( m4uint8_t ai_iIsToCompile )
    {
        m_iIsToCompile = ai_iIsToCompile ;
    }

    m4uint8_t	GetCsType( void ) const
    {
        return( m_iCsType ) ;
    }

    m4uint8_t	GetIsToCompile( void ) const
    {
        return( m_iIsToCompile ) ;
    }

	void	SetConnectorsToCompile( m4uint16_t ai_iConnectors )
	{
		m_iConnectorsToCompile = ai_iConnectors ;
	}

	void	SetItemsToCompile( m4uint16_t ai_iItems )
	{
		m_iItemsToCompile = ai_iItems ;
	}

	void	SetSysSentencesToCompile( m4uint16_t ai_iSysSentences )
	{
		m_iSysSentencesToCompile = ai_iSysSentences ;
	}

	void	SetSysHintsToCompile( m4uint16_t ai_iSysHints )
	{
		m_iSysHintsToCompile = ai_iSysHints ;
	}

	void	SetAliasToCompile( m4uint16_t ai_iAliasToCompile )
	{
		m_iAliasToCompile = ai_iAliasToCompile ;
	}
	

// Funciones de referencia ====================================================

    void	AddInherit( void )
	{
		m_oInherits.AddElement() ;
	}

	void	InitInherits( ClTempNodeInherit **ai_ppoInherits )
	{
		m_oInherits.Init( ( m4pvoid_t* ) ai_ppoInherits ) ;
	}

    m4return_t	AddNitem( ClTempNitem* ai_poNitem ) ;

	void	InitNitems( ClTempNitem **ai_ppoNitems )
	{
		m_oNitems.Init( ( m4pvoid_t* ) ai_ppoNitems ) ;
	}

    void	AddConnector( void )
	{
		m_oConnectors.AddElement() ;
	}

	void	InitConnectors( ClTempConnector **ai_ppoConnectors )
	{
		m_oConnectors.Init( ( m4pvoid_t* ) ai_ppoConnectors ) ;
	}

    void	AddInvConnector( void )
	{
		m_oInvConnectors.AddElement() ;
	}

	void	InitInvConnectors( ClTempConnector **ai_ppoConnectors )
	{
		m_oInvConnectors.Init( ( m4pvoid_t* ) ai_ppoConnectors ) ;
	}

    m4return_t	AddIndex( ClTempIndex* ai_poIndex ) ;

	void	InitIndexes( ClTempIndex **ai_ppoIndexes )
	{
		m_oIndexes.Init( ( m4pvoid_t* ) ai_ppoIndexes ) ;
	}

    void	AddPlug( void )
	{
		m_oPlugs.AddElement() ;
	}

	void	InitPlugs( ClTempPlug **ai_ppoPlugs )
	{
		m_oPlugs.Init( ( m4pvoid_t* ) ai_ppoPlugs ) ;
	}

    void	AddAlias( void )
	{
		m_oAlias.AddElement() ;
	}

	void	InitAlias( ClTempAlias **ai_ppoAlias )
	{
		m_oAlias.Init( ( m4pvoid_t* ) ai_ppoAlias ) ;
	}

	void	AddT3Alias( void )
	{
		m_oT3Alias.AddElement() ;
	}

	void	InitT3Alias( ClTempT3Alias **ai_ppoT3Alias )
	{
		m_oT3Alias.Init( ( m4pvoid_t* ) ai_ppoT3Alias ) ;
	}

    m4return_t	AddFilter( ClTempFilter* ai_poFilter ) ;

	void	InitFilters( ClTempFilter **ai_ppoFilters )
	{
		m_oFilters.Init( ( m4pvoid_t* ) ai_ppoFilters ) ;
	}


// Funciones varias ===========================================================

    m4return_t	OrderItemConnMappings( void ) ;

    m4return_t	CalculateGroupOrder( void ) ;
    m4return_t	CalculateItemTypes( void ) ;
    m4return_t	CalculateDmds( void ) ;
    m4return_t	ComputeConnectedNitemsScope( void ) ;

} ;


#endif

