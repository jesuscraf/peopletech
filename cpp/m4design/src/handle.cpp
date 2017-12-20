// Handle.cpp: implementation of the CHandle class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "resource.h"
#include "handle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=396AF99902D5
CM4Handle::CM4Handle()
{
	
}

//##ModelId=396AF99902CD
CM4Handle::~CM4Handle()
{
	
}

//##ModelId=396AF99902D6
HRESULT CM4Handle::DrawHandle(HDC dc, BOOL bActive)
{
	HDC memDC = CreateCompatibleDC(dc);
	HBITMAP hBitmap = NULL;
	
	if (bActive)
		hBitmap = LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_HANDLEENABLED));
	else
		hBitmap = LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_HANDLEDISABLED));
	
	if (hBitmap)
	{
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

		BitBlt(dc,X-3,Y-3,X+4, Y+4, memDC, 0,0, SRCCOPY);
			
		SelectObject(memDC, hOldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(memDC);
	}

	return S_OK;

}

//##ModelId=396AF99902F2
BOOL CM4Handle::HitTest(int oX, int oY)
{

	int dX = oX - X + 3;
	int dY = oY - Y + 3;

	if ((dX < 7) && (dX >= 0) && (dY < 7) && (dY >= 0))
		return TRUE;
	else
		return FALSE;

}

//##ModelId=396AF99902F5
BOOL CM4Handle::MoveHandle(HWND hWnd)
{
	/*for (;;)
	{
		MSG msg;
		::GetMessage(&msg, NULL, 0, 0);

		if (GetCapture() != hWnd)
			break;

		switch (msg.message)
		{
			// handle movement/accept messages
			case WM_LBUTTONUP:
			case WM_MOUSEMOVE:
				break;

			// handle cancel messages
			case WM_KEYDOWN:
					break;
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			// just dispatch rest of the messages
			default:
				DispatchMessage(&msg);
				break;
		}

	}

ExitLoop:*/
	return TRUE;
	
}

//##ModelId=396AF99902FD
BOOL CM4Handle::BeginDrag(HWND hWndHook, HWND hWndPaint, POINT pt, POINT ptStatic, long GridSize)
{
	// Marchando una Bomba de Mensajes
	BOOL		bRepeat = TRUE;
	POINT		ptInt;
	POINT		ptInicial;
	
	POINT		ptAbs;
	POINT		ptTemp;

	ptAbs.x = X;
	ptAbs.y = Y;

	ptTemp.x = X;
	ptTemp.y = Y;
	
	ptInicial.x = pt.x;
	ptInicial.y = pt.y;

	HDC dc = GetDC(hWndPaint);

	DrawTempLine(dc, ptStatic, ptTemp);

	SetCapture(hWndHook);
	while(bRepeat)
	{
		MSG msg;
		::GetMessage(&msg, NULL, 0, 0);

		if (GetCapture() != hWndHook)
		{
			bRepeat = FALSE;
			break;
		}
			

		switch (msg.message)
		{
			// handle movement/accept messages
			case WM_LBUTTONUP:
				bRepeat = FALSE;
				X = ptTemp.x;
				Y = ptTemp.y;
				break;

			case WM_MOUSEMOVE:
				GetCursorPos(&ptInt);
				ScreenToClient(hWndPaint, &ptInt);
				
				DrawTempLine(dc, ptStatic, ptTemp);
				if (GridSize)
				{
					ptInt.x += GridSize/2;
					ptInt.y += GridSize/2;
					ptTemp.x = ptAbs.x + (ptInt.x - ptInicial.x);
					ptTemp.y = ptAbs.y + (ptInt.y - ptInicial.y);
					ptTemp.x = ptTemp.x - ptTemp.x%GridSize;
					ptTemp.y = ptTemp.y - ptTemp.y%GridSize;
				}
				else
				{
					ptTemp.x = ptAbs.x + (ptInt.x - ptInicial.x);
					ptTemp.y = ptAbs.y + (ptInt.y - ptInicial.y);
				}

				DrawTempLine(dc, ptStatic, ptTemp);
				break;

			// handle cancel messages
			case WM_KEYDOWN:
				bRepeat = FALSE;
				break;
			case WM_RBUTTONDOWN:
				bRepeat = FALSE;
				break;

			// just dispatch rest of the messages
			default:
				DispatchMessage(&msg);
				break;
		}

	}

	ReleaseDC(hWndHook, dc);
	//InvalidateRect(hWnd, NULL, FALSE);
	ReleaseCapture();
	return TRUE;
}

//##ModelId=396AF99902C1
void CM4Handle::DrawTempLine(HDC dc, POINT ptA, POINT ptB)
{
	int OldRop2 = SetROP2(dc, R2_NOTXORPEN);

	HPEN pen;
	pen = CreatePen(PS_SOLID, 1, RGB(192,192,192));
	HPEN oldPen = (HPEN) SelectObject(dc, pen);
	MoveToEx(dc, ptA.x, ptA.y, NULL);
	LineTo(dc, ptB.x, ptB.y);
	SelectObject(dc, oldPen);
	SetROP2(dc, OldRop2);

	DeleteObject (pen);
}
