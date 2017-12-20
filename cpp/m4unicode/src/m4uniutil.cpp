
//==============================================================================
//
// (c) Copyright  1991-2006 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4unicode.dll
// File:                m4uniutil.cpp   
// Project:             Meta4 PeopleNet    
// Author:              Meta Software M.S. , S.A
// Date:                03-01-2007
// Language:            C++
// Operating System:    ALL
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la definición de las funciones de utilidad de unicode
//
//
//==============================================================================


#include "m4unicode.hpp"
#include "m4uniutil.hpp"

#include "xercesc/util/XMLUTF8Transcoder.hpp"
XERCES_CPP_NAMESPACE_USE




//=================================================================================
// Clase de inicialización de XML
//=================================================================================

class	ClXMLInitializer
{

public:

		ClXMLInitializer( void )
		{
			try
			{
      			XMLPlatformUtils::Initialize() ;
			}
			catch( const XMLException& )
			{
			}
		}
} ;


//=================================================================================
// Variable estática de inicialización de XML
//=================================================================================
static	ClXMLInitializer	s_oXMLInitializer ;




//=================================================================================
// Variable estática de tipo unicode
//=================================================================================
static	int s_iUnicode = 1 ;


//=================================================================================
// Indica si la plataforma es unicode o no
//
// @return int			Entero 1 si estamos en entorno unicode, 0 sino
//=================================================================================

int	_IsUnicode( void )
{
	return( s_iUnicode ) ;
}



//=================================================================================
// Establece si la plataforma es unicode o no
//
// @param ai_iUniode	Entero 1 si estamos en entorno unicode, 0 sino
//=================================================================================

void	_SetUnicode( int ai_iUnicode )
{
	s_iUnicode = ai_iUnicode ;
}



//=================================================================================
// Calcula el número de caracteres utf16 de una cadena utf8
// Lo hace mirando si un byte es trailing o no, que son los que empiezan por 10
// No comprueba que la cadena sea utf8 válido
//
// De http://tools.ietf.org/html/rfc3629
//
// 3. UTF-8 definition
// 
//    UTF-8 is defined by the Unicode Standard [UNICODE].  Descriptions and
//    formulae can also be found in Annex D of ISO/IEC 10646-1 [ISO.10646]
// 
//    In UTF-8, characters from the U+0000..U+10FFFF range (the UTF-16
//    accessible range) are encoded using sequences of 1 to 4 octets.  The
//    only octet of a "sequence" of one has the higher-order bit set to 0,
//    the remaining 7 bits being used to encode the character number.  In a
//    sequence of n octets, n>1, the initial octet has the n higher-order
//    bits set to 1, followed by a bit set to 0.  The remaining bit(s) of
//    that octet contain bits from the number of the character to be
//    encoded.  The following octet(s) all have the higher-order bit set to
//    1 and the following bit set to 0, leaving 6 bits in each to contain
//    bits from the character to be encoded.
// 
//    The table below summarizes the format of these different octet types.
//    The letter x indicates bits available for encoding bits of the
//    character number.
// 
//    Char. number range  |        UTF-8 octet sequence
//       (hexadecimal)    |              (binary)
//    --------------------+---------------------------------------------
//    0000 0000-0000 007F | 0xxxxxxx
//    0000 0080-0000 07FF | 110xxxxx 10xxxxxx
//    0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
//    0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//
// @param ai_pccString	Cadena "multi byte" (utf8) que se quiere medir
// @param ai_iLength	Tamaño de entrada de la cadena que se quiere medir
// @return size_t		Número de double bytes necesarios para representar ai_pccString en utf16
//=================================================================================

