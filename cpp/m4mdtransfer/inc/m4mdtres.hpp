//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4metadatatransfer
// File:                m4mdtres.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                31-03-2004
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



#ifndef	__M4MDTRES_HPP__
#define	__M4MDTRES_HPP__



#define	M4MDTERROR_NO_MEMORY							0x0001 + M4MDTERROR_BASE
// Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicaci�n.
#define M4MDTERROR_INTERNAL_ERROR						0x0002 + M4MDTERROR_BASE
// Error interno.\n \nSe ha producido un error en la funci�n "%0:n".\nDebe consultar con el administrador.\nInformaci�n adicional "%1:s".
#define	M4MDTERROR_BAD_COLUMN							0x0003 + M4MDTERROR_BASE
// No existe la columna.\n \nNo se ha podido acceder a la columna "%0:s" porque s�lo hay "%1:s" columnas.\nDebe consultar con el administrador.
#define	M4MDTERROR_BAD_ROW								0x0004 + M4MDTERROR_BASE
// No existe la fila.\n \nNo se puede acceder a la fila "%0:s" porque s�lo hay "%1:s" filas.\nDebe consultar con el administrador.
#define	M4MDTERROR_INIT_ROW								0x0005 + M4MDTERROR_BASE
// Error al inicializar la fila.\n \nSe ha producido un error al inicializar la fila con "%0:s" elementos.\nDebe consultar con el administrador.
#define	M4MDTERROR_NO_FIELD_DESCRIPTION_BY_COLUMN		0x0006 + M4MDTERROR_BASE
// No existe la descripci�n del campo.\n \nNo se ha podido encontrar la descripci�n del campo de la columna "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_ADD_FIELD_DESCRIPTION				0x0007 + M4MDTERROR_BASE
// Error a�adiendo descripci�n de campo.\n \nNo se ha podido a�adir la descripci�n del campo "%0:s" al mapa de descripci�n de campos.\nDebe consultar con el administrador.
#define	M4MDTERROR_INIT_CONNECTION						0x0008 + M4MDTERROR_BASE
// Error inicializando conexi�n.\n \nNo se ha podido inicializar la conexi�n "%0:s".\nDebe revisar la cadena de conexi�n.
#define	M4MDTERROR_CLOSE_CONNECTION						0x0009 + M4MDTERROR_BASE
// Error cerrando conexi�n.\n \nNo se ha podido cerrar la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_ADD_CONNECTION						0x000A + M4MDTERROR_BASE
// Error a�adiendo conexi�n.\n \nNo se ha podido a�adir la conexi�n "%0:s" al mapa de conexiones.\nDebe consultar con el administrador.
#define	M4MDTERROR_NO_CONNECTION						0x000B + M4MDTERROR_BASE
// No existe la conexi�n.\n \nNo se ha podido acceder a la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_OPEN_CURSOR							0x000C + M4MDTERROR_BASE
// Error al abrir cursor.\n \nNo se ha podido abrir un cursor para la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_PREPARE_STATEMENT					0x000D + M4MDTERROR_BASE
// Error al preparar sentencia para la conexi�n "%0:s".\n \nNo se ha podido preparar la sentencia "%1:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_BIND_PARAM							0x000E + M4MDTERROR_BASE
// Error al bindar par�metro.\n \nNo se ha podido bindar el par�metro "%0:s" con el valor "%1:s", tipo "%2:s", precisi�n "%3:s" y escala "%4:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_INIT_CURSOR							0x000F + M4MDTERROR_BASE
// Error al inicializar el cursor.\n \nNo se ha podido inicializar el cursor de la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_EXECUTE_STATEMENT					0x0010 + M4MDTERROR_BASE
// Error al ejecutar sentencia.\n \nNo se ha podido ejecutar la sentencia "%0:s" sobre la conexi�n "%1:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_CLOSE_CURSOR							0x0011 + M4MDTERROR_BASE
// Error al cerrar cursor.\n \nNo se ha podido cerrar el cursor abierto sobre la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_BIND_COLUMN							0x0012 + M4MDTERROR_BASE
// Error al bindar columna para la conexi�n "%0:s".\n \nNo se ha podido bindar la columna "%1:s" con tipo "%2:s", precisi�n "%3:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_FETCH								0x0013 + M4MDTERROR_BASE
// Error al realizar SQLFecth.\n \nNo se han podido recuperar los datos para la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_GET_DATA								0x0014 + M4MDTERROR_BASE
// Error al realizar SQLGetData.\n \nNo se han podido recuperar los datos de tipo LONG para la columna "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_ODBC									0x0015 + M4MDTERROR_BASE
// Se ha producido un error de ODBC.\n \nEl c�digo del error es "%0:s" y el mensaje del error es "%1:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_NO_BEGIN_TRANSACTION					0x0016 + M4MDTERROR_BASE
// Error al abrir transacci�n.\n \nNo se ha podido abrir una transacci�n sobre la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_NO_COMMIT							0x0017 + M4MDTERROR_BASE
// Error al cerrar transacci�n.\n \nNo se ha podido hacer commit de la transacci�n sobre la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_NO_ROLLBACK							0x0018 + M4MDTERROR_BASE
// Error al cerrar transacci�n.\n \nNo se ha podido hacer rollback de la transacci�n sobre la conexi�n "%0:s".\nDebe consultar con el administrador.
#define	M4MDTERROR_NO_SUCH_PARAM_TYPE					0x0019 + M4MDTERROR_BASE
// Error en el tipo del par�metro especificado.\n \nNo se ha podido establecer el tipo del par�metro "%0:d" al valor "%1:d".\nDebe consultar con el administrador.


