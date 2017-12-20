// $Id: ClStorage.cpp,v 1.1 1998/7/13 pablop $


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "m4types.hpp"
#include "m4unicode.hpp"
#include <clstorage.hpp>


// Recubrimiento de clases para acceso a datos: ficheros, base de datos, ...

m4return_t ClFileStorageCrypt::Open( m4pchar_t ai_szName, Mode ai_mode )
{
	FILE *pFile;
	m4char_t mode[8];

	switch ( ai_mode ) {
		case R: sprintf( mode, "%s", "rb" ); break;
		case W: sprintf( mode, "%s", "wb" ); break;
		case R_W: sprintf( mode, "%s", "r+b" ); break;
		case R_A: sprintf( mode, "%s", "a+b" ); break;
		default: 
			//printf ( "Cannot open %s : improper mode.\n" , 
			//	ai_szName );
			return M4_ERROR;
	}
	
	// CUIDADO CON EL NULL siguiente que puede ser que falle.
	// UNICODE FILE
	int iLength = -1 ;
	char* pcFileName = M4CppToANSI( ai_szName, iLength ) ;
	pFile = fopen ( pcFileName, mode ) ;
	delete [] pcFileName ;

	if ( pFile == NULL )
	{
	//printf ( "Cannot open %s \n." , ai_szName );
	return M4_ERROR;
	}


	// Si tenía un fichero abierto antes lo cierro ahora.
	if ( szStoreDesc != NULL ) fclose ( ( FILE *) szStoreDesc );
	
	sprintf ( szStoreName, "%s", ai_szName );
	szStoreDesc = pFile;
		
	return M4_SUCCESS;
}	


m4return_t ClFileStorageCrypt::Close() 
{
	if ( szStoreDesc == NULL ) 
	{ // file not opened 
		//printf ( "Cannot close %s: file not opened.\n" ,
		//szStoreName );
		return M4_ERROR;
	}
	
	if ( fclose ( ( FILE*)szStoreDesc ) != 0 ) 
	{
		//printf ( "Cannot close %s \n." , szStoreName );
		return M4_ERROR;
	};
	
	sprintf ( szStoreName, "%s", "" );
	szStoreDesc = NULL;

	return M4_SUCCESS;
}


m4return_t ClFileStorageCrypt::Read( m4int_t ai_sizeBuf, 
								m4pchar_t ao_pBuf, 
								m4int_t &ao_sizeBuf )
{
	if ( szStoreDesc == NULL ) 
	{ // file not opened 
		//printf ( "Cannot read from file: file not opened.\n" );
		return M4_ERROR;
	}

	// El 1 es el tamaño de los items a leer en bytes.
	ao_sizeBuf = (m4int_t) fread ( ao_pBuf, 1, ai_sizeBuf,
		( FILE *) szStoreDesc );
	
	return M4_SUCCESS;
}


m4return_t ClFileStorageCrypt::Write( m4pchar_t ai_pBuf , 
								 m4int_t ai_sizeBuf, 
								 m4int_t &ao_sizeBuf )
{
	if ( szStoreDesc == NULL ) 
	{ // file not opened 
		//printf ( "Cannot write in file: file not opened.\n" );
		return M4_ERROR;
	}
	
	// El 1 es el tamaño de los items a leer en bytes.
	ao_sizeBuf = (m4int_t) fwrite ( ai_pBuf, 1, ai_sizeBuf,
		( FILE *) szStoreDesc );
	
	return M4_SUCCESS;
}


m4return_t ClFileStorageCrypt::Seek( m4int_t ai_offset,
							   origin_seek ai_origin )
{
	m4int16_t origen;
	
	if ( szStoreDesc == NULL ) 
	{ // file not opened 
		//printf ( "Cannot write in file: file not opened.\n" );
		return M4_ERROR;
	}
	
		switch ( ai_origin )
	{
		case _SEEK_CUR: origen = SEEK_CUR; break;
		case _SEEK_END: origen = SEEK_END; break;
		case _SEEK_SET: origen = SEEK_SET; break;
		default: 
				//printf ( "Bad origin for seek into %s \n." , 
				//szStoreName );
				return M4_ERROR;
	};

	if ( fseek ( ( FILE *) szStoreDesc, ai_offset, origen ) != 0 ) 
	{
		//printf ( "Cannot seek into %s \n." , szStoreName );
		return M4_ERROR;
	};
	
	return M4_SUCCESS;
}




