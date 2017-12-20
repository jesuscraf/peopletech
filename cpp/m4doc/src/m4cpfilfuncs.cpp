//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc library
// File:             functions.cpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             30-August-1999
// Language:         C++
// Operating System: Windows 32 (All), UNIX - Solaris (part)
// Design Document:  
//
//
// Definition:
//
// Original m4cpfil functions now on m4doc.
//
//		M4CopyFileGD
//		M4DMDeleteFile	
//		M4DMSetPermissionOnFile
// 
//==============================================================================


// This project headeres.
#include "m4docfunctions.hpp"
#include "m4odmdef.hpp"
#include "m4docmultipla.hpp"

// Copy functions and more.
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

//Errors.
#include "m4docerrors.hpp"
#include "m4unicode.hpp"





/* **************************************************************************** */
// Function: M4CopyFileGD															
// Module:   m4cpfil library
// Arguments:																	
//		IN:    (Como argumentos de LN4, la cabecera de la función es la standart para
//				 llamadas desde CVM
//			sFileSource			string that specifies the name of an existing file. 
//			sFileTarget			string that specifies the name of the new file. 
//			bFailIfExists		Specifies how this operation is to proceed if a file 
//								of the same name as that specified by lpNewFileName already
//								exists. If this parameter is TRUE and the new file already
//								exists, the function fails. 
//								If this parameter is FALSE and the new file already exists,
//								the function overwrites the existing file and succeeds. 
//			
//		RETURN:
//			A CMV M4_SUCCESS, puesto que es dificil que el método como código falle
//			A LN4, en ai_pvReturn:
//						M4_ERROR		si hay un error, se añade a la Log informació adicional
//						M4_SUCCESS		si todo va bien
// 
//		This function copies fileSource on fileTarget, sets fileTarget permissions to read and write and makes it visible. 
//		The flag specifies how this operation is to proceed if a file of the same name as that 
//		specified by fileTarget already exists. 
//		If this parameter is TRUE(=1) and the new file already exists, the function fails. 
//		If this parameter is FALSE(=0) and the new file already exists, the function overwrites the existing file, succeeds.
//
/* **************************************************************************** */
m4return_t M4CopyFileGD( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// varaibles
	m4int_t iFailIfExists=0;
	string  sFileSource, 
		    sFileTarget;
	string  sErrorMessage;
	FILE*	ptrFileSource = NULL;
	FILE*	ptrFileTarget = NULL;
	
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[2].GetType() != M4CL_CPP_TYPE_NUMBER) )	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

	// Recogemos los valores de la llamada
		sFileSource = m4pchar_t(ai_pvArg[0]);
		sFileTarget = m4pchar_t(ai_pvArg[1]);
		iFailIfExists = (m4int_t) m4double_t(ai_pvArg[2]);

	// Comprobamos la existencia de los ficheros
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	if ( (ptrFileSource = M4Fopen(sFileSource.c_str(), M4UniReadBinary, eEncoding)) == NULL) 	{
		// Un error
		sErrorMessage = "Sourde: <" + sFileSource + ">";
		M4DOCErrorHandling(M4DOCERROR_FILENOEXISTS, sErrorMessage.c_str());
		// el error se revuelve el return del código LN4, no en la llamada a la función C
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_SUCCESS; // Failure.
	}

	// Veamos si existe el destino
	// UNICODE FILE
	eEncoding = M4UniANSI ;
	if ( (ptrFileTarget = M4Fopen(sFileTarget.c_str(), M4UniReadBinary, eEncoding)) != NULL ) 
	{
		// el fichero existe
		if (iFailIfExists == 1) 
		{
			// Un error
			// antes de que se nos olvide el fichero origen lo cerramos
			if (ptrFileSource != NULL) {
				fclose (ptrFileSource);
			}

			fclose (ptrFileTarget);

			sErrorMessage = "Target: <" + sFileTarget + "> The file already exists.";
			M4DOCErrorHandling(M4DOCERROR_OTHER, sErrorMessage.c_str());
			// el error se revuelve el return del código LN4, no en la llamada a la función C
			ai_pvReturn.SetDouble(M4_ERROR); 
			return M4_SUCCESS; // Failure.
		}
		else 
		{
			// el fichero existe y nos han dicho que podemos machacarlo, pero
			// ¿ tenemos permisos para grabarlo ?
			fclose (ptrFileTarget);

			// UNICODE FILE
			eEncoding = M4UniANSI ;
			if ( (ptrFileTarget = M4Fopen(sFileTarget.c_str(), M4UniWrite, eEncoding)) == NULL ) {
				// Un error
				// antes de que se nos olvide el fichero origen lo cerramos
				fclose (ptrFileSource);

				sErrorMessage = "Target: <" + sFileTarget + ">";
				M4DOCErrorHandling(M4DOCERROR_PERMIT, sErrorMessage.c_str());
				// el error se revuelve el return del código LN4, no en la llamada a la función C
				ai_pvReturn.SetDouble(M4_ERROR); 
				return M4_SUCCESS; // Failure.
			}
		}
	}

	
	// recordar que tenemos los dos ficheros abiertos
	// pero en Unix los necesitamos asi

