
//==============================================================================
//
// (c) Copyright  1991-2005 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clmappings.cpp   
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
//    This module defines the functions for the mappings for objects
//
//
//==============================================================================



#include "clmappings.hpp"

#include "m4facres.hpp"
#include "m4facdef.hpp"
#include "m4mdrt.hpp"
#include "m4objglb.hpp"
#include "node_knl.hpp"
#include "access.hpp"
#include "channel.hpp"
#include "dm.hpp"
#include "clgeniod.hpp"
#include "clmapcac.hpp"



//=================================================================================
// Defines
//=================================================================================

#define M4_CH_MAGIC_NUMBER_MAP							0x4A90F283
#define M4_MAP_SERIALIZE_VERSION						2





//=================================================================================
// Funciones auxiliares globales
//=================================================================================

m4return_t	M4MapReadString( m4pcchar_t &ao_rpccValue, m4pcchar_t ai_pccDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister )
{

	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		iHandle = 0 ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR = NULL ;


	ao_rpccValue = ai_pccDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItem << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << "Mappings" ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_STRING_VAR )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) << M4ClCppType( vValue.Type ) << "Mappings" ) ;
			return( M4_ERROR ) ;
		}

		ao_rpccValue = vValue.Data.PointerChar ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4MapReadDate( m4date_t &ao_rdValue, m4date_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItem, ClRegister *ai_poRegister )
{

	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		iHandle = 0 ;
    m4char_t		acDefault[ M4_SECOND_END + 1 ] ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR = NULL ;


	ao_rdValue = ai_dDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItem << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << "Mappings" ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_DATE )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( vValue.Type ) << "Mappings" ) ;
			return( M4_ERROR ) ;
		}

		ao_rdValue = vValue.Data.DoubleData ;

// sólo si hemos leido valor

		if( ao_rdValue < M4CL_MINUS_INFINITE_IN_JULIAN )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MIM_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				ClDateToString( ai_dDefault, acDefault, M4_SECOND_END + 1 ) ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MIM_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4CL_MINUS_INFINITE_DATE << acDefault << "Mappings" ) ;
			}

			ao_rdValue = ai_dDefault ;
		}

		if( ao_rdValue > M4CL_PLUS_INFINITE_IN_JULIAN )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MAX_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				ClDateToString( ai_dDefault, acDefault, M4_SECOND_END + 1 ) ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MAX_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4CL_PLUS_INFINITE_DATE << acDefault << "Mappings" ) ;
			}

			ao_rdValue = ai_dDefault ;
		}
	}

	return( M4_SUCCESS ) ;
}






//=================================================================================
//
// ClOneMap
//
// Clase que implementa la definición de un mapeo
//
//=================================================================================


// Funciones de inicialización ================================================
	
ClOneMap :: ClOneMap( void )
{
	m_iSource = 0 ;
	m_iDestination = 0 ;
	m_poPool = NULL ;
}



// Funciones de serialización =================================================

