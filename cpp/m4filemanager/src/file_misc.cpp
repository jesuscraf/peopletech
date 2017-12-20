
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4filemanager.dll
// File:                file_misc.cpp   
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
#include "file_adv.hpp"
#include "cldates.hpp"
#include "cldefine.hpp"
#include "m4unicode.hpp"

#include <math.h>
#include <sys/stat.h>

#ifdef _UNIX
	#include <string.h>
#endif


// backslash
#ifdef _WINDOWS
#define M4_FM_BACK_SLASH	'\\'
#else
#define M4_FM_BACK_SLASH	'/'
#endif





// ============================================================================
// Class ClComparisonResultMask
// ============================================================================

ClComparisonResultMask::ClComparisonResultMask( void )
{
	Reset() ;
}


ClComparisonResultMask::ClComparisonResultMask( m4uint32_t ai_iMask )
{
	Reset() ;

	if( ai_iMask & COMPARISON_MASK_ERASED )
	{
		Erased = 1 ;
	}

	if( ai_iMask & COMPARISON_MASK_CREATED )
	{
		Created = 1 ;
	}

	if( ai_iMask & COMPARISON_MASK_RENAMED )
	{
		Renamed = 1 ;
	}

	if( ai_iMask & COMPARISON_MASK_ATTRIBUTES )
	{
		Attributes = 1 ;
	}

	if( ai_iMask & COMPARISON_MASK_SIZE )
	{
		Size = 1 ;
	}

	if( ai_iMask & COMPARISON_MASK_DATA )
	{
		Data = 1 ;
	}
}


void	ClComparisonResultMask::Reset( void )
{
	// Set all values to 0
	Erased = 0 ;
	Created = 0 ;
	Renamed = 0 ;
	Attributes = 0 ;
	Size = 0 ;
	Data = 0 ;
}


m4bool_t	ClComparisonResultMask::AnyChange( void ) const
{
	// M4_TRUE if any member is 1, M4_FALSE otherwise
	if( Data || Size || Attributes || Renamed || Created || Erased )
	{
		return( M4_TRUE ) ;
	}
	return( M4_FALSE ) ;
}


ClComparisonResultMask::operator m4uint32_t( void ) const
{
	m4uint32_t	iMask = 0 ;

	if( Erased )
	{
		iMask |= COMPARISON_MASK_ERASED ;
	}

	if( Created )
	{
		iMask |= COMPARISON_MASK_CREATED ;
	}

	if( Renamed )
	{
		iMask |= COMPARISON_MASK_RENAMED ;
	}

	if( Attributes )
	{
		iMask |= COMPARISON_MASK_ATTRIBUTES ;
	}

	if( Size )
	{
		iMask |= COMPARISON_MASK_SIZE ;
	}

	if( Data )
	{
		iMask |= COMPARISON_MASK_DATA ;
	}

	return( iMask ) ;
}


void	ClComparisonResultMask::AdjustFlags( void )
{
	// If erased or creation
	if( Erased == 1 || Created == 1 )
	{
		Attributes = 1 ;
		Size = 1 ;
		Data = 1 ;
	}
	if( Size == 1 )
	{
		Data = 1 ;
	}
}




// ============================================================================
// Class ClFileName
// ============================================================================

