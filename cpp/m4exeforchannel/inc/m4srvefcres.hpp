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
//"Error ejecutando comando de administraci�n.\n \nNo se estaba ejecutando nada.\nDebe estar ejecutando para poder llamar a un comando de administraci�n del ejecutor."
#define M4_ERR_PROXY_INCORRECT_SESSION_ID		0x0017 + M4_SRV_EXE_FOR_CHANNEL_BASE
//"Error en el id de sesi�n.\n \nEl id de sesi�n especificado no coincide con el id de sesi�n de ninguno de los espacios proxy.\nDebe comprobar que ha introducido correctamente el id de sesi�n."
#define M4_ERR_PROXY_SPACES_TO_ADD_BAD_FORMAT	0x0018 + M4_SRV_EXE_FOR_CHANNEL_BASE
//"Error en el formato del n�mero de espacios proxy a a�adir.\n \nHa introducido una cadena de caracteres o un n�mero negativo.\nDebe introducir un entero positivo entre 1 y %0:s."
#define M4_ERR_PROXY_CAN_NOT_CREATE_MORE_SPACES	0x0019 + M4_SRV_EXE_FOR_CHANNEL_BASE
//"Error intentando a�adir nuevos espacios proxy.\n \nNo hay suficientes conexiones l�gicas de base de datos. Actualmente quedan disponibles para espacios proxys %0:s conexiones l�gicas.\nDebe revisar la configuraci�n del servidor."


// XML

// "X\nX\nX\n"

