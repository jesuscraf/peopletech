
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4unicode.cpp   
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
//    Este módulo contiene la definición de las funciones de unicode
//
//
//==============================================================================


#include "m4unicode.hpp"
#include "m4uniutil.hpp"





//=================================================================================
// Indica si la plataforma es unicode o no
//
// @return int			Entero 1 si estamos en entorno unicode, 0 sino
//=================================================================================

M4_DECL_M4UNICODE	int	M4IsUnicode( void )
{
	return( _IsUnicode() ) ;
}



//=================================================================================
// Establece si la plataforma es unicode o no
//
// @param ai_iUnicode	Entero 1 si estamos en entorno unicode, 0 sino
//=================================================================================

M4_DECL_M4UNICODE	void	M4SetUnicode( int ai_iUnicode )
{
	_SetUnicode( ai_iUnicode ) ;
}



//=================================================================================
// Calcula el número de caracteres de una cadena de bytes
// Tiene en cuenta si es utf8 o ansi
//
// @param ai_pccString	Cadena "multi byte" (stringz) que se quiere medir
// @param ai_iLength	Número de bytes de la cadena que se quiere medir
// @return size_t		Número de caracteres de ai_pccString
//=================================================================================

size_t	M4StrLen( const char* ai_pccString, int ai_iLength )
{

	size_t	iResult = 0 ;


	if( ai_pccString == NULL )
	{
		return( 0 ) ;
	}

	if( ai_iLength == -1 )
	{
		ai_iLength = (int) strlen( ai_pccString ) ;
	}

	if( _IsUnicode() == 1 )
	{
		iResult = _Utf8Chars( ai_pccString, ai_iLength ) ;
	}
	else
	{
		iResult = ai_iLength ;
	}

	return( iResult ) ;
}



//=================================================================================
// Calcula la posición de un caracter en una cadena de caracteres
// Tiene en cuenta si es utf8 o ansi
//
// @param ai_pccString		Cadena "multi byte" (stringz) donde se va a buscar
// @param ai_iLength		Número de bytes de la cadena donde se va a buscar
// @param ai_iCharNumber	Número del caracter que se quiere buscar
// @param ao_riLength		Longitud de salida del caracter que se quiere buscar
// @return size_t			Posición del caracter ai_iPosition en ai_pccString o -1
//								si no existe el caracter
//=================================================================================
int	M4CharPosition( const char* ai_pccString, int ai_iLength, size_t ai_iCharNumber, size_t& ao_riSize )
{

	int	iResult = -1 ;

	
	ao_riSize = 1 ;

	if( ai_pccString == NULL )
	{
		return( -1 ) ;
	}

	if( ai_iLength == -1 )
	{
		ai_iLength = (int) strlen( ai_pccString ) ;
	}

	if( _IsUnicode() == 1 )
	{
		iResult = _Utf8Position( ai_pccString, ai_iLength, ai_iCharNumber, ao_riSize ) ;
	}
	else
	{
		if( ai_iCharNumber <= ai_iLength )
		{
			iResult = (int) ai_iCharNumber ;
		}
	}

	return( iResult ) ;
}



