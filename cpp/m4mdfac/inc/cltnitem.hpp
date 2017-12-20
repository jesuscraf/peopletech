
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltnitem.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                18-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary item of a node
//    It also containd de reference to an item of a node (for system items)
//
//
//==============================================================================



#include "cltypes.hpp"
#include "clpstack.hpp"
#include "cldstack.hpp"
#include "cltnform.hpp"
#include "cltitem.hpp"
#include "facide.hpp"


#ifndef __CLTNITEM_HPP__
#define __CLTNITEM_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempChannel ;
class	ClTempTi ;
class	ClTempNode ;
class	ClTempNitem ;
class	ClTempNitemConn ;
class	ClVMBaseEnv ;
class	ClTempTotal ;
class	ClTempTotalTag ;
class	ClTempTag ;
class	ClSliceArray ;
class	ClLongString ;
class	ClMdStatistics ;



// ritem =======================================================
// (referencia de un nitem)


class	ClTempRitem
{

protected:

// Serializacion
	m4uint32_t				m_iHandle ;
	ClStaticPoolStr			*m_poUpperPool ;
	ClStaticPoolStr			*m_poNoUpperPool ;

// fijos

	m4uint32_t				m_iName ;
    m4uint32_t              m_iConditionantsStart ;
    m4uint32_t              m_iAssignedStart ;
    m4uint32_t              m_iExecutedStart ;
    m4uint32_t              m_iFormIdsStart ;
    m4uint32_t              m_iFormulasStart ;

    m4uint8_t               m_iScope ;

// variables

    ClPointerStack          m_oConnectors ;
    ClPointerStack          m_oInvConnectors ;
    ClPointerStack          m_oConditionants ;
    ClPointerStack          m_oAssigned ;
    ClPointerStack          m_oExecuted ;
    ClPointerStack          m_oFormIds ;

    ClPointerStack          m_oFormulas ;

// Auxiliares

    ClPointerStack			m_oTotals ;
    ClPointerStack			m_oTotalTags ;
    ClPointerStack			m_oTags ;
    ClPointerStack			m_oDependents ;

	m4pcchar_t				m_pccName ;

	m4uint16_t			    m_iFormulaNumber ;
	m4uint16_t			    m_iSliceNumber ;

	ClTempNitemFormula		**m_ppoFormulaArray ;
	m4date_t				*m_pdDatesArray ;

    m4int16_t               m_iConditionantDegree ;
    m4uint16_t              m_iAfterMethods ;

    m4int16_t               m_iConnectedDegree ;

	m4bool_t				m_bIsDatabase ;

// client server
	m4uint16_t				m_iConnectorsToCompile ;


public:


// Funciones de inicialización
	
        ClTempRitem( void ) ;
		~ClTempRitem( void ) ;

    m4return_t	Delete( void ) ;


// Funciones de escritura

    void    SetScope( m4uint8_t ai_iScope )
    {
        m_iScope = ai_iScope ;
    }

	m4return_t	SetNameInPool( void )
	{
		return( M4MdFacSetPoolId( m_pccName, m_iName, m_poNoUpperPool ) ) ;
	}

    void	SetName( m4pcchar_t ai_pccName )
	{
		m_pccName = ai_pccName ;
	}

    void    SetConditionantDegree( m4int16_t ai_iConditionantDegree )
    {
        m_iConditionantDegree = ai_iConditionantDegree ;
    }

    void    SetConnectedDegree( m4int16_t ai_iConnectedDegree )
    {
        m_iConnectedDegree = ai_iConnectedDegree ;
    }

    m4return_t	AddConnector	( ClTempNitemConn		*ai_poConnector		) ;
    m4return_t	AddInvConnector	( ClTempNitemConn		*ai_poConnector		) ;
    m4return_t	AddConditionant	( ClTempNitem			*ai_poConditionant	) ;
    m4return_t	AddAssigned		( ClTempNitem			*ai_poAssigned		) ;
    m4return_t	AddExecuted		( ClTempNitem			*ai_poExecuted		) ;
    m4return_t	AddDependent	( ClTempNitem			*ai_poDependent		) ;

