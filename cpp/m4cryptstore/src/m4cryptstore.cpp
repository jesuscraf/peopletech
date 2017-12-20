// $Id: m4cryptstore.cpp,v 1.1 1998/7/02 pablop $


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "m4types.hpp"
#include <m4cryptstore.hpp>
#include <m4cryptc.hpp>

#define CIFRAS	4		// Para fijar el numero de cifras que se va a reservar
				// a los contadores de bytes en el fichero cifrado.

#define TAM_FILE 500000		// Tamaño máximo de fichero.



// Clase para parametrizar el algoritmo de criptografía utilizado en cada caso.
/*
//las siguientes funciones de encriptación usaban la librería
//M4crypt que debía de ser sustituida por la librería m4ciptc


ClCipherSuite::ClCipherSuite( SymCrypto s , AsymCrypto a , Digest d ) 
{
	sym = s;
	asym = a;
	dig = d;	
}




m4return_t SymEncrypt ( SymCrypto algorithm, ClPing p,
			m4pchar_t src, m4int_t lsrc,
			m4pchar_t dst, m4int_t &ldst ) 
{
	ClKey *key = new ClKey ( (m4uchar_t *)p.getPingName(), p.getlPing() );

	ClSymCryptography * c ;
	switch ( algorithm ) 
	{
	case _NULLS:
		c = new ClNullSymCryptography ( key );
		break;

	case _DES:
		c = new ClDESCryptography ( key );
		break;

	case _3DES:
		c = new Cl3DESCryptography ( key );
		break;

	case _RC4:
		c = new ClRC4Cryptography ( key );
		break;

	case _IDEA:
		c = new ClIDEACryptography ( key );
		break;

	default:  printf ( "Cannot Encrypt source : improper algorithm.\n" );
				return M4_ERROR;
	};

	ClCryptContext* ctx = c -> EncryptStart();
	c -> Encrypt ( ctx, src, lsrc, dst, &ldst );
	lsrc = ldst;
	c -> EncryptFinish ( ctx, &dst[lsrc], &ldst );
	ldst += lsrc;

	delete key;
	delete c;

	return M4_SUCCESS;
}


m4return_t SymDecrypt ( SymCrypto algorithm, ClPing p,
			m4pchar_t src, m4int_t lsrc,
			m4pchar_t dst, m4int_t &ldst ) 
{
	ClKey *key = new ClKey ( (m4uchar_t *)p.getPingName(), p.getlPing() );
	
	ClSymCryptography * c ;
	switch ( algorithm ) 
	{
	case _NULLS:
		c = new ClNullSymCryptography ( key );
		break;

	case _DES:
		c = new ClDESCryptography ( key );
		break;

	case _3DES:
		c = new Cl3DESCryptography ( key );
		break;

	case _RC4:
		c = new ClRC4Cryptography ( key );
		break;

	case _IDEA:
		c = new ClIDEACryptography ( key );
		break;

	default:  printf ( "Cannot Decrypt source : improper algorithm.\n" );
		return M4_ERROR;
	};
	
	ClCryptContext* ctx = c -> DecryptStart();
	c -> Decrypt ( ctx, src, lsrc, dst, &ldst );
	lsrc = ldst;
	c -> DecryptFinish ( ctx, &dst[lsrc], &ldst );
	ldst += lsrc;

	delete key;
	delete c;

	return M4_SUCCESS;
}



m4return_t Digest ( Digest algorithm, m4pchar_t src, m4int_t lsrc,
		    m4pchar_t dst, m4int_t &ldst ) 
{
	
	ClDigest* c ;
	switch ( algorithm ) 
	{
	case _NULLD:
		c = new ClNullDigest ();
		break;

	case _MD5:
		c = new ClMD5Digest ();
		break;

	case _SHA1:
		c = new ClSHA1Digest ();
		break;

	default:  printf ( "Cannot Digest source : improper algorithm.\n" );
		return M4_ERROR;
	};

			
	ClMDContext* ctx = c -> DigestStart();
	c -> Digest ( ctx, src, lsrc );
	c -> DigestFinish ( ctx, dst, &ldst );

	delete c;

	return M4_SUCCESS;
}

*/


