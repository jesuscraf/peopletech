// M4Tree32Ctl.cpp : Implementation of the CM4Tree32Ctrl ActiveX Control class.

#include "stdafx.h"
#include "limits.h"
#include "math.h"
#include "malloc.h"
#include "string.h"
#include "cm4tree.hpp"
#include "cm4bitmp.hpp"
#include "m4tree32.h"
#include "m4tree32ctl.h"
#include "m4tree32ppg.h"

#include "m4prndlg.h"

#define NAMEDLL "M4TREE32"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CM4Tree32Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CM4Tree32Ctrl, COleControl)
	//{{AFX_MSG_MAP(CM4Tree32Ctrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CM4Tree32Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CM4Tree32Ctrl)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sSiblingDistance", m_sSiblingDistance, OnSSiblingDistanceChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sLevelDistance", m_sLevelDistance, OnSLevelDistanceChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sLinkType", m_sLinkType, OnSLinkTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sLeftOffset", m_sLeftOffset, OnSLeftOffsetChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sTopOffset", m_sTopOffset, OnSTopOffsetChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sTreeStyle", m_sTreeStyle, OnSTreeStyleChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sAppearance", m_sAppearance, OnSAppearanceChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "bCompact", m_bCompact, OnBCompactChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "bBalance", m_bBalance, OnBBalanceChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sLinkStyle", m_sLinkStyle, OnSLinkStyleChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "dScale", m_dScale, OnDScaleChanged, VT_R8)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "bShow", m_bShow, OnBShowChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "sLinkWidth", m_sLinkWidth, OnSLinkWidthChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CM4Tree32Ctrl, "bStoreBitmap", m_bStoreBitmap, OnBStoreBitmapChanged, VT_BOOL)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4DeleteNode", M4DeleteNode, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetLastError", M4GetLastError, VT_I4, VTS_NONE)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetIDNode", M4GetIDNode, VT_I4, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4PrintTree", M4PrintTree, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4CutNode", M4CutNode, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4PasteNode", M4PasteNode, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetLevelCount", M4GetLevelCount, VT_I4, VTS_NONE)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeCount", M4GetNodeCount, VT_I4, VTS_NONE)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeLevel", M4GetNodeLevel, VT_I4, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeNumberChildren", M4GetNodeNumberChildren, VT_I4, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4SetNodeValueLong", M4SetNodeValueLong, VT_BOOL, VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4SetNodeValueString", M4SetNodeValueString, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeValueLong", M4GetNodeValueLong, VT_BOOL, VTS_I4 VTS_BSTR VTS_PI4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeValueString", M4GetNodeValueString, VT_BOOL, VTS_I4 VTS_BSTR VTS_PBSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeProperty", M4GetNodeProperty, VT_BOOL, VTS_I4 VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeNumberProperties", M4GetNodeNumberProperties, VT_I4, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4SetStyleValueLong", M4SetStyleValueLong, VT_BOOL, VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4SetStyleValueString", M4SetStyleValueString, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetStyleValueLong", M4GetStyleValueLong, VT_BOOL, VTS_I4 VTS_BSTR VTS_PI4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetStyleValueString", M4GetStyleValueString, VT_BOOL, VTS_I4 VTS_BSTR VTS_PBSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetStyleProperty", M4GetStyleProperty, VT_BOOL, VTS_I4 VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetStyleNumberProperties", M4GetStyleNumberProperties, VT_I4, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeExpand", M4GetNodeExpand, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4SetNodeExpand", M4SetNodeExpand, VT_BOOL, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4ExpandLevel", M4ExpandLevel, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4FindLongTree", M4FindLongTree, VT_I4, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4FindStringTree", M4FindStringTree, VT_I4, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4CreateStyle", M4CreateStyle, VT_I4, VTS_NONE)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4SetNodeStyle", M4SetNodeStyle, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4SetLevelStyle", M4SetLevelStyle, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeStyle", M4GetNodeStyle, VT_I4, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4DeleteNodeProperty", M4DeleteNodeProperty, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4DeleteStyleProperty", M4DeleteStyleProperty, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4InsertNode", M4InsertNode, VT_I4, VTS_I4)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetTreeStatus", M4GetTreeStatus, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CM4Tree32Ctrl, "M4GetNodeIDNode", M4GetNodeIDNode, VT_I4, VTS_I4 VTS_I4)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_BORDERSTYLE()
	DISP_STOCKPROP_APPEARANCE()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CM4Tree32Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CM4Tree32Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CM4Tree32Ctrl)
	EVENT_CUSTOM("M4LClickInNode", FireM4LClickInNode, VTS_I4)
	EVENT_CUSTOM("M4LDbClickInNode", FireM4LDbClickInNode, VTS_I4)
	EVENT_CUSTOM("M4RClickInNode", FireM4RClickInNode, VTS_I4)
	EVENT_CUSTOM("M4RDbClickInNode", FireM4RDbClickInNode, VTS_I4)
	EVENT_CUSTOM("M4ClickOutNode", FireM4ClickOutNode, VTS_NONE)
	EVENT_CUSTOM("M4RClickOutNode", FireM4RClickOutNode, VTS_NONE)
	EVENT_CUSTOM("M4LClickOutNode", FireM4LClickOutNode, VTS_NONE)
	EVENT_CUSTOM("M4RDbClickOutNode", FireM4RDbClickOutNode, VTS_NONE)
	EVENT_CUSTOM("M4LDbClickOutNode", FireM4LDbClickOutNode, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CM4Tree32Ctrl, 1)
	PROPPAGEID(CM4Tree32PropPage::guid)
END_PROPPAGEIDS(CM4Tree32Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CM4Tree32Ctrl, "M4TREE32.M4Tree32Ctrl.1",
	0x64f5a7a8, 0xbfa7, 0x11d0, 0x8d, 0xa1, 0, 0xc0, 0x4f, 0xb7, 0x14, 0x6e)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CM4Tree32Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DM4Tree32 =
		{ 0x64f5a7a6, 0xbfa7, 0x11d0, { 0x8d, 0xa1, 0, 0xc0, 0x4f, 0xb7, 0x14, 0x6e } };
const IID BASED_CODE IID_DM4Tree32Events =
		{ 0x64f5a7a7, 0xbfa7, 0x11d0, { 0x8d, 0xa1, 0, 0xc0, 0x4f, 0xb7, 0x14, 0x6e } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwM4Tree32OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CM4Tree32Ctrl, IDS_M4TREE32, _dwM4Tree32OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::CM4Tree32CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CM4Tree32Ctrl

BOOL CM4Tree32Ctrl::CM4Tree32CtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_M4TREE32,
			IDB_M4TREE32,
			afxRegApartmentThreading,
			_dwM4Tree32OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}

#ifndef NO_MORE_LICENSES_PLEASE	
/////////////////////////////////////////////////////////////////////////////
// Licensing strings

static const TCHAR BASED_CODE _szLicFileName[] = _T("M4Tree32.lic");

static const WCHAR BASED_CODE _szLicString[] =
	L"Copyright (c) 1997 Meta4";


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::CM4Tree32CtrlFactory::VerifyUserLicense -
// Checks for existence of a user license

