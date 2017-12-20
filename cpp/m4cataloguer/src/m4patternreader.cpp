
//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cataloguer
// File:                m4patternreader.cpp   
// Project:             PeopleNet      
// Author:              Meta Software M.S. , S.A
// Date:                12-02-2016
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements the class for reading catalogue patterns
//
//==============================================================================


#include "m4string.hpp"
#include "m4patternreader.hpp"
#include "m4unicode.hpp"

#include "xercesc/dom/DOM.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/framework/LocalFileInputSource.hpp"
#include "xercesc/sax/ErrorHandler.hpp"
#include "xercesc/sax/SAXParseException.hpp"

XERCES_CPP_NAMESPACE_USE




//=================================================================================
// Clase auxiliares
//=================================================================================

//=================================================================================
//
// ClErrorHandler
//
// Clase que gestiona los errores de parseo de la xerces. Funciona como un callback
// al que se avisa cuando ha habido un error. Para ello debe registrase con el
// parser
// Implementa el interfaz de ErrorHandler
//
//=================================================================================

class	ClErrorHandler : public ErrorHandler
{

public:

//=================================================================================
// Funciones de inicialización
//=================================================================================

		//=================================================================================
		// Constructor
		// Inicializa los miembros a nulo
		//=================================================================================
		ClErrorHandler( void )
		{
			m_iError = 0 ;
			m_pwcMessage = NULL ;
		}

		//=================================================================================
		// Destructor
		//=================================================================================
		virtual ~ClErrorHandler( void )
		{
			resetErrors() ;
		}


//=================================================================================
// Funciones de lectura
//=================================================================================

	//=================================================================================
	// Retorna el error
	//=================================================================================
	m4return_t	GetError( void ) const
	{
		return( m_iError ) ;
	}

	//=================================================================================
	// Retorna el mensaje
	//=================================================================================
	const M4XMLCh*	GetMessage( void ) const
	{
		return( m_pwcMessage ) ;
	}


//=================================================================================
// Funciones del interfaz de ErrorHandler
//=================================================================================

		virtual void  warning( const SAXParseException& exception )
		{
			resetErrors() ;
		}
 
		virtual void  error( const SAXParseException& exception )
		{
			size_t			iLength = 0 ;
			const XMLCh*	pcwcException = NULL;

			resetErrors() ;

			m_iError = -1 ;
			pcwcException = exception.getMessage() ;

			iLength = XMLString::stringLen( pcwcException ) + 1 ;

			m_pwcMessage = new M4XMLCh[ iLength ] ;

			if( m_pwcMessage != NULL )
			{
				memcpy( m_pwcMessage, pcwcException, iLength * sizeof( XMLCh ) ) ;
			}
		}
 
		virtual void  fatalError( const SAXParseException& exception )
		{
			error( exception ) ;
		}
 
		virtual void  resetErrors( void )
		{
			m_iError = 0 ;
			if( m_pwcMessage != NULL )
			{
				delete [] m_pwcMessage ;
				m_pwcMessage = NULL ;
			}
		}


protected:

	//=================================================================================
	// Indicador de error
	//=================================================================================
	m4return_t	m_iError ;

	//=================================================================================
	// Texto del posible error
	//=================================================================================
	M4XMLCh		*m_pwcMessage ;

} ;





//=================================================================================
//
// Reads patterns from pattern file.
//
// Parameters
//		ai_pccDirectory: Directory to get pattern file from
//		ai_rvStartIncludes: List of start include patterns
//		ai_rvEndIncludes: List of end include patterns
//		ai_rvStartExcludes: List of start exclude patterns
//		ai_rvEndExcludes: List of end exclude patterns
//
// Returns
//		M4_ERROR if failure; M4_SUCCESS otherwise
//
//=================================================================================

