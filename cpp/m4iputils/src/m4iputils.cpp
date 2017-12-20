//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4iputils.dll
// File:             m4iputils.cpp
// Project:          IP Address utils
// Author:           Meta Software M.S. , S.A
// Date:             24/08/16
// Language:         C++
// Operating System: WINDOWS, UNIX
//==============================================================================

#include "m4iputils.hpp"
#include "m4iputilserrors.hpp"
#include "m4log.hpp"


#define	RET_IP_MATCHES_RANGE			0
#define	RET_IP_DOES_NOT_MATCH_RANGE		1
#define	RET_IP_UNRECOGNIZED				2
#define	RET_RANGE_IP_UNRECOGNIZED		3

// ---------------------------------------------------------
// Public LN4 function.
// Validates if an address belongs to a IP addresses range.
//
//	arg_0:	IP address to validate.
//	arg_1:	IP range.
//
// LN4 return:	0:	IP belongs to the range.
//				1:	IP does not belongs to the range.
//				2:	IP unrecognized.
//				3:	Range IP unrecognized.
//
// Return:		M4_ERROR / M4_SUCCESS.
// ---------------------------------------------------------
m4return_t CheckAddressInRange( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	char			*pcAddress;
	ClIPRange*		pRange;
	ClIPAddress*	pAddress;
	bool			bRet;


	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;

	if( ai_iLongArg != 2 )
	{
		SETCODEF( M4_ERR_BAD_NUM_ARGS, ERRORLOG, "#*s1*#%s#", "CheckAddressInRange" );
		return M4_ERROR;
	} 

	if( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR || ai_pvArg[0].Data.PointerChar == NULL )
	{
		SETCODEF( M4_ERR_BAD_ARG_TYPE, ERRORLOG, "#*s1*#%s#", "CheckAddressInRange" );
		return M4_ERROR;
	}
	else
	{
		pcAddress = ai_pvArg[0].Data.PointerChar;
	}

	if( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR || ai_pvArg[1].Data.PointerChar == NULL )
	{
		SETCODEF( M4_ERR_BAD_ARG_TYPE, ERRORLOG, "#*s1*#%s#", "CheckAddressInRange" );
		return M4_ERROR;
	}


	pRange = ClIPRange::GetInstance( ai_pvArg[1].Data.PointerChar );

	if( pRange == NULL )
	{
		ai_pvReturn.Data.DoubleData = RET_RANGE_IP_UNRECOGNIZED;
		return M4_SUCCESS;
	}

	pAddress = ClIPAddress::GetInstance( pcAddress );

	if( pAddress == NULL )
	{
		ai_pvReturn.Data.DoubleData = RET_IP_UNRECOGNIZED;

		delete pRange;
		return M4_SUCCESS;
	}

	iRet = pRange->IsAddressInRange( pAddress, bRet );

	if( iRet == M4_SUCCESS )
	{
		if( bRet == M4_TRUE )
		{
			ai_pvReturn.Data.DoubleData = RET_IP_MATCHES_RANGE;
		}
		else
		{
			ai_pvReturn.Data.DoubleData = RET_IP_DOES_NOT_MATCH_RANGE;
		}
	}
	else
	{
		SETCODEF( M4_ERR_VALIDATING_IP, ERRORLOG, "#*s1*#%s#", "CheckAddressInRange" );
		return M4_ERROR;
	}

	delete pRange;
	delete pAddress;


	return M4_SUCCESS;
}


// ---------------------------------------------------------
// Public LN4 function.
// Checks an address format.
//
//	arg_0:	IP address to check.
//
// LN4 return:	M4_TRUE:	Format Ok.
//				M4_FALSE:	Bad format.
//
// Return:		M4_ERROR / M4_SUCCESS.
// ---------------------------------------------------------
m4return_t CheckAddressFormat( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	ClIPAddress*	pAddress;


	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;

	if( ai_iLongArg != 1 )
	{
		SETCODEF( M4_ERR_BAD_NUM_ARGS, ERRORLOG, "#*s1*#%s#", "CheckAddressFormat" );
		return M4_ERROR;
	} 

	if( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR || ai_pvArg[0].Data.PointerChar == NULL )
	{
		SETCODEF( M4_ERR_BAD_ARG_TYPE, ERRORLOG, "#*s1*#%s#", "CheckAddressFormat" );
		return M4_ERROR;
	}


	pAddress = ClIPAddress::GetInstance( ai_pvArg[0].Data.PointerChar );

	if( pAddress == NULL )
	{
		ai_pvReturn.Data.DoubleData = M4_FALSE;
	}
	else
	{
		delete pAddress;
		ai_pvReturn.Data.DoubleData = M4_TRUE;
	}


	return M4_SUCCESS;
}


