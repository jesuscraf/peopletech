#include"lobj.hpp"                                          
 
/* ******************************************************************************************* 
   *******************************************************************************************
            IMPLEMENTACION DE LA CLASE QUE CONTIENE A UN FICHERO
            
   ******************************************************************************************* 
   *******************************************************************************************/  
//

  
M4ClassLOBL::M4ClassLOBL()
    {
         
         pBloque    = NULL;    
         bdefbloq   = FALSE;
         pHands     = NULL;
         pAnaliz    = NULL;
         banalizador= FALSE;      
         hId        = 0;               
         bdefinido  = FALSE;
         pAMProp    = new ArrayMatch;
         pAMBlq     = new ArrayMatch;
         hTask      = 0;
         uPosOut    = 0;   
         
         // AÑADIDO JMMS -> 13 / 07 / 95
         m_lPosicionError = 0UL;
         m_lLineaError    = 0UL;
         m_lOffSetError   = 0UL;
     };    
     
     
                                   
M4ClassLOBL::~M4ClassLOBL()
     {  
        if ( pAnaliz )       delete pAnaliz;
        if ( pHands  )       delete pHands;
        if ( pBloque )       delete pBloque;
        if ( pAMProp )       delete pAMProp; 
        if ( pAMBlq  )       delete pAMBlq;
        
     };  
    



M4RETCODE M4ClassLOBL::GetPtrBlq ( UINT ai_uHandBlq , M4ClassBloque ** ai_pBloque )
  {
  
         int retdev;
         void *pBlq=NULL;
        
         // Consigo el bloque por el handle
         retdev=pHands->GetPtrBlq( ai_uHandBlq , &pBlq );
         if (retdev!=EXITO) return retdev;
         
         *ai_pBloque = ( M4ClassBloque * )pBlq;

         return EXITO;
  }         
  
  

    
M4RETCODE M4ClassLOBL::SetObl(char *Version,char *NombFich,char *ExtFich,char *Titulo,char *Base,UINT hObl)
       { 
          /* esta funcion devuelve, a esta funcion se le tiene que pasar todos los valores
            para que valla bien y además tiene que pasarle el handle. 
                       EXITO -> Exito y pone a true la var. bdefinido
                       IDS_RTE_NO_MEMORIA  -> Error en caso de no haber tenido memoria suficiente
          */      
         if (!Version) return IDS_RTE_BLQ_PATH_NO_FOUND;
         if (hObl<0) return IDS_RTE_OBL_HANDLE_INCORECTO;
     //    pszVersion = new(char[MAX_TAM_IDENT]);
     //    if ((pszVersion==NULL))
     //        return IDS_RTE_NO_MEMORIA; 
         if ( !NombFich || !ExtFich || !Titulo || !Base ) return IDS_RTE_LOBJ_PARAM_NULL;
         StrCpy(szVersion,Version);
         StrCpy(szNombFich,NombFich);
         StrCpy(szExtFich,ExtFich);
         StrCpy(szTitulo,Titulo);
         StrCpy(szBase,Base);
         hId = hObl;
         bdefinido = TRUE;
         return EXITO;
       }; 



       
 
/* ******************************************************************************************* 
   *******************************************************************************************
        IMPLEMENTACION DE LA PARTE PRIVADA DE LA CLASE QUE CONTIENE A UN BLOQUE COMPLETO
   ******************************************************************************************* 
   *******************************************************************************************/  

   BOOL M4ClassLOBL::definidoObl()
     {
       return bdefinido;
     };
    
    M4RETCODE M4ClassLOBL::OblGetVersion(char **pszVer)
    {
      if (!bdefinido) return IDS_RTE_LOBL_NO_DEFINIDO;
      *pszVer=szVersion;
      return EXITO;
    };    
    
    M4RETCODE M4ClassLOBL::OblSetVersion(char *szVer) 
     {
      if (!szVer) return IDS_RTE_LOBJ_PARAM_NULL;
      StrCpy(szVersion,szVer); 
      return EXITO;
     };
   
    
    M4RETCODE M4ClassLOBL::OblGetNombFich(char **NombFich)
        {
        if (!bdefinido) return IDS_RTE_LOBL_NO_DEFINIDO;
        *NombFich = szNombFich;
        return EXITO;
       };
   
    M4RETCODE M4ClassLOBL::OblGetExtFich(char **ExtFich)
     {
        if (!bdefinido) return IDS_RTE_LOBL_NO_DEFINIDO;
        *ExtFich = szExtFich;
        return EXITO;
     }
   
    M4RETCODE M4ClassLOBL::OblGetTitulo(char **Titulo)
     {
       if (!bdefinido) return IDS_RTE_LOBL_NO_DEFINIDO;
       *Titulo = szTitulo;
       return EXITO;
     }
   
    M4RETCODE M4ClassLOBL::OblGetBase(char **Base) 
     {
      if (!bdefinido) return IDS_RTE_LOBL_NO_DEFINIDO;
      *Base = szBase;
      return EXITO;
     }
   
     
    M4RETCODE M4ClassLOBL::OblGetClass(char **szCls)
    {
      M4RETCODE retdev;
      // compruebo que el Objeto obl esta definido
      if (!bdefinido) return IDS_RTE_LOBL_NO_DEFINIDO;
      // compruebo que existe pBloque
      if (pBloque==NULL) return IDS_RTE_LOBL_NO_BLOQUE;
    
      retdev = pBloque->M4CBGetClass(&(*szCls));
      return retdev;
    };  
    
    M4RETCODE M4ClassLOBL::OblGetHinst(char **szHis)  
    {
      M4RETCODE retdev;
      // compruebo que el Objeto obl esta definido
      if (!bdefinido) return IDS_RTE_LOBL_NO_DEFINIDO;
      // compruebo que existe pBloque
      if (pBloque==NULL) return IDS_RTE_LOBL_NO_BLOQUE;
    
      retdev = pBloque->M4CBGetHinst(&(*szHis));
      return retdev;
    };    
 
   M4RETCODE M4ClassLOBL::OblSetHinst(char *szHis)  
    {
      M4RETCODE retdev;
      // compruebo que el Objeto obl esta definido
      if (!bdefinido) return IDS_RTE_LOBL_NO_DEFINIDO;
      // compruebo que existe pBloque
      if (pBloque==NULL) return IDS_RTE_LOBL_NO_BLOQUE;
    
      retdev = pBloque->M4CBSetHinst(szHis);

      return retdev;
     };        
    
    
    
         
    t_PID M4ClassLOBL::OblGetTask()
     {
      return hTask;
     } 
     
    M4RETCODE M4ClassLOBL::OblSetTask(t_PID hTaskFich)
     {
      hTask = hTaskFich;
      return EXITO;
     }         
     
    M4RETCODE M4ClassLOBL::OblSetFich(char *pszfichero)
     {              
       /* le pasamos el fichero y no esta inicializado el analizador entonces lo crea y le
          mete el texto en caso de que este ya generado el objeto se le añade al texto lo ya
          creado
            IDS_RTE_NO_MEMORIA -> FALTA MEMORIA
            EXITO */                        
       M4RETCODE retdev;
       if (bdefbloq) return IDS_RTE_BLQ_CONSTRUIDO;
       if (!banalizador)   
         { 
          if ( (pAnaliz = new M4ClassAnalizador ) == NULL) return  IDS_RTE_NO_MEMORIA;
          retdev = pAnaliz->SetFichero(pszfichero);
          if (retdev!=EXITO) return retdev;
          }
       else
          { 
            retdev = pAnaliz->AppendTexto(pszfichero);
            if (retdev!=EXITO) return retdev;
           }
       banalizador = TRUE;
       return EXITO;
     }; 


/*********************************************************************************
            ACCESO A LAS PROPIEDADES A PARTIR DE LOS HANDLES
 *********************************************************************************/

M4RETCODE M4ClassLOBL::OblGetClassBlqHandle (UINT handle,char **pszClass)
 {
    int retdev;
    void *pBlq=NULL;
    
    retdev=pHands->GetPtrBlq(handle,&pBlq);
    if (retdev!=EXITO) return retdev;
    
    retdev=((M4ClassBloque *)pBlq)->M4CBGetClass(&(*pszClass));
    return retdev ;
    
 }; 
 
 
 
 
 
M4RETCODE M4ClassLOBL::OblGetHinstBlqHandle (UINT handle,char **pszHinst)
 {
    int retdev;
    void *pBlq=NULL;   
    
    
    retdev=pHands->GetPtrBlq(handle,&pBlq);
    if (retdev!=EXITO) return retdev;
    
    retdev=((M4ClassBloque *)pBlq)->M4CBGetHinst(&(*pszHinst));
    
    return retdev ;
 }; 

M4RETCODE M4ClassLOBL::OblSetHinstBlqHandle (UINT handle,char *a_pszHinst)
 {
         int retdev;
         void *pBlq=NULL;
         M4ClassBloque *pBlqPadre = NULL;
    
    retdev=pHands->GetPtrBlq(handle,&pBlq);
    if (retdev!=EXITO) return retdev;
    
    retdev= ((M4ClassBloque *)pBlq)->M4CBSetHinst( a_pszHinst ) ;
    if ( retdev != EXITO ) return retdev;
    
    retdev=((M4ClassBloque *)pBlq) -> M4CBGetPadre ( & pBlqPadre );
    if ( retdev != EXITO ) return retdev;    
    
    if ( pBlqPadre == NULL ) return EXITO;
    
    retdev = pBlqPadre -> M4ABlOrdenar(); 
    
    return retdev;

     
 }; 




 
M4RETCODE M4ClassLOBL::OblGetValPropHandle (UINT handle,char **pszVal)
  {  
    int retdev;
    void *pProp=NULL;
    
    retdev=pHands->GetPtrProp(handle,&pProp);
    if (retdev!=EXITO) return retdev;
    
    retdev=((M4ClassPropiedad *)pProp)->M4CPGetPropiedad(&(*pszVal));
    return retdev ;
  };  
            
M4RETCODE M4ClassLOBL::OblGetNomPropHandle (UINT handle,char **pszVal)
  {  
    M4RETCODE retdev;
    void *pProp=NULL;
    
    retdev=pHands->GetPtrProp(handle,&pProp);
    if (retdev!=EXITO) return retdev;
    
    retdev=((M4ClassPropiedad *)pProp)->M4CPGetNomPropiedad(&(*pszVal));
    return retdev ;
  }; 
                                       

     
/*******************************************************************************
********************************************************************************      
M4RETCODE OblGetTipoHandNomb(UINT hBlq,char * szNombProp,int * iTipo);  

   Implementacion -> 12 / 07 / 95
   
    parametros : hblq -> (I) handle del del bloque que contiene  la propiedad
                 szNombProp ->(I) Este es el nombre de la propiedad
                 iTipo -> (O) Devuelve el tipo de esta propiedad
    
    Extracto  :  Consigue el tipo de la propiedad dando el handle del bloque 
                 que la contiene y el nombre de la propiedad
   
********************************************************************************      
********************************************************************************/      

