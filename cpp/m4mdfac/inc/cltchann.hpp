
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltmcr.hpp   
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
//    This module defines the temporary metachanel in RAM
//
//
//==============================================================================



#include "facide.hpp"
#include "clpstack.hpp"
#include "spoolstr.h"



#ifndef _CLTCHANN_HPP_
#define _CLTCHANN_HPP_



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class   ClFactoryImp ;
class	ClTempT3Inherit ;
class	ClTempSentence ;
class   ClTempRSM ;
class	ClTempTi ;
class	ClTempNode ;
class	ClTempNodeInherit ;
class	ClTempTiInherit ;
class	ClTempTiReference ;
class	ClTempConcept ;
class	ClTempTotal ;
class	ClTempTotalTag ;
class	ClTempTag ;
class	ClTempTrigger ;
class	ClTempInstance ;
class	ClTempItem ;
class	ClTempItemInherit ;
class	ClTempArgument ;
class	ClTempNitem ;
class	ClTempItemFormula ;
class	ClTempConnector ;
class	ClTempNitemConn ;
class	ClTempIndex ;
class	ClTempIndexItem ;
class	ClTempPlug ;
class	ClTempPlugItem ;
class	ClTempAlias ;
class	ClTempT3Alias ;
class	ClTempFilter ;
class	ClTempBusinessArg ;
class	ClTempBusinessMethod ;
class	ClMdStatistics ;
class	M4HsParser ;
class	ClLongString;



//=================================================================================
//
// ClTempChannel
//
// Clase que implementa la definición de los M4Objs
//
//=================================================================================

class	ClTempChannel
{

protected:

// Serializacion
	m4uint32_t				m_iHandle ;
	ClStaticPoolStr			*m_poUpperPool ;
	ClStaticPoolStr			*m_poNoUpperPool ;

// Fijos
    m4uint32_t			    m_iSize ;
    m4uint16_t			    m_iBuild ;
    m4uint8_t			    m_iTimeUnit ;
    m4uint8_t			    m_iCsType ;
    m4uint8_t			    m_iLanguage ;
	m4date_t			    m_dVersion ;
	m4date_t			    m_dStartDate ;
	m4date_t			    m_dEndDate ;
	m4date_t			    m_dCorStartDate ;
	m4date_t			    m_dCorEndDate ;
	m4date_t			    m_dCreationDate ;
	m4date_t			    m_dCacheMaxPeriod ;
    m4uint32_t			    m_iChannelId ;
	m4uint32_t			    m_iName ;
	m4uint32_t			    m_iPool ;
	m4uint32_t			    m_iOleClassId ;
	m4uint32_t			    m_iServiceId ;
	m4uint32_t			    m_iServiceAltId ;
	m4uint32_t			    m_iEnvironmentVars ;
	m4uint32_t			    m_iOwnerFlag ;
	m4uint32_t			    m_iRootsStart ;
	m4uint32_t			    m_iSentencesStart ;
	m4uint32_t			    m_iInstancesStart ;
	m4uint32_t			    m_iSystemItemsStart ;
	m4uint32_t			    m_iTisStart ;
	m4uint32_t			    m_iParametersStart ;
	m4uint32_t			    m_iRSMsStart ;
	m4uint32_t			    m_iBusinessMethodsStart ;

	m4uint16_t			    m_iSecurity ;
	m4uint16_t			    m_iVMCapacityReq ;

	m4uint8_t			    m_iType ;
	m4uint8_t			    m_iIsSeparable ;
	m4uint8_t			    m_iCsExeType ;
	m4uint8_t			    m_iIsExternal ;
	m4uint8_t			    m_iHasSecurity ;
	m4uint8_t			    m_iIsCacheable ;
	m4uint8_t			    m_iIsMDCacheable ;
	m4uint8_t			    m_iCheckConcurrency ;
	m4uint8_t			    m_iOrganizationType ;
	m4uint8_t			    m_iDataLanguage ;
	m4uint8_t			    m_iMetaLanguage ;
	m4uint8_t			    m_iCreationType ;
	m4uint8_t			    m_iStyleUse ;
	m4uint8_t			    m_iWSSSecurity ;

// Componentes
    ClPointerStack          m_oRoots ;

// Variables
    ClPointerStack          m_oT3Inherits ;
    ClPointerStack          m_oSentences ;
    ClPointerStack          m_oRSMs ;
    ClPointerStack          m_oTis ;
    ClPointerStack          m_oTiInherits ;
    ClPointerStack          m_oItems ;
    ClPointerStack          m_oItemInherits ;
    ClPointerStack          m_oConcepts ;
    ClPointerStack          m_oArguments ;
    ClPointerStack          m_oItemFormulas ;
    ClPointerStack          m_oTriggers ;
    ClPointerStack          m_oTotals ;
    ClPointerStack          m_oTotalTags ;
    ClPointerStack          m_oTags ;
	ClPointerStack		    m_oNodes ;
	ClPointerStack		    m_oNodeInherits ;
	ClPointerStack		    m_oNitems ;
	ClPointerStack		    m_oConnectors ;
	ClPointerStack		    m_oInvConnectors ;
    ClPointerStack          m_oNitemConns ;
    ClPointerStack          m_oIndexes ;
    ClPointerStack          m_oIndexItems ;
    ClPointerStack          m_oPlugs ;
    ClPointerStack          m_oPlugItems ;
    ClPointerStack          m_oAlias ;
    ClPointerStack          m_oT3Alias ;
    ClPointerStack          m_oFilters ;
    ClPointerStack          m_oInstances ;
    ClPointerStack          m_oParameters ;
    ClPointerStack          m_oBusinessMethods ;
    ClPointerStack          m_oBusinessArgs ;
    ClPointerStack          m_oSystemArguments ;


