//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ini
// File:                m4xmlreg.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                22-01-2008
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module implements the functionality to access registry information from xml files.
//
//
//==============================================================================


#include "m4xmlreg.hpp"
#include "cldefine.hpp"
#include "m4unicode.hpp"
#include "syncro.hpp"
#include "blocksynchronization.hpp"
#include "m4regdef.hpp"
#include "m4buildversion.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>

#ifdef _WINDOWS
#include <shlwapi.h>
#include <direct.h>
#include <shlobj.h>
#else
#include <sys/stat.h>
#include <wchar.h>
#endif


#include "xercesc/dom/DOMNamedNodeMap.hpp"
#include "xercesc/framework/LocalFileInputSource.hpp"
#include "xercesc/framework/MemBufInputSource.hpp"
#include "xercesc/sax/ErrorHandler.hpp"
#include "xercesc/sax/SAXParseException.hpp"
#include "xercesc/util/XMLChar.hpp"

extern void	M4SetUnicode( int ) ;



// ============================================================================
// Defines
// ============================================================================
#define	M4INI_CSIDL_COMMON_APPDATA						0x023		// No lo tenemos definido en nuestros includes

#define	M4INI_MAX_REG_FILES								8

#define	M4INI_MAX_REG_LINE								4096

#ifdef _WINDOWS
#define	M4INI_DEBUG_FILE								"regmeta4.log"
#define	M4INI_DEBUG_DIR									"TEMP"
#define	M4INI_MAX_PATH									MAX_PATH
#else
#define	M4INI_DEBUG_FILE								".regmeta4.log"
#define	M4INI_DEBUG_DIR									"HOME"
#define	M4INI_MAX_PATH									PATH_MAX
#endif

#define M4INI_RETURN_ERROR0( cond, format )				do { if( cond ) { _DumpError( format##". Line <%d> file <%s> condition <%s>\n",				__LINE__, __FILE__, #cond ) ; return( M4_ERROR ) ; } ; } while( 0 )
#define M4INI_RETURN_ERROR1( cond, a, format )			do { if( cond ) { _DumpError( format##". Line <%d> file <%s> condition <%s>\n", a,			__LINE__, __FILE__, #cond ) ; return( M4_ERROR ) ; } ; } while( 0 )
#define M4INI_RETURN_ERROR2( cond, a, b, format )		do { if( cond ) { _DumpError( format##". Line <%d> file <%s> condition <%s>\n", a, b,		__LINE__, __FILE__, #cond ) ; return( M4_ERROR ) ; } ; } while( 0 )
#define M4INI_RETURN_ERROR3( cond, a, b, c, format )	do { if( cond ) { _DumpError( format##". Line <%d> file <%s> condition <%s>\n", a, b, c,	__LINE__, __FILE__, #cond ) ; return( M4_ERROR ) ; } ; } while( 0 )

#define M4INI_CHECK_ERROR0( cond, format )				do { if( cond ) { _DumpError( format##". Line <%d> file <%s> condition <%s>\n",				__LINE__, __FILE__, #cond ) ; return ; } ; } while( 0 )
#define M4INI_CHECK_ERROR1( cond, a, format )			do { if( cond ) { _DumpError( format##". Line <%d> file <%s> condition <%s>\n", a,			__LINE__, __FILE__, #cond ) ; return ; } ; } while( 0 )
#define M4INI_CHECK_ERROR2( cond, a, b, format )		do { if( cond ) { _DumpError( format##". Line <%d> file <%s> condition <%s>\n", a, b,		__LINE__, __FILE__, #cond ) ; return ; } ; } while( 0 )
#define M4INI_CHECK_ERROR3( cond, a, b, c, format )		do { if( cond ) { _DumpError( format##". Line <%d> file <%s> condition <%s>\n", a, b, c, 	__LINE__, __FILE__, #cond ) ; return ; } ; } while( 0 )

#define M4INI_DUMP_ERROR0( format )						do { _DumpError( format##". Line <%d> file <%s>\n",				__LINE__, __FILE__ ) ; } while( 0 )
#define M4INI_DUMP_ERROR1( a, format )					do { _DumpError( format##". Line <%d> file <%s>\n", a,			__LINE__, __FILE__ ) ; } while( 0 )
#define M4INI_DUMP_ERROR2( a, b, format )				do { _DumpError( format##". Line <%d> file <%s>\n", a, b,		__LINE__, __FILE__ ) ; } while( 0 )
#define M4INI_DUMP_ERROR3( a, b, c, format )			do { _DumpError( format##". Line <%d> file <%s>\n", a, b, c,	__LINE__, __FILE__ ) ; } while( 0 )

// Descomentar para activar trazas de lectura y escritura
// #define	M4_INI_TRACE

#ifdef	M4_INI_TRACE
#define M4INI_DUMP_TRACE0( format )						M4INI_DUMP_ERROR0( format )
#define M4INI_DUMP_TRACE1( a, format )					M4INI_DUMP_ERROR1( a, format )
#define M4INI_DUMP_TRACE2( a, b, format )				M4INI_DUMP_ERROR2( a, b, format )
#define M4INI_DUMP_TRACE3( a, b, c, format )			M4INI_DUMP_ERROR3( a, b, c, format )
#else
#define M4INI_DUMP_TRACE0( format )
#define M4INI_DUMP_TRACE1( a, format )
#define M4INI_DUMP_TRACE2( a, b, format )
#define M4INI_DUMP_TRACE3( a, b, c, format )
#endif


//=================================================================================
// Variables globales
//=================================================================================

static	ClMutex	g_oMutex( M4_TRUE ) ;
static	ClMutex	g_oFileMutex( M4_TRUE ) ;



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
	const XMLCh*	GetMessage( void ) const
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

			m_pwcMessage = new XMLCh[ iLength ] ;

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
	XMLCh		*m_pwcMessage ;

} ;





//=================================================================================
// Funciones auxiliares
//=================================================================================


//=================================================================================
// Limpia el contenido de una lista de nombres
//=================================================================================

void	_ClearList( list<m4pchar_t> &ao_rlSubKeys )
{

	m4pchar_t	pcKeyName = NULL ;
	list<m4pchar_t>::iterator	itIterator ;


	// Se itera por los elementos limpiando
	itIterator = ao_rlSubKeys.begin() ;

	while( itIterator != ao_rlSubKeys.end() )
	{
		pcKeyName = *itIterator ;
		delete [] pcKeyName ;
		itIterator++ ;
	}
	ao_rlSubKeys.clear() ;
}



//=================================================================================
// Vuelca un mensaje de error al dispositivo de salida
//
// Pasos
//		Si no está creado el dispositivo de salida se crea.
//		En windows se crea en el temporal y en unix en el home.
//		Si existe dispositivo de salida se vuelca el mensaje formateado.
//		Devuelve el número de bytes escritos.
//
//=================================================================================

static	int	_DumpError( m4pcchar_t ai_pccFormat, ... )
{

	int			iWrite =  0 ;
	m4pcchar_t	pccFileDir = NULL ;
	m4char_t	acFile[ M4INI_MAX_PATH + 1 ] ;
	va_list		list ;

	static FILE *s_pfOutput = NULL;

	
	if( s_pfOutput == NULL )
	{
		pccFileDir = getenv( M4INI_DEBUG_DIR ) ;

		if( pccFileDir == NULL )
		{
			// Si no se consigue el directorio se pone en el actual
			pccFileDir = "." ;
		}

		sprintf( acFile, "%s%s%s", pccFileDir, M4_BACK_SLASH, M4INI_DEBUG_FILE ) ;

		eUniFileEncodingType_t eEncoding = M4UniNative ;
		s_pfOutput = M4Fopen( acFile, M4UniWrite, eEncoding ) ;
	}

	if( s_pfOutput != NULL )
	{
		va_start( list, ai_pccFormat ) ;
		iWrite = vfprintf( s_pfOutput, ai_pccFormat, list ) ;
		fflush( s_pfOutput ) ;
		va_end( list ) ;
	}

	return( iWrite ) ;
}



//=================================================================================
//
// Escribe una cadena escapada a un fichero xml
// Recorre la cadena escapando los caracteres de xml
//
// @param ao_pfFile
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

