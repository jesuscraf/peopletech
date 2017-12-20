
//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cataloguer
// File:                m4patternreader.hpp   
// Project:             PeopleNet      
// Author:              Meta Software M.S. , S.A
// Date:                12-02-2016
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the class for reading catalogue patterns
//
//==============================================================================


#include "m4types.hpp"
#include "m4stl.hpp"


#ifndef __M4PATTERNREADER_HPP__
#define __M4PATTERNREADER_HPP__


#ifdef _UNIX
	#define	M4_CATALOGUER_MAX_PATH			PATH_MAX
	#define	M4_DIRECTORY_SEPARATOR			"/"
#else
	#define	M4_CATALOGUER_MAX_PATH			MAX_PATH
	#define	M4_DIRECTORY_SEPARATOR			"\\"
#endif



//=================================================================================
//
// M4PatternReader
//
// This class reads pattern lists from catalogue pattern
//
//=================================================================================

class	M4PatternReader
{
public:
	static m4return_t	ReadPatterns( m4pcchar_t ai_pccDirectory, vector<m4pchar_t>& ai_rvStartIncludes, vector<m4pchar_t>& ai_rvEndIncludes, vector<m4pchar_t>& ai_rvStartExcludes, vector<m4pchar_t>& ai_rvEndExcludes ) ;
};


#endif

