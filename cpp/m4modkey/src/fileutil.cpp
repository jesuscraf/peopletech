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

#include "fileutil.hpp"

#include "error.hpp"
#include "m4unicode.hpp"


//----------------------------------------------
//Clase ClFileUtil
//----------------------------------------------
m4return_t ClFileUtil::ReadFile( const m4pchar_t ai_pFileName, m4pchar_t & ao_pFileBuffer, m4int32_t & ao_iSize )
{
	//Reserva el espacio necesario y retorna el puntero y el tamaño del buffer.

	if (ao_pFileBuffer != NULL) {
		//Debe haber algo que leer.
		ClErrorManager::DumpError( M4MODKEY_ERROR_FILE_READING );
		return M4_ERROR;
	}

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	FILE * pFileIn = M4Fopen(ai_pFileName, M4UniReadBinary, eEncoding);	
	m4uint32_t filesize = 0;
	if (pFileIn == 0) {
		//Error abriendo el fichero de lectura.
		ClErrorManager::DumpError( M4MODKEY_ERROR_FILE_READING );
		return M4_ERROR;
	}

	//El tamaño es el número de caracteres del fichero! y nosotros añadimos uno más para el
	//caracter null string.

	fseek( pFileIn, 0, SEEK_END );
	filesize = ftell( pFileIn );
	rewind( pFileIn );
	
	m4pchar_t filebuf = new m4char_t [filesize + 1];
	if (filebuf == NULL) {
		fclose(pFileIn);
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	//Leemos de una tacada el fichero.
	if (fread(filebuf, sizeof(m4char_t), filesize, pFileIn) != filesize) {
		delete [] filebuf;
		fclose(pFileIn);
		ClErrorManager::DumpError( M4MODKEY_ERROR_FILE_READING );		
		return M4_ERROR;
	}

	fclose(pFileIn);
	filebuf[filesize] = NULL;

	//Damos el tamaño del bloque que hemos escrito.
	ao_pFileBuffer = filebuf;	
	ao_iSize = filesize;	

	return M4_SUCCESS;
}

m4return_t ClFileUtil::WriteFile( const m4pchar_t ai_pFileName, const m4pchar_t ai_pFileBuffer, m4int32_t ai_iSize, m4bool_t ai_bReWrite )
{
	//Si no indicamos nombre lo volcamos a salida estandar.

	FILE * pFileOut;
	m4bool_t bStdOut = M4_FALSE;

	if (ai_pFileBuffer == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_FILE_WRITING );
		return M4_ERROR;
	}

	if (ai_pFileName != NULL) {

		//Si existe lo sobreescribimos o damos un error.
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		pFileOut = M4Fopen(ai_pFileName, M4UniRead, eEncoding);
		if (pFileOut != 0) {
			//Ya existe.
			if (ai_bReWrite == M4_FALSE) {
				//El fichero ya existe y no quiero sobreescribirlo.
				fclose(pFileOut);
				ClErrorManager::DumpError( M4MODKEY_ERROR_FILE_WRITING_OVR );				
				return M4_ERROR;
			}
			fclose(pFileOut);
		}

		// UNICODE FILE revised
		eEncoding = M4UniANSI ;
		pFileOut = M4Fopen(ai_pFileName, M4UniWriteBinary, eEncoding);
		if (pFileOut == 0) {
			ClErrorManager::DumpError( M4MODKEY_ERROR_FILE_WRITING );			
			return M4_ERROR;
		}
	}
	else {
		pFileOut= stdout;
		bStdOut = M4_TRUE;
	}

	m4uint32_t filesize = ai_iSize;
	//Escribimos de una tacada el fichero.
	if (fwrite(ai_pFileBuffer, sizeof(m4char_t), filesize, pFileOut) != filesize) {
		if (bStdOut != M4_TRUE) {
			fclose(pFileOut);
		}		
		ClErrorManager::DumpError( M4MODKEY_ERROR_FILE_WRITING );
		return M4_ERROR;
	}

	if (bStdOut != M4_TRUE) {
		fclose(pFileOut);
	}

	return M4_SUCCESS;
}


//----------------------------------------------
//FIN Clase ClFileUtil
//----------------------------------------------
