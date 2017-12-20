//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4xml
// File:                m4xmlutil.cpp   
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
//    Este módulo contiene la implementación de las funciones de utilidad
//
//
//==============================================================================

#include "m4xmlutil.hpp"

#include "xercesc/util/PlatformUtils.hpp"





//=================================================================================
// Clases auxiliares
//=================================================================================

//=================================================================================
//
// ClRCXInitializer
//
// Clase que gestiona la inicialización única de las utilidades de xerces
//
//=================================================================================

class	ClRCXInitializer
{

public:

	//=================================================================================
	// Constructor
	// Inicializa las utilidades de xerces
	//=================================================================================
	ClRCXInitializer( void )
	{
		static	int	m_iInitialized = 0 ;

		if( m_iInitialized == 0 )
		{
			try
			{
      			XMLPlatformUtils::Initialize() ;
				m_iInitialized = 1 ;
			}
			catch( const XMLException& )
			{
			}
		}
	}
} ;




//=================================================================================
// Variables globales
//=================================================================================

// Inicializador de la xerces
static	ClRCXInitializer	s_oInitializer ;






//=================================================================================
// Funciones auxiliares
//=================================================================================




//=================================================================================
//
// Conviete una cadena XMLCh a BSTR
//
// Pasos
//		Si la cadena de entrada en nula retorna nulo
//		Obtiene la cadena BSTR a partir de la XMLCh
//		Retorna la cadena BSTR
//
//=================================================================================

BSTR	_DOMStringToBSTR( const XMLCh *ai_pcsString )
{

	BSTR			sResult = NULL ;
	unsigned int	iLength = 0 ;


	if( ai_pcsString == NULL )
	{
		return( NULL ) ;
	}

	// UNICODE XML
	iLength = XMLString::stringLen( ai_pcsString ) ;
	sResult = SysAllocStringLen( (OLECHAR *) ai_pcsString, iLength ) ;

	return( sResult ) ;
}



//=================================================================================
//
// Obtiene la cadena de error asociada a un número de error
//
// @param ai_hResult
//
//			Parámetro de entrada con el número del error
//
// @param ai_pcwcErrorText
//
//			Parámetro de entrada con la cadena de error en formato Unicode
//
// @param ao_psErrorMessage
//
//			Parámetro de salida con la cadena del texto del error en formato BSTR
//
// Pasos
//		Si el parámetro de salida no es vacío lo libera y lo deja vacío
//		Si se ha pasado una cadena de entrada se convierte a la de salida en
//			formato BSTR
//		Si no se pasa ya un mensaje de error se calcula a partir del handle de error
//		Se obtiene el texto del error con la función del sistema correspondiente
//		Si se ha obtenido el mensaje correctamente se copia en la variable de
//			salida y se libera la memoria que devuelve la función del sistema
//		Si no se ha obtenido el mensaje de error se da un mensaje genérico
//
//=================================================================================

void	_formatError( HRESULT ai_hResult, const XMLCh *ai_pcwcErrorText, BSTR *ao_psErrorMessage )
{

	DWORD	dwResult = 0 ;
	LPVOID	pvBuffer = NULL ;
	LPWSTR	psBuffer = NULL ;


	if( *ao_psErrorMessage != NULL )
	{
		SysFreeString( *ao_psErrorMessage ) ;
		*ao_psErrorMessage = NULL ;
	}

	if( ai_pcwcErrorText != NULL )
	{
		// UNICODE VB
		*ao_psErrorMessage = SysAllocString( (OLECHAR *) ai_pcwcErrorText ) ;
	}
	else
	{
		if( ai_hResult == -1 )
		{
			ai_hResult = GetLastError() ;
		}

		dwResult = FormatMessageW
			( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				ai_hResult,
				0,
				(LPWSTR) &pvBuffer,
				0,
				NULL 
			) ;

		if( dwResult > 0 && pvBuffer != NULL )
		{
			psBuffer = (LPWSTR) pvBuffer ;

			if( dwResult >= 2 && psBuffer[ dwResult - 2 ] == '\r' && psBuffer[ dwResult - 1 ] == '\n' )
			{
				dwResult -= 2 ;
				psBuffer[ dwResult ] = '\0' ;
			}

			// UNICODE VB
			*ao_psErrorMessage = SysAllocString( psBuffer ) ;
			LocalFree( pvBuffer ) ;
		}
		else
		{
			// UNICODE VB
			*ao_psErrorMessage = SysAllocString( L"No message available." ) ;
		}
	}
}

