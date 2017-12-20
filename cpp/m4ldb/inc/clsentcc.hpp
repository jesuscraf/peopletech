//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                clsentcc.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines ...
//
//
//==============================================================================


#ifndef __CLSENTCC_HPP__
#define __CLSENTCC_HPP__


#include "defmain.hpp"


class ClStmt;
class ClLConn;

struct StSentKey {

public:
	// Claves que distinguen las sentencias:
	m4int32_t	m_hNode; 
	m4int32_t	m_hStmt; 
	m4int32_t	m_lRole;
	m4int32_t	m_lStmt;
	m4double_t	m_MCRVersion;
	m4double_t	m_SessionVersion;
	m4pchar_t	m_pRole;
	m4pchar_t	m_pStmt;
	m4language_t m_Language;
	m4char_t	m_IdAutoFilter;
	m4char_t	m_CSType;
	m4char_t	m_Operation;
	m4char_t	m_OrgType;
	m4int32_t	m_idRConn;
	m4bool_t	m_bHasValidation;
	m4bool_t	m_bUseTramitation;
	m4bool_t	m_bHasRange;
	m4bool_t	m_bSecurityView;
	m4bool_t	m_bOwn;
	
	StSentKey (m4int32_t ai_hNode, m4double_t ai_MCRVersion, m4double_t ai_SessionVersion, m4language_t ai_Language, m4char_t ai_IdAutoFilter, m4char_t ai_CSType, m4char_t ai_Operation, m4pcchar_t ai_pRole, m4pcchar_t ai_pStmt, m4char_t ai_OrgType, m4int32_t ai_idRConn, m4int32_t ai_hStmt, m4bool_t ai_bHasValidation, m4bool_t ai_bUseTramitation, m4bool_t ai_bHasRange, m4bool_t ai_bSecurityView)
	{
		m_hNode = ai_hNode;
		m_hStmt = ai_hStmt;
		m_MCRVersion = ai_MCRVersion;
		m_SessionVersion = ai_SessionVersion;
		m_Language = ai_Language;
		m_IdAutoFilter = ai_IdAutoFilter;
		m_CSType = ai_CSType;
		m_Operation = ai_Operation;
		m_lRole = strlen (ai_pRole);
		m_pRole = new m4char_t [m_lRole + 1];
		memcpy (m_pRole, ai_pRole, m_lRole + 1);
		m_lStmt = strlen (ai_pStmt);
		m_pStmt = new m4char_t [m_lStmt + 1];
		memcpy (m_pStmt, ai_pStmt, m_lStmt + 1);
		m_OrgType = ai_OrgType;
		m_idRConn = ai_idRConn;
		m_bHasValidation = ai_bHasValidation;
		m_bUseTramitation = ai_bUseTramitation;
		m_bHasRange = ai_bHasRange;
		m_bSecurityView = ai_bSecurityView;
		m_bOwn = M4_TRUE;
	}
	
	StSentKey (const struct StSentKey &ai_stCopy) 
	{
		m_hNode = ai_stCopy.m_hNode;
		m_hStmt = ai_stCopy.m_hStmt;
		m_MCRVersion = ai_stCopy.m_MCRVersion;
		m_SessionVersion = ai_stCopy.m_SessionVersion;
		m_Language = ai_stCopy.m_Language;
		m_IdAutoFilter = ai_stCopy.m_IdAutoFilter;
		m_CSType = ai_stCopy.m_CSType;
		m_Operation = ai_stCopy.m_Operation;
		m_pRole = ai_stCopy.m_pRole;
		m_lRole = ai_stCopy.m_lRole;
		m_pStmt = ai_stCopy.m_pStmt;
		m_lStmt = ai_stCopy.m_lStmt;
		m_OrgType = ai_stCopy.m_OrgType ;
		m_idRConn = ai_stCopy.m_idRConn;
		m_bHasValidation = ai_stCopy.m_bHasValidation;
		m_bUseTramitation = ai_stCopy.m_bUseTramitation;
		m_bHasRange = ai_stCopy.m_bHasRange;
		m_bSecurityView = ai_stCopy.m_bSecurityView;
		m_bOwn = M4_FALSE;
	}

	~StSentKey (void) 
	{
		if( m_bOwn && m_pRole )
		{
			delete [] m_pRole;
			m_pRole = NULL;
		}
		if( m_bOwn && m_pStmt )
		{
			delete [] m_pStmt;
			m_pStmt = NULL;
		}
	}

