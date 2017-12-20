//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkeyutil
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

#include "arguments.hpp"
#include "modkeyutil.hpp"

#include "fileutil.hpp"

#include "xmlanalyzerinterface.hpp"
#include "encrypt.hpp"

#include "licenseproperties.hpp"

#include "key.hpp"

#include "m4string.hpp"
#include "m4unicode.hpp"


//Este constructor es valido para la linea de comandos.
ClOptionArguments::ClOptionArguments( m4int32_t argc, m4char_t **argv )
{
	//Es un vector de punteros a cadenas.
	m_iArgc = argc;
	m_pArgv = new m4char_t * [argc];
	if (m_pArgv == NULL) {
		return;
	}
	for (m4int32_t i = 0; i < argc; i++) {
		m4char_t * pC = new m4char_t[strlen(argv[i]) + 1];
		if (pC == NULL) {
			return;
		}
		strcpy(pC, argv[i]);
		m_pArgv[i] = pC;
	}

	m_pVcOptionArgumentsString = new VcOptionArgumentsString;
	_FillOptionArguments( m_iArgc, m_pArgv );

	m_pXmlAnalyzerInput = NULL;
	m_pXmlAnalyzerLicenseInfo = NULL;

	//Inicializamos miembros que pueden resetearse.
	_Reset();
}

ClOptionArguments::ClOptionArguments( const string & ai_sOptionArguments )
{
	m_pVcOptionArgumentsString = new VcOptionArgumentsString;
	_ParseOptionArgumentsString( ai_sOptionArguments, m_pVcOptionArgumentsString );

	_FillOptionArguments( m_pVcOptionArgumentsString );

	m_pXmlAnalyzerInput = NULL;
	m_pXmlAnalyzerLicenseInfo = NULL;

	//Inicializamos miembros que pueden resetearse.
	_Reset();
}
	
ClOptionArguments::~ClOptionArguments( )
{
	for (m4int32_t i = 0; i < m_iArgc; i++) {
		delete [] (m_pArgv[i]);
		m_pArgv[i] = NULL;
	}
	delete [] m_pArgv;	
	m_pArgv = NULL;

	m_iArgc = 0;

	if (m_pVcOptionArgumentsString != NULL) {
		delete m_pVcOptionArgumentsString;
		m_pVcOptionArgumentsString = NULL;
	}

	m_bReWrite = M4_FALSE;

	m_eSwitchType = SWITCH_TYPE_UNDEFINED;
	m_eObjectType = OBJECT_TYPE_UNDEFINED;
	m_eOperationType = OPERATION_TYPE_UNDEFINED;

	//Importante, somos responsables de eliminar los buffers que se han podido crear
	//para obtener el contenido del fichero.
	if (m_pTextBuffer != NULL) {
		delete [] m_pTextBuffer;
		m_pTextBuffer = NULL;
	}
	
	if (m_pEncryptedBuffer != NULL) {
		delete [] m_pEncryptedBuffer;
		m_pEncryptedBuffer = NULL;
	}

	if (m_pLicenseInfoBuffer != NULL) {
		delete [] m_pLicenseInfoBuffer;
		m_pLicenseInfoBuffer = NULL;
	}	
	
	m_iTextSize = 0;
	m_iEncryptedSize = 0;
	m_iLicenseInfoSize = 0;

	//El nombre de los ficheros nos ha venido, no liberamos nada.
	m_pInputFileName = NULL;
	m_pInputFileNameLicenseInfo = NULL;
	m_pOutputFileName = NULL;
	m_pVerboseFileName = NULL;

	if (m_pXmlAnalyzerInput != NULL) {
		ClXmlAnalyzerFactory::DestroyInstanceXmlAnalyzer(m_pXmlAnalyzerInput);
		m_pXmlAnalyzerInput = NULL;
	}
	if (m_pXmlAnalyzerLicenseInfo != NULL) {
		ClXmlAnalyzerFactory::DestroyInstanceXmlAnalyzer(m_pXmlAnalyzerLicenseInfo);
		m_pXmlAnalyzerLicenseInfo = NULL;
	}
}

