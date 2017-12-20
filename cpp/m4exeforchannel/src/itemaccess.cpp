//	==============================================================================
//
//	 (c) Copyright  1991-2013 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              itemaccess.cpp
//	 Project:           M4ExeForChannel
//	 Author:            Meta Software M.S. , S.A
//	 Date:				2013-09-24
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the structures to support item access information
//
//
//	==============================================================================


#include <string.h>
#include "itemaccess.hpp"
#include "dm.hpp"
#include "access.hpp"
#include "executor.hpp"
#include "stack.hpp"
#include "m4srvefcres.hpp"

#define M4_ITEM_INFO_NOT_FOUND 65535

//=================================================================================
// This class just manages automatic object release
//=================================================================================
class M4ItemAccessReleaser {

private:
	ClChannel* &m_poChannel;
	ClAccess*  &m_poAccess;

public:
	M4ItemAccessReleaser( ClChannel* &ai_poChannel, ClAccess* &ai_poAccess );
	~M4ItemAccessReleaser();

};

M4ItemAccessReleaser::M4ItemAccessReleaser( ClChannel* &ai_poChannel, ClAccess* &ai_poAccess )
	: m_poChannel( ai_poChannel ), m_poAccess( ai_poAccess )
{}

M4ItemAccessReleaser::~M4ItemAccessReleaser()
{
	if ( m_poAccess != NULL ) 
	{
		m_poAccess->Destroy();
	}
	
	if ( m_poChannel != NULL ) 
	{
		m_poChannel->Destroy();
	}
}

//=================================================================================
// Estructuras para los permisos de acceso
//=================================================================================

void	M4SetLocalId( m4pcchar_t ai_pccSource, m4pchar_t ao_pcDestination, m4uint16_t ai_iMaxLength )
{
	size_t	iLength = 0 ;

	if( ai_pccSource != NULL )
	{
		iLength = strlen( ai_pccSource ) ;

		if( iLength > ai_iMaxLength )
		{
			iLength = ai_iMaxLength ;
		}

		memcpy( ao_pcDestination, ai_pccSource, iLength ) ;
		ao_pcDestination[ iLength ] = '\0' ;
		strupr( ao_pcDestination ) ;
	}
	else
	{
		*ao_pcDestination = '\0' ;
	}
}



// Clase de permisos de acceso de un ítem
void	ClItemAccess :: Init( ClRegister &ai_roRegister )
{

	m4return_t		iResult = M4_ERROR ;
	m4VariantType	vValue ;


	iResult = ai_roRegister.Item[ "CAN_READ" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 1.0 )
	{
		m_bCanRead = M4_TRUE ;
	}
	else
	{
		m_bCanRead = M4_FALSE ;
	}

	iResult = ai_roRegister.Item[ "CAN_WRITE" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 1.0 )
	{
		m_bCanWrite = M4_TRUE ;
	}
	else
	{
		m_bCanWrite = M4_FALSE ;
	}

	iResult = ai_roRegister.Item[ "CAN_EXECUTE" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 1.0 )
	{
		m_bCanExecute = M4_TRUE ;
	}
	else
	{
		m_bCanExecute = M4_FALSE ;
	}

	iResult = ai_roRegister.Item[ "MUST_AUTHENTICATE" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 0.0 )
	{
		m_bMustAuthenticate = M4_FALSE ;
	}
	else
	{
		m_bMustAuthenticate = M4_TRUE ;
	}

	iResult = ai_roRegister.Item[ "ID_LEVEL" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER )
	{
		m_uiIdLevel = (m4uint16_t) vValue.Data.DoubleData ;
	}
	else
	{
		m_uiIdLevel = 0 ;
	}

	iResult = ai_roRegister.Item[ "ENCRYPTED" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 0.0 )
	{
		m_bEncrypted = M4_FALSE ;
	}
	else
	{
		m_bEncrypted = M4_TRUE ;
	}

}

// Clase clave de permisos de acceso de un ítem por nodo e ítem
void	ClItemAccessKey :: Init( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	M4SetLocalId( ai_pccNode, m_acNode, M4CL_MAX_NODE_ID ) ;
	M4SetLocalId( ai_pccItem, m_acItem, M4CL_MAX_ITEM_ID ) ;
}


void	ClItemAccessKey :: Init( ClRegister &ai_roRegister )
{

	m4return_t		iResult = M4_ERROR ;
	m4VariantType	vValue ;


	iResult = ai_roRegister.Item[ "ID_NODE" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		M4SetLocalId( vValue.Data.PointerChar, m_acNode, M4CL_MAX_NODE_ID ) ;
	}
	else
	{
		*m_acNode = '\0' ;
	}

	iResult = ai_roRegister.Item[ "ID_ITEM" ].Value.Get( vValue ) ;

	if( iResult == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		M4SetLocalId( vValue.Data.PointerChar, m_acItem, M4CL_MAX_ITEM_ID ) ;
	}
	else
	{
		*m_acItem = '\0' ;
	}
}


