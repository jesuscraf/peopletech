
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltitem.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                12-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary item
//
//
//==============================================================================



#include "cltypes.hpp"
#include "clpstack.hpp"
#include "spoolstr.h"
#include "m4mdrtde.hpp"
#include "mdfacglb.hpp"
#include "facide.hpp"



#ifndef __CLTITEM_HPP__
#define __CLTITEM_HPP__


//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempArgument ;
class	ClTempItemFormula ;
class	ClTempTi ;
class	ClTempChannel ;
class	ClMdStatistics ;




//=============================================================================
//
// ClTempItem
//
// Clase que implementa la clase de los items temporales
//
//=============================================================================

class	ClTempItem : public ClTwoIdentifier
{

protected:

// Serializacion
	m4uint32_t				m_iHandle ;
	ClStaticPoolStr			*m_poUpperPool ;
	ClStaticPoolStr			*m_poNoUpperPool ;

// Fijos
	m4double_t			    m_dDefaultValue ;

    m4uint32_t			    m_iItemId ;
	m4int32_t			    m_iSynomym ;
	m4uint32_t			    m_iDmd ;
	m4uint32_t			    m_iDmdComponent ;
	m4uint32_t			    m_iDmdCrosstab ;
	m4uint32_t			    m_iDmdField ;
	m4uint32_t			    m_iDBPrecision ;
	m4int32_t			    m_iPrecision ;
	m4uint32_t			    m_iMaxSize ;
	m4uint32_t			    m_iReadObjectAlias ;
	m4uint32_t			    m_iWriteObjectAlias ;
	m4uint32_t			    m_iReadFieldId ;
	m4uint32_t			    m_iWriteFieldId ;
	m4uint32_t			    m_iReadObjectId ;
	m4uint32_t			    m_iWriteObjectId ;

	m4uint16_t			    m_iOrder ;
	m4uint16_t			    m_iFlags ;

    m4uint8_t			    m_iType ;
	m4uint8_t			    m_iScope ;
	m4uint8_t			    m_iM4Type ;
	m4uint8_t			    m_iCppType ;
    m4uint8_t			    m_iScale ;
	m4uint8_t			    m_iSliceBhrv ;
	m4uint8_t			    m_iSliceSplit ;
	m4uint8_t			    m_iIdTotalize ;
	m4uint8_t			    m_iIdSliceTotType ;
	m4uint8_t			    m_iTransactionMode ;
	m4uint8_t			    m_iMethodLevel ;
	m4uint8_t			    m_iSentTotType ;
	m4uint8_t			    m_iDefaultValueFlag ;
	m4uint8_t			    m_iInternalType ;
	m4uint8_t			    m_iConvertFunction ;
	m4uint8_t			    m_iModifyData ;

    m4uint16_t              m_iReferenceNumber ;

// Variables
    ClReferenceStack	    m_oArguments ;

// Auxiliares
	m4uint8_t			    m_iIsToInclude ;
	m4uint8_t			    m_iIsEvent ;

    m4pchar_t			    m_pcName ;
    m4pchar_t			    m_pcSliceItemId ;
    m4pchar_t			    m_pcAuxItemId ;
    m4pchar_t			    m_pcAuxItemProratId ;
    m4pchar_t			    m_pcGroupId ;
    m4pchar_t			    m_pcT3Id ;
    m4pchar_t			    m_pcNodeId ;

	ClTempTi				*m_poTi ;

// Client server
	m4uint8_t				m_iCsType ;
	m4uint8_t				m_iSyncType ;
    m4uint8_t               m_iIsToCompile ;

// Ordenación
    m4char_t				m_acTiId[ M4CL_MAX_TI_ID + 2 ] ;

public:


	m4pcchar_t	GetParentIdentifier( void ) const
	{
		return( GetTiId() ) ;
	}

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetItemId() ) ;
	}

// Funciones de inicialización ================================================
	
		ClTempItem( void ) ;

		~ClTempItem( void ) ;