M4RETCODE M4ClassLOBL::OblGetTipoHandNomb(UINT hBlq,char * szNombProp,int * iTipo)
 {
    M4RETCODE retdev;
    void *pBlqAux;
    
    // consigo el puntero al bloque perteneciente a este bloque
    retdev = pHands->GetPtrBlq(hBlq,&pBlqAux);
    if (retdev!=EXITO) return retdev;        
                                         
    return  ((M4ClassBloque *)pBlqAux)->M4APGetTipoPropNom(szNombProp,&(*iTipo));
 
 }  

/******************************************************************************
*******************************************************************************
M4ClassLOBL::OblGetTipoPropHandle (UINT uHandPrp,int *iTipo)
Implementacion -> del 29/06/95 
   Se consigue a partir del hadle de la propiedad se obtiene su tipo
      uHandPrp -> (I) Handle de propiedad
      iTipo    -> (O) Esta variable es de salida e indica si es numerico o alfa
******************************************************************************                                       
******************************************************************************/

M4RETCODE M4ClassLOBL::OblGetTipoPropHandle (UINT uHandPrp,int *iTipo)
  {
  
    M4RETCODE retdev;
    void *pProp;
    
    
    retdev=pHands->GetPtrProp(uHandPrp,&pProp);
    if (retdev!=EXITO) return retdev;
    
    retdev=((M4ClassPropiedad *)pProp)->M4CPGetTipoPropiedad(&(*iTipo));
    return retdev ;
  }   
   
   
                       
M4RETCODE M4ClassLOBL::OblGetPropHandNomb(UINT hBlq,char *szNombProp,char **szVal)
  {
    M4RETCODE retdev;
    void *pBlqAux;
    
    // consigo el puntero al bloque perteneciente a este bloque
    retdev = pHands->GetPtrBlq(hBlq,&pBlqAux);
    if (retdev!=EXITO) return retdev;        
                                         
    return  ((M4ClassBloque *)pBlqAux)->M4APGetPropNom(szNombProp,&(*szVal));
  } ;
           
M4RETCODE M4ClassLOBL::OblGetBlqPath(char *pPath,M4ClassBloque **pBlqOut) 
   {
     return pBloque->M4CBGetBlqPath(pPath,&(*pBlqOut)); 
   };

M4RETCODE M4ClassLOBL::OblGetValProp(char *pPathInst,char *pNomb,char **pszVal)
  {
    M4RETCODE retdev;                      
    M4ClassBloque *pBlqAux;
    
    
    
    retdev = OblGetBlqPath(pPathInst,&pBlqAux);
    if (retdev!=EXITO) return retdev;
    
    return pBlqAux->M4APGetPropNom(pNomb,&(*pszVal));
  
  };



M4RETCODE M4ClassLOBL::OblGetValProp( char *pPathInst,char **pszVal)
   {
      // en pPathInst tengo el camino y despues del ultimo punto viene el nombre de la propiedad
      M4RETCODE retdev;
      char *	szNombreProp;
      char *	pPath;
      char *	pszPos;
      size_t	iCar;
      int		iCont;
      
      // averiguamos la ultima ocurrencia del punto y despues
     if (!pPathInst) return IDS_RTE_BLQ_NO_PATH;
     
     pPath = new char [MAX_TAM_PATH]; 
     szNombreProp = new char[MAX_TAM_PROP];
     if (!pPath) return IDS_RTE_NO_MEMORIA;
     strcpy(pPath,pPathInst);
     pszPos=strrchr(pPathInst,'.');
     iCar = pszPos - pPathInst + 1;// el primer caracter despues del punto
     pPath[iCar-1] = 0;  // me quedo solo con la parte del camino
     iCont = 0;
     while ( pPathInst[iCar]>' ')
       {
         szNombreProp[iCont] = pPathInst[iCar];
         iCar++;
         iCont++;
       }; 
      szNombreProp[iCont] = 0;   
       
     retdev = OblGetValProp(pPath,szNombreProp, &(*pszVal));    
     delete []pPath;                 
     delete []szNombreProp;
     return retdev; 
   };


M4RETCODE M4ClassLOBL::OblGetHandProp( char *pPathInst,char *pNomb,UINT *uHand)
  {
    M4RETCODE retdev;                      
    M4ClassBloque *pBlqAux;
     
    retdev = OblGetBlqPath(pPathInst,&pBlqAux);
    if (retdev!=EXITO) return retdev;
                                         
    return pBlqAux->M4APGetHandNom(pNomb,&(*uHand));
  };      
  
  

M4RETCODE M4ClassLOBL::OblGetHandProp( char *pPathInst,UINT *uHand)
    {
      // en pPathInst tengo el camino y despues del ultimo punto viene el nombre de la propiedad
      M4RETCODE		retdev;
      char *		szNombreProp;
      char *		pPath;
      char *		pszPos;
      size_t		iCar ;
      int			iCont;
      
      // averiguamos la ultima ocurrencia del punto y despues
     if (!pPathInst) return IDS_RTE_BLQ_NO_PATH;
     
     pPath = new char [MAX_TAM_PROP]; 
     szNombreProp = new char[MAX_TAM_IDENT];
     if (!pPath) return IDS_RTE_NO_MEMORIA;
     strcpy(pPath,pPathInst);
     pszPos=strrchr(pPathInst,'.');
     iCar = pszPos - pPathInst + 1;// el primer caracter despues del punto
     pPath[iCar-1] = 0;  // me quedo solo con la parte del camino
     iCont = 0;
     while ( pPathInst[iCar]>' ')
       {
         szNombreProp[iCont] = pPathInst[iCar];
         iCar++;
         iCont++;
       };   
     szNombreProp[iCont] = 0;  
     retdev = OblGetHandProp(pPath,szNombreProp, &(*uHand));    
     delete []pPath;                      
     delete []szNombreProp;
     return retdev; 
   }; 
   
// FUNCIONES QUE CONSIGUE EL HANDLE  DE UN BLOQUE DADO EL CAMINO DADO EL CAMINO
   
M4RETCODE M4ClassLOBL::OblGetHandBlq( char *pPathInst,UINT  *uHand)
  {
    M4RETCODE retdev;                      
    M4ClassBloque *pBlqAux;

    retdev = OblGetBlqPath(pPathInst,&pBlqAux);
    if (retdev!=EXITO) return retdev;
                                         
    return pBlqAux->M4CBGetHandle(&(*uHand));
  };                                                                                     


M4RETCODE M4ClassLOBL::OblSetPropPath(char *pPathInst,char *pszNomb,char *pszVal,int iTipo)
 {  
   M4RETCODE retdev;                      
   M4ClassBloque *pBlqAux;
     
   retdev = OblGetBlqPath(pPathInst,&pBlqAux);
   if (retdev!=EXITO) return retdev;             
   return pBlqAux->M4APSetVal(pszNomb,pszVal,iTipo); 


 };

M4RETCODE M4ClassLOBL::OblSetPropNomb(UINT uHandBlq,char *szNomb,char *szValor,int iTipo)
 {
   M4RETCODE retdev;
   M4ClassBloque *pBloque;   
   void *ptr;
   
   retdev = pHands -> GetPtrBlq(uHandBlq, &ptr);
   if (retdev != EXITO ) return retdev ;
   pBloque = (M4ClassBloque *)ptr;
   return pBloque -> M4APSetVal(szNomb,szValor,iTipo);
 
 }    



  
M4RETCODE M4ClassLOBL::OblSetPropHand( UINT uHandProp,char *pszVal,int iTipo)  
 {
    int retdev;
    void *pProp=NULL;
    
    retdev=pHands->GetPtrProp(uHandProp,&pProp);
    if (retdev!=EXITO) return retdev;
    
    retdev=((M4ClassPropiedad *)pProp)->M4CPSetVal(pszVal,iTipo);
    return retdev ;
 };             
 
M4RETCODE M4ClassLOBL::OblAddPropPath(char *pPathInst,char *pszNomb,char *pszVal,int iTipoPrp,UINT * uHandProp )
 { 
   M4RETCODE retdev;           
   M4ClassPropiedad *pPropiedad;       
   UINT uPosicion;
   UINT uHandle;
   M4ClassBloque *pBlqAux;
   
   
   // primero me consigo el puntero al bloque donde quiero meter la propiedad  
   retdev = OblGetBlqPath(pPathInst,&pBlqAux);
   if (retdev!=EXITO) return retdev;
 
   
   // me alloco una propiedad vacia para obtener el puntero
   retdev=pBlqAux->M4APAddNuevaPropiedad(&pPropiedad,&uPosicion,pBlqAux);
   if (retdev!=EXITO) return retdev;
        
   // añado el puentero al array de handles y este al añadirlo me devuelve uno
   retdev = pHands->AddHandle(T_PROPIEDAD, pPropiedad , & uHandle );
   if (retdev!=EXITO) return retdev;
        
   // asgino los valores correspondientes a la propiedad
   retdev = pBlqAux->M4APSetPropiedadn(uPosicion,pszNomb,pszVal,uHandle, iTipoPrp);
   if (retdev!=EXITO) return retdev;
   
   *uHandProp=uHandle;
   return EXITO;
   
 };
                        
  
M4RETCODE M4ClassLOBL::OblAddPropHand(UINT uHandBlq,char *pszNomb,char *pszVal,int iTipoPrp,UINT *uHandProp)
 { 
   M4RETCODE retdev;           
   M4ClassPropiedad *pPropiedad;       
   UINT uPosicion;
   UINT uHandle;
   M4ClassBloque *pBlqAux;
   void *ptr;
   
    
   // consigo el puntero al bloque donde quiero meter el bloque
   retdev=pHands -> GetPtrBlq ( uHandBlq, &ptr );
   if (retdev!=EXITO) return retdev;   
   pBlqAux =(M4ClassBloque *)ptr;
   
   
   // me alloco una propiedad vacia para obtener el puntero
   retdev=pBlqAux->M4APAddNuevaPropiedad(&pPropiedad,&uPosicion,pBlqAux);
   if (retdev!=EXITO) return retdev;
        
   // añado el puentero al array de handles y este al añadirlo me devuelve uno
   retdev = pHands->AddHandle(T_PROPIEDAD,pPropiedad,&uHandle);
   if (retdev!=EXITO) return retdev;
        
   // asgino los valores correspondientes a la propiedad
   retdev = pBlqAux->M4APSetPropiedadn(uPosicion,pszNomb,pszVal,uHandle,iTipoPrp);
   if (retdev!=EXITO) return retdev;
   
   *uHandProp=uHandle;
   return EXITO;
   
 };             
 
 
