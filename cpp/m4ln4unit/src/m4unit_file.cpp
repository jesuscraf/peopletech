//	==============================================================================
//
//	 (c) Copyright  1991-2014 Meta4 Spain S.A
//	 All rights reserved.
//
//	 Module:            m4ln4unit		
//	 File:              m4unit_file.cpp
//	 Project:           m4ln4unit
//	 Author:            Meta4 Spain S.A
//	 Date:				160211
//	 Language:          C++
//	 Operating System:  WINDOWS
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================

#include "m4unit_file.hpp"
#include "m4unicode.hpp"

// Auxilary functions
// Search subarray of bytes in array
int SearchBytes(m4pcchar_t ai_sInput, m4pcchar_t ai_sSearchText, size_t length)
{
	for (int i = 0, j = 0; i<length; i++)
	{
		if (ai_sInput[i] == ai_sSearchText[j])
		{
			// Si se alcanza el deliminatar de búsqueda, se ha encontrado la coincidencia.
			if (ai_sSearchText[++j] == '\0') return i;
		}
		else
		{
			j = 0;
		}
	}
	return -1;
}

// PDF binary comparation
m4return_t M4ComparePDF(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4pchar_t		pcFile1, pcFile2;
	m4return_t      tLN4Return;
	FILE*			ptrHandleFile1 = NULL;
	FILE*			ptrHandleFile2 = NULL;
	eUniFileEncodingType_t eEncoding;
	m4pchar_t		pBuffer1 = NULL;
	m4pchar_t		pBuffer2 = NULL;
	size_t			iBlockSize = 32768;
	size_t			iBlockSeekSize = 256;
	size_t			iBlockBeforeSeek = 26;
	size_t			iBlockRead1;
	size_t			iBlockRead2;
	long			filePos1;
	long			filePos2;
	long			seekPos1;
	long			seekPos2;
	int				seekTextPos1;
	int				seekTextPos2;
	int				newSeekTextPos1;
	int				newSeekTextPos2;
	m4pchar_t		pcModificationText = "<</ModDate(\0";
	m4pchar_t		pcInfoText = "<</Info \0";
	m4pchar_t		pcEndBlock = ">>\0";
	m4pchar_t		pcFontProp1 = "/FontName/\0";
	m4pchar_t		pcFontProp2 = "/BaseFont/\0";
	m4pchar_t		pcFontSplit = "+\0";

	// Comprobación de parámetros
	if (( ai_iLongArg != 2) ||
		( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR || NULL == ai_pvArg[0].Data.PointerChar ) ||
		( ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR || NULL == ai_pvArg[1].Data.PointerChar ))
	{
		// Parámetros incorrectos
		return M4_ERROR;
	}

	// Recogemos los valores de la llamada y abrimos los ficheros
	pcFile1 = ai_pvArg[0].Data.PointerChar;
	eEncoding = M4UniANSI ;
	if ( (ptrHandleFile1 = M4Fopen(pcFile1, M4UniReadBinary, eEncoding)) == NULL) 
	{
		ai_pvReturn.SetDouble(-1); 
		return M4_SUCCESS;
	}
	pcFile2 = ai_pvArg[1].Data.PointerChar;
	eEncoding = M4UniANSI ;
	if ( (ptrHandleFile2 = M4Fopen(pcFile2, M4UniReadBinary, eEncoding)) == NULL)
	{
		fclose (ptrHandleFile1);
		ai_pvReturn.SetDouble(-2); 
		return M4_SUCCESS;
	}

	pBuffer1 = new m4char_t[iBlockSize];
	pBuffer2 = new m4char_t[iBlockSize];

	iBlockRead1 = fread(pBuffer1, sizeof(m4char_t), iBlockSize, ptrHandleFile1);
	iBlockRead2 = fread(pBuffer2, sizeof(m4char_t), iBlockSize, ptrHandleFile2);

	tLN4Return = 0;
	while (iBlockRead1 != 0 && iBlockRead2 != 0 && tLN4Return == 0)
	{
		for (size_t i = 0, j = 0; i < iBlockRead1 && j<iBlockRead2; i++, j++)
		{
			if (pBuffer1[i] != pBuffer2[j])
			{
				tLN4Return = 1;
				// Byte diferente. Se analizan bloques de PDFs
				// Se salvaguarda la posición del fichero
				filePos1 = ftell(ptrHandleFile1);
				filePos2 = ftell(ptrHandleFile2);
				seekPos1 = filePos1 - iBlockRead1 + i - iBlockBeforeSeek;
				seekPos2 = filePos2 - iBlockRead2 + j - iBlockBeforeSeek;
				if (seekPos1 >= 0 && seekPos2 >= 0)
				{
					// Leemos bytes anteriores para buscar bloques específicos
					fseek(ptrHandleFile1, seekPos1, SEEK_SET);
					fseek(ptrHandleFile2, seekPos2, SEEK_SET);
					iBlockRead1 = fread(pBuffer1, sizeof(m4char_t), iBlockSeekSize, ptrHandleFile1);
					iBlockRead2 = fread(pBuffer2, sizeof(m4char_t), iBlockSeekSize, ptrHandleFile2);
					// Se busca la primera propiedad de fuente
					seekTextPos1 = SearchBytes(pBuffer1, pcFontProp1, iBlockRead1);
					seekTextPos2 = SearchBytes(pBuffer1, pcFontProp1, iBlockRead2);
					if (seekTextPos1 == -1 && seekTextPos2 == -1)
					{
						// Se busca la segunda propiedad de fuente
						seekTextPos1 = SearchBytes(pBuffer1, pcFontProp2, iBlockRead1);
						seekTextPos2 = SearchBytes(pBuffer1, pcFontProp2, iBlockRead2);
					}
					if (seekTextPos1 > 0 && seekTextPos1 == seekTextPos2)
					{
						// Se desplaza la posición de los ficheros al inicio de los bloques encontrados
						seekPos1 += seekTextPos1;
						seekPos2 += seekTextPos2;
						// Se busca el separador en la propiedad de fuente
						newSeekTextPos1 = SearchBytes(pBuffer1 + seekTextPos1, pcFontSplit, iBlockRead1 - seekTextPos1);
						newSeekTextPos2 = SearchBytes(pBuffer2 + seekTextPos2, pcFontSplit, iBlockRead2 - seekTextPos2);
						// Se ha encontrado propiedad de fuente justo antes de la diferencia (como máximo puede haber 6 bytes más los dos delimitadores)
						if (newSeekTextPos1 > 0 && newSeekTextPos1 == newSeekTextPos2 && newSeekTextPos1 < 8)
						{
							// Se desplaza la posición de los ficheros al final de los bloques encontrados
							// y se sigue comparando
							seekPos1 += newSeekTextPos1;
							seekPos2 += newSeekTextPos2;
							fseek(ptrHandleFile1, seekPos1, SEEK_SET);
							fseek(ptrHandleFile2, seekPos2, SEEK_SET);
							tLN4Return = 0; // La diferencia es en uno de los bloques asumidos
						}
					}
					else
					{
						// Se busca el bloque de creación/modificación
						seekTextPos1 = SearchBytes(pBuffer1, pcModificationText, iBlockRead1);
						seekTextPos2 = SearchBytes(pBuffer2, pcModificationText, iBlockRead2);
						if (seekTextPos1 == -1 && seekTextPos2 == -1) 
						{
							// No es un bloque de fechas de creación/modificación. Se busca el de Info
							seekTextPos1 = SearchBytes(pBuffer1, pcInfoText, iBlockRead1);
							seekTextPos2 = SearchBytes(pBuffer2, pcInfoText, iBlockRead2);
						}
						if (seekTextPos1 > 0 && seekTextPos2 > 0)
						{
							// Se desplaza la posición de los ficheros al inicio de los bloques encontrados
							seekPos1 += seekTextPos1;
							seekPos2 += seekTextPos2;
							// Se busca el final del bloque
							seekTextPos1 = SearchBytes(pBuffer1 + seekTextPos1, pcEndBlock, iBlockRead1 - seekTextPos1);
							seekTextPos2 = SearchBytes(pBuffer2 + seekTextPos2, pcEndBlock, iBlockRead2 - seekTextPos2);
							if (seekTextPos1 > 0 && seekTextPos2 > 0)
							{
								// Se desplaza la posición de los ficheros al final de los bloques encontrados
								// y se sigue comparando
								seekPos1 += seekTextPos1;
								seekPos2 += seekTextPos2;
								fseek(ptrHandleFile1, seekPos1, SEEK_SET);
								fseek(ptrHandleFile2, seekPos2, SEEK_SET);
								tLN4Return = 0; // La diferencia es en uno de los bloques asumidos
							}
						}
					}
				}
				// Si ha habido alguna diferencia se debe interrumpir el bloque actual y procesar, si procede, el nuevo
				break;
			}
		}
		if (tLN4Return == 0)
		{
			iBlockRead1 = fread(pBuffer1, sizeof(m4char_t), iBlockSize, ptrHandleFile1);
			iBlockRead2 = fread(pBuffer2, sizeof(m4char_t), iBlockSize, ptrHandleFile2);
		}
	}

	if (iBlockRead1 != 0 || iBlockRead2 != 0)
	{
		tLN4Return = 1;
	}

	delete [] pBuffer1;
	delete [] pBuffer2;

	fclose (ptrHandleFile1);
	fclose (ptrHandleFile2);

	// los dos valores de retorno el de C++ y el de LN4
	ai_pvReturn.SetDouble(tLN4Return); 
	return M4_SUCCESS;
}