// "Error inicializando SAX.\n \nEl procesador SAX no ha sido inicializado correctamente.\nDebe verificar la instalaci�n correcta del procesador SAX."
#define M4_ERR_XML_SAX_INIT						0x1000 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Memoria agotada.\nSe ha agotado los recursos (memoria RAM).\nDebe instalar m�s memoria o reconfigurar la aplicaci�n.\n"
#define M4_ERR_XML_OUT_OF_MEMORY				0x1001 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nLa inicializaci�n de las cach�s de estado ha fallado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_INIT_STATE_CACHE				0x1002 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl entorno de la M4ObjectEngine ha fallado.\nDebe configurar correctamente la aplicaci�n.\n"
#define M4_ERR_XML_INIT_CVM_ENV					0x1003 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nHa fallado la inicializaci�n del gestor de la M4ObjectEngine.\nDebe configurar correctamente la aplicaci�n.\n"
#define M4_ERR_XML_INIT_CM						0x1004 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos de entrada.\nLos datos de entrada en el servidor no han podido ser le�dos.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_GET_INPUT_DATA				0x1005 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos de salida.\nLos datos de salida en el servidor no han podido ser le�dos.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_GET_OUTPUT_DATA				0x1006 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error liberando informaci�n de sesi�n.\nLa informaci�n de sesi�n en el servidor no ha podido ser liberada.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SESSION_NULL					0x1007 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error liberando conexi�n.\nHa habido un error liberando una conexi�n l�gica de la LDB.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FREE_CONN					0x1008 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando M4Objects.\nError borrando los M4Objects del gestor.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_RESET_DM						0x1009 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Ejecutor ya eliminado.\nSe intenta parar un ejecutor que est� estaba parado. El comando ser� ignorado.\n\n"
#define M4_ERR_XML_NEW_RESET					0x100a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando gestor.\nEl gestor de M4Objects no ha podido ser borrado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DM_SHUTDOWN					0x100b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error finalizando entorno.\nHa ocurrido un error finalizando el entorno de la M4ObjectEngine.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ENV_END						0x100c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de errores.\nLa petici�n de volcado del ejecutor es inv�lida.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DUMP_ERROR					0x100d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de errores.\nLa inicializaci�n del objeto de errores del ejecutor ha fallado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DIMP_NO_FILE					0x100e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de errores\nEl volcado de los errores no ha sido efectuado con �xito.\nDebe comprobar que hay espacio en disco.\n"
#define M4_ERR_XML_DUMP_ERROR_OUT				0x100f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de volcado\nEl objeto de administraci�n de comandos no ha sido creado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DUMP_CMD_TAG					0x1010 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de volcado.\nEl comando c�digo %0:n no existe.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DUMP_UNK_COMMAND				0x1011 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando cach�s.\nNo se ha inicializado correctamente las cach�s del ejecutor.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_INIT_CACHE					0x1012 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Datos de entrada truncados.\nLos datos de entrada no contienen bloques.\nCompruebe que los datos fueron enviados correctamente.\n"
#define M4_ERR_XML_INPUT_TRUNC					0x1013 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos.\nError iterando por datos de la PDU.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_PDU_SCAN						0x1014 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos.\nLa petici�n XML est� mal formada.\nRevise el formado de los datos enviados.\n"
#define M4_ERR_XML_PARSE_ERROR					0x1015 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl objeto SAXHandlers no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SAX_INIT_INTERNAL			0x1016 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Petici�n de interrupci�n.\nSe ha detectado una petici�n de interrupci�n.\n\n"
#define M4_ERR_XML_REQ_ABORT					0x1017 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando m�todo\nError procesando m�todo "%0:s" en el contexto "%1:s".\nDebe comprobar que el m�todo es v�lido.\n"
#define M4_ERR_XML_PROC_METHOD_VALID			0x1018 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error procesando argumento.\nEl argumento ha sido asignado a un m�todo inv�lido.\nDebe comprobar que el m�todo es v�lido.\n"
#define M4_ERR_XML_ARG_ERROR					0x1019 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de decodificaci�n.\nNo se ha podido decodificar el �tem "%0:s" de la posici�n %1:s!%2:s[%3:s].\nDebe comprobar que la codificaci�n es correcta.\n"
#define M4_ERR_XML_ITEM_DECODE64				0x101a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error volcando errores.\nNo se han volcado los errores correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DUMP_LOG						0x101b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error agregando argumento.\nSe ha intentado a�adir un argumento a un m�todo inv�lido. Ha ocurrido en el �tem "%0:s!%1:s.%2:s".\nDebe comprobar que el m�todo es v�lido.\n"
#define M4_ERR_XML_ADD_ARG_INVALID				0x101c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "�ndice de argumento inv�lido.\nSe ha intentado introducir m�s argumentos de los esperados. Ha ocurrido en el �tem "%0:s!%1:s.%2:s".\nCompruebe el n�mero de argumentos necesarios.\n"
#define M4_ERR_XML_TOO_MANY_ARGS				0x101d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl objeto no ha sido inicializado correctamente.\nDebe consultar otros errores.\n"
#define M4_ERR_XML_EXECUTE_NO_ACCESS			0x101e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nNo se ha podido a�adir un elemento a la pila de argumentos. Ha ocurrido en el �tem "%0:s!%1:s.%2:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_PUSH_ARGS					0x101f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de inicializaci�n.\nEl objeto m�todo no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_METHOD_INIT					0x1020 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo datos XML.\nHa ocurrido un error: "%0:s" procesando la entidad "%1:s" identificador "%2:s" en la l�nea %3:n columna %4:n.\nDebe comprobar que la sintaxis XML de entrada es correcta.\n"
#define M4_ERR_XML_DUMP_MESSAGE					0x1021 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Argumento de traducci�n inv�lido.\nArgumento "%0:s" es inv�lido para traducci�n.\nDebe comprobar si el argumento existe.\n"
#define M4_ERR_XML_TRANSL_ARG_INVALID			0x1022 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de inicializaci�n.\nEl objeto de traducci�n no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_TRANSL_ACCESS				0x1023 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nNo se ha podido sacar un valor de la pila durante la traducci�n. Esto ha ocurrido en el m�todo "%0:s".\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_TRANSL_POP					0x1024 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "�tem para traducci�n inv�lido.\nEl �tem "%0:s" es inv�lido para traducci�n.\nDebe comprobar que el �tem existe.\n"
#define M4_ERR_XML_TRANSL_ADD_ITEM				0x1025 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno\nNo se ha introducido un �tem en un mapa correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_TRANSL_ITEM_INTERNAL			0x1026 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo de traducci�n inv�lido.\nEl nodo "%0:s" no existe en el M4Object de multi validaci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_TRANSL_EXEC_NODE				0x1027 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error Interno.\nEl objeto de traducci�n no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ADD_TRANSL_ITEM				0x1028 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de invocaci�n de m�todo.\nEl m�todo de traducci�n debe tener argumentos.\nDebe comprobar que el m�todo de traducci�n es correcto.\n"
#define M4_ERR_XML_TRANSL_MIN_ARGS				0x1029 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de traducci�n.\nEl valor de retorno debe ser entero.\nDebe comprobar el m�todo de traducci�n.\n"
#define M4_ERR_XML_TRANSL_INVALID_RESULT		0x102a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nLos datos de entrada son inv�lidos.\nDebe comprobar que los datos enviados son correctos.\n"
#define M4_ERR_XML_PROC_IN_FILES_INVALID		0x102b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de datos de entrada.\nEl primer bloque de los datos de entrada debe ser DATA_UNIT y los dem�s FILE_UNIT.\nDebe comprobar que los datos enviados son correctos.\n"
#define M4_ERR_XML_INPUT_MISC					0x102c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serializaci�n.\nNo se ha podido serializar referencia al M4Object de multi validaci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_ACC			0x102d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serializaci�n.\nNo se ha podido serializar el mapa de referencias.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_MAP			0x102e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nNo se ha podido deserializar referencia al M4Object de multi validaci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_ACC			0x102f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nNo se ha podido deserializar el mapa de referencias.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_MAP			0x1030 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl contexto XML no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_INIT					0x1031 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error procesando la pila.\nNo se ha podido sacar un valor de la pila de ejecuci�n.\nPosiblemente el n�mero de par�metros "%0:n" es incorrecto.\n"
#define M4_ERR_XML_CONTEXT_POP					0x1032 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nError de inicializaci�n ejecutando �tem.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_EXECUTE_INIT			0x1033 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo inv�lido.\nEl nodo no existe. Esto ha ocurrido ejecutando el �tem "%0:s" del nodo "%1:s".\nDebe comprobar que el nodo existe.\n"
#define M4_ERR_XML_CHANNEL_ITEM_INVALID_NODE	0x1034 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "�tem inv�lido.\nEl �tem "%0:s|%1:s.%2:s" es inv�lido.\nDebe asegurarse que el �tem existe.\n"
#define M4_ERR_XML_ITEM_INVALID					0x1035 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl objeto nodo no ha sido inicializado correctamente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_INIT					0x1036 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando valor.\nEl valor del �tem "%0:s|%1:s.%2:s" no ha podido ser aplicado.\nDebe comprobar que el �tem existe y que el valor es aplicable.\n"
#define M4_ERR_XML_ITEM_SET_VALUE				0x1037 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando descripci�n.\nSe ha producido un error aplicando la descripci�n de una referencia a fichero en el �tem "%0:s|%1:s.%2:s".\nDebe comprobar que el �tem existe y que la descripci�n es v�lida.\n"
#define M4_ERR_XML_ITEM_SET_DESCR				0x1038 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando valor nulo.\nError de aplicaci�n de valor nulo al �tem "%0:s|%1:s.%2:s".\nDebe comprobar que el �tem existe.\n"
#define M4_ERR_XML_ITEM_SET_NULL				0x1039 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando valor nulo.\nError de aplicaci�n de valor nulo al �tem "%0:s|%1:s.%2:s".\nDebe comprobar que el �tem existe.\n"
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
// "Error aplicando filtro.\nNo se ha podido aplicar el filtro tipo LN4 "%0:s" a todas las vistas del M4Object "%1:s".\nDebe comprobar que el m�todo es correcto y que el M4Object existe.\n"
#define M4_ERR_XML_ITEM_APPLY_LN4				0x1040 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo el valor de un �tem.\nEsto ocurri� en el �tem del nodo "%0:s" del M4Object "%1:s".\nDebe comprobar que el �tem existe.\n"
#define M4_ERR_XML_ITEM_GET_VALUE				0x1041 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo el valor de un �tem.\nEl �tem "%0:s!%1:s.%2:s" no ha podido ser le�do.\nDebe comprobar que el �tem existe.\n"
#define M4_ERR_XML_ITEM_GET_VALUE_GET			0x1042 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serializaci�n.\nNo se ha podido serializar las referencias a los M4Objects.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CURRENT_SERIAL				0x1043 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nNo se ha podido deserializar las referencias a los M4Objects.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CURRENT_DESERIAL				0x1044 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nNo se ha podido acceder al nodo "%0:s".\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CURRENT_DESERIAL_NODE		0x1045 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nError inicializando objeto de volcado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_INIT				0x1046 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de nodo.\nNo existe un alias definido para el volcado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_NO_ALIAS			0x1047 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de nodo.\nEl alias "%0:s" no existe.\nDebe comprobar que el M4Object asociado a este alias existe.\n"
#define M4_ERR_XML_FILTER_LIST_BAD_ALIAS		0x1048 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de nodo.\nEl nodo "%0:s" no es v�lido.\nDebe comprobar que el nodo existe.\n"
#define M4_ERR_XML_FILTER_LIST_BAD_NODE			0x1049 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de registro.\nEl registro es inv�lido.\nDebe seleccionar correctamente el registro para volcado.\n"
#define M4_ERR_XML_FILTER_LIST_BAD_REGISTER		0x104a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de �tem.\nSe ha producido volcando el �tem "%0:s" del alias "%1:s".\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_DUMP_ITEM		0x104b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de volcado de �tem.\nLa descripci�n del �tem "%0:s" del alias "%1:s" no ha podido ser traducida.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_DESCR			0x104c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de traducci�n.\nError invocando m�todo de traducci�n.\nDebe comprobar que la especificaci�n de la traducci�n es correcta.\n"
#define M4_ERR_XML_FILTER_LIST_DUMP_TRANSL		0x104d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de conectores.\nEl conector inverso es inv�lido.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_INV_CONNECTOR	0x104e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo padre inv�lido.\nEl nodo padre no es v�lido.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_INVALID_PARENT	0x104f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo padre inv�lido.\nLa especificaci�n del M4Object no es consistente.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_FILTER_LIST_NO_PARENT		0x1050 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Registro inv�lido.\nEl registro "%0:s" no es v�lido.\nDebe posicionarse correctamente.\n"
#define M4_ERR_XML_FILTER_LIST_UPD_INVALID_REG	0x1051 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error procesando lista de volcado.\nNo hay elementos en la lista.\nDebe especificar elementos para el volcado.\n"
#define M4_ERR_XML_FILTER_LIST_NO_LIST			0x1052 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl objeto de salida no ha sido inicializado correctamente.\nDebe comprobar que hay recursos suficientes para ejecutar la operaci�n.\n"
#define M4_ERR_XML_STREAM_INIT					0x1053 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de codificaci�n.\nHa fallado el proceso de traducci�n a Base64.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_STREAM_TRANSL64				0x1054 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de decodificaci�n.\nHa fallado el proceso de traducci�n desde Base64.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_STREAM_DECODE64				0x1055 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error creando M4Object.\nEl M4Object "%0:s" no ha podido ser creado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_CREATE_CHANNEL		0x1056 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error construyendo M4Object.\nEl M4Object "%0:s" no ha podido ser construido.\nDebe comprobar que el objeto existe.\n"
#define M4_ERR_XML_CONTEXT_BUILD_CHANNEL		0x1057 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error invocando m�todo de traducci�n.\nSe produjo ejecutando �tem "%0:s".\nDebe comprobar que la especificaci�n de la traducci�n es correcta.\n"
#define M4_ERR_XML_CONTEXT_EXECUTE_TRANSL		0x1058 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error creando canal.\nNo se ha podido crear el canal de multi validaci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_TRANSL_CHANNEL		0x1059 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error construyendo canal.\nSe ha producido un error creando el canal de multi validaci�n "%0:s".\nDebe comprobar el canal existe en la base de datos.\n"
#define M4_ERR_XML_CONTEXT_TRANSL_BUILD			0x105a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando m�todo.\nSe ha producido un error ejecutando el �tem "%0:s".\nDebe comprobar otros errores.\n"
#define M4_ERR_XML_ITEM_EXECUTE					0x105b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando m�todo de M4Object.\nEl m�todo "%0:s" no ha sido ejecutado con �xito.\nDebe comprobar otros errores.\n"
#define M4_ERR_XML_CHANNEL_ITEM_EXECUTE			0x105c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando m�todo.\nNo se ha podido sacar un valor de la pila de ejecuci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_EXECUTE_POP					0x105d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando cach�s de estado.\nLa cach� de estados no ha podido ser borrada.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_STATE_CLEAN					0x105e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando m�todo.\nHa ocurrido un error en la ejecuci�n del c�digo XML.\nDebe consultar otros errores.\n"
#define M4_ERR_XML_EXECUTE						0x105f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando contexto.\nHa habido un error borrando M4Object's.\nDebe consultar otros errores.\n"
#define M4_ERR_XML_RESET_STATE					0x1060 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error eliminando M4Object.\nEl M4Object "%0:s" no ha podido ser borrado.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CURRENT_DESTROY				0x1061 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo inv�lido.\nError borrando filtro. El nodo "%0:s" no es v�lido.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_REMOVE_FILTER_NO_NODE	0x1062 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo inv�lido.\nError Ejecutando m�todo. El nodo "%0:s" no es v�lido.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_EXECUTE_INVALID_NODE	0x1063 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de conversi�n.\nEl tipo "%0:n" no puede ser convertido a partir de una cadena.\nDebe comprobar que el tipo del �tem destino es v�lido.\n"
#define M4_ERR_XML_CONV_ARG_INVALID_TYPE		0x1064 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de conversi�n.\nEl tipo "%0:n" no es un tipo destino v�lido.\nDebe comprobar que el tipo del �tem destino es v�lido.\n"
#define M4_ERR_XML_CONV_ARG_INVALID				0x1065 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de argumento.\nEl argumento "%0:s" no pudo ser convertido. Ha ocurrido en el �tem "%1:s" del nodo "%2:s".\nDebe comprobar el tipo del �tem destino.\n"
#define M4_ERR_XML_ADD_ARG_INVALID_TYPE			0x1066 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error en la definici�n del alias.\nEl M4Object "%0:s" de tipo "%1:n" no ha sido encontrado.\nDebe comprobar que el objeto existe."
#define M4_ERR_XML_CONTEXT_FIND_CHANNEL			0x1067 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Identificador de M4Object inv�lido.\nDebe introducir un identificador v�lido.\nDebe comprobar el identificador."
#define M4_ERR_XML_FIND_CHANNEL_TRUNC			0x1068 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Referencia a un objeto con m�s de una instancia..\nEl identificador "%0:s" se refiere a un objeto instanciado m�s de una vez.\nDebe corregir la referencia."
#define M4_ERR_XML_FIND_MULTI					0x1069 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Instancia no encontrada.\nNo se ha encontrada una instancia de un M4Object con identificador "%0:s".\nDebe referenciar un objeto instanciado."
#define M4_ERR_XML_FIND_NOT_FOUND				0x106a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Instancia de nivel 2 no encontrada.\nLa instancia "%0:s" no ha sido encontrada.\nDebe utilizar un identificador v�lido."
#define M4_ERR_XML_FIND_L2_NOT_FOUND			0x106b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de b�squeda.\nLa funci�n de b�squeda interna de nivel 2 ha fallado.\nDebe consultar con el administrador."
#define M4_ERR_XML_FIND_L2_ERROR				0x106c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error buscando subsistema.\nEl subsistema "%0:s" no ha sido encontrado.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_GET_SUBSYS				0x106d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nLa cach� de estados no ha sido inicializada correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_GET_STACHE				0x106e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando sesi�n.\nLa sesi�n "%0:n" no ha podido ser borrada correctamente de la cach� de estados.\nDebe consultar con el administrador."
#define M4_ERR_XML_CLEAR_SESSION				0x106f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando M4Object.\nEl M4Object "%0:s" no ha podido ser borrado.\nDebe consultar otros errores."
#define M4_ERR_XML_CONTEXT_BUILD_DESTROY		0x1070 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error borrando M4Object de traducci�n.\nEl M4Object "%0:s" no ha podido ser borrado.\nDebe consultar otros errores."
#define M4_ERR_XML_TRANSL_BUILD_DESTROY			0x1071 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Nodo inv�lido.\nEl nodo "%0:s" no es v�lido. Ocurri� en el M4Object alias "%1:s".\nDebe comprobar que el nombre del nodo y M4Object alias son correctos."
#define M4_ERR_XML_ITEM_INVALID_NODE			0x1072 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error definiendo alias.\nEl nombre del alias es inv�lido.\nDebe comprobar la definici�n del alias."
#define M4_ERR_XML_INVALID_ALIAS_NAME			0x1073 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nSe ha encontrado un tipo CPP no v�lido.\nDebe consultar con el administrador."
#define M4_ERR_XML_INTERNAL_CPPTYPE				0x1074 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de conversi�n.\nNo se ha podido convertir la cadena "%0:s" al tipo CPP "%1:n" en la propiedad "%2:s". Esto ocurri� en el M4Object "%3:s".\nDebe comprobar que los par�metros para la conversi�n."
#define M4_ERR_XML_CPP_INVALID_CONV				0x1075 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error aplicando propiedad.\nNo se ha podido aplicar el valor "%0:s" de tipo CPP "%1:n" en la propiedad "%2:s". Esto ocurri� en el M4Object "%3:s".\nDebe comprobar que los par�metros para la conversi�n."
#define M4_ERR_XML_CPP_INVALID_SET_PROP			0x1076 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando atributo.\nEl atributo "%0:s" no ha podido ser inicializado.\nDebe consultar con el administrador."
#define M4_ERR_XML_SET_PROP_ERROR				0x1077 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Comando de administraci�n inv�lido.\nEl comando de c�digo "%0:n" no es v�lido.\nDebe consultar con el administrador."
#define M4_ERR_XML_INVALID_ADMIN_CMD			0x1078 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nError interno procesando comando "%0:n". Posiblemente falta de memoria.\nDebe consultar con el administrador."
#define M4_ERR_XML_INTERNAL_CMD_ERROR			0x1079 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nLa cach� de estados no ha sido inicializada correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_CACHE_INIT_ERROR				0x107a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl comando "FLUSH" no ha sido ejecutado correctamente.\nDebe consultar otros errores."
#define M4_ERR_XML_COMMAND_FLUSH				0x107b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nError evaluando n�mero de objetos en la cach� de estados.\nDebe consultar otros errores."
#define M4_ERR_XML_COMMAND_GETNOBJS				0x107c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl comando de borrado de estado ha fallado debido a un par�metro inv�lido.\nDebe consultar con el administrador."
#define M4_ERR_XML_COMMAND_REMOVE_PARAM			0x107d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl valor de la sesi�n para el comando de borrado "%0:n" es inv�lido.\nDebe corregir el valor del argumento."
#define M4_ERR_XML_COMMAND_SESSION_PARAM		0x107e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl comando de borrado de la cach� ha fallado.\nDebe consultar otros errores."
#define M4_ERR_XML_COMMAND_REMOVE				0x107f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl alias "%0:s" no es v�lido para traducci�n.\nDebe comprobar la definici�n del alias."
#define M4_ERR_XML_TRANSL_NO_ALIAS				0x1080 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl alias "%0:s" no ha sido inicializado correctamente. La vista no ha porido ser creada.\nDebe consultar con el administrador."
#define M4_ERR_XML_TRANSL_NO_ACCESS				0x1081 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "No se ha podido procesar la propiedad.\nLa propieadad "%0:s" no ha podido ser le�da.\nDebe consultar con el administrador."
#define M4_ERR_XML_TRANSL_NO_CONVERT			0x1082 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error leyendo estado.\nNo se ha podido leer el estado para la sesi�n "%0:s".\nDebe consultar con el administrador."
#define M4_ERR_XML_LOAD_STATE					0x1083 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando estado.\nNo se ha podido inicializar el estado para la sesi�n "%0:s".\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_SESSION					0x1084 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando sesi�n.\nLa subsesi�n "%0:s" no ha podido ser inicializada.\nDebe consultar con el administrador."
#define M4_ERR_XML_SET_SESSION_CHANNEL			0x1085 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error persistiendo estado.\nEl estado para la subsesi�n "%0:s" no ha podido ser persistido.\nDebe consultar otros errores."
#define M4_ERR_XML_PERSIST_STATE				0x1086 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nLa sesi�n no ha sido inicializada correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_SESSION_INTERNAL		0x1087 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de inicializaci�n.\nEl objecto "DataStorage" no ha podido ser inicializado correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_SESSION_DATA_STORE		0x1088 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error inicializando objecto interno.\nEl objeto interno de serializaci�n no ha sido inicializado correctamente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_IOD_ERROR				0x1089 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serializaci�n.\nHa ocurrido un error de serializaci�n en la inicializaci�n de la sesi�n.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_SERIALIZE				0x108a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nHa ocurrido un error de deserializaci�n en la inicializaci�n de la sesi�n.\nDebe consultar con el administrador."
#define M4_ERR_XML_INIT_DESERIAL				0x108b + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de inicializaci�n.\nEl M4Object de sesi�n no ha podido ser aplicado correctamente.\nDebe consultar otros errores."
#define M4_ERR_XML_SET_SESSION_CHANNEL_BASE		0x108c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno de consistencia.\nEl objeto sesi�n aplicado no es auto consistente.\nDebe consultar con el administrador."
#define M4_ERR_XML_INCONSISTENCE_SESSION		0x108d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nEl par�metro "%0:s" no corresponde a ning�n identificador de sesi�n v�lido.\nDebe revisar el valor del par�metro."
#define M4_ERR_XML_COMMAND_SESSION_NOT_FOUND	0x108e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error ejecutando comando.\nSe ejecut� un comando contra un espacio proxy inv�lido.\nDebe consultar con el administrador."
#define M4_ERR_XML_INVALID_STATE				0x108f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error agregando argumento.\nSe ha intentado a�adir un argumento inv�lido a un m�todo. Ha ocurrido en argumento "%3:s" el �tem "%0:s!%1:s.%2:s".\nDebe comprobar que el argumento es v�lido.\n"
#define M4_ERR_XML_ADD_ARG_INVALID_ARG			0x1090 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error agregando argumentos.\nSe han utilizado insuficientes argumentos fijos (%3:d de %4:d necesarios). Ha ocurrido en el �tem "%0:s!%1:s.%2:s".\nDebe utilizar m�s argumentos.\n"
#define M4_ERR_XML_NOT_ENOUGH_FIXED_ARGS		0x1091 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno en la cach� de estados.\nLa cach� de estados se encuentra en un estado inv�lido.\nDebe consultar con un administrador."
#define M4_ERR_XML_CACHE_INTERNAL_ERROR 		0x1092 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serializaci�n.\nNo se ha podido serializar el nivel de persistencia.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_PERS_LEVEL	0x1093 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nNo se ha podido deserializar el nivel de persistencia.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_PERS_LEVEL	0x1094 + M4_SRV_EXE_FOR_CHANNEL_BASE

