
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clmdfac.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
// 
//    This module defines thefunctions of the class of the factory of
//    compiled metachannels
//
//
//==============================================================================


#include "m4mdfaci.hpp"
#include "m4facres.hpp"
#include "m4facdef.hpp"

#include "m4mdrt.hpp"
#include "clmapcac.hpp"
#include "cltti.hpp"
#include "cltchann.hpp"
#include "cltnode.hpp"
#include "cltitem.hpp"
#include "m4buildversion.hpp"
#include "m4objglb.hpp"
#include "chlog.hpp"
#include "m4mdadaptor.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "m4objreg.hpp"
#include "sttwrap.hpp"
#include "sttlib.hpp"
#include "cachedir.hpp"
#include "cldmcontext.hpp"
#include "cllgadap.hpp"
#include "m4cstrace.hpp"
#include "m4ldb.hpp"
#include "threadobjs.hpp"
#include "rowsquota.hpp"



//=================================================================================
//
// ClFactoryImp
//
// Clase que implementa la factoría base de metadatos y seguridad
//
//=================================================================================

ClFactoryImp :: ClFactoryImp( void )

:
	m_oAccessUpperPool	( M4CL_MCR_ACCESS_UPPER_POOL_SIZE, M4CL_UPPERCASE ),
	m_oAccessNoUpperPool( M4CL_MCR_ACCESS_NO_UPPER_POOL_SIZE ),
	m_oSystemUpperPool	( M4CL_MCR_SYSTEM_UPPER_POOL_SIZE, M4CL_UPPERCASE ),
	m_oSystemNoUpperPool( M4CL_MCR_SYSTEM_NO_UPPER_POOL_SIZE )

#ifdef	_DEBUG
	,m_oTempPool( M4CL_MCR_TEMP_POOL_SIZE ),
	m_oFilePool( M4CL_MCR_FILE_POOL_SIZE )
#endif

{
	m_bIsInit = M4_FALSE ;

	m_poChannelManager = NULL ;
	m_poEnvironment = NULL ;
	m_poLogonAdaptor = NULL ;
	m_poCacheDirectory = NULL ;
	m_poStatisticsWrap = NULL ;
	m_poFunctionTable = NULL ;
	m_poAtributeTable = NULL ;
	m_poCsTrace = NULL ;
	m_poLdb = NULL ;

	m_poAccess = NULL ;
	m_poCMCR = NULL ;

	m_iIsSystemBuilt = 0 ;
	m_poSystemTempM4Obj = NULL ;
	m_poSystemM4Obj = NULL ;
	m_poSystemAccess = NULL ;

	m_iIsMetaBuilt = 0 ;
    m_poMetaM4Obj = NULL ;
	m_poMetaAccess = NULL ;

    m_iIsCheckMetaBuilt = 0 ;
    m_poCheckMetaM4Obj = NULL ;
    m_poCheckMetaAccess = NULL ;
	m_poCheckMetaNode = NULL ;

	m_iIsSecurityBuilt = 0 ;
	m_poSecurityM4Obj = NULL ;
	m_poSecurityAccess = NULL ;

	m_iIsDictBuilt = 0 ;
	m_poDictM4Obj = NULL ;

	m_iIsTransBuilt = 0 ;
	m_poTransM4Obj = NULL ;

	m_poMDCache = NULL ;
	m_poSecCache = NULL ;
	m_poMapCache = NULL ;

	m_poStatistics = NULL ;

#ifdef	_DEBUG
    m_dBuffer = 0 ;
    m_pcBuffer = NULL ;
    m_pfFile = NULL ;
#endif

}


ClFactoryImp :: ~ClFactoryImp( void )
{

	m4return_t	        iResult ;
	m4uint16_t	        i ;
	m4uint16_t	        iLength ;
	m4pvoid_t           pvPointer ;
	ClAccess            *poAccess ;
	ClCompiledMCR		*poCMCR ;

    
    if( m_poSystemTempM4Obj != NULL )
    {
        delete( m_poSystemTempM4Obj );
        m_poSystemTempM4Obj = NULL ;
    }

// primero nos cargamos el access y luego el channel:

    if( m_iIsSystemBuilt == 1 )
    {
        if( m_poSystemAccess != NULL )
        {
            m_poSystemAccess->Destroy() ;
            m_poSystemAccess = NULL ;
        }

        if( m_poSystemM4Obj != NULL )
        {
			poCMCR = m_poSystemM4Obj->GetpBackMCR() ;

			if( poCMCR != NULL )
			{
				poCMCR->Destroy();
				poCMCR = NULL ;
			}

			poCMCR = m_poSystemM4Obj->GetpCMCR() ;
			m_poSystemM4Obj->Destroy() ;
            m_poSystemM4Obj = NULL ;

			if( poCMCR != NULL )
			{
				poCMCR->Destroy();
				poCMCR = NULL ;
			}
        }
    }

    m_iIsSystemBuilt = 0 ;


    if( m_iIsMetaBuilt == 1 )
    {
        if( m_poMetaAccess != NULL )
        {
            m_poMetaAccess->Destroy() ;
            m_poMetaAccess = NULL ;
        }

        if( m_poMetaM4Obj != NULL )
        {
			poCMCR = m_poMetaM4Obj->GetpBackMCR() ;

			if( poCMCR != NULL )
			{
				poCMCR->Destroy();
				poCMCR = NULL ;
			}

			poCMCR = m_poMetaM4Obj->GetpCMCR() ;

			m_poMetaM4Obj->Destroy() ;
            m_poMetaM4Obj = NULL ;

			if( poCMCR != NULL )
			{
				poCMCR->Destroy();
				poCMCR = NULL ;
			}
        }
    }

    m_iIsMetaBuilt = 0 ;


    if( m_iIsCheckMetaBuilt == 1 )
    {
        if( m_poCheckMetaAccess != NULL )
        {
            m_poCheckMetaAccess->Destroy() ;
            m_poCheckMetaAccess = NULL ;
        }

        if( m_poCheckMetaM4Obj != NULL )
        {
			m_poCheckMetaM4Obj->Destroy() ;
            m_poCheckMetaM4Obj = NULL ;
        }
    }

	m_iIsCheckMetaBuilt = 0 ;


    if( m_iIsSecurityBuilt == 1 )
    {
        if( m_poSecurityAccess != NULL )
        {
            m_poSecurityAccess->Destroy() ;
            m_poSecurityAccess = NULL ;
        }

        if( m_poSecurityM4Obj != NULL )
        {
			m_poSecurityM4Obj->Destroy() ;
            m_poSecurityM4Obj = NULL ;
        }
    }

	m_iIsSecurityBuilt = 0 ;


    if( m_iIsDictBuilt == 1 )
    {
        iLength = m_oDictAccesses.GetLength() ;

	    for( i = 0 ; i < iLength ; i++ )
	    {
		    iResult = m_oDictAccesses.GetElement( i, pvPointer ) ;

		    if( iResult != M4_SUCCESS )
		    {
				DUMP_CHLOG_DEBUGF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		    }

		    if( pvPointer != NULL )
		    {
			    poAccess = ( ClAccess* ) pvPointer ;
				poAccess->Destroy() ;
		    }
	    }

        if( m_poDictM4Obj != NULL )
        {
			poCMCR = m_poDictM4Obj->GetpBackMCR() ;

			if( poCMCR != NULL )
			{
				poCMCR->Destroy();
				poCMCR = NULL ;
			}

			poCMCR = m_poDictM4Obj->GetpCMCR() ;

			m_poDictM4Obj->Destroy() ;
            m_poDictM4Obj = NULL ;

			if( poCMCR != NULL )
			{
				poCMCR->Destroy();
				poCMCR = NULL ;
			}
        }
	}

    m_iIsDictBuilt = 0 ;

		
    if( m_iIsTransBuilt == 1 )
    {
        iLength = m_oTransAccesses.GetLength() ;

	    for( i = 0 ; i < iLength ; i++ )
	    {
		    iResult = m_oTransAccesses.GetElement( i, pvPointer ) ;

		    if( iResult != M4_SUCCESS )
		    {
				DUMP_CHLOG_DEBUGF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		    }

		    if( pvPointer != NULL )
		    {
			    poAccess = ( ClAccess* ) pvPointer ;
				poAccess->Destroy() ;
		    }
	    }

        if( m_poTransM4Obj != NULL )
        {
			poCMCR = m_poTransM4Obj->GetpBackMCR() ;

			if( poCMCR != NULL )
			{
				poCMCR->Destroy();
				poCMCR = NULL ;
			}

			poCMCR = m_poTransM4Obj->GetpCMCR() ;

			m_poTransM4Obj->Destroy() ;
            m_poTransM4Obj = NULL ;

			if( poCMCR != NULL )
			{
				poCMCR->Destroy();
				poCMCR = NULL ;
			}
        }

    }

	m_iIsTransBuilt = 0 ;


	if( m_poMDCache != NULL )
	{
		if( m_poCacheDirectory != NULL )
		{
			m_poCacheDirectory->Detach( m_poMDCache ) ;
		}
		delete( m_poMDCache ) ;
		m_poMDCache = NULL ;
	}

	if( m_poSecCache != NULL )
	{
		if( m_poCacheDirectory != NULL )
		{
			m_poCacheDirectory->Detach( m_poSecCache ) ;
		}
		delete( m_poSecCache ) ;
		m_poSecCache = NULL ;
	}

	if( m_poMapCache != NULL )
	{
		if( m_poCacheDirectory != NULL )
		{
			m_poCacheDirectory->Detach( m_poMapCache ) ;
		}
		delete( m_poMapCache ) ;
		m_poMapCache = NULL ;
	}


	if( m_poStatistics != NULL )
	{
		delete( m_poStatistics ) ;
		m_poStatistics = NULL ;
	}


#ifdef	_DEBUG
    m_dBuffer = 0 ;

    if( m_pcBuffer != NULL )
    {
        delete [] m_pcBuffer ;
        m_pcBuffer = NULL ;
    }

    if( m_pfFile != NULL )
    {
        fclose( m_pfFile ) ;
        m_pfFile = NULL ;
    }
#endif

}




