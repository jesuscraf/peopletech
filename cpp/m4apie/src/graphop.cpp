// GraphOp.cpp : Implementation of CGraphOp
#include "stdafx.h"
#include "m4apie.h"
#include "graphop.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphOp



//##ModelId=396B32960048
STDMETHODIMP CGraphOp::DrawGrid(long hWnd, long nGridSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

// Obtenemos el dc de la ventana
	HDC dc = GetDC((HWND)hWnd);
	
	// Declaramos variables a usar
	int m_nImageWidth, m_nImageHeight;
	int x,y,i,j;
	int cCont;
	COLORREF PointColor;
	RECT rc;
	
	// Creamos el Pen y el Brush
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
	HBRUSH brush = CreateSolidBrush(RGB(255,255,255));
	

	// Calculamos tamaño óptimo del grid
	m_nImageWidth = (MAXIMO / nGridSize) * nGridSize;	
	m_nImageHeight = 1;
	cCont = 0;
		
	// Creación y selección del bitmap
	HDC dc1 = CreateCompatibleDC(dc);
	HBITMAP bmp = CreateCompatibleBitmap(dc, m_nImageWidth,m_nImageHeight);
	HBITMAP OldBmp = (HBITMAP) SelectObject(dc1, bmp);
	
	// Selección de los pen y brush
	HBRUSH OldBrush = (HBRUSH) SelectObject(dc1, brush);
	HPEN OldPen = (HPEN) SelectObject(dc1, pen);
	
	// dibujamos el fondo con Rectangle
	Rectangle(dc1, 0, 0, m_nImageWidth,m_nImageHeight);

	// Dibujamos los puntos en el lugar correspondiente
	PointColor = RGB(0,0,0); //TranslateColor(GetBackColor());
	for (i = 0; i < m_nImageWidth; i += nGridSize)
	{
		//PointColor = RGB(rand()%255,rand()%255,rand()%255);
		SetPixel(dc1, i,0, PointColor);
	}
	
	
	// Intentamos (por si acaso) obtener el área de recorte
	//GetClientRect((HWND)hWnd, &rc);

	GetClipBox(dc, &rc);

	
	// Cálculos de inserción del bitmap
	int nStartCol = rc.left / m_nImageWidth;
	int nEndCol = (rc.right + m_nImageWidth + 1) / m_nImageWidth;
	int nStartRow = rc.top / nGridSize;
	int nEndRow = (rc.bottom + nGridSize + 1) / nGridSize;

	// Inserción del bitmap
	for (i = nStartCol; i < nEndCol; i++)
	{
		for (j = nStartRow; j < nEndRow; j++)
		{
			x = i * m_nImageWidth;
			y = j * nGridSize;
			BitBlt(dc, x , y, m_nImageWidth, m_nImageHeight, dc1, 0,0,SRCAND);
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

//##ModelId=396B32960052
STDMETHODIMP CGraphOp::CreateEMFFromWnd(long hWnd, BSTR strFileName, BOOL bClient)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	RECT rc;
	HDC  hdcEmf, hdcWnd;

	if (bClient)
	{
		GetClientRect((HWND) hWnd, &rc);
	}
	else
	{
		GetWindowRect((HWND) hWnd, &rc);
	}

	rc.bottom -= rc.top;
	rc.right -= rc.left;
	rc.top = rc.left = 0;

	if (bClient)
	{
		hdcWnd = GetDC((HWND) hWnd);
	}
	else
	{
		hdcWnd = GetWindowDC((HWND) hWnd);
	}
	
	char * Nombre = OLE2A(strFileName);
		
	hdcEmf = CreateEnhMetaFile(NULL, Nombre, NULL, "Created By M4APIE\0BitBlt\0\0");

	BitBlt(hdcEmf, 0,0, rc.right, rc.bottom, hdcWnd, 0,0, SRCCOPY);

	HENHMETAFILE hemf = CloseEnhMetaFile(hdcEmf);
	DeleteEnhMetaFile(hemf);

	ReleaseDC((HWND) hWnd, hdcWnd);

	return S_OK;
}

//##ModelId=396B32960065
STDMETHODIMP CGraphOp::DrawGrayBox(long x1, long y1, long x2, long y2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	HDC dc = GetDC(NULL);

	//HBRUSH brush = CreateHatchBrush(RGB(255,255,255));
	
	
	

	RECT rc;
	
	rc.top = y1;
	rc.bottom = y2;
	for (int i = x1; i < x2; i+=2)
	{
		rc.left = i;
		rc.right = (i+2);
		DrawFocusRect(dc, &rc);
	}

	/*HBRUSH brush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	

	HBRUSH oldBrush = (HBRUSH)SelectObject(dc, brush);


	UINT oldROP2 = SetROP2(dc, R2_NOT);
	Rectangle(dc, x1, y1, x2, y2);
	SetROP2(dc, oldROP2);

	SelectObject(dc, oldBrush);
	DeleteObject(brush);*/

	ReleaseDC(NULL, dc);
	return S_OK;
}

//##ModelId=396B32960071
STDMETHODIMP CGraphOp::DrawDragBox(long hWnd, long Left, long Top, long Right, long Bottom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	RECT rc;
	
	// Cogemos DC del escritorio para dibujar por encima de todos, pero limitamos región
	// a la parte visible de la ventana
	HDC dc = GetDC(NULL);

	RECT wrc;
	GetWindowRect((HWND) hWnd, &wrc);
	HRGN rgn = CreateRectRgnIndirect(&wrc);
	SelectClipRgn(dc, rgn);

	RECT crc;
	GetClientRect((HWND) hWnd, &crc);
	rc.top = Top + (wrc.top + crc.top);
	rc.left = Left + (wrc.left + crc.left);
	rc.right = Right + (wrc.left + crc.left);
	rc.bottom = Bottom + (wrc.top + crc.top);
	
	//HPEN hPen;

	HBRUSH brush = (HBRUSH) GetStockObject(NULL_BRUSH);
	HPEN pen = CreatePen(PS_SOLID , 2, 0);	
	HBRUSH oldBrush = (HBRUSH) SelectObject(dc, brush);
	HPEN oldPen= (HPEN) SelectObject(dc, pen);
	int oldROP2 = SetROP2(dc, R2_NOT);
	
	Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
	
	SetROP2(dc, oldROP2);
	SelectObject(dc, oldBrush);
	DeleteObject(brush);
	SelectObject(dc, oldPen);
	DeleteObject(pen);
	ReleaseDC(NULL, dc);
		

	/*Dim hPen As Long, hOldPen As Long
    Dim hBrush As Long, hOldBrush As Long
    Dim hScreenDC As Long, nDrawMode As Long
    'Dim thisRect As RECT
    Dim pen As LOGPEN
    Dim brush As LOGBRUSH
    Dim brushpt As POINTAPI
    
        
        


    'Get DC of entire screen in order to
    
    'draw on top of all controls
    hScreenDC = GetDC(0)
    If () Then
    Else
        Stop
    End If
        
                
    'Select GDI object
    brush.lbColor = &H0
    brush.lbHatch = 0
    brush.lbStyle = BS_NULL
        
    'hBrush = GetStockObject(NULL_BRUSH)
    hBrush = CreateBrushIndirect(brush)
    hOldBrush = SelectObject(hScreenDC, hBrush)
        
    brushpt.x = 2
    brushpt.y = 2
    
    
    pen.lopnColor = &HFFFFFF
    pen.lopnStyle = 1
    pen.lopnWidth = brushpt
        
    'hPen = CreatePen(PS_DASH, 2, 0)
    hPen = CreatePenIndirect(pen)
    
    hOldPen = SelectObject(hScreenDC, hPen)
    nDrawMode = SetROP2(hScreenDC, R2_NOT)
         
    'Draw rectangle       Old way
    Rectangle hScreenDC, m_DragRect.Left, m_DragRect.Top, _
        m_DragRect.Right, m_DragRect.Bottom
    
    
    ' Draw a focused rectangle
'    thisRect.Top = m_DragRect.Top
'    thisRect.Left = m_DragRect.Left
'    thisRect.Bottom = m_DragRect.Bottom
'    thisRect.Right = m_DragRect.Right
'
'    Call DrawFocusRect(hScreenDC, thisRect)
    
    
    'Restore DC
    SetROP2 hScreenDC, nDrawMode
    SelectObject hScreenDC, hOldBrush
    SelectObject hScreenDC, hOldPen
    ReleaseDC 0, hScreenDC
    
    'Delete GDI objects
    DeleteObject hPen*/





	return S_OK;
}

//##ModelId=396B329600D3
STDMETHODIMP CGraphOp::DrawBorder(long hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	RECT rc;

	HDC myDC = GetDC((HWND) hWnd);
	GetClientRect((HWND) hWnd, &rc);
	Rectangle(myDC, rc.left, rc.top, rc.right, rc.bottom);
	ReleaseDC((HWND) hWnd, myDC);

	return S_OK;
}

//##ModelId=396B32960097
STDMETHODIMP CGraphOp::DrawRectWithClipWnd(long hWnd, long Left, long Top, long Right, long Bottom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HWND hWndClip = (HWND) hWnd;

	RECT	rc,clipRect;
	POINT	p;

	HDC screenDC = GetDC(NULL);
	
	p.y = Top;
	p.x = Left;
	ClientToScreen(hWndClip, &p);
	rc.top = p.y;
	rc.left = p.x;

	p.x = Right;
	p.y = Bottom;
	ClientToScreen(hWndClip, &p);
	rc.right = p.x;
	rc.bottom = p.y;

	
	GetClientRect((HWND) hWnd, &clipRect);
	
	p.y = clipRect.top;
	p.x = clipRect.left;
	ClientToScreen(hWndClip, &p);
	clipRect.top = p.y;
	clipRect.left = p.x;

	p.x = clipRect.right;
	p.y = clipRect.bottom;
	ClientToScreen(hWndClip, &p);
	clipRect.right = p.x;
	clipRect.bottom = p.y;
	
	HRGN rgn = CreateRectRgnIndirect(&clipRect);
	SelectClipRgn(screenDC, rgn);
	
	DrawFocusRect(screenDC, &rc);
	
	ReleaseDC(NULL, screenDC);


	return S_OK;
}

//##ModelId=396B329600AB
STDMETHODIMP CGraphOp::DrawItem(long hDC, long Left, long Top, long Right, long Bottom, long Style)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HBRUSH	hBrush;
	HPEN	hPen;
	HDC ohDC = (HDC) hDC;
	HPEN   oldPen;
	HBRUSH oldBrush;
	RECT rc;

	// Allá vamos de nuevo
	
	switch (Style)
	{
		case 0:
			hBrush = GetSysColorBrush(COLOR_3DFACE);
			oldBrush = (HBRUSH) SelectObject(ohDC, hBrush);
			Rectangle(ohDC, Left, Top, Right, Bottom);
			rc.top = Top;
			rc.left = Left;
			rc.right = Right;
			rc.bottom = Bottom;
			DrawBorder(ohDC, rc);
			DrawText(ohDC, "...", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			SelectObject(ohDC, oldBrush);
			DeleteObject(hBrush);
			
			break;
		case 1:
			hBrush = (HBRUSH) GetStockObject(NULL_BRUSH);
			hPen = CreatePen(PS_NULL, 1, GetSysColor(COLOR_3DFACE));
			oldPen = (HPEN) SelectObject(ohDC, hPen);
			oldBrush = (HBRUSH) SelectObject(ohDC, hBrush);
			Rectangle(ohDC, Left, Top, Right, Bottom);
			SelectObject(ohDC, oldPen);
			DeleteObject(hPen);
			SelectObject(ohDC, oldBrush);
			break;
	}

	return S_OK;
}

//##ModelId=396B329600C1
void CGraphOp::DrawBorder(HDC hDC, RECT rc)
{

	HPEN	pen;
	HPEN	pen2;

	pen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	pen2 = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));

	HPEN pOldPen = (HPEN) SelectObject(hDC, pen);
	MoveToEx(hDC, rc.right, rc.top, NULL);
	LineTo(hDC, rc.left, rc.top);
	LineTo(hDC, rc.left, rc.bottom-1);
	SelectObject(hDC, pen2);
	LineTo(hDC, rc.right-1, rc.bottom-1);
	LineTo(hDC, rc.right-1, rc.top-1);
	SelectObject(hDC, pOldPen);


}

