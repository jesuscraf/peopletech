//==============================================================================
//
//  (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                traductor.hpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module translates SQLSentences depending on which DB we are using.
//
//
//==============================================================================

#ifndef _TRADUCTOR_HPP_
#define _TRADUCTOR_HPP_

#include "m4types.hpp"
#include "defmain.hpp"
#include "sqlojtrans.hpp"

class ClMetaSentence;
class _AClSQLTranslator;
class _ClLockerTranslator;
class ClGlobVars;

extern _ClLockerTranslator	**s_ppLockerTranslator;
class ClTraductor
{
public:
	ClTraductor (ClMetaSentence	*ai_pmtsent, eDDBBType_t ai_eDDBB,ClGlobVars * ai_pGV, m4bool_t ai_bIsSecurityView, string* ai_psTranslatedSQL);
	~ClTraductor();
	m4return_t		TranslateSentence(string &ao_stTrans,m4int32_t ai_indColIni,m4int32_t ai_indColEnd);
private:
	_AClSQLTranslator	*m_pTranslator;

public:
	// Objetos estaticos para traduccion.
	static ClTranslatorUtils	*s_ppTrUtils[5];

private:
	// Bloqueos.
	static ClLockerTranslator_ForUpdate		s_oLockTrans_ForUpdate;	// Oracle, DB2
	static ClLockerTranslator_Sybase		s_oLockTrans_Sybase;	// Sybase
	static ClLockerTranslator_SQLServer		s_oLockTrans_SQLServer;	// SQLServer
	//Bug 0077123.
	//Informix. No acepta ORDER BY c1 FOR UPDATE.
	static ClLockerTranslator_Informix		s_oLockTrans_Informix;	// Informix

	// Outer Joins ( solo el simblo ).
	static ClOuterJoinTranslator_Nihil		s_oOJTrans_Nihil;		// Infromix
	static ClOuterJoinTranslator_SQLServer	s_oOJTrans_SQLServer;	// DB2 y SQLServer. Simplemente añaden el Outer join en las condiciones para que despues se pueda mover.
	static ClOuterJoinTranslator_Sybase		s_oOJTrans_Sybase;		// Sybase
	static ClOuterJoinTranslator_Oracle		s_oOJTrans_Oracle;		// Oracle

	// NVL
	static ClNVLTranslator_NVL				s_oNVLTrans_NVL;		// Oracle, Sybase e Informix
	static ClNVLTranslator_IsNull			s_oNVLTrans_IsNull;		// SQLServer
	static ClNVLTranslator_Coalesce			s_oNVLTrans_Coalesce;	// DB2

	// Order By
	static ClOrderByTranslator_Nihil		s_oOrderByTrans_Nihil;	// Oracle, SQLServer y Sybase
	static ClOrderByTranslator_Move			s_oOrderByTrans_Move;	// Informix y DB2

	//SQLServer
	static ClTranslatorUtils				s_TrUtilsSQLServer;
	//Informix
	static ClTranslatorUtils				s_TrUtilsInformix;
	//Oracle
	static ClTranslatorUtils				s_TrUtilsOracle;
	//DB2
	static ClTranslatorUtils				s_TrUtilsDB2;
	//Sybase
	static ClTranslatorUtils				s_TrUtilsSybase;
};

#endif


