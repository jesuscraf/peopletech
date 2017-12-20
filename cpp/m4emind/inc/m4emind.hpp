//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4emind.dll
// File:             m4emind.hpp
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
//    This module defines m4emind logon API interface.
//
//==============================================================================

#ifndef m4emind_h
#define m4emind_h 1


#include "m4types.hpp"
#include "m4variant.hpp"
#include "m4emind_dll.hpp"


// ------------------------------------------------------------------------------
// -- Exported functions --------------------------------------------------------
// ------------------------------------------------------------------------------


// -- Authenticates eMind user.
// -- Args (LN4):
// --      ai_pvArg[0]: User Id.
// --      ai_pvArg[1]: eMind user password (eMind encrypted).
// --      ai_pvArg[2]: User password to validate (PeopleNet encrypted).
// -- Return (LN4):
// --      M4_SUCCESS: Authentication success.
// --      1:		   Authentication denied.
// --      M4_ERROR:   Error during authentication.

extern "C" M4_DECL_M4EMIND m4return_t authenticateEMindUser( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,	m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );



// ------------------------------------------------------------------------------
// -- Not exported functions ----------------------------------------------------
// ------------------------------------------------------------------------------


// -- Decrypts user password with eMind key.
// -- Args:
// --      ai_pccUsr: User Id.
// --      ai_pccPwd: Password to decrypt.
// --      ao_pcDecrPwd: Output decrypted password.
// -- Return:	
// --	   M4_SUCCESS / M4_ERROR
// -- 
// -- Allocates memory for ao_pcDecrPwd being the caller who must free it.

m4return_t decryptEMindUsrPwd( m4pcchar_t ai_pccUsr, m4pcchar_t ai_pccPwd, m4pchar_t& ao_pcDecrPwd );


// -- Decrypts user password with PeopleNet key.
// -- Args:
// --      ai_pccPwd:    Password to decrypt.
// --      ao_pcDecrPwd: Output decrypted password.
// -- Return:	
// --	   M4_SUCCESS / M4_ERROR
// -- 
// -- Allocates memory for ao_pcDecrPwd being the caller who must free it.

m4return_t decryptPeopleNetUsrPwd ( m4pcchar_t ai_pccPwd, m4pchar_t& ao_pcDecrPwd );


// -- Sets the return value for LN4 calls.
// -- Input:
// --      ai_pvReturn: Input/output variant.
// --      ai_iRetVal:  Return value.
// -- Return:
// --      No return value.

m4void_t SetReturnValue (m4VariantType &ai_pvReturn, m4int_t ai_iRetVal);


#endif