m4return_t ClOptionArguments::_InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey )
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
		printf("\nSending key error.\n");
		delete [] pKeyA;
		delete [] pSignature;
		return M4_ERROR;
	}

	//Verificamos que la fecha es correcta.
	if (memcmp(&dNow, pSignature, sizeof(dNow)) != 0) {
		printf("\nVerifying key error.\n");
		delete [] pKeyA;
		delete [] pSignature;
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

m4return_t ClOptionArguments::_ParseOptionArgumentsString( const string & ai_sOptionArguments, VcOptionArgumentsString * ai_pVcOptionArgumentsString )
{
	m4int32_t iTokens = 0;
	string sToken("");
	m4int32_t iSize = ai_sOptionArguments.size();	
	m4int32_t iCurrent = -1;
	m4bool_t bDoubleQuotesOpened = M4_FALSE;

	if ((iSize == 0) || (ai_pVcOptionArgumentsString == NULL)) {
		return M4_SUCCESS;
	}

	do
	{				
		iCurrent++;

		//----------------------------------------------------
		//Si no he llegado al final todavia tiene sentido comprobar.
		if (iCurrent != iSize) {

			//Si nos encontramos dentro de dobles comillas.
			if (bDoubleQuotesOpened == M4_TRUE) {
				while (iCurrent <= iSize)
				{
					if (ai_sOptionArguments.compare(iCurrent, 1, "\"") == 0) {
						sToken.append(ai_sOptionArguments.substr(iCurrent, 1));					
						continue;
					}
					else {
						//Hemos llegado al final del token comillado.
						bDoubleQuotesOpened = (bDoubleQuotesOpened == M4_TRUE) ? M4_FALSE: M4_TRUE;					
						break;
					}
				}

				//Si estamos aqui es pq hemos llegado al fin de token comillado o al final de
				//la cadena. De cualquier manera tenemos un token.
			}
			else {
				//No es un token comillado.
				//Si no es un blanco continuamos iterando.
				if (ai_sOptionArguments.compare(iCurrent, 1, " ") != 0) {				
					//No es un blanco.
					sToken.append(ai_sOptionArguments.substr(iCurrent, 1));			
					continue;
				}
			}

		} //if (iCurrent == iSize) {
		//----------------------------------------------------

		iTokens++;

		ai_pVcOptionArgumentsString->push_back(sToken);

		//Limpiamos y continuaremos. Erase esta dejando un error en boundchecker.		
		//sToken.erase();
		sToken.assign(M4MODKEY_EMPTY);

	} 	while (iCurrent < iSize);


	return M4_SUCCESS;
}

void ClOptionArguments::_FillOptionArguments( VcOptionArgumentsString * ai_pVcOptionArgumentsString )
{
	if (ai_pVcOptionArgumentsString == NULL) {
		m_iArgc = 0;
		m_pArgv = NULL;
		return ;
	}

	m_iArgc = ai_pVcOptionArgumentsString->size();
	m_pArgv = new m4char_t * [m_iArgc];
	if (m_pArgv == NULL) {
		return;
	}
	m4int32_t i = 0;
	
	for (itVcOptionArgumentsString it = ai_pVcOptionArgumentsString->begin(); it != ai_pVcOptionArgumentsString->end() ; it++) {
		string s = (*it);
		m4char_t * pC = new m4char_t[s.size() + 1];	
		if (pC == NULL) {
			return;
		}
		strcpy(pC, s.c_str());
		m_pArgv[i++] = pC;
	}
}

void ClOptionArguments::_FillOptionArguments( m4int32_t ai_iArgc, m4char_t **ai_ppArgv )
{

	if (m_pVcOptionArgumentsString == NULL) {
		return;
	}

	for (m4int32_t i = 0; i < ai_iArgc; i++) {		
		m_pVcOptionArgumentsString->push_back(ai_ppArgv[i]);
	}
}

void ClOptionArguments::_Reset( void )
{
	//Iniciliza los miembros que hayan podido establecerse en el parseo y que pueden resetearse.

	//---------------------------------------------------	
	m_ePermissionMask = PERMISSION_OPERATION_ENCRYPT;

	m_bReWrite = M4_FALSE;

	m_eSwitchType = SWITCH_TYPE_UNDEFINED;
	m_eObjectType = OBJECT_TYPE_UNDEFINED;
	m_eOperationType = OPERATION_TYPE_UNDEFINED;

	//Tendremos un buffer desencriptado (texto) y otro con la misma informacion encriptada.
	m_pTextBuffer = NULL;
	m_pEncryptedBuffer = NULL;
	m_pLicenseInfoBuffer = NULL;

	m_iTextSize = 0;
	m_iEncryptedSize = 0;
	m_iLicenseInfoSize = 0;

	//Tenemos el nombre de los ficheros a leer.
	m_pInputFileName = NULL;
	m_pInputFileNameLicenseInfo = NULL;
	m_pOutputFileName = NULL;

	//Fichero de informacion.
	m_pVerboseFileName = NULL;
	//---------------------------------------------------
}

m4bool_t ClOptionArguments::_IsInitilized( void )
{
	if (m_eObjectType == OBJECT_TYPE_UNDEFINED) {
		return M4_FALSE;
	}

	return M4_TRUE;
}

void ClOptionArguments::DumpUsage( void )
{
#ifdef _M4MODKEYUTIL_SECURITY_LICENSE_INFO
	_DumpUsageInfoVersion();
#else
	_DumpUsageCompleteVersion();
#endif
}

void ClOptionArguments::_DumpUsageInfoVersion( void )
{
	const m4char_t pTab[] = "   ";

	printf("Usage:\n");
	printf("%s ", m_pArgv[0]);
	printf("<-Switch> <InputFileOptions> [OuputFileOptions]\n");

	printf("%s<-Switch>\n", pTab);
		printf("%s%s<%s | %s>\n", pTab, pTab, ARG_OPTION_INFO, ARG_OPTION_INFO_SUMMARY);
			printf("%s%s%s* Dump License Information: Input is an encrypted License file.\n", pTab, pTab, pTab);		
			printf("%s%s%s* Dump License Information summarized: Input is an encrypted file (reduced information).\n", pTab, pTab, pTab);
	
	printf("%s<InputFileOptions>\n", pTab);
		printf("%s%s<%s <InputFile>>\n", pTab, pTab, ARG_OPTION_SOURCE_FILE);
			printf("%s%s%s* Encrypted license file.\n", pTab, pTab, pTab);

	printf("%s[OuputFileOptions]\n", pTab);		
		printf("%s%s<%s [%s] <OutputFile>>\n", pTab, pTab, ARG_OPTION_DESTINATION_FILE, ARG_OPTION_REWRITE_DESTINATION_FILE);
			printf("%s%s%s* Optional rewrite destination file.\n", pTab, pTab, pTab);
			printf("%s%s%s* License information.\n", pTab, pTab, pTab);

	printf("\n");

		printf("%s%s%s\n", pTab, pTab, "m4modkeyutil -I -i lic_x.txt -o -w info.txt");			
			printf("%s%s%s%s\n", pTab, pTab, pTab, "Dump License information from encrypted license file.");

		printf("\n");

		printf("%s%s%s\n", pTab, pTab, "m4modkeyutil -S -i summary_xxx.txt");			
			printf("%s%s%s%s\n", pTab, pTab, pTab, "Dump License Summary to stdout. Input is an encrypted file.");
}

void ClOptionArguments::_DumpUsageCompleteVersion( void )
{
	const m4char_t pTab[] = "   ";

	printf("Usage:\n");
	printf("%s ", m_pArgv[0]);
	printf("<-Switch> [-Operation] <InputFileOptions> <OuputFileOptions> [-Verbose]\n");

	printf("%s<-Switch>\n", pTab);
		printf("%s%s<%s | %s | %s | %s | %s] >\n", pTab, pTab, ARG_OPTION_OBJECTS, ARG_OPTION_LICENSE, ARG_OPTION_GENERATION, ARG_OPTION_INFO, ARG_OPTION_INFO_SUMMARY);
			printf("%s%s%s* Object-Module: Encrypt or decrypt an Object-Module file.\n", pTab, pTab, pTab);
			printf("%s%s%s* License: Encrypt or decrypt a License file.\n", pTab, pTab, pTab);
			printf("%s%s%s* License Generation: Generate a License file.\n", pTab, pTab, pTab);
			printf("%s%s%s  Object-Module and License Information files required (decrypted).\n", pTab, pTab, pTab);
			printf("%s%s%s* Dump License Information: Input is an encrypted License file.\n", pTab, pTab, pTab);		
			printf("%s%s%s* Dump License Information summarized: Input is an encrypted file (reduced information).\n", pTab, pTab, pTab);
	
	printf("%s[-Operation]\n", pTab);
		printf("%s%s<%s | %s>\n", pTab, pTab, ARG_OPTION_ENCRYPTION, ARG_OPTION_DECRYPTION);			
			printf("%s%s%s  Do not use with Dump Licence Information.\n",pTab, pTab, pTab);
			printf("%s%s%s* Encryption mode.\n",pTab, pTab, pTab);
			printf("%s%s%s* Decryption mode: Only private use!.\n", pTab, pTab, pTab);	 			

	printf("%s<InputFileOptions>\n", pTab);
		printf("%s%s<%s <InputFile> | %s <Object-Module file> %s <License Information file>>\n", pTab, pTab, ARG_OPTION_SOURCE_FILE, ARG_OPTION_SOURCE_FILE, ARG_OPTION_SOURCE_FILE);
			printf("%s%s%s* Input file can be encrypted or decrypted.\n", pTab, pTab, pTab);
			printf("%s%s%s  Input file can be Object-Module file, License file \n", pTab, pTab, pTab);
			printf("%s%s%s  or License encrypted file (dump info).\n", pTab, pTab, pTab);
			printf("%s%s%s* In License generation. Object-Module and License Information\n", pTab, pTab, pTab);
			printf("%s%s%s  must be decrypted.\n", pTab, pTab, pTab);

	printf("%s<OuputFileOptions>\n", pTab);		
		printf("%s%s<%s [%s] <OutputFile>>\n", pTab, pTab, ARG_OPTION_DESTINATION_FILE, ARG_OPTION_REWRITE_DESTINATION_FILE);
			printf("%s%s%s* Optional rewrite destination file.\n", pTab, pTab, pTab);
			printf("%s%s%s* Destination file could be encrypted or decrypted.\n", pTab, pTab, pTab);

	printf("%s[-Verbose]\n", pTab);
		printf("%s%s[<%s> [OutputFile]]\n", pTab, pTab, ARG_OPTION_VERBOSE);
			printf("%s%s%s* Dump information to file.\n", pTab, pTab, pTab);
			printf("%s%s%s  Optional OutputFile. (stdout by default).\n", pTab, pTab, pTab);	

	printf("\n");

	printf("%s-- Examples --\n", pTab);
		printf("%s%s%s\n", pTab, pTab, "m4modkeyutil -G -E -i modules.xml -i licinf.xml -o -w lic_x.txt -v verb.txt");
			printf("%s%s%s%s\n", pTab, pTab, pTab, "Generates file License lic_x.txt encrypted (overwrite file).");

		printf("\n");

		printf("%s%s%s\n", pTab, pTab, "m4modkeyutil -I -i lic_x.txt -o -w info.txt");			
			printf("%s%s%s%s\n", pTab, pTab, pTab, "Dump License information from encrypted license file.");

		printf("\n");

		printf("%s%s%s\n", pTab, pTab, "m4modkeyutil -S -i summary_xxx.txt");			
			printf("%s%s%s%s\n", pTab, pTab, pTab, "Dump License Summary to stdout. Input is an encrypted file.");
}

void ClOptionArguments::DumpInfo( m4char_t * ai_pFileName )
{
	FILE * pFile = NULL;

	//Si no esta inicilizado no hacemos nada.
	if (_IsInitilized() == M4_FALSE) {
		printf("Option arguments not initialized.\n");
		return;
	}

	if (ai_pFileName == NULL) {
		_DumpInfo(stdout, 0);		
	}
	else {
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		pFile = M4Fopen(ai_pFileName, M4UniWrite, eEncoding);
		if (pFile != NULL) {
			_DumpInfo(pFile, 0);
			fclose(pFile);
		}
	}
}

void ClOptionArguments::_DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}

	//Si no han inicializado nos vamos.
	if (_IsInitilized() == M4_FALSE) {
		return;
	}

	string sTab("");	
	for (m4int32_t i = 0; i<= ai_iLevel; i++) {
		sTab.append("\t");
	}

	fprintf(ai_pFile, "%s *** Begin - ClOptionArguments ***\n", sTab.c_str());

	fprintf(ai_pFile, "%s m_ePermissionMask: %i\n", sTab.c_str(), m_ePermissionMask);
	fprintf(ai_pFile, "%s m_eSwitchType: %i\n", sTab.c_str(), m_eSwitchType);	
	fprintf(ai_pFile, "%s m_eObjectType: %i\n", sTab.c_str(), m_eObjectType);
	fprintf(ai_pFile, "%s m_eOperationType: %i\n", sTab.c_str(), m_eOperationType);

	//----------------------------------------
	if (m_pInputFileName != NULL) {
		fprintf(ai_pFile, "%s m_pInputFileName: %s\n", sTab.c_str(), m_pInputFileName);
	}
	if (m_pInputFileNameLicenseInfo != NULL) {
		fprintf(ai_pFile, "%s m_pInputFileNameLicenseInfo: %s\n", sTab.c_str(), m_pInputFileNameLicenseInfo);
	}
	if (m_pOutputFileName != NULL) {
		fprintf(ai_pFile, "%s m_pOutputFileName: %s\n", sTab.c_str(), m_pOutputFileName);
	}
	//----------------------------------------

	//Solo volcamos si tenemos permiso para verlo.
	//----------------------------------------
	if (_PermissionInOperation(OPERATION_TYPE_DECRYPTION) == M4_TRUE) {		
		fprintf(ai_pFile, "%s m_iTextSize: %i\n", sTab.c_str(), m_iTextSize);
		if (m_pTextBuffer != NULL) {
			fprintf(ai_pFile, "%s----- Begin m_pTextBuffer -----\n", sTab.c_str());
			//El buffer es binario, por lo tanto tendriamos que reabrirlo en modo binario.
			fprintf(ai_pFile, "%s %s\n", sTab.c_str(), m_pTextBuffer);	
			fprintf(ai_pFile, "%s----- End m_pTextBuffer -----\n", sTab.c_str());
		}
	}
	else {
		fprintf(ai_pFile, "%s m_iTextSize: %s\n", sTab.c_str(), "****** no privileges ******");
		fprintf(ai_pFile, "%s m_pTextBuffer: %s\n", sTab.c_str(), "****** no privileges ******");		
	}
	//----------------------------------------

	//----------------------------------------
	//Solo tiene sentido ver esto si estamos generando la licencia, independientemente de la encriptacion
	//o no del fichero destino (la licencia).
	if (m_eSwitchType == SWITCH_TYPE_GENERATION) {		
		fprintf(ai_pFile, "%s m_iLicenseInfoSize: %i\n", sTab.c_str(), m_iLicenseInfoSize);
		if (m_pLicenseInfoBuffer != NULL) {
			fprintf(ai_pFile, "%s----- Begin m_pLicenseInfoBuffer -----\n", sTab.c_str());
			//El buffer es binario, por lo tanto tendriamos que reabrirlo en modo binario.
			fprintf(ai_pFile, "%s %s\n", sTab.c_str(), m_pLicenseInfoBuffer);
			fprintf(ai_pFile, "%s----- End m_pLicenseInfoBuffer -----\n", sTab.c_str());
		}	
	}
	//----------------------------------------

	//----------------------------------------	
	if (_PermissionInOperation(OPERATION_TYPE_ENCRYPTION) == M4_TRUE) {		
		fprintf(ai_pFile, "%s m_iEncryptedSize: %i\n", sTab.c_str(), m_iEncryptedSize);
		if (m_pEncryptedBuffer != NULL) {
			fprintf(ai_pFile, "%s----- Begin m_pEncryptedBuffer -----\n", sTab.c_str());
			//El buffer es binario, por lo tanto tendriamos que reabrirlo en modo binario.
			fprintf(ai_pFile, "%s %s\n", sTab.c_str(), m_pEncryptedBuffer);
			fprintf(ai_pFile, "%s----- End m_pEncryptedBuffer -----\n", sTab.c_str());
		}	
	}
	else {
		fprintf(ai_pFile, "%s m_iEncryptedSize: %s\n", sTab.c_str(), "****** no privileges ******");
		fprintf(ai_pFile, "%s m_pEncryptedBuffer: %s\n", sTab.c_str(), "****** no privileges ******");
	}
	//----------------------------------------

	if (m_pXmlAnalyzerInput != NULL) {
		m_pXmlAnalyzerInput->DumpInfo(ai_pFile, ai_iLevel + 1);
	}
	if (m_pXmlAnalyzerLicenseInfo != NULL) {
		m_pXmlAnalyzerLicenseInfo->DumpInfo(ai_pFile, ai_iLevel + 1);
	}

	fprintf(ai_pFile, "%s *** End - ClOptionArguments ***\n", sTab.c_str());

	fflush(ai_pFile);
}

