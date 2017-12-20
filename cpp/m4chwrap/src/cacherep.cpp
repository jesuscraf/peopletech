//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             cacherep.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             28/01/14
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CacheReplacer
//
//==============================================================================

#include "cacherep.hpp"
#include "access.hpp"
#include "cllgadap.hpp"
#include "cachedir.hpp"
#include "m4objreg.hpp"
#include "m4unicode.hpp"
#include "m4objglb.hpp"
#include "wores.hpp"
#include <direct.h>
#include "m4xmlreg.hpp"

extern int	M4IsUnicode( void );

// -----------------------------------------------------------------------------
// Helper macros.
// -----------------------------------------------------------------------------

#define METHOD_STACK_PUSH(X,Y)	do { if(m_poNode->RecordSet.Current.Item[X].StackPush(Y)!=M4_SUCCESS){return M4_ERROR;} } while (0)
#define GET_NODE_ITEM(X, Y)		do { if(m_poNode->RecordSet.Current.Item[X].Value.Get(Y)!=M4_SUCCESS){return M4_ERROR;} } while (0)
#define CHECK_EMPTY_STRING(X)	do { m4VariantType& oV=X; if(oV.Type==M4CL_M4_TYPE_NULL || (oV.Type==M4CL_CPP_TYPE_STRING_VAR && oV.Data.PointerChar==NULL)){return M4_SUCCESS;} } while (0)

// -----------------------------------------------------------------------------
// Constants.
// -----------------------------------------------------------------------------

// Registry path where the cache packages are saved in mono-install application mode.
static const char	s_acSavedPacksRegPath[]	= "SOFTWARE\\Meta4\\PeopleNet\\Machine\\CachePack\\";

// Max length of registry readable values.
static const int	s_iRegValueMaxLength	= 128;

// Cache package registry attribute: Language of the package installed.
static const char	s_acAttrLangRegKey[]	= "language";

// Cache package registry attribute: Unicode context of the package installed.
static const char	s_acAttrUnicodeRegKey[]	= "unicode";

// Cache package registry attribute: Tech version context of the package installed.
static const char	s_acAttrTechVerRegKey[]	= "techversion";

// Cache package registry attribute: Organization of the package installed.
static const char	s_acAttrOrgRegKey[]		= "organization";

// Cache package registry attribute: Change date of the package installed.
static const char	s_acAttrChgDateRegKey[]	= "changedate";


// -----------------------------------------------------------------------------
// Class ClCachePack
// -----------------------------------------------------------------------------

// Constructor.
ClCachePack::ClCachePack()
{
	m_iLanguage			= M4_LANG_NONE_ID;
	m_bUnicode			= -1;
	m_pcOrganization	= NULL;
	m_pcChangeDate		= NULL;
	m_pcPack			= NULL;
	m_pcTechVer			= NULL;
}

// Destructor.
ClCachePack::~ClCachePack()
{
	dispose();
}

// Resets the object.
void ClCachePack::Reset()
{
	m_iLanguage	= M4_LANG_NONE_ID;
	m_bUnicode	= -1;

	dispose();
}

// Language getter.
m4language_t ClCachePack::GetLanguage()
{
	return m_iLanguage;
}

// Unicode getter.
int ClCachePack::GetUnicode()
{
	return m_bUnicode;
}

// Organization getter.
m4pchar_t ClCachePack::GetOrganization()
{
	return m_pcOrganization;
}

// Change date getter.
m4pchar_t ClCachePack::GetChangeDate()
{
	return m_pcChangeDate;
}

// Package file getter.
m4pchar_t ClCachePack::GetPack()
{
	return m_pcPack;
}

// Tech version file getter.
m4pchar_t ClCachePack::GetTechVer()
{
	return m_pcTechVer;
}

// Language setter.
void ClCachePack::SetLanguage( m4language_t ai_iLanguage )
{
	m_iLanguage = ai_iLanguage;
}

// Unicode setter.
void ClCachePack::SetUnicode( int ai_iUnicode )
{
	m_bUnicode = ai_iUnicode;
}

// Organization setter.
void ClCachePack::SetOrganization( m4pcchar_t ai_pcOrganization )
{
	copyString( m_pcOrganization, ai_pcOrganization );
}

