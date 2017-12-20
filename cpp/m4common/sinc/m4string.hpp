//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4string.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/1/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
#ifndef __M4STRING__HPP__
#define __M4STRING__HPP__

#include "m4stl.hpp"
#include <stdio.h>

//	ADD EMN	991117	m4string_t

typedef string	m4string_t;

typedef string	M4ClStringBase;


class M4ClString : public M4ClStringBase 
{

	public:

		M4ClString () { ; };
		M4ClString ( char * ai_szString ) : M4ClStringBase ( ai_szString ) { ; };
		M4ClString ( const char * ai_szString ) : M4ClStringBase ( ai_szString ) { ; };
		~M4ClString () { ; };

		friend M4ClString & operator += ( M4ClString & ai_this  , m4int16_t  ai_iOperator )
		{
			char szBuffer [ 128 ];

			sprintf ( szBuffer , "%d" , ai_iOperator );

			ai_this += szBuffer ;

			return ai_this;
		};	

		operator char *  ( )
		{
			return ( char*)(*this).c_str();
		};

		operator const char * ( )
		{
			return (*this).c_str();
		};
			
};

#ifdef  _UNIX
#include "unix/m4uxstr.hpp"
#endif

#endif