// Funciones de lectura de la parte fija ======================================

	
	m4uint32_t	GetItemIdHandle( void )
	{
		return( m_iItemId ) ;
	}

	m4pcchar_t	GetItemId( void ) const
	{
		return( m_poUpperPool->GetString( m_iItemId ) ) ;
	}

    m4uint32_t	GetDmdHandle( void ) const
	{
		return( m_iDmd ) ;
	}

    m4uint32_t	GetDmdComponentHandle( void ) const
	{
		return( m_iDmdComponent ) ;
	}

    m4uint32_t	GetDmdFieldHandle( void ) const
	{
		return( m_iDmdField ) ;
	}
	
    m4pcchar_t	GetDmdId( void ) const
	{
		return( m_poUpperPool->GetString( m_iDmd ) ) ;
	}

    m4pcchar_t	GetDmdComponentId( void ) const
	{
		return( m_poUpperPool->GetString( m_iDmdComponent ) ) ;
	}

    m4pcchar_t	GetDmdFieldId( void ) const
	{
		return( m_poUpperPool->GetString( m_iDmdField ) ) ;
	}
	
	m4pcchar_t	GetReadObjectAlias( void ) const
	{
		return( m_poUpperPool->GetString( m_iReadObjectAlias ) ) ;
	}

	m4pcchar_t	GetReadFieldId( void ) const
	{
		return( m_poUpperPool->GetString( m_iReadFieldId ) ) ;
	}

	m4pcchar_t	GetReadObjectId( void ) const
	{
		return( m_poUpperPool->GetString( m_iReadObjectId ) ) ;
	}

    m4uint8_t	GetFlag( m4uint16_t ai_iFlag ) const
    {
	    return( ( m_iFlags & ai_iFlag ) != 0 ) ;
    }

    m4uint8_t   GetVariableArguments( void ) const
    {
        return( GetFlag( M4CL_MCR_ITEM_FLAG_HAS_VARIABLE_ARGS ) ) ;
    }

    m4uint8_t	GetIsParameter( void ) const
    {
        return( GetFlag( M4CL_MCR_ITEM_FLAG_IS_PARAMETER ) ) ;
    }

    m4uint8_t   GetType( void ) const
    {
        return( m_iType ) ;
    }

    m4uint8_t   GetScope( void ) const
    {
        return( m_iScope ) ;
    }

    m4uint8_t   GetCppType( void ) const
    {
        return( m_iCppType ) ;
    }

    m4uint8_t   GetMethodLevel( void ) const
    {
        return( m_iMethodLevel ) ;
    }

    m4uint8_t   GetInternalType( void ) const
    {
        return( m_iInternalType ) ;
    }


// Otras funciones de lectura =================================================

    m4uint8_t	GetIsToInclude( void ) const
    {
		return( m_iIsToInclude ) ;
    }

    m4uint8_t	GetIsEvent( void ) const
    {
		return( m_iIsEvent ) ;
    }

    m4pcchar_t	GetName( void ) const
	{
		if( m_pcName == NULL )
		{
			return( "" ) ;
		}
		return( m_pcName + 1 ) ;
	}

    m4pcchar_t	GetSliceItemId( void ) const
	{
		if( m_pcSliceItemId == NULL )
		{
			return( "" ) ;
		}
		return( m_pcSliceItemId + 1 ) ;
	}

    m4pcchar_t	GetAuxItemId( void ) const
	{
		if( m_pcAuxItemId == NULL )
		{
			return( "" ) ;
		}
		return( m_pcAuxItemId + 1 ) ;
	}

    m4pcchar_t	GetAuxItemProratId( void ) const
	{
		if( m_pcAuxItemProratId == NULL )
		{
			return( "" ) ;
		}
		return( m_pcAuxItemProratId + 1 ) ;
	}

    m4pchar_t	GetGroupId( void )
	{
		if( m_pcGroupId == NULL )
		{
			return( "" ) ;
		}
		return( m_pcGroupId + 1 ) ;
	}

    m4pchar_t	GetT3Id( void )
	{
		if( m_pcT3Id == NULL )
		{
			return( "" ) ;
		}
		return( m_pcT3Id + 1 ) ;
	}

    m4pchar_t	GetNodeId( void )
	{
		if( m_pcNodeId == NULL )
		{
			return( "" ) ;
		}
		return( m_pcNodeId + 1 ) ;
	}

    m4uint16_t  GetNumberOfArguments( void ) const
    {
        return( m_oArguments.GetLength() ) ;
    }

    m4uint16_t  GetNumberOfReferedArguments( void ) const
    {
        return( m_iReferenceNumber ) ;
    }

    ClTempArgument*     GetArgumentById( m4pcchar_t ai_pccArgumentId ) ;
    ClTempArgument*     GetArgumentByPosition( m4uint16_t ai_iPosition ) ;


