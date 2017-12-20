//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              channel
// File:                m4srvefcres.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                19-10-1999
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the error codes for the channel
//
//
//==============================================================================


#ifndef __M4SRVEFCRES_HPP__
#define __M4SRVEFCRES_HPP__


#include <m4res.hpp>


//-----------------------------------------------------------------------------
//									M4EXEFORCHANNEL
//-----------------------------------------------------------------------------

// PROXY's
#define M4_ERR_PROXY_OUT_OF_MEMORY				0x0000 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_CACHE					0x0001 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_ENGINE_PARAM				0x0002 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_PXSPACE_CACHE			0x0003 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_SUBSYS					0x0004 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_OBJDIR_DESER				0x0005 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_OBJDIR_SER					0x0006 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_CREATE_CHANNEL				0x0007 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_BUILD_CHANNEL				0x0008 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_IOD_WRITE					0x0009 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_IOD_READ					0x000A + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_REL_SPACE					0x000B + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_GET_PXSPACE				0x000C + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_WRONG_STATE				0x000D + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NO_PX_MANAGER				0x000E + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_EXECUTOR					0x000F + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_SESSION_CHANNEL			0x0010 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_BUILD_PXSPACE				0x0011 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_INF_PROXY_GENERIC_0					0x0012 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_EXEC_COMMAND				0x0013 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_DEBUG_VIEW					0x0014 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_GET_MAXNSPACE				0x0015 + M4_SRV_EXE_FOR_CHANNEL_BASE
#define M4_ERR_PROXY_NOT_EXECUTING				0x0016 + M4_SRV_EXE_FOR_CHANNEL_BASE
//"Error ejecutando comando de administración.\n \nNo se estaba ejecutando nada.\nDebe estar ejecutando para poder llamar a un comando de administración del ejecutor."
#define M4_ERR_PROXY_INCORRECT_SESSION_ID		0x0017 + M4_SRV_EXE_FOR_CHANNEL_BASE
//"Error en el id de sesión.\n \nEl id de sesión especificado no coincide con el id de sesión de ninguno de los espacios proxy.\nDebe comprobar que ha introducido correctamente el id de sesión."
#define M4_ERR_PROXY_SPACES_TO_ADD_BAD_FORMAT	0x0018 + M4_SRV_EXE_FOR_CHANNEL_BASE
//"Error en el formato del número de espacios proxy a añadir.\n \nHa introducido una cadena de caracteres o un número negativo.\nDebe introducir un entero positivo entre 1 y %0:s."
#define M4_ERR_PROXY_CAN_NOT_CREATE_MORE_SPACES	0x0019 + M4_SRV_EXE_FOR_CHANNEL_BASE
//"Error intentando añadir nuevos espacios proxy.\n \nNo hay suficientes conexiones lógicas de base de datos. Actualmente quedan disponibles para espacios proxys %0:s conexiones lógicas.\nDebe revisar la configuración del servidor."


// XML

// "X\nX\nX\n"

