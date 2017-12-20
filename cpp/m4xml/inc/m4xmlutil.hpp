//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4xml
// File:                m4xmlutil.hpp   
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
//    Este módulo contiene la declaración de las funciones de utilidad
//
//
//==============================================================================


#ifndef __M4XMLUTIL_HPP__
#define __M4XMLUTIL_HPP__



#include "stdafx.h"

#include "xercesc/util/XercesDefs.hpp"
#include "xercesc/sax/ErrorHandler.hpp"
#include "xercesc/sax/SAXParseException.hpp"


XERCES_CPP_NAMESPACE_USE




//=================================================================================
// Defines
//=================================================================================

#define M4RCX_RETURN_ERROR(cond, id, result, message)		do { if( cond ) { _formatError( id, NULL, message ) ; result = -1 ; return( S_OK ) ; } } while( 0 )





//=================================================================================
// Funciones auxiliares
//=================================================================================

BSTR	_DOMStringToBSTR( const XMLCh *ai_pcsString ) ;
void	_formatError( HRESULT ai_hResult, const XMLCh *ai_pcwcErrorText, BSTR *ao_psErrorMessage ) ;





//=================================================================================
// Clases auxiliares
//=================================================================================


//=================================================================================
//
// ClRCXErrorHandler
//
// Clase que gestiona los errores de parseo de la xerces. Funciona como un callback
// al que se avisa cuando ha habido un error. Para ello debe registrase con el
// parser
// Implementa el interfaz de ErrorHandler
//
//=================================================================================

class	ClRCXErrorHandler : public ErrorHandler
{

public:

//=================================================================================
// Funciones de inicialización
//=================================================================================

		//=================================================================================
		// Constructor
		// Inicializa los miembros a nulo
		//=================================================================================
		ClRCXErrorHandler( void )
		{
			m_lError = 0 ;
			m_pwcMessage = NULL ;
		}

		//=================================================================================
		// Destructor
		//=================================================================================
		virtual ~ClRCXErrorHandler( void )
		{
			resetErrors() ;
		}


//=================================================================================
// Funciones de lectura
//=================================================================================

	//=================================================================================
	// Retorna el error
	//=================================================================================
	LONG	GetError( void ) const
	{
		return( m_lError ) ;
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
			unsigned int	iLength = 0 ;
			const XMLCh*	pcwcException = NULL;

			resetErrors() ;

			m_lError = -1 ;
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
			m_lError = 0 ;
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
	LONG	m_lError ;

	//=================================================================================
	// Texto del posible error
	//=================================================================================
	XMLCh	*m_pwcMessage ;

} ;


#endif //__M4XMLUTIL_HPP__