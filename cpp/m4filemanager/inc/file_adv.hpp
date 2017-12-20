
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4filemanager.dll
// File:                file_adv.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//
//==============================================================================


#include "file_misc.hpp"


#ifndef __FILE_ADV_HPP__
#define __FILE_ADV_HPP__



class M4_DECL_M4FILEMANAGER ClFile 
{

public:

		ClFile( void ) ; ;
		ClFile( m4pcchar_t ai_pccFileName ) ;
		ClFile( const ClFile& ai_roFile ) ;

	ClFile& operator = ( const ClFile& ai_roFile ) ;
	m4return_t	SetFileName( m4pcchar_t ai_pccFileName ) ;

	m4pcchar_t	GetFileName( void ) const
	{
		return( m_oFileName.GetFileName() ) ;
	}

	m4bool_t	IsValid( void ) const ;
	m4bool_t	IsFile( void ) const ;
	m4bool_t	IsDir( void ) const ;
	m4uint32_t	GetSize( void ) const ;
	m4date_t	GetLastWrite( void ) ;
	m4date_t	GetLastGmtWrite( void ) ;
	m4return_t	GetShortName( string& ao_rsShortName ) const ;

	m4return_t	Delete( void ) ;

protected:

	void	_Init( m4pcchar_t ai_pccFileName ) ;

	ClFileName	m_oFileName ;
	ClFileState	m_oFileState ;
};


#endif


