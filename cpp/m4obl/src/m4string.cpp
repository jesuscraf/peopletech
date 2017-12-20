/*---   INCLUDES   ---*/

#include "m4stringobl.hpp"
#include <stdio.h>


//J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
char *sol_malloc_char(size_t Size);
char *sol_realloc_char(void *pointer, size_t Size);
char *sol_calloc_char(size_t NumberOfElements, size_t ElementSize);
void sol_free(void *Pointer);
#endif
//END

/*---   FIN DE LA CABECERA DEL FICHERO   ---*/


                                       

/*----------
|
|         DEFINICIÓN DE LOS MÉTODOS DE LA CLASE  'M4ClStringOld'
|
|-----*/

M4ClStringOld::~M4ClStringOld()
 {
      if ( this -> m_pszbuffer != NULL ) 
      {
//J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
          sol_free(this -> m_pszbuffer);
#else
           free (this -> m_pszbuffer);
#endif
	  //END
           this -> m_pszbuffer = NULL;
      }     
 }     




/*---   Función  'getbuffer'   ---*/

/*****
     FUNCIÓN: getbuffer
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN: 
     RETORNA: puntero 'void' al buffer usado por el string.
*****/

void * M4ClStringOld::getbuffer ()
{
     return (void *) this -> m_pszbuffer;
}
/*---   Fin de la función  'getbuffer'   ---*/









/*---   Función  'erase'   ---*/

/*****
     FUNCIÓN: erase
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN: borra el string, poniéndolo a NULL
     RETORNA: <no retorna nada>
*****/

void M4ClStringOld::erase ()
{
     if ( this -> m_pszbuffer != NULL ) 
     {
          free (this -> m_pszbuffer);
          this -> m_pszbuffer = NULL;
     }     
}
/*---   Fin de la función  'erase'   ---*/









/*---   Función  'strlen'   ---*/

/*****
     FUNCIÓN: strlen
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN:
     RETORNA: 
          n : la longitud del string.
          0 : si el string está vacío o es NULL.
*****/

size_t M4ClStringOld::strlen ()
{
     if ( this -> m_pszbuffer == NULL ) return 0U;
     return ::strlen (this -> m_pszbuffer);
}
/*---   Fin de la función  'strlen'   ---*/









/*---   Función  'trim'   ---*/

/*****
     FUNCIÓN: trim
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN:
     RETORNA: 
          0 : correcto.
         -1 : sin memoria suficiente.
*****/

int M4ClStringOld::trim ()
{
     const char * pszaux;
     int ilongitud;
     
     if ( (this -> m_pszbuffer == NULL) || ( *(this->m_pszbuffer) == '\x0') ) return 0;
     pszaux = this -> m_pszbuffer;
     while ( * pszaux == ' ' ) pszaux ++;
     ilongitud = (int)(::strcspn (pszaux, " "));
     return this -> strcpy (pszaux, ilongitud);
}
/*---   Fin de la función  'strlen'   ---*/









/*---   Función  'strcatleft'   ---*/

/*****
     FUNCIÓN: strcatleft
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN:
     RETORNA:
          0 : todo bien
         -1 : sin memoria.
*****/

int M4ClStringOld::strcatleft (const char * pszstring, int ilongmaxima)
{
     int	ilongstring;
	 int	ilongtotal;
     char *	psztemp;
     
     //  Si la cadena a agregar es NULL o una cadena vacía, no hacemos nada.
     if ( (pszstring == NULL) || (* pszstring == '\x0') ) return 0;
     if ( ilongmaxima == 0 ) return 0;
     
     //  Calculamos la longitud de la cadena a añadir.
     ilongstring = (int)(::strlen (pszstring));
     ilongmaxima = (ilongmaxima < 0) ? ilongstring : ilongmaxima;
     ilongmaxima = (ilongmaxima < ilongstring) ? ilongmaxima : ilongstring;
     
     //  Calculamos la longitud de la cadena resultante.
     ilongtotal = ilongmaxima + (int)(this -> strlen ());
     
     //  Guardamos memoria para la nueva cadena.
     //J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
     psztemp = sol_calloc_char (ilongtotal + 1, sizeof (char));
#else
     psztemp = (char *) calloc (ilongtotal + 1, sizeof (char));
#endif
    //END
     if ( psztemp == NULL ) return -1;
     
     //  Concatenamos las cadenas.
     * psztemp = '\x0';
     ::strncat (psztemp, pszstring, ilongmaxima);
     if ( this -> m_pszbuffer != NULL ) ::strcat (psztemp, this -> m_pszbuffer);
     
     //  Liberamos la memoria guardada por el string y le asignamos el nuevo buffer.
     this -> erase ();
     this -> m_pszbuffer = psztemp;

     //  Todo ha ido bien.
     return 0;
}
/*---   Fin de la función  'strcatleft'   ---*/









