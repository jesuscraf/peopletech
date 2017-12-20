/**********************************************************************************************                           
 **********************************************************************************************                     
                        CLASE DE ARRAY DE BLOQUES 
 **********************************************************************************************                           
 ***********************************************************************************************/                     

 
#include "def_bloq.hpp"  
#include "cabecera.h"



M4ArrayBloques::M4ArrayBloques()
   { 
     numbloq=0;
     bordenbloque=FALSE;
     ContBloques=NULL;
     uBlqLibres=0;
   };           
   
 
M4ArrayBloques::~M4ArrayBloques()
{
/*
    UINT i;  
                 
    for( i=0 ; i < numbloq + uBlqLibres ; i++ )               
     if ( ContBloques[i] )
       {
        if ( ContBloques[i] != NULL ) { 
        
               delete ContBloques[i]; 
        }       
        this -> ContBloques[i] = NULL ;
       }
      
    numbloq = 0 ;  
    if ( ContBloques != NULL ) delete [] ContBloques;       
*/

    UINT    ui ;
    
    
    if( ContBloques != NULL )
    {
        for( ui = 0 ; ui < numbloq + uBlqLibres ; ui++ )
        {
            if( ContBloques[ ui ] != NULL )
            {
                delete( ContBloques[ ui ] ) ;
                ContBloques[ ui ] = NULL ;
            }
        }
        delete( ContBloques ) ;
        ContBloques = NULL ;
    }

    numbloq = 0 ;
    uBlqLibres = 0 ;  
}  

   
 M4RETCODE M4ArrayBloques::M4ABAddNuevoBlq(char *szHins,char *szClass,UINT hident,M4ClassBloque *pAnterior,M4ClassBloque **pBloque)
   {   
      /* devuelve codigos de error :
           IDS_RTE_NO_MEMORIA -> falta memoria
          */ 
               
      M4RETCODE retdev;
      M4ClassBloque **ppAux;
//      UINT i;
      
      if (uBlqLibres==0)   
         {  
           ppAux = new M4ClassBloque *[numbloq + NUM_ADD_BLQ];
           if (ppAux==NULL) return IDS_RTE_NO_MEMORIA;

/*
           for (i=0;i < numbloq;i++)
               {
                ppAux[i]=ContBloques[i];
                ContBloques[i]=NULL;
               };
           for (i=numbloq;i<numbloq + NUM_ADD_BLQ;i++)
               {
                ppAux[i] = new (M4ClassBloque);    
                retdev = ppAux[i]->M4CBSetPadre(pAnterior);
                if (retdev!=EXITO) return retdev;
               } 
           delete []ContBloques;
           ContBloques=ppAux;
*/

            if( numbloq > 0 )
            {
                memcpy( ppAux, ContBloques, sizeof( M4ClassBloque* ) * numbloq ) ;
            }
            memset( ppAux + numbloq, NULL, sizeof( M4ClassBloque* ) * NUM_ADD_BLQ ) ;
            delete( ContBloques ) ;
            ContBloques = ppAux ;
            
           uBlqLibres = NUM_ADD_BLQ;
           ppAux=NULL;  
         }   

        ContBloques[ numbloq ] = new M4ClassBloque ;    
        retdev = ContBloques[ numbloq ]->M4CBSetPadre( pAnterior ) ;
        if( retdev != EXITO )
        {
            return retdev ;
        }

      // ya tengo una celda libre que es la ultima
      // ahora la asigno su valor
      numbloq++;
      uBlqLibres--;
      bordenbloque=FALSE;
      return ContBloques[numbloq-1]->M4CBSetBloque(szHins,szClass,hident,&(*pBloque));
 
  };
      
