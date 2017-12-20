// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__79936293_4851_11D4_BE06_00C04F71652D__INCLUDED_)
#define AFX_LINE_H__79936293_4851_11D4_BE06_00C04F71652D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=396DF30902FA
class CLine  
{
public:
	//##ModelId=396DF30A0117
	void InvalidateLine();
	//##ModelId=396DF30A001D
	char* m_strContainer;

	//##ModelId=396DF30A0009
	char* m_strKey;

	//##ModelId=396DF30903DF
	CLine* m_pNext;

	//##ModelId=396DF30903CB
	CLine* m_pSelectedNext;

	//##ModelId=396DF30A00DB
	void DrawTempLine(POINT ptA, POINT ptB);
	//##ModelId=396DF30A00BD
	void MoveDef(long dX, long dY);
//	void MoveTempDelta(HDC dc, long dX, long dY);
	//##ModelId=396DF30A00B3
	void SetRect(RECT rc);
	//##ModelId=396DF30A00A0
	void AbsRectReal(RECT *rc);
	//##ModelId=396DF30A009F
	void InvalidateHandles();
	//##ModelId=396DF30A0095
	void AbsRect(RECT * rc);
	//##ModelId=396DF30A0077
	BOOL BeginLineDrag(POINT pt, long GridSize);
	//##ModelId=396DF30A0063
	BOOL BeginDrag(POINT pt, long GridSize);
	//##ModelId=396DF30A004F
	HRESULT DrawLine(HDC dc);
	
	//##ModelId=396DF30A0045
	BOOL LineHitTest(POINT pt);
	//##ModelId=396DF30A0032
	BOOL HandleHitTest(POINT pt);
	//##ModelId=396DF30903B7
	M4SelectionType		m_eSelected;
	//##ModelId=396DF30903A3
	CM4Handle				m_pB;
	//##ModelId=396DF309038F
	CM4Handle				m_pA;
	//##ModelId=396DF3090383
	HWND				m_hWndHook;
	//##ModelId=396DF3090379
	HWND				m_hWndPaint;

	//##ModelId=396DF3090371
	OLE_COLOR			m_Color;
	//##ModelId=396DF309035B
	long				m_nWidth;
	//##ModelId=396DF3090347
	M4LineStyle			m_eStyle;

	//##ModelId=397C1CFF0356
	M4LineEndCap		m_eEndCap;

	M4LineDrag			m_eLineDrag;

	//##ModelId=396DF30A00F9
	void DrawTempLine(HDC dc, POINT ptA, POINT ptB);
	
	//##ModelId=396DF30A0031
	CLine();
	//##ModelId=396DF30A0027
	virtual ~CLine();
	
private:
	

protected:
	
};

#endif // !defined(AFX_LINE_H__79936293_4851_11D4_BE06_00C04F71652D__INCLUDED_)
