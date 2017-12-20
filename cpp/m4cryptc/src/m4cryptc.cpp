//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4criptc.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:15/01/1999
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================



#include <m4win.hpp>
#include <m4cryptc.hpp>
#include <stdio.h>

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/des.h>

#define _m4crypt_interno
#include "m4cryptcobj.hpp"

#define MAX_KEY_LEN		48

m4return_t ASCIItoHEX ( m4pcchar_t ai_szASCII , m4int_t ai_iLen , m4pchar_t ao_szHEX , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) ;
m4return_t HEXtoASCII ( m4pcchar_t ai_szHEX , m4int_t ai_iLen , m4pchar_t ao_szASCII , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) ;


m4return_t EncryptOneWayMD5 ( m4pcchar_t src , m4int_t slen , m4pchar_t dst , m4int_t lenbufferout , m4int_t &dlen )
{
	ClMD5Digest oMD5Digest ;
	int dlen2 = 0;
	m4pchar_t aux ;
	m4uint32_t iAuxLen ;
	
	
	iAuxLen= (slen +1 + 8 ) *2 ;
	
	aux = new m4char_t [ iAuxLen] ;
	
	memcpy ( aux , src , slen +1 ) ;
	
	ClMDContext* poContext = oMD5Digest.DigestStart();
	oMD5Digest.Digest(poContext, aux, slen);
	oMD5Digest.DigestFinish(poContext, aux ,  &dlen2);
	
	ASCIItoHEX ( aux , dlen2 , dst , lenbufferout , dlen ) ;
	
	delete [] aux ;
	
	return M4_SUCCESS ;
}


m4return_t EncryptOneWaySHA(m4pcchar_t ai_pSource, m4int_t ai_iLen, m4pchar_t ai_pTarget, m4int_t ai_iMaxLenOut, m4int_t &aio_iLenOut)
{
	EVP_MD_CTX		oMdCtx;
	m4uint_t		iLen = 0;
	m4char_t		pTarget[SHA512_DIGEST_LENGTH];

	EVP_MD_CTX_init(&oMdCtx);
	EVP_DigestInit_ex(&oMdCtx, EVP_sha512(), NULL);
	EVP_DigestUpdate(&oMdCtx, (unsigned char *) ai_pSource, ai_iLen);
	EVP_DigestFinal_ex(&oMdCtx, (unsigned char *) pTarget, &iLen);
	EVP_MD_CTX_cleanup(&oMdCtx);

	// Convertir a hexadecimal.
	ASCIItoHEX(pTarget, iLen, ai_pTarget, ai_iMaxLenOut, aio_iLenOut);
	
	return M4_SUCCESS;
}


// AES - Borrar
m4return_t SynCrypt ( m4pcchar_t ai_szText , m4int_t ai_iLenText , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szCrypt , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut )
{
	m4pchar_t			aux  ;
	int					dlen ;
	m4return_t			iRet = M4_SUCCESS ;


	aux = new m4char_t [ai_iLenBufferOut];

	iRet = SymCryptASCII  ( ai_szText , ai_iLenText , ai_szKey , ai_iLenKey , (m4pchar_t) aux , ai_iLenBufferOut , dlen ) ;

	if ( M4_SUCCESS == iRet )
	{
		if ( M4_ERROR == ASCIItoHEX ( (m4pchar_t) aux , dlen , ao_szCrypt , ai_iLenBufferOut , ao_iLenOut  ) )
		{
			iRet = M4_ERROR ;
		}
	}
	
	delete [] aux ;

	return iRet ;
}