M4RETCODE M4ArrayBloques::M4ABAddNuevoBlq(M4ClassBloque *pAnterior,M4ClassBloque **pBloque)
   {
     // tenque que crearme un bloque y pasar el puntero
     /* pasmos como parametro al padre y devuelve un puntero al blqoue asignado*/
      
      M4ClassBloque **ppAux;
//      UINT i;
      M4RETCODE retdev;
      
      if (uBlqLibres==0)   
         {  
           ppAux = new M4ClassBloque *[numbloq + NUM_ADD_BLQ];
           if (ppAux==NULL) return IDS_RTE_NO_MEMORIA;

/*
           for (i=0;i < numbloq;i++)
              {
                ppAux[i]=ContBloques[i];
                ContBloques[i]=NULL;
              };
           for (i=numbloq;i<numbloq + NUM_ADD_BLQ;i++)
              {
                ppAux[i] = new (M4ClassBloque); 
                retdev = ppAux[i]->M4CBSetPadre(pAnterior);
                if (retdev!=EXITO) return retdev;   
              };
           delete []ContBloques;
           ContBloques=ppAux;
*/
            if( numbloq > 0 )
            {
                memcpy( ppAux, ContBloques, sizeof( M4ClassBloque* ) * numbloq ) ;
            }
            memset( ppAux + numbloq, NULL, sizeof( M4ClassBloque* ) * NUM_ADD_BLQ ) ;
            delete( ContBloques ) ;
            ContBloques = ppAux ;

           uBlqLibres = NUM_ADD_BLQ;
           ppAux=NULL;  
         };   

        ContBloques[ numbloq ] = new M4ClassBloque ;    
        retdev = ContBloques[ numbloq ]->M4CBSetPadre( pAnterior ) ;
        if( retdev != EXITO )
        {
            return retdev ;
        }

      // ya tengo una celda libre que es la ultima
      // ahora la asigno su valor
      numbloq++;
      uBlqLibres--;
      bordenbloque=FALSE;
      *pBloque = ContBloques[numbloq-1];
      return EXITO;
     };   
             
M4RETCODE M4ArrayBloques::M4ABlDelBlq(char *pszClass,char *pszHinst)
     {    
       // Dado una hinstancia , la funcion busca esta hinstancia , la borra y 
       //   recoloca todo el array de bloques para que no quede ningun hueco libre
       
       int iPosicion;
       UINT uCont;
       M4ClassBloque *pPadrecito;
       M4RETCODE retdev;
       
       iPosicion = M4ABlBuscar(pszClass,pszHinst);
       if (iPosicion < 0 ) return IDS_RTE_ABLQ_NO_FOUND; 
       // con la posicion del bloque miro si esta vacio
       if (!ContBloques[iPosicion]->M4CBVacio()) return IDS_RTE_ABLQ_NO_VACIO;
       // Borro la entrada porque esta vacion el bloque                
       
       retdev = ContBloques[ iPosicion ] -> M4CBGetPadre(&pPadrecito);
       if (retdev != EXITO) return retdev;                                   
       
       delete ContBloques[iPosicion];

       for (uCont=iPosicion;uCont<numbloq - 1 ;uCont++)
       {
           ContBloques[uCont] = ContBloques[uCont + 1 ];
       }
       
/*
       ContBloques[ numbloq - 1 ] = new M4ClassBloque ;         
       // asigno el padre
       ContBloques[ numbloq - 1 ] -> M4CBSetPadre( pPadrecito );
*/       
       ContBloques[ numbloq - 1 ] = NULL ;         
 
       uBlqLibres++;
       numbloq--; 
      
       return EXITO;
       
     };    