size_t	_Utf16Chars( const char* ai_pccString, int ai_iLength )
{

	size_t			iResult = 0 ;
	unsigned char	b = 0 ;
	int				i = 0 ;
	size_t			iTrailings = 0 ;


	if( ai_pccString == NULL )
	{
		return( 0 ) ;
	}

	if( ai_iLength == -1 )
	{
		ai_iLength = (int) strlen( ai_pccString ) ;
	}

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		b = ai_pccString[ i ] ;

		if( M4UNI_IS_NOT_TRAILING( b ) )
		{
			iResult++ ;

			if( iTrailings > 2 )
			{
				// Si hemos leido más de 2 trailings se necesita un surrogate
				iResult++ ;
			}
			iTrailings = 0 ;
		}
		else
		{
			iTrailings++ ;
		}
	}

	if( iTrailings > 2 )
	{
		// Si hemos leido más de 2 trailings se necesita un surrogate
		iResult++ ;
	}

	return( iResult ) ;
}



//=================================================================================
// Calcula el número de bytes utf8 de una cadena utf16
// Lo hace teniendo en cuenta el rango de los caracteres
// No comprueba que la cadena sea utf16 válido
//
// De http://tools.ietf.org/html/rfc2781
//
//2. UTF-16 definition
//
//   UTF-16 is described in the Unicode Standard, version 3.0 [UNICODE].
//   The definitive reference is Annex Q of ISO/IEC 10646-1 [ISO-10646].
//   The rest of this section summarizes the definition is simple terms.
//
//   In ISO 10646, each character is assigned a number, which Unicode
//   calls the Unicode scalar value. This number is the same as the UCS-4
//   value of the character, and this document will refer to it as the
//   "character value" for brevity. In the UTF-16 encoding, characters are
//   represented using either one or two unsigned 16-bit integers,
//   depending on the character value. Serialization of these integers for
//   transmission as a byte stream is discussed in Section 3.
//
//   The rules for how characters are encoded in UTF-16 are:
//
//   -  Characters with values less than 0x10000 are represented as a
//      single 16-bit integer with a value equal to that of the character
//      number.
//
//   -  Characters with values between 0x10000 and 0x10FFFF are
//      represented by a 16-bit integer with a value between 0xD800 and
//      0xDBFF (within the so-called high-half zone or high surrogate
//      area) followed by a 16-bit integer with a value between 0xDC00 and
//      0xDFFF (within the so-called low-half zone or low surrogate area).
//
//   -  Characters with values greater than 0x10FFFF cannot be encoded in
//      UTF-16.
//
// ...
//
// 2.2 Decoding UTF-16
// 
//    Decoding of a single character from UTF-16 to an ISO 10646 character
//    value proceeds as follows. Let W1 be the next 16-bit integer in the
//    sequence of integers representing the text. Let W2 be the (eventual)
//    next integer following W1.
// 
//    1) If W1 < 0xD800 or W1 > 0xDFFF, the character value U is the value
//       of W1. Terminate.
// 
//    2) Determine if W1 is between 0xD800 and 0xDBFF. If not, the sequence
//       is in error and no valid character can be obtained using W1.
//       Terminate.
// 
//    3) If there is no W2 (that is, the sequence ends with W1), or if W2
//       is not between 0xDC00 and 0xDFFF, the sequence is in error.
//       Terminate.
// 
//    4) Construct a 20-bit unsigned integer U', taking the 10 low-order
//       bits of W1 as its 10 high-order bits and the 10 low-order bits of
//       W2 as its 10 low-order bits.
// 
//    5) Add 0x10000 to U' to obtain the character value U. Terminate.
//
// @param ai_pcwcString	Cadena "wide char" (utf16) que se quiere medir
// @param ai_iLength	Tamaño de entrada de la cadena que se quiere medir
// @return size_t		Número de bytes necesarios para representar ai_pcwcString en utf8
//=================================================================================

