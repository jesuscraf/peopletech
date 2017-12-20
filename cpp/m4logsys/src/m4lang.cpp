//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           m4lang.cpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           12 abr 99
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Read languages information from Registry.
////
//==============================================================================


#include <stdlib.h>
#include "m4lang.hpp"
#include "m4var.hpp"
#include "regedit.hpp"
#include "m4objreg.hpp"
#include "m4xmlreg.hpp"



#define MAX_REG_VALUE_LEN 256


static bool M4LanguageCompareFunction( const M4StLang* a, const M4StLang* b )
{
	return a->id < b->id;
}



// Instance (like singletton)
M4ClLangInfo	*M4ClLangInfo::ms_poInstance = NULL ;
ClMutex			*M4ClLangInfo::ms_poMutex = new ClMutex( M4_TRUE ) ;


m4pcchar_t  M4ClLanguage( m4language_t ai_langValue )
{
	return M4ClLangInfo::Instance()->GetErrorName( ai_langValue ) ;
}



// To get the instance of the singleton
M4ClLangInfo* M4ClLangInfo::Instance( void )
{
	if( ms_poInstance == NULL )
	{
		ms_poMutex->Lock() ;

		if( ms_poInstance == NULL )
		{
			ms_poInstance = new M4ClLangInfo() ;
		}
		ms_poMutex->Unlock() ;
	}
	return ms_poInstance ;
}


// Free the instance
void M4ClLangInfo::FreeInstance( void )
{
	if( ms_poInstance != NULL )
	{
		ms_poMutex->Lock() ;

		if( ms_poInstance != NULL )
		{
			delete ms_poInstance ;
			ms_poInstance = NULL ;
		}
		ms_poMutex->Unlock() ;
	}
}


// Object constructor
M4ClLangInfo::M4ClLangInfo( void )
{
#ifdef _WINDOWS
	modeCurrentLang = autoWin32CurrentLang ;
#else
	modeCurrentLang = manualCurrentLang ;
#endif // _WINDOWS

	m_poSettingsLanguage = NULL ;
	m_poCurrentLanguage = NULL ;
	m_poDefaultLanguage = NULL ;

	_ReadAllFromRegistry() ;
}


// Object destructor
M4ClLangInfo::~M4ClLangInfo( void )
{

	size_t		i = 0 ;
	size_t		iLength = 0 ;
	M4StLang	*poLanguage = NULL ;

	iLength = m_vLanguages.size();

	for( i = 0 ; i < iLength ; i++ )
	{
		poLanguage = m_vLanguages[ i ] ;

		if( poLanguage != NULL )
		{
			delete poLanguage ;
		}
	}
	m_vLanguages.clear() ; // Delete vector elements
}