static	int	_WriteScaped( FILE* ao_pfFile, m4pcchar_t ai_pccValue )
{

	int			iResult =  0 ;
	int			iWrite =  0 ;
	m4char_t	c = '\0' ;
	size_t		i = 0 ;
	size_t		iLength = 0 ;


	if( ai_pccValue == NULL )
	{
		// Si es nulo no hay valor que escribir
		return( 0 ) ;
	}

	iLength = strlen( ai_pccValue ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		c = ai_pccValue[ i ] ;

		switch( c )
		{
			case '<':
				iWrite = fprintf( ao_pfFile, "&lt;" ) ;
				break;

			case '>':
				iWrite = fprintf( ao_pfFile, "&gt;" ) ;
				break;

			case '"':
				iWrite = fprintf( ao_pfFile, "&quot;" ) ;
				break;

			case '\'':
				iWrite = fprintf( ao_pfFile, "&apos;" ) ;
				break;

			case '&':
				iWrite = fprintf( ao_pfFile, "&amp;" ) ;
				break;

			default:
				iWrite = fprintf( ao_pfFile, "%c", c ) ;
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



// ============================================================================
// Imprime un nodo a fichero de manera recursiva
//
// @param ao_pfFile
//
//		Fichero al que se escribe
//
// @param ai_poNode
//
//		Nodo que se escribe
//
// @param ai_iIndent
//
//		Nivel de tabulación
//
// @param ai_pccFileName
//
//		Nombre del fichero al que se escribe (para depuración)
//
// @return
//
//		M4_ERROR si se ha producido algún error o M4_SUCCESS en caso contrario
// ============================================================================

static	m4return_t	_PrintNode( FILE *ao_pfFile, const DOMNode *ai_poNode, size_t ai_iIndent, m4pcchar_t ai_pccFileName )
{

	m4return_t			iResult = M4_ERROR ;
	size_t				i = 0 ;
	size_t				iLength = 0 ;
	int					iWrite = -1 ;
	m4pchar_t			pcNodeName = NULL ;
	m4pchar_t			pcAttributeName = NULL ;
	m4pchar_t			pcAttributeValue = NULL ;
    m4char_t			acIndent[ 50 ] ;
	DOMNode				*poNextNode = NULL ;
	DOMNode				*poAttribute = NULL ;
	DOMNamedNodeMap		*poAttributes = NULL ;


    for( i = 0 ; i < ai_iIndent ; i++ )
    {
        acIndent[ i ] = '\t' ;
    }

    acIndent[ i ] = '\0' ;

	// Se imprime el elemento
	pcNodeName = M4XMLToUtf8( (M4XMLCh*) ai_poNode->getNodeName() ) ;
	iWrite = fprintf( ao_pfFile, "%s<%s", acIndent, pcNodeName ) ;
	delete [] pcNodeName ;
	M4INI_RETURN_ERROR2( iWrite < 0, ai_pccFileName, strerror( errno ), "Cannot write to file <%s> because <%s>" ) ;

	// Se imprimen los atributos
	poAttributes = ai_poNode->getAttributes() ;
	iLength = poAttributes->getLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poAttribute = poAttributes->item( i ) ;

		pcAttributeName = M4XMLToUtf8( (M4XMLCh*) poAttribute->getNodeName() ) ;
		iWrite = fprintf( ao_pfFile, " %s='", pcAttributeName ) ;
		delete [] pcAttributeName ;
		M4INI_RETURN_ERROR2( iWrite < 0, ai_pccFileName, strerror( errno ), "Cannot write to file <%s> because <%s>" ) ;

		pcAttributeValue = M4XMLToUtf8( (M4XMLCh*) poAttribute->getNodeValue() ) ;
		iWrite = _WriteScaped( ao_pfFile, pcAttributeValue ) ;
		delete [] pcAttributeValue ;
		M4INI_RETURN_ERROR2( iWrite < 0, ai_pccFileName, strerror( errno ), "Cannot write to file <%s> because <%s>" ) ;

		iWrite = fprintf( ao_pfFile, "'" ) ;
		M4INI_RETURN_ERROR2( iWrite < 0, ai_pccFileName, strerror( errno ), "Cannot write to file <%s> because <%s>" ) ;
	}

	iWrite = fprintf( ao_pfFile, ">\n" ) ;
	M4INI_RETURN_ERROR2( iWrite < 0, ai_pccFileName, strerror( errno ), "Cannot write to file <%s> because <%s>" ) ;

	// Se itera por los hijos para imprimir recursivamente
	poNextNode = ai_poNode->getFirstChild() ;
	while( poNextNode != NULL )
	{
		if( poNextNode->getNodeType() == DOMNode::ELEMENT_NODE )
		{
			iResult = _PrintNode( ao_pfFile, poNextNode, ai_iIndent + 1, ai_pccFileName ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}

		poNextNode = poNextNode->getNextSibling() ;
	}

	pcNodeName = M4XMLToUtf8( (M4XMLCh*) ai_poNode->getNodeName() ) ;
	iWrite = fprintf( ao_pfFile, "%s</%s>\n", acIndent, pcNodeName ) ;
	delete [] pcNodeName ;
	M4INI_RETURN_ERROR2( iWrite < 0, ai_pccFileName, strerror( errno ), "Cannot write to file <%s> because <%s>" ) ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene un nombre xml válido a partir de un identificador.
// Cambia los caracteres no válidos por _.
// ============================================================================

static	m4return_t	_GetValidName( m4pcchar_t ai_pccName, size_t ai_iLength, m4pchar_t& ao_rpcValidName )
{

	bool		bIsFirstChar = false ;
	bool		bIsChar = false ;
	int			i = 0 ;
	int			iHead = 0 ;
	int			iLength = -1 ;
	M4XMLCh		wc = 0 ;
	M4XMLCh		*pwcName = NULL ;
	M4XMLCh		*pwcValidName = NULL ;


	ao_rpcValidName = NULL ;

	iLength = (int) ai_iLength ;
	pwcName = M4CppToUtf16( ai_pccName, iLength ) ;

	if( iLength == 0 )
	{
		// Si es cadena vacía no es válido
		iHead = 1 ;
	}
	else
	{
		bIsChar = XMLChar1_0::isNameChar( *pwcName ) ;
		bIsFirstChar = XMLChar1_0::isFirstNameChar( *pwcName ) ;

		if( bIsChar == true && bIsFirstChar == false )
		{
			iHead = 1 ;
		}
		else
		{
			iHead = 0 ;
		}
	}

	pwcValidName = new M4XMLCh[ iHead + iLength + 1 ] ;
	M4INI_RETURN_ERROR1( pwcValidName == NULL, sizeof( XMLCh ) * ( iHead + iLength + 1 ), "Cannot allocate <%d> bytes" ) ;

	// Si el primer caracter es inválido se añade un _
	if( iHead == 1 )
	{
		pwcValidName[ 0 ] = '_' ;
	}

	// Se cambian los caracteres inválidos de los nombres de los elementos por _
	for( i = 0 ; i < iLength ; i++ )
	{
		wc = pwcName[ i ] ;

		bIsChar = XMLChar1_0::isNameChar( wc ) ;

		if( bIsChar == false )
		{
			wc = '_' ;
		}

		pwcValidName[ iHead + i ] = wc ;
	}
	pwcValidName[ iHead + iLength ] = '\0' ;
	delete [] pwcName ;

	iLength = iHead + iLength ;
	ao_rpcValidName = M4Utf16ToCpp( pwcValidName, iLength ) ;
	delete [] pwcValidName ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Busca un elemento dentro de una rama del documento.
// Lo hace de manera recursiva.
// Puede crear la rama si no existe y así se pide.
// ============================================================================

static	m4return_t	_GetElement( DOMNode *ai_poParentElement, m4pcchar_t ai_pccKey, m4bool_t ai_bCreate, DOMNode* &ao_rpoElement )
{

	m4return_t	iResult = M4_ERROR ;
	size_t		iLength = 0 ;
	int			iCompare = 0 ;
	m4pcchar_t	pccSlash = NULL ;
	m4pchar_t	pcKey = NULL ;
	m4pchar_t	pcNodeName = NULL ;
	M4XMLCh		*pwcKey = NULL ;
	DOMNode		*poNextNode = NULL ;
	DOMNode		*poChildElement = NULL ;


	ao_rpoElement = NULL ;

	// Se busca la rama actual a buscar
	pccSlash = strchr( ai_pccKey, '\\' ) ;

	if( pccSlash != NULL )
	{
		iLength = pccSlash - ai_pccKey ;

		while( *pccSlash == '\\' )
		{
			pccSlash++ ;
		}
	}
	else
	{
		iLength = strlen( ai_pccKey ) ;
	}

	iResult = _GetValidName( ai_pccKey, iLength, pcKey ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se itera por los elementos hijo buscando el elemento actual de la rama
	poChildElement = NULL ;
	poNextNode = ai_poParentElement->getFirstChild() ;
	while( poNextNode != NULL )
	{
		if( poNextNode->getNodeType() == DOMNode::ELEMENT_NODE )
		{
			pcNodeName = M4XMLToCpp( (M4XMLCh*) poNextNode->getNodeName() ) ;
			iCompare = M4StrCmpi( pcNodeName, pcKey ) ;
			delete [] pcNodeName ;

			if( iCompare == 0 )
			{
				poChildElement = poNextNode ;
				break ;
			}
		}

		poNextNode = poNextNode->getNextSibling() ;
	}

	// Si no existe el elemento y hay que crearlo se crea y añade
	if( poChildElement == NULL && ai_bCreate == M4_TRUE )
	{
		pwcKey = M4CppToXML( pcKey ) ;
	    poChildElement = (DOMNode*) ai_poParentElement->getOwnerDocument()->createElement( (XMLCh*) pwcKey ) ;
		delete [] pwcKey ;
		ai_poParentElement->appendChild( poChildElement ) ;
	}

	delete [] pcKey ;

	if( poChildElement == NULL )
	{
		// Si no se ha encontrado se retorna error
		return( M4_ERROR ) ;
	}

	if( pccSlash != NULL && *pccSlash != '\0' )
	{
		// Si no es el último en la rama se sigue si hay hijos
		iResult = _GetElement( poChildElement, pccSlash, ai_bCreate, ao_rpoElement ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}
	else
	{
		// Si es el último se devuelve
		ao_rpoElement = poChildElement ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Busca un atributo de un elemento.
// Puede crear el atributo si no existe y así se pide.
// ============================================================================

static	m4return_t	_GetAttribute( DOMNode *ai_poElement, m4pcchar_t ai_pccValue, m4bool_t ai_bCreate, DOMNode* &ao_rpoAttribute )
{

	m4return_t		iResult = M4_ERROR ;
	int				iCompare = 0 ;
	size_t			i = 0 ;
	size_t			iLength = 0 ;
	m4pchar_t		pcValue = NULL ;
	m4pchar_t		pcAttributeName = NULL ;
	M4XMLCh			*pwcValue = NULL ;
    DOMNode			*poAttribute = NULL ;
	DOMNamedNodeMap	*poAttributes = NULL ;


	ao_rpoAttribute = NULL ;

	iResult = _GetValidName( ai_pccValue, strlen( ai_pccValue ), pcValue ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	poAttributes = ai_poElement->getAttributes() ;
	iLength = poAttributes->getLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poAttribute = poAttributes->item( i ) ;

		pcAttributeName = M4XMLToCpp( (M4XMLCh*) poAttribute->getNodeName() ) ;
		iCompare = M4StrCmpi( pcAttributeName, pcValue ) ;
		delete [] pcAttributeName ;

		if( iCompare == 0 )
		{
			ao_rpoAttribute = poAttribute ;
			break ;
		}
	}

	// Si no existe el atributo y hay que crearlo se crea y añade
	if( ao_rpoAttribute == NULL && ai_bCreate == M4_TRUE )
	{
		pwcValue = M4CppToXML( pcValue ) ;
	    ao_rpoAttribute = (DOMNode*) ai_poElement->getOwnerDocument()->createAttribute( (XMLCh*) pwcValue ) ;
		delete [] pwcValue ;
		ai_poElement->getAttributes()->setNamedItem( ao_rpoAttribute ) ;
	}

	delete [] pcValue ;

	if( ao_rpoAttribute == NULL )
	{
		// Si no se ha encontrado se retorna error
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Busca un atributo de un elemento dentro de una rama del documento.
// Puede crear el atributo si no existe y así se pide.
// ============================================================================

static	m4return_t	_GetAttribute( DOMNode *ai_poParentElement, m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4bool_t ai_bCreate, DOMNode* &ao_rpoAttribute )
{

	m4return_t	iResult = M4_ERROR ;
	DOMNode		*poElement = NULL ;


	ao_rpoAttribute = NULL ;

	iResult = _GetElement( ai_poParentElement, ai_pccKey, ai_bCreate, poElement ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = _GetAttribute( poElement, ai_pccValue, ai_bCreate, ao_rpoAttribute ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Crea un documento xml vacío con un root de tipo HKEY_LOCAL_MACHINE
// ============================================================================

static	m4return_t	_CreateEmptyDocument( XercesDOMParser& ai_roParser, DOMNode* &ao_rpoRoot )
{

	m4return_t		iError = 0 ;
	m4pchar_t		pcEmpty = "<HKEY_LOCAL_MACHINE/>" ;
	m4pchar_t		pcException = NULL ;
	InputSource		*poSource = NULL ;
	ClErrorHandler	oErrorHandler ;


	ao_rpoRoot = NULL ;

	try
	{
		poSource = new MemBufInputSource( (XMLByte*) pcEmpty, strlen( pcEmpty ), "" ) ;
		M4INI_RETURN_ERROR1( poSource == NULL, sizeof( MemBufInputSource ), "Cannot allocate <%d> bytes" ) ;

		ai_roParser.setErrorHandler( &oErrorHandler ) ;
		ai_roParser.parse( *poSource ) ;

		iError = oErrorHandler.GetError() ;

		if( iError == 0 )
		{
			ao_rpoRoot = (DOMNode*) ai_roParser.getDocument()->getDocumentElement() ;
		}
		else
		{
			pcException = M4XMLToCpp( (M4XMLCh*) oErrorHandler.GetMessage() ) ;
			M4INI_DUMP_ERROR1( pcException, "System exception with message <%s>" ) ;
			delete [] pcException ;
		}
	}
	catch( const XMLException &e )
	{
		pcException = M4XMLToCpp( (M4XMLCh*) e.getMessage() ) ;
		M4INI_DUMP_ERROR1( pcException, "System exception with message <%s>" ) ;
		delete [] pcException ;
	}

	delete( poSource ) ;

	if( ao_rpoRoot == NULL )
	{
		// Si no se ha podido crear se retorna error
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Funciones de acceso al registro de windows
//=================================================================================

#ifdef _WINDOWS

// ============================================================================
// Obtiene del registro el valor de un atributo del documento.
// ============================================================================

m4return_t	_RegistryGetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pchar_t &ao_rpcData )
{

	int			iLength = -1 ;
	LONG		lResult = -1 ;
	DWORD		dwType = -1 ;
	DWORD		dwLength = -1 ;
	HKEY		hKey ;
	M4XMLCh		*pwcKey = NULL ;
	M4XMLCh		*pwcValue = NULL ;
	M4XMLCh		*pwcData = NULL ;
	M4XMLCh		awcData[ M4INI_MAX_REG_VALUE_LEN + 1 ] ;
	M4XMLCh		awcNumber[ 128 + 1 ] ;

	
	ao_rpcData = NULL ;

	M4INI_DUMP_TRACE2( ai_pccValue, ai_pccKey, "Reading value <%s> of registry key <%s>" ) ;

	iLength = -1 ;
	pwcKey = M4CppToUtf16( ai_pccKey, iLength ) ;
	lResult = RegOpenKeyExW( HKEY_LOCAL_MACHINE, (LPCWSTR)pwcKey, 0, KEY_QUERY_VALUE, &hKey ) ;
	delete [] pwcKey ;
	M4INI_RETURN_ERROR2( lResult != ERROR_SUCCESS, lResult, ai_pccKey, "*** ERROR <%d> opening registry key <%s>" ) ;

	iLength = -1 ;
	pwcValue = M4CppToUtf16( ai_pccValue, iLength ) ;
	dwLength = M4INI_MAX_REG_VALUE_LEN ;
	*awcData = '\0' ;
	lResult = RegQueryValueExW( hKey, (LPCWSTR)pwcValue, NULL, &dwType, (BYTE*)awcData, &dwLength ) ;
	delete [] pwcValue ;

	if( lResult != ERROR_SUCCESS )
	{
		M4INI_DUMP_ERROR3( lResult, ai_pccValue, ai_pccKey, "*** ERROR <%d> reading value <%s> of registry key <%s>" ) ;
		lResult = RegCloseKey( hKey ) ;
		return( M4_ERROR ) ;
	}

	lResult = RegCloseKey( hKey ) ;

	if( dwType == REG_DWORD )
	{
		swprintf((wchar_t*)awcNumber, L"%.1d", *((DWORD*)awcData));
		pwcData = awcNumber ;
	}
	else if( dwType == REG_SZ )
	{
		pwcData = (M4XMLCh*) awcData ;
	}
	else
	{
		M4INI_DUMP_ERROR3( dwType, ai_pccValue, ai_pccKey, "*** ERROR invalid type <%d> reading value <%s> of registry key <%s>" ) ;
		return( M4_ERROR ) ;
	}

	iLength = -1 ;
	ao_rpcData = M4Utf16ToCpp( pwcData, iLength ) ;

	M4INI_DUMP_TRACE1( ao_rpcData, "Success reading value <%s>" ) ;
	return( M4_SUCCESS ) ;
}



// ============================================================================
// Establece en el registro el valor de un atributo del documento.
// ============================================================================

m4return_t	_RegistrySetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pcchar_t ai_pccData )
{

	int			iLength = -1 ;
	LONG		lResult = -1 ;
	DWORD		dwDisposition = -1 ;
	HKEY		hKey ;
	M4XMLCh		*pwcKey = NULL ;
	M4XMLCh		*pwcValue = NULL ;
	M4XMLCh		*pwcData = NULL ;


	M4INI_DUMP_TRACE2( ai_pccValue, ai_pccKey, "Writing value <%s> of registry key <%s>" ) ;

	iLength = -1 ;
	pwcKey = M4CppToUtf16( ai_pccKey, iLength ) ;
	lResult = RegCreateKeyExW( HKEY_LOCAL_MACHINE, (LPCWSTR)pwcKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisposition ) ;
	delete [] pwcKey ;
	M4INI_RETURN_ERROR2( lResult != ERROR_SUCCESS, lResult, ai_pccKey, "*** ERROR <%d> opening registry key <%s>" ) ;

	// Se crea siempre de tipo cadena
	iLength = -1 ;
	pwcValue = M4CppToUtf16( ai_pccValue, iLength ) ;
	iLength = -1 ;
	pwcData = M4CppToUtf16( ai_pccData, iLength ) ;
	lResult = RegSetValueExW( hKey, (LPCWSTR)pwcValue, 0, REG_SZ, (CONST BYTE*)pwcData, ( iLength + 1 ) * sizeof( M4XMLCh ) ) ;
	delete [] pwcValue ;
	delete [] pwcData ;

	if( lResult != ERROR_SUCCESS )
	{
		M4INI_DUMP_ERROR3( lResult, ai_pccValue, ai_pccKey, "*** ERROR <%d> setting value <%s> of registry key <%s>" ) ;
		lResult = RegCloseKey( hKey ) ;
		return( M4_ERROR ) ;
	}

	lResult = RegCloseKey( hKey ) ;

	M4INI_DUMP_TRACE0( "Success writing value" ) ;
	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene del registro la lista de los nombres de los atributos
// o de los elementos hijos de un elemento según un parámetro.
// ============================================================================

m4return_t	_RegistryGetElementNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlElements, m4bool_t ai_bValue )
{

	int			iLength = -1 ;
	LONG		lResult = -1 ;
	DWORD		dwIndex = -1 ;
	DWORD		dwLength = -1 ;
	HKEY		hKey ;
	m4pchar_t	pcName = NULL ;
	M4XMLCh		*pwcKey = NULL ;
	M4XMLCh		awcName[ M4INI_MAX_REG_VALUE_LEN + 1 ] ;


	// Se limpia la lista
	_ClearList( ao_rlElements ) ;

	iLength = -1 ;
	pwcKey = M4CppToUtf16( ai_pccKey, iLength ) ;
	lResult = RegOpenKeyExW( HKEY_LOCAL_MACHINE, (LPCWSTR)pwcKey, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hKey ) ;
	delete [] pwcKey ;
	M4INI_RETURN_ERROR2( lResult != ERROR_SUCCESS, lResult, ai_pccKey, "*** ERROR <%d> opening registry key <%s>" ) ;

	dwIndex = 0 ;
	while( 1 )
	{
		dwLength = M4INI_MAX_REG_VALUE_LEN ;
		*awcName = '\0' ;

		if( ai_bValue == M4_TRUE )
		{
			lResult = RegEnumValueW( hKey, dwIndex, (LPWSTR)awcName, &dwLength, NULL, NULL, NULL, NULL ) ;
		}
		else
		{
			lResult = RegEnumKeyExW( hKey, dwIndex, (LPWSTR)awcName, &dwLength, NULL, NULL, NULL, NULL ) ;
		}

		if( lResult == ERROR_NO_MORE_ITEMS )
		{
			// Si no hay más elementos se acaba
			break ;
		}

		if( lResult != ERROR_SUCCESS )
		{
			M4INI_DUMP_ERROR2( lResult, ai_pccKey, "*** ERROR <%d> enumerating registry key <%s>" ) ;
			lResult = RegCloseKey( hKey ) ;
			return( M4_ERROR ) ;
		}

		if( dwLength > 0 )
		{
			iLength = dwLength ;
			pcName = M4Utf16ToCpp( awcName, iLength ) ;
			ao_rlElements.push_back( pcName ) ;
		}

		dwIndex++ ;
	}

	lResult = RegCloseKey( hKey ) ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene del registro la lista de los nombres de los atributos de un elemento.
// ============================================================================

m4return_t	_RegistryGetValueNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlValues )
{
	return( _RegistryGetElementNames( ai_pccKey, ao_rlValues, M4_TRUE ) ) ;
}



// ============================================================================
// Obtiene del registro la lista de los nombres de los elementos hijos de un elemento.
// ============================================================================

m4return_t	_RegistryGetSubKeyNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlSubKeys )
{
	return( _RegistryGetElementNames( ai_pccKey, ao_rlSubKeys, M4_FALSE ) ) ;
}



// ============================================================================
// Borra del registro un atributo.
// ============================================================================

m4return_t	_RegistryDeleteValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue )
{
	int			iLength = -1 ;
	LONG		lResult = -1 ;
	DWORD		dwDisposition = -1 ;
	HKEY		hKey ;
	M4XMLCh		*pwcKey = NULL ;
	M4XMLCh		*pwcValue = NULL ;


	iLength = -1 ;
	pwcKey = M4CppToUtf16( ai_pccKey, iLength ) ;
	lResult = RegCreateKeyExW( HKEY_LOCAL_MACHINE, (LPCWSTR)pwcKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisposition ) ;
	delete [] pwcKey ;
	M4INI_RETURN_ERROR2( lResult != ERROR_SUCCESS, lResult, ai_pccKey, "*** ERROR <%d> opening registry key <%s>" ) ;

	iLength = -1 ;
	pwcValue = M4CppToUtf16( ai_pccValue, iLength ) ;
	lResult = RegDeleteValueW( hKey, (LPCWSTR)pwcValue ) ;
	delete [] pwcValue ;

	if( lResult != ERROR_SUCCESS )
	{
		M4INI_DUMP_ERROR3( lResult, ai_pccValue, ai_pccKey, "*** ERROR <%d> deleting value <%s> of registry key <%s>" ) ;
		lResult = RegCloseKey( hKey ) ;
		return( M4_ERROR ) ;
	}

	lResult = RegCloseKey( hKey ) ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Borra del registro un elemento.
// ============================================================================

m4return_t	_RegistryDeleteKey( m4pcchar_t ai_pccKey )
{

	int			iLength = -1 ;
	LONG		lResult = -1 ;
	m4return_t	iResult = M4_ERROR ;
	m4pcchar_t	pccKeyName = NULL ;
	M4XMLCh		*pwcKey = NULL ;
	m4char_t	acSubKey[ M4INI_MAX_REG_LINE + 1 ] ;

	list<m4pchar_t>				lKeys ;
	list<m4pchar_t>::iterator	itIterator ;


	// Se borran primero las ramas hijas recursivamente
	iResult = _RegistryGetSubKeyNames( ai_pccKey, lKeys ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	itIterator = lKeys.begin() ;

	while( itIterator != lKeys.end() )
	{
		pccKeyName = *itIterator ;
		sprintf( acSubKey, "%s%s%s", ai_pccKey, M4_BACK_SLASH, pccKeyName ) ;

		// Se procesa recursivamente
		iResult = _RegistryDeleteKey( acSubKey ) ;

		if( iResult != M4_SUCCESS )
		{
			_ClearList( lKeys ) ;
			return( M4_ERROR ) ;
		}

		itIterator++ ;
	}
	_ClearList( lKeys ) ;


	// Se borra la rama ya sin hijas
	iLength = -1 ;
	pwcKey = M4CppToUtf16( ai_pccKey, iLength ) ;
	lResult = RegDeleteKeyW( HKEY_LOCAL_MACHINE, (LPCWSTR)pwcKey ) ;
	delete [] pwcKey ;
	M4INI_RETURN_ERROR2( lResult != ERROR_SUCCESS, lResult, ai_pccKey, "*** ERROR <%d> deleting registry key <%s>" ) ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Carga una rama del registro de windows de manera recursiva
// ============================================================================

static	m4return_t	_LoadKey( m4pcchar_t ai_pccKey, DOMNode *ai_poRoot )
{

	m4return_t	iResult = M4_ERROR ;
	m4pchar_t	pccValueName = NULL ;
	m4pchar_t	pccKeyName = NULL ;
	m4pchar_t	pcData = NULL ;
	M4XMLCh		*pcwData = NULL ;
	m4char_t	acSubKey[ M4INI_MAX_REG_LINE + 1 ] ;
	DOMNode		*poElement = NULL ;
	DOMNode		*poAttribute = NULL ;

	list<m4pchar_t>				lValues ;
	list<m4pchar_t>				lKeys ;
	list<m4pchar_t>::iterator	itIterator ;


	// Se crea la rama recursivamente
	iResult = _GetElement( ai_poRoot, ai_pccKey, M4_TRUE, poElement ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se obtienen los valores
	iResult = _RegistryGetValueNames( ai_pccKey, lValues ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	itIterator = lValues.begin() ;

	while( itIterator != lValues.end() )
	{
		pccValueName = *itIterator ;

		// Se obtiene el atributo
		iResult = _GetAttribute( ai_poRoot, ai_pccKey, pccValueName, M4_TRUE, poAttribute ) ;

		if( iResult != M4_SUCCESS )
		{
			_ClearList( lValues ) ;
			return( M4_ERROR ) ;
		}

		iResult = _RegistryGetValue( ai_pccKey, pccValueName, pcData ) ;

		if( iResult != M4_SUCCESS )
		{
			_ClearList( lValues ) ;
			return( M4_ERROR ) ;
		}

		pcwData = M4CppToXML( pcData ) ;
		delete [] pcData ;
		poAttribute->setNodeValue( (XMLCh*) pcwData ) ;
		delete [] pcwData ;

		itIterator++ ;
	}
	_ClearList( lValues ) ;


	// Se obtienen las ramas
	iResult = _RegistryGetSubKeyNames( ai_pccKey, lKeys ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	itIterator = lKeys.begin() ;

	while( itIterator != lKeys.end() )
	{
		pccKeyName = *itIterator ;
		sprintf( acSubKey, "%s%s%s", ai_pccKey, M4_BACK_SLASH, pccKeyName ) ;

		// Se procesa recursivamente
		iResult = _LoadKey( acSubKey, ai_poRoot ) ;

		if( iResult != M4_SUCCESS )
		{
			_ClearList( lKeys ) ;
			return( M4_ERROR ) ;
		}

		itIterator++ ;
	}
	_ClearList( lKeys ) ;


	return( M4_SUCCESS ) ;
}


	
// ============================================================================
// Obtiene el elemento raiz a partir del registro de windows.
// Se abre la rama PeopleNet y si existe se crea recursivamente.
// ============================================================================

static	m4return_t	_LoadFromWindows( XercesDOMParser& ai_roParser, DOMNode* &ao_rpoNode )
{

	m4return_t	iResult = M4_ERROR ;
	LONG		lResult = -1 ;
	HKEY		hKey ;
	m4pcchar_t	pccKey = NULL ;
	DOMNode		*poRoot = NULL ;


	ao_rpoNode = NULL ;

	// Se crea un documento vacío con el elemento raiz
	iResult = _CreateEmptyDocument( ai_roParser, poRoot ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se comprueba que exista la rama peoplenet del registro
	pccKey = "SOFTWARE\\Meta4\\PeopleNet";
	lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, pccKey, 0, KEY_QUERY_VALUE, &hKey ) ;
	M4INI_RETURN_ERROR2( lResult != ERROR_SUCCESS, lResult, pccKey, "*** ERROR <%d> opening registry key <%s>" ) ;
	lResult = RegCloseKey( hKey ) ;

	// Se crea la rama recursivamente
	iResult = _LoadKey( pccKey, poRoot ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	ao_rpoNode = poRoot ;

	return( M4_SUCCESS ) ;
}

#endif



// ============================================================================
// Obtiene el elemento raiz a partir de un fichero xml.
// Si el fichero existe se abre, se parsea y se obtiene el documento xml.
// ============================================================================

static	m4return_t	_LoadFromXml( m4pchar_t ai_pccFile, XercesDOMParser& ai_roParser, DOMNode* &ao_rpoRoot )
{

	m4return_t		iError = 0 ;
	m4pchar_t		pcException = NULL ;
	M4XMLCh			*pwcFile = NULL ;
	InputSource		*poSource = NULL ;
	ClErrorHandler	oErrorHandler ;


	ao_rpoRoot = NULL ;

	try
	{
		// Si el fichero existe se intenta parsear
		pwcFile = M4CppToXML( ai_pccFile ) ;

		poSource = new LocalFileInputSource( (XMLCh*) pwcFile ) ;
		M4INI_RETURN_ERROR1( poSource == NULL, sizeof( LocalFileInputSource ), "Cannot allocate <%d> bytes" ) ;

		ai_roParser.setErrorHandler( &oErrorHandler ) ;
		ai_roParser.parse( *poSource ) ;

		iError = oErrorHandler.GetError() ;

		if( iError == 0 )
		{
			ao_rpoRoot = (DOMNode*) ai_roParser.getDocument()->getDocumentElement() ;
		}
		else
		{
			pcException = M4XMLToCpp( (M4XMLCh*) oErrorHandler.GetMessage() ) ;
			M4INI_DUMP_ERROR1( pcException, "System exception with message <%s>" ) ;
			delete [] pcException ;
		}
	}
	catch( const XMLException &e )
	{
		pcException = M4XMLToCpp( (M4XMLCh*) e.getMessage() ) ;
		M4INI_DUMP_ERROR1( pcException, "System exception with message <%s>" ) ;
		delete [] pcException ;
	}

	delete( poSource ) ;
	delete [] pwcFile ;

	if( ao_rpoRoot == NULL )
	{
		// Si no se ha podido crear se retorna error
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene el elemento raiz a partir de un fichero reg.
// Si el fichero existe se abre, se parsea y se obtienen el documento xml.
// ============================================================================

static	m4return_t	_LoadFromReg( FILE *ai_pfFile, XercesDOMParser& ai_roParser, DOMNode* &ao_rpoNode )
{

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bFound = M4_FALSE ;
	size_t		iRead = 0 ;
	size_t		iSize = 0 ;
	int			i = 0 ;
	int			j = 0 ;
	int			iLength = 0 ;
	int			iScan = 0 ;
	int			iValue = 0 ;
	m4pcchar_t	pccHeader = NULL ;
	m4pchar_t	pcLine = NULL ;
	m4pchar_t	pcQuote = NULL ;
	m4pchar_t	pcBackSlash = NULL ;
	m4pchar_t	pcWord = NULL ;
	m4pchar_t	pcKey = NULL ;
	m4pchar_t	pcName = NULL ;
	m4pchar_t	pcValidName = NULL ;
	m4pchar_t	pcValue = NULL ;
	M4XMLCh		*pwcValidName = NULL ;
	M4XMLCh		*pcwValue = NULL ;
	m4char_t	acNumber[ 128 + 1 ] ;
	wchar_t		acLine[ M4INI_MAX_REG_LINE + 1 ] ;
	DOMNode		*poRoot = NULL ;
	DOMNode		*poElement = NULL ;
	DOMNode		*poLastElement = NULL ;
	DOMNode		*poAttribute = NULL ;

	eUniFileEncodingType_t eEncoding = M4UniNative ;


	ao_rpoNode = NULL ;

	// Se crea un documento vacío con el elemento raiz
	iResult = _CreateEmptyDocument( ai_roParser, poRoot ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	pccHeader = "[HKEY_LOCAL_MACHINE\\" ;
	iSize = strlen( pccHeader ) ;

	// Se comprueba si es utf-16, utf-8 o ANSI
	iRead = fread( acLine, 1, M4UNI_UTF16_BOM_SIZE, ai_pfFile ) ;

	if( iRead == M4UNI_UTF16_BOM_SIZE && memcmp( acLine, acUTF16BOM, M4UNI_UTF16_BOM_SIZE ) == 0 )
	{
		eEncoding = M4UniUtf16 ;
	}
	else
	{
		fseek( ai_pfFile, 0, SEEK_SET ) ;

		iRead = fread( acLine, 1, M4UNI_UTF8_BOM_SIZE, ai_pfFile ) ;

		if( iRead == M4UNI_UTF8_BOM_SIZE && memcmp( acLine, acUTF8BOM, M4UNI_UTF8_BOM_SIZE ) == 0 )
		{
			eEncoding = M4UniUtf8 ;
		}
		else
		{
			fseek( ai_pfFile, 0, SEEK_SET ) ;
			eEncoding = M4UniANSI ;
		}
	}

	// Se recorren las entradas
	poLastElement = NULL ;

	do
	{
		*acLine = '\0' ;

		if( eEncoding == M4UniUtf16 )
		{
			pcLine = (m4pchar_t) fgetws( acLine, M4INI_MAX_REG_LINE, ai_pfFile ) ;

			if( pcLine != NULL )
			{
				iLength = -1 ;
				pcLine = M4Utf16ToCpp( (M4XMLCh*) pcLine, iLength ) ;
				memcpy( acLine, pcLine, iLength + 1 ) ;
				delete [] pcLine ;
				pcLine = (m4pchar_t) acLine ;
			}
		}
		else
		{
			pcLine = fgets( (m4pchar_t) acLine, M4INI_MAX_REG_LINE, ai_pfFile ) ;

			if( pcLine != NULL )
			{
				iLength = -1 ;

				if( eEncoding == M4UniUtf8 )
				{
					pcLine = M4Utf8ToCpp( pcLine, iLength ) ;
				}
				else
				{
					pcLine = M4ANSIToCpp( pcLine, iLength ) ;
				}
				memcpy( acLine, pcLine, iLength + 1 ) ;
				delete [] pcLine ;
				pcLine = (m4pchar_t) acLine ;
			}
		}

		if( pcLine == NULL )
		{
			break ;
		}

		iLength = (int) strlen( pcLine ) ;

		// Si viene el retorno de carro se quita
		if( iLength > 0 && pcLine[ iLength - 1 ] == '\n' )
		{
			iLength-- ;
			pcLine[ iLength ] = '\0' ;
		}

		if( iLength > 0 && pcLine[ iLength - 1 ] == '\r' )
		{
			iLength-- ;
			pcLine[ iLength ] = '\0' ;
		}

		// Si es una linea en blanco se sigue
		if( iLength == 0 )
		{
			continue ;
		}

		// Si es un valor por defecto no se trata
		if( *pcLine == '@' )
		{
			continue ;
		}

		if( strnicmp( pcLine, pccHeader, iSize ) == 0 && pcLine[ iLength - 1 ] == ']' )
		{
			// Se ha encontrado una rama
			pcKey = pcLine + iSize ;
			pcLine[ iLength - 1 ] = '\0' ;

			iResult = _GetElement( poRoot, pcKey, M4_TRUE, poElement ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			poLastElement = poElement ;
		}
		else
		{
			// Si no es una rama soportada hay que saltar sus atributos
			if( *pcLine == '[' )
			{
				poLastElement = NULL ;
			}

			// Es un valor o una cabecera de fichero (éstas se ignoran)
			if( poLastElement != NULL )
			{
				// Formatos soportados
				// "xxx"="yyy"
				// "xxx"=dword:yyy
				// Dentro de xxx no puede haber "

				bFound = M4_FALSE ;

				pcQuote = strchr( pcLine, '\"' ) ;

				if( pcQuote != NULL )
				{
					pcName = pcQuote + 1 ;
					pcQuote = strchr( pcName, '\"' ) ;

					if( pcQuote != NULL )
					{
						// Se ha encontrado el nombre
						*pcQuote = '\0' ;
						pcLine = pcQuote + 1 ;
						pcQuote = strchr( pcLine, '\"' ) ;

						if( pcQuote != NULL )
						{
							pcValue = pcQuote + 1 ;
							pcQuote = strrchr( pcValue, '\"' ) ;	// Se busca por atrás

							if( pcQuote != NULL )
							{
								*pcQuote = '\0' ;
								bFound = M4_TRUE ;

								// Si hay comillas o barras escapadas hay que eliminar la barra
								pcBackSlash = strchr( pcValue, '\\' ) ;

								if( pcBackSlash != NULL )
								{
									iLength = (int) strlen( pcValue ) ;
									pcValidName = new m4char_t[ iLength + 1 ] ;
									M4INI_RETURN_ERROR1( pcValidName == NULL, sizeof( m4char_t ) * ( iLength + 1 ), "Cannot allocate <%d> bytes" ) ;

									j = 0 ;
									for( i = 0 ; i < iLength ; i++ )
									{
										if( pcValue[ i ] == '\\' && ( pcValue[ i + 1 ] == '\\' || pcValue[ i + 1 ] == '\"' ) )
										{
											// Si es una barra que está escapando otra barra o unas comillas nos la saltamos
											i++ ;
										}
										pcValidName[ j++ ] = pcValue[ i ] ;
									}
									pcValidName[ j ] = '\0' ;

									// Se copia a la cadena original
									memcpy( pcValue, pcValidName, j + 1 ) ;
									delete [] pcValidName ;
								}
							}
						}
						else
						{
							pcWord = strstr( pcLine, "dword:" ) ;

							if( pcWord != NULL )
							{
								// Se parsea el hexadecimal
								iScan = sscanf( pcWord, "dword:%x", &iValue ) ;

								if( iScan == 1 )
								{
									// Se imprime sin decimales y con al menos 1 dígito
									sprintf( acNumber, "%.1d", iValue ) ;
									pcValue = acNumber ;
									bFound = M4_TRUE ;
								}
							}
						}
					}
				}

				if( bFound == M4_TRUE )
				{
					// Si el parseo es correcto se crea el atributo
					iResult = _GetValidName( pcName, strlen( pcName ), pcValidName ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}

					pwcValidName = M4CppToXML( pcValidName ) ;
					delete [] pcValidName ;
					poAttribute = (DOMNode*) poLastElement->getOwnerDocument()->createAttribute( (XMLCh*) pwcValidName ) ;
					delete [] pwcValidName ;

					poElement->getAttributes()->setNamedItem( poAttribute ) ;
					pcwValue = M4CppToXML( pcValue ) ;
					poAttribute->setNodeValue( (XMLCh*) pcwValue ) ;
					delete [] pcwValue ;
				}
			}
		}
	}
	while( pcLine != NULL ) ;

	ao_rpoNode = poRoot ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Carga una rama del registro de windows de manera recursiva
// ============================================================================

static	m4return_t	_MergeKey( m4pcchar_t ai_pccKey, M4XmlRegistryFile& ai_roSource, M4XmlRegistryFile& ai_roDestination )
{

	m4return_t	iResult = M4_ERROR ;
	m4pchar_t	pccValueName = NULL ;
	m4pchar_t	pccKeyName = NULL ;
	m4pchar_t	pcData = NULL ;
	m4char_t	acSubKey[ M4INI_MAX_REG_LINE + 1 ] ;

	list<m4pchar_t>				lValues ;
	list<m4pchar_t>				lKeys ;
	list<m4pchar_t>::iterator	itIterator ;


	// Se copia la rama recursivamente

	// Se obtienen los valores
	iResult = ai_roSource.GetValueNames( ai_pccKey, lValues ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	itIterator = lValues.begin() ;

	while( itIterator != lValues.end() )
	{
		pccValueName = *itIterator ;

		// Se obtiene el atributo
		iResult = ai_roSource.GetValue( ai_pccKey, pccValueName, pcData ) ;

		if( iResult != M4_SUCCESS )
		{
			_ClearList( lValues ) ;
			return( M4_ERROR ) ;
		}

		iResult = ai_roDestination.SetValue( ai_pccKey, pccValueName, pcData ) ;
		delete [] pcData ;

		if( iResult != M4_SUCCESS )
		{
			_ClearList( lValues ) ;
			return( M4_ERROR ) ;
		}

		itIterator++ ;
	}
	_ClearList( lValues ) ;


	// Se obtienen las ramas
	iResult = ai_roSource.GetSubKeyNames( ai_pccKey, lKeys ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	itIterator = lKeys.begin() ;

	while( itIterator != lKeys.end() )
	{
		pccKeyName = *itIterator ;
		sprintf( acSubKey, "%s%s%s", ai_pccKey, M4_BACK_SLASH, pccKeyName ) ;

		// Se procesa recursivamente
		iResult = _MergeKey( acSubKey, ai_roSource, ai_roDestination ) ;

		if( iResult != M4_SUCCESS )
		{
			_ClearList( lKeys ) ;
			return( M4_ERROR ) ;
		}

		itIterator++ ;
	}
	_ClearList( lKeys ) ;


	return( M4_SUCCESS ) ;
}


//=============================================================================
//
// M4XmlRegistryFile
//
// Clase que proporciona acceso a entradas de registro guardadas en un fichero xml 
//
//=============================================================================

// ============================================================================
// Almacena el path al fichero de registro en la clase.
// ============================================================================

m4return_t	M4XmlRegistryFile :: _SetFile( m4pcchar_t ai_pccFile )
{

	size_t		iLength = 0 ;
	m4char_t	acFile[ M4INI_MAX_PATH + 1 ] ;

	
	if( m_pcFile != NULL )
	{
		delete [] m_pcFile ;
	}

	if( ai_pccFile == NULL || *ai_pccFile == '\0' )
	{
		// Si no se pasa es el directorio actual y regmeta4.xml
		sprintf( acFile, "%s%s%s", ".", M4_BACK_SLASH, "regmeta4.xml" ) ;
		ai_pccFile = acFile ;
	}

	iLength = strlen( ai_pccFile ) + 1 ;
	m_pcFile = new m4char_t[ iLength ] ;
	M4INI_RETURN_ERROR1( m_pcFile == NULL, sizeof( m4char_t ) * iLength, "Cannot allocate <%d> bytes" ) ;
	memcpy( m_pcFile, ai_pccFile, iLength ) ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Inicializa la clase con un documento vacío.
// ============================================================================

M4XmlRegistryFile :: M4XmlRegistryFile( void )
{

	m4return_t	iResult = M4_ERROR ;

	
	m_bIsEmpty = M4_TRUE ;
	m_pcFile = NULL ;
	m_poRoot = NULL ;

	// Se establece el path por defecto
	iResult = _SetFile( NULL ) ;

	// Se crea un documento vacío con el elemento raiz
	iResult = _CreateEmptyDocument( m_oParser, m_poRoot ) ;
}



// ============================================================================
// Libera la memoria utilizada
// ============================================================================

M4XmlRegistryFile :: ~M4XmlRegistryFile( void )
{
	if( m_pcFile != NULL )
	{
		delete [] m_pcFile ;
		m_pcFile = NULL ;
	}
}



// ============================================================================
// Función que borra memoria para el m4adminworker que no puede borrar de otra librería
// ============================================================================

void	M4XmlRegistryFile :: Release( m4pchar_t& ai_rpcMemory )
{
	delete [] ai_rpcMemory ;
	ai_rpcMemory = NULL ;
}

// Instancias globales de ficheros de registro para las herramientas
M4XmlRegistryFile*	g_apoRegistryFile[M4INI_MAX_REG_FILES] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };


//=================================================================================
// Función que obtiene acceso a una de las instancias
//=================================================================================

M4XmlRegistryFile*	M4XmlRegistryFile :: GetRegistryFile(unsigned short ai_iHandle)
{
	if (ai_iHandle >= M4INI_MAX_REG_FILES)
	{
		return(NULL);
	}

	if (g_apoRegistryFile[ai_iHandle] == NULL)
	{
		g_apoRegistryFile[ai_iHandle] = new M4XmlRegistryFile;
	}

	return(g_apoRegistryFile[ai_iHandle]);
}

//=================================================================================
// Función que libera una de las instancias
//=================================================================================

m4bool_t M4XmlRegistryFile::FreeRegistryFile(unsigned short ai_iHandle)
{
	if (ai_iHandle < M4INI_MAX_REG_FILES)
	{
		if (g_apoRegistryFile[ai_iHandle] != NULL)
		{
			delete g_apoRegistryFile[ai_iHandle];
			g_apoRegistryFile[ai_iHandle] = NULL;
			return M4_TRUE;
		}
	}
	return M4_FALSE;
}


// ============================================================================
// Carga el documento a partir del fichero de registro de máquina
// ============================================================================

m4return_t	M4XmlRegistryFile :: LoadMachine( void )
{

	m4return_t	iResult = M4_ERROR ;
	m4pcchar_t	pccServer = NULL ;
	m4char_t	acFile[ M4INI_MAX_PATH + 1 ] ;


	// Si estamos en server se intenta del directorio actual
	pccServer = getenv( "M4SERVER_PATH" ) ;

	if( pccServer != NULL )
	{
		sprintf( acFile, "%s%s%s", ".", M4_BACK_SLASH, "regmeta4.xml" ) ;
		iResult = Load( acFile, M4_TRUE ) ;

		if( iResult != M4_SUCCESS )
		{
			// Se intenta encontrar el .reg
			sprintf( acFile, "%s%s%s", ".", M4_BACK_SLASH, "regmeta4.reg" ) ;
			iResult = Load( acFile, M4_FALSE ) ;
		}

		return( iResult ) ;
	}


#ifdef _WINDOWS

	BOOL				bResult = FALSE ;
	HRESULT				hResult = S_FALSE ;
	DWORD				dwResult = 0 ;
	int					iLength = -1 ;
	m4pchar_t			pcMachineDirectory = NULL ;
	M4XMLCh				awcDirectory[ M4INI_MAX_PATH + 1 ] ;
	IMalloc				*pShellMalloc = NULL ;
	LPITEMIDLIST		pIDL = NULL ;
	M4XmlRegistryFile	oCustomRegistryFile ;


	// En windows se intenta encontrar el fichero xml donde el ejecutable
	dwResult = GetModuleFileNameW( NULL, (LPWSTR)awcDirectory, M4INI_MAX_PATH ) ;

	if( dwResult > 0 )
	{
		bResult = PathRemoveFileSpecW( (LPWSTR)awcDirectory ) ;

		iLength = -1 ;
		pcMachineDirectory = M4Utf16ToCpp( awcDirectory, iLength ) ;
		sprintf( acFile, "%s%s%s", pcMachineDirectory, M4_BACK_SLASH, "regmeta4.xml" ) ;
		delete [] pcMachineDirectory ;

		iResult = Load( acFile, M4_TRUE ) ;

		if( iResult == M4_SUCCESS )
		{
			// Si existe el fichero custom al lado del ejecutable se hace el merge
			iLength = -1 ;
			pcMachineDirectory = M4Utf16ToCpp( awcDirectory, iLength ) ;
			sprintf( acFile, "%s%s%s", pcMachineDirectory, M4_BACK_SLASH, "regmeta4custom.xml" ) ;
			delete [] pcMachineDirectory ;

			iResult = oCustomRegistryFile.Load( acFile, M4_TRUE ) ;

			if( iResult == M4_SUCCESS )
			{
				iResult = Merge( oCustomRegistryFile ) ;
			}

			return( M4_SUCCESS ) ;
		}
	}

	// Si no está donde el ejecutable se intenta en el Common AppData
	hResult = SHGetSpecialFolderLocation( NULL, M4INI_CSIDL_COMMON_APPDATA, &pIDL ) ;
	M4INI_RETURN_ERROR0( hResult != S_OK || pIDL == NULL , "Cannot find Common AppData" ) ;

	bResult = SHGetPathFromIDListW( pIDL, (LPWSTR)awcDirectory ) ;

	SHGetMalloc( &pShellMalloc ) ;

	if( pShellMalloc != NULL )
	{
		pShellMalloc->Free( pIDL ) ;
	}
	M4INI_RETURN_ERROR0( bResult == FALSE, "Cannot find Common AppData" ) ;

	iLength = -1 ;
	pcMachineDirectory = M4Utf16ToCpp( awcDirectory, iLength ) ;
	sprintf( acFile, "%s%s%s%s%s", pcMachineDirectory, M4_BACK_SLASH, "Meta4", M4_BACK_SLASH, "regmeta4.xml" ) ;
	delete [] pcMachineDirectory ;

	iResult = Load( acFile, M4_TRUE ) ;

#endif

	return( iResult ) ;
}



// ============================================================================
// Carga el documento a partir de un fichero o del registro
// El fichero puede ser formato xml o formato reg.
// ============================================================================

m4return_t	M4XmlRegistryFile :: Load( m4pcchar_t ai_pccFile, m4bool_t ai_bIsXml )
{

	m4return_t	iResult = M4_ERROR ;
	FILE		*pfFile = NULL ;
	DOMNode		*poRoot = NULL ;


	m_bIsEmpty = M4_TRUE ;
	poRoot = NULL ;

	// Se establece el path
	iResult = _SetFile( ai_pccFile ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Si no se pasa fichero se lee del registro en windows
	if( ai_pccFile == NULL )
	{
		M4INI_DUMP_ERROR0( "Loading registry from Windows" ) ;

#ifdef _WINDOWS
		iResult = _LoadFromWindows( m_oParser, poRoot ) ;
#else
		// En UNIX da error
		iResult = M4_ERROR ;
#endif
	}
	else
	{
		// Se abre el fichero
		eUniFileEncodingType_t eEncoding = M4UniNative ;
		pfFile = M4Fopen( m_pcFile, M4UniReadBinary, eEncoding ) ;

		M4INI_DUMP_ERROR1( m_pcFile, "Loading registry from file <%s>" ) ;

		if( pfFile != NULL )
		{
			if( ai_bIsXml == M4_TRUE )
			{
				iResult = _LoadFromXml( m_pcFile, m_oParser, poRoot ) ;
			}
			else
			{
				iResult = _LoadFromReg( pfFile, m_oParser, poRoot ) ;
			}

			fclose( pfFile ) ;
		}
		else
		{
			iResult = M4_ERROR ;
		}
	}

	// Si no existe el fichero o no se ha podido parsear se crear un xml vacío
	if( iResult == M4_ERROR )
	{
		M4INI_DUMP_ERROR0( "*** ERROR loading xml" ) ;

		iResult = _CreateEmptyDocument( m_oParser, poRoot ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		m_poRoot = poRoot ;
		return( M4_ERROR ) ;
	}

	// Se ha cargado correctamente
	m_poRoot = poRoot ;
	m_bIsEmpty = M4_FALSE ;

	M4INI_DUMP_ERROR0( "Success loading xml" ) ;
	return( M4_SUCCESS ) ;
}


// ============================================================================
// Obtiene el valor de un atributo del documento.
// ============================================================================

m4return_t	M4XmlRegistryFile :: GetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pchar_t &ao_rpcData )
{
	// La xerces no es multithread. El mutex debe ir al principio
	ClMutBlock	oBlock( &g_oFileMutex ) ;

	m4return_t	iResult = M4_ERROR ;
	DOMNode		*poAttribute = NULL ;


	ao_rpcData = NULL ;

	M4INI_DUMP_TRACE3( ai_pccValue, ai_pccKey, m_pcFile, "Reading value <%s> of registry key <%s> from <%s>" ) ;

	if( m_bIsEmpty == M4_TRUE )
	{
		// Si está vacío no hay nada que encontrar
		return( M4_ERROR ) ;
	}

	// Se obtiene el atributo
	iResult = _GetAttribute( m_poRoot, ai_pccKey, ai_pccValue, M4_FALSE, poAttribute ) ;

	if( iResult != M4_SUCCESS )
	{
		M4INI_DUMP_ERROR2( ai_pccValue, ai_pccKey, "*** ERROR reading value <%s> of registry key <%s>" ) ;
		return( M4_ERROR ) ;
	}

	ao_rpcData = M4XMLToCpp( (M4XMLCh*) poAttribute->getNodeValue() ) ;

	if( ao_rpcData == NULL )
	{
		// Si viene a nulo es que es cadena vacía
		ao_rpcData = new m4char_t[ 1 ] ;
		M4INI_RETURN_ERROR1( ao_rpcData == NULL, sizeof( m4char_t ) * 1, "Cannot allocate <%d> bytes" ) ;
		*ao_rpcData = '\0' ;
	}

	M4INI_DUMP_TRACE1( ao_rpcData, "Success reading value <%s>" ) ;
	return( M4_SUCCESS ) ;
}



// ============================================================================
// Establece el valor de un atributo del documento.
// ============================================================================

m4return_t	M4XmlRegistryFile :: SetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pcchar_t ai_pccData )
{
	// La xerces no es multithread. El mutex debe ir al principio
	ClMutBlock	oBlock( &g_oFileMutex ) ;

	m4return_t	iResult = M4_ERROR ;
	M4XMLCh		*pcwData = NULL ;
	DOMNode		*poAttribute = NULL ;


	M4INI_DUMP_TRACE3( ai_pccValue, ai_pccKey, m_pcFile, "Writing value <%s> of registry key <%s> to <%s>" ) ;

	// Se obtiene el atributo
	iResult = _GetAttribute( m_poRoot, ai_pccKey, ai_pccValue, M4_TRUE, poAttribute ) ;

	if( iResult != M4_SUCCESS )
	{
		M4INI_DUMP_ERROR2( ai_pccValue, ai_pccKey, "*** ERROR writing value <%s> of registry key <%s>" ) ;
		return( M4_ERROR ) ;
	}

	pcwData = M4CppToXML( ai_pccData ) ;
	poAttribute->setNodeValue( (XMLCh*) pcwData ) ;
	delete [] pcwData ;

	// Si se añade algo ya no está vacío
	m_bIsEmpty = M4_FALSE ;

	M4INI_DUMP_TRACE0( "Success writing value" ) ;
	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene la lista de los nombres de los atributos de un elemento.
// ============================================================================

m4return_t	M4XmlRegistryFile :: GetValueNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlValues )
{
	// La xerces no es multithread. El mutex debe ir al principio
	ClMutBlock	oBlock( &g_oFileMutex ) ;

	m4return_t			iResult = M4_ERROR ;
	size_t				i = 0 ;
	size_t				iLength = 0 ;
	m4pchar_t			pcAttributeName = NULL ;
	DOMNode				*poElement = NULL ;
    DOMNode				*poAttribute = NULL ;
	DOMNamedNodeMap		*poAttributes = NULL ;


	// Se limpia la lista
	_ClearList( ao_rlValues ) ;

	if( m_bIsEmpty == M4_TRUE )
	{
		// Si está vacío no hay nada que encontrar
		return( M4_ERROR ) ;
	}

	// Se obtiene el elemento
	iResult = _GetElement( m_poRoot, ai_pccKey, M4_FALSE, poElement ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se itera por los atributos añadiéndolos a la lista
	poAttributes = poElement->getAttributes() ;
	iLength = poAttributes->getLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poAttribute = poAttributes->item( i ) ;

		pcAttributeName = M4XMLToCpp( (M4XMLCh*) poAttribute->getNodeName() ) ;
		ao_rlValues.push_back( pcAttributeName ) ;
	}
	
	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene la lista de los nombres de los elementos hijos de un elemento.
// ============================================================================

m4return_t	M4XmlRegistryFile :: GetSubKeyNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlSubKeys )
{
	// La xerces no es multithread. El mutex debe ir al principio
	ClMutBlock	oBlock( &g_oFileMutex ) ;

	m4return_t	iResult = M4_ERROR ;
	m4pchar_t	pcNodeName = NULL ;
	DOMNode		*poElement = NULL ;
	DOMNode		*poNextNode = NULL ;


	// Se limpia la lista
	_ClearList( ao_rlSubKeys ) ;

	if( m_bIsEmpty == M4_TRUE )
	{
		// Si está vacío no hay nada que encontrar
		return( M4_ERROR ) ;
	}

	// Se obtiene el elemento
	iResult = _GetElement( m_poRoot, ai_pccKey, M4_FALSE, poElement ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Se itera por los hijos añadiéndolos a la lista
	poNextNode = poElement->getFirstChild() ;
	while( poNextNode != NULL )
	{
		if( poNextNode->getNodeType() == DOMNode::ELEMENT_NODE )
		{
			pcNodeName = M4XMLToCpp( (M4XMLCh*) poNextNode->getNodeName() ) ;
			ao_rlSubKeys.push_back( pcNodeName ) ;
		}

		poNextNode = poNextNode->getNextSibling() ;
	}
	
	return( M4_SUCCESS ) ;
}



// ============================================================================
// Borra un atributo.
// ============================================================================

m4return_t	M4XmlRegistryFile :: DeleteValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue )
{
	// La xerces no es multithread. El mutex debe ir al principio
	ClMutBlock	oBlock( &g_oFileMutex ) ;

	m4return_t	iResult = M4_ERROR ;
	DOMNode		*poElement = NULL;
	DOMNode		*poAttribute = NULL ;


	// Se obtiene el elemento
	iResult = _GetElement( m_poRoot, ai_pccKey, M4_FALSE, poElement ) ;

	if( iResult != M4_SUCCESS )
	{
		// Si no existe ya no hace falta borrarlo
		return( M4_SUCCESS ) ;
	}

	// Se obtiene el atributo
	iResult = _GetAttribute( poElement, ai_pccValue, M4_FALSE, poAttribute ) ;

	if( iResult != M4_SUCCESS )
	{
		// Si no existe ya no hace falta borrarlo
		return( M4_SUCCESS ) ;
	}

	poElement->getAttributes()->removeNamedItem( poAttribute->getNodeName() ) ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Borra un elemento.
// ============================================================================

m4return_t	M4XmlRegistryFile :: DeleteKey( m4pcchar_t ai_pccKey )
{
	// La xerces no es multithread. El mutex debe ir al principio
	ClMutBlock	oBlock( &g_oFileMutex ) ;

	m4return_t	iResult = M4_ERROR ;
	DOMNode		*poElement = NULL ;


	// Se obtiene el elemento
	iResult = _GetElement( m_poRoot, ai_pccKey, M4_FALSE, poElement ) ;

	if( iResult != M4_SUCCESS )
	{
		// Si no existe ya no hace falta borrarlo
		return( M4_SUCCESS ) ;
	}

	poElement->getParentNode()->removeChild( poElement ) ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Añade el contenido de otro odumento a este.
// Llama a la función que lo hace recursivamente.
// ============================================================================

m4return_t	M4XmlRegistryFile :: Merge( M4XmlRegistryFile& ai_roSource )
{

	m4return_t	iResult = M4_ERROR ;

	
	iResult = _MergeKey( "SOFTWARE", ai_roSource, *this ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Salva el contenido del documento a un fichero.
// Se abre el fichero.
// Se imprimen recursivamente los elementos.
// Se cierra el fichero.
// ============================================================================

m4return_t	M4XmlRegistryFile :: Save( m4pcchar_t ai_pccFile ) const
{
	// La xerces no es multithread. El mutex debe ir al principio
	ClMutBlock	oBlock( &g_oFileMutex ) ;
	
	m4return_t	iResult = M4_ERROR ;
	int			iWrite = -1 ;
	int			iClose = -1 ;
	m4pchar_t	pcDot = NULL ;
	m4char_t	acFile[ M4INI_MAX_PATH + 1 ] ;
	FILE		*pfFile = NULL ;


	if( m_bIsEmpty == M4_TRUE )
	{
		// Si está vacío no hay nada que grabar
		return( M4_SUCCESS ) ;
	}

	if( ai_pccFile == NULL || *ai_pccFile == '\0' )
	{
		// Si no viene se toma el origen con extensión xml
		strcpy( acFile, m_pcFile ) ;
		pcDot = strrchr( acFile, '.' ) ;

		if( pcDot != NULL )
		{
			strcpy( pcDot + 1, "xml" ) ;
		}

		ai_pccFile = acFile ;
	}

	M4INI_DUMP_ERROR1( ai_pccFile, "Saving registry to file <%s>" ) ;

	/* Bug 0148459
	Se escribe la salida siempre en utf-8
	*/
	eUniFileEncodingType_t eEncoding = M4UniUtf8 ;
	pfFile = M4Fopen( ai_pccFile, M4UniWrite, eEncoding ) ;
	M4INI_RETURN_ERROR2( pfFile == NULL, ai_pccFile, strerror( errno ), "Cannot open file <%s> because <%s>" ) ;

	iWrite = fprintf( pfFile, M4Utf8Header() ) ;
	M4INI_RETURN_ERROR2( iWrite < 0, ai_pccFile, strerror( errno ), "Cannot write to file <%s> because <%s>" ) ;

	iResult = _PrintNode( pfFile, m_poRoot, 0, acFile ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iClose = fclose( pfFile ) ;

	return( M4_SUCCESS ) ;
}







//=============================================================================
//
// M4XmlRegistry
//
// Clase que proporciona el recubrimiento del registro desde ficheros xml 
//
//=============================================================================

M4XmlRegistry* M4XmlRegistry::ms_poInstance = NULL ;


// ============================================================================
// Devuelve la única instancia de esta clase
// Si no existe la construye
// ============================================================================

M4XmlRegistry*	M4XmlRegistry::Instance( void )
{
	// Instanciación segura
	ClMutBlock	oBlock( &g_oMutex ) ;

	if( ms_poInstance == NULL )
	{
		try
		{
      		XMLPlatformUtils::Initialize() ;
		}
		catch( const XMLException& )
		{
		}

		ms_poInstance = new M4XmlRegistry ;
	}

	return( ms_poInstance ) ;
}



// ============================================================================
// Libera la instancia de la clase
// ============================================================================

void M4XmlRegistry::FreeInstance( void )
{
	// Desinstanciación segura
	ClMutBlock	oBlock( &g_oMutex ) ;

	if( ms_poInstance != NULL )
	{
		delete( ms_poInstance ) ;
		ms_poInstance = NULL ;
	}
}



// ============================================================================
// Inicializa la clase
// En windows intenta encontrar el regmeta4.xml en el directorio de la aplicación
// y su correspondiente de usuario en el path dado por APPDATA
// ============================================================================

M4XmlRegistry :: M4XmlRegistry( void )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint32_t	iSize = 0 ;
	int			iLength = -1 ;
	int			iMkDir = -1 ;
	m4pchar_t	pcUnicode = NULL ;
	m4pcchar_t	pccServer = NULL ;
	m4pcchar_t	pccSide = NULL ;
	m4pcchar_t	pccAppData = NULL ;
	m4pchar_t	pcUserDirectory = NULL ;
	m4char_t	acPath[ M4INI_MAX_REG_VALUE_LEN + 1 ] ;
	m4char_t	acFile[ M4INI_MAX_PATH + 1 ] ;


	m_bMachineInit = M4_FALSE ;
	m_bUserInit = M4_FALSE ;

	// Se intenta cargar el fichero de máquina
	iResult = m_MachineRegistryFile.LoadMachine() ;

	if( iResult == M4_SUCCESS )
	{
		// El de máquina está inicializado sólo si se ha cargado de fichero
		m_bMachineInit = M4_TRUE ;
	}

	pccServer = getenv( "M4SERVER_PATH" ) ;

	// Se establece el modo unicode
	if( pccServer == NULL )
	{
		pccSide = M4_NEW_REGEDIT_KEY_CLIENT ;
	}
	else
	{
		pccSide = M4_NEW_REGEDIT_KEY_SERVER ;
	}

	iResult = M4ComposeRegistryPath( pccSide, M4_NEW_REGEDIT_KEY_CVM, acPath, M4INI_MAX_REG_VALUE_LEN, iSize ) ;

	if( iResult == M4_SUCCESS )
	{
		if( GetValue( acPath, M4_INIT_UNICODE, pcUnicode ) != M4_ERROR && pcUnicode != NULL )
		{
			if( strcmp( pcUnicode, "1" ) == 0 )
			{
				M4SetUnicode( 1 ) ;
			}
			else
			{
				M4SetUnicode( 0 ) ;
			}

			delete(	pcUnicode ) ;
		}
	}

	if( m_bMachineInit == M4_FALSE )
	{
		// Si no está se sale sin inicializar
		return ;
	}


	// Si estamos en cliente se lee el fichero de usuario en Document and Settings
	if( pccServer == NULL )
	{

#ifdef _WINDOWS

		BOOL		bResult = FALSE ;
		DWORD		dwResult = 0 ;
		m4pchar_t	pcCurrentDirectory = NULL ;
		M4XMLCh		awcDirectory[ M4INI_MAX_PATH + 1 ] ;

		// En windows se intenta encontrar el fichero xml donde el ejecutable
		dwResult = GetModuleFileNameW( NULL, (LPWSTR)awcDirectory, M4INI_MAX_PATH ) ;

		if( dwResult > 0 )
		{
			bResult = PathRemoveFileSpecW( (LPWSTR)awcDirectory ) ;

			iLength = -1 ;
			pcCurrentDirectory = M4Utf16ToCpp( awcDirectory, iLength ) ;
			sprintf( acFile, "%s%s%s", pcCurrentDirectory, M4_BACK_SLASH, "regmeta4user.xml" ) ;
			delete [] pcCurrentDirectory ;

			iResult = m_UserRegistryFile.Load( acFile, M4_TRUE ) ;

			if( iResult == M4_SUCCESS )
			{
				m_bUserInit = M4_TRUE;
				// Se hace el merge del de máquina con el del usuario
				iResult = m_MachineRegistryFile.Merge( m_UserRegistryFile ) ;
			}
		}
#endif

		if ( m_bUserInit == M4_FALSE )
		{
			pccAppData = getenv( "APPDATA" ) ;

			if( pccAppData != NULL )
			{
				iLength = -1 ;
				pcUserDirectory = M4ANSIToCpp( pccAppData, iLength ) ;
				sprintf( acFile, "%s%s%s%s%s", pcUserDirectory, M4_BACK_SLASH, "Meta4", M4_BACK_SLASH, "regmeta4user.xml" ) ;
				iResult = m_UserRegistryFile.Load( acFile, M4_TRUE ) ;

				// El de usuario está inicializado si se ha buscado, aunque no se ha cargado de fichero
				m_bUserInit = M4_TRUE ;

				if( iResult == M4_SUCCESS )
				{
					// Se hace el merge del de máquina con el del usuario
					iResult = m_MachineRegistryFile.Merge( m_UserRegistryFile ) ;
				}
				else
				{
					// Si no se ha cargado se intenta crear el directorio, por si acaso no existe
					sprintf( acFile, "%s%s%s", pcUserDirectory, M4_BACK_SLASH, "Meta4" ) ;
					iMkDir = M4MakeDirectory( acFile ) ;
				}
				delete( pcUserDirectory ) ;
			}
		}
	}
}



// ============================================================================
// No hace nada
// ============================================================================

M4XmlRegistry :: ~M4XmlRegistry( void )
{
}



// ============================================================================
// Obtiene el valor de un atributo del documento.
// ============================================================================

m4return_t	M4XmlRegistry :: GetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pchar_t &ao_rpcData )
{

	m4return_t	iResult = M4_ERROR ;


	ao_rpcData = NULL ;

	// Si el registro está en fichero se utiliza ese
	if( m_bMachineInit == M4_TRUE )
	{
		// Se lee de la máquina que tiene los valores fundidos
		iResult = m_MachineRegistryFile.GetValue( ai_pccKey, ai_pccValue, ao_rpcData ) ;
	}

	/* Bug 0146436
	Si no se encuentra la entrada en fichero se intenta en el registro, por compatibilidad
	*/
	if( iResult != M4_SUCCESS )
	{
#ifdef _WINDOWS
		// Se lee del registro
		iResult = _RegistryGetValue( ai_pccKey, ai_pccValue, ao_rpcData ) ;
#else
		// En UNIX da error
		iResult = M4_ERROR ;
#endif
	}

	return( iResult ) ;
}



// ============================================================================
// Establece el valor de un atributo del documento.
// ============================================================================

m4return_t	M4XmlRegistry :: SetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pcchar_t ai_pccData )
{

	m4return_t	iResult = M4_ERROR ;


	// Si el registro está en fichero se utiliza ese
	if( m_bMachineInit == M4_TRUE )
	{
		iResult = m_MachineRegistryFile.SetValue( ai_pccKey, ai_pccValue, ai_pccData ) ;

		if( iResult == M4_SUCCESS )
		{
			// Si hay de usuario se utiliza
			if( m_bUserInit == M4_TRUE )
			{
				iResult = m_UserRegistryFile.SetValue( ai_pccKey, ai_pccValue, ai_pccData ) ;
			}
		}
	}
	else
	{
#ifdef _WINDOWS
		// Se escribe en el registro
		iResult = _RegistrySetValue( ai_pccKey, ai_pccValue, ai_pccData ) ;
#else
		// En UNIX da error
		iResult = M4_ERROR ;
#endif
	}

	return( iResult ) ;
}



// ============================================================================
// Obtiene la lista de los nombres de los atributos de un elemento.
// ============================================================================

m4return_t	M4XmlRegistry :: GetValueNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlValues )
{

	m4return_t	iResult = M4_ERROR ;


	// Se limpia la lista
	_ClearList( ao_rlValues ) ;

	// Si el registro está en fichero se utiliza ese
	if( m_bMachineInit == M4_TRUE )
	{
		// Se leen atributos de la máquina
		iResult = m_MachineRegistryFile.GetValueNames( ai_pccKey, ao_rlValues ) ;
	}
	else
	{
#ifdef _WINDOWS
		// Se lee del registro
		iResult = _RegistryGetValueNames( ai_pccKey, ao_rlValues ) ;
#else
		// En UNIX da error
		iResult = M4_ERROR ;
#endif
	}

	return( iResult ) ;
}



// ============================================================================
// Obtiene la lista de los nombres de los elementos hijos de un elemento.
// ============================================================================

m4return_t	M4XmlRegistry :: GetSubKeyNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlSubKeys )
{

	m4return_t	iResult = M4_ERROR ;


	// Se limpia la lista
	_ClearList( ao_rlSubKeys ) ;


	// Si el registro está en fichero se utiliza ese
	if( m_bMachineInit == M4_TRUE )
	{
		// Se leen claves de la máquina
		iResult = m_MachineRegistryFile.GetSubKeyNames( ai_pccKey, ao_rlSubKeys ) ;
	}
	else
	{
#ifdef _WINDOWS
		// Se lee del registro
		iResult = _RegistryGetSubKeyNames( ai_pccKey, ao_rlSubKeys ) ;
#else
		// En UNIX da error
		iResult = M4_ERROR ;
#endif
	}

	return( iResult ) ;
}



// ============================================================================
// Borra un atributo.
// ============================================================================
m4return_t	M4XmlRegistry :: DeleteValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue )
{
	m4return_t	iResult = M4_ERROR ;


	// Si el registro está en fichero se utiliza ese
	if( m_bMachineInit == M4_TRUE )
	{
		// Se borra en los dos registros
		iResult = m_MachineRegistryFile.DeleteValue( ai_pccKey, ai_pccValue ) ;

		if( iResult == M4_SUCCESS )
		{
			// Si hay de usuario se utiliza
			if( m_bUserInit == M4_TRUE )
			{
				iResult = m_UserRegistryFile.DeleteValue( ai_pccKey, ai_pccValue ) ;
			}
		}
	}
	else
	{
#ifdef _WINDOWS
		// Se borra en el registro
		iResult = _RegistryDeleteValue( ai_pccKey, ai_pccValue ) ;
#else
		// En UNIX da error
		iResult = M4_ERROR ;
#endif
	}

	return( iResult ) ;
}



// ============================================================================
// Borra un elemento.
// ============================================================================
m4return_t	M4XmlRegistry :: DeleteKey( m4pcchar_t ai_pccKey )
{
	m4return_t	iResult = M4_ERROR ;


	// Si el registro está en fichero se utiliza ese
	if( m_bMachineInit == M4_TRUE )
	{
		// Se borra en los dos registros
		iResult = m_MachineRegistryFile.DeleteKey( ai_pccKey ) ;

		if( iResult == M4_SUCCESS )
		{
			// Si hay de usuario se utiliza
			if( m_bUserInit == M4_TRUE )
			{
				iResult = m_UserRegistryFile.DeleteKey( ai_pccKey ) ;
			}
		}
	}
	else
	{
#ifdef _WINDOWS
		// Se borra en el registro
		iResult = _RegistryDeleteKey( ai_pccKey ) ;
#else
		// En UNIX da error
		iResult = M4_ERROR ;
#endif
	}

	return( iResult ) ;
}



// ============================================================================
// Salva el contenido del registro a fichero.
// ============================================================================

m4return_t	M4XmlRegistry :: Save( m4pcchar_t ai_pccFile ) const
{

	m4return_t	iResult = M4_ERROR ;


	// Si el registro está en fichero se utiliza ese
	if( m_bUserInit == M4_TRUE )
	{
		// Sólo se salva la parte de usuario
		iResult = m_UserRegistryFile.Save( ai_pccFile ) ;
	}
	else
	{
		// En caso de leer del registro el salvar no hace nada
		iResult = M4_SUCCESS ;
	}

	return( iResult ) ;
}





#ifdef _WINDOWS


//=================================================================================
// Funciones para el manejo del registro
//=================================================================================


BYTE WINAPI M4UnLoadRegistry( void )
{
	M4XmlRegistry::FreeInstance() ;
	return( M4_SUCCESS ) ;
}



//=================================================================================
// Funciones para las herramientas en VB
//=================================================================================


// ============================================================================
// Carga el documento a partir del fichero de registro de máquina
// ============================================================================

BYTE WINAPI M4LoadMachineRegistry( unsigned short ai_iHandle )
{

	m4return_t	iResult = M4_ERROR ;


	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->LoadMachine() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Carga el documento a partir de un fichero o del registro
// ============================================================================

BYTE WINAPI M4LoadRegistry( unsigned short ai_iHandle, const BSTR ai_pccFile, VARIANT_BOOL ai_bIsXml )
{

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bIsXML = M4_FALSE ;
	int			iLength = -1 ;
	m4pchar_t	pcFile = NULL ;


	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	if( ai_pccFile != NULL )
	{
		iLength = -1 ;
		pcFile = M4ANSIToCpp( (m4pcchar_t) ai_pccFile, iLength ) ;
	}

	if( ai_bIsXml != VARIANT_FALSE )
	{
		bIsXML = M4_TRUE ;
	}
	else
	{
		bIsXML = M4_FALSE ;
	}

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->Load( pcFile, bIsXML ) ;

	delete [] pcFile ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene el valor de un atributo del documento.
// ============================================================================

BYTE WINAPI M4GetRegistryValue( unsigned short ai_iHandle, const BSTR ai_pccKey, const BSTR ai_pccValue, BSTR *ao_pcData )
{

	m4return_t	iResult = M4_ERROR ;
	int			iLength = -1 ;
	m4pchar_t	pcKey = NULL ;
	m4pchar_t	pcValue = NULL ;
	m4pchar_t	pcData = NULL ;
	m4pchar_t	pcDataANSI = NULL ;


	if( *ao_pcData != NULL )
	{
		SysFreeString( *ao_pcData ) ;
		*ao_pcData = NULL ;
	}

	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iLength = -1 ;
	pcKey = M4ANSIToCpp( (m4pcchar_t) ai_pccKey, iLength ) ;

	iLength = -1 ;
	pcValue = M4ANSIToCpp( (m4pcchar_t) ai_pccValue, iLength ) ;

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->GetValue( pcKey, pcValue, pcData ) ;

	delete [] pcKey ;
	delete [] pcValue ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iLength = -1 ;
	pcDataANSI = M4CppToANSI( pcData, iLength ) ;
	delete [] pcData ;

	*ao_pcData = SysAllocStringByteLen( pcDataANSI, iLength ) ;
	delete [] pcDataANSI ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Establece el valor de un atributo del documento.
// ============================================================================

BYTE WINAPI M4SetRegistryValue( unsigned short ai_iHandle, const BSTR ai_pccKey, const BSTR ai_pccValue, const BSTR ai_pccData )
{

	m4return_t	iResult = M4_ERROR ;
	int			iLength = -1 ;
	m4pchar_t	pcKey = NULL ;
	m4pchar_t	pcValue = NULL ;
	m4pchar_t	pcData = NULL ;


	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iLength = -1 ;
	pcKey = M4ANSIToCpp( (m4pcchar_t) ai_pccKey, iLength ) ;

	iLength = -1 ;
	pcValue = M4ANSIToCpp( (m4pcchar_t) ai_pccValue, iLength ) ;

	iLength = -1 ;
	pcData = M4ANSIToCpp( (m4pcchar_t) ai_pccData, iLength ) ;

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->SetValue( pcKey, pcValue, pcData ) ;

	delete [] pcKey ;
	delete [] pcValue ;
	delete [] pcData ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene la lista de los nombres de los atributos
// o de los elementos hijos de un elemento según un parámetro.
// ============================================================================

static	BYTE	_M4GetRegistryElementNames( unsigned short ai_iHandle, const BSTR ai_pccKey, SAFEARRAY **ao_psaElements, m4bool_t ai_bValue )
{

	m4return_t		iResult = M4_ERROR ;
	HRESULT			hResult = S_OK ;
	int				iLength = -1 ;
	size_t			i = 0 ;
	m4pchar_t		pcKey = NULL ;
	m4pchar_t		pccName = NULL ;
	m4pchar_t		pccNameANSI = NULL ;
	BSTR			*pcElements = NULL ;
	list<m4pchar_t>	oElements ;
	list<m4pchar_t>::iterator	itIterator ;


	if( *ao_psaElements != NULL )
	{
		SafeArrayDestroy( *ao_psaElements ) ;
		*ao_psaElements = NULL ;
	}

	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iLength = -1 ;
	pcKey = M4ANSIToCpp( (m4pcchar_t) ai_pccKey, iLength ) ;

	if( ai_bValue == M4_TRUE )
	{
		iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->GetValueNames( pcKey, oElements ) ;
	}
	else
	{
		iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->GetSubKeyNames( pcKey, oElements ) ;
	}

	delete [] pcKey ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iLength = (int) oElements.size() ;

	*ao_psaElements = SafeArrayCreateVector( VT_BSTR, 0, iLength ) ;
	M4INI_RETURN_ERROR1( *ao_psaElements == NULL, iLength, "Cannot create SafeArray of size <%d>" ) ;

	hResult = SafeArrayAccessData( *ao_psaElements, (void**) &pcElements ) ;

	if( hResult != S_OK )
	{
		M4INI_DUMP_ERROR0( "Cannot access SafeArray" ) ;
		_ClearList( oElements ) ;
		SafeArrayDestroy( *ao_psaElements ) ;
		*ao_psaElements = NULL ;
		return( M4_ERROR ) ;
	}

	i = 0 ;
	itIterator = oElements.begin() ;

	while( itIterator != oElements.end() )
	{
		pccName = *itIterator ;

		iLength = -1 ;
		pccNameANSI = M4CppToANSI( pccName, iLength ) ; 
		pcElements[ i++ ] = SysAllocStringByteLen( pccNameANSI, iLength ) ;
		delete [] pccNameANSI ;

		itIterator++ ;
	}
	_ClearList( oElements ) ;

	hResult = SafeArrayUnaccessData( *ao_psaElements ) ;

	if( hResult != S_OK )
	{
		M4INI_DUMP_ERROR0( "Cannot unaccess SafeArray" ) ;
		SafeArrayDestroy( *ao_psaElements ) ;
		*ao_psaElements = NULL ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene la lista de los nombres de los atributos de un elemento.
// ============================================================================

BYTE WINAPI M4GetRegistryValueNames( unsigned short ai_iHandle, const BSTR ai_pccKey, SAFEARRAY **ao_psaValues )
{
	return( _M4GetRegistryElementNames( ai_iHandle, ai_pccKey, ao_psaValues, M4_TRUE ) ) ;
}



// ============================================================================
// Obtiene la lista de los nombres de los elementos hijos de un elemento.
// ============================================================================

BYTE WINAPI M4GetRegistrySubKeyNames( unsigned short ai_iHandle, const BSTR ai_pccKey, SAFEARRAY **ao_psaKeys )
{
	return( _M4GetRegistryElementNames( ai_iHandle, ai_pccKey, ao_psaKeys, M4_FALSE ) ) ;
}



// ============================================================================
// Borra un atributo.
// ============================================================================

BYTE WINAPI M4DeleteRegistryValue( unsigned short ai_iHandle, const BSTR ai_pccKey, const BSTR ai_pccValue )
{

	m4return_t	iResult = M4_ERROR ;
	int			iLength = -1 ;
	m4pchar_t	pcKey = NULL ;
	m4pchar_t	pcValue = NULL ;


	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iLength = -1 ;
	pcKey = M4ANSIToCpp( (m4pcchar_t) ai_pccKey, iLength ) ;

	iLength = -1 ;
	pcValue = M4ANSIToCpp( (m4pcchar_t) ai_pccValue, iLength ) ;

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->DeleteValue( pcKey, pcValue ) ;

	delete [] pcKey ;
	delete [] pcValue ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Borra un elemento.
// ============================================================================

BYTE WINAPI M4DeleteRegistryKey( unsigned short ai_iHandle, const BSTR ai_pccKey )
{

	m4return_t	iResult = M4_ERROR ;
	int			iLength = -1 ;
	m4pchar_t	pcKey = NULL ;


	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iLength = -1 ;
	pcKey = M4ANSIToCpp( (m4pcchar_t) ai_pccKey, iLength ) ;

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->DeleteKey( pcKey ) ;

	delete [] pcKey ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Mezcla el contenido de dos registros.
// ============================================================================

BYTE WINAPI M4MergeRegistry( unsigned short ai_iSourceHandle, unsigned short ai_iDestinationHandle )
{

	m4return_t	iResult = M4_ERROR ;


	M4INI_RETURN_ERROR2( ai_iSourceHandle >= M4INI_MAX_REG_FILES, ai_iSourceHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;
	M4INI_RETURN_ERROR2( ai_iDestinationHandle >= M4INI_MAX_REG_FILES, ai_iDestinationHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iSourceHandle )->Merge( *M4XmlRegistryFile::GetRegistryFile( ai_iDestinationHandle ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Salva el contenido del registro a fichero.
// ============================================================================

BYTE WINAPI M4SaveRegistry( unsigned short ai_iHandle, const BSTR ai_pccFile )
{

	m4return_t	iResult = M4_ERROR ;
	int			iLength = -1 ;
	m4pchar_t	pcFile = NULL ;


	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	if( ai_pccFile != NULL )
	{
		iLength = -1 ;
		pcFile = M4ANSIToCpp( (m4pcchar_t) ai_pccFile, iLength ) ;
	}

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->Save( pcFile ) ;

	delete [] pcFile ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


//=================================================================================
// Funciones para las herramientas en install script
//=================================================================================


// ============================================================================
// Obtiene el valor de un atributo del documento.
// ============================================================================

BYTE WINAPI M4WIGetRegistryValue( unsigned short ai_iHandle, const BSTR ai_pccKey, const BSTR ai_pccValue, BSTR ao_pcData, short *aio_piSize )
{

	m4return_t	iResult = M4_ERROR ;
	short		iSize = -1 ;
	int			iLength = -1 ;
	m4pchar_t	pcKey = NULL ;
	m4pchar_t	pcValue = NULL ;
	m4pchar_t	pcData = NULL ;
	m4pchar_t	pcDataANSI = NULL ;


	iSize = *aio_piSize ;
	*aio_piSize = 0 ;
	*ao_pcData = '\0' ;

	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iLength = -1 ;
	pcKey = M4ANSIToCpp( (m4pcchar_t) ai_pccKey, iLength ) ;

	iLength = -1 ;
	pcValue = M4ANSIToCpp( (m4pcchar_t) ai_pccValue, iLength ) ;

	iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->GetValue( pcKey, pcValue, pcData ) ;

	delete [] pcKey ;
	delete [] pcValue ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iLength = -1 ;
	pcDataANSI = M4CppToANSI( pcData, iLength ) ;
	delete [] pcData ;

	if( iLength > iSize )
	{
		iLength = iSize ;
	}

	*aio_piSize = iLength ;
	memcpy( ao_pcData, pcDataANSI, iLength + 1 ) ;
	delete [] pcDataANSI ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene el número de atributos o de elementos hijos de un elemento del documento
// según un parámetro.
// ============================================================================

static	BYTE	_M4WIGetRegistryElementCount( unsigned short ai_iHandle, const BSTR ai_pccKey, short *ao_piCount, m4bool_t ai_bValue )
{

	m4return_t		iResult = M4_ERROR ;
	int				iLength = -1 ;
	m4pchar_t		pcKey = NULL ;
	list<m4pchar_t>	oElements ;


	*ao_piCount = 0 ;

	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iLength = -1 ;
	pcKey = M4ANSIToCpp( (m4pcchar_t) ai_pccKey, iLength ) ;

	if( ai_bValue == M4_TRUE )
	{
		iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->GetValueNames( pcKey, oElements ) ;
	}
	else
	{
		iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->GetSubKeyNames( pcKey, oElements ) ;
	}

	delete [] pcKey ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	*ao_piCount = (short) oElements.size() ;
	_ClearList( oElements ) ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene el número de atributos de un elemento del documento
// ============================================================================

BYTE WINAPI M4WIGetRegistryValueCount( unsigned short ai_iHandle, const BSTR ai_pccKey, short *ao_piCount )
{
	return( _M4WIGetRegistryElementCount( ai_iHandle, ai_pccKey, ao_piCount, M4_TRUE ) ) ;
}



// ============================================================================
// Obtiene el número de elementos hijos de un elemento del documento
// ============================================================================

BYTE WINAPI M4WIGetRegistrySubKeyCount( unsigned short ai_iHandle, const BSTR ai_pccKey, short *ao_piCount )
{
	return( _M4WIGetRegistryElementCount( ai_iHandle, ai_pccKey, ao_piCount, M4_FALSE ) ) ;
}



// ============================================================================
// Obtiene el nombre de un atributo o de un elemento hijo
// de un elemento del documento según un parámetro.
// ============================================================================

static	BYTE	_M4WIGetRegistryElementName( unsigned short ai_iHandle, const BSTR ai_pccKey, short ai_iPosition, BSTR ao_pcName, short *aio_piSize, m4bool_t ai_bValue )
{

	m4return_t		iResult = M4_ERROR ;
	short			iSize = -1 ;
	size_t			i = 0 ;
	int				iLength = -1 ;
	m4pchar_t		pcKey = NULL ;
	m4pchar_t		pcName = NULL ;
	m4pchar_t		pcNameANSI = NULL ;
	list<m4pchar_t>	oElements ;
	list<m4pchar_t>::iterator	itIterator ;


	iSize = *aio_piSize ;
	*aio_piSize = 0 ;
	*ao_pcName = '\0' ;

	M4INI_RETURN_ERROR2( ai_iHandle >= M4INI_MAX_REG_FILES, ai_iHandle, M4INI_MAX_REG_FILES - 1, "Invalid Handle <%d>. Handle can range from <0> to <%d>" ) ;

	iLength = -1 ;
	pcKey = M4ANSIToCpp( (m4pcchar_t) ai_pccKey, iLength ) ;

	if( ai_bValue == M4_TRUE )
	{
		iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->GetValueNames( pcKey, oElements ) ;
	}
	else
	{
		iResult = M4XmlRegistryFile::GetRegistryFile( ai_iHandle )->GetSubKeyNames( pcKey, oElements ) ;
	}

	delete [] pcKey ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iLength = (int) oElements.size() ;

	M4INI_RETURN_ERROR2( ai_iPosition >= iLength, ai_iPosition, iLength - 1, "Invalid Position <%d>. Position can range from <0> to <%d>" ) ;

	itIterator = oElements.begin() ;
	for( i = 0 ; i < ai_iPosition ; i++ )
	{
		itIterator++ ;
	}

	pcName = *itIterator ;

	iLength = -1 ;
	pcNameANSI = M4CppToANSI( pcName, iLength ) ; 
	_ClearList( oElements ) ;

	if( iLength > iSize )
	{
		iLength = iSize ;
	}

	*aio_piSize = iLength ;
	memcpy( ao_pcName, pcNameANSI, iLength + 1 ) ;
	delete [] pcNameANSI ;

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Obtiene el nombre de un atributo de un elemento del documento.
// ============================================================================

BYTE WINAPI M4WIGetRegistryValueName( unsigned short ai_iHandle, const BSTR ai_pccKey, short ai_iPosition, BSTR ao_pcName, short *aio_piSize )
{
	return( _M4WIGetRegistryElementName( ai_iHandle, ai_pccKey, ai_iPosition, ao_pcName, aio_piSize, M4_TRUE ) ) ;
}



// ============================================================================
// Obtiene el nombre de un elemento hijo de un elemento del documento.
// ============================================================================

BYTE WINAPI M4WIGetRegistrySubKeyName( unsigned short ai_iHandle, const BSTR ai_pccKey, short ai_iPosition, BSTR ao_pcName, short *aio_piSize )
{
	return( _M4WIGetRegistryElementName( ai_iHandle, ai_pccKey, ai_iPosition, ao_pcName, aio_piSize, M4_FALSE ) ) ;
}

#endif




//=================================================================================
// Funciones para programas C (rpcspy)
//=================================================================================

int	M4CGetRegistryValue( const char* ai_pccKey, const char* ai_pccValue, char* ao_pcData, size_t ai_iSize, size_t* ao_piLength )
{

	m4return_t	iResult = M4_ERROR ;
	size_t		iLength = 0 ;
	m4pchar_t	pcData = NULL ;


	if( ao_pcData == NULL )
	{
		return( -1 ) ;
	}

	*ao_pcData = '\0' ;

	if( ao_piLength != NULL )
	{
		*ao_piLength = 0 ;
	}

	iResult = M4XmlRegistry::Instance()->GetValue( ai_pccKey, ai_pccValue, pcData ) ;

	if( iResult != M4_SUCCESS )
	{
		return( -1 ) ;
	}

	iLength = strlen( pcData ) ;

	if( iLength > ai_iSize - 1 )
	{
		iLength = ai_iSize - 1 ;
	}

	memcpy( ao_pcData, pcData, iLength ) ;
	ao_pcData[ iLength ] = '\0' ;
	delete [] pcData ;

	if( ao_piLength != NULL )
	{
		*ao_piLength = iLength ;
	}

	return( 0 ) ;
}
