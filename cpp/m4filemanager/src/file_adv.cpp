
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4filemanager.dll
// File:                file_adv.cpp   
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


#include "file_adv.hpp"
#include "m4unicode.hpp"


void	ClFile::_Init( m4pcchar_t ai_pccFileName )
{
	m_oFileName.SetFileName( ai_pccFileName ) ;
	m_oFileState.SetFileName( m_oFileName.GetFileName() ) ;
}


ClFile::ClFile( void )
{
	_Init( NULL ) ;
}


ClFile::ClFile( m4pcchar_t ai_pccFileName )
{
	_Init( ai_pccFileName ) ;
}


ClFile::ClFile( const ClFile& ai_roFile )
{
	_Init( ai_roFile.GetFileName() ) ;
}


ClFile& ClFile::operator = ( const ClFile& ai_roFile )
{
	if( this != &ai_roFile )
	{
		_Init( ai_roFile.GetFileName() ) ;
	}
	return( *this ) ;
}


m4return_t	ClFile::SetFileName( m4pcchar_t ai_pccFileName )
{
	_Init( ai_pccFileName ) ;

	if( IsValid() == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}
	return( M4_SUCCESS ) ;
}


m4bool_t	ClFile::IsValid( void ) const
{
	return( m_oFileState.IsValid() ) ;
}


m4bool_t	ClFile::IsFile( void ) const
{
	return( m_oFileState.IsFile() ) ;
}


m4bool_t	ClFile::IsDir( void ) const
{
	return( m_oFileState.IsDir() ) ;
}


m4uint32_t	ClFile::GetSize( void ) const
{
	return( m_oFileState.GetSize() ) ;
}


m4date_t	ClFile::GetLastWrite( void )
{
	return( m_oFileState.GetLastWrite() ) ;
}


m4date_t	ClFile::GetLastGmtWrite( void )
{
	return( m_oFileState.GetLastGmtWrite() ) ;
}


m4return_t	ClFile::GetShortName( string& ao_rsShortName ) const
{
	return( m_oFileName.GetShortName( ao_rsShortName ) ) ;
}


m4return_t ClFile::Delete( void )
{
	if( m_oFileName.IsNull() )
	{
		return( M4_ERROR ) ;
	}
	return( M4RemoveFile( m_oFileName.GetFileName() ) ) ;
}