size_t	_Utf8Bytes( const M4XMLCh* ai_pcwcString, int ai_iLength )
{

	size_t			iResult = 0 ;
	unsigned int	c = 0 ;
	int				i = 0 ;


	if( ai_pcwcString == NULL )
	{
		return( 0 ) ;
	}

	if( ai_iLength == -1 )
	{
		ai_iLength = (int) XMLString::stringLen( (XMLCh*) ai_pcwcString ) ;
	}

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		c = ai_pcwcString[ i ] ;

        if( c < 0x80 )
		{
			iResult++ ;
		}
		else if( c < 0x800 )
		{
			iResult += 2 ;
		}
		else if( c < 0x10000 )
		{
			iResult += 3 ;

	        if( c >= 0xD800 && c <= 0xDBFF )
			{
				// Si es un surrogate va a ser siempre > 0x10000, con lo que son 4 bytes
				iResult++ ;

				// Y hay que saltar el surrogate
				i++ ;
			}
		}
		else 
		{
			iResult += 4 ;
		}
	}

	return( iResult ) ;
}



//=================================================================================
// Calcula el número de caracteres una cadena utf8
// Lo hace mirando si un byte es trailing o no, que son los que empiezan por 10
// No comprueba que la cadena sea utf8 válido
//
// Ver notas _Utf16Chars
//
// @param ai_pccString	Cadena "multi byte" (utf8) que se quiere medir
// @param ai_iLength	Tamaño de entrada de la cadena que se quiere medir
// @return size_t		Número de caracteres de ai_pccString
//=================================================================================

size_t	_Utf8Chars( const char* ai_pccString, int ai_iLength )
{

	size_t			iResult = 0 ;
	unsigned char	b = 0 ;
	int				i = 0 ;


	if( ai_pccString == NULL )
	{
		return( 0 ) ;
	}

	if( ai_iLength == -1 )
	{
		ai_iLength = (int) strlen( ai_pccString ) ;
	}

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		b = ai_pccString[ i ] ;

		if( M4UNI_IS_NOT_TRAILING( b ) )
		{
			iResult++ ;
		}
	}

	return( iResult ) ;
}



//=================================================================================
// Calcula la posición de un caracter en una cadena de caracteres
// Lo hace mirando si un byte es trailing o no, que son los que empiezan por 10
// No comprueba que la cadena sea utf8 válido
//
// Ver notas _Utf16Chars
//
// @param ai_pccString		Cadena "multi byte" (stringz) donde se va a buscar
// @param ai_iLength		Número de bytes de la cadena donde se va a buscar
// @param ai_iCharNumber	Número del caracter que se quiere buscar
// @param ao_riLength		Longitud de salida del caracter que se quiere buscar
// @return size_t			Posición del caracter ai_iPosition en ai_pccString o -1
//								si no existe el caracter
//=================================================================================

int	_Utf8Position( const char* ai_pccString, int ai_iLength, size_t ai_iCharNumber, size_t& ao_riSize )
{

	int				iResult = -1 ;
	int				iChars = 0 ;
	unsigned char	b = 0 ;
	int				i = 0 ;

	
	ao_riSize = 1 ;

	if( ai_pccString == NULL )
	{
		return( -1 ) ;
	}

	if( ai_iLength == -1 )
	{
		ai_iLength = (int) strlen( ai_pccString ) ;
	}

	// Primero se busca el comienzo del caracter
	for( i = 0 ; i <= ai_iLength ; i++ )
	{
		b = ai_pccString[ i ] ;

		if( i == ai_iLength || M4UNI_IS_NOT_TRAILING( b ) )
		{
			if( iChars == ai_iCharNumber )
			{
				iResult = i ;
				break ;
			}

			iChars++ ;
		}
	}

	// Después se busca el siguiente caracter para calcular la longitud
	for( i = i + 1 ; i <= ai_iLength ; i++ )
	{
		b = ai_pccString[ i ] ;

		if( i == ai_iLength || M4UNI_IS_NOT_TRAILING( b ) )
		{
			ao_riSize = i - iResult ;
			break ;
		}
	}

	return( iResult ) ;
}



//=================================================================================
// Convierte una cadena "wide char" (utf16) de C++ en una cadena "multi byte" (utf8)
// Se emplea un transcoder de utf8 de la xerces
//
// @param ai_pcwcString	Cadena "wide char" (utf16) de C++ que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return char*		Cadena "multi byte" (utf8), conversión de ai_pcwcString
//=================================================================================

