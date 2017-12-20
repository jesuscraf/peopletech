
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4unifile.cpp   
// Project:             Meta4 PeopleNet    
// Author:              Meta Software M.S. , S.A
// Date:                13-11-2006
// Language:            C++
// Operating System:    ALL
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la definición de las funciones de fichero de unicode
//
//
//==============================================================================


#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "m4unicode.hpp"
#include "m4uniutil.hpp"

#ifdef _UNIX
#include <unistd.h>
#include <m4string.hpp>
#else
#include <direct.h>
#include <io.h>
#endif


//=================================================================================
// Defines
//=================================================================================
#define	M4UNI_MAX_CHARACTERS					1048576	// 1 MB



//=================================================================================
// Cabeceras de codificación de fichero
//=================================================================================
char	acUTF8BOM     [ M4UNI_UTF8_BOM_SIZE  ] = { 0xEFU, 0xBBU, 0xBFU } ;
char	acUTF16WinBOM [ M4UNI_UTF16_BOM_SIZE ] = { 0xFFU, 0xFEU } ;
char	acUTF16UnixBOM[ M4UNI_UTF16_BOM_SIZE ] = { 0xFEU, 0xFFU } ;

#ifdef _WINDOWS
char	acUTF16BOM[ M4UNI_UTF16_BOM_SIZE ] = { 0xFFU, 0xFEU } ;
#else
char	acUTF16BOM[ M4UNI_UTF16_BOM_SIZE ] = { 0xFEU, 0xFFU } ;
#endif


//=================================================================================
// Modo de apertura de ficheros
//=================================================================================

static char	aacFileOpenModes[][ 4 ] =
{
	"rt",
	"rb",
	"wt",
	"wb",
	"at",
	"ab",
	"r+t",
	"r+b"
} ;

static int	aiFileOpenModes[] =
{
	ios::in  | M4_IOS_OPEN_MODE_NO_CREATE,
	ios::in  | M4_IOS_OPEN_MODE_NO_CREATE	| ios::binary,
	ios::out | ios::trunc,
	ios::out | ios::trunc					| ios::binary,
	ios::out | ios::app,
	ios::out | ios::app						| ios::binary,
	ios::in  | ios::out,
	ios::in  | ios::out						| ios::binary
 } ;



//=================================================================================
// Genera un mensaje de error de manejo de ficheros
//
// @param ai_iError			Error returned by function
// @param ao_ppError		Error string corresponding to function error
// @param ai_WarningCode	Error code to be considered as warning
// @return int				0 on success; -1 on error; 1 on warning
//=================================================================================

int	M4StoreError( int ai_iError, char** ao_ppError, int ai_WarningCode = -1 )
{

	int		iLength = -1 ;
	int		iError = -1 ;
	char*	pcError = NULL ;


	if( ao_ppError != NULL )
	{
		*ao_ppError = NULL ;
	}
	
	if( ai_iError != 0 )
	{
		iError = errno ;

		if( ao_ppError != NULL )
		{
			pcError = strerror( iError ) ;

			if( pcError != NULL )
			{
				iLength = -1 ;
				*ao_ppError = M4ANSIToCpp( pcError, iLength ) ;
			}
		}

		if( iError == ai_WarningCode )
		{
			return( 1 ) ;
		}
		return( -1 ) ;
	}

	return( 0 ) ;
}



//=================================================================================
// Escribe en un fichero el encoding si es necesario
//
// @param ai_pfFile			Fichero al que poner el encoding
// @return int				Número de bytes escritos
//=================================================================================

int	M4WriteBOM( FILE* ai_pfFile )
{

	int	iResult = 0 ;


	if( _IsUnicode() == 1 )
	{
		iResult = (int) fwrite( acUTF8BOM, 1, M4UNI_UTF8_BOM_SIZE, ai_pfFile ) ;
	}

	return( iResult ) ;
}



