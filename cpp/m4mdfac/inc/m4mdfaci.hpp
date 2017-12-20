
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                m4mdfaci.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                29-02-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la implementación básica de la factoría de metadatos
//
//
//==============================================================================


#include "m4mdfac.hpp"

#include "index.hpp"
#include "spoolstr.h"


#ifndef __M4MDFACI_HPP__
#define __M4MDFACI_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClRegister ;
class	ClAccess ;
class	ClChannel ;
class	ClNode ;
class	ClTempTi ;
class	ClTempChannel ;
class	ClTempItemFormula ;
class	ClTempTag ;
class	ClTempArgument ;
class	ClTempNode ;
class	ClTempNodeInherit ;
class	ClTempItem ;
class	ClTempItemInherit ;
class	ClTempNitem ;
class	ClTempConnector ;
class	ClTempIndex ;
class	ClTempIndexItem ;
class	ClTempTrigger ;
class	ClTempAlias ;
class	ClTempT3Alias ;
class	ClTempPlug ;
class	ClTempPlugItem ;
class	ClTempSentence ;
class	ClTempNitemConn ;
class	ClTempFilter ;
class	ClTempBusinessArg ;
class	ClTempBusinessMethod ;
class	ClCache ;
class	ClCacheDirectory ;
class	ClVMStatisticsWrapper ;
class	IFuncTableDesc ;
class	IAtributeFuncTable ;
class	ClChannelManager ;
class	ClVMBaseEnv ;
class	ClCMCRCache ;
class	ClCSCRCache ;
class	ClMDAdaptor ;
class	ClLogonAdaptor ;
class	ClCsTrace ;
class	ClAccessRecordSet ;
class	ClTempT3Inherit ;
class	ClTempTiInherit ;
class	ClTempRSM ;
class	ClTempTotal ;
class	ClTempTotalTag ;
class	ClTempConcept ;
class	ClMdStatistics ;
class	ILdb ;


//=================================================================================
// Tipos de factorías
//=================================================================================

typedef	enum	eM4MdFacTypes_tag
{
	eM4MdFacImp = 0,
	eM4MdFacClient,
	eM4MdFacServer,
	eM4MdFacClientEmul,
	eM4MdFacServerEmul

} eM4MdFacTypes_t ;



//=================================================================================
//
// ClFactoryImp
//
// Clase que implementa la factoría base de metadatos y seguridad
//
//=================================================================================


class	M4_DECL_M4MDFAC	 ClFactoryImp : public ClCMCRFactory
{

protected:

//=================================================================================
// Miembros
//=================================================================================

// Inicialización

	m4bool_t				m_bIsInit ;

// Punteros de referencia

	ClChannelManager		*m_poChannelManager ;
	ClVMBaseEnv				*m_poEnvironment ;
	ClLogonAdaptor			*m_poLogonAdaptor ;
	ClCacheDirectory		*m_poCacheDirectory ;
	ClVMStatisticsWrapper	*m_poStatisticsWrap ;
	IFuncTableDesc			*m_poFunctionTable ;
	IAtributeFuncTable		*m_poAtributeTable ;
	ClCsTrace				*m_poCsTrace ;
	ILdb					*m_poLdb ;

// Lo demás

    ClAccess				*m_poAccess ;
    ClCompiledMCR			*m_poCMCR ;

    ClStaticPoolStr			m_oAccessUpperPool ;
    ClStaticPoolStr			m_oAccessNoUpperPool ;
    ClStaticPoolStr			m_oSystemUpperPool ;
    ClStaticPoolStr			m_oSystemNoUpperPool ;

// Items del sistema

    m4uint8_t				m_iIsSystemBuilt ;
    ClTempChannel			*m_poSystemTempM4Obj ;
    ClChannel				*m_poSystemM4Obj ;
    ClAccess				*m_poSystemAccess ;

// Metadatos

    m4uint8_t				m_iIsMetaBuilt ;
    ClChannel				*m_poMetaM4Obj ;
    ClAccess				*m_poMetaAccess ;

    m4uint8_t				m_iIsCheckMetaBuilt ;
    ClChannel				*m_poCheckMetaM4Obj ;
    ClAccess				*m_poCheckMetaAccess ;
    ClNode					*m_poCheckMetaNode ;

// Seguridad

