
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtutils.hpp
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
//    Este módulo contiene la declaraciónd de las funciones globales
//    de utilidad general
//
//
//==============================================================================

#include "m4types.hpp"
#include "m4stl.hpp"

#include <xercesc/util/XercesDefs.hpp>


#ifndef __M4MDTUTILS_HPP__
#define __M4MDTUTILS_HPP__


//=================================================================================
// Declaraciones por adelantado
//=================================================================================
class ClCharHolder;
class ClMDTDomTraverser;

XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class DOMNode;
class AttributeList;
XERCES_CPP_NAMESPACE_END



//=================================================================================
// Defines
//=================================================================================

typedef multimap< string, DOMNode*, less<string> >			MDTDomNodeMap_t ;
typedef map< string, ClMDTDomTraverser *, less <string> >	MDTDOMTraverserMap_t;

//=================================================================================
//
// Busca un atributo dentro de una lista de atributos
//
// @param ai_pcsName
//
//		Nombre del atributo que se busca
//
// @param ai_roAttributes
//
//		Lista de atributos
//
// @param ao_rpcValue
//
//		Parámetro de salida con el valor del atributo o vacío si no existe
//
// @return
//
//		M4_SUCCESS si existe el atributo o M4_ERROR sino
//
//=================================================================================
m4return_t	M4MDT_DOMGetAttribute( const XMLCh* const ai_pcsName, const AttributeList &ai_roAttributes, ClCharHolder &ao_rpcValue ) ;



//=================================================================================
//
// Busca un atributo dentro de una lista de atributos
//
// @param ai_pcsName
//
//		Nombre del atributo que se busca
//
// @param ai_roAttributes
//
//		Lista de atributos
//
// @param ao_rsValue
//
//		Parámetro de salida con el valor del atributo o NULL si no existe
//
// @return
//
//		M4_SUCCESS si existe el atributo o M4_ERROR sino
//
//=================================================================================
m4return_t	M4MDT_DOMGetAttribute( const XMLCh* const ai_pcsName, const AttributeList &ai_roAttributes, string &ao_rsValue ) ;



//=================================================================================
//
// Busca un atributo dentro de un nodo DOM
//
// @param ai_pcsName
//
//		Nombre del atributo que se busca. Puede ser un puntero a char, ya que
//		hay un constructor con char pointer
//
// @param ai_pcoNode
//
//		Nodo DOM en el que buscar el atributo
//
// @param ao_rpcValue
//
//		Parámetro de salida con el valor del atributo o NULL si no existe
//
// @return
//
//		M4_SUCCESS si existe el atributo o M4_ERROR sino
//
//=================================================================================
m4return_t	M4MDT_DOMGetAttribute( const XMLCh* const ai_pcsName, const DOMNode* ai_pcoNode, ClCharHolder &ao_rpcValue ) ;



//=================================================================================
//
// Busca un atributo dentro de un nodo DOM
//
// @param ai_pcsName
//
//		Nombre del atributo que se busca. Puede ser un puntero a char, ya que
//		hay un constructor con char pointer
//
// @param ai_pcoNode
//
//		Nodo DOM en el que buscar el atributo
//
// @param ao_rsValue
//
//		Parámetro de salida con el valor del atributo o vacío si no existe
//
// @return
//
//		M4_SUCCESS si existe el atributo o M4_ERROR sino
//
//=================================================================================
m4return_t	M4MDT_DOMGetAttribute( const XMLCh* const ai_pcsName, const DOMNode* ai_pcoNode, string &ao_rsValue ) ;



//=================================================================================
//
// Busca un nodo DOM dentro de otro nodo DOM
//
// @param ai_pcsName
//
//		Nombre del nodo que se busca
//
// @param ai_iElement
//
//		Posición del nodo hijo a partir del cual buscar
//
// @param ai_pcoNode
//
//		Nodo DOM en el que buscar el nodo DOM
//
// @param ao_rpoChildNode
//
//		Parámetro de salida con el nodo DOM hijo si existe o NULL si no existe
//
// @return
//
//		La posición del nodo hijo ó -1 si no existen más hijos
//
//=================================================================================
unsigned int	M4MDT_DOMFindChildNode( const XMLCh* const ai_pcsName, unsigned int ai_iElement, const DOMNode* ai_pcoNode, DOMNode* &ao_rpoChildNode ) ;