// WINDOWS
// ------------
#if defined(WINDOWS) || defined(_WINDOWS) || defined(_M4WINX86_)

	// cerramos los ficheros porque la copia la hace el SO
	if (ptrFileSource != NULL) {
		fclose (ptrFileSource);
	}

	if (ptrFileTarget != NULL) {
		fclose (ptrFileTarget);
	}

	// UNICODE FILE
	int iLength = -1 ;

	iLength = -1 ;
	M4XMLCh* pwcFileSource = M4CppToUtf16( sFileSource.c_str(), iLength ) ;
	iLength = -1 ;
	M4XMLCh* pwcFileTarget = M4CppToUtf16( sFileTarget.c_str(), iLength ) ;

	// Copy file.
	BOOL b = CopyFileW( (LPCWSTR) pwcFileSource, (LPCWSTR) pwcFileTarget, iFailIfExists);
	delete pwcFileSource ;

	if( b != 0 ) { 
		// If the source file were hidden, the target file would also retain this
		// hidden attribute, to prevent undesired effect the file is made visible.
		DWORD dTargetAttributes, dTargetNewAttributes;

		dTargetAttributes = GetFileAttributesW( (LPCWSTR) pwcFileTarget ) ;

		if (dTargetAttributes & FILE_ATTRIBUTE_HIDDEN)	{
			dTargetNewAttributes = dTargetAttributes & (~FILE_ATTRIBUTE_HIDDEN);
			SetFileAttributesW ( (LPCWSTR) pwcFileTarget, dTargetNewAttributes);
		}

		int c = _wchmod( (wchar_t *) pwcFileTarget, _S_IWRITE ) ;
		delete pwcFileTarget ;

		// The access mode of the target is changed to R/W.
		if( c !=0 ) {
			M4DOCErrorHandling(M4DOCERROR_PERMIT, sFileTarget.c_str());
			// el error se revuelve el return del código LN4, no en la llamada a la función C
			ai_pvReturn.SetDouble(M4_ERROR); 
		}
		else {
			// todo Ok
			ai_pvReturn.SetDouble(M4_SUCCESS); 
		}
	} 
	else {
		//Failure en la copia
		LPVOID lpMsgBuf;
		FormatMessageW (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
			           NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					   (LPWSTR)&lpMsgBuf, 0, NULL);

		iLength = -1 ;
		m4pchar_t pcError = M4Utf16ToCpp( (M4XMLCh*) lpMsgBuf, iLength ) ;
		LocalFree(lpMsgBuf); 
		sErrorMessage = pcError ;
		delete pcError ;

		sErrorMessage = "(Source, Target) <" + sFileSource + "> , <" + sFileTarget + ">. Error: " + sErrorMessage;
		M4DOCErrorHandling(M4DOCERROR_NATIVE, sErrorMessage.c_str());

		// el error se revuelve el return del código LN4, no en la llamada a la función C
		ai_pvReturn.SetDouble(M4_ERROR); 
	} // copia

	return M4_SUCCESS; // Failure.

	
