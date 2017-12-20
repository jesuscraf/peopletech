#ifndef _CLCHKJOINS_HPP_
#define _CLCHKJOINS_HPP_

#include "metainfo.hpp"
#include "m4dbtypes.hpp"

class ClCheckJoins{
public:
	ClCheckJoins	(PVcClMtObject	ai_pvcObject,PVcClMtAlias	ai_pvcAlias,PVcClMtRelat	ai_pvcRelat,PVcClMtField	ai_pvcField,ClGlobVars		*ai_pGlobVars, string *ai_pstWhere);
	m4return_t		CheckConsistency(eDDBBType_t	eDDBB);

protected:
	m4return_t	_CheckObjects				();
	m4return_t	_CheckOneObject				(ClMetaObject *ai_pObj);
	m4return_t	_SetObjectJoinToInnerJoin	(ClMetaObject *ai_pObj);
	void		_CheckForIncompatibilityWarning(ClMetaObject *ai_pObj);
	m4return_t	_CheckOneDominatedTwoDominator (void);
private:
	m4bool_t		m_bError;
	m4bool_t		m_bDoChange;
	ClGlobVars		*m_pGlobVars;
	PVcClMtObject	m_pvcObject;
	PVcClMtAlias	m_pvcAlias;
	PVcClMtRelat	m_pvcRelat;
	PVcClMtField	m_pvcField;
	string			*m_pstWhere;
};
#endif
