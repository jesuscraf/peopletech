//	=============================================================================
//	
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              m4printpdf.cpp
//	 Project:           m4printpdf
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================

#include <stdlib.h>
#include <stdio.h>

#include "m4printpdf.hpp"
#include "m4printpdfutils.hpp"
#include "m4objglb.hpp"
#include "regedit.hpp"
#include "m4unicode.hpp"
#include "m4buildversion.hpp"
#include "m4regdef.hpp"

#ifdef _WINDOWS

#include "ddeml.h"
#include "m4filedownldr.hpp"

void _printError(HRESULT ai_hResult)
{

	DWORD	dwResult = 0 ;
	LPVOID	pvBuffer = NULL ;
	int iError = 0 ; 
	char *pcError = NULL ;

	//iError = errno ;
	//pcError = strerror(iError) ;

	dwResult = FormatMessage
		( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			ai_hResult,
			0,
			(LPSTR) &pvBuffer,
			0,
			NULL 
		) ;

	if( dwResult > 0 && pvBuffer != NULL )
	{
		DUMP_CHLOG_ERRORF(M4PRINT_DOWNLOAD_ERROR_DOWNLOAD_SYSTEM, static_cast<LPSTR>(pvBuffer)) ;
		LocalFree( pvBuffer ) ;
	}
	else
	{

		iError = errno ;
		if (iError != 0)
		{
			pcError = strerror(iError) ;
			DUMP_CHLOG_ERRORF(M4PRINT_DOWNLOAD_ERROR_DOWNLOAD_SYSTEM, pcError);
			//vaciamos el error
			errno = 0 ;
		}
		else
		{
			DUMP_CHLOG_ERRORF(M4PRINT_DOWNLOAD_ERROR_DOWNLOAD_SYSTEM, "No message available.");
		}	
	}
}



//=============================================================================
// Convierte una cadena a minusculas y cambia los slash a formato unix
// Tiene en cuenta que sólo hay parte de la cadena que hay que pasar a minúsculas
// Para ello tiene en cuenta la raiz que se lee del registro
//=============================================================================

m4pchar_t	_NormalizePath( m4pcchar_t ai_pccPath )
{

	m4pchar_t	pcResult = NULL ;
	m4return_t	iResult = M4_SUCCESS ;
	char		cResult = TRUE ;
	size_t		i = 0 ;
	size_t		iLength = 0 ;
	m4uint32_t	iOut = 0 ;
	m4char_t	acRegistryPath[ MAX_PATH + 1 ] = "" ;

	static	m4bool_t	s_bInitialized = M4_FALSE ;
	static	m4uint32_t	s_iSize = 0 ;
	static	m4char_t	s_acPattern[ MAX_PATH + 1 ] = "" ;


	if( s_bInitialized == M4_FALSE )
	{
		s_bInitialized = M4_TRUE ;

		// Se compone el path al patrón
		iResult = M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_CLIENT, "CLIENT\\Presentation Runtime Vars", acRegistryPath, MAX_PATH, iOut ) ;

		if( iResult == M4_SUCCESS )
		{
			// Se lee el patrón
			cResult = GetRegKeyData( acRegistryPath, "CommonFiles_Path", s_acPattern ) ;

			if( cResult == TRUE )
			{
				s_iSize = strlen( s_acPattern ) ;

				// Se normalizan las barras del patrón
				for( i = 0 ; i < s_iSize ; i++ )
				{
					if( s_acPattern[ i ] == '\\' )
					{
						s_acPattern[ i ] = '/' ;
					}
				}
			}
		}
	}


	if( ai_pccPath == NULL )
	{
		return( NULL ) ;
	}

	iLength = strlen( ai_pccPath ) ;

	// Se crea la cadena de salida con la cadena original
	pcResult = new m4char_t[ iLength + 1 ] ;
	
	if( pcResult == NULL )
	{
		return( NULL ) ;
	}

	memcpy( pcResult, ai_pccPath, iLength + 1 ) ;

	// Primero se normalizan las barras
	for( i = 0 ; i < iLength ; i++ )
	{
		if( pcResult[ i ] == '\\' )
		{
			pcResult[ i ] = '/' ;
		}
	}

	// Si el comienzo coincide con el patrón se pasa a minúsculas el final
	if( s_iSize > 0 && strnicmp( pcResult, s_acPattern, s_iSize ) == 0 )
	{
		for( i = s_iSize ; i < iLength ; i++ )
		{
			pcResult[ i ] = tolower( pcResult[ i ] ) ;
		}
	}

	return( pcResult ) ;
}


