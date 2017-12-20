#include "stdio.h"
#include "stdlib.h"

#include "m4uxwinobl.hpp"
////////#include "pidstatu.hpp"

//J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
#include "ux_mem.cpp"
#endif
//END



#define MAX_GLB_MEM_OBJS    1000
#define M4_NUM_MAX_PROCESS  10

static  char FAR*   s_apvGlobalMemoryObjects[ MAX_GLB_MEM_OBJS ];
static  DWORD       s_aszGlobalMemoryObjectsSize[ MAX_GLB_MEM_OBJS ];

/////////ClPidStatus g_clPidStatus(M4_NUM_MAX_PROCESS);


HGLOBAL WINAPI GlobalAlloc( UINT a_uiMode, DWORD a_ulSize )
{
    
    HGLOBAL     result ;
    unsigned int    i ;
    int     found = 0 ;
        
    
    a_uiMode = a_uiMode ;
    
    for( i = 0 ; i < MAX_GLB_MEM_OBJS && !found ; i++ ) {
        
        if( s_apvGlobalMemoryObjects[ i ] == NULL ) {
            found = 1 ;
        }
    }

    if( found ) {
        
      //J.A.Sanchez  20/3/97
#ifdef _SOL_JAS
          s_apvGlobalMemoryObjects[ i - 1 ] = sol_malloc_char( size_t( a_ulSize ) ) ;
#else
        s_apvGlobalMemoryObjects[ i - 1 ] = (char FAR*) malloc( size_t( a_ulSize ) ) ;
#endif
	  //END

        if( s_apvGlobalMemoryObjects[ i - 1 ] != NULL ) {
            
            s_aszGlobalMemoryObjectsSize[ i - 1 ] = a_ulSize ;
            result = HGLOBAL( i ) ;
        }
        else {
            s_aszGlobalMemoryObjectsSize[ i - 1 ] = 0 ;
            result = HGLOBAL( NULL ) ;
        }
    }
    else {
        result = HGLOBAL( NULL ) ;
    }
    
    return( result ) ;

}

HGLOBAL WINAPI GlobalReAlloc( HGLOBAL a_hGMO, DWORD a_ulSize, UINT a_uiMode )
{

    HGLOBAL     result ;


    a_uiMode = a_uiMode ;
    
    if( a_hGMO < 1 || a_hGMO > MAX_GLB_MEM_OBJS ) {
    
        result = HGLOBAL( NULL ) ;
    }
    else{
    
        if( s_apvGlobalMemoryObjects[ a_hGMO - 1 ] != NULL ) {

	  //J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
          sol_free( s_apvGlobalMemoryObjects[ a_hGMO - 1 ] );
#else
          free( s_apvGlobalMemoryObjects[ a_hGMO - 1 ] ) ;
#endif
	  //END
        }
        
	  //J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
	s_apvGlobalMemoryObjects[ a_hGMO - 1 ] = sol_malloc_char(size_t( a_ulSize ) ) ;
#else
        s_apvGlobalMemoryObjects[ a_hGMO - 1 ] = (char FAR*) malloc( size_t( a_ulSize ) ) ;
#endif

        if( s_apvGlobalMemoryObjects[ a_hGMO - 1 ] != NULL ) {
            
            s_aszGlobalMemoryObjectsSize[ a_hGMO - 1 ] = a_ulSize ;
            result = HGLOBAL( a_hGMO ) ;
        }
        else {
            s_aszGlobalMemoryObjectsSize[ a_hGMO - 1 ] = 0 ;
            result = HGLOBAL( NULL ) ;
        }
    }
    
    return( result ) ;

}

                


HGLOBAL WINAPI GlobalFree( HGLOBAL a_hGMO )
{

    HGLOBAL result ;
    
    
    if( a_hGMO < 1 || a_hGMO > MAX_GLB_MEM_OBJS ) {
    
        result = a_hGMO ;
    }
    else if( s_apvGlobalMemoryObjects[ a_hGMO - 1 ] != NULL ) {

        
        s_aszGlobalMemoryObjectsSize[ a_hGMO - 1 ] = 0 ;
      //J.A.Sanchez  20/3/97
#ifdef _SOL_JAS
        sol_free( s_apvGlobalMemoryObjects[ a_hGMO - 1 ] );
#else
        free( s_apvGlobalMemoryObjects[ a_hGMO - 1 ] ) ;
#endif
	  //END
        s_apvGlobalMemoryObjects[ a_hGMO - 1 ] = NULL ;
        
        result = HGLOBAL( NULL ) ;
    }
    else {
        result = a_hGMO ;
    }
    
    return( result ) ;
}



char FAR* WINAPI GlobalLock( HGLOBAL a_hGMO )
{

    char FAR*   result ;
    
    
    if( a_hGMO < 1 || a_hGMO > MAX_GLB_MEM_OBJS ) {
    
        result = (char FAR*) NULL ;
    }
    else {
        result = s_apvGlobalMemoryObjects[ a_hGMO - 1 ] ;
    }
    
    return( result ) ;  
}


