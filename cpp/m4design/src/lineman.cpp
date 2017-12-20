// LineMan.cpp : Implementation of CLineMan
#include "stdafx.h"
#include "m4design.h"
#include "handle.h"
#include "linecol.h"
#include "line.h"
#include "dline.h"
#include "hook_data.h"
#include "lineman.h"

/////////////////////////////////////////////////////////////////////////////
// CLineMan

//##ModelId=396AF99B0331
STDMETHODIMP CLineMan::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ILineMan
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


//##ModelId=396AF99B00F6
STDMETHODIMP CLineMan::get_GridSize(long *pVal)
{
	*pVal = m_nGridSize;

	return S_OK;
}

//##ModelId=396AF99B00EC
STDMETHODIMP CLineMan::put_GridSize(long newVal)
{
	m_nGridSize = newVal;

	return S_OK;
}

//##ModelId=396AF99B010A
STDMETHODIMP CLineMan::AddLine(long hWndHook, long hWndPaint, BSTR key, BSTR contKey, long pAX, long pAY, long pBX, long pBY, IDesignLine ** pLine)
{
	USES_CONVERSION;

	char * pKey = OLE2A(key);
	CLine * pNext;
	
	
	if (pKey == NULL)
		return E_INVALIDARG;
	
	pNext = m_pData;
	
	// Primero buscamos, no vaya a ser que ya exista
	while(pNext && strcmp(pKey, pNext->m_strKey))
	{
		pNext = pNext->m_pNext;
	}

	if (pNext)
		return E_INVALIDARG; // La Key Ya Existe

	pNext = m_pData;
	
	if (pNext == NULL)
	{
		// Es el primer elemento
		pNext = new CLine;
		m_pData = pNext;
	}
	else
	{
		while(pNext->m_pNext) pNext = pNext->m_pNext;
		pNext->m_pNext = new CLine;
		pNext = pNext->m_pNext;
	}


	//memset(pNext, 0, sizeof(CLine));
	
	pNext->m_hWndHook = (HWND) hWndHook;
	pNext->m_hWndPaint = (HWND) hWndPaint;
	pNext->m_pA.X = pAX;
	pNext->m_pA.Y = pAY;
	pNext->m_pB.X = pBX;
	pNext->m_pB.Y = pBY;
	pNext->m_strKey = (char*)malloc(strlen(pKey)+1);
	strcpy(pNext->m_strKey, pKey);
	pKey = OLE2A(contKey);
	pNext->m_strContainer = (char*)malloc(strlen(pKey)+1);
	strcpy(pNext->m_strContainer, pKey);

	AddHook((HWND)hWndHook);
	AddHook((HWND)hWndPaint);

	pNext->m_eSelected =  M4DESIGN_NOTSELECTED;
	SetProp((HWND)hWndHook, "HProp", (HANDLE) this);
	SetProp((HWND)hWndPaint, "HPROP", (HANDLE) this);

	RECT rc;
	pNext->AbsRect(&rc);
	
	CComObject<CDLine> * pDLine;

	CComObject<CDLine>::CreateInstance(&pDLine);

	pDLine->m_oLine = pNext;
	pDLine->QueryInterface(IID_IDesignLine, (void**)pLine);
	pNext->InvalidateLine();
	return S_OK;
}

