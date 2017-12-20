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
#include "m4xmldoc.hpp"

#include "xercesc/dom/DOMNode.hpp"
#include "xercesc/dom/DOMDocument.hpp"
#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/dom/DOMNamedNodeMap.hpp"
#include "xercesc/framework/LocalFileInputSource.hpp"
#include "xercesc/sax/InputSource.hpp"





//=============================================================================
//
// M4XmlDocument
//
//=============================================================================

M4XmlDocument::M4XmlDocument( void )
{
	m_poDocument = NULL ;
	m_poNode = NULL ;
}



M4XmlDocument::~M4XmlDocument( void )
{
	m_poDocument = NULL ;
	m_poNode = NULL ;
}



//=================================================================================
//
// Inicia la lectura de un fichero en formato XML
//
// Pasos
//		Se resetean las variables de salida
//		Se chequea que los parámetros de entrada no sean nulos
//		Se abre una input source a partir del fichero
//		Se asigna el manejador de errores al parser
//		Se parsea la input source del fichero
//		Se controlan los errores de parseo, tanto si se ha producido una excepción
//			como si vienen por el manejador
//		Se guarda el documento DOM
//
//=================================================================================

STDMETHODIMP	M4XmlDocument::Open( BSTR ai_sXMLFileName, BSTR *ao_psErrorMessage, LONG *ao_plResult )
{

	const XMLCh			*pcwcException = NULL ;
	InputSource			*poSource = NULL ;
	ClRCXErrorHandler	oErrorHandler ;


	*ao_plResult = 0 ;

	if( *ao_psErrorMessage != NULL )
	{
		SysFreeString( *ao_psErrorMessage ) ;
		*ao_psErrorMessage = NULL ;
	}

	M4RCX_RETURN_ERROR( ai_sXMLFileName == NULL, E_INVALIDARG, *ao_plResult, ao_psErrorMessage ) ;


	m_poDocument = NULL ;
	m_poNode = NULL ;

    try
    {
		poSource = new LocalFileInputSource( (XMLCh *) ai_sXMLFileName ) ;
		M4RCX_RETURN_ERROR( poSource == NULL, E_OUTOFMEMORY, *ao_plResult, ao_psErrorMessage ) ;

		// void
		m_oParser.setErrorHandler( &oErrorHandler ) ;
		m_oParser.parse( *poSource ) ;

		*ao_plResult = oErrorHandler.GetError() ;

		if( *ao_plResult == 0 )
		{
			m_poDocument = m_oParser.getDocument() ;
		}
		else
		{
			// UNICODE VB
			pcwcException = oErrorHandler.GetMessage() ;
			_formatError( E_UNEXPECTED, pcwcException, ao_psErrorMessage ) ;
			*ao_plResult = -1 ;
		}
    }
    catch( const XMLException &e )
    {
		// UNICODE VB
		pcwcException = e.getMessage() ;
		_formatError( E_UNEXPECTED, pcwcException, ao_psErrorMessage ) ;
		*ao_plResult = -1 ;
    }

	if( poSource != NULL )
	{
		delete( poSource ) ;
	}


	return( S_OK ) ;
}



//=================================================================================
//
// Lee el siguiente elemento de un fichero en formato XML
//
// Pasos
//		Resetea las variables de salida
//		Se chequea que el documento esté abierto
//		Se obtiene el siguiente elemento  del actual. Para ello:
//			Si es nodo actual es nulo se empieza en el elemento documento. Eso
//				permite que se pueda recorrer el DOM tantas veces como se quiera
//				porque al acabar se pone el elemento actual a nulo
//			Si el nodo actual no es nulo se busca el primer hijo de tipo
//				ELEMENT_NODE
//			Si no se encuentra un hijo se busca el siguiente hermano de tipo
//				ELEMENT_NODE
//			Si no se encuentra un hermano se busca el siguiente hermano de tipo
//				ELEMENT_NODE del padre
//			Si no se encuentra el hermano del padre se busca el siguiente hermano
//				de tipo	ELEMENT_NODE de su padre y así recursivamente
//			Se actualiza el elemento actual con el siguiente, sea o no nulo
//		Si se ha encontrado elemento siguiente se inspecciona y se rellenan las 
//			variables de salida
//			Se actualiza el flag de existencia
//			Se obtiene el nombre del elemento y se rellena la cadena de salida
//			Se obtienen los atributos del elemento
//			Se crea un safearray de 2 dimensiones, la primera del número de
//				atributos y la segunda de 2, uno para el atributo y otro para
//				el valor. Las dimensiones del safearray se empiezan en 1.Para cada
//				atributo se obtiene su nombre y su valor y se insertan en la
//				posición correcta del safearray
//
//=================================================================================