// AES - Borrar
m4return_t SynDeCrypt ( m4pcchar_t ai_szCrypt , m4int_t ai_iLenCrypt , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szText , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut )
{
	m4pchar_t				aux ;
	int						auxlen;
	m4return_t				iRet ;

	auxlen = ( ai_iLenCrypt /2 )+2 ;

	aux = (m4pchar_t ) new m4uchar_t [auxlen];

	iRet = HEXtoASCII ( ai_szCrypt , ai_iLenCrypt , aux, auxlen , ai_iLenCrypt ) ;

	if ( M4_SUCCESS == iRet )
	{
		iRet = SymDeCryptASCII ( aux, ai_iLenCrypt , ai_szKey , ai_iLenKey , ao_szText, ai_iLenBufferOut , ao_iLenOut ) ;
	}

	delete [] aux ;

	return iRet ;
}


m4return_t EncryptTwoWaySecondKey(m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t & ao_pcCrypted, m4int_t & ao_iLenCryptedOut)
{
	m4return_t	iRet = M4_SUCCESS;

	// Inicializaciones.
	ao_iLenCryptedOut = 0;
	ao_pcCrypted = NULL;
	
	iRet = EncryptTwoWay(ai_szText, ai_iLenText, ai_szKey, ai_iLenKey, ao_pcCrypted, ao_iLenCryptedOut);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (ao_iLenCryptedOut > 1)
	{
		// Marcar el campo encriptado con una segunda clave
		if (*(ao_pcCrypted + 1) <= '9')
		{
			*(ao_pcCrypted + 1) -= 10;
		}
		else
		{
			*(ao_pcCrypted + 1) += 6;
		}
	}

	return M4_SUCCESS;
}


m4return_t EncryptTwoWay(m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t & ao_pcCrypted, m4int_t & ao_iLenCryptedOut)
{
	m4return_t		iRet = M4_SUCCESS;
	m4pchar_t		pcCrypted = NULL;
	m4pchar_t		pcRaw = NULL;
	m4int_t			iLenCryptedText = 0;
	m4int_t			iLenCryptedRaw = 0;

	// Inicializaciones.
	ao_iLenCryptedOut = 0;
	ao_pcCrypted = NULL;
	
	// Bug 0106119
	// Si no hay nada en la password la librería de terceros da un assert.
	if (ai_iLenText == 0) 
	{
		ao_pcCrypted = new m4char_t[1];
		*ao_pcCrypted = '\0';
		return M4_SUCCESS;
	}

	// Encriptar twoway.
	iRet = EncryptTwoWayRaw(ai_szText, ai_iLenText, ai_szKey, ai_iLenKey, pcRaw, iLenCryptedRaw);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Alocar memoria para el buffer de salida.
	// Se reserva un byte más para los casos unicode UTF16.
	iLenCryptedText = 2 * iLenCryptedRaw;
	pcCrypted = new m4char_t[iLenCryptedText + 1];
	
	// Convertir de binario a hexadecimal.
	iRet = ASCIItoHEX(pcRaw, iLenCryptedRaw, pcCrypted, iLenCryptedText, ao_iLenCryptedOut);
	if (iRet != M4_SUCCESS)
	{
		delete pcRaw;
		delete pcCrypted;
		return M4_ERROR;
	}
	
	delete pcRaw;

	// Marcar el campo encriptado para distinguirlo de los antiguos encriptados con DES.
	if (*pcCrypted <= '9')
	{
        *pcCrypted -= 10;
	}
    else
	{
		*pcCrypted += 6;
	}
    
	// Alocar memoria para el buffer de salida.
	ao_pcCrypted = new m4char_t[iLenCryptedText + 2];
	memcpy(ao_pcCrypted, pcCrypted, iLenCryptedText + 1);
	ao_pcCrypted[iLenCryptedText + 1] = '\0';

	delete pcCrypted;

	return M4_SUCCESS;
}

