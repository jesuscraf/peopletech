// Messages.h: interface for the CMessages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGES_H__068EECB3_F0A4_11D2_BD21_00A0248A69E8__INCLUDED_)
#define AFX_MESSAGES_H__068EECB3_F0A4_11D2_BD21_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//##ModelId=397307370378
class CMessages  
{
public:
	//##ModelId=3973073703A2
	CMessages();
	//##ModelId=3973073703A0
	virtual ~CMessages();
	//##ModelId=397307370398
	void Cargar(ICPropBag * pval);
	//##ModelId=397307370396
	CString TextMessage (CString Code);
private:
	//##ModelId=39730737038C
	ICPropBag *m_pPBMessages;

};

#endif // !defined(AFX_MESSAGES_H__068EECB3_F0A4_11D2_BD21_00A0248A69E8__INCLUDED_)
