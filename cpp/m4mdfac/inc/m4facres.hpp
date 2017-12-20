
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                m4facres.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                29-02-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este m�dulo contiene los defines de error de la factor�a de metadatos
//
//
//==============================================================================


#include "m4res.hpp"


#ifndef __M4FACRES_HPP__
#define __M4FACRES_HPP__



// ============================================================================
// De la factor�a
// ============================================================================

#define M4_MDFAC_NO_MEMORY									0x0001 + M4_MDFAC_ERROR_BASE
// Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicaci�n.\nEsto sucedi� en la l�nea "%1:s" del archivo "%2:s".
#define M4_MDFAC_INTERNAL_ERROR								0x0002 + M4_MDFAC_ERROR_BASE
// Error interno.\n \nDebe consultar con el administrador.\nEsto sucedi� en la l�nea "%0:s" del archivo "%1:s".
#define M4_MDFAC_INTERNAL_ERROR_WITH_INFO					0x0003 + M4_MDFAC_ERROR_BASE
// Error interno.\n \nInformaci�n adicional: "%2:s".\nDebe consultar con el administrador.\nEsto sucedi� en la l�nea "%0:s" del archivo "%1:s".
#define M4_MDFAC_AMBIGUOUS_ALIAS_INSTANCE					0x0004 + M4_MDFAC_ERROR_BASE
// Instancia ambigua.\n \nLa instancia "%0:s" define el par�metro "%3:s" como "%5:s" cuando ya hab�a sido definido en este u otro nodo como "%4:s".\nDebe definir la instancia de manera un�voca o definir una nueva intancia.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_AMBIGUOUS_TI_CALL_INSTALLING				0x0005 + M4_MDFAC_ERROR_BASE
// Estructura de nodo ambigua.\n \nLa estructura de nodo "%0:s" no est� resuelta para uno de los "%1:s" nodos que la utilizan.\nDebe resolver el alias de estructura de nodo.\nEsto sucedi� en "%2:s" de "%3:s" de "%4:s".
#define M4_MDFAC_NOT_USED_1									0x0006 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_BAD_GROUP_ITEM								0x0007 + M4_MDFAC_ERROR_BASE
// Grupo incorrecto.\n \nEl elemento "%0:s" no puede pertenecer al grupo "%3:s" porque tiene argumentos o no es un m�todo.\nDebe eliminar el elemento del grupo.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_INCORRECT_RULE								0x0008 + M4_MDFAC_ERROR_BASE
// Regla incorrecta.\n \nLa regla "%0:s" no puede usarse porque est� marcada como incorrecta.\nDebe corregir la regla.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s".
#define M4_MDFAC_NULL_POLISH								0x0009 + M4_MDFAC_ERROR_BASE
// Regla nula.\n \nLa regla "%0:s" no puede usarse porque no tiene c�digo LN4 asociado.\nDebe corregir la regla.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s".
#define M4_MDFAC_INTERNAL_TYPE_REDEFINITION					0x000A + M4_MDFAC_ERROR_BASE
// Tipo interno repetido.\n \nEl tipo interno "%0:s" est� repetido en un nodo.\nDebe cambiar el tipo interno del elemento.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s".
#define M4_MDFAC_ITEM_CYCLE									0x000B + M4_MDFAC_ERROR_BASE
// Ciclo de conceptos.\n \nExiste un ciclo de conceptos en el grupo "%3:s".\nDebe revisar el c�digo de los elementos del grupo.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NODE_CYCLE									0x000C + M4_MDFAC_ERROR_BASE
// Ciclo de nodos.\n \nExiste un ciclo de nodos en el Meta4Object.\nDebe cambiar la estructura de conectores.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_MDFAC_INTERNAL_TYPE_MISMATCH						0x000D + M4_MDFAC_ERROR_BASE
// Tipo interno incorrecto.\n \nEl tipo interno "%0:s" debe ser "%1:s" en vez de "%2:s".\nDebe cambiar el tipo del elemento o cambiar su tipo interno.\nEsto sucedi� en "%3:s" de "%4:s" de "%5:s".
#define M4_MDFAC_BAD_AUTO_CONNECTOR_TYPE					0x000E + M4_MDFAC_ERROR_BASE
// Conexi�n incorrecta.\n \nEl nodo "%0:s" est� conectado a s� mismo en modo "%2:s".\nDebe eliminar el conector o cambiar su tipo.\nEsto sucedi� en "%1:s".
#define M4_MDFAC_BAD_FILE_VERSION							0x000F + M4_MDFAC_ERROR_BASE
// Versi�n incorrecta.\n \nLa versi�n del archivo de definici�n es "%1:s" y la de la aplicaci�n en curso es "%2:s".\nDebe instalar correctamente la aplicaci�n.\nEsto sucedi� en el archivo "%0:s".
#define M4_MDFAC_MAX_VALUE									0x0010 + M4_MDFAC_ERROR_BASE
// Definici�n incorrecta.\n \nEl valor "%0:s" es mayor que el m�ximo permitido "%4:s" y se usar� el valor por defecto "%5:s".\nDebe cambiar el valor a uno permitido.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s" al generar "%6:s".
#define M4_MDFAC_MIM_VALUE									0x0011 + M4_MDFAC_ERROR_BASE
// Definici�n incorrecta.\n \nEl valor "%0:s" es menor que el m�nimo permitido "%4:s" y se usar� el valor por defecto "%5:s".\nDebe cambiar el valor a uno permitido.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s" al generar "%6:s".
#define M4_MDFAC_BAD_VALUE									0x0012 + M4_MDFAC_ERROR_BASE
// Definici�n incorrecta.\n \nEl valor "%0:s" no est� permitido para la definici�n "%1:s" y se usar� el valor por defecto "%2:s".\nDebe cambiar el valor a uno permitido.\nEsto sucedi� al generar "%3:s".
#define M4_MDFAC_DATA_TRUNCATED								0x0013 + M4_MDFAC_ERROR_BASE
// ID truncado.\n \nEl ID "%0:s" supera su m�xima longitud "%1:s" y se usar� "%2:s".\nDebe cambiar el ID.
#define M4_MDFAC_BAD_ARG_MAPPING_NUMBER						0x0014 + M4_MDFAC_ERROR_BASE
// Conexi�n incorrecta.\n \nEl elemento "%0:s" s�lo tiene "%6:s" de sus "%3:s" argumentos resueltos.\nDebe definir el mapeo de todos los argumentos.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s" al conectarse a "%4:s" de "%5:s".
#define M4_MDFAC_BAD_READ_NUMBER							0x0015 + M4_MDFAC_ERROR_BASE
// Archivo incorrecto.\n \nNo se encuentra el ID num�rico "%0:s" en el archivo de configuraci�n.\nDebe instalar correctamente la aplicaci�n.\nEsto sucedi� en la posici�n "%1:s" de un archivo de configuraci�n.
#define M4_MDFAC_BAD_READ_STRING							0x0016 + M4_MDFAC_ERROR_BASE
// Archivo incorrecto.\n \nNo se encuentra el ID cadena "%0:s" en el archivo de configuraci�n.\nDebe instalar correctamente la aplicaci�n.\nEsto sucedi� en la posici�n "%1:s" de un archivo de configuraci�n.
#define M4_MDFAC_NO_SUCH_TOKEN								0x0017 + M4_MDFAC_ERROR_BASE
// Archivo incorrecto.\n \nNo se encuentra el ID "%0:s" en el archivo de configuraci�n.\nDebe instalar correctamente la aplicaci�n.\nEsto sucedi� en la l�nea "%1:s" del archivo "%2:s".
#define M4_MDFAC_NO_READ_NUMBER								0x0018 + M4_MDFAC_ERROR_BASE
// Archivo incorrecto.\n \nNo se encuentra un ID num�rico en el archivo de configuraci�n.\nDebe instalar correctamente la aplicaci�n.\nEsto sucedi� en la l�nea "%0:s" del archivo "%1:s".
#define M4_MDFAC_NO_READ_STRING								0x0019 + M4_MDFAC_ERROR_BASE
// Archivo incorrecto.\n \nNo se encuentra un ID cadena en el archivo de configuraci�n.\nDebe instalar correctamente la aplicaci�n.\nEsto sucedi� en la l�nea "%0:s" del archivo "%1:s".
#define M4_MDFAC_NO_READ_DATE								0x001A + M4_MDFAC_ERROR_BASE
// Archivo incorrecto.\n \nNo se encuentra un ID fecha en el archivo de configuraci�n.\nDebe instalar correctamente la aplicaci�n.\nEsto sucedi� en la l�nea "%0:s" del archivo "%1:s".
#define M4_MDFAC_NOT_USED_2									0x001B + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_ZERO_INDEX									0x001C + M4_MDFAC_ERROR_BASE
// �ndice incorrecto.\n \nUn �ndice no puede tener como ID cero.\nDebe definir un n�mero de �ndice v�lido.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_MDFAC_NOT_USED_3									0x001D + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_4									0x001E + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NO_OPEN_FILE								0x001F + M4_MDFAC_ERROR_BASE
// Archivo no v�lido.\n \nNo se puede abrir el archivo del sistema "%0:s" en modo "%1:s".\nDebe consultar con el administrador.
#define M4_MDFAC_BAD_PARAMETER								0x0020 + M4_MDFAC_ERROR_BASE
// Par�metro incorrecto.\n \nUn par�metro no puede definirse de tipo "%3:s" y alcance "%4:s".\nDebe cambiar el tipo o alcance del par�metro o eliminarlo.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NO_SUCH_M4OBJ								0x0021 + M4_MDFAC_ERROR_BASE
// No existe el Meta4Object.\n \nDebe definir el Meta4Object "%0:s".
#define M4_MDFAC_NOT_USED_5									0x0022 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_6									0x0023 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_7									0x0024 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_CACHE_PUT_CMCR								0x0025 + M4_MDFAC_ERROR_BASE
// No se pudo almacenar la definici�n del Meta4Object "%0:s" en la cach�.\n \nDebe consultar con el administrador.\nAtributos de la definici�n del Meta4Object "%0:s": tipo cliente/servidor "%1:s".
#define M4_MDFAC_CACHE_PUT_CSCR								0x0026 + M4_MDFAC_ERROR_BASE
// No se pudo almacenar definici�n de la seguridad del Meta4Object "%0:s" en la cach�.\n \nDebe consultar con el administrador.\nAtributos de la definici�n de la seguridad del Meta4Object "%0:s": perfil "%1:s", tipo cliente/servidor "%2:s".
#define M4_MDFAC_NOT_USED_8									0x0027 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_9									0x0028 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_10								0x0029 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_11								0x002A + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_12								0x002B + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NO_SUCH_INDEX								0x002C + M4_MDFAC_ERROR_BASE
// No existe el �ndice.\n \nEl elemento "%0:s" pertenece al �ndice "%1:s" que no existe.\nDebe definir el �ndice o eliminar el elemento de ese �ndice.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_MDFAC_BAD_CONNECTION_SCOPE						0x002D + M4_MDFAC_ERROR_BASE
// Conexi�n incorrecta.\n \nNo se puede conectar un elemento de �mbito "%6:s" a otro de �mbito "%7:s" en una relaci�n "%0:s".\nDebe cambiar el �mbito de los elementos o eliminar la conexi�n.\nEsto sucedi� al conectar el elemento "%1:s" de "%2:s" al elemento "%3:s" de "%4:s" de "%5:s".
#define M4_MDFAC_BAD_CONNECTION_CS_TYPE						0x002E + M4_MDFAC_ERROR_BASE
// Conexi�n incorrecta.\n \nNo se puede conectar un elemento de tipo cliente/servidor "%6:s" a otro de tipo "%7:s" en una conexi�n "%0:s".\nDebe cambiar el tipo cliente/servidor de los elementos o eliminar la conexi�n.\nEsto sucedi� al conectar el elemento "%1:s" de "%2:s" al elemento "%3:s" de "%4:s" de "%5:s".
#define M4_MDFAC_BAD_ITEM_CS_TYPE							0x002F + M4_MDFAC_ERROR_BASE
// Tipo cliente/servidor incorrecto.\n \nUn elemento de tipo cliente/servidor "%3:s" no puede pertenecer a un nodo de tipo "%4:s".\nDebe cambiar el tipo cliente/servidor del elemento o del nodo.\nEsto sucedi� en "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NOT_USED_13								0x0030 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NO_SUCH_TI_SENTENCE						0x0031 + M4_MDFAC_ERROR_BASE
// No existe la sentencia.\n \nDebe definir la sentencia "%0:s" o eliminarla de la estructura de nodo.\nEsto sucedi� en "%2:s" de "%1:s".
#define M4_MDFAC_NO_SUCH_CONNECTOR_SENTENCE					0x0032 + M4_MDFAC_ERROR_BASE
// No existe la sentencia.\n \nDebe definir la sentencia "%0:s" o eliminarla del conector.\nEsto sucedi� al conectar "%2:s" a "%3:s" en "%1:s".
#define M4_MDFAC_NO_SUCH_ITEM_CON							0x0033 + M4_MDFAC_ERROR_BASE
// No existe el conector de elementos.\n \nNo existe el conector de elementos entre "%0:s" de "%1:s" y "%2:s" de "%3:s".\nDebe definir el conector o eliminar el mapeo de argumentos.\nEsto sucedi� al generar la asignaci�n de argumentos de "%4:s".
#define M4_MDFAC_NO_SUCH_NODE_CON							0x0034 + M4_MDFAC_ERROR_BASE
// No existe el conector.\n \nNo existe el conector entre "%0:s" y "%1:s".\nDebe definir el conector o eliminar el conector de elementos.\nEsto sucedi� al generar el conector de elementos entre "%3:s" y "%4:s" de "%2:s".
#define M4_MDFAC_NO_SUCH_ITEM_ARG							0x0035 + M4_MDFAC_ERROR_BASE
// No existe el argumento del elemento.\n \nNo existe el argumento "%0:s" del elemento "%1:s" de "%2:s".\nDebe definir el argumento o eliminar la asignaci�n de argumentos.\nEsto sucedi� al generar el mapeo de argumentos a "%4:s" de "%5:s" de "%3:s".
#define M4_MDFAC_NO_SUCH_ITEM_FORMULA						0x0036 + M4_MDFAC_ERROR_BASE
// No existe la f�rmula.\n \nNo existe la f�rmula "%0:s" de "%1:s" de "%2:s" de "%3:s".\nDebe definir la f�rmula o no invocarla.\nEsto sucedi� al instalar la f�rmula "%4:s" de "%5:s" de "%6:s" de "%7:s".
#define M4_MDFAC_NO_SUCH_PLUG_INSTALLING					0x0037 + M4_MDFAC_ERROR_BASE
// No existe el conector de Meta4Objects.\n \nNo existe el conector entre "%0:s" de "%1:s" y "%2:s" de "%3:s".\nDebe definir el conector de Meta4Objects o eliminar el conector de elementos.\nEsto sucedi� al generar los conectores de elementos entre Meta4Objects.
#define M4_MDFAC_NOT_USED_14								0x0038 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_ITEM_TYPE_MISMATCH							0x0039 + M4_MDFAC_ERROR_BASE
// Tipo incorrecto.\n \nEl tipo del elemento "%0:s" es "%4:s" y deber�a ser "%3:s".\nDebe instalar correctamente la aplicaci�n.\nEsto sucedi� en "%1:s" de "%2:s" al generar "%5:s".
#define M4_MDFAC_NOT_USED_15								0x003A + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_AUTOCONNECTED_TI_ITEM						0x003B + M4_MDFAC_ERROR_BASE
// Elemento conectado a s� mismo.\n \nEl elemento "%0:s" est� conectado a s� mismo.\nDebe eliminar ese conector de elementos de la estructura de nodo.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_AUTOCONNECTED_NODE_ITEM					0x003C + M4_MDFAC_ERROR_BASE
// Elemento conectado a s� mismo.\n \nEl elemento "%0:s" est� conectado a s� mismo.\nDebe eliminar ese conector de elementos del nodo.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_EMPTY_SECURITY_MASK						0x003D + M4_MDFAC_ERROR_BASE
// M�scara no definida.\n \nNo se puede generar el Meta4Object "%0:s" porque su m�scara no est� definida.\nDebe definir una m�scara para el perfil "%1:s".
#define M4_MDFAC_TOO_MANY_PARENT_NODES						0x003E + M4_MDFAC_ERROR_BASE
// Nodo con varios padres.\n \nEl nodo "%0:s" tiene como padres a "%2:s" y "%3:s".\nDebe eliminar un conector.\nEsto sucedi� en "%1:s".
#define M4_MDFAC_NOT_USED_16								0x003F + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NO_CREATE_M4OBJ							0x0040 + M4_MDFAC_ERROR_BASE
// Meta4Object mal definido.\n \nNo se puede generar el Meta4Object "%0:s" de tipo "%1:s".\nDebe consultar otros mensajes de error.
#define M4_MDFAC_NO_CREATE_SECURITY							0x0041 + M4_MDFAC_ERROR_BASE
// Seguridad mal definida.\n \nNo se puede generar la seguridad de "%0:s" de tipo "%2:s" para el perfil "%1:s".\nDebe consultar otros mensajes de error.
#define M4_MDFAC_NOT_USED_17								0x0042 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_18								0x0043 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_19								0x0044 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NO_GET_ITEM_VALUE							0x0045 + M4_MDFAC_ERROR_BASE
// Error de lectura.\n \nNo se puede leer el valor de una definici�n del elemento "%0:s".\nDebe consultar con el administrador.\nEsto sucedi� en "%1:s" de "%2:s" al generar "%3:s".
#define M4_MDFAC_NO_SUCH_TI									0x0046 + M4_MDFAC_ERROR_BASE
// No existe la estructura de nodo.\n \nDebe definir la estructura de nodo "%0:s" o no utilizarla.\nEsto sucedi� al generar "%2:s" de "%1:s".
#define M4_MDFAC_NO_SUCH_NODE								0x0047 + M4_MDFAC_ERROR_BASE
// No existe el nodo.\n \nDebe definir el nodo "%0:s" o no utilizarlo.\nEsto sucedi� al generar "%2:s" de "%1:s".
#define M4_MDFAC_NO_SUCH_ITEM_IN_TI							0x0048 + M4_MDFAC_ERROR_BASE
// No existe el elemento.\n \nDebe definir el elemento "%0:s" de "%1:s" o no utilizarlo.\nEsto sucedi� al generar "%3:s" de "%2:s".
#define M4_MDFAC_MAX_INHERITANCE							0x0049 + M4_MDFAC_ERROR_BASE
// Herencia compleja.\n \nSe alcanzad� el m�ximo nivel de herencia "%0:s".\nDebe limitar la herencia de las estructuras de nodo.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_NOT_USED_20								0x004A + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_21								0x004B + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NO_SUCH_ITEM_IN_NODE						0x004C + M4_MDFAC_ERROR_BASE
// No existe el elemento.\n \nDebe definir el elemento "%0:s" de "%1:s" o no utilizarlo.\nEsto sucedi� al generar "%3:s" de "%2:s".
#define M4_MDFAC_NOT_USED_22								0x004D + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_BAD_NODE_CS_TYPE							0x004E + M4_MDFAC_ERROR_BASE
// Conexi�n incorrecta.\n \nNo se puede conectar un nodo de tipo cliente/servidor "%1:s" a otro de tipo "%3:s".\nDebe cambiar el tipo cliente/servidor de los nodos o eliminar la conexi�n.\nEsto sucedi� al conectar el nodo "%0:s" a "%2:s" de "%4:s".
#define M4_MDFAC_NOT_USED_23								0x004F + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_24								0x0050 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_25								0x0051 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_26								0x0052 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_27								0x0053 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_28								0x0054 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_29								0x0055 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_SENTENCE_WITH_TABLE_JOIN					0x0056 + M4_MDFAC_ERROR_BASE
// Sentencia con join.\n \nNo se puede generar la sentencia para "%0:s" porque utiliza las tablas "%1:s" y "%2:s".\nDebe cambiar la tabla del elemento "%3:s".
#define M4_MDFAC_SENTENCE_WITH_DIFFERENT_ALIAS				0x0057 + M4_MDFAC_ERROR_BASE
// Sentencia con varios alias.\n \nNo se puede generar la sentencia para "%0:s" porque utiliza los alias "%1:s" y "%2:s".\nDebe cambiar el alias del elemento "%3:s".
#define M4_MDFAC_SENTENCE_WITHOUT_OBJECT					0x0058 + M4_MDFAC_ERROR_BASE
// Sentencia sin tabla.\n \nNo se puede generar la sentencia para "%0:s" porque no tiene tabla de lectura.\nDebe definir la tabla de lectura de la estructura de nodo.
#define M4_MDFAC_BAD_LANE_NODE								0x0059 + M4_MDFAC_ERROR_BASE
// Lista en nodo hijo.\n \nEl nodo "%0:s" tiene "%2:s" listas definidas y no es un nodo ra�z.\nDebe eliminar las listas de ese nodo o definir el nodo como nodo ra�z.\nEsto sucedi� en "%0:s" de "%1:s".
#define M4_MDFAC_AUTO_SLICE_ITEM							0x005A + M4_MDFAC_ERROR_BASE
// Elemento de tramos de s� mismo.\n \nEl elemento "%0:s" es elemento de tramos de s� mismo.\nDebe definir otro elemento como elemento de tramos.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_AUTO_AUX_ITEM								0x005B + M4_MDFAC_ERROR_BASE
// Elemento auxiliar de s� mismo.\n \nEl elemento "%0:s" es elemento auxiliar de s� mismo.\nDebe definir otro elemento como elemento auxiliar.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_NO_NODES									0x005C + M4_MDFAC_ERROR_BASE
// Meta4Object sin nodos.\n \nEl Meta4Object "%0:s" no tiene ning�n nodo definido.\nDebe definir alg�n nodo para ese Meta4Object.
#define M4_MDFAC_MAX_LIMIT									0x005D + M4_MDFAC_ERROR_BASE
// Definici�n incorrecta.\n \nEl valor "%0:s" es mayor que el m�ximo permitido "%4:s" y se usar� el valor m�ximo.\nDebe cambiar el valor a uno permitido.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s" al construir "%5:s".
#define M4_MDFAC_DEFAULT_BIGGER_THAN_PREC					0x005E + M4_MDFAC_ERROR_BASE
// Precisi�n incorrecta.\n \nLa precisi�n "%0:s" es menor que la longitud "%1:s" del valor por defecto "%2:s" y se truncar� el valor por defecto a "%3:s".\nDebe cambiar la precisi�n del elemento.\nEsto sucedi� en "%4:s" de "%5:s" al construir "%6:s".
#define M4_MDFAC_DEFAULT_WITH_ZERO_PREC						0x005F + M4_MDFAC_ERROR_BASE
// Precisi�n incorrecta.\n \nLa precisi�n cero es menor que la longitud "%0:s" del valor por defecto "%1:s".\nDebe cambiar la precisi�n del elemento.\nEsto sucedi� en "%2:s" de "%3:s" al construir "%4:s".
#define M4_MDFAC_NEGATIVE_DB_PREC							0x0060 + M4_MDFAC_ERROR_BASE
// Precisi�n incorrecta.\n \nLa precisi�n "%0:s" negativa o cero no es v�lida para un elemento de tipo "%1:s" y se usar� el valor cero.\nDebe cambiar la precisi�n del elemento.\nEsto sucedi� en "%2:s" de "%3:s" al construir "%4:s".
#define M4_MDFAC_NOT_USED_30								0x0061 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_31								0x0062 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_32								0x0063 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_33								0x0064 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_34								0x0065 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_SENTENCE_WITHOUT_FIELDS					0x0066 + M4_MDFAC_ERROR_BASE
// Sentencia sin campos.\n \nNo se puede generar la sentencia para "%0:s" porque no tiene campos de lectura.\nDebe incluir alg�n campo de lectura en la estructura de nodo.
#define M4_MDFAC_NOT_USED_35								0x0067 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NO_SYSTEM_ITEMS							0x0068 + M4_MDFAC_ERROR_BASE
// No se encontraron los elementos del sistema.\n \nNo se encontraron los elementos de la estructura de nodo del sistema.\nDebe consultar con el administrador.
#define M4_MDFAC_NOT_USED_36								0x0069 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_37								0x006A + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_38								0x006B + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_39								0x006C + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_40								0x006D + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_41								0x006E + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_42								0x006F + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_43								0x0070 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_44								0x0071 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_45								0x0072 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_46								0x0073 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_47								0x0074 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_48								0x0075 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_49								0x0076 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_INIT									0x0077 + M4_MDFAC_ERROR_BASE
// Factor�a no inicializada.\n \nNo se puede ejecutar la funci�n "%0:s" porque la factor�a no est� inicializada.\nDebe consultar con el administrador.
#define M4_MDFAC_BAD_TOTAL_START_DATE						0x0078 + M4_MDFAC_ERROR_BASE
// Fecha de inicio fuera de rango.\n \nLa fecha de inicio "%0:d" de la salida a total "%1:s" est� fuera del rango permitido por "%2:d" y "%3:d" del total "%4:s".\nDebe cambiar el periodo de la salida a total.\nEsto sucedi� en "%5:s" de "%6:s".
#define M4_MDFAC_BAD_TOTAL_END_DATE							0x0079 + M4_MDFAC_ERROR_BASE
// Fecha de fin fuera de rango.\n \nLa fecha de fin "%0:d" de la salida a total "%1:s" est� fuera del rango permitido por "%2:d" y "%3:d" del total "%4:s".\nDebe cambiar el periodo de la salida a total.\nEsto sucedi� en "%5:s" de "%6:s".
#define M4_MDFAC_SYSTEM_INHERIT								0x007A + M4_MDFAC_ERROR_BASE
// Herencia del sistema.\n \nNo se puede heredar de la estructura de nodo del sistema "%0:s".\nDebe cambiar el �rbol de herencia.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_TI_INHERIT_CYCLE							0x007B + M4_MDFAC_ERROR_BASE
// Ciclo de herencia de estructura de nodo.\n \nExiste un ciclo de herencia generado por la estructura de nodo "%0:s".\nDebe cambiar el �rbol de herencia.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_REPEATED_ITEM								0x007C + M4_MDFAC_ERROR_BASE
// Elemento sobrescrito.\n \nEl elemento "%0:s" de "%1:s" se sobrescribi� y aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de elementos.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_MDFAC_REPEATED_TOTAL								0x007D + M4_MDFAC_ERROR_BASE
// Salida a total repetida.\n \nNo se puede definir la salida a total "%0:s" de "%1:s" de "%2:s" en el periodo con fecha de inicio "%3:d" y fecha de fin "%4:d" porque ya est� definida en el periodo con fecha de inicio "%5:d" y fecha de fin "%6:d".\nDebe cambiar la definici�n de la salida a total.\nEsto sucedi� en "%7:s" de "%8:s".
#define M4_MDFAC_INCOHERENT_TOTAL_REF						0x007E + M4_MDFAC_ERROR_BASE
// Periodo incoherente.\n \nLa salida a total "%0:s" de "%1:s" tiene un periodo cuya fecha de inicio "%2:d" es posterior a la fecha de fin "%3:d".\nDebe cambiar el periodo de la salida a total.\nEsto sucedi� en "%4:s".
#define M4_MDFAC_INCOHERENT_TOTAL							0x007F + M4_MDFAC_ERROR_BASE
// Periodo incoherente.\n \nEl total "%0:s" de "%1:s" tiene un periodo cuya fecha de inicio "%2:d" es posterior a la fecha de fin "%3:d".\nDebe cambiar el periodo del total.\nEsto sucedi� en "%4:s".
#define M4_MDFAC_INCOHERENT_RULE							0x0080 + M4_MDFAC_ERROR_BASE
// Periodo incoherente.\n \nLa regla "%0:s" de "%1:s" tiene un periodo cuya fecha de inicio "%2:d" es posterior a la fecha de fin "%3:d".\nDebe cambiar el periodo de la regla.\nEsto sucedi� en "%4:s".
#define M4_MDFAC_REPEATED_TRIGGER							0x0081 + M4_MDFAC_ERROR_BASE
// Connector de elemento sobrescrito.\n \nEl connector de elemento entre "%0:s" y "%1:s" se sobrescribi�.\nDebe cambiar el �rbol de herencia de los conectores de elemento.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_NO_BACK_PAY_ITEM									0x0082 + M4_MDFAC_ERROR_BASE
// No existe el elemento de "Back Pay".\n \nDebe definir el elemento de "Back Pay" para "%1:s".\nEsto sucedi� al generar la f�rmula de total para "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_REPEATED_TAG								0x0083 + M4_MDFAC_ERROR_BASE
// Condici�n de ejecuci�n sobrescrita.\n \nLa condici�n de ejecuci�n "%0:s" para la regla "%1:s" de "%2:s" de "%3:s" se sobrescribi� en el periodo con fecha de inicio "%4:d" y fecha de fin "%5:d".\nDebe cambiar el �rbol de herencia de las condiciones de ejecuci�n.\nEsto sucedi� en "%6:s" de "%7:s".
#define M4_MDFAC_INCOHERENT_TAG								0x0084 + M4_MDFAC_ERROR_BASE
// Periodo incoherente.\n \nLa condici�n de ejecuci�n "%0:s" para la regla "%1:s" de "%2:s" de "%3:s" tiene un periodo cuya fecha de inicio "%4:d" es posterior a la fecha de fin "%5:d".\nDebe cambiar el periodo de la condici�n de ejecuci�n.\nEsto sucedi� en "%6:s".
#define M4_MDFAC_NO_REV_NODE								0x0085 + M4_MDFAC_ERROR_BASE
// No existe el nodo de revisi�n.\n \nDebe definir el nodo de revisi�n para "%1:s".\nEsto sucedi� al generar la metarregla para "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NO_REV_ITEM								0x0086 + M4_MDFAC_ERROR_BASE
// No existe el elemento de revisi�n.\n \nDebe definir el elemento de revisi�n para "%1:s".\nEsto sucedi� al generar la metarregla para "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_TAGS_IGNORED								0x0087 + M4_MDFAC_ERROR_BASE
// Condiciones de ejecuci�n ignoradas.\n \nSe ignoraron las condiciones de ejecuci�n de "%0:s" porque ya tiene metarreglas o porque tiene argumentos variables.\nDebe eliminar las condiciones de ejecuci�n o revisar la definici�n del elemento.\nEsto sucedi� al generar la metarregla para "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_INCOHERENT_RULE_ORDER						0x0088 + M4_MDFAC_ERROR_BASE
// Orden de ejecuci�n incoherente.\n \nLa regla "%0:s" en el periodo comprendido entre la fecha de inicio "%1:d" y la fecha de fin "%2:d" tiene un orden de ejecuci�n "%3:s" diferente al de otros periodos "%4:s".\nDebe cambiar el orden de ejecuci�n.\nEsto sucedi� en "%5:s" de "%6:s" de "%7:s".
#define M4_MDFAC_INCOHERENT_RULE_RETRO						0x0089 + M4_MDFAC_ERROR_BASE
// Tipo de retroactividad incoherente.\n \nLa regla "%0:s", en el periodo comprendido entre la fecha de inicio "%1:d" y la fecha de fin "%2:d", tiene un tipo de retroactividad "%3:s" diferente al de otros periodos "%4:s".\nDebe cambiar el tipo de retroactividad.\nEsto sucedi� en "%5:s" de "%6:s" de "%7:s".
#define M4_MDFAC_BAD_NO_AUTO_CONNECTOR_TYPE					0x008A + M4_MDFAC_ERROR_BASE
// Conexi�n incorrecta.\n \nEl nodo "%0:s" est� conectado a "%1:s" en modo "auto".\nDebe eliminar el conector o cambiar su tipo.\nEsto sucedi� en "%2:s".
#define M4_MDFAC_TOO_T3_INHERITANCES						0x008B + M4_MDFAC_ERROR_BASE
// M�ximo nivel de herencia.\n \nEl Meta4Object "%0:s" tiene "%1:s" niveles de herencia y el m�ximo permitido es "%2:s".\nDebe cambiar el �rbol de herencia de este Meta4Object.
#define M4_MDFAC_T3_INHERIT_CYCLE							0x008C + M4_MDFAC_ERROR_BASE
// Ciclo de herencia de Meta4Object.\n \nExiste un ciclo de herencia generado por "%0:s".\nDebe cambiar el �rbol de herencia.\nEsto sucedi� en "%1:s".
#define M4_MDFAC_BAD_INHERIT_LEVEL							0x008D + M4_MDFAC_ERROR_BASE
// Nivel de herencia incorrecto.\n \nNo existe el nivel de herencia "%0:s".\nDebe cambiar el �rbol de herencia.\nEsto sucedi� en "%2:s" de "%1:s".
#define M4_MDFAC_BAD_NODE_OVERWRITE							0x008E + M4_MDFAC_ERROR_BASE
// Sobrescritura de nodo incorrecta.\n \nSe ha sobrescrito el nodo "%2:s" en el Meta4Object "%0:s" que no deriva de "%1:s" donde est� definido el nodo.\nDebe cambiar el �rbol de sobrescritura de nodos.\nEsto sucedi� en "%3:s".
#define M4_MDFAC_REPEATED_NODE_INHERIT						0x008F + M4_MDFAC_ERROR_BASE
// Sobrescritura de nodo repetida.\n \nLa sobrescritura de "%1:s" est� repetida en el Meta4Object "%0:s".\nDebe cambiar el �rbol de sobrescritura de nodos.\nEsto sucedi� en "%2:s".
#define M4_MDFAC_BAD_NODE_TI_OVERWRITE						0x0090 + M4_MDFAC_ERROR_BASE
// Sobrescritura de nodo incorrecta.\n \nEl nodo "%2:s" se sobrecribi� con una estructura de nodo "%0:s" que no hereda de "%1:s".\nDebe cambiar el �rbol de sobrescritura de nodos.\nEsto sucedi� en "%3:s".
#define M4_MDFAC_REPEATED_NODE								0x0091 + M4_MDFAC_ERROR_BASE
// Nodo sobrescrito.\n \nEl nodo "%0:s" aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de nodos.\nEsto sucedi� en "%1:s".
#define M4_MDFAC_REPEATED_ALIAS								0x0092 + M4_MDFAC_ERROR_BASE
// Alias sobrescrito.\n \nEl alias "%0:s" aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de alias.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_REPEATED_CONNECTOR							0x0093 + M4_MDFAC_ERROR_BASE
// Conector sobrescrito.\n \nEl conector establecido entre "%0:s" y "%1:s" aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de conectores.\nEsto sucedi� en "%2:s".
#define M4_MDFAC_REPEATED_ITEM_CONNECTOR					0x0094 + M4_MDFAC_ERROR_BASE
// Conector de elemento sobrescrito.\n \nEl conector entre "%0:s" de "%1:s" y "%2:s" de "%3:s" aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de conectores de elemento.\nEsto sucedi� en "%4:s".
#define M4_MDFAC_REPEATED_LIST								0x0095 + M4_MDFAC_ERROR_BASE
// Lista sobrescrita.\n \nLa lista "%0:s" de "%1:s" aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de listas.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_MDFAC_REPEATED_ITEM_LIST							0x0096 + M4_MDFAC_ERROR_BASE
// Conector de elemento de lista sobrescrito.\n \nEl elemento "%0:s" aparece m�s de una vez en la lista "%1:s" de "%2:s".\nDebe cambiar el �rbol de herencia de conectores de elementos de lista.\nEsto sucedi� en "%3:s" de "%4:s".
#define M4_MDFAC_REPEATED_INDEX								0x0097 + M4_MDFAC_ERROR_BASE
// �ndice sobrescrito.\n \nEl �ndice "%0:s" aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de �ndices.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_REPEATED_INDEX_ITEM						0x0098 + M4_MDFAC_ERROR_BASE
// Elemento de �ndice sobrescrito.\n \nEl elemento "%0:s" del �ndice "%1:s" aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de elementos de �ndice.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_MDFAC_REPEATED_FILTER							0x0099 + M4_MDFAC_ERROR_BASE
// Filtro sobrescrito.\n \nEl filtro "%0:s" aparece m�s de una vez.\nDebe cambiar el �rbol de herencia de filtros.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_REPEATED_ITEM_CONNECTOR_MAPPING			0x009A + M4_MDFAC_ERROR_BASE
// Argumento de conector de elemento sobrescrito.\n \nEl argumento "%0:s" de "%3:s" est� resuelto m�s de una vez en el conector entre "%1:s" de "%2:s" y "%3:s" de "%4:s".\nDebe cambiar el �rbol de herencia de argumentos de conector de elemento.\nEsto sucedi� en "%5:s".
#define M4_MDFAC_REPEATED_DEFAULT_INDEX						0x009B + M4_MDFAC_ERROR_BASE
// �ndice por defecto sobrescrito.\n \nEl �ndice "%0:s" no puede ser �ndice por defecto porque este nodo ya tiene un �ndice por defecto "%1:s".\nDebe desmarcar este �ndice por defecto.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_NO_BAD_BACK_PAY_ARGUMENTS						0x009C + M4_MDFAC_ERROR_BASE
// Argumentos incorrectos del elemento de "Back Pay".\n \nEl elemento de "Back Pay" "%0:s" tiene definidos "%1:s" argumentos".\nDebe definir el elemento con un s�lo argumento.\nEsto sucedi� al generar la f�rmula de total para "%2:s" de "%3:s" de "%4:s".
#define M4_MDFAC_NO_CREATE_XML								0x009D + M4_MDFAC_ERROR_BASE
// Error al generar XML.\n \nNo se puede generar la definici�n en XML del Meta4Object "%0:s".\nDebe consultar otros mensajes de error.
#define M4_MDFAC_NOT_USED_50								0x009E + M4_CH_MCR_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_51								0x009F + M4_CH_MCR_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_52								0x00A0 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_NOT_USED_53								0x00A1 + M4_MDFAC_ERROR_BASE
// No se usa.
#define M4_MDFAC_AUX_ITEM_PRORAT_BAD_TYPE					0x00A2 + M4_MDFAC_ERROR_BASE
// Tipo de elemento de prorrateo incorrecto.\n \nEl elemento auxiliar de prorrateo "%0:s" debe ser de tipo "Date" y no "%1:s".\nDebe cambiar el tipo del elemento.\nEsto sucedi� en "%2:s" de "%3:s" de "%4:s".
#define M4_MDFAC_AUX_ITEM_PRORAT_NO_AUX_ITEM				0x00A3 + M4_MDFAC_ERROR_BASE
// Elemento auxiliar no definido.\n \nNo se ha definido el elemento auxiliar del elemento de prorrateo "%0:s".\nDebe definir el elemento auxiliar.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s".
#define M4_MDFAC_CONNECTOR_ITEM_CYCLE						0x00A4 + M4_MDFAC_ERROR_BASE
// Ciclo de conectores de item.\n \nExiste un ciclo de conectores de item formado por "%0:s".\nDebe cambiar la estructura de conectores de item.\nEsto sucedi� en "%1:s".
#define M4_MDFAC_BAD_SENTENCE_MERGE							0x00A5 + M4_MDFAC_ERROR_BASE
// Merge de sentencias erroneo.\n \nNo se puede generar la sentencia para "%0:s" porque las sentencias "%1:s" y "%2:s" no son compatibles.\nDebe cambiar el �rbol de herencia de sentencias.
#define M4_MDFAC_NULL_M4_TYPE								0x00A6 + M4_MDFAC_ERROR_BASE
// El tipo nulo no es v�lido.\n \nNo se puede aplicar el tipo Meta4 nulo a un elemento de tipo "%0:s" porque no es un m�todo.\nDebe cambiar el tipo Meta4 del elemento.\nEsto sucedi� en "%1:s" de "%2:s".
#define M4_MDFAC_FUTURE_DATE								0x00A7 + M4_MDFAC_ERROR_BASE
// Definici�n incorrecta.\n \nLa fecha "%0:s" es mayor que la fecha actual "%4:s".\nDebe comprobar las fechas de grabaci�n de sus metadatos.\nEsto sucedi� en "%1:s" de "%2:s" de "%3:s" al generar "%5:s".
#define M4_MDFAC_NO_SUCH_NODE_FOR_BO						0x00A8 + M4_MDFAC_ERROR_BASE
// No existe el nodo.\n \nEl nodo "%0:s" no existe y no puede ser utilizado en un m�todo de negocio.\nEsto sucedi� al generar "%1:s" de "%2:s".
#define M4_MDFAC_NO_SUCH_ITEM_FOR_BO						0x00A9 + M4_MDFAC_ERROR_BASE
// No existe el item.\n \nEl item "%0:s" de "%1:s" no existe y no puede ser utilizado en un m�todo de negocio.\nEsto sucedi� al generar "%2:s" de "%3:s".
#define M4_MDFAC_NO_SUCH_NODE_FOR_BO_ARG					0x00AA + M4_MDFAC_ERROR_BASE
// No existe el nodo.\n \nEl nodo "%0:s" no existe y no puede ser utilizado como argumento complejo de un m�todo de negocio.\nEsto sucedi� al generar "%1:s" de "%2:s".
#define M4_MDFAC_REAPEATED_ARG_IN_BO						0x00AB + M4_MDFAC_ERROR_BASE
// Argumento repetido.\n \nEl nodo "%0:s" no puede ser utilizado como argumento complejo de un m�todo de negocio porque ya existe un argumento b�sico con ese nombre.\nEsto sucedi� al generar "%1:s" de "%2:s".
#define M4_MDFAC_NO_SUCH_METHOD								0x00AC + M4_MDFAC_ERROR_BASE
// No existe el m�todo de negocio.\n \nDebe definir el m�todo de negocio "%0:s" o no utilizarlo.\nEsto sucedi� al generar "%2:s" de "%1:s".
#define M4_MDFAC_BAD_TOTAL_TAG_START_DATE					0x00AD + M4_MDFAC_ERROR_BASE
// Fecha de inicio fuera de rango.\n \nLa fecha de inicio "%0:d" de la condici�n de ejecuci�n de salida a total "%1:s" de "%2:s" est� fuera del rango permitido por "%3:d" y "%4:d" de la salida a total "%5:s".\nDebe cambiar el periodo de la salida a total.\nEsto sucedi� en "%6:s" de "%7:s".
#define M4_MDFAC_BAD_TOTAL_TAG_END_DATE						0x00AE + M4_MDFAC_ERROR_BASE
// Fecha de fin fuera de rango.\n \nLa fecha de fin "%0:d" de la condici�n de ejecuci�n de salida a total "%1:s" de "%2:s" est� fuera del rango permitido por "%3:d" y "%4:d" de la salida a total "%5:s".\nDebe cambiar el periodo de la salida a total.\nEsto sucedi� en "%6:s" de "%7:s".
#define M4_MDFAC_INCOHERENT_TOTAL_TAG						0x00AF + M4_MDFAC_ERROR_BASE
// Periodo incoherente.\n \nLa condici�n de ejecuci�n "%0:s" para la salida a total "%1:s" de "%2:s" de "%3:s" tiene un periodo cuya fecha de inicio "%4:d" es posterior a la fecha de fin "%5:d".\nDebe cambiar el periodo de la condici�n de ejecuci�n.\nEsto sucedi� en "%6:s".
#define M4_MDFAC_REPEATED_TOTAL_TAG							0x00B0 + M4_MDFAC_ERROR_BASE
// Condici�n de ejecuci�n de salida a total sobrescrita.\n \nLa condici�n de ejecuci�n de salida a total "%0:s" para la salida a total "%1:s" de "%2:s" de "%3:s" se sobrescribi� en el periodo con fecha de inicio "%4:d" y fecha de fin "%5:d".\nDebe cambiar el �rbol de herencia de las condiciones de ejecuci�n de salidas a total.\nEsto sucedi� en "%6:s" de "%7:s".


