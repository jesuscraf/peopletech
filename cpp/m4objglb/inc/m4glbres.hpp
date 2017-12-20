//## begin module%3912A6BD0166.cm preserve=no
//## end module%3912A6BD0166.cm

//## begin module%3912A6BD0166.cp preserve=no
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
//## end module%3912A6BD0166.cp

//## Module: m4glbres%3912A6BD0166; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\m4glbres.hpp

#ifndef __M4GLBRES_HPP__
#define __M4GLBRES_HPP__ 1

//## begin module%3912A6BD0166.additionalIncludes preserve=no
//## end module%3912A6BD0166.additionalIncludes

//## begin module%3912A6BD0166.includes preserve=yes

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                m4glbres.hpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                09-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Este módulo tiene mensajes de error de la global
//
//
//==============================================================================

#include "m4res.hpp"

//## end module%3912A6BD0166.includes

//## begin module%3912A6BD0166.declarations preserve=no
//## end module%3912A6BD0166.declarations

//## begin module%3912A6BD0166.additionalDeclarations preserve=yes
//## end module%3912A6BD0166.additionalDeclarations


//## begin module%3912A6BD0166.epilog preserve=yes


//M4_OBJGLB_ERROR_BASE = 1769472


#define M4_OBJGLB_NO_MEMORY									0x0001 + M4_OBJGLB_ERROR_BASE
// Recursos insuficientes.\n \nNo hay memoria suficiente para obtener "%0:s" bytes.\nDebe cerrar alguna aplicación.\nEsto sucedió en la línea "%1:s" del archivo "%2:s".
#define M4_OBJGLB_INTERNAL_ERROR							0x0002 + M4_OBJGLB_ERROR_BASE
// Error interno.\n \nDebe consultar con el administrador.\nEsto sucedió en la línea "%0:s" del archivo "%1:s".
#define M4_OBJGLB_INTERNAL_ERROR_WITH_INFO					0x0003 + M4_OBJGLB_ERROR_BASE
// Error interno.\n \nInformación adicional: "%2:s".\nDebe consultar con el administrador.\nEsto sucedió en la línea "%0:s" del archivo "%1:s".
#define M4_OBJGLB_CANT_FIND_METHOD							0x0004 + M4_OBJGLB_ERROR_BASE
// Método externo no encontrado.\n \nNo se puede encontrar el método "%0:s" en librería externa "%1:s".\nDebe asegurarse de que el método exista y sea accesible.
#define M4_OBJGLB_EXTERNAL_METHOD_ERROR						0x0005 + M4_OBJGLB_ERROR_BASE
// Error en ejecución.\n \nEl método externo "%0:s" de la biblioteca "%1:s" se ejecutó con errores.\nDebe consultar el resto de mensajes.
#define M4_OBJGLB_CANT_RESIZE_STACK							0x0006 + M4_OBJGLB_ERROR_BASE
// Ejecución demasiado compleja.\n \nNo se puede seguir ejecutando debido a la excesiva complejidad del código, a demasiadas ejecuciones propagadas o a una recursividad infinita.\nDebe reducir la complejidad de su código.
#define M4_OBJGLB_TRACE_VM_L1								0x0007 + M4_OBJGLB_ERROR_BASE
//Traza.\n \n"%1:s".
#define M4_OBJGLB_CANT_FIND_LIBRARY							0x0008 + M4_OBJGLB_ERROR_BASE
// Librería externa no encontrada.\n \nNo se puede encontrar la librería externa "%0:s".\nDebe asegurarse de que la librería exista y sea accesible.



//## end module%3912A6BD0166.epilog


#endif