void	ClFileName::_Init( m4pcchar_t ai_pccFileName )
{

	m4bool_t	bLastBackslash = M4_FALSE ;
	int			iLength = 0 ;
	m4pchar_t	pcFileName = NULL ;
	m4pchar_t	pcANSIFileName = NULL ;
	m4pchar_t	pcCppAbsolute = NULL ;
	m4char_t	acAbsolute[ M4_MAX_PATH + 1 ] ;


	m_iLength = 0 ;
	m_pcFileName = NULL ;

	if( ai_pccFileName == NULL || *ai_pccFileName == '\0' )
	{
		return ;
	}

	iLength = (int) strlen( ai_pccFileName ) ;

	if( iLength >= M4_MAX_PATH )
	{
		return ;
	}

	if( ai_pccFileName[ iLength - 1 ] == M4_FM_BACK_SLASH )
	{
		bLastBackslash = M4_TRUE ;
	}

	iLength = -1 ;
	pcANSIFileName = M4CppToANSI( ai_pccFileName, iLength ) ;

	if( pcANSIFileName == NULL )
	{
		return ;
	}

#ifdef _WINDOWS
	pcFileName = _fullpath( acAbsolute, pcANSIFileName, M4_MAX_PATH ) ;
#else
	FILE*	pfFile = fopen( pcANSIFileName, "r" ) ;

	if( pfFile != NULL )
	{
		// Existe el fichero
		fclose( pfFile ) ;
		pfFile = NULL ;
		pcFileName = realpath( pcANSIFileName , acAbsolute ) ;
	}
	else
	{
		// No existe. Intentamos crearlo para llamar a realpath
		// y luego lo borramos para dejar las cosas como estaban
		pfFile = fopen( pcANSIFileName, "w" ) ;

		if( pfFile != NULL )
		{
			fclose( pfFile ) ;
			pfFile = NULL ;
			pcFileName = realpath( pcANSIFileName , acAbsolute ) ;
			remove( pcANSIFileName ) ;
		}
	}
#endif

	delete( pcANSIFileName ) ;

	if( pcFileName == NULL )
	{
		strcpy( acAbsolute, ai_pccFileName ) ;
	}
	else
	{
		iLength = -1 ;
		pcCppAbsolute = M4ANSIToCpp( pcFileName, iLength ) ;

		if( pcCppAbsolute == NULL )
		{
			return ;
		}
		strcpy( acAbsolute, pcCppAbsolute ) ;
		delete( pcCppAbsolute ) ;
	}
	
	iLength = (int) strlen( acAbsolute ) ;

	if( bLastBackslash == M4_FALSE )
	{
		if( iLength > 0 && acAbsolute[ iLength - 1 ] == M4_FM_BACK_SLASH )
		{
			acAbsolute[ iLength - 1 ] = '\0' ;
			iLength--;
		}
	}
	else
	{
		if( iLength == 0 || acAbsolute[ iLength - 1 ] != M4_FM_BACK_SLASH )
		{
			acAbsolute[ iLength ] = M4_FM_BACK_SLASH ;
			acAbsolute[ iLength + 1 ] = '\0' ;
			iLength++ ;
		}
	}

	if( iLength > M4_MAX_PATH )
	{
		iLength = M4_MAX_PATH ;
	}

	m_pcFileName = new m4char_t[ iLength + 1 ] ;

	if( m_pcFileName == NULL )
	{
		return ;
	}

	memcpy( m_pcFileName, acAbsolute, iLength ) ;
	m_pcFileName[ iLength ] = '\0' ;
	m_iLength = iLength + 1 ;
}


void	ClFileName::_Destroy( void )
{
	if( m_pcFileName != NULL )
	{
		delete( m_pcFileName ) ;
		m_pcFileName = NULL ;
	}
	m_iLength = 0 ;
}


ClFileName::ClFileName( void )
{
	_Init( NULL ) ;
}


ClFileName::ClFileName( m4pcchar_t ai_pccFileName )
{
	_Init( ai_pccFileName ) ;
}


ClFileName::ClFileName( const ClFileName& ai_roFileName )
{
	_Init( ai_roFileName.GetFileName() ) ;
}


ClFileName::~ClFileName( void )
{
	_Destroy() ;
}


ClFileName& ClFileName::operator = ( m4pcchar_t ai_pccFileName )
{
	SetFileName( ai_pccFileName ) ;
	return( *this ) ;
}


ClFileName& ClFileName::operator = ( const ClFileName& ai_roFileName )
{
	if( this != &ai_roFileName )
	{
		SetFileName( ai_roFileName.GetFileName() ) ;
	}
	return( *this ) ;
}


m4bool_t ClFileName::IsNull( void ) const
{
	return( m_pcFileName == NULL ) ;
}


m4bool_t ClFileName::IsValid( void ) const
{
	return( ClFileState( m_pcFileName ).IsValid() ) ;
}


m4bool_t ClFileName::IsFile( void ) const
{
	return( ClFileState( m_pcFileName ).IsFile() ) ;
}


m4bool_t ClFileName::IsDir( void ) const
{
	return( ClFileState( m_pcFileName ).IsDir() ) ;
}


m4return_t	ClFileName::SetFileName( m4pcchar_t ai_pccFileName )
{
	_Destroy() ;
	_Init( ai_pccFileName ) ;
	return( M4_SUCCESS ) ;
}


m4return_t	ClFileName::GetFileName( m4pchar_t ao_acFileName ) const
{
	if( m_pcFileName != NULL )
	{
		strcpy( ao_acFileName, m_pcFileName ) ;
		return( M4_SUCCESS ) ;
	}

	*ao_acFileName = '\0' ;
	return( M4_WARNING ) ;
}


