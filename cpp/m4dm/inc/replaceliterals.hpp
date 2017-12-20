//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           replaceliterals.hpp
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


#ifndef _REPLACELITERALS_HPP_
#define _REPLACELITERALS_HPP_

#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "syncro.hpp"
#include "m4serial.hpp"
#include "cachebase.hpp"
#include "m4cachetypes.hpp"



//=================================================================================
// Declaraciones
//=================================================================================

class ClChannelManager;
class ClAccess;
class ClCacheDirectory;



//=================================================================================
// Clase de parejas de cadenas a substituir
//=================================================================================

class	ClLiteralPair
{
public:

	ClLiteralPair( void ) ;
	ClLiteralPair( m4pcchar_t ai_pccLeft, m4pcchar_t ai_pccRight ) ;
	~ClLiteralPair( void ) ;

	m4return_t	Serialize( ClGenericIODriver &IOD ) ;
	m4return_t	DeSerialize( ClGenericIODriver &IOD ) ;

protected:

	void	_Reset( void ) ;

	m4pchar_t	m_pccLeft ;
	m4pchar_t	m_pccRight ;

friend class ClLiteralCache;
};

// Vector de parejas de literales a sustituir
typedef vector< ClLiteralPair* >		vLiteralPairs_t ;



//=================================================================================
// Clase de claves y lista de parejas a sustituir
//=================================================================================

class	ClLiteralKey
{
public:

	ClLiteralKey( void ) ;
	ClLiteralKey( m4pcchar_t ai_pccKey, m4language_t ai_iLanguage ) ;
	~ClLiteralKey( void ) ;

	m4return_t	Serialize( ClGenericIODriver &IOD ) ;
	m4return_t	DeSerialize( ClGenericIODriver &IOD ) ;

protected:

	void	_Reset( void ) ;

	m4pchar_t		m_pccKey ;
	m4language_t	m_iLanguage ;
	vLiteralPairs_t	m_vPairs ;

friend class ClLiteralCache;
};

// Vector de claves de literales a substituir
typedef vector< ClLiteralKey* >	vLiteralKeys_t;



//=================================================================================
// Clase de organización y lista de claves a sustituir
//=================================================================================

class	ClLiteralOrganization
{
public:

	ClLiteralOrganization( void ) ;
	ClLiteralOrganization( m4pcchar_t ai_pccOrganization ) ;
	~ClLiteralOrganization( void ) ;

	m4return_t	Serialize( ClGenericIODriver &IOD ) ;
	m4return_t	DeSerialize( ClGenericIODriver &IOD ) ;

protected:

	void	_Reset( void ) ;

	m4pchar_t		m_pccOrganization ;
	m4date_t		m_dtVersion ;
	vLiteralKeys_t	m_vKeys ;

friend class ClLiteralCache;

};

// Vector organizaciones de literales a substituir
typedef vector< ClLiteralOrganization* >	vLiteralOrganizations_t;


//=================================================================================
// Clase de caché de literales
//=================================================================================

class	ClLiteralCache : public ClBaseCacheInterface
{
public:

	ClLiteralCache( void ) ;
	~ClLiteralCache( void ) ;

	m4return_t	ReplaceLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccInput, m4uint16_t ai_iLength, m4pchar_t& ao_rpcOutput, m4pcchar_t ai_pccOrganization, m4bool_t& ao_rbModified ) ;
	m4return_t	LiteralsVersion ( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization, m4date_t& ao_rdtVersion ) ;
	m4return_t	CleanLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization ) ;
	m4return_t	CleanAllLiterals( void ) ;
	m4return_t	SetLiteralsCacheDirectory( ClCacheDirectory *ai_poCacheDirectory ) ;

	// Api de caché
	void		Init( void ) {}
	m4return_t	PersistCache( void ) { return M4_SUCCESS; }
	m4return_t	DePersistCache( void ) { return M4_SUCCESS; }
	m4return_t	DeleteCache( void ) { return M4_SUCCESS; }
	m4return_t	RemoveObjectById( m4pchar_t ai_pcId, m4puint32_t ai_piRemovedCount = NULL ) { return M4_SUCCESS; }
	m4return_t	GetNextId( m4pchar_t& ao_pcId, void*& ai_poIterator ) { return M4_SUCCESS; }
	m4return_t	Dump( ClDumpCacheVisitor& ai_oVisitor ) { return M4_SUCCESS; }

	m4uint16_t	GetType( void )
	{
		return( M4_CACHE_LITERALS_CACHE_TYPE ) ;
	}

	m4return_t	GetNumObjects( m4uint32_t& ao_iNumObjects )
	{
		ao_iNumObjects = 0 ;
		return( M4_SUCCESS ) ;
	}

	m4return_t	RemoveAll( m4date_t ai_dDate = 0 ) ;
	m4return_t	RemoveObjectByIdAndVersion( m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType ) ;

	void		Reload( void ) {}

protected:

	void		_Reset( void ) ;

	m4return_t	_Serialize( ClGenericIODriver &IOD ) ;
	m4return_t	_DeSerialize( ClGenericIODriver &IOD ) ;

	m4return_t	_SetDirectory( m4pcchar_t ai_pccDirectory ) ;
	m4return_t	_WriteToFile( void ) ;
	m4return_t	_ReadFromFile( void ) ;

	ClLiteralOrganization*	_GetOrganization( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization ) ;
	m4return_t				_LoadLiterals( ClAccess *ai_poAccess, m4pcchar_t ai_pccOrganization, vLiteralKeys_t& ao_rvLiteralKeys, m4date_t& ao_rdtVersion ) ;
	m4return_t				_LoadLiterals( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization, vLiteralKeys_t& ao_rvLiteralKeys, m4date_t& ao_rdtVersion ) ;


	m4pchar_t				m_pccFileName ;
	ClMutex					m_oMutex ;
	vLiteralOrganizations_t	m_vLiteralOrganizations ;
};



m4return_t	M4_DECL_M4DM	M4ReplaceLiterals ( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccInput, m4uint16_t ai_iLength, m4pchar_t& ao_rpcOutput, m4pcchar_t ai_pccOrganization, m4bool_t& ao_rbModified ) ;
m4return_t	M4_DECL_M4DM	M4ReplaceLiterals ( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccInput, m4pchar_t& ao_rpcOutput, m4pcchar_t ai_pccOrganization, m4bool_t& ao_rbModified ) ;
m4return_t	M4_DECL_M4DM	M4LiteralsVersion ( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization, m4date_t& ao_rdtVersion ) ;
m4return_t	M4_DECL_M4DM	M4CleanLiterals   ( ClChannelManager *ai_poChannelManager, m4pcchar_t ai_pccOrganization ) ;
m4return_t	M4_DECL_M4DM	M4CleanAllLiterals( void ) ;
m4return_t	M4_DECL_M4DM	M4SetLiteralsCacheDirectory( ClCacheDirectory *ai_poCacheDirectory ) ;

#endif	// _REPLACELITERALS_HPP_

