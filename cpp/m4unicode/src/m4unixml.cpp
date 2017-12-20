
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4unixml.cpp   
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
//    Este módulo contiene la definición de las funciones xml de unicode
//
//
//==============================================================================


#include "m4unicode.hpp"
#include "m4uniutil.hpp"





//=================================================================================
// Obtiene el encoding para documentos html y xml
// Tiene en cuenta si es ANSI o utf8
//
// @return const char*	Cadena con el encoding del documento
//=================================================================================

const char*	M4CharSet( void )
{

	const char*	pcResult = NULL ;


	if( _IsUnicode() == 1 )
	{
		pcResult = "UTF-8" ;
	}
	else
	{
		pcResult = "windows-1252" ;
	}

	return( pcResult ) ;
}




//=================================================================================
// Obtiene la cabecera de un documento html
// Tiene en cuenta si es ANSI o utf8
//
// @return const char*	Cadena con la cabecera del documento html
//=================================================================================

const char*	M4HTMLHeader( void )
{

	const char*	pcResult = NULL ;


	if( _IsUnicode() == 1 )
	{
		pcResult = "<META HTTP-EQUIV='Content-Type' content='text/html; charset=UTF-8'>\n" ;
	}
	else
	{
		pcResult = "<META HTTP-EQUIV='Content-Type' content='text/html; charset=windows-1252'>\n" ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Obtiene la cabecera ANSI de un documento xml
//
// @return const char*	Cadena con la cabecera ANSI del documento xml
//=================================================================================

const char*	M4ANSIHeader( void )
{
	return( "<?xml version='1.0' encoding='windows-1252'?>\n" ) ;
}


//=================================================================================
// Obtiene la cabecera ISO de un documento xml
//
// @return const char*	Cadena con la cabecera ISO del documento xml
//=================================================================================

const char*	M4ISOHeader(void)
{
	return("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
}


//=================================================================================
// Obtiene la cabecera utf8 de un documento xml
//
// @return const char*	Cadena con la cabecera utf8 del documento xml
//=================================================================================

const char* M4Utf8Header( void )
{
	return( "<?xml version='1.0' encoding='UTF-8'?>\n" ) ;
}



//=================================================================================
// Obtiene la cabecera de un documento xml
// Tiene en cuenta si es ANSI o utf8
//
// @return const char*	Cadena con la cabecera del documento xml
//=================================================================================

const char* M4XMLHeader( void )
{

	const char*	pcResult = NULL ;


	/* Bug 0124505
	Pasamos a comillas simples para las lineas de comando
	*/
	if( _IsUnicode() == 1 )
	{
		pcResult = M4Utf8Header() ;
	}
	else
	{
		pcResult = M4ANSIHeader() ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Escapa un caracter especial de xml
// Si es especial se devuelve la cadena que lo representa y sino nulo
//
// @param ai_ccChar		Caracter de entrada
// @return char*		Cadena de salida escapada
//=================================================================================

const char*	M4XMLScapeChar( const char ai_ccChar )
{

	char* pcResult = NULL ;


	switch( ai_ccChar )
	{
		case '<':
			pcResult = "&lt;" ;
			break;

		case '>':
			pcResult = "&gt;" ;
			break;

		case '"':
			pcResult = "&quot;" ;
			break;

		case '\'':
			pcResult = "&apos;" ;
			break;

		case '&':
			pcResult =  "&amp;" ;
			break;

		default:
			pcResult = NULL ;
			break;
	}

	return( pcResult ) ;
}



//=================================================================================
// Escapa un caracter especial de html
// Si es especial se devuelve la cadena que lo representa y sino nulo
//
// @param ai_ccChar		Caracter de entrada
// @return char*		Cadena de salida escapada
//=================================================================================

const char*	M4HTMLScapeChar( const char ai_ccChar )
{

	char* pcResult = NULL ;


	switch( ai_ccChar )
	{
		case '<':
			pcResult = "&lt;" ;
			break;

		case '>':
			pcResult = "&gt;" ;
			break;

		case '"':
			pcResult = "&quot;" ;
			break;

		case '&':
			pcResult =  "&amp;" ;
			break;

		default:
			pcResult = NULL ;
			break;
	}

	return( pcResult ) ;
}


//=================================================================================
// Convierte una cadena "wide char" (unicode) de la xerces a "multi byte" (stringz)
// Usa la función interna que lo hace
//
// @param ai_pcwcString	Cadena "wide char" (unicode) de C++ que se quiere convertir
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pcwcString
//=================================================================================

char*	M4XMLToCpp( const M4XMLCh* ai_pcwcString )
{

	char*	pcResult = NULL ;
	int		iLength = -1 ;


	if( ai_pcwcString == NULL )
	{
		return( NULL ) ;
	}

	pcResult = _Utf16ToCpp( ai_pcwcString, iLength ) ;

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" (stringz) a "wide char" (unicode) de la xerces
// Usa la función interna que lo hace
//
// @param ai_pccString		Cadena "multi byte" (stringz) de xerces que se quiere convertir
// @return M4XMLCh*	Cadena "wide char" (unicode), conversión de ai_pccString
//=================================================================================

M4XMLCh*	M4CppToXML( const char* ai_pccString )
{

	M4XMLCh*	pwcResult = NULL ;
	int			iLength = -1 ;


	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	pwcResult = _CppToUtf16( ai_pccString, iLength ) ;

	return( pwcResult ) ;
}




//=================================================================================
// Convierte una cadena "wide char" (unicode) de la xerces a "multi byte" (string)
// Utiliza la función M4XMLToCpp y hace la conversión
// Se emplea para no olvidarse de liberar memoria
//
// @param ai_pcwcString	Cadena "wide char" (unicode) de C++ que se quiere convertir
// @param string&		Cadena de salida "multi byte" (string), conversión de ai_pcwcString
//=================================================================================

void	M4XMLToSTL( const M4XMLCh* ai_pcwcString, string& ao_rsString )
{

	char*	pcResult = NULL ;


	ao_rsString = "" ;

	pcResult = M4XMLToCpp( ai_pcwcString ) ;

	if( pcResult != NULL )
	{
		ao_rsString = pcResult ;
	}

	delete [] pcResult ;

	return ;
}




//=================================================================================
// Convierte una cadena "wide char" (unicode) de la xerces a utf8
// Utiliza la función _Utf16ToUtf8 y hace la conversión
//
// @param ai_pcwcString	Cadena "wide char" (unicode) de C++ que se quiere convertir
// @return char*		Cadena "multi byte" (utf8), conversión de ai_pcwcString
//=================================================================================

char*	M4XMLToUtf8( const M4XMLCh* ai_pcwcString )
{

	char*	pcResult = NULL ;
	int		iLength = -1 ;


	if( ai_pcwcString == NULL )
	{
		return( NULL ) ;
	}

	pcResult = _Utf16ToUtf8( ai_pcwcString, iLength ) ;

	return( pcResult ) ;
}