m4return_t _DecryptTwoWayRaw(m4pcchar_t ai_szCrypted, m4int_t ai_iLenCrypted, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4bool_t ai_bAESCrypted, m4pchar_t & ao_pcText, m4int_t & ao_iLenTextOut)
{
	m4return_t				iRet = M4_SUCCESS;
	m4int_t					iLenText;
	m4int_t					iLenTextOut;
	m4char_t				szKey[MAX_KEY_LEN];
	unsigned char			iv[EVP_MAX_IV_LENGTH];
	des_cblock				* pcDesBlock;
  	const EVP_CIPHER		* oCipher;
	EVP_CIPHER_CTX			oCtx;
	
	// Inicializaciones.
	ao_iLenTextOut = 0;
	ao_pcText = NULL;
	
	// Bug 0106119
	// Si no hay nada en la password la librería de terceros da un assert.
	if (ai_iLenCrypted == 0)
	{
		ao_pcText = new m4char_t[1];
		*ao_pcText = '\0';
		return M4_SUCCESS;
	}

	// Verficar la longitud de la clave.
	if (ai_iLenKey >= MAX_KEY_LEN)
	{
		return M4_ERROR;
	}

	// Clave de encriptación.
	memset(szKey, '\0', MAX_KEY_LEN);
	memcpy(szKey, ai_szKey, ai_iLenKey);
	szKey[ai_iLenKey] = '\0';
		
	// Inicializar el contexto.
	EVP_CIPHER_CTX_init(&oCtx);

	// Establecer el modo de descriptación.
	if (ai_bAESCrypted == M4_TRUE)
	{
		// Algoritmo de encriptación utilizado.
		oCipher = EVP_aes_256_cbc();
	}
	else
	{
		// Algoritmo de encriptación utilizado.
		oCipher = EVP_des_ede3_cbc();

		// La clave son 3 claves DES juntas.
		pcDesBlock = (des_cblock *) szKey;
		des_set_odd_parity(pcDesBlock);
		++pcDesBlock;
		des_set_odd_parity(pcDesBlock);
		++pcDesBlock;
		des_set_odd_parity(pcDesBlock);
	}
	
	// Verificar si el buffer de salida está alocado previamente.
	// iLenText = ai_iLenCrypted + EVP_CIPHER_block_size(oCipher);
	iLenText = M4MaxLenOfDeCrypted(ai_iLenCrypted);
	ao_pcText = new m4char_t[iLenText + 1];

	// Desencriptar.
	memset(iv, 0, sizeof(iv));
	EVP_DecryptInit(&oCtx, oCipher, (unsigned char *) szKey, iv);
	EVP_DecryptUpdate(&oCtx, (unsigned char *) ao_pcText, &ao_iLenTextOut, (unsigned char *) ai_szCrypted, ai_iLenCrypted);
	EVP_DecryptFinal(&oCtx, (unsigned char *) ao_pcText + ao_iLenTextOut, &iLenTextOut);
	ao_iLenTextOut += iLenTextOut;

	// Liberar el contexto.
	EVP_CIPHER_CTX_cleanup(&oCtx);

	if (ao_iLenTextOut == 0)
	{
		delete ao_pcText;
		ao_pcText = NULL;

		iRet = M4_ERROR;
	}
	else
	{
		ao_pcText[ao_iLenTextOut] = '\0';
	}

	return iRet;
}


m4return_t DecryptTwoWaySecondKey(m4pcchar_t ai_pcCrypted, m4int_t ai_iLenCrypted, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t & ao_pcText, m4int_t & ao_iLenTextOut)
{
	m4return_t	iRet = M4_SUCCESS;
	m4pchar_t	pcCripted = NULL;

	// Inicializaciones.
	ao_iLenTextOut = 0;
	ao_pcText = NULL;

	if (ai_iLenCrypted > 1)
	{
		// Verificar el segundo byte byte para saber si estamos con una segunda clave 
		if (*(ai_pcCrypted + 1) < '0' || *(ai_pcCrypted + 1) > 'f')
		{
			// Guardar buffer encriptado.
			pcCripted = new m4char_t[ai_iLenCrypted + 1];
			memcpy(pcCripted, ai_pcCrypted, ai_iLenCrypted);
			pcCripted[ai_iLenCrypted] = '\0';

			if (*(pcCripted + 1) < '0')
			{
				*(pcCripted + 1) += 10;
			}
			else
			{
				*(pcCripted + 1) -= 6;
			}

			iRet = DecryptTwoWay(pcCripted, ai_iLenCrypted, ai_szKey, ai_iLenKey, ao_pcText, ao_iLenTextOut);
			delete pcCripted;
			return iRet;
		}
	}

	iRet = DecryptTwoWay(ai_pcCrypted, ai_iLenCrypted, ai_szKey + ai_iLenKey + 1, ai_iLenKey, ao_pcText, ao_iLenTextOut);
	return iRet;
}