//=================================================================================
// Abre un fichero
// Calcula si hay que volcar el encoding
// Lee el encoding si es necesario y se lo salta
// Calcula en encoding a escribir si es necesario
// Vuelca el encoding si es necesario
//
// @param ai_pccFilename	Nombre del fichero a abrir
// @param ai_eMode			Modo de apertura del fichero
// @return FILE				Fichero abierto
// @param aio_reEncoding	Encoding de entrada salida que se debe poner
//=================================================================================

FILE*	M4Fopen( const char* ai_pccFilename, const eUniFileOpenMode_t ai_eMode, eUniFileEncodingType_t& aio_reEncoding )
{

	FILE*	pfResult = NULL ;
	int		iLength = -1 ;
	int		iWrite = 0 ;
	int		iRead = 0 ;
	int		iStatus = 0 ;
	int		iWriteBOM = 0 ;
	char*	pcMode = NULL ;
	char*	pcFileName = NULL ;
	char	acBuffer[ M4UNI_UTF8_BOM_SIZE ] = "" ;

	struct stat	stStatus ;


	/* Bug 0150957
	Hay que pasar a ANSI el path del fichero
	*/
	iLength = -1 ;
	pcFileName = M4CppToANSI( ai_pccFilename, iLength ) ;
	pcMode = aacFileOpenModes[ ai_eMode ] ;

	pfResult = fopen( pcFileName, pcMode ) ;

	if( pfResult == NULL )
	{
		aio_reEncoding = M4UniANSI ;
		delete( pcFileName ) ;
		return( NULL ) ;
	}


	if( aio_reEncoding == M4UniNative )
	{
		if( _IsUnicode() == 1 )
		{
			aio_reEncoding = M4UniUtf8 ;
		}
		else
		{
			aio_reEncoding = M4UniANSI ;
		}
	}


	iWriteBOM = 0 ;

	switch( ai_eMode )
	{
		case M4UniWrite:
		case M4UniWriteBinary:

			iWriteBOM = 1 ;
			break;

		case M4UniAppend:
		case M4UniAppendBinary:

			iStatus = stat( pcFileName, &stStatus ) ;

			if( iStatus == 0 && stStatus.st_size == 0 )
			{
				iWriteBOM = 1 ;
			}
			break;

		case M4UniRead:
		case M4UniReadBinary:
		case M4UniReadWrite:
		case M4UniReadWriteBinary:
		default:

			aio_reEncoding = M4UniANSI ;

			iRead = (int) fread( acBuffer, 1, M4UNI_UTF8_BOM_SIZE, pfResult ) ;

			if( iRead == M4UNI_UTF8_BOM_SIZE && memcmp( acBuffer, acUTF8BOM, M4UNI_UTF8_BOM_SIZE ) == 0 )
			{
				aio_reEncoding = M4UniUtf8 ;
			}
			else
			{
				fseek( pfResult, 0, SEEK_SET ) ;
			}

			break;
	}

	delete( pcFileName ) ;

	if( iWriteBOM == 1 )
	{
		if( aio_reEncoding == M4UniUtf8 )
		{
			iWrite = (int) fwrite( acUTF8BOM, 1, M4UNI_UTF8_BOM_SIZE, pfResult ) ;
		}
	}

	return( pfResult ) ;
}



//=================================================================================
// Abre un fichero de escritura
// Calcula si hay que volcar el encoding
// Calcula en encoding a escribir si es necesario
// Vuelca el encoding si es necesario
//
// @param ai_rfFile			Stream a abrir 
// @param ai_pccFilename	Nombre del fichero a abrir
// @param ai_eMode			Modo de apertura del fichero
// @param aio_reEncoding	Encoding de entrada salida que se debe poner
//=================================================================================