M4RETCODE M4ClassLOBL::OblDelPropPath(char *pPathInst,char *pszNomb)
  { 
    M4ClassBloque *pBlqAux;
    UINT uHandProp;
    M4RETCODE retdev;
    
   // primero me consigo el puntero del bloque donde dode esta la propiedad  
   retdev = OblGetBlqPath(pPathInst,&pBlqAux);
   if (retdev!=EXITO) return retdev; 
   
   // ahora consigo el handle de la propiedad
   retdev = pBlqAux->M4APGetHandNom(pszNomb,&uHandProp);                          
   if (retdev!=EXITO) return retdev; 
   
   //  borro la entrada en la tabla de handles
   retdev = pHands->DelPtrProp(uHandProp);
   if (retdev!=EXITO) return retdev; 
   
   // borro la entrada del array de propiedades
   retdev = pBlqAux->M4APDelPropiedad(pszNomb);
   if (retdev!=EXITO) return retdev;
   
   return EXITO;
  }; 
      
M4RETCODE M4ClassLOBL::OblDelPropHand(UINT uHandProp)
  { 
   
   /* dando el handle de la propiedad borramos esta*/
  
    M4ClassPropiedad *pPrpABorrar;
    M4ClassBloque *pBlqAux = NULL;
    void *ptr;                     
    char *pszNomb;
    M4RETCODE retdev;
                                                    /* jb1*/
   // primero me consigo el puntero del bloque donde dode esta la propiedad  
   retdev=pHands->GetPtrProp(uHandProp,&ptr);
   if (retdev!=EXITO) return retdev;
   pPrpABorrar =(M4ClassPropiedad *)ptr;
   retdev = pPrpABorrar->M4CPGetPadre(&ptr);   
   if (retdev!=EXITO) return retdev;   
   pBlqAux = (M4ClassBloque *)ptr;
   if (!pBlqAux) pBlqAux=pBloque;
   // consigo el nombre de la propiedad
   retdev=pPrpABorrar->M4CPGetNomPropiedad(&pszNomb);
   if (retdev!=EXITO) return retdev;   
   
   //  borro la entrada en la tabla de handles
   retdev = pHands->DelPtrProp(uHandProp);
   if (retdev!=EXITO) return retdev; 
   
   // borro la entrada del array de propiedades
   retdev = pBlqAux->M4APDelPropiedad(pszNomb);
   if (retdev!=EXITO) return retdev;
   
   return EXITO;
  
  };


  
     
/*******************************************************************************
********************************************************************************      
   OblDelPropHandNomb(UINT uHandBlq,char *pNomb)

   Implementacion -> 03 / 06 / 95
   
    parametros : uHandBlq -> (I) handle del bloque que contiene la propiedad
                 pNomb    -> (I) Nombre de la propiedad
         
    Extracto  : Esta funcion dado el handle del bloque que contiene una propie
                dad y un nombre de prop. La borra y libera el espacio en memoria
                que ocupa.    
   
********************************************************************************      
********************************************************************************/      
      
      
M4RETCODE M4ClassLOBL::OblDelPropHandNomb(UINT uHandBlq,char *pNomb)
  {
    M4RETCODE retdev;
    void *pBlqAux;
    
    // consigo el puntero al bloque perteneciente a este bloque
    retdev = pHands->GetPtrBlq(uHandBlq,&pBlqAux);
    if (retdev!=EXITO) return retdev;  
    // con el puntero del bloque borro la propiedad con el array      
                                         
    return  ((M4ClassBloque *)pBlqAux)->M4APDelPropiedad(pNomb);
  
  }  


        
 
M4RETCODE M4ClassLOBL::OblAddBlqPath(char *pPathInst,char *pszClass,char *pszHinst,UINT *uHandBlqOut)
  {   
     M4ClassBloque *pBlqDest;   // bloque que uso para encontrar en donde lo voy a meter
     M4ClassBloque *pBlqFuturo; // nuevo bloque que voy a meter
     M4ClassBloque *pBloqueaux;  // bloque auxiliar
     M4RETCODE retdev;
     UINT uHandBlqO;
   // primero me consigo el puntero al bloque donde quiero meter la propiedad  
   retdev = OblGetBlqPath(pPathInst,&pBlqDest);
   if (retdev!=EXITO) return retdev;
      
   // añadimos un bloque nuevo en el destino
    retdev=pBlqDest->M4ABAddNuevoBlq(pBlqDest,&pBlqFuturo);
    if (retdev!=EXITO) return retdev;
    // le hago apuntar un handle
    retdev = pHands->AddHandle(T_BLOQUE,pBlqFuturo,&uHandBlqO);
    if (retdev!=EXITO) return retdev;
    // ahora le asigno sus valores respectivos
    retdev = pBlqFuturo->M4CBSetBloque(pszClass,pszHinst,uHandBlqO,&pBloqueaux);
    pBloqueaux->M4CBGetHandle(&(*uHandBlqOut));
    if (retdev!=EXITO) return retdev;
    return EXITO;
  };      
  
   
   
   
M4RETCODE M4ClassLOBL::OblAddBlqHand(UINT uHandBlq,char *pszClass,char *pszHinst,UINT *uHandBlqOut)  
  {
     M4ClassBloque *pBlqDest;   // bloque que uso para encontrar en donde lo voy a meter
     M4ClassBloque *pBlqFuturo; // nuevo bloque que voy a meter
     M4ClassBloque *pBloqueaux;  // bloque auxiliar
     M4RETCODE retdev;
     void *ptr;
     UINT uHandBlqO;
      
     // primero me consigo el puntero al bloque donde quiero meter la propiedad  
     retdev=pHands->GetPtrBlq(uHandBlq,&ptr);
     if ( retdev != EXITO ) return retdev;   
     pBlqDest =(M4ClassBloque *)ptr;
      
   // añadimos un bloque nuevo en el destino
    retdev=pBlqDest->M4ABAddNuevoBlq(pBlqDest,&pBlqFuturo);
    if ( retdev!=EXITO ) return retdev;
    
    // le hago apuntar un handle
    retdev = pHands->AddHandle( T_BLOQUE, pBlqFuturo, & uHandBlqO );
    if (retdev!=EXITO) return retdev;
    
    // ahora le asigno sus valores respectivos
    retdev = pBlqFuturo->M4CBSetBloque(pszClass,pszHinst,uHandBlqO,&pBloqueaux);
    if (retdev!=EXITO) return retdev;
    
    pBloqueaux->M4CBGetHandle(&(*uHandBlqOut));
    return EXITO;
  };     
  
  
     
  
M4RETCODE M4ClassLOBL::OblDelBlqPath(char *pPathInst)
  {
    M4ClassBloque *pBlqPadre,*pBlqABorrar;
    UINT uHandBlq;         
    char *pszInstancia ,*pszClase;
    M4RETCODE retdev;
    
   // primero me consigo el puntero a borrar y el padre
   retdev = OblGetBlqPath(pPathInst,&pBlqABorrar);
   if (retdev!=EXITO) return retdev; 
   
   retdev = pBlqABorrar -> M4CBGetPadre(&pBlqPadre); 
   if (retdev != EXITO) return retdev;  
   if (!pBlqPadre) return IDS_OBL_BORRAR_RAIZ;
   
   // ahora tenemos que buscar el nombre el nombre y la instancia del bloque que queremos borrar
   // tambien conseguimos el handle
   
   retdev = pBlqABorrar->M4CBGetHinst(&pszInstancia);                          
   if (retdev!=EXITO) return retdev; 
   
   retdev = pBlqABorrar->M4CBGetClass(&pszClase);                          
   if (retdev!=EXITO) return retdev;  
   
   retdev = pBlqABorrar->M4CBGetHandle(&uHandBlq);
   
   
   //  borro la entrada en la tabla de handles
   retdev = pHands->DelPtrBlq(uHandBlq);
   if (retdev!=EXITO) return retdev; 
   
   // borro la entrada del array de propiedades
   retdev = pBlqPadre->M4ABlDelBlq(pszClase,pszInstancia);
   if (retdev!=EXITO) return retdev;
   
   return EXITO;
   
   
  }; 
                

 


M4RETCODE M4ClassLOBL::OblDelBlqHand(UINT uHandBlq)
  {
    /* CONSIGO EL CAMINO Y LLAMO A LA QUE BORRA CUANDO TENGO EL CAMINO*/
    char szCamino[MAX_TAM_PROP];      
    M4RETCODE retdev; 
    
    retdev = OblGetPath(uHandBlq,szCamino);
    if (retdev!=EXITO) return retdev;
    return OblDelBlqPath(szCamino);    
    
  }; 
  
  
                
// IMPORTANTE HAY QUE PASARLE COMO PARAMETRO UNA CADENA ALLOCADA
M4RETCODE M4ClassLOBL::OblGetPath(UINT uHandItm,char *szPathout)
  {    
   // se le tiene que pasar un buffer ya alocado
   // y un handle , y devuelvo en caso de que este handle sea correcto
   // devolveria el camino a este objeto tanto si es una propiedad
   // como si es un  
  
   M4ClassBloque *pBlqHijo = NULL,*pBlqPadre= NULL;
   void *ptr= NULL,*ptrProp = NULL;
   
   M4RETCODE retdev;
   char *pClass = NULL;
   char *pHinst = NULL;             
   M4ClStringOld szHinst,szPath;
   int iTipo;       
   size_t iLongitud;       
   
   retdev = pHands->TipoHandle(uHandItm,&iTipo);
   if (retdev!=EXITO)  return retdev;   
   switch (iTipo)                   
     {
     case T_BLOQUE: 
        retdev=pHands->GetPtrBlq(uHandItm,&ptr);
        if (retdev!=EXITO)  return retdev;   
        break;
      case T_PROPIEDAD: 
        retdev = pHands->GetPtrProp(uHandItm,&ptrProp);
        if (retdev != EXITO ) return retdev; 
        retdev=((M4ClassPropiedad *)ptrProp)->M4CPGetPadre(&ptr);
        if (retdev != EXITO) return retdev;   
        break;
      default:
         return IDS_PROP_CORRUPTA;       
         break;
      };
   pBlqHijo =(M4ClassBloque *)ptr;

   retdev= pBlqHijo->M4CBGetPadre(&pBlqPadre);
   if (retdev != EXITO) return retdev;
   
   
   StrCpy(szPath,"");
   while (pBlqPadre)  //mientras el padre sea distinto de null
    {                                    
      pBlqHijo->M4CBGetHinst(&pHinst);
      pBlqHijo->M4CBGetClass(&pClass);
     
      StrCpy(szHinst,pClass);
      szHinst.strcat(".");
      szHinst.strcat(pHinst);
      szHinst.strcat(".");
      
      szHinst.strcat(szPath);
      StrCpy(szPath,(const char *)szHinst);                  
            
      pBlqHijo = pBlqPadre;
      pBlqHijo->M4CBGetPadre(&pBlqPadre);
      if (retdev!= EXITO) return retdev;
    };

   strcpy(szPathout,szPath);
                                  
   iLongitud = strlen (szPathout ) ;                               
   
   if ( iLongitud <= 0 )
     szPathout[ 0 ] = 0 ;
   else
     szPathout[ iLongitud - 1 ] = 0;  

   return EXITO;  
  };
                   
                        
 
