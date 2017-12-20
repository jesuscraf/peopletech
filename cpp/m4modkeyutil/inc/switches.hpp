//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkeyutil
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


#ifndef	_M4MODKEYUTIL_SWITCHES_HPP_
#define _M4MODKEYUTIL_SWITCHES_HPP_

#include "globdec.hpp"

//Switches del programa

//Usage:

//Ejemplo:
// modkeyutil -O -E -i "fichero entrada" -o fichero_salida -v fichero_dump
// modkeyutil -L -E -i "fichero entrada" -o fichero_salida

//Opcion undocumented para poder encriptar/desencriptar.
//modkeyutil -u "usuario" -L -D -i "fichero entrada" -o -w fichero_salida -v "fichero_dump"

enum eSwitchType_t {
	SWITCH_TYPE_UNDEFINED				= 0,
	SWITCH_TYPE_OBJECTS,
	SWITCH_TYPE_LICENSE,
	SWITCH_TYPE_GENERATION,
	SWITCH_TYPE_INFO,
	SWITCH_TYPE_INFO_SUMMARY
};

//Opciones principales.
const m4char_t ARG_OPTION_OBJECTS[] = "-O";
const m4char_t ARG_OPTION_LICENSE[] = "-L";
const m4char_t ARG_OPTION_GENERATION[] = "-G";
const m4char_t ARG_OPTION_INFO[] = "-I";

const m4char_t ARG_OPTION_INFO_SUMMARY[] = "-S";

//Opciones de encriptacion y desencriptacion
const m4char_t ARG_OPTION_ENCRYPTION[] = "-E";
const m4char_t ARG_OPTION_DECRYPTION[] = "-D";

//Opciones de ficheros.
const m4char_t ARG_OPTION_SOURCE_FILE[] = "-i";
const m4char_t ARG_OPTION_DESTINATION_FILE[] = "-o";

const m4char_t ARG_OPTION_REWRITE_DESTINATION_FILE[] = "-w";

//Traza de ejecucion.
const m4char_t ARG_OPTION_VERBOSE[] = "-v";


#endif