// Read info from registry for the Language indicated
m4return_t	M4ClLangInfo::_ReadFromRegistry( m4pcchar_t ai_pccRegKey, M4StLang* &ao_rpoLanguage, m4bool_t& ao_rbIsDefault ) const
{

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bId = M4_FALSE ;
	m4bool_t	bLCID = M4_FALSE ;
	m4bool_t	bName = M4_FALSE ;
	m4bool_t	bDate = M4_FALSE ;
	m4bool_t	bTime = M4_FALSE ;
	size_t		iLength = -1 ;
	m4pchar_t	pcValueName = NULL ;
	m4pchar_t	pcValueData = NULL ;

	list<m4pchar_t> lNames ;
	list<m4pchar_t>::iterator	itIterator ;


	// Initialize output
	ao_rpoLanguage = new M4StLang() ;
	ao_rbIsDefault = M4_FALSE; // At first, it is not default.

	M4XmlRegistry	*poRegistry = M4XmlRegistry::Instance() ;

	iResult = poRegistry->GetValueNames( ai_pccRegKey, lNames ) ;

	if( iResult == M4_SUCCESS )
	{
		itIterator = lNames.begin() ;

		while( itIterator != lNames.end() )
		{
			pcValueName = *itIterator ;

			iResult = poRegistry->GetValue( ai_pccRegKey, pcValueName, pcValueData ) ;

			if( iResult != M4_ERROR )
			{
				if( bId == M4_FALSE && stricmp( pcValueName, M4_LANG_ID_REGEDIT_KEY ) == 0 )
				{
					ao_rpoLanguage->id = atol( pcValueData ) ;

					if( ao_rpoLanguage->id != M4_LANG_NONE_ID ) // This id is not allowed
					{
						bId = M4_TRUE ;
					}
				}
				else if( bLCID == M4_FALSE && stricmp( pcValueName, M4_LANG_LCID_REGEDIT_KEY ) == 0)
				{
					ao_rpoLanguage->LCID = atol( pcValueData ) ;
					bLCID = M4_TRUE ;
				}
				else if( bName == M4_FALSE && stricmp( pcValueName, M4_LANG_NAME_REGEDIT_KEY ) == 0 )
				{
					iLength = strlen( pcValueData ) + 1 ;
					ao_rpoLanguage->pszName = new m4char_t[ iLength ] ;
					memcpy( ao_rpoLanguage->pszName, pcValueData, iLength ) ;
					bName = M4_TRUE ;
				}
				else if( bDate == M4_FALSE && stricmp( pcValueName, M4_LANG_ERR_DATE_REGEDIT_KEY ) == 0 )
				{
					iLength = strlen( pcValueData ) + 1 ;
					ao_rpoLanguage->pszErrorDateFormat = new m4char_t[ iLength ] ;
					memcpy( ao_rpoLanguage->pszErrorDateFormat, pcValueData, iLength ) ;
					bDate = M4_TRUE ;
				}
				else if( bTime == M4_FALSE && stricmp( pcValueName, M4_LANG_ERR_TIME_REGEDIT_KEY ) == 0 )
				{
					iLength = strlen( pcValueData ) + 1 ;
					ao_rpoLanguage->pszErrorTimeFormat = new m4char_t[ iLength ] ;
					memcpy( ao_rpoLanguage->pszErrorTimeFormat, pcValueData, iLength ) ;
					bTime = M4_TRUE ;
				}
				else if( ao_rbIsDefault == M4_FALSE && stricmp( pcValueName, M4_LANG_DEFAULT_REGEDIT_KEY ) == 0 )
				{
					if( *pcValueData == '1' )
					{
						ao_rbIsDefault = M4_TRUE ;
					}
				}
				delete [] pcValueData ;
			}
			delete [] pcValueName ;
			itIterator++ ;
		}
		lNames.clear() ;
	}

	if( bId == M4_FALSE || bLCID == M4_FALSE || bName == M4_FALSE )
	{
		ao_rbIsDefault = M4_FALSE ;
		delete ao_rpoLanguage ;
		return M4_ERROR;
	} 

	if( bDate == M4_FALSE )
	{
		ao_rpoLanguage->pszErrorDateFormat = new m4char_t[ sizeof(M4_LANG_DEFAULT_ERR_DATE) ] ;
		strcpy( ao_rpoLanguage->pszErrorDateFormat, M4_LANG_DEFAULT_ERR_DATE ) ;
	}

	if( bTime == M4_FALSE )
	{
		ao_rpoLanguage->pszErrorTimeFormat = new m4char_t[ sizeof(M4_LANG_DEFAULT_ERR_TIME) ] ;
		strcpy( ao_rpoLanguage->pszErrorTimeFormat, M4_LANG_DEFAULT_ERR_TIME ) ;
	}

	return M4_SUCCESS;
}