BOOL CM4Tree32Ctrl::CM4Tree32CtrlFactory::VerifyUserLicense()
{
	return AfxVerifyLicFile(AfxGetInstanceHandle(), _szLicFileName,
		_szLicString);
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::CM4Tree32CtrlFactory::GetLicenseKey -
// Returns a runtime licensing key

BOOL CM4Tree32Ctrl::CM4Tree32CtrlFactory::GetLicenseKey(DWORD dwReserved,
	BSTR FAR* pbstrKey)
{
	if (pbstrKey == NULL)
		return FALSE;

	*pbstrKey = SysAllocString(_szLicString);
	return (*pbstrKey != NULL);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::CM4Tree32Ctrl - Constructor

CM4Tree32Ctrl::CM4Tree32Ctrl()
{
	InitializeIIDs(&IID_DM4Tree32, &IID_DM4Tree32Events);
	
	m_pTree = NULL;
	m_bRebuild = FALSE;
	m_bResize = FALSE;
	m_bPrinting = FALSE;
	m_lNodeDown = 0L;
	m_lMaxPosHorz = 0L;
	m_lMaxPosVert = 0L;
	m_lOffsetScrollX = 0L;
	m_lOffsetScrollY = 0L;
	m_pMaxWidthLevel = NULL;
	m_pMaxHeightLevel = NULL;
	m_lNextStyle = 0;
	m_lError = M4TREE_NO_ERROR;
	m_bInvalidateAll = TRUE;
	for (long i=0L; i<M4TREE_SIZE_LIST_STYLE; i++)
	{
		m_pStyle[i] = NULL;
	}
	m_pBitmp = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::~CM4Tree32Ctrl - Destructor

CM4Tree32Ctrl::~CM4Tree32Ctrl()
{
	M4DELETE(m_pTree);
	M4DELETEV(m_pMaxWidthLevel);
	M4DELETEV(m_pMaxHeightLevel);
	for (long i=0L; i<M4TREE_SIZE_LIST_STYLE; i++)
	{
		M4DELETE(m_pStyle[i]);
	}
	if (m_pBitmp)
	{
		for (long i=0L; i<m_pBitmp->M4GetCount (); i++)
		{
			CBitmap *pBitmap = m_pBitmp->M4GetBitmap (i);
			if (pBitmap)
			{
				DeleteObject (pBitmap->m_hObject);
				if (m_pBitmp->M4GetDeleteBitmap (i) == TRUE)
					delete pBitmap;
				pBitmap = NULL;
			}
		}
		M4DELETE(m_pBitmp);
	}
}


int CM4Tree32Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableScrollBarCtrl(SB_HORZ, TRUE);
	SetScrollRange(SB_HORZ, M4TREE_SCROLL_MIN, M4TREE_SCROLL_MAX, TRUE);
	
	EnableScrollBarCtrl(SB_VERT, TRUE);
	SetScrollRange(SB_VERT, M4TREE_SCROLL_MIN, M4TREE_SCROLL_MAX, TRUE);

	m_lNextStyle = 1;
	m_pStyle[0] = new CM4Style (20);
	if (m_pStyle[0] == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("OnCreate: Ha fallado la reserva de memoria");
		#endif
		return 0;
	}

	m_pBitmp = new CM4Bitmp (10);

	CString csProp;

	csProp = M4TREE_CONTROL_A; csProp += M4TREE_PROP_CVO; m_pStyle[0]->M4Insert (csProp, "P0O,T0");
	csProp = M4TREE_CONTROL_A; csProp += M4TREE_PROP_CVC; m_pStyle[0]->M4Insert (csProp, "P0C,T0");
	csProp = M4TREE_CONTROL_A; csProp += M4TREE_PROP_CVN; m_pStyle[0]->M4Insert (csProp, "P0N,T0");
	csProp = M4TREE_CONTROL_A; csProp += M4TREE_PROP_FC; m_pStyle[0]->M4Insert (csProp, (long) TranslateColor(GetForeColor()));
	csProp = M4TREE_CONTROL_A; csProp += M4TREE_PROP_BC; m_pStyle[0]->M4Insert (csProp, (long) TranslateColor(GetBackColor()));

	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_X; m_pStyle[0]->M4Insert (csProp, 24);
	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_Y; m_pStyle[0]->M4Insert (csProp, 5);
	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_FC; m_pStyle[0]->M4Insert (csProp, (long) RGB(0,0,0));
	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_FN; m_pStyle[0]->M4Insert (csProp, "Arial");
	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_FS; m_pStyle[0]->M4Insert (csProp, 12);
	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_FW; m_pStyle[0]->M4Insert (csProp, 700);
	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_FI; m_pStyle[0]->M4Insert (csProp, (long) FALSE);
	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_FU; m_pStyle[0]->M4Insert (csProp, (long) FALSE);
	csProp = M4TREE_CONTROL_T; csProp += "0"; csProp += M4TREE_PROP_FK; m_pStyle[0]->M4Insert (csProp, (long) FALSE);

	csProp = M4TREE_CONTROL_P; csProp += "0O"; csProp += M4TREE_PROP_X; m_pStyle[0]->M4Insert (csProp, 5);
	csProp = M4TREE_CONTROL_P; csProp += "0O"; csProp += M4TREE_PROP_Y; m_pStyle[0]->M4Insert (csProp, 4);
	csProp = M4TREE_CONTROL_P; csProp += "0O"; csProp += M4TREE_PROP_BT; m_pStyle[0]->M4Insert (csProp, IDB_OPEN);

	csProp = M4TREE_CONTROL_P; csProp += "0C"; csProp += M4TREE_PROP_X; m_pStyle[0]->M4Insert (csProp, 5);
	csProp = M4TREE_CONTROL_P; csProp += "0C"; csProp += M4TREE_PROP_Y; m_pStyle[0]->M4Insert (csProp, 4);
	csProp = M4TREE_CONTROL_P; csProp += "0C"; csProp += M4TREE_PROP_BT; m_pStyle[0]->M4Insert (csProp, IDB_CLOSE);

	csProp = M4TREE_CONTROL_P; csProp += "0N"; csProp += M4TREE_PROP_X; m_pStyle[0]->M4Insert (csProp, 5);
	csProp = M4TREE_CONTROL_P; csProp += "0N"; csProp += M4TREE_PROP_Y; m_pStyle[0]->M4Insert (csProp, 4);
	csProp = M4TREE_CONTROL_P; csProp += "0N"; csProp += M4TREE_PROP_BT; m_pStyle[0]->M4Insert (csProp, IDB_NODE);

	return 0;
}


void CM4Tree32Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	CRect rcView = rcBounds;
	if ((m_pTree) && (m_bShow == TRUE))
	{
		if (m_bResize == TRUE)
		{
			SizeNode (pdc);
			m_bResize = FALSE;
			m_bRebuild = TRUE;
		}
		if (m_bRebuild == TRUE)
		{
			SetRectTree ();

			long lLeft, lTop, lRight, lBottom;
			m_pTree->M4GetRootNode ()->M4GetRectTotal (lLeft, lTop, lRight, lBottom);
			long lWidthTree = (long) (((double) (lRight-lLeft+(m_sLeftOffset*2)))*m_dScale);
			long lHeightTree = (long) (((double) (lBottom-lTop+(m_sTopOffset*2)))*m_dScale);

			if (lWidthTree > rcView.Width ())
				ShowScrollBar(SB_HORZ, TRUE);
			else
				ShowScrollBar(SB_HORZ, FALSE);

			if (lHeightTree > rcView.Height ())
				ShowScrollBar(SB_VERT, TRUE);
			else
				ShowScrollBar(SB_VERT, FALSE);

			CRect rcNewView;
			GetClientRect (&rcNewView);
			if (rcView.EqualRect (rcNewView) == FALSE)
				return;

			long lNodeLeft, lNodeTop, lNodeRight, lNodeBottom;
			m_pTree->M4GetCurrentNode ()->M4GetRect (lNodeLeft, lNodeTop, lNodeRight, lNodeBottom);
			
			if (lWidthTree > rcView.Width ())
			{
				m_lMaxPosHorz = lWidthTree-rcView.Width ();
				long lWidth, lNotUsed;
				m_pTree->M4GetCurrentNode ()->M4GetSize (lWidth, lNotUsed);
				if (m_sTreeStyle == M4TREE_TREE_DIAG)
					m_lOffsetScrollX = (long) (((double) -lNodeLeft)*m_dScale);
				else
					m_lOffsetScrollX = (long) (((double) (-lNodeLeft-m_sLeftOffset-Middle (lWidth)))*m_dScale+ (double) Middle (rcView.Width ()));
				long lPos = (long) ((((double) -m_lOffsetScrollX)*((double) M4TREE_SCROLL_MAX))/((double) m_lMaxPosHorz));
				if (lPos<M4TREE_SCROLL_MIN)
				{
					lPos = M4TREE_SCROLL_MIN;
					m_lOffsetScrollX = (long) ((((double) -lPos)*((double) m_lMaxPosHorz))/((double) M4TREE_SCROLL_MAX));
				}
				if (lPos>M4TREE_SCROLL_MAX)
				{
					lPos = M4TREE_SCROLL_MAX;
					m_lOffsetScrollX = (long) ((((double) -lPos)*((double) m_lMaxPosHorz))/((double) M4TREE_SCROLL_MAX));
				}
				SetScrollPos (SB_HORZ, (int) lPos, TRUE);
			}
			else
			{
		 		if ((m_sTreeStyle == M4TREE_TREE_VERT) || (m_sTreeStyle == M4TREE_DEFAULT_PROPERTY))
					m_lOffsetScrollX = Middle (rcView.Width ()-lWidthTree);
				else
					m_lOffsetScrollX = 0L;
			}

			if (lHeightTree > rcView.Height ())
			{
				m_lMaxPosVert = lHeightTree-rcView.Height ();
				long lHeight, lNotUsed;
				m_pTree->M4GetCurrentNode ()->M4GetSize (lNotUsed, lHeight);
				if (m_sTreeStyle == M4TREE_TREE_DIAG)
					m_lOffsetScrollY = (long) (((double) -lNodeTop)*m_dScale);
				else
					m_lOffsetScrollY = (long) (((double) (-lNodeTop-m_sTopOffset-Middle (lHeight)))*m_dScale+ (double) Middle (rcView.Height ()));
				long lPos = (long) ((((double) -m_lOffsetScrollY)*((double) M4TREE_SCROLL_MAX))/((double) m_lMaxPosVert));
				if (lPos<M4TREE_SCROLL_MIN)
				{
					lPos = M4TREE_SCROLL_MIN;
					m_lOffsetScrollY = (long) ((((double) -lPos)*((double) m_lMaxPosVert))/((double) M4TREE_SCROLL_MAX));
				}
				if (lPos>M4TREE_SCROLL_MAX)
				{
					lPos = M4TREE_SCROLL_MAX;
					m_lOffsetScrollY = (long) ((((double) -lPos)*((double) m_lMaxPosVert))/((double) M4TREE_SCROLL_MAX));
				}
				SetScrollPos (SB_VERT, (int) lPos, TRUE);
			}
			else
			{
				if (m_sTreeStyle == M4TREE_TREE_HORZ)
		    		m_lOffsetScrollY = Middle (rcView.Height ()-lHeightTree);
				else
					m_lOffsetScrollY = 0L;
			}
			m_bRebuild = FALSE;
		}

		CDC *pDCMen = new CDC;
		if (pDCMen == NULL)
		{
			m_lError = M4TREE_NO_MEMORY;
			#ifdef _DEBUG
				// AfxMessageBox ("OnDraw: Ha fallado la reserva de memoria");
			#endif
			return;
		}

		pDCMen->CreateCompatibleDC (pdc);

		CBitmap *pBitmap = new CBitmap;
		if (pBitmap == NULL)
		{
			M4DELETE(pDCMen);
			m_lError = M4TREE_NO_MEMORY;
			#ifdef _DEBUG
				// AfxMessageBox ("OnDraw: Ha fallado la reserva de memoria");
			#endif
			return;
		}
		pBitmap->CreateCompatibleBitmap (pdc, rcView.Width (), rcView.Height ());
	    CBitmap *oldBitmap = pDCMen->SelectObject (pBitmap);

		CRect rcBorder = rcView;
		rcView.left++;
		rcView.top++;
		rcView.right--;
		rcView.bottom--;
		FillSolidRect(pDCMen, rcBorder.left, rcBorder.top, rcBorder.Width (), rcBorder.Height (), TranslateColor(GetBackColor()));
		DrawTree (pDCMen, m_pTree->M4GetRootNode (), rcView);
		pdc->BitBlt (rcBorder.left, rcBorder.top, rcBorder.Width (), rcBorder.Height (), pDCMen, 0, 0, SRCCOPY);

		pDCMen->SelectObject (oldBitmap);
		DeleteObject (pBitmap->m_hObject);
		M4DELETE(pBitmap);
		
		DeleteDC (pDCMen->m_hDC);
		M4DELETE(pDCMen);
	}
	else
	{
		if (GetHwnd ())
			ShowScrollBar(SB_BOTH, FALSE);
		FillSolidRect(pdc, rcView.left, rcView.top, rcView.Width (), rcView.Height (), TranslateColor(GetBackColor()));
	}

	if (!IsOptimizedDraw())
	{
		// The container does not support optimized drawing.

		// TODO: if you selected any GDI objects into the device context *pdc,
		//		restore the previously-selected objects here.
		//		For more information, please see MFC technical note #nnn,
		//		"Optimizing an ActiveX Control".
	}
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::DoPropExchange - Persistence support

void CM4Tree32Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

    PX_Short(pPX, _T("sTreeStyle"), m_sTreeStyle, M4TREE_DEFAULT_PROPERTY);
    PX_Short(pPX, _T("sSiblingDistance"), m_sSiblingDistance, 10);
    PX_Short(pPX, _T("sLevelDistance"), m_sLevelDistance, 10);
    PX_Short(pPX, _T("sLinkType"), m_sLinkType, M4TREE_DEFAULT_PROPERTY);
    PX_Short(pPX, _T("sLinkStyle"), m_sLinkStyle, M4TREE_DEFAULT_PROPERTY);
    PX_Short(pPX, _T("sLinkWidth"), m_sLinkWidth, 1);
    PX_Short(pPX, _T("sLeftOffset"), m_sLeftOffset, 10);
    PX_Short(pPX, _T("sTopOffset"), m_sTopOffset, 10);
    PX_Short(pPX, _T("sAppearance"), m_sAppearance, M4TREE_DEFAULT_PROPERTY);
    PX_Bool(pPX, _T("bCompact"), m_bCompact, FALSE);
    PX_Bool(pPX, _T("bBalance"), m_bBalance, FALSE);
    PX_Bool(pPX, _T("bShow"), m_bShow, TRUE);
    PX_Double(pPX, _T("dScale"), m_dScale, 1);
    PX_Bool(pPX, _T("bStoreBitmap"), m_bStoreBitmap, TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CM4Tree32Ctrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control can optimize its OnDraw method, by not restoring
	// the original GDI objects in the device context.
	dwFlags |= canOptimizeDraw;
	return dwFlags;
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::OnResetState - Reset control to default state

void CM4Tree32Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl::AboutBox - Display an "About" box to the user

void CM4Tree32Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_M4TREE32);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl message handlers


void CM4Tree32Ctrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	InNode (point, TRUE, TRUE);
	COleControl::OnLButtonDblClk(nFlags, point);
}

void CM4Tree32Ctrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	InNode (point, TRUE, FALSE);
	COleControl::OnLButtonDown(nFlags, point);
}

void CM4Tree32Ctrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_lNodeDown)
	{
		m_lNodeDown = 0L;
		InvalidateControl ();
	}
	COleControl::OnLButtonUp(nFlags, point);
}

void CM4Tree32Ctrl::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	InNode (point, FALSE, TRUE);
	COleControl::OnRButtonDblClk(nFlags, point);
}

void CM4Tree32Ctrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	InNode (point, FALSE, FALSE);
	COleControl::OnRButtonDown(nFlags, point);
}

void CM4Tree32Ctrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (m_lNodeDown)
	{
		m_lNodeDown = 0L;
		InvalidateControl ();
	}
	COleControl::OnRButtonUp(nFlags, point);
}

void CM4Tree32Ctrl::InNode (const CPoint point, BOOL bLeft, BOOL bDBClick)
{
	if (m_pTree == NULL)
		return;
	long lLeft, lTop, lRight, lBottom;

	CRect rcView;
	GetClientRect (&rcView);
	if (!((point.x>rcView.left) && (point.x<rcView.right) && (point.y>rcView.top) && (point.y<rcView.bottom)))
		return;

	CM4Node *pNode = m_pTree->M4GetRootNode ();
	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{
		pNode->M4GetRect(lLeft, lTop, lRight, lBottom);
		OffsetRect (lLeft, lTop, lRight, lBottom);
	
		if ((point.x>lLeft) && (point.x<lRight) && (point.y>lTop) && (point.y<lBottom))
		{
			m_pTree->M4SetCurrentNode (pNode);
			if (bDBClick == TRUE)
			{
				if (bLeft == TRUE)
				{
					FireM4LDbClickInNode (pNode->M4GetNodeID ());
				}
				else
				{
					FireM4RDbClickInNode (pNode->M4GetNodeID ());
				}
			}
			else
			{
				if (bLeft == TRUE)
				{
					FireM4LClickInNode (pNode->M4GetNodeID ());
				}
				else
				{
					FireM4RClickInNode (pNode->M4GetNodeID ());
				}
			}
		
			if (m_sAppearance == M4TREE_TREE_3D)
				m_lNodeDown = pNode->M4GetNodeID ();
			InvalidateControl ();
			return;
		}

		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && (pNode->M4GetExpand () == TRUE))
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
				{
					FireM4ClickOutNode ();
					if (bDBClick == TRUE)
					{
						if (bLeft == TRUE)
						{
							FireM4LDbClickOutNode ();
						}
						else
						{
							FireM4RDbClickOutNode ();
						}
					}
					else
					{
						if (bLeft == TRUE)
						{
							FireM4LClickOutNode ();
						}
						else
						{
							FireM4RClickOutNode ();
						}
					}
					return;
				}
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
						{
							FireM4ClickOutNode ();
							if (bDBClick == TRUE)
							{
								if (bLeft == TRUE)
								{
									FireM4LDbClickOutNode ();
								}
								else
								{
									FireM4RDbClickOutNode ();
								}
							}
							else
							{
								if (bLeft == TRUE)
								{
									FireM4LClickOutNode ();
								}
								else
								{
									FireM4RClickOutNode ();
								}
							}
							return;
						}
					}
				}
			}
		}
	}

	FireM4ClickOutNode ();
	if (bDBClick == TRUE)
	{
		if (bLeft == TRUE)
		{
			FireM4LDbClickOutNode ();
		}
		else
		{
			FireM4RDbClickOutNode ();
		}
	}
	else
	{
		if (bLeft == TRUE)
		{
			FireM4LClickOutNode ();
		}
		else
		{
			FireM4RClickOutNode ();
		}
	}
}

void CM4Tree32Ctrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	static long s_lPos = M4TREE_SCROLL_MIN;
	long lPos = (long) GetScrollPos (SB_VERT);
	switch (nSBCode)
	{
	    case SB_TOP        : lPos = M4TREE_SCROLL_MIN; break;
		case SB_BOTTOM     : lPos = M4TREE_SCROLL_MAX; break;
		case SB_PAGEUP     : lPos -= (M4TREE_SCROLL_MAX / 10); break;
		case SB_PAGEDOWN   : lPos += (M4TREE_SCROLL_MAX / 10); break;
		case SB_LINEUP     : lPos -= (M4TREE_SCROLL_MAX / 100); break;
		case SB_LINEDOWN   : lPos += (M4TREE_SCROLL_MAX / 100); break;
		case SB_THUMBTRACK : lPos = (long) nPos; break;
	}	

	if (s_lPos != lPos)
	{
		long lTemp = m_lOffsetScrollY;
		m_lOffsetScrollY = (long) (((double) -lPos)*((double) m_lMaxPosVert)/((double) M4TREE_SCROLL_MAX));
		SetScrollPos (SB_VERT, (int) lPos, TRUE);
		if (lTemp != m_lOffsetScrollY)
			InvalidateControl ();
		s_lPos = lPos;
	}

	COleControl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CM4Tree32Ctrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	static long s_lPos = 0L;
	long lPos = (long) GetScrollPos (SB_HORZ);
	switch (nSBCode)
	{
	    case SB_TOP        : lPos = M4TREE_SCROLL_MIN; break;
		case SB_BOTTOM     : lPos = M4TREE_SCROLL_MAX; break;
		case SB_PAGEUP     : lPos -= (M4TREE_SCROLL_MAX / 10); break;
		case SB_PAGEDOWN   : lPos += (M4TREE_SCROLL_MAX / 10); break;
		case SB_LINEUP     : lPos -= (M4TREE_SCROLL_MAX / 100); break;
		case SB_LINEDOWN   : lPos += (M4TREE_SCROLL_MAX / 100); break;
		case SB_THUMBTRACK : lPos = (long) nPos; break;
	}	
	
	if (s_lPos != lPos)
	{
		long lTemp = m_lOffsetScrollX;
		m_lOffsetScrollX = (long) (((double) -lPos)*((double) m_lMaxPosHorz)/((double) M4TREE_SCROLL_MAX));
		SetScrollPos (SB_HORZ, (int) lPos, TRUE);
		if (lTemp != m_lOffsetScrollY)
			InvalidateControl ();
		s_lPos = lPos;
	}
	
	COleControl::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CM4Tree32Ctrl::SetRectTree (void)
{
	switch (m_sTreeStyle)
	{
	default:
	case M4TREE_DEFAULT_PROPERTY:
	case M4TREE_TREE_VERT:
		return SetRectTreeVert ();
		break;
	case M4TREE_TREE_HORZ:
		return SetRectTreeHorz ();
		break;
	case M4TREE_TREE_DIAG:
		return SetRectTreeDiag ();
		break;
	}
	return FALSE;
}