/*---   Función  'strcat'   ---*/

/*****
     FUNCIÓN: strcat
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN:
     RETORNA:
          -1 : sin memoria.
           0 : correcto.
*****/

int M4ClStringOld::strcat (const char * pszstring, int ilongmaxima)
{
     int ilongstring;
     int ilongtotal;
     char * psztemp;
     
     //  Si la cadena a agregar es NULL o una cadena vacía, no hacemos nada.
     if ( (pszstring == NULL) || (* pszstring == '\x0') ) return 0;
     if ( ilongmaxima == 0 ) return 0;
     
     //  Calculamos la longitud de la cadena a añadir.
     ilongstring = (int)(::strlen (pszstring));
     ilongmaxima = (ilongmaxima < 0) ? ilongstring : ilongmaxima;
     ilongmaxima = (ilongmaxima < ilongstring) ? ilongmaxima : ilongstring;
     
     //  Calculamos la longitud de la cadena resultante.
     ilongtotal = ilongmaxima + (int)(this -> strlen ());
     
     //  Guardamos memoria para la nueva cadena.
     //J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
     psztemp = sol_realloc_char(this -> m_pszbuffer, (ilongtotal + 1) * sizeof (char));
#else
     psztemp = (char *) realloc (this -> m_pszbuffer, (ilongtotal + 1) * sizeof (char));
#endif
     //END

     if ( psztemp == NULL ) return -1;
     
     //  Concatenamos las cadenas.
     if ( this -> m_pszbuffer == NULL ) * psztemp = '\x0';
     this -> m_pszbuffer = psztemp;
     ::strncat (this -> m_pszbuffer, pszstring, ilongmaxima);
     return 0;
}
/*---   Fin de la función  'strcat'   ---*/









/*---   Función  'strcatmiddle'   ---*/

/*****
     FUNCIÓN: strcatmiddle
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN:
     PARÁMETROS:
     RETORNA:    
          -1 : sin memoria.
           0 : correcto.
*****/

int M4ClStringOld::strcatmiddle (const char * pszleft, const char * pszright)
{
     size_t longitud_nueva = size_t (0);
     size_t longitud_extra = size_t (0);
     char * psztemp = NULL;
     
     //  Si ambas cadenas son NULL o vacías no se hace nada.
     if ( isempty (pszleft) && isempty (pszright) ) return 0;
     
     //  Calculamos la longitud de la cadena a añadir.
     if ( pszleft != NULL ) longitud_extra += ::strlen (pszleft);
     if ( pszright != NULL ) longitud_extra += ::strlen (pszright);
     
     //  Calculamos la longitud de la cadena resultante.
     longitud_nueva = longitud_extra + this -> strlen ();
     
     //  Guardamos memoria para la nueva cadena.
     //J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
     psztemp = sol_calloc_char(longitud_nueva + 1, sizeof (char));
#else
     psztemp = (char *) calloc (longitud_nueva + 1, sizeof (char));
#endif
     //END
     if ( psztemp == NULL ) return -1;
     
     //  Concatenamos las cadenas.
     * psztemp = '\x0';
     if ( pszleft != NULL ) ::strcat (psztemp, pszleft);
     if ( this -> m_pszbuffer != NULL ) ::strcat (psztemp, this -> m_pszbuffer);
     if ( pszright != NULL ) ::strcat (psztemp, pszright);
     
     //  Liberamos la memoria guardada para el string y cambiamos el buffer.
     this -> erase ();
     this -> m_pszbuffer = psztemp;
     
     //  Todo ha ido bien.
     return 0;
}
/*---   Fin de la función  'strcatmiddle'   ---*/









/*---   Función  'strcat'   ---*/

/*****
     FUNCIÓN: strcat
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN:
     PARÁMETROS:
     RETORNA:    
          -1 : sin memoria.
           0 : correcto.
*****/