/****************************************************************************
*	Name:			_GetPrintingDocumentNum
*	Description:	This function returns the number of documents XFDF that
*					are being printing at the same time.
*					This function is not implemented for UNIX.
*	Parameters:
*  	Input:			ai_sXFDFProgramName: name of the process responsible to 
*					print the document.
*					ao_iPrintingDocNum: number of documents that are being 
*					printing.
*
* 	Output:
*
*	Return:			0	No error.	
*					1   Error.
*
*  Additional Information:
*
****************************************************************************/

m4bool_t _GetPrintingDocumentNum(const string &ai_sXFDFProgramName, m4uint32_t & ao_iPrintingDocNum)
{

	m4bool_t bErrorCode = M4_TRUE;
	m4uint32_t iXFDFDocNum = 0;
	string sXFDFProgramName;

	ao_iPrintingDocNum = 0;

	// Get number of these processes previously opened.
	bErrorCode = GetXFDFProcessList(ai_sXFDFProgramName, iXFDFDocNum);
	if (bErrorCode != M4_TRUE)
	{
		return M4_FALSE;
	}
	
	ao_iPrintingDocNum = iXFDFDocNum;
	return M4_TRUE;
}


#endif // _WINDOWS


/****************************************************************************
*	Name:			M4Download
*	Description:	This function download a file 
*					Bug 0113236
*
*	Parameters:
*  	Input:			0 (Input) -> Url del fichero a descargar.
*					1 (Input) -> Path local donde dejar el fichero.
*
* 	Output:
*
*	Return:			0	No error.	
*					<> 0 Error.
*
*  Additional Information:
*
****************************************************************************/