// UNIX
// ------------------
#elif defined(UNIX) || defined(_UNIX) || defined(SOLARIS)

	// variables
	const int blockSize=1024;
	m4int_t sizeRead=0;
	m4char_t buff[blockSize];
	
	// Open target file for writing. In the case that sFileTarget does not
	// exit, is NOT open. If target file exists, it has been opened for 
	// reading before. If target file does not exist, it has not been opened 
	// for reading before.
	// We check the target file is not already open and if it is the case,
	// open it.
	if (ptrFileTarget == NULL)
	{
		// UNICODE FILE
		eEncoding = M4UniANSI ;
		if ((ptrFileTarget = M4Fopen(sFileTarget.c_str(), M4UniWrite, eEncoding)) == NULL)
		{
			// Un error
			// antes de que se nos olvide el fichero origen lo cerramos
			fclose (ptrFileSource);

			sErrorMessage = "Target: <" + sFileTarget + ">";
			M4DOCErrorHandling(M4DOCERROR_FILENOEXISTS, sErrorMessage.c_str());
		
			// el error se revuelve el return del código LN4, no en la llamada a la función C
			ai_pvReturn.SetDouble(M4_ERROR); 
			return M4_SUCCESS; // Failure.
		}
	}

	//Copying loop.
	while(1)
	{
		//The source file is copied in blocks of blockSize size.
		if( (sizeRead=fread(buff,sizeof(char),blockSize,ptrFileSource)) < (blockSize) )
		{
			//If the 'if' above is true it means: a)the end of the file has been reached or b)there's been an error while reading.
			if(feof(ptrFileSource)) // a)Success, EOF reached.
			{
				//The last block is written.
				if( (fwrite(buff,sizeof(char),sizeRead,ptrFileTarget)) < (sizeRead) )
				{
					//There was an error while writing the last block.
					sErrorMessage = "(Source, Target) <" + sFileSource + "> , <" + sFileTarget + ">. ";
					M4DOCErrorHandling(M4DOCERROR_OTHER, sErrorMessage.c_str());										
					// cerramos todo
					fclose(ptrFileSource);
					fclose(ptrFileTarget);

					// el error se revuelve el return del código LN4, no en la llamada a la función C
					ai_pvReturn.SetDouble(M4_ERROR); 
					return M4_SUCCESS; 
				}
				else break;	//The last block was succesfully written.
			}
			else {
				// 	There's been an error.
				sErrorMessage = "(Source, Target) <" + sFileSource + "> , <" + sFileTarget + ">. ";
				M4DOCErrorHandling(M4DOCERROR_OTHER, sErrorMessage.c_str());		

				//There was an error while writing the last block.
				fclose(ptrFileSource);
				fclose(ptrFileTarget);

				// el error se revuelve el return del código LN4, no en la llamada a la función C
				ai_pvReturn.SetDouble(M4_ERROR); 
				return M4_SUCCESS; 
			}

		}
		else {
			//Block of size blockSize succesfully read.
			fwrite(buff,sizeof(char),blockSize,ptrFileTarget);
		}
	}

	// cerramos todo
	// cerramos los ficheros porque la copia la hace el SO
	if (ptrFileSource != NULL) {
		fclose (ptrFileSource);
	}

	if (ptrFileTarget != NULL) {
		fclose (ptrFileTarget);
	}

	// exito en LN4
	ai_pvReturn.SetDouble(M4_SUCCESS);
	return M4_SUCCESS;
#endif
}
/* **************************************************************************** */