/* Esta funcion se le pasa el nombre de la propiedad con los comodines que sean necesarios
   pasando un punto y despues los comodines de la propiedad en cuestion*/
   
/* 
    DESCRIPCION : ESTA FUNCION ME DEVUELVE PROPIEDADES Y BLOQUES DANDO YO MATCHS 
    
    SI BPRIMERAVEZ -> TRUE
          UHANDMATCH -> PARAMETRO DE SALIDA QUE ME DEVUELVE EL HANDLE DEL MATCH QUE ACABO DE 
                        INTRODUCIR
       BPRIMERAVEZ -> FALSE
          UHANDMATCH -> PARAMETRO DE ENTRADA QUE ME DA EL HANDLE DEL MATCH QUE ESTOY BUSCANDO
                        Y QUE TENGO QUE BUSCAR EL SIGUIENTE
                        */
        
 
M4RETCODE M4ClassLOBL::OblBuscProp(BOOL bprimeravez,char *szPathInst,char *szPrpMatch,char *szPathOut,char **pszNomb,char **pszVal,UINT *uHandPrp,UINT *uHandMatch)
  {
   UINT uMaxHand; 
   UINT uCont;  
   UINT uMatch;
   UINT uPosicion; 
    char *pNombre; 
   M4RETCODE retdev;
   M4ClStringOld szPathAux;
   M4ClStringOld szPathMt;
   M4ClStringOld szPropMt;  
   char *pPropMatch,*pPathMatch;
   
   // comprobamos la validez de las cadenas introducido
   // Allocamos los bufferes    
   
   szPathAux.stralloc ( MAX_TAM_PATH );
   szPathMt.stralloc ( MAX_TAM_PATH );
   szPropMt.stralloc ( MAX_TAM_PATH );


 
   uMaxHand = pHands->NumeroHandles();  
   // cuando es la primera vez asigna a cero la propiedad de la ultima propiedad encontrada
   if (bprimeravez)              
       { 
         if (!szPathInst || !szPrpMatch) {
              return IDS_RTE_PARAM_NULL; 
         }

         strcpy( (char *)szPathMt , szPathInst );
         strcpy( (char *)szPropMt ,szPrpMatch );

        retdev = pAMProp->AMAddNuevo(szPathMt,szPropMt,&uMatch);
         if (retdev != EXITO ) {
            return retdev;
         }

         *uHandMatch = uMatch;
         uPosicion = 0;
       }
   else
       {

         retdev = pAMProp->AMGetPath(*uHandMatch,&pPathMatch);

         if (retdev!=EXITO)  {
            return retdev;
         }


         strcpy(szPathMt,pPathMatch);
         retdev = pAMProp->AMGetMatch(*uHandMatch,&pPropMatch);
         if (retdev!=EXITO)  {
            return retdev;
         }

         strcpy(szPropMt,pPropMatch);

           // Consigo la posicion la ultima posicion que a encontrado 

         retdev = pAMProp->AMGetPos(*uHandMatch,&uPosicion);

         if (retdev!=EXITO)  {
               
          
               
          
          
            return retdev;
         }

       }
        
   // a partir de aqui tengo actualizado el szpath y szMathProp    
   uCont = uPosicion + 1;
   while (uCont <= uMaxHand)
     {      
       retdev = OblGetNomPropHandle(uCont,&pNombre);
       if (retdev == EXITO)
         if (MatchCmp(szPropMt,pNombre))
             { 
               // ahora faltaria comprobar el path  si tasa o no
                              
               OblGetPath(uCont,szPathAux);
               
               if ( MatchDir(szPathMt,szPathAux) )
                   { 
                     
                     
                     *pszNomb = pNombre;
                     OblGetValPropHandle(uCont,&(*pszVal));
                     
                     *uHandPrp = uCont;

                     
                     strcpy(szPathOut,szPathAux);

                     
                     
                     retdev=pAMProp->AMSetPos(*uHandMatch,uCont);
              
                     
          
          
                     return retdev;
                   };
              };                                                                 
     uCont++;
     };               
  
    
  retdev = pAMProp->AMBorrar(*uHandMatch);                   

  if (retdev!=EXITO) return retdev;
  return IDS_OBJ_NOT_FOUND;
 
 }; 


M4RETCODE M4ClassLOBL::OblBuscBlq(BOOL bprimeravez,char *szPathInst,char *szPathOut,UINT *uHandBlq,UINT *uHandMatch)
  {
   UINT uMaxHand; 
   UINT uCont;  
   UINT uMatch;
   UINT uPosicion;
   M4RETCODE retdev;
 //  int iTipoObj;
   M4ClStringOld szPathAux;
   M4ClStringOld szPathMt;
   char *pPathMatch;         
   char *pszClase;
   M4ClStringOld strPathEntrada;
   // comprobamos la validez de las cadenas introducido
   
   // antes de nada me alloco los buffers
   szPathAux.stralloc ( MAX_TAM_PATH );
   szPathMt.stralloc  ( MAX_TAM_PATH );
   
   StrCpy ( strPathEntrada , szPathInst );
  
   uMaxHand = pHands->NumeroHandles();  
   // cuando es la primera vez asigna a cero la propiedad de la ultima propiedad encontrada
   if (bprimeravez)              
       { 
         if (  (char *)strPathEntrada == NULL ) return IDS_RTE_PARAM_NULL; 
         strcpy(szPathMt,strPathEntrada);
         retdev = pAMBlq->AMAddNuevo(szPathMt,"",&uMatch);
         if (retdev != EXITO ) return retdev;
         *uHandMatch = uMatch;
         uPosicion = 0;
       }
   else
       {
         retdev = pAMBlq->AMGetPath(*uHandMatch,&pPathMatch);
         if (retdev!=EXITO) return retdev;
         strcpy(szPathMt,pPathMatch);
         retdev = pAMBlq->AMGetPos(*uHandMatch,&uPosicion);
         if (retdev!=EXITO) return retdev;
       }
        
   // a partir de aqui tengo actualizado el szpath y szMathProp    
   uCont = uPosicion + 1;
   while (uCont <= uMaxHand)
     { 
       //pHands->TipoHandle(uCont,&iTipoObj);
       retdev = OblGetClassBlqHandle (uCont , & pszClase);
       if ( retdev == EXITO )  
           {
               // ahora faltaria comprobar el path  si tasa o no
               OblGetPath(uCont,szPathAux);
               if ( MatchDir(szPathMt,szPathAux) )
                   {    
                     *uHandBlq = uCont;
                     strcpy(szPathOut,szPathAux);
                     retdev=pAMBlq->AMSetPos(*uHandMatch,uCont);
                     if (retdev!=EXITO) return retdev;
                     return EXITO;
                   };    
             };
     uCont++;
     };
  retdev = pAMBlq->AMBorrar(*uHandMatch);                   
  if (retdev!=EXITO) return retdev;
  return IDS_OBJ_NOT_FOUND;
 };   
 
// INTERFACE DEL OBJETO M4CLASSLOBL PARA LAS FUNCIONES DE BUSQUEDAS  

M4RETCODE M4ClassLOBL::OblFindFirstMatchProp(char *szPathInst,char *szPrpMatch,char *szPathOut,char **pszNomb,char **pszVal,UINT *uHandProp,UINT *uHandMatch)
 {
   return OblBuscProp(TRUE,szPathInst,szPrpMatch,szPathOut,&(*pszNomb),&(*pszVal),&(*uHandProp),&(*uHandMatch));
 };                  
 
M4RETCODE M4ClassLOBL::OblFindFirstMatchBlq(char *szPathInst,char *szPathOut,UINT *uHandBlq,UINT *uHandMatch)
 {  
 
   return OblBuscBlq(TRUE,szPathInst,szPathOut,&(*uHandBlq),&(*uHandMatch));
 };
 
M4RETCODE M4ClassLOBL::OblFindNextMatchProp(UINT uHandMatch,char *szPathOut,char **pszNomb,char **pszVal,UINT *uHandProp)
 {
   return OblBuscProp(FALSE,"","",szPathOut,&(*pszNomb),&(*pszVal),&(*uHandProp),&uHandMatch);
 };
  
M4RETCODE M4ClassLOBL::OblFindNextMatchBlq(UINT uHandMatch,char *szPathOut,UINT *uHandBlq)
 {
                                                 
  return OblBuscBlq(FALSE,"",szPathOut,&(*uHandBlq),&uHandMatch);
  
 };
  

M4RETCODE M4ClassLOBL::WriteFicheroCompleto()
 {
  // esta funcion a partir de ahora imprime el finchero completo en la cadena miembro 
  // szOut y asi puedo recoger datos poco a poco
  M4ClStringOld szLineaAux;
  char *pszVer;          
  
#if !defined(_WINDOWS) && !defined(_UNIX)
  M4ClHString pszBufferAux;
#else
  M4ClStringOld pszBufferAux;
#endif

  M4ClStringOld szBuffer; 
  M4ClStringOld szBufferAux;
  M4RETCODE retdev;

  
  
  retdev = OblGetVersion(&pszVer); 
  if (retdev != EXITO) return retdev; 
  
  StrCpy( szLineaAux ,"VERSION ");
  szLineaAux.strcat(pszVer); 
  szLineaAux.strcat("\r\n");

#if !defined(_WINDOWS) && !defined(_UNIX)
  OBLstrcpy(pszBufferAux,szLineaAux);
#else
  pszBufferAux.strcpy(szLineaAux);
#endif

  retdev = WriteRecursivo(0,pBloque,pszBufferAux);

  if (retdev != EXITO ) return retdev; 
  
  //WriteHandles(szBufferAux):
#if !defined(_WINDOWS) && !defined(_UNIX)
  strcpy(szOut, (char * )pszBufferAux);              
#else
  szOut.strcpy((char * )pszBufferAux);
#endif
  

  
  uPosOut = 0;
  
  
//  delete []pszBufferAux;
  return EXITO;
 }     
 



  
     
