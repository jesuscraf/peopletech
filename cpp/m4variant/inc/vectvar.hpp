//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              M4Variant
// File:                vectvar.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                04-03-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//			Variants   Vector.
//
//
//==============================================================================
#ifndef __VECTORVAR_HPP
#define __VECTORVAR_HPP

#include "m4variant_dll.hpp"
#include "m4var.hpp"
#include "m4stl.hpp"

typedef vector < m4VariantType >	VectorVariants;
typedef VectorVariants::iterator	itVectorVariants;

class M4_DECL_M4VARIANT ClVectorVariants{

public:


	
	ClVectorVariants(){
		m_iNumElem = 0;
	}

	ClVectorVariants (m4int32_t ai_nElem)
	{
		m4VariantType vNull;
		
		vNull.Type = M4CL_CPP_TYPE_NULL;
		m_vvVariants.reserve(ai_nElem);
		for (m4int32_t i = 0 ; i < ai_nElem ; i++) {
			Insert (&vNull);
		}
		m_iNumElem = ai_nElem;
	}

	~ClVectorVariants (){

		if ( !m_vvVariants.empty() )
		{
			m_vvVariants.clear();
		}
	}
	
	ClVectorVariants &Dec (ClVectorVariants &vvOper);        
	ClVectorVariants &Mult (m4VariantType &vOper);        

	ClVectorVariants &Sum(ClVectorVariants &vvOper, m4bool_t ai_bConcatStrings);
	ClVectorVariants &Div (m4VariantType &vOper);
	ClVectorVariants &Max (ClVectorVariants &vvOper);
	ClVectorVariants &Min (ClVectorVariants &vvOper);

	void SetDouble(m4double_t ai_double);
    void SetNull (void);


	m4VariantType &operator[](m4int32_t i)
	{
		return m_vvVariants[i];
	}

	m4int32_t GetSize () {return m_vvVariants.size();}

	void FreeData ( void );
	void Insert (m4VariantType *ai_pvinsert)
	{
		m_vvVariants.insert( m_vvVariants.end() , *ai_pvinsert) ;
		m_iNumElem++;
	}
		

	VectorVariants	m_vvVariants;
	m4int32_t		m_iNumElem;
};


#endif