void	M4Open( ofstream &ai_rfFile, const char* ai_pccFilename, eUniFileOpenMode_t ai_eMode, eUniFileEncodingType_t& aio_reEncoding )
{

	int		iLength = -1 ;
	int		iMode = 0 ;
	int		iStatus = 0 ;
	int		iWriteBOM = 0 ;
	char*	pcFileName = NULL ;

	struct stat	stStatus ;


	/* Bug 0150957
	Hay que pasar a ANSI el path del fichero
	*/
	iLength = -1 ;
	pcFileName = M4CppToANSI( ai_pccFilename, iLength ) ;
	iMode = aiFileOpenModes[ ai_eMode ] ;

	ai_rfFile.open( pcFileName, iMode ) ;

	if( ai_rfFile.rdbuf()->is_open() == 0 )
	{
		aio_reEncoding = M4UniANSI ;
		delete( pcFileName ) ;
		return ;
	}


	if( aio_reEncoding == M4UniNative )
	{
		if( _IsUnicode() == 1 )
		{
			aio_reEncoding = M4UniUtf8 ;
		}
		else
		{
			aio_reEncoding = M4UniANSI ;
		}
	}


	iWriteBOM = 0 ;

	switch( ai_eMode )
	{
		case M4UniWrite:
		case M4UniWriteBinary:

			iWriteBOM = 1 ;
			break;

		case M4UniAppend:
		case M4UniAppendBinary:

			iStatus = stat( pcFileName, &stStatus ) ;

			if( iStatus == 0 && stStatus.st_size == 0 )
			{
				iWriteBOM = 1 ;
			}
			break;

		default:
			aio_reEncoding = M4UniANSI ;
			break;
	}

	delete( pcFileName ) ;

	if( iWriteBOM == 1 )
	{
		if( aio_reEncoding == M4UniUtf8 )
		{
			ai_rfFile.write( acUTF8BOM, M4UNI_UTF8_BOM_SIZE ) ;
		}
	}
}



//=================================================================================
// Se resetea el encoding de salida
// Abre un fichero de lectura
// Lee el encoding y se lo salta
//
// @param ai_rfFile			Stream a abrir 
// @param ai_pccFilename	Nombre del fichero a abrir
// @param ai_eMode			Modo de apertura del fichero
// @param ao_reEncoding		Encoding de salida que tiene el fichero
//=================================================================================

void	M4Open( ifstream &ai_rfFile, const char* ai_pccFilename, eUniFileOpenMode_t ai_eMode, eUniFileEncodingType_t& ao_reEncoding )
{

	int		iLength = -1 ;
	int		iRead = 0 ;
	int		iMode = 0 ;
	char*	pcFileName = NULL ;
	char	acBuffer[ M4UNI_UTF8_BOM_SIZE ] = "" ;

	ao_reEncoding = M4UniANSI ;

	/* Bug 0150957
	Hay que pasar a ANSI el path del fichero
	*/
	iLength = -1 ;
	pcFileName = M4CppToANSI( ai_pccFilename, iLength ) ;
	iMode = aiFileOpenModes[ ai_eMode ] ;

	ai_rfFile.open( pcFileName, iMode ) ;
	delete( pcFileName ) ;

	if( ai_rfFile.rdbuf()->is_open() == 0 )
	{
		return ;
	}

	switch( ai_eMode )
	{
		case M4UniRead:
		case M4UniReadBinary:

			iRead = (int) ai_rfFile.read( acBuffer, M4UNI_UTF8_BOM_SIZE ).gcount() ;

			if( iRead == M4UNI_UTF8_BOM_SIZE && memcmp( acBuffer, acUTF8BOM, M4UNI_UTF8_BOM_SIZE ) == 0 )
			{
				ao_reEncoding = M4UniUtf8 ;
			}
			else
			{
				ai_rfFile.seekg( 0, ios::beg ) ;

				// Bug 0315137 Si es xml se intenta encontrar el encoding
				if( iLength > 4 && strcmpi( ai_pccFilename + iLength - 4, ".xml" ) == 0 )
				{
					char	acLine[ 255 + 1 ] ;

					iRead = (int) ai_rfFile.getline( acLine, 255, '>' ).gcount() ;

					if( iRead > 0 )
					{
						for( int i = 0 ; i < iRead ; i++ )
						{
							acLine[ i ] = tolower( acLine[ i ] ) ;
						}

						if( strstr( acLine, "encoding" ) == NULL || strstr( acLine, "utf-8" ) != NULL )
						{
							ao_reEncoding = M4UniUtf8 ;
						}
					}

					ai_rfFile.seekg( 0, ios::beg ) ;
				}
			}

			break;

		default:
			break;
	}
}



