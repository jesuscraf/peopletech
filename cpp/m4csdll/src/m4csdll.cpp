// m4CSDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "m4csdll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CM4CSDllApp

BEGIN_MESSAGE_MAP(CM4CSDllApp, CWinApp)
	//{{AFX_MSG_MAP(CM4CSDllApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CM4CSDllApp construction

CM4CSDllApp::CM4CSDllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CM4CSDllApp object

CM4CSDllApp theApp;





//=================================================================================
//
// Obtiene un mensaje de texto en el lenguaje del thread
//
// Pasos
//		Se inicializa el parámetro de salida
//		Se obtiene el locale actual
//		Se obtiene el handle del módulo actual
//		Obtiene el texto del mensaje con la función del sistema correspondiente
//		Si no se ha obtenido el mensaje se intenta en el lenguaje que se pueda
//		Si no se ha obtenido el mensaje se devuelve 0
//		Si se ha obtenido el mensaje correctamente se le quita el retorno de
//			carro final si lo tiene
//		Se copia en la variable de salida y se libera la memoria que devuelve la
//			función del sistema
//		Se devuelve la longitud del mensaje
//
//=================================================================================

int	_loadString( int ai_iMessage, CString& aio_rsString )
{

	HMODULE	hModule = 0 ;
	LCID	iLocale = 0 ;
	DWORD	dwFlags = 0 ;
	DWORD	dwResult = 0 ;
	LPVOID	pvBuffer = NULL ;
	LPSTR	psBuffer = NULL ;

	
	aio_rsString.Empty() ;

	iLocale = GetThreadLocale() ;
	hModule = GetModuleHandle( "m4csdll.dll" ) ;

	dwFlags = FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS ;
	dwResult = FormatMessage( dwFlags, hModule, ai_iMessage, iLocale, (LPSTR) &pvBuffer, 0, NULL ) ;

	if( dwResult <= 0 || pvBuffer == NULL )
	{
		iLocale = 0 ;
		dwResult = FormatMessage( dwFlags, hModule, ai_iMessage, iLocale, (LPSTR) &pvBuffer, 0, NULL ) ;

		if( dwResult <= 0 || pvBuffer == NULL )
		{
			return( 0 ) ;
		}
	}

	psBuffer = (LPSTR) pvBuffer ;

	if( dwResult >= 2 && psBuffer[ dwResult - 2 ] == '\r' && psBuffer[ dwResult - 1 ] == '\n' )
	{
		dwResult -= 2 ;
		psBuffer[ dwResult ] = '\0' ;
	}

	aio_rsString = psBuffer ;
	LocalFree( pvBuffer ) ;

	return( dwResult ) ;
}




