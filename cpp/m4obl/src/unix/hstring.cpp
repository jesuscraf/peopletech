#if !defined(_WINDOWS) && !defined(_UNIX)

/*----------------------------------------------------------------------------------------------*/

/*---   INCLUDES PROPIOS   ---*/

#include "hstring.h"

/*---   INCLUDES MICROSOFT   ---*/
#include <stdlib.h> 
#include "m4winobl.hpp"
/*----------------------------------------------------------------------------------------------*/
M4ClHString::~M4ClHString ( )
{
  if ( phCadena != NULL )
      {
          GlobalUnlock (hCadena) ; 
             //MessageBox( GetFocus() , "No ha desalocado hasta cero", "~M4ClStringOld" ,MB_OK );
          m_iUsos--;
                         
          #ifdef _DEBUG 
           if ( m_iUsos != 0 )
             MessageBox ( GetFocus () ,  "Error 1 en hString" , "" , MB_OK );
          #endif

          GlobalFree   (hCadena);
          phCadena = NULL;
      }   
};
           

          


/*---   Función  'StrCpy' (Amiga de M4ClStringOld)   ---*/
DWORD OBLstrcpy (M4ClHString &destino, M4ClHString origen)
  {
    DWORD dwSize;
    // liberamos la memoria anteriormente guardada
    if (destino.phCadena ) 
      {
        GlobalUnlock(destino.hCadena);
        destino.m_iUsos-- ;

        #ifdef _DEBUG
                if ( destino.m_iUsos != 0 )  {
                      MessageBox ( GetFocus () , "Error hString 2" , "" , MB_OK | MB_ICONSTOP );
                     
                 }
        #endif

        GlobalFree(destino.hCadena);
      }

    if ( origen== NULL)
      {
        destino.phCadena = NULL;
        return 0;
      };                        
      
    dwSize = GlobalSize(origen.hCadena);
    
    // sabiendo el tamaño me alloco la memoria para poderlo meter dentro
    
    destino.hCadena = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_SHARE ,dwSize + 1);

    destino.phCadena = (char huge *)GlobalLock(destino.hCadena);

    destino.m_iUsos++;
    
    if (!destino.phCadena) return 0;
                
    hmemcpy(destino.phCadena,origen.phCadena,dwSize);
    
    destino.phCadena[dwSize] = 0 ;
    return dwSize;
  }

DWORD OBLstrcpy (M4ClHString & destino, const char * origen)
  {                                            
          UINT tam;
          
    if ((char huge *)destino.phCadena) 
      {
          GlobalUnlock(destino.hCadena);
          destino.m_iUsos--;
          #ifdef _DEBUG
               if ( destino.m_iUsos != 0 )  {
                MessageBox ( GetFocus () , " Errro hString 3" , "" , MB_OK | MB_ICONSTOP ); 
            }
          #endif
          GlobalFree(destino.hCadena);
       }
 
   if ( origen  == NULL)
      {
        destino.phCadena = NULL;
        return 0;
      };                      
      
    
    // sabiendo el tamaño me alloco la memoria para poderlo meter dentro
    tam = strlen(origen);
    
    destino.hCadena = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_SHARE,tam + 1);
    
    destino.phCadena = (char huge *)GlobalLock(destino.hCadena);
    destino.m_iUsos++;

    
    
    if (!destino.phCadena) return 0;
    
    hmemcpy(destino.phCadena,origen,tam);
    destino.phCadena[tam] = 0 ;
    return tam; 
    
    
    
  }
    
    
DWORD OBLstrncpy (char  * destino,char huge * origen,size_t tam)
  {
   
    // sabiendo el tamaño me alloco la memoria para poderlo meter dentro
	int iTamActual;
    
        
    if (origen == NULL ) return 0;   
    if (!(destino)) return 0;

#if !defined(_WINDOWS) && !defined(_UNIX)
	iTamActual = tam ;
#else
	iTamActual = strlen ( origen );
#endif
	
	tam =  ( iTamActual + 1  < tam ) ? iTamActual + 1  : tam;

    hmemcpy((char  huge *)(destino),origen ,tam);

    destino[ tam ] = 0 ;

    return tam;
 }
      
DWORD OBLstrcat(M4ClHString  & destino , const char *origen)
 {                      
    DWORD dwTam,dwTamDestino;
    HGLOBAL hglobal;
    
    
   
   
    if (destino.phCadena == NULL)
      { 
        dwTamDestino = 0 ;
        dwTam= strlen(origen);  
        destino.hCadena = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_SHARE , dwTam + 1);
        destino.phCadena =(char huge *) GlobalLock(destino.hCadena);
        destino.m_iUsos++;
      }
   else 
      {  
        // realoco la memoria que tengo
        dwTamDestino = strlen (destino ) ;
        dwTam = dwTamDestino + strlen( origen ); 
        
        
//        hglobal = GlobalReAlloc(destino.hCadena,dwTam + 1,GMEM_ZEROINIT |GMEM_MOVEABLE);
        
        destino.phCadena = NULL;
        GlobalUnlock ( destino.hCadena );
        destino.m_iUsos --;
        
        hglobal = GlobalAlloc (  GMEM_ZEROINIT | GMEM_FIXED | GMEM_SHARE  , dwTam + 1 );
        
        char huge *phPointer ;
        
        phPointer = (char huge * )GlobalLock( hglobal  );
        
        destino.phCadena = (char huge *)GlobalLock( destino.hCadena );
        
        hmemcpy ( (char huge *)phPointer , (char huge * ) destino.phCadena , dwTamDestino );
        
        GlobalUnlock ( destino.hCadena );
        
        #ifdef _DEBUG
           if ( destino.m_iUsos != 0 )
              MessageBox ( GetFocus ()  , "Hola Cadena ", "error", MB_OK ); 
        #endif
        GlobalFree ( destino.hCadena );
        
        destino.hCadena = hglobal ;
        destino.phCadena = phPointer ;
        destino.m_iUsos++;
        
      //  destino.hCadena = hglobal;
      // destino.phCadena =(char huge *) GlobalLock(destino.hCadena);
      
      };              
      
    hmemcpy(&destino.phCadena[dwTamDestino],origen,strlen(origen));
    destino.phCadena[dwTam]= 0;
    return dwTam;
 }      

DWORD OBLstrlen(M4ClHString  & origen)
 {                                               
   DWORD  dwLong;
   DWORD   dwCont = 0;
   if (!origen.phCadena) return 0;
   dwLong = GlobalSize(origen.hCadena);
   dwCont = dwLong;
   while (dwCont > 0  && origen.phCadena[dwCont - 1] == 0 ) 
       dwCont--; 
   return dwCont;
 }



#endif		// no en Win32 y UNIX