m4return_t	ClFileName::GetShortName( string& ao_rsShortName ) const
{
	if( m_pcFileName != NULL )
	{
		m4pchar_t	pcParse = m_pcFileName + strlen( m_pcFileName ) ;

		while( pcParse != m_pcFileName )
		{
			pcParse-- ;
			if( *pcParse == M4_FM_BACK_SLASH )
			{
				ao_rsShortName.assign( pcParse + 1 ) ;
				return( M4_SUCCESS ) ;
			}
		}

		ao_rsShortName.assign( m_pcFileName ) ;
		return( M4_SUCCESS ) ;
	}

	ao_rsShortName.erase() ;
	return( M4_WARNING ) ;
}


m4return_t	ClFileName::GetDir( ClFileName& ao_roDirName ) const
{
	if( m_pcFileName != NULL )
	{
		m4pchar_t pcParse = m_pcFileName + strlen( m_pcFileName ) ;

		while( pcParse != m_pcFileName )
		{
			pcParse-- ;
			if( *pcParse == M4_FM_BACK_SLASH )
			{
				*pcParse = '\0' ;
				ao_roDirName.SetFileName( m_pcFileName ) ;
				*pcParse = M4_FM_BACK_SLASH ;
				return( M4_SUCCESS ) ;
			}
		}
	}

	ao_roDirName.SetFileName( NULL ) ;
	return( M4_WARNING ) ;
}


m4return_t	ClFileName::GetFileExtension( m4pchar_t ao_acFileExtension ) const
{
	if( m_pcFileName != NULL )
	{
		m4pchar_t pcParse = m_pcFileName + strlen( m_pcFileName ) ;

		while( pcParse != m_pcFileName )
		{
			pcParse-- ;
			if( *pcParse == '.' )
			{
				strcpy( ao_acFileExtension, pcParse + 1 ) ;
				return( M4_SUCCESS ) ;
			}
		}
	}

	*ao_acFileExtension = '\0' ;
	return( M4_WARNING ) ;
}


// == operator
m4bool_t	operator == ( const ClFileName& a, const ClFileName& b )
{
	m4pcchar_t	pccA = a.GetFileName() ;
	m4pcchar_t	pccB = b.GetFileName() ;

	if( pccA == NULL && pccB == NULL )
	{
		return( M4_TRUE ) ;
	}

	if( pccA == NULL || pccB == NULL )
	{
		return( M4_FALSE ) ;
	}

#ifdef _WINDOWS
	return( strcmpi( pccA, pccB ) == 0 ) ;
#else
	return( strcmp ( pccA, pccB ) == 0 ) ;
#endif
}


// != operator
m4bool_t	operator != ( const ClFileName& a, const ClFileName& b )
{
	m4pcchar_t	pccA = a.GetFileName() ;
	m4pcchar_t	pccB = b.GetFileName() ;

	if( pccA == NULL && pccB == NULL )
	{
		return( M4_FALSE ) ;
	}

	if( pccA == NULL || pccB == NULL )
	{
		return( M4_TRUE ) ;
	}

#ifdef _WINDOWS
	return( strcmpi( pccA, pccB ) != 0 ) ;
#else
	return( strcmp ( pccA, pccB ) != 0 ) ;
#endif
}


// < operator
m4bool_t	operator < ( const ClFileName& a, const ClFileName& b )
{
	m4pcchar_t	pccA = a.GetFileName() ;
	m4pcchar_t	pccB = b.GetFileName() ;

	if( pccB == NULL )
	{
		return( M4_FALSE ) ;
	}

	if( pccA == NULL )
	{
		return( M4_TRUE ) ;
	}

#ifdef _WINDOWS
	return( strcmpi( pccA, pccB ) < 0 ) ;
#else
	return( strcmp ( pccA, pccB ) < 0 ) ;
#endif
}




// ============================================================================
// Class ClFileState
// ============================================================================

void	ClFileState::_Reset( void )
{
	m_bIsValid		= M4_FALSE ;
	m_bIsDir		= M4_FALSE ;
	m_iSize			= 0 ;
	m_iAttributes	= 0 ;
	m_dLastWrite	= 0 ;
	m_dLastGmtWrite	= 0 ;
}


