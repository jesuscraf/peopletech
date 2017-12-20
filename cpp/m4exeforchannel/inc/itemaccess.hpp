//	==============================================================================
//
//	 (c) Copyright  1991-2013 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              itemaccess.hpp
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


#ifndef _ITEMACCESS_HPP_
#define _ITEMACCESS_HPP_

#include "m4stl.hpp"
#include "m4types.hpp"
#include "channel.hpp"
#include "register.hpp"



class ClT3AccessProvider;



//=================================================================================
// Estructuras para los permisos de acceso
//=================================================================================

// Clase de permisos de acceso de un ítem
class	ClItemAccess
{
public:

	ClItemAccess( void )
	{
		m_bCanRead = M4_FALSE ;
		m_bCanWrite = M4_FALSE ;
		m_bCanExecute = M4_FALSE ;
		m_bMustAuthenticate = M4_TRUE ;
		m_bEncrypted = M4_TRUE ;
		m_uiIdLevel = 0 ;
	}

	ClItemAccess( m4bool_t ai_bCanRead, m4bool_t ai_bCanWrite, m4bool_t ai_bCanExecute, m4bool_t ai_bMustAuthenticate, m4uint16_t ai_uiIdLevel, m4bool_t ai_bEncrypted )
	{
		m_bCanRead = ai_bCanRead ;
		m_bCanWrite = ai_bCanWrite ;
		m_bCanExecute = ai_bCanExecute ;
		m_bMustAuthenticate = ai_bMustAuthenticate ;
		m_uiIdLevel = ai_uiIdLevel ;
		m_bEncrypted = ai_bEncrypted ;
	}

	ClItemAccess( const ClItemAccess &ai_roCopy )
	{
		m_bCanRead = ai_roCopy.m_bCanRead ;
		m_bCanWrite = ai_roCopy.m_bCanWrite ;
		m_bCanExecute = ai_roCopy.m_bCanExecute ;
		m_bMustAuthenticate = ai_roCopy.m_bMustAuthenticate ;
		m_uiIdLevel = ai_roCopy.m_uiIdLevel;
		m_bEncrypted = ai_roCopy.m_bEncrypted;
	}

	void	Init( ClRegister &ai_roRegister ) ;

	void GetValues( m4bool_t &ao_bCanRead, m4bool_t &ao_bCanWrite, m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, m4uint16_t &ao_uiIdLevel, m4bool_t &ao_bEncrypted ) const
	{
		ao_bCanRead = m_bCanRead ;
		ao_bCanWrite = m_bCanWrite ;
		ao_bCanExecute = m_bCanExecute ;
		ao_bMustAuthenticate = m_bMustAuthenticate ;
		ao_uiIdLevel = m_uiIdLevel;
		ao_bEncrypted = m_bEncrypted ;
	}

private:

	m4bool_t	m_bCanRead ;
	m4bool_t	m_bCanWrite ;
	m4bool_t	m_bCanExecute ;
	m4bool_t	m_bMustAuthenticate ;
	m4uint16_t	m_uiIdLevel;
	m4bool_t	m_bEncrypted ;
};


// Clase clave de permisos de acceso de un ítem por nodo e ítem
class	ClItemAccessKey
{
public:

	ClItemAccessKey( void )
	{
		*m_acNode = '\0' ;
		*m_acItem = '\0' ;
	}

	ClItemAccessKey( const ClItemAccessKey &ai_roCopy )
	{
		strcpy( m_acNode, ai_roCopy.m_acNode ) ;
		strcpy( m_acItem, ai_roCopy.m_acItem ) ;
	}

	void	Init( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	void	Init( ClRegister &ai_roRegister ) ;

	friend m4bool_t operator < ( const ClItemAccessKey &lhc, const ClItemAccessKey &rhc ) ;
	friend m4bool_t operator == ( const ClItemAccessKey &lhc, const ClItemAccessKey &rhc ) ;

private:

	m4char_t	m_acNode[ M4CL_MAX_NODE_ID + 1 ] ;
	m4char_t	m_acItem[ M4CL_MAX_ITEM_ID + 1 ] ;
};


// Mapa de permisos de acceso por nodo, ítem
typedef map<ClItemAccessKey, ClItemAccess, less<ClItemAccessKey> >	mItemAccess_t;


// Clase de mapa de permisos de acceso de un ítem
class	ClItemAccessMap
{
public:

	ClItemAccessMap( void )
	{
		m_bOwner = M4_TRUE ;
		m_poAccessMap = NULL ;
	}

	ClItemAccessMap( const ClItemAccessMap &ai_roCopy )
	{
		m_bOwner = ai_roCopy.m_bOwner ;
		m_poAccessMap = ai_roCopy.m_poAccessMap ;
		((ClItemAccessMap*)&ai_roCopy)->m_bOwner = M4_FALSE ;
	}