m4return_t	M4PatternReader::ReadPatterns( m4pcchar_t ai_pccDirectory, vector<m4pchar_t>& ai_rvStartIncludes, vector<m4pchar_t>& ai_rvEndIncludes, vector<m4pchar_t>& ai_rvStartExcludes, vector<m4pchar_t>& ai_rvEndExcludes )
{

	m4return_t			iError = 0 ;
	m4bool_t			bStart = M4_TRUE ;
	m4pchar_t			pcException = NULL ;
	m4pchar_t			pcNodeName = NULL ;
	m4pchar_t			pcAttributeValue = NULL ;
	m4char_t			acPatternFile[ M4_CATALOGUER_MAX_PATH + 1 ] ;
	M4XMLCh				*wsFile = NULL ;
	FILE				*pfFile = NULL ;
	InputSource			*poSource = NULL ;
	DOMNode				*poNode = NULL ;
    DOMNode				*poAttribute = NULL;
	ClErrorHandler		oErrorHandler ;
	XercesDOMParser		oParser ;


	sprintf( acPatternFile, "%s%sm4cataloguepattern.xml", ai_pccDirectory, M4_DIRECTORY_SEPARATOR ) ;

	eUniFileEncodingType_t	eEncoding = M4UniANSI ;
	pfFile = M4Fopen( acPatternFile, M4UniRead, eEncoding ) ;

	if( pfFile == NULL )
	{
		printf( "\nCatalogue pattern <m4cataloguepattern.xml> not found." ) ;
		return( M4_ERROR ) ;
	}

	fclose( pfFile ) ;
	pfFile = NULL ;

	try
	{
		// Si el fichero existe se intenta parsear
		wsFile = M4CppToXML( acPatternFile ) ;

		if( wsFile == NULL )
		{
			printf( "\nNot enough memory to allocate <%d> bytes.", (m4uint32_t)( sizeof( M4XMLCh ) * ( strlen( acPatternFile ) + 1 ) ) ) ;
			return( M4_ERROR ) ;
		}

		poSource = new LocalFileInputSource( (XMLCh*)wsFile ) ;
		delete( wsFile ) ;

		if( poSource == NULL )
		{
			printf( "\nNot enough memory to allocate <%d> bytes.", (m4uint32_t)( sizeof( LocalFileInputSource ) ) ) ;
			return( M4_ERROR ) ;
		}

		oParser.setErrorHandler( &oErrorHandler ) ;
		oParser.parse( *poSource ) ;
		delete( poSource ) ;

		iError = oErrorHandler.GetError() ;

		if( iError == 0 )
		{
			poNode = oParser.getDocument()->getDocumentElement()->getFirstChild() ;

			while( poNode != NULL )
			{
				if( poNode->getNodeType() == DOMNode::ELEMENT_NODE )
				{
					bStart = M4_TRUE ;
					poAttribute = poNode->getAttributes()->getNamedItem( M4_XERCES_LITERAL( "startswith" ) ) ;

					if( poAttribute == NULL )
					{
						bStart = M4_FALSE ;
						poAttribute = poNode->getAttributes()->getNamedItem( M4_XERCES_LITERAL( "endswith" ) ) ;
					}

					if( poAttribute != NULL )
					{
						pcAttributeValue = M4XMLToCpp( (M4XMLCh*)poAttribute->getNodeValue() ) ;

						if( pcAttributeValue == NULL )
						{
							printf( "\nNot enough memory to allocate <%d> bytes.", (m4uint32_t)( sizeof( m4pchar_t ) * ( XMLString::stringLen( poAttribute->getNodeValue() ) + 1 ) ) ) ;
							return( M4_ERROR ) ;
						}

						pcNodeName = M4XMLToCpp( (M4XMLCh*)poNode->getNodeName() ) ;

						if( pcNodeName == NULL )
						{
							printf( "\nNot enough memory to allocate <%d> bytes.", (m4uint32_t)( sizeof( m4pchar_t ) * ( XMLString::stringLen( poNode->getNodeName() ) + 1 ) ) ) ;
							delete( pcAttributeValue ) ;
							return( M4_ERROR ) ;
						}

						if( strcmpi( pcNodeName, "include" ) == 0 )
						{
							if( bStart == M4_TRUE )
							{
								ai_rvStartIncludes.push_back( pcAttributeValue ) ;
							}
							else
							{
								ai_rvEndIncludes.push_back( pcAttributeValue ) ;
							}
						}
						else
						{
							if( bStart == M4_TRUE )
							{
								ai_rvStartExcludes.push_back( pcAttributeValue ) ;
							}
							else
							{
								ai_rvEndExcludes.push_back( pcAttributeValue ) ;
							}
						}
						delete( pcNodeName ) ;
					}
				}

				poNode = poNode->getNextSibling() ;
			}
		}
		else
		{
			pcException = M4XMLToCpp( oErrorHandler.GetMessage() ) ;
			printf( "\nSystem exception with message <%s>.", pcException ) ;
			delete( pcException ) ;
			return( M4_ERROR ) ;
		}
	}
	catch( const XMLException &e )
	{
		pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		printf( "\nSystem exception with message <%s>.", pcException ) ;
		delete( pcException ) ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}