BOOL CM4Tree32Ctrl::SetRectTreeVert (void)
{
	long *lLastPositionLevel = new long [m_pTree->M4GetLevelCount ()+2];
	if (lLastPositionLevel == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("SetRectTreeVert: Ha fallado la reserva de memoria");
		#endif
		return FALSE;
	}

	for (long i=0; i<=m_pTree->M4GetLevelCount (); i++)
		lLastPositionLevel[i] = 0L;

	BOOL bNotIf = FALSE;
	BOOL bExitWhile = FALSE;
	BOOL bExitWhile2 = FALSE;
	long TotalHeight = 0L;
	long lWidth, lHeight;
	long lLeft, lTop, lRight, lBottom;
	long lChildLeft, lChildRight, lNotUsed;
	long lLeftAux = 0L;
	long lLastPosition = 0L;

	CM4Node *pNode = m_pTree->M4GetRootNode ();
	CM4Node *pChild, *pSibling, *pParent;
	CM4Node *pChild2, *pSibling2, *pParent2, *pParent3;
	while (pNode)
	{
		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && (pNode->M4GetExpand () == TRUE))
			pNode = pChild;
		else
		{

//////////////////// Establecemos el RECT de un nodo sin hijos //////////
			// El nodo actual no tiene hijos.
			if (m_bCompact == TRUE)
			{
				if (lLastPositionLevel[pNode->M4GetLevel()]>0L)
					// Se coloca el nodo detrás del último nodo creado en ese nivel.
					lLeftAux = lLastPositionLevel[pNode->M4GetLevel()]+m_sSiblingDistance;
				else
					// Se coloca el nodo delante de todos.
					lLeftAux = 0L;
			}
			else
			{
				if (lLastPosition>0L)
					// Se coloca el nodo detrás del último nodo creado.
					lLeftAux = lLastPosition+m_sSiblingDistance;
				else
					// Se coloca el nodo delante de todos.
					lLeftAux = 0L;
			}
			pNode->M4GetSize (lWidth, lHeight);
			lLeft = lLeftAux;
			lRight = lLeft + lWidth;
			TotalHeight=0L;
			for (long i=1L; i<pNode->M4GetLevel (); i++)
				TotalHeight = TotalHeight + m_pMaxHeightLevel[i] + m_sLevelDistance;
			lTop = TotalHeight;
			lBottom = lTop + lHeight;
			pNode->M4SetRect (lLeft, lTop, lRight, lBottom);
			pNode->M4SetRectChilds (lLeft, lTop, lRight, lBottom);
			if (m_bCompact == TRUE)
				lLastPositionLevel[pNode->M4GetLevel()] = lRight;
			else
			if (lRight>lLastPosition)
				lLastPosition = lRight;
/////////////////////////////////////////////////////////////////////////

			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
				{
					M4DELETEV(lLastPositionLevel); 
					return TRUE;
				}
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{

//////////////////// Establecemos el RECT de un nodo con hijos //////////
					// El nodo actual tiene, al menos, un hijo.
					pParent->M4GetSize (lWidth, lHeight);
					m_pTree->M4SetRectChildNode (pParent);
					if (m_bBalance == TRUE)
						pParent->M4GetRectChilds (lChildLeft, lNotUsed, lChildRight, lNotUsed);
					else
					{
						pParent->M4GetFirstChild ()->M4GetRect (lChildLeft, lNotUsed, lNotUsed, lNotUsed);
						pParent->M4GetLastChild ()->M4GetRect (lNotUsed, lNotUsed, lChildRight, lNotUsed);
					}
					if ((lChildRight - lChildLeft) < lWidth)
					{
						// El nodo actual es más grande que sus hijos..
						if ((m_bCompact == TRUE) && (lLastPositionLevel[pParent->M4GetLevel()]>0L))
						{
							// Los hijos ya están bien colocados.
							// Se ajusta el nodo actual a sus hijos.
							lLeftAux = lChildLeft-Middle (lWidth-(lChildRight-lChildLeft));
						}
						else
						{
							bNotIf = FALSE;
							if (m_bBalance == FALSE)
							{
								pParent->M4GetRectChilds (lChildLeft, lNotUsed, lChildRight, lNotUsed);
								if ((lChildRight - lChildLeft) >= lWidth)
								{
									// El nodo actual es más pequeño que sus nietos.
									// Se ajusta el nodo actual a sus hijos.
									pParent->M4GetFirstChild ()->M4GetRect (lChildLeft, lNotUsed, lNotUsed, lNotUsed);
									pParent->M4GetLastChild ()->M4GetRect (lNotUsed, lNotUsed, lChildRight, lNotUsed);
									lLeftAux = lChildLeft+(Middle (lChildRight-lChildLeft))-Middle (lWidth);
									bNotIf = TRUE;
								}
							}

							if (bNotIf == FALSE)
							{
								// Se ajustan los hijos al nodo actual.
								pParent2 = pParent->M4GetFirstChild ();
								while (pParent2)
								{
									pParent2->M4GetRect (lLeft, lTop, lRight, lBottom);
									lLeft += Middle (lWidth-(lChildRight - lChildLeft));
									lRight += Middle (lWidth-(lChildRight - lChildLeft));
									pParent2->M4SetRect (lLeft, lTop, lRight, lBottom);
									if (m_bCompact == TRUE)
										lLastPositionLevel[pParent2->M4GetLevel()] = lRight;
									else
									if (lRight>lLastPosition)
										lLastPosition = lRight;

									pChild2 = pParent2->M4GetFirstChild ();
									if ((pChild2) && (pParent2->M4GetExpand () == TRUE))
										pParent2 = pChild2;
									else
									{
										pSibling2 = pParent2->M4GetNextSibling ();
										if (pSibling2)
											pParent2 = pSibling2;
										else
										{
											pParent3 = pParent2->M4GetParent ();
											if ((pParent3 == NULL) || (pParent3 == pParent))
												pParent2 = NULL;
											else
											{
												bExitWhile2 = FALSE;
												while (bExitWhile2 == FALSE)
												{
													pSibling2 = pParent3->M4GetNextSibling ();
													if (pSibling2)
													{
														pParent2 = pSibling2;
														bExitWhile2 = TRUE;
													}
													else
													{
														pParent3 = pParent3->M4GetParent ();
														if ((pParent3 == NULL) || (pParent3 == pParent))
														{
															bExitWhile2 = TRUE;
															pParent2 = NULL;
														}
													}
												}
											}
										}
									}
								}
								lLeftAux = lChildLeft;
							}
						}	
					}
					else
					{
						// El nodo actual es más pequeño que sus hijos.
						// Se ajusta el nodo actual a sus hijos.
						lLeftAux = lChildLeft+Middle (lChildRight - lChildLeft)-Middle (lWidth);
					}
					if ((m_bCompact == TRUE) && (lLastPositionLevel[pParent->M4GetLevel()]>0L) && (lLeftAux < (lLastPositionLevel[pParent->M4GetLevel()]+m_sSiblingDistance)))
					{
						// El nodo actual entra en conflicto con el último nodo de ese nivel.
						// Se colocan el nodo actual y sus hijos detrás del último nodo de ese nivel.
						pParent2 = pParent->M4GetFirstChild ();
						while (pParent2)
						{
							pParent2->M4GetRect (lLeft, lTop, lRight, lBottom);
							lLeft += (lLastPositionLevel[pParent->M4GetLevel()]+m_sSiblingDistance-lLeftAux);
							lRight += (lLastPositionLevel[pParent->M4GetLevel()]+m_sSiblingDistance-lLeftAux);
							pParent2->M4SetRect (lLeft, lTop, lRight, lBottom);
							lLastPositionLevel[pParent2->M4GetLevel()] = lRight;
	
							pChild2 = pParent2->M4GetFirstChild ();
							if ((pChild2) && (pParent2->M4GetExpand () == TRUE))
								pParent2 = pChild2;
							else
							{
								pSibling2 = pParent2->M4GetNextSibling ();
								if (pSibling2)
									pParent2 = pSibling2;
								else
								{
									pParent3 = pParent2->M4GetParent ();
									if ((pParent3 == NULL) || (pParent3 == pParent))
										pParent2 = NULL;
									else
									{
										bExitWhile2 = FALSE;
										while (bExitWhile2 == FALSE)
										{
											pSibling2 = pParent3->M4GetNextSibling ();
											if (pSibling2)
											{
												pParent2 = pSibling2;
												bExitWhile2 = TRUE;
											}
											else
											{
												pParent3 = pParent3->M4GetParent ();
												if ((pParent3 == NULL) || (pParent3 == pParent))
												{
													bExitWhile2 = TRUE;
													pParent2 = NULL;
												}
											}
										}
									}
								}
							}
						}
						lLeftAux = lLastPositionLevel[pParent->M4GetLevel()]+m_sSiblingDistance;
					}
					lLeft = lLeftAux;
					lRight = lLeft + lWidth;
					TotalHeight=0L;
					for (long i=1L; i<pParent->M4GetLevel (); i++)
						TotalHeight = TotalHeight + m_pMaxHeightLevel[i] + m_sLevelDistance;
					lTop = TotalHeight;
					lBottom = lTop + lHeight;
					pParent->M4SetRect (lLeft, lTop, lRight, lBottom);
					if (m_bCompact == TRUE)
						lLastPositionLevel[pParent->M4GetLevel()] = lRight;
					else
					if (lRight>lLastPosition)
						lLastPosition = lRight;
/////////////////////////////////////////////////////////////////////////
					
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
						{
							M4DELETEV(lLastPositionLevel); 
							return TRUE;
						}
					}
				}
			}
		}
	}
	M4DELETEV(lLastPositionLevel); 
	return TRUE;
}

BOOL CM4Tree32Ctrl::SetRectTreeHorz (void)
{
	long *lLastPositionLevel = new long [m_pTree->M4GetLevelCount ()+2];
	if (lLastPositionLevel == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("SetRectTreeHorz: Ha fallado la reserva de memoria");
		#endif
		return FALSE;
	}
	for (long i=0; i<=m_pTree->M4GetLevelCount (); i++)
		lLastPositionLevel[i] = 0L;

	BOOL bNotIf = FALSE;
	BOOL bExitWhile = FALSE;
	BOOL bExitWhile2 = FALSE;
	long TotalWidth = 0L;
	long lWidth, lHeight;
	long lLeft, lTop, lRight, lBottom;
	long lChildTop, lChildBottom, lNotUsed;
	long lTopAux = 0L;
	long lLastPosition = 0L;

	CM4Node *pNode = m_pTree->M4GetRootNode ();
	CM4Node *pChild, *pSibling, *pParent;
	CM4Node *pChild2, *pSibling2, *pParent2, *pParent3;
	while (pNode)
	{
		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && (pNode->M4GetExpand () == TRUE))
			pNode = pChild;
		else
		{

//////////////////// Establecemos el RECT de un nodo sin hijos //////////
			// El nodo actual no tiene hijos.
			if (m_bCompact == TRUE)
			{
				if (lLastPositionLevel[pNode->M4GetLevel()]>0L)
					// Se coloca el nodo detrás del último nodo creado en ese nivel.
					lTopAux = lLastPositionLevel[pNode->M4GetLevel()]+m_sSiblingDistance;
				else
					// Se coloca el nodo delante de todos.
					lTopAux = 0L;
			}
			else
			{
				if (lLastPosition>0L)
					// Se coloca el nodo detrás del último nodo creado.
					lTopAux = lLastPosition+m_sSiblingDistance;
				else
					// Se coloca el nodo delante de todos.
					lTopAux = 0L;
			}
			pNode->M4GetSize (lWidth, lHeight);
			lTop = lTopAux;
			lBottom = lTop + lHeight;
			TotalWidth=0L;
			for (long i=1L; i<pNode->M4GetLevel (); i++)
				TotalWidth = TotalWidth + m_pMaxWidthLevel[i] + m_sLevelDistance;
			lLeft = TotalWidth;
			lRight = lLeft + lWidth;
			pNode->M4SetRect (lLeft, lTop, lRight, lBottom);
			pNode->M4SetRectChilds (lLeft, lTop, lRight, lBottom);
			if (m_bCompact == TRUE)
				lLastPositionLevel[pNode->M4GetLevel()] = lBottom;
			else
			if (lBottom>lLastPosition)
				lLastPosition = lBottom;
/////////////////////////////////////////////////////////////////////////

			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
				{
					M4DELETEV(lLastPositionLevel); 
					return TRUE;
				}
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{

//////////////////// Establecemos el RECT de un nodo con hijos //////////
					// El nodo actual tiene, al menos, un hijo.
					pParent->M4GetSize (lWidth, lHeight);
					m_pTree->M4SetRectChildNode (pParent);
					if (m_bBalance == TRUE)
						pParent->M4GetRectChilds (lNotUsed, lChildTop, lNotUsed, lChildBottom);
					else
					{
						pParent->M4GetFirstChild ()->M4GetRect (lNotUsed, lChildTop, lNotUsed, lNotUsed);
						pParent->M4GetLastChild ()->M4GetRect (lNotUsed, lNotUsed, lNotUsed, lChildBottom);
					}
					if ((lChildBottom - lChildTop) < lHeight)
					{
						// El nodo actual es más grande que sus hijos..
						if ((m_bCompact == TRUE) && (lLastPositionLevel[pParent->M4GetLevel()]>0L))
						{
							// Los hijos ya están bien colocados.
							// Se ajusta el nodo actual a sus hijos.
							lTopAux = lChildTop-Middle (lHeight-(lChildBottom - lChildTop));
						}
						else
						{
							bNotIf = FALSE;
							if (m_bBalance == FALSE)
							{
								pParent->M4GetRectChilds (lNotUsed, lChildTop, lNotUsed, lChildBottom);
								if ((lChildBottom - lChildTop) >= lHeight)
								{
									// El nodo actual es más pequeño que sus nietos.
									// Se ajusta el nodo actual a sus hijos.
									pParent->M4GetFirstChild ()->M4GetRect (lNotUsed, lChildTop, lNotUsed, lNotUsed);
									pParent->M4GetLastChild ()->M4GetRect (lNotUsed, lNotUsed, lNotUsed, lChildBottom);
									lTopAux = lChildTop+Middle (lChildBottom - lChildTop)-Middle (lHeight);
									bNotIf = TRUE;
								}
							}

							if (bNotIf == FALSE)
							{
								// Se ajustan los hijos al nodo actual.
								pParent2 = pParent->M4GetFirstChild ();
								while (pParent2)
								{
									pParent2->M4GetRect (lLeft, lTop, lRight, lBottom);
									lTop += Middle (lHeight-(lChildBottom - lChildTop));
									lBottom += Middle (lHeight-(lChildBottom - lChildTop));
									pParent2->M4SetRect (lLeft, lTop, lRight, lBottom);
									if (m_bCompact == TRUE)
										lLastPositionLevel[pParent2->M4GetLevel()] = lBottom;
									else
									if (lBottom>lLastPosition)
										lLastPosition = lBottom;

									pChild2 = pParent2->M4GetFirstChild ();
									if ((pChild2) && (pParent2->M4GetExpand () == TRUE))
										pParent2 = pChild2;
									else
									{
										pSibling2 = pParent2->M4GetNextSibling ();
										if (pSibling2)
											pParent2 = pSibling2;
										else
										{
											pParent3 = pParent2->M4GetParent ();
											if ((pParent3 == NULL) || (pParent3 == pParent))
												pParent2 = NULL;
											else
											{
												bExitWhile2 = FALSE;
												while (bExitWhile2 == FALSE)
												{
													pSibling2 = pParent3->M4GetNextSibling ();
													if (pSibling2)
													{
														pParent2 = pSibling2;
														bExitWhile2 = TRUE;
													}
													else
													{
														pParent3 = pParent3->M4GetParent ();
														if ((pParent3 == NULL) || (pParent3 == pParent))
														{
															bExitWhile2 = TRUE;
															pParent2 = NULL;
														}
													}
												}
											}
										}
									}
								}
								lTopAux = lChildTop;
							}
						}
					}
					else
					{
						// El nodo actual es más pequeño que sus hijos.
						// Se ajusta el nodo actual a su hijos.
						lTopAux = lChildTop+Middle (lChildBottom - lChildTop)-Middle (lHeight);
					}
					if ((m_bCompact == TRUE) && (lLastPositionLevel[pParent->M4GetLevel()]>0L) && (lTopAux < (lLastPositionLevel[pParent->M4GetLevel()]+m_sSiblingDistance)))
					{
						// El nodo actual entra en conflicto con el último nodo de ese nivel.
						// Se colocan el nodo actual y sus hijos detrás del último nodo de ese nivel.
						CM4Node *pParent2 = pParent->M4GetFirstChild ();
						while (pParent2)
						{
							pParent2->M4GetRect (lLeft, lTop ,lRight, lBottom);
							lTop += (lLastPositionLevel[pParent->M4GetLevel()]+m_sSiblingDistance-lTopAux);
							lBottom += (lLastPositionLevel[pParent->M4GetLevel()]+m_sSiblingDistance-lTopAux);
							pParent2->M4SetRect (lLeft, lTop, lRight, lBottom);
							lLastPositionLevel[pParent2->M4GetLevel()] = lBottom;
	
							pChild2 = pParent2->M4GetFirstChild ();
							if ((pChild2) && (pParent2->M4GetExpand () == TRUE))
								pParent2 = pChild2;
							else
							{
								pSibling2 = pParent2->M4GetNextSibling ();
								if (pSibling2)
									pParent2 = pSibling2;
								else
								{
									pParent3 = pParent2->M4GetParent ();
									if ((pParent3 == NULL) || (pParent3 == pParent))
										pParent2 = NULL;
									else
									{
										bExitWhile2 = FALSE;
										while (bExitWhile2 == FALSE)
										{
											pSibling2 = pParent3->M4GetNextSibling ();
											if (pSibling2)
											{
												pParent2 = pSibling2;
												bExitWhile2 = TRUE;
											}
											else
											{
												pParent3 = pParent3->M4GetParent ();
												if ((pParent3 == NULL) || (pParent3 == pParent))
												{
													bExitWhile2 = TRUE;
													pParent2 = NULL;
												}
											}
										}
									}
								}
							}
						}
						lTopAux = lLastPositionLevel[pParent->M4GetLevel()]+m_sSiblingDistance;
					}
					lTop = lTopAux;
					lBottom = lTop + lHeight;
					TotalWidth = 0L;
					for (long i=1L; i<pParent->M4GetLevel (); i++)
						TotalWidth = TotalWidth + m_pMaxWidthLevel[i] + m_sLevelDistance;
					lLeft = TotalWidth;
					lRight = lLeft + lWidth;
					pParent->M4SetRect (lLeft, lTop, lRight, lBottom);
					if (m_bCompact == TRUE)
						lLastPositionLevel[pParent->M4GetLevel()] = lBottom;
					else
					if (lBottom>lLastPosition)
						lLastPosition = lBottom;
/////////////////////////////////////////////////////////////////////////
					
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
						{
							M4DELETEV(lLastPositionLevel); 
							return TRUE;
						}
					}
				}
			}
		}
	}
	M4DELETEV(lLastPositionLevel); 
	return TRUE;
}
			
