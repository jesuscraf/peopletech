//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java
// File:                m4javaapires.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                12/01/2009
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


#ifndef	__M4JAVAAPIRES_HPP__
#define	__M4JAVAAPIRES_HPP__


//"Número de argumentos incorrecto.\n \nError verificando los argumentos de entrada del proceso de generación de clientes SOAP. Se ha llamado a la función "%0:s" con "%1:n" argumentos y debería llamarse con "%2:n" argumentos."
#define M4JAVAAPI_ERROR_BAD_NUMBER_ARGUMENTS			0x0001 + M4JAVAAPIERROR_BASE
//"Tipo de argumento incorrecto.\n \nEl argumento "%0:s" de la funcion "%1:s" es de tipo "%2:s" y debería ser de tipo "%3:s"."
#define M4JAVAAPI_ERROR_BAD_ARGUMENT_TYPE				0x0002 + M4JAVAAPIERROR_BASE
//"Argumento vacio.\n \nEl argumento "%0:s" de la funcion "%1:s" contiene una cadena vacia.\nDebe introducir un valor válido."
#define M4JAVAAPI_ERROR_NULL_ARGUMENT					0x0003 + M4JAVAAPIERROR_BASE
//"No se ha podido crear objeto Java.\n \nSe han producido errores al crear objeto para la clase Java "%0:s".\nDebe consultar con el administrador."
#define M4JAVAAPI_ERROR_CANNOT_CREATE_OBJECT			0x0004 + M4JAVAAPIERROR_BASE
//"No se ha podido ejecutar método Java.\n \nSe han producido errores al invocar al método Java "%0:s".\nDebe consultar con el administrador."
#define M4JAVAAPI_ERROR_CANNOT_INVOKE_METHOD			0x0005 + M4JAVAAPIERROR_BASE
//"No se ha podido destruir objeto Java.\n \nSe han producido errores al destruir el objeto Java con handle "%0:d".\nDebe consultar con el administrador."
#define M4JAVAAPI_ERROR_CANNOT_DESTROY_OBJECT			0x0006 + M4JAVAAPIERROR_BASE
//"No se ha podido obtener el handle del M4Object.\n \nSe han producido errores al intentar obtener el handle del M4Object.\nDebe consultar con el administrador."
#define M4JAVAAPI_ERROR_CANNOT_GET_CHANNEL_HANDLE		0x0007 + M4JAVAAPIERROR_BASE
//"No se ha podido obtener la lista de ficheros jar.\n \nSe han producido errores al intentar obtener la lista de ficheros jar.\nDebe consultar con el administrador."
#define M4JAVAAPI_ERROR_CANNOT_GET_JAR_FILES			0x0008 + M4JAVAAPIERROR_BASE
//"No se puede inicializar la máquina virtual de Java\n \nVerifique su parametrización."
#define M4JAVAAPI_ERROR_JVM_NO_INIT						0x0009 + M4JAVAAPIERROR_BASE
//"Se ha producido una excepción Java.\n \n La excepción Java es: "%0:s"."
#define M4JAVAAPI_ERROR_JNI_EXCEPTION					0x000A + M4JAVAAPIERROR_BASE

