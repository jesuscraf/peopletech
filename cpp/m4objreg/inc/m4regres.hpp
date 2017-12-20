//## begin module%3912A1F60050.cm preserve=no
//## end module%3912A1F60050.cm

//## begin module%3912A1F60050.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%3912A1F60050.cp

//## Module: m4regres%3912A1F60050; Package specification
//## Subsystem: m4objreg::inc%3912A34F03C7
//## Source file: d:\new\m4objreg\inc\m4regres.hpp

#ifndef __M4REGRES_HPP__
#define __M4REGRES_HPP__ 1

//## begin module%3912A1F60050.additionalIncludes preserve=no
//## end module%3912A1F60050.additionalIncludes

//## begin module%3912A1F60050.includes preserve=yes

#include "m4res.hpp"

#define	M4_CH_OBJREG_NO_READ_REGISTRY				0x0001 + M4_OBJREG_ERROR_BASE
// Error leyendo el registro.\n \nDebe consultar con el administrador.
#define	M4_CH_OBJREG_PROPERTY_NOT_FOUND             0x0002 + M4_OBJREG_ERROR_BASE
// Atributo no encontrado.\n \nNo se encontró el atributo "%0:s" en el registro.\nDebe consultar con el administrador.\nEsto sucedió en el camino "%1:s".
#define M4_CH_OBJREG_BAD_DLL_VERSION				0x0003 + M4_OBJREG_ERROR_BASE
// Versión incorrecta.\n \nLa versión registrada es "%0:s" y la de la aplicación en curso es "%1:s".\nDebe instalar correctamente la aplicación.
#define M4_CH_OBJREG_WRONG_PROPERTY_VALUE			0x0004 + M4_OBJREG_ERROR_BASE
// Valor de atributo incorrecto.\n \nEl atributo "%0:s" contiene un valor incorrecto "%2:s".\nSe utiliza el valor por defecto "%1:s".
#define M4_CH_OBJREG_BAD_DIRECTORY					0x0005 + M4_OBJREG_ERROR_BASE
// No se puede crear el directorio.\n \nNo se ha podido crear el directorio "%0:s" por la razón "%1:s".\nDebe comprobar que el directorio padre exista y que tenga permisos de escritura.


//## end module%3912A1F60050.includes

//## begin module%3912A1F60050.declarations preserve=no
//## end module%3912A1F60050.declarations

//## begin module%3912A1F60050.additionalDeclarations preserve=yes
//## end module%3912A1F60050.additionalDeclarations


//## begin module%3912A1F60050.epilog preserve=yes
//## end module%3912A1F60050.epilog


#endif
