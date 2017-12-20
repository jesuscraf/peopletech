#include "arraprop.hpp"

 
M4ArrayPropiedades::M4ArrayPropiedades()
 {
   bordenado = FALSE;
   pContPropiedades = NULL;
   numprops = 0;  
   ulibres=0;
 };  
   

M4ArrayPropiedades::~M4ArrayPropiedades()
{ 
/*
    UINT cont=0;
    

    for (cont=0;cont < numprops + ulibres;cont++)
       if ( pContPropiedades[cont] ) delete pContPropiedades[cont];    // si no es null borro la propiedad                                 
    delete []pContPropiedades;
    pContPropiedades=NULL;
    numprops = 0;
    bordenado = 0;
*/

    UINT    ui ;
    
    if( pContPropiedades != NULL )
    {
        for( ui = 0 ; ui < numprops + ulibres ; ui++ )
        {
            if( pContPropiedades[ ui ] != NULL )
            {
                delete( pContPropiedades[ ui ] ) ;
                pContPropiedades[ ui ] = NULL ;
            }
        }
    
        delete( pContPropiedades ) ;
        pContPropiedades = NULL ;
    }
    
    numprops = 0 ;
    bordenado = 0 ;

}   
  
  

M4RETCODE M4ArrayPropiedades::M4APAddNuevaPropiedad(char *szPropiedad,char *szValor,UINT uhident,int iTipoPrp,void *pPadre)
  {   
    M4ClassPropiedad **ppAux=NULL;   
//    UINT i;
    M4RETCODE retdev;
    /* añade una propiedad al array y devuelve asignadole los valores que le hemos pasado como parametro
           -1  en caso  de que el primer elemento lo haya asignado mal 
           -3  no ha habido memoria suficente  */
    if (ulibres==0)  
     {             
       // no hay ninguna celda libre para asignarles valores
       // me alloco mas celdas
       ppAux  = new M4ClassPropiedad*[numprops + NUM_ADD_PROP];
       if (ppAux == NULL) return -3 ; // no a hecho el new en condiciones
       // asigno las propiedades que ya estan asignadas


/*
       for (i=0;i<numprops;i++)
          ppAux[i]=pContPropiedades[i];         
       // las que no estan asignadas las preparo   
       for (i=numprops;i<numprops+NUM_ADD_PROP;i++)
          {
            ppAux[i] = new M4ClassPropiedad;
            // le asigno padre porque se quien es 
            ppAux[i]->M4CPSetPadre(pPadre);
          }; 
       // actualizo el contenedor   
       delete []pContPropiedades;
       pContPropiedades=ppAux; 
*/

        if( numprops > 0 )
        {
            memcpy( ppAux, pContPropiedades, sizeof( M4ClassPropiedad* ) * numprops ) ;
        }
        memset( ppAux + numprops, NULL, sizeof( M4ClassPropiedad* ) * NUM_ADD_PROP ) ;
        delete( pContPropiedades ) ;
        pContPropiedades = ppAux ;

       ulibres = NUM_ADD_PROP;
      };
    
    pContPropiedades[ numprops ] = new M4ClassPropiedad ;
    pContPropiedades[ numprops ]->M4CPSetPadre( pPadre ) ;

    numprops++; 
    ulibres--; 
    retdev=pContPropiedades[numprops-1]->M4CPSetPropiedad(szPropiedad,szValor,uhident,iTipoPrp);
    if (bordenado && (retdev==EXITO)) bordenado=FALSE; // pongo ordenado a false
    return retdev;
} 

 
 M4RETCODE M4ArrayPropiedades::M4APAddNuevaPropiedad(M4ClassPropiedad **ppropiedad,UINT *ppos,void *pPadre)
  {   
    M4ClassPropiedad **ppAux;
//    UINT i;   
    /* añade una propiedad al array y devuelve el puentero a esa propiedad libre y la posicion que ocupa en 
       en array
           -1  en caso  de que el primer elemento lo haya asignado mal 
           -3  no ha habido memoria suficente  */
    
    // ANALOGO AL PROCEDIMIENTO ANTERIOR
    if (ulibres==0)
     {         
       ppAux  = new M4ClassPropiedad*[numprops + NUM_ADD_PROP];
       if (ppAux == NULL) return -3 ; // no a hecho el new en condiciones

/*
       for (i=0;i<numprops;i++)
          ppAux[i]=pContPropiedades[i];
       for (i=numprops;i<numprops+NUM_ADD_PROP;i++)
           {
            ppAux[i] = new M4ClassPropiedad;
            ppAux[i] -> M4CPSetPadre(pPadre);
           };           
       delete []pContPropiedades;    
       pContPropiedades=ppAux;
*/

        if( numprops > 0 )
        {
            memcpy( ppAux, pContPropiedades, sizeof( M4ClassPropiedad* ) * numprops ) ;
        }
        memset( ppAux + numprops, NULL, sizeof( M4ClassPropiedad* ) * NUM_ADD_PROP ) ;
        delete( pContPropiedades ) ;
        pContPropiedades = ppAux ;

       ulibres = NUM_ADD_PROP;
      }; 
      
    pContPropiedades[ numprops ] = new M4ClassPropiedad ;
    pContPropiedades[ numprops ]->M4CPSetPadre( pPadre ) ;

    numprops++; 
    ulibres--; 
    // devuelvo la posicion donde se guardara la primera celda libre
    *ppos=numprops-1;                                            
    // puntero de la propiedad libre
    *ppropiedad = pContPropiedades[numprops-1];
    bordenado=FALSE;
    return EXITO;
  }  
  
 M4RETCODE M4ArrayPropiedades::M4APDelPropiedad(char *pszNombre)
  {   
    // borra una propiedad en el array y reajusta el array para que no haya huecos libres en el
    int iPosicion;
    UINT  uCont;           
    void  *pPadrecito;
    
    // calculo la posicion en array del nombre que quiero borrar
    iPosicion = M4APBuscarNom(pszNombre); 
    if ( iPosicion < 0 ) return IDS_OBJ_NOT_FOUND;
    // he encontrado la posicion que queria borrar
    // consigo el padre antes
    pContPropiedades[ iPosicion ] -> M4CPGetPadre( & pPadrecito ); 
    delete pContPropiedades[iPosicion];
    //una vez borrado reajusto todas la propiedades del array
    for (uCont=iPosicion;uCont<numprops-1;uCont++)
    {
      pContPropiedades[uCont]= pContPropiedades[uCont+1];  // el ultimo que modifico es el numprops-1
    }

/*
    // ahora el ultimo lo alloco
    pContPropiedades[numprops - 1] = new M4ClassPropiedad;   
    pContPropiedades[numprops - 1 ] -> M4CPSetPadre( pPadrecito ); 
*/
    pContPropiedades[ numprops - 1 ] = NULL ;   


    // ya tengo uno más libre y una propiedad menos
    numprops--;            
    ulibres++;            
    return EXITO;
    
  }; 
 
 M4RETCODE M4ArrayPropiedades::M4APDelPropiedadn(UINT uPos)
  {
    UINT uCont;
    void  *pPadrecito;
    
                             
    pContPropiedades[uPos] -> M4CPGetPadre( & pPadrecito ); 
    delete pContPropiedades[uPos];
    
    //una vez borrado reajusto todas la propiedades del array
    for (uCont=uPos;uCont<numprops-1;uCont++)
    {
      pContPropiedades[uCont]= pContPropiedades[uCont+1];  // el ultimo que modifico es el numprops-1
    }
    
/*
    // ahora el ultimo lo alloco
    pContPropiedades[numprops - 1 ] = new M4ClassPropiedad;
     
    // Asigno el padre a la propiedad que me acabo de crear
    pContPropiedades[numprops - 1 ] -> M4CPSetPadre( pPadrecito ); 
*/
    pContPropiedades[ numprops - 1 ] = NULL ;   

    // ya tengo uno más libre y una propiedad menos
    numprops--;                                             
    ulibres++;            
    
    return EXITO;
  } 
       
 M4RETCODE M4ArrayPropiedades::M4APSetPropiedadn(UINT upos,char *szPropiedad,char *szValor,UINT uhident,int iTipoPrp)
  {                                
   M4RETCODE retdev;
   if (upos>numprops-1)  return IDS_RTE_APROP_POS;
   // se añade la propiedad en la posicion especificada
   retdev = pContPropiedades[upos]->M4CPSetPropiedad(szPropiedad,szValor,uhident,iTipoPrp);
   // en caso de que no se añada bien se borra la propiedad
   if (retdev != EXITO) M4APDelPropiedadn(upos);
   return retdev; 
  }; 
  

 M4RETCODE M4ArrayPropiedades::M4APSetValn(UINT uPos,char *pszValor,int iTipoPrp)
  {
   if (uPos>numprops-1 ) return IDS_RTE_APROP_POS;
   return pContPropiedades[uPos]->M4CPSetVal(pszValor,iTipoPrp); 
  };
  
 M4RETCODE M4ArrayPropiedades::M4APSetVal(char *pszNomb,char *pszValor,int iTipo)
  { 
    int iPosicion;
    
    if (!pszNomb) return IDS_RTE_APROP_STR_NULL;
    iPosicion = M4APBuscarNom(pszNomb); 
    if ( iPosicion < 0 ) return IDS_OBJ_NOT_FOUND;
    // tengo la posicion de la propiedad que quiero modificar
    return M4APSetValn(iPosicion,pszValor,iTipo);
  }; 
  
           
 M4RETCODE M4ArrayPropiedades::M4APGetPropHand(UINT uhident,char **szProp)
  { 
    /* Esta funcion devuelve el valor de la propiedad habiendo dado el handle
       de esta propiedad 
       devuelve  -13 si el array esta vacio
               EXITO  */
    
    UINT haux=0;
    UINT cont=0;
    char *szaux;
    BOOL encontrado =FALSE;    
    M4RETCODE retdev;
    
    if (numprops==0) return -13;
    cont=0;   
    *szProp = NULL;
    while (!encontrado && cont<numprops)
     {
       retdev=pContPropiedades[cont]->M4CPGetHandle(&haux);
       if (retdev<0) return retdev;
       if (haux==uhident)
         { 
          retdev=pContPropiedades[cont]->M4CPGetPropiedad(&szaux);
          *szProp = szaux;
          return retdev;
         };
       cont++; 
     }
    return -4;  // cuando llega aqui es que no lo ha encontrado ;
   };    
            
 