// "Error inicializando SAX.\n \nEl procesador SAX no ha sido inicializado correctamente.\nDebe verificar la instalación correcta del procesador SAX."
#define M4_ERR_XML_SAX_INIT						0x1000 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Memoria agotada.\nSe ha agotado los recursos (memoria RAM).\nDebe instalar más memoria o reconfigurar la aplicación.\n"
#define M4_ERR_XML_OUT_OF_MEMORY				0x1001 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nLa inicialización de las cachés de estado ha fallado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_INIT_STATE_CACHE				0x1002 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl entorno de la M4ObjectEngine ha fallado.\nDebe configurar correctamente la aplicación.\n"
#define M4_ERR_XML_INIT_CVM_ENV					0x1003 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nHa fallado la inicialización del gestor de la M4ObjectEngine.\nDebe configurar correctamente la aplicación.\n"
#define M4_ERR_XML_INIT_CM						0x1004 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos de entrada.\nLos datos de entrada en el servidor no han podido ser leídos.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_GET_INPUT_DATA				0x1005 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos de salida.\nLos datos de salida en el servidor no han podido ser leídos.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_GET_OUTPUT_DATA				0x1006 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error liberando información de sesión.\nLa información de sesión en el servidor no ha podido ser liberada.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SESSION_NULL					0x1007 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error liberando conexión.\nHa habido un error liberando una conexión lógica de la LDB.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FREE_CONN					0x1008 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando M4Objects.\nError borrando los M4Objects del gestor.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_RESET_DM						0x1009 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Ejecutor ya eliminado.\nSe intenta parar un ejecutor que está estaba parado. El comando será ignorado.\n\n"
#define M4_ERR_XML_NEW_RESET					0x100a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando gestor.\nEl gestor de M4Objects no ha podido ser borrado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DM_SHUTDOWN					0x100b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error finalizando entorno.\nHa ocurrido un error finalizando el entorno de la M4ObjectEngine.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ENV_END						0x100c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de errores.\nLa petición de volcado del ejecutor es inválida.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DUMP_ERROR					0x100d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de errores.\nLa inicialización del objeto de errores del ejecutor ha fallado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DIMP_NO_FILE					0x100e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de errores\nEl volcado de los errores no ha sido efectuado con éxito.\nDebe comprobar que hay espacio en disco.\n"
#define M4_ERR_XML_DUMP_ERROR_OUT				0x100f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de volcado\nEl objeto de administración de comandos no ha sido creado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DUMP_CMD_TAG					0x1010 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de volcado.\nEl comando código %0:n no existe.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DUMP_UNK_COMMAND				0x1011 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando cachés.\nNo se ha inicializado correctamente las cachés del ejecutor.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_INIT_CACHE					0x1012 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Datos de entrada truncados.\nLos datos de entrada no contienen bloques.\nCompruebe que los datos fueron enviados correctamente.\n"
#define M4_ERR_XML_INPUT_TRUNC					0x1013 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos.\nError iterando por datos de la PDU.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_PDU_SCAN						0x1014 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos.\nLa petición XML está mal formada.\nRevise el formado de los datos enviados.\n"
#define M4_ERR_XML_PARSE_ERROR					0x1015 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl objeto SAXHandlers no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SAX_INIT_INTERNAL			0x1016 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Petición de interrupción.\nSe ha detectado una petición de interrupción.\n\n"
#define M4_ERR_XML_REQ_ABORT					0x1017 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando método\nError procesando método "%0:s" en el contexto "%1:s".\nDebe comprobar que el método es válido.\n"
#define M4_ERR_XML_PROC_METHOD_VALID			0x1018 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error procesando argumento.\nEl argumento ha sido asignado a un método inválido.\nDebe comprobar que el método es válido.\n"
#define M4_ERR_XML_ARG_ERROR					0x1019 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de decodificación.\nNo se ha podido decodificar el ítem "%0:s" de la posición %1:s!%2:s[%3:s].\nDebe comprobar que la codificación es correcta.\n"
#define M4_ERR_XML_ITEM_DECODE64				0x101a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error volcando errores.\nNo se han volcado los errores correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DUMP_LOG						0x101b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error agregando argumento.\nSe ha intentado añadir un argumento a un método inválido. Ha ocurrido en el ítem "%0:s!%1:s.%2:s".\nDebe comprobar que el método es válido.\n"
#define M4_ERR_XML_ADD_ARG_INVALID				0x101c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Índice de argumento inválido.\nSe ha intentado introducir más argumentos de los esperados. Ha ocurrido en el ítem "%0:s!%1:s.%2:s".\nCompruebe el número de argumentos necesarios.\n"
#define M4_ERR_XML_TOO_MANY_ARGS				0x101d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl objeto no ha sido inicializado correctamente.\nDebe consultar otros errores.\n"
#define M4_ERR_XML_EXECUTE_NO_ACCESS			0x101e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nNo se ha podido añadir un elemento a la pila de argumentos. Ha ocurrido en el ítem "%0:s!%1:s.%2:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_PUSH_ARGS					0x101f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de inicialización.\nEl objeto método no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_METHOD_INIT					0x1020 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos XML.\nHa ocurrido un error: "%0:s" procesando la entidad "%1:s" identificador "%2:s" en la línea %3:n columna %4:n.\nDebe comprobar que la sintaxis XML de entrada es correcta.\n"
#define M4_ERR_XML_DUMP_MESSAGE					0x1021 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Argumento de traducción inválido.\nArgumento "%0:s" es inválido para traducción.\nDebe comprobar si el argumento existe.\n"
#define M4_ERR_XML_TRANSL_ARG_INVALID			0x1022 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de inicialización.\nEl objeto de traducción no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_TRANSL_ACCESS				0x1023 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nNo se ha podido sacar un valor de la pila durante la traducción. Esto ha ocurrido en el método "%0:s".\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_TRANSL_POP					0x1024 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Ítem para traducción inválido.\nEl ítem "%0:s" es inválido para traducción.\nDebe comprobar que el ítem existe.\n"
#define M4_ERR_XML_TRANSL_ADD_ITEM				0x1025 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno\nNo se ha introducido un ítem en un mapa correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_TRANSL_ITEM_INTERNAL			0x1026 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo de traducción inválido.\nEl nodo "%0:s" no existe en el M4Object de multi validación.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_TRANSL_EXEC_NODE				0x1027 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error Interno.\nEl objeto de traducción no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ADD_TRANSL_ITEM				0x1028 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de invocación de método.\nEl método de traducción debe tener argumentos.\nDebe comprobar que el método de traducción es correcto.\n"
#define M4_ERR_XML_TRANSL_MIN_ARGS				0x1029 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de traducción.\nEl valor de retorno debe ser entero.\nDebe comprobar el método de traducción.\n"
#define M4_ERR_XML_TRANSL_INVALID_RESULT		0x102a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nLos datos de entrada son inválidos.\nDebe comprobar que los datos enviados son correctos.\n"
#define M4_ERR_XML_PROC_IN_FILES_INVALID		0x102b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de datos de entrada.\nEl primer bloque de los datos de entrada debe ser DATA_UNIT y los demás FILE_UNIT.\nDebe comprobar que los datos enviados son correctos.\n"
#define M4_ERR_XML_INPUT_MISC					0x102c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serialización.\nNo se ha podido serializar referencia al M4Object de multi validación.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_ACC			0x102d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serialización.\nNo se ha podido serializar el mapa de referencias.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_MAP			0x102e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nNo se ha podido deserializar referencia al M4Object de multi validación.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_ACC			0x102f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nNo se ha podido deserializar el mapa de referencias.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_MAP			0x1030 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl contexto XML no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_INIT					0x1031 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error procesando la pila.\nNo se ha podido sacar un valor de la pila de ejecución.\nPosiblemente el número de parámetros "%0:n" es incorrecto.\n"
#define M4_ERR_XML_CONTEXT_POP					0x1032 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nError de inicialización ejecutando ítem.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_EXECUTE_INIT			0x1033 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo inválido.\nEl nodo no existe. Esto ha ocurrido ejecutando el ítem "%0:s" del nodo "%1:s".\nDebe comprobar que el nodo existe.\n"
#define M4_ERR_XML_CHANNEL_ITEM_INVALID_NODE	0x1034 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Ítem inválido.\nEl ítem "%0:s|%1:s.%2:s" es inválido.\nDebe asegurarse que el ítem existe.\n"
#define M4_ERR_XML_ITEM_INVALID					0x1035 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl objeto nodo no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_INIT					0x1036 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando valor.\nEl valor del ítem "%0:s|%1:s.%2:s" no ha podido ser aplicado.\nDebe comprobar que el ítem existe y que el valor es aplicable.\n"
#define M4_ERR_XML_ITEM_SET_VALUE				0x1037 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando descripción.\nSe ha producido un error aplicando la descripción de una referencia a fichero en el ítem "%0:s|%1:s.%2:s".\nDebe comprobar que el ítem existe y que la descripción es válida.\n"
#define M4_ERR_XML_ITEM_SET_DESCR				0x1038 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando valor nulo.\nError de aplicación de valor nulo al ítem "%0:s|%1:s.%2:s".\nDebe comprobar que el ítem existe.\n"
#define M4_ERR_XML_ITEM_SET_NULL				0x1039 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando valor nulo.\nError de aplicación de valor nulo al ítem "%0:s|%1:s.%2:s".\nDebe comprobar que el ítem existe.\n"
#define M4_ERR_XML_ITEM_SET_NULL_VALUE			0x103a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de posicionamiento.\n El alias "%0:s" no ha podido ser generado.\nDebe comprobar que el M4Object asociado al alias existe.\n"
#define M4_ERR_XML_ITEM_MOVE_NO_ALIAS			0x103b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error eliminando filtro.\n El alias "%0:s" no ha podido ser generado eliminando filtro.\nDebe comprobar que el M4Object asociado al alias existe.\n"
#define M4_ERR_XML_ITEM_REMOVE_FILTER_NO_ALIAS	0x103c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error eliminando filtro.\nNo se ha podido eliminar filtro "%0:s" en todas las vistas del M4Object asignado a "%1:s".\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_REMOVE_FILTER			0x103d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando filtro.\nEl alias "%0:s" no existe.\nDebe comprobar que el M4Object existe.\n"
#define M4_ERR_XML_ITEM_FILTER_NO_ALIAS			0x103e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando filtro.\nHa ocurrido un error aplicando el filtro "%0:s" de tipo "%1:s" al M4Object asignado a "%2:s".\nDebe comprobar que los valores son correctos.\n"
#define M4_ERR_XML_ITEM_APPLY_FILTER			0x103f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando filtro.\nNo se ha podido aplicar el filtro tipo LN4 "%0:s" a todas las vistas del M4Object "%1:s".\nDebe comprobar que el método es correcto y que el M4Object existe.\n"
#define M4_ERR_XML_ITEM_APPLY_LN4				0x1040 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo el valor de un ítem.\nEsto ocurrió en el ítem del nodo "%0:s" del M4Object "%1:s".\nDebe comprobar que el ítem existe.\n"
#define M4_ERR_XML_ITEM_GET_VALUE				0x1041 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo el valor de un ítem.\nEl ítem "%0:s!%1:s.%2:s" no ha podido ser leído.\nDebe comprobar que el ítem existe.\n"
#define M4_ERR_XML_ITEM_GET_VALUE_GET			0x1042 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serialización.\nNo se ha podido serializar las referencias a los M4Objects.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CURRENT_SERIAL				0x1043 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nNo se ha podido deserializar las referencias a los M4Objects.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CURRENT_DESERIAL				0x1044 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nNo se ha podido acceder al nodo "%0:s".\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CURRENT_DESERIAL_NODE		0x1045 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nError inicializando objeto de volcado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_INIT				0x1046 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de nodo.\nNo existe un alias definido para el volcado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_NO_ALIAS			0x1047 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de nodo.\nEl alias "%0:s" no existe.\nDebe comprobar que el M4Object asociado a este alias existe.\n"
#define M4_ERR_XML_FILTER_LIST_BAD_ALIAS		0x1048 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de nodo.\nEl nodo "%0:s" no es válido.\nDebe comprobar que el nodo existe.\n"
#define M4_ERR_XML_FILTER_LIST_BAD_NODE			0x1049 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de registro.\nEl registro es inválido.\nDebe seleccionar correctamente el registro para volcado.\n"
#define M4_ERR_XML_FILTER_LIST_BAD_REGISTER		0x104a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de ítem.\nSe ha producido volcando el ítem "%0:s" del alias "%1:s".\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_DUMP_ITEM		0x104b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de ítem.\nLa descripción del ítem "%0:s" del alias "%1:s" no ha podido ser traducida.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_DESCR			0x104c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de traducción.\nError invocando método de traducción.\nDebe comprobar que la especificación de la traducción es correcta.\n"
#define M4_ERR_XML_FILTER_LIST_DUMP_TRANSL		0x104d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de conectores.\nEl conector inverso es inválido.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_INV_CONNECTOR	0x104e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo padre inválido.\nEl nodo padre no es válido.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_INVALID_PARENT	0x104f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo padre inválido.\nLa especificación del M4Object no es consistente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_NO_PARENT		0x1050 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Registro inválido.\nEl registro "%0:s" no es válido.\nDebe posicionarse correctamente.\n"
#define M4_ERR_XML_FILTER_LIST_UPD_INVALID_REG	0x1051 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error procesando lista de volcado.\nNo hay elementos en la lista.\nDebe especificar elementos para el volcado.\n"
#define M4_ERR_XML_FILTER_LIST_NO_LIST			0x1052 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl objeto de salida no ha sido inicializado correctamente.\nDebe comprobar que hay recursos suficientes para ejecutar la operación.\n"
#define M4_ERR_XML_STREAM_INIT					0x1053 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de codificación.\nHa fallado el proceso de traducción a Base64.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_STREAM_TRANSL64				0x1054 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de decodificación.\nHa fallado el proceso de traducción desde Base64.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_STREAM_DECODE64				0x1055 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error creando M4Object.\nEl M4Object "%0:s" no ha podido ser creado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_CREATE_CHANNEL		0x1056 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error construyendo M4Object.\nEl M4Object "%0:s" no ha podido ser construido.\nDebe comprobar que el objeto existe.\n"
#define M4_ERR_XML_CONTEXT_BUILD_CHANNEL		0x1057 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error invocando método de traducción.\nSe produjo ejecutando ítem "%0:s".\nDebe comprobar que la especificación de la traducción es correcta.\n"
#define M4_ERR_XML_CONTEXT_EXECUTE_TRANSL		0x1058 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error creando canal.\nNo se ha podido crear el canal de multi validación.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_TRANSL_CHANNEL		0x1059 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error construyendo canal.\nSe ha producido un error creando el canal de multi validación "%0:s".\nDebe comprobar el canal existe en la base de datos.\n"
#define M4_ERR_XML_CONTEXT_TRANSL_BUILD			0x105a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando método.\nSe ha producido un error ejecutando el ítem "%0:s".\nDebe comprobar otros errores.\n"
#define M4_ERR_XML_ITEM_EXECUTE					0x105b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando método de M4Object.\nEl método "%0:s" no ha sido ejecutado con éxito.\nDebe comprobar otros errores.\n"
#define M4_ERR_XML_CHANNEL_ITEM_EXECUTE			0x105c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando método.\nNo se ha podido sacar un valor de la pila de ejecución.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_EXECUTE_POP					0x105d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando cachés de estado.\nLa caché de estados no ha podido ser borrada.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_STATE_CLEAN					0x105e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando método.\nHa ocurrido un error en la ejecución del código XML.\nDebe consultar otros errores.\n"
#define M4_ERR_XML_EXECUTE						0x105f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando contexto.\nHa habido un error borrando M4Object's.\nDebe consultar otros errores.\n"
#define M4_ERR_XML_RESET_STATE					0x1060 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error eliminando M4Object.\nEl M4Object "%0:s" no ha podido ser borrado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CURRENT_DESTROY				0x1061 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo inválido.\nError borrando filtro. El nodo "%0:s" no es válido.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_REMOVE_FILTER_NO_NODE	0x1062 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo inválido.\nError Ejecutando método. El nodo "%0:s" no es válido.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_EXECUTE_INVALID_NODE	0x1063 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de conversión.\nEl tipo "%0:n" no puede ser convertido a partir de una cadena.\nDebe comprobar que el tipo del ítem destino es válido.\n"
#define M4_ERR_XML_CONV_ARG_INVALID_TYPE		0x1064 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de conversión.\nEl tipo "%0:n" no es un tipo destino válido.\nDebe comprobar que el tipo del ítem destino es válido.\n"
#define M4_ERR_XML_CONV_ARG_INVALID				0x1065 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de argumento.\nEl argumento "%0:s" no pudo ser convertido. Ha ocurrido en el ítem "%1:s" del nodo "%2:s".\nDebe comprobar el tipo del ítem destino.\n"
#define M4_ERR_XML_ADD_ARG_INVALID_TYPE			0x1066 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error en la definición del alias.\nEl M4Object "%0:s" de tipo "%1:n" no ha sido encontrado.\nDebe comprobar que el objeto existe."
#define M4_ERR_XML_CONTEXT_FIND_CHANNEL			0x1067 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Identificador de M4Object inválido.\nDebe introducir un identificador válido.\nDebe comprobar el identificador."
#define M4_ERR_XML_FIND_CHANNEL_TRUNC			0x1068 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Referencia a un objeto con más de una instancia..\nEl identificador "%0:s" se refiere a un objeto instanciado más de una vez.\nDebe corregir la referencia."
#define M4_ERR_XML_FIND_MULTI					0x1069 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Instancia no encontrada.\nNo se ha encontrada una instancia de un M4Object con identificador "%0:s".\nDebe referenciar un objeto instanciado."
#define M4_ERR_XML_FIND_NOT_FOUND				0x106a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Instancia de nivel 2 no encontrada.\nLa instancia "%0:s" no ha sido encontrada.\nDebe utilizar un identificador válido."
#define M4_ERR_XML_FIND_L2_NOT_FOUND			0x106b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de búsqueda.\nLa función de búsqueda interna de nivel 2 ha fallado.\nDebe consultar con el administrador."
#define M4_ERR_XML_FIND_L2_ERROR				0x106c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error buscando subsistema.\nEl subsistema "%0:s" no ha sido encontrado.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_GET_SUBSYS				0x106d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nLa caché de estados no ha sido inicializada correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_GET_STACHE				0x106e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando sesión.\nLa sesión "%0:n" no ha podido ser borrada correctamente de la caché de estados.\nDebe consultar con el administrador."
#define M4_ERR_XML_CLEAR_SESSION				0x106f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando M4Object.\nEl M4Object "%0:s" no ha podido ser borrado.\nDebe consultar otros errores."
#define M4_ERR_XML_CONTEXT_BUILD_DESTROY		0x1070 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando M4Object de traducción.\nEl M4Object "%0:s" no ha podido ser borrado.\nDebe consultar otros errores."
#define M4_ERR_XML_TRANSL_BUILD_DESTROY			0x1071 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo inválido.\nEl nodo "%0:s" no es válido. Ocurrió en el M4Object alias "%1:s".\nDebe comprobar que el nombre del nodo y M4Object alias son correctos."
#define M4_ERR_XML_ITEM_INVALID_NODE			0x1072 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error definiendo alias.\nEl nombre del alias es inválido.\nDebe comprobar la definición del alias."
#define M4_ERR_XML_INVALID_ALIAS_NAME			0x1073 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nSe ha encontrado un tipo CPP no válido.\nDebe consultar con el administrador."
#define M4_ERR_XML_INTERNAL_CPPTYPE				0x1074 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de conversión.\nNo se ha podido convertir la cadena "%0:s" al tipo CPP "%1:n" en la propiedad "%2:s". Esto ocurrió en el M4Object "%3:s".\nDebe comprobar que los parámetros para la conversión."
#define M4_ERR_XML_CPP_INVALID_CONV				0x1075 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando propiedad.\nNo se ha podido aplicar el valor "%0:s" de tipo CPP "%1:n" en la propiedad "%2:s". Esto ocurrió en el M4Object "%3:s".\nDebe comprobar que los parámetros para la conversión."
#define M4_ERR_XML_CPP_INVALID_SET_PROP			0x1076 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando atributo.\nEl atributo "%0:s" no ha podido ser inicializado.\nDebe consultar con el administrador."
#define M4_ERR_XML_SET_PROP_ERROR				0x1077 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Comando de administración inválido.\nEl comando de código "%0:n" no es válido.\nDebe consultar con el administrador."
#define M4_ERR_XML_INVALID_ADMIN_CMD			0x1078 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nError interno procesando comando "%0:n". Posiblemente falta de memoria.\nDebe consultar con el administrador."
#define M4_ERR_XML_INTERNAL_CMD_ERROR			0x1079 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nLa caché de estados no ha sido inicializada correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_CACHE_INIT_ERROR				0x107a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl comando "FLUSH" no ha sido ejecutado correctamente.\nDebe consultar otros errores."
#define M4_ERR_XML_COMMAND_FLUSH				0x107b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nError evaluando número de objetos en la caché de estados.\nDebe consultar otros errores."
#define M4_ERR_XML_COMMAND_GETNOBJS				0x107c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl comando de borrado de estado ha fallado debido a un parámetro inválido.\nDebe consultar con el administrador."
#define M4_ERR_XML_COMMAND_REMOVE_PARAM			0x107d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl valor de la sesión para el comando de borrado "%0:n" es inválido.\nDebe corregir el valor del argumento."
#define M4_ERR_XML_COMMAND_SESSION_PARAM		0x107e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl comando de borrado de la caché ha fallado.\nDebe consultar otros errores."
#define M4_ERR_XML_COMMAND_REMOVE				0x107f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl alias "%0:s" no es válido para traducción.\nDebe comprobar la definición del alias."
#define M4_ERR_XML_TRANSL_NO_ALIAS				0x1080 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl alias "%0:s" no ha sido inicializado correctamente. La vista no ha porido ser creada.\nDebe consultar con el administrador."
#define M4_ERR_XML_TRANSL_NO_ACCESS				0x1081 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "No se ha podido procesar la propiedad.\nLa propieadad "%0:s" no ha podido ser leída.\nDebe consultar con el administrador."
#define M4_ERR_XML_TRANSL_NO_CONVERT			0x1082 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo estado.\nNo se ha podido leer el estado para la sesión "%0:s".\nDebe consultar con el administrador."
#define M4_ERR_XML_LOAD_STATE					0x1083 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando estado.\nNo se ha podido inicializar el estado para la sesión "%0:s".\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_SESSION					0x1084 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando sesión.\nLa subsesión "%0:s" no ha podido ser inicializada.\nDebe consultar con el administrador."
#define M4_ERR_XML_SET_SESSION_CHANNEL			0x1085 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error persistiendo estado.\nEl estado para la subsesión "%0:s" no ha podido ser persistido.\nDebe consultar otros errores."
#define M4_ERR_XML_PERSIST_STATE				0x1086 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nLa sesión no ha sido inicializada correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_SESSION_INTERNAL		0x1087 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de inicialización.\nEl objecto "DataStorage" no ha podido ser inicializado correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_SESSION_DATA_STORE		0x1088 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando objecto interno.\nEl objeto interno de serialización no ha sido inicializado correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_IOD_ERROR				0x1089 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serialización.\nHa ocurrido un error de serialización en la inicialización de la sesión.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_SERIALIZE				0x108a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nHa ocurrido un error de deserialización en la inicialización de la sesión.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_DESERIAL				0x108b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicialización.\nEl M4Object de sesión no ha podido ser aplicado correctamente.\nDebe consultar otros errores."
#define M4_ERR_XML_SET_SESSION_CHANNEL_BASE		0x108c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de consistencia.\nEl objeto sesión aplicado no es auto consistente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INCONSISTENCE_SESSION		0x108d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl parámetro "%0:s" no corresponde a ningún identificador de sesión válido.\nDebe revisar el valor del parámetro."
#define M4_ERR_XML_COMMAND_SESSION_NOT_FOUND	0x108e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nSe ejecutó un comando contra un espacio proxy inválido.\nDebe consultar con el administrador."
#define M4_ERR_XML_INVALID_STATE				0x108f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error agregando argumento.\nSe ha intentado añadir un argumento inválido a un método. Ha ocurrido en argumento "%3:s" el ítem "%0:s!%1:s.%2:s".\nDebe comprobar que el argumento es válido.\n"
#define M4_ERR_XML_ADD_ARG_INVALID_ARG			0x1090 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error agregando argumentos.\nSe han utilizado insuficientes argumentos fijos (%3:d de %4:d necesarios). Ha ocurrido en el ítem "%0:s!%1:s.%2:s".\nDebe utilizar más argumentos.\n"
#define M4_ERR_XML_NOT_ENOUGH_FIXED_ARGS		0x1091 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno en la caché de estados.\nLa caché de estados se encuentra en un estado inválido.\nDebe consultar con un administrador."
#define M4_ERR_XML_CACHE_INTERNAL_ERROR 		0x1092 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serialización.\nNo se ha podido serializar el nivel de persistencia.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_PERS_LEVEL	0x1093 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nNo se ha podido deserializar el nivel de persistencia.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_PERS_LEVEL	0x1094 + M4_SRV_EXE_FOR_CHANNEL_BASE