// Funciones de escritura de parte fija =======================================

	m4return_t	SetDefaultValue( m4pcchar_t ai_pccDefaultValue, ClTempChannel* ai_poChannel ) ;

	m4return_t	SetItemId( m4pcchar_t ai_pccItemId )
	{
		return( M4MdFacSetPoolId( ai_pccItemId, m_iItemId, m_poUpperPool ) ) ;
	}

	void	SetSynonym( m4uint32_t ai_iSynonym )
	{
		m_iSynomym = ai_iSynonym ;
	}

	m4return_t	SetDmd( m4pcchar_t ai_pccDmd )
	{
		return( M4MdFacSetPoolId( ai_pccDmd, m_iDmd, m_poUpperPool ) ) ;
	}

	m4return_t	SetDmdComponent( m4pcchar_t ai_pccDmdComponent )
	{
		return( M4MdFacSetPoolId( ai_pccDmdComponent, m_iDmdComponent, m_poUpperPool ) ) ;
	}

	m4return_t	SetDmdCrosstab( m4pcchar_t ai_pccDmdCrosstab )
	{
		return( M4MdFacSetPoolId( ai_pccDmdCrosstab, m_iDmdCrosstab, m_poUpperPool ) ) ;
	}

	m4return_t	SetDmdField( m4pcchar_t ai_pccDmdField )
	{
		return( M4MdFacSetPoolId( ai_pccDmdField, m_iDmdField, m_poUpperPool ) ) ;
	}

    void    SetDBPrecision( m4uint32_t ai_iDBPrecision )
    {
	    m_iDBPrecision = ai_iDBPrecision ;
    }

	m4return_t	SetReadObjectAlias( m4pcchar_t ai_pccReadObjectAlias )
	{
		return( M4MdFacSetPoolId( ai_pccReadObjectAlias, m_iReadObjectAlias, m_poUpperPool ) ) ;
	}

	m4return_t	SetWriteObjectAlias( m4pcchar_t ai_pccWriteObjectAlias )
	{
		return( M4MdFacSetPoolId( ai_pccWriteObjectAlias, m_iWriteObjectAlias, m_poUpperPool ) ) ;
	}

	m4return_t	SetReadFieldId( m4pcchar_t ai_pccReadFieldId )
	{
		return( M4MdFacSetPoolId( ai_pccReadFieldId, m_iReadFieldId, m_poUpperPool ) ) ;
	}

	m4return_t	SetWriteFieldId( m4pcchar_t ai_pccWriteFieldId )
	{
		return( M4MdFacSetPoolId( ai_pccWriteFieldId, m_iWriteFieldId, m_poUpperPool ) ) ;
	}

	m4return_t	SetReadObjectId( m4pcchar_t ai_pccReadObjectId )
	{
		return( M4MdFacSetPoolId( ai_pccReadObjectId, m_iReadObjectId, m_poUpperPool ) ) ;
	}

	m4return_t	SetWriteObjectId( m4pcchar_t ai_pccWriteObjectId )
	{
		return( M4MdFacSetPoolId( ai_pccWriteObjectId, m_iWriteObjectId, m_poUpperPool ) ) ;
	}

    void    SetOrder( m4uint16_t ai_iOrder )
    {
	    m_iOrder = ai_iOrder ;
    }

	void    SetFlag( m4uint16_t ai_iFlag )
    {
	    m_iFlags |= ai_iFlag ;
    }

    void    UnSetFlag( m4uint16_t ai_iFlag )
    {
	    m_iFlags &= ~ai_iFlag ;
    }

    void    SetType( m4uint8_t ai_iType )
    {
	    m_iType = ai_iType ;
    }

    void    SetScope( m4uint8_t ai_iScope )
    {
	    m_iScope = ai_iScope ;
    }

    void    SetM4Type( m4uint8_t ai_iM4Type )
    {
	    m_iM4Type = ai_iM4Type ;
    }

    m4return_t	SetCppType( ClTempChannel* ai_poChannel ) ;

    void    SetScale( m4uint8_t ai_iScale )
    {
	    m_iScale = ai_iScale ;
    }

    void    SetSliceBhrv( m4uint8_t ai_iSliceBhrv ) ;

    void    SetSliceSplit( m4uint8_t ai_iSliceSplit )
    {
	    m_iSliceSplit = ai_iSliceSplit ;
    }

    void    SetIdTotalize( m4uint8_t ai_iIdTotalize )
    {
	    m_iIdTotalize = ai_iIdTotalize ;
    }

    void    SetIdSliceTotType( m4uint8_t ai_iIdSliceTotType )
    {
	    m_iIdSliceTotType = ai_iIdSliceTotType ;
    }

    void    SetTransactionMode( m4uint8_t ai_iTransactionMode )
    {
	    m_iTransactionMode = ai_iTransactionMode ;
    }

    void    SetMethodLevel( m4uint8_t ai_iMethodLevel )
    {
        if( ai_iMethodLevel > 0 )
        {
	        m_iMethodLevel = ai_iMethodLevel - 1 ;
        }
        else
        {
	        m_iMethodLevel = ai_iMethodLevel ;
        }
    }

    void    SetSentTotType( m4uint8_t ai_iSentTotType )
    {
	    m_iSentTotType = ai_iSentTotType ;
    }

    void    SetInternalType( m4uint8_t ai_iInternalType )
    {
	    m_iInternalType = ai_iInternalType ;
    }

    void    SetConvertFunction( m4uint8_t ai_iConvertFunction )
    {
	    m_iConvertFunction = ai_iConvertFunction ;
    }

    void    SetModifyData( m4uint8_t ai_iModifyData )
    {
	    m_iModifyData = ai_iModifyData ;
    }