// Change date setter.
void ClCachePack::SetChangeDate( m4pcchar_t ai_pcChangeDate )
{
	copyString( m_pcChangeDate, ai_pcChangeDate );
}

// Package file setter.
void ClCachePack::SetPack( m4pcchar_t ai_pcPack )
{
	copyString( m_pcPack, ai_pcPack );
}

// Tech version file setter.
void ClCachePack::SetTechVer( m4pcchar_t ai_pcTechVer )
{
	copyString( m_pcTechVer, ai_pcTechVer );
}

// -----------------------------------------------------------------------------
// Private methods
// -----------------------------------------------------------------------------

// Frees allocated memory.
void ClCachePack::dispose()
{
	if( m_pcOrganization != NULL )
	{
		delete m_pcOrganization;
		m_pcOrganization = NULL;
	}

	if( m_pcChangeDate != NULL )
	{
		delete m_pcChangeDate;
		m_pcChangeDate = NULL;
	}

	if( m_pcPack != NULL )
	{
		delete m_pcPack;
		m_pcPack = NULL;
	}

	if( m_pcTechVer != NULL )
	{
		delete m_pcTechVer;
		m_pcTechVer = NULL;
	}
}


// Makes a copy of a string.
void ClCachePack::copyString( m4pchar_t& ai_pcTarget, m4pcchar_t ai_pcSource )
{
	size_t	iLen;

	if( ai_pcSource == NULL )
	{
		if( ai_pcTarget != NULL )
		{
			delete ai_pcTarget;
			ai_pcTarget = NULL;
		}

		return;
	}

	iLen = strlen( ai_pcSource );

	if( ai_pcTarget == NULL )
	{
		ai_pcTarget = new m4char_t[ iLen + 1 ];
	}
	else if( iLen > strlen( ai_pcTarget ) )
	{
		delete ai_pcTarget;
		ai_pcTarget = new m4char_t[ iLen + 1 ];
	}

	strcpy( ai_pcTarget, ai_pcSource );
}


// -----------------------------------------------------------------------------
// Class ClCacheReplacer
// -----------------------------------------------------------------------------

// Constructor.
ClCacheReplacer::ClCacheReplacer( ClChannelManager* ai_poChManager )
{
	m_poChManager			= NULL;
	m_poChannel				= NULL;
	m_poAccess				= NULL;
	m_poNode				= NULL;
	m_iEnvLang				= M4_LANG_NONE_ID;
	m_bEnvUnicode			= -1;
	m_pcEnvTechVer			= NULL;
	m_pcEnvCachePath		= NULL;
	
	Initialize( ai_poChManager );
}


// Destructor.
ClCacheReplacer::~ClCacheReplacer()
{
	if( m_poAccess != NULL )
	{
		m_poAccess->Destroy();
	}

	if( m_poChannel	!= NULL )
	{
		m_poChannel->Destroy();
	}

	if( m_pcEnvCachePath != NULL )
	{
		delete m_pcEnvCachePath;
	}
}


// Replace the local cache with the more recent published cache package.
m4return_t ClCacheReplacer::ReplaceCache()
{
	m4return_t	iRet;
	ClCachePack	oLocalPack, oNewPack;
	m4bool_t	bAvailable = M4_FALSE;


	// Nothing to do.
	if( m_poChManager == NULL )
	{
		return M4_ERROR;
	}


	// Retrieves the attributes of the cache package currently installed.
	iRet = getLocalPack( oLocalPack );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	
	// Creates the underlying M4Object.
	iRet = buildChannel();

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	
	// Checks for a more recent published cache package.
	iRet = checkForNewPack( oLocalPack, oNewPack, bAvailable );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	if( bAvailable == M4_FALSE )
	{
		return M4_SUCCESS;
	}
	

	// Clears the cache.
	iRet = clearCache();

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}


	// Install the new cache package.
	iRet = installCachePack( oNewPack );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	
	// Writes into the registry the cache package that is currently installed.
	iRet = setLocalPack( oNewPack );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Private methods
// -----------------------------------------------------------------------------

