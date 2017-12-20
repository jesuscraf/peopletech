//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           dm_def.hpp
// File:             dm_def.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:16/06/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef __DM_DEF__HPP__
#define __DM_DEF__HPP__

//----------------------------------------------------------------------------
// Common Options
//----------------------------------------------------------------------------

// Marcas de operacion de nodo y canal

#define M4DM_OPERATION_NONE             0
#define M4DM_OPERATION_DELETE           1
#define M4DM_OPERATION_INSERT           2
#define M4DM_OPERATION_UPDATE           4
#define M4DM_OPERATION_DELETE_BY_DDBB   8
#define M4DM_OPERATION_INSERT_BY_DDBB   16
#define M4DM_OPERATION_UPDATE_BY_DDBB   32

// NONRE_BY_DDBB es equivalente a preguntar si ***OPERATION < 8***

// Marcas de operacion realizadas en un bloque por la ldb.

#define M4DM_LDB_OPERATION_DELETE   1
#define M4DM_LDB_OPERATION_INSERT   2
#define M4DM_LDB_OPERATION_UPDATE   4

// Operaciones poara los filtros de operacion.

#define M4DM_OP_TYPE_DELETE         1
#define M4DM_OP_TYPE_MODIFY         2
#define M4DM_OP_TYPE_NEW            3
#define M4DM_OP_TYPE_UPDATE         4
#define M4DM_OP_TYPE_NOTDELETE      5
#define M4DM_OP_TYPE_NOTMODIFY      6
#define M4DM_OP_TYPE_NOTNEW         7
#define M4DM_OP_TYPE_NOTUPDATE      8

// Para distinguir las operaciones para el mensaje de error.

#define M4_CH_DM_ITEM_GENERIC_OPERATION     0
#define M4_CH_DM_ITEM_WRITE_OPERATION       1
#define M4_CH_DM_ITEM_READ_OPERATION        2


#endif //__DM_DEF__HPP__ 
