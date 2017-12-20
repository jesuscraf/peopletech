//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ldap.dll
// File:                errors.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                21/12/98
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the error codes for the channel- core
//
//
//==============================================================================


#ifndef __ERRORS_HPP__
#define __ERRORS_HPP__

#include "m4res.hpp"

//==============================================================================
//  Errores de LDAP
//==============================================================================

#define	M4_LDAP_E							2359601
#define M4_LDAP_E_BAD_PARAMETERS			2359602
#define M4_LDAP_E_SESSION_NOT_INITIALIZED	2359603
#define M4_LDAP_E_GET_SESSION_HANDLE		2359604
#define M4_LDAP_E_INVALID_ARGUMENT			2359605
#define M4_LDAP_E_INVALID_ENTRY_DN			2359606
#define M4_LDAP_E_INVALID_ENTRY_ATTRIBUTES	2359607
#define M4_LDAP_E_APP_UNAVAILABLE_TO_USER 	2359608
#define M4_LDAP_E_INVALID_DN_CHECK_APP_AV 	2359609
#define M4_LDAP_E_AD_RECOVERING_USER_DN		2359610
#define M4_LDAP_E_AD_EXTRACTING_USER_ID		2359611

#endif
