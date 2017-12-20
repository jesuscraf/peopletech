//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                m4varbase.inl   
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



// Default constructor
inline  m4VariantTypeBase::m4VariantTypeBase( void )
{
	m_bOwner = M4_FALSE;
	IsUser = M4_FALSE;
	Type = M4CL_CPP_TYPE_NULL;
	memset( &Data, 0, sizeof( m4VariantData ) );
}


// Basic types constructors
inline  m4VariantTypeBase::m4VariantTypeBase( const m4double_t &ai_double )
{
	m_bOwner = M4_FALSE;
	IsUser = M4_FALSE;
	Type = M4CL_CPP_TYPE_NUMBER;
	Data.DoubleData = ai_double;
}


inline  m4VariantTypeBase::m4VariantTypeBase( const m4pchar_t &ai_char, m4bool_t ai_bCopy )
{
	m_bOwner = (ai_bCopy && ai_char) ? M4_TRUE : M4_FALSE;
	IsUser = M4_FALSE;
	Type = M4CL_CPP_TYPE_STRING_VAR;

	if( m_bOwner )
	{
		size_t iStrSize = strlen( ai_char ) + 1;
		Data.PointerChar = new m4char_t[iStrSize];
		memcpy( Data.PointerChar, ai_char, iStrSize );
	}
	else
	{
		Data.PointerChar = ai_char;
	}
}


inline  void m4VariantTypeBase::FreeData( void )        
{
	if( m_bOwner ) 
	{
		if( Type == M4CL_CPP_TYPE_STRING_VAR || Type == M4CL_CPP_TYPE_STRING )
		{
			delete [] Data.PointerChar;
			Data.PointerChar = NULL;
		}
		else if( Type == M4CL_CPP_TYPE_STRING_SYS )
		{
			M4SysFreeString( Data.PointerChar );
			Data.PointerChar = NULL;
		}
		m_bOwner = M4_FALSE;
	}
}


inline  m4VariantTypeBase::~m4VariantTypeBase( void )
{
	FreeData();
}


inline	m4bool_t m4VariantTypeBase::GetOwner( void ) const
{
	return( m_bOwner ) ;
}

inline	void m4VariantTypeBase::SetOwner( m4bool_t ai_bOwner )
{
	m_bOwner = ai_bOwner;
}

inline  m4char_t m4VariantTypeBase::GetType( void ) const
{
	return Type;	// Return variant type
}


inline  void m4VariantTypeBase::Set( const m4VariantTypeBase &ai_var, m4bool_t ai_bCopy )        
{
	FreeData();

	IsUser = ai_var.IsUser;
	Type = ai_var.Type;

	if( ai_bCopy == M4_TRUE && ( Type == M4CL_CPP_TYPE_STRING_VAR || Type == M4CL_CPP_TYPE_STRING_SYS || Type == M4CL_CPP_TYPE_STRING ) ) //si además de quererlo owner es de tipo cadena
	{
		m_bOwner = M4_TRUE;
	}
	else 
	{
		m_bOwner = ai_var.m_bOwner;
	}
	
	if( m_bOwner )
	{
		size_t iStrSize;

		if( Type == M4CL_CPP_TYPE_STRING_VAR || Type == M4CL_CPP_TYPE_STRING )
		{
			iStrSize = strlen( ai_var.Data.PointerChar ) + 1;
			Data.PointerChar = new m4char_t[iStrSize];
			memcpy( Data.PointerChar, ai_var.Data.PointerChar, iStrSize );
		}
		else if( Type == M4CL_CPP_TYPE_STRING_SYS )
		{
			iStrSize = strlen( ai_var.Data.PointerChar ) + 1;
			Data.PointerChar = M4SysAllocString((m4uint32_t) iStrSize);
			memcpy( Data.PointerChar, ai_var.Data.PointerChar, iStrSize );
		}
		else
		{
			Data = ai_var.Data;
		}
	}
	else
	{
		Data = ai_var.Data;
	}
}


inline  m4VariantTypeBase & m4VariantTypeBase::operator = ( const m4VariantTypeBase &ai_var )        
{
	Set( ai_var );
	return *this;
}


