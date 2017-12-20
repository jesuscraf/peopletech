// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "m4design.h"
#include "resource.h"
#include "handle.h"
#include "line.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=396AF9970264
CLine::CLine()
{
	m_hWndHook = NULL;	
	m_hWndPaint = NULL;
	m_Color = 0;
	m_nWidth = 1;
	m_eStyle = M4DESIGN_STYLE_SOLID;
	m_eSelected = M4DESIGN_NOTSELECTED;
	m_pNext = NULL;
	m_pSelectedNext = NULL;
	m_eEndCap = M4DESIGN_ROUND;
	m_eLineDrag = M4DESIGN_LINE_DRAG_NORMAL;
}

//##ModelId=396AF9970259
CLine::~CLine()
{
	free(m_strContainer);
	free(m_strKey);
}

//##ModelId=396AF9970265
BOOL CLine::HandleHitTest(POINT pt)
{
	// En el caso de que no se permita Drag, directamente decimos que no
	if (m_eLineDrag == M4DESIGN_LINE_DRAG_NORMAL)
	{
		// En caso de estar seleccionados, hacemos hittest sobre los puntos
		if ((m_eSelected == M4DESIGN_SELECTEDNORMAL) || (m_eSelected == M4DESIGN_SELECTEDSECONDARY))
		{
			if (m_pA.HitTest(pt.x, pt.y) || m_pB.HitTest(pt.x, pt.y))
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

//##ModelId=396AF997032B
BOOL CLine::LineHitTest(POINT pt)
{
	// En caso de no permitir Drag, nos vamos directamente
	if (m_eLineDrag != M4DESIGN_LINE_DRAG_NORMAL)
		return FALSE;
	
	
	// Primero, las comprobaciones triviales.

	// Qué pasa si son líneas rectas?
	if (m_pA.X == m_pB.X)
	{
		if ((abs((pt.x - m_pA.X)) < 4) && ((abs((pt.x - m_pA.X)) < 4)))
		{
			// Ahora hay que mirar si está dentro
			if (m_pA.Y < m_pB.Y)
				return ((pt.y >= m_pA.Y) && (pt.y <= m_pB.Y));
			else
				return ((pt.y >= m_pB.Y) && (pt.y <= m_pA.Y));
		}
	}

	if (m_pA.Y == m_pB.Y)
	{
		if ((abs((pt.y - m_pA.Y)) < 4) && ((abs((pt.y - m_pA.Y)) < 4)))
		{
			// Ahora hay que mirar si está dentro
			if (m_pA.X < m_pB.X)
				return ((pt.x >= m_pA.X) && (pt.x <= m_pB.X));
			else
				return ((pt.x >= m_pB.X) && (pt.x <= m_pA.X));
		}
	}
	
	// hay que aplicar puntos de tolerancia
	if (m_pA.X > m_pB.X)
	{
		if (pt.x > (m_pA.X+4))
			return FALSE;
		if (pt.x < (m_pB.X-4))
			return FALSE;
	}
	else
	{
		if (pt.x > (m_pB.X+4))
			return FALSE;
		if (pt.x < (m_pA.X-4))
			return FALSE;
	}
	

	if (m_pA.Y > m_pB.Y)
	{
		if (pt.y > (m_pA.Y+4))
			return FALSE;
		if (pt.y < (m_pB.Y-4))
			return FALSE;
	}
	else
	{
		if (pt.y > (m_pB.Y+4))
			return FALSE;
		if (pt.y < (m_pA.Y-4))
			return FALSE;
	}

	// Si esto no nos ha filtrado, pasamos al cálculo complejo
	double fMa,fMb,fCa,fCb;
	// Hay que aplicar cercanía a la línea.

	fMa = (double)(m_pB.Y - m_pA.Y)/(double)(m_pB.X - m_pA.X);
	fCa = m_pB.Y - fMa*(double)m_pB.X;

	// La ecuación de la perpendicular que pasa por el punto de test
	fMb = -(1.0/fMa);
	fCb = pt.y - fMb*pt.x;

	// Calculamos el punto de corte
	POINT pCorte;
	double xTemp = (fCb-fCa)/(fMa-fMb);
	pCorte.x = (long)xTemp;
	pCorte.y = (long)(fMa*xTemp + fCa);

	long dist = (pt.x-pCorte.x)*(pt.x-pCorte.x)+(pt.y-pCorte.y)*(pt.y-pCorte.y);
	return (dist<17);
}

//##ModelId=396AF9970335
HRESULT CLine::DrawLine(HDC dc)
{
	HPEN pen;
	COLORREF color;
	LOGBRUSH lBrush;
	DWORD nStyle = PS_GEOMETRIC;
	
	switch(m_eEndCap)
	{
		case M4DESIGN_ROUND:
			nStyle |= PS_ENDCAP_ROUND;
			break;
		case M4DESIGN_SQUARE:
			nStyle |= PS_ENDCAP_SQUARE;
			break;
		case M4DESIGN_FLAT:
			nStyle |= PS_ENDCAP_FLAT;
			break;
	}

	OleTranslateColor(m_Color, NULL, &color);
	lBrush.lbColor = color;
	lBrush.lbStyle = BS_SOLID;
	lBrush.lbHatch = 0;
	
	switch (m_eStyle)
	{
		case M4DESIGN_STYLE_SOLID:
			nStyle |= PS_SOLID;
			break;
		case M4DESIGN_STYLE_DASH:
			nStyle |= PS_DASH;
			break;
		case M4DESIGN_STYLE_DOT:
			nStyle |= PS_DOT;
			break;
		case M4DESIGN_STYLE_DASHDOT:
			nStyle |= PS_DASHDOT;
			break;
		case M4DESIGN_STYLE_DASHDOTDOT:
			nStyle |= PS_DASHDOTDOT;
			break;
		default:
			break;
	}
	
	pen = ExtCreatePen(nStyle, m_nWidth, &lBrush, 0, NULL);
	HPEN oldPen = (HPEN)SelectObject(dc, pen);
	
	MoveToEx(dc, m_pA.X, m_pA.Y, NULL);
	
	LineTo(dc, m_pB.X, m_pB.Y);
	
	SelectObject(dc, oldPen);
	DeleteObject(pen);
	if (m_eSelected == M4DESIGN_SELECTEDNORMAL)
	{
		m_pA.DrawHandle(dc, TRUE);
		m_pB.DrawHandle(dc, TRUE);
	}
	else if (m_eSelected == M4DESIGN_SELECTEDSECONDARY)
	{
		m_pA.DrawHandle(dc, FALSE);
		m_pB.DrawHandle(dc, FALSE);
	}
	
	return S_OK;
}



//##ModelId=396AF9970349
BOOL CLine::BeginDrag(POINT pt, long GridSize)
{
	POINT tmp;
	RECT rcOld;
	RECT rcNew;
	BOOL bMoved = FALSE;
	
	//InvalidateLine();
	
	if (m_pA.HitTest(pt.x, pt.y))
	{
		// Hacemos el Drag del m_pA
		tmp.x = m_pB.X;
		tmp.y = m_pB.Y;
		AbsRect(&rcOld);
		m_pA.BeginDrag(m_hWndHook, m_hWndPaint, pt, tmp, GridSize);
	}
	else if (m_pB.HitTest(pt.x, pt.y))
	{
		// Hacemos el Drag del m_pB
		tmp.x = m_pA.X;
		tmp.y = m_pA.Y;
		AbsRect(&rcOld);
		m_pB.BeginDrag(m_hWndHook, m_hWndPaint, pt, tmp, GridSize);
	}
	
	AbsRect(&rcNew);

	if ((rcOld.top != rcNew.top) || (rcOld.left != rcNew.left) || (rcOld.bottom != rcNew.bottom) || (rcOld.right != rcNew.right))
		bMoved = TRUE;
	
	rcOld.left -= m_nWidth;
	rcOld.right += m_nWidth;
	rcOld.top -= m_nWidth;
	rcOld.bottom += m_nWidth;
	
	InvalidateRect(m_hWndPaint, &rcOld, FALSE);
	InvalidateLine();
	return bMoved;
}


//##ModelId=396AF997035D
BOOL CLine::BeginLineDrag(POINT pt, long GridSize)
{
	BOOL		bRepeat = TRUE;
	BOOL		bMoved = FALSE;
	POINT		ptInt;
	POINT		ptInicial;
	POINT		ptAAbs;
	POINT		ptBAbs;
	POINT		ptATemp;
	POINT		ptBTemp;

	ptAAbs.x = m_pA.X;
	ptAAbs.y = m_pA.Y;
	ptBAbs.x = m_pB.X;
	ptBAbs.y = m_pB.Y;
	
	ptATemp.x = m_pA.X;
	ptATemp.y = m_pA.Y;
	ptBTemp.x = m_pB.X;
	ptBTemp.y = m_pB.Y;

	ptInicial.x = pt.x;
	ptInicial.y = pt.y;

	RECT rc;
	
	
	AbsRect(&rc);
	// Dibujamos la primera posición
	HDC dc = GetDC(m_hWndPaint);

	M4SelectionType eSel = m_eSelected;

	m_eSelected = M4DESIGN_NOTSELECTED;
	
	InvalidateHandles();

	DrawTempLine(dc, ptATemp, ptBTemp);

	SetCapture(m_hWndHook);
	while(bRepeat)
	{
		MSG msg;
		::GetMessage(&msg, NULL, 0, 0);
		BOOL bDGrid = (msg.wParam & MK_SHIFT) && (msg.wParam & MK_CONTROL);
		long dX,dY;

		if (GetCapture() != m_hWndHook)
		{
			bRepeat = FALSE;
			break;
		}
			

		switch (msg.message)
		{
			
			// handle movement/accept messages
			case WM_LBUTTONUP:
				bRepeat = FALSE;
				m_pA.X = ptATemp.x;
				m_pA.Y = ptATemp.y;
				m_pB.X = ptBTemp.x;
				m_pB.Y = ptBTemp.y;
				if ((ptAAbs.x != m_pA.X) || (ptAAbs.y != m_pA.Y) || (ptBAbs.x != m_pB.X) || (ptBAbs.y != m_pB.Y))
					bMoved = TRUE;
				break;

			case WM_MOUSEMOVE:
				GetCursorPos(&ptInt);
				ScreenToClient(m_hWndPaint, &ptInt);

				// Borramos la anterior
				DrawTempLine(dc, ptATemp, ptBTemp);
				
				if (GridSize && (!bDGrid))
				{
					ptInt.x += GridSize/2;
					ptInt.y += GridSize/2;
					ptATemp.x = ptAAbs.x + (ptInt.x - ptInicial.x);
					ptATemp.y = ptAAbs.y + (ptInt.y - ptInicial.y);
					ptBTemp.x = ptBAbs.x + (ptInt.x - ptInicial.x);
					ptBTemp.y = ptBAbs.y + (ptInt.y - ptInicial.y);
					dX = ptATemp.x%GridSize;
					dY = ptATemp.y%GridSize;
					ptATemp.x -= dX;
					ptATemp.y -= dY;
					ptBTemp.x -= dX;
					ptBTemp.y -= dY;
				}
				else
				{
					// Hay que ajustar los dos puntos
					ptATemp.x = ptAAbs.x + (ptInt.x - ptInicial.x);
					ptATemp.y = ptAAbs.y + (ptInt.y - ptInicial.y);
					ptBTemp.x = ptBAbs.x + (ptInt.x - ptInicial.x);
					ptBTemp.y = ptBAbs.y + (ptInt.y - ptInicial.y);
				}
				DrawTempLine(dc, ptATemp, ptBTemp); // Dibujamos la nueva
				

				break;

			// handle cancel messages
			case WM_KEYDOWN:
				if (msg.wParam	== 27)
				{
					bRepeat = FALSE;
				}
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

	rc.left -= m_nWidth;
	rc.right += m_nWidth;
	rc.top -= m_nWidth;
	rc.bottom += m_nWidth;
	
	InvalidateRect(m_hWndPaint, &rc, FALSE);
	InvalidateLine();
	m_eSelected = eSel;
	ReleaseCapture();
	ReleaseDC(m_hWndPaint, dc);
	
	return bMoved;
}

//##ModelId=396AF9980002
void CLine::DrawTempLine(HDC dc, POINT ptA, POINT ptB)
{
	int OldRop2 = SetROP2(dc, R2_NOTXORPEN);

	HPEN pen;
	pen = CreatePen(PS_SOLID, 1, RGB(192,192,192));
	HPEN oldPen = (HPEN) SelectObject(dc, pen);
	
	MoveToEx(dc, ptA.x, ptA.y, NULL);
	LineTo(dc, ptB.x, ptB.y);
	SelectObject(dc, oldPen);
	SetROP2(dc, OldRop2);
	
	DeleteObject(pen);
	
}

//##ModelId=396AF997037C
void CLine::AbsRect(RECT *rc)
{

	if (m_pA.X > m_pB.X)
	{
		rc->left = m_pB.X;
		rc->right = m_pA.X;
	}
	else
	{
		rc->right = m_pB.X;
		rc->left = m_pA.X;
	}

	if (m_pA.Y < m_pB.Y)
	{
		rc->bottom = m_pB.Y;
		rc->top = m_pA.Y;
	}
	else
	{
		rc->top = m_pB.Y;
		rc->bottom = m_pA.Y;
	}

	// La engordamos para pillar los Handles
	rc->top -= 3;
	rc->bottom += 4;
	rc->left -= 3;
	rc->right += 4;

}

//##ModelId=396AF9970390
void CLine::InvalidateHandles()
{
	RECT rc;

	rc.top = m_pA.Y - 4;
	rc.left = m_pA.X - 4;
	rc.bottom = m_pA.Y + 4;
	rc.right = m_pA.X + 4;

	InvalidateRect(m_hWndPaint, &rc, FALSE);

	rc.top = m_pB.Y - 4;
	rc.left = m_pB.X - 4;
	rc.bottom = m_pB.Y + 4;
	rc.right = m_pB.X + 4;

	InvalidateRect(m_hWndPaint, &rc, FALSE);

}

//##ModelId=396AF9970391
void CLine::AbsRectReal(RECT *rc)
{
	if (m_pA.X > m_pB.X)
	{
		rc->left = m_pB.X;
		rc->right = m_pA.X;
	}
	else
	{
		rc->right = m_pB.X;
		rc->left = m_pA.X;
	}

	if (m_pA.Y < m_pB.Y)
	{
		rc->bottom = m_pB.Y;
		rc->top = m_pA.Y;
	}
	else
	{
		rc->top = m_pB.Y;
		rc->bottom = m_pA.Y;
	}
}

//##ModelId=396AF99703A4
void CLine::SetRect(RECT rc)
{
	RECT rc2;
	// Tendremos que invalidarnos Antes
	if (m_hWndPaint)
	{
		AbsRect(&rc2);
		InvalidateRect(m_hWndPaint, &rc2, FALSE);
	}
	
	
	m_pA.X = rc.left;
	m_pA.Y = rc.top;
	m_pB.X = rc.right;
	m_pB.Y = rc.bottom;

	// Tendremos que invalidarnos Despues
	if (m_hWndPaint)
	{
		AbsRect(&rc2);
		InvalidateRect(m_hWndPaint, &rc2, FALSE);
	}
}

//##ModelId=396AF99703AE
void CLine::MoveDef(long dX, long dY)
{
	// Invalidamos la posición anterior
	//RECT rc;

	InvalidateLine();
	
	//AbsRect(&rc);
	//InvalidateRect(m_hWndPaint, &rc, FALSE);

	m_pA.X += dX;
	m_pA.Y += dY;
	m_pB.X += dX;
	m_pB.Y += dY;
	
	//AbsRect(&rc);
	//InvalidateRect(m_hWndPaint, &rc, FALSE);
	InvalidateLine();
}

//##ModelId=396AF99703D6
void CLine::DrawTempLine(POINT ptA, POINT ptB)
{
	// Creamos un HDC asociado a nuestro Handle de ventana y dibujamos de la forma stándard
	if (m_hWndPaint)
	{
		HDC dc = GetDC(m_hWndPaint);
		DrawTempLine(dc, ptA, ptB);
		ReleaseDC(m_hWndPaint, dc);
	}

}

//##ModelId=396AF998002A
void CLine::InvalidateLine()
{
	RECT rc;
	AbsRect(&rc);
	
	rc.left -= m_nWidth;
	rc.right += m_nWidth;
	rc.top -= m_nWidth;
	rc.bottom += m_nWidth;

	InvalidateRect(m_hWndPaint, &rc, FALSE);
	InvalidateHandles();
}