char*	_Utf16ToUtf8( const M4XMLCh* ai_pcwcString, int& aio_riLength )
{

	char*			pcResult = NULL ;
	unsigned int	iSrcCount = 0 ;
	size_t			iMaxBytes = 0 ;
	size_t			iCopied = 0;
	XMLSize_t		iCharsEaten = 0;


	static XMLUTF8Transcoder	oTranscoder( NULL, 4096 ) ;

	
	if( ai_pcwcString == NULL )
	{
		aio_riLength = 0 ;
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) XMLString::stringLen( (XMLCh*) ai_pcwcString ) ;
	}

	iSrcCount = aio_riLength ;
	iMaxBytes = _Utf8Bytes( ai_pcwcString, aio_riLength ) ;
	pcResult = new char[ iMaxBytes + 1 ] ;

	if( pcResult == NULL )
	{
		return( NULL ) ;
	}

	try
	{
		iCopied = oTranscoder.transcodeTo( (XMLCh*) ai_pcwcString, iSrcCount, (XMLByte*)pcResult, iMaxBytes, iCharsEaten, XMLTranscoder::UnRep_RepChar ) ;
	}
	catch( const XMLException& )
	{
		iCopied = 0 ;
	}

	aio_riLength = (int) iCopied ;
	pcResult[ aio_riLength ] = '\0' ;

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" (utf8) en una cadena "wide char" (utf16) de C++
// Se emplea un transcoder de utf8 de la xerces
//
// @param ai_pccString		Cadena "multi byte" (utf8) que se quiere convertir
// @param aio_riLength		Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return M4XMLCh*	Cadena "wide char" (utf16) de C++, conversión de ai_pccString
//=================================================================================

M4XMLCh*	_Utf8ToUtf16( const char* ai_pccString, int& aio_riLength )
{

	M4XMLCh*		pwcResult = NULL ;
	unsigned int	iSrcCount = 0 ;
	size_t			iMaxChars = 0 ;
	size_t			iCopied = 0 ;
	unsigned char*	pcCharSizes = NULL ;
	XMLSize_t		iBytesEaten = 0;


	static XMLUTF8Transcoder	oTranscoder( NULL, 4096 ) ;

	
	if( ai_pccString == NULL )
	{
		aio_riLength = 0 ;
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) strlen( ai_pccString ) ;
	}

	iSrcCount = aio_riLength ;
	iMaxChars = _Utf16Chars( ai_pccString, aio_riLength ) ;
	pwcResult = new M4XMLCh[ iMaxChars + 1 ] ;

	if( pwcResult == NULL )
	{
		return( NULL ) ;
	}

	pcCharSizes = new unsigned char[ iMaxChars + 1 ] ;

	if( pcCharSizes == NULL )
	{
		return( NULL ) ;
	}

	try
	{
		iCopied = oTranscoder.transcodeFrom( (const XMLByte*) ai_pccString, iSrcCount, (XMLCh*)pwcResult, iMaxChars, iBytesEaten, pcCharSizes ) ;
	}
	catch( const XMLException& )
	{
		iCopied = 0 ;
	}

	aio_riLength = (int) iCopied ;
	pwcResult[ aio_riLength ] = '\0' ;

	delete [] pcCharSizes ;

	return( pwcResult ) ;
}



//=================================================================================
// Convierte una cadena "wide char" (utf16) de C++ en una cadena ANSI
// Se emplea el transcode de la xerces
//
// @param ai_pcwcString	Cadena "wide char" (utf16) de C++ que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return char*		Cadena ANSI, conversión de ai_pcwcString
//=================================================================================