	~ClItemAccessMap( void )
	{
		if( m_poAccessMap != NULL && m_bOwner == M4_TRUE )
		{
			delete( m_poAccessMap ) ;
		}
	}

	m4return_t	Init( ClRegister &ai_roRegister ) ;

	void	Find( m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4bool_t &ao_bCanRead, m4bool_t 
				&ao_bCanWrite, m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, 
			m4uint16_t &ao_uiIdLevel, m4bool_t &ao_bEncrypted ) ;

private:

	m4bool_t		m_bOwner ;
	mItemAccess_t	*m_poAccessMap ;
};

// Clase clave de permisos de acceso de un ítem por objecto, nodo e ítem
class	ClChannelItemAccessKey
{
public:

	ClChannelItemAccessKey( const ClChannelItemAccessKey &ai_roCopy )
	{
		strcpy( m_acChannel, ai_roCopy.m_acChannel ) ;
		strcpy( m_acNode, ai_roCopy.m_acNode ) ;
		strcpy( m_acItem, ai_roCopy.m_acItem ) ;
	}

	ClChannelItemAccessKey( m4pcchar_t ai_pccChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;

	friend m4bool_t operator < ( const ClChannelItemAccessKey &lhc, const ClChannelItemAccessKey &rhc ) ;
	friend m4bool_t operator == ( const ClChannelItemAccessKey &lhc, const ClChannelItemAccessKey &rhc ) ;

private:

	m4char_t	m_acChannel[ M4CL_MAX_T3_ID   + 1 ] ;
	m4char_t	m_acNode   [ M4CL_MAX_NODE_ID + 1 ] ;
	m4char_t	m_acItem   [ M4CL_MAX_ITEM_ID + 1 ] ;
};

// Clase contenedora de persismos de item
class ClCachedItemAccess
{
	// private
private:
	ClItemAccess	m_oItemAccess;
	string			m_sEncryptedFuncGroups;

public:
	ClCachedItemAccess ( ClItemAccess ai_oItemAccess, string &ai_sEncryptedFuncGroups );
	void GetItemAccess( ClItemAccess &ao_oItemAccess );
	void GetEncryptedFuncGroups ( string &ao_sEncryptedFuncGroups );
};

// Mapa de permisos de acceso por objeto, nodo, ítem
typedef map<ClChannelItemAccessKey, ClCachedItemAccess, less<ClChannelItemAccessKey> >mChannelItemAccess_t;

// Clase de clave de mapa de permisos de acceso de un ítem por ámbito
class	ClItemAccessMapKey
{
public:

	ClItemAccessMapKey( void )
	{
		*m_acRole = '\0' ;
		*m_acOrganization = '\0' ;
		m_iLanguage = M4CL_LANGUAGE_NONE ;
	}

	ClItemAccessMapKey( const ClItemAccessMapKey &ai_roCopy )
	{
		strcpy( m_acRole, ai_roCopy.m_acRole ) ;
		strcpy( m_acOrganization, ai_roCopy.m_acOrganization ) ;
		m_iLanguage = ai_roCopy.m_iLanguage ;
	}

	void	Init( ClChannel *ai_poChannel ) ;

	friend m4bool_t operator < ( const ClItemAccessMapKey &lhc, const ClItemAccessMapKey &rhc ) ;
	friend m4bool_t operator == ( const ClItemAccessMapKey &lhc, const ClItemAccessMapKey &rhc ) ;

	m4pcchar_t	GetRole( void ) const
	{
		return( m_acRole ) ;
	}

	m4pcchar_t	GetOrganization( void ) const
	{
		return( m_acOrganization ) ;
	}

	m4language_t	GetLanguage( void ) const
	{
		return( m_iLanguage ) ;
	}

private:

	m4char_t		m_acRole[ M4CL_MAX_ROLE_ID + 1 ] ;
	m4char_t		m_acOrganization[ M4CL_MAX_ORG_ID + 1 ] ;
	m4language_t	m_iLanguage ;
};


// Mapa de permisos de acceso por ámbito
typedef map<ClItemAccessMapKey, ClItemAccessMap, less<ClItemAccessMapKey> >	mItemAccessMap_t;



// Clase de permisos de acceso de un Meta4Object
class	ClT3Access
{
public:

	ClT3Access( void )
	{
		m_bOwner = M4_TRUE ;
		m_poSingleAccess = NULL ;
		m_poMultiAccess = NULL ;
	}

	ClT3Access( const ClT3Access &ai_roCopy )
	{
		m_bOwner = ai_roCopy.m_bOwner ;
		m_poSingleAccess = ai_roCopy.m_poSingleAccess ;
		m_poMultiAccess = ai_roCopy.m_poMultiAccess ;
		((ClT3Access*)&ai_roCopy)->m_bOwner = M4_FALSE ;
	}