int M4ClStringOld::strcat (int inumstrings, ...)
{
     M4ClStringOld strtemp;
     va_list argumentos;
     const char * pszstring;
     int iretcode = 0;
     
     va_start (argumentos, inumstrings);
     for (int i = 0; i < inumstrings; i++)
     {
          pszstring = va_arg (argumentos, char *);
          if ( iretcode == 0 ) iretcode = strtemp.strcat(pszstring);
     }
     va_end (argumentos);
     
     if ( iretcode == 0 ) iretcode = this -> strcat (strtemp);
     return iretcode;
}
/*---   Fin de la función  'strcat'   ---*/









/*---   Función  'strcpy'   ---*/

/*****
     FUNCIÓN: strcpy
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN: copia la cadena pasada al string. Si pasamos NULL actúa como 'erase'.
     PARÁMETROS:
          pszstring   : cadena que deseamos copiar.
          ilongmaxima : número máximo de caracteres a copiar. Si es negativo, copiamos toda
                        la cadena
     RETORNA:                        
          -1 :sin memoria.
           0 : correcto.
*****/

int M4ClStringOld::strcpy (const char * pszstring, int ilongmaxima)
{
     int ilongstring;
     char * psztemp;

     //  Si el origen es un puntero NULL, ponemos a NULL el destino.
     if ( pszstring == NULL ) 
     {
          this -> erase ();
          return 0;
     }     
          
     //  Calculamos la longitud de la cadena a añadir.
     ilongstring = (int)(::strlen (pszstring));
     ilongmaxima = ( ilongmaxima < 0 ) ? ilongstring : ilongmaxima;
     ilongmaxima = ( ilongmaxima < ilongstring ) ? ilongmaxima : ilongstring;
     
     //  Guardamos memoria para copiar la cadena.
     //J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
     psztemp = sol_calloc_char( (ilongmaxima + 1 ) , sizeof (char));
#else
     psztemp = (char *) calloc ( (ilongmaxima + 1 ) , sizeof (char));
#endif
     //END
     
    // #ifdef MEM_DEBUG
     
    //           unsigned long ulCont = dbgMemTotalCount();
               
    //           if ( ulCont == 887 ) {
               
   //              ulCont = ulCont;
   //            }
     
     
  //   #endif
     
     if ( psztemp == NULL ) return -1;
     
     //  Copiamos la cadena origen en auxiliar.
     
	 /*
     * psztemp = '\x0';     
	 ::strncat (psztemp, pszstring, ilongmaxima);
	 */
	 ::strncpy (psztemp, pszstring, ilongmaxima);
	 psztemp[ilongmaxima]='\0';
     
     //  Asignamos el auxiliar al destino.
     this -> erase ();
     this -> m_pszbuffer = psztemp;
     return 0;
}
/*---   Fin de la función  'strcpy'   ---*/









/*---   Función  'strcpy'   ---*/

/*****
     FUNCIÓN: strcpy
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN: copia la cadena pasada al string. Si pasamos NULL actúa como 'erase'.
     PARÁMETROS:
          pszstring   : cadena que deseamos copiar.
          ilongmaxima : número máximo de caracteres a copiar. Si es negativo, copiamos toda
                        la cadena
     RETORNA:                        
          -1 : sin memoria.
           0 : correcto.
*****/

int M4ClStringOld::strcpy (int inumstrings, ...)
{
     M4ClStringOld strtemp;
     va_list argumentos;
     const char * pszstring;
     int iretcode = 0;
     
     va_start (argumentos, inumstrings);
     for (int i = 0; i < inumstrings; i++)
     {
          pszstring = va_arg (argumentos, const char *);
          if ( iretcode == 0 ) iretcode = strtemp.strcat (pszstring);
     }
     va_end (argumentos);
     
     if ( iretcode == 0 ) iretcode = this -> strcpy (strtemp);
     return iretcode;
}
/*---   Fin de la función  'strcpy'   ---*/

/*****
     FUNCIÓN: strcpyf
     CLASE: M4ClStringOld.
     TIPO: publica
     DESCRIPCIÓN: Hace una copia con formato en la cacharra
     PARÁMETROS:
          strformato -> nos da el formato de lo que queremos copiar
          ... todos los formatos
     RETORNA:                        
          -1 : sin memoria.
           0 : correcto.
*****/  