//##ModelId=396AF99B0179
STDMETHODIMP CLineMan::get_Selected(BSTR bKey, VARIANT_BOOL *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

//##ModelId=396AF99B015A
STDMETHODIMP CLineMan::put_Selected(BSTR bKey, VARIANT_BOOL newVal)
{
	USES_CONVERSION;
	
	char * cKey = OLE2A(bKey);
	
	SelectLine(cKey);
	
	return S_OK;
}

//##ModelId=396AF99B0197
void CLineMan::SelectLine(char *cKey)
{
	CLine * pData = m_pData;
	BOOL bFind = FALSE;

	while(pData)
	{
		if (strcmp(cKey, pData->m_strKey) == 0)
		{
			if (!pData->m_eSelected)
			{
				pData->m_eSelected = M4DESIGN_SELECTEDNORMAL;
				pData->InvalidateHandles();
				pData->m_pSelectedNext = NULL;
				LaunchLineSelected(pData);
				bFind = TRUE;
				m_pSelectedLines = pData;
			}
		}
		else
		{
			if (pData->m_eSelected)
			{
				pData->m_eSelected = M4DESIGN_NOTSELECTED;
				pData->m_pSelectedNext = NULL;
				pData->InvalidateHandles();
			}
		}
		
		pData = pData->m_pNext;
	}
}

//##ModelId=396AF99B01A1
void CLineMan::ButtonUp(HWND hWnd, POINT pt, long keyState)
{
	// Tenemos el KeyState, el hWnd y el Punto.
	BOOL bMulti = (keyState & MK_CONTROL) || (keyState & MK_SHIFT);
	CLine * pLast = NULL, * pTemp = NULL;
	BOOL bHit = FALSE;

	if (bMulti)
	{
		// Empiezan los análisis
		// Si se hace click sobre una línea que no está presente en la colección de elementos seleccionados.
		// Click sobre una línea Ya presente.
		// Click Sobre la nada
		
		// Si se hace click sobre otra ventana con Multi no hacemos caso
		if (m_pSelectedLines)
			if (m_pSelectedLines->m_hWndHook != hWnd)
				return;
		
		pTemp = m_pData;

		while (pTemp)
		{
			if ((pTemp->m_hWndHook == hWnd) && (pTemp->LineHitTest(pt)))
			{
				bHit = TRUE;
				if (TestInSelected(pTemp))
				{
					// Hay que eliminarla
					RemoveFromSelection(pTemp);
				}
				else
				{
					// Hay que añadirla a la selección
					AddToSelection(pTemp);
				}
			}
			pTemp = pTemp->m_pNext;
		}
	}
	else
	{
		// Sólo queda seleccionada la que se ha pulsado.
		// Deseleccionamos todo
		pTemp = m_pSelectedLines;
		CLine * pTemp2;

		while(pTemp)
		{
			pTemp->m_eSelected = M4DESIGN_NOTSELECTED;
			pTemp->InvalidateHandles();
			pTemp2 = pTemp;
			pTemp = pTemp->m_pSelectedNext;
			pTemp2->m_pSelectedNext = NULL;
		}
		m_pSelectedLines = NULL;

		//Probamos, hasta seleccionar la del premio
		pTemp = m_pData;
		while(pTemp)
		{
			if ((pTemp->m_hWndHook == hWnd) && (pTemp->LineHitTest(pt)))
			{
				// La seleccionamos
				pTemp->m_eSelected = M4DESIGN_SELECTEDNORMAL;
				LaunchLineSelected(pTemp);
				pTemp->InvalidateHandles();
				m_pSelectedLines = pTemp;
				pTemp->m_pSelectedNext = NULL;
				pTemp = NULL;
			}
			else
				pTemp = pTemp->m_pNext;
		}
	}

}

//##ModelId=396AF99B01BF
BOOL CLineMan::TestInSelected(CLine *pData)
{
	CLine * pTemp = m_pSelectedLines;

	while(pTemp)
	{
		if (pTemp == pData)
			return TRUE;
		
		pTemp = pTemp->m_pSelectedNext;
	}
	return FALSE;
}

//##ModelId=396AF99B01C9
void CLineMan::AddToSelection(CLine *pData)
{
	CLine * pTemp = m_pSelectedLines;

	if (pTemp)
	{
		// Tratamiento del primer elemento
		pTemp->m_eSelected = M4DESIGN_SELECTEDSECONDARY;
		pTemp->InvalidateHandles();

		// Entramos a los elementos contenidos
		while(pTemp->m_pSelectedNext)
		{
			pTemp = pTemp->m_pSelectedNext;
			pTemp->m_eSelected = M4DESIGN_SELECTEDSECONDARY;
			pTemp->InvalidateHandles();
		}
		pTemp->m_pSelectedNext = pData;
		pData->m_eSelected = M4DESIGN_SELECTEDNORMAL;
		//LaunchLineSelected(pData);
		pData->InvalidateHandles();
	}
	else
	{
		m_pSelectedLines = pData;
		m_pSelectedLines->m_pSelectedNext = NULL;
		pData->m_eSelected = M4DESIGN_SELECTEDNORMAL;
		//LaunchLineSelected(pData);
		pData->InvalidateHandles();
	}


}

//##ModelId=396AF99B01DD
void CLineMan::RemoveFromSelection(CLine *pData)
{
	CLine * pTemp = m_pSelectedLines;

	// Comprobamos que no sea el Primero
	if (pData == m_pSelectedLines)
	{
		if (pData->m_eSelected == M4DESIGN_SELECTEDNORMAL)
		{
			// Hay que seleccionar alguna como principal
			if (m_pSelectedLines->m_pSelectedNext)
			{
				m_pSelectedLines->m_pSelectedNext->m_eSelected = M4DESIGN_SELECTEDNORMAL;
				m_pSelectedLines->m_pSelectedNext->InvalidateHandles();
			}
		}
		pData->m_eSelected = M4DESIGN_NOTSELECTED;
		pData->InvalidateHandles();
		m_pSelectedLines = m_pSelectedLines->m_pSelectedNext;
		pData->m_pSelectedNext = NULL;
	}
	else
	{
		// Buscamos el elemento que es equivalente al de entrada
		while(pTemp && (pTemp->m_pSelectedNext != pData))
		{
			pTemp = pTemp->m_pSelectedNext;
		}
		// O el siguiente es el nuestro, o no se ha encontrado
		if (pTemp)
		{
			// Lo sacamos de la lista
			pTemp->m_pSelectedNext = pTemp->m_pSelectedNext->m_pSelectedNext;
			if (pData->m_eSelected == M4DESIGN_SELECTEDNORMAL)
			{
				// Hay que colocar alguno como Principal
				m_pSelectedLines->m_eSelected = M4DESIGN_SELECTEDNORMAL;
				m_pSelectedLines->InvalidateHandles();
			}
			pData->m_eSelected = M4DESIGN_NOTSELECTED;
			pData->InvalidateHandles();
			pData->m_pSelectedNext = NULL;
		}
	}

	// Queda por seleccionar el último que nos quede
}

//##ModelId=396AF99B01E7
void CLineMan::BeginMultiDrag(HWND hWnd, POINT pt)
{
	// Nos suscribimos a otra bomba de mensajes.
	m_bCanceled = FALSE;
	POINT ptInit, ptTemp, ptDelta;	
	BOOL bRepeat = TRUE;
	CLine * pTemp;
	
	ptInit.x = pt.x;
	ptInit.y = pt.y;
	ptDelta.x = 0;
	ptDelta.y = 0;

	// Dibujamos todas en estado inicial

	pTemp = m_pSelectedLines;
	while(pTemp)
	{
		POINT ptA,ptB;
		ptA.x = pTemp->m_pA.X;
		ptA.y = pTemp->m_pA.Y;
		ptB.x = pTemp->m_pB.X;
		ptB.y = pTemp->m_pB.Y;
		pTemp->DrawTempLine(ptA, ptB);
		pTemp = pTemp->m_pSelectedNext;
	}
	
	SetCapture(hWnd);
	
	while(bRepeat)
	{
		MSG msg;
		::GetMessage(&msg, NULL, 0, 0);

		if (GetCapture() != hWnd)
		{
			bRepeat = FALSE;
			break;
		}

		switch (msg.message)
		{
			
			// Gestión de éstos mensajes
			case WM_LBUTTONUP:
				// Finalizamos la tarea

				GetCursorPos(&ptTemp);
				ScreenToClient(hWnd, &ptTemp);
				

				// Borramos lo último pintado
				pTemp = m_pSelectedLines;
				while(pTemp)
				{
					POINT ptA,ptB;
					ptA.x = pTemp->m_pA.X - ptDelta.x;
					ptA.y = pTemp->m_pA.Y - ptDelta.y;
					ptB.x = pTemp->m_pB.X - ptDelta.x;
					ptB.y = pTemp->m_pB.Y - ptDelta.y;
					pTemp->DrawTempLine(ptA, ptB);
					pTemp = pTemp->m_pSelectedNext;
				}
				
				ptDelta.x = ptTemp.x - ptInit.x;
				ptDelta.y = ptTemp.y - ptInit.y;

				pTemp = m_pSelectedLines;
				while(pTemp)
				{
					pTemp->MoveDef(ptDelta.x, ptDelta.y);
					if (ptDelta.x || ptDelta.y)
						LaunchLineMoved(pTemp);
					pTemp = pTemp->m_pSelectedNext;
				}
				
				bRepeat = FALSE;
				break;

			case WM_MOUSEMOVE:
				// Movemos y notificamos movimiento a los objetos
				GetCursorPos(&ptTemp);
				ScreenToClient(hWnd, &ptTemp);
				
				pTemp = m_pSelectedLines;
				while(pTemp)
				{
					POINT ptA,ptB;
					ptA.x = pTemp->m_pA.X - ptDelta.x;
					ptA.y = pTemp->m_pA.Y - ptDelta.y;
					ptB.x = pTemp->m_pB.X - ptDelta.x;
					ptB.y = pTemp->m_pB.Y - ptDelta.y;
					pTemp->DrawTempLine(ptA, ptB);
					pTemp = pTemp->m_pSelectedNext;
				}
				
				ptDelta.x = ptInit.x - ptTemp.x;
				ptDelta.y = ptInit.y - ptTemp.y;

				ptDelta.x -= ptDelta.x%m_nGridSize;
				ptDelta.y -= ptDelta.y%m_nGridSize;

				// Volvemos a pintar
				pTemp = m_pSelectedLines;
				while(pTemp)
				{
					POINT ptA,ptB;
					ptA.x = pTemp->m_pA.X - ptDelta.x;
					ptA.y = pTemp->m_pA.Y - ptDelta.y;
					ptB.x = pTemp->m_pB.X - ptDelta.x;
					ptB.y = pTemp->m_pB.Y - ptDelta.y;
					pTemp->DrawTempLine(ptA, ptB);
					pTemp = pTemp->m_pSelectedNext;
				}
				
				break;

			case WM_KEYDOWN:
				if (msg.wParam	== 27)
				{
					// Borramos lo último pintado
					pTemp = m_pSelectedLines;
					while(pTemp)
					{
						POINT ptA,ptB;
						ptA.x = pTemp->m_pA.X - ptDelta.x;
						ptA.y = pTemp->m_pA.Y - ptDelta.y;
						ptB.x = pTemp->m_pB.X - ptDelta.x;
						ptB.y = pTemp->m_pB.Y - ptDelta.y;
						pTemp->DrawTempLine(ptA, ptB);
						pTemp = pTemp->m_pSelectedNext;
					}
					m_bCanceled = TRUE;
					bRepeat = FALSE;
				}
				break;
			case WM_RBUTTONDOWN:
				// Se acabó
				// Borramos lo último pintado
				m_bCanceled = TRUE;
				pTemp = m_pSelectedLines;
				while(pTemp)
				{
					POINT ptA,ptB;
					ptA.x = pTemp->m_pA.X - ptDelta.x;
					ptA.y = pTemp->m_pA.Y - ptDelta.y;
					ptB.x = pTemp->m_pB.X - ptDelta.x;
					ptB.y = pTemp->m_pB.Y - ptDelta.y;
					pTemp->DrawTempLine(ptA, ptB);
					pTemp = pTemp->m_pSelectedNext;
				}

				bRepeat = FALSE;
				break;

			default:
				DispatchMessage(&msg);
				break;
		}

	}

	ReleaseCapture();
}

//##ModelId=396AF99B01FB
STDMETHODIMP CLineMan::DrawGrid(long hWnd, long nGridSize)
{

	// Obtenemos el dc de la ventana
	HDC dc = GetDC((HWND)hWnd);
	
	// Declaramos variables a usar
	int nImageWidth, nImageHeight;
	int x,y,i,j;
	int cCont;
	COLORREF PointColor;
	RECT rc;
	
	// Creamos el Pen y el Brush
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
	HBRUSH brush = CreateSolidBrush(RGB(255,255,255));
	

	// Calculamos tamaño óptimo del grid
	nImageWidth = (250 / nGridSize) * nGridSize;
	nImageHeight = 1;
	cCont = 0;
		
	// Creación y selección del bitmap
	HDC dc1 = CreateCompatibleDC(dc);
	HBITMAP bmp = CreateCompatibleBitmap(dc, nImageWidth,nImageHeight);
	HBITMAP OldBmp = (HBITMAP) SelectObject(dc1, bmp);
	
	// Selección de los pen y brush
	HBRUSH OldBrush = (HBRUSH) SelectObject(dc1, brush);
	HPEN OldPen = (HPEN) SelectObject(dc1, pen);
	
	// dibujamos el fondo con Rectangle
	Rectangle(dc1, 0, 0, nImageWidth,nImageHeight);

	// Dibujamos los puntos en el lugar correspondiente
	PointColor = RGB(0,0,0); //TranslateColor(GetBackColor());
	for (i = 0; i < nImageWidth; i += nGridSize)
		SetPixel(dc1, i,0, PointColor);
	GetClipBox(dc, &rc);
	
	// Cálculos de inserción del bitmap
	int nStartCol = rc.left / nImageWidth;
	int nEndCol = (rc.right + nImageWidth + 1) / nImageWidth;
	int nStartRow = rc.top / nGridSize;
	int nEndRow = (rc.bottom + nGridSize + 1) / nGridSize;

	// Inserción del bitmap
	for (i = nStartCol; i < nEndCol; i++)
	{
		for (j = nStartRow; j < nEndRow; j++)
		{
			x = i * nImageWidth;
			y = j * nGridSize;
			BitBlt(dc, x , y, nImageWidth, nImageHeight, dc1, 0,0,SRCAND);
			cCont++;
		}
	}

	
	// Selección de los antiguos brush (si no lo hacemos perdemos GDI)
	SelectObject(dc1, OldBrush);
	SelectObject(dc1, OldPen);
	SelectObject(dc1, OldBmp);
	
	// Eliminamos objetos creados (GDI)
	DeleteObject(OldBrush);
	DeleteObject(OldPen);
	DeleteObject(OldBmp);
	DeleteObject(bmp);  
	DeleteDC(dc1);
  	DeleteObject(brush);
	DeleteObject(pen);

	// El Release de turno y adiós
	ReleaseDC((HWND)hWnd, dc);

	return S_OK;
	
}

//##ModelId=396AF99B0219
void CLineMan::AddHook(HWND hWnd)
{
	HOOK_DATA * pTemp = m_pHookData;
	
	// Primero buscamos si ya existe el Hook

	while (pTemp)
	{
		if (pTemp->hWnd == hWnd)
		{
			pTemp->nRefCount += 1;
			return;
		}
		pTemp = pTemp->pNext;
	}

	
	// Tenemos que insertar un nuevo Hook
	
	if (m_pHookData)
	{
		pTemp = m_pHookData;
		while(pTemp->pNext)
			pTemp = pTemp->pNext;

		// Si nos encontramos aquí, es un Hook nuevo
		pTemp->pNext = new HOOK_DATA;
		pTemp = pTemp->pNext;
		memset(pTemp, 0, sizeof(HOOK_DATA));
		pTemp->hWnd = hWnd;
		pTemp->nRefCount = 1;
		pTemp->WndProc = SetWindowLongPtr(hWnd, GWLP_WNDPROC,  (LONG_PTR)WindowProc);
		pTemp->pNext = NULL;
	}
	else
	{
		m_pHookData = new HOOK_DATA;
		memset(m_pHookData, 0, sizeof(HOOK_DATA));
		m_pHookData->hWnd = hWnd;
		m_pHookData->WndProc = SetWindowLongPtr(hWnd, GWLP_WNDPROC,  (LONG_PTR)WindowProc);
		m_pHookData->nRefCount = 1;
		m_pHookData->pNext = NULL;
	}
}

//##ModelId=396AF99B0223
void CLineMan::ReleaseHook(HWND hWnd)
{
	HOOK_DATA * pTemp = NULL;
	
	if (m_pHookData == NULL) return;
	
	// Caso especial, el primer elemento
	if (m_pHookData->hWnd == hWnd)
	{
		if (--m_pHookData->nRefCount == 0)
		{
			pTemp = m_pHookData;
			SetWindowLongPtr(m_pHookData->hWnd, GWLP_WNDPROC, m_pHookData->WndProc);
			m_pHookData = m_pHookData->pNext;
			delete(pTemp);
		}
		return;
	}
	
	// Estamos aquí porque no es el primer elemento
	pTemp = m_pHookData;
	while(pTemp)
	{
		if (pTemp->pNext)
		{
			if (pTemp->pNext->hWnd == hWnd)
			{
				HOOK_DATA * pHD = pTemp->pNext;
				if (--pHD->nRefCount == 0)
				{
					SetWindowLongPtr(pHD->hWnd, GWLP_WNDPROC, pHD->WndProc);
					pTemp->pNext = pTemp->pNext->pNext;
					delete(pHD);
				}
				return;
			}
		}
		pTemp = pTemp->pNext;
	}
}

//##ModelId=396AF99B022D
WNDPROC CLineMan::GetWndProcByHWND(HWND hWnd)
{
	HOOK_DATA * pTemp = m_pHookData;

	while(pTemp)
	{
		if (pTemp->hWnd == hWnd)
		{
			return (WNDPROC) pTemp->WndProc;
		}
		pTemp = pTemp->pNext;
	}
	return NULL;
}

//##ModelId=396AF99B0241
STDMETHODIMP CLineMan::RemoveLine(BSTR strKey)
{
	USES_CONVERSION;
	
	// La buscamos en la lista y la eliminamos
	CLine * pTemp = NULL;
	char * cKey = OLE2A(strKey);
	//RECT rc;
	
	if (m_pData == NULL) return E_FAIL;
	
	// Caso especial, el primer elemento
	if (strcmp(m_pData->m_strKey, cKey) == 0)
	{
		m_pData->InvalidateLine();
		//m_pData->AbsRect(&rc);
		//InvalidateRect(m_pData->m_hWndPaint, &rc, TRUE);
		RemoveFromSelection(m_pData);
		pTemp = m_pData;
		ReleaseHook(m_pData->m_hWndHook);
		ReleaseHook(m_pData->m_hWndPaint);
		m_pData = m_pData->m_pNext;
		delete(pTemp);
		return S_OK;
	}
	
	// Estamos aquí porque no es el primer elemento
	pTemp = m_pData;
	while(pTemp)
	{
		if (pTemp->m_pNext)
		{
			if (strcmp(pTemp->m_pNext->m_strKey, cKey) == 0)
			{
				CLine * pDD = pTemp->m_pNext;
				pTemp->m_pNext->InvalidateLine();
				//pDD->AbsRect(&rc);
				//InvalidateRect(pDD->m_hWndPaint, &rc, TRUE);
				RemoveFromSelection(pDD);
				ReleaseHook(pDD->m_hWndHook);
				ReleaseHook(m_pData->m_hWndPaint);
				pTemp->m_pNext = pTemp->m_pNext->m_pNext;
				delete(pDD);
				return S_OK;
			}
		}
		pTemp = pTemp->m_pNext;
	}

	return S_OK;
}

//##ModelId=396AF99B024B
STDMETHODIMP CLineMan::Line(VARIANT vKey, IDesignLine **pRet)
{
	USES_CONVERSION;

	*pRet = NULL;
	long nIndex = -1;
	CLine * pData = NULL;
	
	if (vKey.vt == VT_BSTR)
	{
		char * cKey = OLE2A(vKey.bstrVal);
		pData = m_pData;
		while(pData)
		{
			if (strcmp(pData->m_strKey, cKey) == 0)
			{
				CComObject<CDLine> * pDLine;
				CComObject<CDLine>::CreateInstance(&pDLine);
				pDLine->m_oLine = pData;
				//pDLine->m_bstrKey = A2BSTR(pData->m_strKey);
				//pDLine->m_bstrContainer = A2BSTR(pData->m_strContainer);
				pDLine->QueryInterface(IID_IDesignLine, (void**)pRet);
				return S_OK;
			}
			pData = pData->m_pNext;
		}
		return E_INVALIDARG;
	}
	else if (vKey.vt = VT_I2)
	{
		nIndex = vKey.iVal;
	}
	else if (vKey.vt == VT_I4)
	{
		nIndex = vKey.lVal;
	}

	if (nIndex >= 0)
	{
		pData = m_pData;
		while(nIndex >= 0 && pData)
		{
			pData = pData->m_pNext;
			nIndex--;
		}
		if (pData)
		{
			CComObject<CDLine> * pDLine;
			CComObject<CDLine>::CreateInstance(&pDLine);
			pDLine->m_oLine = pData;
			//pDLine->m_bstrKey = A2BSTR(pData->m_strKey);
			//pDLine->m_bstrContainer = A2BSTR(pData->m_strContainer);
			pDLine->QueryInterface(IID_IDesignLine, (void**)pRet);
			return S_OK;
		}
	}

	return E_INVALIDARG;
}


//##ModelId=396AF99B02A5
void CLineMan::DrawTemporalLine(HWND hWnd, POINT ptA, POINT ptB)
{
	HDC dc = GetDC(hWnd);
	
	int OldRop2 = SetROP2(dc, R2_NOTXORPEN);

	HPEN pen;
	pen = CreatePen(PS_SOLID, 1, RGB(192,192,192));
	HPEN oldPen = (HPEN) SelectObject(dc, pen);
	
	MoveToEx(dc, ptA.x, ptA.y, NULL);
	LineTo(dc, ptB.x, ptB.y);
	SelectObject(dc, oldPen);
	SetROP2(dc, OldRop2);
	
	DeleteObject(pen);
	ReleaseDC(hWnd, dc);
}

//##ModelId=396AF99B02C3
STDMETHODIMP CLineMan::DrawTemporalLine(long hWnd, long Left, long Top, long Right, long Bottom)
{
	HDC dc = GetDC((HWND) hWnd);
	POINT pt;
	
	int OldRop2 = SetROP2(dc, R2_NOTXORPEN);

	HPEN pen;
	pen = CreatePen(PS_SOLID, 1, RGB(192,192,192));
	HPEN oldPen = (HPEN) SelectObject(dc, pen);

	pt.x = Left;
	pt.y = Top;

	ScreenToClient((HWND) hWnd, &pt);

	MoveToEx(dc, pt.x, pt.y, NULL);

	pt.x = Right;
	pt.y = Bottom;

	ScreenToClient((HWND) hWnd, &pt);

	
	LineTo(dc, pt.x, pt.y);
	SelectObject(dc, oldPen);
	SetROP2(dc, OldRop2);
	
	DeleteObject(pen);
	ReleaseDC((HWND) hWnd, dc);
	
	return S_OK;
}

//##ModelId=3968FD57024D
/*CLineMan::Lines()
{
	// TODO: Add your implementation code here
	
	return S_OK;
}*/



//##ModelId=396AF99B025F
STDMETHODIMP CLineMan::Lines(ILineCol **pVal)
{
	CComObject<CLineCol> *pLineCol = NULL;
	CComObject<CLineCol>::CreateInstance(&pLineCol);

	pLineCol->QueryInterface(pVal);

	CLine * pLine = m_pData;

	while (pLine)
	{
		AddLineToCol(*pVal, pLine);
		pLine = pLine->m_pNext;
	}

	return S_OK;
}

//##ModelId=396AF99B027D
STDMETHODIMP CLineMan::SelectedLines(ILineCol **pVal)
{
	CComObject<CLineCol> *pLineCol = NULL;
	CComObject<CLineCol>::CreateInstance(&pLineCol);

	pLineCol->QueryInterface(pVal);

	CLine * pLine = m_pSelectedLines;

	while (pLine)
	{
		AddLineToCol(*pVal, pLine);
		pLine = pLine->m_pSelectedNext;
	}

	return S_OK;
}

//##ModelId=396AF99B02F5
void CLineMan::AddLineToCol(ILineCol *pCol, CLine *pLine)
{
	CComObject<CDLine> *pDLine = NULL;
	CComObject<CDLine>::CreateInstance(&pDLine);

	IDesignLine * pDesignLine = NULL;

	pDLine->QueryInterface(&pDesignLine);

	CDLine * pStatic = static_cast<CDLine*>(pDLine);
	
	pStatic->m_oLine = pLine;
	pCol->Add(pDesignLine, CComVariant(pLine->m_strKey));
	pDesignLine->Release();
}

//##ModelId=396AF99B0309
void CLineMan::LaunchLineSelected(CLine *pLine)
{
	if (m_pEvents)
	{
		IDesignLine *pDesignLine = NULL;
		CComObject<CDLine> * pDLine;
		CComObject<CDLine>::CreateInstance(&pDLine);
		pDLine->m_oLine = pLine;
		pDLine->QueryInterface(IID_IDesignLine, (void**)&pDesignLine);
		m_pEvents->LineSelected(pDesignLine);
		pDesignLine->Release();
	}
}

//##ModelId=396AF99B0291
STDMETHODIMP CLineMan::Construct(ILineManEvents *pLineManEvents)
{
	if (m_pEvents)
		m_pEvents->Release();

	m_pEvents = NULL;
	if (pLineManEvents)
	{
		m_pEvents = pLineManEvents;
		m_pEvents->AddRef();
	}

	return S_OK;
}

//##ModelId=396AF99B029B
STDMETHODIMP CLineMan::Destruct()
{
	if (m_pEvents)
		m_pEvents->Release();

	m_pEvents = NULL;

	return S_OK;
}

//##ModelId=396AF99B0313
void CLineMan::LaunchLineMoved(CLine *pLine)
{
	if (m_pEvents)
	{
		IDesignLine *pDesignLine = NULL;
		CComObject<CDLine> * pDLine;
		CComObject<CDLine>::CreateInstance(&pDLine);
		pDLine->m_oLine = pLine;
		pDLine->QueryInterface(IID_IDesignLine, (void**)&pDesignLine);
		m_pEvents->LineMoved(pDesignLine);
		pDesignLine->Release();
	}
}

//##ModelId=396AF99B0327
void CLineMan::LaunchRButtonUp(CLine *pLine)
{
	if (m_pEvents)
	{
		IDesignLine *pDesignLine = NULL;
		CComObject<CDLine> * pDLine;
		CComObject<CDLine>::CreateInstance(&pDLine);
		pDLine->m_oLine = pLine;
		pDLine->QueryInterface(IID_IDesignLine, (void**)&pDesignLine);
		m_pEvents->LineRButton(pDesignLine);
		pDesignLine->Release();
	}
}

STDMETHODIMP CLineMan::UnselectAll()
{
	CLine * pTemp = m_pSelectedLines;
	CLine * pTemp2 = NULL;

	while(pTemp)
	{
		pTemp->m_eSelected = M4DESIGN_NOTSELECTED;
		pTemp->InvalidateHandles();
		//pTemp->m_pSelectedNext = NULL;
		pTemp2 = pTemp;
		pTemp = pTemp->m_pSelectedNext;
		pTemp2->m_pSelectedNext = NULL;
	}

	m_pSelectedLines = NULL;
	
	return S_OK;
}

STDMETHODIMP CLineMan::get_DragState(VARIANT_BOOL *pVal)
{
	if (pVal)
	{
		*pVal = m_bDragState;
		return S_OK;
	}
	return E_POINTER;
}

STDMETHODIMP CLineMan::put_DragState(VARIANT_BOOL newVal)
{
	m_bDragState = newVal;

	return S_OK;
}
