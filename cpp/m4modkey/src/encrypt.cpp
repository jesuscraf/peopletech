//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#include <m4cryptc.hpp>
#include <m4cryptcobj.hpp>

#include "encrypt.hpp"
#include "key.hpp"

#include "license.hpp"

#include "m4date.hpp"

#include "error.hpp"

#ifndef _WINDOWS
	#include <netinet/in.h>
#endif

//----------------------------------------------
//Clase ClEncryption
//----------------------------------------------
ClEncryption::ClEncryption( void )
{
	m_pMaskKeyB = NULL;
	m_iSizeMaskKeyB = 0;
}

ClEncryption::~ClEncryption( void )
{
	if (m_pMaskKeyB != NULL) {
		delete [] m_pMaskKeyB;
		m_pMaskKeyB = NULL;
	}
	m_iSizeMaskKeyB = 0;
}

void ClEncryption::_DecryptWithKeyA( m4pchar_t ai_pBufferEncryptedWithKeyA, m4int32_t ai_iSizeEncryptedWithKeyA )
{
	m4pchar_t pBufferEncryptedWithKeyA = ai_pBufferEncryptedWithKeyA;
	m4int32_t iSizeEncryptedWithKeyA = ai_iSizeEncryptedWithKeyA;

	if ((pBufferEncryptedWithKeyA == NULL) || (iSizeEncryptedWithKeyA == 0)) {
		return;
	}

	//Desencripta el buffer con la clave A.
	//Se realizan los pasos contrarios a la encriptación.

	//Para encriptar (pe tierra):
	//- Hacemos swap de 6º con 1º:			aierrt
	//- Nos saltamos el 5º.
	//- Hacemos swap de 4º con 3º:			airert
	//
	//- A la cadena resultado le sumamos un delta.

	//Para desencriptar se realizan los pasos inversos.
	//- Restamos un delta.
	//- Hacemos el swap inverso. (El swap se hace con el bucle que recorre la cadena).
	
	for (m4int32_t i=0; i<iSizeEncryptedWithKeyA; i++)
	{		
		*pBufferEncryptedWithKeyA = ((*pBufferEncryptedWithKeyA) - M4MODKEY_ENCRYPT_KEYA);
		pBufferEncryptedWithKeyA++;
	}

	//Restauramos para dar otra vuelta.
	pBufferEncryptedWithKeyA = ai_pBufferEncryptedWithKeyA;
	
	m4int32_t iLeft = 0;
	m4int32_t iRight = (iSizeEncryptedWithKeyA - 1);	
	while (iLeft <= iRight)
	{
		if ((iLeft % 2) == 0) {
			//Es caracter par.			
			m4char_t cTemp = *(pBufferEncryptedWithKeyA + iRight);
			*(pBufferEncryptedWithKeyA + iRight) = *(pBufferEncryptedWithKeyA + iLeft);
			*(pBufferEncryptedWithKeyA + iLeft) = cTemp;
		}
		iLeft++;
		iRight--;
	}
}

