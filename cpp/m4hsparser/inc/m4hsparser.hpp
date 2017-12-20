//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                m4hsparser.hpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Language:            C++
// Operating System:    UNIX, WIN32
// Date:                22/2/01
// Design Document:     
//
//
// Definition:
//
//    Definción de los elementos exportados por la librería m4hsparser
//
//
//==============================================================================


#ifndef __M4HSPARSER_HPP__
#define __M4HSPARSER_HPP__

#include "m4hsparser_dll.hpp"

#include "m4types.hpp"
#include "m4stl.hpp"

class M4_DECL_M4HSPARSER M4HsParser
{

public:

	M4HsParser ();
	virtual ~M4HsParser();

	m4return_t SentenceMerge( istream &ai_oChild, istream &ai_oParent, ostream &aio_oResult );
};


#endif //__M4HSPARSER_HPP__

