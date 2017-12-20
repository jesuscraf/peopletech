#ifndef CM4STYLE_HPP
#define CM4STYLE_HPP

#include "m4detree.hpp"
#include "string.h"
#include "stdlib.h"

struct stDictionary
{
	CString m_csProperty;
	CString m_csValue;
};

class CM4Style
{
private:
	stDictionary *m_pstDictionary;
	long m_lNextEmpty;
	long m_lSize;

public:
	CM4Style (long a_lSize);
	~CM4Style (void);
	BOOL M4Insert (LPCTSTR a_szProperty, LPCTSTR a_szValue);
	BOOL M4FindProperty (LPCTSTR a_szProperty, long &lIndex);
	void M4DeleteProperty (long a_lIndex);

#include "cm4style.inl"

};

#endif