// Otras funciones de escritura ===============================================

    void    SetIsToInclude( m4uint8_t ai_iIsToInclude )
    {
	    m_iIsToInclude = ai_iIsToInclude ;
    }

    void    SetIsEvent( m4uint8_t ai_iIsEvent )
    {
	    m_iIsEvent = ai_iIsEvent ;
    }

    m4return_t  SetName( m4pcchar_t ai_pccName)
	{
		return( M4MdFacSetLocalId( ai_pccName, m_pcName, M4_FALSE ) ) ;
	}

    m4return_t  SetSliceItem( m4pcchar_t ai_pccSliceItem, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccSliceItem, m_pcSliceItemId, ai_bConvert ) ) ;
	}

    m4return_t  SetAuxItem( m4pcchar_t ai_pccAuxItem, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccAuxItem, m_pcAuxItemId, ai_bConvert ) ) ;
	}

    m4return_t  SetAuxItemProrat( m4pcchar_t ai_pccAuxItemProrat, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccAuxItemProrat, m_pcAuxItemProratId, ai_bConvert ) ) ;
	}

   	m4return_t	SetGroup( m4pcchar_t ai_pccGroup, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccGroup, m_pcGroupId, ai_bConvert ) ) ;
	}

   	m4return_t	SetT3Id( m4pcchar_t ai_pccT3Id, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccT3Id, m_pcT3Id, ai_bConvert ) ) ;
	}

   	m4return_t	SetNodeId( m4pcchar_t ai_pccNodeId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccNodeId, m_pcNodeId, ai_bConvert ) ) ;
	}

	void	SetTi( ClTempTi *ai_poTi )
	{
		m_poTi = ai_poTi ;
	}


// Funciones de impresión =====================================================

	m4pcchar_t	iItem( void ) const ;
	m4pcchar_t	nItem( void ) const ;


// Funciones de referencia ====================================================

    void	AddArgument( ClTempArgument *ai_poItemArgument ) ;

	void	InitArguments( ClTempArgument **ai_ppoArguments )
	{
		m_oArguments.Init( ( m4pvoid_t* ) ai_ppoArguments ) ;
	}


// Funciones de serialización =================================================

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

    m4return_t	GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize ) ;


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

    m4uint8_t	GetSyncType( void ) const
    {
        return( m_iSyncType ) ;
    }

    m4uint8_t	GetIsToCompile( void ) const
    {
        return( m_iIsToCompile ) ;
    }

// Funciones de ordenación ====================================================

	m4pcchar_t	GetTiId( void ) const
	{
		return( m_acTiId + 1 ) ;
	}

	m4return_t	SetTiId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccTiId, m_acTiId, M4CL_MAX_TI_ID, ai_bConvert ) ) ;
	}

} ;


#endif

