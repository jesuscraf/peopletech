#ifndef _SQLOJTRANS_HPP_
#define _SQLOJTRANS_HPP_

#include "m4stl.hpp"
#include "metainfo.hpp"
#include "clmtsent.hpp"
////////////////////////////////////////////////
// Outer Joins
////////////////////////////////////////////////
class _ClOuterJoinTranslator
{
public:
	virtual void AddOuterJoinInEqual(string &ao_stTrans) = 0;	// Estilo SQL92
	virtual void AddOuterJoinAfter(string &ao_stTrans) = 0;	// Estilo Oracle.
	virtual void AddOuterJoinInCondition(string &ao_stTrans) = 0;	// Para las condiciones. 
																	//Añade el simbolo (+). En SQLServer y en DB2 se utiliza para movber las condiciones a la parte From.
protected:
};
class ClOuterJoinTranslator_Nihil	: public	_ClOuterJoinTranslator
{
	public:
	virtual void AddOuterJoinInEqual(string &ao_stTrans){}
	virtual void AddOuterJoinAfter(string &ao_stTrans){}
	virtual void AddOuterJoinInCondition(string &ao_stTrans){}
};
class ClOuterJoinTranslator_SQLServer	: public	_ClOuterJoinTranslator
{
	public:
	virtual void AddOuterJoinInEqual(string &ao_stTrans){}
	virtual void AddOuterJoinAfter(string &ao_stTrans){}
	virtual void AddOuterJoinInCondition(string &ao_stTrans){
		ao_stTrans.append("(+)");
	}
};
class ClOuterJoinTranslator_Oracle	: public	_ClOuterJoinTranslator
{
public:
	virtual void AddOuterJoinInEqual(string &ao_stTrans){}
	virtual void AddOuterJoinAfter(string &ao_stTrans){
		ao_stTrans.append("(+)");
	}
	virtual void AddOuterJoinInCondition(string &ao_stTrans){
		ao_stTrans.append("(+)");
	}
};
class ClOuterJoinTranslator_Sybase	: public	_ClOuterJoinTranslator
{
public:
	virtual void AddOuterJoinInEqual(string &ao_stTrans){
		ao_stTrans.append("*");	// Siempre tenemos el Outer join en este sentido.
	}
	virtual void AddOuterJoinAfter(string &ao_stTrans){}
	virtual void AddOuterJoinInCondition(string &ao_stTrans){
		ao_stTrans.append("*");	
	}
};

class _ClOuterJoinReductor
{
	public: 
	virtual m4return_t	ReduceOuterJoins	(VcClMtRelat &ai_vRelat,ClMetaSentence * ai_pMtSent,m4bool_t &ai_bHasChanges);
	protected:
	virtual m4bool_t	_LookForConflicts	(VcClMtRelat &ai_vRelat,ClMetaSentence * ai_pMtSent,m4bool_t &ai_bBigConflict);
	virtual m4return_t	_SolveConflicts		(VcClMtRelat &ai_vRelat,ClMetaSentence * ai_pMtSent,m4bool_t &ai_bHasChanges) = 0;

};
// Oracle, SQLServer y DB2 Informix y Sybase.
// En caso de conflicto, prevalece el Outer Join. ( Generalmente el propio gestor quitará luego el Outer Join ).
// Lo he implementado así porque es muy posible que me haga falta uno distinto para Informix y Sybase.
class ClOuterJoinReductor_Oracle : public _ClOuterJoinReductor 
{	
	protected:
	virtual m4return_t _SolveConflicts(VcClMtRelat &ai_vRelat,ClMetaSentence * ai_pMtSent,m4bool_t &ai_bHasChanges);
};


