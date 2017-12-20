//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                m4var.inl   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//
//
//==============================================================================



#ifdef _WINDOWS
#include "cvdate.h"
#endif

#include "m4stl.hpp"


// Copy constructor
inline  m4VariantType::m4VariantType( const m4VariantType &ai_var )
{
	m_bOwner = M4_FALSE;
	Set( ai_var );
}


inline  m4VariantType::operator m4double_t( void ) const
{
	if( Type == M4CL_CPP_TYPE_NUMBER || Type == M4CL_CPP_TYPE_DATE )
	{
		return Data.DoubleData;
	}
	else
	{
		return 0;
	}
}


inline  m4VariantType::operator m4pchar_t( void ) const
{
	if( Type == M4CL_CPP_TYPE_STRING_VAR || Type == M4CL_CPP_TYPE_STRING_SYS )
	{
		return Data.PointerChar;
	}
	else
	{
		return NULL;
	}
}


//////////////////////////////////////
//
// Windows specific
//
#ifdef _WINDOWS

//
// Copy constructor with VARIANT
//
inline  m4VariantType::m4VariantType( VARIANT& ai_VARIANT )
{
	m_bOwner = M4_FALSE;
	SetVARIANT( ai_VARIANT );
}


inline  m4VariantType & m4VariantType::operator = ( VARIANT &ai_VARIANT )
{
	SetVARIANT( ai_VARIANT );
	return *this;
}

#endif
//
// End of Windows specific
/////////////////////
