
//==============================================================================
//
// (c) Copyright  1991-2012 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4log.dll
// File:                m4logfile.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                27-02-2012
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the class for handling log files dumping and backup
//
//
//==============================================================================


#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#ifdef _UNIX
#include <dirent.h>
#endif

#include "m4stl.hpp"
#include "m4logfile.hpp"
#include "regedit.hpp"
#include "cldefine.hpp"
#include "m4regdef.hpp"
#include "m4regimpl.hpp"
#include "m4clock.hpp"
#include "cldates.hpp"
#include "m4thread.hpp"
#include "m4unicode.hpp"
#include "m4buildversion.hpp"
#include "blocksynchronization.hpp"




//=================================================================================
//
// M4LogFile
//
// Clase que implementa los ficheros de traza
//
//=================================================================================


m4date_t	M4LogFile::sm_dMaxPerdiod = 1 ;



//=================================================================================
// Hace una copia de una cadena
//=================================================================================

m4pchar_t	NewString( m4pcchar_t ai_pccString )
{

	m4pchar_t	pcResult = NULL ;
	size_t		iLength = 0 ;


	if( ai_pccString != NULL )
	{
		iLength = strlen( ai_pccString ) ;
		pcResult = new m4char_t[ iLength + 1 ] ;

		if( pcResult != NULL )
		{
			memcpy( pcResult, ai_pccString, iLength + 1 ) ;
		}
	}

	return( pcResult ) ;
}



//=================================================================================
// Libera una cadena y la pone a nulo
//=================================================================================

void	DeleteString( m4pchar_t &aio_rpcString )
{
	if( aio_rpcString != NULL )
	{
		delete( aio_rpcString ) ;
		aio_rpcString = NULL ;
	}
}



//=================================================================================
// Obtiene el directorio de depuración
//=================================================================================

m4pchar_t	M4LogFile::_GetDirectory( void )
{

	m4return_t	iResult = M4_ERROR ;
	int			iLength = -1 ;
	m4uint32_t	iSize = 0 ;
	m4pcchar_t	pccSide = NULL ;
	m4pcchar_t	pccDebugDir = NULL ;
	m4char_t	acPath[ M4CH_MAX_TEMP_BUFFER + 1 ] = "" ;
	m4char_t	acDirectory[ M4LOG_MAX_PATH + 1 ] ;


	if( m_pcDirectory == NULL )
	{
		// Si no se tiene el directorio se busca en el registros
		if( getenv( "M4SERVER_PATH" ) != NULL )
		{
			pccSide = M4_NEW_REGEDIT_KEY_SERVER ;
		}
		else
		{
			pccSide = M4_NEW_REGEDIT_KEY_CLIENT ;
		}

		iResult = M4ComposeRegistryPath( pccSide, M4_NEW_REGEDIT_KEY_CVM_LOG, acPath, M4CH_MAX_TEMP_BUFFER, iSize ) ;

		if( iResult == M4_SUCCESS )
		{
			if( GetRegKeyData( acPath, M4_INIT_DEBUG_DIR, acDirectory ) == TRUE )
			{
				m_pcDirectory = NewString( acDirectory ) ;
			}
		}

		// Si no está en el registro se toma el temporal
		if( m_pcDirectory == NULL )
		{
			pccDebugDir = getenv( M4LOG_DEBUG_DIR ) ;

			if( pccDebugDir != NULL )
			{
				iLength = -1 ;
				m_pcDirectory = M4ANSIToCpp( pccDebugDir, iLength ) ;
			}
			else
			{
				// Si no se tiene temporal se pone el directorio actual
				m_pcDirectory = NewString( "." ) ;
			}
		}
	}

	return( m_pcDirectory ) ;
}



//=================================================================================
// Compone el nombre completo del fichero, normal o como backup coon una fecha
//=================================================================================

void	M4LogFile::_ComposeFileName( m4pchar_t ao_pcFile, m4date_t ai_dDate )
{

	m4int_t		iYear = 0 ;
	m4int_t		iMonth = 0 ;
	m4int_t		iDay = 0 ;
	m4int32_t	iHour = 0 ;
	m4int32_t	iMinute = 0 ;
	m4int32_t	iSecond = 0 ;


	strcpy( ao_pcFile, _GetDirectory() ) ;
	strcat( ao_pcFile, M4_BACK_SLASH ) ;
	strcat( ao_pcFile, m_pcName ) ;

	if( ai_dDate != 0 )
	{
        ClDateToNumbers( ai_dDate, iYear, iMonth, iDay, iHour, iMinute, iSecond ) ;
		sprintf( ao_pcFile + strlen( ao_pcFile ), "_%04d-%02d-%02d@%02d%02d%02d", iYear, iMonth, iDay, iHour, iMinute, iSecond ) ;
	}

	strcat( ao_pcFile, "." ) ;
	strcat( ao_pcFile, m_pcExtension ) ;
}



