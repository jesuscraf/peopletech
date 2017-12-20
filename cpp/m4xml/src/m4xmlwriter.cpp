//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4xml
// File:                m4xmldoc.cpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                18-02-2005
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la implementación de las funciones del objeto
//    documento
//
//
//==============================================================================


#include "m4xmlutil.hpp"
#include "m4xml.h"
#include "m4xmlwriter.hpp"




// ============================================================================
// Cabecera de XML
// ============================================================================
#define M4RCX_XML_HEADER							L"<?xml version='1.0' encoding='UTF-16'?>\n"
#define M4RCX_XML_COPYRIGHT							L"<!--\nCopyright 1991-2007 by Meta4, Inc.\nCentro Europa Empresarial - Edf. Roma\nC/ Rozabella, 8\n28230 Las Rozas - Madrid\nSpain\n\nPrivate and Confidential\nThe information contained in this document is the property of Meta4.\nIt is for the exclusive use of designated employees\nand not for distribution without prior written authorization.\n-->\n"




// ============================================================================
// Funciones auxiliares
// ============================================================================

//=================================================================================
//
// Escribe una cadena utf16 escapada a un fichero xml
// Recorre la cadena escapando los caracteres de xml
//
// @param ai_pfFile
//
//		Fichero al que se escribe
//
// @param ai_pccValue
//
//		Cadena que se escribe
//
// @return
//
//		Número de bytes escritos
//
//=================================================================================

static	int	_WriteScaped( FILE* ai_pfFile, const unsigned short* const ai_pcwcValue )
{

	int				iResult =  0 ;
	int				iWrite =  0 ;
	unsigned short	b = '\0' ;
	size_t			i = 0 ;
	size_t			iLength = 0 ;


	iLength = wcslen( (wchar_t *) ai_pcwcValue ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		b = ai_pcwcValue[ i ] ;

		switch( b )
		{
			case L'<':
				iWrite = fwprintf( ai_pfFile, L"&lt;" ) ;
				break;

			case L'>':
				iWrite = fwprintf( ai_pfFile, L"&gt;" ) ;
				break;

			case L'"':
				iWrite = fwprintf( ai_pfFile, L"&quot;" ) ;
				break;

			case L'\'':
				iWrite = fwprintf( ai_pfFile, L"&apos;" ) ;
				break;

			case L'&':
				iWrite = fwprintf( ai_pfFile, L"&amp;" ) ;
				break;

			default:
				iWrite = fwprintf( ai_pfFile, L"%c", b ) ;
				break;
		}

		if( iWrite < 0 )
		{
			return( -1 ) ;
		}

		iResult += iWrite ;
	}

	return( iResult ) ;
}





//=============================================================================
//
// M4XmlWriter
//
//=============================================================================

M4XmlWriter::M4XmlWriter( void )
{
	m_pfFile = NULL ;
}



M4XmlWriter::~M4XmlWriter( void )
{
	_Close() ;
}


int	M4XmlWriter::_Close( void )
{

	int	iResult = 0 ;

	
	if( m_pfFile != NULL )
	{
		iResult = fclose( m_pfFile ) ;
		m_pfFile = NULL ;
	}

	return( iResult ) ;
}



//=================================================================================
// Inicia la escritura de un fichero en formato XML
//
// Pasos
//		Se resetean las variables de salida
//		Se chequea que el fichero no esté ya abierto, y si es así se cierra
//		Se chequea que los parámetros de entrada no sean nulos
//		Se abre el fichero para escribir en binario
//		Se añade la cabecera del fichero XML en utf-16
//
//=================================================================================

