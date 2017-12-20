//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           filemem.cpp
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


#if defined(_WINDOWS)

	#include <windows.h>
	
#elif defined(_UNIX)
	
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <sys/mman.h>

	#define FILEMEM_MAP_ERROR(ptr) ((ptr)==(m4pchar_t)-1)

#else

	#error "Plataforma no definida !!!!"

#endif


#include "filemem.hpp"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ClFileInMem::ClFileInMem()
{
	m_bIsOpen = M4_FALSE;
	m_uFlags = FILEMEM_CLOSED;
	m_pMemPtr = NULL;
	m_uSize=0;
}


ClFileInMem::~ClFileInMem()
{
	Close();
}

m4return_t ClFileInMem::Open(m4pchar_t ai_pszFileName, m4uint32_t ai_flags)
{
	if (m_bIsOpen==M4_TRUE)
		Close();

#if defined(_WINDOWS)		// Win32 Open

	DWORD dwFlags=0;

	if (ai_flags & FILEMEM_READONLY) {

		if (ai_flags & FILEMEM_OPT_RANDOM)
			dwFlags |= FILE_FLAG_RANDOM_ACCESS;

		if (ai_flags & FILEMEM_OPT_SEQ)
			dwFlags |= FILE_FLAG_SEQUENTIAL_SCAN;

		m_hFile = CreateFile(ai_pszFileName, GENERIC_READ, FILE_SHARE_READ, 
									NULL,OPEN_EXISTING, dwFlags, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) 
			return M4_ERROR;

		m_uSize = GetFileSize(m_hFile,NULL);
		if (m_uSize == 0xFFFFFFFF) {
			CloseHandle(m_hFile);
			m_uSize=0;
			return M4_ERROR;
		}

		m_hFileMapping = CreateFileMapping(m_hFile, NULL,
														PAGE_READONLY,0,0,
														NULL);
		if (m_hFileMapping==NULL) {
			CloseHandle(m_hFile);
			m_uSize=0;
			return M4_ERROR;
		}


		m_pMemPtr = (m4pchar_t)MapViewOfFile(m_hFileMapping,
														FILE_MAP_READ,0,0,0);

		if (m_pMemPtr==NULL) {
			CloseHandle(m_hFileMapping);
			CloseHandle(m_hFile);
			m_uSize=0;
			return M4_ERROR;
		}

	} else {
		return M4_ERROR; // Unknown option
	}

#elif defined(_UNIX)		// Unix Open
	
	off_t size;

	if (ai_flags & FILEMEM_READONLY) {

		m_fildes = open(ai_pszFileName, O_RDONLY);	// Open file
		if (m_fildes==-1) {
			return M4_ERROR;
		}
	
		size=lseek(m_fildes, 0, SEEK_END);	// Get length
		if (size == (off_t)-1) {
			close(m_fildes);
			return M4_ERROR;
		}	
		lseek(m_fildes,0, SEEK_SET);
	
		m_pMemPtr=(m4pchar_t)mmap(NULL,size,PROT_READ, MAP_SHARED, m_fildes, 0);
		if (FILEMEM_MAP_ERROR(m_pMemPtr)) {
			close(m_fildes);
			m_pMemPtr=NULL;
			return M4_ERROR;
		}

		m_uSize = (m4uint32_t) size;
	} else {
		return M4_ERROR; // Unknown option
	}

#else
	#error "Plataforma no definida !!!!"

#endif

	m_bIsOpen=M4_TRUE;
	m_uFlags=ai_flags;
	return M4_SUCCESS;
}


m4return_t ClFileInMem::Close()
{
	if (m_bIsOpen==M4_FALSE) // Nothing to close
		return M4_WARNING;

	m_bIsOpen=M4_FALSE;	// Always close

#if defined(_WINDOWS)	// Win32 Close

	BOOL ret, ret2, ret3;

	ret = UnmapViewOfFile(m_pMemPtr);
	ret2 = CloseHandle(m_hFileMapping);
	ret3 = CloseHandle(m_hFile);

	m_uFlags = FILEMEM_CLOSED;
	m_pMemPtr = NULL;
	m_uSize=0;

	if (ret==0 || ret2==0 || ret==3)
		return M4_WARNING;
	else
		return M4_SUCCESS;
	
#elif defined(_UNIX)	// Unix Close

	m4uint32_t ret, ret2;

	ret = munmap(m_pMemPtr,(off_t)m_uSize);
	ret2 = close(m_fildes);
	m_uFlags = FILEMEM_CLOSED;
	m_pMemPtr = NULL;
	m_uSize=0;
	if (ret==-1 || ret2==-1)
		return M4_WARNING;
	else
		return M4_SUCCESS;
	
#else
	#error "Plataforma no definida !!!!"

#endif

	return M4_SUCCESS;
}
