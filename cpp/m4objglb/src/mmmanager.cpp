//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                mmmanager.cpp
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



#include "mmmanager.hpp"

#ifdef _UNIX
#include <sys/types.h>
#include <sys/mman.h>
#endif



ClMappedMemoryManager :: ClMappedMemoryManager( size_t ai_iSize )
{

	m_pcMemory = NULL ;


#ifdef _WINDOWS

	m_hFileMapping = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, ai_iSize, NULL ) ;

	if( m_hFileMapping != NULL )
	{
		m_pcMemory = (m4pchar_t) MapViewOfFile( m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, ai_iSize ) ;
	}
#else

	m_iSize = ai_iSize ;
	m_pcMemory = (m4pchar_t) mmap( NULL, m_iSize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0 ) ;

	if( m_pcMemory == MAP_FAILED )
	{
		m_pcMemory = NULL ;
		m_iSize = 0 ;
	}
#endif
}


ClMappedMemoryManager :: ~ClMappedMemoryManager( void )
{
#ifdef _WINDOWS

	if( m_pcMemory != NULL )
	{
		UnmapViewOfFile( m_pcMemory ) ;
		m_pcMemory = NULL ;
	}

	if( m_hFileMapping != NULL )
	{
		CloseHandle( m_hFileMapping ) ;
		m_hFileMapping = NULL ;
	}
#else

	if( m_pcMemory != NULL )
	{
		munmap( m_pcMemory, m_iSize ) ;
		m_pcMemory = NULL ;
	}
#endif
}