m4return_t DecryptTwoWay(m4pcchar_t ai_pcCrypted, m4int_t ai_iLenCrypted, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t & ao_pcText, m4int_t & ao_iLenTextOut)
{
	m4return_t		iRet = M4_SUCCESS;
	m4pchar_t		pcCripted = NULL;
	m4pchar_t		pcRaw = NULL;
	m4int_t			iLenRaw = 0;
	m4int_t			iLenRawOut = 0;
	m4bool_t		bAESCrypted = M4_FALSE;

	// Inicializaciones.
	ao_iLenTextOut = 0;
	ao_pcText = NULL;
	
	// Bug 0106119
	// Si no hay nada en la password la librería de terceros da un assert.
	if (ai_iLenCrypted == 0)
	{
		ao_pcText = new m4char_t[1];
		*ao_pcText = '\0';
		return M4_SUCCESS;
	}

	// Comprobar que la longitud de la cadena encriptada es par.
	if ((ai_iLenCrypted % 2) != 0)
	{
		return M4_ERROR;
	}

	// Guardar buffer encriptado.
	pcCripted = new m4char_t[ai_iLenCrypted + 1];
	memcpy(pcCripted, ai_pcCrypted, ai_iLenCrypted);
	pcCripted[ai_iLenCrypted] = '\0';

	// Verificar el primer byte para saber si estamos en modo antiguo (encriptación DES) o 
	// en modo nuevo (encriptación AES).
	
	// Modo antiguo (DES).
	if (*pcCripted >= '0' && *pcCripted <= 'f')
	{
		bAESCrypted = M4_FALSE;
	}
	else
	{
		// Modo nuevo (AES).
		bAESCrypted = M4_TRUE;

		if (*pcCripted < '0')
		{
			*pcCripted += 10;
		}
		else
		{
			*pcCripted -= 6;
		}
	}
	
	// Convertir hexadecimal a binario.
	iLenRaw = (ai_iLenCrypted / 2);
	pcRaw = new m4char_t[iLenRaw + 1];
	iRet = HEXtoASCII(pcCripted, ai_iLenCrypted, pcRaw, iLenRaw, iLenRawOut);
	if (iRet != M4_SUCCESS)
	{
		delete pcCripted;
		delete pcRaw;
		return M4_ERROR;
	}
	
	delete pcCripted;

	// Se desencripta atendiendo al modo de encriptación (AES o DES).
	iRet = _DecryptTwoWayRaw(pcRaw, iLenRawOut, ai_szKey, ai_iLenKey, bAESCrypted, ao_pcText, ao_iLenTextOut);
	if (iRet != M4_SUCCESS)
	{
		delete pcRaw;
		return M4_ERROR;
	}
	
	delete pcRaw;

	return M4_SUCCESS;
}