//Error al encriptar datos.\n \nSe ha producido un error al encriptar los datos en la cach� de XML.\nDebe consultar con el administrador.
#define M4_ERR_XML_NO_ENCRYPT					0x1095 + M4_SRV_EXE_FOR_CHANNEL_BASE
//Error al desencriptar datos.\n \nSe ha producido un error al desencriptar los datos en la cach� de XML.\nDebe consultar con el administrador.
#define M4_ERR_XML_NO_DECRYPT					0x1096 + M4_SRV_EXE_FOR_CHANNEL_BASE

// "Error definiendo alias.\nEl nombre del alias para la referencia es inv�lido.\nDebe comprobar la definici�n de la referencia."
#define M4_ERR_XML_INVALID_ALIAS_REF			0x1097 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error definiendo alias.\nEl nombre del la referencia para nivel 2 es inv�lido.\nDebe comprobar la definici�n de la referencia."
#define M4_ERR_XML_INVALID_ALIAS_L2_REF			0x1098 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error definiendo alias.\nLa instancia del objeto referenciado es inv�lida.\nDebe comprobar la definici�n de la referencia."
#define M4_ERR_XML_INVALID_M4O_INSTANCE			0x1099 + M4_SRV_EXE_FOR_CHANNEL_BASE

// "Error de serializaci�n.\nNo se ha podido serializar el contexto de autenticaci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_AUTH_INFO		0x109a + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nNo se ha podido deserializar el contexto de autenticaci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_AUTH_INFO	0x109b + M4_SRV_EXE_FOR_CHANNEL_BASE