    ClTempNitem             *m_poSysLoad ;
    ClTempNitem             *m_poSysPersist ;

    ClTempNitem             *m_poClientSysLoad ;
    ClTempNitem             *m_poClientSysPersist ;
    ClTempNitem             *m_poServerSysLoad ;
    ClTempNitem             *m_poServerSysPersist ;

// Auxiliares
    m4uint8_t			    m_iDefaultLanguage ;

	m4date_t			    m_dUserStartDate ;
	m4date_t			    m_dUserEndDate ;

    ClTempTi                *m_poSystemTi ;
    ClTempNode              *m_poSystemNode ;

    ClFactoryImp			*m_poFactory ;

    ClCompNumericId			*m_poCompT3Inherit ;
    ClCompNumericId			m_oCompT3Inherit ;

    ClCompIdentifier		*m_poComparisonTi ;
    ClCompIdentifier		m_oComparisonTi ;

    ClCompTwoIdentifier		m_oComparisonNode ;
    ClCompTwoIdentifier		*m_poComparisonNode ;

    ClCompTwoIdentifier		m_oComparisonItem ;
    ClCompTwoIdentifier		*m_poComparisonItem ;

    ClCompIdentifier		*m_poComparisonSentence ;
    ClCompIdentifier		m_oComparisonSentence ;

    ClCompIdentifier	    *m_poComparisonInstance ;
    ClCompIdentifier	    m_oComparisonInstance ;

// Client server
	m4uint16_t				m_iNodesToCompile ;
	m4uint16_t				m_iTisToCompile ;
	m4uint16_t				m_iSentencesToCompile ;
	m4uint16_t				m_iParametersToCompile ;

// Memoria alojada
	m4uint16_t				m_iNitemConnsSize ;

	ClTempT3Inherit			*m_poT3InheritsArray ;
	ClTempSentence			*m_poSentencesArray ;
	ClTempRSM				*m_poRSMsArray ;
	ClTempTi				*m_poTisArray ;
	ClTempTiInherit			*m_poTiInheritsArray ;
	ClTempTiReference		*m_poTiReferencesArray ;
	ClTempItem				*m_poItemsArray ;
	ClTempItemInherit		*m_poItemInheritsArray ;
	ClTempConcept			*m_poConceptsArray ;
	ClTempArgument			*m_poArgumentsArray ;
	ClTempItemFormula		*m_poItemFormulasArray ;
	ClTempTrigger			*m_poTriggersArray ;
	ClTempTotal				*m_poTotalsArray ;
	ClTempTotalTag			*m_poTotalTagsArray ;
	ClTempTag				*m_poTagsArray ;
	ClTempNode				*m_poNodesArray ;
	ClTempNodeInherit		*m_poNodeInheritsArray ;
	ClTempNitem				*m_poNitemsArray ;
	ClTempConnector			*m_poConnectorsArray ;
	ClTempNitemConn			*m_poNitemConnsArray ;
	ClTempIndex				*m_poIndexesArray ;
	ClTempIndexItem			*m_poIndexItemsArray ;
	ClTempPlug				*m_poPlugsArray ;
	ClTempPlugItem			*m_poPlugItemsArray ;
	ClTempAlias				*m_poAliasArray ;
	ClTempT3Alias			*m_poT3AliasArray ;
	ClTempFilter			*m_poFiltersArray ;
	ClTempInstance			*m_poInstancesArray ;
	ClTempBusinessMethod	*m_poBusinessMethodsArray ;
	ClTempBusinessArg		*m_poBusinessArgsArray ;

	ClTempItem				*m_poClientSysLoadItem ;
	ClTempItem				*m_poClientSysPersistItem ;
	ClTempItem				*m_poServerSysLoadItem ;
	ClTempItem				*m_poServerSysPersistItem ;
	ClTempItem				*m_poExecuteReportItem ;
	ClTempItem				*m_poDynFilterItem ;

	ClTempArgument			*m_poSystemArgumentsArray ;

// Herencia de sentencias;
	M4HsParser				*m_poHsParser;

// Depuración
	m4uint8_t				m_iPrint ;

// Auxiliares
	m4uint16_t				m_iNoQBFRoots ;


    ClTempT3Inherit*		_GetT3InheritById( m4pcchar_t ai_pccT3Id ) ;
	ClTempItem*				_CreateSystemItem( m4pcchar_t ai_pccItemId, m4uint8_t ai_iType, m4uint8_t ai_iScope, m4uint8_t ai_iM4Type, m4uint32_t ai_iDBPrecision, m4uint8_t ai_iScale, m4uint8_t ai_iInternalType, m4uint8_t ai_CsType, m4uint8_t ai_iTransactionMode ) ;
	m4return_t				_AddSystemItem( ClTempItem *ai_poSystemItem, ClTempNitem *ai_poSystemNitem, ClTempNode *ai_poNode, m4bool_t ai_bAddFormula, m4int8_t ai_iType, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccMethod, m4uint16_t ai_iArgumentStart, m4uint16_t ai_iArgumentNumber, m4uint8_t ai_iCreationType ) ;
	m4return_t				_CreateSystemArguments( void ) ;

public:


// Funciones de inicialización ================================================
	
