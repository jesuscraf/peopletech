#ifndef CM4BITMP_HPP
#define CM4BITMP_HPP

#include "m4detree.hpp"
#include "string.h"
#include "stdlib.h"

struct stDictBitmap
{
	CString m_csBitmap;
	CBitmap *m_pBitmap;
	BOOL m_bDelete;
	long m_lWidth;
	long m_lHeight;
};

class CM4Bitmp
{
private:
	stDictBitmap *m_pstDictBitmap;
	long m_lNextEmpty;
	long m_lSize;

public:
	CM4Bitmp (long a_lSize);
	~CM4Bitmp (void);
	BOOL M4Insert (LPCTSTR a_szBitmap, CBitmap *a_pBitmap, BOOL a_bDelete);
	BOOL M4Find (LPCTSTR a_szBitmap, long &lIndex);
	void M4DeleteAll (void);
	CBitmap *M4GetBitmap (long a_lIndex);
	long M4GetWidthBitmap (long a_lIndex);
	long M4GetHeightBitmap (long a_lIndex);
	BOOL M4GetDeleteBitmap (long a_lIndex);
	long M4GetCount (void);
};

#endif