// Read all languages and info from Registry, and set the language to the default language.
// Returns the number of read languages
void	M4ClLangInfo::_ReadAllFromRegistry()
{

	m4bool_t	bIsDefault = M4_FALSE ;
	m4pchar_t	pConcatLang = NULL ;
	m4char_t	szLanguageKey[ MAX_REG_VALUE_LEN + 1 ] ;
	m4LCID_t	iSettingsLCID = -1 ;
	M4StLang*	poLanguage = NULL ;


	// Estimated number of languages 
	m_vLanguages.reserve( 50 ) ;

#ifdef _WINDOWS
	// Bug 0282270. En server no se toma el lenguaje de regional settings
	if( getenv( "M4SERVER_PATH" ) == NULL )
	{
		iSettingsLCID = GetUserDefaultLCID() ;
	}
#endif

	m_poSettingsLanguage = NULL ;
	m_poDefaultLanguage = NULL ;

	m4return_t	iResult = M4_ERROR ;
	m4pchar_t	pcKeyName = NULL ;

	list<m4pchar_t> lSubKeys ;
	list<m4pchar_t>::iterator	itIterator ;

	iResult = M4XmlRegistry::Instance()->GetSubKeyNames( M4_NEW_LANG_REGEDIT_KEY, lSubKeys ) ;

	if( iResult == M4_SUCCESS )
	{
		strcpy( szLanguageKey, M4_NEW_LANG_REGEDIT_KEY ) ;
		pConcatLang = szLanguageKey + strlen( szLanguageKey ) ;
		*pConcatLang++ = '\\' ;

		itIterator = lSubKeys.begin() ;

		while( itIterator != lSubKeys.end() )
		{
			pcKeyName = *itIterator ;
			strcpy( pConcatLang, pcKeyName ) ;

			iResult = _ReadFromRegistry( szLanguageKey, poLanguage, bIsDefault ) ;

			if( iResult == M4_SUCCESS )
			{
				if( _QueryLanguageByIdNoBlock( poLanguage->id ) == NULL )
				{
					//Add to languages vector if it not exist
					m_vLanguages.push_back( poLanguage ) ;

					if( bIsDefault == M4_TRUE && m_poDefaultLanguage == NULL )
					{
						m_poDefaultLanguage = poLanguage ;
					}

					if( poLanguage->LCID == iSettingsLCID && m_poSettingsLanguage == NULL )
					{
						m_poSettingsLanguage = poLanguage ;
					}
				}
				else
				{
					delete poLanguage ;
				}
			}

			delete [] pcKeyName ;
			itIterator++ ;
		}
		lSubKeys.clear() ;
	}

	if( m_poDefaultLanguage == NULL )
	{
		// No default language found
		poLanguage = _QueryLanguageByIdNoBlock( M4_LANG_DEFAULT_ID ) ;

		if( poLanguage == NULL )
		{
			poLanguage = _QueryLanguageByLCIDNoBlock( M4_LANG_DEFAULT_LCID ) ;

			if( poLanguage == NULL )
			{
				// It uses defaults in defines
				poLanguage = new M4StLang() ;
				poLanguage->id = M4_LANG_DEFAULT_ID ;
				poLanguage->LCID = M4_LANG_DEFAULT_LCID ;

				poLanguage->pszName = new m4char_t[ sizeof(M4_LANG_DEFAULT_NAME) ] ;
				strcpy( poLanguage->pszName, M4_LANG_DEFAULT_NAME ) ;

				poLanguage->pszErrorDateFormat = new m4char_t[ sizeof(M4_LANG_DEFAULT_ERR_DATE) ] ;
				strcpy( poLanguage->pszErrorDateFormat, M4_LANG_DEFAULT_ERR_DATE ) ;

				poLanguage->pszErrorTimeFormat = new m4char_t[ sizeof(M4_LANG_DEFAULT_ERR_TIME) ];
				strcpy( poLanguage->pszErrorTimeFormat, M4_LANG_DEFAULT_ERR_TIME );

				m_vLanguages.push_back( poLanguage ) ;
			}
		}
		m_poDefaultLanguage = poLanguage ;
	}

	sort( m_vLanguages.begin(), m_vLanguages.end(), M4LanguageCompareFunction ); // Order by uId.
	m_poCurrentLanguage = m_poDefaultLanguage ;	// Sets the current language to the default language
}


// Looks for a language by index
M4StLang*	M4ClLangInfo::_QueryLanguageByIndex( m4uint32_t ai_iIndex ) const
{

	M4StLang*	poResult = NULL ;


	ms_poMutex->Lock() ;

	if( ai_iIndex < m_vLanguages.size() )
	{
		poResult = m_vLanguages[ ai_iIndex ] ;
	}

	ms_poMutex->Unlock() ;

	return poResult ;
}


// Looks for a language by language id without blocking
M4StLang*	M4ClLangInfo::_QueryLanguageByIdNoBlock( m4language_t ai_iLanguage ) const
{

	M4StLang*	poResult = NULL ;
	M4StLang*	poLanguage = NULL ;


	size_t iLength = m_vLanguages.size();

	for( size_t i = 0; i < iLength; i++ )
	{
		poLanguage = m_vLanguages[ i ] ;

		if( poLanguage != NULL && poLanguage->id == ai_iLanguage )
		{
			poResult = poLanguage ;
			break ;
		}
	}

	return poResult ;
}


// Looks for a language by language id
M4StLang*	M4ClLangInfo::_QueryLanguageById( m4language_t ai_iLanguage ) const
{

	M4StLang*	poResult = NULL ;


	ms_poMutex->Lock() ;
	poResult = _QueryLanguageByIdNoBlock( ai_iLanguage ) ;
	ms_poMutex->Unlock() ;

	return poResult ;
}