    m4uint8_t				m_iIsSecurityBuilt ;
    ClChannel				*m_poSecurityM4Obj ;
    ClAccess				*m_poSecurityAccess ;

// Diccionario

    m4uint8_t				m_iIsDictBuilt ;
    ClChannel				*m_poDictM4Obj ;
    ClPointerStack			m_oDictAccesses ;

// Traducciones

    m4uint8_t				m_iIsTransBuilt ;
    ClChannel				*m_poTransM4Obj ;
    ClPointerStack			m_oTransAccesses ;

// Caches

	ClCMCRCache				*m_poMDCache ;
	ClCSCRCache				*m_poSecCache ;
	ClMapCache				*m_poMapCache ;

// Estadísticas
	ClMdStatistics			*m_poStatistics ;

#ifdef	_DEBUG
    m4double_t				m_dBuffer ;
    m4pchar_t				m_pcBuffer ;
    FILE					*m_pfFile ;

    ClStaticPoolStr			m_oFilePool ;
    ClStaticPoolStr			m_oTempPool ;
#endif


//=================================================================================
// Funciones polimórficas
//=================================================================================

	virtual	m4uint8_t	p_GetContext( void ) const
	{
		return( eM4MdFacImp ) ;
	}

	virtual	m4return_t	p_DelegateMDFromId( m4pcchar_t ai_pccM4ObjId, ClBuidParameters &ai_roBP, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, ClCompiledMCR *ai_poCMCR, ClCMCRWrapper* &ao_rpoCMCRWrap )
	{
		return( M4_ERROR ) ;
	}

	virtual	m4return_t	p_DelegateMapFromId( m4pcchar_t ai_pccMapId, ClMappings* &ao_rpoMappings )
	{
		return( M4_ERROR ) ;
	}

	virtual	m4return_t	p_DelegateSecFromId( m4pcchar_t ai_pccMask, ClCompiledMCR *ai_poCMCR, ClCSCRWrapper* &ai_rpoSecWrap )
	{
		return( M4_ERROR ) ;
	}



//=================================================================================
// Funciones internas
//=================================================================================

	m4pchar_t	_ComposeMappingId( m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccOrganization, m4bool_t ai_bConvert, m4pchar_t ai_pcBuffer, m4uint8_t ai_iSize ) const ;

	m4return_t	_BuildSystemChannel( m4pcchar_t ai_pccFile, ClChannel* &ao_roChannel ) ;

	m4return_t	_CreateSystemAccess( void ) ;
	m4return_t	_CreateMetaAccess( void ) ;
	m4return_t	_CreateSecurityAccess( void ) ;

    m4return_t	_CreateSecFromAccess( m4pcchar_t ai_pccRSM, ClAccess *ai_poAccess, m4uint8_t ai_iRegister, ClCompiledMCR *ai_poCMCR ) ;
    m4return_t	_CreateSecBuffer( m4pcchar_t ai_pccRSM, ClCompiledMCR *ai_poCMCR, ClAccess *ai_poSecurityAccess ) const ;
	m4return_t	_CreateSecFromId( m4pcchar_t ai_pccRSM, m4uint8_t ai_iConvertRSM, ClCompiledMCR *ai_poCMCR ) ;

    m4return_t  _CreateFromTemp( ClTempChannel *ao_poChannel, m4uint8_t ai_iCsType, ClStaticPoolStr *ao_poPool, ClStaticPoolStr *ao_poNoUpperPool, ClCompiledMCR* &ao_rpCMCR ) ;

	m4return_t	_CheckVersion( m4pcchar_t ai_pccM4ObjId, m4date_t ai_dVersion, m4bool_t& ao_rbOld ) ;


// XML ========================================================================