int M4ClStringOld::strcpyf( char * a_pszFormato,...)
 {
    
    va_list argumentos;
    char *pBuffer = NULL;
    int     iIncremento = 256, iTam = iIncremento,iEscrito = -1;
    int iRetcode;
    
    if ( a_pszFormato == NULL ) 
       {
         this -> erase(); 
         return 0;
       }
    
    va_start ( argumentos , a_pszFormato );

   #if defined(_WINDOWS) 
    
         while ( iEscrito == -1 )
          {              
            // alloco espacio en para escribirlo
	    //J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
            if (( pBuffer = sol_realloc_char( pBuffer, iTam ) ) == NULL )
#else
            if (( pBuffer = (char * )realloc( pBuffer, iTam ) ) == NULL )
#endif
	    //END
               return -1;
            iEscrito = _vsnprintf ( pBuffer , iTam, a_pszFormato, argumentos );
            if ( iEscrito == -1 )
                iTam += iIncremento;
          }            
   #elif defined(_UNIX)              
                   // alloco un buffer lo suficientemente grande para escribir lo que se me ha pasado
	    //J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
	 if( ( pBuffer = sol_malloc_char( 0xFFF0 )  )  == NULL ) return -1;
#else
                   if ( ( pBuffer = (char * )malloc ( 0xFFF0 )  )  == NULL ) return -1; 
#endif
	 //END
                        
                   // Hago la copia en el buffer sin tener en cuenta el tamaño del buffer
                   iEscrito = vsprintf ( pBuffer , a_pszFormato, argumentos );
   #else
     
          #error No platform defined...                
              
   #endif      
    
    va_end ( argumentos );
    iRetcode = this -> strcpy ( pBuffer );
    // J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
    sol_free( pBuffer );
#else
    free( pBuffer );
#endif
	  //END
     
    return iRetcode;       
 } 










/*---   Función  'strreplace'   ---*/

/*****
     FUNCIÓN: strreplace
     CLASE: M4ClStringOld.
     TIPO: pública.
     DESCRIPCIÓN:
     RETORNA:        
          -1 : sin memoria.
           0 : correcto.
*****/

int M4ClStringOld::strreplace (const char * pszreplaced, const char * pszreplacer)
{
     M4ClStringOld strauxiliar;
     const char * pszauxiliar;
     int istartreplaced, ilongreplaced;
     
     //  Comprobamos los parámetros.
     if ( isempty (pszreplaced) ) return 0;
     
     //  Buscamos la cadena que vamos a cambiar.
     pszauxiliar = strstr (this -> m_pszbuffer, pszreplaced);
     if ( pszauxiliar == NULL ) return 0;
     // istartreplaced = ( int )((long)(strstr (this -> m_pszbuffer, pszreplaced)) - (long)(this -> m_pszbuffer));
	 istartreplaced = (int)((strstr(this->m_pszbuffer, pszreplaced)) - (this->m_pszbuffer));
     
     //  Calculamos la longitud de la cadena que sustituirá.
     ilongreplaced = (int)(::strlen (pszreplaced));
     
     //  Reemplazamos.
     strauxiliar.erase ();
     if ( strauxiliar.strcat (this -> m_pszbuffer, istartreplaced) != 0 ) return -1;
     if ( ! isempty (pszreplacer) )
          if ( strauxiliar.strcat (pszreplacer) != 0 ) return -1;
     if ( strauxiliar.strcat ((this -> m_pszbuffer) + istartreplaced + ilongreplaced) != 0 ) return -1;
     if ( this -> strcpy (strauxiliar) != 0 ) return -1;
     
     //  Todo ha ido bien.
     return 0;
}
/*---   Fin de la función  'strreplace'   ---*/
        
        
        
/*---   Función  'strreplace'   ---*/

/*****
     FUNCIÓN: stralloc         
     CLASE: M4ClStringOld.
     TIPO: pública.
     DESCRIPCIÓN: Crea una cadena de esto bytes borrando la anterior si la habia.
     RETORNA:        
          -1 : sin memoria.
           0 : correcto.
*****/