//##ModelId=396B329600D6
STDMETHODIMP CGraphOp::TransformHimetric(IPictureDisp * pDisp, long * Width, long * Height)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	IPicture *pPict = NULL;
	HRESULT hr = E_INVALIDARG;
	if (pDisp != NULL)
	{
		hr = pDisp->QueryInterface(IID_IPicture, (void**)&pPict);
		if (hr == S_OK)
		{
			OLE_YSIZE_HIMETRIC oleHeight;
			OLE_XSIZE_HIMETRIC oleWidth;
			
			hr = pPict->get_Width(&oleWidth);
			hr = pPict->get_Height(&oleHeight);

			CDC dc;
			dc.CreateDC("DISPLAY", NULL, NULL, NULL);
			SIZE size;
			size.cx = oleWidth;
			size.cy = oleHeight;
			dc.HIMETRICtoLP(&size);

			*Width = size.cx;
			*Height = size.cy;

			dc.DeleteDC();

			pPict->Release();
			pPict = NULL;
		}
	}

	return hr;
}

//##ModelId=396B329600E7
STDMETHODIMP CGraphOp::GetDisabledPicture(IPictureDisp *pPictureIn, OLE_COLOR * nColor, IPictureDisp **ppPictureRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Comprobamos el origen
	if ((pPictureIn == NULL) || (ppPictureRet == NULL)) return E_INVALIDARG;

	if (*ppPictureRet)
	{
		(*ppPictureRet)->Release();
		*ppPictureRet = NULL;
	}

	m_nBackColor = *nColor;
	IPicture * pPicOrigen = NULL;
	HRESULT hr = pPictureIn->QueryInterface(IID_IPicture, (void**)&pPicOrigen);
	if (hr == S_OK)
	{

		PICTDESC picDesc;
		// Primero miramos si es Bitmap ó Icono
		short nType;
		pPicOrigen->get_Type(&nType);
		
		if (nType == PICTYPE_BITMAP)
		{
			
			HBITMAP hBmpOrigen;
			HBITMAP hBmpDestino;

			HPALETTE hPal;
			
			HDC hDCDestino;
			
			pPicOrigen->get_Handle((OLE_HANDLE*)&hBmpOrigen);
			pPicOrigen->get_hPal((OLE_HANDLE*)&hPal);
			
			BITMAP bitmap;
			::GetObject(hBmpOrigen, sizeof(BITMAP), &bitmap);
			
			hDCDestino = CreateCompatibleDC(NULL);
			long bpps = GetDeviceCaps(hDCDestino, BITSPIXEL);
			hBmpDestino = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, bpps, NULL);
			
			HBITMAP * hOldDestino = (HBITMAP*)::SelectObject(hDCDestino, hBmpDestino);
			
			DitherBlt(hDCDestino, 0,0, bitmap.bmWidth, bitmap.bmHeight, hBmpOrigen, 0,0);

			::SelectObject(hDCDestino, hOldDestino);
			::DeleteDC(hDCDestino);

			picDesc.cbSizeofstruct = sizeof(PICTDESC);
			picDesc.picType = PICTYPE_BITMAP;
			picDesc.bmp.hbitmap = hBmpDestino;
			picDesc.bmp.hpal = NULL;
			OleCreatePictureIndirect(&picDesc, IID_IPictureDisp, true,(void**)ppPictureRet);
		}
		else if (nType == PICTYPE_ICON)
		{
			HICON hIcon;

			pPicOrigen->get_Handle((OLE_HANDLE*)&hIcon);
			
			ICONINFO iconInfo;
			GetIconInfo(hIcon, &iconInfo);

			BITMAP bitmap;
			::GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bitmap);
			HDC hDCDestino = CreateCompatibleDC(NULL);
			long bpps = GetDeviceCaps(hDCDestino, BITSPIXEL);
			HBITMAP hBMPDestino = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, bpps, NULL);

			HBITMAP * hOldDestino = (HBITMAP*)::SelectObject(hDCDestino, hBMPDestino);
			DitherBlt(hDCDestino, 0,0, bitmap.bmWidth, bitmap.bmHeight, iconInfo.hbmMask, 0,0);
			::SelectObject(hDCDestino, hOldDestino);
			::DeleteDC(hDCDestino);
			picDesc.cbSizeofstruct = sizeof(PICTDESC);
			picDesc.picType = PICTYPE_BITMAP;
			picDesc.bmp.hbitmap = hBMPDestino;
			picDesc.bmp.hpal = NULL;
			OleCreatePictureIndirect(&picDesc, IID_IPictureDisp, true,(void**)ppPictureRet);
		}
		pPicOrigen->Release();
	}

	return S_OK;
}


