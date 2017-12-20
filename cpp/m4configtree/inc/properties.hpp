
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           properties.cpp
// File:             Properties.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:2/7/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    Define the class for the properties structure
//
//
//==============================================================================

#ifndef _PROPERTIES_HPP_

#define _PROPERTIES_HPP_


//----------------------------------------------------------------------------
//			list of posible properties
//----------------------------------------------------------------------------


// Function call constants



#define ENDSTR	-1	//	terminator for properties strings


#define MAXBUFFERSIZE	65534

#define MAXNAMEPROP	300

#include <m4stl.hpp>
#include "m4types.hpp"
#include "m4define.hpp"
#include "m4configtree_dll.hpp"



//----------------------------------------------------------------------------
//	Vector of characters for property name
//----------------------------------------------------------------------------

typedef vector <char> PropertyName; 

//----------------------------------------------------------------------------
//		Vector of characters for property values
//----------------------------------------------------------------------------

typedef vector <m4char_t> PropertyValue;

//----------------------------------------------------------------------------
//		map that join each property to its value
//----------------------------------------------------------------------------

typedef multimap <PropertyName , PropertyValue , less<PropertyName> > tProperties;

typedef tProperties::iterator tPropIter;

//----------------------------------------------------------------------------
//This function return the leng of a property value ended with -1
//----------------------------------------------------------------------------
m4int32_t StringLen ( m4pchar_t ai_szString );


//----------------------------------------------------------------------------
//			Class for the pool of properties
//----------------------------------------------------------------------------

	class M4_DECL_M4CONFIGTREE ClProperties
	{

	protected:

		tProperties m_STLproperties;	//pool of properties

	public:

		~ClProperties();
		m4return_t SetStringProperties ( m4pchar_t );
		m4return_t GetProperty( m4pchar_t , m4pchar_t , m4int32_t);
		m4return_t GetProperty( m4pchar_t , m4int32_t & );
		m4return_t GetpProperty(m4pchar_t ai_szProperty , m4pchar_t & ao_szValue ) ;
		m4return_t GetProperties ( m4pchar_t , m4uint32_t );
		m4return_t SetProperty(m4pchar_t , m4pchar_t , m4int32_t);
		m4return_t SetProperty(m4pchar_t , m4int32_t);
		m4return_t MatchFirstProp (m4pchar_t , m4pchar_t , m4uint32_t , m4pchar_t , m4uint32_t , m4uint32_t & , void * & );
		m4return_t MatchNextProp (m4pchar_t , m4pchar_t , m4uint32_t , m4pchar_t , m4uint32_t , m4uint32_t &, void * &);
		m4bool_t IsProperty ( m4pchar_t ai_szProperty ) ;
		m4return_t Vaciar();
	};

#endif