//Error al encriptar datos.\n \nSe ha producido un error al encriptar los datos en la caché de XML.\nDebe consultar con el administrador.
#define M4_ERR_XML_NO_ENCRYPT					0x1095 + M4_SRV_EXE_FOR_CHANNEL_BASE
//Error al desencriptar datos.\n \nSe ha producido un error al desencriptar los datos en la caché de XML.\nDebe consultar con el administrador.
#define M4_ERR_XML_NO_DECRYPT					0x1096 + M4_SRV_EXE_FOR_CHANNEL_BASE

// "Error definiendo alias.\nEl nombre del alias para la referencia es inválido.\nDebe comprobar la definición de la referencia."
#define M4_ERR_XML_INVALID_ALIAS_REF			0x1097 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error definiendo alias.\nEl nombre del la referencia para nivel 2 es inválido.\nDebe comprobar la definición de la referencia."
#define M4_ERR_XML_INVALID_ALIAS_L2_REF			0x1098 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error definiendo alias.\nLa instancia del objeto referenciado es inválida.\nDebe comprobar la definición de la referencia."
#define M4_ERR_XML_INVALID_M4O_INSTANCE			0x1099 + M4_SRV_EXE_FOR_CHANNEL_BASE

// "Error de serialización.\nNo se ha podido serializar el contexto de autenticación.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_AUTH_INFO		0x109a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nNo se ha podido deserializar el contexto de autenticación.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_AUTH_INFO	0x109b + M4_SRV_EXE_FOR_CHANNEL_BASE

