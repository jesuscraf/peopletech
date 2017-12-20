
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ipcs.dll
// File:                m4pprocess.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                23-12-2014
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements a class for child processes connected by pipes.
//
//
//==============================================================================

#include "m4pprocess.hpp"
#include "m4unicode.hpp"
#include "m4clock.hpp"
#include "m4date.hpp"
#include "iexecutor.hpp"
#include <errno.h>

extern int	M4IsUnicode( void ) ;



#define	M4PP_BUFFER_SIZE						4096
#define M4PP_CHECK_ERROR( condition, error )	do { if( condition ) { _SetError( error ) ; return( M4_ERROR ) ; } } while( 0 )

#ifdef _UNIX
	#include <unistd.h>
	#include <fcntl.h>
	#include <spawn.h>
	extern char **environ;

	#define	M4_WINDOWS_PLATFORM				"0"
	#define	M4_ENOMEM						ENOMEM
	#define M4_EINVAL						EINVAL
	#define M4_EPIPE						EPIPE
	#define M4_EAGAIN						EAGAIN

	#define	error()							errno
	#define	pipe( phandles )				pipe( phandles )
	#define	fdnoinherit( fd )				fcntl( fd, F_SETFD, FD_CLOEXEC )
	#define fdnonblocking( fd, result )		result = fcntl( fd, F_SETFL, fcntl( fd, F_GETFL ) | O_NONBLOCK )
	#define fdblocking( fd, result )		result = fcntl( fd, F_SETFL, fcntl( fd, F_GETFL ) & ~O_NONBLOCK )
	#define	osfhandle( fd )					fd
	#define	spawn( cmdname, argv, pid )		posix_spawn( &pid, cmdname, NULL, NULL, argv, environ ) == 0 ? 0 : -1
	#define	kill( pid )						kill( pid, SIGKILL )
#else
	#include <io.h>
	#include <fcntl.h>
	#include <process.h>

	#define	M4_WINDOWS_PLATFORM				"1"
	#define	M4_ENOMEM						ERROR_NOT_ENOUGH_MEMORY
	#define M4_EINVAL						ERROR_BAD_ARGUMENTS
	#define M4_EPIPE						ERROR_BROKEN_PIPE
	#define M4_EAGAIN						ERROR_NO_DATA

	#define	error()							GetLastError()
	#define	pipe( phandles )				_pipe( phandles, 32 * 1024, _O_BINARY )
	#define	fdnoinherit( fd )				SetHandleInformation( (HANDLE)_get_osfhandle( fd ), HANDLE_FLAG_INHERIT, 0 ) == TRUE ? 0 : -1
	#define fdnonblocking( fd, result )		{ DWORD dwMode = PIPE_NOWAIT ; result = SetNamedPipeHandleState( (HANDLE)_get_osfhandle( fd ), &dwMode, NULL, NULL ) != 0 ? 0 : -1 ; }
	#define fdblocking( fd, result )		{ DWORD dwMode = PIPE_WAIT ; result = SetNamedPipeHandleState( (HANDLE)_get_osfhandle( fd ), &dwMode, NULL, NULL ) != 0 ? 0 : -1 ; }
	#define	osfhandle( fd )					_get_osfhandle( fd )
	#define	spawn( cmdname, argv, pid )		( pid = _spawnvp( _P_NOWAIT, cmdname, argv ) ) == -1 ? -1 : 0
	#define	kill( pid )						TerminateProcess( (HANDLE)pid, -1 ) == TRUE ? 0 : -1

static	m4bool_t _HasSpaces( m4pcchar_t ai_pccString )
{

	size_t		iLength = 0 ;
	m4pcchar_t	p = NULL ;


	if( ai_pccString == NULL )
	{
		return( M4_FALSE ) ;
	}

	iLength = strlen( ai_pccString ) ;

	if( iLength > 1 && ai_pccString[ 0 ] == '"' && ai_pccString[ iLength - 1 ] == '"' )
	{
		// Si está entre comillas no tiene espacios
		return( M4_FALSE ) ;
	}

	p = ai_pccString ;

	while( *p != '\0' )
	{
		if( isspace( *p++ ) != 0 )
		{
			return( M4_TRUE ) ;
		}
	}

	return( M4_FALSE ) ;
}
#endif


