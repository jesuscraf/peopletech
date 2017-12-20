
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4filemanager.dll
// File:                file_misc.hpp   
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


#include "m4filemanager_dll.hpp"
#include "m4stl.hpp"
#include <m4types.hpp>


#ifndef __FILE_MISC_HPP__
#define __FILE_MISC_HPP__



#define COMPARISON_MASK_ERASED				1
#define COMPARISON_MASK_CREATED				2
#define COMPARISON_MASK_RENAMED				4
#define COMPARISON_MASK_ATTRIBUTES			8
#define COMPARISON_MASK_SIZE				16
#define COMPARISON_MASK_DATA				32



// ============================================================================
// Class ClComparisonResultMask
// ============================================================================

class M4_DECL_M4FILEMANAGER ClComparisonResultMask 
{

public:

		ClComparisonResultMask( void ) ;
		ClComparisonResultMask( m4uint32_t ai_iMask ) ;

	void	Reset( void ) ;

	m4bool_t	AnyChange( void ) const ;

	operator m4uint32_t( void ) const ;

	void AdjustFlags( void ) ;

	unsigned int Erased: 1 ;
	unsigned int Created: 1 ;
	unsigned int Renamed: 1 ;
	unsigned int Attributes: 1 ;
	unsigned int Size: 1 ;
	unsigned int Data: 1 ;
};



// ============================================================================
// Class ClFileName
// ============================================================================

class M4_DECL_M4FILEMANAGER ClFileName 
{
public:

		ClFileName( void ) ;
		ClFileName( m4pcchar_t ai_pccFileName ) ;
		ClFileName( const ClFileName& ai_roFileName ) ;
		~ClFileName( void ) ;

	ClFileName& operator = ( m4pcchar_t ai_pccFileName ) ;
	ClFileName& operator = ( const ClFileName& ai_roFileName ) ;

	operator const m4char_t*( void ) const
	{
		return( m_pcFileName ) ;
	}

	m4pcchar_t	GetFileName( void ) const
	{
		return( m_pcFileName ) ;
	}

	m4uint32_t GetLength () const
	{
		return m_iLength; 
	}

	m4bool_t	IsNull( void ) const ;
	m4bool_t	IsValid( void ) const ;
	m4bool_t	IsFile( void ) const ;
	m4bool_t	IsDir( void ) const ;

	m4return_t	SetFileName( m4pcchar_t ai_pccFileName ) ;
	m4return_t	GetFileName( m4pchar_t ao_acFileName ) const ;

	m4return_t	GetShortName( string& ao_rsShortName ) const ;
	m4return_t	GetDir( ClFileName& ao_roDirName ) const ;
	m4return_t	GetFileExtension( m4pchar_t ao_acFileExtension ) const ;


	friend m4bool_t	M4_DECL_M4FILEMANAGER operator == ( const ClFileName& a, const ClFileName& b ) ;
	friend m4bool_t	M4_DECL_M4FILEMANAGER operator != ( const ClFileName& a, const ClFileName& b ) ;
	friend m4bool_t	M4_DECL_M4FILEMANAGER operator <  ( const ClFileName& a, const ClFileName& b ) ;

protected:

	void	_Init( m4pcchar_t ai_pccFileName ) ;
	void	_Destroy( void ) ;

	m4pchar_t	m_pcFileName ;
	m4uint32_t	m_iLength ;
};


typedef list<ClFileName>		ClFileList ;
typedef ClFileList::iterator	itClFileList ;





// ============================================================================
// Class ClFileState
// ============================================================================

class M4_DECL_M4FILEMANAGER ClFileState 
{
public:

		ClFileState( void ) ;
		ClFileState( m4pcchar_t ai_pccFileName ) ;
		ClFileState( const ClFileState& ai_roFileState ) ;

	ClFileState& operator = ( const ClFileState& ai_roFileState ) ;

	void	SetFileName( m4pcchar_t ai_pccFileName ) ;
	void	CompareAndSetMask( const ClFileState& ai_roNewState, ClComparisonResultMask& ao_roDiffMask ) const ;

	m4bool_t	IsValid( void ) const
	{
		return( m_bIsValid ) ;
	}

	m4bool_t	IsFile( void ) const
	{
		return( m_bIsValid == M4_TRUE && m_bIsDir == M4_FALSE ) ;
	}

	m4bool_t	IsDir( void ) const
	{
		return( m_bIsValid == M4_TRUE && m_bIsDir == M4_TRUE ) ;
	}

	m4uint32_t	GetSize( void ) const
	{
		return( m_bIsValid == M4_TRUE ? m_iSize : 0 ) ;
	}

	m4uint32_t	GetAttributes( void ) const
	{
		return( m_iAttributes ) ;
	}

	m4date_t	GetLastWrite( void  ) const
	{
		return( m_bIsValid == M4_TRUE ? m_dLastWrite : 0 ) ;
	}

	m4date_t	GetLastGmtWrite( void ) const
	{
		return( m_bIsValid == M4_TRUE ? m_dLastGmtWrite : 0 ) ;
	}

	friend m4bool_t	M4_DECL_M4FILEMANAGER	operator == ( const ClFileState& a, const ClFileState& b ) ;
	friend m4bool_t	M4_DECL_M4FILEMANAGER	operator != ( const ClFileState& a, const ClFileState& b ) ;

protected:

	void	_Reset( void ) ;
	void	_Init( const ClFileState& ai_oFileState ) ;

	m4bool_t	m_bIsValid;
	m4bool_t	m_bIsDir;
	m4uint32_t	m_iSize;
	m4uint32_t	m_iAttributes;
	m4date_t	m_dLastWrite;
	m4date_t	m_dLastGmtWrite;
};


#endif
