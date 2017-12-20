
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                mdfacglb.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo tiene las funciones globales de la factoría
//
//
//==============================================================================


#include "m4stl.hpp"
#include "cltypes.hpp"


#ifndef __MDFACGLB_HPP__
#define __MDFACGLB_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class   ClStaticPoolStr ;


//=============================================================================
// Para asignar identificadores con longitud por delante en array local
//=============================================================================
m4return_t	M4MdFacSetLocalId( m4pcchar_t ai_pccSource, m4pchar_t ao_pcDest, m4uint16_t ai_iMaxLength, m4bool_t ai_bConvert ) ;

//=============================================================================
// Para asignar nombres con longitud por delante en puntero local
//=============================================================================
m4return_t	M4MdFacSetLocalId( m4pcchar_t ai_pccSource, m4pchar_t& ao_rpcDest, m4bool_t ai_bConvert ) ;

//=============================================================================
// Para asignar identificadores con longitud por delante en un pool
//=============================================================================
m4return_t	M4MdFacSetPoolId( m4pcchar_t ai_pccSource, m4uint32_t &ao_riDest, ClStaticPoolStr *ai_poPool ) ;



#endif

