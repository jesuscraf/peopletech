
//==============================================================================
//
// (c) Copyright  1991-2005 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clmapcac.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                21-06-2005
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the cache for the mappings
//
//
//==============================================================================




#include "clmapcac.hpp"
#include "m4facres.hpp"



//=================================================================================
//
// ClMapContainer
//
// Clase container de objeto cacheable de mapeos
//
//=================================================================================

// Funciones de inicialización ================================================

ClMapContainer :: ClMapContainer( ClCachePolicy *ai_poPolicy, m4uint32_t ai_iSizeObject, ClCacheDatedKey *ai_poClCacheKey )
	
: ClDatedContainer <ClMappings> ( ai_poPolicy, ai_iSizeObject, ai_poClCacheKey )

{
}


// Interfaz de ClCacheableObject ==============================================

m4bool_t	ClMapContainer :: MatchCondition( void* ai_poCondition )
{

	m4bool_t		bValue = M4_FALSE ;
	ClCOCondition	*poCondition = NULL ;


	poCondition = (ClCOCondition*) ai_poCondition ;

	if( poCondition == NULL || poCondition->m_pValue == NULL )
	{
		return( M4_FALSE ) ;
	}

	bValue = M4_FALSE ;

	switch( poCondition->m_iConditionMask )
	{
		case M4_CO_COND_CONTAINED_IN_KEY:

			//Se cumple si ai_pcPortionKey se encuentra al principio de Getm_pszKey()!!
			if( strstr( m_poClCacheKey->Getm_pszKey(), (m4pchar_t)poCondition->m_pValue ) == m_poClCacheKey->Getm_pszKey() )		
			{
				bValue = M4_TRUE ;
			}
			break ;

		case M4_CO_COND_OLDER_VERSION:

			if( m_dVersion < *(m4date_t*)( poCondition->m_pValue ) )
			{
				bValue = M4_TRUE ;
			}
			break ;

		default:
			break ;
	} ;

	if( poCondition->m_pNext != NULL )
	{
		if( ((poCondition->m_iAnd) && (M4_TRUE == bValue)) || ((!poCondition->m_iAnd) && (M4_FALSE == bValue)) )
		{
			bValue = MatchCondition( poCondition->m_pNext ) ;
		}
	}

	return( bValue ) ;
}



// Interfaz de ClDatedContainer ===============================================

ClObjectId*	ClMapContainer :: CreateObjectIdFromKey( void )
{
	ClTypedId		oTypeID( m_poClCacheKey->Getm_pszKey() ) ;
	ClUniqueKey		oUniqueKey( "" ) ;
	ClEnvironmentId	oEnvId( GetEnvId() ) ;

	ClObjectId		*poObjID = new ClObjectId( MapObjectType, &oEnvId, &oTypeID, &oUniqueKey ) ;
	CHECK_CHLOG_ERRORF( poObjID == NULL, NULL, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClObjectId ) ) << __LINE__ << __FILE__ ) ;

	return( poObjID ) ;
}





//=================================================================================
//
// ClMapCache
//
// Clase caché de objeto cacheable de mapeos
//
//=================================================================================

// Funciones de inicialización ================================================

ClMapCache :: ClMapCache( ClCache* ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod )

:	ClBaseCache<ClMappings, ClMapContainer>( ai_poCache, ai_pPath, "MapCache", ai_iMode, ai_iMaxMemory, ai_iNumObjects, ai_iRefreshRatio, ai_dMaxPeriod, ai_dMaxUnusedPeriod )

{
	static ClMapCacheableObjectFactory	*poCoFactory = new ClMapCacheableObjectFactory ;

	SetPersistor( poCoFactory ) ;
	SetCOFactory( poCoFactory ) ;
}




// Interfaz de ClBaseCacheInterface ===========================================

m4return_t ClMapCache :: RemoveObjectById( m4pchar_t ai_pccMappingId, m4puint32_t ai_piRemovedCount )
{

	m4return_t	iResult = M4_SUCCESS ;
	size_t		iLength = 0 ;
	m4pchar_t	pcMapId = NULL ;


	iLength = strlen( ai_pccMappingId ) + 2 ;
	pcMapId = new m4char_t[ iLength ] ;
	CHECK_CHLOG_ERRORF( pcMapId == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iLength ) << __LINE__ << __FILE__ ) ;

	sprintf( pcMapId, "%s@", ai_pccMappingId ) ;
	strupr( pcMapId ) ;

    //Hacemos remove de todos los objetos que tengan en su clave la cadena.
	iResult = m_poCache->RemoveAll( pcMapId, ai_piRemovedCount ) ;

	delete [] pcMapId ;

	return( iResult ) ;
}