// Looks for a language by language LCID without blocking
M4StLang*	M4ClLangInfo::_QueryLanguageByLCIDNoBlock( m4LCID_t ai_iLCID ) const
{

	M4StLang*	poResult = NULL ;
	M4StLang*	poLanguage = NULL ;


	size_t iLength = m_vLanguages.size();

	for( size_t i = 0; i < iLength; i++ )
	{
		poLanguage = m_vLanguages[ i ] ;

		if( poLanguage != NULL && poLanguage->LCID == ai_iLCID )
		{
			poResult = poLanguage ;
			break ;
		}
	}

	return poResult ;
}


// Looks for a language by language LCID
M4StLang*	M4ClLangInfo::_QueryLanguageByLCID( m4LCID_t ai_iLCID ) const
{

	M4StLang*	poResult = NULL ;


	ms_poMutex->Lock() ;
	poResult = _QueryLanguageByLCIDNoBlock( ai_iLCID ) ;
	ms_poMutex->Unlock() ;

	return poResult ;
}


M4StLang*	M4ClLangInfo::_GetSettingsLanguage( void )
{
	return m_poSettingsLanguage ;
}


// Get current language
M4StLang*	M4ClLangInfo::_GetCurrentLanguage( void )
{

#ifdef _WINDOWS
	if( modeCurrentLang == autoWin32CurrentLang )
	{
		m4LCID_t iLCID = GetThreadLocale() ;

		if( m_poCurrentLanguage == NULL || m_poCurrentLanguage->LCID != iLCID )
		{
			m_poCurrentLanguage = _QueryLanguageByLCID( iLCID ) ;

			if( m_poCurrentLanguage == NULL )
			{
				m_poCurrentLanguage = m_poDefaultLanguage ;
			}
		}
	}
#endif

	return m_poCurrentLanguage ;
}


m4uint32_t M4ClLangInfo::GetNumberOfLanguages( void ) const
{

	m4uint32_t	iResult = 0 ;

	
	ms_poMutex->Lock() ;
	iResult = m_vLanguages.size() ;
	ms_poMutex->Unlock() ;

	return iResult ;
}


m4language_t M4ClLangInfo::GetErrorIdByIndex( m4uint32_t ai_iIndex ) const
{

	M4StLang*	poLanguage = _QueryLanguageByIndex( ai_iIndex ) ;

	if( poLanguage != NULL )
	{
		return poLanguage->id ;
	}
	return LOGSYS_INVALID_M4UINT32 ;
}


// If id is not found, it returns LOGSYS_INVALID_M4UINT32
m4LCID_t	M4ClLangInfo::GetErrorLCID( m4language_t ai_iLanguage ) const
{
	M4StLang*	poLanguage = _QueryLanguageById( ai_iLanguage ) ;

	if( poLanguage != NULL )
	{
		return poLanguage->LCID ;
	}
	return LOGSYS_INVALID_M4UINT32 ;
}


// If id is not found, it returns M4_LANG_UNDEFINED_NAME
m4pcchar_t	M4ClLangInfo::GetErrorName( m4language_t ai_iLanguage ) const
{
	M4StLang*	poLanguage = _QueryLanguageById( ai_iLanguage ) ;

	if( poLanguage != NULL )
	{
		return poLanguage->pszName ;
	}
	return M4_LANG_UNDEFINED_NAME ;
}


// Get Date format for the language indicated. If not founded return ""
m4pcchar_t	M4ClLangInfo::GetErrorDateFormat( m4language_t ai_iLanguage ) const
{
	M4StLang*	poLanguage = _QueryLanguageById( ai_iLanguage ) ;

	if( poLanguage != NULL )
	{
		return poLanguage->pszErrorDateFormat;
	}
	return "" ;
}


// Get Time format for the language indicated. If not founded return ""
m4pcchar_t	M4ClLangInfo::GetErrorTimeFormat( m4language_t ai_iLanguage ) const
{
	M4StLang*	poLanguage = _QueryLanguageById( ai_iLanguage ) ;

	if( poLanguage != NULL )
	{
		return poLanguage->pszErrorTimeFormat;
	}
	return "" ;
}


m4pcchar_t	M4ClLangInfo::GetSettingsErrorDateFormat( void )
{
	M4StLang*	poLanguage = _GetSettingsLanguage() ;

	if( poLanguage != NULL )
	{
		return poLanguage->pszErrorDateFormat;
	}
	return "" ;
}


m4pcchar_t	M4ClLangInfo::GetSettingsErrorTimeFormat( void )
{
	M4StLang*	poLanguage = _GetSettingsLanguage() ;

	if( poLanguage != NULL )
	{
		return poLanguage->pszErrorTimeFormat;
	}
	return "" ;
}