m4return_t ClOptionArguments::StartParse( void )
{
	//Si ya está inicializada no podemos volver a parsear.
	if (_IsInitilized() == M4_TRUE) {
		printf("\nOnly one parsing allowed.\n");
		return M4_ERROR;
	}

	if (_StartParse() == M4_ERROR) {
		printf("\nExecution with error.\n");
		return M4_ERROR;
	}

	//Si no es nulo es pq queremos volcar informacion!.
	if (m_pVerboseFileName != NULL) {
		DumpInfo(m_pVerboseFileName);
	}

	printf("\nExecution successfully.\n");
	return M4_SUCCESS;
}

m4return_t ClOptionArguments::_StartParse( void )
{
	//Vamos a analizar qué opciones introduce el usuario.
		
	//El primer argumento es el propio ejecutable.
	m4int32_t iCurrentArg = 0;
	while (iCurrentArg < m_iArgc)
    {
		iCurrentArg++;

		//Cada vez que incremento debere comprobarlo.
		if (iCurrentArg >= m_iArgc) {
			break;
		}

		//----------------------------------------------------------------------
		//VERBOSE
		//----------------------------------------------------------------------
		if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_VERBOSE) == 0) 
		{
			if (_IsSwitchAllowed(ARG_OPTION_VERBOSE) == M4_FALSE) {
				printf("Incorrect operation.\n");
				return M4_ERROR;
			}

			iCurrentArg ++;
			if (iCurrentArg == m_iArgc) {
				printf("Dump information to stdout.\n");
				continue;
			}
			m_pVerboseFileName = m_pArgv[iCurrentArg];
			//Al continuar no incrementamos el iCurrentArg.
			continue;
		}
		//----------------------------------------------------------------------
		//FIN VERBOSE
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//INFO o SUMMARY (resumen)
		//----------------------------------------------------------------------
		if ((strcmp(m_pArgv[iCurrentArg], ARG_OPTION_INFO) == 0) 
			|| (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_INFO_SUMMARY) == 0)) 
		{
			if (_IsSwitchAllowed(ARG_OPTION_INFO) == M4_FALSE) {
				printf("Incorrect operation.\n");
				return M4_ERROR;
			}

			if (m_eSwitchType != OBJECT_TYPE_UNDEFINED) {
				printf("Switch '%s' not allowed. Another option has been set.\n", m_pArgv[iCurrentArg]);
				return M4_ERROR;
			}

			if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_INFO) == 0) {
				m_eSwitchType = SWITCH_TYPE_INFO;
			}
			else {
				m_eSwitchType = SWITCH_TYPE_INFO_SUMMARY;
			}

			iCurrentArg ++;
			if (iCurrentArg == m_iArgc) {
				printf("Missing switch '%s'.\n", ARG_OPTION_SOURCE_FILE);
				return M4_ERROR;
			}

			//El tipo de objeto es de licencia, solo se ve informacion de licencia!.
			m_eObjectType = OBJECT_TYPE_LICENSE;

			//El siguiente parametro tiene que ser el fichero de entrada!.
			//Primero se comprueba que pasamos el fichero source y despues incremento el argumento.
			if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_SOURCE_FILE) == 0) {				

				iCurrentArg ++;
				if (iCurrentArg == m_iArgc) {
					printf("Missing license file name.\n");
					return M4_ERROR;
				}
				//Obtenemos el fichero de entrada.
				m_pInputFileName = m_pArgv[iCurrentArg];

				iCurrentArg ++;
				if (iCurrentArg == m_iArgc) {
					m_pOutputFileName = NULL;
					break;
				}
				else {
					//--- Especificamos fichero de salida.
					if (strcmpi(m_pArgv[iCurrentArg], ARG_OPTION_DESTINATION_FILE) == 0) {
						iCurrentArg ++;
						if (iCurrentArg == m_iArgc) {
							printf("Missing output license information filename.\n");
							return M4_ERROR;
						}

						//Comprobamos si hay que sobreescribir el fichero de salida.
						if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_REWRITE_DESTINATION_FILE) == 0) {
								m_bReWrite = M4_TRUE;
								iCurrentArg++;
						}
						else {
							//Si empieza por el guion es que estamos poniendo una opcion no valida.
							if (strncmp(m_pArgv[iCurrentArg], "-", 1) == 0) {
								printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
								return M4_ERROR;
							}
						}
						if (iCurrentArg == m_iArgc) {
							printf("Missing output license information filename.\n");
							return M4_ERROR;
						}

						//Obtenemos el fichero de salida.
						m_pOutputFileName = m_pArgv[iCurrentArg];					

						//Continuamos con el siguiente argumento.
						continue;
					}
					else {
						printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
						return M4_ERROR;
					}
				}
				//--- FIN Especificamos fichero de salida.
			}
			else {
				printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
				return M4_ERROR;
			}
		}
		//----------------------------------------------------------------------
		//FIN INFO o SUMMARY (resumen)
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//OBJECTOS-MODULO
		//----------------------------------------------------------------------
        if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_OBJECTS) == 0) 
		{
			if (_IsSwitchAllowed(ARG_OPTION_OBJECTS) == M4_FALSE) {
				printf("Incorrect operation.\n");
				return M4_ERROR;
			}
			
			if (m_eSwitchType != OBJECT_TYPE_UNDEFINED) {
				printf("Switch '%s' not allowed. Another option has been set.\n", m_pArgv[iCurrentArg]);
				return M4_ERROR;
			}
			m_eSwitchType = SWITCH_TYPE_OBJECTS;

			iCurrentArg ++;
			if (iCurrentArg == m_iArgc) {
				printf("Missing switch '%s' or '%s'.\n", ARG_OPTION_ENCRYPTION, ARG_OPTION_DECRYPTION);
				return M4_ERROR;
			}
            m_eObjectType = OBJECT_TYPE_MODULE;

			//El siguiente parametro tiene que indicarme si es encriptacion o desencriptacion.
			if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_ENCRYPTION) == 0) {
					m_eOperationType = OPERATION_TYPE_ENCRYPTION;
					if (_PermissionInOperation(OPERATION_TYPE_ENCRYPTION) == M4_FALSE) {
						printf("Insufficient permissions in encryption operation.\n");
						return M4_ERROR;
					}
			}
			else {
				if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_DECRYPTION) == 0) {
					m_eOperationType = OPERATION_TYPE_DECRYPTION;
					if (_PermissionInOperation(OPERATION_TYPE_DECRYPTION) == M4_FALSE) {
						printf("Insufficient permissions in decryption operation.\n");
						return M4_ERROR;
					}
				}
				else {
					printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
					return M4_ERROR;
				}
			}
			iCurrentArg ++;
			if (iCurrentArg == m_iArgc) {
				printf("Missing switch '%s'.\n", ARG_OPTION_SOURCE_FILE);
				return M4_ERROR;
			}

			//El siguiente parametro tiene que ser el fichero de entrada!.
			//Primero se comprueba que pasamos el fichero source y despues incremento el argumento.
			if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_SOURCE_FILE) == 0) {				

				iCurrentArg ++;
				if (iCurrentArg == m_iArgc) {
					printf("Missing input file name.\n");
					return M4_ERROR;
				}
				//Obtenemos el fichero de entrada.
				m_pInputFileName = m_pArgv[iCurrentArg];

				iCurrentArg ++;
				if (iCurrentArg == m_iArgc) {
					printf("Missing switch '%s'.\n", ARG_OPTION_DESTINATION_FILE);
					return M4_ERROR;
				}

				if (strcmpi(m_pArgv[iCurrentArg], ARG_OPTION_DESTINATION_FILE) == 0) {
					iCurrentArg ++;
					if (iCurrentArg == m_iArgc) {
						printf("Missing output filename.\n");
						return M4_ERROR;
					}

					//Comprobamos si hay que sobreescribir el fichero de salida.
					if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_REWRITE_DESTINATION_FILE) == 0) {
							m_bReWrite = M4_TRUE;
							iCurrentArg++;
					}
					else {
						//Si empieza por el guion es que estamos poniendo una opcion no valida.
						if (strncmp(m_pArgv[iCurrentArg], "-", 1) == 0) {
							printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
							return M4_ERROR;
						}
					}
					if (iCurrentArg == m_iArgc) {
						printf("Missing output filename.\n");
						return M4_ERROR;
					}

					//Obtenemos el fichero de salida.
					m_pOutputFileName = m_pArgv[iCurrentArg];					

					if (m_eOperationType == OPERATION_TYPE_ENCRYPTION) {
						printf("Encrypting objects information.\n");
					}
					else {
						printf("Decrypting objects information.\n");
					}

					//Nos vamos a ejecutar pero iteramos otra vez por si hay mas opciones.
					//Al continuar no incrementamos el iCurrentArg.
					continue;
				}
				else {
					printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
					return M4_ERROR;
				}
			}
			else {
				printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
				return M4_ERROR;
			}
		}
		//----------------------------------------------------------------------
		//FIN OBJECTOS-MODULO
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//LICENCIA
		//----------------------------------------------------------------------
        if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_LICENSE) == 0) 
		{
			if (_IsSwitchAllowed(ARG_OPTION_LICENSE) == M4_FALSE) {
				printf("Incorrect operation.\n");
				return M4_ERROR;
			}


			if (m_eSwitchType != OBJECT_TYPE_UNDEFINED) {
				printf("Switch '%s' not allowed. Another option has been set.\n", m_pArgv[iCurrentArg]);
				return M4_ERROR;
			}
			m_eSwitchType = SWITCH_TYPE_LICENSE;

			iCurrentArg ++;
			if (iCurrentArg == m_iArgc) {
				printf("Missing switch '%s' | '%s'.\n", ARG_OPTION_ENCRYPTION, ARG_OPTION_DECRYPTION);
				return M4_ERROR;
			}
            m_eObjectType = OBJECT_TYPE_LICENSE;

			//El siguiente parametro tiene que indicarme si es encriptacion o desencriptacion.
			if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_ENCRYPTION) == 0) {
					m_eOperationType = OPERATION_TYPE_ENCRYPTION;
					if (_PermissionInOperation(OPERATION_TYPE_ENCRYPTION) == M4_FALSE) {
						printf("Insufficient permissions in encryption operation.\n");
						return M4_ERROR;
					}
			}
			else {
				if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_DECRYPTION) == 0) {
					m_eOperationType = OPERATION_TYPE_DECRYPTION;
					if (_PermissionInOperation(OPERATION_TYPE_DECRYPTION) == M4_FALSE) {
						printf("Insufficient permissions in decryption operation.\n");
						return M4_ERROR;
					}
				}
				else {
					printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
					return M4_ERROR;
				}
			}
			iCurrentArg ++;
			if (iCurrentArg == m_iArgc) {
				printf("Missing switch '%s'.\n", ARG_OPTION_SOURCE_FILE);
				return M4_ERROR;
			}

			//El siguiente parametro tiene que ser el fichero de entrada!.
			//Primero se comprueba que pasamos el fichero source y despues incremento el argumento.
			if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_SOURCE_FILE) == 0) {
				iCurrentArg ++;
				if (iCurrentArg == m_iArgc) {
					printf("Missing input filename.\n");
					return M4_ERROR;
				}
				//Obtenemos el fichero de entrada.
				m_pInputFileName = m_pArgv[iCurrentArg];

				iCurrentArg ++;
				if (iCurrentArg == m_iArgc) {
					printf("Missing switch '%s'.\n", ARG_OPTION_DESTINATION_FILE);
					return M4_ERROR;
				}

				if (strcmpi(m_pArgv[iCurrentArg], ARG_OPTION_DESTINATION_FILE) == 0) {
					iCurrentArg ++;
					if (iCurrentArg == m_iArgc) {
						printf("Missing output filename.\n");
						return M4_ERROR;
					}

					//Comprobamos si hay que sobreescribir el fichero de salida.
					if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_REWRITE_DESTINATION_FILE) == 0) {
							m_bReWrite = M4_TRUE;
							iCurrentArg++;
					}
					else {
						//Si empieza por el guion es que estamos poniendo una opcion no valida.
						if (strncmp(m_pArgv[iCurrentArg], "-", 1) == 0) {
							printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
							return M4_ERROR;
						}
					}
					if (iCurrentArg == m_iArgc) {
						printf("Missing output filename.\n");
						return M4_ERROR;
					}

					//Obtenemos el fichero de salida.
					m_pOutputFileName = m_pArgv[iCurrentArg];

					if (m_eOperationType == OPERATION_TYPE_ENCRYPTION) {
						printf("Encrypting License information.\n");
					}
					else {
						printf("Decrypting License information.\n");
					}

					//Nos vamos a ejecutar pero iteramos otra vez por si hay mas opciones.
					//Al continuar no incrementamos el iCurrentArg.
					continue;

				}
				else {
					printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
					return M4_ERROR;
				}
			}
			else {
				printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
				return M4_ERROR;
			}
		}
		//----------------------------------------------------------------------
		//FIN LICENCIA
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//GENERACION DE LICENCIA
		//----------------------------------------------------------------------
        if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_GENERATION) == 0) 
		{
			if (_IsSwitchAllowed(ARG_OPTION_GENERATION) == M4_FALSE) {
				printf("Incorrect operation.\n");
				return M4_ERROR;
			}

			if (m_eSwitchType != OBJECT_TYPE_UNDEFINED) {
				printf("Switch '%s' not allowed. Another option has been set.\n", m_pArgv[iCurrentArg]);
				return M4_ERROR;
			}
			m_eSwitchType = SWITCH_TYPE_GENERATION;

			iCurrentArg ++;
			if (iCurrentArg == m_iArgc) {
				printf("Missing switch '%s' | '%s'.\n", ARG_OPTION_ENCRYPTION, ARG_OPTION_DECRYPTION);
				return M4_ERROR;
			}

			//Lo igualamos a modulo porque es el primer fichero que se leera, pero el que manda es el flag de generacion
			//de licencia.			
            m_eObjectType = OBJECT_TYPE_MODULE;

			//El siguiente parametro tiene que indicarme si es encriptacion o desencriptacion.
			if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_ENCRYPTION) == 0) {
					m_eOperationType = OPERATION_TYPE_ENCRYPTION;
					if (_PermissionInOperation(OPERATION_TYPE_ENCRYPTION) == M4_FALSE) {
						printf("Insufficient permissions in encryption operation.\n");
						return M4_ERROR;
					}
			}
			else {
				if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_DECRYPTION) == 0) {
					m_eOperationType = OPERATION_TYPE_DECRYPTION;
					if (_PermissionInOperation(OPERATION_TYPE_DECRYPTION) == M4_FALSE) {
						printf("Insufficient permissions in decryption operation.\n");
						return M4_ERROR;
					}
				}
				else {
					printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
					return M4_ERROR;
				}
			}
			iCurrentArg ++;
			if (iCurrentArg == m_iArgc) {
				printf("Missing switch '%s'.\n", ARG_OPTION_SOURCE_FILE);
				return M4_ERROR;
			}

			//El siguiente parametro tiene que ser el fichero de entrada (MODULO-OBJETO)!.
			//Primero se comprueba que pasamos el fichero source y despues incremento el argumento.
			if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_SOURCE_FILE) == 0) {
				iCurrentArg ++;
				if (iCurrentArg == m_iArgc) {
					printf("Missing input filename.\n");
					return M4_ERROR;
				}
				//Obtenemos el fichero de entrada.
				m_pInputFileName = m_pArgv[iCurrentArg];

				iCurrentArg ++;
				if (iCurrentArg == m_iArgc) {
					printf("Missing switch '%s'.\n", ARG_OPTION_SOURCE_FILE);
					return M4_ERROR;
				}

				//El siguiente parametro tiene que ser el fichero de entrada (INFORMACION DE LICENCIA)!.
				//Primero se comprueba que pasamos el fichero source y despues incremento el argumento.
				if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_SOURCE_FILE) == 0) {
					iCurrentArg ++;
					if (iCurrentArg == m_iArgc) {
						printf("Missing input filename.\n");
						return M4_ERROR;
					}
					//Obtenemos el fichero de entrada.
					m_pInputFileNameLicenseInfo = m_pArgv[iCurrentArg];

					iCurrentArg ++;
					if (iCurrentArg == m_iArgc) {
						printf("Missing switch '%s'.\n", ARG_OPTION_DESTINATION_FILE);
						return M4_ERROR;
					}

					if (strcmpi(m_pArgv[iCurrentArg], ARG_OPTION_DESTINATION_FILE) == 0) {
						iCurrentArg ++;
						if (iCurrentArg == m_iArgc) {
							printf("Missing output filename.\n");
							return M4_ERROR;
						}

						//Comprobamos si hay que sobreescribir el fichero de salida.
						if (strcmp(m_pArgv[iCurrentArg], ARG_OPTION_REWRITE_DESTINATION_FILE) == 0) {
								m_bReWrite = M4_TRUE;
								iCurrentArg++;
						}
						else {
							//Si empieza por el guion es que estamos poniendo una opcion no valida.
							if (strncmp(m_pArgv[iCurrentArg], "-", 1) == 0) {
								printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
								return M4_ERROR;
							}
						}
						if (iCurrentArg == m_iArgc) {
							printf("Missing output filename.\n");
							return M4_ERROR;
						}

						//Obtenemos el fichero de salida.
						m_pOutputFileName = m_pArgv[iCurrentArg];

						if (m_eOperationType == OPERATION_TYPE_ENCRYPTION) {
							printf("Creating License encrypted.\n");
						}
						else {
							printf("Creating License decrypted.\n");
						}

						//Nos vamos a ejecutar pero iteramos otra vez por si hay mas opciones.
						//Al continuar no incrementamos el iCurrentArg.
						continue;

					}
					else {	//Destination
						printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
						return M4_ERROR;
					}
				}
				else {	//Source2
					printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
					return M4_ERROR;
				}
			}
			else {	//Source1
				printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
				return M4_ERROR;
			}
		}
		//----------------------------------------------------------------------
		//FIN GENERACION DE LICENCIA
		//----------------------------------------------------------------------

		//No hemos podido encontrar la opcion!.
		printf("Switch '%s' not recognized.\n", m_pArgv[iCurrentArg]);
		return M4_ERROR;
	}

	//Comprobamos la obligatoriedad de switches.
	if (_IsInitilized() == M4_FALSE) {
		printf("Mandatory switch is missing.\n");
		return M4_ERROR;
	}

	return _Execute();
}