BOOL CM4Tree32Ctrl::SetRectTreeDiag (void)
{
	long lWidth, lHeight;
	long lLeft, lTop, lRight, lBottom;
	long lLeftAux = 0L;
	long lTopAux = 0L;
	long lLastPosition = 0L;
	BOOL bExitWhile = FALSE;

	CM4Node *pNode = m_pTree->M4GetRootNode ();
	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{
		pNode->M4GetSize (lWidth, lHeight);
		lLeftAux = m_sLevelDistance*(pNode->M4GetLevel()-1);
		lTopAux = lLastPosition;
		lLeft = lLeftAux;
		lRight = lLeft + lWidth;
		lTop = lTopAux;
		lBottom = lTop + lHeight;
		pNode->M4SetRect (lLeft, lTop, lRight, lBottom);
		lLastPosition = m_sSiblingDistance+lBottom;

		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && (pNode->M4GetExpand () == TRUE))
			pNode = pChild;
		else
		{
			pNode->M4SetRectChilds (lLeft, lTop, lRight, lBottom);
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
					return TRUE;
				m_pTree->M4SetRectChildNode (pParent);
				bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
							return TRUE;
						m_pTree->M4SetRectChildNode (pParent);
					}
				}
			}
		}
	}
	return TRUE;
}

void CM4Tree32Ctrl::DrawTree (CDC* pdc, CM4Node *a_pNode, CRect &rcBounds)
{
	long lLeft, lTop, lRight, lBottom;

	CM4Node *pNode = a_pNode;
	
	if ((pNode->M4GetFirstChild () == NULL) || (pNode->M4GetExpand () == FALSE))
	{
		pNode->M4GetRect (lLeft, lTop, lRight, lBottom);
		OffsetRect (lLeft, lTop, lRight, lBottom);

		if (!((lLeft>rcBounds.right) || (lRight<rcBounds.left) || (lTop>rcBounds.bottom) || (lBottom<rcBounds.top)))
		{
			if (m_lNodeDown == pNode->M4GetNodeID ())
				DrawNodeDown (pdc, pNode);
			else
				DrawNode (pdc, pNode);
		}
		return;
	}

	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{
		if ((pNode != a_pNode) && (m_sLinkType<=M4TREE_LINK_DOT))
			DrawLink(pdc, pNode);

		pNode->M4GetRect (lLeft, lTop, lRight, lBottom);
		OffsetRect (lLeft, lTop, lRight, lBottom);

		if (!((lLeft>rcBounds.right) || (lRight<rcBounds.left) || (lTop>rcBounds.bottom) || (lBottom<rcBounds.top)))
		{
			if (m_lNodeDown == pNode->M4GetNodeID ())
				DrawNodeDown (pdc, pNode);
			else
				DrawNode (pdc, pNode);
		}

		pChild = pNode->M4GetFirstChild ();

		pNode->M4GetRectTotal (lLeft, lTop, lRight, lBottom);
		OffsetRect (lLeft, lTop, lRight, lBottom);

		if ((!((lLeft>rcBounds.right) || (lRight<rcBounds.left) || (lTop>rcBounds.bottom) || (lBottom<rcBounds.top))) && ((pChild) && (pNode->M4GetExpand () == TRUE)))
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if ((pParent == NULL) || (pParent == a_pNode))
					return;
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if ((pParent == NULL) || (pParent == a_pNode))
							return;
					}
				}
			}
		}
	}
}

void CM4Tree32Ctrl::DrawLink(CDC* pdc, CM4Node *a_pNode)
{
	int cxp, cyp, cxh, cyh;
	CM4Node *pParentNode = a_pNode->M4GetParent ();

	long lLeft, lTop, lRight, lBottom;
	a_pNode->M4GetRect (lLeft, lTop, lRight, lBottom);
	OffsetRect (lLeft, lTop, lRight, lBottom);

	long lParentLeft, lParentTop, lParentRight, lParentBottom;
	pParentNode->M4GetRect (lParentLeft, lParentTop, lParentRight, lParentBottom);
	OffsetRect (lParentLeft, lParentTop, lParentRight, lParentBottom);
	
	lLeft = min (lLeft, INT_MAX);
	lRight = min (lRight, INT_MAX);
	lTop = min (lTop, INT_MAX);
	lBottom = min (lBottom, INT_MAX);
	lParentLeft = min (lParentLeft, INT_MAX);
	lParentRight = min (lParentRight, INT_MAX);
	lParentTop = min (lParentTop, INT_MAX);
	lParentBottom = min (lParentBottom, INT_MAX);
	
	lLeft = max (lLeft, INT_MIN);
	lRight = max (lRight, INT_MIN);
	lTop = max (lTop, INT_MIN);
	lBottom = max (lBottom, INT_MIN);
	lParentLeft = max (lParentLeft, INT_MIN);
	lParentRight = max (lParentRight, INT_MIN);
	lParentTop = max (lParentTop, INT_MIN);
	lParentBottom = max (lParentBottom, INT_MIN);
	
	int iLinkType = m_sLinkType;
	if (m_sLinkType != M4TREE_DEFAULT_PROPERTY)
		iLinkType--;

//	CPen *pPen = new CPen ((int) iLinkType, (int) (m_sLinkWidth * m_dScale), (COLORREF) GetValueLong (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_FC));
		CPen *pPen = new CPen ((int) iLinkType, (int) (m_sLinkWidth * m_dScale), TranslateColor(GetForeColor()));
	if (pPen == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("DrawLink: Ha fallado la reserva de memoria");
		#endif
		return;
	}

	CPen *oldPen = pdc->SelectObject (pPen);
	pdc->SetBkMode(TRANSPARENT);

	switch (m_sTreeStyle)
	{
	default:
	case M4TREE_DEFAULT_PROPERTY:
	case M4TREE_TREE_VERT:
		cxh = (int) (lLeft+Middle (lRight-lLeft));
		cyh = (int) lTop;
		if (pParentNode->M4GetNumberChildren () == 1)
			cxp = cxh;
		else
			cxp = (int) (lParentLeft+Middle (lParentRight-lParentLeft));
		cyp = (int) lParentBottom;
		pdc->MoveTo(cxp, cyp);
		if (m_sLinkStyle != M4TREE_LINK_DIRECT)
		{
			pdc->LineTo(cxp, (int) (cyh-(((double) Middle (m_sLevelDistance))*m_dScale)));
			pdc->LineTo(cxh, (int) (cyh-(((double) Middle (m_sLevelDistance))*m_dScale)));
		}
		if (a_pNode->M4GetNodeID ())
			pdc->LineTo(cxh, cyh);
		else
			pdc->LineTo(cxh, (int) lBottom);
		break;
	case M4TREE_TREE_HORZ:
		cxh = (int) lLeft;
		cyh = (int) (lTop+Middle (lBottom-lTop));
		cxp = (int) lParentRight;
		if (pParentNode->M4GetNumberChildren () == 1)
			cyp = cyh;
		else
			cyp = (int) (lParentTop+Middle (lParentBottom-lParentTop));
		pdc->MoveTo(cxp, cyp);
		if (m_sLinkStyle != M4TREE_LINK_DIRECT)
		{
			pdc->LineTo((int) (cxh-(((double) Middle (m_sLevelDistance))*m_dScale)), cyp);
			pdc->LineTo((int) (cxh-(((double) Middle (m_sLevelDistance))*m_dScale)), cyh);
		}
		if (a_pNode->M4GetNodeID ())
			pdc->LineTo(cxh, cyh);
		else
			pdc->LineTo((int) lRight, cyh);
		break;
	case M4TREE_TREE_DIAG:
		cxh = (int) lLeft;
		cyh = (int) (lTop+Middle (lBottom-lTop));
		cxp = (int) (lParentLeft+(Middle (m_sLevelDistance)*m_dScale));
		cyp = (int) lParentBottom;
		pdc->MoveTo(cxp, cyp);
		if (m_sLinkStyle != M4TREE_LINK_DIRECT)
			pdc->LineTo(cxp, cyh);
		pdc->LineTo(cxh, cyh);
		break;
	}

	pdc->SelectObject (oldPen);
	DeleteObject (pPen->m_hObject);
	M4DELETE(pPen);
}

void CM4Tree32Ctrl::DrawNode (CDC* pdc, CM4Node *a_pNode)
{
	if (a_pNode->M4GetNodeID ()==0)	return;

	long lLeft, lTop, lRight, lBottom;
	a_pNode->M4GetRect (lLeft, lTop, lRight, lBottom);
	CRect rcNode (0, 0, (int) (lRight-lLeft), (int) (lBottom-lTop));
	CRect rcBorder = rcNode;
	OffsetRect (lLeft, lTop, lRight, lBottom);

	if ((a_pNode == m_pTree->M4GetCurrentNode ()) && (m_bPrinting == FALSE))
	{
		CRect rcSelect;
		rcSelect.left = (int) (lLeft-1);
		rcSelect.top = (int) (lTop-1);
		rcSelect.right = (int) (lRight+1);
		rcSelect.bottom = (int) (lBottom+1);
		Draw3dRect (pdc, rcSelect, RGB(127,127,127), RGB(127,127,127));
	}
	if (m_sAppearance == M4TREE_TREE_SHADOW)
		FillSolidRect(pdc, (int) (lLeft+(4*m_dScale)), (int) (lTop+(4*m_dScale)), (int) (lRight-lLeft), (int) (lBottom-lTop), RGB(127,127,127));

	CDC *pDCMen = new CDC;
	if (pDCMen == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("DrawNode: Ha fallado la reserva de memoria");
		#endif
		return;
	}
	pDCMen->CreateCompatibleDC (pdc);

	CBitmap *pBitmap = new CBitmap;
	if (pBitmap == NULL)
	{
		M4DELETE(pDCMen);
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("DrawNode: Ha fallado la reserva de memoria");
		#endif
		return;
	}
	pBitmap->CreateCompatibleBitmap (pdc, rcNode.right, rcNode.bottom);
	CBitmap *oldBitmap = pDCMen->SelectObject (pBitmap);

	COLORREF cBackColor = (COLORREF) GetValueLong (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_BC);
	FillSolidRect(pDCMen, rcNode.left, rcNode.top, rcNode.Width (), rcNode.Height (), cBackColor);

	switch (m_sAppearance)
	{
	default:
	case M4TREE_DEFAULT_PROPERTY:
	case M4TREE_TREE_FLAT:
		Draw3dRect (pDCMen, rcNode, RGB(0,0,0), RGB(0,0,0));
		break;
	case M4TREE_TREE_3D:
		rcBorder.left++;
		rcBorder.top++;
		rcBorder.right--;
		rcBorder.bottom--;
		Draw3dRect (pDCMen, rcBorder, RGB(255,255,255), RGB(127,127,127));
		Draw3dRect (pDCMen, rcNode, RGB(0,0,0), RGB(0,0,0));
		break;
	case M4TREE_TREE_SHADOW:
		FillSolidRect(pDCMen, (int) rcNode.left, (int) rcNode.top, rcNode.Width (), rcNode.Height (), cBackColor);
		Draw3dRect (pDCMen, rcNode, RGB(0,0,0), RGB(0,0,0));
		break;
	case M4TREE_NO_BORDER:
		break;
	}

	CString csVisible;
	CString csControl;
	if (a_pNode->M4GetFirstChild ())
	{
		if (a_pNode->M4GetExpand ())
		{
			csVisible = GetValueString (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVO);
		}
		else
		{
			csVisible = GetValueString (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVC);
		}
	}
	else
	{
		csVisible = GetValueString (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVN);
	}
	while (csVisible.IsEmpty () == FALSE)
	{
		int nIndex = csVisible.Find (',');
		if (nIndex > 0)
		{
			csControl = csVisible.Left (nIndex);
			csVisible = ((LPCTSTR) csVisible)+nIndex+1;
		}
		else
		{
			csControl = csVisible;
			csVisible.Empty ();
		}
		switch (csControl[0])
		{
		case M4TREE_CONTROL_P:
			DrawBitmap (pDCMen, a_pNode, csControl);
			break;
		case M4TREE_CONTROL_T:
			DrawText (pDCMen, a_pNode, csControl);
			break;
		}
	}

	pdc->StretchBlt ((int) lLeft, (int) lTop, (int) (lRight-lLeft), (int) (lBottom-lTop), pDCMen, 0, 0, rcNode.right, rcNode.bottom, SRCCOPY);

	pDCMen->SelectObject (oldBitmap);
	DeleteObject (pBitmap->m_hObject);
	M4DELETE(pBitmap);

	DeleteDC (pDCMen->m_hDC);
	M4DELETE(pDCMen);
}

