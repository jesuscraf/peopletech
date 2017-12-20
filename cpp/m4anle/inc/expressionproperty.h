// ExpressionProperty.h: interface for the CExpressionProperty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPRESSIONPROPERTY_H__2291DA43_EDB5_11D2_BD1D_00A0248A69E8__INCLUDED_)
#define AFX_EXPRESSIONPROPERTY_H__2291DA43_EDB5_11D2_BD1D_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

typedef struct estr_Check {
		CString Loopinit;
		CString Loopend;
		CString Expression;
		CString Code;
} Str_Check;

//##ModelId=39730740026C
class CExpressionProperty  
{
public:
	//##ModelId=397307400281
	short VerifyProperty ( CString NameProperty, CString ValueProperty, CString NameObject, Str_Check Datas );
	//##ModelId=397307400278
	CExpressionProperty();
	//##ModelId=397307400276
	virtual ~CExpressionProperty();

};

#endif // !defined(AFX_EXPRESSIONPROPERTY_H__2291DA43_EDB5_11D2_BD1D_00A0248A69E8__INCLUDED_)
