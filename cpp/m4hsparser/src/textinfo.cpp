//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                textinfo.cpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Date:                22/2/01
// Language:            C++
// Operating System:    UNIX, WIN32
// Design Document:     
//
//
// Definition:
//
//    Definición de una elemento literal en una sentencia select
//
//
//==============================================================================

#include "textinfo.hpp"
#include <malloc.h>
#include <stdlib.h>


TextInfo::TextInfo (m4pcchar_t ai_pccText)
{
	m_pcText = strdup (ai_pccText);

	m_eType = TEXTINFO;
}


TextInfo::~TextInfo()
{
	free( m_pcText );
}


m4void_t TextInfo::MyDump (ostream & ai_oStream)
{
	ai_oStream << " ";
	ai_oStream << m_pcText;
}