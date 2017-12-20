
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                facide.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se declara el objeto para los identificadores
//
//
//==============================================================================


#include "mdfacglb.hpp"
#include "cldefine.hpp"


#ifndef __FACIDE_HPP__
#define __FACIDE_HPP__




//=================================================================================
// Define de identificador standard, salvo fórmulas de nitem
//=================================================================================
/* Bug 0106669
El tamaño de los mapeos es 40
*/
#define	M4CL_MAX_IDENTIFIER	                            40





// Funciones de comparacion ===================================================

int	M4ClCompareObjectsByNumber	( m4pcvoid_t ai_pcvObject1, m4pcvoid_t ai_pcvObject2 ) ;
int	M4ClCompareObjectsById		( m4pcvoid_t ai_pcvObject1, m4pcvoid_t ai_pcvObject2 ) ;
int	M4ClCompareObjectsByTwoIds	( m4pcvoid_t ai_pcvObject1, m4pcvoid_t ai_pcvObject2 ) ;




//=================================================================================
//
// ClNumericId
//
// Clase que implementa la definición de los objetos con identificador numérico
//
//=================================================================================

class	ClNumericId
{

public:


// Funciones de inicialización ================================================
	
		ClNumericId( void ) {} ;
virtual	~ClNumericId( void ) {} ;


// Funciones de lectura =======================================================

virtual	m4uint32_t	GetIdentifier( void ) const = 0 ;

} ;



//=================================================================================
//
// ClCompNumericId
//
// Clase que implementa la definición de los objetos comparación con identificador
// numérico
//
//=================================================================================

class	ClCompNumericId : public ClNumericId
{

protected:

	m4uint32_t	m_iIdentifier ;

public:


// Funciones de inicialización ================================================
	
		ClCompNumericId( void )
		{
			m_iIdentifier = 0 ;
		}

		~ClCompNumericId( void ) {} ;


// Funciones de lectura =======================================================

	m4uint32_t	GetIdentifier( void ) const
	{
		return( m_iIdentifier ) ;
	}

// Funciones de escritura =====================================================

	void	SetIdentifier( m4uint32_t ai_iIdentifier )
	{
		m_iIdentifier = ai_iIdentifier ;
	}

} ;





//=================================================================================
//
// ClIdentifier
//
// Clase que implementa la definición de los objetos con identificador
//
//=================================================================================

class	ClIdentifier
{

public:


// Funciones de inicialización ================================================
	
		ClIdentifier( void ) {} ;
virtual	~ClIdentifier( void ) {} ;


// Funciones de lectura =======================================================

virtual	m4pcchar_t	GetIdentifier( void ) const = 0 ;

} ;



//=================================================================================
//
// ClCompIdentifier
//
// Clase que implementa la definición de los objetos comparación con identificador
//
//=================================================================================

class	ClCompIdentifier : public ClIdentifier
{

protected:

	m4char_t	m_acIdentifier[ M4CL_MAX_IDENTIFIER + 2 ] ;


public:


// Funciones de inicialización ================================================
	
		ClCompIdentifier( void )
		{
			m_acIdentifier[ 0 ] = '\0' ;
			m_acIdentifier[ 1 ] = '\0' ;
		}

		~ClCompIdentifier( void ) {} ;


// Funciones de lectura =======================================================

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( m_acIdentifier + 1 ) ;
	}

// Funciones de escritura =====================================================

	m4return_t	SetIdentifier( m4pcchar_t ai_pccIdentifier, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccIdentifier, m_acIdentifier, M4CL_MAX_IDENTIFIER, ai_bConvert ) ) ;
	}

} ;




//=================================================================================
//
// ClTwoIdentifier
//
// Clase que implementa la definición de los objetos con dos identificadores
//
//=================================================================================

class	ClTwoIdentifier : public ClIdentifier
{

public:


// Funciones de inicialización ================================================
	
		ClTwoIdentifier( void ) {} ;
virtual	~ClTwoIdentifier( void ) {} ;


// Funciones de lectura =======================================================

virtual	m4pcchar_t	GetParentIdentifier( void ) const = 0 ;

} ;



//=================================================================================
//
// ClCompTwoIdentifier
//
// Clase que implementa la definición de los objetos comparación con
// dos identificadores
//
//=================================================================================



class	ClCompTwoIdentifier : public ClTwoIdentifier
{

protected:

	m4char_t	m_acParentIdentifier[ M4CL_MAX_IDENTIFIER + 2 ] ;
	m4char_t	m_acIdentifier      [ M4CL_MAX_IDENTIFIER + 2 ] ;


public:


// Funciones de inicialización ================================================
	
		ClCompTwoIdentifier( void )
		{
			m_acParentIdentifier[ 0 ] = '\0' ;
			m_acParentIdentifier[ 1 ] = '\0' ;
			m_acIdentifier      [ 0 ] = '\0' ;
			m_acIdentifier      [ 1 ] = '\0' ;
		}

		~ClCompTwoIdentifier( void ) {} ;


// Funciones de lectura =======================================================

	m4pcchar_t	GetParentIdentifier( void ) const
	{
		return( m_acParentIdentifier + 1 ) ;
	}

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( m_acIdentifier + 1 ) ;
	}


// Funciones de escritura =====================================================

	m4return_t	SetParentIdentifier( m4pcchar_t ai_pccIdentifier, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccIdentifier, m_acParentIdentifier, M4CL_MAX_IDENTIFIER, ai_bConvert ) ) ;
	}

	m4return_t	SetIdentifier( m4pcchar_t ai_pccIdentifier, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccIdentifier, m_acIdentifier, M4CL_MAX_IDENTIFIER, ai_bConvert ) ) ;
	}

} ;




#endif