//=================================================================================
// Errores de los par�metros
//=================================================================================

#define M4MDTERROR_NO_SUCH_PARAM						0x0100 + M4MDTERROR_BASE
// No existe el par�metro.\n \nNo existe el par�metro dado por el handle "%0:n".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_SUCH_PARAM_VALUE					0x0101 + M4MDTERROR_BASE
// No existe el valor del par�metro.\n \nNo existe el valor dado por la fila "%0:n" para el par�metro dado por el handle "%2:n".\nEl n�mero m�ximo de filas es "%1:n".\nDebe consultar con el administrador.
#define M4MDTERROR_PARAM_VALUE_NO_STRING				0x0102 + M4MDTERROR_BASE
// El valor del par�metro no es de tipo cadena.\n \nSe est� intentando usar como cadena el valor dado por la fila "%0:n" para el par�metro dado por el handle "%1:n" que es de tipo par�metro.\nDebe consultar con el administrador.
#define M4MDTERROR_PARAM_VALUE_NO_PARAM					0x0103 + M4MDTERROR_BASE
// El valor del par�metro no es de tipo par�metro.\n \nSe est� intentando usar como par�metro el valor dado por la fila "%0:n" para el par�metro dado por el handle "%1:n" que es de tipo cadena.\nDebe consultar con el administrador.



//=================================================================================
// Errores del servicio
//=================================================================================

#define M4MDTERROR_LIBRARY_NOT_INITAILISED				0x0200 + M4MDTERROR_BASE
// Librer�a no inicializada.\n \nNo se ha inicializado la librer�a "m4mdtransfer".\nDebe consultar con el administrador.
#define M4MDTERROR_LIBRARY_ALREADY_INITAILISED			0x0201 + M4MDTERROR_BASE
// Librer�a ya inicializada.\n \nNo se puede inicializar la librer�a "m4mdtransfer" porque ya ha sido inicializada.\nDebe consultar con el administrador.
#define M4MDTERROR_EXCEPTION							0x0202 + M4MDTERROR_BASE
// Excepci�n.\n \nSe ha producido una excepci�n con el mensaje "%0:s".\nDebe consultar con el administrador.



//=================================================================================
// Errores del diccionario
//=================================================================================