char*	_Utf16ToANSI( const M4XMLCh* ai_pcwcString, int& aio_riLength )
{

	char*		pcResult = NULL ;
	M4XMLCh*	pwcString = NULL ;


	if( ai_pcwcString == NULL )
	{
		aio_riLength = 0 ;
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) XMLString::stringLen( (XMLCh*) ai_pcwcString ) ;
	}

	try
	{
		if( aio_riLength >= 0 && ai_pcwcString[ aio_riLength ] != '\0' )
		{
			pwcString = new M4XMLCh[ aio_riLength + 1 ] ;

			if( pwcString == NULL )
			{
				aio_riLength = 0 ;
				return( NULL ) ;
			}
			memcpy( pwcString, ai_pcwcString, aio_riLength * sizeof( M4XMLCh ) ) ;
			pwcString[ aio_riLength ] = '\0' ;

			pcResult = XMLString::transcode( (XMLCh*) pwcString ) ;
			delete( pwcString ) ;
		}
		else
		{
			pcResult = XMLString::transcode( (XMLCh*) ai_pcwcString ) ;
		}

		aio_riLength = (int) strlen( pcResult ) ;
	}
	catch( const XMLException& )
	{
		aio_riLength = 0 ;
		pcResult = new char[ 1 ] ;

		if( pcResult == NULL )
		{
			return( NULL ) ;
		}
		pcResult[ 0 ] = '\0' ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena ANSI en una cadena "wide char" (utf16) de C++
// Se emplea el transcode de la xerces
//
// @param ai_pccString		Cadena "multi byte" (utf8) que se quiere convertir
// @param aio_riLength		Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return M4XMLCh*	Cadena "wide char" (utf16) de C++, conversión de ai_pccString
//=================================================================================

M4XMLCh*	_ANSIToUtf16( const char* ai_pccString, int& aio_riLength )
{

	M4XMLCh*	pwcResult = NULL ;
	char*		pcString = NULL ;

	
	if( ai_pccString == NULL )
	{
		aio_riLength = 0 ;
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) strlen( ai_pccString ) ;
	}

	try
	{
		if( aio_riLength >= 0 && ai_pccString[ aio_riLength ] != '\0' )
		{
			pcString = new char[ aio_riLength + 1 ] ;

			if( pcString == NULL )
			{
				aio_riLength = 0 ;
				return( NULL ) ;
			}

			memcpy( pcString, ai_pccString, aio_riLength * sizeof( char ) ) ;
			pcString[ aio_riLength ] = '\0' ;

			pwcResult = (M4XMLCh*) XMLString::transcode( pcString ) ;
			delete( pcString ) ;
		}
		else
		{
			pwcResult = (M4XMLCh*) XMLString::transcode( ai_pccString ) ;
		}

		aio_riLength = (int) XMLString::stringLen( (XMLCh*) pwcResult ) ;
	}
	catch( const XMLException& )
	{
		aio_riLength = 0 ;
		pwcResult = new M4XMLCh[ 1 ] ;

		if( pwcResult == NULL )
		{
			return( NULL ) ;
		}
		pwcResult[ 0 ] = '\0' ;
	}

	return( pwcResult ) ;
}



//=================================================================================
// Convierte una cadena "wide char" (unicode) de C++ a "multi byte" (stringz)
// Tiene en cuenta si es utf8 o ansi
// En ansi traduce con el API string de la xerces
//
// @param ai_pcwcString	Cadena "wide char" (unicode) de C++ que se quiere convertir
// @param aio_riLength	Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return char*		Cadena "multi byte" (stringz), conversión de ai_pcwcString
//=================================================================================

char*	_Utf16ToCpp( const M4XMLCh* ai_pcwcString, int& aio_riLength )
{

	char*	pcResult = NULL ;


	if( ai_pcwcString == NULL )
	{
		aio_riLength = 0 ;
		return( NULL ) ;
	}

	if( aio_riLength == -1 )
	{
		aio_riLength = (int) XMLString::stringLen( (XMLCh*) ai_pcwcString ) ;
	}

	if( _IsUnicode() == 1 )
	{
		pcResult = _Utf16ToUtf8( ai_pcwcString, aio_riLength ) ;
	}
	else
	{
		pcResult = _Utf16ToANSI( ai_pcwcString, aio_riLength ) ;
	}

	return( pcResult ) ;
}



