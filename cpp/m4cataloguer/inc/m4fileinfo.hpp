
//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cataloguer
// File:                m4fileinfo.hpp   
// Project:             PeopleNet      
// Author:              Meta Software M.S. , S.A
// Date:                11-02-2016
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the class for storing installation file information
//
//==============================================================================


#include "m4types.hpp"
#include "m4stl.hpp"

#ifndef __M4FILEINFO_HPP__
#define __M4FILEINFO_HPP__



//=================================================================================
//
// M4FileInfo
//
// This class holds file information
//
//=================================================================================

class	M4FileInfo
{

protected:

	m4pchar_t	m_pcPath ;		// File path relative to root
	m4uint32_t	m_iSize ;		// File size
	m4uint32_t	m_iCrc ;		// File contents crc

public:

// Funciones de inicialización ================================================

		M4FileInfo( m4pcchar_t ai_pccPath, m4uint32_t ai_iSize, m4uint32_t ai_iCrc ) ;
		~M4FileInfo( void ) ;

// Funciones del lectura ======================================================

	m4pcchar_t	GetPath( void ) const
	{
		return( m_pcPath ) ;
	}

	m4uint32_t	GetSize( void ) const
	{
		return( m_iSize ) ;
	}

	m4uint32_t	GetCrc( void ) const
	{
		return( m_iCrc ) ;
	}

// Funciones del escritura ====================================================

	static m4return_t	Write( m4pcchar_t ai_pccFilename, vector<M4FileInfo*>& ai_rvFiles ) ;
};


#endif

