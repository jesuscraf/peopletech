#ifndef __M4STRINGOBL_HPP__
#define __M4STRINGOBL_HPP__

/*---   INCLUDES   ---*/

#include "m4winobl.hpp"
#include "m4strobl.hpp"

#include <stdlib.h>  
#include <stdarg.h>

/*---   DEFINES   ---*/

#define getdir(obj,t) ((t)(obj).getbuffer())

/*---   PROTOTIPOS DE FUNCIONES EXTERNAS   ---*/

extern BOOL isempty      ( const char * );
extern BOOL isnull       ( const char * );
extern BOOL IsNumeric    ( const char * );

#ifndef _AIX
extern size_t __strlen   ( const char * string );
extern char * __strcpy   ( char * destino, const char * origen );
extern char * __strncat  ( char * destino, const char * origen, size_t count );
extern char * __strcat   ( char * destino, const char * origen );

extern int __stricmp     ( const char * string1, const char * string2 );
extern int __strcmp      ( const char * string1, const char * string2 );

#endif


/*---   FIN DE LA CABECERA DEL FICHERO   ---*/




/*----------
|
|         DECLARACIÓN DE LA CLASE  'M4ClStringOld'
|
|----------*/





class M4ClStringOld
{
private:
     char * m_pszbuffer;

public:
     
     //  Constructor.
     M4ClStringOld () { this -> m_pszbuffer = NULL; };

     //  Destructor
     ~M4ClStringOld ();
     
    // Operadores  de conversión
    //operator const unsigned char * () { return (const unsigned char *)m_pszbuffer; }
    //operator       unsigned char * () { return (      unsigned char *)m_pszbuffer; }
    operator const          char * () { return (const          char *)m_pszbuffer; }
    operator                char * () { return (               char *)m_pszbuffer; }
     
     //  Sobrecarga del operador de indexación.
     const char operator [] (int index) ;
                                
     //  Sobrecarga del operador de desplazamiento.
     const char * operator << (int index) ;


     //  Métodos de la clase
     void * getbuffer (void);
     void erase (void);
     size_t strlen (void);
     int trim (void);
     int strcatleft (const char * pszstring, int ilongmaxima = -1);
     int strcat (const char * pszstring, int ilongmaxima = -1);
     int strcatmiddle (const char * pszleft, const char * pszright);
     int strcat (int inumstrings, ...);
     int strcpy (const char * pszstring, int ilongmaxima = -1);
     int strcpy (int inumstrings, ...);
     int strcpyf (char * a_pszFormato,...);
     int change (int istartreplaced, int ilongreplaced, const char * pszreplacer);
     int strchange (const char * pszfind, const char * pszreplace);
     int strcount (const char * pszfind);
     int strreplace (const char * pszreplaced, const char * pszreplacer);
     int stralloc(int a_iNumbytes);     

     //  Funciones amigas.
     friend int StrCpy(M4ClStringOld & destino, const char * origen);
     friend int StrCmp (M4ClStringOld & cadena1 ,M4ClStringOld & cadena2);
     
     // EDU
     void fill(char);
     int strcmp(const char *);
};

#endif //__M4STRING_HPP__