void	ClFileState::_Init( const ClFileState& ai_roFileState )
{
	m_bIsValid		= ai_roFileState.m_bIsValid ;
	m_bIsDir		= ai_roFileState.m_bIsDir ;
	m_iSize			= ai_roFileState.m_iSize ;
	m_iAttributes	= ai_roFileState.m_iAttributes ;
	m_dLastWrite	= ai_roFileState.m_dLastWrite ;
	m_dLastGmtWrite	= ai_roFileState.m_dLastGmtWrite ;
}


ClFileState::ClFileState( void )
{
	_Reset() ; 
}


ClFileState::ClFileState( m4pcchar_t ai_pccFileName )
{
	SetFileName( ai_pccFileName ) ;
}


ClFileState::ClFileState( const ClFileState& ai_roFileState )
{
	_Init( ai_roFileState ) ;
}


ClFileState& ClFileState::operator = ( const ClFileState& ai_roFileState )
{
	if( this != &ai_roFileState )
	{
		_Init( ai_roFileState ) ;
	}
	return( *this ) ;
}


void	ClFileState::SetFileName( m4pcchar_t ai_pccFileName )
{
	_Reset() ;

	if( ai_pccFileName == NULL || *ai_pccFileName == '\0' )
	{
		return ;
	}

	struct stat	oStatus ;

	if( M4FileStatus( ai_pccFileName, oStatus ) != 0 )
	{
		return ;
	}

	m_bIsValid = M4_TRUE ;

	if( oStatus.st_mode & S_IFDIR )
	{
		// It is a directory
		m_bIsDir = M4_TRUE ;
	}

	m_iSize	= oStatus.st_size ;

#ifdef _WINDOWS
	int			iLength = -1 ;
	m4pchar_t	pcFileName = M4CppToANSI( ai_pccFileName, iLength ) ;

	m_iAttributes = GetFileAttributes( pcFileName ) ;
	delete( pcFileName ) ;
#endif

	// Bug 0312525 Si la fehca es -1 no se intenta convertir
	if( oStatus.st_mtime > 0 )
	{
		m_dLastWrite	= ClTime_tToDate( oStatus.st_mtime, M4CL_TIME_LOCAL_TIME ) ;
		m_dLastGmtWrite	= ClTime_tToDate( oStatus.st_mtime, M4CL_TIME_GM_TIME ) ;
	}
}


void	ClFileState::CompareAndSetMask( const ClFileState& ai_roNewState, ClComparisonResultMask& ao_roDiffMask ) const
{
	ao_roDiffMask.Reset() ;

	// If not erase or creation
	if( m_bIsValid == M4_TRUE && ai_roNewState.m_bIsValid == M4_TRUE )
	{
		if( m_iAttributes != ai_roNewState.m_iAttributes )
		{
			ao_roDiffMask.Attributes = 1 ;
		}

		/* Bug 0069087
		Hay que verificar que las fechas sean iguales, pero según su precisión.
		Si no lo que pasa es que saltan eventos de que el fichero se ha modificado que no son ciertos.
		En el caso de este bug eso hace que el canal quede en un estado incorrecto para la serialización.
		*/
		if( fabs( m_dLastWrite - ai_roNewState.m_dLastWrite ) >= M4_LN4_DATE_PRECISION )
		{
			ao_roDiffMask.Data = 1 ;
		}

		if( m_iSize != ai_roNewState.m_iSize )
		{
			ao_roDiffMask.Size = 1 ;
		}
	}
	else if( m_bIsValid == M4_TRUE && ai_roNewState.m_bIsValid == M4_FALSE )
	{
		// Erased
		ao_roDiffMask.Erased = 1 ;
	}
	else if( m_bIsValid == M4_FALSE && ai_roNewState.m_bIsValid == M4_TRUE )
	{
		// Creation
		ao_roDiffMask.Created = 1 ;
	}

	ao_roDiffMask.AdjustFlags() ;
}


// == operator
m4bool_t	operator == ( const ClFileState& a, const ClFileState& b )
{
	ClComparisonResultMask	oDiffMask ;

	a.CompareAndSetMask( b, oDiffMask ) ;
	return( oDiffMask.AnyChange() == M4_FALSE ) ;
}


// != operator
m4bool_t	operator != ( const ClFileState& a, const ClFileState& b )
{
	ClComparisonResultMask	oDiffMask ;

	a.CompareAndSetMask( b, oDiffMask ) ;
	return( oDiffMask.AnyChange() == M4_TRUE ) ;
}