// ============================================================================
// De la instalaci�n de f�rmulas
// ============================================================================

#define M4_MDFAC_ADD_TO_POOL_INS							0x0100 + M4_MDFAC_ERROR_BASE
// Recursos agotados.\n \nNo se pueden obtener recursos suficientes para instalar una constante alfanum�rica.\nDebe consultar con el administrador.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_BAD_POINTER_INS							0x0101 + M4_MDFAC_ERROR_BASE //solo debug
// Error interno de instalaci�n.\n \nEl apuntador "%3:s" no es correcto.\nDebe consultar con el administrador.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_CANT_CONNECT_ITEMS							0x0102 + M4_MDFAC_ERROR_BASE
// Conector err�neo.\n \nNo se puede conectar este elemento porque el elemento padre tiene un n�mero de argumentos variable.\nDebe desconectar estos dos elementos.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s". 
#define M4_MDFAC_FUNCTION_NOT_FOUND							0x0103 + M4_MDFAC_ERROR_BASE
// Funci�n de tipo CPP inexistente.\n \nEl elemento est� asignado a la funci�n "%3:s" que no existe.\nDebe cambiar el m�todo al que mapea el elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NO_ILLEGAL_INSTRUCTION_SERIALIZE 			0x0104 + M4_MDFAC_ERROR_BASE
// Tipo ilegal de instrucci�n.\n \nEl tipo de instrucci�n "%3:s" no es correcto.\nDebe revisar la integridad de sus datos o volver a compilar el elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NO_ITEM_FORMULA_PREGETSIZE					0x0105 + M4_MDFAC_ERROR_BASE
// C�digo compilado incorrecto.\n \nEl c�digo polaco "%3:s" no es correcto.\nDebe volver a compilar el elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NO_SUCH_ITEM_IN_VIEW						0x0106 + M4_MDFAC_ERROR_BASE
// Elemento inexistente.\n \nEl elemento "%3:s" de "%4:s" no existe en este tipo de cliente/servidor. Debe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NO_SUCH_NODE_IN_VIEW						0x0107 + M4_MDFAC_ERROR_BASE
// Nodo inexistente.\n \nNo existe el nodo "%3:s" en este tipo de cliente/servidor.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_T3_ALIAS_ID_INS							0x0108 + M4_MDFAC_ERROR_BASE
// Alias inexistente.\n \nEl ID de alias del Meta4Object "%3:s" no es correcto.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_TOO_MUCH_LABELS_INS           				0x0109+ M4_MDFAC_ERROR_BASE
// C�digo demasiado complejo.\n \nNo se puede instalar el c�digo del elemento por ser demasiado complejo.\nDebe simplificar el c�digo o partir en varios elementos.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_ARG_MAPPING  						0x010A + M4_MDFAC_ERROR_BASE
// Mapeo incorrecto de argumentos.\n \nNo existe argumento en la posici�n "%3:s" del elemento padre.\nDebe cambiar el mapeo de argumentos.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_ARG_TYPE_MAPPING						0x010B + M4_MDFAC_ERROR_BASE
// Tipo de mapeo de argumentos incorrecto.\n \nEl tipo de mapeo (a la salida o por referencia) no es correcto en conexiones previas. S�lo se permite mapeo de argumentos de tipo entrada. Debe cambiar los conectores.\n\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_CHANNEL_FUNCTION						0x010C + M4_MDFAC_ERROR_BASE
// Funci�n de Meta4Object inexistente.\n \nNo existe la funci�n "%3:s" de este Meta4Object.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_CHANNEL_INS							0x010D + M4_MDFAC_ERROR_BASE
// Meta4Object inexistente.\n \nNo existe el Meta4Object "%3:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_FUNCTION								0x010E + M4_MDFAC_ERROR_BASE
// Funci�n inexistente.\n \nLa funci�n "%3:s" no existe.\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_ITEM_HANDLE_INS						0x010F + M4_MDFAC_ERROR_BASE
// Elemento inexistente.\n \nNo existe el elemento "%3:s" de "%4:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_NODE_INS								0x0110 + M4_MDFAC_ERROR_BASE
// Nodo inexistente.\n \nNo existe el nodo "%3:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_OBJECT_FORMAT						0x0111 + M4_MDFAC_ERROR_BASE
// Formato incorrecto de objeto.\n \nAl instalar "%3:s" se detect� que el formato de llamadas es incorrecto.\nDebe volver a compilar el elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_PARAM_INPUT_INS						0x0112 + M4_MDFAC_ERROR_BASE
// Tipo incorrecto de argumento.\n \nArgumento "%3:s" para la regla llama a la funci�n CPP "%4:s" debe ser de entrada.\nDebe cambiar el tipo del argumento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_PARAM_NUM_INS_1						0x0113 + M4_MDFAC_ERROR_BASE
// N�mero de argumentos no v�lido.\n \nLa funci�n "%3:s" usa argumentos variables y la definici�n del elemento espera un n�mero fijo.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_RULE_TYPE							0x0114 + M4_MDFAC_ERROR_BASE
// Tipo de regla no valido.\n \nEl tipo de regla "%3:s" no es correcto.\nDebe cambiar el tipo de la regla.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_PARAM_NUM_INS_2						0x0115 + M4_MDFAC_ERROR_BASE
// N�mero de argumentos no v�lido.\n \nLa definici�n del elemento indica un n�mero de argumento variable; sin embargo, la funci�n "%3:s". espera un n�mero fijo.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_PARAM_NUM_INS_3						0x0116 + M4_MDFAC_ERROR_BASE
// N�mero de argumentos no v�lido.\n \nLa definici�n del elemento indica un n�mero de argumento menor al de la funci�n CPP "%3:s" (con un n�mero variable de argumentos). Son necesarios al menos "%4:s".\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_PARAM_NUM_INS_4						0x0117 + M4_MDFAC_ERROR_BASE
// N�mero de argumentos no v�lido.\n \nLa definici�n del elemento indica un n�mero de argumento distinto al de la funci�n "%3:s". Necesita "%4:s".\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_NOT_MAPPEABLE_FUNCTION						0x0118 + M4_MDFAC_ERROR_BASE
// Funci�n incorrecta.\n \nLa funci�n "%3:s" no se puede mapear a un elemento.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_PARAM_REF_INS						0x0119 + M4_MDFAC_ERROR_BASE
// Argumento incorrecto.\n \nArgumento "%3:s" para la regla que llama la funci�n CPP "%4:s" debe ser de tipo entrada.\nDebe cambiar la definici�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_NODE_POINTER_ITEM_INS				0x011A + M4_MDFAC_ERROR_BASE
// Error interno de instalaci�n.\n \nEl apuntador del nodo que se est� usando para buscar "%3:s" de "%4:s" no es correcto.\nDebe consultar al administrador de su sistema.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_ITEM_POINTER_RULE_INS				0x011B + M4_MDFAC_ERROR_BASE
// Error interno de instalaci�n.\n \nEl apuntador de elemento para buscar la regla "%3:s" no es correcto.\nDebe consultar con el administrador.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_T3_NODE_INSTANCE						0x011C + M4_MDFAC_ERROR_BASE
// Instancia no definida.\n \nNo existe esta instancia del alias de Meta4Object "%3:s".\nDebe cambiar el c�digo del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_NODE_POINTER_CONN					0x011D + M4_MDFAC_ERROR_BASE
// Nodo inexistente.\n \nNo existe el nodo conectado.\nDebe cambiar el conector.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_BAD_PREINSTALL						0x011E + M4_MDFAC_ERROR_BASE //debug only
// Error de instalaci�n.\n \n"%0:s". Error al preinstalar "%1:s".
#define M4_MDFAC_WRONG_NOTHING_TO_INSTALL					0x011F + M4_MDFAC_ERROR_BASE
// Regla vac�a.\n \nRegla vac�a en un elemento de tipo "%3:s".\nDebe completar la regla.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_WRONG_RULE_ORDER							0x0120 + M4_MDFAC_ERROR_BASE
// Orden de regla no v�lido.\n \nOrden de regla no v�lido "%6:s" para la regla "%5:s" de "%3:s" de "%4:s".\nDebe poner un orden dentro de rango.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDFAC_BAD_FUNCTION_INDEX_ITEM					0x0121 + M4_MDFAC_ERROR_BASE
// Funci�n de elemento de �ndice incorrecta.\n \nEl elemento "%0:s" del �ndice "%1:s" tiene una funci�n distinta de "IGUAL" y no es el �ltimo elemento del �ndice.\nDebe cambiar la funci�n del elemento de �ndice.\nEsto sucedi� en "%2:s" de "%3:s".
#define M4_MDFAC_BAD_CHANNEL_FUNCTION_USE					0x0122 + M4_MDFAC_ERROR_BASE
// Uso incorrecto de funci�n de Meta4Object.\n \nLa funci�n de Meta4Object "%3:s" no puede ser usada como funci�n C++ de un elemento.\nDebe cambiar la funci�n del elemento.\nEsto sucedi� al instalar "%0:s" de "%1:s" de "%2:s".
#define M4_MDRT_BAD_MAP_MAGIC_NUMBER						0x0123 + M4_MDFAC_ERROR_BASE
// Formato incorrecto.\n \nEl formato de codificaci�n de los mapeos es incorrecto.\nDebe consultar con el administrador.
#define M4_MDFAC_CACHE_PUT_MAP								0x0124 + M4_MDFAC_ERROR_BASE
// No se pudo almacenar definici�n de los mapeos "%0:s" en la cach�.\n \nDebe consultar con el administrador.
#define M4_MDRT_BAD_MAP_VERSION								0x0125 + M4_MDFAC_ERROR_BASE
// Versi�n incorrecta.\n \nLa versi�n del mapeos es "%0:s" y la de la aplicaci�n en curso es "%1:s".\nDebe instalar correctamente la aplicaci�n.




#endif // __M4FACRES_HPP__


