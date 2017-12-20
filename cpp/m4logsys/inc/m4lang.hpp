//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           m4lang.hpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           4 feb 99
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

#ifndef __M4LANG_HPP__
#define __M4LANG_HPP__

#include "m4logsys_dll.hpp" 
#include "m4types.hpp" 
#include "m4stl.hpp" 
#include "syncro.hpp"



//The same define in 'm4logsys.hpp'
#define LOGSYS_INVALID_M4UINT32 0xFFFFFFFF

M4_DECL_M4LOGSYS m4pcchar_t M4ClLanguage( m4language_t ai_langValue ) ;


// Structure with information about each language
struct M4_DECL_M4LOGSYS M4StLang
{
	m4language_t	id ;				// identification of the language.
	m4LCID_t		LCID ;				// LCID of the language.
	m4pchar_t		pszName ;
	m4pchar_t		pszErrorDateFormat ;
	m4pchar_t		pszErrorTimeFormat ;

	M4StLang( void )
	{
		id = LOGSYS_INVALID_M4UINT32 ;
		LCID = LOGSYS_INVALID_M4UINT32 ;
		pszName = NULL ;
		pszErrorDateFormat = NULL ;
		pszErrorTimeFormat = NULL ;
	}

	~M4StLang( void )
	{
		id = LOGSYS_INVALID_M4UINT32 ;
		LCID = LOGSYS_INVALID_M4UINT32 ;

		if( pszName != NULL )
		{
			delete pszName ;
			pszName = NULL ;
		}

		if( pszErrorDateFormat != NULL )
		{
			delete pszErrorDateFormat ;
			pszErrorDateFormat = NULL ;
		}

		if( pszErrorTimeFormat != NULL )
		{
			delete pszErrorTimeFormat ;
			pszErrorTimeFormat = NULL ;
		}
	}
};

typedef vector <M4StLang*> M4StLangVector ;



// Class to store registry info for all languages.
// Info is sorted by Language Id.
// It's a unique instance, like a singletton.
class M4_DECL_M4LOGSYS M4ClLangInfo
{

public:

	enum { manualCurrentLang, autoWin32CurrentLang } modeCurrentLang;

	// To get the instance of the singleton
	static M4ClLangInfo* Instance( void ) ;

	// Free the data to avoid memory leaks in the end of the program
	static void	FreeInstance( void ) ;

	m4uint32_t		GetNumberOfLanguages( void ) const ;

	// If index is not found, it returns LOGSYS_INVALID_M4UINT32
	m4language_t	GetErrorIdByIndex( m4uint32_t ai_iIndex ) const ;

	// If id is not found, it returns LOGSYS_INVALID_M4UINT32
	m4LCID_t		GetErrorLCID( m4language_t ai_iLanguage ) const ;

	// If id is not found, it returns M4_LANG_UNDEFINED_NAME
	m4pcchar_t		GetErrorName( m4language_t ai_iLanguage ) const ;

	// Get Date format for the language indicated. If not founded return ""
	m4pcchar_t		GetErrorDateFormat( m4language_t ai_iLanguage ) const ;
	m4pcchar_t		GetErrorTimeFormat( m4language_t ai_iLanguage ) const ;

	m4pcchar_t		GetSettingsErrorDateFormat( void ) ;
	m4pcchar_t		GetSettingsErrorTimeFormat( void ) ;

	m4pcchar_t		GetCurrentErrorDateFormat( void ) ;
	m4pcchar_t		GetCurrentErrorTimeFormat( void ) ;

	m4language_t	GetDefaultLanguageId( void ) const ;

	m4return_t		SetCurrentLangId( m4language_t ai_iLanguage ) ;

	// Adds a new language
	m4bool_t		AddLanguage( m4language_t ai_iLanguage, m4LCID_t ai_iLCID, m4pcchar_t ai_pccName, m4pcchar_t ai_pccErrorDateFormat, m4pcchar_t ai_pccErrorTimeFormat ) ;

	// Check if aio_uIdLang is a valid language id. If it is valid, it returns
	// M4_TRUE, if not, it sets aio_uIdLang to the default language and returns M4_FALSE
	m4bool_t		CheckAndSetValidLangId( m4language_t& aio_iLanguage ) ;


protected:

	M4ClLangInfo( void ) ;
	~M4ClLangInfo( void ) ;

	void		_ReadAllFromRegistry( void ) ;
	m4return_t	_ReadFromRegistry( m4pcchar_t ai_pccRegKey, M4StLang* &ao_rpoLanguage, m4bool_t& ao_rbIsDefault ) const ;

	M4StLang*	_QueryLanguageByIndex( m4uint32_t ai_iIndex ) const ;
	M4StLang*	_QueryLanguageByIdNoBlock( m4language_t ai_iLanguage ) const ;
	M4StLang*	_QueryLanguageById( m4language_t ai_iLanguage ) const ;
	M4StLang*	_QueryLanguageByLCIDNoBlock( m4LCID_t ai_iLCID ) const ;
	M4StLang*	_QueryLanguageByLCID( m4LCID_t ai_iLCID ) const ;
	M4StLang*	_GetSettingsLanguage( void ) ;
	M4StLang*	_GetCurrentLanguage( void ) ;


	M4StLang			*m_poSettingsLanguage ;		// Regional settings language entry in m_vLanguages
	M4StLang			*m_poCurrentLanguage ;		// Current language entry in m_vLanguages
	M4StLang			*m_poDefaultLanguage ;		// Default language (do not modify, just read).
	M4StLangVector		m_vLanguages ;				// Languages ordered by uId (do not modify, just read).

	static M4ClLangInfo	*ms_poInstance ;
	static ClMutex		*ms_poMutex ;

};


#endif // __M4LANG_HPP__