/*******************************************************************************
********************************************************************************      
   void WriteHandles(M4ClStringOld & strBuffer)

   Implementacion -> 02 /08 / 95
   
    parametros : strBuffer -> (o) Es donde se escribe los handles que hay actual
                 mente libre
    
    Extracto  :  Dibuja todos los handles que haya
   
********************************************************************************      
********************************************************************************/      
/*      
void M4ClassLOBL::WriteHandles(M4ClStringOld & strBuffer)
  {
                             
                             
                             
    int iNumHandles ;        
    char szAux[300];
    UINT uCont;
    void * ptr;
    M4RETCODE retdev ;
    UINT iTipo;
    
    iNumHandles = pHands -> NumeroHandles();
    szBufferAux[ 0 ] = 0;
    spritf( szAux, "Numero de handles -> %d \n\n",iNumHandles );
    StrCatRight( strBuffer , szAux );
    
    
    for ( uCont = 0  ;  uCont < uNumHandles  ; uCont ++ )
       {
         
         retdev = TipoHandle( iCont , & iTipo);
         if ( retdev ==EXITO  ) 
           { 
             switch ( iTipo )
               {
                   case T_BLOQUE :
                        sprintf (szBuffer,"El handle nº-> %d , Error %u\n",uCont,retdev);
                        StrCatRight( strBuffer , szAux );
                   case T_PROPIEDAD : 
                        sprintf (szBuffer,"El handle nº-> %d , Error %u\n",uCont,retdev);
                        StrCatRight( strBuffer , szAux );
               
               }
           
           
           
           
           }    
         else
           {
             sprintf (szBuffer,"El handle nº-> %d , Error %u\n",uCont,retdev);
             StrCatRight( strBuffer , szAux );
           
           }  
          
         
       
       }
  
  
  }      

  */
  
  
M4RETCODE M4ClassLOBL::WriteFicheroFirst(char *szBuffer,UINT uTamMaxszBuffer,UINT *uTamszBuffer)
 {
   M4RETCODE	retdev;
   UINT			uTamszOut;
   size_t		iTamAux;
   
   // primero actualizo el buffer de salida        // primero actualizo el buffer de salida

   retdev = WriteFicheroCompleto();
   //uTamszOut = strlen(szOut);
  
   // Calculo la longitud  de todo el fichero 
#if !defined(_WINDOWS) && !defined(_UNIX)
   dwTamAux = strlen(szOut);
#else
   iTamAux = szOut.strlen();
#endif
   
   if (iTamAux < (unsigned long )uTamMaxszBuffer) {
   
            // tenemos que el buffer cabe todo
            uTamszOut = (UINT)iTamAux;
            
            // Copio lo que necesito            
#if !defined(_WINDOWS) && !defined(_UNIX)
			OBLstrncpy(szBuffer,( char huge * )szOut,uTamszOut);
#else
			strncpy(szBuffer, ( char huge * )szOut,uTamszOut);
			szBuffer[uTamszOut] = '\0';
#endif

            // doy el tamaño
            *uTamszBuffer = uTamszOut ;
            
            // Vacio la cadena  donde esta  metido el fichero
#if !defined(_WINDOWS) && !defined(_UNIX)
			strcpy(szOut,"");
#else
			szOut.strcpy("");
#endif           
            
            // Pongo a cero la posicion de salida
            uPosOut = 0; 
            
            retdev = EXITO;
   }
   else
      {  
             // Copio solo los caracteres que caben en el buffer             
			strncpy(szBuffer, ( char huge * )szOut,uTamMaxszBuffer-1);
            
             // Cero termino la cadena
             szBuffer[ uTamMaxszBuffer - 1 ] = 0 ;
             
             // Actualizo la posicion de mi buffer donde tengo guardado el fichero
             uPosOut = uTamMaxszBuffer - 1 ;
             
             // Devuelvo la cantidad de caracteres que me ha cogido
             *uTamszBuffer = uTamMaxszBuffer - 1;                  
             
             // Devuelvo que se ha truncado la salida
             retdev = IDS_RES_TRUNCADO;
       
       };      
       
    return retdev;   
        
  }
     

 M4RETCODE M4ClassLOBL::WriteFicheroNext(char *szBuffer,UINT uTamMaxszBuffer,UINT *uTamszBuffer)
  {
               char  huge     *pPos;   
               size_t         iTamszOut;
               UINT           uTamszOut ;
               int            retdev;
    
     
    // Comprobamos que queda algo del fichero que imprimir 
    if (uPosOut == 0) return IDS_RTE_WRITE_PRIMERO;
       
    // Colocamos el puntero en el sitio donde se dejo de imprimir
    pPos = (char huge *)szOut + uPosOut ;        
    
    // Calculo el tamaño que me queda por imprimir    
#if !defined(_WINDOWS) && !defined(_UNIX)
	iTamszOut = strlen(szOut) - uPosOut;
#else
	iTamszOut = szOut.strlen() - uPosOut;
#endif
    
    if (iTamszOut < uTamMaxszBuffer) {
            
            // Caso de que lo que queda es menor que lo que la capacidad del buffer
            uTamszOut = ( UINT )iTamszOut;
            
            // Copio lo que queda
            strncpy (szBuffer,pPos, uTamszOut );
            
            //meto la cantidad  que he guardado en el parametro de salida 
            *uTamszBuffer = uTamszOut ;
            
            // Cero termino la cadena
            szBuffer[*uTamszBuffer]= 0;
            
            // Vacio el buffer donde tengo guardado el fichero            
#if !defined(_WINDOWS) && !defined(_UNIX)
			strcpy(szOut,NULL);
#else
			szOut.strcpy((char *)NULL);
#endif

            uPosOut = 0; 
            
            retdev =  EXITO;
   }
   else {                                          
        
             // Copio solo lo que cabe en el buffer
             strncpy(szBuffer,pPos,uTamMaxszBuffer-1);
             
             // Cero termino la cadena
             szBuffer[uTamMaxszBuffer - 1] = 0 ;
             
             // Actualizo la posicion donde he dejado la devolucion del buffer
             uPosOut =  uPosOut + uTamMaxszBuffer - 1 ;
             
             //Guardo en el parametro de salida la cantidad que devuevo 
             *uTamszBuffer = uTamMaxszBuffer - 1;
             
             retdev = IDS_RES_TRUNCADO;
   };
       
   return retdev;     
  
  };

#if !defined(_WINDOWS) && !defined(_UNIX)
	M4RETCODE M4ClassLOBL::WriteBloque(UINT uHandBlq,M4ClHString & pszBufferout)
#else
	M4RETCODE M4ClassLOBL::WriteBloque(UINT uHandBlq,M4ClStringOld & pszBufferout)
#endif

 {           
   M4ClassBloque *pBlqRes;
   void *ptr;           
   M4RETCODE retdev;
   
   // consigo el puntero del bloque
   retdev=pHands->GetPtrBlq(uHandBlq,&ptr);
   if (retdev!=EXITO) return retdev;   
   pBlqRes =(M4ClassBloque *)ptr;
 
   
   //pszBufferout[0] = 0;
   retdev = WriteRecursivo(0,pBlqRes,pszBufferout);
   if (retdev != EXITO ) return retdev;
   
   return EXITO;
 }


/******************************************************************************/
/******************************************************************************/
//      FUNC. MIEMBROS QUE GENERA EL BLOQUE A PARTIR DEL FICHERO                                                                                
/******************************************************************************//******************************************************************************/
/******************************************************************************/                                                                                


char *M4ClassLOBL::NextPal()
 {  
   char *pszpalabra;
   M4RETCODE retdev;
   
   retdev = pAnaliz->NextPalabra(&pszpalabra);
   if (retdev!=EXITO) return "";  
   return pszpalabra;
 };                 
 
 
 
M4RETCODE M4ClassLOBL::SaltoLinea()
{
 char *pszpalabra;     
 M4RETCODE retdev;
 
 pszpalabra = NextPal();
 if (strcmp(strupr(pszpalabra),"")==0) return IDS_RTE_NO_ENCONTRADO_REM;
 if (strcmp(strupr(pszpalabra),"")==0) return IDS_RTE_NO_ENCONTRADO_REM;
 if (strcmp(strupr(pszpalabra),"REM")==0) 
   {
     do 
       {
         pszpalabra = NextPal();
         if (strcmp(strupr(pszpalabra),"")==0) return IDS_RTE_NO_FIN_LINEA;
       } while (strcmp(pszpalabra,STR_FIN_LINEA)!=0); 
    }
  else // NO HA ENCONTRADO NULL , ENTONCES TIENE QUE SER SALTO CARRO 
      if (strcmp(pszpalabra,STR_FIN_LINEA)!=0) return IDS_RTE_NO_ENCONTRADO_REM;
 
 retdev = pAnaliz->SaltaLineasBlancas();
 if (retdev!=EXITO && retdev!=FINFICH) return retdev;
 
 pszpalabra=NULL;
 return EXITO;
};
 


M4RETCODE M4ClassLOBL::GeneraObl(char *psztexto)
{                             
   /* 
     si devuelve:
        2180 -> implica que el analizador ya esta asignado con una cadena*/
        
        
  M4RETCODE retdev; 
  char *pszpalabra;
  char *szversion;
  int cont=0;
  
  szversion=new(char[MAX_TAM_IDENT]);
  
  if (bdefbloq) return IDS_RTE_BLQ_CONSTRUIDO;     // YA SE HA CONSTRUIDO EL BLOQUE Y NO SE PUEDE EJECUTAR DENUEVO
  retdev=OblSetFich(psztexto);       // INICIALIZO AL FICHERO
  
  if (retdev!=EXITO) return retdev;
  retdev = OblSetFich("\n"); 
  if (retdev != EXITO ) return retdev;
  pHands=new(M4AHandles);  
  
  // una vez que tengo  el array de handles y el analizador inicializados empiezo la lectura
  // etapa de la recuperacion de la version
  //retdev = pAnaliz -> SaltaLineasBlancas (); // salta todas las lineas en blanco que haya al principio  
  pszpalabra=NextPal();
  if ( strcmp(pszpalabra,"")==0 || strcmp("VERSION",strupr(pszpalabra))!=0)
         {
          // modificacion hecha por jmms 18 /07 /95
          // consigo la posicion acutal y destruyo el analizador
          pAnaliz -> GetPosicionActual( & m_lPosicionError , & m_lLineaError ,& m_lOffSetError );
          delete []szversion;  
          szversion = NULL ;
          delete pAnaliz;
          pAnaliz = NULL;
          // fin modificacion 
          return IDS_RTE_VERSION;
         }
          
  pszpalabra = NextPal();
  if (strcmp(pszpalabra,"")==0)
       {
          // modificacion hecha por jmms 18 /07 /95
          // consigo la posicion acutal y destruyo el analizador
          pAnaliz -> GetPosicionActual( & m_lPosicionError , & m_lLineaError ,& m_lOffSetError );
          delete []szversion;
          szversion = NULL;
          delete pAnaliz;  
          pAnaliz = NULL;
          // fin modificacion 
    
         return IDS_RTE_VERSION;
       }
       
  strcpy(szversion,pszpalabra);
  hTask = GetCurrentTask();
  // una vez que tengo la version localizada ya tengo todos los elementos para crearme el objeto obl
  OblSetVersion(szversion);
   
  //hago el salto linea , esto es saltarpszpalabras hasta que el analizador encuentre el endlinea   
  
  retdev=SaltoLinea();
  if (retdev!=EXITO) return retdev;  

  // comprobamos que esta el begin
  pszpalabra=NextPal();                   
  
  if (strcmp(pszpalabra,"")==0 || strcmp(strupr(pszpalabra),"BEGIN")!=0) 
      { 
        // modificacion hecha por jmms 18 /07 /95
        // consigo la posicion acutal y destruyo el analizador
        pAnaliz -> GetPosicionActual( & m_lPosicionError , & m_lLineaError ,& m_lOffSetError );
        delete []szversion;
        szversion = NULL;
        delete pAnaliz;
        pAnaliz = NULL;
        // fin modificacion 
        return IDS_RTE_NOBEGIN;
      }  

                      
  // me alloco el bloque
  pBloque = new(M4ClassBloque);  
   if (pBloque==NULL) return IDS_RTE_NO_MEMORIA;   
  
  // ya nos hemos generado un bloque y tenemos que construirlo
  
  retdev = GeneraOblBloque(pBloque);
  
 // modificacion hecha por jmms 13 /07 /95
  if (retdev!=EXITO)
   {      
      // consigo la posicion acutal y destruyo el analizador
      pAnaliz -> GetPosicionActual( & m_lPosicionError , & m_lLineaError ,& m_lOffSetError );
      delete []szversion;
      delete pAnaliz;
      delete pBloque;  
      pBloque = NULL;
      pAnaliz = NULL;
      return retdev;
   }   
   
  
  bdefbloq = TRUE;
  // una vez que ya hemos reconocido es bloque borramos la cadena que hemos usado
  
  delete []szversion;
  delete pAnaliz; 
  pAnaliz = NULL;
  pszpalabra=NULL;
 
  return EXITO;
  
  
  
};
             
    
    