M4RETCODE M4ArrayBloques::M4ABlDelBlqn(char *pszClass,char *pszHinst)
     {    
       // Dado una hinstancia , la funcion busca esta hinstancia , la borra y 
       // recoloca todo el array de bloques para que no quede ningun hueco libre
       // y despues asigno el padre otra vez
       
       // La deferencia con respecto a la otra funcion de borrar un bloque en un 
       // array es que la otra comprueba que no haya elementos metidos
       
       int iPosicion;
       UINT uCont;          
       M4ClassBloque *pPadrecito;
       M4RETCODE retdev;
       
       iPosicion = M4ABlBuscar(pszClass,pszHinst);
       if (iPosicion < 0 ) return IDS_RTE_ABLQ_NO_FOUND; 
       
        retdev = ContBloques[ iPosicion ] -> M4CBGetPadre(&pPadrecito);
        if (retdev != EXITO) return retdev;                            
        
        delete ContBloques[ iPosicion ] ;
        
        for (uCont=iPosicion;uCont<numbloq - 1 ;uCont++)
        {
           ContBloques[uCont] = ContBloques[uCont + 1 ];
        }
           
/*        ContBloques[  numbloq - 1 ]  = new  M4ClassBloque ;
        // Se asigna el padre el 
        ContBloques[ numbloq - 1 ] -> M4CBSetPadre( pPadrecito );
*/
       ContBloques[ numbloq - 1 ] = NULL ;         
       
        uBlqLibres++;
        numbloq--; 
      
        return EXITO;
       
     };    
     
M4RETCODE M4ArrayBloques::M4ABlGetHandpBlq(M4ClassBloque *pBloque,UINT *hIdent)
   {
     return pBloque->M4CBGetHandle(hIdent);
   };
   

M4RETCODE M4ArrayBloques::M4ABlGetHinstpBlq(M4ClassBloque *pBloque,char **szHinst)
   {
     return pBloque->M4CBGetHinst(&(*szHinst));
   };

M4RETCODE M4ArrayBloques::M4ABlGetClasspBlq(M4ClassBloque *pBloque,char **szClass)
   {
     return pBloque->M4CBGetClass(&(*szClass));
   };           
   
M4RETCODE M4ArrayBloques::M4ABlOrdenar()
  {
                           
     /* Ordena el array de propiedades por  el nombre de  la Propiedad
           2120     -> no hay Bloques 
           EXITO  -> se ha producido con exito la operacion*/
    
     if (numbloq==0) return 2120;
     M4ABlQuicksort(0,numbloq-1);
     bordenbloque=TRUE; 
     return EXITO;
  };                                                 
 
   
  