		ClTempChannel( void ) ;
		~ClTempChannel( void ) ;

	m4return_t	Delete( void ) ;


// Funciones de lectura de la parte fija ======================================

	m4uint8_t	GetCsType( void ) const
	{
		return( m_iCsType ) ;
	}
	
	m4uint8_t	GetLanguage ( void ) const
	{
		return( m_iLanguage ) ;
	}
	
	m4uint8_t	GetDefaultLanguage ( void ) const
	{
		return( m_iDefaultLanguage ) ;
	}
	
	m4uint32_t	GetChannelIdHandle( void ) const
	{
		return( m_iChannelId ) ;
	}

    m4date_t    GetCreationDate( void ) const
    {
        return( m_dCreationDate ) ;
    }

    m4date_t    GetStartDate( void ) const
    {
        return( m_dStartDate ) ;
    }

    m4date_t    GetEndDate( void ) const
    {
        return( m_dEndDate ) ;
    }

    m4date_t    GetUserStartDate( void ) const
    {
        return( m_dUserStartDate ) ;
    }

    m4date_t    GetUserEndDate( void ) const
    {
        return( m_dUserEndDate ) ;
    }

    m4date_t    GetCorStartDate( void ) const
    {
        return( m_dCorStartDate ) ;
    }

    m4date_t    GetCorEndDate( void ) const
    {
        return( m_dCorEndDate ) ;
    }

	m4pcchar_t	GetChannelId( void ) const
	{
		return( m_poUpperPool->GetString( m_iChannelId ) ) ;
	}

	m4pcchar_t	GetName( void ) const
	{
		return( m_poNoUpperPool->GetString( m_iName ) ) ;
	}

	m4uint8_t	GetIsSeparable( void ) const
	{
        return( m_iIsSeparable ) ;
    }

	m4uint8_t	GetCsExeType( void ) const
	{
        return( m_iCsExeType ) ;
    }

	m4uint8_t	GetDataLanguage( void ) const
	{
        return( m_iDataLanguage ) ;
    }

	m4uint8_t	GetMetaLanguage( void ) const
	{
        return( m_iMetaLanguage ) ;
    }

	m4uint8_t	GetCreationType( void ) const
	{
        return( m_iCreationType ) ;
    }


// Otras funciones de lectura ================================================

    ClFactoryImp*	GetFactory( void ) const
    {
        return( m_poFactory ) ;
    }

	ClTempTi*	GetSystemTi( void ) const
	{
		return( m_poSystemTi ) ;
	}

    m4uint16_t	GetNumberOfT3Inherits( void ) const
    {
        return( m_oT3Inherits.GetLength() ) ;
    }

    m4uint16_t	GetNumberOfTis( void ) const
    {
        return( m_oTis.GetLength() ) ;
    }

    m4uint16_t	GetNumberOfNodes( void ) const
    {
        return( m_oNodes.GetLength() ) ;
    }

    m4uint16_t	GetNumberOfNitems( void ) const
    {
        return( m_oNitems.GetLength() ) ;
    }
	
    m4uint16_t	GetNumberOfTriggers( void ) const
    {
        return( m_oTriggers.GetLength() ) ;
    }
	
    m4uint16_t	GetNumberOfConnectors( void ) const
    {
        return( m_oConnectors.GetLength() ) ;
    }
	
    m4uint16_t	GetNumberOfInvConnectors( void ) const
    {
        return( m_oInvConnectors.GetLength() ) ;
    }
	
    ClTempT3Inherit*		GetT3InheritByLevel( m4uint16_t ai_iLevel ) ;