int M4ClStringOld::stralloc(int a_iNumBytes)
  {
                                               
      
      
                                               
      // En caso de que haya algo lo elimino
      if  ( this -> m_pszbuffer != NULL ) {
          
	//J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
          sol_free( this -> m_pszbuffer );
#else
          free ( this -> m_pszbuffer );     
#endif
	  //END
           
          this -> m_pszbuffer = NULL ;
      }    
          
      // en caso de que sea negativo devuelvo null
      if ( a_iNumBytes < 0 )
        {
          this -> m_pszbuffer = NULL;
          return 0;
        
         } 
      
      // Alloco el numero de byte soliciatdos
	//J.A.Sanchez 20/3/97
#ifdef _SOL_JAS
      this -> m_pszbuffer = sol_malloc_char(a_iNumBytes * sizeof ( char ) );
#else
      this -> m_pszbuffer = ( char * )malloc( a_iNumBytes * sizeof ( char ) );
#endif
      //END
      
       
      if ( this -> m_pszbuffer == NULL ) return -1;

      // lo hago cero inicializada
      memset ( this -> m_pszbuffer , 0 , a_iNumBytes);
      
      // devuelvo exito
      return 0;
          
  }






/*---   Función  'change'   ---*/

/*****
     FUNCIÓN: change
     CLASE: M4ClStringOld.
     TIPO: privada
     DESCRIPCIÓN:
     RETORNA:        
          -1 : sin memoria.
           0 : correcto.
*****/

int M4ClStringOld::change (int istartreplaced, int ilongreplaced, const char * pszreplacer)
{
     M4ClStringOld auxiliar;
     int iretcode = 0;
     
     auxiliar.erase ();
     if ( iretcode == 0 ) iretcode = auxiliar.strcat (this -> m_pszbuffer, istartreplaced);
     if ( iretcode == 0 ) 
          if ( ! isempty (pszreplacer) )
               iretcode = auxiliar.strcat (pszreplacer);
     if ( iretcode == 0 ) iretcode = auxiliar.strcat ((this -> m_pszbuffer) + (istartreplaced + ilongreplaced));
     if ( iretcode == 0 ) iretcode = this -> strcpy (auxiliar);
     return iretcode;
}
/*---   Fin de la función  'change'   ---*/









/*---   Función  'strchange'   ---*/

/*****
     FUNCIÓN: strchange
     CLASE: M4ClStringOld.
     TIPO: pública.
     DESCRIPCIÓN:
     RETORNA:
          -1 : sin memoria.
           n : número de veces se que se cambiado 'pszfind' por 'pszreplace'.
*****/

int M4ClStringOld::strchange (const char * pszfind, const char * pszreplace)
{
     char * pszinicio;    
     int iretcode = 0;
     int inicio = 0;
     int count = 0;
     
     if ( isempty (this -> m_pszbuffer) || isempty (pszfind) ) return 0;
     
     while ( (pszinicio = strstr(this -> m_pszbuffer + inicio, pszfind)) != NULL )
     {
          inicio = int(pszinicio - (this -> m_pszbuffer));
          iretcode = this -> change (inicio, (int)(::strlen (pszfind)), pszreplace);
          if ( iretcode != 0 ) return iretcode;
          inicio += (int)(::strlen (pszreplace));
          count ++;
     }     
     return count;
}
/*---   Fin de la función  'strchange'   ---*/









/*---   Función  'strcount'   ---*/

/*****
     FUNCIÓN: strcount
     CLASE: M4ClStringOld.
     TIPO: pública.
     DESCRIPCIÓN:
     RETORNA:
          n : número de veces que se ha encontrado 'pszfind' en el string.
*****/

int M4ClStringOld::strcount (const char * pszfind)
{
     int count = 0;
     const char * pszinicio = this -> m_pszbuffer;
     
     if ( isempty (pszinicio) ) return 0;
     if ( isempty (pszfind) ) return 0;
     while ( (pszinicio = ::strstr (pszinicio, pszfind)) != NULL ) 
     {
          pszinicio += ::strlen(pszfind);
          count ++;    
     }     
     return count;
}
/*---   Fin de la función  'strcount'   ---*/


void M4ClStringOld::fill (char c)
{
    memset (m_pszbuffer, c, strlen());
}

int M4ClStringOld::strcmp (const char *p)
{                   
    int ires;
    
    if (m_pszbuffer == NULL) {
        if (p == NULL) {
            ires = 0;
        }
        else {
            ires = -1;
        }
    } 
    else {
        if (p == NULL) {
            ires = +1;
        }
        else {        
            ires = ::strcmp (m_pszbuffer, p);
        }
    }
    
    return ires;
}






/*---   FUNCIONES AMIGAS DE LAS CLASE M4ClStringOld  ---------------------*/