int M4ArrayBloques::M4ABlBuscarHinst(char *hHinst)
   {
     int mitad,izq,der,comparacion;
     char *szmitad;  
     M4RETCODE retdev;
     
                  
     // si devuelve >=0 -> encontrado y devuelve el indice de donde està
     // si -1           -> no encontrado
     if (!bordenbloque)     
       {
        retdev = M4ABlOrdenar();
        if (retdev!=EXITO) return -2;
       };
    
     izq=0;
     der=numbloq-1;
     mitad = (int)(der+izq)/2;
     do
      {             
        mitad = (int)(der+izq)/2;
        ContBloques[mitad]->M4CBGetHinst(&szmitad);
        comparacion=strcmp(hHinst,szmitad);
        if (comparacion==0) { return mitad;} // he encontrado el que buscaba devuelvo el numero
        else
          if(comparacion<0) der=mitad-1;
          else izq=mitad+1;
      } while (izq<=der);
      
      return -1;
   };             
          
          
   int M4ArrayBloques::M4ABlBuscar(char *szClase,char *szInstancia)
   {
     // esta funcion devuelve -1 si no encuntra clase.instacia y
     // el indice en caso de que lo encontrara     
     int iIndex;
     char *pClase , *pInstancia;            
     BOOL bEncontrada=FALSE;
     
     iIndex = M4ABlBuscarHinst(szInstancia);
     if (iIndex < 0  ) return -1;
     ContBloques[iIndex]->M4CBGetHinst(&pInstancia);
     while (iIndex > 0  &&  strcmp(szInstancia,pInstancia)== 0) 
            { 
              iIndex--;
              ContBloques[iIndex]->M4CBGetHinst(&pInstancia);
            }; 
     if ( strcmp(szInstancia,pInstancia) != 0 ) iIndex++;               
     // tengo el indice de la primera de las hinstancia szHinst
     // tengo que buscar la que coincida la clase
     ContBloques[iIndex]->M4CBGetHinst(&pInstancia);
     ContBloques[iIndex]->M4CBGetClass(&pClase);     
     
//EDU     
     //ANTES
     //while ( strcmp(szInstancia,pInstancia)==0 && strcmp(szClase,pClase)!=0 && (UINT)iIndex < numbloq)
     
     //DESPUES:
     while ( strcmp(szInstancia,pInstancia)==0 && strcmp(szClase,pClase)!=0 && (UINT)iIndex < (numbloq-1))
     
       { 
         iIndex++;
         ContBloques[iIndex]->M4CBGetHinst(&pInstancia);
         ContBloques[iIndex]->M4CBGetClass(&pClase);
       }
     if (strcmp(szInstancia,pInstancia)==0 && strcmp(szClase,pClase)==0 ) 
         return iIndex;
     else
         return -1;
   
   };

    
  M4RETCODE M4ArrayBloques::M4ABlGetpBlqCI(char * szClase,char *szInstancia,M4ClassBloque **blq)
   {
    int retdev;    
    retdev=M4ABlBuscar(szClase,szInstancia);
    if (retdev<0) return IDS_RTE_ABLQ_NO_FOUND;
    *blq= ContBloques[retdev];
    return EXITO;  
   }  
   
  M4RETCODE M4ArrayBloques::M4ABlGetpBlqn(UINT uPos,M4ClassBloque **pBlq)
   {
    
    if (uPos >= numbloq) return IDS_RTE_ABLQ_INDEX_OVERFLOW;
    *pBlq = ContBloques[uPos];
    return EXITO;
   
   }   
   

   M4RETCODE M4ArrayBloques::M4ABlGetHand(char *szClass,char *szInstancia,UINT *hIdent)
    {       
     int retdev;
    
     retdev=M4ABlBuscar(szClass,szInstancia);
     if (retdev<0) return IDS_RTE_ABLQ_NO_FOUND;
     ContBloques[retdev]->M4CBGetHandle(&(*hIdent));
     return EXITO;
      
    };   
    

   UINT M4ArrayBloques::NumBloques()    
    { 
        return numbloq;
    };
    
   BOOL M4ArrayBloques::M4ABlOrdenado()
    {
      return bordenbloque;
    }
    
    
    M4ClassBloque *M4ArrayBloques::PtrBloquen(UINT i)
    {                 
      UINT auxi=0;
                   
      auxi=i;
      if (i >= numbloq) auxi=numbloq; 
      return ContBloques[auxi];
    };             
    
  
         
/****************************************************************************************
                    MIEMBROS PROTECTED DEL ARRAY DE BLOQUES                              
 ***************************************************************************************/                            
         
 void M4ArrayBloques::PermutarBloque(int izq,int der)
  {
    M4ClassBloque *aux;
       
    aux = ContBloques[izq];
    ContBloques[izq]=ContBloques[der];
    ContBloques[der]=aux;
  }; 
       
  
   void M4ArrayBloques::M4ABlQuicksort(int inf,int sup)
  {
    int izq,der;
    M4ClassBloque *mitad;
    
    izq = inf;
    der = sup;
    // consigo la propiedad de la mitad
    mitad = ContBloques[(int)(izq+der)/2];
    
    do
     {
       while (ComparaBloque(ContBloques[izq],mitad)<0 && izq<sup) izq++;
       while (ComparaBloque(mitad,ContBloques[der])<0 && der>inf) der--;
       if (izq<=der)
        {
          // permutamos los registros
          PermutarBloque(izq,der);
          izq++;
          der--;
        }
      } while (izq<=der);
     
     if (inf<der) M4ABlQuicksort(inf,der);
     if (izq<sup) M4ABlQuicksort(izq,sup);
  };                
  
  int ComparaBloque(M4ClassBloque *ele1,M4ClassBloque *ele2)
     {
        char *strele1,*strele2;
        
        ele1->M4CBGetHinst(&strele1);
        ele2->M4CBGetHinst(&strele2);
        return strcmp(strele1,strele2);
     };      
     
    
