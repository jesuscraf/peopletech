//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             gstring.cpp
// Project:          M4APIE
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of gstring
//
//==============================================================================

#include "gstring.h"
#include "m4unicode.hpp"


gstring::gstring()
{
	m_sz = NULL;
}

gstring::~gstring()
{
	End();
}

gstring::gstring(m4pcchar_t sz)
{
	Strdup(sz);
}

gstring::gstring(const BSTR& ai_st)
{
	BstrInit(ai_st);
}

// This should be int
gstring::operator bool()
{
	return (m_sz != NULL);
}

bool gstring::operator!()
{
	return (m_sz == NULL);
}

gstring::operator BSTR()
{
	// UNICODE VB
	BSTR st = M4CppToVB(m_sz);
	return st;
}

gstring::operator m4pchar_t()
{
	// Simply return the contained pointer 
	return m_sz;
}

gstring&	gstring::operator=(m4pcchar_t ai_sz)
{
	End();
	Strdup(ai_sz);
	return *this;
}

gstring&	gstring::operator=(const BSTR& ai_st)
{
	End();
	BstrInit(ai_st);
	return *this;
}

void	gstring::Strdup(m4pcchar_t sz)
{
	// If there is a string
	if (sz) 
	{
		// if we can allocate memory
		if (m_sz = new m4char_t[strlen(sz)+1])
		{
			strcpy(m_sz, sz);
		}
	}
    else
	{
        m_sz = NULL;
	}
}

void	gstring::End(void)
{
	if (m_sz) 
	{
		delete [] m_sz;
		m_sz = NULL;
	}
}

void	gstring::BstrInit(const BSTR& ai_st)
{
	// UNICODE VB
	m_sz = M4VBToCpp(ai_st);
}
