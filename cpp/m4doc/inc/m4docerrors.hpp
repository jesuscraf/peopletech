//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc.dll
// File:             errors.hpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             30-August-1999
// Language:         C++
// Operating System:  
// Design Document:  
//
// Definition:
//
//	This module provides:
//		-the error ids for the M4DOC.DLL library error handling.
//		-the error function headers for the M4DOC.DLL library error handling.
//==============================================================================


#ifndef __HPP_M4DOC_ERRORS__
#define __HPP_M4DOC_ERRORS__ 1


//Global error type identifiers.
#define M4DOCERROR_BASE						0x780000

//"Cannot Allocate memory"
#define M4DOCERROR_MEMORY					M4DOCERROR_BASE + 1
//"Can´t Load library"
#define M4DOCERROR_LIBRARY					M4DOCERROR_BASE + 2
//"Cannot Get Process Address"
#define M4DOCERROR_PROCADDRSSS				M4DOCERROR_BASE + 3
//"The function of the following DLL returned an error: %s"
#define M4DOCERROR_EXTERNDLL				M4DOCERROR_BASE + 4
//"Invalid arguments passed in function %0:s"
#define M4DOCERROR_CHECKNULLS				M4DOCERROR_BASE + 5
//"Cannot create unique temporary file name"
#define M4DOCERROR_UNIQUENAME				M4DOCERROR_BASE + 6
//"File %0:s cannot be accessed. Might not exist, be in use or be invalid."
#define M4DOCERROR_FILENOEXISTS				M4DOCERROR_BASE + 7
// "Cannot modify file permissions on file %0:s"
#define M4DOCERROR_PERMIT					M4DOCERROR_BASE + 8
// "Cannot delete file %0:s"
#define M4DOCERROR_DELETEFILE				M4DOCERROR_BASE + 9
// "Undetermined error: %0:s"
#define M4DOCERROR_OTHER					M4DOCERROR_BASE + 10
// "Native error message: %0:s"
#define M4DOCERROR_NATIVE					M4DOCERROR_BASE + 11
// "Cannot copy file: %0:s"
#define M4DOCERROR_CPFIL					M4DOCERROR_BASE + 12
// "Se ha superado el número máximo de estructras de búsqueda abiertas."
#define M4DOCERROR_MAX_NUM_EXT_SEARCH		M4DOCERROR_BASE + 13
// "El handle a búsqueda extendida %0:s no es válido."
#define M4DOCERROR_EXTSEARCH_HDL_NO_VALID	M4DOCERROR_BASE + 14
// "Se ha producido un error al leer el fichero %0:s"
#define M4DOCERROR_READING_FILE				M4DOCERROR_BASE + 15
// "El path %0:s no existe."
#define M4DOCERROR_NO_VALID_PATH			M4DOCERROR_BASE + 16
// "No se puede borrar el directorio %0:s. No existe o no está vacio."
#define M4DOCERROR_DIR_NOT_EMPTY			M4DOCERROR_BASE + 17
// "Función no soportada en plataforma actual.\n \nLa función "%0:s" invocada no se encuentra disponible en la plataforma actual. consulte con su administrador."
#define M4DOCERROR_NOT_SUPPORTED			0x001A + M4DOCERROR_BASE
// "Error al iniciar navegador web por defecto.\n \nSe produjo el error "%0:d - %1:s" mientras se iniciaba el navegador web por defecto. consulte con su administrador."
#define M4DOCERROR_DEF_BROWSER				0x001B + M4DOCERROR_BASE