m4pchar_t	_FormatError( int ai_iError )
{

	int			iLength = -1 ;
	m4pchar_t	pcResult = NULL ;

#ifdef _UNIX
	pcResult = M4ANSIToCpp( strerror( ai_iError ), iLength ) ;
#else
	DWORD		dwResult = 0 ;
	LPSTR		pcBuffer = NULL ;

	dwResult = FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ai_iError, 0, (LPSTR)&pcBuffer, 0, NULL ) ;

	if( dwResult > 0 && pcBuffer != NULL )
	{
		if( dwResult >= 2 && pcBuffer[ dwResult - 2 ] == '\r' && pcBuffer[ dwResult - 1 ] == '\n' )
		{
			dwResult -= 2 ;
			pcBuffer[ dwResult ] = '\0' ;
		}
		pcResult = M4ANSIToCpp( pcBuffer, iLength ) ;
		LocalFree( pcBuffer ) ;
	}
	else
	{
		pcResult = M4ANSIToCpp( "No message available.", iLength ) ;
	}
#endif

	return( pcResult ) ;	
}



//=================================================================================
//
// M4PipedProcess
//
// Clase que define un proceso hijo enlazado por pipes
//
//=================================================================================


// Funciones de inicialización ================================================
	
M4PipedProcess::M4PipedProcess( void )
{
	_Init() ;
}


M4PipedProcess::M4PipedProcess( m4pcchar_t ai_apccArguments[], m4uint32_t ai_iLength )
{
	_Init() ;
	Open( ai_apccArguments, ai_iLength ) ;
}


M4PipedProcess::~M4PipedProcess( void )
{
	Close() ;
	_FreeBuffer() ;
}