//##ModelId=396B32960029
void CGraphOp::DitherBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, 
				int nHeight, HBITMAP hbm, int nXSrc, int nYSrc)
{
	ASSERT(hdcDest && hbm);
	ASSERT(nWidth > 0 && nHeight > 0);
	
	// Create a generic DC for all BitBlts
	HDC hDC = CreateCompatibleDC(hdcDest);
	ASSERT(hDC);
	
	if (hDC)
	{
		// Create a DC for the monochrome DIB section
		HDC bwDC = CreateCompatibleDC(hDC);
		ASSERT(bwDC);
		
		if (bwDC)
		{
			// Create the monochrome DIB section with a black and white palette
			struct {
				BITMAPINFOHEADER bmiHeader; 
				RGBQUAD 		 bmiColors[2]; 
			} RGBBWBITMAPINFO = {
				
				{		// a BITMAPINFOHEADER
					sizeof(BITMAPINFOHEADER),	// biSize 
						nWidth, 				// biWidth; 
						nHeight,				// biHeight; 
						1,						// biPlanes; 
						1,						// biBitCount 
						BI_RGB, 				// biCompression; 
						0,						// biSizeImage; 
						0,						// biXPelsPerMeter; 
						0,						// biYPelsPerMeter; 
						0,						// biClrUsed; 
						0						// biClrImportant; 
				},
				
				{
					{ 0x00, 0x00, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 }
					} 
			};
			VOID *pbitsBW;
			HBITMAP hbmBW = CreateDIBSection(bwDC,(LPBITMAPINFO)&RGBBWBITMAPINFO, DIB_RGB_COLORS, &pbitsBW, NULL, 0);
			ASSERT(hbmBW);
			
			if (hbmBW)
			{
				// Attach the monochrome DIB section and the bitmap to the DCs
				SelectObject(bwDC, hbmBW);
				SelectObject(hDC, hbm);
				
				// BitBlt the bitmap into the monochrome DIB section
				BitBlt(bwDC, 0, 0, nWidth, nHeight, hDC, nXSrc, nYSrc, SRCCOPY);
				
				COLORREF color;
				if (OleTranslateColor(m_nBackColor, NULL, &color) != S_OK) color = ::GetSysColor(COLOR_3DFACE);

				HBRUSH br = CreateSolidBrush(color);
				FillRect(hdcDest, CRect(nXDest, nYDest, nXDest + nWidth, nYDest +
					nHeight), br);
				DeleteObject(br);
				
				// BitBlt the black bits in the monochrome bitmap into COLOR_3DHILIGHT bits in the destination DC
				// The magic ROP comes from the Charles Petzold's book
				HBRUSH hb = CreateSolidBrush(GetSysColor(COLOR_3DHILIGHT));
				HBRUSH oldBrush = (HBRUSH)SelectObject(hdcDest, hb);
				BitBlt(hdcDest, nXDest + 1, nYDest + 1, nWidth, nHeight, bwDC, 0, 0, 0xB8074A);
				
				// BitBlt the black bits in the monochrome bitmap into COLOR_3DSHADOW bits in the destination DC
				hb = CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));
				DeleteObject(SelectObject(hdcDest, hb));
				BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, bwDC, 0, 0, 0xB8074A);
				DeleteObject(SelectObject(hdcDest, oldBrush));
				DeleteObject(hbmBW);
			}
			
			VERIFY(DeleteDC(bwDC));
		}
		
		VERIFY(DeleteDC(hDC));
	}
}