m4return_t ClCryptStore::CreateStore ( )
{

	if ( M4_ERROR == m_storage->Open( m_szStoreName , W ) )
		return M4_ERROR ;

	m4int_t		n_var = 0, n_cif = 0, n_sum = 0, n_csum = 0;
	m4char_t	var[10];
	m4pchar_t	pcCif = NULL;
	m4char_t	csum[2048];
	
	
	sprintf ( var , "%s", "HEADER" );
	n_var = (m4int_t) strlen ("HEADER");
	if (M4_ERROR == EncryptTwoWay(var, n_var, m_okey.Key(), m_okey.Keylen(), pcCif, n_cif))
	{
		return M4_ERROR;
	}
	
/*

	// Utilizo esta variable para controlar los posibles errores en
	// llamadas a funciones de storage. Como sólo devuelvo M4_SUCCESS (0)
	// o M4_ERROR (-1), puedo hacer el control mediante una suma.
	m4return_t ret ;

    ret += Digest ( ciphers.getDig(), var, n_var, sum,  n_sum );
 				
	ret += SymEncrypt ( ciphers.getSym(), p, sum, n_sum, csum, n_csum );
*/
	
	
	m4int_t char_escritos = 0;

	m4char_t num_str[10];


	// Primero va el contador con el número de trios (variable, valor,
	// checksum) que hay en el fichero. En este caso 1: la cabecera.

	// Supongo que cada cifra en num_str ocupa un byte, y cada número aunque
	// sea pequeño ocupa CIFRAS bytes.

	sprintf ( num_str , "%*d", CIFRAS, 1 );	
	if ( M4_ERROR == m_storage->Write( num_str, CIFRAS, char_escritos) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	sprintf ( num_str , "%*d", CIFRAS, n_var );	
	if ( M4_ERROR == m_storage->Write( num_str, CIFRAS, char_escritos) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	sprintf ( num_str , "%*d", CIFRAS, n_cif );	
	if ( M4_ERROR == m_storage->Write( num_str, CIFRAS, char_escritos) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	sprintf ( num_str , "%*d", CIFRAS, n_csum );	
	if ( M4_ERROR == m_storage->Write( num_str, CIFRAS, char_escritos) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	if ( M4_ERROR == m_storage->Write( var, n_var, char_escritos) )
	{
		delete pcCif;
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_storage->Write( pcCif, n_cif, char_escritos) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	delete pcCif;

	if ( M4_ERROR == m_storage->Write( csum, n_csum, char_escritos) )
		return M4_ERROR ;

	//Inserto un retorno de carro al final de cada item.
	//sprintf ( num_str , "\r\n" );
	//ret += storage->Write( num_str, strlen( num_str ), char_escritos);
	
	num_str[0] = '\r';
	if ( M4_ERROR == m_storage->Write( num_str, 1, char_escritos) )
		return M4_ERROR ;
	
	num_str[0] = '\n';
	if ( M4_ERROR == m_storage->Write( num_str, 1, char_escritos) )
		return M4_ERROR ;
	
	if ( M4_ERROR == m_storage->Close() )
		return M4_ERROR ;

	// Para guardar un valor por defecto del fichero en que trabajamos.

	return M4_SUCCESS;

}


m4return_t ClCryptStore::CheckHeader (m4bool_t &estado)

{
	// Utilizo esta variable para controlar los posibles errores en
	// llamadas a funciones de storage. Como sólo devuelvo M4_SUCCESS (0)
	// o M4_ERROR (-1), puedo hacer el control mediante una suma.
	m4return_t	ret = M4_SUCCESS;

	m4int_t		n_var = 0, n_plano = 0, n_cif = 0, n_sum = 0,
				n_sum2 = 0, n_csum = 0, n_csum2 = 0, char_leidos = 0;
	m4pchar_t	pcPlano = NULL;
	m4char_t	num_str[10];
	m4char_t	var[1024];
	m4char_t	cif[1024];
	m4char_t	csum[1024];
	m4char_t	csum2[1024];


	estado = M4_TRUE;

	if ( M4_ERROR == m_storage->Open( m_szStoreName, R ) )
		return M4_ERROR ;

	if ( M4_ERROR == m_storage->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;
	// La primera lectura es el nº de items y no me interesa ahora.

	if ( M4_ERROR == m_storage->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	n_var = atoi ( num_str );	// FUNCIONARA?????????
			
	if ( M4_ERROR == m_storage->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	n_cif = atoi ( num_str );	

	if ( M4_ERROR == m_storage->Read( CIFRAS, num_str, char_leidos ))
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	n_csum = atoi ( num_str );	


	if ( M4_ERROR == m_storage->Read( n_var, var, char_leidos) )
		return M4_ERROR ;
	if ( M4_ERROR == m_storage->Read( n_cif, cif, char_leidos) )
		return M4_ERROR ;
	if ( M4_ERROR == m_storage->Read( n_csum, csum, char_leidos) )
		return M4_ERROR ;

	if (M4_ERROR == DecryptTwoWay(cif, n_cif, m_okey.Key(), m_okey.Keylen(), pcPlano, n_plano))
	{
		return M4_ERROR;
	}

	delete pcPlano;

	/*
	ret += Digest ( ciphers.getDig(), plano, n_plano, sum2,  n_sum2);
	
	ret += SymEncrypt ( ciphers.getSym(), p, sum2, n_sum2, csum2,  n_csum2);
 	//ret += SymDecrypt ( ciphers.getSym(), p, csum, n_csum, sum, (m4int_t &) n_sum );
	*/
	if ( ( memcmp( csum, csum2, n_csum ) != 0 ) || ( n_csum != n_csum2 ) ) 
		estado = M4_FALSE;

	//if ( ( memcmp( sum, sum2, n_sum ) != 0 ) || ( n_sum != n_sum2 ) ) 
	//	estado = M4_FALSE;

	if ( M4_ERROR == m_storage->Close() )
		return M4_ERROR ;

	// Para guardar un valor por defecto del fichero en que trabajamos.

	return M4_SUCCESS ;
}


m4return_t ClCryptStore::SeekData ( m4int_t ai_offset, ClStorage *ai_store,
				m4pchar_t ao_var, m4int_t &ao_lvar,
				m4pchar_t ao_valor, m4int_t &ao_lvalor,
				m4pchar_t ao_csum, m4int_t &ao_lcsum )
{

	// Utilizo esta variable para controlar los posibles errores en
	// llamadas a funciones de storage. Como sólo devuelvo M4_SUCCESS (0)
	// o M4_ERROR (-1), puedo hacer el control mediante una suma.
	m4return_t ret = M4_SUCCESS;

	m4int_t char_leidos = 0;
	m4char_t num_str[10];


	if ( M4_ERROR == ai_store->Seek( ai_offset, _SEEK_SET ) )
		return M4_ERROR ;

	if ( M4_ERROR == ai_store->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	ao_lvar = atoi ( num_str );		//FUNCIONARA ??

	if ( M4_ERROR == ai_store->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	ao_lvalor = atoi ( num_str );		//FUNCIONARA ??

	if ( M4_ERROR == ai_store->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	ao_lcsum = atoi ( num_str );		//FUNCIONARA ??

	if ( M4_ERROR == ai_store->Read( ao_lvar, ao_var, char_leidos ) )
		return M4_ERROR ;

	if ( M4_ERROR == ai_store->Read( ao_lvalor, ao_valor, char_leidos ) )
		return M4_ERROR ;

	if ( M4_ERROR == ai_store->Read( ao_lcsum, ao_csum, char_leidos ) )
		return M4_ERROR ;

	return M4_SUCCESS;
}


m4return_t ClCryptStore::InsertData ( m4pchar_t ai_var, m4int_t ai_lvar,
					m4pchar_t ai_valor, m4int_t ai_lvalor)
{
	// Utilizo esta variable para controlar los posibles errores en
	// llamadas a funciones de storage. Como sólo devuelvo M4_SUCCESS (0)
	// o M4_ERROR (-1), puedo hacer el control mediante una suma.
	m4return_t	ret = M4_SUCCESS;
	m4int_t		n_items = 0, n_var = 0, n_cif = 0, n_sum = 0,
				n_csum = 0, char_leidos = 0, char_escritos = 0;
	m4int_t		off;
	m4char_t	num_str[10];
	m4char_t	var[1024];
	m4char_t	cif[CRYPT_PROP_VALLUE_MAX_LEN];
	m4pchar_t	pcCif = NULL;
	m4char_t	csum[1024];
	m4bool_t	estado = M4_FALSE;

	// Compruebo que el ping es bueno descifrando la cabecera.
	if ( M4_ERROR == CheckHeader( estado) )
		return M4_ERROR ;

	if ( estado == M4_FALSE ) 
		return M4_ERROR ;

	//Leo el numero de entradas en el fichero.
	if ( M4_ERROR == m_storage->Open( m_szStoreName, R_W ) )
		return M4_ERROR ;

	if ( M4_ERROR == m_storage->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	n_items = atoi ( num_str );		//FUNCIONARA ??


	// Ahora busco en el fichero una entrada igual a la que quiero
	// insertar. No está permitido.

	off = CIFRAS;		// Para saltar el contador de entradas.
	
	m4uint16_t i ;

	for ( i = 0; i < n_items ; ++i )
	{
		SeekData ( off, m_storage, var, n_var, cif, n_cif, csum, n_csum );
		if ( memcmp( var, ai_var, ai_lvar ) == 0 ) 
		{
			ret += m_storage->Close();	

			// Para guardar un valor por defecto del fichero 
			// en que trabajamos.

			return M4_ERROR;
		}
		off = off + 3*CIFRAS + n_var + n_cif + n_csum + 2;	

	// Teniendo en cuenta que la búsqueda la hago siempre desde el ppio
	// del fichero.
	// Añado 1 porque despues de cada entrada añado un retorno de carro, para que
	// el fichero sea editable.
	}

	if ( M4_ERROR == EncryptTwoWay(ai_valor, ai_lvalor, m_okey.Key(), m_okey.Keylen(), pcCif, n_cif))
	{
		return M4_ERROR;
	}
	
	/*
	ret += Digest ( ciphers.getDig(), ai_valor, ai_lvalor, sum, n_sum );
 	ret += SymEncrypt ( ciphers.getSym(), p, sum, n_sum, csum, n_csum );
	*/
	if ( M4_ERROR == m_storage->Seek( 0, _SEEK_END ) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	sprintf ( num_str , "%*d", CIFRAS, ai_lvar );	
	if ( M4_ERROR == m_storage->Write( num_str, CIFRAS, char_escritos ) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	sprintf ( num_str , "%*d", CIFRAS, n_cif );	
	if ( M4_ERROR == m_storage->Write( num_str, CIFRAS, char_escritos ) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	sprintf ( num_str , "%*d", CIFRAS, n_csum );	
	if ( M4_ERROR == m_storage->Write( num_str, CIFRAS, char_escritos ) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	if ( M4_ERROR == m_storage->Write( ai_var, ai_lvar, char_escritos ) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	if ( M4_ERROR == m_storage->Write( pcCif, n_cif, char_escritos ) )
	{
		delete pcCif;
		return M4_ERROR;
	}

	delete pcCif;

	if ( M4_ERROR == m_storage->Write( csum, n_csum, char_escritos ) )
		return M4_ERROR ;

	//Inserto un retorno de carro al final de cada item.
	//sprintf ( num_str , "%s", "\n " );
	//ret += storage->Write( num_str, strlen( num_str ), char_escritos);

	num_str[0] = '\r';
	if ( M4_ERROR == m_storage->Write( num_str, 1, char_escritos) )
		return M4_ERROR ;
	
	num_str[0] = '\n';
	if ( M4_ERROR == m_storage->Write( num_str, 1, char_escritos) )
		return M4_ERROR ;


	// Actualizo el contador de entradas.
	n_items += 1;
	sprintf ( num_str , "%*d", CIFRAS, n_items );	

	// Me posiciono al principio del fichero.
	if ( M4_ERROR == m_storage->Seek( 0, _SEEK_SET ) )
		return M4_ERROR ;
	if ( M4_ERROR == m_storage->Write( num_str, CIFRAS, char_escritos ) )
		return M4_ERROR ;

	if ( M4_ERROR == m_storage->Close() )
		return M4_ERROR ;

	// Para guardar un valor por defecto del fichero en que trabajamos.

	return M4_SUCCESS;
}


m4return_t ClCryptStore::ExtractData (m4pchar_t ai_var, m4int_t ai_lvar,
					m4pchar_t ao_valor, m4uint32_t ai_iBufferValue , m4int_t &ao_lvalor)
{
	// Utilizo esta variable para controlar los posibles errores en
	// llamadas a funciones de storage. Como sólo devuelvo M4_SUCCESS (0)
	// o M4_ERROR (-1), puedo hacer el control mediante una suma.
	m4return_t	ret = M4_SUCCESS;

	m4int_t		n_items = 0, n_var = 0, n_cif = 0, n_sum = 0,
				n_sum2 = 0, n_csum = 0, n_csum2 = 0, char_leidos = 0;
	m4int_t		off;
	m4pchar_t	pcValor = NULL;
	m4char_t	num_str[10];
	m4char_t	var[1024];
	m4char_t	cif[CRYPT_PROP_VALLUE_MAX_LEN];
	m4char_t	csum[1024];
	m4char_t	csum2[1024];
	m4bool_t	estado = M4_FALSE;

	// Inicializaciones.
	*ao_valor = '\0';
	ao_lvalor = 0;

	// Compruebo que el ping es bueno descifrando la cabecera.
	if ( M4_ERROR == CheckHeader( estado) )
		return M4_ERROR ;

	if ( estado == M4_FALSE ) return M4_ERROR;

	//Leo el numero de entradas en el fichero.
	if ( M4_ERROR == m_storage->Open( m_szStoreName, R ) )
		return M4_ERROR ;

	if ( M4_ERROR == m_storage->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	n_items = atoi ( num_str );		//FUNCIONARA ??


	// Ahora busco en el fichero una entrada igual a la que quiero
	// extraer.

	off = CIFRAS;		// Para saltar el contador de entradas.
	
	for ( m4uint16_t i = 0; i < n_items; ++i )
	{
		SeekData ( off, m_storage, var, n_var, cif, n_cif, csum, n_csum );
		if ( memcmp( var, ai_var, n_var ) == 0 && ( ai_lvar == n_var ) ) 
		{
			if (M4_ERROR == DecryptTwoWay(cif, n_cif, m_okey.Key(), m_okey.Keylen(), pcValor, ao_lvalor))
			{
				return M4_ERROR;
			}

			// Guardar el valor desencriptado en el array de salida, comprobando si hay espacio suficiente.
			if (ai_iBufferValue < ao_lvalor + 1)
			{
				delete pcValor;
				return M4_ERROR;
			}
			
			// Copiar el buffer desencriptado con el nulo final.
			memcpy(ao_valor, pcValor, ao_lvalor  + 1);
			delete pcValor;
			
/*
			// Hago una comprobacion adicional.
			ret += Digest ( ciphers.getDig(), ao_valor, ao_lvalor, 
					sum2,  n_sum2);

			ret += SymEncrypt ( ciphers.getSym(), p, sum2, n_sum2, csum2, n_csum2);
*/
			//ret += SymDecrypt ( ciphers.getSym(), p, csum, n_csum, 
			//		sum, (m4int_t &) n_sum );
	
			if ( ( memcmp( csum, csum2, n_csum ) != 0 ) || 
				 ( n_csum != n_csum2 ) ) 
				estado = M4_FALSE;

			
			//if ( ( memcmp( sum, sum2, n_sum ) != 0 ) || ( n_sum != n_sum2 ))
			//	return M4_ERROR;

			if ( M4_ERROR == m_storage->Close() )
				return M4_ERROR ;

			// Para guardar un valor por defecto del fichero en que trabajamos.

			return M4_SUCCESS;
		}

		off = off + 3*CIFRAS + n_var + n_cif + n_csum + 2;	

	// Teniendo en cuenta que la búsqueda la hago siempre desde el ppio
	// del fichero.
	// Añado 1 porque despues de cada entrada añado un retorno de carro, 
	// para que el fichero sea editable.
	}


	// Si llegamos aqui, entonces no hemos encontrado la entrada.
	// Salimos devolviendo error.

	m_storage->Close();	

	// Para guardar un valor por defecto del fichero en que trabajamos.

	return M4_ERROR;
}


m4return_t ClCryptStore::ChangePing ( ClKey key, ClKey newkey)
{
	// Utilizo esta variable para controlar los posibles errores en
	// llamadas a funciones de storage. Como sólo devuelvo M4_SUCCESS (0)
	// o M4_ERROR (-1), puedo hacer el control mediante una suma.
	m4return_t	ret = M4_SUCCESS;

	m4int_t		n_items = 0, n_var = 0, n_cif = 0, n_sum = 0,
				n_csum = 0, n_plano = 0, char_leidos = 0, char_escritos = 0;
	m4int_t		off;
	m4char_t	num_str[10];
	m4char_t	var[1024];
	m4char_t	cif[CRYPT_PROP_VALLUE_MAX_LEN];
	m4pchar_t	pcCif = NULL;
	m4char_t	csum[1024];
	m4pchar_t	pcPlano = NULL;
	m4bool_t	estado = M4_FALSE;
	m4char_t	buff_init[TAM_FILE];
	m4pchar_t	buff_curr = buff_init;

	// Compruebo que el ping es bueno descifrando la cabecera.
	if ( M4_ERROR == CheckHeader( estado ) )
		return M4_ERROR ;

	if ( estado == M4_FALSE ) return M4_ERROR;

	//Leo el numero de entradas en el fichero.
	if ( M4_ERROR == m_storage->Open( m_szStoreName, R_W ) )
		return M4_ERROR ;

	if ( M4_ERROR == m_storage->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	n_items = atoi ( num_str );		//FUNCIONARA ??


	// Empiezo a llenar el buffer.
	sprintf ( buff_curr , "%*d", CIFRAS, n_items );	
	buff_curr += CIFRAS;

	// Ahora tomo los datos, los codifico con el nuevo ping y los guardo
	// en un buffer para escribirlos al final.

	off = CIFRAS;		// Para saltar el contador de entradas.
	m4uint16_t i ;
	for ( i = 0; i < n_items; ++i )
	{
		SeekData ( off, m_storage, var, n_var, cif, n_cif, csum, n_csum );

		if (M4_ERROR == DecryptTwoWay(cif, n_cif, m_okey.Key(), m_okey.Keylen(), pcPlano, n_plano))
		{
			return M4_ERROR;
		}

		if (M4_ERROR == EncryptTwoWay(pcPlano, n_plano, m_okey.Key(), m_okey.Keylen(), pcCif, n_cif)) 
		{
			return M4_ERROR;
		}

		delete pcPlano;
			
	/*
		ret += Digest ( ciphers.getDig(), plano, n_plano, 
					sum,  n_sum);		
		ret += SymEncrypt ( ciphers.getSym(), newping, sum, n_sum,
					csum, n_csum );
	*/
		sprintf ( buff_curr , "%*d", CIFRAS, n_var ); 
		buff_curr += CIFRAS;	
		sprintf ( buff_curr , "%*d", CIFRAS, n_cif ); 
		buff_curr += CIFRAS;		
		sprintf ( buff_curr , "%*d", CIFRAS, n_csum ); 
		buff_curr += CIFRAS;		
		memcpy ( buff_curr , var, n_var );	
		buff_curr += n_var;
		memcpy ( buff_curr , pcCif, n_cif );	
		buff_curr += n_cif;
		memcpy ( buff_curr , csum, n_csum );	
		buff_curr += n_csum;

		delete pcCif;

		//Inserto un retorno de carro al final de cada item.
		num_str[0] = '\r';
		if ( M4_ERROR == m_storage->Write( num_str, 1, char_escritos) )
			return M4_ERROR ;

		memcpy ( buff_curr, num_str, 1 );	
		buff_curr += 1;

		num_str[0] = '\n';
		if ( M4_ERROR == m_storage->Write( num_str, 1, char_escritos) )
			return M4_ERROR ;

		memcpy ( buff_curr, num_str, 1 );	
		buff_curr += 1;
		
		off = off + 3*CIFRAS + n_var + n_cif + n_csum + 2;	

	// Teniendo en cuenta que la búsqueda la hago siempre desde el ppio
	// del fichero.
	// Añado 1 porque despues de cada entrada añado un retorno de carro, para que
	// el fichero sea editable.
	}

	// Me posiciono al principio del fichero.
	if ( M4_ERROR == m_storage->Seek( 0, _SEEK_SET ) )
		return M4_ERROR ;
	
	if ( M4_ERROR == m_storage->Write( buff_init, off, char_escritos ) )
		return M4_ERROR ;

	if ( M4_ERROR == m_storage->Close() )
		return M4_ERROR ;

	// Para guardar un valor por defecto del fichero en que trabajamos.

	return M4_SUCCESS;
}


m4return_t ClCryptStore::ConsistencyControl ( m4bool_t &estado)
{
	// Utilizo esta variable para controlar los posibles errores en
	// llamadas a funciones de storage. Como sólo devuelvo M4_SUCCESS (0)
	// o M4_ERROR (-1), puedo hacer el control mediante una suma.
	m4return_t	ret = M4_SUCCESS;

	m4int_t		n_items = 0, n_var = 0, n_cif = 0, n_sum = 0,
				n_sum2 = 0, n_csum = 0, n_csum2 = 0, n_plano = 0, char_leidos = 0;
	m4int_t		off;
	m4char_t	num_str[10];
	m4char_t	var[1024];
	m4char_t	cif[CRYPT_PROP_VALLUE_MAX_LEN];
	m4char_t	csum[1024];
	m4char_t	csum2[1024];
	m4pchar_t	pcPlano = NULL;

	estado = M4_TRUE;

	//Leo el numero de entradas en el fichero.
	if ( M4_ERROR == m_storage->Open( m_szStoreName, R ) )
		return M4_ERROR ;

	if ( M4_ERROR == m_storage->Read( CIFRAS, num_str, char_leidos ) )
		return M4_ERROR ;

	num_str[char_leidos] = '\0';
	n_items = atoi ( num_str );		//FUNCIONARA ??


	// Ahora compruebo en cada entrada el checksum.

	off = CIFRAS;		// Para saltar el contador de entradas.
	
	for ( m4uint16_t i = 0; i < n_items; ++i )
	{
		SeekData ( off, m_storage, var, n_var, cif, n_cif, csum, n_csum );

		if (M4_ERROR == DecryptTwoWay(cif, n_cif, m_okey.Key(), m_okey.Keylen(), pcPlano, n_plano))
		{
			return M4_ERROR;
		}

		delete pcPlano;
		
/*
		ret += Digest ( ciphers.getDig(), plano, n_plano, 
					sum2, n_sum2);

		ret += SymEncrypt ( ciphers.getSym(), p, sum2, n_sum2, csum2, n_csum2);
		*/

		if ( ( memcmp( csum, csum2, n_csum ) != 0 ) || ( n_csum != n_csum2 ) ) 
		{
			estado = M4_FALSE;
			break;
		}


	 	//ret += SymDecrypt ( ciphers.getSym(), p, csum, n_csum, 
		//			sum, (m4int_t &) n_sum );
	
		//if ( ( memcmp( sum, sum2, n_sum ) != 0 ) || ( n_sum != n_sum2 ) )
		//{
		//	estado = M4_FALSE;
		//	break;
		//}

		off = off + 3*CIFRAS + n_var + n_cif + n_csum + 2;	

	// Teniendo en cuenta que la búsqueda la hago siempre desde el ppio
	// del fichero.
	// Añado 1 porque despues de cada entrada añado un retorno de carro, para que
	// el fichero sea editable.
	}

	if ( M4_ERROR == m_storage->Close() )
		return M4_ERROR ;

	// Para guardar un valor por defecto del fichero en que trabajamos.

	return M4_SUCCESS;
}