#define M4MDTERROR_NULL_ARGUMENT						0x0300 + M4MDTERROR_BASE
// Argumento nulo a una funci�n.\n \nEl argumento "%0:s" del m�todo "%1:s" es nulo.\nDebe consultar con el administrador.\nEsto sucedi� en la l�nea "%2:s" del archivo "%3:s".
#define M4MDTERROR_DUPLICATED_OBJECT					0x0301 + M4MDTERROR_BASE
// Objeto traspasable duplicado.\n \nExiste m�s de una definici�n del objeto traspasable "%0:s" para la versi�n "%1:n".\nDebe consultar con el administrador.
#define M4MDTERROR_NOT_USED_1							0x0302 + M4MDTERROR_BASE
// No se usa.
#define M4MDTERROR_NO_CREATE_DIR						0x0303 + M4MDTERROR_BASE
// No se ha podido crear el directorio.\n \nNo se ha podido crear el directorio "%0:s" por la raz�n "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_DUPLICATED_TABLE						0x0304 + M4MDTERROR_BASE
// Tabla l�gica duplicada.\n \nExiste m�s de una definici�n de la tabla l�gica "%0:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_SUCH_OBJECT						0x0305 + M4MDTERROR_BASE
// No existe el objeto traspasable.\n \nNo existe el objeto traspasable "%0:s" para la versi�n "%1:n".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_SUCH_TABLE						0x0306 + M4MDTERROR_BASE
// No existe la tabla l�gica.\n \nNo existe la tabla l�gica "%0:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_OPEN_FILE							0x0307 + M4MDTERROR_BASE
// No se ha podido crear el archivo.\n \nNo se ha podido crear el archivo "%0:s" por la raz�n "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_WRITE_FILE						0x0308 + M4MDTERROR_BASE
// No se ha podido escribir en el archivo.\n \nNo se ha podido escribir en el archivo "%0:s" por la raz�n "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_DELETE_FILE						0x0309 + M4MDTERROR_BASE
// No se ha podido borrar el archivo.\n \nNo se ha podido borrar el archivo "%0:s" por la raz�n "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_CLOSE_FILE						0x030A + M4MDTERROR_BASE
// No se ha podido cerrar el archivo.\n \nNo se ha podido cerrar el archivo "%0:s" por la raz�n "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_FIND_FILE							0x030B + M4MDTERROR_BASE
// No se ha podido encontrar el archivo.\n \nNo se ha podido encontrar el archivo "%0:s" por la raz�n "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_REAL_FIELD						0x030C + M4MDTERROR_BASE
// No existe el campo real.\n \nNo existe el campo real para el campo "%0:s" del objeto "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_READ_FILE							0x030D + M4MDTERROR_BASE
// No se ha podido leer el archivo.\n \nNo se ha podido leer el archivo "%0:s" por la raz�n "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_STAT_FILE							0x030E + M4MDTERROR_BASE
// No se ha podido leer el estado del archivo.\n \nNo se ha podido leer el estado del archivo "%0:s" por la raz�n "%1:s".\nDebe consultar con el administrador.



//=================================================================================
// Errores del transformer
//=================================================================================