M4RETCODE M4ClassLOBL::GeneraOblBloque(M4ClassBloque *pBloque)
{            
   
  char *pszclase,*pszhinstancia;
  char *pszpalabra;
  UINT uhandle,uposicion;
  M4ClassBloque *pBloqueFuturo,*pBloqueaux;
  M4ClassPropiedad *ppropiedad;
  M4RETCODE retdev;
  int cont=0;      
  int iTipo;
      
  pszclase = NULL;
  pszhinstancia=NULL;  
  pszpalabra =NULL;  
  pszclase = new(char[MAX_TAM_IDENT]);
  pszhinstancia = new(char[MAX_TAM_PROP]);
  
   // ahora tenemos que conseguir la Clase de Bloque que es
   pszpalabra = NextPal(); 
   if (strcmp(pszpalabra,"")==0) 
    {   
        //Modificado Enrique
        delete []pszclase; delete []pszhinstancia ;
        return IDS_FALTA_CLASE;
    }
   strcpy(pszclase,pszpalabra);
  
  // ahora tenemos que conseguir la Histancia del Bloque
  pszpalabra=NextPal(); 
  if (strcmp(pszpalabra,"")==0) 
    {   
        //Modificado Enrique
        delete []pszclase; delete []pszhinstancia ;
        return IDS_FALTA_INST;
    }    
  strcpy(pszhinstancia,pszpalabra);
  
  // hago el salto linea
  retdev = SaltoLinea();
  if (retdev!=EXITO)
    {   
        //Modificado Enrique
        delete []pszclase; delete []pszhinstancia ;
        return retdev;  
    }
  
  
  // doy valor a la clase y meto el punetero de esta clase en la estructura de los handles
  // meto el handle
  retdev = pHands->AddHandle(T_BLOQUE,pBloque,&uhandle);
  if (retdev!=EXITO)
    {   
        //Modificado Enrique
        delete []pszclase; delete []pszhinstancia ;
        return retdev;
    }

  retdev = pBloque->M4CBSetBloque(pszclase,pszhinstancia,uhandle,&pBloqueaux);
  pBloqueaux = NULL;                                                                   
  if (retdev!=EXITO) 
    {   
        //Modificado Enrique
        delete []pszclase; delete []pszhinstancia ;
        return retdev;
    }
  
  // una vez que tengo ya actualizado el bloque me pongo añadir pijadas como las propiedades y
  // y los bloques que estan dentro
  
  pszpalabra=NextPal(); 
  if (strcmp(pszpalabra,"")==0)
    {   
        //Modificado Enrique
        delete []pszclase; delete []pszhinstancia ;
         return IDS_FIN_OBL_ERRONEO; 
    }
  
  while (strcmp(strupr(pszpalabra),"END")!=0)
   {
     if (strcmp(strupr(pszpalabra),"BEGIN")==0)
      { 
        // si he entrado por aquí quiere decir que estoy en un bloque 
        retdev=pBloque->M4ABAddNuevoBlq(pBloque,&pBloqueFuturo);
        if (retdev!=EXITO) 
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                return retdev;
            }
        
        // una vez que nos hemos alocado el nuevo bloque y la cosa ha ido bien lo que hago es
        // hacer una llamada recursiva para generame el bloque de dentro
                  
         retdev = GeneraOblBloque(pBloqueFuturo);
         if (retdev!=EXITO)
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                 return retdev;
            }
         
      }
    else 
    if (strcmp(strupr(pszpalabra),"REM") == 0)
      {
       // En caso de comentario lo que tengo que hacer es ignorar todo y saltarme todo hasta la
       // que encuentre el salto carro
       do 
         {
           pszpalabra = NextPal();
           if (strcmp(strupr(pszpalabra),"")==0) 
              {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                return IDS_RTE_NO_FIN_LINEA;
              }
         } while (strcmp(pszpalabra,STR_FIN_LINEA)!=0); 
       // una vez que he encotrado el salto carro salto todas las linea en blanco que encuentro
       retdev = pAnaliz->SaltaLineasBlancas();
       if (retdev!=EXITO && retdev!=FINFICH) 
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                return retdev;
            }
      }
    else                                   
      {                                                                         
      
        strcpy(pszclase,pszpalabra); // en este buffer he guardado el comentario
        
        // en este momento me acabo de leer la propiedad
        pszpalabra=NextPal(); 
        if (strcmp(pszpalabra,"")==0 || strcmp(pszpalabra,"=")!=0)
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                return IDS_RTE_ESPERAIGUAL ;
            }
        
        
        
   // #define __CONSEGUIRVALORPROPIEDAD   
    #ifndef __CONSEGUIRVALORPROPIEDAD
       // ahora leo el valor de la siguiente palabra
        pszpalabra = NextPal();    
       if (strcmp(pszpalabra,"")==0)
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                return IDS_FIN_OBL_ERRONEO;
            }
       if (strcmp(pszpalabra,"\"")==0)
           { 
            // si entra por aqui es que es una cadena
            // Con la fucion Hasta la comilla lo que hace es devolverme todo lo que haya
            // hasta encontrar la siguiente comilla
            retdev=pAnaliz->Hastalacomilla(&pszpalabra);
            if (retdev != EXITO )
                {   
                    //Modificado Enrique
                    delete []pszclase; delete []pszhinstancia ;
                     return retdev; 
                }
            
            strcpy(pszhinstancia,pszpalabra);                    
            iTipo = T_PRP_STR;
            pszpalabra=NextPal();
            
            if (strcmp(pszpalabra,"\"")!=0) 
                    {   
                        //Modificado Enrique
                        delete []pszclase; delete []pszhinstancia ;
                        return IDS_RTE_NO_COMILLAS;
                    }
           }
       else 
           {
              if (stricmp(pszpalabra,"(d)") == 0 )
                 {
                  pszpalabra = NextPal();
                  if (strcmp(pszpalabra,"\"")==0)
                     { 
                      // si entra por aqui es que es una cadena
                      // Con la fucion Hasta la comilla lo que hace es devolverme todo lo que haya
                      // hasta encontrar la siguiente comilla
                      retdev=pAnaliz->Hastalacomilla(&pszpalabra);
                      if (retdev != EXITO ) 
                            {   
                                //Modificado Enrique
                                delete []pszclase; delete []pszhinstancia ;
                                return retdev; 
                            }
                      
                      strcpy(pszhinstancia,pszpalabra);                    
                      iTipo = T_PRP_FEC;
                      pszpalabra=NextPal();
                      if (strcmp(pszpalabra,"\"")!=0)
                            {   
                                //Modificado Enrique
                                delete []pszclase; delete []pszhinstancia ;
                                 return IDS_RTE_NO_COMILLAS;
                            }
                     } 
                 }    
                else     
                 {
                   strcpy(pszhinstancia,pszpalabra);
                   iTipo = T_PRP_INT;
                 }
                      
           };  
   #else   

       // Calcula el valor de la propiedad 
       retdev = ConsegirValorPropiedad ( pszhinstancia ,MAX_TAM_PROP , & iTipo );
       if ( retdev != EXITO )
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                 return retdev ;
            }

   #endif        
       
        
        // me alloco una propiedad vacia para obtener el puntero
        retdev = pBloque->M4APAddNuevaPropiedad(&ppropiedad,&uposicion,pBloque);
        if (retdev!=EXITO) 
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                return retdev;
            }
        
        // añado el puentero al array de handles y este al añadirlo me devuelve uno
        retdev = pHands->AddHandle(T_PROPIEDAD,ppropiedad,&uhandle);
        if (retdev!=EXITO) 
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                return retdev;
            }
        
        // asgino los valores correspondientes a la propiedad
        retdev = pBloque->M4APSetPropiedadn(uposicion,pszclase,pszhinstancia,uhandle,iTipo);
        uposicion=0;
        if (retdev!=EXITO) 
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                return retdev;
            }
        
        // hago el salto linea
        retdev=SaltoLinea();
        if (retdev!=EXITO)
            {   
                //Modificado Enrique
                delete []pszclase; delete []pszhinstancia ;
                 return retdev;    
            }
      };
     pszpalabra=NextPal(); 
     if (strcmp(pszpalabra,"")==0) 
        {   
            //Modificado Enrique
            delete []pszclase; delete []pszhinstancia ;
            return IDS_FIN_OBL_ERRONEO; 
        }
   };
  // hago el salto linea
  retdev=SaltoLinea();
  if (retdev!=EXITO) 
    {   
        //Modificado Enrique
        delete []pszclase; delete []pszhinstancia ;
        return retdev;                         
    }
                   
   // desaloco y pongo a null todo lo usado de forma auxiliar en este procedimiento                
  delete  []pszclase;
  delete  []pszhinstancia;   
  pszpalabra = NULL;
  pBloqueFuturo = NULL;
  pBloqueaux = NULL;
  ppropiedad = NULL;
  
  return EXITO;
};
     
