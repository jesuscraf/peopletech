//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              OLEExec.dll
// File:                errors.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                31-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the error codes for the channel- core
//
//
//==============================================================================


#ifndef __ERRORS_HPP__
#define __ERRORS_HPP__

#include "m4res.hpp"

//==============================================================================
//  Errores de OLE
//==============================================================================

#define	M4_OBJ_OLE_INIT_ERROR				0x0001 + M4_OBJ_OLE_BASE
//Error in Initialise (invalid arguments)
#define	M4_OBJ_OLE_NO_CLSID					0x0002 + M4_OBJ_OLE_BASE
//ProgID not found
#define	M4_OBJ_OLE_CREATE_FAILED			0x0003 + M4_OBJ_OLE_BASE
//CoCreateInstance failed
#define	M4_OBJ_OLE_NO_AUTOMATION			0x0004 + M4_OBJ_OLE_BASE
//Object does not support automation
#define	M4_OBJ_OLE_ENTER_FUNCTION			0x0005 + M4_OBJ_OLE_BASE
//Debug information
#define M4_OBJ_OLE_EXIT_FUNCTION				0x0006 + M4_OBJ_OLE_BASE
//Debug information
#define M4_OBJ_OLE_INFORMATION				0x0007 + M4_OBJ_OLE_BASE
//Debug information
#define	M4_OBJ_OLE_INVALID_NAME				0x0008 + M4_OBJ_OLE_BASE
//OLE method/property name not recognized
#define	M4_OBJ_OLE_INVOKE_FAILED			0x0009 + M4_OBJ_OLE_BASE
//Invocación de método/propiedad ha fallado.\n \nEl método OLE "%1:s" no ha podido ser ejecutado. Error de sistema "%0:s": "%2:s".\nDebe revisar la la existencia del método.
#define	M4_OBJ_OLE_INVALID_COLLECTION		0x000A + M4_OBJ_OLE_BASE
//OLE collection name not recognized
#define	M4_OBJ_OLE_INVALID_POINTER			0x000B + M4_OBJ_OLE_BASE
//OLE pointer no longer valid
#define	M4_OBJ_OLE_INVALID_ARGUMENT			0x000C + M4_OBJ_OLE_BASE
//El método "%0:s" no ha podido ser ejecutado. Argumento inexistente o incorrecto en la llamada. El argumento requerido debe ser de tipo cadena.
#define	M4_OBJ_OLE_METHOD_ERROR				0x000D + M4_OBJ_OLE_BASE
//"OleExec. Error ejecutando el método "%0:s": "%1:s""
#define	M4_OBJ_OLE_M4O_NOT_FOUND			0x000E + M4_OBJ_OLE_BASE
//"Error al ejecutar el método "%0:s".\n \nLa instancia de Meta4Object "%1:d" no existe."
#define	M4_OBJ_OLE_NODE_NOT_FOUND			0x000F + M4_OBJ_OLE_BASE
//"Error al ejecutar el método "%0:s".\n \nEl nodo "%1:s" no existe en el Meta4Object "%2:s"."
#define	M4_OBJ_OLE_ERR_SETTING_ITEM			0x0010 + M4_OBJ_OLE_BASE
//"Error al ejecutar el método "%0:s".\n \nSe ha producido un error asignando valor al item "%1:s" del nodo "%2:s"."
#define	M4_OBJ_OLE_CB_OPR_LOCKED			0x0012 + M4_OBJ_OLE_BASE
// "Ejecución de método bloqueada.\n \nSe ha bloqueado la ejecución del método "%0:s" en el contexto del servidor de aplicaciones. Debe invocar el método en el contexto cliente."
#define	M4_OBJ_SRV_COM_APP_LOCKED			0x0013 + M4_OBJ_OLE_BASE
// "Ejecución de aplicación no permitida.\n \nNo está permitida la ejecución de la aplicación "%0:s" en el contexto del servidor. Debe invocar el método en el contexto cliente."
#endif