#define M4MDTERROR_TR_NOT_INITIALIZED					0x0400 + M4MDTERROR_BASE
// Transformer no inicializado.\n\nEl objeto no ha sido inicializado correctamente.\nDebe consultar con el administrador.
#define M4MDTERROR_TR_OBJ_GENERATION					0x0401 + M4MDTERROR_BASE
// Error generando objeto.\n\nNo se ha generado correctamente el XML coresspondente al objeto "%0:s".\nCompruebe el nombre del objeto.
#define M4MDTERROR_TR_PARSING_OBJ						0x0402 + M4MDTERROR_BASE
// Error procesando objeto xml.\n\nNo se ha procesado correctamente el XML coresspondente al objeto "%0:s".\nDebe consultar con el administrador.
#define M4MDTERROR_TR_RESET								0x0403 + M4MDTERROR_BASE
// Error procesando objeto xml.\n\nNo se ha inicializado correctamente el procesador XML.\nDebe comprobar el entorno de ejecuci�n.
#define M4MDTERROR_TR_OPENFILE							0x0404 + M4MDTERROR_BASE
// Error creando fichero.\n\nNo se ha podido crear el fichero de salida "%0:s".\nDebe comprobar el nombre de fichero y el directorio.
#define M4MDTERROR_TR_NOT_INIT							0x0405 + M4MDTERROR_BASE
// Error de inicializaci�n.\n\nEl procesador XML no se ha inicializado correctamente.\nDebe comprobar otros errores.
#define M4MDTERROR_TR_GENERATE_TABLE					0x0406 + M4MDTERROR_BASE
// Error de generaci�n.\n\nSe ha producido un error en la generaci�n de una tabla.\nDebe comprobar otros errores.
#define M4MDTERROR_TR_TABLE_LOAD						0x0407 + M4MDTERROR_BASE
// Error de generaci�n.\n\nSe ha producido un error en la carga de la tabla "%0:s".\nDebe comprobar otros errores.
#define M4MDTERROR_TR_TABLE_DEF							0x0408 + M4MDTERROR_BASE
// Error de generaci�n.\n\nSe ha producido un error obteniendo la definici�n de la tabla "%0:s".\nDebe comprobar otros errores.
#define M4MDTERROR_TR_TABLE_DUMP						0x0409 + M4MDTERROR_BASE
// Error de generaci�n.\n\nSe ha producido un error volcando la tabla "%0:s".\nDebe comprobar otros errores.
#define M4MDTERROR_TR_BLOCK_LOAD						0x040a + M4MDTERROR_BASE
// Error de generaci�n.\n\nSe ha producido un error cargando el bloque "%0:n".\nDebe comprobar otros errores.
#define M4MDTERROR_TR_TABLE_SELECT						0x040b + M4MDTERROR_BASE
// Error de generaci�n.\n\nSe ha producido un error generando la sentencia de carga.\nDebe comprobar otros errores.
#define M4MDTERROR_TR_INVALID_VALUE						0x040c + M4MDTERROR_BASE
// Error evaluando s�mbolo.\n \nEl s�mbolo "%0:s" no es v�lido.\nDebe comprobar el modelo.
#define M4MDTERROR_TR_INVALID_PARAM						0x040d + M4MDTERROR_BASE
// Error en par�metro.\n \nEl par�metro "%0:s" no es v�lido.\nDebe comprobar el modelo.
#define M4MDTERROR_TR_INVALID_PARENT					0x040e + M4MDTERROR_BASE
// Contexto padre inv�lido.\n \nEl contexto padre para el campo "%0:s" y regirstro "%1:d" no es v�lido.\nDebe comprobar el modelo de datos.
#define M4MDTERROR_TR_WRITE_TAG							0x040f + M4MDTERROR_BASE
// Error de escritura.\n \nNo se ha podido escribir el element "%0:s" en el dispositivo de salida.\nDebe comprobar el directorio de salida.
#define M4MDTERROR_TR_READ_VALUE						0x0410 + M4MDTERROR_BASE
// Error leyendo valor.\n \nNo se ha podido leer un valor para el atribudo "%0:s".\nDebe comprobar el nombre del atributo.
#define M4MDTERROR_TR_CREATE_PARAMETER					0x0411 + M4MDTERROR_BASE
// Error creando par�metro.\n \nNo se ha podido crear un objeto par�metro.\nDebe consultar con el administrador.
#define M4MDTERROR_TR_CREATE_TEMPLATE_ARG				0x0412 + M4MDTERROR_BASE
// Error creando plantilla.\n \nNo se ha podido crear un argumento para la plantilla.\nDebe consultar con el administrador.
#define M4MDTERROR_TR_ADD_COLUMN						0x0413 + M4MDTERROR_BASE
// Error creando columna.\n \nNo se ha podido crear una nueva columna.\nDebe consultar con el administrador.
#define M4MDTERROR_TR_PARAM_NAME_NULL					0x0414 + M4MDTERROR_BASE
// Par�metro nulo.\n \nEl par�metro para el objeto es nulo.\nDebe consultar con el administrador.
#define M4MDTERROR_TR_PARAM_VALUE_NULL					0x0415 + M4MDTERROR_BASE
// Valor de par�metro nulo.\n \nEl valor del par�metro "%s:0" es nulo.\nDebe consultar con el administrador.
#define M4MDTERROR_TR_INVALID_PARAM_PK					0x0416 + M4MDTERROR_BASE
// Par�metro PK inv�lido.\n \nEl par�metro "%0:s" del objeto "%1:s" no es v�lido.\nDebe comprobar el modelo de traspaso.
#define M4MDTERROR_TR_RELEASE_PARAMETER					0x0417 + M4MDTERROR_BASE
// Error borrando par�metro.\n \nEl par�metro "%0:s" de handler "%1:d" no ha podido ser eliminado.\nDebe consultar con el administrador.
#define M4MDTERROR_TR_DB_MORE_THAN_ONE					0x0418 + M4MDTERROR_BASE
// M�s de un registro encontrado.\n \nSe ha encontrado m�s de un registro en el repositorio alternativo "%0:s".\nDebe consultar con el administrador.
#define M4MDTERROR_TR_CALL_TABLE_NULL_PK				0x0419 + M4MDTERROR_BASE
// PK nula.\n \nSe ha intentado cargar un bloque con la PK "%0:s" con el valor nulo en un objeto de tipo call-table.\nDebe consultar con el administrador.
#define M4MDTERROR_GENERATE_UPDATE_FUNCTION				0x041a + M4MDTERROR_BASE
// Funci�n no implementada.\n \nLa funci�n de generaci�n "%0:s" no est� implementada.\nDebe consultar con el administrador.



