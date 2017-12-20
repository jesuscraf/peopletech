// Animate.h : Declaration of the CAnimate

#ifndef __ANIMATE_H_
#define __ANIMATE_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAnimate
class ATL_NO_VTABLE CAnimate : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAnimate, &CLSID_Animate>,
	public IDispatchImpl<IAnimate, &IID_IAnimate, &LIBID_M4ANIMLib>
{
public:
	CAnimate()
	{
		m_pAnimate = NULL;
	}

	~CAnimate()
	{
		if (m_pAnimate != NULL)
		{
			// no vaya a ser que ya esté destruída
			if (m_pAnimate->m_hWnd != NULL)
			{
				
				m_pAnimate->Stop();
				m_pAnimate->Close();
				// Nos aseguramos de hacer el trabajo sucio para que la CWnd no pete en el destructor
				::InvalidateRect(m_pAnimate->m_hWnd, NULL, TRUE);
				::DestroyWindow(m_pAnimate->m_hWnd);
				delete m_pAnimate;
				m_pAnimate = NULL;
			}
		}
	}


	
DECLARE_REGISTRY_RESOURCEID(IDR_ANIMATE)

BEGIN_COM_MAP(CAnimate)
	COM_INTERFACE_ENTRY(IAnimate)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IAnimate

public:
	STDMETHOD(Stop)(VARIANT_BOOL bPosInFirstFrame);
	STDMETHOD(Play)();
	STDMETHOD(move)(/*[in]*/ long nLeft, /*[in]*/ long nTop, /*[in]*/ long nWidth, /*[in]*/ long nHeight);
	STDMETHOD(ShowAnimate)(/*[in]*/ long hWnd, /*[in]*/ long nTop, /*[in]*/ long nLeft, /*[in]*/ long nWidth, /*[in]*/ long nHeight, /*[in]*/ nAnimateType nType, /*[in, optional]*/ BSTR strFileName, VARIANT_BOOL bTransparent, VARIANT_BOOL bFirstFrame);

private:
	CAnimateCtrl	* m_pAnimate;
	CWnd			* theParent;
};

#endif //__ANIMATE_H_
