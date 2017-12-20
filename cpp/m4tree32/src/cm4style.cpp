
#include "cm4style.hpp"


CM4Style::CM4Style (long a_lSize)
{
	m_lNextEmpty = 0L;
	m_lSize = a_lSize;
	m_pstDictionary = new stDictionary[a_lSize];
	if (m_pstDictionary == NULL)
		return;
	for (long i=0L; i<m_lSize; i++)
	{
		m_pstDictionary[i].m_csProperty.Empty ();
		m_pstDictionary[i].m_csValue.Empty ();
	}
}

CM4Style::~CM4Style (void)
{
	if (m_pstDictionary)
	{
		for (long i=0L; i<m_lSize; i++)
		{
			m_pstDictionary[i].m_csProperty.Empty ();
			m_pstDictionary[i].m_csValue.Empty ();
		}
		M4DELETEV(m_pstDictionary);
	}
	m_pstDictionary = NULL;
}

BOOL CM4Style::M4Insert (LPCTSTR a_szProperty, LPCTSTR a_szValue)
{
	long lIndex = 0L;
	BOOL bResult = M4FindProperty (a_szProperty, lIndex);
	if (bResult == TRUE)
	{
		m_pstDictionary[lIndex].m_csValue = a_szValue;
		return TRUE;
	}
	if (m_lNextEmpty == m_lSize)
	{
		stDictionary *pstDictionary = new stDictionary[m_lSize+16];
		if (pstDictionary == NULL)
			return FALSE;
		for (long i=0L; i<m_lSize; i++)
		{
			pstDictionary[i].m_csProperty = m_pstDictionary[i].m_csProperty;
			pstDictionary[i].m_csValue = m_pstDictionary[i].m_csValue;
		}
		for (long j=m_lSize; j<m_lSize+16; j++)
		{
			pstDictionary[j].m_csProperty.Empty ();
			pstDictionary[j].m_csValue.Empty ();
		}
		for (long k=0L; k<m_lSize; k++)
		{
			m_pstDictionary[k].m_csProperty.Empty ();
			m_pstDictionary[k].m_csValue.Empty ();
		}
		m_lSize = m_lSize+16;
		delete [] m_pstDictionary;
		m_pstDictionary = pstDictionary;
	}
	for (long i=m_lNextEmpty-1; i>=lIndex ; i--)
	{
		m_pstDictionary[i+1].m_csProperty = m_pstDictionary[i].m_csProperty;
		m_pstDictionary[i+1].m_csValue = m_pstDictionary[i].m_csValue;
	}
	m_pstDictionary[lIndex].m_csProperty = a_szProperty;
	m_pstDictionary[lIndex].m_csValue = a_szValue;

	m_lNextEmpty++;
	return TRUE;
}
	
BOOL CM4Style::M4FindProperty (LPCTSTR a_szProperty, long &a_lIndex)
{
	long lLeftIndex = 0L;
	long lRightIndex = m_lNextEmpty-1;
	a_lIndex = 0L;
	while (lLeftIndex <= lRightIndex)
	{
		long lCenterIndex = (long) ((lLeftIndex + lRightIndex) / 2.0);
		int iResult = m_pstDictionary[lCenterIndex].m_csProperty.Compare (a_szProperty);
		if (iResult > 0)
		{
			a_lIndex = lCenterIndex;
			lRightIndex = lCenterIndex - 1;
		}
		else
		{
			if (iResult < 0)
			{
				a_lIndex = lCenterIndex + 1;
				lLeftIndex = lCenterIndex + 1;
			}
			else
			{
				a_lIndex = lCenterIndex;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CM4Style::M4DeleteProperty (long a_lIndex)
{
	m_pstDictionary[a_lIndex].m_csProperty.Empty ();
	m_pstDictionary[a_lIndex].m_csValue.Empty ();
	for (long i=a_lIndex; i<m_lNextEmpty; i++)
	{
		m_pstDictionary[i].m_csProperty = m_pstDictionary[i+1].m_csProperty;
		m_pstDictionary[i].m_csValue = m_pstDictionary[i+1].m_csValue;
	}
	m_lNextEmpty--;
}

