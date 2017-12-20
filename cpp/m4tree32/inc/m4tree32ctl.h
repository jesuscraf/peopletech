#if !defined(AFX_M4TREE32CTL_H__64F5A7B6_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED_)
#define AFX_M4TREE32CTL_H__64F5A7B6_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// M4Tree32Ctl.h : Declaration of the CM4Tree32Ctrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CM4Tree32Ctrl : See M4Tree32Ctl.cpp for implementation.

class CM4Tree32Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CM4Tree32Ctrl)

// Constructor
public:
	CM4Tree32Ctrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CM4Tree32Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CM4Tree32Ctrl();



#ifdef NO_MORE_LICENSES_PLEASE	
	DECLARE_OLECREATE_EX(CM4Tree32Ctrl)    // Class factory and guid
#else
	BEGIN_OLEFACTORY(CM4Tree32Ctrl)        // Class factory and guid
		virtual BOOL VerifyUserLicense();
		virtual BOOL GetLicenseKey(DWORD, BSTR FAR*);
	END_OLEFACTORY(CM4Tree32Ctrl)
#endif
		
	DECLARE_OLETYPELIB(CM4Tree32Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CM4Tree32Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CM4Tree32Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CM4Tree32Ctrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CM4Tree32Ctrl)
	short m_sSiblingDistance;
	afx_msg void OnSSiblingDistanceChanged();
	short m_sLevelDistance;
	afx_msg void OnSLevelDistanceChanged();
	short m_sLinkType;
	afx_msg void OnSLinkTypeChanged();
	short m_sLeftOffset;
	afx_msg void OnSLeftOffsetChanged();
	short m_sTopOffset;
	afx_msg void OnSTopOffsetChanged();
	short m_sTreeStyle;
	afx_msg void OnSTreeStyleChanged();
	short m_sAppearance;
	afx_msg void OnSAppearanceChanged();
	BOOL m_bCompact;
	afx_msg void OnBCompactChanged();
	BOOL m_bBalance;
	afx_msg void OnBBalanceChanged();
	short m_sLinkStyle;
	afx_msg void OnSLinkStyleChanged();
	double m_dScale;
	afx_msg void OnDScaleChanged();
	BOOL m_bShow;
	afx_msg void OnBShowChanged();
	short m_sLinkWidth;
	afx_msg void OnSLinkWidthChanged();
	BOOL m_bStoreBitmap;
	afx_msg void OnBStoreBitmapChanged();
	afx_msg BOOL M4DeleteNode(long a_lNodeID);
	afx_msg long M4GetLastError();
	afx_msg long M4GetIDNode(long a_lType);
	afx_msg BOOL M4PrintTree();
	afx_msg BOOL M4CutNode(long a_lNodeID);
	afx_msg BOOL M4PasteNode(long a_lParentID);
	afx_msg long M4GetLevelCount();
	afx_msg long M4GetNodeCount();
	afx_msg long M4GetNodeLevel(long a_lNodeID);
	afx_msg long M4GetNodeNumberChildren(long a_lNodeID);
	afx_msg BOOL M4SetNodeValueLong(long a_lNodeID, LPCTSTR a_szProperty, long a_lValue);
	afx_msg BOOL M4SetNodeValueString(long a_lNodeID, LPCTSTR a_szProperty, LPCTSTR a_szValue);
	afx_msg BOOL M4GetNodeValueLong(long a_lNodeID, LPCTSTR a_szProperty, long FAR* a_lValue);
	afx_msg BOOL M4GetNodeValueString(long a_lNodeID, LPCTSTR a_szProperty, BSTR FAR* a_szValue);
	afx_msg BOOL M4GetNodeProperty(long a_lNodeID, long a_lIndex, BSTR FAR* a_szProperty);
	afx_msg long M4GetNodeNumberProperties(long a_lNodeID);
	afx_msg BOOL M4SetStyleValueLong(long a_lStyle, LPCTSTR a_szProperty, long a_lValue);
	afx_msg BOOL M4SetStyleValueString(long a_lStyle, LPCTSTR a_szProperty, LPCTSTR a_szValue);
	afx_msg BOOL M4GetStyleValueLong(long a_lStyle, LPCTSTR a_szProperty, long FAR* a_lValue);
	afx_msg BOOL M4GetStyleValueString(long a_lStyle, LPCTSTR a_szProperty, BSTR FAR* a_szValue);
	afx_msg BOOL M4GetStyleProperty(long a_lStyle, long a_lIndex, BSTR FAR* a_szProperty);
	afx_msg long M4GetStyleNumberProperties(long a_lStyle);
	afx_msg BOOL M4GetNodeExpand(long a_lNodeID);
	afx_msg BOOL M4SetNodeExpand(long a_lNodeID, BOOL a_bExpand);
	afx_msg BOOL M4ExpandLevel(long a_lLevel);
	afx_msg long M4FindLongTree(LPCTSTR a_szProperty, long a_lValue);
	afx_msg long M4FindStringTree(LPCTSTR a_szProperty, LPCTSTR a_szValue);
	afx_msg long M4CreateStyle();
	afx_msg BOOL M4SetNodeStyle(long a_lNodeID, long a_lStyle);
	afx_msg BOOL M4SetLevelStyle(long a_lLevel, long a_lStyle);
	afx_msg long M4GetNodeStyle(long a_lNodeID);
	afx_msg BOOL M4DeleteNodeProperty(long a_lNodeID, LPCTSTR a_szProperty);
	afx_msg BOOL M4DeleteStyleProperty(long a_lStyle, LPCTSTR a_szProperty);
	afx_msg long M4InsertNode(long a_lParentID);
	afx_msg BOOL M4GetTreeStatus();
	afx_msg long M4GetNodeIDNode(long a_lNodeID, long a_lType);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CM4Tree32Ctrl)
	void FireM4LClickInNode(long a_lNodeID)
		{FireEvent(eventidM4LClickInNode,EVENT_PARAM(VTS_I4), a_lNodeID);}
	void FireM4LDbClickInNode(long a_lNodeID)
		{FireEvent(eventidM4LDbClickInNode,EVENT_PARAM(VTS_I4), a_lNodeID);}
	void FireM4RClickInNode(long a_lNodeID)
		{FireEvent(eventidM4RClickInNode,EVENT_PARAM(VTS_I4), a_lNodeID);}
	void FireM4RDbClickInNode(long a_lNodeID)
		{FireEvent(eventidM4RDbClickInNode,EVENT_PARAM(VTS_I4), a_lNodeID);}
	void FireM4ClickOutNode()
		{FireEvent(eventidM4ClickOutNode,EVENT_PARAM(VTS_NONE));}
	void FireM4RClickOutNode()
		{FireEvent(eventidM4RClickOutNode,EVENT_PARAM(VTS_NONE));}
	void FireM4LClickOutNode()
		{FireEvent(eventidM4LClickOutNode,EVENT_PARAM(VTS_NONE));}
	void FireM4RDbClickOutNode()
		{FireEvent(eventidM4RDbClickOutNode,EVENT_PARAM(VTS_NONE));}
	void FireM4LDbClickOutNode()
		{FireEvent(eventidM4LDbClickOutNode,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CM4Tree32Ctrl)
	dispidSSiblingDistance = 1L,
	dispidSLevelDistance = 2L,
	dispidSLinkType = 3L,
	dispidSLeftOffset = 4L,
	dispidSTopOffset = 5L,
	dispidSTreeStyle = 6L,
	dispidSAppearance = 7L,
	dispidBCompact = 8L,
	dispidBBalance = 9L,
	dispidSLinkStyle = 10L,
	dispidDScale = 11L,
	dispidBShow = 12L,
	dispidSLinkWidth = 13L,
	dispidBStoreBitmap = 14L,
	dispidM4DeleteNode = 15L,
	dispidM4GetLastError = 16L,
	dispidM4GetIDNode = 17L,
	dispidM4PrintTree = 18L,
	dispidM4CutNode = 19L,
	dispidM4PasteNode = 20L,
	dispidM4GetLevelCount = 21L,
	dispidM4GetNodeCount = 22L,
	dispidM4GetNodeLevel = 23L,
	dispidM4GetNodeNumberChildren = 24L,
	dispidM4SetNodeValueLong = 25L,
	dispidM4SetNodeValueString = 26L,
	dispidM4GetNodeValueLong = 27L,
	dispidM4GetNodeValueString = 28L,
	dispidM4GetNodeProperty = 29L,
	dispidM4GetNodeNumberProperties = 30L,
	dispidM4SetStyleValueLong = 31L,
	dispidM4SetStyleValueString = 32L,
	dispidM4GetStyleValueLong = 33L,
	dispidM4GetStyleValueString = 34L,
	dispidM4GetStyleProperty = 35L,
	dispidM4GetStyleNumberProperties = 36L,
	dispidM4GetNodeExpand = 37L,
	dispidM4SetNodeExpand = 38L,
	dispidM4ExpandLevel = 39L,
	dispidM4FindLongTree = 40L,
	dispidM4FindStringTree = 41L,
	dispidM4CreateStyle = 42L,
	dispidM4SetNodeStyle = 43L,
	dispidM4SetLevelStyle = 44L,
	dispidM4GetNodeStyle = 45L,
	dispidM4DeleteNodeProperty = 46L,
	dispidM4DeleteStyleProperty = 47L,
	dispidM4InsertNode = 48L,
	dispidM4GetTreeStatus = 49L,
	dispidM4GetNodeIDNode = 50L,
	eventidM4LClickInNode = 1L,
	eventidM4LDbClickInNode = 2L,
	eventidM4RClickInNode = 3L,
	eventidM4RDbClickInNode = 4L,
	eventidM4ClickOutNode = 5L,
	eventidM4RClickOutNode = 6L,
	eventidM4LClickOutNode = 7L,
	eventidM4RDbClickOutNode = 8L,
	eventidM4LDbClickOutNode = 9L,
	//}}AFX_DISP_ID
	};