//=================================================================================
// Carga la información de los ficheros de backup que hay en disco
//=================================================================================

void	M4LogFile::_LoadBackUps( void )
{

	int				iLength = -1 ;
	int				iScan = 0 ;
	m4int_t			iYear = 0 ;
	m4int_t			iMonth = 0 ;
	m4int_t			iDay = 0 ;
	m4int32_t		iHour = 0 ;
	m4int32_t		iMinute = 0 ;
	m4int32_t		iSecond = 0 ;
	m4date_t		dDate = 0 ;
	m4pchar_t		pcFile = NULL ;
	m4char_t		acFile[ M4LOG_MAX_PATH + 1 ] = "" ;
	m4char_t		acPattern[ M4LOG_MAX_PATH + 1 ] = "" ;
	list<m4date_t>	oDates ;

	list<m4date_t>::iterator	it ;


	// La primera vez se crea la cola de backups
	if( m_poBackUps != NULL )
	{
		return ;
	}

	m_poBackUps = new BackUpList_t() ;

	if( m_poBackUps == NULL )
	{
		return ;
	}

	// Se leen los ficheros de backup de disco y se almacenan en una lista temporal
	strcpy( acPattern, m_pcName ) ;
	strcat( acPattern, "_%04d-%02d-%02d@%02d%02d%02d" ) ;
	strcat( acPattern, "." ) ;
	strcat( acPattern, m_pcExtension ) ;

#ifdef _WINDOWS

	HANDLE			hFind = 0 ;
	WIN32_FIND_DATA	oFindFileData ;

	strcpy( acFile, _GetDirectory() ) ;
	strcat( acFile, M4_BACK_SLASH ) ;
	strcat( acFile, m_pcName ) ;
	strcat( acFile, "_*-*-*@*" ) ;
	strcat( acFile, "." ) ;
	strcat( acFile, m_pcExtension ) ;

	iLength = -1 ;
	pcFile = M4CppToANSI( acFile, iLength ) ;
	hFind = FindFirstFile( pcFile, &oFindFileData ) ;
	delete( pcFile ) ;

	if( hFind != INVALID_HANDLE_VALUE )
	{
		do
		{
			iScan = sscanf( oFindFileData.cFileName, acPattern, &iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond ) ;

			if( iScan == 6 )
			{
				ClNumbersToDate( iYear, iMonth, iDay, iHour, iMinute, iSecond, dDate ) ;
				oDates.push_back( dDate ) ;
			}
		}
		while( FindNextFile( hFind, &oFindFileData ) != 0 ) ;

		FindClose( hFind ) ;
	}
#else

	DIR*			poDir = NULL ;
	struct dirent	*poDirEntry = NULL ;
	struct dirent	*poDirAux = NULL ;
		
	strcpy( acFile, _GetDirectory() ) ;

	iLength = -1 ;
	pcFile = M4CppToANSI( acFile, iLength ) ;
	poDir = opendir( pcFile ) ;
	delete( pcFile ) ;

	if( poDir != NULL )
	{
#ifdef _SOLARIS
		poDirEntry = (struct dirent *) new m4uint32_t[ M4_ROUND_TO_4( sizeof( struct dirent ) + M4LOG_MAX_PATH ) ] ;
		while( readdir_r( poDir, poDirEntry ) != NULL )
#else
		poDirEntry = new struct dirent() ;
		while( readdir_r( poDir, poDirEntry, &poDirAux ) == 0 )
#endif
		{
			iScan = sscanf( poDirEntry->d_name, acPattern, &iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond ) ;

			if( iScan == 6 )
			{
				ClNumbersToDate( iYear, iMonth, iDay, iHour, iMinute, iSecond, dDate ) ;
				oDates.push_back( dDate ) ;
			}
		}
		delete(  poDirEntry ) ;
		closedir( poDir ) ;
	}
#endif

	// Se añaden los ficheros de manera ordenada a la cola de backups
	if( oDates.size() > 0 )
	{
		oDates.sort();

		it = oDates.begin() ;
		while( it != oDates.end() )
		{
			_AddBackUp( *it++ ) ;
		}
	}

	// Se hace la limpieza de backups
	_CleanBackUps() ;
}