m4pcchar_t	M4ClLangInfo::GetCurrentErrorDateFormat( void )
{
	M4StLang*	poLanguage = _GetCurrentLanguage() ;

	if( poLanguage != NULL )
	{
		return poLanguage->pszErrorDateFormat;
	}
	return "" ;
}


m4pcchar_t	M4ClLangInfo::GetCurrentErrorTimeFormat( void )
{
	M4StLang*	poLanguage = _GetCurrentLanguage() ;

	if( poLanguage != NULL )
	{
		return poLanguage->pszErrorTimeFormat;
	}
	return "" ;
}


m4language_t	M4ClLangInfo::GetDefaultLanguageId( void ) const
{
	if( m_poDefaultLanguage != NULL )
	{
		return m_poDefaultLanguage->id;
	}
	return LOGSYS_INVALID_M4UINT32 ;
}


// Set the current language using Id
m4return_t M4ClLangInfo::SetCurrentLangId( m4language_t ai_iLanguage )
{
	if( modeCurrentLang != autoWin32CurrentLang )
	{
		if( m_poCurrentLanguage == NULL || m_poCurrentLanguage->id != ai_iLanguage )
		{
			m_poCurrentLanguage = _QueryLanguageById( ai_iLanguage ) ;

			if( m_poCurrentLanguage == NULL )
			{
				m_poCurrentLanguage = m_poDefaultLanguage ;
				return M4_ERROR ;
			}
		}
	}

	return M4_SUCCESS ;
}


// Adds a new language
m4bool_t M4ClLangInfo::AddLanguage( m4language_t ai_iLanguage, m4LCID_t ai_iLCID, m4pcchar_t ai_pccName, m4pcchar_t ai_pccErrorDateFormat, m4pcchar_t ai_pccErrorTimeFormat )
{

	m4bool_t	bResult = M4_FALSE ;
	size_t		iLength = -1 ;
	M4StLang	*poLanguage = NULL ;


	ms_poMutex->Lock() ;

	if( _QueryLanguageByIdNoBlock( ai_iLanguage ) == NULL )
	{
		if( ai_iLCID == 0 )
		{
			ai_iLCID = M4_LANG_DEFAULT_LCID ;
		}

		if( ai_pccName == NULL )
		{
			ai_pccName = M4_LANG_UNDEFINED_NAME ;
		}

		if( ai_pccErrorDateFormat == NULL )
		{
			ai_pccErrorDateFormat = M4_LANG_DEFAULT_ERR_DATE ;
		}

		if( ai_pccErrorTimeFormat == NULL )
		{
			ai_pccErrorTimeFormat = M4_LANG_DEFAULT_ERR_TIME ;
		}

		poLanguage = new M4StLang() ;

		poLanguage->id = ai_iLanguage ;
		poLanguage->LCID = ai_iLCID ;

		iLength = strlen( ai_pccName ) + 1 ;
		poLanguage->pszName = new m4char_t[ iLength ] ;
		memcpy( poLanguage->pszName, ai_pccName, iLength ) ;

		iLength = strlen( ai_pccErrorDateFormat ) + 1 ;
		poLanguage->pszErrorDateFormat = new m4char_t[ iLength ] ;
		memcpy( poLanguage->pszErrorDateFormat, ai_pccErrorDateFormat, iLength ) ;

		iLength = strlen( ai_pccErrorTimeFormat ) + 1 ;
		poLanguage->pszErrorTimeFormat = new m4char_t[ iLength ] ;
		memcpy( poLanguage->pszErrorTimeFormat, ai_pccErrorTimeFormat, iLength ) ;

		m_vLanguages.push_back( poLanguage ) ;
		sort( m_vLanguages.begin(), m_vLanguages.end(), M4LanguageCompareFunction ) ; // Order by uId.
		bResult = M4_TRUE ;
	}
	ms_poMutex->Unlock() ;

	return bResult ;
}


// Check if aio_uIdLang is a valid language id. If it is valid, it returns
// M4_TRUE, if not, it sets aio_uIdLang to the default language and returns M4_FALSE
m4bool_t	M4ClLangInfo::CheckAndSetValidLangId( m4language_t& aio_iLanguage )
{
	if( _QueryLanguageById( aio_iLanguage ) != NULL )
	{
		return M4_TRUE ;
	}

	if( m_poDefaultLanguage != NULL )
	{
		aio_iLanguage = m_poDefaultLanguage->id ;
	}
	else
	{
		aio_iLanguage = LOGSYS_INVALID_M4UINT32 ;
	}

	return M4_FALSE ;
}