    m4return_t	AddFormula		( ClTempNitemFormula	*ai_poFormula		) ;
	m4return_t	AddTotal		( ClTempTotal			*ai_poTotal			) ;
	m4return_t	AddTotalTag		( ClTempTotalTag		*ai_poTotalTag		) ;
	m4return_t	AddTag			( ClTempTag				*ai_poTag			) ;

    
	void    SetConnectorsDelta( m4uint16_t ai_iDelta )
    {
        m_oConnectors.SetDelta( ai_iDelta ) ;
    }

	void    SetInvConnectorsDelta( m4uint16_t ai_iDelta )
    {
        m_oInvConnectors.SetDelta( ai_iDelta ) ;
    }

    void    SetConditionantDelta( m4uint16_t ai_iDelta )
    {
        m_oConditionants.SetDelta( ai_iDelta ) ;
    }
    
    void    SetAssignedDelta( m4uint16_t ai_iDelta )
    {
        m_oAssigned.SetDelta( ai_iDelta ) ;
    }
    
    void    SetExecutedDelta( m4uint16_t ai_iDelta )
    {
        m_oExecuted.SetDelta( ai_iDelta ) ;
    }

    void    SetDependentDelta( m4uint16_t ai_iDelta )
    {
        m_oDependents.SetDelta( ai_iDelta ) ;
    }

    void    AddConnectorsDelta( m4uint16_t ai_iDelta )
    {
        m_oConnectors.AddDelta( ai_iDelta ) ;
    }

    void    AddInvConnectorsDelta( m4uint16_t ai_iDelta )
    {
        m_oInvConnectors.AddDelta( ai_iDelta ) ;
    }

    void    AddConditionantDelta( m4uint16_t ai_iDelta )
    {
        m_oConditionants.AddDelta( ai_iDelta ) ;
    }

    void    AddAssignedDelta( m4uint16_t ai_iDelta )
    {
        m_oAssigned.AddDelta( ai_iDelta ) ;
    }

    void    AddExecutedDelta( m4uint16_t ai_iDelta )
    {
        m_oExecuted.AddDelta( ai_iDelta ) ;
    }

    void    AddDependentDelta( m4uint16_t ai_iDelta )
    {
        m_oDependents.AddDelta( ai_iDelta ) ;
    }

    void    IncrementConditionantDegree( void )
    {
        m_iConditionantDegree++ ;
    }

    void    DecrementConditionantDegree( void )
    {
        m_iConditionantDegree-- ;
    }

    void    IncrementConnectedDegree( void )
    {
        m_iConnectedDegree++ ;
    }

    void    DecrementConnectedDegree( void )
    {
        m_iConnectedDegree-- ;
    }


    void    SetFormulasDelta( m4uint16_t ai_iDelta )
    {
        m_oFormulas.SetDelta( ai_iDelta ) ;
    }

    void    AddFormulasDelta( m4uint16_t ai_iDelta )
    {
        m_oFormulas.AddDelta( ai_iDelta ) ;
    }

    void    AddTotalsDelta( m4uint16_t ai_iDelta )
    {
        m_oTotals.AddDelta( ai_iDelta ) ;
    }

    void    AddTotalTagsDelta( m4uint16_t ai_iDelta )
    {
        m_oTotalTags.AddDelta( ai_iDelta ) ;
    }

    void    AddTagsDelta( m4uint16_t ai_iDelta )
    {
        m_oTags.AddDelta( ai_iDelta ) ;
    }



// Funciones de lectura

    m4uint8_t   GetScope( void ) const
    {
        return( m_iScope ) ;
    }

    ClTempNitemConn*   GetInverseConnector( void ) const ;

    m4int16_t   GetConditionantDegree( void ) const
    {
        return( m_iConditionantDegree ) ;
    }

    m4uint16_t  GetAfterMethods( void ) const
    {
        return( m_iAfterMethods ) ;
    }

    m4int16_t   GetConnectedDegree( void ) const
    {
        return( m_iConnectedDegree ) ;
    }

