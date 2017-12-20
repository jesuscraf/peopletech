//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             regtot.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             22/09/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    register totalizer
//
//
//==============================================================================

#ifndef __REGTOT_HPP__
#define __REGTOT_HPP__

#include "m4var.hpp"
#include "index.hpp"
#include "totalize.hpp"

class ClHandle ;


///////////////////////////////////////////////////////////////////////////////
//
// ClRegTotalize
//

class M4_DECL_M4DM ClRegTotalize: public ClTotalize < _ClRegister_Base, m4VariantType, ClGetValue>
{
public:
	ClRegTotalize()
	: Index(M4DM_EOF_INDEX)
	{
	}

	void Init( _ClRegister_Base * ai_tIterator )
	{
		ClTotalize < _ClRegister_Base , m4VariantType, ClGetValue>::Init( ai_tIterator );
	}

	m4return_t Get( m4VariantType &ao_vTotValOut, m4char_t TotType = -1 );
	m4return_t GetNoConcat( m4VariantType &ao_vTotValOut, m4char_t TotType = -1 );

	m4return_t Count( m4VariantType &ao_vTotValOut );
    m4return_t Sum( m4VariantType &ao_vTotValOut, m4bool_t ai_bConcatStrings );
	m4return_t Avg( m4VariantType &ao_vTotValOut );
	m4return_t Max( m4VariantType &ao_vTotValOut );
	m4return_t Min( m4VariantType &ao_vTotValOut );
	m4return_t First( m4VariantType &ao_vTotValOut );
	m4return_t Last( m4VariantType &ao_vTotValOut );
	m4return_t WeightedSum( m4VariantType &ao_vTotValOut);

    ClRegTotalize & operator[] (const ClItemIndex &ai_itemindex);
	ClRegTotalize & operator[] (const ClHandle &ai_itemhandle);
    ClRegTotalize & operator[] (m4pcchar_t ai_itemid);

	m4return_t GetSize(ClSize &ao_size);


    ClItemIndex Index;

protected:

	m4return_t _Get( m4VariantType &ao_vTotValOut, m4char_t TotType, m4bool_t ai_bConcatStrings );


};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#endif // __REGTOT_HPP__
