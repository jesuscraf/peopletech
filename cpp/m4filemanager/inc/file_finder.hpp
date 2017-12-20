
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4filemanager.dll
// File:                file_finder.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//
//==============================================================================


#include "file_adv.hpp"
#include "file_misc.hpp"


#ifndef __FILE_FINDER_HPP__
#define __FILE_FINDER_HPP__



class M4_DECL_M4FILEMANAGER  ClFileFinder 
{
public:

		ClFileFinder( void ) ;
		ClFileFinder( m4pcchar_t ai_pccPath ) ;
		~ClFileFinder( void ) ;

	void	Init( m4pcchar_t ai_pccPath ) ;
	void	End( void ) ;

	m4return_t FindNext( ClFile& ao_roFile ) ;

protected:

	void		_Destroy( void ) ;
#ifdef _UNIX
	m4return_t	_CheckWildCards( m4pcchar_t ai_pccTemplate, m4pcchar_t ai_pccFileName ) ;
#endif
	
	m4pchar_t	m_pcANSIPath ;
	m4pchar_t	m_pcANSIDirectory ;
	m4pchar_t	m_pcANSITemplate ;
	void		*m_poHandle ;
};


#endif

