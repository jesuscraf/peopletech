// CompType.h: interface for the CCompType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPTYPE_H__8FA49ED4_D158_11D2_8BE7_00105A6690CF__INCLUDED_)
#define AFX_COMPTYPE_H__8FA49ED4_D158_11D2_8BE7_00105A6690CF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

typedef struct _info_error_
{
	CString Type;
	CString Code;
	CString Message;
}info_error;

//##ModelId=3973074200F3
class CCompType  
{
public:
	//##ModelId=397307420157
	CCompType();
	//##ModelId=39730742014F
	virtual ~CCompType();

	//##ModelId=39730742013B
	short comptype(LPCSTR ctipo, LPCSTR cvalor, info_error *pserror);
	//##ModelId=397307420139
	void cargapb(ICPropBag *pbag);

private:
	//##ModelId=397307420109
	ICPropBag *m_pPBTypes;

	//##ModelId=397307420125
	short comptipobase(LPCSTR tipo, LPCSTR valor, info_error *pserror);
	//##ModelId=39730742011C
	short compstring(CString palab);
	//##ModelId=397307420113
	short complist(CString palab);
	//##ModelId=397307420111
	short CCompType::complistint(CString palab);

};

#endif // !defined(AFX_COMPTYPE_H__8FA49ED4_D158_11D2_8BE7_00105A6690CF__INCLUDED_)