m4return_t	ClFactoryImp :: Init(
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
								)
{

	m4uint8_t	iDebug ;
	m4uint8_t	iMode ;
	m4uint32_t	iMaxMemory ;
	m4uint32_t	iMaxObjects ;
	m4uint32_t	iRatio ;
	m4pcchar_t	pccDirectory ;


	if( m_bIsInit == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}

	if	(
			ai_poChannelManager == NULL ||
			ai_poEnvironment == NULL ||
			ai_poLogonAdaptor == NULL ||
			ai_poStatisticsWrap == NULL ||
			ai_poFunctionTable == NULL ||
			ai_poAtributeTable == NULL ||
			ai_poCsTrace == NULL
		)
	{
		return( M4_ERROR ) ;
	}

	m_poChannelManager = ai_poChannelManager ;
	m_poEnvironment = ai_poEnvironment ;
	m_poLogonAdaptor = ai_poLogonAdaptor ;
	m_poStatisticsWrap = ai_poStatisticsWrap ;
	m_poFunctionTable = ai_poFunctionTable ;
	m_poAtributeTable = ai_poAtributeTable ;
	m_poCsTrace = ai_poCsTrace ;


	iMode = m_poEnvironment->GetMDDiskCache() ;

	if( iMode != M4CL_CACHE_NONE )
	{
		m_poCacheDirectory = ai_poCacheDirectory ;

		iMaxMemory = m_poEnvironment->GetMDCacheMaxMemory();
		iMaxObjects = m_poEnvironment->GetMDCacheMaxNumObjects() ;
		iRatio = m_poEnvironment->GetMDCacheRefreshRatio() ;
		pccDirectory = m_poEnvironment->GetCacheDirectory() ;

		if( ai_poMDCache != NULL )
		{
			m_poMDCache = new ClCMCRCache( ai_poMDCache, pccDirectory, iMode, iMaxMemory, iMaxObjects, iRatio, 0, 0 ) ;
			CHECK_CHLOG_ERRORF( m_poMDCache == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCMCRCache ) ) << __LINE__ << __FILE__ ) ;

			if( m_poCacheDirectory != NULL )
			{
				m_poCacheDirectory->Attach( m_poMDCache ) ;
			}
		}

		if( ai_poSecCache != NULL )
		{
			m_poSecCache = new ClCSCRCache( ai_poSecCache, pccDirectory, iMode, iMaxMemory, iMaxObjects, iRatio, 0, 0 ) ;
			CHECK_CHLOG_ERRORF( m_poSecCache == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCSCRCache ) ) << __LINE__ << __FILE__ ) ;

			if( m_poCacheDirectory != NULL )
			{
				m_poCacheDirectory->Attach( m_poSecCache ) ;
			}
		}

		if( ai_poMapCache != NULL )
		{
			m_poMapCache = new ClMapCache( ai_poMapCache, pccDirectory, iMode, iMaxMemory, iMaxObjects, iRatio, 0, 0 ) ;
			CHECK_CHLOG_ERRORF( m_poMapCache == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClMapCache ) ) << __LINE__ << __FILE__ ) ;

			if( m_poCacheDirectory != NULL )
			{
				m_poCacheDirectory->Attach( m_poMapCache ) ;
			}
		}
	}


	iDebug = ClVMDebugEnv::getInstance()->getDebugLevel() ;

	if( iDebug > 1 )
	{
		m_poStatistics = new ClMdStatistics ;
		CHECK_CHLOG_ERRORF( m_poStatistics == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClMdStatistics ) ) << __LINE__ << __FILE__ ) ;
	}

	m_bIsInit = M4_TRUE ;


	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: End( void )
{
	if( m_bIsInit == M4_FALSE )
	{
		return( M4_SUCCESS ) ;
	}

	if( m_poMDCache != NULL )
	{
		if( m_poCacheDirectory != NULL )
		{
			m_poCacheDirectory->Detach( m_poMDCache ) ;
		}
		delete( m_poMDCache ) ;
		m_poMDCache = NULL ;
	}

	if( m_poSecCache != NULL )
	{
		if( m_poCacheDirectory != NULL )
		{
			m_poCacheDirectory->Detach( m_poSecCache ) ;
		}
		delete( m_poSecCache ) ;
		m_poSecCache = NULL ;
	}

	if( m_poMapCache != NULL )
	{
		if( m_poCacheDirectory != NULL )
		{
			m_poCacheDirectory->Detach( m_poMapCache ) ;
		}
		delete( m_poMapCache ) ;
		m_poMapCache = NULL ;
	}

	m_poCacheDirectory = NULL ;

	m_poLdb = NULL ;
	m_poCsTrace = NULL ;
	m_poAtributeTable = NULL ;
	m_poFunctionTable = NULL ;
	m_poStatisticsWrap = NULL ;
	m_poLogonAdaptor = NULL ;
	m_poEnvironment = NULL ;
	m_poChannelManager = NULL ;

	m_bIsInit = M4_FALSE ;

	return( M4_SUCCESS ) ;
}



m4pchar_t	ClFactoryImp ::_ComposeMappingId( m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccOrganization, m4bool_t ai_bConvert, m4pchar_t ai_pcBuffer, m4uint8_t ai_iSize ) const
{

	m4uint32_t	iLength1 = 0 ;
	m4uint32_t	iLength2 = 0 ;


	if( ai_pccMapId == NULL )
	{
		ai_pccMapId = "" ;
	}

	if( ai_pccOrganization == NULL )
	{
		ai_pccOrganization = "" ;
	}

	iLength1 = strlen( ai_pccMapId ) ;
	iLength2 = strlen( ai_pccOrganization ) ;

	CHECK_CHLOG_ERRORF( iLength1 + 1 + iLength2 > ai_iSize, NULL, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * ( iLength1 + 1 + iLength2 + 1 ) ) << __LINE__ << __FILE__ ) ;

	memcpy( ai_pcBuffer, ai_pccMapId, iLength1 ) ;
	memcpy( ai_pcBuffer + iLength1, "@", 1 ) ;
	memcpy( ai_pcBuffer + iLength1 + 1, ai_pccOrganization, iLength2 + 1 ) ;

	if( ai_bConvert == M4_TRUE )
	{
		strupr( ai_pcBuffer ) ;
	}

	return( ai_pcBuffer ) ;
}



m4return_t  ClFactoryImp :: _BuildSystemChannel( m4pcchar_t ai_pccFile, ClChannel* &ao_rpoChannel)
{

    m4return_t      iResult ;
	m4uint8_t		iCsType ;
    m4VariantType   vValue ;
    ClCompiledMCR   *poCMCR ;


    m_poChannelManager->CreateChannel( ao_rpoChannel ) ;
	CHECK_CHLOG_DEBUGF( ao_rpoChannel == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClChannel ) ) << __LINE__ << __FILE__ ) ;

