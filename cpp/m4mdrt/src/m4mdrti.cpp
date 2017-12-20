
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdrt.dll
// File:                m4mdrt.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene el interfaz del run time de metadatos
//
//
//==============================================================================


#include <stdio.h>
#include "m4mdrt.hpp"

#include "m4mdrtde.hpp"
#include "m4types.hpp"




//=================================================================================
//
// ClCompiledMCR
//
// Clase con todo el metacanal compilado
//
//=================================================================================

// Funciones inline ===========================================================

m4uint32_t	ClCompiledMCR :: GetChannelSize( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SIZE ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetChannelBuild( void ) const
{
    return( M4ClUnPackInt16( m_pcBase, M4CL_MCR_CHANNEL_BUILD ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelTimeUnit( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_TIME_UNIT ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelCsType( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_CS_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelLanguage( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_LANGUAGE ) ) ;
}

m4date_t	ClCompiledMCR :: GetChannelStartDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_START_DATE ) ) ;
}

m4date_t	ClCompiledMCR :: GetChannelEndDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_END_DATE ) ) ;
}

m4date_t	ClCompiledMCR :: GetChannelCorStartDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_COR_START_DATE ) ) ;
}

m4date_t	ClCompiledMCR :: GetChannelCorEndDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_COR_END_DATE ) ) ;
}

m4date_t	ClCompiledMCR :: GetChannelVersion( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_VERSION ) ) ;
}

m4date_t	ClCompiledMCR :: GetChannelCreationDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_CREATION_DATE ) ) ;
}

m4date_t	ClCompiledMCR :: GetChannelCacheMaxPeriod( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_CACHE_MAX_PERIOD ) ) ;
}


m4pcchar_t	ClCompiledMCR :: GetChannelId( void ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ID ) ;

    return( m_pcBase + iHandle ) ;
}