////////////////////////////////////////////////
// Bloqueos
////////////////////////////////////////////////
class _ClLockerTranslator
{
public:
	virtual void AddLockBetweenJoins(string &ao_stTrans)= 0;	// SQLServer (ROWLOCK UPDLOCK).
	virtual void AddLockForUpdate(string &ao_stTrans)= 0;	// Oracle, Informix, Sybase, DB2.
	virtual void AddLockForUpdateInOrderBy(string &ao_stTrans)= 0;	// Oracle, Informix, Sybase, DB2.
};
class ClLockerTranslator_ForUpdate : public _ClLockerTranslator // Oracle, DB2.
{
public:
	virtual void AddLockBetweenJoins(string &ao_stTrans){}
	virtual void AddLockForUpdate(string &ao_stTrans){
		ao_stTrans.append(" FOR UPDATE");
	}	
	virtual void AddLockForUpdateInOrderBy(string &ao_stTrans) {
		AddLockForUpdate (ao_stTrans);
	}

};
class ClLockerTranslator_SQLServer : public _ClLockerTranslator 
{
public:
	virtual void AddLockBetweenJoins(string &ao_stTrans){	// SQLServer (ROWLOCK UPDLOCK).
		/* Bug 0167731
		Se cambia HOLDLOCK por UPDLOCK
		*/
		ao_stTrans.append(" WITH (ROWLOCK UPDLOCK) ");
	}
	virtual void AddLockForUpdate(string &ao_stTrans){}	
	virtual void AddLockForUpdateInOrderBy(string &ao_stTrans) {
		AddLockForUpdate (ao_stTrans);
	}

};
class ClLockerTranslator_Sybase : public _ClLockerTranslator 
{
public:
	virtual void AddLockBetweenJoins(string &ao_stTrans){	// Sybase HOLDLOCK.
		ao_stTrans.append(" HOLDLOCK ");
	}
	virtual void AddLockForUpdate(string &ao_stTrans){}	
	virtual void AddLockForUpdateInOrderBy(string &ao_stTrans) {
		AddLockForUpdate (ao_stTrans);
	}

};
//Bug 0077123.
//Informix. No acepta ORDER BY c1 FOR UPDATE.
//Creamos esta nueva clase para controlar el caso de Order by con update.
class ClLockerTranslator_Informix : public _ClLockerTranslator // Informix
{
public:
	virtual void AddLockBetweenJoins(string &ao_stTrans){}
	virtual void AddLockForUpdate(string &ao_stTrans){
		ao_stTrans.append(" FOR UPDATE");
	}
	//En Informix No se pone FOR UPDATE si la sentencia tiene ORDER BY.
	virtual void AddLockForUpdateInOrderBy(string &ao_stTrans) {}
};
////////////////////////////////////////////////
// NVL
////////////////////////////////////////////////
class _ClNVLTranslator
{
public:
	virtual void AddNVL(string &ao_stTrans)= 0;	
};
class ClNVLTranslator_NVL	: public _ClNVLTranslator
{
public:
	virtual void AddNVL(string &ao_stTrans){ao_stTrans.append("NVL");}	// NVL ( , ).
};
class ClNVLTranslator_IsNull	: public _ClNVLTranslator
{
public:
	virtual void AddNVL(string &ao_stTrans){ao_stTrans.append("ISNULL");}	// SQLServer IS_NULL( , ).
};
class ClNVLTranslator_Coalesce	: public _ClNVLTranslator
{
public:
	virtual void AddNVL(string &ao_stTrans){ao_stTrans.append("COALESCE");}	// DB2 COALESCE( , ).
};

////////////////////////////////////////////////
// Order By
////////////////////////////////////////////////
class _ClOrderByTranslator
{
public:
	virtual m4return_t MoveLocalFieldsToBegining(string &ao_stTrans, string ai_stLocalPlace)= 0;	
};
class ClOrderByTranslator_Move	: public _ClOrderByTranslator
{
public:
	virtual m4return_t MoveLocalFieldsToBegining(string &ao_stTrans, string ai_stLocalPlace);
protected:
	m4int32_t	RFindNotInQuote(string &ai_stString, string &ai_stFind, m4uint32_t aio_iStartPos = 0);
	m4return_t	_MoveOneLocalFieldToSelect(string &ao_stLocal, m4int32_t &aio_iLocalFieldBegin, m4int32_t &aio_iLocalFieldEnd, string &ao_stSelect);
	m4return_t	DelStrFromBE(string &ai_stString, m4int32_t &aio_iBegin, m4int32_t &aio_iEnd, string &ai_stFind, m4int8_t ai_iWhere);
};

class ClOrderByTranslator_Nihil	: public _ClOrderByTranslator
{
public:
	virtual m4return_t MoveLocalFieldsToBegining(string &ao_stTrans, string ai_stLocalPlace){return M4_SUCCESS;}	
};


class ClTranslatorUtils{
public:
	ClTranslatorUtils(_ClOuterJoinTranslator	*ai_pOJTrans,_ClLockerTranslator	*ai_pLockTrans,_ClNVLTranslator		*ai_pNVLTrans,_ClOrderByTranslator	*ai_pOrderByTrans){
		M4_ASSERT(ai_pOJTrans&&ai_pLockTrans&&ai_pNVLTrans&&ai_pOrderByTrans);
		m_pOJTrans	 =	ai_pOJTrans;
		m_pLockTrans =	ai_pLockTrans;
		m_pNVLTrans	 =	ai_pNVLTrans;
		m_pOrderByTrans = ai_pOrderByTrans;
	}

	_ClOuterJoinTranslator	*m_pOJTrans;
	_ClLockerTranslator		*m_pLockTrans;
	_ClNVLTranslator		*m_pNVLTrans;
	_ClOrderByTranslator	*m_pOrderByTrans;
};
// Funciones globales utilizadas en ClOrderByTranslator_Move y ClSQLTranslator_SQLServer;
m4return_t	GetNextCompleteToken(m4pcchar_t ai_pcString, m4int32_t &ao_lenToken, m4char_t ai_cAditionalSeparator = '\0');
m4int32_t	FindNotInQuote(string &ai_stString, string &ai_stFind, m4uint32_t aio_iStartPos = 0);



#endif