//=================================================================================
// Errores del updater
//=================================================================================

#define M4MDTERROR_SEQUENCE_ERROR						0x0440 + M4MDTERROR_BASE
// XML de datos mal definido.\n \nSe ha producido un error en la secuencia de procesamiento del fichero XML de datos. El elemento "%0:s" no puede venir depu�s del elemento "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT			0x0441 + M4MDTERROR_BASE
// No existe el atributo.\n \nNo existe el atributo "%0:s" en el elemento "%1:s" de tipo "%2:s".\nEsto sucedi� procesando el objeto l�gico "%3:s".\nDebe consultar con el administrador.
#define M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_RICH_DATA		0x0442 + M4MDTERROR_BASE
// No existe el atributo.\n \nNo existe el atributo "%0:s" en el elemento "%1:s" del XML de datos.\nDebe consultar con el administrador.
#define M4MDTERROR_NO_SUCH_ELEMENT_IN_DICT				0x0443 + M4MDTERROR_BASE
// No existe el elemento.\n \nNo existe el elemento "%0:s" de tipo "%1:s".\nEsto sucedi� procesando el objeto l�gico "%2:s".\nDebe consultar con el administrador.
#define M4MDTERROR_UNSUPPORTED_UPDATE_FUNCTION			0x0444 + M4MDTERROR_BASE
// Funci�n no implementada.\n \nLa funci�n de actualizaci�n "%0:s" no est� implementada.\nEsto sucedi� procesando el objeto l�gico "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_BAD_UPDATE_FUNCTION					0x0445 + M4MDTERROR_BASE
// Funci�n mal definida.\n \nLa funci�n de actualizaci�n "%0:s" no est� correctamente definida".\nEsto sucedi� procesando el objeto l�gico "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_MISSING_UPDATE_ARGUMENT				0x0446 + M4MDTERROR_BASE
// Argumento no definido.\n \nNo se ha definido el argumento "%0:s" de la funci�n de actualizaci�n "%1:s".\nEsto sucedi� procesando el objeto l�gico "%2:s".\nDebe consultar con el administrador.



//=================================================================================
// Errores del rich transformer.
//=================================================================================
#define M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA		0x0460 + M4MDTERROR_BASE
// No existe el atributo.\n \nNo existe el atributo "%0:s" en el elemento "%1:s" del XML de datos.\nDebe consultar con el administrador.
#define M4MDTERROR_NO_LOOKUP_VALUE						0x0461 + M4MDTERROR_BASE
// No se usa.
#define M4MDTERROR_NO_XML_DATA_FILE						0x0462 + M4MDTERROR_BASE
// Error abriendo fichero.\n\nNo se ha podido abrir el fichero XML de datos "%0:s".\nDebe comprobar el nombre de fichero y el directorio.


//=================================================================================
// Errores del merger
//=================================================================================
#define M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_EXCLUSION_XML_DATA		0x0500 + M4MDTERROR_BASE
// No existe el atributo.\n \nNo existe el atributo "%0:s" en el elemento "%1:s" del XML de campos excluidos.\nDebe consultar con el administrador.