m4return_t ClEncryption::_CalculateCheckSum( m4pcchar_t ai_pText , m4int_t ai_iLenText, m4pchar_t & ao_pBufferCheckSum, m4int_t & ao_iSizeCheckSum )
{
	//Esta funcion nos da un checksum para el buffer de entrada.

	//El checksum no es terminado en nulo!!. El tamaño que devolvemos es el tamaño en 
	//bytes del checksum.

	//Los dos parametros de salida son:
	//- Buffer donde estara el checksum y sera responsabilidad del invocante de liberar.
	//- Tamaño del checksum.
	
	//El tamaño que se reserva es proporcional al buffer de entrada sobre el cual queremos calcular
	//el checksum.

	ao_pBufferCheckSum = NULL;
	ao_iSizeCheckSum = -1;

	ClMD5Digest oMD5Digest ;
	m4int_t iSizeCheckSum = 0;
	m4pchar_t aux ;
	m4uint32_t iAuxLen ;

	iAuxLen= (ai_iLenText + 1 + 8 ) * 2 ;
	aux = new m4char_t [iAuxLen];
	if (aux == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}

	//Copiamos el buffer de entrada..	
	memcpy ( aux , ai_pText , ai_iLenText + 1);

	ClMDContext* poContext = oMD5Digest.DigestStart();
	oMD5Digest.Digest(poContext, aux, ai_iLenText);
	oMD5Digest.DigestFinish(poContext, aux ,  &iSizeCheckSum);
	if (iSizeCheckSum <= 0) {
		delete [] aux;
		return M4_ERROR;
	}

	//A esta funcion se le pasa un buffer y su tamaño para que calcule el checksum.
	//El buffer de salida y su longitud (debe ser por mayor de (2 * ai_iLenText) + 1.
	//El quinto argumento es el tamaño del buffer real con que nos hemos quedado.
	//m4int_t dRealSize;

	//ASCIItoHEX ( aux , dlen2 , ao_pBufferCheckSum , ao_iSizeCheckSum , dRealSize ) ;

	//Creamos el buffer de salida NO es NULL terminado
	ao_pBufferCheckSum = new m4char_t [iSizeCheckSum];
	if (ao_pBufferCheckSum == NULL) {
		delete [] aux;
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}

	memcpy(ao_pBufferCheckSum, aux, sizeof(m4char_t) * iSizeCheckSum );
	delete [] aux;
	ao_iSizeCheckSum = iSizeCheckSum;

	return M4_SUCCESS;
}

/*
void ClEncryption::_ASCIItoHEX ( m4pcchar_t ai_szASCII , m4int_t ai_iLen , m4pchar_t ao_szHEX , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut )
{
	m4int_t iCont ;
	m4char_t aux [10];
	m4uint8_t car ;

	if ( ai_iLenBufferOut < ( ( ai_iLen *2 )  + 1 ) )
	{
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
*/

m4bool_t ClEncryption::_ExistsHeader( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted )
{
	//------------------------------------------
	//Tiene que haber algo para desencriptar.  No comparo tamaño pq puede estar vacio el buffer.
	if (ai_pBufferEncrypted ==  NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_FALSE;
	}
	//------------------------------------------

	//Podemos hacer cast a sin signo pq seguro que no es negativo.
	//Tiene que estar la cabecera!.
	if (((m4uint_t) ai_iSizeEncrypted) < strlen(M4MODKEY_ENCRYPT_HEADER)){		
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_FALSE;
	}
	
	//Comprueba que esta la cabecera y que coincide el identificador.
	if (strncmp(ai_pBufferEncrypted, M4MODKEY_ENCRYPT_HEADER, strlen(M4MODKEY_ENCRYPT_HEADER)) != 0) {
		//No esta la cabecera.
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_FALSE;
	}

	//Si está la cabecera!.

	return M4_TRUE;
}