//=================================================================================
// Añade un fichero a la cola de backups
//=================================================================================

void	M4LogFile::_AddBackUp( m4date_t ai_dDate )
{

	int			iLength = -1 ;
	m4pchar_t	pcFile = NULL ;
	m4char_t	acFile[ M4LOG_MAX_PATH + 1 ] ;
	struct stat oStatus ;
	StBackUp_t	oBackUp ;


	if( m_poBackUps == NULL )
	{
		return ;
	}

	oBackUp.m_dDate = ai_dDate ;
	oBackUp.m_iSize = 0 ;

	if( m_iTotalSize != 0 )
	{
		// Si hay gestión de tamaño total se lee el tamaño del fichero
		_ComposeFileName( acFile, ai_dDate ) ;

		iLength = -1 ;
		pcFile = M4CppToANSI( acFile, iLength ) ;

		if( stat( pcFile, &oStatus) == 0 )
		{
			oBackUp.m_iSize = oStatus.st_size ;
		}
		delete( pcFile ) ;
	}

	// Se añade a la cola y se ajusta el tamaño total
	m_poBackUps->push( oBackUp ) ;
	m_iTotalLength += oBackUp.m_iSize ;
}



//=================================================================================
// Borra los ficheros obsoletos o que excedan el tamaño
//=================================================================================

void	M4LogFile::_CleanBackUps( void )
{

	int			iLength = -1 ;
	m4date_t	dCutDate = 0 ;
	m4pchar_t	pcFile = NULL ;
	m4char_t	acFile[ M4LOG_MAX_PATH + 1 ] ;
	StBackUp_t	oBackUp ;


	if( m_poBackUps == NULL )
	{
		return ;
	}

	if( m_poBackUps->size() > 0 )
	{
		ClActualDate( dCutDate ) ;
		dCutDate -= sm_dMaxPerdiod ;

		while( m_poBackUps->empty() == M4_FALSE )
		{
			oBackUp = m_poBackUps->front() ;

			if( oBackUp.m_dDate >= dCutDate && m_iTotalLength <= m_iTotalSize )
			{
				// Ya no queda nada que borrar
				break ;
			}

			m_poBackUps->pop() ;
			m_iTotalLength -= oBackUp.m_iSize ;

			_ComposeFileName( acFile, oBackUp.m_dDate ) ;

			iLength = -1 ;
			pcFile = M4CppToANSI( acFile, iLength ) ;
			unlink( pcFile ) ;
			delete( pcFile ) ;
		}
	}
}



//=================================================================================
// Cierra el fichero actual y hace un backup de él
//=================================================================================

void	M4LogFile::_Rename( void )
{

	int			iRename = 0 ;
	m4date_t	dDate = 0 ;
	m4pchar_t	pcError = NULL;
	m4char_t	acNewFile[ M4LOG_MAX_PATH + 1 ] ;
	m4char_t	acOldFile[ M4LOG_MAX_PATH + 1 ] ;


	if( m_pfFile != NULL )
	{
		fclose( m_pfFile ) ;
		m_pfFile = NULL ;
	}

	m_iLength = 0 ;

	ClActualDate( dDate ) ;

	_ComposeFileName( acNewFile, dDate ) ;
	_ComposeFileName( acOldFile ) ;

	iRename = M4RenameFile( acOldFile, acNewFile, &pcError ) ;

	if( pcError != NULL )
	{
		WriteLine( "Rename Error: %s", pcError ) ;
		delete( pcError ) ;
		pcError = NULL ;
	}

	if( iRename == 0 )
	{
		// Si se ha podido renombrar se añade a la cola de backups
		_AddBackUp( dDate ) ;
	}

	// Se hace la limpieza de backups
	_CleanBackUps() ;
}



//=================================================================================
// Opens log file if it is not open
//=================================================================================

