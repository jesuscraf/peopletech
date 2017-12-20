#ifndef __M4PRESRES_HPP
#define __M4PRESRES_HPP

#include "m4res.hpp" 

#define M4_CH_PRES_BAD_PRES_MAGIC_NUMBER				0x0001 + M4_CH_PRES_ERROR_BASE
// Formato incorrecto.\n \nEl formato de codificación de la Presentación es incorrecto.\nDebe consultar con el administrador.
//Antiguo #define M4_CH_MCR_BAD_PRES_MAGIC_NUMBER						0x0061 + M4_CH_MCR_ERROR_BASE

#define M4_CH_PRES_BAD_PRES_VERSION						0x0002 + M4_CH_PRES_ERROR_BASE
// Versión incorrecta.\n \nLa versión de la presentación es "%0:s" y la de la aplicación en curso es "%1:s".\nDebe instalar correctamente la aplicación.

#define M4_CH_PRES_CACHE_PUT_PRESENTATION				0x0003 + M4_CH_PRES_ERROR_BASE
// No se pudo almacenar la presentación "%0:s" en la caché.\n \nDebe consultar con el administrador.\nAtributos de la presentación "%0:s": perfil "%1:s".
//Antiguo  #define M4_CH_MCR_CACHE_PUT_PRESENTATION					0x0027 + M4_CH_MCR_ERROR_BASE

#define M4_CH_PRES_BUILD_BASIC_PRESENTATION				0x0004 + M4_CH_PRES_ERROR_BASE
// Error al generar la presentación "%0:s".\n \nNo se pudo generar la presentación básica.\nDebe consultar con el administrador.\nAtributos del la presentación "%0:s": perfil "%1:s".
//Antiguo  #define M4_CH_MCR_BUILD_BASIC_PRESENTATION					0x0023 + M4_CH_MCR_ERROR_BASE

#define M4_CH_PRES_BUILD_PRESENTATION					0x0005 + M4_CH_PRES_ERROR_BASE
// Error al generar la presentación "%0:s".\n \nNo se pudo generar la presentación procesada.\nDebe consultar con el administrador.\nAtributos del la presentación "%0:s": perfil "%1:s".
//Antiguo #define M4_CH_MCR_BUILD_PRESENTATION						0x0024 + M4_CH_MCR_ERROR_BASE

#define M4_CH_PRES_BUILD_INCLUDE						0x0006 + M4_CH_PRES_ERROR_BASE
//2621446="Error al generar la presentación "%0:s".\n \nLa presentación procesada no se pudo generar debido a un problema en el include "%1:s".\nDebe consultar con el administrador."

#define M4_CH_PRES_BUILD_INTERNAL_ERROR					0x0007 + M4_CH_PRES_ERROR_BASE
//2621447="Error interno al generar la presentación.\n \nEsto sucedió en la línea "%1:s" del archivo "%0:s"."

#define M4_CH_PRES_INFINITE_RECURSION					0x0008 + M4_CH_PRES_ERROR_BASE
//2621448="Error al generar la presentación.\n \nLa inclusión de la presentación "%0:s" provoca un bucle infinito de inclusiones. (Bucle de inclusión: "%1:s")."

#define M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR			0x0009 + M4_CH_PRES_ERROR_BASE
//2621449="Error interno en herencia al generar la presentación.\n \nEsto sucedió en la línea "%1:s" del archivo "%0:s"."

#define M4_CH_PRES_INHERIT_MERGE_DIFFERENT_OBJECTS		0x000A + M4_CH_PRES_ERROR_BASE
//2621450="Error en herencia al generar la presentación.\n \nNo es posible fusionar el objeto "%1:s" (de tipo "%0:s") con el objeto "%3:s" (de tipo "%2:s").\nRevise las presentaciones."

#define M4_CH_PRES_MAPPING_NOT_ALLOWED					0x000B + M4_CH_PRES_ERROR_BASE
//2621451="Mapeo no permitido.\n \nNo se puede construir la presentación "%0:s" porque tiene un mapeo "%1:s" para la sociedad "%2:s".\nDebe eliminar el mapeo."

#define M4_CH_PRES_INHERIT_MISSING_POSITIONAFTER		0x000C + M4_CH_PRES_ERROR_BASE
//2621452="Error en herencia al generar la presentación.\n \nEn el objeto con ID '%0:s' existe un posicionamiento respecto a un objeto con ID '%1:s' que no existe en ninguna de las presentaciones de las que hereda."

#endif // __M4PRESRES_HPP