//##ModelId=396B329600F3
STDMETHODIMP CGraphOp::GiveMeNewPicture(IPictureDisp **ppPicRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	PICTDESC picDesc;
		
	//HBITMAP hBMP = CreateBitmap(50,50,1,8,NULL);
		
	HDC hDC;
	HBITMAP hBMP;
	hDC = CreateCompatibleDC(NULL);
	long bpps = GetDeviceCaps(hDC, BITSPIXEL);
	hBMP = ::CreateBitmap(250,250, 1, bpps, NULL);

	
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hDC, hBMP);
	for (int i = 250; i > 0; i--)
	{
		HBRUSH br = ::CreateSolidBrush(RGB(rand()%255,rand()%255,rand()%255)); 
		::FillRect(hDC, CRect(0,0,i,i), br);
	}
	::SelectObject(hDC, hOldBmp);

	picDesc.cbSizeofstruct = sizeof(PICTDESC);
	picDesc.picType = PICTYPE_BITMAP;
	picDesc.bmp.hbitmap = hBMP;
	picDesc.bmp.hpal = NULL;
	OleCreatePictureIndirect(&picDesc, IID_IPictureDisp, true,(void**)ppPicRet);

	return S_OK;
}

/*STDMETHODIMP CGraphOp::GiveMeNewPictureIcon(IPictureDisp **ppRetPicture)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	PICTDESC picDesc;
	HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	HBITMAP hBMPDestino;
	
	ICONINFO iconInfo;
	GetIconInfo(hIcon, &iconInfo);

	BITMAP bitmap;
	::GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bitmap);

	HDC hDCDestino = CreateCompatibleDC(NULL);

	long bpps = GetDeviceCaps(hDCDestino, BITSPIXEL);
	hBMPDestino = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, bpps, NULL);
	
	
	
	HBITMAP * hOldDestino = (HBITMAP*)::SelectObject(hDCDestino, hBMPDestino);
	
	DitherBlt(hDCDestino, 0,0, bitmap.bmWidth, bitmap.bmHeight, iconInfo.hbmColor, 0,0);

	::SelectObject(hDCDestino, hOldDestino);
	::DeleteDC(hDCDestino);

	picDesc.cbSizeofstruct = sizeof(PICTDESC);
	picDesc.picType = PICTYPE_BITMAP;
	picDesc.bmp.hbitmap = hBMPDestino;
	picDesc.bmp.hpal = NULL;
	OleCreatePictureIndirect(&picDesc, IID_IPictureDisp, true,(void**)ppRetPicture);

	DestroyIcon(hIcon);

	return S_OK;
}*/

