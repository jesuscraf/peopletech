// Prueba para comprobar el funcionamiento de la DLL para el 
// control de fichero de passwords del servidor : M4CryptStore.


// Para invocar el programa ver la función help() al final del 
// listado.

#include <m4types.hpp>
#include <stdio.h>
#include <string.h>
#include <m4cryptstore.hpp>
#include <m4win.hpp>


int main ( int argc , char ** argv )
{
	m4uint32_t hostid = 0;
	m4return_t ret = M4_SUCCESS;
	m4bool_t estado = M4_TRUE;
	m4char_t szPing [1024];
	
	m4char_t szUser [1024];
	m4char_t szPassword [1024];
	m4char_t szFileName [1024];

	
	// comprobar argumentos de entrada.
	if (argc != 4)
	{
		printf ("Usage: \n");
		printf ("   %s user password output-encrypted-file\n", argv[0]);
		return -1;
	}

	strcpy(szUser, argv[1]);
	strcpy(szPassword, argv[2]);
	strcpy(szFileName, argv[3]);


#ifdef _WINDOWS

	/* Bug 0115958
	Para no depender de la tsap se hace directamente la inicialización de los sockets
	*/
	WSADATA wsaData;
	ret = WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) ;

	if ( ret != 0 )
	{
		printf ("Error in Socket initialization.\n");
		return -1;
	}

	gethostname ( szPing , 1024 );
#else
	hostid = gethostid ( ) ;
	sprintf ( szPing , "%u" , hostid ) ;
#endif


	ClCryptStore c(szFileName,szPing,(m4uint32_t)strlen(szPing));

	if ( ret != M4_SUCCESS )
	{
		printf ("Error in PING initialization.\n");
		return -1;
	}


	// create output file
	ret += c.CreateStore( ); 
	if ( ret != M4_SUCCESS )
	{
		printf ("Error creating output file.\n");
		return -1;
	}
	
	
	// insert user
	ret += c.InsertData( "USER", 4 , szUser, (m4int_t) strlen(szUser));
	if ( ret != M4_SUCCESS )
	{
		printf ("Error inserting user data.\n");
		return -1;
	}

	
	// insert password
	ret += c.InsertData( "PASSWORD", 8,	szPassword, (m4int_t) strlen(szPassword));
	if ( ret != M4_SUCCESS ) 
	{
		printf ("Error inserting password data.\n");
		return -1;
	}


	ret = c.ConsistencyControl ( estado);

	return ret;
}
