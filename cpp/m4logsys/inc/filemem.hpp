//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           filemem.hpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           4 feb 99
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Files mapped in memory
////
//==============================================================================

#ifndef __FILEMEM_HPP__
#define __FILEMEM_HPP__

#include "m4types.hpp" 
/*
// If you don't want to include m4types, you will need:
typedef	char m4return_t;
typedef	char*	m4pchar_t;
typedef	unsigned int m4uint32_t;     //UNSIGNED int 32 bits (0, 4294967295)
typedef	char m4bool_t;
#define	M4_FALSE 0
#define	M4_TRUE 1
#define	M4_ERROR -1
#define	M4_SUCCESS 0
#define	M4_WARNING 1
*/


#define FILEMEM_CLOSED			0
#define FILEMEM_READONLY		1
#define FILEMEM_OPT_SEQ			64
#define FILEMEM_OPT_RANDOM		128

class ClFileInMem  
{
public:

	ClFileInMem();
	~ClFileInMem();

	m4return_t Open(m4pchar_t ai_pszFileName, m4uint32_t ai_flags);
	m4return_t Close();

	m4uint32_t GetSize() {
		return m_uSize;
	};

	m4pchar_t GetPtr() {
		return m_pMemPtr;
	};

	m4uint32_t GetFlags() {
		return m_uFlags;
	};

	m4bool_t IsOpen() {
		return m_bIsOpen;
	}

private:

	m4bool_t m_bIsOpen;
	m4uint32_t m_uFlags;

	m4pchar_t m_pMemPtr;
	m4uint32_t m_uSize;


#if defined(_WINDOWS)

	// We use void* instead of HANDLE so we don't need to include windows.h
	//  in this header.
	void *m_hFile;		
	void *m_hFileMapping;
	
#elif defined(_UNIX)
	int m_fildes; // File descriptor
	
#else
	#error "Plataforma no definida !!!!"

#endif

};

#endif // __FILEMEM_HPP__
