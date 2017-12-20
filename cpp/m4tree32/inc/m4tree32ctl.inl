
long Middle (int a_lValue)
{
	return ((long) ((double) a_lValue / 2.0));
}

long Middle (long a_lValue)
{
	return ((long) ((double) a_lValue / 2.0));
}

void OffsetRect(long &a_lLeft, long &a_lTop, long &a_lRight, long &a_lBottom)
{
	a_lLeft = (long) ((((double) (a_lLeft + m_sLeftOffset))*m_dScale) + m_lOffsetScrollX);
	a_lRight = (long) ((((double) (a_lRight + m_sLeftOffset))*m_dScale) + m_lOffsetScrollX);
	a_lTop = (long) ((((double) (a_lTop + m_sTopOffset))*m_dScale) + m_lOffsetScrollY);
	a_lBottom = (long) ((((double) (a_lBottom + m_sTopOffset))*m_dScale) + m_lOffsetScrollY);
}

void Draw3dRect(CDC *dc, LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	int x = lpRect->left;
	int y = lpRect->top;
	int cx = lpRect->right - lpRect->left;
	int cy = lpRect->bottom - lpRect->top;
	FillSolidRect(dc, x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(dc, x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(dc, x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(dc, x, y + cy, cx, -1, clrBottomRight);
}

void FillSolidRect(CDC *pdc, int x, int y, int cx, int cy, COLORREF clr)
{
	COLORREF oldClr = pdc->SetBkColor(clr);
	CRect rect(x, y, x + cx, y + cy);
	pdc->ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
	pdc->SetBkColor(oldClr);
}

CM4Tree *GetPTree (void)
{
	return m_pTree;
}

void SetPrinting (BOOL a_bPrinting)
{
	m_bPrinting = a_bPrinting;
}

void SetOffsetScrollX (long a_lOffsetScrollX)
{
	m_lOffsetScrollX = a_lOffsetScrollX;
}

void SetOffsetScrollY (long a_lOffsetScrollY)
{
	m_lOffsetScrollY = a_lOffsetScrollY;
}

long GetOffsetScrollX (void)
{
	return m_lOffsetScrollX;
}

long GetOffsetScrollY (void)
{
	return m_lOffsetScrollY;
}

short GetLeftOffset (void)
{
	return m_sLeftOffset;
}

short GetTopOffset (void)
{
	return m_sTopOffset;
}

void SetScale (double a_dScale)
{
	m_dScale = a_dScale;
}

double GetScale (void)
{
	return m_dScale;
}

long GetValueLong (CM4Node *a_pNode, LPCTSTR a_szControl, LPCTSTR a_szProperty)
{
	CString csProp (a_szControl);
	csProp += a_szProperty;
	return GetValueLong (a_pNode, csProp);
}

LPCTSTR GetValueString (CM4Node *a_pNode, LPCTSTR a_szControl, LPCTSTR a_szProperty)
{
	CString csProp (a_szControl);
	csProp += a_szProperty;
	return GetValueString (a_pNode, csProp);
}

long GetValueLong (CM4Node *a_pNode, char a_cControl, LPCTSTR a_szProperty)
{
	CString csProp (a_cControl);
	csProp += a_szProperty;
	return GetValueLong (a_pNode, csProp);
}

LPCTSTR GetValueString (CM4Node *a_pNode, char a_cControl, LPCTSTR a_szProperty)
{
	CString csProp (a_cControl);
	csProp += a_szProperty;
	return GetValueString (a_pNode, csProp);
}