	// Funciones de generación
	m4return_t	_PrintXmlArg			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition ) ;
	m4return_t	_PrintXmlConnector		( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4pcchar_t ai_pccTag ) ;
	m4return_t	_PrintXmlItem			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint16_t ai_iIndex, m4pcchar_t ai_pccOrganization ) ;
	m4return_t	_PrintXmlIndexItem		( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint16_t ai_iPosition ) ;
	m4return_t	_PrintXmlIndex			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle ) ;
	m4return_t  _PrintXmlListColumn		( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle ) ;
	m4return_t	_PrintXmlList			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccOrganization ) ;
	m4return_t	_PrintXmlFilter			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint8_t ai_iType, m4uint32_t ai_iNodeHandle ) ;
	m4return_t	_PrintXmlAlias			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iNodeHandle, m4uint16_t ai_iPosition ) ;
	m4return_t	_PrintXmlNode			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4pcchar_t ai_pccParentNode, m4pcchar_t ai_pccOrganization ) ;
	m4return_t	_PrintXmlBasicBusArg	( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint16_t ai_iPosition ) ;
	m4return_t	_PrintXmlObjectBusArg	( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iNodeHandle ) ;
	m4return_t	_PrintXmlObjectBusArg	( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBusinessMehtodHandle, m4uint16_t ai_iPosition ) ;
	m4return_t	_PrintXmlBusinessMethod	( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4pcchar_t ai_pccMethodId, m4uint32_t ai_iItemHandle ) ;
	m4return_t	_PrintXmlBusinessMethod	( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle ) ;
	m4return_t	_PrintXmlRelationColumn	( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle ) ;
	m4return_t	_PrintXmlRelation		( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle ) ;
	m4return_t	_PrintXmlTiRef			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iTiHandle, m4uint16_t ai_iPosition ) ;
	m4return_t	_PrintXmlTi				( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle ) ;
	m4return_t	_PrintXmlM4Obj			( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4date_t ai_dVersion, m4pcchar_t ai_pccOrganization ) ;
	m4return_t	_PrintXmlHeader			( ClCompiledMCR *ai_poCMCR,											   ClLongString &ao_roXML ) ;


	ClVMBaseEnv*	_GetEnvironment( void ) const
	{
		return( m_poEnvironment ) ;
	}

	ClLogonAdaptor*	_GetLogonAdaptor( void ) const
	{
		return( m_poLogonAdaptor ) ;
	}

	IFuncTableDesc*		_GetFunctionTable( void ) const
	{
		return( m_poFunctionTable ) ;
	}

	IAtributeFuncTable*	_GetAtributeTable( void ) const
	{
		return( m_poAtributeTable ) ;
	}

	friend	class	ClTempRitem ;
	friend	class	ClTempNitemFormula ;
	friend	class	ClTempChannel ;


// Lectura de fichero =========================================================

