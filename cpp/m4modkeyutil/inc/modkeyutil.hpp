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


#ifndef	_M4MODKEYUTIL_MODKEYUTIL_HPP_
#define _M4MODKEYUTIL_MODKEYUTIL_HPP_

#include <stdio.h>
#include "m4types.hpp"

//Vamos a tener dos versiones (versiones de compilacion):
//	1.- Solo puedes ver informacion de la licencia.
//		Definición de _M4MODKEYUTIL_SECURITY_LICENSE_INFO

//	2.- Accesso generación de licencia.
//		Se puede generar la licencia pero no se podría desencriptar.

//	Nota.- Superusuario (Esta tercera macro realmente no la vamos a compilar!).
//		Se podría encriptar/desencriptar licencias y mostrar el buffer desencriptado.
//		Definición de _M4MODKEYUTIL_SECURITY_SUPERUSER


//------------------------------------------------
//Version privada definiremos la version superusuario y recompilaremos.
//********* #define _M4MODKEYUTIL_SECURITY_SUPERUSER
//------------------------------------------------

#endif