//=================================================================================
// Convierte una cadena "multi byte" (stringz) a "wide char" (unicode) de C++
// Tiene en cuenta si es utf8 o ansi
// En ansi traduce con el API string de la xerces
//
// @param ai_pccString		Cadena "multi byte" (stringz) de C++ que se quiere convertir
// @param aio_riLength		Tamaño de entrada de la cadena que se quiere convertir
//								y tamaño de salida de la cadena convertida
// @return M4XMLCh*	Cadena "wide char" (unicode), conversión de ai_pccString
//=================================================================================

M4XMLCh*	_CppToUtf16( const char* ai_pccString, int& aio_riLength )
{

	M4XMLCh*	pwcResult = NULL ;


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
		pwcResult = _Utf8ToUtf16( ai_pccString, aio_riLength ) ;
	}
	else
	{
		pwcResult = _ANSIToUtf16( ai_pccString, aio_riLength ) ;
	}

	return( pwcResult ) ;
}






#ifdef _WINDOWS

//=================================================================================
// Convierte una cadena "multi byte" (stringz) en una cadena "wide char" (unicode) de VB
// Se encarga de comprobar los tamaños y hacer la reserva de memoria pertienente
// Tiene en cuenta la página de códigos que se le pasa
//
// @param ai_pccString	Cadena "multi byte" (stringz) que se quiere convertir
// @param ai_uiPage		Identificador numérico de la página de códigos
// @param ai_iMbLength	Longitud de la cadena de entrada
// @return BSTR			Cadena "wide char" (unicode) de VB, conversión de ai_pccString
//=================================================================================

BSTR	_MultiByteToWideChar( const char* ai_pccString, UINT	ai_uiPage, int ai_iMbLength )
{

	BSTR	oResult = NULL ;
	int		iWcLength = 0 ;


	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	// Se calcula la longitud de la cadena destino
	iWcLength = MultiByteToWideChar( ai_uiPage, 0, ai_pccString, ai_iMbLength, NULL, 0 ) ;
	
	// Se reserva espacio para el 0 final
	oResult = SysAllocStringLen( NULL, iWcLength ) ;
	
	if( oResult == NULL )
	{
		return( NULL ) ;
	}

	// Se hace la transformación
	iWcLength = MultiByteToWideChar( ai_uiPage, 0, ai_pccString, ai_iMbLength, oResult, iWcLength ) ;

	return( oResult ) ;
}



//=================================================================================
// Convierte una cadena "wide char" (unicode) de VB en una cadena "multi byte" (stringz)
// Se encarga de comprobar los tamaños y hacer la reserva de memoria pertienente
// Tiene en cuenta la página de códigos que se le pasa
//
// @param ai_oString	Cadena "wide char" (unicode) de VB que se quiere convertir
// @param ai_uiPage		Identificador numérico de la página de códigos
// @param ai_iWcLength	Longitud de la cadena de entrada
// @return char*		Cadena "multi byte" (stringz), conversión de ai_oString
//=================================================================================

char*	_WideCharToMultiByte( const BSTR& ai_oString, UINT	ai_uiPage, int ai_iWcLength )
{

	char*	pcResult = NULL ;
	int		iMbLength = 0 ;


	if( ai_oString == NULL )
	{
		return( NULL ) ;
	}

	// Se calcula la longitud de la cadena destino
	iMbLength = WideCharToMultiByte( ai_uiPage, 0, ai_oString, ai_iWcLength, NULL, 0, NULL, NULL ) ;
	
	// Se reserva espacio para el 0 final
	pcResult = new char[ iMbLength + 1 ] ;
	
	if( pcResult == NULL )
	{
		return( NULL ) ;
	}

	// Se hace la transformación
	WideCharToMultiByte( ai_uiPage, 0, ai_oString, ai_iWcLength, pcResult, iMbLength, NULL, NULL ) ;
	
	// Se null termina siempre
	pcResult[ iMbLength ] = '\0' ;

	return( pcResult ) ;
}

#endif