m4return_t ClMapCache :: RemoveObjectByIdAndVersion( m4pchar_t ai_pccMappingId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType )
{

	m4return_t		iResult = M4_SUCCESS ;
	size_t			iLength = 0 ;
	m4pchar_t		pcMapId = NULL ;
	ClCOCondition	oCondId ;
	ClCOCondition	oCondVer ;


	iLength = strlen( ai_pccMappingId ) + 2 ;
	pcMapId = new m4char_t[ iLength ] ;
	CHECK_CHLOG_ERRORF( pcMapId == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iLength ) << __LINE__ << __FILE__ ) ;

	sprintf( pcMapId, "%s@", ai_pccMappingId ) ;
	strupr( pcMapId ) ;


	oCondId.m_iConditionMask = M4_CO_COND_CONTAINED_IN_KEY ;
	oCondId.m_pValue = pcMapId ;
	oCondId.m_iAnd = 1 ;
	oCondId.m_pNext = &oCondVer ;

	oCondVer.m_iConditionMask = M4_CO_COND_OLDER_VERSION ;
	oCondVer.m_pValue = &ai_dVersion ;
	oCondVer.m_iAnd = 1 ;
	oCondVer.m_pNext = 0 ;

	iResult = m_poCache->RemoveByCondition( &oCondId ) ;

	delete [] pcMapId ;	

	return( iResult ) ;
}



// Funciones de lectura escritura =============================================

m4return_t ClMapCache :: GetObject( m4pcchar_t ai_pccMappingId, ClMappings* &ao_rpoMappings )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4uint16_t	iVerCounter = 0 ;
	m4pchar_t	pcMapId = NULL ;


	pcMapId = ComposeStringKey( ai_pccMappingId ) ;
	iResult = _GetObject( pcMapId, M4CL_MINUS_INFINITE_IN_JULIAN, M4CL_PLUS_INFINITE_IN_JULIAN, ClActualDate(), iVerCounter, ao_rpoMappings ) ;
	delete [] pcMapId ;	

	return( iResult ) ;
}


m4return_t ClMapCache :: PutObject( m4pcchar_t ai_pccMappingId, ClMappings* ai_poMappings )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4pchar_t	pcMapId = NULL ;


	pcMapId = ComposeStringKey( ai_pccMappingId ) ;
	iResult = _PutObject( pcMapId, 0, ai_poMappings, 0, 0 ) ;									 
	delete [] pcMapId ;	

	return( iResult ) ;
}



// Otras funciones ============================================================

m4pchar_t ClMapCache :: ComposeStringKey( m4pcchar_t ai_pccMappingId )
{

	size_t		iLength = 0 ;
	m4pchar_t	pcMapId = NULL ;

	
	iLength = strlen( ai_pccMappingId ) + 1 ;
	pcMapId = new m4char_t[ iLength ] ;
	CHECK_CHLOG_ERRORF( pcMapId == NULL, NULL, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iLength ) << __LINE__ << __FILE__ ) ;

	sprintf( pcMapId, "%s", ai_pccMappingId ) ;
	strupr( pcMapId ) ;

	return( pcMapId ) ;
}







//=================================================================================
//
// ClMapCacheableObjectFactory
//
// Clase de la factoria de MapCacheableObject
//
//=================================================================================

// Interfaz de ClBaseCacheableObjectFactory ===================================

m4return_t	ClMapCacheableObjectFactory :: CreateObject( m4uint8_t ai_iType, ClCacheableObject* &ao_rpObject )
{
	ao_rpObject = new ClMapContainer() ;
	CHECK_CHLOG_ERRORF( ao_rpObject == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClMapContainer )  ) << __LINE__ << __FILE__ ) ;

	( (ClMapContainer*)ao_rpObject )->SetPersistor( this ) ;

	return( M4_SUCCESS ) ;
}