m4bool_t ClOptionArguments::_PermissionInOperation( eOperation_t ai_eOperation )
{
	//-------------------------------------------------------
	//Si somos superusuario podemos hacer de todo!.
	if (_IsSuperUser() == M4_TRUE) {
		return M4_TRUE;
	}
	//-------------------------------------------------------

	//Nos dice si hay permisos para realizar la operacion.
	switch (ai_eOperation)
	{
		case OPERATION_TYPE_ENCRYPTION:
			if ((m_ePermissionMask == PERMISSION_OPERATION_SUPERUSER) 
				||(m_ePermissionMask == PERMISSION_OPERATION_ENCRYPT) ) {
					return M4_TRUE;
			}
			break;

		case OPERATION_TYPE_DECRYPTION:
			if ((m_ePermissionMask == PERMISSION_OPERATION_SUPERUSER) 
				||(m_ePermissionMask == PERMISSION_OPERATION_DECRYPT) ) {
					return M4_TRUE;
			}
			break;

		default:
			return M4_FALSE;
	}

	return M4_FALSE;
}

m4bool_t ClOptionArguments::_IsSuperUser( void )
{
	//Dependiendo de la version vamos a permitir la opcion o no.
#ifdef _M4MODKEYUTIL_SECURITY_SUPERUSER
	
	return M4_TRUE;		

#else

	return M4_FALSE;

#endif
}