	friend m4bool_t operator < (const struct StSentKey &lhc, const struct StSentKey &rhc) 
	{
		if (lhc.m_hNode < rhc.m_hNode){
			return M4_TRUE;
		}
		if (lhc.m_hNode != rhc.m_hNode){
			return M4_FALSE;
		}
		if (lhc.m_hStmt < rhc.m_hStmt){
			return M4_TRUE;
		}
		if (lhc.m_hStmt != rhc.m_hStmt){
			return M4_FALSE;
		}
		if (lhc.m_lRole < rhc.m_lRole){
			return M4_TRUE;
		}
		if (lhc.m_lRole != rhc.m_lRole){
			return M4_FALSE;
		}
		if (lhc.m_lStmt < rhc.m_lStmt){
			return M4_TRUE;
		}
		if (lhc.m_lStmt != rhc.m_lStmt){
			return M4_FALSE;
		}
		if (lhc.m_MCRVersion < rhc.m_MCRVersion){
			return M4_TRUE;
		}
		if (lhc.m_MCRVersion != rhc.m_MCRVersion){
			return M4_FALSE;
		}
		if (lhc.m_SessionVersion < rhc.m_SessionVersion){
			return M4_TRUE;
		}
		if (lhc.m_SessionVersion != rhc.m_SessionVersion){
			return M4_FALSE;
		}
		if (lhc.m_OrgType < rhc.m_OrgType ){
			return M4_TRUE;
		}
		if (lhc.m_OrgType != rhc.m_OrgType ){
			return M4_FALSE;
		}
		if (lhc.m_idRConn < rhc.m_idRConn ){
			return M4_TRUE;
		}
		if (lhc.m_idRConn != rhc.m_idRConn ){
			return M4_FALSE;
		}
		if (lhc.m_bHasValidation < rhc.m_bHasValidation ){
			return M4_TRUE;
		}
		if (lhc.m_bHasValidation != rhc.m_bHasValidation ){
			return M4_FALSE;
		}
		if (lhc.m_bUseTramitation < rhc.m_bUseTramitation ){
			return M4_TRUE;
		}
		if (lhc.m_bUseTramitation != rhc.m_bUseTramitation ){
			return M4_FALSE;
		}
		if (lhc.m_bHasRange < rhc.m_bHasRange ){
			return M4_TRUE;
		}
		if (lhc.m_bHasRange != rhc.m_bHasRange ){
			return M4_FALSE;
		}
		if (lhc.m_bSecurityView < rhc.m_bSecurityView ){
			return M4_TRUE;
		}
		if (lhc.m_bSecurityView != rhc.m_bSecurityView ){
			return M4_FALSE;
		}
		if (lhc.m_Language < rhc.m_Language){
			return M4_TRUE;
		}
		if (lhc.m_Language != rhc.m_Language){
			return M4_FALSE;
		}
		if (lhc.m_IdAutoFilter < rhc.m_IdAutoFilter){
			return M4_TRUE;
		}
		if (lhc.m_IdAutoFilter != rhc.m_IdAutoFilter){
			return M4_FALSE;
		}
		if (lhc.m_CSType < rhc.m_CSType){
			return M4_TRUE;
		}
		if (lhc.m_CSType != rhc.m_CSType){
			return M4_FALSE;
		}
		if (lhc.m_Operation < rhc.m_Operation ){
			return M4_TRUE;
		}
		if (lhc.m_Operation  != rhc.m_Operation ){
			return M4_FALSE;
		}
		if (memcmp (lhc.m_pRole, rhc.m_pRole, lhc.m_lRole) < 0) {
			return M4_TRUE;
		}
		if (memcmp (lhc.m_pStmt, rhc.m_pStmt, lhc.m_lStmt) < 0) {
			return M4_TRUE;
		}
		return (M4_FALSE);
	}