//=================================================================================
// Lee un número de caracteres de un fichero
// Si es utf8 lee uno a uno y va contando caracteres
//		Si se llega al final del fichero hay que resetear el estado de lectura
// Si es ansi lee tantos bytes como caracteres
// Si se pasa del límite de caracteres se trunca
// Al final se transforman los caracteres leidos al encoding de trabajo
//
// @param ai_rfFile			Fichero del que se lee
// @param ai_iCharNumber	Número de caracteres a leer
// @param ai_eEncoding		Tipo de codificación del fichero a leer
// @param ao_riLength		Parámetro de salida con el número de caracteres leidos
//=================================================================================

char*	M4ReadFile( ifstream &ai_rfFile, size_t ai_iCharNumber, eUniFileEncodingType_t ai_eEncoding, int& ao_riLength )
{

	char*			pcResult = NULL ;
	char*			pcBuffer = NULL ;
	char			b = 0 ;
	size_t			iChars = 0 ;
	size_t			iBytes = 0 ;
	int				iRead = 0 ;
	streampos		iPosition = 0 ;


	ao_riLength = 0 ;

	if( ai_eEncoding == M4UniUtf8 )
	{
		iPosition = ai_rfFile.tellg() ;

		iChars = 0 ;
		iBytes = 0 ;

		while( 1 )
		{
			iRead = (int) ai_rfFile.get( b ).gcount() ;

			if( iRead != 1 )
			{
				break ;
			}

			if( M4UNI_IS_NOT_TRAILING( b ) )
			{
				if( iChars == ai_iCharNumber )
				{
					break ;
				}

				iChars++ ;
			}

			iBytes++ ;
		}

		// Si se llega al final hay que resetear
		if( ai_rfFile.bad() == 0 && ai_rfFile.eof() != 0 )
		{
			ai_rfFile.clear( 0 ) ;
		}

		ai_rfFile.seekg( iPosition ) ;
	}
	else
	{
		iBytes = ai_iCharNumber ;
	}

	if( iBytes >= M4UNI_MAX_CHARACTERS )
	{
		iBytes = M4UNI_MAX_CHARACTERS ;
	}

	pcBuffer = new char[ iBytes + 1 ] ;

	if( pcBuffer == NULL )
	{
		return( NULL ) ;
	}

	iRead = (int) ai_rfFile.read( pcBuffer, iBytes ).gcount() ;
	pcBuffer[ iRead ] = '\0' ;

	if( ai_eEncoding == M4UniUtf8 )
	{
		pcResult = M4Utf8ToCpp( pcBuffer, iRead ) ;
	}
	else
	{
		pcResult = M4ANSIToCpp( pcBuffer, iRead ) ;
	}

	delete( pcBuffer ) ;

	ao_riLength = (int) M4StrLen( pcResult, iRead ) ;

	return( pcResult ) ;
}



//=================================================================================
// Lee caracteres de un fichero hasta encontrar un caracter
// Primero calcula la longitud del separador. En ANSI es siempre 1
// Lee de uno en uno y va comparando caracteres con el separador
// Si se llega al final del fichero hay que resetear el estado de lectura
// Si se pasa del límite de caracteres se trunca
// Al final se cuentan los caracteres leidos
//
// @param ai_rfFile		Fichero del que se lee 
// @param ai_pccChar	Caracter que se busca
// @param ai_eEncoding	Tipo de codificación del fichero a leer
// @param ao_riLength	Parámetro de salida con el número de caracteres leidos
//=================================================================================

