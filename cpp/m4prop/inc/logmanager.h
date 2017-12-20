// LogManager.h: interface for the CLogManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMANAGER_H__7BEEC593_4A34_11D3_BD06_00C04F71652D__INCLUDED_)
#define AFX_LOGMANAGER_H__7BEEC593_4A34_11D3_BD06_00C04F71652D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CLogManager  
{
public:
	char * GetBuffer();
	long Write(char * pChar);
	void Init();
	CLogManager();
	virtual ~CLogManager();

private:
	BOOL TestBufferSize(long nSize);
	char * m_pBuffer;
	char * m_pBufferActual;
	char * m_pBufferFinal;

};

#endif // !defined(AFX_LOGMANAGER_H__7BEEC593_4A34_11D3_BD06_00C04F71652D__INCLUDED_)