m4return_t ClEncryption::_InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey )
{
	if (ai_pClEncryption == NULL) {
		return M4_ERROR;
	}

	//------------- IMPORTANTE --------------------------
	//Tenemos que seguir el protocolo.
	m4date_t dNow;
	ClActualDate (dNow);
	m4int32_t iSizeKeyA = sizeof(dNow) + strlen(ai_pKey);
	m4char_t * pKeyA = new m4char_t[iSizeKeyA + 1];
	if (pKeyA == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(pKeyA, &dNow, sizeof(dNow));
	memcpy(pKeyA + sizeof(dNow), ai_pKey, strlen(ai_pKey));
	pKeyA[iSizeKeyA] = NULL;
	
	//----------------------------------------------------
	//Aplicamos la encriptación de la ClaveA.
	//----------------------------------------------------
	m4pchar_t pBufferEncryptedWithKeyA = pKeyA;
	m4int32_t iSizeEncryptedWithKeyA = iSizeKeyA;

	m4int32_t iLeft = 0;
	m4int32_t iRight = (iSizeEncryptedWithKeyA - 1);	
	while (iLeft <= iRight)
	{
		if ((iLeft % 2) == 0) {
			//Es caracter par.			
			m4char_t cTemp = *(pBufferEncryptedWithKeyA + iRight);
			*(pBufferEncryptedWithKeyA + iRight) = *(pBufferEncryptedWithKeyA + iLeft);
			*(pBufferEncryptedWithKeyA + iLeft) = cTemp;
		}
		iLeft++;
		iRight--;
	}

	//Restauramos para dar otra vuelta.
	pBufferEncryptedWithKeyA = pKeyA;

	for (m4int32_t i=0; i<iSizeEncryptedWithKeyA; i++)
	{			
		*pBufferEncryptedWithKeyA = ((*pBufferEncryptedWithKeyA) + M4MODKEY_ENCRYPT_KEYA);
		pBufferEncryptedWithKeyA++;
	}
	//----------------------------------------------------
	//FIN Aplicamos la encriptación de la ClaveA.
	//----------------------------------------------------

	//Emviamos la clave a utilizar!. Esperamos una fecha.
	m4char_t * pSignature = new m4char_t[sizeof(dNow) + 1];
	if (ai_pClEncryption->SendKeyB( pKeyA, iSizeKeyA, pSignature, sizeof(dNow) ) == M4_ERROR) {
		delete [] pKeyA;
		delete [] pSignature;
		ClErrorManager::DumpError( M4MODKEY_ERROR_SEND_KEY );
		return M4_ERROR;
	}

	//Verificamos que la fecha es correcta.
	if (memcmp(&dNow, pSignature, sizeof(dNow)) != 0) {
		delete [] pKeyA;
		delete [] pSignature;
		ClErrorManager::DumpError( M4MODKEY_ERROR_VERIFYING_KEY );		
		return M4_ERROR;
	}

	//Si estamos aqui es pq el protocolo se ha validado. Desmontamos la paraeta.
	delete [] pKeyA;
	delete [] pSignature;
	pKeyA = NULL;
	pSignature = NULL;
	//------------- FIN IMPORTANTE --------------------------

	return M4_SUCCESS;
}

m4return_t ClEncryption::SendKeyB( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted, m4pchar_t ao_pSignature, m4int32_t ai_iSizeSignature )
{
	//--------------------------------------------------------
	//PROTOCOLO: Acuerdo de la clave a utilizar!.
	//--------------------------------------------------------

	//--------------------------------------------------------
	//Solucionamos el problema de pasar la clave de encriptación!.
	//El que invoca decide con qué clave se encripta.

	//Escenario.
	//1.- Nos llaman con un buffer (Hora#ClaveB) encriptado con una determinada
	//	ClaveA. Puede ser muy sencilla.
	//2.- Aqui la podemos desencriptar, pq conocemos ClaveA y obtendremos 
	//	el buffer: (Hora#ClaveB)
	//3.- Extraemos la hora y encriptamos el buffer: (Hora) con ClaveA y lo retornamos.
	//4.- El que nos invocó desencriptará con ClaveA y comprueba si la 
	//	(Hora) coincide con la que envió.	

	//Nota: El tamaño de (Hora) lo sabe el que me invoca por lo tanto no hace falta
	//pasarlo de salida. NO ES NECESARIO PASAR UNA FIRMA!.

	//Si coincide entonces todo OK. La clave a utilizar es la clave B.
	//--------------------------------------------------------
	
	//Si ya se ha establecido una clave no podemos establecer otra.
	if (m_pMaskKeyB != NULL) {		
		return M4_ERROR;
	}

	//Nos tienen que pasar algo encriptado.
	if (ai_pBufferEncrypted == NULL) {
		return M4_ERROR;
	}

	//-------------------------------------------------------
	//---- Desencriptamos con ClaveA. -----
	m4pchar_t pBufferEncryptedWithKeyA = new m4char_t [ai_iSizeEncrypted + 1];
	if (pBufferEncryptedWithKeyA == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(pBufferEncryptedWithKeyA, ai_pBufferEncrypted, ai_iSizeEncrypted);
	pBufferEncryptedWithKeyA[ai_iSizeEncrypted] = NULL;

	_DecryptWithKeyA( pBufferEncryptedWithKeyA, ai_iSizeEncrypted );	
	//-------------------------------------------------------

	//---------------- IMPORTANTE ---------------------------
	//El cuarto parametro me indica la longitud de lo que espera el invocante.
	//Pero no es necesario!!.
	if (ao_pSignature != NULL) {
		memcpy(ao_pSignature, pBufferEncryptedWithKeyA, ai_iSizeSignature);
		ao_pSignature[ai_iSizeSignature] = NULL;
	}
	else {
		//Por si acaso me lo han pasado mal!.
		ai_iSizeSignature = 0;
	}
	//-------------------------------------------------------

	//-------------------------------------------------------
	//Lo que queda es la ClaveB, que nos estan pasando. Es la clave que se va a utiizar.

	//IMPORTANTE: En realidad nos vamos a guardar la ClaveB enmascarada.	
	m_iSizeMaskKeyB = ai_iSizeEncrypted - ai_iSizeSignature;
	m_pMaskKeyB = new m4char_t [m_iSizeMaskKeyB + 1];	
	if (m_pMaskKeyB == NULL) {		
		delete[] pBufferEncryptedWithKeyA;
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(m_pMaskKeyB, pBufferEncryptedWithKeyA + ai_iSizeSignature, m_iSizeMaskKeyB);
	m_pMaskKeyB[m_iSizeMaskKeyB] = NULL;
	for (m4int32_t j=0; j<m_iSizeMaskKeyB; j++)
	{
		*(m_pMaskKeyB + j) = (*(m_pMaskKeyB + j)) + M4MODKEY_ENCRYPT_MASK_DELTA;
	}
	//-------------------------------------------------------

	delete[] pBufferEncryptedWithKeyA;
	pBufferEncryptedWithKeyA = NULL;

	return M4_SUCCESS;
}

m4return_t ClEncryption::Encrypt( m4pcchar_t ai_pText , m4int_t ai_iLenText, m4pchar_t & ao_pBufferEncrypted, m4int_t & ao_iSizeEncrypted )
{
	//El tamaño del buffer de texto NO incluye el null terminado!!. Pero el texto SI es null terminado!.
	//La salida será una cadena null terminada y el tamaño de salida no incluye el null terminado.
	//P.e:
	//ai_pText				= Jamon				ai_iLenText = 5
	//ao_pBufferEncrypted	= 69a683521b578796	ao_iSizeEncrypted = 16

	//Encripta un texto limpio de una longitud.
	//Retorna un puntero a un buffer y su longitud donde está encriptada la informacion.

	//Importante: La liberacion del buffer la realizará el invocante!.

	ao_pBufferEncrypted = NULL;
	ao_iSizeEncrypted = 0;

	//------------------------------------------
	//Tiene que haber algo para encriptar. No comparo tamaño pq puede estar vacio el buffer.
	if (ai_pText ==  NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_ENCRYPT );
		return M4_ERROR;
	}
	//------------------------------------------

	//------------------------------------------
	//Tiene que haber clave.
	if ((m_pMaskKeyB == NULL) || (m_iSizeMaskKeyB == 0)) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_ENCRYPT );
		return M4_ERROR;
	}
	//------------------------------------------

	//------------------------------------------
	//Vamos a sacar el checksum del buffer que nos pasan.	
	m4pchar_t pBufferCheckSum = NULL;
	m4int_t iSizeCheckSum = 0;
	if (ClEncryption::_CalculateCheckSum(ai_pText , ai_iLenText, pBufferCheckSum, iSizeCheckSum) == M4_ERROR) {		
		ClErrorManager::DumpError( M4MODKEY_ERROR_ENCRYPT );	
		return M4_ERROR;
	}
	//------------------------------------------

	//Si todo ha ido bien, tendremos:
	//En iSizeCheckSum el tamaño del checksum.
	//En pBufferCheckSum un buffer con el checksum. Se habrá reservado un buffer con la memoria necesaria.
	
	//Lo que al final encriptaremos será: el tamaño del checksum, el checksum, el tamaño del texto y el texto.
	//Eso será el texto limpio qeu definitivamente encriptaremos.	
	m4int_t iText = sizeof(iSizeCheckSum) + iSizeCheckSum + sizeof(ai_iLenText) + ai_iLenText;

	//------------------------------------------
	//El texto limpio va a ser el checksum + texto que queremos encriptar.	
	m4pchar_t pText = new m4char_t [ iText + 1];
	if (pText == NULL) {
		delete [] pBufferCheckSum;
		ClErrorManager::DumpError( M4MODKEY_ERROR_ENCRYPT );
		return M4_ERROR;
	}

	//Copiamos buffers: entero que indica el tamaño del checksum, el checksum,
	//el entero que indica el tamaño del texto y el texto.	

	//Empleamos las funciones de transformacion de enteros para compatibilidad Unix.
	//El numero que queremos guardar lo pasamos por la funcion htonl.
	m4int32_t iSizeCheckSumHostToNetwork = htonl(iSizeCheckSum);
	memcpy(pText, &iSizeCheckSumHostToNetwork, sizeof(iSizeCheckSumHostToNetwork));
	memcpy(pText + sizeof(iSizeCheckSumHostToNetwork), pBufferCheckSum, iSizeCheckSum);
	m4int32_t ai_iLenTextHostToNetwork = htonl(ai_iLenText);
	memcpy(pText + sizeof(iSizeCheckSumHostToNetwork) + iSizeCheckSum, &ai_iLenTextHostToNetwork, sizeof(ai_iLenTextHostToNetwork));
	memcpy(pText + sizeof(iSizeCheckSumHostToNetwork) + iSizeCheckSum + sizeof(ai_iLenTextHostToNetwork), ai_pText, ai_iLenText);
	pText[iText] = NULL;
	//------------------------------------------

	//------------------------------------------
	//Antes de utilizar la ClaveB hay que desenmascararla.
	m4int32_t iSizeKey = m_iSizeMaskKeyB;
	m4pchar_t pKey = new m4char_t [iSizeKey + 1];
	if (pKey == NULL) {
		delete [] pBufferCheckSum;
		delete [] pText;
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(pKey, m_pMaskKeyB, iSizeKey);	
	pKey[iSizeKey] = NULL;
	for (m4int32_t j=0; j<iSizeKey; j++) {
		*(pKey + j) = (*(pKey + j)) - M4MODKEY_ENCRYPT_MASK_DELTA;
	}

	//Encriptamos la cadena.
	m4int_t		iCryptLen = 0;
	m4pchar_t	pCrypt = NULL;
	if (EncryptTwoWay(pText, iText, pKey, iSizeKey * sizeof(m4char_t), pCrypt, iCryptLen) == M4_ERROR) {
		//Es un error encriptando.
		delete [] pKey;
		delete [] pBufferCheckSum;
		delete [] pText;
		ClErrorManager::DumpError( M4MODKEY_ERROR_ENCRYPT );
		return M4_ERROR;
	}

	delete [] pKey;
	pKey = NULL;

	//El buffer auxiliar sí lo liberamos.
	delete [] pBufferCheckSum;
	delete [] pText;

	//La liberacion del buffer sera resposabilidad del que invoca a la encriptacion.
	ao_pBufferEncrypted = pCrypt;
	ao_iSizeEncrypted = iCryptLen;
	return M4_SUCCESS;
}

m4return_t ClEncryption::Decrypt( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted, m4pchar_t & ao_pText , m4int_t & ao_iLenText )
{
	//Desencripta el buffer de entrada.
	//El buffer de entrada es null terminado y la salida tambien.

	//Pero los tamaños NO incluyen en null final!.

	ao_pText = NULL;
	ao_iLenText = 0;

	//------------------------------------------
	//Tiene que haber algo para desencriptar.  No comparo tamaño pq puede estar vacio el buffer.
	if (ai_pBufferEncrypted ==  NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}
	//------------------------------------------

	//------------------------------------------
	//Tiene que haber clave.
	if ((m_pMaskKeyB == NULL) || (m_iSizeMaskKeyB == 0)) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}
	
	//------------------------------------------
	//Antes de utilizar la ClaveB hay que desenmascararla.
	m4int32_t iSizeKey = m_iSizeMaskKeyB;
	m4pchar_t pKey = new m4char_t [iSizeKey + 1];	
	if (pKey == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(pKey, m_pMaskKeyB, iSizeKey);	
	pKey[iSizeKey] = NULL;
	for (m4int32_t j=0; j<iSizeKey; j++) {
		*(pKey + j) = (*(pKey + j)) - M4MODKEY_ENCRYPT_MASK_DELTA;
	}
	
	//Desencriptamos.
	m4int_t		iDeCryptLen = 0;
	m4pchar_t	pDeCrypt = NULL;
	if (DecryptTwoWay(ai_pBufferEncrypted, ai_iSizeEncrypted, pKey, iSizeKey * sizeof(m4char_t), pDeCrypt, iDeCryptLen) == M4_ERROR) {
		//Error en la propida desencriptacion.
		delete [] pKey;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}
	delete [] pKey;
	pKey = NULL;

	//Hemos desencriptado un buffer, ahora vamos a interpretarlo!. Debe tener los mismos elementos que se añadieron en
	//la encriptacion.

	//Extraemos el tamaño del checkSum.
	//Tenemos que realizar la operacion contraria a como se encriptó.
	m4int_t iSizeCheckSumNetworkToHost = 0;
	memcpy(&iSizeCheckSumNetworkToHost, pDeCrypt, sizeof(iSizeCheckSumNetworkToHost));
	m4int_t iSizeCheckSum = ntohl(iSizeCheckSumNetworkToHost);
	if (iSizeCheckSum <= 0) {
		delete [] pDeCrypt;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}

	//Extraemos el checksum que lo tendremos que comparar.
	m4pchar_t pBufferCheckSum = NULL;
	pBufferCheckSum = new m4char_t [iSizeCheckSum];
	if ((pBufferCheckSum == NULL) || (iSizeCheckSum > (iDeCryptLen - sizeof(iSizeCheckSum)))){
		if (pBufferCheckSum != NULL) {
			delete [] pBufferCheckSum;
		}
		delete [] pDeCrypt;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}	
	memcpy(pBufferCheckSum, pDeCrypt + sizeof(iSizeCheckSum), iSizeCheckSum);

	//Extraemos el tamaño del buffer limpio.
	//Tenemos que realizar la operacion contraria a como se encriptó.
	m4int_t iTextNetworkToHost = 0;
	memcpy(&iTextNetworkToHost, pDeCrypt + sizeof(iSizeCheckSum) + iSizeCheckSum, sizeof(iTextNetworkToHost));
	m4int_t iText = ntohl(iTextNetworkToHost);
	if (iText <= 0) {
		delete [] pDeCrypt;
		delete [] pBufferCheckSum;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}
	
	//Extraemos el texto limpio, pero tenemos que calcular su checksum y compararlo con el que se guardó.
	//Vamos a hacerlo null terminado.
	m4char_t * pText = new m4char_t [iText + 1];
	if (pText ==  NULL) {
		delete [] pDeCrypt;
		delete [] pBufferCheckSum;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}
	memcpy(pText, pDeCrypt + sizeof(iSizeCheckSum) + iSizeCheckSum +  sizeof(iText), iText);
	pText[iText] = NULL;

	// Liberar el buffer desencriptado.
	delete [] pDeCrypt;

	//Calculamos el checksum que debe coincidir con el que se guardó.

	//------------------------------------------
	//Vamos a sacar el checksum del buffer que nos pasan.	
	m4pchar_t pBufferCheckSumNow = NULL;
	m4int_t iSizeCheckSumNow = 0;
	if (ClEncryption::_CalculateCheckSum(pText, iText, pBufferCheckSumNow, iSizeCheckSumNow) == M4_ERROR) {
		delete [] pBufferCheckSum;
		delete [] pText;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}
	//------------------------------------------

	//Deben coincidir en tamaño y contenido.
	if ((iSizeCheckSum != iSizeCheckSumNow) || (memcmp(pBufferCheckSum, pBufferCheckSumNow, iSizeCheckSum) != 0)) {
		delete [] pBufferCheckSum;
		delete [] pBufferCheckSumNow;
		delete [] pText;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}
	
	delete [] pBufferCheckSum;
	delete [] pBufferCheckSumNow;	

	//El buffer que nos da la funcion de desencriptacion SI es null terminado, pero para el texto lo tendremos que
	//null terminar nosotros.
	pText[iText] = NULL;
	ao_pText = pText;	
	ao_iLenText = iText;

	return M4_SUCCESS;
}

m4return_t ClEncryption::EncryptSecurizedObject( const ClSecurizedObject & oClSecurizedObject, m4pcchar_t ai_pText , m4int_t ai_iLenText, m4pchar_t & ao_pBufferEncrypted, m4int_t & ao_iSizeEncrypted )
{
	//Importante:
	//Antes de invocar a esta funcion, se ha tenido que establecer la clave de
	//encriptacion/desencriptacion.

	//El invocante se encarga de liberar el buffer de salida!.

	//Nos pasan el objeto segurizable y la regla que deseamos encriptar!.

	ao_pBufferEncrypted = NULL;
	ao_iSizeEncrypted = 0;

	//------------------------------------------
	//Tiene que haber algo para encriptar.
	if (ai_pText ==  NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_ENCRYPT );
		return M4_ERROR;
	}
	//------------------------------------------

	//Obtenemos el identificador del item y también lo encriptamos!.
	string oIdSecurizedObject = oClSecurizedObject.GetIdSecurizedObject();

	m4pchar_t pText = NULL;
	pText = new m4char_t [oIdSecurizedObject.size() + ai_iLenText + 1];
	if (pText == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(pText, oIdSecurizedObject.c_str(), oIdSecurizedObject.size());
	memcpy(pText + oIdSecurizedObject.size(), ai_pText, ai_iLenText);
	pText[oIdSecurizedObject.size() + ai_iLenText] = NULL;

	//Ahora sí encriptamos el buffer.
	m4pchar_t pBuffer = NULL;
	m4int_t iSizeBuffer = 0;
	if (Encrypt(pText, oIdSecurizedObject.size() + ai_iLenText, pBuffer, iSizeBuffer) == M4_ERROR) {
		delete [] pText;
		pText = NULL;
		return M4_ERROR;
	}
	//Ya no nos sirve, despues la reutilizaremos.
	delete [] pText;
	pText = NULL;

	//Si estamos aqui es pq se ha encriptado correctamente. Falta el paso de indicar buffer encriptado.

	//Importante. La desencriptacion devuelve un buffer null terminado!.	
	pText = new m4char_t [strlen(M4MODKEY_ENCRYPT_HEADER) + iSizeBuffer + 1];
	if (pText == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(pText, M4MODKEY_ENCRYPT_HEADER, strlen(M4MODKEY_ENCRYPT_HEADER));
	memcpy(pText + strlen(M4MODKEY_ENCRYPT_HEADER), pBuffer, iSizeBuffer);
	pText[strlen(M4MODKEY_ENCRYPT_HEADER) + iSizeBuffer] = NULL;

	//Definitivamente ya tenemos el buffer encriptado en el formato que queriamos.
	ao_pBufferEncrypted = pText;
	ao_iSizeEncrypted = strlen(M4MODKEY_ENCRYPT_HEADER) + iSizeBuffer;

	return M4_SUCCESS;
}

m4return_t ClEncryption::DecryptSecurizedObject( const ClSecurizedObject & oClSecurizedObject, m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted, m4pchar_t & ao_pText, m4int_t & ao_iLenText )
{
	//Es el proceso inverso a la encriptación.

	ao_pText = NULL;
	ao_iLenText = 0;

	//------------------------------------------
	//Tiene que haber algo para desencriptar.
	if (ai_pBufferEncrypted ==  NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}
	//------------------------------------------

	//------------------------------------------
	//El primer paso es comprobar la cabecera, que tiene que existir!.
	if (_ExistsHeader( ai_pBufferEncrypted, ai_iSizeEncrypted ) == M4_FALSE) {
		return M4_ERROR;
	}
	//------------------------------------------

	//La cabecera existe, desencriptamos el resto.
	m4pcchar_t pBufferEncrypted = ai_pBufferEncrypted + strlen(M4MODKEY_ENCRYPT_HEADER);
	m4int32_t iSizeEncrypted = ai_iSizeEncrypted - strlen(M4MODKEY_ENCRYPT_HEADER);

	m4pchar_t pText = NULL;
	m4int_t iLenText = 0;
	//------------------------------------------	
	if (Decrypt(pBufferEncrypted, iSizeEncrypted, pText, iLenText) == M4_ERROR) {
		return M4_ERROR;
	}
	//------------------------------------------

	//Esto no puede ocurrir pq hubiera retornado error, pero nos curamos en salud.
	if (pText == NULL) {
		return M4_ERROR;		
	}

	//------------------------------------------
	//Podemos hacer cast a sin signo pq seguro que no es negativo.
	string oIdSecurizedObject = oClSecurizedObject.GetIdSecurizedObject();
	if (((m4uint_t) iLenText) < oIdSecurizedObject.size()) {
		delete [] pText;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_ERROR;
	}

	//La desencriptacion ha ido bien, ahora verificamos que realmente se trata del objeto que toca!.
	if (strncmp(pText, oIdSecurizedObject.c_str(), oIdSecurizedObject.size()) != 0) {
		//No es el item que me esperaba!.
		delete [] pText;
		ClErrorManager::DumpError( M4MODKEY_ERROR_DECRYPT );
		return M4_FALSE;
	}
	//------------------------------------------

	//El item coincide, por lo tanto podemos pasar el buffer desencriptado!.
	//Tenemos que restar el tamaño del identificador del item!.
	ao_iLenText = iLenText - oIdSecurizedObject.size();
	ao_pText = new m4char_t [ao_iLenText + 1];
	if (ao_pText == NULL) {
		delete [] pText;
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(ao_pText, pText + oIdSecurizedObject.size(), ao_iLenText);
	ao_pText[ao_iLenText] = NULL;

	//Liberamos el buffer que nos devolvio la desencriptacion.
	delete [] pText;
	
	return M4_SUCCESS;
}

m4bool_t ClEncryption::IsEncryptedSecurizedObject( const ClSecurizedObject & oClSecurizedObject, m4pcchar_t ai_pBuffer, m4int32_t ai_iSize )
{
	return _ExistsHeader( ai_pBuffer, ai_iSize );
}

//----------------------------------------------
//FIN Clase ClEncryption
//----------------------------------------------