char*	M4ReadFile( ifstream &ai_rfFile, const char* ai_pccChar, eUniFileEncodingType_t ai_eEncoding, int& ao_riLength )
{

	char*			pcResult = NULL ;
	char*			pcBuffer = NULL ;
	char*			pcChar = NULL ;
	char			b = 0 ;
	size_t			iSize = 0 ;
	size_t			iMatches = 0 ;
	size_t			iMatchesNeeded = 0 ;
	size_t			iBytes = 0 ;
	int				iLength = 0 ;
	int				iRead = 0 ;
	streampos		iPosition = 0 ;


	ao_riLength = 0 ;

	iLength = -1 ;

	if( ai_eEncoding == M4UniUtf8 )
	{
		pcChar = M4CppToUtf8( ai_pccChar, iLength ) ;
		iMatchesNeeded = _Utf8Position( pcChar, iLength, 1, iSize ) ;
	}
	else
	{
		pcChar = M4CppToANSI( ai_pccChar, iLength ) ;
		iMatchesNeeded = sizeof( char ) ;
	}


	iPosition = ai_rfFile.tellg() ;
	iMatches = 0 ;
	iBytes = 0 ;

	while( 1 )
	{
		iRead = (int) ai_rfFile.get( b ).gcount() ;

		if( iRead != 1 )
		{
			break ;
		}

		iBytes++ ;

		if( b == (unsigned char) pcChar[ iMatches ] )
		{
			iMatches++ ;
			if( iMatches == iMatchesNeeded )
			{
				break ;
			}
		}
		else
		{
			iMatches = 0 ;
		}
	}

	delete( pcChar ) ;

	// Si se llega al final hay que resetear
	if( ai_rfFile.bad() == 0 && ai_rfFile.eof() != 0 )
	{
		ai_rfFile.clear( 0 ) ;
	}

	ai_rfFile.seekg( iPosition ) ;

	if( iBytes >= M4UNI_MAX_CHARACTERS )
	{
		iBytes = M4UNI_MAX_CHARACTERS ;
	}

	pcBuffer = new char[ iBytes + 1 ] ;

	if( pcBuffer == NULL )
	{
		return( NULL ) ;
	}

	iRead = (int) ai_rfFile.read( pcBuffer, iBytes ).gcount() ;
	pcBuffer[ iRead ] = '\0' ;

	if( ai_eEncoding == M4UniUtf8 )
	{
		pcResult = M4Utf8ToCpp( pcBuffer, iRead ) ;
	}
	else
	{
		pcResult = M4ANSIToCpp( pcBuffer, iRead ) ;
	}

	delete( pcBuffer ) ;

	ao_riLength = (int) M4StrLen( pcResult, iRead ) ;

	return( pcResult ) ;
}



//=================================================================================
// Renombra un fichero o directorio
//
// @param ai_pccOldFilename	Nombre del fichero origen
// @param ai_pccNewFilename	Nombre del fichero destino
// @param ao_ppError		Mensaje de error
// @return char*			0 si todo va bien, -1 si hay error
//=================================================================================

int	M4RenameFile( const char* ai_pccOldFilename, const char* ai_pccNewFilename, char** ao_ppError )
{

	int		iError = -1 ;
	int		iLength = -1 ;
	char*	pcOldFileName = NULL ;
	char*	pcNewFileName = NULL ;


	iLength = -1 ;
	pcOldFileName = M4CppToANSI( ai_pccOldFilename, iLength ) ;

	iLength = -1 ;
	pcNewFileName = M4CppToANSI( ai_pccNewFilename, iLength ) ;

	iError = rename( pcOldFileName, pcNewFileName ) ;

	delete( pcOldFileName ) ;
	delete( pcNewFileName ) ;

	return( M4StoreError( iError, ao_ppError ) ) ;
}



//=================================================================================
// Borra un fichero
// Si no se puede borrar devuelve la cadena de error si se pide
//
// @param ai_pccFilename	Nombre del fichero a borrar
// @param ao_ppError		Mensaje de error
// @return char*			0 si todo va bien, -1 si hay error
//=================================================================================

