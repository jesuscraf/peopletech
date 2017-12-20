// Type.h: interface for the CType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYPE_H__599B49F3_ED7F_11D2_BD1D_00A0248A69E8__INCLUDED_)
#define AFX_TYPE_H__599B49F3_ED7F_11D2_BD1D_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

typedef struct estr_type {
		CString BaseType;
		CString Loopinit;
		CString Loopend;
		CString Expression;
		CString Code;
		int Errorlevel;
} Str_type;

//##ModelId=397307400186
class CType  
{
public:
	//##ModelId=3973074001D8
	CType();
	//##ModelId=3973074001D6
	virtual ~CType();

	//##ModelId=3973074001CC
	short GetType(CString Type, Str_type * Datas);
	//##ModelId=3973074001C3
	void CargarPB (ICPropBag * pPBTypesrec);

private:
	//##ModelId=3973074001BA
	static BOOL IsBaseType(CString Type);
	//##ModelId=3973074001AE
	static int GenConstraint(CString Type, Str_type *Datas);

private:
	//##ModelId=3973074001A6
	ICPropBag *m_pPBTypes;
};

#endif // !defined(AFX_TYPE_H__599B49F3_ED7F_11D2_BD1D_00A0248A69E8__INCLUDED_)