#ifdef	_DEBUG
    m4return_t  _ReadNumber( m4pcchar_t ai_pccPattern ) ;
    m4return_t  _ReadString( m4pcchar_t ai_pccPattern ) ;
    m4return_t  _ReadEmpty ( m4pcchar_t ai_pccPattern ) ;
    m4return_t  _ReadLong  ( m4pcchar_t ai_pccPattern ) ;

    m4return_t  _ReadOneT3FromFile			( ClTempChannel *ai_poChannel ) ;
    m4return_t  _ReadOneSentenceFromFile	( ClTempSentence *ao_poSentence ) ;
    m4return_t  _ReadOneTiFromFile			( ClTempChannel *ai_poChannel, ClTempTi *ao_poTi ) ;
    m4return_t  _ReadOneTiInheritFromFile	( ClTempChannel *ai_poChannel, ClTempTiInherit *ai_poTiInherit ) ;
    m4return_t  _ReadOneItemFromFile		( ClTempChannel *ai_poChannel, ClTempItem *ao_poItem ) ;
    m4return_t  _ReadOneRuleFromFile		( ClTempChannel *ai_poChannel, ClTempItemFormula *ao_poFormula ) ;
    m4return_t  _ReadOneNodeFromFile		( ClTempChannel *ai_poChannel, ClTempNode *ao_poNode ) ;
    m4return_t  _ReadOneConnectorFromFile	( ClTempChannel *ai_poChannel, ClTempConnector *ao_poConnector ) ;
    m4return_t  _ReadOneNitemConnFromFile	( ClTempChannel *ai_poChannel, ClTempNitemConn *ao_poNitemConn ) ;
    m4return_t  _ReadOneIndexFromFile		( ClTempIndex *ao_poIndex ) ;
    m4return_t  _ReadOneIndexItemFromFile	( ClTempIndexItem *ao_poIndexItem ) ;

    m4return_t  _ReadT3FromFile				( ClTempChannel *ao_poChannel, m4pcchar_t ai_pccFile ) ;
	m4return_t  _ReadT3InheritsFromFile		( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadSentencesFromFile		( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadTisFromFile			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadTiInheritsFromFile		( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadItemsFromFile			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadRulesFromFile			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadNodesFromFile			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadNodeInheritsFromFile	( ClTempChannel *ao_poChannel ) ;
	m4return_t  _ReadConnectorsFromFile		( ClTempChannel *ai_poChannel ) ;
    m4return_t  _ReadItemConnectorsFromFile	( ClTempChannel *ai_poChannel ) ;
    m4return_t  _ReadIndexesFromFile		( ClTempChannel *ai_poChannel ) ;
    m4return_t  _ReadIndexItemsFromFile		( ClTempChannel *ai_poChannel ) ;
#endif


// Lectura de access ==========================================================

    m4return_t  _ReadOneT3FromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister ) ;
    m4return_t  _ReadOneT3InheritFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempT3Inherit		*ao_poT3Inherit		) ;
    m4return_t  _ReadOneSentenceFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempSentence		*ao_poSentence		) ;
	m4return_t  _ReadOneRSMFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempRSM				*ao_poRSM			) ;
    m4return_t  _ReadOneTiFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTi				*ao_poTi, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool ) ;
    m4return_t  _ReadOneTiInheritFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTiInherit		*ao_poTiInherit		) ;
    m4return_t  _ReadOneItemFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempItem			*ao_poItem			) ;
    m4return_t  _ReadOneItemInheritFromAccess		( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempItemInherit		*ao_poItemInherit	) ;
    m4return_t  _ReadOneConceptFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempConcept			*ao_poConcept		) ;
    m4return_t  _ReadOneArgumentFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempArgument		*ao_poArgument		) ;
    m4return_t  _ReadOneRuleFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempItemFormula		*ao_poFormula		) ;
    m4return_t  _ReadOneTagFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTag				*ao_poTag			) ;
    m4return_t  _ReadOneTriggerFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTrigger			*ao_poTrigger		) ;
    m4return_t  _ReadOneTotalFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTotal			*ao_poTotal			) ;
	m4return_t  _ReadOneTotalTagFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempTotalTag		*ao_poTotalTag		) ;
    m4return_t  _ReadOneNodeFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempNode			*ao_poNode			) ;
    m4return_t  _ReadOneNodeInheritFromAccess		( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempNodeInherit		*ao_poNodeInherit	) ;
    m4return_t  _ReadOneConnectorFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempConnector		*ao_poConnector		) ;
    m4return_t  _ReadOneItemConnFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempNitemConn		*ao_poNitemConn		) ;
	m4return_t	_ReadOneArgumentMappingFromAccess	( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, StArgumentMappig_t	*ao_pstMapping, ClTempNitem *ai_poSonNitem, ClTempNode *ai_poSonNode, ClTempItem *ai_poFatherItem, ClTempNode *ai_poFatherNode, ClTempArgument *ai_poArgument ) ;
    m4return_t  _ReadOneIndexFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempIndex			*ao_poIndex			) ;
    m4return_t  _ReadOneIndexItemFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempIndexItem		*ao_poIndexItem		) ;
    m4return_t  _ReadOnePlugFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempPlug			*ao_poPlug			) ;
    m4return_t  _ReadOnePlugItemFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempPlugItem		*ao_poPlugItem		) ;
    m4return_t  _ReadOneAliasFromAccess				( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempAlias			*ao_poAlias			) ;
    m4return_t  _ReadOneT3AliasFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempT3Alias			*ao_poT3Alias		) ;
    m4return_t  _ReadOneFilterFromAccess			( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempFilter			*ao_poFilter		) ;
    m4return_t  _ReadOneBusinessMethodFromAccess	( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempBusinessMethod	*ao_poBusinessMethod) ;
    m4return_t  _ReadOneBusinessArgFromAccess		( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister, ClTempBusinessArg		*ao_poBusinessArg	) ;