//"Error interno.\n \nDebe consultar con el administrador.\nEsto ha sucedido en la línea "%0:s" del fichero "%1:s"."
#define M4JAVA_INTERNAL_ERROR							0x00A0 + M4JAVAAPIERROR_BASE
//"Argumento nulo a una función.\n \nEl argumento "%0:s" del método "%1:s" es nulo.\nDebe consultar con el administrador.\nEsto sucedió en la línea "%2:s" del archivo "%3:s"."
#define M4JAVA_NULL_ARGUMENT							0x00A1 + M4JAVAAPIERROR_BASE
//"Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicación.\nEsto ha sucedido en la línea "%1:s" del archivo "%2:s"."
#define M4JAVA_NO_MEMORY								0x00A2 + M4JAVAAPIERROR_BASE
//"No se ha podido inicializar el proceso java.\n \nSe ha producido el error "%0:s" al inicializar el proceso java "%1:s"."
#define M4JAVA_OPEN_PROCESS_ERROR						0x00A3 + M4JAVAAPIERROR_BASE
//"No se ha podido enviar datos al proceso java.\n \nSe ha producido el error "%0:s" al enviar datos al proceso java.\nPosiblemente el proceso java ya no está disponible."
#define M4JAVA_SEND_DATA_ERROR							0x00A4 + M4JAVAAPIERROR_BASE
//"No se ha podido recibir datos del proceso java.\n \nSe ha producido el error "%0:s" al recibir datos del proceso java.\nPosiblemente el proceso java ya no está disponible."
#define M4JAVA_READ_DATA_ERROR							0x00A5 + M4JAVAAPIERROR_BASE
// No se usa.
#define M4JAVA_NOT_USED_1								0x00A6 + M4JAVAAPIERROR_BASE
//"No hay más handles de objetos java disponibles.\n \nDebe consultar al administrador."
#define M4JAVA_NO_MORE_HANDLES							0x00A7 + M4JAVAAPIERROR_BASE
//"No existe el objeto java.\n \nEl handle "%0:s" no corresponde a un objeto java."
#define M4JAVA_BAD_HANDLE								0x00A8 + M4JAVAAPIERROR_BASE
//"No existe el objeto java.\n \nEl handle "%0:s" no corresponde a un objeto java de este entorno."
#define M4JAVA_NO_LOCAL_HANDLE							0x00A9 + M4JAVAAPIERROR_BASE
//"No existe el objeto java.\n \nEl handle "%0:s" corresponde a un objeto java que posiblemente ya ha sido borrado."
#define M4JAVA_DELETED_HANDLE							0x00AA + M4JAVAAPIERROR_BASE
//"Se ha producido un error al crear un objeto java.\n \nNo se ha podido crear un objeto java de la clase "%0:s".\nEsto sucedió en la línea "%1:s" del código de "%2:s" de "%3:s" de "%4:s"."
#define M4JAVA_CREATE_OBJECT_ERROR						0x00AB + M4JAVAAPIERROR_BASE
//"Se ha producido un error al borrar un objecto java.\n \nNo se ha podido borrar el objeto java "%0:s" de la clase "%1:s".\nEsto sucedió en la línea "%2:s" del código de "%3:s" de "%4:s" de "%5:s"."
#define M4JAVA_DESTROY_OBJECT_ERROR						0x00AC + M4JAVAAPIERROR_BASE
//"Se ha producido un error al borrar un objecto java.\n \nNo se ha podido borrar el objeto java "%0:s".\nEsto sucedió en la línea "%1:s" del código de "%2:s" de "%3:s" de "%4:s"."
#define M4JAVA_DESTROY_OBJECT_ERROR_NO_CLASS			0x00AD + M4JAVAAPIERROR_BASE
//"Se ha producido un error al invocar un método java.\n \nHa fallado la invocación al método java "%0:s" del objeto "%1:s" de la clase "%2:s".\nEsto sucedió en la línea "%3:s" del código de "%4:s" de "%5:s" de "%6:s"."
#define M4JAVA_OBJECT_METHOD_ERROR						0x00AE + M4JAVAAPIERROR_BASE
//"Se ha producido un error al invocar un método java.\n \nHa fallado la invocación al método java "%0:s" del objeto "%1:s".\nEsto sucedió en la línea "%2:s" del código de "%3:s" de "%4:s" de "%5:s"."
#define M4JAVA_OBJECT_METHOD_ERROR_NO_CLASS				0x00AF + M4JAVAAPIERROR_BASE
//"Se ha producido un error al invocar un método java estático.\n \nHa fallado la invocación al método java estático "%0:s" de la clase "%1:s".\nEsto sucedió en la línea "%2:s" del código de "%3:s" de "%4:s" de "%5:s"."
#define M4JAVA_CLASS_METHOD_ERROR						0x00B0 + M4JAVAAPIERROR_BASE
//"No se ha podido enviar datos al proceso java.\n \nSe ha producido el error "%0:s" al enviar datos al proceso java.\nPosiblemente el proceso java no se ha inicializado correctamente."
#define M4JAVA_SEND_DATA_FIRST_TIME_ERROR				0x00B1 + M4JAVAAPIERROR_BASE
// No se puede crear el directorio de archivos JAR.\n \nNo se ha podido crear el directorio "%0:s" por la razón "%1:s".\nLas aplicaciones java no estarán disponibles.
#define M4JAVA_BAD_JAR_DIRECTORY						0x00B2 + M4JAVAAPIERROR_BASE
// Modo java no permitido.\n \nEl modo java in process no está activado.\nLa ejecución java se realizará en modo out process.
#define M4JAVA_IN_PROCESS_NOT_ALLOWED					0x00B3 + M4JAVAAPIERROR_BASE
// Ejecución java cancelada.\n \nSe ha cancelado la ejecución del método java.
#define M4JAVA_CANCELED									0x00B4 + M4JAVAAPIERROR_BASE
// Ejecución java demasiado compleja.\n \nSe ha cancelado la ejecución del método java porque su ejecución ha sobrepasado los "%0:d" segundos.
#define M4JAVA_COMPLEX_EXECUTION						0x00B5 + M4JAVAAPIERROR_BASE

// M4O SCH_DOCUMENT_EDITOR.
//"El estado del documento no es válido.\n \Compruebe que el documento ha sido abierto antes de realizar acciones sobre el mismo."
#define M4JAVAAPI_ERROR_INVALID_DOCUMENT_STATUS			0x00C8 + M4JAVAAPIERROR_BASE
//"No se ha podido abrir el documento "%0:s".\n \nCompruebe que no exista un documento previamente abierto."
#define M4JAVAAPI_ERROR_CANNOT_OPEN_DOCUMENT			0x00C9 + M4JAVAAPIERROR_BASE

#endif