STDMETHODIMP	M4XmlWriter::Open( BSTR ai_sXMLFileName, BSTR *ao_psErrorMessage, LONG *ao_plResult )
{

	int	iWrite = 0 ;
	int	iClose = 0 ;

	
	*ao_plResult = 0 ;

	if( *ao_psErrorMessage != NULL )
	{
		SysFreeString( *ao_psErrorMessage ) ;
		*ao_psErrorMessage = NULL ;
	}

	iClose = _Close() ;
	M4RCX_RETURN_ERROR( iClose != 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	M4RCX_RETURN_ERROR( ai_sXMLFileName == NULL, E_INVALIDARG, *ao_plResult, ao_psErrorMessage ) ;


	m_pfFile = _wfopen( ai_sXMLFileName, L"wb" ) ;
	M4RCX_RETURN_ERROR( m_pfFile == NULL, -1, *ao_plResult, ao_psErrorMessage ) ;

	iWrite = fprintf( m_pfFile, "ÿþ" ) ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	iWrite = fwprintf( m_pfFile, M4RCX_XML_HEADER ) ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	iWrite = fwprintf( m_pfFile, M4RCX_XML_COPYRIGHT ) ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	return( S_OK ) ;
}



//=================================================================================
// Finaliza la escritura de un fichero en formato XML
//
// Pasos
//		Se resetean las variables de salida
//		Se chequea que el fichero no esté ya abierto, y si es así se cierra
//
//=================================================================================

STDMETHODIMP M4XmlWriter::Close( BSTR *ao_psErrorMessage, LONG *ao_plResult )
{

	int	iClose = 0 ;

	
	*ao_plResult = 0 ;

	if( *ao_psErrorMessage != NULL )
	{
		SysFreeString( *ao_psErrorMessage ) ;
		*ao_psErrorMessage = NULL ;
	}

	iClose = _Close() ;
	M4RCX_RETURN_ERROR( iClose != 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	return( S_OK ) ;
}



//=================================================================================
// Escribe un elemento en un fichero en formato XML
//
// Pasos
//		Se resetean las variables de salida
//		Se chequea que los parámetros de entrada no sean nulos
//		Se chequea que el fichero esté abierto
//		Imprime el corchete de inicio y el nombre del elemento
//		Se recorren los atributos y se van imprimiendo
//		Si uno viene vacío el nombre se deja de imprimir
//		Si se pide que se cierre se imprime el slash de final
//		Imprime el corchete de fin
//
//=================================================================================

STDMETHODIMP M4XmlWriter::StartElement( BSTR ai_sName, SAFEARRAY **ai_pasAttributes, SHORT ai_bClose, BSTR *ao_psErrorMessage, LONG *ao_plResult )
{

	HRESULT	hr = S_OK ;
	int		i = 0 ;
	int		iStart = 0 ;
	int		iStart2 = 0 ;
	int		iEnd = 0 ;
	int		iWrite = 0 ;
	long	alIndex[ 2 ] ;
	BSTR	sAttribute ;
	BSTR	sValue ;
	

	*ao_plResult = 0 ;

	if( *ao_psErrorMessage != NULL )
	{
		SysFreeString( *ao_psErrorMessage ) ;
		*ao_psErrorMessage = NULL ;
	}


	M4RCX_RETURN_ERROR( ai_sName == NULL, E_INVALIDARG, *ao_plResult, ao_psErrorMessage ) ;


	M4RCX_RETURN_ERROR( m_pfFile == NULL, ERROR_INVALID_HANDLE_STATE, *ao_plResult, ao_psErrorMessage ) ;

	iWrite = fwprintf( m_pfFile, L"<" ) ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	iWrite = fwprintf( m_pfFile, ai_sName ) ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	if( ai_pasAttributes != NULL && *ai_pasAttributes != NULL )
	{
		M4RCX_RETURN_ERROR( (*ai_pasAttributes)->cDims != 2, E_INVALIDARG, *ao_plResult, ao_psErrorMessage ) ;

		iStart = (*ai_pasAttributes)->rgsabound[ 1 ].lLbound ;
		iEnd = iStart + (*ai_pasAttributes)->rgsabound[ 1 ].cElements ;

		iStart2 = (*ai_pasAttributes)->rgsabound[ 0 ].lLbound ;

		for( i = iStart ; i < iEnd ; i++ )
		{
			alIndex[ 0 ] = i ;

			alIndex[ 1 ] = iStart2 ;
			hr = SafeArrayGetElement( *ai_pasAttributes, alIndex, &sAttribute ) ;
			M4RCX_RETURN_ERROR( hr != S_OK, hr, *ao_plResult, ao_psErrorMessage ) ;

			if( *sAttribute == 0 )
			{
				break ;
			}

			alIndex[ 1 ] = iStart2 + 1 ;
			hr = SafeArrayGetElement( *ai_pasAttributes, alIndex, &sValue ) ;
			M4RCX_RETURN_ERROR( hr != S_OK, hr, *ao_plResult, ao_psErrorMessage ) ;

			iWrite = fwprintf( m_pfFile, L" " ) ;
			M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

			iWrite = fwprintf( m_pfFile, sAttribute ) ;
			M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

			iWrite = fwprintf( m_pfFile, L"=\'" ) ;
			M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

			iWrite = _WriteScaped( m_pfFile, (unsigned short *) sValue ) ;
			M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

			iWrite = fwprintf( m_pfFile, L"\'" ) ;
			M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;
		}
	}

	if( ai_bClose == TRUE )
	{
		iWrite = fwprintf( m_pfFile, L"/" ) ;
		M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;
	}

	iWrite = fwprintf( m_pfFile, L">\n" ) ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	return( S_OK ) ;
}



//=================================================================================
// Escribe el final de un elemento en un fichero en formato XML
//
// Pasos
//		Se resetean las variables de salida
//		Se chequea que los parámetros de entrada no sean nulos
//		Se chequea que el fichero esté abierto
//		Imprime el tag final de elemento
//
//=================================================================================

STDMETHODIMP M4XmlWriter::EndElement( BSTR ai_sName, BSTR *ao_psErrorMessage, LONG *ao_plResult )
{

	int	iWrite = 0 ;


	*ao_plResult = 0 ;

	if( *ao_psErrorMessage != NULL )
	{
		SysFreeString( *ao_psErrorMessage ) ;
		*ao_psErrorMessage = NULL ;
	}


	M4RCX_RETURN_ERROR( ai_sName == NULL, E_INVALIDARG, *ao_plResult, ao_psErrorMessage ) ;


	M4RCX_RETURN_ERROR( m_pfFile == NULL, ERROR_INVALID_HANDLE_STATE, *ao_plResult, ao_psErrorMessage ) ;

	iWrite = fwprintf( m_pfFile, L"</") ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	iWrite = fwprintf( m_pfFile, ai_sName) ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	iWrite = fwprintf( m_pfFile, L">\n" ) ;
	M4RCX_RETURN_ERROR( iWrite < 0, -1, *ao_plResult, ao_psErrorMessage ) ;

	return S_OK;
}
