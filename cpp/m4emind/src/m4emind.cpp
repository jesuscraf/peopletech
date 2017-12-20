//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4emind.dll
// File:             m4emind.cpp
// Project:          eMind Single Sign-on
// Author:           Meta Software M.S. , S.A
// Date:             19/12/05
// Language:         C++
// Operating System: WINDOWS, UNIX
// Design Document:  
//
//
// Definition:
//
//    This module implements m4emind logon API interface
//
//==============================================================================


#include "m4log.hpp"
#include "m4cryptc.hpp"
#include "m4emind.hpp"
#include "m4eminderrors.hpp"

#define	M4CH_DUMMY_B2	"\x2\x3\x8\x1\x9\x6\x5\xF\xD\x8\xF\x8\xB\x4\x7\x7\xF\x6\x4\xE\x4\xE\xE\x5\xB\xE\x6\x7\xF\x3\x7\x2\xB\x6\xC\x4\xD\xB\x6\x7\x8\x3\xE\x8\xA\xB\x7\x9\x6\x6\x7\x9\xB\x6\x4\x8\xE\xF\x3\x8\x8\xC\xA\x0\x4\x2\x6\x8\x4\xC\x4\xD\x9\x2\x8\x4\x8\x8\x6\x8\xF\x1\x1\xF\xC\x1\x7\xE"
#define M4EM_PRFX "\x41\x44\x4E"

#define M4AUTHENTICODE_MAX_NUM_ARGS 3
#define M4_PASSWORD_DOES_NOT_MATCH 1

/* ------------------------------------------------------------------------------------------------ */
/* -- Authenticates eMind user																	 -- */  
/* -- Arguments (LN4):       																	 -- */
/* --      ai_pvArg[0]: User Id.																 -- */
/* --      ai_pvArg[1]: eMind user password (eMind encrypted).									 -- */
/* --      ai_pvArg[2]: User password to validate (PeopleNet encrypted).						 -- */
/* -- Return (LN4):																				 -- */
/* --      M4_SUCCESS: Authentication success.													 -- */
/* --      M4_PASSWORD_DOES_NOT_MATCH (1): Authentication denied.								 -- */
/* --      M4_ERROR: Error during authentication.												 -- */
/* --																							 -- */
/* -- Decrypts both passwords with their respectives keys and compares them in lowercase.		 -- */
/* ------------------------------------------------------------------------------------------------ */

m4return_t authenticateEMindUser(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	m4return_t iRet		   = M4_ERROR;
	m4pchar_t pc_pnDecrPwd = NULL;
	m4pchar_t pc_emDecrPwd = NULL;
	m4int_t iBufferSize    = 0;


	/* -- Check arguments ------------------------------------------------------------------------- */
	if ( ai_iLongArg < M4AUTHENTICODE_MAX_NUM_ARGS ) 
	{
		SETCODEF ( M4_EM_ERR_INVALID_NUM_ARGS, ERRORLOG, "#*s1*#%s#", "authenticateEMindUser" );
		SetReturnValue ( ai_pvReturn, M4_ERROR );
		return M4_ERROR;
	}

	if ( ai_pvArg[0].Type != M4CL_CPP_TYPE_STRING_VAR || ai_pvArg[0].Data.PointerChar[0] == '\0' )
	{
		SETCODEF ( M4_EM_ERR_INVALID_FUNC_ARG, ERRORLOG, "#*s1*#%s#%s#", "UserID", "authenticateEMindUser" );
		SetReturnValue ( ai_pvReturn, M4_ERROR );
		return M4_ERROR;
	}

	if ( ai_pvArg[1].Type != M4CL_CPP_TYPE_STRING_VAR || ai_pvArg[1].Data.PointerChar[0] == '\0' )
	{	
		SETCODEF ( M4_EM_ERR_INVALID_FUNC_ARG, ERRORLOG, "#*s1*#%s#%s#", "Password", "authenticateEMindUser" );
		SetReturnValue ( ai_pvReturn, M4_ERROR );
		return M4_ERROR;
	}

	if ( ai_pvArg[2].Type != M4CL_CPP_TYPE_STRING_VAR || ai_pvArg[2].Data.PointerChar[0] == '\0' )
	{	
		SETCODEF ( M4_EM_ERR_INVALID_FUNC_ARG, ERRORLOG, "#*s1*#%s#%s#", "PwdToValidate", "authenticateEMindUser" );
		SetReturnValue ( ai_pvReturn, M4_ERROR );
		return M4_ERROR;
	}
		
	SETCODEF ( M4_EM_DBG_AUTH_USER, DEBUGINFOLOG, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );	

	/* -- Decrypt password to validate ------------------------------------------------------------ */
	iRet = decryptPeopleNetUsrPwd ( ai_pvArg[2].Data.PointerChar, pc_pnDecrPwd );

	if ( iRet != M4_SUCCESS )
	{
		SETCODEF ( M4_EM_ERR_DECR_PN_PWD, ERRORLOG, "#*s1*#" );
		iRet = M4_ERROR;
	}
	else
	{

		/* -- Decrypt eMind stored user password -------------------------------------------------- */
		iRet = decryptEMindUsrPwd ( ai_pvArg[0].Data.PointerChar, ai_pvArg[1].Data.PointerChar, pc_emDecrPwd );

		if ( iRet != M4_SUCCESS )
		{
			SETCODEF ( M4_EM_ERR_DECR_EM_PWD, ERRORLOG, "#*s1*#" );
			iRet = M4_ERROR;
		}
		else
		{

			/* -- Check if passwords match ignoring case ------------------------------------------ */
			iRet = stricmp ( pc_pnDecrPwd, pc_emDecrPwd );

			if ( iRet != 0 )
			{		
				iRet = M4_PASSWORD_DOES_NOT_MATCH;
				SETCODEF ( M4_EM_ERR_AUTH, ERRORLOG, "#*s1*#" );
			}
			else
			{
				SETCODEF ( M4_EM_DBG_MATCH_RESULT, DEBUGINFOLOG, "#*s1*#%s#", "Success" );
			}
		
		}
	}

	if ( pc_pnDecrPwd != NULL )
	{
		delete[] pc_pnDecrPwd;
	}
	if ( pc_emDecrPwd != NULL )
	{
		delete[] pc_emDecrPwd;
	}

	SetReturnValue ( ai_pvReturn, iRet );

	return M4_SUCCESS;

}