// Autenticaci�n

// "Error de autenticaci�n.\nNo se ha podido obtener la factor�a de objetos.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_OBJ_SERVICE				0x109c + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha podido obtener el gestor de objetos.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_CHANNEL_MANAGER			0x109d + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha podido creear el objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_CREATE_CHANNEL			0x109e + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha podido construir el objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_BUILD_CHANNEL			0x109f + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha podido destruir el objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_BUILD_DESTROY			0x10a0 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha crear un acceso al objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_CREATE_ACCESS			0x10a1 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha obtener el node %0:s del objeto %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_GET_NODE				0x10a2 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha obtener el ejecutor para el objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_GET_EXECUTOR			0x10a3 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha podido a�adir un argumento a un m�todo del objeto %0:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_PUSH_ARG				0x10a4 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha podido ejecutor el m�todo %0:s del objeto %1:s. El valor de retorno ha sido %2:n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_EXECUTE_CALL			0x10a5 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha podido ejecutor el c�digo ln4 del m�todo %0:s del objeto %1:s. El valor de retorno ha sido %2:n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_EXECUTE_LN4				0x10a6 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nNo se ha podido recuperar el valor del item %0:s del objeto del objeto %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_GET_PARAM_VALUE			0x10a7 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de autenticaci�n.\nEl tipo/contenido del item no es v�lido. Eso ocurri� para el item  %0:s del objeto del objeto %1:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_AUTH_GET_INVALID_VALUE		0x10a8 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de serializaci�n.\nError serializando la clave de autenticaci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_AUTH_PASS		0x10a9 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nError deserializando la clave de autenticaci�n.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_AUTH_PASS	0x10aa + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de desencriptaci�n.\nError desencriptando cadena.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DECRYPT						0x10ab + M4_SRV_EXE_FOR_CHANNEL_BASE

