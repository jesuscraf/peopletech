//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4jnisoapclient
// File:                m4jnisoapclientres.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                12/01/2009
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the error numbers.
//
//
//==============================================================================



#include "m4res.hpp"


#ifndef	__M4JNISOAPCLIENTRES_HPP__
#define	__M4JNISOAPCLIENTRES_HPP__


//"Número de argumentos incorrecto.\n \nError verificando los argumentos de entrada del proceso de generación de clientes SOAP. Se ha llamado a la función "%0:s" con "%1:n" argumentos y debería llamarse con "%2:n" argumentos."
#define M4JSOAPCL_ERROR_BAD_NUMBER_ARGUMENTS			0x0001 + M4JSOAPCLERROR_BASE
//"Tipo de argumento incorrecto.\n \nEl argumento "%0:s" de la funcion "%1:s" es de tipo "%2:s" y debería ser de tipo "%3:s"."
#define M4JSOAPCL_ERROR_BAD_ARGUMENT_TYPE				0x0002 + M4JSOAPCLERROR_BASE
//"Argumento vacio.\n \nEl argumento "%0:s" de la funcion "%1:s" contiene una cadena vacia.\nDebe introducir un valor válido."
#define M4JSOAPCL_ERROR_NULL_ARGUMENT					0x0003 + M4JSOAPCLERROR_BASE
//"No se ha podido generar el cliente SOAP.\n \nSe han producido errores al generar el cliente SOAP para el WSDL "%0:s".\nDebe consultar con el administrador."
#define M4JSOAPCL_ERROR_CANNOT_GENERATE_SOAP_CLIENT		0x0004 + M4JSOAPCLERROR_BASE
//"No se ha podido obtener la versión de la máquina virtual de Java.\n \nSe han producido errores al intentar averiguar la versión de JRE."
#define M4JSOAPCL_ERROR_GET_JRE_VERSION					0x0005 + M4JSOAPCLERROR_BASE
//"No se ha podido obtener la versión de apache axis.\n \nSe han producido errores al intentar averiguar la versión apache axis."
#define M4JSOAPCL_ERROR_GET_AXIS_VERSION				0x0006 + M4JSOAPCLERROR_BASE

#endif