m4return_t M4Download(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	//Por defecto devolvemos error
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

#ifdef _WINDOWS
	
	int			iLength = -1 ;
	char*		pcFileName = NULL ;
	m4pchar_t	pcUrl = NULL;
	m4pchar_t	pcNormalizedUrl = NULL;
	m4pchar_t	pcTargetPath = NULL;
	m4char_t	acFilePath[M4_MAX_PATH + 1] = "";
	M4FileDownloader oDownloader;
	m4return_t	iRet;

	HRESULT		hr = S_OK ;
	BOOL		br = FALSE ;


	//Comprobaciones
	// Check parameters number.
	CHECK_CHLOG_ERRORF( ai_iLongArg != 2, M4_SUCCESS, M4PRINT_DOWNLOAD_ERROR_BAD_NUMBER_ARGUMENTS, "M4Download"<<ai_iLongArg<<2 ) ;


	// URL de donde bajarnos el fichero, tiene que venir con el fichero incluido
	CHECK_CHLOG_ERRORF( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR , M4_SUCCESS, M4PRINT_DOWNLOAD_ERROR_BAD_ARGUMENT_TYPE, "URL"<<"M4Download"<<M4ClCppType(ai_pvArg[0].Type)<<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[0].Data.PointerChar == NULL || *(ai_pvArg[0].Data.PointerChar) == '\0' , M4_SUCCESS, M4PRINT_DOWNLOAD_ERROR_NULL_ARGUMENT, "URL"<<"M4Download") ;

	pcUrl = ai_pvArg[0].Data.PointerChar ;

	// target path donde dejar el fichero descargado, con el nombre del fichero 
	// incluido en el path.
	CHECK_CHLOG_ERRORF( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR , M4_SUCCESS, M4PRINT_DOWNLOAD_ERROR_BAD_ARGUMENT_TYPE, "File"<<"M4Download"<<M4ClCppType(ai_pvArg[1].Type)<<M4ClCppType(M4CL_CPP_TYPE_STRING_VAR) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[1].Data.PointerChar == NULL || *(ai_pvArg[1].Data.PointerChar) == '\0', M4_SUCCESS, M4PRINT_DOWNLOAD_ERROR_NULL_ARGUMENT, "File"<<"M4Download") ;
	pcTargetPath = ai_pvArg[1].Data.PointerChar ;
	

	pcNormalizedUrl = _NormalizePath( pcUrl ) ;

	//1.- Nos bajamos al temporal del explorer el fichero solicitado
	// Verificar si el fichero se puede bajar desde una URL (necesario para el RichWeb).
	// Si se ha podido descargar, en pFilePath estará el path local donde se ha dejado.
	// Si no se ha podido descargar, la función devuelve false.
	// Descargar el fichero desde la URL a local.

	/* Bug 0156464
	Se pasa al formato ANSI el url porque la función lo espera en ANSI
	*/
	iLength = -1 ;
	pcFileName = M4CppToANSI( pcNormalizedUrl, iLength ) ;
	delete [] pcNormalizedUrl ;

	// bg 275109
	iRet = oDownloader.DownloadFile( pcFileName, acFilePath, M4_MAX_PATH );
	delete [] pcFileName ;

	if (iRet != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

	/* Bug 0156464
	Se pasa al formato ANSI el target porque el PATH se ha devuelto en ANSI
	*/
	iLength = -1 ;
	pcFileName = M4CppToANSI( pcTargetPath, iLength ) ;

	//Si el origen y el destino es el mismo no hacemos nada.
	if( strcmpi( acFilePath, pcFileName ) != 0 )
	{
		//2.- Copiamos el fichero del temporal del explorer al directorio indicado.
		br = CopyFile( acFilePath, pcFileName, FALSE ) ;

		if( br == 0 )
		{
			hr = GetLastError();
			
			if( hr != S_OK )
			{
				delete [] pcFileName ;
				_printError (hr) ;
				return M4_SUCCESS;
			}
		}
	}

	delete [] pcFileName ;

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;

#else

	DUMP_CHLOG_ERROR(M4PRINT_PDF_ERROR_UNIX_NOT_SUPPORTED);
	return M4_SUCCESS;

#endif // _WINDOWS
}


/****************************************************************************
*	Name:			PrintPDF
*	Description:	This function prints a file to a printer. The document to
*					print is a XFDF/FDF document (Adobe format). It can print
*					a number of copies of the given document. You can also 
*					specifies the number of copies it has to be printed at same
*					time.
*	Parameters:
*  	Input:			0 (Input) -> path to file to be printed (usually, FDF/XFDF).
*					1 (Input) -> number of copies.
*					2 (Input) -> printer name.
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/

m4return_t PrintPDF(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	//Por defecto devolvemos error
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

#ifdef _WINDOWS

	m4return_t iResult = M4_SUCCESS;
	m4bool_t bErrorCode = M4_TRUE;
	string sFilePath, sPrinterName, sCommand;
	m4int32_t iNumCopies = 0;
	m4int32_t iNumConcurrentCopies = 0;

	// Check parameters number.
	if (ai_iLongArg != 4)
	{
		SETCODEF(M4PRINT_PDF_ERROR_BAD_ARGUMENTS, ERRORLOG,"#*s1*#");
		return M4_SUCCESS;
	}

	// Get file path.
	if ((ai_pvArg[0].Type != M4CL_CPP_TYPE_NULL) && (ai_pvArg[0].Data.PointerChar != NULL))
	{
		sFilePath = (m4pchar_t) ai_pvArg[0].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4PRINT_PDF_ERROR_BAD_ARGUMENTS, ERRORLOG,"#*s1*#");
		return M4_SUCCESS;
	}
	
	// Get number of copies.
	if (ai_pvArg[1].Type != M4CL_CPP_TYPE_NULL)
	{
		iNumCopies = (m4int32_t) ai_pvArg[1].Data.DoubleData;
	}
	else
	{
		SETCODEF(M4PRINT_PDF_ERROR_BAD_ARGUMENTS, ERRORLOG,"#*s1*#");
		return M4_SUCCESS;
	}
	
	// Get number of concurrent copies.
	if (ai_pvArg[2].Type != M4CL_CPP_TYPE_NULL)
	{
		iNumConcurrentCopies = (m4int32_t) ai_pvArg[2].Data.DoubleData;
	}
	else
	{
		SETCODEF(M4PRINT_PDF_ERROR_BAD_ARGUMENTS, ERRORLOG,"#*s1*#");
		return M4_SUCCESS;
	}
	
	// Get printer name.
	if ((ai_pvArg[3].Type != M4CL_CPP_TYPE_NULL) && (ai_pvArg[3].Data.PointerChar != NULL))
	{
		sPrinterName = (m4pchar_t) ai_pvArg[3].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4PRINT_PDF_ERROR_BAD_ARGUMENTS, ERRORLOG,"#*s1*#");
		return M4_SUCCESS;
	}
	
	// Check input arguments are not empty.
	if ((sFilePath.empty() == M4_TRUE) || (sPrinterName.empty() == M4_TRUE) || iNumCopies <= 0)
	{
		SETCODEF(M4PRINT_PDF_ERROR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#");
		return M4_SUCCESS;
	}

	// If we are not able to get the number of documents that are printing, we will
	// assumed a default value.
	
	// Check the number of concurrent copies. We usually use Acrobat Reader 
	// (or Writer) to perform any action to a XFDF document. When we perform
	// printto action to a XFDF document, the first Acrobat Reader instance opened
	// remains opened, even when all documents are printed.
	// This forced us not to allow to have concurrent copies = 1.
	if (iNumConcurrentCopies < M4PDF_MIN_CONCURRENT_PRINTING_DOC)
	{
		iNumConcurrentCopies = M4PDF_MIN_CONCURRENT_PRINTING_DOC;
		
		// Not allowed to have just one concurrent copy
		// because we will just print one copy.
		SETCODEF(M4PRINT_PDF_ERROR_CONCURRENT_COPIES_NUM, DEBUGINFOLOG, "#*s1*#%d#%d#", iNumConcurrentCopies, M4PDF_MIN_CONCURRENT_PRINTING_DOC);
	}
	
	// Get the program name for printing the XFDF documents. Look for it within the registry.
	string sXFDFProgramName;
	bErrorCode = _GetXFDFProgramName(sXFDFProgramName);
	if (bErrorCode != M4_TRUE)
	{
		/* Bug 0117554
		Hay que poner el c_str, porque sino da un GP
		*/
		SETCODEF(M4PRINT_PDF_ERROR_PROGRAM_NAME, ERRORLOG, "#*s1*#%s#", sFilePath.c_str());
		return M4_SUCCESS;
	}

	// Print file.
	m4int32_t iShowMode = SW_HIDE;
	string sVerboseMode = "printto";

	/* Bug 0117946
	El nombre de la impresora debe ir entrecomillado
	*/
	string	sParameters = "";

	if( sPrinterName[ 0 ] != '\"' )
	{
		sParameters += "\"";
	}

	sParameters += sPrinterName ;

	if( sPrinterName[ 0 ] != '\"' )
	{
		sParameters += "\"";
	}

	for (m4uint32_t i = 0; i < iNumCopies; i++)
	{
		// Check number of concurrent copies. We can only have a number of copies
		// printing at same time.
		while (1)
		{
			// Get number of documents that are printing at the same time.
			m4uint32_t iDocPrint = 0;
			if (_GetPrintingDocumentNum(sXFDFProgramName, iDocPrint) != M4_TRUE)
			{
				SETCODEF(M4PRINT_PDF_ERROR_GET_PRINT_DOC_NUM, ERRORLOG, "#*s1*#%d#", iNumCopies);
			}
			
			if (iDocPrint < iNumConcurrentCopies)
			{
				HANDLE hProcessDummy = 0;
				iResult = _ShellExecute(sFilePath, iShowMode, sVerboseMode, sParameters, hProcessDummy);
				if (iResult != M4_SUCCESS)
				{
					SETCODEF(M4PRINT_PDF_ERROR_PRINT_DOCUMENT, ERRORLOG, "#*s1*#%s#", sFilePath.c_str());
					return M4_SUCCESS;
				}
				break;
			}
			else
			{
				Sleep(5000);
			}
		}
	}

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;

