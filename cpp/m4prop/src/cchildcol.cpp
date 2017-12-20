// CChildCol.cpp : Implementation of CCChildCol
#include "stdafx.h"
#include "m4prop.h"
#include "cchildcol.h"
#include "cpropbag.h"
/////////////////////////////////////////////////////////////////////////////
// CCChildCol


STDMETHODIMP CCChildCol::InvalidateLength()
{
	// TODO: Add your implementation code here
	ICPropBag * pb;
	CMinNode* m_pNode;

	m_pNode = m_pNodeHead;

	while(m_pNode != NULL)
	{
		pb = m_pNode->pData;

		if( pb != NULL )
		{
			pb->Invalidatelength();
		}
		m_pNode = m_pNode->pNext;
	}

	return S_OK;
}

STDMETHODIMP CCChildCol::get_Length(long * pVal)
{
	ICPropBag * pb;
	CMinNode * pNode;
	long nLength;

	pNode = m_pNodeHead;

	nLength = 0;
	while (pNode != NULL)
	{
		long tmpVal;
		pb = pNode->pData;

		if( pb != NULL )
		{
			pb->get_Length(&tmpVal);
			nLength += tmpVal + 1 + Longlength(tmpVal);
		}
		pNode = pNode->pNext;
	}
	
	*pVal = nLength;
	return S_OK;

}

STDMETHODIMP CCChildCol::WriteASN(unsigned char** charIn)
{
	long tmpLength = 0;
	long tmpLongLength = 0;

	CMinNode* pNode = m_pNodeHead;
	while (pNode != NULL)
	{
		CCPropBag* pb = (CCPropBag*) pNode->pData;

		if( pb != NULL )
		{
			tmpLength = 0;
			pb->get_Length(&tmpLength);
			tmpLongLength = Longlength(tmpLength);
			*(*charIn) = ASNSequence;
			(*charIn)++;
			writeLength(charIn, tmpLength);
			pb->WriteASN(charIn);
		}
		pNode = pNode->pNext;
	}
	
	return S_OK;
}

STDMETHODIMP CCChildCol::WriteXML(ClLongString &roXML)
{
	CMinNode* pNode = m_pNodeHead;
	while (pNode != NULL)
	{
		CCPropBag* pb = (CCPropBag*) pNode->pData;

		if( pb != NULL )
		{
			pb->WriteXML(roXML);
		}
		pNode = pNode->pNext;
	}
	
	return S_OK;
}

void CCChildCol::PrePersistProccess()
{
	CCPropBag * pb;
	CMinNode * pNode;

	pNode = m_pNodeHead;
	while (pNode != NULL)
	{
		pb = (CCPropBag*) pNode->pData;

		if( pb != NULL )
		{
			pb->PrePersistProccess();
		}
		pNode = pNode->pNext;
	}

}