m4bool_t ClOptionArguments::_IsSwitchAllowed( const m4char_t * ai_pSwitch )
{
	//Dependiendo de la version vamos a permitir la opcion o no.
#ifdef _M4MODKEYUTIL_SECURITY_LICENSE_INFO

	//En la version de info, sólo permitimos informacion de volcado.	
	if ((strcmp(ai_pSwitch, ARG_OPTION_INFO) == 0)
		|| (strcmp(ai_pSwitch, ARG_OPTION_INFO_SUMMARY) == 0)
		|| (strcmp(ai_pSwitch, ARG_OPTION_VERBOSE) == 0))
	{
		return M4_TRUE;
	}
	return M4_FALSE;

#else

	return M4_TRUE;

#endif
}

m4return_t ClOptionArguments::_Execute( void )
{
	m4return_t iRet = M4_SUCCESS;

	m4pchar_t pBuffer = NULL;
	m4int32_t iSize32 = 0;
	m4int_t iSize = 0;

	switch (m_eSwitchType)
	{
	case SWITCH_TYPE_OBJECTS:

		//Veamos qué tipo de analizador tenemos que crear!.
		m_pXmlAnalyzerInput = ClXmlAnalyzerFactory::CreateInstanceXmlAnalyzer( OBJECT_TYPE_MODULE );
		if (m_pXmlAnalyzerInput == NULL) {
			//No se ha podido crear.
			return M4_ERROR;
		}

		iRet = _InitEncrypter( &m_oClEncryption, M4MODKEY_ENCRYPT_KEY );
		if (iRet == M4_ERROR)  {
			return M4_ERROR;
		}

		iRet = _ExecuteObjectsLicense( m_pXmlAnalyzerInput );
		break;

	case SWITCH_TYPE_LICENSE:

		//Veamos qué tipo de analizador tenemos que crear!.
		m_pXmlAnalyzerInput = ClXmlAnalyzerFactory::CreateInstanceXmlAnalyzer( OBJECT_TYPE_LICENSE );
		if (m_pXmlAnalyzerInput == NULL) {
			//No se ha podido crear.
			return M4_ERROR;
		}

		iRet = _InitEncrypter( &m_oClEncryption, M4MODKEY_ENCRYPT_KEY );
		if (iRet == M4_ERROR)  {
			return M4_ERROR;
		}

		iRet = _ExecuteObjectsLicense( m_pXmlAnalyzerInput );
		break;

	case SWITCH_TYPE_GENERATION:

		//En la generacion, el primer archivo de entrada es el de objetos-modulo.
		m_pXmlAnalyzerInput = ClXmlAnalyzerFactory::CreateInstanceXmlAnalyzer( OBJECT_TYPE_MODULE );
		if (m_pXmlAnalyzerInput == NULL) {
			//No se ha podido crear.
			return M4_ERROR;
		}

		iRet = _InitEncrypter( &m_oClEncryption, M4MODKEY_ENCRYPT_KEY );
		if (iRet == M4_ERROR)  {
			return M4_ERROR;
		}

		iRet = _ExecuteGenerateLicense( m_pXmlAnalyzerInput );
		break;
	
	case SWITCH_TYPE_INFO:

		//Cuando queremos volcar informacion de la licencia, el archivo de entrada es de licencia.
		m_pXmlAnalyzerInput = ClXmlAnalyzerFactory::CreateInstanceXmlAnalyzer( OBJECT_TYPE_LICENSE );
		if (m_pXmlAnalyzerInput == NULL) {
			//No se ha podido crear.
			return M4_ERROR;
		}

		iRet = _InitEncrypter( &m_oClEncryption, M4MODKEY_ENCRYPT_KEY );
		if (iRet == M4_ERROR)  {
			return M4_ERROR;
		}

		iRet = _ExecuteInfo( m_pXmlAnalyzerInput );
		break;

	case SWITCH_TYPE_INFO_SUMMARY:

		iRet = _InitEncrypter( &m_oClEncryption, M4MODKEY_ENCRYPT_KEY_SUMMARY );
		if (iRet == M4_ERROR)  {
			return M4_ERROR;
		}

		//Queremos volcar informacion resumida de la licencia. El archivo de entrada esta encriptado.
		iRet = _ExecuteInfoSummary();
		break;

	default:
		return M4_ERROR;
	}

	return iRet;
}

