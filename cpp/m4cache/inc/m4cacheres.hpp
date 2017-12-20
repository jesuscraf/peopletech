#ifndef __M4CACHERES_HPP
#define __M4CACHERES_HPP
//## begin module.includes preserve=yes

#include "m4res.hpp"

#define M4_CACHE_PERSIST								0x0001	+ M4_SRV_CACHE_ERROR_BASE
// No se pudo guardar en disco la caché.\n \nTodavía no se ha iniciado la caché.\nDebe consultar con el administrador.

#define M4_CACHE_DEPERSIST								0x0002	+ M4_SRV_CACHE_ERROR_BASE
// No se pudo cargar de disco la caché.\n \nTodavía no se ha iniciado la caché.\nDebe consultar con el administrador.

#define M4_CACHE_DELETE									0x0003	+ M4_SRV_CACHE_ERROR_BASE
// No se pudo eliminar de disco la caché.\n \nTodavía no se ha iniciado la caché.\nDebe consultar con el administrador.

#define M4_CACHE_OVER_SIZE_LIMIT						0x0004	+ M4_SRV_CACHE_ERROR_BASE
// La caché sobrepasa su límite en memoria.\n \nDebe consultar con el administrador.

#define M4_CACHE_DESTROY_REFER_OBJECT					0x0005	+ M4_SRV_CACHE_ERROR_BASE
// No se pudo eliminar el Meta4Object de la caché.\n \nEl Meta4Object "%0:s" está referenciado y no se puede destruir de la caché.\nDebe consultar con el administrador.

#define M4_CACHE_NOT_INITIALIZED						0x0006  + M4_SRV_CACHE_ERROR_BASE
// Intentando acceder a una caché no inicializada.\n \nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_CACHE_NOT_INITIALIZED						0x0028 + M4_CH_MCR_ERROR_BASE

#define M4_CACHE_SAVEVALUETODISK_EMPTY					0x0007  + M4_SRV_CACHE_ERROR_BASE
// No se pudo guardar en disco el objeto "%0:s" de la caché.\n \nEl objeto está vacío.\nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_CACHE_SAVEVALUETODISK_EMPTY				0x004F + M4_CH_MCR_ERROR_BASE

#define M4_CACHE_SAVEVALUETODISK_PERSIST				0x0008  + M4_SRV_CACHE_ERROR_BASE
// No se pudo guardar en disco el objeto "%0:s" de la caché.\n \nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_CACHE_SAVEVALUETODISK_PERSIST				0x0050 + M4_CH_MCR_ERROR_BASE

#define M4_CACHE_LOADVALUEFROMDISK_NOTEMPTY				0x0009 + M4_SRV_CACHE_ERROR_BASE
// No se pudo descargar desde el disco el objeto "%0:s" de la caché.\n \nEl objeto no está vacío.\nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_CACHE_LOADVALUEFROMDISK_NOTEMPTY			0x0051 + M4_CH_MCR_ERROR_BASE

#define M4_CACHE_LOADVALUEFROMDISK_DEPERSIST			0x000A + M4_SRV_CACHE_ERROR_BASE
// No se pudo cargar de disco el objeto "%0:s" de la caché.\n \nDebe consultar con el administrador.
//Amtiguo M4_CH_MCR_CACHE_LOADVALUEFROMDISK_DEPERSIST			0x0052 + M4_CH_MCR_ERROR_BASE

#define M4_CACHE_DELETEVALUEFROMDISK					0x000B + M4_SRV_CACHE_ERROR_BASE
// No se pudo eliminar de disco el objeto "%0:s" de la caché.\n \nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_CACHE_DELETEVALUEFROMDISK					0x0053 + M4_CH_MCR_ERROR_BASE

#define M4_CACHE_DELETEVALUEFROMRAM						0x000C + M4_SRV_CACHE_ERROR_BASE
// No se pudo eliminar de memoria el objeto "%0:s" de la caché.\n \nEl objeto está vacío.\nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_CACHE_DELETEVALUEFROMRAM					0x0054 + M4_CH_MCR_ERROR_BASE

#define M4_CACHE_VALUE_ACCESS_TO_CACHE					0x000D + M4_SRV_CACHE_ERROR_BASE
// El objeto "%0:s" no tiene acceso a la caché en la que estaba almacenado.\nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_CACHE_VALUE_ACCESS_TO_CACHE				0x0055 + M4_CH_MCR_ERROR_BASE

#define M4_CACHE_BAD_CO_TYPE							0x000E + M4_SRV_CACHE_ERROR_BASE
// Tipo de objeto almacenable en la caché no válido.\n \nEl tipo "%0:s" no es válido para un objeto almacenable en la caché.\nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_BAD_CO_TYPE								0x0022 + M4_CH_MCR_ERROR_BASE

//## end module.includes preserve=yes
#endif // __SERRES_HPP
