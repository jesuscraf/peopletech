//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                mmmanager.hpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                15-09-2013
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo define un gestor de memoria mapeada a afichero
//
//
//==============================================================================


#ifndef __MMMANAGER_HPP__
#define __MMMANAGER_HPP__

#ifdef _WINDOWS
#include <windows.h>
#endif

#include "m4objglb_dll.hpp"
#include "m4types.hpp"


class M4_DECL_M4OBJGLB ClMappedMemoryManager 
{

public:

		ClMappedMemoryManager( size_t ai_iSize ) ;

		~ClMappedMemoryManager( void ) ;

		m4pchar_t	GetMemory( void ) const
		{
			return( m_pcMemory ) ;
		}

private:

	m4pchar_t	m_pcMemory ;

#ifdef _WINDOWS
	HANDLE		m_hFileMapping ;
#else
	size_t		m_iSize ;
#endif
};


#endif
