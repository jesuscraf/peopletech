
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4unidb.cpp   
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
//    Este módulo contiene la definición de las funciones de base de datos de unicode
//
//
//==============================================================================


#include "m4unicode.hpp"
#include "m4uniutil.hpp"

#include "xercesc/util/XMLUTF8Transcoder.hpp"
XERCES_CPP_NAMESPACE_USE





//=================================================================================
// Convierte una cadena "wide char" (unicode) o "multi byte" (stringz) de DB
// a "multi byte" (stringz)
// Tiene en cuenta si es utf16 o no y si el destino es utf8 o ansi
//
// @param ai_pcvcString	Cadena de la cadena de DB que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pcvcString
//=================================================================================

char*	M4DBToCpp( const void* ai_pcvcString, int& aio_riLength )
{

	char*	pcResult = NULL ;


	if( ai_pcvcString == NULL )
	{
		aio_riLength = 0 ;
		return( NULL ) ;
	}

	if( _IsUnicode() == 1 )
	{
		if( aio_riLength == -1 )
		{
			aio_riLength = (int) XMLString::stringLen( (XMLCh*) ai_pcvcString ) ;
		}

		pcResult = _Utf16ToUtf8( (M4XMLCh*) ai_pcvcString, aio_riLength ) ;
	}
	else
	{
		if( aio_riLength == -1 )
		{
			aio_riLength = (int) strlen( (const char*) ai_pcvcString ) ;
		}

		pcResult = new char[ aio_riLength + 1 ] ;

		if( pcResult == NULL )
		{
			return( NULL ) ;
		}

		memcpy( pcResult, ai_pcvcString, aio_riLength ) ;
		pcResult[ aio_riLength ] = '\0' ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" (stringz) a "wide char" (unicode) o
// "multi byte" (stringz) de DB
// Tiene en cuenta si es utf8 o ansi y si el destino es utf16 o no 
//
// @param ai_pccString		Cadena "multi byte" (stringz) DB que se quiere convertir
// @param aio_riLength		Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return M4XMLCh*	Cadena "wide char" (unicode), conversión de ai_pccString
//=================================================================================

void*	M4CppToDB( const char* ai_pccString, int& aio_riLength )
{

	void*	pvResult = NULL ;


	if( ai_pccString == NULL )
	{
		aio_riLength = 0 ;
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) strlen( ai_pccString ) ;
	}

	if( _IsUnicode() == 1 )
	{
		pvResult = _Utf8ToUtf16( ai_pccString, aio_riLength ) ;
	}
	else
	{
		pvResult = new char[ aio_riLength + 1 ] ;

		if( pvResult == NULL )
		{
			return( NULL ) ;
		}

		memcpy( pvResult, ai_pccString, aio_riLength ) ;
		((char*)pvResult)[ aio_riLength ] = '\0' ;
	}

	return( pvResult ) ;
}