bool	M4LogFile::_Open( void )
{

	m4bool_t				bNewFile = M4_FALSE ;
	int						iLength = -1 ;
	m4pchar_t				pcFile = NULL ;
	eUniFileEncodingType_t	eEncoding = M4UniANSI ;
    m4char_t				acBuffer[ 128 + 1 ] ;
	m4char_t				acFile[ M4LOG_MAX_PATH + 1 ] ;
	struct stat				oStatus ;


	if( m_pfFile != NULL )
	{
		return( M4_FALSE ) ;
	}

	_ComposeFileName( acFile ) ;

	// Si es la primera vez se cargan los backups y se calcula el tamaño del actual
	if( m_poBackUps == NULL )
	{
		_LoadBackUps() ;

		iLength = -1 ;
		pcFile = M4CppToANSI( acFile, iLength ) ;

		if( stat( pcFile, &oStatus) == 0 )
		{
			m_iLength = oStatus.st_size ;
		}
		delete( pcFile ) ;
	}

	// Se crea el fichero y se escribe la cabecera
	if( m_bIsUnicode == M4_TRUE )
	{
		eEncoding = M4UniNative ;
	}

	m_pfFile = M4Fopen( acFile, M4UniAppend, eEncoding ) ;

	if( m_pfFile != NULL )
	{
		bNewFile = M4_TRUE ;

		if( m_iLength > 0 )
		{
			ClDateToString( m_bStart, acBuffer, M4_SECOND_END + 1 ) ;
			m_iLength += fwrite( "\n\n", sizeof( m4char_t ), 2, m_pfFile ) ;
			m_iLength += fwrite( acBuffer, sizeof( m4char_t ), strlen( acBuffer ), m_pfFile ) ;
			m_iLength += fwrite( M4LOG_START, sizeof( m4char_t ), strlen( M4LOG_START ), m_pfFile ) ;
		}

		if( m_pcHeader != NULL )
		{
			m_iLength += fwrite( m_pcHeader, sizeof( m4char_t ), strlen( m_pcHeader ), m_pfFile ) ;
			m_iLength += fwrite( "\n", sizeof( m4char_t ), 1, m_pfFile ) ;
#ifdef _WINDOWS
			m_iLength++ ;
#endif
		}
	}

	return( bNewFile ) ;
}



//=================================================================================
// Escribe una cadena en el fichero
//=================================================================================

m4bool_t	M4LogFile::_WriteString( m4pcchar_t ai_pccString )
{

	m4bool_t	bNewFile = M4_FALSE ;


	if( ai_pccString == NULL )
	{
		return( M4_FALSE ) ;
	}

	bNewFile = _Open() ;

	// Se escribe la cadena
	if( m_pfFile != NULL )
	{
		m_iLength += fwrite( ai_pccString, sizeof( m4char_t ), strlen( ai_pccString ), m_pfFile ) ;

#ifdef _WINDOWS
		if( *ai_pccString == '\n' )
		{
			m_iLength++ ;
		}
#endif
	}

	return( bNewFile ) ;
}



//=================================================================================
// Escribe una cadena de argumentos variables en el fichero
//=================================================================================

m4bool_t	M4LogFile::_WriteString( m4pcchar_t ai_pccFormat, va_list ai_vaList )
{

	m4bool_t	bNewFile = M4_FALSE ;


	bNewFile = _Open() ;

	// Se escribe la cadena variable
	if( m_pfFile != NULL )
	{
		m_iLength += vfprintf( m_pfFile, ai_pccFormat, ai_vaList ) ;
	}

	return( bNewFile ) ;
}



//=================================================================================
// Escribe el comienzo de una línea en el fichero
//=================================================================================

m4bool_t	M4LogFile::_WriteLineStart( void )
{

	m4bool_t	bNewFile = M4_FALSE ;
    m4char_t	acBuffer[ 128 + 1 ] ;


	// Se escribe el tickcount si se pide
	if( m_bTick == M4_TRUE )
	{
		sprintf( acBuffer, M4LOG_LONG_FORMAT "\t", ClPrecisionTicker::GetTimeTick() ) ;
		bNewFile |= _WriteString( acBuffer ) ;
	}

	// Se escribe la fecha si se pide
	if( m_bTime == M4_TRUE )
	{
		ClActualDate( acBuffer, M4_SECOND_END + 1 ) ;
		bNewFile |= _WriteString( acBuffer ) ;
		bNewFile |= _WriteString( "\t" ) ;
	}

	// Se escribe el thread si se pide
	if( m_bThread == M4_TRUE )
	{
		sprintf( acBuffer, "%d\t", M4Thread::GetCurrentIdThread() ) ;
		bNewFile |= _WriteString( acBuffer ) ;
	}

	return( bNewFile ) ;
}



//=================================================================================
// Escribe el final de una línea en el fichero
//=================================================================================

m4bool_t	M4LogFile::_WriteLineEnd( void )
{

	m4bool_t	bNewFile = M4_FALSE ;


	bNewFile |= _WriteString( "\n" ) ;

	if( m_pfFile != NULL )
	{
		fflush( m_pfFile ) ;
	}

	// Si el fichero llega al tamaño se hace un backup
	if( m_iLength > m_iSize )
	{
		_Rename() ;
	}

	return( bNewFile ) ;
}