/**********************************************************************************************                           
 **********************************************************************************************                     
                        CLASE DE LOS BLOQUES 
 **********************************************************************************************                           
 ***********************************************************************************************/                     


                     
M4ClassBloque::M4ClassBloque()
   {
     //HinstBl=NULL;
     //ClassBl=NULL;  
     pPadre = NULL;
     hId = 0;
     bdefinido = FALSE;
   };


M4ClassBloque::~M4ClassBloque()
  {                
    
    pPadre = NULL;
    hId = 0;
    bdefinido = FALSE;
  };
      

BOOL M4ClassBloque::M4CBVacio() // me dice si un bloque esta vacio de propiedades y de bloques
  {  
    UINT uNumProp;
    
    uNumProp = NumPropiedades();
    return ( uNumProp == 0 && numbloq==0);
  };
        
   
M4RETCODE M4ClassBloque::M4CBSetBloque(char *szClass,char *szHinst,UINT hIdent,M4ClassBloque **pBloq)
    {
       /* Asigna los valores a una propiedad y los modifica en caso de modificar
       libera la memoria anteriormente allocada valores de vueltos
           2090   -> error handle mal definido
           2091   -> cadena no validad        
           IDS_RET_STR_SIN_CARACTERES -> se intenta asignar una cadena vacia
           2093   -> No ha habido memoria para la cadena de un bloque nuevo
          EXITO -> se ha introducido las cadenas correctamente */
       size_t len; 
                       
       if (szHinst==NULL || szClass==NULL) return 2091;
       if (strlen(szHinst)==0 || strlen(szClass)==0) return IDS_RET_STR_SIN_CARACTERES;
       if (hIdent==0) return 2090;
      // HinstBl = new( char[strlen(szHinst)+1]);
     //  ClassBl = new( char[strlen(szClass)+1]);
       strcpy(HinstBl,szHinst);
       strcpy(ClassBl,szClass);
       len = strlen(szHinst);
       HinstBl[strlen(szHinst)]= 0;
       ClassBl[strlen(szClass)]= 0;
       hId=hIdent; 
       // compruebo que ha habido memoria para las cadenas de los nombres
       if (HinstBl==NULL || ClassBl==NULL ) return 2093;
       bdefinido = TRUE; 
       *pBloq= this;
       return EXITO;
    
    };

M4RETCODE M4ClassBloque::M4CBSetPadre(M4ClassBloque *pFather)
    {
     pPadre = pFather;
     return EXITO;
    }; 
    
    
M4RETCODE M4ClassBloque::M4CBGetPadre(M4ClassBloque **pFather)
    {
     *pFather = pPadre;
     return EXITO;
    };       
          
       
M4RETCODE M4ClassBloque::M4CBGetHinst(char **Hinst)
 {
   /*devuelve 
      -1  -> cuando no esta definida la clase todavia
   EXITO  -> operacion correcta*/
   
   if (!bdefinido) return -1;
   *Hinst=HinstBl;
   return EXITO;
 };   
 
M4RETCODE M4ClassBloque::M4CBSetHinst(char *a_pszHinst)
 {                   
          char szBuffer[MAX_TAM_IDENT];
 

   if ( !bdefinido || a_pszHinst == NULL ) return -1;     
   
   if ( strlen ( a_pszHinst ) >= MAX_TAM_IDENT ) return -2 ;
   
   strcpy( szBuffer , a_pszHinst );
   
   strcpy( HinstBl , strupr ( szBuffer) );
   
   return EXITO;
 } 
      
       
             
M4RETCODE M4ClassBloque::M4CBGetClass(char **Class)
 { 
   /*devuelve 
      -1  -> cuando no esta definida la clase todavia
   EXITO  -> operacion correcta*/  
  
   if (!bdefinido) return -1;
   *Class=ClassBl;
   return EXITO;
 };        
      
       
M4RETCODE M4ClassBloque::M4CBGetHandle(UINT *hIdent)
  { 
   /*devuelve 
      -1  -> cuando no esta definida la clase todavia
   EXITO  -> operacion correcta*/  
  
   if (!bdefinido) return -1;
   *hIdent=hId; 
   return EXITO;
 };            

