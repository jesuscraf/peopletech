// CompObjects.h: interface for the CCompObjects class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPOBJECTS_H__F9A881D1_D167_11D2_BCF4_00A0248A69E8__INCLUDED_)
#define AFX_COMPOBJECTS_H__F9A881D1_D167_11D2_BCF4_00A0248A69E8__INCLUDED_

#include "pilaobligatorios.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



typedef	struct estr_constraint {
		BOOL Endvalidate;
		CString Expression;
		CString Code;
		short Errorlevel;
		CString Loopinit;
		CString Loopend;
		CString Message;
} tpcons;

typedef	struct estr_property {
		CString Type;
		CString Kind;
		BOOL Translated;
		CString Helpstring;
		BOOL Runtimeproperty;
		BOOL Runtimemethod;
		tpcons Constraint;
} tpprop;

typedef struct estr_objects {
		CString Objectname;
		int mininstances;
		int maxinstances;
} tpobj;

//##ModelId=397307370101
class CCompObjects  
{
public:
	//##ModelId=397307370289
	long BusCont ( long nobj, CString nomobj);
	//##ModelId=39730737027E
	void CargaPb(ICPropBag * pval1,ICPropBag * pval2,ICPropBag * pval3);
	//##ModelId=39730737026A
	long EncProp (long nobj, CString nomprop, tpprop& estr_prop);
	//##ModelId=39730737024B
	long BuscObj(CString nomobj, short& allpro, short& allobj, short& ignore);
	//##ModelId=397307370238
	short IntroduOblig(long nobj, CPilaObligatorios * PilaOblig, int id_obj);
	//##ModelId=397307370237
	CCompObjects();
	//##ModelId=39730737022D
	virtual ~CCompObjects();
private:
	//##ModelId=397307370223
	short RellEstrucConstr ( ICChildCol ** pval, tpprop & estr_pro);
	//##ModelId=39730737020F
	short GroupProblig ( ICPropBag * pval, CPilaObligatorios * PilaOblig, int id_obj);
	//##ModelId=3973073701FB
	short GuarProblig (CString nomgroup, CPilaObligatorios * PilaOblig, int id_obj);
	//##ModelId=3973073701E8
	short RecorrerGroupObj ( ICPropBag * pval, CPilaObligatorios * PilaOblig, int id_obj);
	//##ModelId=3973073701D4
	short GroupObjOblig ( CString nomgrup, CPilaObligatorios * PilaOblig, int id_obj);
	//##ModelId=3973073701C0
	short GuarObjOblig ( ICPropCol ** pval, CPilaObligatorios * PilaOblig, int id_obj);
	//##ModelId=3973073701B5
	short Allobjecto ( ICPropBag *pval, CString tipo);
	//##ModelId=3973073701A1
	short EncGroupPro (ICPropBag * pval, CString nompro, tpprop& estr_prop);
	//##ModelId=397307370197
	short EncContainsGroup ( ICPropBag * pval, CString nomobj);
	//##ModelId=397307370184
	short RellEstruc (ICPropCol ** pval, tpprop & estr_prop);
	//##ModelId=397307370170
	short EncAgregates (CString nompro, CString nomgroup, tpprop &estr_prop);
	//##ModelId=397307370166
	short RellConstraint(ICPropCol * * pval, tpcons & estr_tpcons);	
	//##ModelId=39730737015B
	short EncGroupObj (CString nomobj, CString nomgroup);
	//##ModelId=397307370148
	short EncObj ( ICPropCol  ** pval, CString nomobj);
	//##ModelId=39730737013D
	short EncPropObl ( ICPropCol * * pval);

	//##ModelId=397307370135
	ICPropBag *m_pPBObjects;
	//##ModelId=397307370121
	ICPropBag *m_pPBObjectsgroup;
	//##ModelId=39730737010D
	ICPropBag *m_pPBPropertysgroup;
};

#endif // !defined(AFX_COMPOBJECTS_H__F9A881D1_D167_11D2_BCF4_00A0248A69E8__INCLUDED_)