// ---------------------------------------------------------
// Public LN4 function.
// Checks an address format.
//
//	arg_0:	IP Range to check.
//
// LN4 return:	M4_TRUE:	Format Ok.
//				M4_FALSE:	Bad format.
//
// Return:		M4_ERROR / M4_SUCCESS.
// ---------------------------------------------------------
m4return_t CheckRangeFormat( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	ClIPRange*	pRange;


	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;

	if( ai_iLongArg != 1 )
	{
		SETCODEF( M4_ERR_BAD_NUM_ARGS, ERRORLOG, "#*s1*#%s#", "CheckRangeFormat" );
		return M4_ERROR;
	} 

	if( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR || ai_pvArg[0].Data.PointerChar == NULL )
	{
		SETCODEF( M4_ERR_BAD_ARG_TYPE, ERRORLOG, "#*s1*#%s#", "CheckRangeFormat" );
		return M4_ERROR;
	}


	pRange = ClIPRange::GetInstance( ai_pvArg[0].Data.PointerChar );

	if( pRange == NULL )
	{
		ai_pvReturn.Data.DoubleData = M4_FALSE;
	}
	else
	{
		delete pRange;
		ai_pvReturn.Data.DoubleData = M4_TRUE;
	}


	return M4_SUCCESS;
}


// ---------------------------------------------------------
// Public LN4 function.
// Gets the IP range limits.
//
//	arg_0:	Input. Range.
//	arg_1:	Output. Start of range.
//	arg_2:	Output. End of range.
//
// LN4 return:	M4_ERROR / M4_SUCCESS.
//
// Return:		M4_ERROR / M4_SUCCESS.
// ---------------------------------------------------------
m4return_t GetRangeLimits( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	ClIPRange*	pRange;
	m4string_t	sStart, sEnd;
	m4return_t	iRet;


	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;

	if( ai_iLongArg != 3 )
	{
		SETCODEF( M4_ERR_BAD_NUM_ARGS, ERRORLOG, "#*s1*#%s#", "GetRangeLimits" );
		return M4_ERROR;
	} 


	pRange = ClIPRange::GetInstance( ai_pvArg[0].Data.PointerChar );

	if( pRange == NULL )
	{
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}

	iRet = pRange->GetLimits( sStart, sEnd );

	if( iRet == M4_ERROR )
	{
		ai_pvReturn.Data.DoubleData = M4_ERROR;
	}
	else
	{
		ai_pvReturn.Data.DoubleData = M4_SUCCESS;

		ai_pvArg[1].SetString( ( char* )sStart.c_str() );
		ai_pvArg[2].SetString( ( char* )sEnd.c_str() );
	}

	delete pRange;
	

	return M4_SUCCESS;
}


// ---------------------------------------------------------
// Public LN4 function.
// Gets the IP as decimal string.
//
//	arg_0:	Input. IP.
//	arg_1:	Output. Decimal IP string.
//
// LN4 return:	M4_ERROR / M4_SUCCESS.
//
// Return:		M4_ERROR / M4_SUCCESS.
// ---------------------------------------------------------
m4return_t GetIPDecString( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	ClIPAddress*	pAddress;
	m4pchar_t		pcDecimal;


	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvArg[1].SetNull();

	if( ai_iLongArg != 2 )
	{
		SETCODEF( M4_ERR_BAD_NUM_ARGS, ERRORLOG, "#*s1*#%s#", "GetIPDecString" );
		return M4_ERROR;
	} 

	if( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR || ai_pvArg[0].Data.PointerChar == NULL )
	{
		SETCODEF( M4_ERR_BAD_ARG_TYPE, ERRORLOG, "#*s1*#%s#", "GetIPDecString" );
		return M4_ERROR;
	}

	pAddress = ClIPAddress::GetInstance( ai_pvArg[0].Data.PointerChar );

	if( pAddress == NULL )
	{
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}

	pcDecimal = pAddress->GetDecimal();
	
	ai_pvArg[1].SetString( pcDecimal );

	delete pcDecimal;
	delete pAddress;

	ai_pvReturn.Data.DoubleData = M4_SUCCESS;


	return M4_SUCCESS;
}
