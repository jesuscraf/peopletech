
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdrt.dll
// File:                m4mdrtre.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Este módulo tiene mensajes de error del metacanal compilado
//
//
//==============================================================================


#include "m4res.hpp"


#ifndef __M4MDRTRE_HPP__
#define __M4MDRTRE_HPP__


#define M4_MDRT_NO_MEMORY									0x0001 + M4_MDRT_ERROR_BASE
// Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicación.\nEsto sucedió en la línea "%1:s" del archivo "%2:s".
#define M4_MDRT_INTERNAL_ERROR								0x0002 + M4_MDRT_ERROR_BASE
// Error interno.\n \nDebe consultar con el administrador.\nEsto sucedió en la línea "%0:s" del archivo "%1:s".
#define M4_MDRT_INTERNAL_ERROR_WITH_INFO					0x0003 + M4_MDRT_ERROR_BASE
// Error interno.\n \nInformación adicional: "%2:s".\nDebe consultar con el administrador.\nEsto sucedió en la línea "%0:s" del archivo "%1:s".
#define M4_MDRT_BAD_MD_MAGIC_NUMBER							0x0004 + M4_MDRT_ERROR_BASE
// Formato incorrecto.\n \nEl formato de codificación de los datos de definición es incorrecto.\nDebe consultar con el administrador.
#define M4_MDRT_BAD_SEC_MAGIC_NUMBER						0x0005 + M4_MDRT_ERROR_BASE
// Formato incorrecto.\n \nEl formato de codificación de la seguridad es incorrecto.\nDebe consultar con el administrador.
#define M4_MDRT_BAD_M4OBJ_VERSION							0x0006 + M4_MDRT_ERROR_BASE
// Versión incorrecta.\n \nLa versión del Meta4Object es "%0:s" y la de la aplicación en curso es "%1:s".\nDebe instalar correctamente la aplicación.
#define M4_MDRT_DATA_TRUNCATED								0x0007 + M4_MDRT_ERROR_BASE
// ID truncado.\n \nEl ID "%0:s" supera su máxima longitud "%1:s" y se usará "%2:s".\nDebe cambiar el ID.
#define M4_MDRT_AMBIGUOUS_TI_CALL_IN_RUN_TIME				0x0008 + M4_MDRT_ERROR_BASE
// Estructura de nodo ambigua.\n \nLa estructura de nodo "%0:s" no está resuelta para uno de los "%1:s" nodos que la utilizan.\nDebe resolver el alias de estructura de nodo.\nEsto sucedió en "%2:s" de "%3:s".
#define M4_MDRT_BAD_CONNECT_LANE							0x0009 + M4_MDRT_ERROR_BASE
// Error al conectar la lista.\n \nNo se puede conectar el documento a la lista "%0:s" de "%1:s".\nDebe definir correctamente la conexión.\nEsto sucedió en "%2:s" de "%3:s".
#define M4_MDRT_NO_SUCH_PLUG_IN_RT							0x000A + M4_MDRT_ERROR_BASE
// No existe el conector de Meta4Objects.\n \nNo existe el conector entre "%0:s" de "%1:s" y "%2:s" de "%3:s".\nDebe definir el conector de Meta4Objects.
#define M4_MDRT_NO_SUCH_NODE_IN_LANE						0x000B + M4_MDRT_ERROR_BASE
// No existe el nodo.\n \nDebe definir el nodo "%0:s" de "%1:s" o eliminar el conector de Meta4Objects.\nEsto sucedió al conectarse a la lista "%2:s" de "%3:s".
#define M4_MDRT_NO_SUCH_ITEM_IN_LANE						0x000C + M4_MDRT_ERROR_BASE
// No existe el elemento.\n \nDebe definir el elemento "%0:s" de "%3:s" de "%4:s" o eliminar el conector de elementos de Meta4Objects.\nEsto sucedió al conectarse al documento "%1:s" de "%2:s".
#define M4_MDRT_BAD_LANE_NODE_IN_RT							0x000D + M4_MDRT_ERROR_BASE
// Lista en nodo hijo.\n \nNo se puede conectar "%0:s" de "%1:s" y "%2:s" de "%3:s" porque el nodo del documento no es un nodo raíz.\nDebe definir el conector de los Meta4Objects sobre un nodo raíz.
#define M4_MDRT_BAD_LANE_M4OBJ_IN_RT						0x000E + M4_MDRT_ERROR_BASE
// Lista en Meta4Object proxy.\n \nNo se puede conectar "%0:s" de "%1:s" y "%2:s" de "%3:s" porque el documento es un Meta4Object proxy.\nDebe cambiar el tipo C/S del Meta4Object documento.
#define M4_MDRT_AUTOGENEATED_CODE							0x000F + M4_MDRT_ERROR_BASE
// Este código ha sido creado de forma automática por la aplicación.
#define M4_MDRT_INTERNAL_INVERSE_ERROR						0x0010 + M4_MDRT_ERROR_BASE
// Error interno.\n \nSe ha producido un error en el proceso de ingeniería inversa de la fórmula "%0:s" de "%1:s" de "%2:s" de "%3:s" en el período de "%4:d" hasta "%5:d".\nDebe consultar con el administrador.\nEsto sucedió en la línea "%6:s" del archivo "%7:s".


#endif
