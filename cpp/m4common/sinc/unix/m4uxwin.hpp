#ifndef __MUXWIN_HPP__
#define __MUXWIN_HPP__

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "m4types.hpp"

#define t_PID        pid_t
#define NULL_PID     0

#define t_PROCESS	pid_t
#define NULL_PROCESS     0

#define t_STATUS unsigned long

#define STILL_ACTIVE    1 

#define _export
//#define signed
#define __cdecl
#define far
#define huge

#define WINVER  0x031a

#define GetCurrentTask() 	( getpid() )
#define GetCurrentProcess() 	( getpid() )
//JRM
//#define m4sleep( sec ) 		( sleep( sec ) )
#define Sleep( milisec )    ( sleep( (milisec) / 1000 ) )
#define _heapmin()

/******* Common definitions and ////   typedefs ***********************************/


#define FAR                 //  _far
#define NEAR            //  _near
#define PASCAL          //  _pascal
#define CDECL            //  _cdecl
#define EXPORT



#define WINAPI              //  _far _pascal
#define CALLBACK            //  _far _pascal

/****** Simple types & common helper macros *********************************/

// Constantes de MessageBox
#define	MB_TASKMODAL		0
#define IDOK                1
#define IDCANCEL            2
#define IDABORT             3
#define IDRETRY             4
#define IDIGNORE            5
#define IDYES               6
#define IDNO                7
#ifndef	MB_OK
#define MB_OK                       0x00000000L
#endif
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L

//int   WINAPI  MessageBox( HWND a_hWinParent, LPCSTR a_lpszText, LPCSTR a_lpszTitle, UINT a_uiStyle)
//#define MessageBox(a_hWinParent, a_lpszText,a_lpszTitle,a_uiStyle) ( printf( "MessageBox( %d, \"%s\", \"%s\", %d )\n",a_hWinParent, a_lpszText ,a_lpszTitle, a_uiStyle )) 
inline int MessageBox( HWND a_hWinParent, LPCSTR a_lpszText, LPCSTR a_lpszTitle, UINT a_uiStyle)
{

    printf( "MessageBox( %d, \"%s\", \"%s\", %d )\n",
            a_hWinParent, a_lpszText, a_lpszTitle, a_uiStyle ) ;

    return 1;
}






 
		 
															 
#endif   // __MUXWIN_HPP__ 







