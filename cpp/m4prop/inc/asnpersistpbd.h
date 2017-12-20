// ASNPersistPBD.h: interface for the CASNPersistPBD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASNPERSISTPBD_H__DD7D4845_0DFF_11D3_BCAB_00C04F71652D__INCLUDED_)
#define AFX_ASNPERSISTPBD_H__DD7D4845_0DFF_11D3_BCAB_00C04F71652D__INCLUDED_

#include "logmanager.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CASNPersistPBD  
{
public:
	void PersistIdHelpTopic(BOOL bPersist);
	void SetLogManager(CLogManager * pLog);
	BSTR GetLangString(long nLang);
	BSTR GetMainString();

	HRESULT ProccessPBD(ICPropBagDef * pPBD);
	void ClearASNBuffers();
	void InitASNBuffers(ITraductProp * pTraduct);
	CASNPersistPBD();
	virtual ~CASNPersistPBD();

private:

	void ExpandBuffer();
	long PersistString(char * strValue);
	long PersistBool(short bBool);
	long PersistNumber(long nNumber);
	long PersistProperty(ICDefProp *pProp);
	long PersistLength(long nLength);
	ITraductProp	*m_pTraduct;

	BYTE * m_pASNBuffer;
	BYTE ** m_ppASNLangBuffer;
	
	BYTE * m_pASNBufferActual;
	BYTE ** m_ppASNLangBufferActual;
	
	BYTE * m_pASNBufferFinal;
	BYTE ** m_ppASNLangBufferFinal;

	long m_nASNLength, *m_pnASNLangLength;
	long m_nLangNumber;
	CDataProperties	*m_pDefData;

	char *m_pstrIdChannel;
	char *m_pstrIdChannelAlias;
	char *m_pstrIdChannelKey;
	char *m_pstrIdAccessAlias;
	char *m_pstrIdItem;
	char *m_pstrIdNode;
	short m_nGrants;
	long m_nIdPresentation;
	long m_nStartPos;
	long m_nRefCount;
	CLogManager * m_pLog;
	char * m_pTempBuffer;

private:
	BOOL m_bPersistIdHelpTopic;
	void ExpandLangBuffer(long nLang);
	void PersistLangLength(long nLang, long nLength);
	void PersistLangString(long nLang, char * strValue);
	void TerminateLangData();
	long PersistTraductProp(ITraductProp *pTraduct);
	long PersistDataProperties(CDataProperties *pActualData, CDataProperties *pInheritData);
	long PersistFont(IM4FontHolder *pFontV, IM4FontHolder *pFontDef);
	long RecProccess(ICPropBagDef * pPBD,CDataProperties * pData);
};

#endif // !defined(AFX_ASNPERSISTPBD_H__DD7D4845_0DFF_11D3_BCAB_00C04F71652D__INCLUDED_)
