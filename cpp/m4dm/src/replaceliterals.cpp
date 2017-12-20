//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           replaceliterals.cpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           15 oct 2013
// Language:        C++ 
// Operating System: WINDOWS, UNIX
// Design Document:  
//
// 
// Definition: 
//  
//    Replace literals functionality
//
//==============================================================================

#include "replaceliterals.hpp"

#include "iexecutor.hpp"
#include "access.hpp"
#include "cllgadap.hpp"
#include "m4unicode.hpp"
#include "cachedir.hpp"
#include "m4objreg.hpp"
#include "clfileiod.hpp"



//=================================================================================
// Defines
//=================================================================================

#define LITERALS_SERIALIZATION_VERSION			1



//=================================================================================
// Funciones auxiliares
//=================================================================================

void	M4DeleteString( m4pchar_t& ao_rpcDestination )
{
	if( ao_rpcDestination != NULL )
	{
		delete [] ao_rpcDestination ;
		ao_rpcDestination = NULL ;
	}
}


m4return_t	M4NewString( m4pcchar_t ai_pccSource, m4pchar_t& ao_rpcDestination )
{

	size_t	iLength = 0 ;


	M4DeleteString( ao_rpcDestination ) ;

	if( ai_pccSource != NULL )
	{
		iLength = strlen( ai_pccSource ) + 1 ;
		ao_rpcDestination = new m4char_t[ iLength ] ;
		CHECK_CHLOG_ERRORF( ao_rpcDestination == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( iLength * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;
		memcpy( ao_rpcDestination, ai_pccSource, iLength ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	DeSerializeString( ClGenericIODriver &IOD, m4pchar_t& ao_rpcDestination )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	iSize = 0 ;


	iResult = IOD.ReadSize( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize > 0 )
	{
		ao_rpcDestination = new m4char_t[ iSize + 1 ] ;
		CHECK_CHLOG_ERRORF( ao_rpcDestination == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( ( iSize + 1 ) * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;

		iResult = IOD.ReadBuffer( ao_rpcDestination ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		ao_rpcDestination[ iSize ] = '\0' ;
	}
	else
	{
		M4NewString( NULL, ao_rpcDestination ) ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Clase de parejas de cadenas a substituir
//=================================================================================

ClLiteralPair::ClLiteralPair( void )
{
	m_pccLeft = NULL ;
	m_pccRight = NULL ;
}


ClLiteralPair::ClLiteralPair( m4pcchar_t ai_pccLeft, m4pcchar_t ai_pccRight )
{
	m_pccLeft = NULL ;
	m_pccRight = NULL ;

	M4NewString( ai_pccLeft, m_pccLeft ) ;
	M4NewString( ai_pccRight, m_pccRight ) ;
}


ClLiteralPair::~ClLiteralPair( void )
{
	_Reset() ;
}


void	ClLiteralPair::_Reset( void )
{
	M4DeleteString( m_pccLeft ) ;
	M4DeleteString( m_pccRight ) ;
}


m4return_t	ClLiteralPair::Serialize( ClGenericIODriver &IOD )
{

	m4return_t	iResult = M4_ERROR ;


	iResult = IOD.Write( m_pccLeft ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Write( m_pccRight ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralPair::DeSerialize( ClGenericIODriver &IOD )
{

	m4return_t	iResult = M4_ERROR ;


	_Reset() ;

	iResult = DeSerializeString( IOD, m_pccLeft ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = DeSerializeString( IOD, m_pccRight ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Clase de claves y lista de parejas a sustituir
//=================================================================================

ClLiteralKey::ClLiteralKey( void)
{
	m_pccKey = NULL ;
	m_iLanguage = 0 ;
}


ClLiteralKey::ClLiteralKey( m4pcchar_t ai_pccKey, m4language_t ai_iLanguage )
{
	m_pccKey = NULL ;
	m_iLanguage = ai_iLanguage ;

	M4NewString( ai_pccKey, m_pccKey ) ;
	M4StrUpr( m_pccKey ) ;
}


ClLiteralKey::~ClLiteralKey( void )
{
	_Reset() ;
}


void	ClLiteralKey::_Reset( void )
{

	m4uint32_t	iSize = 0 ;

	vLiteralPairs_t::iterator	itPairs ;


	M4DeleteString( m_pccKey ) ;
	m_iLanguage = 0 ;

	iSize = m_vPairs.size() ;

	if( iSize > 0 )
	{
		itPairs = m_vPairs.begin() ;

		while( itPairs != m_vPairs.end() )
		{
			delete( *itPairs ) ;
			itPairs++ ;
		}

		m_vPairs.clear() ;
	}
}


m4return_t	ClLiteralKey::Serialize( ClGenericIODriver &IOD )
{

	m4uint32_t	iSize = 0 ;
	m4return_t	iResult = M4_ERROR ;

	vLiteralPairs_t::iterator	itPairs ;


	iResult = IOD.Write( m_pccKey ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Write( m_iLanguage ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iSize = m_vPairs.size() ;

	iResult = IOD.Write( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize > 0 )
	{
		itPairs = m_vPairs.begin() ;

		while( itPairs != m_vPairs.end() )
		{
			iResult = (*itPairs)->Serialize( IOD ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			itPairs++ ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralKey::DeSerialize( ClGenericIODriver &IOD )
{

	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iSize = 0 ;
	ClLiteralPair	*poPair = NULL ;


	_Reset() ;

	iResult = DeSerializeString( IOD, m_pccKey ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Read( m_iLanguage ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Read( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	for( i = 0 ; i < iSize ; i++ )
	{
		poPair = new ClLiteralPair() ;
		CHECK_CHLOG_ERRORF( poPair == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClLiteralPair ) ) << __LINE__ << __FILE__ ) ;

		m_vPairs.push_back( poPair ) ;

		iResult = poPair->DeSerialize( IOD ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Clase de organización y lista de claves a sustituir
//=================================================================================

ClLiteralOrganization::ClLiteralOrganization( void )
{
	m_pccOrganization = NULL ;
	m_dtVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;
}


ClLiteralOrganization::ClLiteralOrganization( m4pcchar_t ai_pccOrganization )
{
	m_pccOrganization = NULL ;
	m_dtVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;

	M4NewString( ai_pccOrganization, m_pccOrganization ) ;
}


ClLiteralOrganization::~ClLiteralOrganization( void )
{
	_Reset() ;
}


void	ClLiteralOrganization::_Reset( void )
{

	m4uint32_t	iSize = 0 ;

	vLiteralKeys_t::iterator	itKeys ;


	M4DeleteString( m_pccOrganization ) ;
	m_dtVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;

	iSize = m_vKeys.size() ;

	if( iSize > 0 )
	{
		itKeys = m_vKeys.begin() ;

		while( itKeys != m_vKeys.end() )
		{
			delete( *itKeys ) ;
			itKeys++ ;
		}

		m_vKeys.clear() ;
	}
}


m4return_t	ClLiteralOrganization::Serialize( ClGenericIODriver &IOD )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	iSize = 0 ;

	vLiteralKeys_t::iterator	itKeys ;


	iResult = IOD.Write( m_pccOrganization ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Write( m_dtVersion ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iSize = m_vKeys.size() ;

	iResult = IOD.Write( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize > 0 )
	{
		itKeys = m_vKeys.begin() ;

		while( itKeys != m_vKeys.end() )
		{
			iResult = (*itKeys)->Serialize( IOD ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			itKeys++ ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralOrganization::DeSerialize( ClGenericIODriver &IOD )
{

	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iSize = 0 ;
	ClLiteralKey	*poKey = NULL ;


	_Reset() ;

	iResult = DeSerializeString( IOD, m_pccOrganization ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Read( m_dtVersion ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Read( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	for( i = 0 ; i < iSize ; i++ )
	{
		poKey = new ClLiteralKey() ;
		CHECK_CHLOG_ERRORF( poKey == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClLiteralKey ) ) << __LINE__ << __FILE__ ) ;

		m_vKeys.push_back( poKey ) ;

		iResult = poKey->DeSerialize( IOD ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Clase de caché de literales
//=================================================================================

ClLiteralCache::ClLiteralCache( void )
: m_oMutex( M4_TRUE )
{
	m_pccFileName = NULL ;
}


ClLiteralCache::~ClLiteralCache( void )
{
	M4DeleteString( m_pccFileName ) ;

	_Reset() ;
}


void	ClLiteralCache::_Reset( void )
{

	m4uint32_t	iSize = 0 ;

	vLiteralOrganizations_t::iterator	itOrganizations ;


	iSize = m_vLiteralOrganizations.size() ;

	if( iSize > 0 )
	{
		itOrganizations = m_vLiteralOrganizations.begin() ;

		while( itOrganizations != m_vLiteralOrganizations.end() )
		{
			delete( *itOrganizations ) ;
			itOrganizations++ ;
		}

		m_vLiteralOrganizations.clear() ;
	}
}


m4return_t	ClLiteralCache::ReplaceLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccInput, m4uint16_t ai_iLength, m4pchar_t& ao_rpcOutput, m4pcchar_t ai_pccOrganization, m4bool_t& ao_rbModified )
{

	m4return_t				iResult = M4_ERROR ;
	m4bool_t				bFound = M4_FALSE ;
	m4language_t			iActualLanguage = M4_LANG_NONE_ID ;
	m4language_t			iKeyLanguage = M4_LANG_NONE_ID ;
	size_t					i = 0 ;
	size_t					iLength = 0 ;
	size_t					iPosition = 0 ;
	m4uint32_t				iSize = 0 ;
	m4pcchar_t				pccKey = NULL ;
	m4pcchar_t				pccLeft = NULL ;
	m4pcchar_t				pccRight = NULL ;
	m4pchar_t				pcUpperInput = NULL ;
	m4pchar_t				pcFound = NULL ;
	string					sResult ;
	vLiteralPairs_t			*poPairs  = NULL ;
	ClLiteralOrganization	*poOrganization = NULL ;
	ClLogonAdaptor			*poLogonAdaptor = NULL ;

	vLiteralKeys_t::iterator			itKeys ;
	vLiteralPairs_t::iterator			itPairs ;


	ao_rpcOutput = NULL ;
	ao_rbModified = M4_FALSE ;

	CHECK_CHLOG_ERRORF( ai_poChannelManager == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_poChannelManager" << "ClLiteralCache::ReplaceLiterals" << __LINE__ << __FILE__ ) ;

	if( ai_pccInput == NULL || *ai_pccInput == '\0' || ai_iLength == 0 )
	{
		// Si es cadena vacía no se hace nada
		return( M4_SUCCESS ) ;
	}

	poOrganization = _GetOrganization( ai_poChannelManager, ai_pccOrganization ) ;

	if( poOrganization == NULL )
	{
		// Si no se tiene sociedad no se hace nada
		return( M4_SUCCESS ) ;
	}

	// Se procesan todas las palabras clave si las hay
	iSize = poOrganization->m_vKeys.size() ;

	if( iSize > 0 )
	{
		// Se convierte la entrada a mayúscula para buscar la clave
		pcUpperInput = new m4char_t[ ai_iLength + 1 ] ;
		CHECK_CHLOG_ERRORF( pcUpperInput == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( ( ai_iLength + 1 ) * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;
		memcpy( pcUpperInput, ai_pccInput, ai_iLength ) ;
		pcUpperInput[ ai_iLength ] = '\0' ;
		pcUpperInput = M4StrUpr( pcUpperInput ) ;

		/* Bug 0268571
		Hay que inicializar el logon adaptor
		*/ 
		poLogonAdaptor = ai_poChannelManager->GetLogonAdaptor() ;

		if( poLogonAdaptor != NULL )
		{
			poLogonAdaptor->GetLanguage( iActualLanguage ) ;
		}

		itKeys = poOrganization->m_vKeys.begin() ;

		while( itKeys != poOrganization->m_vKeys.end() )
		{
			pccKey = (*itKeys)->m_pccKey ;
			iKeyLanguage = (*itKeys)->m_iLanguage ;
			poPairs = &( (*itKeys)->m_vPairs ) ;
			itKeys++ ;

			if( iKeyLanguage != M4_LANG_NONE_ID && iKeyLanguage!= iActualLanguage )
			{
				// Si tiene lenguaje y no es el de trabajo no se hace nada
				continue ;
			}

			pcFound = strstr( pcUpperInput, pccKey ) ;

			if( pcFound == NULL )
			{
				// Si la cadena clave no se encuentra no se hace nada
				continue ;
			}

			// Se recorren las parejas
			itPairs = poPairs->begin() ;

			while( itPairs != poPairs->end() )
			{
				pccLeft = (*itPairs)->m_pccLeft ;
				pccRight = (*itPairs)->m_pccRight ;
				itPairs++ ;

				iLength = strlen( pccLeft ) ;

				if( ao_rbModified == M4_FALSE )
				{
					// Si no se ha modificado hay que buscar sobre la cadena original
					/* Bug 0199363
					Se cambia el strstr porque no permite trabajar con cadenas no nullterminadas
					*/
					bFound = M4_FALSE ;
					if( ai_iLength >= iLength )
					{
						for( i = 0 ; i <= ai_iLength - iLength ; i++ )
						{
							if( memcmp( ai_pccInput + i, pccLeft, iLength ) == 0 )
							{
								bFound = M4_TRUE ;
								break ;
							}
						}
					}

					if( bFound == M4_FALSE )
					{
						// Si la parte de la izquierda no está en la cadena original no se hace nada
						continue ;
					}

					// Sólo se hace copia la primera vez que se detecta un cambio
					sResult.append( ai_pccInput, ai_iLength ) ;
					ao_rbModified = M4_TRUE ;
				}

				iPosition = M4_STR_NPOS ;
				iPosition = sResult.rfind( pccLeft, iPosition, iLength ) ;

				while( iPosition != M4_STR_NPOS )
				{
					// Se va sustituyendo de atrás alante
					sResult.replace( iPosition, iLength, pccRight ) ;

					if( iPosition < iLength )
					{
						// Si se busca con longitud menor que lo que se busca puede dar GP
						break ;
					}

#if M4_USE_NATIVE_STL == 1
					// En las stl nativas el rfind hay que rebovinarlo manualmente
					iPosition -= iLength ;
#endif

					iPosition = sResult.rfind( pccLeft, iPosition, iLength ) ;
				}
			}
		}

		delete [] pcUpperInput ;

		if( ao_rbModified == M4_TRUE )
		{
			iLength = sResult.size() + 1 ;
			ao_rpcOutput = new m4char_t[ iLength ] ;
			CHECK_CHLOG_ERRORF( ao_rpcOutput == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( iLength * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;
			memcpy( ao_rpcOutput, sResult.c_str(), iLength ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralCache::LiteralsVersion( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization, m4date_t& ao_rdtVersion )
{

	ClLiteralOrganization	*poOrganization = NULL ;


	ao_rdtVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;

	CHECK_CHLOG_ERRORF( ai_poChannelManager == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_poChannelManager" << "ClLiteralCache::LiteralsVersion" << __LINE__ << __FILE__ ) ;


	poOrganization = _GetOrganization( ai_poChannelManager, ai_pccOrganization ) ;

	if( poOrganization == NULL )
	{
		// Si no se tiene sociedad no se hace nada
		return( M4_SUCCESS ) ;
	}

	ao_rdtVersion = poOrganization->m_dtVersion ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralCache::CleanLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization )
{

	m4uint32_t				iSize = 0 ;
	ClLiteralOrganization	*poOrganization = NULL ;
	ClLogonAdaptor			*poLogonAdaptor = NULL ;

	vLiteralOrganizations_t::iterator	itOrganizations ;


	CHECK_CHLOG_ERRORF( ai_poChannelManager == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_poChannelManager" << "ClLiteralCache::CleanLiterals" << __LINE__ << __FILE__ ) ;


	if( ai_pccOrganization == NULL || *ai_pccOrganization == '\0' )
	{
		poLogonAdaptor = ai_poChannelManager->GetLogonAdaptor() ;

		if( poLogonAdaptor != NULL )
		{
			ai_pccOrganization = poLogonAdaptor->GetOrganization() ;
		}

		if( ai_pccOrganization == NULL || *ai_pccOrganization == '\0' )
		{
			// Si no se tiene sociedad no se hace nada
			return( M4_SUCCESS ) ;
		}
	}


	m_oMutex.Lock() ;

	// Se buscan los literales de la organización
	iSize = m_vLiteralOrganizations.size() ;

	if( iSize > 0 )
	{
		itOrganizations = m_vLiteralOrganizations.begin() ;

		while( itOrganizations != m_vLiteralOrganizations.end() )
		{
			if( strcmpi( (*itOrganizations)->m_pccOrganization, ai_pccOrganization ) == 0 )
			{
				// Se libera la memoria y se elimina del vector
				delete( *itOrganizations ) ;
				m_vLiteralOrganizations.erase( itOrganizations ) ;
				_WriteToFile() ;
				break ;
			}
			itOrganizations++ ;
		}
	}

	m_oMutex.Unlock() ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralCache::CleanAllLiterals( void )
{
	m_oMutex.Lock() ;

	_Reset() ;
	_WriteToFile() ;

	m_oMutex.Unlock() ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralCache::SetLiteralsCacheDirectory( ClCacheDirectory *ai_poCacheDirectory )
{

	m4pcchar_t	pccDirectory = NULL ;
	ClObjectId	*poObjectId = NULL ;


	m_oMutex.Lock() ;

	if( ai_poCacheDirectory != NULL )
	{
		ai_poCacheDirectory->Detach( this ) ;
		ai_poCacheDirectory->Attach( this ) ;

		/* Bug 0266294
		Comprobación de que se puede obtener el directorio.
		*/
		poObjectId = ai_poCacheDirectory->GetObjId() ;

		if( poObjectId != NULL )
		{
			pccDirectory = poObjectId->GetEnvironmentId() ;

			if( pccDirectory != NULL )
			{
				_SetDirectory( pccDirectory ) ;
			}
		}
	}

	m_oMutex.Unlock() ;

	return( M4_SUCCESS ) ;
}


m4return_t ClLiteralCache::RemoveAll( m4date_t ai_dDate )
{
	return CleanAllLiterals() ;
}


m4return_t ClLiteralCache::RemoveObjectByIdAndVersion( m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType )
{
	return CleanAllLiterals() ;
}


m4return_t	ClLiteralCache::_Serialize( ClGenericIODriver &IOD )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	iSize = 0 ;

	vLiteralOrganizations_t::iterator	itOrganizations ;


	iResult = IOD.Write( m4uint16_t( LITERALS_SERIALIZATION_VERSION ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iSize = m_vLiteralOrganizations.size() ;

	iResult = IOD.Write( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize > 0 )
	{
		itOrganizations = m_vLiteralOrganizations.begin() ;

		while( itOrganizations != m_vLiteralOrganizations.end() )
		{
			iResult = (*itOrganizations)->Serialize( IOD ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			itOrganizations++ ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralCache::_DeSerialize( ClGenericIODriver &IOD )
{

	m4return_t				iResult = M4_ERROR ;
	m4uint16_t				iVersion = 0 ;
	m4uint32_t				i = 0 ;
	m4uint32_t				iSize = 0 ;
	ClLiteralOrganization	*poOrganization = NULL ;


	_Reset() ;

	iResult = IOD.Read( iVersion ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iVersion != LITERALS_SERIALIZATION_VERSION )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Read( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	for( i = 0 ; i < iSize ; i++ )
	{
		poOrganization = new ClLiteralOrganization() ;
		CHECK_CHLOG_ERRORF( poOrganization == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClLiteralOrganization ) ) << __LINE__ << __FILE__ ) ;

		m_vLiteralOrganizations.push_back( poOrganization ) ;

		iResult = poOrganization->DeSerialize( IOD ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralCache::_SetDirectory( m4pcchar_t ai_pccDirectory )
{

	int			iResult = 0 ;
	m4char_t	acBuffer[ M4CH_MAX_FILE + 1 ] = "" ;


	if( ai_pccDirectory == NULL )
	{
		return( M4_ERROR ) ;
	}

	strcpy( acBuffer, ai_pccDirectory ) ;

	iResult = M4RegMkDir( acBuffer, M4_TRUE ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	strcat( acBuffer, M4_BACK_SLASH ) ;
	strcat( acBuffer, "RLT" ) ;

	iResult = M4RegMkDir( acBuffer, M4_TRUE ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	strcat( acBuffer, M4_BACK_SLASH ) ;
	strcat( acBuffer, "literals.rlt" ) ;

	M4NewString( acBuffer, m_pccFileName ) ;

	_ReadFromFile() ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralCache::_WriteToFile( void )
{

	m4return_t		iResult = M4_ERROR ;
	ClFileIODriver	IOD ;


	if( m_pccFileName == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Init( GEN_IO_MODE_WRITE, m_pccFileName ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = _Serialize( IOD ) ;

    IOD.End( M4_TRUE ) ;

	if( iResult != M4_SUCCESS )
	{
		M4RemoveFile( m_pccFileName ) ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClLiteralCache::_ReadFromFile( void )
{

	m4return_t		iResult = M4_ERROR ;
	ClFileIODriver	IOD ;


	if( m_pccFileName == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = IOD.Init( GEN_IO_MODE_READ, m_pccFileName, M4_FALSE ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = _DeSerialize( IOD ) ;

    IOD.End( M4_TRUE ) ;

	if( iResult != M4_SUCCESS )
	{
		_Reset() ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


ClLiteralOrganization*	ClLiteralCache::_GetOrganization( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization )
{

	m4return_t				iResult = M4_ERROR ;
	ClLiteralOrganization	*poOrganization = NULL ;
	ClLogonAdaptor			*poLogonAdaptor = NULL ;

	vLiteralOrganizations_t::iterator	itOrganizations ;

	
	CHECK_CHLOG_ERRORF( ai_poChannelManager == NULL, NULL, M4_CH_DM_NULL_ARGUMENT, "ai_poChannelManager" << "ClLiteralCache::_GetOrganization" << __LINE__ << __FILE__ ) ;


	if( ai_pccOrganization == NULL || *ai_pccOrganization == '\0' )
	{
		poLogonAdaptor = ai_poChannelManager->GetLogonAdaptor() ;

		if( poLogonAdaptor != NULL )
		{
			ai_pccOrganization = poLogonAdaptor->GetOrganization() ;
		}

		if( ai_pccOrganization == NULL || *ai_pccOrganization == '\0' )
		{
			// Si no se tiene sociedad no se hace nada
			return( NULL ) ;
		}
	}


	m_oMutex.Lock() ;

	// Se buscan los literales de la organización
	itOrganizations = m_vLiteralOrganizations.begin() ;

	while( itOrganizations != m_vLiteralOrganizations.end() )
	{
		if( strcmpi( (*itOrganizations)->m_pccOrganization, ai_pccOrganization ) == 0 )
		{
			break ;
		}
		itOrganizations++ ;
	}

	if( itOrganizations == m_vLiteralOrganizations.end() )
	{
		// Si no están se construyen
		poOrganization = new ClLiteralOrganization( ai_pccOrganization ) ;
		CHECK_CHLOG_ERRORF( poOrganization == NULL, NULL, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClLiteralOrganization ) ) << __LINE__ << __FILE__ ) ;

		// Si falla la inicialización no se hace nada
		iResult = _LoadLiterals( ai_poChannelManager, ai_pccOrganization, poOrganization->m_vKeys, poOrganization->m_dtVersion ) ;
		m_vLiteralOrganizations.push_back( poOrganization ) ;
		_WriteToFile() ;
	}
	else
	{
		poOrganization = *itOrganizations ;
	}

	m_oMutex.Unlock() ;

	return( poOrganization ) ;
}


// Carga los literales a partir de un acceso ya creado
m4return_t	ClLiteralCache::_LoadLiterals( ClAccess *ai_poAccess, m4pcchar_t ai_pccOrganization, vLiteralKeys_t& ao_rvLiteralKeys, m4date_t& ao_rdtVersion )
{

	m4return_t		iResult = M4_ERROR ;
	m4bool_t		bEnabled = M4_FALSE ;
	m4bool_t		bIsNotEof = M4_FALSE ;
	m4language_t	iLanguage = M4_LANG_NONE_ID ;
	m4pchar_t		pcValue = NULL ;
	m4pchar_t		pcEqual = NULL ;
	m4pchar_t		pcNewLine = NULL ;
	ClNode			*poNode = NULL ;
	IStackInterface	*poStack = NULL ;
	ClLiteralKey	*poKey = NULL ;
	ClLiteralPair	*poPair = NULL ;
	m4VariantType	vKey ;
	m4VariantType	vLanguage ;
	m4VariantType	vVersion ;
	m4VariantType	vValue ;


	ao_rdtVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;

	poNode = &( ai_poAccess->Node[ "SAV_PARAMS"] ) ;

	if( poNode == NULL )
	{
		return( M4_ERROR ) ;
	}

	poStack = &( ai_poAccess->GetpExecutor()->Stack ) ;

	if( poStack == NULL )
	{
		return( M4_ERROR ) ;
	}

	vValue.SetStringRef( "FORMS" ) ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	vValue.SetStringRef( "CHANGETEXT" ) ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	vValue.SetNull() ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	vValue.SetNull() ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	vValue.SetNull() ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	vValue.SetStringRef( (m4pchar_t)ai_pccOrganization ) ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se ejecuta el RET_VALUE_WITH_PARAMS( "FORMS", "CHANGETEXT", NULL, NULL, NULL, "ORGANIZATION" )
	iResult = poNode->RecordSet.Item[ "RET_VALUE_WITH_PARAMS" ].Call( NULL, 6 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	// Se procesan los resultados
	poNode = &( ai_poAccess->Node[ "FINAL_RETURN"] ) ;

	if( poNode == NULL )
	{
		return( M4_ERROR ) ;
	}

	// Se hace una primera pasada buscando el ENABLED
	bIsNotEof = poNode->RecordSet.Register.Begin() ;

	bEnabled = M4_FALSE ;
	while( bIsNotEof == M4_TRUE )
	{
		iResult = poNode->RecordSet.Register.Item[ "ID_KEY" ].Value.Get( vKey ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( vKey.Type != M4CL_CPP_TYPE_STRING_VAR || vKey.Data.PointerChar == NULL || strcmpi( vKey.Data.PointerChar, "ENABLED" ) != 0 )
		{
			bIsNotEof = poNode->RecordSet.Register.Next() ;
			continue ;
		}

		iResult = poNode->RecordSet.Register.Item[ "DT_LAST_UPDATE" ].Value.Get( vVersion ) ;

		if( iResult == M4_SUCCESS && vVersion.Type == M4CL_CPP_TYPE_DATE )
		{
			if( vVersion.Data.DoubleData > ao_rdtVersion )
			{
				ao_rdtVersion = vVersion.Data.DoubleData;
			}
		}

		iResult = poNode->RecordSet.Register.Item[ "APP_VALUE" ].Value.Get( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL && strcmpi( vValue.Data.PointerChar, "1" ) == 0 )
		{
			bEnabled = M4_TRUE ;
		}

		break ;
	}

	if( bEnabled == M4_TRUE )
	{
		// Se hace una segunda pasada buscando los valores si están activados
		bIsNotEof = poNode->RecordSet.Register.Begin() ;

		bEnabled = M4_FALSE ;
		while( bIsNotEof == M4_TRUE )
		{
			iResult = poNode->RecordSet.Register.Item[ "ID_KEY" ].Value.Get( vKey ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			if( vKey.Type != M4CL_CPP_TYPE_STRING_VAR || vKey.Data.PointerChar == NULL || strcmpi( vKey.Data.PointerChar, "ENABLED" ) == 0 )
			{
				bIsNotEof = poNode->RecordSet.Register.Next() ;
				continue ;
			}

			iResult = poNode->RecordSet.Register.Item[ "DT_LAST_UPDATE" ].Value.Get( vVersion ) ;

			if( iResult == M4_SUCCESS && vVersion.Type == M4CL_CPP_TYPE_DATE )
			{
				if( vVersion.Data.DoubleData > ao_rdtVersion )
				{
					ao_rdtVersion = vVersion.Data.DoubleData;
				}
			}

			iResult = poNode->RecordSet.Register.Item[ "APP_VALUE" ].Value.Get( vValue ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				iResult = poNode->RecordSet.Register.Item[ "ID_T3" ].Value.Get( vLanguage ) ;

				if( iResult == M4_SUCCESS && vLanguage.Type == M4CL_CPP_TYPE_STRING_VAR && vLanguage.Data.PointerChar != NULL )
				{
					iLanguage = atoi( vLanguage.Data.PointerChar ) ;
				}
				else
				{
					iLanguage = M4_LANG_NONE_ID ;
				}

				poKey = new ClLiteralKey( vKey.Data.PointerChar, iLanguage ) ;
				CHECK_CHLOG_ERRORF( poKey == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClLiteralKey ) ) << __LINE__ << __FILE__ ) ;

				pcValue = vValue.Data.PointerChar ;

				// Se lee el formato de la cadena left=right\r\nleft=right...
				do
				{
					pcNewLine = strchr( pcValue, '\n' ) ;

					if( pcNewLine != NULL )
					{
						*pcNewLine = '\0' ;

						if( pcNewLine > pcValue && *( pcNewLine - 1 ) == '\r' )
						{
							*( pcNewLine - 1 )= '\0' ;
						}
					}

					pcEqual = strchr( pcValue, '=' ) ;

					/* Bug
					Si no hay parte izquierda no se añade como patrón
					*/
					if( pcEqual != NULL && pcEqual != pcValue )
					{
						*pcEqual = '\0' ;

						poPair = new ClLiteralPair( pcValue, pcEqual + 1 ) ;
						CHECK_CHLOG_ERRORF( poPair == NULL, M4_ERROR, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( ClLiteralPair ) ) << __LINE__ << __FILE__ ) ;

						poKey->m_vPairs.push_back( poPair ) ;
					}

					if( pcNewLine != NULL )
					{
						pcValue = pcNewLine + 1 ;
					}
				}
				while( pcNewLine != NULL ) ;

				if( poKey->m_vPairs.size() > 0 )
				{
					ao_rvLiteralKeys.push_back( poKey ) ;
				}
				else
				{
					delete poKey ;
				}
			}

			bIsNotEof = poNode->RecordSet.Register.Next() ;
		}
	}

	return( M4_SUCCESS ) ;
}


// Carga los literales a partir de un channelnameger
// Sirve para simplificar la gestión de errores
m4return_t	ClLiteralCache::_LoadLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization, vLiteralKeys_t& ao_rvLiteralKeys, m4date_t& ao_rdtVersion )
{

	m4return_t	iResult = M4_ERROR ;
	ClAccess	*poAccess = NULL ;
	ClChannel	*poChannel = NULL ;


	ao_rdtVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;

	CHECK_CHLOG_ERRORF( ai_poChannelManager == NULL, M4_ERROR, M4_CH_DM_NULL_ARGUMENT, "ai_poChannelManager" << "_LoadLiterals" << __LINE__ << __FILE__ ) ;


	// Se crea una instancia del SAV_PARAMS
	iResult = ai_poChannelManager->CreateChannel( poChannel ) ;

	if( iResult != M4_SUCCESS || poChannel == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = poChannel->BuildFromId( "SAV_PARAMS" ) ;

	if( iResult != M4_SUCCESS )
	{
		poChannel->Destroy() ;
		return( M4_ERROR ) ;
	}

	iResult = poChannel->CreateAccess( poAccess ) ;

	if( iResult != M4_SUCCESS )
	{
		poChannel->Destroy() ;
		return( M4_ERROR ) ;
	}

	iResult = _LoadLiterals( poAccess, ai_pccOrganization, ao_rvLiteralKeys, ao_rdtVersion ) ;

	poAccess->Destroy() ;
	poChannel->Destroy() ;

	return( iResult ) ;
}






//=================================================================================
// Funciones de substitución de literales
//=================================================================================

static	ClLiteralCache	s_oLiteralCache ;


m4return_t	M4ReplaceLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccInput, m4uint16_t ai_iLength, m4pchar_t& ao_rpcOutput, m4pcchar_t ai_pccOrganization, m4bool_t& ao_rbModified )
{
	return( s_oLiteralCache.ReplaceLiterals( ai_poChannelManager, ai_pccInput, ai_iLength, ao_rpcOutput, ai_pccOrganization, ao_rbModified ) ) ;
}


m4return_t	M4ReplaceLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccInput, m4pchar_t& ao_rpcOutput, m4pcchar_t ai_pccOrganization, m4bool_t& ao_rbModified )
{

	m4uint16_t iLength = 0 ;


	if( ai_pccInput != NULL )
	{
		iLength = strlen( ai_pccInput ) ;
	}

	return(	M4ReplaceLiterals( ai_poChannelManager, ai_pccInput, iLength, ao_rpcOutput, ai_pccOrganization, ao_rbModified ) ) ;
}


m4return_t	M4LiteralsVersion( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization, m4date_t& ao_rdtVersion )
{
	return( s_oLiteralCache.LiteralsVersion( ai_poChannelManager, ai_pccOrganization, ao_rdtVersion ) ) ;
}


m4return_t	M4CleanLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization )
{
	return( s_oLiteralCache.CleanLiterals( ai_poChannelManager, ai_pccOrganization ) ) ;
}


m4return_t	M4CleanAllLiterals( void )
{
	return( s_oLiteralCache.CleanAllLiterals() ) ;
}


m4return_t	M4SetLiteralsCacheDirectory( ClCacheDirectory *ai_poCacheDirectory )
{
	return( s_oLiteralCache.SetLiteralsCacheDirectory( ai_poCacheDirectory ) ) ;
}






