#ifndef _ANALIZADOR__HPP_                                                 
#define _ANALIZADOR__HPP_
#include "m4strobl.hpp" 
#include "cabecera.h"
#include "m4stringobl.hpp"
#include "hstring.h"
 
class M4ClassAnalizador           
  {
    protected:                      
      char *delimitadores;

#if !defined(_WINDOWS) && !defined(_UNIX)
      M4ClHString szBuffer;
#else
	  M4ClStringOld szBuffer;
#endif

      DWORD  size;
      DWORD  car;          
      UINT  sizepalabra;
      char *palabra;   // lo usaremos cada vez que necesitamos guardar una palabra , por ejemplo el valor de                         una propiedad
      char *UltimaLinea;    //tenemos la linea que estamos analizando en este momento
      BOOL bhaytexto;
    public:
      M4ClassAnalizador();
      ~M4ClassAnalizador(); 
      BOOL HayTexto();
      M4RETCODE SetFichero(const char *texto);
      M4RETCODE AppendTexto(char *);
      M4RETCODE Hastalacomilla(char **pal);
      M4RETCODE NextPalabra(char **pal);
      M4RETCODE NextCarNoNull(char * caracter);
      M4RETCODE CurrentCar(char * caracter );
      M4RETCODE SaltaLineasBlancas(); // me salta las lineas que haya en blanco y  me deja en "\n"
      M4RETCODE GetPosicionActual(DWORD *lPosicion,DWORD *lLinea,DWORD *lOffSet);
    protected:
      M4RETCODE Saltablancos();  
      
      
      
      
  };   



#endif   //_ANALIZADOR__HPP_     