int StrCpy(M4ClStringOld & destino, const char * origen)
{
     return destino.strcpy(origen);
}

int StrCmp (M4ClStringOld & cadena1 ,M4ClStringOld & cadena2)
{
     return __strcmp(cadena1,cadena2) ;
}









/*----------
|
|         DEFINICIÓN DE FUNCIONES DE MANEJO DE CADENAS DE CARACTERES
|
|----------*/




/*---   Función  'isspace'   ---*/
/* NO SE USA
int isspace (const char * pszstring)
{
     if ( pszstring == NULL ) return TRUE;
     if ( * pszstring == '\x0' ) return TRUE;
     if ( strspn (pszstring, " ") == strlen (pszstring) ) return TRUE;
     return FALSE;
}
*/
/*---   Fin de la función  'isspace'   ---*/
     
     







/*---   Función  'isempty'   ---*/

BOOL isempty (const char * pszstring)
{
     return (pszstring == NULL) ? TRUE : ((* pszstring == '\x0') ? TRUE : FALSE);
}
/*---   Fin de la función  'isempty'   ---*/






/*---   Función  'isnull'   ---*/

BOOL isnull (const char * pszstring)
{
     return (pszstring == NULL) ? TRUE : FALSE;
}
/*---   Fin de la función  'isnull'   ---*/



/*---   Función  'IsNumeric'   ---*/

BOOL IsNumeric ( const char * ai_pszString )
{
        
        char * szCadenaNumeros = "0123456789";
   
   if ( ai_pszString == NULL ) {
    
        return FALSE;
   }
   
   return ( strspn ( ai_pszString , szCadenaNumeros ) == strlen (szCadenaNumeros) ) ? TRUE : FALSE;      

}

/*---   Fin de la función  'IsNumeric'   ---*/



#ifndef _AIX


/*---   Función  '__strlen'   ---*/

size_t __strlen (const char * string)
{
     if ( string == NULL ) return (size_t)0;
     return strlen (string);
}
/*---   Fin de la función  '__strlen'   ---*/









/*---   Función  '__strcpy'   ---*/

char * __strcpy (char * destino, const char * origen)
{
     if ( destino == NULL ) return NULL;
     if ( origen == NULL ) return strcpy (destino, "");
     return strcpy (destino, origen);
}
/*---   Fin de la función  '__strcpy'   ---*/









/*---   Función  '__strncat'   ---*/

char * __strncat (char * destino, const char * origen, size_t count)
{
     if ( destino == NULL ) return NULL;
     if ( origen == NULL ) return strncat (destino, "", count);
     return strncat (destino, origen, count);
}
/*---   Fin de la función  '__strncat'   ---*/









/*---   Función  '__strcat'   ---*/

char * __strcat (char * destino, const char * origen)
{
     if ( destino == NULL ) return NULL;
     if ( origen == NULL ) return strcat (destino, "");
     return strcat (destino, origen);
}
/*---   Fin de la función  '__strcat'   ---*/









/*---   Función  '__stricmp'   ---*/
          
int __stricmp (const char * string1, const char * string2)
{
     if ( string1 == NULL && string2 == NULL ) return 0;
     if ( string1 == NULL ) return -1;
     if ( string2 == NULL ) return +1;
     return stricmp (string1, string2);
}
/*---   Fin de la función  '__stricmp'   ---*/









/*---   Función  '__strcmp'   ---*/

int __strcmp (const char * string1, const char * string2)
{
     if ( string1 == NULL && string2 == NULL ) return 0;
     if ( string1 == NULL ) return -1;
     if ( string2 == NULL ) return +1;
     return strcmp (string1, string2);
}
/*---   Fin de la función  '__strcmp'   ---*/

#endif // _AIX                                           

     const char M4ClStringOld::operator [] (int index)
     {                            
          if ( this -> m_pszbuffer == NULL ) return '\x0';
          if ( (index < 0) || (index > int (this -> strlen ())) ) return '\x0';
          return (this -> m_pszbuffer) [index];
     }
                                
     //  Sobrecarga del operador de desplazamiento.
     const char * M4ClStringOld::operator << (int index)
     {
          if ( this -> m_pszbuffer == NULL ) return NULL;
          if ( index < 0) return this -> m_pszbuffer;
          if ( index > int (this -> strlen ()) ) return (this -> m_pszbuffer) + this -> strlen ();
          return (this -> m_pszbuffer) + index;
     }


