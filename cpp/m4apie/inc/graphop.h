// GraphOp.h : Declaration of the CGraphOp

#ifndef __GRAPHOP_H_
#define __GRAPHOP_H_

#include "resource.h"       // main symbols


#define MAXIMO	250


/////////////////////////////////////////////////////////////////////////////
// CGraphOp

//##ModelId=396B329503CB
class ATL_NO_VTABLE CGraphOp : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGraphOp, &CLSID_GraphOp>,
	public IDispatchImpl<IGraphOp, &IID_IGraphOp, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B3296010F
	CGraphOp()
	{
		m_nBackColor = 0x8000000F;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GRAPHOP)

BEGIN_COM_MAP(CGraphOp)
	COM_INTERFACE_ENTRY(IGraphOp)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IGraphOp
public:
	//##ModelId=396B354D0119
	STDMETHOD (DrawLine)(long hWnd, long pAX, long pAY, long pBX, long pBY, long nWidth, OLE_COLOR color, M4LineStyle nStyle);

	//##ModelId=396B329600FD
	STDMETHOD(GetPictureFromHandle)(/*[in]*/ long hIcon, /*[out,retval]*/ IPictureDisp * * ppRetVal);
//	STDMETHOD(GiveMeNewPictureIcon)(/*[out, retval]*/ IPictureDisp * * ppRetPicture);
	//##ModelId=396B329600F3
	STDMETHOD(GiveMeNewPicture)(/*[out,retval]*/ IPictureDisp * * ppPicRet);
	//##ModelId=396B329600E7
	STDMETHOD(GetDisabledPicture)(/*[in]*/ IPictureDisp * pPictureIn, OLE_COLOR * nColor, /*[out,retval]*/ IPictureDisp * * ppPictureRet);
	//##ModelId=396B329600D6
	STDMETHOD(TransformHimetric)(/*[in]*/ IPictureDisp *pDisp, /*[in,out]*/long *Width, /*[in, out]*/long *Height);
	//##ModelId=396B329600C1
	void DrawBorder(HDC hDC, RECT rc);
	//##ModelId=396B329600AB
	STDMETHOD(DrawItem)(/*[in]*/ long hDC, /*[in]*/ long Left, /*[in]*/long Top,/*[in]*/ long Right, /*[in]*/ long Bottom, /*[in]*/ long Style);
	//##ModelId=396B32960097
	STDMETHOD(DrawRectWithClipWnd)(long hWnd, long Left, long Top, long Right, long Bottom);
	//##ModelId=396B329600D3
	STDMETHOD(DrawBorder)(/*[in]*/ long hWnd);
	//##ModelId=396B32960071
	STDMETHOD(DrawDragBox)(/*[in]*/ long hWnd, /*[in]*/ long Left, /*[in]*/ long Top, /*[in]*/ long Right, /*[in]*/ long Bottom);
	//##ModelId=396B32960065
	STDMETHOD(DrawGrayBox)(/*[in]*/ long x1, /*[in]*/ long y1, /*[in]*/ long x2, /*[in]*/ long y2);
	//##ModelId=396B32960052
	STDMETHOD(CreateEMFFromWnd)(long hWnd, BSTR strFileName, BOOL bClient);
	//##ModelId=396B32960048
	STDMETHOD(DrawGrid)(long hWnd, long nGridSize);

private:
	//##ModelId=396B32960021
	OLE_COLOR m_nBackColor;
	//##ModelId=396B32960029
	void DitherBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, 
				int nHeight, HBITMAP hbm, int nXSrc, int nYSrc);
};

#endif //__GRAPHOP_H_