int	M4RemoveFile( const char* ai_pccFilename, char** ao_ppError )
{

	int		iError = 0 ;
	int		iLength = -1 ;
	char*	pcFileName = NULL ;


	pcFileName = M4CppToANSI( ai_pccFilename, iLength ) ;
	iError = remove( pcFileName ) ;
	delete( pcFileName ) ;

	return( M4StoreError( iError, ao_ppError, ENOENT ) ) ;
}



//=================================================================================
// Crea un directorio
// Si no se puede crear devuelve la cadena de error si se pide
//
// @param ai_pccDirectory	Nombre del directorio a crear
// @param ao_ppError		Mensaje de error
// @return char*			0 si todo va bien, -1 si hay error
//=================================================================================

int	M4MakeDirectory( const char* ai_pccDirectory, char** ao_ppError )
{

	int		iError = 0 ;
	int		iLength = -1 ;
	int		iAccess = -1 ;
	char*	pcDirectory = NULL ;


	pcDirectory = M4CppToANSI( ai_pccDirectory, iLength ) ;

	iAccess = access( pcDirectory, 0 ) ;

	if( iAccess == -1 )
	{
#ifdef _UNIX
		iError = mkdir( pcDirectory, S_IRWXU | S_IRWXG | S_IRWXO ) ;
#else
		iError = mkdir( pcDirectory ) ;
#endif
	}

	delete( pcDirectory ) ;

	return( M4StoreError( iError, ao_ppError, EEXIST ) ) ;
}



//=================================================================================
// Borra un directorio
// Si no se puede borrar devuelve la cadena de error si se pide
//
// @param ai_pccDirectory	Nombre del directorio a borrar
// @param ao_ppError		Mensaje de error
// @return char*			0 si todo va bien, -1 si hay error
//=================================================================================

int	M4RemoveDirectory( const char* ai_pccDirectory, char** ao_ppError )
{

	int		iError = 0 ;
	int		iLength = -1 ;
	char*	pcDirectory = NULL ;


	pcDirectory = M4CppToANSI( ai_pccDirectory, iLength ) ;
	iError = rmdir( pcDirectory ) ;
	delete( pcDirectory ) ;

	return( M4StoreError( iError, ao_ppError, ENOENT ) ) ;
}



//=================================================================================
// Obtiene el estado de un fichero
//
// @param ai_pccFilename	Nombre del fichero a abrir
// @param ao_rstStatus		Estado del fichero
// @return int				0 si todo va bien, -1 si hay error
//=================================================================================

int	M4FileStatus( const char* ai_pccFilename, struct stat& ao_rstStatus, char** ao_ppError )
{

	int		iError = 0 ;
	int		iLength = -1 ;
	char*	pcFileName = NULL ;


	memset( &ao_rstStatus, 0, sizeof( struct stat ) ) ;

	iLength = -1 ;
	pcFileName = M4CppToANSI( ai_pccFilename, iLength ) ;
	iError = stat( pcFileName, &ao_rstStatus ) ;
	delete( pcFileName ) ;

	return( M4StoreError( iError, ao_ppError ) ) ;
}



//=================================================================================
// Convierte una cadena que viene de fichero a una cadena "multi byte" (stringz)
// Tiene en cuenta si el fichero es utf8 o no
// Si es utf8 lo pasa de utf8 a utf8 en unicode y ANSI en no unicode
// Si es ANSI lo pasa de ANSI a utf8 en unicode y ANSI en no unicode
// Utiliza otras funciones que hacen la conversión
//
// @param ai_pccString	Cadena del fichero que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//							y tamaño de salida de la cadena convertida
// @param ai_iIsUtf8	Flag que indica si es un fichero utf8 o no
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pccString
//=================================================================================

char*	M4FileToCpp( const char* ai_pccString, int& aio_riLength, int ai_iIsUtf8 )
{

	char*	pcResult = NULL ;

	
	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) strlen( ai_pccString ) ;
	}

	if( ai_iIsUtf8 )
	{
		pcResult = M4Utf8ToCpp( ai_pccString, aio_riLength ) ;
	}
	else
	{
		pcResult = M4ANSIToCpp( ai_pccString, aio_riLength ) ;
	}

	return( pcResult ) ;
}