public:

	void DrawTree (CDC* pdc, CM4Node *a_pNode, CRect &rcBounds);

#include "m4tree32ctl.inl"

protected:
	CM4Tree		*m_pTree;
	CM4Style	*m_pStyle[M4TREE_SIZE_LIST_STYLE];
	CM4Bitmp	*m_pBitmp;

	BOOL m_bPrinting;			// Bandera que se activa cuando se está imprimiendo.
	BOOL m_bResize;				// Bandera que se activa cuando se quiere cambiar el área del nodo y la matriz de areas del árbol.
	BOOL m_bRebuild;			// Bandera que se activa cuando se quiere rediseñar el arbol.
	BOOL m_bInvalidateAll;		// Bandera que se activa cuando se quiere invalidar todos los nodos.
	long m_lOffsetScrollX;		// Desplazamiento X del arbol con respecto al scroll.
	long m_lOffsetScrollY;		// Desplazamiento Y del arbol con respecto al scroll.
	long m_lNodeDown;			// Identificador del nodo que está pulsado.
	long m_lMaxPosHorz;			// Posición máxima que puede alcanzar el scroll horizontal.
	long m_lMaxPosVert;			// Posición máxima que puede alcanzar el scroll vertical.
	long *m_pMaxWidthLevel;		// Máxima anchura por nivel.
	long *m_pMaxHeightLevel;	// Máxima altura por nivel.
	long m_lNextStyle;
	long m_lError;
	
	
	BOOL SetRectTree (void);
	BOOL SetRectTreeVert (void);
	BOOL SetRectTreeHorz (void);
	BOOL SetRectTreeDiag (void);
	void DrawLink (CDC* pdc, CM4Node *a_pNode);
	void DrawNode (CDC* pdc, CM4Node *a_pNode);
	void DrawNodeDown (CDC* pdc, CM4Node *a_pNode);
	void DrawBitmap (CDC* pdc, CM4Node *a_pNode, LPCTSTR a_szControl);
	void DrawText (CDC* pdc, CM4Node *a_pNode, LPCTSTR a_szControl);
	void InNode (const CPoint point, BOOL bLeft, BOOL bDBClick);
	void SizeNode (CDC* pdc);
	long GetValueLong (CM4Node *a_pNode, LPCTSTR a_szProperty);
	LPCTSTR GetValueString (CM4Node *a_pNode, LPCTSTR a_szProperty);
	HBITMAP ReadBitmap (CDC* pdc, LPCTSTR a_szPath);

	BOOL M4ChangeLevel (void); // En construcción.

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_M4TREE32CTL_H__64F5A7B6_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED)
