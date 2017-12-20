//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           ClBSearch.hpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    Virtual machine de nivel 1
////
//==============================================================================

//#include "vmachin1.h"
#include "m4dm_dll.hpp"
#include "m4stl.hpp"
#include "m4log.hpp"
#include "m4var.hpp"
#include "handles.hpp"
#include "index.hpp"

//#include "dm.hpp"
//#include "channel.hpp"
//#include "access.hpp"
//#include "node.hpp"
//#include "register.hpp"
//#include "item.hpp"
//#include "trad.h"
//#include "execontx.hpp"
//#include "intln4.hpp"

class _ClRegister_Base;
class ClCompiledMCR;


#ifndef _ClBSearch_HPP
#define _ClBSearch_HPP

typedef	m4int8_t ( *m4BSCompareVariant_f )( const m4VariantType *, const m4VariantType * ) ;

class ClBSearchIndex;

class ClSize;

class M4_DECL_M4DM ClBSItemIndex{
private:
    ClHandle					hHandle ;			//Handle del item
    ClItemIndex                 iIndex;             //indice del item

    m4int8_t					iIndexOrder;		//Search Way
    m4uint8_t					iIndexFunction;		//Search Function
    m4BSCompareVariant_f		pfCompareVar;    

public:
	m4int8_t VarCompare( const m4VariantType &pstVariant1, const m4VariantType &pstVariant2 ) const
    {
        return pfCompareVar (&pstVariant1, &pstVariant2);
    }

    ClHandle GetItemHandle (void) const
    {
        return hHandle;
    }

    m4int8_t GetOrder (void) const
    {
        return iIndexOrder;
    }

    m4int8_t GetFunction (void) const
    {
        return iIndexFunction;
    }

	m4return_t GetSize(ClSize &ao_size);

friend class ClBSearchIndex;

};

/*
class ClStringItemIndex:public ClBSItemIndex{

public:
	m4int8_t VarCompare( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2 );

};


class ClDoubleItemIndex:public ClBSItemIndex{

public:
	m4int8_t VarCompare( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2 );

};
*/


class M4_DECL_M4DM ClBSearchIndex{

private:
	ClBSItemIndex * m_poItemIndex;
	//m4VariantType * m_pvArguments;
	m4uint32_t	m_iNumItemIndex;
    m4uint32_t  m_Id;

public:
	ClBSearchIndex ( void );
	~ClBSearchIndex ( void );

	m4return_t Init ( ClCompiledMCR *poCMCR, m4uint32_t hIndex, m4uint32_t ai_id = 0/*para que siga siendo compatible*/);
        //normal

    m4return_t Init (_ClRegister_Base * ai_pClRegister, m4int16_t ai_iNumberItems, m4pchar_t * ai_pItemsNames, m4uint8_t * ai_pOrders);
        //se usa para las ordenaciones que se definen dinámicamente

    m4return_t Update (ClCompiledMCR *poCMCR);

    m4int8_t CompareIndex ( _ClRegister_Base & ai_poRegister, m4VariantType *ai_pstArguments, m4uint8_t *ai_pfunctions); 

    ClBSItemIndex *Get (m4uint32_t ai_iPos) const;
    m4uint32_t Count (void) const;

    m4uint32_t Id (void) const
    {
        return m_Id;
    }
	m4return_t GetSize(ClSize &ao_size);

};

#endif
