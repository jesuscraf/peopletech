// m4CSDll.h : main header file for the M4CSDLL DLL
//

#if !defined(AFX_M4CSDLL_H__60578C08_7F8E_11D2_8DBF_00C04FA9BAF9__INCLUDED_)
#define AFX_M4CSDLL_H__60578C08_7F8E_11D2_8DBF_00C04FA9BAF9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CM4CSDllApp
// See m4CSDll.cpp for the implementation of this class
//

class CM4CSDllApp : public CWinApp
{
public:
	CM4CSDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CM4CSDllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CM4CSDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


//=================================================================================
//
// Obtiene un mensaje de texto en el lenguaje del thread
//
// @param ai_iMessage
//
//		Par�metro de entrada con el n�mero del mensaje de texto
//
// @param aio_rsString
//
//		Par�metro de salida con la cadena del mensaje de texto o cadena vac�a
//			si ha habido alg�n error
//
// @return
//
//		El n�mero de caracteres del mensaje de texto sin el 0 final
//		o 0 si ha habido un error
//
//=================================================================================
int	_loadString( int ai_iMessage, CString& aio_rsString ) ;


#endif // !defined(AFX_M4CSDLL_H__60578C08_7F8E_11D2_8DBF_00C04FA9BAF9__INCLUDED_)
