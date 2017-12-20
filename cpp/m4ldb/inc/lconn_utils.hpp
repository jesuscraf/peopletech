#ifndef _LCONN_UTILS_HPP_
#define _LCONN_UTILS_HPP_

#include "cltblmem.hpp"
#include "m4dbtypes.hpp"
class ClPersist;
class ClCompiledMCR;
class ClAccess;

struct StPostValidation
{
	ClPersist *m_pStm;
	ClTblMem m_oParam;
	m4uint32_t m_indOperation;
	m4int32_t m_indLgcTbl;
	m4int32_t m_hNode;
	ClAccess *m_pAccess;
	m4uint32_t m_indBlock;
	m4uint32_t m_indReg;
	m4bool_t m_bOperationValidated;

	StPostValidation (void)
	{
		m_pStm = NULL;
		m_indOperation = 0;
		m_indLgcTbl = NULL;
		m_hNode = 0;
		m_pAccess = NULL;
		m_indBlock = 0;
		m_indReg = 0;
		m_bOperationValidated = M4_FALSE;
	}
};
typedef list<StPostValidation *>		LsPStPostValidation;
typedef LsPStPostValidation::iterator	itLsPStPostValidation;

struct StSecurityFilter
{
	m4id_t m_idFilter;
	m4pcchar_t m_pAlias;
	m4pcchar_t m_pFilter;
	m4int32_t m_lLevel;
	string m_sJoin;

	StSecurityFilter (void)
	{
		m_idFilter = NULL;
		m_lLevel = 0;
		m_pFilter = NULL;
		m_pAlias = NULL;
	}

	StSecurityFilter (m4id_t ai_idFilter, m4int32_t ai_lLevel, m4pcchar_t ai_pFilter, m4pcchar_t ai_pAlias)
	{
		m_idFilter = ai_idFilter;
		m_lLevel = ai_lLevel;
		m_pFilter = ai_pFilter;
		m_pAlias = ai_pAlias;
	}
};
typedef vector<StSecurityFilter *>				VcPStSecurityFilter;
typedef VcPStSecurityFilter::const_iterator		itPStSecurityFilter;

struct StAccReload
{
	ClAccess *m_pAccess;
	m4bool_t m_bReload;
	StAccReload(ClAccess * ai_pAcc){
		m_pAccess = ai_pAcc;
		m_bReload = M4_FALSE;
	}
	void SetReLoad(){
		m_bReload = M4_TRUE;
	}
	void ResetReLoad(){
		m_bReload = M4_FALSE;
	}

};
typedef vector<StAccReload *>				VcPClAccessRl;
typedef VcPClAccessRl::const_iterator		itPVcPClAccessRl;

	
struct StTransaction {

public:
	m4date_t m_dDTStart;
	m4date_t m_dDTStartCorr;
	m4int32_t m_indLgcTbl;
	string m_stPK;
	
	StTransaction (void) 
	{
		m_dDTStart = M4LDB_MINUS_INF_DATE;
		m_dDTStartCorr = 0;
		m_indLgcTbl = M4LDB_NULL_INDEX;
	}
	
	StTransaction (const struct StTransaction &ai_stCopy) 
	{
		m_dDTStart = ai_stCopy.m_dDTStart;
		m_dDTStartCorr = ai_stCopy.m_dDTStartCorr;
		m_indLgcTbl = ai_stCopy.m_indLgcTbl;
		m_stPK = ai_stCopy.m_stPK;
	}
	
	friend m4bool_t operator < (const struct StTransaction &lhc, const struct StTransaction &rhc) 
	{
		m4double_t diff = M4LDB_COMPARE_DATES (lhc.m_dDTStart, rhc.m_dDTStart);
		if (diff < 0) {
			return (M4_TRUE);
		}
		if (diff){
			return (M4_FALSE);
		}
		diff = M4LDB_COMPARE_DATES (lhc.m_dDTStartCorr, rhc.m_dDTStartCorr);
		if (diff < 0) {
			return (M4_TRUE);
		}
		if (diff){
			return (M4_FALSE);
		}
		if (lhc.m_indLgcTbl < rhc.m_indLgcTbl){
			return (M4_TRUE);
		}
		if (lhc.m_indLgcTbl != rhc.m_indLgcTbl){
			return (M4_FALSE);
		}
		if (lhc.m_stPK < rhc.m_stPK){
			return (M4_TRUE);
		}
		return (M4_FALSE);
	}


	friend m4bool_t operator ==(const struct StTransaction &lhc, const struct StTransaction &rhc) 
	{
		if (M4LDB_COMPARE_DATES (lhc.m_dDTStart, rhc.m_dDTStart)) {
			return (M4_FALSE);
		}
		if (M4LDB_COMPARE_DATES (lhc.m_dDTStartCorr, rhc.m_dDTStartCorr)) {
			return (M4_FALSE);
		}
		if (lhc.m_indLgcTbl != rhc.m_indLgcTbl){
			return (M4_FALSE);
		}
		if (lhc.m_stPK != rhc.m_stPK){
			return (M4_FALSE);
		}
		return (M4_TRUE);
	}
	
};
	
typedef map<struct StTransaction, class ClStmt*, less<struct StTransaction> >	MpTransCache;
typedef MpTransCache::iterator														itMpTransCache;


#endif

