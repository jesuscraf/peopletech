// Handle.h: interface for the CHandle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLE_H__93450F89_481D_11D4_BE06_00C04F71652D__INCLUDED_)
#define AFX_HANDLE_H__93450F89_481D_11D4_BE06_00C04F71652D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=396DF30B032B
class CM4Handle  
{
public:
	//##ModelId=396DF30B036A
	BOOL BeginDrag(HWND hWndHook, HWND hWndPaint, POINT pt, POINT ptStatic, long GridSize);
	//##ModelId=396DF30B0368
	BOOL MoveHandle(HWND hWnd);
	//##ModelId=396DF30B035F
	BOOL HitTest(int oX, int oY);
	//##ModelId=396DF30B0356
	HRESULT DrawHandle(HDC dc, BOOL bActive);
	
	//##ModelId=396DF30B0341
	long X;
	//##ModelId=396DF30B0340
	long Y;


	//##ModelId=396DF30B0355
	CM4Handle();
	//##ModelId=396DF30B034D
	virtual ~CM4Handle();
protected:
	//##ModelId=396DF30B0342
	void DrawTempLine(HDC dc, POINT ptA, POINT ptB);
};

#endif // !defined(AFX_HANDLE_H__93450F89_481D_11D4_BE06_00C04F71652D__INCLUDED_)
