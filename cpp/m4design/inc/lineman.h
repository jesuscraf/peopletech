// LineMan.h : Declaration of the CLineMan

#ifndef __LINEMAN_H_
#define __LINEMAN_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLineMan
//##ModelId=396DF30D01E4
class ATL_NO_VTABLE CLineMan : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLineMan, &CLSID_LineMan>,
	public ISupportErrorInfo,
	//public IConnectionPointContainerImpl<CLineMan>,
	public IDispatchImpl<ILineMan, &IID_ILineMan, &LIBID_M4DESIGNLib>
	//public CProxy_ILineManEvents< CLineMan >
{
public:
	//##ModelId=396DF30E015A
	CLineMan()
	{
	};

	// Desplazamos a los Final

	//##ModelId=397C1D0602CA
	HRESULT FinalConstruct()
	{
		m_bDragState = 0;
		m_pSelectedLines = NULL;
		m_nGridSize = 1;
		m_bCanceled = FALSE;
		m_pData = NULL;
		m_pHookData = NULL;
		m_nLines = 0;
		m_pEvents = NULL;
		return S_OK;
	}

	//##ModelId=397C1D060216
	void FinalRelease()
	{
		// Destruimos y deasignamos WindowProcs
		while(m_pData)
		{
			CLine * pTemp = m_pData;
			m_pData = m_pData->m_pNext;
			delete(pTemp);
		}
		while(m_pHookData)
		{
			HOOK_DATA * pTemp = m_pHookData;
			m_pHookData = m_pHookData->pNext;
			SetWindowLongPtr(pTemp->hWnd, GWLP_WNDPROC, pTemp->WndProc);			
			delete(pTemp);
		}
		if (m_pEvents)
			m_pEvents->Release();
	}
	
	//##ModelId=396DF30E0159
	~CLineMan()
	{
	};


DECLARE_REGISTRY_RESOURCEID(IDR_LINEMAN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLineMan)
	COM_INTERFACE_ENTRY(ILineMan)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	//COM_INTERFACE_ENTRY(IConnectionPointContainer)
	//COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

/*BEGIN_CONNECTION_POINT_MAP(CLineMan)
//	CONNECTION_POINT_ENTRY(DIID__ILineManEvents)
//	CONNECTION_POINT_ENTRY(IID__ILineManEvents)
	CONNECTION_POINT_ENTRY(DIID__ILineManEvents)
END_CONNECTION_POINT_MAP()*/


// ISupportsErrorInfo
	//##ModelId=396DF30E0145
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	//##ModelId=396DF30E0131
	void LaunchRButtonUp(CLine * pLine);
	//##ModelId=396DF30E0127
	void LaunchLineMoved(CLine *pLine);
	//##ModelId=396DF30E011D
	void LaunchLineSelected(CLine * pLine);
	//##ModelId=396DF30E0109
	void AddLineToCol(ILineCol * pCol, CLine * pLine);
	//##ModelId=396DF30E00B1
	void DrawTemporalLine(HWND hWnd, POINT ptA, POINT ptB);

	
// ILineMan
public:
	//##ModelId=397C1D05032D
	STDMETHOD(get_DragState)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	//##ModelId=397C1D0500CA
	STDMETHOD(put_DragState)(/*[in]*/ VARIANT_BOOL newVal);
	//##ModelId=396DF30E00AF
	STDMETHOD(UnselectAll)();
	//##ModelId=396DF30E00A5
	STDMETHOD(Destruct)();
	//##ModelId=396DF30E009B
	STDMETHOD(Construct)(/*[in]*/ ILineManEvents * pLineManEvents);
	//##ModelId=396DF30E0087
	STDMETHOD(SelectedLines)(/*[out, retval]*/ ILineCol ** pVal);
	//##ModelId=396DF30E007D
	STDMETHOD(Lines)(/*[out, retval]*/ ILineCol ** pVal);
	//##ModelId=396901B0006C
	//Lines();

	//##ModelId=396DF30E00CD
	STDMETHOD(DrawTemporalLine)(/*[in]*/ long hWnd, /*[in]*/ long Left, /*[in]*/ long Top, /*[in]*/ long Right, /*[in]*/ long Bottom);
	//##ModelId=396DF30E005F
	STDMETHOD(Line)(/*[in]*/ VARIANT vKey, /*[out, retval]*/ IDesignLine * * pRet);
	//##ModelId=396DF30E0055
	STDMETHOD(RemoveLine)(/*[in]*/ BSTR strKey);
	//##ModelId=396DF30E004B
	WNDPROC GetWndProcByHWND(HWND hWnd);
	//##ModelId=396DF30E0037
	void ReleaseHook(HWND hWnd);
	//##ModelId=396DF30E002D
	void AddHook(HWND hWnd);
	//##ModelId=396DF30E000F
	STDMETHOD(DrawGrid)(/*[in]*/ long hWnd, /*[in]*/ long nGridSize);
	//##ModelId=396DF30D03E3
	void BeginMultiDrag(HWND hWnd, POINT pt);
	//##ModelId=396DF30D03D9
	void RemoveFromSelection(CLine * pData);
	//##ModelId=396DF30D03C5
	void AddToSelection(CLine * pData);
	//##ModelId=396DF30D03BB
	BOOL TestInSelected(CLine * pData);
	//##ModelId=396DF30D039D
	void ButtonUp(HWND hWnd, POINT pt, long keyState);
	//##ModelId=396DF30D0393
	void SelectLine(char * cKey);
	//##ModelId=396DF30D036A
	STDMETHOD(get_Selected)(/*[in]*/ BSTR bKey, /*[out, retval]*/ VARIANT_BOOL *pVal);
	//##ModelId=396DF30D0356
	STDMETHOD(put_Selected)(/*[in]*/ BSTR bKey, /*[in]*/ VARIANT_BOOL newVal);
	//##ModelId=396DF30D02FC
	STDMETHOD(AddLine)(/*[in]*/ long hWndHook, /*[in]*/ long hWndPaint,/*[in]*/ BSTR key,/*[in]*/ BSTR contKey,/*[in]*/ long pAX, /*[in]*/ long pAY, /*[in]*/ long pBX, /*[in]*/ long pBY, IDesignLine ** pLine);
	//##ModelId=396DF30D02F2
	STDMETHOD(get_GridSize)(/*[out, retval]*/ long *pVal);
	//##ModelId=396DF30D02DE
	STDMETHOD(put_GridSize)(/*[in]*/ long newVal);

	//##ModelId=396DF30D02A2
	BOOL		m_bCanceled;
	//##ModelId=397C1D050070
	VARIANT_BOOL	m_bDragState;
	//##ModelId=396DF30D029A
	POINT		m_ptI;
	//##ModelId=396DF30D0284
	long		m_nGridSize;
	//##ModelId=396DF30D027C
	CLine *  m_pData;
	//##ModelId=396DF30D0266
	CLine *  m_pSelectedLines;
	//##ModelId=396DF30D0252
	long		m_nLines;
	//##ModelId=396DF30D024A
	HOOK_DATA *	m_pHookData;

	//##ModelId=396DF30D0236
	ILineManEvents * m_pEvents;

	//##ModelId=396DF30D02B6
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		
		CLineMan *	pData = (CLineMan*)GetProp(hwnd, "HProp");
		CLine *		pDesData = pData->m_pData;
		CLine *		pTemp;
		BOOL		bPaint = FALSE;
		POINT		pt;
		
		if (uMsg == WM_SETCURSOR)
		{
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);
			HCURSOR hCur = NULL;
			// Iteramos por todos los elementos que tengamos

			pTemp = pDesData;
			while(pTemp)
			{
				if (pTemp->m_hWndHook == hwnd)
				{	
					if (pTemp->HandleHitTest(pt))
					{
						hCur = LoadCursor(_Module.m_hInst, MAKEINTRESOURCE(IDC_MYCURSOR));
						SetCursor(hCur);
						return TRUE;
					}
				}
				pTemp = pTemp->m_pNext;
			}
			hCur = LoadCursor(NULL, MAKEINTRESOURCE(32512));
			SetCursor(hCur);
			return TRUE;
		}
		else if (uMsg == WM_PAINT)
		{
			pTemp = pDesData;
			while(pTemp)
			{
				if (pTemp->m_hWndPaint == hwnd)
				{	
					if (!bPaint)
					{
						CallWindowProc(pData->GetWndProcByHWND(hwnd), hwnd, uMsg, wParam, lParam);
						bPaint = TRUE;
					}
					HDC dc = GetDC(hwnd);
					pTemp->DrawLine(dc);
					ReleaseDC(hwnd, dc);		
				}
				pTemp = pTemp->m_pNext;
			}
			//return TRUE;
		}
		else if (uMsg == WM_LBUTTONDOWN)
		{
			if (!pData->m_bDragState)
			{
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);

			
				
				pData->m_bCanceled = FALSE;


				// Si estamos con shift o control pulsados no comprobamos movimiento múltiple

				if (pData->m_pSelectedLines && !(pData->m_pSelectedLines->m_pSelectedNext) && hwnd == pData->m_pSelectedLines->m_hWndHook)
				{
					// Tenemos monoSelección
					if (pData->m_pSelectedLines->HandleHitTest(pt))
					{
						if (pData->m_pSelectedLines->BeginDrag(pt, pData->m_nGridSize))
							pData->LaunchLineMoved(pData->m_pSelectedLines);
						return TRUE;
						
					}
					else if (pData->m_pSelectedLines->LineHitTest(pt))
					{
						if (pData->m_pSelectedLines->BeginLineDrag(pt, pData->m_nGridSize))
							pData->LaunchLineMoved(pData->m_pSelectedLines);
						return TRUE;
					}
				}

				
				// Vamos a comprobar el caso Multiselección
				if (!((wParam & MK_CONTROL) || (wParam & MK_SHIFT)))
				{
					if (pData->m_pSelectedLines && pData->m_pSelectedLines->m_pSelectedNext && hwnd == pData->m_pSelectedLines->m_hWndHook)
					{
						// Tenemos al menos dos
						pTemp = pData->m_pSelectedLines;
						while(pTemp)
						{
							if (pTemp->LineHitTest(pt))
							{
								pData->BeginMultiDrag(pTemp->m_hWndHook, pt);
								return TRUE;
							}
							pTemp = pTemp->m_pSelectedNext;
						}
					}
				}
				
				if (pData->m_pSelectedLines)
					if (pData->m_pSelectedLines->m_hWndHook != hwnd)
						return TRUE;
				
				// En el caso de estar aquí, nos lanzamos a comprobar la pulsación de una Línea
				pTemp = pData->m_pData;

				while (pTemp)
				{
					if ((pTemp->m_hWndHook == hwnd) && (pTemp->LineHitTest(pt)))
					{
						// Podemos proceder a moverla
						if (pTemp->BeginLineDrag(pt, pData->m_nGridSize))
							pData->LaunchLineMoved(pTemp);
						// Forzamos el ButtonUp
						GetCursorPos(&pt);
						ScreenToClient(hwnd, &pt);
						pData->ButtonUp(hwnd, pt, wParam);
						return TRUE;
					}
					pTemp = pTemp->m_pNext;
				}
			}

		}
		else if (uMsg == WM_LBUTTONUP)
		{
			if (!pData->m_bDragState)
			{
				// Controlamos la multiselección.
				pt.x = LOWORD(lParam); 
				pt.y = HIWORD(lParam);

				if (!pData->m_bCanceled)
				{
					pData->ButtonUp(hwnd, pt, wParam);
				}
				pData->m_bCanceled = FALSE;
			}

		}
		else if (uMsg == WM_RBUTTONDOWN)
		{
			if (!pData->m_bDragState)
			{
				// Evitamos que el evento se propage a VB si estamos encima de una línea
				pt.x = LOWORD(lParam); 
				pt.y = HIWORD(lParam);

				CLine * pLine = pData->m_pData;

				while (pLine)
				{
					if ((pLine->m_hWndHook == hwnd) && pLine->LineHitTest(pt))
					{
						pData->LaunchLineSelected(pLine);
						return TRUE;
					}
					pLine = pLine->m_pNext;
				}
			}
		}
		else if (uMsg == WM_RBUTTONUP)
		{
			if (!pData->m_bDragState)
			{
				// Controlamos la multiselección.
				pt.x = LOWORD(lParam); 
				pt.y = HIWORD(lParam);

				CLine * pLine = pData->m_pSelectedLines;

				while (pLine)
				{
					if ((pLine->m_hWndHook == hwnd) && pLine->LineHitTest(pt))
					{
						pData->LaunchRButtonUp(pLine);
						return TRUE;
					}
					pLine = pLine->m_pSelectedNext;
				}
				
				pLine = pData->m_pData;
				while (pLine)
				{
					if ((pLine->m_hWndHook == hwnd) && pLine->LineHitTest(pt))
					{
						pData->LaunchRButtonUp(pLine);
						return TRUE;
					}
					pLine = pLine->m_pNext;
				}
			}
		}

		return CallWindowProc(pData->GetWndProcByHWND(hwnd), hwnd, uMsg, wParam, lParam);
	}
};

#endif //__LINEMAN_H_
