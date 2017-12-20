// Animate.cpp : Implementation of CAnimate
#include "stdafx.h"
#include "M4Anim.h"
#include "Animate.h"

/////////////////////////////////////////////////////////////////////////////
// CAnimate

STDMETHODIMP CAnimate::ShowAnimate(long hWnd, long nLeft, long nTop, long nWidth, long nHeight, nAnimateType nType, BSTR strFileName, VARIANT_BOOL bTransparent, VARIANT_BOOL bFirstFrame)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;
	// TODO: Add your implementation code here

	if (m_pAnimate == NULL)
	{
		RECT rc;
		rc.top = nTop;
		rc.left = nLeft;
		rc.right = nLeft + nWidth;
		rc.bottom = nTop + nHeight;

		theParent = CWnd::FromHandle((HWND) hWnd);
		if (theParent != NULL)
		{
			UINT nResourceId = -1;
			LPSTR  theFileName;
			
			switch (nType)
			{
				case aviFromFile:
					
					if(::SysStringLen(strFileName) > 0)
					{
						theFileName = OLE2A(strFileName);
						nResourceId = 0;
					}
					else
						return E_INVALIDARG;
					
					break;
				case aviSearchFolder:
					nResourceId = IDR_SF;
					break;
				case aviSearchFile:
					nResourceId = IDR_SFS;
					break;
				case aviFileDestroy:
					nResourceId = IDR_DTF;
					break;
				case aviRecFileDestroy:
					nResourceId = IDR_RDF;
					break;
				case aviFileToRec:
					nResourceId = IDR_DF;
					break;
				case aviSearchComputer:
					nResourceId = IDR_SC;
					break;
				case aviCopyFiles:
					nResourceId = IDR_MF;
					break;
				case aviCopyFile:
					nResourceId = IDR_CF;
					break;
				case aviMindInit:
					nResourceId = IDR_MI;
					break;
				case aviMindFour:
					nResourceId = IDR_MINDINIT;
					break;
				case aviMindSave:
					nResourceId = IDR_MINDSAVE;
					break;
				default:
					nResourceId = nType;
					break;

			}
			
			
			if (nResourceId >= 0)
			{
				m_pAnimate = new CAnimateCtrl;
				long Style;
				
				Style = WS_CHILD | WS_VISIBLE | ACS_CENTER | ((bTransparent == -1)*ACS_TRANSPARENT) | ((bFirstFrame == -1)*ACS_AUTOPLAY);
				
				if (m_pAnimate->Create( Style , rc , theParent, 1))
				{
					long nReturnValue;
					if (nResourceId != 0)
					{
						nReturnValue = m_pAnimate->Open(nResourceId);
					}
					else
					{
						nReturnValue = m_pAnimate->Open(theFileName);
					}
					if (nReturnValue != 0)
					{
						//m_pAnimate->Play(0, 0xFFFF, -1);
					}
					else
					{
						delete m_pAnimate;
						m_pAnimate = NULL;
						return E_INVALIDARG;
					}
				}
			}
			else
				return E_INVALIDARG;
		}
	}

	return S_OK;
}

STDMETHODIMP CAnimate::move(long nTop, long nLeft, long nWidth, long nHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (m_pAnimate != NULL)
	{
		m_pAnimate->MoveWindow(nTop, nLeft, nWidth, nHeight);
	}

	return S_OK;
}

STDMETHODIMP CAnimate::Play()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (m_pAnimate != NULL)
	{
		m_pAnimate->Play(-1,-1,-1);
	}

	return S_OK;
}

STDMETHODIMP CAnimate::Stop(VARIANT_BOOL bPosInFirstFrame)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (m_pAnimate != NULL)
	{
		m_pAnimate->Stop();
		if (bPosInFirstFrame)
		{
			m_pAnimate->Seek(0);
		}
	}
	
	return S_OK;
}