// Lectura plana de access ====================================================

    m4return_t  _ReadT3FromAccess					( ClTempChannel *ao_poChannel, ClRegister *ai_poRegister ) ;
    m4return_t  _ReadT3InheritsFromAccess			( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadSentencesFromAccess			( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool ) ;
    m4return_t  _ReadRSMsFromAccess					( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadTisFromAccess					( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool ) ;
    m4return_t  _ReadTiInheritsFromAccess			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadItemsFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool ) ;
    m4return_t  _ReadItemInheritsFromAccess			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadConceptsFromAccess				( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadArgumentsFromAccess			( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool ) ;
    m4return_t  _ReadRulesFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadTagsFromAccess					( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadTriggersFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadTotalsFromAccess				( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadTotalTagsFromAccess			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadNodesFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poUpperPool, ClStaticPoolStr *ao_poNoUpperPool ) ;
    m4return_t  _ReadNodeInheritsFromAccess			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadNodeReadsFromAccess			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadConnectorsFromAccess			( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadItemConnsFromAccess			( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadArgumentMappingsFromAccess		( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadIndexesFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadIndexItemsFromAccess			( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadPlugsFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool, ClStaticPoolStr *ao_poNoUpperPool ) ;
    m4return_t  _ReadItemPlugsFromAccess			( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadAliasFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadT3AliasFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadFiltersFromAccess				( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool ) ;
    m4return_t  _ReadItemCsDescFromAccess			( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadBusinessServiceFromAccess		( ClTempChannel *ao_poChannel ) ;
    m4return_t  _ReadBusinessMethodsFromAccess		( ClTempChannel *ao_poChannel, ClStaticPoolStr *ao_poPool, ClStaticPoolStr *ao_poNoUpperPool ) ;
    m4return_t  _ReadBusinessArgsFromAccess			( ClTempChannel *ao_poChannel ) ;

	m4return_t	_GetRecorset( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccMessage, ClAccessRecordSet *&ao_rpoRecordset ) ;
	m4return_t	_ReadName( ClTempChannel *ai_poChannel, m4pcchar_t &ao_rpccValue, m4bool_t ai_bUseDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister, m4pcchar_t ai_pccTransNode, ClItemIndex ai_iTransIndex, m4pcchar_t ai_pccFirst, m4pcchar_t ai_pccSecond = NULL );


// Lectura de seguridad =======================================================

    m4return_t  _ReadOneT3SecFromAccess		( m4pchar_t ai_pccMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR, m4pchar_t ai_pcSecurity ) ;
    m4return_t  _ReadOneNodeSecFromAccess	( m4pchar_t ai_pccMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR ) ;
    m4return_t  _ReadOneItemSecFromAccess	( m4pchar_t ai_pccMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR ) ;
    m4return_t  _ReadOneFilterSecFromAccess	( m4pchar_t ai_pccMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR ) ;
    m4return_t  _ReadOneT3ConSecFromAccess	( m4pchar_t ai_pccMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR ) ;

// Lectura plana de seguridad =================================================

    m4return_t  _ReadNodesSecFromAccess		( ClAccess *ai_poAccess, ClCompiledMCR *ai_poCMCR ) ;
    m4return_t  _ReadItemsSecFromAccess		( ClAccess *ai_poAccess, ClCompiledMCR *ai_poCMCR ) ;
    m4return_t  _ReadFiltersSecFromAccess	( ClAccess *ai_poAccess, ClCompiledMCR *ai_poCMCR ) ;
    m4return_t  _ReadT3ConsSecFromAccess	( ClAccess *ai_poAccess, ClCompiledMCR *ai_poCMCR ) ;
    m4return_t  _ReadSecurityFromAccess		( ClAccess *ai_poAccess, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR ) ;


// Funciones de inicialización ================================================

		ClFactoryImp( void ) ;

public:

		~ClFactoryImp( void ) ;

	m4return_t	Init	(
							ClChannelManager		*ai_poChannelManager,
							ClVMBaseEnv				*ai_poEnvironment,
							ClLogonAdaptor			*ai_poLogonAdaptor,
							ClCache					*ai_poMDCache,
							ClCache					*ai_poSecCache,
							ClCache					*ai_poMapCache,
							ClCacheDirectory		*ai_poCacheDirectory,
							ClVMStatisticsWrapper	*ai_poStatisticsWrap,
							IFuncTableDesc			*ai_poFunctionTable,
							IAtributeFuncTable		*ai_poAtributeTable,
							ClCsTrace				*ai_poCsTrace
						) ;

	m4return_t	End( void ) ;


	//=================================================================================
	// Genera un metacanal compilado a partir de un fichero
	//=================================================================================
	m4return_t	MDFac_BuildFromFile( m4pcchar_t ai_pccFileName, m4uint8_t ai_iCsType, m4uint8_t ai_iUseMcrs, m4pchar_t ai_pccPath, ClCompiledMCR* &ao_rpCMCR ) ;

	//=================================================================================
	// Genera un metacanal compilado a partir de un acceso
	//=================================================================================
	m4return_t	MDFac_BuildFromAccess( ClAccess *ai_poAccess, m4uint8_t ai_iCsType, m4uint8_t ai_iLanguage, m4uint8_t ai_iRegister, m4bool_t ai_bAddSystem, ClCompiledMCR* &ao_rpCMCR ) ;

	//=================================================================================
	// Genera un metacanal compilado a partir de su modelo en base de datos
	//=================================================================================
	m4return_t  MDFac_BuildFromId( ClBuidParameters &ai_roBP, ClCompiledMCR* &ao_rpCMCR ) ;

	//=================================================================================
	// Genera un metacanal compilado en formato xml a partir de su modelo en base de datos
	//=================================================================================
	m4return_t  MDFac_BuildXML( ClCompiledMCR *ai_poCMCR, m4pcchar_t ai_pccOrganization, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4bool_t ai_bGenHeader ) ;

	//=================================================================================
	// Genera un wrapper de metacanal compilado a partir de su modelo en base de datos
	//=================================================================================
	m4return_t  MDFac_BuildMDWrapper( m4pcchar_t ai_pccM4ObjId, m4pcchar_t ai_pccOrgId, m4uint8_t ai_iCsType, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, m4uint8_t ai_iLanguage, m4bool_t ai_bApplyMappings, ClCMCRWrapper* &ao_rpoMDWrap ) ;

	//=================================================================================
	// Genera un wrapper de seguridad compilada a partir de su modelo en base de datos
	//=================================================================================
	m4return_t	MDFac_BuildSecWrapper( m4pcchar_t ai_pccM4ObjId, m4uint8_t ai_iCsType, m4pcchar_t ai_pccMask, m4uint8_t ai_iLanguage, ClCSCRWrapper* &ai_poSecWrap ) ;

	//=================================================================================
	// Obtiene un tipo concreto de mapeos
	//=================================================================================
	m4return_t	MDFac_BuildMappings( m4pcchar_t ai_pccMapId, ClMappings* &ai_rpoMappings ) ;

	//=================================================================================
	// Obtiene un mapeo para un tipo de mapeo, una sociedad y un origen
	//=================================================================================
	m4pchar_t	MDFac_GetMappingById( m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccSource, m4bool_t ai_bConvert ) ;

	//=================================================================================
	// Determina si 2 sociedades son equivalentes para un tipo de mapeo y un destino
	//=================================================================================
	m4return_t	MDFac_IsEqualOrg( m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccOrganization1, m4pcchar_t ai_pccOrganization2, m4pcchar_t ai_pccDestination, m4bool_t ai_bConvert, m4bool_t& ao_rbIsEquivalent ) ;

	//=================================================================================
	// Asocia una máscara de seguridada a un metacanal compilado
	//=================================================================================
	m4return_t  MDFac_AttachRSM( ClCompiledMCR *ai_poCMCR, m4pcchar_t ai_pccRSM ) ;

	//=================================================================================
	// Construye accesos nuevos sobre los metacanales del diccionario
	//=================================================================================
	m4return_t	MDFac_BuildDictAccess( ClAccess* &ai_rpoDictAccess, ClAccess* &ai_rpoTransAccess ) ;

	//=================================================================================
	// Borra accesos sobre los metacanales del diccionario
	//=================================================================================
	m4return_t	MDFac_RemoveDictAccess( ClAccess *ai_poDictAccess, ClAccess *ai_poTransAccess ) ;

	//=================================================================================
	// Obtención de la caché de metadatos
	//=================================================================================
    virtual	ClCMCRCache*	MDFac_GetMDCache( void )
	{
		return( m_poMDCache )  ;
	}

	//=================================================================================
	// Obtención de la caché de seguridad
	//=================================================================================
    virtual	ClCSCRCache*	MDFac_GetSecCache( void )
	{
		return( m_poSecCache )  ;
	}

	//=================================================================================
	// Obtención de la caché de mapeos
	//=================================================================================
    virtual	ClMapCache*	MDFac_GetMapCache( void )
	{
		return( m_poMapCache )  ;
	}

} ;





//=================================================================================
//
// ClFactoryClient
//
// Clase que implementa la factoría cliente base de metadatos y seguridad
//
//=================================================================================


class	M4_DECL_M4MDFAC	 ClFactoryClient : public ClFactoryImp
{

protected:

	ClMDAdaptor	*m_poMDAdaptor ;

public:

		ClFactoryClient( void )
		{
			m_poMDAdaptor = NULL ;
		}

		~ClFactoryClient( void ) {} ;

	m4return_t	Init	(
							ClChannelManager		*ai_poChannelManager,
							ClVMBaseEnv				*ai_poEnvironment,
							ClLogonAdaptor			*ai_poLogonAdaptor,
							ClCache					*ai_poMDCache,
							ClCache					*ai_poSecCache,
							ClCache					*ai_poMapCache,
							ClCacheDirectory		*ai_poCacheDirectory,
							ClVMStatisticsWrapper	*ai_poStatisticsWrap,
							IFuncTableDesc			*ai_poFunctionTable,
							IAtributeFuncTable		*ai_poAtributeTable,
							ClCsTrace				*ai_poCsTrace,
							ClMDAdaptor				*ai_poMDAdaptor
						) ;

	m4return_t	End( void ) ;


//=================================================================================
// Funciones polimórficas
//=================================================================================

	m4uint8_t	p_GetContext( void ) const
	{
		return( eM4MdFacClient ) ;
	}

	m4return_t	p_DelegateMDFromId ( m4pcchar_t ai_pccM4ObjId, ClBuidParameters &ai_roBP, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, ClCompiledMCR *ai_poCMCR, ClCMCRWrapper* &ao_rpoCMCRWrap ) ;
	m4return_t	p_DelegateMapFromId( m4pcchar_t ai_pccMapId, ClMappings* &ao_rpoMappings ) ;
	m4return_t	p_DelegateSecFromId( m4pcchar_t ai_pccRSM, ClCompiledMCR *ai_poCMCR, ClCSCRWrapper* &ai_rpoSecWrap ) ;

} ;



//=================================================================================
//
// ClFactoryServer
//
// Clase que implementa la factoría server base de metadatos y seguridad
//
//=================================================================================


class	M4_DECL_M4MDFAC	 ClFactoryServer : public ClFactoryImp
{

public:

		ClFactoryServer( void ) {} ;
		~ClFactoryServer( void ) {} ;

	void	SetLdb( ILdb *ai_poLdb )
	{
		m_poLdb = ai_poLdb ;
	}


//=================================================================================
// Funciones polimórficas
//=================================================================================

	m4uint8_t	p_GetContext( void ) const
	{
		return( eM4MdFacServer ) ;
	}

	m4return_t	p_DelegateMDFromId ( m4pcchar_t ai_pccM4ObjId, ClBuidParameters &ai_roBP, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, ClCompiledMCR *ai_poCMCR, ClCMCRWrapper* &ao_rpoCMCRWrap ) ;
	m4return_t	p_DelegateMapFromId( m4pcchar_t ai_pccMapId, ClMappings* &ao_rpoMappings ) ;
	m4return_t	p_DelegateSecFromId( m4pcchar_t ai_pccRSM, ClCompiledMCR *ai_poCMCR, ClCSCRWrapper* &ai_rpoSecWrap ) ;

} ;



//=================================================================================
//
// ClFactoryClientEmul
//
// Clase que implementa la factoría cliente emulación base de metadatos y seguridad
//
//=================================================================================


class	M4_DECL_M4MDFAC	 ClFactoryClientEmul : public ClFactoryClient
{

public:

		ClFactoryClientEmul( void ) {} ;
		~ClFactoryClientEmul( void ) {} ;

//=================================================================================
// Funciones polimórficas
//=================================================================================

	m4uint8_t	p_GetContext( void ) const
	{
		return( eM4MdFacClientEmul ) ;
	}

} ;



//=================================================================================
//
// ClFactoryServerEmul
//
// Clase que implementa la factoría server emulación de metadatos y seguridad
//
//=================================================================================


class	M4_DECL_M4MDFAC	 ClFactoryServerEmul : public ClFactoryServer
{

public:

		ClFactoryServerEmul( void ) {} ;
		~ClFactoryServerEmul( void ) {} ;


//=================================================================================
// Funciones polimórficas
//=================================================================================

	m4uint8_t	p_GetContext( void ) const
	{
		return( eM4MdFacServerEmul ) ;
	}

} ;



#endif

