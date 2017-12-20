//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             gstring.h
// Project:          M4APIE
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//   Declaration of gstring
//
//==============================================================================
#include <atlbase.h>
#include "m4types.hpp"

#ifndef __GSTRING_H_
#define __GSTRING_H_

class gstring
{
public:
	// Default constructor and destructor
	gstring();
	virtual ~gstring();

	// Constructors
	gstring(m4pcchar_t sz);
	gstring(const BSTR& ai_st);

	// Type conversion operators
	operator BSTR();
	operator m4pchar_t();
	operator bool();
	bool operator!();

	// Assignment operators
	gstring& operator=(m4pcchar_t ai_sz);
	gstring& operator=(const BSTR& ai_st);

private:
	void BstrInit(const BSTR& ai_st);
	void Strdup(m4pcchar_t sz);
	void End(void);

	m4pchar_t m_sz;
};
#endif