    ClTempSentence*			GetSentenceById ( m4pcchar_t ai_pccSentenceId, m4bool_t ai_bConvert ) ;
    ClTempTi*				GetTiById		( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert ) ;
    ClTempItem*				GetItemByTiAndId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvertTi, m4pcchar_t ai_pccItemId, m4bool_t ai_bConvertItem ) ;
    ClTempNode*				GetNodeById		( m4pcchar_t ai_pccNodeId, m4bool_t ai_bConvert ) ;
    ClTempNode*				GetNodeByTiId	( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert, m4uint8_t ai_iLevel, ClTempNitem* ai_poNitem ) ;
    ClTempInstance*			GetInstanceById	( m4pcchar_t ai_pccInstanceId, m4bool_t ai_bConvert ) ;

    ClTempT3Inherit*		GetT3InheritByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempTi*				GetTiByPosition				( m4uint16_t ai_iPosition ) ;
    ClTempItem*				GetItemByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempNode*				GetNodeByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempNitem*			GetNitemByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempIndex*			GetIndexByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempPlug*				GetPlugByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempBusinessMethod*	GetBusinessMethodByPosition	( m4uint16_t ai_iPosition ) ;
	
	ClTempNodeInherit**		GetNodeInheritPointerByPosition		( m4uint16_t ai_iPosition ) ;
	ClTempTiInherit**		GetTiInheritPointerByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempItem**			GetItemPointerByPosition			( m4uint16_t ai_iPosition ) ;
	ClTempItemInherit**		GetItemInheritPointerByPosition		( m4uint16_t ai_iPosition ) ;
	ClTempConcept**			GetConceptPointerByPosition			( m4uint16_t ai_iPosition ) ;
	ClTempArgument**		GetArgumentPointerByPosition		( m4uint16_t ai_iPosition ) ;
	ClTempItemFormula**		GetItemFormulaPointerByPosition		( m4uint16_t ai_iPosition ) ;
	ClTempTrigger**			GetTriggerPointerByPosition			( m4uint16_t ai_iPosition ) ;
	ClTempTotal**			GetTotalPointerByPosition			( m4uint16_t ai_iPosition ) ;
	ClTempTotalTag**		GetTotalTagPointerByPosition		( m4uint16_t ai_iPosition ) ;
	ClTempTag**				GetTagPointerByPosition				( m4uint16_t ai_iPosition ) ;
	ClTempNitem**			GetNitemPointerByPosition			( m4uint16_t ai_iPosition ) ;
	ClTempConnector**		GetConnectorPointerByPosition		( m4uint16_t ai_iPosition ) ;
	ClTempConnector**		GetInvConnectorPointerByPosition	( m4uint16_t ai_iPosition ) ;
    ClTempNitemConn**		GetNitemConnPointerByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempIndex**			GetIndexPointerByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempIndexItem**		GetIndexItemPointerByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempPlug**			GetPlugPointerByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempPlugItem**		GetPlugItemPointerByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempAlias**			GetAliasPointerByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempT3Alias**			GetT3AliasPointerByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempFilter**			GetFilterPointerByPosition			( m4uint16_t ai_iPosition ) ;
    ClTempBusinessArg**		GetBusinessArgPointerByPosition		( m4uint16_t ai_iPosition ) ;
    ClTempArgument**		GetSystemArgumentPointerByPosition	( m4uint16_t ai_iPosition ) ;


// Funciones de escritura de parte fija =======================================

    void    SetSize( m4uint32_t ai_iSize )
    {
	    m_iSize = ai_iSize ;
    }

    void    SetBuild( m4uint16_t ai_iBuild )
    {
	    m_iBuild = ai_iBuild ;
    }

    void    SetTimeUnit( m4uint8_t ai_iTimeUnit )
    {
	    m_iTimeUnit = ai_iTimeUnit ;
    }

    void    SetCsType( m4uint8_t ai_iCsType )
    {
	    m_iCsType = ai_iCsType ;
    }

    void    SetLanguage( m4uint8_t ai_iLanguage )
    {
	    m_iLanguage = ai_iLanguage ;
    }
	
    void    SetDefaultLanguage( m4uint8_t ai_iDefaultLanguage )
    {
	    m_iDefaultLanguage = ai_iDefaultLanguage ;
    }
	
    void    SetVersion( m4date_t a_dVersion )
    {
        if( a_dVersion > m_dVersion )
        {
            m_dVersion = a_dVersion ;
        }
    }

    void    SetStartDate( m4date_t ai_dStartDate )
    {
	    m_dStartDate = ai_dStartDate ;
    }

    void    SetEndDate( m4date_t ai_dEndDate )
    {
	    m_dEndDate = ai_dEndDate ;
    }

    void    SetUserStartDate( m4date_t ai_dUserStartDate )
    {
	    m_dUserStartDate = ai_dUserStartDate ;
    }

    void    SetUserEndDate( m4date_t ai_dUserEndDate )
    {
	    m_dUserEndDate = ai_dUserEndDate ;
    }

    void    SetCorStartDate( m4date_t ai_dCorStartDate )
    {
	    m_dCorStartDate = ai_dCorStartDate ;
    }

    void    SetCorEndDate( m4date_t ai_dCorEndDate )
    {
	    m_dCorEndDate = ai_dCorEndDate ;
    }

    void    SetCacheMaxPeriod( m4date_t ai_dCacheMaxPeriod )
    {
	    m_dCacheMaxPeriod = ai_dCacheMaxPeriod ;
    }

	m4return_t	SetChannelId( m4pcchar_t ai_pccChannelId )
	{
		return( M4MdFacSetPoolId( ai_pccChannelId, m_iChannelId, m_poUpperPool ) ) ;
	}

	m4return_t	SetName( m4pcchar_t ai_pccName )
	{
		return( M4MdFacSetPoolId( ai_pccName, m_iName, m_poNoUpperPool ) ) ;
	}

	m4return_t	SetOleClassId( m4pcchar_t ai_pccOleClassId )
	{
		return( M4MdFacSetPoolId( ai_pccOleClassId, m_iOleClassId, m_poUpperPool ) ) ;
	}

	m4return_t	SetServiceId( m4pcchar_t ai_pccServiceId )
	{
		return( M4MdFacSetPoolId( ai_pccServiceId, m_iServiceId, m_poUpperPool ) ) ;
	}

	m4return_t	SetServiceAltId( m4pcchar_t ai_pccServiceAltId )
	{
		return( M4MdFacSetPoolId( ai_pccServiceAltId, m_iServiceAltId, m_poUpperPool ) ) ;
	}

    void    SetEnvironmentVars( m4uint32_t ai_iEnvironmentVars )
    {
	    m_iEnvironmentVars = ai_iEnvironmentVars ;
    }

    void    SetOwnerFlag( m4uint32_t ai_iOwnerFlag )
    {
	    m_iOwnerFlag = ai_iOwnerFlag ;
    }

    void    SetPool( m4uint32_t ai_iPool )
    {
	    m_iPool = ai_iPool ;
    }

    void    SetVMCapacityReq( m4uint16_t ai_iVMCapacityReq )
    {
        m_iVMCapacityReq = ai_iVMCapacityReq ;
    }

    void    SetType( m4uint8_t ai_iType )
    {
        m_iType = ai_iType ;
    }

    void    SetIsCacheable( m4uint8_t ai_iIsCacheable )
    {
        m_iIsCacheable = ai_iIsCacheable ;
    }
	
    void    SetCsExeType( m4uint8_t ai_iCsExeType )
    {
        m_iCsExeType = ai_iCsExeType ;
    }

    void    SetIsExternal( m4uint8_t ai_iIsExternal )
    {
        m_iIsExternal = ai_iIsExternal ;
    }

    void    SetHasSecurity( m4uint8_t ai_iHasSecurity )
    {
        m_iHasSecurity = ai_iHasSecurity ;
    }

    void    SetIsSeparable( m4uint8_t ai_iIsSeparable )
    {
        m_iIsSeparable = ai_iIsSeparable ;
    }

    void    SetIsMDCacheable( m4uint8_t ai_iIsMDCacheable )
    {
        m_iIsMDCacheable = ai_iIsMDCacheable ;
    }

    void    SetCheckConcurrency( m4uint8_t ai_iCheckConcurrency )
    {
        m_iCheckConcurrency = ai_iCheckConcurrency ;
    }

    void    SetOrganizationType( m4uint8_t ai_iOrganizationType )
    {
        m_iOrganizationType = ai_iOrganizationType ;
    }

    void    SetDataLanguage( m4uint8_t ai_iDataLanguage )
    {
        m_iDataLanguage = ai_iDataLanguage ;
    }

    void    SetMetaLanguage( m4uint8_t ai_iMetaLanguage )
    {
        m_iMetaLanguage = ai_iMetaLanguage ;
    }

    void    SetCreationType( m4uint8_t ai_iCreationType )
    {
        m_iCreationType = ai_iCreationType ;
    }

    void    SetStyleUse( m4uint8_t ai_iStyleUse )
    {
        m_iStyleUse = ai_iStyleUse ;
    }

    void    SetWSSSecurity( m4uint8_t ai_iWSSSecurity )
    {
        m_iWSSSecurity = ai_iWSSSecurity ;
    }