char *M4ArrayPropiedades::ValPropn(UINT i)
   {        
     /* devuelve la cadena que corresponde a la propiedad nesima
        si el indice esta mal o bien no hay 
                ninguna propiedad -> devuelve NULL
                indice superior -> devuelve NULL */
                
     UINT auxi;
     char *szprop;
    
     if ( (numprops==0) && (i>(numprops-1)) ) return NULL; // error en el indice
     else auxi=i;
     pContPropiedades[auxi]->M4CPGetPropiedad(&szprop);
     return szprop;
   };
 
 
 char *M4ArrayPropiedades::NomPropn(UINT i)
    {        
     /* devuelve la cadena que corresponde a la propiedad nesima
        si el indice esta mal o bien no hay 
                ninguna propiedad -> devuelve NULL
                indice superior -> devuelve NULL */
                
     UINT auxi;
     char *sznomprop;
    
     if ( (numprops==0) && (i>(numprops-1)) ) return NULL; // error en el indice
     else auxi=i;
     pContPropiedades[auxi]->M4CPGetNomPropiedad(&sznomprop);
     return sznomprop;
   };    
     
 int M4ArrayPropiedades::TipoPropn(UINT i)
    {        
     UINT auxi;
     int iTipo;
    
     if ( (numprops==0) && (i>(numprops-1)) ) return T_PRP_STR; // error en el indice
     else auxi=i;
     pContPropiedades[auxi]->M4CPGetTipoPropiedad(&iTipo);
     return iTipo;
   };    
     
 UINT M4ArrayPropiedades::NumPropiedades()
    {
      return numprops;
    };                 
    
    
 BOOL M4ArrayPropiedades::APOrdenado()
    {
      return bordenado;
    };    
       
 M4RETCODE M4ArrayPropiedades::M4APOrdenar()    
   {                        
     /* Ordena el array de propiedades por  el nombre de  la Propiedad
           -1     -> no hay propiedades 
           EXITO  -> se ha producido con exito la operacion*/
    
     if (numprops==0) return -1;
     quicksort(0,numprops-1);
     bordenado=TRUE; 
     return EXITO; 
     
   };                             
   
 M4RETCODE M4ArrayPropiedades::M4APGetPropNom(char *szNombProp,char **szProp)
   {  
      /*    devuelve EXITO -> todo bien
                     -1    -> no se ha encontrado este nombre y se ha devuelto nulo
                     -2    -> ho hay elementos en el array */
   
      UINT indice;
      
      if (numprops==0) return IDS_RTE_APROP_VACIO; //no hay propiedades
      indice=M4APBuscarNom(szNombProp);
      if (indice == (UINT) -1)
        {
          *szProp=NULL;
          return IDS_OBJ_NOT_FOUND;
        }
      else
          return pContPropiedades[indice]->M4CPGetPropiedad(&(*szProp));
   };   
   