	m4bool_t	GetIsDatabase( void ) const
	{
		return( m_bIsDatabase ) ;
	}

    m4uint16_t  GetNumberOfConnectors( void ) const
    {
        return( m_oConnectors.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfInvConnectors( void ) const
    {
        return( m_oInvConnectors.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfConditionants( void ) const
    {
        return( m_oConditionants.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfDependents( void ) const
    {
        return( m_oDependents.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfFormulas( void ) const
    {
        return( m_oFormulas.GetLength() ) ;
    }

    m4uint16_t	GetNumberOfTotals( void ) const
    {
        return( m_oTotals.GetLength() ) ;
    }

    m4uint16_t	GetNumberOfTotalTags( void ) const
    {
        return( m_oTotalTags.GetLength() ) ;
    }

    m4uint16_t	GetNumberOfTags( void ) const
    {
        return( m_oTags.GetLength() ) ;
    }


    m4return_t              GetFormulaOrderById( m4pcchar_t ai_pccRuleId, ClTempNitem *ai_poNitem, ClTempNode *ai_poNode, m4pcchar_t ai_pccThisFormula, ClTempNitem *ai_poThisNitem, m4uint8_t ai_iInheritance, m4uint16_t &ao_riOrder ) ;
    ClTempNitemConn*		GetConnectorBySonNitem( ClTempNitem *ai_poSonNitem ) ;

    ClTempNitemConn*		GetConnectorByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempNitemConn*		GetInvConnectorByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempNitem*            GetConditionantByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempNitem*            GetDependentByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempNitemFormula*     GetFormulaByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempTotal*			GetTotalByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempTotalTag*			GetTotalTagByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempTag*				GetTagByPosition			( m4uint16_t ai_iPosition ) ;



// Serialización
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
    m4return_t	GetSize   ( m4uint32_t &ao_iSize, m4uint8_t ai_iType, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile   ( ClTempChannel *ai_poChannel, ClTempNitem *ai_poNitem, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize ) ;

// client server

	void	SetConnectorsToCompile( m4uint16_t ai_iConnectors )
	{
		m_iConnectorsToCompile = ai_iConnectors ;
	}



// varias

    void	OrderConnectors( void ) ;

    m4return_t	OrderItemConnMappings( void ) ;
    m4return_t  ComputeConnectedNitemsScope( ClTempNitem *ai_poFatherNitem ) ;

	m4return_t	CalculateTotals		( ClTempNitem *ai_poNitem, ClSliceArray *ai_poSliceArray, ClLongString *ai_poPolish, ClDateStack *ai_poDateStack, ClTempNitem *ai_poBackPayNitem, m4date_t ai_dTimeUnit ) ;
	m4return_t	CalculateMetaRules	( ClTempNitem *ai_poNitem, ClSliceArray *ai_poSliceArray, ClLongString *ai_poPolish, ClDateStack *ai_poDateStack, ClTempNode *ai_poRevNode, ClTempNitem *ai_poRevNitem, m4date_t ai_dTimeUnit ) ;
	m4return_t	CalculateFormulas	( ClTempNitem *ai_poNitem, ClSliceArray *ai_poSliceArray, ClLongString *ai_poPolish, ClDateStack *ai_poDateStack ) ;

} ;



// nitem =======================================================


class	ClTempNitem : public ClTwoIdentifier
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;
	ClStaticPoolStr		*m_poNoUpperPool ;

// Fijos
    m4uint16_t			m_iSCRStart ;
    m4uint16_t			m_iIndex ;

	m4uint8_t			m_iFlags ;
    m4uint8_t			m_iCsType ;
    m4uint8_t			m_iSyncType ;
	m4uint8_t			m_iLevel ;

// Auxiliares

    ClTempItem			*m_poItem ;
    ClTempRitem			*m_poRitem ;
    ClTempNode			*m_poNode ;
    ClTempChannel		*m_poChannel ;

    m4uint8_t			m_iIsInGroup ;
    m4uint8_t			m_iHasBackPay ;
    m4uint8_t			m_iIsOverWritten ;
    m4uint8_t			m_iIsNameOverWritten ;

// client server

    m4uint8_t			m_iIsToCompile ;


public:

	m4pcchar_t	GetParentIdentifier( void ) const
	{
		return( GetNodeId() ) ;
	}

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetNitemId() ) ;
	}


// Funciones de inicialización
	
        ClTempNitem( void ) ;
		~ClTempNitem( void ) ;


// Funciones de escritura

	void    SetFlag( m4uint8_t ai_iFlag )
    {
	    m_iFlags |= ai_iFlag ;
    }

    void    UnSetFlag( m4uint8_t ai_iFlag )
    {
	    m_iFlags &= ~ai_iFlag ;
    }

	void    SetLevel( m4uint8_t ai_iLevel )
	{
		m_iLevel = ai_iLevel ;
	}

    void    SetSCRStart( m4uint16_t ai_iSCRStart )
    {
        m_iSCRStart = ai_iSCRStart ;
    }

    void    SetIndex( m4uint16_t ai_iIndex )
    {
        m_iIndex = ai_iIndex ;
    }

	m4return_t	SetItem( ClTempChannel *ai_poChannel, ClTempNode *ai_poNode, ClTempItem *ai_poItem, ClTempRitem *ai_poRitem ) ;

	void	SetHasBackPay( m4uint8_t ai_iHasBackPay )
	{
		m_iHasBackPay = ai_iHasBackPay ;
	}

	void	SetIsOverWritten( m4uint8_t ai_iIsOverWritten )
	{
		m_iIsOverWritten = ai_iIsOverWritten ;
	}

	void	SetIsNameOverWritten( m4uint8_t ai_iIsNameOverWritten )
	{
		m_iIsNameOverWritten = ai_iIsNameOverWritten ;
	}


// funciones de impresión

	m4pcchar_t	iItem( void ) const ;
	m4pcchar_t	nItem( void ) const ;
	m4pcchar_t	iNode( void ) const ;
	m4pcchar_t	nNode( void ) const ;
	m4pcchar_t	iM4Obj( void ) const ;
	m4pcchar_t	nM4Obj( void ) const ;


// Funciones de lectura

    m4uint8_t	GetFlag( m4uint8_t ai_iFlag ) const
    {
	    return( ( m_iFlags & ai_iFlag ) != 0 ) ;
    }

	m4uint8_t	GetLevel( void ) const
	{
		return( m_iLevel ) ;
	}

	m4pcchar_t	GetNitemId( void ) const
	{
		return( m_poItem->GetItemId() ) ;
	}

	m4uint32_t	GetNitemIdHandle( void ) const
	{
		return( m_poItem->GetItemIdHandle() ) ;
	}

    ClTempItem*    GetItem( void ) const
    {
        return( m_poItem ) ;
    }

    ClTempRitem*	GetRitem( void ) const
    {
        return( m_poRitem ) ;
    }

    ClTempNode*    GetNode( void ) const
    {
        return( m_poNode ) ;
    }

    ClTempChannel*	GetChannel( void ) const
    {
        return( m_poChannel ) ;
    }

    m4pcchar_t	GetNodeId( void ) const ;

    m4uint8_t    IsInGroup( void ) const
    {
        return( m_iIsInGroup ) ;
    }

	m4uint8_t	GetHasBackPay( void ) const
	{
		return( m_iHasBackPay ) ;
	}

	m4uint8_t	GetIsOverWritten( void ) const
	{
		return( m_iIsOverWritten ) ;
	}

	m4uint8_t	GetIsNameOverWritten( void ) const
	{
		return( m_iIsNameOverWritten ) ;
	}



// Serialización
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
	m4return_t	Compile   ( ClTempChannel *ai_poChannel, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize ) ;


// client server

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

    m4uint8_t	GetSyncType( void ) const
    {
        return( m_iSyncType ) ;
    }

    m4uint8_t	GetIsToCompile( void ) const
    {
        return( m_iIsToCompile ) ;
    }


// varias

	m4return_t	UnreferNitem( ClTempTi *ai_poSystemTi ) ;

} ;


#endif

