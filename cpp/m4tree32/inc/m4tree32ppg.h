#if !defined(AFX_M4TREE32PPG_H__64F5A7B8_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED_)
#define AFX_M4TREE32PPG_H__64F5A7B8_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// M4Tree32Ppg.h : Declaration of the CM4Tree32PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CM4Tree32PropPage : See M4Tree32Ppg.cpp.cpp for implementation.

class CM4Tree32PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CM4Tree32PropPage)
	DECLARE_OLECREATE_EX(CM4Tree32PropPage)

// Constructor
public:
	CM4Tree32PropPage();

// Dialog Data
	//{{AFX_DATA(CM4Tree32PropPage)
	enum { IDD = IDD_PROPPAGE_M4TREE32 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CM4Tree32PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_M4TREE32PPG_H__64F5A7B8_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED)
