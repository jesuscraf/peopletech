
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4imdterror.hpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                23-04-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene el interfaz del dispositivo de salida de errores de la
//    librería m4mdtransfer
//
//
//==============================================================================


#include "m4mdtransfer_dll.hpp"

#include "m4types.hpp"




#ifndef __M4IMDTERROR_HPP__
#define __M4IMDTERROR_HPP__




//=================================================================================
//
// IMDTErrorDevice
//
// Interfaz de dispositivo de salida de errores
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER	IMDTErrorDevice
{

public:


	//=================================================================================
	//
	// Vuelca un mensaje al dispositivo de salida
	//
	// @param ai_iCode
	//
	//			Número del mensaje
	//
	// @param ai_iSeverity
	//
	//			Severidad del mensaje
	//
	// @param ai_pccParameters
	//
	//			Parámetros del mensaje
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters ) = 0 ;


} ;

#endif