m4bool_t operator < ( const ClItemAccessKey &lhc, const ClItemAccessKey &rhc )
{

	int	iCompare = 0 ;


	iCompare = strcmp( lhc.m_acItem, rhc.m_acItem ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	if( iCompare != 0 )
	{
		return M4_FALSE ;
	}

	iCompare = strcmp( lhc.m_acNode, rhc.m_acNode ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	return M4_FALSE ;
}


m4bool_t operator == ( const ClItemAccessKey &lhc, const ClItemAccessKey &rhc )
{
	if( strcmp( lhc.m_acItem, rhc.m_acItem ) == 0 && strcmp( lhc.m_acNode, rhc.m_acNode ) == 0 )
	{
		return M4_TRUE ;
	}
	return M4_FALSE ;
}


// Clase clave de permisos de acceso de un ítem por objeto, nodo e ítem
ClChannelItemAccessKey :: ClChannelItemAccessKey( m4pcchar_t ai_pccChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	M4SetLocalId( ai_pccChannel, m_acChannel,	M4CL_MAX_T3_ID ) ;
	M4SetLocalId( ai_pccNode,    m_acNode,		M4CL_MAX_NODE_ID ) ;
	M4SetLocalId( ai_pccItem,    m_acItem,		M4CL_MAX_ITEM_ID ) ;
}


m4bool_t operator < ( const ClChannelItemAccessKey &lhc, const ClChannelItemAccessKey &rhc )
{

	int	iCompare = 0 ;

	iCompare = strcmp( lhc.m_acItem, rhc.m_acItem ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	if( iCompare != 0 )
	{
		return M4_FALSE ;
	}

	iCompare = strcmp( lhc.m_acNode, rhc.m_acNode ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	if( iCompare != 0 )
	{
		return M4_FALSE ;
	}

	iCompare = strcmp( lhc.m_acChannel, rhc.m_acChannel ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	return M4_FALSE ;
}


m4bool_t operator == ( const ClChannelItemAccessKey &lhc, const ClChannelItemAccessKey &rhc )
{
	if( strcmp( lhc.m_acItem, rhc.m_acItem ) == 0 && strcmp( lhc.m_acNode, rhc.m_acNode ) == 0 
		&& strcmp( lhc.m_acChannel, rhc.m_acChannel ) == 0 )
	{
		return M4_TRUE ;
	}
	return M4_FALSE ;
}


// Clase de mapa de permisos de acceso de un ítem
m4return_t	ClItemAccessMap :: Init( ClRegister &ai_roRegister )
{

	m4return_t		iResult = M4_ERROR ;
	m4bool_t		bIsNotEof = M4_FALSE ;
	ClItemAccessKey	oKey ;
	ClItemAccess	oValue ;


	if( m_poAccessMap != NULL && m_bOwner == M4_TRUE )
	{
		delete( m_poAccessMap ) ;
	}

	m_poAccessMap = NULL ;

	bIsNotEof = ai_roRegister.Begin() ;

	if( bIsNotEof == M4_TRUE )
	{
		m_poAccessMap = new mItemAccess_t() ;
		CHECK_CHLOG_ERROR( m_poAccessMap == NULL, M4_ERROR, M4_ERR_XML_OUT_OF_MEMORY ) ;

		while( bIsNotEof == M4_TRUE )
		{
			oKey.Init( ai_roRegister ) ;
			oValue.Init( ai_roRegister ) ;
			m_poAccessMap->insert(mItemAccess_t::value_type(oKey, oValue));
			bIsNotEof = ai_roRegister.Next() ;
		}
	}

	return( M4_SUCCESS ) ;
}


void	ClItemAccessMap :: Find( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4bool_t &ao_bCanRead, 
								m4bool_t &ao_bCanWrite, m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, 
								m4uint16_t &ao_uiIdLevel, m4bool_t &ao_bEncrypted )
{

	ClItemAccessKey			oKey ;
	mItemAccess_t::iterator	it ;


	ao_bCanRead = M4_FALSE ;
	ao_bCanWrite = M4_FALSE ;
	ao_bCanExecute = M4_FALSE ;
	ao_bMustAuthenticate = M4_TRUE ;
	ao_uiIdLevel = M4_ITEM_INFO_NOT_FOUND ;
	ao_bEncrypted = M4_TRUE;

	if( m_poAccessMap != NULL )
	{
		oKey.Init( ai_pccNode, ai_pccItem ) ;

		it = m_poAccessMap->find( oKey ) ;

		if( it != m_poAccessMap->end() )
		{
			it->second.GetValues( ao_bCanRead, ao_bCanWrite, ao_bCanExecute, ao_bMustAuthenticate, ao_uiIdLevel, ao_bEncrypted ) ;
		}
	}
}



// Clase de clave de mapa de permisos de acceso de un ítem por ámbito
void	ClItemAccessMapKey :: Init( ClChannel *ai_poChannel )
{

	m4double_t	dLanguage = M4CL_LANGUAGE_NONE ;
	m4pchar_t	pcRole = NULL ;
	m4pchar_t	pcOrganization = NULL ;


	ai_poChannel->Role_ID.Get( pcRole ) ;
	M4SetLocalId( pcRole, m_acRole, M4CL_MAX_ROLE_ID ) ;

	ai_poChannel->Organization.Get( pcOrganization ) ;
	M4SetLocalId( pcOrganization, m_acOrganization, M4CL_MAX_ORG_ID ) ;

	ai_poChannel->Data_Language.Get( dLanguage ) ;
	m_iLanguage = m4language_t( dLanguage ) ;
}


m4bool_t operator < ( const ClItemAccessMapKey &lhc, const ClItemAccessMapKey &rhc )
{

	int	iCompare = 0 ;


	if( lhc.m_iLanguage < rhc.m_iLanguage )
	{
		return M4_TRUE ;
	}

	if( lhc.m_iLanguage != rhc.m_iLanguage )
	{
		return M4_FALSE ;
	}

	iCompare = strcmp( lhc.m_acOrganization, rhc.m_acOrganization ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	if( iCompare != 0 )
	{
		return M4_FALSE ;
	}

	iCompare = strcmp( lhc.m_acRole, rhc.m_acRole ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	return M4_FALSE ;
}


m4bool_t operator == ( const ClItemAccessMapKey &lhc, const ClItemAccessMapKey &rhc )
{
	if( lhc.m_iLanguage == rhc.m_iLanguage && strcmp( lhc.m_acOrganization, rhc.m_acOrganization ) == 0 && strcmp( lhc.m_acRole, rhc.m_acRole ) == 0 )
	{
		return M4_TRUE ;
	}
	return M4_FALSE ;
}

// Clase contenedora de cache de acceso de item
ClCachedItemAccess::ClCachedItemAccess ( ClItemAccess ai_oItemAccess, string &ai_sEncryptedFuncGroups )
{
	m_oItemAccess = ai_oItemAccess;
	m_sEncryptedFuncGroups = ai_sEncryptedFuncGroups;
}

void ClCachedItemAccess::GetItemAccess( ClItemAccess &ao_oItemAccess ) 
{
	ao_oItemAccess = m_oItemAccess;
}

void ClCachedItemAccess::GetEncryptedFuncGroups ( string &ao_sEncryptedFuncGroups )
{
	ao_sEncryptedFuncGroups = m_sEncryptedFuncGroups;
}


// Clase de permisos de acceso de un Meta4Object
void	ClT3Access :: Find( ClT3AccessProvider *ai_poAcessProvider, m4pcchar_t ai_pccGroup, ClChannel *ai_poChannel, 
						   m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4bool_t &ao_bCanRead, m4bool_t &ao_bCanWrite, 
						   m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, m4uint16_t &ao_uiIdLevel, 
						   m4bool_t &ao_bEncrypted )
{

	m4return_t					iResult = M4_ERROR ;
	ClItemAccessMapKey			oKey ;
	ClItemAccessMap				oValue ;
	mItemAccessMap_t::iterator	it ;


	ao_bCanRead = M4_FALSE ;
	ao_bCanWrite = M4_FALSE ;
	ao_bCanExecute = M4_FALSE ;
	ao_bMustAuthenticate = M4_TRUE ;
	ao_uiIdLevel = M4_ITEM_INFO_NOT_FOUND ;
	ao_bEncrypted = M4_TRUE;

	if( m_poSingleAccess != NULL )
	{
		// Si no depende de parámetros se devuelve el general
		m_poSingleAccess->Find( ai_pccNode, ai_pccItem, ao_bCanRead, ao_bCanWrite, ao_bCanExecute, ao_bMustAuthenticate, ao_uiIdLevel, ao_bEncrypted ) ;
		return ;
	}

	oKey.Init( ai_poChannel ) ;

	if( m_poMultiAccess != NULL )
	{
		// Si depende de parámetros se busca si existe la información para los parámetros actuales
		it = m_poMultiAccess->find( oKey ) ;

		if( it != m_poMultiAccess->end() )
		{
			it->second.Find( ai_pccNode, ai_pccItem, ao_bCanRead, ao_bCanWrite, ao_bCanExecute, ao_bMustAuthenticate, ao_uiIdLevel, ao_bEncrypted ) ;
			return ;
		}
	}

	// Si no existe información para los parámetros actuales se carga
	iResult = _Load( ai_poAcessProvider, ai_pccGroup, ai_poChannel, oKey, oValue ) ;

	if( iResult == M4_SUCCESS )
	{
		oValue.Find( ai_pccNode, ai_pccItem, ao_bCanRead, ao_bCanWrite, ao_bCanExecute, ao_bMustAuthenticate, ao_uiIdLevel, ao_bEncrypted ) ;
	}
}


m4return_t	ClT3Access :: _Load( ClT3AccessProvider *ai_poAcessProvider, m4pcchar_t ai_pccGroup, ClChannel *ai_poChannel, const ClItemAccessMapKey &ai_roKey, ClItemAccessMap& ao_roMap )
{

	m4return_t		iResult = M4_ERROR ;
	ClNode			*poNode = NULL ;
	m4VariantType	vValue ;


	poNode = ai_poAcessProvider->LoadGroup( ai_pccGroup, ai_poChannel, ai_roKey ) ;

	if( poNode == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = ao_roMap.Init( poNode->RecordSet.Register ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( m_poSingleAccess == NULL && m_poMultiAccess == NULL )
	{
		// La primera vez se lee si tiene o no parámetros
		iResult = poNode->RecordSet.Item[ "HAS_PARAMETERS" ].Value.Get( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData == 1.0 )
		{
			m_poMultiAccess = new mItemAccessMap_t() ;

			if( m_poMultiAccess == NULL )
			{
				DUMP_CHLOG_ERROR( M4_ERR_XML_OUT_OF_MEMORY ) ;
				return( M4_ERROR ) ;
			}
		}
		else
		{
			m_poSingleAccess = new ClItemAccessMap( ao_roMap ) ;

			if( m_poSingleAccess == NULL )
			{
				DUMP_CHLOG_ERROR( M4_ERR_XML_OUT_OF_MEMORY ) ;
				return( M4_ERROR ) ;
			}
		}
	}

	if( m_poMultiAccess != NULL )
	{
		m_poMultiAccess->insert(mItemAccessMap_t::value_type(ai_roKey, ao_roMap));
	}

	return( M4_SUCCESS ) ;
}


// Clase clave de permisos de acceso de un Meta4Object por grupo y Meta4Object
void	ClT3AccessKey :: Init( m4pcchar_t ai_pccGroup, m4pcchar_t ai_pccT3 )
{
	M4SetLocalId( ai_pccGroup, m_acGroup, M4CL_MAX_T3_ID ) ;
	M4SetLocalId( ai_pccT3, m_acT3, M4CL_MAX_T3_ID ) ;
}


m4bool_t operator < ( const ClT3AccessKey &lhc, const ClT3AccessKey &rhc )
{

	int	iCompare = 0 ;


	iCompare = strcmp( lhc.m_acT3, rhc.m_acT3 ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	if( iCompare != 0 )
	{
		return M4_FALSE ;
	}

	iCompare = strcmp( lhc.m_acGroup, rhc.m_acGroup ) ;

	if( iCompare < 0 )
	{
		return M4_TRUE ;
	}

	return M4_FALSE ;
}


m4bool_t operator == ( const ClT3AccessKey &lhc, const ClT3AccessKey &rhc )
{
	if( strcmp( lhc.m_acT3, rhc.m_acT3 ) == 0 && strcmp( lhc.m_acGroup, rhc.m_acGroup ) == 0 )
	{
		return M4_TRUE ;
	}
	return M4_FALSE ;
}



// Clase de mapa de permisos de acceso de un Meta4Object

ClT3AccessProvider :: ~ClT3AccessProvider( void )
{
	_Reset() ;
}


m4bool_t	ClT3AccessProvider :: CanRead( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	m4bool_t	bCanRead = M4_FALSE ;
	m4bool_t	bCanWrite = M4_FALSE ;
	m4bool_t	bCanExecute = M4_FALSE ;
	m4bool_t	bMustAuthenticate = M4_FALSE ;
	m4bool_t	bEncrypted = M4_FALSE ;
	string		sEncryptedFuncGroups;

	_FindAny( ai_poChannel, ai_pccNode, ai_pccItem, bCanRead, bCanWrite, bCanExecute, bMustAuthenticate, bEncrypted, sEncryptedFuncGroups ) ;
	return( bCanRead ) ;
}


m4bool_t	ClT3AccessProvider :: CanWrite( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	m4bool_t	bCanRead = M4_FALSE ;
	m4bool_t	bCanWrite = M4_FALSE ;
	m4bool_t	bCanExecute = M4_FALSE ;
	m4bool_t	bMustAuthenticate = M4_FALSE ;
	m4bool_t	bEncrypted = M4_FALSE ;
	string		sEncryptedFuncGroups;

	_FindAny( ai_poChannel, ai_pccNode, ai_pccItem, bCanRead, bCanWrite, bCanExecute, bMustAuthenticate, bEncrypted, sEncryptedFuncGroups ) ;
	return( bCanWrite ) ;
}


m4bool_t	ClT3AccessProvider :: CanExecute( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	m4bool_t	bCanRead = M4_FALSE ;
	m4bool_t	bCanWrite = M4_FALSE ;
	m4bool_t	bCanExecute = M4_FALSE ;
	m4bool_t	bMustAuthenticate = M4_FALSE ;
	m4bool_t	bEncrypted = M4_FALSE ;
	string		sEncryptedFuncGroups;

	_FindAny( ai_poChannel, ai_pccNode, ai_pccItem, bCanRead, bCanWrite, bCanExecute, bMustAuthenticate, bEncrypted, sEncryptedFuncGroups ) ;
	return( bCanExecute ) ;
}


m4bool_t	ClT3AccessProvider :: MustAuthenticate( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	m4bool_t	bCanRead = M4_FALSE ;
	m4bool_t	bCanWrite = M4_FALSE ;
	m4bool_t	bCanExecute = M4_FALSE ;
	m4bool_t	bMustAuthenticate = M4_FALSE ;
	m4bool_t	bEncrypted = M4_FALSE ;
	string		sEncryptedFuncGroups;

	_FindAny( ai_poChannel, ai_pccNode, ai_pccItem, bCanRead, bCanWrite, bCanExecute, bMustAuthenticate, bEncrypted, sEncryptedFuncGroups ) ;
	return( bMustAuthenticate ) ;
}


m4bool_t	ClT3AccessProvider :: IsEncrypted( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, string &ao_sEncryptedFuncGroups )
{
	m4bool_t	bCanRead = M4_FALSE ;
	m4bool_t	bCanWrite = M4_FALSE ;
	m4bool_t	bCanExecute = M4_FALSE ;
	m4bool_t	bMustAuthenticate = M4_FALSE ;
	m4bool_t	bEncrypted = M4_FALSE ;

	_FindAny( ai_poChannel, ai_pccNode, ai_pccItem, bCanRead, bCanWrite, bCanExecute, bMustAuthenticate, bEncrypted, ao_sEncryptedFuncGroups ) ;
	return( bEncrypted ) ;
}

static m4uint32_t getBitMask( m4bool_t ai_bValue, m4int32_t ai_iShift)
{
	m4uint8_t iValue = (ai_bValue == M4_TRUE) ? 1 : 0;
	return iValue << ai_iShift;
}

m4uint32_t	ClT3AccessProvider :: GetSecurityMask( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem )
{
	m4bool_t	bCanRead = M4_FALSE ;
	m4bool_t	bCanWrite = M4_FALSE ;
	m4bool_t	bCanExecute = M4_FALSE ;
	m4bool_t	bMustAuthenticate = M4_FALSE ;
	m4bool_t	bEncrypted = M4_FALSE ;
	string		sEncryptedFuncGroups ;

	_FindAny( ai_poChannel, ai_pccNode, ai_pccItem, bCanRead, bCanWrite, bCanExecute, bMustAuthenticate, bEncrypted, sEncryptedFuncGroups ) ;
	
	return(	getBitMask(bCanRead ,			0) | 
			getBitMask(bCanWrite,			1) |
			getBitMask(bCanExecute,			2) |
			getBitMask(bMustAuthenticate,	3) |
			getBitMask(bEncrypted,			4));
}

ClNode*		ClT3AccessProvider :: LoadGroup( m4pcchar_t ai_pccGroup, ClChannel *ai_poChannel, const ClItemAccessMapKey &ai_roKey )
{

	m4return_t			iResult = M4_ERROR ;
	m4pchar_t			pcRole = NULL ;
	m4pchar_t			pcOrganization = NULL ;
	m4double_t			dLanguage = M4CL_LANGUAGE_NONE ;
	ClChannelManager	*poChannelManager = NULL ;
	ClNode				*poNode = NULL ;
	ClAccess			*poAccess = NULL ;
	ClChannel			*poChannel = NULL ;
	IStackInterface		*poStack = NULL ;
	m4VariantType		vValue ;


	if( m_poReaderChannel != NULL )
	{
		iResult = m_poReaderChannel->Role_ID.Get( pcRole ) ;

		if( iResult != M4_SUCCESS )
		{
			return( NULL ) ;
		}

		iResult = m_poReaderChannel->Organization.Get( pcOrganization ) ;

		if( iResult != M4_SUCCESS )
		{
			return( NULL ) ;
		}

		iResult = m_poReaderChannel->Data_Language.Get( dLanguage ) ;

		if( iResult != M4_SUCCESS )
		{
			return( NULL ) ;
		}

		if( m4language_t( dLanguage ) != ai_roKey.GetLanguage() || strcmp( pcOrganization, ai_roKey.GetOrganization() ) != 0 || strcmp( pcRole, ai_roKey.GetRole() ) != 0 )
		{
			_Reset() ;
		}
	}

	poChannelManager = ai_poChannel->GetpChannelManager() ;

	if( poChannelManager == NULL )
	{
		return( NULL ) ;
	}

	if( m_poReaderAccess == NULL )
	{
		iResult = poChannelManager->CreateChannel( m_poReaderChannel ) ;

		if( iResult != M4_SUCCESS || m_poReaderChannel == NULL )
		{
			return( NULL ) ;
		}

		iResult = m_poReaderChannel->Role_ID.Set( (m4pchar_t) ai_roKey.GetRole() ) ;
			
		if( iResult != M4_SUCCESS )
		{
			m_poReaderChannel->Destroy() ;
			m_poReaderChannel = NULL ;
			return( NULL ) ;
		}

		iResult = m_poReaderChannel->Organization.Set( (m4pchar_t) ai_roKey.GetOrganization() ) ;
			
		if( iResult != M4_SUCCESS )
		{
			m_poReaderChannel->Destroy() ;
			m_poReaderChannel = NULL ;
			return( NULL ) ;
		}

		iResult = m_poReaderChannel->Data_Language.Set( ai_roKey.GetLanguage() ) ;
			
		if( iResult != M4_SUCCESS )
		{
			m_poReaderChannel->Destroy() ;
			m_poReaderChannel = NULL ;
			return( NULL ) ;
		}

		iResult = m_poReaderChannel->BuildFromIdNoSec( "M4RSC_CLIENT_USE_READER" ) ;

		if( iResult != M4_SUCCESS )
		{
			m_poReaderChannel->Destroy() ;
			m_poReaderChannel = NULL ;
			return( NULL ) ;
		}

		/* Bug 0269461
		Se quita el canal del channel manager para que no se serialize.
		*/
		poChannelManager->Channel.Detach( m_poReaderChannel ) ;

		iResult = m_poReaderChannel->CreateAccess( m_poReaderAccess ) ;

		if( iResult != M4_SUCCESS || m_poReaderAccess == NULL )
		{
			m_poReaderChannel->Destroy() ;
			m_poReaderChannel = NULL ;
			return( NULL ) ;
		}
	}

	poNode = &( m_poReaderAccess->Node[ "M4RSC_CLIENT_USE_READER"] ) ;

	if( poNode == NULL )
	{
		return( NULL ) ;
	}

	poStack = &( m_poReaderAccess->GetpExecutor()->Stack ) ;

	if( poStack == NULL )
	{
		return( NULL ) ;
	}

	vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
	vValue.Data.PointerChar = (m4pchar_t) ai_pccGroup ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
	vValue.Data.PointerChar = (m4pchar_t) ai_poChannel->GetpChannelDef()->Id() ;
	iResult = poStack->Push( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	iResult = poNode->RecordSet.Item[ "READ" ].Call( NULL, 3 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	/* Bug 0269461
	Se quita el canal del channel manager para que no se serialize.
	*/
	iResult = m_poReaderChannel->Level2.m_oInstanceMap.Find( "M4RSC_CLIENT_USE", &poAccess, &poChannel ) ;

	if( iResult == M4_SUCCESS && poChannel != NULL )
	{
		poChannelManager->Channel.Detach( poChannel ) ;
	}

	return( poNode ) ;
}


void	ClT3AccessProvider :: _Reset( void )
{
	if ( m_poReaderAccess != NULL ) 
	{
		m_poReaderAccess->Destroy() ;
		m_poReaderAccess = NULL ;
	}
	
	if ( m_poReaderChannel != NULL ) 
	{
		m_poReaderChannel->Destroy() ;
		m_poReaderChannel = NULL ;
	}
}


const mFunctionalGroupsSet_t* ClT3AccessProvider :: _LoadGroups( ClChannel *ai_poChannel )
{

	// checks if the functional groups are loaded

	// gets the channel role
	m4pchar_t pcRole;
	ai_poChannel->Role_ID.Get( pcRole ) ;
	mFunctionalGroupsMap_t::iterator itEntry = m_oFunctionalGroupsMap.find( pcRole );
	if ( itEntry != m_oFunctionalGroupsMap.end() )
	{
		return & ( (*itEntry).second );
	}

	// adds a new entry to the map
	mFunctionalGroupsSet_t oDummy;
	m_oFunctionalGroupsMap.insert(mFunctionalGroupsMap_t::value_type(pcRole, oDummy ));
	

	// peeks the map object reference
	mFunctionalGroupsSet_t &oFunctionalGroupsSet = ( *m_oFunctionalGroupsMap.find( pcRole ) ).second;

	// sets the status
	m4return_t			iResult;
	ClChannelManager	*poChannelManager = NULL ;
	ClChannel			*poChannel = NULL ;
	ClAccess			*poAccess = NULL ;
	ClNode				*poNode = NULL ;
	IStackInterface		*poStack = NULL ;
	m4VariantType		vValue ;

	M4ItemAccessReleaser releaser( poChannel, poAccess );

	poChannelManager = ai_poChannel->GetpChannelManager() ;

	if( poChannelManager == NULL )
	{
		return( NULL ) ;
	}

	iResult = poChannelManager->CreateChannel( poChannel ) ;

	if( iResult != M4_SUCCESS || poChannel == NULL )
	{
		return( NULL ) ;
	}

	iResult = poChannel->Role_ID.Set( pcRole ) ;
		
	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	m4pchar_t pcOrganization;
	ai_poChannel->Organization.Get( pcOrganization ) ;
	iResult = poChannel->Organization.Set( pcOrganization ) ;
		
	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	m4double_t dLanguage;
	ai_poChannel->Data_Language.Get( dLanguage ) ;
	iResult = poChannel->Data_Language.Set( dLanguage ) ;
		
	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	iResult = poChannel->BuildFromIdNoSec( "ESS_SBP_T3" ) ;

	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	iResult = poChannel->CreateAccess( poAccess ) ;

	if( iResult != M4_SUCCESS || poAccess == NULL )
	{
		return( NULL ) ;
	}

	poNode = &( poAccess->Node[ "ESS_CODE_DEF"] ) ;

	if( poNode == NULL )
	{
		return( NULL ) ;
	}

	poStack = &( poAccess->GetpExecutor()->Stack ) ;

	if( poStack == NULL )
	{
		return( NULL ) ;
	}

	iResult = poNode->RecordSet.Item[ "LOAD_SEC_TASKS" ].Call( ) ;

	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	iResult = poStack->Pop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( NULL ) ;
	}

	// gets the values
	// iterates
	for ( int i = 0 ; i < poNode->RecordSet.Count() ; ++ i)
	{
		// moves to the record
		poNode->RecordSet.Register.MoveTo(i);

		// gets the value
		iResult = poNode->RecordSet.Register.Item[ "ID_GROUP" ].Value.Get( vValue ) ;

		if( iResult != M4_SUCCESS )
		{
			return( NULL ) ;
		}

		if( vValue.Type == M4CL_CPP_TYPE_NULL ) 
		{
			// there is no functional group defined for this task. Ignoring
			continue;
		}

		if( vValue.Type != M4CL_CPP_TYPE_STRING_VAR || vValue.Data.PointerChar == NULL )
		{
			return NULL;
		}

		// retrieves the value
		oFunctionalGroupsSet.insert( vValue.Data.PointerChar );
	}

	// returns the object pointer
	return &oFunctionalGroupsSet;
}


void	ClT3AccessProvider :: _FindAny( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4bool_t &ao_bCanRead, m4bool_t &ao_bCanWrite, 
									   m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, m4bool_t &ao_bEncrypted, string &ao_sEncryptedFuncGroups)
{
	// intentamos primero buscar en la caché a ver si el item ha sido proceso anteriormente
	ClChannelItemAccessKey oCacheKey(ai_poChannel->GetpChannelDef()->Id(), ai_pccNode, ai_pccItem);
	mChannelItemAccess_t::iterator oCacheIterator = m_oChannelItemMap.find(oCacheKey);
	if ( oCacheIterator != m_oChannelItemMap.end() ) 
	{
		// el valor está en la cache
		ClItemAccess oItemAccess;
		oCacheIterator->second.GetItemAccess(oItemAccess);
		m4uint16_t uiIdLevel = 0;
		oItemAccess.GetValues( ao_bCanRead, ao_bCanWrite, ao_bCanExecute, ao_bMustAuthenticate, uiIdLevel, ao_bEncrypted ) ;
		oCacheIterator->second.GetEncryptedFuncGroups( ao_sEncryptedFuncGroups );

		return;
	}

	// no lo hemos encontrado en la caché. Hacemos la búsqueda por primera vez
	m4bool_t bCanRead(M4_FALSE), bCanWrite(M4_FALSE), bCanExecute(M4_FALSE), bMustAuthenticate(M4_TRUE), bEncrypted(M4_FALSE);

	const mFunctionalGroupsSet_t* poFunctionalGroupsSet = _LoadGroups(ai_poChannel);

	if ( poFunctionalGroupsSet == NULL )
	{
		return;
	}

	m4uint16_t uiMinIdLevel = M4_ITEM_INFO_NOT_FOUND;

	m4bool_t bFound = M4_FALSE;

	for (mFunctionalGroupsSet_t::const_iterator it = poFunctionalGroupsSet->begin(); it != poFunctionalGroupsSet->end(); ++it)
	{
		m4pcchar_t pccGroup = (*it).c_str();
		m4uint16_t uiIdLevel = 0;

		m4bool_t btCanRead(M4_FALSE), btCanWrite(M4_FALSE), btCanExecute(M4_FALSE), btMustAuthenticate(M4_TRUE), btEncrypted(M4_FALSE);
		_Find( pccGroup, ai_poChannel, ai_pccNode, ai_pccItem, btCanRead, btCanWrite, btCanExecute, btMustAuthenticate, uiIdLevel, btEncrypted ); 

		if ( uiIdLevel == M4_ITEM_INFO_NOT_FOUND ) 
		{
			continue;
		}

		if ( bFound == M4_FALSE )
		{
			bFound = M4_TRUE;
		}

		// if we're encrypted we have to register the functional group
		if ( btEncrypted == M4_TRUE )
		{
			if (ao_sEncryptedFuncGroups.size() != 0) 
			{
				ao_sEncryptedFuncGroups.append(",");
			}
			ao_sEncryptedFuncGroups.append(pccGroup);
		}

		if ( uiMinIdLevel == -1 || uiIdLevel < uiMinIdLevel )
		{
			uiMinIdLevel = uiIdLevel;
		}

		if ( uiIdLevel == uiMinIdLevel ) 
		{ 
			// lets update the current state
			bCanRead = btCanRead == M4_TRUE ? M4_TRUE : bCanRead;
			bCanWrite = btCanWrite == M4_TRUE ? M4_TRUE : bCanWrite;
			bCanExecute = btCanExecute == M4_TRUE ? M4_TRUE : bCanExecute;
			bMustAuthenticate = btMustAuthenticate == M4_FALSE ? M4_FALSE : bMustAuthenticate;
			bEncrypted = btEncrypted == M4_TRUE ? M4_TRUE : bEncrypted;
		}
	}

	// sets default values if not found
	if ( bFound == M4_FALSE )
	{
		bCanRead = M4_FALSE;
		bCanWrite = M4_FALSE;
		bCanExecute = M4_FALSE;
		bMustAuthenticate = M4_FALSE;
		bEncrypted = M4_FALSE;
		ao_sEncryptedFuncGroups = string();
	}

	// actualizamos la cache
	ClItemAccess oItemAccess( bCanRead, bCanWrite, bCanExecute, bMustAuthenticate, uiMinIdLevel, bEncrypted ) ;
	ClCachedItemAccess oCachedItemAccess( oItemAccess, ao_sEncryptedFuncGroups );
	m_oChannelItemMap.insert (mChannelItemAccess_t::value_type(oCacheKey, oCachedItemAccess ));

	// ajusta los parámetros de salida
	oItemAccess.GetValues( ao_bCanRead, ao_bCanWrite, ao_bCanExecute, ao_bMustAuthenticate, uiMinIdLevel, ao_bEncrypted ) ;
}


void	ClT3AccessProvider :: _Find( m4pcchar_t ai_pccGroup, ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4bool_t &ao_bCanRead, m4bool_t &ao_bCanWrite, m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, m4uint16_t &ao_uiIdLevel, m4bool_t &ao_bEncrypted )
{

	ClT3AccessKey			oKey ;
	ClT3Access				oValue ;
	mT3Access_t::iterator	it ;


	ao_bCanRead = M4_FALSE ;
	ao_bCanWrite = M4_FALSE ;
	ao_bCanExecute = M4_FALSE ;
	ao_bMustAuthenticate = M4_TRUE ;
	ao_uiIdLevel = M4_ITEM_INFO_NOT_FOUND ;
	ao_bEncrypted = M4_TRUE;

	oKey.Init( ai_pccGroup, ai_poChannel->GetpChannelDef()->Id() ) ;

	it = m_oAccessMap.find( oKey ) ;

	if( it == m_oAccessMap.end() )
	{
		oValue.Find( this, oKey.GetGroup(), ai_poChannel, ai_pccNode, ai_pccItem, ao_bCanRead, ao_bCanWrite, ao_bCanExecute, ao_bMustAuthenticate, ao_uiIdLevel, ao_bEncrypted ) ;
		m_oAccessMap.insert(mT3Access_t::value_type(oKey, oValue));
	}
	else
	{
		it->second.Find( this, oKey.GetGroup(), ai_poChannel, ai_pccNode, ai_pccItem, ao_bCanRead, ao_bCanWrite, ao_bCanExecute, ao_bMustAuthenticate, ao_uiIdLevel, ao_bEncrypted ) ;
	}
}

