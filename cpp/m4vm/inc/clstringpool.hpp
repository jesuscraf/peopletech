
//==============================================================================
//
// (c) Copyright  1991-2012 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4variant.dll
// File:                clstringpool.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                17-10-2012
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines a dynamic string pool.
//
//
//==============================================================================


#include "m4vm_dll.hpp"

#include <stdio.h>
#include "m4types.hpp"
#include "medvarin.hpp"


//#define _M4_TRACE_POOL

#ifdef _M4_TRACE_POOL
#include "cldefine.hpp"
#include "m4stl.hpp"
class ClVMachine0 ;
#endif


#ifndef __CLSTRINGPOOL_HPP__
#define __CLSTRINGPOOL_HPP__

#ifdef _M4_TRACE_POOL
struct StStringAllocation
{
//	m4char_t	m_acChannel [ M4CL_MAX_T3_ID   + 1 ] ;
//	m4char_t	m_acNode	[ M4CL_MAX_NODE_ID + 1 ] ;
//	m4char_t	m_acItem	[ M4CL_MAX_ITEM_ID + 1 ] ;
	m4uint32_t	m_iAllocation ;
	m4uint32_t	m_iHandle ;
	m4uint32_t	m_iLine ;
};
#endif






//=================================================================================
//
// ClOnePool
//
// Clase que implementa un pool unitario de cadenas
//
//=================================================================================


class ClOnePool
{

protected:

	m4pchar_t   m_pcStart ;
	m4pchar_t   m_pcCurrent ;
	m4pchar_t   m_pcEnd ;

public:

// Funciones de inicialización ================================================
	
		ClOnePool( void ) ;
		~ClOnePool( void ) ;

	void Reset( void ) ;
	m4return_t	Create( m4uint32_t ai_iSize ) ;
	m4pchar_t	Destroy( m4bool_t ai_bFree = M4_TRUE ) ;
	void		Move( ClOnePool& ai_roPool ) ;

// Funciones de lectura =======================================================

	size_t	GetLength( void ) const
	{
		return( m_pcCurrent - m_pcStart ) ;
	}

	size_t	GetSize( void ) const
	{
		return( m_pcEnd - m_pcStart ) ;
	}

	m4pchar_t	GetString( m4uint32_t ai_iOffset ) const ;
	m4return_t	Dump( FILE *ai_poFile ) const ;

// Funciones de escritura =====================================================

	m4return_t	NewString( m4uint32_t ai_iInSize, m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t &ao_rpcOutString, m4uint32_t &ao_riOutOffset ) ;
	m4return_t	DeleteString( m4uint32_t ai_iOffset ) ;
};




//=================================================================================
//
// ClStringPool
//
// Clase que implementa el pool dinámico de cadenas
//
//=================================================================================

class M4_DECL_M4VM ClStringPool : public IStringPool
{

protected:

	m4uint32_t	m_iPoolSize ;
	m4uint16_t	m_iLength ;
	m4uint16_t	m_iSize ;
	ClOnePool*	m_poPoolArray ;

#ifdef _M4_TRACE_POOL
	ClVMachine0*	m_poVM ;
	m4uint32_t		m_iAllocation ;
	map<m4int32_t, StStringAllocation, less<m4int32_t> > m_oAllocatedStrings ;
#endif

public:

// Funciones de inicialización ================================================
	
		ClStringPool( m4uint32_t ai_PoolSize ) ;
		~ClStringPool( void ) ;

    void Reset( void ) ;

// Funciones de lectura =======================================================

	m4uint32_t	GetSize( void ) const ;
	m4pchar_t	GetString( m4uint32_t ai_iOffset ) const ;
	m4uint32_t	GetStringLength( m4uint32_t ai_iOffset ) const ;
	m4return_t	Dump( FILE *ai_poFile ) const ;

// Funciones de escritura =====================================================

	m4return_t	AddString( m4pcchar_t ai_pccInString, m4uint32_t &ao_riOutOffset ) ;
	m4return_t	AddStringConcat( m4uint32_t ai_iInOffset1, m4uint32_t ai_iInOffset2 , m4uint32_t &ao_riOutOffset ) ;
	m4return_t	SetString( m4uint32_t ai_iInOffset, m4uint32_t &aio_riOutOffset ) ;
	m4return_t	DeleteString( m4uint32_t ai_iOffset ) ;

#ifdef _M4_TRACE_POOL
	void SetVM( ClVMachine0 *ai_poVM )
	{
		m_poVM = ai_poVM ;
	}
#endif

private:

// Funciones auxiliares =======================================================

	m4return_t	AllocString( m4uint32_t ai_iInSize, m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t &ao_rpcOutString, m4uint32_t &ao_riOutOffset ) ;
	m4return_t	NewString( m4uint16_t ai_iPool, m4uint32_t ai_iInSize, m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t &ao_rpcOutString, m4uint32_t &ao_riOutOffset ) ;
};


#endif

