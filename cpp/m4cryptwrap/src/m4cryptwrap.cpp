//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cryptwrap
// File:                
// Project:             peoplenet
// Author:              Meta4 Spain S.A
// Date:                08-03-2006
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     
//
//
// Definition:
//
//   
//	  
//
//==============================================================================


#include <m4cryptwrap.hpp>
#include <m4cryptc.hpp>


long M4_DECL_M4CRYPTWRAP WINAPI SynCryptSecondKeyWrap( m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t ao_szCrypt, m4int_t ai_iLenBufferOut, m4int_t & ao_iLenOut )
{
	m4pchar_t	pcCrypted = NULL;

	m4return_t	iResult = EncryptTwoWaySecondKey(ai_szText, ai_iLenText, ai_szKey, ai_iLenKey * sizeof(m4char_t), pcCrypted, ao_iLenOut);

	if (iResult != M4_SUCCESS)
	{
		return ( -1 );
	}

	// Verificar que el buffer encriptado devuelto me cabe en el array de salida.
	if (ai_iLenBufferOut < ao_iLenOut + 1)
	{
		delete pcCrypted;
		return ( -1 );
	}
	
	// Copiar el buffer desencriptado. Ya copia el nulo final.
	memcpy(ao_szCrypt, pcCrypted, ao_iLenOut + 1);

	delete pcCrypted;

	return ( 0 );
}


long M4_DECL_M4CRYPTWRAP WINAPI SynCryptWrap( m4pcchar_t ai_szText, m4int_t ai_iLenText, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t ao_szCrypt, m4int_t ai_iLenBufferOut, m4int_t & ao_iLenOut )
{
	m4pchar_t	pcCrypted = NULL;

	m4return_t	iResult = EncryptTwoWay(ai_szText, ai_iLenText, ai_szKey, ai_iLenKey * sizeof(m4char_t), pcCrypted, ao_iLenOut);

	if (iResult != M4_SUCCESS)
	{
		return ( -1 );
	}

	// Verificar que el buffer encriptado devuelto me cabe en el array de salida.
	if (ai_iLenBufferOut < ao_iLenOut + 1)
	{
		delete pcCrypted;
		return ( -1 );
	}
	
	// Copiar el buffer desencriptado. Ya copia el nulo final.
	memcpy(ao_szCrypt, pcCrypted, ao_iLenOut + 1);

	delete pcCrypted;

	return ( 0 );
}


long M4_DECL_M4CRYPTWRAP WINAPI SynDeCryptSecondKeyWrap( m4pcchar_t ai_szCrypt, m4int_t ai_iLenCrypt, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t ao_szText, m4int_t ai_iLenBufferOut, m4int_t & ao_iLenOut )
{
	m4pchar_t	pcText = NULL;

	m4return_t	iResult = DecryptTwoWaySecondKey(ai_szCrypt, ai_iLenCrypt, ai_szKey, ai_iLenKey * sizeof(m4char_t), pcText, ao_iLenOut);

	if (iResult != M4_SUCCESS)
	{
		return ( -1 );
	}

	// Verificar que el buffer desencriptado devuelto me cabe en el array de salida.
	if (ai_iLenBufferOut < ao_iLenOut + 1)
	{
		delete pcText;
		return ( -1 );
	}
	
	// Copiar el buffer desencriptado. Ya copia el nulo final.
	memcpy(ao_szText, pcText, ao_iLenOut + 1);

	delete pcText;

	return ( 0 );
}


long M4_DECL_M4CRYPTWRAP WINAPI SynDeCryptWrap( m4pcchar_t ai_szCrypt, m4int_t ai_iLenCrypt, m4pcchar_t ai_szKey, m4int_t ai_iLenKey, m4pchar_t ao_szText, m4int_t ai_iLenBufferOut, m4int_t & ao_iLenOut )
{
	m4pchar_t	pcText = NULL;

	m4return_t	iResult = DecryptTwoWay(ai_szCrypt, ai_iLenCrypt, ai_szKey, ai_iLenKey * sizeof(m4char_t), pcText, ao_iLenOut);

	if (iResult != M4_SUCCESS)
	{
		return ( -1 );
	}

	// Verificar que el buffer desencriptado devuelto me cabe en el array de salida.
	if (ai_iLenBufferOut < ao_iLenOut + 1)
	{
		delete pcText;
		return ( -1 );
	}
	
	// Copiar el buffer desencriptado. Ya copia el nulo final.
	memcpy(ao_szText, pcText, ao_iLenOut + 1);

	delete pcText;

	return ( 0 );
}