m4return_t EncryptTwoWayRaw(m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t & ao_pcCrypted, m4int_t & ao_iLenCryptedOut)
{
	m4return_t			iRet = M4_SUCCESS;
	m4int_t				iLenCrypted;
	m4int_t				iLenCryptedOut;
  	m4char_t			szKey[MAX_KEY_LEN];
	unsigned char		iv[EVP_MAX_IV_LENGTH];
	const EVP_CIPHER	* oCipher;
  	EVP_CIPHER_CTX		oCtx;

	// Inicializaciones.
	ao_iLenCryptedOut = 0;
	ao_pcCrypted = NULL;
	
	// Bug 0106119
	// Si no hay nada en la password la librería de terceros da un assert.
	if (ai_iLenText == 0) 
	{
		ao_pcCrypted = new m4char_t[1];
		*ao_pcCrypted = '\0';
		return M4_SUCCESS;
	}
	
	// Verficar la longitud de la clave.
	if (ai_iLenKey >= MAX_KEY_LEN)
	{
		return M4_ERROR;
	}

	// Clave de encriptación.
	memset(szKey, '\0', MAX_KEY_LEN);
	memcpy(szKey, ai_szKey, ai_iLenKey);
	szKey[ai_iLenKey] = '\0';

	// Algoritmo de encriptación utilizado.
	oCipher = EVP_aes_256_cbc();
  	
	// Alocar el buffer de salida.
	// iLenCrypted = 2 * EVP_CIPHER_block_size(oCipher) + ai_iLenText - 1;
	iLenCrypted = M4MaxLenOfEnCrypted(ai_iLenText);
	ao_pcCrypted = new m4char_t[iLenCrypted];
	
  	// Encriptar.
	memset(iv, 0, sizeof(iv));
	EVP_EncryptInit(&oCtx, oCipher, (unsigned char *) szKey, iv);
	EVP_EncryptUpdate(&oCtx, (unsigned char *) ao_pcCrypted, &ao_iLenCryptedOut, (unsigned char *) ai_szText, ai_iLenText); 
  	EVP_EncryptFinal(&oCtx, (unsigned char *) ao_pcCrypted + ao_iLenCryptedOut, &iLenCryptedOut);
	ao_iLenCryptedOut += iLenCryptedOut;

	// Liberar el contexto.
	EVP_CIPHER_CTX_cleanup(&oCtx);

	if (ao_iLenCryptedOut == 0)
	{
		delete ao_pcCrypted;
		ao_pcCrypted = NULL;

		iRet = M4_ERROR;
	}
	
	return iRet;
}


m4return_t DecryptTwoWayRaw(m4pcchar_t ai_szCrypted, m4int_t ai_iLenCrypted, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t & ao_pcText, m4int_t & ao_iLenTextOut)
{
	return _DecryptTwoWayRaw(ai_szCrypted, ai_iLenCrypted, ai_szKey, ai_iLenKey, M4_TRUE, ao_pcText, ao_iLenTextOut);
}


m4int_t	M4MaxLenOfEnCrypted( m4int_t ai_iSize )
{
	return M4_ROUND_TO_16(ai_iSize + 1);
}


m4int_t	M4MaxLenOfDeCrypted( m4int_t ai_iSize )
{
	return ai_iSize;
}


// AES - Borrar
m4return_t SymCryptASCII ( m4pcchar_t ai_szText , m4int_t ai_iLenText , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szCrypt , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) 
{
	ClKey					* poKey ;									  
	Cl3DESCryptography		* poCrypt;
	ClCryptContext			* poContext ;
	m4return_t				iRet = M4_SUCCESS ;

	//Bug 0106119
	//Si no hay nada en la password la librería de terceros da un assert.
	if (ai_iLenText == 0) {
		ao_iLenOut = 1;
		ao_szCrypt[0] = NULL;		
		return iRet;
	}

	poKey = new ClKey (  ai_szKey , ai_iLenKey ) ;

	poCrypt = new Cl3DESCryptography ( poKey ) ;

	poContext = poCrypt -> EncryptStart();

	poCrypt -> Encrypt(poContext , (void *)ai_szText , ai_iLenText , ao_szCrypt , &ao_iLenOut );

	ai_iLenText = ao_iLenOut ;

	poCrypt -> EncryptFinish(poContext, ao_szCrypt+ ai_iLenText , &ao_iLenOut );

	ao_iLenOut+= ai_iLenText ;
	
	delete poKey ;

	delete poCrypt ;

	if (! ao_iLenOut )
		iRet = M4_ERROR ;

	ao_szCrypt[ao_iLenOut] = 0;

	return iRet ;
}