// Initializes the object.
m4return_t ClCacheReplacer::Initialize( ClChannelManager* ai_poChManager )
{
	m4language_t	iLang;
	m4pcchar_t		pccPath;
	int				iLength = -1;


	// Nothing to do.
	if( ai_poChManager == NULL )
	{
		return M4_ERROR;
	}


	m_poChManager	= ai_poChManager;


	// Sets the environment attributes.
	m_poChManager->GetLogonAdaptor()->GetLanguage( iLang );
	m_iEnvLang		= iLang;

	m_bEnvUnicode	= M4IsUnicode();

	m_pcEnvTechVer = ( char* )m_poChManager->GetEnvironment()->GetVersion();
	
	pccPath = m_poChManager->GetEnvironment()->GetCacheDirectory();
	m_pcEnvCachePath = M4CppToANSI( pccPath, iLength );


	return M4_SUCCESS;
}


// Retrieves from registry which cache package is currently installed.
m4return_t ClCacheReplacer::getLocalPack( ClCachePack& ao_oLocalPack )
{
	m4return_t		iRet;
	char*			pcRegValue;
	M4XmlRegistry*	poReg;
	

	poReg = M4XmlRegistry::Instance();

	if( poReg == NULL )
	{
		return M4_ERROR;
	}	
	
	ao_oLocalPack.Reset();


	// Language of the installed cache package.
	iRet = poReg->GetValue( s_acSavedPacksRegPath, s_acAttrLangRegKey, pcRegValue );

	if( iRet != M4_SUCCESS )
	{
		ao_oLocalPack.Reset();
		return M4_SUCCESS;
	}

	ao_oLocalPack.SetLanguage( atol( pcRegValue ) );
	delete [] pcRegValue;

	// Unicode context of the installed cache package.
	iRet = poReg->GetValue( s_acSavedPacksRegPath, s_acAttrUnicodeRegKey, pcRegValue );
	
	if( iRet != M4_SUCCESS )
	{
		ao_oLocalPack.Reset();
		return M4_SUCCESS;
	}

	ao_oLocalPack.SetUnicode( atoi( pcRegValue ) );
	delete [] pcRegValue;

	// Organization of the installed cache package.
	iRet = poReg->GetValue( s_acSavedPacksRegPath, s_acAttrOrgRegKey, pcRegValue );
	
	if( iRet != M4_SUCCESS )
	{
		ao_oLocalPack.Reset();
		return M4_SUCCESS;
	}

	ao_oLocalPack.SetOrganization( pcRegValue );
	delete [] pcRegValue;

	// Change date of the installed cache package.
	iRet = poReg->GetValue( s_acSavedPacksRegPath, s_acAttrChgDateRegKey, pcRegValue );
	
	if( iRet != M4_SUCCESS )
	{
		ao_oLocalPack.Reset();
		return M4_SUCCESS;
	}

	ao_oLocalPack.SetChangeDate( pcRegValue );
	delete [] pcRegValue;

	// Tech version of the installed cache package.
	iRet = poReg->GetValue( s_acSavedPacksRegPath, s_acAttrTechVerRegKey, pcRegValue );
	
	if( iRet != M4_SUCCESS )
	{
		ao_oLocalPack.Reset();
		return M4_SUCCESS;
	}

	ao_oLocalPack.SetTechVer( pcRegValue );
	delete [] pcRegValue;


	return M4_SUCCESS;
}


// Creates the cache packages Meta4Object.
m4return_t ClCacheReplacer::buildChannel()
{
	m4return_t	iRet;


	if( m_poChannel == NULL )
	{
		iRet = m_poChManager->CreateChannel( m_poChannel );

		if( iRet != M4_SUCCESS || m_poChannel == NULL )
		{
			return M4_ERROR;
		}
		
		iRet = m_poChannel->BuildFromId( "SSC_CACHE_PACKS" );

		if( iRet != M4_SUCCESS )
		{
			return M4_ERROR;
		}
	}

	if( m_poAccess == NULL )
	{
		m_poAccess = m_poChannel->CreateAccess();

		if( m_poAccess == NULL )
		{
			return M4_ERROR;
		}
	}

	if( m_poNode == NULL )
	{
		m_poNode = &( m_poAccess->Node[ "SSC_CACHE_PACKS" ] );

		if( m_poNode == NULL )
		{
			return M4_ERROR;
		}
	}


	return M4_SUCCESS;
}