m4pcchar_t	ClCompiledMCR :: GetChannelName( void ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_NAME ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelPool( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_POOL ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelSysLoadItem( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SYS_LOAD_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelSysPersistItem( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SYS_PERSIST_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelTiSystemHandle( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_TI_SYSTEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelSysLoadClientHandle( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SYS_LOAD_CLIENT ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelSysPersistClientHandle( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SYS_PERSIST_CLIENT ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelSysLoadServerHandle( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SYS_LOAD_SERVER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelSysPersistServerHandle( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SYS_PERSIST_SERVER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelExecuteReportHandle( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_EXECUTEREPORT ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelDynFilterHandle( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_DYN_FILTER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelEnvironmentVars( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ENVIRONMENT_VARS ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelOwnerFlag( void ) const
{
    return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_OWNER_FLAG ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelOleClassId( void ) const
{

    m4uint32_t	iHandle ;

    iHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_OLE_CLASS_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelServiceId( void ) const
{

    m4uint32_t	iHandle ;

    iHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SERVICE_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelServiceAltId( void ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SERVICE_ALT_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelIsExternal( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_IS_EXTERNAL ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelType( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelHasSecurity( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_HAS_SECURITY ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelIsCacheable( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_IS_CACHEABLE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelIsMDCacheable( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_IS_MD_CACHEABLE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelCheckConcurrency( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_CHECK_CONCURRENCY ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelOrganizationType( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_ORGANIZATION_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelInheritLevel( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_INHERIT_LEVEL ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelDataLanguage( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_DATA_LANGUAGE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelMetaLanguage( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_META_LANGUAGE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelCreationType( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_CREATION_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelStyleUse( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_STYLEUSE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelWSSSecurity( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_WSS_SECURITY ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfNodes( void ) const
{
    return( M4ClUnPackInt16( m_pcBase, M4CL_MCR_CHANNEL_NODE_NUMBER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelNodeHandleByPosition( m4uint16_t ai_iNodePosition ) const
{

    m4uint32_t	iNodeHandle ;


    iNodeHandle = M4CL_MCR_CHANNEL_NODES + ai_iNodePosition * M4CL_MCR_CHANNEL_NODE_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iNodeHandle + M4CL_MCR_CHANNEL_NODE_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelNodeIdByPosition( m4uint16_t ai_iNodePosition ) const
{

    m4uint32_t	iHandle ;
    m4uint32_t	iNodeHandle ;


    iNodeHandle = M4CL_MCR_CHANNEL_NODES + ai_iNodePosition * M4CL_MCR_CHANNEL_NODE_SIZE ;
    iHandle = M4ClUnPackInt32( m_pcBase, iNodeHandle ) ;

    return( m_pcBase + iNodeHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfRoots( void ) const
{

    m4uint32_t	iRootsHandle ;


    iRootsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ROOTS_START ) ;

    return( M4ClUnPackInt16( m_pcBase, iRootsHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelRootHandleByPosition( m4uint16_t ai_iRootPosition ) const
{

    m4uint32_t	iRootsHandle ;
    m4uint32_t	iHandle ;


    iRootsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ROOTS_START ) ;
    iHandle = iRootsHandle + M4CL_MCR_CHANNEL_ROOTS + ai_iRootPosition * M4CL_MCR_CHANNEL_ROOT_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_CHANNEL_ROOT_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelRootIdByPosition( m4uint16_t ai_iRootPosition ) const
{

    m4uint32_t	iRootsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iRootsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ROOTS_START ) ;
    iHandle = iRootsHandle + M4CL_MCR_CHANNEL_ROOTS + ai_iRootPosition * M4CL_MCR_CHANNEL_ROOT_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfSentences( void ) const
{

    m4uint32_t	iSentencesHandle ;


    iSentencesHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SENTENCES_START ) ;

    return( M4ClUnPackInt16( m_pcBase, iSentencesHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelSentenceHandleByPosition( m4uint16_t ai_iSentencePosition ) const
{

    m4uint32_t	iSentencesHandle ;
    m4uint32_t	iHandle ;


    iSentencesHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SENTENCES_START ) ;
    iHandle = iSentencesHandle + M4CL_MCR_CHANNEL_SENTENCES + ai_iSentencePosition * M4CL_MCR_CHANNEL_SENTENCE_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_CHANNEL_SENTENCE_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelSentenceIdByPosition( m4uint16_t ai_iSentencePosition ) const
{

    m4uint32_t	iSentencesHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iSentencesHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SENTENCES_START ) ;
    iHandle = iSentencesHandle + M4CL_MCR_CHANNEL_SENTENCES + ai_iSentencePosition * M4CL_MCR_CHANNEL_SENTENCE_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfInstances( void ) const
{

    m4uint32_t	iInstancesHandle ;


    iInstancesHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_INSTANCES_START ) ;

    return( M4ClUnPackInt16( m_pcBase, iInstancesHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelInstanceHandleByPosition( m4uint16_t ai_iInstancePosition ) const
{

    m4uint32_t	iInstancesHandle ;
    m4uint32_t	iHandle ;


    iInstancesHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_INSTANCES_START ) ;
    iHandle = iInstancesHandle + M4CL_MCR_CHANNEL_INSTANCES + ai_iInstancePosition * M4CL_MCR_CHANNEL_INSTANCE_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_CHANNEL_INSTANCE_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelInstanceIdByPosition( m4uint16_t ai_iInstancePosition ) const
{

    m4uint32_t	iInstancesHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iInstancesHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_INSTANCES_START ) ;
    iHandle = iInstancesHandle + M4CL_MCR_CHANNEL_INSTANCES + ai_iInstancePosition * M4CL_MCR_CHANNEL_INSTANCE_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfItems( void ) const
{

    m4uint32_t	iItemsHandle ;


    iItemsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ITEMS_START ) ;

    return( M4ClUnPackInt16( m_pcBase, iItemsHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelItemHandleByPosition( m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iItemsHandle ;
    m4uint32_t	iHandle ;


    iItemsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ITEMS_START ) ;
    iHandle = iItemsHandle + M4CL_MCR_CHANNEL_ITEMS + ai_iItemPosition * M4CL_MCR_CHANNEL_ITEM_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_CHANNEL_ITEM_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelItemIdByPosition( m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iItemsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iItemsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ITEMS_START ) ;
    iHandle = iItemsHandle + M4CL_MCR_CHANNEL_ITEMS + ai_iItemPosition * M4CL_MCR_CHANNEL_ITEM_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfTis( void ) const
{

    m4uint32_t	iTisHandle ;


    iTisHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_TIS_START ) ;

    return( M4ClUnPackInt16( m_pcBase, iTisHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelTiHandleByPosition( m4uint16_t ai_iTiPosition ) const
{

    m4uint32_t	iTisHandle ;
    m4uint32_t	iHandle ;


    iTisHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_TIS_START ) ;
    iHandle = iTisHandle + M4CL_MCR_CHANNEL_TIS + ai_iTiPosition * M4CL_MCR_CHANNEL_TI_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_CHANNEL_TI_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelTiIdByPosition( m4uint16_t ai_iTiPosition ) const
{

    m4uint32_t	iTisHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iTisHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_TIS_START ) ;
    iHandle = iTisHandle + M4CL_MCR_CHANNEL_TIS + ai_iTiPosition * M4CL_MCR_CHANNEL_TI_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfParameters( void ) const
{

    m4uint32_t	iParametersHandle ;


    iParametersHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_PARAMETERS_START ) ;

    return( M4ClUnPackInt16( m_pcBase, iParametersHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelParameterHandleByPosition( m4uint16_t ai_iParameterPosition ) const
{

    m4uint32_t	iParametersHandle ;
    m4uint32_t	iHandle ;


    iParametersHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_PARAMETERS_START ) ;
    iHandle = iParametersHandle + M4CL_MCR_CHANNEL_PARAMETERS + ai_iParameterPosition * M4CL_MCR_CHANNEL_PARAMETER_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfRSMs( void ) const
{

    m4uint32_t	iRSMsHandle ;


    iRSMsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_RSMS_START ) ;

    return( M4ClUnPackInt16( m_pcBase, iRSMsHandle ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelRSMIdByPosition( m4uint16_t ai_iRSMPosition ) const
{

    m4uint32_t	iRSMsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iRSMsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_RSMS_START ) ;
    iHandle = iRSMsHandle + M4CL_MCR_CHANNEL_RSMS + ai_iRSMPosition * M4CL_MCR_CHANNEL_RSMS_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4pcchar_t	ClCompiledMCR :: GetChannelRSMMaskIdByPosition( m4uint16_t ai_iRSMPosition ) const
{

    m4uint32_t	iRSMsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iRSMsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_RSMS_START ) ;
    iHandle = iRSMsHandle + M4CL_MCR_CHANNEL_RSMS + ai_iRSMPosition * M4CL_MCR_CHANNEL_RSMS_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_CHANNEL_RSMS_MASK_ID ) ;

    return( m_pcBase + iIdHandle ) ;
}

m4uint16_t	ClCompiledMCR :: GetChannelNumberOfBusinessMethods( void ) const
{

    m4uint32_t	iBusinessMethodsHandle ;


    iBusinessMethodsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_BUSINESS_METHOD_START ) ;

    return( M4ClUnPackInt16( m_pcBase, iBusinessMethodsHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetChannelBusinessMethodHandleByPosition( m4uint16_t ai_iBusinessMethodPosition ) const
{

    m4uint32_t	iBusinessMethodsHandle ;
    m4uint32_t	iHandle ;


    iBusinessMethodsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_BUSINESS_METHOD_START ) ;
    iHandle = iBusinessMethodsHandle + M4CL_MCR_CHANNEL_BUSINESS_METHODS + ai_iBusinessMethodPosition * M4CL_MCR_CHANNEL_BUSINESS_METHOD_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_CHANNEL_BUSINESS_METHOD_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetChannelBusinessMethodIdByPosition( m4uint16_t ai_iBusinessMethodPosition ) const
{

    m4uint32_t	iBusinessMethodsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iBusinessMethodsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_BUSINESS_METHOD_START ) ;
    iHandle = iBusinessMethodsHandle + M4CL_MCR_CHANNEL_BUSINESS_METHODS + ai_iBusinessMethodPosition * M4CL_MCR_CHANNEL_BUSINESS_METHOD_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint32_t	ClCompiledMCR :: GetNodeReadSentenceHandle( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;

    return( M4ClUnPackInt32( m_pcBase, iTiHandle + M4CL_MCR_TI_READ_SENTENCE ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeWriteSentenceHandle( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;

    return( M4ClUnPackInt32( m_pcBase, iTiHandle + M4CL_MCR_TI_WRITE_SENTENCE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeReadSentenceId( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;
    m4uint32_t	iSentenceHandle ;
    m4pcchar_t	pccSentence ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;
    iSentenceHandle = M4ClUnPackInt32( m_pcBase, iTiHandle + M4CL_MCR_TI_READ_SENTENCE ) ;

    if( iSentenceHandle != 0 )
    {
        pccSentence = GetSentenceId( iSentenceHandle ) ;
    }
    else
    {
        pccSentence = NULL ;
    }

    return( pccSentence ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeWriteSentenceId( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;
    m4uint32_t	iSentenceHandle ;
    m4pcchar_t	pccSentence ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;
    iSentenceHandle = M4ClUnPackInt32( m_pcBase, iTiHandle + M4CL_MCR_TI_WRITE_SENTENCE ) ;

    if( iSentenceHandle != 0 )
    {
        pccSentence = GetSentenceId( iSentenceHandle ) ;
    }
    else
    {
        pccSentence = NULL ;
    }

    return( pccSentence ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeReadObjectId( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;
    m4uint32_t	iHandle ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iTiHandle + M4CL_MCR_TI_READ_OBJECT_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeWriteObjectId( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;
    m4uint32_t	iHandle ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iTiHandle + M4CL_MCR_TI_WRITE_OBJECT_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeTemporality( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iTiHandle + M4CL_MCR_TI_TEMPORARY_ID ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeCompleteness( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iTiHandle + M4CL_MCR_TI_COMPLETE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodePeriodCorrected( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iTiHandle + M4CL_MCR_TI_CORRECTED ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeOwnSentence( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iTiHandle ;


    iTiHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iTiHandle + M4CL_MCR_TI_OWN_SENTENCE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeId( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeM4ObjId( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_M4OBJ_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeName( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_NAME ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeTiId( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_TI_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeGroupObjects( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_GROUP_OBJECTS ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeStartDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_START_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeEndDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_END_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeStartCorDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_START_COR_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeEndCorDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_END_COR_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeVirtualFlagItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_VIRTUAL_FLAG_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeFilterStartDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_FILTER_START_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeFilterEndDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_FILTER_END_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeFilterStartCorDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_FILTER_START_COR_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeFilterEndCorDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_FILTER_END_COR_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeCurrencyTypeItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CURRENCY_TYPE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeCurrencyExchDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CURRENCY_EXCH_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeChangeReasonItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CHANGE_REASON_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeDmdComponentItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DMD_COMPONENT_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeDmdValueItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DMD_VALUE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeLoadSQLItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_LOAD_SQL_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodePriorityItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PRIORITY_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeImputeDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IMPUTE_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodePayDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PAY_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodePayTypeItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PAY_TYPE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodePayFrequencyItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PAY_FREQUENCY_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeImputePayTypeItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IMPUTE_PAY_TYPE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeImputePayFrequencyItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IMPUTE_PAY_FREQUENCY_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeClosingDateItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CLOSING_DATE_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeSysParamsItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SYS_PARAMS_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeBDLChecksItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_BDL_CHECKS_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeRAMOrderByItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_RAM_ORDER_BY_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeOrganizationIdItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_ID_ORGANIZATION ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeLoadBlkItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_LOAD_BLK_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeDeleteBlkItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DELETE_BLK_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeUpdateBlkItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_UPDATE_BLK_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeInsertBlkItem( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_INSERT_BLK_ITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeDefaultIndex( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DEFAULT_INDEX ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeNumRows( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_NUM_ROWS ) ) ;
}

m4int32_t	ClCompiledMCR :: GetNodeNumKeepRows( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_NUM_KEEP_ROWS ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeNumRowsDB( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_NUM_ROWS_DB ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeOleDispId( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_OLE_DISP_ID ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeOleNodeInterId( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_OLE_NODE_INTER_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeMaxNumBlocks( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_MAX_NUM_BLOCKS ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeMaxNumRecords( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_MAX_NUM_RECORDS ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeIndex( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_INDEX ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeType( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_TYPE ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeOrder( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_ORDER ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeCapacity( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CAPACITY ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeIsRoot( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IS_ROOT ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeIsRealRoot( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IS_REAL_ROOT ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeAutoLoad( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_AUTOLOAD ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeIsOrdered( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IS_ORDERED ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeIsVisible( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IS_VISIBLE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeDynFilter( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DYN_FILTER ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeIdAutoFilter( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_ID_AUTO_FILTER ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeIsExternal( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IS_EXTERNAL ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeIsQBFFilter( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IS_QBF_FILTER ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeAffectsDB( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_AFFECTS_DB ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeDBReload( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DB_RELOAD ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeTiInheritLevel( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_TI_INHERIT_LEVEL ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeIsOverWritten( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_IS_OVERWRITTEN ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfItems( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_NITEM_NUMBER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeItemHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iItemPosition ) const
{
    
    m4uint32_t	iHandle ;
    m4uint32_t	iItemHandle ;


    iItemHandle = ai_iNodeHandle + M4CL_MCR_NODE_NITEMS + ai_iItemPosition * M4CL_MCR_NODE_NITEM_SIZE ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemHandle ) ;

    return( M4ClUnPackInt32( m_pcBase, iItemHandle + M4CL_MCR_NODE_NITEM_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeItemIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iIdHandle = ai_iNodeHandle + M4CL_MCR_NODE_NITEMS + ai_iItemPosition * M4CL_MCR_NODE_NITEM_SIZE ;
    iHandle = M4ClUnPackInt32( m_pcBase, iIdHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfConnectors( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iConnectorsHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConnectorsHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfRBConnectors( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iConnectorsHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConnectorsHandle + M4CL_MCR_NODE_RB_CONNECTOR_NUMBER ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfBBConnectors( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iConnectorsHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConnectorsHandle + M4CL_MCR_NODE_BB_CONNECTOR_NUMBER ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfNRConnectors( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iConnectorsHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConnectorsHandle + M4CL_MCR_NODE_NR_CONNECTOR_NUMBER ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeRBFirstConnector( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iConnectorsHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConnectorsHandle + M4CL_MCR_NODE_RB_FIRST_CONNECTOR ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeBBFirstConnector( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iConnectorsHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConnectorsHandle + M4CL_MCR_NODE_BB_FIRST_CONNECTOR ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNRFirstConnector( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iConnectorsHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConnectorsHandle + M4CL_MCR_NODE_NR_FIRST_CONNECTOR ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfAutoConnectors( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iConnectorsHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConnectorsHandle + M4CL_MCR_NODE_AUTO_CONNECTOR_NUMBER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeConnectorHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iConnectedPosition ) const
{

    m4uint32_t	iConnectorsHandle ;
    m4uint32_t	iHandle ;


    iConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CONNECTORS_START ) ;

	if( iConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iConnectorsHandle + M4CL_MCR_NODE_CONNECTORS + ai_iConnectedPosition * M4_SIZE_OF_INT32 ;

    return( M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfInvConnectors( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iInvConnectorsHandle ;


    iInvConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_INV_CONNECTORS_START ) ;

	if( iInvConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iInvConnectorsHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeInvConnectorHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iInvConnectedPosition ) const
{

    m4uint32_t	iInvConnectorsHandle ;
    m4uint32_t	iHandle ;


    iInvConnectorsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_INV_CONNECTORS_START ) ;

	if( iInvConnectorsHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iInvConnectorsHandle + M4CL_MCR_NODE_INV_CONNECTORS + ai_iInvConnectedPosition * M4_SIZE_OF_INT32 ;

    return( iHandle + M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfGroups( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iGroupsHandle ;


    iGroupsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_GROUPS_START ) ;

	if( iGroupsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iGroupsHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeGroupHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iGroupPosition ) const
{

    m4uint32_t	iGroupsHandle ;
    m4uint32_t	iHandle ;


    iGroupsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_GROUPS_START ) ;

	if( iGroupsHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iGroupsHandle + M4CL_MCR_NODE_GROUPS + ai_iGroupPosition * M4CL_MCR_NODE_GROUP_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_GROUP_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeGroupIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iGroupPosition ) const
{

    m4uint32_t	iGroupsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iGroupsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_GROUPS_START ) ;

	if( iGroupsHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iGroupsHandle + M4CL_MCR_NODE_GROUPS + ai_iGroupPosition * M4CL_MCR_NODE_GROUP_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfPlugs( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iPlugsHandle ;


    iPlugsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PLUGS_START ) ;

	if( iPlugsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iPlugsHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodePlugHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iPlugPosition ) const
{

    m4uint32_t	iPlugsHandle ;
    m4uint32_t	iHandle ;


    iPlugsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PLUGS_START ) ;

	if( iPlugsHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iPlugsHandle + M4CL_MCR_NODE_PLUGS + ai_iPlugPosition * M4CL_MCR_NODE_PLUG_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_PLUG_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodePlugFatherChanneIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iPlugPosition ) const
{

    m4uint32_t	iPlugsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iPlugsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PLUGS_START ) ;

	if( iPlugsHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iPlugsHandle + M4CL_MCR_NODE_PLUGS + ai_iPlugPosition * M4CL_MCR_NODE_PLUG_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_PLUG_CHANNEL_ID ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4pcchar_t	ClCompiledMCR :: GetNodePlugFatherNodeIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iPlugPosition ) const
{

    m4uint32_t	iPlugsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iPlugsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PLUGS_START ) ;

	if( iPlugsHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iPlugsHandle + M4CL_MCR_NODE_PLUGS + ai_iPlugPosition * M4CL_MCR_NODE_PLUG_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_PLUG_NODE_ID ) ;

    return( m_pcBase + iIdHandle + iHandle + M4CL_MCR_NODE_PLUG_NODE_ID ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfIndexes( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iIndexesHandle ;


    iIndexesHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_INDEXES_START ) ;

	if( iIndexesHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iIndexesHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeIndexHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iIndexPosition ) const
{

    m4uint32_t	iIndexesHandle ;
    m4uint32_t	iHandle ;


    iIndexesHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_INDEXES_START ) ;

	if( iIndexesHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iIndexesHandle + M4CL_MCR_NODE_INDEXES + ai_iIndexPosition * M4CL_MCR_NODE_INDEX_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_INDEX_HANDLE ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeIndexIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iIndexPosition ) const
{

    m4uint32_t	iIndexesHandle ;
    m4uint32_t	iHandle ;


    iIndexesHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_INDEXES_START ) ;

	if( iIndexesHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iIndexesHandle + M4CL_MCR_NODE_INDEXES + ai_iIndexPosition * M4CL_MCR_NODE_INDEX_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfAlias( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iAliasHandle ;


    iAliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_ALIAS_START ) ;

	if( iAliasHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iAliasHandle ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeAliasIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iAliasPosition ) const
{

    m4uint32_t	iAliasHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iAliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_ALIAS_START ) ;

	if( iAliasHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iAliasHandle + M4CL_MCR_NODE_ALIAS + ai_iAliasPosition * M4CL_MCR_NODE_ALIAS_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint32_t	ClCompiledMCR :: GetNodeAliasNodeHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iAliasPosition ) const
{

    m4uint32_t	iAliasHandle ;
    m4uint32_t	iHandle ;


    iAliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_ALIAS_START ) ;

	if( iAliasHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iAliasHandle + M4CL_MCR_NODE_ALIAS + ai_iAliasPosition * M4CL_MCR_NODE_ALIAS_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_ALIAS_NODE_HANDLE ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfT3Alias( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iT3AliasHandle ;


    iT3AliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_T3_ALIAS_START ) ;

	if( iT3AliasHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iT3AliasHandle ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeT3AliasIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iT3AliasPosition ) const
{

    m4uint32_t	iT3AliasHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iT3AliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_T3_ALIAS_START ) ;

	if( iT3AliasHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iT3AliasHandle + M4CL_MCR_NODE_T3_ALIAS + ai_iT3AliasPosition * M4CL_MCR_NODE_T3_ALIAS_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4pcchar_t	ClCompiledMCR :: GetNodeT3AliasInstanceByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iT3AliasPosition ) const
{

    m4uint32_t	iT3AliasHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iT3AliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_T3_ALIAS_START ) ;

	if( iT3AliasHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iT3AliasHandle + M4CL_MCR_NODE_T3_ALIAS + ai_iT3AliasPosition * M4CL_MCR_NODE_T3_ALIAS_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_T3_ALIAS_INSTANCE_ID ) ;

    return( m_pcBase + iIdHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeT3AliasInstanceHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iT3AliasPosition ) const
{

    m4uint32_t	iT3AliasHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;
    m4uint32_t	iInstanceHandle ;


    iT3AliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_T3_ALIAS_START ) ;

	if( iT3AliasHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iT3AliasHandle + M4CL_MCR_NODE_T3_ALIAS + ai_iT3AliasPosition * M4CL_MCR_NODE_T3_ALIAS_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_T3_ALIAS_INSTANCE_ID ) ;

    iInstanceHandle = GetChannelInstanceHandleById( m_pcBase + iIdHandle ) ;

    return( iInstanceHandle ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfDmds( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iDmdsHandle ;


    iDmdsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DMDS_START ) ;

	if( iDmdsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iDmdsHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeDmdHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iDmdPosition ) const
{

    m4uint32_t	iDmdsHandle ;
    m4uint32_t	iHandle ;


    iDmdsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DMDS_START ) ;

	if( iDmdsHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iDmdsHandle + M4CL_MCR_NODE_DMDS + ai_iDmdPosition * M4CL_MCR_NODE_DMD_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_DMDS_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeDmdDmdComponentIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iDmdPosition ) const
{

    m4uint32_t	iDmdsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iDmdsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DMDS_START ) ;

	if( iDmdsHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iDmdsHandle + M4CL_MCR_NODE_DMDS + ai_iDmdPosition * M4CL_MCR_NODE_DMD_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4pcchar_t	ClCompiledMCR :: GetNodeDmdDmdFieldIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iDmdPosition ) const
{

    m4uint32_t	iDmdsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iDmdsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DMDS_START ) ;

	if( iDmdsHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iDmdsHandle + M4CL_MCR_NODE_DMDS + ai_iDmdPosition * M4CL_MCR_NODE_DMD_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_DMDS_DMD_FIELD ) ;

    return( m_pcBase + iIdHandle + iHandle + M4CL_MCR_NODE_DMDS_DMD_FIELD ) ; // porque es relativo
}

m4pcchar_t	ClCompiledMCR :: GetNodeDmdDmdIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iDmdPosition ) const
{

    m4uint32_t	iDmdsHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iDmdsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DMDS_START ) ;

	if( iDmdsHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iDmdsHandle + M4CL_MCR_NODE_DMDS + ai_iDmdPosition * M4CL_MCR_NODE_DMD_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_DMDS_DMD ) ;

    return( m_pcBase + iIdHandle + iHandle + M4CL_MCR_NODE_DMDS_DMD ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfQBFFilters( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iFiltersHandle ;


    iFiltersHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_QBF_FILTERS_START ) ;

	if( iFiltersHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iFiltersHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeQBFFilterHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iFilterPosition ) const
{

    m4uint32_t	iFiltersHandle ;
    m4uint32_t	iHandle ;


    iFiltersHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_QBF_FILTERS_START ) ;

	if( iFiltersHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iFiltersHandle + M4CL_MCR_NODE_FILTERS + ai_iFilterPosition * M4CL_MCR_NODE_FILTER_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_FILTER_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeQBFFilterIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iFilterPosition ) const
{

    m4uint32_t	iFiltersHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iFiltersHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_QBF_FILTERS_START ) ;

	if( iFiltersHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iFiltersHandle + M4CL_MCR_NODE_FILTERS + ai_iFilterPosition * M4CL_MCR_NODE_FILTER_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfFindFilters( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iFiltersHandle ;


    iFiltersHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_FIND_FILTERS_START ) ;

	if( iFiltersHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iFiltersHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeFindFilterHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iFilterPosition ) const
{

    m4uint32_t	iFiltersHandle ;
    m4uint32_t	iHandle ;


    iFiltersHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_FIND_FILTERS_START ) ;

	if( iFiltersHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iFiltersHandle + M4CL_MCR_NODE_FILTERS + ai_iFilterPosition * M4CL_MCR_NODE_FILTER_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_FILTER_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeFindFilterIdByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iFilterPosition ) const
{

    m4uint32_t	iFiltersHandle ;
    m4uint32_t	iHandle ;
    m4uint32_t	iIdHandle ;


    iFiltersHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_FIND_FILTERS_START ) ;

	if( iFiltersHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iFiltersHandle + M4CL_MCR_NODE_FILTERS + ai_iFilterPosition * M4CL_MCR_NODE_FILTER_SIZE ;
    iIdHandle = M4ClUnPackInt32( m_pcBase, iHandle ) ;

    return( m_pcBase + iIdHandle + iHandle ) ; // porque es relativo
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfSysSentences( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iSysSentencesHandle ;


    iSysSentencesHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SYS_SENTENCES_START ) ;

	if( iSysSentencesHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iSysSentencesHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeSysSentenceHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iSysSentencePosition ) const
{

    m4uint32_t	iSysSentencesHandle ;
    m4uint32_t	iHandle ;


    iSysSentencesHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SYS_SENTENCES_START ) ;

	if( iSysSentencesHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iSysSentencesHandle + M4CL_MCR_NODE_SYS_SENTENCES + ai_iSysSentencePosition * M4CL_MCR_NODE_SYS_SENTENCE_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_SYS_SENTENCE_HANDLE ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeNumberOfSysHints( m4uint32_t ai_iNodeHandle ) const
{

    m4uint32_t	iSysHintsHandle ;


    iSysHintsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SYS_HINTS_START ) ;

	if( iSysHintsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iSysHintsHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeSysHintHandleByPosition( m4uint32_t ai_iNodeHandle, m4uint16_t ai_iSysHintPosition ) const
{

    m4uint32_t	iSysHintsHandle ;
    m4uint32_t	iHandle ;


    iSysHintsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SYS_HINTS_START ) ;

	if( iSysHintsHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iSysHintsHandle + M4CL_MCR_NODE_SYS_HINTS + ai_iSysHintPosition * M4CL_MCR_NODE_SYS_HINT_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_NODE_SYS_HINT_HANDLE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetTiId( m4uint32_t ai_iTiHandle ) const
{

    m4uint32_t	iHandle ;


	iHandle = M4ClUnPackInt32( m_pcBase, ai_iTiHandle + M4CL_MCR_TI_ID_TI ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint16_t	ClCompiledMCR :: GetTiNumberOfItems( m4uint32_t ai_iTiHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iTiHandle + M4CL_MCR_TI_ITEM_NUMBER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetTiItemHandleByPosition( m4uint32_t ai_iTiHandle, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iHandle ;


    iHandle = ai_iTiHandle + M4CL_MCR_TI_ITEMS + ai_iItemPosition * M4CL_MCR_TI_ITEM_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetTiReferenceNodeHandleByPosition( m4uint32_t ai_iTiHandle, m4uint16_t ai_iReferencePosition ) const
{
    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iTiHandle + M4CL_MCR_TI_REFERENCES_START ) + ai_iReferencePosition * M4CL_MCR_REFERENCE_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_REFERENCE_NODE ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetTiReferenceNumberByPosition( m4uint32_t ai_iTiHandle, m4uint16_t ai_iReferencePosition ) const
{
    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iTiHandle + M4CL_MCR_TI_REFERENCES_START ) + ai_iReferencePosition * M4CL_MCR_REFERENCE_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_REFERENCE_NUMBER ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemId( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_ID  ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemTiId( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_TI_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemDmd( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DMD ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemDmdComponent( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DMD_COMPONENT ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemDmdCrosstab( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DMD_CROSSTAB ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemDmdField( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DMD_FIELD ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemDBPrecision( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DB_PRECISION ) ) ;
}

m4int32_t	ClCompiledMCR :: GetItemPrecision( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_PRECISION ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemMaxSize( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_MAX_SIZE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemReadObjectAlias( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_READ_OBJECT_ALIAS ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemWriteObjectAlias( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_WRITE_OBJECT_ALIAS ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemReadFieldId( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_READ_FIELD_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemWriteFieldId( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_WRITE_FIELD_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemReadObjectId( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_READ_OBJECT_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemWriteObjectId( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_WRITE_OBJECT_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4int32_t	ClCompiledMCR :: GetItemSynonymId( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_SYNONYM_ID ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemIndex( m4uint32_t ai_iItemHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_INDEX ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemOrder( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt16( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_ORDER ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemFlag( m4uint32_t ai_iItemHandle, m4uint16_t ai_iFlag ) const
{

    m4uint16_t	iFlag ;
    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

	iFlag = M4ClUnPackInt16( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_FLAGS ) ;

    return( ( iFlag & ai_iFlag ) != 0 ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemType( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemTheoreticScope( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_SCOPE ) ) ;

}

m4uint8_t	ClCompiledMCR :: GetItemM4Type( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_M4_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemCppType( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_CPP_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemScale( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_SCALE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemSliceBhrv( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_SLICE_BHRV ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemSliceSplit( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_SLICE_SPLIT ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIdTotalize( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_TOTALIZE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemSliceTotalize( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_SLICE_TOT_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemTransactionMode( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_TRANSACTION_MODE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemMethodLevel( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_METHOD_LEVEL ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIsVariableLength( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemSentTotType( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_SENT_TOT_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemVariableArguments( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_HAS_VARIABLE_ARGS ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIsVisible( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_IS_VISIBLE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemModifyData( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_MODIFY_DATA ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemDefaultValueFlag( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DEFAULT_VALUE_FLAG ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemInternalType( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_INTERNAL_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemConvertFunction( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_CONVERT_FUNCTION ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIsInherited( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_IS_INHERITED ) ) ;
}
m4uint8_t	ClCompiledMCR :: GetItemIsPublic( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_IS_PUBLIC ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIsParameter( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_IS_PARAMETER ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemCheckNotNull( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_CHECK_NOT_NULL ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemClientNotNull( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_CLIENT_NOT_NULL ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemNotNull( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_NOT_NULL ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemAffectsDB( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_AFFECTS_DB ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemGenerateSlices( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_GENERATE_SLICES ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIsPK( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_IS_PK ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIsNodal( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemFlag( ai_iItemHandle, M4CL_MCR_ITEM_FLAG_NODAL_ITEM ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfArguments( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt16( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_ARGUMENT_NUMBER ) ) ;

}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfReferedArguments( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemDefHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    return( M4ClUnPackInt16( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_ARGUMENT_REFERENCES ) ) ;

}

m4pcchar_t	ClCompiledMCR :: GetItemArgumentIdByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iArgumentHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iArgumentHandle = iItemDefHandle + M4CL_MCR_ITEM_ARGUMENTS + ai_iArgumentPosition * M4CL_MCR_ITEM_ARGUMENT_SIZE ;
    iHandle = M4ClUnPackInt32( m_pcBase, iArgumentHandle ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemArgumentNameByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iArgumentHandle ;
    m4uint32_t	iHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iArgumentHandle = iItemDefHandle + M4CL_MCR_ITEM_ARGUMENTS + ai_iArgumentPosition * M4CL_MCR_ITEM_ARGUMENT_SIZE ;
    iHandle = M4ClUnPackInt32( m_pcBase, iArgumentHandle + M4CL_MCR_ARGUMENT_NAME ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint16_t	ClCompiledMCR ::  GetItemArgumentPrecisionByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iArgumentHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iArgumentHandle = iItemDefHandle + M4CL_MCR_ITEM_ARGUMENTS + ai_iArgumentPosition * M4CL_MCR_ITEM_ARGUMENT_SIZE ;

    return( M4ClUnPackInt16( m_pcBase, iArgumentHandle + M4CL_MCR_ARGUMENT_PREC ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemArgumentTypeByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iArgumentHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iArgumentHandle = iItemDefHandle + M4CL_MCR_ITEM_ARGUMENTS + ai_iArgumentPosition * M4CL_MCR_ITEM_ARGUMENT_SIZE ;

    return( M4ClUnPackInt8( m_pcBase, iArgumentHandle + M4CL_MCR_ARGUMENT_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemArgumentM4TypeByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iArgumentPosition ) const
{

    m4uint32_t	iItemDefHandle ;
    m4uint32_t	iArgumentHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;
    iArgumentHandle = iItemDefHandle + M4CL_MCR_ITEM_ARGUMENTS + ai_iArgumentPosition * M4CL_MCR_ITEM_ARGUMENT_SIZE ;

    return( M4ClUnPackInt8( m_pcBase, iArgumentHandle + M4CL_MCR_ARGUMENT_M4_TYPE ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemDefinitionHandle( m4uint32_t ai_iItemHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemNodeHandle( m4uint32_t ai_iItemHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_NODE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemName( m4uint32_t ai_iItemHandle ) const
{
    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iHandle ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_NAME  ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemSliceItem( m4uint32_t ai_iItemHandle ) const
{
    m4uint32_t	iItemRefHandle ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    return( M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_SLICE_NITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemAuxItem( m4uint32_t ai_iItemHandle ) const
{
    m4uint32_t	iItemRefHandle ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    return( M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_AUX_NITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemAuxItemProrat( m4uint32_t ai_iItemHandle ) const
{
    m4uint32_t	iItemRefHandle ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    return( M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_AUX_NITEM_PRORAT ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIsMyItem( m4uint32_t ai_iItemHandle ) const
{
	m4uint8_t	iFlag ;

	iFlag = M4ClUnPackInt8( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_FLAGS ) ;
    return( ( iFlag & M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) != 0 ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemIsEvent( m4uint32_t ai_iItemHandle ) const
{
	m4uint8_t	iFlag ;

	iFlag = M4ClUnPackInt8( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_FLAGS ) ;
    return( ( iFlag & M4CL_MCR_ITEM_FLAG_IS_EVENT ) != 0 ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemLevel( m4uint32_t ai_iItemHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_LEVEL ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfConnectedItems( m4uint32_t ai_iItemHandle ) const
{
    m4uint32_t	iItemRefHandle ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    return( M4ClUnPackInt16( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_CONNECTED_ITEM_NUMBER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemConnectorHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iConnectorPosition ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iHandle ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iHandle = iItemRefHandle + M4CL_MCR_RITEM_CONNECTED_ITEMS + ai_iConnectorPosition * M4_SIZE_OF_INT32 ;

    return( M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfConditionants( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iConditionantHandle ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iConditionantHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_CONDITIONANTS_START ) ;

	if( iConditionantHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iConditionantHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemConditionantHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iConditionantPosition ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iConditionantHandle ;
    m4uint32_t	iHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iConditionantHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_CONDITIONANTS_START ) ;

	if( iConditionantHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iConditionantHandle + M4CL_MCR_RITEM_CONDITIONANTS + ai_iConditionantPosition * M4CL_MCR_RITEM_CONDITIONANT_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfDependents( m4uint32_t ai_iItemHandle )
{
	m_oDependents.Calculate( ai_iItemHandle, this ) ;
	return( m_oDependents.GetLength() ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemDependentHandleByPosition ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iDependentPosition )
{
	m_oDependents.Calculate( ai_iItemHandle, this ) ;
	return( m_oDependents.GetDependent( ai_iDependentPosition ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfAssigned( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iAssignedHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iAssignedHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_ASSIGNED_START ) ;

	if( iAssignedHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iAssignedHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemAssignedHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iAssignedPosition ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iAssignedHandle ;
    m4uint32_t	iHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iAssignedHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_ASSIGNED_START ) ;

	if( iAssignedHandle == 0 )
	{
		return( 0 ) ;
	}
    
	iHandle = iAssignedHandle + M4CL_MCR_RITEM_ASSIGNED + ai_iAssignedPosition * M4CL_MCR_RITEM_ASSIGNED_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfAssignedDependents( m4uint32_t ai_iItemHandle ) 
{
	m_oAssignedDependents.Calculate( ai_iItemHandle, this ) ;
	return( m_oAssignedDependents.GetLength() ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemAssignedDependentHandleByPosition ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iDependentPosition )
{
	m_oAssignedDependents.Calculate( ai_iItemHandle, this ) ;
	return( m_oAssignedDependents.GetDependent( ai_iDependentPosition ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfExecuted( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iExecutedHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iExecutedHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_EXECUTED_START ) ;

	if( iExecutedHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iExecutedHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemExecutedHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iExecutedPosition ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iExecutedHandle ;
    m4uint32_t	iHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iExecutedHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_EXECUTED_START ) ;

	if( iExecutedHandle == 0 )
	{
		return( 0 ) ;
	}

    iHandle = iExecutedHandle + M4CL_MCR_RITEM_EXECUTED + ai_iExecutedPosition * M4CL_MCR_RITEM_EXECUTED_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR ::GetItemNumberOfExecutedDependents ( m4uint32_t ai_iItemHandle )
{
	m_oExecutedDependents.Calculate( ai_iItemHandle, this ) ;
	return( m_oExecutedDependents.GetLength() ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemExecutedDependentHandleByPosition ( m4uint32_t ai_iItemHandle, m4uint16_t ai_iDependentPosition )
{
	m_oExecutedDependents.Calculate( ai_iItemHandle, this ) ;
	return( m_oExecutedDependents.GetDependent( ai_iDependentPosition ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfFormIds( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iFormIdsHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormIdsHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORM_IDS_START ) ;

	if( iFormIdsHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iFormIdsHandle ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetItemFormIdByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iFormIdPosition ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iFormIdsHandle ;
    m4uint32_t	iHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormIdsHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORM_IDS_START ) ;

	if( iFormIdsHandle == 0 )
	{
		return( "" ) ;
	}

    iHandle = iFormIdsHandle + M4CL_MCR_RITEM_FORM_IDS + ai_iFormIdPosition * M4CL_MCR_RITEM_FORM_IDS_SIZE ;

    return( m_pcBase + M4ClUnPackInt32( m_pcBase, iHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfSlices( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iFormulaHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormulaHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORMULAS_START ) ;

	if( iFormulaHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iFormulaHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemNumberOfFormulas( m4uint32_t ai_iItemHandle ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iFormulaHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormulaHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORMULAS_START ) ;

	if( iFormulaHandle == 0 )
	{
		return( 0 ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, iFormulaHandle + M4CL_MCR_FORMULA_FORMULA_NUMBER ) ) ;
}

m4date_t	ClCompiledMCR :: GetItemSliceStartDate( m4uint32_t ai_iItemHandle, m4uint16_t ai_iSlicePosition ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iFormulaHandle ;
    m4uint32_t	iSliceHandle ;
    m4uint16_t  iNumberOfFormulas ;


    iNumberOfFormulas = GetItemNumberOfFormulas( ai_iItemHandle ) ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormulaHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORMULAS_START ) ;

	if( iFormulaHandle == 0 )
	{
		return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
	}

    iSliceHandle = iFormulaHandle + M4CL_MCR_FORMULA_FORMULA_START + ai_iSlicePosition * M4_ROUND_TO_8( 2 * M4_SIZE_OF_DATE + iNumberOfFormulas * M4_SIZE_OF_INT32 ) ;

    return( M4ClUnPackDate( m_pcBase, iSliceHandle ) ) ;
}

m4date_t	ClCompiledMCR :: GetItemSliceEndDate( m4uint32_t ai_iItemHandle, m4uint16_t ai_iSlicePosition ) const
{

    m4uint32_t	iItemRefHandle ;
    m4uint32_t	iFormulaHandle ;
    m4uint32_t	iSliceHandle ;
    m4uint16_t  iNumberOfFormulas ;


    iNumberOfFormulas = GetItemNumberOfFormulas( ai_iItemHandle ) ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormulaHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORMULAS_START ) ;

	if( iFormulaHandle == 0 )
	{
		return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
	}

    iSliceHandle = iFormulaHandle + M4CL_MCR_FORMULA_FORMULA_START + ai_iSlicePosition * M4_ROUND_TO_8( 2 * M4_SIZE_OF_DATE + iNumberOfFormulas * M4_SIZE_OF_INT32 ) ;

    return( M4ClUnPackDate( m_pcBase, iSliceHandle + M4CL_MCR_FORMULA_END_DATE ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetCodeNumberOfVariales( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( 0 ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetNumVars() ) ;
}

m4uint8_t	ClCompiledMCR :: GetCodeMustAssign( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( 1 ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetMustAssign() ) ;
}

m4uint8_t	ClCompiledMCR :: GetCodeAutoGenerated( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( 0 ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetIsAutogenerated() ) ;
}

m4uint8_t	ClCompiledMCR :: GetCodePriority( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( M4CL_RULE_MAX_PRIORITY ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetPriority() ) ;
}

m4uint8_t	ClCompiledMCR :: GetCodeType( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( M4CL_LENGUAGE_LN4 ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetType() ) ;
}

m4uint8_t	ClCompiledMCR :: GetCodeLevel( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( 0 ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetLevel() ) ;
}

m4uint32_t	ClCompiledMCR :: GetCodeNumberOfInstructions( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( 0 ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetNumberOfInstructions() ) ;
}

m4uint32_t	ClCompiledMCR :: GetCodeSize( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( 0 ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetFormulaSize() ) ;
}

m4pcchar_t	ClCompiledMCR :: GetCodeRuleTiId( m4pcchar_t ai_pccCode ) const
{

	m4uint32_t		iHandle ;
	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( "" ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	iHandle = oFormula.GetRuleTI() ;

	if( iHandle == 0 )
	{
		return( "" ) ;
	}

	return( m_pcBase + iHandle ) ;
}

m4date_t	ClCompiledMCR :: GetCodeStartDate( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( M4CL_MINUS_INFINITE_IN_JULIAN ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetStartDate() ) ;
}

m4date_t	ClCompiledMCR :: GetCodeEndDate( m4pcchar_t ai_pccCode ) const
{

	ClDecodeFormula	oFormula ;

	
	if( ai_pccCode == NULL )
	{
		return( M4CL_PLUS_INFINITE_IN_JULIAN ) ;
	}

	oFormula.Init( ai_pccCode ) ;

	return( oFormula.GetEndDate() ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeConnectorFatherNodeHandle( m4uint32_t ai_iNodeConnectorHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeConnectorHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeConnectorSonNodeHandle( m4uint32_t ai_iNodeConnectorHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeConnectorHandle + M4CL_MCR_NODE_CONNECTOR_SON_NODE ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetNodeConnectorSentenceHandle( m4uint32_t ai_iNodeConnectorHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iNodeConnectorHandle + M4CL_MCR_NODE_CONNECTOR_SENTENCE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetNodeConnectorSentenceId( m4uint32_t ai_iNodeConnectorHandle ) const
{

    m4uint32_t	iSentenceHandle ;
    m4pcchar_t pccSentence;


    iSentenceHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeConnectorHandle + M4CL_MCR_NODE_CONNECTOR_SENTENCE ) ;

    if( iSentenceHandle != 0 )
    {
        pccSentence = GetSentenceId( iSentenceHandle ) ;
    }
    else
    {
        pccSentence = NULL ;
    }

    return( pccSentence ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeConnectorType( m4uint32_t ai_iNodeConnectorHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeConnectorHandle + M4CL_MCR_NODE_CONNECTOR_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeConnectorExpandDelete( m4uint32_t ai_iNodeConnectorHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeConnectorHandle + M4CL_MCR_NODE_CONNECTOR_EXPAND_DELETE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeConnectorExpandUndo( m4uint32_t ai_iNodeConnectorHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeConnectorHandle + M4CL_MCR_NODE_CONNECTOR_EXPAND_UNDO ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemConnectorNodeConnectorHandle( m4uint32_t ai_iItemConnectorHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iItemConnectorHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemConnectorFatherItemHandle( m4uint32_t ai_iItemConnectorHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iItemConnectorHandle + M4CL_MCR_NITEM_CONNECTOR_FATHER_NITEM ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetItemConnectorSonItemHandle( m4uint32_t ai_iItemConnectorHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iItemConnectorHandle + M4CL_MCR_NITEM_CONNECTOR_SON_NITEM ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemConnectorPrecedence( m4uint32_t ai_iItemConnectorHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iItemConnectorHandle + M4CL_MCR_NITEM_CONNECTOR_PRECEDENCE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemConnectorSpin( m4uint32_t ai_iItemConnectorHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iItemConnectorHandle + M4CL_MCR_NITEM_CONNECTOR_SPIN ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemConnectorRelation( m4uint32_t ai_iItemConnectorHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iItemConnectorHandle + M4CL_MCR_NITEM_CONNECTOR_RELATION ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemConnectorContext( m4uint32_t ai_iItemConnectorHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iItemConnectorHandle + M4CL_MCR_NITEM_CONNECTOR_CONTEXT ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemConnectorCsType( m4uint32_t ai_iItemConnectorHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iItemConnectorHandle + M4CL_MCR_NITEM_CONNECTOR_CS_TYPE ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetGroupNumberOfItems( m4uint32_t ai_iGroupHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iGroupHandle ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetGroupItemHandleByPosition( m4uint32_t ai_iGroupHandle, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iItemHandle ;


    iItemHandle = ai_iGroupHandle + M4CL_MCR_GROUP_ITEMS + ai_iItemPosition * M4_SIZE_OF_INT32 ;

    return( M4ClUnPackInt32( m_pcBase, iItemHandle ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetPlugFatherChanneId( m4uint32_t ai_iPlugHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_FATHER_CHANNEL_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetPlugFatherChanneName( m4uint32_t ai_iPlugHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_FATHER_CHANNEL_NAME ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetPlugFatherNodeId( m4uint32_t ai_iPlugHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_FATHER_NODE_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint8_t	ClCompiledMCR :: GetPlugType( m4uint32_t ai_iPlugHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetPlugIsFilter( m4uint32_t ai_iPlugHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_IS_FILTER ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetPlugRelationTypeId( m4uint32_t ai_iPlugHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_ID_RELATION_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetPlugIsMultiselection( m4uint32_t ai_iPlugHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_IS_MULTISELECTION ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetPlugNumberOfItems( m4uint32_t ai_iPlugHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_ITEMS_NUMBER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetPlugItemHandleByPosition ( m4uint32_t ai_iPlugHandle, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iPlugItemHandle ;


    iPlugItemHandle = ai_iPlugHandle + M4CL_MCR_PLUG_ITEMS + ai_iItemPosition * M4_SIZE_OF_INT32 ;

    return( M4ClUnPackInt32( m_pcBase, iPlugItemHandle ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetPlugItemFatherItemId( m4uint32_t ai_iPlugItemHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iPlugItemHandle + M4CL_MCR_PLUG_FATHER_ITEM_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetPlugItemSonItemHandle( m4uint32_t ai_iPlugItemHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iPlugItemHandle + M4CL_MCR_PLUG_SON_ITEM_HANDLE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetPlugItemRelation( m4uint32_t ai_iPlugItemHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iPlugItemHandle + M4CL_MCR_PLUG_RELATION ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetIndexId( m4uint32_t ai_iIndexHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iIndexHandle ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetIndexNumberOfItems( m4uint32_t ai_iIndexHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iIndexHandle + M4CL_MCR_INDEX_ITEM_NUMBER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetIndexItemHandleByPosition( m4uint32_t ai_iIndexHandle, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iIndexItemHandle ;


    iIndexItemHandle = ai_iIndexHandle + M4CL_MCR_INDEX_ITEMS + ai_iItemPosition * M4CL_MCR_INDEX_ITEM_SIZE ;

    return( M4ClUnPackInt32( m_pcBase, iIndexItemHandle ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetIndexFunctionByPosition( m4uint32_t ai_iIndexHandle, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iIndexItemHandle ;


    iIndexItemHandle = ai_iIndexHandle + M4CL_MCR_INDEX_ITEMS + ai_iItemPosition * M4CL_MCR_INDEX_ITEM_SIZE ;

    return( M4ClUnPackInt8( m_pcBase, iIndexItemHandle + M4CL_MCR_INDEX_ITEM_FUNCTION_ID ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetIndexWayByPosition( m4uint32_t ai_iIndexHandle, m4uint16_t ai_iItemPosition ) const
{

    m4uint32_t	iIndexItemHandle ;


    iIndexItemHandle = ai_iIndexHandle + M4CL_MCR_INDEX_ITEMS + ai_iItemPosition * M4CL_MCR_INDEX_ITEM_SIZE ;

    return( M4ClUnPackInt8( m_pcBase, iIndexItemHandle + M4CL_MCR_INDEX_ITEM_WAY ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetFilterId( m4uint32_t ai_iFilterHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iFilterHandle ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetFilterT3Id( m4uint32_t ai_iFilterHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iFilterHandle + M4CL_MCR_FILTER_T3_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetFilterNodeId( m4uint32_t ai_iFilterHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iFilterHandle + M4CL_MCR_FILTER_NODE_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetFilterSysSentenceItem( m4uint32_t ai_iFilterHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iFilterHandle + M4CL_MCR_FILTER_SYS_SENTENCE_ITEM ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetFilterSysParamItem( m4uint32_t ai_iFilterHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iFilterHandle + M4CL_MCR_FILTER_SYS_PARAM_ITEM ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint8_t   ClCompiledMCR :: GetFilterBehaviourType( m4uint32_t ai_iFilterHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iFilterHandle + M4CL_MCR_FILTER_BEHAVIOUR_TYPE ) ) ;
}

m4uint8_t   ClCompiledMCR :: GetFilterIsOwnNode( m4uint32_t ai_iFilterHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iFilterHandle + M4CL_MCR_FILTER_IS_OWN_NODE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetSentenceId( m4uint32_t ai_iSentenceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iSentenceHandle ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetSentenceGroupObjects( m4uint32_t ai_iSentenceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iSentenceHandle + M4CL_MCR_SENTENCE_GROUP_OBJECTS ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetSentenceApiSql( m4uint32_t ai_iSentenceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iSentenceHandle + M4CL_MCR_SENTENCE_APISQL_1 ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetSentenceApiSql2( m4uint32_t ai_iSentenceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iSentenceHandle + M4CL_MCR_SENTENCE_APISQL_2 ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetSentenceApiSql3( m4uint32_t ai_iSentenceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iSentenceHandle + M4CL_MCR_SENTENCE_APISQL_3 ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetSentenceApiSql4( m4uint32_t ai_iSentenceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iSentenceHandle + M4CL_MCR_SENTENCE_APISQL_4 ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetSentenceMaxNumCol( m4uint32_t ai_iSentenceHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iSentenceHandle + M4CL_MCR_SENTENCE_MAX_NUM_COL ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetInstanceId( m4uint32_t ai_iInstanceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iInstanceHandle ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetInstanceT3Id( m4uint32_t ai_iInstanceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iInstanceHandle + M4CL_MCR_INSTANCE_T3_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetInstanceRoleId( m4uint32_t ai_iInstanceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iInstanceHandle + M4CL_MCR_INSTANCE_ROLE_ID ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetInstanceOrganization( m4uint32_t ai_iInstanceHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iInstanceHandle + M4CL_MCR_INSTANCE_ORGANIZATION ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint8_t	ClCompiledMCR :: GetInstanceScope( m4uint32_t ai_iInstanceHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iInstanceHandle + M4CL_MCR_INSTANCE_SCOPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetInstanceOpenMode( m4uint32_t ai_iInstanceHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iInstanceHandle + M4CL_MCR_INSTANCE_OPEN_MODE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetInstanceCsAccess( m4uint32_t ai_iInstanceHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iInstanceHandle + M4CL_MCR_INSTANCE_CS_ACCESS ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetInstanceUseRole( m4uint32_t ai_iInstanceHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iInstanceHandle + M4CL_MCR_INSTANCE_USE_ROLE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetInstanceOrganizationType( m4uint32_t ai_iInstanceHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iInstanceHandle + M4CL_MCR_INSTANCE_ORGANIZATION_TYPE ) ) ;
}

m4pcchar_t	ClCompiledMCR :: GetBusinessMethodId( m4uint32_t ai_iBusinessMethodHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iBusinessMethodHandle ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetBusinessMethodName( m4uint32_t ai_iBusinessMethodHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iBusinessMethodHandle + M4CL_MCR_BUSINESS_METHOD_NAME ) ;

    return( m_pcBase + iHandle ) ;
}

m4pcchar_t	ClCompiledMCR :: GetBusinessMethodDescription( m4uint32_t ai_iBusinessMethodHandle ) const
{

    m4uint32_t	iHandle ;


    iHandle = M4ClUnPackInt32( m_pcBase, ai_iBusinessMethodHandle + M4CL_MCR_BUSINESS_METHOD_DESCRIPTION ) ;

    return( m_pcBase + iHandle ) ;
}

m4uint32_t	ClCompiledMCR :: GetBusinessMethodItemHandle( m4uint32_t ai_iBusinessMethodHandle ) const
{
    return( M4ClUnPackInt32( m_pcBase, ai_iBusinessMethodHandle + M4CL_MCR_BUSINESS_METHOD_ITEM_HANDLE ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetBusinessMethodNumberOfArguments( m4uint32_t ai_iBusinessMethodHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iBusinessMethodHandle + M4CL_MCR_BUSINESS_METHOD_ARGUMENT_NUMBER ) ) ;
}

m4uint32_t	ClCompiledMCR :: GetBusinessMethodArgumentNodeHandleByPosition( m4uint32_t ai_iBusinessMethodHandle, m4uint16_t ai_iArgumentPosition ) const
{

    m4uint32_t	iHandle ;


    iHandle = ai_iBusinessMethodHandle + M4CL_MCR_BUSINESS_METHOD_ARGUMENTS + ai_iArgumentPosition * M4CL_MCR_BUSINESS_ARG_SIZE ;

	return( M4ClUnPackInt32( m_pcBase, iHandle + M4CL_MCR_BUSINESS_ARG_NODE_ID ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetBusinessMethodArgumentTypeByPosition( m4uint32_t ai_iBusinessMethodHandle, m4uint16_t ai_iArgumentPosition ) const
{

    m4uint32_t	iHandle ;


    iHandle = ai_iBusinessMethodHandle + M4CL_MCR_BUSINESS_METHOD_ARGUMENTS + ai_iArgumentPosition * M4CL_MCR_BUSINESS_ARG_SIZE ;

	return( M4ClUnPackInt8( m_pcBase, iHandle + M4CL_MCR_BUSINESS_ARG_TYPE ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetChannelVMCapacityReq( void ) const
{
    return( M4ClUnPackInt16( m_pcBase, M4CL_MCR_CHANNEL_VM_CAPACITY_REQ ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelIsSeparable( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_IS_SEPARABLE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetChannelCsExeType( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_CS_EXE_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeCsType( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_CS_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetNodeSyncType( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SYNC_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemCsType( m4uint32_t ai_iItemHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_CS_TYPE ) ) ;
}

m4uint8_t	ClCompiledMCR :: GetItemSyncType( m4uint32_t ai_iItemHandle ) const
{
    return( M4ClUnPackInt8( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_SYNC_TYPE ) ) ;
}


// seguridad ==================================================================


m4uint16_t	ClCompiledMCR :: GetChannelSecurity( void ) const
{
    return( M4ClUnPackInt16( m_pcBase, M4CL_MCR_CHANNEL_SECURITY ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetNodeSCRStart( m4uint32_t ai_iNodeHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SCR_START ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetItemSCRStart( m4uint32_t ai_iItemHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_SCR_START ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetFilterSCRStart( m4uint32_t ai_iFilterHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iFilterHandle + M4CL_MCR_FILTER_SCR_START ) ) ;
}

m4uint16_t	ClCompiledMCR :: GetPlugSCRStart( m4uint32_t ai_iPlugHandle ) const
{
    return( M4ClUnPackInt16( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_SCR_START ) ) ;
}






//=================================================================================
//
// ClCMCRWrapper
//
// Clase para el wrapper de metadatos
//
//=================================================================================

// Funciones inline ===========================================================

m4date_t	ClCMCRWrapper :: GetStartDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_START_DATE ) ) ;
}

m4date_t	ClCMCRWrapper :: GetEndDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_END_DATE ) ) ;
}

m4date_t	ClCMCRWrapper :: GetCorStartDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_COR_START_DATE ) ) ;
}

m4date_t	ClCMCRWrapper :: GetCorEndDate( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_COR_END_DATE ) ) ;
}

m4date_t	ClCMCRWrapper :: GetVersion( void ) const
{
    return( M4ClUnPackDate( m_pcBase, M4CL_MCR_CHANNEL_VERSION ) ) ;
}

m4uint8_t	ClCMCRWrapper :: GetMetaLanguage( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_META_LANGUAGE ) ) ;
}

m4uint8_t	ClCMCRWrapper :: GetIsMDCacheable( void ) const
{
    return( M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_IS_MD_CACHEABLE ) ) ;
}




//=================================================================================
//
// ClDecodeInstruction
//
// Clase decodificar instrucciones
//
//=================================================================================

// Funciones inline ===========================================================

// ############ CAMBIO DE ESTADO: Avance o Retroceso por la Formula #############

//Cambia el estado del objeto, moviendose al no. de Instruccion indicado
void	ClDecodeInstruction :: MoveTo (m4int32_t ai_iNumIns){
	
	m_pcOffsetInstruction = m_pcFirstOffsetIns + ai_iNumIns*M4CL_MCR_CODE_INSTRUCTION_OFFSET_SIZE;
	m4int32_t iTempOffset = M4ClUnPackInt32 (m_pcOffsetInstruction, 0) ;
	
	//Calculamos el Offset de la instruccion
	if (M4CL_MCR_INS_EMB_IS_EMBEBBED (iTempOffset) ){
		m_pcInstruction	  = m_pcOffsetInstruction ; //si es embebida, la instrucción está en el propio offset
	}
	else{
		m_pcInstruction	  = m_pcOffsetInstruction + M4CL_MCR_INS_EMB_OFFSET(iTempOffset) ;
	}
}


// ############ OBTENCION DE DATOS DE LA INSTRUCCION #############


m4uint16_t	ClDecodeInstruction :: GetExeFunc (void)
{
	// Valido para todos los Tipos de Instruccion
	m4uint32_t iVal=M4ClUnPackInt32(m_pcOffsetInstruction, 0);
	
	if ( M4CL_MCR_INS_EMB_IS_EMBEBBED (iVal) )
	{
		//control	
		if (M4CL_MCR_INS_EMB_IS_CONTROL(iVal) )
		{
			return g_aiFlagToFunc[ M4CL_MCR_INS_EMB_GET_CONTROL_FUNC(iVal) ];
		}
		if (M4CL_MCR_INS_EMB_IS_SIMPLE(iVal) )
		{
			return m4uint16_t(M4CL_MCR_INS_EMB_GET_SIMPLE_FUNC(iVal));
		}
	}
	
	//resto de instrucciones
	return ( M4ClUnPackInt16 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_EXEFUNC) );
}


m4int16_t	ClDecodeInstruction :: GetFlags(void)
{
	// Valido para todos los Tipos de Instruccion
	if (M4CL_MCR_INS_EMB_IS_EMBEBBED (M4ClUnPackInt32(m_pcOffsetInstruction, 0) ) )
	{
		m4int16_t iFlag=0;
		M4CL_SET_LEVEL_FLAG(iFlag, M4CL_MCR_INS_EMB_GET_LEVEL(M4ClUnPackInt32(m_pcInstruction, 0)));
		return iFlag;
	}
	
	return ( M4ClUnPackInt16 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_FLAGS) );
}

m4uint32_t	ClDecodeInstruction :: GetNumNodo(void)
{
	return ( M4ClUnPackInt32 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_NODE) );
}

m4uint32_t	ClDecodeInstruction :: GetNumItem(void)
{
	if (M4CL_MCR_INS_EMB_IS_EMBEBBED (M4ClUnPackInt32(m_pcOffsetInstruction, 0) ) )
	{
		//como lo que retorna es unsigned, numca vendrá con signo
		//así que ni siquiera lo tenemos en cuenta
		return M4CL_MCR_INS_EMB_GET_CONTROL_ARG (M4ClUnPackInt32(m_pcOffsetInstruction, 0)) ;
	}
	
	return ( M4ClUnPackInt32 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_ITEMVAR) );
}

m4int32_t	ClDecodeInstruction :: GetNumVar(void)
{
	if (M4CL_MCR_INS_EMB_IS_EMBEBBED (M4ClUnPackInt32(m_pcOffsetInstruction, 0) ) )
	{
		m4uint32_t iVal=M4ClUnPackInt32(m_pcOffsetInstruction, 0);

		if (M4CL_MCR_INS_EMB_HAS_CONTROL_SIGN(iVal))
		{
			return - ( (m4int32_t)(M4CL_MCR_INS_EMB_GET_CONTROL_ARG (iVal)) );
		}
		return M4CL_MCR_INS_EMB_GET_CONTROL_ARG (iVal) ;
	}
	
	return ( M4ClUnPackInt32 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_ITEMVAR) );
}

m4uint16_t	ClDecodeInstruction :: GetNumRegla(void)
{
	return (m4uint16_t) ( M4ClUnPackInt32 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_RULE) );
}

m4uint8_t	ClDecodeInstruction :: GetNumAtrib(void)
{
	return ( M4ClUnPackInt8 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_ATTR) );
}

m4uint32_t	ClDecodeInstruction :: GetNameChannel(void)
{
	return ( M4ClUnPackInt32 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_CHANNEL) );
}

m4int32_t	ClDecodeInstruction :: GetVariantInt(void)
{
	return ( M4ClUnPackInt32 (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_VARIANT_AS_INT) );
}

m4double_t	ClDecodeInstruction :: GetVariantDouble(void)
{
	if (M4CL_MCR_INS_EMB_HAS_VARIANT_INT(M4ClUnPackInt32(m_pcOffsetInstruction, 0) ) )
	{
		return ( m4int32_t( M4ClUnPackInt32 (m_pcInstruction, M4CL_MCR_CODE_INSTRUCTION_VARIANT_AS_INT) ) ) ;
	}
	return ( M4ClUnPackDouble (m_pcInstruction , M4CL_MCR_CODE_INSTRUCTION_VARIANT) );
}

// ######## OPERADORES SOBRECARGADOS: Cambian el estado de la Instruccion #########

void	ClDecodeInstruction :: operator ++ (void)
{
	m_pcOffsetInstruction += M4CL_MCR_CODE_INSTRUCTION_OFFSET_SIZE;
	m4int32_t iTempOffset = M4ClUnPackInt32 (m_pcOffsetInstruction, 0) ;
	
	//Calculamos el Offset de la instruccion
	if (M4CL_MCR_INS_EMB_IS_EMBEBBED (iTempOffset) )
	{
		m_pcInstruction	  = m_pcOffsetInstruction ; //si es embebida, la instrucción está en el propio offset
	}
	else
	{
		m_pcInstruction	  = m_pcOffsetInstruction + M4CL_MCR_INS_EMB_OFFSET(iTempOffset) ;
	}
}

void	ClDecodeInstruction :: operator -- (void)
{
	m_pcOffsetInstruction -= M4CL_MCR_CODE_INSTRUCTION_OFFSET_SIZE;
	m4int32_t iTempOffset = M4ClUnPackInt32 (m_pcOffsetInstruction, 0) ;
	
	//Calculamos el Offset de la instruccion
	if (M4CL_MCR_INS_EMB_IS_EMBEBBED (iTempOffset) )
	{
		m_pcInstruction	  = m_pcOffsetInstruction ; //si es embebida, la instrucción está en el propio offset
	}
	else
	{
		m_pcInstruction	  = m_pcOffsetInstruction + M4CL_MCR_INS_EMB_OFFSET(iTempOffset) ;
	}
}


// ############ Serializacion/Deserializacion usando IODriver ############

m4return_t ClDecodeInstruction :: Serialize(ClGenericIODriver& IOD)
{
	m4uint32_t iNumIns;
	iNumIns = (m_pcOffsetInstruction - m_pcFirstOffsetIns) / M4CL_MCR_CODE_INSTRUCTION_OFFSET_SIZE;
	m4return_t RetVal = IOD.Write(iNumIns);
	return RetVal;
}



//=================================================================================
//
// ClDecodeFormula
//
// Clase decodificar fórmulas
//
//=================================================================================

// Funciones inline ===========================================================

//inicialización
void ClDecodeFormula :: Init (const m4char_t * ai_pcIniFormula)
{
	m_pcIniFormula	= ai_pcIniFormula ;
	if (ai_pcIniFormula)
	{ //iniciamos la instrucción si la formula no es nula
		m_oInstruction.Init (ai_pcIniFormula + M4CL_MCR_CODE_INSTRUCTION_START);
	}
}

m4uint16_t	ClDecodeFormula :: GetNumVars	(void) const
{
	return (M4ClUnPackInt16 (m_pcIniFormula , M4CL_MCR_CODE_VARIABLE_NUMBER) );
}

m4bool_t	ClDecodeFormula :: GetMustAssign (void) const
{
	m4int8_t iVal=M4ClUnPackInt8 (m_pcIniFormula , M4CL_MCR_CODE_PRIOR_ASSIGN) ;
	return M4_BOOL(M4CL_HAS_FLAG(iVal,M4CL_MCR_CODE_ASSIGN_FLAG));
}

m4bool_t	ClDecodeFormula :: GetIsAutogenerated (void) const
{
	m4int8_t iVal=M4ClUnPackInt8 (m_pcIniFormula , M4CL_MCR_CODE_PRIOR_ASSIGN) ;
	return M4_BOOL(M4CL_HAS_FLAG(iVal,M4CL_MCR_CODE_AUTOGENERATED_FLAG));
}

m4int8_t	ClDecodeFormula :: GetPriority (void) const
{
	m4int8_t iVal=M4ClUnPackInt8 (m_pcIniFormula , M4CL_MCR_CODE_PRIOR_ASSIGN) ;
	return M4CL_APPLY_MASK(iVal,M4CL_MCR_CODE_PRIOR_MASK);
}

m4int8_t	ClDecodeFormula :: GetType	(void) const
{
	return (M4ClUnPackInt8 (m_pcIniFormula , M4CL_MCR_CODE_TYPE) );
}

m4uint8_t	ClDecodeFormula :: GetLevel	(void) const
{
	m4uint32_t	iLevelAndInstructions = M4ClUnPackInt32( m_pcIniFormula , M4CL_MCR_CODE_NUMBER_INSTRUCTIONS ) ;
	iLevelAndInstructions =  M4CL_APPLY_MASK(iLevelAndInstructions, M4CL_MCR_CODE_INHERIT_LEVEL_MASK ) ;
	return( (m4uint8_t) ( iLevelAndInstructions >> 24 ) ) ;
}

m4int32_t	ClDecodeFormula :: GetNumberOfInstructions (void) const
{
	m4uint32_t	iLevelAndInstructions = M4ClUnPackInt32 (m_pcIniFormula , M4CL_MCR_CODE_NUMBER_INSTRUCTIONS) ;
	return M4CL_APPLY_MASK(iLevelAndInstructions,M4CL_MCR_CODE_INSTRUCTION_NUMBER_MASK);
}

m4int32_t	ClDecodeFormula :: GetFormulaSize (void) const
{
	return (M4ClUnPackInt32 (m_pcIniFormula , M4CL_MCR_CODE_FORMULA_SIZE) );
}

m4int32_t	ClDecodeFormula :: GetRuleTI (void) const
{
	return (M4ClUnPackInt32 (m_pcIniFormula , M4CL_MCR_CODE_RULE_TI) );
}

m4date_t	ClDecodeFormula :: GetStartDate(void) const
{
	m4uint32_t iSize=M4ClUnPackInt32 (m_pcIniFormula , M4CL_MCR_CODE_FORMULA_SIZE);
	m4int8_t iVal=M4ClUnPackInt8 (m_pcIniFormula , iSize-sizeof (m4uint8_t)) ;

	if (M4CL_MCR_INS_IS_START_PLUS_INF(iVal))
	{
		return M4CL_PLUS_INFINITE_IN_JULIAN;
	}
	else if(M4CL_MCR_INS_IS_START_MINUS_INF(iVal))
	{
		return M4CL_MINUS_INFINITE_IN_JULIAN;
	}
	else
	{
		m4uint32_t iOffset=1; //como contamos para atras emprezamos quitando el byte de fechas
		m4double_t dSeconds=0;
		m4uint32_t iDays=0;

		if (M4CL_MCR_INS_HAS_END_DAYS_AND_SECONDS(iVal))
		{
			iOffset+= 2 * sizeof (m4uint32_t);
		}
		else if(M4CL_MCR_INS_HAS_END_DAYS(iVal))
		{
			iOffset+= sizeof (m4uint32_t);
		}

		iOffset+= sizeof (m4uint32_t);
		
		//si viene con segundos
		if (M4CL_MCR_INS_HAS_START_DAYS_AND_SECONDS(iVal))
		{
			dSeconds=(m4double_t)M4ClUnPackInt32 (m_pcIniFormula , iSize - iOffset);
			iOffset+= sizeof (m4uint32_t);
		}
		if (M4CL_MCR_INS_HAS_START_DAYS(iVal))
		{
			iDays=M4ClUnPackInt32 (m_pcIniFormula , iSize - iOffset);
		}

		return (m4double_t)iDays + (dSeconds/M4_SECONDS_DAY);
	}
}

m4date_t	ClDecodeFormula :: GetEndDate(void) const
{
	
	m4uint32_t iSize=M4ClUnPackInt32 (m_pcIniFormula , M4CL_MCR_CODE_FORMULA_SIZE);
	m4int8_t iVal=M4ClUnPackInt8 (m_pcIniFormula , iSize-sizeof (m4uint8_t)) ;

	if (M4CL_MCR_INS_IS_END_PLUS_INF(iVal))
	{
		return M4CL_PLUS_INFINITE_IN_JULIAN;
	}
	else if(M4CL_MCR_INS_IS_END_MINUS_INF(iVal))
	{
		return M4CL_MINUS_INFINITE_IN_JULIAN;
	}
	else
	{
		m4uint32_t iOffset=1; //como contamos para atras emprezamos quitando el byte de fechas
		m4double_t dSeconds=0;
		m4uint32_t iDays=0;

		iOffset+= sizeof (m4uint32_t);
		
		//si viene con segundos
		if (M4CL_MCR_INS_HAS_END_DAYS_AND_SECONDS(iVal))
		{
			dSeconds=(m4double_t)M4ClUnPackInt32 (m_pcIniFormula , iSize - iOffset);
			iOffset+= sizeof (m4uint32_t);
		}
		if (M4CL_MCR_INS_HAS_END_DAYS(iVal))
		{
			iDays=M4ClUnPackInt32 (m_pcIniFormula , iSize - iOffset);
		}

		return (m4double_t)iDays + (dSeconds/M4_SECONDS_DAY);
	}
}



