
#include "cm4bitmp.hpp"


CM4Bitmp::CM4Bitmp (long a_lSize)
{
	m_lNextEmpty = 0L;
	m_lSize = a_lSize;
	m_pstDictBitmap = new stDictBitmap[a_lSize];
	if (m_pstDictBitmap == NULL)
		return;
	for (long i=0L; i<m_lSize; i++)
	{
		m_pstDictBitmap[i].m_csBitmap.Empty ();
		m_pstDictBitmap[i].m_pBitmap = NULL;
		m_pstDictBitmap[i].m_bDelete = TRUE;
		m_pstDictBitmap[i].m_lWidth = 0L;
		m_pstDictBitmap[i].m_lHeight = 0L;	
	}
}

CM4Bitmp::~CM4Bitmp (void)
{
	if (m_pstDictBitmap)
	{
		M4DeleteAll ();
		M4DELETEV(m_pstDictBitmap);
	}
}

BOOL CM4Bitmp::M4Insert (LPCTSTR a_szBitmap, CBitmap *a_pBitmap, BOOL a_bDelete)
{
	BITMAP bm;
	long lIndex = 0L;
	BOOL bResult = M4Find (a_szBitmap, lIndex);
	if (bResult == TRUE)
	{
		return FALSE;
	}
	if (m_lNextEmpty == m_lSize)
	{
		stDictBitmap *pstDictBitmap = new stDictBitmap[m_lSize+16];
		if (pstDictBitmap == NULL)
			return FALSE;
		for (long i=0L; i<m_lSize; i++)
		{
			pstDictBitmap[i].m_csBitmap = m_pstDictBitmap[i].m_csBitmap;
			pstDictBitmap[i].m_pBitmap = m_pstDictBitmap[i].m_pBitmap;
			pstDictBitmap[i].m_bDelete = m_pstDictBitmap[i].m_bDelete;
			pstDictBitmap[i].m_lWidth = m_pstDictBitmap[i].m_lWidth;
			pstDictBitmap[i].m_lHeight = m_pstDictBitmap[i].m_lHeight;
		}
		for (long j=m_lSize; j<m_lSize+16; j++)
		{
			pstDictBitmap[j].m_csBitmap.Empty ();
			pstDictBitmap[j].m_pBitmap = NULL;
			pstDictBitmap[j].m_bDelete = FALSE;
			pstDictBitmap[j].m_lWidth = 0L;
			pstDictBitmap[j].m_lHeight = 0L;	
		}
		for (long k=0L; k<m_lSize; k++)
		{
			m_pstDictBitmap[k].m_csBitmap.Empty ();
			m_pstDictBitmap[k].m_pBitmap = NULL;
		}
		m_lSize = m_lSize+16;
		M4DELETEV(m_pstDictBitmap);
		m_pstDictBitmap = pstDictBitmap;
	}
	for (long i=m_lNextEmpty-1; i>=lIndex ; i--)
	{
		m_pstDictBitmap[i+1].m_csBitmap = m_pstDictBitmap[i].m_csBitmap;
		m_pstDictBitmap[i+1].m_pBitmap = m_pstDictBitmap[i].m_pBitmap;
		m_pstDictBitmap[i+1].m_bDelete = m_pstDictBitmap[i].m_bDelete;
		m_pstDictBitmap[i+1].m_lWidth = m_pstDictBitmap[i].m_lWidth;
		m_pstDictBitmap[i+1].m_lHeight = m_pstDictBitmap[i].m_lHeight;
	}
	m_pstDictBitmap[lIndex].m_csBitmap = a_szBitmap;
	m_pstDictBitmap[lIndex].m_pBitmap = a_pBitmap;
	m_pstDictBitmap[lIndex].m_bDelete = a_bDelete;
	m_pstDictBitmap[lIndex].m_pBitmap->GetObject (sizeof(bm), &bm);
	m_pstDictBitmap[lIndex].m_lWidth = (long) bm.bmWidth;
	m_pstDictBitmap[lIndex].m_lHeight = (long) bm.bmHeight;

	m_lNextEmpty++;
	return TRUE;
}
	
BOOL CM4Bitmp::M4Find (LPCTSTR a_szBitmap, long &a_lIndex)
{
	long lLeftIndex = 0L;
	long lRightIndex = m_lNextEmpty-1;
	a_lIndex = 0L;
	while (lLeftIndex <= lRightIndex)
	{
		long lCenterIndex = (long) ((lLeftIndex + lRightIndex) / 2.0);
		int iResult = m_pstDictBitmap[lCenterIndex].m_csBitmap.Compare (a_szBitmap);
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

void CM4Bitmp::M4DeleteAll (void)
{
	for (long i=0L; i<m_lNextEmpty; i++)
	{
		m_pstDictBitmap[i].m_csBitmap.Empty ();
		m_pstDictBitmap[i].m_pBitmap = NULL;
	}
	m_lNextEmpty=0L;
}

CBitmap *CM4Bitmp::M4GetBitmap (long a_lIndex)
{
	return m_pstDictBitmap[a_lIndex].m_pBitmap;
}

BOOL CM4Bitmp::M4GetDeleteBitmap (long a_lIndex)
{
	return m_pstDictBitmap[a_lIndex].m_bDelete;
}

long CM4Bitmp::M4GetWidthBitmap (long a_lIndex)
{
	return m_pstDictBitmap[a_lIndex].m_lWidth;
}

long CM4Bitmp::M4GetHeightBitmap (long a_lIndex)
{
	return m_pstDictBitmap[a_lIndex].m_lHeight;
}

long CM4Bitmp::M4GetCount (void)
{
	return m_lNextEmpty;
}

