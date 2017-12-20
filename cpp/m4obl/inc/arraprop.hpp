#ifndef __ARRAY_DE_PROPIEDADES__
#define __ARRAY_DE_PROPIEDADES__

// esto lo que es la implementeacion de la clase array de propiedades
// usaremos claramente la clase propiedades


#include "propied.hpp"
#include "cabecera.h"
#include <stdlib.h> 





 
class M4ArrayPropiedades
{
 private:
     M4ClassPropiedad **pContPropiedades;     // puntero al array de propiedades 
     UINT numprops;                           // nos indica el numero de propiedades que tenemos almacenadas
     UINT ulibres;                            // numero huecos libres que tenemos todavia, ulibres + numprops = numero de celdas del array
     BOOL bordenado;                          // indica si el array esta ordenado o no 
 
 protected:     
     void PermutarPropiedad(int izq,int der); // En el array cambia de sitio dos propiedades
     void quicksort(int inf,int sup);         // funcion que me ordena el array de propiedades
     
 public:
// CONSTRUCTOR Y DESTRUCTOR
  M4ArrayPropiedades();
  ~M4ArrayPropiedades(); 
  
  
  char * ValPropn(UINT i);  // ME DA EL VALOR DE LA PROPIEDAD QUE ESTA EN LA POSICION I-ESIMA
  char * NomPropn(UINT i);  // NOMBRE I-ESIMO
  int TipoPropn(UINT i);    // TIPO I-ESIMO
  UINT NumPropiedades();    // EL NUMERO DE PROPIEDADES ALMACENADAS EN EL ARRAY
  BOOL APOrdenado();        // ME DICE SI EL ARRAY ESTÁ ORDENADO O NO
  
  M4RETCODE M4APAddNuevaPropiedad(char *szPropiedad,char *szValor,UINT uhident,int iTipoProp,void *pPadre); // añade un neva propiedad al array
  M4RETCODE M4APAddNuevaPropiedad(M4ClassPropiedad **ppropiedad,UINT *ppos,void *pPadre); // añade una celda vacia al array de propiedades devuelve el puentero a esa celda
                                                            // que es un objeto de tipo M4ClassPropiedad
  M4RETCODE M4APDelPropiedad(char *pszNomb);       // BORRA LA PROPIEDAD CON EL NOMBRE PSZNOMB, EN TEORIA NO PUEDE HABER DOS PROPIEDADES CON EL MISMO NOMBRE                                                     
  M4RETCODE M4APDelPropiedadn(UINT uPos);
  
  M4RETCODE M4APSetPropiedadn(UINT upos,char *szPropiedad,char *szValor,UINT uhident,int iTipoPrp); // RELLENA LA PROPIEDAD INDICADA CON LOS DATOS PASADOS
  M4RETCODE M4APSetValn(UINT uPos,char *pszValor,int iTipo); //Asigna valor a la propiedad uPos esima  
  M4RETCODE M4APSetVal(char *pszNomb,char *pszValor,int iTipo); //Asigna el valor a la propiedad con el nombre
  
  M4RETCODE M4APGetPropHand(UINT uhident,char **szProp);  // consigue el valor de la propiedad a partir del handle
  M4RETCODE M4APGetPropNom(char *szNombProp,char **szProp); // CONSIGUE EL VALOR DE LA PROIEDAD DANDO EL NOMBRE   
  M4RETCODE M4APGetTipoPropNom(char *szNombProp,int * iTipo); // CONSIGUE EL TIPO DE LA PROPIEDAD DANDO EL NOMBRE
  M4RETCODE M4APGetHandNom(char *szNombProp,UINT *uhident);   // CONSIGUE EL HANDLE DE LA PROPIEDAD DANDO EL NOMBRE
  
  M4RETCODE M4APGetNomn(UINT uPos,char  **szNom);
  M4RETCODE M4APGetValn(UINT uPos,char **szVal);
  M4RETCODE M4APGetHandn(UINT uPos,UINT *uhident); // consigue el handle del  I-ESIMO elemento del array
  M4RETCODE M4APGetTipon(UINT uPos,int *iTipo);
  M4ClassPropiedad *M4APGetpProp(UINT uPos);
  int M4APBuscarNom(char *Nombre); // DADO UN NOMBRE ME DICE LA POSICION QUE OCUPA EN EL ARRAY
  M4RETCODE M4APOrdenar();         // ORDENA EL ARRAY POR EL NOMBRE
  
  
};
 
   
        
#endif  // __ARRAY_DE_PROPIEDADES__     
      


