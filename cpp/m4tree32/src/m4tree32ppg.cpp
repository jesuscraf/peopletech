// M4Tree32Ppg.cpp : Implementation of the CM4Tree32PropPage property page class.

#include "stdafx.h"
#include "m4tree32.h"
#include "m4tree32ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CM4Tree32PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CM4Tree32PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CM4Tree32PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CM4Tree32PropPage, "M4TREE32.M4Tree32PropPage.1",
	0x64f5a7a9, 0xbfa7, 0x11d0, 0x8d, 0xa1, 0, 0xc0, 0x4f, 0xb7, 0x14, 0x6e)


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32PropPage::CM4Tree32PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CM4Tree32PropPage

BOOL CM4Tree32PropPage::CM4Tree32PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_M4TREE32_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32PropPage::CM4Tree32PropPage - Constructor

CM4Tree32PropPage::CM4Tree32PropPage() :
	COlePropertyPage(IDD, IDS_M4TREE32_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CM4Tree32PropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32PropPage::DoDataExchange - Moves data between page and properties

void CM4Tree32PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CM4Tree32PropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CM4Tree32PropPage message handlers