//=================================================================================
// Inicializa la clase
//=================================================================================

M4LogFile::M4LogFile()
{
}

//=================================================================================
// Inicializa la clase a partir de los valores pasados
//=================================================================================

M4LogFile::M4LogFile( m4pcchar_t ai_pccName, m4uint32_t ai_iSize, m4pcchar_t ai_pccHeader, m4pcchar_t ai_pccExtension, m4bool_t ai_bTick, m4bool_t ai_bTime, m4bool_t ai_bThread, m4bool_t ai_bIsUnicode, m4uint32_t ai_iTotalSize )
{
	_Init( ai_pccName, ai_iSize, ai_pccHeader, ai_pccExtension, ai_bTick, ai_bTime, ai_bThread, ai_bIsUnicode, ai_iTotalSize );
}



//=================================================================================
// Libera la memoria de la clase
//=================================================================================

M4LogFile::~M4LogFile( void )
{
	DeleteString( m_pcName ) ;
	DeleteString( m_pcHeader ) ;
	DeleteString( m_pcExtension ) ;
	DeleteString( m_pcDirectory ) ;

	if( m_pfFile != NULL )
	{
		fclose( m_pfFile ) ;
		m_pfFile = NULL ;
	}

	if( m_poMutex != NULL )
	{
		delete( m_poMutex ) ;
		m_poMutex = NULL ;
	}

	if( m_poBackUps != NULL )
	{
		delete( m_poBackUps ) ;
		m_poBackUps = NULL ;
	}
}

//=================================================================================
// Inicializa la clase a partir de los valores pasados
//=================================================================================

void M4LogFile::_Init( m4pcchar_t ai_pccName, m4uint32_t ai_iSize, m4pcchar_t ai_pccHeader, m4pcchar_t ai_pccExtension, m4bool_t ai_bTick, m4bool_t ai_bTime, m4bool_t ai_bThread, m4bool_t ai_bIsUnicode, m4uint32_t ai_iTotalSize )
{
	m_bStart = ClActualDate() ;
	m_bTick = ai_bTick ;
	m_bTime = ai_bTime ;
	m_bThread = ai_bThread ;
	m_bIsUnicode = ai_bIsUnicode ;
	m_iLength = 0 ;
	m_iSize = ai_iSize ;
	m_iTotalLength = 0 ;
	m_iTotalSize = ai_iTotalSize ;
	m_pcName = NewString( ai_pccName ) ;
	m_pcHeader = NewString( ai_pccHeader ) ;
	m_pcExtension = NewString( ai_pccExtension ) ;
	m_pcDirectory = NULL ;
	m_pfFile = NULL ;
	m_poMutex = new ClMutex( M4_TRUE ) ;
	m_poBackUps = NULL ;
}

//=================================================================================
// Escribe una cadena en el fichero
//=================================================================================

m4bool_t	M4LogFile::WriteString( m4pcchar_t ai_pccString )
{
	// Se escibe sin bloquear
	return( _WriteString( ai_pccString ) ) ;
}



//=================================================================================
// Escribe el comienzo de una línea en el fichero
//=================================================================================

m4bool_t	M4LogFile::WriteLineStart( void )
{
	m_poMutex->Lock() ;
	return( _WriteLineStart() ) ;
}



//=================================================================================
// Escribe el final de una línea en el fichero
//=================================================================================

m4bool_t	M4LogFile::WriteLineEnd( void )
{

	m4bool_t	bNewFile = M4_FALSE ;

		
	bNewFile |= _WriteLineEnd() ;
	m_poMutex->Unlock() ;

	return( bNewFile ) ;
}



//=================================================================================
// Escribe una línea en el fichero
//=================================================================================

m4bool_t	M4LogFile::WriteLine( m4pcchar_t ai_pccFormat, ... )
{

	m4bool_t	bNewFile = M4_FALSE ;
	va_list		vaList;


	// Siempre se bloquea para escribir
	ClMutBlock oMutexBlock( m_poMutex ) ;

	va_start( vaList, ai_pccFormat ) ;
	bNewFile |= _WriteLineStart() ;
	bNewFile |= _WriteString( ai_pccFormat, vaList ) ;
	bNewFile |= _WriteLineEnd() ;
	va_end( vaList ) ;

	return( bNewFile ) ;
}



