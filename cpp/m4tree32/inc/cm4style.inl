

BOOL M4Insert (LPCTSTR a_szProperty, long a_lValue)
{
	char szValue[20];
	ltoa (a_lValue, szValue, 10);
	return M4Insert (a_szProperty, szValue);
}

LPCTSTR M4GetProperty (long a_lIndex)
{
	return m_pstDictionary[a_lIndex].m_csProperty;
}

LPCTSTR M4GetValueString (long a_lIndex)
{
	return m_pstDictionary[a_lIndex].m_csValue;
}

long M4GetValueLong (long a_lIndex)
{
	return atol(m_pstDictionary[a_lIndex].m_csValue);
}

long M4GetNumberProperties (void)
{
	return m_lNextEmpty;
}