/*******************************************************************************
********************************************************************************      
M4RETCODE ConsegirValorPropiedad ( char ** pszpalabra , int  * iTipo )

   Implementacion -> 13 / 07 / 95
   
    parametros : pszpalabra -> ( O ) Donde devuelvo el valor de la propiedad
                 iTipo -> ( O ) Donde dejo el valor del tipo 
                 
    Entrada  : Cuando se entra a esta funcion se ha testeado el igual y tenemos
               el puntero del buffer al sigiente caracter despues del igual
               a =  (d)"12-12-12"
                  ^puntero
    
    Extracto  :La salida de esta funcion tiene que se en pszpalabra el valor de
               la propiedad y en iTipo su tipo y encaso de tener exito devolver
               EXITO      
               Para ampliaciones de tipos solo hay que añadir en donde lo indica
********************************************************************************      
********************************************************************************/      


M4RETCODE M4ClassLOBL::ConsegirValorPropiedad ( char * szBuffer ,int iBufferLen, int  * iTipo )
 {
  M4RETCODE    retdev; 
  char         cSiguiente;
  char         cTipo; 
  char         *pszpalabra;     
  char         szActual[2];
  
  
  // primer caracter no nulo despues del "="
  retdev = pAnaliz -> NextCarNoNull ( & cSiguiente );
  if (retdev!=EXITO) return retdev;    
  
  
  switch ( cSiguiente )
   {
      case '\"':
            // CASO QUE LO QUE ENCUENTRE SEA UNA CADENA DE CARACTERES
               retdev = pAnaliz -> Hastalacomilla ( & pszpalabra );
               if (retdev!=EXITO) return retdev; 
               *iTipo = T_PRP_STR;
               
               retdev = pAnaliz -> NextCarNoNull ( & cSiguiente );
               if (retdev != EXITO ) return retdev ; 
               if ( cSiguiente != '\"') return IDS_RTE_NO_COMILLAS;
               
               strncpy ( szBuffer, pszpalabra , iBufferLen - 1  );
               szBuffer[iBufferLen - 1] = '\0';
                         
               break;
     

      case '(' :
             // CAO DE QUE ESTE TIPADO SE HACE CASTING
               // Consigo el tipo
               retdev = pAnaliz -> NextCarNoNull  ( & cTipo );
               if ( retdev != EXITO ) return retdev ; 
               
               // compruebo que se cierra el parentesis
               retdev = pAnaliz -> NextCarNoNull  ( & cSiguiente );
               if ( retdev != EXITO ) return retdev ; 
               if ( cSiguiente != ')' ) return IDS_RTE_CASTING;     
               
               // lo que viene despues es el valor etre comillas
               
               // compruebo las primeras comillas
               retdev = pAnaliz -> NextCarNoNull  ( & cSiguiente );
               if ( retdev != EXITO ) return retdev ;
               if ( cSiguiente != '\"') return IDS_RTE_NO_COMILLAS;
               
               // consigo el valor
               
               retdev = pAnaliz -> Hastalacomilla ( & pszpalabra );
               if (retdev!=EXITO) return retdev;                    
                             
               strncpy ( szBuffer, pszpalabra , iBufferLen - 1  );
               szBuffer[iBufferLen - 1] = '\0';

               // compruebo las siguientes comillas 
     
               retdev = pAnaliz -> NextCarNoNull  ( & cSiguiente );
               if (retdev != EXITO ) return retdev ; 
               if ( cSiguiente != '\"') return IDS_RTE_NO_COMILLAS; 
               
               // ahora que todo ha ido bien añado los demas tipos
               // en caso de ampliacion de tipos solo hay que modificar esto
        
               switch ( cTipo )
                 {
                   case 'd' : 
                   case 'D' :
                       *iTipo = T_PRP_FEC ;
                       break;
                   default :
                        return IDS_RTE_CASTING;
                 } 
               break;
          
          
         default:  
              // POR DEFECTO ES UN ENTERO O NUMERICO 
               szBuffer[0] = cSiguiente;
               szBuffer[1] = '\0' ;
               retdev = pAnaliz->CurrentCar( & szActual[ 0 ] );
               
               szActual [ 1 ] = '\0';   
               
               // en caso de que lo que haya encontrado es un numero
               if (strcspn ( "1234567890,.-", & szActual[ 0 ] ) < strlen(pszpalabra) ) 
                 {   
                    // solo hay numeros
                    pszpalabra = NextPal ();
                    strncpy ( &szBuffer[1], pszpalabra , iBufferLen - 2  );
                    szBuffer[iBufferLen - 2] = '\0';
                  };   
               
               *iTipo = T_PRP_INT;

   }
 
  return EXITO;
 }    

// QUEDA LOS CODIGOS DE ERROR:

    
M4RETCODE M4ClassLOBL::ReparaObjeto(M4ClassBloque *pBlq)
 {     
   UINT uContador;      
   
   UINT uNumObj;
   UINT uHand;
   M4ClassPropiedad *pPrp;
   M4ClassBloque *pBloque;
   BOOL bReparado;           
   
   // REPARA EL OBJETO ACTUAL                   
   pBlq->M4CBGetHandle(&uHand);
   bReparado = pHands->RepararHand(uHand,pBlq,T_BLOQUE);
   // CALCULO EL NUMERO DE PROPIEDADES
   uNumObj = pBlq->NumPropiedades();
   uContador = 0;                
   // PARA CADA UNA DE LA PROPEIDADES
   while (uContador < uNumObj )
     { 
       // CONSIGO SU HANDLE
       pBlq->M4APGetHandn(uContador,&uHand);
       // CONSIGO EL PUNTERO
       pPrp  = pBlq->M4APGetpProp(uContador);
       // LA REPARO O LA COMPRUEBO
       bReparado = pHands->RepararHand(uHand,pPrp,T_PROPIEDAD);
       if (!bReparado) return IDS_RTE_REPARAR;
       uContador++;
     }; 
                                  
   // CONSIGO EL NUMERO DE BLOQUES QUE HAY                               
   uNumObj = pBlq->NumBloques();                
   // PARA CADA BLOQUE 
   while (uContador<uNumObj)
     {           
       // CONSIGO EL PUENTERO  
       pBloque = pBlq->PtrBloquen(uContador);
       // SI EL PUNTERO NO ES NULLO LO REPARO
       if (pBloque) ReparaObjeto(pBloque);
       uContador++;
     };
   return EXITO;  
 }
 

M4RETCODE  M4ClassLOBL::DelHandRama(M4ClassBloque *pBlq)
 {                            
   UINT uContador;    
   UINT uNumObj;
   UINT uHand;       
   M4RETCODE retdev;
  
   M4ClassBloque *pBlqAux;
   BOOL bReparado = TRUE;    
   // Borrar las propiedades 
   uNumObj = pBlq->NumPropiedades();
   uContador = 0; 
                  
   while (bReparado && uContador < uNumObj )
     { 
       retdev = pBlq->M4APGetHandn(uContador,&uHand);
       if (retdev != EXITO ) return retdev;
       retdev = pHands->DelPtrProp(uHand);
       if (retdev != EXITO ) return retdev;
       uContador++;
     }; 
                                  
   uNumObj = pBlq->NumBloques();                
   uContador = 0;
   while (uContador<uNumObj)
     {           
       pBlqAux = pBlq->PtrBloquen(uContador);
       if (pBlqAux) 
           { 
             retdev = DelHandRama(pBlqAux);
             if (retdev != EXITO ) return retdev;
           }
       
       uContador++;
     };     
   retdev = pBlq->M4CBGetHandle(&uHand);  
   if (retdev != EXITO ) return retdev;
   retdev = pHands->DelPtrBlq(uHand);  
   if (retdev != EXITO ) return retdev;
   
   return EXITO;  
 };       

               
               
M4RETCODE M4ClassLOBL::OblDelRama(char *szPath)
 {  
  M4ClassBloque *pBlqaBorrar,*pPadre;
  char *szHinst,*szClass;
  M4RETCODE retdev;
  
  // CONSIGO EL PUNTERO DEL PATH DADO
  retdev = OblGetBlqPath(szPath,&pBlqaBorrar);
  if (retdev != EXITO) return retdev;
  // CONSIGO LA INSTANCIA
  retdev = pBlqaBorrar->M4CBGetHinst(&szHinst);
  if (retdev != EXITO) return retdev;
  // LA CLASE
  retdev =pBlqaBorrar ->M4CBGetClass(&szClass);
  if (retdev != EXITO) return retdev;
  //MIRO QUIEN ES EL PADRE  
  // Hasta aqui todo va bien hace lo que tiene que hacer 
  retdev = pBlqaBorrar->M4CBGetPadre(&pPadre);
  if (retdev != EXITO) return retdev;    
  
  // SI ES EL FICHERO RAIZ NO LO BORRO
  if (!pPadre) return IDS_OBL_BORRAR_RAIZ;
  // BORRO LA RAMA DE HANDLES
  retdev = DelHandRama(pBlqaBorrar);
  //if (retdev != EXITO ) 
  //    { 
  //     ReparaObjeto(pBlqaBorrar);
  //     return retdev;
  //    };
  pPadre->M4ABlDelBlqn(szClass,szHinst);
  return EXITO;
 };    

     
/*******************************************************************************
********************************************************************************      
M4RETCODE OblGetPoscion(DWORD lPos,DWORD lLinea,DWORD lOffSet);

   Implementacion -> 13 / 07 / 95
   
    parametros :  lPos     ->  posicion actual
                  lLinea   ->  linea actual cuando se ha cometido un error
                  lOffSet  ->  desplazmiento actual en la linea
                  
    Extracto  :  Esta funcion es de depuracion y solo se hace cuando se ha 
                 cometido un error al crearse el obj
   
********************************************************************************      
********************************************************************************/      
                                           
M4RETCODE M4ClassLOBL::OblGetPoscionError(DWORD * lPos, DWORD  * lLinea,DWORD  * lOffSet)
  {
    *lPos = m_lPosicionError;
    *lLinea = m_lLineaError;
    *lOffSet = m_lOffSetError;
    return EXITO;
  }
  


