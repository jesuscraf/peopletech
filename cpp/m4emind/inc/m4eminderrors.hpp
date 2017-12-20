//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4emind.dll
// File:             m4eminderrors.hpp
// Project:          eMind Single Sign-on
// Author:           Meta Software M.S. , S.A
// Date:             19/12/05
// Language:         C++
// Operating System: WINDOWS, UNIX
// Design Document:
//
//
// Definition:
//
//    This module defines the error codes for m4emind component.
//
//==============================================================================


#ifndef m4eminderrors_h
#define m4eminderrors_h 1

#include "m4res.hpp"

//==============================================================================
//  Errores de eMind
//==============================================================================

// 4470000="M4EMind: Número de argumentos incorrecto en la llamada a la función "%0:s"."
#define  M4_EM_ERR_INVALID_NUM_ARGS							M4_EMIND_ERROR_BASE
// 4470001="M4EMind: El argumento "%0:s" en la llamada a "%1:s" es inválido."
#define  M4_EM_ERR_INVALID_FUNC_ARG							0x0001 + M4_EMIND_ERROR_BASE
// 4470002="M4EMind: Error desencriptando la password PeopleNet."
#define  M4_EM_ERR_DECR_PN_PWD								0x0002 + M4_EMIND_ERROR_BASE
// 4470003="M4EMind: Error desencriptando la password eMind."
#define  M4_EM_ERR_DECR_EM_PWD								0x0003 + M4_EMIND_ERROR_BASE
// 4470004="M4EMind: Resultado autenticación usuario eMind: "%0:s"."
#define  M4_EM_DBG_MATCH_RESULT								0x0004 + M4_EMIND_ERROR_BASE
// 4470004="M4EMind: Autenticación del usuario eMind: "%0:s"."
#define  M4_EM_DBG_AUTH_USER								0x0005 + M4_EMIND_ERROR_BASE
//4470006="Meta4 eMind.\n \nError autenticando al usuario. Compruebe que el usuario y la contraseña son correctos."
#define  M4_EM_ERR_AUTH										0x0006 + M4_EMIND_ERROR_BASE


#endif
