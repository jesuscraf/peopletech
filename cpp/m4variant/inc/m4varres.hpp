#ifndef __VARRES_HPP
#define __VARRES_HPP

#include "m4res.hpp"



#define M4_VARIANT_NO_MEMORY								M4_VARIANT_ERROR_BASE + 0x0000
//Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicación.\nEsto sucedió en la línea "%1:s" del archivo "%2:s".
#define M4_VARIANT_INTERNAL_ERROR							M4_VARIANT_ERROR_BASE + 0x0001
//Error interno.\n \nDebe consultar con el administrador.\nEsto sucedió en la línea "%0:s" del archivo "%1:s".
#define M4_VARIANT_INTERNAL_ERROR_WITH_INFO					M4_VARIANT_ERROR_BASE + 0x0002
//Error interno.\n \nInformación adicional: "%2:s".\nDebe consultar con el administrador.\nEsto sucedió en la línea "%0:s" del archivo "%1:s".
#define M4_VARIANT_NO_BSTR_TO_STRING						M4_VARIANT_ERROR_BASE + 0x0003
//Error al traducir una cadena.\n \nAlguno de los caracteres "unicode" de la cadena introducida no se pueden convertir a "ASCII".\nDebe revisar los datos introducidos.
#define M4_VARIANT_NO_STRING_TO_BSTR						M4_VARIANT_ERROR_BASE + 0x0004
//Error al traducir una cadena.\n \nAlguno de los caracteres "ASCII" de la cadena introducida no se pueden convertir a "unicode".\nDebe revisar los datos introducidos.
#define M4_VARIANT_DATE_OVERFLOW							M4_VARIANT_ERROR_BASE + 0x0005
//Fecha incorrecta.\n \nLa fecha "%0:s" no es una fecha válida o su formato no es correcto y se utilizará "%1:f".\nDebe revisar los datos introducidos.


#endif //__VARRES_HPP
