//## begin module%3912A6BD0233.cm preserve=no
//## end module%3912A6BD0233.cm

//## begin module%3912A6BD0233.cp preserve=no
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
//## end module%3912A6BD0233.cp

//## Module: tfres%3912A6BD0233; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\tfres.hpp

#ifndef __TFRES_HPP
#define __TFRES_HPP 1

//## begin module%3912A6BD0233.additionalIncludes preserve=no
//## end module%3912A6BD0233.additionalIncludes

//## begin module%3912A6BD0233.includes preserve=yes

#include "m4res.hpp"

#define M4_CH_TF_RESULT_TRUNCATED						0x0001 + M4_CH_TF_ERROR_BASE
//El resultado no se mostrará completo.\n \nEl resultado es demasiado grande y no se mostrará completo. Se cargaron los "%0:s" primeros registros. Debe redefinir sus criterios si está realizando una búsqueda.\nEsto sucedió en "%1:s" de "%2:s".
#define M4_CH_TF_READ_ACCESS_FIELD_REFUSE				0x0002 + M4_CH_TF_ERROR_BASE
//Acceso de lectura denegado.\n \nAcceso de lectura a columna "%0:s" de la tabla "%1:s" denegado.\nDebe verificar su perfil.\nEsto sucedió en "%3:s" de "%4:s".
#define M4_CH_TF_CONCURRENCY_USERS						0x0003 + M4_CH_TF_ERROR_BASE
//Registro bloqueado.\n \nNo se puede modificar el registro "%0:s" de "%1:s" porque ha sido modificado ya por otro usuario.\nDebe reintentarlo.\nEsto sucedió en "%3:s" de "%4:s"."
#define M4_CH_TF_BAD_ID_ORGANIZATION_IN_CHANNEL			0x0004 + M4_CH_TF_ERROR_BASE
//No se ha podido conseguir el identificador de la sociedad.\n \nEl Meta4Object "%0:s" tiene mal definida la sociedad.\nEsto sucedió en "%2:s" de "%3:s".

#define M4_CH_TF_DATA_REFERENCED_IN_CHILDREN			0x000B + M4_CH_TF_ERROR_BASE
//Los datos están siendo usados.\n \nEl registro "%0:f" de "%1:s" está siendo utilizado por "%2:s".\nEsto sucedió en "%3:s" de "%4:s".

#define M4_CH_TF_RESULT_TRUNCATED_OF_NUMBER				0x000D + M4_CH_TF_ERROR_BASE
//El resultado no se mostrará completo.\n \nEl resultado es demasiado grande y no se mostrará completo. Se cargaron los "%0:s" primeros registros de un total de "%1:s". Debe redefinir sus criterios si está realizando una búsqueda.\nEsto sucedió en "%2:s" de "%3:s".
#define M4_CH_TF_RESULT_TRUNCATED_OF_LIMIT				0x000E + M4_CH_TF_ERROR_BASE
//El resultado no se mostrará completo.\n \nEl resultado es demasiado grande y no se mostrará completo. Se cargaron los "%0:s" primeros registros de más de "%1:s". Debe redefinir sus criterios si está realizando una búsqueda.\nEsto sucedió en "%2:s" de "%3:s".

//## end module%3912A6BD0233.includes

//## begin module%3912A6BD0233.declarations preserve=no
//## end module%3912A6BD0233.declarations

//## begin module%3912A6BD0233.additionalDeclarations preserve=yes
//## end module%3912A6BD0233.additionalDeclarations


//## begin module%3912A6BD0233.epilog preserve=yes
//## end module%3912A6BD0233.epilog


#endif