void CM4Tree32Ctrl::DrawNodeDown (CDC* pdc, CM4Node *a_pNode)
{
	if (a_pNode->M4GetNodeID ()==0)	return;

	long lLeft, lTop, lRight, lBottom;
	a_pNode->M4GetRect (lLeft, lTop, lRight, lBottom);
	CRect rcNode (0, 0, (int) (lRight-lLeft), (int) (lBottom-lTop));
	CRect rcBorder = rcNode;
	OffsetRect (lLeft, lTop, lRight, lBottom);

	CDC *pDCMen = new CDC;
	if (pDCMen == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("DrawNodeDown: Ha fallado la reserva de memoria");
		#endif
		return;
	}
	pDCMen->CreateCompatibleDC (pdc);

	CBitmap *pBitmap = new CBitmap;
	if (pBitmap == NULL)
	{
		M4DELETE(pDCMen);
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("DrawNodeDown: Ha fallado la reserva de memoria");
		#endif
		return;
	}
	pBitmap->CreateCompatibleBitmap (pdc, rcNode.right, rcNode.bottom);
	CBitmap *oldBitmap = pDCMen->SelectObject (pBitmap);

	COLORREF cBackColor = (COLORREF) GetValueLong (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_BC);
	FillSolidRect(pDCMen, rcNode.left, rcNode.top, rcNode.Width (), rcNode.Height (), cBackColor);

	rcBorder.left++;
	rcBorder.top++;
	rcBorder.right--;
	rcBorder.bottom--;
	Draw3dRect (pDCMen, rcBorder, RGB(127,127,127), RGB(255,255,255));
	Draw3dRect (pDCMen, rcNode, RGB(0,0,0), RGB(0,0,0));

	CString csVisible;
	CString csControl;
	if (a_pNode->M4GetFirstChild ())
	{
		if (a_pNode->M4GetExpand ())
		{
			csVisible = GetValueString (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVO);
		}
		else
		{
			csVisible = GetValueString (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVC);
		}
	}
	else
	{
		csVisible = GetValueString (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVN);
	}
	while (csVisible.IsEmpty () == FALSE)
	{
		int nIndex = csVisible.Find (',');
		if (nIndex > 0)
		{
			csControl = csVisible.Left (nIndex);
			csVisible = ((LPCTSTR) csVisible)+nIndex+1;
		}
		else
		{
			csControl = csVisible;
			csVisible.Empty ();
		}
		switch (csControl[0])
		{
		case M4TREE_CONTROL_P:
			DrawBitmap (pDCMen, a_pNode, csControl);
			break;
		case M4TREE_CONTROL_T:
			DrawText (pDCMen, a_pNode, csControl);
			break;
		}
	}

	pdc->StretchBlt ((int) (lLeft+1), (int) (lTop+1), (int) (lRight-lLeft), (int) (lBottom-lTop), pDCMen, 0, 0, rcNode.right, rcNode.bottom, SRCCOPY);

	pDCMen->SelectObject (oldBitmap);
	DeleteObject (pBitmap->m_hObject);
	M4DELETE(pBitmap);

	DeleteDC (pDCMen->m_hDC);
	M4DELETE(pDCMen);
}

void CM4Tree32Ctrl::DrawBitmap (CDC* pdc, CM4Node *a_pNode, LPCTSTR a_szControl)
{
	CDC *pDCMen = new CDC;
	if (pDCMen == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("DrawBitmap: Ha fallado la reserva de memoria");
		#endif
		return;
	}
	pDCMen->CreateCompatibleDC (pdc);

	BOOL bFile = FALSE;
	CBitmap *pBitmap;
	CString csBitmap = GetValueString (a_pNode, a_szControl, M4TREE_PROP_BT);

	long lIndex = 0L;
	BOOL bFound = FALSE;
	if (m_bStoreBitmap == TRUE)
	{
		bFound = m_pBitmp->M4Find (csBitmap, lIndex);
	}
	if (bFound == FALSE)
	{
		BOOL bDeleteBitmap = TRUE;
	 	pBitmap = new CBitmap;
		if (pBitmap == NULL)
		{
			M4DELETE(pDCMen);
			m_lError = M4TREE_NO_MEMORY;
			#ifdef _DEBUG
				// AfxMessageBox ("DrawBitmap: Ha fallado la reserva de memoria");
			#endif
			return;
		}
		UINT hBitmap = (UINT) GetValueLong (a_pNode, a_szControl, M4TREE_PROP_BT);
		if (hBitmap == 0L)
		{
			if (pBitmap->LoadBitmap (csBitmap) == FALSE)
			{
				bFile = TRUE;
				M4DELETE(pBitmap);
				bDeleteBitmap = FALSE;
				HBITMAP hBitmap = ReadBitmap (pdc, csBitmap);
				if (hBitmap == 0)
				{
					M4DELETE(pDCMen);
					return;
				}
				pBitmap = CBitmap::FromHandle (hBitmap);
			}
		}
		else
		{
			if (pBitmap->LoadBitmap (hBitmap) == FALSE)
			{
				M4DELETE(pDCMen);
				M4DELETE(pBitmap);
				return;
			}
		}
		if (m_bStoreBitmap == TRUE)
			m_pBitmp->M4Insert (csBitmap, pBitmap, bDeleteBitmap);
	}
	else
	{
		pBitmap = m_pBitmp->M4GetBitmap (lIndex);
		if (pBitmap == NULL)
		{
			M4DELETE(pDCMen);
			return;
		}
	}
    CBitmap *oldBitmap = pDCMen->SelectObject (pBitmap);

	long cx = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_CX);
	long cy = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_CY);
	if ((cx == 0L) || (cy == 0L))
	{
		if (m_bStoreBitmap == TRUE)
		{
			cx = m_pBitmp->M4GetWidthBitmap (lIndex);
			cy = m_pBitmp->M4GetHeightBitmap (lIndex);
		}
		else
		{
			BITMAP bm;
			pBitmap->GetObject (sizeof(bm), &bm);
			if (cx == 0L)
				cx = (long) bm.bmWidth;
			if (cy == 0L)
				cy = (long) bm.bmHeight;
		}
	}

	CBrush *pBrush = new CBrush;
	if (pBrush == NULL)
	{
		M4DELETE(pBitmap);
		M4DELETE(pDCMen);
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("DrawBitmap: Ha fallado la reserva de memoria");
		#endif
		return;
	}
	pBrush->CreateSolidBrush ((COLORREF) GetValueLong (a_pNode, M4TREE_CONTROL_A, M4TREE_PROP_BC));
	CBrush *oldBrush = pDCMen->SelectObject (pBrush);

	pDCMen->ExtFloodFill (0, 0, RGB (255, 255, 255), FLOODFILLSURFACE);

	pdc->BitBlt ((int) GetValueLong (a_pNode, a_szControl, M4TREE_PROP_X), (int) (GetValueLong (a_pNode, a_szControl, M4TREE_PROP_Y)), (int) cx, (int) cy, pDCMen, 0, 0, SRCCOPY );

	pDCMen->SelectObject (oldBrush);
	DeleteObject (pBrush->m_hObject);
	M4DELETE(pBrush);

	pDCMen->SelectObject (oldBitmap);
	if (m_bStoreBitmap == FALSE)
	{
		DeleteObject (pBitmap->m_hObject);
		if (bFile == FALSE)
		{
			M4DELETE(pBitmap);
		}
	}

	DeleteDC (pDCMen->m_hDC);
	M4DELETE(pDCMen);
}

void CM4Tree32Ctrl::DrawText (CDC* pdc, CM4Node *a_pNode, LPCTSTR a_szControl)
{
	CString csTitle (GetValueString (a_pNode, a_szControl, M4TREE_PROP_TT));
	if (csTitle.IsEmpty () == TRUE)
		return;

	long lFontSize = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_FS);
	long lFontWeight = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_FW);
	long lFontItalic = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_FI);
	long lFontUnder = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_FU);
	long lFontStrike = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_FK);
	CString csFontName (GetValueString (a_pNode, a_szControl, M4TREE_PROP_FN));

	CFont* pFont = new CFont;
	if (pFont == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("DrawText: Ha fallado la reserva de memoria");
		#endif
		return;
	}
	pFont->CreateFont((int) lFontSize, 0, 0, 0, (int) lFontWeight, (BYTE) lFontItalic, (BYTE) lFontUnder, (BYTE) lFontStrike, 0, 0, 0, 0, 0, csFontName);
	CFont* oldFont = pdc->SelectObject (pFont);

	pdc->SetTextColor((COLORREF) GetValueLong (a_pNode, a_szControl, M4TREE_PROP_FC));
	pdc->SetBkMode(TRANSPARENT);

	long cx = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_CX);
	long cy = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_CY);
	if ((cx == 0L) || (cy == 0L))
	{
		CSize cDimText = pdc->GetTextExtent(csTitle, csTitle.GetLength ());
		if (cx == 0L)
			cx = (long) cDimText.cx;
		if (cy == 0L)
			cy = (long) cDimText.cy;
	}

	long x = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_X);
	long y = GetValueLong (a_pNode, a_szControl, M4TREE_PROP_Y);
	CRect rcText ((int) x, (int) y, (int) (x+cx), (int) (y+cy));
	
	pdc->ExtTextOut (rcText.left, rcText.top, ETO_CLIPPED, &rcText, csTitle, csTitle.GetLength (), NULL);

	pdc->SelectObject (oldFont);
	DeleteObject (pFont->m_hObject);
	M4DELETE(pFont);
}

