//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4printpdf
// File:                m4printpdfres.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                28-10-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the error numbers.
//
//
//==============================================================================



#include "m4res.hpp"



#ifndef	__M4PRINTPDFRES_HPP__
#define	__M4PRINTPDFRES_HPP__


// "Error verificando los argumentos de entrada del proceso de impresi�n de documentos. Revise que estos argumentos son correctos."
#define M4PRINT_PDF_ERROR_BAD_ARGUMENTS						0x0001 + M4PRINTPDFERROR_BASE
// "Error imprimiendo fichero. No se ha podido imprimir el documento "%0:s". \nDebe consultar con el administrador."
#define M4PRINT_PDF_ERROR_PRINT_DOCUMENT					0x0002 + M4PRINTPDFERROR_BASE
// "Error ejecutando m�todo "%0:s". System error: "%1:s". \nDebe consultar con el administrador."
#define M4PRINT_PDF_ERROR_SHELLEXECUTE						0x0003 + M4PRINTPDFERROR_BASE
// "Funcionalidad s�lo implementada para plataforma Windows. \nDebe consultar con el administrador."
#define M4PRINT_PDF_ERROR_UNIX_NOT_SUPPORTED				0x0004 + M4PRINTPDFERROR_BASE
// "Error al intentar averiguar el n�mero de instancias concurrentes durante la impresi�n de la copia "%0:d". No se ha podido determinar el n�mero de documentos que se est�n imprimiendo concurrentemente. \nDebe consultar con el administrador."
#define M4PRINT_PDF_ERROR_GET_PRINT_DOC_NUM					0x0005 + M4PRINTPDFERROR_BASE
// "El argumento que especifica el n�mero de copias concurrentes tiene un valor no v�lido, "%0:d". El n�mero de copias concurrentes permitido no puede ser menor de 1. \nSe asumir� como valor para el n�mero de copias concurrentes "%1:d"."
#define M4PRINT_PDF_ERROR_CONCURRENT_COPIES_NUM				0x0006 + M4PRINTPDFERROR_BASE
// "Error al intentar averiguar el nombre del programa para imprimir el documento, "%0:s". No se ha podido encontrar el nombre del programa que se utilizar� paara imprimir el documento especificado. \nDebe consultar con el administrador."
#define M4PRINT_PDF_ERROR_PROGRAM_NAME						0x0007 + M4PRINTPDFERROR_BASE
// "Se ha superado el tiempo m�ximo de consulta al spool de la impresora para el documento, "%0:s", en la copia "%1:d". \nEste proceso se terminar�, verifique que se haya terminado de imprimir."
#define M4PRINT_PDF_WIN_SPOOL_MAX_NUM						0x0008 + M4PRINTPDFERROR_BASE

//"N�mero de argumentos incorrecto.\n \nError verificando los argumentos de entrada del proceso de descarga de documentos. Se ha llamado a la funci�n "%0:s" con "%1:n" argumentos y deber�a llamarse con "%2:n" argumentos."
#define M4PRINT_DOWNLOAD_ERROR_BAD_NUMBER_ARGUMENTS			0x0009 + M4PRINTPDFERROR_BASE
//"Tipo de argumento incorrecto.\n \nEl argumento "%0:s" de la funcion "%1:s" es de tipo "%2:s" y deber�a ser de tipo "%3:s"."
#define M4PRINT_DOWNLOAD_ERROR_BAD_ARGUMENT_TYPE			0x000A + M4PRINTPDFERROR_BASE
//"Argumento vacio.\n \nEl argumento "%0:s" de la funcion "%1:s" contiene una cadena vacia.\nDebe introducir un valor v�lido."
#define M4PRINT_DOWNLOAD_ERROR_NULL_ARGUMENT				0x000B + M4PRINTPDFERROR_BASE
//"Error descargando fichero.\n \nSe ha producido un error al ejecutar la funcion de descarga del sistema. El sistema devuelve el error: "%0:s". "
#define M4PRINT_DOWNLOAD_ERROR_DOWNLOAD_SYSTEM				0x000C + M4PRINTPDFERROR_BASE
//"Error imprimiendo fichero "%0:s".\n \nExisten aplicaciones Acrobat ejecut�ndose actualmente en el sistema. Debe cerrar todas las aplicaciones Acrobat para que se pueda imprimir el documento correctamente."
#define M4PRINT_ACROBAT_CURRENTLY_RUNNING						0x000D + M4PRINTPDFERROR_BASE
//"Error al intentar establecer una conversaci�n con el servidor DDE. \nError al conectarse al servicio "%0:s" y tema "%1:s".\nDebe consultar con el administrador."
#define M4PRINT_PDF_ERROR_DDE_CONNECT						0x000E + M4PRINTPDFERROR_BASE
//"La versi�n instalada de Acrobat no est� soportada. \nPara poder imprimir el documento, debe utilizar la opci�n de men� de Acrobat."
#define M4PRINT_PDF_ERROR_ACROBAT_VERSION_NOT_SUPPORTED		0x000F + M4PRINTPDFERROR_BASE

#endif