m4return_t	ClCacheReplacer::checkForNewPack( ClCachePack& ai_oLocalPack, ClCachePack& ao_oNewPack, m4bool_t& ao_bAvailable )
{
	static const int s_iDateMaxLen = 64;
	m4return_t		iRet;
	m4VariantType	vArg, vRes;
	ClFinalizer		oFinalizer;
	m4double_t		dDate;
	m4char_t		acDate[ s_iDateMaxLen ];

	ao_bAvailable	= M4_FALSE;
	
	oFinalizer.setPackage( &ao_oNewPack );

	
	// Pushes the method arguments.

	// Current context language.
	vArg.Type = M4CL_CPP_TYPE_NUMBER;
	vArg.Data.DoubleData = m_iEnvLang;

	METHOD_STACK_PUSH( "GET_CACHE_PACKAGE", vArg );


	// Current context encoding type.
	vArg.Data.DoubleData = m_bEnvUnicode;

	METHOD_STACK_PUSH( "GET_CACHE_PACKAGE", vArg );


	// Current context tech version.
	vArg.Type = M4CL_CPP_TYPE_STRING_VAR;
	vArg.Data.PointerChar = m_pcEnvTechVer;

	METHOD_STACK_PUSH( "GET_CACHE_PACKAGE", vArg );
	

	// Language of the installed cache package.
	vArg.Type = M4CL_CPP_TYPE_NUMBER;
	vArg.Data.DoubleData = ai_oLocalPack.GetLanguage();
	if( vArg.Data.DoubleData == M4_LANG_NONE_ID )
	{
		vArg.Type = M4CL_CPP_TYPE_NULL;
	}

	METHOD_STACK_PUSH( "GET_CACHE_PACKAGE", vArg );


	// Unicode context of the installed cache package.
	vArg.Data.DoubleData = ai_oLocalPack.GetUnicode();
	if( vArg.Data.DoubleData == -1 )
	{
		vArg.Type = M4CL_CPP_TYPE_NULL;
	}
	else
	{
		vArg.Type = M4CL_CPP_TYPE_NUMBER;
	}

	METHOD_STACK_PUSH( "GET_CACHE_PACKAGE", vArg );
	

	// Organization of the installed cache package.
	vArg.Data.PointerChar = ai_oLocalPack.GetOrganization();
	if( vArg.Data.PointerChar != NULL && vArg.Data.PointerChar != '\0' )
	{
		vArg.Type = M4CL_CPP_TYPE_STRING_VAR;
	}
	else
	{
		vArg.Type = M4CL_CPP_TYPE_NULL;
	}

	METHOD_STACK_PUSH( "GET_CACHE_PACKAGE", vArg );


	// Change date of the installed cache package.
	vArg.Data.PointerChar = ai_oLocalPack.GetChangeDate();

	if( vArg.Data.PointerChar != NULL && *vArg.Data.PointerChar != '\0' )
	{
		ClStringToDate( vArg.Data.PointerChar, dDate );
		vArg.Data.DoubleData = dDate;
		vArg.Type = M4CL_CPP_TYPE_DATE;
	}
	else
	{
		vArg.Type = M4CL_CPP_TYPE_NULL;
	}

	METHOD_STACK_PUSH( "GET_CACHE_PACKAGE", vArg );


	// Tech version of the installed cache package.
	vArg.Data.PointerChar = ai_oLocalPack.GetTechVer();
	if( vArg.Data.PointerChar != NULL && vArg.Data.PointerChar != '\0' )
	{
		vArg.Type = M4CL_CPP_TYPE_STRING_VAR;
	}
	else
	{
		vArg.Type = M4CL_CPP_TYPE_NULL;
	}

	METHOD_STACK_PUSH( "GET_CACHE_PACKAGE", vArg );


	// Executes the method.
	iRet = m_poNode->RecordSet.Current.Item[ "GET_CACHE_PACKAGE" ].Call();
	if( iRet != M4_SUCCESS )
	{
		return M4_ERROR;
	}
	
	// Pops the method return.
	iRet = m_poNode->RecordSet.Current.Item[ "GET_CACHE_PACKAGE" ].StackPop( vRes );
	
	if( iRet != M4_SUCCESS )
	{
		return M4_ERROR;
	}
	
	if( vRes.Data.DoubleData != 0 )
	{
		return M4_ERROR;
	}
	

	// There isn't any package to install.
	if( m_poNode->RecordSet.Current.Count() == 0 )
	{
		return M4_SUCCESS;
	}


	// Gets the output items.

	// New cache package organization.
	GET_NODE_ITEM( "ID_ORGANIZATION", vArg );
	CHECK_EMPTY_STRING( vArg );
	
	ao_oNewPack.SetOrganization( vArg.Data.PointerChar );
	
	
	// New cache package change date.
	GET_NODE_ITEM( "DT_CHANGE", vArg );
	
	if( vArg.Type == M4CL_M4_TYPE_NULL )
	{
		return M4_SUCCESS;
	}
	ClDateToString( vArg.Data.DoubleData, acDate, s_iDateMaxLen );

	ao_oNewPack.SetChangeDate( acDate );

	
	// New cache package file.
	GET_NODE_ITEM( "PACK", vArg );
	// Not checking for empty string because it is allowed.

	ao_oNewPack.SetPack( vArg.Data.PointerChar );


	// New cache package language.
	ao_oNewPack.SetLanguage( m_iEnvLang );

	// New cache package encoding type.
	ao_oNewPack.SetUnicode( m_bEnvUnicode );

	// New cache package tech version.
	ao_oNewPack.SetTechVer( m_pcEnvTechVer );

	
	// Avoid reseting the package definition on finalization.
	oFinalizer.setPackage( NULL );

	ao_bAvailable = M4_TRUE;
	

	return M4_SUCCESS;
}


