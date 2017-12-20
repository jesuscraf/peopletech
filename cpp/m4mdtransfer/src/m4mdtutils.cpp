
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtutils.cpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                04-05-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene funciones globales de utilidad general
//
//
//==============================================================================


#ifdef _WINDOWS
#include <windows.h>
#else
#endif // _WINDOWS


#include "m4mdtutils.hpp"
#include "m4mdtdef.hpp"
#include "m4mdtres.hpp"
#include "m4objglb.hpp"
#include "m4unicode.hpp"

#include "xercesc/dom/DOMNode.hpp"
#include "xercesc/sax/AttributeList.hpp"
#include "xercesc/dom/DOMNodeList.hpp"
#include "xercesc/dom/DOMNamedNodeMap.hpp"
#include "xercesc/util/XMLString.hpp"





//=================================================================================
//
// Busca un atributo dentro de una lista de atributos
// Devuelve un contenedor de caracteres
//
// Los pasos son
//		Se obtiene el atributo
//		Si no existe se pone la variable de salida a nulo y se retorna error
//		Se convierte a cadena de caracteres
//
//=================================================================================

m4return_t	M4MDT_DOMGetAttribute( const XMLCh* const ai_pcsName, const AttributeList &ai_roAttributes, ClCharHolder &ao_rpcValue )
{

	const XMLCh	*pccValue = NULL ;


	pccValue = ai_roAttributes.getValue( ai_pcsName ) ;

	if( pccValue == NULL )
	{
		ao_rpcValue.SetNull() ;
		return( M4_ERROR ) ;
	}

	ao_rpcValue.Set( pccValue ) ;
	
	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Busca un atributo dentro de una lista de atributos
// Devuelve un string
//
// Los pasos son
//		Se obtiene el atributo
//		Si no existe se pone la variable de salida a vacío y se retorna error
//		Se almacena en el string de salida
//
//=================================================================================

m4return_t	M4MDT_DOMGetAttribute( const XMLCh* const ai_pcsName, const AttributeList &ai_roAttributes, string &ao_rsValue )
{

	m4return_t		iResult = M4_ERROR ;
	ClCharHolder	pcAttribute ;
	const XMLCh		*pccValue = NULL ;


	iResult = M4MDT_DOMGetAttribute( ai_pcsName, ai_roAttributes, pcAttribute ) ;

	if( iResult != M4_SUCCESS )
	{
		ao_rsValue = "" ;
		return( M4_ERROR ) ;
	}

	ao_rsValue = pcAttribute ;
	
	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Busca un atributo dentro de un nodo DOM
// Devuelve un contenedor de caracteres
//
// Los pasos son
//		Se obtiene el atributo dentro de la lista
//		Si no existe se pone el puntero de salida a nulo y se retorna error
//		Se convierte a cadena de caracteres
//
//=================================================================================

m4return_t	M4MDT_DOMGetAttribute( const XMLCh* const ai_pcsName, const DOMNode* ai_pcoNode, ClCharHolder &ao_rpcValue )
{

	DOMNode	*poAttribute = NULL ;


  	poAttribute = ai_pcoNode->getAttributes()->getNamedItem( ai_pcsName ) ;

	if( poAttribute == NULL )
	{
		ao_rpcValue.SetNull() ;
		return( M4_ERROR ) ;
	}

	ao_rpcValue.Set( poAttribute->getNodeValue() ) ;
	
	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Busca un atributo dentro de un nodo DOM
// Devuelve un string
//
// Los pasos son
//		Se obtiene el atributo dentro de la lista
//		Si no existe se pone la variable de salida a vacío y se retorna error
//		Se almacena en el string de salida
//
//=================================================================================

m4return_t	M4MDT_DOMGetAttribute( const XMLCh* const ai_pcsName, const DOMNode* ai_pcoNode, string &ao_rsValue )
{

	m4return_t		iResult = M4_ERROR ;
	ClCharHolder	pcAttribute ;


	iResult = M4MDT_DOMGetAttribute( ai_pcsName, ai_pcoNode, pcAttribute ) ;

	if( iResult != M4_SUCCESS )
	{
		ao_rsValue = "" ;
		return( M4_ERROR ) ;
	}

	ao_rsValue = pcAttribute ;
	
	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Busca un nodo DOM dentro de otro nodo DOM
//
// Los pasos son
//		Se obtiene la lista de nodos hijos
//		Se itera por la lista de hijos a partir de la posición indicada
//		Se procesan los hijos que no sean nulos y que sean de tipo elemento
//		Se obtinene el nombre del hijo y se procesa si coincide con el pedido
//		Si no se encuentra se retorna -1 y el nodo a null
//		Si se encuentra se devuelve su posición
//
//=================================================================================

unsigned int	M4MDT_DOMFindChildNode( const XMLCh* const ai_pcsName, unsigned int ai_iElement, const DOMNode* ai_pcoNode, DOMNode* &ao_rpoChildNode )
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bFound = M4_FALSE ;
	short				iType = 0 ;
	unsigned int		i = 0 ;
	unsigned int		iLength = 0 ;
	DOMNode				*poChild = NULL ;
	DOMNodeList			*poList = NULL ;


	poList = ai_pcoNode->getChildNodes() ;
	iLength = poList->getLength() ;

	bFound = M4_FALSE ;

	for( i = ai_iElement ; i < iLength && bFound == M4_FALSE ; i++ )
	{
		ao_rpoChildNode = poList->item( i ) ;

		if( ao_rpoChildNode == NULL )
		{
			continue ;
		}

		iType = ao_rpoChildNode->getNodeType() ;

		if( iType != DOMNode::ELEMENT_NODE )
		{
			continue ;
		}

		if( XMLString::compareString( ao_rpoChildNode->getNodeName(), ai_pcsName ) == 0 )
		{
			bFound = M4_TRUE ;
		}
	}

	if( bFound == M4_FALSE )
	{
		ao_rpoChildNode = NULL ;
		return( -1 ) ;
	}

	return( i - 1 ) ;
}




//=================================================================================
//
// ClMDTDomTraverser
//
//=================================================================================


//=================================================================================
// Funciones de inicialización
//=================================================================================


//=================================================================================
//
// Constructor
//
// De momento no hace nada
//
//=================================================================================

ClMDTDomTraverser::ClMDTDomTraverser( void )
{
}



//=================================================================================
//
// Destructor
//
// De momento no hace nada
//
//=================================================================================

ClMDTDomTraverser::~ClMDTDomTraverser( void )
{
}



//=================================================================================
//
// Rellena el mapa a partir de un nodo DOM y un nombre de atributo
//
// Pasos
//		Limpia las entradas del mapa
//		Se itera por los hijos del nodo que se pasa
//		Se ignoran los hijos nulos o que no sean de tipo nodo
//		Si se encuentra un hijo cuyo nombre coincide con el pasado y tiene el
//			atributo pasado se inserta con la clave el valor del atributo
//		Si no existe el atributo se ignora y no se da error
//
//=================================================================================

m4return_t	ClMDTDomTraverser::Init( const XMLCh* const ai_pcsNodeName, const XMLCh* const ai_pcsAttributeName, const DOMNode* ai_pcoNode )
{

	m4return_t			iResult = M4_ERROR ;
	short				iType = 0 ;
	unsigned int		i = 0 ;
	unsigned int		iLength = 0 ;
	string				sValue ;
	DOMNode				*poChild = NULL ;
	DOMNodeList			*poList = NULL ;


	clear() ;

	poList = ai_pcoNode->getChildNodes() ;
	iLength = poList->getLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poChild = poList->item( i ) ;

		if( poChild == NULL )
		{
			continue ;
		}

		iType = poChild->getNodeType() ;

		if( iType != DOMNode::ELEMENT_NODE )
		{
			continue ;
		}

		if( XMLString::compareString( poChild->getNodeName(), ai_pcsNodeName ) == 0 )
		{
			iResult = M4MDT_DOMGetAttribute( ai_pcsAttributeName, poChild, sValue ) ;

			if( iResult == M4_SUCCESS )
			{
				insert(value_type(sValue, poChild )) ;
			}
		}
	}


	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Busca el primer nodo con un valor de atributo concreto
//
// Pasos
//		Hace un find en el mapa
//		Si no existe el elemento pone a null la variable de salida y retorna error
//		Si existe se retorna el primer elemento de la secuencia
//
//=================================================================================

m4return_t	ClMDTDomTraverser::FindChild( const string& ai_rcsAttributeValue, DOMNode* &ao_rpoNode ) const
{

	MDTDomNodeMap_t::const_iterator	iterator ;


	iterator = find( ai_rcsAttributeValue ) ;

	if( iterator == end() )
	{
		ao_rpoNode = NULL ;
		return( M4_ERROR ) ;
	}

	ao_rpoNode = (*iterator).second ;

	return( M4_SUCCESS ) ;
}







//=================================================================================
//
// ClCharHolder
//
//=================================================================================


//=================================================================================
// Funciones auxiliares
//=================================================================================

//=================================================================================
//
// Asigna el puntero a char a partir de otro puntero a char
//
//=================================================================================

void ClCharHolder::_Set( m4pcchar_t ai_pccValue )
{

	size_t	iLength = 0 ;


	if( ai_pccValue != NULL )
	{
		iLength = strlen( ai_pccValue ) + 1 ;

		m_pcValue = new m4char_t[ iLength ] ;

		if( m_pcValue != NULL )
		{
			memcpy( m_pcValue, ai_pccValue, iLength ) ;
		}
	}
	else
	{
		m_pcValue = NULL ;
	}
}


//=================================================================================
//
// Asigna el puntero a char a partir del puntero a XMLCh
//
//=================================================================================

void ClCharHolder::_Set( const XMLCh * const ai_pccValue )
{
	if( ai_pccValue != NULL )
	{
		// UNICODE XML
		m_pcValue = M4XMLToCpp( (M4XMLCh*)ai_pccValue ) ;
	}
	else
	{
		m_pcValue = NULL ;
	}
}



