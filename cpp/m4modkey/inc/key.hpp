//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#ifndef	_M4MODKEY_KEY_HPP_
#define _M4MODKEY_KEY_HPP_

#include "globdec.hpp"

//*************** PRIVADO ************************
//----------------------------------------------
//Clave de encriptación.
//----------------------------------------------

//Clave de encriptacion base y la efectiva. Exportamos la clave.
//Aqui estamos poniendo 88 caracteres hexadecimales -> son 44 bytes.
const m4char_t M4MODKEY_ENCRYPT_BASE[] = "\x1\x4\x5\x6\x9\xA\xF\xC\x4\x2\x7\x5\x7\xB\xB\xD\x2\x7\x6\xC\x1\x4\x3\x8\x9\x3\x5\xD\xF\xE\x2\x6\x3\x7\x8\x9\x2\x4\x7\xF\xA\xB\x9\x4\x3\x2\x1\x7\x8\x3\x8\x9\xC\xC\xA\xB\x1\x5\x8\x3\x1\x4\x9\x0\x2\x3\x8\x5\xF\x2\xC\xB\x6\x3\xD\x2\x1\x6\x8\xA\xB\x4\x7\x3\x5\x8\xC\x5";

//	M4MODKEY_ENCRYPT_BASE + 0:
//	\x1\x4\x5\x6\x9\xA\xF\xC\x4\x2\x7\x5
//	M4MODKEY_ENCRYPT_BASE + 12:
//	\x7\xB\xB\xD\x2\x7\x6\xC\x1\x4\x3\x8
//	M4MODKEY_ENCRYPT_BASE + 24:
//	\x9\x3\x5\xD\xF\xE\x2\x6\x3\x7\x8\x9
//	M4MODKEY_ENCRYPT_BASE + 36 (clave tamaño 27):
//	\x2\x4\x7\xF\xA\xB\x9\x4\x3\x2\x1\x7\x8\x3\x8\x9\xC\xC\xA\xB\x1\x5\x8\x3\x1\x4\x9
//	M4MODKEY_ENCRYPT_BASE + 63:
//	\x0\x2\x3\x8\x5\xF\x2\xC\xB\x6\x3\xD\x2\x1\x6\x8\xA\xB\x4\x7\x3\x5\x8\xC\x5

//Ojito, la clave no puede ser superior a 48 bytes!!!
//Clave de encriptacion de licencia.
const m4char_t * const M4MODKEY_ENCRYPT_KEY = M4MODKEY_ENCRYPT_BASE + 36;

//Clave de encriptación de reglas.
const m4char_t * const M4MODKEY_ENCRYPT_KEY_CODE = M4MODKEY_ENCRYPT_KEY + 17;	//Reservamos 10 caracteres de clave.

//Clave de encriptación de resumen.
const m4char_t * const M4MODKEY_ENCRYPT_KEY_SUMMARY = M4MODKEY_ENCRYPT_KEY + 22;	//Reservamos 5 caracteres de clave.

const m4int8_t		M4MODKEY_ENCRYPT_KEYA		= 3;
const m4int8_t		M4MODKEY_ENCRYPT_MASK_DELTA = 20;

const m4char_t M4MODKEY_ENCRYPT_HEADER[] = "<<encrypted>>";

//----------------------------------------------
//FIN Clave de encriptación.
//----------------------------------------------

#endif