/*** esta funcion add un bloque en el array de bloques hijos **/ 
 
M4RETCODE M4ClassBloque::M4CBAddBlqArray(char *szHins,char *szClass,UINT hident,M4ClassBloque **pBloque)
  { 
    /*devueve 
        2110 -> se intenta add un bloque  en un array de un bloque no definido 
        EXITO-> en caso contrario
        */
        
    if (!bdefinido) return 2110;
    return M4ABAddNuevoBlq(szHins,szClass,hident,this,&(*pBloque));
  }   
  
                                                     
  M4RETCODE M4ClassBloque::M4CBGetBlqPath(char *pPath,M4ClassBloque **pBlqOut)
  {
    char szInst[MAX_TAM_IDENT];
    char szClase[MAX_TAM_IDENT];
    char *pPunto;
    size_t iLong;            // saber la longitud del path pasado
    M4RETCODE retdev;   
    M4ClassBloque *pBlq,*pBlqAux;  // punteros con los que voy a navegar en el arbol
    /** JMM MODIFICACION **/
    M4ClStringOld strPath;

    if (!pPath) return IDS_RTE_BLQ_NO_PATH;  
    
    StrCpy( strPath , pPath);
    
    pBlq = this;
    //iLong = strlen(pPath); 
    iLong = strlen( strPath );
    if (iLong > 0 )    
      {  
          //pPunto = pPath;
          pPunto = ( char *)strPath;
          
          pPunto = strtok(pPunto,".");
          while (pPunto)
            { 
              
              if (pPunto==NULL) return IDS_RTE_PATH;
              strcpy(szClase,pPunto);
              
              pPunto = strtok(NULL,".");
              if (pPunto==NULL) return IDS_RTE_PATH;
              strcpy(szInst,pPunto);
             
              pPunto = strtok(NULL,".");         
             
             // en este punto tengo actualizado szInst y szClase
              retdev =  pBlq -> M4ABlGetpBlqCI(szClase,szInst,&pBlqAux);
              if  (retdev!=EXITO ) return IDS_RTE_BLQ_NO_PATH;
              pBlq = pBlqAux;
           }
        
          *pBlqOut = pBlq;
      }                                                                        
    else
        { // si la longitud es cero quiere decir que le estoy pasando el bloque actual
          *pBlqOut = this;
        };         
   // tengo que buscar el ultimo camino
   return EXITO; 
 };
             
 /**********************************************************************************************                           
 ***********************************************************************************************/                            
 
#if !defined(_WINDOWS) && !defined(_UNIX)
	M4RETCODE WriteRecursivo(int nivel,M4ClassBloque *ptbloque,M4ClHString & pszBufferOut)      
#else
	M4RETCODE WriteRecursivo(int nivel,M4ClassBloque *ptbloque,M4ClStringOld & pszBufferOut)      