#else

	DUMP_CHLOG_ERROR(M4PRINT_PDF_ERROR_UNIX_NOT_SUPPORTED);
	return M4_SUCCESS;

#endif // _WINDOWS
}



/****************************************************************************
*	Name:			PrintPDFWithoutConcurrency
*	Description:	This function prints a file to a printer. The document to
*					print is a XFDF/FDF document (Adobe format). It can print
*					a number of copies of the given document. You cannot 
*					specifies the number of copies it has to be printed at same
*					time (there is no concurrency), we print the copies 
*					one-to-one. When the document (PDF document) is within 
*					the windows spooler, we close Acrobat Reader. The document
*					is identified in thw spooler by the PDF document name.
*	Parameters:
*  	Input:			0 (Input) -> path to file to be printed (usually, FDF/XFDF).
*					1 (Input) -> PDF document name.
*					2 (Input) -> number of copies.
*					3 (Input) -> printer name.
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/

m4return_t PrintPDFWithoutConcurrency(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	//Por defecto devolvemos error
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;

#ifdef _WINDOWS

	m4return_t	iResult = M4_SUCCESS;
	m4uint32_t	i = 0;
	m4int32_t	iNumCopies = 0;
	m4pchar_t	pcPDFFileName = NULL ;
	m4pchar_t	pcPinterName = NULL ;
	string		sXFDFFilePath;
	ofstream	fout;
	m4bool_t	bIsAcrobatRunning = M4_FALSE;
	DWORD		dwIdInst = 0;
	HCONV		hConv = NULL;
	string		sDriverName = "";
	string		sPortName = "";
	string		sCloseAcrobatCmdLine = "";
	HANDLE		hProcessDummy = 0;
	string		sVerboseMode = "open";
	m4int32_t	iShowMode = SW_NORMAL;
	string		sParameters = "";

	// Se abre el fichero de trazas
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

	PDF_DEBUG( M4Open( fout, "c:\\temp\\pdftrace.txt", M4UniAppend, eEncoding ) ) ;

	// Check parameters number.
	CHECK_CHLOG_ERROR( ai_iLongArg != 4, M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;


	// Get XFDF file path.
	CHECK_CHLOG_ERROR( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR , M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;
	CHECK_CHLOG_ERROR( ai_pvArg[0].Data.PointerChar == NULL || *(ai_pvArg[0].Data.PointerChar) == '\0' , M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;
	sXFDFFilePath = ai_pvArg[0].Data.PointerChar ;

	
	// Get PDF file name.
	CHECK_CHLOG_ERROR( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR , M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;
	CHECK_CHLOG_ERROR( ai_pvArg[1].Data.PointerChar == NULL || *(ai_pvArg[1].Data.PointerChar) == '\0' , M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;
	pcPDFFileName = ai_pvArg[1].Data.PointerChar ;


	// Get number of copies.
	CHECK_CHLOG_ERROR( ai_pvArg[2].Type != M4CL_CPP_TYPE_NUMBER, M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;
	CHECK_CHLOG_ERROR( ai_pvArg[2].Data.DoubleData == 0, M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;
	iNumCopies = ai_pvArg[2].Data.DoubleData ;

	
	// Get printer name.
	CHECK_CHLOG_ERROR( ai_pvArg[3].Type != M4CL_CPP_TYPE_STRING_VAR , M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;
	CHECK_CHLOG_ERROR( ai_pvArg[3].Data.PointerChar == NULL || *(ai_pvArg[3].Data.PointerChar) == '\0' , M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS ) ;
	pcPinterName = ai_pvArg[3].Data.PointerChar ;

	
	PDF_DEBUG(fout << endl << endl);
	PDF_DEBUG(fout << "Imprimiendo archivo '" << sXFDFFilePath << "', número de copias " <<  iNumCopies << ", impresora '" << pcPinterName << "'..." << endl);

	// Bug 137828. Se utiliza DDE para imprimir.
	
	// Para que se pueda imprimir con DDE se tiene que garantizar que no existe ninguna instancia de Acrobat
	// ejecutándose en la máquina.
	m4int32_t iNum = 0;
	PDF_DEBUG(fout << "Comprobando si existen instancias de Acrobat ejecutándose ..." << endl);
	while (iNum < M4PDF_ACROBAT_RUNNING_MAX_NUM)
	{
		iResult = _IsAcrobatRunning(bIsAcrobatRunning);
		if (iResult != M4_SUCCESS)
		{
			SETCODEF(M4PRINT_PDF_ERROR_PRINT_DOCUMENT, ERRORLOG, "#*s1*#%s#", sXFDFFilePath.c_str());
			return M4_SUCCESS;
		}
		
		if (bIsAcrobatRunning == M4_FALSE)
		{
			break;
		}
		else
		{
			PDF_DEBUG(fout << "Existen instancias de Acrobat ejecutándose actualmente. Vuelta número: " << iNum << endl);
		}
					
		iNum++;
		
		Sleep(M4PDF_ACROBAT_RUNNING_WAIT_TIME);
	}

	if (bIsAcrobatRunning == M4_TRUE)
	{
		PDF_DEBUG(fout << "No se puede imprimir el documento porque existen instancias de Acrobat ejecutándose actualmente." << endl);
		SETCODEF(M4PRINT_ACROBAT_CURRENTLY_RUNNING, ERRORLOG, "#*s1*#%s#", sXFDFFilePath.c_str());
		return M4_SUCCESS;
	}

	PDF_DEBUG(fout << "No existen instancias de Acrobat ejecutándose actualmente." << endl);

	PDF_DEBUG(fout << "Inicialización servidor DDE ..." << endl);
	iResult = DdeInitialize(&dwIdInst, (PFNCALLBACK)_DdeCallback, APPCLASS_STANDARD | APPCMD_CLIENTONLY, 0);
	CHECK_CHLOG_ERRORF(iResult != DMLERR_NO_ERROR, M4_SUCCESS, M4PRINT_PDF_ERROR_PRINT_DOCUMENT, sXFDFFilePath.c_str());
	
	PDF_DEBUG(fout << "Servidor DDE correctamente inicializado." << endl);

	// Abrir el acrobat con el documento que se desea imprimir.
	PDF_DEBUG(fout << "Abriendo documento XFDF \""<< sXFDFFilePath.c_str() << "\"." << endl);
	iResult = _OpenXFDFDocument(dwIdInst, sXFDFFilePath, hConv, fout);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4PRINT_PDF_ERROR_PRINT_DOCUMENT, ERRORLOG, "#*s1*#%s#", sXFDFFilePath.c_str());
		DdeDisconnect(hConv);
		DdeUninitialize(dwIdInst);
		return M4_SUCCESS;
	}
	PDF_DEBUG(fout << "Documento XFDF \""<< sXFDFFilePath.c_str() << "\"" << " abierto."<< endl);
	
	// Obtener el driver y el puerto de la impresora.
	PDF_DEBUG(fout << "Obtener datos de la impresora \"" << pcPinterName << "\"." << endl);
	iResult = _GetPrinterInfo(pcPinterName, sDriverName, sPortName);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4PRINT_PDF_ERROR_PRINT_DOCUMENT, ERRORLOG, "#*s1*#%s#", sXFDFFilePath.c_str());
		DdeDisconnect(hConv);
		DdeUninitialize(dwIdInst);
		return M4_SUCCESS;
	}
	PDF_DEBUG(fout << "Datos de la impresora \""<< pcPinterName << "\"" << " obtenidos."<< endl);

	// Imprimir las copias que se indican.
	for( i = 0; i < iNumCopies; i++ )
	{
		HDDEDATA hData = NULL;
		string sPrintDocCmdLine = "";
		string sPrinterName = pcPinterName;

		PDF_DEBUG(fout << "Imprimiendo copia número: " << i + 1 << " de un total de " << iNumCopies << endl);
		
		// Componer el comando DDE para imprimir.
		// sPrintDocCmdLine = "[FilePrintSilentEx(";
		// sPrintDocCmdLine += "\"" + sXFDFFilePath + "\")]";
		sPrintDocCmdLine = "[FilePrintTo(";
		sPrintDocCmdLine += "\"" + sXFDFFilePath + "\"";
		sPrintDocCmdLine += ",\"" + sPrinterName + "\"";
		sPrintDocCmdLine += ",\"" + sDriverName + "\"";
		sPrintDocCmdLine += ",\"" + sPortName + "\")]";
		iResult = _ExecuteDDECommand(dwIdInst, hConv, sPrintDocCmdLine);
		if (iResult != M4_SUCCESS)
		{
			SETCODEF(M4PRINT_PDF_ERROR_PRINT_DOCUMENT, ERRORLOG, "#*s1*#%s#", sXFDFFilePath.c_str());
			DdeDisconnect(hConv);
			DdeUninitialize(dwIdInst);
			return M4_SUCCESS;
		}

		PDF_DEBUG( fout << "La copia número " << i + 1 << " se ha mandado a imprimir correctamente." << endl );
	}

	// Cerrar Acrobat.
	PDF_DEBUG( fout << "Cerrando aplicación Acrobat..." << endl );
	
	sCloseAcrobatCmdLine = "[AppExit()]";
	iResult = _ExecuteDDECommand(dwIdInst, hConv, sCloseAcrobatCmdLine);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4PRINT_PDF_ERROR_PRINT_DOCUMENT, ERRORLOG, "#*s1*#%s#", sXFDFFilePath.c_str());
		DdeDisconnect(hConv);
		DdeUninitialize(dwIdInst);
		return M4_SUCCESS;
	}

	PDF_DEBUG(fout << "La aplicación Acrobat ha sido cerrado con éxito." << endl << endl << flush);
	PDF_DEBUG(fout << endl << "===============================================================================================" << endl << endl << flush);

	// Desconexión.
	if (hConv != NULL)
	{
		DdeDisconnect(hConv);
	}
	
	if (dwIdInst != NULL)
	{
		DdeUninitialize(dwIdInst);
	}
	

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;

#else

	DUMP_CHLOG_ERROR(M4PRINT_PDF_ERROR_UNIX_NOT_SUPPORTED);
	return M4_SUCCESS;

#endif // _WINDOWS
}

/****************************************************************************
*	Name:			GetAcrobatVersion
*	Description:	Get Acrobat installed version.
*	Parameters:
*  	Input:			0 (Output) -> Acrobat installed version.
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/

m4return_t GetAcrobatVersion(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	string	sAcrobatVersion = "";

	//Por defecto devolvemos error
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_ERROR;
	
#ifdef _WINDOWS

	m4return_t	iResult = M4_SUCCESS;
	
	// Check parameters number.
	CHECK_CHLOG_ERROR(ai_iLongArg != 1, M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS);

	CHECK_CHLOG_ERROR(ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4PRINT_PDF_ERROR_BAD_ARGUMENTS);
	
	_GetAcrobatVersion(sAcrobatVersion);
	
	ai_pvArg[0].SetSysString((m4pchar_t)sAcrobatVersion.c_str());
	
	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;

#else

	DUMP_CHLOG_ERROR(M4PRINT_PDF_ERROR_UNIX_NOT_SUPPORTED);
	return M4_SUCCESS;

#endif // _WINDOWS
}
