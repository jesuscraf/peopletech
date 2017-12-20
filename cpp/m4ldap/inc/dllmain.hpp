//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ldap.dll
// File:             dllmain.hpp
// Project:          OLE Object integration
// Author:           Meta Software M.S. , S.A
// Date:             5/5/99
// Language:         C++
// Operating System: WINDOWS, UNIX
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "m4types.hpp"
#include "m4ldap_dll.hpp"
#include "ldap.h"


//Exported functions

extern "C" M4_DECL_M4LDAP m4return_t LogonAuthenticode		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPInit				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPBind				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPFind				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPRelease			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPPersist			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPDeleteEntry		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPUpdateEntry		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPSetVersion			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPInitSSL			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPResetADPassword	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPEnableADAccount	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4LDAP m4return_t LDAPChangeADPassword	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

//Not exported functions

m4return_t _LogonAuthenticodeNoSSL (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
m4return_t _LogonAuthenticodeSSL (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
m4return_t _checkUserApplAvailability (m4pchar_t ai_pcDNUsrNode, LDAP * ai_pHandle, m4pvoid_t ai_pvContext);

m4void_t SetResultValues (m4pvoid_t ai_pvContext, m4int_t ai_iRetVal, m4pchar_t ai_pccRetDesc);
m4void_t SetReturnValue (m4VariantType &ai_pvReturn, m4int_t ai_iRetVal);

m4return_t BuildLDAPMod (LDAPMod ** mods, ClAccessRecordSet* curr);

m4void_t SetLastPEntry (m4pvoid_t ai_pvContext, m4int_t ai_iPEntry);

m4pchar_t* Parse( m4pchar_t pcCad, const m4pchar_t pcSep);

m4int_t _charpToUTF16LE (m4pchar_t ai_pszSource, void*& ao_pTarget);

m4int_t _charpToUTF8 (m4pcchar_t ai_pccSource, m4pchar_t& ao_pcTarget);

m4bool_t _isADShortFormat( LDAP * ai_pHandle, m4pcchar_t ai_pcUsr ) ;

m4return_t _getADUserDN ( LDAP * ai_pHandle, m4pcchar_t ai_pccUser, m4pchar_t &ao_pccUser) ;

m4return_t _getADUserID( m4pcchar_t ai_pccUser, m4pchar_t &ao_pcUserID ) ;

// m4void_t ResetModify (m4pvoid_t ai_pvContext);
// Por si hace falta