void CM4Tree32Ctrl::SizeNode (CDC* pdc)
{
	M4DELETEV(m_pMaxWidthLevel);
	M4DELETEV(m_pMaxHeightLevel);

	if (m_pBitmp)
	{
		for (long i=0L; i<m_pBitmp->M4GetCount (); i++)
		{
			CBitmap *pBitmap = m_pBitmp->M4GetBitmap (i);
			if (pBitmap)
			{
				DeleteObject (pBitmap->m_hObject);
				if (m_pBitmp->M4GetDeleteBitmap (i) == TRUE)
					delete pBitmap;
				pBitmap = NULL;
			}
		}
		m_pBitmp->M4DeleteAll ();
	}
		
	m_pMaxWidthLevel = new long [m_pTree->M4GetLevelCount ()+1];
	if (m_pMaxWidthLevel == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("SizeNode: Ha fallado la reserva de memoria");
		#endif
		return;
	}
	m_pMaxHeightLevel = new long [m_pTree->M4GetLevelCount ()+1];
	if (m_pMaxHeightLevel == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("SizeNode: Ha fallado la reserva de memoria");
		#endif
		return;
	}

	for (long i=0; i<m_pTree->M4GetLevelCount ()+1; i++)
	{
		m_pMaxWidthLevel[i] = 0L;
		m_pMaxHeightLevel[i] = 0L;
	}

	CM4Node *pNode = m_pTree->M4GetRootNode ();
	CM4Node *pChild, *pSibling, *pParent;

	while (pNode)
	{
		long acx, acy;
		if ((pNode->M4GetValid ()==FALSE) || (m_bInvalidateAll == TRUE))
		{
			acx = GetValueLong (pNode, M4TREE_CONTROL_A, M4TREE_PROP_CX);
			acy = GetValueLong (pNode, M4TREE_CONTROL_A, M4TREE_PROP_CY);
			
			if ((acx == 0L) || (acy == 0L))
			{
				long cxmn = INT_MAX;
				long cxmx = 0L;
				long cymn = INT_MAX;
				long cymx = 0L;
			
				CString csControl, csVisible;
				if (pNode->M4GetFirstChild ())
				{
					if (pNode->M4GetExpand ())
					{
						csVisible = GetValueString (pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVO);
					}
					else
					{
						csVisible = GetValueString (pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVC);
					}
				}
				else
				{
					csVisible = GetValueString (pNode, M4TREE_CONTROL_A, M4TREE_PROP_CVN);
				}
				if (csVisible.IsEmpty () == TRUE)
				{
					acx = 10;
					acy = 10;
				}
				while (csVisible.IsEmpty () == FALSE)
				{
					int nIndex = csVisible.Find (',');
					if (nIndex > 0)
					{
						csControl = csVisible.Left (nIndex);
						csVisible = ((LPCTSTR) csVisible)+nIndex+1;
					}
					else
					{
						csControl = csVisible;
						csVisible.Empty ();
					}
					switch (csControl[0])
					{
					case M4TREE_CONTROL_P:
						{
							BOOL bFile = FALSE;
							long cx = GetValueLong (pNode, csControl, M4TREE_PROP_CX);
							long cy = GetValueLong (pNode, csControl, M4TREE_PROP_CY);
							if ((cx == 0L) || (cy == 0L))
							{
								CBitmap *pBitmap;
								CString csBitmap = GetValueString (pNode, csControl, M4TREE_PROP_BT);
						
								long lIndex = 0L;
								BOOL bFound = FALSE;
								if (m_bStoreBitmap == TRUE)
								{
									bFound = m_pBitmp->M4Find (csBitmap, lIndex);
								}

								if (bFound == FALSE)
								{
									BOOL bDeleteBitmap = TRUE;
	 								pBitmap = new CBitmap;
									if (pBitmap == NULL)
									{
										m_lError = M4TREE_NO_MEMORY;
										#ifdef _DEBUG
											// AfxMessageBox ("SizeNode: Ha fallado la reserva de memoria");
										#endif
										return;
									}
									UINT hBitmap = (UINT) GetValueLong (pNode, csControl, M4TREE_PROP_BT);
									if (hBitmap == 0L)
									{
										if (pBitmap->LoadBitmap (csBitmap) == FALSE)
										{
											bFile = TRUE;
											M4DELETE(pBitmap);
											bDeleteBitmap = FALSE;
											HBITMAP hBitmap = ReadBitmap (pdc, csBitmap);
											if (hBitmap == 0)
												break;
											pBitmap = CBitmap::FromHandle (hBitmap);
										}
									}
									else
									{
										if (pBitmap->LoadBitmap (hBitmap) == FALSE)
										{
											M4DELETE(pBitmap);
											break;
										}
									}
									if (m_bStoreBitmap == TRUE)
										m_pBitmp->M4Insert (csBitmap, pBitmap, bDeleteBitmap);
								}
								else
								{
									pBitmap = m_pBitmp->M4GetBitmap (lIndex);
									if (pBitmap == NULL)
									{
										break;
									}
								}

								if (m_bStoreBitmap == TRUE)
								{
									cx = m_pBitmp->M4GetWidthBitmap (lIndex);
									cy = m_pBitmp->M4GetHeightBitmap (lIndex);
								}
								else
								{
									BITMAP bm;
									pBitmap->GetObject (sizeof(bm), &bm);
									if (cx == 0L)
										cx = (long) bm.bmWidth;
									if (cy == 0L)
										cy = (long) bm.bmHeight;
									DeleteObject (pBitmap->m_hObject);
									if (bFile == FALSE)
									{
										M4DELETE(pBitmap);
									}
								}
							}

							long x = GetValueLong (pNode, csControl, M4TREE_PROP_X);
							long y = GetValueLong (pNode, csControl, M4TREE_PROP_Y);
							if (cxmn > x)
								cxmn = x;
							if (cxmx < x + cx)
								cxmx = x + cx;
							if (cymn > y)
								cymn = y;
							if (cymx < y + cy)
								cymx = y + cy;
						}
						break;
					case M4TREE_CONTROL_T:
						{
							BOOL bText = TRUE;
							long cx = GetValueLong (pNode, csControl, M4TREE_PROP_CX);
							long cy = GetValueLong (pNode, csControl, M4TREE_PROP_CY);
							if ((cx == 0L) || (cy == 0L))
							{
								CString csTitle (GetValueString (pNode, csControl, M4TREE_PROP_TT));
								if (csTitle.IsEmpty () == TRUE)
								{
									bText = FALSE;
									csTitle = "Error";
								}

								long lFontSize = GetValueLong (pNode, csControl, M4TREE_PROP_FS);
								long lFontWeight = GetValueLong (pNode, csControl, M4TREE_PROP_FW);
								long lFontItalic = GetValueLong (pNode, csControl, M4TREE_PROP_FI);
								long lFontUnder = GetValueLong (pNode, csControl, M4TREE_PROP_FU);
								long lFontStrike = GetValueLong (pNode, csControl, M4TREE_PROP_FK);
								CString csFontName (GetValueString (pNode, csControl, M4TREE_PROP_FN));

								CFont* pFont = new CFont;
								if (pFont == NULL)
								{
									m_lError = M4TREE_NO_MEMORY;
									#ifdef _DEBUG
										// AfxMessageBox ("SizeNode: Ha fallado la reserva de memoria");
									#endif
									return;
								}
								pFont->CreateFont((int) lFontSize, 0, 0, 0, (int) lFontWeight, (BYTE) lFontItalic, (BYTE) lFontUnder, (BYTE) lFontStrike, 0, 0, 0, 0, 0, csFontName);
								CFont* oldFont = pdc->SelectObject (pFont);

								CSize cDimText = pdc->GetTextExtent(csTitle, csTitle.GetLength ());
								if (cx == 0L)
									cx = (long) cDimText.cx;
								if (cy == 0L)
									cy = (long) cDimText.cy;
								pdc->SelectObject (oldFont);
								DeleteObject (pFont->m_hObject);
								M4DELETE(pFont);
							}

							long x = GetValueLong (pNode, csControl, M4TREE_PROP_X);
							long y = GetValueLong (pNode, csControl, M4TREE_PROP_Y);
							if (bText == TRUE)
							{
								if (cxmn > x)
									cxmn = x;
								if (cxmx < x + cx)
									cxmx = x + cx;
								if (cymn > y)
									cymn = y;
								if (cymx < y + cy)
									cymx = y + cy;
							}
						}
						break;
					}
				}
				if (acx == 0L)
					acx = cxmx + cxmn;
				if (acx >= INT_MAX)
					acx = 10;
				if (acy == 0L)
					acy = cymx + cymn;
				if (acy >= INT_MAX)
					acy = 10;
			}
			pNode->M4SetSize (acx, acy);
			pNode->M4SetValid (TRUE);
		}
		else
		{
			pNode->M4GetSize (acx, acy);
		}

		if (m_pMaxWidthLevel[pNode->M4GetLevel ()] < acx)
			m_pMaxWidthLevel[pNode->M4GetLevel ()] = acx;
		if (m_pMaxHeightLevel[pNode->M4GetLevel ()] < acy)
			m_pMaxHeightLevel[pNode->M4GetLevel ()] = acy;

		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && (pNode->M4GetExpand () == TRUE))
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
				{
					m_bInvalidateAll = FALSE;
					return;
				}
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
						{
							m_bInvalidateAll = FALSE;
							return;
						}
					}
				}
			}
		}
	}
	m_bInvalidateAll = FALSE;
}

long CM4Tree32Ctrl::GetValueLong (CM4Node *a_pNode, LPCTSTR a_szProperty)
{
	long lIndex = 0L;
	long lIndex2 = 0L;
	if (a_pNode->M4GetLocalProperty ()->M4FindProperty (a_szProperty, lIndex) == TRUE)
	{
		return a_pNode->M4GetLocalProperty ()->M4GetValueLong (lIndex);
	}
	CString csPropST;
	csPropST = M4TREE_CONTROL_A; csPropST += M4TREE_PROP_ST;
	if (a_pNode->M4GetLocalProperty ()->M4FindProperty (csPropST, lIndex) == TRUE)
	{
		long lIndex2 = a_pNode->M4GetLocalProperty ()->M4GetValueLong (lIndex);
		long lIndex3 = 0L;
		if (m_pStyle[lIndex2])
		{
			if (m_pStyle[lIndex2]->M4FindProperty (a_szProperty, lIndex3) == TRUE)
			{
				return m_pStyle[lIndex2]->M4GetValueLong (lIndex3);
			}
		}
	}
	if (m_pStyle[0]->M4FindProperty (a_szProperty, lIndex) == TRUE)
	{
		return m_pStyle[0]->M4GetValueLong (lIndex);
	}
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "La propiedad %s no existe en el nodo %ld", a_szProperty, a_pNode->M4GetNodeID ());
		// AfxMessageBox (szWarning);
	#endif
	return 0L;
}

LPCTSTR CM4Tree32Ctrl::GetValueString (CM4Node *a_pNode, LPCTSTR a_szProperty)
{
	long lIndex = 0L;
	if (a_pNode->M4GetLocalProperty ()->M4FindProperty (a_szProperty, lIndex) == TRUE)
	{
		return a_pNode->M4GetLocalProperty ()->M4GetValueString (lIndex);
	}
	CString csPropST;
	csPropST = M4TREE_CONTROL_A; csPropST += M4TREE_PROP_ST;
	if (a_pNode->M4GetLocalProperty ()->M4FindProperty (csPropST, lIndex) == TRUE)
	{
		long lIndex2 = a_pNode->M4GetLocalProperty ()->M4GetValueLong (lIndex);
		long lIndex3 = 0L;
		if (m_pStyle[lIndex2])
		{
			if (m_pStyle[lIndex2]->M4FindProperty (a_szProperty, lIndex3) == TRUE)
			{
				return m_pStyle[lIndex2]->M4GetValueString (lIndex3);
			}
		}
	}
	if (m_pStyle[0]->M4FindProperty (a_szProperty, lIndex) == TRUE)
	{
		return m_pStyle[0]->M4GetValueString (lIndex);
	}
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "La propiedad %s no existe en el nodo % ld", a_szProperty, a_pNode->M4GetNodeID ());
		// AfxMessageBox (szWarning);
	#endif
	return 0L;
}

HBITMAP CM4Tree32Ctrl::ReadBitmap (CDC* pdc, LPCTSTR a_szPath)
{
	CFile* pFile = new CFile (a_szPath, CFile::modeRead | CFile::typeBinary);
	if (pFile == NULL)
	{
		m_lError = M4TREE_NO_FILE;
		#ifdef _DEBUG
			// AfxMessageBox ("ReadBitmap: El fichero no exite");
		#endif
		return 0;
	}
	HBITMAP hBitmap;
	BITMAPFILEHEADER stBFileHeader;
	BITMAPINFOHEADER stBInfoHeader;
	LPBYTE lpByte;
	PBITMAPINFO pBInfo;

	pFile->Read(&stBFileHeader, sizeof(BITMAPFILEHEADER));
	pFile->Read(&stBInfoHeader, sizeof (BITMAPINFOHEADER));
 
	pBInfo = (PBITMAPINFO) new char[sizeof(BITMAPINFOHEADER) + ((1<<stBInfoHeader.biBitCount) * sizeof(RGBQUAD))];
	if (pBInfo == NULL)
	{
		M4DELETE(pFile);
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("ReadBitmap: Ha fallado la reserva de memoria");
		#endif
		return 0;
	}
	pBInfo->bmiHeader.biSize = stBInfoHeader.biSize; 
	pBInfo->bmiHeader.biWidth = stBInfoHeader.biWidth; 
	pBInfo->bmiHeader.biHeight = stBInfoHeader.biHeight; 
	pBInfo->bmiHeader.biPlanes = stBInfoHeader.biPlanes; 
	pBInfo->bmiHeader.biBitCount = stBInfoHeader.biBitCount; 
	pBInfo->bmiHeader.biCompression = stBInfoHeader.biCompression; 
	pBInfo->bmiHeader.biSizeImage = stBInfoHeader.biSizeImage; 
	pBInfo->bmiHeader.biXPelsPerMeter = stBInfoHeader.biXPelsPerMeter; 
	pBInfo->bmiHeader.biYPelsPerMeter = stBInfoHeader.biYPelsPerMeter; 
	pBInfo->bmiHeader.biClrUsed = stBInfoHeader.biClrUsed; 
	pBInfo->bmiHeader.biClrImportant = stBInfoHeader.biClrImportant; 
 
	pFile->Read(pBInfo->bmiColors, (sizeof(RGBQUAD) * (1<<stBInfoHeader.biBitCount)));
	lpByte = (LPBYTE) new char[(stBFileHeader.bfSize - stBFileHeader.bfOffBits)];
	if (lpByte == NULL)
	{
		M4DELETE(pBInfo);
		M4DELETE(pFile);
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("ReadBitmap: Ha fallado la reserva de memoria");
		#endif
		return 0;
	}
	pFile->Read(lpByte, (UINT) (stBFileHeader.bfSize - stBFileHeader.bfOffBits));
 
	hBitmap = CreateDIBitmap(pdc->m_hDC, &stBInfoHeader, CBM_INIT, lpByte, pBInfo, DIB_RGB_COLORS); 

	M4DELETE(pBInfo);
	M4DELETE(lpByte);
	pFile->Close ();
	M4DELETE(pFile);
	return hBitmap;
}

///////////////////////// Metodos /////////////////////////

/***********************************************************************************
Función:
	long M4InsertNode (long a_lParentID)
***********************************************************************************
Descripción:
	Inserta un nuevo nodo en el árbol.
Parámetros:
	a_szTitle -> Título del nodo.
	a_lParentID -> Identificador del nodo padre.
Valor de retorno:
	Identificador del nodo o cero si no se realiza la insersión.
Comentarios:
	Si el árbol no existe, lo crea.
	Si a_lParentID es cero, se toma como padre al nodo actual.
***********************************************************************************/
long CM4Tree32Ctrl::M4InsertNode(long a_lParentID) 
{
	if (m_pTree == NULL)
		m_pTree = new CM4Tree ();
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("M4InsertNode: Ha fallado la reserva de memoria");
		#endif
		return 0;
	}

	long lCurrentNode = m_pTree->M4InsertNode(a_lParentID);
	if (lCurrentNode == 0L)
	{
		m_lError = M4TREE_NO_NODE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4InsertNode: Ha fallado la creación de un nuevo nodo");
		#endif
		return 0L;
	}
	CM4Node *pParent = m_pTree->M4FindNode (a_lParentID);
	pParent->M4SetValid (FALSE);
	m_bResize = TRUE;
	InvalidateControl();
	return lCurrentNode;
}

/***********************************************************************************
Función:
	BOOL M4DeleteNode (long a_lNodeID)
***********************************************************************************
Descripción:
	Borra un nodo y sus hijos del árbol.
Parámetros:
	a_lNodeID -> Identificador del nodo.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Si el ID es el del nodo raiz, se destruye el árbol completamente.
**********************************************************************************/
BOOL CM4Tree32Ctrl::M4DeleteNode (long a_lNodeID) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4DeleteNode: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	CM4Node *pParent = pNode->M4GetParent ();
	if (m_pTree->M4DeleteNode (a_lNodeID) == FALSE)
	{
		m_lError = M4TREE_NO_NODE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4DeleteNode: El nodo %ld no existe", a_lNodeID);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	if (m_pTree->M4GetRootNode () == NULL)
	{
		M4DELETE(m_pTree);
	}
	pParent->M4SetValid (FALSE);
	m_bResize = TRUE;
	InvalidateControl();
	return TRUE;
}

/***********************************************************************************
Función:
	long M4GetLastError ()
***********************************************************************************
Descripción:
	Retorna el último error ocurrido en el árbol.
Parámetros:
	Ninguno.
Valor de retorno:
	Identificador del error.
***********************************************************************************/
long CM4Tree32Ctrl::M4GetLastError () 
{
	long lError = m_lError;
	m_lError = M4TREE_NO_ERROR;
	return lError;
}

