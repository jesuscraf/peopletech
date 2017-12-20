//-- {CycleCode: 120} file [0..1505]
//-- {StartSubRegion: 122} module.ifndef [0..20]
#ifndef __SERRES_HPP
//-- {StartSubRegion: 123} module.define [21..41]
#define __SERRES_HPP
//-- {AddDecl: 121} module.includes preserve=yes [42..1481]
//## begin module.includes preserve=yes

#include "m4res.hpp"


#define M4_SERIAL_OUT_OF_MEMORY								0x0000 + M4_SERIAL_ERROR_BASE
//No hay suficiente memoria.

#define M4_SERIAL_UNKNOWN									0x0001 + M4_SERIAL_ERROR_BASE
//Error interno.

#define M4_SERIAL_CANT_OPEN_FILE							0x0002 + M4_SERIAL_ERROR_BASE
//No se puede abrir el fichero.\n \nEl sistema operativo retorna el error: "%0:s".\nDebe comprobar que el camino al fichero es correcto.\nEsto sucedió en el fichero "%1:s".

#define M4_SERIAL_CANT_WRITE_TO_FILE						0x0003 + M4_SERIAL_ERROR_BASE
//Error de escritura en fichero.\n \nEl sistema operativo retorna el error: "%0:s".\nDebe liberar espacio en la unidad, o cambiar la unidad de destino.\nEsto sucedió en el fichero "%1:s".

#define M4_SERIAL_CANT_READ_FROM_FILE						0x0004 + M4_SERIAL_ERROR_BASE
//Error de lectura en fichero.\n \nEl sistema operativo retorna el error: "%0:s".\nEsto sucedió en el fichero "%1:s".

#define M4_SERIAL_CANT_SEEK_FILE							0x0005 + M4_SERIAL_ERROR_BASE
//Error posicionando en el fichero.\n \nEl sistema operativo retorna el error: "%0:s".\nEsto sucedió en el fichero "%1:s".

#define M4_SERIAL_WRONG_PARAMS								0x0006 + M4_SERIAL_ERROR_BASE
//Parametros incorrectos en llamada.

#define M4_SERIAL_NO_PERSISTIABLE_OBJECT_ID					0x0007 + M4_SERIAL_ERROR_BASE
// No existe ID en objeto persistente.\n \nDebe consultar con el administrador.
//Antiguo M4_CH_MCR_NO_PERSISTIABLE_OBJECT_ID					0x002A + M4_CH_MCR_ERROR_BASE

//## end module.includes preserve=yes
//-- {StartSubRegion: 124} module.endif [1482..1505]
#endif // __SERRES_HPP
