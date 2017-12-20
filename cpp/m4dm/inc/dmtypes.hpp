#ifndef _DMTYPES_HPP_
#define _DMTYPES_HPP_

#include "m4dm_dll.hpp"
#include "m4var.hpp"
#include "index.hpp"


#define M4_ACUMULATE_TIS_SEPARATOR					';'

#define M4_NODE_SCHERRORS_NODE_CODES                "ERRORS"
#define M4_NODE_SCHERRORS_ITEM_GET_ERROR            "GETERROR"
#define M4_NODE_SCHERRORS_ITEM_GET_ERRORS			"GETERRORS"


typedef struct M4_DECL_M4DM StFunctionCmp_tag {

	ClItemIndex			m_iIndex;
	ClItemIndex			m_iSecondIndex;
	m4VariantType		m_vVariant;
	m4CompareVariant_f	m_pfcomp;
	
	void	Fill(ClItemIndex ai_Index,m4VariantType &ai_vVariant,m4CompareVariant_f ai_pfcomp,ClItemIndex ai_iSecondIndex=0);
}StFunctionCmp_t;


extern  m4CompareVariant_f M4_DECL_M4VARIANT g_apfM4VariantCompareFunctions[ M4CL_MAX_VARIANT_COMPARE_FUNCTION ] ;
typedef struct StFindRegisterCol_tag
{

    ClItemIndex             iItemIndex ;
    m4VariantType           *pvValue ;
    m4CompareVariant_f      pfFunction ;

} StFindRegisterCol_t ;

#endif	// _DMTYPES_HPP_


