#include "sqlojtrans.hpp"
#include "traductor.hpp"
#include "sqltrans.hpp"
#include "sqltrans_db.hpp"
#include "clglbvar.hpp"
#include "connmanager.hpp"

// Bloqueos.
ClLockerTranslator_ForUpdate	ClTraductor::s_oLockTrans_ForUpdate;	// Oracle, DB2
ClLockerTranslator_Sybase		ClTraductor::s_oLockTrans_Sybase;		// Sybase
ClLockerTranslator_SQLServer	ClTraductor::s_oLockTrans_SQLServer;	// SQLServer
//Bug 0077123.
//Informix. No acepta ORDER BY c1 FOR UPDATE.
ClLockerTranslator_Informix		ClTraductor::s_oLockTrans_Informix;		// Informix

// Outer Joins ( solo el simblo ).
ClOuterJoinTranslator_Nihil		ClTraductor::s_oOJTrans_Nihil;		// Infromix
ClOuterJoinTranslator_SQLServer	ClTraductor::s_oOJTrans_SQLServer;	// DB2 y SQLServer. Simplemente añaden el Outer join en las condiciones para que despues se pueda mover.
ClOuterJoinTranslator_Sybase	ClTraductor::s_oOJTrans_Sybase;		// Sybase
ClOuterJoinTranslator_Oracle	ClTraductor::s_oOJTrans_Oracle;		// Oracle

// NVL
ClNVLTranslator_NVL				ClTraductor::s_oNVLTrans_NVL;		// Oracle, Sybase e Informix
ClNVLTranslator_IsNull			ClTraductor::s_oNVLTrans_IsNull;		// SQLServer
ClNVLTranslator_Coalesce		ClTraductor::s_oNVLTrans_Coalesce;	// DB2

// Order By
ClOrderByTranslator_Nihil		ClTraductor::s_oOrderByTrans_Nihil;	// Oracle, SQLServer y Sybase
ClOrderByTranslator_Move		ClTraductor::s_oOrderByTrans_Move;	// Informix y DB2

//SQLServer
ClTranslatorUtils				ClTraductor::s_TrUtilsSQLServer	(&s_oOJTrans_SQLServer,&s_oLockTrans_SQLServer,&s_oNVLTrans_IsNull,&s_oOrderByTrans_Nihil);
//Informix
//Bug 0077123.
//Informix. No acepta ORDER BY c1 FOR UPDATE.
ClTranslatorUtils				ClTraductor::s_TrUtilsInformix	(&s_oOJTrans_Nihil,&s_oLockTrans_Informix,&s_oNVLTrans_NVL,&s_oOrderByTrans_Move);
//Oracle
ClTranslatorUtils				ClTraductor::s_TrUtilsOracle		(&s_oOJTrans_Oracle,&s_oLockTrans_ForUpdate,&s_oNVLTrans_NVL,&s_oOrderByTrans_Nihil);
//DB2
ClTranslatorUtils				ClTraductor::s_TrUtilsDB2		(&s_oOJTrans_SQLServer,&s_oLockTrans_ForUpdate,&s_oNVLTrans_Coalesce,&s_oOrderByTrans_Move);
//Sybase
ClTranslatorUtils				ClTraductor::s_TrUtilsSybase		(&s_oOJTrans_Sybase,&s_oLockTrans_Sybase,&s_oNVLTrans_NVL,&s_oOrderByTrans_Nihil);


ClTranslatorUtils	*ClTraductor::s_ppTrUtils[5] = {&s_TrUtilsSQLServer,&s_TrUtilsInformix,&s_TrUtilsOracle,&s_TrUtilsDB2,&s_TrUtilsSybase };

ClTraductor::ClTraductor (ClMetaSentence *ai_pmtsent, eDDBBType_t ai_eDDBB,ClGlobVars * ai_pGV, m4bool_t ai_bIsSecurityView, string* ai_psTranslatedSQL)
{
	switch (ai_eDDBB){
	case M4DBW_SQL_SERVER_DDBB :
	case M4DBW_DB2_DDBB :
		m_pTranslator = new ClSQLTranslator_SQLServer(ai_pmtsent,ai_pGV,s_ppTrUtils[ai_eDDBB-1], ai_bIsSecurityView, ai_psTranslatedSQL);
		break;
	case M4DBW_INFORMIX_DDBB :
		m_pTranslator = new ClSQLTranslator_Informix(ai_pmtsent,ai_pGV,s_ppTrUtils[ai_eDDBB-1], ai_bIsSecurityView, ai_psTranslatedSQL);
		break;
	case M4DBW_ORACLE_DDBB :
	case M4DBW_SYBASE_DDBB :
		m_pTranslator = new ClSQLTranslator_Oracle(ai_pmtsent,ai_pGV,s_ppTrUtils[ai_eDDBB-1], ai_bIsSecurityView, ai_psTranslatedSQL);
		break;
	default:
		// Oracle.
		m_pTranslator = new ClSQLTranslator_Oracle(ai_pmtsent,ai_pGV,s_ppTrUtils[M4DBW_ORACLE_DDBB-1], ai_bIsSecurityView, ai_psTranslatedSQL);
		break;
	}
}

ClTraductor::~ClTraductor(){
	if (m_pTranslator){
		delete m_pTranslator;
	}
}

m4return_t	ClTraductor::TranslateSentence(string &ao_stTrans,m4int32_t ai_indColIni,m4int32_t ai_indColEnd)
{
	m4return_t ret;

	ret = m_pTranslator->TranslateSentence(ao_stTrans,ai_indColIni,ai_indColEnd);
	
	if (ret!=M4_SUCCESS){
		return ret;
	}

	return m_pTranslator->TranslateSentenceAfter(ao_stTrans);
}
