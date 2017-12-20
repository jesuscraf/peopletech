
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtparameter.cpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                30-03-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene las funciones de parámetros de la librería
//
//
//==============================================================================


#include "m4mdtparameter.hpp"

#include "m4mdtinterfaces.hpp"
#include "m4mdtdef.hpp"
#include "m4mdtres.hpp"





//=================================================================================
// Funciones de inicialización
//=================================================================================


//=================================================================================
//
// Constructor
//
// Inicializa el contador a 0
//
//=================================================================================

ClMDTParameterManager::ClMDTParameterManager( void )
{
	m_iLastParameter = 0 ;
}



//=================================================================================
//
// Destructor
//
// Recorre el conjunto de parámetros, liberando cada uno de ellos de manera
// no recursiva
//
//=================================================================================

ClMDTParameterManager::~ClMDTParameterManager( void )
{

	m4return_t					iResult = M4_SUCCESS ;
	m4int32_t					iParameter = -1 ;
	ParameterMap_t::iterator	iterator ;


	iterator = m_oParameters.begin() ;

	while( iterator != m_oParameters.end() )
	{
		iParameter = (*iterator).first ;

		iResult = _ReleaseParameter( iParameter, M4_FALSE ) ;
		M4MDT_CHECK_ERROR2( iResult != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::~ClMDTParameterManager", iParameter, "%s#Parameter = <%d>" ) ;

		iterator++ ;
	}
}



//=================================================================================
// Inicializa el objeto
// De momento no hace nada
//=================================================================================

m4return_t ClMDTParameterManager::Init( void )
{
	return( M4_SUCCESS ) ;
}





//=================================================================================
// Funciones del interfaz
//=================================================================================


//=================================================================================
// Obtiene el número de parejas de valores de un parámetro
// Para ello obtiene el parámetro por handle y pregunta el tamaño del vector
//=================================================================================

m4return_t	ClMDTParameterManager::GetParameterSize( m4int32_t ai_iParameter, m4uint32_t& ao_riSize ) const
{

	ParameterValueVector_t	*poParameter = NULL ;


	ao_riSize = 0 ;
	poParameter = _GetParameter( ai_iParameter ) ;
	M4MDT_RETURN_ERROR2( poParameter == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::GetParameterSize", ai_iParameter, "%s#Parameter = <%d>" ) ;

	ao_riSize = poParameter->size() ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Obtiene los valores de una pareja de valores de un parámetro
// La pareja se identifica por su posición dentro del conjunto
// El valor de la pareja debe ser de tipo cadena
//
// Pasos
//		Se inicializan los parámetros de salida a NULL
//		Se obtiene el puntero a la pareja de valores
//		Se asignan los parámetros de salida a sus valores constantes
//
//=================================================================================

m4return_t	ClMDTParameterManager::GetParameterValue( m4int32_t ai_iParameter, m4uint32_t ai_iPosition, m4pcchar_t& ao_rpccFirst, m4pcchar_t& ai_rpccSecond ) const
{

	ParameterValue_t	*poValue = NULL ;


	ao_rpccFirst = NULL ;
	ai_rpccSecond = NULL ;

	poValue = _GetParameterValue( ai_iParameter, ai_iPosition ) ;
	M4MDT_RETURN_ERROR3( poValue == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::GetParameterValue", ai_iParameter, ai_iPosition, "%s#Parameter = <%d> - Position = <%d>" ) ;
	M4MDT_RETURN_ERROR2( poValue->Type != m4mdt_string, M4_ERROR, M4MDTERROR_PARAM_VALUE_NO_STRING, ai_iPosition, ai_iParameter, "%d#%d" ) ;

	ao_rpccFirst = poValue->First ;
	ai_rpccSecond = poValue->Second.Value ;

	
	return( M4_SUCCESS ) ;
}



//=================================================================================
// Obtiene los valores de una pareja de valores de un parámetro
// La pareja se identifica por su posición dentro del conjunto
// El valor de la pareja debe ser de tipo parámetro
//
// Pasos
//		Se inicializan los parámetros de salida a NULL
//		Se obtiene el puntero a la pareja de valores
//		Se asignan los parámetros de salida a sus valores
//
//=================================================================================

m4return_t	ClMDTParameterManager::GetParameterValue( m4int32_t ai_iParameter, m4uint32_t ai_iPosition, m4pcchar_t& ao_rpccFirst, m4int32_t& ai_riSecond ) const
{

	ParameterValue_t	*poValue = NULL ;


	ao_rpccFirst = NULL ;
	ai_riSecond = -1 ;

	poValue = _GetParameterValue( ai_iParameter, ai_iPosition ) ;
	M4MDT_RETURN_ERROR3( poValue == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::GetParameterValue", ai_iParameter, ai_iPosition, "%s#Parameter = <%d> - Position = <%d>" ) ;
	M4MDT_RETURN_ERROR2( poValue->Type != m4mdt_parameter, M4_ERROR, M4MDTERROR_PARAM_VALUE_NO_PARAM, ai_iPosition, ai_iParameter, "%d#%d" ) ;

	ao_rpccFirst = poValue->First ;
	ai_riSecond = poValue->Second.Parameter ;
	

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Crea un parámetro nuevo
//
// Pasos
//		Se inicializa el parámetro de salida a -1
//		Se crea un nuevo vector de parejas de valores con tamaño 16
//		Se incrementa el contador de parámetros
//		Se añade ese vector al mapa de parámetros. Si ya existe y no se añade se
//			da un error
//		Se asigna el handle de salida al contador de parámetros
//
//=================================================================================

m4return_t	ClMDTParameterManager::CreateParameter( m4int32_t& ao_riParameter )
{

	ParameterValueVector_t					*poParameter = NULL ;
	pair<ParameterMap_t::iterator, bool>	pair ;


	ao_riParameter = -1 ;

	poParameter = new ParameterValueVector_t ;
	M4MDT_RETURN_ERROR1( poParameter == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( ParameterValueVector_t ), "%d" ) ;

	poParameter->reserve( 16 ) ;

	m_iLastParameter++ ;

	pair = m_oParameters.insert( ParameterMap_t::value_type( m_iLastParameter, poParameter ) ) ;
	M4MDT_RETURN_ERROR2( pair.second != true, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::CreateParameter", m_iLastParameter, "%s#Parameter = <%d>" ) ;

	ao_riParameter = m_iLastParameter ;


	return( M4_SUCCESS ) ;
}



//=================================================================================
// Libera un parámetro existente
// Lo hace de manera recursiva
//=================================================================================

m4return_t	ClMDTParameterManager::ReleaseParameter( m4int32_t ai_iParameter )
{

	m4return_t	iResult = M4_SUCCESS ;


	iResult = _ReleaseParameter( ai_iParameter, M4_TRUE ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::ReleaseParameter", ai_iParameter, "%s#Parameter = <%d>" ) ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Crea una nueva pareja de valores, la rellena con los valores pasados y la añade
// al conjunto de parejas al final
// El valor de la pareja se crea de tipo cadena
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se obtiene el vector de parejas por handle
//		Se crea una pareja de valores con la copia de los parámetros de entrada
//		Se añade la pareja al final del vector
//
//=================================================================================

m4return_t	ClMDTParameterManager::AddParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4pcchar_t ai_pccSecond )
{

	size_t					iLength = 0 ;
	ParameterValue_t		oValue ;
	ParameterValueVector_t	*poParameter = NULL ;


	M4MDT_RETURN_ERROR2( ai_pccFirst == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFirst", "ClMDTParameterManager::AddParameterValue", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccSecond == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccSecond", "ClMDTParameterManager::AddParameterValue", "%s#%s" ) ;


	poParameter = _GetParameter( ai_iParameter ) ;
	M4MDT_RETURN_ERROR2( poParameter == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::AddParameterValue", ai_iParameter, "%s#Parameter = <%d>" ) ;


	iLength = strlen( ai_pccFirst ) + 1 ;
	oValue.First = new m4char_t[ iLength ] ;
	M4MDT_RETURN_ERROR1( oValue.First == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( m4char_t ) * iLength, "%d" ) ;

	memcpy( oValue.First, ai_pccFirst, iLength ) ;

	oValue.Type = m4mdt_string ;

	iLength = strlen( ai_pccSecond ) + 1 ;
	oValue.Second.Value = new m4char_t[ iLength ] ;
	M4MDT_RETURN_ERROR1( oValue.Second.Value == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( m4char_t ) * iLength, "%d" ) ;

	memcpy( oValue.Second.Value, ai_pccSecond, iLength ) ;


	poParameter->push_back( oValue ) ;


	return( M4_SUCCESS ) ;
}


//=================================================================================
// Crea una nueva pareja de valores, la rellena con los valores pasados y la añade
// al conjunto de parejas al final
// El valor de la pareja se crea de tipo parámetro
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se obtiene el vector de parejas por handle
//		Se crea una pareja de valores con la copia del primer parámetro de entrada
//		Se añade la pareja al final del vector
//
//=================================================================================

m4return_t	ClMDTParameterManager::AddParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4int32_t ai_iSecond )
{

	size_t					iLength = 0 ;
	ParameterValue_t		oValue ;
	ParameterValueVector_t	*poParameter = NULL ;


	M4MDT_RETURN_ERROR2( ai_pccFirst == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFirst", "ClMDTParameterManager::AddParameterValue", "%s#%s" ) ;

	
	poParameter = _GetParameter( ai_iParameter ) ;
	M4MDT_RETURN_ERROR2( poParameter == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::AddParameterValue", ai_iParameter, "%s#Parameter = <%d>" ) ;

	iLength = strlen( ai_pccFirst ) + 1 ;
	oValue.First = new m4char_t[ iLength ] ;
	M4MDT_RETURN_ERROR1( oValue.First == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( m4char_t ) * iLength, "%d" ) ;

	memcpy( oValue.First, ai_pccFirst, iLength ) ;

	oValue.Type = m4mdt_parameter ;
	oValue.Second.Parameter = ai_iSecond ;


	poParameter->push_back( oValue ) ;


	return( M4_SUCCESS ) ;
}


//=================================================================================
//
// Elimina una nueva pareja de valores de tipo cadena
//
// Pasos
//		Se obtiene el vector de parejas por handle
//		Se itera por el vector
//		Se borran aquellos elementos de tipo cadena cuyos miembros
//			primero y segundo son iguales a los argumentos de la función.
//		Para borrarlos hay que eliminarlos del vector y liberar su memoria
//		La función erase del vector ya avanza el iterador al siguiente válido
//
//=================================================================================

m4return_t	ClMDTParameterManager::RemoveParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4pcchar_t ai_pccSecond )
{

	m4pchar_t							pccFirst = NULL ;
	m4pchar_t							pccSecond = NULL ;
	ParameterValueVector_t				*poParameter = NULL ;
	ParameterValueVector_t::iterator	iterator ;


	M4MDT_RETURN_ERROR2( ai_pccFirst == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFirst", "ClMDTParameterManager::RemoveParameterValue", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccSecond == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccSecond", "ClMDTParameterManager::RemoveParameterValue", "%s#%s" ) ;

	
	poParameter = _GetParameter( ai_iParameter ) ;
	M4MDT_RETURN_ERROR2( poParameter == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::RemoveParameterValue", ai_iParameter, "%s#Parameter = <%d>" ) ;


	iterator = poParameter->begin() ;

	while( iterator != poParameter->end() )
	{
		// M4MDT_RETURN_ERROR2( iterator == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::RemoveParameterValue", ai_iParameter, "%s#Parameter = <%d>" ) ;

		if( iterator->Type == m4mdt_string && strcmpi( iterator->Second.Value, ai_pccSecond ) == 0 && strcmpi( iterator->First, ai_pccFirst ) == 0 )
		{
			pccFirst = iterator->First ;
			pccSecond = iterator->Second.Value ;

			iterator = poParameter->erase( iterator ) ;

			delete [] pccFirst ;
			delete [] pccSecond ;
		}
		else
		{
			iterator++ ;
		}
	}

	return( M4_SUCCESS ) ;
}


//=================================================================================
//
// Elimina una nueva pareja de valores de tipo parámetro
//
// Pasos
//		Se obtiene el vector de parejas por handle
//		Se itera por el vector
//		Se borran aquellos elementos de tipo parámetro cuyos miembros
//			primero y segundo son iguales a los argumentos de la función.
//		Para borrarlos hay que eliminarlos del vector y liberar su memoria
//		La función erase del vector ya avanza el iterador al siguiente válido
//
//=================================================================================

m4return_t	ClMDTParameterManager::RemoveParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4int32_t ai_iSecond )
{

	m4pchar_t							pccFirst = NULL ;
	ParameterValueVector_t				*poParameter = NULL ;
	ParameterValueVector_t::iterator	iterator ;


	M4MDT_RETURN_ERROR2( ai_pccFirst == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFirst", "ClMDTParameterManager::RemoveParameterValue", "%s#%s" ) ;

	
	poParameter = _GetParameter( ai_iParameter ) ;
	M4MDT_RETURN_ERROR2( poParameter == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::RemoveParameterValue", ai_iParameter, "%s#Parameter = <%d>" ) ;


	iterator = poParameter->begin() ;

	while( iterator != poParameter->end() )
	{
		// M4MDT_RETURN_ERROR2( iterator == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::RemoveParameterValue", ai_iParameter, "%s#Parameter = <%d>" ) ;

		if( iterator->Type == m4mdt_parameter && iterator->Second.Parameter == ai_iSecond && strcmpi( iterator->First, ai_pccFirst ) == 0 )
		{
			pccFirst = iterator->First ;

			iterator = poParameter->erase( iterator ) ;

			delete [] pccFirst ;
		}
		else
		{
			iterator++ ;
		}
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Funciones auxiliares
//=================================================================================


//=================================================================================
//
// Obtiene el vector de valores de un parámetro identificado por su handle
//
//
// Pasos
//		Se hace una búsqueda
//		Se comprueba que el elemento exista
//		Se devuelve un puntero al elemento
//
//=================================================================================

ParameterValueVector_t*	ClMDTParameterManager::_GetParameter( m4int32_t ai_iParameter ) const
{

	ParameterValueVector_t			*poResult = NULL ;
	ParameterMap_t::const_iterator	iterator ;


	iterator = m_oParameters.find( ai_iParameter ) ;
	M4MDT_RETURN_ERROR1( iterator == m_oParameters.end(), NULL, M4MDTERROR_NO_SUCH_PARAM, ai_iParameter, "%d" ) ;

	poResult = (*iterator).second ;

	return( poResult ) ;
}



//=================================================================================
//
// Obtiene la estructura con los valores de una pareja de valores de un parámetro
// La pareja se identifica por su posición dentro del conjunto
//
//
// Pasos
//		Se obtiene el vector de parejas por handle
//		Se obtiene el tamaño del vector de parejas y se chequea la validez
//		Se devuelve la pareja por posición
//
//=================================================================================

ParameterValue_t*	ClMDTParameterManager::_GetParameterValue( m4int32_t ai_iParameter, m4uint32_t ai_iPosition ) const
{

	ParameterValue_t		*poResult = NULL ;
	m4uint32_t				iSize = 0 ;
	ParameterValueVector_t	*poParameter = NULL ;


	poParameter = _GetParameter( ai_iParameter ) ;
	M4MDT_RETURN_ERROR2( poParameter == NULL, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::_GetParameterValue", ai_iParameter, "%s#Parameter = <%d>" ) ;

	iSize = poParameter->size() ;
	M4MDT_RETURN_ERROR3( ai_iPosition >= iSize, NULL, M4MDTERROR_NO_SUCH_PARAM_VALUE, ai_iPosition, iSize, ai_iParameter, "%d#%d#%d" ) ;

	poResult = &( (*poParameter)[ ai_iPosition ] ) ;
	

	return( poResult ) ;
}



//=================================================================================
//
// Libera un parámetro existente
// No se hace gestión de errores, porque aunque algo falle hay que seguir
// para borrar todo correctamente
//
// Pasos
//		Se obtiene el vector de parejas por handle
//		Si el borrado es recursivo se saca el parámetro del mapa
//		Se recorre el vector de parejas
//		Para cada pareja se libera la memoria asociada a su primer elemento
//		Si es de tipo cadena se libera la memoria asociada a su segundo elemento
//		Si es de tipo parámetro se libera recursivamente el segundo elemento
//		Al acabar el recorrido se vacía el vector de parejas y se borra
//
//=================================================================================

m4return_t	ClMDTParameterManager::_ReleaseParameter( m4int32_t ai_iParameter, m4bool_t ai_bRecursive )
{

	m4return_t				iResult = M4_SUCCESS ;
	m4uint32_t				i = 0 ;
	m4uint32_t				iSize = 0 ;
	ParameterValue_t		*poValue = NULL ;
	ParameterValueVector_t	*poParameter = NULL ;


	poParameter = _GetParameter( ai_iParameter ) ;
	M4MDT_RETURN_ERROR2( poParameter == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::_ReleaseParameter", ai_iParameter, "%s#Parameter = <%d>" ) ;

	if( ai_bRecursive == M4_TRUE )
	{
		m_oParameters.erase( ai_iParameter ) ;
	}
	
	iSize = poParameter->size() ;

	for( i = 0 ; i < iSize ; i++ )
	{
		poValue = &( (*poParameter)[ i ] ) ;

		if( poValue->First != NULL )
		{
			delete [] poValue->First ;
			poValue->First = NULL ;
		}

		if( poValue->Type == m4mdt_string )
		{
			if( poValue->Second.Value != NULL )
			{
				delete [] poValue->Second.Value ;
				poValue->Second.Value = NULL ;
			}
		}
		else
		{
			if( ai_bRecursive == M4_TRUE )
			{
				if( poValue->Second.Parameter != -1 )
				{
					iResult = _ReleaseParameter( poValue->Second.Parameter, ai_bRecursive ) ;
					M4MDT_CHECK_ERROR3( iResult != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTParameterManager::_ReleaseParameter", poValue->Second.Parameter, i, "%s#Parameter = <%d> - Position = <%d>" ) ;

					poValue->Second.Parameter = -1 ;
				}
			}
		}
	}

	delete( poParameter ) ;


	return( M4_SUCCESS ) ;
}



