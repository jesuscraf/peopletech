#ifndef __CABECERA_H_
#define __CABECERA_H_
  
  #include <stdio.h>
  #include <stdlib.h>
  #include "m4strobl.hpp"   // Compatibilidad con UNIX 
  #include "m4winobl.hpp"   // Compatibilidad con UNIX 
  #include "busque.hpp"
  #include "resource.h"  
  #include "m4stringobl.hpp" 
  #include "hstring.h" 
 
// DEFINES DE DEPURACION
#define _DEPURACIONOBJ  

// MACROS PARA HACER DEBUGER
#define Msg(CADENA) MessageBox(GetActiveWindow(),CADENA,"DEBUGER",MB_OK)                       

// 
  // CONSTANTE  DE CADENAS
  #define STR_FIN_LINEA "\n"       
  
  #define STR_EXTENSION       "EXTENSION"
  #define STR_TASK            "TAREA"
  #define STR_NOMBRE_FICHERO  "NOMBRE_FICHERO"
  
  // CONSTANTES DE TIPO
 #define T_BLOQUE     1
 #define T_PROPIEDAD  2
 
 // Estos son los tipos que se usan
 #define T_PRP_INT    1
 #define T_PRP_STR    2
 #define T_PRP_FEC    3   

 
 #define NUMERO_SEGMENTOS       32
 #define NUM_ADD_HANDLES        1000 // maximo numero de handles que tengo la posibilidad de meter por segmento 
 #define NUM_ADD_PROP           5    // bloques de propiedades que voy añadiendo
 #define NUM_ADD_BLQ            5 
 
 #define MAX_TAM_IDENT          100   // maximo tamaño de los identificadores 
 #define MAX_TAM_PROP           5096 /// maximo del valor de un identificador
 #define MAX_TAM_BUFFER         65000  // tamaño de buffer del añalizador
 #define MAX_TAM_DELIMITADORES  10 // tamaño maximo de los delimitadores
 #define MAX_TAM_PATH           1000  
 #define MAX_CADENA_RECURSOS    255
 
// #define MAX_TAM_PROP       MAX_TAM_PROP  
 #define M4RETCODE int

 #define FINFICH  -1

 #define EXITO 1
 #define TRUE  1
 #define FALSE 0 

 


 
 // AÑADIR LAS CONSTANTES QUE ESTAN EL LA TABLA DE STRING
 
 #include "resource.h"    
 
 // contralar el error de sobreescritura

#endif


