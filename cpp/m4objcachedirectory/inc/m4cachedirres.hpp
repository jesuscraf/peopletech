#ifndef __M4CACHEDIRRES_HPP
#define __M4CACHEDIRRES_HPP

#include "m4res.hpp" 

#define M4_CH_CACHEDIR_MD_REFRESH_LIST						0x0001 + M4_CH_CACHEDIR_ERROR_BASE
//Error al refrescar la lista de cambios de definición de datos.\n \nDebe consultar con el administrador.
//Antiguo #define M4_CH_CS_MD_REFRESH_LIST								0x003C + M4_CH_CS_ERROR_BASE

#define M4_CH_CACHEDIR_MD_LOAD_UPDATE_LIST					0x0002 + M4_CH_CACHEDIR_ERROR_BASE
//Error al cargar la lista de cambios de definición de datos.\n \nDebe consultar con el administrador.
//Antiguo #define M4_CH_CS_MD_LOAD_UPDATE_LIST							0x003D + M4_CH_CS_ERROR_BASE

#define M4_CH_CACHEDIR_MD_GET_REPOSITORY_KEY				0x0003 + M4_CH_CACHEDIR_ERROR_BASE
//Error al obtener la clave de repositorio.\n \nDebe consultar con el administrador.
//Antiguo #define M4_CH_CS_MD_GET_REPOSITORY_KEY							0x003E + M4_CH_CS_ERROR_BASE

#define M4_CH_CACHEDIR_MD_LAST_METADATA_DATE_BIGGER			0x0004 + M4_CH_CACHEDIR_ERROR_BASE
//La fecha de última actualización de datos de definición es incorrecta.\n \nLa fecha de última actualización de datos de definición es posterior a la del servidor.\nDebe consultar con el Administrador.
//Antiguo #define M4_CH_CS_MD_LAST_METADATA_DATE_BIGGER					0x003F + M4_CH_CS_ERROR_BASE

#define M4_CH_CACHEDIR_NO_MD_CACHE_REFREH					0x0005 + M4_CH_CACHEDIR_ERROR_BASE
//Error al refrescar las caches de definición de datos.\n \nDebe consultar con el Administrador.
//#define M4_CH_CS_NO_MD_CACHE_REFREH								0x003B + M4_CH_CS_ERROR_BASE

#define M4_CH_CACHEDIR_BAD_VER_MAGIC_NUMBER					0x0006 + M4_CH_CACHEDIR_ERROR_BASE
// Formato incorrecto.\n \nEl formato de codificación de la versión de cachés es incorrecto.\nDebe consultar con el administrador.
//Antiguo #define M4_CH_MCR_BAD_VER_MAGIC_NUMBER						0x0064 + M4_CH_MCR_ERROR_BASE


#endif // __M4CACHEDIRRES_HPP
