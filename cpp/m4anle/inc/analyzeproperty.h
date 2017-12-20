// AnalyzeProperty.h: interface for the CAnalyzeProperty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYZEPROPERTY_H__F2C09DA3_ECFF_11D2_BD1C_00A0248A69E8__INCLUDED_)
#define AFX_ANALYZEPROPERTY_H__F2C09DA3_ECFF_11D2_BD1C_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "expressionproperty.h"

//##ModelId=397307460026
class CAnalyzeProperty  
{
public:
	//##ModelId=39730746012C
	short AnalyzeProp (BSTR Object, BSTR Property, BSTR Value);
	//##ModelId=397307460118
	void CargarPB(ICPropBag * pPBTypesrec, ICPropBag * pPBPropertiesgrouprec, ICPropBag * pPBObjectsrec);
	//##ModelId=397307460117
	CAnalyzeProperty();
	//##ModelId=39730746010D
	virtual ~CAnalyzeProperty();

private:
	//##ModelId=397307460103
	short SeeValueConstraint (ICPropBag * pval);
	//##ModelId=3973074600F9
	short IsCorrect (long Id_Object);
	//##ModelId=3973074600EF
	short SeeValue ( ICPropCol  ** pval);
	//##ModelId=3973074600C7
	long IsObject ();
	//##ModelId=3973074600BC
	short SeeAggregates (CString NomAggreg);
	//##ModelId=3973074600A9
	short SeeValueAggreg ( ICPropBag * pval);
	//##ModelId=39730746009E
	short SeeConstraintAggreg ( ICPropBag * pval);
	//##ModelId=397307460096
	ICPropBag * m_pPBTypes;
	//##ModelId=397307460082
	ICPropBag * m_pPBObjects;
	//##ModelId=39730746006E
	ICPropBag * m_pPBPropertiesgroup;
	//##ModelId=397307460062
	CString m_NomObjeto;
	//##ModelId=397307460058
	CString m_NomProperty;
	//##ModelId=39730746004E
	CString m_ValueProp;
	//##ModelId=397307460046
	CExpressionProperty m_Expression;
};

#endif // !defined(AFX_ANALYZEPROPERTY_H__F2C09DA3_ECFF_11D2_BD1C_00A0248A69E8__INCLUDED_)