/* **************************************************************************** */
// Function: M4DMDeleteFile
// Module:   m4cpfil library
// Arguments:																	
//		IN:    (Como argumentos de LN4, la cabecera de la función es la standart para
//				 llamadas desde CVM
//			sFileName			string that specifies the name of an existing file. 
//		RETURN:
//			A CMV M4_SUCCESS, puesto que es dificil que el método como código falle
//			A LN4, en ai_pvReturn:
//						M4_ERROR		si hay un error, se añade a la Log informació adicional
//						M4_SUCCESS		si todo va bien
// Description.
//		This function Deletes a file.
//
/* **************************************************************************** */
m4return_t M4DMDeleteFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// variables
	int		iRet = -1;
	char*	pcError = NULL;
	string  sFileName; 
	string  sErrorMessage;

	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) )
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

	// Recogemos los valores de la llamada
	sFileName = m4pchar_t(ai_pvArg[0]);

	iRet = M4RemoveFile( sFileName.c_str(), &pcError ) ;

	if( iRet == -1 )
	{
		sErrorMessage = "File: <" + sFileName + " >. Error: " + pcError;
		M4DOCErrorHandling(M4DOCERROR_DELETEFILE, sErrorMessage.c_str());

		// el error se revuelve el return del código LN4, no en la llamada a la función C
		ai_pvReturn.SetDouble(M4_ERROR); 
	}
	else if( iRet == 1 )
	{
		sErrorMessage = "File: <" + sFileName + " >";
		M4DOCErrorHandling(M4DOCERROR_FILENOEXISTS, sErrorMessage.c_str());
		// el error se revuelve el return del código LN4, no en la llamada a la función C
		ai_pvReturn.SetDouble(M4_ERROR); 
	}
	else
	{
		ai_pvReturn.SetDouble(M4_SUCCESS); 
	}

	if( pcError != NULL )
	{
		delete( pcError ) ;
	}

	return M4_SUCCESS; 
}
/* **************************************************************************** */


/* **************************************************************************** */
// Function: M4DMSetPermission 
// Module:   m4cpfil library
// Arguments:																	
//		IN:
//				fileName - existing file
//				permission attribute - 	0: Read access. 1: Write access.
//
// Purpose of this function: 
//			Provide file R/W permission control from Mind.
//
// ONLY Windows supported!!!
/* **************************************************************************** */
m4return_t M4DMSetPermissionOnFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	// variables
	string  sFileName; 
	string  sErrorMessage;
	m4int_t permitFlag;
	int retChMod=0;

	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_NUMBER) )
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

		// Recogemos los valores de la llamada
		sFileName = m4pchar_t(ai_pvArg[0]);
		permitFlag = (m4int_t) m4double_t(ai_pvArg[1]);
		// Second argument is the mode to be set on the file.
		// 0: Read access.	// 1: Write access.

// Windows.
#if defined(WINDOWS) || defined(_WINDOWS) || defined(_M4WINX86_)
	// The access mode of the target is changed to R/W.
	// 0: Read access.	// 1: Write access.

	/* Bug 0174509
	Hay que pasar de CPP a Utf16
	*/
	int iLength = -1 ;
	M4XMLCh* pwcFileName = M4CppToUtf16( sFileName.c_str(), iLength ) ;

	switch(permitFlag)
	{
		case 0:
			retChMod = _wchmod((wchar_t *) pwcFileName, _S_IREAD);
			break;
		case 1:
			retChMod = _wchmod((wchar_t *) pwcFileName, _S_IWRITE);
			break;
		default:
			retChMod=1; //to avoid error notification below.
			break;
	}

	delete pwcFileName ;

	if(retChMod!=0)
	{
		M4DOCErrorHandling(M4DOCERROR_PERMIT, sFileName.c_str());
		LPVOID lpMsgBuf;
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			          NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					  (LPWSTR)&lpMsgBuf, 0, NULL);

		iLength = -1 ;
		m4pchar_t pcError = M4Utf16ToCpp( (M4XMLCh*) lpMsgBuf, iLength ) ;
		LocalFree(lpMsgBuf); 
		sErrorMessage = pcError ;
		delete pcError ;

		sErrorMessage = "File: <" + sFileName + " >. Error: " + sErrorMessage;
		M4DOCErrorHandling(M4DOCERROR_PERMIT, sErrorMessage.c_str());

		// el error se revuelve el return del código LN4, no en la llamada a la función C
		ai_pvReturn.SetDouble(M4_ERROR); 

	}
	// el error se revuelve el return del código LN4, no en la llamada a la función C
	ai_pvReturn.SetDouble(M4_SUCCESS); 

