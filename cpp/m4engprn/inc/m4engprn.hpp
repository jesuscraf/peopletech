//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             M4EngPrn.hpp
// Project:			 M4EngPrn.dll	
// Author:           Meta Software M.S. , S.A
// Date:			 15/07/98
// Language:         C++
// Operating System: WINDOWS , UNIX (HP, DEC , AIX , SUN)
//  
// Propietary:       Manuel Lazcano Perez
// Modifications: 	 Who && Date	
//
// Definition:
//
//    This module defines the exported functions of the library
//
//
//==============================================================================


#include "m4engprn_dll.hpp"
#include "m4types.hpp"


/////////////////////////////////////////////////////////////////////////////////
//    EXPORTED FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////

//Devuelve true si la funcion tiene exito, false si no. Es independiente de plataforma, puesto que
// se implementa segun la arquitectura utilizada al compilar. En el caso de Win32 se envian los 
// datos en bloques de 64K.

M4_DECL_M4ENGPRN m4bool_t WINAPI M4_SendFilePathToPrinter(m4char_t* ai_pcPrinterName, m4char_t* ai_pcDataFile);

//Deshabilitada porque si usa clfile, esta clase deberia de estar en algun sitio comun a 
//	todos componentes.
//Mucho mejor es que reciba un * FILE (puntero para evitar copiar objetos en pila).
//OJITO, que la he quitado del DEF
//DLL_FUN M4_SendFileToPrinter(m4char_t* ai_pcPrinterName, M4ClFile ai_oDataFile); 