// Autenticación

// "Error de autenticación.\nNo se ha podido obtener la factoría de objetos.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_OBJ_SERVICE				0x109c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha podido obtener el gestor de objetos.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_CHANNEL_MANAGER			0x109d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha podido creear el objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_CREATE_CHANNEL			0x109e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha podido construir el objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_BUILD_CHANNEL			0x109f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha podido destruir el objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_BUILD_DESTROY			0x10a0 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha crear un acceso al objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_CREATE_ACCESS			0x10a1 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha obtener el node %0:s del objeto %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_GET_NODE				0x10a2 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha obtener el ejecutor para el objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_GET_EXECUTOR			0x10a3 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha podido añadir un argumento a un método del objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_PUSH_ARG				0x10a4 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha podido ejecutor el método %0:s del objeto %1:s. El valor de retorno ha sido %2:n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_EXECUTE_CALL			0x10a5 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha podido ejecutor el código ln4 del método %0:s del objeto %1:s. El valor de retorno ha sido %2:n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_EXECUTE_LN4				0x10a6 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nNo se ha podido recuperar el valor del item %0:s del objeto del objeto %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_GET_PARAM_VALUE			0x10a7 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticación.\nEl tipo/contenido del item no es válido. Eso ocurrió para el item  %0:s del objeto del objeto %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_GET_INVALID_VALUE		0x10a8 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serialización.\nError serializando la clave de autenticación.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_AUTH_PASS		0x10a9 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nError deserializando la clave de autenticación.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_AUTH_PASS	0x10aa + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de desencriptación.\nError desencriptando cadena.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DECRYPT						0x10ab + M4_SRV_EXE_FOR_CHANNEL_BASE