// UNIX
#else
	// si alguien lo necesita que se lo pique
#endif

// Redundant.
return M4_SUCCESS;
}
/* **************************************************************************** */


m4void_t M4DOCSetWarning( m4int_t ai_iError, m4pcchar_t ai_pccMessage )
{
	string txt = "#*s0*#";

	txt = txt + ai_pccMessage + "#";
	SETCODE( ai_iError, WARNINGLOG, txt.c_str());
	return;
}

/* **************************************************************************** */
// Function: M4CopyFileGDRetrying															
// Arguments:																	
//			sFileSource:	Name of an existing file. 
//			sFileTarget:	Name of the target file. 
//			bFailIfExists:	Must return error if target file exists.
//			iMaxRetries:	Maximun number of retries in case of error.
//			lRetryDelay:	Milliseconds of delay between retries.
//		RETURN:
//			C: M4_SUCCESS / M4_ERROR
// 
// See M4CopyFileGD
/* **************************************************************************** */
m4return_t M4CopyFileGDRetrying( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4int_t iFailIfExists=0;
	string  sFileSource, 
		    sFileTarget;
	string  sErrorMessage;
	FILE*	ptrFileSource = NULL;
	FILE*	ptrFileTarget = NULL;
	int		iMaxRetries;
	int		iNumRetries;
	int		lRetryDelay;
	
	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[2].GetType() != M4CL_CPP_TYPE_NUMBER) ||
		 (ai_pvArg[3].GetType() != M4CL_CPP_TYPE_NUMBER) ||
		 (ai_pvArg[4].GetType() != M4CL_CPP_TYPE_NUMBER) )	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}

	// Recogemos los valores de la llamada
	sFileSource = m4pchar_t(ai_pvArg[0]);
	sFileTarget = m4pchar_t(ai_pvArg[1]);
	iFailIfExists = (m4int_t) m4double_t(ai_pvArg[2]);
	iMaxRetries = (m4int32_t) m4double_t(ai_pvArg[3]);
	lRetryDelay = (m4int32_t) m4double_t(ai_pvArg[4]);

	// Comprobamos la existencia de los ficheros
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	if ( (ptrFileSource = M4Fopen(sFileSource.c_str(), M4UniReadBinary, eEncoding)) == NULL) 	{
		// Un error
		sErrorMessage = "Sourde: <" + sFileSource + ">";
		M4DOCErrorHandling(M4DOCERROR_FILENOEXISTS, sErrorMessage.c_str());
		// el error se revuelve el return del código LN4, no en la llamada a la función C
		ai_pvReturn.SetDouble(M4_ERROR); 
		return M4_SUCCESS; // Failure.
	}

	// Veamos si existe el destino
	// UNICODE FILE
	eEncoding = M4UniANSI ;
	if ( (ptrFileTarget = M4Fopen(sFileTarget.c_str(), M4UniReadBinary, eEncoding)) != NULL ) 
	{
		// el fichero existe
		if (iFailIfExists == 1) 
		{
			// Un error
			// antes de que se nos olvide el fichero origen lo cerramos
			if (ptrFileSource != NULL) {
				fclose (ptrFileSource);
			}

			fclose (ptrFileTarget);

			sErrorMessage = "Target: <" + sFileTarget + "> The file already exists.";
			M4DOCErrorHandling(M4DOCERROR_OTHER, sErrorMessage.c_str());
			// el error se revuelve el return del código LN4, no en la llamada a la función C
			ai_pvReturn.SetDouble(M4_ERROR); 
			return M4_SUCCESS; // Failure.
		}
		else 
		{
			// el fichero existe y nos han dicho que podemos machacarlo, pero
			// ¿ tenemos permisos para grabarlo ?
			fclose (ptrFileTarget);

			// UNICODE FILE
			eEncoding = M4UniANSI ;
			if ( (ptrFileTarget = M4Fopen(sFileTarget.c_str(), M4UniWrite, eEncoding)) == NULL ) {
				// Un error
				// antes de que se nos olvide el fichero origen lo cerramos
				fclose (ptrFileSource);

				sErrorMessage = "Target: <" + sFileTarget + ">";
				M4DOCErrorHandling(M4DOCERROR_PERMIT, sErrorMessage.c_str());
				// el error se revuelve el return del código LN4, no en la llamada a la función C
				ai_pvReturn.SetDouble(M4_ERROR); 
				return M4_SUCCESS; // Failure.
			}
		}
	}

	
	// recordar que tenemos los dos ficheros abiertos
	// pero en Unix los necesitamos asi