// Otras funciones de escritura ===============================================

    void    SetFactory( ClFactoryImp* ai_poFactory )
    {
        m_poFactory = ai_poFactory ;
    }

    void    SetSystemTi( ClTempTi* ai_poSystemTi )
    {
        m_poSystemTi = ai_poSystemTi ;
    }

    void    SetSystemNode( ClTempNode* ai_poSystemNode )
    {
        m_poSystemNode = ai_poSystemNode ;
    }

    void    SetT3InheritsDelta( m4uint16_t ai_iDelta )
    {
        m_oT3Inherits.SetDelta( ai_iDelta ) ;
    }
   
    void    SetSentencesDelta( m4uint16_t ai_iDelta )
    {
        m_oSentences.SetDelta( ai_iDelta ) ;
    }
   
    void    SetRSMsDelta( m4uint16_t ai_iDelta )
    {
        m_oRSMs.SetDelta( ai_iDelta ) ;
    }
   
    void    SetTisDelta( m4uint16_t ai_iDelta )
    {
        m_oTis.SetDelta( ai_iDelta ) ;
    }

    void    SetTiInheritsDelta( m4uint16_t ai_iDelta )
    {
        m_oTiInherits.SetDelta( ai_iDelta ) ;
    }

    void    SetItemsDelta( m4uint16_t ai_iDelta )
    {
        m_oItems.SetDelta( ai_iDelta ) ;
    }

    void    SetItemInheritsDelta( m4uint16_t ai_iDelta )
    {
        m_oItemInherits.SetDelta( ai_iDelta ) ;
    }

    void    SetConceptsDelta( m4uint16_t ai_iDelta )
    {
        m_oConcepts.SetDelta( ai_iDelta ) ;
    }

    void    SetArgumentsDelta( m4uint16_t ai_iDelta )
    {
        m_oArguments.SetDelta( ai_iDelta ) ;
    }

    void    SetItemFormulasDelta( m4uint16_t ai_iDelta )
    {
        m_oItemFormulas.SetDelta( ai_iDelta ) ;
    }

    void    SetTriggersDelta( m4uint16_t ai_iDelta )
    {
        m_oTriggers.SetDelta( ai_iDelta ) ;
    }

    void    SetTotalsDelta( m4uint16_t ai_iDelta )
    {
        m_oTotals.SetDelta( ai_iDelta ) ;
    }

    void    SetTotalTagsDelta( m4uint16_t ai_iDelta )
    {
        m_oTotalTags.SetDelta( ai_iDelta ) ;
    }
	
    void    SetTagsDelta( m4uint16_t ai_iDelta )
    {
        m_oTags.SetDelta( ai_iDelta ) ;
    }
	
    void    SetNodesDelta( m4uint16_t ai_iDelta )
    {
        m_oNodes.SetDelta( ai_iDelta ) ;
    }

    void    SetNodeInheritsDelta( m4uint16_t ai_iDelta )
    {
        m_oNodeInherits.SetDelta( ai_iDelta ) ;
    }

    void    SetNitemsDelta( m4uint16_t ai_iDelta )
    {
        m_oNitems.SetDelta( ai_iDelta ) ;
    }

    void    SetConnectorsDelta( m4uint16_t ai_iDelta )
    {
        m_oConnectors.SetDelta( ai_iDelta ) ;
    }

    void    SetInvConnectorsDelta( m4uint16_t ai_iDelta )
    {
        m_oInvConnectors.SetDelta( ai_iDelta ) ;
    }

    void    SetNitemConnsDelta( m4uint16_t ai_iDelta )
    {
        m_oNitemConns.SetDelta( ai_iDelta ) ;
    }

    void    SetIndexesDelta( m4uint16_t ai_iDelta )
    {
        m_oIndexes.SetDelta( ai_iDelta ) ;
    }

    void    SetIndexItemsDelta( m4uint16_t ai_iDelta )
    {
        m_oIndexItems.SetDelta( ai_iDelta ) ;
    }

    void    SetPlugsDelta( m4uint16_t ai_iDelta )
    {
        m_oPlugs.SetDelta( ai_iDelta ) ;
    }

    void    SetPlugItemsDelta( m4uint16_t ai_iDelta )
    {
        m_oPlugItems.SetDelta( ai_iDelta ) ;
    }

    void    SetAliasDelta( m4uint16_t ai_iDelta )
    {
        m_oAlias.SetDelta( ai_iDelta ) ;
    }

    void    SetT3AliasDelta( m4uint16_t ai_iDelta )
    {
        m_oT3Alias.SetDelta( ai_iDelta ) ;
    }

    void    SetFiltersDelta( m4uint16_t ai_iDelta )
    {
        m_oFilters.SetDelta( ai_iDelta ) ;
    }

	void	SetT3AliasLength( m4uint16_t ai_iLength )
	{
		m_oT3Alias.SetLength( ai_iLength ) ;
	}

    void    SetBusinessMethodsDelta( m4uint16_t ai_iDelta )
    {
        m_oBusinessMethods.SetDelta( ai_iDelta ) ;
    }

    void    SetBusinessArgsDelta( m4uint16_t ai_iDelta )
    {
        m_oBusinessArgs.SetDelta( ai_iDelta ) ;
    }

    void    SetSystemArgumentsDelta( m4uint16_t ai_iDelta )
    {
        m_oSystemArguments.SetDelta( ai_iDelta ) ;
    }

	m4return_t	SetT3AliasByPosition( m4uint16_t ai_iPosition, ClTempT3Alias *ai_poT3Alias )
	{
        return( m_oT3Alias.SetElement( ai_iPosition, ai_poT3Alias ) ) ;
	}


	m4return_t	AddT3Inherit		( ClTempT3Inherit		*ai_poT3Inherit		) ;
	m4return_t	AddSentence			( ClTempSentence		*ai_poSentence		) ;
	m4return_t	AddRSM				( ClTempRSM				*ai_poRSM			) ;
	m4return_t	AddTi				( ClTempTi				*ai_poTi			) ;
	m4return_t	AddTiInherit		( ClTempTiInherit		*ai_poTiInherit		) ;
	m4return_t	AddItem				( ClTempItem			*ai_poItem			) ;
	m4return_t	AddItemInherit		( ClTempItemInherit		*ai_poItemInherit	) ;
	m4return_t	AddConcept			( ClTempConcept			*ai_poConcept		) ;
	m4return_t	AddArgument			( ClTempArgument		*ai_poArgument		) ;
	m4return_t	AddItemFormula		( ClTempItemFormula		*ai_poItemFormula	) ;
	m4return_t	AddTrigger			( ClTempTrigger			*ai_poTrigger		) ;
	m4return_t	AddTotal			( ClTempTotal			*ai_poTotal			) ;
	m4return_t	AddTotalTag			( ClTempTotalTag		*ai_poTotalTag		) ;
	m4return_t	AddTag				( ClTempTag				*ai_poTag			) ;
	m4return_t	AddNode				( ClTempNode			*ai_poNode			) ;
	m4return_t	AddNodeInherit		( ClTempNodeInherit		*ai_poNodeInherit	) ;
	m4return_t	AddConnector		( ClTempConnector		*ai_poConnector		) ;
	m4return_t	AddInvConnector		( ClTempConnector		*ai_poConnector		) ;
	m4return_t	AddNitemConn		( ClTempNitemConn		*ai_poNitemConn		) ;
	m4return_t	AddIndex			( ClTempIndex			*ai_poIndex			) ;
	m4return_t	AddIndexItem		( ClTempIndexItem		*ai_poIndexItem		) ;
	m4return_t	AddPlug				( ClTempPlug			*ai_poPlug			) ;
	m4return_t	AddPlugItem			( ClTempPlugItem		*ai_poItem			) ;
	m4return_t	AddAlias			( ClTempAlias			*ai_poAlias			) ;
	m4return_t	AddT3Alias			( ClTempT3Alias			*ai_poT3Alias		) ;
	m4return_t	AddFilter			( ClTempFilter			*ai_poFilter		) ;
	m4return_t	AddInstance			( ClTempInstance		*ai_poInstance		) ;
	m4return_t	AddBusinessMethod	( ClTempBusinessMethod	*ai_poBusinessMethod) ;
	m4return_t	AddBusinessArg		( ClTempBusinessArg		*ai_poBusinessArg	) ;
	m4return_t	AddSystemArgument	( ClTempArgument		*ai_poArgument		) ;

	m4return_t	AddInstance( ClTempInstance *ao_poInstance, m4pcchar_t ai_pccInstanceId, m4pcchar_t ai_pccT3Id, m4pcchar_t ai_pccRoleId, m4pcchar_t ai_pccOrgId, m4uint8_t ai_iScope, m4uint8_t ai_iOpenMode, m4uint8_t ai_iCsAccess, m4uint8_t ai_iUseRole, m4uint8_t ai_iOrgType ) ;

	m4return_t	FillTisWithItems( void ) ;
	m4return_t	FillTisWithItemInherits( void ) ;
	m4return_t	FillTisWithConcepts( void ) ;
	m4return_t	FillTisWithInherits( void ) ;
	m4return_t	FillItemsWithArguments( void ) ;
	m4return_t	FillTisWithItemFormulas( void ) ;
	m4return_t	FillTisWithTriggers( void ) ;
	m4return_t	FillTisWithTotals( void ) ;
	m4return_t	FillTisWithTotalTags( void ) ;
	m4return_t	FillTisWithTags( void ) ;
    m4return_t  FillChannelWithNodesByTiId( void ) ;
    m4return_t  FillNodesWithInherits( void ) ;
    m4return_t  FillNodesWithItems( void ) ;
    m4return_t  FillItemsWithInheritance( void ) ;
    m4return_t  FillNodesWithConnectors( void ) ;
    m4return_t  FillItemsWithConnectors( void ) ;
    m4return_t  FillNodesWithIndexes( void ) ;
    m4return_t  FillIndexesWithItems( void ) ;
    m4return_t  FillNodesWithPlugs( void ) ;
    m4return_t  FillPlugsWithItems( void ) ;
    m4return_t  FillNodesWithAlias( void ) ;
    m4return_t  FillNodesWithT3Alias( void ) ;
    m4return_t  FillNodesWithFilters( void ) ;
    m4return_t  FillChannelWithInstances( void ) ;
    m4return_t  FillBusinessMethodsWithArgs( void ) ;