//=================================================================================
// Errores de la tool de presentaciones
//=================================================================================

#define M4MDTTERROR_NO_INIT_COM							0x0600 + M4MDTERROR_BASE
// Error inicializando COM.\n \nNo se ha podido inicializar la librer�a de COM por la raz�n "%0:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".
#define M4MDTTERROR_NO_CREATE_INSTANCE					0x0601 + M4MDTERROR_BASE		
// Error creando objeto COM.\n \nNo se ha podido crear el objeto COM "%0:s" por la raz�n "%1:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%2:s".
#define M4MDTTERROR_NO_INIT_COM_OBJECT					0x0602 + M4MDTERROR_BASE	
// Error inicializando objeto COM.\n \nNo se ha podido inicializar el objeto COM "%0:s" por la raz�n "%1:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%2:s".
#define M4MDTTERROR_NO_PUT_ARRAY_ELEMENT				0x0603 + M4MDTERROR_BASE			
// Error a�adiendo elemento a un array COM.\n \nNo se ha podido a�adir un elemento a un array COM por la raz�n "%0:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".
#define M4MDTTERROR_NO_CHILD_COL_FROM_TEXT_PRES			0x0604 + M4MDTERROR_BASE				
// Error leyendo presentaci�n de formato texto.\n \nNo se ha podido obtener la presentaci�n proveniente de un obl de presentaci�n por la raz�n "%0:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".
#define M4MDTTERROR_NO_GET_ARRAY_PRESENTATION			0x0605 + M4MDTERROR_BASE
// Error leyendo array de elementos.\n \nNo se ha podido obtener un elemento del array de presentaci�n por la raz�n "%0:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".
#define M4MDTTERROR_NO_PRES_FROM_ARRAY					0x0606 + M4MDTERROR_BASE		
// Error transformando presentaci�n.\n \nNo se ha podido obtener el objeto presentaci�n a partir del array de elementos por la raz�n "%0:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".
#define M4MDTTERROR_NO_OBL_FROM_PRES					0x0607 + M4MDTERROR_BASE		
// Error transformando presentaci�n.\n \nNo se ha podido obtener el obl de presentaci�n a partir del objeto presentaci�n por la raz�n "%0:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".
#define M4MDTTERROR_NO_PRES_FROM_OBL					0x0608 + M4MDTERROR_BASE		
// Error transformando presentaci�n.\n \nNo se ha podido obtener el objeto presentaci�n a partir del obl de presentaci�n por la raz�n "%0:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".
#define M4MDTTERROR_NO_ARRAY_FROM_PRESS					0x0609 + M4MDTERROR_BASE		
// Error transformando presentaci�n.\n \nNo se ha podido obtener el array de elementos a partir del objeto presentaci�n por la raz�n "%0:n".\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".
#define M4MDTTERROR_NO_BASE64_TO_BINARY					0x060A + M4MDTERROR_BASE		
// Error transformando presentaci�n.\n \nNo se ha podido obtener la presentaci�n en formato binario a partir del formato base64.\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%0:s".
#define M4MDTTERROR_NULL_PRES							0x060B + M4MDTERROR_BASE		
// Error transformando presentaci�n.\n \nNo se ha podido obtener el obl de presentaci�n a partir del objeto presentaci�n porque el campo "%0:s" es nulo.\nDebe consultar con el administrador.\nEsto sucedi� al procesar la presentaci�n "%1:s".



//=================================================================================
// Errores de la wrapper
//=================================================================================

#define M4MDTERROR_FILE_ALREADY_OPEN					0x0700 + M4MDTERROR_BASE
// Fichero ya abierto.\n \nNo se ha podido abrir el fichero "%0:n" porque ya est� abierto como "%1:s".\nDebe consultar con el administrador.
#define M4MDTERROR_FILE_NOT_OPEN						0x0701 + M4MDTERROR_BASE
// Fichero no abierto.\n \nNo se ha podido operar con el fichero porque no est� abierto.\nDebe consultar con el administrador.



#endif