inline  void m4VariantTypeBase::AllocCatString( m4VariantTypeBase &ai_var )
{
	if( Type == ai_var.Type && (Type & M4CL_CPP_TYPE_STRING) )
	{
		//el & es para ver si es algún tipo de cadena

		size_t iStrSize,iOrigSize;
		m4pchar_t pcStrAux;
		
		iStrSize =  strlen( ai_var.Data.PointerChar );
		iOrigSize = strlen( Data.PointerChar );
		
		if( Type == M4CL_CPP_TYPE_STRING_VAR || ai_var.Type == M4CL_CPP_TYPE_STRING )
		{
			pcStrAux = new m4char_t[ iStrSize + iOrigSize + 1 ];
			memcpy( pcStrAux, Data.PointerChar, iOrigSize );
			memcpy( pcStrAux + iOrigSize, ai_var.Data.PointerChar, iStrSize + 1 );

			if( m_bOwner )
			{
				delete [] Data.PointerChar;
			}
			Data.PointerChar = pcStrAux;
			m_bOwner = M4_TRUE;
			IsUser = IsUser || ai_var.IsUser;
		}
		else if( Type == M4CL_CPP_TYPE_STRING_SYS )
		{
			pcStrAux = M4SysAllocString( (m4uint32_t) (iStrSize + iOrigSize + 1) );
			memcpy( pcStrAux, Data.PointerChar, iOrigSize );
			memcpy( pcStrAux + iOrigSize, ai_var.Data.PointerChar, iStrSize + 1 );

			if( m_bOwner )
			{
				M4SysFreeString( Data.PointerChar );
			}
			Data.PointerChar = pcStrAux;
			m_bOwner = M4_TRUE;
			IsUser = IsUser || ai_var.IsUser;
		}
	} //si tipos == y cadena
}


inline  void m4VariantTypeBase::SetStringRef( const m4pchar_t ai_char, m4bool_t ai_bIsUser )
{
	// Liberamos el antiguo Variant
	FreeData();
	// Establecemos el nuevo Variant como String sin Owner
	IsUser = ai_bIsUser;
	m_bOwner = M4_FALSE;
	Type = M4CL_CPP_TYPE_STRING_VAR;
	Data.PointerChar = ai_char;
}


inline  void m4VariantTypeBase::SetString( const m4pchar_t ai_char, m4bool_t ai_bIsUser )
{
	// Liberamos el antiguo Variant
	FreeData();
	// Establecemos el nuevo Variant como String con Owner
	IsUser = ai_bIsUser;
	m_bOwner = M4_TRUE;
	Type = M4CL_CPP_TYPE_STRING_VAR;
	// Reservamos memoria y copiamos
	size_t iStrSize = strlen ( ai_char ) + 1;
	Data.PointerChar = new m4char_t[ iStrSize ];
	memcpy( Data.PointerChar, ai_char, iStrSize );
}


inline  void m4VariantTypeBase::SetSysString( const m4pchar_t ai_char, m4bool_t ai_bIsUser )
{
	// Liberamos el antiguo Variant
	FreeData();
	// Establecemos el nuevo Variant como String con Owner
	IsUser = ai_bIsUser;
	m_bOwner = M4_TRUE;
	Type = M4CL_CPP_TYPE_STRING_SYS;
	// Reservamos memoria y copiamos
	size_t iStrSize = strlen( ai_char ) + 1;
	Data.PointerChar = M4SysAllocString( (m4uint32_t) iStrSize );
	memcpy( Data.PointerChar, ai_char, iStrSize );
}


inline  void m4VariantTypeBase::SetDouble( m4double_t ai_double )
{
    FreeData();
	IsUser = M4_FALSE;
    Type = M4CL_CPP_TYPE_NUMBER;
    Data.DoubleData = ai_double;
}


inline  void m4VariantTypeBase::SetDate( m4date_t ai_date )
{
    FreeData();
	IsUser = M4_FALSE;
    Type = M4CL_CPP_TYPE_DATE;
    Data.DoubleData = ai_date;
}


inline  void m4VariantTypeBase::SetNull( void )
{
    FreeData();
	IsUser = M4_FALSE;
    Type = M4CL_CPP_TYPE_NULL;
    Data.DoubleData = 0;
}