M4RETCODE M4ArrayPropiedades::M4APGetTipoPropNom(char *szNombProp,int * iTipo)
   {  
      /*    devuelve EXITO -> todo bien
                     -1    -> no se ha encontrado este nombre y se ha devuelto nulo
                     -2    -> ho hay elementos en el array */
   
      UINT indice;
       // compruebo parametros
      if (numprops==0) return IDS_RTE_APROP_VACIO; //no hay propiedades
      // busco el indice de la propiedad con el nombre
      indice=M4APBuscarNom(szNombProp);
      // si no lo encuentra devuelve error en caso contrario se calcula el tipo de la proiedad
      if (indice == (UINT) -1) 
          return IDS_OBJ_NOT_FOUND;
      else
          return pContPropiedades[indice]->M4CPGetTipoPropiedad(&(*iTipo));
   };   
   
    

M4RETCODE M4ArrayPropiedades::M4APGetHandNom(char *szNombProp,UINT *uhident)
  {  
     /*    devuelve EXITO -> todo bien
                     -1    -> no se ha encontrado este nombre y se ha devuelto nulo
                     -2    -> ho hay elementos en el array */
      
   
      int indice;
      
      if (numprops==0) return IDS_OBJ_NOT_FOUND;    //no hay propiedades
      indice=M4APBuscarNom(szNombProp);
      if (indice<0)
        {
          return IDS_OBJ_NOT_FOUND;
        }
      else                                
          return pContPropiedades[indice]->M4CPGetHandle(&(*uhident));
   };
   
      
