//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             Platform.hpp
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
//		This module defines the actual functionality for printing management
//		on every platform	
//
//==============================================================================


#include "m4types.hpp"


//==============================================================================
// En este modulo se implementa el envio de datos procedentes de la libreria
// engindll.dll a una impresora ai_szPrinterName. El envio es dependiente de
// plataforma, por lo cual se ha implementado un funcion para cada arquitec-
// tura
//==============================================================================


#ifdef _WINDOWS
	
	//Mediante esta funcion podemos enviar datos especificos de impresion (RAW, PCL, ESC, etc)
	// a una impresora de forma directa, evitando hacer uso de los drivers especificos de la
	// impresora. La implementacion hace uso del API Win32, que proporciona una funcionalidad
	// aplicable a impresoras locales y en red. Para ello debemos pasar como parametros el
	// nombre del dispositivo (path completo en red) y el numero de bytes a imprimir ai_dwCount,
	// puesto que a la salida se comprueba que efectivamente se ha imprimido el numero de bytes
	// correcto.

	m4bool_t Win32_SendsDataToPrinter(LPSTR ai_szPrinterName, m4char_t* ai_pcDataFile);

#elif defined _UNIX
	
	//Fncion implementada para Unix-Solaris. Su funcionamiento es sencillo, puesto que al final
	// realizamos una llamada al sistema. Construimos una cadena estatica que almacenara el 
	// el nombre del dispositivo (path completo de la impresora) y la ruta del fichero	a imprimir,
	// generado por el engine. Segun el tipo de datos a imprimir, la cadena contendra el comando
	// adecuado. A la salida se comprueba que no se machaca memoria

	m4bool_t UNIX_SendsDataToPrinter(m4char_t* ai_szPrinterName, m4char_t* ai_szDataFilePath);

#endif