M4RETCODE M4ClassLOBL::OblCpyBlq(M4ClassBloque *pBlqSource,M4ClassBloque *pBlqCopia )
 { 
   /** 
     HA ESTA FUNCION SE LE PASA UN PUNTERO CON EL BLOQUE QUE QUEREMOS COPIAR Y 
     OTRO PUNTERO CON UN BLOQUE ALLOCADO  PARA COPIAR TODOS LOS DATOS 
     Y CON EL PADRE YA ASIGNADO
   */ 
 
 
   M4ClassBloque *pAux,*pSourceAux; 
   M4ClassPropiedad *pProp,*pPrpAux;   
   char *szHinst,*szClass; 
   char *szNomb,*szVal;
   int iTipo;
   UINT uNumObj,uCont; 
   UINT uHand,uPos;    
   M4RETCODE retdev;
   
   // conseguimos los datos del bloque fuente  
   retdev = pBlqSource -> M4CBGetHinst(&szHinst);
   if (retdev != EXITO) return retdev;
   
   retdev = pBlqSource -> M4CBGetClass(&szClass);
   if (retdev != EXITO) return retdev;  
   
   // añado un handle con este puntero    
   retdev = pHands->AddHandle(T_BLOQUE,(void *)pBlqCopia,&uHand) ;
   if (retdev != EXITO) return retdev;
                                      
   // asigno valores
   retdev = pBlqCopia -> M4CBSetBloque(szClass,szHinst,uHand,&pAux);
   if (retdev != EXITO) return retdev;
  
   
   // ahora Copio todas las todas las propiedades
   
   uNumObj = pBlqSource->NumPropiedades();
   for (uCont=0 ; uCont< uNumObj;uCont ++ )
    {
      pProp = pBlqSource ->M4APGetpProp(uCont); 
      
      retdev = pProp -> M4CPGetPropiedad(&szVal);
      if (retdev != EXITO) return retdev;
      
      retdev = pProp -> M4CPGetNomPropiedad(&szNomb);
      if (retdev != EXITO) return retdev;
      
      retdev = pProp -> M4CPGetTipoPropiedad(&iTipo);
      if (retdev != EXITO) return retdev;
      
      // me acabo de allocar una propiedad
      retdev = pBlqCopia -> M4APAddNuevaPropiedad(&pPrpAux,&uPos,pBlqCopia); 
      if (retdev != EXITO) return retdev; 
      
      // consigo el handle
      retdev = pHands->AddHandle(T_PROPIEDAD,(void *)pPrpAux,&uHand) ;
      if (retdev != EXITO) return retdev;
      
      // asigno los valores a la propiedad uPos que ya la tenemos allocada
      retdev = pBlqCopia->M4APSetPropiedadn(uPos,szNomb,szVal,uHand,iTipo); 
      if (retdev != EXITO) return retdev;
      
            
    }; 
    
    // ahora me toca copiarme todos los Bloques
    uNumObj = pBlqSource -> NumBloques();
    
    for (uCont = 0;uCont < uNumObj; uCont++)
     {
                                       
       // CONSIGO EL PRIMER CADA UNO DE LOS PUNTEROS DEL BLOQUES DEL ARRAY                                
       retdev = pBlqSource-> M4ABlGetpBlqn(uCont,&pSourceAux);
       if (retdev != EXITO) return retdev;
       
       retdev = pBlqCopia->M4ABAddNuevoBlq(pBlqCopia,&pAux);
       if (retdev != EXITO) return retdev;
                                            
       //copio recursivamente esta ramas                                     
       retdev = OblCpyBlq(pSourceAux,pAux);
       if (retdev != EXITO) return retdev;
       
     }                               
     
     
     
     return EXITO;
   
 
 };    
    

M4RETCODE OblCpy(M4ClassLOBL *pOblSource,M4ClassLOBL **pOblDestino,UINT hId) 
 {
   
   M4RETCODE retdev;
   M4ClassLOBL *pOblAux;
   char *szVer,*szNombF,*szExtF,*szTit,*szBase;
   t_PID hTask;
                
   // Consigo los datos del obl fuente que me ha pasado
   retdev = pOblSource -> OblGetVersion(&szVer);
   if (retdev != EXITO) return retdev;
   
   retdev = pOblSource -> OblGetNombFich(&szNombF);
   if (retdev != EXITO) return retdev;
   
   retdev = pOblSource -> OblGetExtFich(&szExtF);
   if (retdev != EXITO) return retdev;
   
   retdev = pOblSource -> OblGetTitulo(&szTit);
   if (retdev != EXITO) return retdev;
   
   retdev = pOblSource -> OblGetBase(&szBase);
   if (retdev != EXITO) return retdev;
                   
                   
  
   // Me creo un obl auxiliar
   pOblAux = new M4ClassLOBL;           
   if (!pOblAux) return IDS_RTE_NO_MEMORIA;
   
     
   // Asigno al obl nuevo los datos nuevos  
   retdev = pOblAux->SetObl(szVer,szNombF,szExtF,szTit,szBase,hId);
   if (retdev != EXITO) return retdev;
       
   // Consigo la tarea actual    
   hTask = GetCurrentTask(); 
   // Asigno la tarea actual al obl
   retdev = pOblAux -> OblSetTask(hTask);
   
   // me alloco un bloque que es donde va a ir todo el cotarro
   pOblAux->pBloque = new M4ClassBloque;
   
   // Ademas me creo un array el array de handles en el cacharrin
   pOblAux->pHands = new M4AHandles;

   // Hago la copia de los bloques
   retdev = pOblAux -> OblCpyBlq(pOblSource->pBloque,pOblAux->pBloque);
   if (retdev != EXITO) return retdev;
   
   *pOblDestino = pOblAux;
   return EXITO;
   
 
 };
        
      
BOOL OblCmp(M4ClassLOBL *pObl1,M4ClassLOBL *pObl2)           
 {
   
   M4RETCODE retdev;
   char *szCmp1,*szCmp2;
                
   // comparamos la version             
   retdev = pObl1 -> OblGetVersion(&szCmp1);
   if (retdev != EXITO) return FALSE;
   retdev = pObl2 -> OblGetVersion(&szCmp2);
   if (retdev != EXITO) return FALSE;
   
   if (strcmp(szCmp1,szCmp2) != 0 ) return FALSE;
   
   
   // comparamos el nombre de los ficheros
   retdev = pObl1 -> OblGetNombFich(&szCmp1);
   if (retdev != EXITO) return FALSE;
   retdev = pObl2 -> OblGetNombFich(&szCmp2);
   if (retdev != EXITO) return FALSE;       
   
   if (strcmp(szCmp1,szCmp2) != 0 ) return FALSE;
   
   // comparamos la extension de los ficheros
   
   retdev = pObl1 -> OblGetExtFich(&szCmp1);
   if (retdev != EXITO) return FALSE;
   retdev = pObl2 -> OblGetExtFich(&szCmp2);
   if (retdev != EXITO) return FALSE;        
   
   if (strcmp(szCmp1,szCmp2) != 0 ) return FALSE;
   
   // comparamos el titulo de los ficheros
   
   retdev = pObl1 -> OblGetTitulo(&szCmp1);
   if (retdev != EXITO) return FALSE;
   retdev = pObl2 -> OblGetTitulo(&szCmp2);
   if (retdev != EXITO) return FALSE;      
   
   if (strcmp(szCmp1,szCmp2) != 0 ) return FALSE;
   
   
   // comparamos la base de ambos dos ficheros
   retdev = pObl1 -> OblGetBase(&szCmp1);
   if (retdev != EXITO) return FALSE;
   retdev = pObl2 -> OblGetBase(&szCmp2);
   if (retdev != EXITO) return FALSE;            
   
   if (strcmp(szCmp1,szCmp2) != 0 ) return FALSE;                
   
   return BlqCmp(pObl1->pBloque,pObl2->pBloque);
   
 };
 
 
 // FIN DE LA IMPLEMATACION DEL OBJETO OBL
 
 
 //*************************************************************** 
 // FUNCION PARA TONI
 //*************************************************************** 
 M4RETCODE M4ClassLOBL::OblFindHandBlq  ( UINT ai_uHandBlq , 
                                          char *  ai_pszNombreClase  , 
                                          int & aio_iPosicion ,
                                          UINT &  ao_uHandBlq )
                                          
   {
    
          void *pBlq=NULL;   
           M4ClassBloque  *pBase , *pComponente;
           char * pszClase ;
           char * pszInstance;
           int retdev;             
           M4ClStringOld strInstance;  
           M4ClStringOld strClass;
           
       // primero compruebo si viene formato CLASE.INSTANCIA.
       // O bien ver si viene en formato CLASE
       // Si viene en formato CLASE se le concatena .* 
       char * ptrToken;
       ptrToken = strchr ( ai_pszNombreClase , '.' );
       
       if ( ptrToken == NULL ) {
        
        	strInstance.strcpy ( "*" ); 
        	strClass.strcpy( ai_pszNombreClase );
        
       }
       else {              
       		
       	
       		*ptrToken = '\0';
       	
       		strInstance.strcpy ( ptrToken + 1 );
       		
       		strClass.strcpy ( ai_pszNombreClase );
       		
       		*ptrToken = '.';
       }
           
    
       // Consigo el puntero al bloque
       retdev=pHands->GetPtrBlq(ai_uHandBlq,&pBlq);
       if (retdev!=EXITO) return retdev;
       
       pBase  = ( M4ClassBloque * )pBlq;   
       
       //aumento la posicion en uno
       aio_iPosicion ++;
       
       do 
        {   
            // Consigo el n-esimo bloque del array
            retdev = pBase -> M4ABlGetpBlqn( aio_iPosicion , &pComponente ); 
            
            if ( retdev != EXITO ) {
               
                return retdev;
            }    
              
            // Consigo una clase de ese bloque   
            retdev = pComponente -> M4CBGetClass ( &pszClase );  
            
            if ( retdev != EXITO ) {
               
                return retdev;
            }    
            
            // Consigo la Instancia de este bloque
            
            retdev = pComponente -> M4CBGetHinst ( &pszInstance );  
            
            if ( retdev != EXITO ) {
               
                return retdev;
            }      
            
            // Comparo el nombre que me han pasado con la clase obtenida
            if ( MatchCmp ( strClass , pszClase ) && MatchCmp ( strInstance , pszInstance ) ) {
            
                return pComponente -> M4CBGetHandle( &ao_uHandBlq );
            
            }    
            
            // En caso de no encontrarla aumento en uno la posicion
            aio_iPosicion++;
       

        }   while ( aio_iPosicion < pBase -> NumBloques() );                                    
 
      return IDS_RTE_ABLQ_NO_FOUND;   }






//  NUEVO CANARIO                                                     

 // Si el ai_TipoSearch = 0 	Bloque
 //       ai_TipoSearch = 1		Propiedad

M4RETCODE M4ClassLOBL::OblKillSearch ( UINT ai_uHandMatch , int ai_iTipoSearch )
 {  
  		M4RETCODE retdev ;
  	
	if ( ai_iTipoSearch == 0 ) {  	
  	
  		retdev = pAMBlq -> AMBorrar( ai_uHandMatch );
  	}
  	else {	
  	
  		retdev = pAMProp -> AMBorrar( ai_uHandMatch );
  	} 
  	
  	return retdev;
  	
 }
 
//  FIN NUEVO CANARIO         


