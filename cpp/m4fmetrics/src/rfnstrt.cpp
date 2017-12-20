//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             RFNStrt.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:27/01/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "fontm.h"

#include "fontmdf.cpp"

#include "rfnstrt.h"


const m4double_t ClFONT_M_RFNStruct::ESC_FACTOR_COMPR=0.6; 

const m4char_t *ClFONT_M_RFNStruct::RFN_FILE_ID="Report Viewer Fonts Meta4" ;


m4int16_t ClFONT_M_RFNStruct::PackSIndex( m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline )
{	
	return  0 | 
		(ai_bSwBold ? BOLD_FLAG : 0) | 
		(ai_bSwItalic ? ITALIC_FLAG : 0) | 
		(ai_bSwUnderline ? UNDERLINE_FLAG : 0) ; 
}

void ClFONT_M_RFNStruct::UnpackSIndex( m4int16_t ai_Index, m4bool_t *ai_bSwBold, m4bool_t *ai_bSwItalic, m4bool_t *ai_bSwUnderline )
{
	*ai_bSwBold = M4_BOOL( ai_Index&BOLD_FLAG ); 
	*ai_bSwItalic = M4_BOOL( ai_Index&ITALIC_FLAG );
	*ai_bSwUnderline = M4_BOOL( ai_Index&UNDERLINE_FLAG );
}

m4int16_t ClFONT_M_RFNStruct::PackDvcFlags( 
		m4bool_t ai_bSwPCL, m4bool_t ai_bSwPSC, 
		m4bool_t ai_bSwRVW, m4bool_t ai_bSwESCP ) 
{
	return  0 | 
		(ai_bSwPCL ? PCL_FLAG : 0) | 
		(ai_bSwPSC ? PSC_FLAG : 0) | 
		(ai_bSwRVW ? RVW_FLAG : 0) |
		(ai_bSwESCP ? ESCP_FLAG : 0) ; 
}

m4int16_t ClFONT_M_RFNStruct::PackESCP_Flags( m4bool_t ai_bSw2Y, m4bool_t ai_bSw2X, m4bool_t ai_bSwCompr ) 
{
	return  0 | 
		(ai_bSw2Y ? ESCP_2Y_FLAG : 0) | 
		(ai_bSw2X ? ESCP_2X_FLAG : 0) | 
		(ai_bSwCompr ? ESCP_COMPR_FLAG : 0) ; 
}


void ClFONT_M_RFNStruct::UnpackESCP_Flags( m4int16_t ai_Value, m4bool_t *ai_bSw2Y, m4bool_t *ai_bSw2X, m4bool_t *ai_bSwCompr ) 
{
	*ai_bSw2Y = M4_BOOL( ai_Value&ESCP_2Y_FLAG ); 
	*ai_bSw2X = M4_BOOL( ai_Value&ESCP_2X_FLAG );
	*ai_bSwCompr = M4_BOOL( ai_Value&ESCP_COMPR_FLAG );
}


m4char_t *ClFONT_M_RFNStruct::GetDvcTraceName(m4int16_t ai_Dvc) 
{
	switch(ai_Dvc)
	{
	case ClFONT_M_RFNStruct::PCL:
		return "PCL" ;
		break;
	case ClFONT_M_RFNStruct::PSC:
		return "PSC" ;
		break;
	case ClFONT_M_RFNStruct::RVW:
		return "RVW" ;
		break;
	case ClFONT_M_RFNStruct::ESCP:
		return "ESCP" ;
		break;
	default:
		goto exit;
		break ;
	}

exit:

	return "Unknown Dvc Type" ;
}