BOOL    WINAPI GlobalUnlock( HGLOBAL a_hGMO )
{
     a_hGMO = a_hGMO ;
     return( BOOL( 0 ) ) ;
}


void    WINAPI hmemcpy(void _huge* pvDest, const void _huge* cpvSource, long lSize )
{
//ojo con el tamano de size_t, puede que sea mas pequeno que long.
    memcpy( pvDest, cpvSource, (size_t)lSize ) ;
}

 
 
 
DWORD   WINAPI GlobalSize( HGLOBAL a_hGMO )
{

    DWORD   result ;
    
    
    if( a_hGMO < 1 || a_hGMO > MAX_GLB_MEM_OBJS ) {
    
        result = 0 ;
    }
    else {
        result = s_aszGlobalMemoryObjectsSize[ a_hGMO - 1 ] ;
    }
    
    return( result ) ;  
}



/*
HTASK   WINAPI GetCurrentTask( void )
{
    return( HTASK( 0 ) ) ;
}
*/



HWND    GetFocus( void )
{
    return( HWND( NULL ) ) ;
}



HWND    GetActiveWindow( void )
{
    return( HWND( NULL ) ) ;
}



int ShowCursor( BOOL a_bShow )
{
    a_bShow = a_bShow ;
    return( 0 ) ;
}



HCURSOR LoadCursor( HINSTANCE a_hInst, LPCSTR a_pszCursor )
{
    a_hInst     = a_hInst ;
    a_pszCursor = a_pszCursor ;
    
    return( HCURSOR( NULL ) ) ;
}



HCURSOR SetCursor( HCURSOR a_hCursor )
{
    a_hCursor = a_hCursor ;
    return( HCURSOR( NULL ) ) ;
}


HCURSOR GetCursor( void )
{
    return( HCURSOR( NULL ) ) ;
}


int     WINAPI GetAsyncKeyState( int a_iVKey )
{
    a_iVKey = a_iVKey ;
    return( 0 ) ;
}



int     WINAPI MessageBox( HWND a_hWinParent, LPCSTR a_lpszText, LPCSTR a_lpszTitle, UINT a_uiStyle)
{

    printf( "MessageBox( %d, \"%s\", \"%s\", %d )\n",
            a_hWinParent, a_lpszText, a_lpszTitle, a_uiStyle ) ;

    return( IDOK ) ;

}



LRESULT WINAPI  SendMessage( HWND a_hWindow, UINT a_uiMessage, WPARAM a_wParam, LPARAM a_lParam)
{
    
    printf( "SendMessage( %d, %d, %d, \"%s\" )", a_hWindow, a_uiMessage, a_wParam, a_lParam );

    switch( a_uiMessage ) {

        case    WM_SETTEXT :    printf( "\n" ) ;
                    break ;

        default        :    printf( " Not Implemented.\n" );
                    break ;
    }

    return( LRESULT( 0 ) ) ;

}

/*
int M4CreateProcess (char *ai_pcExe, t_PROCESS   *ai_pid)
{
t_PROCESS pid;

pid = fork();
////printf ("pid: %i\n ",pid);

    if (pid == 0)
    {  // codigo del hijo
	////printf ("arrancamos un proceso %s\n", ai_pcExe);
	execlp ("sh", "sh", "-c",ai_pcExe,NULL);
////	printf ("error: Fallo execlp\n");
	return(0);
      }
    else if (pid == -1)
    { // error
////	printf("Error al crear los procesos");
	return (0);
	}
    else
    {  // codigo del padre
	*ai_pid = pid;
	// se graba el estado de proceso vivo
	g_clPidStatus.SetPidStatus( pid, STILL_ACTIVE);
	return(1);
      }

}

int GetExitCodeProcess ( t_PROCESS ai_process, unsigned long* result)
{
// devuelve STILL_ACTIVE o el resultado que devolvio
// se guarda en un array los procesos
// se mira la tabal de procesos levantados, y su estado final

g_clPidStatus.GetPidStatus(ai_process, *result);
////printf ("GetExitCode  proc %d, resul %d\n",ai_process, *result);

if (*result != STILL_ACTIVE)
   {
////   printf ("Liberamos el proceso correspondiente\n");
   g_clPidStatus.FreePidStatus(ai_process);
   }
return 1;
      }
      

void sigchild_died_handler(int sig)
{
int status;
t_PROCESS pid;

// los 8 primeros bits son lo devuelto por el proceso, y los 8 siguientes la terminacion
  ////   printf ("se ha recibido la senial de muerto hijo\n");
	pid = wait(&status);
	status = status >> 8;
	g_clPidStatus.SetPidStatus( pid,status);
////	printf ( "Se ha muerto un hijo proceso %d. Causa natural %d\n", pid, sig);

// volvemos a restaurar la funcion de la se7al de hijo
   signal(SIGCHLD, sigchild_died_handler);
}

int TerminateProcess (t_PROCESS ai_PID, int ai_iExitcode)
{
	int retcode;
	retcode=kill (ai_PID,  ai_iExitcode);
	return retcode;
	} */