m4return_t	M4PipedProcess::Open( m4pcchar_t ai_apccArguments[], m4uint32_t ai_iLength )
{

	int			iResult = -1 ;
	int			iLength = -1 ;
	m4uint32_t	i = 0 ;
	m4char_t	acUnicode[ 32 + 1 ] ;
	m4char_t	acHandle1[ 32 + 1 ] ;
	m4char_t	acHandle2[ 32 + 1 ] ;
	m4pchar_t	pcFile = NULL ;
	m4pchar_t	pcArgument = NULL ;
	m4pchar_t*	apcArguments = NULL ;


	Close() ;

	// Pipe de datos de padre a hijo
	iResult = pipe( m_aiParentToChild ) ;
	M4PP_CHECK_ERROR( iResult < 0, error() ) ;

	// Pipe de datos de hijo a padre
	iResult = pipe( m_aiChildToParent ) ;
	M4PP_CHECK_ERROR( iResult < 0, error() ) ;

	// No heredar los extremos del padre
	iResult = fdnoinherit( m_aiParentToChild[ 1 ] ) ;
	M4PP_CHECK_ERROR( iResult < 0, error() ) ;

	iResult = fdnoinherit( m_aiChildToParent[ 0 ] ) ;
	M4PP_CHECK_ERROR( iResult < 0, error() ) ;

	// Lanzamiento de proceso
	sprintf( acUnicode, "%d", M4IsUnicode() ) ;
	sprintf( acHandle1, M4_PRIdPTR, osfhandle( m_aiParentToChild[0] ) ) ;
	sprintf( acHandle2, M4_PRIdPTR, osfhandle( m_aiChildToParent[1] ) ) ;

	apcArguments = new m4pchar_t[ ai_iLength + 5 ] ;
	M4PP_CHECK_ERROR( apcArguments == NULL, M4_ENOMEM ) ;

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		iLength = -1 ;
		pcArgument = M4CppToANSI( ai_apccArguments[ i ], iLength ) ;

#ifdef _WINDOWS
		if( _HasSpaces( pcArgument ) == M4_TRUE )
		{
			// Si hay espacios se entrecomilla
			apcArguments[ i ] = new m4char_t[ iLength + 3 ] ;

			if( apcArguments[ i ] != NULL )
			{
				apcArguments[ i ][ 0 ] ='"' ;
				memcpy( apcArguments[ i ] + 1, pcArgument, iLength ) ;
				apcArguments[ i ][ iLength + 1 ] = '"' ;
				apcArguments[ i ][ iLength + 2 ] = '\0' ;
			}
			delete( pcArgument ) ;
		}
		else
#endif
		{
			apcArguments[ i ] = pcArgument ;
		}
	}
	
	apcArguments[ ai_iLength + 0 ] = M4_WINDOWS_PLATFORM ;
	apcArguments[ ai_iLength + 1 ] = acUnicode ;
	apcArguments[ ai_iLength + 2 ] = acHandle1 ;
	apcArguments[ ai_iLength + 3 ] = acHandle2 ;
	apcArguments[ ai_iLength + 4 ] = NULL ;

	iLength = -1 ;
	pcFile = M4CppToANSI( ai_apccArguments[ 0 ], iLength ) ;

	if( access( pcFile, 0 ) == -1 )
	{
		iResult = -1 ;
	}
	else
	{
		iResult = spawn( pcFile, apcArguments, m_iProcess ) ;
	}

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		if( apcArguments[ i ] != NULL )
		{
			delete( apcArguments[ i ] ) ;
			apcArguments[ i ] = NULL ;
		}
	}
	delete [] apcArguments ;
	delete( pcFile ) ;
	M4PP_CHECK_ERROR( iResult < 0, error() ) ;

	// Liberación de recursos. No se controla el retorno
	if( m_aiParentToChild[ 0 ] != -1 )
	{
		iResult = close( m_aiParentToChild[ 0 ] ) ;
		m_aiParentToChild[ 0 ] = -1 ;
	}

	if( m_aiChildToParent[ 1 ] != -1 )
	{
		iResult = close( m_aiChildToParent[ 1 ] ) ;
		m_aiChildToParent[ 1 ] = -1 ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4PipedProcess::Close( void )
{
	_SetError() ;
	return( M4_SUCCESS ) ;
}


// Funciones sencillas de lectura y escritura =================================

m4return_t	M4PipedProcess::Write( m4pvoid_t ai_pvMemory, m4uint32_t ai_iLength )
{

	int			iResult = -1 ;
	m4uint32_t	iStart = 0 ;
	m4uint32_t	iPending = 0 ;
	m4uint32_t	iBytes = 0 ;


	M4PP_CHECK_ERROR( m_iProcess == 0, M4_EPIPE ) ;

	if( ai_iLength == 0 )
	{
		return( M4_SUCCESS ) ;
	}

	M4PP_CHECK_ERROR( ai_pvMemory == NULL, M4_EINVAL ) ;

	iStart = 0 ;
	iPending = ai_iLength ;

	while( iPending > 0 )
	{
		iBytes = iPending > M4PP_BUFFER_SIZE ? M4PP_BUFFER_SIZE : iPending ;

		iResult = write( m_aiParentToChild[ 1 ], (char*)ai_pvMemory + iStart, iBytes ) ;
		M4PP_CHECK_ERROR( iResult == 0, M4_EPIPE ) ;
		M4PP_CHECK_ERROR( iResult < 0, error() ) ;

		iStart += iResult ;
		iPending -= iResult ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4PipedProcess::Read( m4pvoid_t ao_pvMemory, m4uint32_t ai_iLength )
{

	int			iResult = -1 ;
	m4uint32_t	iStart = 0 ;
	m4uint32_t	iPending = 0 ;
	m4uint32_t	iBytes = 0 ;


	M4PP_CHECK_ERROR( m_iProcess == 0, M4_EPIPE ) ;

	if( ai_iLength == 0 )
	{
		return( M4_SUCCESS ) ;
	}

	M4PP_CHECK_ERROR( ao_pvMemory == NULL, EINVAL ) ;

	iStart = 0 ;
	iPending = ai_iLength ;

	while( iPending > 0 )
	{
		iBytes = iPending > M4PP_BUFFER_SIZE ? M4PP_BUFFER_SIZE : iPending ;

		iResult = read( m_aiChildToParent[ 0 ], (char*)ao_pvMemory + iStart, iBytes ) ;
		M4PP_CHECK_ERROR( iResult == 0, M4_EPIPE ) ;
		M4PP_CHECK_ERROR( iResult < 0, error() ) ;

		iStart += iResult ;
		iPending -= iResult ;
	}

	return( M4_SUCCESS ) ;
}


// Funciones avanzadas de lectura y escritura =================================

m4return_t	M4PipedProcess::WriteString( m4pcchar_t ai_pccString, m4int32_t ai_iLength )
{

	m4return_t	iResult = M4_ERROR ;


	if( ai_pccString == NULL )
	{
		ai_iLength = 0 ;
	}
	else
	{
		if( ai_iLength < 0 )
		{
			ai_iLength = (m4int32_t) strlen( ai_pccString ) ;
		}
	}

	iResult = Write( &ai_iLength, sizeof( ai_iLength ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( ai_iLength > 0 )
	{
		iResult = Write( (m4pvoid_t)ai_pccString, ai_iLength ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4PipedProcess::ReadString( m4pchar_t &ao_rpcString, m4int32_t *ao_piLength, m4bool_t ai_bCopy )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	iLength = -1 ;
	m4pchar_t	pcString = NULL ;


	if( ao_piLength != NULL )
	{
		*ao_piLength = 0 ;
	}
	ao_rpcString = NULL ;

	iResult = Read( &iLength, sizeof( iLength ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( ai_bCopy == M4_TRUE )
	{
		pcString = new m4char_t[ iLength + 1 ] ;
	}
	else
	{
		pcString = _New( iLength + 1 ) ;
	}
	M4PP_CHECK_ERROR( pcString == NULL, M4_ENOMEM ) ;

	if( iLength > 0 )
	{
		iResult = Read( pcString, iLength ) ;

		if( iResult != M4_SUCCESS )
		{
			if( ai_bCopy == M4_TRUE )
			{
				delete( pcString ) ;
			}
			return( M4_ERROR ) ;
		}
	}

	pcString[ iLength ] = '\0' ;

	if( ao_piLength != NULL )
	{
		*ao_piLength = iLength ;
	}
	ao_rpcString = pcString ;

	return( M4_SUCCESS ) ;
}



m4return_t	M4PipedProcess::ReadByte( m4int8_t& ao_rbByte, m4uint32_t ai_iTimeOut, IExecutor* ai_poExecutor, m4return_t& ao_riCanceled )
{

	int				iResult = -1 ;
	int				iReturn = -1 ;
	int				iError = 0 ;
	m4uint32_t		iTimes = 0 ;
	m4millisec_t	endTime = 0 ;


	ao_rbByte = 0 ;
	ao_riCanceled = 0 ;

	M4PP_CHECK_ERROR( m_iProcess == 0, M4_EPIPE ) ;

	if( ai_poExecutor != NULL && ai_iTimeOut != 0 )
	{
		if( ai_iTimeOut > 0 )
		{
			endTime = ClPrecisionTicker::GetTimeTick() + ai_iTimeOut * 1000 ;
		}

		fdnonblocking( m_aiChildToParent[ 0 ], iReturn ) ;
		M4PP_CHECK_ERROR( iReturn < 0, error() ) ;

		do
		{
			iResult = read( m_aiChildToParent[ 0 ], &ao_rbByte, 1 ) ;

			if( iResult < 0 && ( iError = error() ) == M4_EAGAIN )
			{
				iTimes++ ;
				if( iTimes % 1000 == 0 )
				{
					if( ai_iTimeOut > 0 && ClPrecisionTicker::GetTimeTick() >= endTime )
					{
						ao_riCanceled = -2 ;
						break ;
					}

					if( ai_poExecutor != NULL && ai_poExecutor->GetCancelStatus() == M4_TRUE )
					{
						ao_riCanceled = -1 ;
						break ;
					}
				}
				m4usleep( 500 ) ;
			}
		}
		while( iResult < 0 && iError == M4_EAGAIN ) ;

		fdblocking( m_aiChildToParent[ 0 ], iReturn ) ;

		if( iResult > 0 )
		{
			// Este chequeo sólo si todo ha ido bien, sino hay que seguir haciendo cosas
			M4PP_CHECK_ERROR( iReturn < 0, error() ) ;
		}

		if( ao_riCanceled != 0 )
		{
			iReturn = kill( m_iProcess ) ;
			M4PP_CHECK_ERROR( iReturn < 0, error() ) ;

			Close() ;
		}
		else
		{
			// Si no se ha cancelado se controla el error del read normalmente
			M4PP_CHECK_ERROR( iResult == 0, M4_EPIPE ) ;
			M4PP_CHECK_ERROR( iResult < 0, error() ) ;
		}
	}
	else
	{
		iResult = read( m_aiChildToParent[ 0 ], &ao_rbByte, 1 ) ;
		M4PP_CHECK_ERROR( iResult == 0, M4_EPIPE ) ;
		M4PP_CHECK_ERROR( iResult < 0, error() ) ;
	}

	return( M4_SUCCESS ) ;
}


// Funciones internas =========================================================

void	M4PipedProcess::_Init( void )
{
	m_iProcess = 0 ;
	m_pcError = NULL ;

	m_aiParentToChild[ 0 ] = -1 ;
	m_aiParentToChild[ 1 ] = -1 ;
	m_aiChildToParent[ 0 ] = -1 ;
	m_aiChildToParent[ 1 ] = -1 ;

	m_iSize = 0 ;
	m_pcBuffer = NULL ;
}


void	M4PipedProcess::_SetError( int ai_iError )
{

	int	iResult = -1 ;

	
	// Ante cualquier error se cierra
	m_iProcess = 0 ;

	if( m_pcError != NULL )
	{
		delete( m_pcError ) ;
		m_pcError = NULL ;
	}

	if( ai_iError != -1 )
	{
		m_pcError = _FormatError( ai_iError ) ;
	}

	// Liberación de recursos. No se controla el retorno
	if( m_aiParentToChild[ 0 ] != -1 )
	{
		iResult = close( m_aiParentToChild[ 0 ] ) ;
		m_aiParentToChild[ 0 ] = -1 ;
	}

	if( m_aiParentToChild[ 1 ] != -1 )
	{
		iResult = close( m_aiParentToChild[ 1 ] ) ;
		m_aiParentToChild[ 1 ] = -1 ;
	}

	if( m_aiChildToParent[ 0 ] != -1 )
	{
		iResult = close( m_aiChildToParent[ 0 ] ) ;
		m_aiChildToParent[ 0 ] = -1 ;
	}

	if( m_aiChildToParent[ 1 ] != -1 )
	{
		iResult = close( m_aiChildToParent[ 1 ] ) ;
		m_aiChildToParent[ 1 ] = -1 ;
	}
}


void	M4PipedProcess::_FreeBuffer( void )
{
	if( m_pcBuffer != NULL )
	{
		delete( m_pcBuffer ) ;
		m_pcBuffer = NULL ;
	}
	m_iSize = 0 ;
}


m4pchar_t M4PipedProcess::_New( m4uint32_t ai_iSize )
{
	if( m_iSize < ai_iSize )
	{
		_FreeBuffer() ;

		if( ai_iSize < M4PP_BUFFER_SIZE )
		{
			ai_iSize = M4PP_BUFFER_SIZE ;
		}

		m_pcBuffer = new m4char_t[ ai_iSize ] ;

		if( m_pcBuffer != NULL )
		{
			m_iSize = ai_iSize ;
		}
	}

	return( m_pcBuffer ) ;
}
