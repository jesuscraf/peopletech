//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              channel
// File:                wores.hpp   
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


#ifndef __WORES_HPP__
#define __WORES_HPP__

#include "m4res.hpp"

//==============================================================================
//  Errores del wo
//==============================================================================

#define M4_CH_WO_INTERFACE_UNSUPPORTED			0x0001 + M4_CH_WO_ERROR_BASE
// No se soporta el interfaz OLE.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_CREATE_OBJECT				0x0002 + M4_CH_WO_ERROR_BASE
// No se puede crear el objeto OLE.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_CREATE_CHANNEL			0x0003 + M4_CH_WO_ERROR_BASE
// No se puede crear el Meta4Object.\n \nDebe consultar con el administrador.
#define M4_CH_WO_ZERO_LENGTH_STRING				0x0004 + M4_CH_WO_ERROR_BASE
// Cadena vacía.\n \nNo se puede operar con una cadena de longitud cero.\nDebe consultar con el administrador.
#define M4_CH_WO_CANT_CONVERT_STRING			0x0005 + M4_CH_WO_ERROR_BASE
// Cadena incorrecta.\n \nPosiblemente ha introducido una cadena vacía.\nDebe consultar con el administrador.
#define M4_CH_WO_CANT_CREATE_STRING				0x0006 + M4_CH_WO_ERROR_BASE
// Cadena incorrecta.\n \nPosiblemente ha introducido una cadena vacía.\nDebe consultar con el administrador.
#define M4_CH_WO_CANT_CONVERT_BSTR_TO_STRING	0x0007 + M4_CH_WO_ERROR_BASE
// Cadena incorrecta.\n \nNo se puede convertir una cadena de Visual Basic a C++.\nDebe consultar con el administrador.
#define M4_CH_WO_CANT_CONVERT_STRING_TO_BSTR	0x0008 + M4_CH_WO_ERROR_BASE
// Cadena incorrecta.\n \nNo se puede convertir una cadena de C++ a Visual Basic.\nDebe consultar con el administrador.
#define M4_CH_WO_CANT_INITIALISE_BLOCK			0x0009 + M4_CH_WO_ERROR_BASE
// No se puede acceder al conjunto de filas.\n \nPosiblemente el nodo no existe.\nDebe consultar con el administrador.
#define M4_CH_WO_CANT_COPY_VARIANT				0x000A + M4_CH_WO_ERROR_BASE
// No se puede asignar el variant de Visual Basic.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_GET_PROPERTY				0x000B + M4_CH_WO_ERROR_BASE
// No se puede leer el atributo.\n \nPosiblemente ese atributo no existe.\nDebe consultar con el administrador.
#define M4_CH_WO_UNABLE_TO_GET_NODE				0x000C + M4_CH_WO_ERROR_BASE
// No se puede acceder al conjunto de filas.\n \nPosiblemente el nodo no existe.\nDebe consultar con el administrador.
#define M4_CH_WO_INVALID_DATE					0x000D + M4_CH_WO_ERROR_BASE
// La fecha no es válida.\n \nEl variant de Visual Basic es cero o no es de tipo fecha.\nDebe consultar con el administrador.
#define M4_CH_WO_INVALID_GET_SLICE				0x000E + M4_CH_WO_ERROR_BASE
// No se puede leer el tramo.\n \nSe está intentado leer un tramo ni por posición ni por fecha.\nDebe consultar con el administrador.
#define M4_CH_WO_INVALID_GET_ITEM				0x000F + M4_CH_WO_ERROR_BASE
// No se puede leer el elemento.\n \nSe está intentado leer un elemento ni por posición ni por nombre.\nDebe consultar con el administrador.
#define M4_CH_WO_NO_SUCH_ITEM					0x0010 + M4_CH_WO_ERROR_BASE
// No se puede acceder al elemento o a la fila.\n \nSe está intentado acceder a un elemento o a una fila que no existe.\nDebe consultar con el administrador.
#define M4_CH_WO_NOT_SUPPORTED_FUNCTION			0x0011 + M4_CH_WO_ERROR_BASE
// Función no soportada.\n \nLa función "%0:s" de la librería "%1:s" no está soportada.\nDebe consultar con el administrador.
#define M4_CH_WO_CANNOT_CLONE_ITEM				0x0012 + M4_CH_WO_ERROR_BASE
// No se puede crear la copia del elemento.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANNOT_COPY_ITEM				0x0013 + M4_CH_WO_ERROR_BASE
// No se puede crear la copia del elemento.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANNOT_CLONE_REGISTER			0x0014 + M4_CH_WO_ERROR_BASE
// No se puede crear la copia de la fila.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANNOT_COPY_REGISTER			0x0015 + M4_CH_WO_ERROR_BASE
// No se puede crear la copia de la fila.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANNOT_CLONE_SLICE				0x0016 + M4_CH_WO_ERROR_BASE
// No se puede crear la copia del tramo.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANNOT_COPY_SLICE				0x0017 + M4_CH_WO_ERROR_BASE
// No se puede crear la copia del tramo.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_CREATE_ACCESS				0x0018 + M4_CH_WO_ERROR_BASE
// No se puede crear el acceso sobre el Meta4Object.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_SERIALIZE_CHANNEL			0x0019 + M4_CH_WO_ERROR_BASE
// No se puede serializar el Meta4Object.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_OPEN_FILE					0x001A + M4_CH_WO_ERROR_BASE
// No se puede abrir el fichero.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_DESERIALIZE_CHANNEL		0x001B + M4_CH_WO_ERROR_BASE
// No se puede deserializar el Meta4Object.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_SERIALIZE_ACCESS			0x001C + M4_CH_WO_ERROR_BASE
// No se puede serializar el acceso del Meta4Object.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_DESERIALIZE_ACCESS		0x001D + M4_CH_WO_ERROR_BASE
// No se puede deserializar el acceso del Meta4Object.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_GET_PRES_BUFFER			0x001E + M4_CH_WO_ERROR_BASE
// Recursos insuficientes.\n \nNo hay memoria suficiente para crear una presentación.\nDebe consultar con el administrador.
#define M4_CH_WO_COM_METRICS					0x001F + M4_CH_WO_ERROR_BASE
// Error de traza.\n \nNo se puede abrir el fichero de traza.\nDebe consultar con el administrador.
#define M4_CH_WO_CANT_GET_PRES_FAC				0x0020 + M4_CH_WO_ERROR_BASE
// No existe la factoría de presentaciones.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_GET_RSM					0x0021 + M4_CH_WO_ERROR_BASE
// No existe el perfil.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CANT_CREATE_SAFE_ARRAY			0x0022 + M4_CH_WO_ERROR_BASE
// Recursos insuficientes.\n \nNo hay memoria suficiente para crear una presentación.\nDebe consultar con el administrador.
#define M4_CH_WO_NO_SESSION_CHANNEL				0x0023 + M4_CH_WO_ERROR_BASE
// No se puede obtener el Meta4Object de la sesión.\n \nDebe consultar con el administrador.
#define M4_CH_WO_NO_SUCH_BP						0x0024 + M4_CH_WO_ERROR_BASE
// No existe el punto de ruptura pedido.\n \nDebe consultar con el administrador.
#define M4_CH_WO_NO_SUCH_INFO					0x0025 + M4_CH_WO_ERROR_BASE
// No existe la entrada pedida en la pila de llamadas.\n \nDebe consultar con el administrador.
#define M4_CH_WO_CVM_NOT_STARTED				0x0026 + M4_CH_WO_ERROR_BASE
// Meta4Object Engine no iniciado.\n \nDebe consultar con el administrador.
#define M4_CH_WO_LOGON_PARAM_NOT_FOUND			0x0027 + M4_CH_WO_ERROR_BASE
// Parámetro de Logon no encontrado.\n \nEl parámetro de Logon "%0:s" no existe.\nDebe consultar con el administrador.
#define M4_CH_WO_NULL_ARGUMENT					0x0028 + M4_CH_WO_ERROR_BASE
// Argumento nulo.\n \nNo se puede ejecutar la función "%0:s" porque se está pasando el argumento "%1:s" vacío.\nDebe consultar con el administrador.
#define M4_CH_WO_OPEN_REG_KEY					0x0029 + M4_CH_WO_ERROR_BASE
// Error instalando paquete de caché.\n \nSe ha producido el error "%0:d" abriendo la clave del registro "%1:s".\nDebe consultar con el administrador.
#define M4_CH_WO_ACCESS_4_DEL_DISK_CACHE		0x002A + M4_CH_WO_ERROR_BASE
// Error instalando paquete de caché.\n \nSe ha producido el error "%0:d" accediendo a la cache en disco "%1:s" para su borrado.\nDebe consultar con el administrador.
#define M4_CH_WO_DELETING_DISK_CACHE			0x002B + M4_CH_WO_ERROR_BASE
// Error instalando paquete de caché.\n \nSe ha producido el error "%0:d" eliminando la cache en disco "%1:s".\nDebe consultar con el administrador.
#define M4_CH_WO_DEL_REG_KEY					0x002C + M4_CH_WO_ERROR_BASE
// Error instalando paquete de caché.\n \nSe ha producido el error "%0:d" eliminando la clave del registro "%1:s".\nDebe consultar con el administrador.
#define M4_CH_WO_CREATE_REG_KEY					0x002D + M4_CH_WO_ERROR_BASE
// Error instalando paquete de caché.\n \nSe ha producido el error "%0:d" creando la clave del registro "%1:s".\nDebe consultar con el administrador.
#endif	// __WORES_HPP__