STDMETHODIMP	M4XmlDocument::GetNextElement( SHORT *ao_pbExists, BSTR *ao_psName, SAFEARRAY **ao_pasAttributes, BSTR *ao_psErrorMessage, LONG *ao_plResult )
{

	HRESULT				hr = S_OK ;
	int					bFound = 0 ;
	int					i = 0 ;
	int					iLength = 0 ;
	BSTR				sAttribute ;
	BSTR				sValue ;
	long				alIndex[ 2 ] ;
	SAFEARRAYBOUND		rgsabound[ 2 ] ;

	DOMNode				*poNextNode = NULL ;
	DOMNode				*poParentNode = NULL ;
	DOMNode				*poAttribute = NULL ;
	DOMNamedNodeMap		*poAttributes = NULL ;


	*ao_plResult = 0 ;
	*ao_pbExists = FALSE ;

	if( *ao_psName != NULL )
	{
		SysFreeString( *ao_psName ) ;
		*ao_psName = NULL ;
	}

	if( *ao_psErrorMessage != NULL )
	{
		SysFreeString( *ao_psErrorMessage ) ;
		*ao_psErrorMessage = NULL ;
	}

	if( *ao_pasAttributes != NULL )
	{
		hr = SafeArrayDestroy( *ao_pasAttributes ) ;
		*ao_pasAttributes = NULL ;
		M4RCX_RETURN_ERROR( hr != S_OK, hr, *ao_plResult, ao_psErrorMessage ) ;
	}

	
	M4RCX_RETURN_ERROR( m_poDocument == NULL, OLE_E_BLANK, *ao_plResult, ao_psErrorMessage ) ;


	if( m_poNode == NULL )
	{
		poNextNode = m_poDocument->getDocumentElement() ;
	}
	else
	{
		poNextNode = m_poNode->getFirstChild() ;
		while( poNextNode != NULL && poNextNode->getNodeType() != DOMNode::ELEMENT_NODE )
		{
			poNextNode = poNextNode->getNextSibling() ;
		}

		if( poNextNode == NULL )
		{
			poNextNode = m_poNode->getNextSibling() ;
			while( poNextNode != NULL && poNextNode->getNodeType() != DOMNode::ELEMENT_NODE )
			{
				poNextNode = poNextNode->getNextSibling() ;
			}

			if( poNextNode == NULL )
			{
				poParentNode = m_poNode->getParentNode() ;

				bFound = 0 ;
				while( bFound == 0 && poParentNode != NULL )
				{
					poNextNode = poParentNode->getNextSibling() ;
					while( poNextNode != NULL && poNextNode->getNodeType() != DOMNode::ELEMENT_NODE )
					{
						poNextNode = poNextNode->getNextSibling() ;
					}

					if( poNextNode == NULL )
					{
						poParentNode = poParentNode->getParentNode() ;
					}
					else
					{
						bFound = 1 ;
					}
				}
			}
		}
	}

	m_poNode = poNextNode ;


	if( m_poNode != NULL )
	{
		*ao_pbExists = TRUE ;
		*ao_psName = _DOMStringToBSTR( m_poNode->getNodeName() ) ;

		poAttributes = m_poNode->getAttributes() ;
		iLength = poAttributes->getLength() ;

		rgsabound[ 0 ].lLbound = 1 ;
		rgsabound[ 0 ].cElements = iLength ;
		rgsabound[ 1 ].lLbound = 1 ;
		rgsabound[ 1 ].cElements = 2 ;

		*ao_pasAttributes = SafeArrayCreate( VT_BSTR, 2, rgsabound ) ;
		M4RCX_RETURN_ERROR( *ao_pasAttributes == NULL, E_OUTOFMEMORY, *ao_plResult, ao_psErrorMessage ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poAttribute = poAttributes->item( i ) ;

			alIndex[ 0 ] = i + 1 ;

			alIndex[ 1 ] = 1 ;
			sAttribute = _DOMStringToBSTR( poAttribute->getNodeName() ) ;
			hr = SafeArrayPutElement( *ao_pasAttributes, alIndex, sAttribute ) ;
			SysFreeString( sAttribute ) ;
			M4RCX_RETURN_ERROR( hr != S_OK, hr, *ao_plResult, ao_psErrorMessage ) ;

			alIndex[ 1 ] = 2 ;
			sValue = _DOMStringToBSTR( poAttribute->getNodeValue() ) ;
			hr = SafeArrayPutElement( *ao_pasAttributes, alIndex, sValue ) ;
			SysFreeString( sValue ) ;
			M4RCX_RETURN_ERROR( hr != S_OK, hr, *ao_plResult, ao_psErrorMessage ) ;
		}
	}

	return( S_OK ) ;
}