/* ------------------------------------------------------------------------------------------------ */
/* -- Decrypts user password with eMind key.                             						 -- */
/* -- Args:																						 -- */
/* --      ai_pccUsr: User Id.																	 -- */
/* --      ai_pccPwd: Password to decrypt.														 -- */
/* --      ao_pcDecrPwd: Output decrypted password.												 -- */
/* -- Return:																					 -- */
/* --      M4_SUCCESS/M4_ERROR																	 -- */
/* --                                   														 -- */
/* -- Allocates memory for ao_pcDecrPwd being the caller who must free it.                       -- */
/* ------------------------------------------------------------------------------------------------ */

m4return_t decryptEMindUsrPwd ( m4pcchar_t ai_pccUsr, m4pcchar_t ai_pccPwd, m4pchar_t& ao_pcDecrPwd )
{

	m4return_t	iRet = M4_ERROR;
	m4pchar_t	pcKey = NULL;
	m4int_t		iKeyLen = 0;
	m4pchar_t	pcBuffer = NULL;
	m4int_t		iBuffSzReq = 0;

	
	/* -- Check arguments ------------------------------------------------------------------------- */
	if ( ai_pccUsr == NULL || ai_pccPwd == NULL )
	{
		return M4_ERROR;
	}

	if ( ao_pcDecrPwd != NULL )
	{
		delete[] ao_pcDecrPwd;
		ao_pcDecrPwd = NULL;
	}	


	/* -- Build eMind dynamic decryption key ------------------------------------------------------ */
	iKeyLen = strlen ( M4EM_PRFX ) + strlen ( ai_pccUsr );
	pcKey   = new m4char_t [ iKeyLen + 1 ];

	strncpy ( pcKey, M4EM_PRFX, strlen ( M4EM_PRFX ) );
	strncpy ( pcKey + strlen ( M4EM_PRFX ), ai_pccUsr, strlen ( ai_pccUsr ) );


	/* -- Decrypt it ------------------------------------------------------------------------------ */
	iRet = DecryptTwoWay ( ai_pccPwd, strlen ( ai_pccPwd ), pcKey, iKeyLen, pcBuffer, iBuffSzReq );

	if ( iRet == M4_SUCCESS )
	{
		m4int_t iClearPwdLen = strlen ( pcBuffer ) - strlen ( ai_pccUsr );
		ao_pcDecrPwd = new m4char_t [ iClearPwdLen + 1 ];
		strncpy ( ao_pcDecrPwd, pcBuffer + strlen ( ai_pccUsr ), iClearPwdLen );
		*(ao_pcDecrPwd + iClearPwdLen) = 0;
	}
	else
	{
		iRet = M4_ERROR;
	}
	
	delete[] pcKey;
	delete[] pcBuffer;

	return iRet;

}


/* ------------------------------------------------------------------------------------------------ */
/* -- Decrypts user password cyphered with PeopleNet key.                  						 -- */
/* -- Input:																					 -- */
/* --      ai_pccPwd: Password to decrypt.														 -- */
/* --      ao_pcDecrPwd: Output decrypted password.												 -- */
/* -- Return:																					 -- */
/* --      M4_SUCCESS/M4_ERROR																	 -- */
/* --                                   														 -- */
/* -- Allocates memory for ao_pcDecrPwd being the caller who must free it.                       -- */
/* ------------------------------------------------------------------------------------------------ */

m4return_t decryptPeopleNetUsrPwd ( m4pcchar_t ai_pccPwd, m4pchar_t& ao_pcDecrPwd )
{

	m4return_t	iRet    = M4_ERROR;
	m4int_t		iPwdLenReq = 0;


	/* -- Check arguments ------------------------------------------------------------------------- */
	if ( ai_pccPwd == NULL  )
	{
		return M4_ERROR;
	}

	if ( ao_pcDecrPwd != NULL )
	{
		delete[] ao_pcDecrPwd;
		ao_pcDecrPwd = NULL;
	}

	/* -- Decrypt it ------------------------------------------------------------------------------ */
	iRet = DecryptTwoWay( ai_pccPwd, strlen ( ai_pccPwd ), M4CH_DUMMY_B2 + 23, strlen ( M4CH_DUMMY_B2 + 23 ), ao_pcDecrPwd, iPwdLenReq );

	if ( iRet != M4_SUCCESS )
	{
		iRet = M4_ERROR;
	}

	return iRet;

}


/* ------------------------------------------------------------------------------------------------ */
/* -- Sets the return value for LN4 calls.				                 						 -- */
/* -- Input:																					 -- */
/* --      ai_pvReturn: Input/output variant.   												 -- */
/* --      ai_iRetVal:  Return value.															 -- */
/* -- Return:																					 -- */
/* --      No return value.																		 -- */
/* --                                   														 -- */
/* ------------------------------------------------------------------------------------------------ */

m4void_t SetReturnValue ( m4VariantType &ai_pvReturn, m4int_t ai_iRetVal )
{
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = ai_iRetVal;
}