	friend m4bool_t operator ==(const struct StSentKey &lhc, const struct StSentKey &rhc) 
	{
		if (lhc.m_hNode != rhc.m_hNode){
			return M4_FALSE;
		}
		if (lhc.m_hStmt != rhc.m_hStmt){
			return M4_FALSE;
		}
		if (lhc.m_lRole != rhc.m_lRole){
			return M4_FALSE;
		}
		if (lhc.m_lStmt != rhc.m_lStmt){
			return M4_FALSE;
		}
		if (lhc.m_MCRVersion != rhc.m_MCRVersion){
			return M4_FALSE;
		}
		if (lhc.m_SessionVersion != rhc.m_SessionVersion){
			return M4_FALSE;
		}
		if (lhc.m_OrgType != rhc.m_OrgType ){
			return M4_FALSE;
		}
		if (lhc.m_idRConn != rhc.m_idRConn ){
			return M4_FALSE;
		}		
		if (lhc.m_bHasValidation != rhc.m_bHasValidation ){
			return M4_FALSE;
		}
		if (lhc.m_bUseTramitation != rhc.m_bUseTramitation ){
			return M4_FALSE;
		}
		if (lhc.m_bHasRange != rhc.m_bHasRange ){
			return M4_FALSE;
		}
		if (lhc.m_bSecurityView != rhc.m_bSecurityView ){
			return M4_FALSE;
		}
		if (lhc.m_Language != rhc.m_Language){
			return M4_FALSE;
		}
		if (lhc.m_IdAutoFilter != rhc.m_IdAutoFilter){
			return M4_FALSE;
		}
		if (lhc.m_CSType != rhc.m_CSType){
			return M4_FALSE;
		}
		if (lhc.m_Operation != rhc.m_Operation ){
			return M4_FALSE;
		}
		if (memcmp (lhc.m_pRole, rhc.m_pRole, lhc.m_lRole) != 0) {
			return M4_FALSE;
		}
		if (memcmp (lhc.m_pStmt, rhc.m_pStmt, lhc.m_lStmt) != 0) {
			return M4_FALSE;
		}
		return (M4_TRUE);
	}

};

struct StSentStmt;

typedef multimap<struct StSentKey, struct StSentStmt*, less<struct StSentKey> >	MMpSentCache;
typedef MMpSentCache*															PMMpSentCache;
typedef MMpSentCache::iterator													itMMpSentCache;


typedef map<m4int32_t, itMMpSentCache, less<m4int32_t> >						MpSentIter;
typedef MpSentIter*																PMpSentIter;
typedef MpSentIter::iterator													itMpSentIter;
typedef pair<itMpSentIter, bool>												pairMpSentIter;



struct StSentStmt {

public:
	ClStmt *m_pStmt;
	m4int32_t m_indStmt; // Esto sirve también para el peso de la sentencia.
	m4bool_t m_bIsFree;
	
	StSentStmt (void)
	{
		m_pStmt = NULL;
		m_indStmt = 0;
		m_bIsFree = M4_FALSE;
	}

	StSentStmt (ClStmt *ai_pStmt)
	{
		m_pStmt = ai_pStmt;
		m_indStmt = 0;
		m_bIsFree = M4_FALSE;
	}
};


typedef multimap<m4int32_t, struct StSentStmt*, less<m4int32_t> >			MMpSentCacheSystem;
typedef MMpSentCacheSystem*													PMMpSentCacheSystem;
typedef MMpSentCacheSystem::iterator										itMMpSentCacheSystem;


class ClLdb;


class ClSentCache
{

protected:
	ClLdb *m_pLdb;
	m4int32_t m_indStmt;
	m4uint32_t m_nMaxNumSent;
	MMpSentCache m_mmpSentCache;
	MMpSentCacheSystem m_mmpSentCacheSystem;
	m4int32_t m_nStmtUsed;

public:
	ClSentCache (void);
	~ClSentCache (void);

	void Init (ClLdb *ai_pLdb)
	{
		m_pLdb = ai_pLdb;
	}

	m4int32_t EraseSentsNoUsed (m4bool_t ai_bAll);
	m4return_t InsertSent (struct StSentKey &ai_oSentKey, itMMpSentCache &ao_itSent, ClStmt *ai_pStmt, ClLConn *aio_pLConn);
	m4return_t DeleteSent (itMMpSentCache &ao_itSent);
	ClStmt *FindSent (struct StSentKey &ai_oSentKey, itMMpSentCache &ao_itSent, ClLConn *aio_pLConn);
	m4return_t FreeSents (ClLConn *ai_pLConn);
	m4return_t FreeSent (itMMpSentCache &ao_itSent);
	m4int32_t Clear (void);
	m4return_t DefuseSent (ClLConn *ai_pLConn);

	m4return_t InsertSentSystem (m4int32_t ai_idSent, itMMpSentCacheSystem &ao_itSent, ClStmt *ai_pStmt);
	m4return_t DeleteSentSystem (itMMpSentCacheSystem &ao_itSent);
	ClStmt *FindSentSystem (m4int32_t ai_idSent, itMMpSentCacheSystem &ao_itSent);
	m4return_t FreeSentSystem (itMMpSentCacheSystem &ao_itSent);
	m4int32_t ClearSystem (void);
	m4return_t DefuseSentSystem (ClLConn *ai_pLConn);

	void SetMaxNumSent (m4int32_t ai_nMaxNumSent)
	{
		m_nMaxNumSent = ai_nMaxNumSent;
	}

	m4int32_t GetStmtUsed (void)
	{
		return (m_nStmtUsed);
	}

	m4uint32_t	GetSentCacheSize()
	{
		return m_mmpSentCache.size();
	}
};


#endif




