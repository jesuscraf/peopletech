//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4iputils.dll
// File:             m4iputilserrors.hpp
// Project:          IP Address utils
// Author:           Meta Software M.S. , S.A
// Date:             24/08/16
// Language:         C++
// Operating System: WINDOWS, UNIX
//==============================================================================

#ifndef m4iputilsrrors_h
#define m4iputilsrrors_h 1

#include "m4res.hpp"

//==============================================================================
//  Errores
//==============================================================================

// 1572977="Número de argumentos no válido.\n \nEl número de argumentos para el método "%0:s" no es válido. Debe consultar con el administrador."
#define M4_ERR_BAD_NUM_ARGS						0x0071 + M4_CH_TF_ERROR_BASE
// 1572978="Tipo de argumentos no válido.\n \nEl tipo de datos de los argumentos para el método "%0:s" no es válido. Debe consultar con el administrador."
#define M4_ERR_BAD_ARG_TYPE						0x0071 + M4_CH_TF_ERROR_BASE
// 1572979="Error validando dirección IP.\n \nSe produjo un error validando la dirección IP. Debe consultar con el administrador."
#define M4_ERR_VALIDATING_IP					0x0073 + M4_CH_TF_ERROR_BASE


#endif