// security

// "Error de asignaci�n.\nNo tiene permisos para modificar el elemento %0:s del nodo %1:s del Meta4Object %2:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SEC_ITEM_SET_VALUE			0x10ac + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de ejecuci�n.\nNo tiene permisos para ejecutar el elemento %0:s del nodo %1:s del Meta4Object %2:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SEC_ITEM_EXECUTE				0x10ad + M4_SRV_EXE_FOR_CHANNEL_BASE

// deltas

// "Error de inicializaci�n.\nError inicializando vector de deltas.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_DELTA_INIT					0x10ae + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error interno.\nNo se ha podido asignar el id �nico de registro. Ha ocurrido en el �tem "%0:s!%1:s.%2:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_SET_RECORD_UNIQUE_ID			0x10af + M4_SRV_EXE_FOR_CHANNEL_BASE

// m4xml session encryption

// "Error de serializaci�n.\nNo se ha podido serializar el id �nico del contexto.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_SERIAL_UUID			0x10b0 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de deserializaci�n.\nNo se ha podido deserializar el id �nico del contexto.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_CONTEXT_DESERIAL_UUID		0x10b1 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de encriptaci�n.\nError encriptando cadena.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ENCRYPT						0x10b2 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de desencriptaci�n.\nNo se ha podido desencriptar el elemento %0:s del nodo %1:s del alias %2:s para los grupos funcionales %3:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ITEM_DECRYPT_VALUE			0x10b3 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de desencriptaci�n.\nNo se ha podido desencriptar argumento %0:s del item %1:s del nodo %2:s para los grupos funcionales %3:s.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ADD_ARG_ERROR_DECRYPT		0x10b4 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de generaci�n de UUID.\nNo se ha podido generar el UUID de contexto.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_GENERATE_UUID				0x10b5 + M4_SRV_EXE_FOR_CHANNEL_BASE
// "Error de asignaci�n de UUID.\nNo se ha podido asignar el UUID de contexto.\nDebe consultar con el administrador.\n"
#define M4_ERR_XML_ASSIGN_UUID					0x10b6 + M4_SRV_EXE_FOR_CHANNEL_BASE

// "X\nX\nX"
#define edX				0x1080 + M4_SRV_EXE_FOR_CHANNEL_BASE


#endif