// Clears the cache.
m4return_t ClCacheReplacer::clearCache()
{
	m4return_t	iRet;


	// Memory.
	m_poChManager->GetCacheDirectory()->CleanCaches( M4_CACHE_ALL_CACHE_TYPE );

	// Disk.
	iRet = removeDirTree( m_pcEnvCachePath, M4_TRUE );


	return iRet;
}


// Install the new cache package.
m4return_t ClCacheReplacer::installCachePack( ClCachePack& ai_oNewPack )
{
	m4return_t		iRet;
	m4pchar_t		pcPackPath = NULL;
	m4VariantType	vArg, vRes;
	

	pcPackPath = ai_oNewPack.GetPack();

	// Null package is for local cleaning.
	if( pcPackPath == NULL || *pcPackPath == '\0' )
	{
		return M4_SUCCESS;
	}


	// Pushes the method arguments.

	// Cache package file path.
	vArg.Type = M4CL_CPP_TYPE_STRING_VAR;
	vArg.Data.PointerChar = pcPackPath;

	METHOD_STACK_PUSH( "M4EXPANDZIP", vArg );


	// Cache directory path.
	vArg.Data.PointerChar = ( m4pchar_t )m_poChManager->GetEnvironment()->GetCacheDirectory();

	METHOD_STACK_PUSH( "M4EXPANDZIP", vArg );


	// Executes the method.
	iRet = m_poNode->RecordSet.Current.Item[ "M4EXPANDZIP" ].Call();
	
	if( iRet != M4_SUCCESS )
	{
		return M4_ERROR;
	}
	
	// Pops the method return.
	iRet = m_poNode->RecordSet.Current.Item[ "M4EXPANDZIP" ].StackPop( vRes );
	
	if( iRet != M4_SUCCESS )
	{
		return M4_ERROR;
	}
	
	if( vRes.Data.DoubleData != 0 )
	{
		return M4_ERROR;
	}


	// Reloads the internal cache from disk.
	m_poChManager->GetCacheDirectory()->ReloadCaches();
	
	
	return M4_SUCCESS;
}