m4return_t ClOptionArguments::_ExecuteObjectsLicense( IXmlAnalyzer * ai_pIXmlAnalyzer )
{
	m4pchar_t pBuffer = NULL;
	m4int32_t iSize32 = 0;
	m4int_t iSize;

	//En funcion de la operacion vamos a ejecutar.
	switch (m_eOperationType)
	{
	case OPERATION_TYPE_ENCRYPTION:

		//Queremos ENCRIPTAR un fichero que no está encriptado. Leemos y lo dejamos en el correspondiente buffer.
		
		//Leemos el fichero en LIMPIO!.

		//Lo pasamos por referencia y NOS ENCARGAREMOS DE LIBERARLO!.
		if (m_pInputFileName == NULL) {
			return M4_ERROR;
		}
		if (ClFileUtil::ReadFile( m_pInputFileName, pBuffer, iSize32 ) == M4_ERROR) {
			return M4_ERROR;
		}
		m_pTextBuffer = pBuffer;
		pBuffer = NULL;
		m_iTextSize = iSize32;
		iSize = (m4int_t) iSize32;

		//Una vez leido, lo podemos parsear sin mas pq está en limpio!.
		if (_AnalyzeXmlFile( ai_pIXmlAnalyzer, m_pTextBuffer ) == M4_ERROR) {
			return M4_ERROR;
		}

		//Antes de encriptar es necesario validar
		//Ahora tenemos que encriptar y guardar en el fichero!. Obtendremos el buffer encriptado!.
		//Ojo, podemos estar perdiendo si el tamaño es muy grande??.
		if (m_oClEncryption.Encrypt( m_pTextBuffer, m_iTextSize, pBuffer, (m4int_t) iSize ) == M4_ERROR) {
			return M4_ERROR;
		}
		m_pEncryptedBuffer = pBuffer;
		pBuffer = NULL;
		m_iEncryptedSize = (m4int32_t) iSize;

		//Por ultimo guardamos en el fichero de salida.
		if (m_pOutputFileName == NULL) {
			return M4_ERROR;
		}
		if (ClFileUtil::WriteFile( m_pOutputFileName, m_pEncryptedBuffer, m_iEncryptedSize, m_bReWrite ) == M4_ERROR) {
			return M4_ERROR;
		}

		break;

	case OPERATION_TYPE_DECRYPTION:

		//Queremos DESENCRIPTAR un fichero.
		
		//Leemos el fichero en ENCRIPTADO!.

		//Lo pasamos por referencia y NOS ENCARGAREMOS DE LIBERARLO!.
		if (m_pInputFileName == NULL) {
			return M4_ERROR;
		}
		if (ClFileUtil::ReadFile( m_pInputFileName, pBuffer, iSize32 ) == M4_ERROR) {
			return M4_ERROR;
		}
		m_pEncryptedBuffer = pBuffer;
		pBuffer = NULL;
		m_iEncryptedSize = iSize32;
		iSize = (m4int_t) iSize32;

		//Desencriptamos!.
		if (m_oClEncryption.Decrypt( m_pEncryptedBuffer, m_iEncryptedSize, pBuffer, (m4int_t) iSize ) == M4_ERROR) {
			return M4_ERROR;
		}
		m_pTextBuffer = pBuffer;
		pBuffer = NULL;
		m_iTextSize = (m4int32_t) iSize;

		//Una vez leido, lo podemos parsear sin mas pq está en limpio!.
		if (_AnalyzeXmlFile( ai_pIXmlAnalyzer, m_pTextBuffer ) == M4_ERROR) {
			return M4_ERROR;
		}

		//Por ultimo guardamos en el fichero de salida.
		if (m_pOutputFileName == NULL) {
			return M4_ERROR;
		}
		if (ClFileUtil::WriteFile( m_pOutputFileName, m_pTextBuffer, m_iTextSize, m_bReWrite ) == M4_ERROR) {
			return M4_ERROR;
		}

		break;

	default:
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClOptionArguments::_ExecuteGenerateLicense( IXmlAnalyzer * ai_pIXmlAnalyzer )
{
	m4return_t iRet = M4_SUCCESS;

	//Necesitamos leer los xml, tanto modulos-objeto como informacion de licencia (están desencriptados).

	//------------------------------ OBJETOS-MODULO -------------------------------------------
	m4pchar_t pBuffer = NULL;
	m4int32_t iSize32 = 0;

	//El primero es el de modulos-objeto.
	//Lo pasamos por referencia y NOS ENCARGAREMOS DE LIBERARLO!.
	if (ClFileUtil::ReadFile( m_pInputFileName, pBuffer, iSize32 ) == M4_ERROR) {
		return M4_ERROR;
	}

	//Ya podemos analizar.
	if (_AnalyzeXmlFile( ai_pIXmlAnalyzer, pBuffer ) == M4_ERROR) {
		delete [] pBuffer;
		return M4_ERROR;
	}
	
	//-------------------------------------
	//Nos quedamos con el buffer de entrada sera modulos desencriptado.
	if (pBuffer != NULL) {
		m_pTextBuffer = pBuffer;		
		m_iTextSize = iSize32;
	}
	pBuffer = NULL;
	//-------------------------------------

	//------------------------------ FIN OBJETOS-MODULO -------------------------------------------

	//------------------------------ INFORMACION LICENCIA -----------------------------------------
	m4pchar_t pBufferLicenseInfo = NULL;
	m4int32_t iSizeLicenseInfo32 = 0;

	m_pXmlAnalyzerLicenseInfo = ClXmlAnalyzerFactory::CreateInstanceXmlAnalyzer( OBJECT_TYPE_LICENSE );
	if (m_pXmlAnalyzerLicenseInfo == NULL) {
		//No se ha podido crear.
		return M4_ERROR;
	}

	//Leemos el fichero de informacion de licencia.
	//Lo pasamos por referencia y NOS ENCARGAREMOS DE LIBERARLO!.
	if (ClFileUtil::ReadFile( m_pInputFileNameLicenseInfo, pBufferLicenseInfo, iSizeLicenseInfo32 ) == M4_ERROR) {					
		return M4_ERROR;
	}

	//Ya podemos analizar.
	if (_AnalyzeXmlFile( m_pXmlAnalyzerLicenseInfo, pBufferLicenseInfo ) == M4_ERROR) {	
		delete [] pBufferLicenseInfo;
		return M4_ERROR;
	}

	//-------------------------------------
	//Nos quedamos el buffer de informacion de licencia por si es necesario volcarlo!.
	if (pBufferLicenseInfo != NULL) {
		m_pLicenseInfoBuffer = pBufferLicenseInfo;
		m_iLicenseInfoSize = iSizeLicenseInfo32;
	}
	pBufferLicenseInfo = NULL;
	//-------------------------------------

	//------------------------------ INFORMACION LICENCIA -----------------------------------------

	//---------------------------------------------------------------------------
	//Ahora hacemos el merge de los dos xml, y obtendremos un buffer que será la licencia.
	m4pchar_t pBufferMerged = NULL;
	m4int32_t iSizeMerged = 0;

	//Importante:
	//Solo vamos a dejar hacer merge partiendo del de informacion de licencia.
	//Es resposabilidad nuestra LIBERAR el puntero pBufferMerged
	if (m_pXmlAnalyzerLicenseInfo->MergeXml(ai_pIXmlAnalyzer, pBufferMerged, iSizeMerged) == M4_ERROR) {
		return M4_ERROR;
	}
	//---------------------------------------------------------------------------


	//---------------------------------------------------------------------------
	//Hay dos opciones, puede ser que deseemos encriptar o desencriptar la licencia generada.
	
	pBuffer = NULL;	
	m4int_t iSize = (m4int_t) iSizeMerged;

	switch (m_eOperationType)
	{
	case OPERATION_TYPE_ENCRYPTION:
		//Queremos generar la licencia encriptada, sera lo normal.
		if (m_oClEncryption.Encrypt( pBufferMerged, iSizeMerged, pBuffer, (m4int_t) iSize ) == M4_ERROR) {	
			iRet = M4_ERROR;
		}

		//Ya no me hace falta el merged.
		if (pBufferMerged != NULL) {
			delete [] pBufferMerged;
			pBufferMerged = NULL;
		}

		break;

	case OPERATION_TYPE_DECRYPTION:		
		//Queremos generar la licencia desencriptada, tendremos que tener permisos.
		//Ojo: Si estamos aqui no se habra reservado memoria para pBuffer!.
		pBuffer = pBufferMerged;
		iSize = iSizeMerged;
		break;

	default: 
		iRet = M4_ERROR;
	}

	if (iRet == M4_ERROR) {
		if (pBuffer != NULL) {
			delete [] pBuffer;
		}
		return M4_ERROR;
	}

	//Por ultimo guardamos en el fichero de salida.
	if (ClFileUtil::WriteFile( m_pOutputFileName, pBuffer, (m4int32_t) iSize, m_bReWrite ) == M4_ERROR) {
		iRet = M4_ERROR;
	}

	//-------------------------------------
	//Nos quedamos el buffer de salida!.
	//En caso de poder generar licencia desencriptada (caso atipico) este buffer realmente estará desencriptado.
	if (pBuffer != NULL) {
		m_pEncryptedBuffer = pBuffer;
		pBuffer = NULL;
		m_iEncryptedSize = iSize;
	}

	return iRet;
}

m4return_t ClOptionArguments::_ExecuteInfo( IXmlAnalyzer * ai_pIXmlAnalyzer )
{
	m4return_t iRet = M4_SUCCESS;

	m4pchar_t pBuffer = NULL;
	m4int32_t iSize32 = 0;
	m4int_t iSize = 0;

	//Queremos dar la informacion de una licencia que obligatoriamente está encriptada.
	
	//Queremos DESENCRIPTAR un fichero.
	
	//Leemos el fichero en ENCRIPTADO!.

	//Lo pasamos por referencia y NOS ENCARGAREMOS DE LIBERARLO!.
	if ((m_pInputFileName == NULL) || (ai_pIXmlAnalyzer == NULL)) {
		return M4_ERROR;
	}
	if (ClFileUtil::ReadFile( m_pInputFileName, pBuffer, iSize32 ) == M4_ERROR) {
		return M4_ERROR;
	}
	m_pEncryptedBuffer = pBuffer;
	pBuffer = NULL;
	m_iEncryptedSize = iSize32;
	iSize = (m4int_t) iSize32;

	//Desencriptamos!.
	if (m_oClEncryption.Decrypt( m_pEncryptedBuffer, m_iEncryptedSize, pBuffer, (m4int_t) iSize ) == M4_ERROR) {
		return M4_ERROR;
	}
	m_pTextBuffer = pBuffer;
	pBuffer = NULL;
	m_iTextSize = (m4int32_t) iSize;

	//Una vez leido, lo podemos parsear sin mas pq está en limpio!.
	if (_AnalyzeXmlFile( ai_pIXmlAnalyzer, m_pTextBuffer ) == M4_ERROR) {
		return M4_ERROR;
	}

	//------------ Importante!!! -------------------
	//Cuando estamos haciendo un dump de la informacion de licencia, lo que tenemos que volcar
	//en el fichero de salida NO es el buffer encriptado, tenemos que hacer un DumpInfo!!!!.
	DumpInfo(m_pOutputFileName);

	return iRet;
}

m4return_t ClOptionArguments::_ExecuteInfoSummary( void )
{
	m4return_t iRet = M4_SUCCESS;

	//Leemos de un fichero y desencriptamos! Con la clave que toca!.

	m4pchar_t pBuffer = NULL;
	m4int32_t iSize32 = 0;
	m4int_t iSize = 0;

	//Lo pasamos por referencia y NOS ENCARGAREMOS DE LIBERARLO!.
	if (m_pInputFileName == NULL) {
		return M4_ERROR;
	}
	if (ClFileUtil::ReadFile( m_pInputFileName, pBuffer, iSize32 ) == M4_ERROR) {
		return M4_ERROR;
	}
	m_pEncryptedBuffer = pBuffer;
	pBuffer = NULL;
	m_iEncryptedSize = iSize32;
	iSize = (m4int_t) iSize32;

	//Pasamos el buffer al objeto de propiedades, que extraera la informacion
	//desencriptando.
	ClModKeyProperties oClProperties;
	oClProperties.Init(m_pEncryptedBuffer, m_iEncryptedSize);

	iRet = oClProperties.DumpSummary(m_pOutputFileName);

	return iRet;
}

m4return_t ClOptionArguments::_AnalyzeXmlFile( IXmlAnalyzer * ai_pIXmlAnalyzer, const m4char_t * ai_pFileBuffer )
{
	if ((ai_pFileBuffer == NULL) || (ai_pIXmlAnalyzer == NULL)) {
		return M4_ERROR;
	}
	
	return ai_pIXmlAnalyzer->AnalyzeXmlFile(ai_pFileBuffer);
}