// consigue e handle de la propiedad i-esima

M4RETCODE M4ArrayPropiedades::M4APGetNomn(UINT uPos,char  **szNom)
   {
     if (uPos>numprops-1)  return IDS_RTE_APROP_POS;
     return pContPropiedades[uPos]->M4CPGetNomPropiedad(&(*szNom));
   }
 
M4RETCODE M4ArrayPropiedades::M4APGetValn(UINT uPos,char **szVal)
   {
     if (uPos>numprops-1)  return IDS_RTE_APROP_POS;
     return pContPropiedades[uPos]->M4CPGetPropiedad(&(*szVal));
   }

      
M4RETCODE M4ArrayPropiedades::M4APGetHandn(UINT uPos,UINT *uhident)  
   {                                                          
   
     if (uPos>numprops-1)  return IDS_RTE_APROP_POS;
     return pContPropiedades[uPos]->M4CPGetHandle(&(*uhident));
                       
   }
   
       
M4RETCODE M4ArrayPropiedades::M4APGetTipon(UINT uPos,int  *iTipo)
   {
    if (uPos>numprops-1)  return IDS_RTE_APROP_POS;
    return pContPropiedades[uPos]->M4CPGetTipoPropiedad(&(*iTipo));   
   }   


M4ClassPropiedad *M4ArrayPropiedades::M4APGetpProp(UINT uPos)    
   {
     if (uPos >= numprops) return NULL;
     return pContPropiedades[uPos];
   } 
      
   
/******************************************************************************************/
/******************************************************************************************/
/*                                      FUNCIONES EN EL BLOQUE PROTECTED                                                                                            */
/******************************************************************************************/
/******************************************************************************************/
                                        
 
 
 void M4ArrayPropiedades::PermutarPropiedad(int izq,int der)
  {
    M4ClassPropiedad *aux;
       
    aux  = pContPropiedades[izq];
    pContPropiedades[izq]=pContPropiedades[der];
    pContPropiedades[der]=aux;
  }; 
  
 void M4ArrayPropiedades::quicksort(int inf,int sup)
  {
    int izq,der;
    M4ClassPropiedad *mitad;
    
    izq = inf;
    der = sup;
    // consigo la propiedad de la mitad
    mitad = pContPropiedades[(int)(izq+der)/2];
    
    do
     {
       while (ComparaProp(&(*pContPropiedades[izq]),&(*mitad))<0 && izq<sup) izq++;
       while (ComparaProp(&(*mitad),&(*pContPropiedades[der]))<0 && der>inf) der--;
       if (izq<=der)
        {
          // permutamos los registros
          PermutarPropiedad(izq,der);
          izq++;
          der--;
        }
      } while (izq<=der);
     
     if (inf<der) quicksort(inf,der);
     if (izq<sup) quicksort(izq,sup);
  };
          
    
    
 int M4ArrayPropiedades::M4APBuscarNom(char *pszNombre)
   {
     int mitad,izq,der,comparacion;
     char *szmitad;
     
     // si devuelve >=0 -> encontrado y devuelve el indice de donde està
     // si devuelve -1 -> no encontrado 
     // si devuelve -2 -> la funcion ha ordenado mal
     if (!bordenado) 
        if (M4APOrdenar()!=EXITO) return -2;
     izq=0;
     der=numprops-1;
     mitad = (int)(der+izq)/2;
     do
      {             
        mitad = (int)(der+izq)/2;
        pContPropiedades[mitad]->M4CPGetNomPropiedad(&szmitad);
        comparacion=strcmp(pszNombre,szmitad);
        if (comparacion==0) { return mitad;} // he encontrado el que buscaba devuelvo el numero
        else
          if(comparacion<0) der=mitad-1;
          else izq=mitad+1;
      } while (izq<=der);
      
      return -1;
   };



