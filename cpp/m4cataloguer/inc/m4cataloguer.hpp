
//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cataloguer
// File:                m4cataloguer.hpp   
// Project:             PeopleNet      
// Author:              Meta Software M.S. , S.A
// Date:                09-02-2016
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the class for creating installation catalogues
//
//==============================================================================


#include "m4types.hpp"
#include "m4stl.hpp"
#include "file_adv.hpp"
#include "lzmaenc.h"


#ifndef __M4CATALOGUER_HPP__
#define __M4CATALOGUER_HPP__


class M4FileInfo;



//=================================================================================
//
// M4Cataloguer
//
// This class defines installation catalogue creator
//
//=================================================================================

class	M4Cataloguer : public ICompressProgress
{

protected:

	m4uint32_t			m_iLastPercentage ;			// Last progress percentage
	m4uint32_t			m_iTotalBytesRead ;			// Number of bytes read so far
	m4uint32_t			m_iTotalBytesToRead ;		// Total number of bytes to read
	m4pcchar_t			m_pccRootDirectory ;		// Root directory file path
	size_t				m_iRootDirectoryLength ;	// Root directory file path length
	m4pcchar_t			m_pccZipExension ;			// Compressed files extension
	size_t				m_iZipExtensionLength ;		// Compressed files extension length

	vector<m4pchar_t>	m_oStartIncludes ;			// Start include pattern list
	vector<m4pchar_t>	m_oEndIncludes ;			// End include pattern list
	vector<m4pchar_t>	m_oStartExcludes ;			// Start exclude pattern list
	vector<m4pchar_t>	m_oEndExcludes ;			// End exclude pattern list

public:

// Funciones de inicialización ================================================

		M4Cataloguer( m4pcchar_t ai_pccDirectory ) ;
		~M4Cataloguer( void ) ;

// Funciones de progreso ======================================================

	m4uint32_t	GetTotalBytesRead( void ) const
	{
		return( m_iTotalBytesRead ) ;
	}

	void		SetProgress( m4uint32_t ai_iBytes ) ;

// Funciones del API ==========================================================

	m4int32_t	Catalogue( void ) ;

protected:

// Funciones internas =========================================================

	void		_OnProgressChanged( m4uint32_t ai_Percentage ) ;
	m4int32_t	_Catalogue( void ) ;
	m4return_t	_GenerateFileLists( m4pcchar_t ai_pccDirectory, vector<ClFile>& ai_rvFileList, vector<ClFile>& ai_rvHelpList, m4uint32_t& ao_riTotalBytes ) ;
	m4bool_t	_HasToInclude( m4pcchar_t ai_pccFilename ) ;
	m4return_t	_GenerateCatalogue( m4pcchar_t ai_pccDirectory, m4pcchar_t ai_pccCatalogue, vector<ClFile>& ai_rvList, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, CLzmaEncHandle& ai_roEncoder ) ;
	m4return_t	_GenerateFileInfo( ClFile& ai_roFile, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, CLzmaEncHandle& ai_roEncoder, M4FileInfo* &ao_rpoFile ) ;
};


#endif