/***********************************************************************************
Función:
	long M4GetIDNode (long a_lType)
***********************************************************************************
Descripción:
	Retorna el identificador del nodo solicitado.
Parámetros:
	a_lType -> Tipo de nodo solicitado.
Valor de retorno:
	Identificador del nodo solicitado o cero si no se realiza.
Comentarios:
	 Los valores que admite esta función son:
		M4TREE_CURRENT_NODE
		M4TREE_ROOT_NODE
		M4TREE_NEXT_NODE
***********************************************************************************/
long CM4Tree32Ctrl::M4GetIDNode (long a_lType) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetIDNode: El árbol no existe");
		#endif
		return 0L;
	}
	CM4Node *pNode = NULL;
	switch (a_lType)
	{
		case M4TREE_CURRENT_NODE:
			return m_pTree->M4GetCurrentNode ()->M4GetNodeID ();
		case M4TREE_ROOT_NODE:
			m_pTree->M4SetNextNode (m_pTree->M4GetRootNode ());
			return m_pTree->M4GetRootNode ()->M4GetNodeID ();
		case M4TREE_NEXT_NODE:
			pNode = m_pTree->M4GetNextNode ();
			if (pNode)
			{
				m_pTree->M4SetNextNode (pNode);
				return pNode->M4GetNodeID ();
			}
	}
	return 0L;
}

/***********************************************************************************
Función:
	BOOL M4CutNode (long a_lNodeID)
***********************************************************************************
Descripción:
	Cortar un nodo y sus hijos del árbol.
Parámetros:
	a_lNodeID -> Identificador del nodo.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Al realizar esta función por segunda vez, todo subárbol anteriormente
    cortado se borra.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4CutNode(long a_lNodeID) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4CutNode: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	CM4Node *pParent = pNode->M4GetParent ();
	if (m_pTree->M4CutNode (a_lNodeID) == FALSE)
	{
		m_lError = M4TREE_NO_NODE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4CutNode: El nodo %ld no existe", a_lNodeID);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	if (m_pTree->M4GetRootNode ()==NULL)
	{
		M4DELETE(m_pTree);
	}
	pParent->M4SetValid (FALSE);
	m_bResize = TRUE;
	InvalidateControl();
	return TRUE;
}

/***********************************************************************************
Función:
	BOOL M4PasteNode (long a_lNodeID)
***********************************************************************************
Descripción:
	Pega un nodo y sus hijos al árbol.
Parámetros:
	a_lNodeID -> Identificador del nodo padre.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4PasteNode (long a_lParentID) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4PasteNode: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pParent = m_pTree->M4FindNode (a_lParentID);
		if (m_pTree->M4PasteNode (a_lParentID) == FALSE)
	{
		m_lError = M4TREE_NO_NODE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4PasteNode: El nodo %ld no existe", a_lParentID);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	pParent->M4SetValid (FALSE);
	m_bResize = TRUE;
	InvalidateControl ();
	return TRUE;
}

/***********************************************************************************
Función:
	BOOL M4PrintTree ()
***********************************************************************************
Descripción:
	Imprime el árbol.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4PrintTree(void) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4PrintTree: El árbol no existe");
		#endif
		return FALSE;
	}
    CM4PrintDialog *printDlg = new CM4PrintDialog();
	if (printDlg == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("M4PrintTree: Ha fallado la reserva de memoria");
		#endif
		return FALSE;
	}
	printDlg->m_pTreeCtrl = this;
    printDlg->DoModal();
    M4DELETE(printDlg);

	return TRUE;
}

/***********************************************************************************
Función:
	long M4GetLevelCount ()
***********************************************************************************
Descripción:
	Retorna el número de niveles del árbol.
Valor de retorno:
	Número de niveles del árbol o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4GetLevelCount() 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetLevelCount: El árbol no existe");
		#endif
		return 0L;
	}
	return m_pTree->M4GetLevelCount ();
}

/***********************************************************************************
Función:
	long M4GetNodeCount ()
***********************************************************************************
Descripción:
	Retorna el número de nodos del árbol.
Valor de retorno:
	Número de nodos del árbol o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4GetNodeCount() 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeCount: El árbol no existe");
		#endif
		return 0L;
	}
	return m_pTree->M4GetNodeCount ();
}

/***********************************************************************************
Función:
	long M4GetNodeIDNode (long a_lNodeID, long a_lType)
***********************************************************************************
Descripción:
	Retorna el identificador del nodo asociado al nodo referenciado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
	a_lType -> Tipo de nodo solicitado.
Valor de retorno:
	Identificador del nodo solicitado o cero si no se realiza.
Comentarios:
	 Los valores que admite esta función son:
		M4TREE_PARENT
		M4TREE_FIRST_CHILD
		M4TREE_LAST_CHILD
		M4TREE_NEXT_SIBLING
		M4TREE_PREV_SIBLING
***********************************************************************************/
long CM4Tree32Ctrl::M4GetNodeIDNode(long a_lNodeID, long a_lType) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeIDNode: El árbol no existe");
		#endif
		return 0L;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode == NULL)
	{
		m_lError = M4TREE_NO_NODE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetNodeIDNode: El nodo %ld no existe", a_lNodeID);
			// AfxMessageBox (szWarning);
		#endif
		return 0L;
	}

	CM4Node *pAux = NULL;
	switch (a_lType)
	{
		case M4TREE_PARENT:
			pAux = pNode->M4GetParent ();
			break;
		case M4TREE_FIRST_CHILD:
			pAux = pNode->M4GetFirstChild ();
			break;
		case M4TREE_LAST_CHILD:
			pAux = pNode->M4GetLastChild ();
			break;
		case M4TREE_NEXT_SIBLING:
			pAux = pNode->M4GetNextSibling ();
			break;
		case M4TREE_PREV_SIBLING:
			pAux = pNode->M4GetPrevSibling ();
			break;
	}
	if (pAux)
		return pAux->M4GetNodeID ();
	return 0L;
}