// AES - Borrar
m4return_t SymDeCryptASCII ( m4pcchar_t ai_szCrypt , m4int_t ai_iLenCrypt , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szText , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) 
{
	ClKey					* poKey ;									  
	Cl3DESCryptography		* poCrypt;
	ClCryptContext			* poContext ;
	m4return_t				iRet = M4_SUCCESS ;

	//Bug 0106119
	//Si no hay nada en la password la librería de terceros da un assert.
	if (ai_iLenCrypt == 0) {
		ao_iLenOut = 1;
		ao_szText[0] = NULL;
		return iRet;
	}

	poKey = new ClKey ( ai_szKey , ai_iLenKey ) ;

	poCrypt = new Cl3DESCryptography ( poKey ) ;

	poContext = poCrypt -> DecryptStart();

	poCrypt -> Decrypt(poContext, (void *)ai_szCrypt , ai_iLenCrypt , (m4pchar_t ) ao_szText, &ao_iLenOut);

	ai_iLenCrypt = ao_iLenOut ;

	poCrypt -> DecryptFinish(poContext, ( m4pchar_t ) ( ao_szText + ai_iLenCrypt ), &ao_iLenOut);

	ao_iLenOut+= ai_iLenCrypt ;

	ao_szText [ ao_iLenOut ] = 0 ;

	if (! ao_iLenOut )
		iRet = M4_ERROR ;

	delete poKey ;
	delete poCrypt ;

	ao_szText [ao_iLenOut] =0 ;

	return iRet ;
}



m4return_t ASCIItoHEX ( m4pcchar_t ai_szASCII , m4int_t ai_iLen , m4pchar_t ao_szHEX , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut )
{
	m4int_t iCont ;
	m4char_t aux [10];
	m4uint8_t car ;

	m4int32_t iRequiredBufferLen = (ai_iLen * 2);
	if ( ai_iLenBufferOut < iRequiredBufferLen )
	{
		ao_iLenOut = iRequiredBufferLen;
		return M4_ERROR ;
	}

	for ( iCont =0 ; iCont < ai_iLen ; iCont ++ )
	{
		car = ai_szASCII [iCont] ;

		sprintf ( aux , "%2xc" , car );

		if ( car < 16 )
			aux[0] = '0' ;

		memcpy ( ao_szHEX + (iCont * 2) , aux , 2 ) ;
	}

	ao_szHEX [ iCont * 2 ] = 0 ;

	ao_iLenOut = iCont *2 ;

	return M4_SUCCESS;
}



m4return_t HEXtoASCII ( m4pcchar_t ai_szHEX , m4int_t ai_iLen , m4pchar_t ao_szASCII , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut )
{
	m4int32_t iCont ;
	m4char_t aux [5] = "0x";
	m4uchar_t car ;
	m4int_t icar ;

	m4int32_t iRequiredBufferLen = (ai_iLen / 2);
	if (ai_iLenBufferOut < iRequiredBufferLen)
	{
		ao_iLenOut = iRequiredBufferLen;
		return M4_ERROR;
	}

	ao_iLenOut = 0;

	for ( iCont = 0 ; iCont < ai_iLen ; iCont +=2 , ao_iLenOut ++ )
	{
		strncpy ( aux  , ai_szHEX + iCont , 2 ) ;

		aux[2] = 0;

		sscanf ( aux , "%x" , &icar ) ;

		car = icar ;

		ao_szASCII [ ao_iLenOut ] = car ;
	}

	ao_szASCII [ ao_iLenOut ] = 0;

	return M4_SUCCESS ;
}