// WINDOWS
// ------------
#if defined(WINDOWS) || defined(_WINDOWS) || defined(_M4WINX86_)

	// cerramos los ficheros porque la copia la hace el SO
	if (ptrFileSource != NULL) {
		fclose (ptrFileSource);
	}

	if (ptrFileTarget != NULL) {
		fclose (ptrFileTarget);
	}

	// UNICODE FILE
	int iLength = -1 ;

	iLength = -1 ;
	M4XMLCh* pwcFileSource = M4CppToUtf16( sFileSource.c_str(), iLength ) ;
	iLength = -1 ;
	M4XMLCh* pwcFileTarget = M4CppToUtf16( sFileTarget.c_str(), iLength ) ;

	iNumRetries = 0;
	iMaxRetries = ( iMaxRetries > 0 ) ? iMaxRetries : 1;
	char strRetErr[64];
	BOOL b		= false;

	do
	{
		if( iNumRetries && lRetryDelay > 0 )
		{
			Sleep( lRetryDelay );
		}

		iNumRetries++;

		// Copy file.
		b = CopyFileW((LPCWSTR) pwcFileSource, (LPCWSTR) pwcFileTarget, iFailIfExists);

		if( b != 0 ) { 
			// If the source file were hidden, the target file would also retain this
			// hidden attribute, to prevent undesired effect the file is made visible.
			DWORD dTargetAttributes, dTargetNewAttributes;

			dTargetAttributes = GetFileAttributesW((LPCWSTR) pwcFileTarget ) ;

			if (dTargetAttributes & FILE_ATTRIBUTE_HIDDEN)	{
				dTargetNewAttributes = dTargetAttributes & (~FILE_ATTRIBUTE_HIDDEN);
				SetFileAttributesW ((LPCWSTR) pwcFileTarget, dTargetNewAttributes);
			}

			int c = _wchmod( (wchar_t *) pwcFileTarget, _S_IWRITE ) ;
			delete pwcFileTarget ;

			// The access mode of the target is changed to R/W.
			if( c !=0 ) {
				M4DOCErrorHandling(M4DOCERROR_PERMIT, sFileTarget.c_str());
				// el error se revuelve el return del código LN4, no en la llamada a la función C
				ai_pvReturn.SetDouble(M4_ERROR); 
			}
			else {
				// todo Ok
				ai_pvReturn.SetDouble(M4_SUCCESS); 
			}
		} 
		else {
			//Failure en la copia
			LPVOID lpMsgBuf;
			FormatMessageW (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
						   NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
						   (LPWSTR)&lpMsgBuf, 0, NULL);

			iLength = -1 ;
			m4pchar_t pcError = M4Utf16ToCpp( (M4XMLCh*) lpMsgBuf, iLength ) ;
			LocalFree(lpMsgBuf); 
			sErrorMessage = pcError ;
			delete pcError ;

			sErrorMessage = "(Source, Target) <" + sFileSource + "> , <" + sFileTarget + ">. Error: " + sErrorMessage;

			memset( strRetErr, 0, 64 );
			sprintf( strRetErr, " [%d/%d]", iNumRetries, iMaxRetries );
			sErrorMessage += strRetErr ;

			if( iNumRetries < iMaxRetries )
			{
				M4DOCSetWarning(M4DOCERROR_NATIVE, sErrorMessage.c_str());
			}
			else
			{
				M4DOCErrorHandling(M4DOCERROR_NATIVE, sErrorMessage.c_str());
			}
			
			// el error se revuelve el return del código LN4, no en la llamada a la función C
			ai_pvReturn.SetDouble(M4_ERROR); 
		} // copia
	}
	while( !b && iNumRetries < iMaxRetries );

	delete pwcFileSource ;

	return M4_SUCCESS; // Failure.

	
