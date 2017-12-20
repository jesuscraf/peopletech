// Runtime.h: interface for the CRuntime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTIME_H__B5B17F04_E02A_11D2_BD0B_00A0248A69E8__INCLUDED_)
#define AFX_RUNTIME_H__B5B17F04_E02A_11D2_BD0B_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//##ModelId=397307440240
class CRuntime  
{
public:
	//##ModelId=3973074402E1
	BOOL IsClassnameDefined (CString nombreobj);
	//##ModelId=3973074402D7
	bool Runtimeproperty(CString objname, CString propname);
	//##ModelId=3973074402CD
	bool Runtimemethod (CString objname, CString propname);
	//##ModelId=3973074402CC
	CRuntime();
	//##ModelId=3973074402C3
	virtual ~CRuntime();
	//##ModelId=3973074402B9
	void Cargar(ICPropBag * pval1,ICPropBag * pval2);
private:
	//##ModelId=39730744029C
	bool Isencprop (ICPropBag *pval, CString propname, short tipo);
	//##ModelId=397307440290
	bool Isaggreagates(CString propname, CString nomgrupo, short tipo);
	//##ModelId=397307440286
	bool Isruntime(ICPropCol * *pval, short tipo);
	//##ModelId=397307440272
	bool Encmethod (long obj, CString propname, short tipo);
	//##ModelId=39730744026A
	ICPropBag *m_pPBObjects;
	//##ModelId=397307440256
	ICPropBag *m_pPBPropertysgroup;
};

#endif // !defined(AFX_RUNTIME_H__B5B17F04_E02A_11D2_BD0B_00A0248A69E8__INCLUDED_)