#endif
     {   
       
      // tenemos un buffer lo suficientemente grande para conseguir que pueda caber el fichero 
      // y que su primer caracter sea el cero
     
      UINT contador;         
      int cont,retdev;  // variable entera que me sirve para recivir codigos y como contador
      char *strnombre,*strvalor,*strtabuladores;
      M4ClassBloque *puntero;
      char *pszLineaAux; 
      int iTipo;                                  
      
      BOOL bOrdenado;
      M4ClassBloque * pPadre;  
      // ESTA VARIABLE ES BASURA DE DEBUG
      UINT uHandBlqAux;                        
      UINT uHandPrpAux;
      
      if  (!ptbloque) return IDS_RTE_OBJ_NULL;
      if (!ptbloque->M4CBDefinido()) return IDS_RTE_BLQ_ERROR_ESCRITURA;
      
      strtabuladores=new char[nivel+1];
      
      /* preparo la cadena de tabuladores que va antes de cada propiedad */
      for (cont=0;cont<nivel;cont++) strtabuladores[cont] = 9;
      strtabuladores[nivel]='\0';

      // preparamos  la cabecera
      retdev=ptbloque->M4CBGetClass(&strnombre);  
      if (retdev != EXITO) return IDS_RTE_OBJ_NULL;

      retdev=ptbloque->M4CBGetHinst(&strvalor);
      if (retdev != EXITO) return IDS_RTE_OBJ_NULL;               
      
      pszLineaAux = new char[MAX_TAM_IDENT + MAX_TAM_PROP + 1000];
      if (!pszLineaAux) return  IDS_RTE_NO_MEMORIA;
      
      ptbloque -> M4CBGetPadre ( & pPadre ) ;     
      // borrar esto
      ptbloque -> M4CBGetHandle (& uHandBlqAux);
      sprintf( pszLineaAux ,"%sBEGIN %s %s\r\n",strtabuladores,strnombre,strvalor);

#if !defined(_WINDOWS) && !defined(_UNIX)
	  OBLstrcat(pszBufferOut, pszLineaAux);
#else
	  pszBufferOut.strcat( pszLineaAux);
#endif
             
      // ahora me imprimo las propiedades
      contador=ptbloque->NumPropiedades();
      bOrdenado = ptbloque ->APOrdenado();
      if (contador>0 && !bOrdenado) ptbloque->M4APOrdenar(); // pregunto si hay propiedades y si estan ordenadas
      for (cont=0;cont<(int)contador;cont++)
        {         
          strnombre = ptbloque->NomPropn(cont);
          
          strvalor  = ptbloque->ValPropn(cont);
          
          iTipo = ptbloque->TipoPropn(cont);        
          
          retdev = ptbloque -> M4APGetHandNom( strnombre, &uHandPrpAux );
          
          if ((strnombre==NULL) || (strvalor==NULL))  {
                 
                 delete []pszLineaAux;
                 
                 return IDS_RTE_OBJ_NULL;
          };               
          // compruebo que es un entero si no lo es meto las comillas
          switch ( iTipo )   {
          
            case  T_PRP_STR :
          
               sprintf( pszLineaAux ,"%s  %s = \"%s\"\r\n",strtabuladores,strnombre,strvalor);
          
               break;
          
            case T_PRP_FEC :
          
               sprintf( pszLineaAux ,"%s  %s = (d) \"%s\"\r\n",strtabuladores,strnombre,strvalor);
          
               break;
          
             case T_PRP_INT:      
          
               sprintf( pszLineaAux ,"%s  %s = %s\r\n",strtabuladores,strnombre,strvalor);
          
               break;
          
             default : 

               sprintf( pszLineaAux ,"%s  %s = \"%s\"\r\n",strtabuladores,strnombre,strvalor);
          
               break;
          }     
          
          #if !defined(_WINDOWS) && !defined(_UNIX)
				OBLstrcat(pszBufferOut, pszLineaAux);
		  #else
				pszBufferOut.strcat( pszLineaAux);
		  #endif

        };
      //aqui ya tengo escrito todos las propiedades ahora toca 
      //imprimirse todos los bloques   
                                     
                                     
      contador=ptbloque->NumBloques();
      if (contador>0) ptbloque->M4ABlOrdenar();
      for (cont=0;cont<(int)contador;cont++)
        { 
         puntero=NULL;
         puntero=ptbloque->PtrBloquen(cont);
         retdev=WriteRecursivo(nivel+1,puntero,pszBufferOut);
         if (retdev!=EXITO)                     
               { 
                 delete []pszLineaAux;
                 return retdev; 
               };
        }
      sprintf( pszLineaAux ,"%sEND\r\n",strtabuladores);
      #if !defined(_WINDOWS) && !defined(_UNIX)
			  OBLstrcat(pszBufferOut, pszLineaAux);
	  #else
			  pszBufferOut.strcat( pszLineaAux);
	  #endif

      
      delete []pszLineaAux;
      delete []strtabuladores;
      return EXITO;  
                          
};
       