// lo cogemos el del canal a ver que tal

	ao_rpoChannel->CsType.Get( vValue ) ;
	iCsType = m4uint8_t( vValue.Data.DoubleData ) ;
    
	iResult = MDFac_BuildFromFile( ai_pccFile, iCsType, 1, NULL, poCMCR ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    iResult = ao_rpoChannel->BuildFromMCR( poCMCR ) ;

    if( iResult != M4_SUCCESS )
    {
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
    }

	// EFV: corrige bug de canales de sistema en el channel manager
	// jcr Tiene que ir después de la build
	m_poChannelManager->Channel.Detach( ao_rpoChannel ) ;


// Si el tipo era front, siempre construyo el back en cualquier caso

	if( iCsType == M4CL_CSTYPE_FRONT )
	{
		ClCompiledMCR   *poBackCMCR ;

		iResult = MDFac_BuildFromFile( ai_pccFile, M4CL_CSTYPE_BACK, 1, NULL, poBackCMCR ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}


		iResult = ao_rpoChannel->BuildBackFromMCR( poBackCMCR ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _CreateSystemAccess( void )
{

    m4return_t			iResult ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4VariantType		vValue ;
	ClTempTi			*poTisArray ;
	ClTempNode			*poNodesArray ;
	ClTempItem			*poItem ;
    ClNode				*poDNode ;


    if( m_iIsSystemBuilt == 1 )
    {
        return( M4_SUCCESS ) ;
    }

    iResult = _BuildSystemChannel( M4MDFAC_FILE_M4SYSTEM_M2, m_poSystemM4Obj ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    m_poSystemAccess = m_poSystemM4Obj->CreateAccess() ;
	CHECK_CHLOG_DEBUGF( m_poSystemAccess == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poDNode = &( m_poSystemAccess->Node[ M4MDFAC_NODE_SYS_ITEMS ] ) ;


	poDNode->RecordSet.Item[ M4CL_ITEM_LOAD ] ;

	if( poDNode->RecordSet.Item.GetIndex() == M4DM_EOF_INDEX )
	{
		return( M4_ERROR ) ;
	}

	iResult = poDNode->RecordSet.Item.Call() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poDNode->RecordSet.Item.StackPop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER || vValue.Data.DoubleData != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
	}


    m_oSystemUpperPool.Reset() ;
    m_oSystemNoUpperPool.Reset() ;

    m_poAccess = m_poSystemAccess ;
    m_poCMCR = m_poSystemAccess->GetpCMCR() ; ;


// Creamos el T3 del sistema ==================================================

    m_poSystemTempM4Obj = new ClTempChannel() ;
	CHECK_CHLOG_ERRORF( m_poSystemM4Obj == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempChannel ) ) << __LINE__ << __FILE__ ) ;

	m_poSystemTempM4Obj->SetUpperPool( &m_oSystemUpperPool ) ;
	m_poSystemTempM4Obj->SetNoUpperPool( &m_oSystemNoUpperPool ) ;

	m_poSystemTempM4Obj->SetChannelId( "" ) ;
    m_poSystemTempM4Obj->SetFactory( this ) ;


    iResult = _ReadT3InheritsFromAccess( m_poSystemTempM4Obj, &m_oSystemUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Creamos la ti del sistema  y la rellenamos =================================

    poTisArray = new ClTempTi[ 1 ] ;
	CHECK_CHLOG_ERRORF( poTisArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTi ) * 1 ) << __LINE__ << __FILE__ ) ;

	m_poSystemTempM4Obj->SetTisArray( poTisArray ) ;
	m_poSystemTempM4Obj->SetTisDelta( 1 ) ;

	poTisArray->SetUpperPool( &m_oSystemUpperPool ) ;
	poTisArray->SetNoUpperPool( &m_oSystemNoUpperPool ) ;

    iResult = poTisArray->SetTiId( M4MDFAC_TI_TI_SYSTEM ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    iResult = m_poSystemTempM4Obj->AddTi( poTisArray ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    iResult = poTisArray->SetReadObjectId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = poTisArray->SetWriteObjectId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iResult = m_poSystemTempM4Obj->CreateTiReferences() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = _ReadTiInheritsFromAccess( m_poSystemTempM4Obj ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    iResult = _ReadItemsFromAccess( m_poSystemTempM4Obj, &m_oSystemUpperPool, &m_oSystemNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	iLength = poTisArray->GetNumberOfItems() ;
	CHECK_CHLOG_ERROR( iLength == 0, M4_ERROR, M4_MDFAC_NO_SYSTEM_ITEMS ) ;


    for( i = 0 ; i < iLength ; i++ )
    {
        poItem = poTisArray->GetItemByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItem->SetFlag( M4CL_MCR_ITEM_FLAG_IS_INHERITED ) ;
	}

	poTisArray->SetItemsToCompile( iLength ) ;


    iResult = _ReadItemInheritsFromAccess( m_poSystemTempM4Obj ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    iResult = _ReadArgumentsFromAccess( m_poSystemTempM4Obj, &m_oSystemUpperPool, &m_oSystemNoUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    iResult = _ReadRulesFromAccess( m_poSystemTempM4Obj, &m_oSystemUpperPool ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Creamos el nodo del sistema  y lo rellenamos ===============================

    poNodesArray = new ClTempNode[ 1 ] ;
	CHECK_CHLOG_ERRORF( poNodesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNode ) * 1 ) << __LINE__ << __FILE__ ) ;

	m_poSystemTempM4Obj->SetNodesArray( poNodesArray ) ;
	m_poSystemTempM4Obj->SetNodesDelta( 1 ) ;

	poNodesArray->SetUpperPool( &m_oSystemUpperPool ) ;
	poNodesArray->SetNoUpperPool( &m_oSystemNoUpperPool ) ;

	poNodesArray->SetChannel( m_poSystemTempM4Obj ) ;


    iResult = m_poSystemTempM4Obj->AddNode( poNodesArray ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    poNodesArray->SetTi( poTisArray ) ;


// Node Inherits ==============================================================

    iResult = _ReadNodeInheritsFromAccess( m_poSystemTempM4Obj ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// ============================================================================
// Añadimos la herencia a los items ===========================================
// ============================================================================

	iResult = m_poSystemTempM4Obj->FillItemsWithInheritance() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

// Calculamos los totales y las fórmulas ======================================

    iResult = m_poSystemTempM4Obj->ComputeFormulas() ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    m_iIsSystemBuilt = 1 ;

    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _CreateMetaAccess( void )
{

    m4return_t      iResult ;


    if( m_iIsMetaBuilt == 1 )
    {
		return( M4_SUCCESS ) ;
    }

    iResult = _BuildSystemChannel( M4MDFAC_FILE_M4LOAD_M2, m_poMetaM4Obj ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    m_poMetaAccess = m_poMetaM4Obj->CreateAccess() ;
	CHECK_CHLOG_DEBUGF( m_poMetaAccess == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = m_poMetaAccess->Node[M4MDFAC_NODE_SYS_T3S_TRANS].RecordSet.SetCurrentSearchIndexByPos( 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
	}

	iResult = m_poMetaAccess->Node[M4MDFAC_NODE_SYS_ITEMS_INHERIT_TRANS].RecordSet.SetCurrentSearchIndexByPos( 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
	}

	iResult = m_poMetaAccess->Node[M4MDFAC_NODE_SYS_CONCEPTS_TRANS].RecordSet.SetCurrentSearchIndexByPos( 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
	}

	iResult = m_poMetaAccess->Node[M4MDFAC_NODE_SYS_NODES_TRANS].RecordSet.SetCurrentSearchIndexByPos( 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
	}

	iResult = m_poMetaAccess->Node[M4MDFAC_NODE_SYS_T3_CONNECTORS_TRANS].RecordSet.SetCurrentSearchIndexByPos( 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
	}

	iResult = m_poMetaAccess->Node[M4MDFAC_NODE_SYS_BUSINESS_METHODS_TRANS].RecordSet.SetCurrentSearchIndexByPos( 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
	}

    m_iIsMetaBuilt = 1 ;

	return( M4_SUCCESS ) ;
}





m4return_t  ClFactoryImp :: _CreateFromTemp( ClTempChannel *ao_poChannel, m4uint8_t ai_iCsType, ClStaticPoolStr* ao_poPool, ClStaticPoolStr* ao_poNoUpperPool, ClCompiledMCR* &ao_rpCMCR )
{

    m4return_t		iResult ;
	m4uint8_t		iDebug ;
	m4uint8_t		iIsSeparable ;
	m4uint8_t		iTimeUnit ;
    m4uint16_t		iBuild ;
    m4uint32_t		iTotalSize ;
    m4uint32_t		iChannelSize ;
    m4uint32_t		iUpperSize ;
    m4uint32_t		iNoUpperSize ;
    m4pcchar_t		pccPath ;
    m4pchar_t		pcCMCR ;
	ClCMCRWrapper	*poCMCRWrapper ;


// Calculamos los totales y las fórmulas

    iResult = ao_poChannel->ComputeFormulas() ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Calculamos el orden del grafo y el escope real

    iResult = ao_poChannel->ComputeGraphOrder() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

// Se buscan los ciclos de conectores de item

    iResult = ao_poChannel->ComputeConnectorOrder() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iIsSeparable = ao_poChannel->GetIsSeparable() ;

	if( iIsSeparable == 0 )
	{
		ai_iCsType = M4CL_CSTYPE_BOTH ;
	}


// El cstype lo ponemos antes empezar, para que tengamos esa información disponible durante la creación del cmcr
    ao_poChannel->SetCsType( ai_iCsType ) ;

	iResult = ao_poChannel->ComputeCS( ai_iCsType ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Calculamos las fórmulas del SYS_LOAD y SYS_PERSIST

    iResult = ao_poChannel->ComputeDBFormulas() ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    iChannelSize = 0 ;


// Esto es porque ahora los nitems del sistema están en el canal del sistema
// Cada vez reseteo el tipo del canal del sistema para que los métodos se instalen correctamente
	if( m_poSystemTempM4Obj != NULL )
	{
		m_poSystemTempM4Obj->SetCsType( ai_iCsType ) ;

		// También hay que resetear las fórmulas
		m_poSystemTempM4Obj->ClearFormulas() ;
	}


    iResult = ao_poChannel->PreGetSize() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	if( m_poStatistics != NULL )
	{
		m_poStatistics->Reset() ;
	}

    iResult = ao_poChannel->GetSize( iChannelSize, m_poStatistics ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    iUpperSize = ao_poPool->GetLength() ;

	if( ao_poNoUpperPool != NULL )
	{
		iNoUpperSize = ao_poNoUpperPool->GetLength() ;
	}
	else
	{
		iNoUpperSize = 0 ;
	}

    iTotalSize = iChannelSize + iUpperSize + iNoUpperSize ;

    ao_poChannel->SetSize( iTotalSize ) ;

	iBuild = GetM2VersionNumber() ;
    ao_poChannel->SetBuild( iBuild ) ;

	if( m_poEnvironment != NULL )
	{
		iTimeUnit = m_poEnvironment->GetIntegerTimeUnit() ;
	}
	else
	{
		iTimeUnit = M4_UNIT_ONE_DAY ;
	}

    ao_poChannel->SetTimeUnit( iTimeUnit ) ;
    ao_poChannel->SetPool( iUpperSize + iNoUpperSize ) ;

	poCMCRWrapper = new ClCMCRWrapper ;
	CHECK_CHLOG_ERRORF( poCMCRWrapper == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCMCRWrapper ) ) << __LINE__ << __FILE__ ) ;

    pcCMCR = poCMCRWrapper->CreateBuffer( iTotalSize ) ;

	if( pcCMCR == NULL )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		delete( poCMCRWrapper ) ;
		return( M4_ERROR ) ;
	}

	if( ao_poNoUpperPool != NULL )
	{
		iResult = ao_poChannel->Compile( pcCMCR, iChannelSize, iUpperSize ) ;
	}
	else
	{
		iResult = ao_poChannel->Compile( pcCMCR, iChannelSize, 0 ) ;
	}

    if( iResult != M4_SUCCESS )
	{
		delete( poCMCRWrapper ) ;
		return( M4_ERROR ) ;
	}


    ao_poPool->Copy( pcCMCR, iChannelSize ) ;

	if( ao_poNoUpperPool != NULL )
	{
		ao_poNoUpperPool->Copy( pcCMCR, iChannelSize + iUpperSize ) ;
	}

    ao_rpCMCR->SetCMCRWrapper( poCMCRWrapper ) ;



// Imprimir el metacanal y el canal temporal===================================

	if( m_poEnvironment != NULL )
	{
		iDebug = ClVMDebugEnv::getInstance()->getDebugLevel() ;
		pccPath = m_poEnvironment->GetDebugDirectory() ;

		ao_poChannel->Print( iDebug, pccPath ) ;
		ao_rpCMCR->PrintM4Obj( iDebug, pccPath, m_poStatistics ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _CheckVersion( m4pcchar_t ai_pccM4ObjId, m4date_t ai_dVersion, m4bool_t& ao_rbOld )
{

	m4return_t		iResult = M4_SUCCESS ;
	m4VariantType   vValue ;


	ao_rbOld = M4_FALSE ;


	// Se construye el canal de chequeo si no está construido y se posiciona en el item
    if( m_iIsCheckMetaBuilt == 0 )
	{
		iResult = m_poChannelManager->CreateChannel( m_poCheckMetaM4Obj ) ;

		if( iResult != M4_SUCCESS || m_poCheckMetaM4Obj == NULL )
		{
			m_poCheckMetaM4Obj = NULL ;
			return( M4_ERROR ) ;
		}

		iResult = m_poCheckMetaM4Obj->BuildFromId( M4_CHANNEL_MD_VERSION ) ;

		if( iResult != M4_SUCCESS )
		{
			m_poCheckMetaM4Obj->Destroy() ;
			m_poCheckMetaM4Obj = NULL ;
			return( M4_ERROR ) ;
		}

		/* Bug 0153637
		Para que los espacios proxy no borren el canal
		*/
		m_poChannelManager->Channel.Detach( m_poCheckMetaM4Obj ) ;

		m_poCheckMetaAccess = m_poCheckMetaM4Obj->CreateAccess() ;

		if( m_poCheckMetaAccess == NULL )
		{
			m_poCheckMetaM4Obj->Destroy() ;
			m_poCheckMetaM4Obj = NULL ;
			return( M4_ERROR ) ;
		}

		m_poCheckMetaNode = &( m_poCheckMetaAccess->Node[ M4_NODE_MD_VERSION ] ) ;

		if( m_poCheckMetaNode == NULL )
		{
			m_poCheckMetaAccess->Destroy() ;
			m_poCheckMetaM4Obj->Destroy() ;
			m_poCheckMetaAccess = NULL ;
			m_poCheckMetaM4Obj = NULL ;
			return( M4_ERROR ) ;
		}

		m_poCheckMetaNode->RecordSet.Item[ M4_METHOD_MD_VERSION_CHECK_M4OBJ_VERSION ] ;

		if( m_poCheckMetaNode->RecordSet.Item.GetIndex() == M4DM_EOF_INDEX != M4_SUCCESS )
		{
			m_poCheckMetaAccess->Destroy() ;
			m_poCheckMetaM4Obj->Destroy() ;
			m_poCheckMetaAccess = NULL ;
			m_poCheckMetaM4Obj = NULL ;
			return( M4_ERROR ) ;
		}

		m_iIsCheckMetaBuilt = 1 ;
	}


	// Se ejecuta el método de comprobación

    vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
    vValue.Data.PointerChar = (m4pchar_t) ai_pccM4ObjId ;

	iResult = m_poCheckMetaNode->RecordSet.Item.StackPush( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    vValue.Type = M4CL_CPP_TYPE_DATE ;
    vValue.Data.DoubleData = ai_dVersion ;

	iResult = m_poCheckMetaNode->RecordSet.Item.StackPush( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = m_poCheckMetaNode->RecordSet.Item.Call() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = m_poCheckMetaNode->RecordSet.Item.StackPop( vValue ) ;

	if( iResult != M4_SUCCESS || vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		return( M4_ERROR ) ;
	}

	if( vValue.Data.DoubleData == 1.0 )
	{
		ao_rbOld = M4_TRUE ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Genera un metacanal compilado a partir de su modelo en base de datos
//=================================================================================
m4return_t  ClFactoryImp :: MDFac_BuildFromId( ClBuidParameters &ai_roBP, ClCompiledMCR* &ao_rpCMCR )
{

    m4return_t      iResult = M4_SUCCESS ;
	m4bool_t		bCMCRFoundInCache ;
	m4bool_t		bSttEnabled ;
	m4uint8_t		iValid ;
	m4uint8_t		iType ;
	m4uint8_t		iHasSecurity ;
	m4uint8_t		iLanguage ;
	m4uint8_t		iDevLanguage ;
	m4uint16_t		iVerCounter ;
	m4uint32_t		iPlugHandle ;
	size_t			iLength ;
	m4pchar_t		pcRole ;
	m4pchar_t		pcMappingId ;
	m4char_t		acM4ObjId[ M4CL_MAX_T3_ID + 1 ] ;
	m4VariantType	vStartDate ;
	m4VariantType	vEndDate ;
	m4VariantType	vCorDate ;
	m4VariantType   vValue ;
	ClLogonAdaptor	*poLogonAdaptor = NULL ;
	ClCMCRWrapper	*poCMCRWrap = NULL ;
	ClCMCRWrapper	*poCMCRWrapAux = NULL ;
	ClVMStatistics	*poStatistics = NULL ;
	ClRowsQuotaController	*pRQC = NULL;
	m4bool_t		bEnabled = M4_FALSE;


	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_BuildFromId" ) ;

	pRQC = (ClRowsQuotaController*)ClThreadObjects::Get(ClThreadObjects::ROWS_QUOTA_CONTROLLER);

	poStatistics = m_poStatisticsWrap->GetStatistics() ;

	//Para tomar el tiempo desde que entra hasta que sale de esta funcion
	ClSttStateTimer oSttStateTimer( poStatistics, m_poStatisticsWrap->GetStatisticsLevel(), M4_VM_STT_SPECIAL_ITEM_CREATE_MCR ) ;


	bCMCRFoundInCache = M4_FALSE ;
	ao_rpCMCR = NULL ;

	CHECK_CHLOG_DEBUGF( ai_roBP.m_pccM4ObjId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Por el problema del locale con el Internet Explorer, sin ActiveDesktop.
// Cuando linkemos en estatico con la CRT no será necesario
	M4ResetLocale();


	// Se calculan los mapeos
	/* Bug 0106669
	Hay que limitar el tamaño
	*/
	iLength = strlen( ai_roBP.m_pccM4ObjId ) ;

	if( iLength > M4CL_MAX_T3_ID )
	{
		iLength = M4CL_MAX_T3_ID ;
	}
	memcpy( acM4ObjId, ai_roBP.m_pccM4ObjId, iLength ) ;
	acM4ObjId[ iLength ] = '\0' ;

	if( ai_roBP.m_bApplyMappings == M4_TRUE && ai_roBP.m_pccIdOrg != NULL )
	{
		bEnabled = M4_FALSE;
		if (pRQC != NULL)
		{
			bEnabled = pRQC->SwitchOff();
		}

		pcMappingId = MDFac_GetMappingById( "M4OBJECT_MAPPINGS", ai_roBP.m_pccIdOrg, acM4ObjId, M4_TRUE ) ;

		if (pRQC != NULL && bEnabled == M4_TRUE)
		{
			pRQC->SwitchOn();
		}

		if( pcMappingId != NULL )
		{
			iLength = strlen( pcMappingId ) ;

			if( iLength > M4CL_MAX_T3_ID )
			{
				iLength = M4CL_MAX_T3_ID ;
			}
			memcpy( acM4ObjId, pcMappingId, iLength ) ;
			acM4ObjId[ iLength ] = '\0' ;
			delete [] pcMappingId ;
		}
	}
	

	// first of all we check the security context
	if( m_poChannelManager != NULL && ai_roBP.m_pccIdOrg != NULL && ai_roBP.m_pccRSM != NULL && strcmpi( ai_roBP.m_pccRSM, M4CL_SEC_SUPER_USER ) != 0 )
	{
		if( m_poChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE )
		{
			poLogonAdaptor = m_poChannelManager->GetLogonAdaptor() ;

			if( poLogonAdaptor != NULL )
			{
				bEnabled = M4_FALSE;
				if (pRQC != NULL)
				{
					bEnabled = pRQC->SwitchOff();
				}

				iValid = poLogonAdaptor->CheckProjectTreeValidation( acM4ObjId, (m4pchar_t) ai_roBP.m_pccIdOrg ) ;

				if (pRQC != NULL && bEnabled == M4_TRUE)
				{
					pRQC->SwitchOn();
				}

				if ( iValid != 1 )
				{
					return( M4_ERROR ) ;
				}
			}
		}
	}


// ala, para pinchar el cstype a un valor correcto

	if( ai_roBP.m_iCsType == 0 )
	{
		ai_roBP.m_iCsType = M4CL_CSTYPE_BOTH ;
	}
	/* Bug 0150731
	Ya no se tienen metacanales de debug, son de tipo back
	if( ai_roBP.m_iCsType != M4CL_CSTYPE_DEBUG )
	*/
	ai_roBP.m_iCsType &= M4CL_CSTYPE_BOTH ;


// comprobación de fechas


	if( ai_roBP.m_dStartDate < M4CL_MINUS_INFINITE_IN_JULIAN || ai_roBP.m_dStartDate > M4CL_PLUS_INFINITE_IN_JULIAN )
	{
		ai_roBP.m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	}

	if( ai_roBP.m_dEndDate < M4CL_MINUS_INFINITE_IN_JULIAN || ai_roBP.m_dEndDate > M4CL_PLUS_INFINITE_IN_JULIAN )
	{
		ai_roBP.m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
	}

	if( ai_roBP.m_dCorDate < M4CL_MINUS_INFINITE_IN_JULIAN || ai_roBP.m_dCorDate > M4CL_PLUS_INFINITE_IN_JULIAN )
	{
		ai_roBP.m_dCorDate = ClActualDate() ;
	}

	vStartDate.Type = M4CL_CPP_TYPE_DATE ;
	vStartDate.Data.DoubleData = ai_roBP.m_dStartDate;

	vEndDate.Type = M4CL_CPP_TYPE_DATE ;
	vEndDate.Data.DoubleData = ai_roBP.m_dEndDate;

	vCorDate.Type = M4CL_CPP_TYPE_DATE ;
	vCorDate.Data.DoubleData = ai_roBP.m_dCorDate;



    ao_rpCMCR = new ClCompiledMCR ;
	CHECK_CHLOG_ERRORF( ao_rpCMCR == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCompiledMCR ) ) << __LINE__ << __FILE__ ) ;

	ao_rpCMCR->SetCacheCsType( ai_roBP.m_iCsType ) ;


	bSttEnabled = oSttStateTimer.GetSttEnabled() ;


	if( m_poMDCache != NULL )
	{

// ============================================================================
// Pedazo de apaño para el tema de los idiomas de logon y desarrollo
// y las cachés de metadatos
// ============================================================================

		// Si no viene el lenguaje hay que buscarse la vida según el tipo de canal
		if( ai_roBP.m_iLanguage == M4CL_LANGUAGE_NONE )
		{
			iLanguage    = m4uint8_t( m_poLogonAdaptor->GetSessionLanguage   ( acM4ObjId, M4CL_LANGUAGE_NONE ) ) ;
			iDevLanguage = m4uint8_t( m_poLogonAdaptor->GetSessionDevLanguage( acM4ObjId, M4CL_LANGUAGE_NONE ) ) ;

			// Primero lo buscamos por el de logon
			iResult = m_poMDCache->GetObject( ai_roBP.m_iCsType, acM4ObjId, iLanguage, vStartDate, vEndDate, vCorDate, iVerCounter, poCMCRWrap ) ;

			if( iResult == M4_SUCCESS )
			{
				// Hay que ver si es lo que queremos
				iType = poCMCRWrap->GetMetaLanguage() ;

				if( iType != M4CL_LANGUAGE_TYPE_LOGON )
				{
					// Si es de desarrollo podría coincidir el lenguaje y lo habríamos encontrado
					// Si no lo hemos encontrado probamos con el de desarrollo
					if( iDevLanguage != iLanguage )
					{
						// Hay que destruirlo porque no lo queremos
						poCMCRWrap->Destroy() ;
						poCMCRWrap = NULL ;

						iResult = m_poMDCache->GetObject( ai_roBP.m_iCsType, acM4ObjId, iDevLanguage, vStartDate, vEndDate, vCorDate, iVerCounter, poCMCRWrap ) ;
					}
				}
			}
			else
			{
				// Si no lo hemos encontrado probamos con el de desarrollo
				// Si es el mismo lenguaje y no estaba no hay que volver a buscar, no está
				if( iDevLanguage != iLanguage )
				{
					iResult = m_poMDCache->GetObject( ai_roBP.m_iCsType, acM4ObjId, iDevLanguage, vStartDate, vEndDate, vCorDate, iVerCounter, poCMCRWrap ) ;

					if( iResult == M4_SUCCESS )
					{
						// Hay que ver si es lo que queremos
						iType = poCMCRWrap->GetMetaLanguage() ;

						if( iType != M4CL_LANGUAGE_TYPE_DEVELOPMENT )
						{
							// Hay que destruirlo porque no lo queremos
							poCMCRWrap->Destroy() ;
							poCMCRWrap = NULL ;

							iResult = M4_ERROR ;
						}
					}
				}
			}
		}
// ============================================================================
// Fin de apaño
// ============================================================================
		else
		{
			// Si viene, lo hacemos como siempre
			iResult = m_poMDCache->GetObject( ai_roBP.m_iCsType, acM4ObjId, ai_roBP.m_iLanguage, vStartDate, vEndDate, vCorDate, iVerCounter, poCMCRWrap ) ;
		}

		/* Bug 0152188
		Si es volatil hay que chequear que no esté obsoleto
		*/
		if( iResult == M4_SUCCESS )
		{
			if( poCMCRWrap->GetIsMDCacheable() == M4CL_CACHEABLE_VOLATILE )
			{
				m4bool_t	bOld = M4_FALSE ;
				m4return_t	iCheck = _CheckVersion( acM4ObjId, poCMCRWrap->GetVersion(), bOld ) ;

				if( iCheck == M4_SUCCESS && bOld == M4_TRUE )
				{
					// Si es obsoleto lo ignoramos
					poCMCRWrap->Destroy() ;
					poCMCRWrap = NULL ;
					iResult = M4_ERROR ;

					// Se borra el diccionario en parte server
					if( m_poLdb != NULL && ( p_GetContext() == eM4MdFacServerEmul || p_GetContext() == eM4MdFacServer ) )
					{
						m_poLdb->ClearAll() ;
					}
				}
			}
		}

		if( bSttEnabled == M4_TRUE )
		{
			//Ponemos el cachehit a las Stt

			if( iResult == M4_SUCCESS )
			{
				poStatistics->sttAddCacheHit( M4_TRUE ) ;
			}
			else
			{
				poStatistics->sttAddCacheHit( M4_FALSE ) ;
			}
		}
	}
	else
	{
		iResult = M4_ERROR ;
	}

	if( bSttEnabled == M4_TRUE )
	{
		if( poStatistics->sttGetInitState() & M4_VM_STATISTICS_ENABLED_PARAMS )
		{
			poStatistics->sttAddExecParam( (m4pchar_t) acM4ObjId ) ;
			poStatistics->sttAddExecParam( (m4double_t) ai_roBP.m_iLanguage ) ;
			poStatistics->sttAddExecParam( (m4double_t) ai_roBP.m_iCsType ) ;
		}
	}


	if( iResult == M4_SUCCESS )
	{
		//Si está en la cache.
		if( m_poCsTrace->IsEnabled() == M4_TRUE )
		{
			pcRole = m_poLogonAdaptor->GetRole() ;

			m_poCsTrace->PreMetaDataTrace() ;
			m_poCsTrace->PostMetaDataTrace( 0, 0, acM4ObjId, ai_roBP.m_iCsType, pcRole, M4_TRUE ) ;
		}

		ao_rpCMCR->SetCMCRWrapper( poCMCRWrap ) ;
		bCMCRFoundInCache = M4_TRUE ;
	}
	else
	{
		bEnabled = M4_FALSE;
		if (pRQC != NULL)
		{
			bEnabled = pRQC->SwitchOff();
		}

		// Llamar a remoto si hace falta. Función polimórfica
		iResult = p_DelegateMDFromId( acM4ObjId, ai_roBP, vStartDate, vEndDate, vCorDate, ao_rpCMCR, poCMCRWrap ) ;

		if (pRQC != NULL && bEnabled == M4_TRUE)
		{
			pRQC->SwitchOn();
		}

		if( iResult != M4_SUCCESS )
		{
			ao_rpCMCR->Destroy() ;
			ao_rpCMCR = NULL ;
			return( M4_ERROR ) ;
		}

		if( m_poMDCache != NULL )
		{
			//Borramos de la caché todos los MCRs con el mismo ID y versión anterior.
			m_poMDCache->RemoveObjectByIdAndVersion( acM4ObjId, poCMCRWrap->GetVersion(), ClBaseCacheInterface::MAIN ) ;

			if( m_poCacheDirectory != NULL )
			{
				//Borramos del resto de las caché todos los SCRs y presentaciones creados a partir de versiones anteriores del MCR.
				m_poCacheDirectory->RemoveObjectByIdAndVersion( M4_CACHE_CSCR_CACHE_TYPE | M4_CACHE_PRES_CACHE_TYPE, (m4pchar_t)acM4ObjId, poCMCRWrap->GetVersion(), ClBaseCacheInterface::PARENT ) ;
			}

			if( ao_rpCMCR->GetChannelIsMDCacheable() != M4CL_CACHEABLE_FALSE )
			{
				/* Bug 0075632
				Si está el la caché se borra antes de insertarlo siempre
				*/
				iResult = m_poMDCache->GetObject( ai_roBP.m_iCsType, acM4ObjId, ao_rpCMCR->GetChannelLanguage(), vStartDate, vEndDate, vCorDate, iVerCounter, poCMCRWrapAux ) ;

				if( iResult == M4_SUCCESS )
				{
					if( poCMCRWrapAux != NULL )
					{
						m_poMDCache->RemoveObject( poCMCRWrapAux ) ;
						poCMCRWrapAux->Destroy() ;
						poCMCRWrapAux = NULL ;
					}
				}

				iResult = m_poMDCache->PutObject( ai_roBP.m_iCsType, acM4ObjId, ao_rpCMCR->GetChannelLanguage(), iVerCounter, poCMCRWrap ) ;

				if( iResult == M4_ERROR )
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_CACHE_PUT_CMCR, LOG_CAT_AND( ao_rpCMCR->iM4Obj(), ao_rpCMCR->nM4Obj() ) << M4ClCsType( ai_roBP.m_iCsType ) ) ;
				}
			}
		}
	}


// si hay carril lo metemos ==========================

	if( ai_roBP.m_poLane != NULL )
	{
		if( ai_roBP.m_poLane->m_poFatherCMCR == NULL || ai_roBP.m_poLane->m_pccNode == NULL || ai_roBP.m_poLane->m_pccFatherNode == NULL )
		{
			DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_M4OBJ, LOG_CAT_AND( ao_rpCMCR->iM4Obj(), ao_rpCMCR->nM4Obj() ) << M4ClCsType( ai_roBP.m_iCsType ) ) ;
			ao_rpCMCR->Destroy() ;
			ao_rpCMCR = NULL ;
			return( M4_ERROR ) ;
		}

		iResult = ao_rpCMCR->ConnectNodeToNode( ai_roBP.m_poLane->m_pccNode, ai_roBP.m_poLane->m_poFatherCMCR, ai_roBP.m_poLane->m_pccFather, ai_roBP.m_poLane->m_pccFatherNode, iPlugHandle ) ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_M4OBJ, LOG_CAT_AND( ao_rpCMCR->iM4Obj(), ao_rpCMCR->nM4Obj() ) << M4ClCsType( ai_roBP.m_iCsType ) ) ;
			ao_rpCMCR->Destroy() ;
			ao_rpCMCR = NULL ;
			return( M4_ERROR ) ;
		}
	}


// si hay seguridad la metemos =====================

	if( ai_roBP.m_bBuildSecurity == M4_TRUE )
	{
		iHasSecurity = ao_rpCMCR->GetChannelHasSecurity() ;

		if( iHasSecurity == 1 )
		{
			if( ai_roBP.m_bConvertRSM == M4_TRUE )
			{
				if( ai_roBP.m_pccRSM != NULL && strcmpi( ai_roBP.m_pccRSM, M4CL_SEC_SUPER_USER ) == 0 )
				{
					iHasSecurity = 0 ;
				}
			}
			else
			{
				if( ai_roBP.m_pccRSM != NULL && *ai_roBP.m_pccRSM == '\0' )
				{
					iHasSecurity = 0 ;
				}
			}

			if( iHasSecurity == 1 )
			{
				iResult = _CreateSecFromId( ai_roBP.m_pccRSM, ai_roBP.m_bConvertRSM, ao_rpCMCR ) ;

				if( iResult != M4_SUCCESS )
				{
					ao_rpCMCR->Destroy() ;
					ao_rpCMCR = NULL ;
					return( M4_ERROR ) ;
				}

				if( bCMCRFoundInCache == M4_TRUE )
				{
					//Comprobamos las fechas de metacanal y mascara 
					if( poCMCRWrap->GetVersion() < ao_rpCMCR->GetSecurityT3Version() )
					{
						// El metacanal cacheado esta desfasado: 
						// Pasar el CMCR a la cola de borrado, Liberar el MCR y quitar referencias, y Repetir el Build (que ya no encontrara el metacanal en la cache).
						m_poMDCache->RemoveObject( poCMCRWrap ) ;
						ao_rpCMCR->Destroy() ;
						ao_rpCMCR = NULL ;
						return( MDFac_BuildFromId( ai_roBP, ao_rpCMCR ) ) ;
					}
				}
			}
		}
	}


    return( M4_SUCCESS ) ;
}




//=================================================================================
// Genera un wrapper de metacanal compilado a partir de su modelo en base de datos
//=================================================================================
m4return_t  ClFactoryImp :: MDFac_BuildMDWrapper( m4pcchar_t ai_pccM4ObjId, m4pcchar_t ai_pccOrgId, m4uint8_t ai_iCsType, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, m4uint8_t ai_iLanguage, m4bool_t ai_bApplyMappings, ClCMCRWrapper* &ao_rpoMDWrap )
{

	m4return_t			iResult ;
	ClCompiledMCR		*poMCR ;
	ClBuidParameters	oParameters ;


	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_BuildMDWrapper" ) ;


	oParameters.m_poLane = NULL ;				// No hay carril
	oParameters.m_pccM4ObjId = ai_pccM4ObjId ;
	oParameters.m_pccIdOrg = ai_pccOrgId ;
	oParameters.m_pccRSM = NULL ;				// No hay máscara
	oParameters.m_dStartDate = ai_roStartDate.Data.DoubleData ;
	oParameters.m_dEndDate = ai_roEndDate.Data.DoubleData ;
	oParameters.m_dCorDate = ai_roCorDate.Data.DoubleData ;
	oParameters.m_iCsType = ai_iCsType ;
	oParameters.m_iLanguage = ai_iLanguage ;
	oParameters.m_bConvertRSM = M4_FALSE ;		// No hay que convertir la máscara
	oParameters.m_bBuildSecurity = M4_FALSE ;	// No construimos seguridad
	oParameters.m_bApplyMappings = ai_bApplyMappings ;


	iResult = MDFac_BuildFromId( oParameters, poMCR );

	if( iResult != M4_ERROR )
	{
		if( poMCR->GetCSCRWrapper() )
		{
			poMCR->GetCSCRWrapper()->Destroy() ;
		}

		ao_rpoMDWrap = poMCR->GetCMCRWrapper() ;

		delete( poMCR ) ;
	}

	return( iResult ) ;
}



//=================================================================================
// Genera un wrapper de seguridad compilada a partir de su modelo en base de datos
//=================================================================================
m4return_t	ClFactoryImp :: MDFac_BuildSecWrapper( m4pcchar_t ai_pccM4ObjId, m4uint8_t ai_iCsType, m4pcchar_t ai_pccMask, m4uint8_t ai_iLanguage, ClCSCRWrapper* &ai_poSecWrap )
{

	m4return_t			iResult ;
	ClCompiledMCR		*poMCR ;
	ClBuidParameters	oParameters ;


	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_BuildSecWrapper" ) ;


	oParameters.m_poLane = NULL ;				// No hay carril
	oParameters.m_pccM4ObjId = ai_pccM4ObjId ;
	oParameters.m_pccIdOrg = NULL ;
	oParameters.m_pccRSM = ai_pccMask ;
	oParameters.m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	oParameters.m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
	oParameters.m_dCorDate = ClActualDate() ;
	oParameters.m_iCsType = ai_iCsType ;
	oParameters.m_iLanguage = ai_iLanguage ;
	oParameters.m_bConvertRSM = M4_FALSE ;
	oParameters.m_bBuildSecurity = M4_TRUE ;
	oParameters.m_bApplyMappings = M4_FALSE ;	// En seguridad no se aplican mapeos


//De momento!! Se puede optimizar.

	iResult = MDFac_BuildFromId( oParameters, poMCR );

	if( iResult != M4_ERROR )
	{
		if( poMCR->GetCMCRWrapper() )
		{
			poMCR->GetCMCRWrapper()->Destroy() ;
		}

		ai_poSecWrap = poMCR->GetCSCRWrapper() ;

		delete( poMCR ) ;
	}

	return( iResult ) ;
}



//=================================================================================
// Obtiene un tipo concreto de mapeos
//=================================================================================
m4return_t  ClFactoryImp :: MDFac_BuildMappings( m4pcchar_t ai_pccMapId, ClMappings* &ai_rpoMappings )
{

	m4return_t		iResult = M4_SUCCESS ;
//	m4bool_t		bSttEnabled = M4_FALSE ;
//	m4pchar_t		pcRole = NULL ;
//	ClVMStatistics	*poStatistics = NULL ;
	ClMappings		*poMappings = NULL ;
	ClMappings		*poMappingsAux = NULL ;


	ai_rpoMappings = NULL ;

	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_BuildMappings" ) ;


/*
	No se vuelcan ni estadísticas ni trazas
	poStatistics = m_poStatisticsWrap->GetStatistics() ;

	//Para tomar el tiempo desde que entra hasta que sale de esta funcion
	ClSttStateTimer	oSttStateTimer( poStatistics, m_poStatisticsWrap->GetStatisticsLevel(), M4_VM_STT_SPECIAL_ITEM_CREATE_MAP ) ;

	bSttEnabled = oSttStateTimer.GetSttEnabled() ;
*/

	if( m_poMapCache != NULL )
	{
		iResult = m_poMapCache->GetObject( ai_pccMapId, poMappings ) ;

/*
		if( bSttEnabled == M4_TRUE )
		{
			//Ponemos el cachehit a las Stt

			if( iResult == M4_SUCCESS )
			{
				poStatistics->sttAddCacheHit( M4_TRUE ) ;
			}
			else
			{
				poStatistics->sttAddCacheHit( M4_FALSE ) ;
			}
		}
*/
	}
	else
	{
		iResult = M4_ERROR ;
	}

/*
	if( bSttEnabled == M4_TRUE )
	{
		if( poStatistics->sttGetInitState() & M4_VM_STATISTICS_ENABLED_PARAMS )
		{
			poStatistics->sttAddExecParam( (m4pchar_t) ai_pccMapId ) ;
		}
	}
*/

	if( iResult == M4_SUCCESS )
	{
		//Si está en la cache.
/*
		if( m_poCsTrace->IsEnabled() == M4_TRUE )
		{
			pcRole = m_poLogonAdaptor->GetRole() ;

			m_poCsTrace->PreMapTrace() ;
			m_poCsTrace->PostMapTrace( 0, 0, 0, 0, ai_pccMapId, pcRole, M4_TRUE ) ;
		}
*/
		ai_rpoMappings = poMappings ;
		return( M4_SUCCESS ) ;
	}


	// Llamar a remoto si hace falta. Función polimórfica
	iResult = p_DelegateMapFromId( ai_pccMapId, poMappings ) ;

	if( iResult != M4_SUCCESS )
	{
		if( poMappings != NULL )
		{
			poMappings->Destroy() ;
		}
		return( M4_ERROR ) ;
	}


	if( m_poMapCache != NULL )
	{
		//Borramos de la caché todos los Maps con el mismo ID y versión anterior.
		m_poMapCache->RemoveObjectByIdAndVersion( (m4pchar_t) ai_pccMapId, poMappings->GetVersion(), ClBaseCacheInterface::MAIN ) ;

		//Hacemos el Put de la caché si es que no está
		iResult = m_poMapCache->GetObject( ai_pccMapId, poMappingsAux ) ;

		if( iResult == M4_SUCCESS )
		{
			if( poMappingsAux != NULL )
			{
				poMappingsAux->Destroy() ;
			}
		}
		else
		{
			iResult = m_poMapCache->PutObject( ai_pccMapId, poMappings ) ;

			if( iResult == M4_ERROR )
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_CACHE_PUT_MAP, ai_pccMapId ) ;
			}
		}
	}

	ai_rpoMappings = poMappings ;

	return( iResult ) ;
}





//=================================================================================
// Obtiene un mapeo para un tipo de mapeo, una sociedad y un origen
//=================================================================================
m4pchar_t	ClFactoryImp :: MDFac_GetMappingById( m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccSource, m4bool_t ai_bConvert )
{

    m4return_t	iResult = M4_SUCCESS ;
	size_t		iLength = 0 ;
	m4pchar_t	pcResult = NULL ;
	m4pchar_t	pcMapId = NULL ;
	m4pcchar_t	pccM4ObjId = NULL ;
	m4char_t	acBuffer[ M4CL_MAX_T3_ID + 1 + M4CL_MAX_ORG_ID + 1 ] ;
	ClMappings	*poMappings = NULL ;


	pcMapId = _ComposeMappingId( ai_pccMapId, ai_pccOrganization, ai_bConvert, acBuffer, M4CL_MAX_T3_ID + 1 + M4CL_MAX_ORG_ID ) ;

	if( pcMapId == NULL )
	{
		return( NULL ) ;
	}

	iResult = MDFac_BuildMappings( pcMapId, poMappings ) ;

	if( iResult != M4_SUCCESS || poMappings == NULL )
	{
		return( NULL ) ;
	}

	pccM4ObjId = poMappings->GetMappingById( ai_pccOrganization, ai_pccSource, ai_bConvert ) ;

	if( pccM4ObjId != NULL )
	{
		iLength = strlen( pccM4ObjId ) + 1 ;
		pcResult = new m4char_t[ iLength ] ;
		CHECK_CHLOG_ERRORF( pcResult == NULL, NULL, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iLength ) << __LINE__ << __FILE__ ) ;

		memcpy( pcResult, pccM4ObjId, iLength ) ;
	}

	poMappings->Destroy() ;

	return( pcResult ) ;
}





//=================================================================================
// Determina si 2 sociedades son equivalentes para un tipo de mapeo y un destino
//=================================================================================
m4return_t	ClFactoryImp :: MDFac_IsEqualOrg( m4pcchar_t ai_pccMapId, m4pcchar_t ai_pccOrganization1, m4pcchar_t ai_pccOrganization2, m4pcchar_t ai_pccDestination, m4bool_t ai_bConvert, m4bool_t& ao_rbIsEquivalent )
{

    m4return_t	iResult = M4_SUCCESS ;
	m4pchar_t	pcMapId1 = NULL ;
	m4pchar_t	pcMapId2 = NULL ;
	m4char_t	acBuffer1[ M4CL_MAX_T3_ID + 1 + M4CL_MAX_ORG_ID + 1 ] ;
	m4char_t	acBuffer2[ M4CL_MAX_T3_ID + 1 + M4CL_MAX_ORG_ID + 1 ] ;
	ClMappings	*poMappings1 = NULL ;
	ClMappings	*poMappings2 = NULL ;


	ao_rbIsEquivalent = M4_FALSE ;

	pcMapId1 = _ComposeMappingId( ai_pccMapId, ai_pccOrganization1, ai_bConvert, acBuffer1, M4CL_MAX_T3_ID + 1 + M4CL_MAX_ORG_ID ) ;

	if( pcMapId1 == NULL )
	{
		return( NULL ) ;
	}

	pcMapId2 = _ComposeMappingId( ai_pccMapId, ai_pccOrganization2, ai_bConvert, acBuffer2, M4CL_MAX_T3_ID + 1 + M4CL_MAX_ORG_ID ) ;

	if( pcMapId2 == NULL )
	{
		return( NULL ) ;
	}

	iResult = MDFac_BuildMappings( pcMapId1, poMappings1 ) ;

	if( iResult != M4_SUCCESS || poMappings1 == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = MDFac_BuildMappings( pcMapId2, poMappings2 ) ;

	if( iResult != M4_SUCCESS || poMappings2 == NULL )
	{
		poMappings1->Destroy() ;
		return( M4_ERROR ) ;
	}

	iResult = ClMappings::IsEquivalentOrganization( poMappings1, poMappings2, ai_pccOrganization1, ai_pccOrganization2, ai_pccDestination, ai_bConvert, ao_rbIsEquivalent ) ;

	poMappings1->Destroy() ;
	poMappings2->Destroy() ;

	return( iResult ) ;
}





//=================================================================================
// Asocia una máscara de seguridada a un metacanal compilado
//=================================================================================
m4return_t  ClFactoryImp :: MDFac_AttachRSM( ClCompiledMCR *ai_poCMCR, m4pcchar_t ai_pccRSM )
{

    m4return_t		iResult ;
	m4uint8_t		iHasSecurity ;

	
	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_AttachRSM" ) ;
	CHECK_CHLOG_DEBUGF( ai_pccRSM == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iHasSecurity = ai_poCMCR->GetChannelHasSecurity() ;

	if( iHasSecurity == 1 )
	{
		if( ai_pccRSM != NULL && strcmpi( ai_pccRSM, M4CL_SEC_SUPER_USER ) == 0 )
		{
			iHasSecurity = 0 ;
		}

		if( iHasSecurity == 1 )
		{
// siempre es RSM
			iResult = _CreateSecFromId( ai_pccRSM, 1, ai_poCMCR ) ;
			// ya sale por debajo
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_SECURITY, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccRSM << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
		}
	}


    return( M4_SUCCESS ) ;
}




//=================================================================================
// Construye accesos nuevos sobre los metacanales del diccionario
//=================================================================================
m4return_t	ClFactoryImp :: MDFac_BuildDictAccess( ClAccess* &ai_rpoDictAccess, ClAccess* &ai_rpoTransAccess )
{

    m4return_t      iResult ;

    
	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_BuildDictAccess" ) ;


    if( m_iIsDictBuilt == 0 )
    {
        m_oDictAccesses.SetDelta( 1 ) ;

        iResult = _BuildSystemChannel( M4MDFAC_FILE_M4DICT_M2, m_poDictM4Obj ) ;

        if( iResult != M4_SUCCESS )
        {
		    return( M4_ERROR ) ;
        }

        m_iIsDictBuilt = 1 ;
    }

    if( m_iIsTransBuilt == 0 )
    {
        m_oTransAccesses.SetDelta( 1 ) ;

        iResult = _BuildSystemChannel( M4MDFAC_FILE_M4TRANS_M2, m_poTransM4Obj ) ;

        if( iResult != M4_SUCCESS )
        {
		    return( M4_ERROR ) ;
        }

        m_iIsTransBuilt = 1 ;
    }

    ai_rpoDictAccess = m_poDictM4Obj->CreateAccess() ;
	CHECK_CHLOG_DEBUGF( ai_rpoDictAccess == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oDictAccesses.AddElement( ai_rpoDictAccess ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    ai_rpoTransAccess = m_poTransM4Obj->CreateAccess() ;
	CHECK_CHLOG_DEBUGF( ai_rpoTransAccess == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTransAccesses.AddElement( ai_rpoTransAccess ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    return( M4_SUCCESS ) ;
}


//=================================================================================
// Borra accesos sobre los metacanales del diccionario
//=================================================================================
m4return_t	ClFactoryImp :: MDFac_RemoveDictAccess( ClAccess *ai_poDictAccess, ClAccess *ai_poTransAccess )
{

	m4return_t	        iResult ;
	m4uint8_t	        iFound ;
	m4uint16_t	        i ;
	m4uint16_t	        iLength ;
	m4pvoid_t           pvPointer ;
	ClAccess            *poAccess ;

    
	CHECK_CHLOG_ERRORF( m_bIsInit == M4_FALSE, M4_ERROR, M4_MDFAC_NOT_INIT, "ClFactoryImp :: MDFac_RemoveDictAccess" ) ;


	if( m_iIsDictBuilt == 1 )
    {
        iLength = m_oDictAccesses.GetLength() ;

		iFound = 0 ;
	    for( i = 0 ; i < iLength && iFound == 0 ; i++ )
	    {
		    iResult = m_oDictAccesses.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		    if( pvPointer != NULL )
		    {
			    poAccess = ( ClAccess* ) pvPointer ;

				if( poAccess == ai_poDictAccess )
				{
					iFound = 1 ;
					m_oDictAccesses.SetElement( i, NULL ) ;
					poAccess->Destroy() ;
				}
		    }
	    }
	}

	if( m_iIsTransBuilt == 1 )
    {
        iLength = m_oTransAccesses.GetLength() ;

		iFound = 0 ;
	    for( i = 0 ; i < iLength && iFound == 0 ; i++ )
	    {
		    iResult = m_oTransAccesses.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		    if( pvPointer != NULL )
		    {
			    poAccess = ( ClAccess* ) pvPointer ;

				if( poAccess == ai_poTransAccess )
				{
					iFound = 1 ;
					m_oTransAccesses.SetElement( i, NULL ) ;
					poAccess->Destroy() ;
				}
		    }
	    }
	}

    return( M4_SUCCESS ) ;
}





//=================================================================================
//
// ClFactoryClient
//
// Clase que implementa la factoría cliente base de metadatos y seguridad
//
//=================================================================================


m4return_t	ClFactoryClient :: Init	(
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
									)
{

	m4return_t	iResult ;


	if( m_bIsInit == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}

	if( ai_poMDAdaptor == NULL )
	{
		return( M4_ERROR ) ;
	}

	m_poMDAdaptor = ai_poMDAdaptor ;

	iResult = ClFactoryImp :: Init( ai_poChannelManager, ai_poEnvironment, ai_poLogonAdaptor, ai_poMDCache, ai_poSecCache, ai_poMapCache, ai_poCacheDirectory, ai_poStatisticsWrap, ai_poFunctionTable, ai_poAtributeTable, ai_poCsTrace ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	m_bIsInit = M4_TRUE ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryClient :: End( void )
{

	m4return_t	iResult ;

	
	if( m_bIsInit == M4_FALSE )
	{
		return( M4_SUCCESS ) ;
	}

	m_poMDAdaptor = NULL ;

	iResult = ClFactoryImp :: End() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	m_bIsInit = M4_FALSE ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryClient :: p_DelegateMDFromId( m4pcchar_t ai_pccM4ObjId, ClBuidParameters &ai_roBP, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, ClCompiledMCR *ai_poCMCR, ClCMCRWrapper* &ao_rpoCMCRWrap )
{

    m4return_t	iResult ;


	iResult = m_poMDAdaptor->RemoteGetCMCR( ai_roBP.m_iCsType, ai_pccM4ObjId, ai_roBP.m_pccIdOrg, ai_roBP.m_iLanguage, ai_roBP.m_bApplyMappings, ai_roStartDate, ai_roEndDate, ai_roCorDate, ao_rpoCMCRWrap, m_poStatisticsWrap ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	ai_poCMCR->SetCMCRWrapper( ao_rpoCMCRWrap ) ;

    return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryClient :: p_DelegateMapFromId( m4pcchar_t ai_pccMapId, ClMappings* &ao_rpoMappings )
{

    m4return_t	iResult ;


	iResult = m_poMDAdaptor->RemoteGetMap( ai_pccMapId, ao_rpoMappings, m_poStatisticsWrap ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

    return( M4_SUCCESS ) ;
}


//=================================================================================
//
// ClFactoryServer
//
// Clase que implementa la factoría server base de metadatos y seguridad
//
//=================================================================================


m4return_t	ClFactoryServer :: p_DelegateMDFromId( m4pcchar_t ai_pccM4ObjId, ClBuidParameters &ai_roBP, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, ClCompiledMCR *ai_poCMCR, ClCMCRWrapper* &ao_rpoCMCRWrap )
{

	m4return_t			iResult ;
	m4bool_t			bIsNotEof ;
	m4uint32_t			iType ;
	m4VariantType		vValue ;
    ClCompiledMCR		*poCMCR ;
    ClNode				*poDNode ;
    ClAccessRecordSet   *poDRecordSet ;
    ClRegister          *poRegister ;


	if( m_iIsMetaBuilt == 0 )
	{
		iResult = _CreateMetaAccess() ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;
	}
	else
	{
		// Limpiamos todo el contenido del metacanal
		m_poMetaM4Obj->ReleaseAll() ;
	}


// Ejecutamos la carga normal =================================================

	poDNode = &( m_poMetaAccess->Node[ M4MDFAC_NODE_SYS_T3S ] ) ;
    poDRecordSet = &( poDNode->RecordSet ) ;
    poRegister = &( poDRecordSet->Register ) ;

	iResult = poRegister->Item[ M4MDFAC_T3S_DT_START ].Value.Set( ai_roStartDate ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	iResult = poRegister->Item[ M4MDFAC_T3S_DT_END ].Value.Set( ai_roEndDate ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	iResult = poRegister->Item[ M4MDFAC_T3S_DT_CORR ].Value.Set( ai_roCorDate ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	iResult = poRegister->Item[ M4MDFAC_T3S_ID_T3 ].Value.Set( ai_pccM4ObjId, strlen( ai_pccM4ObjId ) + 1 ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	poRegister->Item[ M4MDFAC_T3S_LOAD ] ;
	CHECK_CHLOG_ERRORF( poRegister->Item.GetIndex() == M4DM_EOF_INDEX, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	iResult = poRegister->Item.Call() ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	iResult = poRegister->Item.StackPop( vValue ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS || vValue.Type != M4CL_CPP_TYPE_NUMBER || vValue.Data.DoubleData != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	bIsNotEof = poRegister->Begin() ;
	CHECK_CHLOG_ERRORF( bIsNotEof == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_M4OBJ, ai_pccM4ObjId ) ;


// Ejecutamos la carga de nomina ==============================================

    iResult = poRegister->Item[ M4MDFAC_T3S_CREATION_TYPE ].Value.Get( vValue ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		iType = M4CL_CHANNEL_CREATION_TYPE_NONE ;
	}
	else
	{
		iType = m4uint32_t( vValue.Data.DoubleData ) ;
	}

	if( iType == M4CL_CHANNEL_CREATION_TYPE_PAYROLL )
	{
		poRegister->Item[ M4MDFAC_T3S_LOAD_PAYROLL ] ;
		CHECK_CHLOG_ERRORF( poRegister->Item.GetIndex() == M4DM_EOF_INDEX, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

		iResult = poRegister->Item.Call() ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

		iResult = poRegister->Item.StackPop( vValue ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS || vValue.Type != M4CL_CPP_TYPE_NUMBER || vValue.Data.DoubleData != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;
	}


// Ejecutamos la carga de lenguajes si es dinámico ============================

	if( m_iIsSystemBuilt == 0 )
	{
		iResult = _CreateSystemAccess() ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;
			return( M4_ERROR ) ;
		}
	}

	poCMCR = m_poSystemAccess->GetpCMCR() ;

	iResult = poRegister->Item[ M4MDFAC_T3S_META_LANGUAGE ].Value.Get( vValue ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
	{
		iType = M4CL_LANGUAGE_TYPE_LOGON ;
	}
	else
	{
		if( vValue.Data.DoubleData == 1.0 )
		{
			iType = M4CL_LANGUAGE_TYPE_LOGON ;
		}
		else
		{
			iType = M4CL_LANGUAGE_TYPE_DEVELOPMENT ;
		}
	}

	if( iType == M4CL_LANGUAGE_TYPE_LOGON )
	{
		vValue.Data.DoubleData = m_poLogonAdaptor->GetSessionLanguage( ai_pccM4ObjId, ai_roBP.m_iLanguage ) ;
	}
	else
	{
		vValue.Data.DoubleData = m_poLogonAdaptor->GetSessionDevLanguage( ai_pccM4ObjId, ai_roBP.m_iLanguage ) ;
	}

	if( vValue.Data.DoubleData >= 9 )
	{
		iResult = poRegister->Item[ M4MDFAC_T3S_ID_LANGUAGE ].Value.Set( vValue ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

		poRegister->Item[ M4MDFAC_T3S_LOAD_LANGUAGE ] ;
		CHECK_CHLOG_ERRORF( poRegister->Item.GetIndex() == M4DM_EOF_INDEX, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

		iResult = poRegister->Item.Call() ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;

		iResult = poRegister->Item.StackPop( vValue ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS || vValue.Type != M4CL_CPP_TYPE_NUMBER || vValue.Data.DoubleData != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_M4OBJ, ai_pccM4ObjId << M4ClCsType( ai_roBP.m_iCsType ) ) ;
	}


// Creamos el metacanal =======================================================

	iResult = MDFac_BuildFromAccess( m_poMetaAccess, ai_roBP.m_iCsType, ai_roBP.m_iLanguage, 0, M4_TRUE, ai_poCMCR ) ;


// ============================================================================
// Optimización para generar el metacanal back a la vez que el front 10-01-2001
// ============================================================================

	m4uint8_t		iLanguage ;
	m4uint16_t		iVerCounterBack ;
	ClCMCRWrapper	*poCMCRWrapBack = NULL ;
	ClCompiledMCR	*poCMCRBack = NULL ;


	if( iResult == M4_SUCCESS )
	{
		// Sólo si estamos construyendo el front
		if( ai_roBP.m_iCsType == M4CL_CSTYPE_FRONT )
		{
			// Sólo si hay cachés, porque sino no sirve de nada
			if( m_poMDCache != NULL )
			{
				// Sólo si es cacheable de metadatos
				if( ai_poCMCR->GetChannelIsMDCacheable() != M4CL_CACHEABLE_FALSE )
				{
					iLanguage = ai_poCMCR->GetChannelLanguage() ;

					// Sólo si no está ya en la caché
					iResult = m_poMDCache->GetObject( M4CL_CSTYPE_BACK, ai_pccM4ObjId, iLanguage, ai_roStartDate, ai_roEndDate, ai_roCorDate, iVerCounterBack, poCMCRWrapBack ) ;

					if( iResult == M4_SUCCESS )
					{
						if( poCMCRWrapBack != NULL )
						{
							poCMCRWrapBack->Destroy() ;
							poCMCRWrapBack = NULL ;
						}
					}
					else
					{
						poCMCRBack = new ClCompiledMCR() ;
						CHECK_CHLOG_ERRORF( poCMCRBack == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCompiledMCR ) ) << __LINE__ << __FILE__ ) ;

						iResult = MDFac_BuildFromAccess( m_poMetaAccess, M4CL_CSTYPE_BACK, iLanguage, 0, M4_TRUE, poCMCRBack ) ;

						if( iResult == M4_SUCCESS )
						{
							//Hacemos el Put de la caché.
							poCMCRWrapBack = poCMCRBack->GetCMCRWrapper();
							iResult = m_poMDCache->PutObject( M4CL_CSTYPE_BACK, ai_pccM4ObjId, iLanguage, iVerCounterBack, poCMCRWrapBack ) ;
						}

						poCMCRBack->Destroy() ;
					}
				}
			}
		}
	}

// ============================================================================
// Fin de optimización
// ============================================================================

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	ao_rpoCMCRWrap = ai_poCMCR->GetCMCRWrapper() ;

    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryServer :: p_DelegateMapFromId( m4pcchar_t ai_pccMapId, ClMappings* &ao_rpoMappings )
{

	m4return_t	iResult = M4_ERROR ;


	ao_rpoMappings = new ClMappings() ;
	CHECK_CHLOG_ERRORF( ao_rpoMappings == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClMappings ) ) << __LINE__ << __FILE__ ) ;

	iResult = ao_rpoMappings->Init( ai_pccMapId, m_poChannelManager ) ;

	if( iResult != M4_SUCCESS )
	{
		delete( ao_rpoMappings ) ;
		ao_rpoMappings = NULL ;
		return( M4_ERROR ) ;
	}

    return( M4_SUCCESS ) ;
}