//=================================================================================
// Comprueba si una cadena ANSI o Utf8 tiene caracteres no ASCII
//
// @param ai_pccString	Cadena "multi byte" (stringz) de C++ que se quiere chequear
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere chequear
// @return int			1 si la cadena es ASCII 0 si no lo es
//=================================================================================
int	M4IsAscii( const char* ai_pccString, int ai_iLength )
{

	int	i = 0 ;

	
	if( ai_pccString == NULL )
	{
		return( 1 ) ;
	}

	if( ai_iLength == -1 )
	{
		ai_iLength = (int) strlen( ai_pccString ) ;
	}

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		if( M4UNI_IS_ASCII( ai_pccString[ i ] ) == 0 )
		{
			return( 0 ) ;
		}
	}

	return( 1 ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" (stringz) a una cadena multibyte ANSI
// Tiene en cuenta si ya es ANSI o no
// En unicode se pasa a utf16 y después a ANSI
// En ANSI ya es ANSI
//
// @param ai_pccString	Cadena "multi byte" (stringz) de C++ que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//							y tamaño de salida de la cadena convertida
// @return char*		Cadena multibyte ANSI, conversión de ai_pccString
//=================================================================================

char*	M4CppToANSI( const char* ai_pccString, int& aio_riLength )
{

	char*		pcResult = NULL ;
	M4XMLCh*	pwcString = NULL ;


	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) strlen( ai_pccString ) ;
	}

	if( _IsUnicode() == 1 )
	{
		pwcString = _Utf8ToUtf16( ai_pccString, aio_riLength ) ;
		pcResult = _Utf16ToANSI( pwcString, aio_riLength ) ;
		delete [] pwcString ;
	}
	else
	{
		pcResult = new char[ aio_riLength + 1 ] ;

		if( pcResult == NULL )
		{
			return( NULL ) ;
		}

		memcpy( pcResult, ai_pccString, aio_riLength ) ;
		pcResult[ aio_riLength ] = '\0' ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena multibyte ANSI a una cadena "multi byte" (stringz)
// Tiene en cuenta si ya va a ser ANSI
// En unicode se pasa a utf16 y después a utf8
// En ANSI ya es ANSI
//
// @param ai_pccString	Cadena multibyte ANSI de C++ que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//							y tamaño de salida de la cadena convertida
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pccString
//=================================================================================

char*	M4ANSIToCpp( const char* ai_pccString, int& aio_riLength )
{

	char*		pcResult = NULL ;
	M4XMLCh*	pwcString = NULL ;


	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) strlen( ai_pccString ) ;
	}

	if( _IsUnicode() == 1 )
	{
		pwcString = _ANSIToUtf16( ai_pccString, aio_riLength ) ;
		pcResult = _Utf16ToUtf8( pwcString, aio_riLength ) ;
		delete [] pwcString ;
	}
	else
	{
		pcResult = new char[ aio_riLength + 1 ] ;

		if( pcResult == NULL )
		{
			return( NULL ) ;
		}

		memcpy( pcResult, ai_pccString, aio_riLength ) ;
		pcResult[ aio_riLength ] = '\0' ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" (stringz) a una cadena multibyte utf8
// Tiene en cuenta si ya es utf8 o no
// En unicode ya es utf8
// En ANSI se pasa a utf16 y después a utf8
//
// @param ai_pccString	Cadena "wide byte" (stringz) de C++ que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//							y tamaño de salida de la cadena convertida
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pccString
//=================================================================================

char*	M4CppToUtf8( const char* ai_pccString, int& aio_riLength )
{

	char*		pcResult = NULL ;
	M4XMLCh*	pwcString = NULL ;


	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) strlen( ai_pccString ) ;
	}

	if( _IsUnicode() == 1 )
	{
		pcResult = new char[ aio_riLength + 1 ] ;

		if( pcResult == NULL )
		{
			return( NULL ) ;
		}

		memcpy( pcResult, ai_pccString, aio_riLength ) ;
		pcResult[ aio_riLength ] = '\0' ;
	}
	else
	{
		pwcString = _ANSIToUtf16( ai_pccString, aio_riLength ) ;
		pcResult = _Utf16ToUtf8( pwcString, aio_riLength ) ;
		delete [] pwcString ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" (utf8) a una cadena multibyte stringz
// Tiene en cuenta si va a ser utf8 o no
// En unicode ya va a ser utf8
// En ANSI se pasa a utf16 y después a C++
//
// @param ai_pccString	Cadena "wide byte" (stringz) de C++ que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//							y tamaño de salida de la cadena convertida
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pccString
//=================================================================================

char*	M4Utf8ToCpp( const char* ai_pccString, int& aio_riLength )
{

	char*		pcResult = NULL ;
	M4XMLCh*	pwcString = NULL ;


	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) strlen( ai_pccString ) ;
	}

	if( _IsUnicode() == 1 )
	{
		pcResult = new char[ aio_riLength + 1 ] ;

		if( pcResult == NULL )
		{
			return( NULL ) ;
		}

		memcpy( pcResult, ai_pccString, aio_riLength ) ;
		pcResult[ aio_riLength ] = '\0' ;
	}
	else
	{
		pwcString = _Utf8ToUtf16( ai_pccString, aio_riLength ) ;
		pcResult = _Utf16ToANSI( pwcString, aio_riLength ) ;
		delete [] pwcString ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena "wide char" (unicode) a su formato inverso
//
// @param ai_pcwcString	Cadena de la cadena de utf16 que se quiere convertir
// @param ai_iLength	Tamaño de entrada de la cadena que se quiere convertir
// @return void
//=================================================================================

void	M4SwapUtf16( const M4XMLCh* ai_pcwcString, int ai_iLength )
{
	
	int		i = 0 ;
	char	c = 0 ;
	char	*p = NULL ;


	for( i = 0 ; i < ai_iLength ; i++ )
	{
		p = (char*) &ai_pcwcString[ i ] ;
		c = *p ;
		*p = *( p + 1 ) ;
		*( p + 1 ) = c ;
	}
}



//=================================================================================
// Convierte una cadena "wide char" (unicode) a "multi byte" (stringz)
// Usa la función interna que lo hace
//
// @param ai_pcwcString	Cadena de la cadena de utf16 que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pcwcString
//=================================================================================

char*	M4Utf16ToCpp( const M4XMLCh* ai_pcwcString, int& aio_riLength )
{
	return( _Utf16ToCpp( ai_pcwcString, aio_riLength ) ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" (stringz) a "wide char" (unicode)
// Usa la función interna que lo hace
//
// @param ai_pccString		Cadena "multi byte" (stringz) utf16 que se quiere convertir
// @param aio_riLength		Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return M4XMLCh*	Cadena "wide char" (unicode), conversión de ai_pccString
//=================================================================================

M4XMLCh*	M4CppToUtf16( const char* ai_pccString, int& aio_riLength )
{
	return( _CppToUtf16( ai_pccString, aio_riLength ) ) ;
}