// Memoria alojada ============================================================

	void	SetNitemConnsArraySize( m4uint16_t ai_iSize )
	{
		m_iNitemConnsSize = ai_iSize ;
	}

	void	SetT3InheritsArray( ClTempT3Inherit *ai_poT3InheritsArray )
	{
		m_poT3InheritsArray = ai_poT3InheritsArray ;
	}
	
	void	SetSentencesArray( ClTempSentence *ai_poSentencesArray )
	{
		m_poSentencesArray = ai_poSentencesArray ;
	}
	
	void	SetRSMsArray( ClTempRSM *ai_poRSMsArray )
	{
		m_poRSMsArray = ai_poRSMsArray ;
	}
	
	void	SetTisArray( ClTempTi *ai_poTisArray )
	{
		m_poTisArray = ai_poTisArray ;
	}
	
	void	SetTiInheritsArray( ClTempTiInherit *ai_poTiInheritsArray )
	{
		m_poTiInheritsArray = ai_poTiInheritsArray ;
	}

	void	SetItemsArray( ClTempItem *ai_poItemsArray )
	{
		m_poItemsArray = ai_poItemsArray ;
	}

	void	SetItemInheritsArray( ClTempItemInherit *ai_poItemInheritsArray )
	{
		m_poItemInheritsArray = ai_poItemInheritsArray ;
	}

	void	SetConceptsArray( ClTempConcept *ai_poConceptsArray )
	{
		m_poConceptsArray = ai_poConceptsArray ;
	}

	void	SetArgumentsArray( ClTempArgument *ai_poArgumentsArray )
	{
		m_poArgumentsArray = ai_poArgumentsArray ;
	}

	void	SetItemFormulasArray( ClTempItemFormula *ai_poItemFormulasArray )
	{
		m_poItemFormulasArray = ai_poItemFormulasArray ;
	}

	void	SetTriggersArray( ClTempTrigger *ai_poTriggersArray )
	{
		m_poTriggersArray = ai_poTriggersArray ;
	}

	void	SetTotalsArray( ClTempTotal *ai_poTotalsArray )
	{
		m_poTotalsArray = ai_poTotalsArray ;
	}

	void	SetTotalTagsArray( ClTempTotalTag *ai_poTotalTagsArray )
	{
		m_poTotalTagsArray = ai_poTotalTagsArray ;
	}

	void	SetTagsArray( ClTempTag *ai_poTagsArray )
	{
		m_poTagsArray = ai_poTagsArray ;
	}

	void	SetNodesArray( ClTempNode *ai_poNodesArray )
	{
		m_poNodesArray = ai_poNodesArray ;
	}

	void	SetNodeInheritsArray( ClTempNodeInherit *ai_poNodeInheritsArray )
	{
		m_poNodeInheritsArray = ai_poNodeInheritsArray ;
	}

	void	SetNitemsArray( ClTempNitem *ai_poNitemsArray )
	{
		m_poNitemsArray = ai_poNitemsArray ;
	}

	void	SetConnectorsArray( ClTempConnector *ai_poConnectorsArray )
	{
		m_poConnectorsArray = ai_poConnectorsArray ;
	}

	void	SetNitemConnsArray( ClTempNitemConn *ai_poNitemConnsArray )
	{
		m_poNitemConnsArray = ai_poNitemConnsArray ;
	}

	void	SetIndexesArray( ClTempIndex *ai_poIndexesArray )
	{
		m_poIndexesArray = ai_poIndexesArray ;
	}

	void	SetIndexItemsArray( ClTempIndexItem *ai_poIndexItemsArray )
	{
		m_poIndexItemsArray = ai_poIndexItemsArray ;
	}

	void	SetPlugsArray( ClTempPlug *ai_poPlugsArray )
	{
		m_poPlugsArray = ai_poPlugsArray ;
	}

	void	SetPlugItemsArray( ClTempPlugItem *ai_poPlugItemsArray )
	{
		m_poPlugItemsArray = ai_poPlugItemsArray ;
	}

	void	SetAliasArray( ClTempAlias *ai_poAliasArray )
	{
		m_poAliasArray = ai_poAliasArray ;
	}

	void	SetT3AliasArray( ClTempT3Alias *ai_poT3AliasArray )
	{
		m_poT3AliasArray = ai_poT3AliasArray ;
	}

	void	SetFiltersArray( ClTempFilter *ai_poFiltersArray )
	{
		m_poFiltersArray = ai_poFiltersArray ;
	}

	void	SetBusinessMethodsArray( ClTempBusinessMethod *ai_poBusinessMethodsArray )
	{
		m_poBusinessMethodsArray = ai_poBusinessMethodsArray ;
	}

	void	SetBusinessArgsArray( ClTempBusinessArg *ai_poBusinessArgsArray )
	{
		m_poBusinessArgsArray = ai_poBusinessArgsArray ;
	}