	~ClT3Access( void )
	{
		if( m_poSingleAccess != NULL && m_bOwner == M4_TRUE )
		{
			delete( m_poSingleAccess ) ;
		}

		if( m_poMultiAccess != NULL && m_bOwner == M4_TRUE )
		{
			delete( m_poMultiAccess ) ;
		}
	}

	void	Find( ClT3AccessProvider *ai_poAcessProvider, m4pcchar_t ai_pccGroup, ClChannel *ai_poChannel, 
		m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4bool_t &ao_bCanRead, 
		m4bool_t &ao_bCanWrite, m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, 
		m4uint16_t &ao_uiIdLevel, m4bool_t &ao_bEncrypted ) ;

private:

	m4return_t	_Load( ClT3AccessProvider *ai_poAcessProvider, m4pcchar_t ai_pccGroup, ClChannel *ai_poChannel, const ClItemAccessMapKey &ai_roKey, ClItemAccessMap& ao_roMap ) ;

	m4bool_t			m_bOwner ;
	ClItemAccessMap		*m_poSingleAccess ;
	mItemAccessMap_t	*m_poMultiAccess ;
};


// Clase clave de permisos de acceso de un Meta4Object por grupo y Meta4Object
class	ClT3AccessKey
{
public:

	ClT3AccessKey( void )
	{
		*m_acGroup = '\0' ;
		*m_acT3 = '\0' ;
	}

	ClT3AccessKey( const ClT3AccessKey &ai_roCopy )
	{
		strcpy( m_acGroup, ai_roCopy.m_acGroup ) ;
		strcpy( m_acT3, ai_roCopy.m_acT3 ) ;
	}

	void	Init( m4pcchar_t ai_pccGroup, m4pcchar_t ai_pccT3 ) ;

	m4pcchar_t	GetGroup( void ) const
	{
		return( m_acGroup ) ;
	}

	m4pcchar_t	GetT3( void ) const
	{
		return( m_acT3 ) ;
	}

	friend m4bool_t operator < ( const ClT3AccessKey &lhc, const ClT3AccessKey &rhc ) ;
	friend m4bool_t operator == ( const ClT3AccessKey &lhc, const ClT3AccessKey &rhc ) ;

private:

	m4char_t	m_acGroup[ M4CL_MAX_T3_ID + 1 ] ;
	m4char_t	m_acT3[ M4CL_MAX_T3_ID + 1 ] ;
};




// Mapa de permisos de acceso por grupo y Meta4Object
typedef map< ClT3AccessKey, ClT3Access, less<ClT3AccessKey> >	mT3Access_t;
typedef set< string > mFunctionalGroupsSet_t;
typedef map< string, mFunctionalGroupsSet_t >	mFunctionalGroupsMap_t;


// Clase de mapa de permisos de acceso de un Meta4Object
class	ClT3AccessProvider
{
public:

	ClT3AccessProvider( void )
	{
		m_poReaderChannel = NULL ;
		m_poReaderAccess = NULL ;
	}

	~ClT3AccessProvider( void ) ;

	m4bool_t	CanRead( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	m4bool_t	CanWrite( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	m4bool_t	CanExecute( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	m4bool_t	MustAuthenticate( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;
	m4bool_t	IsEncrypted( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, string &ao_sFuncGroups ) ;
	m4uint32_t	GetSecurityMask( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem ) ;

	ClNode*		LoadGroup( m4pcchar_t ai_pccGroup, ClChannel *ai_poChannel, const ClItemAccessMapKey &ai_roKey ) ;

private:

	void							_Reset( void ) ;
	void							_FindAny( ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4bool_t &ao_bCanRead, m4bool_t &ao_bCanWrite, m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, m4bool_t &ao_bEncrypted, string &ao_sEncryptedFuncGroups ) ;
	void							_Find( m4pcchar_t ai_pccGroup, ClChannel *ai_poChannel, m4pcchar_t ai_pccNode, m4pcchar_t ai_pccItem, m4bool_t &ao_bCanRead, m4bool_t &ao_bCanWrite, m4bool_t &ao_bCanExecute, m4bool_t &ao_bMustAuthenticate, m4uint16_t &ao_uiIdLevel, m4bool_t &ao_bEncrypted);
	const mFunctionalGroupsSet_t*	_LoadGroups( ClChannel *ai_poChannel ) ;

	mT3Access_t				m_oAccessMap ;
	mFunctionalGroupsMap_t	m_oFunctionalGroupsMap ;
	mChannelItemAccess_t	m_oChannelItemMap ;

	ClChannel				*m_poReaderChannel ;
	ClAccess				*m_poReaderAccess ;
};


#endif