// File binary comparation
m4return_t M4CompareFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4pchar_t		pcFile1, pcFile2;
	m4return_t      tLN4Return;
	FILE*			ptrHandleFile1 = NULL;
	FILE*			ptrHandleFile2 = NULL;
	eUniFileEncodingType_t eEncoding;
	m4pchar_t		pBuffer1 = NULL;
	m4pchar_t		pBuffer2 = NULL;
	size_t			iBlockSize = 32768;
	size_t			iBlockRead1;
	size_t			iBlockRead2;
	long			lFile1Length;
	long			lFile2Length;

	// Comprobación de parámetros
	if (( ai_iLongArg != 2) ||
		( ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR || NULL == ai_pvArg[0].Data.PointerChar ) ||
		( ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR || NULL == ai_pvArg[1].Data.PointerChar ))
	{
		// Parámetros incorrectos
		return M4_ERROR;
	}

	// Recogemos los valores de la llamada y abrimos los ficheros
	pcFile1 = ai_pvArg[0].Data.PointerChar;
	eEncoding = M4UniANSI ;
	if ( (ptrHandleFile1 = M4Fopen(pcFile1, M4UniReadBinary, eEncoding)) == NULL) 
	{
		ai_pvReturn.SetDouble(-1); 
		return M4_SUCCESS;
	}
	pcFile2 = ai_pvArg[1].Data.PointerChar;
	eEncoding = M4UniANSI ;
	if ( (ptrHandleFile2 = M4Fopen(pcFile2, M4UniReadBinary, eEncoding)) == NULL)
	{
		fclose (ptrHandleFile1);
		ai_pvReturn.SetDouble(-2); 
		return M4_SUCCESS;
	}

	fseek(ptrHandleFile1, 0, SEEK_END);
	lFile1Length = ftell(ptrHandleFile1);
	fseek(ptrHandleFile1, 0, SEEK_SET);

	fseek(ptrHandleFile2, 0, SEEK_END);
	lFile2Length = ftell(ptrHandleFile2);
	fseek(ptrHandleFile2, 0, SEEK_SET);

	if (lFile1Length == lFile2Length)
	{
		pBuffer1 = new m4char_t[iBlockSize];
		pBuffer2 = new m4char_t[iBlockSize];

		iBlockRead1 = fread(pBuffer1, sizeof(m4char_t), iBlockSize, ptrHandleFile1);
		iBlockRead2 = fread(pBuffer2, sizeof(m4char_t), iBlockSize, ptrHandleFile2);

		tLN4Return = 0;
		while (iBlockRead1 != 0 && tLN4Return == 0)
		{
			if (iBlockRead1 != iBlockRead2)
			{
				// Nunca debería entrar por aquí. Los ficheros se han comprobado que eran iguales
				// y ahora descuadran los bloques !!
				tLN4Return = -3;
			}
			else if (memcmp(pBuffer1, pBuffer2, iBlockRead1) != 0)
			{
				tLN4Return =1;
			}
			else
			{
				iBlockRead1 = fread(pBuffer1, sizeof(m4char_t), iBlockSize, ptrHandleFile1);
				iBlockRead2 = fread(pBuffer2, sizeof(m4char_t), iBlockSize, ptrHandleFile2);
			}
		}

		delete [] pBuffer1;
		delete [] pBuffer2;
	}
	else
	{
		tLN4Return = 1;
	}
	fclose (ptrHandleFile1);
	fclose (ptrHandleFile2);

	// los dos valores de retorno el de C++ y el de LN4
	ai_pvReturn.SetDouble(tLN4Return); 
	return M4_SUCCESS;
}