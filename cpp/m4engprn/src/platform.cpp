//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             Platform.cpp
// Project:			 M4EngPrn.dll
// Author:           Meta Software M.S. , S.A
// Date:			 16/07/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//  
// Propietary:       Manuel Lazcano Perez
// Modifications: 	 Who && Date				
//
// Definition:
//
//	  This module implements the actual functionality for each and every 
//	  platform.
//
//==============================================================================

#include <stdlib.h>
#include <string.h>

#include "m4win.hpp"
#include "platform.hpp"
#include "clfile.h"


//==============================================================================

#ifdef _WINDOWS

	m4bool_t Win32_SendsDataToPrinter( LPSTR ai_szPrinterName, m4char_t* ai_pcDataFile )
	{
		HANDLE		hPrinter ;			//Handle del dispositivo
		DOC_INFO_1  DocInfo ;			//Struct que almacena informacion del documento 
		DWORD		dwJob ;				//entero unsigned de 32-bit que almacena la direccion del Job
		DWORD		dwBytesWritten ;	//Almacena el numero de bytes impresos
	
		M4ClFile	oFile ;
		size_t		szReadBytes ;
		m4bool_t	SwOk=M4_TRUE ;
		m4uchar_t	upcBuffer[65536] ;

		//Necesitamos un handle del dispositivo (impresora)
		if( !OpenPrinter( ai_szPrinterName, &hPrinter, NULL )){
			return M4_FALSE ;
		}

		//Almacenamos la informacion del documento
		DocInfo.pDocName = "Printing Report" ;
		DocInfo.pOutputFile = NULL ;
		DocInfo.pDatatype = "RAW";

		//Le indicamos al spooler que el job va a iniciarse
		if(( dwJob = StartDocPrinter( hPrinter, 1, (LPBYTE)&DocInfo )) ==0 ){
			ClosePrinter( hPrinter ) ;
			return M4_FALSE ;
		}

		//Empieza una pagina
		if( !StartPagePrinter( hPrinter ) ){
			EndDocPrinter( hPrinter ) ;
			ClosePrinter( hPrinter ) ;
			return M4_FALSE ;
		}
		
		//Abrimos fichero y enviamos datos a la impresora por bloques
		if(oFile.Open( ai_pcDataFile, M4ClFile::EXIST, M4ClFile::READ ) ){
			do{
 				if ( (szReadBytes = oFile.R( upcBuffer, 1, 65536 ))==0 ) 
					break ;
				if( !WritePrinter( hPrinter, upcBuffer, szReadBytes, &dwBytesWritten ) ){
					EndPagePrinter( hPrinter ) ;
					EndDocPrinter( hPrinter ) ;
					ClosePrinter( hPrinter );
					return M4_FALSE ;
				}
			} while(szReadBytes==65536);
		};

		//Finaliza pagina
		if( !EndPagePrinter( hPrinter ) ){
			EndDocPrinter( hPrinter ) ;
			ClosePrinter( hPrinter ) ;
			return M4_FALSE ;
		}

		//Informamos al spooler que el documento ha finalizado
		if( !EndDocPrinter( hPrinter ) ){
			ClosePrinter( hPrinter ) ;
			return M4_FALSE ;
		}

		//Limpiamos el handle del dispositivo
		ClosePrinter( hPrinter ) ;

		//Si todo ha ido bien...
		return M4_TRUE ;
	}

#elif defined _UNIX
//Version para Sun-Solaris 

	m4bool_t UNIX_SendsDataToPrinter(m4char_t* ai_szPrinterName, m4char_t* ai_szDataFilePath)
	{
		m4char_t szCommand[10000];

		sprintf(szCommand,"m4lp.ksh '%s' '%s'", ai_szPrinterName,  ai_szDataFilePath);

		//Llamada al sistema con la cadena de comando
		system(szCommand);

		//Comprueba que no se pisa memoria
		assert(strlen(szCommand)<=9999);

		return M4_TRUE;
	}

#endif