BOOL BlqCmp(M4ClassBloque *pBlq1,M4ClassBloque *pBlq2)
  {               
    
    char *szComp1,*szComp2;
    int iTipo1,iTipo2;
    M4RETCODE retdev;   
    M4ClassBloque *pBlqAux1,*pBlqAux2;
    UINT uContObj1,uContObj2,uCont;                      
    BOOL bIguales;
    //Comparo las Instancias
    
    retdev = pBlq1->M4CBGetHinst(&szComp1);
    if (retdev != EXITO) return FALSE;
    
    retdev = pBlq2->M4CBGetHinst(&szComp2);
    if (retdev != EXITO) return FALSE;
    
    if (strcmp(szComp1,szComp2) != 0 ) return FALSE;
    
    // Comparo las clases haber si coinciden
    
    retdev = pBlq1->M4CBGetClass(&szComp1);
    if (retdev != EXITO) return FALSE;
    
    retdev = pBlq2->M4CBGetClass(&szComp2);
    if (retdev != EXITO) return FALSE;
    
    if (strcmp(szComp1,szComp2) != 0 ) return FALSE;
    
    // comparo las propiedades
    
    uContObj1= pBlq1->NumPropiedades();
    uContObj2= pBlq2->NumPropiedades();
    
    if (uContObj1 != uContObj2) return FALSE;
    
    // si tienen el mismo numero de propiedades las ordenos y las comparo una a una
    
    
    if (!pBlq1->APOrdenado()) pBlq1->M4APOrdenar();
    if (!pBlq2->APOrdenado()) pBlq2->M4APOrdenar();
    
    for (uCont = 0;uCont< uContObj1;uCont ++)
     {                                       
       // se compara si los dos nombres son iguales
       retdev = pBlq1->M4APGetNomn(uCont,&szComp1);
       if (retdev != EXITO) return FALSE;
       
       retdev = pBlq2->M4APGetNomn(uCont,&szComp2);
       if (retdev != EXITO) return FALSE;
       
       if (strcmp(szComp1,szComp2) != 0 ) return FALSE;
       
       
       // se compara que los dos valores son iguales
       
       retdev = pBlq1->M4APGetValn(uCont,&szComp1);
       if (retdev != EXITO) return FALSE;
       
       retdev = pBlq2->M4APGetValn(uCont,&szComp2);
       if (retdev != EXITO) return FALSE;
       
       if (strcmp(szComp1,szComp2) != 0 ) return FALSE;        
       


       // se compara que los dos tipos de las propiedades
       
       retdev = pBlq1->M4APGetTipon(uCont,&iTipo1);
       if (retdev != EXITO) return FALSE;
       
       retdev = pBlq2->M4APGetTipon(uCont,&iTipo2);
       if (retdev != EXITO) return FALSE;
       
       if (iTipo1 != iTipo2 ) return FALSE;

     } 
     
     
     // si estamos aque quiered decir que estamos con todo igual y falta ver los bloques internos
    
    uContObj1= pBlq1->NumBloques();
    uContObj2= pBlq2->NumBloques();
    
    if (uContObj1 != uContObj2) return FALSE;
    
    // si tienen el mismo numero de propiedades las ordenos y las comparo una a una
    
    
    if (!pBlq1->M4ABlOrdenado()) pBlq1->M4ABlOrdenar();
    if (!pBlq2->M4ABlOrdenado()) pBlq2->M4ABlOrdenar();
    
    for (uCont = 0;uCont< uContObj1;uCont ++)
     {                                       
       retdev = pBlq1->M4ABlGetpBlqn(uCont,&pBlqAux1); 
       if (retdev != EXITO) return FALSE;
       
       retdev = pBlq2->M4ABlGetpBlqn(uCont,&pBlqAux2); 
       if (retdev != EXITO) return FALSE;      
       
       
       bIguales = BlqCmp(pBlqAux1,pBlqAux2);
       
       if (!bIguales) return FALSE;
       
     };    
     
     return TRUE; 
     
 }