// security

// "Error de asignación.\nNo tiene permisos para modificar el elemento %0:s del nodo %1:s del Meta4Object %2:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SEC_ITEM_SET_VALUE			0x10ac + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de ejecución.\nNo tiene permisos para ejecutar el elemento %0:s del nodo %1:s del Meta4Object %2:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SEC_ITEM_EXECUTE				0x10ad + M4_SRV_EXE_FOR_CHANNEL_BASE

// deltas

// "Error de inicialización.\nError inicializando vector de deltas.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DELTA_INIT					0x10ae + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nNo se ha podido asignar el id único de registro. Ha ocurrido en el ítem "%0:s!%1:s.%2:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SET_RECORD_UNIQUE_ID			0x10af + M4_SRV_EXE_FOR_CHANNEL_BASE

// m4xml session encryption

// "Error de serialización.\nNo se ha podido serializar el id único del contexto.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_UUID			0x10b0 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserialización.\nNo se ha podido deserializar el id único del contexto.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_UUID		0x10b1 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de encriptación.\nError encriptando cadena.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ENCRYPT						0x10b2 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de desencriptación.\nNo se ha podido desencriptar el elemento %0:s del nodo %1:s del alias %2:s para los grupos funcionales %3:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_DECRYPT_VALUE			0x10b3 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de desencriptación.\nNo se ha podido desencriptar argumento %0:s del item %1:s del nodo %2:s para los grupos funcionales %3:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ADD_ARG_ERROR_DECRYPT		0x10b4 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de generación de UUID.\nNo se ha podido generar el UUID de contexto.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_GENERATE_UUID				0x10b5 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de asignación de UUID.\nNo se ha podido asignar el UUID de contexto.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ASSIGN_UUID					0x10b6 + M4_SRV_EXE_FOR_CHANNEL_BASE

// "X\nX\nX"
#define edX				0x1080 + M4_SRV_EXE_FOR_CHANNEL_BASE


#endif
