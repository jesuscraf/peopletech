
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4univb.cpp   
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
//    Este módulo contiene la definición de las funciones de VB de unicode
//
//
//==============================================================================


#include "m4unicode.hpp"
#include "m4uniutil.hpp"




//=================================================================================
// Convierte una cadena "multi byte" (stringz) en una cadena "wide char" (unicode) de VB
// Tiene en cuenta la página de códigos activa o el encoding utf8
//
// @param ai_pccString	Cadena "multi byte" (stringz) que se quiere convertir
// @return BSTR			Cadena "wide char" (unicode) de VB, conversión de ai_pccString
//=================================================================================

BSTR	M4CppToVB( const char* ai_pccString )
{

	BSTR	oResult = NULL ;
	UINT	uiPage = 0 ;
	int		iMbLength = 0 ;


	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	if( _IsUnicode() == 1 )
	{
		uiPage = CP_UTF8 ;
	}
	else
	{
		uiPage = GetACP() ;
	}

	// Se calcula la longitud de la cadena origen
	iMbLength = (int) strlen( ai_pccString ) ;

	// Se hace la transformación
	oResult = _MultiByteToWideChar( ai_pccString, uiPage, iMbLength ) ;

	return( oResult ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" binaria (stringz) en una cadena "wide char" (unicode) de VB
// Hace la conversión con la página ansi como se hacía antes por compatibilidad
//
// @param ai_pccString	Cadena "multi byte" binaria (stringz) que se quiere convertir
// @param ai_iMbLength	Entero que indica la longitud de la cadena de entrada
// @return BSTR			Cadena "wide char" (unicode) de VB, conversión de ai_pccString
//=================================================================================

BSTR	M4CppToVBBinary( const char* ai_pccString, size_t ai_iMbLength )
{

	BSTR	oResult = NULL ;
	size_t	i = 0 ;


	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	// Se reserva espacio para el 0 final
	oResult = SysAllocStringLen( NULL, (UINT) ai_iMbLength ) ;

	if( oResult == NULL )
	{
		return( NULL ) ;
	}

	for( i = 0 ; i < ai_iMbLength ; i++ )
	{
		// Se hace la transformación manualmente para no perder información
		oResult[ i ] = (unsigned char)ai_pccString[ i ] ;
	}

	// Se null termina siempre
	oResult[ ai_iMbLength ] = '\0' ;

	return( oResult ) ;
}



//=================================================================================
// Convierte una cadena "wide char" (unicode) de VB en una cadena "multi byte" (stringz)
// Se encarga de comprobar los tamaños y hacer la reserva de memoria pertienente
// Tiene en cuenta la página de códigos activa o el encoding utf8
//
// @param ai_oString	Cadena "wide char" (unicode) de VB que se quiere convertir
// @return char*		Cadena "multi byte" (stringz), conversión de ai_oString
//=================================================================================

char*	M4VBToCpp( const BSTR& ai_oString )
{

	char*	pcResult = NULL ;
	UINT	uiPage = 0 ;
	int		iWcLength = 0 ;


	if( ai_oString == NULL )
	{
		return( NULL ) ;
	}

	if( _IsUnicode() == 1 )
	{
		uiPage = CP_UTF8 ;
	}
	else
	{
		uiPage = GetACP() ;
	}

	// Se calcula la longitud de la cadena origen
	iWcLength = SysStringLen( ai_oString ) ;

	// Se hace la transformación
	pcResult = _WideCharToMultiByte( ai_oString, uiPage, iWcLength ) ;

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena "wide char" binaria (unicode) de VB en una cadena "multi byte" (string)
// Hace la conversión con la página ansi como se hacía antes por compatibilidad
//
// @param ai_oString	Cadena "wide char" binaria (unicode) de VB que se quiere convertir
// @param ai_iWcLength	Entero que indica la longitud de la cadena de entrada
// @return char*		Cadena "multi byte" (string), conversión de ai_oString
//=================================================================================

char*	M4VBToCppBinary( const BSTR& ai_oString, size_t ai_iWcLength )
{

	char*	pcResult = NULL ;
	size_t	i = 0 ;


	if( ai_oString == NULL )
	{
		return( NULL ) ;
	}

	// Se reserva espacio para el 0 final
	pcResult = new char[ ai_iWcLength + 1 ] ;

	if( pcResult == NULL )
	{
		return( NULL ) ;
	}

	for( i = 0 ; i < ai_iWcLength ; i++ )
	{
		// Se hace la transformación manualmente para no perder información
		pcResult[ i ] = (char)ai_oString[ i ] ;
	}

	// Se null termina siempre
	pcResult[ ai_iWcLength ] = '\0' ;

	return( pcResult ) ;
}