// UNIX
// ------------------
#elif defined(UNIX) || defined(_UNIX) || defined(SOLARIS)

	// variables
	const int blockSize=1024;
	m4int_t sizeRead=0;
	m4char_t buff[blockSize];
	
	// Open target file for writing. In the case that sFileTarget does not
	// exit, is NOT open. If target file exists, it has been opened for 
	// reading before. If target file does not exist, it has not been opened 
	// for reading before.
	// We check the target file is not already open and if it is the case,
	// open it.
	if (ptrFileTarget == NULL)
	{
		// UNICODE FILE
		eEncoding = M4UniANSI ;
		if ((ptrFileTarget = M4Fopen(sFileTarget.c_str(), M4UniWrite, eEncoding)) == NULL)
		{
			// Un error
			// antes de que se nos olvide el fichero origen lo cerramos
			fclose (ptrFileSource);

			sErrorMessage = "Target: <" + sFileTarget + ">";
			M4DOCErrorHandling(M4DOCERROR_FILENOEXISTS, sErrorMessage.c_str());
		
			// el error se revuelve el return del código LN4, no en la llamada a la función C
			ai_pvReturn.SetDouble(M4_ERROR); 
			return M4_SUCCESS; // Failure.
		}
	}

	//Copying loop.
	while(1)
	{
		//The source file is copied in blocks of blockSize size.
		if( (sizeRead=fread(buff,sizeof(char),blockSize,ptrFileSource)) < (blockSize) )
		{
			//If the 'if' above is true it means: a)the end of the file has been reached or b)there's been an error while reading.
			if(feof(ptrFileSource)) // a)Success, EOF reached.
			{
				//The last block is written.
				if( (fwrite(buff,sizeof(char),sizeRead,ptrFileTarget)) < (sizeRead) )
				{
					//There was an error while writing the last block.
					sErrorMessage = "(Source, Target) <" + sFileSource + "> , <" + sFileTarget + ">. ";
					M4DOCErrorHandling(M4DOCERROR_OTHER, sErrorMessage.c_str());										
					// cerramos todo
					fclose(ptrFileSource);
					fclose(ptrFileTarget);

					// el error se revuelve el return del código LN4, no en la llamada a la función C
					ai_pvReturn.SetDouble(M4_ERROR); 
					return M4_SUCCESS; 
				}
				else break;	//The last block was succesfully written.
			}
			else {
				// 	There's been an error.
				sErrorMessage = "(Source, Target) <" + sFileSource + "> , <" + sFileTarget + ">. ";
				M4DOCErrorHandling(M4DOCERROR_OTHER, sErrorMessage.c_str());		

				//There was an error while writing the last block.
				fclose(ptrFileSource);
				fclose(ptrFileTarget);

				// el error se revuelve el return del código LN4, no en la llamada a la función C
				ai_pvReturn.SetDouble(M4_ERROR); 
				return M4_SUCCESS; 
			}

		}
		else {
			//Block of size blockSize succesfully read.
			fwrite(buff,sizeof(char),blockSize,ptrFileTarget);
		}
	}

	// cerramos todo
	// cerramos los ficheros porque la copia la hace el SO
	if (ptrFileSource != NULL) {
		fclose (ptrFileSource);
	}

	if (ptrFileTarget != NULL) {
		fclose (ptrFileTarget);
	}

	// exito en LN4
	ai_pvReturn.SetDouble(M4_SUCCESS);
	return M4_SUCCESS;
#endif
}
/* **************************************************************************** */