m4return_t	ClOneMap :: Serialize( ClGenericIODriver& IOD ) const
{

	m4return_t	iResult = M4_SUCCESS ;


	iResult = IOD.Write( m_iSource ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Write( m_iDestination ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClOneMap :: DeSerialize( ClGenericIODriver& IOD, ClStaticPoolStr *ao_poPool )
{

	m4return_t	iResult = M4_SUCCESS ;


	m_poPool = ao_poPool ;

	iResult = IOD.Read( m_iSource ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Read( m_iDestination ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}




//=================================================================================
//
// ClMapGroup
//
// Clase que implementa la definición de un grupo de mapeos
//
//=================================================================================


// Funciones de inicialización ================================================
	

ClMapGroup :: ClMapGroup( void )
{
	m_poMappingsArray = NULL ;
	m_poComparisonOneMap = &m_oComparisonOneMap ;
}


ClMapGroup :: ~ClMapGroup( void )
{
	if( m_poMappingsArray != NULL )
	{
		delete [] m_poMappingsArray ;
		m_poMappingsArray = NULL ;
	}
}


m4return_t	ClMapGroup :: Init( ClAccessRecordSet* ai_poRecordset, ClStaticPoolStr *ao_poPool )
{

	m4return_t		iResult = M4_SUCCESS ;
	m4uint16_t		i = 0 ;
	m4uint16_t		iLength = 0 ;
	m4pcchar_t		pccValue = NULL ;
	ClOneMap		*poOneMap = NULL ;
	ClRegister		*poRegister = NULL ;
	ClItemIndex		iSource = -1 ;
	ClItemIndex		iDestination = -1 ;


	// No puede haber más de 65536 elementos
	iLength = m4uint16_t( ai_poRecordset->Count() ) ;

	if( iLength > 0 )
	{
		m_poMappingsArray = new ClOneMap[ iLength ] ;
		CHECK_CHLOG_ERRORF( m_poMappingsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClOneMap ) * iLength ) << __LINE__ << __FILE__ ) ;

		m_oMappings.SetDelta( iLength ) ;

		iSource = ai_poRecordset->Register.Item[ M4MDFAC_NODE_MAP_SOURCE ].GetIndex() ;
		iDestination = ai_poRecordset->Register.Item[ M4MDFAC_NODE_MAP_DESTINATION ].GetIndex() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( ai_poRecordset->Register[ i ] ) ;
			poOneMap = &( m_poMappingsArray[ i ] ) ;
			poOneMap->SetUpperPool( ao_poPool ) ;

			iResult = M4MapReadString( pccValue, "", iSource, M4MDFAC_NODE_MAP_SOURCE, poRegister ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iResult = poOneMap->SetSource( pccValue ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = M4MapReadString( pccValue, "", iDestination, M4MDFAC_NODE_MAP_DESTINATION, poRegister ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iResult = poOneMap->SetDestination( pccValue ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = m_oMappings.AddElement( poOneMap ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		m_oMappings.Sort( M4ClCompareObjectsById ) ;
	}

	return( M4_SUCCESS ) ;
}



// Otras funciones de lectura ================================================

m4bool_t	ClMapGroup :: AreEqual( ClMapGroup& ai_roMapGroup )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4uint16_t	i = 0 ;
	m4uint16_t	iLength1 = 0 ;
	m4uint16_t	iLength2 = 0 ;
	m4pvoid_t	pvElement = NULL ;
	ClOneMap	*poOneMap1 = NULL ;
	ClOneMap	*poOneMap2 = NULL ;


	iLength1 = m_oMappings.GetLength() ;
	iLength2 = ai_roMapGroup.m_oMappings.GetLength() ;

	if( iLength1 != iLength2 )
	{
		return( M4_FALSE ) ;
	}

	for( i = 0 ; i < iLength1 ; i++ )
	{
		iResult = m_oMappings.GetElement( i, pvElement ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvElement == NULL, M4_FALSE, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poOneMap1 = ( ClOneMap* ) pvElement ;

		iResult = ai_roMapGroup.m_oMappings.GetElement( i, pvElement ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvElement == NULL, M4_FALSE, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poOneMap2 = ( ClOneMap* ) pvElement ;

		if( poOneMap1->AreEqual( *poOneMap2 ) == M4_FALSE )
		{
			return( M4_FALSE ) ;
		}
	}

	return( M4_TRUE ) ;
}


m4return_t	ClMapGroup :: GetMappingByPosition( m4uint16_t ai_iPosition, m4pcchar_t& ao_rpccSource, m4pcchar_t& ao_rpccDestination )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4pvoid_t	pvElement = NULL ;
	ClOneMap	*poOneMap = NULL ;


	ao_rpccSource = NULL ;
	ao_rpccDestination = NULL ;

	iResult = m_oMappings.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS || pvElement == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poOneMap = ( ClOneMap* ) pvElement ;

	ao_rpccSource = poOneMap->GetSource() ;
	ao_rpccDestination = poOneMap->GetDestination() ;

	return( M4_SUCCESS ) ;
}


m4pcchar_t	ClMapGroup :: GetMappingById( m4pcchar_t ai_pccSource, m4bool_t ai_bConvert )
{

	m4pcchar_t	pccResult = NULL ;
	ClOneMap	**ppoResult = NULL ;


	if( ai_pccSource == NULL )
	{
		ai_pccSource = "" ;
	}

	m_oComparisonOneMap.SetIdentifier( ai_pccSource, ai_bConvert ) ;

	ppoResult = ( ClOneMap** ) m_oMappings.Find( M4ClCompareObjectsById, &m_poComparisonOneMap ) ;

	if( ppoResult == NULL )
	{
		return( NULL ) ;
	}

	pccResult = (*ppoResult)->GetDestination() ;

	return( pccResult ) ;
}


m4uint32_t ClMapGroup :: GetSize( void )
{

	m4uint32_t	iSize = 0 ;
	m4uint32_t	iStatic = 0 ;
	m4uint32_t	iPointers = 0 ;


	m_oMappings.GetTotalSize( iStatic, iPointers ) ;

	iSize += iPointers ;
	iSize += ( m_oMappings.GetLength() * sizeof( ClOneMap ) ) ;
	iSize += sizeof( ClMapGroup ) ;

	return( iSize ) ;
}




// Funciones de serialización =================================================

m4return_t	ClMapGroup :: Serialize( ClGenericIODriver& IOD ) const
{
	
	m4return_t	iResult = M4_SUCCESS ;
	m4uint16_t	i = 0 ;
	m4uint16_t	iLength = 0 ;
	m4pvoid_t	pvElement = NULL ;
	ClOneMap	*poOneMap = NULL ;


	iLength = m_oMappings.GetLength() ;

	iResult = IOD.Write( iLength ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( iLength > 0 )
	{
		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oMappings.GetElement( i, pvElement ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvElement == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poOneMap = ( ClOneMap* ) pvElement ;

			iResult = poOneMap->Serialize( IOD ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClMapGroup :: DeSerialize( ClGenericIODriver& IOD, ClStaticPoolStr *ao_poPool )
{
	
	m4return_t	iResult = M4_SUCCESS ;
	m4uint16_t	i = 0 ;
	m4uint16_t	iLength = 0 ;
	ClOneMap	*poOneMap = NULL ;


	iResult = IOD.Read( iLength ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( iLength > 0 )
	{
		m_poMappingsArray = new ClOneMap[ iLength ] ;
		CHECK_CHLOG_ERRORF( m_poMappingsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClOneMap ) * iLength ) << __LINE__ << __FILE__ ) ;

		m_oMappings.SetDelta( iLength ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poOneMap = &( m_poMappingsArray[ i ] ) ;

			iResult = poOneMap->DeSerialize( IOD, ao_poPool ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iResult = m_oMappings.AddElement( poOneMap ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}
	}

	return( M4_SUCCESS ) ;
}




//=================================================================================
//
// ClOrgMaps
//
// Clase que implementa la definición de los mapeos de una sociedad
//
//=================================================================================


// Funciones de inicialización ================================================
	
ClOrgMaps :: ClOrgMaps( void )
{
	m_iOrganization = 0 ;
	m_iMapGroupPosition = -1 ;
	m_poPool = NULL ;
	m_poMapGroups = NULL ;
}



// Funciones de lectura =======================================================

ClMapGroup*  ClOrgMaps :: GetMapGroup( void ) const
{

	m4return_t	iResult = M4_SUCCESS ;
	m4pvoid_t	pvElement = NULL ;


	if( m_iMapGroupPosition < 0 )
	{
		return NULL ;
	}

	iResult = m_poMapGroups->GetElement( m_iMapGroupPosition, pvElement ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS || pvElement == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( ( ClMapGroup* ) pvElement ) ;
}



// Funciones de serialización =================================================

m4return_t	ClOrgMaps :: Serialize( ClGenericIODriver& IOD ) const
{

	m4return_t	iResult = M4_SUCCESS ;


	iResult = IOD.Write( m_iOrganization ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Write( m_iMapGroupPosition ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClOrgMaps :: DeSerialize( ClGenericIODriver& IOD, ClStaticPoolStr *ao_poPool, ClPointerStack *ai_poMapGroups )
{

	m4return_t	iResult = M4_SUCCESS ;

	m_poPool = ao_poPool ;
	m_poMapGroups = ai_poMapGroups ;

	iResult = IOD.Read( m_iOrganization ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Read( m_iMapGroupPosition ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}





//=================================================================================
//
// ClMappings
//
// Clase que implementa la definición de los mapeos de un objeto
//
//=================================================================================


// Funciones de inicialización ================================================
	
ClMappings :: ClMappings( void )

:	ClPersistiableObject( NULL ),
	m_oPool( M4CL_MAP_UPPER_POOL_SIZE, M4CL_UPPERCASE )

{
	m_iSize = 0 ;
	m_dVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m_poOrgMappingsArray = NULL ;

	m_poComparisonOrgMapping = &m_oComparisonOrgMapping ;
	m_pCO = NULL ;
}


ClMappings :: ~ClMappings( void )
{
	m4return_t	iResult = M4_SUCCESS ;
	m4uint16_t	i = 0 ;
	m4uint16_t	iLength = 0 ;
	m4pvoid_t	pvElement = NULL ;
	ClMapGroup	*poMapGroup = NULL ;


	if( m_poOrgMappingsArray != NULL )
	{
		delete [] m_poOrgMappingsArray ;
		m_poOrgMappingsArray = NULL ;
	}

	iLength = m_oMapGroups.GetLength() ;

	if( iLength > 0 )
	{
		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oMapGroups.GetElement( i, pvElement ) ;

			if( iResult != M4_SUCCESS || pvElement == NULL )
			{
				continue ;
			}

			poMapGroup = ( ClMapGroup* ) pvElement ;
			delete( poMapGroup ) ;
		}
	}

	m_pCO = NULL ;
}



m4return_t	ClMappings :: Init( m4pcchar_t ai_pccMapId, ClChannelManager *ai_poChannelManager )
{

	m4return_t			iResult = M4_SUCCESS ;
	m4pchar_t			pcOrganization = NULL ;
	m4VariantType		vValue ;
	ClChannel			*poChannel = NULL ;
	ClAccess			*poAccess = NULL ;
    ClNode				*poOrgNode = NULL ;
	ClAccessRecordSet	*poOrgRecordset = NULL ;


	CHECK_CHLOG_DEBUGF( ai_poChannelManager == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( ai_pccMapId == NULL )
	{
		ai_pccMapId = "" ;
	}

    iResult = ai_poChannelManager->CreateChannel( poChannel ) ;

	if( iResult != M4_SUCCESS || poChannel == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = poChannel->ApplyMappings.Set( 0, M4CL_CPP_TYPE_NUMBER ) ;

    if( iResult != M4_SUCCESS )
    {
		poChannel->Destroy() ;
		return( M4_ERROR ) ;
    }

	poChannel->Role_ID.Set( M4CL_SEC_SUPER_USER ) ;

	pcOrganization = (m4pchar_t)strchr( ai_pccMapId, '@' ) ;

	if( pcOrganization != NULL )
	{
		*pcOrganization = '\0' ;
	}

	iResult = poChannel->BuildFromIdNoSec( ai_pccMapId ) ;

	if( pcOrganization != NULL )
	{
		*pcOrganization = '@' ;
		pcOrganization++ ;
	}

    if( iResult != M4_SUCCESS )
    {
		poChannel->Destroy() ;
		return( M4_ERROR ) ;
    }

	poAccess = poChannel->CreateAccess() ;

	if( poAccess == NULL )
	{
		poChannel->Destroy() ;
		return( M4_ERROR ) ;
	}

	if( pcOrganization != NULL )
	{
		poOrgNode = &( poAccess->Node[ M4MDFAC_NODE_MAP_ORGANIZATIONS ] ) ;
		CHECK_CHLOG_DEBUGF( poOrgNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_MAP_ORGANIZATIONS << ai_pccMapId << "Mappings" ) ;

		poOrgRecordset = &( poOrgNode->RecordSet ) ;

		vValue.SetStringRef( pcOrganization ) ;
		poOrgRecordset->Item[ M4MDFAC_ITEM_MAP_ID_ORGANIZATION_FILTER ].Value.Set( vValue ) ;

		vValue.SetStringRef( "FROM &SDM_OS_PROJECTS A WHERE A.ID_ORGANIZATION = @ID_ORGANIZATION_FILTER" ) ;
		poOrgRecordset->Item[ M4MDFAC_ITEM_MAP_SYS_SENTENCE ].Value.Set( vValue ) ;
	}

	iResult = _Init( ai_pccMapId, poAccess ) ;

	poAccess->Destroy() ;
	poChannel->Destroy() ;

	return( iResult ) ;
}


void ClMappings :: Destroy( m4bool_t ai_bRemoveFromCache )
{
    if( m_pCO != NULL )
    {
		if( ai_bRemoveFromCache == M4_TRUE )
		{
			( (ClMapContainer*) m_pCO )->RemoveObjectFromCache() ;
		}

		( (ClMapContainer*) m_pCO )->FreeObjectFromCache() ;
    }
	else
	{
		delete( this ) ;
	}
}


// Funciones de lectura =======================================================

m4pcchar_t	ClMappings :: GetMappingById( m4pcchar_t ai_pccOrganization, m4pcchar_t ai_pccSource, m4bool_t ai_bConvert )
{

	m4pcchar_t	pccResult = NULL ;
	ClOrgMaps	**ppoResult = NULL ;
	ClMapGroup	*poMapGroup = NULL ;


	if( ai_pccOrganization == NULL )
	{
		ai_pccOrganization = "" ;
	}

	m_oComparisonOrgMapping.SetIdentifier( ai_pccOrganization, ai_bConvert ) ;

	ppoResult = ( ClOrgMaps** ) m_oOrgMappings.Find( M4ClCompareObjectsById, &m_poComparisonOrgMapping ) ;

	if( ppoResult == NULL )
	{
		return( NULL ) ;
	}

	poMapGroup = (*ppoResult)->GetMapGroup() ;

	if( poMapGroup == NULL )
	{
		return( NULL ) ;
	}

	pccResult = poMapGroup->GetMappingById( ai_pccSource, ai_bConvert ) ;

	return( pccResult ) ;
}


m4return_t	ClMappings :: IsEquivalentOrganization( ClMappings* ai_poMappings1, ClMappings* ai_poMappings2, m4pcchar_t ai_pccOrganization1, m4pcchar_t ai_pccOrganization2, m4pcchar_t ai_pccDestination, m4bool_t ai_bConvert, m4bool_t& ao_rbIsEquivalent )
{

	m4return_t			iResult = M4_SUCCESS ;
	m4bool_t			bFound = M4_FALSE ;
	m4uint16_t			i = 0 ;
	m4uint16_t			iLength = 0 ;
	m4pcchar_t			pccSource = NULL ;
	m4pcchar_t			pccDestination1 = NULL ;
	m4pcchar_t			pccDestination2 = NULL ;
	ClOrgMaps			**ppoOrgMappings1 = NULL ;
	ClOrgMaps			**ppoOrgMappings2 = NULL ;
	ClMapGroup			*poMapGroup1 = NULL ;
	ClMapGroup			*poMapGroup2 = NULL ;
	ClCompIdentifier	oOrganization1 ;
	ClCompIdentifier	oOrganization2 ;
	ClCompIdentifier	oDestination ;
	ClCompIdentifier	*poOrganization1 = &oOrganization1 ;
	ClCompIdentifier	*poOrganization2 = &oOrganization2 ;
	ClCompIdentifier	*poDestination = &oDestination ;


	ao_rbIsEquivalent = M4_FALSE ;

	if( ai_pccOrganization1 == NULL )
	{
		ai_pccOrganization1 = "" ;
	}

	if( ai_pccOrganization2 == NULL )
	{
		ai_pccOrganization2 = "" ;
	}

	if( ai_pccDestination == NULL )
	{
		ai_pccDestination = "" ;
	}

	oOrganization1.SetIdentifier( ai_pccOrganization1, ai_bConvert ) ;
	oOrganization2.SetIdentifier( ai_pccOrganization2, ai_bConvert ) ;

	if( M4MdFacStrcmp( oOrganization1.GetIdentifier(), oOrganization2.GetIdentifier() ) == 0 )
	{
		ao_rbIsEquivalent = M4_TRUE ;
		return( M4_SUCCESS ) ;
	}

	oDestination.SetIdentifier( ai_pccDestination, ai_bConvert ) ;

	
	ppoOrgMappings1 = ( ClOrgMaps** ) ai_poMappings1->m_oOrgMappings.Find( M4ClCompareObjectsById, &poOrganization1 ) ;
	ppoOrgMappings2 = ( ClOrgMaps** ) ai_poMappings2->m_oOrgMappings.Find( M4ClCompareObjectsById, &poOrganization2 ) ;

	if( ppoOrgMappings1 != NULL )
	{
		poMapGroup1 = (*ppoOrgMappings1)->GetMapGroup() ;
	}

	if( ppoOrgMappings2 != NULL )
	{
		poMapGroup2 = (*ppoOrgMappings2)->GetMapGroup() ;
	}

	iLength = 0 ;
	if( poMapGroup1 != NULL )
	{
		/* Bug 0106737
		También hay que comprobar que en las 2 sociedades el destino se mapee a lo mismo
		*/
		pccDestination1 = poMapGroup1->GetMappingById( oDestination.GetIdentifier(), M4_FALSE ) ;

		if( pccDestination1 != NULL )
		{
			bFound = M4_FALSE ;

			if( poMapGroup2 != NULL )
			{
				pccDestination2 = poMapGroup2->GetMappingById( oDestination.GetIdentifier(), M4_FALSE ) ;

				if( pccDestination2 != NULL && M4MdFacStrcmp( pccDestination2, pccDestination1 ) == 0 )
				{
					bFound = M4_TRUE ;
				}
			}

			if( bFound == M4_FALSE )
			{
				ao_rbIsEquivalent = M4_FALSE ;
				return( M4_SUCCESS ) ;
			}
		}

		iLength = poMapGroup1->GetNumberOfMappings() ;
	}

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = poMapGroup1->GetMappingByPosition( i, pccSource, pccDestination1 ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		if( M4MdFacStrcmp( pccDestination1, oDestination.GetIdentifier() ) == 0 )
		{
			bFound = M4_FALSE ;

			if( poMapGroup2 != NULL )
			{
				pccDestination2 = poMapGroup2->GetMappingById( pccSource, M4_FALSE ) ;

				if( pccDestination2 != NULL && M4MdFacStrcmp( pccDestination2, pccDestination1 ) == 0 )
				{
					bFound = M4_TRUE ;
				}
			}

			if( bFound == M4_FALSE )
			{
				ao_rbIsEquivalent = M4_FALSE ;
				return( M4_SUCCESS ) ;
			}
		}
	}

	iLength = 0 ;
	if( poMapGroup2 != NULL )
	{
		/* Bug 0106737
		También hay que comprobar que en las 2 sociedades el destino se mapee a lo mismo
		*/
		pccDestination1 = poMapGroup2->GetMappingById( oDestination.GetIdentifier(), M4_FALSE ) ;

		if( pccDestination1 != NULL )
		{
			bFound = M4_FALSE ;

			if( poMapGroup1 != NULL )
			{
				pccDestination2 = poMapGroup1->GetMappingById( oDestination.GetIdentifier(), M4_FALSE ) ;

				if( pccDestination2 != NULL && M4MdFacStrcmp( pccDestination2, pccDestination1 ) == 0 )
				{
					bFound = M4_TRUE ;
				}
			}

			if( bFound == M4_FALSE )
			{
				ao_rbIsEquivalent = M4_FALSE ;
				return( M4_SUCCESS ) ;
			}
		}

		iLength = poMapGroup2->GetNumberOfMappings() ;
	}

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = poMapGroup2->GetMappingByPosition( i, pccSource, pccDestination1 ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		if( M4MdFacStrcmp( pccDestination1, oDestination.GetIdentifier() ) == 0 )
		{
			bFound = M4_FALSE ;

			if( poMapGroup1 != NULL )
			{
				pccDestination2 = poMapGroup1->GetMappingById( pccSource, M4_FALSE ) ;

				if( pccDestination2 != NULL && M4MdFacStrcmp( pccDestination2, pccDestination1 ) == 0 )
				{
					bFound = M4_TRUE ;
				}
			}

			if( bFound == M4_FALSE )
			{
				ao_rbIsEquivalent = M4_FALSE ;
				return( M4_SUCCESS ) ;
			}
		}
	}

	ao_rbIsEquivalent = M4_TRUE ;

	return( M4_SUCCESS ) ;
}



// Funciones de cache =========================================================

void ClMappings :: RemoveFromCache( void )
{
    if( m_pCO != NULL )
    {
		( (ClMapContainer*) m_pCO )->RemoveObjectFromCache() ;
	}
}



// Funciones de ClPersistiableObject ==========================================

m4return_t	ClMappings :: Serialize( ClGenericIODriver& IOD )
{
	
	m4return_t	iResult = M4_SUCCESS ;
	m4uint16_t	i = 0 ;
	m4uint16_t	iLength = 0 ;
	m4pvoid_t	pvElement = NULL ;
	ClOrgMaps	*poOrgMaps = NULL ;
	ClMapGroup	*poMapGroup = NULL ;


	iResult = IOD.Write( m4uint32_t( M4_CH_MAGIC_NUMBER_MAP ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Write( m4uint32_t( M4_MAP_SERIALIZE_VERSION ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Write( m_iSize ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Write( m_dVersion ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iLength = m_oOrgMappings.GetLength() ;

	iResult = IOD.Write( iLength ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( iLength > 0 )
	{
		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oOrgMappings.GetElement( i, pvElement ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvElement == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poOrgMaps = ( ClOrgMaps* ) pvElement ;

			iResult = poOrgMaps->Serialize( IOD ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	iLength = m_oMapGroups.GetLength() ;

	iResult = IOD.Write( iLength ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( iLength > 0 )
	{
		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oMapGroups.GetElement( i, pvElement ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvElement == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poMapGroup = ( ClMapGroup* ) pvElement ;

			iResult = poMapGroup->Serialize( IOD ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	iResult = m_oPool.Serialize( IOD ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClMappings :: DeSerialize( ClGenericIODriver& IOD )
{
	
	m4return_t	iResult = M4_SUCCESS ;
	m4uint16_t	i = 0 ;
	m4uint16_t	iLength = 0 ;
	m4uint32_t	iMagicNumber = 0 ;
	m4uint32_t	iSerializeVersion = 0 ;
	ClOrgMaps	*poOrgMaps = NULL ;
	ClMapGroup	*poMapGroup = NULL ;


	iResult = IOD.Read( iMagicNumber ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( iMagicNumber != M4_CH_MAGIC_NUMBER_MAP, M4_ERROR, M4_MDRT_BAD_MAP_MAGIC_NUMBER, iMagicNumber << m4uint32_t( M4_CH_MAGIC_NUMBER_MAP ) ) ;

	iResult = IOD.Read( iSerializeVersion ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_WARNINGF( iSerializeVersion != M4_MAP_SERIALIZE_VERSION, M4_ERROR, M4_MDRT_BAD_MAP_VERSION, iSerializeVersion << m4uint32_t( M4_MAP_SERIALIZE_VERSION ) ) ;

	iResult = IOD.Read( m_iSize ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Read( m_dVersion ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Read( iLength ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( iLength > 0 )
	{
		m_poOrgMappingsArray = new ClOrgMaps[ iLength ] ;
		CHECK_CHLOG_ERRORF( m_poOrgMappingsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClOrgMaps ) * iLength ) << __LINE__ << __FILE__ ) ;

		m_oOrgMappings.SetDelta( iLength ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poOrgMaps = &( m_poOrgMappingsArray[ i ] ) ;

			iResult = poOrgMaps->DeSerialize( IOD, &m_oPool, &m_oMapGroups ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iResult = m_oOrgMappings.AddElement( poOrgMaps ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}
	}

	iResult = IOD.Read( iLength ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( iLength > 0 )
	{
		m_oMapGroups.SetDelta( iLength ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poMapGroup = new ClMapGroup ;
			CHECK_CHLOG_ERRORF( poMapGroup == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClMapGroup ) ) << __LINE__ << __FILE__ ) ;

			iResult = poMapGroup->DeSerialize( IOD, &m_oPool ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iResult = m_oMapGroups.AddElement( poMapGroup ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}
	}

	iResult = m_oPool.DeSerialize( IOD ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// Funciones privadas =========================================================

m4return_t	ClMappings :: _Init( m4pcchar_t ai_pccMapId, ClAccess *ai_poAccess )
{

	m4return_t			iResult = M4_SUCCESS ;
	m4bool_t			bFound = M4_FALSE ;
	m4uint16_t			i = 0 ;
	m4uint16_t			j = 0 ;
	m4uint16_t			iLength = 0 ;
	m4uint16_t			iMapGroups = 0 ;
	m4uint32_t			iStatic = 0 ;
	m4uint32_t			iPool =  0 ;
	m4uint32_t			iPointers = 0 ;
	m4pcchar_t			pccValue = NULL ;
	m4pvoid_t			pvElement = NULL ;
	ClMapGroup			*poMapGroup = NULL ;
	ClOrgMaps			*poOrgMaps = NULL ;
    ClNode				*poVersionNode = NULL ;
    ClNode				*poOrgNode = NULL ;
    ClNode				*poMapNode = NULL ;
	ClAccessRecordSet	*poVersionRecordset = NULL ;
	ClAccessRecordSet	*poOrgRecordset = NULL ;
	ClAccessRecordSet	*poMapRecordset = NULL ;
	ClRegister			*poRegister = NULL ;
	ClItemIndex			iDate = -1 ;
	ClItemIndex			iOrganization = -1 ;


	iResult = ai_poAccess->Load() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	poVersionNode = &( ai_poAccess->Node[ M4MDFAC_NODE_MAP_VERSION ] ) ;
	CHECK_CHLOG_DEBUGF( poVersionNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_MAP_VERSION << ai_pccMapId << "Mappings" ) ;

	poVersionRecordset = &( poVersionNode->RecordSet ) ;

	poOrgNode = &( ai_poAccess->Node[ M4MDFAC_NODE_MAP_ORGANIZATIONS ] ) ;
	CHECK_CHLOG_DEBUGF( poOrgNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_MAP_ORGANIZATIONS << ai_pccMapId << "Mappings" ) ;

	poOrgRecordset = &( poOrgNode->RecordSet ) ;

	poMapNode = &( ai_poAccess->Node[ M4MDFAC_NODE_MAP_MAPPINGS ] ) ;
	CHECK_CHLOG_DEBUGF( poMapNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_MAP_MAPPINGS << ai_pccMapId << "Mappings" ) ;

	poMapRecordset = &( poMapNode->RecordSet ) ;


	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poVersionRecordset->Count() ) ;

	if( iLength == 0 )
	{
		m_dVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;
	}
	else
	{
		iDate = poVersionRecordset->Register.Item[ M4MDFAC_ITEM_MAP_DT_MD_VERSION ].GetIndex() ;

		poRegister = &( poVersionRecordset->Register[ 0 ] ) ;

		iResult = M4MapReadDate( m_dVersion, M4CL_MINUS_INFINITE_IN_JULIAN, iDate, M4MDFAC_ITEM_MAP_DT_MD_VERSION, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


	// No puede haber más de 65536 elementos
    iLength = m4uint16_t( poOrgRecordset->Count() ) ;

	if( iLength > 0 )
	{
		m_poOrgMappingsArray = new ClOrgMaps[ iLength ] ;
		CHECK_CHLOG_ERRORF( m_poOrgMappingsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClOrgMaps ) * iLength ) << __LINE__ << __FILE__ ) ;

		m_oOrgMappings.SetDelta( iLength ) ;
		m_oMapGroups.SetDelta( iLength ) ;

		iOrganization = poOrgRecordset->Register.Item[ M4MDFAC_ITEM_MAP_ID_ORGANIZATION ].GetIndex() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poOrgRecordset->Current.MoveTo( i ) ;
			poRegister = &( poOrgRecordset->Register[ i ] ) ;
			poOrgMaps = &( m_poOrgMappingsArray[ i ] ) ;

			poOrgMaps->SetUpperPool( &m_oPool ) ;
			poOrgMaps->SetMapGroups( &m_oMapGroups ) ;

			iResult = M4MapReadString( pccValue, "", iOrganization, M4MDFAC_ITEM_MAP_ID_ORGANIZATION, poRegister ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iResult = poOrgMaps->SetOrganization( pccValue ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = m_oOrgMappings.AddElement( poOrgMaps ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			poMapGroup = new ClMapGroup ;
			CHECK_CHLOG_ERRORF( poMapGroup == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClMapGroup ) ) << __LINE__ << __FILE__ ) ;

			iResult = poMapGroup->Init( poMapRecordset, &m_oPool ) ;

			if( iResult != M4_SUCCESS )
			{
				delete( poMapGroup ) ;
				return( M4_ERROR ) ;
			}

			if( poMapGroup->GetNumberOfMappings() == 0 )
			{
				poOrgMaps->SetMapGroupPosition( -1 ) ;
				delete( poMapGroup ) ;
				continue ;
			}


			iMapGroups = m_oMapGroups.GetLength() ;

			if( iMapGroups > 0 )
			{
				bFound = M4_FALSE ;

				for( j = 0 ; j < iMapGroups ; j++ )
				{
					iResult = m_oMapGroups.GetElement( j, pvElement ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvElement == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					if( poMapGroup->AreEqual( *( ( ClMapGroup* ) pvElement ) ) == M4_TRUE )
					{
						bFound = M4_TRUE ;
						break ;
					}
				}

				if( bFound == M4_TRUE )
				{
					poOrgMaps->SetMapGroupPosition( j ) ;
					delete( poMapGroup ) ;
					continue ;
				}
			}

			poOrgMaps->SetMapGroupPosition( m_oMapGroups.GetLength() ) ;

			iResult = m_oMapGroups.AddElement( poMapGroup ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			m_iSize += poMapGroup->GetSize() ;
		}
	}

	m_oOrgMappings.Sort( M4ClCompareObjectsById ) ;

	m_oMapGroups.GetTotalSize( iStatic, iPointers ) ;
	m_iSize += iPointers ;
	m_oOrgMappings.GetTotalSize( iStatic, iPointers ) ;
	m_iSize += iPointers ;
	m_iSize += ( m_oOrgMappings.GetLength() * sizeof( ClOrgMaps ) ) ;
	m_oPool.GetTotalSize( iStatic, iPool, iPointers ) ;
	m_iSize += iPool + iPointers ;
	m_iSize += sizeof( ClMappings ) ;

	return( M4_SUCCESS ) ;
}