//##ModelId=396B329600FD
STDMETHODIMP CGraphOp::GetPictureFromHandle(long hIcon, IPictureDisp **ppRetVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (hIcon && ppRetVal)
	{
		if (*ppRetVal)
		{
			(*ppRetVal)->Release();
			*ppRetVal = NULL;
		}
			
		HICON newIcon = (HICON) hIcon;

		PICTDESC picDesc;
		picDesc.cbSizeofstruct = sizeof(PICTDESC);
		picDesc.picType = PICTYPE_ICON;
		picDesc.icon.hicon = newIcon;
		OleCreatePictureIndirect(&picDesc, IID_IPictureDisp, true,(void**)ppRetVal);

		return S_OK;
	}
	return E_INVALIDARG;
}

//##ModelId=396B354D0119
STDMETHODIMP CGraphOp::DrawLine(long hWnd, long pAX, long pAY, long pBX, long pBY, long nWidth, OLE_COLOR olecColor, M4LineStyle nStyle)
{
	HPEN pen;
	COLORREF color;
	LOGBRUSH lBrush;
	DWORD nLineStyle = PS_GEOMETRIC | PS_ENDCAP_SQUARE;
	
/*	switch(m_eEndCap)
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
	}*/

	HDC dc = GetDC((HWND)hWnd);

	OleTranslateColor(olecColor, NULL, &color);
	lBrush.lbColor = color;
	lBrush.lbStyle = BS_SOLID;
	lBrush.lbHatch = 0;
	
	switch (nStyle)
	{
		case M4DESIGN_STYLE_SOLID:
			nLineStyle |= PS_SOLID;
			break;
		case M4DESIGN_STYLE_DASH:
			nLineStyle |= PS_DASH;
			break;
		case M4DESIGN_STYLE_DOT:
			nLineStyle |= PS_DOT;
			break;
		case M4DESIGN_STYLE_DASHDOT:
			nLineStyle |= PS_DASHDOT;
			break;
		case M4DESIGN_STYLE_DASHDOTDOT:
			nLineStyle |= PS_DASHDOTDOT;
			break;
		default:
			break;
	}
	
	pen = ExtCreatePen(nLineStyle, nWidth, &lBrush, 0, NULL);
	HPEN oldPen = (HPEN)SelectObject(dc, pen);
	
	MoveToEx(dc, pAX, pAY, NULL);
	
	LineTo(dc, pBX, pBY);
	
	SelectObject(dc, oldPen);
	DeleteObject(pen);

	ReleaseDC((HWND)hWnd, dc);

	return S_OK;
}