/***********************************************************************************
Función:
	long M4GetNodeLevel (long a_lNodeID)
***********************************************************************************
Descripción:
	Retorna el nivel del nodo.
Parámetros:
	a_lNodeID -> Identificador del nodo.
Valor de retorno:
	Nivel del nodo o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4GetNodeLevel(long a_lNodeID) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeLevel: El árbol no existe");
		#endif
		return 0L;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
		return pNode->M4GetLevel ();

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetLevelCount: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return 0L;
}

/***********************************************************************************
Función:
	long M4GetNodeNumberChildren (long a_lNodeID)
***********************************************************************************
Descripción:
	Retorna el número de hijos del nodo.
Parámetros:
	a_lNodeID -> Identificador del nodo.
Valor de retorno:
	Número de hijos del nodo o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4GetNodeNumberChildren(long a_lNodeID) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeNumberChildren: El árbol no existe");
		#endif
		return 0L;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
		return pNode->M4GetNumberChildren ();

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetLevelNumberChildren: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return 0L;
}

/***********************************************************************************
Función:
	BOOL M4SetNodeValueLong(long a_lNodeID, LPCTSTR a_szProperty, long a_lValue) 
***********************************************************************************
Descripción:
	Establece un valor long en la propiedad del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_lValue -> Valor.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Las propiedades implicantes son las siguientes:
		A_CVO	-> Controles visibles para el estado NODO ABIERTO.
		A_CVC	-> Controles visibles para el estado NODO CERRADO.
		A_CVN	-> Controles visibles para el estado NODO SIN HIJOS.
		A_BC	-> Color de fondo.
		A_ST	-> Estilo del nodo.
		A_CX	-> Anchura del nodo.
		A_CY	-> Altura del nodo.
		T*_TT	-> Texto.
		T*_FC	-> Color del texto.
		T*_FN	-> Nombre de la fuente.
		T*_FS	-> Tamaño de la fuente.
		T*_FW	-> Anchura de la fuente.
		T*_FI	-> Bandera de cursiva.
		T*_FU	-> Bandera de subrayado.
		T*_FK	-> Bandera de tachado.
		T*_X	-> Origen X del texto respecto al origen del nodo.
		T*_Y	-> Origen Y del texto respecto al origen del nodo.
		T*_CX	-> Anchura del texto.
		T*_CY	-> Altura del texto.
		P*_BT	-> Bitmap.
		P*_X	-> Origen X del bitmap respecto al origen del nodo.
		P*_Y	-> Origen Y del bitmap respecto al origen del nodo.
		P*_CX	-> Anchura del bitmap.
		P*_CY	-> Altura del bitmap.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4SetNodeValueLong(long a_lNodeID, LPCTSTR a_szProperty, long a_lValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4SetNodeValueLong: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		pNode->M4GetLocalProperty ()->M4Insert (a_szProperty, a_lValue);
		pNode->M4SetValid (FALSE);
		m_bResize = TRUE;
		InvalidateControl();
		return TRUE;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4SetNodeValueLong: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/***********************************************************************************
Función:
	BOOL M4SetNodeValueString(long a_lNodeID, LPCTSTR a_szProperty, LPCTSTR a_szValue) 
***********************************************************************************
Descripción:
	Establece una cadena en la propiedad del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_szValue -> Cadena que contiene el valor.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4SetNodeValueString(long a_lNodeID, LPCTSTR a_szProperty, LPCTSTR a_szValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4SetNodeValueString: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		pNode->M4GetLocalProperty ()->M4Insert (a_szProperty, a_szValue);
		pNode->M4SetValid (FALSE);
		m_bResize = TRUE;
		InvalidateControl();
		return TRUE;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4SetNodeValueString: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/***********************************************************************************
Función:
	BOOL M4GetNodeValueLong(long a_lNodeID, LPCTSTR a_szProperty, long FAR* a_lValue) 
***********************************************************************************
Descripción:
	Retorna el valor long de la propiedad del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_lValue -> Valor de retorno.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4GetNodeValueLong(long a_lNodeID, LPCTSTR a_szProperty, long FAR* a_lValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeValueLong: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		long lIndex = 0L;
		if (pNode->M4GetLocalProperty ()->M4FindProperty (a_szProperty, lIndex) == TRUE)
		{
			*a_lValue = pNode->M4GetLocalProperty ()->M4GetValueLong (lIndex);
			return TRUE;
		}
		m_lError = M4TREE_NO_PROPERTY;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetNodeValueLong: La propiedad %s no existe", a_szProperty);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetNodeValueLong: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4GetNodeValueString(long a_lNodeID, LPCTSTR a_szProperty, BSTR FAR* a_szValue) 
***********************************************************************************
Descripción:
	Retorna el valor long de la propiedad del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_szValue -> Cadena que contiene el valor de retorno.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4GetNodeValueString(long a_lNodeID, LPCTSTR a_szProperty, BSTR FAR* a_szValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeValueString: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		long lIndex = 0L;
		if (pNode->M4GetLocalProperty ()->M4FindProperty (a_szProperty, lIndex) == TRUE)
		{
			CString szReturn (pNode->M4GetLocalProperty ()->M4GetValueString (lIndex));
			szReturn.SetSysString (a_szValue);
			return TRUE;
		}
		m_lError = M4TREE_NO_PROPERTY;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetNodeValueString: La propiedad %s no existe", a_szProperty);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetNodeValueString: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4GetNodeProperty(long a_lNodeID, long a_lIndex, BSTR FAR* a_szProperty) 
***********************************************************************************
Descripción:
	Retorna la propiedad especificada por un indice del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
	a_lIndex -> Indice de la propiedad.
	a_szProperty -> Cadena que contiene el nombre de la propiedad de retorno.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4GetNodeProperty(long a_lNodeID, long a_lIndex, BSTR FAR* a_szProperty) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeProperty: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		CString szReturn (pNode->M4GetLocalProperty ()->M4GetProperty (a_lIndex));
		if (szReturn.IsEmpty () == FALSE)
		{
			szReturn.SetSysString (a_szProperty);
			return TRUE;
		}
		m_lError = M4TREE_NO_PROPERTY;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetNodeProperty: La propiedad %s no existe", a_szProperty);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetNodeProperty: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	long M4GetNodeNumberProperties(long a_lNodeID) 
***********************************************************************************
Descripción:
	Retorna el número de propiedades del nodo.
Parámetros:
	a_lNodeID -> Identificador del nodo.
Valor de retorno:
	Número de propiedades del nodo o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4GetNodeNumberProperties(long a_lNodeID) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeNumberProperties: El árbol no existe");
		#endif
		return 0L;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
		return pNode->M4GetLocalProperty ()->M4GetNumberProperties ();

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetNodeNumberProperties: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return 0L;
}

/**********************************************************************************
Función:
	BOOL M4SetStyleValueLong(long a_lStyle, LPCTSTR a_szProperty, long a_lValue) 
***********************************************************************************
Descripción:
	Establece un valor long en la propiedad del estilo indicado.
Parámetros:
	a_lStyle -> Identificador del estilo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_lValue -> Valor.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4SetStyleValueLong(long a_lStyle, LPCTSTR a_szProperty, long a_lValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4SetStyleValueLong: El árbol no existe");
		#endif
		return FALSE;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4SetStyleValueLong: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	m_pStyle[a_lStyle]->M4Insert (a_szProperty, a_lValue);
	m_bInvalidateAll = TRUE;
	m_bResize = TRUE;
	InvalidateControl();
	return TRUE;
}

/**********************************************************************************
Función:
	BOOL M4SetStyleValueString(long a_lStyle, LPCTSTR a_szProperty, LPCTSTR a_szValue) 
***********************************************************************************
Descripción:
	Establece una cadena en la propiedad del estilo indicado.
Parámetros:
	a_lStyle -> Identificador del estilo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_szValue -> Cadena que contiene el valor.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4SetStyleValueString(long a_lStyle, LPCTSTR a_szProperty, LPCTSTR a_szValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4SetStyleValueString: El árbol no existe");
		#endif
		return FALSE;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4SetStyleValueString: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	m_pStyle[a_lStyle]->M4Insert (a_szProperty, a_szValue);
	m_bInvalidateAll = TRUE;
	m_bResize = TRUE;
	InvalidateControl();
	return TRUE;
}

/**********************************************************************************
Función:
	BOOL M4GetStyleValueLong(long a_lStyle, LPCTSTR a_szProperty, long FAR* a_lValue) 
***********************************************************************************
Descripción:
	Retorna el valor long de la propiedad del estilo indicado.
Parámetros:
	a_lStyle -> Identificador del estilo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_lValue -> Valor de retorno.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4GetStyleValueLong(long a_lStyle, LPCTSTR a_szProperty, long FAR* a_lValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetStyleValueLong: El árbol no existe");
		#endif
		return FALSE;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetStyleValueLong: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	long lIndex = 0L;
	if (m_pStyle[a_lStyle]->M4FindProperty (a_szProperty, lIndex) == TRUE)
	{
		*a_lValue = m_pStyle[a_lStyle]->M4GetValueLong (lIndex);
		return TRUE;
	}

	m_lError = M4TREE_NO_PROPERTY;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetStyleValueLong: La propiedad %s no existe", a_szProperty);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4GetStyleValueString(long a_lStyle, LPCTSTR a_szProperty, BSTR FAR* a_szValue) 
***********************************************************************************
Descripción:
	Retorna el valor long de la propiedad del estilo indicado.
Parámetros:
	a_lStyle -> Identificador del estilo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_szValue -> Cadena que contiene el valor de retorno.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4GetStyleValueString(long a_lStyle, LPCTSTR a_szProperty, BSTR FAR* a_szValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetStyleValueString: El árbol no existe");
		#endif
		return FALSE;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetStyleValueString: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	long lIndex = 0L;
	if (m_pStyle[a_lStyle]->M4FindProperty (a_szProperty, lIndex) == TRUE)
	{
		CString szReturn (m_pStyle[a_lStyle]->M4GetValueString (lIndex));
		szReturn.SetSysString (a_szValue);
		return TRUE;
	}

	m_lError = M4TREE_NO_PROPERTY;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetStyleValueString: La propiedad %s no existe", a_szProperty);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4GetStyleProperty(long a_lStyle, long a_lIndex, BSTR FAR* a_szProperty) 
***********************************************************************************
Descripción:
	Retorna la propiedad especificada por un indice del estilo indicado.
Parámetros:
	a_lStyle -> Identificador del estilo.
	a_lIndex -> Indice de la propiedad.
	a_szProperty -> Cadena que contiene el nombre de la propiedad de retorno.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
Comentarios:
	 Ver propiedades implicantes.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4GetStyleProperty(long a_lStyle, long a_lIndex, BSTR FAR* a_szProperty) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetStyleProperty: El árbol no existe");
		#endif
		return FALSE;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetStyleProperty: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	CString szReturn (m_pStyle[a_lStyle]->M4GetProperty (a_lIndex));
	if (szReturn.IsEmpty () == FALSE)
	{
		szReturn.SetSysString (a_szProperty);
		return TRUE;
	}

	m_lError = M4TREE_NO_PROPERTY;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetStyleProperty: La propiedad %s no existe", a_szProperty);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	long M4GetStyleNumberProperties(long a_lStyle) 
***********************************************************************************
Descripción:
	Retorna el número de propiedades del nestilo.
Parámetros:
	a_lStyle -> Identificador del estilo.
Valor de retorno:
	Número de propiedades del nodo o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4GetStyleNumberProperties(long a_lStyle) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetStyleNumberProperties: El árbol no existe");
		#endif
		return 0L;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetStyleNumberProperties: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return 0L;
	}
	return m_pStyle[a_lStyle]->M4GetNumberProperties ();
}

/**********************************************************************************
Función:
	BOOL M4GetNodeExpand(long a_lNodeID) 
***********************************************************************************
Descripción:
	Retorna el valor de expansión del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nivel.
Valor de retorno:
	TRUE si el nodo está expandido y FALSE si está colapsado o si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4GetNodeExpand(long a_lNodeID) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeExpand: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
		return pNode->M4GetExpand ();

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetNodeExpand: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4SetNodeExpand(long a_lNodeID, BOOL a_bExpand) 
***********************************************************************************
Descripción:
	Establece el valor de expansión del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nivel.
	a_bExpand -> Valor de expansión.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4SetNodeExpand(long a_lNodeID, BOOL a_bExpand) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4SetNodeExpand: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		pNode->M4SetExpand (a_bExpand);
		pNode->M4SetValid (FALSE);
		m_bResize = TRUE;
		InvalidateControl();
		return TRUE;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4SetNodeExpand: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4ExpandLevel (long a_lLevel)
***********************************************************************************
Descripción:
	Expande un nivel del árbol.
Parámetros:
	a_lLevel -> Identificador del nivel.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4ExpandLevel(long a_lLevel) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4ExpandLevel: El árbol no existe");
		#endif
		return FALSE;
	}
	m_pTree->M4ExpandLevel (a_lLevel);
	m_bInvalidateAll = TRUE;
	m_bResize = TRUE;
	InvalidateControl ();
	SetModifiedFlag();
	return TRUE;
}

/**********************************************************************************
Función:
	long M4FindLongTree(LPCTSTR a_szProperty, long a_lValue) 
***********************************************************************************
Descripción:
	Busca una propiedad y un valor long dentro del árbol.
Parámetros:
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_lValue -> Valor.
Valor de retorno:
	Identificador del primer nodo que cumple la condición o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4FindLongTree(LPCTSTR a_szProperty, long a_lValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4FindLongTree: El árbol no existe");
		#endif
		return 0L;
	}
	CM4Node *pNode = m_pTree->M4GetRootNode ();
	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{

		long lIndex = 0L;
		if (pNode->M4GetLocalProperty ()->M4FindProperty (a_szProperty, lIndex) == TRUE)
		{
			if (pNode->M4GetLocalProperty ()->M4GetValueLong (lIndex) == a_lValue)
				return pNode->M4GetNodeID ();
		}

		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && (pNode->M4GetExpand () == TRUE))
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
					return 0L;
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
							return 0L;
					}
				}
			}
		}
	}
	return 0L;
}

/**********************************************************************************
Función:
	long M4FindStringTree(LPCTSTR a_szProperty, LPCTSTR a_szValue) 
***********************************************************************************
Descripción:
	Busca una propiedad y una cadena dentro del árbol.
Parámetros:
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
	a_sValue -> Cadena que contiene el valor.
Valor de retorno:
	Identificador del primer nodo que cumple la condición o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4FindStringTree(LPCTSTR a_szProperty, LPCTSTR a_szValue) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4FindLongTree: El árbol no existe");
		#endif
		return 0L;
	}
	CM4Node *pNode = m_pTree->M4GetRootNode ();
	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{

		long lIndex = 0L;
		if (pNode->M4GetLocalProperty ()->M4FindProperty (a_szProperty, lIndex) == TRUE)
		{
			if (strcmp (pNode->M4GetLocalProperty ()->M4GetValueString (lIndex), a_szValue) == 0)
				return pNode->M4GetNodeID ();
		}

		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && (pNode->M4GetExpand () == TRUE))
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
					return 0L;
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
							return 0L;
					}
				}
			}
		}
	}
	return 0L;
}

/**********************************************************************************
Función:
	long M4CreateStyle() 
***********************************************************************************
Descripción:
	Crea un nuevo estilo.
Parámetros:
	Ninguno.
Valor de retorno:
	Identificador del estilo creado o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4CreateStyle() 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4CreateStyle: El árbol no existe");
		#endif
		return 0L;
	}
	if (m_lNextStyle == M4TREE_SIZE_LIST_STYLE)
	{
		#ifdef _DEBUG
			// AfxMessageBox ("M4CreateStyle: No se pueden crear más estilos");
		#endif
		return 0L;
	}
	m_pStyle[m_lNextStyle] = new CM4Style (16);
	if (m_pStyle[m_lNextStyle] == NULL)
	{
		m_lError = M4TREE_NO_MEMORY;
		#ifdef _DEBUG
			// AfxMessageBox ("M4CreateStyle: Ha fallado la reserva de memoria");
		#endif
		return FALSE;
	}
	m_lNextStyle++;
	return m_lNextStyle-1;
}

/**********************************************************************************
Función:
	BOOL M4SetNodeStyle(long a_lNodeID, long a_lStyle) 
***********************************************************************************
Descripción:
	Establece el estilo del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
	a_lStyle -> Identificador del estilo.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4SetNodeStyle(long a_lNodeID, long a_lStyle) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4SetNodeStyle: El árbol no existe");
		#endif
		return FALSE;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4SetNodeStyle: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		CString csPropST;
		csPropST = M4TREE_CONTROL_A; csPropST += M4TREE_PROP_ST;
		pNode->M4GetLocalProperty ()->M4Insert (csPropST, a_lStyle);
		pNode->M4SetValid (FALSE);
		m_bResize = TRUE;
		InvalidateControl();
		return TRUE;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4SetNodeStyle: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4SetLevelStyle(long a_lLevel, long a_lStyle) 
***********************************************************************************
Descripción:
	Establece el estilo del nivel especificado.
Parámetros:
	a_lLevel -> Identificador del nivel.
	a_lStyle -> Identificador del estilo.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4SetLevelStyle(long a_lLevel, long a_lStyle) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4SetLevelStyle: El árbol no existe");
		#endif
		return FALSE;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4SetLevelStyle: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4GetRootNode ();
	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{

		if ((pNode->M4GetLevel () == a_lLevel) || (a_lLevel == M4TREE_ALL_LEVEL))
		{
			CString csPropST;
			csPropST = M4TREE_CONTROL_A; csPropST += M4TREE_PROP_ST;
			pNode->M4GetLocalProperty ()->M4Insert (csPropST, a_lStyle);
			pNode->M4SetValid (FALSE);
		}

		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && ((pNode->M4GetLevel () < a_lLevel) || (a_lLevel == M4TREE_ALL_LEVEL)))
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
				{
					m_bResize = TRUE;
					InvalidateControl();
					return TRUE;
				}
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
						{
							m_bResize = TRUE;
							InvalidateControl();
							return TRUE;
						}
					}
				}
			}
		}
	}
	m_bResize = TRUE;
	InvalidateControl();
	return TRUE;
}

/**********************************************************************************
Función:
	long M4GetNodeStyle(long a_lNodeID) 
***********************************************************************************
Descripción:
	Retorna el estilo del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
Valor de retorno:
	Identificador del estilo del nodo o cero si no se realiza.
***********************************************************************************/
long CM4Tree32Ctrl::M4GetNodeStyle(long a_lNodeID) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4GetNodeStyle: El árbol no existe");
		#endif
		return 0L;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		long lIndex = 0L;
		CString csPropST;
		csPropST = M4TREE_CONTROL_A; csPropST += M4TREE_PROP_ST;
		if (pNode->M4GetLocalProperty ()->M4FindProperty (csPropST, lIndex) == TRUE)
			return pNode->M4GetLocalProperty ()->M4GetValueLong (lIndex);
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4GetNodeStyle: El nodo %ld no tiene estilo", a_lNodeID);
			// AfxMessageBox (szWarning);
		#endif
		return 0L;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4GetNodeStyle: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return 0L;
}

/**********************************************************************************
Función:
	BOOL M4DeleteNodeProperty(long a_lNodeID, LPCTSTR a_szProperty) 
***********************************************************************************
Descripción:
	Borra una propiedad del nodo indicado.
Parámetros:
	a_lNodeID -> Identificador del nodo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4DeleteNodeProperty(long a_lNodeID, LPCTSTR a_szProperty) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4DeleteNodeProperty: El árbol no existe");
		#endif
		return FALSE;
	}
	CM4Node *pNode = m_pTree->M4FindNode (a_lNodeID);
	if (pNode)
	{
		long lIndex = 0L;
		if (pNode->M4GetLocalProperty ()->M4FindProperty (a_szProperty, lIndex) == TRUE)
		{
			pNode->M4GetLocalProperty ()->M4DeleteProperty (lIndex);
			pNode->M4SetValid (FALSE);
			return TRUE;
		}
		m_lError = M4TREE_NO_PROPERTY;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4DeleteNodeProperty: La propiedad %s no existe", a_szProperty);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}

	m_lError = M4TREE_NO_NODE;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4DeleteNodeProperty: El nodo %ld no existe", a_lNodeID);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4DeleteStyleProperty(long a_lStyle, LPCTSTR a_szProperty) 
***********************************************************************************
Descripción:
	Borra una propiedad del estilo indicado.
Parámetros:
	a_lStyle -> Identificador del estilo.
	a_szProperty -> Cadena que contiene el nombre de la propiedad.
Valor de retorno:
	TRUE si se realiza y FALSE si no se realiza.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4DeleteStyleProperty(long a_lStyle, LPCTSTR a_szProperty) 
{
	if (m_pTree == NULL)
	{
		m_lError = M4TREE_NO_TREE;
		#ifdef _DEBUG
			// AfxMessageBox ("M4DeleteStyleProperty: El árbol no existe");
		#endif
		return FALSE;
	}
	if (m_pStyle[a_lStyle] == NULL)
	{
		m_lError = M4TREE_NO_STYLE;
		#ifdef _DEBUG
			char szWarning [150];
			sprintf (szWarning, "M4DeleteStyleProperty: El estilo %ld no existe", a_lStyle);
			// AfxMessageBox (szWarning);
		#endif
		return FALSE;
	}
	long lIndex = 0L;
	if (m_pStyle[a_lStyle]->M4FindProperty (a_szProperty, lIndex) == TRUE)
	{
		m_pStyle[a_lStyle]->M4DeleteProperty (lIndex);
		m_bInvalidateAll = TRUE;
		return TRUE;
	}

	m_lError = M4TREE_NO_PROPERTY;
	#ifdef _DEBUG
		char szWarning [150];
		sprintf (szWarning, "M4DeleteStyleProperty: La propiedad %s no existe", a_szProperty);
		// AfxMessageBox (szWarning);
	#endif
	return FALSE;
}

/**********************************************************************************
Función:
	BOOL M4GetTreeStatus() 
***********************************************************************************
Descripción:
	Retorna el estado del árbol.
Parámetros:
	Ninguno.
Valor de retorno:
	TRUE si el árbol existe y FALSE si no existe.
***********************************************************************************/
BOOL CM4Tree32Ctrl::M4GetTreeStatus() 
{
	if (m_pTree)
		return TRUE;
	return FALSE;
}
///////////////////////// Propiedades /////////////////////////

void CM4Tree32Ctrl::OnSTreeStyleChanged() 
{
	m_bRebuild = TRUE;
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnSSiblingDistanceChanged() 
{
	m_bRebuild = TRUE;
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnSLevelDistanceChanged() 
{
	m_bRebuild = TRUE;
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnSLinkTypeChanged() 
{
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnSLinkStyleChanged() 
{
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnSLeftOffsetChanged() 
{
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnSTopOffsetChanged() 
{
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnSAppearanceChanged() 
{
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnBCompactChanged() 
{
	m_bRebuild = TRUE;
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnBBalanceChanged() 
{
	m_bRebuild = TRUE;
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnDScaleChanged() 
{
	if (m_dScale < 0.1)
		m_dScale = 0.1;
	else
	{
		m_bRebuild = TRUE;
		InvalidateControl ();
		SetModifiedFlag();
	}
}

void CM4Tree32Ctrl::OnBShowChanged() 
{
	m_bRebuild = TRUE;
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnBStoreBitmapChanged() 
{
	m_bRebuild = TRUE;
	InvalidateControl ();
	SetModifiedFlag();
}

void CM4Tree32Ctrl::OnSLinkWidthChanged() 
{
	InvalidateControl ();
	SetModifiedFlag();
}

BOOL CM4Tree32Ctrl::M4ChangeLevel(void) 
{
	if (m_pTree == NULL) return FALSE;

	m_pTree->M4ChangeLevelNode (m_pTree->M4GetCurrentNode ());
	m_bResize = TRUE;
	InvalidateControl ();
	return TRUE;
}


void CM4Tree32Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	InvalidateControl ();
}