#define M4DOCERROR_BAD_PARAM_NUMBER			M4DOCERROR_BASE + 3780
// Número de argumentos incorrecto.\n \nEl método "%0:s" admite "%1:d" argumentos y el número de argumentos actuales es "%2:s".\nDebe revisar su Meta4Object.
#define	M4DOCERROR_BAD_PARAM_TYPE			M4DOCERROR_BASE + 3781
// Tipo de argumento incorrecto.\n \nEl argumento "%0:s" del método "%1:s" es de tipo "%2:s" y debería ser de tipo "%3:s".\nDebe revisar su Meta4Object.
#define	M4DOCERROR_NO_SUCH_ITEM				M4DOCERROR_BASE + 3782
// No existe el elemento.\n \nDebe definir el elemento "%0:s" o no utilizarlo.\nEsto sucedió en la función "%1:s".
#define M4DOCERROR_ITEM_TYPE_MISMATCH		M4DOCERROR_BASE + 3783
// Tipo incorrecto.\n \nEl tipo del elemento "%0:s" es "%1:s" y debería ser "%2:s".\nEsto sucedió en la función "%3:s".
#define M4DOCERROR_NO_OPEN_FILE				M4DOCERROR_BASE + 3784
// No se ha podido abrir el archivo.\n \nNo se ha podido abrir el archivo "%0:s" en modo "%1:s" por la razón "%2:s".\nDebe consultar con el administrador.\nEsto sucedió en la función "%3:s".
#define M4DOCERROR_NO_WRITE_FILE			M4DOCERROR_BASE + 3785
// No se ha podido escribir en el archivo.\n \nNo se ha podido escribir en el archivo "%0:s" por la razón "%1:s".\nDebe consultar con el administrador.\nEsto sucedió en la función "%2:s".
#define M4DOCERROR_NO_SET_VALUE				M4DOCERROR_BASE + 3786
// Asignación de elemento incorrecta.\n \nError al establecer el valor de "%0:s"\nDebe consultar otros mensajes de error.\nEsto sucedió en la función "%1:s".
#define M4DOCERROR_NO_UTF16_FORMAT			M4DOCERROR_BASE + 3787
// Formato de datos incorrecto.\n \nEl formato del elemento "%0:s" no es UTF-16\nDebe consultar con el administrador.\nEsto sucedió en la función "%1:s".
#define M4DOCERROR_ZIP_NO_MEMORY			M4DOCERROR_BASE + 3788
// Error procesando fichero zip.\n \nNo se puede alocar memoria\nDebe consultar con el administrador.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_ENTRY				M4DOCERROR_BASE + 3789
// Error procesando item zip.\n \nNo se puede crear la entrada "%1:s" en el archivo zip.\nDebe consultar con el administrador.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_ENTRY_OPEN			M4DOCERROR_BASE + 3790
// Error procesando item zip.\n \nNo se puede abrir el archivo "%1:s" para lectura.\nDebe consultar con el administrador.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_ENTRY_READ			M4DOCERROR_BASE + 3791
// Error procesando item zip.\n \nNo se puede leer el archivo "%1:s".\nDebe consultar con el administrador.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_ENTRY_WRITE			M4DOCERROR_BASE + 3792
// Error procesando item zip.\n \nNo se puede escribir en el archivo "%1:s".\nDebe consultar con el administrador.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_OPEN					M4DOCERROR_BASE + 3793
// Error procesando fichero zip.\n \nNo se puede abrir el archivo "%1:s".\nDebe consultar con el administrador.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_CLOSE				M4DOCERROR_BASE + 3794
// Error procesando fichero zip.\n \nNo se puede cerrar el archivo "%1:s".\nDebe consultar con el administrador.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_NO_CHANGE_DIR			M4DOCERROR_BASE + 3795
// Error procesando fichero zip.\n \nNo se puede cambiar al directorio "%1:s".\nEl directorio debe existir.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_GET_INFO				M4DOCERROR_BASE + 3796
// Error procesando fichero zip.\n \nError abriendo zip file. Código de error "%1:s".\nDebe comprobar el fichero zip.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_GET_NEXT_FILE		M4DOCERROR_BASE + 3797
// Error procesando fichero zip.\n \nError procesando la siguiente fichero comprimido. Código de error "%1:s".\nDebe comprobar el fichero zip.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_ENTRY_NOT_FOUND		M4DOCERROR_BASE + 3798
// Error procesando fichero zip.\n \nLa entrada con nombre "%1:s" no ha sido encontrado en el fichero zip.\nDebe comprobar el parámetro.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_FILE_WRITE			M4DOCERROR_BASE + 3799
// Error procesando fichero zip.\n \nNo se ha podido escribir el fichero "%1:s" expandiendo el fichero zip.\nDebe comprobar el directorio destino.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_READ_ENTRY			M4DOCERROR_BASE + 3800
// Error procesando fichero zip.\n \nNo se ha podido leer la entrada del fichero zip. Código de error "%1:s".\nDebe comprobar el fichero zip.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_PASSWORD				M4DOCERROR_BASE + 3801
// Error procesando fichero zip.\n \nNo se ha podido desencriptar la entrada con la clave suministrada. Código de error "%1:s".\nDebe comprobar el fichero zip o la clave.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_CREATE_DIR			M4DOCERROR_BASE + 3802
// Error procesando fichero zip.\n \nNo se ha podido crear el directorio "%1:s".\nDebe comprobar el parámetro o permisos.\nEsto sucedió en la función "%0:s".
#define M4DOCERROR_ZIP_CLOSE_FILE			M4DOCERROR_BASE + 3803
// Error procesando fichero zip.\n \nNo se ha podido cerrar el fichero. Código de error "%1:s".\nDebe consultar con el administrador.\nEsto sucedió en la función "%0:s".
#define	M4DOCERROR_M4OBJECT_NOT_FOUND		M4DOCERROR_BASE + 3804
//"La instancia de Meta4Object "%1:d" no existe.\nEsto sucedió en la función "%0:s"
#define	M4DOCERROR_NODE_NOT_FOUND			M4DOCERROR_BASE + 3805
//"No existe el nodo "%1:s" del Meta4Object "%2:s".\nEsto sucedió en la función "%0:s"
#define	M4DOCERROR_ADD_REGISTER				M4DOCERROR_BASE + 3806
//"Se ha producido un error al añadir un registro en el nodo "%1:s".\nEsto sucedió en la función "%0:s"
#define M4DOCERROR_UNZIP_PATH_TOO_LONG		M4DOCERROR_BASE + 3827
//"Error procesando fichero zip.\n \nLa ruta de los archivos resultado de la descompresión es demasiado larga.\nDebe especificar como destino de los archivos descomprimidos una ruta más corta del sistema de archivos.\nEsto sucedió en la función "%0:s"."
#define M4DOCERROR_DWNLD_BAD_ARGS			M4DOCERROR_BASE + 3828
//"Error descargando archivo.\n \Argumentos incorrectos en función de desarga de archivos.\nDebe consultar con el administrador."
#define M4DOCERROR_DWNLD_INSUFF_BUFFER		M4DOCERROR_BASE + 3829
//"Error descargando archivo.\n \nNo se ha descargado el archivo "%0:s" porque la memoria suministrada es insuficiente.\nDebe consultar con el administrador."
#define M4DOCERROR_DWNLD_BAD_URL			M4DOCERROR_BASE + 3830
//"Error descargando archivo.\n \nNo se ha descargado el archivo "%0:s" porque el formato de la URL no es el esperado.\nDebe consultar con el administrador."
#define M4DOCERROR_DWNLD_BUILD_TGT_PATH		M4DOCERROR_BASE + 3831
//"Error descargando archivo.\n \nNo se ha descargado el archivo "%0:s" porque no se ha podido construir la ruta de destino.\nDebe consultar con el administrador."
#define M4DOCERROR_DWNLD_SYS_ERROR			M4DOCERROR_BASE + 3832
//"Error descargando archivo.\n \nSe ha producido el error "%0:d" durante la llamada a la función "%1:s": "%2:s".\nDebe consultar con el administrador."
#define M4DOCERROR_DWNLD_OPEN_FILE_ERROR	M4DOCERROR_BASE + 3833
//"Error descargando archivo.\n \nSe ha producido el error "%0:s" al copiar el archivo en disco.\nDebe consultar con el administrador."
#define M4DOCERROR_DWNLD_EMPTY_FILE			M4DOCERROR_BASE + 3834
//"Error descargando archivo.\n \nEl archivo a descargar "%0:s" está vacío. Probablemente la dirección no sea correcta.\nDebe consultar con el administrador."
#define M4DOCERROR_DWNLD_NOT_SUCCESS		M4DOCERROR_BASE + 3839
//"Error descargando archivo.\n \nNo se ha podido descargar el archivo "%0:s" por el motivo (%1:d) %2:s.\nDebe consultar con el administrador."
// Error handling functions.
#ifdef __cplusplus

#include "m4types.hpp"

m4void_t M4DOCErrorHandling(m4int_t error_type, m4pcchar_t add_error_message);

#endif // #ifdef __cplusplus

#endif