//=================================================================================
//
// ClMDTDomTraverser
//
// Clase que recubre la lista de nodos hijos de un elemento DOM
// Indexa los hijos por el valor de un atributo y permite hacer búsquedas
// Al ser un multimapa permite varias entradas para una misma clave
//
//=================================================================================

class	ClMDTDomTraverser : public MDTDomNodeMap_t
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTDomTraverser( void ) ;
		virtual ~ClMDTDomTraverser( void ) ;


	//=================================================================================
	//
	// Inicializa el objeto a partir de un nodo DOM y un nombre de atributo
	//
	// @param ai_pcsNodeName
	//
	//		Nodo padre
	//
	// @param ai_pcsAttributeName
	//
	//		Nombre del atributo por el que indexar
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( const XMLCh* const ai_pcsNodeName, const XMLCh* const ai_pcsAttributeName, const DOMNode* ai_pcoNode ) ;


	//=================================================================================
	//
	// Busca el primer nodo con un valor de atributo concreto
	//
	// @param ai_rcsAttributeValue
	//
	//		Valor del atributo que debe tener el nodo que se busca
	//
	// @param ao_rpoNode
	//
	//		Nodo resultado de la búsqueda, que es el primero con ese nombre de atributo
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	FindChild( const string& ai_rcsAttributeValue, DOMNode* &ao_rpoNode ) const ;


} ;





//=================================================================================
//
// ClCharHolder
//
// Clase auxiliar que recubre un puntero a char generado a partir de un puntero 
// a XMLCh o un DOMString
// Permite gestionar la memoria creada, que se libera en el destructor
//
//=================================================================================

class	ClCharHolder
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		//=================================================================================
		//
		// Constructor vacío
		//
		//=================================================================================
		ClCharHolder::ClCharHolder( void )
		{
			m_pcValue = NULL ;
		}

		//=================================================================================
		//
		// Constructor a partir de un puntero a XMLCh
		// Traduce el puntero XMLCh a puntero a char
		//
		// @param ai_pccValue
		//
		//		El valor XMLCh a transformar y copiar
		//
		//=================================================================================
		ClCharHolder::ClCharHolder( const XMLCh* const ai_pccValue )
		{
			_Set( ai_pccValue ) ;
		}

		//=================================================================================
		//
		// Destructor
		// Libera la memoria del puntero
		//
		//=================================================================================
		virtual ~ClCharHolder( void )
		{
			_Clean() ;
		}


	//=================================================================================
	//
	// Asigna el puntero a nulo
	// Libera primero la memoria existente
	//
	//=================================================================================
	void SetNull()
	{
		_Clean() ;
	}

	//=================================================================================
	//
	// Asigna el puntero a char a partir de otro puntero a char
	// Libera primero la memoria existente
	//
	// @param ai_pccValue
	//
	//		El valor a copiar
	//
	//=================================================================================
	void Set( m4pcchar_t ai_pccValue )
	{
		_Clean() ;
		_Set( ai_pccValue ) ;
	}

	//=================================================================================
	//
	// Asigna el puntero a char a partir del puntero a XMLCh
	// Libera primero la memoria existente
	//
	// @param ai_pccValue
	//
	//		El valor XMLCh a transformar y copiar
	//
	//=================================================================================
	void Set( const XMLCh* const ai_pccValue )
	{
		_Clean() ;
		_Set( ai_pccValue ) ;
	}

	//=================================================================================
	//
	// Operador de casting a puntero a char
	//
	// @return
	//
	//			El valor del puntero
	//
	//=================================================================================
	operator m4pchar_t( void )
	{
		return( m_pcValue ) ;
	}


protected:

//=================================================================================
// Funciones auxiliares
//=================================================================================

	//=================================================================================
	//
	// Asigna el puntero a char a partir de otro puntero a char
	//
	// @param ai_pccValue
	//
	//		El valor a copiar
	//
	//=================================================================================
	void _Set( m4pcchar_t ai_pccValue ) ;

	//=================================================================================
	//
	// Asigna el puntero a char a partir del puntero a XMLCh
	//
	// @param ai_pccValue
	//
	//		El valor XMLCh a transformar y copiar
	//
	//=================================================================================
	void _Set( const XMLCh* const ai_pccValue ) ;

	//=================================================================================
	//
	// Libera la memoria asoicada al puntero
	//
	//=================================================================================
	void _Clean( void )
	{
		if( m_pcValue!= NULL )
		{
			delete [] m_pcValue ;
			m_pcValue = NULL ;
		}
	}


//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Puntero al valor
	//=================================================================================
	m4pchar_t	m_pcValue ;

} ;




#endif