// Funciones de impresión =====================================================

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

	m4return_t	ComputeCS( m4uint8_t ai_iCsType ) ;


// Funciones varias ===========================================================

    void	OrderT3Inherits( void ) ;

    void	OrderSentences( void )
	{
		m_oSentences.Sort( M4ClCompareObjectsById ) ;
	}

    void	OrderRSMs( void )
	{
		m_oRSMs.Sort( M4ClCompareObjectsById ) ;
	}

    void	OrderTis( void )
	{
		m_oTis.Sort( M4ClCompareObjectsById ) ;
	}

    void	OrderTiInherits( void ) ;
    
	void	OrderItems( void )
	{
		m_oItems.Sort( M4ClCompareObjectsByTwoIds ) ;
	}

    void	OrderItemInherits( void )
	{
		m_oItemInherits.Sort( M4ClCompareObjectsById ) ;
	}

    void	OrderConcepts( void )
	{
		m_oConcepts.Sort( M4ClCompareObjectsById ) ;
	}

    void	OrderArguments( void ) ;

    void	OrderItemFormulas( void ) ;

    void	OrderTriggers( void )
	{
		m_oTriggers.Sort( M4ClCompareObjectsById ) ;
	}

    void	OrderTotals( void ) ;
    void	OrderTotalTags( void ) ;
    void	OrderTags( void ) ;

    void	OrderNodes( void )
	{
		m_oNodes.Sort( M4ClCompareObjectsById ) ;
	}

    void	OrderNodeInherits( void ) ;
    void	OrderConnectors( void ) ;
    void	OrderInvConnectors( void ) ;
    void	OrderNitemConns( void ) ;
    void	OrderIndexes( void ) ;
    void	OrderIndexItems( void ) ;
    void	OrderPlugs( void ) ;
    void	OrderPlugItems( void ) ;

    void	OrderAlias( void )
	{
		m_oAlias.Sort( M4ClCompareObjectsByTwoIds ) ;
	}

    void	OrderT3Alias( void ) ;
    void	OrderFilters( void ) ;

    void	OrderBusinessMethods( void )
	{
		m_oBusinessMethods.Sort( M4ClCompareObjectsById ) ;
	}

    void	OrderBusinessArgs( void )
	{
		m_oBusinessArgs.Sort( M4ClCompareObjectsById ) ;
	}


    m4return_t	OrderItemConnMappings( void ) ;


	m4return_t  CreateTiReferences( void ) ;
	m4return_t  ComputeFormulas( void ) ;
	m4return_t  ComputeOneDBFormula( ClTempNitem *ai_poNitem, ClLongString *ai_poPolish, m4pcchar_t ai_pccItemId, m4bool_t ai_bAddTransaction, m4bool_t ai_bExcludeBack ) ;
	m4return_t  ComputeDBFormulas( void ) ;
    m4return_t  ComputeGraphOrder( void ) ;
    m4return_t  ComputeConnectorOrder( void ) ;
    m4return_t  ComputeRoots( void ) ;

	void	ClearFormulas( void ) ;
    

// Funciones de depuración ====================================================
	
	void	SetPrint( m4uint8_t ai_iPrint )
    {
        m_iPrint = ai_iPrint ;
    }

	m4return_t _Print( FILE* ai_pfFile, m4uint8_t ai_iIndent = 0 ) ;
	m4return_t	Print( m4uint8_t ai_iSystemDebug, m4pcchar_t ai_pccPath ) ;


} ;


#endif