// Writes into the registry the cache package that is currently installed.
m4return_t ClCacheReplacer::setLocalPack( ClCachePack& ai_oLocalPack )
{
	m4return_t		iRet;
	m4char_t		acValue[ s_iRegValueMaxLength + 1 ];
	m4pchar_t		pcValue;
	M4XmlRegistry*	poReg;
	

	poReg = M4XmlRegistry::Instance();

	if( poReg == NULL )
	{
		return M4_ERROR;
	}

	pcValue = ai_oLocalPack.GetPack();

	// Null package is used for local cleaning.
	if( pcValue == NULL || *pcValue == '\0' )
	{
		iRet = poReg->DeleteKey( s_acSavedPacksRegPath );
			
		if( iRet != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_WO_DEL_REG_KEY, iRet << s_acSavedPacksRegPath );
			return M4_ERROR;
		}
	}
	else
	{
		// Language of the installed cache package.
		ltoa( ai_oLocalPack.GetLanguage(), acValue, 10 );

		iRet = poReg->SetValue( s_acSavedPacksRegPath, s_acAttrLangRegKey, acValue );
		
		if( iRet != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_WO_CREATE_REG_KEY, iRet << s_acAttrLangRegKey );
			return M4_ERROR;
		}


		// Unicode context of the installed cache package.
		itoa( ai_oLocalPack.GetUnicode(), acValue, 10 );

		iRet = poReg->SetValue( s_acSavedPacksRegPath, s_acAttrUnicodeRegKey, acValue );
		
		if( iRet != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_WO_CREATE_REG_KEY, iRet << s_acAttrUnicodeRegKey );
			return M4_ERROR;
		}

		
		// Organization of the installed cache package.
		pcValue = ai_oLocalPack.GetOrganization();

		iRet = poReg->SetValue( s_acSavedPacksRegPath, s_acAttrOrgRegKey, pcValue );
		
		if( iRet != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_WO_CREATE_REG_KEY, iRet << s_acAttrOrgRegKey );
			return M4_ERROR;
		}


		// Change date of the installed cache package.
		pcValue = ai_oLocalPack.GetChangeDate();

		iRet = poReg->SetValue( s_acSavedPacksRegPath, s_acAttrChgDateRegKey, pcValue );
		
		if( iRet != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_WO_CREATE_REG_KEY, iRet << s_acAttrChgDateRegKey );
			return M4_ERROR;
		}

		// tech version of the installed cache package.
		pcValue = ai_oLocalPack.GetTechVer();

		iRet = poReg->SetValue( s_acSavedPacksRegPath, s_acAttrTechVerRegKey, pcValue );
		
		if( iRet != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CH_WO_CREATE_REG_KEY, iRet << s_acAttrTechVerRegKey );
			return M4_ERROR;
		}
	}

	poReg->Save( NULL );


	return M4_SUCCESS;
}


// Removes a directory tree.
m4return_t ClCacheReplacer::removeDirTree( m4pcchar_t ai_pcPath, m4bool_t ai_bSkip )
{
	m4char_t		acPath[ MAX_PATH + 1 ] = "";
	HANDLE			hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA stFindData;
	m4return_t		iRet;
	DWORD			dwError = 0;

	
	sprintf( acPath, "%s\\*", ai_pcPath );

	hFind = FindFirstFile( acPath, &stFindData );

	if( hFind == INVALID_HANDLE_VALUE ) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_WO_ACCESS_4_DEL_DISK_CACHE, GetLastError() << acPath );
		return M4_ERROR;
	} 
   
	do
	{
		if( stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( strcmp( stFindData.cFileName, "." ) != 0 && strcmp( stFindData.cFileName, ".." ) != 0 )
			{
				sprintf( acPath, "%s\\%s", ai_pcPath, stFindData.cFileName );
				
				iRet = removeDirTree( acPath, M4_FALSE );

				if( iRet == M4_ERROR )
				{
					return M4_ERROR;
				}
			}
		}
		else
		{
			sprintf( acPath, "%s\\%s", ai_pcPath, stFindData.cFileName );
			
			iRet = _unlink( acPath );

			if( iRet )
			{
				DUMP_CHLOG_ERRORF( M4_CH_WO_DELETING_DISK_CACHE, errno << acPath );
				FindClose( hFind );
				
				return M4_ERROR;
			}

		}
	}
	while( FindNextFile( hFind, &stFindData ) != 0 );
 
	FindClose( hFind );

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_WO_ACCESS_4_DEL_DISK_CACHE, dwError << acPath );
		return M4_ERROR;
	}

	if( ai_bSkip == M4_FALSE )
	{
		rmdir( ai_pcPath );
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Class ClFinalizer
// -----------------------------------------------------------------------------

// Constructor.
ClFinalizer::ClFinalizer()
{
	m_pPack	= NULL;
}

// Destructor.
ClFinalizer::~ClFinalizer()
{
	if( m_pPack != NULL )
	{
		m_pPack->Reset();
	}
}

// Setter for package.
void ClFinalizer::setPackage( ClCachePack* ai_pPack )
{	
	m_pPack = ai_pPack; 
}
