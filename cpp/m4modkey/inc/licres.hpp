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

#ifndef	_M4MODKEY_LICRES_HPP_
#define _M4MODKEY_LICRES_HPP_

#include "m4res.hpp"

//1966081="Licencia.\n \nSe ha sobrepasado el número de personas en fase de alta procesados establecido en la licencia de uso. Por favor, póngase en contacto con el departamento comercial de Meta4."
#define M4_CH_LICENSE_STATE_VALIDATE_EXECUTION_NP					0x0001 + M4_CH_LICENSE_ERROR_BASE

//1966082="Licencia.\n \nEl Meta4Object "%0:s" contiene elementos segurizados y la licencia instalada no permite su uso. Revise su licencia."
#define M4_CH_LICENSE_NOT_ALLOWED_FINISHED							0x0002 + M4_CH_LICENSE_ERROR_BASE

//1966083="Licencia.\n \nLa licencia instalada es "%0:s"."
#define M4_CH_LICENSE_INSTALLED										0x0003 + M4_CH_LICENSE_ERROR_BASE

//1966084="Licencia.\n \nNo es posible validar la licencia instalada."
#define M4_CH_LICENSE_INCORRECT										0x0004 + M4_CH_LICENSE_ERROR_BASE

//1966085="Licencia.\n \nNo es posible validar la licencia instalada. La licencia instalada ya ha expirado."
#define M4_CH_LICENSE_EXPIRED										0x0005 + M4_CH_LICENSE_ERROR_BASE

#endif


