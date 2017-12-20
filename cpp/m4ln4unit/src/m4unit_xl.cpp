//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ln4unit
// File:             m4unit_xl.cpp
// Project:			 m4ln4unit
// Author:           Meta Software M.S. , S.A
// Date:			 04/03/2016
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines Unit Testing API and utilities.
//
//==============================================================================

#include <m4unit_xl.hpp>
#include <m4xlutapi.hpp>
#include <m4xlapierrors.hpp>
#include <m4log.hpp>

// -----------------------------------------------------------------------------
// Compares two Excel workbook structs.
// 
//	Arg_0: File 1 path.
//	Arg_1: File 2 path.
// -----------------------------------------------------------------------------
m4return_t M4CmpXlsWb( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	m4bool_t	bEq;

	
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER; 
	ai_pvReturn.Data.DoubleData = M4_FALSE;
	
	if( ai_iLongArg != 2 )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWb" );
		return M4_ERROR;
	}

	if( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWb" );
		return M4_ERROR;
	}

	if( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWb" );
		return M4_ERROR;
	}

	iRet = CmpWbStruct( ai_pvArg[0].Data.PointerChar, ai_pvArg[1].Data.PointerChar, ai_pvContext, bEq );

	ai_pvReturn.Data.DoubleData = bEq;

	
	return iRet;
}


// -----------------------------------------------------------------------------
// Compares two Excel worksheets.
// 
//	Arg_0: File 1 path.
//	Arg_1: [Optional] File 2 path.
//	Arg_2: Worksheet 1 index.
//	Arg_3: Worksheet 2 index.
//	Arg_4: First row.
//	Arg_5: First column.
//	Arg_6: [Optional] Last row.
//	Arg_7: [Optional] Last column.
//	Arg_8: [Optional] List of cells to ignore.
// -----------------------------------------------------------------------------
m4return_t M4CmpXlsWs( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	m4bool_t	bEq;


	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER; 
	ai_pvReturn.Data.DoubleData = M4_FALSE;
	
	if( ai_iLongArg != 9 )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWs" );
		return M4_ERROR;
	}

	if( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWs" );
		return M4_ERROR;
	}

	if( ai_pvArg[2].Type != M4CL_CPP_TYPE_NUMBER )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWs" );
		return M4_ERROR;
	}

	if( ai_pvArg[3].Type != M4CL_CPP_TYPE_NUMBER )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWs" );
		return M4_ERROR;
	}
	
	if( ai_pvArg[4].Type != M4CL_CPP_TYPE_NUMBER )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWs" );
		return M4_ERROR;
	}

	if( ai_pvArg[5].Type != M4CL_CPP_TYPE_NUMBER )
	{
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "M4CmpXlsWs" );
		return M4_ERROR;
	}


	iRet = CmpWsContent( ai_pvArg[0].Data.PointerChar, ai_pvArg[1].Data.PointerChar, ( int )ai_pvArg[2].Data.DoubleData, ( int )ai_pvArg[3].Data.DoubleData, ( int )ai_pvArg[4].Data.DoubleData, ( int )ai_pvArg[5].Data.DoubleData, ( int )ai_pvArg[6].Data.DoubleData, ( int )ai_pvArg[7].Data.DoubleData, ai_pvArg[8].Data.PointerChar, ai_pvContext, bEq );

	ai_pvReturn.Data.DoubleData = bEq;

	
	return iRet;
}

