
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4unireport.cpp   
// Project:             Meta4 PeopleNet    
// Author:              Meta Software M.S. , S.A
// Date:                13-11-2006
// Language:            C++
// Operating System:    ALL
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la definición de las funciones de reports de unicode
//
//
//==============================================================================


#include "m4unicode.hpp"
#include "m4uniutil.hpp"



//=================================================================================
// Convierte una cadena "wide char" (unicode) a "multi byte" (stringz)
// Usa la función interna que lo hace
//
// @param ai_pcwcString	Cadena de la cadena de utf16 que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pcwcString
//=================================================================================

char*	M4ReportsToCpp( const M4XMLCh* ai_pcwcString, int& aio_riLength )
{

	char*		pcResult = NULL ;
	M4XMLCh*	pwcTemp = NULL ;


	if( aio_riLength == -1 )
	{
		// Se debe determinar la longitud hasta el doble \0 (null terminate wide char)
		pcResult = _Utf16ToCpp( ai_pcwcString, aio_riLength ) ;
	}
	else
	{
		// Se crea una cadena temporal con la logitud indicada
		pwcTemp = new M4XMLCh[ aio_riLength + 1 ] ;

		memcpy ( pwcTemp, ai_pcwcString, aio_riLength * sizeof( M4XMLCh ) ) ;
		pwcTemp[ aio_riLength ] = 0 ;
		aio_riLength = -1 ; // Se ajusta a -1 para que nos devuelva el tamaño de la cadena resultado
		pcResult = _Utf16ToCpp ( pwcTemp, aio_riLength ) ;
		delete( pwcTemp ) ;
	}

	return( pcResult ) ;
}


//=================================================================================
// Convierte una cadena "multi byte" (stringz) a "wide char" (unicode) de C++ 
// Usa la función interna que lo hace
//
// @param ai_pccString		Cadena "multi byte" (stringz) de C++ que se quiere convertir
// @param aio_riLength		Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return M4XMLCh*	Cadena "wide char" (unicode), conversión de ai_pccString
//=================================================================================

M4XMLCh*	M4CppToReports( const char* ai_pccString, int& aio_riLength )
{
	return( _CppToUtf16( ai_pccString, aio_riLength ) ) ;
}


